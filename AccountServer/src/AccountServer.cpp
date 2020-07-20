/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include "AccountServer.hpp"
#include "account/AccountData.h"
#include "AccountDb.hpp"
#include "AccountCmds.h"
#include "cmdparse/CmdAccountServer.h"
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/components/stringcache.h>
#include <utilitieslib/components/estring.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/foldercache.h>
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/memorymonitor.h>
#include <utilitieslib/network/net_masterlist.h>
#include <utilitieslib/components/stashTable.h>
#include <utilitieslib/network/net_linklist.h>
#include <utilitieslib/network/net_link.h>
#include "comm_backend.h"
#include <utilitieslib/utils/ConsoleDebug.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/network/netcomp.h>
#include "account/AccountData.h"
#include "account/AccountCatalog.h"
#include "AccountSql.h"
#include "request.hpp"
#include "request_certification.hpp"
#include "request_rename.hpp"
#include "request_respec.hpp"
#include "request_shardxfer.hpp"
#include "request_slot.hpp"
#include "transaction.h"
#include "account_inventory.h"
#include "account_loyaltyrewards.h"
#include "accountcmds.h"
#include <utilitieslib/utils/SuperAssert.h>
#include <DbgHelp.h>

/// Spread the authentication key generation over a period of time
#define SHARD_RECONNECT_REAUTH_PLAYERS_PER_SECOND (100)

#define MAX_FLUSH_TIME (2.0f)

/* 24 hours * 60 minutes * 60 secs */
#define DAYS_TO_SECS( _days_ )    (( _days_ ) * 24 * 60 * 60 )

int g_bVerbose = false;
int g_flush_timer = 0;
int g_activeAccount_timer = 0;
// bool g_bThrottled = false;
int g_packetcount = 0;

#define ACCOUNTSERVER_TICK_FREQ 32 //ticks/sec
#define SHARDRECONNECT_RETRY_TIME    10
#define LINKS_PER_SHARD                4
#define SHARD_DEADMAN_TIME            (60*2)
#define ACCOUNTSERVER_MERGE_RETRY    60
#define ACCOUNTSERVER_MERGE_DEAD    7200 // changed because live was taking over an hour.  Should be irrelevant after I21.

#define DEFAULT_PLAYSPAN_RELAY_SERVER_RETRY_SECS                15    // retry relay server connection this often
#define DEFAULT_PLAYSPAN_RELAY_SERVER_ACK_ALARM_SEC                45    // issue a warning if no data has arrive from the relay server
#define DEFAULT_PLAYSPAN_RELAY_SERVER_ACK_ALARM_REPEAT_SECS        30    // repeat the warning related to a missing ack this oftem
#define DEFAULT_PLAYSPAN_MAKE_TIME_STAMPED_DIGEST                1    // playspan now requires timestamp so default this to 1
#define DEFAULT_PLAYSPAN_AUTH_TIMEOUT_MINS                        60    // playspan now requires timestamp so default this to 1
#define DEFAULT_PLAYSPAN_REKEY_INTERVAL_MINS                    19    // rekey 3 times before timeout, so client can miss 2 with no issues

#define SafeNonNullStr( _aPtr_ ) ((( _aPtr_ ) == NULL ) ? "" : ( _aPtr_ ))

typedef struct NetLink NetLink;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static ParseTable parse_AccountServerShard[] =  
{
    { "ShardName",            TOK_STRUCTPARAM|TOK_POOL_STRING|TOK_STRING(AccountServerShard,name,0)    },
    { "ShardId",            TOK_U8(AccountServerShard,shard_id,0)                                    },
    { "Address",            TOK_STRING(AccountServerShard,ip,0)                                        },
    { "NoXferOut",            TOK_BOOLFLAG(AccountServerShard,shardxfer_not_a_src,0)                    },
    { "NoXferIn",            TOK_BOOLFLAG(AccountServerShard,shardxfer_not_a_dst,0)                    },
    { "XferDestinations",    TOK_STRINGARRAY(AccountServerShard,shardxfer_destinations)                },
    { "ShardEnd",            TOK_END                                                                    },
    { 0 },
};

StaticDefineInt parse_PlaySpanStoreFlags[] =
{
    DEFINE_INT
    {"NoLocalization",    STOREFLAG_NO_LOCALIZATION    },
    DEFINE_END
};

static ParseTable parse_AccountServerCfg[] =  
{
    { "ShardName",                                TOK_STRUCT(AccountServerCfg,shards,parse_AccountServerShard)    },
    { "MtxEnvironment",                            TOK_FIXEDSTR(AccountServerCfg,mtxEnvironment) },
    { "MtxSecretKey",                            TOK_FIXEDSTR(AccountServerCfg,mtxSecretKey) },
    { "MtxIOThreads",                            TOK_INT(AccountServerCfg,mtxIOThreads,1) },
    { "SqlLogin",                                TOK_FIXEDSTR(AccountServerCfg,sqlLogin) },
    { "SqlDbName",                                TOK_FIXEDSTR(AccountServerCfg,sqlDbName) },
    { "PlaySpanServerRetryFreqSecs",            TOK_INT(AccountServerCfg,playSpanServerRetryFreqSecs,DEFAULT_PLAYSPAN_RELAY_SERVER_RETRY_SECS)            },
    { "PlaySpanRelayServerAckAlarmSecs",        TOK_INT(AccountServerCfg,playSpanServerAckAlarmSecs,DEFAULT_PLAYSPAN_RELAY_SERVER_ACK_ALARM_SEC)            },
    { "PlaySpanRelayServerAckAlarmRepeatSecs",    TOK_INT(AccountServerCfg,playSpanServerAckAlarmRepeatFregSecs,DEFAULT_PLAYSPAN_RELAY_SERVER_ACK_ALARM_REPEAT_SECS)            },
    { "PlaySpanMakeTimeStampedDigest",            TOK_INT(AccountServerCfg,playSpanMakeTimeStampedDigest,DEFAULT_PLAYSPAN_MAKE_TIME_STAMPED_DIGEST)},
    { "CatalogTimeStampTestOffsetDays",            TOK_INT(AccountServerCfg, catalogTimeStampTestOffsetDays, 0)                        },
    { "PlaySpanStoreFlags",                        TOK_FLAGS(AccountServerCfg,playSpanStoreFlags,0), parse_PlaySpanStoreFlags        },
    { "PlaySpanAuthTimeoutMins",                TOK_INT(AccountServerCfg,playSpanAuthTimeoutMins,DEFAULT_PLAYSPAN_AUTH_TIMEOUT_MINS)            },
    { "PlaySpanAuthRekeyIntervalMins",            TOK_INT(AccountServerCfg,playSpanAuthRekeyIntervalMins,DEFAULT_PLAYSPAN_REKEY_INTERVAL_MINS)            },
    { "PlaySpanCatalog",                        TOK_STRING(AccountServerCfg,playSpanCatalog,"OURO")                },
    { "GrantSKUFromList",                        TOK_BOOLFLAG(AccountServerCfg,grant_sku_from_list,0)            },
    { "GrantAllSKU",                            TOK_BOOLFLAG(AccountServerCfg,grant_all_sku,0)                    },
    { "MinLoyaltyPoints",                        TOK_INT(AccountServerCfg,min_loyalty_points,0)                    },
    { "AutoBuyProducts",                        TOK_BOOLFLAG(AccountServerCfg,auto_buy_products,0)                },
    // Legacy stuff
    { "ArchiveMerged",                            TOK_BOOLFLAG(AccountServerCfg,archive_merged,0)                    },
    { "CompMode",                                TOK_BOOLFLAG(AccountServerCfg,comp_mode,0)                        },
    { "clientAuthTimeout",                        TOK_INT(AccountServerCfg, clientAuthTimeout, 1800)                            },
    { "ShardXfersDayMemory",                    TOK_INT(AccountServerCfg, shardXferDayMemory, 14)                            },
    { "ShardXfersAllowedInMemory",                TOK_INT(AccountServerCfg, shardXferAllowedInMemory, 0)                        },
    { "FreeShardXfersOnlyEnabled",                TOK_BOOLFLAG(AccountServerCfg, bFreeShardXfersOnlyEnabled, 0)    },
    { 0 },
}; 

AccountServerState g_accountServerState;

static StashTable s_ProductsWithUpdatesToBroadcast = NULL;

