#include <stdio.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/file.h>
#include "statservercomm.h"
#include "comm_backend.h"
#include "dbserver/servercfg.h"
#include <utilitieslib/network/netio.h>
#include "container.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/error.h>
#include "svrmoncomm.h"
#include <utilitieslib/utils/structNet.h>
#include "dbdispatch.h"
#include <utilitieslib/version/AppVersion.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include "launchercomm.h"
#include "mapcrashreport.h"
#include "loadBalancing.h"
#include "weeklyTFcfg.h"
#include "serverAutoStart.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/estring.h>
#include "beaconservercomm.h"
#include <utilitieslib/components/StashTable.h>
#include "dbmsg.h"
#include <utilitieslib/utils/log.h>
#include "turnstileDb.h"
#include "overloadProtection.h"
#include "launcher_common.h"


typedef struct
{
    LauncherCon    *container;
    U8            local_only;
    U8            sent_first_report;
    U8            cpu_count;
    U32            launcher_id;    // sequence ID of launcher

    struct {
        U32        last_update_time;
        
        struct {
            U32        clients;
            U32        request_servers;
            U32        master_servers;
            U32        mapserver_tsrs; // Not actually for the beaconizer
        } count;
    } beaconizer;
} LauncherLink;

BOOL g_heuristicdebug_enabled = 0;
int    g_total_map_start_requests = 0;

NetLinkList    launcher_links;
#define MAX_LAUNCHER_CLIENTS 500

// Recalculate the overload proteciton at least once every 5 seconds
#define OVERLOADPROTECTION_RECALCULATE_INTERVAL 5
static int  s_last_recalculate_overloadprotection_time = 0;

static int launcher_log=0;
void launcherCommLog(int log)
{
    launcher_log = log;
}

int launcherCount()
{
    int ret=0, i;
    NetLink *link;
    LauncherLink *client;
    for (i=0; i<launcher_links.links->size; i++) {
        link = launcher_links.links->storage[i];
        if (!link->connected)
            continue;
        client = link->userData;
        if (!client->sent_first_report)
            continue;
        ret++;
    }
    return ret;
}

int launcherCountStaticMapLaunchers()
{
    int ret=0, i;
    NetLink *link;
    LauncherLink *client;
    for (i=0; i<launcher_links.links->size; i++) {
        ServerRoleDescription *role;
        link = launcher_links.links->storage[i];
        if (!link->connected)
            continue;
        client = link->userData;
        if (!client->sent_first_report)
            continue;
        if (client->local_only)
            continue;
        role = getServerRole(link->addr.sin_addr.S_un.S_addr);
        if (!(role->primaryRole & ROLE_ZONE))
            continue;
        ret++;
    }
    return ret;
}

static void launcherSendStartProcess(NetLink* link, int container_id, const char* cmd, int cookie, LaunchType launch_type, int trackByExe, int monitorOnly)
{
    Packet            *pak = pktCreateEx(link, LAUNCHERQUERY_STARTPROCESS);
    LauncherLink    *client;

    client = link->userData;
    pktSendBitsPack(pak,1,container_id);
    pktSendString(pak,cmd);
    pktSendBitsPack(pak, 1, cookie);
    pktSendBitsPack(pak, 1, launch_type);        // launch type: static map, mission map, server app, etc
    pktSendBits(pak, 1, trackByExe ? 1 : 0);    // track by exe is used for launcher to monitor already running server apps
    pktSendBits(pak, 1, monitorOnly ? 1 : 0);
    pktSend(&pak,link);
}

static void launchBeaconClient(NetLink* link){
    RunServer rs = {0};
    char cmd[1000];

    sprintf(cmd,
            ".\\BeaconClient.exe"
            " -db %s"
            " -beaconclient %s"
            " -noencrypt"
            " -beaconproductionmode"
            " -beaconallownovodex"
            ,
            myHostname(),
            server_cfg.master_beacon_server);

    Strncpyt(rs.appname, "BeaconClient");
    rs.command = cmd;
    rs.ip = link->addr.sin_addr.S_un.S_addr;
    rs.track_by_id = 1;
    
    startServerApp(&rs);
}

static void launchMapServerTSR(NetLink* link){
    RunServer rs = {0};
    char cmd[1000];

    sprintf(cmd,
        ".\\MapServer.exe"
        " -db %s"
        " -tsr2"
        " -tsrdbconnect"
        ,
        myHostname());

    Strncpyt(rs.appname, "MapServer-TSR");
    rs.command = cmd;
    rs.ip = link->addr.sin_addr.S_un.S_addr;
    rs.track_by_id = 1;

    startServerApp(&rs);
}

static void launchRequestBeaconServer(NetLink* link){
    RunServer rs = {0};
    char cmd[1000];

    sprintf(cmd,
            ".\\BeaconServer.exe"
            " -db %s"
            " -beaconrequestserver %s"
            " -noencrypt"
            " -beaconproductionmode"
            " -beaconallownovodex"
            ,
            myHostname(),
            server_cfg.master_beacon_server);

    Strncpyt(rs.appname, "Request-BeaconServer");
    rs.command = cmd;
    rs.ip = link->addr.sin_addr.S_un.S_addr;
    rs.track_by_id = 1;
    
    startServerApp(&rs);
}

static void launchMasterBeaconServer(NetLink* link){
    RunServer rs = {0};
    char cmd[1000];

    sprintf(cmd,
            ".\\BeaconServer.exe"
            " -db %s"
            " -beaconmasterserver"
            " -beaconrequestcachedir %s"
            " -noencrypt"
            " -beaconproductionmode"
            " -beaconallownovodex"
            " -beaconnosymstore"
            ,
            myHostname(),
            server_cfg.beacon_request_cache_dir);

    Strncpyt(rs.appname, "Master-BeaconServer");
    rs.command = cmd;
    rs.ip = link->addr.sin_addr.S_un.S_addr;
    rs.track_by_id = 1;
    
    startServerApp(&rs);
}

static void launcherRequestBeaconServers(    ServerRoleDescription* role,
                                            LauncherLink* launcher,
                                            U32 master_server_ip)
{
    NetLink*    link = launcher->container->link;
    S32            requestServersNeeded = 0;
    S32            i;    
    
    // Check all matching roles for the maximum count.
    
    requestServersNeeded = 0;
    
    for(; role; role = getNextServerRole(role, link->addr.sin_addr.S_un.S_addr)){
        requestServersNeeded = max(requestServersNeeded, role->requestBeaconServerCount);
    }
    
    if(    !requestServersNeeded &&
        link->addr.sin_addr.S_un.S_addr == master_server_ip)
    {
        if(server_cfg.request_beacon_server_count < 0){
            requestServersNeeded = 2;
        }else{
            requestServersNeeded = server_cfg.request_beacon_server_count;
        }
    }
    
    requestServersNeeded = MIN(128, requestServersNeeded);

    for(i = launcher->beaconizer.count.request_servers; i < requestServersNeeded; i++){
        launchRequestBeaconServer(link);
    }
}

void launcherLaunchBeaconizers()
{
    static int lastTime;
    
    DbList*    serverapp_list;
    int        i;
    int        curTime = timerSecondsSince2000();
    U32        master_server_ip;

    if(    !server_cfg.master_beacon_server[0] ||
        curTime - lastTime < 5)
    {
        return;
    }
    
    lastTime = curTime;
    
    // Lookup the ip of the master beacon server.
    
    master_server_ip = ipFromString(server_cfg.master_beacon_server);
    
    if(!master_server_ip){
        return;
    }

    // Reset the beacon client count on each laucher.
    
    for(i = 0; i < launcher_list->num_alloced; i++){
        LauncherCon* containerLauncher = (LauncherCon*)launcher_list->containers[i];
        NetLink* link = containerLauncher->link;
        
        if(link && link->userData){
            LauncherLink* launcher = link->userData;
            
            ZeroStruct(&launcher->beaconizer.count);
        }
    } 
    
    // For each beacon client, increment the count on its launcher.
    
    serverapp_list = dbListPtr(CONTAINER_SERVERAPPS);
    
    for(i = 0; i < serverapp_list->num_alloced; i++){
        ServerAppCon* containerApp = (ServerAppCon*)serverapp_list->containers[i];
        LauncherCon* containerLauncher;
        S32 isBeaconClient = 0;
        S32 isRequestBeaconServer = 0;
        S32 isMasterBeaconServer = 0;
        S32 isMapServerTSR = 0;
        
        if(!stricmp(containerApp->appname, "BeaconClient")){
            isBeaconClient = 1;
        }
        else if(!stricmp(containerApp->appname, "Request-BeaconServer")){
            isRequestBeaconServer = 1;
        }
        else if(!stricmp(containerApp->appname, "Master-BeaconServer")){
            isMasterBeaconServer = 1;
        }
        else if(!stricmp(containerApp->appname, "MapServer-TSR")){
            isMapServerTSR = 1;
        }
        else{
            continue;
        }
        
        containerLauncher = containerPtr(launcher_list, containerApp->launcher_id);
        
        if(containerLauncher){
            NetLink* link = containerLauncher->link;
        
            if(link && link->userData){
                LauncherLink* launcher = link->userData;
                
                if(isBeaconClient){
                    launcher->beaconizer.count.clients++;
                }
                else if(isRequestBeaconServer){
                    launcher->beaconizer.count.request_servers++;
                }
                else if(isMasterBeaconServer){
                    launcher->beaconizer.count.master_servers++;
                }
                else if(isMapServerTSR){
                    launcher->beaconizer.count.mapserver_tsrs++;
                }
            }
        }
    }

    // Launch missing request servers and clients on each launcher.

    for(i = 0; i < launcher_list->num_alloced; i++){
        LauncherCon*            containerLauncher = (LauncherCon*)launcher_list->containers[i];
        NetLink*                link = containerLauncher->link;
        LauncherLink*            launcher = link ? link->userData : NULL;
        ServerRoleDescription*    role;
        S32                        j;
        
        if(!launcher){
            continue;
        }

        if(    !launcher->cpu_count ||
            launcher->beaconizer.last_update_time - curTime < 5)
        {
            continue;
        }
        
        launcher->beaconizer.last_update_time = curTime;
        
        role = getServerRole(link->addr.sin_addr.S_un.S_addr);
        
        if(!role){
            continue;
        }
        
        // Start Master-BeaconServer.
        
        if(    !server_cfg.do_not_launch_master_beacon_server &&
            link->addr.sin_addr.S_un.S_addr == master_server_ip)
        {
            if(!launcher->beaconizer.count.master_servers){
                launchMasterBeaconServer(link);
            }
        }
            
        // Start BeaconClients.

        if(    !launcher->local_only &&
            !server_cfg.do_not_launch_beacon_clients)
        {
            // Check if the role of this launcher is "monitor", in which case ignore it.
            
            if(!(role->primaryRole & ROLE_MONITOR)){
                // Launch enough beacon clients to get to #CPUs+1.
                
                for(j = launcher->beaconizer.count.clients ; j < server_cfg.beacon_client_count; j++){
                    launchBeaconClient(link);
                }

                // Launch a single MapServer TSR
                // We could move this out so it is not inside the do_not_launch_beacon_clients, 
                //   it is only there for convenience
                if (launcher->beaconizer.count.mapserver_tsrs < 1 && !server_cfg.do_not_launch_mapserver_tsrs)
                {
                    launchMapServerTSR(link);
                }
            }
        }
        
        // Start Request-BeaconServers.
        
        launcherRequestBeaconServers(role, launcher, master_server_ip);
    } 
}

