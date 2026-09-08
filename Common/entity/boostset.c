/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <stdlib.h>
#include <string.h>
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/error.h>

#include <utilitieslib/components/StashTable.h>
#include "entity/powers.h"
#include "entity/boostset.h"
#include "entity/boost.h"
#include <utilitieslib/components/earray.h>
#include "entity/character_base.h"
#include "entity/character_inventory.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/entity.h"
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/SharedHeap.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/file.h>
#include "account/AccountCatalog.h"

SHARED_MEMORY BoostSetDictionary g_BoostSetDict = { NULL, NULL, NULL, NULL, NULL };
SHARED_MEMORY ConversionSets g_ConversionSets = { NULL };

// Fixes the character's autopowers from boost set bonuses
static void boostset_FixAutoPowers(Character *c, const BasePower **ppAutos)
{
    int i;
    int iNumAutos = eaSize(&ppAutos);
    int *pCountOwned = NULL;
    int *pCountWanted = NULL;

    eaiCreate(&pCountOwned);
    eaiCreate(&pCountWanted);

    eaiSetSize(&pCountOwned, iNumAutos);
    eaiSetSize(&pCountWanted, iNumAutos);

    // Fills out the count and wanted arrays
    for(i=eaSize(&c->ppPowerSets)-1; i>=0; i--)
    {
        int j;
        for(j=eaSize(&c->ppPowerSets[i]->ppPowers)-1; j>=0; j--)
        {
            Power *ppow = c->ppPowerSets[i]->ppPowers[j];
            if(ppow)
            {
                int k;
                // Count owned
                if(ppow->ppowBase->eType==kPowerType_Auto)
                {
                    for(k=iNumAutos-1; k>=0; k--)
                    {
                        if(ppow->ppowBase==ppAutos[k])
                            pCountOwned[k]++;
                    }
                }

                // Count wanted
                if(ppow->ppBonuses)
                {
                    for(k=eaSize(&ppow->ppBonuses)-1; k>=0; k--)
                    {
                        int l;
                        for(l=eaSize(&ppow->ppBonuses[k]->ppAutoPowers)-1; l>=0; l--)
                        {
                            int m;
                            for(m=iNumAutos-1; m>=0; m--)
                            {
                                if(ppow->ppBonuses[k]->ppAutoPowers[l]==ppAutos[m])
                                    pCountWanted[m]++;
                            }
                        }
                    }
                }
            }
        }
    }

    // Add or remove as appropriate
    for(i=iNumAutos-1; i>=0; i--)
    {
        pCountWanted[i] -= pCountOwned[i];
        while(pCountWanted[i]>0)
        {
            Power *pPow = character_AddRewardPower(c,ppAutos[i]);

            // Assuming we didn't already have too many of this boost set power,
            // we should reset the level bought for this boost set power. It will
            // then always work as long as the enhancements in the set are still
            // active.
            if(pPow)
            {
                pPow->iLevelBought = 0;
            }
            pCountWanted[i]--;
        }
        while(pCountWanted[i]<0)
        {
            character_RemoveBasePower(c,ppAutos[i]);
            pCountWanted[i]++;
        }
    }

    eaiDestroy(&pCountOwned);
    eaiDestroy(&pCountWanted);
}

// Finds the boost list in pset that ppow is a member of, or NULL if ppow is not a member
static const BoostList *boostset_FindBoostInSet(const BasePower *ppow, const BoostSet *pset)
{
    int i;
    for(i=eaSize(&pset->ppBoostLists)-1; i>=0; i--)
    {
        int j;
        for(j=eaSize(&pset->ppBoostLists[i]->ppBoosts)-1; j>=0; j--)
        {
            if(ppow==pset->ppBoostLists[i]->ppBoosts[j])
                return pset->ppBoostLists[i];
        }
    }
    return NULL;
}


