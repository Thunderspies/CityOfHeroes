#include <utilitieslib/components/earray.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "account/AccountCatalog.h"
#include "account/AccountData.h"
#include "AccountInventory.h"
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/timing.h>

#if defined(SERVER)
#include "comm_backend.h"
#include "comm_game.h"
#include "dbcomm/dbcomm.h"
#include <utilitieslib/utils/timing.h>
#include "svr/svr_base.h"
#endif

#define CERTIFICATION_LOCK_TIMEOUT 300

AccountInventory *AccountInventoryFindItem(Entity *pEnt, SkuId sku_id)
{
    if (!pEnt || !pEnt->pl || !pEnt->pl->account_inv_loaded)
        return NULL;
    return AccountInventorySet_Find(&pEnt->pl->account_inventory, sku_id);
}

int AccountInventoryGetRawGrantValue(Entity *pEnt, SkuId sku_id)
{
    AccountInventory* item = AccountInventoryFindItem(pEnt, sku_id);
    return item ? item->granted_total : 0;
}

AccountInventory *AccountInventoryFindCert(Entity *pEnt, const char * recipe )
{
    const AccountProduct * product = accountCatalogGetProductByRecipe(recipe);
    if (!product)
    {
        Errorf("No product was found for the recipe '%s'", recipe);
        return NULL;
    }

    if (product->invType != kAccountInventoryType_Voucher && product->invType != kAccountInventoryType_Certification)
    {
        Errorf("SKU '%.8s' is not a voucher or certification for recipe '%s'", product->sku_id.c, recipe);
        return NULL;
    }

    if (!pEnt || !pEnt->pl || !pEnt->pl->account_inv_loaded)
        return NULL;

    return AccountInventorySet_Find( &pEnt->pl->account_inventory, product->sku_id );
}

int AccountInventoryGetCertBought(Entity *pEnt, const char * description )
{
    AccountInventory* item = AccountInventoryFindCert(pEnt, description);
    if( item )
    {
        return item->granted_total;
    }
    return 0;
}

#if defined(SERVER) || defined(CLIENT)
int AccountInventoryGetVoucherAvailible(Entity *pEnt, const char * description )
{
    AccountInventory* item = AccountInventoryFindCert(pEnt, description);
    if( item )
    {
        devassertmsg(item->invType == kAccountInventoryType_Voucher, "Tried to get availible count on a non voucher");
        return item->granted_total - item->claimed_total;
    }
    return 0;
}
#endif

#if defined(SERVER)
void AccountInventorySetRawGrantValue(Entity *pEnt, SkuId sku_id, int value)
{
    Packet *pak_out;

    if (!pEnt)
        return;

    pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_CHANGE_INV);
    pktSendBitsAuto(pak_out, pEnt->auth_id);
    pktSendBitsAuto2(pak_out, sku_id.u64);
    pktSendBitsAuto(pak_out, value);
    pktSend(&pak_out,&db_comm_link);

    if (pEnt->pl && pEnt->pl->account_inv_loaded)
    {
        // Keep local copy up-to-date until we receive the authoritative copy.
        AccountInventory* item = AccountInventoryFindItem(pEnt, sku_id);

        if (item)
            item->granted_total = value;
        else
        {
            item = (AccountInventory*) calloc(1,sizeof(AccountInventory));

            if (item)
            {
                item->sku_id = sku_id;
                item->granted_total = value;
                AccountInventorySet_AddAndSort( &pEnt->pl->account_inventory, item );
            }
        }
    }
}

void AccountServerTransactionFinish(U32 auth_id, OrderId order_id, bool success)
{
    Packet *pak_out = pktCreateEx(&db_comm_link, DBCLIENT_ACCOUNTSERVER_TRANSACTION_FINISH);
    pktSendBitsAuto(pak_out, auth_id);
    pktSendBitsAuto2(pak_out, order_id.u64[0]);
    pktSendBitsAuto2(pak_out, order_id.u64[1]);
    pktSendBool(pak_out, success);
    pktSend(&pak_out,&db_comm_link);
}

void sendAccountInventoryToClient(Entity * e, char *pShard, int bonus_slots)
{
    int i, size = eaSize(&e->pl->account_inventory.invArr);
    START_PACKET(pak_out, e, SERVER_ACCOUNTSERVER_INVENTORY);
    pktSetOrdered(pak_out, 1);
    pktSendBitsAuto(pak_out, e->db_id);
    pktSendString(pak_out, pShard);
    pktSendBits(pak_out, 1, e->pl->accountInformationAuthoritative);
    pktSendBitsAuto(pak_out, size);

    for (i = 0; i < size; i++)
    {
        AccountInventorySendItem(pak_out, e->pl->account_inventory.invArr[i]);
    }

    pktSendBitsArray(pak_out, LOYALTY_BITS, e->pl->loyaltyStatus);
    pktSendBitsAuto(pak_out, e->pl->loyaltyPointsUnspent);
    pktSendBitsAuto(pak_out, e->pl->loyaltyPointsEarned);
    pktSendBitsAuto(pak_out, e->pl->virtualCurrencyBal);
    pktSendBitsAuto(pak_out, e->pl->lastEmailTime);
    pktSendBitsAuto(pak_out, e->pl->lastNumEmailsSent);
    pktSendBitsAuto(pak_out, e->pl->account_inventory.accountStatusFlags);
    pktSendBitsAuto(pak_out, bonus_slots);

    END_PACKET;
}

#endif

// TODO: Add a stashtbale for faster lookup
CertificationRecord* certificationRecordInHistory(Entity *pEnt, const char * recipe )
{
    int i;
    for( i = eaSize(&pEnt->pl->ppCertificationHistory)-1; i>=0; i-- )
    {
        if( stricmp( pEnt->pl->ppCertificationHistory[i]->pchRecipe, recipe) == 0 )
            return  pEnt->pl->ppCertificationHistory[i];
    }
    return NULL;
}

CertificationRecord* certificationRecord_Create()
{
    return calloc(1, sizeof(CertificationRecord));
}

void certificationRecord_Destroy(void* pRecordData)
{
    CertificationRecord* pRecord = (CertificationRecord*)pRecordData;
    estrDestroy(&pRecord->pchRecipe);
    SAFE_FREE(pRecord);
}

void certificationRecord_DestroyAll(Entity *pEnt)
{

    if (pEnt && pEnt->pl)
    {
        eaClearEx(&pEnt->pl->ppCertificationHistory, certificationRecord_Destroy);
    }
}

bool certificationRecord_Locked(CertificationRecord* pRecord)
{
    if (pRecord->status != kCertStatus_None
        && timerSecondsSince2000() <= pRecord->timed_locked + CERTIFICATION_LOCK_TIMEOUT)
    {
        return true;
    }
    else
    {
        return false;
    }
}
