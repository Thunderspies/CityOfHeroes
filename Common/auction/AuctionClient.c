/***************************************************************************
 *     Copyright (c) 2006-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 * $Workfile: $
 * $Revision: 1.36 $
 * $Date: 2006/02/07 06:13:59 $
 *
 * Module Description:
 *
 * Revision History:
 ***************************************************************************/
#include "AuctionClient.h"
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/utils/timing.h>
#include "dbcomm/logcomm.h"
#include <utilitieslib/components/EString.h>
#include "gameComm/trayCommon.h"
#include "Xact.h"
#include "entity/character_base.h"
#include "entity/character_combat.h"
#include "Auction.h"
//#include "entclient.h"
#include "entity/entPlayer.h"
#include "entity/entity.h"
#include "svr/svr_player.h"
#include "comm_backend.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include "entity/powers.h"
#include "comm_game.h"
#include "bases/DetailRecipe.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/character_net.h"
#include "AuctionData.h"
#include <utilitieslib/components/bitfield.h>
#include <utilitieslib/network/netcomp.h>
#include "entity/entGameActions.h"

#include "svr/svr_base.h" // START_PACKET and END_PACKET
#include "gameComm/sendToClient.h" // conPrintf
#include "entity/entserver.h" // entFromDbId
#include "language/langServerUtil.h"
#include "plaque.h" // piggy-backing on the plaque stuff for general purpose server to client dialog boxes
#include "gameComm/svr_chat.h" // chatSendToPlayer
#include "storyarc/mission.h"

#include "dbcomm/dbcontainer.h"
#include "dbcomm/dbmapxfer.h"

#include "gameData/mininghelper.h"
#include "player/badges_server.h"

static XactCmd **s_shardjumpers = NULL;

// TODO: these probably belong somewhere generally accessible
static char* s_AuctionItemToDisplayString(AuctionItem *pItem);
static char* s_IdentifierStringToDisplayString(const char *pchIdentifier);

extern NetLink db_comm_link;

Packet *ent_AuctionPktCreateEx(Entity *e, int cmd)
{
    Packet *pak;
    if(!e)
        return NULL;
    
    pak = pktCreateEx(&db_comm_link,cmd);
     pktSendBitsAuto(pak,e->db_id);
    pktSendString(pak,e->name);
    return pak;
}

Packet *ent_AuctionPktCreate(Entity *e, int cmd)
{
    if(!e || !e->pl)
        return NULL;

    return ent_AuctionPktCreateEx(e, cmd);
}


static void acLog(char const *cmdname,Entity *e, XactCmd *cmd, AuctionInvItem *itm, char *context, char const *fmt, ...)
{
    static char* str = NULL;
    char    *pname = "_UNKNOWN_";
    int        teamup_id = 0;
    va_list    ap;

    va_start(ap, fmt);
    estrClear(&str);
    if(cmd)
        estrConcatf(&str,"(xid=%i)", cmd->xid);
    else
        estrConcatStaticCharArray(&str, "(request)");
    estrConcatfv(&str, (char*)fmt, ap);
    if(itm)
        estrConcatf(&str," Item is '%s'", itm->pchIdentifier);
    if(!context && cmd)
        context = cmd->context;
    if(context)
        estrConcatf(&str,"Context:\"%s\"",context);
    va_end(ap);

    LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "%s %s", cmdname, str );
}

#define AUCTION_SPAM_ALLOWED (10)
#define AUCTION_SPAM_BAN_TIME (10)

void auctionRateLimiter(Entity *e)
{
    U32 time = dbSecondsSince2000();

    if( e->pl->lastAuctionMsgSent == 0 || e->pl->lastAuctionMsgSent <= time-10 )
    {
        e->pl->auctionSpams = AUCTION_SPAM_ALLOWED;
        e->pl->lastAuctionMsgSent = time;
    }
    else
        e->pl->auctionSpams--;

    if(e->pl->auctionSpams<0)
    {
        e->pl->auctionSpams = 0;
        e->auction_ban_expire = MAX( e->auction_ban_expire, (U32)(time + AUCTION_SPAM_BAN_TIME) );
    }
}

void sendAuctionBannedUpdateToClient(Entity *e)
{
    Packet *pak = NULL;
    START_PACKET(pak, e, SERVER_AUCTION_BANNED_UPDATE);
    pktSendBits(pak, 1, !(e->pl->auctionSpams > 0));
    END_PACKET; 
}



int auctionBanned(Entity *e, int notify)
{
    int     dt;
    dt = e->auction_ban_expire - dbSecondsSince2000();
    if (dt <= 0)
        e->auction_ban_expire = 0;
    else
    {
        char* message;
        int hour, min, sec;
        hour = dt / 3600;
        min = (dt % 3600) / 60;
        sec = (dt - hour*3600 - min*60);
        if( notify )
        {
            message = localizedPrintf(e, "BannedFromAuction");
    
            if (!e->pl->afk) // Prevent an infinite message bounce condition
            {
                chatSendToPlayer(e->db_id, message, INFO_GMTELL, 0);
                sendAuctionBannedUpdateToClient(e);
            }
        }
        return 1;
    }
    return 0;
}

int canUseAuction( Entity *e, int notify )
{
    if(e && e->pl && !auctionBanned(e, notify) && !OnMissionMap())
    {
        if( e->pl->at_auction || e->auc_trust_gameclient || accountLoyaltyRewardHasNode(e->pl->loyaltyStatus, "RemoteAuction") )
            return 1;
    }
    return 0;
}
// *********************************************************************************
// AuctionInvItemInfoCache
//
// Policy: a short term cache that exists to prevent spamming the AuctionServer with lots of
// duplicate requests, and to provide relatively accurate information to the player
// quickly. Once a cached item expires it allows only one new request to be sent to the
// AuctionServer. This assumes the request gets through and updates the cache properly.
// *********************************************************************************
typedef struct AuctionInvItemInfoCache
{
    const char *pchIdentifier;
    int buying;
    int selling;
    U32 last_update;
} AuctionInvItemInfoCache;
#define AUCTIONINVITEMSTATUS_CACHE_TIMEOUT (5) // cache timout
#define AUCTIONINVITEMSTATUS_CACHE_REQ_DELAY (30) // request retry timeout

MP_DEFINE(AuctionInvItemInfoCache);
static AuctionInvItemInfoCache* AuctionInvItemInfoCache_Create(const char *pchIdentifier)
{
    AuctionInvItemInfoCache *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(AuctionInvItemInfoCache, 64);
    res = MP_ALLOC( AuctionInvItemInfoCache );
    if( devassert( res ))
    {
        res->pchIdentifier = pchIdentifier ? allocAddString(pchIdentifier) : NULL;
        res->last_update = timerSecondsSince2000(); 
    }
    return res;
}

static void AuctionInvItemInfoCache_Destroy( AuctionInvItemInfoCache *hItem )
{
    if(hItem)
    {
        MP_FREE(AuctionInvItemInfoCache, hItem);
    }
}

static StashTable g_cachedItemStatusFromId = {0};

static AuctionInvItemInfoCache* AuctionInvItemInfoCache_Add(const char *pchIdentifier, int update_dt)
{
    AuctionInvItemInfoCache *cached;

    cached = AuctionInvItemInfoCache_Create(pchIdentifier);
    if(!stashAddPointer(g_cachedItemStatusFromId,allocAddString(pchIdentifier),cached,false))
    {
        Errorf("unable to add to add to cache item %s",pchIdentifier);
        AuctionInvItemInfoCache_Destroy(cached);
        cached = NULL;
    }
    if(cached)
        cached->last_update = timerSecondsSince2000() + update_dt;
    return cached;
}

static AuctionInvItemInfoCache* AuctionInvItemInfoCache_Get(const char *pchIdentifier)
{
    AuctionInvItemInfoCache *cache = NULL;
    if(!pchIdentifier)
        return NULL;
    if(!g_cachedItemStatusFromId)
        g_cachedItemStatusFromId = stashTableCreateWithStringKeys(128, StashDefault);
    stashFindPointer(g_cachedItemStatusFromId,pchIdentifier,&cache);
    return cache;
}

static int AuctionInvItemInfoCache_ProcessFunc(StashElement element)
{
    AuctionInvItemInfoCache *cache = (AuctionInvItemInfoCache*)stashElementGetPointer(element);
    if ( cache )
        cache->buying = cache->selling = 0;
    return 1;
}

static void AuctionInvItemInfoCache_Clear(void)
{
    if(g_cachedItemStatusFromId)
        stashForEachElement(g_cachedItemStatusFromId, AuctionInvItemInfoCache_ProcessFunc);
}

void AuctionClient_UpdateItemStatus(const char *pchIdentifier, int buying, int selling)
{
    AuctionInvItemInfoCache *cached = NULL;

    if(!pchIdentifier)
        return;

    cached = AuctionInvItemInfoCache_Get(pchIdentifier);
    if(!cached)
    {
        cached = AuctionInvItemInfoCache_Add(pchIdentifier,0);
        if(!cached)
            return;
    }

    cached->buying = buying;
    cached->selling = selling;
    cached->last_update = timerSecondsSince2000();
}

