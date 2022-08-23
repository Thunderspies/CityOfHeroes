/***************************************************************************
 *     Copyright (c) 2007-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 * $Workfile: $
 * $Revision: 58050 $
 * $Date: 2010-07-12 11:11:54 -0700 (Mon, 12 Jul 2010) $
 *
 * Module Description:
 *
 * Revision History:
 *
 ***************************************************************************/
#include "account/AccountData.h"
#include "AccountDb.hpp"
#include "AccountSql.h"
#include "AccountServer.hpp"
#include "AccountCmds.h"
#include "account_inventory.h"
#include "account_loyaltyrewards.h"
#include "request_shardxfer.hpp"
#include "comm_backend.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/log.h>
#include <utilitieslib/utils/timing.h>

struct adbGlobals {
    Account **accounts; // EArray
    Account * updateListHead; // Linked list connected via the via the nextDirty pointer
    StashTable byAuthId;
    StashTable byAuthName;
} adb;

MP_DEFINE(Account);

void AccountDb_Init()
{
    MP_CREATE(Account, ACCOUNT_INITIAL_CONTAINER_SIZE);
    adb.byAuthId = stashTableCreateInt(stashOptimalSize(ACCOUNT_INITIAL_CONTAINER_SIZE));
    adb.byAuthName = stashTableCreateWithStringKeys(stashOptimalSize(ACCOUNT_INITIAL_CONTAINER_SIZE), StashDefault);
}

void AccountDb_Shutdown()
{
    stashTableDestroy(adb.byAuthId);
    stashTableDestroy(adb.byAuthName);
    adb.updateListHead = NULL;
    MP_DESTROY(Account);
}

int AccountDb_GetCount()
{
    return eaSize(&adb.accounts);
}

static void AccountDb_Send(Account * account)
{
    Packet *pak_out;
    int i;
    int size;

    if (!SAFE_MEMBER(account->shard, link.connected))
        return;

    // send back account and inventory information
    pak_out = pktCreateEx(&account->shard->link, ACCOUNT_SVR_INVENTORY);
    pktSendBitsAuto(pak_out, account->auth_id);

    size = eaSize(&account->invSet.invArr);
    pktSendBitsAuto(pak_out, size);
    for (i = 0; i < size; i++)
        AccountInventorySendItem(pak_out, account->invSet.invArr[i]);

    pktSendBits(pak_out, 1, g_accountServerState.cfg.bFreeShardXfersOnlyEnabled ? 1 : 0);
    pktSendBitsAuto(pak_out, updateShardXferTokenCount(account));
    pktSendBitsArray(pak_out, LOYALTY_BITS, AccountDb_GetLoyaltyBits(account));
    pktSendBitsAuto(pak_out, account->loyaltyPointsUnspent);
    pktSendBitsAuto(pak_out, account->loyaltyPointsEarned);
    pktSendBitsAuto(pak_out, account->virtualCurrencyBal);
    pktSendBitsAuto(pak_out, account->lastEmailTime);
    pktSendBitsAuto(pak_out, account->lastNumEmailsSent);

    pktSend(&pak_out, &account->shard->link);
}

bool AccountDb_Tick()
{
    Account *prev = NULL;
    Account *next = NULL;
    for (Account *account = adb.updateListHead; account; account = next)
    {
        next = account->nextUpdate;

        if (account->updateFlags & ACCOUNTDB_UPDATE_SQL_ACCOUNT)
        {
            AccountDb_CommitAsync(account);
            account->updateFlags &= ~ACCOUNTDB_UPDATE_SQL_ACCOUNT;
        }

        if (account->updateFlags & ACCOUNTDB_UPDATE_NETWORK && AccountDb_IsDataLoaded(account))
        {
            AccountDb_Send(account);
            account->updateFlags &= ~ACCOUNTDB_UPDATE_NETWORK;
        }

        if (!account->updateFlags) {
            // remove from dirty list
            account->nextUpdate = NULL;
            if (!prev)
                adb.updateListHead = next;
            else
                prev->nextUpdate = next;
        } else {
            prev = account;
        }
    }

    return adb.updateListHead == NULL;
}

