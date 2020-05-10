#include "dbinit.h"
#include <utilitieslib/stdtypes.h>
#include "dbserver/auctionservercomm.h"
#include "accountservercomm.h"
#include "missionservercomm.h"
#include <stdio.h>
#include <direct.h>
#include <utilitieslib/network/net_socket.h>
#include "doors.h"
#include "launchercomm.h"
#include <utilitieslib/utils/utils.h>
#include "comm_backend.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/timing.h>
#include "dbdispatch.h"
#include "dbserver/servercfg.h"
#include "weeklyTFcfg.h"
#include "status.h"
#include "dbperf.h"
#include "container_flatfile.h"
#include <utilitieslib/network/sock.h>
#include "dbmsg.h"
#include <conio.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include "auth/auth.h"
#include "authcomm.h"
#include "clientcomm.h"
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/file.h>
#include "dbmission.h"
#include "svrmoncomm.h"
#include "mapcrashreport.h"
#include "namecache.h"
#include <utilitieslib/utils/memlog.h>
#include "dblog.h"
#include "waitingEntities.h"
#include "container_merge.h"
#include "namecache.h"
#include "statagg.h"
#include <utilitieslib/Version/AppRegCache.h>
#include "dbrelay.h"
#include "logserver.h"
#include "chatrelay.h"
#include "arenacomm.h"
#include "statservercomm.h"
#include "container_sql.h"
#include "sql_fifo.h"
#include "container_tplt.h"
#include <utilitieslib/utils/sysutil.h>
#include "mapxfer.h"
#include <utilitieslib/utils/StringUtil.h>
#include "offline.h"
#include "loadBalancing.h"
#include "serverAutoStart.h"
#include <utilitieslib/utils/FolderCache.h>
#include "beaconservercomm.h"
#include "dbcomm/staticMapInfo.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/StringTable.h>
#include <utilitieslib/utils/stackdump.h>
#include "dbimport.h"
#include "backup.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/winutil.h>
#include "turnstiledb.h"
#include "missionservercomm.h"    //for reset mission server link console
#include "queueservercomm.h"
#include "../Common/ClientLogin/clientcommlogin.h"
#include "dbEventHistory.h"
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/genericdialog.h>
#include <utilitieslib/components/earray.h>
#include "account/AccountCatalog.h"
#include "overloadProtection.h"

#pragma comment(lib,"winmm.lib") // timeBeginPeriod

#define MAX_CLIENTS 2500
NetLinkList                net_links;
static StashTable lock_id_hashes;

DbList    *testdatabasetypes_list, *map_list, *ent_list, *doors_list;
DbList    *teamups_list, *supergroups_list, *taskforces_list, *levelingpacts_list, *league_list;
DbList    *email_list, *petitions_list, *crashedmap_list, *mapgroups_list, *arenaevent_list, *arenaplayer_list, *baseraid_list, *base_list, *stat_sgrp_list, *itemofpowergame_list, *itemofpower_list, *offline_list, *sgraidinfo_list, *mineacc_list;
DbList    *launcher_list, *serverapp_list, *eventhistory_list, *autocommands_list, *shardaccounts_list;
char    my_hostname[256];

#ifdef OFFLINE_ENABLE_DELETION_LOG_IMPORT_CODE
    static char    deletion_logs_to_process[2048] = { 0 };
#endif

static db_lock_id;

AttributeList    *var_attrs;
AttributeList    *badge_attrs;
AttributeList    *pop_help_attrs;

static void detachContainerServer(DBClientLink* client, DbList* dblist);

char *myHostname()
{
    return my_hostname;
}

static void unloadStrandedEnts(int map_id, NetLink *link)
{
    int        i;
    EntCon    *ent_con;
    int        lock_id = dbLockIdFromLink(link);

    if (lock_id == 0) {
        if (!link) 
        {
            LOG_OLD_ERR( "unloadStrandedEnts called with lock_id of 0! map_id %d, (NULL link)", map_id);
        } 
        else 
        {
            LOG_OLD_ERR( "unloadStrandedEnts called with lock_id of 0! map_id %d, (%s)", map_id, makeIpStr(link->addr.sin_addr.S_un.S_addr));
        }
        return;
    }

    for(i=ent_list->num_alloced-1;i>=0;i--)
    {
        ent_con = (EntCon *)ent_list->containers[i];
        if (ent_con->lock_id == lock_id || ent_con->callback_link == link)
        {
            LOG_DEBUG( "handlePlayerDisconnect %d",ent_con->id);
            sendPlayerLogout(ent_con->id,5,false);
        }
    }
}

void mapOrTeamSpecialFree(DbContainer *container)
{
    /*
    int        list_id = container->type;

    if (list_id == CONTAINER_TEAMUPS)
    {
        GroupCon *teamcon = (GroupCon*)container;
        if (teamcon->map_id)
        {
            MapCon *mapcon = containerPtr(dbListPtr(CONTAINER_MAPS),teamcon->map_id);

            if (mapcon)
                sendMapserverShutdown(mapcon->link,0,"");
        }

    }

    if (list_id == CONTAINER_MAPS)
    {
        MapCon *mapcon = (MapCon*)container;
        char    buf[1000];

        if (mapcon->callback_idx)
        {
            sprintf(buf,"TeamupMissionStoppedRunning: %d",mapcon->id);
            containerBroadcastMsg(CONTAINER_TEAMUPS,0,0,mapcon->callback_idx,buf);
        }
    }
    */
}

void freeMapContainer(MapCon *container)
{
    if (!container)
        return;
    if(container->is_static)
        LOG_OLD("removing map container %d", container->id);
    mapOrTeamSpecialFree((DbContainer*)container);
    container->active = 0;
    container->num_ents = container->num_monsters = container->num_players = container->num_players_connecting = 0;
    if (!container->starting)
        unloadStrandedEnts(container->id, container->link);
    container->starting = 0;
    containerUnlockById(dbLockIdFromLink(container->link));
    container->link = 0;
    container->lastRecvTime = 0;
    container->map_running = 0;
    memset(&container->remote_process_info, 0, sizeof(container->remote_process_info));
    container->ticks_sec = 0;
    container->long_tick = 0;

    if (!container->is_static)
        containerDelete(container);
}

U32 dbLockIdFromLink(NetLink *link)
{
    DBClientLink    *client;

    if (!link)
        return 0;
    client = link->userData;
    return client->lock_id;
}

NetLink *dbLinkFromLockId(int lock_id)
{
    NetLink            *link;

    if ( lock_id && stashIntFindPointer(lock_id_hashes, lock_id, &link))
        return link;
    return NULL;
}

int dbClientCallback(NetLink *link)
{
    DBClientLink    *client;

    client = link->userData;
    memset(client, 0, sizeof(*client));
    client->link = link;
    client->ready = 0;
    client->map_id = -1;
    client->lock_id = ++db_lock_id;
    client->has_auctionhouse = false;
    link->cookieEcho = 1;
    stashIntAddPointer(lock_id_hashes, client->lock_id, link, false);
    return 1;
}

int netLinkDelCallback(NetLink *link)
{
    DBClientLink    *client = link->userData;
    MapCon            *container;
    int                i;

    stashIntRemovePointer(lock_id_hashes, client->lock_id, NULL);
    // cleanup if the mapserver crashed

    if (client->container_server)
    {
        client->wait_list_id = 0;
        for (i = 1; i < MAX_CONTAINER_TYPES; i++)
        {
            DbList* dblist = dbListPtr(i);
            if (dblist && dblist->owner_lockid == client->lock_id)
            {
                detachContainerServer(client, dblist);
            }
        }
    }

    if (client->map_id < 0)
        return 1;
    container = containerPtr(map_list,client->map_id);
    if (container && container->link == link)
    {
        container->launcher_id = 0;
        freeMapContainer(container);
    }

    // ab: could loop over all ents in the process of logging on here, but won't.

    return 1;
}

int mapserverCount(void)
{
    return net_links.links->size;
}

NetLink *mapserverLink(int index)
{
    return index < 0 || index >= net_links.links->size ? NULL : (NetLink *) net_links.links->storage[index];
}

int raidServerCount(void)
{
    DbList* dblist = dbListPtr(CONTAINER_BASERAIDS);
    return (dblist && dblist->owner_lockid)? 1 : 0;
}

int sql_queue_depth=0;
int sql_throughput=0;
long sql_avg_lat_usec=0;
long sql_worst_lat_usec=0;
float sql_fore_idle_ratio=0.0f;
float sql_back_idle_ratio=0.0f;
F32 db_tick_len=0;
void updateDbServerTitle()
{
    char        beaconServerString[100] = "";
    char        buf[1000];
    size_t        packetCount, bsCount;
    static    int    timer,slowest_cmd,latency_msec,update_timer;
    int            init=0;

    if (!timer)
    {
        timer = timerAlloc();
        update_timer = timerAlloc();
        init = 1;
    }
    if (timerElapsed(timer) > 10.f)
    {
        latency_msec = dbGetCurrMaxLatency(&slowest_cmd) * 1000;
        sql_queue_depth = sqlFifoWorstQueueDepth();
        sqlConnGetTimes(&sql_throughput, &sql_avg_lat_usec, &sql_worst_lat_usec, &sql_fore_idle_ratio, &sql_back_idle_ratio);
        db_tick_len = dbTickLength();
        overloadProtection_SetSQLStatus(sql_queue_depth);
        timerStart(timer);
    }
    packetGetAllocationCounts(&packetCount, &bsCount);

    if (timerElapsed(update_timer) < 0.5 && !init)
        return;
    timerStart(update_timer);
    if(beaconServerCount()){
        strcatf(beaconServerString, "Bs%d ", beaconServerCount());
    }
    if(beaconClientCount()){
        strcatf(beaconServerString, "Bc%d ", beaconClientCount());
    }
    
    sprintf(buf,
        "DbServer Launchrs %d  Maps %d  Svrs:%s%s%s%s%s LoggingIn %d  Playing %d  Packets %Id  Latency %d(%d)  SQL(Q:%d R:%d A:%d W:%d) Tick %1.2fs %s",
            launcherCount(),
            mapserverCount(),
            arenaServerCount()? "Ar ": "",
            statServerCount()? "St ": "",
            raidServerCount()? "Rd ": "",
            beaconServerString,
            !arenaServerCount() && !statServerCount() && !raidServerCount() && !beaconServerCount() ? "none ":"",
            clientLoginCount(),
            inUseCount(dbListPtr(CONTAINER_ENTS)),
            packetCount,
            latency_msec,slowest_cmd,
            sql_queue_depth,
            sql_throughput,
            sql_avg_lat_usec,
            sql_worst_lat_usec,
            db_tick_len,
            pktGetDebugInfo()?"(PACKET DEBUG ON)":"");
    setConsoleTitle(buf);
}