static void sendCatalogUpdatesToShard(AccountServerShard *shard );
static void handleUpdateEmailStats(AccountServerShard *shard, Packet *pak_in);
static void handleClientCountByShardRequest(AccountServerShard *shard, Packet *pak_in);
static void handleClientCountByShardReply(AccountServerShard *shard, Packet *pak_in);
static void handleClientRegisterAccount(AccountServerShard *shard, Packet *pak_in);
static void handleClientLogoutAccount(AccountServerShard *shard, Packet *pak_in);
static void handleClientProductCatalogRequest(AccountServerShard *shard, Packet *pak_in);
static void handleClientActiveAccountRequest(AccountServerShard *shard, Packet *pak_in);
static void handleGetPlayNCAuthKey(AccountServerShard *shard, Packet *packet_in);
static void handleMarkSaved(AccountServerShard *shard, Packet *pak_in);
static void handleRecoverUnsaved(AccountServerShard *shard, Packet *pak_in);

static Queue s_delayedPackets;
static U32 s_delayedTickId;
static CLinkedList<Account*> s_accountsToReAuth;

AccountServerShard* AccountServer_GetShard(U8 shard_id)
{
    if (!devassert(shard_id < eaSize(&g_accountServerState.connectedShardsById)))
        return NULL;

    return g_accountServerState.connectedShardsById[shard_id];
}

AccountServerShard* AccountServer_GetShardByName(const char *name)
{
    if (devassert(name && g_accountServerState.connectedShardsByName))
    {
        AccountServerShard *shard;
        if (stashFindPointer(g_accountServerState.connectedShardsByName, name, reinterpret_cast<void**>(&shard)))
        {
            return shard;
        }
    }
    return NULL;
}

bool AccountServer_IsCompMode(void)
{
    return g_accountServerState.cfg.comp_mode;
}

bool AccountServer_UseXferToken(Account* account)
{
    if (g_accountServerState.cfg.bFreeShardXfersOnlyEnabled)
    {
        int remainingXfers = updateShardXferTokenCount(account);
        if (remainingXfers)
        {
            //    push a new one with the current date
            eaiPush(&account->shardXferTokens, timerSecondsSince2000());
            return true;
        }
    }
    return false;
}

static INLINEDBG int Account_reauth_cmp(const Account &a,const Account &b)
{
    return a.reauthTime - b.reauthTime;
}

static INLINEDBG int Account_reauth_cmp(const CLink<Account*> &a,const CLink<Account*> &b)
{
    return Account::reauthLinkCompare(a, b, Account_reauth_cmp);
}

static Account *recvAccountData(AccountServerShard *shard, Packet *pak)
{
    U32 auth_id = pktGetBitsAuto(pak);
    const char * auth_name = pktGetString(pak);

    Account *account = AccountDb_LoadOrCreateAccount(auth_id, auth_name);
    assert(account);

    // boot the account from the old shard
    if (account->shard && account->shard != shard && account->shard->link.connected)
    {
        Packet *pak = pktCreateEx(&account->shard->link, ACCOUNT_SVR_FORCE_LOGOUT);
        pktSendBitsAuto(pak, auth_id);
        pktSend(&pak, &account->shard->link);
    }

    account->shard = shard;

    // Request an initial inventory update
    AccountDb_MarkUpdate(account, ACCOUNTDB_UPDATE_NETWORK);
    return account;
}

static void recvAllAccountData(AccountServerShard *shard, Packet *pak)
{
    int i;
    int count = pktGetBitsAuto(pak);
    U32 now = timerSecondsSince2000();
    
    for (i = 0; i < count; i++)
    {
        Account *account = recvAccountData(shard, pak);

        // spread the load out over a period of time
        if (account->reauthLink.linked())
            account->reauthLink.unlink();

        account->reauthTime = now + (count - i) / SHARD_RECONNECT_REAUTH_PLAYERS_PER_SECOND;
        s_accountsToReAuth.addSortedNearHead(account->reauthLink, Account_reauth_cmp);
    }
}

static void retryConnect(NetLink *link, AccountServerShard *shard)
{
}

