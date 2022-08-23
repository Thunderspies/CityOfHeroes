/*********************************************************************
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
 *
 ***************************************************************************/
#include "XactServer.h"
#include "auctioncmds.h"
#include "auctionfulfill.h"
#include "auctionhistory.h"
#include "XactServerInternal.h"
#include <utilitieslib/utils/file.h>
#include "AsyncFileWriter.h"
#include "AuctionDb.h"
#include <utilitieslib/components/StringCache.h>
#include "auction/Xact.h"
#include "comm_backend.h"
#include "AuctionServer.h"
#include "auction/Auction.h"
#include "entity/character_inventory.h"
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/network/net_linklist.h>
#include <utilitieslib/network/net_link.h>
#include <utilitieslib/network/net_masterlist.h>
#include <utilitieslib/network/net_structdefs.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/network/net_linklist.h>
#include <utilitieslib/network/net_link.h>
#include <utilitieslib/network/net_masterlist.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/log.h>

typedef enum XactFulfillMode
{
    XactFulfillMode_TwoParty,        // Normal auction
    XactFulfillMode_AutoBuy,        // Only player seller
    XactFulfillMode_AutoSell,        // Only player buyer

    XactFulfillMode_Count
} XactFulfillMode;

XactServer g_XactServer;

typedef struct XactionDb
{
    Xaction **xacts;
    int max_xid;
} XactionDb;
XactionDb g_xactdb;

// in auctioncmds.c
void AuctionServer_HandleLoggedSlashCmd(Xaction *xact, AuctionEnt *ent, char *str, XactServerMode svr_mode);

static void s_XactServer_UpdateXaction(Xaction *xact);
static void XactServer_FinalizeCmd(XactCmd *cmd, XactUpdateType fin);

static int XactServer_PrepCmd(Xaction *xact, AuctionShard *shard, int ident, XactCmd *cmd)
{
    if(!xact||!cmd||!shard)
        return 0;
    cmd->xid = xact->xid;
    cmd->subid = xact->cmdCtr++;
    cmd->shard = shard;
    cmd->ident = ident;

    return 1;
}

static void XactServer_AddCmd(Xaction *xact, AuctionShard *shard, int ident, XactCmd *cmd)
{
    if(XactServer_PrepCmd(xact, shard, ident, cmd))
        eaPush(&xact->cmds,cmd);
}

//----------------------------------------
//  default behavior for command update: send it to the shard's link.
//----------------------------------------
static void XactServer_ClientSend(XactCmd *cmd, XactUpdateType type)
{
    Packet *pak;
    NetLink *link = AuctionShard_GetLink(SAFE_MEMBER(cmd,shard));
    assert(cmd);

    if(!link)
    {
        // @todo -AB: save packets to send to link when finally connect :11/20/06
        return;
    }
    
    pak = pktCreateEx(link,AUCTION_SVR_XACT_CMD);
    pktSendBitsAuto( pak, cmd->ident);
    pktSendBitsAuto( pak, type);
    XactCmd_Send(cmd,pak);
    pktSend(&pak, link);
}

static void XactServer_DispatchCmd(XactServerMode svr_mode, XactCmd *cmd, XactUpdateType type)
{
    switch ( svr_mode )
    {
    case XactServerMode_Normal:
        XactServer_ClientSend(cmd,type);
        break;
    case XactServerMode_LogPlayback:
        break; // do nothing
    break;
    default:
        printf("XactServer_DispatchCmd invalid switch value.\n");
        break;
    };    
}

// *********************************************************************************
// helpers
// *********************************************************************************

static XactCmd *Xaction_FindCmd(Xaction *xact, int subid)
{
    int i;
    if(xact)
    {
        for( i = eaSize(&xact->cmds) - 1; i >= 0; --i)
        {
            XactCmd *cmd = xact->cmds[i];
            if(cmd && cmd->subid == subid)
                return cmd;
        }
    }
    return NULL;
}

Xaction *XactServer_FindXaction(int xid)
{
    int i;
    for( i = eaSize(&g_xactdb.xacts) - 1; i >= 0; --i)
    {
        Xaction *xact = g_xactdb.xacts[i];
        if(xact && xact->xid == xid)
            return xact;
    }
    return NULL;
}

// *********************************************************************************
// internal and for XactLog, unlogged xactions. don't call these
// *********************************************************************************
typedef struct XactCmdFulfillAuction
{
    const char *pchIdentifier;
    char *buy_shard;
    int buy_ident;
    int buy_item;
    char *sell_shard;
    int sell_ident;
    int sell_item;
    int amt;
} XactCmdFulfillAuction;


static TokenizerParseInfo parse_XactCmdFulfillAuction[] =
{
    { "{",            TOK_START,                                            0},
    { "Item",        TOK_POOL_STRING|TOK_STRING(XactCmdFulfillAuction,pchIdentifier,0), 0},
    { "BuyShard",    TOK_STRING(XactCmdFulfillAuction,buy_shard,0),        0},
    { "BuyIdent",    TOK_INT(XactCmdFulfillAuction,buy_ident,0),            0},
    { "BuyItem",    TOK_INT(XactCmdFulfillAuction,buy_item,0),            0}, 
    { "SellShard",    TOK_STRING(XactCmdFulfillAuction,sell_shard,0),        0},
    { "SellIdent",    TOK_INT(XactCmdFulfillAuction,sell_ident,0),        0},
    { "SellItem",    TOK_INT(XactCmdFulfillAuction,sell_item,0),            0}, 
    { "Amount",        TOK_INT(XactCmdFulfillAuction,amt,0),                0},
    { "}",            TOK_END,                                            0},
    { "", 0, 0 },
};

static bool XactCmdFulfillAuction_ToStr(XactCmdFulfillAuction *fulfill, char **hestr)
{
    if(!fulfill || !hestr)
        return false;
    return ParserWriteTextEscaped(hestr,parse_XactCmdFulfillAuction,fulfill,0,0);
}

