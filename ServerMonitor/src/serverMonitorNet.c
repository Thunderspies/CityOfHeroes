#include <comm_backend.h>
#include <container.h>
#define SVRMONCOMM_PARSE_INFO_DEFS
#include <container/containerbroadcast.h>
#include <entity/entVarUpdate.h>
#include <launcher_common.h>
#include <svrmoncomm.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/network/net_link.h>
#include <utilitieslib/network/net_socket.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/netio_core.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/ListView.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/structHist.h>
#include <utilitieslib/utils/structNet.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>

#include <assert.h>
#include <direct.h>
#include <psapi.h>
#include <windowsx.h>

#include "processMonitor.h"
#include "serverMonitorNet.h"
#include "serverMonitor.h"
#include "serverMonitorEnts.h"
#include "serverMonitorCrashMsg.h"
#include "shardMonitor.h"

U32 timestamp=0; // Set globally
bool g_someDataOutOfSync = false;

//static bool received_update=false;

static TokenizerParseInfo *destructor_tpi=NULL;
static StructHistCollection destructor_shc=NULL;
static void destructor(void *data)
{
    if (data==NULL || data==(void*)(intptr_t)1)
        return;

    shcRemove(destructor_shc, data, timestamp);

    if (destructor_tpi) {
        sdFreeStruct(destructor_tpi, data);
    } else {
        assert(0);
    }
    free(data);
}

void startServersIfNotRunning(void)
{
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    int founddb=0, foundlauncher=0, foundlogserver=0, foundmapserver=0;

    if (!g_primaryServerMonitor)
        return;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    for ( i = 0; i < cProcesses; i++ ) {
        char szProcessName[MAX_PATH] = "unknown";

        // Get a handle to the process.
        HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE, aProcesses[i] );

        // Get the process name.
        if (NULL != hProcess )
        {
            HMODULE hMod;

            if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
                &cbNeeded) )
            {
                GetModuleBaseNameA( hProcess, hMod, szProcessName, 
                    sizeof(szProcessName) );
            }
            else {
                CloseHandle( hProcess );
                continue;
            }
        }
        else continue;

        // Check the process name.
        if (stricmp(szProcessName, "dbserver")==0 ||
            stricmp(szProcessName, "dbserver.exe")==0 ||
            stricmp(szProcessName, "dbserver64")==0 ||
            stricmp(szProcessName, "dbserver64.exe")==0)
        {
            founddb=1;
        }
        if (stricmp(szProcessName, "launcher")==0 ||
            stricmp(szProcessName, "launcher.exe")==0)
        {
            foundlauncher=1;
        }
        if (stricmp(szProcessName, "logserver")==0 ||
            stricmp(szProcessName, "logserver.exe")==0)
        {
            foundlogserver=1;
        }
        if (stricmp(szProcessName, "mapserver")==0 ||
            stricmp(szProcessName, "mapserver.exe")==0)
        {
            foundmapserver=1;
        }

        CloseHandle( hProcess );
    }
    if (!foundlauncher && processmonitors[PME_LAUNCHER].monitor) {
        executeServer(processmonitors[PME_LAUNCHER].cmdline, 0);
    }
    if (!founddb && processmonitors[PME_DBSERVER].monitor) {
        executeServer(processmonitors[PME_DBSERVER].cmdline, 0);
    }
}

static void clearConsFromFilterList(DbContainer ***eaCons, DbContainer ***eaConsFiltered)
{
    int i;
    // Remove all duplicatse of the eaMaps structure that happen to be in eaMapsStuck
    for (i=0; i<eaSize(eaCons); i++) {
        DbContainer *con = (*eaCons)[i];
        eaRemove(eaConsFiltered, eaFind(eaConsFiltered, con));
    }
}

void svrMonClearAllLists(ServerMonitorState *state)
{
#define DELETE_LISTVIEW(lv) \
    if (lv) { \
        listViewDelAllItems(lv, NULL); \
    }
    DELETE_LISTVIEW(state->lvMaps);
    DELETE_LISTVIEW(state->lvMapsStuck);
    DELETE_LISTVIEW(state->lvLaunchers);
    DELETE_LISTVIEW(state->lvServerApps);
    DELETE_LISTVIEW(state->lvEnts);

    clearConsFromFilterList((DbContainer***)state->eaMaps, (DbContainer***)state->eaMapsStuck);
    destructor_shc = state->shcServerMonHistory;
    destructor_tpi = MapConNetInfo;
    eaClearEx(state->eaMaps, destructor);
    destructor_tpi = CrashedMapConNetInfo;
    eaClearEx(state->eaMapsStuck, destructor);

    destructor_tpi = LauncherConNetInfo;
    eaClearEx(state->eaLaunchers, destructor);

    destructor_tpi = ServerAppConNetInfo;
    eaClearEx(state->eaServerApps, destructor);

    destructor_tpi = EntConNetInfo;
    eaClearEx(state->eaEnts, destructor);

    destroyStructHistCollection(state->shcServerMonHistory);
    state->shcServerMonHistory = createStructHistCollection(state->dbserveraddr[0]?state->dbserveraddr:"SvrMonPerfLogs");
}

int svrMonConnect(ServerMonitorState *state, char *ip_str, bool allow_autostart)
{
    Packet    *pak;
    static bool inited=false;

    if (!inited) {
        sockStart();
        packetStartup(0,0);
        inited=true;
    }

    if (allow_autostart && (!ip_str[0] || stricmp(ip_str, "localhost")==0)) {
        startServersIfNotRunning();
        state->local_dbserver = true;
    } else {
        state->local_dbserver = false;
    }

    svrMonClearAllLists(state);

    if (!netConnect(&state->db_link, ip_str, DEFAULT_SVRMON_PORT, NLT_TCP, 5, NULL)) {
        return 0;
    }

    state->db_link.userData = state;

    netLinkSetMaxBufferSize(&state->db_link, BothBuffers, 1*1024*1024); // Set max size to auto-grow to
    netLinkSetBufferSize(&state->db_link, BothBuffers, 64*1024);
    //netLinkSetBufferSize(&state->db_link, BothBuffer, 256*1024);
    pak = pktCreateEx(&state->db_link, DBSVRMON_CONNECT);
    pktSendBits(pak, 32, SVRMON_PROTOCOL_MAJOR_VERSION);
    pktSendBits(pak, 32, SVRMON_PROTOCOL_MINOR_VERSION);
    // No longer CRC these
//    pktSendBits(pak, 32, DBSERVER_PROTOCOL_VERSION);
//    pktSendBits(pak, 32, ParserCRCFromParseInfo(EntConNetInfo));
//    pktSendBits(pak, 32, ParserCRCFromParseInfo(LauncherConNetInfo));
//    pktSendBits(pak, 32, ParserCRCFromParseInfo(CrashedMapConNetInfo));
//    pktSendBits(pak, 32, ParserCRCFromParseInfo(MapConNetInfo));
    pktSend(&pak, &state->db_link);

    svrMonRequest(state, DBSVRMON_REQUESTVERSION);

    lnkFlush(&state->db_link);
//    received_update = false;
    return 1;
}

