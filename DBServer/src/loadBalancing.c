#include "loadBalancing.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/file.h>
#include "serverAutoStart.h"
#include <utilitieslib/utils/FileSystem.h>
#include <utilitieslib/utils/tokenstore.h>
#include <utilitieslib/utils/log.h>
#include "dbserver/servercfg.h"


LoadBalanceConfig load_balance_config;

StaticDefineInt    ParseRole[] =
{
    DEFINE_INT
    { "None",        ROLE_NONE},
    { "CityZone",    ROLE_ZONE},
    { "Mission",    ROLE_MISSION},
    { "Monitor",    ROLE_MONITOR},
    DEFINE_END
};

TokenizerParseInfo ParseRunServer[] = 
{
    { "Command",        TOK_STRING(RunServer, command,       0) },
    { "KillAndRestart", TOK_INT(RunServer, kill_and_restart, 0) },
    { "MonitorOnly",    TOK_INT(RunServer, monitor,          0) },
    { "TrackByID",      TOK_INT(RunServer, track_by_id,      0) },
    { "AppName",        TOK_STRING(RunServer,appname_cfg,    0) },
    { "End",            TOK_END },
    { "", 0, 0 }
};


TokenizerParseInfo ParseIPRange[] =
{
    {    "",         TOK_STRUCTPARAM | TOK_INT(ServerRoleDescription, iprange[0], 0), 0, TOK_FORMAT_IP },
    {    "",         TOK_STRUCTPARAM | TOK_INT(ServerRoleDescription, iprange[1], 0), 0, TOK_FORMAT_IP },
    {    "\n",       TOK_END },
    {    "", 0, 0 }
};

TokenizerParseInfo ParseServerRoleDescription[] =
{
    { "Host",            TOK_REDUNDANTNAME | TOK_INT(ServerRoleDescription, iprange[0], 0), 0, TOK_FORMAT_IP},
    { "HostRange",        TOK_EMBEDDEDSTRUCT(ServerRoleDescription, iprange[0], ParseIPRange) },
    { "PrimaryRole",    TOK_FLAGS(ServerRoleDescription, primaryRole, 0), ParseRole},
    { "SecondaryRole",    TOK_FLAGS(ServerRoleDescription, secondaryRole, 0), ParseRole},
    { "Server",            TOK_STRUCT(ServerRoleDescription, runServers, ParseRunServer) },
    { "RequestBeaconServerCount", TOK_INT(ServerRoleDescription,requestBeaconServerCount,0), },
    { "End",            TOK_END },
    { "", 0, 0 }
};

static StaticDefineInt load_balance_flags[] = {
    DEFINE_INT
    { "Sequential",        LOADBALANCE_SEQUENTIAL },
    { "Random",            LOADBALANCE_RANDOM },
    { "RandomChoice",    LOADBALANCE_RANDOM_CHOICE },
    { "Search",            LOADBALANCE_SEARCH },

    { "Utilization",    LOADBALANCE_UTILIZATION },
    { "TotalOccupancy",    LOADBALANCE_TOTAL_OCCUPANCY },
    { "TypeOccupancy",    LOADBALANCE_TYPE_OCCUPANCY },
    DEFINE_END
};

