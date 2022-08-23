/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>

#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/genericlist.h>

#include "entity/entity.h"
#include "entity/classes.h"
#include "entity/origins.h"
#include "entity/character_base.h"
#include "character_mods.h"
#include "entity/character_level.h"
#include "entity/character_inventory.h"
#include "entity/character_eval.h"
#include "entity/PowerInfo.h"
#include "entity/powers.h"
#include <utilitieslib/utils/error.h>
#include "entity/entPlayer.h"
#include "auth/authUserData.h"
#include "entity/motion.h"
#include "gameComm/trayCommon.h"
#include <utilitieslib/components/MemoryPool.h>
#include "entity/Proficiency.h"
#include "entity/Concept.h"
#include "bases/DetailRecipe.h"
#include "gameData/mininghelper.h"
#include "entity/boostset.h"
#include <utilitieslib/utils/scriptvars.h>
#include <utilitieslib/language/MessageStore.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/costume.h"
#include <utilitieslib/utils/mathutil.h>
#include "auction/Auction.h"
#include <utilitieslib/utils/file.h>
#include "entity/PCC_Critter.h"
#include "entity/incarnate.h"

#if SERVER
#include "entity/entGameActions.h"
#include "entity/character_pet.h"
#include "entity/character_combat.h"
#include "entity/character_animfx.h"
#include "entity/buddy_server.h"
#include "entity/character_tick.h"
#include "dbcomm/logcomm.h"
#include "dbcomm/dbcomm.h"
#include "dbghelper.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/file.h>
#include "comm_game.h"
#include "language/langServerUtil.h"
#include "storyarc/missionobjective.h"
#include "dbcomm/staticMapInfo.h"
#include "character_karma.h"
#include "gameComm/NpcServer.h"
#include "dbcomm/staticMapInfo.h"
#include "gameSys/Turnstile.h"
#include "entity/incarnate_server.h"
#include "bases/basedata.h"

char *g_estrDestroyedCritters = NULL;
char *g_estrDestroyedPlayers = NULL;
char *g_estrDestroyedPets = NULL;

U32 g_uTimeCombatStatsStarted = 0;

#define DEBUG_SLOTTING 0

#endif // SERVER

#if CLIENT
#include "UI/uiTray.h"
#endif // CLIENT

char * g_smallInspirations[] = { "Luck", "Sturdy", "Insight", "Catch_a_Breath", "Respite", "Enrage", "Break_Free", "Awaken", 0 };
char * g_mediumInspirations[] = { "Good_Luck", "Rugged", "Keen_Insight", "Take_a_Breather", "Dramatic_Improvement", "Focused_Rage", "Emerge", "Bounce_Back", 0 };
char * g_largeInspirations[] = { "Phenomenal_Luck", "Robust", "Uncanny_Insight", "Second_Wind", "Resurgence", "Righteous_Rage", "Escape", "Restoration", 0 };

#include <utilitieslib/utils/timing.h>

#define CODEOBJ_USED_INSPIRATION "CODEOBJ_USED_INSPIRATION"

/**********************************************************************func*
 * character_Create
 *
 */
void character_CreatePartOne(Character *pchar, Entity *e, const CharacterOrigin *porigin, const CharacterClass *pclass)
{
    int i;
    int j;
    int isPlayer = ENTTYPE(e) == ENTTYPE_PLAYER;
    PowerSet *pset;

    assert(pchar!=NULL);
#ifndef CLIENT   //Creating a character shouldn't require an origin specified right away
    assert(porigin!=NULL);  
#endif
    assert(pclass!=NULL);

    // remove old character if it exists
    if(pchar->ppPowerSets!=NULL)
    {
        character_Destroy(pchar,NULL);
    }

    memset(pchar, 0, sizeof(Character));

    pchar->entParent = e;
    pchar->porigin = porigin;
    pchar->pclass = pclass;

    if (isPlayer)
    {
        for (i = 0; i < MAX_BUILD_NUM; i++)
        {
            eaCreate(&pchar->ppBuildPowerSets[i]);
            // DGNOTE 8/27/08 - I don't think I've ever seen this bigger than 16.  There's a check
            // in character_SelectBuild - if that ever trips and directs you here, increase the
            // size of this allocate.
            // DGNOTE 9/18/2008 - Yes, the assert tripped when loading omnipotent characters.
            // Therefore, in dev mode we set this to huge (1024), and a slightly more
            // generous 32 for release mode
            // DGNOTE 10/1/2008 - The above idea will cause a mapserver crash when an omnipotent is loaded
            // on live.  Which, unfortunately, people do from time to time.  So, we take a different tack.
            // Allow the array to grow, and then watch for changes in the switchbuild and addnewpowerset
            // routines.
            eaSetCapacity(&pchar->ppBuildPowerSets[i], /* isProductionMode() ? 32 : 1024*/ 4 );
        }
        pchar->ppPowerSets = pchar->ppBuildPowerSets[0];
        pchar->iCurBuild = 0;
        pchar->iActiveBuild = 0;

        // Praetorian tutorial limits health to 50% of max at first. We
        // cache the status here so character/powers code doesn't have to
        // go rummaging through the ent.
        if (e->db_flags & DBFLAG_HALF_MAX_HEALTH && ENT_CAN_GO_PRAETORIAN_TUTORIAL(e))
            pchar->bHalfMaxHealth = 1;

#ifdef SERVER
        // If this map is part of the Praetorian tutorial whether zone or
        // mission, players cannot be killed.
        if (g_MapIsTutorial)
            pchar->bCannotDie = 1;
#endif
    }
    else
    {
        eaCreate(&pchar->ppPowerSets);
    }
    eaCreateConst(&pchar->ppLastDamageFX);
    eaiCreate(&pchar->piStickyAnimBits);
    eaiCreate(&pchar->pPowerModes);

    // Initialize phases.
    pchar->bitfieldCombatPhases = 1; // default to be in the prime phase

    // Give them their auto issue sets (shared among builds)
    character_GrantAutoIssueBuildSharedPowerSets(pchar);

    // Give them their auto issue sets (build zero)
    character_GrantAutoIssueBuildSpecificPowerSets(pchar);
        
    if (isPlayer)
    {
        for (i = 1; i < MAX_BUILD_NUM; i++)
        {
            for (j = 0; j < g_numSharedPowersets; j++)
            {
                int result;
                pset = pchar->ppBuildPowerSets[0][j];
                result = eaPush(&pchar->ppBuildPowerSets[i], pset);
                assert(pset->idx == j);
                assert(result == j);
            }
            character_SelectBuildLightweight(pchar->entParent, i);
            // Give them their auto issue sets (build one and above)
            character_GrantAutoIssueBuildSpecificPowerSets(pchar);
        }
        character_SelectBuildLightweight(pchar->entParent, 0);
    }

    // Calculate vars and fill in arrays as appropriate.
    character_LevelFinalize(pchar, false);

    // Fill in the character's attributes
    character_Reset(pchar, true);
#if SERVER
    CharacterKarmaInit(pchar);
#endif
}

void character_CreatePartTwo(Character *pchar)
{
    character_GrantAutoIssuePowers(pchar);
}

void character_Create(Character *pchar, Entity *e, const CharacterOrigin *porigin, const CharacterClass *pclass)
{
    character_CreatePartOne(pchar, e, porigin, pclass);
    character_CreatePartTwo(pchar);
}

/**********************************************************************func*
* character_ResetBoostsets
*
*/
void character_ResetBoostsets(Character *pchar)
{
    int i, j;
    for(i=eaSize(&pchar->ppPowerSets)-1; i>=0; i--)
    {
        PowerSet *pPowerSet = pchar->ppPowerSets[i];
        for(j=eaSize(&pPowerSet->ppPowers)-1; j>=0; j--)
        {
            Power *pPower = pchar->ppPowerSets[i]->ppPowers[j];
            if (pPower) 
            {
                pPower->bBoostsCalcuated = false;
            }
        }
    }
}


/**********************************************************************func*
* character_ClearBoostsetAutoPowers
*
*/
void character_ClearBoostsetAutoPowers(Character *pchar)
{
    int i, j;
    for(i=eaSize(&pchar->ppPowerSets)-1; i>=0; i--)
    {
        PowerSet *pPowerSet = pchar->ppPowerSets[i];
        for(j=eaSize(&pPowerSet->ppPowers)-1; j>=0; j--)
        {
            Power *pPower = pchar->ppPowerSets[i]->ppPowers[j];
            if (pPower) 
            {
                boostset_RecompileBonuses(pchar, pPower, true);
            }
        }
    }
}

// Turns off the powers system and then immediately turns it back on again.
// Also resets movement and health to default state
// Can drop toggles if you tell it to
void character_Reset(Character *pchar, bool resetToggles)
{
// TODO: I'm not real happy with this ifdef, but this seems like the most
//       appropriate place to put this.
#ifdef SERVER
    int iset;

    if(!character_IsInitialized(pchar))
    {
        return;
    }

    character_CancelPowers(pchar);

    powlist_RemoveAll(&pchar->listRechargingPowers);
    powlist_RemoveAll(&pchar->listTogglePowers);

    if (resetToggles)
    {
        for(iset=eaSize(&pchar->ppPowerSets)-1; iset>=0; iset--)
        {
            int ipow;
            for(ipow=eaSize(&pchar->ppPowerSets[iset]->ppPowers)-1; ipow>=0; ipow--)
            {
                Power *ppow = pchar->ppPowerSets[iset]->ppPowers[ipow];
                if(ppow!=NULL)
                {
                    ppow->bActive = false;
                }
            }
        }
    }

    // First the defaults
    // Don't do this on the client.  The client will receive attrib info
    //   from the server.
    pchar->attrCur.fHitPoints = pchar->pclass->pattrMax[pchar->iCombatLevel].fHitPoints;
    pchar->attrCur.fEndurance = pchar->pclass->pattrMax[pchar->iCombatLevel].fEndurance;
    pchar->attrCur.fInsight = pchar->pclass->pattrMax[pchar->iCombatLevel].fInsight;
    pchar->attrCur.fRage = 0.0f;

    pchar->bRecalcStrengths = true;
    pchar->bNoModsLastTick = false;

    // Honestly, I don't know if this is needed. There is some problem with
    // these getting set strangely and I'm doing some defensive programming. --poz
    pchar->ulTimeHeld = pchar->ulTimeImmobilized = pchar->ulTimeSleep = pchar->ulTimeStunned = timerSecondsSince2000();

    character_UncancelPowers(pchar);

    setSurfaceModifiers(pchar->entParent,SURFPARAM_AIR, pchar->attrCur.fMovementControl, pchar->attrCur.fMovementFriction, 1.0f, 1.0f);
    setSurfaceModifiers(pchar->entParent,SURFPARAM_GROUND, 1,1, 1.0f, 1.0f);
    setJumpHeight(pchar->entParent, pchar->attrCur.fJumpHeight);
    setSpeed(pchar->entParent, SURFPARAM_GROUND, 1);


    // For combat logging startup
    if(g_uTimeCombatStatsStarted==0)
    {
        g_uTimeCombatStatsStarted = timerSecondsSince2000();
    }

#endif
}

// This function temporarily turns off the powers system for the specified character
//   until character_UncancelPowers().
// YOU MUST CALL character_UncancelPowers() after calling this!
void character_CancelPowers(Character *pchar)
{
#ifdef SERVER
    if(!character_IsInitialized(pchar))
    {
        return;
    }

    character_DestroyAllPets(pchar);
    character_RemoveAllMods(pchar);

    // Shut off all the powers the character has on.
    character_ShutOffAllTogglePowers(pchar);
    character_ShutOffAllAutoPowers(pchar);
    character_KillCurrentPower(pchar);
    character_KillQueuedPower(pchar);
    character_KillRetryPower(pchar);

    character_DisableAllPowers(pchar);

    // Run this to ensure we process onDeactivate and onDisable mods properly.
    // Don't call character_TickPhaseTwo() so we don't re-enable all the powers.
    character_AccrueMods(pchar, 0, NULL);
#endif
}

// Undoes character_CancelPowers().  I don't really like this name, but I'm at brain capacity atm...
void character_UncancelPowers(Character *pchar)
{
#ifdef SERVER
    if(!character_IsInitialized(pchar))
    {
        return;
    }

    character_UpdateAllEnabledState(pchar);
    character_ActivateAllAutoPowers(pchar);
    character_TurnOnActiveTogglePowers(pchar);

    // Calculate the initial basic attributes of this character.
    character_AccrueMods(pchar, 0.0f, NULL);
#endif
}

#if SERVER

/**********************************************************************func*
 * character_ResetStats
 *
 */
void character_ResetStats(Character *pchar)
{
    int i;

    memset(&pchar->stats, 0, sizeof(CharacterStats));

    for(i=eaSize(&pchar->ppPowerSets)-1; i>=0; i--)
    {
        int j;
        for(j=eaSize(&pchar->ppPowerSets[i]->ppPowers)-1; j>=0; j--)
        {
            if (pchar->ppPowerSets[i]->ppPowers[j]) {
                memset(&pchar->ppPowerSets[i]->ppPowers[j]->stats, 0, sizeof(PowerStats));
            }
        }
    }
}

/**********************************************************************func*
 * character_DumpStats
 *
 */
void character_DumpStats(Character *pchar, char **pestr)
{
    U32 now = timerSecondsSince2000();

    if(!*pestr)
    {
        estrCreate(pestr);
    }

    if(pchar->stats.iCntUsed>0)
    {
        char ach[256];
        char *start = ach;

        if(ENTTYPE(pchar->entParent)!=ENTTYPE_PLAYER && erGetDbId(pchar->entParent->erOwner)!=0)
        {
            Entity *pOwner = erGetEnt(pchar->entParent->erOwner);
            if(pOwner)
            {
                dbg_FillNameStr(SAFESTR(ach), pOwner);
            }
            else
            {
                sprintf(ach, "DBID-%d", erGetDbId(pchar->entParent->erOwner));
            }
            strcat(ach, ":");
            start = ach + strlen(ach);
        }
        dbg_FillNameStr(start, ARRAY_SIZE_CHECKED(ach) - (start-ach), pchar->entParent);

        // log_printf("combat",
        estrConcatf(pestr, "%s\t%s\t%s\t\t\t%1.2f\t%1.2f\t%1.2f\t%1.2f\t%d\t%d\t%d\t%1.2f\t%1.2f\t%1.2f\t%1.2f\t%d\t%d\t%d\t%d\n",
            ach,
            pchar->pclass->pchName,
            "Total",
            pchar->stats.fDamageGiven,
            pchar->stats.fDamageGiven/((float)now-(float)g_uTimeCombatStatsStarted),
            pchar->stats.fHealingGiven,
            pchar->stats.fHealingGiven/((float)now-(float)g_uTimeCombatStatsStarted),
            pchar->stats.iCntUsed,
            pchar->stats.iCntHits,
            pchar->stats.iCntMisses,
            (pchar->stats.iCntMisses+pchar->stats.iCntHits) ?
                100.0f*(float)pchar->stats.iCntHits/(float)(pchar->stats.iCntMisses+pchar->stats.iCntHits) : 0.0f,
            (pchar->stats.iCntMisses+pchar->stats.iCntHits) ?
                100.0f*(float)pchar->stats.iCntMisses/(float)(pchar->stats.iCntMisses+pchar->stats.iCntHits) : 0.0f,
            pchar->stats.fDamageReceived,
            pchar->stats.fHealingReceived,
            pchar->stats.iCntBeenHit,
            pchar->stats.iCntBeenMissed,
            pchar->stats.iCntKnockbacks,
            pchar->stats.iCntKills);
        {
            int i;
            for(i=eaSize(&pchar->ppPowerSets)-1; i>=0; i--)
            {
                int j;
                for(j=eaSize(&pchar->ppPowerSets[i]->ppPowers)-1; j>=0; j--)
                {
                    PowerStats *pStats = &pchar->ppPowerSets[i]->ppPowers[j]->stats;

                    if(pchar->ppPowerSets[i]->ppPowers[j] && pStats->iCntUsed!=0)
                    {
                        estrConcatf(pestr, "%s\t%s\t%s\t%s\t%s\t%1.2f\t%1.2f\t%1.2f\t%1.2f\t%d\t%d\t%d\t%1.2f\t%1.2f\n",
                            ach,
                            pchar->pclass->pchName,
                            pchar->ppPowerSets[i]->psetBase->pcatParent->pchName,
                            pchar->ppPowerSets[i]->psetBase->pchName,
                            pchar->ppPowerSets[i]->ppPowers[j]->ppowBase->pchName,
                            pStats->fDamageGiven,
                            pStats->fDamageGiven/((float)now-(float)g_uTimeCombatStatsStarted),
                            pStats->fHealingGiven,
                            pStats->fHealingGiven/((float)now-(float)g_uTimeCombatStatsStarted),
                            pStats->iCntUsed,
                            pStats->iCntHits,
                            pStats->iCntMisses,
                            (pStats->iCntMisses+pStats->iCntHits) ?
                                100.0f*(float)pStats->iCntHits/(float)(pStats->iCntMisses+pStats->iCntHits) : 0.0f,
                            (pStats->iCntMisses+pStats->iCntHits) ?
                                100.0f*(float)pStats->iCntMisses/(float)(pStats->iCntMisses+pStats->iCntHits) : 0.0f);
                    }
                }
            }
        }
    }
}
#endif

