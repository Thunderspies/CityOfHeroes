/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "accountservercomm.h"
#include "statservercomm.h"
#include <utilitieslib/network/sock.h>
#include "clientcomm.h"
#include "offline.h"
#include "dbrelay.h"
#include "auction/Xact.h"
#include "sql_fifo.h"
#include <utilitieslib/components/EString.h>
#include "dbserver/servercfg.h"
#include "dbinit.h"
#include "container.h"
#include "comm_backend.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/network/net_packet.h>
#include "container_sql.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/network/net_structdefs.h>
#include "auction/Auction.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include "namecache.h"
#include "account/AccountCatalog.h"

#include <utilitieslib/utils/file.h> // isProductionMode()
#include "dbserver/auctionservercomm.h"
#include "authcomm.h"
#include "dbdispatch.h"
#include "mapxfer.h"
#include "entity/gametypes.h"
#include <utilitieslib/components/StashTable.h>
#include "account\AccountData.h"
#include <utilitieslib/components/HashFunctions.h>
#include "auth/authUserData.h"
#include "container_tplt.h"
#include "container/container_util.h"
#include "container_merge.h"
#include "container_diff.h"
#include "comm_game.h"
#include "turnstileDb.h"
#include "turnstile/TurnstileServerCommon.h"
#include "queueservercomm.h"
#include <utilitieslib/utils/log.h>
#include "account/AccountData.h"
#include <utilitieslib/utils/StringUtil.h>

#define ACCOUNT_DEADMAN_TIME (60*2)


typedef struct AccountServerInfo {
    U32 last_received;        // deadman's switch
    int link_rotor;
} AccountServerInfo;

typedef struct ClientRebroadcastCookie
{
    Packet*    pak_in;
    unsigned int    cursor_bit_pos;
    unsigned int    cursor_byte_pos;
} ClientRebroadcastCookie;

static void account_sendCatalogUpdateToClientCB( NetLink* link, GameClientLink *gameClientOrNULL, bool direct, U32 auth_id, int dbId, void* userData );
static void account_sendCatalogUpdateToClient( NetLink* link, GameClientLink *gameClientOrNULL, U32 auth_id, int dbId, Packet* pak_in );

static void account_sendCatalogUpdateToMapCB( NetLink* link, void* userData );
static void account_sendCatalogUpdateToMap( NetLink* link, Packet* pak_in );

static AccountServerInfo account_state = {0};
static NetLinkList accountserver_links = {0};
static bool mtx_alive = false;

static int accountCertificationTestStage;

int accountServerSecondsSinceUpdate()
{
    int secs = 9999;
    if(accountserver_links.links && accountserver_links.links->size)
        secs = timerSecondsSince2000() - account_state.last_received;
    if(!mtx_alive)
        secs = 8889; // odd numbers show up as red in server monitor
    return secs;
}

NetLink *accountLink()
{
    int count = accountserver_links.links ? accountserver_links.links->size : 0;

    if(!count)
        return NULL;

    account_state.link_rotor = (account_state.link_rotor+1) % count;
    return accountserver_links.links->storage[account_state.link_rotor];
}

bool accountServerResponding()
{
    if (accountServerSecondsSinceUpdate() < 120)
        return true;
    else 
        return false;
}

// *********************************************************************************
// dbserver account inventory handling
// *********************************************************************************    

int accountInventoryFindBonusCharacterSlots(U32 auth_id)
{
    ShardAccountCon* shardaccount_con = (ShardAccountCon*)containerLoad(dbListPtr(CONTAINER_SHARDACCOUNT), auth_id);
    if (shardaccount_con)
    {
        return shardaccount_con->slot_count;
    }
    return 0;
}

U8 *accountInventoryGetLoyaltyStatus(U32 auth_id)
{
    ShardAccountCon* shardaccount_con = (ShardAccountCon*)containerPtr(dbListPtr(CONTAINER_SHARDACCOUNT), auth_id);
    if (shardaccount_con)
    {
        return shardaccount_con->loyaltyStatus;
    }
    return NULL;
}

static void accountInventoryRelayHelper(NetLink *link_out, ShardAccountCon *con, Packet *pak_out, int virtualCurrencyBal, U32 lastEmailTime, U32 lastNumEmailsSent)
{
    int i, size;

    pktSetOrdered(pak_out, 1);
    pktSendString(pak_out, server_cfg.shard_name);

    // Remove nulls from the list before we compute the size
    for (i = eaSize(&con->inventory)-1; i >= 0; i--)
        if (con->inventory[i] == NULL || skuIdIsNull(con->inventory[i]->sku_id))
            eaRemoveAndDestroy(&con->inventory, i, NULL);  

    size = eaSize(&con->inventory);
    pktSendBitsAuto(pak_out, size);

    for (i = 0; i < size; i++)
    {
        ShardAccountInventoryItem *pInv = con->inventory[i];

#if defined(FULLDEBUG)
        // dbserver doesn't have the product catalog to do full verification, but at least check for valid strings
        assert(!skuIdIsNull(pInv->sku_id) && skuIdEquals(pInv->sku_id, skuIdFromString(skuIdAsString(pInv->sku_id))));
#endif

        pktSendBitsAuto2(pak_out, pInv->sku_id.u64);
        pktSendBitsAuto(pak_out, pInv->granted_total);
        pktSendBitsAuto(pak_out, pInv->claimed_total);
        pktSendBitsAuto(pak_out, pInv->expires);
    }
    pktSendBitsArray(pak_out, LOYALTY_BITS, con->loyaltyStatus);
    pktSendBitsAuto(pak_out, con->loyaltyPointsUnspent);
    pktSendBitsAuto(pak_out, con->loyaltyPointsEarned);
    pktSendBitsAuto(pak_out, virtualCurrencyBal);
    pktSendBitsAuto(pak_out, lastEmailTime);
    pktSendBitsAuto(pak_out, lastNumEmailsSent);
    pktSendBitsAuto(pak_out, con->accountStatusFlags);
    pktSendBitsAuto(pak_out, con->slot_count);
    pktSend(&pak_out,link_out);
}

static Packet *accountInventoryStartRelayThroughMapserver(NetLink *link_out, bool enablePlayerCheck, int dbid, int authoritative)
{
    Packet *pak_out = pktCreateEx(link_out,DBSERVER_ACCOUNTSERVER_INVENTORY);
    pktSendBits(pak_out, 1, authoritative);
    pktSendBool(pak_out, enablePlayerCheck); // MapCheckDisallowedPlayer
    pktSendBitsAuto(pak_out, dbid);
    return pak_out;
}

void accountInventoryRelayForRelay(NetLink *link_out, ShardAccountCon *con, int dbid)
{
    Packet *pak_out = accountInventoryStartRelayThroughMapserver(link_out, false, dbid, false);
    accountInventoryRelayHelper(link_out, con, pak_out, 0, 0, 0);
}

void accountInventoryRelayToClient(U32 auth_id, int shardXferTokens, int shardXferFreeOnly, int virtualCurrencyBal, U32 lastEmailTime, U32 lastNumEmailsSent, int authoritative)
{
    ShardAccountCon *con = containerPtr(shardaccounts_list, auth_id);
    if (con)
    {
        bool direct;
        int dbid;
        NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);

        if(link_out)
        {
            Packet *pak_out = NULL;

            if(direct)
            {
                GameClientLink *gameClient = dbClient_getGameClient(auth_id, link_out);
                if(gameClient)
                {
                    pak_out = dbClient_createPacket(DBGAMESERVER_ACCOUNTSERVER_INVENTORY, gameClient);
                    pktSendBits(pak_out, 1, authoritative);
                    pktSendBits(pak_out, 1, shardXferFreeOnly? 1: 0);
                    pktSendBitsAuto(pak_out, shardXferTokens);
                }
            }
            else
            {
                pak_out = accountInventoryStartRelayThroughMapserver(link_out, true, dbid, authoritative);
            }

            if(pak_out)
            {
                accountInventoryRelayHelper(link_out, con, pak_out, virtualCurrencyBal, lastEmailTime, lastNumEmailsSent);
            }
        }
    }
}

// *********************************************************************************
//  account svr sql handling
// *********************************************************************************    

typedef enum AccountSqlCbType
{
    ACCOUNTSQLCB_CHARCOUNT1,
    ACCOUNTSQLCB_CHARCOUNT2,
    ACCOUNTSQLCB_SHARDXFER_VALIDATE,
    ACCOUNTSQLCB_SHARDXFER_VALIDATE_DST,
    ACCOUNTSQLCB_SHARDXFER_COPY_SRC,
    ACCOUNTSQLCB_SHARDXFER_COPY_DST,
    ACCOUNTSQLCB_SHARDXFER_RECOVER,
    ACCOUNTSQLCB_CHARRENAME_VALID1,
    ACCOUNTSQLCB_CHARRENAME_VALID2,
    ACCOUNTSQLCB_CHARRENAME_FULFILL,
    ACCOUNTSQLCB_CHARRESPEC_VALID,
    ACCOUNTSQLCB_CHARRESPEC_FULFILL,
} AccountSqlCbType;

typedef struct AccountSqlCbData 
{
    AccountSqlCbType type;
    OrderId order_id;
    U32 dbid;
    char *container_txt;
    U32 auth_id;
    int generic_integer_one;
    int generic_integer_two;
} AccountSqlCbData;
MP_DEFINE(AccountSqlCbData);

static AccountSqlCbData *s_AccountSqlCbData(AccountSqlCbType type, OrderId order_id, int dbid, char *container_txt, U32 auth_id, int generic_integer_one, int generic_integer_two)
{
    // this will leak if the account server link is dropped while the sql command is in flight
    AccountSqlCbData *data;
    MP_CREATE(AccountSqlCbData,64);
    data = MP_ALLOC(AccountSqlCbData);
    assert(data);
    data->type = type;
    data->order_id = order_id;
    data->dbid = dbid;
    data->container_txt = container_txt ? strdup(container_txt) : NULL;
    data->auth_id = auth_id;
    data->generic_integer_one = generic_integer_one;
    data->generic_integer_two = generic_integer_two;
    return data;
}

static void s_AccountSqlCbData_Destroy(AccountSqlCbData *data)
{
    if(data->container_txt)
        free(data->container_txt);
    MP_FREE(AccountSqlCbData,data);
}

static void s_SendShardXferInit(int dbid, OrderId order_id, int type, int homeShard, char *shardVisitorData);
static void s_SendCharRenameInit(int dbid, OrderId order_id);
static void s_SendCharRespecInit(int dbid, OrderId order_id);

static void s_ClearAccSvrLock(int dbid, U32 auth_id, OrderId order_id)
{
    char *cmd = estrTemp();
    estrPrintf(&cmd, "UPDATE dbo.Ents2 SET AccSvrLock=NULL WHERE ContainerId=%d;", dbid);
    sqlExecAsyncEx(cmd, estrLength(&cmd), dbid, false);
    estrDestroy(&cmd);

    accountMarkOneSaved(auth_id, order_id);
}

const char *unlocked_used_count_restrict = "WHERE AuthId=%d AND IsSlotLocked IS NULL";
const char *total_used_count_restrict = "WHERE AuthId=%d";

