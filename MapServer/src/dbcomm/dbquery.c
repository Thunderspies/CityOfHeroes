#include <utilitieslib/network/netio.h>
#include <utilitieslib/components/EString.h>
#include "dbcomm.h"
#include "dbcontainer.h"
#include <utilitieslib/utils/utils.h>
#include "dbquery.h"
#include <utilitieslib/utils/timing.h>
#include "staticMapInfo.h"
#include <utilitieslib/utils/file.h>
#include "comm_backend.h"
#include <utilitieslib/network/netcomp.h>
#include "entity/classes.h"
#include "entity/origins.h"
#include <utilitieslib/components/earray.h>
#include "group/group.h"
#include "auth/authUserData.h"
#include "cmdparse/cmdserver.h"
#include "entity/entity.h"
#include "container/team.h"
#include <utilitieslib/utils/log.h>


StuffBuff query_buf;

// Used only for removing fields that don't exist in this build
static bool meetsPutFilter(const char *s)
{
    static char *beginnings[] = {
        "NumBonusPowerSets", "NumBonusPowers"
    };
    int i;
    // Look for things we don't want copied
    for (i=0; i<ARRAY_SIZE(beginnings); i++) {
        if (strnicmp(s, beginnings[i], strlen(beginnings[i]))==0) {
            return 1;
        }
    }
    return 0;
}

// Used for removing non-transferable attributes
bool dbQueryGetFilter(const char *line)
{
    static char *beginnings[] = {"supergroupsid ", "teamupsid ", "taskforcesid ", "Friends[", "BuddyDbId", "BuddyType", "Ents2[0].LevelingPactsId", "Ents2[0].LeagueId"
    };
    int i;
    // Look for things we don't want copied
    for (i=0; i<ARRAY_SIZE(beginnings); i++) {
        if (strnicmp(line, beginnings[i], strlen(beginnings[i]))==0) {
            return 1;
        }
    }
    // Reset DbId
    if (strstri((char*)line, "AssignedDbId "))
        return 1;
    return 0;
}

int dbQueryGetContainerIDForName(int type, char *db_id)
{
    char buf[2000] = "";
    char *s;
    int iRet=atoi(db_id);

    if(iRet==0)
    {
        // Let's guess that db_id is a name instead
        sprintf(buf, "-dbquery -find %d name \"%s\"", type, db_id);
        s = dbQuery(buf);
        if(s && strnicmp("\ncontainer_id = ", s, 16)==0)
        {
            char *pch=strstr(s, " = ");
            if(pch)
            {
                iRet = atoi(pch+3);
            }
        }
        else
        {
            printf("Unable to get container_id for \"%s\"\n", db_id);
        }
    }

    return iRet;
}

#include "container/baseloadsave.h"
static int dbQueryPrintContainers(Packet *pak,int cmd,NetLink *link)
{
    int                i;
    ContainerInfo    ci;
    int        list_id,count;

    list_id    = pktGetBitsPack(pak,1);
    count    = pktGetBitsPack(pak,1);

    for(i=0;i<count;i++)
    {
        if (!dbReadContainerUncached(pak,&ci,list_id))
        {
            if (ci.error_code == CONTAINER_ERR_DOESNT_EXIST)
                addStringToStuffBuff(&query_buf,"container %d doesnt exist\n",ci.id);
            if (ci.error_code == CONTAINER_ERR_ALREADY_LOCKED)
                addStringToStuffBuff(&query_buf,"container %d is already locked\n",ci.id);
        }
        else
        {
            addStringToStuffBuff(&query_buf,"%s\n",ci.data);
        }
    }
    return 1;
}

static int is_online;

static void dbQueryReceiveStatus(Packet *pak,int cmd,NetLink *link)
{
    int        list_id,count,i,valid;
    char    *s;

    is_online = 1;
    list_id = pktGetBitsPack(pak,1);
    count = pktGetBitsPack(pak,1);
    for(i=0;i<count;i++)
    {
        valid = pktGetBitsPack(pak,1);
        if (!valid)
        {
            addStringToStuffBuff(&query_buf,"invalid container request\n");
            is_online = 0;
            continue;
        }
        s = pktGetString(pak);
        addStringToStuffBuff(&query_buf,"%s\n",s);
    }
}

