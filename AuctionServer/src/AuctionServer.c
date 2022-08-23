/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include "AuctionServer.h"
#include "auctioncmds.h"
#include "cmdparse/cmdauction.h"
#include <utilitieslib/network/crypt.h>
#include "AuctionDb.h"
#include "XactServer.h"
#include <utilitieslib/components/StringCache.h>
#include "auctionfulfill.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/network/net_masterlist.h>
#include <utilitieslib/components/StashTable.h>
#include "auction/Auction.h"
#include <utilitieslib/network/net_linklist.h>
#include <utilitieslib/network/net_link.h>
#include "comm_backend.h"
#include <utilitieslib/utils/ConsoleDebug.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include "auctionhistory.h"
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/log.h>
#include "account/AccountTypes.h"
#include "AuctionSql.h"

#define MIN_THROTTLE (1.0f)
#define MAX_THROTTLE (8.0f)
#define MAX_FLUSH_TIME (2.0f)

int g_bVerbose = false;
int g_frame_timer = 0;
int g_flush_timer = 0;
bool g_bThrottled = false;
int g_packetcount = 0;

// #define SHARDRECONNECT_RETRY_TIME    (isDevelopmentMode()?5:30)
#define SHARDRECONNECT_RETRY_TIME    (isDevelopmentMode()?15:120)
#define LINKS_PER_SHARD                4
#define SHARD_DEADMAN_TIME            (60*2)

#define LOGID "AuctionServer"
typedef struct NetLink NetLink;

char *strcatType(char *str,AuctionServerType type)
{
    THREADSAFE_STATIC char *buf = NULL;
    THREADSAFE_STATIC_MARK(buf);
    estrPrintf(&buf,"%s%s",str,AuctionServerType_ToStr(type)); 
    return buf;
}

static TokenizerParseInfo parse_AuctionServerCfg[] =  
{
    { "{",        TOK_START,       0 },
    { "ShardIp",    TOK_STRINGARRAY(AuctionServerCfg, shardIps)},
    { "AutoFulfillTime",    TOK_INT(AuctionServerCfg, iAutoFulfillTime, 0)    },
    { "AutoSellPrice",    TOK_INT(AuctionServerCfg, iAutoSellPrice, 0)    },
    { "AutoBuyPrice",    TOK_INT(AuctionServerCfg, iAutoBuyPrice, 0)    },
    { "SqlLogin",        TOK_FIXEDSTR(AuctionServerCfg, sqlLogin) },
    { "SqlDbName",        TOK_FIXEDSTR(AuctionServerCfg, sqlDbName) },

    { "}",            TOK_END,         0 },
    { 0 },
};


typedef struct AucShardLink
{    
    AuctionShard *shard;
    AuctionServerState *state;
} AucShardLink;
MP_DEFINE(AucShardLink);
static AucShardLink* AucShardLink_Create(void)
{
    AucShardLink *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(AucShardLink, 64);
    res = MP_ALLOC( AucShardLink );
    if( devassert( res ))
    {
    }
    return res;
}

static void AucShardLink_Destroy( AucShardLink *hItem )
{
    if(hItem)
    {
        MP_FREE(AucShardLink, hItem);
    }
}




AuctionServerState g_auctionstate;

// void shardDisconnectAll()
// {
//      netLinkListDisconnect(&g_auctionstate.db_links); 
// }

// void serverShutdown()
// {
//     // TODO
// }

// #define INITIAL_CLIENT_COUNT 12

// int auction_clientCreateCb(NetLink *link)
// {
//     netLinkSetMaxBufferSize(link, BothBuffers, 8*1024*1024); // Set max size to auto-grow to
//     netLinkSetBufferSize(link, BothBuffers, 1*1024*1024);
//     return 1;
// }

int auction_clientDeleteCb(NetLink *link)
{
    AuctionShard    *client = link->userData;
    // TODO: link loss
    return 1;
}


void AuctionServer_SendInv(NetLink *link, AuctionShard *shard, int ident)
{
    AuctionInventory null_inv = {0};
    char *invstr = NULL;
    AuctionEnt *ent;
    Packet *pak;

    if(!shard || !link || ident <= 0)
        return;

    ent = AuctionDb_FindEnt(shard,ident);
    if(ent)
        AuctionInventory_ToStr(&ent->inv, &invstr);
    else
        AuctionInventory_ToStr(&null_inv, &invstr);

    pak = pktCreateEx(link,AUCTION_SVR_SEND_INV);
    pktSendBitsAuto(pak, ident);
    pktSendString(pak, invstr);
    pktSend(&pak, link);

    estrDestroy(&invstr);
}

AuctionInvItem *AuctionServer_AddInvItm(AuctionEnt *ent, AuctionInvItem *itemReq)
{
    AuctionInvItem *itm = NULL;
    if(!devassert(ent))
        return NULL;

    // Fake agents don't care about the inventory limit because they never
    // have to use the UI or send the details to anybody else.
    if(ent->inv.invSize && !AuctionEnt_IsFakeAgent(ent) && eaSize(&ent->inv.items)>=ent->inv.invSize)
    {
        LOG( LOG_AUCTION_TRANSACTION, LOG_LEVEL_IMPORTANT, 0, "%s:%s(%i) trying to add item to full inventory",(ent->shard?ent->shard->name:"no shard"),ent->nameEnt,ent->ident);
    }
    else
        itm = AuctionEnt_AddItem(ent,itemReq);
    
    if(itm)
        auctionfulfiller_AddInvItm(itm);

    return itm;
}

