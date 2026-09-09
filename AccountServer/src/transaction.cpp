#include <utilitieslib/stdtypes.h>
#include "transaction.h"
#include "account/AccountCatalog.h"
#include "AccountDb.hpp"
#include "AccountServer.hpp"
#include "AccountSql.h"
#include "Playspan/JSONParser.h"
#include "request.hpp"
#include "account_inventory.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/timing.h>

#include <sha.h>
#include <hmac.h>
#include <md5.h>
#include <hex.h>

MP_DEFINE(MicroTransaction);
MP_DEFINE(GameTransaction);
MP_DEFINE(MultiGameTransaction);

void Transaction_Init() {
    cryptMD5Init();

    MP_CREATE(MicroTransaction, ACCOUNT_INITIAL_CONTAINER_SIZE);
    MP_CREATE(GameTransaction, ACCOUNT_INITIAL_CONTAINER_SIZE);
    MP_CREATE(MultiGameTransaction, ACCOUNT_INITIAL_CONTAINER_SIZE);
}

void Transaction_Shutdown() {
    MP_DESTROY(MicroTransaction);
    MP_DESTROY(GameTransaction);
    MP_DESTROY(MultiGameTransaction);
}

static void Transaction_SetTransactionDate(SQL_TIMESTAMP_STRUCT * s_tm) {
    SYSTEMTIME systemtime;
    GetSystemTime(&systemtime);

    s_tm->year = systemtime.wYear;
    s_tm->month = systemtime.wMonth;
    s_tm->day = systemtime.wDay;
    s_tm->hour = systemtime.wHour;
    s_tm->minute = systemtime.wMinute;
    s_tm->second = systemtime.wSecond;
    s_tm->fraction = systemtime.wMilliseconds * 1000000UL;
}

static void Transaction_SetTransactionDateFromUnixTimeString(SQL_TIMESTAMP_STRUCT * s_tm, const char *str) {
    char * end = NULL;
    time_t t = _strtoi64(str, &end, 10);
    if (!t || !end || *end) {
        memset(s_tm, 0, sizeof(SQL_TIMESTAMP_STRUCT));
        return;
    }

    struct tm tm;
    gmtime_s(&tm, &t);

    s_tm->year = (SQLUSMALLINT)(tm.tm_year + 1900);
    s_tm->month = (SQLUSMALLINT)(tm.tm_mon + 1);
    s_tm->day = (SQLUSMALLINT)tm.tm_mday;
    s_tm->hour = (SQLUSMALLINT)tm.tm_hour;
    s_tm->minute = (SQLUSMALLINT)tm.tm_min;
    s_tm->second = (SQLUSMALLINT)tm.tm_sec;
    s_tm->fraction = 0;
}

void Transaction_MicroStartTransaction(Account *account, OrderId order_id, SkuId sku_id, const char * transaction_date, int quantity, int points, PostbackMessage * message)
{
    assert(account);

    const AccountProduct * product = accountCatalogGetProduct(sku_id);
    if (!devassert(product)) {
        // This line needs to log enough data for CSR to recover with
        LOG(LOG_TRANSACTION, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "{\"reason\":\"mtx has invalid product\", \"order_id\":\"%.16s\", \"auth_id\":%d, \"sku_id\":\"%.8s\", \"quantity\":%d, \"points\":%d}",
            orderIdAsString(order_id), account->auth_id, sku_id.c, quantity, points);
        return;
    }

    MicroTransaction * transaction = MP_ALLOC(MicroTransaction);
    transaction->account = account;
    transaction->product = product;
    transaction->message = message;

    transaction->mtx.order_id = order_id;
    transaction->mtx.auth_id = account->auth_id;
    transaction->mtx.sku_id = product->sku_id;
    transaction->mtx.quantity = quantity;
    transaction->mtx.points = points;

    Transaction_SetTransactionDateFromUnixTimeString(&transaction->mtx.transaction_date, transaction_date);

    asql_add_micro_transaction_async(transaction);
}

void Transaction_MicroFinished(bool success, MicroTransaction * transaction)
{
    if (success)
    {
        accountInventory_UpdateInventoryFromSQL(transaction->account, &transaction->inv);
    }
    else
    {
        // This line needs to log enough data for CSR to recover with
        LOG(LOG_TRANSACTION, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "{\"reason\":\"mtx failed\", \"order_id\":\"%s\", \"auth_id\":%d, \"sku_id\":\"%.8s\", \"quantity\":%d, \"points\":%d}",
            orderIdAsString(transaction->mtx.order_id), transaction->mtx.auth_id, transaction->mtx.sku_id.c, transaction->mtx.quantity, transaction->mtx.points);
    }

    // ack on failure so we do not log too many failures in the transaction log for customer service
    MP_FREE(MicroTransaction, transaction);
}

