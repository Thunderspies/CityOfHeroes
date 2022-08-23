/***************************************************************************
*     Copyright (c) 2006-2007, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
***************************************************************************/

#include "AuctionDb.h"
#include <utilitieslib/network/crypt.h>
#include "auctionhistory.h"
#include "AuctionServer.h"
#include "AsyncFileWriter.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include "auction/Auction.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/log.h>
#include "AuctionSql.h"

#define AUCTION_SHARD_ENTSBYID_INITIAL_CAPACITY 300000

AuctionDb g_AuctionDb;

// Everybody doing text I/O needs to include the fake shard - it's only
// networking that wants to skip it.
ParseLink g_auction_shardsLink =
{
    &g_AuctionDb.shardsPlusFake,
    {
        { offsetof(AuctionShard,name), 0},
    }
};

TokenizerParseInfo parse_AuctionEnt[] =
{
    // auction state
    { "{",        TOK_START,                            0},
    { "id",        TOK_INT(AuctionEnt,ident,0),        0},
    { "Name",    TOK_STRING(AuctionEnt,nameEnt,0),    0},
    { "AccessLevel",TOK_INT(AuctionEnt,accesslevel,0),        0},
    { "Shard",    TOK_LINK(AuctionEnt,shard,0,&g_auction_shardsLink)},
    { "LastId",    TOK_INT(AuctionEnt,iLastId,0),        0},
    { "Deleted",TOK_BOOL(AuctionEnt,deleted,false),    0},
    { "inv",    TOK_EMBEDDEDSTRUCT(AuctionEnt,inv,parse_AuctionInventory)},    
    { "}",        TOK_END,                            0},
    { "", 0, 0 }
};

TokenizerParseInfo parse_AuctionShard[] =
{
    { "{",            TOK_START,       0 },
    { "Name",        TOK_STRING(AuctionShard, name, NULL)},
    { "}",            TOK_END,         0 },
    { 0 },
};

// *********************************************************************************
//  AuctionShard 
// *********************************************************************************
NetLink* AuctionShard_GetLink(AuctionShard *shard)
{
    if(!shard)
        return NULL;

    while(eaSize(&shard->links))
    {
        NetLink *link;
        shard->link_rotor = (shard->link_rotor+1) % eaSize(&shard->links);
        link = shard->links[shard->link_rotor];

        if(!link || !link->connected)
        {
            eaRemove(&shard->links,shard->link_rotor);
            --shard->link_rotor;
        }
        else
            return link;
    }

    return NULL;
}

// *********************************************************************************
//  AuctionEnt 
// *********************************************************************************
static AuctionEnt* AuctionEnt_Create( AuctionShard *shard, int id, char *name, int invSize )
{
    AuctionEnt *res = NULL;

    // create the pool, arbitrary number
    res = StructAllocRaw( sizeof(AuctionEnt) );
    if( devassert( res ))
    {
        res->ident = id;
        res->nameEnt = StructAllocString(name);
        res->shard = shard;
        res->inv.invSize = invSize;
    }
    return res;
}

static void AuctionEnt_Destroy(AuctionEnt *ent)
{
    if(ent)
    {
        StructClear(parse_AuctionEnt,ent);
        StructFree(ent);
    }
}

static void AuctionEnt_UpdateName(AuctionEnt *ent, const char *name)
{
    if(ent)
    {
        if(ent->nameEnt)
            StructFreeString(ent->nameEnt);
        ent->nameEnt = name ? StructAllocString(name) : NULL;
    }
}

void AuctionEnt_ToStr(AuctionEnt *ent, char **hestr)
{
    if(!ent||!hestr)
        return;
    ParserWriteTextEscaped(hestr,parse_AuctionEnt,ent,0,0);
}

bool AuctionEnt_FromStr(AuctionEnt **hent, char *str)
{
    if(!hent || !str)
        return false;

    if(*hent)
        AuctionEnt_Destroy(*hent);
    *hent = StructAllocRaw(sizeof(AuctionEnt));

    return ParserReadTextEscaped(&str,parse_AuctionEnt,*hent);
}

void AuctionEnt_SetInvSize(AuctionEnt *ent, int invSize)
{
    if(!ent)
        return;

    if(invSize >= ent->inv.invSize || !invSize)
    {
        if(invSize != ent->inv.invSize)
        {
            ent->inv.invSize = invSize;
            AuctionDb_EntInsertOrUpdate(ent);
        }
    }
    else
        LOG( LOG_AUCTION, LOG_LEVEL_VERBOSE, 0,"%s.%s(%i):SetInvSize failed, attempted to shrink from %i to %i",
                    ent->shard->name, ent->nameEnt, ent->ident, ent->inv.invSize, invSize);
}