int shardMsgCallback(Packet *pak, int cmd, NetLink *link)
{
    AccountServerState *state = &g_accountServerState;
    int cursor = bsGetCursorBitPosition(&pak->stream);
    AccountServerShard *shard = reinterpret_cast<AccountServerShard*>(link->userData);
    assert(shard);

    // The retransmit queue flag is overriden for our delayed packet queue
    assert(!pak->inRetransmitQueue);

    g_packetcount++;
    link->last_update_time = timerCpuSeconds();

    // This forces data to go back to the dbserver a little more smoothly
    // when there's a huge number of incoming packets.
    if(timerElapsed(g_flush_timer) > MAX_FLUSH_TIME)
    {
        lnkFlushAll();
        timerStart(g_flush_timer);
    }

    if(!shard->name && cmd != ACCOUNT_CLIENT_CONNECT)
    {
        printf_stderr("received cmd from dbserver at %s (%i) before shard connect\n", shard->ip, cmd);
        netSendDisconnect(link,0);
        return 1;
    }

    switch ( cmd )
    {
    case ACCOUNT_CLIENT_CONNECT:
    {
        int version = pktGetBitsAuto(pak);
        char *shardname = pktGetString(pak);
        if(version != DBSERVER_ACCOUNT_PROTOCOL_VERSION)
        {
            printf_stderr("dbserver at %s needs to be upgraded, rechecking version in %is. (got %i, need %i)\n",
                            shard->ip, SHARDRECONNECT_RETRY_TIME, version, DBSERVER_ACCOUNT_PROTOCOL_VERSION);
            netSendDisconnect(link,0);
            break;
        }

        if(shard->name)
        {
            if(strcmp(shard->name,shardname))
            {
                printf_stderr("dbserver at %s gave a name that doesn't match config (got %s, have %s).\n",
                            shard->ip, shardname, shard->name);
                netSendDisconnect(link,0);
                break;
            }
        }
        else if(stashFindPointer(state->connectedShardsByName,shardname,NULL))
        {
            printf_stderr("dbserver at %s has the same name as a shard that is already connected (%s), dropping the new connection.\n",
                shard->ip, shardname);
            netSendDisconnect(link,0);
            break;
        }
        else
        {
            shard->name = allocAddString(shardname);
        }
        stashAddPointer(state->connectedShardsByName,shard->name,shard,true);
        state->connectedShardsById[shard->shard_id] = shard;

        recvAllAccountData(shard, pak);
        sendCatalogUpdatesToShard( shard );
    }
    xcase ACCOUNT_CLIENT_SLASHCMD:
    {
        int message_dbid, dbid;
        int auth_id;
        char *slashcmd = NULL;
        
        message_dbid = pktGetBitsAuto(pak);
        auth_id = pktGetBitsAuto(pak);
        dbid = pktGetBitsAuto(pak);
        slashcmd = pktGetString(pak);
        AccountServer_HandleSlashCmd(link, message_dbid, auth_id, dbid, slashcmd); 
    }
    xcase ACCOUNT_CLIENT_REGISTER_ACCOUNT:
    {
        handleClientRegisterAccount(shard,pak);
    }
    xcase ACCOUNT_CLIENT_LOGOUT_ACCOUNT:
    {
        handleClientLogoutAccount(shard,pak);
    }
    xcase ACCOUNT_CLIENT_CHARCOUNT_REQUEST:
    {
        handleClientCountByShardRequest(shard,pak);
    }
    xcase ACCOUNT_CLIENT_CHARCOUNT_REPLY:
    {
        handleClientCountByShardReply(shard,pak);
    }
    xcase ACCOUNT_CLIENT_SLOTCOUNT_REPLY:
    {
        AccountServer_RedeemedCountByShardReply(shard,pak);
    }
    xcase ACCOUNT_CLIENT_SHARDXFER:
    {
        handlePacketClientShardTransfer(pak);
    }
    xcase ACCOUNT_CLIENT_RENAME:
    {
        handlePacketClientRename(pak);
    }
    xcase ACCOUNT_CLIENT_SLOT:
    {
        handlePacketClientSlot(pak);
    }
    xcase ACCOUNT_CLIENT_SHARD_XFER_TOKEN_CLAIM:
    {
        handleShardXferTokenClaim(shard, pak);
    }
    xcase ACCOUNT_CLIENT_GENERIC_CLAIM:
    {
        handleGenericClaim(shard, pak);
    }
    xcase ACCOUNT_CLIENT_RESPEC:
    {
        handlePacketClientRespec(pak);
    }
    xcase ACCOUNT_CLIENT_INVENTORY_REQUEST:
    {
        handleInventoryRequest(shard, pak);
    }
    xcase ACCOUNT_CLIENT_INVENTORY_CHANGE:
    {
        handleInventoryChangeRequest(shard, pak);
    }
    xcase ACCOUNT_CLIENT_UPDATE_EMAIL_STATS:
    {
        handleUpdateEmailStats(shard, pak);
    }
    xcase ACCOUNT_CLIENT_CERTIFICATION_TEST:
        g_accountServerState.accountCertificationTestStage = pktGetBitsAuto(pak);
    xcase ACCOUNT_CLIENT_CERTIFICATION_GRANT:
    {
        handleCertificationGrant(shard, pak);
    }
    xcase ACCOUNT_CLIENT_CERTIFICATION_CLAIM:
    {
        handleCertificationClaim(shard, pak);
    }
    xcase ACCOUNT_CLIENT_CERTIFICATION_REFUND:
    {
        handleCertificationRefund(shard, pak);
    }
    xcase ACCOUNT_CLIENT_MULTI_GAME_TRANSACTION:
    {
        handleMultiGameTransaction(shard, pak);
    }
    xcase ACCOUNT_CLIENT_TRANSACTION_FINISH:
    {
        handleTransactionFinish(shard, pak);
    }
    xcase ACCOUNT_CLIENT_LOYALTY_CHANGE:
    {
        handleLoyaltyChangeRequest(shard, pak);
    }    
    xcase ACCOUNT_CLIENT_AUTH_UPDATE:
    {
        handleAuthUpdateRequest(shard, pak);
    }
    xcase ACCOUNT_CLIENT_SET_INVENTORY:
    {
        handleSetInventoryRequest(shard, pak);
    }
    xcase ACCOUNT_CLIENT_GET_PLAYNC_AUTH_KEY:
    {
        handleGetPlayNCAuthKey(shard, pak);
    }
    xcase ACCOUNT_CLIENT_MARK_SAVED:
    {
        handleMarkSaved(shard, pak);
    }
    xcase ACCOUNT_CLIENT_RECOVER_UNSAVED:
    {
        handleRecoverUnsaved(shard, pak);
    }
    xcase ACCOUNT_CLIENT_VISITORXFER:
    {
        TODO(); // No product code assigned for visitor transfers
#if 0
        Account *account;
        AccountRequest *request;
        AccountOrderType type;
        char authname[64];
        int dbid;
        int isOutBound;
        char *targetShard;

        strncpyt(authname, pktGetString(pak), sizeof(authname));
        dbid = pktGetBitsAuto(pak);
        isOutBound = pktGetBitsAuto(pak);
        targetShard = pktGetString(pak);
        type = isOutBound ? kAccountOrderType_VisitorXferOutbound : kAccountOrderType_VisitorXferReturn;

        if (authname[0] == 0)
        {
            break;
        }

        account = AccountDb_GetOrAddAccount(authname);
        devassert(account->shard == NULL || account->shard == shard);
        account->shard = shard;

        order = AccountDb_NewOrder(account, kProductId_CharTransfer, type, ACCOUNTORDER_NOCHARGE, shard->name, dbid, 0, targetShard, 0);
        AccountServer_AddPendingOrder(order);
#endif
    }
    xcase ACCOUNT_CLIENT_UPDATE_LOG_LEVELS:
    {
        int i;
        for( i = 0; i < LOG_COUNT; i++ )
        {
            int log_level = pktGetBitsPack(pak,1);
            logSetLevel((LogType)i, (LogLevel)log_level);
        }
    }
    xcase ACCOUNT_CLIENT_TEST_LOGS:
    {
        LOG( LOG_DEPRECATED, LOG_LEVEL_ALERT, LOG_CONSOLE, "AccountServer Log Line Test: Alert" );
        LOG( LOG_DEPRECATED, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "AccountServer Log Line Test: Important" );
        LOG( LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "AccountServer Log Line Test: Verbose" );
        LOG( LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, LOG_CONSOLE, "AccountServer Log Line Test: Deprecated" );
        LOG( LOG_DEPRECATED, LOG_LEVEL_DEBUG, LOG_CONSOLE, "AccountServer Log Line Test: Debug" );
    }
    xdefault:
        devassertmsg(0,"invalid switch value. %i\n",cmd);
    };

    if (pak->inRetransmitQueue)
    {
        // rewind packet
        bsSetCursorBitPosition(&pak->stream, cursor);
        // HACK: store the command in the ordered_id
        pak->ordered_id = cmd;
        return NETLINK_MONITOR_DO_NOT_FREE_PACKET;
    }

    return 1;
}

static void AccountServer_TickDelayedPackets()
{
    Packet *pak;

    // Start a new frame
    s_delayedTickId++;

    while (pak = (Packet*)qPeek(s_delayedPackets))
    {
        int cmd;
        int ret;

        // Consume packets until we find one that has gotten requeued
        // again with the current delayed tick id
        if (pak->retransCount == s_delayedTickId)
            break;

        // Clear the our overriden re-transmit status and try again
        assert(qDequeue(s_delayedPackets) == pak);
        pak->inRetransmitQueue = 0;

        // HACK: the command was stored in the ordered_id
        cmd = pak->ordered_id;

        // Re-dispatch the packet
        ret = shardMsgCallback(pak, cmd, pak->link);
        if (!pak->inRetransmitQueue)
            pktFree(pak);
        if (ret < 0)
            break;
    }
}

static void AccountServer_InitDelayedQueue()
{
    s_delayedPackets = createQueue();
    s_delayedTickId = 0;
}

bool AccountServer_DelayPacketUntilLater(AccountServerShard *shard, Packet *pak)
{
    pak->link = &shard->link;

    // Override the "retransmit queue" for our delayed packet queue
    pak->inRetransmitQueue = 1;
    pak->retransCount = s_delayedTickId;

    // Queue the packet in the delayed packet queue
    return qEnqueue(s_delayedPackets, pak) != 0;
}

void AccountServer_NotifyTransactionFinished(Account * account, const AccountProduct * product, OrderId order_id, int granted, int claimed, bool success)
{
    AccountRequest::OnTransactionCompleted(order_id, success);

    if (!SAFE_MEMBER(account->shard, link.connected))
        return;

    Packet *pak_out = pktCreateEx(&account->shard->link, ACCOUNT_SVR_NOTIFYTRANSACTION);
    pktSendBitsAuto(pak_out, account->auth_id);
    pktSendBitsAuto2(pak_out, product->sku_id.u64);
    pktSendBitsAuto(pak_out, product->invType);
    pktSendBitsAuto2(pak_out, order_id.u64[0]);
    pktSendBitsAuto2(pak_out, order_id.u64[1]);
    pktSendBitsAuto(pak_out, granted);
    pktSendBitsAuto(pak_out, claimed);
    pktSendBool(pak_out, success);
    pktSend(&pak_out, &account->shard->link);
}

void AccountServer_NotifyRequestFinished(Account * account, AccountRequestType reqType, OrderId order_id, bool success, const char *msg)
{
    if (!SAFE_MEMBER(account->shard, link.connected))
        return;

    Packet *pak_out = pktCreateEx(&account->shard->link, ACCOUNT_SVR_NOTIFYREQUEST);
    pktSendBitsAuto(pak_out, account->auth_id);
    pktSendBitsAuto(pak_out, reqType);
    pktSendBitsAuto2(pak_out, order_id.u64[0]);
    pktSendBitsAuto2(pak_out, order_id.u64[1]);
    pktSendBool(pak_out, success);
    pktSendString(pak_out, msg);
    pktSend(&pak_out, &account->shard->link);
}

void AccountServer_NotifySaved(Account * account, OrderId order_id, bool success)
{
    if (!SAFE_MEMBER(account->shard, link.connected))
        return;

    Packet *pak_out = pktCreateEx(&account->shard->link, ACCOUNT_SVR_NOTIFYSAVED);
    pktSendBitsAuto(pak_out, account->auth_id);
    pktSendBitsAuto2(pak_out, order_id.u64[0]);
    pktSendBitsAuto2(pak_out, order_id.u64[1]);
    pktSendBool(pak_out, success);
    pktSend(&pak_out, &account->shard->link);
}