int svrMonConnected(ServerMonitorState *state)
{
    return state->db_link.socket > 0;
}

int svrMonConnectionLooksDead(ServerMonitorState *state)
{
    return !svrMonConnected(state) ||  (svrMonGetNetDelay(state) > 30);
}

int svrMonDisconnect(ServerMonitorState *state)
{
    if (!svrMonConnected(state))
        return 0;
    state->connection_expected = false;
    netSendDisconnect(&state->db_link,2);
    return 1;
}

int svrMonRequest(ServerMonitorState *state, int msg)
{
    Packet *pak;
    if (!svrMonConnected(state))
        return 0;
    pak = pktCreateEx(&state->db_link, msg);
    pktSend(&pak, &state->db_link);
    lnkFlush(&state->db_link);
    return 1;
}

void svrMonRequestEnts(ServerMonitorState *state, int val)
{
    Packet *pak;
    if (!svrMonConnected(state))
        return;
    pak = pktCreateEx(&state->db_link, DBSVRMON_REQUEST_PLAYERS);
    pktSendBits(pak, 1, !!val);
    pktSend(&pak, &state->db_link);
    lnkFlush(&state->db_link);
    return;
}

int svrMonGetSendRate(ServerMonitorState *state)
{
    return pktRate(&state->db_link.sendHistory);
}

int svrMonGetRecvRate(ServerMonitorState *state)
{
    return pktRate(&state->db_link.recvHistory);
}

int svrMonGetNetDelay(ServerMonitorState *state)
{
    if (!state->db_link.connected)
        return 0;
    return timerCpuSeconds() - state->db_link.lastRecvTime;
}



int svrMonRequestDiff(ServerMonitorState *state)
{
    Packet *pak;
    if (!svrMonConnected(state))
        return 0;
    pak = pktCreateEx(&state->db_link, DBSVRMON_REQUESTDIFF);
    pktSend(&pak, &state->db_link);
    lnkFlush(&state->db_link);
    return 1;
}

int svrMonResetMission(ServerMonitorState *state)
{
    svrMonSendDbMessage(state, "MSLinkReset", "");
    lnkBatchSend(&state->db_link);
    return 1;
}

int svrMonShutdownAll(ServerMonitorState *state, const char *reason)
{
    if (!svrMonConnected(state))
        return 0;
    svrMonSendDbMessage(state, "Shutdown", reason);
    lnkFlush(&state->db_link);
    return 1;
}

typedef int (*ContainerFilter)(DbContainer *con, void *filterData); // Returns 1 if it passes the filter

// Macro to handle automatic casting
#define HandleRecvList(state, pak, eaCons, tpi, ptpi, size, lv, eaConsFiltered, lvFiltered, filter, filterData, logName) \
    handleRecvList(state, pak, (DbContainer***)eaCons, tpi, ptpi, size, lv, (DbContainer***)eaConsFiltered, lvFiltered, (ContainerFilter)filter, filterData, logName)

