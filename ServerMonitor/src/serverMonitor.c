#include <container.h>
#include <launcher_common.h>
#include <svrmoncomm.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/ListView.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/structHist.h>
#include <utilitieslib/utils/structNet.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/WinTabUtil.h>
#include <utilitieslib/utils/winutil.h>

#include <assert.h>
#include <commctrl.h>
#include <process.h>
#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chatMonitor.h"
#include "processMonitor.h"
#include "prompt.h"
#include "resource.h"
#include "serverMonitor.h"
#include "serverMonitorCmdRelay.h"
#include "serverMonitorCommon.h"
#include "serverMonitorCrashMsg.h"
#include "serverMonitorEnts.h"
#include "serverMonitorListener.h"
#include "serverMonitorNet.h"
#include "serverMonitorOverload.h"
#include "shardMonitor.h"
#include "shardMonitorComm.h"

extern bool g_someDataOutOfSync;

ServerMonitorState g_state = {0};

// All of these are logged and sent over the network (e.g. to shardmon).
// make sure to update ServerStatsDispInfo2 if you change this
TokenizerParseInfo ServerStatsGeneralInfo[] = 
{
    { "DbServerTrouble",        TOK_MINBITS(2)|        TOK_INT(ServerStats, dbserver_in_trouble, 0)},
    { "TroubledMapservers",        TOK_MINBITS(2)|        TOK_INT(ServerStats, servers_in_trouble, 0)},
    { "ChatServerInTrouble",    TOK_MINBITS(2)|        TOK_INT(ServerStats, chatserver_in_trouble, 0)},
    { "ArenaServerInTrouble",    TOK_MINBITS(2)|        TOK_INT(ServerStats, arenaserver_in_trouble, 0)},
    { "SecondsSinceUpdate",        TOK_MINBITS(3)|        TOK_INT(ServerStats, secondsSinceDbUpdate, 0)},
    { "ArenaSecSinceUpdate",                        TOK_INT(ServerStats, arenaSecSinceUpdate, 0)},
    { "StatSecSinceUpdate",                            TOK_INT(ServerStats, statSecSinceUpdate, 0)},
    { "BeaconWait",                                    TOK_INT(ServerStats, beaconWaitSeconds, 0)},
    { "MapServers",                TOK_MINBITS(10)|    TOK_INT(ServerStats, mscount, 0)},
    { "StuckMapservers",        TOK_MINBITS(1)|        TOK_INT(ServerStats, smscount, 0), },
    { "StaticMapServers",        TOK_MINBITS(10)|    TOK_INT(ServerStats, mscount_static, 0), },
    { "BaseMapServers",            TOK_MINBITS(10)|    TOK_INT(ServerStats, mscount_base, 0), },
    { "MissionMapServers",        TOK_MINBITS(10)|    TOK_INT(ServerStats, mscount_missions, 0), },
    { "Launchers",                TOK_MINBITS(6)|        TOK_INT(ServerStats, lcount, 0), },
    { "ServerApps",                TOK_MINBITS(6)|        TOK_INT(ServerStats, sacount, 0), },
    { "PlayersPlaying",            TOK_MINBITS(14)|    TOK_INT(ServerStats, pcount, 0), },
    { "PlayerEntsLoaded",        TOK_MINBITS(14)|    TOK_INT(ServerStats, pcount_ents, 0), },
    { "PlayersLoggingIn",        TOK_MINBITS(6)|        TOK_INT(ServerStats, pcount_login, 0), },
    { "PlayersQueued",            TOK_MINBITS(6)|        TOK_INT(ServerStats, pcount_queued, 0), },
    { "PlayersTransfering",        TOK_MINBITS(6)|        TOK_INT(ServerStats, pcount_connecting, 0), },
    { "QueueConnections",        TOK_MINBITS(6)|        TOK_INT(ServerStats, queue_connections, 0), },
    { "Entities",                TOK_MINBITS(15)|    TOK_INT(ServerStats, ecount, 0), },
    { "MonsterCount",            TOK_MINBITS(15)|    TOK_INT(ServerStats, mcount, 0), },
    { "ServerMonitors",            TOK_MINBITS(2)|        TOK_INT(ServerStats, servermoncount, 0), },
    { "SQLWBDepth",                TOK_MINBITS(7)|        TOK_INT(ServerStats, sqlwb, 0), },
    { "SQLThroughput",            TOK_MINBITS(10)|    TOK_INT(ServerStats, sqlthroughput, 0), },
    { "SQLAvgLat",                TOK_MINBITS(10)|    TOK_INT(ServerStats, sqlavglat, 0), },
    { "SQLWorstLat",            TOK_MINBITS(10)|    TOK_INT(ServerStats, sqlworstlat, 0), },
    { "SQLForeIdleRatio",                            TOK_F32(ServerStats, sqlforeidleratio, 0), 0, TOK_FORMAT_PERCENT},
    { "SQLBackIdleRatio",                            TOK_F32(ServerStats, sqlbackidleratio, 0), 0, TOK_FORMAT_PERCENT},
    { "LogLatency",                TOK_MINBITS(10)|    TOK_INT(ServerStats, loglat, 0), },
    { "LogBytes",                TOK_MINBITS(32)|    TOK_AUTOINT(ServerStats, logbytes, 0), },
    { "LogQueueCount",            TOK_MINBITS(10)|    TOK_INT(ServerStats, logqcnt, 0), },
    { "LogQueueMax",            TOK_MINBITS(10)|    TOK_INT(ServerStats, logqmax, 0), },
    { "LogSortMem",                TOK_MINBITS(32)|    TOK_AUTOINT(ServerStats, logsortmem, 0), },
    { "LogSortMemCap",            TOK_MINBITS(32)|    TOK_AUTOINT(ServerStats, logsortcap, 0), },
    { "MaxDbTickLen",                                TOK_F32(ServerStats, dbticklen, 0)},
    { "AutoDelinkTime",            TOK_MINBITS(10)|    TOK_INT(ServerStats, autodelinktime, 0)},
    { "AutoDelinkEnabled",        TOK_MINBITS(2)|        TOK_BOOL(ServerStats, autodelink, 0)},
    { "AverageCPU",                                    TOK_F32(ServerStats, avgCpu, 0), 0, TOK_FORMAT_PERCENT},
    { "AverageCPU60",                                TOK_F32(ServerStats, avgCpu60, 0), 0, TOK_FORMAT_PERCENT},
    { "MaxCPU",                                        TOK_F32(ServerStats, maxCpu, 0), 0, TOK_FORMAT_PERCENT},
    { "MaxCPU60",                                    TOK_F32(ServerStats, maxCpu60, 0), 0, TOK_FORMAT_PERCENT},
    { "TotalPhysUsed",            TOK_MINBITS(24)|    TOK_INT(ServerStats, totalPhysUsed, 0), },
    { "TotalVirtUsed",            TOK_MINBITS(24)|    TOK_INT(ServerStats, totalVirtUsed, 0), },
    { "MinPhysAvail",            TOK_MINBITS(24)|    TOK_INT(ServerStats, minPhysAvail, 0), },
    { "MinVirtAvail",            TOK_MINBITS(24)|    TOK_INT(ServerStats, minVirtAvail, 0), },
    { "AvgPhysAvail",            TOK_MINBITS(24)|    TOK_INT(ServerStats, avgPhysAvail, 0), },
    { "AvgVirtAvail",            TOK_MINBITS(24)|    TOK_INT(ServerStats, avgVirtAvail, 0), },
    { "MaxPhysAvail",            TOK_MINBITS(24)|    TOK_INT(ServerStats, maxPhysAvail, 0), },
    { "MaxVirtAvail",            TOK_MINBITS(24)|    TOK_INT(ServerStats, maxVirtAvail, 0), },
    { "MaxSecondsSinceUpdate",    TOK_MINBITS(4)|        TOK_INT(ServerStats, maxSecondsSinceUpdate, 0), },
    { "MaxCrashedMaps",            TOK_MINBITS(2)|        TOK_INT(ServerStats, maxCrashedMaps, 0), },
    { "HeroPlayers",            TOK_MINBITS(6)|        TOK_INT(ServerStats, pcount_hero, 0), },
    { "VillainPlayers",            TOK_MINBITS(6)|        TOK_INT(ServerStats, pcount_villain, 0), },
    { "SMS.CrashedCount",        TOK_MINBITS(4)|        TOK_INT(ServerStats, sms_crashed_count, 0), },
    { "SMS.LongTickCount",        TOK_MINBITS(2)|        TOK_INT(ServerStats, sms_long_tick_count, 0), },
    { "SMS.StuckCount",            TOK_MINBITS(2)|        TOK_INT(ServerStats, sms_stuck_count, 0), },
    { "SMS.StuckStartingCount",    TOK_MINBITS(2)|        TOK_INT(ServerStats, sms_stuck_starting_count, 0), },
    { "SA.CrashedCount",        TOK_MINBITS(2)|        TOK_INT(ServerStats, sa_crashed_count, 0), },
    { "Hero Auction",                                TOK_INT(ServerStats, heroAuctionSecSinceUpdate, 0), },
    { "Villain Auction",                            TOK_INT(ServerStats, villainAuctionSecSinceUpdate, 0), },
    { "Account Server",                                TOK_INT(ServerStats, accountSecSinceUpdate, 0), },
    { "Acount Server",            TOK_REDUNDANTNAME|    TOK_INT(ServerStats, accountSecSinceUpdate, 0), },
    { "Mission Server",                                TOK_INT(ServerStats, missionSecSinceUpdate, 0), },
    { "Turnstile Server",                            TOK_INT(ServerStats, turnstileSecSinceUpdate, 0), },
    { "Overload Protection",                        TOK_INT(ServerStats, overloadProtection, 0), },
    {0}
};