static bool XactCmdFulfillAuction_FromStr(XactCmdFulfillAuction *f, char *str)
{
    if(!f || !str)
        return false;
    return ParserReadTextEscaped(&str,parse_XactCmdFulfillAuction,f);
}

Xaction *XactServer_Start(int xid_override)
{
    Xaction *xact = XactServer_FindXaction(xid_override);
    if(!xact)
    {
        xact = Xaction_Create();
        eaPush(&g_xactdb.xacts,xact);
    }
    
    if(xid_override)
    {
        xact->xid = xid_override;
        g_xactdb.max_xid = MAX(g_xactdb.max_xid,xid_override);
    }
    else
    {
        xact->xid = ++g_xactdb.max_xid;
    }
    
    return xact;
}

static void s_HandleFulfillAuction(Xaction *xact, char *context, XactServerMode svr_mode, XactFulfillMode fulfill_mode)
{
    XactCmdFulfillAuction f = {0};
    int need_buyer = 1;
    int need_seller = 1;
    AuctionShard *buy_shard = NULL, *sell_shard = NULL;
    AuctionEnt *buyer = NULL, *seller = NULL;
    AuctionInvItem *buyer_itm = NULL;
    AuctionInvItem *seller_itm = NULL;
    XactCmd *cmd = NULL;
    char *cmd_context = NULL;
    int amt;
    int force_full_inventory_update = 0;
    
    if(!xact || !context)
        return;
    
    if(!XactCmdFulfillAuction_FromStr(&f,context))
        FatalErrorf("couldn't parse context %s",context);

    //auctionfulfiller_HandlingItem(f.pchIdentifier);

    STATIC_INFUNC_ASSERT(XactFulfillMode_Count == 3)

    // Auto-buy has no buyer, only a seller
    if(fulfill_mode == XactFulfillMode_AutoBuy)
        need_buyer = 0;
    // And auto-sell has no seller, only a buyer
    if(fulfill_mode == XactFulfillMode_AutoSell)
        need_seller = 0;

    // Shouldn't ever have a zero-party transaction!
    devassert(need_buyer || need_seller);

    if(need_buyer)
        buy_shard = AuctionDb_FindShard(f.buy_shard);
    if(need_seller)
        sell_shard = AuctionDb_FindShard(f.sell_shard);

    if((need_buyer && !buy_shard) || (need_seller && !sell_shard))
    {
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: NULL buy shard or sell shard",xact->xid);
        StructClear(parse_XactCmdFulfillAuction,&f);
        return;
    }

    if(need_buyer)
        buyer = AuctionDb_FindEnt(buy_shard,f.buy_ident);
    if(need_seller)
        seller = AuctionDb_FindEnt(sell_shard,f.sell_ident);

    if((need_buyer && !buyer) || (need_seller && !seller))
    {
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: NULL buyer or seller",xact->xid);
        StructClear(parse_XactCmdFulfillAuction,&f);
        return;
    }

    if(need_buyer)
        buyer_itm = AuctionEnt_GetItem(buyer,f.buy_item);
    if(need_seller)
        seller_itm = AuctionEnt_GetItem(seller,f.sell_item);

    if((need_buyer && !buyer_itm) || (need_seller && !seller_itm))
    {
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: fulfiller NULL buyer or seller item",xact->xid);
        StructClear(parse_XactCmdFulfillAuction,&f);
        return;
    }

    amt = f.amt;

    if(need_buyer)
    {
        if(buyer_itm->lockid)
        {
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: fulfiller buyer item is locked",xact->xid);
            StructClear(parse_XactCmdFulfillAuction,&f);
            return;
        }
        if(buyer_itm->auction_status != AuctionInvItemStatus_Bidding)
        {
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: fulfiller buyer is not bidding",xact->xid);
            StructClear(parse_XactCmdFulfillAuction,&f);
            return;
        }
        if(buyer_itm->amtOther < amt)
        {
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: fulfiller buyer does not want enough items",xact->xid);
            StructClear(parse_XactCmdFulfillAuction,&f);
            return;
        }
    }

    if(need_seller)
    {
        if(seller_itm->lockid)
        {
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: fulfiller seller item is locked",xact->xid);
            StructClear(parse_XactCmdFulfillAuction,&f);
            return;
        }
        if(seller_itm->auction_status != AuctionInvItemStatus_ForSale)
        {
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: fulfiller seller not selling item",xact->xid);
            StructClear(parse_XactCmdFulfillAuction,&f);
            return;
        }
        if(seller_itm->amtStored < amt)
        {
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: fulfiller seller does not have enough items",xact->xid);
            StructClear(parse_XactCmdFulfillAuction,&f);
            return;
        }
    }

    if(need_buyer)
        auctionfulfiller_RemoveInvItm(buyer_itm);

    if(need_seller)
        auctionfulfiller_RemoveInvItm(seller_itm);

    if(need_buyer)
    {
        buyer_itm->infStored -= buyer_itm->infPrice * amt;
        buyer_itm->amtStored += amt;
        buyer_itm->amtOther -= amt;

        if(!buyer_itm->amtOther)
            buyer_itm->auction_status = AuctionInvItemStatus_Bought;
        else
            auctionfulfiller_AddInvItm(buyer_itm);

        // SQL changes
        AuctionDb_EntInsertOrUpdate(buyer);
    }

    if(need_seller)
    {
        // Do the work! Fake agents are money sinks, so we don't credit them.
        // Auto-sold items get the price they asked, which we know is no more
        // than the auto-sell price floor.
        // JP!: added item splitting if infStored would be > AUCTION_MAX_INFLUENCE
        int inf_per_item = buyer_itm ? buyer_itm->infPrice : seller_itm->infPrice;
        int max_amt_add_to_current = (AUCTION_MAX_INFLUENCE - seller_itm->infStored) / inf_per_item;
        int amt_add_to_current = MIN(max_amt_add_to_current, amt);
        if(!AuctionEnt_IsFakeAgent(seller))
        {
            if (amt > max_amt_add_to_current)
            {
                int max_amt_extra_per_slot = AUCTION_MAX_INFLUENCE / inf_per_item;
                int amt_extra = amt - max_amt_add_to_current;
                int extra_slot_count = amt_extra / max_amt_extra_per_slot;
                int extra_slot_index;
                for (extra_slot_index = 0; extra_slot_index < extra_slot_count; ++extra_slot_index)
                {
                    AuctionInvItem *seller_itm_added;

                    AuctionInvItem *seller_itm_extra = NULL;
                    AuctionInvItem_Clone(&seller_itm_extra, seller_itm);
                    seller_itm_extra->auction_status = AuctionInvItemStatus_Sold;
                    seller_itm_extra->amtCancelled = 0;
                    seller_itm_extra->amtStored = 0;
                    if (extra_slot_index < extra_slot_count - 1)
                    {
                        seller_itm_extra->amtOther = max_amt_extra_per_slot;
                    }
                    else
                    {
                        seller_itm_extra->amtOther = amt_extra - (max_amt_extra_per_slot * (extra_slot_count - 1));
                    }
                    seller_itm_extra->infStored = seller_itm_extra->amtOther * inf_per_item;
                    seller_itm_extra->ent = NULL;
                    seller_itm_extra->lockid = 0;
                    seller_itm_extra->subid = 0;

                    seller_itm_added = AuctionEnt_AddItem(seller, seller_itm_extra);
                    assert(seller_itm_added);
                    AuctionInvItem_Destroy(seller_itm_extra);

                    // SQL changes
                    AuctionDb_EntInsertOrUpdate(seller);
                }

                force_full_inventory_update = 1;
            }

            seller_itm->infStored += amt_add_to_current * inf_per_item;
        }

        seller_itm->amtStored -= amt;
        seller_itm->amtOther += amt_add_to_current;

        if(!seller_itm->amtStored)
            seller_itm->auction_status = AuctionInvItemStatus_Sold;
        else
            auctionfulfiller_AddInvItm(seller_itm);

        // SQL changes
        AuctionDb_EntInsertOrUpdate(seller);
    }

    // Update history. We don't record auto-buys or auto-sells, only genuine
    // two party transactions.
    if(need_buyer && need_seller)
        auctionhistory_AddInv(buyer_itm, buyer_itm->ent->nameEnt, seller_itm->ent->nameEnt); 

    if(need_buyer)
    {
        // send info to the buyer/seller
        AuctionInvItem_ToStr(buyer_itm, &cmd_context);
        cmd = XactCmd_Create(need_seller ? XactCmdType_FulfillAuction : XactCmdType_FulfillAutoSell, cmd_context);
        cmd->res = kXactCmdRes_Yes;
        cmd->shard = buy_shard;
        cmd->ident = buyer->ident;
        XactServer_DispatchCmd(svr_mode, cmd, kXactUpdateType_Commit);
        XactCmd_Destroy(cmd);
    }

    if(need_seller)
    {
        if(AuctionEnt_IsFakeAgent(seller) && seller_itm->amtStored == 0)
        {
            eaFindAndRemove(&seller->inv.items,seller_itm);
            AuctionInvItem_Destroy(seller_itm);
            AuctionDb_EntInsertOrUpdate(seller);
        }
        else
        {
            AuctionInvItem_ToStr(seller_itm, &cmd_context);
            cmd = XactCmd_Create(need_buyer ? XactCmdType_FulfillAuction : XactCmdType_FulfillAutoBuy, cmd_context);
            cmd->res = kXactCmdRes_Yes;
            cmd->shard = sell_shard;
            cmd->ident = seller->ident;
            XactServer_DispatchCmd(svr_mode, cmd, kXactUpdateType_Commit);
            XactCmd_Destroy(cmd);
        }
        
        if (force_full_inventory_update)
            AuctionServer_SendInv(AuctionShard_GetLink(seller->shard),seller->shard,seller->ident);
    }

    estrDestroy(&cmd_context);
    
    // ----------
    // finally

    StructClear(parse_XactCmdFulfillAuction,&f);
}