void AuctionEnt_SetAccessLevel(AuctionEnt *ent, int accesslevel)
{
    if(ent && ent->accesslevel != accesslevel)
    {
        ent->accesslevel = accesslevel;
        AuctionDb_EntInsertOrUpdate(ent);
    }
}

void AuctionEnt_ChangeShard(AuctionEnt *ent, AuctionShard *dst_shard, int dst_ident)
{
    bool executed;
    AuctionShard *src_shard;
    int src_ident;

    if(!ent || !dst_shard || !dst_ident)
        return;

    if(stashIntFindPointer(dst_shard->entById,dst_ident,NULL))
    {
        LOG(LOG_AUCTION, LOG_LEVEL_VERBOSE, 0, "Tried to change shards (%s[%d]%s -> %s[%d]), but dst already exists!",
            ent->shard->name,ent->ident,ent->nameEnt,dst_shard->name,dst_ident);
        return;
    }

    if(AuctionEnt_IsFakeAgent(ent))
    {
        LOG(LOG_AUCTION, LOG_LEVEL_VERBOSE, 0, "Tried to move a fake agent to a different shard");
        return;
    }

    if(dst_shard->name && stricmp(dst_shard->name, "!!FakeShard!!") == 0)
    {
        LOG(LOG_AUCTION, LOG_LEVEL_VERBOSE, 0,"Tried to move an ordinary ent to the fake shard");
        return;
    }

    stashIntRemovePointer(ent->shard->entById,ent->ident,NULL);
    eaFindAndRemove(&ent->shard->ents,ent);

    src_shard = ent->shard;
    src_ident = ent->ident;
    ent->shard = dst_shard;
    ent->ident = dst_ident;
    stashIntAddPointer(dst_shard->entById,dst_ident,ent,true);
    eaPush(&dst_shard->ents,ent);

    executed = aucsql_delete_ent(src_ident, src_shard->name);
    devassert(executed);

    AuctionDb_EntInsertOrUpdate(ent);
}


AuctionInvItem* AuctionEnt_GetItem(AuctionEnt *ent, int id)
{
    int i;
    if(!ent)
        return NULL;

    for(i = eaSize(&ent->inv.items)-1; i >= 0; --i)
        if(devassert(ent->inv.items[i]) && ent->inv.items[i]->id == id)
            return ent->inv.items[i];
    return NULL;
}

AuctionInvItem* AuctionEnt_GetItemEvenFromFake(AuctionEnt *ent, AuctionInvItem *itemReq)
{
    int i;

    if(ent && itemReq)
    {
        // Ordinary agents know their inventory so they always can explicitly
        // give the ID number. Fake agents don't receive the inventory so they
        // can't tell what they are modifying. Thus, they pass a bad ID and we
        // match the item anywhere in their existing inventory.
        if(itemReq->id <= 0 && AuctionEnt_IsFakeAgent(ent))
        {
            for(i = eaSize(&ent->inv.items) - 1; i >= 0; --i)
            {
                AuctionInvItem *item = ent->inv.items[i];

                // Names always have to match.
                if(devassert(item) && !stricmp(item->pchIdentifier, itemReq->pchIdentifier))
                {
                    // If we want to sell N units for price P, the quantities
                    // must match (stored won't have a price set yet), and the
                    // existing item(s) must be in storage, so all that will
                    // happen is we'll change the state. This stops us from
                    // stomping on other, similar auctions.
                    if(itemReq->auction_status == AuctionInvItemStatus_ForSale &&
                        item->auction_status == AuctionInvItemStatus_Stored &&
                        item->amtStored == itemReq->amtStored)
                    {
                        return item;
                    }
                }
            }
        }
        else
        {
            return AuctionEnt_GetItem(ent, itemReq->id);
        }
    }

    return NULL;
}

AuctionInvItem* AuctionEnt_AddItem(AuctionEnt *ent, AuctionInvItem *item)
{
    AuctionInvItem *item_clone = NULL;
    if(AuctionInvItem_Clone(&item_clone,item))
    {
        item_clone->iMapSide = 0;
        item_clone->lockid = 0;
        item_clone->subid = 0;
        item_clone->ent = ent;
        item_clone->id = ++ent->iLastId;

        if(g_auctionstate.cfg.iAutoFulfillTime)
            item_clone->autofulfilltime = timerSecondsSince2000() + g_auctionstate.cfg.iAutoFulfillTime;

        eaPush(&ent->inv.items,item_clone);
        AuctionDb_EntInsertOrUpdate(ent);
    }
    return item_clone;
}