/**********************************************************************func*
 * character_Destroy
 *
 */
void character_Destroy(Character *pchar, const char *context)
{
    int ibuild,iset,i,stopHere;

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(pchar);
    }

#if SERVER
    if(isDevelopmentMode() && pchar->entParent)
    {
        if(ENTTYPE(pchar->entParent)==ENTTYPE_PLAYER)
        {
            character_DumpStats(pchar, &g_estrDestroyedPlayers);
        }
        else if(erGetDbId(pchar->entParent->erOwner)!=0)
        {
            character_DumpStats(pchar, &g_estrDestroyedPets);
        }
        else
        {
            character_DumpStats(pchar, &g_estrDestroyedCritters);
        }
    }
#endif

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(pchar);
    }

    // Clean out all the lists
    character_CancelPowers(pchar); // doesn't call character_UncancelPowers(), because we're destroying the character...
#ifdef SERVER
    character_DestroyAllArenaPets( pchar );
#endif

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(pchar);
    }

    character_InventoryFree(pchar);
    AuctionInventory_Destroy(pchar->auctionInv);

    if (pchar->entParent && ENTTYPE(pchar->entParent) == ENTTYPE_PLAYER)
    {
        // First time through, destroy everything down to and including entry zero.  This means we include temps, accolades & dayjob powers.
        // Since those are explicitly shared across all builds, they must only be destroyed once.  Hence, on the second and subsequent
        // iterations, we stop at g_numSharedPowersets.
        stopHere = 0;
        for(ibuild = 0; ibuild < MAX_BUILD_NUM; ibuild++)
        {
            for(iset = eaSize(&pchar->ppBuildPowerSets[ibuild]) - 1; iset >= stopHere; iset--)
            {
                powerset_Destroy(pchar->ppBuildPowerSets[ibuild][iset],context);
            }
            eaDestroy(&pchar->ppBuildPowerSets[ibuild]);
            stopHere = g_numSharedPowersets;
        }
        pchar->ppPowerSets = NULL;
    }
    else
    {
        for(iset = eaSize(&pchar->ppPowerSets) - 1; iset >= 0; iset--)
        {
            powerset_Destroy(pchar->ppPowerSets[iset],context);
        }
        eaDestroy(&pchar->ppPowerSets);
    }
    eaDestroyConst(&pchar->ppLastDamageFX);

    eaiDestroy(&pchar->piStickyAnimBits);
    eaiDestroy(&pchar->pPowerModes);

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(pchar);
    }

    for (i=0; i<CHAR_BOOST_MAX; i++)
    {
        if (pchar->aBoosts[i]!=NULL)
        {
            boost_Destroy(pchar->aBoosts[i]);
            pchar->aBoosts[i]=NULL;
        }
    }

    if (pchar->powersByUniqueID) {
        stashTableDestroy(pchar->powersByUniqueID);
    }

#if SERVER
    eaiDestroy(&pchar->invStatusChange.type);
    eaiDestroy(&pchar->invStatusChange.idx);
#endif

    eaDestroyConst(&pchar->expiredPowers);
    eaDestroyExConst(&pchar->designerStatuses, NULL);
    eaDestroyEx(&pchar->critterRewardTokens, rewardtoken_Destroy);
#if SERVER
    CharacterDestroyKarmaBuckets(pchar);
#endif

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(pchar);
    }
}

/**********************************************************************func*
 * character_GetBaseAttribs
 *
 */
void character_GetBaseAttribs(Character *p, CharacterAttributes *pattr)
{
    *pattr = *p->pclass->ppattrBase[0];

    pattr->fHitPoints = (float)p->pclass->pattrMax[p->iCombatLevel].fHitPoints;
    pattr->fAbsorb = 0.0f;
    pattr->fEndurance = (float)p->pclass->pattrMax[p->iCombatLevel].fEndurance;
}

/**********************************************************************func*
 * character_AddNewPowerSet
 *
 */
PowerSet *character_AddNewPowerSet(Character *p, const BasePowerSet *psetBase)
{
    PowerSet *pset = powerset_Create(psetBase);
    PowerSet **orig_ppPowerSets = p->ppPowerSets;

    assert(p!=NULL);
    assert(psetBase!=NULL);
    assert(pset!=NULL);
    assert(p->entParent!=NULL);

    pset->pcharParent = p;
    pset->idx = eaPush(&p->ppPowerSets, pset);
    
    if (ENTTYPE(p->entParent) == ENTTYPE_PLAYER && orig_ppPowerSets != p->ppPowerSets)
    {
        p->ppBuildPowerSets[p->iCurBuild] = p->ppPowerSets;
    }

    return pset;
}

/**********************************************************************func*
* character_RemovePowerSet
*
*/
void character_RemovePowerSet(Character *pchar, const BasePowerSet *psetBase, const char *context)
{
    int i;

    assert(pchar!=NULL);
    assert(psetBase!=NULL);

    if (pchar->ppowDefault && pchar->ppowDefault->ppowBase && 
        pchar->ppowDefault->ppowBase->psetParent == psetBase)
    {
        pchar->ppowDefault = NULL;
    }
    
    if (pchar->ppowStance && pchar->ppowStance->ppowBase &&
        pchar->ppowStance->ppowBase->psetParent == psetBase)
    {
        pchar->ppowStance = NULL; // Should I call character_EnterStance() instead?
    }

    for(i=eaSize(&pchar->ppPowerSets)-1; i>=0; i--)
    {
        if(psetBase == pchar->ppPowerSets[i]->psetBase)
        {
            powerset_Destroy(pchar->ppPowerSets[i],context);
            eaRemove(&pchar->ppPowerSets, i);
            return;
        }
    }
}

/**********************************************************************func*
 * character_OwnsPowerSet
 *
 */
PowerSet *character_OwnsPowerSet(Character *pchar, const BasePowerSet *psetTest)
{
    int i;

    assert(pchar!=NULL);
    assert(psetTest!=NULL);

    for(i=eaSize(&pchar->ppPowerSets)-1; i>=0; i--)
    {
        if(pchar->ppPowerSets[i]->psetBase==psetTest)
        {
            return pchar->ppPowerSets[i];
        }
    }

    return NULL;
}

/**********************************************************************func*
 * character_OwnsPowerSetInAnyBuild
 *
 */
PowerSet *character_OwnsPowerSetInAnyBuild(Character *pchar, const BasePowerSet *psetTest)
{
    int i;
    int j;

    assert(pchar!=NULL);
    assert(psetTest!=NULL);

    for(i=0; i < MAX_BUILD_NUM; i++)
    {
        for(j=eaSize(&pchar->ppBuildPowerSets[i])-1; j>=0; j--)
        {
            if(pchar->ppBuildPowerSets[i][j]->psetBase==psetTest)
            {
                return pchar->ppBuildPowerSets[i][j];
            }
        }
    }

    return NULL;
}

Power *character_OwnsPowerByUID(Character *pchar, int powerUID, int basePowerCRC)
{
    Power *returnMe;

    if (!pchar)
    {
        Errorf("character_OwnsPowerByUID() received a null Character!");
        return NULL;
    }

    stashIntFindPointer(pchar->powersByUniqueID, powerUID, &returnMe);

    if (returnMe)
    {
        if (returnMe->ppowBase->crcFullName != basePowerCRC)
            returnMe = NULL; // this isn't the power you're looking for.
    }

    return returnMe;
}

Power *character_OwnsPower(Character *pchar, const BasePower *ppowTest)
{
    int i;
    int j;

    if (!pchar)
    {
        Errorf("character_OwnsPower() received a null Character!");
        return NULL;
    }

    if (!ppowTest)
    {
        Errorf("character_OwnsPower() received a null BasePower!");
        return NULL;
    }

    for(i=eaSize(&pchar->ppPowerSets)-1; i>=0; i--)
    {
        if (pchar->ppPowerSets[i]->psetBase == ppowTest->psetParent)
        {
            for(j=eaSize(&pchar->ppPowerSets[i]->ppPowers)-1; j>=0; j--)
            {
                if(pchar->ppPowerSets[i]->ppPowers[j]!=NULL
                    && 
                    pchar->ppPowerSets[i]->ppPowers[j]->ppowOriginal==ppowTest)
                {
                    return pchar->ppPowerSets[i]->ppPowers[j];
                }
            }
        }
    }

    return NULL;
}

/**********************************************************************func*
 * character_OwnsPowerNum
 *
 * Returns the number of copies of this power owned by the player.
 */
int character_OwnsPowerNum(Character *pchar, const BasePower *ppowTest)
{
    int i, j;
    int iCnt = 0;

    if (!pchar || !ppowTest)
        return 0;

    for(i=eaSize(&pchar->ppPowerSets)-1; i>=0; i--)
    {
        if (pchar->ppPowerSets[i]->psetBase == ppowTest->psetParent)
        {
            for(j=eaSize(&pchar->ppPowerSets[i]->ppPowers)-1; j>=0; j--)
            {
                if(pchar->ppPowerSets[i]->ppPowers[j]!=NULL
                    &&    pchar->ppPowerSets[i]->ppPowers[j]->ppowOriginal==ppowTest)
                {
                    iCnt++;
                }
            }
        }
    }

    return iCnt;
}

PowerSet *character_BuyPowerSet(Character *p, const BasePowerSet *psetBase )
{
    PowerSet *pset;
#if SERVER
    int i;
#endif

    if(p==NULL) return NULL;
    if(psetBase==NULL) return NULL;

    if((pset=character_OwnsPowerSet(p, psetBase))!=NULL)
    {
        // They already own it.
        return pset;
    }

    pset = character_AddNewPowerSet(p, psetBase);
    pset->iLevelBought = (pset->psetBase && pset->psetBase->iForceLevelBought >= 0) ? pset->psetBase->iForceLevelBought : character_GetLevel(p);

#if SERVER
    // Now add all the powers they get for free in this set.
    for(i=eaiSize(&psetBase->piAvailable)-1; i>=0; i--)
    {
        if(psetBase->piAvailable[i]<0
            && character_IsAllowedToHavePower(p, psetBase->ppPowers[i]))
        {
            Power *ppow = powerset_AddNewPower(pset, psetBase->ppPowers[i], 0, NULL);
            ppow->iLevelBought = pset->iLevelBought;
#if SERVER
            if (Incarnate_slotFromPower(ppow->ppowBase) == kIncarnateSlot_Count)
            {
                // Explicitly does not trigger onEnable mods for Incarnate powers,
                //   because we're about to disable them in a little bit.  Don't trigger
                //   both onEnable/onDisable when buying Incarnate powers...
                character_UpdateEnabledState(p, ppow);
            }
#endif
            power_LevelFinalize(ppow);
        }
    }

    // Now give them any appropriate costume keys
    for(i = eaSize(&psetBase->ppchCostumeKeys)-1; i >= 0; --i)
        costume_Award(p->entParent,psetBase->ppchCostumeKeys[i], true, 0);
#endif

    return pset;
}

bool character_CanBuyPowerWithoutOverflow(Character *pchar, const BasePower *ppowTest)
{
    int i;
    int j;
    Power *ppow = NULL;
    int iCnt = 0;

    if (!pchar || !ppowTest)
        return false;

    assert(pchar!=NULL);
    assert(ppowTest!=NULL);

    // verify that this is a valid power for this character to have
    if (ENTTYPE(pchar->entParent) == ENTTYPE_PLAYER && 
        pchar->entParent->access_level == 0 &&
        (strnicmp(ppowTest->pchName, "prestige", 8) != 0) &&  // this was required because some prestige rewards use auth bits 
        !character_IsAllowedToHavePowerHypothetically(pchar, ppowTest))
    {
        return false;
    }

    // verify that it will not over flow any existing power extension limits
    for(i=eaSize(&pchar->ppPowerSets)-1; i>=0 && ppow == NULL; i--)
    {
        for(j=eaSize(&pchar->ppPowerSets[i]->ppPowers)-1; j>=0; j--)
        {
            if(pchar->ppPowerSets[i]->ppPowers[j]!=NULL
                && pchar->ppPowerSets[i]->ppPowers[j]->ppowOriginal==ppowTest)
            {
                ppow = pchar->ppPowerSets[i]->ppPowers[j];
                iCnt++;
            }
        }
    }

    if (!ppow)                // we don't have a previous copy of the power
        return true;

    if(iCnt >= ppowTest->iNumAllowed)
    {
        // check to see if we can extend this power
        if (ppowTest->bStackingUsage && (ppowTest->bHasLifetime || ppowTest->bHasUseLimit))
        {
            int max;
            float fMax;

            if (ppowTest->iNumCharges > 0)
            {    
                if (ppowTest->iMaxNumCharges == 0)
                    max = ppowTest->iNumCharges;
                else if (ppowTest->iMaxNumCharges > 0)
                    max = ppowTest->iMaxNumCharges;

                if (ppow->iNumCharges + ppowTest->iNumCharges > max)
                    return false;
            }

            if (ppowTest->fUsageTime > 0)
            {    
                if (ppowTest->fMaxUsageTime == 0)
                    fMax = ppowTest->fUsageTime;
                else if (ppowTest->fMaxUsageTime > 0)
                    fMax = ppowTest->fMaxUsageTime;

                if (ppow->fUsageTime + ppowTest->fUsageTime> fMax)
                    return false;
            }

            if (ppowTest->fLifetime > 0)
            {    
                if (ppowTest->fMaxLifetime == 0)
                    fMax = ppowTest->fLifetime;
                else if (ppowTest->fMaxLifetime > 0)
                    fMax = ppowTest->fMaxLifetime;

                if ((ppow->ulCreationTime + 2*(ppowTest->fLifetime) - timerSecondsSince2000()) > ((U32) (fMax + 0.5f)))
                    return false;
            }

            if (ppowTest->fMaxLifetimeInGame > 0)
            {    
                if (ppowTest->fMaxLifetimeInGame == 0)
                    fMax = ppowTest->fLifetimeInGame;
                else if (ppowTest->fMaxLifetimeInGame > 0)
                    fMax = ppowTest->fMaxLifetimeInGame;

                if ((2*ppowTest->fLifetimeInGame) - ppow->fAvailableTime > fMax)
                    return false;
            }
        }
    } else {
        return true;
    }

    return true;
}