void AccountServer_NotifyAccountFound(const char *search_string, U32 found_auth_id)
{
    AccountServer_SlashNotifyAccountFound(search_string, found_auth_id);
}

static void CatalogUpdate_Tick(void)
{
    if ( s_ProductsWithUpdatesToBroadcast != NULL )
    {
        StashTableIterator iter;
        StashElement elem;
        stashGetIterator( g_accountServerState.connectedShardsByName, &iter );
        while ( stashGetNextElement( &iter, &elem ) )
        {
            AccountServerShard *shard = (AccountServerShard*)stashElementGetPointer(elem);
            sendCatalogUpdatesToShard( shard );
        }
        stashTableDestroy(s_ProductsWithUpdatesToBroadcast);
        s_ProductsWithUpdatesToBroadcast = NULL;
    }
}

static void DBServer_Heartbeat(bool mtx_alive)
{
    int count;
    Packet *pkt;
    NetLink *link;

    for (count = 0; count < eaSize(&(g_accountServerState.shards)); count++)
    {
        link = &(g_accountServerState.shards[count]->link);
        if (link->connected)
        {
            pkt = pktCreateEx(link, ACCOUNT_SVR_DB_HEARTBEAT);
            pktSendBits(pkt, 1, mtx_alive);
            pktSend(&pkt, link);
        }
    }
}

// *********************************************************************************
//  game client stuff // TODO: move this somewhere else
// *********************************************************************************

static void handleUpdateEmailStats(AccountServerShard *shard, Packet *pak_in)
{
    Account * pAccount;
    U32 auth_id = pktGetBitsAuto(pak_in);
    U32 lastEmailTime = pktGetBitsAuto(pak_in);
    U32 lastNumEmailsSent = pktGetBitsAuto(pak_in);

    pAccount = AccountDb_GetAccount(auth_id);
    if (!devassert(pAccount))
        return;

    if (!AccountDb_IsDataLoaded(pAccount))
    {
        // try to queue this command for later
        if (AccountServer_DelayPacketUntilLater(shard, pak_in))
            return;
    }

    pAccount->lastEmailTime = lastEmailTime;
    pAccount->lastNumEmailsSent = lastNumEmailsSent;

    AccountDb_MarkUpdate(pAccount, ACCOUNTDB_UPDATE_SQL_ACCOUNT);
}

#define CLIENTCOUNTREPLY_TIMEOUT (10) // seconds

#define CLIENTCOUNT_INVALID -3
#define CLIENTCOUNT_WAITING -2
#define CLIENTCOUNT_NOCOUNT -1

typedef struct ClientAwaitingCounts
{
    U32 auth_id;
    AccountServerShard *shard;
    int *unlocked_used_counts;
    int *total_used_counts;
    int *account_extra_counts; // excludes VIP slots
    int *server_max_counts;
    int *vip_only_statuses;
    U32 requested;
} ClientAwaitingCounts;

MP_DEFINE(ClientAwaitingCounts);

static ClientAwaitingCounts **ppWaitingForCounts;
static StashTable stWaitingForCounts;

static void handleClientCountByShardRequest(AccountServerShard *shard, Packet *pak_in)
{
    int i;
    AccountServerState *state = &g_accountServerState;

    int auth_id = pktGetBitsAuto(pak_in);
    bool vip = pktGetBool(pak_in);
    ClientAwaitingCounts *client;

    if(shard->shardxfer_not_a_src || !eaSize(&shard->shardxfer_destinations))
    {
        Packet *pak_out = pktCreateEx(&shard->link,ACCOUNT_SVR_CHARCOUNT_REPLY);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto(pak_out,0);
        pktSendString(pak_out,"AccountClientInvalidSrc");
        pktSend(&pak_out,&shard->link);
        return;
    }

    MP_CREATE(ClientAwaitingCounts,64);
    client = MP_ALLOC(ClientAwaitingCounts);
    assert(client);

    if(!stWaitingForCounts)
        stWaitingForCounts = stashTableCreateInt(128);

    if(!stashIntAddPointer(stWaitingForCounts,auth_id,client,false))
    {
        MP_FREE(ClientAwaitingCounts,client);

        // if this is actually a second request for the same authname, redirect the reply
        if(stashIntFindPointer(stWaitingForCounts,auth_id,reinterpret_cast<void**>(&client)))
            client->shard = shard;

        return;
    }
    eaPush(&ppWaitingForCounts,client);

    client->auth_id = auth_id;
    client->shard = shard;
    client->unlocked_used_counts = static_cast<int*>(calloc(state->shard_count, sizeof(int)));
    client->total_used_counts = static_cast<int*>(calloc(state->shard_count, sizeof(int)));
    client->account_extra_counts = static_cast<int*>(calloc(state->shard_count, sizeof(int)));
    client->server_max_counts = static_cast<int*>(calloc(state->shard_count, sizeof(int)));
    client->vip_only_statuses = static_cast<int*>(calloc(state->shard_count, sizeof(int)));
    client->requested = timerSecondsSince2000();

    for(i = 0; i < state->shard_count; ++i)
    {
        client->unlocked_used_counts[i] = CLIENTCOUNT_INVALID;
        client->total_used_counts[i] = CLIENTCOUNT_INVALID;
    }

    for(i = eaSize(&shard->shardxfer_destinations)-1; i >= 0; --i)
    {
        AccountServerShard *shard_out = AccountServer_GetShardByName(shard->shardxfer_destinations[i]);
        int shard_idx = eaFind(&state->shards,shard_out);

        if(shard_out && shard_idx >= 0)
        {
            if(shard_out->shardxfer_not_a_dst || (isProductionMode() && shard_out == shard))
            {
                // client->counts[shard_idx] = CLIENTCOUNT_INVALID;
            }
            else if(shard_out->name && shard_out->link.connected)
            {
                Packet *pak_out = pktCreateEx(&shard_out->link,ACCOUNT_SVR_CHARCOUNT_REQUEST);
                pktSendBitsAuto(pak_out, auth_id);
                pktSendBool(pak_out, vip);
                pktSend(&pak_out,&shard_out->link);

                client->unlocked_used_counts[shard_idx] = CLIENTCOUNT_WAITING;
                client->total_used_counts[shard_idx] = CLIENTCOUNT_WAITING;
            }
            else
            {
                client->unlocked_used_counts[shard_idx] = CLIENTCOUNT_NOCOUNT;
                client->total_used_counts[shard_idx] = CLIENTCOUNT_NOCOUNT;
            }
        }
    }
}

static void handleClientCountByShardReply(AccountServerShard *shard, Packet *pak_in)
{
    AccountServerState *state = &g_accountServerState;
    int shard_idx = eaFind(&state->shards,shard);
    ClientAwaitingCounts *client;

    int auth_id = pktGetBitsAuto(pak_in);

    if(devassert(INRANGE0(shard_idx,state->shard_count)) && stashIntFindPointer(stWaitingForCounts,auth_id, reinterpret_cast<void**>(&client)))
    {
        int unlocked_used_count = pktGetBitsAuto(pak_in);
        int total_used_count = pktGetBitsAuto(pak_in);
        int account_extra_count = pktGetBitsAuto(pak_in); // excludes VIP slots
        int server_max_count = pktGetBitsAuto(pak_in);
        int vip_only_status = pktGetBitsAuto(pak_in);
        if(devassert(unlocked_used_count >= 0 && total_used_count >= 0 && account_extra_count >= 0 && server_max_count >= 0))
        {
            client->unlocked_used_counts[shard_idx] = unlocked_used_count;
            client->total_used_counts[shard_idx] = total_used_count;
            client->account_extra_counts[shard_idx] = account_extra_count;
            client->server_max_counts[shard_idx] = server_max_count;
            client->vip_only_statuses[shard_idx] = vip_only_status;
        }
    }
    else
    {
        LOG(LOG_ACCOUNT, LOG_LEVEL_VERBOSE, 0, "Got a slow character count response from %s for %d", shard?shard->name:"(null shard)", auth_id);
    }
}