AuctionInvItem *AuctionServer_FindInvItm(AuctionEnt *ent, int id)
{
    int i;
    if(!devassert(ent))
        return NULL;
    for(i = eaSize(&ent->inv.items) - 1; i >= 0; --i)
        if(ent->inv.items[i] && ent->inv.items[i]->id == id)
            return ent->inv.items[i];

    return NULL;
}

void AuctionServer_RemInvItm(AuctionInvItem *itm)
{
    if(!itm)
        return;

    if(!itm->lockid)
        auctionfulfiller_RemoveInvItm(itm); 

    if(itm->ent)
        eaFindAndRemove(&itm->ent->inv.items,itm);
    AuctionInvItem_Destroy(itm);
}

// FIXME: this name sucks
AuctionInvItem *AuctionShard_ChangeInvItemStatus(AuctionInvItem *itm, int price, AuctionInvItemStatus status)
{
    if(!itm)
        return NULL;
    
    if(itm->lockid)
        return NULL;

    auctionfulfiller_RemoveInvItm(itm);

    // Start the timer for auto-fulfillment as long as we're switching into
    // buy mode or sell mode and auto-fulfill is enabled.
    if(status != itm->auction_status && g_auctionstate.cfg.iAutoFulfillTime && (status == AuctionInvItemStatus_ForSale || status == AuctionInvItemStatus_Bidding))
        itm->autofulfilltime = timerSecondsSince2000() + g_auctionstate.cfg.iAutoFulfillTime;

    itm->auction_status = status;
    itm->infPrice = price;
    auctionfulfiller_AddInvItm(itm);
    return itm;
}

AuctionInvItem *AuctionShard_ChangeInvItemAmount(AuctionInvItem *itm, int amtStored, int amtOther, int infStored)
{
    if(!itm)
        return NULL;
    
    if(itm->lockid)
        return NULL;

    if(itm->amtStored + amtStored < 0 || itm->amtOther + amtOther < 0 || itm->infStored + infStored < 0)
        return NULL;

    auctionfulfiller_RemoveInvItm(itm);
    itm->amtStored += amtStored;
    itm->amtOther += amtOther;
    itm->infStored += infStored;
    // TODO: Get rid of AuctionInvItemStatus_Sold and AuctionInvItemStatus_Bought
    if(!itm->amtStored && itm->auction_status == AuctionInvItemStatus_ForSale)
        itm->auction_status = AuctionInvItemStatus_Sold;
    else if ( itm->auction_status == AuctionInvItemStatus_Sold && itm->amtStored ) 
        itm->auction_status = AuctionInvItemStatus_ForSale;
    if(!itm->amtOther && itm->auction_status == AuctionInvItemStatus_Bidding)
        itm->auction_status = AuctionInvItemStatus_Bought;
    else if ( itm->auction_status == AuctionInvItemStatus_Bought && itm->amtOther )
        itm->auction_status = AuctionInvItemStatus_Bidding;
    auctionfulfiller_AddInvItm(itm);
    return itm;
}

static int s_AuctionShard_RemoveItemIfInvalid(AuctionInvItem *itm)
{
    int removed = 0;
    if ( itm->infStored == 0 && itm->amtStored == 0 )
    {
        if(itm->ent)
            eaFindAndRemove(&itm->ent->inv.items,itm);
        AuctionInvItem_Destroy(itm);
        removed = 1;
    }
    return removed;
}

int AuctionShard_SetInfStored(AuctionInvItem *itm, int newInfStored)
{
    if(!itm)
        return 0;
    
    if(itm->lockid)
        return 0;

    auctionfulfiller_RemoveInvItm(itm);
    itm->infStored = newInfStored;

    if ( itm->infStored == 0 && itm->amtOther > 0 )
    {
        itm->amtOther = 0;
        if ( itm->auction_status == AuctionInvItemStatus_Bidding && itm->amtStored)
            itm->auction_status = AuctionInvItemStatus_Bought;
    }

    return 1;
}

int AuctionShard_SetAmtStored(AuctionInvItem *itm, int newAmtStored)
{
    if(!itm)
        return 0;
    
    if(itm->lockid)
        return 0;

    auctionfulfiller_RemoveInvItm(itm);
    itm->amtStored = newAmtStored;

    if(!itm->amtStored && itm->auction_status == AuctionInvItemStatus_ForSale)
        itm->auction_status = AuctionInvItemStatus_Sold;
    else if(itm->amtStored && itm->auction_status == AuctionInvItemStatus_Sold)
        itm->auction_status = AuctionInvItemStatus_ForSale;

    return 1;
}

static void AuctionServer_HandleHistoryInfoRequest(NetLink *link, AuctionShard *shard, int ident, const char *pchIdentifier)
{
    AuctionHistoryItem *histItm = auctionhistory_Get(pchIdentifier);
    int i, buying, selling, count = histItm ? eaSize(&histItm->histories) : 0;
    Packet *pak;

    if(!link || !shard)
        return;

    auctionfulfiller_GetItemInfo(pchIdentifier,&buying,&selling);

    pak = pktCreateEx(link,AUCTION_SVR_SEND_HISTORYINFO);
    pktSendBitsAuto(pak,ident);
    pktSendString(pak,pchIdentifier);
    pktSendBitsAuto(pak,buying);
    pktSendBitsAuto(pak,selling);
    pktSendBitsAuto(pak,count);
    for(i = 0; i < count; ++i)
    {
        pktSendBitsAuto(pak,histItm->histories[i]->date);
        pktSendBitsAuto(pak,histItm->histories[i]->price);
    }
    pktSend(&pak,link);
}

