#include <comm_backend.h>
#include <container/containerbroadcast.h>
#include <entity/entVarUpdate.h>
#include <svrmoncomm.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/ListView.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/structHist.h>
#include <utilitieslib/utils/structNet.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/WinTabUtil.h>
#include <utilitieslib/utils/winutil.h>

#include "processMonitor.h"
#include "prompt.h"
#include "resource.h"
#include "serverMonitor.h"
#include "serverMonitorCommon.h"
#include "serverMonitorCrashMsg.h"
#include "serverMonitorNet.h"
#include "serverMonitorOverload.h"
#include "shardMonitor.h"
#include "shardMonitorCmdRelay.h"
#include "shardMonitorComm.h"
#include "shardMonitorConfigure.h"

#define maxtps 10
#define DEFAULT_RECONNECT_TIME maxtps*30
#define RECONNECT_UPDATE_DELAY maxtps*2

extern bool g_someDataOutOfSync;
char trouble_reason[8192];

ServerStats **eaServerStats_data=NULL, ***eaServerStats = &eaServerStats_data;
ServerStats **eaServerStatsSum_data=NULL, ***eaServerStatsSum = &eaServerStatsSum_data;
ListView *lvSmShards1=NULL, *lvSmShards2=NULL, *lvShardRelays=NULL;

StructHistCollection shcShardMonHistory=NULL;

static bool shardMonitorBusy=false;

void destroyServerStats(ServerStats *stats) { 
    sdFreeParseInfo(stats->tpiServerStatsNetInfo);
    free(stats);
}
ServerStats *createServerStats(void) { return calloc(sizeof(ServerStats), 1); }

static bool smLogEnabled=0;
static F32 smLogInterval=2;
static bool smFilterLongTick=0;
static bool smFilterServerApps=0;

static VarMap mapping[] = {
    {IDC_CHK_LOG,                VMF_READ, "ShardMonLogging",     TOK_BOOL_X, (size_t)&smLogEnabled, },
    {IDC_TXT_LOGINTERVAL,        VMF_READ, "ShardMonLogInterval", TOK_F32_X,  (size_t)&smLogInterval, },
    {IDC_CHK_LONG_TICK,          VMF_READ, NULL,                  TOK_BOOL_X, (size_t)&smFilterLongTick, },
    {IDC_CHK_CRASHED_SERVERAPPS, VMF_READ, NULL,                  TOK_BOOL_X, (size_t)&smFilterServerApps, },
};

ServerStats zeroServerStat = {0};

enum {
    STAT_MIN,
    STAT_MAX,
    STAT_AVG,
    STAT_TOT,
};

static StructOp ops[] = { // What op to perform on each of the above stats
    STRUCTOP_MIN,
    STRUCTOP_MAX,
    STRUCTOP_ADD,
    STRUCTOP_ADD,
};

char *special_rows[] = {"MINIMUM","MAXIMUM","AVERAGE","TOTAL"};

static void destroyServerStatsAdapter(void* arg0)
{
    destroyServerStats((ServerStats *)arg0);
}

void shardMonInit()
{
    int i;
    shardMonLoadConfig("./ShardMonConfig.txt");
    listViewDelAllItems(lvSmShards1, NULL);
    listViewDelAllItems(lvSmShards2, NULL);
    eaClearEx(eaServerStats, destroyServerStatsAdapter);
    eaClearEx(eaServerStatsSum, destroyServerStatsAdapter);
    for (i=0; i<eaSize(&shmConfig.shardList); i++) {
        ServerStats *stat = createServerStats();
        eaPush(eaServerStats, stat);
        strcpy(stat->name, shmConfig.shardList[i]->name);
        stat->ip = shmConfig.shardList[i]->ip;
        strcpy(stat->status, "Not Connected");
        stat->connected = 0;
        listViewAddItem(lvSmShards1, stat);
        listViewAddItem(lvSmShards2, stat);
        if(lvShardRelays)    
            listViewAddItem(lvShardRelays, stat);
        listViewSetItemColor(lvSmShards1, stat, COLOR_DISABLED);
        listViewSetItemColor(lvSmShards2, stat, COLOR_DISABLED);
    }
    listViewAddItem(lvSmShards1, LISTVIEW_EMPTY_ITEM);
    listViewAddItem(lvSmShards2, LISTVIEW_EMPTY_ITEM);
    listViewSetItemColor(lvSmShards1, LISTVIEW_EMPTY_ITEM, COLOR_DIVIDER);
    listViewSetItemColor(lvSmShards2, LISTVIEW_EMPTY_ITEM, COLOR_DIVIDER);
    for (i=0; i<ARRAY_SIZE(special_rows); i++) {
        ServerStats *stat = createServerStats();
        eaPush(eaServerStatsSum, stat);
        strcpy(stat->name, special_rows[i]);
        strcpy(stat->status, "");
        stat->special = true;    // denote non-shard rows
        listViewAddItem(lvSmShards1, stat);
        listViewAddItem(lvSmShards2, stat);
        listViewSetItemColor(lvSmShards1, stat, COLOR_STATS);
        listViewSetItemColor(lvSmShards2, stat, COLOR_STATS);
    }
    if (!shcShardMonHistory) {
        shcShardMonHistory = createStructHistCollection("ShardMonLogs");
    }
}

