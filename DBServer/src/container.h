#ifndef _CONTAINER_H
#define _CONTAINER_H

typedef const struct StashTableImp *cStashTable;
typedef struct Packet Packet;

#include "auth/auth.h"        // for AUTH_BYTES
#include "account/AccountTypes.h"

typedef struct StashTableImp *StashTable;
typedef struct ContainerTemplate ContainerTemplate;
typedef struct NetLink NetLink;

typedef void AnyContainer;

typedef struct RemoteProcessInfo
{
    char    hostname[128];
    int        map_id;
    int        process_id;            // process id of remote process
    U32        mem_used_phys;        // Working set size in KB (private and shared)
    U32        mem_peak_phys;        // Peak working set size in KB
    U32        mem_used_virt;        // Private commit charge of process in KB (cannot be shared with other processes)
//    U32        mem_priv_virt;        // Private commit charge of process in KB (cannot be shared with other processes)
    U32        mem_peak_virt;        // Peak commit charge of process in KB
    U32        thread_count;        // Number of threads used by process
    U32        total_cpu_time;        // cumulative cpu time process has spent in kernal and user modes combined (in milliseconds)
    F32        cpu_usage,cpu_usage60;    // CPU usage percentage instantaneous and averaged over a minute (unnormalized. 1.0 = 1 core)
    U32        last_update_time;    // timer marker when this remote process info was last refreshed (i.e., received in a launcher update)
    int        seconds_since_update;// elapsed time since rpi refreshed, used to detect stale data reporting
} RemoteProcessInfo;

/**
* Indicate that this is not a real command, but a placeholder when this vlaue
* is set as a str_idx in a @ref LineTracker entry.
*/
#define FAKE_STR_IDX -1
#define FAKE_ROW_PLACEHOLDER "(-nullrowplaceholder-)"

typedef struct LineTracker
{
    union
    {
        int        ival;
        F32        fval;
        int        str_idx;
    };
    U32        idx : 31;
    U32        is_str : 1;
    size_t    size;
} LineTracker;

typedef struct
{
    U32        idx : 31;
    U32        add : 1;
} RowAddDel;

typedef struct LineList
{
    LineTracker    *lines;
    int            count;
    int            max_lines;
    char        *text;
    int            text_count;
    int            text_max;
    RowAddDel    *row_cmds;
    int            cmd_count;
    int            cmd_max;
} LineList;

#define DBCONTAINER_MEMBERS \
    LineList    line_list;    \
    char    *raw_data;        \
    U32        container_data_size; \
    int        id;                    \
    U32        on_since;            \
    U32        lock_id;            \
    struct                        \
    {                            \
        U32        type : 8;        \
        U32        active : 1;        \
        U32        locked : 1;        \
        U32        is_deleting : 1;    \
        U32        just_created : 1;    \
        U32        demand_loaded : 1;    \
        U32        dont_save : 1; \
    };


typedef struct DbContainer {
    DBCONTAINER_MEMBERS
} DbContainer;

typedef struct DoorCon
{
    DBCONTAINER_MEMBERS
    Vec3    pos;            // only used by doors
    U32        tag;
    int        door_active_map;
    int        map_id;
} DoorCon;