static XactCmd* XactServer_CreateXactCmd(XactCmdType type, char *context, bool on_svr)
{
    XactCmd *cmd = XactCmd_Create(type,context);
    if( cmd )
    {
        if( on_svr )
        {
            cmd->on_svr = true;
            cmd->res = kXactCmdRes_Yes;
        }
        else
        {
            cmd->res = kXactCmdRes_Waiting;
        }
        
    }
    return cmd;
}

static void XactServer_LockInvItem(AuctionInvItem *itm, int lockID, int subID)
{
    assert(itm&&lockID&&subID);
    itm->lockid = lockID;
    itm->subid = subID;
    auctionfulfiller_RemoveInvItm(itm);
}

static void XactServer_UnlockInvItem(AuctionInvItem *itm)
{
    itm->lockid = 0;
    auctionfulfiller_AddInvItm(itm);
}

static bool s_HandleReq_GetEnt(AuctionEnt **ppent, Xaction *xact, AuctionShard *shard, int ident, char *name)
{
    (*ppent) = AuctionDb_FindAddEnt(shard, ident, name);
    if(devassert(*ppent))
        return true;
    else
    {
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: couldn't find/add ent %s:%s(%i)",
            xact->xid,(shard ? shard->name : "no shard"),name,ident);
        return false;
    }
}

static bool s_HandleReq_GetItemReq(AuctionInvItem **pitem, Xaction *xact, char *context)
{
    if(devassert(AuctionInvItem_FromStr(pitem,context)))
        return true;
    else
    {
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: failed to get context: %s",xact->xid,context);
        return false;
    }
}

