#include "queueservercomm.h"
#include "comm_backend.h"
#include "authcomm.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>

#include "dbserver/servercfg.h"
#include "dbrelay.h"
#include "dbdispatch.h"
#include "entity/entVarUpdate.h"
#include "ClientLogin/clientcommLogin.h"
#include "clientcomm.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/StringUtil.h>
#include <utilitieslib/utils/log.h>
#include "overloadProtection.h"
#include "accountservercomm.h"
#include "loadBalancing.h"

static NetLinkList s_queuelinks = {0};

//only placed in here once authenticated
StashTable gameClientHashById;
StashTable gameClientHashByName;
#define STARTING_CLIENT_HASH_SIZE 1000

typedef struct QueueStatus
{
    U32 loggingIn;    //how many people have already made it through the queue and are ready to play?
    U32 queuedUp;    //people still waiting to log in.
    U32 authenticating;    //people who are trying to get passed authentication.
    U32 connected;  //number of open netlink connections on the queueserver
} QueueStatus;

QueueStatus g_queueStatus = {0};
QueueStatus g_queueStatus_debug = {0};

MP_DEFINE(GameClientLink);

void destroyGameClientByName(char *authname, U32 linkId)
{
    char    account_name[64] = {0};
    GameClientLink *temp = 0;
    strncpy(account_name, authname, 64);
    Str_trimTrailingWhitespace(account_name);

    getGameClientOnQueueServerByName(account_name, &temp);
    if(!temp || mpReclaimed(temp) || (linkId && temp->linkId != linkId))
        return;

    if(gameClientHashByName && stashRemovePointer(gameClientHashByName, account_name, &temp))
    {
        assert(stashIntRemovePointer(gameClientHashById, temp->auth_id, NULL));
        queueservercomm_gameclientDestroy(temp);
    }
}
static void destroyQueuePacketData(Packet *pak)
{
    estrDestroy((char **)&pak->userData);
}

static void pktFixForQ(Packet *pak, char *name)
{
    estrPrintCharString((char **)&pak->userData, name);
    pak->delUserDataCallback = destroyQueuePacketData;
}

static int stash_destroyClient(StashElement element)
{
    GameClientLink* game = (GameClientLink*)stashElementGetPointer(element);

    if(game)
    {
        destroyGameClientByName(game->account_name, 0);
    }
    return 1;
}

static int stash_countClient(StashElement element)
{
    GameClientLink* game = (GameClientLink*)stashElementGetPointer(element);

    if(game)
    {
        g_queueStatus_debug.connected++;
        if(game->loginStatus == GAMELLOGIN_UNAUTHENTICATED)
            g_queueStatus_debug.authenticating++;
        else if(game->loginStatus == GAMELLOGIN_QUEUED)
            g_queueStatus_debug.queuedUp++;
        else if(game->loginStatus == GAMELLOGIN_LOGGINGIN)
            g_queueStatus_debug.loggingIn++;
    }
    return 1;
}

void destroyAllGameClientLinks()
{
    if(gameClientHashByName && stashGetValidElementCount(gameClientHashByName))
        stashForEachElement(gameClientHashByName, stash_destroyClient);
    devassert(g_queueStatus.loggingIn == 0 && g_queueStatus.queuedUp == 0);
    g_queueStatus.connected = g_queueStatus.loggingIn = g_queueStatus.queuedUp = 0;
}


static int stash_countClientActivity(StashElement element)
{
    GameClientLink* game = (GameClientLink*)stashElementGetPointer(element);

    if(game)
    {
        g_queueStatus_debug.connected++;
        if(game->loginStatus == GAMELLOGIN_UNAUTHENTICATED)
            g_queueStatus_debug.authenticating++;
        else if(game->loginStatus == GAMELLOGIN_QUEUED)
            g_queueStatus_debug.queuedUp++;
        else if(game->loginStatus == GAMELLOGIN_LOGGINGIN)
            g_queueStatus_debug.loggingIn++;
    }
    return 1;
}