static void killBeaconizers(U32 ip)
{
    DbList* serverapp_list = dbListPtr(CONTAINER_SERVERAPPS);
    S32 i;
    
    for(i = 0; i < serverapp_list->num_alloced; i++){
        ServerAppCon* containerApp = (ServerAppCon*)serverapp_list->containers[i];
        S32 isBeaconClient = 0;
        S32 isRequestBeaconServer = 0;
        S32 isMasterBeaconServer = 0;
        
        if(containerApp->ip != ip){
            continue;
        }
        else if(!stricmp(containerApp->appname, "BeaconClient")){
            isBeaconClient = 1;
        }
        else if(!stricmp(containerApp->appname, "Request-BeaconServer")){
            isRequestBeaconServer = 1;
        }
        else if(!stricmp(containerApp->appname, "Master-BeaconServer")){
            isMasterBeaconServer = 1;
        }
        else{
            continue;
        }
        
        containerDelete(containerApp);
        i--;
    }
    
    beaconCommKillAtIP(ip);
}

int launchersStillConnecting()
{
    return launcherCount() - launcher_links.links->size;
}

static void handleGameVersion(Packet *pak,NetLink *link)
{
    LauncherLink* client = link->userData;
    char *launcher_version = pktGetString(pak);
    const char *local_version = getCompatibleGameVersion();
    int ret=0;
    Packet *pakout;

    if (strnicmp(local_version, "dev:", 4)==0 &&
        strnicmp(launcher_version, "dev:", 4)==0)
    {
        // Both dev versions, close enough
        ret = 1;
    } else {
        ret = 0==strcmp(local_version, launcher_version);
    }
    
    if(ret)
    {
        client->cpu_count = pktGetBitsPack(pak, 1);
    }
    
    pakout = pktCreateEx(link, LAUNCHERQUERY_GAMEVERSION);
    pktSendBits(pakout, 1, ret);
    LOG_OLD( "Launcher (%d CPUs) regisetered %s with version %s", client->cpu_count, ret?"successfully":"UNSUCCESSFULLY", launcher_version);
    pktSendString(pakout,local_version);
    pktSend(&pakout, link);

}

static void checkForOrphanedServerApps(U32 ip, int current_time)
{
    int i;
    DbList *serverapp_list = dbListPtr(CONTAINER_SERVERAPPS);
    ServerAppCon **deleteme=NULL;
    ServerAppCon *con;
    U32 ss2k = timerSecondsSince2000();
    
    for (i=0; i<serverapp_list->num_alloced; i++)
    {
        con = (ServerAppCon *)serverapp_list->containers[i];
        if (con->ip == ip)
        {
            if (con->lastUpdateTime != current_time &&    // Not in this update and
                (con->remote_process_info.process_id || // Running
                (ss2k - con->on_since) > 60)) // Or started more than a minute ago
            {
                // Abandoned!
                eaPush(&deleteme, con);
            }
        }
    }
    for (i=eaSize(&deleteme)-1; i>=0; i--) 
    {
        con = deleteme[i];
        //printf("Launcher on %s failed to report on ServerApp %d, pid %d\n", makeIpStr(ip), con->id, con->remote_process_info.process_id);
        LOG_OLD( "Launcher on %s failed to report on ServerApp %d, pid %d", makeIpStr(ip), con->id, con->remote_process_info.process_id);
        containerDelete(con);
    }
    eaDestroy(&deleteme);
}

/*
    Host utilization metric serves as a heuristic for machine load.
    We use this metric in making load balancing and overload decisions.
    Generally going to represent host load from 0-200+ (i.e. percent)
    where 100% is going to be the machine hopefully humming along at
    100% on useful work. If it is actually overloaded and doing a lot
    of paging, etc, then the numbers scale up past 100.
    This does not account for configuration, environmental, or load factors which
    would lead to an outright suspension of launching on a given host
    those are considered at launcher selection time.
*/
static U32 calc_host_utilization_metric(LauncherCon const* launcher_con)
{
    U32 utilization=0;
    F32 est_cpu_usage;            // for accumulating estimated usage
    int est_mem_avail_phys=0;    

    est_cpu_usage = launcher_con->cpu_usage * 0.01f;        // convert instantaneous CPU Usage percentage the OS gives us to decimal
    est_mem_avail_phys = launcher_con->mem_avail_phys/1024;    // convert from KB to MB for easier comparison w/ load_balance_config values

    //**
    // Adjust estimate of memory/cpu usage by how many map servers are in the process of starting.
    // Note that we are using totals provided by the launcher for its host so
    // this captures the counts for all dbservers using this launcher (which is more appropriate for load balancing)

    // You could argue that this is pretty dubious because you don't know what phase of startup
    // the starting maps are in (e.g. how much memory have they already claimed) and the effects of starting
    // will show up in cpu and paging load. But it does provide a means to bias selection of launchers
    // that aren't starting maps but that could be done more directly

    // CPU
    est_cpu_usage += launcher_con->num_static_maps_starting * load_balance_config.startingStaticCPUBias;
    est_cpu_usage += launcher_con->num_mission_maps_starting * load_balance_config.startingMissionCPUBias;
    if (load_balance_config.staticCPUBias > 0)
    {
        // staticCPUBias % bias for ready static maps, so if they're idle, they still count for something
        int num_static_maps_ready = launcher_con->num_static_maps - launcher_con->num_static_maps_starting;
        est_cpu_usage += num_static_maps_ready*load_balance_config.staticCPUBias;
    }

    // Memory
    // recall load_balance_config numbers are specified in MB
    est_mem_avail_phys -= launcher_con->num_static_maps_starting * load_balance_config.startingStaticMemBias;        // in MB
    est_mem_avail_phys -= launcher_con->num_mission_maps_starting * load_balance_config.startingMissionMemBias;        // in MB
    if (est_mem_avail_phys < 0)
        est_mem_avail_phys = 0;


    //**
    // Build the heuristic from measurements and estimates

    // If host is in the last XX MB of physical memory we can add a load penalty for that
    // which might be helpful in selecting between hosts.
    if ((unsigned)est_mem_avail_phys < load_balance_config.minAvailPhysicalMemory)
        est_cpu_usage += load_balance_config.minAvailPhysicalMemoryBias;

    // Instantaneous host CPU usage - > 0-100+, nominally. convert back to integer percentage (accounted for multiple CPUs)
    utilization = (int)(est_cpu_usage*100 + 0.5); // round up

    // Hard page faults give us an indication of significant host resources
    // which are ultimately not doing any useful work and significant stalls to processes
    if (load_balance_config.pagingLoadHigh > load_balance_config.pagingLoadLow)
    {
        U32 est_pages_sec = launcher_con->pages_input_sec + launcher_con->pages_output_sec;

        if (est_pages_sec > load_balance_config.pagingLoadLow)
        {
            F32 range = (F32)(load_balance_config.pagingLoadHigh - load_balance_config.pagingLoadLow);
            F32 delta = (est_pages_sec - load_balance_config.pagingLoadLow)/range;
            utilization += delta*100;

        }
    }

    return utilization;
}

