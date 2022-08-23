/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>

#include <utilitieslib/components/earray.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/classes.h"
#include "entity/origins.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/powers.h"
#include <utilitieslib/utils/mathutil.h>
#include "account/AccountInventory.h"

#if SERVER
#include "player/badges_server.h"
#include "entity/buddy_server.h"
#include "dbcomm/dbcomm.h"    // for dbLog() messages
#include "dbcomm/shardcomm.h"
#include "entity/entPlayer.h"
#include "auth/authUserData.h"
#include "entity/power_system.h"
#include "player/pophelp.h"
#include "storyarc/storyarcinterface.h"
#include "dbcomm/logcomm.h"
#endif

// A spot for all the schedules.
SHARED_MEMORY Schedules g_Schedules;

// All the XP levelling info
SHARED_MEMORY ExperienceTables g_ExperienceTables;

int gArchitectMapMinLevel;
int gArchitectMapMaxLevel;


/**********************************************************************func*
 * character_SystemGetLevel
 *
 */
int character_GetLevel(Character *p )
{
    return p->iLevel;
}
/**********************************************************************func*
 * character_PowersGetMaximumLevel
 *
 */
int character_PowersGetMaximumLevel(Character *p)
{
    int i;
    int iLevel;
    
    if (!p)
        return 0;

    iLevel = p->iLevel;

    for (i = 0; i < MAX_BUILD_NUM; i++)
    {
        if (p->iBuildLevels[i] > iLevel)
        {
            iLevel = p->iBuildLevels[i];
        }
    }
    return iLevel;
}

/**********************************************************************func*
 * character_SystemSetLevelInternal
 *
 */
void character_SetLevelInternal(Character *p, int iVal)
{
    p->iLevel = iVal;
}

/**********************************************************************func*
 * character_SystemGetExperiencePoints
 *
 */
int character_GetExperiencePoints(Character *p)
{
    return p->iExperiencePoints;
}

/**********************************************************************func*
 * character_SystemAddExperiencePoints
 *
 */
int character_AddExperiencePoints(Character *p, int ival)
{
    p->iExperiencePoints += ival;
    return p->iExperiencePoints;
}

/**********************************************************************func*
 * character_SystemSetExperiencePoints
 *
 */
void character_SetExperiencePoints(Character *p, int ival)
{
    p->iExperiencePoints = ival;

}

/**********************************************************************func*
 * character_SystemGetExperienceDebt
 *
 */
int character_GetExperienceDebt(Character *p)
{
    return p->iExperienceDebt;
}

/**********************************************************************func*
 * character_SystemSetExperienceDebt
 *
 */
void character_SetExperienceDebt(Character *p, int iVal)
{
    p->iExperienceDebt = iVal;
}

/**********************************************************************func*
 * character_SystemAddExperienceDebt
 *
 */
int character_AddExperienceDebt(Character *p, int iVal)
{
    p->iExperienceDebt += iVal;
    return p->iExperienceDebt;
}

/**********************************************************************func*
 * character_SystemCanBuyPowerForLevel
 *
 */
bool character_CanBuyPowerForLevel(Character *p, int iLevel)
{
    assert(p!=NULL);

    return character_CountPowersBought(p)
         < CountForLevel(iLevel, g_Schedules.aSchedules.piPower);
}

/**********************************************************************func*
 * character_SystemCountPowersForLevel
 *
 */
int character_CountPowersForLevel(Character *p, int iLevel)
{
    assert(p!=NULL);

    return CountForLevel(iLevel, g_Schedules.aSchedules.piPower) - character_CountPowersBought(p);
}

/**********************************************************************func*
 * character_CalcExperienceLevel
 *
 */
int character_CalcExperienceLevel(Character *p)
{
    int iLevel;
    int iDiff=0;

    assert(p!=NULL);

    iLevel = character_GetLevel(p);

    if(p->entParent && ENTTYPE(p->entParent)==ENTTYPE_PLAYER)
    {
        int iExp = character_GetExperiencePoints(p);

        while(iLevel < eaiSize(&g_ExperienceTables.aTables.piRequired)
            && (iDiff = g_ExperienceTables.aTables.piRequired[iLevel] - iExp)<=0)
        {
            iLevel++;
        }

        if(iDiff>=0 && iLevel>0)
        {
            iLevel--;
        }
    }

    return iLevel;
}

