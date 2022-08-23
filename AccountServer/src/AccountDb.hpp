/***************************************************************************
 *     Copyright (c) 2007-2007, Cryptic Studios
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
#ifndef ACCOUNTDB_H
#define ACCOUNTDB_H

#include "account/AccountData.h"
#include <utilitieslib/UtilsCXX/list.hpp>

C_DECLARATIONS_BEGIN

typedef struct AccountServerShard AccountServerShard;
typedef struct asql_account asql_account;

typedef U32 AccountUpdateFlags;

/// Object needs the [account] table updated in SQL
#define ACCOUNTDB_UPDATE_SQL_ACCOUNT 1

/// Object needs to be transmitted to the active shard
#define ACCOUNTDB_UPDATE_NETWORK 2

/**
* Account server account information.
*/
typedef struct Account
{
    /// Linked for objects that need to be processed on the next @ref AccountDb_Tick
    struct Account * nextUpdate;

    /// List of operations to perform on the next @ref AccountDB_Tick
    AccountUpdateFlags updateFlags;

    /// AuthServer UID for the account
    U32 auth_id;

    /// PlayNC Game Account name
    char auth_name[32];
    
    /// Account SQL data
    asql_account * asql;

    /// Shard that the account is connected too
    AccountServerShard *shard;

    /// Last time the client auth digest was sent    
    U32 authDigestSent;

    /// Inventory item records for this account
    AccountInventorySet invSet;

    /// Number of loyalty points earned for this account
    /// @note Not stored in SQL
    int loyaltyPointsEarned;

    /// Number of unspent loyalty points for this account
    /// @note Not stored in SQL
    int loyaltyPointsUnspent;

    /// Last seen number of Paragon points
    int virtualCurrencyBal;

    /// Last time the player sent an email
    U32 lastEmailTime;

    /// Something to do with @ref lastEmailTime
    int lastNumEmailsSent;

    /// Last month a player received a free shard transfer
    U32 freeXferMonths;

    /// Set when a character is undergoing a shard transfer
    bool shardXferInProgress;

    // LEGACY
    int * shardXferTokens;

    /// Value when to resend authentication data
    U32 reauthTime;

    /// Next pointer for the re-auth link
    LINK(Account, reauthLink);
} Account;

void AccountDb_Init();
void AccountDb_Shutdown();
bool AccountDb_Tick();

Account* AccountDb_LoadOrCreateAccount(U32 auth_id, const char *auth_name);
Account* AccountDb_GetAccount(U32 auth_id);
Account* AccountDb_SearchForOnlineAccount(const char * auth_name_or_id);
void AccountDb_SearchForOfflineAccount(const char * auth_name_or_id);

int AccountDb_GetCount();
void AccountDb_LoadFinished(Account *account);
bool AccountDb_IsDataLoaded(Account *account);
void AccountDb_WaitFor(Account *account);
void AccountDb_CommitAsync(Account *account);
void AccountDb_MarkUpdate(Account *account, AccountUpdateFlags addFlags);

U8* AccountDb_GetLoyaltyBits(Account *account);

C_DECLARATIONS_END

#endif //ACCOUNTDB_H