// this function uses the host utilization as calculated from the last
// reported status as a basis for calculating a utilization metric for
// a particular launch
static U32 calc_launch_utilization(LauncherCon const* launcher_con, LaunchType launch_type)
{
    // start with host utilization as calculated from last update by host
    // we keep that pristine as we want to base utilization capacity overload on
    // last status update.
    U32 heuristic = launcher_con->host_utilization_metric;

    // account for maps we've started since last update
    // @todo should we just include this in the host utilization calc and refresh that before each launch?
    // But most of that would be wasted calc work and it is nice to hold onto the utilization as derived from last status update
    // @todo these also aren't affecting the physical/virtual mem used at the moment
    if (launcher_con->num_launched_static_since_update || launcher_con->num_launched_mission_since_update)
    {
        heuristic += launcher_con->num_launched_static_since_update * (int)(load_balance_config.startingStaticCPUBias*100);
        heuristic += launcher_con->num_launched_mission_since_update * (int)(load_balance_config.startingMissionCPUBias*100);

        // even though maps started since last update from launcher were already be factored into
        // utilization we have an optional extra bias that can be used to try strongly deter launching
        // consecutively on the same launcher before a status update
        heuristic += load_balance_config.usedLauncherBias;
    }

    // Penalty for delinks? (too many and it will get completely 'trouble' suspended)
    if (launcher_con->delinks>1)
    {
        heuristic += load_balance_config.delinkLoadBias;
    }

    // Penalty for secondary role?
    // @todo secondary rules no longer appear relevant and should be phased out of the code and config files
    //        if (!isPrimary && isSecondary)
    //            heuristic += (int)(load_balance_config.secondaryRoleBias*100);

    return heuristic;
}

// reassess the status of this launcher and adjust the relevant suspension
// flags which indicate the global launch capabilities of the launcher
static void refresh_launcher_suspension_status( LauncherLink* client )
{
    LauncherCon* launcher_con = client->container;
    U32 total_maps;
    int est_mem_avail_virt;

    // Check for automated trouble suspension and expiration
    launcher_con->suspension_flags &= ~kLaunchSuspensionFlag_Trouble;
    if (launcher_con->trouble_suspend_expire) {
        if (timerSecondsSince2000() > launcher_con->trouble_suspend_expire) {
            // trouble suspension has expired
            launcher_con->trouble_suspend_expire = 0;
            launcher_con->delinks -= 2;
        }
        else
        {
            launcher_con->suspension_flags |= kLaunchSuspensionFlag_Trouble;
        }
    }

    // Precalculate the total maps
    total_maps = launcher_con->num_mapservers_raw;

    total_maps += launcher_con->num_launched_static_since_update;
    total_maps += launcher_con->num_launched_mission_since_update;

    // Precalculate the available virtual memory
    {
        int est_starting_static = launcher_con->num_static_maps_starting;
        int est_starting_mission = launcher_con->num_mission_maps_starting;
        est_mem_avail_virt = launcher_con->mem_avail_virt/1024;    // convert from KB to MB for easier comparison w/ load_balance_config values

        est_starting_static += launcher_con->num_launched_static_since_update;
        est_starting_mission += launcher_con->num_launched_mission_since_update;

        est_mem_avail_virt -= est_starting_static * load_balance_config.startingStaticMemBias;        // in MB
        est_mem_avail_virt -= est_starting_mission * load_balance_config.startingMissionMemBias;    // in MB
    }

    if (launcher_con->suspension_flags & kLaunchSuspensionFlag_Capacity)
    {
        U32 maxHostUtilization_lowWaterMark = load_balance_config.maxHostUtilization_lowWaterMark ? load_balance_config.maxHostUtilization_lowWaterMark : load_balance_config.maxHostUtilization;
        U32 maxMapservers_lowWaterMark = load_balance_config.maxMapservers_lowWaterMark ? load_balance_config.maxMapservers_lowWaterMark : load_balance_config.maxMapservers;
        U32 minAvailVirtualMemory_lowWaterMark = load_balance_config.minAvailVirtualMemory_lowWaterMark ? load_balance_config.minAvailVirtualMemory_lowWaterMark : load_balance_config.minAvailVirtualMemory;
        bool shouldClearCapacitySuspension = true;

        // All low-water marks have to be met before the suspension flag will be cleared
        if (maxHostUtilization_lowWaterMark > 0 && launcher_con->host_utilization_metric > maxHostUtilization_lowWaterMark)
        {
            shouldClearCapacitySuspension = false;
        }

        if (maxMapservers_lowWaterMark > 0 && total_maps >= maxMapservers_lowWaterMark)
        {
            shouldClearCapacitySuspension = false;
        }

        if (minAvailVirtualMemory_lowWaterMark > 0 && est_mem_avail_virt > (signed)minAvailVirtualMemory_lowWaterMark)
        {
            shouldClearCapacitySuspension = false;
        }

        if (shouldClearCapacitySuspension)
        {
            launcher_con->suspension_flags &= ~kLaunchSuspensionFlag_Capacity;
        }
    }
    else
    {
        // Are we in excess of host utilization target for stability and QoS?
        if(load_balance_config.maxHostUtilization > 0 && launcher_con->host_utilization_metric >= load_balance_config.maxHostUtilization)
        {
            launcher_con->suspension_flags |= kLaunchSuspensionFlag_Capacity;
        }

        // Don't exceed the maximum number of server processes allowed on a host machine that reflects
        // provisioning (e.g., avail network ports) and desired QoS and stability
        // @todo have separate limits for static and mission maps?
        // @todo To be completely correct we should be checking the launchers role here
        // and only checking on role mission|zone, but that seems like wasted energy
        if(load_balance_config.maxMapservers > 0 && total_maps >= load_balance_config.maxMapservers)
        {
            launcher_con->suspension_flags |= kLaunchSuspensionFlag_Capacity;
        }

        // Last XX MB of virtual memory
        // Server blades should be configured with healthy swap space for a high commit limit.
        // If we ever run out of commit space the server is going down.
        if (load_balance_config.minAvailVirtualMemory > 0 && est_mem_avail_virt < (signed)load_balance_config.minAvailVirtualMemory)
        {
            launcher_con->suspension_flags |= kLaunchSuspensionFlag_Capacity;
        }
    }
}

// Store heuristic for this traversal back into the container so ServerMonitor
// can display each launchers calculated heuristics for the last launch.
// @todo, is this useful?
void record_launcher_last_heuristic(NetLink* link, U32 heuristic)
{
    LauncherLink*    launcher_link    = link->userData;
    launcher_link->container->heuristic = heuristic;
}


static void overload_protection_update(void);