static int putCharacterGetUsedSlots(const char *restrict_format, U32 auth_id)
{
    char restriction[100];
    int row_count;
    ColumnInfo *field_ptrs[4];
    char *rows;

    sprintf(restriction, restrict_format, auth_id);
    rows = sqlReadColumnsSlow(ent_list->tplt->tables,0,"ContainerId, Name",restriction, &row_count, field_ptrs);
    if (!rows)
        return 0;

    return row_count;
}

static void s_AccountSqlCb(Packet *pak, U8 *cols, int col_count, ColumnInfo **field_ptrs, void* udata)
{
    AccountSqlCbData *data = udata;
    NetLink *link = accountLink(); // FIXME: verify that link is the same as pak's link?

    if(!link || !link->connected)
    {
        LOG_OLD_ERR("lost AccountServer link during sql query");
        s_AccountSqlCbData_Destroy(data);
        pktDestroy(pak);
        return;
    }

    switch(data->type)
    {
        xcase ACCOUNTSQLCB_CHARCOUNT1:
        {
            int bonus_slots = data->generic_integer_one;
            int auth_id = data->auth_id;
            char *restriction = NULL;
            estrPrintf(&restriction, unlocked_used_count_restrict, auth_id);
            sqlFifoBarrier();
            sqlReadColumnsAsync(ent_list->tplt->tables,0,"ContainerId",restriction,s_AccountSqlCb,
                                s_AccountSqlCbData(ACCOUNTSQLCB_CHARCOUNT2,kOrderIdInvalid,0,NULL,auth_id,bonus_slots,col_count),pak,0);
            estrDestroy(&restriction);
        }
        xcase ACCOUNTSQLCB_CHARCOUNT2:
        {
            int bonus_slots = data->generic_integer_one;
            int total_char_count = data->generic_integer_two;
            pktSendBitsAuto(pak, col_count);
            pktSendBitsAuto(pak, total_char_count);
            pktSendBitsAuto(pak, bonus_slots); // excludes VIP slots
            pktSendBitsAuto(pak, MAX_SLOTS_PER_SHARD + server_cfg.dual_player_slots);
            pktSendBitsAuto(pak, server_cfg.isVIPServer);
            pktSend(&pak,link);
        }
        xcase ACCOUNTSQLCB_SHARDXFER_VALIDATE:
            pktSendBool(pak,col_count > 0);
            pktSend(&pak,link);

        xcase ACCOUNTSQLCB_SHARDXFER_VALIDATE_DST:
        {
            int accountMaxSlots;
            U32 auth_id;
            int totalUsedCount;
            int unlockedUsedCount;
            int actual_vip = data->generic_integer_one;
            accountMaxSlots = actual_vip ? server_cfg.dual_player_slots : 0;
            if (col_count > 0)
            {
                int slotCount = *(int*)&cols[0];
                accountMaxSlots += slotCount;
            }

            auth_id = data->auth_id;
            totalUsedCount = putCharacterGetUsedSlots(total_used_count_restrict, auth_id);
            unlockedUsedCount = putCharacterGetUsedSlots(unlocked_used_count_restrict, auth_id);

            pktSendBitsAuto(pak, unlockedUsedCount);
            pktSendBitsAuto(pak, totalUsedCount);
            pktSendBitsAuto(pak, accountMaxSlots); // includes VIP slots
            pktSendBitsAuto(pak, MAX_SLOTS_PER_SHARD + server_cfg.dual_player_slots);
            pktSendBitsAuto(pak, server_cfg.isVIPServer);
            pktSend(&pak,link);
        }

        xcase ACCOUNTSQLCB_SHARDXFER_COPY_SRC:
            if(col_count > 0)
            {
                pktDestroy(pak);
                s_SendShardXferInit(data->dbid, data->order_id, XFER_TYPE_PAID, 0, "UNUSED");
            }
            else // character has been deleted (recovering or unfulfillable)
            {
                pktSendString(pak,""); // container_txt
                pktSend(&pak,link);
            }

        xcase ACCOUNTSQLCB_SHARDXFER_COPY_DST:
        {
            int new_dbid = (col_count > 0) ? *(int*)(&cols[0]) : handleCreateEntByStr(data->container_txt);
            if(new_dbid)
            {
                pktSendBitsAuto(pak,new_dbid);
                pktSend(&pak,link);
            }
            else // handleCreateEntByStr failed. this has been logged. accountserver will reattempt
                pktDestroy(pak);
        }

        xcase ACCOUNTSQLCB_SHARDXFER_RECOVER:
        {
            if(col_count > 0)
            {
                int new_dbid = *(int*)(&cols[0]);
                s_ClearAccSvrLock(new_dbid, data->auth_id, data->order_id);
                LOG_OLD("Character with dbid %d successfully shardxferred with order_id %s", new_dbid, orderIdAsString(data->order_id));
            }
            else
                LOG_OLD_ERR("Character not found for shardxfer with order_id %s", orderIdAsString(data->order_id));

            pktSendBool(pak,col_count > 0);
            pktSend(&pak,link);
        }

        xcase ACCOUNTSQLCB_CHARRENAME_VALID1:
            if(col_count > 0)
            {
                char *restriction = NULL;
                estrPrintf(&restriction,"WHERE ContainerId=%d",data->dbid);
                sqlReadColumnsAsync(ent_list->tplt->tables,0,"DbFlags",restriction,s_AccountSqlCb,
                                    s_AccountSqlCbData(ACCOUNTSQLCB_CHARRENAME_VALID2,kOrderIdInvalid,data->dbid,NULL,0,0,0),pak,data->dbid);
                estrDestroy(&restriction);
            }
            else // character not found or locked
            {
                pktSendBool(pak,false); // confirm
                pktSend(&pak,link);
            }

        xcase ACCOUNTSQLCB_CHARRENAME_VALID2:
        {
            bool confirm = false;

            if (col_count > 0)
            {
                DbFlag db_flags = *(int*)(&cols[0]);
                if(!(db_flags & DBFLAG_RENAMEABLE))
                    confirm = true;
            }
            pktSendBool(pak,confirm);
            pktSend(&pak,link);
        }

        xcase ACCOUNTSQLCB_CHARRENAME_FULFILL:
            if(col_count > 0)
            {
                pktDestroy(pak);
                s_SendCharRenameInit(data->dbid, data->order_id);
            }
            else // character has been deleted (unfulfillable)
            {
                pktSendBool(pak,false); // success
                pktSend(&pak,link);
            }

        xcase ACCOUNTSQLCB_CHARRESPEC_VALID:
            if(col_count > 0)
            {
                pktSendBool(pak, true); // success
            }
            else // character not found or locked
            {
                pktSendBool(pak, false); // fail
            }
            pktSend(&pak,link);

        xcase ACCOUNTSQLCB_CHARRESPEC_FULFILL:
            if(col_count > 0)
            {
                pktDestroy(pak);
                s_SendCharRespecInit(data->dbid, data->order_id);
            }
            else // character has been deleted (unfulfillable)
            {
                pktSendBool(pak, false); // fail
                pktSend(&pak,link);
            }

        xdefault:
            LOG_OLD_ERR( "Unknown AccountSqlCbType %d", data->type);
    }

    s_AccountSqlCbData_Destroy(data);
}

// *********************************************************************************
//  account svr message handler
// *********************************************************************************    

// msgs from account server
static int accountMsgCallback(Packet *pak_in,int cmd,NetLink *auc_link);

static int account_clientCreateCb(NetLink *link)
{
    netLinkSetMaxBufferSize(link, BothBuffers, 8*1024*1024); // Set max size to auto-grow to
    netLinkSetBufferSize(link, BothBuffers, 1*1024*1024);
    link->userData = NULL;
    return 1;
}

static int account_clientDeleteCb(NetLink *link)
{
    LOG_OLD( "Lost connection to accountserver.");
    return 1;
}

void accountMonitorInit(void)
{
    netLinkListAlloc(&accountserver_links,2,0,account_clientCreateCb);
    accountserver_links.destroyCallback = account_clientDeleteCb;
    if(netInit(&accountserver_links,0, DEFAULT_ACCOUNTSERVER_PORT))
        NMAddLinkList(&accountserver_links, accountMsgCallback);
    else
        FatalErrorf("Error listening for account server.\n");
}

static void account_clientCountFailure(U32 auth_id)
{
    GameClientLink *client = 0;
    NetLink *link = linkFromAuthId(auth_id, &client); 
    if(link && client)
    {
        Packet *pak_out = dbClient_createPacket(DBGAMESERVER_ACCOUNTSERVER_CHARCOUNT, client);
        pktSendBitsAuto(pak_out,0); // no reply shards
        pktSendString(pak_out,"AccountClientNoServer");
        pktSend(&pak_out,link);
    }
}

static void account_clientGenericFailure(U32 auth_id, char *error)
{
    GameClientLink *client = 0;
    NetLink *link = linkFromAuthId(auth_id, &client); 
    if(link && client)
    {
        Packet *pak_out = dbClient_createPacket(DBGAMESERVER_LOGIN_DIALOG_RESPONSE, client);
        pktSendString(pak_out, error);
        pktSend(&pak_out, link);
    }
}

static void account_clientAccountServerUnavailable(U32 auth_id)
{
    GameClientLink *client = 0;
    NetLink *link = linkFromAuthId(auth_id, &client); 
    if(link && client)
    {
        Packet *pak_out = dbClient_createPacket(DBGAMESERVER_ACCOUNTSERVER_UNAVAILABLE, client);
        pktSend(&pak_out, link);
    }
}

static void account_sendUnavailableMessage(U32 auth_id)
{
    bool direct;
    int dbid;
    NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
    Packet *pak_out = NULL;

    if (link_out)
    {
        if (direct)
        {
            // TODO implement me!
        }
        else
        {
            pak_out = pktCreateEx(link_out, DBSERVER_CLIENT_MESSAGE);
            pktSendBitsAuto(pak_out, dbid);
        }

        if (pak_out)
        {
            pktSendString(pak_out, "AccountServerDown");
            pktSend(&pak_out, link_out);
        }
    }
}

static void s_SendShardXferInit(int dbid, OrderId order_id, int type, int homeShard, char *shardVisitorData)
{
    char *cmd = NULL;
    estrPrintf(&cmd,"cmdrelay_dbid %d\nshardxfer_init %s %d %d %s", dbid, orderIdAsString(order_id), type, homeShard, shardVisitorData);
    sendToEnt(NULL,dbid,1,cmd);
    estrDestroy(&cmd);
}

static void s_SendCharRenameInit(int dbid, OrderId order_id)
{
    char *cmd = NULL;
    estrPrintf(&cmd,"cmdrelay_dbid %d\norder_rename %s", dbid, orderIdAsString(order_id));
    sendToEnt(NULL,dbid,1,cmd);
    estrDestroy(&cmd);
}

static void s_SendCharRespecInit(int dbid, OrderId order_id)
{
    char *cmd = NULL;
    estrPrintf(&cmd,"cmdrelay_dbid %d\norder_respec %s", dbid, orderIdAsString(order_id));
    sendToEnt(NULL,dbid,1,cmd);
    estrDestroy(&cmd);
}

