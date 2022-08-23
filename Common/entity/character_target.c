/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>

#include <utilitieslib/components/earray.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "gridcoll/gridcoll.h"
#include "entity/powers.h"
#include "entity/character_base.h"

#if SERVER
#include "entity/character_combat_eval.h"
#include "player/pmotion.h" // for entMode
#include "cmdparse/cmdserver.h"
#include "dbcomm/logcomm.h"
#else
#include "graphics/camera.h"
#endif

#include "entity/teamup.h"
#include "entity/character_target.h"
#include "group/groupscene.h"
#include <utilitieslib/utils/mathutil.h>

#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/timing.h>
#include "storyarc/pnpcCommon.h"

// For each of the the target types, specify if the target is allowed to be
// dead.
bool g_abTargetAllowedDead[] =
{
    false, // kTargetType_None,

    true,  // kTargetType_Caster,
    false, // kTargetType_Player,
    false, // kTargetType_PlayerHero,
    false, // kTargetType_PlayerVillain,
    true,  // kTargetType_DeadPlayer,
    true,  // kTargetType_DeadPlayerFriend,
    true,  // kTargetType_DeadPlayerFoe,

    false, // kTargetType_Teammate,
    true,  // kTargetType_DeadTeammate,
    true,  // kTargetType_DeadOrAliveTeammate,

    false, // kTargetType_Villain,
    true,  // kTargetType_DeadVillain,
    false, // kTargetType_NPC,

    true,  // kTargetType_DeadOrAliveFriend,
    true,  // kTargetType_DeadFriend,
    false, // kTargetType_Friend,

    true,  // kTargetType_DeadOrAliveFoe,
    true,  // kTargetType_DeadFoe,
    false, // kTargetType_Foe,

    true,  // kTargetType_Location,
    true,  // kTargetType_Any,
    true,  // kTargetType_Teleport,

    true,  // kTargetType_DeadOrAliveMyPet
    true,  // kTargetType_DeadMyPet
    false, // kTargetType_MyPet

    false, // kTargetType_MyOwner
    false, // kTargetType_MyCreator

    false, // kTargetType_MyCreation
    true,  // kTargetType_DeadMyCreation
    true,  // kTargetType_DeadOrAliveMyCreation

    false, // kTargetType_Leaguemate
    true,  // kTargetType_DeadLeaguemate
    true,  // kTargetType_DeadOrAliveLeaguemate

    true,  // kTargetType_Position
};

// For each of the the target types, specify if the target is allowed to be
// alive.
bool g_abTargetAllowedAlive[] =
{
    false, // kTargetType_None,

    true,  // kTargetType_Caster,
    true,  // kTargetType_Player,
    true,  // kTargetType_PlayerHero,
    true,  // kTargetType_PlayerVillain,
    false, // kTargetType_DeadPlayer,
    false, // kTargetType_DeadPlayerFriend,
    false, // kTargetType_DeadPlayerFoe,

    true,  // kTargetType_Teammate,
    false, // kTargetType_DeadTeammate,
    true,  // kTargetType_DeadOrAliveTeammate,

    true,  // kTargetType_Villain,
    false, // kTargetType_DeadVillain,
    true,  // kTargetType_NPC,

    true,  // kTargetType_DeadOrAliveFriend,
    false, // kTargetType_DeadFriend,
    true,  // kTargetType_Friend,

    true,  // kTargetType_DeadOrAliveFoe,
    false, // kTargetType_DeadFoe,
    true,  // kTargetType_Foe,

    true,  // kTargetType_Location,
    true,  // kTargetType_Any,
    true,  // kTargetType_Teleport,

    true,  // kTargetType_DeadOrAliveMyPet
    false, // kTargetType_DeadMyPet
    true,  // kTargetType_MyPet

    true,  // kTargetType_MyOwner
    true,  // kTargetType_MyCreator

    true,  // kTargetType_MyCreation
    false, // kTargetType_DeadMyCreation
    true,  // kTargetType_DeadOrAliveMyCreation

    true,  // kTargetType_Leaguemate
    false, // kTargetType_DeadLeaguemate
    true,  // kTargetType_DeadOrAliveLeaguemate

    true,  // kTargetType_Position
};

static int (*GangTestFunction)(int gang1, int gang2) = NULL;
void setGangTestFunction(int (*_GangTestFunction)(int, int))
{
    GangTestFunction = _GangTestFunction;
}

/**********************************************************************func*
 * character_TargetIsFoe
 *
 */
INLINEDBG bool character_TargetIsFoe(Character *pSrc, Character *pTarget) 
{
    Character *pTopSrc = pSrc;
    Character *pTopTarget = pTarget;
    
    if(pTopSrc->entParent && pTopSrc->entParent->erOwner)
    {
        Entity *e= erGetEnt(pTopSrc->entParent->erOwner);
        if(e && e->pchar && pSrc->iAllyID == e->pchar->iAllyID && pSrc->iGangID == e->pchar->iGangID)
        {
            pTopSrc = e->pchar;
        }
    }

    if(pTopTarget->entParent && pTopTarget->entParent->erOwner)
    {
        Entity *e= erGetEnt(pTopTarget->entParent->erOwner);
        if(e && e->pchar && pTarget->iAllyID == e->pchar->iAllyID && pTarget->iGangID == e->pchar->iGangID)
        {
            pTopTarget = e->pchar;
        }
    }

    if( pTopTarget == pTopSrc ) // I'm the owner of this pet
        pTopTarget = pTarget;   // allow pets and owners to hate each other

    if (pTopSrc->iGangID && pTopTarget->iGangID) 
    {
        if (GangTestFunction)
        {
            return (*GangTestFunction)(pTopSrc->iGangID, pTopTarget->iGangID);
        }
        return (pTopSrc->iGangID != pTopTarget->iGangID);
    }

    return (pTopSrc->iAllyID != pTopTarget->iAllyID);
}