// Pass in a uniqueID of zero if you are buying a fresh power, non-zero if you are buying something that the client/server already bought
// oldCharacter is only used in respecs.
Power *character_BuyPowerEx(Character *p, PowerSet *pset, const BasePower *ppowBase, bool *pbAdded, int bIgnoreConningRescrictions, int uniqueID, Character *oldCharacter)
{
    int set;
    int i;
    int n;
    Power* ppow = NULL;
    int iCnt = 0;

    if(p==NULL) 
        return NULL;
    if(pset==NULL) 
        return NULL;
    if(ppowBase==NULL) 
        return NULL;

    if (ENTTYPE(p->entParent) == ENTTYPE_PLAYER && 
        p->entParent->access_level == 0 &&
        (strnicmp(ppowBase->pchName, "prestige", 8) != 0) &&  // this was required because some prestige rewards use auth bits 
        !character_IsAllowedToHavePowerHypothetically(p, ppowBase))
    {
#if SERVER
        LOG_ENT( p->entParent, LOG_CHEATING, LOG_LEVEL_IMPORTANT, 0, "CHEAT: Invalid power bought. Power: %s, BoughtAt: %d", 
                    ppowBase->pchName, character_GetLevel(p));
#endif
        return NULL;
    }

#if SERVER
    if (ENTTYPE(p->entParent) == ENTTYPE_CRITTER)
    {
        int rankCon;

        // checking for max level limits
        if( !bIgnoreConningRescrictions && ((basepower_GetAIMaxLevel(ppowBase) < p->iLevel) || (basepower_GetAIMinLevel(ppowBase) > p->iLevel)))
        {
            return NULL;
        }

        // checking for rank con limits
        rankCon = villainRankConningAdjustment(p->entParent->villainRank);
        if (!bIgnoreConningRescrictions && ((basepower_GetAIMaxRankCon(ppowBase) < rankCon) ||    (basepower_GetAIMinRankCon(ppowBase) > rankCon)))
        {
            return NULL;
        }
    }
#endif

    set = pset->idx;

    n = eaSize(&p->ppPowerSets[set]->ppPowers);
    for(i=0; i<n; i++)
    {
        if(p->ppPowerSets[set]->ppPowers[i] && p->ppPowerSets[set]->ppPowers[i]->ppowOriginal==ppowBase)
        {
            ppow = p->ppPowerSets[set]->ppPowers[i];
            iCnt++;
        }
    }

    if(iCnt>=ppowBase->iNumAllowed)
    {
        // We hit our max

        // check to see if we can extend this power
        if (ppowBase->bStackingUsage && (ppowBase->bHasLifetime || ppowBase->bHasUseLimit))
        {
            int max;
            float fMax;

            if (ppowBase->iNumCharges > 0)
            {    
                ppow->iNumCharges += ppowBase->iNumCharges;

                if (ppowBase->iMaxNumCharges == 0)
                    max = ppowBase->iNumCharges;
                else if (ppowBase->iMaxNumCharges > 0)
                    max = ppowBase->iMaxNumCharges;

                if (ppow->iNumCharges > max)
                    ppow->iNumCharges = max;
            }

            if (ppowBase->fUsageTime > 0)
            {    
                ppow->fUsageTime += ppowBase->fUsageTime;

                if (ppowBase->fMaxUsageTime == 0)
                    fMax = ppowBase->fUsageTime;
                else if (ppowBase->fMaxUsageTime > 0)
                    fMax = ppowBase->fMaxUsageTime;

                if (ppow->fUsageTime > fMax)
                    ppow->fUsageTime = fMax;
            }

            if (ppowBase->fLifetime > 0)
            {    
                ppow->ulCreationTime += ((U32) (ppowBase->fLifetime + 0.5f));

                if (ppowBase->fMaxLifetime == 0)
                    fMax = ppowBase->fLifetime;
                else if (ppowBase->fMaxLifetime > 0)
                    fMax = ppowBase->fMaxLifetime;

                if ((ppow->ulCreationTime + ppowBase->fLifetime - timerSecondsSince2000()) > ((U32) (fMax + 0.5f)))
                    ppow->ulCreationTime = timerSecondsSince2000() + ((U32) (fMax + 0.5f)) - ((U32) (ppowBase->fLifetime + 0.5f));
            }

            if (ppowBase->fMaxLifetimeInGame > 0)
            {    
                ppow->fAvailableTime -= ppowBase->fLifetimeInGame;

                if (ppowBase->fMaxLifetimeInGame == 0)
                    fMax = ppowBase->fLifetimeInGame;
                else if (ppowBase->fMaxLifetimeInGame > 0)
                    fMax = ppowBase->fMaxLifetimeInGame;

                if (ppowBase->fLifetimeInGame - ppow->fAvailableTime > fMax)
                    ppow->fAvailableTime = ppowBase->fLifetimeInGame - fMax;
            }

            if (pbAdded)
                *pbAdded = true;
        } else {
            if (pbAdded)
                *pbAdded = false;
        }

        return ppow;
    }

    ppow = powerset_AddNewPower(pset, ppowBase, uniqueID, oldCharacter);
    if(ppow)
    {
        ppow->bIgnoreConningRestrictions = bIgnoreConningRescrictions;
        ppow->iLevelBought = (ppow->ppowBase && ppow->ppowBase->iForceLevelBought >= 0) ? ppow->ppowBase->iForceLevelBought : character_GetLevel(p);

        // Fix the level bought
        if(ppowBase->bAutoIssue && !ppowBase->bAutoIssueSaveLevel)
        {
            const BasePowerSet *psetBase = ppowBase->psetParent;
            
            for(i = eaiSize(&(psetBase->piAvailable))-1; i>=0; i--)
            {
                if(psetBase->ppPowers[i]==ppowBase)
                {
                    ppow->iLevelBought = psetBase->piAvailable[i];
                    break;
                }
            }
        }

#if SERVER
        if (ppowBase->bInstanceLocked)
        {
            int instanceID;
            turnstileMapserver_getMapInfo(&instanceID, NULL);
            if (!db_state.local_server && !instanceID)
            {
                Errorf("Granting an instance locked power in a non instance (turnstile launched) map.\nIf development testing, you can use \"turnstile_debug_set_map_info\" to set the instance. \nIf QA, this power is being used improperly");
            }
            ppow->instanceBought = instanceID;
        }

        if (Incarnate_slotFromPower(ppow->ppowBase) == kIncarnateSlot_Count)
        {
            // Explicitly does not trigger onEnable mods for Incarnate powers,
            //   because we're about to disable them in a little bit.  Don't trigger
            //   both onEnable/onDisable when buying Incarnate powers...
            character_UpdateEnabledState(p, ppow);
        }
#endif
        power_LevelFinalize(ppow);

        if (pbAdded)
            *pbAdded = true;
        p->bJustBoughtPower = true;
    }

    return ppow;
}

// Pass in a uniqueID of zero if you are buying a fresh power, non-zero if you are buying something that the client/server already bought
// oldCharacter is only used in respecs.
Power *character_BuyPower(Character *p, PowerSet *pset, const BasePower *ppowBase, int uniqueID)
{
    bool bAdded;
    return character_BuyPowerEx(p, pset, ppowBase, &bAdded, 0, uniqueID, NULL);
}

/**********************************************************************func*
* character_CanAddTemporaryPower
*
*/
bool character_CanAddTemporaryPower( Character *p, Power * pow )
{
    PowerSet *pset;
    int set, i, n, iCnt = 0;

    if( !p || !pow )
        return 0;

    // activating, recharging, or toggle that is on
    if( pow->fTimer|| !pow->ppowBase->bTradeable || (pow->bActive && pow->ppowBase->eType != kPowerType_Auto ) )
        return 0;

    pset = character_OwnsPowerSet(p, pow->psetParent->psetBase);
    if( pset ) // if we don't have the powerset then it will be added
    {
        set = pset->idx;
        n = eaSize(&p->ppPowerSets[set]->ppPowers);
        for(i=0; i<n; i++)
        {
            if(p->ppPowerSets[set]->ppPowers[i] && p->ppPowerSets[set]->ppPowers[i]->ppowOriginal==pow->ppowBase)
                iCnt++;
        }

        if(iCnt>=pow->ppowBase->iNumAllowed)
            return 0; // oh snap! we can't have any more of this
    }

    // Temp powers don't go through character_AllowedToHavePower anywhere else 
    return 1;
}

/**********************************************************************func*
* character_AddTemporaryPower
*
*/
bool character_AddTemporaryPower( Character *p, Power * pow )
{
    Power * newPow = character_AddRewardPower(p, pow->ppowBase);
    if( newPow )
    {    // this was a traded temp power so it keeps it old values
        newPow->iNumCharges = pow->iNumCharges;
        newPow->fUsageTime = pow->fUsageTime;
        newPow->fAvailableTime = pow->fAvailableTime;
        newPow->ulCreationTime = pow->ulCreationTime;
        newPow->instanceBought = pow->instanceBought;
        return 1;
    }

    return 0;
}

/**********************************************************************func*
* character_CanRemoveTemporaryPower
*
*/
bool character_CanRemoveTemporaryPower( Character *p, Power * pow )
{
    if( !p || !pow )
        return 0;

    // activating, recharging, or toggle that is on
    if( pow->fTimer || !pow->ppowBase->bTradeable || (pow->bActive && pow->ppowBase->eType != kPowerType_Auto ) )
        return 0;

    return 1;
}

const Power *character_GetPowerFromArrayIndices(Character *pchar, int iset, int ipow)
{
    assert(pchar != NULL);
    assert(iset >= 0 && iset < eaSize(&pchar->ppPowerSets));
    assert(ipow >= 0 && ipow < eaSize(&pchar->ppPowerSets[iset]->ppPowers));

    return pchar->ppPowerSets[iset]->ppPowers[ipow];
}

/**********************************************************************func*
 * character_BuyBoostSlot
 *
 */
bool character_BuyBoostSlot(Character *p, Power *ppow)
{
    assert(p!=NULL);
    assert(ppow!=NULL);

    if( ppow->iNumBoostsBought >= MAX_BOOSTS_BOUGHT
        || eaSize(&ppow->ppBoosts) >= ppow->ppowBase->iMaxBoosts)
    {
        return false;
    }

    ppow->iNumBoostsBought++;

    power_AddBoostSlot(ppow);

    return true;
}

/**********************************************************************func*
* character_RemoveBoostSlot
*
*/
bool character_RemoveBoostSlot(Character *p, Power *ppow)
{
    assert(p!=NULL);
    assert(ppow!=NULL);

    if( ppow->iNumBoostsBought <= 0 )
    {
        return false;
    }

    ppow->iNumBoostsBought--;

    power_RemoveBoostSlot(ppow);

    return true;
}


/**********************************************************************func*
* character_DisableBoosts
*
*/
bool character_DisableBoosts(Character *p)
{
    bool retval = true;

    if( p->bIgnoreBoosts )
    {
        retval = false;
    }
    else
    {
        // Only force a recalc if the state is actually changing.
        p->bRecalcStrengths = true;
    }

    p->bIgnoreBoosts = true;

    return retval;
}


/**********************************************************************func*
* character_EnableBoosts
*
*/
bool character_EnableBoosts(Character *p)
{
    bool retval = true;

    if( !p->bIgnoreBoosts )
    {
        retval = false;
    }
    else
    {
        // Need to recalc if we're turning from off to on.
        p->bRecalcStrengths = true;
    }

    p->bIgnoreBoosts = false;

    return retval;
}


/**********************************************************************func*
* character_DisableInspirations
*
*/
bool character_DisableInspirations(Character *p)
{
    bool retval = true;

    if( p->bIgnoreInspirations )
    {
        retval = false;
    }

    p->bIgnoreInspirations = true;

    return retval;
}


/**********************************************************************func*
* character_EnableInspirations
*
*/
bool character_EnableInspirations(Character *p)
{
    bool retval = true;

    if( !p->bIgnoreInspirations )
    {
        retval = false;
    }

    p->bIgnoreInspirations = false;

    return retval;
}


/**********************************************************************func*
 * character_GetFirstCategory
 *
 */
const PowerCategory *character_GetFirstCategory(Character *p, int *pos)
{
    assert(p!=NULL);
    assert(pos!=NULL);

    *pos = 0;

    return p->pclass->pcat[*pos];
}

/**********************************************************************func*
 * character_GetNextCategory
 *
 */
const PowerCategory *character_GetNextCategory(Character *p, int *pos)
{
    assert(p!=NULL);
    assert(pos!=NULL);

    (*pos)++;

    if(*pos<kCategory_Count)
    {
        return p->pclass->pcat[*pos];
    }

    return NULL;
}

/**********************************************************************func*
 * character_GetCategoryByIdx
 *
 */
const PowerCategory *character_GetCategoryByIdx(Character *p, int icat)
{
    assert(p!=NULL);
    assert(icat<kCategory_Count);

    return p->pclass->pcat[icat];
}

/**********************************************************************func*
 * character_NumPowersOwned
 *
 * Returns the number of powers owned over all builds.
 * Used to compare against the DB powers cap.
 *
 */
int character_NumPowersOwned(Character *p)
{
    int icnt=0;
    int i, l;
    int iCurBuild;
    int iSizeBuilds;

    assert(p != NULL);

    // TODO: I could cache this, I suppose.

    iCurBuild = character_GetActiveBuild(p->entParent);
    iSizeBuilds = ENTTYPE(p->entParent) == ENTTYPE_PLAYER ? MAX_BUILD_NUM : 1;
    for (l = 0; l < iSizeBuilds; l++)
    {
        character_SelectBuildLightweight(p->entParent, l);
        for (i = eaSize(&p->ppPowerSets) - 1; i >= 0; i--)
        {
            icnt += eaSize(&p->ppPowerSets[i]->ppPowers);
        }
    }

    character_SelectBuildLightweight(p->entParent, iCurBuild);

    return icnt;
}

/**********************************************************************func*
 * character_NumPowerSetsOwned
 *
 */
int character_NumPowerSetsOwned(Character *p)
{
    assert(p!=NULL);

    return eaSize(&p->ppPowerSets);
}

/**********************************************************************func*
 * character_VerifyPowerSets
 *
 */
void character_VerifyPowerSets(Character *p)
{
    int iCurBuild;
    int i;

    iCurBuild = p->iCurBuild;

    if (iCurBuild != 0 && eaSize(&p->ppPowerSets) <= g_numSharedPowersets + g_numAutomaticSpecificPowersets && 
                        eaSize(&p->ppBuildPowerSets[0]) > g_numSharedPowersets + g_numAutomaticSpecificPowersets)
    {
        for (i = 1; i < MAX_BUILD_NUM; i++)
        {
            if (eaSize(&p->ppBuildPowerSets[i]) <= g_numSharedPowersets + g_numAutomaticSpecificPowersets)
            {
                if (i != iCurBuild)
                {
                    character_SelectBuildLightweight(p->entParent, i);
                }
                character_AddNewPowerSet(p, powerdict_GetBasePowerSetByName(&g_PowerDictionary, p->pclass->pcat[kCategory_Primary]->pchName, p->entParent->pl->primarySet));
                character_AddNewPowerSet(p, powerdict_GetBasePowerSetByName(&g_PowerDictionary, p->pclass->pcat[kCategory_Secondary]->pchName, p->entParent->pl->secondarySet));
                if (i != iCurBuild)
                {
                    character_SelectBuildLightweight(p->entParent, iCurBuild);
                }
            }
        }
    }
}

/**********************************************************************func*
 * character_IsAllowedToHavePowerSetHypothetically
 *
 */
bool character_IsAllowedToHavePowerSetHypothetically(Character *p, const BasePowerSet *psetBase )
{
    return character_IsAllowedToHavePowerSetHypotheticallyAtSpecializedLevel(p, p->iLevel, psetBase, NULL);
}

bool character_IsAllowedToHavePowerSetHypotheticallyEx(Character *p, const BasePowerSet *psetBase, const char **outputMessage)
{
    return character_IsAllowedToHavePowerSetHypotheticallyAtSpecializedLevel(p, p->iLevel, psetBase, outputMessage);
}

/**********************************************************************func*
 * character_IsAllowedToHavePowerSetHypotheticallyAtSpecializedLevel
 *
 */
bool character_IsAllowedToHavePowerSetHypotheticallyAtSpecializedLevel(Character *p, int specializedLevel, const BasePowerSet *psetBase, const char **outputMessage)
{
    int i;
    int numPowers;

    // Some powersets are for specialization and only available under certain
    // circumstances.
    if (baseset_IsSpecialization(psetBase) && !character_IsAllowedToSpecializeInPowerSet(p, specializedLevel, psetBase))
    {
        return false;
    }

    if (psetBase->ppchSetBuyRequires && !chareval_Eval(p, psetBase->ppchSetBuyRequires, psetBase->pchSourceFile))
    {
        if (outputMessage)
            (*outputMessage) = psetBase->pchSetBuyRequiresFailedText;
        return false;
    }

    numPowers = eaSize(&psetBase->ppPowers);
    for(i=0; i<numPowers; i++)
    {
        if(character_IsAllowedToHavePowerHypothetically(p, psetBase->ppPowers[i]))
        {
            return true;
        }
    }

    return false;
}

/**********************************************************************func*
 * character_IsAllowedToHavePowerHypothetically
 *
 */
bool character_IsAllowedToHavePowerHypothetically(Character *p, const BasePower *ppowBase)
{
    bool bRet;

    assert(ppowBase!=NULL);

    // Are they high enough level to own this power?

    bRet = baseset_BasePowerAvailableByIdx( ppowBase->psetParent,
        ppowBase->id.ipow, character_GetLevel(p) ) <= 0;

    // Make sure they have all the pre-reqs.
    bRet = bRet && (!ppowBase->ppchBuyRequires || chareval_Eval(p, ppowBase->ppchBuyRequires, ppowBase->pchSourceFile));

    return bRet;
}

/**********************************************************************func*
 * character_IsAllowedToSpecializeInPowerSet
 *
 */
bool character_IsAllowedToSpecializeInPowerSet(Character *p, int specializedLevel, const BasePowerSet *psetBase)
{
    bool bRet = false;

    // It has to be a specialization set, we have to be high enough level to
    // specialize, and we have to pass the (optional) requirements.
    if (psetBase->iSpecializeAt)
    {
        if (specializedLevel >= psetBase->iSpecializeAt)
        {
            if (!psetBase->ppSpecializeRequires
                || chareval_Eval(p, psetBase->ppSpecializeRequires, psetBase->pchSourceFile))
            {
                bRet = true;
            }
        }
    }

    return bRet;
}

/**********************************************************************func*
 * character_WilLBeAllowedToSpecializeInPowerSet
 *
 */
