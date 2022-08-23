/*********************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include "auctionfulfill.h"
#include "BinHeap.h"
#include "XactServer.h"
#include "AuctionDb.h"
#include <utilitieslib/components/StringCache.h>
#include "AuctionServer.h"
#include "auction/Auction.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include "auctionhistory.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/log.h>
// *********************************************************************************
//  Auction fulfillment
// *********************************************************************************

typedef struct AucFillCat
{
    char const *pchIdentifier;
    BinHeap *buying;
     BinHeap *selling;
} AucFillCat;

static AucFillCat **s_AuctionCats;
static StashTable s_AuctionCatStash;

int auctionfullfiller_CatsCount(void)
{
    return eaSize(&s_AuctionCats);
}


int auctioninvitm_BinHeapCmpBuying(void *lhs, void *rhs)
{
    AuctionInvItem *a = lhs;
    AuctionInvItem *b = rhs;
    if(stricmp(a->pchIdentifier,b->pchIdentifier))
        FatalErrorf("Mismatched items in Buying heap:\n%s\n%s",a->pchIdentifier,b->pchIdentifier);
    return b->infPrice - a->infPrice;
}

int auctioninvitm_BinHeapCmpSelling(void *lhs, void *rhs)
{
    AuctionInvItem *a = lhs;
    AuctionInvItem *b = rhs;
    if(stricmp(a->pchIdentifier,b->pchIdentifier))
        FatalErrorf("Mismatched items in Selling heap:\n%s\n%s",a->pchIdentifier,b->pchIdentifier);
    return a->infPrice - b->infPrice;
}

MP_DEFINE(AucFillCat);
static AucFillCat* AucFillCat_Create(char const *pchIdentifier)
{
    AucFillCat *res = NULL;

    pchIdentifier = allocAddString(pchIdentifier);

    // create the pool, arbitrary number
    MP_CREATE(AucFillCat, 64);
    res = MP_ALLOC( AucFillCat );
    if( verify( res ))
    {
        res->pchIdentifier = pchIdentifier;
        res->selling = binheap_Create(auctioninvitm_BinHeapCmpSelling);
        res->buying = binheap_Create(auctioninvitm_BinHeapCmpBuying);
    }
    return res;
}

static void AucFillCat_Destroy( AucFillCat *hItem )
{
    if(hItem)
        MP_FREE(AucFillCat, hItem);
}

BinHeap *AucFillCat_BinHeapForItm(AucFillCat *cat, AuctionInvItem *itm)
{
    if(!cat || !itm)
        return NULL;
    switch ( itm->auction_status )
    {
    case AuctionInvItemStatus_ForSale:
        return cat->selling;
    case AuctionInvItemStatus_Bidding:
        return cat->buying;
    default:
        return NULL;
    };
    return NULL;
}


void AucFillCat_AddInvItm( AucFillCat *cat, AuctionInvItem *itm )
{
    BinHeap *bh = NULL;
    
    if(!cat || !itm || itm->bMergedBid || !devassert(itm->ent) || !devassert(0==stricmp(cat->pchIdentifier,itm->pchIdentifier)))
        return;

    bh = AucFillCat_BinHeapForItm(cat,itm);
    if(bh)
    {
        if(devassert(-1==binheap_Find(bh,itm)))
            binheap_Push(bh,itm);
        else
            LOG( LOG_AUCTION_TRANSACTION, LOG_LEVEL_VERBOSE, 0, "in fulfiller tried to add itm twice: ptr(%p) ident(%s) owner(%s)",itm,itm->pchIdentifier,itm->ent->nameEnt);
    }
}


void AucFillCat_RemoveInvItm( AucFillCat *cat, AuctionInvItem *itm )
{
    BinHeap *bh = NULL;
    
    if(!cat || !itm 
       || !devassert(itm->ent)
       || !devassert(0==stricmp(cat->pchIdentifier,itm->pchIdentifier))) 
        return;

    binheap_RemoveElt(cat->buying,itm);
    binheap_RemoveElt(cat->selling,itm);
}

void auctionfulfiller_Init(void)
{
    s_AuctionCatStash = stashTableCreateWithStringKeys(AUC_NUM_TYPES,StashDefault);
}

AucFillCat *auctionfulfiller_GetCat(const char *pchIdentifier)
{
    AucFillCat *res;
    if(stashFindPointer(s_AuctionCatStash,pchIdentifier,&res))
        return res;
    return NULL;
}

AucFillCat *auctionfulfiller_AddCat(const char *pchIdentifier)
{
    AucFillCat *cat = auctionfulfiller_GetCat(pchIdentifier);
    if(!cat)
    {
        pchIdentifier = allocAddString(pchIdentifier);
        cat = AucFillCat_Create(pchIdentifier);
        eaPush(&s_AuctionCats,cat);
        stashAddPointer(s_AuctionCatStash,pchIdentifier,cat,true);
    }
    return cat;
}

void auctionfulfiller_AddInvItm(AuctionInvItem *itm)
{
    AucFillCat *cat = auctionfulfiller_AddCat(itm->pchIdentifier);
    if(devassert(!itm->lockid))
        AucFillCat_AddInvItm( cat, itm );
}

bool auctionfulfiller_InvItmIsInFulfiller(AuctionInvItem *itm)
{
    AucFillCat *cat;
    BinHeap *bh = NULL;
    if(!itm)
        return false;
    cat = auctionfulfiller_GetCat(itm->pchIdentifier);
    if(!cat)
        return false;
    return -1!=binheap_Find(cat->selling,itm)
        || -1!=binheap_Find(cat->buying,itm);
}


void auctionfulfiller_RemoveInvItm(AuctionInvItem *itm)
{
    AucFillCat *cat = auctionfulfiller_GetCat(itm->pchIdentifier);
    if(devassert(cat))
        AucFillCat_RemoveInvItm( cat, itm );
}

void auctionfulfiller_AddInv(AuctionInventory *inv)
{
    int i;
    for( i = eaSize(&inv->items) - 1; i >= 0; --i)
    {
        AuctionInvItem *itm = inv->items[i];
        if(itm)
            auctionfulfiller_AddInvItm(itm);
    }
}

void auctionfulfiller_RemoveInv(AuctionInventory *inv)
{
    int i;
    for( i = eaSize(&inv->items) - 1; i >= 0; --i)
    {
        AuctionInvItem *itm = inv->items[i];
        if(itm)
            auctionfulfiller_RemoveInvItm(itm);
    }
}


void auctionfulfiller_Tick(void)
{
    int i;
    for(i = eaSize(&s_AuctionCats)-1; i >= 0; --i)
    {
        int j;
        AucFillCat *cat = s_AuctionCats[i];
        AuctionInvItem *topbuyer;
        AuctionInvItem *topseller;
        AuctionInvItem **skipped_buyers = NULL;
        AuctionInvItem **skipped_sellers = NULL;

        for(;;)
        {
            topbuyer = binheap_Top(cat->buying);
            topseller = binheap_Top(cat->selling);

            // We need at least one party to do anything.
            if(!topbuyer && !topseller)
                break;

            // Can't buy if item is locked, is asking for zero, is not a bid, or was a bid pre-merger.
            if(topbuyer && (topbuyer->lockid || topbuyer->auction_status != AuctionInvItemStatus_Bidding || topbuyer->amtOther <= 0 || topbuyer->bMergedBid))
            {
                devassert(!"Bad topbuyer");
                LOG( LOG_AUCTION, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Fulfiller: removing invalid topbuyer \"%s\" lock:%i status:%i amount:%i",
                              topbuyer->pchIdentifier,topbuyer->lockid,topbuyer->auction_status,topbuyer->amtOther);
                auctionfulfiller_RemoveInvItm(topbuyer);
                topbuyer = NULL;
            }

            // Similarly, can't sell if item is locked, doesn't have at least one
            // still on sale and item isn't marked for sale.
            if(topseller && (topseller->lockid || topseller->auction_status != AuctionInvItemStatus_ForSale || topseller->amtStored <= 0))
            {
                devassert(!"Bad topseller");
                LOG( LOG_AUCTION, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Fulfiller: removing invalid topseller \"%s\" lock:%i status:%i amount:%i",
                              topseller->pchIdentifier,topseller->lockid,topseller->auction_status,topseller->amtStored);
                auctionfulfiller_RemoveInvItm(topseller);
                topseller = NULL;
            }

            // NOTE: Buyer heap is sorted by highest bid to lowest. Seller heap
            // is sorted by lowest asking price to highest. Thus the first from
            // each is the most optimal combination.

            // Need a buyer and a seller to make a regular transaction.
            if(topbuyer && topseller)
            {
                if(topbuyer->ent == topseller->ent && !topbuyer->ent->accesslevel)
                {
                    eaPush(&skipped_buyers,binheap_Pop(cat->buying));
                    //eaPush(&skipped_sellers,binheap_Pop(cat->selling));
                    // ab: let's try leaving the the sell, but invalidating the buy
                    continue;
                }

                if(topbuyer->infPrice >= topseller->infPrice)
                {
                    // fill the sale. NOTE: not handling sale to self. if you sell to yourself, you're an idiot. plus FFXI apparently lets you do that to drive price up. so: gameplay
                    int amt = MIN(topseller->amtStored,topbuyer->amtOther);
                    char *context = XactServer_CreateFulfillContext(cat->pchIdentifier,topseller,topbuyer,amt);
                    if(devassert(context))
                    {
                        XactServer_AddReq(XactCmdType_FulfillAuction,context,NULL,0,NULL);
                        estrDestroy(&context);

                        break; // only one request per cat at a time
                    }
                }
            }

            if (topbuyer || topseller)
            {
                // Seller wants more than buyer is offering, there are sellers but
                // no buyers, or vice versa. Can one of them be auto-fulfilled?
                int buyerFulfill = -1;
                int sellerFulfill = -1;
                U32 now = timerSecondsSince2000();

                // How much more than the auto-sell asking price will the buyer
                // pay, assuming they've waited long enough for a seller?
                if (topbuyer && !AuctionEnt_IsFakeAgent(topbuyer->ent) && topbuyer->autofulfilltime && topbuyer->autofulfilltime < now &&
                    topbuyer->infPrice >= g_auctionstate.cfg.iAutoBuyPrice)
                {
                    buyerFulfill = topbuyer->infPrice - g_auctionstate.cfg.iAutoBuyPrice;
                }

                // Similarly, how much less than the auto-buy bid will the seller
                // want, if they have waited long enough for buyers?
                if (topseller && !AuctionEnt_IsFakeAgent(topseller->ent) && topseller->autofulfilltime && topseller->autofulfilltime < now &&
                    topseller->infPrice <= g_auctionstate.cfg.iAutoSellPrice)
                {
                    sellerFulfill = g_auctionstate.cfg.iAutoSellPrice - topseller->infPrice;
                }

                // Buyer wins in a tie breaker.
                if (topbuyer && buyerFulfill >= sellerFulfill && buyerFulfill > -1)
                {
                    char *context = XactServer_CreateFulfillAutoSellContext(cat->pchIdentifier,topbuyer,topbuyer->amtOther);
                    if(devassert(context))
                    {
                        XactServer_AddReq(XactCmdType_FulfillAutoSell,context,NULL,0,NULL);
                        estrDestroy(&context);
                    }
                }
                else if (topseller && sellerFulfill > -1)
                {
                    char *context = XactServer_CreateFulfillAutoBuyContext(cat->pchIdentifier,topseller,topseller->amtStored);
                    if(devassert(context))
                    {
                        XactServer_AddReq(XactCmdType_FulfillAutoBuy,context,NULL,0,NULL);
                        estrDestroy(&context);
                    }
                }
            }

            break;  // only one request per cat at a time
        }
        for( j = eaSize(&skipped_buyers) - 1; j >= 0; --j)
            binheap_Push(cat->buying,skipped_buyers[j]);
        for( j = eaSize(&skipped_sellers) - 1; j >= 0; --j)
            binheap_Push(cat->selling,skipped_sellers[j]);        
        eaDestroy(&skipped_buyers);
        eaDestroy(&skipped_sellers);
    }
}

static void auctionfulfiller_GetItemInfoInternal(AucFillCat *cat, int *pBuying, int *pSelling)
{
    int buying = 0;
    int selling = 0;

    if(cat)
    {
        int i;
        for(i = binheap_Size(cat->buying)-1; i >= 0; --i)
            buying += ((AuctionInvItem*)cat->buying->elts[i])->amtOther;
        for(i = binheap_Size(cat->selling)-1; i >= 0; --i)
            selling += ((AuctionInvItem*)cat->selling->elts[i])->amtStored;
    }

    if(pBuying)
        *pBuying = buying;
    if(pSelling)
        *pSelling = selling;
}

void auctionfulfiller_GetItemInfo(const char *pchIdentifier, int *pBuying, int *pSelling)
{
    auctionfulfiller_GetItemInfoInternal(auctionfulfiller_GetCat(pchIdentifier),pBuying,pSelling);
}

void auctionfulfiller_GetItemInfoByIdx(int idx, const char **ppchIdentifier, int *pBuying, int *pSelling)
{
    AucFillCat *cat = eaGet(&s_AuctionCats,idx);
    if(ppchIdentifier)
        *ppchIdentifier = SAFE_MEMBER(cat,pchIdentifier);
    auctionfulfiller_GetItemInfoInternal(cat,pBuying,pSelling);
}