void reconnectToAuth()
{
    printf_stderr("AuthServer disconnected from the DbServer, trying to reconnect...\n");
    for(;;)
    {
        if (authCommInit(server_cfg.auth_server,server_cfg.auth_server_port))
            break;
    }
}

static void sendSqlKeepAlive(void)
{
    static char keepalive[] = ";";
    static U32 last = 0;

    unsigned i;
    U32 now = timerSecondsSince2000();

    if ((now-last) < 60)
        return;

    last = now;
    sqlConnExecDirect(keepalive, sizeof(keepalive)-1, SQLCONN_FOREGROUND, false);

    for (i=0; i<SQLCONN_MAX-1; i++)
        sqlExecAsyncEx(keepalive, sizeof(keepalive)-1, i, false);
}

void msgScan()
{
    sendSqlKeepAlive();
    sqlFifoTick();
    delinkCrashedMaps();
    delinkDeadLaunchers();
    sqlFifoDisableNMMonitorIfMemoryLow();
    NMMonitor(1);

    svrMonSendUpdates();
    checkServerAutoStart();
    if(authDisconnected())
        reconnectToAuth();
    
    // @todo -AB: work this out :07/12/07
    waitingEntitiesCheck();
    dbRelayQueueCheck();

    if(!server_cfg.queue_server)
        clientClearDeadLinks();
#if defined QUEUESERVER_VERIFICATION
    else
        queueservercomm_updateCount();
#endif
    launcherLaunchBeaconizers();
    stat_Update();
    if (!server_cfg.use_logserver) {
        shardChatMonitor();
        updateLogStats();
    }
    auctionMonitorTick();
    launcherOverloadProtectionTick();

    tempLockTick(server_cfg.name_lock_timeout, timerSecondsSince2000());  //timeout temporary name locks

    updateDbServerTitle();
    checkExitRequest();
    FolderCacheDoCallbacks();
    mpCompactPools();
}

char *getMapName(char *data,char *buf)
{
    char    *str = _strdup(data),*args[10];
    int        count;

    count = tokenize_line(str,args,0);
    if (count < 2)
        return 0;
    strcpy(buf,args[1]);
    forwardSlashes(buf);
    free(str);
    return buf;
}

int getMapId(char *data)
{
    char    name[128];
    MapCon    *c;

    if (!getMapName(data,name))
        return -1;
    c = containerFindByMapName(dbListPtr(CONTAINER_MAPS),name);
    if (!c || !c->is_static)
        return -1;
    return c->id;
}

static int    *static_map_ids,static_map_count,static_map_max;

void getStaticMapList()
{
    int            i,*idp;
    MapCon        *mapcon;
    DbList        *maps = dbListPtr(CONTAINER_MAPS);

    if (static_map_count)
        return;

    // leave an empty slot at the beginning
    dynArrayAdd(&static_map_ids,sizeof(static_map_ids[0]),&static_map_count,&static_map_max,1);
    for(i=0;i<maps->num_alloced;i++)
    {
        mapcon = (MapCon*) maps->containers[i];
        if (mapcon->is_static)
        {
            idp = dynArrayAdd(&static_map_ids,sizeof(static_map_ids[0]),&static_map_count,&static_map_max,1);
            *idp = mapcon->id;
        }
    }
}

int registerDbClient(Packet *pak,NetLink *link)
{
    int            map_id,ip_list[2],udp_port,tcp_port,static_link,cookie;
    char        *map_name_data,patch_version[100],map_name[256],ipbuf1[1000],ipbuf2[1000];
    MapCon        *container=0;

    getStaticMapList();
    map_id        = pktGetBitsPack(pak,1);
    ip_list[0]    = pktGetBitsPack(pak,1);
    ip_list[1]    = pktGetBitsPack(pak,1);
    if (dbIsLocalIp(ip_list[1]) && !dbIsLocalIp(ip_list[0])) {
        U32 temp = ip_list[1];
        ip_list[1] = ip_list[0];
        ip_list[0] = temp;
    }
    udp_port    = pktGetBitsPack(pak,1);
    tcp_port    = pktGetBitsPack(pak,1);
    static_link    = pktGetBitsPack(pak,1);
    cookie        = pktGetBitsPack(pak,1);
    strcpy(patch_version, pktGetString(pak));

    if (map_id < 0) // mainly used by mapservers running in 'localmapserver' (editing) mode
    {
        map_name_data = pktGetString(pak);
        getMapName(map_name_data,map_name);
        if (static_link) // used for manually connecting static maps
        {
            DBClientLink    *client = link->userData;
            container = containerFindByMapName(map_list,map_name);
            client->static_link = 1;
        }
        if (!container)
        {
            container = containerAlloc(map_list,-1);
            containerUpdate(map_list,container->id,map_name_data,0);
        }
        container->edit_mode = 1;
    }
    else
    {
        container = containerPtr(map_list,map_id);
    }
    if (!container)
        return 0;

    // Verify that this connection is from the actual mapserver we wanted to spawn
    if (cookie && container->cookie!=cookie)
    {
        // The map connecting has a cookie (i.e. launched via a launcher, not someone running a
        //  mapserver from the command line), and the cookies don't match.  That means that we've
        //  sent out two requests for this specific map id to be launched and we just received a
        //  response from the first one.  Either that or this mapserver was launched by the
        //  previous DbServer.  Discard the connection.
        printf_stderr("%s Received connection from a MapServer with an invalid cookie (%d, expected %d) from %s\n", timerGetTimeString(), cookie, container->cookie, makeIpStr(link->addr.sin_addr.S_un.S_addr));
        return 0;
    }
    container->cookie *= -1; // "clear" it out, but keep it around in case we need to debug?

    if (container->link && container->link != link) {
        // We already have a link
        netRemoveLink(container->link);
    }

    if (dbIsShuttingDown()) {
        // The DbServer is shutting down, don't allow this new mapserver to connect!
        container->active = 0;
        return 0;
    }

    if (container->is_static) {
        //netLinkSetBufferSize(link, BothBuffer, 8*1024*1024);
        netLinkSetMaxBufferSize(link, BothBuffers, 8*1024*1024); // Set max size to auto-grow to
        netLinkSetBufferSize(link, BothBuffers, 1*1024*1024);
    } else {
        netLinkSetMaxBufferSize(link, BothBuffers, 1*1024*1024); // Set max size to auto-grow to
        netLinkSetBufferSize(link, BothBuffers, 64*1024);
    }

    container->ip_list[0]    = ip_list[0];
    container->ip_list[1]    = ip_list[1];
    container->udp_port        = udp_port;
    container->tcp_port        = tcp_port;
    container->link            = link;
    container->active        = 1;
    container->starting        = 1;
    strcpy(container->patch_version, patch_version);
    static_map_ids[0] = container->id;
    containerSendList(map_list,static_map_ids,static_map_count,0,link,0);
    strcpy(ipbuf1,makeIpStr(container->ip_list[0]));
    strcpy(ipbuf2,makeIpStr(container->ip_list[1]));
    container->on_since = timerSecondsSince2000();
    container->lastRecvTime = timerCpuSeconds();
    return container->id;
}

static void attachContainerServer(DBClientLink* client, DbList* dblist)
{
    Packet            *pak_out;

    // attach
    client->container_server = 1;
    client->ready = 1;
    if (client->wait_list_id == dblist->id)
        client->wait_list_id = 0;
    dblist->owner_lockid = client->lock_id;

    // notify client
    pak_out = pktCreateEx(client->link, DBSERVER_CONTAINER_SERVER_ACK);
    pktSendBitsPack(pak_out, 1, dblist->id);
    pktSendBitsPack(pak_out, 1, dblist->max_session_id);
    pktSend(&pak_out,client->link);

    printf_stderr("Attaching server for %s\n", dblist->name);
}

static void detachContainerServer(DBClientLink* client, DbList* dblist)
{
    int i;

    if (dblist->owner_lockid != client->lock_id)
    {
        printf_stderr("Internal error in detachContainerServer\n");
        return;
    }
    dblist->owner_lockid = 0;
    printf_stderr("Detaching server for %s\n", dblist->name);

    // look for another container server waiting for this list
    for (i=0; i<net_links.links->size; i++) 
    {
        NetLink            *link;
        DBClientLink    *waitclient;

        link = net_links.links->storage[i];
        waitclient = link->userData;
        if (waitclient->wait_list_id == dblist->id)
        {
            attachContainerServer(waitclient, dblist);
            break;
        }
    }
}

// multiple servers can attempt to gain ownership of a dblist.  the
// second will not get an ack until the first has been delinked
void registerContainerServer(Packet *pak,NetLink *link)
{
    int                list_id;
    U32                lock_id = dbLockIdFromLink(link);
    DBClientLink    *client;
    DbList            *dblist;

    LOG_OLD("container server link from %s\n",makeIpStr(link->addr.sin_addr.S_un.S_addr));
    client = link->userData;
    list_id = pktGetBitsPack(pak, 1);
    dblist = dbListPtr(list_id);
    if (!dblist)
    {
        printf_stderr("Received container registration for invalid container %i\n", list_id);
        netRemoveLink(link);
        return;
    }

    // if another container server is running, note that this link is interested
    if (dblist && dblist->owner_lockid && dblist->owner_lockid != lock_id)
    {
        client->container_server = 1;
        client->ready = 0;
        client->wait_list_id = list_id;
        return;
    }
    attachContainerServer(client, dblist);
}

