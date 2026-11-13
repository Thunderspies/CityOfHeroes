#include "clientcomm/authclient.h"
#include "clientcomm/auth_create_account_private.h"
#include "UI/uiGame.h"
#include "UI/uiLogin.h"
#include "clientcomm/clientcomm.h"
#include "cmdparse/cmdgame.h"
#include "comm_backend.h"
#include "game.h"
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/timing.h>
#include <string.h>

extern NetLink db_comm_link;

typedef enum AuthCreateStage
{
    AUTH_CREATE_STAGE_IDLE,
    AUTH_CREATE_STAGE_RESOLVE,
    AUTH_CREATE_STAGE_CONNECT,
    AUTH_CREATE_STAGE_HANDSHAKE,
    AUTH_CREATE_STAGE_RESULT
} AuthCreateStage;

typedef struct AuthCreateRequest
{
    AuthCreateStage stage;
    AuthInfo saved;
    AuthPacket response;
    unsigned char plain[ACCOUNT_CREATE_PLAIN_BYTES];
    int timer;
} AuthCreateRequest;

static AuthCreateRequest creation;

int authCreateAccountIsPending(void)
{
    return creation.stage != AUTH_CREATE_STAGE_IDLE;
}

static AuthCreateAccountResult finishCreation(AuthCreateAccountResult result)
{
    authCreateResolveCancel();
    authDisconnect();
    auth_info = creation.saved;
    timerFree(creation.timer);
    accountCreationClear(&creation, sizeof(creation));
    return result;
}

static int creationResponseHasValidPadding(const AuthPacket *packet)
{
    int paddedLength;
    int i;
    if (packet->bytesRead < 0 || packet->bytesRead > packet->length || packet->length > sizeof(packet->data))
        return 0;
    if (!authIsGlobalAuth())
        return packet->length == packet->bytesRead;
    paddedLength = (packet->bytesRead + 7) & ~7;
    if (packet->length != paddedLength + 8)
        return 0;
    for (i = packet->bytesRead; i < paddedLength; ++i)
        if (packet->data[i])
            return 0;
    for (i = paddedLength + 4; i < packet->length; ++i)
        if (packet->data[i])
            return 0;
    return 1;
}

AuthCreateAccountResult authCreateAccountBegin(const char *name, const char *password)
{
#if TEST_CLIENT
    return AUTH_CREATE_BUSY;
#else
    unsigned char plain[ACCOUNT_CREATE_PLAIN_BYTES] = {0};
    AccountCreationResult validation;
    if (authCreateAccountIsPending() || !isMenu(MENU_LOGIN) || s_loggedIn_serverSelected != LOGIN_STAGE_START || db_comm_link.connected ||
        db_comm_link.socket || commConnected() || g_shardVisitorDBID)
        return AUTH_CREATE_BUSY;
    if (!game_state.auth_address[0] || game_state.cs_address[0])
        return AUTH_CREATE_CONNECTION_ERROR;
    validation = accountCreationValidate(name, password, (char *)plain);
    if (validation != ACCOUNT_CREATE_SUCCESS)
    {
        accountCreationClear(plain, sizeof(plain));
        return (AuthCreateAccountResult)validation;
    }
    memcpy(plain + ACCOUNT_CREATE_NAME_BYTES, password, strlen(password));
    // A failed login can leave an auth socket open on the initial login screen.
    // Registration owns a fresh connection and handshake, not that old session.
    if (authSocket())
        authDisconnect();
    creation.saved = auth_info;
    memcpy(creation.plain, plain, sizeof(plain));
    accountCreationClear(plain, sizeof(plain));
    memset(&auth_info, 0, sizeof(auth_info));
    creation.stage = AUTH_CREATE_STAGE_RESOLVE;
    creation.timer = timerAlloc();
    timerStart(creation.timer);
    if (!authCreateResolveBegin(game_state.auth_address))
        return finishCreation(AUTH_CREATE_CONNECTION_ERROR);
    return AUTH_CREATE_PENDING;
#endif
}