TokenizerParseInfo ServerStatsLogInfoServerMon[] = // Logged to Global.csv on ServerMonitor
{
    { "GeneralInfo",        TOK_NULLSTRUCT(ServerStatsGeneralInfo) },
    {0}
};

TokenizerParseInfo ServerStatsLogInfoShardMon[] = // Logged to Shards.csv on ShardMonitor (per-shard)
{
    { "Name",                TOK_FIXEDSTR(ServerStats, name), 0, TOK_FORMAT_LVWIDTH(70)},
    { "Status",                TOK_FIXEDSTR(ServerStats, status), 0, TOK_FORMAT_LVWIDTH(110)},
    { "GeneralInfo",        TOK_NULLSTRUCT(ServerStatsGeneralInfo) },
    { "ServerVersion",        TOK_FIXEDSTR(ServerStats, serverversion) },
    {0}
};

TokenizerParseInfo ShardRelayNetInfo[] =    // extra relay info sent over network
{
    { "MaxLastUpdate",    TOK_INT(ServerStats, maxLastUpdate, 0), 0,        TOK_FORMAT_LVWIDTH(40)},
    { "DRelays",        TOK_INT(ServerStats, ds_relays, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "MRelays",        TOK_INT(ServerStats, ms_relays, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "CRelays",        TOK_INT(ServerStats, custom_relays, 0), 0,        TOK_FORMAT_LVWIDTH(60)},
    { "Crashed Maps",    TOK_INT(ServerStats, crashed_mscount, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { "Status",            TOK_FIXEDSTR(ServerStats, shardrelay_status), 0,TOK_FORMAT_LVWIDTH(120)},
    { "Message",        TOK_FIXEDSTR(ServerStats, shardrelay_msg), 0,    TOK_FORMAT_LVWIDTH(120)},
    { 0 }
};

TokenizerParseInfo ShardChatNetInfo[] =    // extra relay info sent over network
{
    { "ChatSvrConnected",    TOK_INT(ServerStats, chatServerConnected, 0), 0,TOK_FORMAT_LVWIDTH(40)},
    { "ChatSecSinceUpdate",    TOK_INT(ServerStats, chatSecSinceUpdate, 0), 0,    TOK_FORMAT_LVWIDTH(60)},
    { "ChatTotalUsers",        TOK_INT(ServerStats, chatTotalUsers, 0), 0,        TOK_FORMAT_LVWIDTH(80)},
    { "ChatOnlineUsers",    TOK_INT(ServerStats, chatOnlineUsers, 0), 0,    TOK_FORMAT_LVWIDTH(90)},
    { "ChatChannels",        TOK_INT(ServerStats, chatChannels, 0), 0,        TOK_FORMAT_LVWIDTH(70)},
    { "ChatLinks",            TOK_INT(ServerStats, chatLinks, 0), 0,            TOK_FORMAT_LVWIDTH(50)},
    { 0 }
};


TokenizerParseInfo ProcessMonitorInfo[] =    
{
    { "Monitor",    TOK_BOOL(ProcessMonitorEntry, monitor, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { "Start",        TOK_BOOL(ProcessMonitorEntry, start, 0), 0,        TOK_FORMAT_LVWIDTH(40)},
    { "Restart",    TOK_BOOL(ProcessMonitorEntry, restart, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { "Seconds",    TOK_INT(ProcessMonitorEntry, seconds, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { "Status",        TOK_INT(ProcessMonitorEntry, status, 0), 0,        TOK_FORMAT_LVWIDTH(40)},
    { "NumRestarts",TOK_INT(ProcessMonitorEntry, crashCount, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { 0 }
};

TokenizerParseInfo ProcessMonitorCrashInfo[] =    
{
    { "NumCrashes",TOK_INT(ProcessMonitorEntry, crashCount, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { 0 }
};


TokenizerParseInfo ServerStatsNetInfo[] = // Info sent between ServerMonitor and ShardMonitor
{
    { "GeneralInfo",        TOK_NULLSTRUCT(ServerStatsGeneralInfo) },
    { "RelayInfo",            TOK_NULLSTRUCT(ShardRelayNetInfo) },
    { "ChatServerInfo",        TOK_NULLSTRUCT(ShardChatNetInfo) },
    { "ClientVersion",        TOK_FIXEDSTR(ServerStats, gameversion)},
    { "ServerVersion",        TOK_FIXEDSTR(ServerStats, serverversion)},
    { "ProcMonDbServer",    TOK_OPTIONALSTRUCT(ServerStats, dbServerMonitor, ProcessMonitorInfo)},
    { "ProcMonLauncher",    TOK_OPTIONALSTRUCT(ServerStats, launcherMonitor, ProcessMonitorInfo)},
    {0}
};

TokenizerParseInfo ServerStatsDispInfo1[] = 
{
    { "Name",                TOK_FIXEDSTR(ServerStats, name), 0,        TOK_FORMAT_LVWIDTH(70)},
    { "Status",                TOK_FIXEDSTR(ServerStats, status), 0,    TOK_FORMAT_LVWIDTH(110)},
    { 0 }
};

// shardmon display panel
// received via ServerStatsGeneralInfo, update that if you add to this.
TokenizerParseInfo ServerStatsDispInfo2[] = 
{
    { "DBTrbl",                TOK_INT(ServerStats, dbserver_in_trouble, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { "MSTrbl",                TOK_INT(ServerStats, servers_in_trouble, 0), 0,        TOK_FORMAT_LVWIDTH(40)},
    { "StuckMapservers",    TOK_INT(ServerStats, smscount, 0), 0,                TOK_FORMAT_LVWIDTH(60)},
    { "#Playing",            TOK_INT(ServerStats, pcount, 0), 0,                    TOK_FORMAT_LVWIDTH(60)},
    { "#LoggingIn",            TOK_INT(ServerStats, pcount_login, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "#Queued",            TOK_INT(ServerStats, pcount_queued, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "#Xfering",            TOK_MINBITS(6)| TOK_INT(ServerStats, pcount_connecting, 0), 0, TOK_FORMAT_LVWIDTH(60)},
    { "#Heroes",            TOK_INT(ServerStats, pcount_hero, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "#Villains",            TOK_INT(ServerStats, pcount_villain, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "#QueueConns",        TOK_INT(ServerStats, queue_connections, 0), 0,        TOK_FORMAT_LVWIDTH(60)},
    { "SQLWBDepth",            TOK_INT(ServerStats, sqlwb, 0), 0,                    TOK_FORMAT_LVWIDTH(60)},
    { "SQLThroughput",        TOK_INT(ServerStats, sqlthroughput, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "SQLAvgLat",            TOK_INT(ServerStats, sqlavglat, 0), 0,                TOK_FORMAT_MICROSECONDS|TOK_FORMAT_LVWIDTH(65)},
    { "SQLWorstLat",        TOK_INT(ServerStats, sqlworstlat, 0), 0,            TOK_FORMAT_MICROSECONDS|TOK_FORMAT_LVWIDTH(65)},
    { "SQLForeIdleRatio",    TOK_F32(ServerStats, sqlforeidleratio, 0), 0,        TOK_FORMAT_LVWIDTH(45)|TOK_FORMAT_PERCENT},
    { "SQLBackIdleRatio",    TOK_F32(ServerStats, sqlbackidleratio, 0), 0,        TOK_FORMAT_LVWIDTH(45)|TOK_FORMAT_PERCENT},
    { "LogLatency",            TOK_INT(ServerStats, loglat, 0), 0,                    TOK_FORMAT_MICROSECONDS|TOK_FORMAT_LVWIDTH(65)},
    { "LogBytes",            TOK_AUTOINT(ServerStats, logbytes, 0), 0,                TOK_FORMAT_BYTES|TOK_FORMAT_LVWIDTH(65)},
    { "LogQueueCount",        TOK_INT(ServerStats, logqcnt, 0), 0,                TOK_FORMAT_LVWIDTH(60)},
    { "LogQueueMax",        TOK_INT(ServerStats, logqmax, 0), 0,                TOK_FORMAT_LVWIDTH(60)},
    { "LogSortMem",            TOK_AUTOINT(ServerStats, logsortmem, 0), 0,                TOK_FORMAT_BYTES|TOK_FORMAT_LVWIDTH(65)},
    { "LogSortMemCap",        TOK_AUTOINT(ServerStats, logsortcap, 0), 0,                TOK_FORMAT_BYTES|TOK_FORMAT_LVWIDTH(65)},
    { "DbTickLen",            TOK_F32(ServerStats, dbticklen, 0), 0,                TOK_FORMAT_LVWIDTH(60)},
    { "Launchers",            TOK_INT(ServerStats, lcount, 0), 0,                    TOK_FORMAT_LVWIDTH(60)},
    { "ChatTrbl",            TOK_INT(ServerStats, chatserver_in_trouble, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { "SecondsSinceUpdate",    TOK_INT(ServerStats, secondsSinceDbUpdate, 0)},
    { "ArenaSecSinceUpdate",TOK_INT(ServerStats, arenaSecSinceUpdate, 0)},
    { "StatSecSinceUpdate",    TOK_INT(ServerStats, statSecSinceUpdate, 0)},
    { "BeaconWait",            TOK_INT(ServerStats, beaconWaitSeconds, 0)},
    { "AvgCPU",                TOK_F32(ServerStats, avgCpu, 0), 0,                    TOK_FORMAT_LVWIDTH(45)|TOK_FORMAT_PERCENT},
    { "AvgCPU60",            TOK_F32(ServerStats, avgCpu60, 0), 0,                TOK_FORMAT_LVWIDTH(45)|TOK_FORMAT_PERCENT},
    { "MaxCPU",                TOK_F32(ServerStats, maxCpu, 0), 0,                    TOK_FORMAT_LVWIDTH(45)|TOK_FORMAT_PERCENT},
    { "MaxCPU60",            TOK_F32(ServerStats, maxCpu60, 0), 0,                TOK_FORMAT_LVWIDTH(45)|TOK_FORMAT_PERCENT},
    { "TotalPhysUsed",        TOK_INT(ServerStats, totalPhysUsed, 0), 0,            TOK_FORMAT_KBYTES|TOK_FORMAT_LVWIDTH(65)},
    { "TotalVirtUsed",        TOK_INT(ServerStats, totalVirtUsed, 0), 0,            TOK_FORMAT_KBYTES|TOK_FORMAT_LVWIDTH(65)},
    { "MinVirtAvail",        TOK_INT(ServerStats, minVirtAvail, 0), 0,            TOK_FORMAT_KBYTES|TOK_FORMAT_LVWIDTH(65)},
    { "MinPhysAvail",        TOK_INT(ServerStats, minPhysAvail, 0), 0,            TOK_FORMAT_KBYTES|TOK_FORMAT_LVWIDTH(65)},
    { "AvgPhysAvail",        TOK_INT(ServerStats, avgPhysAvail, 0), 0,            TOK_FORMAT_KBYTES|TOK_FORMAT_LVWIDTH(65)},
    { "AvgVirtAvail",        TOK_INT(ServerStats, avgVirtAvail, 0), 0,            TOK_FORMAT_KBYTES|TOK_FORMAT_LVWIDTH(65)},
    { "MaxPhysAvail",        TOK_INT(ServerStats, maxPhysAvail, 0), 0,            TOK_FORMAT_KBYTES|TOK_FORMAT_LVWIDTH(65)},
    { "MaxVirtAvail",        TOK_INT(ServerStats, maxVirtAvail, 0), 0,            TOK_FORMAT_KBYTES|TOK_FORMAT_LVWIDTH(65)},
    { "ServerApps",            TOK_INT(ServerStats, sacount, 0), 0,                TOK_FORMAT_LVWIDTH(60)},
    { "MapServers",            TOK_INT(ServerStats, mscount, 0), 0,                TOK_FORMAT_LVWIDTH(60)},
    { "StaticMS",            TOK_INT(ServerStats, mscount_static, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "BaseMS",                TOK_INT(ServerStats, mscount_base, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "MissionMS",            TOK_INT(ServerStats, mscount_missions, 0), 0,        TOK_FORMAT_LVWIDTH(60)},
    { "#EntsLoaded",        TOK_INT(ServerStats, pcount_ents, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "#Ents",                TOK_INT(ServerStats, ecount, 0), 0,                    TOK_FORMAT_LVWIDTH(60)},
    { "#Monsters",            TOK_INT(ServerStats, mcount, 0), 0,                    TOK_FORMAT_LVWIDTH(60)},
    { "#MSCrashed",            TOK_INT(ServerStats, sms_crashed_count, 0), 0,        TOK_FORMAT_LVWIDTH(60)},
    { "#MSLongTick",        TOK_INT(ServerStats, sms_long_tick_count, 0), 0,    TOK_FORMAT_LVWIDTH(60)},
    { "#MSStuck",            TOK_INT(ServerStats, sms_stuck_count, 0), 0,        TOK_FORMAT_LVWIDTH(60)},
    { "#MSStuckStarting",    TOK_INT(ServerStats, sms_stuck_starting_count,0),0, TOK_FORMAT_LVWIDTH(60)},
    { "#SACrashed",            TOK_INT(ServerStats, sa_crashed_count, 0), 0,        TOK_FORMAT_LVWIDTH(60)},
    { "MaxCrashedMaps",        TOK_INT(ServerStats, maxCrashedMaps, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "MaxSecondsSinceUpdate",    TOK_INT(ServerStats, maxSecondsSinceUpdate,0),0,TOK_FORMAT_LVWIDTH(60)},
    { "ServerMonitors",        TOK_INT(ServerStats, servermoncount, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "AutoDelinkTime",        TOK_INT(ServerStats, autodelinktime, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "AutoDelinkEnabled",    TOK_BOOL(ServerStats, autodelink, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "NetLink",            TOK_EMBEDDEDSTRUCT(ServerStats, link, NetLinkInfo) },
    { "ClientVersion",        TOK_FIXEDSTR(ServerStats, gameversion), 0,            TOK_FORMAT_LVWIDTH(120)},
    { "ServerVersion",        TOK_FIXEDSTR(ServerStats, serverversion), 0,        TOK_FORMAT_LVWIDTH(120)},

/*    // Don't really care about this raw data visually
    { "ProcMonDbServer",    TOK_POINTER,    offsetof(ServerStats, dbServerMonitor), sizeof(ProcessMonitorEntry), ProcessMonitorInfo},
    { "ProcMonLauncher",    TOK_POINTER,    offsetof(ServerStats, launcherMonitor), sizeof(ProcessMonitorEntry), ProcessMonitorInfo},
*/
    { "DbServer.exe",        TOK_FIXEDSTR(ServerStats, dbServerProcessStatus), 0,TOK_FORMAT_LVWIDTH(80)},
//    { "DbServerCrashes",    TOK_POINTER,    offsetof(ServerStats, dbServerMonitor), sizeof(ProcessMonitorEntry), ProcessMonitorCrashInfo},
    { "Launcher.exe",        TOK_FIXEDSTR(ServerStats, launcherProcessStatus), 0,TOK_FORMAT_LVWIDTH(80)},
//    { "LauncherCrashes",    TOK_POINTER,    offsetof(ServerStats, launcherMonitor), sizeof(ProcessMonitorEntry), ProcessMonitorCrashInfo},

    { "ChatSvrConnected",    TOK_INT(ServerStats, chatServerConnected, 0), 0,    TOK_FORMAT_LVWIDTH(80)},
    { "ChatTotalUsers",        TOK_INT(ServerStats, chatTotalUsers, 0), 0,            TOK_FORMAT_LVWIDTH(80)},
    { "ChatOnlineUsers",    TOK_INT(ServerStats, chatOnlineUsers, 0), 0,        TOK_FORMAT_LVWIDTH(80)},
    { "ChatChannels",        TOK_INT(ServerStats, chatChannels, 0), 0,            TOK_FORMAT_LVWIDTH(80)},
    { "ChatSecSinceUpdate",    TOK_INT(ServerStats, chatSecSinceUpdate, 0), 0,        TOK_FORMAT_LVWIDTH(80)},
    { "ChatLinks",            TOK_INT(ServerStats, chatLinks, 0), 0,                TOK_FORMAT_LVWIDTH(80)},

    { "Configured IP",        TOK_INT(ServerStats, ip, 0), 0, TOK_FORMAT_IP | TOK_FORMAT_LVWIDTH(100)},
    { "Hero Auction",        TOK_INT(ServerStats, heroAuctionSecSinceUpdate, 0), 0, TOK_FORMAT_LVWIDTH(80)},
    { "Villain Auction",    TOK_INT(ServerStats, villainAuctionSecSinceUpdate, 0), 0, TOK_FORMAT_LVWIDTH(80)},
    { "Account Server",        TOK_INT(ServerStats, accountSecSinceUpdate, 0), 0, TOK_FORMAT_LVWIDTH(80)},
    { "Acount Server",        TOK_REDUNDANTNAME|TOK_INT(ServerStats, accountSecSinceUpdate, 0), 0, TOK_FORMAT_LVWIDTH(80)},
    { "Mission Server",        TOK_INT(ServerStats, missionSecSinceUpdate, 0), 0, TOK_FORMAT_LVWIDTH(80)},
    { "Turnstile Server",    TOK_INT(ServerStats, turnstileSecSinceUpdate, 0), 0, TOK_FORMAT_LVWIDTH(80)},
    { "Overload Protection",TOK_INT(ServerStats, overloadProtection, 0), 0, TOK_FORMAT_LVWIDTH(80)},

    {0}
};




TokenizerParseInfo ShardRelayDispInfo[] =    // defines display box under shard relay tab
{
    { "Name",                    TOK_FIXEDSTR(ServerStats, name), 0,                TOK_FORMAT_LVWIDTH(83)},
    { "Status",                    TOK_FIXEDSTR(ServerStats, status), 0,            TOK_FORMAT_LVWIDTH(110)},
    { "IpAddress",                TOK_INT(ServerStats, ip, 0), 0,                    TOK_FORMAT_IP | TOK_FORMAT_LVWIDTH(90)},
    { "DRelays",                TOK_INT(ServerStats, ds_relays, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "MRelays",                TOK_INT(ServerStats, ms_relays, 0), 0,            TOK_FORMAT_LVWIDTH(60)},
    { "CRelays",                TOK_INT(ServerStats, custom_relays, 0), 0,        TOK_FORMAT_LVWIDTH(60)},
    { "MaxLastUpdate (sec)",    TOK_INT(ServerStats, maxLastUpdate, 0), 0,        TOK_FORMAT_LVWIDTH(40)},    
    { "Launchers",                TOK_INT(ServerStats, lcount, 0), 0,                TOK_FORMAT_LVWIDTH(40)},
    { "Maps",                    TOK_INT(ServerStats, mscount, 0), 0,            TOK_FORMAT_LVWIDTH(40)},
    { "Crashed Maps",            TOK_INT(ServerStats, crashed_mscount, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { "Version",                TOK_FIXEDSTR(ServerStats, serverversion), 0,    TOK_FORMAT_LVWIDTH(120)},
    { "RelayStatus",            TOK_FIXEDSTR(ServerStats, shardrelay_status), 0,TOK_FORMAT_LVWIDTH(120)},
    { "Message",                TOK_FIXEDSTR(ServerStats, shardrelay_msg), 0,    TOK_FORMAT_LVWIDTH(120)},
    { 0 }
};

ServerMonitorState *createServerMonitorState(void)
{
    return calloc(sizeof(ServerMonitorState),1);
}

void destroyServerMonitorState(ServerMonitorState *state)
{
    svrMonDisconnect(state);
    svrMonClearAllLists(state);
    if (state->lvMaps) listViewDestroy(state->lvMaps);
    if (state->lvLaunchers) listViewDestroy(state->lvLaunchers);
    if (state->lvServerApps) listViewDestroy(state->lvServerApps);
    if (state->lvMapsStuck) listViewDestroy(state->lvMapsStuck);
    if (state->shcServerMonHistory) destroyStructHistCollection(state->shcServerMonHistory);
    sdFreeParseInfo(state->tpiMapConNetInfo);
    sdFreeParseInfo(state->tpiCrashedMapConNetInfo);
    sdFreeParseInfo(state->tpiLauncherConNetInfo);
    sdFreeParseInfo(state->tpiServerAppConNetInfo);
    sdFreeParseInfo(state->tpiEntConNetInfo);
    free(state);
}

void initServerMonitorState(ServerMonitorState *state)
{
    assert(state->eaMaps_data == NULL);
    state->eaMaps = &state->eaMaps_data;
    state->eaMapsStuck = &state->eaMapsStuck_data;
    state->eaLaunchers = &state->eaLaunchers_data;
    state->eaServerApps = &state->eaServerApps_data;
    state->eaEnts = &state->eaEnts_data;
    state->servers_in_trouble_changed=1;
    state->log_interval=2.0;
    state->log_enabled=false;
    state->connectButtons_enabled=true;
    state->connectButtons_viewplayers_enabled=true;
    state->checkAutoDelink_last_known_autodelink=300;
    state->autoDelinkButtons_enabled=true;
    state->log_button_enabled = false;
    state->stats.dbserver_in_trouble = 1;
}

static VarMap mapping[] = {
    {IDC_TXT_MSCOUNT,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.mscount, 0)},
    {IDC_TXT_LCOUNT,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.lcount, 0), },
    {IDC_TXT_PCOUNT,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.pcount, 0), },
    {IDC_TXT_PCOUNT_HERO,        VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.pcount_hero, 0), },
    {IDC_TXT_PCOUNT_VILLAIN,    VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.pcount_villain, 0), },
    {IDC_TXT_PCOUNTENTS,        VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.pcount_ents, 0), },
    {IDC_TXT_PCOUNTCON,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.pcount_connecting, 0), },
    {IDC_TXT_LOGINCOUNT,        VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.pcount_login, 0), },
    {IDC_TXT_QUEUEDCOUNT,        VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.pcount_queued, 0), },
    {IDC_TXT_QUEUECONNS,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.queue_connections, 0), },
    {IDC_TXT_MCOUNT,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.mcount, 0), },
    {IDC_TXT_ECOUNT,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.ecount, 0), },
    {IDC_TXT_SQLWB,                VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.sqlwb, 0), },
    {IDC_TXT_SQLTHROUGH,        VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.sqlthroughput, 0), },
    {IDC_TXT_SQLAVGLAT,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.sqlavglat, 0), 0, TOK_FORMAT_MICROSECONDS},
    {IDC_TXT_SQLWORSTLAT,        VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.sqlworstlat, 0), 0, TOK_FORMAT_MICROSECONDS},
    {IDC_TXT_SQLFOREIDLERATIO,    VMF_WRITE, 0, TOK_F32(ServerMonitorState, stats.sqlforeidleratio, 0), 0, TOK_FORMAT_PERCENT},
    {IDC_TXT_SQLBACKIDLERATIO,    VMF_WRITE, 0, TOK_F32(ServerMonitorState, stats.sqlbackidleratio, 0), 0, TOK_FORMAT_PERCENT},
    {IDC_TXT_LOGLAT,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.loglat, 0), 0, TOK_FORMAT_MICROSECONDS},
    {IDC_TXT_LOGBYTES,            VMF_WRITE, 0, TOK_AUTOINT(ServerMonitorState, stats.logbytes, 0), 0, TOK_FORMAT_BYTES},
    {IDC_TXT_LOGQCNT,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.logqcnt, 0), },
    {IDC_TXT_LOGQMAX,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.logqmax, 0), },
    {IDC_TXT_LOGSORTMEM,        VMF_WRITE, 0, TOK_AUTOINT(ServerMonitorState, stats.logsortmem, 0), 0, TOK_FORMAT_BYTES},
    {IDC_TXT_LOGSORTCAP,        VMF_WRITE, 0, TOK_AUTOINT(ServerMonitorState, stats.logsortcap, 0), 0, TOK_FORMAT_BYTES},
    {IDC_TXT_DBSERVER,            VMF_READ|VMF_WRITE, 0, TOK_FIXEDSTR(ServerMonitorState, dbserveraddr), },
    {IDC_TXT_SERVERVER,            VMF_WRITE, 0, TOK_FIXEDSTR(ServerMonitorState, stats.serverversion), },
    {IDC_TXT_GAMEVER,            VMF_WRITE, 0, TOK_FIXEDSTR(ServerMonitorState, stats.gameversion), },
    {IDC_TXT_AUTODELINKTIME,    VMF_READ, "ServerMonAutoDelinkTime",    TOK_INT(ServerMonitorState, stats.autodelinktime, 0), },
    {IDC_CHK_AUTODELINK,        VMF_READ, "ServerMonAutoDelink", TOK_BOOL(ServerMonitorState, stats.autodelink, 0), },
    {IDC_TXT_AVGCPU,            VMF_WRITE, 0, TOK_F32(ServerMonitorState, stats.avgCpu, 0), 0, TOK_FORMAT_PERCENT},
    {IDC_TXT_AVGCPU60,            VMF_WRITE, 0, TOK_F32(ServerMonitorState, stats.avgCpu60, 0), 0, TOK_FORMAT_PERCENT},
    {IDC_TXT_MAXCPU,            VMF_WRITE, 0, TOK_F32(ServerMonitorState, stats.maxCpu, 0), 0, TOK_FORMAT_PERCENT},
    {IDC_TXT_MAXCPU60,            VMF_WRITE, 0, TOK_F32(ServerMonitorState, stats.maxCpu60, 0), 0, TOK_FORMAT_PERCENT},
    {IDC_TXT_MINPHYS,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.minPhysAvail, 0), 0, TOK_FORMAT_KBYTES},
    {IDC_TXT_MINVIRT,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.minVirtAvail, 0), 0, TOK_FORMAT_KBYTES},
    {IDC_TXT_MAXSECS,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.maxSecondsSinceUpdate, 0), },
    {IDC_TXT_ARENASECS,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.arenaSecSinceUpdate, 0), },
    {IDC_TXT_STATSECS,            VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.statSecSinceUpdate, 0), },
    {IDC_TXT_ACCOUNTSECS,        VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.accountSecSinceUpdate, 0), },
    {IDC_TXT_HEROAUCTIONSECS,    VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.heroAuctionSecSinceUpdate, 0), },
    {IDC_TXT_VILLAINAUCTIONSECS,VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.villainAuctionSecSinceUpdate, 0), },
    {IDC_TXT_BEACONWAIT,        VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.beaconWaitSeconds, 0), },
    {IDC_TXT_MISSIONSECS,        VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.missionSecSinceUpdate, 0), },
    {IDC_TXT_TURNSTILESECS,        VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.turnstileSecSinceUpdate, 0), },
    {IDC_TXT_OVERLOADPROTECTION,VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.overloadProtection, 0), },
    {IDC_TXT_MAPSTARTREQUEST_COUNT,VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.dbserver_map_start_request_total, 0), },
    {IDC_TXT_MAPSTARTREQUEST_RATE,    VMF_WRITE, 0, TOK_F32(ServerMonitorState, stats.dbserver_avg_map_request_rate, 0), 0, },
    {IDC_TXT_ENT_WAITING_PEAK,    VMF_WRITE, 0, TOK_INT(ServerMonitorState, stats.dbserver_peak_waiting_entities, 0), 0, },
    {IDC_CHK_LOG,                VMF_READ, "ServerMonLogging", TOK_BOOL(ServerMonitorState, log_enabled, 0), },
    {IDC_TXT_LOGINTERVAL,        VMF_READ, "ServerMonLogInterval", TOK_F32(ServerMonitorState, log_interval, 0), },
};


#define listSetBgColor(id, color) \
    ListView_SetBkColor(GetDlgItem(hDlg, id), color); \
    ListView_SetTextBkColor(GetDlgItem(hDlg, id), color);

void fixConnectButtons(HWND hDlg, ServerMonitorState *state)
{
    if (svrMonConnected(state)) {
        if (state->connectButtons_enabled) {
            EnableWindow(GetDlgItem(hDlg, IDC_TXT_DBSERVER), FALSE);
            SetDlgItemTextA(hDlg, IDC_BTN_CONNECT, "Disconnect");
            listSetBgColor(IDC_LST_LAUNCHERS, GetSysColor(COLOR_WINDOW));
            listSetBgColor(IDC_LST_MS, GetSysColor(COLOR_WINDOW));
            UpdateWindow(GetDlgItem(hDlg, IDC_LST_LAUNCHERS));
            UpdateWindow(GetDlgItem(hDlg, IDC_LST_MS));
            //EnableWindow(GetDlgItem(hDlg, IDC_LST_LAUNCHERS), TRUE);
            //EnableWindow(GetDlgItem(hDlg, IDC_LST_MS), TRUE);
            //EnableWindow(GetDlgItem(hDlg, IDC_LST_MS_STUCK), TRUE);
            state->connectButtons_enabled=false;
        }
    } else {
        if (!state->connectButtons_enabled) {
            if (!g_shardmonitor_mode)
                EnableWindow(GetDlgItem(hDlg, IDC_TXT_DBSERVER), TRUE);
            if (g_shardmonitor_mode) {
                SetDlgItemTextA(hDlg, IDC_BTN_CONNECT, "Connect");
            } else {
                SetDlgItemTextA(hDlg, IDC_BTN_CONNECT, "Connect/Start");
            }
            listSetBgColor(IDC_LST_LAUNCHERS, GetSysColor(COLOR_BTNFACE));
            listSetBgColor(IDC_LST_MS, GetSysColor(COLOR_BTNFACE));
            UpdateWindow(GetDlgItem(hDlg, IDC_LST_LAUNCHERS));
            UpdateWindow(GetDlgItem(hDlg, IDC_LST_MS));
            //EnableWindow(GetDlgItem(hDlg, IDC_LST_LAUNCHERS), FALSE);
            //EnableWindow(GetDlgItem(hDlg, IDC_LST_MS), FALSE);
            //EnableWindow(GetDlgItem(hDlg, IDC_LST_MS_STUCK), FALSE);
            state->connectButtons_enabled=true;
        }
    }
    if (state->local_dbserver && state->stats.pcount >= 500 || g_shardmonitor_mode) {
        if (state->connectButtons_viewplayers_enabled) {
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_VIEWENTS), FALSE);
            state->connectButtons_viewplayers_enabled = false;
        }
    } else {
        if (!state->connectButtons_viewplayers_enabled) {
            EnableWindow(GetDlgItem(hDlg, IDC_BTN_VIEWENTS), TRUE);
            state->connectButtons_viewplayers_enabled = true;
        }
    }
}