static void clientCountByShardTick(void)
{
    AccountServerState *state = &g_accountServerState;
    int i;

    for(i = eaSize(&ppWaitingForCounts)-1; i >= 0; --i)
    {
        ClientAwaitingCounts *client = ppWaitingForCounts[i];
        int j, reply_shards = 0;
        bool invalid = true;

        for(j = 0; j < state->shard_count; ++j)
        {
            if(client->unlocked_used_counts[j] == CLIENTCOUNT_WAITING)
            {
                if(client->requested + CLIENTCOUNTREPLY_TIMEOUT < timerSecondsSince2000())
                {
                    client->unlocked_used_counts[j] = CLIENTCOUNT_NOCOUNT;
                    client->total_used_counts[j] = CLIENTCOUNT_NOCOUNT;
                }
                else
                    break;
            }

            if(client->unlocked_used_counts[j] == CLIENTCOUNT_NOCOUNT)
                invalid = false;

            if(client->unlocked_used_counts[j] >= 0)
                ++reply_shards;
        }

        if(j >= state->shard_count) // We've heard from all the shards
        {
            // if we still know who we're talking to, send the response
            if(client->shard->link.connected)
            {
                Packet *pak_out = pktCreateEx(&client->shard->link,ACCOUNT_SVR_CHARCOUNT_REPLY);
                pktSendBitsAuto(pak_out,client->auth_id);
                pktSendBitsAuto(pak_out,reply_shards);
                if(reply_shards)
                {
                    for(j = 0; j < state->shard_count; ++j)
                    {
                        if (client->unlocked_used_counts[j] >= 0 && client->total_used_counts[j] >= 0 &&
                            client->account_extra_counts[j] >= 0 && client->server_max_counts[j] >= 0)
                        {
                            pktSendString(pak_out,state->shards[j]->name);
                            pktSendBitsAuto(pak_out,client->unlocked_used_counts[j]);
                            pktSendBitsAuto(pak_out,client->total_used_counts[j]);
                            pktSendBitsAuto(pak_out,client->account_extra_counts[j]); // excludes VIP slots
                            pktSendBitsAuto(pak_out,client->server_max_counts[j]);
                            pktSendBitsAuto(pak_out,client->vip_only_statuses[j]);
                        }
                    }
                }
                else if(invalid)
                {
                    pktSendString(pak_out,"AccountClientInvalidSrc");
                }
                else
                {
                    pktSendString(pak_out,"AccountClientNoResponse");
                }
                pktSend(&pak_out,&client->shard->link);
            }

            // clean up
            stashIntRemovePointer(stWaitingForCounts,client->auth_id,NULL);
            eaRemoveFast(&ppWaitingForCounts,i);
            free(client->unlocked_used_counts);
            free(client->total_used_counts);
            free(client->account_extra_counts);
            free(client->server_max_counts);
            free(client->vip_only_statuses);
            MP_FREE(ClientAwaitingCounts,client);
        }
    }
}

#define kHexStringCharsPerU32 8
void MakeHexStringFromCryptoHash( const unsigned char* byteArray, int byteArrayLen, char* resultString, int resultStringBuffSz )
{
    //
    //    Make a hex string. Note that we're treating the hash as an array
    //    of bytes, rather than 4 U32's. So we knowingly ignore the endian
    //    issues which would apply if md5Key were treated as an array of 
    //    32-bit ints.
    //
    int i;
    int resultStrIndex = 0;
    for ( i=0; ( i < byteArrayLen ) && ( resultStrIndex < ( resultStringBuffSz - 2 )) ; i++ )
    {
        unsigned char keyByte = byteArray[i];
        unsigned char nibble;
            
        nibble = ( keyByte >> 4 );
        resultString[resultStrIndex+0] = ( nibble < 10 ) ? ( '0' + nibble ) : ( 'a' + nibble - 10 );
        nibble = ( keyByte & 0x0F );
        resultString[resultStrIndex+1] = ( nibble < 10 ) ? ( '0' + nibble ) : ( 'a' + nibble - 10 );
        resultStrIndex += 2;
    }
    resultString[resultStrIndex] = '\0';
}

// NOTE: Returned pointer is only good for one call at a time!
#define kMD5_U32_ArraySz 4
static char* MakeHexStringFromMD5( U32 md5Key[kMD5_U32_ArraySz] )
{
    static const int nBufferSize = ( ( kMD5_U32_ArraySz * kHexStringCharsPerU32 ) + 1 );
    static char resultBuffer[nBufferSize];
    MakeHexStringFromCryptoHash( (unsigned char*)md5Key, (sizeof(U32) * kMD5_U32_ArraySz), resultBuffer, nBufferSize );
    return resultBuffer;
}

#define kSHA1_U32_ArraySz 5
#if 0
static char* MakeHexStringFromSHA1( U32 sha1Key[kSHA1_U32_ArraySz] )
{
    static const int nBufferSize = ( ( kSHA1_U32_ArraySz * kHexStringCharsPerU32 ) + 1 );
    static char resultBuffer[nBufferSize];
    MakeHexStringFromCryptoHash( (unsigned char*)sha1Key, (sizeof(U32) * kSHA1_U32_ArraySz), resultBuffer, nBufferSize );
    return resultBuffer;
}
#endif

static char* MakePlaySpanAuthDigest_MD5_NoTimestamp( const char* secretKey, const char* mtxEnvironment, U32 auth_id )
{
    // MD5(userid + NCSoft secretekey)
    char rawBuffer[ 512 ], dottedAuth[128];
    int dataSz;
    U32 md5Key[4];

    // first create lowercase accountserver.auth (client will do the same, to avoid mixed case issues)
    dataSz = sprintf_s( dottedAuth, ARRAY_SIZE(dottedAuth), "%s.%u", mtxEnvironment, auth_id );

    // now create the digest using the shared key
    dataSz = sprintf_s( rawBuffer, ARRAY_SIZE(rawBuffer), "%s%s", dottedAuth, secretKey );

    cryptMD5Init();
    cryptMD5Update( (U8*)rawBuffer, dataSz );
        
    cryptMD5Final(md5Key);

    return MakeHexStringFromMD5( md5Key );
}

static char* MakePlaySpanAuthDigest_SHA1_WithTimestamp( const char* secretKey, const char* mtxEnvironment, U32 auth_id, U32 unixStyleTimeVal)
{
    static char * base64Str = NULL;
    static char digestBuff[512];
    static HMAC_SHA1_Handle    s_playspan_hmac = NULL;

    U32 hash[kSHA1_U32_ArraySz + 1];
    
    char dottedAuth[128];
    char timeStampBuff[80];
    const char* partnerAccessID = "NCSF";

    sprintf_s( dottedAuth, ARRAY_SIZE(dottedAuth), "%s.%u", mtxEnvironment, auth_id );
    // PlaySpan wants a string which is the time x 1000 so we just add three '0' characters to the end.
    sprintf_s( timeStampBuff, ARRAY_SIZE(timeStampBuff), "%u000", unixStyleTimeVal );

    // Create the HMAC object if this is the first time we are computing a digest
    // ** N.B.: this routine ignores the secret key argument after the first time through. **
    // Thus key changes from config file reloads will not be handled correctly.
    // To do this efficiently requires more refactoring than desireable for a release candidate.
    if (!s_playspan_hmac)
    {
        s_playspan_hmac = cryptHMAC_SHA1Create( (U8*)secretKey, (int)strlen(secretKey) );
    }
    cryptHMAC_SHA1Update( s_playspan_hmac, (U8*)dottedAuth,   (int)strlen(dottedAuth) );
    cryptHMAC_SHA1Update( s_playspan_hmac, (U8*)timeStampBuff, (int)strlen(timeStampBuff) );
    cryptHMAC_SHA1Update( s_playspan_hmac, (U8*)partnerAccessID, (int)strlen(partnerAccessID) );

    memset(hash, 0, sizeof(hash)); // see comment below
    cryptHMAC_SHA1Final(  s_playspan_hmac, hash);

    base64_encode( &base64Str, (U8*)hash, ( kSHA1_U32_ArraySz * sizeof(U32) ) );
    // There is an apparent bug in base64_encode() with this 20 byte hash. It pulls in
    // bytes beyond the end of the source buffer. So we make hash[] one element larger
    // and initialize it to zero so random unitialized bytes don't get into our string.
    hash[kSHA1_U32_ArraySz] = 0;

#define PRINT_DIGEST_TO_CONSOLE 0
#if PRINT_DIGEST_TO_CONSOLE
    printf("Creating playSpanAuthDigest:\n\ttimestamp %s\n\tpartnerAccessID %s\n\tauth %s\n\tkey: %s\n", timeStampBuff, partnerAccessID, dottedAuth, secretKey);
    printf("==> %s (before escaping)\n", base64Str);
#endif
    
    {
        // Make it URL friendly
        int base64I=0;
        int digestI=0;
        for ( base64I=0; digestI < (ARRAY_SIZE(digestBuff)-4); base64I++ )
        {
            if ( base64Str[base64I] == '\0' )
            {
                digestBuff[digestI] = '\0';
                break;
            }
            #if 1    // comment out if we don't need tp replace special characters with HTTP reps
                else if (( base64Str[base64I] < 'A' ) &&
                         ((  base64Str[base64I] < '0' ) || (  base64Str[base64I] > '9' )))
                {
                    sprintf_s(( digestBuff + digestI ), 4, "%%%02X", base64Str[base64I] );
                    digestI += 3;
                }
            #endif
            else
            {
                digestBuff[digestI++] = base64Str[base64I];
            }
        }
    } 
#if PRINT_DIGEST_TO_CONSOLE
    printf("==> %s (after escaping)\n", digestBuff);
#endif

    return digestBuff;
}

