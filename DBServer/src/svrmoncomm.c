#define SVRMONCOMM_PARSE_INFO_DEFS
#include "svrmoncomm.h"
#include "accountservercomm.h"
#include "auction/Auction.h"
#include "dbserver/auctionservercomm.h"
#include <utilitieslib/utils/error.h>
#include "comm_backend.h"
#include <stdio.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/structNet.h>
#include <utilitieslib/network/netio_core.h>
#include <utilitieslib/components/StashTable.h>
#include "dbdispatch.h"
#include "launchercomm.h"
#include <utilitieslib/version/AppVersion.h>
#include "clientcomm.h"
#include <utilitieslib/utils/timing.h>
#include "container/containerbroadcast.h"
#include <utilitieslib/utils/utils.h>
#include "dbmsg.h"
#include "arenacomm.h"
#include "statservercomm.h"
#include "beaconservercomm.h"
#include "missionservercomm.h"
#include "turnstileDb.h"
#include <utilitieslib/utils/log.h>
#include "overloadProtection.h"
#include "waitingEntities.h"

#define MAX_CLIENTS 50

NetLinkList        svrMonLinks;

struct {
    ContainerType container_type;
    TokenizerParseInfo *tpi;
    int size;
    bool needsSendPlayersBit;
    int cmd;
    int active_only;
    bool needNewClient; // Only send these to a new ServerMonitor (not one running an old protocol)
} svr_mon_data_types[] = {
    {CONTAINER_MAPS,        MapConNetInfo,            sizeof(MapCon),        false,    DBSVRMON_MAPSERVERS,        1,    false},
    {CONTAINER_CRASHEDMAPS,    CrashedMapConNetInfo,    sizeof(MapCon),        false,    DBSVRMON_CRASHEDMAPSERVERS,    0,    false},
    {CONTAINER_LAUNCHERS,    LauncherConNetInfo,        sizeof(LauncherCon),false,    DBSVRMON_LAUNCHERS,            1,    false},
    {CONTAINER_ENTS,        EntConNetInfo,            sizeof(EntCon),        true,    DBSVRMON_PLAYERS,            0,    false},
    {CONTAINER_SERVERAPPS,    ServerAppConNetInfo,    sizeof(ServerAppCon),false,    DBSVRMON_SERVERAPPS,        0,    true},   };

// common db stats for updates to all server monitors that we calculate and store here
// during the updates
typedef struct ServerMonStatsCommon
{
    F32    seconds_since_update;        // seconds since update (floating point)
    U32    milliseconds_since_update;    // number of milliseconds that have elapsed between updates to server monitors
    U32 peak_waiting_entities;        // peak number of entities in the waiting list for map xfer
} ServerMonStatsCommon;

int svrMonConnectCallback(NetLink *link)
{
    SvrMonClientLink    *client;
    int i;

    LOG_OLD("new svrmon link at %s\n",makeIpStr(link->addr.sin_addr.S_un.S_addr));
    client = link->userData;
    client->link = link;
    for (i=0; i<MAX_CONTAINER_TYPES; i++) {
        client->htSentIds[i] = stashTableCreateInt(16);
    }
    client->htTemp = stashTableCreateInt(16);
    return 1;
}

static TokenizerParseInfo *destructor_tpi=NULL;
static void destructor(void *data)
{
    if (data==NULL || data==(void*)1)
        return;
    if (destructor_tpi) {
        sdFreeStruct(destructor_tpi, data);
    } else {
        assert(0);
    }
    free(data);
}