void AuctionServer_HandleBatchStatus(void)
{
    int i, size = auctionfullfiller_CatsCount();
    char *str = NULL;

    estrPrintCharString(&str,"");

    for ( i = 0; i < size; ++i )
    {
        const char *pchIdentifier;
        int buying, selling;
        auctionfulfiller_GetItemInfoByIdx(i,&pchIdentifier,&buying,&selling);
        estrConcatf(&str,"%s\n%d\n%d\n",pchIdentifier,buying,selling);
    }

    size = eaSize(&g_AuctionDb.shards);
    for(i = 0; i < size; ++i)
    {
        AuctionShard *shard = g_AuctionDb.shards[i];
        if(shard && shard->data_ok)
        {
            NetLink *link = AuctionShard_GetLink(shard);
            if(link)
            {
                Packet *pak_out = pktCreateEx(link,AUCTION_SVR_BATCH_SEND_ITEMINFO);
                if(pak_out)
                {
                    pktSendBitsAuto(pak_out, 0);
                    pktSendZipped(pak_out, estrLength(&str)+1, str); // TODO: zip once
                    pktSend(&pak_out, link);

                    // don't send any more big packets until this one goes through
                    shard->data_ok = false;
                }
            }
        }
    }

    estrDestroy(&str);
}

static void AuctionServer_SendThrottleCommand(int cmd)
{
    int i;
    int size = eaSize(&g_AuctionDb.shards);
    for(i = 0; i < size; ++i)
    {
        AuctionShard *shard = g_AuctionDb.shards[i];
        if(shard)
        {
            NetLink *link = AuctionShard_GetLink(shard);
            if(link)
            {
                Packet *pak_out = pktCreateEx(link,cmd);
                if(pak_out)
                    pktSend(&pak_out, link);
            }
        }
    }

    lnkFlushAll();
}

static void s_AuctionServer_FailLink(NetLink *link);

// We don't immediately do the string compare because most of the time this
// will not be a fake agent request and so it would be a waste of CPU time.
#define AUC_RECV() \
        int ident = pktGetBitsAuto(pak); \
        char *name; \
        strdup_alloca( name,pktGetString(pak)); \
        if( name && name[0] == '!' && strnicmp(name, "!!FakeName", 10) == 0 ) \
            shard = AuctionDb_GetFakeShard();