TokenizerParseInfo ParseLoadBalanceConfig[] = {
    { "ServerRole",            TOK_STRUCT(LoadBalanceConfig,serverRoles,ParseServerRoleDescription)},

    { "BalanceModeZone",    TOK_FLAGS(LoadBalanceConfig,balanceModeZone,0),load_balance_flags},
    { "BalanceModeMission",    TOK_FLAGS(LoadBalanceConfig,balanceModeMission,0),load_balance_flags},

    { "MinAvailPhysicalMemory",    TOK_INT(LoadBalanceConfig,minAvailPhysicalMemory,500), },
    { "MinAvailPhysicalMemoryBias",        TOK_F32(LoadBalanceConfig,minAvailPhysicalMemoryBias,0), },
    { "MinAvailVirtualMemory",    TOK_INT(LoadBalanceConfig,minAvailVirtualMemory,1000), },
    { "PagingLoadLow",    TOK_INT(LoadBalanceConfig,pagingLoadLow,0), },
    { "PagingLoadHigh",    TOK_INT(LoadBalanceConfig,pagingLoadHigh,0), },
    { "TroubleSuspensionTime",        TOK_INT(LoadBalanceConfig,troubleSuspensionTime,0), },
    { "MaxMapservers",        TOK_INT(LoadBalanceConfig,maxMapservers,200), },
    { "MaxHostUtilization",        TOK_INT(LoadBalanceConfig,maxHostUtilization,300), },
    { "StaticCPUBias",        TOK_F32(LoadBalanceConfig,staticCPUBias,0), },
    { "StartingStaticMemBias",    TOK_INT(LoadBalanceConfig,startingStaticMemBias,0), },
    { "StartingStaticCPUBias",    TOK_F32(LoadBalanceConfig,startingStaticCPUBias,0), },
    { "StartingMissionMemBias",    TOK_INT(LoadBalanceConfig,startingMissionMemBias,0), },
    { "StartingMissionCPUBias",    TOK_F32(LoadBalanceConfig,startingMissionCPUBias,0), },
    { "SecondaryRoleBias",        TOK_F32(LoadBalanceConfig,secondaryRoleBias,0), },

    { "MinPhysicalMemory",    TOK_REDUNDANTNAME | TOK_INT(LoadBalanceConfig,minAvailPhysicalMemory,0), },        // allow old names until we transition data
    { "MinVirtualMemory",    TOK_REDUNDANTNAME | TOK_INT(LoadBalanceConfig,minAvailVirtualMemory,0), },        // allow old names until we transition data
    { "SuspensionTime",        TOK_REDUNDANTNAME | TOK_INT(LoadBalanceConfig,troubleSuspensionTime,0), },        // allow old names until we transition data

    { "MinVirtualMemory_LowWaterMark",      TOK_REDUNDANTNAME | TOK_INT(LoadBalanceConfig,minAvailVirtualMemory_lowWaterMark,0), },
    { "MaxMapservers_LowWaterMark",         TOK_INT(LoadBalanceConfig,maxMapservers_lowWaterMark,0), },
    { "MaxHostUtilization_LowWaterMark",    TOK_INT(LoadBalanceConfig,maxHostUtilization_lowWaterMark,0), },
    { "MinAvailVirtualMemory_LowWaterMark", TOK_INT(LoadBalanceConfig,minAvailVirtualMemory_lowWaterMark,0), },
    { "", 0, 0 }
};

void copyDefaults(LoadBalanceConfig *dst, LoadBalanceConfig *src)
{
    TokenizerParseInfo *pti = ParseLoadBalanceConfig;
    int i;
    FORALL_PARSEINFO(pti, i) 
    {
        switch (TOK_GET_TYPE(pti[i].type)) {
        xcase TOK_INT_X:
            if (TokenStoreGetInt(pti, i, src, 0))
                TokenStoreSetInt(pti, i, dst, 0, TokenStoreGetInt(pti, i, src, 0));
        xcase TOK_F32_X:
            if (TokenStoreGetF32(pti, i, src, 0))
                TokenStoreSetF32(pti, i, dst, 0, TokenStoreGetF32(pti, i, src, 0));
        }
    }
}

static int count_one_bits( U32 val )
{
    int ones = 0;

    while(val > 0)
    {
        ones += val & 0x1;
        val >>= 1;
    }
    return ones;
}

static bool validate_balancing_mode_flags(U32 mode_flags, const char* name)
{
    U32 strategy = mode_flags & LOADBALANCE_STRATEGY_MASK;
    U32 heuristic = mode_flags & LOADBALANCE_HEURISTIC_MASK;
    int ones;

    // should have one and only one strategy flag set
    ones = count_one_bits(strategy);
    if (ones == 0)
    {
        LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: '%s': No load balancing strategy flag is specified.", name);
        return false;
    }
    else if (ones > 1)
    {
        LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: '%s': Only one load balancing strategy flag is allowed.", name);
        return false;
    }

    // allowed heuristic flags depend on strategy
    ones = count_one_bits(heuristic);
    if (strategy&(LOADBALANCE_SEQUENTIAL|LOADBALANCE_RANDOM))
    {
        if (ones > 0)
        {
            LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: '%s': load balancing heuristic flags are not allowed with Sequential or Random strategy.", name);
            return false;
        }
    }
    else
    {
        if (ones == 0)
        {
            LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: '%s': load balancing heuristic flag must be included with this strategy.", name);
            return false;
        }
        else if (ones > 1)
        {
            LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: '%s': only a single load balancing heuristic flag can be included with this strategy.", name);
            return false;
        }
    }

    return true;
}

