/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/memlog.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/textparser.h>
#include "entity/entity.h"
#include <utilitieslib/components/MemoryPool.h>
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_inventory.h"
#include "entity/salvage.h"
#include "entity/Concept.h"
#include "entity/powers.h"
#include "bases/basedata.h"
#include "bases/DetailRecipe.h"
#include <utilitieslib/components/StashTable.h>
#include "entity/LoadDefCommon.h"
#include "gameData/mininghelper.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/log.h>
#include <utilitieslib/components/SharedHeap.h>

#if SERVER
#include "cmdparse/cmdserver.h"
#include "container/containerInventory.h"
#include "dbcomm/logcomm.h"
#include "dbcomm/dbcontainer.h"
#include "Reward.h"
#include "entity/entPlayer.h"
#include "entity/entGameActions.h"
#include "svr/svr_base.h"
#include "comm_game.h"

#elif CLIENT
#include "cmdparse/cmdgame.h"
#include "UI/uiRecipeInventory.h"
#include "UI/uiIncarnate.h"
#include "UI/uiOptions.h"
#include "UI/uiSalvage.h"
#include "UI/uiStoredSalvage.h"
#include "UI/uiTrade.h"
#endif

// ------------------------------------------------------------
// statics

static SHARED_MEMORY AttribFileDict s_invAttribFiles[kInventoryType_Count] = {0} ;

static const GenericInventoryType* const* * GetItemsByIdArrayRef( InventoryType type );


// ------------------------------------------------------------
// inventory size definitions

TokenizerParseInfo ParseInventoryDefinition[] =
{
    { "{",                TOK_START,  0 },
    { "AmountAtLevel",    TOK_INTARRAY(InventorySizeDefinition, piAmountAtLevel)     },
    { "AmountAtLevelFree",    TOK_INTARRAY(InventorySizeDefinition, piAmountAtLevelFree)     },
    { "}",                TOK_END,    0 },
    { "", 0, 0 }
};

TokenizerParseInfo ParseInventoryDefinitions[] =
{
    { "{",                TOK_START,  0 },
    { "Salvage",        TOK_EMBEDDEDSTRUCT(InventorySizes, salvageSizes, ParseInventoryDefinition)},
    { "Recipe",            TOK_EMBEDDEDSTRUCT(InventorySizes, recipeSizes, ParseInventoryDefinition)},
    { "Auction",        TOK_EMBEDDEDSTRUCT(InventorySizes, auctionSizes, ParseInventoryDefinition)},
    { "StoredSalvage",        TOK_EMBEDDEDSTRUCT(InventorySizes, storedSalvageSizes, ParseInventoryDefinition)},
    { "}",                TOK_END,    0 },
    { "", 0, 0 }
};

SHARED_MEMORY InventorySizes g_InventorySizeDefs;

TokenizerParseInfo ParseInventoryLoyaltyBonusDefinition[] =
{
    { "{",                    TOK_START,  0 },
    { "Name",                TOK_STRING(InventoryLoyaltyBonusSizeDefinition, name, 0)     },
    { "SalvageBonusSlots",    TOK_INT(InventoryLoyaltyBonusSizeDefinition, salvageBonus, 0)     },
    { "RecipeBonusSlots",    TOK_INT(InventoryLoyaltyBonusSizeDefinition, recipeBonus, 0)     },
    { "VaultBonusSlots",    TOK_INT(InventoryLoyaltyBonusSizeDefinition, vaultBonus, 0)     },
    { "AuctionBonusSlots",    TOK_INT(InventoryLoyaltyBonusSizeDefinition, auctionBonus, 0)     },
    { "}",                    TOK_END,    0 },
    { 0 }
};

TokenizerParseInfo ParseInventoryLoyaltyBonusDefinitions[] =
{
    { "LoyaltyBonus",        TOK_STRUCT(InventoryLoyaltyBonusSizes, bonusList, ParseInventoryLoyaltyBonusDefinition)},
    { "", 0, 0 }
};

SHARED_MEMORY InventoryLoyaltyBonusSizes g_InventoryLoyaltyBonusSizeDefs;


// ------------------------------------------------------------
// dictionary functions


//------------------------------------------------------------
// make the .def filename. uses a temporary static buffer.
//----------------------------------------------------------
char * inventorytype_MakeDefFilename( InventoryType type )
{
    char const *tablename = inventory_GetDbTableName(type);
    static char s_tmp[MAX_PATH];
    if( tablename )
    {
        sprintf(s_tmp,"defs/%s.def", tablename);
        return s_tmp;
    }
    return NULL;
}


//------------------------------------------------------------
//  Copy character inventory from one to another
//----------------------------------------------------------
#if SERVER
void character_CopyInventory(Character *pcharOrig, Character *pcharNew)
{
    int i, count;

    // copying inventory information
    pcharNew->iLevel                          = pcharOrig->iLevel;
    pcharNew->iCurBuild                      = pcharOrig->iCurBuild;
    pcharNew->iActiveBuild                    = pcharOrig->iActiveBuild;
    memcpy(pcharNew->iBuildLevels, pcharOrig->iBuildLevels, sizeof(pcharNew->iBuildLevels));

    pcharNew->salvageInvBonusSlots            = pcharOrig->salvageInvBonusSlots;
    pcharNew->salvageInvTotalSlots            = pcharOrig->salvageInvTotalSlots;
    pcharNew->salvageInvCurrentCount        = pcharOrig->salvageInvCurrentCount;
    pcharNew->storedSalvageInvBonusSlots    = pcharOrig->storedSalvageInvBonusSlots;
    pcharNew->storedSalvageInvTotalSlots    = pcharOrig->storedSalvageInvTotalSlots;
    pcharNew->storedSalvageInvCurrentCount    = pcharOrig->storedSalvageInvCurrentCount;
    pcharNew->auctionInvBonusSlots            = pcharOrig->auctionInvBonusSlots;
    pcharNew->auctionInvTotalSlots            = pcharOrig->auctionInvTotalSlots;
    pcharNew->recipeInvBonusSlots            = pcharOrig->recipeInvBonusSlots;
    pcharNew->recipeInvTotalSlots            = pcharOrig->recipeInvTotalSlots;
    pcharNew->recipeInvCurrentCount            = pcharOrig->recipeInvCurrentCount;

    count = eaSize(&pcharOrig->salvageInv);
    for(i=0; i<count; i++)
    {
        if (!pcharOrig->salvageInv[i] || !pcharOrig->salvageInv[i]->salvage)
            continue;
        character_AdjustInventoryEx(pcharNew, kInventoryType_Salvage, pcharOrig->salvageInv[i]->salvage->salId, pcharOrig->salvageInv[i]->amount, NULL, true, false );
    }
    character_SetSalvageInvCurrentCount(pcharNew);

    count = eaSize(&pcharOrig->storedSalvageInv);
    for(i=0; i<count; i++)
    {
        if (!pcharOrig->storedSalvageInv[i] || !pcharOrig->storedSalvageInv[i]->salvage)
            continue;
        character_AdjustInventoryEx(pcharNew, kInventoryType_StoredSalvage, pcharOrig->storedSalvageInv[i]->salvage->salId, pcharOrig->storedSalvageInv[i]->amount, NULL, true, false );
    }
    character_SetStoredSalvageInvCurrentCount(pcharNew);

    count = eaSize(&pcharOrig->detailInv);
    for(i=0; i<count; i++)
    {
        if (!pcharOrig->detailInv[i] || !pcharOrig->detailInv[i]->item)
            continue;
        character_AdjustInventoryEx(pcharNew, kInventoryType_BaseDetail, pcharOrig->detailInv[i]->item->id, pcharOrig->detailInv[i]->amount, NULL, true, false );
    }

    count = eaSize(&pcharOrig->recipeInv);
    for(i=0; i<count; i++)
    {
        if (!pcharOrig->recipeInv[i] || !pcharOrig->recipeInv[i]->recipe)
            continue;
        character_AdjustInventoryEx(pcharNew, kInventoryType_Recipe, pcharOrig->recipeInv[i]->recipe->id, pcharOrig->recipeInv[i]->amount, NULL, true, false );
    }
    character_SetRecipeInvCurrentCount(pcharNew);

    count = eaSize(&pcharOrig->conceptInv);
    for(i=0; i<count; i++)
    {
        if (!pcharOrig->conceptInv[i] || !pcharOrig->conceptInv[i]->concept)
            continue;
        character_AdjustConcept( pcharNew, pcharOrig->conceptInv[i]->concept->def->id, pcharOrig->conceptInv[i]->concept->afVars, pcharOrig->conceptInv[i]->amount );
    }
}
#endif

//------------------------------------------------------------
//  Helper for if you are allowed to add inventory
//----------------------------------------------------------
bool character_InventoryFull(Character *pchar, InventoryType type)
{
    bool res = false;
    switch ( type )
    {
    case kInventoryType_Salvage:
        res = pchar->salvageInvCurrentCount >= character_GetInvTotalSize(pchar, type);
        break;
    case kInventoryType_Concept:
        // concepts are the only ones with a hard limit
        res = eaSize(&pchar->conceptInv) >= CONCEPT_MAX_PER_ROW*CONCEPT_MAX_ROWS;
        break;
    case kInventoryType_Recipe:
        res = pchar->recipeInvCurrentCount >= character_GetInvTotalSize(pchar, type);
        break;
    case kInventoryType_BaseDetail:
        break;
    case kInventoryType_StoredSalvage:
        res = pchar->storedSalvageInvCurrentCount >= character_GetInvTotalSize(pchar, type);
        break;
    default:
        verify(0 && "invalid switch value");
    };
    STATIC_INFUNC_ASSERT(kInventoryType_Count == 5);
    return res;
}

int character_InventoryEmptySlots(Character *pchar, InventoryType type)
{
    switch ( type )
    {
    case kInventoryType_Salvage:
        return character_GetInvTotalSize(pchar, type) - pchar->salvageInvCurrentCount;

    case kInventoryType_Concept:
        return CONCEPT_MAX_PER_ROW*CONCEPT_MAX_ROWS - eaSize(&pchar->conceptInv);

    case kInventoryType_Recipe:
        return character_GetInvTotalSize(pchar, type) - pchar->recipeInvCurrentCount;

    case kInventoryType_BaseDetail:
        return 0;

    case kInventoryType_StoredSalvage:
        return character_GetInvTotalSize(pchar,type) - pchar->storedSalvageInvCurrentCount;

    default:
        verify(0 && "invalid switch value");
        return 0;
    };
}