void AuctionClient_ProcessBatchInfo(char *str)
{
    char *c;

    if(!str)
        return;

    while((c = strchr(str,'\n')) != NULL)
    {
        const char *pchIdentifier;
        int buying = 0;
        int selling = 0;

        *c = '\0';
        pchIdentifier = allocAddString(str); // just in case StructCopy doesn't know to do this
        str = c + 1;

        if((c = strchr(str,'\n')) != NULL)
        {
            buying = atoi(str);
            str = c + 1;
        }

        if((c = strchr(str,'\n')) != NULL)
        {
            selling = atoi(str);
            str = c + 1;
        }

        AuctionClient_UpdateItemStatus(pchIdentifier,buying,selling);
    }
}

void ent_BatchSendAuctionInvItemInfoUpdate(Entity *e, int *forSaleList, int *forBuyList, int idOffset, int size)
{
    int i;
    if(!e || !forSaleList || !forBuyList)
        return;
    START_PACKET(pak, e, SERVER_AUCTION_BATCH_ITEMSTATUS);
    pktSendBitsAuto( pak, size);
    pktSendBitsAuto( pak, idOffset);
    pktSendBitsAuto(pak, eaiSize(&forSaleList));
    for ( i = 0; i < eaiSize(&forSaleList); ++i )
    {
        pktSendBitsPack(pak, 1, forSaleList[i]);
    }
    for ( i = 0; i < ea32Size(&forBuyList); ++i )
    {
        pktSendBitsPack(pak, 1, forBuyList[i]);
    }
    END_PACKET;
}

void ent_ListSendAuctionInvItemInfoUpdate(Entity *e, int size, int *idlist, int *forSaleList, int *forBuyList )
{
    int i;
    if(!e || !forSaleList || !forBuyList)
        return;
    START_PACKET(pak, e, SERVER_AUCTION_LIST_ITEMSTATUS);
    pktSendBitsAuto( pak, size);

    for ( i = 0; i < size; i++ )
    {
        pktSendBitsAuto(pak, idlist[i]);
        pktSendBitsAuto(pak, forSaleList[i]);
        pktSendBitsAuto(pak, forBuyList[i]);
    }
    END_PACKET;
}


void ent_ListReqAuctionItemStatus(Entity *e, int *idlist)
{
    int i, size = eaiSize(&idlist);
    static int *forSaleList = NULL;
    static int *forBuyList = NULL;

    eaiSetSize(&forSaleList, 0);
    eaiSetSize(&forBuyList, 0);

    for ( i = 0; i < eaiSize(&idlist); i++ )
    {
        AuctionInvItemInfoCache *cache;
        AuctionItem * pItem = (auctionData_GetDict())->ppItems[idlist[i]];

        cache = AuctionInvItemInfoCache_Get(pItem->pchIdentifier);

        if ( cache && cache->selling )
            eaiPush(&forSaleList, cache->selling);
        else
            eaiPush(&forSaleList, 0);

        if ( cache && cache->buying )
            eaiPush(&forBuyList, cache->buying);
        else
            eaiPush(&forBuyList, 0);

        if(!cache) // add it and make sure a request delay is put in. but don't return success.
            AuctionInvItemInfoCache_Add(pItem->pchIdentifier, AUCTIONINVITEMSTATUS_CACHE_REQ_DELAY);
    }

    ent_ListSendAuctionInvItemInfoUpdate(e, size, idlist, forSaleList, forBuyList);
}

void ent_BatchReqAuctionItemStatus(Entity *e, int startIdx,  int reqSize)
{
    int i;
    int lastID = -1;
    static int *forSaleList = NULL;
    static int *forBuyList = NULL;

    eaiSetSize(&forSaleList, 0);
    eaiSetSize(&forBuyList, 0);

    if ( startIdx < 0 )
        startIdx = 0;

    for ( i = startIdx; i < eaSize(&auctionData_GetDict()->ppItems) && i-startIdx < reqSize; ++i )
    {
        AuctionInvItemInfoCache *cache;
        AuctionItem * pItem = (auctionData_GetDict())->ppItems[i];

        cache = AuctionInvItemInfoCache_Get(pItem->pchIdentifier);

        if ( cache && cache->selling )
            eaiPush(&forSaleList, cache->selling);
        else
            eaiPush(&forSaleList, 0);

        if ( cache && cache->buying )
            eaiPush(&forBuyList, cache->buying);
        else
            eaiPush(&forBuyList, 0);

        lastID = i;

        if(!cache) // add it and make sure a request delay is put in. but don't return success.
            AuctionInvItemInfoCache_Add(pItem->pchIdentifier, AUCTIONINVITEMSTATUS_CACHE_REQ_DELAY);
    }

    if ( eaiSize(&forSaleList) )
        ent_BatchSendAuctionInvItemInfoUpdate(e, forSaleList, forBuyList, startIdx, lastID-startIdx);
}


void ent_ReqAuctionInv(Entity *e)
{
    if(e)
    {
        Packet *pak_out = ent_AuctionPktCreate(e, DBCLIENT_AUCTION_REQ_INV);
        pktSend(&pak_out, &db_comm_link);
        e->pchar->auctionInvUpdated = true;
    }
}

void ent_ReqAuctionHist(Entity *e,char *pchIdentifier)
{
    if(stashFindPointer(auctionData_GetDict()->stAuctionItems, pchIdentifier,NULL))
    {
        Packet *pak_out = ent_AuctionPktCreate(e, DBCLIENT_AUCTION_REQ_HISTINFO);
        pktSendString(pak_out, pchIdentifier);
        pktSend(&pak_out, &db_comm_link);
    }
}

int auction_dbMessageCallback(Packet *pak,int cmd,NetLink *link)
{
    int cursor = bsGetCursorBitPosition(&pak->stream); // for debugging

    switch(cmd)
    {
    case DBSERVER_AUCTION_SEND_INV:
    {
        int ident = pktGetBitsAuto(pak);
        Entity *e = entFromDbId(ident); 
        char *invStr = pktGetString(pak);
        int iExpectedInvSize;
        if(e&&e->pchar)
        {
            iExpectedInvSize = e->access_level ? 0 : character_GetAuctionInvTotalSize(e->pchar);

            AuctionInventory_FromStr(&e->pchar->auctionInv,invStr);
            e->pchar->auctionInvUpdated = true;

            if(e->pchar->auctionInv && e->pchar->auctionInv->invSize != iExpectedInvSize)
                ent_AuctionSendInvSizeXact(e,iExpectedInvSize);
        }
    }
    break;
    case DBSERVER_AUCTION_SEND_HIST:
    {
        int ident = pktGetBitsAuto(pak);
        Entity *e = entFromDbId(ident);

        char *pchIdentifier = pktGetString(pak);
        int buying = pktGetBitsAuto(pak);
        int selling = pktGetBitsAuto(pak);

        AuctionClient_UpdateItemStatus(pchIdentifier,buying,selling);

        if(e)
        {
            int i;
            int count = pktGetBitsAuto(pak);

            START_PACKET(pak_out, e, SERVER_AUCTION_HISTORY);
            pktSendString(pak_out,pchIdentifier);
            pktSendBitsAuto(pak_out,buying);
            pktSendBitsAuto(pak_out,selling);
            pktSendBitsAuto(pak_out,count);
            // TODO: cache history
            for(i = 0; i < count; ++i)
            {
                pktSendBitsAuto(pak_out,pktGetBitsAuto(pak)); // date
                pktSendBitsAuto(pak_out,pktGetBitsAuto(pak)); // price
            }
            END_PACKET;
        }
    }
    break;
    case DBSERVER_AUCTION_XACT_CMD:
    {
        int ident = pktGetBitsAuto(pak);
        XactUpdateType update_type = pktGetBitsAuto(pak);
        Entity *e = entFromDbId(ident);
        XactCmd *cmd = NULL;  
        XactCmd_Recv(&cmd,pak);
        MapHandleXactCmd(e,update_type,cmd);
        XactCmd_Destroy(cmd);
    }
    break;
    case DBSERVER_AUCTION_XACT_ABORT:
    {
        int ident = pktGetBitsAuto(pak);
        int idxact = pktGetBitsAuto(pak);
        Entity *e = entFromDbId(ident);
        if(e && e->access_level && e->client)
            conPrintf(e->client,"transaction %i aborted",idxact);
    }
    break; 
    case DBSERVER_AUCTION_XACT_COMMIT:
    {
        int ident = pktGetBitsAuto(pak);
        int idxact = pktGetBitsAuto(pak);
        Entity *e = entFromDbId(ident);
        if(e && e->access_level && e->client)
            conPrintf(e->client,"transaction %i commited",idxact);
    }
    break;
    case DBSERVER_AUCTION_XACT_FAIL: // currently this only occurs when there is no auction server or it is throttled
    {
        int ident = pktGetBitsAuto(pak);
        int xact_type = pktGetBitsAuto(pak);
        Entity *e = entFromDbId(ident);
        char *context;
        strdup_alloca(context,pktGetString(pak));

        MapHandleXactFail(e,xact_type,context);

        if(e && e->access_level && e->client)
            conPrintf(e->client,"transaction failed (no server or server is throttled)");
    }
    break;
    case DBSERVER_AUCTION_BATCH_INFO:
    {
        char *batch_info_str = pktGetZipped(pak,NULL);

        AuctionInvItemInfoCache_Clear();
        AuctionClient_ProcessBatchInfo(batch_info_str);

        free(batch_info_str);
    }
    break;
    default:
        LOG_OLD_ERR("AuctionClientErrs unknown command %i",cmd); 
    };
    return 1;
}

