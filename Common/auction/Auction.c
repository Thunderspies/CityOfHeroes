/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include "auction/Auction.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>

// AuctionInvItemStatusEnum //////////////////////////////////////////////////

StaticDefineInt AuctionInvItemStatusEnum [] =
{
    DEFINE_INT
    {"None",AuctionInvItemStatus_None},
    {"Stored",AuctionInvItemStatus_Stored}, 
    {"ForSale",AuctionInvItemStatus_ForSale},
    {"Sold",AuctionInvItemStatus_Sold},
    {"Bidding",AuctionInvItemStatus_Bidding},
    {"Bought",AuctionInvItemStatus_Bought},
    DEFINE_END
};
STATIC_ASSERT(ARRAY_SIZE(AuctionInvItemStatusEnum)==AuctionInvItemStatus_Count+2);
bool auctioninvitemstatus_Valid( AuctionInvItemStatus e )
{
    return INRANGE0(e, AuctionInvItemStatus_Count);
}
 
const char *AuctionInvItemStatus_ToStr(AuctionInvItemStatus status)
{
    return StaticDefineIntRevLookup(AuctionInvItemStatusEnum,status);    
}


// AuctionInvItem ////////////////////////////////////////////////////////////

TokenizerParseInfo parse_AuctionInvItem[] = 
{
    { "AucInvItem",        TOK_START,                                    0},
    { "ID",                TOK_INT(AuctionInvItem,id,0),                0},
    { "Item",            TOK_POOL_STRING|TOK_STRING(AuctionInvItem,pchIdentifier,0),    0},
    { "Status",            TOK_INT(AuctionInvItem,auction_status,0),    AuctionInvItemStatusEnum},
    { "StoredCount",    TOK_INT(AuctionInvItem,amtStored,0),        0},
    { "StoredInf",        TOK_INT(AuctionInvItem,infStored,0),        0},
    { "OtherCount",        TOK_INT(AuctionInvItem,amtOther,0),            0},
    { "Price",            TOK_INT(AuctionInvItem,infPrice,0),            0},
    { "MapSideIdx",        TOK_INT(AuctionInvItem,iMapSide,0),            0},
    { "DeleteMe",        TOK_BOOL(AuctionInvItem,bDeleteMe,false),    0},
    { "MergedBid",        TOK_BOOL(AuctionInvItem,bMergedBid,false),    0},
    { "CancelledCount",    TOK_INT(AuctionInvItem,amtCancelled,0),        0},
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

static AuctionInvItem* AuctionInvItem_CreateRaw(void)
{
    AuctionInvItem *res = StructAllocRaw(sizeof( AuctionInvItem ));
    assert(res);
    return res;
}

AuctionInvItem* AuctionInvItem_Create(char *pchIdentifier)
{
    AuctionInvItem *res = AuctionInvItem_CreateRaw();
    res->pchIdentifier = allocAddString(pchIdentifier);
    return res;
}

void AuctionInvItem_Destroy( AuctionInvItem *hItem )
{
    if(hItem)
        StructDestroy(parse_AuctionInvItem,hItem);
}

bool AuctionInvItem_ToStr(AuctionInvItem *itm, char **hestr)
{
    if(!itm||!hestr)
        return FALSE;
    if(*hestr)
        estrClear(hestr);
    return ParserWriteTextEscaped(hestr,parse_AuctionInvItem,itm,0,0);
}

bool AuctionInvItem_FromStr(AuctionInvItem **hitm, char *str)
{
    if(!hitm || !str)
        return false;
    if(!*hitm)
        *hitm = StructAllocRaw(sizeof(**hitm));
    return ParserReadTextEscaped(&str,parse_AuctionInvItem,*hitm);
} 

AuctionInvItem* AuctionInvItem_Clone(AuctionInvItem **hdest, AuctionInvItem *itm)
{
    if(!itm||!hdest)
        return NULL;
    if(!(*hdest))
        (*hdest) = AuctionInvItem_CreateRaw();
    if(!StructCopyAll(parse_AuctionInvItem,itm,sizeof(*itm),*hdest))
    {
        AuctionInvItem_Destroy(*hdest);
        (*hdest) = NULL;
    }
    return *hdest;
}

TokenizerParseInfo parse_AuctionInventory[] = 
{
    { "InventorySize",    TOK_INT(AuctionInventory,invSize,0),                        0},
    { "Items",            TOK_STRUCT(AuctionInventory,items,parse_AuctionInvItem),    0},
    { "End",            TOK_END,                                                    0},
    { "", 0, 0 }
};

TokenizerParseInfo ParseAuctionConfig[] =
{
    { "AuctionConfig",    TOK_IGNORE, 0 },
    { "{",                TOK_START,  0 },
    { "SellFeePercent",    TOK_F32(AuctionConfig, fSellFeePercent, 0.0f)     },
    { "BuyFeePercent",    TOK_F32(AuctionConfig, fBuyFeePercent, 0.0f)     },
    { "MinFee",            TOK_INT(AuctionConfig, iMinFee, 0.0f)     },
    { "}",                TOK_END,    0 },
    { "", 0, 0 }
};

SHARED_MEMORY AuctionConfig g_AuctionConfig = {0};

void AuctionInventory_Send(AuctionInventory *inv, Packet *pak)
{
     char *s = NULL;
    AuctionInventory_ToStr(inv,&s);
    pktSendString(pak,s);
    estrDestroy(&s);
}

void AuctionInventory_Recv(AuctionInventory **hinv, Packet *pak)
{
    char *s;
    
    if(!devassert(hinv) || !pak)
        return;
    
    if( !(*hinv) )
        (*hinv) = AuctionInventory_Create();
    s = pktGetString(pak);
    AuctionInventory_FromStr(hinv,s);
}

bool auctionservertype_Valid( AuctionServerType e )
{
    return (e == kAuctionServerType_Heroes) ? true : false;
}

StaticDefineInt AuctionServerTypeEnum[] =
{
    DEFINE_INT
    {"Heroes/Villains",        kAuctionServerType_Heroes},
    {"Deprecated",            kAuctionServerType_Villains},
    DEFINE_END
};
STATIC_ASSERT(ARRAY_SIZE( AuctionServerTypeEnum ) == kAuctionServerType_Count+2);

const char *AuctionServerType_ToStr( AuctionServerType s)
{
    return StaticDefineIntRevLookup(AuctionServerTypeEnum,s); 
}

AuctionInventory* AuctionInventory_Create(void)
{
    AuctionInventory *res = NULL;
    res = StructAllocRaw(sizeof( AuctionInventory ));
    if( devassert( res ))
    {
        //res->invSize = AUCTION_INV_DEFAULT_SIZE;
    }
    return res;
}

void AuctionInventory_Destroy( AuctionInventory *hItem )
{
    if(hItem)
        StructDestroy(parse_AuctionInventory,hItem);
}


void setAuctionHistoryItemDetail(AuctionHistoryItemDetail *dest, char *buyer, char *seller, U32 date, int price)
{
    if(dest->buyer)
        StructFreeString(dest->buyer);
    dest->buyer = buyer ? StructAllocString(buyer) : NULL;
    if(dest->seller)
        StructFreeString(dest->seller);
    dest->seller = seller ? StructAllocString(seller) : NULL;
    dest->date = date;
    dest->price = price;
}


AuctionHistoryItemDetail **allocatedHistoryItemDetails = NULL;

AuctionHistoryItemDetail *newAuctionHistoryItemDetail(char *buyer, char *seller, U32 date, int price)
{
    AuctionHistoryItemDetail *ret = NULL;

    if ( allocatedHistoryItemDetails && eaSize(&allocatedHistoryItemDetails) )
        ret = eaPop(&allocatedHistoryItemDetails);
    else
        ret = StructAllocRaw(sizeof(AuctionHistoryItemDetail));

    if ( ret )
        setAuctionHistoryItemDetail(ret, buyer, seller, date, price);

    return ret;
}

void freeAuctionHistoryItemDetail(AuctionHistoryItemDetail *itm)
{
    if ( itm )
    {
        if(itm->buyer)
            StructFreeString(itm->buyer);
        if(itm->seller)
            StructFreeString(itm->seller);
        eaPush(&allocatedHistoryItemDetails, itm);
    }
}

AuctionHistoryItem *newAuctionHistoryItem(const char *pchIdentifier)
{
    AuctionHistoryItem *ret = NULL;
    ret = StructAllocRaw(sizeof(AuctionHistoryItem));
    ret->pchIdentifier = allocAddString(pchIdentifier);
    return ret;
}

void freeAuctionHistoryItem(AuctionHistoryItem *itm)
{
    if(!itm)
        return;

    eaDestroyEx(&itm->histories,freeAuctionHistoryItemDetail);
    StructFree(itm);
}

void AuctionInventory_ToStr(AuctionInventory *inv, char **hestr)
{
    if(!inv||!hestr)
        return;
    ParserWriteTextEscaped(hestr,parse_AuctionInventory,inv,0,0);
}

void AuctionInventory_FromStr(AuctionInventory **hinv,char *str)
{
    if(!str||!hinv)
        return;
    if(*hinv)
        AuctionInventory_Destroy(*hinv);
    *hinv = AuctionInventory_Create();
    ParserReadTextEscaped(&str,parse_AuctionInventory,(*hinv));
}

INT64 calcAuctionSellFee(INT64 price)
{
    return max(g_AuctionConfig.iMinFee, price * g_AuctionConfig.fSellFeePercent);
}

int calcAuctionSoldFee(int price, int origPrice, int amt, int amtCancelled)
{
    INT64 fee = price * g_AuctionConfig.fBuyFeePercent - (calcAuctionSellFee(origPrice) * amt);
    if (price)
    {
        fee = max(g_AuctionConfig.iMinFee, fee);
    }

    if (amtCancelled)
    {
        fee -= calcAuctionSellFee(origPrice) * amtCancelled;
    }

    return fee;
}
