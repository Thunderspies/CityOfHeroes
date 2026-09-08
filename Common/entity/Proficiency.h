/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef PROFICIENCY_H
#define PROFICIENCY_H

#include <utilitieslib/stdtypes.h>
#include "TokenizerUiWidget.h"
#include <utilitieslib/utils/structDefines.h>

// --------------------
// forward decls

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct ParseTable ParseTable;
#define TokenizerParseInfo ParseTable
typedef struct BasePower BasePower;
typedef struct Boost Boost;
typedef struct AttribFileDict AttribFileDict;
typedef struct Character Character;

typedef enum ProfOriginType
{
    kProfOriginType_Tech,
    kProfOriginType_Science,
    kProfOriginType_Mutant,
    kProfOriginType_Magic,
    kProfOriginType_Natural,
    kProfOriginType_Count
} ProfOriginType;


typedef enum ProfRarityType
{
    kProfRarityType_Ubiquitous,
    kProfRarityType_Common,
    kProfRarityType_Uncommon,
    kProfRarityType_Rare,
    kProfRarityType_Unique,
    kProfRarityType_Count,
} ProfRarityType;


//------------------------------------------------------------
//  definition of a proficiency item as loaded from def files.
// A proficiency is used to craft Enhancements from Templates. they
// are 'slotted' into the template
// GenericInventoryType member-compatible
//----------------------------------------------------------
typedef struct ProficiencyItem
{ 
    int id;
    char *name;
    TokenizerUiWidget    ui;
    ProfOriginType origin;
    ProfRarityType rarity;
} ProficiencyItem;


// GenericInventoryDict member-compatible
typedef struct ProficiencyDictionary
{
    // Defines a set of related categories. (Examples include character and
    // villain)
    const ProficiencyItem **ppProficiencyItems;
    cStashTable haItemNames;
    const ProficiencyItem **itemsById;
} ProficiencyDictionary;

// global inst of dict
extern SHARED_MEMORY ProficiencyDictionary g_ProficiencyDict;
extern StaticDefineInt OriginEnum[];
bool proficiency_FinalProcess(TokenizerParseInfo *pti, ProficiencyDictionary *pdict, bool shared_memory);
TokenizerParseInfo* proficiency_GetParseInfo();

ProficiencyItem const* proficiency_GetItem(char const *name);
ProficiencyItem const* proficiency_GetItemById(int id);

bool proficiency_ValidId(int id);
int proficiency_MaxId();

AttribFileDict const* proficiency_GetAttribs();
char const *proficiency_GetIdmapFilename();

#if SERVER
void proficiency_WriteDbIds();
#endif

#endif //PROFICIENCY_H
