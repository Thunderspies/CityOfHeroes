#ifndef AUCTIONDATA_H
#define AUCTIONDATA_H

#include <utilitieslib/components/StashTable.h>
#include "gameComm/trayCommon.h"
#include "auction/Auction.h"

void auctiondata_Init(void);

typedef struct SalvageItem SalvageItem;
typedef struct BasePower BasePower;
typedef struct DetailRecipe DetailRecipe;

typedef struct AuctionItem
{
    int id;

    char * pchIdentifier;
    char * sortName;
    const char * displayName;
    TrayItemType type;

    union
    {
        void *pItem;
        SalvageItem *salvage;
        BasePower *enhancement;
        BasePower *inspiration;
        BasePower *temp_power;
        const DetailRecipe *recipe;
    };

    int lvl;
    int numForSale;
    int numForBuy;
    int buyPrice;
    int playerType;

    /*** these correspond to the values in AuctionInvItem */
    /**/    int auction_id;
    /**/    AuctionInvItemStatus auction_status;
    /**/    int amtCancelled;
    /**/    int amtStored;
    /**/    int infStored;
    /**/    int amtOther;
    /**/    int infPrice;
    /**/    bool bMergedBid;
    /******************************************************/

} AuctionItem;

typedef struct AuctionItemDict
{
    AuctionItem **ppItems;
    StashTable stAuctionItems;
} AuctionItemDict;

extern AuctionItemDict g_AuctionItemDict;

static INLINEDBG AuctionItemDict *auctionData_GetDict(void)
{
    if ( !g_AuctionItemDict.ppItems )
        auctiondata_Init();
    return &g_AuctionItemDict;
}

char* auction_identifierFromItem(AuctionItem * item, TrayItemType type, int level );
char* auction_identifier( int type, const char *name, int level );

int auctionData_GetID(int type, const char *name, int level);

bool IsAttunedEnhancementAllowedInAuctionHouse(BasePower *ppow, const int level);
bool IsAuctionItemValid(AuctionItem *pItem);
bool DoesAuctionItemPassAuctionRequiresCheck(AuctionItem *pItem, Character *pchar);

#endif