static void checkLogHistory(HWND hDlg, ServerMonitorState *state)
{

    if (state->log_enabled) {

        svrMonLogHistory(state);

        if (!state->log_button_enabled) {
            EnableWindow(GetDlgItem(hDlg, IDC_TXT_LOGINTERVAL), TRUE);
            SetDlgItemInt(hDlg, IDC_TXT_LOGINTERVAL, state->log_interval, TRUE);
            state->log_button_enabled = true;
        }
    } else {
        if (state->log_button_enabled) {
            EnableWindow(GetDlgItem(hDlg, IDC_TXT_LOGINTERVAL), FALSE);
            state->log_button_enabled = false;
        }
    }
}

void checkAutoDelink(HWND hDlg, ServerMonitorState *state)
{
    int i;
    bool allow_auto_delink = !svrMonConnected(state) || !g_shardmonitor_mode && state->local_dbserver;

    if (!allow_auto_delink) {
        // Disable checkbox
        if (state->autoDelinkButtons_enabled) {
            EnableWindow(GetDlgItem(hDlg, IDC_TXT_AUTODELINKTIME), FALSE);
            state->checkAutoDelink_enabled=false;
            EnableWindow(GetDlgItem(hDlg, IDC_CHK_AUTODELINK), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_STATIC_AUTODELINK), FALSE);
            SendMessage(GetDlgItem(hDlg, IDC_CHK_AUTODELINK), BM_SETCHECK, 0, 0);
            state->autoDelinkButtons_enabled=false;
        }
    } else {
        // Enable checkbox
        if (!state->autoDelinkButtons_enabled) {
            EnableWindow(GetDlgItem(hDlg, IDC_CHK_AUTODELINK), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_STATIC_AUTODELINK), TRUE);
            state->autoDelinkButtons_enabled=true;
        }

        if (state->stats.autodelink) {
            if (state->stats.autodelinktime < 120) {
                state->stats.autodelinktime = state->checkAutoDelink_last_known_autodelink;
            } else {
                state->checkAutoDelink_last_known_autodelink = state->stats.autodelinktime;
            }

            for (i=0; i<eaSize(state->eaMapsStuck); i++) {
                MapCon *map = eaGet(state->eaMapsStuck, i);
                if (map->seconds_since_update > state->stats.autodelinktime &&
                    !map->starting && !notTroubleStatus(map->status))
                {
                    printf("Auto delinking map %d, %d seconds since last update", map->id, map->seconds_since_update);
                    svrMonDelink(state, map);
                    strcpy(map->status, "DELINKING...");
                }
            }
            if (!state->checkAutoDelink_enabled) {
                EnableWindow(GetDlgItem(hDlg, IDC_TXT_AUTODELINKTIME), TRUE);
                SetDlgItemInt(hDlg, IDC_TXT_AUTODELINKTIME, state->stats.autodelinktime, TRUE);
                state->checkAutoDelink_enabled = true;
            }
        } else {
            if (state->checkAutoDelink_enabled) {
                EnableWindow(GetDlgItem(hDlg, IDC_TXT_AUTODELINKTIME), FALSE);
                state->checkAutoDelink_enabled=false;
            }
        }
    }
}