static bool s_HandleReq_GetItm(AuctionInvItem **pitem, Xaction *xact, AuctionEnt *ent, AuctionInvItem *itemReq)
{
    AuctionInvItem *itm = AuctionEnt_GetItemEvenFromFake(ent,itemReq);

    if(!itm)
    {
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0,"%i: failed to find item %s(%i)[%i]: %s",
            xact->xid,ent->nameEnt,ent->ident,itemReq->id,itemReq->pchIdentifier);
        return false;
    }

// this shouldn't be necessary anymore
//     if(!(itm->type == xi->type && itm->amt == xi->amt && itm->name && xi->name && !strcmp(itm->name,xi->name)))
//     {
//         LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: item does not match %s(%i)[%i]: %i,%s,%i vs %i,%s,%i",
//             xact->xid,ent->nameEnt,ent->ident,xi->index,itm->type,(itm->name?itm->name:"[NULL]"),itm->amt,xi->type,(xi->name?xi->name:"[NULL]"),xi->amt);
//         return false;
//     }
// 
    if(itm->lockid)
    {
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: item has lockid (%i) %s(%i)[%i]: %s",
            xact->xid,itm->lockid,ent->nameEnt,ent->ident,itm->id,itm->pchIdentifier);
        return false;
    }

    (*pitem) = itm;
    return true;
}