#if SERVER
/**********************************************************************func*
 * character_SystemCapExperienceDebt
 *
 */
bool character_CapExperienceDebt(Character *p)
{
    int iCap;
    bool bCapped = false;
    int iExpDebt = character_GetExperienceDebt(p);
    int iExpLevel = character_CalcExperienceLevel(p);

    if (iExpLevel >= eaiSize(&g_ExperienceTables.aTables.piDefeatPenalty))
        iExpLevel = eaiSize(&g_ExperienceTables.aTables.piDefeatPenalty)-1;

    // Cap debt to 5 times the penalty levied.
    iCap = g_ExperienceTables.aTables.piDefeatPenalty[iExpLevel]*5;

    if(iExpDebt>iCap)
    {
        iExpDebt = iCap;
        bCapped = true;
    }
    if(iExpDebt<0)
        iExpDebt = 0;

    character_SetExperienceDebt(p, iExpDebt);

    return bCapped;
}
#endif

/**********************************************************************func*
 * character_SystemExperienceNeeded
 *
 */
int character_ExperienceNeeded(Character *p)
{
    int iLevel;
    int iExp;
    int iDiff;

    assert(p!=NULL);

    iLevel = character_GetLevel(p);
    iExp = character_GetExperiencePoints(p);

    while(iLevel < MAX_PLAYER_LEVEL    && (iDiff = g_ExperienceTables.aTables.piRequired[iLevel] - iExp)<=0)
    {
        iLevel++;
    }

    if(iDiff<0)
    {
        iDiff = 0;
    }

    return iDiff;
}

/**********************************************************************func*
* character_SystemAdditionalExperienceNeededForLevel
*
*/
int character_AdditionalExperienceNeededForLevel(Character *p, int iLevel)
{
    if (iLevel < MAX_PLAYER_LEVEL)
    {
        if (iLevel < 1)
            return 0;

        return g_ExperienceTables.aTables.piRequired[iLevel] - g_ExperienceTables.aTables.piRequired[iLevel-1];
    } 
    else
    {
        return 0;
    }
}

/**********************************************************************func*
 * character_SystemCountPoolPowerSetsBought
 *
 */
int character_CountPoolPowerSetsBought(Character *p)
{
    int i;
    int iCnt = 0;

    assert(p!=NULL);

    for(i=eaSize(&p->ppPowerSets)-1; i>=0; i--)
    {
        PowerSet *pset = p->ppPowerSets[i];
        if( pset->psetBase->pcatParent == p->pclass->pcat[kCategory_Pool])
        {
            iCnt++;
        }
    }

    return iCnt;
}

/**********************************************************************func*
 * character_SystemCanBuyPoolPowerSetForLevel
 *
 */
bool character_CanBuyPoolPowerSetForLevel(Character *p, int iLevel)
{
    assert(p!=NULL);

    return character_CountPoolPowerSetsBought(p) < (CountForLevel(iLevel, g_Schedules.aSchedules.piPoolPowerSet));
}

/**********************************************************************func*
 * character_CanBuyPoolPowerSet
 *
 */
bool character_CanBuyPoolPowerSet(Character *p)
{
    assert(p!=NULL);

    return character_CanBuyPoolPowerSetForLevel(p, character_GetLevel(p));
}

/**********************************************************************func*
 * character_SystemCountEpicPowerSetsBought
 *
 */
int character_CountEpicPowerSetsBought(Character *p)
{
    int i;
    int iCnt = 0;

    assert(p!=NULL);

    for(i=eaSize(&p->ppPowerSets)-1; i>=0; i--)
    {
        PowerSet *pset = p->ppPowerSets[i];
        if( pset->psetBase->pcatParent == p->pclass->pcat[kCategory_Epic])
        {
            iCnt++;
        }
    }

    return iCnt;
}

/**********************************************************************func*
 * character_SystemCanBuyEpicPowerSetForLevel
 *
 */
bool character_CanBuyEpicPowerSetForLevel(Character *p, int iLevel)
{
    assert(p!=NULL);

    return character_CountEpicPowerSetsBought(p)
        < (CountForLevel(iLevel, g_Schedules.aSchedules.piEpicPowerSet));
}

/**********************************************************************func*
 * character_SystemCanBuyEpicPowerSet
 *
 */
bool character_CanBuyEpicPowerSet(Character *p)
{
    assert(p!=NULL);

    return character_CanBuyEpicPowerSetForLevel(p, character_GetLevel(p));
}