int shardMsgCallback(Packet *pak, int cmd, NetLink *link)
{
    int cursor = bsGetCursorBitPosition(&pak->stream); // for debugging
    AucShardLink *link_shard = link->userData;
    AuctionShard *shard = link_shard->shard;

    g_packetcount++;

    // If we've spent more than MAX_THROTTLE seconds dealing with new
    // packets, tell the dbservers to stop sending new transactions
    // so we can catch up.
    if(!g_bThrottled && timerElapsed(g_frame_timer) > MAX_THROTTLE)
    {
        AuctionServer_SendThrottleCommand(AUCTION_SVR_THROTTLE_ON);
        g_bThrottled = true;

        if(g_bVerbose)
        {
            F32 elapsed = timerElapsed(g_frame_timer);

            printf("\n");
            printf("Throttle: On  (%f secs)\n", elapsed);
        }
    }

    // This forces data to go back to the dbserver a little more smoothly
    // when there's a huge number of incoming packets.
    if(timerElapsed(g_flush_timer) > MAX_FLUSH_TIME)
    {
        lnkFlushAll();
        timerStart(g_flush_timer);
    }

    if(shard)
    {
        // this should happen at least every time we send a batch update
        shard->last_received = timerSecondsSince2000();
    }
    else if(cmd != AUCTION_CLIENT_CONNECT)
    {
        LOG( LOG_AUCTION, LOG_LEVEL_VERBOSE, 0, "received cmd (%i) before shard connect",cmd);
        return 1;
    }

    switch ( cmd )
    {
    case AUCTION_CLIENT_CONNECT: // shard connect
    {
        U32 protocol;
        char *shardname;

        protocol = pktGetBitsPack(pak,1);
        if(protocol != DBSERVER_AUCTION_PROTOCOL_VERSION)
        {
            printf("Info: dbserver at %s needs to be upgraded, rechecking version in %is. (got %i, need %i)\n",
                makeIpStr(link->addr.sin_addr.S_un.S_addr),SHARDRECONNECT_RETRY_TIME,protocol,DBSERVER_AUCTION_PROTOCOL_VERSION);
            s_AuctionServer_FailLink(link);
            break;
        }

        strdup_alloca(shardname,pktGetString(pak));

        if(!shardname || !shardname[0] || !StructLinkValidName(shardname))
        {
            printf("invalid name %s for shard at %s. disconnecting.\n",shardname,makeIpStr(link->addr.sin_addr.S_un.S_addr));
            s_AuctionServer_FailLink(link);
            break;
        }

        shard = AuctionDb_FindAddShard(shardname);
        if(!devassert(shard))
            break;

        if(eaFind(&shard->links,link) < 0)
            eaPush(&shard->links,link);
        shard->last_received = timerSecondsSince2000();

        if(!link_shard->shard)
            link_shard->shard = shard;
        if(!link_shard->state)
            link_shard->state = &g_auctionstate;

        // TODO: the data_ok flag is a very simple implementation.
        // instead, we might want to keep track of which link cleared the flag
        // and only set it when that link gets a response or reconnects.
        // currently, if one link reconnects while another is waiting on a
        // data_ok response, the data_ok flag will be set erroneously
        shard->data_ok = true;
        
        if(pktGetBits(pak,1)) // DBServer is sending a list of recent ents
        {
            int i;
            StashTableIterator is;
            StashElement elem;

            int nEnts = pktGetBitsAuto(pak);
            StashTable seen_ents = stashTableCreateInt(nEnts);
            AuctionEnt **ents_to_remove = NULL;

            for( i = 0; i < nEnts; ++i ) 
            {
                int ident = pktGetBitsAuto(pak);
                AuctionEnt *ent = AuctionDb_FindEnt(shard,ident);
                if(ent)
                    stashIntAddPointer(seen_ents,ident,ent,false);
            }

            // finally, remove any obsolete characters
            if(nEnts) // just to prevent anything stupid
            {
                for(stashGetIterator( shard->entById,&is);stashGetNextElement(&is,&elem);)
                {
                    AuctionEnt *ent = stashElementGetPointer( elem );
                    if(!stashIntFindPointer(seen_ents,ent->ident,NULL))
                        eaPush(&ents_to_remove,ent);
                }
            }
            else
                printf("DBServer said there were no players on recently!.. Ignoring this.\n");

            for( i = eaSize( &ents_to_remove ) - 1; i >= 0; --i)
                XactServer_AddReq(XactCmdType_DropAucEnt,"Not on DBServer fresh list",shard,ents_to_remove[i]->ident,ents_to_remove[i]->nameEnt);

            eaDestroy(&ents_to_remove);
            stashTableDestroy(seen_ents);
        }
    }
    break;
    case AUCTION_CLIENT_INVENTORY_REQUEST: 
    {
        AUC_RECV();
        AuctionServer_SendInv(link,shard,ident);
    }
    break;
    case AUCTION_CLIENT_HISTORYINFO_REQUEST:
    {
        char *pchIdentifier;
        AUC_RECV();
        pchIdentifier = pktGetString(pak);
        if(pchIdentifier)
            AuctionServer_HandleHistoryInfoRequest(link, shard, ident, pchIdentifier);
    }
    break;
    case AUCTION_CLIENT_XACT_REQ:
    {
        XactCmdType xact_type;
        char *context = NULL;
        AUC_RECV();
        xact_type = pktGetBitsAuto(pak);
        context = pktGetString(pak);
        strdup_alloca(context,context);
        XactServer_AddReq(xact_type,context,shard,ident,name);
    }
    break;
    case AUCTION_CLIENT_XACT_MULTI_REQ:
    {
        int xact_count = 0;
        int count;
        int xact_type;
        char *xact_context;
        AUC_RECV();
        xact_count = pktGetBitsAuto(pak);
        for (count = 0; count < xact_count; count++)
        {
            xact_type = pktGetBitsAuto(pak);
            strdup_alloca(xact_context, pktGetString(pak));
            XactServer_AddReq(xact_type,xact_context,shard,ident,name);
        }
    }
    break;
    case AUCTION_CLIENT_XACT_UPDATE:
    {
        int xid = pktGetBitsAuto(pak);
        int subid = pktGetBitsAuto(pak);
        XactCmdRes res = pktGetBitsAuto(pak);
        XactServer_AddUpdate(xid,subid,res);
    }
    break;
    case AUCTION_CLIENT_SLASHCMD:
    {
        AuctionEnt *ent;
        char *str;
        int message_dbid;
        AUC_RECV();
        message_dbid = pktGetBitsAuto(pak);
        strdup_alloca(str,pktGetString(pak));
        ent = AuctionDb_FindEnt(shard,ident);
        if(ent)
            AuctionServer_HandleSlashCmd(ent,message_dbid,str);
    }
    break;
    case AUCTION_CLIENT_DATAOK:
    {
        if(devassert(shard))
            shard->data_ok = true; // feel free to send big data again
    }
    break;
    case AUCTION_CLIENT_ACCOUNT_SHARDXFER:
    {
        OrderId order_id = { pktGetBitsAuto2(pak), pktGetBitsAuto2(pak) };
        int ident = pktGetBitsAuto(pak);
        AuctionEnt *ent = AuctionDb_FindEnt(shard,ident);
        char *dst_shardname = pktGetString(pak);
        AuctionShard *dst_shard = AuctionDb_FindShard(dst_shardname);
        int dst_ident = pktGetBitsAuto(pak);

        if(devassert(dst_shard))
        {
            Packet *pak_out;

            if(ent)
            {
                AuctionEnt_ChangeShard(ent,dst_shard,dst_ident);
            }
            // else the character had no auction inventory

            pak_out = pktCreateEx(link,AUCTION_SVR_ACCOUNT_SHARDXFER);
            pktSendBitsAuto2(pak_out,order_id.u64[0]);
            pktSendBitsAuto2(pak_out,order_id.u64[1]);
            pktSendBitsAuto(pak_out,ident);
            pktSend(&pak_out,link);
        }
        else
        {
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "Nonexistant destination shard (%s) for xfer (ent %s(%d)%s)", dst_shardname,shard->name,ident,ent?ent->nameEnt:"[null ent]");
        }
    }
    break;
    case AUCTION_CLIENT_PURGE_FAKE:
    {
        int count;
        int count2;
        AuctionShard *shard = AuctionDb_GetFakeShard();
        AuctionEnt *ent;
        AuctionInvItem *item;

        for(count = 0; count < eaSize(&(shard->ents)); count++)
        {
            ent = shard->ents[count];

            if(ent && !ent->deleted && AuctionEnt_IsFakeAgent(ent))
            {
                for(count2 = eaSize(&ent->inv.items) - 1; count2 >= 0; count2--)
                {
                    item = ent->inv.items[count2];

                    if(item && !item->lockid && !item->bDeleteMe)
                    {
                        auctionfulfiller_RemoveInvItm(item); 
                        eaRemove(&ent->inv.items, count2);
                        AuctionInvItem_Destroy(item);
                        AuctionDb_EntInsertOrUpdate(ent);
                    }
                }
            }
        }
    }
    break;
    case AUCTION_CLIENT_SET_LOG_LEVELS:
        {
            int i;
            for( i = 0; i < LOG_COUNT; i++ )
            {
                int log_level = pktGetBitsPack(pak,1);
                logSetLevel(i,log_level);
            }
        }break;

    case AUCTION_CLIENT_TEST_LOGS:
        {
            LOG( LOG_DEPRECATED, LOG_LEVEL_ALERT, LOG_CONSOLE, "AuctionServer Log Line Test: Alert" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "AuctionServer Log Line Test: Important" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_VERBOSE, LOG_CONSOLE, "AuctionServer Log Line Test: Verbose" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_DEPRECATED, LOG_CONSOLE, "AuctionServer Log Line Test: Deprecated" );
            LOG( LOG_DEPRECATED, LOG_LEVEL_DEBUG, LOG_CONSOLE, "AuctionServer Log Line Test: Debug" );
        }break;

    default:
        printf("shardMsgCallback invalid switch value. %i\n",cmd);
    };
    return 1;
}

