#include "clientcomm/authclient.h"
#include <utilitieslib/utils/osdependent.h>
#include "Des.h"
#include <string.h>
#include <stdio.h>
#include <utilitieslib/network/sock.h>
#include "comm_backend.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/version/AppRegCache.h>
#include <utilitieslib/network/crypt.h>
#include "game.h"
#include <utilitieslib/language/AppLocale.h>
#include "auth/authUserData.h"

AuthInfo    auth_info;
static        char    auth_error_msg[1000];

typedef enum AuthServerListType
{
    kAuthServerListType_None,
    kAuthServerListType_Normal = 1, // original AQ_SERVER_LIST_EXT packet
    kAuthServerListType_Extended = 3, // new from NCS: includes if the shard is test or not.    
} AuthServerListType;


static int session_id[2];
static AuthServerListType server_list_expected = kAuthServerListType_None;
static int login_ok_expected = 0;
static int play_ok_expected = 0;

void acCleanup(void)
{
    SAFE_FREE(auth_info.servers);
    SAFE_FREE(auth_info.blocks);
}

#include <utilitieslib/utils/utils.h>

static void dumpData(char *msg,const void *datav,int count)
{
#if 0
    int        i;
    unsigned char    *data = (unsigned char*)datav;

    printf("%s[%d]:",msg,count);
    for(i=0;i<count;i++)
        printf(" %02x",data[i]);
    printf("\n");
#endif
}

void acSendLoginMD5(const char* account_p, const char* password_p, int subscription, AuthFlags flags)
{
    AuthPacket    *pak=0;
    char        RSA_plain_buf[94];
    char        RSA_cipher_buf[512];
    U8            md5_buf[16],pass_hash[16];
    U64            sha512_buf[8];
    char        session[32];
    char        account[15],password[17];
    U32            RSA_cipher_size;

    memset(account,0,sizeof(account));
    strncpy(account,account_p,sizeof(account));
    removeLeadingAndFollowingSpaces(account);
    strlwr(account);


    memset(password,0,sizeof(password));
    strncpy(password,password_p,sizeof(password));

    login_ok_expected = 1;
    switch(auth_info.protocol)
    {
    case ASIA_AUTH_PROTOCOL_VERSION:
    {
        char    md5hash[30];
        
        pak = authAllocPacket(AQ_LOGIN_MD5_MD5);
        memset(md5hash, 0, sizeof(md5hash));
        memcpy(md5hash, account, 14);
        memcpy(md5hash+14, password, 16);
        
        dumpData("hash input ",md5hash,sizeof(md5hash));
        cryptMD5Update(md5hash, sizeof(md5hash));
        cryptMD5Final((U32 *)pass_hash);
        dumpData("hash(account+pass)",pass_hash,sizeof(pass_hash));
        
        DesKeyInit("9=3*2;x]11sa;/4sd.%+$@a*\'!2z0~g`{z]?n8(");
        authdbg_printf("send: AQ_LOGIN_MD5_MD5 %s\n", account);
    }
    break;
    case USA_AUTH_PROTOCOL_VERSION:
    case GLOBAL_AUTH_PROTOCOL_VERSION:
    case GR_REACTIVATION_PROTOCOL_VERSION:
    case OUROBOROS_PROTOCOL_VERSION_1:
    case OUROBOROS_PROTOCOL_VERSION_2:
    {
        char salt_buf[9];
        U32 salt;
        salt = cryptAdler32(account, strlen(account));
        snprintf(salt_buf,9,"%08x",salt);
        pak = authAllocPacket(AQ_LOGIN_MD5);
        authEncryptLineage2(password, pass_hash, 16);
        cryptSHA512Init();
        cryptSHA512Update(password, strlen(password));
        cryptSHA512Update(salt_buf, 8);
        cryptSHA512Final((U64*)sha512_buf);
        authdbg_printf("send: AQ_LOGIN_MD5 %s\n", account);
    }
    break;
    default:
        FatalErrorf("unknown auth protocol: %d",auth_info.protocol);
    }

    itoa(authGetKey(),session,10);

    cryptMD5Update(pass_hash, 16);
    cryptMD5Update(session, strlen(session));
    cryptMD5Final((U32 *)md5_buf);
    dumpData("hash(hash1+session)",md5_buf,sizeof(md5_buf));

    memset(RSA_plain_buf,0,sizeof(RSA_plain_buf));
    Strncpyt(RSA_plain_buf,account);
    memcpy(RSA_plain_buf+14,md5_buf,16);
    memcpy(RSA_plain_buf+30,sha512_buf, 64);
    RSA_cipher_size = sizeof(RSA_cipher_buf);
    cryptRSAEncrypt(auth_info.RSA_mod, auth_info.RSA_modlen, auth_info.RSA_exp, auth_info.RSA_explen, RSA_plain_buf, sizeof(RSA_plain_buf), RSA_cipher_buf, &RSA_cipher_size);
    authPutU32(pak,RSA_cipher_size);
    authPutArray(pak,RSA_cipher_buf,RSA_cipher_size);
    authPutU32(pak,subscription);
    authPutU16(pak,flags); // cdkind - what is it? ab: now it is mac/pc flag
    if (auth_info.protocol == ASIA_AUTH_PROTOCOL_VERSION)
        authPutU08(pak,game_runningCohClientCount()); // number of copies of game running
    authSendPacket(pak);
}

