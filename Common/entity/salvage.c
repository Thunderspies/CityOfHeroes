/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/textparser.h> // for TokenizerParseInfo
#include "entity/character_inventory.h"
#include "entity/salvage.h"
#include "bases/basedata.h"
#include "bases/baseparse.h"
#include "gameComm/trayCommon.h"
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/SharedHeap.h>
#include "entity/character_eval.h"

// ------------------------------------------------------------
// globals

//------------------------------------------------------------
// represents the global constant salvage and its
//  hash table.
//
// mostly global due to load_def.c, otherwise static may
//  be better.
// -AB: created :2005 Feb 15 05:49 PM
//----------------------------------------------------------
SHARED_MEMORY SalvageDictionary g_SalvageDict;
SERVER_SHARED_MEMORY SalvageTrackedByEntList g_SalvageTrackedList;

STATIC_ASSERT(IS_GENERICINVENTORYTYPE_COMPATIBLE(SalvageItem,salId,pchName));
STATIC_ASSERT(IS_GENERICINVENTORYDICT_COMPATIBLE(SalvageDictionary,ppSalvageItems,haItemNames,itemsById))


ParseLink g_salvageInfoLink =
{
    (void*)&g_SalvageDict.ppSalvageItems,
    {
        { offsetof( SalvageItem, pchName ), 0 },
    }
};


//------------------------------------------------------------
// enum choices for the rarity
// -AB: created :2005 Feb 15 12:05 PM
//----------------------------------------------------------
StaticDefineInt RarityEnum[] =
{
    DEFINE_INT
    { "Ubiquitous", kSalvageRarity_Ubiquitous },
    { "Common",     kSalvageRarity_Common },
    { "Uncommon",   kSalvageRarity_Uncommon },
    { "Rare",       kSalvageRarity_Rare },
    { "VeryRare",    kSalvageRarity_VeryRare },
    { "Unique",     kSalvageRarity_Unique },
    { "Count",      kSalvageRarity_Count },
    DEFINE_END
};

//------------------------------------------------------------
// enum choices for the type
//----------------------------------------------------------
StaticDefineInt TypeEnum[] =
{
    DEFINE_INT
    { "Base",            kSalvageType_Base },
    { "Invention",        kSalvageType_Invention },
    { "Token",            kSalvageType_Token },
    { "Incarnate",        kSalvageType_Incarnate },
    { "Count",            kSalvageType_Count },
    DEFINE_END
};


StaticDefineInt ParseSalvageFlags[] =
{
    DEFINE_INT
    { "NoTrade",        SALVAGE_NOTRADE },
    { "NoDelete",        SALVAGE_NODELETE },
    { "ImmediateUse",    SALVAGE_IMMEDIATE },
    { "AutoOpen",        SALVAGE_AUTO_OPEN },
    { "NoAuction",        SALVAGE_NOAUCTION },
    DEFINE_END
};

#define SALVAGECATEGORY_DEFAULT "none"

static TokenizerParseInfo ParseSalvageDef[] = {
    { "{",                            TOK_START,                           0},
    { "Name",                        TOK_STRING(SalvageItem, pchName, 0) },
    TOKENIZERUIWIDGET_INLINEPARSEINFO(SalvageItem),
    { "DisplayTabName",                TOK_STRING(SalvageItem, pchDisplayTabName, 0) },
    { "DisplayDropMessage",            TOK_STRING(SalvageItem, pchDisplayDropMsg, 0) },
    { "Rarity",                        TOK_INT(SalvageItem,rarity, 0), RarityEnum },
    { "Type",                        TOK_INT(SalvageItem, type, 0), TypeEnum },
    { "Workshop",                    TOK_LINKARRAY(SalvageItem, ppWorkshops, &g_base_detailInfoLink) },
    { "ChallengePoints",            TOK_INT(SalvageItem, challenge_points, 0) },
    { "RewardTableName",            TOK_STRINGARRAY(SalvageItem, ppchRewardTables) },
    { "OpenRequires",                TOK_STRINGARRAY(SalvageItem, ppchOpenRequires) },
    { "AuctionRequires",            TOK_STRINGARRAY(SalvageItem, ppchAuctionRequires) },
    { "DisplayOpenRequiresFailed",    TOK_STRING(SalvageItem, pchDisplayOpenRequiresFail, 0) },
    { "MinReverseEngineerLevel",    TOK_INT(SalvageItem, minRevEngLevel, 0) },
    { "MaxInventoryAmount",            TOK_INT(SalvageItem, maxInvAmount, 0) },
    { "SellAmount",                    TOK_INT(SalvageItem, sellAmount, 0) },
    { "Flags",                        TOK_FLAGS(SalvageItem,flags,0), ParseSalvageFlags }, 
    { "StoreProduct",                TOK_STRING(SalvageItem, pchStoreProduct, 0) },
    { "}",                            TOK_END,0 },
    { "", 0, 0 }
};