void handleRecvList(ServerMonitorState *state, Packet *pak, DbContainer ***eaCons, TokenizerParseInfo *tpi, TokenizerParseInfo **ptpi, int size, ListView *lv,
                    DbContainer ***eaConsFiltered, ListView *lvFiltered, ContainerFilter filter, void *filterData, char *logName)
{
    DbContainer *con;
    bool    full_update;
    int        id;
    int        index;
    StashTable htIds = 0;
    U32        server_time_offset;

    server_time_offset = pktGetBits(pak, 32);
    timerSetSecondsOffset(server_time_offset); // Assume the server time is the same as the local time

    full_update = pktGetBits(pak, 1);
    
    if (lv) {
        SetWindowRedraw(listViewGetListViewWindow(lv), FALSE);
    }

    if (full_update) {
        tpi = sdUnpackParseInfo(tpi, pak, false);
        *ptpi = tpi;
        if (lv) {
            listViewDelAllItems(lv, NULL);
        }
        if (lvFiltered) {
            // Warning: this assumes that in our special set up we will always receive a full update
            //  on eaMaps and then immediately receive a full update on eaMapsStuck with no diffs in between
            listViewDelAllItems(lvFiltered, NULL);
        }

        if (eaConsFiltered) {
            clearConsFromFilterList(eaCons, eaConsFiltered);
        }
        if (eaCons == (DbContainer***)state->eaMapsStuck) {
            // HACK for a full update on stuck maps, it might already contain pointers to cons in the eaMaps structure
            clearConsFromFilterList((DbContainer***)state->eaMaps, (DbContainer***)state->eaMapsStuck);
            // Then free the remaining ones (actual crashed maps)
        }
        destructor_shc = state->shcServerMonHistory;
        destructor_tpi = tpi;
        eaClearEx(eaCons, destructor);
        // This should happen implicitly from the function call above
        //if (eaConsFiltered) {
        //    eaSetSize(eaConsFiltered, 0);
        //}
    } else {
        int i;

        tpi = *ptpi;

        // Hash all of the IDs for quick lookup
        htIds = stashTableCreateInt((int)eaSize(eaCons)*1.5);
        for (i=0; i<eaSize(eaCons); i++) {
            con = (DbContainer*)eaGet(eaCons, i);
            if (con) {
                stashIntAddPointer(htIds, con->id, con, false);
            }
        }
    }

    id = pktGetBitsPack(pak, 3);
    while (id) {
        bool update;
        bool meets_filter=false;

        assert(tpi); // If we're getting data, we better have a descriptor!

        if (full_update) {
            con = (DbContainer*)calloc(size,1);
            con->id = id;
            update=false;
        } else {
            if (!stashIntFindPointer(htIds, id, &con))
                con = NULL;
            assert(!con || con->id == id);
            if (con) {
                update=true;
            } else {
                con = (DbContainer*)calloc(size,1);
                con->id = id;
                update=false;
            }
        }
        shcUpdate(&state->shcServerMonHistory, con, tpi, timestamp, logName);
        g_someDataOutOfSync |= !sdUnpackDiff(tpi, pak, con, NULL, false);
        if (!update) {
            eaPush(eaCons, con);
            if (lv)
                listViewAddItem(lv, con);
        } else {
            if (lv)
                listViewItemChanged(lv, con);
        }
        // Check filter
        if (filter) {
            meets_filter = filter(con, filterData);
        }
        if (meets_filter) {
            if (eaConsFiltered) {
                index = eaFind(eaConsFiltered, con);
                if (index==-1) {
                    // Was not previously in the filter list
                    eaPush(eaConsFiltered, con);
                } else {
                    // Already in the EArray, this must be an update
                    assert(update);
                }
            }
            if (lvFiltered) {
                index = listViewFindItem(lvFiltered, con);
                if (index==-1) {
                    listViewAddItem(lvFiltered, con);
                } else {
                    listViewItemChanged(lvFiltered, con);
                }
            }
        } else {
            // Doesn't meet the filter, remove it if it's in either
            if (eaConsFiltered) {
                eaRemove(eaConsFiltered, eaFind(eaConsFiltered, con));
            }
            if (lvFiltered) {
                index = listViewFindItem(lvFiltered, con);
                if (index!=-1) {
                    listViewDelItem(lvFiltered, index);
                }
            }
        }

        id = pktGetBitsPack(pak, 3);
    }

    // Receive deletes
    id = pktGetBitsPack(pak, 1);
    while (id) {
        bool update;
        int i;
        con=NULL;
        for (i=0; i<eaSize(eaCons); i++) {
            con = (DbContainer*)eaGet(eaCons, i);
            if (con && con->id == id)
                break;
        }
        if (con && con->id == id) {
            update=true;
            eaRemove(eaCons, i);
            if (lv) {
                listViewDelItem(lv, listViewFindItem(lv, con));
            }
            // Remove from filtered list
            if (eaConsFiltered) {
                eaRemove(eaConsFiltered, eaFind(eaConsFiltered, con));
            }
            if (lvFiltered) {
                index = listViewFindItem(lvFiltered, con);
                if (index!=-1) {
                    listViewDelItem(lvFiltered, index);
                }
            }

            destructor_shc = state->shcServerMonHistory;
            destructor_tpi = tpi;
            destructor(con);
        } else {
            assert(!"Deleting something never received!");
        }
        id = pktGetBitsPack(pak, 1);
    }

    if (htIds) {
        stashTableDestroy(htIds);
    }
    timerSetSecondsOffset(0); // Reset

    if (lv) {
        SetWindowRedraw(listViewGetListViewWindow(lv), TRUE);
    }
}

static char *notTroubleStatii = "CRASHED DELINKING... Delinked Killed";
int notTroubleStatus(char *status) {
    if (!status || !status[0])
        return 0;
    return !!strstri(notTroubleStatii, status);
}

int inTroubleFilter(MapCon *con, void *junk, ServerStats *stats)
{
    bool trouble = false;
    if (!stats) {
        static ServerStats dummy_stats;
        stats = &dummy_stats;
    }
    if (!con->starting && con->seconds_since_update >= 15 && con->seconds_since_update < 120 && !notTroubleStatus(con->status)) {
        strcpy(con->status, "STUCK");
        stats->sms_stuck_count++;
        trouble = true;
    } else if (con->starting && con->seconds_since_update >=120 && !notTroubleStatus(con->status)) {
        strcpy(con->status, "STUCK STARTING");
        stats->sms_stuck_starting_count++;
        trouble = true;
    } else if (!con->starting && con->seconds_since_update >= 120 && !notTroubleStatus(con->status)) {
        strcpy(con->status, "TROUBLE");
        stats->sms_stuck_count++;
        trouble = true;
    } else if (con->long_tick >= 1200 && con->num_players > 2 && !notTroubleStatus(con->status)) {
        int dt = (int)timerSecondsSince2000() - (int)con->on_since;
        if ( dt > 60 ) { // ignore the first minute
            strcpy(con->status, "LONG TICK");
            stats->sms_long_tick_count++;
            trouble = true;
        }
    } else if (stricmp(con->status, "CRASHED")==0) {
        stats->sms_crashed_count++;
    }
    if (trouble)
        return 1;
    return 0;
}

int inTroubleFilterSA(ServerAppCon *con, void *junk, ServerStats *stats)
{
    bool trouble = false;
    if (con->crashed) {
        if (stricmp(con->status, "Killed")!=0)
            strcpy(con->status, "CRASHED");
        stats->sa_crashed_count++;
        trouble = true;
    } else if (con->remote_process_info.process_id) {
        if (stricmp(con->status, "Killed")!=0)
            strcpy(con->status, "Running");
    } else if (con->monitor) {
        strcpy(con->status, "Not Running");
    } else {
        strcpy(con->status, "Starting");
    }
    if (trouble)
        return 1;
    return 0;
}

int stuckFilter(MapCon *con, void *junk)
{
    int trouble = inTroubleFilter(con, junk, NULL); // Set the status field
    return trouble;
//    if (con->seconds_since_update >= 15 && !con->starting || con->seconds_since_update >= 120)
//        return 1;
//    return 0;
}

