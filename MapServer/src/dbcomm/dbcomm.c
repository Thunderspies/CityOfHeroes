#include <utilitieslib/assert/assert.h>
#include <process.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/netio_send.h>
#include <utilitieslib/utils/error.h>
#include "dbcomm.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include "comm_backend.h"
#include "dbdoor.h"
#include "dbmapxfer.h"
#include "dbcontainer.h"
#include <utilitieslib/utils/timing.h>
#include "dbquery.h"
#include "process.h"
#include <utilitieslib/utils/error.h>
#include "dbghelper.h"
#include "storyarc/storyarcinterface.h"
#include <utilitieslib/components/EString.h>
#include "svr/svr_player.h"
#include <utilitieslib/network/sock.h>
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/strings_opt.h>
#include "cmdparse/cmdserver.h"
#include "container/team.h"
#include "logcomm.h"
#include "container/mapgroup.h"
#include "gameSys/arenamapserver.h"
#include "gameSys/Turnstile.h"
#include "shardcomm.h"
#include "chatdb.h"
#include "entity/entity.h"
#include "dbnamecache.h"
#include "container/baseloadsave.h"
#include "group/group.h"
#include <utilitieslib/utils/memlog.h>
#include <utilitieslib/utils/RegistryReader.h>
#include "dbbackup.h"
#include <utilitieslib/components/EString.h>
#include "container/containerEmail.h"
#include "storyarc/missionServerMapTest.h"
#include "svr/svr_base.h"
#include "auth/authUserData.h"
#include "entity/character_base.h"
#include <utilitieslib/version/AppVersion.h>
#include <utilitieslib/utils/sysutil.h>
#include "container/EventHistory.h"
#include "entity/character_workshop.h"
#include "entity/character_combat.h"
#include "entity/character_net_server.h"
#include <utilitieslib/utils/log.h>
#include "gameSys/arenamapserver.h"
#include "account/AccountCatalog.h"
#include "entity/character_inventory.h"

#if defined (SERVER) && !defined(DBQUERY)
#include "account/AccountInventory.h"
#include "account/AccountCatalog.h"
#include "auth/authUserData.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "gameComm/sendToClient.h"
#include "script/ZoneEvents/ScriptedZoneEvent.h"
#include "bases/DetailRecipe.h"
#include "gameComm/svr_chat.h"
#include "cmdparse/cmdserver.h"
#include "entity/costume.h"
#include "entity/character_eval.h"
#include "player/pophelp.h"
#include "player/parseClientInput.h"
#include "staticMapInfo.h"
#include "player/badges_server.h"
#else
static U32 g_overridden_auth_bits[AUTH_DWORDS] = { 0 };
#endif

#if !defined AUX_SERVER_DEFINE && !defined DBQUERY
#include "entity/entPlayer.h"
#include "svr/svr_base.h"
#endif

#if STATSERVER
#include "entity/SgrpStats.h"
#endif


NetLink db_comm_link;
NetLink log_comm_link;
DBCommState db_state = {-1,1};
bool disconnected_from_dbserver=false; // Set to true when we manually disconnect to prevent sending of delink messages after we have disconnected
int num_relay_cmds=0;
bool log_relay_cmds=false;

#if defined(SERVER) && !defined(DBQUERY) // auction on mapservers only
    static void cacheAccountServerCatalogUpdate( Packet* pak_in );
    static void sendAccountCatalogUpdateToClients( Packet* pak_in );
#endif

void csvrDbMessage(int cmd,Packet* pak);

typedef struct
{
    int        cookie;
    int        *ptr;
} DataCallback;

static DataCallback *data_callbacks;
static int data_callback_count,data_callback_max;

int relay_response_succeeded, relay_response_map_id;

int dbProcessDataCallback(int cookie,int data)
{
    int                i;
    DataCallback    *dcb;

    for(i=data_callback_count-1;i>=0;i--)
    {
        dcb = &data_callbacks[i];
        if (dcb->ptr && dcb->cookie == cookie)
        {
            *dcb->ptr = data;
            memmove(dcb,&data_callbacks[i+1],(--data_callback_count - i) * sizeof(data_callbacks[0]));
            return 1;
        }
    }
    return 0;
}

int dbSetDataCallback(int *data)
{
    static    int cookie;
    DataCallback    *dcb;

    if (!data)
        return 0;
    dcb = dynArrayAdd(&data_callbacks,sizeof(data_callbacks[0]),&data_callback_count,&data_callback_max,1);
    dcb->ptr = data;
    dcb->cookie = ++cookie;
    if (!dcb->cookie) // never use 0 as a cookie
        dcb->cookie = ++cookie;
    *data = DBCOMM_ASYNC_WAITING;
    return cookie;
}

void dbCommShutdown(void)
{
    // Disconnect the link if not already.
    // FIXME!!
    // Figure out how to disconnect from db server.

    clearNetLink(&db_comm_link);
}

static void dbCommCheckShutdown(void)
{
    if (!db_comm_link.connected)
    {
        if (db_state.local_server) // should keep running if we're just being used for test/editing
            return;
        printf("Detected dbserver shut down, shutting down.\n");
#if defined SERVER && !defined DBQUERY
        groupCleanUp(); // Clean up ctris that might be in shared memory
#endif
        svrLogPerf();
        logSendDisconnect(30);    // remote logs (logserver)
        logShutdownAndWait();    // local logs
        exit(0);                // dbserver disconnected, no point going on
    }
}

void dbAsyncReturnAllToStaticMap(const char *msg0, const char *msg1)
{
    db_state.server_quitting = 1;
    estrConcatf(&db_state.server_quitting_msg,", (%s: %s)",(msg0 ? msg0 : "NULL"),(msg1 ? msg1 : "NULL"));
}

int dbMessageScan(int lookfor)
{
    return netLinkMonitor(&db_comm_link, lookfor, dbMessageCallback);
}

int dbMessageScanUntil(const char* name, PerformanceInfo** perfInfo)
{
    int        ret;

    PERFINFO_AUTO_START("dbMessageScanUntil", 1);
        PERFINFO_RUN(if(perfInfo)PERFINFO_AUTO_START_STATIC(name, perfInfo, 1););
            //teamlogPrintf(__FUNCTION__, "%s", name); 
            ret = netLinkMonitorBlock(&db_comm_link, LOOK_FOR_COOKIE, dbMessageCallback, 100000000.f);
        PERFINFO_RUN(if(perfInfo)PERFINFO_AUTO_STOP();)
    PERFINFO_AUTO_STOP();
    dbCommCheckShutdown();
    if (ret)
        return ret > 0;
    return 0;
}

int dbMessageScanUntilTimeout(const char* name, PerformanceInfo** perfInfo, int timeout)
{
    int        ret;

    PERFINFO_AUTO_START("dbMessageScanUntil", 1);
        PERFINFO_RUN(if(perfInfo)PERFINFO_AUTO_START_STATIC(name, perfInfo, 1););
            //teamlogPrintf(__FUNCTION__, "%s", name); 
            ret = netLinkMonitorBlock(&db_comm_link, LOOK_FOR_COOKIE, dbMessageCallback, timeout);
        PERFINFO_RUN(if(perfInfo)PERFINFO_AUTO_STOP();)
    PERFINFO_AUTO_STOP();
    dbCommCheckShutdown();
    if (ret)
        return ret > 0;
    return 0;
}

#define MAPFNAME "c:/mapname.txt"
static char *getEditMapName()
{
    char    *mem,*s;

    mem = fileAlloc(MAPFNAME,0);
    if (!mem)
        return 0;

    s = strtok(mem," \n\r");
    return s;
}

void dbSaveEditMapName(char *name)
{
    FILE    *file;

    if (getEditMapName() && stricmp(name,getEditMapName())==0)
        return;
    PERFINFO_AUTO_START("dbSaveEditMapName", 1);
        file = fileOpen(MAPFNAME,"wt");
        fprintf(file,"%s\n",name);
        fclose(file);
    PERFINFO_AUTO_STOP();
}

void dbReadyForPlayers()
{
    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_READY_FOR_PLAYERS);
    pktSendBitsPack(pak,1,db_state.map_id);
    pktSend(&pak,&db_comm_link);
    lnkBatchSend(&db_comm_link);
}

int dbRegister(int map_id,U32 *ip_list,int udp_port,int tcp_port,int cookie)
{
    static PerformanceInfo* perfInfo;
    
    Packet    *pak;
    char    *s,buf[500];

    db_state.map_registered = 0;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_REGISTER);
    pktSendBitsPack(pak,1,map_id);
    pktSendBitsPack(pak,1,ip_list[0]);
    pktSendBitsPack(pak,1,ip_list[1]);
    pktSendBitsPack(pak,1,udp_port);
    pktSendBitsPack(pak,1,tcp_port);
    pktSendBitsPack(pak,1,db_state.staticlink);
    pktSendBitsPack(pak,1,cookie);
    pktSendString(pak,getExecutablePatchVersion("CoH Server"));

    if (map_id < 0)
    {
        s = getEditMapName();
        if (!s)
            s = "noname.txt";

        sprintf(buf,"MapName \"%s\"\n",s);
        pktSendString(pak,buf);
    }
    pktSend(&pak,&db_comm_link);

    //printf("waiting to hear back on registration..\n");
    if (!dbMessageScanUntil(__FUNCTION__, &perfInfo))
    {
        FatalErrorf("message scan timed out\n");
    }

    loadend_printf("map id %d",db_state.map_id);

    return 1;
}



void dbSetupMap()
{
    dbGatherMapLinkInfo();
    sendDoorsToDbServer(db_state.is_static_map);
    if (db_state.is_static_map)
        mapGroupInit("AllStaticMaps");
}

void testClientSetDb(const char *server_name)
{
    RegReader rr = createRegReader();
    initRegReader(rr, "HKEY_CURRENT_USER\\SOFTWARE\\Cryptic\\TestClient");
    rrWriteString(rr, "CS", server_name);
    destroyRegReader(rr);
}