static void handleProcesses(Packet *pak,NetLink *link)
{
    RemoteProcessInfo    *rpi,dummy;
    int                    first=1;
    DbList                *map_list = dbListPtr(CONTAINER_MAPS);
    DbList                *serverapp_list = dbListPtr(CONTAINER_SERVERAPPS);
    LauncherCon            *launcher;
    LauncherLink        *client;
    int                    current_time = timerCpuSeconds();
    int                    is_manually_suspended;

    client = link->userData;

    if (client->sent_first_report == 0) 
    {
        client->sent_first_report = 1;
        LOG_OLD( "Got first launcher connect from %s", makeIpStr(link->addr.sin_addr.S_un.S_addr));
    }

    for(;;first=0)
    {
        int container_type;
        int container_id;
        int cookie;
        ServerCon *container=NULL;
        MapCon *map_con=NULL;
        ServerAppCon *serverapp_con=NULL;
        container_id = pktGetBitsPack(pak,1);
        if (container_id < 0)
            break;
        container_type = pktGetBitsPack(pak,1);
        cookie = pktGetBitsPack(pak,10);
        if (first)
            container = (ServerCon*)client->container;
        else
        {
            if (container_type == CONTAINER_MAPS)
            {
                container = containerPtr(map_list,container_id);
                map_con = (MapCon*)container;
                if (map_con && !(map_con->cookie == cookie || 
                    -map_con->cookie == cookie))
                {
                    // The cookies don't match, this is an update for the wrong map (or a ServerApp)
                    container = NULL;
                    map_con = NULL;
                }
            }
            if (container_type == CONTAINER_SERVERAPPS)
            {
                container = containerPtr(serverapp_list, container_id);
                serverapp_con = (ServerAppCon*)container;
                if (serverapp_con && !(serverapp_con->cookie == cookie))
                {
                    container = NULL;
                    serverapp_con = NULL;
                }
            }
        }
        if (container)
            rpi = &container->remote_process_info;
        else
            rpi = &dummy;

        // Ignore updates from launchers not owning this mapserver (i.e. when one mapserver is being debugged, and the new one starts up on
        //  another computer, both launchers will report status for a mapserver with the same map id)
        if (container && container->link && 
            container->link->addr.sin_addr.S_un.S_addr != link->addr.sin_addr.S_un.S_addr && // The container's links IPs aren't the same (launcher case)
            map_con &&
            map_con->ip_list[0] != link->addr.sin_addr.S_un.S_addr && // For mapservers, if one of the IPs match, use it!
            map_con->ip_list[1] != link->addr.sin_addr.S_un.S_addr)
        {
            rpi = &dummy;
        }

        rpi->process_id        = pktGetBitsPack(pak,1);
        rpi->mem_used_phys    = pktGetBitsPack(pak,1);
        rpi->mem_peak_phys    = pktGetBitsPack(pak,1);
        rpi->mem_used_virt    = pktGetBitsPack(pak,1);
//        rpi->mem_priv_virt    = pktGetBitsPack(pak,1);
        rpi->mem_peak_virt    = pktGetBitsPack(pak,1);
        rpi->thread_count    = pktGetBitsPack(pak,1);
        rpi->total_cpu_time    = pktGetBitsPack(pak,1);
        rpi->cpu_usage        = pktGetF32(pak);
        rpi->cpu_usage60    = pktGetF32(pak);

        rpi->last_update_time = current_time;    // mark when this rpi was last refreshed (so we know when information is stale)

        if (container) {
            if (container->link) {
                if (first || !map_con) {
                    // Update launchers based on link receive time
                    container->seconds_since_update = current_time - container->link->lastRecvTime;
                } else {
                    // Update mapservers based on when they last sent us a WHO update
                    if (map_con->lastRecvTime) {
                        map_con->seconds_since_update = current_time - map_con->lastRecvTime;
                    } else {
                        map_con->seconds_since_update = 0;
                    }
                }
                if (rpi->hostname[0]==0 && client->container->remote_process_info.hostname[0] &&
                    (container->link->addr.sin_addr.S_un.S_addr == link->addr.sin_addr.S_un.S_addr ||
                    map_con && map_con->ip_list[0] == link->addr.sin_addr.S_un.S_addr || // For mapservers, if one of the IPs match, use it!
                    map_con && map_con->ip_list[1] == link->addr.sin_addr.S_un.S_addr))
                {
                    strcpy(rpi->hostname, client->container->remote_process_info.hostname);
                }
            } else {
                if (map_con) {
                    if (map_con->lastRecvTime) {
                        map_con->seconds_since_update = current_time - map_con->lastRecvTime;
                    } else {
                        map_con->seconds_since_update = 0;
                    }
                } else {
                    container->seconds_since_update = 0;
                }
            }
            if (serverapp_con) {
                serverapp_con->lastUpdateTime = current_time;
            }
        }
    }

    // Launcher container and host information
    launcher = client->container;
    launcher->mem_total_phys    = pktGetBits(pak, 32);
    launcher->mem_avail_phys    = pktGetBits(pak, 32);
    launcher->mem_total_virt    = pktGetBits(pak, 32);    
    launcher->mem_avail_virt    = pktGetBits(pak, 32);
    launcher->mem_peak_virt        = pktGetBits(pak, 32);
    launcher->process_count        = pktGetBits(pak, 32);
    launcher->thread_count        = pktGetBits(pak, 32);
    launcher->handle_count        = pktGetBits(pak, 32);
    launcher->net_bytes_sent    = pktGetBits(pak, 32);
    launcher->net_bytes_read    = pktGetBits(pak, 32);
    launcher->pages_input_sec    = pktGetBits(pak, 32);
    launcher->pages_output_sec    = pktGetBits(pak, 32);
    launcher->page_reads_sec    = pktGetBits(pak, 32);
    launcher->page_writes_sec    = pktGetBits(pak, 32);
    launcher->cpu_usage            = pktGetBitsPack(pak, 8);
    launcher->num_processors    = pktGetBitsPack(pak, 5);
    is_manually_suspended        = pktGetBits(pak, 1);
    launcher->num_static_maps    = pktGetBitsPack(pak, 12);
    launcher->num_mission_maps    = pktGetBitsPack(pak, 12);
    launcher->num_server_apps    = pktGetBitsPack(pak, 12);
    launcher->num_static_maps_starting    = pktGetBitsPack(pak, 12);
    launcher->num_mission_maps_starting    = pktGetBitsPack(pak, 12);
    launcher->num_mapservers_all_shards = launcher->num_static_maps + launcher->num_mission_maps;    // total for servermon 'tracked' maps on host
    launcher->num_mapservers_raw        = pktGetBitsPack(pak, 12);
    launcher->num_starts_total            = pktGetBits(pak, 32);
    launcher->avg_starts_per_sec        = pktGetF32(pak);    // average start rate of processes per second experienced by launcher since last update
    launcher->num_connected_dbservers    = pktGetBitsPack(pak, 12);
    launcher->num_crashes_raw            = pktGetBitsPack(pak, 12);    // number of crashed processes on the launcher host

    // Once we get an updated status from a launcher we no longer need to track the number
    // of launches we've done.
    launcher->num_launched_static_since_update = 0;
    launcher->num_launched_mission_since_update = 0;

    // Calculate host utilization metric on each update so we can monitor this number dynamically
    // Must do this before updating suspension status.
    // (if load balancing configuration information was transmitted to the launchers they could do this
    // calculation which would reduce the amount of information that needs to be passed from the launchers
    // on each update...but we would need some other way to monitor that information then)
    launcher->host_utilization_metric = calc_host_utilization_metric(launcher);

    // update the launcher suspension flags so we know if this launcher is an available
    // launch target or if it is suspended due to trouble, or overload, etc.

    if (is_manually_suspended)    // launcher has been manually suspended?
        launcher->suspension_flags |= kLaunchSuspensionFlag_Manual;
    else
        launcher->suspension_flags &= ~kLaunchSuspensionFlag_Manual;

    refresh_launcher_suspension_status(client);    // update 'automatic' suspension status

    // N.B. that with multiple shards using the same launchers our host metrics and
    // usage counts can become outdated quickly in an environment with lots of map
    // starts between update intervals.

    // Check for ServerApps which were not reported on, assume dead!
    checkForOrphanedServerApps(link->addr.sin_addr.S_un.S_addr, current_time);

    // Update the overload protection
    overload_protection_update();
}

static void handleProcessClosed(Packet *pak,NetLink *link)
{
    ServerCon *container;
    MapCon *map_con=NULL;
    ServerAppCon *serverapp_con=NULL;
    int container_id = pktGetBitsPack(pak, 1);
    int container_type = pktGetBitsPack(pak, 1);
    int cookie = pktGetBitsPack(pak, 10);
    int process_id = pktGetBitsPack(pak, 1);

    if (container_type == CONTAINER_MAPS)
    {
        container = containerPtr(dbListPtr(CONTAINER_MAPS),container_id);
        map_con = (MapCon*)container;
        if (map_con && !(map_con->cookie == cookie || 
            -map_con->cookie == cookie))
        {
            // The cookies don't match, this is an update for the wrong map (or a ServerApp)
            container = NULL;
            map_con = NULL;
        }
        if (map_con && !map_con->is_static)
        {
            if (map_con->mission_info && map_con->mission_info[0] == 'E')
                turnstileDBserver_handleCrashedIncarnateMap(container_id);
        }        
        // We don't actually care about maps, they get handled elsewhere
    }
    if (container_type == CONTAINER_SERVERAPPS)
    {
        container = containerPtr(dbListPtr(CONTAINER_SERVERAPPS), container_id);
        serverapp_con = (ServerAppCon*)container;
        if (serverapp_con && !(serverapp_con->cookie == cookie))
        {
            container = NULL;
            serverapp_con = NULL;
        }
        if (serverapp_con)
            containerDelete(serverapp_con);
    }
}

static void handleProcessCrashed(Packet *pak,NetLink *link)
{
    ServerCon *container;
    MapCon *map_con=NULL;
    ServerAppCon *serverapp_con=NULL;
    int container_id = pktGetBitsPack(pak, 1);
    int container_type = pktGetBitsPack(pak, 1);
    int cookie = pktGetBitsPack(pak, 10);
    int process_id = pktGetBitsPack(pak, 1);
    int unhandled_crash = pktGetBitsPack(pak, 1);

    if (container_type == CONTAINER_MAPS)
    {
        container = containerPtr(dbListPtr(CONTAINER_MAPS),container_id);
        map_con = (MapCon*)container;
        if (map_con && !(map_con->cookie == cookie || 
            -map_con->cookie == cookie))
        {
            // The cookies don't match, this is an update for the wrong map (or a ServerApp)
            container = NULL;
            map_con = NULL;
        }
        if (map_con && !map_con->is_static)
        {
            if (map_con->mission_info && map_con->mission_info[0] == 'E')
                turnstileDBserver_handleCrashedIncarnateMap(container_id);
        }    
        if (unhandled_crash) 
        {
            handleCrashedMapReport(container_id, process_id, cookie, strdup("UNHANDLED CRASH"), link->addr.sin_addr.S_un.S_addr);
        }
        LOG_OLD(  "Launcher on %s reported map %d, pid %d crashed (%s)", makeIpStr(link->addr.sin_addr.S_un.S_addr), container_id, process_id, unhandled_crash?"UNHANDLED":"Handled");
    }
    if (container_type == CONTAINER_SERVERAPPS)
    {
        container = containerPtr(dbListPtr(CONTAINER_SERVERAPPS), container_id);
        serverapp_con = (ServerAppCon*)container;
        if (serverapp_con && !(serverapp_con->cookie == cookie))
        {
            container = NULL;
            serverapp_con = NULL;
        }
        // Flag it as crashed
        // report to ServerMonitor (automatically)
        // TODO: close sub-system's NetLinks,
        // TODO: restart in X seconds?  Allow ServerMonitor to delink?
        if (serverapp_con) {
            serverapp_con->crashed = 1 + unhandled_crash;
        }
        LOG_OLD_ERR("Launcher on %s reported ServerApp %d, pid %d crashed", makeIpStr(link->addr.sin_addr.S_un.S_addr), container_id, process_id);
    }
}