int svrMonDelCallback(NetLink *link)
{
    SvrMonClientLink    *client = link->userData;
    int i;
    LOG_OLD("del svrmon link at %s\n",makeIpStr(link->addr.sin_addr.S_un.S_addr));

    for (i=0; i<ARRAY_SIZE(svr_mon_data_types); i++) 
    {
        destructor_tpi = svr_mon_data_types[i].tpi;
        stashTableDestroyEx(client->htSentIds[svr_mon_data_types[i].container_type], NULL, destructor);
        client->htSentIds[svr_mon_data_types[i].container_type]=NULL;
    }
    destructor_tpi = NULL;
    for (i=0; i<MAX_CONTAINER_TYPES; i++) {
        if (client->htSentIds[i]) {
            stashTableDestroyEx(client->htSentIds[i], NULL, destructor);
            client->htSentIds[i] = NULL;
        }
    }
    stashTableDestroyEx(client->htTemp, NULL, destructor);

    return 1;
}


static int sendDeleteProcessor(void* pakData, StashElement element)
{
    Packet* pak = (Packet*)pakData;
    DbContainer *con = stashElementGetPointer(element);

    pktSendBitsPack(pak, 1, con->id);
    return 1;
}


void svrMonSendList(SvrMonClientLink *client, ContainerType type, int size, int msg, TokenizerParseInfo *tpi, bool full_update, int active_only)
{
    Packet *pak;
    int *ilist;
    int count, i;
    DbContainer    *con;
    DbContainer *oldcon;
    DbList *list = dbListPtr(type);
    StashTable htSentThisFrame = client->htTemp;
    int starting_included=0;
    // Delete detect scheme:  move all elements from one hashtable to a newer one, and iterate over all that are left

    if (type == CONTAINER_MAPS && active_only) {
        active_only = 0;
        starting_included = 1;
    }

    ilist = listGetAll(list,&count,active_only);

// JE: I don't know why this was here, but if it is here, it causes a ServerMonitor
//     crash if there are 0 mapservers and 1+ crashed map running
//    if (!full_update && gHashGetSize(client->htSentIds[type])==0 && count) {
//        full_update = true;
//    }

    pak = pktCreateEx(client->link, msg);
    pktSendBits(pak, 32, timerSecondsSince2000() - timerCpuSeconds());
    pktSendBits(pak, 1, full_update);
    if (full_update) {
        sdPackParseInfo(tpi, pak);
    }

    if (full_update) {
        destructor_tpi = tpi;
        stashTableClearEx(client->htSentIds[type], NULL, destructor);
    }

    assert(stashGetValidElementCount(client->htTemp)==0);

    for(i=0;i<count;i++)
    {
        con = containerPtr(list,ilist[i]);
        if (!con)
        {
            continue;
        }
        if (starting_included) {
            MapCon *map_con = (MapCon*)con;
            if (!map_con->active && !map_con->starting)
                continue;
        }

        assert(ilist[i]!=0);
        pktSendBitsPack(pak,3,ilist[i]);
        if (!stashIntFindPointer(client->htSentIds[type], ilist[i], &oldcon))
            oldcon = NULL;
        sdPackDiff(tpi, pak, oldcon, con, full_update || !oldcon, true, false, 0, 0);
        if (!oldcon) {
            oldcon = calloc(size, 1);
        } else {
            // We already sent this once, remove it from the other HT
            stashIntRemovePointer(client->htSentIds[type], ilist[i], NULL);
        }
        stashIntAddPointer(htSentThisFrame, ilist[i], oldcon, false);
        sdCopyStruct(tpi, con, oldcon);
    }
    pktSendBitsPack(pak,3,0);
    free(ilist);

    // Now, everything left in client->htSentIds[type] no longer exists, send deletes!
    stashForEachElementEx(client->htSentIds[type], sendDeleteProcessor, pak);
    pktSendBitsPack(pak, 1, 0);

    // Remove all from htSentIds, and set it to htTemp
    destructor_tpi = tpi;
    stashTableClearEx(client->htSentIds[type], NULL, destructor);
    client->htTemp = client->htSentIds[type];
    client->htSentIds[type] = htSentThisFrame;

    pktSend(&pak, client->link);
    lnkFlush(client->link);
}