int dbTryConnect(F32 timeout, int port)
{
    // In -localmapserver mode, this function is called to reconnect as well as for an initial connection

    static PerformanceInfo* perfInfo;

    // ST: Seems this hash table is never used after being loaded up with 1 meg of data
    //dbLoadAttributes();
    
    dbClearContainerIDCache();

    getAutoDbName(db_state.server_name);
    testClientSetDb(db_state.server_name); // Do this a second time here in case "test" resolves to something else.

    if (netConnect(&db_comm_link,db_state.server_name,port,NLT_TCP,timeout,NULL))
    {
        Packet    *pak;

        db_comm_link.cookieSend = 1;
        pak = pktCreateEx(&db_comm_link,DBCLIENT_INITIAL_CONNECT);
        pktSendBitsPack(pak,1,DBSERVER_PROTOCOL_VERSION);
        pktSend(&pak,&db_comm_link);
        if (dbMessageScanUntil("DBCLIENT_INITIAL_CONNECT", &perfInfo) <= 0) {
            // Exit peacefully, DbServer may have crashed, and we don't want to have to manually close the maps
            if (isDevelopmentMode()) {
                FatalErrorf("%s",last_db_error);
            } else {
                return 0;
            }
        }
        dbUpdateAllOnlineInfo();
        dbUpdateAllOnlineInfoComments();
    }
    else
        return 0;
    return 1;
}

#if !defined AUX_SERVER_DEFINE && !defined DBQUERY
static void sendServerStats(NetLink *link)
{
    static __int64 last_tick = 0;
    static __int64 long_tick = 0;
    static int num_ticks = 0;
    static int timer = 0;
    F32        ticks_sec;
    __int64 tick;

    if (!timer)
        timer = timerAlloc();

    // get some stats on server ticks
    tick = timerCpuTicks64();
    if (last_tick == 0)
        last_tick = tick;
    if (tick - last_tick > long_tick)
        long_tick = tick - last_tick;
    num_ticks++;

    if (timerElapsed(timer) > 10.f)
    {
        int        i,player_count=0,monster_count=0,total_count=0,hero_count=0,villain_count=0;
        int        total_ping=0, total_ping_count=0, min_ping=0, max_ping=0;
        Packet    *pak;

        for(i=1;i<entities_max;i++)
        {
            if(entity_state[i] & ENTITY_IN_USE)
            {
                total_count++;
                if (ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER) {
                    if (entities[i]->client && entities[i]->client->link) {
                        int ping = pingAvgRate(&entities[i]->client->link->pingHistory);
                        if (!total_ping_count)
                            min_ping = max_ping = ping;
                        else {
                            min_ping = MIN(min_ping, ping);
                            max_ping = MAX(max_ping, ping);
                        }
                        total_ping += ping;
                        total_ping_count++;
                    }
                    player_count++;
                    if (ENT_IS_ON_BLUE_SIDE(entities[i])) {
                        hero_count++;
                    } else if (ENT_IS_ON_RED_SIDE(entities[i])) {
                        villain_count++;
                    }
                } else if (ENTTYPE_BY_ID(i) == ENTTYPE_CRITTER)
                    monster_count++;
            }
        }

        svr_hero_count = hero_count;
        svr_villain_count = villain_count;

        ticks_sec = num_ticks / timerElapsed(timer);

        pak = pktCreateEx(link,DBCLIENT_SERVER_STATS_UPDATE);
        pktSendBitsPack(pak,1,total_count);
        pktSendBitsPack(pak,1,player_count);
        pktSendBitsPack(pak,1,monster_count);
        pktSendBitsPack(pak,1,player_ents[PLAYER_ENTS_CONNECTING].count);
        pktSendF32(pak,ticks_sec);
        pktSendF32(pak,1000*timerSeconds64(long_tick)); // in ms
        pktSendBitsPack(pak,1,hero_count);
        pktSendBitsPack(pak,1,villain_count);
        pktSendBitsAuto(pak,num_relay_cmds);
        pktSendBitsPack(pak,9,total_ping/(total_ping_count?total_ping_count:1));
        pktSendBitsPack(pak,9,min_ping);
        pktSendBitsPack(pak,9,max_ping);
        pktSend(&pak,link);

        timerStart(timer);
        long_tick = 0;
        num_ticks = 0;
        num_relay_cmds = 0;
    }

    last_tick = tick;
}
#endif

void dbComm()
{
    dbCommCheckShutdown();
    dbMessageScan(0);
    lnkBatchSend(&db_comm_link);
    //netIdle(&db_comm_link, 0, 10);
#if !defined AUX_SERVER_DEFINE && !defined DBQUERY
    sendServerStats(&db_comm_link);
#endif
}


static void handleMapRegisterCallback(Packet *pak)
{
    int        door_id,map_id;

    door_id = pktGetBitsPack(pak,1);
    map_id = pktGetBitsPack(pak,1);
    verbose_printf("door %d = map %d\n",door_id,map_id);
}

static void handleForceLogout(Packet *pak_in)
{
    int        db_id,reason;

    db_id = pktGetBitsPack(pak_in,1);
    reason = pktGetBitsPack(pak_in,1);
    LOG_OLD( "handleForceLogout: db_id %d reason %d\n",db_id,reason);
    forceLogout(db_id,reason);
}

void handleShutdown(Packet *pak)
{
    int        terminal;
    char    msg[2000];

    if (db_state.local_server)
        return;

    terminal = pktGetBitsPack(pak,1);
    strcpy(msg,pktGetString(pak));
    returnAllPlayers(terminal,msg);
}

void handleTeamLeftMission()
{
    MissionRequestShutdown();
}

void relayCommandLogToggle(void)
{
    log_relay_cmds = !log_relay_cmds;
    if (log_relay_cmds)
        printf("Logging all relay commands\n");
    else
        printf("Not logging relay commands\n");
}

void handleRelayCmd(Packet *pak)
{
    char *msg = pktGetString(pak);
    num_relay_cmds++;
    //if (log_relay_cmds) // uncomment this once I've debugged the command that causing the crash
        LOG_OLD("relaycmds - %s", msg);
    cmdOldServerHandleRelay(msg);
}

void handleRelayCmdResponse(Packet *pak)
{
    relay_response_succeeded = pktGetBitsPack(pak, 1);
    if (relay_response_succeeded) {
        relay_response_map_id = pktGetBitsPack(pak, 1);
    } else {
        relay_response_map_id = -1;
    }
#ifdef SERVER
#ifndef DBQUERY
    memlog_printf(&g_teamlog, "RelayCmdResponse: succeeded: %d; map_id: %d", relay_response_succeeded, relay_response_map_id);
#endif
#endif
}

int handleClientCmdFailed(Packet *pak)
{
    last_db_error_code = pktGetBitsPack(pak,1);
    strcpy(last_db_error,pktGetString(pak));
    LOG( LOG_ERROR, LOG_LEVEL_VERBOSE, 0, "handleClientCmdFailed: %s\n",last_db_error);
    if (last_db_error_code == CONTAINER_ERR_CANT_COMPLETE_SERIOUS)
    {
        if (fileIsUsingDevData() && strstri(last_db_error, "(Invalid field name 'Badges")) {
            Errorf(last_db_error);
        } else {
            assertmsg(0,last_db_error);
        }
    }
    return 0;
}

void dbForceRelayCmdToMapByEnt(int dbid, const char *fmt, ...)
{
    char *buf = estrTemp();
    Packet *pak_out;

    VA_START(va, fmt);
    estrConcatf(&buf, "cmdrelay_dbid %d\n", dbid);
    estrConcatfv(&buf, fmt, va);
    VA_END();

    pak_out = pktCreateEx(&db_comm_link, DBCLIENT_RELAY_CMD_BYENT);
    pktSendBitsPack(pak_out, 1, dbid);
    pktSendBitsPack(pak_out, 1, 1);
    pktSendString(pak_out, buf);
    pktSend(&pak_out, &db_comm_link);

    estrDestroy(&buf);
}

void handleCustomData(Packet *pak)
{
    DbCustomDataCallback    cb;
    int                        db_id,count;
#if defined(_M_X64)
    #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
    cb        = (DbCustomDataCallback)(uintptr_t)pktGetBits(pak,32);
    db_id    = pktGetBitsPack(pak,1);
    count    = pktGetBitsPack(pak,1);
    cb(pak,db_id,count);
}


typedef int NetCallback(Packet *pak,NetLink *link);


void dbSendSaveCmd()
{
    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_SAVELISTS);
    pktSend(&pak,&db_comm_link);
    if (log_comm_link.connected)
    {
        pak = pktCreateEx(&log_comm_link,DBCLIENT_SAVELISTS);
        pktSend(&pak,&log_comm_link);
    }
}

void dbSendPlayerDisconnect(int db_id, int logout, int logout_login)
{
    Packet    *pak;

    assert(db_id > 0);

    LOG_OLD("sending disconnect to db_id: %d\n",db_id);
    pak = pktCreateEx(&db_comm_link,DBCLIENT_PLAYER_DISCONNECT);
    pktSendBitsPack(pak,1,db_id);
    pktSendBitsPack(pak,1,logout);
    pktSendBitsPack(pak,2,logout_login);
    pktSend(&pak,&db_comm_link);
}


U32 dbSecondsSince2000()
{
    // HACK: Just returning timerSeconds since time_offset seems to be very
    //       wrong.
    return timerSecondsSince2000() /* + db_state.time_offset */;
}

void contactLauncher()
{
    NetLink    launcher_link;
    Packet    *pak;
    int        my_pid;

    ZeroStruct(&launcher_link);
    if (!netConnect(&launcher_link,"127.0.0.1",DEFAULT_LAUNCHER_PORT,NLT_TCP,2.f,NULL))
        return;
    my_pid = _getpid();
    pak = pktCreateEx(&launcher_link, LAUNCHERQUERY_REGISTER);
    pktSendBitsPack(pak,1,db_state.map_id);
    pktSendBitsPack(pak,1,my_pid);
    pktSendBitsPack(pak,10,db_state.cookie);
    pktSend(&pak,&launcher_link);
    netSendDisconnect(&launcher_link,2.f);
}

void dbPlayerKicked(int player_id,int banned)
{
    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_PLAYER_KICKED);
    pktSendBitsPack(pak,1,player_id);
    pktSendBitsPack(pak,1,banned);
    pktSend(&pak,&db_comm_link);
}

