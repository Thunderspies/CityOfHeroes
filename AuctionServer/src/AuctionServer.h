/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#ifndef AUCTIONSERVER_H
#define AUCTIONSERVER_H

#include "auction/Auction.h"
#include "auction/Xact.h"

#define AUC_NUM_TYPES 10000 // rough estimate of the actual

C_DECLARATIONS_BEGIN

typedef struct NetLink NetLink;

typedef struct AuctionServerCfg
{
    char **shardIps;
    int iAutoSellPrice;                // Maximum we'll pay to selling players
    int iAutoBuyPrice;                // Minimum we'll take from buying players
    U32 iAutoFulfillTime;

    char sqlLogin[1024];
    char sqlDbName[1024];
} AuctionServerCfg;

typedef struct AuctionServerState
{
    AuctionServerCfg cfg;
    const char *server_name;
    AuctionServerType type;

    int quitting;

    NetLink *shard_links;
    U32 failed_shards;
    int next_retry_shard;
    U32 last_failed_time;
} AuctionServerState;
 
extern char g_exe_name[];
extern AuctionServerState g_auctionstate;

void AuctionServer_SendInv(NetLink *link, AuctionShard *shard, int ident);
AuctionInvItem *AuctionServer_AddInvItm(AuctionEnt *ent, AuctionInvItem *item);
AuctionInvItem *AuctionServer_FindInvItm(AuctionEnt *ent, int idx);
void AuctionServer_RemInvItm(AuctionInvItem *itm);
AuctionInvItem *AuctionShard_ChangeInvItemStatus(AuctionInvItem *itm, int price, AuctionInvItemStatus status);
void AuctionServer_EntInvToStr(char **hestr,AuctionEnt *ent);
AuctionInvItem *AuctionShard_ChangeInvItemAmount(AuctionInvItem *itm, int amtStored, int amtOther, int infStored);
int AuctionShard_SetInfStored(AuctionInvItem *itm, int newInfStored);
int AuctionShard_SetAmtStored(AuctionInvItem *itm, int newAmtStored);

C_DECLARATIONS_END

#endif //AUCTIONSERVER_H
