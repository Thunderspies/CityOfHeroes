#include "QueueServer.h"
#include "QueueServer_Passthrough.h"

#include <serverlib/serverlib.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/UtilsNew/meta.h>
#include <utilitieslib/UtilsNew/Str.h>

#include "comm_backend.h"
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/ConsoleDebug.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/utils.h>
#include <math.h>
#include "BinHeap.h"

#include "ClientLogin/clientcommLogin.h"

static QueueServerState g_queueServerState;


#define QUEUESERVER_TICK_FREQ            1        // ticks/sec
#define QUEUESERVER_PLAYER_UPDATE_FREQ    10
#define QUEUESERVER_PACKETFLUSHTIME        (2.0f)
#define QUEUESERVER_WAITTOREMOVE        5        //once flagged for removal by the dbserver, we'll hold onto the client for this amount of time before
                                                //forcing the client to quit.  This is so we have time to send them the quit message.
#define QUEUESERVER_LOGINS_PER_MINUTE_DEFAULT    100        //number of logins allowed per second.
NetLinkList    gameclient_links;

static NetLink **players_waiting_for_character_list_retrieval = NULL;

void queueserver_handleCountUpdate(Packet *pak_in);
void queueserver_requestCountUpdate();
void queueserver_enqueue(char *authname, U32 linkId, int priority);
void queueserver_updateClients();
void queueserver_letPlayersThrough();
////////////////////////////////////////////////////////////////////////////////
StashTable gameClientHashByName;    //these are the clients who authenticated and are either enqueued or are communicating with the dbserver.
//unauthenticatedGameClientHash is not strictly necessary since we could just pull these out of the netlink list by ID.  However,
//this further segregates the unauthorized clients from the authorized clients (conceptually at least), and gives me one place to look
//for everyone who has not yet authenticated.
StashTable unauthenticatedGameClientHash;    //these are the clients who are attempting to connect but have not been authenticated.
#define STARTING_CLIENT_HASH_SIZE 1000

static void getGameLink(char *authname, NetLink **link, U32 linkId)
{
    if(!gameClientHashByName)
    {
        return;
    }

    stashFindPointer( gameClientHashByName, authname, link);
    if(linkId && *link && (*link)->UID != linkId)
        *link = NULL;
}

static void DelGameLink(char *authname)
{
    NetLink *link = 0;
    if(gameClientHashByName)
        stashRemovePointer(gameClientHashByName, authname, &link);
}

//registers the game link as authenticated
static void AuthenticateGameLink(NetLink *link)
{
    GameClientLink * client = (GameClientLink *)link->userData;
    char *authname = (client)?client->account_name:0;
    NetLink *oldLink = 0;
    if(authname)
    {
        getGameLink(authname, &oldLink, 0);
        if(oldLink)
        {
            DelGameLink(authname);
            lnkBatchSend(oldLink);
            netRemoveLink(oldLink);
        }

        if(!gameClientHashByName)
        {
            gameClientHashByName = stashTableCreateWithStringKeys(STARTING_CLIENT_HASH_SIZE, StashDefault);
        }

        stashAddPointer(gameClientHashByName, authname, link, false);
    }
}

static void DelUnauthenticatedGameLink(U32 id)
{
    NetLink *link = 0;
    if(unauthenticatedGameClientHash)
        stashIntRemovePointer(unauthenticatedGameClientHash, id, &link);
}

static void getUnauthenticatedGameLink(U32 id, NetLink **link)
{
    if(!unauthenticatedGameClientHash)
    {
        return;
    }

    stashIntFindPointer( unauthenticatedGameClientHash, id, link);
}


static void SetUnauthenticatedGameLink(U32 id, NetLink *link)
{
    NetLink *oldLink = 0;
    getUnauthenticatedGameLink(id, &oldLink);
    if(oldLink)
    {
        DelUnauthenticatedGameLink(id);
        lnkBatchSend(oldLink);
        netRemoveLink(oldLink);
    }
    if(!unauthenticatedGameClientHash)
    {
        unauthenticatedGameClientHash = stashTableCreateInt(STARTING_CLIENT_HASH_SIZE);
    }

    stashIntAddPointer(unauthenticatedGameClientHash, id, link, false);
}

#if defined QUEUESERVER_VERIFICATION
QueueServerState g_queueServerState_debug = {0};
static int stash_countClientActivity(StashElement element)
{
    NetLink *link = (NetLink*)stashElementGetPointer(element);
    GameClientLink* game = link?(GameClientLink *)link->userData:0;

    if(game && game->valid)
    {
        if(game->loginStatus == GAMELLOGIN_LOGGINGIN)
            g_queueServerState_debug.current_players++;
        else if(game->loginStatus == GAMELLOGIN_QUEUED)
            g_queueServerState_debug.waiting_players++;
        else
            assert(0);
    }
    return 1;
}


void queueserver_updateCount()
{
    g_queueServerState_debug.waiting_players = g_queueServerState_debug.current_players = 0;
        if(gameClientHashByName)
            stashForEachElement(gameClientHashByName, stash_countClientActivity);
    /*if(!devassert(g_queueServerState.current_players == g_queueServerState_debug.current_players))
        g_queueServerState.current_players = g_queueServerState_debug.current_players;*/
    if(!devassert(g_queueServerState.waiting_players >= g_queueServerState_debug.waiting_players))
        g_queueServerState.waiting_players = g_queueServerState_debug.waiting_players;
}
#endif
////////////////////////////////////////////////////////////////////////////////