void accountAuctionShardXfer(OrderId order_id, int dbid)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out;
        EntCon *ent = containerLoad(ent_list,dbid); // deletePlayer will "unload"

        if(ent)
            offlinePlayerDelete(ent->auth_id,ent->account,dbid);
        deletePlayer(dbid,true,NULL,"AccountServer",NULL);

        pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_SHARDXFER);
        pktSendBitsAuto2(pak_out, order_id.u64[0]);
        pktSendBitsAuto2(pak_out, order_id.u64[1]);
        pktSendBitsAuto(pak_out,SHARDXFER_FULFILL_COMMIT_SRC);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        LOG_OLD_ERR( "accountAuctionShardXfer: AccountServer link lost, will try again later. {\"order_id\":\"%s\", \"dbid\":%d)", orderIdAsString(order_id), dbid);
    }
}

static void s_handleShardXferPacket(NetLink *acc_link, Packet *pak_in)
{
    OrderId order_id = { pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in) };
    ShardXferStep step = pktGetBitsAuto(pak_in);

    switch(step)
    {
        xcase SHARDXFER_VALIDATE_SRC:
        {
            int dbid = pktGetBitsAuto(pak_in);
            bool confirm = containerPtr(ent_list, dbid) != NULL;

            Packet *pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_SHARDXFER);
            pktSendBitsAuto2(pak_out,order_id.u64[0]);
            pktSendBitsAuto2(pak_out,order_id.u64[1]);
            pktSendBitsAuto(pak_out,SHARDXFER_VALIDATE_SRC);

            if(confirm)
            {
                pktSendBool(pak_out,confirm);
                pktSend(&pak_out,acc_link);
            }
            else // the character isn't loaded, run a query instead
            {
                char *restriction = NULL;
                estrPrintf(&restriction,"WHERE ContainerId=%d",dbid);
                sqlReadColumnsAsync(ent_list->tplt->tables,0,"ContainerId",restriction,s_AccountSqlCb,
                                    s_AccountSqlCbData(ACCOUNTSQLCB_SHARDXFER_VALIDATE,kOrderIdInvalid,0,NULL,0,0,0),pak_out,dbid);
                estrDestroy(&restriction);
            }
        }

        xcase SHARDXFER_VALIDATE_DST:
        {
            Packet *pak_out;
            char *restriction;

            U32 auth_id = pktGetBitsAuto(pak_in);
            int vip = pktGetBits(pak_in, 1);

            pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_SHARDXFER);
            pktSendBitsAuto2(pak_out,order_id.u64[0]);
            pktSendBitsAuto2(pak_out,order_id.u64[1]);
            pktSendBitsAuto(pak_out, SHARDXFER_VALIDATE_DST);

            restriction = NULL;
            estrPrintf(&restriction,"WHERE ContainerId=%d",auth_id);
            sqlReadColumnsAsync(shardaccounts_list->tplt->tables,0,"SlotCount",restriction,s_AccountSqlCb,
                                s_AccountSqlCbData(ACCOUNTSQLCB_SHARDXFER_VALIDATE_DST,kOrderIdInvalid,0,NULL,auth_id,vip,0),pak_out,auth_id);
            estrDestroy(&restriction);
        }

        xcase SHARDXFER_FULFILL_COPY_SRC:
        {
            int dbid;
            int type;
            int homeShard;
            char shardVisitorData[SHARD_VISITOR_DATA_SIZE + 32];// Local (safe) copy of shardVisitorData string.  "+ 32" to allow for the appended shardname
            char *shardVisitorDataPkt;                            // Pointer to copy of above we get from pktGetString
            bool confirm;
            DBServerCfg *dbserver;

            dbid = pktGetBitsAuto(pak_in);
            type = pktGetBitsAuto(pak_in);
            shardVisitorDataPkt = pktGetString(pak_in);
            confirm = containerPtr(ent_list, dbid) != NULL;

            if (type == XFER_TYPE_VISITOR_OUTBOUND || type == XFER_TYPE_VISITOR_BACK)
            {
                _snprintf_s(shardVisitorData, sizeof(shardVisitorData), sizeof(shardVisitorData) - 1, "%s;%s", shardVisitorDataPkt, server_cfg.shard_name);

                dbserver = turnstileFindShardByName(server_cfg.shard_name);
                // If this assert trips, it's because the shardname we think we're called (i.e. in servers.cfg) doesn't have a matching entry
                // in turnstile_server.cfg
                devassert(dbserver && "turnstile_server.cfg is broken: servers.cfg ShardName entry not found");
                homeShard = dbserver->shardID;

                if (homeShard == 0)
                {
                    printf("**** Error: ShardName in Server.cfg %s has no matching entry in turnstile_server.cfg\n", server_cfg.shard_name);
                    break;
                }
            }
            else
            {
                strncpyt(shardVisitorData, shardVisitorDataPkt, sizeof(shardVisitorData));
            }

            if (confirm)
            {
                s_SendShardXferInit(dbid, order_id, type, homeShard, shardVisitorData);
            }
            else // the character isn't loaded, run a query instead
            {
                char *restriction = NULL;

                // This should never trip, because for that to happen, someone would have to be doing a visitor shard
                // transfer while offline.  That's not supposed to be possible, since being online is a pre-requisite
                // for a visitor transfer.
                if (type == XFER_TYPE_PAID)
                {
                    Packet *pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_SHARDXFER);
                    pktSendBitsAuto2(pak_out,order_id.u64[0]);
                    pktSendBitsAuto2(pak_out,order_id.u64[1]);
                    pktSendBitsAuto(pak_out,SHARDXFER_FULFILL_COPY_SRC);
                    pktSendBool(pak_out,1);                                    // success
    
                    // check if the container exists, we may be recovering or the order may be unfulfillable
                    estrPrintf(&restriction,"WHERE ContainerId=%d",dbid);
                    sqlReadColumnsAsync(ent_list->tplt->tables,0,"ContainerId",restriction,s_AccountSqlCb,
                                    s_AccountSqlCbData(ACCOUNTSQLCB_SHARDXFER_COPY_SRC,order_id,dbid,NULL,0,0,0),pak_out,dbid);
                    estrDestroy(&restriction);
                }
                else
                {
                    // However we have this fall back code to handle the case of an untimely disconnect.  In that instance we report back to
                    // the account server that things went south, and that they should drop the transfer.  This does mean that the Turnstile
                    // server has a slightly incorrect view of reality: it thinks this character is visiting when they are actually not.
                    // However in that case, the character will remain visiting (as far as the TSS is concerned) till either the event completes
                    // or the visit duration timeout expires.  In either instance, the TSS will request a return transfer, which will also silently
                    // fail, but at least that will restore the system to a coherent state.
                    Packet *pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_SHARDXFER);
                    pktSendBitsAuto2(pak_out,order_id.u64[0]);
                    pktSendBitsAuto2(pak_out,order_id.u64[1]);
                    pktSendBitsAuto(pak_out,SHARDXFER_FULFILL_COPY_SRC);
                    pktSendBool(pak_out,0);                                    // failure
                    pktSend(&pak_out,acc_link);
                }
            }
        }

        xcase SHARDXFER_FULFILL_COPY_DST:
        {
            char *container_txt = pktGetString(pak_in);
            char *restriction = NULL;

            Packet *pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_SHARDXFER);
            pktSendBitsAuto2(pak_out,order_id.u64[0]);
            pktSendBitsAuto2(pak_out,order_id.u64[1]);
            pktSendBitsAuto(pak_out,SHARDXFER_FULFILL_COPY_DST);

            // check if a character has already been created for this OrderId
            estrPrintf(&restriction,"WHERE AccSvrLock='%s'",orderIdAsString(isProductionMode()?order_id:kOrderIdInvalid));
            sqlFifoBarrier();
            sqlReadColumnsAsync(&(ent_list->tplt->tables[1]),0,"ContainerId",restriction,s_AccountSqlCb,
                                s_AccountSqlCbData(ACCOUNTSQLCB_SHARDXFER_COPY_DST,order_id,0,container_txt,0,0,0),pak_out,0);
            estrDestroy(&restriction);
        }

        xcase SHARDXFER_FULFILL_COMMIT_SRC:
        {
            int dbid = pktGetBitsAuto(pak_in);
            char *dst_shard = pktGetString(pak_in);
            int dst_dbid = pktGetBitsAuto(pak_in);
            int type = pktGetBitsAuto(pak_in);

            // At this point shard visitor transfers diverge from paid transfers.
            if (type == XFER_TYPE_VISITOR_OUTBOUND || type == XFER_TYPE_VISITOR_BACK)
            {
                // Shard visitor transfers send a message via the turnstile server to the remote dbid asking it to prepare for the client to connect, and send
                // us back the appropriate cookie
                turnstileDBserver_generateCookieRequest(order_id, dbid, type, dst_dbid, dst_shard);
            }
            else
            {
                // Meanwhile paid transfers have a conversation with the auction server here to shift inventory, and on receipt of a successful reply, they continue.
                auctionServerShardXfer(order_id, dbid, dst_shard, dst_dbid);
                // on response from auction server, accountAuctionShardXfer will be called
            }
        }

        xcase SHARDXFER_FULFILL_COMMIT_DST:
        {
            Packet *pak_out;
            U32 auth_id = pktGetBitsAuto(pak_in);
            int dbid = pktGetBitsAuto(pak_in);
            s_ClearAccSvrLock(dbid, auth_id, order_id);

            LOG_OLD( "Character with dbid %d successfully shard transferred on guid %s", dbid, orderIdAsString(order_id));

            pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_SHARDXFER);
            pktSendBitsAuto2(pak_out,order_id.u64[0]);
            pktSendBitsAuto2(pak_out,order_id.u64[1]);
            pktSendBitsAuto(pak_out,SHARDXFER_FULFILL_COMMIT_DST);
            pktSend(&pak_out,acc_link);
        }

        xcase SHARDXFER_FULFILL_COMMIT_RECOVERY:
        {
            char *restriction = NULL;
            U32 auth_id = pktGetBitsAuto(pak_in);

            Packet *pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_SHARDXFER);
            pktSendBitsAuto2(pak_out,order_id.u64[0]);
            pktSendBitsAuto2(pak_out,order_id.u64[1]);
            pktSendBitsAuto(pak_out,SHARDXFER_FULFILL_COMMIT_RECOVERY);

            // make sure a character was created with this OrderId
            estrPrintf(&restriction,"WHERE AccSvrLock='%s'",orderIdAsString(order_id));
            sqlFifoBarrier();
            sqlReadColumnsAsync(&(ent_list->tplt->tables[1]),0,"ContainerId",restriction,s_AccountSqlCb,
                                s_AccountSqlCbData(ACCOUNTSQLCB_SHARDXFER_RECOVER,order_id,0,NULL,auth_id,0,0),pak_out,0);
            estrDestroy(&restriction);
        }

        xcase SHARDXFER_FULFILL_XFER_JUMP:
        {
            int dbid = pktGetBitsAuto(pak_in);

            turnstileDBserver_generateJumpRequest(dbid);
        }

        xdefault:
        {
            LOG_OLD_ERR("Invalid step %d for shardxfer packet",step);
        }
    }
}

/**
* Send any necessary account information over to the accountserver
*/
static void sendAccountData(Packet *pak, U32 auth_id, const char *auth_name)
{
    pktSendBitsAuto(pak, auth_id);
    pktSendString(pak, auth_name);
}

