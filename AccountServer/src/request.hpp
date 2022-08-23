#ifndef REQUEST_H
#define REQUEST_H

#include "account/AccountData.h"
#include "comm_backend.h"
#include <utilitieslib/UtilsCXX/list.hpp>

typedef struct Account Account;
typedef struct AccountServerShard AccountServerShard;

typedef enum AccountRequestState
{
    kAccountRequestState_Charge,
    kAccountRequestState_Validate,
    kAccountRequestState_Fulfill,
    kAccountRequestState_Count
} AccountRequestState;

typedef U32 AccountRequestFlags;
#define ACCOUNTREQUEST_DEFAULT 0
#define ACCOUNTREQUEST_CSR (1<<0)
#define ACCOUNTREQUEST_BYVIP (1<<1)
/**
* AccountServer game modification request.
*/
class AccountRequest
{
    // sort key
    U32 nextTick;
    LINK(AccountRequest, tickList);

public:
    // request data
    Account *owner;
    const AccountProduct *product;
    AccountRequestType reqType;
    AccountRequestFlags flags;
    AccountRequestState reqState;
    OrderId order_id;
    U8 shard_id;
    U32 dbid;
    SkuId sku_id;
    int quantity;
    bool completed;

protected:
    void ReSort(U32 newTick);
    bool AddSorted();
    static void del(void *request);
    static bool Remove(OrderId order_id);

    virtual bool ValidateProduct();
    virtual bool ValidateTransaction() = 0;
    virtual void SubmitTransaction() = 0;
    virtual void RevertTransaction() = 0;

    AccountRequest();
    virtual ~AccountRequest() {}
    virtual bool Start(const void *extraData) { return true; }
    void UpdateNextTick();
    virtual void State_Charge(bool timeout);
    virtual void State_Validate(bool timeout);
    virtual void State_Fulfill(bool timeout) = 0;
    void RunState(bool timeout);

    static AccountRequest* Allocate(AccountRequestType reqType);

public:
    static void Init();
    static void Shutdown();
    static void Tick();
    static void ProcessPendingDeletes();

    static OrderId New(Account *account, AccountRequestType reqType, AccountRequestFlags flags, U8 shard_id, U32 dbid, SkuId sku_id, int quantity, const void *extraData);
    static AccountRequest* Recover(OrderId order_id, Account *account, AccountRequestType reqType, AccountRequestFlags flags, U8 shard_id, U32 dbid, SkuId sku_id, int quantity, const void *extraData);
    static AccountRequest* Find(OrderId order_id);
    static void OnTransactionCompleted(OrderId order_id, bool success);
    static int cmp(const AccountRequest &a, const AccountRequest &b);
    static int cmp(const CLink<AccountRequest*> &a, const CLink<AccountRequest*> &b);

    AccountServerShard* GetShard();

    void ChangeState(AccountRequestState newState);
    virtual void Complete(bool success, const char *msg = NULL);
};

class AccountGrantRequest : public AccountRequest
{
    virtual bool ValidateTransaction();
    virtual void SubmitTransaction();
    virtual void RevertTransaction();
};

class AccountClaimRequest : public AccountRequest
{
    virtual void RevertTransaction();

    protected:
        virtual void SubmitTransaction();
        virtual bool ValidateTransaction();
};

#endif