static void Transaction_SetMultiGameOrderIdFromHash(MultiGameTransaction *transaction) {
    static U32 counter = timerSecondsSince2000();

    counter++;

    cryptMD5Update((U8*)transaction->transactions, sizeof(transaction->transactions));
    cryptMD5Update((U8*)&counter, sizeof(counter));
    cryptMD5Final(transaction->order_id.u32);

    assert(!orderIdIsNull(transaction->order_id));

    for (signed index = 0; index < transaction->count; index++)
    {
        transaction->transactions[index].order_id = transaction->order_id;

        // we're just incrementing the child order_ids to ensure that they are all adjacent
        // in the database, to make reverting transactions fast enough to work on live.
        transaction->transactions[index].order_id.u32[0] += (index + 1);
    }
}

static void Transaction_SetGameOrderIdFromHash(asql_game_transaction * gtx) {
    static U32 counter = timerSecondsSince2000();

    counter++;

    cryptMD5Update((U8*)gtx, sizeof(asql_game_transaction));
    cryptMD5Update((U8*)&counter, sizeof(counter));
    cryptMD5Final(gtx->order_id.u32);

    assert(!orderIdIsNull(gtx->order_id));
}

OrderId Transaction_GameStartTransaction(Account *account, const AccountProduct *product, U8 shard_id, U32 ent_id, int granted, int claimed, bool csr_did_it)
{
    assert(account);
    assert(product);

    GameTransaction * transaction = MP_ALLOC(GameTransaction);
    transaction->account = account;
    transaction->product = product;

    transaction->gtx.auth_id = account->auth_id;
    transaction->gtx.sku_id = product->sku_id;
    transaction->gtx.shard_id = shard_id;
    transaction->gtx.ent_id = ent_id;
    transaction->gtx.granted = granted;
    transaction->gtx.claimed = claimed;
    transaction->gtx.csr_did_it = csr_did_it;

    Transaction_SetTransactionDate(&transaction->gtx.transaction_date);
    Transaction_SetGameOrderIdFromHash(&transaction->gtx);

    OrderId order_id = transaction->gtx.order_id;
    asql_add_game_transaction_async(transaction);
    return order_id;
}

OrderId Transaction_GamePurchase(Account *account, const AccountProduct *product, U8 shard_id, U32 ent_id, int quantity, bool csr_did_it)
{
    return Transaction_GameStartTransaction(account, product, shard_id, ent_id, quantity, 0, csr_did_it);
}

OrderId Transaction_GamePurchaseBySkuId(Account *account, SkuId sku_id, U8 shard_id, U32 ent_id, int quantity, bool csr_did_it)
{
    const AccountProduct * product = accountCatalogGetProduct(sku_id);
    if (!product)
        return kOrderIdInvalid;

    return Transaction_GamePurchase(account, product, shard_id, ent_id, quantity, csr_did_it);
}

OrderId Transaction_GameClaim(Account *account, const AccountProduct *product, U8 shard_id, U32 ent_id, int quantity, bool csr_did_it)
{
    return Transaction_GameStartTransaction(account, product, shard_id, ent_id, 0, quantity, csr_did_it);
}

OrderId Transaction_GameClaimBySkuId(Account *account, SkuId sku_id, U8 shard_id, U32 ent_id, int quantity, bool csr_did_it)
{
    const AccountProduct * product = accountCatalogGetProduct(sku_id);
    if (!product)
        return kOrderIdInvalid;

    return Transaction_GameStartTransaction(account, product, shard_id, ent_id, 0, quantity, csr_did_it);
}

void Transaction_GameRevert(Account *account, OrderId order_id)
{
    asql_revert_game_transaction_async(account, order_id);
}

OrderId Transaction_MultiGameStartTransaction(Account *account, U8 shard_id, U32 ent_id, U32 subtransactionCount, const AccountProduct **products, U32 *grantedValues, U32 *claimedValues, bool csr_did_it)
{
    U32 index;

    assert(account);
    if (!devassert(subtransactionCount))
        return kOrderIdInvalid;

    MultiGameTransaction *transaction = MP_ALLOC(MultiGameTransaction);
    transaction->account = account;
    transaction->count = subtransactionCount;
    Transaction_SetTransactionDate(&transaction->transactions[0].transaction_date);

    for (index = 0; index < subtransactionCount; index++)
    {
        transaction->products[index] = products[index];
        transaction->transactions[index].auth_id = account->auth_id;
        transaction->transactions[index].sku_id = products[index]->sku_id;
        transaction->transactions[index].shard_id = shard_id;
        transaction->transactions[index].ent_id = ent_id;
        transaction->transactions[index].granted = grantedValues[index];
        transaction->transactions[index].claimed = claimedValues[index];
        transaction->transactions[index].csr_did_it = csr_did_it;

        if (index)
            transaction->transactions[index].transaction_date = transaction->transactions[0].transaction_date;
    }

    Transaction_SetMultiGameOrderIdFromHash(transaction);

    OrderId order_id = transaction->order_id;
    asql_add_multi_game_transaction_async(transaction);
    return order_id;
}