void shardMonDisconnect(ServerStats *stat)
{
    clearNetLink(&stat->link);
    strcpy(stat->status, "Not Connected");
    stat->connected = 0;
    stat->reconnect_countdown = 0;
    listViewSetItemColor(lvSmShards1, stat, COLOR_DISABLED);
    listViewSetItemColor(lvSmShards2, stat, COLOR_DISABLED);
    listViewItemChanged(lvSmShards1, stat);
}

void shardMonDisconnectAll()
{
    int i;
    if (shardMonitorBusy)
        return;
    shardMonitorBusy = true;
    for (i=0; i<eaSize(eaServerStats); i++) {
        ServerStats *stat = eaGet(eaServerStats, i);
        shardMonDisconnect(stat);
    }
    shardMonitorBusy = false;
}

static ServerStats *g_temp_stat=NULL;
static void idleCallback(F32 timeLeft)
{
    NMMonitor(1);
    sprintf(g_temp_stat->status, "Connecting (%1.1fs)...", timeLeft);
    listViewItemChanged(lvSmShards1, g_temp_stat);
    serverMonitorMessageLoop();
}

int shardMonHandleMsg(Packet *pak,int cmd, NetLink *link)
{
    ServerStats *stat = (ServerStats*)link->userData;
    if (!stat)
        return 0;
    switch (cmd) {
        case SVRMONSHARDMON_CONNECT:
            if (!pktGetBits(pak, 1)) {
                // Version check failed!
                int crc_num = pktGetBitsPack(pak, 1);
                U32 server_crc = pktGetBits(pak, 32);
                U32 my_crc = pktGetBits(pak, 32);
                shardMonDisconnect(stat);
                if (crc_num<=1) {
                    sprintf(stat->status, "ProtocolErr %d: S:%d C:%d", crc_num, server_crc, my_crc);
                } else {
                    sprintf(stat->status, "ProtocolErr(%d): S:%08x C:%08x", crc_num, server_crc, my_crc);
                }                
            } else {
                // Connected fine
                strcpy(stat->status, "CONNECTED");
                listViewSetItemColor(lvSmShards1, stat, COLOR_CONNECTED);
                listViewSetItemColor(lvSmShards2, stat, COLOR_CONNECTED);
                stat->connected = 1;
            }
            listViewItemChanged(lvSmShards1, stat);
            break;
        case SVRMONSHARDMON_STATS:
            {
                int full_update = pktGetBits(pak, 1);
                if (full_update) {
                    stat->tpiServerStatsNetInfo = sdUnpackParseInfo(ServerStatsNetInfo, pak, false);
                }
                g_someDataOutOfSync |= !sdUnpackDiff(stat->tpiServerStatsNetInfo, pak, stat, NULL, false);
                listViewItemChanged(lvSmShards2, stat);
                if(lvShardRelays)
                    listViewItemChanged(lvShardRelays, stat);
                shcUpdate(&shcShardMonHistory, stat, ServerStatsLogInfoShardMon, timerCpuTicks(), "Shards");
            }
            break;
    }
    return 1;
}