void svrMonSendFullUpdate(SvrMonClientLink *client)
{
    int i;
    for (i=0; i<ARRAY_SIZE(svr_mon_data_types); i++) 
    {
        if (svr_mon_data_types[i].needNewClient && !client->allow_new_container_types)
            continue;
        if (svr_mon_data_types[i].needsSendPlayersBit && !client->sendPlayers)
            continue;

        svrMonSendList(client,
            svr_mon_data_types[i].container_type,
            svr_mon_data_types[i].size,
            svr_mon_data_types[i].cmd,
            svr_mon_data_types[i].tpi,
            true,
            svr_mon_data_types[i].active_only);
    }
    if (client->sendPlayers) {
        client->sentPlayers=1;
        client->sendPlayers=0;
    }
}

void svrMonSendDiff(SvrMonClientLink *client)
{

    int i;
    for (i=0; i<ARRAY_SIZE(svr_mon_data_types); i++) 
    {
        bool full_update = svr_mon_data_types[i].needsSendPlayersBit?(client->sentPlayers?false:true):false;
        if (svr_mon_data_types[i].needNewClient && !client->allow_new_container_types)
            continue;
        if (svr_mon_data_types[i].needsSendPlayersBit && !client->sendPlayers)
            continue;

        svrMonSendList(client,
            svr_mon_data_types[i].container_type,
            svr_mon_data_types[i].size,
            svr_mon_data_types[i].cmd,
            svr_mon_data_types[i].tpi,
            full_update,
            svr_mon_data_types[i].active_only);
    }
    if (client->sendPlayers) {
        client->sentPlayers=1;
        client->sendPlayers=0;
    }
}

void svrMonHandleConnect(SvrMonClientLink *client, Packet *pak)
{
    static int init=0;
    static U32 server_crcs[1];
    U32 crc;
    int i;
    Packet *pak_out;
    if (!init) {
        init = 1;
        server_crcs[0] = SVRMON_PROTOCOL_MAJOR_VERSION;
        // No longer CRC these
//        server_crcs[0] = DBSERVER_PROTOCOL_VERSION;
//        server_crcs[1] = ParserCRCFromParseInfo(EntConNetInfo);
//        server_crcs[2] = ParserCRCFromParseInfo(LauncherConNetInfo);
//        server_crcs[3] = ParserCRCFromParseInfo(CrashedMapConNetInfo);
//        server_crcs[4] = ParserCRCFromParseInfo(MapConNetInfo);
    }

    pak_out = pktCreateEx(client->link, DBSVRMON_CONNECT);

    for (i=0; i<ARRAY_SIZE(server_crcs); i++) {
        crc = pktGetBits(pak, 32);
        if (crc != server_crcs[i]) {
            pktSendBits(pak_out, 1, 0);
            pktSendBitsPack(pak_out, 1, i);
            pktSendBits(pak_out, 32, server_crcs[i]);
            pktSendBits(pak_out, 32, crc);
            pktSend(&pak_out, client->link);
            return;
        }
    }
    client->allow_new_container_types = 0;
    if (!pktEnd(pak)) {
        U32 minor_version = pktGetBits(pak, 32);
        if (minor_version >= 20050927) {
            client->allow_new_container_types = 1;
        }
    }
    client->svrmon_ready = 1;
    client->full_update = 1;
    pktSendBits(pak_out, 1, 1);
    pktSend(&pak_out, client->link);
}