void updateInTroubleState(ServerMonitorState *state)
{
    int i;
    int trouble=0;
    state->stats.sms_long_tick_count = 0;
    state->stats.sms_stuck_count = 0;
    state->stats.sms_stuck_starting_count = 0;
    state->stats.sa_crashed_count = 0;
    state->stats.sms_crashed_count = 0;
    for (i=0; i<eaSize(state->eaMapsStuck); i++) {
        if (inTroubleFilter(eaGet(state->eaMapsStuck, i), NULL, &state->stats)) {
            trouble++;
        }
    }
    for (i=0; i<eaSize(state->eaServerApps); i++) {
        if (inTroubleFilterSA(eaGet(state->eaServerApps, i), NULL, &state->stats)) {
            trouble++;
        }
    }
    if (state->stats.servers_in_trouble!=trouble)
        state->servers_in_trouble_changed=1;
    state->stats.servers_in_trouble=trouble;

}

void handleDbStats(ServerMonitorState *state, Packet* pak)
{
    int version = pktGetBitsPack(pak, 1);

    // If you change this function, make sure it's still backwards compatible by connecting to a Live and TraingingRoom DbServer
    // packet sent in svrmoncomm.c

    state->stats.pcount_login = pktGetBitsPack(pak, 10);
    state->stats.pcount_ents = pktGetBitsPack(pak, 10);

    state->stats.sqlwb = pktGetBitsPack(pak, 10);
    state->stats.servermoncount = pktGetBitsPack(pak, 10);
    state->stats.dbticklen = pktGetF32(pak);
    if(version > 5)
        state->stats.arenaSecSinceUpdate = pktGetBitsPack(pak, 10);
    if(version > 6)
        state->stats.statSecSinceUpdate = pktGetBitsPack(pak, 10);
    if(version > 7)
        state->stats.beaconWaitSeconds = pktGetBitsPack(pak, 4);
    if(version > 8)
        state->stats.heroAuctionSecSinceUpdate = pktGetBitsAuto(pak);
    if(version > 9)
        state->stats.villainAuctionSecSinceUpdate = pktGetBitsAuto(pak);
    if(version > 10)
        state->stats.accountSecSinceUpdate = pktGetBitsAuto(pak);
    if(version > 11)
        state->stats.missionSecSinceUpdate = pktGetBitsAuto(pak);
    if(version > 12) {
        state->stats.sqlthroughput = pktGetBitsAuto(pak);
        state->stats.sqlavglat = pktGetBitsAuto(pak);
        state->stats.sqlworstlat = pktGetBitsAuto(pak);
        state->stats.loglat = pktGetBitsAuto(pak);

        state->stats.logbytes = pktGetBitsAuto(pak);
        state->stats.logqcnt = pktGetBitsAuto(pak);
        state->stats.logqmax = pktGetBitsAuto(pak);
        state->stats.logsortmem = pktGetBitsAuto(pak);

        state->stats.logsortcap = pktGetBitsAuto(pak);
    }
    if(version > 21)
        state->stats.pcount_queued = pktGetBitsAuto(pak);
    if(version > 22)
        state->stats.queue_connections = pktGetBitsAuto(pak);
    if(version > 23) {
            state->stats.sqlforeidleratio = pktGetF32(pak);
            state->stats.sqlbackidleratio = pktGetF32(pak);
    }
    if (version > 25) {
        state->stats.turnstileSecSinceUpdate = pktGetBitsAuto(pak);
    }
    // Version 27: added overload protection
    if (version >= 27) {
        state->stats.overloadProtection = pktGetBitsAuto(pak);
    } else {
        state->stats.overloadProtection = -1;
    }
    // Version 28: added total map start requests and delta map start requests since last update
    if (version >= 28) {
        int updated_map_start_request_total;
        int delta_requests;

        state->stats.dbserver_stat_time_delta        = pktGetBitsAuto(pak);
        updated_map_start_request_total                = pktGetBitsAuto(pak);
        state->stats.dbserver_peak_waiting_entities = pktGetBitsAuto(pak);

        delta_requests = updated_map_start_request_total - state->stats.dbserver_map_start_request_total;
        state->stats.dbserver_map_start_request_total = updated_map_start_request_total;
        if (state->stats.dbserver_stat_time_delta > 0)
        {
            state->stats.dbserver_avg_map_request_rate = (delta_requests*1000.0f)/state->stats.dbserver_stat_time_delta;
        }
    }
}

static char previous_patch_version[100];
static char previous_hostname[128];
static void updateBadVersionColors(ListView *lv, void *structptr, void *data)
{
    ServerMonitorState *state = data;
    char *patch_version;
    char *hostname;

    if (structptr == NULL)
    {
        return;
    }

    patch_version = ((MapCon*)structptr)->patch_version;
    hostname = ((MapCon*)structptr)->remote_process_info.hostname;

    if (previous_patch_version[0] != 0 &&
        patch_version[0] != 0 &&
        stricmp(previous_patch_version, patch_version) != 0)
    {
        char buf[4000];
        sprintf_s(buf, sizeof(buf), "Diverging versions found!\r\n%s: %s\r\n%s: %s\r\n\r\nHave all mapservers been updated?", previous_hostname, previous_patch_version, hostname, patch_version);
        updateCrashMsg(listViewGetListViewWindow(lv), buf);
    }

    if (previous_patch_version[0] == 0 && patch_version[0] != 0)
    {
        strcpy(previous_patch_version, patch_version);
        strcpy(previous_hostname, hostname);
    }

    if (patch_version[0] == 0 || stricmp(patch_version, state->stats.serverversion) == 0)
    {
        // leave current coloring, only override if it is bad version
//        listViewSetItemColor(lv, structptr, COLOR_CONNECTED);
    }
    else
    {
        listViewSetItemColor(lv, structptr, COLOR_BAD_VERSION);
    }
}

static void updateMapItemColors(ListView *lv, void *structptr, void *data)
{
    ServerMonitorState *state = data;
    MapCon*    map = (MapCon*)structptr;

    if (structptr == NULL)
    {
        return;
    }

    // default color
    listViewSetItemColor(lv, structptr, COLOR_CONNECTED);

    // color if it has stale launcher remote process info
    if (map->remote_process_info.seconds_since_update > 30)
    {
        listViewSetItemColor(lv, structptr, LISTVIEW_DEFAULT_COLOR, RGB(210,200,170));
    }

    // give priority override to 'bad version' coloring
    updateBadVersionColors(lv, structptr, data);
}