void dbReqArenaAddress(int syncronous)
{
    static PerformanceInfo* perfInfo;
    Packet    *pak;
    pak = pktCreateEx(&db_comm_link,DBCLIENT_REQ_ARENA_ADDRESS);
    pktSend(&pak,&db_comm_link);
    if (syncronous)
        dbMessageScanUntil(__FUNCTION__, &perfInfo);
}

// list_id - container type (CONTAINTER_ENTS)
// table - sub-table to search in ents ex: "SuperGroupStats"
// search - what you are looking for (sql syntax)
//  Ex: "Where SupergroupsId = 15"
// field - what you are searching for
// value - value field has to match to return something (supergroup number 8)
// columns - all columns you want returned when match found
// Callback thing - my function to parse it
// db_id - where to put data when we have it
void dbReqCustomData(int list_id,char *table,char *limit,char *search,char *columns,DbCustomDataCallback cb,int db_id)
{
    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_REQ_CUSTOM_DATA);
#if defined(_M_X64)
    #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
    pktSendBits(pak,32,(U32)(uintptr_t)cb);
    pktSendBitsPack(pak,1,db_id);
    pktSendString(pak,limit);
    pktSendString(pak,search);
    pktSendString(pak,columns);
    pktSendBitsPack(pak,1,list_id);
    pktSendString(pak,table);
    pktSend(&pak,&db_comm_link);
}

void dbRequestPlayNCAuthKey(Entity * e, Packet *pak)
{
    Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_GET_PLAYNC_AUTH_KEY);
    pktSendBitsAuto(pak_out, e->auth_id);
    pktSendGetBitsAuto(pak_out, pak);    // request_key
    pktSendGetString(pak_out, pak);        // auth_name
    pktSendGetString(pak_out, pak);        // digest_data
    pktSend(&pak_out,&db_comm_link);
}

void dbReqSgChannelInvite(Entity * e, char * channel, int min_rank)
{
// only included by mapserver
#ifndef AUX_SERVER_DEFINE
    Packet    *pak = pktCreateEx(&db_comm_link,DBCLIENT_REQ_SG_CHANNEL_INVITE);

    pktSendBitsPack(pak, 1, getAuthId(e));
    pktSendBitsPack(pak, 1, e->db_id);
    pktSendBitsPack(pak, 1, e->supergroup_id);
    pktSendBitsPack(pak, 1, min_rank);
    pktSendString(pak, channel);
    pktSend(&pak,&db_comm_link);
#endif
}

static const char* getNextValueFromString( const char* buffer, int* pIntVal, 
                                                char* strVal, size_t strValSz,
                                                bool bTreatAsOneField /* ignore breaks in the text */ )
{
    //    Handle buffer as an integer (stringized) or a name. 
    //    If surrounded by quotes, force interpretation as a string,
    //    even if the value is otherwise all digits.
    int int_value = 0;
    const char* p = buffer;
    while (( *p != '\0' )  && ( ( *p >= '0') && ( *p <= '9') ))
    {
        int_value = (( int_value * 10 ) + ( *p - '0' ));
        p++;
    }
    if (( *p == '\0' ) || ( *p == ' ' ))
    {
        // all digits
        *pIntVal    = int_value;
        if ( strValSz > 0 )
        {
            strVal[0]    = '\0';
        }
        while (*p == ' ') p++;
    }
    else if ( strValSz > 0 )
    {
        size_t strSz = 0;
        bool bQuoted = false;
        char quote;
        *pIntVal    = 0;
        p = buffer;
        // Skip quotes and brackets around the name, if present.
        if ( *p == '\"' )
        {
            bQuoted = true;
            quote = '\"';
            p++;
        }
        else if ( *p == '\'' )
        {
            bQuoted = true;
            quote = '\'';
            p++;
        }
        while (( *p != '\0' ) && 
                ( (bQuoted && *p != quote) || (!bQuoted && *p != '\"' && *p != '\'') ) &&
                ( bQuoted || bTreatAsOneField || *p != ' ' ) &&
                ( strSz < ( strValSz - 1 )))
        {
            strVal[strSz++] = *p++;
        }
        strVal[strSz] = '\0';
        if (( *p == '\"' ) || ( *p == '\'' ))
        {
            p++;
        }
        while ( *p == ' ' ) p++;
    }
    return p;    // points to the next value on the string (or '\0').
}

void dbOfflineCharacter(ClientLink* client, char* dbIdStrOrCharName)
{
    #if defined (SERVER) && !defined(DBQUERY)
        Packet    *pak;
        int db_id;
        char playerName[80];
        EntityRef clientRef;

        assert(sizeof(EntityRef)==sizeof(S64));    // the dbserver assumes this
        clientRef = erGetRef(client->entity);
        
        getNextValueFromString(dbIdStrOrCharName,&db_id,playerName,ARRAY_SIZE(playerName), true);
        if ( db_id == 0 )
        {
            if (( db_id = dbPlayerIdFromName(playerName)) <= 0 )
            {
                conPrintf(client, "Unknown character name: '%s'.\n", playerName );
            }
        }
        if ( db_id <= 0 )
        {
            conPrintf( client, "Unable to find character data. Operation failed.\n" );
            return;
        }

        pak = pktCreateEx(&db_comm_link,DBCLIENT_OFFLINE_CHAR);
        pktSendBitsArray(pak,(sizeof(clientRef)*8),&clientRef);
        pktSendBitsPack(pak, 1, db_id);
        pktSend(&pak,&db_comm_link);
    #endif
}

#if defined (SERVER)
void dbOfflineCharacter_HandleRslt(Packet* pak)
{
    #if !defined(DBQUERY)
        // Received DBSERVER_OFFLINE_CHARACTER_RSLT from dbserver
        char* rsltStr;
        S64 clientRef;
        Entity* ent = NULL;
        ClientLink* client = NULL;
        
        pktGetBitsArray(pak,(sizeof(clientRef)*8),&clientRef);
        rsltStr = pktGetString(pak);
        if ((( ent = erGetEnt(clientRef) ) != NULL ) &&
            (( client = clientFromEnt(ent)) != NULL ))    // could it be?
        {
            conPrintf( client, "%s\n", rsltStr );
        }
    #endif
}
#endif

void dbRestoreDeletedChar(ClientLink* client, char* paramStr /* auth, char, date */)
{
    #if defined(SERVER) && !defined(DBQUERY)
        Packet    *pak;
        EntityRef clientRef;
        int auth_id;
        int db_id;
        char authName[80];
        char charName[80];
        char deletionDate[24];
        const char* paramPtr = paramStr;
        int seqNbr;
        
        paramPtr = getNextValueFromString( paramPtr,&auth_id,authName,ARRAY_SIZE(authName), false);
        paramPtr = getNextValueFromString( paramPtr,&db_id,charName,ARRAY_SIZE(charName), false);
        paramPtr = getNextValueFromString( paramPtr,&seqNbr,NULL,0,false);
        strncpy_s( deletionDate, ARRAY_SIZE(deletionDate), paramPtr, _TRUNCATE );
        
        // We send the char name as a string, since deleted characters are
        // not normally in the name cache.
        if ((( *authName == '\0' ) && ( auth_id == 0 ))|| 
            ( *charName == '\0' ) ||
            ( seqNbr < 1 ) ||
            ( strlen(deletionDate) != 7) || ( deletionDate[2] != '/'))
        {
            conPrintf( client, "Parameter error! Usage: <auth> <character name> <sequence-#> <deletion date MM/YYYY>\n" );
            return;
        }
        
        assert(sizeof(EntityRef)==sizeof(S64));    // the dbserver assumes this
        clientRef = erGetRef(client->entity);
        
        pak = pktCreateEx(&db_comm_link,DBCLIENT_RESTORE_DELETED_CHAR);
        pktSendBitsArray(pak,(sizeof(clientRef)*8),&clientRef);
        pktSendBitsPack(pak,1,auth_id);
        pktSendString(pak, authName);
        pktSendString(pak, charName);
        pktSendBitsPack(pak,1,seqNbr);
        pktSendString(pak, deletionDate);
        pktSend(&pak,&db_comm_link);
    #endif
}

#if defined (SERVER)
void dbRestoreDeletedChar_HandleRslt(Packet* pak)
{
    #if !defined(DBQUERY)
        // Received DBSERVER_OFFLINE_RESTORE_RSLT from dbserver
        char* rsltStr;
        S64 clientRef;
        Entity* ent = NULL;
        ClientLink* client = NULL;
        
        pktGetBitsArray(pak,(sizeof(clientRef)*8),&clientRef);
        rsltStr = pktGetString(pak);
        if ((( ent = erGetEnt(clientRef) ) != NULL ) &&
            (( client = clientFromEnt(ent)) != NULL ))    // could it be?
        {
            conPrintf( client, "%s\n", rsltStr );
        }
    #endif
}
#endif

void dbListDeletedChars(ClientLink* client, char* paramStr /* auth, date */)
{
    #if defined(SERVER) && !defined(DBQUERY)
        Packet    *pak;
        int auth_id;
        char authName[80];
        char deletionDate[16];
        EntityRef clientRef;
        const char* paramPtr = paramStr;

        paramPtr = getNextValueFromString( paramPtr,&auth_id,authName,ARRAY_SIZE(authName),false);
        strncpy_s( deletionDate, ARRAY_SIZE(deletionDate), paramPtr, _TRUNCATE );
        
        if ((( *authName == '\0' ) && ( auth_id == 0 )) ||
            ( strlen(deletionDate) != 7) || ( deletionDate[2] != '/'))
        {
            conPrintf( client, "Parameter error! Usage: <auth> <deletion date MM/YYYY>\n" );
            return;
        }
        assert(sizeof(EntityRef)==sizeof(S64));    // the dbserver assumes this
        clientRef = erGetRef(client->entity);

        pak = pktCreateEx(&db_comm_link,DBCLIENT_LIST_DELETED_CHARS);
        pktSendBitsArray(pak,(sizeof(EntityRef)*8),&clientRef);
        pktSendBitsPack(pak,1,auth_id);
        pktSendString(pak, authName);
        pktSendString(pak, deletionDate);
        pktSend(&pak,&db_comm_link);
    #endif
}

