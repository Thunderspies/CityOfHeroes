#ifndef _SVRMONCOMM_H
#define _SVRMONCOMM_H

#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/textparser.h>
#include "comm_backend.h"

#define SVRMON_PROTOCOL_MAJOR_VERSION 20110804
#define SVRMON_PROTOCOL_MINOR_VERSION 20110804

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;

typedef struct
{
    NetLink        *link;
    StashTable htSentIds[MAX_CONTAINER_TYPES];
    StashTable htTemp;
    U32            sendPlayers:1; // Whether or not this client wants entities sent
    U32            svrmon_ready:1; // Whether or not this client is ready and wants anything sent
    U32            full_update:1;
    U32            sentPlayers:1; // Whether or not this client ever had entities sent
    U32            allow_new_container_types:1; // Whether or not it's a new client that allows more container types
} SvrMonClientLink;

void svrMonInit(void);
void svrMonUpdate(void);
void svrMonSendUpdates(void);

extern TokenizerParseInfo NetLinkInfo[];
extern TokenizerParseInfo MapConNetInfo[];
extern TokenizerParseInfo LauncherConNetInfo[];
extern TokenizerParseInfo EntConNetInfo[];
extern TokenizerParseInfo CrashedMapConNetInfo[];
extern TokenizerParseInfo ServerAppConNetInfo[];


#ifdef SVRMONCOMM_PARSE_INFO_DEFS
#define LAUNCHERCOMM_PARSE_INFO_DEFS

#include <utilitieslib/utils/ListView.h>
#include "container.h"
#include <utilitieslib/network/net_structdefs.h>
#include "../Launcher/src/performance.h"
#include <utilitieslib/utils/structNet.h>


TokenizerParseInfo NetLinkInfo[] = 
{
//    { "Type",            TOK_INT(NetLink, type), LVMakeParam(0,0)},
//    { "Socket",            TOK_INT(NetLink, socket), LVMakeParam(0,0)},
//    { "ConnectTime",    TOK_INT(NetLink, connectTime), LVMakeParam(0,0)},
    { "LastRecvTime",    TOK_MINBITS(28) | TOK_INT(NetLink, lastRecvTime, 0), 0, TOK_FORMAT_LVWIDTH(93) | TOK_FORMAT_FRIENDLYCPU},
    { "IP",                TOK_MINBITS(32) | TOK_INT(NetLink, addr.sin_addr.S_un.S_addr, 0), 0, TOK_FORMAT_LVWIDTH(100) | TOK_FORMAT_IP},
//    { "BytesSent",        TOK_INT(NetLink, totalBytesSent), LVMakeParam(0,0)},
//    { "BytesRead",        TOK_INT(NetLink, totalBytesRead), LVMakeParam(0,0)},
    {0}
};


TokenizerParseInfo BaseConNetInfo[] = 
{
    { "ID",                TOK_MINBITS(7) | TOK_INT(DbContainer, id, 0), 0, TOK_FORMAT_LVWIDTH(30)},
    { "OnSince",        TOK_MINBITS(28) | TOK_INT(DbContainer, on_since, 0), 0, TOK_FORMAT_LVWIDTH(93) | TOK_FORMAT_FRIENDLYSS2000},
    { 0 }
};