int character_InventoryEmptySlotsById(Character *p, InventoryType type, int id)
{
    if( type == kInventoryType_Salvage )
        return character_SalvageEmptySlots(p, salvage_GetItemById(id));
    else if( type == kInventoryType_StoredSalvage)
        return character_StoredSalvageEmptySlots(p, salvage_GetItemById(id));
    else // no special checking
        return character_InventoryEmptySlots(p,type);
}

int character_InventoryUsedSlots(Character *pchar, InventoryType type)
{
    switch ( type )
    {
    case kInventoryType_Salvage:
        return pchar->salvageInvCurrentCount;
    case kInventoryType_Concept:
        return eaSize(&pchar->conceptInv);
    case kInventoryType_Recipe:
        return pchar->recipeInvCurrentCount;
    case kInventoryType_BaseDetail:
        return 0;
    case kInventoryType_StoredSalvage:
        return pchar->storedSalvageInvCurrentCount;
    default:
        verify(0 && "invalid switch value");
        return 0;
    };
}

static void s_GetInvAndItem(InventoryType type, GenericInvItem ****phInv, void **phItem, Character *p, int id);

//------------------------------------------------------------
//  get the amount in the players inventory of an item of a specific type/id
//----------------------------------------------------------
int character_GetItemAmount(Character *p, InventoryType type, int id)
{
    int res = -1;

    if( verify( p && inventorytype_Valid(type) ))
    {
        if( type == kInventoryType_Salvage )
        {
            const SalvageItem *pSalvage = salvage_GetItemById(id);
            if ( pSalvage )
                res = character_SalvageCount(p, pSalvage);
            else
                res = 0;
        }
        else if( type == kInventoryType_Concept )
        {
            assertmsg(0,"not handled here.");
        }
        else if( type == kInventoryType_StoredSalvage)
        {
            const SalvageItem *pSalvage = salvage_GetItemById(id);
            if ( pSalvage )
                res = character_StoredSalvageCount(p, salvage_GetItemById(id));
            else 
                res = 0;
        }
        else
        {
            GenericInvItem ***hInv = NULL;
            void *item = NULL;

            // get the item
            s_GetInvAndItem( type, &hInv, &item, p, id );

            // if it was found
            if( devassert( hInv && item ))
            {
                int i;
                res = 0;

                // find the item in the inventory
                for( i = eaSize( hInv ) - 1; i >= 0 && res < 1; --i)
                {
                    if( (*hInv)[i]->item == item )
                    {
                        res = (*hInv)[i]->amount;
                        break;
                    }
                }
            }
        }
    }

    return res;
}


MP_DEFINE(GenericInvItem);
GenericInvItem* genericinvitem_Create( const void *item )
{
    GenericInvItem *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(GenericInvItem, 16);
    res = MP_ALLOC( GenericInvItem );
    if( verify( res ))
    {
        res->item= item;
        res->amount = 0;
    }
    // --------------------
    // finally, return

    return res;
}

void genericinvitem_Destroy(GenericInvItem *item)
{
    MP_FREE(GenericInvItem,item);
}

static MemLog sallog_ml = {0};
static void salLog(const char *fmt,...)
{

    va_list ap;
    va_start(ap, fmt);
    memlog_printfv(&sallog_ml,fmt,ap);
    va_end(ap);
}


void character_InventoryFree(Character *pchar)
{
    if (!pchar)
        return;
    if (pchar->salvageInv)
    {
        salLog("%s: %i items",__FUNCTION__,eaSize(&pchar->salvageInv));
        eaDestroyEx(&pchar->salvageInv,genericinvitem_Destroy);
        pchar->salvageInv = NULL;
    }
    if (pchar->recipeInv)
    {
        eaDestroyEx(&pchar->recipeInv,genericinvitem_Destroy);
        pchar->recipeInv = NULL;
    }
    if (pchar->conceptInv)
    {
        eaDestroyEx(&pchar->conceptInv,genericinvitem_Destroy);
        pchar->conceptInv = NULL;
    }
    if (pchar->storedSalvageInv)
    {
        salLog("%s: %i stored items",__FUNCTION__, eaSize(&pchar->storedSalvageInv));
        eaDestroyEx(&pchar->storedSalvageInv, genericinvitem_Destroy);
        pchar->storedSalvageInv = NULL;
    }
}


DetailInventoryItem *detailinventoryitem_Create()
{
    return (DetailInventoryItem*)genericinvitem_Create(NULL);
}

// ------------------------------------------------------------
// generic inventory

//------------------------------------------------------------
//  helper for getting the earray of an inventory
//----------------------------------------------------------
static GenericInvItem ***character_GetInvHandleByType(Character *p, InventoryType t)
{
    GenericInvItem ***res = NULL;

    switch ( t )
    {
    xcase kInventoryType_Salvage:
        res = cpp_static_cast(GenericInvItem***)(&p->salvageInv);
    xcase kInventoryType_Concept:
        res = cpp_static_cast(GenericInvItem***)(&p->conceptInv);
    xcase kInventoryType_Recipe:
        res = cpp_static_cast(GenericInvItem***)(&p->recipeInv);
    xcase kInventoryType_BaseDetail:
        res = cpp_static_cast(GenericInvItem***)(&p->detailInv);
    xcase kInventoryType_StoredSalvage:
        res = cpp_static_cast(GenericInvItem***)(&p->storedSalvageInv);
    xdefault:
        assertmsg(0,"unhandled type");
    };

    // --------------------
    // finally, return

    return (GenericInvItem***)res;
}


//------------------------------------------------------------
//  Set the salvage to the given slot in its rarity.
//  rarity: if < 0 then get rarity from the salvage item
//  idx : the idx into the inventory array to insert the item
//  -AB: created :2005 Feb 23 05:36 PM
//  -AB: added rarity as a param for NULL salvages :2005 Feb 25 02:18 PM
//----------------------------------------------------------
static bool s_PrepInvToSetItem(Character *p, GenericInvItem ***invRef, InventoryType type, int idx)
{
    bool res = false;


    // arbitrary number for idx, I just don't think we'll ever hold that many
    if( verify( p && invRef && inventorytype_Valid(type)
        ))
    {
        // create the array, if none
        if( !*invRef )
        {
            eaCreate( invRef );
        }

        if( idx >= eaSizeUnsafe( invRef ) )
        {
            // ----------
            // need to expand the array to this size

            // double the capacity for linear growth cost
            if( eaCapacity( invRef ) <= idx )
                eaSetCapacity( invRef, (idx+1)*2 );

            // set the size.
            eaSetSize( invRef, (idx + 1) );
        }

        // prep the status change
#if SERVER
        eaiPush( &p->invStatusChange.type, type);
        eaiPush( &p->invStatusChange.idx, idx);
#endif
        res = true;
    }
    // --------------------
    // finally

    return res;
}

//------------------------------------------------------------
//  set the inventory at the specified index to the specified amount
//----------------------------------------------------------
static void genericinv_SetItem( Character *p, GenericInvItem ***hInv, InventoryType type, const void *item, int idx, int amount, char const *context)

{
    int id = -1;            // tracks recipe id to refresh windows correctly

    if( verify( hInv && s_PrepInvToSetItem(p, hInv, type, idx )))
    {
        // create, if necessary
        if( (*hInv)[idx] == NULL )
        {
            // note -AB: this is a dangerous part here if any kooky typecasting is going on :2005 Mar 17 05:09 PM
            (*hInv)[idx] = genericinvitem_Create( item );
        }

        switch (type)
        {
        case kInventoryType_Salvage:
        {
            const SalvageItem * sal = (const SalvageItem *) item;
            if (salvageitem_IsInvention(sal))
            {
                p->salvageInvCurrentCount += amount - (*hInv)[idx]->amount;
                salLog("%s->%s: setting %s to %i",context,__FUNCTION__,sal->pchName,amount);
            }
        }
        break;
        case kInventoryType_Recipe:
            p->recipeInvCurrentCount += amount - (*hInv)[idx]->amount;
            if (item != NULL)
            {
                id = ((DetailRecipe *) item)->id;
            } else if ((*hInv)[idx]->item != NULL) {
                id = ((DetailRecipe *) (*hInv)[idx]->item)->id;
            }
            break;
        case kInventoryType_StoredSalvage:
            p->storedSalvageInvCurrentCount += amount - (*hInv)[idx]->amount;
            break;
        default:
            break;    
        }

        // now that we have room, set the item
        (*hInv)[idx]->item = item;
        (*hInv)[idx]->amount = amount;
        if (amount <= 0) {
            (*hInv)[idx]->item = NULL;
            (*hInv)[idx]->amount = 0;
        }

        // on change triggers

        if (type == kInventoryType_Salvage)
        {
#if CLIENT && !TEST_CLIENT
            if (amount > 0)
                optionSet(kUO_ShowSalvage, 1, 1);
            salvageReparse();
            recipeInventoryReparse();
            uiIncarnateReparse();
            if (p && p->entParent)
                uiTrade_UpdateSalvageInventoryCount(p->entParent, idx);
#elif SERVER
            if (amount > 0 && p->entParent && p->entParent->pl)
                p->entParent->pl->showSalvage = true;
#endif
        } else if (type == kInventoryType_Recipe) {
#if CLIENT && !TEST_CLIENT
            recipeInventoryReparse();
            uiIncarnateReparse();
            if (p && p->entParent && id >= 0)
                uiTrade_UpdateRecipeInventoryCount(p->entParent, idx, id);
#endif
        }
        else if( type == kInventoryType_StoredSalvage)
        {
#if CLIENT && !TEST_CLIENT
            storedSalvageReparse();
#endif
        }
    }
}



//------------------------------------------------------------
//  generic function for adding an inventory item. checking if it
// is in the list already.
//----------------------------------------------------------
static int genericinv_AdjustItem(Character *p, GenericInvItem ***hInv, InventoryType type, const void *item, int amount, char const *context )
{
    int res = -1;
    if( verify( hInv && item))
    {
        // find the item in the inventory
        int i;
        int i_null = -1;

//        if (character_InventoryFull(p, type) && amount > 0)
//            return -1; //Adding when too many items

        for( i = eaSize( hInv ) - 1; i >= 0 && res < 1; --i)
        {
            if( (*hInv)[i]->item == NULL && i_null < 0 )
            {
                i_null = i;
            }
            if( (*hInv)[i]->item == item )
            {
                res = i;
            }
        }

        // set res to the correct index
        if( res < 0 )
        {
            if (amount < 0)
                return -1; //try to remove nonexistent item
            res = i_null > 0 ? i_null : eaSize( hInv );
        }

        // increment the amount
        if( res >= 0 )
        {
            genericinv_SetItem( p, hInv, type, item, res,
                                (EAINRANGE( res, *hInv ) && verify((*hInv)[res]) ? (*hInv)[res]->amount + amount : amount),
                                context );
        }
    }

    return res;
}