#if defined (SERVER)
void dbListDeletedChars_HandleRslt(Packet *pak)
{
    #if !defined(DBQUERY)
        // received deleted char list from dbserver (DBSERVER_SEND_DELETED_CHARS).
        int i;
        EntityRef clientRef;
        ClientLink* client;
        Entity* ent;
        int auth_id;
        char auth_name[80];
        int num_deleted;
        
        pktGetBitsArray(pak,(sizeof(clientRef)*8),&clientRef);
        if ((( ent = erGetEnt(clientRef) ) != NULL ) &&
            (( client = clientFromEnt(ent)) != NULL ))
        {
            auth_id        = pktGetBitsPack(pak,1);
            strcpy_s( auth_name, ARRAY_SIZE(auth_name), pktGetString(pak) );
            num_deleted    = pktGetBitsPack(pak,1);
        
            conPrintf( client, "# deleted players for %s: %d\n", auth_name, num_deleted );
            if ( num_deleted > 0 )
            {
                conPrintf( client, "-------------------------------------------------------------------\n" );
                conPrintf( client, "Name                             Seq-# Deletion-Date       Lvl Type\n" );
                conPrintf( client, "-------------------------------------------------------------------\n" );
                for ( i=0; i < num_deleted; i++ )
                {
                    char* playerName;
                    U32 dateDeleted;
                    char dateStr[64];
                    int level;
                    int archetype;
                                    
                    playerName    = pktGetString(pak);
                    dateDeleted    = pktGetBitsPack(pak,1);
                    level        = pktGetBitsPack(pak,1);
                    archetype    = pktGetBitsPack(pak,1);
                    
                    timerMakeDateStringFromSecondsSince2000( dateStr, dateDeleted ),                    
                    conPrintf( client, "%-32s   %02d  %s %3d %3d\n", playerName, (i+1), dateStr, level, archetype );
                }
                conPrintf( client, "-------------------------------------------------------------------\n" );
            }
        }
    #endif
}
#endif

void dbOverrideStartZone(int map_id)
{
    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_OVERRIDE_START_ZONE);
    pktSendBitsPack(pak, 1, map_id);
    pktSend(&pak,&db_comm_link);
}

void handleSgChannelInvite(Packet *pak)
{
// only included by mapserver
#ifndef AUX_SERVER_DEFINE
    handleSgChannelInviteHelper(pak);
#endif
}


static handleReceiveZMQstatus(Packet *pak)
{
    #if defined(SERVER) && !defined(DBQUERY)
        U32 db_id = pktGetBitsAuto(pak);
        char* statusString = pktGetString(pak);
        Entity* e = entFromDbId(db_id);
        ClientLink* client;
        if ( e && (( client = clientFromEnt(e)) != NULL )) 
        {
            conPrintf( client, "%s\n", statusString);
        }
    #endif
}


void dbExecuteSql(char *sql_command)
{
    Packet *pak = pktCreateEx(&db_comm_link,DBCLIENT_EXECUTE_SQL);

    pktSendString(pak,sql_command);
    pktSend(&pak,&db_comm_link);
}

void sgrpstatserver_ReceiveCmd(Packet *pak);
void lpactstats_removeInactive(Packet *pak);
void miningdata_Receive(Packet *pak);

#if defined(SERVER) && !defined(DBQUERY)
#include "comm_game.h"
#include "svr/svr_base.h"

#include "gameComm/sendToClient.h"
#include "language/langServerUtil.h"

static void s_mapserver_MessageEntity(Packet *pak_in)
// this should really be a meta function
// ideally, entid should get some mark up to let a mapservers know if it can handle the call itself
// void mapserver_messageEntity(ENTITYID entid, int channel, FORMAT fmt, ...)
{
    int entid = pktGetBitsAuto(pak_in);
    Entity *e = entFromDbId(entid);
    if(e)
    {
        intptr_t argv[4];    // arbitrary size for now
                            // if you change this, fix the actual calls below
                            // this'd be cleaner if we faked out a va_list instead
        int argc = 0;
        char *s;

        int channel = pktGetBitsAuto(pak_in);
        char *fmt = pktGetStringTemp(pak_in);
        for(s = fmt; s = strchr(s, '%'); )
        {
            switch(*++s)
            {
                xcase 's':    argv[argc++] = (intptr_t)pktGetStringTemp(pak_in);
                xcase 'd':    argv[argc++] = pktGetBitsAuto(pak_in);
                xcase '%':    // do nothing
                xdefault:    assertmsgf(0, "Unhandled printf type '%c'", *fmt);
            }
        }
        assertmsg(argc <= ARRAY_SIZE(argv), "overflowed max printf params");

        {    // FIXME: this should always be messagestore translation, with fmt automatically added to the messagestore
            char *buf = estrTemp();
            char *message = buf;

            if(isLocalizeable(e, fmt))
                message = localizedPrintf(e, fmt, argv[0], argv[1], argv[2], argv[3]);
            else
                estrConcatf(&buf, fmt, argv[0], argv[1], argv[2], argv[3]);

            if(channel < 0 && e->client)
                conPrintf(e->client, "%s", message);
            else
                sendInfoBoxAlreadyTranslated(e, channel, message);

            estrDestroy(&buf);
        }
    }
}
#endif