TokenizerParseInfo MapConNetInfo[] =
{
    { "BaseConNetInfo",    TOK_NULLSTRUCT(BaseConNetInfo) },
    { "PatchVersion",    TOK_FIXEDSTR(MapCon, patch_version), 0, TOK_FORMAT_LVWIDTH(100) | TOK_FORMAT_NOPATH},
    { "MapName",        TOK_FIXEDSTR(MapCon, map_name), 0, TOK_FORMAT_LVWIDTH(100) | TOK_FORMAT_NOPATH},
    { "#Players",        TOK_INT(MapCon, num_players, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "#Connecting",    TOK_INT(MapCon, num_players_connecting, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "HostName",        TOK_FIXEDSTR(MapCon, remote_process_info.hostname), 0, TOK_FORMAT_LVWIDTH(92)},
    { "PID",            TOK_MINBITS(12) | TOK_INT(MapCon, remote_process_info.process_id, 0), 0, TOK_FORMAT_LVWIDTH(42)},
    { "PhysUsed",        TOK_MINBITS(30) | TOK_INT(MapCon, remote_process_info.mem_used_phys, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "PhysPeak",        TOK_MINBITS(30) | TOK_INT(MapCon, remote_process_info.mem_peak_phys, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "PrivCommit",        TOK_MINBITS(30) | TOK_INT(MapCon, remote_process_info.mem_used_virt, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "PeakCommit",        TOK_MINBITS(30) | TOK_INT(MapCon, remote_process_info.mem_peak_virt, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "CpuUsage",        TOK_FLOAT_ROUNDING(FLOAT_HUNDRETHS) | TOK_F32(MapCon, remote_process_info.cpu_usage, 0), 0, TOK_FORMAT_LVWIDTH(45) | TOK_FORMAT_PERCENT},
    { "Cpu-60",            TOK_FLOAT_ROUNDING(FLOAT_HUNDRETHS) | TOK_F32(MapCon, remote_process_info.cpu_usage60, 0), 0, TOK_FORMAT_LVWIDTH(45) | TOK_FORMAT_PERCENT},
    { "Ticks/Sec",        TOK_FLOAT_ROUNDING(FLOAT_HUNDRETHS) | TOK_F32(MapCon, ticks_sec, 0), 0, TOK_FORMAT_LVWIDTH(45)},
    { "LongTick",        TOK_FLOAT_ROUNDING(FLOAT_ONES) | TOK_F32(MapCon, long_tick, 0), 0, TOK_FORMAT_LVWIDTH(45)},
    { "RelayCmds",        TOK_MINBITS(2) | TOK_INT(MapCon, num_relay_cmds, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "AvgPing",        TOK_MINBITS(9) | TOK_INT(MapCon, average_ping, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "MinPing",        TOK_MINBITS(9) | TOK_INT(MapCon, min_ping, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "MaxPing",        TOK_MINBITS(9) | TOK_INT(MapCon, max_ping, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "SecondsSinceUpdate",    TOK_MINBITS(6) | TOK_INT(MapCon, seconds_since_update, 0), 0, TOK_FORMAT_LVWIDTH(45)},
    { "#Monsters",        TOK_INT(MapCon, num_monsters, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "#Ents",            TOK_INT(MapCon, num_ents, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "#Heroes",        TOK_INT(MapCon, num_hero_players, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "#Villains",        TOK_INT(MapCon, num_villain_players, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "IP0",            TOK_MINBITS(32) | TOK_INT(MapCon, ip_list[0], 0), 0, TOK_FORMAT_IP | TOK_FORMAT_LVWIDTH(100)},
    { "IP1",            TOK_MINBITS(32) | TOK_INT(MapCon, ip_list[1], 0), 0, TOK_FORMAT_IP | TOK_FORMAT_LVWIDTH(100)},
    { "UDPPort",        TOK_MINBITS(12) | TOK_INT(MapCon, udp_port, 0), 0, TOK_FORMAT_LVWIDTH(53)},
    { "NetLink",        TOK_OPTIONALSTRUCT(MapCon, link, NetLinkInfo)},
    { "LauncherSecondsSinceUpdate",    TOK_MINBITS(6) | TOK_INT(MapCon, remote_process_info.seconds_since_update, 0), 0, TOK_FORMAT_LVWIDTH(45)},
    { "#Threads",        TOK_INT(MapCon, remote_process_info.thread_count, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "Starting",        TOK_MINBITS(1) | TOK_U8(MapCon, starting, 0), 0, TOK_FORMAT_LVWIDTH(25)},
    { 0 }
};

TokenizerParseInfo CrashedMapConNetInfo[] =
{
    { "Status",            TOK_FIXEDSTR(MapCon, status), 0, TOK_FORMAT_LVWIDTH(60)},
    { "BaseConNetInfo",    TOK_NULLSTRUCT(BaseConNetInfo)},
    { "OrigID",            TOK_MINBITS(7) | TOK_INT(MapCon, base_map_id, 0), 0, TOK_FORMAT_LVWIDTH(30)},
    { "MapName",        TOK_FIXEDSTR(MapCon, map_name), 0, TOK_FORMAT_NOPATH | TOK_FORMAT_LVWIDTH(100)},
    { "#Players",        TOK_INT(MapCon, num_players, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "#Connecting",    TOK_INT(MapCon, num_players_connecting, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "HostName",        TOK_FIXEDSTR(MapCon, remote_process_info.hostname), 0, TOK_FORMAT_LVWIDTH(92)},
    { "PID",            TOK_MINBITS(12) | TOK_INT(MapCon, remote_process_info.process_id, 0), 0, TOK_FORMAT_LVWIDTH(42)},
    { "PhysUsed",        TOK_MINBITS(30) | TOK_INT(MapCon, remote_process_info.mem_used_phys, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "PhysPeak",        TOK_MINBITS(30) | TOK_INT(MapCon, remote_process_info.mem_peak_phys, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "PrivCommit",        TOK_MINBITS(30) | TOK_INT(MapCon, remote_process_info.mem_used_virt, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "PeakCommit",        TOK_MINBITS(30) | TOK_INT(MapCon, remote_process_info.mem_peak_virt, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "CpuUsage",        TOK_FLOAT_ROUNDING(FLOAT_HUNDRETHS) | TOK_F32(MapCon, remote_process_info.cpu_usage, 0), 0, TOK_FORMAT_LVWIDTH(45) | TOK_FORMAT_PERCENT},
    { "Cpu-60",            TOK_FLOAT_ROUNDING(FLOAT_HUNDRETHS) | TOK_F32(MapCon, remote_process_info.cpu_usage60, 0), 0, TOK_FORMAT_LVWIDTH(45) | TOK_FORMAT_PERCENT},
    { "LongTick",        TOK_FLOAT_ROUNDING(FLOAT_ONES) | TOK_F32(MapCon, long_tick, 0), 0, TOK_FORMAT_LVWIDTH(45)},
    { "SecondsSinceUpdate",    TOK_MINBITS(6) | TOK_INT(MapCon, seconds_since_update, 0), 0, TOK_FORMAT_LVWIDTH(45)},
    { "#Monsters",        TOK_INT(MapCon, num_monsters, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "#Ents",            TOK_INT(MapCon, num_ents, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "IP0",            TOK_MINBITS(32) | TOK_INT(MapCon, ip_list[0], 0), 0, TOK_FORMAT_IP | TOK_FORMAT_LVWIDTH(100)},
    { "IP1",            TOK_MINBITS(32) | TOK_INT(MapCon, ip_list[1], 0), 0, TOK_FORMAT_IP | TOK_FORMAT_LVWIDTH(100)},
    { "UDPPort",        TOK_MINBITS(12) | TOK_INT(MapCon, udp_port, 0), 0, TOK_FORMAT_LVWIDTH(53)},
    { "#Threads",        TOK_INT(MapCon, remote_process_info.thread_count, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "Crash",            TOK_STRING(MapCon, raw_data, 0), 0, TOK_FORMAT_LVWIDTH(92)},
    { 0 }
};

TokenizerParseInfo LauncherConNetInfo[] =
{
    { "ID",                TOK_MINBITS(7) | TOK_INT(LauncherCon, id, 0), 0, TOK_FORMAT_LVWIDTH(30)},
    { "HostName",        TOK_FIXEDSTR(LauncherCon, remote_process_info.hostname), 0, TOK_FORMAT_LVWIDTH(92)},
    { "#db",            TOK_MINBITS(1) | TOK_INT(LauncherCon, num_connected_dbservers, 0), 0, TOK_FORMAT_LVWIDTH(32)},
    { "#M.raw",            TOK_MINBITS(1) | TOK_INT(LauncherCon, num_mapservers_raw, 0), 0, TOK_FORMAT_LVWIDTH(50)},
    { "#crash",            TOK_MINBITS(1) | TOK_INT(LauncherCon, num_crashes_raw, 0), 0, TOK_FORMAT_LVWIDTH(42)},
    { "#M all",            TOK_MINBITS(1) | TOK_INT(LauncherCon, num_mapservers_all_shards, 0), 0, TOK_FORMAT_LVWIDTH(42)},
    { "#M.s",            TOK_MINBITS(1) | TOK_INT(LauncherCon, num_static_maps, 0), 0, TOK_FORMAT_LVWIDTH(42)},
    { "#M.m",            TOK_MINBITS(1) | TOK_INT(LauncherCon, num_mission_maps, 0), 0, TOK_FORMAT_LVWIDTH(42)},
    { "#M db",            TOK_MINBITS(1) | TOK_INT(LauncherCon, num_mapservers, 0), 0, TOK_FORMAT_LVWIDTH(43)},
    { "Util.",            TOK_MINBITS(1) | TOK_INT(LauncherCon, host_utilization_metric, 0), 0, TOK_FORMAT_LVWIDTH(44) | TOK_FORMAT_PERCENT},
    { "Suspension",        TOK_MINBITS(1) | TOK_INT(LauncherCon, suspension_flags, 0), 0, TOK_FORMAT_LVWIDTH(48)},
    { "#CPU",            TOK_MINBITS(2)  | TOK_INT(LauncherCon, num_processors, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "CPU Use",        TOK_PRECISION(8) | TOK_INT(LauncherCon, cpu_usage, 0), 0, TOK_FORMAT_LVWIDTH(45) | TOK_FORMAT_PERCENT},
    { "Cpu-60",            TOK_FLOAT_ROUNDING(FLOAT_HUNDRETHS) | TOK_F32(LauncherCon, remote_process_info.cpu_usage60, 0), 0, TOK_FORMAT_LVWIDTH(45) | TOK_FORMAT_PERCENT},
    { "PhysUsed",        TOK_MINBITS(30) | TOK_INT(LauncherCon, remote_process_info.mem_used_phys, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
    { "PhysAvail",        TOK_MINBITS(30) | TOK_INT(LauncherCon, mem_avail_phys, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "Commit",            TOK_MINBITS(30) | TOK_INT(LauncherCon, remote_process_info.mem_used_virt, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
    { "CommitAvail",    TOK_MINBITS(30) | TOK_INT(LauncherCon, mem_avail_virt, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "CommitPeak",        TOK_MINBITS(30) | TOK_INT(LauncherCon, mem_peak_virt, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(70)},
    { "NetLink",        TOK_OPTIONALSTRUCT(LauncherCon, link, NetLinkInfo)},
    { "BytesSent/sec",    TOK_PRECISION(14) | TOK_INT(LauncherCon, net_bytes_sent, 0), },
    { "BytesRead/sec",    TOK_PRECISION(14) | TOK_INT(LauncherCon, net_bytes_read, 0), },
    { "OnSince",        TOK_MINBITS(28) | TOK_INT(LauncherCon, on_since, 0), 0, TOK_FORMAT_FRIENDLYSS2000 | TOK_FORMAT_LVWIDTH(93)},
    { "#CrashedMaps",    TOK_MINBITS(1) | TOK_INT(LauncherCon, num_crashed_mapservers, 0), 0, TOK_FORMAT_LVWIDTH(38)},
    { "Crashes",        TOK_MINBITS(1) | TOK_INT(LauncherCon, crashes, 0), 0, TOK_FORMAT_LVWIDTH(30)},
    { "Delinks",        TOK_MINBITS(1) | TOK_INT(LauncherCon, delinks, 0), 0, TOK_FORMAT_LVWIDTH(30)},
    { "Processes",        TOK_MINBITS(1) | TOK_INT(LauncherCon, process_count, 0), 0, TOK_FORMAT_LVWIDTH(38)},
    { "Threads",        TOK_MINBITS(1) | TOK_INT(LauncherCon, thread_count, 0), 0, TOK_FORMAT_LVWIDTH(50)},
    { "Handles",        TOK_MINBITS(1) | TOK_INT(LauncherCon, handle_count, 0), 0, TOK_FORMAT_LVWIDTH(50)},
    { "PagesInput/sec",    TOK_PRECISION(14) | TOK_INT(LauncherCon, pages_input_sec, 0), 0, TOK_FORMAT_LVWIDTH(75)},
    { "PagesOutput/sec",TOK_PRECISION(14) | TOK_INT(LauncherCon, pages_output_sec, 0), 0, TOK_FORMAT_LVWIDTH(75)},
    { "PageRead/sec",    TOK_PRECISION(14) | TOK_INT(LauncherCon, page_reads_sec, 0), 0, TOK_FORMAT_LVWIDTH(65)},
    { "PageWrite/sec",    TOK_PRECISION(14) | TOK_INT(LauncherCon, page_writes_sec, 0), 0, TOK_FORMAT_LVWIDTH(65)},
    { "Static Starting",TOK_MINBITS(1) | TOK_INT(LauncherCon, num_static_maps_starting, 0), 0, TOK_FORMAT_LVWIDTH(70)},
    { "Mission starting",TOK_MINBITS(1) | TOK_INT(LauncherCon, num_mission_maps_starting, 0), 0, TOK_FORMAT_LVWIDTH(70)},
    { "#Starts",        TOK_MINBITS(1) | TOK_INT(LauncherCon, num_starts_total, 0), 0, TOK_FORMAT_LVWIDTH(70)},
    { "Starts/s",        TOK_FLOAT_ROUNDING(FLOAT_HUNDRETHS) | TOK_F32(LauncherCon, avg_starts_per_sec, 0), 0, TOK_FORMAT_LVWIDTH(55)},
    { "Last Heuristic",    TOK_MINBITS(1) | TOK_INT(LauncherCon, heuristic, 0), 0, TOK_FORMAT_LVWIDTH(85)},
    { "Flags",            TOK_MINBITS(1) | TOK_INT(LauncherCon, flags, 0), 0, TOK_FORMAT_LVWIDTH(55)},
    { 0 }
};

TokenizerParseInfo ServerAppConNetInfo[] =
{
    { "BaseConNetInfo",    TOK_NULLSTRUCT(BaseConNetInfo)},
    { "Status",            TOK_FIXEDSTR(ServerAppCon, status), 0, TOK_FORMAT_LVWIDTH(73)},
    { "AppName",        TOK_FIXEDSTR(ServerAppCon, appname), 0, TOK_FORMAT_LVWIDTH(92)},
    { "HostName",        TOK_FIXEDSTR(ServerAppCon, remote_process_info.hostname), 0, TOK_FORMAT_LVWIDTH(92)},
    { "PID",            TOK_MINBITS(12) | TOK_INT(ServerAppCon, remote_process_info.process_id, 0), 0, TOK_FORMAT_LVWIDTH(42)},
    { "PhysUsed",        TOK_MINBITS(30) | TOK_INT(ServerAppCon, remote_process_info.mem_used_phys, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
    { "PFUsed",            TOK_MINBITS(30) | TOK_INT(ServerAppCon, remote_process_info.mem_used_virt, 0), 0, TOK_FORMAT_KBYTES | TOK_FORMAT_LVWIDTH(65)},
    { "CpuUsage",        TOK_FLOAT_ROUNDING(FLOAT_HUNDRETHS) | TOK_F32(ServerAppCon, remote_process_info.cpu_usage, 0), 0, TOK_FORMAT_LVWIDTH(45) | TOK_FORMAT_PERCENT},
    { "Cpu-60",            TOK_FLOAT_ROUNDING(FLOAT_HUNDRETHS) | TOK_F32(ServerAppCon, remote_process_info.cpu_usage60, 0), 0, TOK_FORMAT_LVWIDTH(45) | TOK_FORMAT_PERCENT},
    { "IP",                TOK_MINBITS(32) | TOK_INT(ServerAppCon, ip, 0), 0, TOK_FORMAT_IP | TOK_FORMAT_LVWIDTH(100)},
    { "Crashed",        TOK_MINBITS(1) | TOK_INT(ServerAppCon, crashed, 0), 0, TOK_FORMAT_LVWIDTH(17)},
    { "MonitorOnly",    TOK_MINBITS(1) | TOK_INT(ServerAppCon, monitor, 0), 0, TOK_FORMAT_LVWIDTH(17)},
    { 0 }
};

TokenizerParseInfo EntConNetInfo[] = 
{
    //Note: Do *not* add the "data" field to this, since we keep one copy of all of these fields for *every* entity
    //  for *each* servermonitor, this is a lot of data to be passed around, not to mention a lot of bandwidth to send
    //  verbatim and raw all the time.
    { "BaseConNetInfo",    TOK_NULLSTRUCT(BaseConNetInfo)},
    { "Auth",            TOK_FIXEDSTR(EntCon, account), 0, TOK_FORMAT_LVWIDTH(100)},
    { "Name",            TOK_FIXEDSTR(EntCon, ent_name), 0, TOK_FORMAT_LVWIDTH(100)},
    { "IP",                TOK_MINBITS(32) | TOK_INT(EntCon, client_ip, 0), 0, TOK_FORMAT_IP},
    { "MapId",            TOK_MINBITS(7)  | TOK_INT(EntCon, map_id, 0), 0, TOK_FORMAT_LVWIDTH(45)},
    { "StaticMapId",    TOK_MINBITS(7)  | TOK_INT(EntCon, static_map_id, 0), 0, TOK_FORMAT_LVWIDTH(45)},
    { "Teamup",            TOK_MINBITS(10) | TOK_INT(EntCon, teamup_id, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "Supergroup",        TOK_MINBITS(10) | TOK_INT(EntCon, supergroup_id, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "Taskforce",        TOK_MINBITS(10) | TOK_INT(EntCon, taskforce_id, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "League",            TOK_MINBITS(10) | TOK_INT(EntCon, league_id, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "Connected",        TOK_MINBITS(1)  | TOK_U8(EntCon, connected, 0), 0, TOK_FORMAT_LVWIDTH(25)},
    { 0 }
};


#endif

#endif