//------------------------------------------------------------
//  get the inventory list and the item to put there
//----------------------------------------------------------
static void s_GetInvAndItem(InventoryType type, GenericInvItem ****phInv, const void **phItem, Character *p, int id)
{
    if( verify( phInv && phItem && p ))
    {
        const GenericInventoryType* const* * hItems = GetItemsByIdArrayRef(type);
        *phInv = NULL;
        *phItem = NULL;

        // get the generic
        if( verify( hItems && EAINRANGE( id, *hItems )))
        {
            *phItem = (*hItems)[id];
        }

        *phInv = character_GetInvHandleByType(p, type);
    }
}

//------------------------------------------------------------
//  add inventory in a generic way based on type
// returns -1 if fails, the index where it is inserted otherwise
//----------------------------------------------------------
int character_AdjustInventoryEx(Character *p, InventoryType type, int id, int amount, const char *context, int bNoInvWarning, int autoOpenSalvage)
{
    int res = -1;

    if( verify( p && inventorytype_Valid(type) ))
    {
        if( type == kInventoryType_Salvage )
            res = character_AdjustSalvageEx(p, salvage_GetItemById(id), amount, context, bNoInvWarning, autoOpenSalvage );
        else if( type == kInventoryType_Concept )
            assertmsg(0,"not handled here.");
        else if( type == kInventoryType_StoredSalvage)
            res = character_AdjustStoredSalvage(p, salvage_GetItemById(id), amount, context, bNoInvWarning );
        else
        {
            GenericInvItem ***hInv = NULL;
            const void *item = NULL;

            // get the item
            s_GetInvAndItem( type, &hInv, &item, p, id );

            // if it was found
            if( devassert( hInv && item ))
            {
                res = genericinv_AdjustItem( p, hInv, type, item, amount, context );
            }
        }
    }
    // --------------------
    // finally, return

    return res;
}

int character_AdjustInventory(Character *p, InventoryType type, int id, int amount, const char *context, int bNoInvWarning)
{
    return character_AdjustInventoryEx(p, type, id, amount, context, bNoInvWarning, true);
}

//------------------------------------------------------------
//  returns true if the character has room for the given type/amount
//----------------------------------------------------------
bool character_CanAddInventory(Character *p, InventoryType type, int id, int amount)
{
    if(devassert(amount > 0))
    {
        if( type == kInventoryType_Salvage )
            return character_CanAdjustSalvage(p, salvage_GetItemById(id), amount);
        else if( type == kInventoryType_StoredSalvage)
            return character_CanAdjustStoredSalvage(p, salvage_GetItemById(id), amount);
        else // no special checking
            return character_InventoryEmptySlots(p,type) >= amount;
    }
    return false;
}

//------------------------------------------------------------
//  set inventory in a generic way based on type
//----------------------------------------------------------
void character_SetInventory(Character *p, InventoryType type, int id, int idx, int amount, char const *context)
{
    if( verify( p ))
    {
        if( type == kInventoryType_Concept )
        {
            assertmsg(0,"this type is not handled here.");
        }
        else
        {
            GenericInvItem ***hInv = NULL;
            void *item = NULL;

            // get the item
            s_GetInvAndItem( type, &hInv, &item, p, id );

            // if it was found, or we're deleting an item
            if(hInv && (item || amount == 0))
                genericinv_SetItem( p, hInv, type, item, idx, amount, context );
        }
    }
}

void character_ClearInventory(Character *p, InventoryType type, char const *context) 
{
    static char *ctxt = NULL;
    int i;
    GenericInvItem ***hInv = character_GetInvHandleByType(p,type);
    if (verify(hInv))
    {
        estrPrintf(&ctxt,"%s=>character_ClearInventory",context);
        for( i = eaSize(hInv)-1; i >= 0; i-- )
        {
            if ((*hInv)[i]->item && (*hInv)[i]->amount > 0)
                character_SetInventory(p,type,((GenericInventoryType*)(*hInv)[i]->item)->id,i,0, context);
        }
    }
}
//------------------------------------------------------------
//  add the passed detail to inventory
//----------------------------------------------------------
int character_CountInventory(Character *p, InventoryType type)
{
    int i, count = 0;
    GenericInvItem ***hInv = character_GetInvHandleByType(p,type);
    if( !p || !hInv )
        return 0;

    for( i = eaSize(hInv)-1; i >= 0; i-- )
    {
        if( (*hInv)[i] && (*hInv)[i]->amount > 0 && (*hInv)[i]->item)
            count++;
    }

    return count;
}

static const GenericInventoryType* const* * GetItemsByIdArrayRef( InventoryType type )
{
    const GenericInventoryType* const* *res = NULL;
    if( verify( inventorytype_Valid(type) ))
    {
        switch ( type )
        {
        case kInventoryType_Salvage:            // fall thru
        case kInventoryType_StoredSalvage:
            res = cpp_reinterpret_cast(const GenericInventoryType* const* *)(&g_SalvageDict.itemsById);
            break;
        case kInventoryType_Concept:
            res = cpp_reinterpret_cast(const GenericInventoryType* const* *)(&g_ConceptDict.itemsById);
            break;
        case kInventoryType_Recipe:
            res = cpp_reinterpret_cast(const GenericInventoryType* const* *)(&g_DetailRecipeDict.itemsById);
            break;
        case kInventoryType_BaseDetail:
            res = (GenericInventoryType ***)&g_DetailDict.itemsById;
            break;
        default:
            assertmsg(0,"invalid.");
        };
        STATIC_INFUNC_ASSERT(kInventoryType_Count == 5);
    }
    // --------------------
    // finally, return

    return res;
}

int genericinv_GetInvIdFromItem(InventoryType type, void * ptr)
{
    int res = -1;
    if( verify( inventorytype_Valid(type) ))
    {
        switch ( type )
        {
        case kInventoryType_Salvage:
        case kInventoryType_StoredSalvage:
        {
            const SalvageItem *itm = (const SalvageItem*)ptr;
            if(itm)
                res = itm->salId;
        }
        break;
        case kInventoryType_Recipe:
        {
            const DetailRecipe *itm = (const DetailRecipe*)ptr;
            if(itm)
                res = itm->id;
        }
        break;
        default:
            assertmsg(0,"invalid.");
        };
        STATIC_INFUNC_ASSERT(kInventoryType_Count == 5);
    }
    return res;
}



// ------------------------------------------------------------
// salvage functions

bool salvageinventoryitem_Valid(SalvageInventoryItem *inv)
{
    return inv && inv->amount > 0 && inv->salvage;
}


bool salvageitem_IsInvention(SalvageItem const *item)
{
    if (item)
        return (item->type == kSalvageType_Invention);   // 

    return false;
}

// Be paranoid and only allow fully successful changes
bool character_CanAdjustSalvage(Character *p, SalvageItem const *pSalvageItem, int amount )
{
    if( verify(p && pSalvageItem ))
    {
        SalvageInventoryItem **salvages = p->salvageInv;
        int i;

        // check for overful inventory
        if(amount > 0 && salvageitem_IsInvention(pSalvageItem))
        {
            int totalSlots = character_GetInvTotalSize(p, kInventoryType_Salvage);
            if (totalSlots >= 0 && p->salvageInvCurrentCount + amount > totalSlots)
                return false;
        }

        for( i = eaSize( &salvages ) - 1; i >= 0; --i)
        {
            if( salvages[i]->salvage == pSalvageItem )
            {
                unsigned int newamount = salvages[i]->amount + amount;
                if (newamount < 0) return false; // We end up with less than 0, bad
                if (pSalvageItem->maxInvAmount == 0) return true; //0 means unlimited
                if (newamount <= pSalvageItem->maxInvAmount) return true;
                return false; //we end up with more than max, bad.
            }
        }
        if (amount < 0) return false; //don't remove item we dont have
        if (pSalvageItem->maxInvAmount == 0) return true;
        if ((unsigned int)amount > pSalvageItem->maxInvAmount) return false;
        return true;
    }
    return false;
}

int character_SalvageEmptySlots(Character *p, SalvageItem const *pSalvageItem)
{
    int empty_slots = INT_MAX;

    if(verify(p && pSalvageItem))
    {
        SalvageInventoryItem **salvages = p->salvageInv;
        int i;

        // check for overful inventory
        if(salvageitem_IsInvention(pSalvageItem))
        {
            int invention_empty_slots = character_GetInvTotalSize(p,kInventoryType_Salvage) - p->salvageInvCurrentCount;
            empty_slots = MIN(empty_slots,invention_empty_slots);
        }

        for( i = eaSize( &salvages ) - 1; i >= 0; --i)
        {
            if( salvages[i]->salvage == pSalvageItem )
            {
                if(pSalvageItem->maxInvAmount) //0 means unlimited
                {
                    int per_item_empty_slots = pSalvageItem->maxInvAmount - salvages[i]->amount;
                    empty_slots = MIN(empty_slots,per_item_empty_slots);
                }
                break;
            }
        }
        if(i < 0 && pSalvageItem->maxInvAmount) //0 means unlimited
        {
            int per_item_empty_slots = pSalvageItem->maxInvAmount;
            empty_slots = MIN(empty_slots,per_item_empty_slots);
        }
    }
    else
    {
        return 0;
    }

    return MAX(empty_slots,0);
}

// Allow partially successful actions
bool character_CanSalvageBeChanged(Character *p, SalvageItem const *pSalvageItem, int amount )
{
    if( verify(p && pSalvageItem ))
    {
        SalvageInventoryItem **salvages = p->salvageInv;
        int i;

        // check for overful inventory
        if(amount > 0 && salvageitem_IsInvention(pSalvageItem))
        {
            int totalSlots = character_GetInvTotalSize(p, kInventoryType_Salvage);
            if (totalSlots >= 0 && p->salvageInvCurrentCount + amount > totalSlots)
                return false;
        }

        for( i = eaSize( &salvages ) - 1; i >= 0; --i)
        {
            if( salvages[i]->salvage == pSalvageItem )
            { //we found an existing item
                if (amount > 0 && (salvages[i]->amount < pSalvageItem->maxInvAmount ||
                    pSalvageItem->maxInvAmount == 0))
                    return true; // if we can add at least 1, let it go and do it.
                if (amount < 0 && salvages[i]->amount > 0)
                    return true; // if we can subtract at least 1, let it go and do it.
                return false;
            }
        }

        if (amount < 0) 
            return false; //don't remove item we don't have
        
        return true;
    }
    return false;
}

bool character_SalvageIsFull(Character *p, SalvageItem const *pSalvageItem )
{
    return !character_CanAdjustSalvage(p, pSalvageItem, 1 ); // if I can add one, I'm not full
}