/**********************************************************************func*
 * character_SystemCountBoostsBought
 *
 */
int character_CountBoostsBought(Character *p)
{
    int i;
    int iCnt = 0;

    assert(p!=NULL);

    for(i=eaSize(&p->ppPowerSets)-1; i>=0; i--)
    {
        int j;
        PowerSet *pset = p->ppPowerSets[i];

        for(j=eaSize(&pset->ppPowers)-1; j>=0; j--)
        {
            if(pset->ppPowers[j] != NULL)
            {
                iCnt += pset->ppPowers[j]->iNumBoostsBought;
            }
        }
    }

    return iCnt;
}

/**********************************************************************func*
 * character_SystemCanBuyBoostForLevel
 *
 */
bool character_CanBuyBoostForLevel(Character *p, int iLevel)
{
    assert(p!=NULL);

    return character_CountBoostsBought(p) <    CountForLevel(iLevel, g_Schedules.aSchedules.piAssignableBoost);
}

/**********************************************************************func*
* character_SystemCountBuyBoostForLevel
*
*/
int character_CountBuyBoostForLevel(Character *p, int iLevel)
{
    assert(p!=NULL);

    return CountForLevel(iLevel, g_Schedules.aSchedules.piAssignableBoost)-character_CountBoostsBought(p);
}


/**********************************************************************func*
 * character_SystemCanBuyBoost
 *
 */
bool character_CanBuyBoost(Character *p)
{
    assert(p!=NULL);

    return character_CanBuyBoostForLevel(p, character_GetLevel(p));
}

/**********************************************************************func*
 * character_SystemCountPowersBought
 *
 */
int character_CountPowersBought(Character *p)
{
    int i, j;
    int iCnt = 0;

    assert(p!=NULL);

    // Temp powers are all free, will fail this
    for(i=eaSize(&p->ppPowerSets)-1; i>=0; i--)
    {
        PowerSet *pset = p->ppPowerSets[i];

        for(j=eaSize(&pset->ppPowers)-1; j>=0; j-- )
        {
            if(pset->ppPowers[j] != NULL
                && !pset->ppPowers[j]->ppowBase->bFree)
            {
                iCnt++;
            }
        }
    }
    return iCnt;
}

/**********************************************************************func*
 * character_SystemCanBuyPower
 *
 */
bool character_CanBuyPower(Character *p)
{
    assert(p!=NULL);

    return character_CanBuyPowerForLevel(p, character_GetLevel(p));
}


/**********************************************************************func*
 * character_SystemSetLevel
 *
 */
void character_SetLevel(Character* p, int iLevel)
{
    assert(p!=NULL);

    if(iLevel < 0)
        return;

    if(iLevel<eaiSize(&g_ExperienceTables.aTables.piRequired))
    {
        character_SetLevelInternal(p, iLevel);
    }
    else
    {
        character_SetLevelInternal(p, eaiSize(&g_ExperienceTables.aTables.piRequired)-1);
    }

#if SERVER
    badge_RecordLevelChange(p->entParent);
#endif

    character_LevelFinalize(p, true);
    character_Reset(p, true);
}

/**********************************************************************func*
 * character_SystemCalcExperienceLevelByExp
 *
 */
int character_CalcExperienceLevelByExp(int iExperience)
{
    int iLevel=0;
    int iDiff=0;

    if(iExperience < 0)
        return 0;

    while(iLevel<eaiSize(&g_ExperienceTables.aTables.piRequired)
        && (iDiff=g_ExperienceTables.aTables.piRequired[iLevel]-iExperience)<=0)
    {
        iLevel++;
    }

    if(iDiff>0 && iLevel>0)
    {
        iLevel--;
    }

    return iLevel;
}

/**********************************************************************func*
 * character_SystemCanLevel
 *
 */
bool character_CanLevel(Character *p)
{
    assert(p!=NULL);

    return (character_CalcExperienceLevel(p) > character_GetLevel(p));
}

#ifdef SERVER

/**********************************************************************func*
 * character_SystemApplyDefeatPenalty
 *
 */
