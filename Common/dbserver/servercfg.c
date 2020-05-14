#include "servercfg.h"
#include "auction/auction.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>
#include <string.h>
#include <utilitieslib/components/SharedHeap.h>
#include <utilitieslib/utils/SuperAssert.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/mathutil.h>
#include "auth/auth.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/log.h>
#include "overloadProtection.h"

#ifdef DBSERVER
#include "auth/authUserData.h"
#include "container_tplt_utils.h"
#endif

ServerCfg    server_cfg;

int dbIsLocalIp(U32 ip)
{
    int        i;
    U32        local;

    if (!server_cfg.local_ip_count)
        return isLocalIp(ip);
    for(i=0;i<server_cfg.local_ip_count;i++)
    {
        local = server_cfg.local_ips[i];

        if ((ip & 0x0000ffff) == (local & 0x00000ffff))
            return 1;
    }
    return 0;
}

int parseAssertMode(char *s)
{
    int mode = ASSERTMODE_DEBUGBUTTONS | ASSERTMODE_DATEDMINIDUMPS | ASSERTMODE_NOIGNOREBUTTONS;
    if (strstri(s, "Minidump")!=0)
        mode |= ASSERTMODE_MINIDUMP;
    if (strstri(s, "Fulldump")!=0)
        mode |= ASSERTMODE_FULLDUMP;
    if (strstri(s, "Exit")!=0)
        mode |= ASSERTMODE_EXIT;
    if (strstri(s, "NoTimestamp")!=0)
        mode &= ~ASSERTMODE_DATEDMINIDUMPS;
    if (strstri(s, "NoDate")!=0)
        mode &= ~ASSERTMODE_DATEDMINIDUMPS;
    if (strstri(s, "Zip")!=0)
        mode |= ASSERTMODE_ZIPPED;
    if (strstri(s, "Ignore")!=0)
        mode &= ~ASSERTMODE_NOIGNOREBUTTONS;
    return mode;
}

static void setDefaultBeaconizerConfig()
{
    server_cfg.do_not_launch_beacon_clients = 0;
    server_cfg.do_not_launch_master_beacon_server = 0;
    
    server_cfg.request_beacon_server_count = -1;
    server_cfg.beacon_client_count = 2;
    
    getExecutableDir(server_cfg.beacon_request_cache_dir);
    
    forwardSlashes(server_cfg.beacon_request_cache_dir);
    
    if(    strlen(server_cfg.beacon_request_cache_dir) >= 3 &&
        isalpha((U8)server_cfg.beacon_request_cache_dir[0]) &&
        !strncmp(server_cfg.beacon_request_cache_dir + 1, ":/", 2))
    {
        strcpy_s(server_cfg.beacon_request_cache_dir + 3, sizeof(server_cfg.beacon_request_cache_dir) - 3, "beaconrequestcache");
    }
    
    forwardSlashes(server_cfg.beacon_request_cache_dir);
    
    server_cfg.master_beacon_server[0] = 0;
}