void queueservercomm_updateCount()
{
    g_queueStatus_debug.authenticating = g_queueStatus_debug.connected = g_queueStatus_debug.loggingIn = g_queueStatus_debug.queuedUp = 0;
    if(gameClientHashByName && stashGetValidElementCount(gameClientHashByName))
        stashForEachElement(gameClientHashByName, stash_countClientActivity);
    devassert(g_queueStatus_debug.connected == g_queueStatus_debug.authenticating + g_queueStatus_debug.queuedUp + g_queueStatus_debug.loggingIn);
    if(!devassert(g_queueStatus.authenticating == g_queueStatus_debug.authenticating))
        g_queueStatus.authenticating = g_queueStatus_debug.authenticating;
    /*if(!devassert(g_queueStatus.connected == g_queueStatus_debug.connected))
        g_queueStatus.connected = g_queueStatus_debug.connected;*/
    if(!devassert(g_queueStatus.loggingIn == g_queueStatus_debug.loggingIn))
        g_queueStatus.loggingIn = g_queueStatus_debug.loggingIn;
    /*if(!devassert(g_queueStatus.queuedUp == g_queueStatus_debug.queuedUp))
        g_queueStatus.queuedUp = g_queueStatus_debug.queuedUp;*/
}

static int stash_reconnectClientToAuth(StashElement element)
{
    GameClientLink* game = (GameClientLink*)stashElementGetPointer(element);

    if(game &&(game->loginStatus == GAMELLOGIN_QUEUED || game->loginStatus == GAMELLOGIN_LOGGINGIN) && !game->remove)
    {
        authReconnectOnePlayer(game->account_name,game->auth_id);
    }
    return 1;
}

void reconnectQueueserverCommPlayersToAuth()
{
    if(gameClientHashByName && stashGetValidElementCount(gameClientHashByName))
        stashForEachElement(gameClientHashByName, stash_reconnectClientToAuth);
}

NetLink* queueserver_getLink(void)
{
    return SAFE_MEMBER(s_queuelinks.links, size) ? s_queuelinks.links->storage[0] : NULL;
}

void queueservercomm_switchgamestatus(GameClientLink *game, GameClientLoginStatus status)
{
    if(server_cfg.queue_server)
    {
        if(game->loginStatus == GAMELLOGIN_LOGGINGIN)
            g_queueStatus.loggingIn--;
    }

    game->loginStatus = status;
    if(server_cfg.queue_server)
    {
        if(game->loginStatus == GAMELLOGIN_LOGGINGIN)
            g_queueStatus.loggingIn++;
    }
}

GameClientLink * queueservercomm_gameclientCreate(U32 ip, U32 linkId)
{
    GameClientLink *game = 0;
    MP_CREATE(GameClientLink, STARTING_CLIENT_HASH_SIZE);
    game = MP_ALLOC( GameClientLink );
    game->link = queueserver_getLink();
    game->nonQueueIP = ip;
    game->linkId = linkId;
    return game;
}

void queueservercomm_gameclientDestroy(GameClientLink *game)
{
    if(game->loginStatus == GAMELLOGIN_LOGGINGIN)
        g_queueStatus.loggingIn--;
    
    MP_FREE(GameClientLink, game);
}

void setGameClientOnQueueServer(GameClientLink *game)
{
    GameClientLink *existingGame = 0;
    if(!game || !strlen(game->account_name))
        return;

    if(!gameClientHashByName)
    {
        gameClientHashByName = stashTableCreateWithStringKeys(STARTING_CLIENT_HASH_SIZE, StashDefault);
        gameClientHashById = stashTableCreateInt(STARTING_CLIENT_HASH_SIZE);
    }

    getGameClientOnQueueServerByName(game->account_name, &existingGame);
    if(existingGame)
    {
        if(existingGame != game)
        {
            // kick the old login
            clientcommKickLogin(game->auth_id, game->account_name, -1);
        }
        destroyGameClientByName(game->account_name, 0);
    }

    stashAddPointer(gameClientHashByName, game->account_name, game, true);
    stashIntAddPointer(gameClientHashById, game->auth_id, game, true);
}

void getGameClientOnQueueServer(U32 auth_id, GameClientLink **game)
{
    int found = 0;
    if(!game)
        return;    //there's nothing that can be done for you
    if(!gameClientHashByName)
    {
        *game = NULL;
        return;
    }
    found = stashIntFindPointer(gameClientHashById, auth_id, game);
}

void getGameClientOnQueueServerByName(char *authname, GameClientLink **game)
{
    char    account_name[64] = {0};
    int found = 0;
    if(!game)
        return;    //there's nothing that can be done for you
    if(!gameClientHashByName)
    {
        *game = NULL;
        return;
    }
    if(authname)
    {
        strncpy(account_name, authname, 64);
        Str_trimTrailingWhitespace(account_name);
        if(strlen(account_name))
        {
            found = stashFindPointer(gameClientHashByName, account_name, game);
        }
    }
}