void character_ApplyDefeatPenalty(Character *p, float fDebtPercentage)
{
    int iPenalty;
    int iExpLevel = character_CalcExperienceLevel(p);
    int iOrigDebt = character_GetExperienceDebt(p);
    int iMaxLevel = eaiSize(&g_ExperienceTables.aTables.piDefeatPenalty);
    bool bCapped;

    LOG_ENT(p->entParent, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Character:AddDebt Initial debt percent is %f", fDebtPercentage );

    if(iExpLevel >= iMaxLevel)
    {
        iExpLevel=iMaxLevel-1;
    }

    iPenalty = (int)(g_ExperienceTables.aTables.piDefeatPenalty[iExpLevel]*fDebtPercentage);

    // modify debt by removing any accrued 
    if (iPenalty > 0 && p->iExperienceRest > 0)
    {
        LOG_ENT(p->entParent, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Character:AddDebt Removing rest XP: removing %d from %d", iPenalty, p->iExperienceRest);
        if (iPenalty > p->iExperienceRest)
        {
            badge_RecordExperienceDebt(p->entParent, p->iExperienceRest);            // give credit for debt being awarded
            badge_RecordExperienceDebtRemoved(p->entParent, p->iExperienceRest);    // and removed
            iPenalty -= p->iExperienceRest;
            p->iExperienceRest = 0;
        } else {
            badge_RecordExperienceDebt(p->entParent, iPenalty);                        // give credit for debt being awarded
            badge_RecordExperienceDebtRemoved(p->entParent, iPenalty);                // and removed
            p->iExperienceRest -= iPenalty;
            iPenalty = 0;
        }
        LOG_ENT(p->entParent, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Character:AddDebt Removing rest XP: need to remove %d more, rest XP now %d", iPenalty, p->iExperienceRest);
    }

    if (iPenalty > 0)
    {
        character_AddExperienceDebt(p, iPenalty);
        bCapped = character_CapExperienceDebt(p);

        if(iPenalty!=0)
        {
            badge_RecordExperienceDebt(p->entParent, character_GetExperienceDebt(p)-iOrigDebt);
            LOG_ENT(p->entParent, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Character:AddDebt Adding %d debt, total debt now %d%s", iPenalty, character_GetExperienceDebt(p), bCapped?" (capped)":"");
        }
    }
}

/**********************************************************************func*
 * character_SystemGiveExperience and related functions
 *
 */
void character_ApplyAndReduceDebt(Character *p, int *piReceived, int *piDebtWorkedOff)
{
    int iNewDebt;

    *piDebtWorkedOff = *piReceived - *piReceived/2;
    *piReceived /= 2;

    iNewDebt = character_GetExperienceDebt(p) - *piDebtWorkedOff;
    if(iNewDebt < 0)
    {
        *piDebtWorkedOff += iNewDebt;    // subtract off the extra
        *piReceived -= iNewDebt;        // add on the extra
        iNewDebt = 0;
    }

    character_SetExperienceDebt(p, iNewDebt);

    if(*piDebtWorkedOff)
    {
        badge_RecordExperienceDebtRemoved(p->entParent, *piDebtWorkedOff);

        if (getPopHelpStateByTag(p->entParent, "CODEPH_Used_XP_Debt") == PopHelpState_Untriggered)
        {
            triggerPopHelpEventHappenedByTag(p->entParent, "CODEPH_Used_XP_Debt");
        }
    }
}
int character_GiveExperienceNoDebt(Character *p, int iReceived)
{
    int iCurXP = character_GetExperiencePoints(p);
    int iMaxLevel = MAX_PLAYER_LEVEL;

    // Don't let them get too much XP
    MINMAX1(iReceived, 0, g_ExperienceTables.aTables.piRequired[iMaxLevel-1] - iCurXP);

    // If the character somehow has more XP than the max for their level
    // the minmax can end up negative. This check ensures  we never, ever\
    // reduce their XP total
    if(iReceived > 0)
    {
        character_AddExperiencePoints(p, iReceived);
        p->iCombatLevel = character_CalcCombatLevel(p);
        character_HandleCombatLevelChange(p);
        badge_RecordExperience(p->entParent, iReceived);
    }

    return iReceived;
}

void character_GiveExperience(Character *p, int iReceived, int *piActuallyReceived, int *piDebtWorkedOff)
{
    character_ApplyAndReduceDebt(p, &iReceived, piDebtWorkedOff);
    *piActuallyReceived = character_GiveExperienceNoDebt(p, iReceived);
}

int character_IncreaseExperienceNoDebt(Character *p, int iIncreaseTo)
{
    return character_GiveExperienceNoDebt(p, iIncreaseTo - character_GetExperiencePoints(p));
}

#endif // SERVER


/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/


/**********************************************************************func*
 * character_LevelFinalize
 *
 */
void character_LevelFinalize(Character *p, bool grantAutoIssuePowers)
{
    int iInspLevel;

    assert(p!=NULL);
    assert(p->pclass!=NULL);

    // Force them to have the min required number of XP.
    // This lets us forcibly and safely level someone.

    if(ENTTYPE(p->entParent)==ENTTYPE_PLAYER)
    {
        int iLevel = character_GetLevel(p);
        int iXP = character_GetExperiencePoints(p);

        // Make sure their level isn't above the max defined.
        if(iLevel >= MAX_PLAYER_LEVEL)
        {
            iLevel = MAX_PLAYER_LEVEL-1;
            character_SetLevel(p, MAX_PLAYER_LEVEL-1);
        }

        if(iXP < g_ExperienceTables.aTables.piRequired[iLevel])
        {
            // If for some reason they don't have enough XP for their level,
            // increase it to the minimum needed.
            character_SetExperiencePoints(p, g_ExperienceTables.aTables.piRequired[iLevel]);
        }
        else if(iXP > g_ExperienceTables.aTables.piRequired[MAX_PLAYER_LEVEL-1])
        {
            // If for some reason they have more XP than allowed, then
            // clamp it down.
            character_SetExperiencePoints(p, g_ExperienceTables.aTables.piRequired[MAX_PLAYER_LEVEL-1]);
        }
    }

#if SERVER
// TODO: system
    p->iCombatLevel = character_CalcCombatLevel(p);
    character_CapExperienceDebt(p);
#endif

    // Inspiration inventory size
    iInspLevel = character_PowersGetMaximumLevel(p);
    p->iNumInspirationRows = CountForLevel(iInspLevel, g_Schedules.aSchedules.piInspirationRow);
    p->iNumInspirationCols = CountForLevel(iInspLevel, g_Schedules.aSchedules.piInspirationCol);

    // And give them any free powers they can now access.
    if (grantAutoIssuePowers)
        character_GrantAutoIssuePowers(p);

    // Update each power for this level.
    // This does things like add free boost slots for powers which gain them.
    // Skip all temp powers, not just the first power set
    // ARM NOTE: I think this code block is unnecessary, power_LevelFinalize is called after
    //   each power is granted.
//     iSize = eaSize(&p->ppPowerSets);
//     for(i=0; i<iSize; i++)
//     {
//         int j;
//         PowerSet *pset = p->ppPowerSets[i];
//         if (stricmp(pset->psetBase->pcatParent->pchName,"Temporary_Powers") == 0)
//             continue;
// 
//         for(j=eaSize(&pset->ppPowers)-1; j>=0; j--)
//         {
//             if(pset->ppPowers[j]!=NULL)
//             {
//                 power_LevelFinalize(pset->ppPowers[j]);
//             }
//         }
//     }

#if SERVER
    if(p->entParent && p->entParent->pl)
    {
        // Only Primals can unlock the epic ATs. They don't exist in
        // Praetoria, so it makes no sense for Praetorians (including
        // ex-Praetorians - otherwise they'd instantly unlock them when they
        // came to Primal Earth) to unlock them.
        if( !ENT_IS_PRAETORIAN(p->entParent) )
        {
            // Unlock the appropriate epic archetype if the character has
            // reached level 20+ and is on the faction they were created as,
            // or are somehow playing an epic AT character of the right side.
            // That means a Kheldian for heroes or a Soldier of Arachnos for
            // villains, and that's what we award respectively.
            if(ENT_IS_VILLAIN(p->entParent))
            {
                if((p->entParent->db_flags & DBFLAG_UNLOCK_VILLAIN_EPICS) && (character_CalcExperienceLevel(p)>=19 || (p->pclass && (class_MatchesSpecialRestriction(p->pclass, "ArachnosSoldier") || class_MatchesSpecialRestriction(p->pclass, "ArachnosWidow")))))
                {
                    authUserSetArachnosAccess(p->entParent->pl->auth_user_data, 1);
                }
            }
            else
            {
                if((p->entParent->db_flags & DBFLAG_UNLOCK_HERO_EPICS) && (character_CalcExperienceLevel(p)>=19 || (p->pclass && class_MatchesSpecialRestriction(p->pclass, "Kheldian"))))
                {
                    authUserSetKheldian(p->entParent->pl->auth_user_data, 1);
                }
            }
        }

        // Mark them as having reached level 6 if they have. (This is when
        // Korea starts charging customers.)
        if(character_CalcExperienceLevel(p)>=5)
        {
            authUserSetKoreanLevelMinimum(p->entParent->pl->auth_user_data, 1);
        }
    }
#endif
}

/**********************************************************************func*
 * character_GetCombatLevelExtern
 *
 */
int character_GetCombatLevelExtern(Character *p)
{
    if (!p) 
        return 0;
    return p->iCombatLevel + 1;
}

int character_GetExperienceLevelExtern(Character *p)
{
    if (!p) 
        return 0;
    return character_CalcExperienceLevel(p) + 1;
}

#if SERVER

int character_GetSecurityLevelExtern(Character *p)
{
    if (!p)
        return 0;
    return p->iLevel + 1;
}

/**********************************************************************func*
 * character_SetLevelExtern
 *
 */
void character_SetLevelExtern(Character *p, int iLevel)
{
    character_SetLevel(p, MAX(0, iLevel-1));
}

static void character_CheckForFadedBoosts(Character *p)
{
    int setIndex, powerIndex, boostIndex;
    int iLevelEff = character_CalcExperienceLevel(p);

    for (setIndex = eaSize(&p->ppPowerSets) - 1; setIndex >= 0; setIndex--)
    {
        PowerSet *pSet = p->ppPowerSets[setIndex];
        for (powerIndex = eaSize(&pSet->ppPowers) - 1; powerIndex >= 0; powerIndex--)
        {
            Power *ppow = pSet->ppPowers[powerIndex];
            if (ppow == NULL)
            {
                continue;
            }

            for (boostIndex = eaSize(&ppow->ppBoosts) - 1; boostIndex >= 0; boostIndex--)
            {
                if (ppow->ppBoosts[boostIndex] != NULL && ppow->ppBoosts[boostIndex]->ppowBase != NULL
                    && !ppow->ppBoosts[boostIndex]->ppowBase->bBoostIgnoreEffectiveness
                    && !boost_Effectiveness(ppow->ppBoosts[boostIndex], p->entParent, iLevelEff, p->entParent->erOwner)
                    && getPopHelpStateByTag(p->entParent, "CODEPH_Slotted_Enhancement_Faded") == PopHelpState_Untriggered)
                {
                    triggerPopHelpEventHappenedByTag(p->entParent, "CODEPH_Slotted_Enhancement_Faded");
                    return;
                }
            }
        }
    }
}

/**********************************************************************func*
 * character_LevelUp
 *
 */
void character_LevelUp(Character *p )
{
    assert(p!=NULL);

    if((p->iLevel+1)<MAX_PLAYER_LEVEL)
        p->iBuildLevels[p->iActiveBuild] = ++p->iLevel;

    badge_RecordLevelChange(p->entParent);
    shardCommStatus(p->entParent);
#if SERVER
    ContactStatusSendAll(p->entParent);
    ContactStatusAccessibleSendAll(p->entParent);
#endif

    // Check to see if any boosts just faded, and trigger pop help if they did.
    character_CheckForFadedBoosts(p);
}


/**********************************************************************func*
 * character_LevelDown
 *
 */
void character_LevelDown(Character *p)
{
    assert(p!=NULL);

    if(p->iLevel>0)
    {
        p->iBuildLevels[p->iActiveBuild] = --p->iLevel;
    }
    

#if SERVER
    badge_RecordLevelChange(p->entParent);
#endif
}

/**********************************************************************func*
 * character_GiveCappedExperience
 *
 */
void character_GiveCappedExperience(Character *p, int iReceived, int *piActuallyReceived, int *piDebtWorkedOff, int *piLeftOver)
{
    int iNewDebt;

    *piActuallyReceived = 0;
    *piLeftOver = 0;
    *piDebtWorkedOff = iReceived;

    iNewDebt = p->iExperienceDebt - *piDebtWorkedOff;
    if(iNewDebt<0)
    {
        *piDebtWorkedOff += iNewDebt; // subtract off the extra
        *piLeftOver += -iNewDebt;     // add on the extra
        iNewDebt = 0;
    }

    p->iExperienceDebt = iNewDebt;

    badge_RecordExperienceDebtRemoved(p->entParent, *piDebtWorkedOff);
}

#endif

/* End of File */