int svrMonHandleMsg(Packet *pak,int cmd, NetLink *link)
{
    ServerMonitorState *state = link->userData;
    if (!state) {
        assert(state);
        return 0;
    }
    timestamp = timerCpuTicks();
    switch (cmd) {
        case DBSVRMON_MAPSERVERS:
            //received_update = true;
            HandleRecvList(state, pak, state->eaMaps, MapConNetInfo, &state->tpiMapConNetInfo, sizeof(MapCon), state->lvMaps, state->eaMapsStuck, state->lvMapsStuck, stuckFilter, NULL, "Maps");
            updateInTroubleState(state);
            previous_patch_version[0] = 0;
            previous_hostname[0] = 0;
            listViewForEach(state->lvMaps, updateMapItemColors, state);
            break;
        case DBSVRMON_CRASHEDMAPSERVERS:
            //received_update = true;
            HandleRecvList(state, pak, state->eaMapsStuck, CrashedMapConNetInfo, &state->tpiCrashedMapConNetInfo, sizeof(MapCon), state->lvMapsStuck, NULL, NULL, NULL, NULL, "CrashedMaps");
            updateInTroubleState(state);
            break;
        case DBSVRMON_PLAYERS:
            //received_update = true;
            HandleRecvList(state, pak, state->eaEnts, EntConNetInfo, &state->tpiEntConNetInfo, sizeof(EntCon), NULL, NULL, state->lvEnts, smentsFilter, NULL, "Players");
            break;
        case DBSVRMON_LAUNCHERS:
            //received_update = true;
            HandleRecvList(state, pak, state->eaLaunchers, LauncherConNetInfo, &state->tpiLauncherConNetInfo, sizeof(LauncherCon), state->lvLaunchers, NULL, NULL, NULL, NULL, "Launchers");
            break;
        case DBSVRMON_SERVERAPPS:
            //received_update = true;
            HandleRecvList(state, pak, state->eaServerApps, ServerAppConNetInfo, &state->tpiServerAppConNetInfo, sizeof(ServerAppCon), state->lvServerApps, NULL, NULL, NULL, NULL, "ServerApps");
            updateInTroubleState(state);
            break;
        case DBSVRMON_REQUESTVERSION:
            strcpy(state->stats.gameversion, pktGetString(pak));
            strcpy(state->stats.serverversion, pktGetString(pak));
            break;
        case DBSVRMON_DBSTATS:
            handleDbStats(state,pak);
            break;
        case DBSVRMON_CONNECT:
            if (!pktGetBits(pak, 1)) {
                // Version check failed!
                int crc_num = pktGetBitsPack(pak, 1);
                U32 server_crc = pktGetBits(pak, 32);
                U32 my_crc = pktGetBits(pak, 32);
                char err_buf[1024];
                svrMonDisconnect(state);
                if (crc_num<=1) {
                    sprintf(err_buf, "Error connecting to DbServer, protocol version %d does not match:\n  Server: %d\n  Client: %d", crc_num, server_crc, my_crc);
                } else {
                    sprintf(err_buf, "Error connecting to DbServer, network parse table (%d) CRCs do not match:\n  Server: %08x\n  Client: %08x", crc_num, server_crc, my_crc);
                }
                MessageBoxA(NULL, err_buf, "Error", MB_ICONWARNING);
            } else {
                // Connected fine
//                if (!received_update) {
//                    svrMonRequest(state, DBSVRMON_REQUEST);
//                }
            }
            break;
        default:
            //assert(0);
            return 0;
    }
    return 1;
}

int svrMonNetTick(ServerMonitorState *state)
{
    lnkFlushAll();
    netLinkMonitor(&state->db_link, 0, svrMonHandleMsg);

    if (!svrMonConnectionLooksDead(state) && !state->svrMonNetTick_was_connected) {
        state->svrMonNetTick_was_connected = true;
        if (state->stats.servers_in_trouble==9999) {
            state->stats.servers_in_trouble = 0;
            state->servers_in_trouble_changed = 1;
        }
        state->stats.dbserver_in_trouble = 0;
    } else if (state->svrMonNetTick_was_connected && svrMonConnectionLooksDead(state)) {
        if (!state->stats.servers_in_trouble) {
            state->stats.servers_in_trouble = 9999;
        }
        state->servers_in_trouble_changed = 1;
        state->svrMonNetTick_was_connected = false;
        state->stats.dbserver_in_trouble = 1;
    }
    return 1;
}

void svrMonSendDbMessage(ServerMonitorState *state, const char *msg, const char *params)
{
    Packet *pak;
    if (!svrMonConnected(state) || !msg || !msg[0] || !params)
        return;
    pak = pktCreateEx(&state->db_link, DBSVRMON_RELAYMESSAGE);
    pktSendString(pak, msg);
    pktSendString(pak, params);
    pktSend(&pak, &state->db_link);
}

void svrMonSendAdminMessage(ServerMonitorState *state, const char *msg)
{
    if (!svrMonConnected(state) || !msg || !msg[0])
        return;
    svrMonSendDbMessage(state, "AdminChat", msg);
}

void svrMonSendOverloadProtection(ServerMonitorState *state, const char *msg)
{
    if (!svrMonConnected(state) || !msg || !msg[0])
        return;
    svrMonSendDbMessage(state, "OverloadProtection", msg);
}


void svrMonDelink(ServerMonitorState *state, MapCon *con)
{
    if (con) {
        char buf[256];
        svrMonSendDbMessage(state, "Delink", itoa(con->id, buf, 10));
        lnkBatchSend(&state->db_link);
    }
}

void smcbMsDelink(ListView* lv, void* structptr, void* stateData)
{
    ServerMonitorState * state = (ServerMonitorState *)stateData;
    MapCon *con = (MapCon*)structptr;
    svrMonDelink(state, con);
}

void smcbMsShow(ListView* lv, void* structptr, void* stateData)
{
    ServerMonitorState * state = (ServerMonitorState *)stateData;
    char temp[MAX_PATH];
    MapCon *con = (MapCon*)structptr;
    int cmd = (int)(intptr_t)state->userData;

    if (con) {
        Packet *pak = pktCreateEx(&state->db_link,DBSVRMON_EXEC);
        pktSendBits(pak, 32, con->link?con->link->addr.sin_addr.S_un.S_addr:con->ip_list[0]);
        sprintf(temp, "SHOW %d %d", con->remote_process_info.process_id, cmd);
        pktSendString(pak,temp);
        pktSend(&pak,&state->db_link);
/*        if (con->ip_list[0]!=con->ip_list[1] && con->ip_list[1]) {
            pktSendBits(pak, 32, con->ip_list[1]);
            sprintf(temp, "SHOW %d %d", con->remote_process_info.process_id, cmd);
            pktSendString(pak,temp);
            pktSend(&pak,&state->db_link);
        } */
        lnkBatchSend(&state->db_link);
    }
}