// Returns a list of valid boost set bonuses, or NULL if there are none
// Also fills pppAdjustAutos with autopowers that need to be checked
static const BoostSetBonus **boostset_FindBonuses(Character *c, Power *ppow, const BasePower ***pppAdjustAutos)
{
    int iEffLevel = c->iCombatLevel;
    const BoostSetBonus **ppFoundBonuses = NULL;
    const BasePower *ppowBase = ppow->ppowBase;
    int i;
    for(i=eaSize(&ppowBase->ppBoostSets)-1; i>=0; i--)
    {
        const BoostSet *pSet = ppowBase->ppBoostSets[i];
        int j;

        // Count the boosts we have for this set in this power
        int num = 0;
        BoostList **ppFoundLists = NULL;
        for(j=eaSize(&ppow->ppBoosts)-1; j>=0; j--)
        {
            // If this is a valid boost for this set
            if(ppow->ppBoosts[j]
               && (ppow->ppBoosts[j]->ppowBase->bBoostAlwaysCountForSet || 
                   boost_Effectiveness(ppow->ppBoosts[j], c->entParent, iEffLevel, c->entParent->erOwner) > 0.0f )
               && ppow->ppBoosts[j]->ppowBase->pBoostSetMember == pSet)
            {
                // Find the BoostList it's in
                const BoostList *plist = boostset_FindBoostInSet(ppow->ppBoosts[j]->ppowBase, pSet);
                if(plist)
                    eaPushUniqueConst(&ppFoundLists,plist);
            }
        }

        num = eaSize(&ppFoundLists);

        if (num == 0)
        {
            continue; // don't give any set bonuses if you have zero boosts in the set.
        }

        eaDestroy(&ppFoundLists);

        // Push all the valid bonuses into the list
        for(j=eaSize(&pSet->ppBonuses)-1; j>=0; j--)
        {
            const BoostSetBonus *pBonus = pSet->ppBonuses[j];

            if(num >= pBonus->iMinBoosts 
               && (!pBonus->iMaxBoosts || num <= pBonus->iMaxBoosts))
            {
                int k;
                int bAdd = true;
    
                if (pBonus->ppchRequires)
                    bAdd = power_sloteval_Eval(ppow, NULL, c->entParent, pBonus->ppchRequires);
 
                if (bAdd)
                {
                    // Push all the autopowers into the list
                    for(k=eaSize(&pSet->ppBonuses[j]->ppAutoPowers)-1; k>=0; k--)
                    {
                        eaPushUniqueConst(pppAdjustAutos,pSet->ppBonuses[j]->ppAutoPowers[k]);
                    }

                    // Push the bonus into the list
                    eaPushConst(&ppFoundBonuses,pSet->ppBonuses[j]);
                }
            }
        }
    }

    return ppFoundBonuses;
}

// Recompiles the list of valid bonuses for this power.  Also turns
//  off any autopowers that get removed in the process.
void boostset_RecompileBonuses(Character *c, Power *ppow, int deleteAllAutos)
{
    bool bRedoAutos = false;
    int i;
    BasePower **ppBonusPowers = NULL;
    const BasePower **ppVariedAutoPowers = NULL;
    const BoostSetBonus **ppNewBonuses = boostset_FindBonuses(c, ppow, &ppVariedAutoPowers);

    if (deleteAllAutos)
        ppNewBonuses = NULL;

    // Find old bonuses that no longer apply
    for(i=eaSize(&ppow->ppBonuses)-1; i>=0; i--)
    {
        int j;
        const BoostSetBonus *pOldBonus = ppow->ppBonuses[i];
        for(j=eaSize(&ppNewBonuses)-1; j>=0; j--)
        {
            if(pOldBonus == ppNewBonuses[j])
            {
                break;
            }
        }

        // Not in the new list, so remove the autopowers it gave
        if(j<0)
        {
            int k;
            for(k=eaSize(&pOldBonus->ppAutoPowers)-1; k>=0; k--)
            {
                eaPushUniqueConst(&ppVariedAutoPowers,pOldBonus->ppAutoPowers[k]);
            }
        }
    }

    // No set bonuses if they have enhancements turned off during a
    // taskforce or flashback.
    if(c->bIgnoreBoosts)
    {
        eaDestroyConst(&ppNewBonuses);
    }

    // Out with the old, in with the new
    eaDestroyConst(&ppow->ppBonuses);
    ppow->ppBonuses = ppNewBonuses;

    if(ppVariedAutoPowers)
    {
        boostset_FixAutoPowers(c,ppVariedAutoPowers);
        eaDestroyConst(&ppVariedAutoPowers);
    }
}