int shardMonConnectSingle(ServerStats *stat)
{
    int ret=0;
    if (shardMonitorBusy)
        return 0;
    shardMonitorBusy = true;
    g_temp_stat = stat;
    strcpy(stat->status, "Connecting...");
    listViewItemChanged(lvSmShards1, stat);
    if (netConnect(&stat->link, makeIpStr(stat->ip), DEFAULT_SHARDMON_PORT, NLT_TCP, 5.f,idleCallback)) {
        NMAddLink(&stat->link, shardMonHandleMsg);
        stat->link.userData = stat;
        {
            Packet *pak = pktCreateEx(&stat->link, SVRMONSHARDMON_CONNECT);
            pktSendBits(pak, 32, 20041006); // For backwards compatibility
            pktSendBits(pak, 32, SVRMON_PROTOCOL_MAJOR_VERSION);
            pktSend(&pak, &stat->link);
        }
        strcpy(stat->status, "Negotiating...");
        listViewSetItemColor(lvSmShards1, stat, COLOR_RECONNECT);
        listViewSetItemColor(lvSmShards2, stat, COLOR_RECONNECT);
        listViewItemChanged(lvSmShards1, stat);
        g_temp_stat = NULL;
        ret = 1;
    } else {
        strcpy(stat->status, "FAILED Connection");
        listViewSetItemColor(lvSmShards1, stat, COLOR_TROUBLE);
        listViewSetItemColor(lvSmShards2, stat, COLOR_TROUBLE);
        listViewItemChanged(lvSmShards1, stat);
        g_temp_stat = NULL;
    }
    shardMonitorBusy = false;
    return ret;
}

void shardMonConnectAll()
{
    int i;
    for (i=0; i<eaSize(eaServerStats); i++) {
        ServerStats *stat = eaGet(eaServerStats, i);
        if (!stat->link.connected) {
            if (!shardMonConnectSingle(stat)) {
                stat->reconnect_countdown = DEFAULT_RECONNECT_TIME;
            }
        }
        if (serverMonitorExiting())
            break;
    }
}

static void connectSingleWrapper(ListView *lv, ServerStats *stat, void *junk)
{
    if (!stat->link.connected) {
        if (!shardMonConnectSingle(stat)) {
            stat->reconnect_countdown = DEFAULT_RECONNECT_TIME;
        }
    }
}
static void disconnectSingleWrapper(ListView *lv, ServerStats *stat, void *junk)
{
    shardMonDisconnect(stat);
}

void shardMonConnectSingleWrapper()
{
    listViewDoOnSelected(lvSmShards1, connectSingleWrapper, NULL);
}
void shardMonDisconnectSingleWrapper()
{
    listViewDoOnSelected(lvSmShards1, disconnectSingleWrapper, NULL);
}

static void sendMessage(ListView *lv, ServerStats *stat, char *msg)
{
    Packet *pak;
    pak = pktCreateEx(&stat->link,SVRMONSHARDMON_RELAYMESSAGE);
    // Make payload packet
    pktSendString(pak, "AdminChat");
    pktSendString(pak, msg);
    pktSend(&pak,&stat->link);
}

static void sendLauncherReconcilation(ListView *lv, ServerStats *stat, char *launcherIP)
{
    Packet *pak;
    pak = pktCreateEx(&stat->link,SVRMONSHARDMON_RECONCILE_LAUNCHER);
    // Make payload packet
    pktSendString(pak, "ReconcileLauncher");
    pktSendString(pak, launcherIP);
    pktSend(&pak,&stat->link);
}

static void sendOverloadProtection(ListView *lv, ServerStats *stat, char *msg)
{
    Packet *pak;
    pak = pktCreateEx(&stat->link,SVRMONSHARDMON_RELAYMESSAGE);
    // Make payload packet
    pktSendString(pak, "OverloadProtection");
    pktSendString(pak, msg);
    pktSend(&pak,&stat->link);
}

void shardMonSendAdminMessage(HWND hDlg)
{
    char *msg = promptGetString(g_hInst, hDlg, "Admin message:", "");
    if (msg && msg[0]) {
        listViewDoOnSelected(lvSmShards1, sendMessage, msg);
    }
}

void shardMonReconcileLauncher(HWND hDlg)
{
    char *msg = promptGetString(g_hInst, hDlg, "Launcher IP:", "");
    if (msg && msg[0]) {
        listViewDoOnSelected(lvSmShards1, sendLauncherReconcilation, msg);
    }
}

void shardMonSendOverloadProtection(HWND hDlg, bool enable)
{
    char *msg = enable ? "1" : "0";
    listViewDoOnSelected(lvSmShards1, sendOverloadProtection, msg);
}