static void XactServer_HandleReqInternal(Xaction *xact, XactCmdType xact_type, char *context, AuctionShard *shard, int ident,char *name, XactServerMode svr_mode)
{
    AuctionInvItem *itemReq = NULL; // Declared out here so I can make just one destructor call

    switch ( xact_type )
    {
    case XactCmdType_Test:
    {
    }
    xcase XactCmdType_DropAucEnt:
    {
        AuctionEnt *ent = AuctionDb_FindEnt(shard,ident);
        if(ent)
        {
            int i;
            bool drop_ok = true;

            for(i = eaSize(&ent->inv.items)-1; i >= 0; --i)
            {
                AuctionInvItem *itm = ent->inv.items[i];
                if(itm && itm->lockid)
                    drop_ok = false;
            }

            if(devassert(drop_ok))
            {
                auctionfulfiller_RemoveInv(&ent->inv);
                ent->deleted = true;
                AuctionDb_DropEnt(ent);
            }
            else
            {
                LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: ignoring DropAucEnt attempt on (%s[%i] %s) whose inventory has locked items",
                    xact->xid, (shard?shard->name:"[null shard]"), ident, ent->nameEnt);
            }
        }
    }
    xcase XactCmdType_SetInvSize:
    {
        AuctionEnt *ent;
        int size = atoi(context);

        if(!s_HandleReq_GetEnt(&ent,xact,shard,ident,name))
            break;

        AuctionEnt_SetInvSize(ent,size);
    }
    xcase XactCmdType_AddInvItem:
    {
        AuctionEnt *ent;
        XactCmd *cmd;
        AuctionInvItem *itm;
        char *context_out = NULL;

        if(!s_HandleReq_GetEnt(&ent,xact,shard,ident,name))
            break;
        if(!s_HandleReq_GetItemReq(&itemReq,xact,context))
            break;

        // add the item to auction side
        itm = AuctionServer_AddInvItm(ent,itemReq);
        if(!itm)
        {
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: failed to create item %s(%i):%s,%i,%i,%i,%i",xact->xid,name,ident,itemReq->pchIdentifier,itemReq->auction_status,itemReq->amtStored,itemReq->amtOther,itemReq->infPrice);
            // send to the mapserver that it failed
            cmd = XactCmd_Create(XactCmdType_AddInvItem,context);        
            cmd->shard = ent->shard;
            cmd->xid = xact->xid;
            cmd->res = kXactCmdRes_No;
            XactServer_AddCmd(xact,shard,ident,cmd);
            XactServer_DispatchCmd(svr_mode,cmd,kXactUpdateType_Abort);
            break;
        }

        itemReq->id = itm->id; // update the id so the mapserver know what we're talking about, but keep iMapSide etc.
        AuctionInvItem_ToStr(itemReq,&context_out);

        if(AuctionEnt_IsFakeAgent(ent))
        {
            // Fake agents don't have a MapServer to sync up with, so we
            // can auto-commit the transaction.
            cmd = XactServer_CreateXactCmd(XactCmdType_AddInvItem,context_out,true);
            XactServer_PrepCmd(xact,shard,ident,cmd);
            XactServer_LockInvItem(itm, cmd->xid, cmd->subid);
            cmd->res = kXactCmdRes_Yes;
            XactServer_FinalizeCmd(cmd, kXactUpdateType_Commit);
        }
        else
        {
            // auction side: track the item
            cmd = XactServer_CreateXactCmd(XactCmdType_AddInvItem,context_out,true);
            XactServer_AddCmd(xact,shard,ident,cmd);
            XactServer_LockInvItem(itm, cmd->xid, cmd->subid);
            cmd->res = kXactCmdRes_Yes;

            // mapserver side: wait for confirmation.
            cmd = XactServer_CreateXactCmd(XactCmdType_AddInvItem,context_out,false);
            XactServer_AddCmd(xact,shard,ident,cmd);
            XactServer_DispatchCmd(svr_mode,cmd,kXactUpdateType_Run);
        }

        estrDestroy(&context_out);
    }
    xcase XactCmdType_GetInvItem:
    {
        AuctionEnt *ent;
        AuctionInvItem *itm;
        XactCmd *cmd;
        char *context_out = NULL;

        if(!s_HandleReq_GetEnt(&ent,xact,shard,ident,name))
            break;
        if(!s_HandleReq_GetItemReq(&itemReq,xact,context))
            break;
        if(!s_HandleReq_GetItm(&itm,xact,ent,itemReq))
            break;

        AuctionInvItem_ToStr(itm,&context_out);

        // auction server side
        cmd = XactServer_CreateXactCmd(XactCmdType_GetInvItem,context_out,true);
        XactServer_AddCmd(xact,shard,ident,cmd);
        XactServer_LockInvItem(itm, cmd->xid, cmd->subid);

        // handle map server side
        cmd = XactServer_CreateXactCmd(XactCmdType_GetInvItem,context_out,false);
        XactServer_AddCmd(xact,shard,ident,cmd);
        XactServer_DispatchCmd(svr_mode,cmd,kXactUpdateType_Run);

        estrDestroy(&context_out);
    }
    xcase XactCmdType_ChangeInvStatus:
    {
        AuctionEnt *ent;
        AuctionInvItem *itm;
        XactCmd *cmd;
        char *context_revert = NULL;

        if(!s_HandleReq_GetEnt(&ent,xact,shard,ident,name))
            break;
        if(!s_HandleReq_GetItemReq(&itemReq,xact,context))
            break;
        if(!s_HandleReq_GetItm(&itm,xact,ent,itemReq))
            break;

#define XACTSERVER_VALID_CHANGEINVSTATUS() (itm->auction_status == AuctionInvItemStatus_Stored && itemReq->auction_status == AuctionInvItemStatus_ForSale)
        if(!XACTSERVER_VALID_CHANGEINVSTATUS())
        {
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: invalid ChangeInvStatus (%i to %i) on %s(%i)[%i]: %s,%i",
                xact->xid,itm->auction_status,itemReq->auction_status,ent->nameEnt,ent->ident,itm->id,itm->pchIdentifier,itemReq->infPrice);
            break;
        }

        // save revert info
        if(!AuctionInvItem_ToStr(itm,&context_revert))
        {
            // FIXME: FatalError?
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: could not create revert context for ChangeInvStatus on %s(%i)[%i]: %s",
                xact->xid,ent->nameEnt,ent->ident,itm->id,itm->pchIdentifier);
            break;
        }

        // change status
        if(!AuctionShard_ChangeInvItemStatus(itm,itemReq->infPrice,itemReq->auction_status))
        {
            // FIXME: FatalError?
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: ChangeInvStatus failed on %s(%i)[%i]: %s: %i,%i to %i,%i",
                xact->xid,ent->nameEnt,ent->ident,itm->id,itm->pchIdentifier,
                itm->infPrice,itm->auction_status,itemReq->infPrice,itemReq->auction_status);

            estrDestroy(&context_revert);
            break;
        }

        if(AuctionEnt_IsFakeAgent(ent))
        {
            // Fake agents don't have a MapServer to sync up with, so we
            // can auto-commit the transaction.
            cmd = XactServer_CreateXactCmd(XactCmdType_ChangeInvStatus,context_revert,true);
            XactServer_PrepCmd(xact,shard,ident,cmd);
            XactServer_LockInvItem(itm, cmd->xid, cmd->subid);

            // Nobody can abort this.
            cmd->res = kXactCmdRes_Yes;
            XactServer_FinalizeCmd(cmd, kXactUpdateType_Commit);
        }
        else
        {
            // auction server side
            cmd = XactServer_CreateXactCmd(XactCmdType_ChangeInvStatus,context_revert,true);
            XactServer_AddCmd(xact,shard,ident,cmd);
            XactServer_LockInvItem(itm, cmd->xid, cmd->subid);

            // map side
            cmd = XactServer_CreateXactCmd(XactCmdType_ChangeInvStatus,context,false);
            XactServer_AddCmd(xact,shard,ident,cmd);
            XactServer_DispatchCmd(svr_mode,cmd,kXactUpdateType_Run);
        }

        estrDestroy(&context_revert);
    }
    xcase XactCmdType_ChangeInvAmount:
    {
        AuctionEnt *ent;
        AuctionInvItem *itm;
        XactCmd *cmd;

        if(!s_HandleReq_GetEnt(&ent,xact,shard,ident,name))
            break;
        if(!s_HandleReq_GetItemReq(&itemReq,xact,context))
            break;
        if(!s_HandleReq_GetItm(&itm,xact,ent,itemReq))
            break;

#define XACTSERVER_VALID_CHANGEINVAMOUNT() (itm->auction_status == AuctionInvItemStatus_Stored || itm->auction_status == AuctionInvItemStatus_Bidding)
        if(!XACTSERVER_VALID_CHANGEINVAMOUNT())
        {
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: invalid ChangeInvAmount (%i by %i and %i by %i) on %s(%i)[%i]: %s,%i",
                xact->xid,itm->amtOther,itemReq->amtOther,
                itm->amtStored,itemReq->amtStored,
                // TODO: add infStored
                ent->nameEnt,ent->ident,itm->id,itm->pchIdentifier,itemReq->infPrice);
            break;
        }

        // change amount
        if(!AuctionShard_ChangeInvItemAmount(itm,itemReq->amtStored,itemReq->amtOther, itemReq->infStored))
        {
            // FIXME: FatalError?
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: ChangeInvAmount failed on %s(%i)[%i]: %s: %i,%i to %i,%i",
                xact->xid,ent->nameEnt,ent->ident,itm->id,itm->pchIdentifier,
                // TODO: add infStored
                itm->amtStored,itm->amtOther,itemReq->amtStored,itemReq->amtOther);
            break;
        }

        if(AuctionEnt_IsFakeAgent(ent))
        {
            // Fake agents don't have a MapServer to sync up with, so we
            // can auto-commit the transaction.
            cmd = XactServer_CreateXactCmd(XactCmdType_ChangeInvAmount,context,true);
            XactServer_PrepCmd(xact,shard,ident,cmd);
            XactServer_LockInvItem(itm, cmd->xid, cmd->subid);

            // Nobody can abort this.
            cmd->res = kXactCmdRes_Yes;
            XactServer_FinalizeCmd(cmd, kXactUpdateType_Commit);
        }
        else
        {
            // auction server side
            cmd = XactServer_CreateXactCmd(XactCmdType_ChangeInvAmount,context,true);
            XactServer_AddCmd(xact,shard,ident,cmd);
            XactServer_LockInvItem(itm, cmd->xid, cmd->subid);

            // map side
            cmd = XactServer_CreateXactCmd(XactCmdType_ChangeInvAmount,context,false);
            XactServer_AddCmd(xact,shard,ident,cmd);
            XactServer_DispatchCmd(svr_mode,cmd,kXactUpdateType_Run);
        }
    }
    xcase XactCmdType_GetInfStored:
    {
        AuctionEnt *ent;
        AuctionInvItem *itm;
        XactCmd *cmd;
        char *context_out = NULL;

        if(!s_HandleReq_GetEnt(&ent,xact,shard,ident,name))
            break;
        if(!s_HandleReq_GetItemReq(&itemReq,xact,context))
            break;
        if(!s_HandleReq_GetItm(&itm,xact,ent,itemReq))
            break;

        if ( !itm->amtCancelled && !itm->infStored )
        {
            // TODO: log?
            break;
        }

        AuctionInvItem_ToStr(itm,&context_out);

        // auction server side
        cmd = XactServer_CreateXactCmd(XactCmdType_GetInfStored,context_out,true);
        XactServer_AddCmd(xact,shard,ident,cmd);
        XactServer_LockInvItem(itm, cmd->xid, cmd->subid);

        // map side
        cmd = XactServer_CreateXactCmd(XactCmdType_GetInfStored,context_out,false);
        XactServer_AddCmd(xact,shard,ident,cmd);
        XactServer_DispatchCmd(svr_mode,cmd,kXactUpdateType_Run);

        estrDestroy(&context_out);
    }
    xcase XactCmdType_GetAmtStored:
    {
        AuctionEnt *ent;
        AuctionInvItem *itm;
        XactCmd *cmd;

        if(!s_HandleReq_GetEnt(&ent,xact,shard,ident,name))
            break;
        if(!s_HandleReq_GetItemReq(&itemReq,xact,context))
            break;
        if(!s_HandleReq_GetItm(&itm,xact,ent,itemReq))
            break;

        if(itemReq->amtStored <= 0 || itm->amtStored - itemReq->amtStored < 0)
        {
            LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "%i: invalid GetAmtStored (get %i from %i) on %s(%i)[%i]: %s",
                xact->xid,itemReq->amtStored,itm->amtStored,
                ent->nameEnt,ent->ident,itm->id,itm->pchIdentifier);
            break;
        }

        // auction server side
        cmd = XactServer_CreateXactCmd(XactCmdType_GetAmtStored,context,true);
        XactServer_AddCmd(xact,shard,ident,cmd);
        XactServer_LockInvItem(itm, cmd->xid, cmd->subid);

        // map side
        cmd = XactServer_CreateXactCmd(XactCmdType_GetAmtStored,context,false);
        XactServer_AddCmd(xact,shard,ident,cmd);
        XactServer_DispatchCmd(svr_mode,cmd,kXactUpdateType_Run);
    }
    xcase XactCmdType_FulfillAuction: 
        s_HandleFulfillAuction(xact,context,svr_mode, XactFulfillMode_TwoParty);
    xcase XactCmdType_FulfillAutoBuy:
        s_HandleFulfillAuction(xact,context,svr_mode, XactFulfillMode_AutoBuy);
    xcase XactCmdType_FulfillAutoSell:
        s_HandleFulfillAuction(xact,context,svr_mode, XactFulfillMode_AutoSell);
    xcase XactCmdType_ShardJump:
//     xcase XactCmdType_SlashCmd:
//          AuctionServer_HandleLoggedSlashCmd(xact,AuctionDb_FindAddEnt(shard,ident,name),context,svr_mode);
    xdefault:
         printf("XactServer_HandleReqInternal invalid switch value %i.", xact_type);
    };
    STATIC_INFUNC_ASSERT(XactCmdType_Count == 15); // make sure to update this with new vals

    AuctionInvItem_Destroy(itemReq);
}

//static void XactServer_HandleUpdateInternal(XactCmd *cmdIn, XactServerMode svr_mode)
void XactServer_AddUpdate(int xid, int subid, XactCmdRes res)
{
    Xaction *xact;
    XactCmd *cmd = NULL;

    xact = XactServer_FindXaction(xid);
    if(!xact)
    {
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "Errors AddUpdate: unknown Xaction: xid %i (max xid %i), subid %i, res %i",
                    xid, g_xactdb.max_xid, subid, res);
        return;
    }

    cmd = Xaction_FindCmd(xact,subid);
    if(!cmd)
    {
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "Errors AddUpdate: unknown XactCmd: xid %i (max xid %i), subid %i (ctr %i), res %i",
                    xid, g_xactdb.max_xid, subid, xact->cmdCtr, res);
        return;
    }

    if(!INRANGE0(res,kXactCmdRes_Count))
    {
        LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "Errors AddUpdate: bad XactCmdRes: xid %i (max xid %i), subid %i (ctr %i), res %i",
                    xid, g_xactdb.max_xid, subid, xact->cmdCtr, res);
        return;
    }

    // update
    cmd->res = res;
    s_XactServer_UpdateXaction(xact);
}

// *********************************************************************************
// logged xactions
// *********************************************************************************

void XactServer_AddReq(XactCmdType xact_type, char const *context, AuctionShard *shard, int ident,char *name) 
{
    char *context_buf;
    Xaction *xact = XactServer_Start(0);
    strdup_alloca(context_buf,context); // TODO: make context handling sane
    XactServer_HandleReqInternal(xact,xact_type,context_buf,shard,ident,name,XactServerMode_Normal); 
}

// *********************************************************************************
// internal util methods
// *********************************************************************************