static int processLauncherMsg(Packet *pak,int cmd, NetLink *link)
{
    LauncherLink *client = link->userData;

    switch(cmd)
    {
        xcase LAUNCHERANSWER_PROCESSES:
            handleProcesses(pak,link);
        xcase LAUNCHERANSWER_LOCALONLY:
            client->local_only = 1;
            client->container->flags |= kLauncherFlag_Monitor;
        xcase LAUNCHERANSWER_GAMEVERSION:
            handleGameVersion(pak,link);
        xcase LAUNCHERANSWER_PROCESS_CLOSED:
            handleProcessClosed(pak,link);
        xcase LAUNCHERANSWER_PROCESS_CRASHED:
            handleProcessCrashed(pak,link);
        xdefault:
            LOG_OLD("unknown command %d from launcher client\n",cmd);
    }
    svrMonUpdate(); // Send an update to all svrMonitors
    return 0;
}

// calculate a load heuristic for a prospective launch based on the requested metric mode
static U32 calc_launch_load_heuristic(LauncherCon const* launcher_con, LaunchType launch_type, int balance_mode_flags)
{
    U32 heuristic = 0;

    if (balance_mode_flags & LOADBALANCE_TOTAL_OCCUPANCY)
    {
        heuristic = launcher_con->num_mapservers_raw;    // currently running (tracked or not)
        // plus those we've started since last status update received from launcher
        heuristic += launcher_con->num_launched_mission_since_update;
        heuristic += launcher_con->num_launched_static_since_update;
    }
    else if (balance_mode_flags & LOADBALANCE_TYPE_OCCUPANCY)
    {
        // currently running of type plus those we've starts since last update
        if ( launch_type == LT_STATICMAP )
        {
            heuristic = launcher_con->num_static_maps + launcher_con->num_static_maps_starting + launcher_con->num_launched_static_since_update;
        }
        else if (launch_type == LT_MISSIONMAP)
        {
            heuristic = launcher_con->num_mission_maps + launcher_con->num_mission_maps_starting + launcher_con->num_launched_mission_since_update;
        }
    }
    else if (balance_mode_flags & LOADBALANCE_UTILIZATION)
    {
        heuristic = calc_launch_utilization(launcher_con, launch_type);
    }

    if (launcher_log || g_heuristicdebug_enabled) 
    {
        // Log the *original* data
        LOG_OLD( "\tcalc_launch_load_heuristic for %s = %d : type: %d mode: %d Utilization:%d%% CPU:%d%% Phys: %d Virt: %d Faults: %d SSM: %d SMM: %d SM: %d MM: %d",
            makeIpStr(launcher_con->link->addr.sin_addr.S_un.S_addr),
            heuristic,
            launch_type,
            balance_mode_flags,
            launcher_con->host_utilization_metric,
            launcher_con->cpu_usage,
            launcher_con->mem_avail_phys,
            launcher_con->mem_avail_virt,
            launcher_con->pages_input_sec + launcher_con->pages_output_sec,
            launcher_con->num_static_maps_starting,
            launcher_con->num_mission_maps_starting,
            launcher_con->num_static_maps,
            launcher_con->num_mission_maps);
    }

    return heuristic;
}

// Check for whether or not this launcher's role allows launching this kind of map here
static bool check_host_role( NetLink const* link, LaunchType launch_type, int* rIsPrimary, int* rIsSecondary )
{
    ServerRoleDescription const*  role = getServerRole(link->addr.sin_addr.S_un.S_addr);
    // @todo, make this easily human parseable
    ServerRoles needed_role =
            (launch_type==LT_MISSIONMAP)?ROLE_MISSION:
            (launch_type==LT_STATICMAP)?ROLE_ZONE:
            (launch_type==LT_SERVERAPP)?(ROLE_MISSION|ROLE_ZONE):0;

    bool isPrimary = (role->primaryRole&needed_role) != 0;
    bool isSecondary = (role->secondaryRole&needed_role) != 0;

    if (rIsPrimary)
        *rIsPrimary = isPrimary;
    if (rIsSecondary)
        *rIsSecondary = isSecondary;

    return isPrimary || isSecondary;
}

// Walk the list of launchers to determine the subset that can currently
// be used to perform a launch of this type.
// Returns the number of available launchers and the link indices
// will be stored in the supplied array.
// This routine also will engage overload protection if it is
// determined that the system has reached capacity.
static int audit_launcher_links(LaunchType launch_type, int* available_launcher_indices, int array_size )
{
    int numAvailableLaunchers = 0;
    int numAvailableLaunchers_OP = 0;
    int numImpossibleLaunchers_OP = 0;
    int i;

    // overload_protection_update() calls with launch_type=LT_UNKNOWN which will skip this check
    if (overloadProtection_DoNotStartMaps() && (launch_type==LT_MISSIONMAP || launch_type==LT_STATICMAP))
    {
        //LOG_OLD( __FUNCTION__": Cannot start map. Available launchers are overloaded.");
        return 0;
    }

    for(i=0;i<launcher_links.links->size;i++)
    {
        NetLink* link = launcher_links.links->storage[i];
        LauncherLink *client;
        bool rightRole = true;
        bool countForOP = true;

        if (!link->connected)    // no connection, can't launch
            continue;

        client = link->userData;

        // Clear out last stored heuristic into the container so ServerMonitor
        // can display each launchers calculated heuristics for the last launch.
        // @todo, is this useful?
        client->container->heuristic = -1;

        // private local launcher for performance monitoring, launching not allowed
        if (client->local_only)
            continue;

        // If we have not received a first status from the launcher don't launch on it yet
        if (!client->sent_first_report)
            continue;

        // does the launcher's configured host role allow this type of launch?
        if (!check_host_role(link, launch_type, NULL, NULL))
            rightRole = false;

        // for overload protection, check mission and static maps
        if (!check_host_role(link, LT_MISSIONMAP, NULL, NULL) &&
            !check_host_role(link, LT_STATICMAP, NULL, NULL))
        {
            countForOP = false;
        }

        // See if we can ignore this launcher
        if (!rightRole && !countForOP)
            continue;

        // the checks up until now have been related to configuration or
        // network environment and do not contribute to determining if
        // the service is 'at capacity' with regard to available launchers

        // the checks that follow do count for overload protection

        // has the launcher been manually or automatically suspended?
        refresh_launcher_suspension_status(client);
        if (client->container->suspension_flags)
        {
            if (countForOP)
                numImpossibleLaunchers_OP++;    // these count toward overload protection
            continue;
        }

        if (countForOP)
            numAvailableLaunchers_OP++;

        // checks are finished, this launcher is appropriate and available
        // count it and add it to the array of selectable launchers
        if (rightRole && array_size)
        {
            devassertmsg( numAvailableLaunchers < array_size, __FUNCTION__"overflow of available launcher array.");
            if (numAvailableLaunchers < array_size)
            {
                available_launcher_indices[numAvailableLaunchers] = i;
            }

            numAvailableLaunchers++;
        }
    }

    // Tell the overload protection logic how we are doing with Launchers
    overloadProtection_SetLauncherStatus(numAvailableLaunchers_OP, numImpossibleLaunchers_OP);
    s_last_recalculate_overloadprotection_time = timerSecondsSince2000();

    // Check again to see if overload protection was triggered above
    if (overloadProtection_DoNotStartMaps() && (launch_type==LT_MISSIONMAP || launch_type==LT_STATICMAP))
    {
        //LOG_OLD( __FUNCTION__": Cannot start map. Available launchers are overloaded.");
        return 0;
    }

    return numAvailableLaunchers;
}

static void overload_protection_update(void)
{
    audit_launcher_links(LT_UNKNOWN, NULL, 0);
}

void launcherOverloadProtectionTick(void)
{
    int nowTime = timerSecondsSince2000();

    // See if it has been too long since the overload protection was last checked.
    if ((nowTime - s_last_recalculate_overloadprotection_time) >= OVERLOADPROTECTION_RECALCULATE_INTERVAL)
    {
        // recalculate the overload protection
        overload_protection_update();
    }
}

// @todo using a simple static array and fixed size for speed and simplicity of implementation
// and minimal testing requirements
#define MAX_BALANCER_LAUNCHERS    200