int dbMessageCallback(Packet *pak,int cmd,NetLink *link)
{
    U32                    user_data;
    NetPacketCallback    *cb;
    int                    cursor = bsGetCursorBitPosition(&pak->stream); // for debugging

    switch(cmd)
    {
        xcase DBSERVER_CONTAINERS:
            user_data        = pktGetBitsPack(pak,1);
            if (user_data)
            {
#if defined(_M_X64)
                #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
                cb = (NetPacketCallback*)(uintptr_t)user_data;
                return cb(pak,cmd,link);
            }
            else
                return dbReceiveContainers(pak,link);
        xcase DBSERVER_CONTAINER_INFO:
            dbQueryReceiveInfo(pak);
        xcase DBSERVER_CONTAINER_ACK:
            dbHandleContainerAck(pak);
        xcase DBSERVER_CONTAINER_STATUS:
            user_data        = pktGetBitsPack(pak,1);
            if (user_data)
            {
#if defined(_M_X64)
                #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
                cb = (NetPacketCallback*)(uintptr_t)user_data;
                cb(pak,cmd,link);
            }
            else
            {
                dbReceiveContainerStatus(pak);
            }
        xcase DBSERVER_MAP_XFER_LOADING:
            handleDbNewMapLoading(pak);
        xcase DBSERVER_MAP_XFER_READY:
            handleDbNewMapReady(pak);
        xcase DBSERVER_MAP_XFER_OK:
            handleDbNewMap(pak);
        xcase DBSERVER_MAP_XFER_FAIL:
            handleDbNewMapFail(pak);
        xcase DBSERVER_MAP_REG_CALLBACK:
            handleMapRegisterCallback(pak);
        xcase DBSERVER_SEND_DOORS:
            handleServerDoorUpdate(pak);
        xcase DBSERVER_BROADCAST_MSG:
            handleBroadcastMsg(pak);
        xcase DBSERVER_DISCONNECT_ACK:
            ;
        xcase DBSERVER_TIMEOFFSET:
            db_state.time_offset = pktGetBits(pak,32) - timerSecondsSince2000();
            db_state.timeZoneDelta = pktGetF32(pak);
            #define MAX_MINUTES_SERVER_CAN_BE_OFF_FROM_DB 30
            if( db_state.time_offset > 60 * MAX_MINUTES_SERVER_CAN_BE_OFF_FROM_DB )
                FatalErrorf( "This machine's UTC is off from the DB machine's UTC by %d minutes.  Please make sure both DB and server's system clocks are set to the correct UTC.", db_state.time_offset );
            //if (db_state.server_synch_time_cb)
            //    db_state.server_synch_time_cb();
            serverSynchTime();
        xcase DBSERVER_OVERRIDDEN_AUTHBITS:
            // This code file gets included in lots of projects. The only
            // one that needs this data is the MapServer, everyone else can
            // just dump it. They should still handle the message gracefully
            // in case they get sent it by mistake, otherwise they will
            // crash.
            pktGetBitsArray(pak,sizeof(g_overridden_auth_bits)*8,&g_overridden_auth_bits);

        xcase DBSERVER_DISABLED_ZONE_EVENTS:
            {
                int i;
                char *eventName;

                i = pktGetBitsAuto(pak);
                while (i > 0)
                {
                    eventName = pktGetString(pak);
#if defined(SERVER) && !defined(DBQUERY)
                    ScriptedZoneEventDisableEvent(eventName);
#endif
                    i--;
                }
            }
        xcase DBSERVER_NOTIFY_REACTIVATION:
#if defined(SERVER) && !defined(DBQUERY)
            authUserSetReactivationActive(1);
#endif
        xcase DBSERVER_SEND_ENT_NAMES:
            handleSendEntNames(pak);
        xcase DBSERVER_SEND_GROUP_NAMES:
            handleSendGroupNames(pak);
        xcase DBSERVER_FORCE_LOGOUT:
            handleForceLogout(pak);
        xcase DBSERVER_CONTAINER_ID:
            handleContainerID(pak);
        xcase DBSERVER_SHUTDOWN:
            handleShutdown(pak);
        xcase DBSERVER_WHO:
            strncpy(db_state.who_msg,(char*)unescapeString(pktGetString(pak)),sizeof(db_state.who_msg));
        xcase DBSERVER_RELAY_CMD:
            handleRelayCmd(pak);
        xcase DBSERVER_RELAY_CMD_RESPONSE:
            handleRelayCmdResponse(pak);
        xcase DBSERVER_LOST_MEMBERSHIP:
            handleLostMembership(pak);
        xcase DBSERVER_CLIENT_CMD_FAILED:
            return handleClientCmdFailed(pak);
        xcase DBSERVER_OFFLINE_CHARACTER_RSLT:
            dbOfflineCharacter_HandleRslt(pak);
        xcase DBSERVER_SEND_DELETED_CHARS:
            dbListDeletedChars_HandleRslt(pak);
        xcase DBSERVER_OFFLINE_RESTORE_RSLT:
            dbRestoreDeletedChar_HandleRslt(pak);
        xcase DBSERVER_ONLINE_ENTS:
            handleOnlineEnts(pak);
        xcase DBSERVER_ONLINE_ENT_COMMENTS:
            handleOnlineEntComments(pak);
        xcase DBSERVER_CUSTOM_DATA:
            handleCustomData(pak);
        xcase DBSERVER_CLEAR_PNAME_CACHE_ENTRY:
            handleClearPnameCacheEntry(pak);
        xcase DBSERVER_TEAM_LEFT_MISSION:
            handleTeamLeftMission();
        xcase DBSERVER_ARENA_ADDRESS:
            handleArenaAddress(pak);
        xcase DBSERVER_SG_ELDEST_ON:
            db_state.sg_eldest_on = pktGetBits(pak, 32);
            strncpy(db_state.sg_eldest_on_name, unescapeString(pktGetString(pak)), sizeof(db_state.sg_eldest_on_name));
        xcase DBSERVER_SG_CHANNEL_INVITE:
            handleSgChannelInvite(pak);
        xcase DBSERVER_CONTAINER_RECEIPT:
            handleContainerReceipt(pak);
        xcase DBSERVER_CONTAINER_RELAY:
            handleContainerRelay(pak);
        xcase DBSERVER_CONTAINER_REFLECT:
            handleDbContainerReflect(pak);
        xcase DBSERVER_MISSION_PLAYER_COUNT:
            handleMissionPlayerCountResponse(pak);
        xcase DBSERVER_BACKUP_SEARCH:
            handleDbBackupSearch(pak);
        xcase DBSERVER_BACKUP_APPLY:
            handleDbBackupApply(pak);
        xcase DBSERVER_BACKUP_VIEW:
            handleDbBackupView(pak);
        xcase DBSERVER_SOME_ONLINE_ENTS:
         {
            user_data        = pktGetBitsPack(pak,1);
            if (user_data)
            {
#if defined(_M_X64)
                #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
                cb = (NetPacketCallback*)(uintptr_t)user_data;
                return cb(pak,cmd,link);
            }
         }
        xcase DBSERVER_UPDATE_LOG_LEVELS:
        {
            int i;
            for( i = 0; i < LOG_COUNT; i++ )
            {
                int log_level = pktGetBitsPack(pak,1);
                logSetLevel(i,log_level);
            }
#if defined(SERVER) && !defined(DBQUERY) 
            if( ArenaAsyncConnect() )
            { 
                Packet * pak_out = pktCreateEx(&arena_comm_link, ARENACLIENT_SET_LOG_LEVELS);
                for( i = 0; i < LOG_COUNT; i++ ) 
                    pktSendBitsPack(pak_out, 1, logLevel(i) );
                pktSend(&pak_out, &arena_comm_link);
            }

            { // raidserver
                Packet* pak_out = dbContainerRelayCreate(RAIDCLIENT_SET_LOG_LEVELS, CONTAINER_BASERAIDS, 0, 0, 0);
                for( i = 0; i < LOG_COUNT; i++ ) 
                    pktSendBitsPack(pak_out, 1, logLevel(i) );
                pktSend(&pak_out, &db_comm_link);
            }
#endif
        }
        xcase DBSERVER_TEST_LOG_LEVELS:
        {
            LOG( LOG_DEPRECATED, LOG_LEVEL_ALERT, LOG_CONSOLE, "Dbserver/StatServer Log Line Test: Alert" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "Dbserver/StatServer  Log Line Test: Important" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "Dbserver/StatServer  Log Line Test: Verbose" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, LOG_CONSOLE, "Dbserver/StatServer  Log Line Test: Deprecated" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_DEBUG, LOG_CONSOLE, "Dbserver/StatServer  Log Line Test: Debug" );
        }


#if defined(SERVER) && !defined(DBQUERY) // auction on mapservers only
        xcase DBSERVER_AUCTION_SEND_INV:
        case DBSERVER_AUCTION_SEND_HIST:
        case DBSERVER_AUCTION_XACT_ABORT:
        case DBSERVER_AUCTION_XACT_COMMIT:
        case DBSERVER_AUCTION_XACT_CMD:
        case DBSERVER_AUCTION_XACT_FAIL:
        case DBSERVER_AUCTION_BATCH_INFO:
         {
             extern int auction_dbMessageCallback(Packet *pak,int cmd,NetLink *link);
             auction_dbMessageCallback(pak,cmd,link);
         }
        break;
        xcase DBSERVER_ACCOUNTSERVER_CATALOG:
        {    
            cacheAccountServerCatalogUpdate( pak );
            sendAccountCatalogUpdateToClients( pak );

            cfg_setIsAutoBuyProducts(accountCatalog_IsAutoBuyEnabled());
        }
        xcase DBSERVER_ACCOUNTSERVER_CHARCOUNT:
        {
            int dbid = pktGetBitsAuto(pak);
            Entity *e = entFromDbId(dbid);

            if(e)
            {
                int shards = pktGetBitsAuto(pak);
                int count;

                START_PACKET(pak_out, e, SERVER_ACCOUNTSERVER_CHARCOUNT);
                pktSendBitsAuto(pak_out, shards);
                if(shards)
                {
                    for (count = 0; count < shards; count++)
                    {
                        pktSendGetString(pak_out, pak);        // Shard name
                        pktSendGetBitsAuto(pak_out, pak);    // Unlocked character count
                        pktSendGetBitsAuto(pak_out, pak);    // Total character count
                        pktSendGetBitsAuto(pak_out, pak);    // Account slot count, excludes VIP slots
                        pktSendGetBitsAuto(pak_out, pak);    // Server slot count
                        pktSendGetBitsAuto(pak_out, pak);    // VIP-only status
                    }
                }
                else
                {
                    pktSendGetString(pak_out, pak); // errmsg
                }
                END_PACKET;
            }
        }
        xcase DBSERVER_ACCOUNTSERVER_INVENTORY:
        {
            bool authoritative = pktGetBits(pak, 1);
            bool enablePlayerCheck = pktGetBool(pak);
            U32 dbid = pktGetBitsAuto(pak);
            Entity *e = entFromDbIdEvenSleeping(dbid);
            int i, size, count;
            AccountInventory *pItem = NULL;
            char *pShardName = NULL;

            strdup_alloca(pShardName, pktGetString(pak));

            if(e && e->pl && (!e->pl->accountInformationAuthoritative || authoritative))
            {
                int bonus_slots;

                e->pl->accountInformationAuthoritative = authoritative;

                // destroy any old inventory information
                AccountInventorySet_Release( &e->pl->account_inventory );

                size = pktGetBitsAuto(pak);

                for (i = 0; i < size; i++)
                {
                    CertificationRecord * pRec;

                    pItem = (AccountInventory *) calloc(1,sizeof(AccountInventory));
                    AccountInventoryReceiveItem(pak, pItem);
                    if (!AccountInventoryAddProductData(pItem))
                    {
                        free(pItem);
                        continue;
                    }

                    eaPush(&e->pl->account_inventory.invArr, pItem);

                    pRec = certificationRecordInHistory(e, pItem->recipe );
                    if( pRec )
                    {
                        TODO(); // this behavior can no longer be be supported as is
#if 0
                        pItem->deleted = pRec->deleted; // could optimize by not sending deleted items to client
#endif

                        if( pItem->invType == kAccountInventoryType_Certification ) // certs can be claimed on each character so look at local claim record instead of accountserver record
                            pItem->claimed_total = pRec->claimed;
                    }
                    else
                    {
                        if( pItem->invType == kAccountInventoryType_Certification ) // certs can be claimed on each character so look at local claim record instead of accountserver record
                            pItem->claimed_total = 0;
                    }

                    if (pItem && pItem->invType == kAccountInventoryType_Certification || pItem->invType == kAccountInventoryType_Voucher)
                    {
                        if (db_state.is_static_map)
                        {
                            const MapSpec *spec = MapSpecFromMapId(db_state.base_map_id);
                            StaticMapInfo *info = staticMapInfoFind(db_state.base_map_id);
                            if (spec && info && !info->introZone)
                            {
                                if (getPopHelpStateByTag(e, "CODEPH_CheckEmailCertificate") == PopHelpState_Untriggered)
                                {
                                    triggerPopHelpEventHappenedByTag(e, "CODEPH_CheckEmailCertificate");
                                }
                            }
                        }
                    }
                }
                AccountInventorySet_Sort( &e->pl->account_inventory );

                pktGetBitsArray(pak, LOYALTY_BITS, e->pl->loyaltyStatus);
                e->pl->loyaltyPointsUnspent = pktGetBitsAuto(pak);    
                e->pl->loyaltyPointsEarned = pktGetBitsAuto(pak);
                e->pl->virtualCurrencyBal = pktGetBitsAuto(pak);
                e->pl->lastEmailTime = pktGetBitsAuto(pak);
                e->pl->lastNumEmailsSent = pktGetBitsAuto(pak);
                e->pl->account_inventory.accountStatusFlags = pktGetBitsAuto(pak);
                bonus_slots = pktGetBitsAuto(pak);
                
                // This is the first place we can do work, because only
                // now is the account inventory stored on the player.
                e->pl->account_inv_loaded = true;

                // setting the mission server slot count 
                count = AccountGetStoreProductCount( &(e->pl->account_inventory), SKU("svmaslot"), false );
                if (count > 0)
                {
                    missionserver_map_setPaidPublishSlots(e, count);
                }

                accountCatalogServerAwardGlobalProducts(e);
                costumeAwardAuthParts( e );

                costumeValidateCostumes(e);  // check to see if they bought a new one!

                if (enablePlayerCheck && MapCheckDisallowedPlayer(e))
                {
                    forceLogout(e->db_id, 34);
                    break; 
                }

                // copied from team_AcceptRelay
                // we could have accepted a team invite before we had the account info,
                // so we validate now that we're guaranteed to be accurate.
                if (e->teamup && e->teamup->activetask && TeamTaskValidateStoryTask(e, &e->teamup->activetask->sahandle) != TTSE_SUCCESS)
                {
                    chatSendToTeamup( e, localizedPrintf( e, "couldNotJoinYourTeam", e->name ), INFO_SVR_COM );
                    team_MemberQuit(e);
                }

                if(e->pl && e->pl->accountInformationAuthoritative)    //make sure that the badges know what's going on.
                {
                    badge_RecordAccountInventoryUpdate(e);
                }

                //    if the char is loaded from a sleeping state (which we allow for offline command usage)
                //    the pchar info will be NULL and shouldn't be used -EL
                //    I'm not sure if we should extend this to other code in this section, but a cursory review seems to be okay with offlined chars
                if (e->pchar && e->pchar->entParent)
                {
                    character_ActivateAllAutoPowers(e->pchar);
                    character_GrantAutoIssuePowers(e->pchar);        // grant powers that should be granted automatically
                    character_ResetBoostsets(e->pchar);                // reapply all set bonuses 

                    // update contact information, as this may have changed based on new purchased content
                    ContactStatusSendAll(e);
                    ContactStatusAccessibleSendAll(e);
                }

                sendAccountInventoryToClient(e, pShardName, bonus_slots);

                // Players are required to create characters in a tutorial until they
                // either finish a tutorial or they log in an existing character
                // that is past the tutorial.
                if(authoritative
                    && e->pl->praetorianProgress != kPraetorianProgress_Tutorial
                    && db_state.base_map_id != 41
                    && !AccountHasStoreProduct(ent_GetProductInventory(e), SKU("inttutor")))
                {
                    AccountStoreBuyProduct(e->auth_id, SKU("inttutor"), 1);
                }
            }
        }
        xcase DBSERVER_ACCOUNTSERVER_LOYALTY:
        {
            int dbid = pktGetBitsAuto(pak);
            Entity *e = entFromDbIdEvenSleeping(dbid);  // this is valid since this xact is used during login

            if (e)
            {
                pktGetBitsArray(pak, LOYALTY_BITS, e->pl->loyaltyStatus);
            }
        }
        xcase DBSERVER_ACCOUNTSERVER_CERTIFICATION_GRANT:
        {
            SkuId sku_id = { pktGetBitsAuto2(pak) };
            OrderId order_id = { pktGetBitsAuto2(pak), pktGetBitsAuto2(pak) };
            int dbid = pktGetBitsAuto(pak);
            Entity *e = entFromDbId(dbid);
            int result = 0;
            const AccountProduct *pProduct = accountCatalogGetProduct(sku_id);
            const DetailRecipe *pRecipe = pProduct ? detailrecipedict_RecipeFromName(pProduct->recipe) : NULL;
            CertificationRecord *pRecord;
            bool isDuplicate = false;
            int orderIndex;

            if (!e)    // this can happen if the player has not been loaded yet into the map - delay until later
            {
                // can't send this information back without an auth_id...
                //AccountServerTransactionFinish(order_id, false);
                break;
            }

            if (server_state.accountCertificationTestStage % 100 == 5)
            {
                int flags = server_state.accountCertificationTestStage / 100;

                if (flags % 10 == 0)
                {
                    server_state.accountCertificationTestStage = 0;
                }

                if (flags / 10 == 1)
                {
                    AccountServerTransactionFinish(e->auth_id, order_id, false);
                }

                break;
            }

            for (orderIndex = eaSize(&e->pl->pending_orders) - 1; orderIndex >= 0; orderIndex--)
            {
                if (orderIdEquals(*(e->pl->pending_orders[orderIndex]), order_id))
                {
                    isDuplicate = true;
                    break;
                }
            }

            if (!isDuplicate)
            {
                for (orderIndex = eaSize(&e->pl->completed_orders) - 1; orderIndex >= 0; orderIndex--)
                {
                    if (orderIdEquals(*(e->pl->completed_orders[orderIndex]), order_id))
                    {
                        isDuplicate = true;
                        break;
                    }
                }
            }

            if (isDuplicate)
            {
                // duplicates could be automatic retries, so don't send any messages.
                break;
            }
            
            pRecord = certificationRecordInHistory(e, pRecipe->pchName);

            if( !pRecord )
            {
                // we received update on record we don't have, assume we crashed or something and make a new one
                pRecord = certificationRecord_Create();
                estrConcatCharString(&pRecord->pchRecipe, pRecipe->pchName);
                eaPush(&e->pl->ppCertificationHistory, pRecord);
            }

            // Even if account server didn't allow request, reset this flag.  Assuming the next sendInventory will
            // clear up any inconsistencies
            pRecord->status = kCertStatus_None;

            //
            // LSTL 03/31/11 - If a recipe is purchased, we neither check nor consume the requirements.
            // The user's having paid for the item with an MTX obviates that. ...I think...
            // Just in case I'm wrong about that, or in case removing this call causes unintended (bad)
            // side effects, I'm taking the precaution of only commenting it out.
            //
            
            // VJD 4/13/11 - Recipes for vouchers will need to consume requirements, as that is how they are being used
            //    already by the game in other ways.  Recipes for store products will have no in-game requirements.

            if( pRecipe )  // this will consume the requirements, but shouldn't actually give them anything yet.
                result = character_DetailRecipeConsumeRequirements(e->pchar, pRecipe, DETAILRECIPE_USE_RECIPE_LEVEL, 0, NULL);

            if( !pRecipe || result <= 0 )
            {
                AccountServerTransactionFinish(e->auth_id, order_id, false);
                chatSendToPlayer(dbid, localizedPrintf(e,"CertificationBuyFailedRequirements"), INFO_USER_ERROR, 0);
            }
            else
            {
                OrderId *copy = (OrderId*)malloc(sizeof(OrderId));
                memcpy(copy, &order_id, sizeof(OrderId));
                eaPush(&e->pl->pending_orders, copy);

                AccountServerTransactionFinish(e->auth_id, order_id, true);
                e->pchar->auctionInvUpdated; // force save
            }
        }
        xcase DBSERVER_ACCOUNTSERVER_CERTIFICATION_CLAIM:
        {
            int dbid = pktGetBitsAuto(pak);
            SkuId sku_id = { pktGetBitsAuto2(pak) };
            const AccountProduct *pProduct = accountCatalogGetProduct(sku_id);
            const DetailRecipe *pRecipe = pProduct ? detailrecipedict_RecipeFromName(pProduct->recipe) : NULL;
            Entity *e = entFromDbId(dbid);
            OrderId order_id = { pktGetBitsAuto2(pak), pktGetBitsAuto2(pak) };
            CertificationRecord *pRecord;
            bool isDuplicate = false;
            int orderIndex;

            if (!e)
                break;

            if (server_state.accountCertificationTestStage % 100 == 5)
            {
                int flags = server_state.accountCertificationTestStage / 100;

                if (flags % 10 == 0)
                {
                    server_state.accountCertificationTestStage = 0;
                }

                if (flags / 10 == 1)
                {
                    AccountServerTransactionFinish(e->auth_id, order_id, false);
                }

                break;
            }

            if( !pRecipe )
            {
                AccountServerTransactionFinish(e->auth_id, order_id, false);
                chatSendToPlayer(dbid, localizedPrintf(e,"CertificationClaimFailedBadRecipeName"), INFO_USER_ERROR, 0);
                break;
            }

            for (orderIndex = eaSize(&e->pl->pending_orders) - 1; orderIndex >= 0; orderIndex--)
            {
                if (orderIdEquals(*(e->pl->pending_orders[orderIndex]), order_id))
                {
                    isDuplicate = true;
                    break;
                }
            }

            if (!isDuplicate)
            {
                for (orderIndex = eaSize(&e->pl->completed_orders) - 1; orderIndex >= 0; orderIndex--)
                {
                    if (orderIdEquals(*(e->pl->completed_orders[orderIndex]), order_id))
                    {
                        isDuplicate = true;
                        break;
                    }
                }
            }

            if (isDuplicate)
            {
                // duplicates could be automatic retries, so don't send any messages.
                break;
            }

            pRecord = certificationRecordInHistory(e, pRecipe->pchName);

            if( !pRecord )
            {
                // we received update on record we don't have, assume we crashed or something and make a new one
                pRecord = certificationRecord_Create();
                estrConcatCharString(&pRecord->pchRecipe, pRecipe->pchName);
                eaPush(&e->pl->ppCertificationHistory, pRecord);
            }

            // Even if account server didn't allow request, reset this flag.  Assuming the next sendInventory will
            // clear up any inconsistencies
            pRecord->status = kCertStatus_None;
            // give item, let account server know about it
            if( character_DetailRecipeGrantResults(e->pchar, pRecipe, DETAILRECIPE_USE_RECIPE_LEVEL, 0, 0, 0) > 0 )
            {
                OrderId *copy = (OrderId*)malloc(sizeof(OrderId));
                memcpy(copy, &order_id, sizeof(OrderId));
                eaPush(&e->pl->pending_orders, copy);

                AccountServerTransactionFinish(e->auth_id, order_id, 1);
                pRecord->claimed++;
                e->pchar->auctionInvUpdated; // force save

                //TODO: Replace this line with new log JSON line
                //log successful claim
                LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Claim SKUID: %.8s Recipe: %s Title: %s", pProduct->sku_id.c, pProduct->recipe, pProduct->title ? pProduct->title : "None");
            }
            else
            {
#if defined (SERVER) && !defined(DBQUERY)
                if (pRecipe->ppchReceiveRequires != NULL && pRecipe->pchDisplayReceiveRequiresFail != NULL &&
                    !chareval_Eval(e->pchar, pRecipe->ppchReceiveRequires, pRecipe->pchSourceFile))
                {
                    chatSendToPlayer(dbid, localizedPrintf(e, pRecipe->pchDisplayReceiveRequiresFail), INFO_USER_ERROR, 0);
                } else {
                    chatSendToPlayer(dbid, localizedPrintf(e,"CertificationClaimFailedNoInventory"), INFO_USER_ERROR, 0);
                }
#else
                chatSendToPlayer(dbid, localizedPrintf(e,"CertificationClaimFailedNoInventory"), INFO_USER_ERROR, 0);
#endif
                AccountServerTransactionFinish(e->auth_id, order_id, 0);
            }
        }
        xcase DBSERVER_ACCOUNTSERVER_CERTIFICATION_REFUND:
        {
            TODO(); // Refunds are disabled for now
#if 0
            int dbid = pktGetBitsAuto(pak);
            SkuId sku_id = { pktGetBitsAuto2(pak) };
            const AccountProduct *pProduct = accountCatalogGetProduct(sku_id);
            const DetailRecipe *pRecipe = pProduct ? detailrecipedict_RecipeFromName(pProduct->recipe) : NULL;
            Entity *e = entFromDbId(dbid);
            int success = pktGetBitsAuto(pak);
            OrderId order_id = { pktGetBitsAuto2(pak), pktGetBitsAuto2(pak) };
            char * msg= pktGetString(pak);
            CertificationRecord *pRecord;

            if( !e )
                AccountServerAck(order_id, 0);
            else if (server_state.accountCertificationTestStage % 100 == 5)
            {
                int flags = server_state.accountCertificationTestStage / 100;

                if (flags % 10 == 0)
                {
                    server_state.accountCertificationTestStage = 0;
                }

                if (flags / 10 == 1)
                {
                    AccountServerTransactionFinish(e->auth_id, order_id, false);
                }
            }
            else if( !pRecipe )
            {
                AccountServerAck(order_id, 0);
                chatSendToPlayer(dbid, localizedPrintf(e,"CertificationRefundFailedBadRecipeName"), INFO_USER_ERROR, 0);
            }
            else
            {

                pRecord = certificationRecordInHistory(e, pRecipe->pchName);

                if( !pRecord )
                {
                    // we received update on record we don't have, assume we crashed or something and make a new one
                    pRecord = certificationRecord_Create();
                    estrConcatCharString(&pRecord->pchRecipe, pRecipe->pchName);
                    eaPush(&e->pl->ppCertificationHistory, pRecord);
                }

                // Even if account server didn't allow request, reset this flag.  Assuming the next sendInventory will
                // clear up any inconsistencies
                pRecord->status = kCertStatus_None;
                if( !success )
                {
                    if( msg && *msg)
                        chatSendToPlayer(dbid, localizedPrintf(e,msg), INFO_USER_ERROR, 0);
                }
                else
                {
                    // give item, let account server know about it
                    if( character_DetailRecipeRefundRequirements(e->pchar, pRecipe, DETAILRECIPE_USE_RECIPE_LEVEL) )
                    {
                        OrderId *copy = (OrderId*)malloc(sizeof(OrderId));
                        memcpy(copy, &order_id, sizeof(OrderId));
                        eaPush(&e->pl->pending_orders, copy);

                        AccountServerAck(order_id, 1);
                        e->pchar->auctionInvUpdated; // force save
                    }
                    else
                    {
                        AccountServerAck(order_id, 0);
                        chatSendToPlayer(dbid, localizedPrintf(e,"CertificationRefundFailedNoInventory"), INFO_USER_ERROR, 0);
                    }
                }
            }
#endif
        }
        xcase DBSERVER_ACCOUNTSERVER_MULTI_GAME_TRANSACTION:
        {
            char *salvageName = pktGetString(pak);
            OrderId order_id = { pktGetBitsAuto2(pak), pktGetBitsAuto2(pak) };
            int dbid = pktGetBitsAuto(pak);
            Entity *e = entFromDbId(dbid);
            int result = 0;
            bool isDuplicate = false;
            int orderIndex;
            SalvageItem const *sal = NULL;
            bool goodToGo = true;

            if (!e)    // this can happen if the player has not been loaded yet into the map - delay until later
            {
                // can't send this information back without an auth_id...
                //AccountServerTransactionFinish(order_id, false);
                break;
            }

            if (server_state.accountCertificationTestStage % 100 == 5)
            {
                int flags = server_state.accountCertificationTestStage / 100;

                if (flags % 10 == 0)
                {
                    server_state.accountCertificationTestStage = 0;
                }

                if (flags / 10 == 1)
                {
                    AccountServerTransactionFinish(e->auth_id, order_id, false);
                }

                break;
            }

            for (orderIndex = eaSize(&e->pl->pending_orders) - 1; orderIndex >= 0; orderIndex--)
            {
                if (orderIdEquals(*(e->pl->pending_orders[orderIndex]), order_id))
                {
                    isDuplicate = true;
                    break;
                }
            }

            if (!isDuplicate)
            {
                for (orderIndex = eaSize(&e->pl->completed_orders) - 1; orderIndex >= 0; orderIndex--)
                {
                    if (orderIdEquals(*(e->pl->completed_orders[orderIndex]), order_id))
                    {
                        isDuplicate = true;
                        break;
                    }
                }
            }

            if (isDuplicate)
            {
                // duplicates could be automatic retries, so don't send any messages.
                break;
            }

            //
            // LSTL 03/31/11 - If a recipe is purchased, we neither check nor consume the requirements.
            // The user's having paid for the item with an MTX obviates that. ...I think...
            // Just in case I'm wrong about that, or in case removing this call causes unintended (bad)
            // side effects, I'm taking the precaution of only commenting it out.
            //

            // VJD 4/13/11 - Recipes for vouchers will need to consume requirements, as that is how they are being used
            //    already by the game in other ways.  Recipes for store products will have no in-game requirements.

            if (salvageName[0])
            {
                sal = salvage_GetItem(salvageName);
                goodToGo = character_CanAdjustSalvage(e->pchar, sal, -1);
            }
            // else goodToGo is true from initialization, we didn't have a salvage to remove.

            if (goodToGo)
            {
                OrderId *copy = (OrderId*)malloc(sizeof(OrderId));
                memcpy(copy, &order_id, sizeof(OrderId));
                eaPush(&e->pl->pending_orders, copy);

                if (salvageName[0])
                    character_AdjustSalvage(e->pchar, sal, -1, "open", false);

                AccountServerTransactionFinish(e->auth_id, order_id, true);
                e->pchar->auctionInvUpdated; // force save
            }
            else
            {
                AccountServerTransactionFinish(e->auth_id, order_id, false);
                chatSendToPlayer(dbid, localizedPrintf(e,"CertificationBuyFailedRequirements"), INFO_USER_ERROR, 0);
            }
        }
        xcase DBSERVER_ACCOUNTSERVER_MULTI_GAME_TRANSACTION_COMPLETED:
        {
            U32 auth_id = pktGetBitsAuto(pak);
            char *xactName = pktGetString(pak);
            OrderId order_id = {pktGetBitsAuto2(pak), pktGetBitsAuto2(pak)};
            int quantity = pktGetBitsAuto(pak);
            int dbid = pktGetBitsAuto(pak);
            int subIndex;
            Entity *e = entFromDbId(dbid);

            if (e)
            {
                START_PACKET(pak_out, e, SERVER_OPEN_SALVAGE_RESULTS);
                pktSendString(pak_out, xactName);
                pktSendBitsAuto(pak_out, quantity);

                for (subIndex = 0; subIndex < quantity; subIndex++)
                {
                    SkuId skuId = {pktGetBitsAuto2(pak)};
                    int quantity = pktGetBitsAuto(pak); // granted / quantity
                    int claimed = pktGetBitsAuto(pak); // claimed (unused)
                    int rarity = pktGetBitsAuto(pak);
                    const char *rarityStr = StaticDefineIntRevLookup(AccountTypes_RarityEnum, rarity);

                    pktSendString(pak_out, skuId.c);
                    pktSendBitsAuto(pak_out, quantity);
                    pktSendBitsAuto(pak_out, rarity);

                    //TODO: Replace this line with new log JSON line
                    LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "SuperPackClaim Package: %s, ItemNumber: %d, Product: %.8s, Quantity: %d, Rarity: %s",
                        xactName, subIndex + 1, skuId.c, quantity, rarityStr);
                }
                END_PACKET
            }
        }
        xcase DBSERVER_ACCOUNTSERVER_NOTIFYTRANSACTION:
        {
            // FIXME: put this in a function somewhere and remove comm_game.h and svr_base.h includes
            int dbid = pktGetBitsAuto(pak);
            Entity *e = entFromDbId(dbid);

            if(e)
            {
                SkuId sku_id = { pktGetBitsAuto2(pak) };
                int invType = pktGetBitsAuto(pak);
                OrderId order_id = { pktGetBitsAuto2(pak), pktGetBitsAuto2(pak) };
                int granted = pktGetBitsAuto(pak);
                int claimed = pktGetBitsAuto(pak);
                bool success = pktGetBool(pak);

                e->pl->last_certification_request = 0;
            }
        }
        xcase DBSERVER_ACCOUNTSERVER_NOTIFYSAVED:
        {
            U32 dbid = pktGetBitsAuto(pak);
            Entity *e = entFromDbId(dbid);

            if (e && e->pl)
            {
                int size = eaSize(&e->pl->completed_orders);
                OrderId order_id = { pktGetBitsAuto2(pak), pktGetBitsAuto2(pak) };
                bool success = pktGetBool(pak);
                int i;

                if (success)
                {
                    for (i=size-1; i>=0; i--)
                        if (e->pl->completed_orders[i] && orderIdEquals(*e->pl->completed_orders[i], order_id))
                            eaRemoveAndDestroy(&e->pl->completed_orders, i, NULL);
                }
            }
        }
        xcase DBSERVER_ACCOUNTSERVER_CLIENTAUTH:
        {
            // FIXME: put this in a function somewhere and remove comm_game.h and svr_base.h includes
            int dbid = pktGetBitsAuto(pak);
            Entity *e = entFromDbId(dbid);

            if(e)
            {
                START_PACKET(pak_out, e, SERVER_ACCOUNTSERVER_CLIENTAUTH);
                pktSendGetBitsAuto(pak_out,pak);    // timestamp
                pktSendGetString(pak_out,pak);    // playSpanAuthDigest
                END_PACKET;
            }
        }
        xcase DBSERVER_ACCOUNTSERVER_PLAYNC_AUTH_KEY:
        {
            // FIXME: put this in a function somewhere and remove comm_game.h and svr_base.h includes
            int dbid = pktGetBitsAuto(pak);
            Entity *e = entFromDbId(dbid);

            if(e)
            {
                START_PACKET(pak_out, e, SERVER_ACCOUNTSERVER_PLAYNC_AUTH_KEY);
                pktSendGetBitsAuto(pak_out,pak);    // request_key
                pktSendGetString(pak_out,pak);    // PlayNC auth key
                END_PACKET;
            }
        }
        xcase DBSERVER_ACCOUNTSERVER_NOTIFYREQUEST:
        {
            // FIXME: put this in a function somewhere and remove comm_game.h and svr_base.h includes
            int dbid = pktGetBitsAuto(pak);
            Entity *e = entFromDbId(dbid);

            if(e)
            {
                AccountRequestType reqType = pktGetBitsAuto(pak);
                OrderId order_id = { pktGetBitsAuto2(pak), pktGetBitsAuto2(pak) };
                bool success = pktGetBool(pak);
                int pendingIndex;

                e->pl->last_certification_request = 0;

                for (pendingIndex = eaSize(&e->pl->pending_orders) - 1; pendingIndex >= 0; pendingIndex--)
                {
                    if (orderIdEquals(*(e->pl->pending_orders[pendingIndex]), order_id))
                    {
                        entMarkOrderCompleted(e, order_id);
                        eaRemoveAndDestroy(&e->pl->pending_orders, pendingIndex, NULL);
                        break;
                    }
                }

                // grants and claims already have their own message system
                if (reqType != kAccountRequestType_CertificationGrant
                    && reqType != kAccountRequestType_CertificationClaim
                    && reqType != kAccountRequestType_MultiTransaction
                    && reqType != kAccountRequestType_UnknownTransaction) // assume this is one of the above
                {
                    START_PACKET(pak_out, e, SERVER_ACCOUNTSERVER_NOTIFYREQUEST);
                    pktSendBitsAuto(pak_out, reqType);
                    pktSendBitsAuto2(pak_out, order_id.u64[0]);
                    pktSendBitsAuto2(pak_out, order_id.u64[1]);
                    pktSendBool(pak_out,success);
                    pktSendGetString(pak_out,pak);        // message
                    END_PACKET;
                }
            }
        }
        xcase DBSERVER_CLIENT_MESSAGE:
        {
            int dbid = pktGetBitsAuto(pak);
            char *message = pktGetString(pak);
            Entity *e = entFromDbId(dbid);

            if (e)
            {
                chatSendToPlayer(e->db_id, localizedPrintf(e, message), INFO_SVR_COM, 0);
            }
        }
        xcase DBSERVER_MISSIONSERVER_SEARCHPAGE:
            missionserver_map_receiveSearchPage(pak);
        xcase DBSERVER_MISSIONSERVER_ARCINFO:
            missionserver_map_receiveArcInfo(pak);
        xcase DBSERVER_MISSIONSERVER_BAN_STATUS:
            missionserver_map_receiveBanStatus(pak);
        xcase DBSERVER_MISSIONSERVER_ARCDATA:
            missionserver_map_receiveArcData(pak);
        xcase DBSERVER_MISSIONSERVER_ARCDATA_OTHERUSER:
            missionserver_map_receiveArcDataOtherUser(pak);
        xcase DBSERVER_MISSIONSERVER_INVENTORY:
            missionserver_map_receiveInventory(pak);
        xcase DBSERVER_MISSIONSERVER_CLAIM_TICKETS:
            missionserver_map_receiveTickets(pak);
        xcase DBSERVER_MISSIONSERVER_ITEM_BOUGHT:
            missionserver_map_itemBought(pak);
        xcase DBSERVER_MISSIONSERVER_ITEM_REFUND:
            missionserver_map_itemRefund(pak);
        xcase DBSERVER_MISSIONSERVER_OVERFLOW_GRANTED:
            missionserver_map_overflowGranted(pak);
        xcase DBSERVER_MESSAGEENTITY:
            s_mapserver_MessageEntity(pak);
        xcase DBSERVER_CREATE_EMAIL:
            createEmailFromDbServer(pak);
        xcase DBSERVER_MISSIONSERVER_ALLARCS:
            missionServerMapTest_ReceiveAllArcs(pak);
        xcase DBSERVER_EVENT_WAIT_TIMES:
            turnstileMapserver_handleEventWaitTimes(pak);
        xcase DBSERVER_QUEUE_STATUS:
            turnstileMapserver_handleQueueStatus(pak);
        xcase DBSERVER_EVENT_READY:
            turnstileMapserver_handleEventReady(pak);
        xcase DBSERVER_EVENT_READY_ACCEPT:
            turnstileMapserver_handleEventReadyAccept(pak);
        xcase DBSERVER_EVENT_FAILED_START:
            turnstileMapserver_handleEventFailedStart(pak);
        xcase DBSERVER_MAP_START:
            turnstileMapserver_handleMapStart(pak);
        xcase DBSERVER_EVENT_START:
            turnstileMapserver_handleEventStart(pak);
        xcase DBSERVER_TURNSTILE_PONG:
            turnstileMapserver_handleTurnstilePong(pak);
        xcase DBSERVER_EVENTHISTORY_RESULTS:
            eventHistory_ReceiveResults(pak);
        xcase DBSERVER_REJOIN_FAIL:
            turnstileMapserver_handleRejoinFail(pak);
#endif
        // container server messages
#ifdef CONTAINER_SERVER
        xcase DBSERVER_CONTAINER_SERVER_ACK:
        case DBSERVER_CONTAINER_SERVER_NOTIFY:
            csvrDbMessage(cmd,pak);
#endif // CONTAINER_SERVER

#if STATSERVER 
             // ab: hack in here for now. refactor into statserver itself later. 
             // see statserver.c:dbComm() call
        xcase DBSERVER_SGRP_STATSADJ: 
             sgrpstats_ReceiveToQueue(pak);
        xcase DBSERVER_SEND_STATSERVER_CMD: 
             sgrpstatserver_ReceiveCmd(pak);
        xcase DBSERVER_MININGDATA_RELAY:
             miningdata_Receive(pak);
        xcase DBSERVER_SEND_STATSERVER_INACTIVE_ACCOUNT:
             lpactstats_removeInactive(pak);
#endif

 #if RAIDSERVER
        // ignore turnstile commands to prevent spewing "Unknown dbserver command" below
        xcase DBSERVER_EVENT_WAIT_TIMES:
        xcase DBSERVER_QUEUE_STATUS:
        xcase DBSERVER_EVENT_READY:
        xcase DBSERVER_EVENT_READY_ACCEPT:
        xcase DBSERVER_EVENT_FAILED_START:
        xcase DBSERVER_MAP_START:
        xcase DBSERVER_EVENT_START:
        xcase DBSERVER_TURNSTILE_PONG:
#endif

        xdefault:
            printf("Unknown dbserver command: %d\n",cmd);
            return 0;
    }
    return 1;
}

