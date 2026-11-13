#ifndef _AUTHCLIENT_H
#define _AUTHCLIENT_H

#include "auth/auth.h"
#include "auth/authconn.h"
#include "auth/account_creation.h"

#define AUTH_SERVER_PORT      2106

typedef enum ShardType
{
    kShardType_None,
    kShardType_Live = 1,
    kShardType_Training = 4,
    kShardType_Count
} ShardType;

#define        SERVER_NAME_SIZE        256

typedef struct
{
    int        id;
    U32        ip;
    U32        port;
    U8        age_limit;
    U8        pk_flag;
    U8        server_status;
    U8        isVIP;
    U32        curr_user_count;
    U32        max_user_count;
    U32        server_type;
    char    name[SERVER_NAME_SIZE];
} ServerInfo;

typedef struct
{
    int        reason;
    char    *msg;
} BlockedAccount;

typedef enum Auth2EnabledState
{
    Auth2EnabledState_None,
    Auth2EnabledState_Direct,
    Auth2EnabledState_QueueServer,
    Auth2EnabledState_Count,
} Auth2EnabledState;

typedef struct
{
    int                fail_type;
    int                reason;
    int                block_count;
    BlockedAccount    *blocks;
    int                server_count;
    ServerInfo        *servers;

    U32                pay_stat;
    U32                loyalty;
    U32                remain_time;
    U32                quota_time;
    U32                warn_flag;
    U32                login_flag;
    U32                update_key[2];

    U32                game_key;
    U32                uid;
    int                server_id;
    int                last_login_server_id;
    char            name[128];
    U32                passwordMD5[4];
    int                protocol;
    U8            RSA_mod[400];
    int            RSA_modlen;
    U8            RSA_exp[10];
    int            RSA_explen;
    Auth2EnabledState auth2_enabled; // for when we know auth2 protocol is active. Remove this field after migration. 
} AuthInfo;



extern AuthInfo        auth_info;

typedef enum AuthFlags
{
    AuthFlags_PC,
    AuthFlags_MAC,
    // 16 bit max
    AuthFlags_Max
} AuthFlags;
STATIC_ASSERT(AuthFlags_Max < 0x10000);


/* Server results retain their wire values; local failures are separate from login errors. */
typedef enum AuthCreateAccountResult
{
    AUTH_CREATE_SUCCESS = ACCOUNT_CREATE_SUCCESS,
    AUTH_CREATE_DISABLED = ACCOUNT_CREATE_DISABLED,
    AUTH_CREATE_INVALID_USERNAME = ACCOUNT_CREATE_INVALID_USERNAME,
    AUTH_CREATE_INVALID_PASSWORD = ACCOUNT_CREATE_INVALID_PASSWORD,
    AUTH_CREATE_NAME_TAKEN = ACCOUNT_CREATE_NAME_TAKEN,
    AUTH_CREATE_BLOCKED_IP = ACCOUNT_CREATE_BLOCKED_IP,
    AUTH_CREATE_INVALID_REQUEST = ACCOUNT_CREATE_INVALID_REQUEST,
    AUTH_CREATE_INVALID_STATE = ACCOUNT_CREATE_INVALID_STATE,
    AUTH_CREATE_DATABASE_FAILURE = ACCOUNT_CREATE_DATABASE_FAILURE,
    AUTH_CREATE_CONNECTION_ERROR = 256,
    AUTH_CREATE_TIMEOUT,
    AUTH_CREATE_PROTOCOL_ERROR,
    AUTH_CREATE_ENCRYPTION_ERROR,
    AUTH_CREATE_BUSY,
    AUTH_CREATE_UNCONFIRMED,
    /* An asynchronous request is still resolving, connecting, or awaiting AuthServer. */
    AUTH_CREATE_PENDING
} AuthCreateAccountResult;

/* Begin an asynchronous request using complete plaintext strings. Main thread
 * only, at the initial login screen with no active DbServer/map connection or
 * shard transfer. A valid request replaces any leftover auth connection with a
 * fresh connection and handshake. Invalid inputs or disallowed client states
 * leave that connection alone.
 * Copies the credentials before returning; the caller may immediately clear them.
 * Returns PENDING on acceptance, otherwise an immediate terminal result without
 * changing login state. Poll once per frame until a terminal result is returned.
 * TestClient does not support registration and always returns BUSY. */
AuthCreateAccountResult authCreateAccountBegin(const char *name, const char *password);

/* Advance the active request without waiting for DNS, connection, or packets.
 * Terminal results disconnect and restore login state. Polling while inactive
 * returns INVALID_STATE. Main thread only; not reentrant. */
AuthCreateAccountResult authCreateAccountPoll(void);

/* Abandon any active request, disconnect, clear credentials, and restore login
 * state. Does not wait for DNS; cannot undo a request already sent. Main thread. */
void authCreateAccountCancel(void);

/* Return a static English explanation suitable for the console or an OK dialog.
 * An unconfirmed result may have committed; do not automatically retry it. */
const char *authCreateAccountResultMessage(AuthCreateAccountResult result);

/* Synchronously create an account from complete plaintext strings. Main thread
 * only, at the initial login screen with no active DbServer/map connection or
 * shard transfer. A valid request replaces any leftover auth connection.
 * Preserves login state, opens a fresh auth connection, then always disconnects.
 * Timeout/unconfirmed outcomes may have committed; never automatically retry. */
AuthCreateAccountResult authCreateAccount(const char *name, const char *password);

// Generated by mkproto
void acCleanup(void);
void acSendLogin(const char* account, const char* password, int subscription, AuthFlags flags);
void acSendServerList();
void acSendAboutToPlay(int server_id);
void acSendLogout();
void acGetProtocolVer(AuthPacket *pak);
void acGetLoginOK(AuthPacket *pak);
void acGetLoginFail(AuthPacket *pak);
void acGetSendServerList(AuthPacket *pak);
void acGetSendServerFail(AuthPacket *pak);
void acGetPlayOK(AuthPacket *pak);
void acGetPlayFail(AuthPacket *pak);
void acGetBlockedAccount(AuthPacket *pak);
void acGetBlockedAccountWithMsg(AuthPacket *pak);
void acGetAccountKicked(AuthPacket *pak);
int authWaitFor(enum AuthToClientCmd wait_for_cmd);
int authLogin(char *name,char *password);
char *authGetError();
int authLogout();
int authReloginIfNeeded(char *password);
bool authIsErrorAccountAlreadyLoggedIn();
bool authIsErrorAccountNotPaid();
// End mkproto
#endif