bool AuctionEnt_IsFakeAgent(AuctionEnt *ent)
{
    bool retval = false;
    bool fakeNameEnt = false;
    bool fakeShard = false;

    if(ent)
    {
        // No need to waste performance on string compares if we can
        // trivially reject the possibility.
        if(ent->nameEnt && ent->nameEnt[0] == '!' && strnicmp(ent->nameEnt, "!!FakeName", 10) == 0)
            fakeNameEnt = true;

        if(ent->shard && ent->shard->name && ent->shard->name[0] == '!' && stricmp(ent->shard->name, "!!FakeShard!!") == 0)
            fakeShard = true;

        // Should only be both fake, or neither. Mismatch means an error.
        if(fakeNameEnt ^ fakeShard)
        {
            LOG( LOG_AUCTION, LOG_LEVEL_VERBOSE, 0, "Conflicting fake-agent details. Name %s, shard %s\n",
                ent->nameEnt ? ent->nameEnt : "(null)",
                ent->shard && ent->shard->name ? ent->shard->name : "(null)");
        }

        if(fakeNameEnt && fakeShard)
            retval = true;
    }

    return retval;
}

void AuctionDb_LoadShards(void)
{
    int i;
    U32 pti_crc;
    if (g_AuctionDb.shards)
    {
        FatalErrorf("AuctionDb_load called twice");
    }

    if (!aucsql_read_shards_ver(&g_AuctionDb.uShardsVer))
    {
        FatalErrorf("Could not read version number from AuctionServer SQL DB! Please see console/logs.");
    }

    pti_crc = ParseTableCRC(parse_AuctionShard, NULL);
    if(g_AuctionDb.uShardsVer != pti_crc)
    {
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: Shards had its parse table changed, ignoring crc.  This is normal for a new publish.");
        printf("Updating parse table version for shards.\n");
        aucsql_write_shards_ver(pti_crc);
    }

    if(!aucsql_read_shards(parse_AuctionShard, &g_AuctionDb.shards))
    {
        FatalErrorf("Shard table could not be read! Could not load. Please see console/logs.");
    }

    // We keep two lists of shards. One is just the real shards and is used
    // for network connectivity and so on, the other also has the fake shard
    // and is used for journaling. The former is authoritative so we must
    // mirror it onto the latter list.
    for( i = eaSize(&g_AuctionDb.shards) - 1; i >= 0; --i)
    {
        eaPushUnique(&g_AuctionDb.shardsPlusFake, g_AuctionDb.shards[i]);
    }

    g_AuctionDb.shardByName = stashTableCreateWithStringKeys(stashOptimalSize(eaSize(&g_AuctionDb.shards)), StashDefault);
    for( i = eaSize(&g_AuctionDb.shards) - 1; i >= 0; --i)
    {
        AuctionShard *shard = g_AuctionDb.shards[i];
        if ( !shard->name )
            printf("Shard specified in shards.def does not have a name, and will not be added to shard list: %d\n", i);
        else
        {
            stashAddPointer( g_AuctionDb.shardByName, shard->name, shard, false );
            if(!shard->entById)
                shard->entById = stashTableCreateInt(AUCTION_SHARD_ENTSBYID_INITIAL_CAPACITY);
        }
    }
}