int queueserver_secondsSinceUpdate(void)
{
    int secs = 9999;
    NetLink *queuelink = queueserver_getLink();
    if(queuelink)
        return secs = timerCpuSeconds() - queuelink->lastRecvTime;
    return secs;
}

// *********************************************************************************
//  queuelink handler
// *********************************************************************************    

static int s_handleQueuePacket(Packet *pak_in, int cmd, NetLink *queuelink);

static int s_createQueueLink(NetLink *link)
{
    devassert(s_queuelinks.links->size == 1);
    netLinkSetMaxBufferSize(link, BothBuffers, 8*1024*1024); // Set max size to auto-grow to
    netLinkSetBufferSize(link, BothBuffers, 1*1024*1024);
    link->userData = NULL;
    link->totalQueuedBytes = 0;
    return 1;
}

static int s_deleteQueueLink(NetLink *link)
{
    printf("lost connection to queueserver.\n");
    destroyAllGameClientLinks();
    //we better not have any names left, or we're leaking.
    devassert(!gameClientHashByName || (gameClientHashByName && stashGetValidElementCount(gameClientHashByName) == 0));
    g_queueStatus.connected = g_queueStatus.loggingIn = g_queueStatus.queuedUp = 0;

    return 1;
}


void queueserver_commInit(void)
{
    netLinkListAlloc(&s_queuelinks, 1, 0, s_createQueueLink);
    s_queuelinks.destroyCallback = s_deleteQueueLink;
    if(netInit(&s_queuelinks, 0, DEFAULT_QUEUESERVER_PORT))
        NMAddLinkList(&s_queuelinks, s_handleQueuePacket);
    else
        FatalErrorf("Error listening for queueserver.\n");
}

void queueservercomm_sendUpdateConfig()
{
    NetLink* queue = queueserver_getLink();
    Packet *pak_out = queue?pktCreateEx(queue, QUEUESERVER_SVR_CONNECT ):NULL;
    if(!pak_out)
        return;
    pktSendBitsAuto(pak_out, DBSERVER_QUEUE_PROTOCOL_VERSION);
    if(pktSendBool(pak_out, server_cfg.queue_server)) // do we use the queueserver
    {
        pktSendBitsAuto(pak_out, server_cfg.enqueue_auth_limiter);
        pktSendBitsAuto(pak_out, server_cfg.max_players);
        pktSendBitsAuto(pak_out, inUseCount(dbListPtr(CONTAINER_ENTS)));
        //s_sendAuthLimiterList(pak_out);
    }
    pktSend(&pak_out, queue);
}