static void targettedServerMonitor(ListView *lv, ServerStats *stat, HWND hDlg)
{
    if (!TabDlgActivateTabIfAvailable(NULL, stat->name)) {
        strcpy(g_autoconnect_addr, makeIpStr(stat->ip));
        TabDlgInsertTab(g_hInst, NULL, IDD_DLG_SVRMON, DlgSvrMonProc, stat->name, true, 0);        
    }
}

void shardMonTargettedServerMonitor(HWND hDlg)
{
    listViewDoOnSelected(lvSmShards1, targettedServerMonitor, hDlg);
    
    // hack to make new tabs start at the correct size
    {
        extern HWND g_hDlg;
        SendMessage(g_hDlg, WM_SIZE, SIZE_RESTORED, 0);
    }
}

static void targettedRemoteDesktop(ListView *lv, ServerStats *stat, HWND hDlg)
{
    launchRemoteDesktop(stat->ip, stat->name);
}

void shardMonTargettedRemoteDesktop(HWND hDlg)
{
    listViewDoOnSelected(lvSmShards1, targettedRemoteDesktop, hDlg);
}

static void shardMonLogHistory(F32 interval)
{
    static int timer=0;
    static F32 lasttick=0;
    F32 elapsed;
    if (timer==0) {
        timer = timerAlloc();
    }

    // Snapshot global stats
    //shcUpdate(&shcHistory, &g_stats, ServerStatsLogInfo, timerCpuTicks(), "Global");

    elapsed = timerElapsed(timer);
    if (elapsed > lasttick + interval) {
        lasttick += interval;
        if (elapsed > lasttick + interval) {
            // We're more than a tick behind! Skip 'em!
            lasttick = elapsed;
        }
        // do logging
        shcDump(shcShardMonHistory, timerCpuTicks());
    }
}


static void checkLogHistory(HWND hDlg)
{
    static bool enabled=false;
    static int last_known_autodelink=300;

    if (smLogEnabled) {

        shardMonLogHistory(smLogInterval);

        if (!enabled) {
            EnableWindow(GetDlgItem(hDlg, IDC_TXT_LOGINTERVAL), TRUE);
            SetDlgItemInt(hDlg, IDC_TXT_LOGINTERVAL, smLogInterval, TRUE);
            enabled = true;
        }
    } else {
        if (enabled) {
            EnableWindow(GetDlgItem(hDlg, IDC_TXT_LOGINTERVAL), FALSE);
            enabled=false;
        }
    }
}

static bool badState(int status)
{
    return (status == PMS_CRASHED || status == PMS_NOTRUNNING);
}

