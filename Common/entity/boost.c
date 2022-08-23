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

#include <utilitieslib/components/earray.h>

#include "entity/origins.h"
#include "entity/powers.h"
#include "entity/boost.h"
#include "entity/character_base.h"
#include <utilitieslib/components/MemoryPool.h>
#include "entity/boostset.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/scriptvars.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "account/AccountData.h"

#if CLIENT
#include "bases/DetailRecipe.h"
#include "UI/uiEnhancement.h"
#endif

SHARED_MEMORY float *g_pfBoostCombineChances;
SHARED_MEMORY float *g_pfBoostSameSetCombineChances;
SHARED_MEMORY float *g_pfBoostEffectivenessAbove;
SHARED_MEMORY float *g_pfBoostEffectivenessBelow;
SHARED_MEMORY float *g_pfBoostCombineBoosterChances;
SHARED_MEMORY float *g_pfBoostEffectivenessBoosters;

SHARED_MEMORY ExemplarHandicaps g_ExemplarHandicaps;

MP_DEFINE(Boost);

/**********************************************************************func*
 * boost_Create
 *
 */
Boost *boost_Create(const BasePower *ppowBase, int idx, Power *ppowParent,
                    int iLevel, int iNumCombines, float const *afVars, int numVars, PowerupSlot const *aPowerupSlots, int numPowerupSlots )
{
    Boost *pboost;
    MP_CREATE(Boost, 32);

    pboost = MP_ALLOC(Boost);

    pboost->ppowBase = ppowBase;
    pboost->ppowParent = ppowParent;
    pboost->idx = idx;
    pboost->iLevel = iLevel;
    pboost->iNumCombines = iNumCombines;
    pboost->pUI = NULL;
    pboost->ppowParentRespec = NULL;

    // copy the vars
    numVars = verify(numVars <= ARRAY_SIZE( pboost->afVars )) ? numVars : ARRAY_SIZE( pboost->afVars );
    ZeroStructs( pboost->afVars, ARRAY_SIZE(pboost->afVars));
    CopyStructs( pboost->afVars, afVars, numVars );
    
    // copy the structs
    numPowerupSlots = verify( numPowerupSlots <= ARRAY_SIZE( pboost->aPowerupSlots ) ) ? numPowerupSlots : ARRAY_SIZE( pboost->aPowerupSlots );
    ZeroStructs( pboost->aPowerupSlots, ARRAY_SIZE( pboost->aPowerupSlots ) );
    CopyStructs( pboost->aPowerupSlots, aPowerupSlots, numPowerupSlots );

    return pboost;
}

/**********************************************************************func*
* boost_Clone
*
*/
Boost *boost_Clone(const Boost *pboost)
{
     if( !pboost )
         return NULL;

     return boost_Create(pboost->ppowBase, pboost->idx, pboost->ppowParent, pboost->iLevel, pboost->iNumCombines, pboost->afVars, ARRAY_SIZE( pboost->afVars ), pboost->aPowerupSlots, ARRAY_SIZE( pboost->aPowerupSlots ));
}

/**********************************************************************func*
 * boost_Destroy
 *
 */
void boost_Destroy(Boost *pboost)
{

#if CLIENT && !TEST_CLIENT
    if (pboost && pboost->pUI != NULL)
        uiEnhancementFree(&(pboost->pUI));
#endif

    MP_FREE(Boost, pboost);
}

/**********************************************************************func*
 * boost_CombineChance
 *
 */
float boost_CombineChance(const Boost *pbA, const Boost *pbB)
{
    int iNum;
    const float *pCombineChances;

    int i = (pbA->iLevel+pbA->iNumCombines) - (pbB->iLevel+pbB->iNumCombines);
    i = (i<0 ? -i : i);

    if(pbA->ppowBase->psetParent==pbA->ppowBase->psetParent)
    {
        pCombineChances = g_pfBoostSameSetCombineChances;
    }
    else
    {
        pCombineChances = g_pfBoostCombineChances;
    }

    iNum = eafSize(&pCombineChances);

    if(i<iNum)
    {
        return pCombineChances[i];
    }

    return iNum>0 ? pCombineChances[iNum-1] : 0.0f;
}