void dbQueryReceiveInfo(Packet *pak)
{
    int        i,count;
    char    *str;

    count = pktGetBitsPack(pak,1);
    for(i=0;i<count;i++)
    {
        str = pktGetString(pak);
        addStringToStuffBuff(&query_buf,"%s\n",str);
    }
}

static void netlatency(int hit_sql)
{
    int        i,map_num,timer,loopcount=10000;
    ContainerStatus c_list[1];

    timer = timerAlloc();
    timerStart(timer);
    c_list[0].id = 30;
    for(i=0;i<loopcount;i++)
    {
        if (hit_sql)
            dbSyncContainerFindByElement(CONTAINER_ENTS,"ContainerId","1",&map_num,1);
        else
            dbSyncContainerStatusRequest(1,c_list,1);
    }
    printf("ready: %d\n",c_list[0].ready);
    printf("%f transactions/sec (%d in %f seconds)\n",loopcount/timerElapsed(timer),loopcount,timerElapsed(timer));
    timerFree(timer);
}

int getFileOrStdin(StuffBuff *sb,char *fname)
{
    int        i,c;

    initStuffBuff(sb, 10000);

    if (fname) {
        // Passed a filename
        char *data = fileAlloc(fname, NULL), *s, *t;
        char tempfname[MAX_PATH];
        if (!data) {
            sprintf(tempfname, ".\\%s", fname);
            data = fileAlloc(tempfname, NULL);
        }
        if (!data) {
            return 0;
        }
        // Strip \rs, we just want \ns
        for (s = t = data; *t = *s; s++)
        {
            if (*s != '\r')
                t++;
        }
        // Strip filterables
        for (s=data; *s; s++) {
            if (s==data || *(s-1)=='\n') {
                if (meetsPutFilter(s)) {
                    char *end = strchr(s, '\n');
                    if (!end) {
                        *s=0;
                        break;
                    } else {
                        strcpy(s, end+1);
                        s--;
                    }
                }
            }
        }

        addSingleStringToStuffBuff(sb, data);
        fileFree(data);
    } else {
        for(i=0;;i++)
        {
    #                undef getc
            c = getc(stdin);
            if (c == EOF)
            {
                break;
            }
            addStringToStuffBuff(sb, "%c", c);
        }
    }
    return 1;
}

typedef struct SgLeaderInfo
{
    int id;
    int rank;
} SgLeaderInfo;

static S32 compareSgLeaderInfo(SgLeaderInfo const **l, SgLeaderInfo const **r)
{
    return (*l)->rank - (*r)->rank;
}

static SgLeaderInfo **g_sgleader_infos = NULL;
static void sgleader_customdatacb(Packet *pak,int db_id,int count)
{
    eaDestroyEx(&g_sgleader_infos,NULL);
    for(;count > 0;--count)
    {
        SgLeaderInfo *i = malloc(sizeof(SgLeaderInfo));
        i->id = pktGetBitsPack(pak, 1);
        i->rank = pktGetBitsPack(pak, 1);
        eaPush(&g_sgleader_infos,i);
    }
    qsort(g_sgleader_infos,eaSize(&g_sgleader_infos), sizeof(*g_sgleader_infos), compareSgLeaderInfo);
}