static bool troubledStat(ServerStats *stat, char *reason, int *trouble_type)
{
    // NOTE: NEED TO USE \r\n
    bool trouble=false;
#define TROUBLE(x) *trouble_type = MAX(*trouble_type, x)

    *trouble_type = 0;

    if (eaFind(eaServerStatsSum, stat)!=-1) {
        char *cursor = trouble_reason;
        int i;
        for (i=0; i<eaSize(eaServerStats); i++)
        {
            ServerStats *stat = eaGet(eaServerStats, i);
            int len;
            int tt;
            trouble |= troubledStat(stat, cursor, &tt);
            TROUBLE(tt);
            len = (int)strlen(cursor);
            if (len) {
                strcat(cursor, "-----------------------------------\r\n");
            }
            cursor = cursor + strlen(cursor);
        }
        return trouble;
    }

    if (stat == (void*)(intptr_t)-1 || !stat) {
        strcpy(reason, "");
        return false;
    }
    sprintf(reason, "%s\r\n%s\r\n\r\n", stat->name, makeIpStr(stat->ip));
    if (stat->dbserver_in_trouble) {
        trouble = true;
        TROUBLE(10);
        strcatf(reason, "DbServer in trouble (not responding or not connected)\r\n");
    }
    if (stat->servers_in_trouble) {
        bool foundtrouble = false;
        if (stat->sms_stuck_count) {
            if (stat->autodelink) {
                strcatf(reason, "%d STUCK MapServers (will be auto-delinked in %d seconds)\r\n", stat->sms_stuck_count, stat->autodelinktime );
            } else {
                strcatf(reason, "%d STUCK MapServers (will NOT be auto-delinked - manual intervention required)\r\n", stat->sms_stuck_count );
            }
            TROUBLE(7);
            trouble = true;
            foundtrouble = true;
        }
        if (stat->sms_long_tick_count) {
            if (!smFilterLongTick) {
                strcatf(reason, "%d LONG TICK MapServers (Cryptic investigation required if problem persists)\r\n", stat->sms_long_tick_count); 
                trouble = true;
                TROUBLE(1);
            }
            foundtrouble = true;
        }
        if (stat->sms_stuck_starting_count) {
            //if (stat->autodelink) {
            //    strcatf(reason, "%d STUCK STARTING MapServers (will be auto-delinked in %d seconds)\r\n", stat->sms_stuck_starting_count, stat->autodelinktime );
            //    TROUBLE(9);
            //} else {
                strcatf(reason, "%d STUCK STARTING MapServers (will NOT be auto-delinked - manual intervention required)\r\n", stat->sms_stuck_starting_count );
                TROUBLE(10);
            //}
            trouble = true;
            foundtrouble = true;
        }
        if (stat->sa_crashed_count) {
            if (!smFilterServerApps) {
                strcatf(reason, "%d CRASHED SererApps (Cryptic investigation required)\r\n", stat->sa_crashed_count);
                TROUBLE(8);
                trouble = true;
            }
            foundtrouble = true;
        }
        if (!foundtrouble) {
            // Connected to old ServerMonitor
            strcatf(reason, "MapServer Trouble (%d of maps STUCK, maps LONG TICK, or ServerApps CRASHED)\r\n", stat->servers_in_trouble);
            TROUBLE(9);
            trouble = true;
        }
    }
    if (stat->pcount_connecting > 25) {
        trouble = true;
        TROUBLE(10);
        strcatf(reason, "%d players transferring.  Probable network configuration error.\r\n", stat->pcount_connecting);
    }
    if (stat->chatserver_in_trouble) {
        trouble = true;
        TROUBLE(10);
        strcatf(reason, "ChatServer in trouble\r\n");
    }
    if (stat->arenaserver_in_trouble) {
        trouble = true;
        TROUBLE(10);
        strcatf(reason, "ArenaServer in trouble\r\n");
    }
    if (stat->arenaSecSinceUpdate > 20) {
        trouble = true;
        TROUBLE(10);
        strcatf(reason, "ArenaServer has not responded for %d seconds\r\n", stat->arenaSecSinceUpdate);
    }
//     if (stat->beaconWaitSeconds > 120) {
//         trouble = true;
//         TROUBLE(10);
//         strcatf(reason, "Beacon wait time is %d seconds\r\n", stat->beaconWaitSeconds);
//     }
    if (stat->dbServerMonitor && badState(stat->dbServerMonitor->status)) {
        trouble = true;
        TROUBLE(10);
        strcatf(reason, "DbServer.exe crashed or not running\r\n");
    }
    if (stat->heroAuctionSecSinceUpdate > 120) {
        trouble = true;
        TROUBLE(10);
        strcatf(reason, "HeroAuctionServer has not responded for %d seconds\r\n", stat->heroAuctionSecSinceUpdate);
    }
    if (stat->villainAuctionSecSinceUpdate > 120) {
        trouble = true;
        TROUBLE(10);
        strcatf(reason, "VillainauctionServer has not responded for %d seconds\r\n", stat->villainAuctionSecSinceUpdate);
    }
    if (stat->accountSecSinceUpdate > 120) {
        trouble = true;
        TROUBLE(10);
        strcatf(reason, "AccountServer has not responded for %d seconds\r\n", stat->accountSecSinceUpdate);
    }
    if (stat->missionSecSinceUpdate > 120) {
        trouble = true;
        TROUBLE(10);
        strcatf(reason, "MissionServer has not responded for %d seconds\r\n", stat->missionSecSinceUpdate);
    }
    if (stat->turnstileSecSinceUpdate > 120)
    {
        trouble = true;
        TROUBLE(10);
        strcatf(reason, "TurnstileServer has not responded for %d seconds\r\n", stat->turnstileSecSinceUpdate);
    }
    if (stat->overloadProtection > 0)
    {
        trouble = true;
        TROUBLE(9);
        smoverloadFormat(stat, reason);
    }
// Temporarily removed this since a ServerMonitor got out that was confusing the Log and Launcher
//            || (stat->launcherMonitor        && badState(stat->launcherMonitor->status));
    if (!trouble)
        strcpy(reason, "");
    return trouble;
}

#define TROUBLE_STRING "TROUBLE"
#define TROUBLE_THRESHOLD 5
#define WARNING_STRING "WARNING"
#define WARNING_THRESHOLD 1