/**********************************************************************func*
* boost_BoosterChance
*
*/
float boost_BoosterChance(const Boost *pbA)
{
    int iNum;

    if (!pbA)
        return 0.0f;

    iNum = eafSize(&g_pfBoostCombineBoosterChances);

    if(pbA->iNumCombines >= 0 && pbA->iNumCombines < iNum)
    {
        return g_pfBoostCombineBoosterChances[pbA->iNumCombines];
    }

    return 0.0f;
}


/**********************************************************************func*
* boost_BoosterEffectivenessLevelByCombines
*
*/
float boost_BoosterEffectivenessLevelByCombines(int numCombines)
{
    int iNum;

    iNum = eafSize(&g_pfBoostEffectivenessBoosters);

    if(numCombines >= 0 && numCombines < iNum)
    {
        return g_pfBoostEffectivenessBoosters[numCombines];
    }

    return 1.0f;
}

/**********************************************************************func*
* boost_BoosterEffectivenessLevel
*
*/
float boost_BoosterEffectivenessLevel(const Boost *pbA)
{
    if (!pbA || !pbA->ppowBase)
        return 0.0f;

    if (!pbA->ppowBase->bBoostBoostable)
        return 1.0f;

    return boost_BoosterEffectivenessLevelByCombines(pbA->iNumCombines);
}




/**********************************************************************func*
 * boost_EffectivenessLevel
 *
 */
float boost_EffectivenessLevel(int iBoostLevel, int iLevel)
{
    int iNum;
    const float *pfEff;

    int i = iBoostLevel - iLevel;

    if(i<0)
    {
        iNum = eafSize(&g_pfBoostEffectivenessBelow);
        pfEff = g_pfBoostEffectivenessBelow;
        i = -i;
    }
    else
    {
        iNum = eafSize(&g_pfBoostEffectivenessAbove);
        pfEff = g_pfBoostEffectivenessAbove;
    }

    if(i>=0 && i<iNum)
    {
        return pfEff[i];
    }

    return 0.0f;
}

/**********************************************************************func*
* boost_InventionLicenseLocked
*
*/
bool boost_InventionLicenseLocked(const Boost *pboost, Entity* e)
{
    if (!pboost || !pboost->ppowBase || !e || !e->pl)
        return false;

    return
        pboost &&
        pboost->ppowBase &&
        pboost->iLevel + 1 >= pboost->ppowBase->iBoostInventionLicenseRequiredLevel &&
        !AccountCanUseInvention(ent_GetProductInventory( e ), e->pl->account_inventory.accountStatusFlags);
}

/**********************************************************************func*
 * boost_Effectiveness
 *
 */

float boost_Effectiveness(const Boost *pboost, Entity* e, int iLevel, bool isBoostingAPet)
{
    // check for licenses
    if (e && e->pl && e->pl->account_inv_loaded && boost_InventionLicenseLocked(pboost, e))
        return 0.0f;
 
    if(pboost->ppowBase->bBoostIgnoreEffectiveness) 
    {
        if ((iLevel >= power_MinBoostUseLevel(pboost->ppowBase, pboost->iLevel) &&
             iLevel <= power_MaxBoostUseLevel(pboost->ppowBase, pboost->iLevel)) ||
            isBoostingAPet)
        {
            if (pboost->ppowBase->bBoostBoostable && pboost->iNumCombines > 0)
                return boost_BoosterEffectivenessLevel(pboost);
            else
                return 1.0f;
        }
        else
        {
            return 0.0f;
        }
    }
    else
    {
        return boost_EffectivenessLevel(pboost->iLevel+pboost->iNumCombines, iLevel);
    }
}