bool character_WillBeAllowedToSpecializeInPowerSet(Character *p, const BasePowerSet *psetBase)
{
    bool bRet = false;

    // It has to be a specialization set and we must pass any requirements
    // but even if we're not high enough level now, we will be someday and
    // thus eligible in the future.
    if (psetBase->iSpecializeAt)
    {
        if (!psetBase->ppSpecializeRequires
            || chareval_Eval(p, psetBase->ppSpecializeRequires, psetBase->pchSourceFile))
        {
            bRet = true;
        }
    }

    return bRet;
}

/**********************************************************************func*
 * character_IsAllowedToHavePower
 *
 */
bool character_IsAllowedToHavePower(Character *p, const BasePower *ppowBase)
{
    // Any specialization sets are only available at their current level,
    // there's no respeccing or other thing that makes the specialization
    // availability different.
    return character_IsAllowedToHavePowerAtSpecializedLevel(p, p->iLevel, ppowBase);
}

/**********************************************************************func*
* character_WillBeAllowedToHavePower
*
*/
bool character_WillBeAllowedToHavePower(Character *p, const BasePower *ppowBase)
{
    // Any specialization sets are only available at their current level,
    // there's no respeccing or other thing that makes the specialization
    // availability different.
    return character_IsAllowedToHavePowerAtSpecializedLevel(p, 54, ppowBase);
}


/**********************************************************************func*
 * character_IsAllowedToHaveSpecializationPower
 *
 */
bool character_IsAllowedToHavePowerAtSpecializedLevel(Character *p, int specializedLevel, const BasePower *ppowBase)
{
    bool bRet;

    if (!ppowBase)
    {
        devassert(ppowBase);
        return false;
    }

    bRet = true;

    // Can't take a power from a specialisation set before it the set is
    // available.
    if (baseset_IsSpecialization(ppowBase->psetParent))
    {
        bRet = bRet && character_IsAllowedToSpecializeInPowerSet(p, specializedLevel, ppowBase->psetParent);
    }

    // Make sure they have all the pre-reqs.
    bRet = bRet && (!ppowBase->ppchBuyRequires || chareval_Eval(p, ppowBase->ppchBuyRequires, ppowBase->pchSourceFile));

    // You can only have one power with the same base power name. If they
    // already have it, then don't add the same basic thing.

    bRet = bRet && !character_OwnsPower(p, ppowBase);

    return bRet;
}

/**********************************************************************func*
* character_AtLevelCanUsePower
*
*/
bool character_AtLevelCanUsePower( Character *p, Power* ppow)
{
    if( ppow->ppowBase->bIgnoreLevelBought )
        return 1;

    if( ppow->psetParent && ppow->psetParent->psetBase && ppow->psetParent->psetBase->pcatParent &&
        (stricmp(ppow->psetParent->psetBase->pcatParent->pchName,"Temporary_Powers") != 0 || stricmp(ppow->psetParent->psetBase->pchName,"Accolades") == 0))
    {
        if (ppow->iLevelBought > MIN(p->iCombatLevel + EXEMPLAR_GRACE_LEVELS, p->iLevel))
        {
            return 0;
        }

    }
    return 1;
}



/**********************************************************************func*
 * character_ModesAllowPower
 *
 */
bool character_ModesAllowPower(Character *p, const BasePower *ppowBase)
{
    int iCntModes;
    int iCntRequired;
    bool bRet = false;

    assert(p!=NULL);
    assert(ppowBase!=NULL);

    iCntModes = eaiSize(&p->pPowerModes);
    iCntRequired = eaiSize(&ppowBase->pModesRequired);

    if(iCntRequired==0)
    {
        bRet = true;
    }
    else
    {
        int i;
        for(i=0; i<iCntRequired && !bRet; i++)
        {
            int j;
            for(j=0; j<iCntModes; j++)
            {
                if(ppowBase->pModesRequired[i]==p->pPowerModes[j])
                {
                    bRet = true;
                    break;
                }
            }
        }
    }

    if(bRet)
    {
        int iCntDisallowed = eaiSize(&ppowBase->pModesDisallowed);
        if(iCntModes!=0)
        {
            int i;
            for(i=0; i<iCntDisallowed && bRet; i++)
            {
                int j;
                for(j=0; j<iCntModes; j++)
                {
                    if(p->pPowerModes[j]==ppowBase->pModesDisallowed[i])
                    {
                        bRet = false;
                        break;
                    }
                }
            }
        }
    }

    return bRet;
}

/**********************************************************************func*
 * character_SetPowerMode
 *
 */
void character_SetPowerMode(Character *p, PowerMode mode)
{
    int i;
    int n;

    assert(p!=NULL);
    assert(mode>=0);

    n = eaiSize(&p->pPowerModes);
    for(i=0; i<n; i++)
    {
        if(p->pPowerModes[i]==mode)
        {
            return;
        }
    }

    eaiPush(&p->pPowerModes, mode);
    p->bRecalcEnabledState = true;
}

/**********************************************************************func*
 * character_UnsetPowerMode
 *
 */
void character_UnsetPowerMode(Character *p, PowerMode mode)
{
    assert(p!=NULL);

    if(mode>=0)
    {
        int i;
        int n = eaiSize(&p->pPowerModes);
        for(i=0; i<n; i++)
        {
            if(p->pPowerModes[i]==mode)
            {
                eaiRemove(&p->pPowerModes, i);
                p->bRecalcEnabledState = true;
                return;
            }
        }
    }
}

bool character_IsInPowerMode(Character *p, PowerMode mode)
{
    if(mode>=0)
    {
        int i;
        int n = eaiSize(&p->pPowerModes);
        for(i=0; i<n; i++)
        {
            if(p->pPowerModes[i]==mode)
            {
                return true;
            }
        }
    }

    return false;
}

#if SERVER
bool character_DeathAllowsPower(Character *p, const BasePower *ppowBase)
{
    switch (ppowBase->eDeathCastableSetting)
    {
    case kDeathCastableSetting_AliveOnly:
        return p->attrCur.fHitPoints > 0;
    case kDeathCastableSetting_DeadOnly:
        return !entAlive(p->entParent);
    case kDeathCastableSetting_DeadOrAlive:
    default:
        return true;
    }
}
#endif

/**********************************************************************func*
 * character_GrantEntireCategory
 *
 * Limited to MAX_POWERS powers for database size purposes.
 */
void character_GrantEntireCategory(Character *p, const PowerCategory *pcat, int iLevel)
{
    int i;
    int iSizeSets;
    int numPowersOwned = character_NumPowersOwned(p);

    if (numPowersOwned >= MAX_POWERS - 1)
    {
        return;
    }

    assert(pcat!=NULL);

    iSizeSets = eaSize(&pcat->ppPowerSets);
    for(i=0; i<iSizeSets; i++)
    {
        int j;
        int iSizePows;
        const BasePowerSet *psetBase = pcat->ppPowerSets[i];

        iSizePows = eaSize(&psetBase->ppPowers);
        for(j=0; j<iSizePows; j++)
        {
            if(psetBase->piAvailable[j]<=iLevel)
            {
                Power *ppow;
                PowerSet *pset;
                const BasePower *ppowBase = psetBase->ppPowers[j];

                if(character_OwnsPower(p, ppowBase)==NULL && numPowersOwned < MAX_POWERS - 1)
                {
                    if((pset=character_OwnsPowerSet(p, psetBase))==NULL)
                    {
                        // Only add the power set if there are powers in it to
                        // give.
                        pset = character_BuyPowerSet(p, psetBase);
                    }

                    ppow = character_BuyPower(p, pset, ppowBase, 0);
                    numPowersOwned++;
#if SERVER
                    eaPush(&p->entParent->powerDefChange, powerRef_CreateFromPower(p->iCurBuild, ppow));
#endif
                }
            }
        }
    }
}

/**********************************************************************func*
 * character_GrantAllPowers
 *
 */
void character_GrantAllPowers(Character *p)
{
    const PowerCategory *pcat;
    int pos;

    assert(p!=NULL);

    pcat = character_GetFirstCategory(p, &pos);
    while(pcat!=NULL)
    {
        character_GrantEntireCategory(p, pcat, 99);

        pcat = character_GetNextCategory(p, &pos);
    }

#if SERVER
    // This forces a full redefine to be sent.
    eaPush(&p->entParent->powerDefChange, (void *)-1);
#endif
}

/**********************************************************************func*
 * character_GrantAutoIssueBuildSharedPowerSets
 *
 */
void character_GrantAutoIssueBuildSharedPowerSets(Character *p)
{
    if(ENTTYPE(p->entParent)==ENTTYPE_PLAYER)
    {
        int sharedIndex;

        for (sharedIndex = eaSize(&g_SharedPowersets) - 1; sharedIndex >= 0; sharedIndex--)
        {
            character_AddNewPowerSet(p, g_SharedPowersets[sharedIndex]);
        }
    }
    else
    {
        const BasePowerSet *psetBase;

        // still hard coded
        psetBase = powerdict_GetBasePowerSetByName(&g_PowerDictionary, "Temporary_Powers", "Temporary_Powers");
        character_AddNewPowerSet(p, psetBase);
    }
}

/**********************************************************************func*
 * character_GrantAutoIssueBuildSpecificPowerSets
 *
 */
void character_GrantAutoIssueBuildSpecificPowerSets(Character *p)
{
    if(ENTTYPE(p->entParent)==ENTTYPE_PLAYER)
    {
        const BasePowerSet *psetBase;

        // explicit hack, matches g_numAutomaticSpecificPowersets
        psetBase = powerdict_GetBasePowerSetByName(&g_PowerDictionary, "Inherent", "Inherent");
        character_BuyPowerSet(p, psetBase);
        psetBase = powerdict_GetBasePowerSetByName(&g_PowerDictionary, "Inherent", "Fitness");
        character_BuyPowerSet(p, psetBase);
    }
}

/**********************************************************************func*
 * character_GrantAutoIssuePowers
 *
 */
void character_GrantAutoIssuePowers(Character *p)
{
    int i;
    int iSizeSets;

    iSizeSets = eaSize(&p->ppPowerSets);
    for(i=0; i<iSizeSets; i++)
    {
        int j;
        int iSizePows;
        PowerSet *pset = p->ppPowerSets[i];
        const BasePowerSet *psetBase = pset->psetBase;

        iSizePows = eaSize(&psetBase->ppPowers);
        for(j=0; j<iSizePows; j++)
        {
            if(psetBase->piAvailable[j] <= p->iLevel)
            {
                Power *ppow;
                const BasePower *ppowBase = psetBase->ppPowers[j];

                if(ppowBase->bAutoIssue
                    && character_OwnsPower(p, ppowBase)==NULL
                    && character_IsAllowedToHavePower(p, ppowBase))
                {
                    ppow = character_BuyPower(p, pset, ppowBase, 0);
                    if(ppow)
                    {
                        power_LevelFinalize(ppow);
#if SERVER
                        eaPush(&p->entParent->powerDefChange, powerRef_CreateFromPower(p->iCurBuild, ppow));
#endif
                    }
                }
            }
        }
    }
}

/**********************************************************************func*
 * character_CheckEntireCategoryPresent
 *
 */