char *dbQuery(char *query_cmd)
{
    int        i,map_id=0,map_xfer=0,get_status=0,set_container=0,get_container=0,list_id,container_id,find_element=0;
    Packet    *pak;
    char* argv[100];
    char* element = NULL;
    char* value = NULL;
    int        argc;
    int timeout = 100000000;
    char *set_container_fn = NULL;

    query_buf.idx = 0;
    if (!query_buf.buff)
        initStuffBuff(&query_buf,1000);
    argc = tokenize_line(query_cmd,argv,0);

    db_state.query_mode = 1;

    for(i=0;i<argc;i++)
    {
        if (stricmp(argv[i], "-timeout")==0 && argc - i >= 2)
        {
            timeout = atoi(argv[i+1]);
        }
        if (stricmp(argv[i],"-get")==0 && argc - i >= 3)
        {
            get_container = 1;
            list_id = atoi(argv[i+1]);
            container_id = dbQueryGetContainerIDForName(list_id, argv[i+2]);
        }
        if (stricmp(argv[i],"-getstatus")==0 && argc - i >= 3)
        {
            get_status = 1;
            list_id = atoi(argv[i+1]);
            container_id = atoi(argv[i+2]);
        }
        if (stricmp(argv[i],"-set")==0 && argc - i >= 3)
        {
            set_container = 1;
            list_id = atoi(argv[i+1]);
            container_id = atoi(argv[i+2]);
            set_container_fn = ((argc-i)>=4?argv[i+3]:NULL); // get filename, if exists
        }
        if (stricmp(argv[i],"-find")==0 && argc - i >= 4)
        {
            find_element = 1;
            list_id = atoi(argv[i+1]);
            element = argv[i+2];
            value = argv[i+3];
        }
        if (stricmp(argv[i],"-shutdown")==0 && argc - i >= 2)
        {
            static PerformanceInfo* perfInfo;

            pak = pktCreateEx(&db_comm_link,DBCLIENT_SHUTDOWN);
            pktSendString(pak,argv[i+1]);
            pktSend(&pak,&db_comm_link);
            dbMessageScanUntilTimeout("DBCLIENT_SHUTDOWN", &perfInfo, timeout);
#if defined SERVER && !defined DBQUERY
            groupCleanUp(); // Clean up ctris that might be in shared memory
#endif
            logShutdownAndWait();
            exit(0);
        }
        if (stricmp(argv[i],"-delink")==0 && argc - i >= 2)
        {
            pak = pktCreateEx(&db_comm_link,DBCLIENT_DISCONNECT_MAPSERVER);
            pktSendBitsPack(pak,1,atoi(argv[i+1]));
            pktSend(&pak,&db_comm_link);
            lnkBatchSend(&db_comm_link);
            Sleep(1);
            lnkBatchSend(&db_comm_link);
            return query_buf.buff;
        }
        if (stricmp(argv[i],"-latency")==0)
        {
            netlatency(0);
            return 0;
        }
        if (stricmp(argv[i],"-latency2")==0)
        {
            netlatency(1);
            return 0;
        }
        if (stricmp(argv[i],"-getbase")==0 && argc - i >= 2)
        {
            char    *str;
            int        supergroup_id,user_id;

            supergroup_id = atoi(argv[i+1]);
            user_id = 0;
            str = baseLoadText(supergroup_id,user_id);
            printf("%s",str);
            return "";
        }
        if (stricmp(argv[i],"-setbase")==0 && argc - i >= 2)
        {
            int            supergroup_id,user_id;
            StuffBuff    sb;
            static PerformanceInfo* perfInfo;

            supergroup_id = atoi(argv[i+1]);
            user_id = 0;
            if (getFileOrStdin(&sb,argc==3 ? argv[i+2] : 0));
                baseSaveText(supergroup_id,user_id,sb.buff);
            dbMessageScanUntilTimeout("savebase",&perfInfo, 2000.f);
            return last_db_error;
        }
#define IS_CMD(CMD,N_ARGS)                                  \
        (stricmp(strchrRemoveStatic(argv[i],'_'),CMD)==0    \
                           && argc - i > N_ARGS)
        if (IS_CMD("-sgleader",1))
        {
            char *where = NULL;
            int sgid = 0;
            char *sgname = argv[i+1];
            if(strIsNumeric(sgname)) 
                sgid = atoi(sgname);
            else
                sgid = dbQueryGetContainerIDForName(CONTAINER_SUPERGROUPS, sgname);
            estrPrintf(&where,"WHERE ContainerId = %i", sgid);
            dbReqCustomData(CONTAINER_SUPERGROUPS, "SgrpMembers", 0, where, "dbid, rank", sgleader_customdatacb, 0);
            
            if(dbMessageScanUntil(__FUNCTION__,NULL) && eaSize(&g_sgleader_infos))
            {
                int i;
                int num_leaders = 0;
                printf("sgrp %s leaders (name,rank): ", sgname);
                for( i = eaSize(&g_sgleader_infos) - 1; i >= 0 && num_leaders < 5; --i)
                {
                    int id = g_sgleader_infos[i]->id;
                    char *query = NULL;
                    char *s;
                    char *name;
                    estrPrintf(&query,"-dbquery -get 3 %i", id);
                    s = dbQuery(query);
                    if(!s || !(name=strstri(s,"\nname")))
                    {
                        printf("(couldn't get %i), ", id);
                    }
                    else
                    {
                        *strchr(name+sizeof("\r\nname"),'\n') = 0;
                        printf("%s, %i; ", strchr(name,'\"'), g_sgleader_infos[i]->rank);
                        num_leaders++;
                    }
                    estrDestroy(&query);
                }
                printf("\n");
            }
            else
            {
                printf("couldn't find any leaders for sg %s\n",sgname);
            }
            estrDestroy(&where);
        }
    }
    if (set_container)
    {
        static PerformanceInfo* perfInfo[2];
        int cmd=CONTAINER_CMD_TEMPLOAD;
        StuffBuff sb;

        if (!getFileOrStdin(&sb,set_container_fn))
            return "File not found.";
        if (container_id < 0)
            cmd = CONTAINER_CMD_CREATE;
        dbAsyncContainerUpdate(list_id,container_id,cmd,sb.buff,0);

        freeStuffBuff(&sb);

        if (!dbMessageScanUntilTimeout(cmd == CONTAINER_CMD_TEMPLOAD ? "CONTAINER_CMD_TEMPLOAD_1" : "CONTAINER_CMD_CREATE", &perfInfo[cmd == CONTAINER_CMD_TEMPLOAD], timeout))
            return last_db_error;
    }
    else if (get_container)
    {
        static PerformanceInfo* perfInfo;

        dbAsyncContainerRequest(list_id,container_id,CONTAINER_CMD_TEMPLOAD_OFFLINE,dbQueryPrintContainers);
        if (!dbMessageScanUntilTimeout("CONTAINER_CMD_TEMPLOAD_2", &perfInfo, timeout))
            return "(dbQuery failed)";
    }
    else if (map_xfer)
    {
        pak = pktCreateEx(&db_comm_link,DBCLIENT_TEST_MAP_XFER);
        pktSendBitsPack(pak,1,list_id);
        pktSendBitsPack(pak,1,map_id);
        pktSendBitsPack(pak,1,1);
        pktSendBitsPack(pak,1,container_id);
        pktSend(&pak,&db_comm_link);
    }
    else if (get_status)
    {
        static PerformanceInfo* perfInfo;

        pak = pktCreateEx(&db_comm_link,DBCLIENT_REQ_CONTAINER_STATUS);
#if defined(_M_X64)
        #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
        pktSendBitsPack(pak,1,(U32)(uintptr_t)dbQueryReceiveStatus);
        pktSendBitsPack(pak,1,list_id);
        pktSendBitsPack(pak,1,1);
        pktSendBitsPack(pak,1,container_id);
        pktSend(&pak,&db_comm_link);
        if (!dbMessageScanUntilTimeout("DBCLIENT_REQ_CONTAINER_STATUS", &perfInfo, timeout))
            return "(dbQuery failed)";
    }
    else if (find_element)
    {
        int container_id;

        container_id = dbSyncContainerFindByElementEx(list_id,element,value,0,0,1);
        addStringToStuffBuff(&query_buf,"\ncontainer_id = %d\n", container_id);
    }
    else
    {
        static PerformanceInfo* perfInfo;

        addStringToStuffBuff(&query_buf,"\n");
        addStringToStuffBuff(&query_buf,"-get <list id> <container id>\n");
        addStringToStuffBuff(&query_buf,"-set <list id> <container id> [file] | < file\n");
        addStringToStuffBuff(&query_buf,"-getbase <supergroup id>\n");
        addStringToStuffBuff(&query_buf,"-setbase <supergroup id> [file] | < file\n");
        addStringToStuffBuff(&query_buf,"-getstatus <list id> <container id>\n");
        addStringToStuffBuff(&query_buf,"-find <list_id> <element> <value>\n");
        addStringToStuffBuff(&query_buf,"-shutdown <shutdown message>\n");
        addStringToStuffBuff(&query_buf,"-delink <map id>\n");
        addStringToStuffBuff(&query_buf,"\n");

        pak = pktCreateEx(&db_comm_link,DBCLIENT_CONTAINER_INFO);
        pktSend(&pak,&db_comm_link);
        if (!dbMessageScanUntilTimeout("DBCLIENT_CONTAINER_INFO", &perfInfo, timeout))
            return "(dbQuery failed)";
    }
    return query_buf.buff;
}