static char* MakePlaySpanAuthDigest( const char* secretKey, const char* mtxEnvironment, U32 auth_id, U32 unixStyleTimeVal)
{
    if ( unixStyleTimeVal != 0 )
    {
        return MakePlaySpanAuthDigest_SHA1_WithTimestamp( secretKey, mtxEnvironment, auth_id, unixStyleTimeVal );
    }
    else
    {
        return MakePlaySpanAuthDigest_MD5_NoTimestamp( secretKey, mtxEnvironment, auth_id );
    }
}


// NOTE: Returned pointer is only good for one call at a time!
#define kSHA1_U32_ArraySz 5
static char* MakePlayNCAuthKey( const char* secretKey, const char* auth_name, const char* digest_data )
{
    static char * base64Str = NULL; 
    static HMAC_SHA1_Handle    s_plaync_hmac = NULL;
    U32 hash[kSHA1_U32_ArraySz + 1];
    
    // Create the HMAC object if this is the first time we are computing a digest
    // ** N.B.: this routine ignores the secret key argument after the first time through. **
    // Thus key changes from config file reloads will not be handled correctly.
    // To do this efficiently requires more refactoring than desireable for a release candidate.
    if (!s_plaync_hmac)
    {
        s_plaync_hmac = cryptHMAC_SHA1Create( (U8*)secretKey, (int)strlen(secretKey) );
    }
    // Calculate the digest
    cryptHMAC_SHA1Update( s_plaync_hmac, (U8*)auth_name,   (int)strlen(auth_name) );
    cryptHMAC_SHA1Update( s_plaync_hmac, (U8*)digest_data, (int)strlen(digest_data) );
    cryptHMAC_SHA1Final(  s_plaync_hmac, hash);
    // There is an apparent bug in base64_encode() with this 20 byte hash. It pulls in
    // bytes beyond the end of the source buffer. So we make hash[] one element larger
    // and initialize it to zero so random unitialized bytes don't get into our string.
    hash[kSHA1_U32_ArraySz] = 0;
    
    base64_encode( &base64Str, (U8*)hash, ( kSHA1_U32_ArraySz * sizeof(U32) ) );    
    
    return base64Str;
}

static void sendClientAuth(AccountServerShard *shard, Account *account)
{
    U32 digestTimeStamp = 
                ( g_accountServerState.cfg.playSpanMakeTimeStampedDigest ) ? 
                        timerMakeTimeFromSecondsSince2000( timerSecondsSince2000() + (g_accountServerState.cfg.playSpanAuthTimeoutMins * 60) ) : 0;

    const char *playSpanAuthDigest = 
                MakePlaySpanAuthDigest(
                        g_accountServerState.cfg.mtxSecretKey, 
                        g_accountServerState.cfg.mtxEnvironment, 
                        account->auth_id,
                        digestTimeStamp );
    Packet *pak;

    pak = pktCreateEx(&shard->link, ACCOUNT_SVR_CLIENTAUTH);
    pktSendBitsAuto(pak, account->auth_id);
    pktSendBitsAuto(pak, digestTimeStamp );
    pktSendString(pak, playSpanAuthDigest);
    pktSend(&pak, &shard->link);

    //printf("sendClientAuth: digestTimeStamp = %u (%d mins), mtx.authID = %s.%u, digest = %s (SK %s)\n", digestTimeStamp, g_accountServerState.cfg.playSpanAuthTimeoutMins, 
    //    g_accountServerState.cfg.mtxEnvironment, account->auth_id, playSpanAuthDigest, g_accountServerState.cfg.mtxSecretKey);

    account->authDigestSent = timerSecondsSince2000();

    if (account->reauthLink.linked())
        account->reauthLink.unlink();
    
    account->reauthTime = timerSecondsSince2000() + g_accountServerState.cfg.playSpanAuthRekeyIntervalMins * 60;

    s_accountsToReAuth.addSortedNearTail(account->reauthLink, Account_reauth_cmp);
}

static void clientAuthTick()
{
    U32 now = timerSecondsSince2000();

    FOREACH_LINK_SAFE(Account, account, next, s_accountsToReAuth, reauthLink)
    {
        if (now < account->reauthTime)
            break;

        if (SAFE_MEMBER(account->shard, link.connected))
            sendClientAuth(account->shard, account);
        else
            account->reauthLink.unlink();
    }
}

void sendCatalogUpdatesToShard(AccountServerShard *shard )
{
    Packet *pak = pktCreateEx(&shard->link, ACCOUNT_SVR_PRODUCT_CATALOG_UPDATE);
    accountCatalog_AddAcctServerCatalogToPacket( pak );
    pktSend(&pak, &shard->link);
}

static void handleClientRegisterAccount(AccountServerShard *shard, Packet *pak_in)
{
    Account *account = recvAccountData(shard, pak_in);
    sendClientAuth(shard, account);
}

static void handleClientLogoutAccount(AccountServerShard *shard, Packet *pak_in)
{
    Account * pAccount;
    U32 auth_id = pktGetBitsAuto(pak_in);

    pAccount = AccountDb_GetAccount(auth_id);
    if (!devassert(pAccount))
        return;
    if (!devassert(pAccount->shard == shard))
        return;

    pAccount->shard = NULL;
}

// ACCOUNT_CLIENT_GET_PLAYNC_AUTH_KEY
void handleGetPlayNCAuthKey(AccountServerShard *shard, Packet *packet_in)
{
    U32 auth_id;
    int request_key;
    char* auth_name;
    char* digest_data;
    char* computedAuthKey;
    Account *pAccount;
    Packet *packet_out;

    auth_id = pktGetBitsAuto(packet_in);
    request_key = pktGetBitsAuto(packet_in);
    strdup_alloca(auth_name,pktGetString(packet_in));
    strdup_alloca(digest_data,pktGetString(packet_in));
    
    pAccount = AccountDb_GetAccount(auth_id);
    if (!devassert(pAccount))
        return;
    if (!devassert(pAccount->shard == shard))
        return;
    if ( stricmp( pAccount->auth_name, auth_name) != 0 )
        // Asking for an auth key for someone else???
        return;
    
    //
    //    Compute the authkey....
    //
    computedAuthKey = MakePlayNCAuthKey( 
                            "Test", 
                            auth_name, 
                            digest_data );
    
    packet_out = pktCreateEx(&shard->link, ACCOUNT_SVR_PLAYNC_AUTH_KEY );
    pktSendBitsAuto(packet_out,auth_id);
    pktSendBitsAuto(packet_out,request_key);
    pktSendString(packet_out,computedAuthKey);
    pktSend(&packet_out,&shard->link);
}

void handleMarkSaved(AccountServerShard *shard, Packet *pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);

    Account *pAccount = AccountDb_GetAccount(auth_id);
    if (!devassert(pAccount))
        return;

    int size = pktGetBitsAuto(pak_in);
    for (int i=0; i<size; i++)
    {
        OrderId order_id = { pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in) };
        if (!devassert(!orderIdIsNull(order_id)))
            continue;
        Transaction_GameSave(pAccount, order_id);
    }
}

