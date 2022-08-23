#include "auth/auth.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/network/netio.h>
#include "dbserver/servercfg.h"
#include <stdio.h>
#include <string.h>
#include "mapxfer.h"
#include "comm_backend.h"
#include "clientcomm.h"
#include "dbinit.h"
#include "queueservercomm.h"
#include "sql_fifo.h"
#include "auth/authUserData.h"
#include "../Common/ClientLogin/clientcommLogin.h"
#include "accountservercomm.h"
#include "dbmsg.h"
#include "entity/entVarUpdate.h"
#include "container/containerbroadcast.h"
#include <utilitieslib/utils/StringUtil.h> // binStrToHexStr
#include <utilitieslib/utils/log.h>

typedef struct
{
    U32        id;
    U32        cookie;
    U32        created;
    U8        user_data[AUTH_BYTES];
//     U8        game_data[AUTH_BYTES];
    char    name[32];
    U32        time_remaining;
    U32        reservations;
    U32        pay_stat;
    U32        loyalty;
    U32        loyaltyLegacy;
} AuthEntry;

#define AUTH_CACHE_SIZE 4096
#define AUTH_CACHE_MASK (AUTH_CACHE_SIZE-1)

static AuthEntry auth_cache[AUTH_CACHE_SIZE];
static AuthPacket    s_authpak = {0};

static AuthEntry *authEntryAlloc()
{
    int            i,idx;
    U32            time,cookie;
    AuthEntry    *entry;
    S64        seed64;
    U32        seed;

    GET_CPU_TICKS_64(seed64);
    seed = (U32)seed64;

    cookie = rule30Rand() ^ seed;
    time = timerSecondsSince2000();
    for(i=0;i<AUTH_CACHE_SIZE;i++)
    {
        idx = (i + cookie) & AUTH_CACHE_MASK;
        entry = &auth_cache[idx];
        if (!entry->id || (time - entry->created) > 30)
            break;
    }
    entry->cookie = (cookie & ~AUTH_CACHE_MASK) | idx;
    entry->created = time;
    return entry;
}

static AuthEntry* authentryFromId(U32 auth_id)
{
    int i;
    for( i = 0; i < ARRAY_SIZE(auth_cache); ++i ) 
    {
        if(auth_cache[i].id == auth_id)
            return auth_cache + i;
    }
    return NULL;
}


char *trimTrailingWhitespace(char *s)
{
    char *e = s + strlen(s) - 1;
    while (e >= s && *e==' ') *e--=0;
    return s;
}

int authValidLogin(U32 auth_id,U32 cookie,char *name,U8 user_data[AUTH_BYTES],U32 *loyalty,U32 *loyaltyLegacy,U32 *payStat)
{
    AuthEntry    *entry;

    if (!auth_id)
        return 0;
    entry = &auth_cache[cookie & AUTH_CACHE_MASK];
    if (entry->id == auth_id && entry->cookie == cookie && stricmp(trimTrailingWhitespace(name),entry->name)==0)
    {
        strcpy(name,entry->name);
        memcpy(user_data,entry->user_data,sizeof(entry->user_data));
        *loyalty = entry->loyalty;
        *loyaltyLegacy = entry->loyaltyLegacy;
        *payStat = entry->pay_stat;
        //*time_remaining = entry->time_remaining;
        //*reservations = entry->reservations;

        LOG_OLD( "%s 0x%s copied to client link", entry->name, binStrToHexStr(entry->user_data, AUTH_BYTES));

        memset(entry,0,sizeof(*entry));
        return 1;
    }
    if (entry->id == auth_id && entry->cookie == cookie)
    {
        // The Id and Cookie match but the Name doesn't!  AuthServer bug!
        LOG_OLD_ERR( "AuthServer bug!  User provided AuthId %d Cookie %d name \"%s\", entry had name \"%s\"", auth_id, cookie, name, entry->name);
    }
    return 0;
}

static void authInvalidateLogin(int auth_id)
{
    int        i;
    AuthEntry    *entry;

    for(i=0;i<AUTH_CACHE_SIZE;i++)
    {
        entry = &auth_cache[i];

        if (entry->id == auth_id)
            memset(entry,0,sizeof(*entry));
    }
}