// static void auctionSvrInitNet(AuctionServerState *state)
// {
//     netLinkListAlloc(&state->db_links,INITIAL_CLIENT_COUNT,sizeof(AucShardLink),auction_clientCreateCb);
//     netInit(&state->db_links,0, DEFAULT_AUCTIONSERVER_HEROES_PORT+state->cfg.type);
//     state->db_links.destroyCallback = auction_clientDeleteCb;
//     NMAddLinkList(&state->db_links, shardMsgCallback);
// }

static void s_AuctionServer_PrepFulfiller(void)
{
    int i;

    auctionfulfiller_Init();
    for(i = eaSize(&g_AuctionDb.ents)-1; i >= 0; --i)
    {
        AuctionEnt *ent = g_AuctionDb.ents[i];
        if(ent)
            auctionfulfiller_AddInv(&ent->inv);
    }
}


static void AuctionServer_InitData(void)
{
    // todo: system wide mutex in this function
    HANDLE h_initmtx = CreateMutexA(NULL,0,"Paragon_AuctionShard_InitData_Mutex");
    assert(h_initmtx);
    WaitForSingleObject(h_initmtx,INFINITE);

    // we need shards first, for text parser links
    // note: new shards are written to disk before the journal can reference them
    loadstart_printf("Loading Shards...");
    AuctionDb_LoadShards();
    loadend_printf("");

    // load the ents database and merge in journals
    loadstart_printf("Loading Auction Ents...");
    AuctionDb_LoadEnts();
    loadend_printf("");

    // we are in the main process, do any other loading/fixup/prep
    // if we recovered any journals, then we will be spawning a merge process next tick
    loadstart_printf("Fixup Auction Ents...");
    AuctionDb_Fixup();
    s_AuctionServer_PrepFulfiller();
    loadend_printf("");

    loadstart_printf("Loading Auction History...");
    AuctionHistory_Load();
    loadend_printf("");

    CloseHandle(h_initmtx);
}

// *********************************************************************************
//  main loop
// *********************************************************************************

U32        g_tickCount;
#define AUCTIONSERVER_TICK_FREQ 1 //ticks/sec

void UpdateConsoleTitle(void)
{
    char buf[200];
    int i, nConnected = 0;
    for( i = eaSize(&g_AuctionDb.shards) - 1; i >= 0; --i)
    {
        AuctionShard *s = g_AuctionDb.shards[i];
        if(s)
        {
            int j;
            for(j = eaSize(&s->links)-1; j >= 0; --j)
                if(s->links[j] && s->links[j]->connected)
                    break;
            if(j >= 0)
                ++nConnected;
        }
    }

    sprintf(buf, "%i: AuctionServer - %s. %i shards connected (%i). ",_getpid(), 
        g_auctionstate.server_name ? g_auctionstate.server_name : "Unknown",
        nConnected,
        (g_tickCount / AUCTIONSERVER_TICK_FREQ) % 10);
    setConsoleTitle(buf);
}

bool auctionSvrCfgLoad(AuctionServerCfg *cfg)
{
     char fnbuf[MAX_PATH];
     char *fn = fileLocateRead("server/db/auction_server.cfg",fnbuf);
    bool success = false;
     if(fn && ParserLoadFiles(NULL,fn,NULL,0,parse_AuctionServerCfg,cfg,NULL,NULL,NULL))
         success = true;

    if(!success)
        return false;

    if(cfg->iAutoFulfillTime != 0 && cfg->iAutoFulfillTime < AUCTION_AUTO_FULFILL_MIN_TIME)
    {
        // XXX: Show error - auto-fulfill wait time is too short.
        cfg->iAutoFulfillTime = 0;
    }

    // No need for time to be in seconds, minutes is more understandable.
    cfg->iAutoFulfillTime *= 60;

    if(cfg->iAutoSellPrice != 0 && cfg->iAutoSellPrice < AUCTION_AUTO_SELL_MIN_PRICE)
    {
        // XXX: Show error - maximum buy-from-player price is too low.
        cfg->iAutoSellPrice = 0;
    }

    if(cfg->iAutoBuyPrice != 0 && cfg->iAutoBuyPrice > AUCTION_AUTO_BUY_MIN_PRICE)
    {
        // XXX: Show error - minimum sale-to-player price is too high.
        cfg->iAutoBuyPrice = 0;
    }

    if(cfg->iAutoSellPrice && cfg->iAutoBuyPrice &&    cfg->iAutoBuyPrice <= cfg->iAutoSellPrice)
    {
        // XXX: Show error - prices allow buy-and-sell for profit.
        cfg->iAutoSellPrice = 0;
        cfg->iAutoBuyPrice = 0;
    }

    return success;
}