Account* AccountDb_LoadOrCreateAccount(U32 auth_id, const char *auth_name)
{
    Account *account;
    
    devassert(auth_id);

    if (!stashIntFindPointer(adb.byAuthId, auth_id, (void**)&account))
    {
        account = MP_ALLOC(Account);
        account->auth_id = auth_id;

        if (auth_name)
            strcpy(account->auth_name, auth_name);
        else
            account->auth_name[0] = 0;

        eaPush(&adb.accounts, account);
        assert(stashIntAddPointer(adb.byAuthId, auth_id, account, false));

        asql_find_or_create_account_async(account);    
    }

    return account;
}

Account* AccountDb_GetAccount(U32 auth_id)
{
    Account *account;
    if (stashIntFindPointer(adb.byAuthId, auth_id, (void**)&account))
        return account;
    return NULL;
}

Account* AccountDb_SearchForOnlineAccount(const char * auth_name_or_id)
{
    Account *account;
    if (stashFindPointer(adb.byAuthName, auth_name_or_id, (void**)&account))
        return account;
    return AccountDb_GetAccount(atoi(auth_name_or_id));
}

void AccountDb_SearchForOfflineAccount(const char * auth_name_or_id)
{
    asql_search_for_account_async(auth_name_or_id);
}

bool AccountDb_IsDataLoaded(Account *account)
{
    return account->asql != NULL;
}

void AccountDb_WaitFor(Account *account)
{
    devassertmsg(account->asql, "Performance Warning: AccountDb_WaitFor is going to block because the account data has not arrived yet");

    while (!account->asql)
        asql_wait_for_tick();
}

U8* AccountDb_GetLoyaltyBits(Account *account)
{
    AccountDb_WaitFor(account);
    return account->asql->loyalty_bits;
}

void AccountDb_CommitAsync(Account *account)
{
    assert(account->asql);
    account->asql->last_loyalty_point_count = account->loyaltyPointsEarned;
    account->asql->loyalty_points_spent = account->loyaltyPointsEarned - account->loyaltyPointsUnspent;
    timerMakeSQLTimestampFromSecondsSince2000(account->lastEmailTime, &account->asql->last_email_date);
    account->asql->last_num_emails_sent = account->lastNumEmailsSent;
    timerMakeSQLTimestampFromMonthsSinceZero(account->freeXferMonths, &account->asql->free_xfer_date);

    // This is difficult to check inside SQL so do the check here
    devassert(account->asql->loyalty_points_spent >= accountLoyaltyRewardGetNodesBought(account->asql->loyalty_bits));
    asql_update_account_async(account);
}

void AccountDb_MarkUpdate(Account *account, AccountUpdateFlags addFlags)
{
    assert(addFlags);

    // It is invalid to update the account before it has been retrieved
    if (addFlags & ACCOUNTDB_UPDATE_SQL_ACCOUNT)
        assert(account->asql);

    if (!account->updateFlags) {
        // Insert at linked list head
        account->nextUpdate = adb.updateListHead;
        adb.updateListHead = account;
    }

    account->updateFlags |= addFlags;
}

void AccountDb_LoadFinished(Account *account)
{
    assert(account->asql);

    strncpy(account->auth_name, account->asql->name, sizeof(account->auth_name));
    if (account->auth_name[0])
        assert(stashAddPointer(adb.byAuthName, account->auth_name, account, true));

    // This is difficult to check inside SQL so do the check here
    devassert(account->asql->loyalty_points_spent >= accountLoyaltyRewardGetNodesBought(account->asql->loyalty_bits));

    account->loyaltyPointsEarned = account->asql->last_loyalty_point_count;    
    account->loyaltyPointsUnspent = account->loyaltyPointsEarned - account->asql->loyalty_points_spent;
    account->lastEmailTime = timerGetSecondsSince2000FromSQLTimestamp(&account->asql->last_email_date);
    account->lastNumEmailsSent = account->asql->last_num_emails_sent;
    account->freeXferMonths = timerGetMonthsSinceZeroFromSQLTimestamp(&account->asql->free_xfer_date);

    // CSR might be waiting for this account to load
    AccountServer_SlashNotifyAccountLoaded(account);
}