///////////////////////////// send to auth //////////////////////////////////
void authSendPlayOK(int auth_id, int one_time_pwd )
{
    AuthPacket    *pak = authAllocPacket(AS_PLAY_OK);
    authdbg_printf("sent: AS_PLAY_OK\n");


    authPutU32(pak,auth_id);
    authPutU32(pak,one_time_pwd);
    authSendPacket(pak);
}

void authSendPlayFail(int auth_id, int error)
{
    AuthPacket    *pak = authAllocPacket(AS_PLAY_FAIL);
    authdbg_printf("sent: AS_PLAY_FAIL\n");


    authPutU08(pak,error);
    authPutU32(pak,auth_id);
    authSendPacket(pak);
}

void authSendPlayGame(int auth_id)
{
    AuthPacket    *pak = authAllocPacket(AS_PLAY_GAME);
    authdbg_printf("sent: AS_PLAY_GAME\n");


    authPutU32(pak,auth_id);
    // ab: auth2 docs say send 8 bytes here. no reason given.

    authSendPacket(pak);
}

void authSendQuitGame(int auth_id, short reason, int usetime)
{
    AuthPacket    *pak = authAllocPacket(AS_QUIT_GAME);
    authdbg_printf("sent: AS_QUIT_GAME\n");

    if (!auth_id)
        return;

    authPutU32(pak,auth_id);
    authPutU16(pak,reason);
    authPutU32(pak,usetime);
    authSendPacket(pak);
}

void authSendKickAccount(int auth_id,short reason)
{
    AuthPacket    *pak = authAllocPacket(AS_KICK_ACCOUNT);
    authdbg_printf("sent: AS_KICK_ACCOUNT\n");


    authPutU32(pak,auth_id);
    authPutU16(pak,reason);
    authSendPacket(pak);
}

void authSendBanAccount(int auth_id,short reason)
{
    AuthPacket    *pak = authAllocPacket(AS_BAN_USER);
    authdbg_printf("sent: AS_BAN_USER\n");


    authPutU32(pak,auth_id);
    authPutU16(pak,reason);
    authSendPacket(pak);
}

void authSendServerNumOnline(int num_online)
{
    AuthPacket    *pak = authAllocPacket(AS_SERVER_USER_NUM);
    authdbg_printf("sent: AS_SERVER_USER_NUM\n");

    authPutU16(pak,num_online);
    if (server_cfg.min_players && 0 /*sqlFifoNearFull()*/)
        authPutU16(pak,server_cfg.min_players);
    else
        authPutU16(pak,AUTH_SIZE_MAX+server_cfg.max_players);
    authSendPacket(pak);
}

void authSendVersion(int version)
{
    AuthPacket    *pak = authAllocPacket(AS_VERSION);
    authdbg_printf("sent: AS_VERSION\n");


    authPutU32(pak,version);
    authSendPacket(pak);
}

void authSendUserData(int auth_id, U8 data[AUTH_BYTES])
{
    AuthPacket    *pak = authAllocPacket(AS_WRITE_GAME_DATA);
    authdbg_printf("sent: AS_WRITE_USERDATA\n");


    authPutU32(pak,auth_id);
    authPutArray(pak,data,AUTH_BYTES);
    authSendPacket(pak);
}

void authSendPing(U32 id)
{
    AuthPacket    *pak = authAllocPacket(AS_PING);
    authdbg_printf("sent: AS_PING\n");


    authPutU32(pak,id);
    authSendPacket(pak);
}

void authSendSetConnect()
{
    AuthPacket    *pak = authAllocPacket(AS_SET_CONNECT);
    authdbg_printf("sent: AS_SET_CONNECT\n");

    authPutU32(pak,cfg_VIPShard());
    authSendPacket(pak);
}