void checkAutoReconnect(HWND hDlg, ServerMonitorState *state)
{
    if (!svrMonConnected(state) && state->connection_expected) {
        // auto re-connect
        SetDlgItemTextA(hDlg, IDC_BTN_CONNECT, "Reconnecting...");
        svrMonConnect(state, state->dbserveraddr, state->autostart_expected);
        state->autostart_expected = 0; // Only auto-start the first time, leave it to the Processes tab for the rest
    }
}





int rand_values=0;

void countStuckMaps(ListView *lv, void *structptr, void *data)
{
    MapCon *map = structptr;
    ServerMonitorState *state=data;
    if (map) {
        bool foundit=false;
        int j;
        for (j=0; j<eaSize(state->eaLaunchers); j++) {
            LauncherCon *launcher = state->eaLaunchers_data[j];
            U32 laddr = launcher->link->addr.sin_addr.S_un.S_addr;
            if (laddr == map->ip_list[0] || laddr == map->ip_list[1]) {
                //assert(!foundit); Will only happen if two launchers from the same IP are connected.
                foundit=true;
                launcher->num_crashed_mapservers++;
                state->stats.maxCrashedMaps = MAX(state->stats.maxCrashedMaps, launcher->num_crashed_mapservers);
                break;
            }
        }
    }
}

