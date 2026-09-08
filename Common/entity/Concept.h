/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef CONCEPT_H
#define CONCEPT_H

#include <utilitieslib/stdtypes.h>
#include "TokenizerUiWidget.h"

// --------------------
// forward decls

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct ParseTable ParseTable;
#define TokenizerParseInfo ParseTable
typedef struct BasePower BasePower;
typedef struct Boost Boost;
typedef struct RewardSlot RewardSlot;

typedef enum UiConceptAttribmodGroupFlags
{
    kUiConceptAttribmodGroupFlags_NONE,
    kUiConceptAttribmodGroupFlags_ShowAttribmodGroup = 1,
    kUiConceptAttribmodGroupFlags_ShowRange = kUiConceptAttribmodGroupFlags_ShowAttribmodGroup<<1,
    kUiConceptAttribmodGroupFlags_ALL = (kUiConceptAttribmodGroupFlags_ShowRange | kUiConceptAttribmodGroupFlags_ShowAttribmodGroup | kUiConceptAttribmodGroupFlags_NONE),
} UiConceptAttribmodGroupFlags;


typedef enum ConceptId
{
    kConceptId_Invalid = 0,
} ConceptId;


//------------------------------------------------------------
//  helper for an attribmod group and a float range
// intended for use to determine the attrib mod when the concept
// is slotted
//----------------------------------------------------------
typedef struct AttribModGroup
{ 
    const char *name;
    float min;
    float max;

    // optional
    UiConceptAttribmodGroupFlags uiFlags;
} AttribModGroup;

// ============================================================
// potential enhancement rewards
//
// these rewards take effect when a potential enhancement is created

typedef enum PECreationRewardType
{
    kPECreationReward_ChangeHardenLevel,  // change harden level
    kPECreationReward_ChangeSellback,     // increase sellback by factor
    kPECreationReward_SpawnAmbush,        // create an ambush
    kPECreationReward_WisdomMultiplier,   // increase wisdom reward by factor
    kPECreationReward_PrefillPowerupCost, // after hardened, fill some slots
    kPECreationReward_Count,
} PECreationRewardType;

//------------------------------------------------------------
//  describes the rewards for when a potential enhancement is
// fully created from a recipe.
// PECreationReward SpawnAmbush <spawn name>
//----------------------------------------------------------
typedef struct PECreationRewardDef
{
    PECreationRewardType type;
    char const *param;
} PECreationRewardDef;

// ============================================================
// concept hardening rewards

typedef enum HardenedAttribsTarget
{
    kHardenedAttribsTarget_AllPrev,
    kHardenedAttribsTarget_Prev,
    kHardenedAttribsTarget_Next,
    kHardenedAttribsTarget_AllNext,
    kHardenedAttribsTarget_Count 
} HardenedAttribsTarget;

//------------------------------------------------------------
//  Concept hardening rewards have the ability to affect future
// and previous 
//----------------------------------------------------------
typedef enum ConceptHardeningRewardType
{ 
    kConceptHardeningRewardType_Reward, // <tablename> <level>
    kConceptHardeningRewardType_ChangeHardenedAttribs, // <HardenedAttribsTarget> <amount> 
    kConceptHardeningRewardType_ReducePowerupCost, // <HardenedAttribsTarget> <amount>
    kConceptHardeningRewardType_Count
} ConceptHardeningRewardType;


//------------------------------------------------------------
//  describes a concept hardening reward
// ConceptHardenReward Reward StoryArcSL2 1
//----------------------------------------------------------
typedef struct ConceptHardeningRewardDef
{
    ConceptHardeningRewardType type;
    char const *param0;
    char const *param1;
} ConceptHardeningRewardDef;


// ============================================================
// slotted concept affecting rewards
//
// these things can only affect concepts that have been slotted
// but not hardened

typedef enum SlottedConceptAffectingRewardType
{
    kSlottedConceptAffectingRewardType_IncreaseHardeningChance, // <amount> increases hardening chance
    kSlottedConceptAffectingRewardType_Count,
} SlottedConceptAffectingRewardType;


typedef struct SlottedConceptAffectingRewardDef
{
    SlottedConceptAffectingRewardType type;
    char const *param;
} SlottedConceptAffectingRewardDef;


// ============================================================
// hardened concept affecting rewards
//
// these rewards only affect hardened concepts


//------------------------------------------------------------
//  Rewards that affect hardened slots in an invention.
// see: Invention.c:s_slotInHardened if you change this
//----------------------------------------------------------
typedef enum HardenedAffectingRewardType
{
    kHardenedAffectingRewardType_EmptySlot, // clears the slot its dropped on
    kHardenedAffectingRewardType_Count
} HardenedAffectingRewardType;

typedef struct HardenedConceptAffectingRewardDef
{
    HardenedAffectingRewardType type;
    char const *param;
} HardenedConceptAffectingRewardDef;


// ============================================================
// Concepts

// ------------------------------------------------------------
// concept def

//------------------------------------------------------------
//  definition of a concept item as loaded from def files.
// A concept is used to craft Enhancements from Templates. they
// are 'slotted' into the template
//----------------------------------------------------------
typedef struct ConceptDef
{
    int id;
    const char *name;
    TokenizerUiWidget    ui;
    const AttribModGroup* const* attribMods;
    const RewardSlot* const* powerupCostSlots;
    int modSellback; // amount to add/subtract to sellback
    F32 modEndurance; // as a percent multiplier
    const char* const* slotRewardTables;
    F32 slotSuccessChance;
    int slotsUsed; // slots used when slotted

    // --------------------
    // rewards

    const PECreationRewardDef* const* peCreationRewards;
    const ConceptHardeningRewardDef* const* hardeningRewards;

    const SlottedConceptAffectingRewardDef* const* slottedAffectingRewards;
    const HardenedConceptAffectingRewardDef* const* hardenedAffectingRewards;
} ConceptDef;

const ConceptDef* conceptdef_Get(char const *name);
const ConceptDef* conceptdef_GetById(int id);
int conceptdef_GetAttribModGroupIdx(const ConceptDef *def, char const *name);


bool conceptdef_ValidId(int id);
int conceptdef_MaxId();

// ------------------------------------------------------------
// concept item

#define CONCEPTITEM_MAX_VARS (4)

//------------------------------------------------------------
//  A concept in player's posession. contains rolled vars for the
// attribmods 
//----------------------------------------------------------
typedef struct ConceptItem
{ 
    const ConceptDef *def;
    F32 afVars[CONCEPTITEM_MAX_VARS];
} ConceptItem;

ConceptItem* conceptitem_Create( int defId, F32 *afVars );
ConceptItem* conceptitem_Init(ConceptItem *item, int defId, F32 *afVars );
void conceptitem_Destroy( ConceptItem *item );

// ------------------------------------------------------------
// concept dictionary

typedef struct ConceptDictionary
{
    // Defines a set of related categories. (Examples include character and
    // villain)
    const ConceptDef **ppConceptDefs;
    StashTable haItemNames;
    const ConceptDef **itemsById;
} ConceptDictionary;

// global inst of dict
extern SHARED_MEMORY ConceptDictionary g_ConceptDict;
bool conceptdict_FinalProcess(ParseTable* pti, void* structptr, bool shared_memory);
TokenizerParseInfo* conceptdef_GetParseInfo();

bool basepower_CanApplyConcept( const BasePower *recipe, ConceptDef const *concept );
Boost* boost_ApplyConcept( Boost *resBoost, const BasePower *recipe, ConceptItem const *concept );

#endif //CONCEPT_H