static void updateConnectionStatus(ServerStats *stat)
{
    if (!stat->link.connected && stat->connected) {
        // We were disconnected, flag for reconnect, and update status
        strcpy(stat->status, "Connection Lost");
        listViewSetItemColor(lvSmShards1, stat, COLOR_TROUBLE);
        listViewSetItemColor(lvSmShards2, stat, COLOR_TROUBLE);
        stat->connected = 0;
        stat->reconnect_countdown = DEFAULT_RECONNECT_TIME;
        listViewItemChanged(lvSmShards1, stat);
        listViewItemChanged(lvSmShards2, stat);
        if(lvShardRelays)
            listViewItemChanged(lvShardRelays, stat);
    } else if (!stat->link.connected && stat->reconnect_countdown) {
        stat->reconnect_countdown--;
        if (stat->reconnect_countdown == 0) {
            if (!shardMonConnectSingle(stat)) {
                stat->reconnect_countdown = DEFAULT_RECONNECT_TIME;
            }
        }
        if (stat->reconnect_countdown <  DEFAULT_RECONNECT_TIME - RECONNECT_UPDATE_DELAY) {
            int timeout = (stat->reconnect_countdown + maxtps - 1)/ maxtps;
            sprintf(stat->status, "Reconnect in %1d", timeout);
            listViewSetItemColor(lvSmShards1, stat, COLOR_RECONNECT);
            listViewSetItemColor(lvSmShards2, stat, COLOR_RECONNECT);
            listViewItemChanged(lvSmShards1, stat);
            listViewItemChanged(lvSmShards2, stat);
            if(lvShardRelays)
                listViewItemChanged(lvShardRelays, stat);
        }
    }

    if (stat->link.connected) {
        int trouble_level;
        stat->secondsSinceUpdate = timerCpuSeconds() - stat->link.lastRecvTime;
        if (stat->secondsSinceUpdate > 15) {
            sprintf(stat->status, "Unresponsive %1ds", stat->secondsSinceUpdate);
            listViewSetItemColor(lvSmShards1, stat, COLOR_TROUBLE);
            listViewSetItemColor(lvSmShards2, stat, COLOR_TROUBLE);
            listViewItemChanged(lvSmShards1, stat);
        } else if (strnicmp(stat->status, "Unresponsive", strlen("Unresponsive"))==0) {
            strcpy(stat->status, "CONNECTED");
            listViewSetItemColor(lvSmShards1, stat, COLOR_CONNECTED);
            listViewSetItemColor(lvSmShards2, stat, COLOR_CONNECTED);
            listViewItemChanged(lvSmShards1, stat);
        } else {
            bool trouble = troubledStat(stat, trouble_reason, &trouble_level);
            if (trouble) {
                if (trouble_level >= TROUBLE_THRESHOLD) {
                    if (stricmp(stat->status, TROUBLE_STRING)!=0) {
                        sprintf(stat->status, TROUBLE_STRING);
                        listViewSetItemColor(lvSmShards1, stat, COLOR_TROUBLE);
                        listViewSetItemColor(lvSmShards2, stat, COLOR_TROUBLE);
                        listViewItemChanged(lvSmShards1, stat);
                    }
                } else if (trouble_level >= WARNING_THRESHOLD) {
                    if (stricmp(stat->status, WARNING_STRING)!=0) {
                        sprintf(stat->status, WARNING_STRING);
                        listViewSetItemColor(lvSmShards1, stat, COLOR_TROUBLE_LIGHT);
                        listViewSetItemColor(lvSmShards2, stat, COLOR_TROUBLE_LIGHT);
                        listViewItemChanged(lvSmShards1, stat);
                    }
                } else {
                    // Can't happen
                    assert(0);
                }
            } else {
                // Not in trouble
                if (stricmp(stat->status, "CONNECTED")!=0) {
                    strcpy(stat->status, "CONNECTED");
                    listViewSetItemColor(lvSmShards1, stat, COLOR_CONNECTED);
                    listViewSetItemColor(lvSmShards2, stat, COLOR_CONNECTED);
                    listViewItemChanged(lvSmShards1, stat);
                }
            }
        }
    }
}

void updateShardMonCrashMsg(HWND hDlg, char *text)
{
    static char old_text[4096];
    if (text && text[0] && strcmp(old_text, text)==0)
        return;
    SetDlgItemTextA(hDlg, IDC_EDIT_CRASH_MSG, text?text:"");
    Strncpyt(old_text, text);
}