/**
* Tell the accountserver about all active accounts on this shard
*/
static void sendAllAccountData(Packet *pak)
{
    int i;
    int *ilist = NULL;
    int count;

    for (i=0; i<shardaccounts_list->num_alloced; i++)
    {
        ShardAccountCon *con = cpp_reinterpret_cast(ShardAccountCon*)(shardaccounts_list->containers[i]);
        if (con->logged_in)
            eaiPush(&ilist, i);
    }

    count = eaiSize(&ilist);
    pktSendBitsAuto(pak, count);
    for (i = 0; i < count; i++)
    {
        ShardAccountCon *con = cpp_reinterpret_cast(ShardAccountCon*)(shardaccounts_list->containers[ilist[i]]);
        sendAccountData(pak, con->id, con->account);
    }

    eaiDestroy(&ilist);
}

static void startCharCountFetch(U32 auth_id, int bonus_slots, Packet *pak_out)
{
    char *restriction = NULL;
    estrPrintf(&restriction, total_used_count_restrict, auth_id);
    sqlFifoBarrier();
    sqlReadColumnsAsync(ent_list->tplt->tables,0,"ContainerId",restriction,s_AccountSqlCb,
        s_AccountSqlCbData(ACCOUNTSQLCB_CHARCOUNT1,kOrderIdInvalid,0,NULL,auth_id,bonus_slots,0),pak_out,0);
    estrDestroy(&restriction);
}

static void onVipUpdateCharCount(VipSqlCbData *data)
{
    ShardAccountCon *shardaccount_con = data->shardaccount_con;
    startCharCountFetch(shardaccount_con->id, shardaccount_con->slot_count, data->pak_out);
}