/**********************************************************************func*
 * boost_IsValidToSlotAtLevel
 *
 */
bool boost_IsValidToSlotAtLevel(const Boost *pboost, Entity* e, int iLevel, bool isBoostingAPet)
{
    return (pboost->ppowBase->iMinSlotLevel <= iLevel
            && pboost->ppowBase->iMaxSlotLevel >= iLevel
            && boost_Effectiveness(pboost, e, iLevel, isBoostingAPet) > 0.0f);
}

/**********************************************************************func*
 * power_MinBoostUseLevel
 *
 */
int power_MinBoostUseLevel(const BasePower *ppowBase, int iBoostLevel)
{
    if (ppowBase->bBoostUsePlayerLevel)
    {
        return ppowBase->iMinSlotLevel;
    }
    else if (ppowBase->bBoostIgnoreEffectiveness)
    {
//        return ppowBase->iMinSlotLevel;
        return MAX(ppowBase->iMinSlotLevel, iBoostLevel - (eafSize(&g_pfBoostEffectivenessAbove)-1));
    }
    else
    {
        return MAX(ppowBase->iMinSlotLevel, iBoostLevel - (eafSize(&g_pfBoostEffectivenessAbove)-1));
    }
}

/**********************************************************************func*
 * power_MaxBoostUseLevel
 *
 */
int power_MaxBoostUseLevel(const BasePower *ppowBase, int iBoostLevel)
{
    if (ppowBase->bBoostUsePlayerLevel)
    {
        return ppowBase->iMaxSlotLevel;
    }
    else if (ppowBase->bBoostIgnoreEffectiveness)
    {
        return ppowBase->iMaxSlotLevel;
//        return MIN(ppowBase->iMaxSlotLevel,iBoostLevel) + (eafSize(&g_pfBoostEffectivenessBelow)-1);
    }
    else
    {
        return MIN(ppowBase->iMaxSlotLevel,iBoostLevel) + (eafSize(&g_pfBoostEffectivenessBelow)-1);
    }
}

bool boost_IsOriginSpecific(const BasePower *ppowBase)
{
#define ORIGIN_COUNT 5
    int i;
    int origins[ORIGIN_COUNT] = {0, 0, 0, 0, 0};

    assert(eaSize(&g_CharacterOrigins.ppOrigins) == ORIGIN_COUNT);
    assert(ppowBase != NULL);

    for (i = eaiSize(&(int *) ppowBase->pBoostsAllowed) - 1; i >= 0; i--)
    {
        if (ppowBase->pBoostsAllowed[i] < ORIGIN_COUNT)
        {
            origins[ppowBase->pBoostsAllowed[i]] = 1;
        }
    }

    for (i = 0; i < ORIGIN_COUNT; i++)
    {
        if (!origins[i])
        {
            return 1;
        }
    }

    return 0;
#undef ORIGIN_COUNT
}

bool power_IsValidBoostMod(const Power *ppow, const AttribModTemplate *mod)
{
    int i;

    assert(ppow!=NULL);
    assert(mod!=NULL);

    if(!mod->boostModAllowed)
        return true;

    for(i=eaiSize(&(int *)ppow->ppowBase->pBoostsAllowed)-1; i>=0; i--)
    {
        if(ppow->ppowBase->pBoostsAllowed[i] == mod->boostModAllowed)
            return true;
    }

    return false;
}

/**********************************************************************func*
 * power_IsValidBoost
 *
 */