void ent_AuctionSendInvSizeXact(Entity *e, int invSize)
{
    char *context = NULL;
    Packet *pak = ent_AuctionPktCreate(e,DBCLIENT_AUCTION_XACT_REQ);

    estrConcatf(&context,"%d",invSize);

    dbLog("AuctionClient", e, "req set innventory size: %s", context);

    pktSendBitsAuto(pak,XactCmdType_SetInvSize);
    pktSendString(pak,context);
    pktSend(&pak,&db_comm_link);

    estrDestroy(&context);
}

static void s_handleFixedPricePurchase(Entity *e, AuctionItem *pItem, int id)
{
    bool success = false;
    const char *name = NULL;

    if(!e || !pItem || !e->pchar ) //||    (pItem->playerType && (!e->pl || e->pl->playerType != pItem->playerType-1)) )
        return; // cheater?

    if(e->pchar->iInfluencePoints < pItem->buyPrice)
    {
        sendDialog(e,localizedPrintf(e,"NotEnoughInfluence"));
        dbLog("AuctionClient:Fixed",e,"Not enough inf for fixed-price item (has %i, needs %i) %i:%s",
            e->pchar->iInfluencePoints, pItem->buyPrice, id, pItem->displayName);
        return;
    }

    switch(pItem->type)
    {
        xcase kTrayItemType_Inspiration:
        {
            BasePower *inspiration = pItem->inspiration;
            if(devassert(inspiration))
            {
                if(character_AddInspiration(e->pchar, inspiration, "auction_fixed"))
                    success = true;
                else
                    name = inspiration->pchDisplayName;
            }
            else
                dbLog("AuctionClient:Fixed",e,"Error: Could not find fixed-price inspiration %i:%s",id,inspiration->pchName);
        }
        xcase kTrayItemType_SpecializationInventory:
        {
            BasePower *enchancement = pItem->enhancement;
            if(devassert(enchancement))
            {
                if(character_AddBoost(e->pchar, enchancement, pItem->lvl, 0, "auction_fixed"))
                    success = true;
                else
                    name = enchancement->pchDisplayName;
            }
            else
                dbLog("AuctionClient:Fixed",e,"Error: Could not find fixed-price enhancement %i:%s",id,enchancement->pchName);
        }
        xcase kTrayItemType_Power:
        {
            BasePower *temp_power = pItem->temp_power;
            if(devassert(temp_power))
            {
                if(character_AddRewardPower(e->pchar, temp_power))
                {
                    success = true;
                    sendInfoBox(e, INFO_AUCTION, "TempPowerYouReceived", temp_power->pchDisplayName);
                    serveFloater(e, e, "FloatFoundTempPower", 0.0f, kFloaterStyle_Attention, 0);
                }
                else
                    name = temp_power->pchDisplayName;
            }
            else
                dbLog("AuctionClient:Fixed",e,"Error: Could not find fixed-price temp power %i:%s",id,pItem->displayName);
        }
        xdefault:
            Errorf("Invalid TrayItemType for ent_AddAuctionInvItemXact: %s",TrayItemType_Str(pItem->type));
            break;
    }

    if(success)
    {
        ent_AdjInfluence(e,-pItem->buyPrice, "auction.bought");
        sendInfoBox(e, INFO_AUCTION, "AuctionNotify_PaidInf", pItem->buyPrice);
        dbLog("AuctionClient:Fixed",e,"Purchased fixed-price item for %i inf %i:%s",pItem->buyPrice,id,pItem->displayName);
    }
    else if(name)
    {
        sendDialog(e,localizedPrintf(e,"NoRoomInInventory",name));
        dbLog("AuctionClient:Fixed",e,"Could not add fixed-price item, inventory full %i:%s",id,pItem->displayName);
    }
}

static bool VerifyAddInvItem(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem, INT64 infStoredOverflowCheat);
static bool VerifyChangeInvStatus(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem);
static bool VerifyRemAmtStored(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem);
static bool VerifyRemInfStored(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem);

void ent_AddAuctionInvItemXact(Entity *e, char * pchIdentifier, int index, int amt, int price, AuctionInvItemStatus auc_status, int id)
{
    char *context = NULL;
    Packet *pak;
    AuctionInvItem itm = {0};
    XactCmdType cmdType = XactCmdType_AddInvItem;
    INT64 infStoredOverflowCheat = 0;
    
    AuctionItem * pItem = auctionData_GetDict()->ppItems[id];
    
    if(!e || !pItem )
        return;

    if ( pItem && pItem->buyPrice ) // this is not an auction house xaction
    {
        s_handleFixedPricePurchase(e,pItem,id);
        return;
    }

    itm.pchIdentifier = pchIdentifier;
    itm.iMapSide = index;
    itm.auction_status = auc_status;
    itm.infPrice = price;

    if(auc_status == AuctionInvItemStatus_Bidding || auc_status == AuctionInvItemStatus_Sold)
    {
        itm.amtOther = amt;
        itm.amtStored = 0;
        if(auc_status == AuctionInvItemStatus_Bidding)
        {
            itm.infStored = amt * price;
            infStoredOverflowCheat = (INT64)amt * (INT64)price;
        }
    }
    else
    {
        itm.amtStored = amt;
        itm.amtOther = 0;
    }

    // make sure this is a good transaction, so we can throw it out and not pass it up to the auction server
    // (this is checked again when it comes back from the auction server)
    if(!VerifyAddInvItem(e,NULL,&itm,pItem,infStoredOverflowCheat))
    {
        if(e->pchar)
            e->pchar->auctionInvUpdated = true;
        return;
    }

    if ( (pItem->type == kTrayItemType_Recipe || pItem->type == kTrayItemType_Salvage) && 
        e && e->pchar && e->pchar->auctionInv && EAINRANGE(id, auctionData_GetDict()->ppItems))
    {
        int i;
        for ( i = 0; i < eaSize(&e->pchar->auctionInv->items); ++i )
        {
            if ( !stricmp(e->pchar->auctionInv->items[i]->pchIdentifier, itm.pchIdentifier) &&
                e->pchar->auctionInv->items[i]->auction_status == itm.auction_status &&
                e->pchar->auctionInv->items[i]->infPrice == itm.infPrice )
            {
                if (e->pchar->auctionInv->items[i]->amtStored + e->pchar->auctionInv->items[i]->amtOther + 
                    itm.amtStored + itm.amtOther <= MAX_AUCTION_STACK_SIZE)
                {
                    if (e->pchar->auctionInv->items[i]->infStored <= (AUCTION_MAX_INFLUENCE - itm.infStored))
                    {
                        cmdType = XactCmdType_ChangeInvAmount;
                        itm.id = e->pchar->auctionInv->items[i]->id;
                    }
                }
            }
        }
    }

    AuctionInvItem_ToStr(&itm,&context);
    if ( cmdType == XactCmdType_ChangeInvAmount )
        acLog("AuctionClient:Put", e, NULL, &itm, context, "changing item amount in CH inventory amt = %i", amt);
    else
    {
        if(auc_status == AuctionInvItemStatus_Bidding)
            acLog("AuctionClient:Put", e, NULL, &itm, context, "bidding on item to CH inventory amt = %i, price = %i", amt, price);
        else
            acLog("AuctionClient:Put", e, NULL, &itm, context, "adding item to CH inventory amt = %i, price = %i", amt, price);
    }

    pak = ent_AuctionPktCreate(e,DBCLIENT_AUCTION_XACT_REQ);
    if(!pak)
        return;
    pktSendBitsAuto( pak, cmdType);
    pktSendString( pak, context);
    pktSend( &pak, &db_comm_link); 

    estrDestroy(&context); 
}

static AuctionInvItem* s_ent_AuctionInvItmFromId(Entity *e, int id)
{
    int i;

    if(!e || !e->pchar || !e->pchar->auctionInv || id <= 0)
        return NULL;

    for(i = eaSize(&e->pchar->auctionInv->items)-1; i >= 0; --i)
        if(e->pchar->auctionInv->items[i]->id == id)
            return e->pchar->auctionInv->items[i];

    return NULL;
}