void dbDelinkMeWrapper(char *errorMsg)
{
    if (!IsDebuggerPresent() && !disconnected_from_dbserver) {
        dbDelinkMe(errorMsg);
    }
    #if !DBQUERY
    logDisconnectFlush();
    #endif
}

void dbDelinkMe(char *errorMsg)
{
    int        sock;
    int        len;
    struct sockaddr_in    addr;
    struct
    {
        int map_id;
        int process_id;
        int cookie;
    } map;

    map.map_id = db_state.map_id;
    map.process_id = _getpid();
    map.cookie = db_state.cookie;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockSetAddr(&addr,ipFromString(db_state.server_name),DEFAULT_DBCRASHMAP_PORT);
    connect(sock,(void *)&addr,sizeof(addr));
    send(sock,(void*)&map,sizeof(map),0);
    len = (int)strlen(errorMsg)+1;
    send(sock, (char*)&len, sizeof(len),0);
    send(sock, errorMsg, len, 0);
    closesocket(sock);
}

void dbCommStartup(){
    dbNameTableInit();
    initNetLink(&db_comm_link);
}

int dbRelayGetAccountServerCharacterCounts(U32 auth_id, bool vip)
{
    int retval = 0;

    if (db_comm_link.socket && NLT_NONE != db_comm_link.type)
    {
        Packet *pak = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_CHARCOUNT);
        pktSendBitsAuto(pak, auth_id);
        pktSendBool(pak, vip);
        pktSend(&pak, &db_comm_link);
        retval = 1;
    }

    return retval;
}