int character_SalvageCount(Character *p, SalvageItem const *pSalvageItem )
{
    if( verify(p && pSalvageItem ))
    {
        SalvageInventoryItem **salvages = p->salvageInv;
        int i;

        for( i = eaSize( &salvages ) - 1; i >= 0; --i)
        {
            if( salvages[i]->salvage == pSalvageItem )
                return salvages[i]->amount; 
        }
    }
    return 0;
}

static int s_FindSalvagePlace(SalvageInventoryItem **salvages, SalvageItem const *pSalvageItem)
{
    int i_NULL = -1;
    int res = -1;
    
    if( !salvages )
        return 0;
    
    for( res = 0; res < eaSize( &salvages ); ++res )
    {
        if( salvages[res]->salvage == NULL && i_NULL < 0)
            i_NULL = res;
        else if( salvages[res]->salvage == pSalvageItem )
            break; // BREAK IN FLOW
    }

    if( !EAINRANGE(res,salvages) && i_NULL >= 0 )
        res = i_NULL;
    return res;
}

//------------------------------------------------------------
//  give the character the passed salvage item, incrementing their
// inventory count
//  pSalvageItem : the salvage to add to. NOTE: this must be the
//    salvage item that is in the g_SalvageDict as pointer-comparison is used.
// @return int: the index in the inventory array where the item is stored.
//  -AB: created :2005 Feb 22 02:52 PM
//----------------------------------------------------------
int character_AdjustSalvageEx(Character *p, SalvageItem const *pSalvageItem, int amount, 
                            const char *context, int bNoInvWarning, int autoOpenSalvage )
{
    static char *ctxt = NULL;
    int res = -1;
    int count = 0;
    SalvageInventoryItem **salvages = p ? p->salvageInv : NULL;

    if(!pSalvageItem)
        return res;
    
    res = s_FindSalvagePlace(p->salvageInv,pSalvageItem);
            
    if(EAINRANGE(res,salvages) && salvages[res])
        count = salvages[res]->amount;


    
    if(amount > 0)
    {
        DATA_MINE_SALVAGE(pSalvageItem,"in",context,amount);
        
        if ( pSalvageItem->maxInvAmount != 0 && (U32)(count + amount) > pSalvageItem->maxInvAmount )
        {
            int actual_amount = pSalvageItem->maxInvAmount - count;
            DATA_MINE_SALVAGE(pSalvageItem,"out","inventory_full",amount - actual_amount);
            amount = actual_amount;
        }
    }
    else
    {
        if(-amount > count)
            amount = -count;
        if(amount)
            DATA_MINE_SALVAGE(pSalvageItem,"out",context,-amount);
    }

    // set the salvage and increment the amount
    estrPrintf(&ctxt,"%s->%s",context,__FUNCTION__);
    genericinv_SetItem(p, (GenericInvItem***)&p->salvageInv, kInventoryType_Salvage, pSalvageItem, res, amount + count, ctxt);

#ifdef SERVER
    if( !bNoInvWarning && amount > 0 
        && ((salvageitem_IsInvention(pSalvageItem) 
             && character_InventoryFull(p, kInventoryType_Salvage)) 
            || ((pSalvageItem->maxInvAmount>1) && (U32)count+amount>=pSalvageItem->maxInvAmount)
            ))
    {
        serveFloater(p->entParent, p->entParent, "FloatSalvageFull", 0.0f, kFloaterStyle_Attention, 0xff0000ff);
    }

    // if you want SALVAGE_AUTO_OPEN and SALVAGE_IMMEDIATE to work together
    // integrate the prerequisite checking from SALVAGE_IMMEDIATE into the primary
    // code flow for salvage opens, remove this limitation, and ensure that
    // the function called here checks prerequisites.
    if (p->entParent && autoOpenSalvage && amount > 0 && (pSalvageItem->flags & SALVAGE_AUTO_OPEN)
        && !(pSalvageItem->flags & SALVAGE_IMMEDIATE))
    {
        START_PACKET(pak, p->entParent, SERVER_OPEN_SALVAGE_REQUEST);
        pktSendString(pak, pSalvageItem->pchName);
        pktSendBitsAuto(pak, amount);
        END_PACKET

        // ARM NOTE:  If we actually submit multiple opens, this throttle is going to be a problem.
        if (p->entParent && p->entParent->pl)
            p->entParent->pl->last_certification_request = 0;
    }
#endif

    return res;
}

int character_AdjustSalvage(Character *p, SalvageItem const *pSalvageItem, int amount, 
                              const char *context, int bNoInvWarning )
{
    return character_AdjustSalvageEx(p, pSalvageItem, amount, context, bNoInvWarning, true);
}


///------------------------------------------------------------
//  Find index of the passed inventory item
//----------------------------------------------------------
int character_FindSalvageInvIdx(Character *pchar, const SalvageInventoryItem *sal)
{
    int res = -1;
    int i;

    if( verify( pchar && sal && sal->salvage ))
    {
        SalvageInventoryItem **inv = pchar->salvageInv;

        for( i = 0; i < eaSize(&inv); ++i )
        {
            if(inv[i] == sal)
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
//  Find a salvage inventory item by name
//----------------------------------------------------------
SalvageInventoryItem *character_FindSalvageInv(Character *c, const char *name)
{
    SalvageInventoryItem *res = NULL;

    if( verify( c && name ))
    {
        SalvageInventoryItem **items = c->salvageInv;
        int i;
        for( i = eaSize( &items ) - 1; i >= 0 && !res; --i)
        {
            if( items[i] && items[i]->salvage && 0 == stricmp(name, items[i]->salvage->pchName) )
            {
                res = items[i];
            }
        }
    }

    // --------------------
    // finally

    return res;
}

//  Find a salvage inventory item by name
//----------------------------------------------------------
SalvageInventoryItem *character_FindSalvageByPtr(Character *c, SalvageItem *pSalvage)
{
    SalvageInventoryItem *res = NULL;

    if( verify( c && pSalvage ))
    {
        SalvageInventoryItem **items = c->salvageInv;
        int i;
        for( i = eaSize( &items ) - 1; i >= 0 && !res; --i)
        {
            if( items[i] && items[i]->salvage && items[i]->salvage == pSalvage) 
            {
                res = items[i];
            }
        }
    }

    // --------------------
    // finally

    return res;
}



#if SERVER
//------------------------------------------------------------
//  Reverse engineer the given salvage in inventory
//----------------------------------------------------------
void character_ReverseEngineerSalvage(Character *pchar, int idx)
{
    if( verify( pchar && EAINRANGE( idx, pchar->salvageInv) && pchar->salvageInv[idx]->salvage && pchar->salvageInv[idx]->amount > 0 && pchar->salvageInv[idx]->salvage->ppchRewardTables ))
    {
        SalvageItem const *sal = pchar->salvageInv[idx]->salvage;

        int breakthru_skill_level = pchar->iWisdomLevel;
        int success_pct = 100 + MAX( breakthru_skill_level - sal->challenge_points, 0 );
        int randRoll = rule30Rand() % 100;

        if( randRoll < success_pct )
        {
            // success. call standard reward function.
            rewardFindDefAndApplyToEnt( pchar->entParent, sal->ppchRewardTables, VG_NONE, 1, true, REWARDSOURCE_UNDEFINED, NULL);
        }
        else
        {
            // failure
            // @todo -AB: the 'failed' ui feedback :2005 Feb 28 03:59 PM
        }

        character_AdjustSalvage(pchar,sal,-1,"reverse_engineer", false);
    }
}
#endif // SERVER

// end salvage functions
// ------------------------------------------------------------

//------------------------------------------------------------
//  Get the inventory id and amount
// sets *id and *amount to the values, or 0 if error
//----------------------------------------------------------
void character_GetInvInfo(Character *p, int *id, int *amount, const char **name, InventoryType type, int idx)
{
    int resid = 0, resamt = 0;
    const char *resName = NULL;

    switch( type )
    {
    case kInventoryType_Salvage:
    {
        if( EAINRANGE( idx, p->salvageInv ) && p->salvageInv[idx]->salvage )
        {
            resid = p->salvageInv[idx]->salvage->salId;
            resamt = p->salvageInv[idx]->amount;
            resName = p->salvageInv[idx]->salvage->pchName;
        }
    }
    break;
    case kInventoryType_Concept:
     {
         if(EAINRANGE( idx, p->conceptInv) && p->conceptInv[idx] && p->conceptInv[idx]->concept)
         {
             resid = p->conceptInv[idx]->concept->def->id;
             resamt = p->conceptInv[idx]->amount;
             resName = p->conceptInv[idx]->concept->def->name;
         }
     }
     break;
    case kInventoryType_Recipe:
    {
        if(EAINRANGE( idx, p->recipeInv) && p->recipeInv[idx] && p->recipeInv[idx]->recipe)
        {
            resid = p->recipeInv[idx]->recipe->id;
            resamt = p->recipeInv[idx]->amount;
            resName = p->recipeInv[idx]->recipe->pchName;
        }
    }
    break;
    case kInventoryType_BaseDetail:
    {
        if(EAINRANGE( idx, p->detailInv ) && p->detailInv[idx] && p->detailInv[idx]->item)
        {
            AttribFileDict const *dict =  inventorytype_GetAttribs( kInventoryType_BaseDetail );
            
            if(verify( dict))
            {
                // ab: crazy hack: make sure the case is correct.
                cStashElement elem;
                stashFindElementConst( dict->idHash, p->detailInv[idx]->item->pchName, &elem);
                
                resid = p->detailInv[idx]->item->id;
                resamt = p->detailInv[idx]->amount;
                resName = stashElementGetStringKey(elem);
            }
            else
            {
                LOG_OLD( "character_inventory" __FUNCTION__ ":couldn't get base details dict.");
            }
        }
    }
    break;
    case kInventoryType_StoredSalvage:
    {
        if( EAINRANGE( idx, p->storedSalvageInv ) && p->storedSalvageInv[idx]->salvage )
        {
            resid = p->storedSalvageInv[idx]->salvage->salId;
            resamt = p->storedSalvageInv[idx]->amount;
            resName = p->storedSalvageInv[idx]->salvage->pchName;
        }
    }
    break;
    default:
    {
        assertmsg(0,"invalid type.");
    }
    break;
    };

    // --------------------
    // finally, assign the values

    if(id) *id = resid;
    if(amount) *amount = resamt;
    if(name) *name = resName;
}

//------------------------------------------------------------
//  Get the inventory size
// returns 0 on failure
// note: failure is indistinguishable from empty inventory. this
//  is deliberate since not all inventory types have an explicit inventory
//----------------------------------------------------------
int character_GetInvSize(Character *p, InventoryType type)
{
    GenericInvItem ***h = character_GetInvHandleByType( p, type );
    return h ?  eaSize( h ) : 0;
}



#if SERVER
// Make sure you update CHAR_BOOST_MAX in character_base.h
// and the s_tabData arrays if you add more enhancement slots!
static int ComputeEnhancementSlotsFromStore(Character *p)
{
    int packs = 0;
    if (p && p->entParent)
    {
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisei01")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisei02")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisei03")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisei04")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisei05")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisei06")) != 0;
    }

    return 10 * packs;
}