void ent_ChangeAuctionInvItemStatusXact(Entity *e, int id, int price, AuctionInvItemStatus auc_status)
{
    Packet *pak;
    char *context = NULL;
    AuctionInvItem *itm = s_ent_AuctionInvItmFromId(e,id);
    AuctionInvItem *itemReq = NULL;
    AuctionItem *pItem = NULL;

    if( itm )
        stashFindPointer(auctionData_GetDict()->stAuctionItems, itm->pchIdentifier, &pItem);

    if(!pItem)
        return;

    pak = ent_AuctionPktCreate(e,DBCLIENT_AUCTION_XACT_REQ);
    if(!pak)
        return;

    if(!AuctionInvItem_Clone(&itemReq,itm))
    {
        pktDestroy(pak);
        return;
    }

    itemReq->infPrice = price;
    itemReq->auction_status = auc_status;

    if(!VerifyChangeInvStatus(e,NULL,itemReq,pItem) || !AuctionInvItem_ToStr(itemReq,&context))
    {
        e->pchar->auctionInvUpdated = true;
        pktDestroy(pak);
        AuctionInvItem_Destroy(itemReq);
        return;
    }

    acLog("AuctionClient:Change", e, NULL, itemReq, context, "change price %i and status %s", itemReq->infPrice, AuctionInvItemStatus_ToStr(itemReq->auction_status));

    pktSendBitsAuto( pak, XactCmdType_ChangeInvStatus);
    pktSendString( pak, context);
    pktSend( &pak, &db_comm_link); 

    estrDestroy(&context);
    AuctionInvItem_Destroy(itemReq);
}

static void s_ent_GetXactCommon(Entity *e, int id, XactCmdType xact)
{
    Packet *pak;
    char *context = NULL;
    AuctionInvItem *itm = s_ent_AuctionInvItmFromId(e,id);
    AuctionInvItem *itmCopy = NULL;
    AuctionItem *pItem = NULL;

    if( itm )
        stashFindPointer(auctionData_GetDict()->stAuctionItems, itm->pchIdentifier, &pItem);

    if(!pItem)
        return;

    AuctionInvItem_Clone(&itmCopy, itm);

    if ( !itmCopy )
        return;

    if( xact == XactCmdType_GetAmtStored )
    {
        int empty_slots = 0;

        if(pItem->type == kTrayItemType_Recipe || pItem->type == kTrayItemType_Salvage)
        {
            int invtype = InventoryType_FromTrayItemType(pItem->type);
            int invid = genericinv_GetInvIdFromItem(invtype, pItem->pItem);
            empty_slots = character_InventoryEmptySlotsById(e->pchar, invtype, invid);
        }
        if( pItem->type == kTrayItemType_Inspiration )
            empty_slots = character_InspirationInventoryEmptySlots(e->pchar);
        if( pItem->type == kTrayItemType_SpecializationInventory )
            empty_slots = character_BoostInventoryEmptySlots(e->pchar);

        if ( empty_slots <= 0 )
        {
            plaque_SendString(e, "AuctionPlayerInvFull");
            acLog("AuctionClient:Get", e, NULL, itm, NULL, "Get Failed. inventory full. couldn't get item: %s, %d", pItem->pchIdentifier, itmCopy->amtStored);
            AuctionInvItem_Destroy(itmCopy);
            e->pchar->auctionInvUpdated = true;
            return;
        }

        if ( itmCopy->amtStored > empty_slots )
        {
            itmCopy->amtStored = empty_slots;
            acLog("AuctionClient:Get", e, NULL, itm, NULL, "Getting part of a stack: %s, %d", pItem->pchIdentifier, itmCopy->amtStored);
        }
    }

    if((xact == XactCmdType_GetInvItem || xact == XactCmdType_GetAmtStored) )
    {
        if( !VerifyRemAmtStored(e,NULL,itmCopy,pItem) )
        {
            e->pchar->auctionInvUpdated = true;
            AuctionInvItem_Destroy(itmCopy);
            return;
        }
    }

    if((xact == XactCmdType_GetInvItem || xact == XactCmdType_GetInfStored) &&
        !VerifyRemInfStored(e,NULL,itmCopy,pItem))
    {
        e->pchar->auctionInvUpdated = true;
        AuctionInvItem_Destroy(itmCopy);
        return;
    }

    if( xact == XactCmdType_GetInfStored  &&!ent_canAddInfluence( e, itm->infStored ) )
    {
        plaque_SendString(e, "CannotClaimInf");
        AuctionInvItem_Destroy(itmCopy);
        return;
    }

    pak = ent_AuctionPktCreate(e,DBCLIENT_AUCTION_XACT_REQ);

    if(!pak)
    {
        AuctionInvItem_Destroy(itmCopy);
        return;
    }

    if(!AuctionInvItem_ToStr(itmCopy,&context))
    {
        AuctionInvItem_Destroy(itmCopy);
        pktDestroy(pak);
        return;
    }

    acLog("AuctionClient:Get", e, NULL, itm, context, "getting item from CH inventory (%d).", (int)xact);

    pktSendBitsAuto( pak, xact);
    pktSendString( pak, context);
    pktSend( &pak, &db_comm_link); 

    estrDestroy(&context);
    AuctionInvItem_Destroy(itmCopy);
}

void ent_GetAuctionInvItemXact(Entity *e, int id)
{
    s_ent_GetXactCommon(e, id, XactCmdType_GetInvItem);
}

void ent_GetInfStoredXact(Entity *e, int id)
{
    s_ent_GetXactCommon(e, id, XactCmdType_GetInfStored);
}

void ent_GetAmtStoredXact(Entity *e, int id)
{
    s_ent_GetXactCommon(e, id, XactCmdType_GetAmtStored);
}

void ent_XactReqShardJump(Entity *e, const char *shard, int map, const char *data)
{
    char *context = NULL;
    Packet *pak = ent_AuctionPktCreate(e,DBCLIENT_AUCTION_XACT_REQ);

    estrConcatf(&context,"%s\n%d\n%s",shard,map,data);

    dbLog("AuctionClient", e, "req ShardJump: %s", context);

    pktSendBitsAuto(pak,XactCmdType_ShardJump);
    pktSendString(pak,context);
    pktSend(&pak,&db_comm_link);

    estrDestroy(&context);
}


static void MapSendXactCmdUpdate(Entity *e, XactCmd *cmd)
{
    Packet *pak;
    if(!cmd || !e || !e->pl)
        return;

    pak = pktCreateEx(&db_comm_link,DBCLIENT_AUCTION_XACT_UPDATE);
    pktSendBitsAuto(pak, cmd->xid);
    pktSendBitsAuto(pak, cmd->subid);
    pktSendBitsAuto(pak, cmd->res);
    pktSend(&pak,&db_comm_link);
}

static void AuctionDatamine(AuctionItem *pItem, const char *context1, int amount1, const char *context2, int amount2, 
                            const char *context3, int amount3, const char *context4, int amount4)
{
    char *pCat = "auction";

    if(!pItem)
        return;

    switch(pItem->type)
    {
        case kTrayItemType_Salvage:
        {
            SalvageItem *salvage = pItem->salvage;
            DATA_MINE_SALVAGE(salvage, pCat, context1, amount1);
            DATA_MINE_SALVAGE(salvage, pCat, context2, amount2);
            DATA_MINE_SALVAGE(salvage, pCat, context3, amount3);
            DATA_MINE_SALVAGE(salvage, pCat, context4, amount4);
        }
        xcase kTrayItemType_Recipe:
        {
            const DetailRecipe *recipe = pItem->recipe;
            DATA_MINE_RECIPE(recipe, pCat, context1, amount1);
            DATA_MINE_RECIPE(recipe, pCat, context2, amount2);
            DATA_MINE_RECIPE(recipe, pCat, context3, amount3);
            DATA_MINE_RECIPE(recipe, pCat, context4, amount4);
        }
        xcase kTrayItemType_Inspiration:
        {
            const BasePower *inspiration = pItem->inspiration;
            DATA_MINE_INSPIRATION(inspiration, pCat, context1, amount1);
            DATA_MINE_INSPIRATION(inspiration, pCat, context2, amount2);
            DATA_MINE_INSPIRATION(inspiration, pCat, context3, amount3);
            DATA_MINE_INSPIRATION(inspiration, pCat, context4, amount4);
        }
        xcase kTrayItemType_SpecializationInventory:
        {
            const BasePower *enchancement = pItem->enhancement;
            DATA_MINE_ENHANCEMENT(enchancement, pItem->lvl, pCat, context1, amount1);
            DATA_MINE_ENHANCEMENT(enchancement, pItem->lvl, pCat, context2, amount2);
            DATA_MINE_ENHANCEMENT(enchancement, pItem->lvl, pCat, context3, amount3);
            DATA_MINE_ENHANCEMENT(enchancement, pItem->lvl, pCat, context4, amount4);
        }
        xdefault:
            break;
    }
}