bool power_IsValidBoost(const Power *ppow, const Boost *pboost)
{
    int i, j;
    int bMatchesOrigin = false;
    int bMatchesType = false;

    assert(ppow!=NULL);
    assert(pboost!=NULL);

    // Can't slot boost set boosts into powers which don't use the set
    if(pboost->ppowBase->pBoostSetMember)
    {
        for(i=eaSize(&ppow->ppowBase->ppBoostSets)-1; i>=0; i--)
        {
            if(ppow->ppowBase->ppBoostSets[i] == pboost->ppowBase->pBoostSetMember)
            {
                break;
            }
        }

        if(i<0)
        {
            return false;
        }
    }

    for(i=eaiSize(&(int *)ppow->ppowBase->pBoostsAllowed)-1; i>=0; i--)
    {
        for(j=eaiSize(&(int *)pboost->ppowBase->pBoostsAllowed)-1; j>=0; j--)
        {
            if(pboost->ppowBase->pBoostsAllowed[j]<eaSize(&g_CharacterOrigins.ppOrigins))
            {
                if(stricmp(ppow->psetParent->pcharParent->porigin->pchName,
                    g_CharacterOrigins.ppOrigins[pboost->ppowBase->pBoostsAllowed[j]]->pchName)==0)
                {
                    bMatchesOrigin = true;
                }
            }
            else if(pboost->ppowBase->pBoostsAllowed[j] == ppow->ppowBase->pBoostsAllowed[i])
            {
                bMatchesType = true;
            }

            if(bMatchesType && bMatchesOrigin)
            {
                return true;
            }
        }
    }

    return false;
}

/**********************************************************************func*
 * power_IsValidBoostIgnoreOrigin
 *
 */
bool power_IsValidBoostIgnoreOrigin(const Power *ppow, const Boost *pboost)
{
    int i, j;

    assert(ppow!=NULL);
    assert(pboost!=NULL);

    for(i=eaiSize(&(int *)ppow->ppowBase->pBoostsAllowed)-1; i>=0; i--)
    {
        for(j=eaiSize(&(int *)pboost->ppowBase->pBoostsAllowed)-1; j>=0; j--)
        {
            if(pboost->ppowBase->pBoostsAllowed[j]>=eaSize(&g_CharacterOrigins.ppOrigins))
            {
                if(pboost->ppowBase->pBoostsAllowed[j] == ppow->ppowBase->pBoostsAllowed[i])
                {
                    return true;
                }
            }
        }
    }

    return false;
}

/**********************************************************************func*
 * boost_IsValidBoostForOrigin
 *
 */
bool boost_IsValidBoostForOrigin(const Boost *pboost, const BasePower *ppowBase, const CharacterOrigin *porigin)
{
    int j;

    assert(pboost!=NULL || ppowBase!=NULL);
    assert(porigin!=NULL);

    if(pboost)
    {
        ppowBase = pboost->ppowBase;
    }

    for(j=eaiSize(&(int *)ppowBase->pBoostsAllowed)-1; j>=0; j--)
    {
        if(ppowBase->pBoostsAllowed[j]<eaSize(&g_CharacterOrigins.ppOrigins))
        {
            if(stricmp(porigin->pchName,
                g_CharacterOrigins.ppOrigins[ppowBase->pBoostsAllowed[j]]->pchName)==0)
            {
                return true;
            }
        }
    }

    return false;
}

/**********************************************************************func*
 * power_ValidBoostSlotRequired
 *   Determines the specific slot where the given boost is required to be
 *  placed.  Assumes the boost is allowed in the power generally.  Returns 
 *  -1 if the boost can be placed in any slot.
 *
 */
int power_ValidBoostSlotRequired(const Power *ppow, const Boost *pboost)
{
    int i,j;
    const BoostSet *pset;

    assert(ppow!=NULL);
    assert(pboost!=NULL);

    pset = pboost->ppowBase->pBoostSetMember;
    if(pset)
    {
        const BoostList *plist = NULL;
        // Find the list that contains this boost
        for(i=eaSize(&pset->ppBoostLists)-1; i>=0; i--)
        {
//            plist = pset->ppBoostLists[i];
            for(j=eaSize(&pset->ppBoostLists[i]->ppBoosts)-1; j>=0; j--)
            {
                if(pboost->ppowBase == pset->ppBoostLists[i]->ppBoosts[j])
                {
                    plist = pset->ppBoostLists[i];
                    break;
                }
            }
            if(plist)
            {
                break;
            }
        }

        if(plist)
        {
            // Find a boost in the same list
            for(i=eaSize(&ppow->ppBoosts)-1; i>=0; i--)
            {
                if(ppow->ppBoosts[i] && ppow->ppBoosts[i]->ppowBase->pBoostSetMember == pboost->ppowBase->pBoostSetMember)
                {
                    for(j=eaSize(&plist->ppBoosts)-1; j>=0; j--)
                    {
                        if(ppow->ppBoosts[i]->ppowBase == plist->ppBoosts[j])
                        {
                            return i;
                        }
                    }
                }
            }
        }
    }

    // There are no boosts in the same list, so slot this anywhere
    return -1;
}