/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

static void addToConversionLists( BoostSetDictionary *pDict, const BoostSet * set )
{
    StashElement elem;
    int i, numGroups;
    BoostSet **list;
    BoostSet **new_list = NULL;

    if( !set || !set->ppchConversionGroups )
        return;

    numGroups = eaSize(&set->ppchConversionGroups);

    for (i = 0; i < numGroups; i++ )
    {
        if (!stashFindElementConst(pDict->htConversionLists, set->ppchConversionGroups[i], &elem))
        {
            // Create a new entry
            bool bStashed;

            eaCreate(&new_list);
            bStashed = stashAddPointerAndGetElementConst(pDict->htConversionLists, set->ppchConversionGroups[i], new_list, false, &elem);
            assert(bStashed);
        }

        // Add to the existing entry
        assert(elem);
        list = (BoostSet**)stashElementGetPointer(elem);
        eaPushConst(&list, set);
        stashElementSetPointer(elem, list);    // push can realloc and relocate list
    }
}

/**********************************************************************func*
* baseCreateTabLists
*
*/
static bool boostset_CreateConversionLists(BoostSetDictionary *pDict, bool shared_memory)
{
    bool ret = true;
    int i;
    int iNumSets = eaSize(&pDict->ppBoostSets);

    // build out the stash and the conversion lists completely in private memory
    // (shared stash tables can only be used with shared heap memory addresses so we can't
    // just build the conversion lists privately)
    assert(!pDict->htConversionLists);
    pDict->htConversionLists = stashTableCreateWithStringKeys(stashOptimalSize(iNumSets), stashShared(false));

    for(i=0; i<iNumSets; i++)
    {
        addToConversionLists( pDict, pDict->ppBoostSets[i] );
    }

    // if we are using shared memory we need to move the finalized tab lists
    // over to the shared heap and replace the stash table
    if (shared_memory)
    {
        cStashTable    dict_private = pDict->htConversionLists;
        cStashTableIterator iTabList = {0};
        StashElement elem;
        int i;

        // stash will resize if it gets 75% full so need to set intial size so that doesn't happen in shared memory
        pDict->htConversionLists = stashTableCreateWithStringKeys( stashOptimalSize(stashGetOccupiedSlots(dict_private)), stashShared(shared_memory));

        for( i = 0, stashGetIteratorConst( dict_private, &iTabList); stashGetNextElementConst(&iTabList, &elem); ++i ) 
        {
            Detail **list_private = (Detail**)stashElementGetPointer( elem );
            Detail **list_shared;

            eaCompressConst(&list_shared, &list_private, customSharedMalloc, NULL);
            stashAddPointerConst(pDict->htConversionLists, stashElementGetKey(elem), list_shared, false );
            eaDestroy(&list_private);
        }
        stashTableDestroyConst(dict_private);
    }

    return ret;
}

/////////////////////////////////////////////////////////////////////////////