void shardNetConnectTick(AuctionServerState *state)
{
    AuctionServerCfg *cfg = &state->cfg;
    int shard_count =  eaSize(&cfg->shardIps);
    U32 retry_time = SHARDRECONNECT_RETRY_TIME;
    bool retry_failed_ok = state->last_failed_time + retry_time < timerSecondsSince2000();
    int next_retry_shard = state->next_retry_shard;
    U32 now = timerSecondsSince2000();
    int i;

    // first alloc links if need be
    if(!state->shard_links)
    {
        assert(shard_count <= 32);
        state->shard_links = calloc(sizeof(*state->shard_links),shard_count*LINKS_PER_SHARD);
        for(i = 0; i < shard_count*LINKS_PER_SHARD; ++i)
        {
            NetLink *l = state->shard_links+i;
            initNetLink(l);
        }
    }

    // disconnect any shards that we haven't heard from in a while
    for(i = eaSize(&g_AuctionDb.shards)-1; i >= 0; --i)
    {
        AuctionShard *shard = g_AuctionDb.shards[i];
        if(shard && eaSize(&shard->links) && shard->last_received + SHARD_DEADMAN_TIME < now)
        {
            int j;

            printf("Haven't heard from %s dbserver in over %is, disconnecting.\n",shard->name,SHARD_DEADMAN_TIME);

            for(j = eaSize(&shard->links)-1; j >= 0; --j)
            {
                NetLink *link = shard->links[j];
                if(link && link->connected)
                    netSendDisconnect(link,0);
            }
            eaSetSize(&shard->links,0);
            // we don't need to mark this shard as a failure, we want to try again immediately
        }
    }

    // connect to any disconnected shards
    for(i = 0; i < shard_count; ++i)
    {
        int shardip_idx = (next_retry_shard + i) % shard_count;
        char *shardip = cfg->shardIps[shardip_idx];

        int j;
        for(j = 0; j < LINKS_PER_SHARD; ++j)
        {
            NetLink *link = &state->shard_links[shardip_idx*LINKS_PER_SHARD+j];

            // if the link is down, try to reconnect if the last attempt on that shard was good or
            // the last failed attempt was a while ago
            if( !link->connected && (!(state->failed_shards & (1<<shardip_idx)) || retry_failed_ok) )
            {
                char *ipstr;
                loadstart_printf("connecting (%i) to %s...",j+1,shardip);
                ipstr = makeIpStr(ipFromString(shardip));
                if(!netConnect(link,ipstr, DEFAULT_AUCTIONSERVER_HEROES_PORT,NLT_TCP,1.f,NULL))
                {
                    link->last_update_time = timerCpuSeconds();

                    state->failed_shards |= (1<<shardip_idx); // put or keep the shard on the failure list
                    state->next_retry_shard = shardip_idx + 1; // this will wrap
                    state->last_failed_time = timerSecondsSince2000(); // don't try a known failure again for while
                    retry_failed_ok = false;

                    loadend_printf("failed, retry in %is",retry_time);
                }
                else
                {
                    Packet *pak;
                    if(!link->userData)
                        link->userData = AucShardLink_Create();
                    link->compressionDefault = 1;
                    pak = pktCreateEx(link,AUCTION_SVR_CONNECT);
                    pktSend(&pak,link);

                    state->failed_shards &= ~(1<<shardip_idx); // take this shard off the failure list

                    loadend_printf("done");
                }
            }
        }
    }
}

void shardNetProcessTick(AuctionServerState *state)
{
    int i;
    int link_count =  eaSize(&state->cfg.shardIps) * LINKS_PER_SHARD;

    for(i = 0; i < link_count; ++i)
    {
        NetLink *link = &state->shard_links[i];
        if(link->connected)
        {
            netLinkMonitor(link, 0, shardMsgCallback);
            lnkBatchSend(link);
            link->last_update_time = timerCpuSeconds(); // reset on disconnect in netLinkMonitor
        }
    }
}

static void s_AuctionServer_FailLink(NetLink *link)
{
    // we have to figure out which shard this is, based on the pointer, since userdata isn't necessarily defined
    int shardip_idx = (link - g_auctionstate.shard_links) / LINKS_PER_SHARD;
    netSendDisconnect(link,0);
    link->last_update_time = timerCpuSeconds();
    if(devassert(EAINRANGE(shardip_idx,g_auctionstate.cfg.shardIps)))
    {
        g_auctionstate.failed_shards |= (1<<shardip_idx);
        g_auctionstate.last_failed_time = timerSecondsSince2000();
        // since this wasn't caused by something that stalled the server,
        // we don't want to update next_retry_shard
    }
}


static void memPrompt(void) { printf("Type 'DUMP' to continue: "); }
static void memDump(char* param) { if (strcmp(param, "DUMP")==0) memCheckDumpAllocs(); }

static void conShowShards(void)
{
    int i;
    U32 now = timerSecondsSince2000();
    int n = eaSize(&g_AuctionDb.shards);
    for( i = 0; i < n; ++i ) 
    {
        int j, link_count = 0;
        AuctionShard *s = g_AuctionDb.shards[i];
        U32 ip = 0;
        if(!s)
            continue;

        for(j = eaSize(&s->links)-1; j >= 0; --j)
        {
            if(s->links[j] && s->links[j]->connected)
            {
                ++link_count;
                ip = s->links[j]->addr.sin_addr.S_un.S_addr;
            }
            else
                eaRemove(&s->links,j);
        }

        if(link_count)
        {
            printf("console: shard %s(%s): %i connections, silent %is\n",
                s->name, makeIpStr(ip), link_count, now - s->last_received);
        }
        else
        {
            printf("console: shard %s: disconnected\n", s->name);
        }
    }
}