/**********************************************************************func*
 * boost_IsValidCombination
 *
 */
bool boost_IsValidCombination(const Boost *pboostA, const Boost *pboostB)
{
    int i, j;
    int bMatchesOrigin = false;
    int iMaxLevels = eafSize(&g_pfBoostEffectivenessAbove)-1;
    int iNumOrigins = eaSize(&g_CharacterOrigins.ppOrigins);
    int iCntA = 0;
    int iCntB = 0;
    const Boost *pboost;

    assert(pboostA!=NULL);
    assert(pboostB!=NULL);

    if(!(pboostA->ppowBase->bBoostCombinable && pboostB->ppowBase->bBoostCombinable))
    {
        return false;
    }

    if(pboostA->ppowParent!=NULL && pboostB->ppowParent!=NULL)
    {
        if(pboostA->ppowParent!=pboostB->ppowParent)
        {
            // If they are both in a power, they must be in the same power
            return false;
        }
    }
    else
    {
        Power *ppow = NULL;
        const Boost *pbCheck = NULL;

        if(pboostA->ppowParent!=NULL)
        {
            ppow = pboostA->ppowParent;
            pbCheck = pboostB;
        }
        else if(pboostB->ppowParent!=NULL)
        {
            ppow = pboostB->ppowParent;
            pbCheck = pboostA;
        }

        if(ppow==NULL)
        {
            // At least one of them must be in a power already.
            return false;
        }
        else if(!power_IsValidBoost(ppow, pbCheck))
        {
            // The new one must be valid in the power
            return false;
        }
    }

    // If we're comparing against ourselves, then just say we can do it.
    if(pboostB==pboostA)
    {
        return true;
    }

    if(pboostA->iLevel+pboostA->iNumCombines > pboostB->iLevel+pboostB->iNumCombines)
    {
        pboost  = pboostA;
        pboostA = pboostB;
        pboostB = pboost;
    }
    else if( pboostA->iLevel+pboostA->iNumCombines == pboostB->iLevel+pboostB->iNumCombines )
    {
        if( pboostA->iNumCombines < pboostB->iNumCombines )
        {
            pboost  = pboostA;
            pboostA = pboostB;
            pboostB = pboost;
        }
    }

    if(pboostB->iNumCombines>=BOOST_MAX_COMBINES)
    {
        // You can't combine boosts past their combine limit.
        return false;
    }

    // You must be able to actually improve when you combine.
    if(pboostA->iLevel+pboostA->iNumCombines >= pboostB->iLevel+pboostB->iNumCombines+1)
    {
        return false;
    }

    // first make sure one origin matches and that A and B have the same number
    // of origins
    for(j=eaiSize(&(int *)pboostB->ppowBase->pBoostsAllowed)-1; j>=0; j--)
    {
        if(pboostB->ppowBase->pBoostsAllowed[j] < iNumOrigins)
            iCntB++;
    }
    for(i=eaiSize(&(int *)pboostA->ppowBase->pBoostsAllowed)-1; i>=0; i--)
    {
        if(pboostA->ppowBase->pBoostsAllowed[i] < iNumOrigins)
        {
            iCntA++;

            for(j=eaiSize(&(int *)pboostB->ppowBase->pBoostsAllowed)-1; j>=0; j--)
            {
                if(pboostA->ppowBase->pBoostsAllowed[i] == pboostB->ppowBase->pBoostsAllowed[j]
                    && pboostB->ppowBase->pBoostsAllowed[j] < iNumOrigins)
                {
                    bMatchesOrigin = true;
                }
            }
        }
    }

    if(!bMatchesOrigin || iCntA!=iCntB)
        return false;

    // All of the non-origin-related BoostsAllowed have to be exactly the same
    //    for both boosts to match. They may have variant origin requirements.
    // BoostsAllowed is sorted numerically, and this code takes advantage of that.
    i = eaiSize(&(int *)pboostA->ppowBase->pBoostsAllowed)-1;
    j = eaiSize(&(int *)pboostB->ppowBase->pBoostsAllowed)-1;
    while(i>=0 && j>=0)
    {
        if(pboostA->ppowBase->pBoostsAllowed[i] < iNumOrigins)
        {
            i--;
            continue;
        }
        if(pboostB->ppowBase->pBoostsAllowed[j] < iNumOrigins)
        {
            j--;
            continue;
        }

        if(pboostA->ppowBase->pBoostsAllowed[i]!=pboostB->ppowBase->pBoostsAllowed[j])
        {
            return false;
        }

        i--;
        j--;
    }
    if(i!=j)
        return false;

    return true;
}