// messages from account server
static int accountMsgCallback(Packet *pak_in,int cmd,NetLink *acc_link)
{
    account_state.last_received = timerSecondsSince2000();
    switch (cmd)
    {
    case ACCOUNT_SVR_CONNECT:
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_CONNECT);
        int i;
        
        for(i = 0; i < accountserver_links.links->size; ++i)
        {
            NetLink *link = accountserver_links.links->storage[i];
            if(link && link->connected &&
               acc_link->addr.sin_addr.S_un.S_addr != link->addr.sin_addr.S_un.S_addr)
            {
                LOG_OLD_ERR("Got a second AccountServer link request from a different IP %s! Keeping the existing connection.",stringFromAddr(&acc_link->addr)); 
                netSendDisconnect(acc_link,0.f);
                break;
            }
        }
        if(i < accountserver_links.links->size)
            break;
        
        pktSendBitsAuto( pak_out, DBSERVER_ACCOUNT_PROTOCOL_VERSION);
        pktSendString(pak_out,*server_cfg.shard_name?server_cfg.shard_name:server_cfg.db_server);
        sendAllAccountData(pak_out);
        pktSend(&pak_out,acc_link);

        if( server_cfg.isLoggingMaster )
            sendLogLevels(acc_link, ACCOUNT_CLIENT_UPDATE_LOG_LEVELS);
    }
    xcase ACCOUNT_SVR_RELAY_CMD:
    {
        int message_dbid = pktGetBitsAuto(pak_in);
        U32 auth_id = 0;
        char *cmd_str;

        if(!message_dbid)
            auth_id = pktGetBitsAuto(pak_in);

        cmd_str = pktGetString(pak_in);
        
        if(message_dbid) // mapserver connection
        {
            char *relay_str = NULL;
            estrPrintf(&relay_str,"cmdrelay_dbid %d 0\nconprint \"%s\"",message_dbid,cmd_str);
            sendToEnt(NULL,message_dbid, FALSE, relay_str);
            estrDestroy(&relay_str);
        }
        else if(auth_id)
        {
            GameClientLink *client = 0;
            NetLink *res_link = linkFromAuthId(auth_id, &client); 
            if(res_link && client)
            {
                Packet *pak_out = dbClient_createPacket(DBGAMESERVER_ACCOUNTSERVER_UNAVAILABLE, client);
                pktSendString(pak_out,cmd_str);
                pktSend(&pak_out,res_link);
            }
        }
    }
    xcase ACCOUNT_SVR_CHARCOUNT_REQUEST:
    {
        U32 auth_id = pktGetBitsAuto(pak_in);
        bool vip = pktGetBool(pak_in);
        ShardAccountCon* shardaccount_con;

        Packet *pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_CHARCOUNT_REPLY); // unfortunately, we have to create the packet here
        pktSendBitsAuto(pak_out,auth_id);

        // Need to hanlde VIP -> premium slot locking if the player hasn't logged into this shard since becoming premium.
        shardaccount_con = (ShardAccountCon*)containerLoad(shardaccounts_list, auth_id);
        if (shardaccount_con)
        {
            // If ShardAccount container wasn't loaded already, update the other_hashes cache for consistency.
            stashAddPointer(shardaccounts_list->other_hashes,shardaccount_con->account,shardaccount_con,false);

            // shardaccount_con->accountStatusFlags could be bogus if the player hasn't already logged into this shard.
            // Applying the current VIP value to prevent a false VIP -> free transition.
            if (vip)
                shardaccount_con->accountStatusFlags |= ACCOUNT_STATUS_FLAG_IS_VIP;
            else
                shardaccount_con->accountStatusFlags = ACCOUNT_STATUS_FLAG_DEFAULT;

            updateVip(onVipUpdateCharCount, NULL, shardaccount_con, pak_out);
        }
        else
        {
            // Don't need to worry about VIP -> premium locking in this case.
            // bonus_slots will be 0 anyway, so there aren't any slots for a player to transfer into.
            startCharCountFetch(auth_id, 0, pak_out);
        }
    }
    xcase ACCOUNT_SVR_CHARCOUNT_REPLY:
    {
        U32 auth_id = pktGetBitsAuto(pak_in);
        bool direct;
        int dbid;
        GameClientLink *client = 0;
        NetLink *link = findLinkToClient(auth_id, &direct, &dbid);
        linkFromAuthId(auth_id, &client); 

        if(link && client);
        {
            int i, shards = pktGetBitsAuto(pak_in);
            Packet *pak_out;

            if (direct)
            {
                pak_out = dbClient_createPacket(DBGAMESERVER_ACCOUNTSERVER_CHARCOUNT, client);
            }
            else
            {
                pak_out = pktCreateEx(link,DBSERVER_ACCOUNTSERVER_CHARCOUNT);
                pktSendBitsAuto(pak_out,dbid);
            }
            pktSendBitsAuto(pak_out,shards);
            if(shards)
            {
                for(i = 0; i < shards; ++i)
                {
                    pktSendString(pak_out,pktGetString(pak_in)); // shard name
                    pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // unlocked character count
                    pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // total character count
                    pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // account slot count, excludes VIP slots
                    pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // server slot count
                    pktSendBitsAuto(pak_out,pktGetBitsAuto(pak_in)); // vip-only status
                }
            }
            else
            {
                pktSendString(pak_out,pktGetString(pak_in)); // error message
            }
            pktSend(&pak_out,link);
        }
    }
    xcase ACCOUNT_SVR_SLOTCOUNT_REQUEST:
    {
        NetLink *acc_link = accountLink();
        if(acc_link)
        {
            U32 auth_id = pktGetBitsAuto(pak_in);
            ShardAccountCon* shardaccount_con = (ShardAccountCon*)containerLoad(shardaccounts_list, auth_id);

            Packet *pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_SLOTCOUNT_REPLY);
            pktSendBitsAuto(pak_out,auth_id);
            pktSendBitsAuto(pak_out,shardaccount_con?shardaccount_con->slot_count:0);
            pktSend(&pak_out, acc_link);
        }
    }
    xcase ACCOUNT_SVR_PRODUCT_CATALOG_UPDATE:
    {
        ClientRebroadcastCookie cookie = { 0 };
        cookie.pak_in = pak_in;
        cookie.cursor_bit_pos                = pak_in->stream.cursor.bit;
        cookie.cursor_byte_pos                = pak_in->stream.cursor.byte;
        
        accountCatalog_CacheAcctServerCatalogUpdate( pak_in );        

        // Reset the stream to read the packet again
        cookie.pak_in->stream.cursor.byte    = cookie.cursor_byte_pos;
        cookie.pak_in->stream.cursor.bit    = cookie.cursor_bit_pos;
        mapLinkDoForAll( account_sendCatalogUpdateToMapCB, &cookie );

        cookie.pak_in->stream.cursor.byte    = cookie.cursor_byte_pos;
        cookie.pak_in->stream.cursor.bit    = cookie.cursor_bit_pos;
        clientLinkDoForAll( account_sendCatalogUpdateToClientCB, &cookie );

    }
    xcase ACCOUNT_SVR_CLIENTAUTH:
    {
        U32 auth_id = pktGetBitsAuto(pak_in);
        bool direct;
        int dbid;
        NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
        Packet *pak_out = NULL;

        if(link_out)
        {
            if(direct)
            {
                GameClientLink *gameClient = dbClient_getGameClient(auth_id, link_out);
                if(gameClient)
                    pak_out = dbClient_createPacket(DBGAMESERVER_ACCOUNTSERVER_CLIENTAUTH, gameClient);
            }
            else
            {
                pak_out = pktCreateEx(link_out,DBSERVER_ACCOUNTSERVER_CLIENTAUTH);
                pktSendBitsAuto(pak_out, dbid);
            }
        }

        if(pak_out)
        {
            pktSendGetBitsAuto(pak_out, pak_in );    // timestamp
            pktSendGetString(pak_out, pak_in); // playSpanAuthDigest
            pktSend(&pak_out, link_out);
        }
    }
    xcase ACCOUNT_SVR_PLAYNC_AUTH_KEY:
    {
        U32 auth_id = pktGetBitsAuto(pak_in);
        bool direct;
        int dbid;
        NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
        Packet *pak_out = NULL;

        if(link_out)
        {
            if(direct)
            {
                GameClientLink *gameClient = dbClient_getGameClient(auth_id, link_out);
                if(gameClient)
                    pak_out = dbClient_createPacket(DBGAMESERVER_ACCOUNTSERVER_PLAYNC_AUTH_KEY, gameClient);
            }
            else
            {
                pak_out = pktCreateEx(link_out,DBSERVER_ACCOUNTSERVER_PLAYNC_AUTH_KEY);
                pktSendBitsAuto(pak_out, dbid);
            }
        }

        if(pak_out)
        {
            pktSendGetBitsAuto(pak_out,pak_in); // request_key
            pktSendGetString(pak_out, pak_in); // PlayNC auth key
            pktSend(&pak_out, link_out);
        }
    }
    xcase ACCOUNT_SVR_SHARDXFER:
        s_handleShardXferPacket(acc_link,pak_in);

    xcase ACCOUNT_SVR_RENAME: // TODO: look into merging this with other order types
    {
        OrderId order_id = { pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in) };

        if(pktGetBool(pak_in)) // validating
        {
            int dbid = pktGetBitsAuto(pak_in);
            bool confirm = containerPtr(ent_list, dbid) != NULL;
            char *restriction = NULL;

            Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_RENAME);
            pktSendBitsAuto2(pak_out, order_id.u64[0]);
            pktSendBitsAuto2(pak_out, order_id.u64[1]);
            pktSendBool(pak_out,true); // validating

            if(!confirm)
            {
                estrPrintf(&restriction,"WHERE ContainerId=%d AND AccSvrLock IS NULL",dbid);
                sqlReadColumnsAsync(&(ent_list->tplt->tables[1]),0,"AccSvrLock",restriction,s_AccountSqlCb,
                                    s_AccountSqlCbData(ACCOUNTSQLCB_CHARRENAME_VALID1,kOrderIdInvalid,dbid,NULL,0,0,0),pak_out,dbid);
                estrDestroy(&restriction);
            }
            else
            {
                pktSendBool(pak_out,confirm);
                pktSend(&pak_out,acc_link);
            }
        }
        else // fulfilling
        {
            int dbid = pktGetBitsAuto(pak_in);
            bool confirm = containerPtr(ent_list, dbid) != NULL;

            if(confirm)
            {
                s_SendCharRenameInit(dbid,order_id);
            }
            else // the character isn't loaded, run a query instead
            {
                char *restriction = NULL;

                Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_RENAME);
                pktSendBitsAuto2(pak_out, order_id.u64[0]);
                pktSendBitsAuto2(pak_out, order_id.u64[1]);
                pktSendBool(pak_out,false); // fulfilling

                // check if the container exists, the order may be unfulfillable
                estrPrintf(&restriction,"WHERE ContainerId=%d",dbid);
                sqlReadColumnsAsync(ent_list->tplt->tables,0,"ContainerId",restriction,s_AccountSqlCb,
                    s_AccountSqlCbData(ACCOUNTSQLCB_CHARRENAME_FULFILL,order_id,dbid,NULL,0,0,0),pak_out,dbid);
                estrDestroy(&restriction);
            }
        }
    }

    xcase ACCOUNT_SVR_SLOT: // TODO: look into merging this with other order types
    {
        OrderId order_id = { pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in) };
        U32 auth_id = pktGetBitsAuto(pak_in);
        ShardAccountCon *con = containerPtr(shardaccounts_list, auth_id);
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_SLOT);
        bool confirm;
        if (con)
        {
            char buf[100];
            int new_count = con->slot_count + 1;
            sprintf(buf, "SlotCount %d\n", new_count);
            containerUpdate(dbListPtr(CONTAINER_SHARDACCOUNT), con->id, buf, 1);
            LOG(LOG_CHAR_SLOT_APPLY,
                LOG_LEVEL_IMPORTANT,
                0,
                "\"%s\" (%d) slot applied. current: global %d, server %d\n",
                con->account,
                auth_id,
                con->authoritative_reedemable_slots - 1, // this is the "future" value
                new_count);
            confirm = true;
        }
        else
        {
            confirm = false;
        }

        pktSendBitsAuto2(pak_out, order_id.u64[0]);
        pktSendBitsAuto2(pak_out, order_id.u64[1]);
        pktSendBool(pak_out,confirm);
        pktSend(&pak_out,acc_link);
    }
    
    xcase ACCOUNT_SVR_RESPEC: // TODO: look into merging this with other order types
    {
        OrderId order_id = { pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in) };

        if(pktGetBool(pak_in)) // validating
        {
            int dbid = pktGetBitsAuto(pak_in);
            bool confirm = containerPtr(ent_list, dbid) != NULL;
            char *restriction = NULL;

            Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_RESPEC);
            pktSendBitsAuto2(pak_out, order_id.u64[0]);
            pktSendBitsAuto2(pak_out, order_id.u64[1]);
            pktSendBool(pak_out,true); // validating

            if(!confirm)
            {
                estrPrintf(&restriction,"WHERE ContainerId=%d AND AccSvrLock IS NULL",dbid);
                sqlReadColumnsAsync(&(ent_list->tplt->tables[1]),0,"AccSvrLock",restriction,s_AccountSqlCb,
                    s_AccountSqlCbData(ACCOUNTSQLCB_CHARRESPEC_VALID,kOrderIdInvalid,dbid,NULL,0,0,0),pak_out,dbid);
                estrDestroy(&restriction);
            }
            else
            {
                pktSendBool(pak_out, confirm);
                pktSend(&pak_out, acc_link);
            }
        }
        else // fulfilling
        {
            int dbid = pktGetBitsAuto(pak_in);
            bool confirm = containerPtr(ent_list, dbid) != NULL;

            if(confirm)
            {
                s_SendCharRespecInit(dbid, order_id);
            }
            else // the character isn't loaded, run a query instead
            {
                char *restriction = NULL;

                Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_RESPEC);
                pktSendBitsAuto2(pak_out, order_id.u64[0]);
                pktSendBitsAuto2(pak_out, order_id.u64[1]);
                pktSendBool(pak_out,false); // fulfilling

                // check if the container exists, the order may be unfulfillable
                estrPrintf(&restriction,"WHERE ContainerId=%d",dbid);
                sqlReadColumnsAsync(ent_list->tplt->tables,0,"ContainerId",restriction,s_AccountSqlCb,
                    s_AccountSqlCbData(ACCOUNTSQLCB_CHARRESPEC_FULFILL,order_id,dbid,NULL,0,0,0),pak_out,dbid);
                estrDestroy(&restriction);
            }
        }
    }
    xcase ACCOUNT_SVR_NOTIFYTRANSACTION:
    {
        bool direct;
        int dbid;
        U32 auth_id = pktGetBitsAuto(pak_in);
        NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
        Packet *pak_out = NULL;

        if( !link_out || direct )
            break;

        pak_out = pktCreateEx(link_out, DBSERVER_ACCOUNTSERVER_NOTIFYTRANSACTION);
        pktSendBitsAuto(pak_out, dbid);
        pktSendGetBitsAuto2(pak_out, pak_in); // SKU
        pktSendGetBitsAuto(pak_out, pak_in); // invType
        pktSendGetBitsAuto2(pak_out, pak_in); // order_id
        pktSendGetBitsAuto2(pak_out, pak_in);
        pktSendGetBitsAuto(pak_out, pak_in); // granted
        pktSendGetBitsAuto(pak_out, pak_in); // claimed
        pktSendGetBool(pak_out, pak_in); // success/failure
        pktSend(&pak_out, link_out);
    }
    xcase ACCOUNT_SVR_NOTIFYREQUEST:
    {
        U32 auth_id = pktGetBitsAuto(pak_in);
        bool direct;
        int dbid;
        NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
        Packet *pak_out = NULL;

        if(link_out)
        {
            if(direct)
            {
                GameClientLink *gameClient = dbClient_getGameClient(auth_id, link_out);
                if(gameClient)
                    pak_out = dbClient_createPacket(DBGAMESERVER_ACCOUNTSERVER_NOTIFYREQUEST, gameClient);
            }
            else
            {
                pak_out = pktCreateEx(link_out,DBSERVER_ACCOUNTSERVER_NOTIFYREQUEST);
                pktSendBitsAuto(pak_out,dbid);
            }
        }

        if(pak_out)
        {
            pktSendGetBitsAuto(pak_out, pak_in); // request type
            pktSendGetBitsAuto2(pak_out, pak_in); // order_id
            pktSendGetBitsAuto2(pak_out, pak_in);
            pktSendGetBool(pak_out, pak_in); // success/failure
            pktSendGetString(pak_out,pak_in); // message
            pktSend(&pak_out,link_out);
        }
    }
    xcase ACCOUNT_SVR_NOTIFYSAVED:
    {
        U32 auth_id = pktGetBitsAuto(pak_in);
        bool direct;
        int dbid;
        NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
        Packet *pak_out = NULL;

        // try again later
        if( !link_out || direct )
            break;

        pak_out = pktCreateEx(link_out, DBSERVER_ACCOUNTSERVER_NOTIFYSAVED);
        pktSendBitsAuto(pak_out, dbid);
        pktSendGetBitsAuto2(pak_out, pak_in); // order_id
        pktSendGetBitsAuto2(pak_out, pak_in);
        pktSendGetBool(pak_out, pak_in); // success/failure
        pktSend(&pak_out, link_out);
    }
    xcase ACCOUNT_SVR_INVENTORY:
    {
        // receiving account inventory
        U32 auth_id;
        int i, dbid, size;
        AccountInventorySet tmpInvSet;
        bool direct;
        NetLink *link_out;
        Packet *pak_out = NULL;
        AccountInventory *pItem = NULL;
        int shardXferTokens, shardXferFreeOnly;
        union {
            U8 loyaltyStatus[LOYALTY_BITS/sizeof(U8)];
            U32 loyaltyStatusOld[LOYALTY_BITS/sizeof(U32)];
        } loyalty;
        int loyaltyPointsUnspent;
        int loyaltyPointsEarned;
        U32 virtualCurrencyBal;
        U32 lastEmailTime;
        U32 lastNumEmailsSent;

        auth_id = pktGetBitsAuto(pak_in);
        size = pktGetBitsAuto(pak_in);

        link_out = findLinkToClient(auth_id, &direct, &dbid);

        AccountInventorySet_InitMem(&tmpInvSet);

        // must consume stream
        for (i = 0; i < size; i++)
        {
            pItem = (AccountInventory *) calloc(1,sizeof(AccountInventory));
            AccountInventoryReceiveItem(pak_in, pItem);    
            eaPush(&tmpInvSet.invArr, pItem);
        }
        AccountInventorySet_Sort(&tmpInvSet);

        shardXferFreeOnly = pktGetBits(pak_in, 1);
        shardXferTokens = pktGetBitsAuto(pak_in);

        pktGetBitsArray(pak_in, LOYALTY_BITS, loyalty.loyaltyStatus);
        loyaltyPointsUnspent = pktGetBitsAuto(pak_in);
        loyaltyPointsEarned = pktGetBitsAuto(pak_in);
        virtualCurrencyBal = pktGetBitsAuto(pak_in);
        lastEmailTime = pktGetBitsAuto(pak_in);
        lastNumEmailsSent = pktGetBitsAuto(pak_in);

        // HYBRID : Update account inventory cache
        {
            ShardAccountCon *con = containerPtr(shardaccounts_list, auth_id);

            if (con)
            {
                char *buf, *diff;

                // Non-persisted data
                {
                    AccountInventory* pInv = AccountInventorySet_Find(&tmpInvSet, SKU("svcslots"));                
                    con->authoritative_reedemable_slots = pInv ? (pInv->granted_total - pInv->claimed_total) : 0;
                }

                // Persisted data
                estrCreate(&buf);

                estrPrintf(&buf, "AuthName \"%s\"\n", con->account);
                estrConcatf(&buf, "SlotCount %d\n", con->slot_count);
                estrConcatf(&buf, "WasVIP %d\n", con->was_vip);
                estrConcatf(&buf, "ShowPremiumSlotLockNag %d\n", con->showPremiumSlotLockNag);
                estrConcatf(&buf, "LoyaltyStatus0 %d\n", loyalty.loyaltyStatusOld[0]);
                estrConcatf(&buf, "LoyaltyStatus1 %d\n", loyalty.loyaltyStatusOld[1]);
                estrConcatf(&buf, "LoyaltyStatus2 %d\n", loyalty.loyaltyStatusOld[2]);
                estrConcatf(&buf, "LoyaltyStatus3 %d\n", loyalty.loyaltyStatusOld[3]);
                estrConcatf(&buf, "LoyaltyPointsUnspent %d\n", loyaltyPointsUnspent);
                estrConcatf(&buf, "LoyaltyPointsEarned %d\n", loyaltyPointsEarned);
                estrConcatf(&buf, "AccountStatusFlags %d\n", con->accountStatusFlags);
                estrConcatf(&buf, "AccountInventorySize %d\n", MAX(eaSize(&tmpInvSet.invArr), eaSize(&con->inventory)));

                for (i = 0; i < eaSize(&tmpInvSet.invArr); i++)
                {
                    AccountInventory *item = tmpInvSet.invArr[i];
                    
                    if (item != NULL)
                    {
                        estrConcatf(&buf, "AccountInventory[%d].SkuId0 %d\n", i, item->sku_id.u32[0]);
                        estrConcatf(&buf, "AccountInventory[%d].SkuId1 %d\n", i, item->sku_id.u32[1]);
                        estrConcatf(&buf, "AccountInventory[%d].Granted %d\n", i, item->granted_total);
                        estrConcatf(&buf, "AccountInventory[%d].Claimed %d\n", i, item->claimed_total);
                        estrConcatf(&buf, "AccountInventory[%d].Expires %d\n", i, item->expires);
                    }
                }

                diff = containerDiff(containerGetText(con), buf);

                if (strlen(diff) > 0)
                    containerUpdate(dbListPtr(CONTAINER_SHARDACCOUNT), con->id, diff, 1);
//                containerUpdate_notdiff(dbListPtr(CONTAINER_SHARDACCOUNT), con->id, buf);

                estrDestroy(&buf);

                // send the information back to client
                accountInventoryRelayToClient(con->id, shardXferTokens, shardXferFreeOnly, virtualCurrencyBal, lastEmailTime, lastNumEmailsSent, true);
            } else {
                // request load of container if there's not already a pending request
                if (!sqlIsAsyncReadPending(CONTAINER_SHARDACCOUNT, auth_id))
                {
                    GameClientLink *gameClient = NULL;
                    link_out = findLinkToClient(auth_id, &direct, &dbid);
                    if(link_out && direct)
                    {
                        gameClient = dbClient_getGameClientByName(con->account, link_out);
                    }
                    sqlReadContainerAsync(CONTAINER_SHARDACCOUNT, auth_id, link_out, gameClient, NULL);
                }
            }
        }

        AccountInventorySet_Release(&tmpInvSet);
    }
    xcase ACCOUNT_SVR_CERTIFICATION_GRANT_ACK:
    {
        bool direct;
        U32 auth_id = pktGetBitsAuto(pak_in);
        SkuId sku_id = { pktGetBitsAuto2(pak_in) };
        OrderId order_id = { pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in) };
        int dbid;
        NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
        Packet *pak_out = NULL;
        int dbid_recv = pktGetBitsAuto(pak_in);

        if (accountCertificationTestStage % 100 == 4)
        {
            int flags = accountCertificationTestStage / 100;

            if (flags % 10 == 0)
            {
                accountCertificationTestStage = 0;
            }

            if (flags / 10 == 1)
            {
                account_handleAccountTransactionFinish(auth_id, order_id, false); 
            }

            break;
        }

        if (( !link_out ) || 
            (( dbid_recv != 0 ) && ( dbid != dbid_recv )) || 
            ( direct ) )
        {
            // send failure message
            account_handleAccountTransactionFinish(auth_id, order_id, false); 
            break;
        }
    
        pak_out = pktCreateEx(link_out,DBSERVER_ACCOUNTSERVER_CERTIFICATION_GRANT);
        pktSendBitsAuto2(pak_out, sku_id.u64);
        pktSendBitsAuto2(pak_out, order_id.u64[0]);
        pktSendBitsAuto2(pak_out, order_id.u64[1]);
        pktSendBitsAuto(pak_out, dbid);
        pktSend(&pak_out,link_out);
    }
    xcase ACCOUNT_SVR_CERTIFICATION_CLAIM_ACK:
    {
        bool direct;
        int dbid;
        U32 auth_id = pktGetBitsAuto(pak_in);
        NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
        Packet *pak_out = NULL;
        SkuId sku_id = { pktGetBitsAuto2(pak_in) };
        OrderId order_id = { pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in) };

        if (accountCertificationTestStage % 100 == 4)
        {
            int flags = accountCertificationTestStage / 100;

            if (flags % 10 == 0)
            {
                accountCertificationTestStage = 0;
            }

            if (flags / 10 == 1)
            {
                account_handleAccountTransactionFinish(auth_id, order_id, false); 
            }

            break;
        }

        if( !link_out || direct )
        {
            // send failure message
            account_handleAccountTransactionFinish(auth_id, order_id, false); 
            break;
        }

        pak_out = pktCreateEx(link_out,DBSERVER_ACCOUNTSERVER_CERTIFICATION_CLAIM);
        pktSendBitsAuto(pak_out,dbid);
        pktSendBitsAuto2(pak_out, sku_id.u64);    
        pktSendBitsAuto2(pak_out, order_id.u64[0]); 
        pktSendBitsAuto2(pak_out, order_id.u64[1]); 
        pktSend(&pak_out,link_out);

    }
    xcase ACCOUNT_SVR_CERTIFICATION_REFUND_ACK:
    {
        TODO(); // Refunds are disabled at the moment
#if 0
        bool direct;
        int dbid;
        U32 auth_id = pktGetBitsAuto(pak_in);
        NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
        Packet *pak_out = NULL;
        SkuId sku_id = { pktGetBitsAuto2(pak_in) };
        int success = pktGetBitsAuto(pak_in);
        char * msg = strdup(pktGetString(pak_in));
        OrderId order_id = { pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in) };

        if (accountCertificationTestStage % 100 == 4)
        {
            int flags = accountCertificationTestStage / 100;

            if (flags % 10 == 0)
            {
                accountCertificationTestStage = 0;
            }

            if (flags / 10 == 1)
            {
                account_handleAccountCertificationRefundFinish(order_id, false); 
            }

            break;
        }

        if( !link_out || direct )
        {
            // send failure message
            account_handleAccountCertificationRefundFinish(order_id, false); 
            break;
        }

        pak_out = pktCreateEx(link_out,DBSERVER_ACCOUNTSERVER_CERTIFICATION_REFUND);
        pktSendBitsAuto(pak_out,dbid);
        pktSendBitsAuto2(pak_out, sku_id.u64); 
        pktSendBitsAuto(pak_out, success); 
        pktSendBitsAuto2(pak_out, order_id.u64[0]);
        pktSendBitsAuto2(pak_out, order_id.u64[1]);
        pktSendString(pak_out, msg);   
        pktSend(&pak_out,link_out);

        SAFE_FREE(msg);