static int ComputeSalvageSlotsFromStore(Character *p)
{
    int packs = 0;
    if (p && p->entParent)
    {
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upissi01")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upissi02")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upissi03")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upissi04")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upissi05")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upissi06")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upissi07")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upissi08")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upissi09")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upissi10")) != 0;
    }

    return 10 * packs;
}

static int ComputeRecipeSlotsFromStore(Character *p)
{
    int packs = 0;
    if (p && p->entParent)
    {
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisri01")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisri02")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisri03")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisri04")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisri05")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisri06")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisri07")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisri08")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisri09")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisri10")) != 0;
    }

    return 5 * packs;
}

static int ComputeVaultSlotsFromStore(Character *p)
{
    int packs = 0;
    if (p && p->entParent)
    {
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisvi01")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisvi02")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisvi03")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisvi04")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisvi05")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisvi06")) != 0;
    }

    return 5 * packs;
}

static int ComputeAuctionSlotsFromStore(Character *p)
{
    int packs = 0;
    if (p && p->entParent)
    {
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisai01")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisai02")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisai03")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisai04")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisai05")) != 0;
        packs += AccountHasStoreProduct(ent_GetProductInventory( p->entParent ), SKU("upisai06")) != 0;
    }

    return 5 * packs;
}

static int ComputeSalvageSlotBonusForLoyalty(Character *p)
{
    int bonus = 0;

    if (p->entParent && p->entParent->pl)
    {
        int i;
        for (i = 0; i < eaSize(&g_accountLoyaltyRewardTree.levels); ++i)
        {
            const InventoryLoyaltyBonusSizeDefinition *pBonusSize;
            if (accountLoyaltyRewardHasLevel(p->entParent->pl->loyaltyPointsEarned, g_accountLoyaltyRewardTree.levels[i]->name) &&
                stashFindPointerConst(g_InventoryLoyaltyBonusSizeDefs.st_LoyaltyBonus, g_accountLoyaltyRewardTree.levels[i]->name, &pBonusSize))
            {
                bonus += pBonusSize->salvageBonus;
            }
        }
    }

    return bonus;
}

static int ComputeRecipeSlotBonusForLoyalty(Character *p)
{
    int bonus = 0;

    if (p->entParent && p->entParent->pl)
    {
        int i;
        for (i = 0; i < eaSize(&g_accountLoyaltyRewardTree.levels); ++i)
        {
            const InventoryLoyaltyBonusSizeDefinition *pBonusSize;
            if (accountLoyaltyRewardHasLevel(p->entParent->pl->loyaltyPointsEarned, g_accountLoyaltyRewardTree.levels[i]->name) &&
                stashFindPointerConst(g_InventoryLoyaltyBonusSizeDefs.st_LoyaltyBonus, g_accountLoyaltyRewardTree.levels[i]->name, &pBonusSize))
            {
                bonus += pBonusSize->recipeBonus;
            }
        }
    }

    return bonus;
}

static int ComputeVaultSlotBonusForLoyalty(Character *p)
{
    int bonus = 0;

    if (p->entParent && p->entParent->pl)
    {
        int i;
        for (i = 0; i < eaSize(&g_accountLoyaltyRewardTree.levels); ++i)
        {
            const InventoryLoyaltyBonusSizeDefinition *pBonusSize;
            if (accountLoyaltyRewardHasLevel(p->entParent->pl->loyaltyPointsEarned, g_accountLoyaltyRewardTree.levels[i]->name) &&
                stashFindPointerConst(g_InventoryLoyaltyBonusSizeDefs.st_LoyaltyBonus, g_accountLoyaltyRewardTree.levels[i]->name, &pBonusSize))
            {
                bonus += pBonusSize->vaultBonus;
            }
        }
    }

    return bonus;
}

static int ComputeAuctionSlotBonusForLoyalty(Character *p)
{
    int bonus = 0;

    if (p->entParent && p->entParent->pl)
    {
        int i;
        for (i = 0; i < eaSize(&g_accountLoyaltyRewardTree.levels); ++i)
        {
            const InventoryLoyaltyBonusSizeDefinition *pBonusSize;
            if (accountLoyaltyRewardHasLevel(p->entParent->pl->loyaltyPointsEarned, g_accountLoyaltyRewardTree.levels[i]->name) &&
                stashFindPointerConst(g_InventoryLoyaltyBonusSizeDefs.st_LoyaltyBonus, g_accountLoyaltyRewardTree.levels[i]->name, &pBonusSize))
            {
                bonus += pBonusSize->auctionBonus;
            }
        }
    }

    return bonus;
}

void character_UpdateInvSizes(Character *p)
{
    int iLevel = character_PowersGetMaximumLevel(p);
    int salvageSlotsForLevel;
    int recipeSlotsForLevel;
    int storedSalvageSlotsForLevel;
    if (p->entParent && p->entParent->pl && AccountIsVIP(ent_GetProductInventory( p->entParent ), p->entParent->pl->account_inventory.accountStatusFlags))
    {
        salvageSlotsForLevel = eaiGet(&g_InventorySizeDefs.salvageSizes.piAmountAtLevel, iLevel);
        recipeSlotsForLevel = eaiGet(&g_InventorySizeDefs.recipeSizes.piAmountAtLevel, iLevel);
        storedSalvageSlotsForLevel = eaiGet(&g_InventorySizeDefs.storedSalvageSizes.piAmountAtLevel, iLevel);
    }
    else
    {
        salvageSlotsForLevel = eaiGet(&g_InventorySizeDefs.salvageSizes.piAmountAtLevelFree, iLevel);
        recipeSlotsForLevel = eaiGet(&g_InventorySizeDefs.recipeSizes.piAmountAtLevelFree, iLevel);
        storedSalvageSlotsForLevel = eaiGet(&g_InventorySizeDefs.storedSalvageSizes.piAmountAtLevelFree, iLevel);
    }

    p->salvageInvTotalSlots =
        ComputeSalvageSlotsFromStore(p) +
        ComputeSalvageSlotBonusForLoyalty(p) +
        p->salvageInvBonusSlots +
        salvageSlotsForLevel;
    p->recipeInvTotalSlots =
        ComputeRecipeSlotsFromStore(p) +
        ComputeRecipeSlotBonusForLoyalty(p) +
        p->recipeInvBonusSlots +
        recipeSlotsForLevel;
    p->storedSalvageInvTotalSlots =
        ComputeVaultSlotsFromStore(p) +
        ComputeVaultSlotBonusForLoyalty(p) +
        p->storedSalvageInvBonusSlots +
        storedSalvageSlotsForLevel;
}
#endif

#if SERVER
//------------------------------------------------------------
//  Get the base inventory max size for the specified level without including
//        returns the maximum size of the specified type of inventory
//        returns -1 if no limit
//----------------------------------------------------------
int character_GetInvTotalSizeByLevel(int level, InventoryType type)
{

    switch( type )
    {
        case kInventoryType_Salvage:
            return eaiGet(&g_InventorySizeDefs.salvageSizes.piAmountAtLevel, level);
            break;
        case kInventoryType_Recipe:
            return eaiGet(&g_InventorySizeDefs.recipeSizes.piAmountAtLevel, level);
            break;
        case kInventoryType_Concept:
        case kInventoryType_BaseDetail:
            return -1;
            break;
        case kInventoryType_StoredSalvage:
            return eaiGet(&g_InventorySizeDefs.storedSalvageSizes.piAmountAtLevel, level);
            break;
    };
    return -1;
}
#endif


//------------------------------------------------------------
//  Get the inventory total size
//        returns the maximum size of the specified type of inventory
//        returns -1 if no limit
//----------------------------------------------------------
int character_GetInvTotalSize(Character *p, InventoryType type)
{
    switch( type )
    {
    case kInventoryType_Salvage:
        {
#if SERVER
            character_UpdateInvSizes(p);
#endif
            return p->salvageInvTotalSlots;
        }
        break;
    case kInventoryType_Recipe:
        {
#if SERVER
            character_UpdateInvSizes(p);
#endif
            return p->recipeInvTotalSlots;
        }
        break;
    case kInventoryType_Concept:
    case kInventoryType_BaseDetail:
        {
            return -1;
        }
        break;
    case kInventoryType_StoredSalvage:
#if SERVER
        character_UpdateInvSizes(p);
#endif
            return p->storedSalvageInvTotalSlots;
    default:
        {
            assertmsg(0,"invalid type.");
        }
        break;
    };
    return -1;
}

//------------------------------------------------------------
//  get auction inventory total size based on rewards and level
//----------------------------------------------------------
int character_GetAuctionInvTotalSize(Character *p)
{
#if SERVER
    int iLevel = character_PowersGetMaximumLevel(p);
    int auctionSlotsForLevel;
    if (p->entParent && p->entParent->pl && AccountIsVIP(ent_GetProductInventory( p->entParent ), p->entParent->pl->account_inventory.accountStatusFlags))
    {
        auctionSlotsForLevel = eaiGet(&g_InventorySizeDefs.auctionSizes.piAmountAtLevel, iLevel);
    }
    else
    {
        auctionSlotsForLevel = eaiGet(&g_InventorySizeDefs.auctionSizes.piAmountAtLevelFree, iLevel);
    }
    p->auctionInvTotalSlots =
        ComputeAuctionSlotsFromStore(p) +
        ComputeAuctionSlotBonusForLoyalty(p) +
        p->auctionInvBonusSlots +
        auctionSlotsForLevel;
#endif
    return p->auctionInvTotalSlots;
}

//------------------------------------------------------------
//  get boost inventory total size based on rewards and level
//----------------------------------------------------------
int character_GetBoostInvTotalSize(Character *p)
{
#if SERVER
    int boostSlots = 10;
    boostSlots += ComputeEnhancementSlotsFromStore(p);
    p->iNumBoostSlots = boostSlots;
#endif
    return p->iNumBoostSlots;
}

#if SERVER
//------------------------------------------------------------
//  get auction inventory base size based on level
//----------------------------------------------------------
int character_GetAuctionInvTotalSizeByLevel(int level)
{
    return eaiGet(&g_InventorySizeDefs.auctionSizes.piAmountAtLevel, level);
}
#endif