int character_CheckEntireCategoryPresent(Character *p, const PowerCategory *pcat, int iLevel)
{
    int i;
    int iSizeSets;

    assert(pcat!=NULL);

    iSizeSets = eaSize(&pcat->ppPowerSets);
    for(i=0; i<iSizeSets; i++)
    {
        int j;
        int iSizePows;
        PowerSet *pset = NULL;
        const BasePowerSet *psetBase = pcat->ppPowerSets[i];

        iSizePows = eaSize(&psetBase->ppPowers);
        for(j=0; j<iSizePows; j++)
        {
            if(psetBase->piAvailable[j]<=iLevel)
            {
                const BasePower *ppowBase = psetBase->ppPowers[j];

                if((pset=character_OwnsPowerSet(p, psetBase))==NULL)
                {
                    return 0;
                }

                if(character_OwnsPower(p, ppowBase)==NULL)
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}


/**********************************************************************func*
 * character_CheckAllPowersPresent
 *
 */
int character_CheckAllPowersPresent(Character *p)
{
    const PowerCategory *pcat;
    int pos;

    assert(p!=NULL);

    pcat = character_GetFirstCategory(p, &pos);
    while(pcat!=NULL)
    {
        if(!character_CheckEntireCategoryPresent(p, pcat, 99))
        {
            return 0;
        }

        pcat = character_GetNextCategory(p, &pos);
    }

    return 1;
}

/***************************************************************************
 * character_SlotPowerFromSource
 *
 */
void character_SlotPowerFromSource(Power *ppow, Power *ppowSrc, int iLevelChange)
{
    int i;
    int iCntBoosts;
    int iCnt = 0;

    if( !ppow || !ppowSrc)
        return;

#if SERVER && DEBUG_SLOTTING
    printf("Slotting %s from %s\n", dbg_BasePowerStr(ppow->ppowBase), dbg_BasePowerStr(ppowSrc->ppowBase));
#endif
    iCntBoosts = eaSize(&ppowSrc->ppBoosts);
    for(i=0; i<iCntBoosts; i++)
    {
        Boost *pbSrc = ppowSrc->ppBoosts[i];

        if(pbSrc)
        {
            int iLvl = pbSrc->iLevel+iLevelChange;

            if (pbSrc->ppowBase && pbSrc->ppowBase->bBoostUsePlayerLevel)
                iLvl = pbSrc->iLevel;

#if SERVER && DEBUG_SLOTTING
            printf("\tBoost: %s.%d+%d ", dbg_BasePowerStr(pbSrc->ppowBase), pbSrc->iLevel, pbSrc->iNumCombines);
#endif
            if(iLvl>=0 && power_IsValidBoostIgnoreOrigin(ppow, pbSrc))
            {
                Boost *pb = boost_Create(pbSrc->ppowBase, iCnt, ppow, iLvl, pbSrc->iNumCombines, NULL, 0, NULL, 0);
                if(eaSize(&ppow->ppBoosts)<=iCnt)
                {
                    power_AddBoostSlot(ppow);
                }
                eaSet(&ppow->ppBoosts, pb, iCnt);
#if SERVER && DEBUG_SLOTTING
                printf("added at %d\n", iCnt);
#endif

                iCnt++;
            }
#if SERVER && DEBUG_SLOTTING
            else
            {
                int i;
                printf("NOT added: iLvl(%d+%d=>%d) < 0 or not allowed type.\n", pbSrc->iLevel, iLevelChange, iLvl);
                printf("\t\t%s: ", dbg_BasePowerStr(ppow->ppowBase));
                for(i=eaiSize(&(int *)ppow->ppowBase->pBoostsAllowed)-1; i>=0; i--)
                {
                    printf("%d, ", ppow->ppowBase->pBoostsAllowed[i]);
                }
                printf("\n\t\t%s: ", dbg_BasePowerStr(pbSrc->ppowBase));
                for(i=eaiSize(&(int *)pbSrc->ppowBase->pBoostsAllowed)-1; i>=0; i--)
                {
                    printf("%d, ", pbSrc->ppowBase->pBoostsAllowed[i]);
                }
                printf("\n");
            }
#endif
        }
    }

    iCnt = 0;
    iCntBoosts = eaSize(&ppowSrc->ppGlobalBoosts);
    for(i=0; i<iCntBoosts; i++)
    {
        Boost *pbSrc = ppowSrc->ppGlobalBoosts[i];

        if(pbSrc)
        {
            int iLvl = pbSrc->iLevel+iLevelChange;
#if SERVER && DEBUG_SLOTTING
            printf("\tGlobal Boost: %s.%d+%d ", dbg_BasePowerStr(pbSrc->ppowBase), pbSrc->iLevel, pbSrc->iNumCombines);
#endif
            if(iLvl>=0 && power_IsValidBoostIgnoreOrigin(ppow, pbSrc))
            {
                Boost *pb = boost_Create(pbSrc->ppowBase, 0, ppow, iLvl, 0, NULL, 0, NULL, 0);
                eaPush(&ppow->ppGlobalBoosts, pb);
#if SERVER && DEBUG_SLOTTING
                printf("added at %d\n", iCnt);
#endif

                iCnt++;
            }
#if SERVER && DEBUG_SLOTTING
            else
            {
                int i;
                printf("NOT added: iLvl(%d+%d=>%d) < 0 or not allowed type.\n", pbSrc->iLevel, iLevelChange, iLvl);
                printf("\t\t%s: ", dbg_BasePowerStr(ppow->ppowBase));
                for(i=eaiSize(&(int *)ppow->ppowBase->pBoostsAllowed)-1; i>=0; i--)
                {
                    printf("%d, ", ppow->ppowBase->pBoostsAllowed[i]);
                }
                printf("\n\t\t%s: ", dbg_BasePowerStr(pbSrc->ppowBase));
                for(i=eaiSize(&(int *)pbSrc->ppowBase->pBoostsAllowed)-1; i>=0; i--)
                {
                    printf("%d, ", pbSrc->ppowBase->pBoostsAllowed[i]);
                }
                printf("\n");
            }
#endif
        }
    }
}

/**********************************************************************func*
 * character_AddRewardPower
 *
 */
Power *character_AddRewardPower(Character *p, const BasePower *ppowBase)
{
    Power *ppow;
    PowerSet *pset;
    bool bAdded;

    if (!p)
    {
        Errorf("character_AddRewardPower() received a null Character!");
        return NULL;
    }

    if (!ppowBase)
    {
        Errorf("character_AddRewardPower() received a null BasePower!");
        return NULL;
    }

    pset = character_BuyPowerSet(p, ppowBase->psetParent);
    ppow = character_BuyPowerEx(p, pset, ppowBase, &bAdded, 0, 0, NULL); // checks for duplicates, etc.

#if SERVER
    if(ppow!=NULL && bAdded)
    {
        if(p->entParent && p->entParent->erCreator)
            character_PetSlotPower(p,ppow);
        eaPush(&p->entParent->powerDefChange, powerRef_CreateFromPower(p->iCurBuild, ppow));
        character_ActivateAllAutoPowers(p);
    }
#endif

    return bAdded ? ppow : NULL;
}

/**********************************************************************func*
 * CleanUpRemovedPower
 *
 */
static void CleanUpRemovedPower(Character *p, Power *ppow)
{
#if SERVER
    PowerListItem *ppowListItem;
    PowerListIter iter;

    if(p==NULL || ppow==NULL)
    {
        return;
    }

    if(p->ppowStance==ppow)
    {
        character_EnterStance(p, NULL, 1);
        p->ppowStance=NULL;
    }

    // Make sure this power is no longer referenced anywhere.
    // Most of these may be redundant, but it's best to be careful.
    if(p->ppowTried==ppow) 
    {
        character_ReleasePowerChain(p, p->ppowTried);
        p->ppowTried=NULL;
    }
    if(p->ppowQueued==ppow) 
    {
        character_ReleasePowerChain(p, p->ppowQueued);
        p->ppowQueued=NULL;
    }
    if(p->ppowDefault==ppow)
    {
        character_ReleasePowerChain(p, p->ppowDefault);
        p->ppowDefault=NULL;
    }
    if(p->ppowCurrent==ppow)
    {
        character_ReleasePowerChain(p, p->ppowCurrent);
        p->ppowCurrent=NULL;
    }

    // Yank it out of the auto list if it's there
    if (ppow->ppowBase->eType == kPowerType_Auto)
    {
        character_ShutOffAndRemoveAutoPower(p, ppow);
    }

    // And get rid of it from the toggles list if it's there.
    if (ppow->ppowBase->eType == kPowerType_Toggle)
    {
        character_ShutOffAndRemoveTogglePower(p, ppow);
    }

    // And get rid of it from all powers' global boost lists.
    if (ppow->ppowBase->eType == kPowerType_GlobalBoost)
    {
        character_ShutOffAndRemoveGlobalBoost(p, ppow);
    }

    if (ppow->bEnabled)
    {
        // This will trigger onDisable mods, which we want when cleaning up the power.
        //   Technically the power isn't being manually disabled.
        character_ManuallyDisablePower(p, ppow);
    }

    // Yank it out of the recharge list if it's there
    // Must be done after shutting off Auto and Toggle powers.
    for(ppowListItem = powlist_GetFirst(&p->listRechargingPowers, &iter);
        ppowListItem != NULL;
        ppowListItem = powlist_GetNext(&iter))
    {
        if( ppow == ppowListItem->ppow )
            powlist_RemoveCurrent(&iter);
    }

    eaPush(&p->entParent->powerDefChange, powerRef_CreateFromPower(p->iCurBuild, ppow));
#endif
}

/**********************************************************************func*
 * character_RemovePower
 *
 */
void character_RemovePower(Character *p, Power *ppow)
{
    if(ppow && ppow->psetParent->pcharParent == p)
    {
        CleanUpRemovedPower(p, ppow);
        powerset_DeletePower(ppow->psetParent, ppow, NULL);
    }
}

/**********************************************************************func*
 * character_RemoveBasePower
 *
 */
void character_RemoveBasePower(Character *p, const BasePower *ppowBase)
{
    //There used to be special case code for Temporary powers that did the exact
    //Same thing as the general code.  Removed.
    Power *ppow = character_OwnsPower(p, ppowBase);
    if(ppow)
    {
        character_RemovePower(p, ppow);
    }
}

/**********************************************************************func*
 * character_AddInspiration
 *
 */
int character_AddInspiration(Character *p, const BasePower *ppowBase, const char *context)
{
    int i;
    int j;

    assert(p!=NULL);
    assert(ppowBase!=NULL);

    DATA_MINE_INSPIRATION(ppowBase,"in",context,1);
    for(i=0; i<p->iNumInspirationRows; i++)
    {
        for(j=0; j<p->iNumInspirationCols; j++)
        {
            if(p->aInspirations[j][i]==NULL)
            {
                character_SetInspiration(p, ppowBase, j, i);

#ifdef SERVER
                // did this fill up the inventory?
                if (character_InspirationInventoryFull(p))
                    serveFloater(p->entParent, p->entParent, "FloatInspirationsFull", 0.0f, kFloaterStyle_Attention, 0xff0000ff);
#endif
                return j*CHAR_INSP_MAX_ROWS+i;
            }
        }
    }
    DATA_MINE_INSPIRATION(ppowBase,"out","inventory_full",1);

    return -1;
}

/**********************************************************************func*
 * character_SetInspiration
 *
 * Setting to NULL is OK, but RemoveInspiration is the right way to do it.
 *
 */
void character_SetInspiration(Character *p, const BasePower *ppowBase, int iCol, int iRow)
{
    assert(p!=NULL);
    assert(iCol>=0);
    assert(iRow>=0);

    if(iCol<p->iNumInspirationCols && iRow<p->iNumInspirationRows)
    {
        p->aInspirations[iCol][iRow] = ppowBase;

#ifdef SERVER
        eaiPush(&p->entParent->inspirationStatusChange, iCol*CHAR_INSP_MAX_ROWS+iRow);
#endif
    }
}

/**********************************************************************func*
 * character_RemoveInspiration
 *
 */
bool character_RemoveInspiration(Character *pChar, int iCol, int iRow, const char *context)
{
    Character * p = pChar;

    assert(p!=NULL);
    assert(iCol>=0);
    assert(iRow>=0);

    if(p->entParent->erOwner)
    {
        Entity *e = erGetEnt(p->entParent->erOwner);
        if( e )
            p = e->pchar;
        else
            return false;
    }

    if(iCol<p->iNumInspirationCols && iRow<p->iNumInspirationRows && p->aInspirations[iCol][iRow])
    {
        DATA_MINE_INSPIRATION(p->aInspirations[iCol][iRow],"out",context,1);
        p->aInspirations[iCol][iRow] = NULL;

#ifdef SERVER
        eaiPush(&p->entParent->inspirationStatusChange, iCol*CHAR_INSP_MAX_ROWS+iRow);
        if (g_MapIsTutorial)
            SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, CODEOBJ_USED_INSPIRATION);

#endif
        return true;
    }
    else
        return false;
}

/**********************************************************************func*
* character_RemoveAllInspirations
*
*/
void character_RemoveAllInspirations(Character *p, const char *context)
{
    int i, j;
    assert(p!=NULL);

    for( i = 0; i < p->iNumInspirationCols; i++ )
    {
        for( j = 0; j < p->iNumInspirationRows; j++ )
        {
            character_RemoveInspiration(p, i, j, context);
        }
    }
}

/**********************************************************************func*
* character_MergeInspiration
*
*/
void character_MergeInspiration( Entity * e, char * sourceName, char * destName )
{
    bool bFoundSrc = 0;
    bool bFoundDest = 0;
    int i = 0, j, cols, rows, count = 0;
    const BasePower * pSrc, *pDest;

    if(!e || !e->pchar)
        return;

    pSrc = basePower_inspirationGetByName("Large", sourceName );
    pDest = basePower_inspirationGetByName("Large", destName );
    if( pSrc && pDest )
    {
        while( g_largeInspirations[i] )
        {
            if( stricmp(g_largeInspirations[i],sourceName)==0)
                bFoundSrc = 1;
            if( stricmp(g_largeInspirations[i],destName)==0)
                bFoundDest = 1;
            i++;
        }
    }
    else
    {
        pSrc = basePower_inspirationGetByName("Medium", sourceName );
        pDest = basePower_inspirationGetByName("Medium", destName );
        if( pSrc && pDest )
        {
            while( g_mediumInspirations[i] )
            {
                if( stricmp(g_mediumInspirations[i],sourceName)==0)
                    bFoundSrc = 1;
                if( stricmp(g_mediumInspirations[i],destName)==0)
                    bFoundDest = 1;
                i++;
            }
        }
        else
        {
            pSrc = basePower_inspirationGetByName("Small", sourceName );
            pDest = basePower_inspirationGetByName("Small", destName );
            if( pSrc && pDest )
            {
                while( g_smallInspirations[i] )
                {
                    if( stricmp(g_smallInspirations[i],sourceName)==0)
                        bFoundSrc = 1;
                    if( stricmp(g_smallInspirations[i],destName)==0)
                        bFoundDest = 1;
                    i++;
                }
            }
        }
    }

    if(!bFoundSrc || !bFoundDest)
        return;

    // make sure there are 3 sources
    cols = e->pchar->iNumInspirationCols;
    rows = e->pchar->iNumInspirationRows;
    for( i = 0; i < cols; i++ )
    {
        for( j = 0; j < rows; j++ )
        {
            if( pSrc == e->pchar->aInspirations[i][j] )
                count++;
        }
    }

    if( count < 3 )
        return;

    count = 0;
    for( i = 0; i < cols; i++ )
    {
        for( j = 0; j < rows && count < 3; j++ )
        {
            if( pSrc == e->pchar->aInspirations[i][j] )
            {
                character_RemoveInspiration(e->pchar, i, j, "InspMerge");
                count++;
            }
        }
    }
    character_AddInspiration(e->pchar, pDest, "InspMerge");
    character_CompactInspirations(e->pchar);

}

/**********************************************************************func*
* character_GiveInpirationByName
*
* utility for GiveArenaInspirations
*
*/
void character_AddInspirationByName(Character* p, char* set, char* power, const char *context)
{
    const BasePower *ppow = powerdict_GetBasePowerByName(&g_PowerDictionary, "Inspirations", set, power);
    if(ppow!=NULL)
    {
        character_AddInspiration(p, ppow, context);
    }
}

const BasePower * basePower_inspirationGetByName(char* set, char* power)
{
    return powerdict_GetBasePowerByName(&g_PowerDictionary, "Inspirations", set, power);
}

#ifdef SERVER
//
// By fiat, you can not delete an inspiration in the tutorial mission,
//  (certain restrictions apply)
static int CanNotDeleteInspirationInTutorial(void)
{
    if (g_MapIsMissionMap && g_MapIsTutorial && !ObjectiveIsComplete(CODEOBJ_USED_INSPIRATION))
        return true;
    return false;
}
#endif

/**********************************************************************func*
* character_MoveInspiration
*
*/
bool character_MoveInspiration(Character *p, int srcCol, int srcRow, int destCol, int destRow)
{
    assert(p!=NULL);
    assert(srcCol>=0);
    assert(srcRow>=0);

#ifdef SERVER
    if( character_IsInspirationSlotInUse(p, srcCol,  srcRow) ||
        character_IsInspirationSlotInUse(p, destCol, destRow ) )
    {
        return true; // don't do anything, but its legal for this to fail
    }
#endif

    if( destCol == -1 || destRow == -1 )
    {
#ifdef SERVER
        if (CanNotDeleteInspirationInTutorial())
        {
            return true;    //Do nothing.
        }
        if(srcCol<p->iNumInspirationCols && srcRow<p->iNumInspirationRows && p->aInspirations[srcCol][srcRow])
            dbLog("RemoveInspiration",p->entParent,"Removing %s",p->aInspirations[srcCol][srcRow]->pchName);
#endif
        character_RemoveInspiration( p, srcCol, srcRow, "trash" );
    }
    else if (srcCol < 0 || srcCol >= CHAR_INSP_MAX_COLS || srcRow < 0 || srcRow >= CHAR_INSP_MAX_ROWS ||
        destCol < 0 || destCol >= CHAR_INSP_MAX_COLS || destRow < 0 || destRow >= CHAR_INSP_MAX_ROWS)
    {
        return false;
    }
    else
    {
        const BasePower *ppow = p->aInspirations[destCol][destRow];
        p->aInspirations[destCol][destRow] = p->aInspirations[srcCol][srcRow];
        p->aInspirations[srcCol][srcRow] = ppow;

#ifdef SERVER
        // TODO: During compact, this might end up sending two changes for some
        //       slots.
        eaiPush(&p->entParent->inspirationStatusChange, srcCol*CHAR_INSP_MAX_ROWS+srcRow );
        eaiPush(&p->entParent->inspirationStatusChange, destCol*CHAR_INSP_MAX_ROWS+destRow);
#endif
    }
    return true;
}

/**********************************************************************func*
 * character_CompactInspirations
 *
 */
void character_CompactInspirations(Character *pChar)
{
    int i, j;

    Character * p = pChar;

    assert(p!=NULL);

    if(p->entParent->erOwner)
    {
        Entity *e = erGetEnt(p->entParent->erOwner);
        if( e )
            p = e->pchar;
        else
            return;
    }


    for(i=0; i<p->iNumInspirationCols; i++)
    {
        for(j=1; j<p->iNumInspirationRows; j++)
        {
            if(p->aInspirations[i][j] != NULL)
            {
                int k = j-1;

                while(k >= 0 && p->aInspirations[i][k] == NULL)
                {
                    character_MoveInspiration( p, i, k+1, i, k );
                    k--;
                }
            }
        }
    }
}

/**********************************************************************func*
* character_InspirationInventoryFull
*
*/
bool character_InspirationInventoryFull(Character *p)
{
    int i, j;
    assert(p!=NULL);

    for( i = 0; i < p->iNumInspirationCols; i++ )
    {
        for( j = 0; j < p->iNumInspirationRows; j++ )
        {
            if( !p->aInspirations[i][j] )
                return false;
        }
    }

    return true;
}


/**********************************************************************func*
* character_InspirationInventoryEmptySlots
*
*/
int character_InspirationInventoryEmptySlots(Character *p)
{
    int i, j;
    int retval = 0;
    assert(p!=NULL);

    for( i = 0; i < p->iNumInspirationCols; i++ )
    {
        for( j = 0; j < p->iNumInspirationRows; j++ )
        {
            if( !p->aInspirations[i][j] )
                retval++;
        }
    }

    return retval;
}

/**********************************************************************func*
* character_AddBoostEx
*
*/
static int character_AddBoostEx(Character *p, const BasePower *ppowBase, int iLevel, int iNumCombines, const char *context, int ignoreBoostLimit, int index)
{
    int i;

    assert(p!=NULL);
    assert(ppowBase!=NULL);

    DATA_MINE_ENHANCEMENT(ppowBase,iLevel,"in",context,1);
    if (ignoreBoostLimit || !character_BoostInventoryFull(p))
    {
        int start, end;
        if (index < 0)
        {
            start = 0;
            end = CHAR_BOOST_MAX;
        }
        else
        {
            start = index;
            end = index + 1;
        }
        
        for(i = start; i < end; i++)
        {
            if(p->aBoosts[i]==NULL)
            {
                character_SetBoost(p, i, ppowBase, iLevel, iNumCombines, NULL, 0, NULL, 0);

#ifdef SERVER
                // show the full message if weren't not placing an enhancement in a specific spot.
                if (index < 0)
                {
                    // did this fill up the inventory?
                    if (character_BoostInventoryFull(p))
                        serveFloater(p->entParent, p->entParent, "FloatEnhancementsFull", 0.0f, kFloaterStyle_Attention, 0xff0000ff);
                }
#endif

                return i;
            }
        }
    }
    DATA_MINE_ENHANCEMENT(ppowBase,iLevel,"out","inventory_full",1);

    return -1;
}

/**********************************************************************func*
* character_AddBoost
*
*/
int character_AddBoost(Character *p, const BasePower *ppowBase, int iLevel, int iNumCombines, const char *context)
{
    return character_AddBoostEx(p, ppowBase, iLevel, iNumCombines, context, 0, -1);
}

/**********************************************************************func*
* character_AddBoostIgnoreLimit
*
*/
int character_AddBoostIgnoreLimit(Character *p, const BasePower *ppowBase, int iLevel, int iNumCombines, const char *context)
{
    return character_AddBoostEx(p, ppowBase, iLevel, iNumCombines, context, 1, -1);
}

/**********************************************************************func*
* character_AddBoostSpecificIndex
*
*/
int character_AddBoostSpecificIndex(Character *p, const BasePower *ppowBase, int iLevel, int iNumCombines, const char *context, int index)
{
    return character_AddBoostEx(p, ppowBase, iLevel, iNumCombines, context, 1, index);
}

/**********************************************************************func*
 * character_SetBoost
 *
 * Setting to NULL is OK, but RemoveBoost is the right way to do it.
 *
 */
void character_SetBoost(Character *p, int idx, const BasePower *ppowBase, int iLevel, int iNumCombines,  float const *afVars, int numVars, PowerupSlot const *aPowerupSlots, int numPowerupSlots)
{
    assert(p!=NULL);
    assert(idx>=0);

    if(idx < CHAR_BOOST_MAX)
    {
        boost_Destroy( p->aBoosts[idx] );
        p->aBoosts[idx] = boost_Create(ppowBase, idx, NULL, iLevel, iNumCombines, afVars, numVars, aPowerupSlots, numPowerupSlots);

#ifdef SERVER
        eaiPush(&p->entParent->boostStatusChange, idx);
#endif
    }
}

/**********************************************************************func*
 * character_BoostsSetLevel
 *
 * Sets all slotted boosts to given level, erases combines.
 *
*/
void character_BoostsSetLevel(Character *p, int iLevel)
{
    int i,j,k;

    assert(p!=NULL);
    iLevel = (iLevel<0)?0:iLevel;

    for(i=eaSize(&p->ppPowerSets)-1; i>=0; i--)
    {
        for(j=eaSize(&p->ppPowerSets[i]->ppPowers)-1; j>=0; j--)
        {
            Power *ppow = p->ppPowerSets[i]->ppPowers[j];
            if(ppow!=NULL && ppow->ppBoosts!=NULL)
            {
                for(k=eaSize(&ppow->ppBoosts)-1; k>=0; k--)
                {
                    if (ppow->ppBoosts[k]!=NULL)
                    {
                        ppow->ppBoosts[k]->iLevel = iLevel;
                        ppow->ppBoosts[k]->iNumCombines = 0;
                        ppow->bBoostsCalcuated = false;
                    }
                }
            }
        }
    }

#ifdef SERVER
    eaPush(&p->entParent->powerDefChange, S32_TO_PTR(-1));
#endif

    return;
}


//------------------------------------------------------------
//  Helper for copying a boost from a boost, a common operation.
//----------------------------------------------------------
void character_SetBoostFromBoost(Character *p, int idx, Boost const *srcBoost)
{
    if( verify( srcBoost && p ))
    {
        int numSlots = powerupslot_ValidCount( srcBoost->aPowerupSlots, ARRAY_SIZE( srcBoost->aPowerupSlots ));

        character_SetBoost( p, idx, srcBoost->ppowBase, srcBoost->iLevel, srcBoost->iNumCombines,
                            srcBoost->afVars, ARRAY_SIZE(srcBoost->afVars),
                            srcBoost->aPowerupSlots, numSlots );
    }
}

//------------------------------------------------------------
//  Helper for swapping two boosts in inventory
// note: this is symmetric, either idx0 or idx1, or both can
// be NULL.
//----------------------------------------------------------
void character_SwapBoosts(Character *p, int idx0, int idx1)
{
    Boost *tmp = p->aBoosts[idx1] ? boost_Clone(p->aBoosts[idx1]) : NULL;

    // copy 0 to 1
    if( p->aBoosts[idx0] )
    {
        character_SetBoostFromBoost(p, idx1, p->aBoosts[idx0]);
    }
    else
    {
        character_DestroyBoost(p, idx1, NULL);
    }


    // if there was something at 1, copy it to 0
    // else destroy 0's boost.
    if( tmp )
    {
        character_SetBoostFromBoost(p, idx0, tmp );
        boost_Destroy( tmp );
    }
    else
    {
        character_DestroyBoost(p, idx0, NULL);
    }
}


/**********************************************************************func*
 * character_DestroyBoost
 *
 */
void character_DestroyBoost(Character *p, int idx, const char *context)
{
    assert(p!=NULL);
    assert(idx>=0);

    if( idx < CHAR_BOOST_MAX )
    {
        Boost *pboost = p->aBoosts[idx];
        if(pboost)
            DATA_MINE_ENHANCEMENT(pboost->ppowBase,pboost->iLevel,"out",context,1);
        boost_Destroy(pboost);
        p->aBoosts[idx] = NULL;

#ifdef SERVER
        eaiPush(&p->entParent->boostStatusChange, idx);
#endif
    }
}

/**********************************************************************func*
 * character_RemoveBoost
 *
 */
Boost *character_RemoveBoost(Character *p, int idx, const char *context)
{
    Boost *pboost = NULL;

    assert(p!=NULL);
    assert(idx>=0);

    if(idx < CHAR_BOOST_MAX)
    {
        pboost = p->aBoosts[idx];
        p->aBoosts[idx] = NULL;
        if(pboost)
            DATA_MINE_ENHANCEMENT(pboost->ppowBase,pboost->iLevel,"out",context,1);

#ifdef SERVER
        eaiPush(&p->entParent->boostStatusChange, idx);
#endif
    }

    return pboost;
}

/**********************************************************************func*
* character_BoostInventoryFull
*
*/
bool character_BoostInventoryFull(Character *p)
{
    int i;
    int occupied = 0;
    assert(p!=NULL);

    for( i = 0; i < CHAR_BOOST_MAX; i++ )
    {
        if( p->aBoosts[i] )
            ++occupied;
    }

    return occupied >= character_GetBoostInvTotalSize(p);
}


/**********************************************************************func*
* character_BoostInventoryEmptySlots
*
*/
int character_BoostInventoryEmptySlots(Character *p)
{
    int i;
    int occupied = 0;
    assert(p!=NULL);

    for( i = 0; i < CHAR_BOOST_MAX; i++ )
    {
        if( p->aBoosts[i] )
            ++occupied;
    }

    return MAX(p->iNumBoostSlots - occupied, 0);
}



/**********************************************************************func*
* character_BoostGetHelpText
*
*/
char *character_BoostGetHelpText(Entity *pEnt, const Boost *pBoost, const BasePower *pBase, int iLevel)
{
    ScriptVarsTable        tempvars = {0};
    char                *buf = NULL;
    int                    requiredBufferSize;
    char                levelstr[10];
    char                combinestr[10];

    ScriptVarsTablePushVarEx(&tempvars, "Hero", pEnt, 'E', 0);
    ScriptVarsTablePushVarEx(&tempvars, "Boost", pBase, 'P', 0);

    if (pBoost)
    {
        if (pBoost->ppowBase && pBoost->ppowBase->bBoostBoostable)
            sprintf_s(levelstr, 10, "%i", MINMAX( pBoost->iLevel, 0, 100) );
        else if (pEnt && pEnt->pchar && pBoost->ppowBase && pBoost->ppowBase->bBoostUsePlayerLevel)
            sprintf_s(levelstr, 10, "%i", MINMAX( character_CalcExperienceLevel(pEnt->pchar), 0, 100) );
        else
            sprintf_s(levelstr, 10, "%i", MINMAX( pBoost->iLevel + pBoost->iNumCombines, 0, 100) );
        sprintf_s(combinestr, 10, "%i", MINMAX( pBoost->iNumCombines, 0, 100) );
    }
    else 
    {
        if(pEnt && pEnt->pchar && pBase && pBase->bBoostUsePlayerLevel)
            sprintf_s(levelstr, 10, "%i", MINMAX( character_CalcExperienceLevel(pEnt->pchar), 0, 100) );
        else
            sprintf_s(levelstr, 10, "%i", MINMAX( iLevel, 0, 100 ) );
        sprintf_s(combinestr, 10, "0");
    }
    ScriptVarsTablePushVar(&tempvars, "Level", levelstr);
    ScriptVarsTablePushVar(&tempvars, "Combines", combinestr);

    requiredBufferSize = msPrintfVars(menuMessages, NULL, 0, pBase->pchDisplayHelp, &tempvars, 0, 0) + 1; 
    estrSetLength( &buf, requiredBufferSize );

    msPrintfVars(menuMessages, buf, requiredBufferSize, pBase->pchDisplayHelp, &tempvars, 0, 0); 

    ScriptVarsTableClear(&tempvars);

    return buf;
}

/**********************************************************************func*
* character_BoostGetHelpTextInEntLocale
*
*/
char *character_BoostGetHelpTextInEntLocale(Entity *pEnt, const Boost *pBoost, const BasePower *pBase, int iLevel)
{
    ScriptVarsTable        tempvars = {0};
    char                *buf = NULL;
    int                    requiredBufferSize;
    char                levelstr[10];

    MessageStore*        store = menuMessages;

#if defined(SERVER)
    if (pEnt)
        store = svrMenuMessages->localizedMessageStore[pEnt->playerLocale];
#endif

    ScriptVarsTablePushVarEx(&tempvars, "Hero", pEnt, 'E', 0);
    ScriptVarsTablePushVarEx(&tempvars, "Boost", pBase, 'P', 0);
    if (pBoost)
        sprintf_s(levelstr, 10, "%i", pBoost->iLevel + pBoost->iNumCombines);
    else 
        sprintf_s(levelstr, 10, "%i", iLevel);
    ScriptVarsTablePushVar(&tempvars, "Level", levelstr);

    requiredBufferSize = msPrintfVars(store, NULL, 0, pBase->pchDisplayHelp, &tempvars, 0, 0) + 1; 
    estrSetLength( &buf, requiredBufferSize );

    msPrintfVars(store, buf, requiredBufferSize, pBase->pchDisplayHelp, &tempvars, 0, 0); 

    ScriptVarsTableClear(&tempvars);

    return buf;
}

/**********************************************************************func*
 * character_IsConfused
 *
 */
bool character_IsConfused(Character *p)
{
    if (!p)
    {
        return false;
    }

    if (p->attrCur.fConfused>0)
    {
        return true;
    }

    // If their creator is confused, then they are.
    if (p->entParent && p->entParent->erCreator)
    {
        Entity *e= erGetEnt(p->entParent->erCreator);
        if (e && e->pchar)
        {
            return character_IsConfused(e->pchar);
        }
    }

    return false;
}

/**********************************************************************func*
 * character_IsPvPActive
 *
 */
bool character_IsPvPActive(Character *p)
{
    if(p->bPvPActive)
        return true;

    // If their creator is pvp active, then they are.
    if(p->entParent && p->entParent->erCreator)
    {
        Entity *e= erGetEnt(p->entParent->erCreator);
        if(e && e->pchar)
        {
            return character_IsPvPActive(e->pchar);
        }
    }

    return false;
}

/**********************************************************************func*
 * charlist_GetNext
 *
 */
CharacterRef *charlist_GetNext(CharacterIter *piter)
{
    CharacterRef *pRet = NULL;

    assert(piter!=NULL);
    assert(piter->plist!=NULL);

    if(piter->plist->pBase==NULL)
        return NULL;

    piter->iCur++;
    if(piter->iCur>=0 && piter->iCur < piter->plist->iCount)
    {
        pRet = &piter->plist->pBase[piter->iCur];
    }

    return pRet;
}

/**********************************************************************func*
 * charlist_GetFirst
 *
 */
CharacterRef *charlist_GetFirst(CharacterList *plist, CharacterIter *piter)
{
    assert(plist!=NULL);
    assert(piter!=NULL);

    piter->plist = plist;
    piter->iCur = -1;

    return charlist_GetNext(piter);
}

/**********************************************************************func*
 * charlist_Add
 *
 */
void charlist_Add(CharacterList *plist, Character *pchar, float fDist)
{
    CharacterRef *pref;

    assert(plist!=NULL);
    assert(pchar!=NULL);

    pref = dynArrayAdd(&plist->pBase, sizeof(CharacterRef), &plist->iCount, &plist->iMaxCount, 1);

    pref->fDist = fDist;
    pref->pchar = pchar;
}

/**********************************************************************func*
 * charlist_Destroy
 *
 */
void charlist_Destroy(CharacterList *plist)
{
    if(plist)
    {
        if(plist->pBase)
            free(plist->pBase);

        memset(plist, 0, sizeof(CharacterList));
    }
}

void charList_shuffle(CharacterList *pList)
{
    int i;
    //    http://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
    //    To shuffle an array a of n elements:
    //    for i from n-1 down to 1 do
    //    j <- random integer with 0 <= j <= i
    //        exchange a[j] and a[i]

    //    there is a bias here because our random number generator isn't really random
    //    it does the mod thing which also tends to favor smaller numbers...

    for (i = 0; i < pList->iCount; ++i)
    {
        int index = getCappedRandom(pList->iCount-i)+i;

        if (i != index)
        {
            swap(&pList->pBase[i], &pList->pBase[index], sizeof(CharacterRef));
        }
    }    
}

/**********************************************************************func*
 * charlist_Clear
 *
 */
void charlist_Clear(CharacterList *plist)
{
    plist->iCount = 0;
}


/**********************************************************************func*
* character_RespecTrayReset
*
*/
void character_RespecTrayReset(Entity * e)
{
    int i,j;
    int build;
    TrayObj * obj;
    TrayObj **macros = NULL;

    build = e->pchar->iCurBuild;
    // empty out trays, pulling macros out into a temporary list
    resetTrayIterator(e->pl->tray, build, kTrayCategory_PlayerControlled);
    while (getNextTrayObjFromIterator(&obj))
    {
        if (obj && obj->type != kTrayItemType_None && IS_TRAY_MACRO(obj->type))
        {
            TrayObj *newobj = trayobj_Create();
            buildMacroTrayObj(newobj, obj->command, obj->shortName, obj->iconName, obj->type==kTrayItemType_MacroHideName);
            eaPush(&macros, newobj);
        }

        destroyCurrentTrayObjViaIterator();
    }

    // push macros back into the trays
    resetTrayIterator(e->pl->tray, build, kTrayCategory_PlayerControlled);
    for (i = 0; i < eaSize(&macros); i++)
    {
        if (!getNextTrayObjFromIterator(&obj))
        {
            // If you trigger this, you ran out of tray slots to insert powers into.
            break;
        }

        if (!obj)
        {
            createCurrentTrayObjViaIterator(&obj);
        }

        buildMacroTrayObj(obj, macros[i]->command, macros[i]->shortName, macros[i]->iconName, macros[i]->type == kTrayItemType_MacroHideName);
    }
    eaDestroyEx(&macros, trayobj_Destroy);


    // fill the tray with new powers
    // keep using the same tray iterator from the last chunk
    for( i = 0; i < eaSize( &e->pchar->ppPowerSets ); i++ )
    {
        for( j = 0; j < eaSize( &e->pchar->ppPowerSets[i]->ppPowers ); j++)
        {
            Power *power = e->pchar->ppPowerSets[i]->ppPowers[j];

            if (power
                && power->ppowBase->eType != kPowerType_Auto
                && power->ppowBase->eType != kPowerType_GlobalBoost
                && power->bEnabled
                && stricmp(power->ppowBase->psetParent->pcatParent->pchName, "Incarnate")) // this line's a bit of a hack
            {
                if (!getNextTrayObjFromIterator(&obj))
                {
                    // If you trigger this, you ran out of tray slots to insert powers into.
                    break;
                }

                if (!obj)
                {
                    createCurrentTrayObjViaIterator(&obj);
                }

                buildPowerTrayObj(obj, i, j, 0);
                obj->pchPowerName    = e->pchar->ppPowerSets[i]->ppPowers[j]->ppowBase->pchName;
                obj->pchPowerSetName = e->pchar->ppPowerSets[i]->psetBase->pchName;
                obj->pchCategory     = e->pchar->ppPowerSets[i]->psetBase->pcatParent->pchName;
            }
        }
    }
}

/**********************************************************************func*
 * character_RespecSwap
 *
 */
void character_RespecSwap(Entity *e, Character *pcharOrig, Character *pcharNew, int influenceChange)
{
    int i,j,k;
    int count;

    // copy inspirations
     pcharNew->iNumInspirationCols = pcharOrig->iNumInspirationCols;
    pcharNew->iNumInspirationRows = pcharOrig->iNumInspirationRows;
    for( i = 0; i < pcharNew->iNumInspirationCols; i++ )
    {
        for( j = 0; j < pcharNew->iNumInspirationRows; j++ )
        {
            pcharNew->aInspirations[i][j] = pcharOrig->aInspirations[i][j];
        }
    }

    pcharNew->entParent                = e;
    pcharNew->iExperienceDebt        = pcharOrig->iExperienceDebt;
    pcharNew->iExperienceRest        = pcharOrig->iExperienceRest;
    pcharNew->iExperiencePoints        = pcharOrig->iExperiencePoints;
    pcharNew->iInfluencePoints        = pcharOrig->iInfluencePoints;
    pcharNew->iLevel                   = pcharOrig->iLevel;
    pcharNew->iWisdomLevel             = pcharOrig->iWisdomLevel;
    pcharNew->iAllyID                  = pcharOrig->iAllyID;
    pcharNew->iGangID                  = pcharOrig->iGangID;
    pcharNew->bPvPActive            = pcharOrig->bPvPActive;

    pcharNew->buddy                   = pcharOrig->buddy;
    pcharNew->erLastDamage            = pcharOrig->erLastDamage;

    pcharNew->ulTimeHeld              = pcharOrig->ulTimeHeld;
    pcharNew->ulTimeSleep             = pcharOrig->ulTimeSleep;
    pcharNew->ulTimeImmobilized        = pcharOrig->ulTimeImmobilized;
    pcharNew->ulTimeStunned            = pcharOrig->ulTimeStunned;

    pcharNew->bPvPActive            = pcharOrig->bPvPActive;

    // copying inventory information
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

    // Give them the money for cashing in boosts. We couldn't do this before
    // because we hadn't copied all the money etc into the new pchar.
    ent_AdjInfluence(e, influenceChange, NULL);

    // must set hitpoints to avoid death window (while waiting for update). All other attributes will be updated by server
    pcharNew->attrCur.fHitPoints = pcharOrig->attrCur.fHitPoints;


    // copy all powers except {Primary, Secondary, Pool, Epic, and Inherent} powers
    count = eaSize(&pcharOrig->ppPowerSets);
    for(i=0; i<count; i++)
    {
        PowerSet *pset = pcharOrig->ppPowerSets[i];
        if(pset)
        {
            const PowerCategory * pcat = pset->psetBase->pcatParent;

            if (!(pcat == pcharOrig->pclass->pcat[kCategory_Primary]
                    || pcat == pcharOrig->pclass->pcat[kCategory_Secondary]
                    || pcat == pcharOrig->pclass->pcat[kCategory_Pool]
                    || pcat == pcharOrig->pclass->pcat[kCategory_Epic]
                    || !stricmp(pset->psetBase->pcatParent->pchName, "Inherent"))) // don't want to swap inherent, since the assigned slots might have changed
            {
                Power **ppTemp = NULL;
                PowerSet * pset = character_OwnsPowerSet(pcharNew, pcharOrig->ppPowerSets[i]->psetBase);
                if(! pset)
                    pset = character_AddNewPowerSet(pcharNew, pcharOrig->ppPowerSets[i]->psetBase);

                pset->iLevelBought = pcharOrig->ppPowerSets[i]->iLevelBought;

                ppTemp = pset->ppPowers;
                pset->ppPowers = pcharOrig->ppPowerSets[i]->ppPowers;
                pcharOrig->ppPowerSets[i]->ppPowers = ppTemp;

                for(k=0;k<eaSize(&pset->ppPowers);k++)
                {
                    if (pset->ppPowers[k])
                    {
                        pset->ppPowers[k]->psetParent = pset;
                        power_SetUniqueID(pcharNew, pset->ppPowers[k], pset->ppPowers[k]->iUniqueID, NULL);
                    }
                }

                for(k=0;k<eaSize(&pcharOrig->ppPowerSets[i]->ppPowers);k++)
                {
                    if(pcharOrig->ppPowerSets[i]->ppPowers[k])
                        pcharOrig->ppPowerSets[i]->ppPowers[k]->psetParent = pset;
                }
            }
        }
    }

    // For all builds *EXCEPT* the current, swap the ppBuildPowerSets entries.  This causes the empty ones created inside respec_receiveInfo(),
    // currently in pCharNew to be placed in pcharOrig, where they will get cleaned up when we leave this block.
    // The last bit of work is done at the bottom, when we copy ppPowerSets into the correct slot in the ppBuildPowerSets array
    for (j = 0; j < MAX_BUILD_NUM; j++)
    {
        if (j != e->pchar->iCurBuild)
        {
            PowerSet **psets = pcharOrig->ppBuildPowerSets[j];
            pcharOrig->ppBuildPowerSets[j] = pcharNew->ppBuildPowerSets[j];
            pcharNew->ppBuildPowerSets[j] = psets;
            count = eaSize(&pcharNew->ppBuildPowerSets[j]);
            // We've swapped this pair of builds, but their respective shared powerset pointers now point to the wrong
            // places.  So we carefully splat the ones from the active builds to make everything consistent.
            for (i = 0; i < g_numSharedPowersets; i++)
            {
                pcharOrig->ppBuildPowerSets[j][i] = pcharOrig->ppPowerSets[i];
                pcharNew->ppBuildPowerSets[j][i] = pcharNew->ppPowerSets[i];
                if (pcharNew->ppBuildPowerSets[j][i])
                {
                    pcharNew->ppBuildPowerSets[j][i]->pcharParent = pcharNew;
                }
            }

            for (i = g_numSharedPowersets; i < count; i++)
            {
                PowerSet *pset = pcharNew->ppBuildPowerSets[j][i];
                if (pset)
                {
                    pset->pcharParent = pcharNew;
                    for (k = eaSize(&pset->ppPowers) - 1; k >= 0; k--)
                    {
                        if (pset->ppPowers[k])
                            power_SetUniqueID(pcharNew, pset->ppPowers[k], pset->ppPowers[k]->iUniqueID, NULL);
                    }
                }
            }
        }
    }

#if SERVER
    count = eaSize(&pcharOrig->salvageInv);
    for(i=0; i<count; i++)
    {
        if (!pcharOrig->salvageInv[i] || !pcharOrig->salvageInv[i]->salvage)
            continue;
        character_AdjustInventoryEx(pcharNew, kInventoryType_Salvage, pcharOrig->salvageInv[i]->salvage->salId, pcharOrig->salvageInv[i]->amount, NULL, true, false);
    }
    character_SetSalvageInvCurrentCount(pcharNew);

    count = eaSize(&pcharOrig->storedSalvageInv);
    for(i=0; i<count; i++)
    {
        if (!pcharOrig->storedSalvageInv[i] || !pcharOrig->storedSalvageInv[i]->salvage)
            continue;
        character_AdjustInventoryEx(pcharNew, kInventoryType_StoredSalvage, pcharOrig->storedSalvageInv[i]->salvage->salId, pcharOrig->storedSalvageInv[i]->amount, NULL, true, false);
    }
    character_SetStoredSalvageInvCurrentCount(pcharNew);

    count = eaSize(&pcharOrig->detailInv);
    for(i=0; i<count; i++)
    {
        if (!pcharOrig->detailInv[i] || !pcharOrig->detailInv[i]->item)
            continue;
        character_AdjustInventoryEx(pcharNew, kInventoryType_BaseDetail, pcharOrig->detailInv[i]->item->id, pcharOrig->detailInv[i]->amount, NULL, true, false);
    }

    count = eaSize(&pcharOrig->recipeInv);
    for(i=0; i<count; i++) 
    {
        if (!pcharOrig->recipeInv[i] || !pcharOrig->recipeInv[i]->recipe)
            continue;
        character_AdjustInventoryEx(pcharNew, kInventoryType_Recipe, pcharOrig->recipeInv[i]->recipe->id, pcharOrig->recipeInv[i]->amount, NULL, true, false);
    }
    character_SetRecipeInvCurrentCount(pcharNew);

    count = eaSize(&pcharOrig->conceptInv);
    for(i=0; i<count; i++)
    {
        if (!pcharOrig->conceptInv[i] || !pcharOrig->conceptInv[i]->concept)
            continue;
        character_AdjustConcept( pcharNew, pcharOrig->conceptInv[i]->concept->def->id, pcharOrig->conceptInv[i]->concept->afVars, pcharOrig->conceptInv[i]->amount );
    }

    pcharNew->stSentBuffs = stashTableClone( pcharOrig->stSentBuffs, NULL, NULL );

#endif

    e->pchar = pcharNew;

#if SERVER
    character_CalcCombatLevel(e->pchar);
    character_ActivateAllAutoPowers(e->pchar);
#endif

    character_RespecTrayReset(e);
}

void character_ResetAllPowerTimers(Character *pchar)
{
#if SERVER
    PowerSet** powerSets;
    Power* power;
    int powerSetCount;
    int powerCount;
    int i, j;

    if (!pchar)
    {
        return;
    }

    powerSets = pchar->ppPowerSets;
    powerSetCount = eaSize(&powerSets);

    for(i = 0; i < powerSetCount; i++)
    {
        powerCount = eaSize(&powerSets[i]->ppPowers);

        for(j = 0; j < powerCount; j++)
        {
            if (power = powerSets[i]->ppPowers[j])
            {
                power_ChangeRechargeTimer(power, 0.0f);
            }
        }
    }
#endif
}

// There are three different versions of this guy.  This SelectBuild is the full server version, it selects a build and "finalizes" the change by disabling everything and
// reactivating auto powers.  Next is the Lightweight version, used when receving multiple builds.  This must *ALWAYS* return to the build that was active the first time it was called,
// there are checks in the heavyweight version to detect this.
// Finally, the client version, which is very lightweight indeed
#if SERVER
void character_SelectBuild(Entity *e, int buildNum, int hardSwitch)
{
    int origBuild;

    if (e != NULL && ENTTYPE(e) == ENTTYPE_PLAYER && e->pchar != NULL)
    {
        if (buildNum < 0 || buildNum >= MAX_BUILD_NUM)
        {
            dbLog("Character:SelectBuild:Error", e, "Invalid requested build number: %d", buildNum);
            return;
        }
        if (e->pchar->iCurBuild != e->pchar->iActiveBuild)
        {
            LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_ALERT, 0, "Someone messed up using character_SelectBuildLightweight - active: %d, current %d.  Please tell a programmer\n", e->pchar->iActiveBuild, e->pchar->iCurBuild);
            // And now undo the damage
            character_SelectBuildLightweight(e, e->pchar->iActiveBuild);
        }
        if (e->pchar->iActiveBuild != buildNum)
        {
            if (hardSwitch)
            {
                // MUST RUN character_TickPhaseTwo() here to allow onDeactivate mods to work!
                character_ShutOffAllTogglePowers(e->pchar);
                character_ShutOffAllAutoPowers(e->pchar);
                character_DisableAllPowers(e->pchar);
                character_TickPhaseTwo(e->pchar, 0);
                character_TellClientToSelectBuild(e, buildNum);
            }

            origBuild = e->pchar->iCurBuild;
            assert(e->pchar->iCurBuild >= 0 && e->pchar->iCurBuild < MAX_BUILD_NUM);

            // DGNOTE 8/27/08 - If this assert ever trips, look for another DGNOTE with the same date stamp up in character_Create()
            // Cliff notes version - the eaSetCapacity for e->pchar->ppBuildPowerSets[x] wasn't big enough.
            if (e->pchar->ppBuildPowerSets[e->pchar->iCurBuild] != e->pchar->ppPowerSets)
            {
                // This should never go off.  If it does, it means someone expanded the array someplace other than character_AddNewPowerSet
                LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, "%s, %d, e->pchar->ppBuildPowerSets[e->pchar->iCurBuild] != e->pchar->ppPowerSets\n", __FILE__, __LINE__);
            }
            e->pchar->ppBuildPowerSets[e->pchar->iCurBuild] = e->pchar->ppPowerSets;
            e->pchar->iBuildLevels[e->pchar->iCurBuild] = e->pchar->iLevel;
            if (e->pchar->ppBuildPowerSets[0] && e->pchar->entParent)
            {
                // Cleaning up the character, onDeactivate was already handled earlier...
                character_CancelPowers(e->pchar);
                // character_CancelPowers(...) calls character_TickPhaseTwo(...) which can turn auto powers back on.
                // We don't want this, so we turn them back off again.
                character_ShutOffAllAutoPowers(e->pchar);
            }
            e->pchar->iCurBuild = buildNum;
            e->pchar->iActiveBuild = buildNum;
            e->pchar->iLevel = e->pchar->iBuildLevels[buildNum];
            e->pchar->ppPowerSets = e->pchar->ppBuildPowerSets[buildNum];
            if (hardSwitch)
            {
                e->pchar->ppowDefault = NULL;
                if (eaSize(&e->pchar->ppPowerSets[g_numSharedPowersets]->ppPowers) == 0)
                {
                    character_GrantAutoIssuePowers(e->pchar);
                }
            }
            if (e->pchar->ppBuildPowerSets[0] && e->pchar->entParent)
            {
                character_UncancelPowers(e->pchar);
            }

            if (hardSwitch)
            {
                character_ResetAllPowerTimers(e->pchar);
                // 15 minutes between changes
                e->pchar->iBuildChangeTime = timerSecondsSince2000() + 15 * 60;
            }

            dbLog("Character:SelectBuild:Success", e, "Switched from build %d to build %d", origBuild, buildNum);
        }
    }
}