void acSendLogin(const char* account, const char* password, int subscription,AuthFlags flags)
{
    acSendLoginMD5(account,password,subscription,flags);
}

void acSendServerList()
{
    AuthPacket    *pak = authAllocPacket(AQ_SERVER_LIST_EXT);
    server_list_expected = (getCurrentLocale() == 3) ? kAuthServerListType_Extended : kAuthServerListType_Normal;

    authPutArray(pak, session_id, 8 );
    authPutU08(pak, server_list_expected);
    if (authRequestSpecificGroups(auth_info.protocol))  // Check if our current protocol supports this
    {
        authPutU08(pak, min(game_state.numServerGroups, MAX_SERVER_GROUPS)); // Number of server groups we are requesting
        for (int r = 0; r < game_state.numServerGroups && r < MAX_SERVER_GROUPS; r++)
        {
            authPutU32(pak, game_state.serverGroup[r]); // Request this specific server group
        }
    }
    authSendPacket(pak);

    authdbg_printf("send: AQ_SERVER_LIST_EXT\n");
}

void acSendAboutToPlay(int server_id)
{
    AuthPacket    *pak = authAllocPacket(AQ_ABOUT_TO_PLAY);

    play_ok_expected = 1;

    authPutArray(pak, session_id, 8 );
    authPutU08(pak,server_id);
    authSendPacket(pak);

    authdbg_printf("send: AQ_ABOUT_TO_PLAY \n");
}

void acSendLogout()
{
    AuthPacket    *pak = authAllocPacket(AQ_LOGOUT);

    authPutArray(pak,session_id, 8 );
    authSendPacket(pak);

    authdbg_printf("send: AQ_LOGOUT \n");
}

void acGetProtocolVer(AuthPacket *pak)
{
    U32        one_time_key,protocol,protocol_orig;
    one_time_key    = authGetU32(pak);
    protocol_orig = protocol = authGetU32(pak);
    auth_info.RSA_explen = authGetU32(pak);
    authGetArray(pak, auth_info.RSA_exp, auth_info.RSA_explen);
    auth_info.RSA_modlen = authGetU32(pak);
    authGetArray(pak, auth_info.RSA_mod, auth_info.RSA_modlen);
    
    // ab: sent an email to Allan, but no word back yet, assuming this is okay.
    if(!protocol)
    {
        protocol = GLOBAL_AUTH_PROTOCOL_VERSION;
        auth_info.auth2_enabled = Auth2EnabledState_Direct; // no queue.
    }

    auth_info.protocol = protocol;


    if (protocol != USA_AUTH_PROTOCOL_VERSION && protocol != ASIA_AUTH_PROTOCOL_VERSION &&
        protocol != GLOBAL_AUTH_PROTOCOL_VERSION && protocol != GR_REACTIVATION_PROTOCOL_VERSION)
    {
        auth_info.fail_type = AC_PROTOCOL_VER;
        auth_info.reason = 0;
    }
    authSetKey(one_time_key,protocol >= GLOBAL_AUTH_PROTOCOL_VERSION);
}