// ask load balancer to determine a suitable host (if any) for this launch
static NetLink* balancer_select_launcher_link(LaunchType launch_type)
{
    NetLink*    selected_link = NULL;
    int available_launchers[MAX_BALANCER_LAUNCHERS];
    int num_available_launchers;
    int mode_flags;

    // audit the launchers and build up the collection of available suitable 
    // launchers that the balancer can select from.
    num_available_launchers = audit_launcher_links(launch_type, available_launchers, MAX_BALANCER_LAUNCHERS);

    // There may turn out to be zero such launchers if overload is in progress
    if (num_available_launchers == 0)
        return NULL;

    // If there is only one launcher to choose from then our strategy is simple...
    if (num_available_launchers == 1)
    {
        int launcher_index = available_launchers[0];
        selected_link = launcher_links.links->storage[launcher_index];
        record_launcher_last_heuristic(selected_link, 0);    // @todo, is this still useful for ServerMonitor?
        return selected_link;
    }

    //**
    // more than one launcher available...invoke the load balancer

    // what is are the configured balancing strategy flags for this type of request?
    if (launch_type == LT_MISSIONMAP)
        mode_flags = load_balance_config.balanceModeMission;
    else if (launch_type == LT_STATICMAP)
        mode_flags = load_balance_config.balanceModeZone;
    else
        mode_flags = LOADBALANCE_SEQUENTIAL;    // default balance mode, keep it simple (is this ever invoked for anything but maps? don't think so)

    // apply the balancing strategy
    if (mode_flags & LOADBALANCE_SEARCH)
    {
        // Search the available launchers for the one with the minimum heuristic.
        // What exactly is used to calculate the heuristic is determined by the mode flags
        U32    min_load = 0x7FFFFFFF;
        U32 load;
        int i;

        for(i=0;i<num_available_launchers;i++)
        {
            int                    launcher_index = available_launchers[i];
            NetLink*            link = launcher_links.links->storage[launcher_index];
            LauncherLink*        launcher_link = link->userData;
            LauncherCon*        launcher_con = launcher_link->container;
            
            // calculate the load heuristic for this particular launch
            load = calc_launch_load_heuristic(launcher_con, launch_type, mode_flags);

            record_launcher_last_heuristic(link, load);    // @todo, is this still useful for ServerMonitor?
            
            if (load < min_load)
            {
                selected_link = link;
                min_load = load;
            }
        }
    }
    else if (mode_flags & LOADBALANCE_RANDOM_CHOICE)
    {
        // Choose two available launchers at random
        // and then pick the one with the lowest cost metric
        // This can lead to significantly better expected maximum
        // occupancy than a pure random selection without
        // needing extensive host status communication. (~log log n)
        int                index_a        = rand()%num_available_launchers;
        NetLink*        link_a        = launcher_links.links->storage[available_launchers[index_a]];
        LauncherLink*    launcher_a    = link_a->userData;
        U32                cost_a        = calc_launch_load_heuristic(launcher_a->container, launch_type, mode_flags);
        int                index_b;
        NetLink*        link_b;
        LauncherLink*    launcher_b;
        U32                cost_b;
        bool            chose_a;

        // randomly select another host that is different from the first
        do 
        {
            index_b = rand()%num_available_launchers;
        } while (index_b == index_a);

        link_b        = launcher_links.links->storage[available_launchers[index_b]];
        launcher_b    = link_b->userData;
        cost_b        = calc_launch_load_heuristic(launcher_b->container, launch_type, mode_flags);

        if (cost_a == cost_b)
            chose_a = (rand()%2) != 0;        // flip a coin to resolve ties
        else
            chose_a = cost_a < cost_b;        // choose lowest cost link

        selected_link = chose_a ? link_a : link_b;

        record_launcher_last_heuristic(link_a, cost_a);    // @todo, is this still useful for ServerMonitor?
        record_launcher_last_heuristic(link_b, cost_b);    // @todo, is this still useful for ServerMonitor?
    }
    else if (mode_flags & LOADBALANCE_RANDOM)
    {
        int selected_index = rand()%num_available_launchers;
        int launcher_index = available_launchers[selected_index];
        selected_link = launcher_links.links->storage[launcher_index];
        record_launcher_last_heuristic(selected_link, 0);    // @todo, is this still useful for ServerMonitor?
    }
    else if (mode_flags & LOADBALANCE_SEQUENTIAL)
    {
        // for sequential load balancing we find the next launcher with
        // a launcher id higher than the last one we used. If we hit
        // the end of the launchers then we wrap around to the beginning of the
        // list.
        // Since launcher IDs are assigned monotonically this should give us
        // a means to round robin the available launchers in the face of
        // suspensions, disconnects, reconnects, etc.
        static U32 s_last_used_launcher_id;
        int selected_launcher_id = 0;
        int i;

        for(i=0;i<num_available_launchers;i++)
        {
            int                    launcher_index = available_launchers[i];
            NetLink*            link = launcher_links.links->storage[launcher_index];
            LauncherLink*        launcher_link = link->userData;

            if (launcher_link->launcher_id > s_last_used_launcher_id)
            {
                selected_launcher_id = launcher_link->launcher_id;
                selected_link = link;
                s_last_used_launcher_id = selected_launcher_id;
                break;
            }
        }

        // are we wrapping around to the front of the list?
        if (selected_launcher_id == 0)
        {
            int                launcher_index = available_launchers[0];
            NetLink*        link = launcher_links.links->storage[launcher_index];
            LauncherLink*    launcher_link = link->userData;

            s_last_used_launcher_id = launcher_link->launcher_id;
            selected_link = link;
        }

        record_launcher_last_heuristic(selected_link, 0);    // @todo, is this still useful for ServerMonitor?
    }
    else
    {
        devassertmsg(false, __FUNCTION__": unknown load balancing algorithm selection flags.");
    }

    return selected_link;
}

U32 next_cookie=1;

//------------------------------------------------------------
//  Helper for getting the best link
//----------------------------------------------------------
static NetLink* s_BestLauncherLink(U32 host_ip, LaunchType launch_type)
{
    NetLink* best_link = NULL;

    if (host_ip)
    {
        // use a specific host machine if possible, otherwise abort
        int i;
        // look for a connected launcher
        for(i=0;i<launcher_links.links->size;i++)
        {
            NetLink* link = launcher_links.links->storage[i];
            if (!link->connected)
                continue;
            if (link->addr.sin_addr.S_un.S_addr == host_ip)
                best_link = link;
        }
        if (!best_link)
        {
            LOG_OLD_ERR( "ERROR! Can't find launcher requested for IP: %s!\n",makeIpStr(host_ip));
        }
    }
    else
    {
        // ask the load balancer to select an appropriate host for this launch
        best_link = balancer_select_launcher_link(launch_type);

        // @todo find a better place for this because there will be unwanted
        // side effect of incrementing counters too many times if call is made
        // several times
        if (best_link)
        {
            LauncherLink* client = best_link->userData;
            // keep track of launches we make on this launcher so we can account for them
            // until the next update comes in from the launcher.
            // in general we should prefer not to launch on same launcher again until we get an update from it
            if (launch_type==LT_STATICMAP)
            {
                client->container->num_launched_static_since_update++;    
            }
            else if (launch_type==LT_MISSIONMAP)
            {
                client->container->num_launched_mission_since_update++;    
            }
        }

        if(launcher_log || g_heuristicdebug_enabled)
        {
            if (best_link)
            {
                LauncherLink* client = best_link->userData;
                LOG_OLD( "s_BestLauncherLink chose %s : Utilization: %d CPU: %d\n", makeIpStr(best_link->addr.sin_addr.S_un.S_addr), client->container->host_utilization_metric, client->container->cpu_usage);
            }
            else
            {
                LOG_OLD( "s_BestLauncherLink: Cannot start map. All %d launchers are unsuitable.\n", launcher_links.links->size );
            }
        }
    }

    return best_link;
}

int launcherCommStartServerProcess(const char *command, U32 host_ip, ServerAppCon *server_app, int trackByExe, int monitorOnly)
{
    LauncherLink    *client;
    NetLink            *best_link;

    if (command && !host_ip && (launcher_log || g_heuristicdebug_enabled))
    {
        LOG_OLD( "launcherCommStartServerProcess finding launcher for '%s'\n", command);
    }

    best_link = s_BestLauncherLink( host_ip, LT_SERVERAPP );

    if (!best_link)
    {
        LOG_OLD_ERR("ERROR! No launcher running on %s, can't start \"%s\"!\n", makeIpStr(host_ip), command);
        return 0;
    }

    // Generate the next cookie
    next_cookie++;
    if (next_cookie==0xffffff)
        next_cookie = 1;
    server_app->cookie = next_cookie;
    server_app->on_since = timerSecondsSince2000();

    launcherSendStartProcess(best_link, server_app->id, command, server_app->cookie, LT_SERVERAPP, trackByExe, monitorOnly);

    client = best_link->userData;
    server_app->launcher_id = client->container->id;
    strcpy(server_app->remote_process_info.hostname, client->container->remote_process_info.hostname);
    return 1;
}