/**********************************************************************func*
* boost_IsBoostable
*
*/
bool boost_IsBoostable(Entity* e, const Boost *pboostA)
{
    if (pboostA == NULL || pboostA->ppowBase == NULL)
        return false;

    // can't boost if boost is invention locked
    if (boost_InventionLicenseLocked(pboostA, e))
        return false;

    // can't boost more than array size
    if (pboostA->iNumCombines + 1 >= eafSize(&g_pfBoostEffectivenessBoosters))
        return false;

    return pboostA->ppowBase->bBoostBoostable;
}


/**********************************************************************func*
* boost_IsCatalyzable
*
*/
bool boost_IsCatalyzable(const Boost *pboostA)
{
    bool retval = false;

    if (pboostA != NULL && pboostA->ppowBase != NULL && pboostA->ppowBase->pchBoostCatalystConversion != NULL)
    {
        const BasePower *base = powerdict_GetBasePowerByFullName(&g_PowerDictionary, pboostA->ppowBase->pchBoostCatalystConversion);
        Boost *tempBoost = NULL;

        if (!base)
            return false;

        if (pboostA->ppowParent != NULL)
        {
            // need to remove the original boost
            power_RemoveBoost(pboostA->ppowParent, pboostA->idx, "Temporary");
        }

        
        tempBoost = boost_Create(base, 0, NULL, pboostA->iLevel, 0, NULL, 0, NULL, 0);
        retval = power_BoostCheckRequires(pboostA->ppowParent, tempBoost);
        boost_Destroy(tempBoost);

        if (pboostA->ppowParent != NULL)
        {
            // need to replace the original boost
            power_ReplaceBoostFromBoost(pboostA->ppowParent, pboostA->idx, pboostA, "Temporary");
        }

        return retval;
    }

    return false;
}

/**********************************************************************func*
 * boost_HandicapExemplar
 *
 */