void acGetLoginOK(AuthPacket *pak)
{
    assert(login_ok_expected);
    login_ok_expected = 0;

    authGetArray(pak,session_id,8);
    authGetArray(pak,auth_info.update_key,8);

    auth_info.pay_stat        = authGetU32(pak);
    auth_info.loyalty        = authGetU32(pak);
    auth_info.remain_time    = authGetU32(pak);
    auth_info.quota_time    = authGetU32(pak);
    auth_info.warn_flag        = authGetU32(pak);
    auth_info.login_flag    = authGetU32(pak);
    if (auth_info.protocol >= GR_REACTIVATION_PROTOCOL_VERSION)
        authUserSetReactivationActive(authGetU32(pak));
}

void acGetLoginFail(AuthPacket *pak)
{
    auth_info.fail_type = AC_LOGIN_FAIL;
    auth_info.reason = authGetU08(pak);
}

void acGetSendServerList(AuthPacket *pak)
{
    int            i;
    ServerInfo    *si;
    AuthServerListType response_type = server_list_expected;
    
    assert(server_list_expected);
    server_list_expected = kAuthServerListType_None;

    printf("\nresponse type=%s(%d)\n", (response_type==kAuthServerListType_Normal?"normal":"extended"), response_type);
    auth_info.server_count    = authGetU08(pak);
    auth_info.last_login_server_id    = authGetU08(pak);
    SAFE_FREE(auth_info.servers);
    auth_info.servers = calloc(auth_info.server_count,sizeof(ServerInfo));
    for(i=0;i<auth_info.server_count;i++)
    {
        si = &auth_info.servers[i];
        si->id                = authGetU08(pak);
        si->ip                = authGetU32(pak);
        si->port            = authGetU32(pak);
        si->age_limit        = authGetU08(pak);
        si->pk_flag            = authGetU08(pak);
        si->curr_user_count    = authGetU16(pak);
        si->max_user_count    = authGetU16(pak);    //we lie to the auth server to allow the queue.  This is handled at uilogin
        si->server_status    = authGetU08(pak);
        si->isVIP            = authGetU08(pak);
        if(response_type == kAuthServerListType_Extended)
            si->server_type     = authGetU32(pak);
        else
            si->server_type = kShardType_None;

        // Get shard names.
        auth_info.servers[i].name[0] = '\0';
        if (auth_info.protocol >= OUROBOROS_PROTOCOL_VERSION_1)
        {
            int bufferSize = authGetU08(pak);
            // We're sending size as a byte, so can't be higher than 255.
            if (bufferSize > 0)
            {
                // The server will send bufferSize bytes for the string, but without a null terminator.
                // Be sure to add it.
                authGetArray(pak, auth_info.servers[i].name, bufferSize);
                auth_info.servers[i].name[bufferSize] = '\0';
            }
        }
        if (!auth_info.servers[i].name[0])
            // Original method - generate a lookup key or something.
            sprintf(auth_info.servers[i].name, "%sWorldServer_%d", regGetAppName(), auth_info.servers[i].id);

        printf("Auth sent dbserver #%d, %s %s:%d\n",auth_info.servers[i].id, auth_info.servers[i].name,makeIpStr(auth_info.servers[i].ip),auth_info.servers[i].port);
        //printf("Auth sent dbserver #%d %s:%d (%d/%d)\n",auth_info.servers[i].id,makeIpStr(auth_info.servers[i].ip),auth_info.servers[i].port,si->curr_user_count,si->max_user_count);
    }
}

void acGetSendServerFail(AuthPacket *pak)
{
    auth_info.fail_type = AC_SEND_SERVER_FAIL;
    auth_info.reason = authGetU08(pak);
}

char *trimTrailingWhitespace(char *s)
{
    char *e = s + strlen(s) - 1;
    while (e >= s && *e==' ') *e--=0;
    return s;
}