void setTextQuick(HWND hDlg, int id, char *str)
{
    char stemp[256];
    GetDlgItemTextA(hDlg, id, stemp, ARRAY_SIZE(stemp));
    if (stricmp(stemp, str)!=0) {
        SetDlgItemTextA(hDlg, id, str);
    }
}

void svrMonSetAutoDelink(ServerMonitorState *state, bool bSet, HWND hDlg)
{
    if( state->autoDelinkButtons_enabled ) {
        state->stats.autodelink = bSet;
        SendMessageA(GetDlgItem(hDlg, IDC_CHK_AUTODELINK), BM_SETCHECK, (bSet?1:0), 0);
    }
}

void svrMonTick(HWND hDlg, ServerMonitorState *state)
{
    int i;
    int count;
    getText(hDlg, state, mapping, ARRAY_SIZE(mapping));
    svrMonNetTick(state);
    if (!g_shardmonitor_mode && g_primaryServerMonitor) {
        svrMonShardMonCommSendUpdates();
    }
    state->stats.mscount_base = 0;
    state->stats.mscount_static = 0;
    state->stats.mscount_missions = 0;
    state->stats.mscount = listViewGetNumItems(state->lvMaps);
    state->stats.smscount = listViewGetNumItems(state->lvMapsStuck);
    state->stats.lcount = listViewGetNumItems(state->lvLaunchers);
    state->stats.lcount_suspended = 0;
    state->stats.lcount_suspended_manually = 0;
    state->stats.lcount_suspended_trouble = 0;
    state->stats.lcount_suspended_capacity = 0;
    state->stats.sacount = listViewGetNumItems(state->lvServerApps);
    state->stats.pcount=0;
    state->stats.pcount_connecting=0;
    state->stats.pcount_hero=0;
    state->stats.pcount_villain=0;
    state->stats.ecount=0;
    state->stats.mcount=0;
    state->stats.maxSecondsSinceUpdate=0;
    state->stats.maxCrashedMaps=0;
    state->stats.maxCrashedLaunchers=0;
    for (i=0; i<eaSize(state->eaLaunchers); i++) {
        LauncherCon *launcher = state->eaLaunchers_data[i];
        launcher->num_mapservers = 0;
        launcher->num_crashed_mapservers = 0;
        state->stats.maxCrashedLaunchers = MAX(state->stats.maxCrashedLaunchers, launcher->delinks);
        if (launcher->suspension_flags) {
            state->stats.lcount_suspended++;
            // launcher can be in multiple suspension states
            // count all of them but use the order below to choose the row color
            if (launcher->suspension_flags&kLaunchSuspensionFlag_Capacity)
            {
                state->stats.lcount_suspended_capacity++;
                listViewSetItemColor(state->lvLaunchers, launcher, LISTVIEW_DEFAULT_COLOR, RGB(223,152,200));
            }
            if (launcher->suspension_flags&kLaunchSuspensionFlag_Trouble)
            {
                state->stats.lcount_suspended_trouble++;
                listViewSetItemColor(state->lvLaunchers, launcher, COLOR_TROUBLE);
            }
            if (launcher->suspension_flags&kLaunchSuspensionFlag_Manual)
            {
                listViewSetItemColor(state->lvLaunchers, launcher, LISTVIEW_DEFAULT_COLOR, RGB(210,180,130));
            }
            if (launcher->suspension_flags&kLaunchSuspensionFlag_ServerMonitor)
            {
                listViewSetItemColor(state->lvLaunchers, launcher, LISTVIEW_DEFAULT_COLOR, RGB(210,200,139));
            }
            if ((launcher->suspension_flags&kLaunchSuspensionFlag_Manual) || (launcher->suspension_flags&kLaunchSuspensionFlag_ServerMonitor))
            {
                state->stats.lcount_suspended_manually++;
            }
        } else if (launcher->delinks > 1) {
            listViewSetItemColor(state->lvLaunchers, launcher, COLOR_TROUBLE);
        } else if (launcher->delinks > 0) {
            listViewSetItemColor(state->lvLaunchers, launcher, COLOR_TROUBLE_LIGHT);
        } else if (launcher->flags & kLauncherFlag_Monitor) {
            listViewSetItemColor(state->lvLaunchers, launcher, LISTVIEW_DEFAULT_COLOR, RGB(206,240,251));
        }
        else {
            listViewSetItemDefaultColor(state->lvLaunchers, launcher);
        }
    }

    for (i=0; i<eaSize(state->eaMaps); i++) {
        MapCon *map = state->eaMaps_data[i];
        if (map) {
            bool foundit=false;
            int j;
            state->stats.ecount+=map->num_ents;
            state->stats.mcount+=map->num_monsters;
            state->stats.pcount+=map->num_players;
            state->stats.pcount_hero+=map->num_hero_players;
            state->stats.pcount_villain+=map->num_villain_players;
            state->stats.pcount_connecting+=map->num_players_connecting;
            state->stats.maxSecondsSinceUpdate = MAX(state->stats.maxSecondsSinceUpdate, map->seconds_since_update);
            if (map->is_static) {
                state->stats.mscount_static++;
            } else if (strStartsWith(map->map_name, "Base")) {
                state->stats.mscount_base++;
            } else if (strStartsWith(map->map_name, "maps/Missions")) {
                state->stats.mscount_missions++;
            } else {
                state->stats.mscount_static++;
            }
            for (j=0; j<eaSize(state->eaLaunchers); j++) {
                LauncherCon *launcher = state->eaLaunchers_data[j];
                U32 laddr = launcher->link->addr.sin_addr.S_un.S_addr;
                if (laddr == map->ip_list[0] || laddr == map->ip_list[1]) {
                    //assert(!foundit);
                    if (foundit)
                        break; // Two launchers with the same IP?!
                    foundit=true;
                    launcher->num_mapservers++;
                }
            }
        }
    }

    listViewForEach(state->lvMapsStuck, countStuckMaps, state);
    state->stats.avgCpu=0;
    state->stats.avgCpu60=0;
    state->stats.maxCpu=0;
    state->stats.maxCpu60=0;
    state->stats.totalPhysUsed=0;
    state->stats.totalVirtUsed=0;
    state->stats.minPhysAvail=1<<31;
    state->stats.minVirtAvail=1<<31;
    state->stats.maxPhysAvail=0;
    state->stats.maxVirtAvail=0;
    state->stats.avgPhysAvail=0;
    state->stats.avgVirtAvail=0;
    count = 0;
    for (i=0; i<eaSize(state->eaLaunchers); i++) {
        LauncherCon *launcher = state->eaLaunchers_data[i];
        count++;
        state->stats.avgCpu += launcher->cpu_usage/100.;
        state->stats.avgCpu60 += launcher->remote_process_info.cpu_usage60;
        state->stats.maxCpu = MAX(state->stats.maxCpu, launcher->cpu_usage/100.);
        state->stats.maxCpu60 = MAX(state->stats.maxCpu60, launcher->remote_process_info.cpu_usage60);
        state->stats.totalPhysUsed += launcher->remote_process_info.mem_used_phys;
        state->stats.totalVirtUsed += launcher->remote_process_info.mem_used_virt;
        state->stats.minPhysAvail = MIN(state->stats.minPhysAvail, launcher->mem_avail_phys);
        state->stats.minVirtAvail = MIN(state->stats.minVirtAvail, launcher->mem_avail_virt);
        state->stats.maxPhysAvail = MAX(state->stats.maxPhysAvail, launcher->mem_avail_phys);
        state->stats.maxVirtAvail = MAX(state->stats.maxVirtAvail, launcher->mem_avail_virt);
        state->stats.avgPhysAvail += launcher->mem_avail_phys;
        state->stats.avgVirtAvail += launcher->mem_avail_virt;
    }
    if (count) {
        state->stats.avgCpu /= (F32)count;
        state->stats.avgCpu60 /= (F32)count;
        state->stats.avgPhysAvail /= count;
        state->stats.avgVirtAvail /= count;
    } else {
        state->stats.minPhysAvail = 0;
        state->stats.minVirtAvail = 0;
    }

    updateServerRelayStats(&(state->stats));

    if (rand_values) {
        state->stats.pcount = rand()%1000;
        state->stats.mcount = rand()%100;
        state->stats.mscount = rand()%100;
        state->stats.smscount = rand()%100;
        state->stats.lcount = rand()%100;
        state->stats.pcount_ents = rand()%100;
        state->stats.pcount_connecting = rand()%100;
        state->stats.ecount=rand()%100;
    }

    {
        char buf[1024];
        if (state->stats.lcount_suspended)
        {
            sprintf(buf, "Launchers: %d total, %d suspended (%d manual, %d trouble, %d capacity)", state->stats.lcount,
                        state->stats.lcount_suspended, state->stats.lcount_suspended_manually, state->stats.lcount_suspended_trouble, state->stats.lcount_suspended_capacity);
        }
        else
        {
            sprintf(buf, "Launchers: %d total", state->stats.lcount);
        }
        setTextQuick(hDlg, IDC_TXT_TITLE_LAUNCHERS, buf);
        sprintf(buf, "Mapservers: (%d total, %d static, %d mission, %d base)",
            state->stats.mscount, state->stats.mscount_static, state->stats.mscount_missions, state->stats.mscount_base);
        setTextQuick(hDlg, IDC_TXT_TITLE_MS, buf);
        sprintf(buf, "Stuck/Crashed Mapservers: (%d total, %d crashed, %d stuck, %d stuck starting, %d long tick)", state->stats.smscount, state->stats.sms_crashed_count, state->stats.sms_stuck_count, state->stats.sms_stuck_starting_count, state->stats.sms_long_tick_count);
        setTextQuick(hDlg, IDC_TXT_TITLE_SMS, buf);
        sprintf(buf, "Server Applications: (%d total, %d crashed)", state->stats.sacount, state->stats.sa_crashed_count);
        setTextQuick(hDlg, IDC_TXT_TITLE_SERVERAPPS, buf);
    }


    initText(hDlg, state, mapping, ARRAY_SIZE(mapping));
    fixConnectButtons(hDlg, state);
    if (state->servers_in_trouble_changed) {
        if (state->stats.servers_in_trouble) {
            ListView_SetBkColor(GetDlgItem(hDlg, IDC_LST_MS_STUCK), RGB(255,200,200));
            ListView_SetTextBkColor(GetDlgItem(hDlg, IDC_LST_MS_STUCK), RGB(255,200,200));
        } else {
            ListView_SetBkColor(GetDlgItem(hDlg, IDC_LST_MS_STUCK), RGB(220,255,220));
            ListView_SetTextBkColor(GetDlgItem(hDlg, IDC_LST_MS_STUCK), RGB(220,255,220));
        }
        InvalidateRect(hDlg, NULL, FALSE);

        state->servers_in_trouble_changed=0;
    }

    checkAutoDelink(hDlg, state);

    state->stats.secondsSinceDbUpdate = svrMonGetNetDelay(state);
    if (svrMonGetNetDelay(state) > 10 && svrMonGetNetDelay(state) %2) {
        setStatusBar(state->hStatusBar, 0 | SBT_POPOUT, "Last Update: %ds", state->stats.secondsSinceDbUpdate);
    } else {
        setStatusBar(state->hStatusBar, 0, "Last Update: %ds", state->stats.secondsSinceDbUpdate);
    }
    setStatusBar(state->hStatusBar, 1, "Send: %d B/s", svrMonGetSendRate(state));
    setStatusBar(state->hStatusBar, 2, "Recv: %d B/s", svrMonGetRecvRate(state));
    {
        int mscount = listViewDoOnSelected(state->lvMaps, NULL, NULL);
        setStatusBar(state->hStatusBar, 3, "%d selected mapserver%s", mscount, mscount==1?"":"s");
    }

    smoverloadUpdate();
    UpdateWindow(hDlg);

    checkAutoReconnect(hDlg, state);

    checkLogHistory(hDlg, state);

    serverMonitorListenTick();

    if (g_someDataOutOfSync) {
        static bool warnedUser = false;
        if (!warnedUser) {
            warnedUser = true;
            MessageBoxA(NULL, "You are connecting with a different version of Server Monitor than the shard you are connecting to.  Some data may be invalid.\n\nThis message will only appear once for server monitor connections.", "Server Monitor Warning", MB_ICONWARNING);
        }
    }
}

