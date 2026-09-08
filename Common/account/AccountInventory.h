#ifndef ACCOUNTINVENTORY_H
#define ACCOUNTINVENTORY_H

#include "account/AccountTypes.h"

typedef struct AccountInventory AccountInventory;

typedef enum CertificationStatus
{
    kCertStatus_None,
    kCertStatus_GrantRequested,
    kCertStatus_RefundRequested,
    kCertStatus_ClaimRequested,
} CertificationStatus;

typedef struct CertificationRecord
{
    CertificationStatus status;
    char *pchRecipe;
    int claimed;
    int deleted;
    U32 timed_locked;
} CertificationRecord;

AccountInventory *AccountInventoryFindItem(Entity *pEnt, SkuId sku_id);
int AccountInventoryGetRawGrantValue(Entity *pEnt, SkuId sku_id);

AccountInventory *AccountInventoryFindCert(Entity *pEnt, const char * description );
int AccountInventoryGetCertBought(Entity *pEnt, const char * description );

#if defined(SERVER) || defined(CLIENT)
int AccountInventoryGetVoucherAvailible(Entity *pEnt, const char * description );
#endif

#if defined(SERVER)
void AccountInventorySetRawGrantValue(Entity *pEnt, SkuId sku_id, int value);
void AccountServerTransactionFinish(U32 auth_id, OrderId order_id, bool success);
void sendAccountInventoryToClient(Entity * e, char *pShard, int bonus_slots);
#endif

CertificationRecord* certificationRecordInHistory(Entity *pEnt, const char * pchKey );
CertificationRecord* certificationRecord_Create();

void certificationRecord_Destroy(void* pRecordData);
void certificationRecord_DestroyAll(Entity *pEnt);
bool certificationRecord_Locked(CertificationRecord* pRecord);




#endif