void acGetPlayOK(AuthPacket *pak)
{
    static int old_auth_id = -1;
    static char old_auth_name[32];
    extern char g_achAccountName[32];

    //assert(play_ok_expected);
    play_ok_expected = 0;

    auth_info.game_key    = authGetU32(pak);
    auth_info.uid        = authGetU32(pak);
    // Debug code to check for auth ids switching around!
    if (old_auth_id==-1) {
        old_auth_id = auth_info.uid;
        Strncpyt(old_auth_name, g_achAccountName);
        trimTrailingWhitespace(old_auth_name);
    } else {
        trimTrailingWhitespace(g_achAccountName);
        if (stricmp(g_achAccountName, old_auth_name)==0) {
            // The same login name, assert that we got the same auth id!
            assert(old_auth_id == auth_info.uid);
        } else {
            old_auth_id = auth_info.uid;
            Strncpyt(old_auth_name, g_achAccountName);
            trimTrailingWhitespace(old_auth_name);
        }
    }
    auth_info.server_id    = authGetU08(pak);
}

void acGetPlayFail(AuthPacket *pak)
{
    assert(play_ok_expected);
    play_ok_expected = 0;
    auth_info.fail_type = AC_PLAY_FAIL;
    auth_info.reason = authGetU08(pak);
}

void acGetBlockedAccount(AuthPacket *pak)
{
    auth_info.fail_type = AC_BLOCKED_ACCOUNT;
    auth_info.reason = authGetU08(pak);
}

void acGetBlockedAccountWithMsg(AuthPacket *pak)
{
    int        i;

    auth_info.fail_type = AC_BLOCKED_ACCOUNT_WITH_MSG;
    auth_info.block_count = authGetU08(pak);
    SAFE_FREE(auth_info.blocks);
    auth_info.blocks = calloc(auth_info.block_count,sizeof(auth_info.blocks[0]));
    for(i=0;i<auth_info.block_count;i++)
    {
        auth_info.blocks[i].reason = authGetU16(pak);
        auth_info.blocks[i].msg        = strdup(authGetStr(pak));
    }
}

void acGetAccountKicked(AuthPacket *pak)
{
    auth_info.fail_type = AC_PLAY_FAIL;
    auth_info.reason = authGetU08(pak);
}

void acGetQueueSize(AuthPacket *pak)
{
    int num_queues = authGetU08(pak);
    int i;
    for( i = 0; i < num_queues; ++i ) 
    {
        int uid = authGetU08(pak);
        int queue_level = authGetU08(pak);
        int queue_size = authGetU32(pak);
        int wait_time = authGetU32(pak);
    }
}

void acGetHandoffToQueue(AuthPacket *pak)
{
    int field = authGetU32(pak);
    int uid = authGetU32(pak);
    int server_id = authGetU08(pak);
}

static char const *g_AuthToClientCmdStrs[] = 
{
    "AC_PROTOCOL_VER",                  // 0                    
    "AC_LOGIN_FAIL",                    // 1                      
    "AC_BLOCKED_ACCOUNT",               // 2                 
    "AC_LOGIN_OK",                      // 3                        
    "AC_SEND_SERVER_LIST",              // 4                
    "AC_SEND_SERVER_FAIL",              // 5                
    "AC_PLAY_FAIL",                     // 6                       
    "AC_PLAY_OK",                       // 7                         
    "AC_ACCOUNT_KICKED",                // 8                  
    "AC_BLOCKED_ACCOUNT_WITH_MSG",      // 9        
    "AC_SCCHECKREQ",                    // 10
    "AC_QUEUESIZE",                     // 11
    "AC_HANDOFFTOQUEUE",                // 12
    "AC_POSITIONINQUEUE",               // 13
    "AC_HANDOFFTOGAME",                 // 14
};
STATIC_ASSERT(AuthToClientCmd_Count == 15);