int dbRelayGetAccountServerCatalog(ClientLink *client)
{
    #if defined (SERVER) && !defined(DBQUERY)
        Packet* pak_out = pktCreateEx(client->link, DBGAMESERVER_ACCOUNTSERVER_CATALOG );
        accountCatalog_AddAcctServerCatalogToPacket( pak_out );
        pktSend(&pak_out, client->link);
    #endif
    return 1;
}

int dbRelayGetAccountServerInventory(U32 auth_id)
{
    int retval = 0;

    if (db_comm_link.socket && NLT_NONE != db_comm_link.type)
    {
        Packet *pak = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_GET_INVENTORY);
        pktSendBitsAuto(pak, auth_id);
        pktSend(&pak, &db_comm_link);
        retval = 1;
    }

    return retval;
}

#if defined(SERVER) && !defined(DBQUERY) // auction on mapservers only
void cacheAccountServerCatalogUpdate( Packet* pak_in )
{
    int cursor_byte                = pak_in->stream.cursor.byte;
    int cursor_bit                = pak_in->stream.cursor.bit;

    accountCatalog_CacheAcctServerCatalogUpdate( pak_in );

    pak_in->stream.cursor.byte    = cursor_byte;
    pak_in->stream.cursor.bit    = cursor_bit;

}

void sendAccountCatalogUpdateToClients( Packet* pak_in )
{
    int i;
    PlayerEnts    *players = &player_ents[PLAYER_ENTS_ACTIVE];

    for(i=0;i<players->count;i++)
    {
        int cursor_byte            = pak_in->stream.cursor.byte;
        int cursor_bit            = pak_in->stream.cursor.bit;
        {
            START_PACKET(pak_out, players->ents[i], SERVER_ACCOUNTSERVER_CATALOG);
            accountCatalog_RelayServerCatalogPacket( pak_in, pak_out );
            END_PACKET
        }
        pak_in->stream.cursor.byte = cursor_byte;
        pak_in->stream.cursor.bit    = cursor_bit;
    }
}

#endif