void authReconnectUser(AuthPacket *pak,char *account,U32 auth_id)
{
    authPutStr(pak,account);    // Account name
    authPutU32(pak,auth_id);    // Account UID
    authPutU32(pak,0);            // Payment status
    authPutU32(pak,0);            // Login  IP
    authPutU32(pak,0);            // loginflag
    authPutU32(pak,0);            // warnflag
}

void authReconnectOnePlayer(char *account,U32 auth_id)
{
    if (!authIsGlobalAuth())
    {
        authSendPlayGame(auth_id);
    }
    else
    {
        AuthPacket *pak = authAllocPacket(AS_PLAY_USER_LIST);
        authdbg_printf("sent: AS_PLAY_USER_LIST\n");

        authPutU32(pak,1);
        authReconnectUser(pak,account,auth_id);
        authSendPacket(pak);
    }
}

void authSendCurrentPlayers()
{
    if(!server_cfg.queue_server)
        reconnectClientCommPlayersToAuth();
    else
        reconnectQueueserverCommPlayersToAuth();

    if (!authIsGlobalAuth())
    {
        int i;

        for (i=0; i<shardaccounts_list->num_alloced; i++)
        {
            ShardAccountCon *con = cpp_reinterpret_cast(ShardAccountCon*)(shardaccounts_list->containers[i]);
            if (con->logged_in)
                authReconnectOnePlayer(con->account, con->id);
        }
    }
}

// handled by: handleRecvGameData
static void authSendGameDataReq(int id)
{
    AuthPacket    *pak = authAllocPacket(AS_READ_GAME_DATA);
    authdbg_printf("sent: AS_READ_GAME_DATA\n");
    authPutU32(pak,id);
    authSendPacket(pak);
}

void authSendShardTransfer(int auth_id, int shard)
{
    AuthPacket    *pak = authAllocPacket(AS_SHARD_TRANSFER);
    authdbg_printf("sent: AS_SHARD_TRANSFER\n");

    authPutU32(pak,auth_id);
    authPutU32(pak,shard);
    authSendPacket(pak);
}



///////////////////////////// receive from auth //////////////////////////////////

static void handleVersion(AuthPacket *pak) {
    U32 protocol, reactivationActive;
    const char *build;

    build = authGetStr(pak);
    protocol = authGetU32(pak);

    printf("AuthServer Git Commit Hash: %s, Protocol: %d\n", build, protocol);

    authSetKey(0,1);
    authSendCurrentPlayers();
    authSendSetConnect();

    if (protocol >= GR_REACTIVATION_PROTOCOL_VERSION) {
        reactivationActive = authGetU32(pak);
        printf("Reactivation is %sactive.\n", reactivationActive ? "" : "not ");
        authUserSetReactivationActive(reactivationActive);
    }
}

static void handleAboutToPlay(AuthPacket *pak)
{
    int                login_flag,warn_flag;
    AuthEntry        *entry;

    entry = authEntryAlloc();
    entry->id        = authGetU32(pak);
    strcpy(entry->name,authGetStr(pak));
    trimTrailingWhitespace(entry->name);
    entry->time_remaining    = authGetU32(pak);
    login_flag        = authGetU32(pak);
    warn_flag        = authGetU32(pak);
    authGetArray(pak,entry->user_data,AUTH_BYTES);
    entry->pay_stat = authGetU32(pak); // pay_stat. used to figure out if VIP
    // entry->reservations    = authGetU32(pak);
    entry->loyalty = authGetU32(pak); 
    entry->loyaltyLegacy = authGetU32(pak); 

    LOG_OLD( "%s 0x%s got user_data", entry->name, binStrToHexStr(entry->user_data, AUTH_BYTES));
    LOG_OLD( "Generated cookie for \"%s\" AuthId %d Cookie %d\n", entry->name, entry->id, entry->cookie);

    authSendGameDataReq(entry->id);
}