/**********************************************************************func*
 * character_TargetIsFriend
 *
 */
bool character_TargetIsFriend(Character *pSrc, Character *pTarget) 
{
    Character *pTopSrc = pSrc;
    Character *pTopTarget = pTarget;
    
    if(pTopSrc->entParent && pTopSrc->entParent->erOwner)
    {
        Entity *e= erGetEnt(pTopSrc->entParent->erOwner);
        if(e && e->pchar && pSrc->iAllyID == e->pchar->iAllyID && pSrc->iGangID == e->pchar->iGangID)
        {
            pTopSrc = e->pchar;
        }
    }

    if(pTopTarget->entParent && pTopTarget->entParent->erOwner)
    {
        Entity *e= erGetEnt(pTopTarget->entParent->erOwner);
        if(e && e->pchar && pTarget->iAllyID == e->pchar->iAllyID && pTarget->iGangID == e->pchar->iGangID)
        {
            pTopTarget = e->pchar;
        }
    }

    if (pTopSrc->iGangID && pTopTarget->iGangID) 
    {
        if (GangTestFunction)
        {
            return !(*GangTestFunction)(pTopSrc->iGangID, pTopTarget->iGangID);
        }
        return (pTopSrc->iGangID == pTopTarget->iGangID);
    }

    return (pTopSrc->iAllyID == pTopTarget->iAllyID);
}

// No longer checks the vision phase directly
// Vision phasing is addressed in the target-gathering code
// This allows powers to hit targets in different vision phases if the target type accepts it
bool character_TargetIsInCombatPhase(Character *pSrc, Character *pTarget)
{
    int combinedPhasesSrc = pSrc ? pSrc->bitfieldCombatPhases : 0;
    int combinedPhasesTarget = pSrc ? pTarget->bitfieldCombatPhases : 0;
    int combinedPhases;
    Character *pIterator;
    Entity *e;
    
    pIterator = pSrc;
    while (pIterator)
    {
        // Prime phase needs to be AND'd, all others need to be OR'd.
        if ((combinedPhasesSrc & 1) && (pIterator->bitfieldCombatPhases & 1))
        {
            combinedPhasesSrc |= pIterator->bitfieldCombatPhases;
        }
        else
        {
            combinedPhasesSrc |= pIterator->bitfieldCombatPhases;
            combinedPhasesSrc &= ~1;
        }

        if (pIterator->entParent && pIterator->entParent->erOwner &&
            (e = erGetEnt(pIterator->entParent->erOwner)))
        {
            pIterator = e->pchar; // okay if e->pchar is NULL, just breaks loop
        }
        else
        {
            pIterator = 0; // break loop
        }
    }

    pIterator = pTarget;
    while (pIterator)
    {
        // Prime phase needs to be AND'd, all others need to be OR'd.
        if ((combinedPhasesTarget & 1) && (pIterator->bitfieldCombatPhases & 1))
        {
            combinedPhasesTarget |= pIterator->bitfieldCombatPhases;
        }
        else
        {
            combinedPhasesTarget |= pIterator->bitfieldCombatPhases;
            combinedPhasesTarget &= ~1;
        }

        if (pIterator->entParent && pIterator->entParent->erOwner &&
            (e = erGetEnt(pIterator->entParent->erOwner)))
        {
            pIterator = e->pchar; // okay if e->pchar is NULL, just breaks loop
        }
        else
        {
            pIterator = 0; // break loop
        }
    }

    combinedPhases = combinedPhasesSrc & combinedPhasesTarget;

    if (!pSrc || !pTarget)
    {
        return true; // sanity check to not kill things unless I have to
    }
    else if (combinedPhases == 2) // In the Friendly phase and ONLY the Friendly phase
    {
        return character_TargetIsFriend(pSrc, pTarget);
    }
    else
    {
        return combinedPhases;
    }
}