// container server is registering groups that it wants notification of 
void registerContainerServerNotify(Packet* pak, NetLink* link)
{
    U32            lock_id = dbLockIdFromLink(link);
    U32            notify_mask;
    int            i;

    notify_mask = pktGetBitsPack(pak, 1);

    // squirrel this away in dbLists..
    for (i = 1; i < MAX_CONTAINER_TYPES; i++)
    {
        DbList* dblist = dbListPtr(i);
        if (dblist->owner_lockid == lock_id)
        {
            dblist->owner_notify_mask = notify_mask;
            return;
        }
    }
    printf_stderr("Received container server notify registration from a unregistered container server\n");
}

// tell interested container servers about entity log in/out and group join/drops
void containerServerNotify(int list_id, U32 cid, U32 dbid, int add)
{
    int            i;

    for (i = 1; i < MAX_CONTAINER_TYPES; i++)
    {
        DbList* dblist = dbListPtr(i);
        if (TSTB(&dblist->owner_notify_mask, list_id))
        {
            NetLink* link = dbLinkFromLockId(dblist->owner_lockid);
            if (link)
            {
                Packet* pak = pktCreateEx(link, DBSERVER_CONTAINER_SERVER_NOTIFY);
                pktSendBitsPack(pak, 1, list_id);
                pktSendBitsPack(pak, 1, cid);
                pktSendBitsPack(pak, 1, dbid);
                pktSendBits(pak, 1, add? 1: 0);
                pktSend(&pak, link);
            }
        }
    }

    // if player is logging in, also notify servers of any groups he belongs to
    if (list_id == CONTAINER_ENTS && add)
    {
        EntCon* entcon = containerPtr(dbListPtr(list_id), cid);
        if (entcon)
        {
            if (entcon->teamup_id)
                containerServerNotify(CONTAINER_TEAMUPS, entcon->teamup_id, cid, add);
            if (entcon->supergroup_id)
                containerServerNotify(CONTAINER_SUPERGROUPS, entcon->supergroup_id, cid, add);
            if (entcon->taskforce_id)
                containerServerNotify(CONTAINER_TASKFORCES, entcon->taskforce_id, cid, add);
            if (entcon->raid_id)
                containerServerNotify(CONTAINER_RAIDS, entcon->raid_id, cid, add);
            if (entcon->levelingpact_id)
                containerServerNotify(CONTAINER_LEVELINGPACTS, entcon->levelingpact_id, cid, add);
            if (entcon->league_id)
                containerServerNotify(CONTAINER_LEAGUES, entcon->league_id, cid, add);
        }
    }

    // notify the statserver where necessary
    if(CONTAINER_IS_STATSERVER_GROUP(list_id))
    {
        NetLink *link = statLink();
        if(link)
        {
            Packet* pak = pktCreateEx(link, DBSERVER_CONTAINER_SERVER_NOTIFY);
            pktSendBitsPack(pak, 1, list_id);
            pktSendBitsPack(pak, 1, cid);
            pktSendBitsPack(pak, 1, dbid);
            pktSendBits(pak, 1, add? 1: 0);
            pktSend(&pak, link);
        }
    }
}