int launcherCommStartProcess(const char* db_hostname, U32 host_ip, MapCon* map_con, bool preload_transient)
{
    NetLink            *best_link=0;
    LauncherLink    *client;

    static char*    cmd;
    LaunchType        launch_type;

    g_total_map_start_requests++;    // increment global count of map launch requests (not same as started, especially in overload)

    if (map_con && map_con->map_name && !host_ip && (launcher_log || g_heuristicdebug_enabled))
    {
        LOG_OLD( "launcherCommStartProcess finding launcher for '%s'\n", map_con->map_name);
    }

    launch_type = map_con->is_static?LT_STATICMAP:LT_MISSIONMAP;
    best_link = s_BestLauncherLink( host_ip, launch_type );
    
    if (!best_link)
    {
        LOG_OLD_ERR( "ERROR!  No launchers or all launchers at capacity or suspended! Can't start maps!\n");
        return 0;
    }

    if (launcher_log) 
    {
        LOG_OLD( "Decided on %s for map %d", makeIpStr(best_link->addr.sin_addr.S_un.S_addr), map_con->id);
    }

    // Generate the next cookie
    next_cookie++;
    if (next_cookie==0xffffff)
        next_cookie = 1;

    map_con->cookie = next_cookie;
    client = best_link->userData;
    estrPrintf(&cmd,".\\mapserver.exe -db %s -udp %d -map_id %d -launcher -cookie %d",
        db_hostname,-1/*client->port_id*/,map_con->id, map_con->cookie);
    if (map_con->transient) {
        estrConcatf(&cmd, " -transient");
    }
    if (server_cfg.assertMode) {
        estrConcatf(&cmd, " -assertmode %d", server_cfg.assertMode);
    }
    if (server_cfg.log_server[0]) {
        estrConcatf(&cmd, " -logserver %s", server_cfg.log_server);
    }
    if (server_cfg.no_stats) {
        estrConcatf(&cmd, " -nostats");
    }
    if (server_cfg.map_server_params[0]) {
        estrConcatf(&cmd, " %s", server_cfg.map_server_params);
    }
    if (server_cfg.diff_debug)
        estrConcatf(&cmd, " -diffdebug");
    if (server_cfg.use_cod)
        estrConcatf(&cmd, " -cod");
    if (server_cfg.max_level)
        estrConcatf(&cmd, " -maxLevel %d", server_cfg.max_level);
    if (!server_cfg.max_level && server_cfg.max_coh_level)
        estrConcatf(&cmd, " -maxCoHLevel %d", server_cfg.max_coh_level);
    if (!server_cfg.max_level && server_cfg.max_cov_level)
        estrConcatf(&cmd, " -maxCoVLevel %d", server_cfg.max_cov_level);
    if (server_cfg.complete_broken_tasks)
        estrConcatf(&cmd, " -completebrokentasks");
    if (server_cfg.master_beacon_server[0])
        estrConcatf(&cmd, " -beaconusemasterserver %s", server_cfg.master_beacon_server);
    if (eaSize(&server_cfg.blocked_map_keys))
    {
        int i;
        estrConcatf(&cmd, " -blockedmapkeys ");
        for(i = eaSize(&server_cfg.blocked_map_keys)-1; i >= 1; i--)
        {
            estrConcatf(&cmd, "%s,", server_cfg.blocked_map_keys[i]);
        }
        estrConcatCharString(&cmd, server_cfg.blocked_map_keys[0]);
    }
    if (server_cfg.xpscale != 1.0f)
    {
        estrConcatf(&cmd, " -xpscale %f", server_cfg.xpscale);
    }
    if (server_cfg.influencescale != 1.0f)
    {
        estrConcatf(&cmd, " -influencescale %f", server_cfg.influencescale);
    }
    if (server_cfg.prestigescale != 1.0f)
    {
        estrConcatf(&cmd, " -prestigescale %f", server_cfg.xpscale);
    }
    if (server_cfg.aescale != 1.0f)
    {
        estrConcatf(&cmd, " -aescale %f", server_cfg.aescale);
    }
    if (server_cfg.aeinfluencescale != 1.0f)
    {
        estrConcatf(&cmd, " -aeinfluencescale %f", server_cfg.aeinfluencescale);
    }
    if (server_cfg.aeprestigescale != 1.0f)
    {
        estrConcatf(&cmd, " -aeprestigescale %f", server_cfg.aeprestigescale);
    }
    if (server_cfg.ticketscale != 1.0f)
    {
        estrConcatf(&cmd, " -ticketscale %f", server_cfg.ticketscale);
    }
    if (server_cfg.client_logout_time != 30)
    {
        estrConcatf(&cmd, " -clientlogouttime %d", server_cfg.client_logout_time);
    }
    if (server_cfg.aggrocap > 17)
    {
        estrConcatf(&cmd, " -aggrocap %d", server_cfg.aggrocap);
    }
    else
    {
        estrConcatf(&cmd, " -aggrocap %d", 17);
    }
    if (server_cfg.targetcapmode)
    {
        estrConcatf(&cmd, " -targetcapmode %d", server_cfg.targetcapmode);
    }

    {
        WeeklyTFCfg *weeklyTF_cfg = WeeklyTFCfg_getCurrentWeek(0);
        if (weeklyTF_cfg && eaSize(&weeklyTF_cfg->weeklyTFTokens))
        {
            estrConcatStaticCharArray(&cmd, " -WeeklyTFTokens ");
            WeeklyTF_GenerateTokenString(weeklyTF_cfg, &cmd);
        }
    }

    if (preload_transient)
        estrConcatStaticCharArray(&cmd, " -preloadtransient");

    // 0 is English, 5 is German and 6 is French. We must always tell the new
    // MapServer which locale to use since it may be running on a machine
    // which has a different default locale than us.
    estrConcatf(&cmd, " -locale %d", server_cfg.locale_id);

    launcherSendStartProcess(best_link, map_con->id, cmd, map_con->cookie, launch_type, 0, 0);
    map_con->launcher_id = client->container->id;
    strcpy(map_con->remote_process_info.hostname, client->container->remote_process_info.hostname);
    map_con->starting = 1;
    map_con->lastRecvTime = timerCpuSeconds();
    containerBroadcast(CONTAINER_MAPS,map_con->id,0,0,0);
    return 1;
}

// Give launchers unique increasing numeric IDs
// These are used by the road-robin load balancing scheme
static U32 get_next_launcher_id(void)
{
    static U32 s_last_launcher_id;
    return s_last_launcher_id++;
}

static StashTable ghtDelinkCount = NULL;

static int connectCallback(NetLink *link)
{
    LauncherLink    *client;

    client = link->userData;
    client->container = containerAlloc(launcher_list,-1);
    client->container->active = 1;
    client->container->link = link;
    client->local_only = 0;
    client->launcher_id = get_next_launcher_id();

    strcpy(client->container->remote_process_info.hostname, makeHostNameStr(link->addr.sin_addr.s_addr));
    LOG_OLD( "new launcher link at %s (%s)", makeIpStr(link->addr.sin_addr.s_addr), client->container->remote_process_info.hostname);
    if (!ghtDelinkCount) {
        ghtDelinkCount = stashTableCreateInt(32);
    }
    if ( !stashIntFindInt(ghtDelinkCount, link->addr.sin_addr.S_un.S_addr, &client->container->delinks) )
        client->container->delinks = 0;
    client->container->crashes = client->container->delinks >> 16;
    client->container->delinks &= 0xffff;
    
    return 1;
}

static int disconnectCallback(NetLink *link)
{
    LauncherLink    *client;

    client = link->userData;
    LOG_OLD( "del launcher link at %s (%s)", makeIpStr(link->addr.sin_addr.s_addr), client->container->remote_process_info.hostname);
    stashIntAddInt( ghtDelinkCount, link->addr.sin_addr.S_un.S_addr, (client->container->delinks | client->container->crashes << 16), true);
    containerDelete(client->container);
    svrMonUpdate(); // Send an update to all svrMonitors
    
    killBeaconizers(link->addr.sin_addr.S_un.S_addr);
    
    return 1;
}

void launcherStatusCb(LauncherCon *container,char *buf)
{
    RemoteProcessInfo    *rpi = &container->remote_process_info;

    container->seconds_since_update = timerCpuSeconds() - container->link->lastRecvTime;
    sprintf(buf,"%d Ip:%-13s S:%d Mem:%4dM Load:%.2f %.2f",
        container->id,makeHostNameStr(container->link->addr.sin_addr.s_addr),container->seconds_since_update,
        rpi->mem_used_phys / 1024,rpi->cpu_usage,rpi->cpu_usage60);
}

void launcherCommInit()
{
    netLinkListAlloc(&launcher_links,MAX_LAUNCHER_CLIENTS,sizeof(LauncherLink),connectCallback);
    if (!netInit(&launcher_links,0,DEFAULT_DBLAUNCHER_PORT)) {
        printf("Error binding to port %d!\n", DEFAULT_DBLAUNCHER_PORT);
    }
    launcher_links.destroyCallback = disconnectCallback;
    NMAddLinkList(&launcher_links, processLauncherMsg);
}

int launcherCommExec(const char *cmd,U32 host_ip)
{
    NetLink            *link;
    int                i;

    for(i=0;i<launcher_links.links->size;i++)
    {
        link = launcher_links.links->storage[i];
        if (link->addr.sin_addr.S_un.S_addr == host_ip)
        {
            launcherSendStartProcess(link, 0, cmd, 0, LT_UNKNOWN, 0, 0);

            return 1;
        }
    }
    for(i=0;i<launcher_links.links->size;i++)
    {
        link = launcher_links.links->storage[i];
        if (link->addr.sin_addr.S_un.S_un_b.s_b4 == (host_ip>>24))
        {
            launcherSendStartProcess(link, 0, cmd, 0, LT_UNKNOWN, 0, 0);

            return 1;
        }
    }
    LOG_OLD_ERR( "ERROR! Can't find launcher requested for IP: %s!\n",makeIpStr(host_ip));
    return 0;


}