bool entity_TargetIsInVisionPhase(Entity *pSrc, Entity *pTarget)
{
    int returnMe = true;
    int combinedPhasesSrc = pSrc ? pSrc->bitfieldVisionPhases : 0;
    int combinedPhasesTarget = pTarget ? pTarget->bitfieldVisionPhases : 0;
    int combinedPhases;
    Entity* pIterator = NULL;
    Entity* pSrcOwner = NULL;
    Entity* pTargetOwner = NULL;

    if (!pSrc || !pTarget)
    {
        return true; // sanity check to not phase things out unless I should
    }

    pIterator = pSrc;
    while (pIterator)
    {
        // Prime phase needs to be AND'd, all others need to be OR'd.
        if ((combinedPhasesSrc & 1) && (pIterator->bitfieldVisionPhases & 1))
        {
            combinedPhasesSrc |= pIterator->bitfieldVisionPhases;
        }
        else
        {
            combinedPhasesSrc |= pIterator->bitfieldVisionPhases;
            combinedPhasesSrc &= ~1;
        }

        pSrcOwner = pIterator;

        if (pIterator->erOwner)
        {
            pIterator = erGetEnt(pIterator->erOwner);
        }
        else
        {
            pIterator = 0; // break loop
        }
    }

    pIterator = pTarget;
    while (pIterator)
    {
        // Prime phase needs to be AND'd, all others need to be OR'd.
        if ((combinedPhasesTarget & 1) && (pIterator->bitfieldVisionPhases & 1))
        {
            combinedPhasesTarget |= pIterator->bitfieldVisionPhases;
        }
        else
        {
            combinedPhasesTarget |= pIterator->bitfieldVisionPhases;
            combinedPhasesTarget &= ~1;
        }

        pTargetOwner = pIterator;

        if (pIterator->erOwner)
        {
            pIterator = erGetEnt(pIterator->erOwner); // okay if e->pchar is NULL, just breaks loop
        }
        else
        {
            pIterator = 0; // break loop
        }
    }

    combinedPhases = combinedPhasesSrc & combinedPhasesTarget;

    if (combinedPhases == 2) // In the Friendly phase and ONLY the Friendly phase
    {
        returnMe = (!pSrc->pchar || !pTarget->pchar || character_TargetIsFriend(pSrc->pchar, pTarget->pchar));
        // If either Entity is not a Character, assume they are Friendly
    }
    else
    {
        returnMe = (combinedPhases != 0);
    }
    
    if (returnMe)
    {
        // Now check exclusive vision phasing
        // if you change this logic, change entity_PNPCIsInVisionPhase
        if (pSrc->exclusiveVisionPhase == EXCLUSIVE_VISION_PHASE_ALL 
            || pTarget->exclusiveVisionPhase == EXCLUSIVE_VISION_PHASE_ALL)
        {
            returnMe = true;
        }
        else if (pSrc->exclusiveVisionPhase == EXCLUSIVE_VISION_PHASE_ALL_BUT_NO_TRAFFIC
                    || pTarget->exclusiveVisionPhase == EXCLUSIVE_VISION_PHASE_ALL_BUT_NO_TRAFFIC)
        {
            returnMe = (pSrc->exclusiveVisionPhase != EXCLUSIVE_VISION_PHASE_NO_TRAFFIC 
                        && pTarget->exclusiveVisionPhase != EXCLUSIVE_VISION_PHASE_NO_TRAFFIC);
        }
        else if (pSrc->exclusiveVisionPhase == EXCLUSIVE_VISION_PHASE_SEE_NO_OTHER_PLAYERS
                    && pTarget->exclusiveVisionPhase == EXCLUSIVE_VISION_PHASE_SEE_NO_OTHER_PLAYERS)
        {
            // pSrcOwner and pTargetOwner shouldn't be able to be NULL here
            returnMe = (pSrcOwner == pTargetOwner) || !(ENTTYPE(pSrcOwner) == ENTTYPE_PLAYER && ENTTYPE(pTargetOwner) == ENTTYPE_PLAYER);
        }
        else
        {
            returnMe = (pSrc->exclusiveVisionPhase == pTarget->exclusiveVisionPhase);
        }
    }

    return returnMe;
}

bool entity_PNPCIsInVisionPhase(Entity *pSrc, const PNPCDef *pNPC)
{
    int returnMe = true;
    int combinedPhasesSrc = pSrc ? pSrc->bitfieldVisionPhases : 0;
    int combinedPhasesTarget = pNPC ? pNPC->bitfieldVisionPhases : 0;
    int combinedPhases;
    Entity *pIterator;

    if (!pSrc || !pNPC)
    {
        return true; // sanity check to not phase things out unless I should
    }

    pIterator = pSrc;
    while (pIterator)
    {
        // Prime phase needs to be AND'd, all others need to be OR'd.
        if ((combinedPhasesSrc & 1) && (pIterator->bitfieldVisionPhases & 1))
        {
            combinedPhasesSrc |= pIterator->bitfieldVisionPhases;
        }
        else
        {
            combinedPhasesSrc |= pIterator->bitfieldVisionPhases;
            combinedPhasesSrc &= ~1;
        }

        if (pIterator->erOwner)
        {
            pIterator = erGetEnt(pIterator->erOwner);
        }
        else
        {
            pIterator = 0; // break loop
        }
    }

    combinedPhases = combinedPhasesSrc & combinedPhasesTarget;

    if (combinedPhases == 2) // In the Friendly phase and ONLY the Friendly phase
    {
        returnMe = true;
        // If either Entity is not a Character, assume they are Friendly
        // Since second isn't even an Entity, we must be Friendly
    }
    else
    {
        returnMe = (combinedPhases != 0);
    }

    if (returnMe)
    {
        // Now check exclusive vision phasing
        // if you change this logic, change entity_PNPCIsInVisionPhase
        if (pSrc->exclusiveVisionPhase == EXCLUSIVE_VISION_PHASE_ALL 
            || pNPC->exclusiveVisionPhase == EXCLUSIVE_VISION_PHASE_ALL)
        {
            returnMe = true;
        }
        else if (pSrc->exclusiveVisionPhase == EXCLUSIVE_VISION_PHASE_ALL_BUT_NO_TRAFFIC
            || pNPC->exclusiveVisionPhase == EXCLUSIVE_VISION_PHASE_ALL_BUT_NO_TRAFFIC)
        {
            returnMe = (pSrc->exclusiveVisionPhase != EXCLUSIVE_VISION_PHASE_NO_TRAFFIC 
                && pNPC->exclusiveVisionPhase != EXCLUSIVE_VISION_PHASE_NO_TRAFFIC);
        }
        // don't bother checking EXCLUSIVE_VISION_PHASE_SEE_NO_OTHER_PLAYERS explicitly
        // since a PNPC cannot be a player, that phase is always the same as a regular phase
        else
        {
            returnMe = (pSrc->exclusiveVisionPhase == pNPC->exclusiveVisionPhase);
        }
    }

    return returnMe;
}