int authWaitFor(enum AuthToClientCmd wait_for_cmd)
{
    AuthPacket    pak;
    enum AuthToClientCmd cmd;
    static        int timer;

    if (!timer)
        timer = timerAlloc();
    timerStart(timer);
    memset(&pak,0,sizeof(pak));
    for(;;)
    {
        int ret = authGetPacket(&pak);
        if (ret < 0)
        {
            auth_info.fail_type = -1;
            auth_info.reason = -2;
            return 0;
        }
        else if (ret > 0)
        {
            cmd = authGetU08(&pak);
            authdbg_printf("recv: %s\n", AGET(g_AuthToClientCmdStrs,cmd));
            switch(cmd)
            {
                xcase AC_PROTOCOL_VER:
                    acGetProtocolVer(&pak);
                xcase AC_LOGIN_FAIL:
                    acGetLoginFail(&pak);
                xcase AC_BLOCKED_ACCOUNT:
                    acGetBlockedAccount(&pak);
                xcase AC_LOGIN_OK:
                    acGetLoginOK(&pak);
                xcase AC_SEND_SERVER_LIST:
                    acGetSendServerList(&pak);
                xcase AC_SEND_SERVER_FAIL:
                    acGetSendServerFail(&pak);
                xcase AC_PLAY_FAIL:
                    acGetPlayFail(&pak);
                xcase AC_PLAY_OK:
                    acGetPlayOK(&pak);
                xcase AC_ACCOUNT_KICKED:
                    acGetAccountKicked(&pak);
                xcase AC_BLOCKED_ACCOUNT_WITH_MSG:
                    acGetBlockedAccountWithMsg(&pak);
                xcase AC_QUEUESIZE:    
                    acGetQueueSize(&pak);
                xcase AC_HANDOFFTOQUEUE:    
                    acGetHandoffToQueue(&pak);                    
            }
            if (auth_info.fail_type)
            {
                return 0;
            }
            if (cmd == wait_for_cmd)
                return 1;
            memset(&pak,0,sizeof(pak));
        }
        Sleep(1);
        if (timerElapsed(timer) > 45)
        {
            auth_info.fail_type = -1;
            auth_info.reason = -2;
            return 0;
        }
    }
}

int authLogin(char *name,char *password)
{
    int            ret;
    char passwordCopy[128];
    acCleanup();
    memset(&auth_info,0,sizeof(auth_info));
    Strncpyt(auth_info.name,name);
    Strncpyt(passwordCopy,password);
    cryptMD5Update(passwordCopy, strlen(passwordCopy));
    cryptMD5Final(auth_info.passwordMD5);
    memset(passwordCopy, 0, sizeof(passwordCopy));
    if (!game_state.auth_address[0] || game_state.cs_address[0])
    {
        game_state.auth_address[0] = 0;
        auth_info.server_count = 1;
        SAFE_FREE(auth_info.servers);
        auth_info.servers = calloc(sizeof(auth_info.servers[0]),1);
        auth_info.servers[0].id = 1;
        auth_info.servers[0].server_status = 1;
        auth_info.servers[0].max_user_count = 1;
        getAutoDbName(game_state.cs_address);
        auth_info.servers[0].ip = ipFromString(game_state.cs_address);
        auth_info.servers[0].port = DEFAULT_DBGAMECLIENT_PORT;
        Strncpyt(auth_info.servers[0].name,game_state.cs_address);
        return 1;
    }
    writeConsole(OUTPUT_INFO, "Connecting to AuthServer %s:%d (TCP)", game_state.auth_address, AUTH_SERVER_PORT);
    ret = authConnect(game_state.auth_address, AUTH_SERVER_PORT);
    if (!ret)
    {
        writeConsole(OUTPUT_ERROR, "Failed to connect to AuthServer");
        auth_info.fail_type = -1;
        auth_info.reason = -1;
        return 0;
    }

    authSetKey(1234, 1);        // turn on encryption for first packet, key is ignored for blowfish
    writeConsole(OUTPUT_INFO, "Waiting for AuthServer protocol version");
    if (!authWaitFor(AC_PROTOCOL_VER)) {
        writeConsole(OUTPUT_ERROR, "Failed to receive protocol version");
        return 0;
    }

    writeConsole(OUTPUT_INFO, "Validating credentials");
    acSendLogin(name,password,0,IsUsingCider()?AuthFlags_MAC:AuthFlags_PC); 
    if (!authWaitFor(AC_LOGIN_OK)) {
        writeConsole(OUTPUT_WARNING, "Authentication failed");
        return 0;
    }

    setAssertAuthName(auth_info.name);

    writeConsole(OUTPUT_INFO, "Requesting server list");
    acSendServerList();
    if (!authWaitFor(AC_SEND_SERVER_LIST)) {
        writeConsole(OUTPUT_ERROR, "Failed to receive server list");
        return 0;
    }

    if(auth_info.auth2_enabled == Auth2EnabledState_QueueServer) 
    {
        writeConsole(OUTPUT_INFO, "Receiving queue information");
        // what is this queue for?
        if(!authWaitFor(AC_QUEUESIZE)){
            writeConsole(OUTPUT_ERROR, "Failed to receive queue information");
            return 0;
        }
    }
    return 1;
}