void handleRecoverUnsaved(AccountServerShard *shard, Packet *pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);
    U32 ent_id = pktGetBitsAuto(pak_in);

    Account *pAccount = AccountDb_GetAccount(auth_id);
    if (!devassert(pAccount))
        return;
    if (!devassert(pAccount->shard == shard))
        return;

    Transaction_GameRecoverUnsaved(pAccount, shard->shard_id, ent_id);
}

void AccountServer_QueueProdForUpdateBroadcast( SkuId sku_id )
{
    if ( s_ProductsWithUpdatesToBroadcast == NULL )
    {
        s_ProductsWithUpdatesToBroadcast = stashTableCreateSkuIndex(8);
    }
    stashAddInt( s_ProductsWithUpdatesToBroadcast, sku_id.c, 1, true );
}


// *********************************************************************************
//  main loop
// *********************************************************************************

U32        g_tickCount;

void UpdateConsoleTitle(void)
{
    char buf[2000];
    char const *mtx_stats = "NOMTX";
    int i;
    int nConnected = 0;

    for(i = 0; i < g_accountServerState.shard_count; ++i)
        if(g_accountServerState.shards[i]->link.connected)
            ++nConnected;
    
    snprintf(buf, ARRAY_SIZE(buf), "%i: AccountServer. %i shards %i accounts (PAK R:%d D:%d) (SQL Q:%d) %s (%i).",
        _getpid(), nConnected, AccountDb_GetCount(), g_packetcount, qGetSize(s_delayedPackets), asql_in_flight(), mtx_stats, ((g_tickCount / ACCOUNTSERVER_TICK_FREQ) % 10));
    setConsoleTitle(buf);

    g_packetcount = 0;
}

bool accountSvrCfgLoad(AccountServerCfg *cfg)
{
    bool loadResult = false;
     char fnbuf[MAX_PATH];
     char *fn = fileLocateRead("server/db/account_server.cfg",fnbuf);
    StructInit(cfg,sizeof(*cfg),parse_AccountServerCfg);
    accountCatalog_ReleaseStoreAccessInfo();
    loadResult = (( fn != NULL ) && ( ParserLoadFiles(NULL,fn,NULL,0,parse_AccountServerCfg,cfg,NULL,NULL,NULL) ));
    if ( loadResult )
    {
        AccountStoreAccessInfo*    info = cpp_const_cast(AccountStoreAccessInfo*)(accountCatalog_GetStoreAccessInfo());
        
        accountCatalog_SetTimeStampTestOffsetSecs( DAYS_TO_SECS( cfg->catalogTimeStampTestOffsetDays ) );

        // Force MtxEnvironment names to be uppercase
        strupr(g_accountServerState.cfg.mtxEnvironment);
        accountCatalog_SetMtxEnvironment( cfg->mtxEnvironment );
        
        info->playSpanCatalog            = _strdup( cfg->playSpanCatalog );
        info->playSpanStoreFlags        = cfg->playSpanStoreFlags;
        info->playSpanStoreFlags        |= cfg->auto_buy_products ? STOREFLAG_AUTO_BUY_PRODUCTS : 0;
    }
    return loadResult;
}

void shardNetConnectTick(AccountServerState *state)
{
    AccountServerCfg *cfg = &state->cfg;
    U32 now = timerSecondsSince2000();
    int i;

    // first alloc shards/links if need be
    if(!state->shards)
    {
        int maxShardId = 0;
        state->shards = cfg->shards;
        state->shard_count = eaSize(&state->shards);
        assert(state->shard_count); // this should have already failed during config load
        state->connectedShardsByName = stashTableCreateWithStringKeys(32, StashDefault);

        for(i = 0; i < state->shard_count; ++i)
        {
            maxShardId = MAX(maxShardId, state->shards[i]->shard_id);

            initNetLink(&state->shards[i]->link);
        }

        eaSetSize(&state->connectedShardsById, maxShardId+1);
    }

    // connect to any disconnected shards
    for(i = 0; i < state->shard_count; ++i)
    {
        AccountServerShard *shard = state->shards[i];
        NetLink *link = &shard->link;

        // if the link is down, try to reconnect if the last attempt on that shard was good or
        // the last failed attempt was a while ago
        if( !link->connected && shard->ip)
        {            
            if (!link->pending)
            {
                if (shard->last_connect + SHARDRECONNECT_RETRY_TIME > now)
                    continue;

                printf_stderr("Asynchronously connecting to dbserver at %s\n", shard->ip);

                char *ipstr = makeIpStr(ipFromString(shard->ip));
                int result = netConnectAsync(link, ipstr, DEFAULT_ACCOUNTSERVER_PORT, NLT_TCP, 20.f);
                if (!result)
                    printf_stderr("Failed to connect to dbserver at %s. Retrying in %is\n", shard->ip, SHARDRECONNECT_RETRY_TIME);

                shard->last_connect = now;
            }
            else
            {
                shard->last_connect = now;

                switch (netConnectPoll(link))
                {
                case 1:
                    {
                        Packet *pak = pktCreateEx(link, ACCOUNT_SVR_CONNECT); 
                        pktSend(&pak,link);

                        link->userData = shard;

                        printf_stderr("Connected to dbserver at %s\n", shard->ip);
                        NMAddLink(link, shardMsgCallback);
                    }
                    break;
                case -1:
                    printf_stderr("Failed to connect to dbserver at %s. Retrying in %is\n", shard->ip, SHARDRECONNECT_RETRY_TIME);
                    break;
                case 0:
                default:
                    // nothing to do
                    break;
                }
            }
        }
    }
}

static void memPrompt(void) { printf("Type 'DUMP' to continue: "); }
static void memDump(char* param) { if (strcmp(param, "DUMP")==0) memCheckDumpAllocs(); }

static void conShowShards(void)
{
    int i;
    U32 now = timerCpuSeconds();

    for(i = 0; i < g_accountServerState.shard_count; ++i)
    {
        AccountServerShard *shard = g_accountServerState.shards[i];
        if(!devassert(shard))
            continue;

        if(shard->link.connected)
        {
            printf("shard %s(%s): connected, silent %is\n",
                shard->name, shard->ip, now - shard->link.last_update_time);
        }
        else
        {
            printf("shard %s(%s): disconnected\n", shard->name, shard->ip);
        }
    }
}

static void toggleVerboseInfo(void)
{
    g_bVerbose = !g_bVerbose;

    printf( "verbose: %s", g_bVerbose ? "on\n" : "off\n");
}

static void validateHeaps()
{
    heapValidateAll();
}

static void reloadConfig();
static void reconfig(void) {
    int i;

    reloadConfig();

    for(i = 0; i < g_accountServerState.shard_count; ++i)
        if(g_accountServerState.shards[i]->link.connected)
            sendCatalogUpdatesToShard(g_accountServerState.shards[i]);
}

void takeADump(void)
{
    assertWriteFullDumpSimpleSetFlags(MiniDumpWithProcessThreadData);
}

ConsoleDebugMenu accountserverdebug[] =
{
    { 0,        "General commands:",            NULL,                    NULL        },
    { 'r',        "reload account config",        reconfig,                NULL        },
    { 'M',        "dump memory table",            memPrompt,                memDump        },
    { 'd',        "list connected shards",        conShowShards,            NULL        },
    { 'm',        "print memory usage",            memMonitorDisplayStats,    NULL        },
    { 'v',        "toggle verbose info",            toggleVerboseInfo,        NULL        },
    { '1',        "immediately create full dump with heap",    takeADump,    NULL        },
    { 'H',        "validate heaps",                validateHeaps,            NULL,        },

    { 0,        "Save commands:",                NULL,                    NULL        },

    { 0, NULL, NULL, NULL },
}; 

static char g_cmdline[1024];
static bool s_exiting = false;

static void __cdecl at_exit_func(void)
{
    printf_stderr("Quitting: %s\n",g_cmdline);
    logShutdownAndWait();
}

static BOOL s_CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
        case CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_C_EVENT:
            LOG( LOG_ACCOUNT, LOG_LEVEL_VERBOSE, 0, "Control Event %i",fdwCtrlType);
            printf_stderr("Waiting to exit until it is safe to do so\n");
            s_exiting = true;
            return TRUE;
    }
    return FALSE;
}