void character_SelectBuildLightweight(Entity *e, int buildNum)
{
    assert(e);
    if (ENTTYPE(e) == ENTTYPE_PLAYER)
    {
        if (e->pchar->iCurBuild != buildNum)
        {
            assert(e->pchar);
            assert(e->pchar->iCurBuild >= 0 && e->pchar->iCurBuild < MAX_BUILD_NUM);
            assert(buildNum >= 0 && buildNum < MAX_BUILD_NUM);

            // DGNOTE 8/27/08 - If this assert ever trips, look for another DGNOTE with the same date stamp up in character_Create()
            // Cliff notes version - the eaSetCapacity for e->pchar->ppBuildPowerSets[x] wasn't big enough.
            if (e->pchar->ppBuildPowerSets[e->pchar->iCurBuild] != e->pchar->ppPowerSets)
            {
                // This should never go off.  If it does, it means someone expanded the array someplace other than character_AddNewPowerSet
                LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_ALERT, 0, "%s, %d, e->pchar->ppBuildPowerSets[e->pchar->iCurBuild] != e->pchar->ppPowerSets\n", __FILE__, __LINE__);
            }
            e->pchar->ppBuildPowerSets[e->pchar->iCurBuild] = e->pchar->ppPowerSets;
            e->pchar->iCurBuild = buildNum;
            e->pchar->iLevel = e->pchar->iBuildLevels[buildNum];
            e->pchar->ppPowerSets = e->pchar->ppBuildPowerSets[buildNum];
        }
    }
}
#endif