int dbQueryGetCharacter(char* db_id, char *new_auth)
{
    char    buf[2000] = "",*s;
    int        ret=1;

    if (dbTryConnect(15.0, DEFAULT_DB_PORT)) {
        int dbid=dbQueryGetContainerIDForName(3, db_id);
        sprintf(buf, "-dbquery -get 3 %d", dbid);

        s = dbQuery(buf);
        if (s && !(ret=simpleMatch("container * doesnt exist\n", s))) 
        {
            ret = 0;
            printf("//containerid %i\n",dbid);
            s = strtok(s, "\r\n");
            while (s) {
                bool filtered = dbQueryGetFilter(s);

                // if you change this, you may need to change dbAsyncShardJump() as well
                if (new_auth && strnicmp(s, "AuthName", strlen("AuthName"))==0) {
                    printf("AuthName \"%s\"\n", new_auth);
                } else if (strnicmp(s, "MapId", strlen("MapId"))==0) {
                    // Hack to set the mapid to 0 so that an older buggy DbServer can log him out successfully.
                    printf("MapId 0\n");
                } else if (strnicmp(s, "FriendCount ", strlen("FriendCount "))==0) {
                    printf("FriendCount 0\n");
                } else {
                    printf("%s%s\n",(filtered?"// ":""),s);
                }
                s = strtok(NULL, "\r\n");
            }
        } else {
            // Error
            ret = 1;
        }
        if (db_comm_link.connected)
            netSendDisconnect(&db_comm_link,4.f);
    } else {
        printf("Failed to connect to DbServer ");
        ret = 1;
    }
    return ret;
}