void dbDelink()
{
    extern NetLinkList launcher_links;
    extern NetLinkList svrMonLinks;
    extern NetLinkList gameclient_links;
    extern int crashmap_sock;
    int i;
#define closesocket(sock) {\
        printf_stderr("Closing %s...\n", ##sock); \
        closesocket(sock); \
        printf_stderr("done\r\n");\
    }

#define CLOSELINK(netlink) \
    if (netlink) { \
        closesocket(((NetLink*)netlink)->socket); \
    } else { \
        OutputDebugStringf("Error: %s is null (i=%d)", ##netlink, i); \
    }

    closesocket(net_links.listen_sock);
    for (i=0; i<net_links.links->size; i++) {
        CLOSELINK(net_links.links->storage[i]);
    }
    closesocket(launcher_links.listen_sock);
    for (i=0; i<launcher_links.links->size; i++) {
        CLOSELINK(launcher_links.links->storage[i]);
    }
    closesocket(svrMonLinks.listen_sock);
    for (i=0; i<svrMonLinks.links->size; i++) {
        CLOSELINK(svrMonLinks.links->storage[i]);
    }
    closesocket(gameclient_links.udp_sock);
    for (i=0; i<gameclient_links.links->size; i++) {
        CLOSELINK(gameclient_links.links->storage[i]);
    }
    closesocket(crashmap_sock);
}

static HWND hwnd;
static bool requestClose=false;
static BOOL CtrlHandler(DWORD fdwCtrlType) 
{ 
    switch (fdwCtrlType) 
    { 
    case CTRL_CLOSE_EVENT: 
    case CTRL_LOGOFF_EVENT: 
    case CTRL_SHUTDOWN_EVENT: 
    case CTRL_BREAK_EVENT: 
    case CTRL_C_EVENT: 
        {
            int num_playing = ent_list?ent_list->num_alloced:0;
            if (num_playing > 1) {
                // There are people playing, ask the main thread to exit
                requestClose = true;
                // Do not allow exit
                return TRUE;
            }

            //Ensures that zeromq socket in logserver.c is properly closed if dbserver
            //(whether it is run as logserver, or working as both dbserver and logserver)
            //is closed with control event.  If dbserver (not run as logserver) is closed
            //with control event, this function call safely does nothing
            logServerShutdown();
            
            dbPrepShutdown(NULL, 0);
            // Allow exit
            return FALSE;
        }
        // Pass other signals to the next handler. 
    default:
        printf_stderr("return false\n");
        return FALSE; 
    } 
} 

void checkExitRequest(void) {
    if (requestClose) {
        // Warn them on exit!
        if (IDYES==MessageBoxA(hwnd, "Are you sure you want to forcefully close the DbServer?  Please shut down the servers through the ServerMonitor for a graceful shutdown.", "DBServer Close Confirmation", MB_YESNO | MB_ICONERROR)) {
            // Exit immediately!
            dbPrepShutdown(NULL, 0);
            exit(0);
        }
        requestClose = false;
    }
}

static void initCtrlHandler(void) {
    static int inited=false;
    if (!inited) {
        BOOL fSuccess;        
        hwnd = GetConsoleWindow();

        fSuccess = SetConsoleCtrlHandler( 
            (PHANDLER_ROUTINE) CtrlHandler,  // handler function 
            TRUE);      // add to list 
        inited=true;
    }
}

void dbNetInit()
{
    netLinkListAlloc(&net_links,MAX_CLIENTS,sizeof(DBClientLink),dbClientCallback);
    if (!netInit(&net_links,0,DEFAULT_DB_PORT)) {
        printf_stderr("Error binding to port %d!\n", DEFAULT_DB_PORT);
    }
    net_links.destroyCallback = netLinkDelCallback;
    NMAddLinkList(&net_links, dbHandleClientMsg);
}

static void sendAuthUserData(int container_id,void *data_void)
{
    char    *data_str = (char*)data_void;
    U8        data[AUTH_BYTES],*bindata;
    int        binlen;
    EntCon    *c = containerPtr(ent_list,container_id);

    if (!c)
    {
        LOG_OLD_ERR("sendAuthUserData cant find live container for %d",container_id);
        return;
    }
    if (!c->auth_user_data_valid)
        return; // can't set it before we get the current auth version
    bindata = hexStrToBinStr(data_str, (int)strlen(data_str));
    memset(data,0,sizeof(data));
    binlen = (int)((strlen(data_str) + 1) / 2);
    if (binlen > AUTH_BYTES)
    {
        binlen = AUTH_BYTES;
    }
    memcpy(data,bindata,binlen);
    authSendUserData(c->auth_id,data);
    LOG_OLD("authbits %s 0x%s sent game_data entity %d %s", c->account, data_str, c->id, c->ent_name);
}

static void repairAuthId()
{
    int broken_auth = sqlGetSingleValue("SELECT COUNT(*) FROM dbo.Ents WHERE AuthId IS NULL;", SQL_NTS, NULL, SQLCONN_FOREGROUND);
    if (!broken_auth)
        return;

    Errorf("Found %d characters with broken AuthId values", broken_auth);

    if (isProductionMode() && !server_cfg.fake_auth)
        return;

    // Ask the user if they want to repair their character's auth data.
    if (IDOK != okCancelDialog("Some characters were found in the database that have incorrect authentication information.\r\n\r\n"
        "These characters will not be accessible until you repair them.\r\n\r\n"
        "Do you wish to continue?",
        "Database repair needed"))
        return;

    {
        char *cols;
        int row_count;
        ColumnInfo *field_ptrs[3];
        int i;
        int idx = 0;

        cols = sqlReadColumnsSlow(ent_list->tplt->tables, NULL, "ContainerId, AuthName", "WHERE AuthId IS NULL", &row_count, field_ptrs);

        for (i=0; i<row_count; i++)
        {            
            int container_id;
            char *auth_name;
            int auth_id;
            char cmd[SHORT_SQL_STRING];
            size_t cmd_len;

            container_id = *(int *)(&cols[idx]);
            idx += field_ptrs[0]->num_bytes;

            auth_name = &cols[idx];
            idx += field_ptrs[1]->num_bytes;

            auth_id = genFakeAuthId(auth_name);

            cmd_len = sprintf(cmd, "UPDATE dbo.Ents SET AuthId=%d WHERE ContainerId=%d;", auth_id, container_id);
            sqlExecAsyncEx(cmd, (int)cmd_len, container_id, false);
        }
    }

    // Force all the updates to complete before continuing on
    sqlFifoBarrier();
}

static void accountCacheSetSize(int container_id, void *data_void)
{
    int new_size = *(int*)data_void;
    ShardAccountCon    *c = containerPtr(shardaccounts_list, container_id);

    if (!c)
    {
        LOG_OLD_ERR("accountCacheSetSize cant find live container for %d", container_id);
        return;
    }

    // early out to avoid allocation if we do not need to
    if (new_size == 0 && !c->inventory)
        return;

    // limit to a reasonable allocation size
    if (!devassert(new_size >= 0 && new_size < 4096))
        return;

    if (new_size < eaSize(&c->inventory))
    {
        int i;
        for (i = eaSize(&c->inventory) - 1; i >= new_size; i--) {
            free(c->inventory[i]);
            c->inventory[i] = NULL;
        }
    }

    eaSetSize(&c->inventory, new_size);
}

static void entContainerUpdtCb(AnyContainer* entContainer_p)
{
    if ( entContainer_p != NULL )
    {
        EntCon *ent_container = (EntCon*)entContainer_p;
        entCatalogPopulate(ent_container->id, ent_container);
    }
}

static void* alloc_order()
{
    return calloc(1, sizeof(OrderId));
}

SpecialColumn ent_cmds[] =
{
    { "Name",                        CFTYPE_ANSISTRING,        OFFSETOF(EntCon,ent_name),                                                },
    { "AuthName",                    CFTYPE_ANSISTRING,        OFFSETOF(EntCon,account),                                                },
    { "TeamupsId",                    CFTYPE_INT,                OFFSETOF(EntCon,teamup_id),            CMD_MEMBER,    CONTAINER_TEAMUPS        },
    { "SupergroupsId",                CFTYPE_INT,                OFFSETOF(EntCon,supergroup_id),        CMD_MEMBER,    CONTAINER_SUPERGROUPS    },
    { "TaskforcesId",                CFTYPE_INT,                OFFSETOF(EntCon,taskforce_id),        CMD_MEMBER,    CONTAINER_TASKFORCES    },
    { "Ents2[0].RaidsId",            CFTYPE_INT,                OFFSETOF(EntCon,raid_id),            CMD_MEMBER,    CONTAINER_RAIDS            }, // TODO() delete me
    { "Ents2[0].LevelingPactsId",    CFTYPE_INT,                OFFSETOF(EntCon,levelingpact_id),    CMD_MEMBER,    CONTAINER_LEVELINGPACTS    },
    { "Ents2[0].LeaguesId",            CFTYPE_INT,                OFFSETOF(EntCon,league_id),            CMD_MEMBER, CONTAINER_LEAGUES        },
    { "StaticMapId",                CFTYPE_INT,                OFFSETOF(EntCon,static_map_id),        CMD_SETIFNULL,                        },
    { "MapId",                        CFTYPE_INT,                OFFSETOF(EntCon,map_id),            CMD_SETIFNULL,                        },
    { "AuthId",                        CFTYPE_INT,                OFFSETOF(EntCon,auth_id),            CMD_SETIFNULL,                        },
    { "Banned",                        CFTYPE_INT,                OFFSETOF(EntCon,banned),                                                },
    { "Ents2[0].AccSvrLock",        CFTYPE_ANSISTRING,        OFFSETOF(EntCon,acc_svr_lock),                                            },
    { "Ents2[0].HideField",            CFTYPE_INT,                OFFSETOF(EntCon,hidefield),                                                },
    { "Level",                        CFTYPE_INT,                OFFSETOF(EntCon,level),                                                    },
    { "Class",                        CFTYPE_ANSISTRING,        OFFSETOF(EntCon,archetype),                                                },
    { "Origin",                        CFTYPE_ANSISTRING,        OFFSETOF(EntCon,origin),                                                },
    { "AccessLevel",                CFTYPE_INT,                OFFSETOF(EntCon,access_level),                                            },
    { "Ents2[0].AuthUserDataEx",    CFTYPE_ANSISTRING,        OFFSETOF(EntCon,auth_user_data),    CMD_CALLBACK, 0,    sendAuthUserData},
    { "Ents2[0].LfgFlags",            CFTYPE_INT,                OFFSETOF(EntCon,lfgfield),                                                },
    { "Ents2[0].Comment",            CFTYPE_ANSISTRING,        OFFSETOF(EntCon,comment),                                                },
    { "LastActive",                    CFTYPE_DATETIME,        OFFSETOF(EntCon,last_active),                                            },
    { "MemberSince",                CFTYPE_DATETIME,        OFFSETOF(EntCon,member_since),                                            },
    { "Prestige",                    CFTYPE_INT,                OFFSETOF(EntCon,prestige),                                                },
    { "PlayerType",                    CFTYPE_BYTE,            OFFSETOF(EntCon,playerType),                                            },
    { "Ents2[0].PlayerSubType",        CFTYPE_BYTE,            OFFSETOF(EntCon,playerSubType),                                            },
    { "Ents2[0].PraetorianProgress",CFTYPE_BYTE,            OFFSETOF(EntCon,praetorianProgress),                                    },
    { "Ents2[0].InfluenceType",        CFTYPE_BYTE,            OFFSETOF(EntCon,playerTypeByLocation),                                    },
//    { "Ents2[0].RequiresGoingRogueOrTrial",    CFTYPE_BYTE,    OFFSETOF(EntCon,requiresGoingRogueOrTrial),                                },
    { "CompletedOrders[%d].OrderId0",    CFTYPE_INT,            OFFSETOF(OrderId,u32[0]),    CMD_ARRAY,    OFFSETOF(EntCon,completedOrders), NULL, alloc_order    },
    { "CompletedOrders[%d].OrderId1",    CFTYPE_INT,            OFFSETOF(OrderId,u32[1]),    CMD_ARRAY,    OFFSETOF(EntCon,completedOrders), NULL, alloc_order    },
    { "CompletedOrders[%d].OrderId2",    CFTYPE_INT,            OFFSETOF(OrderId,u32[2]),    CMD_ARRAY,    OFFSETOF(EntCon,completedOrders), NULL, alloc_order    },
    { "CompletedOrders[%d].OrderId3",    CFTYPE_INT,            OFFSETOF(OrderId,u32[3]),    CMD_ARRAY,    OFFSETOF(EntCon,completedOrders), NULL, alloc_order    },
    { "Gender",                        CFTYPE_BYTE,            OFFSETOF(EntCon,gender),                                                },
    { "NameGender",                    CFTYPE_BYTE,            OFFSETOF(EntCon,nameGender),                                            },
    { "Ents2[0].originalPrimary",    CFTYPE_ANSISTRING,        OFFSETOF(EntCon,primarySet),                                            },
    { "Ents2[0].originalSecondary",    CFTYPE_ANSISTRING,        OFFSETOF(EntCon,secondarySet),                                            },
    { 0 }
};

static void* alloc_inventory()
{
    return calloc(1, sizeof(ShardAccountInventoryItem));
}

SpecialColumn shardaccount_cmds[] =
{
    { "SlotCount",                CFTYPE_INT,            OFFSETOF(ShardAccountCon,slot_count),                    },
    { "AuthName",                CFTYPE_ANSISTRING,    OFFSETOF(ShardAccountCon,account),                        },
    { "WasVIP",                    CFTYPE_INT,            OFFSETOF(ShardAccountCon,was_vip),                        },
    { "ShowPremiumSlotLockNag",    CFTYPE_INT,            OFFSETOF(ShardAccountCon,showPremiumSlotLockNag),        },

    { "LoyaltyStatus0",            CFTYPE_INT,            OFFSETOF(ShardAccountCon,loyaltyStatusOld[0]),                    },
    { "LoyaltyStatus1",            CFTYPE_INT,            OFFSETOF(ShardAccountCon,loyaltyStatusOld[1]),                    },
    { "LoyaltyStatus2",            CFTYPE_INT,            OFFSETOF(ShardAccountCon,loyaltyStatusOld[2]),                    },
    { "LoyaltyStatus3",            CFTYPE_INT,            OFFSETOF(ShardAccountCon,loyaltyStatusOld[3]),                    },
    { "LoyaltyPointsUnspent",    CFTYPE_INT,            OFFSETOF(ShardAccountCon,loyaltyPointsUnspent),                },
    { "LoyaltyPointsEarned",    CFTYPE_INT,            OFFSETOF(ShardAccountCon,loyaltyPointsEarned),                },
    { "AccountStatusFlags",        CFTYPE_INT,            OFFSETOF(ShardAccountCon,accountStatusFlags),                },
    { "AccountInventorySize",    CFTYPE_INT,            OFFSETOF(ShardAccountCon,inventorySize),                    CMD_CALLBACK, 0,    accountCacheSetSize},
    { "AccountInventory[%d].SkuId0",    CFTYPE_INT,    OFFSETOF(ShardAccountInventoryItem, sku_id.u32[0]),    CMD_ARRAY,    OFFSETOF(ShardAccountCon,inventory), NULL, alloc_inventory    },
    { "AccountInventory[%d].SkuId1",    CFTYPE_INT,    OFFSETOF(ShardAccountInventoryItem, sku_id.u32[1]),    CMD_ARRAY,    OFFSETOF(ShardAccountCon,inventory), NULL, alloc_inventory    },
    { "AccountInventory[%d].Granted",    CFTYPE_INT,    OFFSETOF(ShardAccountInventoryItem, granted_total),    CMD_ARRAY,    OFFSETOF(ShardAccountCon,inventory), NULL, alloc_inventory    },
    { "AccountInventory[%d].Claimed",    CFTYPE_INT,    OFFSETOF(ShardAccountInventoryItem, claimed_total),    CMD_ARRAY,    OFFSETOF(ShardAccountCon,inventory), NULL, alloc_inventory    },
    { "AccountInventory[%d].Expires",    CFTYPE_INT,    OFFSETOF(ShardAccountInventoryItem, expires),        CMD_ARRAY,    OFFSETOF(ShardAccountCon,inventory), NULL, alloc_inventory    },

    { 0 }
};

SpecialColumn sgroup_cmds[] =
{
    { "Name",            CFTYPE_UNICODESTRING,    OFFSETOF(GroupCon,name),        },
    { "MissionMapId",    CFTYPE_INT,                OFFSETOF(GroupCon,map_id),        },
    { "LeaderId",        CFTYPE_INT,                OFFSETOF(GroupCon,leader_id),    },
    { 0 }
};

SpecialColumn team_cmds[] =
{
    { "MissionMapId",    CFTYPE_INT,        OFFSETOF(GroupCon,map_id),        },
    { "LeaderId",        CFTYPE_INT,        OFFSETOF(GroupCon,leader_id),    },
    { 0 }
};

SpecialColumn map_cmds[] =
{
    { "MapName",        CFTYPE_ANSISTRING,    OFFSETOF(MapCon,map_name)                                        },
    { "MissionInfo",    CFTYPE_ANSISTRING,    OFFSETOF(MapCon,mission_info)                                    },
    { "MapGroupsId",    CFTYPE_INT,            OFFSETOF(MapCon,mapgroup_id),    CMD_MEMBER,    CONTAINER_MAPGROUPS    },
    { "DontAutoStart",    CFTYPE_INT,            OFFSETOF(MapCon,dontAutoStart)                                    },
    { "Transient",        CFTYPE_INT,            OFFSETOF(MapCon,transient)                                        },
    { "NewPlayerSpawn",    CFTYPE_INT,            OFFSETOF(MapCon,deprecated)                                        },
    { "IntroZone",        CFTYPE_INT,            OFFSETOF(MapCon,introZone)                                        },
    { "BaseMapID",        CFTYPE_INT,            OFFSETOF(MapCon,base_map_id)                                    },
    { "SafePlayersLow",    CFTYPE_INT,            OFFSETOF(MapCon,safePlayersLow)                                    },
    { "SafePlayersHigh",CFTYPE_INT,            OFFSETOF(MapCon,safePlayersHigh)                                },
    { 0 }
};

void dbInit(int start_static)
{
    struct hostent    *host_ent;
    int            i;

    initCtrlHandler();
    lock_id_hashes = stashTableCreateInt(4);
    dbNetInit();
    turnstileCommInit();
    startMapCrashReportThread();

    svrMonInit();  
    if (!authCommInit(server_cfg.auth_server,server_cfg.auth_server_port))
        FatalErrorf("Can't find auth server %s:%d quitting.\n",server_cfg.auth_server,server_cfg.auth_server_port);
    if(server_cfg.queue_server)
        queueserver_commInit();
    else
        clientCommInit();

    pnameInit();

    gethostname(my_hostname,sizeof(my_hostname));
    host_ent = gethostbyname(my_hostname);

    if (!server_cfg.sql_db_name[0] || stricmp(server_cfg.sql_db_name,"master")==0)
        FatalErrorf("Can't use 'master' as your db.");
    if (gDatabaseProvider == DBPROV_UNKNOWN)
        FatalErrorf("No SqlDbProvider set");

    assert(sqlConnInit(SQLCONN_MAX));
    sqlConnSetLogin(server_cfg.sql_login);
    if (!sqlConnDatabaseConnect(server_cfg.sql_db_name, "DBSERVER"))
    {
        sqlCheckDdl(DDL_ADD);
        if (sqlConnDatabaseConnect(NULL, "DBSERVER_INIT"))
            sqlExecDdl(DDL_ADD, server_cfg.sql_init, SQL_NTS);
        if (!sqlConnDatabaseConnect(server_cfg.sql_db_name, "DBSERVER"))
            FatalErrorf("Giving up.\n");
    }

    odbcInitialSetup();
    queryDatabaseVersion();

    printf_stderr("%s SQL server %d connected\n", timerGetTimeString(), gDatabaseVersion);
    sqlFifoInit();

    loadstart_printf("Loading templates...");
    var_attrs = tpltLoadAttributes("server/db/templates/vars.attribute","server/db/templates/vars.attribute","Attributes");
    badge_attrs = tpltLoadAttributes("server/db/templates/badgestats.attribute","server/db/templates/badgestats.attribute","BadgeStatsAttributes");
    pop_help_attrs = tpltLoadAttributes("server/db/templates/pophelp.attribute","server/db/templates/pophelp.attribute","PopHelpAttributes");

    testdatabasetypes_list = containerListRegister(CONTAINER_TESTDATABASETYPES,"TestDataBaseTypes",sizeof(DbContainer),NULL,NULL,NULL);
    ent_list = containerListRegister(CONTAINER_ENTS,"Ents",sizeof(EntCon),entStatusCb,entContainerUpdtCb,ent_cmds);
    doors_list = containerListRegister(CONTAINER_DOORS,"Doors",sizeof(DoorCon),doorStatusCb,0,0);
    teamups_list = containerListRegister(CONTAINER_TEAMUPS,"Teamups",sizeof(GroupCon),groupStatusCb,0,team_cmds);
    supergroups_list = containerListRegister(CONTAINER_SUPERGROUPS,"Supergroups",sizeof(GroupCon),groupStatusCb,0,sgroup_cmds);
    taskforces_list = containerListRegister(CONTAINER_TASKFORCES,"Taskforces",sizeof(GroupCon),groupStatusCb,0,team_cmds);
    email_list = containerListRegister(CONTAINER_EMAIL,"Email",sizeof(DbContainer),0,0,0);
    petitions_list = containerListRegister(CONTAINER_PETITIONS,"Petitions",sizeof(DbContainer),0,0,0);
    crashedmap_list = containerListRegister(CONTAINER_CRASHEDMAPS,"CrashedMaps",sizeof(MapCon),mapStatusCb,0,0);
    mapgroups_list = containerListRegister(CONTAINER_MAPGROUPS,"MapGroups",sizeof(GroupCon),groupStatusCb,0,0);
    map_list = containerListRegister(CONTAINER_MAPS,"maps",sizeof(MapCon),mapStatusCb,0,map_cmds);
    arenaevent_list = containerListRegister(CONTAINER_ARENAEVENTS,"ArenaEvents",sizeof(DbContainer),0,0,0);
    arenaplayer_list = containerListRegister(CONTAINER_ARENAPLAYERS,"ArenaPlayers",sizeof(DbContainer),0,0,0);
    baseraid_list = containerListRegister(CONTAINER_BASERAIDS,"BaseRaids",sizeof(DbContainer),0,0,0);
    base_list = containerListRegister(CONTAINER_BASE,"Base",sizeof(DbContainer),0,0,0);
    stat_sgrp_list = containerListRegister(CONTAINER_SGRPSTATS,"statserver_SupergroupStats",sizeof(DbContainer),0,0,0);
    itemofpowergame_list = containerListRegister(CONTAINER_ITEMOFPOWERGAMES,"ItemOfPowerGames",sizeof(DbContainer),0,0,0);
    itemofpower_list = containerListRegister(CONTAINER_ITEMSOFPOWER,"ItemsOfPower",sizeof(DbContainer),0,0,0);
    offline_list = containerListRegister(CONTAINER_OFFLINE,"Offline",sizeof(DbContainer),0,0,0);
    sgraidinfo_list = containerListRegister(CONTAINER_SGRAIDINFO,"SGRaidInfos",sizeof(DbContainer),0,0,0);
    launcher_list = containerListRegister(CONTAINER_LAUNCHERS,"launchers",sizeof(LauncherCon),launcherStatusCb,0,0);
    serverapp_list = containerListRegister(CONTAINER_SERVERAPPS,"ServerApps",sizeof(ServerAppCon),serverAppStatusCb,0,0);
    mineacc_list = containerListRegister(CONTAINER_MININGACCUMULATOR,"MiningAccumulator",sizeof(DbContainer),0,0,0);
    levelingpacts_list = containerListRegister(CONTAINER_LEVELINGPACTS,"LevelingPacts",sizeof(GroupCon),groupStatusCb,0,0);
    league_list = containerListRegister(CONTAINER_LEAGUES,"Leagues",sizeof(GroupCon),groupStatusCb,0,0);
    eventhistory_list = containerListRegister(CONTAINER_EVENTHISTORY,"EventHistory",sizeof(DbContainer),dbEventHistory_StatusCb,dbEventHistory_UpdateCb,0);
    autocommands_list = containerListRegister(CONTAINER_AUTOCOMMANDS,"AutoCommands",sizeof(DbContainer),0,0,0);
    shardaccounts_list = containerListRegister(CONTAINER_SHARDACCOUNT,"ShardAccounts",sizeof(ShardAccountCon),0,0,shardaccount_cmds);

    tpltAddMembership(ent_list->id,teamups_list->tplt);
    tpltAddMembership(ent_list->id,supergroups_list->tplt);
    tpltAddMembership(ent_list->id,taskforces_list->tplt);
    tpltAddMembership(ent_list->id,levelingpacts_list->tplt);
    tpltAddMembership(ent_list->id,league_list->tplt);
    tpltAddMembership(map_list->id,mapgroups_list->tplt);

    loadend_printf("");

    // Any foreign key constraints in the list below need to be special cased
    // with a barrier in handleContainerSet() to prevent a foreign key
    // constraint conflict between the insertion and the referencing thread.
    if (server_cfg.write_teamups_to_sql)
    {
        tpltRegisterForeignKeyConstraint("Ents","TeamupsId","Teamups");
        //tpltRegisterForeignKeyConstraint("Ents2","RaidsId","Raids");
        tpltRegisterForeignKeyConstraint("Ents2","LeaguesId","Leagues");
    }
    else
    {
        sqlRemoveForeignKeyConstraintAsync("Ents", "TeamupsId", "Teamups");
        //sqlRemoveForeignKeyConstraintAsync("Ents2","RaidsId","Raids");
        sqlRemoveForeignKeyConstraintAsync("Ents2","LeaguesId","Leagues");
    } 
    tpltRegisterForeignKeyConstraint("Ents","TaskforcesId","Taskforces");
    tpltRegisterForeignKeyConstraint("Ents","SupergroupsId","Supergroups");
    tpltRegisterForeignKeyConstraint("Ents2","LevelingPactsId","LevelingPacts");
    tpltRegisterForeignKeyConstraint("Base","SupergroupId","Supergroups");
    //tpltRegisterForeignKeyConstraint("Maps","MapGroupsId","MapGroups"); 

    loadstart_printf("Update tables...");
    tpltUpdateSqlcolumns(testdatabasetypes_list->tplt);
    tpltUpdateSqlcolumns(ent_list->tplt);
    tpltUpdateSqlcolumns(teamups_list->tplt);
    tpltUpdateSqlcolumns(supergroups_list->tplt);
    tpltUpdateSqlcolumns(email_list->tplt);
    tpltUpdateSqlcolumns(taskforces_list->tplt);
    tpltUpdateSqlcolumns(petitions_list->tplt);
    tpltUpdateSqlcolumns(mapgroups_list->tplt);
    tpltUpdateSqlcolumns(arenaevent_list->tplt);
    tpltUpdateSqlcolumns(arenaplayer_list->tplt);
    tpltUpdateSqlcolumns(baseraid_list->tplt);
    tpltUpdateSqlcolumns(base_list->tplt);
    tpltUpdateSqlcolumns(stat_sgrp_list->tplt);
    tpltUpdateSqlcolumns(itemofpowergame_list->tplt);
    tpltUpdateSqlcolumns(itemofpower_list->tplt);
    tpltUpdateSqlcolumns(offline_list->tplt);
    tpltUpdateSqlcolumns(sgraidinfo_list->tplt);
    tpltUpdateSqlcolumns(mineacc_list->tplt);
    tpltUpdateSqlcolumns(levelingpacts_list->tplt);
    tpltUpdateSqlcolumns(league_list->tplt);
    tpltUpdateSqlcolumns(eventhistory_list->tplt);
    tpltUpdateSqlcolumns(autocommands_list->tplt);
    tpltUpdateSqlcolumns(shardaccounts_list->tplt);
    tpltDropUnreferencedTables();
    sqlFifoFinish();

#if 0
    sqlExecAsync("CREATE NONCLUSTERED INDEX [Stats21] ON [dbo].[Stats] ([Category] ASC, [ThisMonth] ASC, [Name] ASC);");
    sqlExecAsync("CREATE NONCLUSTERED INDEX [Stats22] ON [dbo].[Stats] ([Name] ASC, [ThisMonth] ASC, [Category] ASC);");
    sqlExecAsync("CREATE NONCLUSTERED INDEX [Stats23] ON [dbo].[Stats] ([Name] ASC, [Yesterday] ASC);");
    sqlExecAsync("CREATE NONCLUSTERED INDEX [Stats24] ON [dbo].[Stats] ([Name] ASC, [Today] ASC);");
#endif

    sqlAddIndexAsync("Petitions_Fetched_date", "Petitions", "Fetched ASC, Date ASC");
    loadend_printf("");
    
    // nuke teamups on restart
    loadstart_printf("Clearing teamups and invalid characters...");
    sqlExecAsync("UPDATE dbo.Ents SET TeamupsId = NULL WHERE TeamupsId is not NULL;", SQL_NTS);
    sqlExecAsync("UPDATE dbo.Ents2 SET RaidsId = NULL WHERE RaidsId is not NULL;", SQL_NTS);
    sqlExecAsync("UPDATE dbo.Tasks SET MissionMapId = NULL WHERE MissionMapId is not NULL;", SQL_NTS);
    sqlExecAsync("UPDATE dbo.TaskForceTasks SET MissionMapId = NULL WHERE MissionMapId is not NULL;", SQL_NTS);
    sqlExecAsync("UPDATE dbo.Ents2 SET LeaguesId = NULL WHERE LeaguesId is not NULL;", SQL_NTS);

    tpltDeleteAll(testdatabasetypes_list->tplt, NULL, true);
    tpltDeleteAll(teamups_list->tplt, NULL, true);
    tpltDeleteAll(league_list->tplt, NULL, true);
    tpltDeleteAll(levelingpacts_list->tplt, "Count", false);
    tpltDeleteAll(ent_list->tplt, "AuthName", false); // get rid of incomplete characters
    tpltDeleteAll(base_list->tplt, "UserId IS NULL AND SupergroupId", false); // ah, sql injection, my old nemesis

    tpltSetAllForeignKeyConstraintsAsync();
    sqlFifoFinish();
    loadend_printf("");
    
    repairAuthId();

#if defined(DO_STUPID_DATABASE_TESTS)
    testDataBaseTypes(testdatabasetypes_list);
#endif
    offlineInitOnce();
    backupLoadIndexFiles();
    
    // Populate the running stats
    stat_ReadTable();
    
    teamups_list->tplt->dont_write_to_sql = !server_cfg.write_teamups_to_sql;
    league_list->tplt->dont_write_to_sql = !server_cfg.write_teamups_to_sql;
    containerListLoadFile(doors_list);
    doorListSetSpecialInfo(doors_list);
    pnameAddAll();
    dbEventHistory_Init();

    accountLoyaltyRewardTreeLoad();

    #ifdef OFFLINE_ENABLE_DELETION_LOG_IMPORT_CODE
        if ( deletion_logs_to_process[0] != '\0' )
        {
            offlineProcessDeletionLogFiles(deletion_logs_to_process);
            _exit(0);
        }
    #endif
    
    //tryConnectConnServer(server_cfg.conn_server,server_cfg.db_server_id,server_cfg.db_server_name);

    launcherCommInit();

    // launchers
    if (start_static != -1) {
        int timer = timerAlloc();
        int calltimer = timerAlloc();
        MapCon        *map_con;
        F32 minTimeToWait=15;
        int minStaticMapLaunchers=1;
        F32 minTimeToWaitPerLauncher=3;
        bool adequateLaunchersConnected=false;

        if (start_static)
        {
            minTimeToWait = 10;
            minStaticMapLaunchers = 12;
            minTimeToWaitPerLauncher = 5;
        }

        do
        {
            U32 startTime = timerCpuSeconds();
            int last_count=0;
            launcherCommLog(1);
            timerStart(timer);
            printf_stderr("%s Waiting for launchers to link up...\n", timerGetTimeString());
            // Wait at least 15 seconds, then wait up to 5 more for 12 launchers to connect (that's all we need for static maps to start)
            // Additionally, reset the count whenever a launcher connects or if anyone is still connecting
            while(timerElapsed(timer) < minTimeToWait && launcherCountStaticMapLaunchers() < minStaticMapLaunchers || timerElapsed(timer) < minTimeToWaitPerLauncher)
            {
                if (launcherCountStaticMapLaunchers()!=last_count || launchersStillConnecting()) {
                    last_count = launcherCountStaticMapLaunchers();
                    timerStart(timer); // Reset timer whenever a launcher connects
                }
                timerStart(calltimer);
                sendSqlKeepAlive();
                delinkDeadLaunchers();
                NMMonitor(1);
                if (timerElapsed(calltimer) > 1)
                {
                    // If more than a 1 second tick, reset the timers and keep listening!
                    timerStart(timer);
                }
            }
            LOG_OLD( "%d zone launchers connected, %d seconds", launcherCountStaticMapLaunchers(), timerCpuSeconds() - startTime);
            if (launcherCountStaticMapLaunchers()==0) {
                if (start_static)
                    printf_stderr("%s Error: no launchers connected, starting without -startall mode\n", timerGetTimeString());
                else
                    printf_stderr("%s No zone launchers connected.\n", timerGetTimeString());
                start_static = 0;
            } else {
                if (start_static)
                    printf_stderr("%s Starting maps (%d zone launchers available)...\n", timerGetTimeString(), launcherCountStaticMapLaunchers());
                else
                    printf_stderr("%s %d zone launchers connected.\n", timerGetTimeString(), launcherCountStaticMapLaunchers());
            }

            // Start LogServer, other servers
            adequateLaunchersConnected = serverAutoStartInit();
            if (!adequateLaunchersConnected) {
                printf_stderr("Launchers specified in loadBalanceShardSpecific.cfg or servers.cfg are not connected. Without these launchers the application servers will function in a degraded state.\n");
                adequateLaunchersConnected = true;
                //int response = MessageBox(compatibleGetConsoleWindow(), "Launchers specified in loadBalanceShardSpecific.cfg or servers.cfg are not connected (see console).\nThese MUST be connected for the server to operate properly, please fix this and click Retry.  (See DbServer console window for details.)", "DbServer Startup ERROR", MB_ABORTRETRYIGNORE|MB_SYSTEMMODAL);
                //if (response==IDABORT) {
                //    exit(1);
                //} else if (response==IDRETRY) {
                //    // Loop
                //    loadBalanceConfigLoad(); // Reload
                //    WeeklyTFCfgLoad();
                //    serverCfgLoad(); // Reload (for Master-BeaconServer IP).
                //} else if (response==IDIGNORE) {
                //    adequateLaunchersConnected = true;
                //}
            }
        } while (!adequateLaunchersConnected);        

        setFieldRequiresCR(0);
        containerListLoadFile(map_list);
        setFieldRequiresCR(1);
        for(i=0;i<map_list->num_alloced;i++)
        {
            map_con = (MapCon *)map_list->containers[i];
            map_con->is_static = 1;

            // This mangles the map name in the event of it not being first.
            //getMapName(map_con->data,map_con->map_name);
        }
        static_map_count = 0; // Cause staticMapList to get regenerated, if a "bad" mapserver connected while
        // we were waiting for our launchers, it will have generated a bad list.
        for(i=0;i<map_list->num_alloced;i++)
        {
            int j;

            map_con = (MapCon *)map_list->containers[i];

            if(!map_con->base_map_id)
            {
                for(j=0;j<map_list->num_alloced;j++)
                {
                    MapCon* other_map = (MapCon *)map_list->containers[j];

                    if(other_map->base_map_id == map_con->id)
                    {
                        map_con->base_map_id = map_con->id;
                        break;
                    }
                }
            }
            else if(map_con->base_map_id != map_con->id)
            {
                for(j=0;j<map_list->num_alloced;j++)
                {
                    MapCon* base_map = (MapCon *)map_list->containers[j];

                    if(base_map->id == map_con->base_map_id)
                    {
                        // Copy stuff from my base map.

                        if(!map_con->map_name[0])
                        {
                            char buffer[10000];
                            sprintf(buffer, "\tMapName \"%s\"\n%s", base_map->map_name, map_con->raw_data);
                            free(map_con->raw_data);
                            map_con->raw_data = strdup(buffer);
                            map_con->container_data_size = (U32)(strlen(buffer) + 1);
                            strcpy(map_con->map_name, base_map->map_name);
                        }

                        if(!map_con->safePlayersLow)
                        {
                            map_con->safePlayersLow = base_map->safePlayersLow;
                            map_con->safePlayersHigh = base_map->safePlayersHigh;
                        }

                        break;
                    }
                }
            }

            if(map_con->safePlayersLow > map_con->safePlayersHigh)
                map_con->safePlayersHigh = map_con->safePlayersLow;
        }
    
        if (!start_static) {
            int count=0;
            for(i=0; i<map_list->num_alloced; i++)
            {
                map_con = (MapCon *)map_list->containers[i];
                if (map_con->dontAutoStart)
                    continue;
                printf_stderr("Would start map %d %s\n", map_con->id, map_con->map_name);
                count++;
            }
            printf_stderr("Would start %d maps at startup on live\n", count);
        } else {
            int launcher_count = launcherCountStaticMapLaunchers();
            int base;
            int abort=0;
            int launched=0;
            int num_starting;
            // Loop through entire list by number of launchers
            for(base=0; !abort && base<map_list->num_alloced && launched < start_static; )
            {
                int low=-1, high=-1, count=0;
                // Find a range of autoStarting maps of size launcher_count...
                for (i=base; i<map_list->num_alloced && count<launcher_count; i++)
                {
                    map_con = (MapCon *)map_list->containers[i];
                    if (!map_con->dontAutoStart)
                    {
                        if (low==-1)
                            low = i;
                        high=i;
                        count++;
                    }
                }
                if (low==-1)
                    break;
                // Start them
                printf_stderr("%s Starting maps %d-%d...", timerGetTimeString(), ((MapCon *)map_list->containers[low])->id, ((MapCon *)map_list->containers[high])->id);

                for (i=low; !abort && i<=high; i++)
                {
                    map_con = (MapCon *)map_list->containers[i];
                    if (!map_con->dontAutoStart)
                    {
                        int ret = launcherCommStartProcess(my_hostname,0,map_con);
                        printf_stderr("%d ", map_con->id);
                        if (!ret) 
                        {// Will wait forever otherwise.
                            printf_stderr("\nFailed to find launcher to start static map, not launching static maps\n");
                            abort = 1;
                            break;
                        }
                        launched++;
                    }
                }
                if (abort)
                    break;
                timerStart(timer);
                // Wait for all of the maps to complete
                do {
                    sendSqlKeepAlive();
                    delinkCrashedMaps();
                    svrMonSendUpdates();
                    NMMonitor(1);
                    num_starting = 0;
                    for (i=low; i<=high; i++) {
                        map_con = (MapCon *)map_list->containers[i];
                        if (!map_con->dontAutoStart && (!map_con->active || map_con->starting))
                            num_starting++;
                    }
                } while (num_starting && timerElapsed(timer) < 200.f);
                printf_stderr("\n");
                base = high+1;
            }
        }
        timerFree(timer);
        timerFree(calltimer);
    }
    launcherCommLog(0);

    arenaCommInit(); // Do not start listening for arena servers until after we're done starting static maps
    statCommInit();
    beaconCommInit();
    auctionMonitorInit();
    accountMonitorInit();
    missionserver_commInit();
    accountCatalogInit();
    if(!server_cfg.queue_server)
        clientCommInitStartListening();

    // Make sure all startup SQL commands are finished
    sqlFifoFinish();
}

static void startupInfo(int argc,char **argv)
{
    static char program_parms[1000];
    int        i;
    char    buffer[MAX_PATH];

    for(i=0;i<argc;i++)
    {
        strcat(program_parms,argv[i]);
        strcat(program_parms," ");
    }

    getcwd(buffer, MAX_PATH);

    printf_stderr(    "(%d) %s %s\n",_getpid(),getExecutableName(),program_parms);
    printf_stderr( "working dir: %s\n", buffer);
    printf_stderr( "SVN Revision: %s\n", build_version);
}

static int enterString(char* buffer, int maxLength, F32 timeout){
    int length = 0;
    int timeStart = timeGetTime();

    buffer[length] = 0;

    while(1){
        int timeCur;
        int key;

        timeCur = timeGetTime();

        if(timeCur - timeStart >= timeout * 1000){
            key = 27;
        }else{
            resetSlowTickTimer();
            Sleep(10);
            key = _kbhit() ? _getch() : -1;
        }

        if(key > 0)
        {
            timeStart = timeCur;
        }

        switch(key){
            xcase -1:
        // Ignored.

        xcase 0:
            case 224:{
                // Eat extended keys.
                _getch();
                     }

                     xcase 27:{
                         while(length > 0){
                             backSpace(1, 1);
                             buffer[--length] = 0;
                         }
                         return 0;
                     }

                     xcase 13:{
                         return 1;
                     }

                     xcase 8:{
                         if(length > 0){
                             backSpace(1, 1);
                             buffer[--length] = 0;
                         }
                     }

xdefault:{
                     if(    length < maxLength &&
                         (    key >= 'a' && key <= 'z' ||
                         key >= '0' && key <= '9' ||
                         strchr(" !@#$%^&*()-_=_[{]}\\|;:'\",<.>/?`~", key)))
                     {
                         buffer[length++] = key;
                         buffer[length] = 0;
                         printf("%c", key);
                     }
            }
        }
    }
}


static int isAllNumbers(const char* str)
{
    if(!str || !*str)
    {
        return 0;
    }

    if(*str == '-')
    {
        str++;
    }

    for(; *str; str++)
    {
        if(!isdigit(*str))
        {
            return 0;
        }
    }

    return 1;
}

static int strHasSubStringNoUnderscores(char* str, char* strFind)
{
    char strBuf[1000];
    char strFindBuf[1000];

    strcpy(strBuf, stripUnderscores(str));
    strcpy(strFindBuf, stripUnderscores(strFind));

    return strstri(strBuf, strFindBuf) ? 1 : 0;
}

static void dbLaunchMapFromConsole()
{
    char buffer[1000];
    int ret;

    consoleSetFGColor(COLOR_BRIGHT|COLOR_GREEN);

    printf(    "To launch a static map, enter the map name or id.  3 second timeout.\n"
        "Prefix with \"name:\" to force name lookup.\n"
        "> ");

    consoleSetDefaultColor();

    ret = enterString(buffer, ARRAY_SIZE(buffer) - 1, 3);

    while(ret && buffer[0] == ' ')
    {
        memmove(buffer, buffer + 1, strlen(buffer));
    }

    if(ret && buffer[0])
    {
        int        map_id = 0;
        char    errmsg[1000];
        MapCon* map_con = NULL;

        printf("\n");

        if(!isAllNumbers(buffer) || !strnicmp(buffer, "name:", 5)){
            int        i;
            int        count = 0;

            if(!strnicmp(buffer, "name:", 5))
            {
                memmove(buffer, buffer + 5, strlen(buffer) - 4);
            }

            for(i = 0; i < map_list->num_alloced; i++)
            {
                MapCon*    con = (MapCon*)map_list->containers[i];

                if (con && con->is_static && strHasSubStringNoUnderscores(con->map_name, buffer))
                {
                    map_con = con;
                    count++;
                    printf("%3d. Found: %9s id %5d:%s\n", count, con->starting || con->map_running ? "(RUNNING)" : "", con->id, con->map_name);
                }
            }

            if(count == 1)
            {
                map_id = map_con->id;
            }
            else
            {
                map_con = NULL;

                if(!count)
                {
                    printf("No map names match: %s\n", buffer);
                }
                else
                {
                    printf("More than one match, please refine search!\n", buffer);
                }
            }
        }
        else
        {
            int i;

            map_id = atoi(buffer);

            for(i = 0; i < map_list->num_alloced; i++)
            {
                MapCon*    con = (MapCon*)map_list->containers[i];

                if (con && con->is_static && con->id == map_id)
                {
                    map_con = con;
                    break;
                }
            }

            if(!map_con)
            {
                printf("No map with id: %d\n", map_id);
                map_id = 0;
            }
        }

        if(map_con)
        {
            printf("Map found: %d:%s%s\n", map_con->id, map_con->map_name, map_con->starting || map_con->map_running ? " (ALREADY RUNNING)" : "");

            consoleSetFGColor(COLOR_BRIGHT|COLOR_GREEN);

            printf("Is this the map you want to launch (y/n)? ");

            consoleSetDefaultColor();

            if(enterString(buffer, 1, 3) && !stricmp(buffer, "y")){
                printf("\n");

                map_con = startMapIfStatic(NULL, NULL, map_con->id, errmsg, "dbLaunchMapFromConsole");

                if(map_con){
                    printf("Launching map %d: %s\n", map_con->id, map_con->map_name);
                }else{
                    printf("Failed to launch map %d: %s\n", map_id, errmsg);
                }
            }else{
                printf("Canceled!\n");
            }
        }
    }
    else
    {
        printf("Canceled!\n");
    }

    printf("\n");

    resetSlowTickTimer();
}

static void floodAuthWithQuits(int nQuits)
{
    int i;
    for( i = 0; i < nQuits; ++i ) 
    {
        // reason can be 1 to 6 AFAICT
        authSendQuitGame(i+1,1,0);
    }
}

static void dbserverAsserCallback(char * errMsg)
{
    LOG( LOG_ERROR, LOG_LEVEL_IMPORTANT, 0, errMsg);
}

#ifndef FINAL
extern int g_force_production_mode;
#endif

int main(int argc,char **argv)
{
    int        i,start_static=0,flat_to_sql=0,is_log_server=0,init_encryption = 1;

    memCheckInit();

    EXCEPTION_HANDLER_BEGIN
    
    setWindowIconColoredLetter(compatibleGetConsoleWindow(), 'D', 0x00ff00);

    regSetAppName("CoH");
    
    timeBeginPeriod(1);

    logSetMsgQueueSize(32 * 1024 * 1024);
    FolderCacheChooseMode();

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
    setAssertCallback(dbserverAsserCallback);
    startupInfo(argc, argv);

    if (strstri(argv[0],"logserver"))
        is_log_server = 1;

    for(i=1;i<argc;i++)
    {
        if (stricmp(argv[i],"-logserver")==0)
            is_log_server = 1;
        else if (stricmp(argv[i],"-noencrypt")==0)
            init_encryption = 0;
#ifndef FINAL
        else if (stricmp(argv[i],"-productionmode")==0)
            g_force_production_mode = 1;
#endif
    }

    sockStart();
    packetStartup(0, init_encryption);    
    logSetDir("dbserver");
    logSetUseTimestamped(true);
    serverCfgLoad();
    if(!server_cfg.do_not_preload_crashreportdll)
    {
        sdStartup(); // Pre-start the stack dumper thread
    }

    if(is_log_server)
    {
        setWindowIconColoredLetter(compatibleGetConsoleWindow(), 'L', 0x00ff00);
        // adding ctrl handler here so that when the dbserver instance that is running as a logserver
        //quits, it can properly close the zmqsocket in logserver.c
        initCtrlHandler();
        logMain();
        exit(0);
    }

    staticMapInfosLoad();
    WeeklyTFCfgLoad();
    while (1) {
        int response;
        if (loadBalanceConfigLoad())
            break;
        if (isDevelopmentMode()) // No message box in development mode
            break;
        response = MessageBoxA(compatibleGetConsoleWindow(), "Errors found in loading loadBalanceShardSpecific.cfg.  These MUST be connected for the server to operate properly, please fix this and click Retry.  (See DbServer console window for details.)", "DbServer Startup ERROR", MB_ABORTRETRYIGNORE|MB_SYSTEMMODAL);
        if (response==IDABORT) {
            exit(1);
        } else if (response==IDRETRY) {
            // Loop
        } else if (response==IDIGNORE) {
            break;
        }
    }
    if (!server_cfg.use_logserver)
        logNetInit();
    else if (server_cfg.use_logserver == 2)
    {
        char    buf[1000],logserver_name[1000];

        strcpy(logserver_name,argv[0]);
        getDirectoryName(logserver_name);
        strcat(logserver_name,"\\logserver.exe");
        sprintf(buf,"copy %s %s",argv[0],logserver_name);
        system(buf);
        system_detach(logserver_name, 0);
    }
    consoleInit(110, 128, 0);
    if (server_cfg.client_project[0]){
        regSetAppName(server_cfg.client_project);
    }
    printf_stderr("dbserver starting\n");
    errorSetVerboseLevel(1);
    for(i=1;i<argc;i++)
    {
        if (stricmp(argv[i],"-startall")==0)
            start_static = INT_MAX;
        else if (stricmp(argv[i],"-start")==0)
        {
            if (i==argc) Errorf("-start needs a count\n");
            start_static = atoi(argv[i+1]);
            i++;
        }
        else if (stricmp(argv[i],"-verbose")==0)
            errorSetVerboseLevel(1);
        else if (stricmp(argv[i], "-productionmode")==0)
        {
            // already handled above
        }
        else if (stricmp(argv[i],"-testcleanshutdown")==0)
        {
            int containers = 100;
            if (isProductionMode() && okCancelDialog("This test will modify data in the database.\r\n\r\nDo you wish to continue?", "testcleanshutdown") != IDOK)
                exit(1);
            if (i < argc-1)
                containers = atoi(argv[i+1]);
            dbInit(-1);
            modifyContainersAsync(containers);
            dbShutdown(NULL,0);
        }
        else if (stricmp(argv[i],"-perfbench")==0)
        {
            if (isProductionMode() && okCancelDialog("This benchmark will modify and duplicate data in the database.\r\n\r\nDo you wish to continue?", "perfbench") != IDOK)
                exit(1);
            dbInit(-1);
            perfLoad(3);
            perfUpdate(3);
            dupContainer(3);
            dbShutdown(NULL, 0);
        }
        else if (stricmp(argv[i],"-perfload")==0)
        {
            int loops = 0;
            if (isProductionMode() && okCancelDialog("This benchmark will put a large amount of load on the database.\r\n\r\nDo you wish to continue?", "perfload") != IDOK)
                exit(1);
            if (i < argc-1)
                loops = atoi(argv[i+1]);
            dbInit(-1);
            perfLoad(loops);
            dbShutdown(NULL, 0);
        }
        else if (stricmp(argv[i],"-perfdup")==0)
        {
            int loops = 0;
            if (isProductionMode() && okCancelDialog("This benchmark will duplicate data in the database.\r\n\r\nDo you wish to continue?", "perfdup") != IDOK)
                exit(1);
            dbInit(-1);
            if (i < argc-1)
                loops = atoi(argv[i+1]);
            dupContainer(loops);
            dbShutdown(NULL, 0);
        }
        else if (stricmp(argv[i],"-perfupdate")==0)
        {
            int loops = 0;
            if (isProductionMode() && okCancelDialog("This benchmark will modify data in the database.\r\n\r\nDo you wish to continue?", "perfupdate") != IDOK)
                exit(1);
            if (i < argc-1)
                loops = atoi(argv[i+1]);
            dbInit(-1);
            perfUpdate(loops);
            dbShutdown(NULL, 0);
        }
        else if (stricmp(argv[i],"-flattosql")==0)
        {
            assert(!isProductionMode());
            dbInit(-1);
            flatToSql();
            dbShutdown(NULL, 0);
        }
        else if (stricmp(argv[i],"-sqltoflat")==0)
        {
            assert(!isProductionMode());
            dbInit(-1);
            sqlToFlat();
            dbShutdown(NULL, 0);
        }
        else if (stricmp(argv[i],"-importdump") == 0)
        {
            dbInit(-1);
            importDump(argc - i - 1,&argv[i+1]);
            dbShutdown(NULL, 0);
        }
        else if (stricmp(argv[i],"-fiximport") == 0)
        {
            dbInit(-1);
            fixImport(argc - i - 1,&argv[i+1]);
            dbShutdown(NULL, 0);
        }
        else if (stricmp(argv[i],"-exportdump") == 0)
        {
            dbInit(-1);
            exportRawDump(argc - i - 1,&argv[i+1]);
            dbShutdown(NULL, 0);
        }
        else if (stricmp(argv[i],"-packetdebug")==0)
            pktSetDebugInfo();
        else if (stricmp(argv[i],"-memlogecho")==0)
        {
            memlog_setCallback(0, dbMemLogEcho);
        }
        else if (stricmp(argv[i],"-cod")==0)
            server_cfg.use_cod = 1;
        #ifdef OFFLINE_ENABLE_DELETION_LOG_IMPORT_CODE
            else if (stricmp(argv[1],"-process_deletion_log")==0 && argc > i)
                strncpy_s(deletion_logs_to_process,ARRAY_SIZE(deletion_logs_to_process),argv[++i],_TRUNCATE);
        #endif
        else
            printf_stderr("-----INVALID PARAMETER: %s\n", argv[i]);
    }


    dbInit(start_static);
    
    printf_stderr("%s DbServer Ready.\n", timerGetTimeString());

    for(;;)
    {
        extern BOOL g_authdbgprintf_enabled;
        extern BOOL g_heuristicdebug_enabled;
        extern void sendRemainTimeToClient(U32 auth_id,U32 curr_avail,U32 total_avail,U8 reservation,int countdown,int change_billing);

        msgScan();
        dbShutdownTick();
        if (_kbhit()) { // debug hack for printing out memory monitor stuff
            static int s_nQuitsToSend = 1000;
            switch (_getch()) {
                case 'm':
                    memMonitorDisplayStats();
                xcase 'w':
                    waitingEntitiesDumpStats();
                xcase 'A': 
                    g_authdbgprintf_enabled = !g_authdbgprintf_enabled;
                    printf("auth debug %s\n",g_authdbgprintf_enabled?"enabled":"disabled");
                xcase 'a':
                    logStatPrint('a');
                xcase 'b':
                    printf("Broadcasting stats...\n");
                    stat_Update();
                    printf("Done.\n");
                xcase 'c':
                    authCommDisconnect();
                xcase 'd':
                    printStringTableMemUsage();
                    printStashTableMemDump();
                xcase 'g':
                    logStatPrint('g');
                xcase 'i':
                    logStatPrint('i');
                xcase 'l':
                    dbLaunchMapFromConsole();
                xcase 'q':
                     printf("sending %i quits\n", s_nQuitsToSend);
                     floodAuthWithQuits(s_nQuitsToSend);    
                xcase 'Q':
                     printf("num quits to send (blocking, hurry!):");
                     scanf("%i", &s_nQuitsToSend);
                     printf("\n");
                xcase 'r':
                     {
                        int oldQueueStatus = server_cfg.queue_server;
                        printf("reloading servers.cfg\n");
                        serverCfgLoad();
                        if(server_cfg.queue_server != oldQueueStatus)
                        {
                            printf("Unable to change queue server use.  Please restart dbserver to apply change.\n");
                            server_cfg.queue_server = oldQueueStatus;
                        }
                        //if we have a queueserver connected, update it with the new cfg data
                        if(server_cfg.queue_server)
                            queueservercomm_sendUpdateConfig();

                        updateAllServerLoggingInformation();
                    }
                xcase 'M':
                    printf("resetting Mission Server link\n");
                    missionserver_forceLinkReset();
                xcase 's':
                    logStatPrint('s');
                xcase '[':
                    sendRemainTimeToClient(0,10*60,20*60,2,0,0);
                xcase ']':
                    sendRemainTimeToClient(0,1*60,20*60,2,0,0);
                xcase '\\':
                    sendRemainTimeToClient(0,10*60,20*60,2,0,1);
                xcase 'x':
                    printf("Enabling to transacted FIFO mode\n");
                    sqlFifoEnableTransacted(1);
                xcase 'X':
                    printf("Disabling to transacted FIFO mode\n");
                    sqlFifoEnableTransacted(0);
                xcase 'T':
                    TestTextParser();
                xcase 't':
                {
                    static int countdown_on;

                    countdown_on = !countdown_on;
                    sendRemainTimeToClient(0,10*60,20*60,2,countdown_on,1);
                }
                xcase 'H':
                    heapValidateAll();
                xcase 'h':
                {
                    g_heuristicdebug_enabled = !g_heuristicdebug_enabled;
                    printf("%s loadBalance heuristic debugging\n", g_heuristicdebug_enabled ? "Enabling" : "Disabling");
                }
                xcase '?':
                {
                    printf("\n");
#if defined(STRING_TABLE_TRACK) || defined(HASH_TABLE_TRACK)
                    printf("d = dump string table and stash table usage\n");
#endif
                    printf("A = toggle auth debug\n");
                    printf("i = cmds_in  stats\n");
                    printf("s = cmds_set stats\n");
                    printf("g = cmds_get stats\n");
                    printf("a = cmds_agg stats\n");
                    printf("b = broadcast stats\n");
                    printf("c = close AuthServer socket (causes auto-reconnect)\n");
                    printf("w = waitingEntitiesDumpStats\n");
                    printf("m = memMonitorDisplayStats\n");
                    printf("r = reload servers.cfg\n");
                    printf("M = Reset Mission Server Connection\n");
                    printf("x = Enable transacted mode for the SQL fifo\n");
                    printf("X = Disable transacted mode for the SQL fifo\n");
                    printf("h = toggle load balancer heuristic debug\n");
                    printf("H = validate heaps\n");
                }
            }
        }
    }
    dbDelink(); // Leave this in, for no other reason than it causes the function to get linked into the .exe!
    packetShutdown();

    EXCEPTION_HANDLER_END
        return 0;
}

