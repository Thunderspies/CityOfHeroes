/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#ifndef AUCTIONDB_H
#define AUCTIONDB_H

#include "auction/Auction.h"

typedef struct NetLink NetLink;
typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct ParseTable ParseTable;

typedef struct AuctionShard
{
    char *name;

    // not persisted
    NetLink **links;
    int link_rotor;
    bool data_ok;        // prevent cascading large data packets (currently just batch info)
    U32 last_received;    // deadman's switch

    StashTable entById;
    AuctionEnt **ents;
} AuctionShard;

NetLink* AuctionShard_GetLink(AuctionShard *shard); // gets the next link, round robin. removes dead links.

typedef struct AuctionEnt
{
    AuctionInventory inv;
    int ident;
    char *nameEnt;
    int accesslevel;
    AuctionShard *shard;
    int iLastId;            // this needs to be persisted even if inv is empty, otherwise merges will make inv items' ids inconsistent
    bool deleted;            // used to indicate deleted ents in the journal
} AuctionEnt;

void AuctionEnt_ToStr(AuctionEnt *ent, char **hestr);
bool AuctionEnt_FromStr(AuctionEnt **hent, char *str);
AuctionInvItem* AuctionEnt_GetItem(AuctionEnt *ent, int id);
AuctionInvItem* AuctionEnt_GetItemEvenFromFake(AuctionEnt *ent, AuctionInvItem *itemReq);
bool AuctionEnt_IsFakeAgent(AuctionEnt *ent);

typedef struct AuctionDb
{
    AuctionShard **shards;
    AuctionShard **shardsPlusFake;

    AuctionEnt **ents;

    // used to check consistency
    U32 uShardsVer; // CRC of ParseTable
    U32 uEntsVer;

    // not persisted/parsed
    StashTable shardByName;
} AuctionDb;
extern AuctionDb g_AuctionDb;
extern ParseLink g_auction_shardsLink; 

void AuctionDb_InitSqlDb(void);
void AuctionDb_LoadShards(void);
void AuctionDb_LoadEnts(void);
void AuctionDb_Fixup(void);

void AuctionDb_InitFakeShard(void);
AuctionShard *AuctionDb_GetFakeShard(void);
AuctionShard *AuctionDb_FindAddShard(const char *name);
AuctionShard *AuctionDb_FindShard(const char *name);
AuctionEnt* AuctionDb_FindAddEnt(AuctionShard *shard, int ident, char *name);
AuctionEnt* AuctionDb_FindEnt(AuctionShard *shard, int ident);
void AuctionDb_DestroyInvItm(AuctionInvItem **hitm);
bool auctionshard_AdjInv(AuctionShard *shard, int ident, char *name,int type, int id, int amt, int price);

// *********************************************************************************
// AuctionDb SQL'ing
// *********************************************************************************
void AuctionEnt_SetInvSize(AuctionEnt *ent, int invSize);                                // updates and SQLs
void AuctionEnt_SetAccessLevel(AuctionEnt *ent, int accesslevel);                        // updates and SQLs
void AuctionEnt_ChangeShard(AuctionEnt *ent, AuctionShard *dst_shard, int dst_dbid);    // updates and SQLs
AuctionInvItem* AuctionEnt_AddItem(AuctionEnt *ent, AuctionInvItem *item);                // clones item, SQLs
void AuctionDb_DropEnt(AuctionEnt *ent);                                                // removes, logs, and SQLs
void AuctionDb_EntInsertOrUpdate(AuctionEnt *ent);                                        // only SQLs

#endif //AUCTIONDB_H