static void AddInfStored(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem)
{
    if(itm->infStored <= 0)
        return;

    if(!e->auc_trust_gameclient)
        ent_AdjInfluence(e,-itm->infStored, "auction.bidding");

    acLog("AuctionClient:Put", e, cmd, itm, NULL, "bid placed. removed %d inf.", itm->infStored);

    if(devassert(itm->auction_status == AuctionInvItemStatus_Bidding))
    {
        char *display = s_AuctionItemToDisplayString(pItem);

        AuctionDatamine(pItem, "inf_bidding",itm->infStored,"bidding",itm->amtOther,NULL,0,NULL,0);

        if(display)
        {
            if(itm->amtOther != 1)
                sendInfoBox(e, INFO_AUCTION, "AuctionNotify_BidInfCount", display, itm->infStored, itm->amtOther);
            else
                sendInfoBox(e, INFO_AUCTION, "AuctionNotify_BidInf", display, itm->infStored);
        }
    }
}

// this checks both inf and amount, as well as sanity checking
static bool VerifyAddInvItem(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem, INT64 infStoredOverflowCheat)
{
    InventoryType invtype;

    if(!IsAuctionItemValid(pItem) || !DoesAuctionItemPassAuctionRequiresCheck(pItem, e->pchar))
    {
        acLog("AuctionClient:Put", e, cmd, itm, NULL, 
            "Cheater: Attempted to add an invalid auction item (%s)", pItem->pchIdentifier);
        return false;
    }

    if(infStoredOverflowCheat < (INT64)INT_MIN || infStoredOverflowCheat > (INT64)INT_MAX)
    {
        acLog("AuctionClient:Put", e, cmd, itm, NULL, "Cheater: Bad AuctionAddInv overflow inf (%d), items (%d), price (%d) or other (%d)", itm->infStored, itm->amtStored, itm->infPrice, itm->amtOther);
        return false;
    }

    if(itm->infStored < 0 || itm->amtStored < 0 || itm->infPrice < 0 || itm->amtOther < 0)
    {
        acLog("AuctionClient:Put", e, cmd, itm, NULL, "Cheater: Bad AuctionAddInv negative inf (%d), items (%d), price (%d) or other (%d)", itm->infStored, itm->amtStored, itm->infPrice, itm->amtOther);
        return false;
    }

    if(!devassert(!!itm->infStored ^ !!itm->amtStored)) // currently, we only support just infStored for bidding, and just amtStored for storing
    {
        acLog("AuctionClient:Put", e, cmd, itm, NULL, "Bad AuctionAddInv, tried to store %s items and influence", (itm->amtOther && itm->infStored ? "both" : "no"));
        return false;
    }

    if(itm->infStored && !e->auc_trust_gameclient)
    {
        if(e->pchar->iInfluencePoints < itm->infStored)
        {
            acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. Couldn't place bid, not enough inf. to cover bid price: e->inf = %d, price = %d", e->pchar->iInfluencePoints, itm->infStored);
            return false;
        }
    }

    if (pItem->buyPrice > 0)
    {
        plaque_SendString(e, "AuctionCannotSellItem");
        acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed: Attempted to add an item with a buy price (%s - %d inf)", pItem->pchIdentifier, pItem->buyPrice);
        return false;
    }

    invtype = InventoryType_FromTrayItemType(pItem->type);

    if(invtype == kTrayItemType_Count &&    // this is a non-stackable item
        itm->amtStored+itm->amtOther != 1)    // can only have one item between stored and other
    {
        acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. Invalid AddInv amount on a non-stacking itm: %s, %d, %d", pItem->pchIdentifier, invtype, itm->amtStored);
        Errorf("Invalid AddInv amount on a non-stacking itm: %s, %d, %d", pItem->pchIdentifier, invtype, itm->amtStored);
        return false;
    }

    if(itm->amtStored)
    {
        if(invtype != kInventoryType_Count) // stackable
        {
            int invid = genericinv_GetInvIdFromItem(invtype,pItem->pItem);

            // verify no trade items
            if (invtype == kInventoryType_Salvage)
            {
                SalvageInventoryItem *pSal = character_FindSalvageInv(e->pchar, pItem->salvage->pchName);

                if (pSal && pSal->salvage && (pSal->salvage->flags & SALVAGE_NOAUCTION))
                {    
                    Errorf("attempted to store non-tradable item %s:%s",inventorytype_StrFromType(pItem->type), pItem->salvage->pchName);
                    acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. attempted to store non-tradable item %s:%s",inventorytype_StrFromType(pItem->type), pItem->salvage->pchName);
                    return false;                    
                }
            }

            if(invid<0)
            {
                Errorf("need to add support for inventory item %s:%s",inventorytype_StrFromType(pItem->type), pItem->salvage->pchName);
                acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. need to add support for inventory item %s:%s",inventorytype_StrFromType(pItem->type), pItem->salvage->pchName);
                return false;
            }
            else if(itm->amtStored > character_GetItemAmount(e->pchar, invtype, invid))
            {
                acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. Could not remove item from inventory: %s, %d, %d", pItem->salvage->pchName, invtype, itm->amtStored);
                return false;
            }
        }
        else if(pItem->type == kTrayItemType_Inspiration)
        {
            int srcCol = itm->iMapSide >> 16;
            int srcRow = itm->iMapSide & 0x0000FFFF;
            if(!INRANGE0(srcCol,e->pchar->iNumInspirationCols) || !INRANGE0(srcRow,e->pchar->iNumInspirationRows))
            {
                acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. Bad source column or row for addinv inspiration: %d,%d (max %d,%d)",
                    srcCol, srcRow, e->pchar->iNumInspirationCols, e->pchar->iNumInspirationRows);
                return false;
            }
            else if(!e->pchar->aInspirations[srcCol][srcRow] || e->pchar->aInspirations[srcCol][srcRow] != pItem->inspiration )
            {

                acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. Non-match for addinv inspiration: %s at %d,%d (found %s)",
                    pItem->inspiration->pchFullName, srcCol, srcRow, (e->pchar->aInspirations[srcCol][srcRow] ? basepower_ToPath(e->pchar->aInspirations[srcCol][srcRow]) : "[NULL]"));
                return false;
            }
            else if (character_IsInspirationSlotInUse(e->pchar, srcCol,  srcRow))
            {
                acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. Could not remove inspiration from inventory: %s at %d,%d",
                    pItem->inspiration->pchFullName, srcCol, srcRow);
                return false;
            }
            else if (!inspiration_IsTradeable(pItem->inspiration, e->auth_name, ""))
            {    
                Errorf("attempted to store non-tradable item %s:%s",inventorytype_StrFromType(pItem->type),pItem->inspiration->pchFullName);
                acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. attempted to store non-tradable item %s:%s",inventorytype_StrFromType(pItem->type),pItem->inspiration->pchFullName);
                return false;                    
            }
        }
        else if(pItem->type == kTrayItemType_SpecializationInventory)
        {
            // TODO: what if they gave us an enhancement with combines?
            if( !AINRANGE(itm->iMapSide, e->pchar->aBoosts) )
            {
                acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. Bad index for addinv boost: %d (max %d)",
                    itm->iMapSide, CHAR_BOOST_MAX);
                return false;
            }
            else 
            {
                const BasePower *basePower;

                if(e->pchar->aBoosts[itm->iMapSide] )
                    basePower = e->pchar->aBoosts[itm->iMapSide]->ppowBase;
                else
                {
                    acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. Bad index for addinv boost: %d (max %d)",
                        itm->iMapSide, CHAR_BOOST_MAX);
                    return false;
                }
                
                if(!e->pchar->aBoosts[itm->iMapSide] || basePower != pItem->enhancement )
                {
                    acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. Non-match for addinv boost: %s at %d (found %s)",
                        pItem->enhancement->pchFullName, itm->iMapSide, (e->pchar->aBoosts[itm->iMapSide] ? basepower_ToPath(e->pchar->aBoosts[itm->iMapSide]->ppowBase) : "[NULL]"));
                    return false;
                }
                if (basePower && basePower->bBoostAccountBound)
                {
                    Errorf("attempted to store non-tradable item %s:%s",inventorytype_StrFromType(pItem->type),pItem->enhancement->pchFullName);
                    acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. attempted to store non-tradable item %s:%s",inventorytype_StrFromType(pItem->type),pItem->enhancement->pchFullName);
                    return false;                    
                } 
                else if (!detailrecipedict_IsBoostTradeable(basePower, e->pchar->aBoosts[itm->iMapSide]->iLevel, e->auth_name, ""))
                {    
                    Errorf("attempted to store non-tradable item %s:%s",inventorytype_StrFromType(pItem->type),pItem->enhancement->pchFullName);
                    acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. attempted to store non-tradable item %s:%s",inventorytype_StrFromType(pItem->type),pItem->enhancement->pchFullName);
                    return false;                    
                }
            }
        }
        else
        {
            acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. need to add support for tray type %i:%s",pItem->type,inventorytype_StrFromType(pItem->type));
            Errorf("need to add support for tray type %i:%s",pItem->type,inventorytype_StrFromType(pItem->type));
            return false;
        }
    }

    return true;
}