/**********************************************************************func*
 * character_TargetMatchesType
 *
 */
bool character_TargetMatchesTypeEx(Character *pSrc, Character *pTarget, TargetType eType, bool targetsThroughVisionPhase, int *wouldHaveIfInPhase)
{
    int temp;
    bool bDead;

    if (!wouldHaveIfInPhase)
    {
        wouldHaveIfInPhase = &temp;
    }
    (*wouldHaveIfInPhase) = false;

    if (!pTarget || !pSrc || !pSrc->entParent || !pTarget->entParent)
    {
        return false;
    }

#ifdef CLIENT
    bDead = pTarget->attrCur.fHitPoints <= 0; // Apparently CLIENT doesn't know about ENT_DEAD
#else // SERVER
    bDead = entMode(pTarget->entParent, ENT_DEAD);

    if(!pTarget->entParent->untargetable || eType == kTargetType_Caster)
    {
#endif
        switch(eType)
        {
            case kTargetType_None:
                break;

            case kTargetType_Any:
                if(!bDead) 
                {
                    (*wouldHaveIfInPhase) = pTarget != pSrc;
                }
                break;

            case kTargetType_Caster:    // The caster (even if dead)
                if(pSrc==pTarget) 
                {
                    (*wouldHaveIfInPhase) = true;
                }
                break;

            case kTargetType_Player:    // Any player characters except the caster
                if(pSrc!=pTarget && !bDead)
                {
                    if(ENTTYPE(pTarget->entParent)==ENTTYPE_PLAYER) 
                    {
                        (*wouldHaveIfInPhase) = true;
                    }
                }
                break;

            case kTargetType_PlayerHero:    // Any living player of type Hero except the caster
                if(pSrc!=pTarget && !bDead)
                {
                    if(ENTTYPE(pTarget->entParent)==ENTTYPE_PLAYER
                       && ENT_IS_ON_BLUE_SIDE(pTarget->entParent))
                    {
                        (*wouldHaveIfInPhase) = true;
                    }
                }
                break;

            case kTargetType_PlayerVillain:    // Any living player of type Villain except the caster
                if(pSrc!=pTarget && !bDead)
                {
                    if(ENTTYPE(pTarget->entParent)==ENTTYPE_PLAYER
                       && ENT_IS_ON_RED_SIDE(pTarget->entParent))
                    {
                        (*wouldHaveIfInPhase) = true;
                    }
                }
                break;

            case kTargetType_DeadPlayer: // Any dead player characters INCLUDING caster
                if(ENTTYPE(pTarget->entParent)==ENTTYPE_PLAYER && bDead) 
                {
                    (*wouldHaveIfInPhase) = true;
                }
                break;

            case kTargetType_Villain:   // Any AI-controlled evil villain except the caster
                if(pSrc!=pTarget && !bDead)
                {
                    if(ENTTYPE(pTarget->entParent)==ENTTYPE_CRITTER    && (pTarget->iAllyID==kAllyID_Evil || pTarget->iAllyID==kAllyID_Foul))
                    {
                        (*wouldHaveIfInPhase) = true;
                    }
                }
                break;

            case kTargetType_DeadVillain: // Any dead AI-controlled evil villain INCLUDING the caster
                if(bDead && ENTTYPE(pTarget->entParent)==ENTTYPE_CRITTER && (pTarget->iAllyID==kAllyID_Evil || pTarget->iAllyID==kAllyID_Foul))
                {
                    (*wouldHaveIfInPhase) = true;
                }
                break;

            case kTargetType_NPC:       // Any NPC but villains
                if(ENTTYPE(pTarget->entParent)==ENTTYPE_NPC && !bDead) 
                {
                    (*wouldHaveIfInPhase) = true;
                }
                break;

            case kTargetType_Friend:            // Anybody on the same "team" except the caster
            case kTargetType_DeadFriend:        // Anybody dead on the same "team" except the caster
            case kTargetType_DeadPlayerFriend:  // Any dead players on the same side as the caster except the caster
            case kTargetType_DeadOrAliveFriend: // Anybody on the same "team" except the caster, dead or alive
                if(pSrc!=pTarget)
                {
                    bool bIsFriend = character_TargetIsFriend(pSrc, pTarget);

                    if(character_IsConfused(pSrc))
                    {
                        // Players and pets owned by players
                        if(ENTTYPE(pSrc->entParent)==ENTTYPE_PLAYER
                            || erGetDbId(pSrc->entParent->erOwner)!=0)
                        {
                            bIsFriend = true;
                        }
                        else
                        {
                            bIsFriend = !bIsFriend;
                        }
                    }

                    if(bIsFriend)
                    {
                        if((eType==kTargetType_DeadOrAliveFriend)
                            || ( bDead && eType==kTargetType_DeadFriend)
                            || ( bDead && eType==kTargetType_DeadPlayerFriend && ENTTYPE(pTarget->entParent)==ENTTYPE_PLAYER)
                            || (!bDead && eType==kTargetType_Friend))
                        {
                            (*wouldHaveIfInPhase) = true;
                        }
                    }
                }
                break;

            case kTargetType_Foe:            // Anybody on the other "team" except the caster
            case kTargetType_DeadFoe:        // Anybody dead on the same "team" except the caster
            case kTargetType_DeadPlayerFoe:  // Any dead players on the same side as the caster except the caster
            case kTargetType_DeadOrAliveFoe: // Anybody on the other "team" except the caster, dead or alive
                if(pSrc!=pTarget)
                {
                    bool bIsFoe = character_TargetIsFoe(pSrc,pTarget);

                    if(character_IsConfused(pSrc))
                    {
                        // Players and pets owned by players
                        if (ENTTYPE(pSrc->entParent)==ENTTYPE_PLAYER
                                || erGetDbId(pSrc->entParent->erOwner)!=0)
                        {
                            if (team_IsMember(pSrc->entParent, pTarget->entParent->db_id))
                                bIsFoe = true;
                            else if (team_IsMember(pTarget->entParent, erGetDbId(pSrc->entParent->erOwner)))
                                bIsFoe = true;
                            else
                                bIsFoe = false;
                        }
                        else
                        {
                            bIsFoe = !bIsFoe;
                        }
                    }

                    // Check if the characters are properly set active IFF this is a pvp map
                    if (bIsFoe && server_visible_state.isPvP)
                    {
                        // Find the owners
                        Entity *pentSrcOwner = erGetEnt(pSrc->entParent->erOwner);
                        Entity *pentTargetOwner = erGetEnt(pTarget->entParent->erOwner);
                        if (!pentSrcOwner) pentSrcOwner = pSrc->entParent;
                        if (!pentTargetOwner) pentTargetOwner = pTarget->entParent;

                        if (ENTTYPE(pentSrcOwner) == ENTTYPE_PLAYER 
                            && ENTTYPE(pentTargetOwner) == ENTTYPE_PLAYER
                            && !(character_IsPvPActive(pentSrcOwner->pchar) && character_IsPvPActive(pentTargetOwner->pchar)))
                        {
                            bIsFoe = false;
                        }
                    }

#if SERVER
                    //MW if this is and exchange between cut scene actors, let them do what they want... TO DO for area of effect this won't quite work...
                    if( pSrc->entParent->cutSceneActor && pTarget->entParent->cutSceneActor )
                        bIsFoe = true;
#endif

                    if(bIsFoe)
                    {
                        if((eType==kTargetType_DeadOrAliveFoe)
                            || ( bDead && eType==kTargetType_DeadFoe)
                            || ( bDead && eType==kTargetType_DeadPlayerFoe && ENTTYPE(pTarget->entParent)==ENTTYPE_PLAYER)
                            || (!bDead && eType==kTargetType_Foe))
                        {
                            (*wouldHaveIfInPhase) = true;
                        }
                    }
                }
                break;

            case kTargetType_Teammate:            // Any teammate or teammate's pet except the caster
            case kTargetType_DeadTeammate:        // Any dead teammate or teammate's dead pet except the caster
            case kTargetType_DeadOrAliveTeammate: // Any teammate or teammate's pet except the caster, dead or alive
                if(pSrc!=pTarget)
                {
                    if(!character_IsConfused(pSrc))
                    {
                        bool areTeammates = entsAreTeamed(pSrc->entParent, pTarget->entParent, false);
                        
                        if(areTeammates)
                        {
                            if ((eType == kTargetType_DeadOrAliveTeammate)
                                || ( bDead && eType == kTargetType_DeadTeammate)
                                || (!bDead && eType == kTargetType_Teammate))
                            {
                                (*wouldHaveIfInPhase) = true;
                            }
                        }
                    }
                }
                break;

            case kTargetType_Leaguemate:            // Any living leaguemate and their pets except the caster
            case kTargetType_DeadLeaguemate:        // Any dead leaguemate and all leaguemate's dead pets except the caster
            case kTargetType_DeadOrAliveLeaguemate: // All leaguemates and their pets, dead or alive except the caster
                if(pSrc!=pTarget)
                {
                    if(!character_IsConfused(pSrc))
                    {
                        bool areLeaguemates = entsAreTeamed(pSrc->entParent, pTarget->entParent, true);

                        if(!areLeaguemates)
                            areLeaguemates = entsAreTeamed(pSrc->entParent, pTarget->entParent, false); // Teams count for Leaguemate targeting
                        
                        if(areLeaguemates)
                        {
                            if((eType==kTargetType_DeadOrAliveLeaguemate)
                                || ( bDead && eType==kTargetType_DeadLeaguemate)
                                || (!bDead && eType==kTargetType_Leaguemate))
                            {
                                (*wouldHaveIfInPhase) = true;
                            }
                        }
                    }
                }
                break;

            case kTargetType_MyPet:            // The target is alive and is the source is the owner
            case kTargetType_DeadMyPet:        // The target is dead and is the source is the owner
            case kTargetType_DeadOrAliveMyPet: // Any target where the source is the owner
                {
                    Entity *e = erGetEnt(pTarget->entParent->erOwner);
                    if(e == pSrc->entParent)
                    {
                        if((eType==kTargetType_DeadOrAliveMyPet)
                            || ( bDead && eType==kTargetType_DeadMyPet)
                            || (!bDead && eType==kTargetType_MyPet))
                        {
                            (*wouldHaveIfInPhase) = true;
                        }
                    }
                }
                break;
            case kTargetType_MyCreator:            // The target is the creator of the caster
            case kTargetType_MyOwner:            // The target is the owner of the caster
                {
                    Entity *e = erGetEnt((eType == kTargetType_MyCreator) ? pSrc->entParent->erCreator : pSrc->entParent->erOwner); 

                    if (e == pTarget->entParent && !bDead)  
                    {
                        (*wouldHaveIfInPhase) = true;
                    }

                }
                break;
            case kTargetType_MyCreation:       // The target is alive and is the source is the creator
            case kTargetType_DeadMyCreation:        // The target is dead and is the source is the creator
            case kTargetType_DeadOrAliveMyCreation: // Any target where the source is the creator
                {
                    Entity *e = erGetEnt(pTarget->entParent->erCreator);
                    if(e == pSrc->entParent)
                    {
                        if((eType==kTargetType_DeadOrAliveMyCreation)
                            || ( bDead && eType==kTargetType_DeadMyCreation)
                            || (!bDead && eType==kTargetType_MyCreation))
                        {
                            (*wouldHaveIfInPhase) = true;
                        }
                    }
                }
                break;
        }
#ifndef CLIENT
    }
#endif

    if (*wouldHaveIfInPhase)
    {
        if (targetsThroughVisionPhase)
            return true;
        else
            return entity_TargetIsInVisionPhase(pSrc->entParent, pTarget->entParent);
    }
    else
    {
        return false;
    }
}

