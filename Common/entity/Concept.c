/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/error.h>
//#include "entity/powers.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/textparser.h> // for TokenizerParseInfo
#include "entity/Concept.h"
#include "entity/character_inventory.h"
//#include "entity/boost.h"
#include <utilitieslib/components/earray.h>

#include "entity/origins.h"
#include "entity/powers.h"
#include "entity/boost.h"
#include "entity/character_base.h"
#include <utilitieslib/components/MemoryPool.h>

// ------------------------------------------------------------
// globals

STATIC_ASSERT(IS_GENERICINVENTORYTYPE_COMPATIBLE(ConceptDef,id,name));
STATIC_ASSERT(IS_GENERICINVENTORYDICT_COMPATIBLE(ConceptDictionary,ppConceptDefs,haItemNames,itemsById))

SHARED_MEMORY ConceptDictionary g_ConceptDict;

static StaticDefineInt UiFlagsAttribModGroupEnum [] =
{
    DEFINE_INT
    {"ShowAttribmod",kUiConceptAttribmodGroupFlags_ShowAttribmodGroup}, 
    {"ShowRange",kUiConceptAttribmodGroupFlags_ShowRange}, 
    DEFINE_END
};

static StaticDefineInt PECreationRewardTypeEnum [] =
{
    DEFINE_INT
    { "ChangeHardenLevel", kPECreationReward_ChangeHardenLevel},
    { "ChangeSellback", kPECreationReward_ChangeSellback},
    { "SpawnAmbush", kPECreationReward_SpawnAmbush},
    { "WisdomMultiplier", kPECreationReward_WisdomMultiplier},
    { "PrefillPowerupCost", kPECreationReward_PrefillPowerupCost},
    DEFINE_END
};

static StaticDefineInt HardenedAttribsTargetEnum[] = 
{
    DEFINE_INT
    {"AllPrev", kHardenedAttribsTarget_AllPrev},
    {"Prev", kHardenedAttribsTarget_Prev},
    {"Next", kHardenedAttribsTarget_Next},
    {"AllNext", kHardenedAttribsTarget_AllNext},
    DEFINE_END
};

static StaticDefineInt ConceptHardeningRewardTypeEnum[] =
{
    DEFINE_INT
    { "Reward", kConceptHardeningRewardType_Reward},
    { "ChangeNextHardenedAttribs", kConceptHardeningRewardType_ChangeHardenedAttribs},
    { "ChangeHardened", kConceptHardeningRewardType_ReducePowerupCost},
    DEFINE_END
};

static StaticDefineInt SlottedConceptAffectingRewardTypeEnum[] =
{
    DEFINE_INT
    { "IncreaseHardeningChance", kSlottedConceptAffectingRewardType_IncreaseHardeningChance},
    DEFINE_END
};

static StaticDefineInt HardenedAffectingRewardTypeEnum[] =
{
    DEFINE_INT
    { "EmptySlot", kHardenedAffectingRewardType_EmptySlot},
    DEFINE_END
};

//------------------------------------------------------------
//
//----------------------------------------------------------
static TokenizerParseInfo ParsePECreationRewardDef[] = 
{
     { "Type",        TOK_STRUCTPARAM | TOK_INT(PECreationRewardDef, type, kPECreationReward_Count), PECreationRewardTypeEnum },
     { "Param",        TOK_STRUCTPARAM | TOK_STRING(PECreationRewardDef, param, 0) } ,
    {    "\n",        TOK_END,                            0},
    { "", 0,0},
};

//------------------------------------------------------------
//  done
//----------------------------------------------------------
static TokenizerParseInfo ParseConceptHardeningRewardDef[] = 
{
     { "Type",        TOK_STRUCTPARAM | TOK_INT(ConceptHardeningRewardDef, type, kConceptHardeningRewardType_Count), ConceptHardeningRewardTypeEnum },
     { "Param0",     TOK_STRUCTPARAM | TOK_STRING(ConceptHardeningRewardDef, param0, 0) } ,
     { "Param1",     TOK_STRUCTPARAM | TOK_STRING(ConceptHardeningRewardDef, param1, 0) } ,
    {    "\n",        TOK_END,                            0},
    { "", 0,0},
};


//------------------------------------------------------------
//  done
//----------------------------------------------------------
static TokenizerParseInfo ParseSlottedConceptAffectingRewardDef[] = 
{
     { "Type",        TOK_STRUCTPARAM | TOK_INT(SlottedConceptAffectingRewardDef, type, kSlottedConceptAffectingRewardType_Count), SlottedConceptAffectingRewardTypeEnum },
     { "Param",      TOK_STRUCTPARAM | TOK_STRING(SlottedConceptAffectingRewardDef, param, 0) } ,
    {    "\n",        TOK_END,                            0},
    { "", 0,0},
};