void killByIP(NetLink *link, U32 ip, U32 pid)
{
    char temp[MAX_PATH];
    Packet *pak = pktCreateEx(link,DBSVRMON_EXEC);
    pktSendBits(pak, 32, ip);
    sprintf(temp, "TASKKILL /F /PID %d", pid);
    pktSendString(pak,temp);
    pktSend(&pak,link);
    lnkBatchSend(link);
    // In case they don't have TASKKILL.EXE try plain old kill
    pak = pktCreateEx(link,DBSVRMON_EXEC);
    pktSendBits(pak, 32, ip);
    sprintf(temp, "KILL %d", pid);
    pktSendString(pak,temp);
    pktSend(&pak,link);
    lnkBatchSend(link);
}

void smcbMsKill(ListView* lv, void* structptr, void* stateData)
{
    ServerMonitorState * state = (ServerMonitorState *)stateData;
    MapCon *con = (MapCon*)structptr;
    if (con) {
        killByIP(&state->db_link, con->link?con->link->addr.sin_addr.S_un.S_addr:con->ip_list[0], con->remote_process_info.process_id);
        strcpy(con->status, "Killed");
        listViewItemChanged(lv, structptr);
    }
}

void smcbMsRemoteDebug(ListView* lv, void* structptr, void* stateData)
{
    ServerMonitorState * state = (ServerMonitorState *)stateData;
    MapCon *con = (MapCon*)structptr;
    if (con) {
        char buf[MAX_PATH];
        sprintf(buf, "RemoteDebuggerer -host %s", makeIpStr(con->ip_list[0]));
        printf("%s\n", buf);
        if (!executeServer(buf, 0)) {
            sprintf(buf, ".\\src\\util\\RemoteDebuggerer -host %s", makeIpStr(con->ip_list[0]));
            printf("%s\n", buf);
            system_detach(buf, 0);
        }
    }
}

typedef BOOL(__stdcall * DisableFsRedirectionProc)(PVOID*);
typedef BOOL(__stdcall * RevertFsRedirectionProc)(PVOID);