int dbQueryPutCharacter(char *filename)
{
    char    buf[2000] = "",*s;
    int        ret=0;

    if (dbTryConnect(15.0, DEFAULT_DB_PORT))
    {
        if (filename) {
            sprintf(buf, "-dbquery -set 3 -1 %s", filename);
        } else {
            sprintf(buf, "-dbquery -set 3 -1");
        }
        s = dbQuery(buf);
        if (s && s[0]) {
            printf("%s",s);
            ret = 1; // Some error
        }
        if (db_comm_link.connected)
            netSendDisconnect(&db_comm_link,4.f);
    } else {
        printf("Failed to connect to DbServer ");
        ret = 1;
    }
    return ret;
}

int dbQueryWrapper(int argc, char **argv)
{
    char    buf[2000] = "",*s;
    int        i;

    for(i=0;i<argc;i++)
    {
        strcat(buf,"\"");
        strcat(buf,argv[i]);
        strcat(buf,"\" ");
    }
    if (dbTryConnect(3600.0, DEFAULT_DB_PORT)) {
        s = dbQuery(buf);
        if (s)
            printf("%s",s);
        if (db_comm_link.connected)
            netSendDisconnect(&db_comm_link,4.f);
        return 0;
    } else {
        printf("Failed to connect to DbServer\n");
    }
    return 1;
}