bool character_TargetMatchesType(Character *pSrc, Character *pTarget, TargetType eType, bool targetsThroughVisionPhase)
{
    return character_TargetMatchesTypeEx(pSrc, pTarget, eType, targetsThroughVisionPhase, 0);
}

/**********************************************************************func*
 * MatchesTypeList
 *
 */
bool MatchesTypeList(Character *pSrc, Character *pTarget, const TargetType *pTypes, bool targetsThroughVisionPhase)
{
    int i;

    PERFINFO_AUTO_START("MatchesTypeList", 1);

    for(i=eaiSize(&(int *)pTypes)-1; i>=0; i--)
    {
        if(character_TargetMatchesType(pSrc, pTarget, pTypes[i], targetsThroughVisionPhase))
        {
            PERFINFO_AUTO_STOP();
            return true;
        }
    }

    PERFINFO_AUTO_STOP();
    return false;
}

/**********************************************************************func*
 * power_AffectsType
 *
 */
bool power_AffectsType(Power *ppow, TargetType type)
{
    int i;

    PERFINFO_AUTO_START("power_AffectsType", 1);

    for(i=eaiSize(&(int *)ppow->ppowBase->pAffected)-1; i>=0; i--)
    {
        if(type == ppow->ppowBase->pAffected[i])
        {
            PERFINFO_AUTO_STOP();
            return true;
        }
    }

    PERFINFO_AUTO_STOP();
    return false;
}