void launchRemoteDesktop(U32 ip, const char *name)
{
    void * oldFsRedirection;
    HMODULE kernel32_dll;
    DisableFsRedirectionProc disableFsRedirectionFunc = NULL;
    RevertFsRedirectionProc revertFsRedirectionFunc = NULL;

    char *rdptemplate =
        "screen mode id:i:2\n"
        "auto connect:i:1\n"
        "full address:s:%s\n"
        "compression:i:1\n"
        "keyboardhook:i:2\n"
        "displayconnectionbar:i:1\n"
        "autoreconnection enabled:i:1\n"
        "username:s:cohadmin\n"
        "domain:s:%s\n"
        "password 51:b:01000000D08C9DDF0115D1118C7A00C04FC297EB010000000D16AEAA1A476C49ADE95622AFA915010000000008000000700073007700000003660000A800000010000000EC580556CF66A3C4E30E5F2A779EE4660000000004800000A000000010000000EF98EB1B3DB7EE57D35BA5BD71F6FDA308020000A02FB43152E5CFBA675A06F84D4A14062215F7924D646BDCD136F455238F11B4FE16231731960709D81850A082BA463C2A98C72A12E829C06E7E58E396BF7AA57A03D63F39BCC7A3598B5F6BC0401AEA39332BD894CAEAFE2B49B2469070A8117D8813E60EE21AA60302502408A1D7BD108A0E4E7FE521DD6EE7B557DE8115E3222171752BB96430B8BBD1545AE186E3AF83CF3A20E97E8B1131E6EF6E567B411BC9F11D1E0A8E83D67E118F1C3D2DBF4226D63E4B72539EE808378189EABFC0A62BFF5824DE34A92DA70177BEF454CC14AAB77453144AA75F4A1722A0BF40C03A83175189BF7C19B1CD42C19973E0523FAA94B4B527E98CC9ACDE8555BF5CC2618A176FCA23514F4861A561BEF4ECEE395F10C52E038F278E3D0255D76A6A672F2E3C37F19D71EC1047EED5792B3B9DE91A5ED6B2192D39B755E7E6673512B6E6D0479844258213096F61070894599B1C90D7C2E75B835E076410EBD26ECBC345A942753581B5F6A72603AEE65B79460F16F21DA384BA42E17962F8CEFC90B48B13D9FB4A87E6F24366CC9B8CFE297BDB64BC6CBEA449A5C0E25A82A5CD47986DEDEC77A69731B2C88DEA196CB5D8101B8C205B35200A6EB28244962D22E8B0BC45E2FA755E39C9C82B6DA961EBD020A0D63F51FD83439B2BF90BBBCDA6D36EC58294929A81173FA9D431028773E5D7341E64F24F563DB0DAB79E4BF1B6C00951BAEB012D353865140000006B8E265690E67FA1AE8F1677FD5A1015AA05CDC50\n"
        "disable full window drag:i:1\n"
        "disable menu anims:i:1\n";
    char *rdptemplateKR =
        "screen mode id:i:2\n"
        "auto connect:i:1\n"
        "full address:s:%s:69\n"
        "compression:i:1\n"
        "keyboardhook:i:2\n"
        "displayconnectionbar:i:1\n"
        "autoreconnection enabled:i:1\n"
        "username:s:cohmaster\n"
        "domain:s:%s\n"
        "password 51:b:01000000D08C9DDF0115D1118C7A00C04FC297EB010000000D16AEAA1A476C49ADE95622AFA915010000000008000000700073007700000003660000A800000010000000EC580556CF66A3C4E30E5F2A779EE4660000000004800000A000000010000000EF98EB1B3DB7EE57D35BA5BD71F6FDA308020000A02FB43152E5CFBA675A06F84D4A14062215F7924D646BDCD136F455238F11B4FE16231731960709D81850A082BA463C2A98C72A12E829C06E7E58E396BF7AA57A03D63F39BCC7A3598B5F6BC0401AEA39332BD894CAEAFE2B49B2469070A8117D8813E60EE21AA60302502408A1D7BD108A0E4E7FE521DD6EE7B557DE8115E3222171752BB96430B8BBD1545AE186E3AF83CF3A20E97E8B1131E6EF6E567B411BC9F11D1E0A8E83D67E118F1C3D2DBF4226D63E4B72539EE808378189EABFC0A62BFF5824DE34A92DA70177BEF454CC14AAB77453144AA75F4A1722A0BF40C03A83175189BF7C19B1CD42C19973E0523FAA94B4B527E98CC9ACDE8555BF5CC2618A176FCA23514F4861A561BEF4ECEE395F10C52E038F278E3D0255D76A6A672F2E3C37F19D71EC1047EED5792B3B9DE91A5ED6B2192D39B755E7E6673512B6E6D0479844258213096F61070894599B1C90D7C2E75B835E076410EBD26ECBC345A942753581B5F6A72603AEE65B79460F16F21DA384BA42E17962F8CEFC90B48B13D9FB4A87E6F24366CC9B8CFE297BDB64BC6CBEA449A5C0E25A82A5CD47986DEDEC77A69731B2C88DEA196CB5D8101B8C205B35200A6EB28244962D22E8B0BC45E2FA755E39C9C82B6DA961EBD020A0D63F51FD83439B2BF90BBBCDA6D36EC58294929A81173FA9D431028773E5D7341E64F24F563DB0DAB79E4BF1B6C00951BAEB012D353865140000006B8E265690E67FA1AE8F1677FD5A1015AA05CDC50\n"
        "disable full window drag:i:1\n"
        "disable menu anims:i:1\n";

    char cmd[MAX_PATH];
    char cwd[MAX_PATH];
    char tempFileName[MAX_PATH];
    char *templateFileName="c:\\temp\\ServerMonitorTemplate.rdp";
    char *templateFileNameDE="c:\\temp\\ServerMonitorTemplateDE.rdp";
    char *templateFileNameAUS="c:\\temp\\ServerMonitorTemplateAUS.rdp";
    char *templateFileNameKR="c:\\temp\\ServerMonitorTemplateKR.rdp";
    char *domain = "NCLA";
    FILE *fout;
    bool isDE = (((ip & 0x0000ff00) >> 8) == 128) || strStartsWith(name, "cohde");
    bool isAUS = ((ip & 0x00ffffff)  == 1253292) || //172.31.19.*
        (((ip & 0x00ff0000) >> 16) == 22); // 172.31.22.*
    bool isKR = ((ip & 0xff) == 10) && (((ip & 0xff00) >> 8) == 50);

    if (isDE)
        templateFileName = templateFileNameDE;
    if (isAUS)
        templateFileName = templateFileNameAUS;
    if (isKR) {
        templateFileName = templateFileNameKR;
        rdptemplate = rdptemplateKR;
    }

    sprintf(tempFileName, "c:\\temp\\%s.rdp", name);
    while (strchr(tempFileName, ' ')) {
        char *s = strchr(tempFileName, ' ');
        strcpy(s, s+1);
    }

    mkdirtree(tempFileName);

    if (fileExists(templateFileName)) {
        int total = fileSize(templateFileName);
        FILE *file = fopen(templateFileName,"r");
        if (file) {
            rdptemplate = calloc(total+1, 1);
            fread(rdptemplate,1,total,file);
            fclose(file);
        }
    } else {
        fout = fopen(templateFileName, "w");
        if (fout) {
            fprintf(fout, "%s", rdptemplate);
            fclose(fout);
        }
        rdptemplate = strdup(rdptemplate);
    }

    fout = fopen(tempFileName, "w");
    if (!fout) {
        free(rdptemplate);
        return;
    }

    if (isDE) {
        domain = "NCDE";
    } else if (isAUS) {
        domain = "NCAUSTIN";
    } else if (isKR) {
        domain = "GAMENCSOFT";
    } else if (((ip & 0x00ff0000) >> 16) == 136) {
        domain = "NCASHF";
    } else if (((ip & 0x00ff0000) >> 16) == 134) {
        domain = "NCASHF";
    } else if (((ip & 0x00ff0000) >> 16) == 132) {
        domain = "NCDC";
    } else if (((ip & 0x00ff0000) >> 16) == 186) {
        domain = "NCASHF";
    } else if (((ip & 0x0000ff00) >> 8) == 30 &&    // 172.30.x
        ((ip & 0x00ff0000) >> 16) == 130 ||
        ((ip & 0x00ff0000) >> 16) == 131 ||
        ((ip & 0x00ff0000) >> 16) == 132 ||
        ((ip & 0x00ff0000) >> 16) == 133)
    {
        domain = "NCDC";
    } else if (((ip & 0x00ff0000) >> 16) == 130) {
        domain = "NCASHF";
    } else if (((ip & 0x00ff0000) >> 16) == 128) {
        domain = "NCDC";
    } else if (((ip & 0x00ff0000) >> 16) == 131) { // 172.31.131 = NCLAX?
        domain = "NCLAX";
    } else if (((ip & 0x00ff0000) >> 16) == 145) {
        domain = "NCLA";
    }

    fprintf(fout, rdptemplate, makeIpStr(ip), domain);
    fclose(fout);

    sprintf(cmd, "mstsc.exe %s", tempFileName);

    _getcwd(cwd, ARRAY_SIZE(cwd));
    _chdir("\\");

    // see if the x64 file system redirection functions exist
    kernel32_dll = LoadLibraryA( "Kernel32.dll" );
    disableFsRedirectionFunc = (DisableFsRedirectionProc) GetProcAddress( kernel32_dll, "Wow64DisableWow64FsRedirection" );
    revertFsRedirectionFunc = (RevertFsRedirectionProc) GetProcAddress( kernel32_dll, "Wow64RevertWow64FsRedirection" );
    // 64 bit machines must disable the file system redirection in order to call mstsc.exe from the system32 directory
    if ( disableFsRedirectionFunc && revertFsRedirectionFunc )
    {
        disableFsRedirectionFunc(&oldFsRedirection);
        ShellExecuteA(NULL, "open", "C:/WINDOWS/system32/mstsc.exe", tempFileName, NULL, SW_SHOW);
        revertFsRedirectionFunc(oldFsRedirection);
    }
    else
    {
        system_detach(cmd, 0);
    }
    _chdir(cwd);
    //system_detach(cmd, 0);
    free(rdptemplate);
}