//------------------------------------------------------------
//  done
//----------------------------------------------------------
static TokenizerParseInfo ParseHardenedConceptAffectingRewardDef[] = 
{
     { "Type",        TOK_STRUCTPARAM | TOK_INT(HardenedConceptAffectingRewardDef, type, kConceptHardeningRewardType_Count), HardenedAffectingRewardTypeEnum },
     { "Param",      TOK_STRUCTPARAM | TOK_STRING(HardenedConceptAffectingRewardDef, param, 0) } ,
    {    "\n",        TOK_END,                            0},
    { "", 0,0},
};



//------------------------------------------------------------
//  
//----------------------------------------------------------
static TokenizerParseInfo ParseAttribModGroupDef[] = 
{
    { "Name",        TOK_STRUCTPARAM | TOK_STRING(AttribModGroup, name, 0) },
    { "Min",        TOK_STRUCTPARAM | TOK_F32(AttribModGroup, min, 0) },
    { "Max",        TOK_STRUCTPARAM | TOK_F32(AttribModGroup, max, 0) },
     { "uiFlags",    TOK_STRUCTPARAM | TOK_INT(AttribModGroup, uiFlags, kUiConceptAttribmodGroupFlags_ALL), UiFlagsAttribModGroupEnum },
    {    "\n",        TOK_END,                            0},
    { "", 0,0},
};

static TokenizerParseInfo ParseConceptDef[] = 
{
    { "{",              TOK_START,                           0},
    { "Name",           TOK_STRING(ConceptDef, name, 0)         },
    TOKENIZERUIWIDGET_INLINEPARSEINFO(ConceptDef),
    { "AttribModGroup", TOK_STRUCT(ConceptDef, attribMods, ParseAttribModGroupDef)},
    { "PowerupSlotCost", TOK_STRUCT(ConceptDef, powerupCostSlots, ParseRewardSlot)} ,
     { "SellBack",        TOK_INT(ConceptDef,modSellback, 0)},
     { "Endurance",      TOK_F32(ConceptDef,modEndurance, 0) },
    { "RewardTable",    TOK_STRINGARRAY(ConceptDef, slotRewardTables) },
     { "Chance",            TOK_F32(ConceptDef,slotSuccessChance, 0)},
     { "SlotUsage",      TOK_INT(ConceptDef,slotsUsed, 1) }, //default to 1 slot used

    // rewards
    { "PECreationReward", TOK_STRUCT(ConceptDef, peCreationRewards, ParsePECreationRewardDef)},
    { "ConceptHardenReward", TOK_STRUCT(ConceptDef, hardeningRewards, ParseConceptHardeningRewardDef)},
    { "SlottedConceptAffectingReward", TOK_STRUCT(ConceptDef, slottedAffectingRewards, ParseSlottedConceptAffectingRewardDef)},    
    { "HardenedAffectingReward", TOK_STRUCT(ConceptDef, hardenedAffectingRewards, ParseHardenedConceptAffectingRewardDef)},    
    
    { "}",                TOK_END,        0 },
    { "", 0, 0 }
};


//------------------------------------------------------------
// parent of the concept
// -AB: created :2005 Feb 10 05:33 PM
//----------------------------------------------------------
TokenizerParseInfo ParseConceptDictionary[] =
{
    { "Concept", TOK_STRUCT(ConceptDictionary, ppConceptDefs, ParseConceptDef)},
    { "", 0, 0 }
};


bool conceptdict_FinalProcess(ParseTable pti[], ConceptDictionary *pdict, bool shared_memory)
{
    if( verify( pdict ))
    {
        int i;
        
        genericinvdict_CreateHashes((GenericInvDictionary*)pdict, inventorytype_GetAttribs(kInventoryType_Concept), shared_memory);

        // --------------------
        // check that the dict is valid

        for( i = eaSize(&pdict->ppConceptDefs) - 1; i >= 0; --i)
        {
            ConceptDef *ci = (ConceptDef*)pdict->ppConceptDefs[i];
            int as = eaSize(&ci->powerupCostSlots);
            if( as > ci->slotsUsed)
            {
                Errorf("Concept %s has more powerupcost than slots (%d>%d). illegal.",ci->name,as,ci->slotsUsed);
                ci->slotsUsed = as;
            }
            if( eaSize(&ci->attribMods) > TYPE_ARRAY_SIZE( ConceptItem, afVars ))
            {
                Errorf("Concept %s has more attribmods than allowed.",ci->name);
            }
        }
    }

    return true;
}

TokenizerParseInfo* conceptdef_GetParseInfo()
{
    return ParseConceptDictionary;
}

//------------------------------------------------------------
//  is the id valid
//----------------------------------------------------------
bool conceptdef_ValidId(int id)
{
    return id > 0 && EAINRANGE( id, g_ConceptDict.itemsById );
}

const ConceptDef* conceptdef_GetById(int id)
{
    if( conceptdef_ValidId(id) && g_ConceptDict.itemsById[id] )
    {
        assert(g_ConceptDict.itemsById[id]->id == id);
        return g_ConceptDict.itemsById[id];
    }
    return NULL;
}

const ConceptDef* conceptdef_Get(char const *name)
{
    return (const ConceptDef*)stashFindPointerReturnPointer( g_ConceptDict.haItemNames, name);
}


