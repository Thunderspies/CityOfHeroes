/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/textparser.h> // for TokenizerParseInfo
#include "entity/Concept.h"
#include "entity/character_inventory.h"
#include <utilitieslib/components/earray.h>

#include "entity/origins.h"
#include "entity/powers.h"
#include "entity/boost.h"
#include "entity/character_base.h"
#include <utilitieslib/components/MemoryPool.h>
#include "entity/Proficiency.h"

// compile time checks.
STATIC_ASSERT(IS_GENERICINVENTORYTYPE_COMPATIBLE(ProficiencyItem,id,name));
STATIC_ASSERT(IS_GENERICINVENTORYDICT_COMPATIBLE(ProficiencyDictionary,ppProficiencyItems,haItemNames,itemsById));

SHARED_MEMORY ProficiencyDictionary g_ProficiencyDict;

StaticDefineInt OriginEnum[] =
{
    DEFINE_INT
    { "Tech", kProfOriginType_Tech},
    { "Technology", kProfOriginType_Tech},
    { "Science", kProfOriginType_Science},
    { "Mutation", kProfOriginType_Mutant},
    { "Mutant", kProfOriginType_Mutant},
    { "Magic", kProfOriginType_Magic},
    { "Natural", kProfOriginType_Natural},
    DEFINE_END
};

static StaticDefineInt RarityEnum[] =
{
    DEFINE_INT
    { "Ubiquitous", kProfRarityType_Ubiquitous},
    { "Common", kProfRarityType_Common},
    { "Uncommon", kProfRarityType_Uncommon},
    { "Rare", kProfRarityType_Rare},
    { "Unique", kProfRarityType_Unique},
    DEFINE_END
};

static TokenizerParseInfo ParseProficiencyDef[] = 
{
    { "{",              TOK_START,                           0},
    { "Name",           TOK_STRING(ProficiencyItem, name, 0)    },
    TOKENIZERUIWIDGET_INLINEPARSEINFO(ProficiencyItem),
     { "Origin",            TOK_STRUCTPARAM | TOK_INT(ProficiencyItem,origin, 0), OriginEnum },
     { "Rarity",            TOK_STRUCTPARAM | TOK_INT(ProficiencyItem,rarity, 0), RarityEnum }, //default to 1 slot used
    { "}",                TOK_END,        0 },
    { "", 0, 0 }
};


//------------------------------------------------------------
// parent of the proficiency
// -AB: created :2005 Feb 10 05:33 PM
//----------------------------------------------------------
TokenizerParseInfo ParseProficiencyDictionary[] =
{
    { "Proficiency", TOK_STRUCT(ProficiencyDictionary, ppProficiencyItems, ParseProficiencyDef)},
    { "", 0, 0 }
};


//------------------------------------------------------------
//  create the hashes for the proficiencies
//----------------------------------------------------------
bool proficiency_FinalProcess(ParseTable* pti, void* structptr, bool shared_memory)
{
    ProficiencyDictionary * pdict = (ProficiencyDictionary *)structptr;
    if( verify( pdict ))
    {
        genericinvdict_CreateHashes((GenericInvDictionary*)pdict, proficiency_GetAttribs(), shared_memory);
    }
    return 1;
}

TokenizerParseInfo* proficiency_GetParseInfo()
{
    return ParseProficiencyDictionary;
}

bool proficiency_ValidId(int id)
{
    return EAINRANGE( id, g_ProficiencyDict.itemsById );
}


ProficiencyItem const* proficiency_GetItem(char const *name)
{
    return (const ProficiencyItem*)stashFindPointerReturnPointerConst( g_ProficiencyDict.haItemNames, name);
}


ProficiencyItem const* proficiency_GetItemById(int id)
{
    if( verify( proficiency_ValidId(id)))
    {
        assert(g_ProficiencyDict.itemsById[id]->id == id);
        return g_ProficiencyDict.itemsById[id];
    }
    return NULL;
}

//------------------------------------------------------------
//  get the attribs for the proficiencies
//----------------------------------------------------------
AttribFileDict const* proficiency_GetAttribs()
{
    static AttribFileDict s_attribs = {0};
    return &s_attribs;
}

//------------------------------------------------------------
//  get the dbid filename
//----------------------------------------------------------
char const * proficiency_GetIdmapFilename()
{
    return "defs/ProficiencyIds.dbidmap";
}

#if SERVER
//------------------------------------------------------------
//  Write the unique ids to the dbid file
//----------------------------------------------------------
void proficiency_WriteDbIds()
{
    genericinvtype_WriteDbIds((GenericInventoryType**)g_ProficiencyDict.itemsById, 
                              proficiency_GetAttribs(), 
                              proficiency_GetIdmapFilename());
}
#endif