static AuctionInvItem* s_Finalize_GetItm(XactCmd *cmd, AuctionEnt **hent)
{
    int i;
    AuctionEnt *ent = AuctionDb_FindEnt(cmd->shard,cmd->ident);

    if(hent)
        *hent = ent;

    if(!ent)
        FatalErrorf("s_Finalize_GetItm: Could not find ent (%s/%d) to finalize Xaction!", cmd->shard->name, cmd->ident);

    for(i = eaSize(&ent->inv.items)-1; i >= 0; --i)
    {
        AuctionInvItem *itm = ent->inv.items[i];

        if(itm && itm->lockid == cmd->xid && itm->subid == cmd->subid)
            return itm;
    }

    FatalErrorf("Could not find item to finalize Xaction!");
    return NULL; // if you want to make this non fatal, note that this case isn't actually handled
}


static void FinalizeItem(AuctionInvItem *itm)
{
    if( itm->infStored == 0 && itm->amtStored == 0 )
    {
        if(itm->ent)
        {
            eaFindAndRemove(&itm->ent->inv.items,itm);
            AuctionDb_EntInsertOrUpdate(itm->ent);
        }

        AuctionInvItem_Destroy(itm);
    }
    else
    {
        auctionfulfiller_AddInvItm(itm);

        if(itm->ent)
            AuctionDb_EntInsertOrUpdate(itm->ent);
    }

}

static void XactServer_FinalizeCmd(XactCmd *cmd,XactUpdateType fin)
{
    if(!cmd) 
        return;

    STATIC_INFUNC_ASSERT(XactCmdType_Count == 15); // make sure to add new xactions here too

    if(cmd->on_svr)
    {
        AuctionEnt *ent = NULL;
        AuctionInvItem *itm = NULL;

        switch ( cmd->type )
        {
        xcase XactCmdType_AddInvItem:
        {
            itm = s_Finalize_GetItm(cmd,&ent);
            if(fin == kXactUpdateType_Commit)
            {
                XactServer_UnlockInvItem(itm);
                AuctionDb_EntInsertOrUpdate(ent);
            }
            else
            {
                AuctionServer_RemInvItm(itm);
            }
        }
        xcase XactCmdType_GetInvItem:
        {
            itm = s_Finalize_GetItm(cmd,&ent);
            if (fin == kXactUpdateType_Commit)
            {
                AuctionServer_RemInvItm(itm);
                AuctionDb_EntInsertOrUpdate(ent);
            }
            else
            {
                XactServer_UnlockInvItem(itm);
            }
        }
        xcase XactCmdType_ChangeInvStatus:
        {
            itm = s_Finalize_GetItm(cmd,&ent);
            XactServer_UnlockInvItem(itm);

            if(fin == kXactUpdateType_Commit)
            {
                AuctionDb_EntInsertOrUpdate(ent);
            }
            else 
            {
                AuctionInvItem *itemRevert = NULL;
                if(!AuctionInvItem_FromStr(&itemRevert,cmd->context))
                    FatalErrorf("Could not convert revert cmd to finalize Xaction!\n%s",cmd->context);

                itm = AuctionShard_ChangeInvItemStatus(itm,itemRevert->infPrice,itemRevert->auction_status);

                AuctionInvItem_Destroy(itemRevert);
            }
        }
        xcase XactCmdType_ChangeInvAmount:
        {
            itm = s_Finalize_GetItm(cmd,&ent);
            XactServer_UnlockInvItem(itm);

            if(fin == kXactUpdateType_Commit)
            {
                AuctionDb_EntInsertOrUpdate(ent);
            }
            else
            {
                AuctionInvItem *itemRevert = NULL;
                int infRevert = 0;
                if(!AuctionInvItem_FromStr(&itemRevert,cmd->context))
                    FatalErrorf("Could not convert revert cmd to finalize Xaction!\n%s",cmd->context);

                // Fake agents never hold money because there's no-one to
                // claim it from them.
                if(ent && AuctionEnt_IsFakeAgent(ent))
                    infRevert = 0;
                else
                    infRevert = -itemRevert->infStored;

                itm = AuctionShard_ChangeInvItemAmount(itm,-itemRevert->amtStored,-itemRevert->amtOther, infRevert);
                if(!itm)
                    FatalErrorf("Could not revert item to finalize Xaction!");

                AuctionInvItem_Destroy(itemRevert);
            }
        }
        xcase XactCmdType_GetInfStored:
        {
            itm = s_Finalize_GetItm(cmd,&ent);
            XactServer_UnlockInvItem(itm);

            if (fin == kXactUpdateType_Commit)
            {
                AuctionShard_SetInfStored(itm, 0);
                FinalizeItem(itm); // does the logging
            }
        }
        xcase XactCmdType_GetAmtStored:
        {
            itm = s_Finalize_GetItm(cmd,&ent);
            XactServer_UnlockInvItem(itm);

            if (fin == kXactUpdateType_Commit)
            {
                AuctionInvItem *itemChange = NULL;

                if ( !AuctionInvItem_FromStr(&itemChange,cmd->context) )
                    FatalErrorf("Could not get the new item in finalize Xaction!\n%s",cmd->context);

                AuctionShard_SetAmtStored(itm, itm->amtStored - itemChange->amtStored);
                FinalizeItem(itm); // does the logging
            }
        }
        xdefault:
            printf("XactServer_FinalizeCmd invalid switch value %i.\n",cmd->type);
        };

    }
    else
    {
        // prep to send to client
        switch ( cmd->type )
        {
        case XactCmdType_AddInvItem:
        case XactCmdType_GetInvItem:
        case XactCmdType_ChangeInvStatus:
        case XactCmdType_ChangeInvAmount:
        case XactCmdType_GetInfStored:
        case XactCmdType_GetAmtStored:
        {
            AuctionInvItem *itmCmd = NULL;
            AuctionInvItem *itmDb = NULL;
            char *context = NULL;
            AuctionEnt *ent = AuctionDb_FindEnt(cmd->shard,cmd->ident);

            if(!ent)
                FatalErrorf("XactServer_FinalizeCmd: Could not find ent (%s/%d) to finalize Xaction!", cmd->shard->name, cmd->ident);

            if(!AuctionInvItem_FromStr(&itmCmd,cmd->context))
                FatalErrorf("Could not parse context during finalize Xaction!\n%s",cmd->context);

            // find the item that this context refers to
            itmDb = AuctionEnt_GetItem(ent,itmCmd->id);

            if(itmDb)
            {
                // the item exists, update the context to reflect the current state
                // (this should always occur after the server-side finalization)
                AuctionInvItem_ToStr(itmDb,&context);
            }
            else
            {
                // the item couldn't be found, tell the mapserver to delete it's local copy
                itmCmd->bDeleteMe = true;
                AuctionInvItem_ToStr(itmCmd,&context);
            }

            XactCmd_SetContext(cmd,context);

            estrDestroy(&context);
            AuctionInvItem_Destroy(itmCmd);
        }
        xdefault:
            break;
        };

        XactServer_ClientSend(cmd,fin);
    }
}