void svrMonHandleRelayMessage(NetLink *link, Packet *pak)
{
    SvrMonClientLink    *client = link->userData;
    char msg_buff[1024];
    char *cmd = pktGetString(pak);

#define CMD(s) (stricmp(s, cmd)==0)
    if (CMD("Shutdown")) 
    {
        LOG_OLD( "%s : Shutdown", makeIpStr(link->addr.sin_addr.S_un.S_addr));
        LOG_OLD("Shutdown command sent from: %s\n",makeIpStr(link->addr.sin_addr.S_un.S_addr));
        printf("Shutdown command sent from: %s\n",makeIpStr(link->addr.sin_addr.S_un.S_addr));
        dbPrepShutdown(pktGetString(pak),0);
    } else if (CMD("AdminChat")) 
    {
        strcpy(msg_buff, DBMSG_CHAT_MSG);
        strcat(msg_buff, " ");
        Strncatt(msg_buff, pktGetString(pak));
        LOG_OLD( "%s : AdminChat %s", makeIpStr(link->addr.sin_addr.S_un.S_addr), msg_buff);
        containerBroadcastMsg(CONTAINER_ENTS, INFO_ADMIN_COM, 0, -1, msg_buff);
    } else if (CMD("OverloadProtection")) 
    {
        char *data = pktGetString(pak);
        LOG_OLD( "%s : OverloadProtection %s", makeIpStr(link->addr.sin_addr.S_un.S_addr), data);
        overloadProtection_MonitorOverride(data);
    } else if (CMD("Delink")) 
    {
        int map_id = atoi(pktGetString(pak));
        LOG_OLD( "%s : Delink %d", makeIpStr(link->addr.sin_addr.S_un.S_addr), map_id);
        if (map_id) {
            dbDelinkMapserver(map_id);
        }
    } else if(CMD("MSLinkReset"))
    {
        LOG_OLD( "%s : MSLinkReset", makeIpStr(link->addr.sin_addr.S_un.S_addr));
        missionserver_forceLinkReset();
    } else if(CMD("ReconcileLauncher"))
    {
        Strncpyt(msg_buff, pktGetString(pak));
        LOG_OLD( "%s : ReconcileLauncher %s", makeIpStr(link->addr.sin_addr.S_un.S_addr), msg_buff);
        launcherReconcile(msg_buff);
    } else if(CMD("SuspendLauncher"))
    {
        Strncpyt(msg_buff, pktGetString(pak));    // IP address
        LOG_OLD( "%s : SuspendLauncher %s", makeIpStr(link->addr.sin_addr.S_un.S_addr), msg_buff);
        launcherCommandSuspend(msg_buff);
    } else if(CMD("ResumeLauncher"))
    {
        Strncpyt(msg_buff, pktGetString(pak));    // IP address
        LOG_OLD( "%s : ResumeLauncher %s", makeIpStr(link->addr.sin_addr.S_un.S_addr), msg_buff);
        launcherCommandResume(msg_buff);
    }
    else 
    {
        LOG_OLD_ERR( "Received unknown command from ServerMonitor: \"%s\" \"%s\"", cmd, pktGetString(pak));
    }
#undef CMD
}

static int log_packet_delay_usec;
static size_t log_bytes_written;
static int log_queue_count;
static int log_queue_max;
static size_t log_sorted_memory;
static size_t log_sorted_memory_limit;

static void storeLogServerStats(NetLink *link, Packet *pak)
{
    int pakid;
    Packet * response;

    pakid = pktGetBitsAuto(pak);
    
    response = pktCreateEx(link, DBSVRMON_LOGSERVERSTATS);
    pktSendBitsAuto(response, pakid);
    pktSend(&response, link);

    log_packet_delay_usec = pktGetBitsAuto(pak);
    log_bytes_written = pktGetBitsAuto(pak);
    log_queue_count = pktGetBitsAuto(pak);
    log_queue_max = pktGetBitsAuto(pak);
    log_sorted_memory = pktGetBitsAuto(pak);
    log_sorted_memory_limit = pktGetBitsAuto(pak);
}