// Fixed up the pointers in the powers
static void boostset_FixPointers(BoostSetDictionary *bdict, bool shared_memory)
{
    int i, j;

    for(i=eaSize(&bdict->ppBoostSets)-1; i>=0; i--)
    {
        BoostSet *pBoostSet = (BoostSet*)bdict->ppBoostSets[i];
        // Mark the powers that can use this set
        for(j=eaSize(&pBoostSet->ppPowers)-1; j>=0; j--)
        {
            const BasePower *ppow = pBoostSet->ppPowers[j];
            if(ppow)
            {
                eaPushUniqueConst(&ppow->ppBoostSets, pBoostSet);
#if SERVER
                //    Make sure the powers are in the same boost sets
                if (ppow->bPowerRedirector && isDevelopmentMode())
                {
                    int imod;
                    for(imod=0; imod<eaSize(&ppow->ppTemplates); imod++)
                    {
                        AttribModTemplate *ptemplate = cpp_const_cast(AttribModTemplate*)(ppow->ppTemplates[imod]);
                        if (ptemplate->offAttrib == kSpecialAttrib_PowerRedirect)
                        {
                            const BasePower *temp = powerdict_GetBasePowerByFullName(&g_PowerDictionary,ptemplate->ppchPrimaryStringList[0]);
                            int k;
                            for(k=eaSize(&pBoostSet->ppPowers)-1; k>=0; k--)
                            {
                                if (temp == pBoostSet->ppPowers[k])
                                {
                                    break;
                                }
                            }
                            if (k < 0)
                            {
                                ErrorFilenamef(ppow->pchSourceFile, "Power (%s) is boostable (%s), but its redirected power (%s) is not.", ppow->pchFullName, pBoostSet->pchName, temp->pchFullName);
                            }
                        }
                    }
                }
#endif
            }
            else
            {
                ErrorFilenamef("defs/boostsets/boostsets.def", "Boostset %s refers to a missing power. IF YOU HAVE DELETED A POWER IN USE LIKE THIS, THE GAME WILL BREAK HORRIBLY!", pBoostSet->pchName);
            }
        }
    }

    if(shared_memory)
    {
        // Move ppBoostSets to shared memory.  Loop thru all BasePowers to do the replacement
        for(i=eaSize(&g_PowerDictionary.ppPowers)-1; i>=0; i--)
        {
            const BasePower* basepower = g_PowerDictionary.ppPowers[i];
            assert(basepower);

            if(basepower->ppBoostSets)
            {
                BoostSet** pOldppboostSets = cpp_const_cast(BoostSet**)(basepower->ppBoostSets);
                eaCompressConst(&basepower->ppBoostSets, &basepower->ppBoostSets, customSharedMalloc, NULL);
                eaDestroy(&pOldppboostSets);
            }
        }
    }
}

static void ConversionListDestructor(const BoostSet **pContext)
{
    eaDestroyConst(&pContext);
}

static void ConversionListDestructorAdapter(void* arg0)
{
    ConversionListDestructor((const BoostSet **)arg0);
}

void boostset_DestroyDictHashes(BoostSetDictionary *bdict)
{
    if (bdict->haItemNames)
    {
        stashTableDestroyConst(bdict->haItemNames);
        bdict->haItemNames = NULL;
    }
    if (bdict->htBasePowerToGroupNameMap)
    {
        stashTableDestroyConst(bdict->htBasePowerToGroupNameMap);
        bdict->htBasePowerToGroupNameMap = NULL;
    }
    if (bdict->htBasePowerToSetNameMap)
    {
        stashTableDestroyConst(bdict->htBasePowerToSetNameMap);
        bdict->htBasePowerToSetNameMap = NULL;
    }

    if (bdict->htConversionLists)
    {
        stashTableDestroyEx(cpp_const_cast(StashTable)bdict->htConversionLists, NULL, ConversionListDestructorAdapter);
        bdict->htConversionLists = NULL;
    }

}

