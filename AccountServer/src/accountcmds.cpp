/***************************************************************************
 *     Copyright (c) 2006-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 * $Workfile: $
 * $Revision: 57973 $
 * $Date: 2010-06-30 11:49:04 -0700 (Wed, 30 Jun 2010) $
 *
 * Module Description:
 *
 * Revision History:
 *
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "accountcmds.h"
#include "account/AccountCatalog.h"
#include "AccountDb.hpp"
#include "AccountServer.hpp"
#include "account_inventory.h"
#include "account_loyaltyrewards.h"
#include "request.hpp"
#include "transaction.h"
#include "comm_backend.h"
#include "cmdparse/cmdenum.h"
#include "cmdparse/cmdaccountserver.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/estring.h>
#include "entity/gametypes.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include <utilitieslib/utils/file.h> // isDevelopmentMode()
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/log.h>

static StashTable g_pendingSearch;
static StashTable g_pendingLoad;

typedef struct CmdServerContext
{
    NetLink *src_link;
    int message_dbid;
    int auth_id;
    int dbid;
    char *cmd;
} CmdServerContext;

void AccountServer_SlashInit()
{
    g_pendingSearch = stashTableCreateWithStringKeys(0, StashDeepCopyKeys);
    g_pendingLoad = stashTableCreateAddress(0);
}

// quotes are not allowed in msg
static void AccountSvrCmd_ConPrintEx(NetLink *link, char const* msg, int message_dbid, int auth_id)
{
    if(link && msg)
    {
        Packet *pak = pktCreateEx(link, ACCOUNT_SVR_RELAY_CMD); 
        pktSendBitsAuto(pak, message_dbid);

        if (!message_dbid)
            pktSendBitsAuto(pak, auth_id);

        pktSendString(pak, msg);
        pktSend(&pak, link);
    }
}

// quotes are not allowed in msg
static void AccountSvrCmd_ConPrint(CmdServerContext *svr_ctxt, char const* msg)
{
    AccountSvrCmd_ConPrintEx(SAFE_MEMBER(svr_ctxt, src_link), msg, svr_ctxt->message_dbid, svr_ctxt->auth_id);
}

static void AccountSvrCmd_GetStatus(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;
    
    for (int i = 0; i < eaSize(&g_accountServerState.shards); ++i)
    {
        AccountServerShard *shard = g_accountServerState.shards[i];
        if(shard->name)
        {
            estrConcatf(&res, "Shard %s: %sconnected, %svalid xfer source, %svalid xfer destination\n",
                shard->name, (shard->link.connected?"":"not "),
                (shard->shardxfer_not_a_src?"not a ":""), (shard->shardxfer_not_a_dst?"not a ":""));

            estrConcatf(&res, "  xfers valid from %s to:",shard->name);
            if(eaSize(&shard->shardxfer_destinations))
            {
                int j;
                for(j = 0; j < eaSize(&shard->shardxfer_destinations); ++j)
                    estrConcatf(&res, "%s %s", (j?",":""), shard->shardxfer_destinations[j]);
                estrConcatStaticCharArray(&res, " [if marked as a valid destination]\n");
            }
            else
                estrConcatStaticCharArray(&res, " [no destinations enabled]\n");
        }
    }

    estrConcatf(&res, "MtxEnvironment %s / Catalog %s (%d threads)\n",
        g_accountServerState.cfg.mtxEnvironment, g_accountServerState.cfg.playSpanCatalog, g_accountServerState.cfg.mtxIOThreads);

    AccountSvrCmd_ConPrint(svr_ctxt,res);
    estrDestroy(&res);
}

void AccountServer_SlashNotifyAccountLoaded(Account *account)
{
    char *res = NULL;

    CmdServerContext *svr_ctxt = NULL;
    if (!stashRemovePointer(g_pendingLoad, account, reinterpret_cast<void**>(&svr_ctxt)))
        return;

    estrConcatf(&res, "Loaded offline account: %s(%d)", account->auth_name, account->auth_id);

    AccountSvrCmd_ConPrint(svr_ctxt,res);
    estrDestroy(&res);

    free(svr_ctxt);
}

void AccountServer_SlashNotifyAccountFound(const char *search_string, U32 found_auth_id)
{
    char *res = NULL;
    bool free_svr_ctxt = true;

    CmdServerContext *svr_ctxt = NULL;
    if (!stashRemovePointer(g_pendingSearch, search_string, reinterpret_cast<void**>(&svr_ctxt)))
        return;

    if (found_auth_id)
    {
        Account *account = AccountDb_LoadOrCreateAccount(found_auth_id, NULL);
        if (!AccountDb_IsDataLoaded(account))
        {
            estrConcatf(&res, "Starting load of offline account: (%d)", found_auth_id);

            if (stashAddPointer(g_pendingLoad, account, svr_ctxt, false))
                free_svr_ctxt = false;
        }
        else
        {
            estrConcatf(&res, "Loaded offline account: %s(%d)", account->auth_name, account->auth_id);
        }
    }
    else
    {
        estrConcatf(&res, "No offline account found: %s", search_string);
    }
    
    AccountSvrCmd_ConPrint(svr_ctxt, res);
    estrDestroy(&res);

    if (free_svr_ctxt)
        free(svr_ctxt);
}

static void AccountSvrCmd_CsrSearchForAccount(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS1(char*,auth);

    Account *account = AccountDb_SearchForOnlineAccount(auth);
    if (account)
    {
        estrConcatf(&res, "Account is already online: %s(%d)\n", account->auth_name, account->auth_id);
    }
    else
    {
        CmdServerContext *svr_ctxt_copy = static_cast<CmdServerContext*>(calloc(1, sizeof(CmdServerContext)));
        svr_ctxt_copy->src_link = svr_ctxt->src_link;
        svr_ctxt_copy->message_dbid = svr_ctxt->message_dbid;
        svr_ctxt_copy->auth_id = svr_ctxt->auth_id;
        svr_ctxt_copy->dbid = svr_ctxt->dbid;

        if (!stashAddPointer(g_pendingSearch, auth, svr_ctxt_copy, false))
        {
            estrConcatf(&res, "Someone is already search for %s\n", auth);
            free(svr_ctxt_copy);
        }
        else
        {
            estrConcatf(&res, "Searching for offline account, please wait\n");
            AccountDb_SearchForOfflineAccount(auth);
        }
    }

    AccountSvrCmd_ConPrint(svr_ctxt,res);
    estrDestroy(&res);
}

static void AccountSvrCmd_CsrAccountInfo(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS1(char*,auth);

    Account *account = AccountDb_SearchForOnlineAccount(auth);
    if (account)
    {
        estrConcatf(&res,"%s(%d) loyalty %d / spent %d (currently on %s) (shard xfer %sin progress) (last free xfer month %d/%02d)",
            account->auth_name, account->auth_id, account->loyaltyPointsEarned, account->loyaltyPointsUnspent,
            account->shard ? account->shard->name : "[offline]", account->shardXferInProgress ? "" : "not ",
            account->freeXferMonths / 12, (account->freeXferMonths % 12) + 1);
    }
    else
    {
        estrConcatf(&res, "No online account found: %s\n", auth);
    }

    AccountSvrCmd_ConPrint(svr_ctxt,res);
    estrDestroy(&res);
}

static void AccountSvrCmd_CsrAccountLoyaltyShow(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS1(char*,auth);

    Account *account = AccountDb_SearchForOnlineAccount(auth);
    if (account)
    {
        estrConcatf(&res,"\nAccount %s\n------------\n", account->auth_name);
        estrConcatf(&res,"Earned Loyalty Points: %d\nUnspent Loyalty Points: %d\n", account->loyaltyPointsEarned, account->loyaltyPointsUnspent);

        // validate names and set indexes
        for (int tierIdx = 0; tierIdx < eaSize(&(g_accountLoyaltyRewardTree.tiers)); tierIdx++)
        {
            LoyaltyRewardTier *tier = g_accountLoyaltyRewardTree.tiers[tierIdx];

            if (accountLoyaltyRewardUnlockedTier(AccountDb_GetLoyaltyBits(account), tier->name))
                estrConcatf(&res, "Tier: %s unlocked\n", tier->name); 

            for (int nodeIdx = 0; nodeIdx < eaSize(&tier->nodeList); nodeIdx++)
            {
                LoyaltyRewardNode *node = tier->nodeList[nodeIdx];

                if (accountLoyaltyRewardHasNode(AccountDb_GetLoyaltyBits(account), node->name))
                    estrConcatf(&res, "    Node: %s\n", node->name);                            
            }
        }

        estrConcatStaticCharArray(&res, "\nLevels Earned\n");                            
        for (int tierIdx = 0; tierIdx < eaSize(&(g_accountLoyaltyRewardTree.levels)); tierIdx++)
        {
            if (accountLoyaltyRewardHasThisLevel(account->loyaltyPointsEarned, g_accountLoyaltyRewardTree.levels[tierIdx]))
                estrConcatf(&res, "    %s\n", g_accountLoyaltyRewardTree.levels[tierIdx]->name); 
        }
    }
    else
    {
        estrConcatf(&res, "No online account found: %s\n", auth);
    }

    AccountSvrCmd_ConPrint(svr_ctxt,res);
    estrDestroy(&res);
}


static void AccountSvrCmd_CsrAccountProductShow(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS1(char*,auth);

    Account *account = AccountDb_SearchForOnlineAccount(auth);
    if(account)
    {
        estrConcatf(&res,"\nAccount %s\n------------\n", account->auth_name);

        for (int invIdx = 0; invIdx < eaSize(&account->invSet.invArr); invIdx++)
        {
            AccountInventory *pItem = account->invSet.invArr[invIdx];
            const AccountProduct *pProduct;
            if (!pItem)
                continue;
            pProduct = accountCatalogGetProduct(pItem->sku_id);       
            if (!devassert(pProduct))
                continue;

            switch (pProduct->invType)
            {
                case kAccountInventoryType_Certification:
                    estrConcatf(&res, "Certification: '%.8s' %s (granted %d, claimed %d, saved %d)\n", pItem->sku_id.c, pProduct->recipe, pItem->granted_total, pItem->claimed_total, pItem->saved_total); 
                case kAccountInventoryType_Voucher:
                    estrConcatf(&res, "Voucher: '%.8s' %s (granted %d, claimed %d, saved %d)\n", pItem->sku_id.c, pProduct->recipe, pItem->granted_total, pItem->claimed_total, pItem->saved_total); 
                case kAccountInventoryType_PlayerInventory:
                    estrConcatf(&res, "PlayerInventory: '%.8s' %s (granted %d, claimed %d, saved %d)\n", pItem->sku_id.c, pProduct->recipe, pItem->granted_total, pItem->claimed_total, pItem->saved_total); 
            }
        }
    }
    else
    {
        estrConcatf(&res, "No online account found: %s\n", auth);
    }

    AccountSvrCmd_ConPrint(svr_ctxt,res);
    estrDestroy(&res);
}

static void AccountSvrCmd_CsrQueryAccountInventory(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS2(char*,auth,char*,product);

    Account *account = AccountDb_SearchForOnlineAccount(auth);
    if(account)
    {
        int itemCount = AccountGetStoreProductCount( &account->invSet, skuIdFromString(product), false );
        estrConcatf(&res,"\nAccount %s\n------------\n", account->auth_name);
        estrConcatf(&res,"  %s Count %d\n", product, itemCount);
    }
    else
    {
        estrConcatf(&res, "No online account found: %s\n", auth);
    }

    AccountSvrCmd_ConPrint(svr_ctxt,res);
    estrDestroy(&res);
}

static void AccountSvrCmd_CsrLoyaltyRefund(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS2(char*,auth,char*,node_name);
    Account *account = AccountDb_SearchForOnlineAccount(auth);
    if(account)
    {
        LoyaltyRewardNode *node = accountLoyaltyRewardFindNode(node_name);

        estrConcatf(&res,"\nAccount %s\n------------\n", account->auth_name);

        if (node != NULL && accountLoyaltyRewardHasNode(AccountDb_GetLoyaltyBits(account), node_name))
        {
            if (accountLoyaltyValidateLoyaltyChange(account, node->index, 0, 0))
            {
                accountLoyaltyChangeNode(account, node->index, false, true);
                estrConcatf(&res,"  Refunding node %s\n", node_name);
            } else {
                estrConcatf(&res,"  Unable to process refund of node %s\n", node_name);
            }
        }
        else 
        {
            estrConcatf(&res,"  Unable to process refund of node %s\n", node_name);
        }
    }
    else
    {
        estrConcatf(&res, "No online account found: %s\n", auth);
    }

    AccountSvrCmd_ConPrint(svr_ctxt,res);
    estrDestroy(&res);
}

static void AccountSvrCmd_CsrProductChange(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS3(char*,auth,char*,product,int,amount);

    Account *account = AccountDb_SearchForOnlineAccount(auth);
    if(account)
    {
        AccountInventory *pItem = AccountInventorySet_Find(&account->invSet, skuIdFromString(product) );
        const AccountProduct *pProduct = accountCatalogGetProduct(skuIdFromString(product));

        if (!pProduct)
            pProduct = accountCatalogGetProductByRecipe(product);

        estrConcatf(&res,"\nAccount %s\n------------\n", account->auth_name);

        if (!pProduct)
            estrConcatf(&res,"  Could not find product '%s'\n", product);
        else if (amount == 0)
            estrConcatf(&res,"  Cannot adjust product '%s' by zero units\n", product);
        else if (pItem == NULL && amount < 0)
            estrConcatf(&res,"  Could not find an existing item '%s' to remove points from\n", product);
        else if(pItem && (pItem->granted_total + amount) < pItem->claimed_total)
            estrConcatf(&res,"  Cannot change the item '%s' grant count to less than the claimed count of %d\n", product, pItem->claimed_total);
        else if(pItem && pProduct->grantLimit && (pItem->granted_total + amount) > pProduct->grantLimit)
            estrConcatf(&res,"  Cannot change the item '%s' grant count to greater than the product grant limit of %d\n", product, pProduct->grantLimit);
        else
        {
            OrderId order_id = Transaction_GamePurchaseBySkuId(account, skuIdFromString(product), 0, 0, amount, true);
            if (!orderIdIsNull(order_id))
                estrConcatf(&res,"  Changing product '%s' by %d units as order %s\n", product, amount, orderIdAsString(order_id));
            else
                estrConcatf(&res,"  Failed to modify product '%s' by %d units\n", product, amount);
        }
    }
    else
    {
        estrConcatf(&res, "No online account found: %s\n", auth);
    }

    AccountSvrCmd_ConPrint(svr_ctxt, res);
    estrDestroy(&res);
}

static void AccountSvrCmd_CsrProductReconcile(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS1(char*,auth);

    Account *account = AccountDb_SearchForOnlineAccount(auth);
    if(account)
    {
        estrConcatf(&res, "Started reconcile of account %s(%d), this may take a few minutes to complete.\n", account->auth_name, account->auth_id);
    }
    else
    {
        estrConcatf(&res, "No online account found: %s\n", auth);
    }

    AccountSvrCmd_ConPrint(svr_ctxt, res);
    estrDestroy(&res);
}

static void AccountSvrCmd_SetAccountFlags(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS3( const char*, auth, U32, flags, U32, setOrClear );

    Account* account = AccountDb_SearchForOnlineAccount( auth );
    if (account)
    {
        if (setOrClear)
            account->invSet.accountStatusFlags |= flags;
        else
            account->invSet.accountStatusFlags &= ~flags;
        AccountDb_MarkUpdate(account, ACCOUNTDB_UPDATE_NETWORK);
    }
    else
    {
        estrConcatf(&res, "No online account found: %s\n", auth);
    }

    AccountSvrCmd_ConPrint(svr_ctxt, res);
    estrDestroy(&res);
}

static void AccountSvrCmd_BuyProductFromStore(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS3( U32, auth_id, const char*, product, int, quantity );
    
    Account *account = AccountDb_GetAccount(auth_id);
    if (account)
    {
        SkuId sku_id = skuIdFromString(product);
        const AccountProduct *product = accountCatalogGetProduct(sku_id);
        AccountInventory *inv = AccountInventorySet_Find(&account->invSet, sku_id);

        if (!product)
            estrConcatf(&res,"  Could not find product '%.8s'\n", sku_id.c);
        else if (inv && product->grantLimit && !(inv->granted_total + quantity <= product->grantLimit))
            estrConcatf(&res,"  Already have product '%.8s'\n", sku_id.c);
        else if (orderIdIsNull(Transaction_GamePurchase(account, product, 0, 0, quantity, false)))
            estrConcatf(&res,"  Product purchase failed of %d '%.8s' for '%d'\n", quantity, sku_id.c, auth_id);
    }
    else
    {
        estrConcatf(&res, "No online account found: %d\n", auth_id);
    }

    AccountSvrCmd_ConPrint(svr_ctxt, res);
    estrDestroy(&res);
}

static void AccountSvrCmd_PublishProduct(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;

    READARGS2( const char*, product, int, publishIt );

    if (!accountInventory_PublishProduct(0, skuIdFromString(product), publishIt!=0))
        AccountSvrCmd_ConPrint(svr_ctxt, "Product publish failed");
}

static void AccountSvrCmd_ShardXferSrc(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS2(char*, shardname, bool, set);

    AccountServerShard *shard = AccountServer_GetShardByName(shardname);
    if (!shard)
        shard = AccountServer_GetShard(atoi(shardname));

    if(shard)
    {
        shard->shardxfer_not_a_src = !set;

        estrPrintf(&res,"Character transfer from %s: %s\n", shard->name, (set ? "enabled" : "disabled"));
    }
    else
    {
        estrPrintf(&res,"Invalid shard %s\n", shardname);
    }

    AccountSvrCmd_ConPrint(svr_ctxt, res);
    estrDestroy(&res);
}

static void AccountSvrCmd_ShardXferDst(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS2(char*, shardname, bool, set);

    AccountServerShard *shard = AccountServer_GetShardByName(shardname);
    if (!shard)
        shard = AccountServer_GetShard(atoi(shardname));

    if(shard)
    {
        shard->shardxfer_not_a_dst = !set;

        estrPrintf(&res,"Character transfer to %s: %s\n", shard->name, (set ? "enabled" : "disabled"));
    }
    else
    {
        estrPrintf(&res,"Invalid shard %s\n", shardname);
    }

    AccountSvrCmd_ConPrint(svr_ctxt, res);
    estrDestroy(&res);
}

static void AccountSvrCmd_ShardXferLink(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS3(char*, srcname, char*, dstname, bool, set);

    AccountServerShard *src = AccountServer_GetShardByName(srcname);
    AccountServerShard *dst = AccountServer_GetShardByName(dstname);
    if(src && dst)
    {
        int i;
        for(i = eaSize(&src->shardxfer_destinations)-1; i >= 0; --i)
        {
            if(!stricmp(src->shardxfer_destinations[i],dst->name))
            {
                // found a match, if we're enabling, we're done, if we're disabling, remove it and keep going
                if(set)
                    break;
                else
                    eaRemoveFastConst(&src->shardxfer_destinations,i);
            }
        }
        if(set && i < 0) // we're enabling and didn't find a match, add it
            eaPushConst(&src->shardxfer_destinations,dst->name);

        estrPrintf(&res,"Character transfer from %s to %s: %s\n", src->name, dst->name, (set ? "enabled" : "disabled"));
    }
    else
    {
        estrPrintCharString(&res,"Invalid shard");
        if(!src && !dst)
            estrConcatf(&res,"s %s and %s\n", srcname, dstname);
        else if(!src)
            estrConcatf(&res," %s\n", srcname);
        else
            estrConcatf(&res," %s\n", dstname);
    }

    AccountSvrCmd_ConPrint(svr_ctxt, res);
    estrDestroy(&res);
}

static void AccountSvrCmd_ShardXferNoCharge(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;
    char *res = NULL;

    READARGS2(char*, dstname, int, isVIP);

    Account *account = AccountDb_GetAccount(svr_ctxt->auth_id);

    AccountServerShard *src = (AccountServerShard*)svr_ctxt->src_link->userData;
    AccountServerShard *dst = AccountServer_GetShardByName(dstname);
    if (!dst)
        dst = AccountServer_GetShard(atoi(dstname));
    
    if (account && src && dst)
    {
        if(src != dst || isDevelopmentMode())
        {            
            int accountFlags = ACCOUNTREQUEST_CSR| (isVIP ? ACCOUNTREQUEST_BYVIP : 0);
            AccountRequest::New(account, kAccountRequestType_ShardTransfer, accountFlags, src->shard_id, svr_ctxt->dbid, SKU("svsvtran"), 0, dst->name);
            estrPrintf(&res,"Added no charge transfer from %s to %s\n", src->name, dstname);
        }
        else
            estrPrintf(&res,"Character is already on %s!",src->name);
    }
    else
        estrPrintCharString(&res,"Invalid account or shard\n");

    AccountSvrCmd_ConPrint(svr_ctxt, res);
    estrDestroy(&res);
}

static void printTokenCounts(Account *account, const char *skuName, const char *skuDesc, char **res)
{
    AccountInventory* pInv = AccountInventorySet_Find(&account->invSet, SKU(skuName));
    int granted_total;
    int claimed_total;
    if (pInv)
    {
        granted_total = pInv->granted_total;
        claimed_total = pInv->claimed_total;
    }
    else
    {
        granted_total = 0;
        claimed_total = 0;
    }

    estrConcatf(res, "Available %s for [%s]\n", skuDesc, account->auth_name);
    estrConcatf(res, "Granted: %d\n", granted_total);
    estrConcatf(res, "Redeemed: %d\n", claimed_total);
    estrConcatf(res, "Remaining: %d\n", granted_total - claimed_total);
}

#define REDEEMEDCOUNTREPLY_TIMEOUT (10) // seconds

#define REDEEMEDCOUNT_INVALID -3
#define REDEEMEDCOUNT_WAITING -2
#define REDEEMEDCOUNT_NOCOUNT -1

typedef struct RedeemedCounts
{
    int *redeemed_token_counts;
    U32 requested;
    Account *account;
    NetLink *link;
    int message_dbid;
} RedeemedCounts;

MP_DEFINE(RedeemedCounts);

static RedeemedCounts **ppWaitingForCounts = NULL;
static StashTable stWaitingForCounts;

static void gatherRedeemedSlotTokenCounts(Account *account, NetLink *link, int message_dbid)
{
    AccountServerState *state = &g_accountServerState;

    MP_CREATE(RedeemedCounts,64);
    RedeemedCounts *client = MP_ALLOC(RedeemedCounts);
    assert(client);

    if(!stWaitingForCounts)
        stWaitingForCounts = stashTableCreateInt(128);

    if(!stashIntAddPointer(stWaitingForCounts,account->auth_id,client,false))
    {
        MP_FREE(RedeemedCounts,client);

        return;
    }
    eaPush(&ppWaitingForCounts,client);

    client->redeemed_token_counts = static_cast<int*>(calloc(state->shard_count, sizeof(int)));
    client->requested = timerSecondsSince2000();
    client->account = account;
    client->link = link;
    client->message_dbid = message_dbid;

    for(int i = 0; i < state->shard_count; ++i)
    {
        client->redeemed_token_counts[i] = REDEEMEDCOUNT_INVALID;
    }

    StashTableIterator iter;
    StashElement elem;
    stashGetIterator( g_accountServerState.connectedShardsByName, &iter );
    while ( stashGetNextElement( &iter, &elem ) )
    {
        AccountServerShard *shard_out = (AccountServerShard*)stashElementGetPointer(elem);
        int shard_idx = eaFind(&state->shards,shard_out);

        if(shard_out && shard_idx >= 0)
        {
            if(shard_out->name && shard_out->link.connected)
            {
                Packet *pak_out = pktCreateEx(&shard_out->link,ACCOUNT_SVR_SLOTCOUNT_REQUEST);
                pktSendBitsAuto(pak_out, account->auth_id);
                pktSend(&pak_out,&shard_out->link);

                client->redeemed_token_counts[shard_idx] = REDEEMEDCOUNT_WAITING;
            }
            else
            {
                client->redeemed_token_counts[shard_idx] = REDEEMEDCOUNT_NOCOUNT;
            }
        }
    }
}

void AccountServer_RedeemedCountByShardReply(AccountServerShard *shard, Packet *pak_in)
{
    AccountServerState *state = &g_accountServerState;
    int shard_idx = eaFind(&state->shards,shard);
    RedeemedCounts *client;

    int auth_id = pktGetBitsAuto(pak_in);

    if(devassert(INRANGE0(shard_idx,state->shard_count)) && stashIntFindPointer(stWaitingForCounts,auth_id,reinterpret_cast<void**>(&client)))
    {
        int redeemed_token_count = pktGetBitsAuto(pak_in);
        if(devassert(redeemed_token_count >= 0))
        {
            client->redeemed_token_counts[shard_idx] = redeemed_token_count;
        }
    }
    else
    {
        LOG(LOG_ACCOUNT, LOG_LEVEL_VERBOSE, 0, "Got a slow redeemed token count response from %s for %d", shard?shard->name:"(null shard)", auth_id);
    }
}

void AccountServer_RedeemedCountByShardTick(void)
{
    AccountServerState *state = &g_accountServerState;
    int i;

    for(i = eaSize(&ppWaitingForCounts)-1; i >= 0; --i)
    {
        RedeemedCounts *client = ppWaitingForCounts[i];
        int j;
        for(j = 0; j < state->shard_count; ++j)
        {
            if(client->redeemed_token_counts[j] == REDEEMEDCOUNT_WAITING)
            {
                if(client->requested + REDEEMEDCOUNTREPLY_TIMEOUT < timerSecondsSince2000())
                {
                    client->redeemed_token_counts[j] = REDEEMEDCOUNT_NOCOUNT;
                }
                else
                    break;
            }
        }

        if(j >= state->shard_count) // We've heard from all the shards
        {
            char *res = NULL;
            printTokenCounts(client->account, "svcslots", "character slot tokens", &res);
            for(j = 0; j < state->shard_count; ++j)
            {
                if (client->redeemed_token_counts[j] >= 0)
                {
                    estrConcatf(&res, "%s: %d\n", state->shards[j]->name, client->redeemed_token_counts[j]);
                }
                else if (client->redeemed_token_counts[j] == REDEEMEDCOUNT_NOCOUNT)
                {
                    estrConcatf(&res, "%s: Try again later\n", state->shards[j]->name);
                }
            }
            estrConcatChar(&res, '\n');
            printTokenCounts(client->account, "svrename", "rename tokens", &res);
            estrConcatChar(&res, '\n');
            printTokenCounts(client->account, "svsvtran", "transfer tokens", &res);
            AccountSvrCmd_ConPrintEx(client->link, res, client->message_dbid, client->account->auth_id);
            estrDestroy(&res);

            // clean up
            stashIntRemovePointer(stWaitingForCounts,client->account->auth_id,NULL);
            eaRemoveFast(&ppWaitingForCounts,i);
            free(client->redeemed_token_counts);
            MP_FREE(RedeemedCounts,client);
        }
    }
}

static void AccountSvrCmd_TokenInfo(CMDARGS)
{
    CmdServerContext *svr_ctxt = cmd_context->svr_context;

    READARGS1(char*,auth);

    Account *account = AccountDb_SearchForOnlineAccount(auth);
    if (account)
    {
        gatherRedeemedSlotTokenCounts(account, svr_ctxt->src_link, svr_ctxt->message_dbid);
    }
    else
    {
        char *res = NULL;
        estrPrintf(&res, "No online account found: %s\n", auth);
        AccountSvrCmd_ConPrint(svr_ctxt,res);
        estrDestroy(&res);
    }
}

static void s_AccCmdHandler(CMDARGS)
{
    int i;
    CmdHandler fp = NULL;
    static StashTable fp_from_cmd = NULL;
    static struct
    {
        ClientAccountCmd cmd;
        CmdHandler fp;
    } s_handlers[] = 
        {
            {ClientAccountCmd_ServerStatus,AccountSvrCmd_GetStatus},
            {ClientAccountCmd_CsrSearchForAccount,AccountSvrCmd_CsrSearchForAccount},
            {ClientAccountCmd_CsrAccountInfo,AccountSvrCmd_CsrAccountInfo},
            {ClientAccountCmd_BuyProductFromStore,AccountSvrCmd_BuyProductFromStore},
            {ClientAccountCmd_PublishProduct,AccountSvrCmd_PublishProduct},
            {ClientAccountCmd_ShardXferSrc,AccountSvrCmd_ShardXferSrc},
            {ClientAccountCmd_ShardXferDst,AccountSvrCmd_ShardXferDst},
            {ClientAccountCmd_ShardXferLink,AccountSvrCmd_ShardXferLink},
            {ClientAccountCmd_ShardXferNoCharge,AccountSvrCmd_ShardXferNoCharge},
            {ClientAccountCmd_TokenInfo,AccountSvrCmd_TokenInfo},
            {ClientAccountCmd_SetAccountFlags,AccountSvrCmd_SetAccountFlags},
            {ClientAccountCmd_CsrAccountLoyaltyShow,AccountSvrCmd_CsrAccountLoyaltyShow},
            {ClientAccountCmd_CsrAccountProductShow,AccountSvrCmd_CsrAccountProductShow},
            {ClientAccountCmd_CsrAccountQueryInventory,AccountSvrCmd_CsrQueryAccountInventory},
            {ClientAccountCmd_CsrAccountLoyaltyRefund,AccountSvrCmd_CsrLoyaltyRefund},
            {ClientAccountCmd_CsrAccountProductChange,AccountSvrCmd_CsrProductChange},
            {ClientAccountCmd_CsrAccountProductReconcile,AccountSvrCmd_CsrProductReconcile},
        };
    STATIC_INFUNC_ASSERT(ARRAY_SIZE(s_handlers)==ClientAccountCmd_Count-ClientAccountCmd_None-1);

    if(!cmd_context || !cmd_context->svr_context)
        return;

    // init lookup
    if(!fp_from_cmd)
    {
        fp_from_cmd = stashTableCreateInt(ARRAY_SIZE(s_handlers)*1.25); 
        for( i = 0; i < ARRAY_SIZE( s_handlers ); ++i )
            stashIntAddPointer(fp_from_cmd,s_handlers[i].cmd,s_handlers[i].fp,TRUE);
    }

    // lookup and dispatch
    stashIntFindPointer(fp_from_cmd,cmd->num,(void**)&fp);
    if(fp)
        fp(cmd,cmd_context);
}

void AccountServer_HandleSlashCmd(NetLink *src_link, int message_dbid, U32 auth_id, int dbid, char *cmd)
{
    static int cmdlist_initted = 0;
    static CmdList server_cmdlist =  
        {
            {{ g_accountserver_cmds_server },
             { 0 }},
            s_AccCmdHandler // default function
        };
    CmdContext context = {CMD_OPERATION_NONE};
    CmdServerContext ctxt = {src_link, message_dbid, (int)auth_id, dbid, cmd};

    if(!(message_dbid || auth_id) || !cmd)
        return;
    
    if(!cmdlist_initted)
    {
        cmdOldInit(g_accountserver_cmds_server);
        cmdlist_initted = 1;
    }
    
    // access level  is not validated by AccountServer!
    context.access_level = ACCESS_INTERNAL;
    context.svr_context = &ctxt;
    cmdOldExecute(&server_cmdlist,cmd,&context);
}