typedef struct EntCon
{
    DBCONTAINER_MEMBERS
    char    ent_name[64];    // only used by ents -- DGNOTE 7/23/2010 bumped to 64 to make room for a trailing ": shardName"
    char    account[32];
    U32        auth_id;
    int        waiting_mapserver_ack;

    NetLink    *callback_link;
    U8        is_gameclient;

    int        teamup_id;
    int        supergroup_id;
    int        taskforce_id;
    int        raid_id;
    int        levelingpact_id;
    int        league_id;

    // search parameters for online player info
    int        lfgfield;
    int        level;
    int        hidefield;
    char    origin[128];
    char    archetype[128];
    char    comment[(128*3) + 1];        // UTF8 format can be 3 bytes for each UTF16 char in the SQL database
    int        access_level;

    char    auth_user_data[AUTH_BYTES * 2 + 1];    // actually AUTH_BYTES bytes of binary, but it's null terminated hex string here
    int        map_id;
    int        static_map_id;

    U8        arena_map : 1;            // They're on an arena map
    U8        mission_map : 1;        // They're in a mission map
    U8        can_co_faction : 1;        // Their map allows hero/villain teaming
    U8        faction_same_map : 1;    // They can't accept cross-faction invites from another map
    U8        can_co_universe : 1;    // Their map allows primal/Praetorian teaming
    U8        universe_same_map : 1;    // Can only accept cross-universe invites from their own map

    int        banned; // true if player is not allowed to login this entity
    char    acc_svr_lock[ORDER_ID_STRING_LENGTH]; // set if the account server is operating on this entity

    U8        connected; // really just a bit, but need to take address of it
    U32        auth_user_data_valid : 1;
    U32        is_map_xfer : 1;
    U32        logging_in : 1;
    U32        handed_to_mapserver : 1; // Used for tracking the tiny bit of time between a
        // user choose a character and it being sent to a mapserver and the moment
        // where the mapserver acknowledges receiving it

    U32        client_ip;    // IP address from when client logged in.

    U32        last_active;
    U32        member_since;
    int        prestige;
    U8        playerType;
    U8        playerSubType;
    U8        praetorianProgress;
    U8        playerTypeByLocation;
    U8        gender;
    U8        nameGender;
    char    primarySet[64];
    char    secondarySet[64];
    OrderId **completedOrders;

} EntCon;


#define SERVERCONTAINER_MEMBERS                    \
    DBCONTAINER_MEMBERS                            \
    NetLink    *link;                                \
    RemoteProcessInfo    remote_process_info;    \
    int        seconds_since_update;

// Mapserver or Launcher or ServerApp
typedef struct ServerCon
{
    SERVERCONTAINER_MEMBERS
} ServerCon;

// Mapserver
typedef struct MapCon
{
    SERVERCONTAINER_MEMBERS
    U8        is_static;
    U8        edit_mode; // someone is running a local mapserver for map editing
    U8        starting;
    U32        ip_list[2];    // 0 = local, 1 = remote    only used by maps
    U32        udp_port;
    U32        tcp_port;
    char    patch_version[100];
    char    map_name[1024];
    char    map_key[1024];
    char    mission_info[1024];
    int        map_running;
    int        num_players;
    int        num_players_connecting;
    int        num_players_waiting_for_start;
    int        num_players_sent_since_who_update;
    int        num_hero_players;
    int        num_villain_players;
    int        num_monsters;
    int        num_ents;
    F32        ticks_sec; // For performance monitoring
    F32        long_tick; // For performance monitoring
    int        num_relay_cmds; // For performance monitoring
    int        average_ping; // Average ping of all connected players
    int        min_ping; // Minimum ping of all connected players
    int        max_ping; // Maximum ping of all connected players
    int        launcher_id; // id of the launcher who was assigned to launch this map
    int        cookie; // The last cookie handed off to a launcher for starting this specific map
    U32        lastRecvTime; // Last time we received a who update (i.e. the mapserver is in it's main loop)

    int        dontAutoStart;
    int        transient;
    int        introZone;
    int        base_map_id;
    int        safePlayersLow;
    int        safePlayersHigh;

    int        mapgroup_id;
    NetLink    *callback_link;
    int        callback_idx;
    char    status[16]; // Used by ServerMonitor to store whether this is a crashed or stuck mapserver

    U8        shutdown;    // Tell this mapserver to shutdown immediately on connection, used if a team abandons a mapserver before it starts up
    int        deprecated;
} MapCon;