// also handles Transaction_MultiGameSave()
void Transaction_GameSave(Account *account, OrderId order_id)
{
    asql_save_game_transaction_async(account, order_id);
}

void Transaction_GameFinished(bool success, GameTransaction * transaction)
{
    if (success)
    {
        accountInventory_UpdateInventoryFromSQL(transaction->account, &transaction->inv);
    }
    else
    {
        // This line needs to log enough data for CSR to recover with
        LOG(LOG_TRANSACTION, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "{\"reason\":\"gtx failed\", \"order_id\":\"%s\", \"auth_id\":%d, \"sku_id\":\"%.8s\", \"granted\":%d, \"claimed\":%d, \"csr_did_it\":%d}",
            orderIdAsString(transaction->gtx.order_id), transaction->gtx.auth_id, transaction->gtx.sku_id.c, transaction->gtx.granted, transaction->gtx.claimed, transaction->gtx.csr_did_it);
    }

    AccountServer_NotifyTransactionFinished(transaction->account, transaction->product, transaction->gtx.order_id, transaction->gtx.granted, transaction->gtx.claimed, success);

    MP_FREE(GameTransaction, transaction);
}

void Transaction_MultiGameFinished(bool success, MultiGameTransaction * transaction)
{
    if (success)
    {
        accountInventory_UpdateInventoryFromFlexSQL(transaction->account, &transaction->flex_inv);
    }
    else
    {
        for (signed index = 0; index < transaction->count; index++)
        {
            // This line needs to log enough data for CSR to recover with
            LOG(LOG_TRANSACTION, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "{\"reason\":\"gtx failed\", \"order_id\":\"%s\", \"auth_id\":%d, \"sku_id\":\"%.8s\", \"granted\":%d, \"claimed\":%d, \"csr_did_it\":%d}",
                orderIdAsString(transaction->transactions[index].order_id), transaction->transactions[index].auth_id, transaction->transactions[index].sku_id.c, transaction->transactions[index].granted, transaction->transactions[index].claimed, transaction->transactions[index].csr_did_it);
        }
    }

    AccountRequest::OnTransactionCompleted(transaction->order_id, success);

    MP_FREE(MultiGameTransaction, transaction);
}

void Transaction_GameRecoverUnsaved(Account *account, U8 shard_id, U32 ent_id)
{
    TODO(); // Only claims attached to a shard entity are supported for now
    if (!devassert(shard_id))
        return;
    if (!devassert(ent_id))
        return;

    asql_read_unsaved_game_transactions_async(account, shard_id, ent_id);
}

void Transaction_GameRecoverUnsavedCallback(bool success, Account *account, asql_game_transaction *gtx_list, int gtx_count)
{
    static const SkuId respec_sku_id = SKU("svrespec");

    if (success)
    {
        for (int i=0; i<gtx_count; i++)
        {
            asql_game_transaction *gtx = gtx_list + i;
            if (!devassert(!gtx->granted))
                continue;

            if (skuIdEquals(gtx->sku_id, respec_sku_id))
            {
                TODO(); // respec retry not currently supported
                continue;
            }

            const AccountProduct * product = accountCatalogGetProduct(gtx->sku_id);
            if (!devassert(product))
                continue;

            AccountRequestType reqType = kAccountRequestType_Count;
            switch (product->invType) {
                case kAccountInventoryType_Certification:
                case kAccountInventoryType_Voucher:
                    reqType = kAccountRequestType_CertificationClaim;
                    break;
            }
            if (reqType == kAccountRequestType_Count)
                continue;

            AccountRequestFlags flags = 0;
            if (gtx->csr_did_it)
                flags |= ACCOUNTREQUEST_CSR;

            AccountRequest::Recover(gtx->order_id, account, reqType, flags, gtx->shard_id, gtx->ent_id, gtx->sku_id, gtx->claimed, NULL);
        }
    }
    else
    {
        #pragma message("Needs more investigation") 
        //NEEDS_REVIEW();
    }
}

// JSON fields
static const char * key_balance[] = {"balance", NULL};
static const char * key_hash[] = {"hash", NULL};
static const char * key_id[] = {"id", NULL};
static const char * key_item[] = {"item", NULL};
static const char * key_itemid[] = {"itemid", NULL};
static const char * key_messageid[] = {"messageid", NULL};
static const char * key_quantity[] = {"quantity", NULL};
static const char * key_sku[] = {"sku", NULL};
static const char * key_transactiondate[] = {"transactiondate", NULL};
static const char * key_transactionid[] = {"transactionid", NULL};
static const char * key_userid[] = {"userid", NULL};
static const char * key_virtualamount[] = {"virtualamount", NULL};
static const char * key_virtualcurrency[] = {"virtualcurrency", NULL};
