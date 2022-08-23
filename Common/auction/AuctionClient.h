/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#ifndef AUCTIONCLIENT_H
#define AUCTIONCLIENT_H

#include "Auction.h"
#include "Xact.h"

typedef struct Entity Entity;
typedef enum TrayItemType TrayItemType;

#ifdef SERVER
typedef struct AuctionItem AuctionItem;

typedef struct AuctionMultiContext
{
    XactCmdType type;
    char *context;
} AuctionMultiContext;
#endif

void auctionRateLimiter(Entity *e);
int auctionBanned(Entity *e, int notify );
int canUseAuction( Entity *e, int notify );
void sendAuctionBannedUpdateToClient(Entity *e);
void AuctionClient_UpdateItemStatus(const char *pchIdentifier, int buying, int selling);
void ent_BatchSendAuctionInvItemInfoUpdate(Entity *e, int *forSaleList, int *forBuyList, int idOffset, int size);
void ent_ListSendAuctionInvItemInfoUpdate( Entity * e, int size, int * idlist, int *forSaleList, int *forBuyList );
void ent_BatchReqAuctionItemStatus(Entity *e, int start_idx,  int reqSize);
void ent_ListReqAuctionItemStatus(Entity *e, int * idlist);
void ent_ReqAuctionInv(Entity *e);
void ent_ReqAuctionHist(Entity *e,char * pchIdent);
void ent_AuctionSendInvSizeXact(Entity *e, int invSize);
void ent_AddAuctionInvItemXact(Entity *e, char *pchIdent, int index, int amt, int price, AuctionInvItemStatus auc_status, int id);
void ent_GetAuctionInvItemXact(Entity *e, int id);
void ent_ChangeAuctionInvItemStatusXact(Entity *e, int id, int price, AuctionInvItemStatus auc_status);
void ent_GetInfStoredXact(Entity *e, int id);
void ent_GetAmtStoredXact(Entity *e, int id);
void ent_XactReqShardJump(Entity *e, const char *shard, int map, const char *data);
Packet *ent_AuctionPktCreate(Entity *e, int cmd);
Packet *ent_AuctionPktCreateEx(Entity *e, int cmd);

#ifdef SERVER
void MapHandleXactCmd(Entity *e, XactUpdateType update_type, XactCmd *cmd);
void MapHandleXactFail(Entity *e, XactCmdType xact_type, char *context);
void AuctionPrepFakeItemList(AuctionMultiContext ***multictxs, AuctionItem *item, int amt, int price);
void AuctionSendFakeItemList(AuctionMultiContext ***multictx);
#endif


#endif //AUCTIONCLIENT_H