float boost_HandicapExemplar(int iCombatLevel, int iXPLevel, float fMagnitude)
{
    int n = eafSize(&g_ExemplarHandicaps.pfLimits)-1;
    int m = eafSize(&g_ExemplarHandicaps.pfHandicaps)-1;
    int o = eafSize(&g_ExemplarHandicaps.pfPreClamp)-1;
    int p = eafSize(&g_ExemplarHandicaps.pfPostClamp)-1;


    if(n<0 || m<0)
    {
        if(g_ulPowersDebug & 1) printf("Handicap: No limits or Handicaps defined.\n");
        return fMagnitude;
    }

    if(o>=0 && fMagnitude>g_ExemplarHandicaps.pfPreClamp[iCombatLevel>o ? o : iCombatLevel])
    {
        if(g_ulPowersDebug & 1) printf("Handicap: Magnitude (%f) > PreClamp[%d] (%f)\n", fMagnitude, iCombatLevel>o ? o : iCombatLevel, g_ExemplarHandicaps.pfPreClamp[iCombatLevel>o ? o : iCombatLevel]);
        fMagnitude = g_ExemplarHandicaps.pfPreClamp[iCombatLevel>o ? o : iCombatLevel];
    }

    if(fMagnitude>=g_ExemplarHandicaps.pfLimits[iCombatLevel>n ? n : iCombatLevel])
    {
        float fHigh;
        float fLow;


        fLow = g_ExemplarHandicaps.pfHandicaps[iCombatLevel>m ? m : iCombatLevel];
        fHigh = g_ExemplarHandicaps.pfHandicaps[iXPLevel>m ? m : iXPLevel];

        if(g_ulPowersDebug & 1) printf("Handicap: Low  = Handicap[%d] %f \n", iCombatLevel>m ? m : iCombatLevel, fLow);
        if(g_ulPowersDebug & 1) printf("Handicap: High = Handicap[%d] %f \n", iXPLevel>m ? m : iXPLevel, fHigh);
        if(g_ulPowersDebug & 1) printf("Handicap: Mag  = %f * %f \n", fMagnitude, fLow/fHigh);

        fMagnitude *= fLow/fHigh;

        if(g_ulPowersDebug & 1) printf("Handicap: Mag  = %f\n", fMagnitude);
    }

    if(p>=0 && fMagnitude>g_ExemplarHandicaps.pfPostClamp[iCombatLevel>p ? p : iCombatLevel])
    {
        if(g_ulPowersDebug & 1) printf("Handicap: Magnitude (%f) > PostClamp[%d] (%f)\n", fMagnitude, iCombatLevel>p ? p : iCombatLevel, g_ExemplarHandicaps.pfPostClamp[iCombatLevel>p ? p : iCombatLevel]);
        fMagnitude = g_ExemplarHandicaps.pfPostClamp[iCombatLevel>p ? p : iCombatLevel];
    }

    if(g_ulPowersDebug & 1) printf("Handicap: Final Magnitude = %f\n", fMagnitude);
    return fMagnitude;
}

//------------------------------------------------------------
// Finds the next valid location to put the passed rewardslot
//----------------------------------------------------------
bool boost_AddPowerupslot( Boost *b, const RewardSlot *slot )
{
    bool res = false;
    int i;

    if( verify( b && slot ))
    {    
        for( i = 0; i < ARRAY_SIZE(b->aPowerupSlots) && !res; ++i ) 
        {
            // NULL string will act as indicator of unused.
            if( !powerupslot_Valid(&b->aPowerupSlots[i]))
            {
                powerupslot_Init( &b->aPowerupSlots[i] );
                b->aPowerupSlots[i].reward = *slot;
                res = true;
            }
        }
    }
    
    // --------------------
    // finally
    
    return res;
}

/* End of File */


//------------------------------------------------------------
//  Helper. simple now, but may get bigger later
//----------------------------------------------------------
bool powerupslot_Valid( PowerupSlot const *slot )
{
    return slot && rewardslot_Valid(&slot->reward);
}

//------------------------------------------------------------
//  The count of contiguous valid powerupslots
//----------------------------------------------------------
int powerupslot_ValidCount(PowerupSlot const *slots, int size)
{
    int i = 0;
    if( verify( slots ))
    {
        for( i = 0; i < size && powerupslot_Valid( &slots[i] ) ; ++i ) 
        {
            // EMPTY
        }
    }
    // --------------------
    // finally
    
    return i;
}

PowerupSlot* powerupslot_Init( PowerupSlot *slot )
{
    if( verify(slot))
    {
        ZeroStruct(slot);
    }
    return slot;
}