bool character_PowerAffectsTarget(Character *pSrc, Character *pTarget, const Power *ppow)
{
    bool bRet;

    PERFINFO_AUTO_START("PowerAffectsTarget", 1);
    bRet = MatchesTypeList(pSrc, pTarget, ppow->ppowBase->pAffected, ppow->ppowBase->bTargetsThroughVisionPhase);
#if SERVER
    if (ppow->ppowBase->ppchTargetRequires && bRet && pSrc && pTarget)
    {
        bRet &= (combateval_Eval(pSrc->entParent, pTarget->entParent, ppow, ppow->ppowBase->ppchTargetRequires, ppow->ppowBase->pchSourceFile) != 0.0f);
    }
#endif
    PERFINFO_AUTO_STOP();

    return bRet;
}

/**********************************************************************func*
 * IsTargetOutOfBounds
 *
 * Send rays in the six cardinal directions. If any of them hit a backfacing
 * polygon first, then we are inside some object. That's a no-no. We also
 * require that we can find some front-facing polygon in all directions
 * but upwards. If we can't then we must be off the edge somewhere.
 *
 */
bool IsTargetOutOfBounds(Vec3 vTarget)
{
    static Vec3 s_vTests[] =
    {
        {       0.0f,   20000.0f,       0.0f }, // up
        {       0.0f,  -20000.0f,       0.0f }, // down
        {   20000.0f,       0.0f,       0.0f }, // left
        {  -20000.0f,       0.0f,       0.0f }, // right
        {       0.0f,       0.0f,   20000.0f }, // forward
        {       0.0f,       0.0f,  -20000.0f }  // back
    };

    // Corresponds to the list above. Specifies which direction MUST hit
    //   something front-facing for this to be a valid teleport location.
    static bool s_abRequiredHits[] =
    {
        false, // up
        true,  // down
        true,  // left
        true,  // right
        true,  // forward
        true,  // back
    };

    bool abHits[6] = {0};
    bool bHitSomething = false;
    bool bOutOfBounds = false;
    int i;
    CollInfo coll;

    for(i=0; i<ARRAY_SIZE(s_vTests); i++)
    {
        Vec3 vTest;
        if (!s_abRequiredHits[i])
        {
            // If we don't need this to be true, why in hell's name are we burning cycles on a collision probe?
            continue;
        }
        addVec3(vTarget, s_vTests[i], vTest);
        if(collGrid(NULL, vTarget, vTest, &coll, 0, COLL_NOTSELECTABLE|COLL_DISTFROMSTART|COLL_BOTHSIDES))
        {
            abHits[i] = true;
            if(coll.backside)
            {
                CollInfo coll2;
                if(collGrid(NULL, vTarget, vTest, &coll2, 0, COLL_NOTSELECTABLE|COLL_DISTFROMSTART))
                {
                    if(distance3Squared(coll2.mat[3], coll.mat[3]) < 0.01)
                    {
                        // This is really close to another polygon facing the
                        // other way; they might be part of a coplanar
                        // pair. Coplanar pairs are found in world objects
                        // like trashcans and the like.
                        // So, we're going to ignore this possible
                        // hit, assuming that one of the other rays will
                        // also hit a backside triangle if we really are
                        // inside something.
                        // I doubt that this will allow sticking
                        // people inside of trashcans and such since the
                        // item needs to be big enough to contain them as
                        // well.
                    }
                    else
                    {
                        bOutOfBounds = true;
                        break;
                    }
                }
            }
        }
    }

    for(i=0; i<ARRAY_SIZE(s_abRequiredHits); i++)
    {
        if(s_abRequiredHits[i] && !abHits[i])
        {
            bOutOfBounds = true;
            break;
        }
    }

    return bOutOfBounds;
}