void reloadConfig(void)
{
    int i;

    if(!accountSvrCfgLoad(&g_accountServerState.cfg))
    {
        if(!isDevelopmentMode())
        {
            FatalErrorf("error loading account config");
        }
        else
        {
            AccountServerShard *shard = reinterpret_cast<AccountServerShard*>(StructAllocRaw(sizeof(AccountServerShard)));

            printf("no config specified. in development mode. adding localhost\n");

            shard->shard_id = 0xFF;
            shard->ip = "localhost";
            eaPush(&g_accountServerState.cfg.shards,shard);
        }
    }

    for (i=0; i<eaSize(&g_accountServerState.cfg.shards); i++)
    {
        AccountServerShard *shard = g_accountServerState.cfg.shards[i];
        if (!shard->shard_id)
        {
            FatalErrorf("Shard %s has no ShardId set\n", shard->name);
        }
    }

    if (!g_accountServerState.cfg.sqlDbName[0] || stricmp(g_accountServerState.cfg.sqlDbName,"master")==0)
    {
        FatalErrorf("AccountServer cannot use 'master' as your SQL database.");
    }

    printf_stderr("Listening for MicroTransactions in the environment '%s'\n", g_accountServerState.cfg.mtxEnvironment);
}

static void stress_test()
{
    const AccountProduct** products = accountCatalogGetEnabledProducts();

    while (true) {
        unsigned i;

        for (i=0; i<10000; i++) {
            int auth_id = randInt(INT_MAX-1)+1;
            char auth_name[32];
            Account * account;

            asql_tick();

            sprintf(auth_name, "stest_%x", auth_id);
            account = AccountDb_LoadOrCreateAccount(auth_id, auth_name);            

            if (!(randInt(4))) {
                while (!account->asql)
                    asql_wait_for_tick();

                memset(account->asql->loyalty_bits, randInt(UCHAR_MAX), LOYALTY_BITS/8);                
                int spent = accountLoyaltyRewardGetNodesBought(account->asql->loyalty_bits);
                account->loyaltyPointsEarned = randInt(UCHAR_MAX) + spent;
                account->loyaltyPointsUnspent = account->loyaltyPointsEarned - spent;
                AccountDb_CommitAsync(account);
            }

            if (randInt(3)) {
                while (!account->asql)
                    asql_wait_for_tick();

                const AccountProduct * product = products[randInt(eaSize(&products))];
                AccountInventory *inv = AccountInventorySet_Find(&account->invSet, product->sku_id);

                int count = randInt(10)+1;
                if (product->grantLimit) {
                    count = MIN(count, product->grantLimit - SAFE_MEMBER(inv, granted_total));
                    if (!count)
                        continue;
                }
                
                Transaction_GamePurchase(account, product, randU64(UCHAR_MAX), randU64(UINT_MAX), count, true);

                OrderId order_id = Transaction_GameClaim(account, product, randInt(UCHAR_MAX), randU64(UINT_MAX), count, true);
                if (randInt(3))
                    Transaction_GameSave(account, order_id);
                else
                    Transaction_GameRevert(account, order_id);
            }
        }
    }
}

int main(int argc,char **argv)
{
    int i;
    int timer;
    int send_timer;
    int startup_timer;
    bool do_stress_test = false;
    
    memCheckInit();

    startup_timer = timerAlloc();

    strcpy_s(SAFESTR(g_accountServerState.exe_name),argv[0]);
    strcpy(g_cmdline, GetCommandLineA());
    
    EXCEPTION_HANDLER_BEGIN 

    setAssertMode(ASSERTMODE_FULLDUMP | ASSERTMODE_DEBUGBUTTONS);
    FatalErrorfSetCallback(NULL); // assert, log, and exit

    cryptInit();
    for(i = 0; i < argc; i++){
        printf("%s \n", argv[i]);
    }
    printf("\n");

    atexit( at_exit_func );
    SetConsoleCtrlHandler((PHANDLER_ROUTINE) s_CtrlHandler, TRUE); // add to list

    setWindowIconColoredLetter(compatibleGetConsoleWindow(), '%', 0xffff00);
    FolderCacheChooseMode();
    preloadDLLs(0);

    if (fileIsUsingDevData()) { 
        bsAssertOnErrors(true);
        setAssertMode(ASSERTMODE_DEBUGBUTTONS |
            (!IsDebuggerPresent()? ASSERTMODE_MINIDUMP : 0));
    } else {
        // In production mode on the servers we want to save all dumps and do full dumps
        setAssertMode(ASSERTMODE_DEBUGBUTTONS |
            ASSERTMODE_FULLDUMP |
            ASSERTMODE_DATEDMINIDUMPS | ASSERTMODE_ZIPPED);
    }

    srand((unsigned int)time(NULL));
    consoleInit(110, 128, 0);
    UpdateConsoleTitle();
    sockStart();
    packetStartup(0,0);

    for(i=1;i<argc;i++)
    {
        if (strcmp(argv[i], "-nolastauthor")==0)
            g_disableLastAuthor = 1;
        else if (strcmp(argv[i], "-stresstest")==0)
            do_stress_test = true;
        else
            printf("-----INVALID PARAMETER: %s\n", argv[i]);
    }

    logSetDir("accountserver");
    logSetUseTimestamped(true);
    //logSetLevel(LOG_ACCOUNT, LOG_LEVEL_VERBOSE);
    //logSetLevel(LOG_TRANSACTION, LOG_LEVEL_VERBOSE);

    errorLogStart();
    LOG( LOG_ACCOUNT, LOG_LEVEL_VERBOSE, 0, "starting %s", GetCommandLine());

    // ensure only one accountserver
    loadstart_printf("Checking for other copies of AccountServer...");
    HANDLE mx = CreateMutexA(NULL, FALSE, "Global\\coh_accountserver_mutex");
    if(!mx || WAIT_TIMEOUT == WaitForSingleObject(mx, 0))
        FatalErrorf("Another AccountServer is already running");
    loadend_printf("done");

    loadstart_printf("Networking startup...");
    timer = timerAlloc(); 
    timerStart(timer);
    packetStartup(0,0);
    loadend_printf("");

    loadstart_printf("loading config...");
    reloadConfig();
    loadend_printf("");

    // product ID to generic category stuff - will load product defs
    // here when we need them
    loadstart_printf("loading product catalog and mapping..");
    accountCatalogInit();
    accountLoyaltyRewardTreeLoad();
    loadend_printf("");

    AccountServer_SlashInit();
    asql_open();
    asql_sync_products();
    accountInventory_Init();
    AccountDb_Init();
    Transaction_Init();
    AccountRequest::Init();
    AccountServer_InitDelayedQueue();

    printfColor(COLOR_RED|COLOR_GREEN|COLOR_BLUE | COLOR_BRIGHT, "AccountServer ready.");
    printf(" (took %f seconds)\n\n",timerElapsed(startup_timer));

    send_timer = timerAlloc();
    g_flush_timer = timerAlloc();
    g_activeAccount_timer = timerAlloc();

    if (do_stress_test)
        stress_test();

    while (!s_exiting)
    {
        timerStart(g_flush_timer);

        shardNetConnectTick(&g_accountServerState);
        NMMonitor(10);
        asql_tick();

        if (timerElapsed(timer) > (1.f / ACCOUNTSERVER_TICK_FREQ))
        {
            timerStart(timer);
            g_tickCount++;

            DoConsoleDebugMenu(accountserverdebug);

            AccountServer_TickDelayedPackets();
            AccountRequest::Tick();
            AccountDb_Tick();
            CatalogUpdate_Tick();
            clientCountByShardTick();
            AccountServer_RedeemedCountByShardTick();
            clientAuthTick();

            if (timerElapsed(g_activeAccount_timer) >= 1.0f)
            {
                timerStart(g_activeAccount_timer);
                UpdateConsoleTitle();
                DBServer_Heartbeat(true);
            }
        }
    }

    // graceful shutdown loop
    while (!qIsEmpty(s_delayedPackets) && AccountDb_Tick())
    {
        AccountServer_TickDelayedPackets();
        NMMonitor(10);
        asql_tick();
        DBServer_Heartbeat(false);
    }

    AccountRequest::Shutdown();
    asql_close();
    Transaction_Shutdown();

    FOREACH_LINK_SAFE(Account, account, next, s_accountsToReAuth, reauthLink)
        account->reauthLink.unlink();

    AccountDb_Shutdown();
    accountInventory_Shutdown();
    lnkFlushAll();

    EXCEPTION_HANDLER_END;
}