static void toggleVerboseInfo(void)
{
    g_bVerbose = !g_bVerbose;

    printf("console", "verbose: %s", g_bVerbose ? "on" : "off");
}

ConsoleDebugMenu auctionserverdebug[] =
{
//    { 0,        "Xaction commands:",            NULL,                    NULL        },
//     { 'A',        "use ent A",                    conSetA,                NULL        },
//     { 'B',        "use ent B",                    conSetB,                NULL        },
//    { 'U',        "set user id",                    NULL,                    SetUid        },
//     { 'i',        "inventory",                    conInv,                    NULL        },
//    { 'b',        "buy foo",                        conBuyFoo,                NULL        },
//    { 's',        "sell foo",                        conSellFoo,                NULL        },
//    { 'c',        "commit xactions",                conCommit,                NULL        },

    { 0,        "General commands:",            NULL,                    NULL        },
    { 'M',        "dump memory table",            memPrompt,                memDump        },
    { 'd',        "list connected shards",        conShowShards,            NULL        },
    { 'm',        "print memory usage",            memMonitorDisplayStats,    NULL        },
    { 'v',        "toggle verbose info",            toggleVerboseInfo,        NULL        },

//     { 0,        "Other commands:",                NULL,                    NULL        },
//     { 'Z',        "debug, don't use",                NULL,                    NULL        },

    { 0, NULL, NULL, NULL },
}; 

static char g_cmdline[1024];

static void __cdecl at_exit_func(void)
{
    printf_stderr("Quitting: %s\n",g_cmdline);

    aucsql_close();

    printf("Quit done.\n");
    logShutdownAndWait();
}

static BOOL s_CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
        #define QUIT_CASE(event)                                            \
            case event:                                                        \
                printf("Control Event \n" #event " (%i)", event);    \
                g_auctionstate.quitting = 1;                                \
                for(;;) /* wait for the main thread to call exit */            \
                    Sleep(10);                                                \
                break;

        QUIT_CASE(CTRL_C_EVENT);
        QUIT_CASE(CTRL_BREAK_EVENT);
        QUIT_CASE(CTRL_CLOSE_EVENT);
        QUIT_CASE(CTRL_LOGOFF_EVENT);
        QUIT_CASE(CTRL_SHUTDOWN_EVENT);

        #undef QUIT_CASE
    }
    return FALSE;
}

