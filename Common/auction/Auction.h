/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#ifndef AUCTION_H
#define AUCTION_H

#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/utils/textparser.h>
#include "basetsd.h"

typedef struct Packet Packet;
typedef struct AuctionEnt AuctionEnt;


#define AUCTION_BUY_SELL_HISTORY_LENGTH 32
#define AUCTION_INV_MAX_SIZE 32
#define AUCTION_INV_DEFAULT_SIZE 2

#define AUCTION_AUTO_FULFILL_MIN_TIME    600
#define AUCTION_AUTO_SELL_MIN_PRICE        10000
#define AUCTION_AUTO_BUY_MIN_PRICE        10000
#define AUCTION_MAX_INFLUENCE            2000000000

typedef enum AuctionServerType
{
    kAuctionServerType_Heroes,
    kAuctionServerType_Villains,
    kAuctionServerType_Count,
    kAuctionServerType_Invalid = -1,
} AuctionServerType;

const char *AuctionServerType_ToStr( AuctionServerType s);
extern StaticDefineInt AuctionServerTypeEnum[];

bool auctionservertype_Valid( AuctionServerType e );

typedef enum AuctionInvItemStatus
{    
    AuctionInvItemStatus_None,
    AuctionInvItemStatus_Stored,  // they have placed the item in the auction inventory, but not put it up for sale yet
    AuctionInvItemStatus_ForSale,
    AuctionInvItemStatus_Sold,
    AuctionInvItemStatus_Bidding,
    AuctionInvItemStatus_Bought,
    AuctionInvItemStatus_Count
} AuctionInvItemStatus;

extern StaticDefineInt AuctionInvItemStatusEnum [];
bool auctioninvitemstatus_Valid( AuctionInvItemStatus e );
const char *AuctionInvItemStatus_ToStr(AuctionInvItemStatus status);

typedef struct AuctionInvItem
{
    int id;    // unique to an AuctionEnt
    const char *pchIdentifier;    // serialized TrayItemIdentifier, in string pool

    AuctionInvItemStatus auction_status;
    int amtCancelled;    // number of cancelled items (refund sellers posting fee)
    int amtStored;        // number of actual items
    int infStored;        // actual influence
    int amtOther;        // remaining bid count or number sold
    int infPrice;        // minimum sell price or bid, per-item
    
    // TextParser knows about these, but AuctionServer defaults them before persisting
    int iMapSide;        // identifier (e.g. tray slot), used during AddInv
    bool bDeleteMe;        // during Xaction finalization, alert the mapserver that the item is being deleted
    bool bMergedBid;    // bids that were made pre-merger and thus invalid

    // not persisted below
#if AUCTIONSERVER
    AuctionEnt *ent;
    int lockid;
    int subid;
    U32 autofulfilltime;
#endif
} AuctionInvItem;



AuctionInvItem* AuctionInvItem_Create(char *pchIdentifier);
void AuctionInvItem_Destroy( AuctionInvItem *hItem );
bool AuctionInvItem_ToStr(AuctionInvItem *itm, char **hestr);
bool AuctionInvItem_FromStr(AuctionInvItem **hitm, char *str);
AuctionInvItem* AuctionInvItem_Clone(AuctionInvItem **hdest, AuctionInvItem *itm);

#define MAX_AUCTION_STACK_SIZE 10

#define AUCTION_HISTORY_SIZE 5
typedef struct AuctionHistoryItem AuctionHistoryItem;

typedef struct AuctionHistoryItemDetail
{
    char *buyer;
    char *seller;
    U32 date; // SS2000
    int price;
} AuctionHistoryItemDetail;

typedef struct AuctionHistoryItem
{
    const char *pchIdentifier;    // serialized TrayItemIdentifier, in string pool
    AuctionHistoryItemDetail **histories;
} AuctionHistoryItem;

typedef struct AuctionConfig
{
    F32 fSellFeePercent;
    F32 fBuyFeePercent;
    U32 iMinFee;
} AuctionConfig;

extern TokenizerParseInfo ParseAuctionConfig[];
extern SHARED_MEMORY AuctionConfig g_AuctionConfig;


typedef struct AuctionInventory
{
    AuctionInvItem **items;
    int invSize;

    // auction server only
    AuctionEnt *owner;
} AuctionInventory;

void AuctionInventory_ToStr(AuctionInventory *inv, char **hestr);
void AuctionInventory_FromStr(AuctionInventory **hinv,char *str);

// auction history stuff
void setAuctionHistoryItemDetail(AuctionHistoryItemDetail *dest, char *buyer, char *seller, U32 date, int price);
AuctionHistoryItemDetail *newAuctionHistoryItemDetail(char *buyer, char *seller, U32 date, int price);
void freeAuctionHistoryItemDetail(void* itmData);
void freeAuctionHistoryItem(AuctionHistoryItem *itm);
AuctionHistoryItem *newAuctionHistoryItem(const char *pchIdentifier);

extern ParseTable parse_AuctionInventory[];
extern ParseTable parse_AuctionInvItem[];

void AuctionInventory_Send(AuctionInventory *inv, Packet *pak);
void AuctionInventory_Recv(AuctionInventory **hinv, Packet *pak);

AuctionInventory* AuctionInventory_Create(void);
void AuctionInventory_Destroy( AuctionInventory *hItem );

INT64 calcAuctionSellFee(INT64 price);
int calcAuctionSoldFee(int price, int origPrice, int amt, int amtCancelled);

#endif //AUCTION_H