bool boostset_Finalize(BoostSetDictionary *bdict, bool shared_memory, const char * filename, bool bErrorOverwrite)
{
    bool ret = true;
    int i, j, k, numBoostSets, numPowerToGroups;

    boostset_FixPointers(bdict, shared_memory);

    assert(!bdict->haItemNames && !bdict->htBasePowerToGroupNameMap && !bdict->htBasePowerToSetNameMap);
    numBoostSets = eaSize(&bdict->ppBoostSets);
    bdict->haItemNames = stashTableCreateWithStringKeys(stashOptimalSize(numBoostSets), stashShared(shared_memory) );

    // accumulate size of powersToGroup/Name hash tables so can make optimal size
    numPowerToGroups = 0;
    for(i=0; i<numBoostSets; i++)
    {
        BoostSet *pBoostSet = (BoostSet*)bdict->ppBoostSets[i];
        for(j=eaSize(&pBoostSet->ppBoostLists)-1; j>=0; j--)
        {
            const BoostList *pBoostList = pBoostSet->ppBoostLists[j];
            numPowerToGroups += eaSize(&pBoostList->ppBoosts);
        }
        //default boosts for min and max level, if one is not set
        if (pBoostSet->iMinLevel == 0)
        {
            pBoostSet->iMinLevel = 10;
        }
        if (pBoostSet->iMaxLevel == 0)
        {
            pBoostSet->iMaxLevel = 50;
        }
    }
    bdict->htBasePowerToGroupNameMap = stashTableCreateWithStringKeys(stashOptimalSize(numPowerToGroups), stashShared(shared_memory));
    bdict->htBasePowerToSetNameMap = stashTableCreateWithStringKeys(stashOptimalSize(numPowerToGroups), stashShared(shared_memory));

    for(i=0; i<numBoostSets; i++)
    {
        BoostSet *pBoostSet = (BoostSet*)bdict->ppBoostSets[i];

        // Add boostset to hashtable
        if (!stashAddPointerConst(bdict->haItemNames, pBoostSet->pchName, pBoostSet, false))
        {
            ErrorFilenamef(filename, "Duplicate Boostset %s found.", pBoostSet->pchName);
            ret = false;
        }

        // Mark the boosts that are members of this set
        for(j=eaSize(&pBoostSet->ppBoostLists)-1; j>=0; j--)
        {
            const BoostList *pBoostList = pBoostSet->ppBoostLists[j];
            for(k=eaSize(&pBoostList->ppBoosts)-1; k>=0; k--)
            {
                BasePower *pBasePower = cpp_const_cast(BasePower*)(pBoostList->ppBoosts[k]);
                if (pBasePower)
                {
                    if(bErrorOverwrite 
                    && pBasePower->pBoostSetMember
                    && pBasePower->pBoostSetMember != pBoostSet)
                    {
                        Errorf("%s BoostSet membership changed from %s to %s",
                            pBasePower->pchFullName,
                            pBasePower->pBoostSetMember->pchName,
                            pBoostSet->pchName);
                    }

                    pBasePower->pBoostSetMember = pBoostSet;
                    mapBoostSetGroupNameToBasePowerName(pBasePower, bdict);
                }
            }
        }
    }

    boostset_CreateConversionLists(bdict, shared_memory);
    return ret;
}

void boostset_Validate(BoostSetDictionary *bdict, const char *pchFilename)
{
    int i;

    for(i=eaSize(&bdict->ppBoostSets)-1; i>=0; i--)
    {
        int j;
        const BoostSet *pBoostSet = (const BoostSet*)bdict->ppBoostSets[i];

        // Verify that all the boosts in a list have the same display name
        for(j=eaSize(&pBoostSet->ppBoostLists)-1; j>=0; j--)
        {
            int k;
            for(k=eaSize(&pBoostSet->ppBoostLists[j]->ppBoosts)-1; k>=1; k--)
            {
                if(0!=strcmp(pBoostSet->ppBoostLists[j]->ppBoosts[k]->pchDisplayName,
                             pBoostSet->ppBoostLists[j]->ppBoosts[0]->pchDisplayName))
                {
                    ErrorFilenamef(pchFilename, "BoostList %d of BoostSet %s has display name mismatch.", j, pBoostSet->pchName);
                }
            }
        }
    }
}

const BoostSet *boostset_FindByName(char *pName)
{
    cStashElement elt = NULL;

    if (stashFindElementConst( g_BoostSetDict.haItemNames, pName, &elt))
        return stashElementGetPointerConst(elt);
    else
        return NULL;
}

int boostset_CompareName(const BoostSet **a, const BoostSet **b, const void *context)
{
    if (*a == NULL)
        return -1;
    if (*b == NULL)
        return 1;

    return stricmp(textStd((*a)->pchName), textStd((*b)->pchName));
}