#endif
    }
    xcase ACCOUNT_SVR_MULTI_GAME_TRANSACTION_ACK:
    {
        bool direct;
        U32 auth_id = pktGetBitsAuto(pak_in);
        char *xactName = pktGetString(pak_in);
        OrderId order_id = { pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in) };
        int dbid;
        NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
        Packet *pak_out = NULL;
        int dbid_recv = pktGetBitsAuto(pak_in);

        if (accountCertificationTestStage % 100 == 4)
        {
            int flags = accountCertificationTestStage / 100;

            if (flags % 10 == 0)
            {
                accountCertificationTestStage = 0;
            }

            if (flags / 10 == 1)
            {
                account_handleAccountTransactionFinish(auth_id, order_id, false); 
            }

            break;
        }

        if (( !link_out ) || 
            (( dbid_recv != 0 ) && ( dbid != dbid_recv )) || 
            ( direct ) )
        {
            // send failure message
            account_handleAccountTransactionFinish(auth_id, order_id, false); 
            break;
        }

        pak_out = pktCreateEx(link_out, DBSERVER_ACCOUNTSERVER_MULTI_GAME_TRANSACTION);
        pktSendString(pak_out, xactName);
        pktSendBitsAuto2(pak_out, order_id.u64[0]);
        pktSendBitsAuto2(pak_out, order_id.u64[1]);
        pktSendBitsAuto(pak_out, dbid);
        pktSend(&pak_out, link_out);
    }
    xcase ACCOUNT_SVR_MULTI_GAME_TRANSACTION_COMPLETED:
    {
        U32 auth_id = pktGetBitsAuto(pak_in);
        char *xactName = pktGetString(pak_in);
        OrderId order_id = {pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in)};
        int quantity = pktGetBitsAuto(pak_in);
        int dbid_recv = pktGetBitsAuto(pak_in);
        bool direct;
        int dbid;
        int subIndex;
        NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
        Packet *pak_out = NULL;
        
        if (accountCertificationTestStage % 100 == 8)
        {
            int flags = accountCertificationTestStage / 100;

            if (flags % 10 == 0)
            {
                accountCertificationTestStage = 0;
            }

            if (flags / 10 == 1)
            {
                account_handleAccountTransactionFinish(auth_id, order_id, false); 
            }

            break;
        }

        if (( !link_out ) || 
            (( dbid_recv != 0 ) && ( dbid != dbid_recv )) || 
            ( direct ) )
        {
            // send failure message
            account_handleAccountTransactionFinish(auth_id, order_id, false); 
            break;
        }

        pak_out = pktCreateEx(link_out, DBSERVER_ACCOUNTSERVER_MULTI_GAME_TRANSACTION_COMPLETED);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendString(pak_out, xactName);
        pktSendBitsAuto2(pak_out, order_id.u64[0]);
        pktSendBitsAuto2(pak_out, order_id.u64[1]);
        pktSendBitsAuto(pak_out, quantity);
        pktSendBitsAuto(pak_out, dbid);

        for (subIndex = 0; subIndex < quantity; subIndex++)
        {
            pktSendGetBitsAuto2(pak_out, pak_in); // sku_id
            pktSendGetBitsAuto(pak_out, pak_in); // granted
            pktSendGetBitsAuto(pak_out, pak_in); // claimed
            pktSendGetBitsAuto(pak_out, pak_in); // rarity
        }

        pktSend(&pak_out, link_out);
    }
    xcase ACCOUNT_SVR_FORCE_LOGOUT:
    {
        EntCon *ent;
        ShardAccountCon *con;
        U32 auth_id = pktGetBitsAuto(pak_in);

        ent = containerFindByAuthId(auth_id);
        if (ent)
            dbForceLogout(ent->id, -1);

        con = containerPtr(shardaccounts_list, auth_id);
        if (con)
            clientcommKickLogin(auth_id, con->account, -1);
    }
    xcase ACCOUNT_SVR_DB_HEARTBEAT:
    {
        mtx_alive = pktGetBits(pak_in, 1);
    }
    xdefault:
        LOG_OLD_ERR("accountMsgCallback: invalid net cmd %i from account server",cmd);
        break;
    };
    return 1;
}

// *********************************************************************************
//  dbdispatch handlers
// *********************************************************************************

// messages to account server
void handleSendAccountCmd(Packet *pak_in, NetLink *link)
{
    Packet *pak_out;

    NetLink *acc_link = accountLink();

    if(!acc_link)
        return;

    pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_SLASHCMD);
    pktSendGetBitsAuto(pak_out,pak_in);    // message_dbid
    pktSendGetBitsAuto(pak_out,pak_in);    // auth_id
    pktSendGetBitsAuto(pak_out,pak_in);    // dbid
    pktSendGetString(pak_out,pak_in);    // cmd
    pktSend(&pak_out,acc_link);
}

