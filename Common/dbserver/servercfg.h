#ifndef _SERVERCFG_H
#define _SERVERCFG_H

#include <utilitieslib/stdtypes.h>
#include "auctionservercomm.h"

#define MAX_DBSERVER 16

typedef struct ServerCfg
{
    char    db_server[256];
    char    db_server_aux[MAX_DBSERVER - 1][256];
    char    log_server[256];
    char    auth_server[256];
    int        auth_server_port;
    int        map_server_count;
    char    sql_login[2000];
    char    sql_db_name[100];
    char    sql_init[2000];
    char    client_project[200];
    char    map_server_params[1000];
    U32        local_ips[1000];
    int        local_ip_count;
    U32        fake_auth : 1;
    U32        sql_allow_all_ddl : 1;
    U32        ddl_attributes : 1;
    U32        ddl_add : 1;
    U32        ddl_del : 1;
    U32        ddl_rebuildtable : 1;
    U32        ddl_altercolumn : 1;
    U32        sql_indentity_insert : 1;
    U32        no_stats : 1;
    U32        use_logserver : 2;
    U32        use_cod : 1;
    U32        complete_broken_tasks : 1;
    U32        dropped_packet_logging : 2;
    U32        auth_bad_packet_reconnect : 1;
    U32        authname_limiter : 1;
    U32        queue_server:1;
    U32        enqueue_auth_limiter:1;
    U32        send_doors_to_all_maps:1;
    U32        MARTY_enabled:1;
    U32        block_free_if_no_account:1;
    U32        authname_limiter_access_level;
    U32        diff_debug;
    U32        container_size_debug;
    U32        time_started;
    int        max_level;
    int        max_cov_level;
    int        max_coh_level;
    int        default_access_level;
    int        assertMode;
    int        max_players;
    int        logins_per_minute;
    int        min_players;
    int        log_relay_verbose;
    char    chat_server[256];
    char    shard_name[256];
    char    change_db_owner_from[256];
    int        offline_idle_days;        // number of days player can not log in before getting moved to offline storage
    int        offline_protect_level;        // max level of player that can be moved to offline storage
    char    master_beacon_server[256];
    char    beacon_request_cache_dir[256];
    int        request_beacon_server_count;
    int        beacon_client_count;
    int        do_not_launch_master_beacon_server;
    int        do_not_launch_beacon_clients;
    int        do_not_launch_mapserver_tsrs;
    int        write_teamups_to_sql;
    char**    blocked_map_keys;
    char**    disabled_zone_events;
    int        dual_player_slots;
    int        backup_period;
    int        do_not_preload_crashreportdll;
    F32        timeZoneDelta;
    int        disableContainerBackups;
    F32        xpscale;
    F32        influencescale;
    F32        prestigescale;
    F32        aescale;
    F32        aeinfluencescale;
    F32        aeprestigescale;
    F32        ticketscale;
    int        client_logout_time;
    int        aggrocap;
    int        targetcapmode;
    int        auction_last_login_delay;
    char    client_commands[2000];
    U32        missionserver_max_queueSize;
    U32        missionserver_max_queueSizePublish;
    U32        eventhistory_cache_days;
    int     locale_id;
    U32        name_lock_timeout;

    int isBetaShard;

    // This stops the Going Rogue nag popup and purchase dialogs from showing
    // up so we can hold it off until PlayNC is ready, for example.
    int GRNagAndPurchase;

    // This is per-player default for RogueAccess. It's not handled by the
    // normal OverrideAuthBit logic because it's enabled by default and has
    // to be explicitly disabled. Only people testing nag functionality have
    // to modify their configs.
    int ownsGR;

    int isVIPServer;

    int isLoggingMaster;
    int debugSendPlayersDelayMS;
    int defaultLoyaltyPointsFakeAuth;
    int defaultLoyaltyLegacyPointsFakeAuth;

    U32        mapserver_idle_exit_timeout;                        // shutdown the mapserver if it is idle for this many minutes, 0 means there is no idle exit
    U32        mapserver_maintenance_idle;                            // idle minutes before performing periodic minor idle maintenance, 0 means there is no idle maintenance
    U32        mapserver_maintenance_daily_start;                    // starting hour of window for automatic daily maintenance (start = end means no maintenance window)
    U32        mapserver_maintenance_daily_end;                    // ending hour of window for automatic daily maintenance

    //The following are parameters that will configure the CoH metrics system
    bool metrics_enable;
    int metrics_portNumber;
    int metrics_mqType;  //determines the socket type of the metrics system's ZeroMQ connection
    char metrics_ipAddress[16000];  //enough space for really, really long hostnames
    unsigned __int64 metrics_hwm; //sets the high water mark for the metric's system ZeroMQ connection

    //This is used to tell the mapserver to advertise this address to clients
    U32 advertisedIp;

} ServerCfg;

extern ServerCfg server_cfg;

//#define MAX_PLAYER_SLOTS 12
#define MAX_PLAYER_SLOTS 48
#define DUAL_PLAYER_SLOTS 12
#define DEFAULT_MAX_PLAYERS 100
#define CHECK_NAME_REFRESH_TIME 600 //checkNameRefreshTime set in uiHybridMenu.c, how often client refreshes
#define DEFAULT_NAME_LOCK_TIMEOUT 1200   //(in seconds) 20 minute timeout default, how long server will hold on to a name

void serverCfgLoad(void);
int dbIsLocalIp(U32 ip);

int cfg_IsBetaShard();
int cfg_VIPShard();

void cfg_setIsBetaShard(int data);
void cfg_setVIPShard(int data);

#endif