// Used by shard visitor, this runs on the destination shard, and sets up the AuthEntry struct necessary to allow the client to connect
int handleShardTransferRequest(int auth_id, char *name, unsigned char *auth_data)
{
    AuthEntry        *entry;

    // DGNOTE - Edison - For fake auth mode, you can probably just have this routine return 0; and do nothing else.
    entry = authEntryAlloc();
    entry->id = auth_id;
    strcpy(entry->name, name);
    trimTrailingWhitespace(entry->name);
    entry->time_remaining = 0;
    memcpy(entry->user_data, auth_data, AUTH_BYTES);
    entry->reservations    = 0;

    LOG_OLD( "Transfer %s 0x%s got user_data", entry->name, binStrToHexStr(entry->user_data, AUTH_BYTES));
    LOG_OLD( "Transfer Generated cookie for \"%s\" AuthId %d Cookie %d\n", entry->name, entry->id, entry->cookie);
    
    return entry->cookie;
}

static void handleKickAccount(AuthPacket *pak)
{
    int        reason;
    U32        auth_id;
    char    *account_name;
    DbList    *ent_list    = dbListPtr(CONTAINER_ENTS);
    EntCon    *e;
    int        not_kicked_by_access_level=0;
    bool kickSent = false;

    auth_id            = authGetU32(pak);
    reason            = authGetU08(pak);
    account_name    = authGetStr(pak);
    

    e = containerFindByAuthId(auth_id);
    if (e)
    {
        if (e->access_level >= 1)
        {
            if (e->active)
                containerBroadcastMsg(CONTAINER_ENTS,INFO_DEBUG_INFO,0,e->id,DBMSG_CHAT_MSG " The AuthServer said to kick you, but we ignored it because of your access level");
            authSendQuitGame( auth_id, 1, 0 );
            return;
        }
        else
        {
            dbForceLogout(e->id,reason);            
        }
    }
    authInvalidateLogin(auth_id);
    if(e)
        kickSent = clientcommKickLogin(auth_id, e->account,reason);
    if(!e && !kickSent)
        authSendQuitGame(auth_id, 1, 0);
}

static void handleNumOnline()
{
    authSendServerNumOnline(inUseCount(dbListPtr(CONTAINER_ENTS)) + clientLoginCount());
}

static void handlePing(AuthPacket *pak)
{
    U32        id;

    id = authGetU32(pak);
    authSendPing(id);
}

static void handleCompleteUserList(AuthPacket *pak)
{
    U32        num_processed;

    num_processed = authGetU32(pak);
}

void sendRemainTimeToClient(U32 auth_id,U32 curr_avail,U32 total_avail,U8 reservation,int countdown,int change_billing)
{
    EntCon    *e;

    printf("PaymentRemainTime: auth_id %d  curr_avail %d  total_avail %d  reservation %d  countdown %d  change_target %d\n",
        auth_id,curr_avail,total_avail,reservation,countdown,change_billing);

    e = containerFindByAuthId(auth_id);
    if (e)
    {
        char    buf[1000];

        printf("found ent, sending..\n");
        sprintf(buf,"%s %u %u %u %u %u",DBMSG_PAYMENT_REMAIN_TIME, curr_avail,total_avail,reservation,countdown,change_billing);
        containerBroadcastMsg(CONTAINER_ENTS,INFO_PAYMENT_REMAIN_TIME,0,e->id,buf);
    }
    else
        printf("could not find active player for auth_id %d\n",auth_id);
}

static void handlePaymentRemainTime(AuthPacket *pak,int change_billing)
{
    U32        auth_id,curr_avail,total_avail;
    U8        reservation,countdown;

    auth_id        = authGetU32(pak);
    curr_avail    = authGetU32(pak);
    total_avail    = authGetU32(pak);
    reservation    = authGetU08(pak);
    countdown    = authGetU08(pak);
    sendRemainTimeToClient(auth_id,curr_avail,total_avail,reservation,countdown,change_billing);
}