static void updateTroubleText(ListView *lv, ServerStats *stat, HWND hDlg)
{
    int trouble_level;
    troubledStat(stat, trouble_reason, &trouble_level);
    updateShardMonCrashMsg(hDlg, trouble_reason);
}


static void shardMonTick(HWND hDlg)
{
    int i, j;
    bool init=true;
    int count=0;
    static bool last_busy=true;
    getText(hDlg, NULL, mapping, ARRAY_SIZE(mapping));
    NMMonitor(1);

    // Initialize min, max, avg, total
    sdCopyStruct(ServerStatsDispInfo2, &zeroServerStat, eaGet(eaServerStatsSum, STAT_AVG));
    sdCopyStruct(ServerStatsDispInfo2, &zeroServerStat, eaGet(eaServerStatsSum, STAT_TOT));

    // Search and update statuses
    for (i=0; i<eaSize(eaServerStats); i++)
    {
        ServerStats *stat = eaGet(eaServerStats, i);

        if (stat) {
            // Check connection status
            updateConnectionStatus(stat);
            // Update average, max, etc
            if (stat->connected) {
                count++;
                if (init) {
                    init = false;
                    sdCopyStruct(ServerStatsDispInfo2, stat, eaGet(eaServerStatsSum, STAT_MIN));
                    sdCopyStruct(ServerStatsDispInfo2, stat, eaGet(eaServerStatsSum, STAT_MAX));
                }
                for (j=0; j<ARRAY_SIZE(ops); j++) {
                    shDoOperation(ops[j], ServerStatsDispInfo2, eaGet(eaServerStatsSum, j), stat);
                }
            }
            // Update process status columns
            if (stat->dbServerMonitor)
                strcpy(stat->dbServerProcessStatus, pmsToString(stat->dbServerMonitor->status));
            if (stat->launcherMonitor)
                strcpy(stat->launcherProcessStatus, pmsToString(stat->launcherMonitor->status));
            listViewItemChanged(lvSmShards2, stat);
        }
    }

    // Calc average
    if (count) {
        shDoOperationSetInt(count);
        shDoOperation(STRUCTOP_DIV, ServerStatsDispInfo2, eaGet(eaServerStatsSum, STAT_AVG), OPERAND_INT);
    }

    for (i=0; i<eaSize(eaServerStatsSum); i++) {
        listViewItemChanged(lvSmShards2, eaGet(eaServerStatsSum, i));
        shcUpdate(&shcShardMonHistory, eaGet(eaServerStatsSum, i), ServerStatsLogInfoShardMon, timerCpuTicks(), special_rows[i]);
    }

    // Update buttons
    if (shardMonitorBusy != last_busy) {
        count = listViewDoOnSelected(lvSmShards1, NULL, NULL);
        last_busy = shardMonitorBusy;
        EnableWindow(GetDlgItem(hDlg, IDC_BTN_CONNECT), !shardMonitorBusy);
        EnableWindow(GetDlgItem(hDlg, IDC_BTN_DISCONNECT), !shardMonitorBusy);
        EnableWindow(GetDlgItem(hDlg, IDC_BTN_CONNECT_SINGLE), count >= 1 && !shardMonitorBusy);
        EnableWindow(GetDlgItem(hDlg, IDC_BTN_DISCONNECT_SINGLE), count >= 1 && !shardMonitorBusy);
    }

    {
        int selected_count = listViewDoOnSelected(lvSmShards1, NULL, NULL);
        if (selected_count == 1) {
            listViewDoOnSelected(lvSmShards1, updateTroubleText, hDlg);
        }
    }

    checkLogHistory(hDlg);

    if (g_someDataOutOfSync) {
        static bool warnedUser = false;
        if (!warnedUser) {
            warnedUser = true;
            MessageBoxA(NULL, "You are connecting with a different version of Shard Monitor than the shard you are connecting to.  Some data may be invalid.\n\nThis message will only appear once once for shard monitor connections.", "Shard Monitor Warning", MB_ICONWARNING);
        }
    }
}

void updateCrashMsgTextShardMon(ListView *lv, ServerStats* stat, void *unused)
{
    int trouble_level;
    troubledStat(stat, trouble_reason, &trouble_level);
    updateShardMonCrashMsg(listViewGetParentWindow(lv), trouble_reason);
}


