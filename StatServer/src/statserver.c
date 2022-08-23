/*\
 *
 *    StatServer.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Single shard-wide server to keep track of global list of 
 *  stats
 *
 */


#include <utilitieslib/network/netio.h>
#include "dbcomm/dbcontainer.h"
#include "bases/baseupkeep.h"
#include "dbcomm/dbcomm.h"
#include "logserver.h"
#include "statdb.h"
#include "Entity/stat_SgrpBadges.h"
#include "player/badges.h"
#include "entity/SgrpBadges.h"
#include "entity/SgrpStats.h"
#include "dbserver/servercfg.h"
#include "comm_backend.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/network/sock.h>
#include <stdio.h>
#include <conio.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/version/AppVersion.h>
#include "dbcomm/dbcomm.h"
#include "container/containerArena.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/network/net_linklist.h>
#include <utilitieslib/utils/sysutil.h>
#include <math.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/language/MultiMessageStore.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/components/StashTable.h>
#include "entity/load_def.h"
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/utils/winfiletime.h>
#include "entity/RewardToken.h"
#include "entity/Supergroup.h"
#include "container/containerSupergroup.h"
#include "entity/SgrpServer.h"
#include "dbcomm/logcomm.h"
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/components/bitfield.h>
#include "statgroups.h"
#include <utilitieslib/utils/log.h>

#define MAX_CLIENTS 50
#define LOGID "Statserver"

StashTable g_sgrpFromId;

// *********************************************************************************
//  arena message store
// *********************************************************************************

MultiMessageStore* g_arenaMessages;
int g_statsVerbose = 0;

// *********************************************************************************
// main, etc.
// *********************************************************************************

static void StatDbConnect(void)
{
#define DB_CONNECT_TRIES 5
    int i;

    dbCommStartup();
    for (i = 0; i < DB_CONNECT_TRIES; i++)
    {
        if (dbTryConnect(15.0,DEFAULT_DBSTAT_PORT))
        {
            break;
        }
        
        if (i < DB_CONNECT_TRIES-1)
        {
            printf("failed to connect to dbserver, retrying..\n");
            Sleep(60);
        }
    }
    if (i == DB_CONNECT_TRIES)
    {
        printf("failed to connect to dbserver, exiting..\n");
        exit(1);
    }
#undef DB_CONNECT_TRIES
}

void UpdateStatTitle(void)
{
    char buf[200];
    sprintf(buf, "StatServer - %s%s", 
        db_comm_link.connected? "": " DISCONNECTED",
        g_statsVerbose? " VERBOSE": "");
    setConsoleTitle(buf);
}

static void StatLoadContainers(void)
{
    dbSyncContainerRequest(CONTAINER_SGRPSTATS, 0, CONTAINER_CMD_LOAD_ALL, 0);
    dbSyncContainerRequest(CONTAINER_LEVELINGPACTS, 0, CONTAINER_CMD_LOAD_ALL, 0);
    dbSyncContainerRequest(CONTAINER_LEAGUES, 0, CONTAINER_CMD_LOAD_ALL, 0);
//    dbSyncContainerRequest(CONTAINER_RAIDS, 0, CONTAINER_CMD_LOCK_AND_LOAD_ALL, 0); // in case of reload
    dbSyncContainerRequest(CONTAINER_MININGACCUMULATOR, 0, CONTAINER_CMD_LOAD_ALL, 0);
    UpdateStatTitle();
}

static void loadConfigFiles()
{
    loadstart_printf("loading badges.. ");
    load_Badges(&g_SgroupBadges, "defs/supergroup_badges.def", 0, "server/db/templates/supergroup_badges.attribute",
                BADGE_SG_MAX_BADGES);
    sgrpbadge_ServerInit();
    loadend_printf("done");

    loadstart_printf("loading base...");
    baseupkeep_Load();
    loadend_printf("done");
}

typedef enum DebugState
{
    kDebugState_None,
    kDebugState_Hotkeys,
    kDebugState_Console,
    kDebugState_Count    
} DebugState;

bool debugstate_Valid( DebugState e )
{
    return INRANGE0(e, kDebugState_Count);
}

// *********************************************************************************
//  util functions
// *********************************************************************************