int boostset_Compare(const BoostSet **a, const BoostSet **b, const void *context)
{
    int retval;
    
    if (*a == NULL)
        return -1;
    if (*b == NULL)
        return 1;

    if((*a)->pchGroupName == NULL || (*b)->pchGroupName == NULL )
    {    
        if((*a)->pchGroupName == NULL && (*b)->pchGroupName != NULL )
            return 1;
        if((*a)->pchGroupName != NULL && (*b)->pchGroupName == NULL )
            return -1;
        return stricmp(textStd((*a)->pchDisplayName), textStd((*b)->pchDisplayName));
    }

    retval = stricmp(textStd((*a)->pchGroupName), textStd((*b)->pchGroupName));
    if (retval == 0)
        retval = stricmp(textStd((*a)->pchDisplayName), textStd((*b)->pchDisplayName));

    return retval;
}

const ConversionSetCost *boostset_findConversionSet(const char *setName)
{
    int i, size;

    size = eaSize(&g_ConversionSets.ppConversionSetCosts);
    for (i = 0; i < size; i++)
    {
        if (g_ConversionSets.ppConversionSetCosts[i] != NULL && 
            g_ConversionSets.ppConversionSetCosts[i]->pchConversionSetName != NULL &&
            stricmp(g_ConversionSets.ppConversionSetCosts[i]->pchConversionSetName, setName) == 0)
        {
            return g_ConversionSets.ppConversionSetCosts[i];
        }
    }

    return NULL;
}

static const BasePower *boostset_FindConversionInSet(const BoostSet *pSet, const BasePower *pExclude, bool allowAttuned)
{
    int i, j, numLists, numBoosts, count;
    BasePower **pickList = NULL;
    const BasePower *retval = NULL;

    eaCreate(&pickList);

    numLists = eaSize(&pSet->ppBoostLists);
    count = 0;

    for (i = 0; i < numLists; i++)
    {
        numBoosts = eaSize(&pSet->ppBoostLists[i]->ppBoosts);
        for (j = 0; j < numBoosts; j++)
        {
            if (pSet->ppBoostLists[i]->ppBoosts[j] != pExclude && 
                pSet->ppBoostLists[i]->ppBoosts[j] != NULL &&
                (!pSet->ppBoostLists[i]->ppBoosts[j]->bBoostAccountBound || allowAttuned))
                eaPush(&pickList, cpp_reinterpret_cast(BasePower *)pSet->ppBoostLists[i]->ppBoosts[j]);
        }
    }

    if (eaSize(&pickList) > 0)
        retval = pickList[randInt(eaSize(&pickList))];
    
    eaDestroy(&pickList);

    return retval;
}

Boost * boostSet_getBoostsetIfConversionParametersVerified(Character * c, int idx, const char * conversionSet)
{
    Boost *pBoost = NULL;
    if (!c || !conversionSet || idx < 0 || idx >= CHAR_BOOST_MAX || c->aBoosts[idx] == NULL)
        return NULL;

    pBoost = c->aBoosts[idx];
    if (!pBoost->ppowBase || !pBoost->ppowBase->pBoostSetMember)
        return NULL;

    return pBoost;
}

static int checkBoostSetValidConversion(Entity * e, BoostSet * pBoostSet, int level)
{
    return (pBoostSet->iMinLevel <= level + 1 &&   // (+1) since level==0 for first level
            pBoostSet->iMaxLevel >= level + 1 &&
            (!pBoostSet->pchStoreProduct || AccountHasStoreProductOrIsPublished(ent_GetProductInventory(e), SKU(pBoostSet->pchStoreProduct))));
}

int boostset_hasValidConversion(Entity * e, const char *conversionSet, int level)
{
    BoostSet **ppConversionSet;
    int i, size, validCount = 0;

    if (!stashFindPointerConst(g_BoostSetDict.htConversionLists, conversionSet, cpp_reinterpret_cast(const BoostSet**)(&ppConversionSet)))
        return 0;

    size = eaSize(&ppConversionSet);
    for (i = 0; i < size; ++i)
    {
        if (checkBoostSetValidConversion(e, ppConversionSet[i], level))
        {
            ++validCount;
        }
    }

    return validCount > 1;  //can only convert to itself
}