//------------------------------------------------------------
// parent of the salvage
// -AB: created :2005 Feb 10 05:33 PM
//----------------------------------------------------------
TokenizerParseInfo ParseSalvageDictionary[] =
{
    { "Salvage", TOK_STRUCT(SalvageDictionary, ppSalvageItems, ParseSalvageDef) },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseTrackedSalvage[] = {
    { "{",                            TOK_START,                           0},
    { "SalvageName",                TOK_STRING(SalvageTrackedByEnt, salvageName, 0) },
    { "DisplayName",                TOK_STRING(SalvageTrackedByEnt, displayName, 0) },
    { "}",                            TOK_END,0 },
    { "", 0, 0 }
};
TokenizerParseInfo ParseTrackedSalvageParseInfo[] =
{
    { "TrackedSalvage", TOK_STRUCT(SalvageTrackedByEntList, ppTrackedSalvage, ParseTrackedSalvage) },
    { "", 0, 0 }
};

/**
* @note If any bug fixes are made here also fix @ref detailrecipe_CreateTabNameHash.
*/
static bool salvage_CreateTabNameHash(SalvageDictionary *pdict, bool shared_memory)
{
    bool ret = true;
    int i;

    StashTableIterator iter;
    StashElement elem;

    StashTable tempItemsFromTabName = stashTableCreateWithStringKeys(eaSize(&pdict->ppSalvageItems), 0);

    // hash lists into a temporary table
    for (i = 0; i < eaSize(&pdict->ppSalvageItems); i++)
    {
        SalvageItem *salvage = cpp_const_cast(SalvageItem*)(pdict->ppSalvageItems[i]);
        SalvageItem **ppSal = NULL;

        if (!stashFindElementConst(tempItemsFromTabName, salvage->pchDisplayTabName, &elem))
        {
            eaCreateWithCapacity(&ppSal, 16);
            stashAddPointerAndGetElement(tempItemsFromTabName, salvage->pchDisplayTabName, ppSal, false, &elem);
        }

        ppSal = stashElementGetPointer(elem);
        eaPush(&ppSal, salvage);
        stashElementSetPointer(elem, ppSal);
    }

    assert(!pdict->itemsFromTabName);
    pdict->itemsFromTabName = stashTableCreateWithStringKeys(stashOptimalSize(stashGetValidElementCount(tempItemsFromTabName)), stashShared(shared_memory));

    // build the final table from the temporary one
    stashGetIterator(tempItemsFromTabName, &iter);    
    while (stashGetNextElement(&iter, &elem))
    {
        SalvageItem **tempSal = stashElementGetPointer(elem);
        SalvageItem **sharedSal = NULL;
        
        if (shared_memory)
        {
            eaCompress(&sharedSal, &tempSal, customSharedMalloc, NULL);
            eaDestroy(&tempSal);
        }
        else
            sharedSal = tempSal;
    
        stashAddPointerConst(pdict->itemsFromTabName, stashElementGetStringKey(elem), sharedSal, false);
    }

    stashTableDestroy(tempItemsFromTabName);

    return ret;
}

//------------------------------------------------------------
// init the salvage table
//----------------------------------------------------------
bool salvage_FinalProcess(ParseTable* pti, void* structptr, bool shared_memory)
{
    SalvageDictionary * pdict = (SalvageDictionary *)structptr;
    bool ret = true;
    int i,j;
    int rarityCount = 0;

    SalvageItem **tempItemsById = NULL;

    struct MaxIdxHashTablePair
    {
        U32 nextIdx;
        cStashTable ht;
    } idHt = {0};

    // get the id hashes
    AttribFileDict const*dict = inventorytype_GetAttribs( kInventoryType_Salvage );
    idHt.ht = dict->idHash;

    // set the next index
    // returns max index found, so we count after that. works even for zero case
    idHt.nextIdx = 1 + dict->maxId;

    assert(!pdict->haItemNames);
    pdict->haItemNames = stashTableCreateWithStringKeys( stashOptimalSize(eaSize(&pdict->ppSalvageItems)), stashShared(shared_memory) );

    // --------------------
    // fill hash, set ids

    for( i = 0; i < eaSize( &pdict->ppSalvageItems ); ++i )
    {
        SalvageItem *salvage = cpp_const_cast(SalvageItem*)(pdict->ppSalvageItems[i]);
        int idx;

        // --------------------
        // hash of names

        for(j = (int)strlen(salvage->pchName)-1; j >= 0; --j)
            if(!isalnum(salvage->pchName[j]) && salvage->pchName[j] != '_')
            {
                Errorf("bad salvage name \"%s\", only alphanumerics and underscore are allowed",salvage->pchName);
                ret =  false;
                break;
            }

        // set id
        if( stashFindInt( idHt.ht, salvage->pchName, &idx ) )
        {
            salvage->salId = idx; // use the existing value
        }
        else
        {
            salvage->salId = idHt.nextIdx++; // grab the highest and incr
        }

        // add the hash
        if( !stashAddPointerConst(pdict->haItemNames, salvage->pchName, salvage, false) )
        {
            Errorf("duplicate salvage name %s", salvage->pchName);
            ret =  false;
        }

        if (salvage->ppchAuctionRequires)
            chareval_Validate(salvage->ppchAuctionRequires, "defs/invention/salvage.salvage");
        if (salvage->ppchOpenRequires)
            chareval_Validate(salvage->ppchOpenRequires, "defs/invention/salvage.salvage");
    }

    ret &= salvage_CreateTabNameHash(pdict, shared_memory);

    // --------------------
    // do the items by id

    eaCreateWithCapacityConst(&tempItemsById, idHt.nextIdx);
    eaSetSize(&tempItemsById, idHt.nextIdx);

    for( i = eaSize( &pdict->ppSalvageItems ) - 1; i >= 0; --i)
    {
        SalvageItem *salvage = cpp_const_cast(SalvageItem*)(pdict->ppSalvageItems[i]);
        assert(eaSet(&tempItemsById, salvage, salvage->salId));
    }

    // Pack into shared memory
    if (shared_memory)
    {
        assert(!pdict->itemsById);
        eaCompressConst(&pdict->itemsById, &tempItemsById, customSharedMalloc, NULL);
        eaDestroy(&tempItemsById);
    }
    else
        pdict->itemsById = tempItemsById;

    return ret;
}

//------------------------------------------------------------
// accessor to the salvage TokenizerParseInfo
// -AB: created :2005 Feb 15 02:16 PM
//----------------------------------------------------------
TokenizerParseInfo* salvage_GetParseInfo()
{
    return ParseSalvageDictionary;
}

TokenizerParseInfo* salvageTrackedByEntParseInfo()
{
    return ParseTrackedSalvageParseInfo;
}

//------------------------------------------------------------
// find the salvage item from the passed id
// -AB: created :2005 Feb 17 04:00 PM
//----------------------------------------------------------
const SalvageItem*
salvage_GetItem( char const *name )
{
    return (const SalvageItem*)stashFindPointerReturnPointerConst( g_SalvageDict.haItemNames, name );
}

//------------------------------------------------------------
// helper for lookup of the SalvageRarity enum
// returns: the string, or <invalid rarity> if invalid.
//----------------------------------------------------------
char const*
salvage_RarityToStr( SalvageRarity r )
{
    return verify( INRANGE( r, 0, kSalvageRarity_Count+1 )) ? StaticDefineIntRevLookup( RarityEnum, r ) : "<invalid rarity>";
}



//------------------------------------------------------------
// get an item by its id
// -AB: created :2005 Feb 23 05:48 PM
//----------------------------------------------------------
const SalvageItem*
salvage_GetItemById( int id )
{
    return EAINRANGE( id, g_SalvageDict.itemsById ) ? g_SalvageDict.itemsById[id] : NULL;
}

//------------------------------------------------------------
// check if the passed rarity is valid
// -AB: created :2005 Feb 24 05:23 PM
//----------------------------------------------------------
int
salvage_ValidRarity( int rarity )
{
    return INRANGE( rarity, 0, kSalvageRarity_Count );
}


/* End of File */