//------------------------------------------------------------
//  Set the inventory total size
//----------------------------------------------------------
void character_SetInvTotalSize(Character *p, InventoryType type, int size)
{
    //! @todo p can be NULL.  How does this occur?  Should we prevent that from occurring?
    if(p)
    {
        switch(type)
        {
            xcase kInventoryType_Salvage:        p->salvageInvTotalSlots = size;
            xcase kInventoryType_Recipe:        p->recipeInvTotalSlots = size;
            xcase kInventoryType_Concept:        // no size limit
            xcase kInventoryType_BaseDetail:    // no size limit
            xcase kInventoryType_StoredSalvage:    p->storedSalvageInvTotalSlots = size;
            xdefault:                            assertmsg(0,"invalid type.");
        };
    }
}

//------------------------------------------------------------
//  check validity of type
//----------------------------------------------------------
bool  inventorytype_Valid(InventoryType t)
{
    return INRANGE( t, 0, kInventoryType_Count );
}

// ------------------------------------------------------------
// concepts

static int s_FindConceptByDef(Character *p, const ConceptDef *def, int startingIdx)
{
    int size = eaSize( &p->conceptInv );
    int res = size;
    if( verify( p && def ))
    {
        int i;
        for( i = startingIdx; i < size; ++i )
        {
            if( p->conceptInv[i] && p->conceptInv[i]->concept && p->conceptInv[i]->concept->def == def )
            {
                res = i;
                break; // BREAK IN FLOW
            }
        }

    }
    // --------------------
    // finally

    return res;
}


//------------------------------------------------------------
//
//----------------------------------------------------------
bool conceptinventoryitem_Valid(ConceptInventoryItem *inv)
{
    return inv && inv->amount > 0 && inv->concept;
}

#if SERVER
int character_AdjustConcept(Character *p, int defId, F32 *afVars, int amount)
{
    int res = -1;
    const ConceptDef *def = conceptdef_GetById(defId);
    if( verify( p && def && afVars))
    {
        int defIdx;
        int size = eaSize(&p->conceptInv);

        // --------------------
        // first try and find a concept that matches the given fvars

        for( defIdx = s_FindConceptByDef( p, def, 0 ); defIdx < size && res < 0 ; defIdx = s_FindConceptByDef( p, def, defIdx+1 ) )
        {
            ConceptInventoryItem *item = p->conceptInv[defIdx];
            if( verify( item && item->concept && item->concept->def ))
            {
                int i;
                int numVars = eaSize(&def->attribMods);
                bool allMatch = true;
                for( i = 0; i < numVars; ++i )
                {
                    F32 scaleAmt = 1000.f;
                    F32 scaledVar = item->concept->afVars[i]*scaleAmt;
                    F32 hi = ceil(scaledVar);
                    F32 lo = floor(scaledVar);
                    allMatch = allMatch && INRANGE( afVars[i]*scaleAmt, lo, hi );
                }

                if( allMatch )
                {
                    res = defIdx;
                    break; // BREAK IN FLOW
                }
            }
        }

        // secondly, try and slot into an empty inventory item.
        // NOTE: this is purely for db reasons. inventory that doesn't change
        // between saves to the db needs to keep its original position. for that
        // reason the concept inventory may not be packed and valid at all times.
        if( res < 0 )
        {
            int i;
            for( i = 0; i < eaSize(&p->conceptInv) && res < 0; ++i )
            {
                if( p->conceptInv[i])
                {
                    if( p->conceptInv[i]->amount == 0 || p->conceptInv[i]->concept == NULL )
                    {
                        res = i;
                    }
                }
            }
        }

        // finally, if still no slot found, add it to the end
        if( res < 0 )
        {
            res = eaSize(&p->conceptInv);
        }

        // ----------------------------------------
        // add the concept

        if( verify( res >= 0 ))
        {
            int curAmt = 0;
            if( EAINRANGE( res, p->conceptInv ) )
            {
                curAmt += p->conceptInv[res]->amount;
            }
            character_SetConcept( p, defId, afVars, res, amount + curAmt );
        }
    }

    return res;
}
#endif // SERVER


//------------------------------------------------------------
//  Set the info about a concept inventory item
// param amount: the amount of inventory, if zero only idx and p
//                 params are necessary.
//----------------------------------------------------------
void character_SetConcept( Character *p, int defId, F32 *afVars, int idx, int amount )
{
    if( verify( p && afVars
        && s_PrepInvToSetItem(p, cpp_static_cast(GenericInvItem***)(&p->conceptInv), kInventoryType_Concept, idx )))
    {
        // create, if necessary
        if( p->conceptInv[idx] == NULL )
        {
            p->conceptInv[idx] = (ConceptInventoryItem *)genericinvitem_Create(NULL);
        }

        // only create the item if
        if( amount > 0 )
        {
            p->conceptInv[idx]->concept = conceptitem_Init( p->conceptInv[idx]->concept, defId, afVars );
        }

        p->conceptInv[idx]->amount = amount;
    }
}


#if SERVER
//------------------------------------------------------------
//  Roll the attributes of a concept from its definition id
//----------------------------------------------------------
void character_AddRollConcept( Character *p, int defId, int amount )
{
    const ConceptDef *cd = conceptdef_GetById( defId );
    if( verify( p && cd ))
    {
        F32 afVars[TYPE_ARRAY_SIZE( ConceptItem, afVars )] = {0};
        int i;
        int size = MIN(ARRAY_SIZE( afVars ), eaSize( &cd->attribMods ));

        // calc the vars
        for( i = 0; i < size; ++i )
        {
            const AttribModGroup *am = cd->attribMods[i];

            // -1 to 1, inclusive.
            F32 roll = rule30Float();

            // scale it to the range
            afVars[i] = (roll*(am->max - am->min) + am->max + am->min)*.5f;
        }

        // add the concept
        character_AdjustConcept( p, defId, afVars, amount );
    }
}
#endif


//------------------------------------------------------------
//  Helper for properly removing an inventory item.
// param amount: the number to remove, if < 0, remove all
//----------------------------------------------------------
U32 character_RemoveConcept(Character *p, int invIdx, int amount)
{
    U32 res;
    if( verify( p && EAINRANGE( invIdx, p->conceptInv ) && p->conceptInv[invIdx] ))
    {
        U32 amt = amount < 0 ? p->conceptInv[invIdx]->amount : (U32)amount;
        res = p->conceptInv[invIdx]->amount > amt ? amt : p->conceptInv[invIdx]->amount;
        p->conceptInv[invIdx]->amount -= res;
        if( p->conceptInv[invIdx]->amount == 0 )
        {
            p->conceptInv[invIdx]->concept = NULL;
        }
    }

    // --------------------
    // finally

    return res;
}


// end concepts
// ------------------------------------------------------------

// ------------------------------------------------------------
// DB section


//------------------------------------------------------------
//  return the name of a table based on its inventory
//----------------------------------------------------------
char const* inventory_GetDbTableName( InventoryType type )
{
    static char const *dbnames[] =
        {
            "InvSalvage",
            "InvConcept", // kInventoryType_Concept
            "InvRecipe",  // kInventoryType_Recipe
            "InvBaseDetail", // ab: not actually used anywhere. see ent_desc
            "InvStoredSalvage",
        };
     STATIC_INFUNC_ASSERT(ARRAY_SIZE( dbnames ) == kInventoryType_Count);
    return INRANGE0( type, kInventoryType_Count  ) ? dbnames[type] : NULL;
}


// end DB section
// ------------------------------------------------------------

//------------------------------------------------------------
//processes the unique id attribute array (ppAttribItems) in the
//passed dict and creates the idHash and maxId for the dict
//
// returns the max id found, or 0 on error.
//
// NOTE: assumes that ppAttribItems is already full.  note:
//independent of type
//----------------------------------------------------------
bool attribfiledict_FinalProcess(ParseTable pti[], AttribFileDict *dict, bool shared_memory)
{
    int res = 0;
    int i;

    assert(!dict->idHash);
    dict->idHash = stashTableCreateWithStringKeys(stashOptimalSize(eaSize(&dict->ppAttribItems)), stashShared(shared_memory));

    assert(!dict->nameFromId);
    dict->nameFromId = stashTableCreate(stashOptimalSize(eaSize(&dict->ppAttribItems)), stashShared(shared_memory), StashKeyTypeInts, sizeof(int));

    // add the id lookup
    for(i = 0; i < eaSize(&dict->ppAttribItems); i++)
    {
        AttribFileItem *p = cpp_const_cast(AttribFileItem*)(dict->ppAttribItems[i]);

        // even if previous iteration has failed, continue to parse.
        if( verify( p->id > 0 ))
        {
            stashAddIntConst(dict->idHash, p->name, p->id, false);
            stashIntAddPointerConst(dict->nameFromId, p->id, p->name, false);
            res = MAX(res, p->id);
        }
    }

    // set the max
    dict->maxId = res;

    return res != 0;
}


//------------------------------------------------------------
//  pool of recipe items
//----------------------------------------------------------
MP_DEFINE(RecipeItem);
static RecipeItem* recipeitem_Create(const BasePower *item)
{
    RecipeItem *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(RecipeItem, 64);
    res = MP_ALLOC( RecipeItem );
    if( verify( res ))
    {
        res->recipe = item;
        res->name = item->pchName;
    }
    // --------------------
    // finally, return

    return res;
}

bool recipeinventoryitem_Valid(RecipeInventoryItem *inv)
{
    return inv && inv->amount > 0 && inv->recipe;
}


//------------------------------------------------------------
//  get the specified recipe by name
//----------------------------------------------------------
const DetailRecipe * recipe_GetItem(const char *name)
{
    return detailrecipedict_RecipeFromName(name);
}


const DetailRecipe *recipe_GetItemById( int id )
{
    return detailrecipedict_RecipeFromId(id);
}

int recipe_GetMaxId()
{
    int res = -1;
    const GenericInventoryType* const* * ref = GetItemsByIdArrayRef( kInventoryType_Recipe );
    if( verify( ref ))
    {
        res = eaSize(ref) - 1;
    }
    // --------------------
    // finally

    return res;
}

// Don't allow partially successful actions, only allow if the player can complete the requested change
bool character_CanRecipeBeChanged(Character *p, DetailRecipe const *pRecipe, int amount )
{
    if( verify(p && pRecipe ))
    {
        RecipeInventoryItem **recipes = p->recipeInv;
        int i;

        // check for overful inventory
        if(amount > 0)
        {
            int totalSlots = character_GetInvTotalSize(p, kInventoryType_Recipe);
            if (totalSlots >= 0 && p->recipeInvCurrentCount + amount > totalSlots)
                return false;
        }

        for( i = eaSize( &recipes ) - 1; i >= 0; --i)
        {
            if( recipes[i]->recipe == pRecipe )
            { //we found an existing item
                if (amount > 0 && (recipes[i]->amount + amount <= (U32) pRecipe->maxInvAmount ||
                    pRecipe->maxInvAmount == 0))
                    return true; // we can add all requested
                if (amount < 0 && (int) recipes[i]->amount + amount >= 0)
                    return true; // we can remove all requested
                return false;
            }
        }
        if (amount < 0) 
            return false; //don't remove item we dont have
        return true;
    }
    return false;
}