int boostset_Convert(Entity * e, int idx, const char *conversionSet)
{
    Boost *pBoost = NULL;
    const ConversionSetCost *pSet = NULL;
    int i, size, valid;
    BoostSet **ppConversionSet;
    const BasePower *newBoost = NULL;
    int level, numCombines;
    const SalvageItem* pConverters = salvage_GetItem( "S_EnhancementConverter" );
    int cost = 0;
    int newBoostIdx = -1;
    Character * pchar = e->pchar;

    // is there a boost in the location?
    pBoost = boostSet_getBoostsetIfConversionParametersVerified(pchar, idx, conversionSet);
    if (!pBoost)
        return newBoostIdx;

    level = pBoost->iLevel;
    numCombines = pBoost->iNumCombines;

    // check for in set conversion
    if (stricmp("In", conversionSet) == 0)
    {
        const BoostSet *pBoostSet = pBoost->ppowBase->pBoostSetMember;

        if (pBoost->ppowBase->pBoostSetMember->ppchConversionGroups != NULL &&
            pBoost->ppowBase->pBoostSetMember->ppchConversionGroups[0] != NULL)
            pSet = boostset_findConversionSet(pBoost->ppowBase->pBoostSetMember->ppchConversionGroups[0]);

        if (!pSet)
            return newBoostIdx;

        // is this enhancement attunded and does this conversion set allow attuned?
        if (pBoost->ppowBase->bBoostUsePlayerLevel && !pSet->allowAttuned)
            return newBoostIdx;

        cost = pSet->tokensInSet;
        newBoost = boostset_FindConversionInSet(pBoostSet, pBoost->ppowBase, pSet->allowAttuned);        
    } else {
        BoostSet **pickList;

        if (!stashFindPointerConst(g_BoostSetDict.htConversionLists, conversionSet, cpp_reinterpret_cast(const BoostSet**)(&ppConversionSet)))
            return newBoostIdx;

        // is the conversion set allowed for this enhancement?
        valid = false;
        size = eaSize(&pBoost->ppowBase->pBoostSetMember->ppchConversionGroups);
        for (i = 0; i < size && !valid; i++)
        {
            if (stricmp(conversionSet, pBoost->ppowBase->pBoostSetMember->ppchConversionGroups[i]) == 0)
                valid = true;
        }

        if (!valid)
            return newBoostIdx;

        // does this conversion set exists
        pSet = boostset_findConversionSet(conversionSet);
        if (!pSet)
            return newBoostIdx;

        // is this enhancement attunded and does this conversion set allow attuned?
        if (pBoost->ppowBase->bBoostUsePlayerLevel && !pSet->allowAttuned)
            return newBoostIdx;


        cost = pSet->tokensOutSet;

        eaCreate(&pickList);
        size = eaSize(&ppConversionSet);
        for (i = 0; i < size; i++)
        {
            if (ppConversionSet[i] != pBoost->ppowBase->pBoostSetMember &&
                checkBoostSetValidConversion(e, ppConversionSet[i], pBoost->iLevel))
            {
                eaPush(&pickList, ppConversionSet[i]);
            }
        }

        if (eaSize(&pickList) > 0)
            newBoost = boostset_FindConversionInSet(pickList[randInt(eaSize(&pickList))], pBoost->ppowBase, pSet->allowAttuned);

        eaDestroy(&pickList);
    }

    if (newBoost == NULL)
        return newBoostIdx;


    // check for required converters
    if (character_SalvageCount(pchar, pConverters) < cost)
        return newBoostIdx;

    // do the actual switch
    character_DestroyBoost( pchar, idx, "conversion" );
    newBoostIdx = character_AddBoostSpecificIndex( pchar, newBoost, level, numCombines, "conversion", idx);
    character_AdjustSalvage(pchar, pConverters, -cost, "conversion", true);

    return newBoostIdx;
}


/* End of File */