void smcbMsViewEnts(ListView *lv, void *structptr, ServerMonitorState *state)
{
    MapCon *con = (MapCon*)structptr;

    smentsSetFilterId(state, con->id);
    smentsShow(state);
}

static void onTimer(HWND hDlg, ServerMonitorState *state)
{
    /*
    static int timer=0;
#define maxfps 10
    static F32 spf = 1./maxfps;
    static int lock=0;
    if (lock!=0)
        return;
    lock++;



    if (timer==0) {
        timer = timerAlloc();
    }
    if (timerElapsed(timer) > spf) {
        timerStart(timer);
        svrMonTick(hDlg, state);
    }
    lock--;
    */
    static int timer = 0; // Shared between all instances!
    F32 elapsed;
#define maxfps 8
    static F32 spf = 1./maxfps;
    static int lock=0;
    if (lock!=0)
        return;
    lock++;

    if (timer==0) {
        timer = timerAlloc();
    }

    elapsed = timerElapsed(timer);
    if (elapsed > state->onTimer_lasttick + spf) {
        state->onTimer_lasttick += spf;
        if (elapsed > state->onTimer_lasttick + spf) {
            // We're more than a tick behind! Skip 'em!
            state->onTimer_lasttick = elapsed;
        }
        // do tick
        svrMonTick(hDlg, state);

    }

    lock--;
}