void handleAccountShardXfer(Packet *pak_in)
{
    Packet *pak_out;

    NetLink *acc_link = accountLink();

    if(!acc_link)
        return;

    pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_SHARDXFER);
    pktSendGetBitsAuto2(pak_out,pak_in);                    // order_id.u64[0]
    pktSendGetBitsAuto2(pak_out,pak_in);                    // order_id.u64[1]
    pktSendBitsAuto(pak_out,SHARDXFER_FULFILL_COPY_SRC);    // step
    pktSendBool(pak_out,1);                                    // success
    pktSendGetBool(pak_out,pak_in);                            // villain/hero
    pktSendGetString(pak_out,pak_in);                        // processed container text
    pktSend(&pak_out,acc_link);
}

void handleAccountOrderRename(Packet *pak_in)
{
    Packet *pak_out;

    NetLink *acc_link = accountLink();

    if(!acc_link)
        return;

    pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_RENAME);
    pktSendGetBitsAuto2(pak_out,pak_in);                // order_id.u64[0]
    pktSendGetBitsAuto2(pak_out,pak_in);                // order_id.u64[1]
    pktSendBool(pak_out,false);                            // fulfilling
    pktSendBool(pak_out,true);                            // success
    pktSend(&pak_out,acc_link);
}

void handleRequestPlayNCAuthKey(Packet *pak)
{
    U32 auth_id;
    int request_key;
    char* auth_name;
    char* digest_data;
    
    auth_id = pktGetBitsAuto(pak);
    request_key = pktGetBitsAuto(pak);
    strdup_alloca(auth_name,pktGetString(pak)); 
    strdup_alloca(digest_data,pktGetString(pak)); 

    account_handleGetPlayNCAuthKey( auth_id, request_key, auth_name, digest_data );
}

void handleAccountOrderRespec(Packet *pak_in)
{
    Packet *pak_out;

    NetLink *acc_link = accountLink();

    if(!acc_link)
        return;

    pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_RESPEC);
    pktSendGetBitsAuto2(pak_out,pak_in);                // order_id.u64[0]
    pktSendGetBitsAuto2(pak_out,pak_in);                // order_id.u64[1]
    pktSendBool(pak_out, false);                        // fulfilling
    pktSendBool(pak_out, true);                            // success
    pktSend(&pak_out, acc_link);
}

void relayAccountCharCount(Packet *pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);
    bool vip = pktGetBool(pak_in);

    account_handleGameClientCharacterCounts(auth_id, vip);
}

void relayAccountGetInventory(Packet *pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);

    account_handleAccountInventoryRequest(auth_id);
}

void relayAccountInventoryChange(Packet *pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);
    SkuId sku_id = { pktGetBitsAuto2(pak_in) };
    int delta = pktGetBitsAuto(pak_in);

    account_handleAccountInventoryChange(auth_id, sku_id, delta);
}

void relayAccountUpdateEmailStats(Packet *pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);
    U32 lastEmailTime = pktGetBitsAuto(pak_in);
    U32 lastNumEmailsSent = pktGetBitsAuto(pak_in);

    account_handleAccountUpdateEmailStats(auth_id, lastEmailTime, lastNumEmailsSent);
}

void relayAccountCertificationTest(Packet * pak_in)
{
    int test_stage = pktGetBitsAuto(pak_in);
    int position = test_stage % 100;

    if (position == 0)
    {
        accountCertificationTestStage = 0;
        // still forward this to the account server.
    }

    if (position == 2 || position == 4 || position == 6 || position == 8)
    {
        accountCertificationTestStage = test_stage;
    }
    else
    {
        NetLink *acc_link = accountLink();
        if(acc_link)
        {
            Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_CERTIFICATION_TEST);
            pktSendBitsAuto(pak_out, test_stage);
            pktSend(&pak_out,acc_link);
        }

    }
}

void relayAccountCertificationGrant(Packet * pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);
    SkuId sku_id = { pktGetBitsAuto2(pak_in) };
    int dbid = pktGetBitsAuto(pak_in);
    int count = pktGetBitsAuto(pak_in);

    if (accountCertificationTestStage % 100 == 2)
    {
        int flags = accountCertificationTestStage / 100;

        if (flags % 10 == 0)
        {
            accountCertificationTestStage = 0;
        }

        if (flags / 10 == 1)
        {
            account_sendUnavailableMessage(auth_id);
        }

        return;
    }

    account_handleAccountCertificationGrant(auth_id, sku_id, dbid, count);
}

void relayAccountCertificationClaim(Packet * pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);
    SkuId sku_id = { pktGetBitsAuto2(pak_in) };
    U32 db_id = pktGetBitsAuto(pak_in);

    if (accountCertificationTestStage % 100 == 2)
    {
        int flags = accountCertificationTestStage / 100;

        if (flags % 10 == 0)
        {
            accountCertificationTestStage = 0;
        }

        if (flags / 10 == 1)
        {
            account_sendUnavailableMessage(auth_id);
        }

        return;
    }

    account_handleAccountCertificationClaim(auth_id, db_id, sku_id);
}

void relayAccountRecoverUnsaved(Packet *pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);
    U32 dbid = pktGetBitsAuto(pak_in);

    account_handleAccountRecoverUnsaved(auth_id, dbid);
}

void relayAccountCertificationRefund(Packet *pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);
    SkuId sku_id = { pktGetBitsAuto2(pak_in) };
    int dbid = pktGetBitsAuto(pak_in);

    if (accountCertificationTestStage % 100 == 2)
    {
        int flags = accountCertificationTestStage / 100;

        if (flags % 10 == 0)
        {
            accountCertificationTestStage = 0;
        }

        if (flags / 10 == 1)
        {
            account_sendUnavailableMessage(auth_id);
        }

        return;
    }

    account_handleAccountCertificationRefund(auth_id, sku_id, dbid);
}

void relayAccountMultiGameTransaction(Packet *pak_in)
{
    NetLink *acc_link = accountLink();

    if (accountCertificationTestStage % 100 == 2)
    {
        int flags = accountCertificationTestStage / 100;

        if (flags % 10 == 0)
        {
            accountCertificationTestStage = 0;
        }

        if (flags / 10 == 1)
        {
            account_sendUnavailableMessage(pktGetBitsAuto(pak_in)); // auth_id
        }

        return;
    }

    if(acc_link)
    {
        U32 subtransactionIndex, subtransactionCount;
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_MULTI_GAME_TRANSACTION);
        pktSendGetBitsAuto(pak_out, pak_in); // auth_id
        pktSendGetBitsAuto(pak_out, pak_in); // db_id
        subtransactionCount = pktSendGetBitsAuto(pak_out, pak_in);
        if (pktSendGetBits(pak_out, pak_in, 1))
        {
            pktSendGetString(pak_out, pak_in); // xactName
        }

        for (subtransactionIndex = 0; subtransactionIndex < subtransactionCount; subtransactionIndex++)
        {
            pktSendGetBitsAuto2(pak_out, pak_in); // skuIds[subtransactionIndex].u64
            pktSendGetBitsAuto(pak_out, pak_in); // grantedValues[subtransactionIndex]
            pktSendGetBitsAuto(pak_out, pak_in); // claimedValues[subtransactionIndex]
            pktSendGetBitsAuto(pak_out, pak_in); // rarities[subtransactionIndex]
        }
        pktSend(&pak_out,acc_link);
    }
    else
    {
        account_sendUnavailableMessage(pktGetBitsAuto(pak_in)); // auth_id
    }
}

void relayAccountTransactionFinish(Packet * pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);
    OrderId order_id = { pktGetBitsAuto2(pak_in), pktGetBitsAuto2(pak_in) };
    bool success = pktGetBool(pak_in);

    if (accountCertificationTestStage % 100 == 6)
    {
        int flags = accountCertificationTestStage / 100;

        if (flags % 10 == 0)
        {
            accountCertificationTestStage = 0;
        }

        if (flags / 10 == 1)
        {
            account_sendUnavailableMessage(auth_id);
        }

        return;
    }

    account_handleAccountTransactionFinish(auth_id, order_id, success);
}

void relayAccountLoyaltyChange(Packet *pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);
    int nodeIndex = pktGetBitsAuto(pak_in);
    bool setunset = pktGetBool(pak_in);

    account_handleLoyaltyChange(auth_id, nodeIndex, setunset);
}

void relayAccountLoyaltyEarnedChange(Packet *pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);
    int delta = pktGetBitsAuto(pak_in);

    account_handleLoyaltyEarnedChange(auth_id, delta);
}

void relayAccountLoyaltyReset(Packet *pak_in)
{
    U32 auth_id = pktGetBitsAuto(pak_in);

    account_handleLoyaltyReset(auth_id);
}

// *********************************************************************************
//  clientcomm handlers
// *********************************************************************************

void account_handleAccountRegister(U32 auth_id, char *auth_name)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_REGISTER_ACCOUNT);
        sendAccountData(pak_out, auth_id, auth_name);
        pktSend(&pak_out, acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void account_handleAccountLogout(U32 auth_id)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_LOGOUT_ACCOUNT);
        pktSendBitsAuto(pak_out, auth_id);
        pktSend(&pak_out, acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void account_handleGameClientCharacterCounts(U32 auth_id, bool vip)
{
    NetLink *acc_link = accountLink();
    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_CHARCOUNT_REQUEST);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBool(pak_out, vip);
        pktSend(&pak_out,acc_link);
    }
    else
        account_clientCountFailure(auth_id);
}

void account_handleProductCatalog(GameClientLink *client)
{
    NetLink *acc_link = accountLink();
    if(acc_link)
    {
        Packet* pak_out = dbClient_createPacket(DBGAMESERVER_ACCOUNTSERVER_CATALOG, client);
            // note: we could send an update of the product flags here, but it has been
            // deemed to be too much data. so we're sending NULL for the product list.
        accountCatalog_AddAcctServerCatalogToPacket( pak_out );
        pktSend(&pak_out,client->link); 
    }
    else
    {
        account_clientAccountServerUnavailable(client->auth_id);
    }
}

static void account_sendCatalogUpdateToMapCB( NetLink* link, void* userData )
{
    ClientRebroadcastCookie* pCookie = (ClientRebroadcastCookie*)userData;

    // Reset the stream to read the packet again
    pCookie->pak_in->stream.cursor.byte = pCookie->cursor_byte_pos;
    pCookie->pak_in->stream.cursor.bit = pCookie->cursor_bit_pos;

    account_sendCatalogUpdateToMap( link, pCookie->pak_in );
}

static void account_sendCatalogUpdateToMap( NetLink* link, Packet* pak_in )
{
    //
    //    RCV: ACCOUNT_SVR_PRODUCT_CATALOG_UPDATE
    //    SND: DBSERVER_ACCOUNTSERVER_CATALOG
    //
    if ( link )
    { 
        Packet *pak_out = pktCreateEx(link,DBSERVER_ACCOUNTSERVER_CATALOG);
        accountCatalog_RelayServerCatalogPacket( pak_in, pak_out );
        pktSend(&pak_out,link);
    }
}

