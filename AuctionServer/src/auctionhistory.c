/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include "auctionhistory.h"
#include "auction/Auction.h"
#include "BinHeap.h"
#include "XactServer.h"
#include "AuctionDb.h"
#include <utilitieslib/components/StringCache.h>
#include "AuctionServer.h"
#include "auction/Auction.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/EString.h>
#include "AsyncFileWriter.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/StashTable.h>
#include "persist/textcrcdb.h"
#include <utilitieslib/utils/utils.h>
#include "AuctionSql.h"
#include <utilitieslib/utils/log.h>

#define AUCTION_HISTORY_INITIAL_CAPACITY 50000

StashTable g_AuctionHistory = NULL;
static AsyncFileWriter *fileWriter = NULL;
static int fileWriteTimer = 0;
static char *historyString = NULL;

TokenizerParseInfo parse_AuctionItemHistoryDetail[] = 
{
    { "Buyer",        TOK_STRING(AuctionHistoryItemDetail,buyer,0),    0},
    { "Seller",        TOK_STRING(AuctionHistoryItemDetail,seller,0),    0},
    { "Date",        TOK_INT(AuctionHistoryItemDetail,date,0),        0,    TOK_FORMAT_DATESS2000},
    { "Price",        TOK_INT(AuctionHistoryItemDetail,price,0),        0}, 
    { "End",        TOK_END,            0},
    { "", 0, 0 }
};

TokenizerParseInfo parse_AuctionItemHistory[] = 
{
    { "Item",            TOK_POOL_STRING|TOK_STRING(AuctionHistoryItem,pchIdentifier,0),                0},
    { "Histories",        TOK_STRUCT(AuctionHistoryItem,histories,parse_AuctionItemHistoryDetail),            0},
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

// this struct is only used to load the auction history to/from a disk
typedef struct LoadedAuctionHistory
{
    AuctionHistoryItem **loadedHistItems;
    U32 uHistoryVer;
} LoadedAuctionHistory;

static LoadedAuctionHistory s_loadedHist = {0};

void auctionhistory_AddInv(AuctionInvItem *itm, char *buyer, char *seller)
{
    AuctionHistoryItem *histItm = NULL;
    AuctionHistoryItemDetail *detail = NULL;
    int iHistory;
    char *estr_history = NULL;
    bool executed;

    if(!devassert(itm && buyer && seller))
        return;

    if ( !stashFindPointer(g_AuctionHistory, itm->pchIdentifier, &histItm) )
    {
        // create new AuctionHistoryItem
        histItm = newAuctionHistoryItem(itm->pchIdentifier);
        eaPush(&s_loadedHist.loadedHistItems, histItm);
        stashAddPointer(g_AuctionHistory, histItm->pchIdentifier, histItm, 0);
    }

#if AUCTION_HISTORY_SIZE > 0
    iHistory = eaSize(&histItm->histories);
    if(iHistory == AUCTION_HISTORY_SIZE)
    {
        detail = histItm->histories[AUCTION_HISTORY_SIZE-1];
        eaSetSize(&histItm->histories, AUCTION_HISTORY_SIZE-1);
    }
    else if(iHistory > AUCTION_HISTORY_SIZE)
        printf("Leaking Memory, Auction History Too Big\n");

    if(detail)
        setAuctionHistoryItemDetail(detail, buyer, seller, timerSecondsSince2000(), itm->infPrice);
    else 
        detail = newAuctionHistoryItemDetail(buyer, seller, timerSecondsSince2000(), itm->infPrice);

    if(!histItm->histories)
    {
        eaCreate(&histItm->histories);
        eaSetCapacity(&histItm->histories,AUCTION_HISTORY_SIZE);
    }
    eaPushFront(&histItm->histories, detail);

    ParserWriteText(&estr_history,parse_AuctionItemHistory,histItm,0,0);
    executed = aucsql_insert_or_update_history(histItm->pchIdentifier, &estr_history);
    devassert(executed);
    estrDestroy(&estr_history);
#endif
}

AuctionHistoryItem *auctionhistory_Get(const char *pchIdentifier)
{
    AuctionHistoryItem *histItm = NULL;
    if(!g_AuctionHistory || !stashFindPointer(g_AuctionHistory, pchIdentifier, &histItm))
        return NULL;
    return histItm;
}

void AuctionHistory_Load(void)
{
    int i;
    U32 pti_crc;
    if (s_loadedHist.loadedHistItems)
    {
        FatalErrorf("AuctionHistory_Load called twice");
    }

    if (!aucsql_read_history_ver(&s_loadedHist.uHistoryVer))
    {
        FatalErrorf("Could not read version number from AuctionServer SQL DB! Please see console/logs.");
    }

    pti_crc = ParseTableCRC(parse_AuctionItemHistory, NULL);
    if(s_loadedHist.uHistoryVer != pti_crc)
    {
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: History had its parse table changed, ignoring crc.  This is normal for a new publish.");
        printf("Updating parse table version for history.\n");
        aucsql_write_history_ver(pti_crc);
    }

    if(!aucsql_read_history(parse_AuctionItemHistory, &s_loadedHist.loadedHistItems))
    {
        FatalErrorf("History table could not be read! Could not load. Please see console/logs.");
    }

    devassert(!g_AuctionHistory);
    g_AuctionHistory = stashTableCreateWithStringKeys(AUCTION_HISTORY_INITIAL_CAPACITY, 0);

    for ( i = 0; i < eaSize(&s_loadedHist.loadedHistItems); ++i )
    {
        AuctionHistoryItem *histItm = s_loadedHist.loadedHistItems[i];
        stashAddPointer(g_AuctionHistory, histItm->pchIdentifier, histItm, 0);
    }
}