static int load_balance_reload;
static void loadBalanceReloadCallback(const char *relPath, int when)
{
    load_balance_reload = 1;
}

static void loadBalanceReloadCallbackFileChanged(const FileChange *change, void *user)
{
	(void)user;
	loadBalanceReloadCallback(change->entry.path, change->kind);
}


void loadBalanceCheckReload()
{
    if (load_balance_reload) {
        loadBalanceConfigLoad();
        load_balance_reload = 0;
    }
}

bool loadBalanceConfigLoad()
{
    int i;
    bool bad=false;
    static bool loaded_once=false;
    LoadBalanceConfig default_config={0}, shard_config={0};
    int default_loaded, shard_loaded;
    char buf[MAX_PATH];
    char* fullpath;
    bool ret = true;

    fullpath = fileLocateRead("server/db/loadBalanceDefault.cfg", buf);
    printf("Loading %s... ", fullpath);
    default_loaded = ParserLoadFiles(NULL, "server/db/loadBalanceDefault.cfg", NULL, 0, ParseLoadBalanceConfig, &default_config, NULL, NULL, NULL);
    if (!default_loaded) 
    {
        LOG_OLD_ERR( "failed to load loadBalanceDefault.cfg");
        if (!loaded_once) 
        {
            FatalErrorf("Failed to load required file server/db/loadBalanceDefault.cfg!");
        } 
        else 
        {
            printf("Failed to load server/db/loadBalanceDefault.cfg!\n");
        }
        return false; // Do nothing if reloading and failed to load
    }
    printf("done.\n");

    if (!fileLocateRead("server/db/loadBalanceShardSpecific.cfg", buf))
        strcpy(buf, "server/db/loadBalanceShardSpecific.cfg");
    printf("Loading %s... ", buf);
    shard_loaded = ParserLoadFiles(NULL, "server/db/loadBalanceShardSpecific.cfg", NULL, 0, ParseLoadBalanceConfig, &shard_config, NULL, NULL, NULL);
    if (!shard_loaded) 
    {
        LOG_OLD_ERR( "failed to load loadBalanceShardSpecific.cfg");
        printf("not found, using defaults.\n");
        // Just use default file
        ParserDestroyStruct(ParseLoadBalanceConfig, &load_balance_config);
    } 
    else 
    {
        ParserDestroyStruct(ParseLoadBalanceConfig, &load_balance_config);
        // Use default except where the values are non-0 in the override and
        //  put default server at the end of the list
        copyDefaults(&default_config, &shard_config);
        // Copy server specifications
        for (i=0; i<eaSize(&shard_config.serverRoles); i++) {
            eaInsert(&default_config.serverRoles, shard_config.serverRoles[i], i);
        }
        eaDestroy(&shard_config.serverRoles);
        printf("done.\n");
    }

    // Verification
    if (eaSize(&default_config.serverRoles)==0)
    {
        LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: No server roles specified.");
        bad = true;
    }

    // Make sure we have at least one mission map and at least one city zone
    if (!bad)
    {
        int i;
        bool zone_found=false;
        bool mission_found=false;
        for (i=0; i<eaSize(&default_config.serverRoles); i++)
        {
            ServerRoleDescription *role = default_config.serverRoles[i];
            if (role->primaryRole & ROLE_ZONE) {
                zone_found = true;
            }
            if (role->primaryRole & ROLE_MISSION) {
                mission_found = true;
            }
            if (role->primaryRole & ROLE_MONITOR && 
                !(role->primaryRole == ROLE_MONITOR))
            {
                LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: Found role with both Monitor and something else.  Monitor is exclusive with all other primary roles.");
                bad = true;
            }
            if (eaSize(&role->runServers) && role->iprange[1] && role->iprange[0] != role->iprange[1])
            {
                LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: Found role that launches a server (\"%s\") but has multiple IPs, this is not valid.", role->runServers[0]->command);
                bad = true;
            }
        }
        if (!zone_found) 
        {
            LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: No servers allowed to launch zone maps");
            bad = true;
        }
        if (!mission_found) 
        {
            LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: No servers allowed to launch mission maps");
            bad = true;
        }
    }

    // fill in appropriate default values for legacy config files that don't have
    // all the required settings
    if (!bad)
    {
        if (default_config.balanceModeZone == 0)
        {
            default_config.balanceModeZone = LOADBALANCE_SEQUENTIAL;
        }
        if (default_config.balanceModeMission == 0)
        {
            default_config.balanceModeMission = LOADBALANCE_SEARCH | LOADBALANCE_UTILIZATION;
        }
    }

    // make sure we have valid balancing flags
    if (!bad)
    {
        bad = !validate_balancing_mode_flags(default_config.balanceModeZone, "BalanceModeZone" );
    }
    if (!bad)
    {
        bad = !validate_balancing_mode_flags(default_config.balanceModeMission, "BalanceModeMission" );
    }

    // make sure low-water marks are sane
    if (!bad && default_config.maxMapservers_lowWaterMark > default_config.maxMapservers)
    {
        LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: 'MaxMapservers_LowWaterMark' is larger than 'MaxMapservers'");
        bad = true;
    }

    if (!bad && default_config.maxHostUtilization_lowWaterMark > default_config.maxHostUtilization)
    {
        LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: 'MaxHostUtilization_LowWaterMark' is larger than 'MaxHostUtilization'");
        bad = true;
    }

    if (!bad && default_config.minAvailVirtualMemory_lowWaterMark > default_config.minAvailVirtualMemory)
    {
        LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "LoadBalanceConfig error: 'MinAvailVirtualMemory_LowWaterMark' is larger than 'MinAvailVirtualMemory'");
        bad = true;
    }

    if (!bad)
    {
        load_balance_config = *&default_config;
        printf("Loaded load balancing config: %d roles specified.\n", eaSize(&load_balance_config.serverRoles));
    }
    else 
    {
        ret = false;
        if (!loaded_once)
        {
            FatalErrorf("Errors in load balancing config files (see console for details)");
        }
    }
    if (!assembleAutoStartList())
        ret = false;

    if (!loaded_once) 
    {
        fileSystemSubscribe(NULL, "server/db/loadBalance*.cfg", FILE_CHANGE_UPDATE|FILE_CHANGE_SHARED, loadBalanceReloadCallbackFileChanged, NULL);
    }

    loaded_once = true;
    return ret;
}

