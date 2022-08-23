#include "request.hpp"
#include "request_certification.hpp"
#include "request_multi.hpp"
#include "request_rename.hpp"
#include "request_respec.hpp"
#include "request_slot.hpp"
#include "request_shardxfer.hpp"
#include "account/AccountCatalog.h"
#include "AccountDb.hpp"
#include "AccountServer.hpp"
#include "transaction.h"
#include <utilitieslib/network/crypt.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/UtilsCXX/listlist.hpp>

#define CHARGE_TIMEOUT 300
#define VALIDATE_TIMEOUT 30
#define FULFILL_TIMEOUT 300

static const char *requestFailMsgs[][kAccountRequestState_Count] =
{
    {"CertificationGrantFailCharge", "CertificationGrantFailValid", "CertificationGrantFailFulfill"},
    {"CertificationClaimFailCharge", "CertificationClaimFailValid", "CertificationClaimFailFulfill"},
    {"RenameTokenClaimFailCharge", "RenameTokenClaimFailValid", "RenameTokenClaimFailFulfill"},
    {"RespecTokenClaimFailCharge", "RespecTokenClaimFailValid", "RespecTokenClaimFailFulfill"},
    {"SlotTokenClaimFailCharge", "SlotTokenClaimFailValid", "SlotTokenClaimFailFulfill"},
    {"ShardXferTokenClaimFailCharge", "ShardXferTokenClaimFailValid", "ShardXferTokenClaimFailFulfill"},
    {"MultiTransactionFailCharge", "MultiTransactionFailValid", "MultiTransactionFailFulfill"},
    {"GenericTransactionFailCharge", "GenericTransactionFailValid", "GenericTransactionFailFulfill"},
};
STATIC_ASSERT(ARRAY_SIZE(requestFailMsgs) == kAccountRequestType_Count);

static const char *requestSuccessMsgs[] =
{
    {"CertificationGrantSuccess"},
    {"CertificationClaimSuccess"},
    {"RenameTokenClaimSuccess"},
    {"RespecTokenClaimSuccess"},
    {"SlotTokenClaimSuccess"},
    {"ShardXferTokenClaimSuccess"},
    {"MultiTransactionSuccess"},
    {"GenericTransactionSuccess"},
};
STATIC_ASSERT(ARRAY_SIZE(requestSuccessMsgs) == kAccountRequestType_Count);

static CLinkedList<AccountRequest*> activeRequests[kAccountRequestState_Count];
static CLinkedList<AccountRequest*> deleteRequests;
static StashTable activeRequestsIndex = NULL;

void AccountRequest::Init() {
    activeRequestsIndex = stashTableCreateFixedSize(ACCOUNT_INITIAL_CONTAINER_SIZE, sizeof(OrderId));
}

void AccountRequest::Shutdown() {
    // fail all outstanding requests
    for (int i=0; i<kAccountRequestState_Count; i++)
        FOREACH_LINK_SAFE(AccountRequest, request, next, activeRequests[i], tickList)
            request->Complete(false);

    ProcessPendingDeletes();

    for (int i=0; i<kAccountRequestState_Count; i++)
        devassert(activeRequests[i].empty());

    stashTableDestroy(activeRequestsIndex);
    activeRequestsIndex = NULL;
}

AccountRequest* AccountRequest::Find(OrderId order_id)
{
    AccountRequest *request = NULL;
    if (!stashFindPointer(activeRequestsIndex, &order_id, (void**)&request))
        return NULL;
    return request;
}

int AccountRequest::cmp(const AccountRequest &a, const AccountRequest &b) {
    return a.nextTick - b.nextTick;
}

int AccountRequest::cmp(const CLink<AccountRequest*> &a, const CLink<AccountRequest*> &b) {
    return tickListCompare(a, b, cmp);
}

void AccountRequest::ReSort(U32 newTick) {
    tickList.unlink();

    nextTick = newTick;

    activeRequests[reqState].addSortedNearTail(tickList, cmp);
}

bool AccountRequest::AddSorted() {
    if (orderIdIsNull(order_id))
        return false;

    if (!stashAddPointer(activeRequestsIndex, &order_id, this, false))
        return false;

    activeRequests[reqState].addSortedNearTail(this->tickList, cmp);
    return true;
}

bool AccountRequest::Remove(OrderId order_id) {
    AccountRequest *request = NULL;
    if (!stashRemovePointer(activeRequestsIndex, &order_id, (void**)&request))
        return false;

    request->tickList.unlink();
    return true;
}