#define PLAYER_RADIUS 1.0f
#define PLAYER_HEIGHT 6.0f

/**********************************************************************func*
 * IsTeleportTargetValid
 *
 * Makes sure that the target can fit a player-sized capsule and isn't
 * inside any objects.
 */
bool IsTeleportTargetValid(Vec3 vecTarget)
{
    Vec3 vTop = { 0.0, PLAYER_HEIGHT, 0.0 };
    Vec3 vTarget = { 0 };
    Vec3 camera = { 0 };
    bool bHit = 0;
    CollInfo coll;

    addVec3(vecTarget, vTop, vTop);

#if CLIENT
    copyVec3(cam_info.cammat[3], camera);
     groupFindOnLine(camera, vecTarget, 0, &bHit, 1);
#endif

    // Disallow anything above the ceiling of 900 feet (which is short of
    // the War Wall
    if(vTop[1]>scene_info.maxHeight)
        bHit = true;

    // Make sure that there's space at the calculated target location.
    if(!bHit) 
        bHit = collGrid(NULL, vTop, vecTarget, &coll, PLAYER_RADIUS, COLL_NOTSELECTABLE|COLL_HITANY|COLL_CYLINDER|COLL_BOTHSIDES);
    if(!bHit)
        bHit = IsTargetOutOfBounds(vecTarget);

    return !bHit;
}

/**********************************************************************func*
 * ExtraTeleportChecks
 *
 * Do some last ditch teleport checks, to ensure the camera and target locations
 * are both valid
 */
bool ExtraTeleportChecks(Vec3 vecTarget, Vec3 playerLoc, Vec3 camera)
{
    bool bHit = 0;
    CollInfo coll;
    Vec3 vTop = { 0.0, PLAYER_HEIGHT, 0.0 };

    // Do we have line of sight from camera to target.  This checks for either front or back side collisions,
    // the theory being if the camera is not in a valid location, but the target is, we ought to hit the
    // back side of a surface probing from one to the other
    bHit = collGrid(NULL, camera, vecTarget, &coll, 0, COLL_NOTSELECTABLE|COLL_HITANY|COLL_BOTHSIDES);

    if(!bHit)
    {
        addVec3(playerLoc, vTop, vTop);
        // Repeat the above test for the camera to player check.
        bHit = collGrid(NULL, camera, vTop, &coll, 0, COLL_NOTSELECTABLE|COLL_HITANY|COLL_BOTHSIDES);
    }

    // last check - is the camera out of bounds?  If so all bets are off.
    if(!bHit)
        bHit = IsTargetOutOfBounds(camera);

#if !CLIENT
    // TODO add some path checking against the target location.  If we can't route to it via a beacon,
    // assume the target location is out of bounds.  That can only be done serverside, since the client
    // doesn't have beacons.  Something along the lines of:

    //beaconPathFind(navSearch, vecTarget, playerLoc, &results);
    //if(!navSearch->path.head && results.resultType == NAV_RESULT_NO_SOURCE_BEACON)
        // Fail.

#endif

    return !bHit;
}