int character_RecipeCount(Character *p, DetailRecipe const *pRecipe )
{
    if( verify(p && pRecipe ))
    {
        RecipeInventoryItem **recipes = p->recipeInv;
        int i;

        for( i = eaSize( &recipes ) - 1; i >= 0; --i)
        {
            if( recipes[i]->recipe == pRecipe )
                return recipes[i]->amount; 
        }
    }
    return 0;
}

bool character_IsSalvageUseful(Character *p, SalvageItem const *pSalvageItem, const DetailRecipe ***useful_to)
{
    bool useful = false;
    int i;
    const DetailRecipe **detailRecipes = NULL;

    if(verify(p && pSalvageItem))
    {
        detailRecipes = entity_GetValidDetailRecipes(p->entParent, NULL, p->entParent->pl->workshopInteracting); 
        for(i = eaSize(&detailRecipes)-1; i >= 0; --i)
        {
            const DetailRecipe *recipe = detailRecipes[i];
            if(recipe)
            {
                int j;
                for(j = eaSize(&recipe->ppSalvagesRequired)-1; j >= 0; --j)
                {
                    const SalvageItem *salvage = recipe->ppSalvagesRequired[j]->pSalvage;
                    if(salvage && salvage->salId == pSalvageItem->salId)
                    {
                        if(useful_to)
                        {
                            eaPushConst(useful_to,recipe);
                            useful = true;
                            break;
                        }
                        else
                        {
                            // leaking detailRecipes?
                            return true;
                        }
                    }
                }
            }
        }

        if (detailRecipes)
            eaDestroyConst(&detailRecipes);
    }
    return useful;
}

#if SERVER

//------------------------------------------------------------
// add a recipe to the character's inventory
//----------------------------------------------------------
int character_AdjustRecipe(Character *p, DetailRecipe const *item, int amount, const char *context)
{
    GenericInvItem ***hInv = (GenericInvItem ***)&p->recipeInv;
    int count = character_RecipeCount(p, item);
    int retval = 0;
    
    if(amount > 0)
    {
        DATA_MINE_RECIPE(item,"in",context,amount);
        if ( count + amount > item->maxInvAmount )
        {
            int actual_amount = item->maxInvAmount - count;
            DATA_MINE_RECIPE(item,"out","inventory_full",amount - actual_amount);
            amount = actual_amount;
        }
    }
    else if(amount < 0)
    {
        if(-amount > count)
            amount = -count;
        DATA_MINE_RECIPE(item, "out", context, -amount);
    }

    retval = genericinv_AdjustItem( p, hInv, kInventoryType_Recipe, (DetailRecipe *) item, amount, context); 

    // did this fill up the inventory?
    if( p->entParent && amount > 0 && character_InventoryFull(p, kInventoryType_Recipe) )
        serveFloater(p->entParent, p->entParent, "FloatRecipesFull", 0.0f, kFloaterStyle_Attention, 0xff0000ff);

    return retval;
}


#endif

void character_SetRecipeInvCurrentCount(Character *p)
{
    RecipeInventoryItem **recipes = NULL;
    int i;

    if(!p)
        return;

    recipes = p->recipeInv;
    p->recipeInvCurrentCount = 0;

    for( i = eaSize( &recipes ) - 1; i >= 0; --i)
    {
        if (recipes[i])
            p->recipeInvCurrentCount += recipes[i]->amount;
    }
}

//------------------------------------------------------------
//  Creates the dictionary of inventory items that a player
// can get from the recipes that can be used to make details.
//----------------------------------------------------------
void basedetail_CreateDetailInvHashes(DetailDict * ddict, bool shared_memory)
{
    if(verify(ddict))
    {
        genericinvdict_CreateHashes((GenericInvDictionary*)ddict, inventorytype_GetAttribs(kInventoryType_BaseDetail), shared_memory);
    }
}


//------------------------------------------------------------
//  add the passed detail to inventory
//----------------------------------------------------------
int character_AddBaseDetail(Character *p,char const *detailName, const char *context)
{
    if(verify( p && detailName ))
    {
        const Detail *d = detail_GetItem(detailName);
        if (d == NULL) return -1;
        return character_AdjustInventory( p, kInventoryType_BaseDetail, d->id, 1, context, false);
    }
    return -1;
}

//------------------------------------------------------------
//  get the named detail
//----------------------------------------------------------
const Detail *detail_GetItem( char const *name )
{
    //return (DetailItem*)genericinv_GetItem( name, kInventoryType_BaseDetail );
    if( name && g_DetailDict.haItemNames )
    {
        return stashFindPointerReturnPointerConst( g_DetailDict.haItemNames, name );
    }
    return NULL;
}

// end detail recipes
// ------------------------------------------------------------


//------------------------------------------------------------
//  get the handle to the inventory attribs for a file.
//----------------------------------------------------------
AttribFileDict const * inventorytype_GetAttribs( InventoryType type )
{
    return &s_invAttribFiles[type];
}

static void attribfiledict_Load(SHARED_MEMORY_PARAM AttribFileDict *hitems, char const *stem, bool bNewAttribs)
{
#if SERVER 
    int flags = PARSER_SERVERONLY | (bNewAttribs?PARSER_FORCEREBUILD:0);
#else
    int flags = (bNewAttribs?PARSER_FORCEREBUILD:0);
#endif
    char *fname = NULL;
    const char *pchBinFilename;
    TokenizerParseInfo *attribParseInfo;

    estrCreate(&fname);
    estrPrintf(&fname, "defs/dbidmaps/%s.dbidmap", stem);

    pchBinFilename = MakeBinFilename(fname);
    attribParseInfo = attribfiledict_GetParseInfo();

    if( hitems )
    {
        ParserLoadFilesShared(MakeSharedMemoryName(pchBinFilename),NULL,fname,pchBinFilename,flags,
            attribParseInfo,hitems,sizeof(*hitems),NULL,NULL,NULL,NULL,attribfiledict_FinalProcess);        
    }

    // cleanup
    estrDestroy(&fname);
}

//------------------------------------------------------------
//  Load all the dbid attribute files
//----------------------------------------------------------
void load_InventoryAttribFiles( bool bNewAttribs )
{
    int i;
    int invCount = kInventoryType_Count;

    // --------------------
    // for each inventory type, load the attribfile

    for( i = 0; i < invCount; ++i )
    {
        attribfiledict_Load( &s_invAttribFiles[i], inventory_GetDbTableName( i ), bNewAttribs );
    }
}

TokenizerParseInfo ParseAttributeItem[] =
{
    {    "Id",            TOK_STRUCTPARAM | TOK_INT(AttribFileItem, id, 0)},
    {    "Name",            TOK_STRUCTPARAM | TOK_STRING(AttribFileItem, name, 0)},
    {    "\n",            TOK_END,                            0},
    {    "", 0, 0 }
};

TokenizerParseInfo ParseAttributeItems[] =
{
    {    "AttribFileItem", TOK_STRUCT(AttribFileDict, ppAttribItems, ParseAttributeItem)},
    {    "", 0, 0 }
};

TokenizerParseInfo *attribfiledict_GetParseInfo()
{
    return ParseAttributeItems;
}

#ifdef SERVER
MP_DEFINE(AttribFileItem);
static AttribFileItem* attribfileitem_Create( int id, char *name )
{
    AttribFileItem *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(AttribFileItem, 64);
    res = MP_ALLOC( AttribFileItem );
    if( verify( res ))
    {
        res->id = id;
        res->name = name;
    }
    // --------------------
    // finally, return

    return res;
}
#endif // SERVER

#ifdef SERVER
static void attribfileitem_Destroy( AttribFileItem *item )
{
    MP_FREE(AttribFileItem, item);
}

static void attribfiledict_Create( AttribFileDict *dict, int size )
{
    eaCreateConst( &dict->ppAttribItems );
    eaSetCapacityConst( &dict->ppAttribItems, size );
}

static void attribfiledict_Cleanup( AttribFileDict *dict )
{
    eaDestroyExConst( &dict->ppAttribItems, attribfileitem_Destroy );
}
#endif // SERVER

#if SERVER
//------------------------------------------------------------
//  helper for writing a dbid file
//----------------------------------------------------------
void s_writeDbidsFromStem(const GenericInventoryType* const* types, const AttribFileDict *attribs, const char *stem)
{
    char *fname = NULL;
    estrCreate(&fname);
    estrPrintf(&fname, "defs/dbidmaps/%s.dbidmap", stem);
    genericinvtype_WriteDbIds(types, attribs, fname);
    estrDestroy(&fname);
}
#endif // SERVER

#ifdef SERVER
//------------------------------------------------------------
//  write the salvage ids for the given
// note: independent of type
//----------------------------------------------------------
void inventory_WriteDbIds()
{
     int type;
    int invCount = kInventoryType_Count;

    // --------------------
    // inventory dbids

    for( type = 0; type < invCount ; ++type )
    {
        const GenericInventoryType* const* * h = GetItemsByIdArrayRef( type );
        if(h)
        {
            char const *stem = inventory_GetDbTableName( type );
            const AttribFileDict* attribs = inventorytype_GetAttribs(type);
            s_writeDbidsFromStem( *h, attribs, stem );
        }
    }
}

#endif

#if SERVER
AttribFileDict inventory_GetMergedAttribs(InventoryType type)
{
    return genericinvtype_MergeAttribFileDict( *GetItemsByIdArrayRef(type), inventorytype_GetAttribs(type));
}
#endif // SERVER

#if SERVER
#endif // SERVER

static bool s_flushIds = false;