void serverCfgLoad()
{
    FILE    *file = NULL;
    char    buf[1000],*s,*s2,delim[] = "\n\r\t",*args[50];
    int        count;
    char    realFilenameBuffer[MAX_PATH];
    char*    realFilename;
    char*    relFilename = "server/db/servers.cfg";
    bool    force_overload_protection = false;
    F32        launcher_high_water_mark_percent = 100.0f;
    F32        launcher_low_water_mark_percent = 90.0f;
    int        sql_queue_high_water_mark = 0;    // disabled
    int        sql_queue_low_water_mark = 0;

    server_cfg.time_started = timerSecondsSince2000();

    realFilename = fileLocateRead(relFilename,realFilenameBuffer);

    printf("Loading %s\n", realFilename);

    // The server config file can be reloaded many times, make sure
    // to restore defaults in order to correctly reflect changes to the configuration.
    server_cfg.dual_player_slots    = DUAL_PLAYER_SLOTS;
    server_cfg.backup_period        = 30;
    server_cfg.logins_per_minute    = 100;

    server_cfg.xpscale                = 1.0f;
    server_cfg.influencescale         = 1.0f;
    server_cfg.prestigescale          = 1.0f;
    server_cfg.aescale                = 1.0f;
    server_cfg.aeinfluencescale       = 1.0f;
    server_cfg.aeprestigescale        = 1.0f;
    server_cfg.ticketscale            = 1.0f;
    server_cfg.client_logout_time     = 30;
    server_cfg.aggrocap               = 17;
    server_cfg.targetcapmode          = 1;
    server_cfg.client_commands[0]    = 0;
    server_cfg.map_server_params[0]    = 0;

    server_cfg.eventhistory_cache_days = 30;
    server_cfg.MARTY_enabled = 0;

#ifdef DBSERVER
    //We might be reloading the config file so bits set by prior versions
    //of the file can't persist
    for (count = 0; count < 4; count++)
        g_overridden_auth_bits[count] = 0;
#endif

    //By default, get timeZoneDelta from machine clock
    {
        TIME_ZONE_INFORMATION timeZoneInformation = {0};
        GetTimeZoneInformation( &timeZoneInformation );
        server_cfg.timeZoneDelta = -timeZoneInformation.Bias / 60.0; //minutes to hrs
    }

    setDefaultBeaconizerConfig();

    eaClearEx(&server_cfg.blocked_map_keys, NULL);
    eaClearEx(&server_cfg.disabled_zone_events, NULL);

    server_cfg.locale_id = -1;

    server_cfg.isBetaShard = 0;
    server_cfg.isVIPServer = 0;
    server_cfg.ownsGR = 1;

    server_cfg.GRNagAndPurchase = 0;
    server_cfg.queue_server = 1;
    server_cfg.max_players = DEFAULT_MAX_PLAYERS;

    server_cfg.name_lock_timeout = DEFAULT_NAME_LOCK_TIMEOUT;

    server_cfg.advertisedIp = 0;

    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        server_cfg.beacon_client_count = si.dwNumberOfProcessors + 1;
    }

    if (!realFilename || !(file = fopen(realFilename, "rt")))
    {
        printf("Can't load server/db/servers.cfg!\n");
        return;
    }
    for(;;)
    {
        char dbserverName[32];
        int i;

        if (!fgets(buf,sizeof(buf),file))
            break;
        if (buf[0] == '#')
            continue;
        count = tokenize_line(buf,args,0);
        if (count < 2)
            continue;

        s = args[0];
        s2 = args[1];

        if (stricmp(s,"DbServer")==0)
        {
            strcpy(server_cfg.db_server,s2);
        }
        // Ugh - counting in this for loop blows.  Actually count from [ 1 .. MAX ) since 0 has been accounted for by the "DbServer" test just above
        for (i = 1; i < MAX_DBSERVER; i++)
        {
            // However add one when generating the name, so that the config file contains "DbServer", "DbServer2", DbServer3" etc.  IMHO this is the least
            // confusing numbering system.
            _snprintf_s(dbserverName, sizeof(dbserverName), sizeof(dbserverName) - 1, "DBserver%d", i + 1);
            if (stricmp(s,dbserverName)==0)
            {
                // And then subtract one so the aux servers are zero based.  Not that I'm convinced this is a massively good idea, because of the
                // "zero is already spoken for" problem.  It leads to a few too many "dingbat - 1"s in the launcher code.
                strcpy(server_cfg.db_server_aux[i - 1],s2);
            }
        }
        if (stricmp(s,"UseFakeAuth")==0)
        {
            server_cfg.fake_auth = atoi(s2);
        }
        else if (stricmp(s,"AuthServer")==0)
        {
            strcpy(server_cfg.auth_server,s2);
            server_cfg.auth_server_port = atoi(args[2]);
        }
        else if (stricmp(s,"LocalIp")==0)
        {
            char    buf[1000],*s=buf;
            U32        ip=0;
            int        i;
            char *context;

            strcpy(buf,s2);
            for(i=0;i<4;i++)
            {
                s = strtok_s(s,".",&context);
                if (!s)
                    break;
                ip |= atoi(s) << (8 * i);
                s = 0;
            }
            server_cfg.local_ips[server_cfg.local_ip_count++] = ip;
        }
#ifdef DBSERVER
        else if (stricmp(s,"SqlDbProvider")==0)
        {
            if (stricmp(s2, "mssql") == 0) {
                gDatabaseProvider = DBPROV_MSSQL;
            } else if (stricmp(s2, "postgresql") == 0) {
                gDatabaseProvider = DBPROV_POSTGRESQL;
            }
        }
#endif
        else if (stricmp(s,"SqlDbName")==0)
            strcpy(server_cfg.sql_db_name,s2);
        
        /* no longer reserved on the launcher
        if (stricmp(s,"SharedHeapMegs")==0)
        {
            U32 uiSize = atoi(s2) * 1024 * 1024;
            setSharedHeapReservationSize(uiSize);
        }
        */
        else if (stricmp(s,"SqlLogin")==0)
            strcpy(server_cfg.sql_login,s2);
        else if (stricmp(s,"SqlInit")==0)
            strcat(server_cfg.sql_init,s2);
        else if (stricmp(s,"SqlAllowDDL")==0 || stricmp(s,"SqlAllowAllDDL")==0 )
            server_cfg.sql_allow_all_ddl = atoi(s2);
        else if (stricmp(s,"SqlAddAttributes")==0)
            server_cfg.ddl_attributes = atoi(s2);
        else if (stricmp(s,"SqlAddColumnOrTable")==0)
            server_cfg.ddl_add = atoi(s2);
        else if (stricmp(s,"SqlDeleteColumnOrTable")==0)
            server_cfg.ddl_del = atoi(s2);
        else if (stricmp(s,"SqlRebuildTable")==0)
            server_cfg.ddl_rebuildtable = atoi(s2);
        else if (stricmp(s,"SqlAlterColumn")==0)
            server_cfg.ddl_altercolumn = atoi(s2);
        else if (stricmp(s,"DefaultAccessLevel")==0)
            server_cfg.default_access_level = atoi(s2);
        else if (stricmp(s,"LogDir")==0)
            logSetDir(s2);
        else if (stricmp(s,"AssertMode")==0)
            server_cfg.assertMode = parseAssertMode(s2);
        else if (stricmp(s,"MaxPlayers")==0)
        {
            server_cfg.max_players = atoi(s2);
            if(server_cfg.max_players >= AUTH_SIZE_MAX)
            {
                assertmsgf(0, "MaxPlayers in db server cfg is set to %i.  Must be less than %i.", server_cfg.max_players , AUTH_SIZE_MAX);
                server_cfg.max_players = AUTH_SIZE_MAX-1;
            }

        }
        else if (stricmp(s,"LoginsPerMinute")==0)
        {
            server_cfg.logins_per_minute = atoi(s2);
            if(server_cfg.logins_per_minute <= 0)
            {
                assertmsgf(0, "LoginsPerMinute in db server cfg is set to %i.  Must be positive value.", server_cfg.logins_per_minute , AUTH_SIZE_MAX);
                server_cfg.logins_per_minute = 100;
            }

        }
        else if (stricmp(s,"MinPlayers")==0)
            server_cfg.min_players = atoi(s2);
        else if (stricmp(s,"UseQueueServer")==0)
            server_cfg.queue_server = atoi(s2);
        else if (stricmp(s,"BlockFreePlayersIfNoAccountServer")==0)
            server_cfg.block_free_if_no_account = atoi(s2);
        else if (stricmp(s,"EnqueueWithAuthLimiter")==0)
            server_cfg.enqueue_auth_limiter = atoi(s2);
        else if (stricmp(s,"NoStats")==0)
            server_cfg.no_stats = atoi(s2);
        else if (stricmp(s,"ClientProject")==0)
            strcpy(server_cfg.client_project,s2);
        else if (stricmp(s,"MapServerParams")==0)
            strcpy(server_cfg.map_server_params,s2);
        else if (stricmp(s,"DiffDebug")==0)
            server_cfg.diff_debug = atoi(s2);
        else if (stricmp(s,"ContainerSizeDebug")==0)
            server_cfg.container_size_debug = atoi(s2);
        else if (stricmp(s,"UseLogServer")==0)
            server_cfg.use_logserver = atoi(s2);
        else if (stricmp(s,"LogServer")==0)
        {
            strcpy(server_cfg.log_server,s2);
            if (!server_cfg.use_logserver)
                server_cfg.use_logserver = 1;
        }
        else if (stricmp(s,"LogRelayVerbose")==0)
            server_cfg.log_relay_verbose = atoi(s2);
        else if (stricmp(s,"ChatServer")==0)
            strcpy(server_cfg.chat_server,s2);
        else if (stricmp(s,"ShardName")==0)
            strcpy(server_cfg.shard_name,s2);
        else if (stricmp(s,"ChangeDbOwnerFrom")==0)
            strcpy(server_cfg.change_db_owner_from,s2);
        else if (stricmp(s, "MaxLevel")==0)
            server_cfg.max_level = atoi(s2);
        else if (stricmp(s, "MaxCoHLevel")==0)
            server_cfg.max_coh_level = atoi(s2);
        else if (stricmp(s, "MaxCoVLevel")==0)
            server_cfg.max_cov_level = atoi(s2);
        else if (stricmp(s, "MaxDualSlots")==0)
            server_cfg.dual_player_slots = MINMAX(atoi(s2),1, DUAL_PLAYER_SLOTS);
        else if (stricmp(s, "BackupDays")==0)
            server_cfg.backup_period = MAX( 30, atoi(s2) );
        else if (stricmp(s, "OfflineProtectLevel")==0)
            server_cfg.offline_protect_level = atoi(s2);
        else if (stricmp(s, "OfflineIdleDays")==0)
            server_cfg.offline_idle_days = atoi(s2);
        else if (stricmp(s, "CompleteBrokenTasks")==0)
            server_cfg.complete_broken_tasks = atoi(s2);
        else if (stricmp(s, "DroppedPacketLogging")==0)
            server_cfg.dropped_packet_logging = atoi(s2);
        else if (stricmp(s, "AuthBadPacketReconnect")==0)
            server_cfg.auth_bad_packet_reconnect = atoi(s2);
        else if (!stricmp(s, "BeaconMasterServer") ||
            !stricmp(s, "MasterBeaconServer"))
        {
            Strncpyt(server_cfg.master_beacon_server, s2);
        }
        else if (!stricmp(s, "DoNotLaunchBeaconMasterServer") ||
            !stricmp(s, "DoNotLaunchMasterBeaconServer"))
        {
            server_cfg.do_not_launch_master_beacon_server = atoi(s2);
        }
        else if (!stricmp(s, "BeaconRequestServerCount") ||
            !stricmp(s, "RequestBeaconServerCount"))
        {
            server_cfg.request_beacon_server_count = atoi(s2);
        }
        else if (!stricmp(s, "BeaconClientCount"))
        {
            int beacon_clients = MIN(atoi(s2), server_cfg.beacon_client_count);
            server_cfg.beacon_client_count = beacon_clients;
        }
        else if (stricmp(s, "DoNotPreloadCrashReportDLL")==0)
            server_cfg.do_not_preload_crashreportdll = atoi(s2);
        else if (stricmp(s, "BeaconRequestCacheDir")==0)
            Strncpyt(server_cfg.beacon_request_cache_dir, s2);
        else if (stricmp(s, "DoNotLaunchBeaconClients")==0)
            server_cfg.do_not_launch_beacon_clients = atoi(s2);
        else if (stricmp(s, "DoNotLaunchMapServerTSR")==0 ||
            stricmp(s, "DoNotLaunchMapServerTSRs")==0)
            server_cfg.do_not_launch_mapserver_tsrs = atoi(s2);
        else if (stricmp(s, "RaidTimeZoneDelta")==0)
            server_cfg.timeZoneDelta = atof(s2);
        else if (stricmp(s, "DisableContainerBackups")==0)
            server_cfg.disableContainerBackups = atoi(s2);
        else if (stricmp(s, "XPScale")==0)
            server_cfg.xpscale = atof(s2);
        else if (stricmp(s, "InfluenceScale")==0)
            server_cfg.influencescale = atof(s2);
        else if (stricmp(s, "PrestigeScale")==0)
            server_cfg.prestigescale = atof(s2);
        else if (stricmp(s, "AEScale")==0)
            server_cfg.aescale = atof(s2);
        else if (stricmp(s, "AEInfluenceScale")==0)
            server_cfg.aeinfluencescale = atof(s2);
        else if (stricmp(s, "AEPrestigeScale")==0)
            server_cfg.aeprestigescale = atof(s2);
        else if (stricmp(s, "TicketScale")==0)
            server_cfg.ticketscale = atof(s2);
        else if (stricmp(s, "ClientLogoutTime")==0)
            server_cfg.client_logout_time = atoi(s2);
        else if (stricmp(s, "AggroCap") == 0)
            server_cfg.aggrocap = atof(s2);
        else if (stricmp(s, "TargetCapMode") == 0)
            server_cfg.targetcapmode = atof(s2);
        else if (stricmp(s, "AuctionInvMaxLastLoginDays")==0)
            server_cfg.auction_last_login_delay = atoi(s2);
        else if (stricmp(s, "AuthnameLimiterEnabled")==0)
            server_cfg.authname_limiter = atoi(s2);
        else if (stricmp(s, "AuthnameLimiterAccessLevel")==0)
            server_cfg.authname_limiter_access_level = atoi(s2);
        else if (stricmp(s, "BlockedMapKey")==0)
        {
            int i;
            for(i = 1; i < count; i++)
            {
                char *start, *newStr;
                int len = 0;
                start = args[i];

                do
                {
                    while(start[len] != ',' && start[len] != '\0')
                        len++;
                    newStr = (char*)malloc((++len) * sizeof(char));
                    strncpyt(newStr, start, len);
                    eaPush(&server_cfg.blocked_map_keys, newStr);
                    start += len;
                    len = 0;
                }while(start[len-1] != '\0');
            }
        }
        else if (stricmp(s, "DisabledZoneEvents")==0)
        {
            int i;
            for(i = 1; i < count; i++)
            {
                char *start, *newStr;
                int len = 0;
                start = args[i];

                do
                {
                    while(start[len] != ',' && start[len] != '\0')
                        len++;
                    newStr = (char*)malloc((++len) * sizeof(char));
                    strncpyt(newStr, start, len);
                    eaPush(&server_cfg.disabled_zone_events, newStr);
                    start += len;
                    len = 0;
                }while(start[len-1] != '\0');
            }
        }
        else if (stricmp(s, "ClientCommands")==0)
            strcpy(server_cfg.client_commands,s2);
        else if (stricmp(s, "AuthRequestGameData")==0)
            printf("AuthRequestGameData is deprecated. Please remove to reduce clutter.\n");
        else if (stricmp(s, "MissionserverMaxSendQueueSize")==0)
            server_cfg.missionserver_max_queueSize = atoi(s2);
        else if (stricmp(s, "MissionserverMaxSendQueuePublishSize")==0)
            server_cfg.missionserver_max_queueSizePublish = atoi(s2);
        else if (stricmp(s, "Locale")==0)
            server_cfg.locale_id = locGetIDByNameOrID(s2);
#ifdef DBSERVER
        else if (stricmp(s, "OverrideAuthBit")==0)
        {
            int i;
            char temparg[1000];

            for (i = 1; i < count; i++)
            {
                int pos = 0;

                while (args[i][pos] != ',' && args[i][pos] != '\0')
                {
                    temparg[pos] = args[i][pos];
                    pos++;
                }

                temparg[pos] = '\0';

                if (strlen(temparg) > 0)
                {
                    if (!authUserSetFieldByName((U32*)(g_overridden_auth_bits), temparg, 1))
                    {
                        assertmsgf(0, "Unknown authbit name '%s' listed in overrides.", temparg);
                    }
                }
            }
        }
#endif
        else if(stricmp(s, "IsBetaShard") == 0)
        {
            server_cfg.isBetaShard = (atoi(s2));
        }
        else if(stricmp(s, "IsVIPShard") == 0)
        {
            server_cfg.isVIPServer = (atoi(s2));
        }
        else if (stricmp(s, "AllowGR") == 0)
        {
            printf("AllowGR is deprecated in i19. Please remove to prevent clutter\n");
        }
        else if (stricmp(s, "AllowGREndgame") == 0)
        {
            printf("AllowGREndgame is deprecated. Please remove to reduce clutter.\n");
        }
        else if (stricmp(s, "AllowIssue20") == 0)
        {
            printf("AllowIssue20 is deprecated. Please remove to reduce clutter.\n");
        }
        else if (stricmp(s, "AllowPraetorians") == 0)
        {
            printf("AllowPraetorians is deprecated. Please remove to reduce clutter.\n");
        }
        else if (stricmp(s, "OwnsGoingRogue") == 0)
        {
            server_cfg.ownsGR = atoi(s2);
        }
        else if (stricmp(s, "PraetoriaZonesLocked") == 0)
        {
            printf("PraetoriaZonesLocked is deprecated. Please remove to reduce clutter.\n");
        }
        else if (stricmp(s, "GoingRogueNagAndPurchase") == 0)
        {
            server_cfg.GRNagAndPurchase = atoi(s2);
        }
        else if (stricmp(s, "KarmaEventHistoryDays") == 0)
        {
            server_cfg.eventhistory_cache_days = atoi(s2);
        }
        else if (stricmp(s, "NameLockTimeout") == 0)
        {
            server_cfg.name_lock_timeout = atoi(s2);
            if (server_cfg.name_lock_timeout <= CHECK_NAME_REFRESH_TIME)
            {
                assertmsgf(0, "NameLockTimeout (%d) is smaller than client refresh time (%d) and will timeout Locked Names.", server_cfg.name_lock_timeout, CHECK_NAME_REFRESH_TIME);
            }
        }
        else if (stricmp(s, "SetLogLevel") == 0)
        {
            if( count < 3 )
                continue;
            logSetLevel( logGetTypeFromName(s2), atoi(args[2]) );
        }
        else if (stricmp(s, "IsLoggingMaster") == 0)
        {
            server_cfg.isLoggingMaster = (atoi(s2));
        }
        else if (stricmp(s, "MARTYEnabled") == 0)
        {
            server_cfg.MARTY_enabled = atoi(s2) ? 1 : 0;
        }
#ifdef DBSERVER
        else if (stricmp(s, "DebugSendPlayersDelayMS") == 0)
        {
            server_cfg.debugSendPlayersDelayMS = (atoi(s2));
        }
        else if (stricmp(s, "DefaultLoyaltyPointsFakeAuth") == 0)
        {
            server_cfg.defaultLoyaltyPointsFakeAuth = (atoi(s2));
        }
        else if (stricmp(s, "DefaultLoyaltyLegacyPointsFakeAuth") == 0)
        {
            server_cfg.defaultLoyaltyLegacyPointsFakeAuth = (atoi(s2));
        }
        else if (stricmp(s, "ForceOverloadProtection") == 0)
        {
            force_overload_protection = (atoi(s2) > 0);
        }
        else if (stricmp(s, "OverloadProtection_LauncherHighWaterMarkPercent") == 0)
        {
            launcher_high_water_mark_percent = atof(s2);
        }
        else if (stricmp(s, "OverloadProtection_LauncherLowWaterMarkPercent") == 0)
        {
            launcher_low_water_mark_percent = atof(s2);
        }
        else if (stricmp(s, "OverloadProtection_SQLQueueHighWaterMark") == 0)
        {
            sql_queue_high_water_mark = atoi(s2);
        }
        else if (stricmp(s, "OverloadProtection_SQLQueueLowWaterMark") == 0)
        {
            sql_queue_low_water_mark = atoi(s2);
        }
#endif
        else if (stricmp(s, "MapserverIdleExit") == 0)
        {
            int idle_minutes = atoi(s2);
            server_cfg.mapserver_idle_exit_timeout = idle_minutes > 0 ? (U32)idle_minutes : 0;
        }
        else if (stricmp(s, "MapserverIdleUpkeep") == 0)
        {
            int idle_minutes = atoi(s2);
            server_cfg.mapserver_maintenance_idle = idle_minutes > 0 ? (U32)idle_minutes : 0;
        }
        else if (stricmp(s, "MapserverDailyUpkeep") == 0)
        {
            if( count < 3 )
            {
                printf("MapserverDailyUpkeep requires a starting hour [0-24] and an ending hour for the maintenance window.\n");
                continue;
            }
            {
                int hour_begin = atoi(s2);
                int hour_end =  atoi(args[2]);
                if (hour_begin >=0 && hour_end > hour_begin && hour_end <= 24)
                {
                    server_cfg.mapserver_maintenance_daily_start = hour_begin;
                    server_cfg.mapserver_maintenance_daily_end = hour_end;
                }
                else
                    printf("MapserverDailyUpkeep begin and end need to be hours from 0-24, with begin <= end");
            }
        }
        else if (stricmp(s, "SendDoorsToAllMaps") == 0)
        {
            server_cfg.send_doors_to_all_maps = (atoi(s2));
        }
        else if (stricmp(s, "MetricsEnable") == 0)
        {
            server_cfg.metrics_enable = (atoi(s2));
        }
        else if (stricmp(s, "MetricsIPAddress") == 0)
        {
            strcpy(server_cfg.metrics_ipAddress,s2);
        }
        else if (stricmp(s, "MetricsPortNumber") == 0)
        {
            server_cfg.metrics_portNumber = (atoi(s2));
        }
        else if (stricmp(s, "MetricsMQType") == 0 )
        {
            server_cfg.metrics_mqType = (atoi(s2));
        }
        else if (stricmp(s, "MetricsHighWaterMark") == 0)
        {
            server_cfg.metrics_hwm = (atoi(s2));
        }
        else if (stricmp(s, "AdvertisedIp") == 0)
        {
            server_cfg.advertisedIp = ipFromString(s2);
        }
    }
    fclose(file);

    if (server_cfg.mapserver_idle_exit_timeout)
    {
        char txt[256];
        sprintf(txt, " -idleExitTimeout %d", server_cfg.mapserver_idle_exit_timeout);
        strcat(server_cfg.map_server_params, txt);
    }
    if (server_cfg.mapserver_maintenance_idle)
    {
        char txt[256];
        sprintf(txt, " -idleUpkeep %d", server_cfg.mapserver_maintenance_idle);
        strcat(server_cfg.map_server_params, txt);
    }
    if (server_cfg.mapserver_maintenance_daily_start >=0 && server_cfg.mapserver_maintenance_daily_end > server_cfg.mapserver_maintenance_daily_start && server_cfg.mapserver_maintenance_daily_end <= 24)
    {
        char txt[256];
        sprintf(txt, " -dailyUpkeep %d %d", server_cfg.mapserver_maintenance_daily_start, server_cfg.mapserver_maintenance_daily_end);
        strcat(server_cfg.map_server_params, txt);
    }
    if (server_cfg.isBetaShard)
    {
        strcat(server_cfg.map_server_params, " -IsBetaShard");
    }
    if (server_cfg.MARTY_enabled)
        strcat(server_cfg.map_server_params, " -MARTY_enabled");

    if ((!server_cfg.auth_server[0]) == (!server_cfg.fake_auth))
        FatalErrorf("Either one of UseFakeAuth or AuthServer needs to be enabled in servers.cfg");

    printf("  Loading %s done\n", realFilename);

#ifdef DBSERVER
    overloadProtection_ManualOverride(force_overload_protection);
    overloadProtection_SetLauncherWaterMarks(launcher_low_water_mark_percent, launcher_high_water_mark_percent);
    overloadProtection_SetSQLQueueWaterMarks(sql_queue_low_water_mark, sql_queue_high_water_mark);
#endif
}

int cfg_IsBetaShard(void)
{
    return server_cfg.isBetaShard;
}

void cfg_setIsBetaShard(int data)
{
    server_cfg.isBetaShard = data;
}

int cfg_VIPShard(void)
{
    return server_cfg.isVIPServer;
}
void cfg_setVIPShard(int data)
{
    server_cfg.isVIPServer = data;
}