// this function can *only* be called before login, not once player
// is loaded.
// response from: authSendGameDataReq
static void handleRecvGameData(AuthPacket *pak)
{
    int i;
    U32 auth_id = authGetU32(pak);
    AuthEntry *entry =  authentryFromId(auth_id);
     U8        game_data[AUTH_BYTES];
    
    if(!entry)
    {
        LOG_OLD_ERR( "RecvGameData failed to find auth entry for id %i",auth_id);
        return;
    }

    authGetArray(pak,game_data,AUTH_BYTES);
    LOG_OLD("%s 0x%s got game_data", entry->name, binStrToHexStr(game_data, AUTH_BYTES));

    authUserMaskAuthBits(entry->user_data, AUTH_USER_AUTHSERVER);
    authUserMaskAuthBits(game_data, AUTH_USER_DBSERVER);

    for( i = 0; i < ARRAY_SIZE(game_data) ; ++i ) 
        entry->user_data[i] |= game_data[i];
    LOG_OLD("authbits", "%s 0x%s merged data (1)", entry->name, binStrToHexStr(entry->user_data, AUTH_BYTES));

    authSendPlayOK(entry->id,entry->cookie);
}

void authProcess()
{
    enum AuthToDbserverCmd type;

    while(authGetPacket(&s_authpak))
    {
        type = authGetU08(&s_authpak);
        switch(type)
        {
            xcase SQ_VERSION:
                authdbg_printf("recv: SQ_VERSION\n");
                handleVersion(&s_authpak);
            xcase SQ_ABOUT_TO_PLAY:
                authdbg_printf("recv: SQ_ABOUT_TO_PLAY\n");
                handleAboutToPlay(&s_authpak);
            xcase SQ_KICK_ACCOUNT:
                authdbg_printf("recv: SQ_KICK_ACCOUNT\n");
                handleKickAccount(&s_authpak);
            xcase SQ_SERVER_NUM:
                authdbg_printf("recv: SQ_SERVER_NUM\n");
                handleNumOnline(&s_authpak);
            xcase SQ_PING:
                authdbg_printf("recv: SQ_PING\n");
                handlePing(&s_authpak);
            xcase SQ_COMPLETE_USERLIST:
                authdbg_printf("recv: SQ_COMPLETE_USERLIST\n");
                handleCompleteUserList(&s_authpak);
#if 0           // obsolete Korean?
            xcase SQ_PAYMENT_REMAIN_TIME:
                authdbg_printf("recv: SQ_PAYMENT_REMAIN_TIME\n");
                handlePaymentRemainTime(&s_authpak,0);
            xcase SQ_PAYMENT_CHANGE_TARGET:
                authdbg_printf("recv: SQ_PAYMENT_CHANGE_TARGET\n");
                handlePaymentRemainTime(&s_authpak,1);
#endif
// Not supported.
//             xcase SQ_RECV_USER_DATA: 
//                 authdbg_printf("recv: SQ_RECV_USER_DATA\n");
//                 handleRecvUserData(&s_authpak);
// this function can *only* be called before login, not once player
// is loaded. 
            xcase SQ_RECV_GAME_DATA: 
                authdbg_printf("recv: SQ_RECV_GAME_DATA\n");
                handleRecvGameData(&s_authpak);
            break;
            default:
                LOG_OLD_ERR("Auth process recieved Unknown auth packet type: %d\n", type);
        }
        memset(&s_authpak,0,sizeof(s_authpak));
    }
}

int authCommInit(char *server,int port)
{

    if (server_cfg.fake_auth)
    {
        authFakeIt();
        printf("No auth server specified in servers.cfg - allowing all logins\n");
        return 1;
    }
    {
        extern int g_auth_always_check_first_packet;

        g_auth_always_check_first_packet = server_cfg.auth_bad_packet_reconnect;
    }
    ZeroStruct(&s_authpak);
    if (!authConnect(server,port))
        return 0;
    NMAddSock(authSocket(), (NMSockCallback *)authProcess, 0);
    authProcess();    // Fire off a single async receive operation to get the chain going.
    return 1;
}

void authCommDisconnect(void)
{
//    NMCloseSock(NMFindSock(authSocket()));
    authDisconnect(); // to memset auth_state, clear asyncOpContextes
}

int authDisconnected()
{
    if (server_cfg.fake_auth)
        return 0;

    if (!authSocketOk())
        authCommDisconnect();
    // Is the net master list still monitoring this socket?
    if(NMFindSock(authSocket()))
        return 0;
    else
        return 1;
}