//----------------------------------------
//  helper for rent due
//----------------------------------------
char* statserver_UpdateBaseUpkeep(int idSgrp)
{
    Supergroup *sg;
    Entity *e = NULL;
    U32 time = dbSecondsSince2000();
    char *res = "did nothing";
    if (!stashIntFindPointer( g_sgrpFromId,idSgrp, &sg))
        sg = NULL;

    if( sg 
        && sg->upkeep.prtgRentDue < 0 
        && dbSyncContainerRequest(CONTAINER_SUPERGROUPS, idSgrp, CONTAINER_CMD_LOCK_AND_LOAD, 0 ))
    {
        dbLog("SgrpUpkeep:StatserverUpdate", NULL, "sgrp %d had negative rent %d, next rent due in %d seconds", idSgrp, sg->upkeep.prtgRentDue, sg->upkeep.secsRentDueDate);            

        // clear bad rent value
        sg->upkeep.prtgRentDue = 0;

        // unlock and update
        {
            char *container_str;
            container_str = packageSuperGroup(sg);
            if(!verify(dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,idSgrp,CONTAINER_CMD_UNLOCK,container_str)))
            {
                LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__ ": failed to unlock sg %s, id %d. container %s",sg->name,idSgrp,container_str);
            }
            free(container_str);
        }
    }

    if( sg && sg->upkeep.secsRentDueDate < time )
    {
        int nLate = sgroup_nUpkeepPeriodsLate(sg);

        // ----------
        // set the rent

        // if the rent is zero it hasn't been charged yet
        if( sg->upkeep.prtgRentDue == 0
            && dbSyncContainerRequest(CONTAINER_SUPERGROUPS, idSgrp, CONTAINER_CMD_LOCK_AND_LOAD, 0 ))
        {
            
            // special case: if the base was just purchased, just set the due date
            if( sg->upkeep.secsRentDueDate == 0 )
            {
                sg->upkeep.secsRentDueDate = time + g_baseUpkeep.periodRent;
                res = "updated due date, no rent charged";
            }            
            else // set rent owed
            {                            
                int taxablePrestige = MAX(sg->prestige,0) + MAX(sg->prestigeBase,0);
                F32 pct = baseupkeep_PctFromPrestige(taxablePrestige);
                sg->upkeep.prtgRentDue = pct*taxablePrestige + sg->prestigeAddedUpkeep;
                res = "charged rent.";

                // don't allow negative rents
                if (sg->upkeep.prtgRentDue < 0)
                    sg->upkeep.prtgRentDue = 0;

                if(0 == sg->upkeep.prtgRentDue)
                {
                    res = "no rent owed. this period free";
                    sg->upkeep.secsRentDueDate = time + g_baseUpkeep.periodRent;
                }
            }
            
            // log it
            dbLog("SgrpUpkeep:StatserverUpdate", NULL, "sgrp %d owes rent %d, next rent due in %d seconds", idSgrp, sg->upkeep.prtgRentDue, sg->upkeep.secsRentDueDate);            

            // unlock and update
            {
                char *container_str;
                container_str = packageSuperGroup(sg);
                if(!verify(dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,idSgrp,CONTAINER_CMD_UNLOCK,container_str)))
                {
                    LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__ ": failed to unlock sg %s, id %d. container %s",sg->name,idSgrp,container_str);
                }
                free(container_str);
            }
        }
        else
        {
            res = "rent already overdue";
        }

        // ----------
        // figure out the upkeep period

        if( nLate > 0 )
        {
            stat_sendToAllSgrpMembers( idSgrp, "sg_base_rentdue_relay" );
        }
    }

    // --------------------
    // finally

    return res;
}

static int *entsOnline = NULL;
static int *entsOffline = NULL;
static int netpacketcallback_OnlineEnts(Packet *pak, int cmd, NetLink *link)
{
    int count = pktGetBitsAuto(pak);
    int i;
    eaiClear(&entsOnline);
    eaiClear(&entsOffline);
    for( i = 0; i < count; ++i ) 
    {
        int dbid = pktGetBitsAuto(pak);
        bool online = pktGetBitsPack(pak, 1);
        if( online )
        {
            eaiPush(&entsOnline,dbid);
        }
        else
        {
            eaiPush(&entsOffline,dbid);
        }
    }
    return 1;
}