AccountServerShard* AccountRequest::GetShard() {
    AccountServerShard *shard = AccountServer_GetShard(shard_id);
    if (!devassert(shard))
    {
        Complete(false);
        return NULL;
    }

    if (!SAFE_MEMBER(shard, link.connected))
    {
        Complete(false);
        return NULL;
    }

    return shard;
}

AccountRequest::AccountRequest() :
    nextTick(0),
    owner(NULL),
    product(NULL),
    reqType(kAccountRequestType_Count),
    flags(0),
    reqState(kAccountRequestState_Count),
    order_id(kOrderIdInvalid),
    shard_id(0),
    dbid(0),
    sku_id(kSkuIdInvalid),
    quantity(0),
    completed(false)
{
}

AccountRequest* AccountRequest::Allocate(AccountRequestType reqType)
{
    AccountRequest *request = NULL;

    switch (reqType) {
        xcase kAccountRequestType_CertificationGrant:
            request = new RequestCertificationGrant();
        xcase kAccountRequestType_CertificationClaim:
            request = new RequestCertificationClaim();
        xcase kAccountRequestType_Rename:
            request = new RequestRename();
        xcase kAccountRequestType_Respec:
            request = new RequestRespec();
        xcase kAccountRequestType_Slot:
            request = new RequestSlot();
        xcase kAccountRequestType_ShardTransfer:
            request = new RequestShardTransfer();
        xcase kAccountRequestType_MultiTransaction:
            request = new RequestMultiTransaction();
        xdefault:
            assertmsgf(0, "unknown request type '%d'", reqType);
    }

    return request;
}

OrderId AccountRequest::New(Account *account, AccountRequestType reqType, AccountRequestFlags flags, U8 shard_id, U32 dbid, SkuId sku_id, int quantity, const void *extraData) {
    if (!devassert(account))
        return kOrderIdInvalid;
    if (!devassert(shard_id))
        return kOrderIdInvalid;

    AccountRequest *request = Allocate(reqType);
    request->owner = account;
    request->reqType = reqType;
    request->flags = flags;
    request->reqState = kAccountRequestState_Charge;
    request->shard_id = shard_id;
    request->dbid = dbid;
    request->sku_id = sku_id;
    request->quantity = quantity;

    if (!request->GetShard())
    {
        delete request;
        return kOrderIdInvalid;
    }

    if (!request->Start(extraData) || !request->ValidateProduct() || !request->ValidateTransaction()) {
        delete request;
        return kOrderIdInvalid;
    }

    request->SubmitTransaction();

    if (!devassert(request->AddSorted())) {
        delete request;
        return kOrderIdInvalid;
    }

    request->ChangeState(kAccountRequestState_Charge);
    return request->order_id;
}

AccountRequest* AccountRequest::Recover(OrderId order_id, Account *account, AccountRequestType reqType, AccountRequestFlags flags, U8 shard_id, U32 dbid, SkuId sku_id, int quantity, const void *extraData)
{
    if (!devassert(!orderIdIsNull(order_id)))
        return NULL;
    if (!devassert(account))
        return NULL;
    if (!devassert(shard_id))
        return NULL;

    AccountRequest *request = Allocate(reqType);
    request->owner = account;
    request->reqType = reqType;
    request->flags = flags;
    request->reqState = kAccountRequestState_Validate;
    request->order_id = order_id;
    request->shard_id = shard_id;
    request->dbid = dbid;
    request->sku_id = sku_id;
    request->quantity = quantity;

    if (!request->Start(extraData) || !request->ValidateProduct()) {
        delete request;
        return NULL;
    }

    if (!devassert(request->AddSorted())) {
        delete request;
        return NULL;
    }

    request->ChangeState(kAccountRequestState_Validate);
    return request;
}

void AccountRequest::State_Charge(bool timeout) {
    if (timeout)
        Complete(false);
}

void AccountRequest::State_Validate(bool timeout) {
    if (timeout)
        Complete(false);
    else
        ChangeState(kAccountRequestState_Fulfill);
}

static bool accountRequest_Complete_tookADump = false;