void AuctionDb_LoadEnts(void)
{
    int i;
    U32 pti_crc;
    if (g_AuctionDb.ents)
    {
        FatalErrorf("AuctionDb_load called twice");
    }

    if (!aucsql_read_ents_ver(&g_AuctionDb.uEntsVer))
    {
        FatalErrorf("Could not read version number from AuctionServer SQL DB! Please see console/logs.");
    }

    pti_crc = ParseTableCRC(parse_AuctionEnt, NULL);
    if(g_AuctionDb.uEntsVer != pti_crc)
    {
        LOG(LOG_TEXT_JOURNAL, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: Ents had its parse table changed, ignoring crc.  This is normal for a new publish.");
        printf("Updating parse table version for ents.\n");
        aucsql_write_ents_ver(pti_crc);
    }

    if(!aucsql_read_ents(parse_AuctionEnt, &g_AuctionDb.ents))
    {
        FatalErrorf("Ents table could not be read! Could not load. Please see console/logs.");
    }

    // fill shard lists
    for( i = eaSize(&g_AuctionDb.ents) - 1; i >= 0; --i)
    {
        AuctionEnt *ent = g_AuctionDb.ents[i];
        AuctionShard *shard = ent->shard;

        if(!stashIntAddPointer(shard->entById,ent->ident,ent,false))
            printf("two ents with the same id:%i %s\n",ent->ident,ent->nameEnt);
        eaPush(&shard->ents,ent);
    }

    // If auto-fulfill is turned on we need to set the auto-fulfill time for
    // all bids and sales.
    if(g_auctionstate.cfg.iAutoFulfillTime)
    {
        for(i = eaSize(&g_AuctionDb.ents) - 1; i >= 0; --i)
        {
            if(g_AuctionDb.ents[i] && g_AuctionDb.ents[i]->inv.items)
            {
                int j;

                for(j = eaSize(&g_AuctionDb.ents[i]->inv.items) - 1; j >= 0; --j)
                {
                    AuctionInvItem *itm = g_AuctionDb.ents[i]->inv.items[j];

                    if(itm && (itm->auction_status == AuctionInvItemStatus_Bidding || itm->auction_status == AuctionInvItemStatus_ForSale))
                        itm->autofulfilltime = timerSecondsSince2000() + g_auctionstate.cfg.iAutoFulfillTime;
                }
            }
        }
    }
}

static char *bad_parts[] = 
{
    "11 S_AndroidCircuitry", 
    "11 S_AndroidBlaster", 
    "11 S_BrainLichen",
    "11 S_PrimordialMoss", 
    "11 S_AndroidArmorPlate", 
    "11 S_PlagueSpores", 
    "11 S_UndamagedAndroidBrain", 
    "11 S_ProgenitorLichen",
};

void AuctionDb_Fixup(void)
{
    int i;
    for(i = eaSize(&g_AuctionDb.ents)-1; i >= 0; --i)
    {
        int j;
        AuctionEnt *ent = g_AuctionDb.ents[i];

        ent->inv.owner = ent;
        for(j = eaSize(&ent->inv.items)-1; j >= 0; --j) 
        {
            AuctionInvItem *itm = ent->inv.items[j];
            itm->ent = ent;
            if(itm->id > ent->iLastId)
            {
                LOG( LOG_AUCTION, LOG_LEVEL_VERBOSE, 0, "Error: AucInvItem's id %i greater than %s(%i) %s's iLastId %i",
                            itm->id, SAFE_MEMBER(ent->shard,name), ent->ident, ent->nameEnt, ent->iLastId);
                ent->iLastId = itm->id;
                AuctionDb_EntInsertOrUpdate(ent);
            }
            if(itm->amtStored < 0)
            {
                LOG( LOG_AUCTION, LOG_LEVEL_IMPORTANT, 0, "Error: %s(%i) %s[%i] has negative amtStored, Deleting",
                            SAFE_MEMBER(ent->shard,name), ent->ident, ent->nameEnt, itm->id);
                eaRemove(&ent->inv.items,j);
                AuctionInvItem_Destroy(itm);
                AuctionDb_EntInsertOrUpdate(ent);
            }
            else
            {
                int k;
                for(k = ARRAY_SIZE(bad_parts)-1; k >=0; k--)
                {
                    if( stricmp(itm->pchIdentifier, bad_parts[k]) == 0)
                    {
                        eaRemove(&ent->inv.items,j);
                        AuctionInvItem_Destroy(itm);
                        AuctionDb_EntInsertOrUpdate(ent);
                        break;
                    }
                }
            }
        }
    }
}


AuctionEnt* AuctionDb_FindAddEnt(AuctionShard *shard, int ident, char *name)
{
    AuctionDb *db = &g_AuctionDb;
    AuctionEnt *ent = NULL;
    
    if(!db||!shard||!ident||!name)
        return NULL;
    
    if(!stashIntFindPointer(shard->entById,ident,&ent))
    {
        ent = AuctionEnt_Create(shard,ident,name,0);
        eaPush(&db->ents,ent);
        eaPush(&shard->ents,ent);
        if(!stashIntAddPointer(shard->entById,ident,ent,false))
        {
            LOG( LOG_AUCTION, LOG_LEVEL_VERBOSE, 0, "fail to add entity %s(%i)",name,ident)
        }
        else
            LOG( LOG_AUCTION, LOG_LEVEL_VERBOSE, 0, "create ent '%s(%i)' for shard %s", name, ident,shard->name);
    }

    // player's name changed
    if(0!=stricmp(ent->nameEnt,name))
    {
        LOG( LOG_AUCTION, LOG_LEVEL_VERBOSE, 0, "Warning: name changed. AuctionEnt %s(%i) changed from '%s' to '%s'",shard->name,ident,ent->nameEnt,name);
        AuctionEnt_UpdateName(ent,name);
    }
    return ent;
}

AuctionEnt* AuctionDb_FindEnt(AuctionShard *shard, int ident)
{
    AuctionEnt *ent = NULL;
    stashIntFindPointer(shard->entById,ident,&ent);
    return ent;
}

void AuctionDb_DestroyInvItm(AuctionInvItem **hitm)
{
    int i;
    AuctionEnt *ent = hitm?(*hitm)->ent:NULL;
    
    if(!hitm)
        return;

    if(ent)
    {
        i = eaFind(&ent->inv.items,*hitm);
        if(i>=0)
            eaRemove(&ent->inv.items,i);
    }
    AuctionInvItem_Destroy(*hitm);
    *hitm = NULL;
}

static AuctionShard s_fakeShard;
static char* s_fakeShardName = "!!FakeShard!!";

void AuctionDb_InitFakeShard(void)
{
    memset(&s_fakeShard, 0, sizeof(s_fakeShard));

    s_fakeShard.name = &(s_fakeShardName[0]);
    s_fakeShard.entById = stashTableCreateInt(AUCTION_SHARD_ENTSBYID_INITIAL_CAPACITY);

    eaPushUnique(&g_AuctionDb.shardsPlusFake, &s_fakeShard);
}

AuctionShard *AuctionDb_GetFakeShard(void)
{
    return &s_fakeShard;
}

AuctionShard *AuctionDb_FindShard(const char *name)
{
    AuctionShard *res;
    if(name && stricmp(name, "!!FakeShard!!") == 0)
        return &s_fakeShard;
    if(stashFindPointer(g_AuctionDb.shardByName,name,&res))
        return res;
    return NULL;
}

AuctionShard *AuctionDb_FindAddShard(const char *name)
{
    AuctionDb *db = &g_AuctionDb;
    AuctionShard *res;
    char* estr_shard = NULL;
    bool executed;
    
    if(!name)
        return NULL;
    
    // give the shard if we already have it
    res = AuctionDb_FindShard(name);
    if(res)
        return res;

    // fake shard should always exist
    if(!res && stricmp(name, "!!FakeShard!!") == 0)
        FatalErrorf("Fake shard not found, should be static");

    // create the shard
    res = StructAllocRaw(sizeof(AuctionShard));
    eaPush(&db->shards,res);
    eaPush(&db->shardsPlusFake,res);
    res->name = StructAllocString(name);
    res->entById = stashTableCreateInt(AUCTION_SHARD_ENTSBYID_INITIAL_CAPACITY);
    stashAddPointer(db->shardByName,res->name,res,true);

    // persist the (should be tiny) shard database
    ParserWriteText(&estr_shard,parse_AuctionShard,res,0,0);
    executed = aucsql_insert_shard(&estr_shard);
    devassert(executed);
    estrDestroy(&estr_shard);
    printf("%s added to shard database.\n",name);

    return res;
}

void AuctionDb_DropEnt(AuctionEnt *ent)
{
    AuctionDb *db = &g_AuctionDb;
    char *drop_str = NULL;
    bool executed;

    if(!ent)
        return;

    executed = aucsql_delete_ent(ent->ident, ent->shard->name);
    devassert(executed);

    AuctionEnt_ToStr(ent,&drop_str);
    LOG( LOG_AUCTION, LOG_LEVEL_VERBOSE, 0, "dropped_invs: Dropped %s[%i] %s\t%s", (ent->shard?ent->shard->name:"[null shard]"), ent->ident, ent->nameEnt, drop_str);
    estrDestroy(&drop_str);

    if( ent->shard )
    {
        stashIntRemovePointer(ent->shard->entById,ent->ident,NULL);
        eaFindAndRemove(&ent->shard->ents,ent);
    }

    eaFindAndRemoveFast(&db->ents,ent);
    AuctionEnt_Destroy(ent);
}

// *********************************************************************************
// ents SQL write
// *********************************************************************************

void AuctionDb_EntInsertOrUpdate(AuctionEnt *ent)
{
    char *estr_ent = estrTemp();
    bool executed;
    ParserWriteText(&estr_ent, parse_AuctionEnt, ent, 0, 0);
    executed = aucsql_insert_or_update_ent(ent->ident, ent->shard->name, &estr_ent);
    devassert(executed);
    estrDestroy(&estr_ent);
}