ServerRoleDescription *getNextServerRole(ServerRoleDescription* curRole, U32 ip)
{
    int size = eaSize(&load_balance_config.serverRoles);
    int i;
    for (i=0; i<size; i++) 
    {
        ServerRoleDescription *role = load_balance_config.serverRoles[i];
        if (role->iprange[1]==0) {
            // Exact match
            if (ip != role->iprange[0])
            {
                role = NULL;
            }
        } else {
            static TokenizerParseInfo ip_comparator[2] = {{"", TOK_INT_X, 0, 0, 0, TOK_FORMAT_IP}, { 0 }};
            // Range
            if (ParserCompareTokens(ip_comparator, 0, &ip, &role->iprange[0]) < 0 ||
                ParserCompareTokens(ip_comparator, 0, &ip, &role->iprange[1]) > 0)
            {
                role = NULL;
            }
        }
        
        if(role){
            if(curRole){
                if(curRole == role){
                    curRole = NULL;
                }
            }else{
                return role;
            }
        }
    }
    
    return NULL;
}

ServerRoleDescription *getServerRole(U32 ip)
{
    ServerRoleDescription* role = getNextServerRole(NULL, ip);
    
    if(!role)
    {
        static ServerRoleDescription fallback;
        
        if (!fallback.primaryRole) {
            fallback.primaryRole = ROLE_ZONE | ROLE_MISSION;
        }
        
        return &fallback;
    }
    
    return role;
}