static void onTimer(HWND hDlg)
{
    static int timer=0;
    static F32 spf = 1./maxtps;
    static int lock=0;
    if (lock!=0)
        return;
    lock++;
    if (timer==0) {
        timer = timerAlloc();
    }
    if (timerElapsed(timer) > spf) {
        timerStart(timer);
        shardMonTick(hDlg);
    }
    lock--;
}



LRESULT CALLBACK DlgShardMonProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    RECT rect;
    LRESULT ret = FALSE;

    switch (iMsg)
    {
    case WM_INITDIALOG:
        if (!lvSmShards1)
            lvSmShards1 = listViewCreate();
        listViewInit(lvSmShards1, ServerStatsDispInfo1, hDlg, GetDlgItem(hDlg, IDC_LST_SHARDS1));
        listViewSetSortable(lvSmShards1, false);
        if (!lvSmShards2)
            lvSmShards2 = listViewCreate();
        listViewInit(lvSmShards2, ServerStatsDispInfo2, hDlg, GetDlgItem(hDlg, IDC_LST_SHARDS2));
        listViewSetSortable(lvSmShards2, false);
        
        loadValuesFromReg(hDlg, NULL, mapping, ARRAY_SIZE(mapping));
        shardMonInit();
        SetTimer(hDlg, 0, 10, NULL);
        GetClientRect(hDlg, &rect); 
        doDialogOnResize(hDlg, (WORD)(rect.right - rect.left), (WORD)(rect.bottom - rect.top), IDC_ALIGNME, IDC_UPPERLEFT);
        setDialogMinSize(hDlg, 0, 200);
        break;

    case WM_TIMER:
        onTimer(hDlg);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            break;
        case IDCANCEL:
            EndDialog(hDlg, 0);
            PostQuitMessage(0);
            return TRUE;
        case IDC_BTN_CONNECT:
            shardMonConnectAll();
            break;
        case IDC_BTN_DISCONNECT:
            shardMonDisconnectAll();
            break;
        case IDC_BTN_CONNECT_SINGLE:
            shardMonConnectSingleWrapper();
            break;
        case IDC_BTN_DISCONNECT_SINGLE:
            shardMonDisconnectSingleWrapper();
            break;
        case IDC_BTN_CONFIGURE:
            shardMonDisconnectAll();
            shardMonConfigure(g_hInst, hDlg, "./ShardMonConfig.txt");
            shardMonInit();
            shardRelayInit();
            shardMonConnectAll();
            break;
        case IDC_BTN_ADMINMESSAGE:
            shardMonSendAdminMessage(hDlg);
            break;
        case IDC_BTN_SERVERMONITOR:
            shardMonTargettedServerMonitor(hDlg);
            break;
        case IDC_BTN_OVERLOADPROTECTION_ON:
            shardMonSendOverloadProtection(hDlg, true);
            break;
        case IDC_BTN_OVERLOADPROTECTION_OFF:
            shardMonSendOverloadProtection(hDlg, false);
            break;
        case IDC_BTN_REMOTEDESKTOP:
            shardMonTargettedRemoteDesktop(hDlg);
            break;
        case IDC_BTN_RECONCILE_LAUNCHER:
            shardMonReconcileLauncher(hDlg);
            break;
        }
        break;
    case WM_SIZE:
        {
            WORD w = LOWORD(lParam);
            WORD h = HIWORD(lParam);
            //printf("shard res: %dx%d\n", w, h);
            //SendMessage(hStatusBar, iMsg, wParam, lParam);
            doDialogOnResize(hDlg, w, h, IDC_ALIGNME, IDC_UPPERLEFT);
        }
        break;
    case WM_NOTIFY:
        {
            int idCtrl = (int)wParam;
            int count;
            ret |= listViewOnNotify(lvSmShards1, wParam, lParam, updateCrashMsgTextShardMon);
            ret |= listViewOnNotify(lvSmShards2, wParam, lParam, NULL);

            count = listViewDoOnSelected(lvSmShards1, NULL, NULL);
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_ADMINMESSAGE), (count >= 1));
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_OVERLOADPROTECTION_ON), (count >= 1));
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_OVERLOADPROTECTION_OFF), (count >= 1));
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_SERVERMONITOR), (count == 1));
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_REMOTEDESKTOP), (count == 1));
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_RECONCILE_LAUNCHER), (count == 1));
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_CONNECT_SINGLE), (count >= 1) && !shardMonitorBusy);
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_DISCONNECT_SINGLE), (count >= 1) && !shardMonitorBusy);
        }
        break;
    }

    return ret;
}