// expects a file of this form:
// Server: Freedom
// <supergroup name>
// <supergroup name>
// <supergroup name>
// ...
// Server: Virtue
// <supergroup name>
// <supergroup name>
// ...
// ...
int dbQueryGetSgLeaders(char *leaders_fn)
{
    int i = 0;
    char *buf = fileAlloc(leaders_fn,NULL);
    char *q = NULL;
    char *last = NULL;
    char **sgnames = NULL;
    int line = 1;
    int res = 1;
    if( buf )
    {
        char *tok;
        FOR_STRTOK(tok,"\r\n",buf)
        {
            if(simpleMatch("Server: *",tok))
            {
                char *server = getLastMatch(); // strstr(tok," ");
                if(!server)
                {
                    printf("couldn't parse server in line %i:%s\n",line,tok);
                    break;
                }
                strcpy(db_state.server_name,server);
                if(db_comm_link.connected)
                    netSendDisconnect(&db_comm_link,10.f);
                if(!dbTryConnect(20.f,DEFAULT_DB_PORT))
                {
                    printf("couldn't connect to dbserver %s\n",db_state.server_name);
                    break;
                }
            }
            else
            {
                estrPrintf(&q,"-sgleader \"%s\" ", tok);
                dbQuery(q);
            }
        }
        res = 0;
    }
    SAFE_FREE(buf);
    estrDestroy(&q);
    return res;
}


static int is_online;

static void dbQueryReceiveOnlineStatus(Packet *pak,int cmd,NetLink *link)
{
    int        list_id,count,i,valid;

    is_online = 1;
    list_id = pktGetBitsPack(pak,1);
    count = pktGetBitsPack(pak,1);
    for(i=0;i<count;i++)
    {
        valid = pktGetBitsPack(pak,1);
        if (!valid)
        {
            is_online = 0;
            continue;
        }
    }
}

int dbQueryOnline(int list_id,int container_id)
{
    static PerformanceInfo* perfInfo;

    Packet    *pak;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_REQ_CONTAINER_STATUS);
#if defined(_M_X64)
        #pragma message("The line below needs to be proper ported to 64 bit compatible code")
#endif
    pktSendBitsPack(pak,1,(U32)(uintptr_t)dbQueryReceiveOnlineStatus);
    pktSendBitsPack(pak,1,list_id);
    pktSendBitsPack(pak,1,1);
    pktSendBitsPack(pak,1,container_id);
    pktSend(&pak,&db_comm_link);
    dbMessageScanUntil("DBCLIENT_REQ_CONTAINER_STATUS", &perfInfo);

    return is_online;
}



static OnlinePlayerInfo    **online_players;
static int                online_player_count,online_player_max;

static cmpOnlineInfo(const OnlinePlayerInfo **a,const OnlinePlayerInfo **b)
{
    return (*a)->db_id - (*b)->db_id;
}