// Launcher
typedef struct LauncherCon
{
    SERVERCONTAINER_MEMBERS
    U32        flags;            
    int        num_processors;    // number of CPU cores on host machine.
    U32        cpu_usage;        // system reported CPU usage percentage (from "\Processor Information(_Total)\% Processor Time")
    U32        net_bytes_sent;    // network bytes sent per second
    U32        net_bytes_read;    // network bytes read per second
    U32        mem_total_phys; // host machine total physical memory in KB
    U32        mem_total_virt;    // host machine commit limit in KB
    U32        mem_avail_phys;    // host machine available physical memory in KB
    U32        mem_avail_virt;    // host machine available address space that can be committed in KB
    U32        mem_peak_virt;    // host machine commit peak in KB
    U32        process_count;    // host machine number of processes
    U32        thread_count;    // host machine number of threads
    U32        handle_count;    // host machine number of open handles
    U32        pages_input_sec;// hard fault count of 4K page reads from paging store/memory mapped files
    U32        pages_output_sec;// hard fault count of 4K page writes to paging store/memory mapped files
    U32        page_reads_sec;    // read *transactions* necessary to load "pages input", implies seeks "\Memory\Page Reads/sec"
    U32        page_writes_sec;// write *transactions* necessary to save "pages output", implies seeks "\Memory\Page Writes/sec"
    U32        heuristic;        // The last heuristic calculated for this machine
    int        crashes;        // Number of times a mapserver has crashed on this launcher
    int        delinks;        // Number of times a launcher at this IP address has been delinked
    int        num_mapservers; // Used by ServerMonitor to store a count of mapservers on this machine
    int        num_crashed_mapservers; // Used by ServerMonitor to store a count of crashed mapservers on this machine
    U32        trouble_suspend_expire;    // When a launcher suspension will expire
    U32        suspension_flags;    // flag launch suspensions currently in effect, e.g. manual, trouble, capacity
    U32        num_static_maps;    // total static maps on launcher host (all dbservers)
    U32        num_mission_maps;    // total mission maps on launcher host (all dbservers)
    U32        num_server_apps;    // total server apps on launcher host (all dbservers)
    U32        num_static_maps_starting;    // total static maps on launcher host still starting (all dbservers)
    U32        num_mission_maps_starting;    // total mission maps on launcher host still starting (all dbservers)
    U32        num_mapservers_all_shards;    // num_static_maps + num_mission_maps, i.e. total of *tracked* mapservers (i.e. that the launcher launched)
    U32        num_mapservers_raw;            // raw count of mapservers on host, tracked or not (good for sanity check and detect when launcher is not in sync)
    U32        num_starts_total;            // total number of processes that have been started on launcher by the service (all dbservers) since launcher was started
    F32        avg_starts_per_sec;            // average start rate of processes per second experienced by launcher since last update (for all dbservers using the launcher host)
    U32        num_connected_dbservers;    // the number of dbservers this launcher is currently connected to and serving
    U32        num_crashes_raw;            // reported number of currently crashed processes on the launcher host

    U32        host_utilization_metric;    // calculated metric based on other launcher host data
    U32        num_launched_static_since_update;        // count which helps us avoid ganging up on one launcher until we get a status update
    U32        num_launched_mission_since_update;        // count which helps us avoid ganging up on one launcher until we get a status update
} LauncherCon;

// Server app
typedef struct ServerAppCon
{
    SERVERCONTAINER_MEMBERS
    char appname[32];
    int launcher_id;
    U32 ip;
    int cookie; // The last cookie handed off to a launcher for starting this specific server
    int crashed;
    int monitor;
    char status[16]; // Used by ServerMonitor to store whether this is a crashed or running app
    int lastUpdateTime;
} ServerAppCon;

typedef struct GroupCon
{
    DBCONTAINER_MEMBERS
    int        member_id_count;
    int        member_id_max;        // current size of member_ids array
    int        *member_ids;
    U32        *member_ids_loaded;
    int        map_id; // mission map linked to this team. kill it if the team gets deleted
    int        leader_id; // team leader id
    char    name[256];            // MK: valid for supergroups, and for some magical reason, mapgroups
} GroupCon;

typedef struct ShardAccountInventoryItem {
    SkuId sku_id;
    int granted_total;
    int claimed_total;
    U32 expires;
} ShardAccountInventoryItem;

typedef struct ShardAccountCon {
    DBCONTAINER_MEMBERS
    int                            slot_count;
    int                            authoritative_reedemable_slots;
    char                        account[32];
    int                            was_vip;
    int                            showPremiumSlotLockNag;

    int                            inventorySize;
    ShardAccountInventoryItem    **inventory;
    union {
        U8                    loyaltyStatus[LOYALTY_BITS/8];
        U32                    loyaltyStatusOld[LOYALTY_BITS/sizeof(U32)/8];
    };
    int loyaltyPointsUnspent;
    int loyaltyPointsEarned;
    U32    accountStatusFlags;    // see AccountStatusFlags

    U32        logged_in : 1;
} ShardAccountCon;