void account_sendCatalogUpdateToNewMap(NetLink *link)
{
    if (link != statLink())
    {
        Packet *pak_out = pktCreateEx(link,DBSERVER_ACCOUNTSERVER_CATALOG);
        accountCatalog_AddAcctServerCatalogToPacket(pak_out);
        pktSend(&pak_out,link);
    }
}

static void account_sendCatalogUpdateToClientCB( NetLink* link, GameClientLink *gameClientOrNULL, bool direct, U32 auth_id, int dbId, void* userData )
{
    account_sendCatalogUpdateToClient( link, gameClientOrNULL, auth_id, dbId, NULL );
}

static void account_sendCatalogUpdateToClient( NetLink* link, GameClientLink *gameClientOrNULL, U32 auth_id, int dbId, Packet* pak_in )
{
    if ( link )
    { 
        GameClientLink *gameClient = gameClientOrNULL;
        if ( gameClient == NULL )
        {
            gameClient = dbClient_getGameClient(auth_id, link);
            if ( gameClient == NULL )
            {
                return;
            }
        }
        if ( pak_in )
        {
            accountCatalog_CacheAcctServerCatalogUpdate( pak_in );
        }
        {
            Packet* pak_out = dbClient_createPacket(DBGAMESERVER_ACCOUNTSERVER_CATALOG, gameClient);
                // note: we could send an update of the product flags here, but it has been
                // deemed to be too much data. so we're sending NULL for the product list.
            accountCatalog_AddAcctServerCatalogToPacket( pak_out );
            pktSend(&pak_out,gameClient->link); 
        }
    }
}

void account_handleShardXferTokenClaim(U32 auth_id, U32 dbid, char *destshard, int isVIP)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_SHARD_XFER_TOKEN_CLAIM);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto(pak_out, dbid);
        pktSendString(pak_out, destshard);
        pktSendBits(pak_out, 1, isVIP);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void account_handleGenericClaim(U32 auth_id, AccountRequestType reqType, U32 dbid)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_GENERIC_CLAIM);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto(pak_out, reqType);
        pktSendBitsAuto(pak_out, dbid);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void account_handleLoyaltyAuthUpdate(U32 auth_id, U32 payStat, U32 loyaltyEarned, U32 loyaltyLegacy, int vip)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_AUTH_UPDATE);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto(pak_out, payStat);
        pktSendBitsAuto(pak_out, loyaltyEarned);
        pktSendBitsAuto(pak_out, loyaltyLegacy);
        pktSendBool(pak_out, vip);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void account_handleLoyaltyChange(U32 auth_id, int nodeIndex, bool set)
{
    NetLink *acc_link = accountLink();
    ShardAccountCon *accountCache = containerPtr(shardaccounts_list, auth_id);
    int bValidChange = true;

    if(acc_link && accountCache)
    {
        if (nodeIndex >= 0 && nodeIndex < LOYALTY_BITS)
        {
            LoyaltyRewardNode *node = accountLoyaltyRewardFindNodeByIndex(nodeIndex);

            if (!node)
            {
                bValidChange= false;
            } else {
                if (set)
                {
                    bValidChange = accountLoyaltyRewardCanBuyNode(NULL, accountCache->accountStatusFlags, accountCache->loyaltyStatus, 
                                                                    accountCache->loyaltyPointsUnspent, node);
                } else {
                    bValidChange =  accountLoyaltyRewardHasThisNode(accountCache->loyaltyStatus, node);
                }
            }
        }
    } else {
        bValidChange = false;
    }

    if (bValidChange)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_LOYALTY_CHANGE);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto(pak_out, nodeIndex);
        pktSendBool(pak_out, set);
        pktSendBitsAuto(pak_out, 0);        // no change in earned
        pktSend(&pak_out,acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void account_handleLoyaltyEarnedChange(U32 auth_id, int delta)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_LOYALTY_CHANGE);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto(pak_out, -1);            // no node being changed
        pktSendBool(pak_out, 0);
        pktSendBitsAuto(pak_out, delta);        // change earned
        pktSend(&pak_out,acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void account_handleLoyaltyReset(U32 auth_id)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_LOYALTY_CHANGE);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto(pak_out, -1);            // no node being changed
        pktSendBool(pak_out, 0);
        pktSendBitsAuto(pak_out, INT_MIN);            // no earned - just reset everything
        pktSend(&pak_out,acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void account_handleAccountInventoryRequest(U32 auth_id)
{
    NetLink *acc_link = accountLink();
    ShardAccountCon *accountCache = NULL;

    if(acc_link)
    {
        // Make request to account server
        Packet *pak_out = pktCreateEx(acc_link,ACCOUNT_CLIENT_INVENTORY_REQUEST);
        pktSendBitsAuto(pak_out, auth_id);
        pktSend(&pak_out,acc_link);
    }
    /*
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
    */

    // See if we have a cached copy
    accountCache =  containerPtr(shardaccounts_list, auth_id);
    if (accountCache != NULL)
    {
        // send this back as an non-authoritative copy
        accountInventoryRelayToClient(accountCache->id, 0, 0, 0, 0, 0, false);

    } else {

        // request to load container if its not already being loaded
        if (!sqlIsAsyncReadPending(CONTAINER_SHARDACCOUNT, auth_id))
        {
            bool direct;
            int dbid;
            GameClientLink *gameClient = NULL;
            NetLink *link_out = findLinkToClient(auth_id, &direct, &dbid);
            if (link_out && direct)
            {
                gameClient = dbClient_getGameClient(auth_id, link_out);
            }

            sqlReadContainerAsync(CONTAINER_SHARDACCOUNT, auth_id, link_out, gameClient, NULL);
        }
    }

}

void account_handleAccountInventoryChange(U32 auth_id, SkuId sku_id, int delta)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_INVENTORY_CHANGE);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto(pak_out, sku_id.u64);
        pktSendBitsAuto(pak_out, delta);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void account_handleAccountUpdateEmailStats(U32 auth_id, U32 lastEmailTime, U32 lastNumEmailsSent)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_UPDATE_EMAIL_STATS);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto(pak_out, lastEmailTime);
        pktSendBitsAuto(pak_out, lastNumEmailsSent);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void account_handleAccountCertificationGrant(U32 auth_id, SkuId sku_id, int dbid, int count)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_CERTIFICATION_GRANT);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto2(pak_out, sku_id.u64);
        pktSendBitsAuto(pak_out, dbid);
        pktSendBitsAuto(pak_out, count);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        account_sendUnavailableMessage(auth_id);
    }
}

void account_handleAccountCertificationClaim(U32 auth_id, U32 db_id, SkuId sku_id)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_CERTIFICATION_CLAIM);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto2(pak_out, sku_id.u64);
        pktSendBitsAuto(pak_out, db_id);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        account_sendUnavailableMessage(auth_id);
    }
}

void account_handleAccountCertificationRefund(U32 auth_id, SkuId sku_id, int dbid)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_CERTIFICATION_REFUND);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto2(pak_out, sku_id.u64);
        pktSendBitsAuto(pak_out, dbid);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        account_sendUnavailableMessage(auth_id);
    }
}

void account_handleAccountTransactionFinish(U32 auth_id, OrderId order_id, bool success)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_TRANSACTION_FINISH);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto2(pak_out, order_id.u64[0]);
        pktSendBitsAuto2(pak_out, order_id.u64[1]);
        pktSendBool(pak_out, success);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        account_sendUnavailableMessage(auth_id);
    }
}

void account_handleAccountRecoverUnsaved(U32 auth_id, U32 dbid)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_RECOVER_UNSAVED);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto(pak_out, dbid);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        account_sendUnavailableMessage(auth_id);
    }
}

void account_handleSlotTokenClaim(const char *ip_str, const char *account_name, U32 auth_id)
{
    // validate that they have redeem slots
    ShardAccountCon* shardaccount_con = (ShardAccountCon*)containerPtr(dbListPtr(CONTAINER_SHARDACCOUNT), auth_id);
    if (shardaccount_con && shardaccount_con->authoritative_reedemable_slots)
    {
        if (shardaccount_con->slot_count >= MAX_SLOTS_PER_SHARD)
        {
            account_clientGenericFailure(auth_id, "redeemSlotsTooMany");
            account_handleAccountInventoryRequest(auth_id);
        } else {
            NetLink *acc_link = accountLink();
            if(acc_link)
            {
                LOG(LOG_CHAR_SLOT_APPLY,
                    LOG_LEVEL_IMPORTANT,
                    0,
                    "\"%s\" (%d) slot requested. ip: %s current: global %d, server %d\n",
                    account_name,
                    auth_id,
                    ip_str,
                    shardaccount_con->authoritative_reedemable_slots,
                    shardaccount_con->slot_count);
                account_handleGenericClaim(auth_id, kAccountRequestType_Slot, 0);
            }
            else
            {
                // TODO: Change this to a generic error reply function.
                account_clientCountFailure(auth_id);
            }
        }
    } else {
        account_clientGenericFailure(auth_id, "redeemSlotsNotEnough");
        account_handleAccountInventoryRequest(auth_id);
    }
}

void account_handleAccountSetInventory(U32 auth_id, SkuId sku_id, U32 value, bool resendInventory)
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_SET_INVENTORY);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto2(pak_out, sku_id.u64);
        pktSendBitsAuto(pak_out, value);
        pktSend(&pak_out,acc_link);

        if (resendInventory)
            account_handleAccountInventoryRequest(auth_id);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void account_handleGetPlayNCAuthKey(U32 auth_id, int request_key, char* auth_name, char* digest_data )
{
    NetLink *acc_link = accountLink();

    if(acc_link)
    {
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_GET_PLAYNC_AUTH_KEY);
        pktSendBitsAuto(pak_out, auth_id);
        pktSendBitsAuto(pak_out, request_key);
        pktSendString(pak_out, auth_name);
        pktSendString(pak_out, digest_data);
        pktSend(&pak_out,acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void accountMarkSaved(U32 auth_id, OrderId **order_ids)
{
    NetLink *acc_link = accountLink();
    int size = eaSize(&order_ids);
    int i;

    for (i=size-1; i>=0; i--)
        if (orderIdIsNull(*order_ids[i]))
            eaRemoveAndDestroy(&order_ids, i, NULL);

    if (!size)
        return;

    if (acc_link)
    {
        int size = eaSize(&order_ids);
        Packet *pak_out = pktCreateEx(acc_link, ACCOUNT_CLIENT_MARK_SAVED);
        pktSendBitsAuto(pak_out, auth_id);

        pktSendBitsAuto(pak_out, size);
        for (i=0; i<size; i++)
        {
            pktSendBitsAuto2(pak_out, order_ids[i]->u64[0]);
            pktSendBitsAuto2(pak_out, order_ids[i]->u64[1]);
        }

        pktSend(&pak_out,acc_link);
    }
    else
    {
        // TODO: Change this to a generic error reply function.
        account_clientCountFailure(auth_id);
    }
}

void accountMarkOneSaved(U32 auth_id, OrderId order_id) 
{
    static OrderId **order_ids = NULL;
    if (!devassert(!orderIdIsNull(order_id)))
        return;
    eaSetForced(&order_ids, &order_id, 0);
    accountMarkSaved(auth_id, order_ids);
}