//------------------------------------------------------------
//  Get the attribmod group for a given def
//----------------------------------------------------------
int conceptdef_GetAttribModGroupIdx(const ConceptDef *def, char const *name)
{
    int res = -1;
    
    if( verify( def ))
    {
        int i;
        for( i = eaSize(&def->attribMods) - 1; i >= 0; --i)
        {
            if( def->attribMods[i] && 0 == stricmp(name,def->attribMods[i]->name))
            {
                res = i;
                break;
            }
        }
    }

    // --------------------
    // finally
    
    return res;
}


//------------------------------------------------------------
//  get the max id for a the concept set
//----------------------------------------------------------
int conceptdef_MaxId()
{
    int res = 0;

    if( verify( g_ConceptDict.itemsById ))
    {
        res = eaSize(&g_ConceptDict.itemsById) - 1;
    }

    // --------------------
    // finally, return
    
    return res;
}

// ------------------------------------------------------------
// ConceptItem functions

MP_DEFINE(ConceptItem);
ConceptItem* conceptitem_Create( int defId, F32 *afVars )
{
    const ConceptDef *def = conceptdef_GetById(defId);
    ConceptItem *res = NULL;

    if( verify( def && afVars ))
    {

        // create the pool, arbitrary number
        MP_CREATE(ConceptItem, 64);
        res = MP_ALLOC( ConceptItem );
        if( verify( res ))
        {
            res->def = def;
            CopyStructs(res->afVars,afVars,ARRAY_SIZE(res->afVars));
        }
    }
    // --------------------
    // finally, return

    return res;
}

void conceptitem_Destroy( ConceptItem *item )
{
    MP_FREE(ConceptItem, item);
}

//------------------------------------------------------------
//  Create a concept item
//----------------------------------------------------------
ConceptItem* conceptitem_Init(ConceptItem *item, int defId, F32 *afVars )
{
    if( !item )
    {
        return conceptitem_Create( defId, afVars );
    }
    else
    {
        const ConceptDef *def = conceptdef_GetById( defId );
        ConceptItem *res = NULL;

        if( verify( def && afVars ) )
        {
            ZeroStruct(item);
            item->def = def;
            CopyStructs( item->afVars, afVars, ARRAY_SIZE(item->afVars));
            res = item;
        }
        return res;
    }
}

// ------------------------------------------------------------
// boost related concept functions

static const PowerVar *FindPowerVar( const PowerVar * const * const * hVars, char const *name )
{
    const PowerVar *res = NULL;
    if( verify( hVars && name ))
    {
        int i;
        for( i = eaSize( hVars ) - 1; i >= 0 && !res; --i)
        {
            const PowerVar *pv = (*hVars)[i];
            if (pv && 0 == stricmp( pv->pchName, name ) )
                res = pv;
        }
    }
    
    //     --------------------
    // finally, return
    
    return res; 
}


//------------------------------------------------------------
//  test if the concept can be slotted
//----------------------------------------------------------
bool basepower_CanApplyConcept( const BasePower *recipe, ConceptDef const *concept )
{
    bool res = false;
    
    if( verify( recipe && concept ))
    {
        int i;
        res = true;
        for( i = eaSize( &concept->attribMods ) - 1; i >= 0 && res; --i)
        {
            const AttribModGroup *rp = concept->attribMods[i];                
            res = rp && FindPowerVar( &recipe->ppVars, rp->name );
        }
    }

    // --------------------
    // finally, return

    return res; 
}


//------------------------------------------------------------
// Slot a concept in the resulting boost.
//----------------------------------------------------------
Boost* boost_ApplyConcept( Boost *resBoost, const BasePower *recipe, ConceptItem const *concept )
{    
    Boost *res = NULL;
    if( verify( resBoost && recipe && concept ))
    {    
        // --------------------
        // if valid, apply the attribs
        
        if( basepower_CanApplyConcept(recipe,concept->def) )
        {    
            int i;
            int size = MIN(eaSize(&concept->def->attribMods),ARRAY_SIZE(concept->afVars));
            
            for( i = 0; i < size; ++i )
            {
                // match the var this concept affects with the var in the recipe
                 const AttribModGroup *am = concept->def->attribMods[i];
                 PowerVar const *var = FindPowerVar( &recipe->ppVars, am->name );
                
                if( verify( am && var && INRANGE0( var->iIndex, ARRAY_SIZE( resBoost->afVars ) ) ))
                {
                    resBoost->afVars[ var->iIndex ] += concept->afVars[i];
                }
            }

            // accum the powerup cost for this concept
            for( i = eaSize( &concept->def->powerupCostSlots ) - 1; i >= 0; --i)
            {
                if( concept->def->powerupCostSlots[i] )
                    boost_AddPowerupslot( resBoost, concept->def->powerupCostSlots[i]);
            }
            
        }
        // set the result
        res = resBoost;
    }


    //     --------------------
    // finally, return

    return res; 
}