typedef void (*ShardAccountInventoryItemCallback)(ShardAccountInventoryItem *item, char *data);

typedef struct
{
    char    *name;
    int        type;
    int        offset;
    int        cmd;
    int        member_of;
    void    (*callback)(int id,void *data);
    void*    (*array_allocate)();
} SpecialColumn;

enum
{
    CMD_MEMBER = 1,
    CMD_SETIFNULL,
    CMD_CALLBACK,
    CMD_ARRAY,
};

#define OFFSET(struct_name,field_name) (int)(&struct_name.field_name) , (int)(&struct_name)
typedef void (*StatusCallback)(AnyContainer *container,char *buf);
typedef void (*ContainerUpdateCallback)(AnyContainer *container);

typedef struct DbList
{
    char                    name[128];
    char                    fname[128];
    DbContainer                **containers;
    int                        num_alloced;
    int                        max_alloced;
    int                        in_use;
    int                        id;
    int                        struct_size;
    int                        max_container_id;
    ContainerTemplate        *tplt;
    SpecialColumn            *special_columns;
    StatusCallback            container_status_cb;
    ContainerUpdateCallback    container_updt_cb;
    StashTable                cid_hashes;
    StashTable                other_hashes;        // Currently, this is just for Ents and ShardAccounts by account

    U32                        owner_lockid;        // MAK - container servers own the entire list
    U32                        owner_notify_mask;    // this container server would like notifications for these containers
    U32                        max_session_id;        // high-water mark of id's touched while dbserver is running
} DbList;

#define CONTAINERADDREM_FLAG_UPDATESQL (1<<0)
#define CONTAINERADDREM_FLAG_NOTIFYSERVERS (1<<1)

char *containerGetText(AnyContainer *container);
char *findFieldTplt(ContainerTemplate *tplt,LineList *list,char *field,char *buf);
DbList *dbListPtr(int idx);
DbList *containerListRegister(int id, char *name, int struct_size, StatusCallback status_cb, ContainerUpdateCallback update_cb, SpecialColumn *special_columns);
void unsetSpecialColumns(DbList *list,DbContainer *container);
AnyContainer *containerUpdate(DbList *list,int id,char *diff,int update_sql);
AnyContainer *containerUpdate_notdiff(DbList *list,int id,char *str);
AnyContainer *containerLoad(DbList *list,int id);
void containerUnload(DbList *list,int id);
void containerDelete(AnyContainer *container_void);
AnyContainer *containerPtr(DbList *list,int idx);
AnyContainer *containerAlloc(DbList *list,int idx);
int containerLock(AnyContainer *container_any,U32 lock_id);
int containerUnlock(AnyContainer *container_any,U32 lock_id);
void containerSendList(DbList *list,int *selections,int count,int lock,NetLink *link,U32 user_data);
void containerSendStatus(DbList *list,int *selections,int count,NetLink *link,U32 user_data);
int *listGetAll(DbList *list,int *ent_count,int active_only);
void containerSendListByMap(DbList *list,int map_id,NetLink *link);
int inUseCount(DbList *list);
void containerSendInfo(NetLink *link);
MapCon *containerFindByMapName(DbList *list,char *name);
MapCon *containerFindByIp(DbList *list,U32 ip);
MapCon *containerFindBaseBySgId(int sgid);
EntCon *containerFindByAuthId(U32 auth_id);
ShardAccountCon *containerFindByAuthShardAccount(const char *auth);
int containerVerifyMapId(DbList *list,char* value);
int containerIdFindByElement(DbList *list,char *element,char *value);
void containerSetMemberState(DbList *list,int id,int member_id,int loaded);
void containerAddMember(DbList *list, int id, int member_id, int flags);
void containerRemoveMember(DbList *list, int id, int member_id, int deleting_container, int flags);
void containerUnlockById(int lock_id);
AnyContainer *containerLoadCached(DbList *list,int id,char **cache_data);
bool containerGetField(DbList *list, DbContainer *container, int field_type, char *field_name, char * ret_str, int * ret_int, F32 *ret_f32 );


// End mkproto
#endif