static bool AddAmtStored(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem)
{
    InventoryType invtype = InventoryType_FromTrayItemType(pItem->type);
    char *display = s_AuctionItemToDisplayString(pItem);

    if(itm->amtStored <= 0)
        return true;

    if(invtype != kInventoryType_Count) // stackable item
    {
        int invid = genericinv_GetInvIdFromItem(invtype,pItem->pItem);
        if(character_AdjustInventory(e->pchar, invtype, invid, -itm->amtStored, "auction", false) < 0 && !e->auc_trust_gameclient)
        {
            acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. Could not remove item from inventory: %s, %d, %d", pItem->pchIdentifier, invtype, itm->amtStored);
            return false;
        }
        else
            acLog("AuctionClient:Put", e, cmd, itm, NULL, "Removed item from inventory: %s, %d, %d", pItem->pchIdentifier, invtype, itm->amtStored);
    }
    else if(pItem->type == kTrayItemType_Inspiration)
    {
        int srcCol = itm->iMapSide >> 16;
        int srcRow = itm->iMapSide & 0x0000FFFF;
        if(!character_RemoveInspiration( e->pchar, srcCol, srcRow, "auction") && !e->auc_trust_gameclient)
        {
            acLog("AuctionClient:Put", e, cmd, itm, NULL, "Failed. Could not remove inspiration from inventory: %s at %d,%d",
                pItem->inspiration->pchFullName, srcCol, srcRow);
            return false;
        }
        else
        {
            // already removed in the previous if clause
            character_CompactInspirations(e->pchar);
            acLog("AuctionClient:Put", e, cmd, itm, NULL, "Success. Removed inspiration from inventory: %s at %d,%d",
                pItem->inspiration->pchFullName, srcCol, srcRow);
        }
    }
    else if(pItem->type == kTrayItemType_SpecializationInventory)
    {
        character_DestroyBoost(e->pchar, itm->iMapSide, "auction");
        acLog("AuctionClient:Put", e, cmd, itm, NULL,"Success. Removed enhancement from inventory: %s at %d",
            pItem->enhancement->pchFullName, itm->iMapSide); 
    }

    if(display)
    {
        if(itm->amtStored != 1)
            sendInfoBox(e, INFO_AUCTION, "AuctionNotify_PutCount", display, itm->amtStored);
        else
            sendInfoBox(e, INFO_AUCTION, "AuctionNotify_Put", display);
    }

    return true;
}

static bool VerifyRemInfStored(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem)
{
    // there are currently no failure cases in this function
    // TODO: fail on overflow
    return true;
}

static void RemInfStored(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem)
{
    int fee = 0;
    int infToBeGiven, res_adj;

    if(itm->amtCancelled == 0 && itm->infStored <= 0)
        return;

    if ( (itm->auction_status == AuctionInvItemStatus_ForSale || itm->auction_status == AuctionInvItemStatus_Sold) )
        fee = calcAuctionSoldFee(itm->infStored, itm->infPrice, itm->amtOther, itm->amtCancelled);
    
    infToBeGiven = itm->infStored - fee;
    res_adj = ent_AdjInfluence(e, infToBeGiven, "action.unbidding");

    if(res_adj != infToBeGiven) // FIXME: still considering overflow a success
        acLog("AuctionClient:Get", e, cmd, itm, NULL, "getting sale. player with %i inf should have been given %i influece, but was given %i",e->pchar->iInfluencePoints,infToBeGiven,res_adj); 
    else 
        acLog("AuctionClient:Get", e, cmd, itm, NULL, "get sale. player was given %i inf.", res_adj);

    if(fee)
    {
        badge_StatAdd(e,"auction.fees",fee);
        badge_StatAdd(e,"auction.fees.sold",fee);
    }

    if(itm->auction_status == AuctionInvItemStatus_Bidding || itm->auction_status == AuctionInvItemStatus_Bought)
    {
        AuctionDatamine(pItem,NULL,0,NULL,0,
                            "inf_bidding_cancelled",itm->infStored,
                            "bidding_cancelled",itm->amtOther);
    }
    else if(itm->auction_status == AuctionInvItemStatus_ForSale || itm->auction_status == AuctionInvItemStatus_Sold)
    {
        AuctionDatamine(pItem,
                            "inf_sold",itm->infStored,
                            "sold",itm->amtOther,
                            NULL,0,NULL,0);

        badge_StatAdd(e,"auction.sold",itm->amtOther);
        switch(pItem->type)
        {
            case kTrayItemType_Salvage:
                badge_StatAdd(e,"auction.sold.salvage",itm->amtOther);
            xcase kTrayItemType_Recipe:
                badge_StatAdd(e,"auction.sold.recipe",itm->amtOther);
            xcase kTrayItemType_Inspiration:
                badge_StatAdd(e,"auction.sold.inspiration",itm->amtOther);
            xcase kTrayItemType_SpecializationInventory:
                badge_StatAdd(e,"auction.sold.boost",itm->amtOther);
            xdefault:
                break;
        }
    }

    if(itm->infStored)
        sendInfoBox(e, INFO_AUCTION, "AuctionNotify_GotInf", itm->infStored);

    if(fee >= 0)
        sendInfoBox(e, INFO_AUCTION, "AuctionNotify_PaidInf", fee);
    else
        sendInfoBox(e, INFO_AUCTION, "AuctionNotify_GotInf", -fee);
}

static bool VerifyRemAmtStored(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem)
{
    InventoryType invtype = InventoryType_FromTrayItemType(pItem->type);
    bool inventory_full = false;

    if(!itm->amtStored)
        return true;

    if(invtype != kInventoryType_Count)
    {
        int invid = genericinv_GetInvIdFromItem(invtype,pItem->pItem);
        if(invid<0)
        {
            Errorf("need to add support for inventory item %s:%s",inventorytype_StrFromType(pItem->type),pItem->pchIdentifier);
            acLog("AuctionClient:Get", e, cmd, itm, NULL, "Get failed. need to add support for inventory item %s:%s",inventorytype_StrFromType(pItem->type),pItem->pchIdentifier);
            return false;
        }
        else if(!character_CanAddInventory(e->pchar,invtype,invid,itm->amtStored) )
            inventory_full = true;
    }
    else if(pItem->type == kTrayItemType_Inspiration)
    {
        if(character_InspirationInventoryEmptySlots(e->pchar) < itm->amtStored)
            inventory_full = true;
    }
    else if(pItem->type == kTrayItemType_SpecializationInventory)
    {
        if(character_BoostInventoryEmptySlots(e->pchar) < itm->amtStored)
            inventory_full = true;
    }
    else
    {
        Errorf("need to add support for tray type %i:%s",pItem->type,inventorytype_StrFromType(pItem->type));
        acLog("AuctionClient:Get", e, cmd, itm, NULL, "Get failed. need to add support for tray type %i:%s",pItem->type,inventorytype_StrFromType(pItem->type));
        return false;
    }

    if(inventory_full && !e->auc_trust_gameclient)
    {
        plaque_SendString(e, "AuctionPlayerInvFull");
        acLog("AuctionClient:Get", e, cmd, itm, NULL, "Get Failed. inventory full. couldn't get item: %s, %d, %d", pItem->pchIdentifier, invtype, itm->amtStored);
        return false;
    }

    return true;
}