void AccountRequest::Complete(bool success, const char *msg) {
    if (!devassert(!completed)) {
        if (!accountRequest_Complete_tookADump) {
            accountRequest_Complete_tookADump = true;
            takeADump();
        }
        return;
    }

    if (!success && !orderIdIsNull(order_id)) {
        // undo the change
        RevertTransaction();
    }

    if (!msg) {
        if (success)
            msg = requestSuccessMsgs[reqType];
        else
            msg = requestFailMsgs[reqType][reqState];
    }

    AccountServer_NotifyRequestFinished(owner, reqType, order_id, success, msg);

    if (Remove(order_id))
        this->tickList.linkTail(deleteRequests);        

    completed = true;
}

void AccountRequest::OnTransactionCompleted(OrderId order_id, bool success) {
    AccountRequest *request = Find(order_id);
    if (!request)
        return;

    if (!devassert(orderIdEquals(request->order_id, order_id)))
        return;

    if (success) {
        if (!devassert(request->reqState == kAccountRequestState_Charge))
            return;

        request->ChangeState(kAccountRequestState_Validate);
    } else {
        request->Complete(false);
    }
}

void AccountRequest::RunState(bool timeout) {
    // C based virtual function
    switch (reqState) {
        xcase kAccountRequestState_Charge:
            State_Charge(timeout);
        xcase kAccountRequestState_Validate:
            State_Validate(timeout);
        xcase kAccountRequestState_Fulfill:
            State_Fulfill(timeout);
        xdefault:
            assertmsgf(0, "unknown request state '%d'", reqState);
    }
}

void AccountRequest::UpdateNextTick()
{
    U32 newTick = timerSecondsSince2000();
    
    switch (reqState) {
        xcase kAccountRequestState_Charge: newTick += CHARGE_TIMEOUT;
        xcase kAccountRequestState_Validate: newTick += VALIDATE_TIMEOUT;
        xcase kAccountRequestState_Fulfill: newTick += FULFILL_TIMEOUT;
    }

    ReSort(newTick);
}

void AccountRequest::ChangeState(AccountRequestState newState) {
    reqState = newState;

    // tick the new state immdiately
    RunState(false);

    // setup the next tick time and place us in the correct position in the list
    if (reqState == newState)
        UpdateNextTick();
}

void AccountRequest::del(void *request) {
    delete reinterpret_cast<AccountRequest*>(request);
}

void AccountRequest::Tick() {
    U32 now = timerSecondsSince2000();

    for (int i=0; i<kAccountRequestState_Count; i++) {
        FOREACH_LINK_SAFE(AccountRequest, request, next, activeRequests[i], tickList) {
            if (now < request->nextTick)
                break;

            assert(request->owner);    

            // record the original state
            AccountRequestState origState = request->reqState;

            request->RunState(true);

            // force a identity state switch to reset the timer
            if (origState == request->reqState)
                request->UpdateNextTick();
        }
    }

    ProcessPendingDeletes();
}
void AccountRequest::ProcessPendingDeletes()
{
    FOREACH_LINK_SAFE(AccountRequest, request, next, deleteRequests, tickList)
        AccountRequest::del(request);
}

bool AccountRequest::ValidateProduct() {
    product = accountCatalogGetProduct(sku_id);
    if (!product) {
        Complete(false);
        return false;
    }

    return true;
}

bool AccountGrantRequest::ValidateTransaction() {
    AccountInventory *inv = AccountInventorySet_Find(&owner->invSet, sku_id);
    int inv_quantity = (inv ? inv->granted_total : 0);
    if (product->grantLimit && !(inv_quantity + quantity <= product->grantLimit)) {
        Complete(false);
        return false;
    }

    return true;
}

void AccountGrantRequest::SubmitTransaction() {
    order_id = Transaction_GamePurchaseBySkuId(owner, sku_id, shard_id, dbid, quantity, (flags & ACCOUNTREQUEST_CSR) != 0);
}

void AccountGrantRequest::RevertTransaction() {
    Transaction_GameRevert(owner, order_id);
}

bool AccountClaimRequest::ValidateTransaction() {
    AccountInventory *inv = AccountInventorySet_Find(&owner->invSet, sku_id);
    if (!inv ||
        !(inv->claimed_total + quantity <= inv->granted_total) ||
        !(inv->claimed_total + quantity >= inv->saved_total))
    {
        Complete(false);
        return false;
    }

    return true;
}

void AccountClaimRequest::SubmitTransaction() {
    order_id = Transaction_GameClaimBySkuId(owner, sku_id, shard_id, dbid, quantity, (flags & ACCOUNTREQUEST_CSR) != 0);
}

void AccountClaimRequest::RevertTransaction() {
    Transaction_GameRevert(owner, order_id);
}