/**********************************************************************func*
 * CalcTeleportTarget
 *
 */
CalcTeleportTargetResult CalcTeleportTarget(Mat4 matLocation, Vec3 vecTargetCalc, Vec3 probe)
{
    if (probe == NULL)
    {
        // Single try at desired location.  We didn't get a probe vector, so we
        // should not try "back stepping"
        Vec3 vTop = { 0.0, PLAYER_HEIGHT, 0.0 };

        // Move the character off of the destination by their radius
        // and put current_location_target at the feet and vTop at
        // the top. (The +0.01 is to keep the actual target surface from
        // intersecting.)
        scaleVec3(matLocation[1], PLAYER_RADIUS+0.01f, vecTargetCalc);

        // First let's try to put the feet there.
        addVec3(matLocation[3], vecTargetCalc, vecTargetCalc);

        // If down-facing or the spot doesn't seem to work, put the player's head
        // at the target instead.
        if(matLocation[1][1] < 0 || !IsTeleportTargetValid(vecTargetCalc))
        {
            // Doesn't work with the feet. Let's try to put the head there instead.
            subVec3(vecTargetCalc, vTop, vecTargetCalc);
        }
        return TELEPORT_TARGET_OK;
    }
    else
    {
        // Original probe vector provided.  If we get a bad location, we can use this to step back
        // and try to find a better location.
        // General gameplan.  If original probe vector is longer than 10 feet, drop back by 3.
        // If that fails, and the original is longer than 25 feet, drop back another 12.
        // If that fails, move forward a fraction so that at least we don't fall to our death.

        // Need the length so we can normalize
        float len = lengthVec3(probe);
        float three_over_len = 3.0f / len;
        Vec3 current_candidate;
        Vec3 back_step;

        // Single try at desired location.  We didn't get a probe vector, so we
        // should not try "back stepping"
        Vec3 vTop = { 0.0, PLAYER_HEIGHT, 0.0 };

        // get the initial backstep, five feet
        scaleVec3(probe, three_over_len, back_step);

        // Move the character off of the destination by their radius
        // and put current_location_target at the feet and vTop at
        // the top. (The +0.01 is to keep the actual target surface from
        // intersecting.)
        scaleVec3(matLocation[1], PLAYER_RADIUS+0.01f, vecTargetCalc);

        // First let's try to put the feet there.
        addVec3(matLocation[3], vecTargetCalc, vecTargetCalc);

        // save current candidate "foot" location
        copyVec3(vecTargetCalc, current_candidate);

        // If this is good, we're done
        if(matLocation[1][1] >= 0 && IsTeleportTargetValid(vecTargetCalc))
            return TELEPORT_TARGET_OK;

        // Doesn't work with the feet. Let's try to put the head there instead.
        subVec3(vecTargetCalc, vTop, vecTargetCalc);

        if(IsTeleportTargetValid(vecTargetCalc))
            return TELEPORT_TARGET_OK;

        // First try failed.  Try 3 feet back if we can
        if (len >= 10.0f)
        {
            addVec3(current_candidate, back_step, vecTargetCalc);

            // save current candidate "foot" location
            copyVec3(vecTargetCalc, current_candidate);

            // If this is good, we're done
            if(IsTeleportTargetValid(vecTargetCalc))
                return TELEPORT_TARGET_FEET;

            // Doesn't work with the feet. Let's try to put the head there instead.
            subVec3(vecTargetCalc, vTop, vecTargetCalc);

            if(IsTeleportTargetValid(vecTargetCalc))
                return TELEPORT_TARGET_HEAD;

            // Second try failed.  Try 15 feet back if we can
            if (len >= 25.0f)
            {
                scaleVec3(back_step, 4.0f, back_step);
                addVec3(current_candidate, back_step, vecTargetCalc);
                scaleVec3(back_step, 0.25f, back_step);

                // If this is good, we're done
                if(IsTeleportTargetValid(vecTargetCalc))
                    return TELEPORT_TARGET_FEET;

                // Doesn't work with the feet. Let's try to put the head there instead.
                subVec3(vecTargetCalc, vTop, vecTargetCalc);

                if(IsTeleportTargetValid(vecTargetCalc))
                    return TELEPORT_TARGET_HEAD;
            }
        }
        // If we reach here, we could not find a valid location by backstepping.  Last ditch effort is to take the player's
        // current loc, and push them forward an inch or two.  The movement is done to ensure their facing stays correct.

        // scale the step so it moves "forward"
        scaleVec3(back_step, -0.01f, back_step);
        addVec3(&probe[3], back_step, vecTargetCalc);
        vecTargetCalc[1] += 0.3f;
        return TELEPORT_TARGET_FEET;
    }
}

/**********************************************************************func*
 * CalcLocationTarget
 *
 */
void CalcLocationTarget(Mat4 matLocation, Vec3 vecTargetCalc)
{
    // Move the location off of the destination by a small bit
    // This is to keep the actual location from intersecting the target
    // surface.
    scaleVec3(matLocation[1], 0.02f, vecTargetCalc);
    addVec3(matLocation[3], vecTargetCalc, vecTargetCalc);
}

/* End of File */