static int strdiff(const char *str1, const char *str2)
{
    int ret=0;
    const char *s1, *s2;
    bool innumber=false;
    if (strlen(str1)!=strlen(str2))
        return 999;
    for (s1=str1, s2=str2; *s1; s1++, s2++) {
        if (*s1!=*s2) {
            int diff = *s1 - *s2;
            ret *= 10;
            ret += diff;
            innumber=true;
        } else if (isdigit((unsigned char)*s1) && innumber) {
            ret *= 10;
        } else {
            innumber = false;
        }
    }
    return ret;
}

void smcbMsRemoteDesktop(ListView* lv, void* structptr, void* stateData)
{
    ServerMonitorState * state = (ServerMonitorState *)stateData;
    MapCon *con = (MapCon*)structptr;
    if (con) {
        U32 ip;
        if ((con->ip_list[0] & 0xff) == 172) {
            ip = con->ip_list[0];
        } else if ((con->ip_list[1] & 0xff) == 172) {
            ip = con->ip_list[1];
        } else {
            ip = con->ip_list[0];
        }
        if (!ip) {
            // Search launchers for this name
            int i;
            for (i=0; i<eaSize(state->eaLaunchers); i++) {
                LauncherCon *launcher = eaGet(state->eaLaunchers, i);
                int diff = strdiff(con->remote_process_info.hostname, launcher->remote_process_info.hostname);
                if (diff==0)
                {
                    if (launcher->link) {
                        ip = launcher->link->addr.sin_addr.S_un.S_addr;
                        break;
                    }
                }
                if (ABS(diff) == 1) // We're *close* to the right name, extrapolate!
                {
                    U32 lowbyte;
                    U32 newip;
                    newip = launcher->link->addr.sin_addr.S_un.S_addr;
                    lowbyte = newip >> 24;
                    lowbyte += diff;
                    newip = (newip & 0x00FFFFFF) | lowbyte << 24;
                    assert(ip==0 || newip == ip); // Otherwise this hack isn't valid
                    ip = newip;
                }
            }
        }
        launchRemoteDesktop(ip, con->remote_process_info.hostname);
    }
}

void smcbSaKill(ListView* lv, void* structptr, void* stateData)
{
    ServerMonitorState * state = (ServerMonitorState *)stateData;
    ServerAppCon *con = (ServerAppCon*)structptr;
    if (con) {
        killByIP(&state->db_link, con->ip, con->remote_process_info.process_id);
        strcpy(con->status, "Killed");
        listViewItemChanged(lv, structptr);
    }
}


void smcbSaRemoteDesktop(ListView* lv, void* structptr, void* stateData)
{
    ServerMonitorState * state = (ServerMonitorState *)stateData;
    ServerAppCon *con = (ServerAppCon*)structptr;
    if (con) {
        launchRemoteDesktop(con->ip, con->remote_process_info.hostname);
    }
}

void smcbLRemoteDesktop(ListView* lv, void* structptr, void* stateData)
{
    ServerMonitorState * state = (ServerMonitorState *)stateData;
    LauncherCon *con = (LauncherCon*)structptr;
    if (con && con->link) {
        launchRemoteDesktop(con->link->addr.sin_addr.S_un.S_addr, con->remote_process_info.hostname);
    }
}

void smcbLauncherSuspend(ListView* lv, void* structptr, void* stateData)
{
    ServerMonitorState * state = (ServerMonitorState *)stateData;
    LauncherCon *con = (LauncherCon*)structptr;
    if (con && con->link) {
        // we don't suspend 'monitor only' launchers because they can't launch
        if ((con->flags & kLauncherFlag_Monitor) == 0)
        {
            char* ipStr = makeIpStr(con->link->addr.sin_addr.S_un.S_addr);
            svrMonSendDbMessage(state, "SuspendLauncher", ipStr);
        }
    }
}

void smcbLauncherResume(ListView* lv, void* structptr, void* stateData)
{
    ServerMonitorState * state = (ServerMonitorState *)stateData;
    LauncherCon *con = (LauncherCon*)structptr;
    if (con && con->link) {
        char* ipStr = makeIpStr(con->link->addr.sin_addr.S_un.S_addr);
        svrMonSendDbMessage(state, "ResumeLauncher", ipStr);
    }
}

void smcbMsViewError(ListView* lv, void* structptr, void* stateData)
{
    ServerMonitorState * state = (ServerMonitorState *)stateData;
    MapCon *con = (MapCon*)structptr;
    if (con && con->raw_data) {
        char    buf[1000];
        FILE *f;
        sprintf(buf,"%s/console.txt",fileTempDir());
        mkdirtree(buf);
        f = fopen(buf, "wt");
        if (f) {
            fwrite(con->raw_data, 1, strlen(con->raw_data), f);
            fclose(f);
            fileOpenWithEditor(buf);
            Sleep(250);
        }
    }
}

void svrMonLogHistory(ServerMonitorState *state)
{
    static int timer = 0; // Shared between all instances!
    F32 elapsed;
    if (timer==0) {
        timer = timerAlloc();
    }

    // Snapshot global stats
    if (svrMonConnected(state)) {
        shcUpdate(&state->shcServerMonHistory, &state->stats, ServerStatsLogInfoServerMon, timerCpuTicks(), "Global");
    }

    elapsed = timerElapsed(timer);
    if (elapsed > state->svrMonLogHistory_lasttick + state->log_interval) {
        state->svrMonLogHistory_lasttick += state->log_interval;
        if (elapsed > state->svrMonLogHistory_lasttick + state->log_interval) {
            // We're more than a tick behind! Skip 'em!
            state->svrMonLogHistory_lasttick = elapsed;
        }
        // do logging
        shcDump(state->shcServerMonHistory, timerCpuTicks());
    }
}