int svrMonHandleClientMsg(Packet *pak,int cmd, NetLink *link)
{
    SvrMonClientLink    *client = link->userData;
    U32        ip;
    char    *s;

    if (!client)
        return 0;

    switch(cmd)
    {
    case DBSVRMON_CONNECT:
        svrMonHandleConnect(client, pak);
        break;
    case DBSVRMON_REQUEST:
        svrMonSendFullUpdate(client);
        break;
    case DBSVRMON_REQUESTDIFF:
        svrMonSendDiff(client);
        break;
    case DBSVRMON_RELAYMESSAGE:
        // Used to send any message a mapserver might send (delink, shutdown)
        svrMonHandleRelayMessage(link, pak);
        break;
    case DBSVRMON_EXEC:
        ip = pktGetBits(pak, 32);
        s = pktGetString(pak);
        launcherCommExec(s, ip);
        break;
    case DBSVRMON_REQUESTVERSION:
        {
            Packet *pak_out = pktCreateEx(client->link, DBSVRMON_REQUESTVERSION);
            pktSendString(pak_out, getCompatibleGameVersion());
            pktSendString(pak_out, getExecutablePatchVersion("CoH Server"));
            pktSend(&pak_out, client->link);
            lnkFlush(client->link);
        }
        break;
    case DBSVRMON_REQUEST_PLAYERS:
        client->sendPlayers = pktGetBits(pak, 1);
        break;
    case DBSVRMON_LOGSERVERSTATS:
        storeLogServerStats(link, pak);
        break;
    default:
        LOG_OLD_ERR("Unknown command from ServerMonitor: %d\n",cmd);
        printf("Unknown command from ServerMonitor: %d\n",cmd);
        return 0;
    }
    return 1;
}

void svrMonInit()
{
    netLinkListAlloc(&svrMonLinks,MAX_CLIENTS,sizeof(SvrMonClientLink),svrMonConnectCallback);
    netInit(&svrMonLinks,0,DEFAULT_SVRMON_PORT);
    svrMonLinks.destroyCallback = svrMonDelCallback;
    NMAddLinkList(&svrMonLinks, svrMonHandleClientMsg);
}

static bool needs_flushing=false;
void svrMonUpdate()
{
    needs_flushing=true;
}

void svrMonSendDbStats(SvrMonClientLink *client, ServerMonStatsCommon const* stats )
{
    Packet *pak;
    // externally tracked stats that are sent
    extern int sql_queue_depth;
    extern int sql_throughput;
    extern int sql_avg_lat_usec;
    extern int sql_worst_lat_usec;
    extern float sql_fore_idle_ratio;
    extern float sql_back_idle_ratio;
    extern F32 db_tick_len;

    // If you change this function, make sure it's backwards compatible by connecting to your DbServer
    //   with an *old* ServerMonitor.exe
    // And make sure you bump the version number below
#define SVRMON_SEND_VERSION 28 // (formerly count of fields being sent)
    pak = pktCreateEx(client->link, DBSVRMON_DBSTATS);
    pktSendBitsPack(pak, 1, SVRMON_SEND_VERSION);

    pktSendBitsPack(pak, 10, clientLoginCount());
    pktSendBitsPack(pak, 10, inUseCount(dbListPtr(CONTAINER_ENTS)));
    pktSendBitsPack(pak, 10, sql_queue_depth);
    pktSendBitsPack(pak, 10, svrMonLinks.links->size);

    pktSendF32(pak, db_tick_len);
    pktSendBitsPack(pak, 10, arenaServerSecondsSinceUpdate());
    pktSendBitsPack(pak, 10, statServerSecondsSinceUpdate());
    pktSendBitsPack(pak, 4, beaconServerLongestWaitSeconds());

    pktSendBitsAuto( pak, auctionServerSecondsSinceUpdate());
    pktSendBitsAuto( pak, 0);    // dummy field to replace villain auction server timestamp
    pktSendBitsAuto( pak, accountServerSecondsSinceUpdate());
    pktSendBitsAuto(pak, missionserver_secondsSinceUpdate());

    pktSendBitsAuto(pak, sql_throughput);
    pktSendBitsAuto(pak, sql_avg_lat_usec);
    pktSendBitsAuto(pak, sql_worst_lat_usec);
    pktSendBitsAuto(pak, log_packet_delay_usec);

    pktSendBitsAuto(pak, (U32)log_bytes_written);
    pktSendBitsAuto(pak, log_queue_count);
    pktSendBitsAuto(pak, log_queue_max);
    pktSendBitsAuto(pak, (U32)log_sorted_memory);

    pktSendBitsAuto(pak, (U32)log_sorted_memory_limit);
    pktSendBitsAuto(pak, clientQueuedCount());
    pktSendBitsAuto(pak, clientConnectedCount());
    pktSendF32(pak, sql_fore_idle_ratio);

    pktSendF32(pak, sql_back_idle_ratio);
    pktSendBitsAuto(pak, turnstileServerSecondsSinceUpdate());

    // Version 27: added overload protection
    pktSendBitsAuto(pak, overloadProtection_GetStatus());

    // Version 28: added total map start requests and delta time since last update was sent in ms
    {
        extern int g_total_map_start_requests;
        pktSendBitsAuto(pak, stats->milliseconds_since_update);        // send elapsed time as milliseconds
        pktSendBitsAuto(pak, g_total_map_start_requests);
        pktSendBitsAuto(pak, stats->peak_waiting_entities);
    }

    // If you change this function, make sure it's backwards compatible by 
    //    updating the version number above

    pktSend(&pak, client->link);
    lnkFlush(client->link);
}