void handleOnlineEnts(Packet *pak)
{
    int                    i,id, num_bytes;
    OnlinePlayerInfo    *opi;
    StaticMapInfo        *info;
    Packet                tmp_pack = {0};

    staticMapInfoResetPlayerCount();

    // create a fake packet to cram zipped data into an read out of as if
    // it were a packet
    tmp_pack.stream.bitLength = pktGetBits(pak, 32);
    tmp_pack.stream.size = pktGetBits(pak, 32);
    tmp_pack.stream.data = pktGetZipped(pak, &num_bytes);
    tmp_pack.stream.byteAlignedMode = true;
    tmp_pack.hasDebugInfo = pak->hasDebugInfo;

    if( !online_players )
        eaCreate(&online_players);

    // loop overl list, update = false
    for( i = eaSize(&online_players)-1; i >= 0; i-- )
        online_players[i]->updated = false;

    for(;;)
    {
        bool push = false;

        id = pktGetBitsPack(&tmp_pack,20);
        if (!id)
            break;

        opi = dbGetOnlinePlayerInfo(id);
        if(!opi)
        {
            opi = calloc(1, sizeof(OnlinePlayerInfo));
            push = true;
        }

        // Since we use 2 or 3 bits for these quantities we need to know if
        // they grow past 4 or 8 valid values!
        STATIC_INFUNC_ASSERT(kPlayerType_Count <= 4);
        STATIC_INFUNC_ASSERT(kPlayerSubType_Count <= 4);
        STATIC_INFUNC_ASSERT(kPraetorianProgress_Count <= 8);

        opi->db_id                = id;
        opi->map_id                = pktGetBitsPack(&tmp_pack,8);
        opi->static_map_id        = pktGetBitsPack(&tmp_pack,8);
        opi->teamsize            = pktGetBitsPack(&tmp_pack,4);
        opi->leader                = pktGetBitsPack(&tmp_pack,1);
        opi->hidefield            = pktGetBitsPack(&tmp_pack,10);
        opi->lfg                = pktGetBitsPack(&tmp_pack,10);
        opi->level                = pktGetBitsPack(&tmp_pack,6);
        opi->archetype            = classes_GetIndexFromName( &g_CharacterClasses, pktGetString(&tmp_pack));
        opi->origin                = origins_GetIndexFromName( &g_CharacterOrigins, pktGetString(&tmp_pack));
        opi->type                = pktGetBitsPack(&tmp_pack, 2);
        opi->influenceType        = pktGetBitsPack(&tmp_pack, 2);
        opi->subtype            = pktGetBitsPack(&tmp_pack, 2);
        opi->praetorian            = pktGetBitsPack(&tmp_pack, 3);
        opi->arena_map            = pktGetBitsPack(&tmp_pack, 1);
        opi->mission_map        = pktGetBitsPack(&tmp_pack, 1);
        opi->can_co_faction        = pktGetBitsPack(&tmp_pack, 1);
        opi->faction_same_map    = pktGetBitsPack(&tmp_pack, 1);
        opi->can_co_universe    = pktGetBitsPack(&tmp_pack, 1);
        opi->universe_same_map    = pktGetBitsPack(&tmp_pack, 1);
        opi->last_active        = pktGetBits(&tmp_pack,32);
        opi->member_since        = pktGetBits(&tmp_pack,32);
        opi->prestige            = pktGetBits(&tmp_pack,32);
        opi->leaguesize            = pktGetBitsPack(&tmp_pack,8);
        opi->league_leader        = pktGetBitsPack(&tmp_pack,1);
        opi->updated            = true;

        if(push)
            eaPush(&online_players, opi);

        info = staticMapInfoFind(opi->map_id);
        if(info)
        {
            info->playerCount++;
            if (opi->type == kPlayerType_Hero)
                info->heroCount++;
            else
                info->villainCount++;
        }

        // DBQuery is sortof a mapserver but it doesn't have access to
        // the masses of shared-memory data.
#if !defined(DBQUERY)
        {
            const MapSpec *spec;

            spec = MapSpecFromMapId(opi->static_map_id);
            if(spec)
            {
                opi->team_area = spec->teamArea;
            }
            else
            {
                opi->team_area = MAP_TEAM_EVERYBODY;
            }
        }
#endif
    }
    if( tmp_pack.stream.data )
        free(tmp_pack.stream.data);

    // if update wasn't set, the entry is no longer valid
    for( i = eaSize(&online_players)-1; i >= 0; i-- )
    {
        if(!online_players[i]->updated)
        {
            opi = eaRemove(&online_players, i);
            if( opi )
                free( opi );
        }
    }

    online_player_count = eaSize(&online_players);

    qsort(online_players,online_player_count,sizeof(void*),cmpOnlineInfo);
}

void handleOnlineEntComments(Packet *pak)
{
    OnlinePlayerInfo *opi;
    for(;;)
    {
        int id = pktGetBitsPack(pak,20);
        if (!id)
            break;

        opi = dbGetOnlinePlayerInfo(id);
        if( opi )
            strncpyt( opi->comment, unescapeString(pktGetString(pak)), MAX_NAME_LEN );
        else
            pktGetString(pak); // read it out of queue, but don't do anything with it
    }
}