char g_exe_name[MAX_PATH];
int main(int argc,char **argv)
{
//     HANDLE h_servertypemtx; // a little too crazy, don't worry about this.
    int i;
    int timer;
    int send_timer;
    int startup_timer = 0;

    memCheckInit();

    startup_timer = timerAlloc();

//     bool binheap_Test();
//     binheap_Test();
    strcpy_s(SAFESTR(g_exe_name),argv[0]);
    strcpy(g_cmdline,GetCommandLineA());
    
    EXCEPTION_HANDLER_BEGIN 
    setAssertMode(ASSERTMODE_MINIDUMP | ASSERTMODE_DEBUGBUTTONS);
    FatalErrorfSetCallback(NULL); // assert, log, and exit
    cryptInit();
    for(i = 0; i < argc; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");

    atexit( at_exit_func );
    SetConsoleCtrlHandler((PHANDLER_ROUTINE) s_CtrlHandler, TRUE); // add to list

    setWindowIconColoredLetter(compatibleGetConsoleWindow(), '$', 0xffff00);
    FolderCacheChooseMode();
    preloadDLLs(0); 

    if (fileIsUsingDevData()) { 
        bsAssertOnErrors(true);
        setAssertMode(ASSERTMODE_DEBUGBUTTONS |
            (!IsDebuggerPresent()? ASSERTMODE_MINIDUMP : 0));
    } else {
        // In production mode on the servers we want to save all dumps and do full dumps
        setAssertMode(ASSERTMODE_DEBUGBUTTONS |
            ASSERTMODE_FULLDUMP |
            ASSERTMODE_DATEDMINIDUMPS | ASSERTMODE_ZIPPED);
    }

    srand((unsigned int)time(NULL));
    consoleInit(110, 128, 0);
    UpdateConsoleTitle();

    //set_log_vprintf(file_and_printf_log_vprintf);
    sockStart();
    packetStartup(0,0);

    {
        char *stemp = strrchr(argv[0], '.');
        const char *progname = getFileName(argv[0]);
        
        if (stemp)
        {
            *stemp = 0;
            if (strstriConst(progname,"villain")) // only one auctionhouse now
                printf("-----INVALID PROGRAM NAME: %s\n", argv[0]);
            else
                g_auctionstate.type = kAuctionServerType_Heroes;
        }
    }

    for(i=1;i<argc;i++)
    {
        if(0==stricmp(argv[i], "-type"))
        {
            if(strstri(argv[++i],"hero"))
                g_auctionstate.type = kAuctionServerType_Heroes;
            else
                printf("-----INVALID PARAMETER: %s\n", argv[i]);
        }
        else
        {
            printf("-----INVALID PARAMETER: %s\n", argv[i]);
        }
    }

    printf("auction server type: %s\n",AuctionServerType_ToStr(g_auctionstate.type));
    logSetDir("auctionserver_combined");
    errorLogStart();

    if(!g_auctionstate.server_name)
        g_auctionstate.server_name = AuctionServerType_ToStr(g_auctionstate.type);

    loadstart_printf("Networking startup...");
    timer = timerAlloc(); 
    timerStart(timer);
    packetStartup(0,0);
    loadend_printf("");
    
    loadstart_printf("loading config...");
    if(!auctionSvrCfgLoad(&g_auctionstate.cfg))
    {
        if(!isDevelopmentMode())
        {
            FatalErrorf("error loading auction config");
            exit(1);
        }
        printf("console: no config specified. in development mode. adding localhost\n");
        eaPush(&g_auctionstate.cfg.shardIps,"localhost");
    }
    loadend_printf("");

    // Special dummy shard entry for all fake auctions to belong to.
    AuctionDb_InitFakeShard();

    // init server
//     loadstart_printf("initializing server net(%s)..", g_auctionstate.cfg.server_name);
//     auctionSvrInitNet(&g_auctionstate);
//     loadend_printf("");
    
    // init misc. systems.
    aucsql_open();
    AuctionServer_InitData();

    // ensure only one auction at a time.
    {
        char *hn = (g_auctionstate.type == kAuctionServerType_Heroes)?"Global\\heroes_auctionserver_mutex":"Global\\villains_auctionserver_mutex";
        HANDLE mx;
        mx = CreateMutexA(0,0,hn);
        loadstart_printf("aquiring mutex %s...",hn);
        if(!mx)
            FatalErrorf("failed to allocate mutex %s",hn);
        else if(WAIT_TIMEOUT == WaitForSingleObject(mx,0))
        {
            FatalErrorf("ERROR: couldn't aquire mutex %s two auction servers of this type running at same time.",hn);
        }
        // CloseHandle(mx); - deliberately leak the handle, not released until exit.
        loadend_printf("done");
    }
    
//     if(*server_cfg.cfg.log_server)
//     {
//         loadstart_printf("Connecting to logserver (%s)...", db_state.log_server);
//         logNetInit();
//         loadend_printf("done");
//     }
//     else
//     {
        printf("no logserver specified. only printing logs locally.\n");
//     }

    printfColor(COLOR_RED|COLOR_GREEN|COLOR_BLUE | COLOR_BRIGHT, "AuctionServer ready.");
    printf(" (took %f seconds)\n\n",timerElapsed(startup_timer));

    g_frame_timer = timerAlloc();
    timerStart(g_frame_timer);

    send_timer = timerAlloc();
    timerStart(send_timer);

    g_flush_timer = timerAlloc();

    while(!g_auctionstate.quitting)
    {
        F32 frame_elapsed;

        g_packetcount = 0;
        timerStart(g_flush_timer);

        NMMonitor(1);
        shardNetConnectTick(&g_auctionstate);
        timerElapsedAndStart(g_frame_timer); // do this here for sensible throttling
        shardNetProcessTick(&g_auctionstate);

        DoConsoleDebugMenu(auctionserverdebug); 

        if (timerElapsed(timer) > (1.f / AUCTIONSERVER_TICK_FREQ))
        {
            timerStart(timer);
            g_tickCount++;
            UpdateConsoleTitle();
            auctionfulfiller_Tick();
            Xactserver_Tick();
        }

        frame_elapsed = timerElapsed(g_frame_timer);

        if(g_bVerbose)
        {
            if(g_bThrottled)
            {
                printf("Tick: %f secs, %d packets\n", frame_elapsed, g_packetcount);
            }
            else
            {
                printf("Tick: %f secs, %d packets                                 \r", frame_elapsed, g_packetcount);
            }
        }

        // If we're throttled, unthrottle if we're below our min time
        if(g_bThrottled && frame_elapsed < MIN_THROTTLE)
        {
            g_bThrottled = false;
            AuctionServer_SendThrottleCommand(AUCTION_SVR_THROTTLE_OFF);

            if(g_bVerbose)
                printf("Throttle: Off  (%f secs)\n", frame_elapsed);
        }

        if(timerElapsed(send_timer) > 30)
        {
            // Send the throttle state to make sure we stay synced up.
            AuctionServer_SendThrottleCommand(g_bThrottled ? AUCTION_SVR_THROTTLE_ON : AUCTION_SVR_THROTTLE_OFF);

            // send the periodic inventory update
            AuctionServer_HandleBatchStatus();
            timerStart(send_timer);
        }

#define MIN_STEPTIME (1.f/30.f)
        frame_elapsed = timerElapsed(g_frame_timer);
        if(frame_elapsed < MIN_STEPTIME)
        {
            int ms_sleep = ceil((MIN_STEPTIME - frame_elapsed)* 1000.f);
            Sleep(MIN(ms_sleep,34));
            if(g_bVerbose)
                printf("Sleeping for %d msec                    \r",MIN(ms_sleep,34));
        }
    }

    EXCEPTION_HANDLER_END;
}

void AuctionServer_EntInvToStr(char **hestr,AuctionEnt *ent)
{
    char *inv = NULL;
    int i;
    if(!ent || !hestr)
        return;
    for( i = 0; i < eaSize(&ent->inv.items); ++i )
    {
        int tmplen;
        char *tmp = NULL;
        char *lockid = NULL;
        AuctionInvItem *itm = ent->inv.items[i];
        if(!itm)
            continue;
        if(itm->lockid)
            estrConcatf(&lockid," (locked xaction:%i[%i])",itm->lockid,itm->subid);
        tmplen = estrLength(&inv);
        ParserWriteText(&inv,parse_AuctionInvItem,itm,0,0);
        tmp = strpbrk(inv+tmplen+2,"\r\n");
        estrInsert(&inv,tmp-inv,lockid,estrLength(&lockid));
        estrDestroy(&lockid);
    }
    (*hestr) = inv;
}