int authReloginIfNeeded(char *password)
{
    static int timer;

    if (!timer)
        timer = timerAlloc();
    if (authSocket())
        return 0;
    if (auth_info.name[0] && password[0])
    {
        char name[128];

        Strncpyt(name, auth_info.name);
        timerStart(timer);
        for(;;)
        {
            
            if (authLogin(name,password))
                return 1;


            //    fail_type -1 means that we couldn't connect to the auth server, retry
            //    S_ALREADY_LOGIN means we're kicking out our old session, retry
            if (timerElapsed(timer) > 45 || ((auth_info.fail_type != -1) && (auth_info.reason != S_ALREADY_LOGIN)))
                break;
            Sleep(250);
        }
    }
    return 0;
}

int authLogout()
{
    if (!game_state.auth_address[0])
        return 1;

    acSendLogout();
    authDisconnect();
    return 1;
}

typedef struct 
{
    int        reason;
    char    *msg;
} AuthErrMsg;

#define     AC(x) {x, #x}

AuthErrMsg auth_err_reasons[] = {
    {-1, "AuthConnectFail"},
    {-2, "AuthTimeout"},
    AC(S_ALL_OK),
    AC(S_DATABASE_FAIL),
    AC(S_INVALID_ACCOUNT),
    AC(S_INCORRECT_PWD),
    AC(S_ACCOUNT_LOAD_FAIL),
    AC(S_LOAD_SSN_ERROR),
    AC(S_NO_SERVERLIST),
    AC(S_ALREADY_LOGIN),
    AC(S_SERVER_DOWN),
    AC(S_INCORRECT_MD5Key),
    AC(S_NO_LOGININFO),
    AC(S_KICKED_BY_WEB),
    AC(S_UNDER_AGE),
    AC(S_KICKED_DOUBLE_LOGIN),
    AC(S_ALREADY_PLAY_GAME),
    AC(S_LIMIT_EXCEED),
    AC(S_SERVER_CHECK),
    AC(S_MODIFY_PASSWORD),
    AC(S_NOT_PAID),
    AC(S_NO_SPECIFICTIME),
    AC(S_SYSTEM_ERROR),
    AC(S_ALREADY_USED_IP),
    AC(S_BLOCKED_IP),
    AC(S_VIP_ONLY),
    AC(S_DELETE_CHRARACTER_OK),
    AC(S_CREATE_CHARACTER_OK),
    AC(S_INVALID_NAME),
    AC(S_INVALID_GENDER),
    AC(S_INVALID_CLASS),
    AC(S_INVALID_ATTR),
    AC(S_MAX_CHAR_NUM_OVER),
    AC(S_TIME_SERVER_LIMIT_EXCEED),
    AC(S_INVALID_USER_STATUS),
    AC(S_INCORRECT_AUTHKEY),
};

char *authGetError()
{
    int        i,found=0;

    if (!auth_info.fail_type)
        return 0;
    for(i=0;i<ARRAY_SIZE(auth_err_reasons);i++)
    {
        if (auth_err_reasons[i].reason == auth_info.reason)
        {
            sprintf(auth_error_msg,"%s",auth_err_reasons[i].msg);
            found = 1;
        }
    }
    if (!found)
        sprintf(auth_error_msg,"AuthFail:Type%d__Reason%d",auth_info.fail_type,auth_info.reason);
    auth_info.fail_type = 0;
    return auth_error_msg;
}

bool authIsErrorAccountAlreadyLoggedIn()
{
    return auth_info.reason == S_ALREADY_LOGIN;
}

bool authIsErrorAccountNotPaid()
{
    bool retval=false;

    if (auth_info.reason == S_NOT_PAID)
        retval = true;

    return retval;
}
