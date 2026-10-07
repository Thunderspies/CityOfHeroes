#ifndef _DBCOMM_H
#define _DBCOMM_H

#include <utilitieslib/network/netio.h>
#include "dbcontainer.h"

//typedef U32        xcontainer_update_cb(ContainerInfo *ci,int type);
typedef void (*DbCustomDataCallback)(Packet *pak,int db_id,int count);

typedef struct Entity  Entity;

typedef struct
{
    int        map_id;
    int        base_map_id;
    int        instance_id;
    int        time_offset;
    int        cookie;
    char    map_name[256];
    char    server_name[256];
    char    log_server[256];
    char    who_msg[4096];
    U32        local_server : 1;
    U32        from_launcher : 1;
    U32        query_mode : 1;
    U32        server_quitting : 1;
    U32        is_static_map : 1;
    U32        staticlink : 1;
    U32        silence_all : 1;
    U32        map_registered : 1;
    U32        has_gurneys : 1;
    U32        has_villain_gurneys : 1;
    U32        is_endgame_raid : 1;
    U32        diff_debug;
    int        last_id_acked;
    int        sg_eldest_on;
    char    sg_eldest_on_name[128];
    char    *server_quitting_msg;

    char    *map_data;            // estr
    int        map_supergroupid;
    int        map_userid;
    U32        map_from_db : 1;

    F32        timeZoneDelta;
    // dbcontainer callbacks
    //U32 (*container_update_cb)(ContainerInfo *ci,int type);
    //void (*container_broadcast_cb)(int container,int *members,int count,char *msg,int msg_type, int senderID, int container_type);
    //void (*lost_membership_cb)(int list_id,int container_id,int ent_id);
    //void (*container_ack_cb)(int list_id,int container_id);

    // dbcomm callbacks
    //void (*force_logout_cb)(int db_id,int reason);
    //void (*shutdown_cb)(int disconnect,char *msg);
    //void (*relay_cmd_cb)(char *str);
    //void (*server_synch_time_cb)();
} DBCommState;

extern DBCommState db_state;
extern NetLink db_comm_link;
extern int relay_response_succeeded, relay_response_map_id;

enum
{
    DBCOMM_ASYNC_WAITING = -2,
    DBCOMM_ASYNC_FAILED = -1,
};

typedef struct PerformanceInfo PerformanceInfo;
typedef struct ClientLink ClientLink;

int dbProcessDataCallback(int cookie,int data);
int dbSetDataCallback(int *data);
void dbCommStartup(void);
void dbCommShutdown(void);
void dbAsyncReturnAllToStaticMap(const char *msg0, const char *msg1);
int dbMessageScan(int lookfor);
int dbMessageScanUntil(const char* name, PerformanceInfo** perfInfo);
int dbMessageScanUntilTimeout(const char* name, PerformanceInfo** perfInfo, int timeout);
void dbSaveEditMapName(char *name);
void dbReadyForPlayers(void);
int dbRegister(int map_id,U32 *ip_list,int udp_port,int tcp_port,int cookie);
void dbSetupMap(void);
int dbTryConnect(F32 timeout, int port);
void dbComm(void);
void handleShutdown(Packet *pak);
void handleRelayCmd(Packet *pak);
int handleClientCmdFailed(Packet *pak);
void handleCustomData(Packet *pak);
int dbMessageCallback(Packet *pak,int cmd,NetLink *link);
void dbSendSaveCmd(void);
void dbSendPlayerDisconnect(int db_id, int logout, int logout_login);
U32 dbSecondsSince2000(void);
void contactLauncher(void);
void dbPlayerKicked(int player_id,int banned);
void dbReqArenaAddress(int syncronous);
void dbReqCustomData(int list_id,char *table,char *limit,char *search,char *columns,DbCustomDataCallback cb,int db_id);
// Administrative escape hatch: SQL uses the selected provider's native dialect.
void dbExecuteAdministrativeNativeSql(char *sql_command);
void dbReqSgChannelInvite(Entity * e, char * channel, int min_rank);
void dbOfflineCharacter(ClientLink* client, char* dbIdStrOrCharName);
void dbOfflineCharacter_HandleRslt(Packet *pak);
void dbRestoreDeletedChar(ClientLink* client, char* paramStr );    // auth, char, date str (MM/YYYY)
void dbRestoreDeletedChar_HandleRslt(Packet *pak);
void dbListDeletedChars(ClientLink* client, char* paramStr);    // auth, date str (MM/YYYY)
void dbListDeletedChars_HandleRslt(Packet *pak);
void dbOverrideStartZone(int map_id);
void dbRequestPlayNCAuthKey(Entity * e, Packet *pak);

void dbForceRelayCmdToMapByEnt(int entid, FORMAT fmt, ...);
// this is mostly for non-maps, it does no lookup, it always relays, and will load offline characters

void dbDelinkMeWrapper(char *errorMsg);
void dbDelinkMe(char *errorMsg);

U32 dealWithContainer(ContainerInfo *ci,int type);
void dealWithEntAck(int list_id,int id);
void dealWithShardAccountAck(int list_id,int id);
void forceLogout(int db_id,int reason);
void returnAllPlayers(int disconnect,char *msg);

void dealWithContainerRelay(Packet* pak, U32 cmd, U32 listid, U32 cid);
void dealWithContainerReceipt(U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data, U32 err, char* str, Packet* pak);
void dealWithContainerReflection(int list_id, int* ids, int deleting, Packet* pak, ContainerReflectInfo** reflectinfo);

void dbLog(char const *cmdname, Entity *e, FORMAT fmt, ...);

void relayCommandLogToggle(void);
void testClientSetDb(const char *server_name);

int dbRelayGetAccountServerCharacterCounts(U32 auth_id, bool vip);
int dbRelayGetAccountServerInventory(U32 auth_id);
int dbRelayGetAccountServerCatalog(ClientLink *client);

#endif