OnlinePlayerInfo *dbGetOnlinePlayerInfo(int db_id)
{
    OnlinePlayerInfo **match, *search_ptr, search;

    if(!online_players)
        return NULL;

    search.db_id = db_id;
    search_ptr = &search;
    match = bsearch(&search_ptr,online_players,online_player_count,sizeof(void*),cmpOnlineInfo);

    if( !match )
        return NULL;
    else
        return *match;
}

#if defined(DBQUERY)
void dbUpdateAllOnlineInfo(void)
{
    return;
}
#else
extern int OnArenaMap(void);

void dbUpdateAllOnlineInfo(void)
{
    Packet    *pak;

    int        count;
    int        plyrs = 0;
    Entity    *e;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_REQ_ONLINE_ENTS);

    // Per-map, so the same for all players here.
    pktSendBitsPack(pak, 1, OnArenaMap() ? 1 : 0);
    pktSendBitsPack(pak, 1, OnMissionMap() ? 1 : 0);
    pktSendBitsPack(pak, 1, server_state.allowHeroAndVillainPlayerTypesToTeamUp ?  1 : 0);
    pktSendBitsPack(pak, 1, server_state.allowPrimalAndPraetoriansToTeamUp ?  1 : 0);

    for( count = 0; count < entities_max; count++ )
    {
        // Only active players have valid status for us to update.
        if( ENTTYPE_BY_ID(count) == ENTTYPE_PLAYER && !ENTSLEEPING_BY_ID(count) &&
            !ENTPAUSED_BY_ID(count) && !ENTINFO_BY_ID(count).kill_me_please &&
            entities[count]->db_id > 0 )
        {
            plyrs++;
        }
    }

    pktSendBitsAuto(pak, plyrs);

    for( count = 0; count < entities_max; count++ )
    {
        if( ENTTYPE_BY_ID(count) == ENTTYPE_PLAYER && !ENTSLEEPING_BY_ID(count) &&
            !ENTPAUSED_BY_ID(count) && !ENTINFO_BY_ID(count).kill_me_please &&
            entities[count]->db_id > 0 )
        {
            int factions_same_map = 0;
            int universes_same_map = 0;

            e = entities[count];

            pktSendBitsAuto(pak, e->db_id);

            if( e->teamup )
            {
                // Opposite faction players must travel to the team member's
                // map if the team is homogenous. They also won't be able to
                // join remotely if the team is running a faction-specific map.
                if( !areMembersAlignmentMixed( &e->teamup->members) )
                {
                    if( !areAllMembersOnThisMap( &e->teamup->members) )
                    {
                        factions_same_map = 1;
                    }
                    else if( !isTeamMissionCrossAlliance(e->teamup) )
                    {
                        factions_same_map = 1;
                    }
                }

                if( !areMembersUniverseMixed(&e->teamup->members) &&
                    !areAllMembersOnThisMap(&e->teamup->members) )
                {
                    universes_same_map = 1;
                }
            }

            pktSendBitsPack(pak, 1, factions_same_map);
            pktSendBitsPack(pak, 1, universes_same_map);
        }
    }

    pktSend(&pak,&db_comm_link);
}
#endif

void dbUpdateAllOnlineInfoComments()
{
    Packet *pak;
    static int timer = -1;
    int commentTimeDelay;

    if( db_state.is_static_map )
        commentTimeDelay = 15; // static maps can get this more often
    else
        commentTimeDelay = 120; // mission maps don't get frequent updates 

    if (timer < 0)
        timer = timerAlloc();
    else if (timerElapsed(timer) < commentTimeDelay )
        return;

    timerAdd(timer, -commentTimeDelay );
    pak = pktCreateEx(&db_comm_link,DBCLIENT_REQ_ONLINE_ENT_COMMENTS);
    pktSend(&pak,&db_comm_link);
}

static int online_player_curr = -1;

OnlinePlayerInfo *onlinePlayerInfoIncremental() 
{
    if( !online_players )
        return NULL;

    online_player_curr++;

    if( online_player_curr >= online_player_count )
    {
        online_player_curr = -1;
        return NULL;
    }

    return online_players[online_player_curr];
}