static bool RemAmtStored(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem)
{
    InventoryType invtype = InventoryType_FromTrayItemType(pItem->type);
    char *display = s_AuctionItemToDisplayString(pItem);
    
    if(itm->amtStored <= 0) // nothing to do, this is a special case (won't generate messages, stats, etc.)
        return true;

    if(invtype != kInventoryType_Count)
    {
        int invid = genericinv_GetInvIdFromItem(invtype,pItem->pItem);
        if(character_AdjustInventory(e->pchar, invtype, invid, itm->amtStored, "auction", false) < 0)
        {
            plaque_SendString(e, "AuctionPlayerInvFull");
            acLog("AuctionClient:Get", e, cmd, itm, NULL, "Get Failed. inventory full. couldn't get item: %s, %d, %d", pItem->pchIdentifier, invtype, itm->amtStored);
            if(!e->auc_trust_gameclient)
                return false;
        }
        else
            acLog("AuctionClient:Get", e, cmd, itm, NULL, "get succeeded. got item: %s, %d, %d", pItem->pchIdentifier, invtype, itm->amtStored);
    }
    else if(pItem->type == kTrayItemType_Inspiration)
    {
        int i;
        for( i = 0; i < itm->amtStored; i++ )
        {
            if(character_AddInspiration(e->pchar, pItem->inspiration, "auction") == -1)
            {
                plaque_SendString(e, "AuctionPlayerInspInvFull");
                acLog("AuctionClient:Get", e, cmd, itm, NULL, "Get failed. inventory full. couldn't get inspiration: %s", pItem->inspiration->pchFullName);
                if(!e->auc_trust_gameclient)
                    return false;
            }
            else
                acLog("AuctionClient:Get", e, cmd, itm, NULL, "got inspiration: %s", pItem->inspiration->pchFullName);
        }
    }
    else if(pItem->type == kTrayItemType_SpecializationInventory)
    {
        int i;
        for( i = 0; i < itm->amtStored; i++ )
        {
            if(character_AddBoost(e->pchar, pItem->enhancement, pItem->lvl, 0, "auction") == -1 )
            {
                plaque_SendString(e, "AuctionPlayerEnhancementInvFull");
                acLog("AuctionClient:Get", e, cmd, itm, NULL, "Get failed. inventory full. couldn't get enhancement: %s, %d", pItem->enhancement->pchFullName, pItem->lvl);
                if(!e->auc_trust_gameclient)
                    return false;
            }
            else
                acLog("AuctionClient:Get", e, cmd, itm, NULL, "got enhancement: %s, %d", pItem->enhancement->pchFullName, pItem->lvl);
        }
    }

    if(display)
    {
        if(itm->amtStored != 1)
            sendInfoBox(e, INFO_AUCTION, "AuctionNotify_GotCount", display, itm->amtStored);
        else
            sendInfoBox(e, INFO_AUCTION, "AuctionNotify_Got", display);
    }

    if(itm->auction_status == AuctionInvItemStatus_Bidding || itm->auction_status == AuctionInvItemStatus_Bought)
    {
        AuctionDatamine(pItem,
                            "inf_bought",itm->infPrice*itm->amtStored,
                            "bought",itm->amtStored,
                            NULL,0,NULL,0);
    }
    else if(itm->auction_status == AuctionInvItemStatus_ForSale || itm->auction_status == AuctionInvItemStatus_Sold)
    {
        AuctionDatamine(pItem,
                            NULL,0,NULL,0,
                            "inf_forsale_cancelled",itm->infPrice*itm->amtStored,
                            "forsale_cancelled",itm->amtStored);
    }

    return true;
}

static bool VerifyChangeInvStatus(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem)
{
    int fee;
    INT64 total;

    // if you change this, make sure to fix ChangeInvStatus() below
    if(itm->infPrice <= 0 || !devassert(itm->auction_status == AuctionInvItemStatus_ForSale))
    {
        acLog("AuctionClient:ChangeStatus", e, cmd, itm, NULL, "Cheater: Bad AuctionAddInv negative inf (%d) or bad status (%d)", itm->infPrice, itm->auction_status);
        return false;
    }


    fee = calcAuctionSellFee(itm->infPrice) * itm->amtStored;
    if(e->pchar->iInfluencePoints < fee  && !e->auc_trust_gameclient)
    {
        // let the client know this failed.
        plaque_SendString(e, "AuctionNotEnoughInf");
        acLog("AuctionClient:Change", e, cmd, itm, NULL, "Not enough inf. to sell item: %s, %d x %d", pItem->pchIdentifier, itm->amtStored, itm->infPrice);
        return false;
    }

    total = (INT64)itm->infPrice * itm->amtStored;
    if(total > 2000000000 )
    {
        // let the client know this failed.
        plaque_SendString(e, "AuctionTooMuchInf");
        acLog("AuctionClient:Change", e, cmd, itm, NULL, "Too much inf. to sell item: %s, %d x %d", pItem->pchIdentifier, itm->amtStored, itm->infPrice);
        return false;
    }

    return true;
}

static void ChangeInvStatus(Entity *e, XactCmd *cmd, AuctionInvItem *itm, AuctionItem *pItem)
{
    int fee = calcAuctionSellFee(itm->infPrice) * itm->amtStored;

    if(!e->auc_trust_gameclient)
        ent_AdjInfluence(e, -fee, "action.fee");

    acLog("AuctionClient:Change", e, cmd, itm, NULL, "selling item. %d inf fee removed from player", fee);

    if(fee)
    {
        badge_StatAdd(e,"auction.fees",fee);
        badge_StatAdd(e,"auction.fees.post",fee);
        sendInfoBox(e, INFO_AUCTION, "AuctionNotify_PaidInf", fee);
    }

    AuctionDatamine(pItem,"inf_forsale",itm->infPrice*itm->amtStored,"forsale",itm->amtStored,NULL,0,NULL,0);
}

static void MapHandleXactAuctionFinalize(Entity *e, XactCmd *cmd, XactUpdateType update_type)
{
    // todo: when locking is put in place commit/rollback xaction
    // no need to respond. xact server is done with us.

    // update the local inventory based on the response
    switch(cmd->type)
    {
        case XactCmdType_AddInvItem:
        case XactCmdType_GetInvItem:
        case XactCmdType_ChangeInvStatus:
        case XactCmdType_ChangeInvAmount:
        case XactCmdType_GetInfStored:
        case XactCmdType_GetAmtStored:
        {
            int i;
            AuctionInvItem *itmCmd = NULL;

            if(!e || !e->pchar || !e->pchar->auctionInv || !cmd->context)
                break;

            if(!AuctionInvItem_FromStr(&itmCmd,cmd->context))
                break;

            for(i = eaSize(&e->pchar->auctionInv->items)-1; i >= 0; --i)
            {
                AuctionInvItem *itmEnt = e->pchar->auctionInv->items[i];
                if(itmEnt && itmEnt->id == itmCmd->id)
                {
                    if(itmCmd->bDeleteMe)
                    {
                        // delete the old version from the entity's inventory
                        AuctionInvItem_Destroy(itmEnt);
                        eaRemove(&e->pchar->auctionInv->items,i);

                        // don't need the new one anymore
                        AuctionInvItem_Destroy(itmCmd);
                    }
                    else
                    {
                        // use the new version, delete the old one
                        AuctionInvItem_Destroy(itmEnt);
                        e->pchar->auctionInv->items[i] = itmCmd;
                    }
                    break;
                }
            }

            if(i < 0) // not in the entity's inventory
            {
                if(itmCmd->bDeleteMe)
                {
                    // just get rid of the new one
                    AuctionInvItem_Destroy(itmCmd);
                }
                else
                {
                    // must be a brand new item
                    eaPush(&e->pchar->auctionInv->items,itmCmd);
                }
            }

            // send the inventory to the client
            e->pchar->auctionInvUpdated = true;
        }
        xdefault:
            break;
    }

    if(update_type == kXactUpdateType_Abort)
    {
        if(cmd->type == XactCmdType_AddInvItem)
        {
            if(e && e->pchar && e->pchar->auctionInv && e->pchar->auctionInv->invSize &&
                eaSize(&e->pchar->auctionInv->items) >= e->pchar->auctionInv->invSize    )
            {
                dbLog("AuctionClient", e, "Auction inventory full");
                sendDialog(e,localizedPrintf(e,"AuctionInventoryFull"));
            }
        }
    }
}

void MapHandleXactFail(Entity *e, XactCmdType xact_type, char *context)
{
    switch(xact_type)
    {
        case XactCmdType_AddInvItem:
        case XactCmdType_GetInvItem:
        case XactCmdType_ChangeInvStatus:
        case XactCmdType_ChangeInvAmount:
        case XactCmdType_GetInfStored:
        case XactCmdType_GetAmtStored:
            if ( e && e->pchar )
            {
                // send inventory update, to fix client prediction
                e->pchar->auctionInvUpdated = true;
                sendInfoBox(e, INFO_AUCTION, "AuctionServerUnableToRespond");
            }
        xcase XactCmdType_ShardJump:
            dbUndoMapXfer(e,"ShardJump currently unavailable.");
        xdefault:
            devassert(0 && "invalid switch value.");
    }
}

static char* s_AuctionItemToDisplayString(AuctionItem *pItem)
{
    if(!pItem)
        return NULL;

    switch(pItem->type)
    {
    case kTrayItemType_Salvage:
        {
            SalvageItem *salvage = pItem->salvage;
            return salvage ? textStd(salvage->ui.pchDisplayName) : NULL;
        }
    case kTrayItemType_Recipe:
        {
            const DetailRecipe *recipe = pItem->recipe;
            return recipe ? textStd(recipe->ui.pchDisplayName) : NULL;
        }
    case kTrayItemType_SpecializationInventory:
    case kTrayItemType_Inspiration:
        {
            BasePower *power = pItem->inspiration;
            return power ? textStd(power->pchDisplayName) : NULL;
        }
    default:
        return NULL;
    }
}

static char* s_IdentifierStringToDisplayString(const char *pchIdentifier)
{
    AuctionItem * pItem;
    if( stashFindPointer(auctionData_GetDict()->stAuctionItems, pchIdentifier, &pItem) )
        return s_AuctionItemToDisplayString(pItem);
    else
        return "Invalid";
}