static int s_handleQueuePacket(Packet *pak_in, int cmd, NetLink *queuelink)
{
    if(queuelink != queueserver_getLink()) // only allow one link
    {
        Packet *pak_out = pktCreateEx(queuelink, QUEUESERVER_SVR_ERROR);
        pktSendString(pak_out, "Too many connections!");
        pktSend(&pak_out, queuelink);
        return 1;
    }

    switch (cmd)
    {
        xcase QUEUECLIENT_CONNECT:
        {
            queueservercomm_sendUpdateConfig();
            printf("QueueServer connecting at %s\n", makeIpStr(queuelink->addr.sin_addr.S_un.S_addr));
            
            if( server_cfg.isLoggingMaster )
                sendLogLevels(queuelink, QUEUESERVER_SVR_SET_LOG_LEVELS);
            queuelink->compressionDefault = 1;
        }
        xcase QUEUECLIENT_CLIENTPLAY:
        {
            char *authname = pktGetString(pak_in);
            U32 linkId = pktGetBitsAuto(pak_in);
            GameClientLink *client = 0;
            getGameClientOnQueueServerByName(authname,&client);
            if(client && client->linkId == linkId)
                clientcomm_PlayerLogin(client);
        }
        xcase QUEUECLIENT_UPDATE_REQUEST:
        {
            Packet *pak_out = pktCreateEx(queuelink, QUEUESERVER_SVR_UPDATE );
            int accountServerStatus = true;
            int overloadProtectionEnabled = overloadProtection_IsEnabled() ? 1 : 0;

            if (server_cfg.block_free_if_no_account && !server_cfg.fake_auth && !accountServerResponding())
                accountServerStatus = false;

            pktSendBitsAuto(pak_out, g_queueStatus.loggingIn + inUseCount(dbListPtr(CONTAINER_ENTS)));//current players
            pktSendBitsAuto(pak_out, server_cfg.max_players);//max active
            pktSendBitsAuto(pak_out, server_cfg.logins_per_minute);//max active
            pktSendBitsAuto(pak_out, accountServerStatus); // is the accountserver talking to us?
            pktSendBitsAuto(pak_out, overloadProtectionEnabled);
            pktSend(&pak_out, queuelink);
            g_queueStatus.queuedUp = pktGetBitsAuto(pak_in);
            g_queueStatus.connected = pktGetBitsAuto(pak_in);
        }
        xcase QUEUECLIENT_REMOVECLIENT:
        {
            char *authname = pktGetString(pak_in);
            U32 linkId = pktGetBitsAuto(pak_in);
            destroyGameClientByName(authname, linkId);
        }
        xcase QUEUECLIENT_CLIENTCOMMAND:
        {
            int clientCmd = pktGetBitsAuto(pak_in);
            U32 clientIP = 0;
            char *authname;
            
            if(clientCmd == DBGAMECLIENT_LOGIN)
            {
                U32 queuedUp;
                queuedUp = pktGetBitsAuto(pak_in);
                g_queueStatus.queuedUp = queuedUp;
                authname = pktGetString(pak_in);
            }
            else
            {
                GameClientLink *client = 0;
                authname = pktGetString(pak_in);
                getGameClientOnQueueServerByName(authname, &client);
                if(!devassert(client))
                {
                    printf("QueueServerLog", "Could not find incoming player %s with message %d\n", authname, clientCmd);
                    return 1;
                }
            }
            pktFixForQ(pak_in, authname);
            processGameClientMsg(pak_in,clientCmd, queuelink);
        }

xdefault:
    
    devassertmsg(0, "invalid command from queueserver");
    LOG_OLD_ERR( "s_handleQueuePacket: invalid net cmd %i from queueserver", cmd);
    };

    return 1;
}

GameClientLink *dbClient_getGameClient(U32 auth_id, NetLink *link)
{
    GameClientLink *gameClient = NULL;
    if(server_cfg.queue_server) {
        getGameClientOnQueueServer(auth_id, &gameClient);
    } else {
        gameClient = link->userData;
        if ( ! devassertmsg(
                (( gameClient == NULL ) || 
                 ( gameClient->auth_id == auth_id )), 
                 "Probable bad cast from void* to GameClientLink*." )) {
            gameClient = NULL;
        }
    }
    return gameClient;
}

GameClientLink *dbClient_getGameClientByName(char *authname, NetLink *link)
{
    GameClientLink *gameClient = 0;
    if(server_cfg.queue_server)
        getGameClientOnQueueServerByName(authname, &gameClient);
    else
        gameClient = link->userData;
    return gameClient;
}


Packet *dbClient_createPacket(int cmd, GameClientLink *client)
{
    Packet *ret;
    NetLink *link;

    if(!devassert(client))
        return NULL;
    
    if(server_cfg.queue_server)
    {
        link = queueserver_getLink();
        if(!devassert(link))
            return NULL;
        ret = pktCreateEx(link, cmd);
        pktFixForQ(ret, client->account_name);
        pktSendString(ret, client->account_name);
        pktSendBitsAuto(ret, client->linkId);
    }
    else
    {
        ret = pktCreateEx(client->link, cmd);
    }
    return ret;
}

GameClientLink *queueservercomm_getLoginClient(Packet *pak_in)
{
    GameClientLink *client;
    U32 clientIP;
    U32 clientLinkId;
    if(!pak_in)
        return 0;

    clientIP = pktGetBitsAuto(pak_in);
    clientLinkId = pktGetBitsAuto(pak_in);
    client = queueservercomm_gameclientCreate(clientIP, clientLinkId);
    return client;
}

void sendClientQuit(char *authname, U32 link_id)
{
    GameClientLink *game = NULL;
    Packet *ret;
    NetLink *link;
    link = queueserver_getLink();
    ret = pktCreateEx(link, QUEUESERVER_SVR_REMOVECLIENT);
    pktSendString(ret, authname);
    pktSendBitsAuto(ret, link_id);
    pktSend(&ret, link);
}

int queueservercomm_getNWaiting()
{
    return g_queueStatus.queuedUp;
}
int queueservercomm_getNLoggingIn()
{
    return g_queueStatus.loggingIn;
}

int queueservercomm_getNConnected()
{
    return g_queueStatus.connected;
}