AuthCreateAccountResult authCreateAccountPoll(void)
{
    int status;
    int command;
    if (!authCreateAccountIsPending())
        return AUTH_CREATE_INVALID_STATE;
    if (timerElapsed(creation.timer) >= (creation.stage <= AUTH_CREATE_STAGE_CONNECT ? 10 : 45))
        return finishCreation(creation.stage == AUTH_CREATE_STAGE_RESULT ? AUTH_CREATE_UNCONFIRMED : AUTH_CREATE_TIMEOUT);

    if (creation.stage == AUTH_CREATE_STAGE_RESOLVE)
    {
        U32 address;
        status = authCreateResolvePoll(&address);
        if (!status)
            return AUTH_CREATE_PENDING;
        if (status < 0)
            return finishCreation(AUTH_CREATE_CONNECTION_ERROR);
        status = authConnectStart(address, AUTH_SERVER_PORT);
        if (status < 0)
            return finishCreation(AUTH_CREATE_CONNECTION_ERROR);
        creation.stage = AUTH_CREATE_STAGE_CONNECT;
    }
    else if (creation.stage == AUTH_CREATE_STAGE_CONNECT)
    {
        status = authConnectPoll();
        if (status < 0)
            return finishCreation(AUTH_CREATE_CONNECTION_ERROR);
    }
    else
    {
        status = authGetPacket(&creation.response);
        if (status < 0)
        {
            AuthCreateAccountResult result = authSocketOk() ? AUTH_CREATE_CONNECTION_ERROR : AUTH_CREATE_PROTOCOL_ERROR;
            if (creation.stage == AUTH_CREATE_STAGE_RESULT && result == AUTH_CREATE_CONNECTION_ERROR)
                result = AUTH_CREATE_UNCONFIRMED;
            return finishCreation(result);
        }
        if (!status)
            return AUTH_CREATE_PENDING;
        command = authGetU08(&creation.response);
        if (creation.stage == AUTH_CREATE_STAGE_HANDSHAKE)
        {
            unsigned char cipher[512] = {0};
            U32 cipherLength = sizeof(cipher);
            AuthPacket *request;
            if (command != AC_PROTOCOL_VER)
                return finishCreation(AUTH_CREATE_PROTOCOL_ERROR);
            acGetProtocolVer(&creation.response);
            if (!authSocketOk() || auth_info.fail_type || !creationResponseHasValidPadding(&creation.response))
                return finishCreation(AUTH_CREATE_PROTOCOL_ERROR);
            status = cryptRSAEncrypt(auth_info.RSA_mod, auth_info.RSA_modlen, auth_info.RSA_exp, auth_info.RSA_explen, creation.plain, sizeof(creation.plain),
                                     cipher, &cipherLength);
            accountCreationClear(creation.plain, sizeof(creation.plain));
            if (!status || !cipherLength || cipherLength > sizeof(cipher))
            {
                accountCreationClear(cipher, sizeof(cipher));
                return finishCreation(AUTH_CREATE_ENCRYPTION_ERROR);
            }
            request = authAllocPacket(AQ_CREATE_ACCOUNT);
            authPutU32(request, cipherLength);
            authPutArray(request, cipher, cipherLength);
            status = authSendPacket(request);
            accountCreationClear(cipher, sizeof(cipher));
            if (!status)
                return finishCreation(AUTH_CREATE_UNCONFIRMED);
            accountCreationClear(&creation.response, sizeof(creation.response));
            creation.stage = AUTH_CREATE_STAGE_RESULT;
            timerStart(creation.timer);
            return AUTH_CREATE_PENDING;
        }
        if (command != AC_CREATE_ACCOUNT_RESULT)
            return finishCreation(AUTH_CREATE_PROTOCOL_ERROR);
        status = authGetU08(&creation.response);
        /* Validate the fixed result payload and any negotiated transport padding.
         */
        if (!authSocketOk() || status >= ACCOUNT_CREATE_RESULT_COUNT || !creationResponseHasValidPadding(&creation.response))
            return finishCreation(AUTH_CREATE_PROTOCOL_ERROR);
        return finishCreation((AuthCreateAccountResult)status);
    }

    if (status > 0)
    {
        authSetKey(1234, 1);
        creation.stage = AUTH_CREATE_STAGE_HANDSHAKE;
        timerStart(creation.timer);
    }
    return AUTH_CREATE_PENDING;
}

void authCreateAccountCancel(void)
{
    if (authCreateAccountIsPending())
        finishCreation(AUTH_CREATE_UNCONFIRMED);
}

AuthCreateAccountResult authCreateAccount(const char *name, const char *password)
{
    AuthCreateAccountResult result = authCreateAccountBegin(name, password);
    while (result == AUTH_CREATE_PENDING)
    {
        Sleep(1);
        result = authCreateAccountPoll();
    }
    return result;
}

const char *authCreateAccountResultMessage(AuthCreateAccountResult result)
{
    switch (result)
    {
    case AUTH_CREATE_SUCCESS:
        return "Account created. You can now log in using the login fields.";
    case AUTH_CREATE_DISABLED:
        return "Account creation is disabled on this AuthServer.";
    case AUTH_CREATE_INVALID_USERNAME:
        return "Username must contain 1-14 ASCII letters or digits.";
    case AUTH_CREATE_INVALID_PASSWORD:
        return "Password must contain 8-15 printable ASCII characters; spaces are "
               "allowed.";
    case AUTH_CREATE_NAME_TAKEN:
        return "That account name is already taken.";
    case AUTH_CREATE_BLOCKED_IP:
        return "This IP address is blocked by the AuthServer.";
    case AUTH_CREATE_INVALID_REQUEST:
        return "AuthServer rejected the account creation request.";
    case AUTH_CREATE_INVALID_STATE:
        return "AuthServer requires a fresh connection for account creation.";
    case AUTH_CREATE_DATABASE_FAILURE:
        return "AuthServer could not create the account because of a database "
               "failure.";
    case AUTH_CREATE_CONNECTION_ERROR:
        return "Could not connect to the AuthServer. Check your connection and "
               "launch settings, or try again later.";
    case AUTH_CREATE_TIMEOUT:
    case AUTH_CREATE_UNCONFIRMED:
        return "Account creation is unconfirmed; it may have completed. Try "
               "logging in before retrying creation.";
    case AUTH_CREATE_PROTOCOL_ERROR:
        return "Invalid AuthServer response. If account creation was already "
               "submitted, try logging in before retrying.";
    case AUTH_CREATE_ENCRYPTION_ERROR:
        return "Could not encrypt account credentials. No creation request was "
               "sent.";
    case AUTH_CREATE_PENDING:
        return "Creating account... Please wait.";
    default:
        return "Account creation requires the initial login screen with no active "
               "connections or shard transfer.";
    }
}