static void s_keepName(const void *object, int keep)
{
    int tempResult;
    const char *name = object;
    if(name && !keep)
        stashRemoveInt(g_queueServerState.allow_list.allowed_stash, name, &tempResult);
}
static void s_addName(const void *object, int add)
{
    const char * name = object;
    if(name && add)
        stashAddInt(g_queueServerState.allow_list.allowed_stash,name,1,true);
}

/*static void s_receiveAuthLimiterList(Packet *pak_in)
{
    int changes = pktGetBitsAuto(pak_in);
    char ** names = g_queueServerState.allow_list.allowed_names;
    static char **newList = 0;
    static char **differenceList = 0;
    int nNames = eaSize(&names);
    int i, j, lastRead = -1;

    if(!changes)
        return;

    eaSetSize(&newList, 0);
    eaSetSize(&differenceList, 0);

    for(i = 0, j = 0; i < changes;)
    {
        char * name = pktGetString(pak_in);
        if(devassert(name))
            eaPush(&differenceList, name);
    }

    //update the names and stash    
    eaSortedDifferenceAction(&newList, &names,&differenceList,strcmp, s_keepName, s_addName);
}*/


//client to dbserver
int clientMsgCallback(Packet *pak_in, int cmd, NetLink *link)
{
    int cursor = bsGetCursorBitPosition(&pak_in->stream); // for debugging
    GameClientLink *client = link->userData;
    Packet *pak_out = 0; //pktCreateEx(&g_queueServerState.db_comm_link,QUEUECLIENT_CLIENTCOMMAND);
    g_queueServerState.packet_count++;

    if(client->remove)    //ignore anything coming from a link marked for removal.
        return 1;

    if((cmd != DBGAMECLIENT_LOGIN && cmd != DBGAMECLIENT_QUITCLIENT))
    {//if they're being deleted or not logging in (if they're in the queue), ignore anything they have to say.
        if(!devassert(client && client->account_name && client->loginStatus==GAMELLOGIN_LOGGINGIN))
            return 1;
    }

    pak_out = pktCreateEx(&g_queueServerState.db_comm_link,QUEUECLIENT_CLIENTCOMMAND);
    pktSendBitsAuto(pak_out, cmd);
    if(cmd != DBGAMECLIENT_LOGIN)
        pktSendString(pak_out, client->account_name);


    // This forces data to go back to the dbserver a little more smoothly
    // when there's a huge number of incoming packets.
    if(timerElapsed(g_queueServerState.packet_timer) > QUEUESERVER_PACKETFLUSHTIME)
    {
        lnkFlushAll();
        timerStart(g_queueServerState.packet_timer);
    }

    switch(cmd)
    {
    xcase DBGAMECLIENT_LOGIN:
    {
        NetLink *existingLink = 0;
        if(client->loginStatus == GAMELLOGIN_UNINITIALIZED)
        {
            client->loginStatus = GAMELLOGIN_UNAUTHENTICATED;
            pktSendBitsAuto(pak_out, g_queueServerState.waiting_players);
            clientPassLogin(pak_in, pak_out, client, link->addr.sin_addr.S_un.S_addr, link->UID);
            pktSend(&pak_out,&g_queueServerState.db_comm_link);
            SetUnauthenticatedGameLink(link->UID, link);
        }
    }

    xcase DBGAMECLIENT_CAN_START_STATIC_MAP:
    {
        clientPassCanStartStaticMap(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }
    
    xcase DBGAMECLIENT_CHOOSE_PLAYER:
    {
        clientPassChoosePlayer(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }
    
    xcase DBGAMECLIENT_CHOOSE_VISITING_PLAYER:
    {
        clientPassChooseVisitingPlayer(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_MAKE_PLAYER_ONLINE:
    {
        clientPassMakePlayerOnline(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_DELETE_PLAYER:
    {
        clientPassDeletePlayer(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_GET_COSTUME:
    {
        clientPassGetCostume(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }
    xcase DBGAMECLIENT_GET_POWERSET_NAMES:
    {
        clientPassGetPowersetName(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }
    xcase DBGAMECLIENT_ACCOUNTSERVER_UNSECURE_CMD:
    {
        // not handled.  There's a TODO for Aaron Brady to implement this,
        // whatever it is.  I look forward to it.
    }

    xcase DBGAMECLIENT_ACCOUNTSERVER_CHARCOUNT:
    {
        clientPassCharacterCounts(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_ACCOUNTSERVER_CATALOG:
    {
        clientPassCatalog(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_ACCOUNTSERVER_LOYALTY_BUY:
    {
        clientPassLoyaltyBuy(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_ACCOUNTSERVER_LOYALTY_REFUND:
    {
        clientPassLoyaltyRefund(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_SHARD_XFER_TOKEN_REDEEM:
    {
        clientPassShardXferTokenRedeem(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_RENAME_CHARACTER:
    {
        clientPassRename(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_RENAME_TOKEN_REDEEM:
    {
        clientPassRenameTokenRedeem(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_RESEND_PLAYERS:
    {
        clientPassResendPlayers(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_CHECK_NAME:
    {
        clientPassCheckName(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_SLOT_TOKEN_REDEEM:
    {
        clientPassRedeemSlot(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_UNLOCK_CHARACTER:
    {
        clientPassUnlockCharacter(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBCLIENT_ACCOUNTSERVER_CMD: // Testing only! this could be anybody.
    {
        clientPassAccountCmd(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_QUITCLIENT:
    {
        clientPassQuitClient(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }

    xcase DBGAMECLIENT_SIGN_NDA:
    {
        clientPassSignNDA(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }
    
    xcase DBGAMECLIENT_GET_PLAYNC_AUTH_KEY:
    {
        clientPassGetPlayNCAuthKey(pak_in, pak_out);
        pktSend(&pak_out,&g_queueServerState.db_comm_link);
    }
    
    xcase QUEUECLIENT_CLIENTPLAY:
    pktSend(&pak_out,&g_queueServerState.db_comm_link);

xdefault:
    printf("invalid msg %i", cmd);
    }
    return 1;
}

//dbserver to client
int shardMsgCallback(Packet *pak_in, int cmd, NetLink *link)
{
    int cursor = bsGetCursorBitPosition(&pak_in->stream); // for debugging
    char * clientId;
    NetLink *client = 0;
    Packet *pak_out = 0;
    U32 clientLinkId = 0;
    GameClientLoginStatus clientDbStatus;
    GameClientLink *gameLink = 0;
    g_queueServerState.packet_count++;

    // This forces data to go back to the dbserver a little more smoothly
    // when there's a huge number of incoming packets.
    if(timerElapsed(g_queueServerState.packet_timer) > QUEUESERVER_PACKETFLUSHTIME)
    {
        lnkFlushAll();
        timerStart(g_queueServerState.packet_timer);
    }
    if(cmd < QUEUESERVER_SVR_CONNECT)
    {
        clientId = pktGetString(pak_in);
        clientLinkId = pktGetBitsAuto(pak_in);
        getGameLink(clientId, &client, clientLinkId);
        if(!client && cmd == DBGAMESERVER_MSG)
        {
            getUnauthenticatedGameLink(clientLinkId, &client);
        }
        if(!client)
        {
            printf("Received client ID %s which could not be found!  Command %d.  Ignoring.\n", clientId, cmd);
            return 0;
        }
        pak_out = pktCreateEx(client,cmd);
        if (pktGetOrdered(pak_in))
        {
            pktSetOrdered(pak_out, 1);
        }
    }

    switch(cmd)
    {
        xcase QUEUESERVER_SVR_CONNECT:
    {
        U32 protocol = pktGetBitsAuto(pak_in);
        assert(protocol == DBSERVER_QUEUE_PROTOCOL_VERSION);

        if(pktGetBool(pak_in))// do we use the queueserver
        {
            g_queueServerState.authLimitedOnlyAutoAllow = !!pktGetBitsAuto(pak_in);
            g_queueServerState.max_activePlayers = pktGetBitsAuto(pak_in);
            g_queueServerState.current_players = pktGetBitsAuto(pak_in);
            //s_receiveAuthLimiterList(pak_in);
        }
        else
        {
            loadend_printf("Dbserver not setup to use Queue Server.");
            exit(0);
        }
    }

    xcase QUEUESERVER_SVR_ERROR:
    {
        printf("CLOSING DUE TO ERROR: %s\n", pktGetStringTemp(pak_in));
    }

    xcase QUEUESERVER_SVR_ENQUEUESTATUS:
    {
        int isVIP, loyaltyPoints;

        clientId = pktGetString(pak_in);
        clientLinkId = pktGetBitsAuto(pak_in);
        clientDbStatus = pktGetBitsAuto(pak_in);
        isVIP = pktGetBitsAuto(pak_in);
        loyaltyPoints = pktGetBitsAuto(pak_in);

        getUnauthenticatedGameLink(clientLinkId, &client);
        if(client)
        {
            DelUnauthenticatedGameLink(clientLinkId);
            gameLink = (GameClientLink *)client->userData;
            if(gameLink->loginStatus == GAMELLOGIN_UNAUTHENTICATED && gameLink->account_name && !stricmp(clientId, gameLink->account_name))
            {
                AuthenticateGameLink(client);
                gameLink->payStat = isVIP;
                if(clientDbStatus == GAMELLOGIN_QUEUED)
                {
                    queueserver_enqueue(clientId, clientLinkId, loyaltyPoints);
                }
                else if (clientDbStatus == GAMELLOGIN_LOGGINGIN)
                {
                    Packet *packet;
                    gameLink->loginStatus = GAMELLOGIN_LOGGINGIN;
                    g_queueServerState.current_players++;
                    eaPushUnique(&players_waiting_for_character_list_retrieval, client);

                    packet = pktCreateEx(client,DBGAMESERVER_QUEUE_POSITION);
                    pktSendBitsAuto(packet, 0);
                    pktSendBitsAuto(packet, g_queueServerState.waiting_players);
                    pktSend(&packet, client);

                    packet = pktCreateEx(&g_queueServerState.db_comm_link,QUEUECLIENT_CLIENTPLAY);
                    pktSendString(packet, gameLink->account_name);
                    pktSendBitsAuto(packet, client->UID);
                    pktSend(&packet, &g_queueServerState.db_comm_link);
                }
                else
                    devassert(0);
            }
            else
            {
                printf("Queuing Command for %s, but %s found with net ID %d\n", clientId, gameLink->account_name, clientId);
            }
        }
    }

    xcase QUEUESERVER_SVR_UPDATE:
    {
        queueserver_handleCountUpdate(pak_in);
    }

    xcase QUEUESERVER_SVR_REMOVECLIENT:
    {
        clientId = pktGetString(pak_in);
        clientLinkId = pktGetBitsAuto(pak_in);
        if(clientLinkId)
            getUnauthenticatedGameLink(clientLinkId, &client);
        if(!client)
            getGameLink(clientId, &client, clientLinkId);
        if(client)
        {
            gameLink = client->userData;
            if(!devassert(gameLink))
            {
                lnkBatchSend(client);
                netRemoveLink(client);
            }
            else
            {
                gameLink->remove = timerCpuSeconds() + QUEUESERVER_WAITTOREMOVE;
            }
        }
    }
    xcase QUEUESERVER_SVR_SET_LOG_LEVELS:
    {
        int i;
        for( i = 0; i < LOG_COUNT; i++ )
        {
            int log_level = pktGetBitsPack(pak_in,1);
            logSetLevel(i,log_level);
        }
    }
    xcase QUEUESERVER_SVR_TEST_LOG_LEVELS:
    {
        LOG( LOG_DEPRECATED, LOG_LEVEL_ALERT, LOG_CONSOLE, "QueueServer Log Line Test: Alert" );
        LOG( LOG_DEPRECATED, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "QueueServer Log Line Test: Important" );
        LOG( LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "QueueServer Log Line Test: Verbose" );
        LOG( LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, LOG_CONSOLE, "QueueServer Log Line Test: Deprecated" );
        LOG( LOG_DEPRECATED, LOG_LEVEL_DEBUG, LOG_CONSOLE, "QueueServer Log Line Test: Debug" );
    }


    //server to client communication
    xcase DBGAMESERVER_SEND_PLAYERS:
    {
        passSendPlayers(pak_in, pak_out);
        eaFindAndRemove(&players_waiting_for_character_list_retrieval, client);
    }

    xcase DBGAMESERVER_SEND_COSTUME:
    {
        passSendCostume(pak_in, pak_out);
    }

    xcase DBGAMESERVER_SEND_POWERSET_NAMES:
    {
        passSendPowersetNames(pak_in, pak_out);
    }

    xcase DBGAMESERVER_ACCOUNTSERVER_CHARCOUNT:
    {
        passSendCharacterCounts(pak_in, pak_out);
    }

    xcase DBGAMESERVER_ACCOUNTSERVER_CATALOG:
    {
        passSendAccountServerCatalog(pak_in, pak_out);
    }

    xcase DBGAMESERVER_ACCOUNTSERVER_CLIENTAUTH:
    {
        passAccountServerClientAuth(pak_in, pak_out);
    }

    xcase DBGAMESERVER_ACCOUNTSERVER_PLAYNC_AUTH_KEY:
    {
        passAccountServerPlayNCAuthKey(pak_in, pak_out);
    }

    xcase DBGAMESERVER_ACCOUNTSERVER_NOTIFYREQUEST:
    {
        passAccountServerNotifyRequest(pak_in, pak_out);
    }

    xcase DBGAMESERVER_ACCOUNTSERVER_INVENTORY:
    {
        passAccountServerInventoryDB(pak_in, pak_out);
    }

    xcase DBGAMESERVER_ACCOUNTSERVER_UNAVAILABLE:
    {
        passAccountServerUnavailable(pak_in, pak_out);
    }

    xcase DBGAMESERVER_MAP_CONNECT:
    {
        passdbOrMapConnect(pak_in, pak_out);
    }

    xcase DBGAMESERVER_CAN_START_STATIC_MAP_RESPONSE:
    {
        passCanStartStaticMapResponse(pak_in, pak_out);
    }

    xcase DBGAMESERVER_DELETE_OK:
    {
        pktSendGetBits(pak_out, pak_in, 1);//db_rename_status
    }

    xcase DBGAMESERVER_MSG:
    {
        pktSendGetStringNLength(pak_out, pak_in);//error_msg
    }

    xcase DBGAMESERVER_CONPRINT:
    {
        pktSendGetStringNLength(pak_out, pak_in);
    }

    xcase DBGAMESERVER_RENAME_RESPONSE:
    {
        pktSendGetStringNLength(pak_out, pak_in);
    }

    xcase DBGAMESERVER_CHECK_NAME_RESPONSE:
    {
        pktSendGetBitsAuto(pak_out, pak_in); //CheckNameSuccess
        pktSendGetBitsAuto(pak_out, pak_in); //
    }

    xcase DBGAMESERVER_UNLOCK_CHARACTER_RESPONSE:
    {
    }

    xcase DBGAMESERVER_LOGIN_DIALOG_RESPONSE:
    {
        pktSendGetStringNLength(pak_out, pak_in);
    }


xdefault:
    printf("invalid msg %i\n", cmd);
    };

    if(pak_out && client)
        pktSend(&pak_out, client);
    return 1;
}

static void shardNetProcessTick(QueueServerState *state)
{
    NetLink *link = &state->db_comm_link;
    if(link->connected)
    {
        netLinkMonitor(link, 0, shardMsgCallback);

        lnkBatchSend(link);
        netIdle(link, 0, 10.f);
        link->last_update_time = timerCpuSeconds(); // reset on disconnect in netLinkMonitor
    }
}
////////////////////////////////////////////////////////////////////////////////
//Server Lib definitions

void queueserver_init(void);
void queueserver_load(void);
void queueserver_tick(void);
void queueserver_stop(void);
void queueserver_http(char**);

ServerLibConfig g_serverlibconfig =
{
    "QueueServer",    // name
    'Q',                // icon
    0x00ccff,            // color
    queueserver_init,
    queueserver_load,
    queueserver_tick,
    queueserver_stop,
    queueserver_http,
    NULL,
    kServerLibNetEncryption,
};

void queueserver_init(void)
{
    int i;
    for(i = 1; i < g_serverlibstate.argc; i++)
    {
        if (strcmp(g_serverlibstate.argv[i],"-db")==0 && i+1<g_serverlibstate.argc)
        {
            strcpy_s(g_queueServerState.server_name,sizeof(g_queueServerState.server_name),g_serverlibstate.argv[++i]);
        }
    }

    if(!g_queueServerState.server_name || !g_queueServerState.server_name[0])
    {
        if(isProductionMode())
        {
            assertmsg(0, "No dbserver defined (\"-db <server>\" cmdline option)!");
        }
        else
        {
            strcpy(g_queueServerState.server_name, "localhost");
            printf("No dbserver specified.  Using localhost by default.\n");
        }
    }
}

int netGameClientLinkDelCallback(NetLink *link)
{
    GameClientLink    *client = link->userData;
    Packet *pak = pktCreateEx(&g_queueServerState.db_comm_link,QUEUECLIENT_REMOVECLIENT); 
    pktSendString(pak, client->account_name);
    pktSendBitsAuto(pak, link->UID);
    pktSend(&pak,&g_queueServerState.db_comm_link);
    DelGameLink(client->account_name);
    DelUnauthenticatedGameLink(link->UID);

    return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Async connect stuff
static void printf_timestamp(char *fmt, ...)
{
    char date_buf[100];
    va_list argptr;

    timerMakeDateString(date_buf);
    printf("%s ", date_buf);

    va_start(argptr, fmt);
    vprintf(fmt, argptr);
    va_end(argptr);
}

// Start a connection attempt to a Dbserver.  This is non-blocking.
void connectDbStart(char *dbServerName, NetLink *netLink)
{
    printf_timestamp("Starting connection to %s\n", dbServerName);
    netConnectAsync(netLink,dbServerName,DEFAULT_QUEUESERVER_PORT,NLT_TCP,5.0f);
}

// Once a connection attempt has been started, call this each loop to see if the connection has succeeded.
int connectDbPoll(char *dbServerName, NetLink *netLink)
{
    int ret;

    ret = netConnectPoll(netLink);

    // If anything other than success ....
    if (ret != 1)
    {
        // -1 means failure
        if (ret == -1)
        {
            // Print a message.  The NetLink will be left in a disconnected state, which we detct in the main loop and react as needed.
            printf_timestamp("Connection to %s failed\n", dbServerName);
        }
        // Anything else (i.e. zero) means it's still in progress so do nothing.
        return ret;
    }

    //listen for clients.
    clientCommLoginInit(&gameclient_links);
    clientCommLoginInitStartListening(&gameclient_links,clientMsgCallback, netGameClientLinkDelCallback);

    return ret;
}



void queueserver_load(void)
{
    g_queueServerState.tick_timer = timerAlloc();
    g_queueServerState.packet_timer = timerAlloc();

#if 0
    // DGNOTE 8/10/2010
    // This is the original connect code - it's the "try once and commit suicide if we don't connect" version.  Given the current condition of the
    // dbserver this isn't as hideous as it sounds.  As things currently stand, it appears that if the queueserver starts a connection attempt too
    // early, the dbserver gets confused and gets stuck in limbo.  I don't know exactly where it gets jammed up, but if you #if 0 this out, and #if 1
    // in the block of code at the top of queueserver_tick(...), that'll switch this over to my standard "retry" code.  If you then start the queue
    // server before the dbserver, it's pretty close to guaranteed that you'll wedge the dbserver.
    //TODO: request startup info from dbserver.
    //connect to dbserver
    loadstart_printf("Connecting to dbserver...");
    initNetLink(&g_queueServerState.db_comm_link);
    loadstart_printf("connecting to %s...",g_queueServerState.server_name);
    netConnect(&g_queueServerState.db_comm_link, g_queueServerState.server_name, DEFAULT_QUEUESERVER_PORT, NLT_TCP, 40.0f, NULL);

    if(g_queueServerState.db_comm_link.connected){
        Packet *pak = pktCreateEx(&g_queueServerState.db_comm_link,QUEUECLIENT_CONNECT); 
        pktSend(&pak,&g_queueServerState.db_comm_link);
        loadend_printf("done.");
    }else{
        loadend_printf("FAILED.");
        exit(0);
    }

    //listen for clients.
    clientCommLoginInit(&gameclient_links);
    clientCommLoginInitStartListening(&gameclient_links,clientMsgCallback, netGameClientLinkDelCallback);
#endif
}

// DGNOTE 8/10/2010
// If you do enable the "retry" block in queueserver_tick(...), this #define does pretty much what it claims.  By enabling it, you introduce a 30
// second delay at startup, before the queueserver tries to connect to the dbserver.  This appears to be long enough to allow the dbserver to get
// going properly.  If and when we fix the dbserver startup, we can then run with the queueserver_tick block enabled, but without this delay.  For
// testing the dbserver failure, you probably want this disabled.
#define I_WANT_A_30_SECOND_DELAY_AT_STARTUP 1

void queueserver_tick(void)
{
    F32 time_elapsed;

// DGNOTE 8/10/2010
// This is the new and improved retry code.  See the comment above in queueserver_load(...) for details on why you might want to turn this on
#if 1
    int result;
    static int connecting_to_dbserver = 1;
#ifdef I_WANT_A_30_SECOND_DELAY_AT_STARTUP
    static int retryTimer = -1;                // timer used to throttle reconnect attempts

    // This little section introduces a 30 second delay on startup
    if (retryTimer == -1)
    {
        retryTimer = timerAlloc();
        timerStart(retryTimer);
        // add -25, so that the elapsed > 5 below really becomes elapsed > 30
        timerAdd(retryTimer, -25.0f);
        printf_timestamp("Starting artificial 30 second delay\n");
    }
#else
    static int retryTimer = 0;                // timer used to throttle reconnect attempts
#endif

    // Initially assume we havent connected to the db server.  In that case, try and connect.
    if (connecting_to_dbserver)
    {
        // If we're not connected, the pending flag says whether the link is disconnected and idle, or in the process of trying an async connect
        if (g_queueServerState.db_comm_link.pending)
        {
            // If it's pending, there's a connect in progress, so poll for completion
            result = connectDbPoll(g_queueServerState.server_name, &g_queueServerState.db_comm_link);
            if (result == -1)
            {
                // Handle the error
                retryTimer = timerAlloc();
                timerStart(retryTimer);
            }
            if (!g_queueServerState.db_comm_link.pending && g_queueServerState.db_comm_link.connected)
            {
                // We're connected.  Note the fact.
                connecting_to_dbserver = 0;
            }
        }
        // Not pending, check the retry timer
        else if (retryTimer == 0)
        {
            // No active retry timer, it must have timed out, so kick start a connection attempt.
            connectDbStart(g_queueServerState.server_name, &g_queueServerState.db_comm_link);
        }
        else
        {
            // Otherwise the retry timer is counting
            if (timerElapsed(retryTimer) > 5.0f)
            {
                // If it's finished clean up, this will cause the connection attempt next time through.
                timerFree(retryTimer);
                retryTimer = 0;
            }
        }
        return;
    }
#endif
    
    //if the dbserver is down, quit!
    if(!g_queueServerState.db_comm_link.connected)
    {
        printf("Dbserver disconnected.  Shutting down queue server.\n");
        exit(0);
    }
    clientCommLoginClearDeadLinks(gameclient_links.links);

    shardNetProcessTick(&g_queueServerState);
    timerStart(g_queueServerState.packet_timer);

    time_elapsed = timerElapsed(g_queueServerState.tick_timer);

    if(time_elapsed > (1.f / QUEUESERVER_TICK_FREQ)) //NOTE!! Once a second we 1) update display, 2) request update of players on dbserver
    {
        char *buf = Str_temp();
        U32 currentTick = (g_queueServerState.tick_count / QUEUESERVER_TICK_FREQ);
        U32 isPlayerUpdateTick = !(currentTick%QUEUESERVER_PLAYER_UPDATE_FREQ);

        Str_catf(&buf, "%d: QueueServer. %d queued (%d). ", 
            _getpid(), g_queueServerState.waiting_players, 
             currentTick% 10);
        setConsoleTitle(buf);

        timerStart(g_queueServerState.tick_timer);
        g_queueServerState.tick_count++;
        Str_destroy(&buf);

        queueserver_requestCountUpdate();
        queueserver_letPlayersThrough();
        
        if(isPlayerUpdateTick)
            queueserver_updateClients();
    }
    clientCommLoginClearDeadLinks(gameclient_links.links);
}

void queueserver_stop(void)
{
    //int i;
    //TODO: send the quit signal to the auth server for everyone in the queue
    printf("Quitting Queue Server.\n");
}

void queueserver_http(char **str)
{
/*#define sendline(fmt, ...)    Str_catf(str, fmt"\n", __VA_ARGS__)
#define rowclass            ((row++)&1)

    int i;
    U32 now = timerCpuSeconds();

    int row = 0;
    sendline("<table>");
    sendline("<tr class=rowt><th colspan=6>MissionServer Summary");
    sendline("<tr class=rowh><th>Shard<th>Region<th>IP Address<th>Connected");
    for(i = 0; i < g_missionServerState.shard_count; ++i)
    {
        MissionServerShard *shard = g_missionServerState.shards[i];
        if(shard)
        {
            sendline("<tr class=row%d><td>%s<td>%s<td>%s<td>%s", rowclass,
                shard->name, authregion_toString(shard->auth_region),
                shard->ip, shard->link.connected ? "&#x2713;" : "");
        }
    }
    sendline("</table>");*/
}

///////////////////////////////////////////////////////////////////////
// THE QUEUESERVER QUA QUEUE
///////////////////////////////////////////////////////////////////////
typedef struct QueueElement
{
    int        enqueuedTime;
    char    authname[64];
    int        priority;
    U32        linkId;
    struct QueueElement *next;
    struct QueueElement *prev;
} QueueElement;
MP_DEFINE(QueueElement);

static BinHeap *priority_queue = NULL;
static int is_queue_dirty = true;

int queueserver_CmpPriority(void *lhs, void *rhs)
{
    QueueElement *a = lhs;
    QueueElement *b = rhs;
    return a->enqueuedTime - b->enqueuedTime;
}

int queueserver_CalculatePriority(int time, int priortity)
{
    return time - (priortity * 60);
}

void queueserver_enqueue(char *authname, U32 linkId, int priority)
{
    QueueElement *newElement = 0;
    NetLink * clientLink = 0;

    getGameLink(authname, &clientLink, linkId);

    if(!devassert(clientLink))
        return;
    else
    {
        GameClientLink    *client = clientLink->userData;
        if(!client || client->loginStatus==GAMELLOGIN_QUEUED)
            return;
        client->loginStatus = GAMELLOGIN_QUEUED;
    }

    if (priority_queue == NULL)
        priority_queue = binheap_Create(queueserver_CmpPriority);

    MP_CREATE(QueueElement, g_queueServerState.max_activePlayers?g_queueServerState.max_activePlayers:1000);//allocate enough for 2x the number of active players.
    newElement = MP_ALLOC( QueueElement );
    strcpy(newElement->authname,authname);
    newElement->linkId = linkId;
    newElement->enqueuedTime = queueserver_CalculatePriority(timerSecondsSince2000(), priority);
    newElement->priority = priority;
    newElement->next = 0;
    newElement->prev = 0;
    g_queueServerState.waiting_players++;

    binheap_Push(priority_queue, newElement);
    is_queue_dirty = true;
}

NetLink *queueserver_pop(int offset)
{
    NetLink *head = 0;
    char *authname = 0;
    GameClientLink    *client = 0;
     while((!head || !head->connected) && binheap_Size(priority_queue) > offset)
    {
        QueueElement *oldHead = binheap_Remove(priority_queue, offset);
        is_queue_dirty = true;
        if (oldHead)
        {
            authname = oldHead->authname;
            if(authname && strlen(authname))
            {
                getGameLink(authname, &head, oldHead->linkId);
            }

            //else --> this guy has already been replaced by someone else with the same name.
            MP_FREE(QueueElement, oldHead);
            g_queueServerState.waiting_players--;

            if(head)
            {
                client = head->userData;
            }
        }
    }
    if(!head)
    {
        devassert(!g_queueServerState.waiting_players);
        g_queueServerState.waiting_players = 0;
    }
    else if(client)
    {
        devassert(client->loginStatus == GAMELLOGIN_QUEUED);
        client->loginStatus = GAMELLOGIN_LOGGINGIN;
    }
    return head;
}

NetLink *queueserver_peek(int offsetFromHead)
{
    NetLink *head = 0;
    char *authname = 0;
    QueueElement *element = binheap_Peek(priority_queue, offsetFromHead);
    authname = element->authname;
    if(authname && strlen(authname))
    {
        getGameLink(authname, &head, element->linkId);
    }

    return head;
}

void queueserver_updateClients()    //only processes people in the queue
{
    QueueElement *last = 0;
    int position = 0, index = 0;
    QueueElement *current = NULL;
    U32 curr_seconds = timerCpuSeconds();
    BinHeap *queueCopy = NULL;

    if (priority_queue == NULL)
        priority_queue = binheap_Create(queueserver_CmpPriority);

    // check to see if we need to re-sort the queue
    if (is_queue_dirty)
    {
        queueCopy = binheap_Copy(priority_queue);

        last = NULL;
        while(binheap_Size(queueCopy) > 0)
        {
            current = binheap_Pop(queueCopy);
            if (current)
            {
                current->prev = last;
                if (last)
                    last->next = current;
                last = current;
            }
        }

        if (last)
            last->next = NULL;

        is_queue_dirty = false;
    }

    current = binheap_Top(priority_queue);
    while(current != NULL)
    {
        NetLink *client = 0;
        GameClientLink *game = 0;

        position++;
        getGameLink(current->authname, &client, current->linkId);
        if(client)
        {
            game = (GameClientLink *)client->userData;
        }

        if(!client || (!devassert(game->loginStatus == GAMELLOGIN_QUEUED)))
        {
            QueueElement *toDelete = current;
            current = current->next;
            position--;
            binheap_RemoveElt(priority_queue, toDelete);
            MP_FREE(QueueElement, toDelete);        
            g_queueServerState.waiting_players--;
            is_queue_dirty = true;
        }
        else
        {
            Packet *pak_out = pktCreateEx(client,DBGAMESERVER_QUEUE_POSITION);
            pktSendBitsAuto(pak_out, position);
            pktSendBitsAuto(pak_out, g_queueServerState.waiting_players);
            pktSend(&pak_out, client);
            current = current->next;
        }
    }

    if(!devassert(position == g_queueServerState.waiting_players))
        g_queueServerState.waiting_players = position;

    // Stayin' alive!
    for (index = eaSize(&players_waiting_for_character_list_retrieval) - 1; index >= 0; index--)
    {
        Packet *pak_out = pktCreateEx(players_waiting_for_character_list_retrieval[index], DBGAMESERVER_QUEUE_POSITION);
        pktSendBitsAuto(pak_out, 0);
        pktSendBitsAuto(pak_out, g_queueServerState.waiting_players);
        pktSend(&pak_out, players_waiting_for_character_list_retrieval[index]);
    }

    binheap_Destroy(queueCopy);
}

void queueserver_letPlayersThrough()
{
    static U32 secondsPassed = 0;
    static int second = 0;
    static int loggedInThisSecond = 0;
    int now = timerCpuSeconds();
    static int loginsAllowedThisSecond = 0;
    int offsetFromHead = 0;
    
    if(second != now)
    {
        int loginsAllowedNow = 0;    //total allowed in this minute up to now
        int loginsAllowedLast = 0;    //total allowed in this minute up to last second
        //update time information
        second = now;
        loggedInThisSecond = 0;
        secondsPassed %= SECONDS_PER_MINUTE;
        secondsPassed++;    //between 1 and SECONDS_PER_MINUTE

        //get the number of new logins this second
        loginsAllowedLast = floor(g_queueServerState.loginsPerSecond*(secondsPassed-1));
        loginsAllowedNow = floor(g_queueServerState.loginsPerSecond*(secondsPassed));
        loginsAllowedThisSecond = loginsAllowedNow - loginsAllowedLast;
    }

    // Let the above code update the 'second' variable first so when the overload protection
    // is disabled, players will trickle back in properly.
    if (g_queueServerState.overload_protection_active)
    {
        return;
    }

    while(loggedInThisSecond < loginsAllowedThisSecond && 
        !g_queueServerState.authLimitedOnlyAutoAllow && 
        g_queueServerState.current_players < g_queueServerState.max_activePlayers && 
        offsetFromHead < (int) g_queueServerState.waiting_players)
    {
        NetLink *client = queueserver_peek(offsetFromHead);
        GameClientLink *game = client?client->userData:NULL;

        offsetFromHead++;

        if (!game)
            continue;

        // check to see if we're blocking free players
        if (game && !g_queueServerState.isAccountServerActive && !game->payStat)
        {
            continue;
        } 

        client = queueserver_pop(offsetFromHead - 1);  // Offset was already incremented above
        game = client?client->userData:NULL;
        if(game)
        {
            Packet *pak_out = pktCreateEx(client,DBGAMESERVER_QUEUE_POSITION);
            pktSendBitsAuto(pak_out, 0);
            pktSendBitsAuto(pak_out, g_queueServerState.waiting_players);
            pktSend(&pak_out, client);

            pak_out = pktCreateEx(&g_queueServerState.db_comm_link,QUEUECLIENT_CLIENTPLAY);
            pktSendString(pak_out, game->account_name);
            pktSendBitsAuto(pak_out, client->UID);
            pktSend(&pak_out, &g_queueServerState.db_comm_link);
            g_queueServerState.current_players++;
            loggedInThisSecond++;
        }
    }
}

void queueserver_handleCountUpdate(Packet *pak_in)
{
    int max_logins_per_minute = 0;
    int new_overload_protection_active;
    g_queueServerState.current_players = pktGetBitsAuto(pak_in);
    g_queueServerState.max_activePlayers = pktGetBitsAuto(pak_in);
    max_logins_per_minute = pktGetBitsAuto(pak_in);
    g_queueServerState.isAccountServerActive = pktGetBitsAuto(pak_in);
    //g_queueServerState.authLimitedOnlyAutoAllow = pktGetBool(pak_in);    //This is only necessary if the auth limiter can change without a cfg update.
    g_queueServerState.loginsPerSecond = ((F32)max_logins_per_minute/((F32)SECONDS_PER_MINUTE));
    if(g_queueServerState.loginsPerSecond <= 0)
        g_queueServerState.loginsPerSecond = ((F32)QUEUESERVER_LOGINS_PER_MINUTE_DEFAULT/((F32)SECONDS_PER_MINUTE));

    new_overload_protection_active = pktGetBitsAuto(pak_in);
    if (new_overload_protection_active != g_queueServerState.overload_protection_active)
    {
        g_queueServerState.overload_protection_active = new_overload_protection_active;
        printf("Overload Protection is now %s\n", new_overload_protection_active ? "ENABLED" : "DISABLED");
    }
#if defined QUEUESERVER_VERIFICATION
    queueserver_updateCount();
#endif
}

void queueserver_requestCountUpdate()
{
    Packet *pak_out = pktCreateEx(&g_queueServerState.db_comm_link,QUEUECLIENT_UPDATE_REQUEST);
    pktSendBitsAuto(pak_out, g_queueServerState.waiting_players);
    pktSendBitsAuto(pak_out, clientCommLoginConnectedCount(&gameclient_links));
    pktSend(&pak_out, &g_queueServerState.db_comm_link);
}