void MapHandleXactCmd(Entity *e, XactUpdateType update_type, XactCmd *cmd)
{    
    if(!cmd)
        return;
    else switch ( cmd->type )
    {
    case XactCmdType_AddInvItem:
    case XactCmdType_GetInvItem:
    case XactCmdType_ChangeInvStatus:
    case XactCmdType_ChangeInvAmount:
    case XactCmdType_GetInfStored:
    case XactCmdType_GetAmtStored:
    {
        if(update_type == kXactUpdateType_Run)
        {
            cmd->res = kXactCmdRes_No; // default to no
            if(e && e->pchar) // TODO: being unable to find the entity should probably result in a 'no' response
            {
                AuctionInvItem *itm = NULL;
                if(AuctionInvItem_FromStr(&itm,cmd->context))
                {
                    AuctionItem *pItem;
                    
                    stashFindPointer( auctionData_GetDict()->stAuctionItems, itm->pchIdentifier, &pItem);

                    if(pItem)
                    {
                        switch(cmd->type)
                        {
                            case XactCmdType_AddInvItem:
                            case XactCmdType_ChangeInvAmount: // intentional fallthrough
                            {
                                if(!VerifyAddInvItem(e,cmd,itm,pItem,0))
                                    break;

                                if(!AddAmtStored(e,cmd,itm,pItem))
                                    break;

                                AddInfStored(e,cmd,itm,pItem);
                                cmd->res = kXactCmdRes_Yes;
                            }
                            xcase XactCmdType_ChangeInvStatus:
                            {
                                if(!VerifyChangeInvStatus(e,cmd,itm,pItem))
                                    break;

                                ChangeInvStatus(e,cmd,itm,pItem);
                                cmd->res = kXactCmdRes_Yes;
                            }
                            xcase XactCmdType_GetInvItem:
                            {
                                if(!VerifyRemInfStored(e,cmd,itm,pItem))
                                    break;

                                if(!VerifyRemAmtStored(e,cmd,itm,pItem))
                                    break;

                                if(!RemAmtStored(e,cmd,itm,pItem))
                                    break;

                                RemInfStored(e,cmd,itm,pItem); // this cannot fail, since RemAmtStored cannot be rolled back
                                cmd->res = kXactCmdRes_Yes;
                            }
                            xcase XactCmdType_GetInfStored:
                            {
                                if(!VerifyRemInfStored(e,cmd,itm,pItem))
                                    break;

                                RemInfStored(e, cmd, itm, pItem);
                                cmd->res = kXactCmdRes_Yes;
                            }
                            xcase XactCmdType_GetAmtStored:
                            {
                                if(!VerifyRemAmtStored(e,cmd,itm,pItem))
                                    break;

                                if(!RemAmtStored(e,cmd,itm,pItem))
                                    break;

                                cmd->res = kXactCmdRes_Yes;
                            }
                        }

                        if(cmd->res == kXactCmdRes_Yes) // something changed, persist
                            e->auctionPersistFlag = true;
                    }
                    else
                        acLog("AuctionClient", e, cmd, itm, NULL, "could not find valid AuctionItem in AuctionItemDict");
                    AuctionInvItem_Destroy(itm);
                }
                else
                    acLog("AuctionClient", e, cmd, itm, NULL, "could not create AuctionInvItem from XactCmd context (%s)", cmd->context);
            }
            // TODO: let the client know about failure.
            MapSendXactCmdUpdate(e,cmd); // always respond to the auctionserver
        }
        else
            MapHandleXactAuctionFinalize(e,cmd,update_type);
    }
    xcase XactCmdType_FulfillAuction:
    case XactCmdType_FulfillAutoBuy:
    case XactCmdType_FulfillAutoSell:
    {
        AuctionInvItem *itm = NULL;
        char *display;
        int i;

        if(!e)
            break;
        if(!AuctionInvItem_FromStr(&itm,cmd->context) || !itm)
        {
            acLog("AuctionClient:Fulfill", e, cmd, NULL, NULL, "fulfill: failed to parse AuctionInvItem context %s", cmd->context);
            break;
        }
        display = s_IdentifierStringToDisplayString(itm->pchIdentifier);
        if(!display)
        {
            acLog("AuctionClient:Fulfill", e, cmd, itm, NULL, "fulfill: failed to parse AuctionItem: %s", itm->pchIdentifier);
            AuctionInvItem_Destroy(itm);
            break;
        }
        
        switch(itm->auction_status)
        {
        case AuctionInvItemStatus_ForSale:
        case AuctionInvItemStatus_Sold:
            acLog("AuctionClient:Fulfill",e,cmd,itm,NULL,"item sold");
            sendInfoBox(e, INFO_AUCTION, "AuctionNotify_Sold", display);
            break;
        case AuctionInvItemStatus_Bidding:
        case AuctionInvItemStatus_Bought:
            acLog("AuctionClient:Fulfill",e,cmd,itm,NULL,"item bought");
            sendInfoBox(e, INFO_AUCTION, "AuctionNotify_Bought", display);
            break;
        default:
            acLog("AuctionClient:Fulfill",e,cmd,itm,"unknown status %s:%i", AuctionInvItemStatus_ToStr(itm->auction_status), itm->auction_status);
            break;
        };

        if ( e->pchar && e->pchar->auctionInv )
        {
            for ( i = 0; i < eaSize(&e->pchar->auctionInv->items); ++i )
            {
                if ( itm->id == e->pchar->auctionInv->items[i]->id )
                {
                    e->pchar->auctionInv->items[i]->auction_status = itm->auction_status;
                    e->pchar->auctionInv->items[i]->amtStored = itm->amtStored;
                    e->pchar->auctionInv->items[i]->infStored = itm->infStored;
                    e->pchar->auctionInv->items[i]->amtOther = itm->amtOther;
                    e->pchar->auctionInv->items[i]->infPrice = itm->infPrice;
                    e->pchar->auctionInvUpdated = true;
                    break;
                }
            }
        }

        AuctionInvItem_Destroy(itm);
    }          
    xcase XactCmdType_ShardJump:
    {
        if(update_type == kXactUpdateType_Run) // destination only
        {
            int i;
            for(i = eaSize(&s_shardjumpers)-1; i >= 0; --i)
                if(!s_shardjumpers[i])
                {
                    s_shardjumpers[i] = cmd;
                    break; // FIXME: send a response
                }
            if(i < 0)
            {
                i = eaSize(&s_shardjumpers);
                eaPush(&s_shardjumpers,cmd);
            }
            // FIXME: send a response
            dbAsyncContainerUpdate(CONTAINER_ENTS,-1,CONTAINER_CMD_CREATE,cmd->context,i);
        }
        else if(update_type == kXactUpdateType_Abort) // source only
        {
            dbUndoMapXfer(e,cmd->context);
        }
        else if(update_type == kXactUpdateType_Commit) // source only
        {
            dbAsyncContainerUpdate(CONTAINER_ENTS,e->db_id,CONTAINER_CMD_DELETE,0,0);
        }
    }
    xdefault:
        verify(0 && "invalid switch value.");
    };
}

void AuctionPrepFakeItemList(AuctionMultiContext ***multictxs, AuctionItem *item, int amt, int price)
{
    AuctionInvItem invitem;
    AuctionMultiContext *multictx = NULL;
    char *context = NULL;
    Packet *pak = NULL;

    if (!multictxs || !item || !amt || !price)
        return;

    memset(&invitem, 0, sizeof(AuctionInvItem));

    multictx = calloc(1, sizeof(AuctionMultiContext));

    invitem.id = 0;
    invitem.pchIdentifier = item->pchIdentifier;
    invitem.amtStored = amt;
    invitem.auction_status = AuctionInvItemStatus_Stored;
    AuctionInvItem_ToStr(&invitem, &multictx->context);

    multictx->type = XactCmdType_AddInvItem;
    eaPush(multictxs, multictx);

    multictx = calloc(1, sizeof(AuctionMultiContext));

    invitem.auction_status = AuctionInvItemStatus_ForSale;
    invitem.infPrice = price;
    AuctionInvItem_ToStr(&invitem, &multictx->context);

    multictx->type = XactCmdType_ChangeInvStatus;
    eaPush(multictxs, multictx);
}

void AuctionSendFakeItemList(AuctionMultiContext ***multictx)
{
    Packet *pak;
    int ctxs;
    int count;

    if (multictx && *multictx)
    {
        ctxs = eaSize(multictx);

        if (ctxs)
        {
            pak = pktCreateEx(&db_comm_link, DBCLIENT_AUCTION_XACT_MULTI_REQ);
             pktSendBitsAuto(pak, 1);
            pktSendString(pak, "!!FakeName!!");
            pktSendBitsAuto(pak, ctxs);

            for (count = 0; count < ctxs; count++)
            {
                pktSendBitsAuto(pak, (*multictx)[count]->type);
                pktSendString(pak, (*multictx)[count]->context);
            }

            pktSend(&pak, &db_comm_link); 

            for (count = 0; count < ctxs; count++)
            {
                estrDestroy(&((*multictx)[count]->context));
                free((*multictx)[count]);
                (*multictx)[count] = NULL;
            }

            eaDestroy(multictx);
        }
    }
}