void launcherShutdown(NetLink *launcherLink)
{
    LauncherLink    *client = launcherLink->userData;
    DbList *map_list = dbListPtr(CONTAINER_MAPS);
    int i;

    // Delink all maps on same host machine, make sure to find maps that are starting and do something about them?

    for (i=0; i<map_list->num_alloced; i++) {
        MapCon* map_con = (MapCon *)map_list->containers[i];
        if (map_con->launcher_id != client->container->id)
            continue;
        dbDelinkMapserver(map_con->id);
    }

    // Delink Launcher
    netRemoveLink(launcherLink);
}


U32 LAUNCHER_SLOW_TICK_TIME=3; // Number of seconds which, if elapsed, defines the last tick as "slow" and therefore disables delink checking
int LAUNCHER_CATCHUP_TICKS=60; // Number of "ticks" on the DbServer to wait after a "slow" tick before delinking dead servers
int LAUNCHER_DELINK_TIME=60*5; // Number of seconds which, if elapsed without receiving data, causes a launcher and all associated maps to be delinked
bool delink_enabled=true;

static int slow_tick_countdown=0;
static int slowTickTimer = 0;

F32 last_db_tick_length=0;
F32 max_db_tick_length=0;
F32 dbTickLength()
{
    F32 ret = max_db_tick_length;
    max_db_tick_length=0;
    return ret;
}


void resetSlowTickTimer()
{
    if(slowTickTimer)
    {
        timerStart(slowTickTimer);
    }
}

void delinkDeadLaunchers()
{
    int i;
    U32 current_time;
    
    if (!delink_enabled)
        return;

    if (!slowTickTimer) {
        slowTickTimer = timerAlloc();
        timerStart(slowTickTimer);
        return;
    }
    
    // calculate the time between the last tick and this tick
    last_db_tick_length = timerElapsed(slowTickTimer);
    timerStart(slowTickTimer);
    max_db_tick_length = MAX(max_db_tick_length, last_db_tick_length);

    // Was this tick really slow?  This happens on major SQL stalls
    if (last_db_tick_length > LAUNCHER_SLOW_TICK_TIME) 
    {
        verbose_printf("%s Detected a slow tick (%1.2fs)\n", timerGetTimeString(), last_db_tick_length);
        slow_tick_countdown = LAUNCHER_CATCHUP_TICKS;
        return;
    }

    // Don't do anything if we're still in the buffer period around slow ticks
    if (slow_tick_countdown>0) {
        slow_tick_countdown--;
        if (slow_tick_countdown) {
            return;
        }
    }

    current_time = timerCpuSeconds();

    // Loop over all launchers and see if any aren't talking to us anymore.
    for (i=0; i<launcher_links.links->size; i++) {
        NetLink            *link;
        LauncherLink    *client;

        link = launcher_links.links->storage[i];
        client = link->userData;

        if (link->lastRecvTime) {
            client->container->seconds_since_update = current_time - link->lastRecvTime;
        } else {
            client->container->seconds_since_update = 0;
        }
        if (client->container->seconds_since_update >= LAUNCHER_DELINK_TIME) 
        {
            printf("%s Delinking slow/dead launcher on %s (%ds since update)\n", timerGetTimeString(), makeIpStr(link->addr.sin_addr.S_un.S_addr), client->container->seconds_since_update);
            client->container->delinks++;
            // Delink all maps on same host machine, make sure to find maps that are starting and do something about them?
            // Delink Launcher
            launcherShutdown(link);
        }
    }
}

static int max_allowed_consecutive_crashes=8; // If this many crashes in a row happen on the same launcher, suspend it

void suspendTroubledLauncher(LauncherCon *launcher)
{
    printf("%s Suspending launcher on %s for %1.1f hours, %d consecutive crashes from maps on that launcher\n", timerGetTimeString(), makeIpStr(launcher->link->addr.sin_addr.S_un.S_addr), load_balance_config.troubleSuspensionTime/3600.f, max_allowed_consecutive_crashes);
    LOG_OLD_ERR( "Suspending launcher %s for %d seconds", makeIpStr(launcher->link->addr.sin_addr.S_un.S_addr), load_balance_config.troubleSuspensionTime);

    launcher->delinks+=2;
    launcher->trouble_suspend_expire = timerSecondsSince2000() + load_balance_config.troubleSuspensionTime;
}

int launcherFindByIp(U32 host_ip)
{
    int i;
    NetLink            *link;
    for(i=0;i<launcher_links.links->size;i++)
    {
        link = launcher_links.links->storage[i];
        if (link->addr.sin_addr.S_un.S_addr == host_ip)
        {
            return i;
        }
    }
    return -1;
}


int launcherCommRecordCrash(U32 host_ip)
{
    NetLink            *link;
    LauncherLink    *client;
    int                i;
    static U32 last_launcher_ip=0;
    static int count_consecutive_crashes=0;

    LOG_OLD_ERR( "MapServer crashed %s", makeIpStr(host_ip));

    for(i=0;i<launcher_links.links->size;i++)
    {
        link = launcher_links.links->storage[i];
        if (link->addr.sin_addr.S_un.S_addr == host_ip)
        {
            client = link->userData;
            client->container->crashes++;
            if (link->addr.sin_addr.S_un.S_addr == last_launcher_ip) {
                count_consecutive_crashes++;
                if (count_consecutive_crashes==max_allowed_consecutive_crashes)
                    suspendTroubledLauncher(client->container);
            } else {
                count_consecutive_crashes=0;
                last_launcher_ip=link->addr.sin_addr.S_un.S_addr;
            }
            return 1;
        }
    }
    for(i=0;i<launcher_links.links->size;i++)
    {
        link = launcher_links.links->storage[i];
        if (link->addr.sin_addr.S_un.S_un_b.s_b4 == (host_ip>>24))
        {
            client = link->userData;
            client->container->crashes++;
            if (link->addr.sin_addr.S_un.S_addr == last_launcher_ip) {
                count_consecutive_crashes++;
                if (count_consecutive_crashes==max_allowed_consecutive_crashes)
                    suspendTroubledLauncher(client->container);
            } else {
                count_consecutive_crashes=0;
                last_launcher_ip=link->addr.sin_addr.S_un.S_addr;
            }
            return 1;
        }
    }
    return 0;
}

void launcherReconcile(const char *ipStr)
{
    NetLink *link;
    LauncherLink *client;
    U32 ip;
    int launcherIndex;

    ip = ipFromString(ipStr);
    if (ip == INADDR_NONE)
    {
        printf("%s Could not find IP address of launcher to reconcile: %s\n", timerGetTimeString(), ipStr);
        return;
    }

    stashIntAddInt(ghtDelinkCount, ip, 0, true);
    printf("%s Launcher IP has been given a fresh start: %s\n", timerGetTimeString(), ipStr);

    launcherIndex = launcherFindByIp(ip);
    if (launcherIndex == -1)
    {
        printf("%s Could not find active launcher to reconcile: %s\n", timerGetTimeString(), ipStr);
        return;
    }

    link = launcher_links.links->storage[launcherIndex];
    client = link->userData;
    if (client->container->trouble_suspend_expire == 0 && client->container->delinks <= 1) {
        printf("%s Active launcher is not in trouble, clearing negativity anyway: %s\n", timerGetTimeString(), ipStr);
    }

    client->container->trouble_suspend_expire = 0;
    client->container->delinks = 0;
    client->container->crashes = 0;
    printf("%s Active launcher has been given a fresh start: %s\n", timerGetTimeString(), ipStr);
}

static LauncherCon* launcherContainerFindByIp(const char *ipStr)
{
    NetLink *link;
    LauncherLink *client;
    U32 ip;
    int launcherIndex;

    ip = ipFromString(ipStr);
    if (ip == INADDR_NONE)
    {
        return NULL;
    }

    launcherIndex = launcherFindByIp(ip);
    if (launcherIndex == -1)
    {
        return NULL;
    }

    link = launcher_links.links->storage[launcherIndex];
    client = link->userData;
    return client->container;
}

// update current
static void send_launcher_control( NetLink* link, bool suspend )
{
    if (link && link->connected)
    {
        Packet *pak = pktCreateEx(link, LAUNCHERQUERY_CONTROL);
        pktSendBits(pak, 1, suspend);
        pktSend(&pak, link);
    }
}

void launcherCommandSuspend(const char *ipStr)
{
    LauncherCon* launcher = launcherContainerFindByIp(ipStr);

    if (launcher)
    {
        // this flag applies immediately to this dbserver, ie. the one the
        // server monitor is controlling.
        launcher->suspension_flags |= kLaunchSuspensionFlag_ServerMonitor;
        // However, we generally want to suspend the launcher from launching for
        // ANY shard so send a control message out to the launcher to have it
        // suspend itself.
        send_launcher_control( launcher->link, true );    // tell launcher to begin a manual suspension for everyone
    }
    else
        printf("%s : warning: "__FUNCTION__" Could not find launcher with IP address: %s\n", timerGetTimeString(), ipStr);
}

void launcherCommandResume(const char *ipStr)
{
    LauncherCon* launcher = launcherContainerFindByIp(ipStr);

    if (launcher)
    {
        // this flag applies immediately to this dbserver, ie. the one the
        // server monitor is controlling.
        launcher->suspension_flags &= ~kLaunchSuspensionFlag_ServerMonitor;
        send_launcher_control( launcher->link, false );    // tell launcher to end any manual suspension for everyone
    }
    else
        printf("%s : warning: "__FUNCTION__" Could not find launcher with IP address: %s\n", timerGetTimeString(), ipStr);
}