int* reqSomeOnlineEnts(int *ids, bool wantOnline)
{
    static PerformanceInfo* perfInfo;
    int i;
    int count = eaiSize( &ids );
    Packet *pak = pktCreateEx(&db_comm_link, DBCLIENT_REQ_SOME_ONLINE_ENTS);
#if defined(_M_X64)
#pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
    pktSendBitsPack( pak, 1, (U32)(uintptr_t)netpacketcallback_OnlineEnts);
    pktSendBitsAuto( pak, count);
    for(i = 0; i < count; ++i )
    {
        pktSendBitsAuto(pak,ids[i]);
    }
    pktSend(&pak, &db_comm_link);
    if(dbMessageScanUntil(__FUNCTION__, &perfInfo))
    {
        if( wantOnline )
        {
            return entsOnline;
        }
        else
        {
            return entsOffline;
        }
    }
    else
    {
        return NULL;
    }
}


typedef int (*statDebug_fpDbgPrintf)(const char *format, ...);
void statDebugHandleString(char *inputStr, statDebug_fpDbgPrintf fpDbgPrintf)
{
    static int idSgrp = 0;
    int oldSgId = idSgrp;
    Supergroup *sg;
    if ( !stashIntFindPointer( g_sgrpFromId, idSgrp, &sg) )
        sg = NULL;

    if( !verify(inputStr && fpDbgPrintf))
    {
        return;
    }    

    // ----------
    // parse the input

    fpDbgPrintf("id is %d (id <num> to set)\n>", idSgrp);

    if(strStartsWith(inputStr,"id"))
    {
        if( 1 != sscanf(inputStr, "id %d", &idSgrp))
        {
            fpDbgPrintf("error in input %s\n", inputStr);
        }
        else
        {
            if( stashIntFindPointer(g_sgrpFromId, idSgrp, NULL) )
            {
                fpDbgPrintf("new sgrp id %d\n", idSgrp);
            }
            else
            {
                fpDbgPrintf("couldn't find supergroup with id %d\n", idSgrp );
                idSgrp = oldSgId;
            }
        }
    }
    else if( strStartsWith (inputStr,"bspres"))
    {
        int basePrestige = 0;
        if( idSgrp <= 0 )
        {
            fpDbgPrintf("no valid sgrp id yet\n");
        }
        else if(1 == sscanf(inputStr,"bspres %d", &basePrestige))
        {
            if( dbSyncContainerRequest(CONTAINER_SUPERGROUPS, idSgrp, CONTAINER_CMD_LOCK_AND_LOAD, 0 ))
            {
                sg->prestigeBase = basePrestige;
                {
                    char *container_str;
                    container_str = packageSuperGroup(sg);
                    if(!verify(dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,idSgrp,CONTAINER_CMD_UNLOCK,container_str)))
                    {
                        LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__ ": failed to unlock sg %s, id %d. container %s",sg->name,idSgrp,container_str);
                    }
                    
                    free(container_str);
                }
                fpDbgPrintf("set prestige to %d\n", sg->prestigeBase);
            }
        }
    }
    else if( strStartsWith (inputStr,"bsrent"))
    {
        int rentOwed = 0;
        if( idSgrp <= 0 )
        {
            fpDbgPrintf("no valid sgrp id yet\n");
        }
        else if(1 == sscanf(inputStr,"bsrent %d", &rentOwed))
        {
            if( dbSyncContainerRequest(CONTAINER_SUPERGROUPS, idSgrp, CONTAINER_CMD_LOCK_AND_LOAD, 0 ))
            {
                sg->upkeep.prtgRentDue = rentOwed;
                {
                    char *container_str;
                    container_str = packageSuperGroup(sg);
                    if(!verify(dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,idSgrp,CONTAINER_CMD_UNLOCK,container_str)))
                    {
                        LOG( LOG_STATSERVER, LOG_LEVEL_IMPORTANT, 0, __FUNCTION__ ": failed to unlock sg %s, id %d. container %s",sg->name,idSgrp,container_str);
                    }
                    
                    free(container_str);
                }
                fpDbgPrintf("set rent to %d\n", sg->upkeep.prtgRentDue);
            }
        }
    }
    else if( strStartsWith(inputStr, "bsdp"))
    {
        int periodsBack = 0;
        if( idSgrp <= 0 )
        {
            fpDbgPrintf("no valid sgrp id yet\n");
        }
        else if(1 == sscanf(inputStr,"bsdp %d", &periodsBack))
        {
            if( dbSyncContainerRequest(CONTAINER_SUPERGROUPS, idSgrp, CONTAINER_CMD_LOCK_AND_LOAD, 0 ))
            {
                sg->upkeep.secsRentDueDate = dbSecondsSince2000() - g_baseUpkeep.periodRent*periodsBack;
                {
                    char *container_str;
                    container_str = packageSuperGroup(sg);
                    verify(dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,idSgrp,CONTAINER_CMD_UNLOCK,container_str));
                    free(container_str);
                }
                fpDbgPrintf("set periods back to %d\n", periodsBack);
            }
        }
    }
    else if( 0 == stricmp(inputStr,"bup"))
    {
        int rentOwed = 0;
        if( idSgrp <= 0 )
        {
            fpDbgPrintf("no valid sgrp id yet\n");
        }
        else
        {
            fpDbgPrintf("%s\nrent due:%d\n", statserver_UpdateBaseUpkeep(idSgrp), sg->upkeep.prtgRentDue);
        }
    }
    else if( 0 == stricmp(inputStr, "ls") )
    {
        StashTableIterator iSgrpIds = {0};
        if( idSgrp <= 0 )
        {
            fpDbgPrintf("no valid sgrp id yet\n");
        }
        else
        {
            fpDbgPrintf("rent owed: %d\n", sg->upkeep.prtgRentDue);
        }
    }
    else if( 0 == stricmp(inputStr, "lsg") )
    {
        StashTableIterator iSgrpIds = {0};
        StashElement elem;
        
        fpDbgPrintf("id:name\n");
        for(stashGetIterator( g_sgrpFromId, &iSgrpIds ); (stashGetNextElement(&iSgrpIds, &elem));)
        {
            Supergroup *sg = stashElementGetPointer(elem);
            int idSgrp = stashElementGetIntKey(elem);
            if( sg )
            {
                fpDbgPrintf("%d: %s\n", idSgrp, sg->name);
            }
        }    
    }
    else if( 0 == stricmp(inputStr, "rbs") )
    {
        StashTableIterator iSgrpIds = {0};
        
        if( idSgrp <= 0 )
        {
            fpDbgPrintf("no valid sgrp id yet\n");
        }
        else
        {
            U32 *eaiStates = sgrpbadge_eaiStatesFromId(idSgrp);
            int n = eaiSize( &eaiStates);
            if( n )
            {
                // clearing this will cause an update automatically
                ZeroStructs( eaiStates, n );
            }
        }
    }
    else if( strStartsWith(inputStr, "statAdj"))
    {
        char statName[512] = "";
        int adj = 0;
        
        if( idSgrp <= 0 )
        {
            fpDbgPrintf("no valid sgrp id yet\n");
        }
        else if(2 == sscanf(inputStr,"statAdj %s %d", statName, &adj))
        {
            sgrpstats_StatAdj( idSgrp, statName, adj );
        }
        else
        {
            fpDbgPrintf("failed to parse cmd\n");
        }
    }
    else if( strStartsWith(inputStr, "rwtoken") )
    {
        char token[512];
        if( !sg )
        {
            fpDbgPrintf("no valid sgrp id yet\n");
        }
        else if( 1 == sscanf(inputStr, "rwtoken %s", token) )
        {
            bool locked = dbSyncContainerRequest(CONTAINER_SUPERGROUPS, idSgrp, CONTAINER_CMD_LOCK_AND_LOAD, 0 );
            
            if(verify(locked))
            {
                char *container_str;
                if( 0 > rewardtoken_Award(&sg->rewardTokens, token, 0 ))
                {
                    fpDbgPrintf("couldn't give token %s\n", token );
                }
                
                container_str = packageSuperGroup(sg);
                verify(dbSyncContainerUpdate(CONTAINER_SUPERGROUPS,idSgrp,CONTAINER_CMD_UNLOCK,container_str));
                free(container_str);
            }
        }
        else
        {
            fpDbgPrintf("couldn't parse cmd.\n");
        }
    }
    else if( 0 == stricmp(inputStr, "owned"))
    {
        if( !sg )
        {
            fpDbgPrintf("no sgrpId set yet\n");
        }
        else
        {
            int i;
            fpDbgPrintf("badges owned:\n");
            for( i = 0; i < BADGE_SG_MAX_BADGES; ++i )
            {
                if( sgrpbadges_IsOwned( &sg->badgeStates, i ))
                {
                    BadgeDef *def;
                    if( stashIntFindPointer(g_SgroupBadges.hashByIdx, i, &def) )
                    {
                        fpDbgPrintf("%d: %s\n", i, def->pchName);
                    }
                    else
                    {
                        fpDbgPrintf("%d: <has no name>\n", i );
                    }
                }
            }
        }
    }
    else if( strStartsWith(inputStr, "onents") || strStartsWith(inputStr, "offents")  ) // online ents
    {
        int i;
        int *ids = NULL;
        int *resEnts = NULL;
        char *tmp = inputStr;
        char *s = NULL;
        for(;*tmp;++tmp)
        {
            if(*tmp > '0' && *tmp < '9' && s == NULL)
            {
                s = tmp;
            }
            else if(*tmp == ',')
            {
                *tmp = 0;
                eaiPush(&ids, atoi(s));
                s = NULL;
            }
        }
        if(s)
            eaiPush(&ids, atoi(s));

        if(ids)
        {
            resEnts = reqSomeOnlineEnts(ids, strStartsWith(inputStr, "onents"));
            printf("results: ");
            for( i = 0; i < eaiSize( &resEnts ); ++i ) 
            {
                printf("%i,", resEnts[i]);
            }
            printf("\n");
            eaiDestroy(&ids);
        }
        else
        {
            printf("must pass some ids: N,N,N,\n");
        }
    }
    else
    {
        fpDbgPrintf(
            "bspres <n>                #set value of base to n\n" \
            "bsrent <n>                #set rent to n\n" \
            "bsdp <n>                #set the due dat back n periods\n" \
            "bup                    #do an update check on the supergroup\n" \
            "id <n>\t\t\t\t #set cur sgrp id\n" \
            "lsg\t\t\t\t#list supergroups\n" \
            "owned\t\t\t\t#owned badges\n" \
            "rbs\t\t\t\t#reset badge states\n" \
            "rwtoken <tokenname> \t\t#reward the given token\n" \
            "statAdj <statname> <amt>\t#to add stats\n" \
            "exit \t\t\t\t#to exit\n"
            ); 
    }
}