static void s_XactServer_FinalizeXaction(Xaction *xact, XactUpdateType fin)
{
    int i;
    
    if(!xact || xact->completed)
        return;

    for(i = 0; i < eaSize(&xact->cmds); ++i) // handle these in the order they were added
    {
        XactCmd *cmd = xact->cmds[i];
        if(cmd)
            XactServer_FinalizeCmd(cmd,fin);
    }
    xact->completed = true;
}

static void s_XactServer_UpdateXaction(Xaction *xact)
{
    int i;
    XactCmdRes res = kXactCmdRes_Yes;
    bool bTimedOut = false;

    if(!xact || xact->completed)
        return;

    if(timerSecondsSince2000() - xact->timeCreated >= XACTION_TIMEOUT)
        bTimedOut = true;

    for(i = eaSize(&xact->cmds) - 1; i >= 0; --i)
    {
        XactCmd *cmd = xact->cmds[i];
        if(cmd && cmd->res!=kXactCmdRes_Yes)
        {
            // if any don't say yes, we aren't done yet or we abort
            if(cmd->res==kXactCmdRes_No)
                res = kXactCmdRes_No;
            else if(bTimedOut)
            {
                XactServer_AddUpdate(cmd->xid,cmd->subid,kXactCmdRes_No);
                LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "errors: timing out xid %i, subid %i",cmd->xid,cmd->subid);
                res = kXactCmdRes_No;
            }
            else if(res != kXactCmdRes_No) // if we already have a no, keep it
                res = kXactCmdRes_Waiting;
            break;
        }
    }

    if(res == kXactCmdRes_Yes)
        s_XactServer_FinalizeXaction(xact,kXactUpdateType_Commit);
    else if(res == kXactCmdRes_No )
        s_XactServer_FinalizeXaction(xact,kXactUpdateType_Abort);
}

void Xactserver_Tick()
{
    int i;
    int n = eaSize(&g_xactdb.xacts);
    for( i = 0; i < n; ++i)
    {
        Xaction *xact = g_xactdb.xacts[i];
        if(!xact)
            continue;
        
        s_XactServer_UpdateXaction(xact);

        if(xact->completed)
        {
            eaRemove(&g_xactdb.xacts,i);
            Xaction_Destroy(xact);
            i--;n--;
        }
    }
}

static void s_FulfillSetBuyer(XactCmdFulfillAuction *ctxptr, AuctionInvItem *buy_itm)
{
    if(ctxptr && buy_itm)
    {
        ctxptr->buy_shard = buy_itm->ent->shard->name;
        ctxptr->buy_ident = buy_itm->ent->ident;
        ctxptr->buy_item = buy_itm->id;
    }
}

static void s_FulfillSetSeller(XactCmdFulfillAuction *ctxptr, AuctionInvItem *sell_itm)
{
    if(ctxptr && sell_itm)
    {
        ctxptr->sell_shard = sell_itm->ent->shard->name;
        ctxptr->sell_ident = sell_itm->ent->ident;
        ctxptr->sell_item = sell_itm->id;
    }
}

static char *s_FulfillFinish(XactCmdFulfillAuction *ctxptr, const char *pchIdentifier, int amt)
{
    char *res = NULL;

    if(ctxptr && pchIdentifier)
    {
        ctxptr->pchIdentifier = pchIdentifier;
        ctxptr->amt = amt;

        if(!XactCmdFulfillAuction_ToStr(ctxptr,&res))
            FatalErrorf("unable to create context for fulfillment");
    }

    return res;
}

// These are separate functions because they return NULL if you don't provide
// everything needed for the type of context. If they were combined and the
// buyer/seller made optional, the mismatch wouldn't show up until later in the
// transaction process.
char *XactServer_CreateFulfillContext(const char *pchIdentifier, AuctionInvItem *sell_itm, AuctionInvItem *buy_itm, int amt)
{
    char *res = NULL;
    XactCmdFulfillAuction f = {0};

    if(devassert(sell_itm && buy_itm && amt))
    {
        s_FulfillSetBuyer(&f, buy_itm);
        s_FulfillSetSeller(&f, sell_itm);
        res = s_FulfillFinish(&f, pchIdentifier, amt);
    }

    return res;
}

char *XactServer_CreateFulfillAutoBuyContext(const char *pchIdentifier, AuctionInvItem *sell_itm, int amt)
{
    char *res = NULL;
    XactCmdFulfillAuction f = {0};

    if(devassert(sell_itm && amt))
    {
        s_FulfillSetSeller(&f, sell_itm);
        res = s_FulfillFinish(&f, pchIdentifier, amt);
    }

    return res;
}

char *XactServer_CreateFulfillAutoSellContext(const char *pchIdentifier, AuctionInvItem *buy_itm, int amt)
{
    char *res = NULL;
    XactCmdFulfillAuction f = {0};

    if(devassert(buy_itm && amt))
    {
        s_FulfillSetBuyer(&f, buy_itm);
        res = s_FulfillFinish(&f, pchIdentifier, amt);
    }

    return res;
}