static void check_update_required_from_overload(void)
{
    static int lastOverloadProtectionStatus = 0;
    int newOverloadProtectionStatus = overloadProtection_GetStatus();

    if (newOverloadProtectionStatus != lastOverloadProtectionStatus)
    {
        lastOverloadProtectionStatus = newOverloadProtectionStatus;
        svrMonUpdate();    // request update
    }
}

// do any final checks for requesting an update to all monitors
static void check_update_required(void)
{
    check_update_required_from_overload();
}

void svrMonSendUpdates()
{
    int i;
    SvrMonClientLink    *client;
    ServerMonStatsCommon    stats;
    int current_time;
    static int timer=0;
    F32    elapsed_time = 0.0f;

    // do final checks to see if needs_flushing gets set
    check_update_required();

    // wait until minimum elapsed time has occurred before even considering an update
    if (timer == 0) {
        timer = timerAlloc();
    } else {
        elapsed_time = timerElapsed(timer);
        if (elapsed_time < 1.0f) {
            return;
        }
    }

    // skip update if nobody has requested one, (usually this means launchers are not running),
    // but do an update at least every so often to pick up changes to the common
    // dbserver stats
    if (!needs_flushing && elapsed_time < 5.0f)
        return;

    current_time = timerCpuSeconds();
    
    // refresh update times for mapservers so monitor will know when maps are stuck or information becomes stale
    for (i=0; i<map_list->num_alloced; i++) {
        MapCon *map_con = (MapCon*)map_list->containers[i];
        if (map_con)
        {
            // Update seconds_since_update for all starting mapservers
            if (!map_con->active && map_con->starting) {
                map_con->seconds_since_update = current_time - map_con->lastRecvTime;
            }
            // update freshness of the last remote process info received from host launcher
            // so monitor can detect if this mapserver has been 'orphaned' from launcher stat updates
            map_con->remote_process_info.seconds_since_update = current_time - map_con->remote_process_info.last_update_time;
        }
    }

    needs_flushing = false;

    timerStart(timer);

    // update local stats structure we are going to send to all the server monitors
    stats.seconds_since_update = elapsed_time;
    stats.milliseconds_since_update = (U32)(elapsed_time*1000);
    stats.peak_waiting_entities = waitingEntitiesGetPeakSize(true);    // get peak count and reset

    for(i=svrMonLinks.links->size-1;i>=0;i--)
    {
        client = ((NetLink*)svrMonLinks.links->storage[i])->userData;
        assert(client->link == svrMonLinks.links->storage[i]);

        if (!client->svrmon_ready)
            continue;

        if (qGetSize(client->link->sendQueue2) > 1000)
        {
            LOG_OLD( "Dropping srvmon link, 1000 packets in output buffer");
            netRemoveLink(client->link);
        } 
        else 
        {
            if (client->full_update)
            {
                svrMonSendFullUpdate(client);
                client->full_update = 0;
            } 
            else 
            {
                svrMonSendDiff(client);
            }
            svrMonSendDbStats(client, &stats);
        }
    }
}