#if SERVER
AttribFileDict genericinvtype_MergeAttribFileDict(const GenericInventoryType* const* types, AttribFileDict const*attribs)
{
    AttribFileDict dict = {0};
    if( types && attribs )
    {
        int i;
        int size = eaSize( &types );

        attribfiledict_Create( &dict, size );

        // --------------------
        //  gather the names

        // first re-add the existing items (can't ever lose an id)
        // @todo -AB: before we release this, comment this out once and we'll flush it. :2005 May 06 05:11 PM
        if( !s_flushIds )
        {
            for( i = 0; i < eaSize(&attribs->ppAttribItems); ++i )
            {
                eaPushConst( &dict.ppAttribItems, attribs->ppAttribItems[i] );
            }
        }

        // add the new items
        for( i = 0; i < size ; ++i )
        {
            int id;

            // add the new items, unless its already in the table
            // note: if trying to
            if( types[i] && types[i]->name && (s_flushIds || !attribs->idHash || !stashFindInt( attribs->idHash, types[i]->name, &id )))
            {
                // if this wasn't in the hash already, add it
                eaPushConst( &dict.ppAttribItems, attribfileitem_Create( (types)[i]->id, (char*)(types)[i]->name ) );
            }
        }
    }
    return dict;
}

#endif // SERVER
#ifdef SERVER
//------------------------------------------------------------
//  write a collection of GenericInventoryTypes to an attribute file
// so their unique ids will be preserved across data changes.
//----------------------------------------------------------
void genericinvtype_WriteDbIds(const GenericInventoryType* const* types, AttribFileDict const *attribs, char const *fname)
{
    if(types && attribs && fname)
    {
        AttribFileDict dict = genericinvtype_MergeAttribFileDict(types,attribs);

        // --------------------
        // write the attribute file

        if(!ParserWriteTextFile( (char*)fname, ParseAttributeItems, &dict, 0, 0))
            Errorf("couldn't write file %s", fname);

        // --------------------
        // finally

        // can't cleanup, part of this is pooled memory (carumba)
        //attribfiledict_Cleanup( &dict );
    }
}


#endif

//------------------------------------------------------------
//  write a collection of GenericInventoryTypes to an attribute file
// so their unique ids will be preserved across data changes.
//----------------------------------------------------------
void genericinvdict_CreateHashes(GenericInvDictionary *pdict, AttribFileDict const *attribs, bool shared_memory)
{
    int nextIdx = attribs->maxId + 1;
    int i;

    GenericInventoryType **tempItemsById = NULL;

    // create the name hash, if necessary
    assert(!pdict->haItemNames);
    pdict->haItemNames = stashTableCreateWithStringKeys( stashOptimalSize(eaSize(&pdict->ppItems)), stashShared(shared_memory) );

    // create the sorted inventory array, if necessary
    eaCreateWithCapacity(&tempItemsById, attribs->maxId);

    //each item knows where it belongs
    // so put it there.
    for( i = 0; i < eaSize( &pdict->ppItems ); i++)
    {
        GenericInventoryType *pItem = cpp_const_cast(GenericInventoryType*)(pdict->ppItems[i]);

        // --------------------
        // add the name and address to the names hash

        // if we find the hash value, assume that we're on a reload, and just skip incrementing this
        if (!stashAddPointerConst( pdict->haItemNames, pItem->name, pItem, false)) {
            continue;
        }

        // --------------------
        // determine the id

        if( attribs->idHash && stashFindInt( attribs->idHash, pItem->name, &pItem->id ) )
        {
            // EMPTY, id assigned implicitly
        }
        else
        {
            pItem->id = nextIdx++;
        }

        // --------------------
        // assign id to its sorted position

        // grow and set the position
        eaSetForced(&tempItemsById, pItem, pItem->id);
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
}


//------------------------------------------------------------
//  test if the power is a recipe
//----------------------------------------------------------
bool basepower_IsRecipe(BasePower const *p)
{
    return p && recipe_GetItem(p->pchName);
}

//----------------------------------------
//  get the enum string
//----------------------------------------
char *inventorytype_StrFromType(InventoryType t)
{
    char *res = NULL;
    if(inventorytype_Valid(t))
    {
        char *strs[] =
            {
                "kInventoryType_Salvage",
                "kInventoryType_Concept",
                "kInventoryType_Recipe",
                "kInventoryType_BaseDetail",
                "kInventoryType_StoredSalvage",
            };
        STATIC_INFUNC_ASSERT(ARRAY_SIZE( strs ) == kInventoryType_Count);
        res = strs[t];
    }
    return res;
}

SalvageInventoryItem *character_FindStoredSalvageInv(Character *c, char *name)
{
    SalvageInventoryItem *res = NULL;

    if( verify( c && name ))
    {
        SalvageInventoryItem **items = c->storedSalvageInv;
        int i;
        for( i = eaSize( &items ) - 1; i >= 0 && !res; --i)
        {
            if( items[i] && items[i]->salvage && 0 == stricmp(name, items[i]->salvage->pchName) )
                res = items[i];
        }
    }

    // --------------------
    // finally

    return res;
}

int character_AdjustStoredSalvage(Character *p, SalvageItem const *pSalvageItem, 
                                  int amount, const char *context, int bNoInvWarning )
{
    int res = -1;
    int count = 0;
    SalvageInventoryItem **salvages = p ? p->storedSalvageInv : NULL;

    if(!pSalvageItem)
        return res;
    
    res = s_FindSalvagePlace(salvages,pSalvageItem);
            
    if(EAINRANGE(res,salvages))
        count = salvages[res]->amount;
    
    if(amount > 0)
    {
        DATA_MINE_STOREDSALVAGE(pSalvageItem,"in",context,amount);
        
        if ( pSalvageItem->maxInvAmount != 0 && (U32)(count + amount) > pSalvageItem->maxInvAmount )
        {
            int actual_amount = pSalvageItem->maxInvAmount - count;
            DATA_MINE_STOREDSALVAGE(pSalvageItem,"out","inventory_full",amount - actual_amount);
            amount = actual_amount;
        }
    }
    else
    {
        if(-amount > count)
            amount = -count;
        if(amount)
            DATA_MINE_STOREDSALVAGE(pSalvageItem,"out",context,-amount);
    }

    // set the salvage and increment the amount
    genericinv_SetItem(p, (GenericInvItem***)&p->storedSalvageInv, kInventoryType_StoredSalvage, pSalvageItem, res, amount + count, context);

#if SERVER
    // did this fill up the inventory?
    if( !bNoInvWarning && amount > 0 && character_InventoryFull(p, kInventoryType_StoredSalvage) )
        serveFloater(p->entParent, p->entParent, "FloatStoredSalvageFull", 0.0f, kFloaterStyle_Attention, 0xff0000ff);
#endif

    return res;
}

bool character_CanAdjustStoredSalvage(Character *p, SalvageItem const *pSalvageItem, int amount )
{
    if( verify(p && pSalvageItem ))
    {
        SalvageInventoryItem **salvages = p->storedSalvageInv;
        int i;

        // check for overful inventory
        if(amount > 0 && salvageitem_IsInvention(pSalvageItem))
        {
            int totalSlots = character_GetInvTotalSize(p, kInventoryType_StoredSalvage);
            if (totalSlots >= 0 && p->storedSalvageInvCurrentCount + amount > totalSlots)
                return false;
        }

        for( i = eaSize( &salvages ) - 1; i >= 0; --i)
        {
            if( salvages[i]->salvage == pSalvageItem )
            {
                unsigned int newamount = salvages[i]->amount + amount;
                if (newamount < 0) return false; // We end up with less than 0, bad
                if (pSalvageItem->maxInvAmount == 0) return true; //0 means unlimited
                if (newamount <= pSalvageItem->maxInvAmount) return true;
                return false; //we end up with more than max, bad.
            }
        }
        if (amount < 0) return false; //don't remove item we dont have
        if (pSalvageItem->maxInvAmount == 0) return true;
        if ((unsigned int)amount > pSalvageItem->maxInvAmount) return false;
        return true;
    }
    return false;
}

int character_StoredSalvageEmptySlots(Character *p, SalvageItem const *pSalvageItem)
{
    int empty_slots = INT_MAX;

    if(verify(p && pSalvageItem))
    {
        SalvageInventoryItem **salvages = p->storedSalvageInv;
        int i;

        // check for overful inventory
        if(salvageitem_IsInvention(pSalvageItem))
        {
            int invention_empty_slots = character_GetInvTotalSize(p,kInventoryType_StoredSalvage) - p->storedSalvageInvCurrentCount;
            empty_slots = MIN(empty_slots,invention_empty_slots);
        }

        for( i = eaSize( &salvages ) - 1; i >= 0; --i)
        {
            if( salvages[i]->salvage == pSalvageItem )
            {
                if(pSalvageItem->maxInvAmount) //0 means unlimited
                {
                    int per_item_empty_slots = pSalvageItem->maxInvAmount - salvages[i]->amount;
                    empty_slots = MIN(empty_slots,per_item_empty_slots);
                }
                break;
            }
        }
        if(i < 0 && pSalvageItem->maxInvAmount) //0 means unlimited
        {
            int per_item_empty_slots = pSalvageItem->maxInvAmount;
            empty_slots = MIN(empty_slots,per_item_empty_slots);
        }
    }
    else
    {
        return 0;
    }

    return MAX(empty_slots,0);
}

int character_GetAdjustStoredSalvageCap(Character *p, SalvageItem const *pSalvageItem, int amount )
{
    while(!character_CanAdjustStoredSalvage(p,pSalvageItem,amount))
    {
        if(amount < 0)
            ++amount;
        else
            --amount;
    }
    return amount;
}

bool character_GetAdjustSalvageCap(Character *p, SalvageItem const *pSalvageItem, int amount )
{
    while(!character_CanAdjustSalvage(p,pSalvageItem,amount))
    {
        if(amount < 0)
            ++amount;
        else
            --amount;
    }
    return amount;
}

void character_SetSalvageInvCurrentCount(Character *p)
{
    int i;
    if(!p)
        return;
    p->salvageInvCurrentCount = 0;
    for( i = eaSize(&p->salvageInv) - 1; i >= 0; --i)
    {
        SalvageInventoryItem *s = p->salvageInv[i];
        if(!s)
            continue;
        if(salvageitem_IsInvention(s->salvage))
            p->salvageInvCurrentCount += s->amount;
    }
}

void character_SetStoredSalvageInvCurrentCount(Character *p)
{
    int i;
    if(!p)
        return;
    p->storedSalvageInvCurrentCount = 0;
    for( i = eaSize(&p->storedSalvageInv) - 1; i >= 0; --i)
    {
        SalvageInventoryItem *s = p->storedSalvageInv[i];
        if(!s || !s->salvage)
            continue;
        p->storedSalvageInvCurrentCount += s->amount;
    }
}

int character_StoredSalvageCount(Character *p, SalvageItem const *pSalvageItem)
{
        int res = 0;
    SalvageInventoryItem **salvages = p ? p->storedSalvageInv : NULL;

    if(!pSalvageItem)
        return 0;
    
    res = s_FindSalvagePlace(salvages,pSalvageItem);
            
    if(EAINRANGE(res,salvages))
        return salvages[res]->amount;

    return 0;
}