static void statDebugHook()
{
    static char cmdStr[200] = {0};
    static char enter_cmd=' ';
    static DebugState debug_state=false;
    
    char ch;

    if (_kbhit()) 
    {
        if( debug_state == kDebugState_Console )
        {
            static char tmp[ARRAY_SIZE( cmdStr )];
            if( gets_s(tmp, sizeof(tmp)) )
            {
                statDebugHandleString(tmp, printf);
                
                if( 0 == stricmp(tmp, "exit"))
                {
                    debug_state = debug_state + 1 >= kDebugState_Count ? kDebugState_None : debug_state + 1;                    
                    printf("bye!\n");
                }
            }
        }
        else
        {    
            ch = _getch();
            
            if (ch == 'd') {
                debug_state = debug_state + 1 >= kDebugState_Count ? kDebugState_None : debug_state + 1;
                if (debug_state == kDebugState_Hotkeys) {
                    printf("Debugging hotkeys enabled (Press '?' for a list).\n");
                }
                else if( debug_state == kDebugState_Console )
                {
                    printf("console mode debugging. (blocks while entering commands, exit to leave)\n");
                }
                else{
                    printf("leave debugging mode\n");
                }
                
            } 
            else if(debug_state) 
            {
                switch (ch) {
                case 'a':
                    if (isDevelopmentMode()) {
                        printf("Asserting!\n");
                        assert(0);
                    }
                    break;
                case 'c':
                    printf("disconnecting from dbserver and clients..\n");
                    stat_LevelingPactReset();
                    stat_LeagueReset();
                    clearNetLink(&db_comm_link);
                    UpdateStatTitle();
                    
                    printf("sleeping..\n");
                    Sleep(1000);
                    
                    printf("reconnecting to dbserver..\n");
                    StatDbConnect();
                    UpdateStatTitle();
                    StatLoadContainers();
                    
                    break;
                case 'm':
                    memMonitorDisplayStats();
                    break;
                case 'n':
                    printf("db_link sendQueue size: %d\n", qGetSize(db_comm_link.sendQueue2));
                    break;
                case 'p':
                    printf("SERVER PAUSED FOR 2 SECONDS PRESS 'p' AGAIN TO PAUSE INDEFINITELY...\n");
                    Sleep(2000);
                    if (_kbhit() && _getch()=='p') {
                        printf("SERVER PAUSED\n");
                        (void)_getch();
                    }
                    printf("Server resumed\n");
                    break;
                case 's':
                    printf("saving database...\n");
                    statdb_Save(true);
                    printf("done.\n");
                    break;
                case 'S':
                    printf("saving mined data...\n");
                    miningdata_Save(0);
                    printf("done.\n");
                    break;
                case 'v':
                    g_statsVerbose = !g_statsVerbose;
                    printf("verbose mode %s\n", g_statsVerbose ? "on" : "off");
                    UpdateStatTitle();
                    break;
                break;
                case 'f':
                {
                    logFlush(true);
                }
                break;
                case 'g':
                {
                    logDisconnectFlush();
                }
                break;
                case '?':
                    printf("General commands:\n");
                    printf("  a - fake a crash/assert if in development mode\n");
                    printf("  m - print memory usage\n");
                    printf("  n - print some network stats\n");
                    printf("  p - pause server\n");
                    printf("  c - reconnect to DbServer in 1s\n");
                    printf("  s - save backup of database\n");
                    printf("  S - save backup of mined data\n");
                    printf("  l - log a message to the logserver using dbLog\n");
                    printf("  f - flush the log\n");
                    printf("  g - flush the log as if it were a shutdown\n");
                    break;
                }
            }
        }
    }
}