#if CLIENT
void character_SelectBuildLightweight(Entity *e, int buildNum)
{
    assert(e);
    if (ENTTYPE(e) == ENTTYPE_PLAYER)
    {
        assert(e->pchar);
        assert(e->pchar->iCurBuild >= 0 && e->pchar->iCurBuild < MAX_BUILD_NUM);
        assert(buildNum >= 0 && buildNum < MAX_BUILD_NUM);

        // DGNOTE 8/27/08 - If this assert ever trips, look for another DGNOTE with the same date stamp up in character_Create()
        // Cliff notes version - the eaSetCapacity for e->pchar->ppBuildPowerSets[x] wasn't big enough.
        e->pchar->ppBuildPowerSets[e->pchar->iCurBuild] = e->pchar->ppPowerSets;
        e->pchar->iCurBuild = buildNum;
        e->pchar->ppPowerSets = e->pchar->ppBuildPowerSets[buildNum];
    }
}

void character_SelectBuildClient(Entity *e, int buildNum)
{
    character_SelectBuildLightweight(e, buildNum);
    e->pchar->ppowDefault = NULL;
#ifndef TEST_CLIENT
    serverTray_Rebuild();
#endif
}

#endif

#if SERVER
void character_TellClientToSelectBuild(Entity *e, int buildNum)
{
    if (ENTTYPE(e) == ENTTYPE_PLAYER)
    {
        START_PACKET(pak, e, SERVER_SELECT_BUILD);
        pktSendBits(pak, 4, buildNum);
        pktSendBits(pak, 6, e->pchar->iBuildLevels[buildNum]);
        END_PACKET
    }
}

void character_BuildRename(Entity *e, Packet *pak)
{
    char *name;

    if (e->pl && e->pchar && e->pchar->iActiveBuild >= 0 && e->pchar->iActiveBuild < MAX_BUILD_NUM)
    {
        name = pktGetString(pak);
        strncpy(e->pl->buildNames[e->pchar->iActiveBuild], name, sizeof(e->pl->buildNames[0]));
        e->pl->buildNames[e->pchar->iActiveBuild][sizeof(e->pl->buildNames[0]) - 1] = 0;
    }
}
#endif

int character_GetCurrentBuild(Entity *e)
{ 
    return e && ENTTYPE(e) == ENTTYPE_PLAYER && e->pchar ? e->pchar->iCurBuild : 0;
}