void svrMonConnectWrap(ServerMonitorState *state, HWND hDlg)
{
    if (!svrMonConnect(state, state->dbserveraddr, g_shardmonitor_mode?false:true)) {
        MessageBoxA(hDlg, "Error connecting to DBServer", "Error", MB_ICONWARNING);
        state->connection_expected = false;
    } else {
        state->connection_expected = true;
        BringWindowToTop(hDlg);
    }
    fixConnectButtons(hDlg, state);
}

char g_autoconnect_addr[128];

ServerMonitorState *allocServerMonitorState(HWND hDlg)
{
    static bool first=true;
    if (!g_shardmonitor_mode) {
        if (first) {
            first = false;
            g_state.stats.dbServerMonitor = &processmonitors[PME_DBSERVER];
            g_state.stats.launcherMonitor = &processmonitors[PME_LAUNCHER];
            return &g_state;
        }
        assert(0);
        return NULL;
    } else {
        ServerMonitorState *state = createServerMonitorState();
        return state;
    }
}



LRESULT CALLBACK DlgSvrMonProc (HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    int i=0;
    RECT rect;
    static LPPROPSHEETPAGE ps = 0;
    // Get the ServerMonitorState, if there isn't one, allocate one
    ServerMonitorState *state = (ServerMonitorState *) GetWindowLongPtr(hDlg, GWLP_USERDATA); 
    if (iMsg != WM_INITDIALOG && state == NULL) {
        return FALSE;
    }

    switch (iMsg)
    {
    case WM_INITDIALOG:
        {
            // Save the PROPSHEETPAGE information.
            ps = (PROPSHEETPAGE *)lParam;
            assert(!state);

            if (!state) {
                state = allocServerMonitorState(hDlg);
                SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR) state);
            }
            initServerMonitorState(state);
            serverMonitorListenSetMainDlg(hDlg);

            loadValuesFromReg(hDlg, state, mapping, ARRAY_SIZE(mapping));
            initText(hDlg, state, mapping, ARRAY_SIZE(mapping));

            if (state->lvMaps) listViewDestroy(state->lvMaps);
            state->lvMaps = listViewCreate();
            listViewInit(state->lvMaps, MapConNetInfo, hDlg, GetDlgItem(hDlg, IDC_LST_MS));

            if (state->lvLaunchers) listViewDestroy(state->lvLaunchers);
            state->lvLaunchers = listViewCreate();
            listViewInit(state->lvLaunchers, LauncherConNetInfo, hDlg, GetDlgItem(hDlg, IDC_LST_LAUNCHERS));

            if (state->lvServerApps) listViewDestroy(state->lvServerApps);
            state->lvServerApps = listViewCreate();
            listViewInit(state->lvServerApps, ServerAppConNetInfo, hDlg, GetDlgItem(hDlg, IDC_LST_SERVERAPPS));

            if (state->lvMapsStuck) listViewDestroy(state->lvMapsStuck);
            state->lvMapsStuck = listViewCreate();
            listViewInit(state->lvMapsStuck, CrashedMapConNetInfo, hDlg, GetDlgItem(hDlg, IDC_LST_MS_STUCK));

            getNameList("NameList");
            for (i=0; i<name_count; i++) 
                SendMessage(GetDlgItem(hDlg, IDC_TXT_DBSERVER), CB_ADDSTRING, 0, (LPARAM)namelist[i]);

            SetTimer(hDlg, 0, 10, NULL);

            // Status bar stuff
            state->hStatusBar = CreateStatusWindowA( WS_CHILD | WS_VISIBLE, "Status bar", hDlg, 1);
            {
                int temp[4];
                temp[0]=100;
                temp[1]=200;
                temp[2]=300;
                temp[3]=-1;
                SendMessageA(state->hStatusBar, SB_SETPARTS, ARRAY_SIZE(temp), (LPARAM)temp);
            }

            GetClientRect(hDlg, &rect); 
            // Initialize initial values
            doDialogOnResize(hDlg, (WORD)(rect.right - rect.left), (WORD)(rect.bottom - rect.top), IDC_ALIGNME, IDC_UPPERLEFT);
            SendMessageA(state->hStatusBar, SB_SETMINHEIGHT, 32, (LPARAM)0);
            SendMessageA(state->hStatusBar, WM_SIZE, 0, (LPARAM)0);

            if (g_autoconnect_addr[0]) {
                strcpy(state->dbserveraddr, g_autoconnect_addr);
                SetDlgItemTextA(hDlg, IDC_TXT_DBSERVER, g_autoconnect_addr);
                svrMonConnectWrap(state, hDlg);
                g_autoconnect_addr[0] = 0;
            }

            if (g_shardmonitor_mode) {
                SetDlgItemTextA(hDlg, IDCANCEL, "Close Tab");
            }
        }
        return FALSE;

    case WM_TIMER:
        onTimer(hDlg, state);
        break;
    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
            case IDCANCEL:
                KillTimer(hDlg, 0);
                SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR) NULL);
                if (g_shardmonitor_mode) {
                    EndDialog(hDlg, 0);
                    TabDlgRemoveTab(NULL, hDlg);
                    destroyServerMonitorState(state);
                } else {
                    EndDialog(hDlg, 0);
                    PostQuitMessage(0);
                }
                return TRUE;
            case IDOK:
            case IDC_BTN_CONNECT:
                getText(hDlg, state, mapping, ARRAY_SIZE(mapping));
                addNameToList("NameList", state->dbserveraddr);
                if (!svrMonConnected(state)) {
                    svrMonConnectWrap(state, hDlg);
                } else {
                    state->connection_expected = false;
                    svrMonDisconnect(state);
                }
                fixConnectButtons(hDlg, state);
                break;
            case IDC_BTN_FAKETROUBLE:
                state->stats.servers_in_trouble = !state->stats.servers_in_trouble;
                state->stats.dbserver_in_trouble = !state->stats.dbserver_in_trouble;
                state->servers_in_trouble_changed = 1;
                rand_values = 1;
                EnableWindow(GetDlgItem(hDlg, IDC_LST_MS_STUCK), TRUE);
                break;
            case IDC_BTN_MSDELINK:
                listViewDoOnSelected(state->lvMaps, smcbMsDelink, state);
                break;
            case IDC_BTN_MSSHOW:
                state->userData = (void*)(intptr_t)SW_SHOW;
                listViewDoOnSelected(state->lvMaps, (ListViewCallbackFunc)smcbMsShow, state);
                break;
            case IDC_BTN_MSHIDE:
                state->userData = (void*)(intptr_t)SW_HIDE;
                listViewDoOnSelected(state->lvMaps, (ListViewCallbackFunc)smcbMsShow, state);
                break;
            case IDC_BTN_MSRDC:
                listViewDoOnSelected(state->lvMaps, (ListViewCallbackFunc)smcbMsRemoteDesktop, state);
                break;
            case IDC_BTN_LRDC:
                listViewDoOnSelected(state->lvLaunchers, (ListViewCallbackFunc)smcbLRemoteDesktop, state);
                break;
            case IDC_BTN_LN_SUSPEND:
                listViewDoOnSelected(state->lvLaunchers, (ListViewCallbackFunc)smcbLauncherSuspend, state);
                break;
            case IDC_BTN_LN_RESUME:
                listViewDoOnSelected(state->lvLaunchers, (ListViewCallbackFunc)smcbLauncherResume, state);
                break;
            case IDC_BTN_SMSDELINK:
                listViewDoOnSelected(state->lvMapsStuck, smcbMsDelink, state);
                break;
            case IDC_BTN_SMSSHOW:
                state->userData = (void*)(intptr_t)SW_SHOW;
                listViewDoOnSelected(state->lvMapsStuck, (ListViewCallbackFunc)smcbMsShow, state);
                break;
            case IDC_BTN_SMSHIDE:
                state->userData = (void*)SW_HIDE;
                listViewDoOnSelected(state->lvMapsStuck, (ListViewCallbackFunc)smcbMsShow, state);
                break;
            case IDC_BTN_SMSKILL:
                listViewDoOnSelected(state->lvMapsStuck, (ListViewCallbackFunc)smcbMsKill, state);
                break;
            case IDC_BTN_SMSRDC:
                listViewDoOnSelected(state->lvMapsStuck, (ListViewCallbackFunc)smcbMsRemoteDesktop, state);
                break;
            case IDC_BTN_SARDC:
                listViewDoOnSelected(state->lvServerApps, (ListViewCallbackFunc)smcbSaRemoteDesktop, state);
                break;
            case IDC_BTN_SAKILL:
                listViewDoOnSelected(state->lvServerApps, (ListViewCallbackFunc)smcbSaKill, state);
                break;
            case IDC_BTN_SHUTDOWNALL:
                if (IDYES==MessageBoxA(hDlg, "Are you sure you want to shutdown all servers connected to this dbserver?", "Confirm Shutdown", MB_YESNO)) {
                    char *reason = promptGetString(g_hInst, hDlg, "Shutdown message:", "Servers are shutting down");
                    svrMonShutdownAll(state, reason);
                    state->connection_expected = false;
                }
                break;
            case IDC_BTN_RESETMISSION:
                if (IDYES==MessageBoxA(hDlg, "Are you sure you want to reset the link between this dbserver and the Mission Server?", "Confirm Reset", MB_YESNO)) {
                    svrMonResetMission(state);
                }
                break;
            case IDC_BTN_SMSREMOTEDEBUG:
                listViewDoOnSelected(state->lvMapsStuck, (ListViewCallbackFunc)smcbMsRemoteDebug, state);
                break;
            case IDC_BTN_SMSVIEWERROR:
                listViewDoOnSelected(state->lvMapsStuck, (ListViewCallbackFunc)smcbMsViewError, state);
                break;
            case IDC_BTN_VIEWENTS:
                if (!g_shardmonitor_mode) {
                    smentsSetFilterId(state, -1);
                    smentsShow(state);
                }
                break;
            case IDC_BTN_MSVIEWENTS:
                if (!g_shardmonitor_mode) {
                    listViewDoOnSelected(state->lvMaps, (ListViewCallbackFunc)smcbMsViewEnts, state);
                }
                break;
            case IDC_BTN_ADMINMESSAGE:
                svrMonSendAdminMessage(state, promptGetString(g_hInst, hDlg, "Admin message:", ""));
                break;
            case IDC_BTN_OVERLOADPROTECTION:
                if (!g_shardmonitor_mode) {
                    smoverloadShow(state);
                }
                break;

        }
        break;
    case WM_SIZE:
        {
            WORD w = LOWORD(lParam);
            WORD h = HIWORD(lParam);
            //printf("server res: %dx%d\n", w, h);
            doDialogOnResize(hDlg, w, h, IDC_ALIGNME, IDC_UPPERLEFT);
            SendMessage(state->hStatusBar, iMsg, wParam, lParam);
        }
        break;

    case WM_NOTIFY:
        {
            int idCtrl = (int)wParam;
            if(   listViewOnNotify(state->lvMaps, wParam, lParam, NULL)
               || listViewOnNotify(state->lvMapsStuck, wParam, lParam, updateCrashMsgText)
               || listViewOnNotify(state->lvLaunchers, wParam, lParam, NULL)
               || listViewOnNotify(state->lvServerApps, wParam, lParam, NULL))
               {
                   return TRUE;    // signal that we processed the message
               }
        }
        break;
    case WM_CTLCOLORSTATIC:
        {
            HWND idCtrl2 = (HWND)lParam;
            HDC hdc = (HDC)wParam;
            static struct
            {
                int idc_txt;
                int statoffset;
            } const color_responsive_timers[] = 
                {
                    {IDC_TXT_ACCOUNTSECS, OFFSETOF(ServerStats,accountSecSinceUpdate)},
                    {IDC_TXT_HEROAUCTIONSECS, OFFSETOF(ServerStats,heroAuctionSecSinceUpdate)},
                    {IDC_TXT_VILLAINAUCTIONSECS, OFFSETOF(ServerStats,villainAuctionSecSinceUpdate)},
                    {IDC_TXT_MISSIONSECS, OFFSETOF(ServerStats,missionSecSinceUpdate)},
                    {IDC_TXT_TURNSTILESECS, OFFSETOF(ServerStats,turnstileSecSinceUpdate)},
                };
            int i;

            for( i = 0; i < ARRAY_SIZE(color_responsive_timers); ++i ) 
            {
                int secs = *(int*)(((intptr_t)&state->stats) + color_responsive_timers[i].statoffset);
                
                if (idCtrl2==GetDlgItem(hDlg, color_responsive_timers[i].idc_txt)) 
                {
                    if (secs > 20) {
                        HGDIOBJ gdiobj = SelectObject(hdc,GetStockObject(DC_BRUSH));
                        SetTextColor(hdc, RGB(0,0,0));
                        if (secs % 2) {
                            SetBkColor(hdc, RGB(0xff, 0x20, 0x20));
                        } else {
                            SetBkMode(hdc, TRANSPARENT);
                        }
                        return (LRESULT)gdiobj;
                    }
                }
            }

            if (idCtrl2==GetDlgItem(hDlg, IDC_TXT_ARENASECS)) 
            {
                if (state->stats.arenaSecSinceUpdate > 20) {
                    HGDIOBJ gdiobj = SelectObject(hdc,GetStockObject(DC_BRUSH));
                    SetTextColor(hdc, RGB(0,0,0));
                    if (state->stats.arenaSecSinceUpdate % 2) {
                        SetBkColor(hdc, RGB(0xff, 0x20, 0x20));
                    } else {
                        SetBkMode(hdc, TRANSPARENT);
                    }
                    return (LRESULT)gdiobj;
                }
            } 
            if (idCtrl2==GetDlgItem(hDlg, IDC_TXT_STATSECS))
            {
                if (state->stats.statSecSinceUpdate > 20) {
                    HGDIOBJ gdiobj = SelectObject(hdc,GetStockObject(DC_BRUSH));
                    SetTextColor(hdc, RGB(0,0,0));
                    if (state->stats.statSecSinceUpdate % 2) {
                        SetBkColor(hdc, RGB(0xff, 0x20, 0x20));
                    } else {
                        SetBkMode(hdc, TRANSPARENT);
                    }
                    return (LRESULT)gdiobj;
                }
            }
            if (idCtrl2==GetDlgItem(hDlg, IDC_TXT_OVERLOADPROTECTION))
            {
                if (state->stats.overloadProtection > 0) {
                    HGDIOBJ gdiobj = SelectObject(hdc,GetStockObject(DC_BRUSH));
                    SetTextColor(hdc, RGB(0,0,0));
                    SetBkColor(hdc, RGB(0xff, 0x20, 0x20));
                    return (LRESULT)gdiobj;
                }
            }
//         if (idCtrl2==GetDlgItem(hDlg, IDC_TXT_BEACONWAIT)) 
//             {
//                 if (state->stats.beaconWaitSeconds > 120) {
//                     HGDIOBJ gdiobj = SelectObject(hdc,GetStockObject(DC_BRUSH));
//                     SetTextColor(hdc, RGB(0,0,0));
//                     if (state->stats.beaconWaitSeconds % 2) {
//                         SetBkColor(hdc, RGB(0xff, 0x20, 0x20));
//                     } else {
//                        SetBkMode(hdc, TRANSPARENT);
//                    }
//                     return (LRESULT)gdiobj;
//                 }
//             } 
        }
        break;
    }
    return FALSE;
}