#define STATSERVER_TICK_FREQ    10        // ticks/sec
#define STATSERVER_BACKUP_FREQ_SECS (1) // one second per. the save command spreads out the data to save
#define STATSERVER_BACKUP_FREQ    (STATSERVER_TICK_FREQ*STATSERVER_BACKUP_FREQ_SECS)
#define STATSERVER_MINEACC_FREQ (STATSERVER_TICK_FREQ*10) // once every ten seconds
#define STATSERVER_MINEACC_DUR    0.01    // spend one hundredth of a second persisting mined data every second
char    g_exe_name[MAX_PATH];

int main(int argc,char **argv)
{
    int        i,timer;
    bool paramLauncher = false;
    int dbLauncherCookie = 0;

    strcpy(g_exe_name,argv[0]);

    EXCEPTION_HANDLER_BEGIN
//    setAssertMode(ASSERTMODE_MINIDUMP | ASSERTMODE_DEBUGBUTTONS);
    setAssertMode(ASSERTMODE_DEBUGBUTTONS | ASSERTMODE_FULLDUMP | ASSERTMODE_DATEDMINIDUMPS | ASSERTMODE_ZIPPED);

    memMonitorInit();
    for(i = 0; i < argc; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");

    setWindowIconColoredLetter(compatibleGetConsoleWindow(), 'S', 0x8080ff);

    FolderCacheChooseMode();

    preloadDLLs(0);

    if (fileIsUsingDevData()) {
        bsAssertOnErrors(true);
        setAssertMode(ASSERTMODE_DEBUGBUTTONS |
            (!IsDebuggerPresent()? ASSERTMODE_MINIDUMP : 0));
    } else {
        // In production mode on the servers we want to save all dumps and do full dumps
        setAssertMode(ASSERTMODE_DEBUGBUTTONS |
            ASSERTMODE_FULLDUMP |
            ASSERTMODE_DATEDMINIDUMPS | ASSERTMODE_ZIPPED);
    }

    // no shared memory for this, different layout than others
    sharedMemorySetMode(SMM_DISABLED);

    srand((unsigned int)time(NULL));
    consoleInit(110, 128, 0);

    // --------------------
    // handle command line params

    {
        bool handleBackup = false;
        
        for(i=1;i<argc;i++)
        {
            if (stricmp(argv[i],"-db")==0)
            {
                strcpy(server_cfg.db_server,argv[++i]);
                if(!server_cfg.log_server)
                    strcpy(server_cfg.log_server,server_cfg.db_server);
            }
            else if(0==stricmp(argv[i], "-logserver"))
            {
                strcpy(server_cfg.log_server,argv[++i]);
            }
            else if (stricmp(argv[i],"-launcher")==0)
            {
                paramLauncher = true;
            }
            else if (stricmp(argv[i],"-cookie")==0 && i+1 < argc)
            {
                sscanf(argv[++i],"%d", &dbLauncherCookie);
            }
            // ab: wait for file-based db
//             else if(stricmp(argv[i],"-backup")==0)
//             {
//                 handleBackup = true;
//             }
            else
            {
                Errorf("-----INVALID PARAMETER: %s\n", argv[i]);
            }
        }

//         if( handleBackup )
//         {
//             printf("BACKUP EXE: making backup\n");
//             statdb_Backup(STATSERVER_BACKUP_FREQ_SECS - 1);
//             printf("BACKUP EXE: done.\n");
//             return  0;
//         }
    }

    // --------------------
    // more init 

    UpdateStatTitle();
    loadConfigFiles();
    statdb_Init();
    
    logSetDir("statserver");
    sockStart();
    serverCfgLoad();
        
    Strncpyt(db_state.server_name, server_cfg.db_server);
    Strncpyt(db_state.log_server, server_cfg.log_server); // copy the logserver
    Strncpyt(db_state.map_name, "Statserver");
    
    loadstart_printf("Networking startup...");
    timer = timerAlloc();
    timerStart(timer);
    packetStartup(0,0);
    loadend_printf("");

    loadstart_printf("Connecting to dbserver (%s)...", db_state.server_name);
    stat_LevelingPactReset();
    stat_LeagueReset();
    StatDbConnect();
    loadend_printf("done");
    
    loadstart_printf("Connecting to logserver (%s)...", db_state.log_server);
    if(*server_cfg.log_server)
    {
        logNetInit();
        loadend_printf("done");
    }
    else
    {
        loadend_printf("no logserver specified. printing logs locally.");
    }


    loadstart_printf("Loading containers...");
    StatLoadContainers();
    loadend_printf("done");
    loadstart_printf("Requesting Removal of Inactive Accounts...");
    stat_LevelingPactRequestActivityCheckList();
    loadend_printf("done");

    loadstart_printf("Misc other..");
    
    loadend_printf("");

    printf("Ready.\n");
    for(;;)
    {
        dbComm(); 
        logFlush(false);
        NMMonitor(1);
        netIdle(&db_comm_link,1,10);
        statDebugHook();
        if (timerElapsed(timer) > (1.f / STATSERVER_TICK_FREQ))
        {
            static U32 timerTicks = 1; // no need to backup right after startup
            timerStart(timer);

            if( sgrpstats_Changed() )
            {                
                // accum any stats received over the net
                // and mark any badges that might have been earned.
                sgrpstats_FlushToSgrps();
            }

            stat_LevelingPactUpdateTick(0);
            stat_LeagueUpdateTick();

            // see if any sgrp badges should be awarded
            sgrpbadges_TickOverHash( g_sgrpFromId );

            // see if we need a backup
            if( (timerTicks % STATSERVER_BACKUP_FREQ) == 0 )
                statdb_Save(false); // supergroup stats
            if( (timerTicks % STATSERVER_MINEACC_FREQ) == 0 )
                miningdata_Save(STATSERVER_MINEACC_DUR);

            // --------------------
            // supergroup upkeep tick
            {
                static U32 dailySgrpCheck = 0;
                U32 time = dbSecondsSince2000();
                if( dailySgrpCheck < time )
                {
                    StashTableIterator hi;
                    StashElement he = NULL;
                    for(stashGetIterator(g_sgrpFromId,&hi); stashGetNextElement(&hi, &he);)
                    {
                        int idSgrp = stashElementGetIntKey(he);
                        statserver_UpdateBaseUpkeep(idSgrp);
                    }
                    // set check
                    dailySgrpCheck = time + 24*60*60;
                }
            }
            

            timerTicks++;
        }
    }
    EXCEPTION_HANDLER_END
}

Supergroup *stat_sgrpFromIdSgrp(int idSgrp, bool loadSync)
{
    Supergroup *res = NULL;
    if( idSgrp && g_sgrpFromId )
    {
        if( !stashIntFindPointer(g_sgrpFromId, idSgrp, &res) && loadSync )
        {
            // temp load is okay, if already loaded it will keep it in memory, otherwise unload
            if( dbSyncContainerRequest(CONTAINER_SUPERGROUPS, idSgrp, CONTAINER_CMD_TEMPLOAD, 0 ))
            {
                if (!stashIntFindPointer(g_sgrpFromId, idSgrp, &res))
                    res = NULL;
            }
        }
    }
    return res;
}


Supergroup *stat_sgrpFromName(char *name, int *pResIdSgrp, bool loadSync)
{
    Supergroup *res = NULL;
    
    if( name )
    {
        int idSgrp = dbSyncContainerFindByElementEx(CONTAINER_SUPERGROUPS, "name", name, NULL, 0, 1);
        res = stat_sgrpFromIdSgrp(idSgrp, loadSync);
        if(pResIdSgrp)
            *pResIdSgrp = idSgrp;
    }
    
    // ----------
    // finally

    return res;
}