int character_GetActiveBuild(Entity *e)
{ 
    return e && ENTTYPE(e) == ENTTYPE_PLAYER && e->pchar ? e->pchar->iActiveBuild : 0;
}

int character_MaxBuildsAvailable(Entity *e)
{
    int maxBuilds = 1;

    if (e->pchar && character_PowersGetMaximumLevel(e->pchar) >= MULTIBUILD_INITIAL_LEVEL)
    {
        maxBuilds++;
    }

    if (IsEntityIncarnate(e))
    {
        maxBuilds++;
    }

    if (e->access_level >= MULTIBUILD_DEVMODE_LEVEL)
    {
        maxBuilds = MAX_BUILD_NUM;
    }

    if (maxBuilds > MAX_BUILD_NUM)
    {
        maxBuilds = MAX_BUILD_NUM;
    }
    return maxBuilds;
}

//------------------------------------------------------------
//  helper for slotting a reward in a boost.
//----------------------------------------------------------
bool s_boost_TrySlot(Boost *b, RewardItemType type, char const *name, int matchLevel)
{
    bool res = false;

    // if this boost has a recipe as a parent it is invented
    if( b && basepower_IsRecipe( b->ppowBase ) )
    {
        int i;
        for( i = 0; i < ARRAY_SIZE(b->aPowerupSlots) && !res; ++i )
        {
            if( !b->aPowerupSlots[i].filled && rewardslot_Matches( &b->aPowerupSlots[i].reward, type, name, matchLevel ) )
            {
                // we have a match.
                res = b->aPowerupSlots[i].filled = true;

                // @todo -AB: something when all slots are filled. :2005 Mar 28 01:54 PM
            }
        }
    }

    // --------------------
    // finally

    return res;
}


bool isPetCommadable( Entity * e)
{
    if( e )
        return e->commandablePet;
    return 0;
}

bool isPetDismissable( Entity * e)
{
    if( e )
        return e->petDismissable;
    return 0;
}


bool isPetDisplayed( Entity * e )
{
    if( !e || !e->pchar )
        return 0;

    // villain def overrides any code method of displaying pet
    if( e->petVisibility >= 0)
        return e->petVisibility;
    
    if( isPetCommadable(e) )
        return 1;

    if(    ENTTYPE(e) == ENTTYPE_CRITTER &&  // not a critter
        e->erCreator && e->erOwner && // not a pet 
        e->pchar->ppowCreatedMe )  // MUST be created by a power
    {
        return 1;
    }

    return 0;
}

int pcccritter_buyAllPowersInPowerset(Entity *e, const char *powerSetName, int pccDifficulty, int minPowersFromThisSet, int selectedPowers, 
                                      int  *bAttackPower, int *difficultyRequirementsMet, int autopower_only)
{
    int total_count = 0;
    #if SERVER
    int i;
    const BasePowerSet*   basePowerSet;
    PowerSet*       powerSet;
    int brawl = 0;
    int bExtremeMet=1;
    int bHardMet=1;
    int bStandardMet=1;
    
    
    if (stricmp(powerSetName, textStd("NoneString")) == 0)
        return 0;

    if (stricmp(powerSetName, textStd("Brawl")) == 0)
    {
        basePowerSet = powerdict_GetBasePowerSetByFullName(&g_PowerDictionary, "Generic.Brawl");
        pccDifficulty = 2;
        brawl = 1;
    }
    else
    {
        basePowerSet = powerdict_GetBasePowerSetByFullName(&g_PowerDictionary, powerSetName);
    }
    if(basePowerSet)
    {
        powerSet = character_BuyPowerSet(e->pchar, basePowerSet);
        if (powerSet)
        {
            int incomplete = 0;
            int powerCount = 0;

            // Buy all powers in the said power set.
            for(i = eaSize(&basePowerSet->ppPowers) - 1; i >= 0; i--)
            {
                if (selectedPowers || pccDifficulty==PCC_DIFF_CUSTOM )
                {
                    if ( selectedPowers & (1 << i) )
                    {
                        bool bAdded;
                        Power *ppow;

                        if( autopower_only && basePowerSet->ppPowers[i]->eType != kPowerType_Auto )
                            continue;
                        ppow = character_BuyPowerEx(e->pchar, powerSet, basePowerSet->ppPowers[i], &bAdded, 1, 0, NULL);
                        if(ppow==NULL)
                        {
                            verbose_printf("Dev warning: (function: pcccritter_buyAllPowersInPowerset) Unable to buy power %s\n", basePowerSet->ppPowers[i]->pchName);
                        }
                        else
                        {
                            if( bAttackPower && !basePowerSet->ppPowers[i]->ppchAIGroups )
                                *bAttackPower = 1;
                            powerCount++;
                            total_count++;
                            ppow->bDontSetStance |= brawl ? 0 : 1;
                        }
                    }
                    else 
                    {
                        int rankCon;
                        const BasePower *ppowbase = basePowerSet->ppPowers[i];

                        // checking for max level limits
                        if ((basepower_GetAIMaxLevel(ppowbase) < e->pchar->iLevel) || (basepower_GetAIMinLevel(ppowbase) > e->pchar->iLevel))
                            continue;

                        // checking for rank con limits
                        rankCon = villainRankConningAdjustment(e->villainRank);
                        if ( ((basepower_GetAIMaxRankCon(ppowbase) < rankCon) || (basepower_GetAIMinRankCon(ppowbase) > rankCon)))
                        {
                            continue;
                        }

                        if ( ( PCC_DIFF_EXTREME <= basepower_GetMaxDifficulty(ppowbase) ) && ( PCC_DIFF_EXTREME >= basepower_GetMinDifficulty(ppowbase) ) )
                            bExtremeMet = 0;
                        if ( ( PCC_DIFF_HARD <= basepower_GetMaxDifficulty(ppowbase) ) && ( PCC_DIFF_HARD >= basepower_GetMinDifficulty(ppowbase) ) )
                            bHardMet = 0;
                        if ( ( PCC_DIFF_STANDARD <= basepower_GetMaxDifficulty(ppowbase) ) && ( PCC_DIFF_STANDARD >= basepower_GetMinDifficulty(ppowbase) ) )
                            bStandardMet = 0; 
                    }
                }
                else
                {
                    if ( ( pccDifficulty <= basepower_GetMaxDifficulty(basePowerSet->ppPowers[i]) ) &&
                        ( pccDifficulty >= basepower_GetMinDifficulty(basePowerSet->ppPowers[i]) ) )
                    {
                        Power *ppow;

                        if( autopower_only && basePowerSet->ppPowers[i]->eType != kPowerType_Auto )
                            continue;
                        ppow = character_BuyPower(e->pchar, powerSet, basePowerSet->ppPowers[i], 0);
                        if(ppow==NULL)
                        {
                            verbose_printf("Dev warning: (function: pcccritter_buyAllPowersInPowerset) Unable to buy power %s\n", basePowerSet->ppPowers[i]->pchName);
                        }
                        else
                        {
                            if( bAttackPower && !basePowerSet->ppPowers[i]->ppchAIGroups )
                                *bAttackPower = 1;
                            powerCount++;
                            total_count++;
                            ppow->bDontSetStance |= brawl ? 0 : 1;
                        }
                    }
                }
            }
        }
        else
        {
            verbose_printf("Dev warning: (function: pcccritter_buyAllPowersInPowerset) Couldn't buy powerset %s\n", textStd(powerSetName));
            return -1;
        }
    }
    else
    {
        verbose_printf("Dev warning: (function: pcccritter_buyAllPowersInPowerset) Couldn't find powerset %s\n", textStd(powerSetName));
        return -1;
    }

    if( difficultyRequirementsMet )
    {
        if( bExtremeMet )
            *difficultyRequirementsMet = PCC_DIFF_EXTREME;
        else if( bHardMet )
            *difficultyRequirementsMet = PCC_DIFF_HARD;
        else if( bStandardMet )
            *difficultyRequirementsMet = PCC_DIFF_STANDARD;
    }

    #endif
    return total_count;
}

static int __cdecl compareF32(const F32* f1, const F32* f2)
{
    return *f2 -*f1;
}

int isRangedCreditPower(const BasePower * pow)
{
    int i;

    if( pow->fRange < 39.f || pow->fPointVal < 65.f || pow->eTargetType != kTargetType_Foe )
        return 0;

    for( i = 0; i < eaSize(&pow->ppTemplates); i++ )
    {
        if( IS_MODIFIER(pow->ppTemplates[i]->offAspect) && IS_HITPOINTS(pow->ppTemplates[i]->offAttrib) )
            return 1;
    }
    
    return 0;
}

F32 pcccritter_tallyPowerValueFromPowers(const BasePower ***ppPowers, int rank, int level)
{
    int i;
    int bFoundRanged = 0;
    int value_cnt = 0;
    int mult_cnt = 0;
    F32 *powerValues = 0;
    F32 powerMultipliers[32][2] = {0};
    F32 *powerMultValues = 0;
    F32 total_points = 0;
    F32 conning_rank[] = { 250, 340, 430, 450, 450, 0 };
    F32 conning_total;

    for( i = eaSize(ppPowers)-1; i>=0 && value_cnt<32 && mult_cnt<32; i-- )
    {
        const BasePower * pow = ((*ppPowers)[i]);

        if( isRangedCreditPower(pow) )
            bFoundRanged = 1;

        if( pow->fPointVal )
        {
            if(!pow->fPointMultiplier)
            {
                int size = eafSize(&powerValues);

                if( !size )
                    eafPush(&powerValues,pow->fPointVal);
                else
                {
                    int idx = 0;
                    while(idx < size && powerValues[idx] > pow->fPointVal )
                        idx++; 
                    if( idx == size )
                        eafPush(&powerValues, pow->fPointVal);
                    else
                        eafInsert(&powerValues,pow->fPointVal,idx);
                }
            }
            else
            {
                powerMultipliers[mult_cnt][0] = pow->fPointVal; 
                powerMultipliers[mult_cnt][1] = pow->fPointMultiplier;
                mult_cnt++;
            }
        }
    }

    if( eafSize(&powerValues) > 5 )
        eafSetSize(&powerValues, 5);

    for( i = 0; i < mult_cnt; i++ )
    {
        int j = 0;
        while( powerMultipliers[i][0] > 0 && j < eafSize(&powerValues) )
        {
            if( powerMultipliers[i][0] < 1 )
                eafPush( &powerMultValues, powerMultipliers[i][0] * powerMultipliers[i][1] * powerValues[j] );        //    insert into a different list so we don't affect/skew the size and original values
            else
                eafPush( &powerMultValues, powerMultipliers[i][1] * powerValues[j] );                                //    insert into a different list so we don't affect/skew the size and original values
            powerMultipliers[i][0]--;
            j++;
        }
    }

    for(i = 0; i < eafSize(&powerValues); i++)        //    sum up on the original points
        total_points += powerValues[i];
    for(i = 0; i < eafSize(&powerMultValues); i++)        //    add onto that the multiplier points
        total_points += powerMultValues[i];
    
    eafDestroy(&powerValues);
    eafDestroy(&powerMultValues);

    if( level < 20 )
        conning_total = (conning_rank[rank] + ((float)level/20.f)*conning_rank[rank])/2.f;
    else
        conning_total = conning_rank[rank] + (((float)level-20.f)/30.f) * conning_rank[rank] * 0.2f;

    if( conning_total )
    {
        if( bFoundRanged )
            return MIN( 1.0f, total_points/conning_total);
        else
            return MIN( .40f, total_points/conning_total);
    }

    return 0;
}
F32 pcccritter_tallyPowerValue(Entity *e, int rank, int level)
{
    const BasePower **basePowerList = NULL;
    int i;
    F32 retVal;
    if(!e || !e->pchar )
        return 0;

    for (i = 0; i < eaSize(&e->pchar->ppPowerSets); ++i)
    {
        int j;
        for (j = 0; j < eaSize(&e->pchar->ppPowerSets[i]->ppPowers); ++j)
        {
            eaPushConst(&basePowerList, e->pchar->ppPowerSets[i]->ppPowers[j]->ppowBase);
        }
    }
    retVal = pcccritter_tallyPowerValueFromPowers(&basePowerList, rank, level);
    eaDestroyConst(&basePowerList);
    return retVal;
}

#ifdef SERVER
void character_RevokeAutoPowers(Character *p, int onlyAEAutoPowers)
{
    int iset;

    assert(p!=NULL);

    for(iset=eaSize(&p->ppPowerSets)-1; iset>=0; iset--)
    {
        int ipow;
        for(ipow=eaSize(&p->ppPowerSets[iset]->ppPowers)-1; ipow>=0; ipow--)
        {
            Power *ppow = p->ppPowerSets[iset]->ppPowers[ipow];
            if(ppow    && ppow->ppowBase)
            {
                const BasePower *bpow = ppow->ppowBase;
                if (bpow->eType == kPowerType_Auto)
                {
                    int i;
                    int removePower = !onlyAEAutoPowers;
                    if (!removePower)
                    {
                        if (bpow->eTargetType != kTargetType_Caster)
                        {
                            removePower = 1;
                        }
                        else
                        {
                            for (i = 0; i < eaiSize(&(int*)(bpow->pAffected)); ++i)
                            {
                                if (bpow->pAffected[i] != kTargetType_Caster)
                                {
                                    removePower = 1;
                                    break;
                                }
                            }
                            for (i = 0; !removePower && (i < eaSize(&bpow->ppTemplates)); ++i)
                            {
                                //    if it does any of the following, they can't keep it =P
                                switch (bpow->ppTemplates[i]->offAttrib)
                                {
                                case kSpecialAttrib_EntCreate:        //    intentional fallthrough
                                case kSpecialAttrib_GrantPower:
                                case kSpecialAttrib_Reward:
                                case kSpecialAttrib_RevokePower:
                                    removePower = 1;
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                    }
                    if (removePower)
                    {
                        character_RemovePower(p, ppow);
                    }
                }
            }
        }
    }
}

void character_RestoreAutoPowers(Entity *e)
{
    if( e->custom_critter )
    {
        if( e->pcc ) // if this is gone then we have no idea what original def was
        {
            pcccritter_buyAllPowersInPowerset(e, e->pcc->primaryPower, e->pcc->difficulty[0], 0, e->pcc->difficulty[0] == PCC_DIFF_CUSTOM ? e->pcc->selectedPowers[0] : 0, 0, 0, 1);
            pcccritter_buyAllPowersInPowerset(e, e->pcc->secondaryPower, e->pcc->difficulty[1], 0, e->pcc->difficulty[1] == PCC_DIFF_CUSTOM ? e->pcc->selectedPowers[1] : 0, 0, 0, 1);

            if (stricmp(e->pcc->travelPower, "none") != 0)    
                pcccritter_buyAllPowersInPowerset(e, e->pcc->travelPower, PCC_DIFF_EXTREME, 0, 0, 0, 0, 1);

            if (stricmp(e->villainDef->name, "CustomCritter_AV") == 0)
            {
                pcccritter_buyAllPowersInPowerset(e, "Mission_Maker_Special.AV_Resistances", PCC_DIFF_EXTREME, 0, 0, 0, 0, 1);
            }
        }
    }
    else if( e->villainDef )
    {
        npcInitPowers(e, e->villainDef->powers, 1);
    }
}
#endif 

void ValidateCharacterPowerSets(Character *p)
{
    if (VALIDATE_CHARACTER_POWER_SETS == 1)
    {
        int setIndex;

        if (!p)
            return;

        for (setIndex = eaSize(&p->ppPowerSets) - 1; setIndex >= 0; setIndex--)
        {
            assert(p->ppPowerSets[setIndex]->pcharParent == p);
        }
    }
    else
    {
        int entityIndex, setIndex, buildIndex;
        Character *iteratedCharacter;

        for (entityIndex = 0; entityIndex < entities_max; entityIndex++)
        {
            if (!entities[entityIndex])
                continue;

            iteratedCharacter = entities[entityIndex]->pchar;

            if (!iteratedCharacter || !iteratedCharacter->ppPowerSets)
                continue;

            for (setIndex = eaSize(&iteratedCharacter->ppPowerSets) - 1; setIndex >= 0; setIndex--)
            {
                assert(iteratedCharacter->ppPowerSets[setIndex]->pcharParent == iteratedCharacter);
            }

            for (buildIndex = MAX_BUILD_NUM - 1; buildIndex >= 0; buildIndex--)
            {
                for (setIndex = eaSize(&iteratedCharacter->ppBuildPowerSets[buildIndex]) - 1; setIndex >= 0; setIndex--)
                {
                    assert(iteratedCharacter->ppBuildPowerSets[buildIndex][setIndex]->pcharParent == iteratedCharacter);
                }
            }
        }
    }
}

/* End of File */
