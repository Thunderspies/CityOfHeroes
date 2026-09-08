/***************************************************************************
 *     Copyright (c) 2003-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <stdio.h>
#include <memory.h>

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/file.h>
#include "dbcomm/logcomm.h"

#include "ai/entai.h"          // for AI_LOG
#include "entVarUpdate.h"   // inexplicably for sendInfoBox
#include "entity/entGameActions.h" // for setFlying and dieNow
#include "player/playerState.h"    // for STATE_SIMPLE. Client-side file
#include <utilitieslib/utils/timing.h>         // for timerSecondsSince2000
#include "entworldcoll.h"   // for entHeight
#include "entity.h"
#include "entPlayer.h"
#include "entity/entserver.h"
#include "svr/svr_base.h"
#include "graphics/camera.h"
#include "dbghelper.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/dbdoor.h"
#include "dbcomm/dbmapxfer.h"
#include "gameComm/sendToClient.h"
#include "svr/svr_player.h"
#include "language/langServerUtil.h"
#include "teamup.h"
#include "motion.h"
#include "door.h"


#include "powers.h"
#include "PowerInfo.h"
#include "auth/authUserData.h"
#include "character_base.h"
#include "character_mods.h"
#include "entity/character_combat.h"
#include "entity/character_animfx.h"
#include "character_tick.h"
#include "character_target.h"
#include "entity/character_pet.h"
#include "character_eval.h"    //    for target requires
#include "entity/buddy_server.h"    // For Buddy_Tick
#include "generator/encounter.h"
#include "player/badges_server.h"
#include "storyarc/storyarcinterface.h"
#include "storyarc/missionobjective.h"
#include "cmdparse/cmdcontrols.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "gameSys/arenamap.h"
#include "gameSys/sgraid.h"
#include "cmdparse/cmdserver.h"
#include "dbcomm/staticMapInfo.h"
#include "script/scriptengine.h"
#include "script/ZoneEvents/ScriptedZoneEvent.h"
#include "teamCommon.h"
#include "player/pophelp.h"
#include "powers.h"
#include "container/team.h"
#include "cmdparse/cmdchat.h"

#include "bases/bases.h" // For updating the sticky state of unpowered details
#include "bases/basesystems.h"

#include "costume.h"
#include "LWC_common.h"
#include "comm_game.h"
#include "ailib/aiBehaviorPublic.h"

bool g_bAutoAssist = true;

#define MAX_DEATH_TIME 120


/**********************************************************************func*
* TeleportToMapSpawnPoint - helper function for Character_NamedTeleport
*
*/
static bool TeleportToMapSpawnPoint(Character *p, int map, char *spawnPoint)
{
    StaticMapInfo *map_info;
    int xferFlag = XFER_STATIC | XFER_FINDBESTMAP;

    map_info = staticMapInfoFind(db_state.map_id);

    if (((map_info && map_info->baseMapID && map_info->baseMapID == map) || (db_state.map_id == map)) && spawnPoint)
    {
        // move to specified location on this map
        DoorEntry *door = dbFindSpawnLocation(spawnPoint);

        if (door)
        {
            entUpdatePosInstantaneous(p->entParent, door->mat[3]);
            p->entParent->move_instantly = 1;
        }
    } else {
        // move to new map
        p->entParent->adminFrozen = 1;
        p->entParent->db_flags |= DBFLAG_DOOR_XFER;
        if (spawnPoint)
        {
            // DGNOTE 11/19/2009 When script transfers are finishing a user choice, we wind up coming through
            // here.  If XFER_FINDBESTMAP makes it all the way down, it means that the following scenario fails:
            // map 1 full, map 2 part full, map 3 part full, user choses map 3.  In that setup, the user will see
            // all three maps, but if they click on map 3, that gets overridden by the presence of XFER_FINDBESTMAP
            // and they wind up on map 2.  So a leading @ is a signal not to do this, and is prepended by the code
            // towards the bottom of enterScriptDoor(...) over in door.c
            if (spawnPoint[0] == '@')
            {
                xferFlag = XFER_STATIC;
                spawnPoint++;
            }
            strcpy(p->entParent->spawn_target, spawnPoint);
        }

        dbAsyncMapMove(p->entParent, map, NULL, xferFlag);
    }

    return true;
}

/**********************************************************************func*
* character_NamedTeleport
*
*/
int character_NamedTeleport(Character *p, Power *originatingPower, const char *pDest)
{
    int retval = 0;
    int map;
    char *spawnPoint, *monorail;
    char *pch = _alloca(strlen(pDest)+1);

    strcpy(pch, pDest);

    pch = strtok(pch, ".");

    if (stricmp(pch, "base") == 0)
    {
        // base move
        Vec3 pos;
        copyVec3(ENTPOS(p->entParent), pos);

        if (!EnterBases(p->entParent, pos))
        {
            sendInfoBox(p->entParent, INFO_USER_ERROR, localizedPrintf(p->entParent,"SGDoesNotOwnBase"));
        }
        else
        {
            retval = 1;
        }
    } else if (stricmp(pch, "mission") == 0) {
        ClientLink *pClient = NULL; 
        if (p->entParent != NULL)
        {
            pClient = clientFromEnt(p->entParent);

            if (pClient != NULL)
            {
                if (MissionGotoMissionDoor(pClient))
                {
                    retval = 1;
                }
            }
        }
    } else if (stricmp(pch, "monorail") == 0) {
        monorail = strtok(NULL, ".");

        if (monorail)
        {
            if (CatchMonorail(p->entParent, monorail))
            {
                retval = 1;
            }
        }
    } else if (stricmp(pch, "Ouroboros") == 0) {
        Entity *e = p->entParent;
        LWC_STAGE mapStage;
        map = staticMapInfoFindOuroboros(ENT_IS_VILLAIN(e), ENT_IS_IN_PRAETORIA(e));
        if (map)
        {
            if (LWC_CheckMapIDReady(map, LWC_GetCurrentStage(e), &mapStage))
            {
                spawnPoint = strtok(NULL, ".");

                if (spawnPoint)
                {
                    if (TeleportToMapSpawnPoint(p, map, spawnPoint))
                    {
                        retval = 1;
                    }
                }
            }
            else
            {
                sendDoorMsg(e, DOOR_MSG_FAIL, localizedPrintf(e, "LWCDataNotReadyMap", mapStage));
            }
        }
    } else if (stricmp(pch, "ScriptTeleport") == 0) {
        Entity *e = p->entParent;
        if (e)
        {
            if (ScriptPlayerMissionPort(e))
            {
                retval = 1;
            }
        }
    } else if (stricmp(pch, "SonicFenceReject") == 0) {
        Entity *e = p->entParent;
        if (e && e->pl)
        {
            if (entUpdatePosInstantaneous(e, e->pl->volume_reject_pos))
            {
                e->move_instantly = 1;
                retval = 1;
            }
        }
    } else if (strnicmp(pch, "marker:", 7 /*strlen("marker:")*/) == 0) {
        Entity *e = p->entParent;
        if (e)
        {
            Vec3 markerPos;
            if (GetPointFromLocation(pch, markerPos))
            {
                if (entUpdatePosInstantaneous(e, markerPos))
                {
                    e->move_instantly = 1;
                    retval = 1;
                }
            }
        }
    }
    else{
        Entity *e = p->entParent;
        LWC_STAGE mapStage;

        if (stricmp(pch, "CurrentMap") == 0)
            map = db_state.map_id;
        else
            map = atoi(pch); // should be base id

        if (LWC_CheckMapIDReady(map, LWC_GetCurrentStage(e), &mapStage))
        {
            spawnPoint = strtok(NULL, ".");

            if (map > 0 && spawnPoint)
            {
                if (TeleportToMapSpawnPoint(p, map, spawnPoint))
                {
                    retval = 1;
                }
            }
        }
        else
        {
            sendDoorMsg(e, DOOR_MSG_FAIL, localizedPrintf(e, "LWCDataNotReadyMap", mapStage));
        }
    }

    if (!retval && originatingPower && originatingPower->ppowBase->bHasUseLimit)
    {
        originatingPower->iNumCharges++;
    }

    return retval;
}

/**********************************************************************func*
 * character_SpendCost
 *
 */
static bool character_SpendCost(Character *p, Power *ppow)
{
    float fCostEnd = ppow->ppowBase->fEnduranceCost/(ppow->pattrStrength->fEnduranceDiscount+0.0001f);
    float fCostIns = ppow->ppowBase->fInsightCost/(ppow->pattrStrength->fInsightDiscount+0.0001f);

    float fEndurance = p->attrCur.fEndurance - fCostEnd;
    float fInsight = p->attrCur.fInsight - fCostIns;

    if(fEndurance<0.0f)
    {
        LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Power:Cancelled Not enough endurance: has %.3f, needs %.3f for %s", fEndurance, fCostEnd, dbg_BasePowerStr(ppow->ppowBase));
        return false;
    }

    if(!p->entParent->invincible)
    {
        p->attrCur.fEndurance = fEndurance;
        p->attrCur.fInsight = fInsight;
    }
    return true;
}

static void character_RefreshPowersOnActivePlayerChange(Character *pchar)
{
    int powersetIndex, powersetCount, powerIndex, powerCount;
//    sendChatMsg(pchar->entParent, "Refreshing Active Player Powers...", INFO_SVR_COM, 0);

    // assumes pchar is not NULL
    powersetCount = eaSize(&pchar->ppPowerSets);
    for (powersetIndex = 0; powersetIndex < powersetCount; powersetIndex++)
    {
        if (pchar->ppPowerSets[powersetIndex])
        {
            powerCount = eaSize(&pchar->ppPowerSets[powersetIndex]->ppPowers);
            for (powerIndex = 0; powerIndex < powerCount; powerIndex++)
            {
                Power *power = pchar->ppPowerSets[powersetIndex]->ppPowers[powerIndex];
                if (power)
                {
                    const BasePower *basePower = power->ppowBase;
                    if (basePower && basePower->bRefreshesOnActivePlayerChange)
                    {
                        character_RemovePower(pchar, power);
                        power = NULL; // no longer valid, only changes local copy, not Character's
                        character_AddRewardPower(pchar, basePower);
                    }
                }
            }
        }
    }
}

/**********************************************************************func*
 * Recharge_Tick
 *
 */
static void Recharge_Tick(Character *p, float fRate)
{
    PowerListItem *ppowListItem;
    PowerListIter iter;

    // Recharge powers recently used.
    for(ppowListItem = powlist_GetFirst(&p->listRechargingPowers, &iter);
        ppowListItem != NULL;
        ppowListItem = powlist_GetNext(&iter))
    {
        if(ppowListItem->ppow->fTimer <= 0.0f
            || power_CheckUsageLimits(ppowListItem->ppow))
        {
            // If it's fully recharged, remove it from the list.
            Power *ppow = ppowListItem->ppow;

            powlist_RemoveCurrent(&iter);
            ppow->fTimer = 0.0f; // possibly redundant?

            if(ppow->ppowBase->fRechargeTime > 0.25f)
            {
                // I arbitrarily chose to ignore short recharge times.
                sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "PowerRecharged", ppow->ppowBase->pchDisplayName);
            }
        }
        else
        {
            ppowListItem->ppow->fTimer -= fRate*ppowListItem->ppow->pattrStrength->fRechargeTime;
        }
    }
}


/**********************************************************************func*
 * RetryQueuedPower
 *
 */
static void RetryQueuedPower(Character *p)
{
    if(ENTTYPE(p->entParent)==ENTTYPE_PLAYER)
    {
        if(p->ppowQueued->ppowBase->fInterruptTime==0.0f)
        {
            p->ppowTried = p->ppowQueued;
            p->erTargetTried = p->erTargetQueued;
            copyVec3(p->vecLocationQueued, p->vecLocationTried);
        }
    }
}

/**********************************************************************func*
 * PowerQueue_Tick
 *
 */
static void PowerQueue_Tick(Character *p)
{
    PowerListItem *ppowListItem;
    PowerListIter iter;
    int i;

    enum
    {
        kSource_Queue,
        kSource_Retry,
        kSource_Default
    } eSource = kSource_Queue;

    // If there is no active or queued power and we have tried to use one
    // (and failed) earlier, or we have a default, make it go.
    if(p->ppowCurrent==NULL && p->ppowQueued==NULL)
    {
        if (p->ppowTried)
        {
            Entity *pentTarget = erGetEnt(p->erTargetTried);
            power_findRedirection(p->entParent, pentTarget, p->ppowTried);
        }
        // Make sure we can actually activate a power right now.
        if (p->ppowTried && character_AllowedToActivatePower(p, p->ppowTried, false))
        {
            if ((p->ppowQueued != p->ppowTried) && p->ppowQueued)
                character_ReleasePowerChain(p, p->ppowQueued);
            p->ppowQueued = p->ppowTried;
            p->erTargetQueued = p->erTargetTried;
            copyVec3(p->vecLocationTried, p->vecLocationQueued);
            eSource = kSource_Retry;
        }
        else if (p->ppowAlternateQueue)
        {
            //we're not checking if character_AllowedToActivatePower is true because it is
            //an inspiration and we presume that the player can activate it
            //NOTE: do one more pass to ensure null checks are needed.
            p->ppowQueued = p->ppowAlternateQueue;
            p->erTargetQueued = p->erTargetAlternateQueued;
            copyVec3(p->vecLocationAlternateQueued, p->vecLocationQueued);
            
            p->ppowAlternateQueue = NULL;
            p->erTargetAlternateQueued = 0;
        }
        else if (p->ppowDefault && p->attrCur.fConfused <= 0.0)
        {
            ClientLink *pClient = clientFromEnt(p->entParent);
            if(pClient)
            {
                Entity *eTarget = validEntFromId(pClient->controls.selected_ent_server_index);
                if (p->ppowDefault)
                {
                    power_findRedirection(p->entParent, eTarget, p->ppowDefault);
                }
                if (character_AllowedToActivatePower(p, p->ppowDefault, false))
                {
                    if ((p->ppowQueued != p->ppowDefault) && p->ppowQueued)
                        character_ReleasePowerChain(p, p->ppowQueued);
                    p->ppowQueued = p->ppowDefault;
                    p->erTargetQueued = erGetRef(eTarget);
                    copyVec3(ENTPOS(p->entParent), p->vecLocationQueued);
                    eSource = kSource_Default;
                }
            }
        }
    }

    // If there is no active power and we have one queued up to go
    if(p->ppowCurrent==NULL && p->ppowQueued!=NULL)
    {
        Entity *pentTarget = NULL;
        bool bValidLocation = true;

        if(p->erTargetQueued!=0)
        {
            pentTarget = erGetEnt(p->erTargetQueued);
        }
        else
        {
            // This was queued to assist someone
            Entity *eAssist = erGetEnt(p->erAssist);

            if(eAssist!=NULL && eAssist->pchar!=NULL)
            {
                pentTarget = eAssist;
                if(!character_TargetMatchesType(p, eAssist->pchar, p->ppowQueued->ppowBase->eTargetType, p->ppowQueued->ppowBase->bTargetsThroughVisionPhase)
                    && eAssist->pchar->erTargetInstantaneous!=0)
                {
                    pentTarget = erGetEnt(eAssist->pchar->erTargetInstantaneous);
                }
            }
        }

        if(p->ppowQueued->ppowBase->eTargetType == kTargetType_Caster)
        {
            pentTarget = p->entParent;
        }

        power_findRedirection(p->entParent, pentTarget, p->ppowQueued);

        if (!(p->ppowQueued && p->ppowQueued->ppowBase
            && p->ppowQueued->ppowBase->eType != kPowerType_Auto
            && p->ppowQueued->ppowBase->eType != kPowerType_Boost))
        {
            dbLog("PowerQueue_Tick", p->entParent, "Clearing the queued power %s because it's an auto power or a boost.", p->ppowQueued->ppowBase->pchName);
            goto fail;
        }

        // Click powers always go into stance, even if they fail.
        // Don't go into stance if this is an automatic power, though.
        // Bugfix for COH-7923, only do this if it's not a toggle.  Toggles get
        // turned on later after we've confirmed they can actually be activated.
        if(eSource==kSource_Queue && p->ppowQueued->ppowBase->eType!=kPowerType_Toggle)
        {
            character_EnterStance(p, p->ppowQueued, p->ppowQueued->ppowBase->eType==kPowerType_Click );
        }

        //
        // Do a bunch of checks to see if this power can be current.
        //

        // Check to see if it's still recharging
        for (ppowListItem = powlist_GetFirst(&p->listRechargingPowers, &iter);
            ppowListItem != NULL;
            ppowListItem = powlist_GetNext(&iter))
        {
            if(ppowListItem->ppow == p->ppowQueued)
                break;
        }
        if(ppowListItem != NULL)
        {
            // It's still recharging.
            if(eSource==kSource_Queue)
            {
                RetryQueuedPower(p);

                sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerStillRecharging", p->ppowQueued->ppowBase->pchDisplayName);
                serveFloatingInfo(p->entParent, p->entParent, "FloatRecharging");
            }
            goto fail;
        }

        // If they're getting up, then they're busy.
        if(entAlive(p->entParent) && getMoveFlags(p->entParent->seq, SEQMOVE_CANTMOVE) )
        {
            if(eSource==kSource_Queue)
                RetryQueuedPower(p);
            goto fail;
        }

        //
        // Validate the current target
        //

        // Validate the given location
        if(p->ppowQueued->ppowBase->eTargetType == kTargetType_Location
            || p->ppowQueued->ppowBase->eTargetType == kTargetType_Teleport)
        {
            bValidLocation &= character_LocationInRange(p, p->vecLocationQueued, p->ppowQueued);
        }
        if(p->ppowQueued->ppowBase->eTargetTypeSecondary == kTargetType_Location
            || p->ppowQueued->ppowBase->eTargetTypeSecondary == kTargetType_Teleport)
        {
            bValidLocation &= character_LocationInRange2(p, p->vecLocationQueued, p->ppowQueued);
        }
        if(!bValidLocation)
        {
            if(eSource==kSource_Queue)
            {
                RetryQueuedPower(p);

                sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerNotInRange", p->ppowQueued->ppowBase->pchDisplayName);
                serveFloatingInfo(p->entParent, p->entParent, "FloatOutOfRange");
            }
            goto fail;
        }

        // Validate the target entity
        if(p->ppowQueued->ppowBase->eTargetType != kTargetType_Location
            && p->ppowQueued->ppowBase->eTargetType != kTargetType_Teleport
            && p->ppowQueued->ppowBase->eTargetType != kTargetType_Position
            && p->ppowQueued->ppowBase->eTargetType != kTargetType_None)
        {
            if(pentTarget!=NULL && character_IsInitialized(pentTarget->pchar)
                && !character_TargetMatchesType(p, pentTarget->pchar, p->ppowQueued->ppowBase->eTargetType, p->ppowQueued->ppowBase->bTargetsThroughVisionPhase))
            {
                bool bNotAffected = true;

                if(g_bAutoAssist
                    && !OnArenaMap()
                    && g_base.rooms == NULL
                    && !server_visible_state.isPvP) // Could probably turn on isPvP when a base raid starts
                {
                    if(character_IsInitialized(pentTarget->pchar))
                    {
                        Entity *eAssistTarget = erGetEnt(pentTarget->pchar->erTargetInstantaneous);
                        if(eAssistTarget!=NULL
                            && character_TargetMatchesType(p, eAssistTarget->pchar, p->ppowQueued->ppowBase->eTargetType, p->ppowQueued->ppowBase->bTargetsThroughVisionPhase))
                        {
                            pentTarget = eAssistTarget;
                            bNotAffected=false;
                        }
                    }
                }

                if(bNotAffected)
                {
                    character_KillRetryPower(p);

                    if(eSource==kSource_Queue)
                    {
                        sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerTargetNotAffected", p->ppowQueued->ppowBase->pchDisplayName);
                        serveFloatingInfo(p->entParent, p->entParent, "FloatNoTarget");
                    }
                    goto fail;
                }
            }

            if(pentTarget==NULL
                || !character_IsInitialized(pentTarget->pchar)
                || !((entAlive(pentTarget) && g_abTargetAllowedAlive[p->ppowQueued->ppowBase->eTargetType])
                    || (g_abTargetAllowedDead[p->ppowQueued->ppowBase->eTargetType]
                        && (team_IsMember(p->entParent, pentTarget->db_id)
                            || (p->ppowQueued->ppowBase->eTargetType != kTargetType_DeadTeammate
                                && p->ppowQueued->ppowBase->eTargetType != kTargetType_DeadOrAliveTeammate)))))
            {
                character_KillRetryPower(p);

                if(eSource==kSource_Queue)
                {
                    if (pentTarget==NULL || !character_IsInitialized(pentTarget->pchar)) 
                    {
                        sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerTargetNotAffected", p->ppowQueued->ppowBase->pchDisplayName);
                        serveFloatingInfo(p->entParent, p->entParent, "FloatNoTarget");
                    }
                    else 
                    {
                        sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerTargetIsDead");
                        serveFloatingInfo(p->entParent, p->entParent, "FloatNoTarget");
                    }
                }
                goto fail;
            }

            // Check to see if the target is close enough.
            if(p->ppowQueued->ppowBase->eEffectArea == kEffectArea_Cone && p->ppowQueued->ppowBase->eTargetType != kTargetType_Caster)
            {
                F32 fDist;
                Vec3 vecLocReachableOffset;
                Vec3 vecRealLoc;

                if(!(character_EntUsesGeomColl(pentTarget) && character_FindHitLoc(p->entParent,pentTarget,vecRealLoc)))
                {
                    entGetPosAtEntTime(pentTarget, p->entParent, vecRealLoc,0);
                }

                getLocationReachableVecOffset( p->entParent, vecLocReachableOffset );

                if(!ValidateConeTarget(p, pentTarget, vecRealLoc, p->ppowQueued, &fDist, vecLocReachableOffset))
                {
                    if(eSource==kSource_Queue)
                    {
                        RetryQueuedPower(p);

                        sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerNotInRange", p->ppowQueued->ppowBase->pchDisplayName);
                        serveFloatingInfo(p->entParent, p->entParent, "FloatOutOfRange");
                    }
                    goto fail;
                }
            }
            else
            {
                if(!character_InRange(p, pentTarget->pchar, p->ppowQueued))
                {
                    if(eSource==kSource_Queue)
                    {
                        RetryQueuedPower(p);

                        sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerNotInRange", p->ppowQueued->ppowBase->pchDisplayName);
                        serveFloatingInfo(p->entParent, p->entParent, "FloatOutOfRange");
                    }
                    goto fail;
                }
            }

            // Check to see if the target is perceptible
            //  Don't do this for the AI, it has its own way of dealing with stealth
            if(ENTTYPE(p->entParent) == ENTTYPE_PLAYER && !character_TargetIsPerceptible(p, pentTarget->pchar))
            {
                if(eSource==kSource_Queue)
                {
                    RetryQueuedPower(p);

                    sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerTargetTooHardToSee");
                    serveFloatingInfo(p->entParent, p->entParent, "FloatTargetTooHardToSee");
                }
                goto fail;
            }


            if(p->ppowQueued->ppowBase->eTargetVisibility!=kTargetVisibility_None
                && !character_CharacterIsReachable(p->entParent, pentTarget))
            {
                if(eSource==kSource_Queue)
                {
                    RetryQueuedPower(p);

                    sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerTargetUnreachable", p->ppowQueued->ppowBase->pchDisplayName);
                    serveFloatingInfo(p->entParent, p->entParent, "FloatTargetUnreachable");
                }
                goto fail;
            }

            if(p->ppowQueued->ppowBase->bTargetNearGround)
            {
                if(entHeight(pentTarget, 2.0f)>1.0f)
                {
                    if (eSource==kSource_Queue)
                    {
                        sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerTargetMustBeOnGround");
                        serveFloatingInfo(p->entParent, p->entParent, "FloatTargetMustBeOnGround");
                    }

                    // Notify AI.
                    {
                        AIEventNotifyParams params = {0};

                        params.notifyType = AI_EVENT_NOTIFY_POWER_FAILED;
                        params.source = p->entParent;
                        params.target = pentTarget;
                        params.powerFailed.ppow = p->ppowQueued;
                        params.powerFailed.targetNotOnGround = 1;

                        aiEventNotify(&params);
                    }

                    goto fail;
                }
            }

            // For foe-targeted powers, we have to check for taunt and placate
            if (p->ppowQueued->ppowBase->eTargetType==kTargetType_Foe)
            {
                // Prevent entities that are taunted from attacking someone other than their taunter
                //  This currently only happens to player's entities
                if (p->pmodTaunt && p->pmodTaunt->erSource != erGetRef(pentTarget))
                {
                    Entity* pentTaunter = erGetEnt(p->pmodTaunt->erSource);
                    character_KillRetryPower(p);
                    if (eSource==kSource_Queue)
                    {
                        sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerTargetMustBeTheTaunter",(pentTaunter)?pentTaunter->name:"");
                        serveFloatingInfo(p->entParent, p->entParent, "FloatTargetMustBeTheTaunter");
                    }

                    LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Power:Cancelled %s Attacking wrong target while taunted.", dbg_BasePowerStr(p->ppowQueued->ppowBase));

                    goto fail;
                }

                // Prevent me from attacking entities that have placated me
                //  This currently only happens to player's entities
                if (p->ppmodPlacates)
                {
                    int i;
                    EntityRef erTarget = erGetRef(pentTarget);
                    for (i = eaSize(&p->ppmodPlacates)-1; i >= 0; i--)
                    {
                        if (p->ppmodPlacates[i]->erSource == erTarget)
                        {
                            character_KillRetryPower(p);
                            if (eSource==kSource_Queue)
                            {
                                sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerTargetPlacating",pentTarget->name);
                                serveFloatingInfo(p->entParent, p->entParent, "FloatTargetPlacating");
                            }

                            LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Power:Cancelled %s Attacking placating target.", dbg_BasePowerStr(p->ppowQueued->ppowBase));

                            goto fail;
                        }
                    }
                }
            }
        }

        // For things which affect a single entity, see if the power can
        // actually affect them.
        if(pentTarget!=NULL && p->ppowQueued->ppowBase->eEffectArea == kEffectArea_Character)
        {
            if(!character_PowerAffectsTarget(p, pentTarget->pchar, p->ppowQueued))
            {
                character_KillRetryPower(p);

                if (eSource==kSource_Queue)
                {
                    sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerTargetNotAffected", p->ppowQueued->ppowBase->pchDisplayName);
                    serveFloatingInfo(p->entParent, p->entParent, "FloatNoTarget");
                }
                goto fail;
            }

            if(character_NeedPowerConfirm(pentTarget->pchar, p->ppowQueued))
            {
                if(pentTarget->pl->uiDelayedInvokeID!=0
                    && timerSecondsSince2000()<pentTarget->pl->timeDelayedExpire)
                {
                    if (eSource==kSource_Queue)
                    {
                        sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerTargetConfirming", pentTarget->name);
                        serveFloatingInfo(p->entParent, p->entParent, "FloatTargetBusy");
                    }
                    goto fail;
                }
            }
        }

        // Check to make sure we're on the ground if we need to be
        if(p->ppowQueued->ppowBase->bNearGround)
        {
            MotionState* motion = p->entParent->motion;
            if(motion->falling || motion->input.flying || motion->jumping)
            {
                if (eSource==kSource_Queue)
                {
                    sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerMustBeOnGround", p->ppowQueued->ppowBase->pchDisplayName);
                    serveFloatingInfo(p->entParent, p->entParent, "FloatMustBeOnGround");
                }
                goto fail;
            }
        }

        // If its using the new ViewAttrib, check target level
        for( i = eaSize(&p->ppowQueued->ppowBase->ppTemplates)-1; i>=0; i-- )
        {
            const AttribModTemplate * pTemplate = p->ppowQueued->ppowBase->ppTemplates[i];
            if( pTemplate->offAttrib == kSpecialAttrib_ViewAttrib &&
                pentTarget->pchar->iLevel >= pTemplate->fScale ) 
            {
                character_KillRetryPower(p);
                if (eSource==kSource_Queue)
                {
                    sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "PowerTargetNotAffected", p->ppowQueued->ppowBase->pchDisplayName);
                    serveFloatingInfo(p->entParent, p->entParent, "FloatTargetTooHigh");
                }
                goto fail;
            }
        }

        //
        // Check for sufficient endurance/insight (and spend it)
        //

        if(!character_SpendCost(p, p->ppowQueued))
        {
            // Fizzle, not enough endurance
            if(eSource==kSource_Queue)
            {
                RetryQueuedPower(p);

                sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "NotEnoughEndurance", p->ppowQueued->ppowBase->pchDisplayName);
                serveFloatingInfo(p->entParent, p->entParent, "FloatNotEnoughEndurance");
                if (getPopHelpStateByTag(p->entParent, "CODEPH_Not_Enough_Endurance_For_Power") == PopHelpState_Untriggered)
                {
                    triggerPopHelpEventHappenedByTag(p->entParent, "CODEPH_Not_Enough_Endurance_For_Power");
                }
                // AI_LOG(AI_LOG_POWERS, (p->entParent, "PowQueue:  Not enough endurance for power.\n"));
            }
            goto fail;
        }

        //
        // Finally, all the checks work out. Make the power current!
        //

        character_KillRetryPower(p);
        character_ShutOffAllTogglePowersInSameGroup(p, p->ppowQueued->ppowBase);

        // Make the queued one current, free up the queue slot
        p->ppowCurrent = p->ppowQueued;
        p->erTargetCurrent = erGetRef(pentTarget); // will return 0 if given 0 (for locations)
        copyVec3(p->vecLocationQueued, p->vecLocationCurrent);
        p->bCurrentPowerActivated = false;
        p->ppowCurrent->bOnLastTick = false;
        p->ppowQueued = p->ppowQueued->chainsIntoPower;        //    this can be NULL, which is okay

        power_ChangeActiveStatus(p->ppowCurrent, true);

        // If this is an automatically executed power or a toggle, put them
        // in their stance.
        if(eSource!=kSource_Queue || p->ppowCurrent->ppowBase->eType==kPowerType_Toggle)
        {
            character_EnterStance(p, p->ppowCurrent, p->ppowCurrent->ppowBase->eType==kPowerType_Click);
        }

        // If the attacker is meant to yell something, have them yell it.
        if(p->ppowCurrent->ppowBase->pchDisplayAttackerAttack!=NULL)
        {
            int i;
            for (i=0; i<player_ents[PLAYER_ENTS_ACTIVE].count; i++)
            {
                const F32 * hearerPos;
                if( server_state.viewCutScene )
                    hearerPos = server_state.cutSceneCameraMat[3];
                else
                    hearerPos = ENTPOS(player_ents[PLAYER_ENTS_ACTIVE].ents[i]);

                if(ent_close(100, ENTPOS(p->entParent), hearerPos))
                {
                    char buf[CHAT_TEMP_BUFFER_LENTH];
                    sprintf( buf, "%s: %s", p->entParent->name, localizedPrintf(p->entParent,p->ppowCurrent->ppowBase->pchDisplayAttackerAttack));
                    sendChatMsgFrom(player_ents[PLAYER_ENTS_ACTIVE].ents[i],
                        p->entParent,
                        ENTTYPE(p->entParent)==ENTTYPE_PLAYER ? INFO_NPC_SAYS : INFO_VILLAIN_SAYS,
                        DEFAULT_BUBBLE_DURATION,
                        buf);
                }
            }
        }

        if (p->ppowCurrent->ppowBase->pchDisplayAttackerAttackFloater)
        {
            for (i=0; i<player_ents[PLAYER_ENTS_ACTIVE].count; i++)
            {
                const F32 * hearerPos;
                Entity *reciever = player_ents[PLAYER_ENTS_ACTIVE].ents[i];
                if( server_state.viewCutScene )
                    hearerPos = server_state.cutSceneCameraMat[3];
                else
                    hearerPos = ENTPOS(reciever);

                if(ent_close(100, ENTPOS(p->entParent), hearerPos))
                {
                    serveFloater(reciever, reciever, p->ppowCurrent->ppowBase->pchDisplayAttackerAttackFloater, 0, kFloaterStyle_PriorityAlert, 0xff0000ff );                
                }

            }
        }

        // Clear out the timer. We're going to time up to the
        //   TimeToActivate. This timer counts up (where others count
        //   down) since we need to know if the interrupt time has passed
        //   also.
        p->ppowCurrent->fTimer = 0.0f;

        // Reset the power timer that is tracked for the terrorize effect
        p->fTerrorizePowerTimer = 5.0;

        // Mark this power as activated if it doesn't have invoke suppression
        if (!p->ppowCurrent->ppowBase->bHasInvokeSuppression)
        {
            p->ppowLastActivated = p->ppowCurrent->ppowBase;

            // Report this Actviation event
            character_RecordEvent(p, kPowerEvent_Activate, timerSecondsSince2000());

            if(p->ppowCurrent->ppowBase->eType == kPowerType_Click)
            {
                bool bFoe = false;
                int i;
                for(i = 0; i < eaiSize(&(int*)p->ppowCurrent->ppowBase->pAffected); i++)
                {
                    if(p->ppowCurrent->ppowBase->pAffected[i] == kTargetType_DeadOrAliveFoe
                        || p->ppowCurrent->ppowBase->pAffected[i] == kTargetType_DeadPlayerFoe
                        || p->ppowCurrent->ppowBase->pAffected[i] == kTargetType_DeadFoe
                        || p->ppowCurrent->ppowBase->pAffected[i] == kTargetType_Foe
                        || p->ppowCurrent->ppowBase->pAffected[i] == kTargetType_Location
                        || p->ppowCurrent->ppowBase->pAffected[i] == kTargetType_Any)
                    {
                        bFoe = true;
                        break;
                    }
                }

                if(bFoe)
                {
                    character_RecordEvent(p, kPowerEvent_ActivateAttackClick, timerSecondsSince2000());
                }
            }

        }

        if (p->ppowCurrent->ppowBase->bFaceTarget)
        {
            if (p->ppowCurrent->ppowBase->eTargetType == kTargetType_Location
                || p->ppowCurrent->ppowBase->eTargetType == kTargetType_Teleport
                || p->ppowCurrent->ppowBase->eTargetType == kTargetType_Position
                || p->ppowCurrent->ppowBase->eTargetTypeSecondary == kTargetType_Location
                || p->ppowCurrent->ppowBase->eTargetTypeSecondary == kTargetType_Teleport
                || p->ppowCurrent->ppowBase->eTargetTypeSecondary == kTargetType_Position)
            {
                character_FaceLocation(p, p->vecLocationCurrent);
            }
            else if (pentTarget!=NULL && pentTarget != p->entParent)
            {
                Vec3 loc;
                if (character_EntUsesGeomColl(pentTarget) && character_FindHitLoc(p->entParent,pentTarget,loc))
                    character_FaceLocation(p,loc);
                else if (pentTarget->seq->type->collisionType == SEQ_ENTCOLL_BONECAPSULES && character_FindHitLoc(p->entParent, pentTarget, loc))
                    character_FaceLocation(p,loc);
                else
                    entity_FaceTarget(p->entParent, pentTarget);
            }
        }

        if(p->ppowCurrent->ppowBase->fInterruptTime!=0.0f)
        {
            // Don't bother them with this message if there is no windup.
            seqOrState(p->entParent->seq->state, 1, STATE_STARTOFFENSESTANCE);
            character_PlayMaintainedFX(p, p,
                power_GetFX(p->ppowCurrent)->pchWindUpFX,
                power_GetTint(p->ppowCurrent),
                0.0f, PLAYFX_NOT_ATTACHED, PLAYFX_NO_TIMEOUT,
                PLAYFX_NORMAL | PLAYFX_TINT_FLAG(p->ppowCurrent));

            sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "PowerWindingUp", p->ppowCurrent->ppowBase->pchDisplayName);
        }
        // AI_LOG(AI_LOG_POWERS, (p->entParent, "PowQueue:  %s is now current.\n", p->ppowCurrent->ppowBase->pchName));


        if(eSource==kSource_Default)
        {
            p->stats.iCntUsed++; // for balancing
            p->ppowCurrent->stats.iCntUsed++; // for balancing
            if (isDevelopmentMode() && !isSharedMemory(p->ppowCurrent->ppowBase))
                (cpp_const_cast(BasePower*)(p->ppowCurrent->ppowBase))->stats.iCntUsed++; // for balancing
        }
    }

    return;

// If the queued power can't be made current for some reason, we hit here.
fail:
    // Don't leave the worthless hit locs sitting around
    eaClearEx(&p->entParent->ppHitLocs,NULL);
    if(eSource!=kSource_Queue)
    {
        // We set ppowQueued in this function. Just silently null it
        //   out. The KillQueuedPower function will send a packet to the
        //   client telling it to unqueue. Since this request didn't come
        //   from the client, do send the update.
        if (p->ppowQueued)
            character_ReleasePowerChain(p, p->ppowQueued);
        p->ppowQueued = NULL;
    }
    else
    {
        character_KillQueuedPower(p);
    }
}

/**********************************************************************func*
 * CurrentPower_Tick
 *
 */
static void CurrentPower_Tick(Character *p, float fRate)
{
    if(p->ppowStance!=NULL)
        character_AddStickyStates(p, power_GetFX(p->ppowStance)->piModeBits);

    if(p->ppowCurrent!=NULL)
    {
        //AI_LOG(AI_LOG_POWERS, (p->entParent, "CurPower:  %s\n", p->ppowCurrent->ppowBase->pchName));

        if(p->ppowCurrent->fTimer < p->ppowCurrent->ppowBase->fInterruptTime)
        {
            // AI_LOG(AI_LOG_POWERS, (p->entParent, "CurPower:      (Interruptible for %6.3f)\n", p->ppowCurrent->ppowBase->fInterruptTime-(p->ppowCurrent->fTimer+fRate)));

            // Set wind-up sequence bits
            character_AddStickyStates(p, power_GetFX(p->ppowCurrent)->piWindUpBits);
        }
        else
        {
            if(!p->bCurrentPowerActivated)
            {
                bool returnNow = false;
                bool applyNow = false;

                // Shut off the WindUp FX
                character_KillMaintainedFX(p, p, power_GetFX(p->ppowCurrent)->pchWindUpFX);

                character_ApplyPowerAtActivation(p, p->ppowCurrent, p->erTargetCurrent, p->vecLocationCurrent, 0);

                // Once we're out of the interrupt time (and it's a click power)
                //   actually apply the power. This will determine if it hits, and
                //   sets up animations and attrib mods.
                // This is done early since animations need to be synchronized.
                if (p->ppowCurrent->ppowBase->eType == kPowerType_Inspiration)
                {
                    Character *p2 = p;

                    if (p->entParent && p->entParent->erOwner)
                    {
                        Entity *tempE = erGetEnt(p->entParent->erOwner);
                        if (tempE && tempE->pchar)
                        {
                            p2 = tempE->pchar;
                        }
                    }

                    if (p2->aInspirations[p->ppowCurrent->idx / CHAR_INSP_MAX_ROWS][p->ppowCurrent->idx % CHAR_INSP_MAX_ROWS] == p->ppowCurrent->ppowBase)
                    {
                        applyNow = true;
                    }
                }
                else if (p->ppowCurrent->ppowBase->eType != kPowerType_Toggle)
                // For toggle powers, power application is done in the ToggleTick.
                {
                    applyNow = true;
                }

                if (applyNow)
                {
                    // ApplyPower handles setting up the animations.
                    returnNow = !character_ApplyPower(p, p->ppowCurrent, p->erTargetCurrent, p->vecLocationCurrent, 0, 0);
                }

                // p->ppowCurrent gets cleared in character_ApplyPower if target is bad (aka if it returns false)
                // toggles are deactivated in character_ShutOffContinuingPower
                if (p->ppowCurrent && p->ppowCurrent->ppowBase->eType != kPowerType_Toggle)
                {
                    character_ApplyPowerAtDeactivation(p, p->ppowCurrent, p->erTargetCurrent, 0);
                }

                if (returnNow)
                {
                    return;
                }

                p->bCurrentPowerActivated = true;
            }
        }

        if(p->ppowCurrent->fTimer >= p->ppowCurrent->ppowBase->fTimeToActivate)
        {
            PERFINFO_AUTO_START("TimeToActivateEvent",1);
            character_RecordEvent(p, kPowerEvent_EndActivate, timerSecondsSince2000());

            if(p->ppowCurrent->ppowBase->eType==kPowerType_Toggle)
            {
                int invokeID = character_GetOnactivateInvokeID(p, p->ppowCurrent->ppowBase);

                // Put the toggle power into the list of active toggle powers.
                powlist_AddWithTargets(&p->listTogglePowers, p->ppowCurrent, p->erTargetCurrent, p->vecLocationCurrent, invokeID ? invokeID : nextInvokeId() );
                if (p->ppowCurrent->ppowBase->bShowBuffIcon)
                    p->entParent->team_buff_update = true;
                p->ppowCurrent->fTimer = 0.0f;

                LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_IMPORTANT, 0, "Power:ToggleOn %s", dbg_BasePowerStr(p->ppowCurrent->ppowBase));
            }
            else if(p->ppowCurrent->ppowBase->eType==kPowerType_Inspiration)
            {
                //
                // Inspirations are removed immediately in order to avoid
                //   having them be executed multiple times. If the recharge
                //   tick is used to remove insps, this can happen because
                //   power queueing is async. You could exec the insp during
                //   phase 1 (after the recharge tick), get the same insp
                //   queued again (async), and then have recharge happen the
                //   next time through phase 1 (removing the insp, but by
                //   then it's too late.)
                //

                Power *ppowWasCurrent = p->ppowCurrent;
                Character *p2 = p;

                if (p->entParent && p->entParent->erOwner)
                {
                    Entity *tempE = erGetEnt(p->entParent->erOwner);
                    if (tempE && tempE->pchar)
                    {
                        p2 = tempE->pchar;
                    }
                }

                // Clean these out just to be sure.
                if(p->ppowTried==ppowWasCurrent) p->ppowTried=NULL;
                if(p->ppowQueued==ppowWasCurrent) p->ppowQueued=NULL;

                if (p2->aInspirations[p->ppowCurrent->idx / CHAR_INSP_MAX_ROWS][p->ppowCurrent->idx % CHAR_INSP_MAX_ROWS] == p->ppowCurrent->ppowBase)
                {
                    // This needs to be done before CompactInspirations.
                    p->ppowCurrent = NULL;

                    // Remove the inspiration from the inventory.
                    character_RemoveInspiration(p,
                        ppowWasCurrent->idx/CHAR_INSP_MAX_ROWS,
                        ppowWasCurrent->idx-((ppowWasCurrent->idx/CHAR_INSP_MAX_ROWS)*CHAR_INSP_MAX_ROWS),
                        "used");

                    character_CompactInspirations(p);

                    // Inspirations are Powers only ephemerally. When their
                    // recharge timer goes off, they get freed up.
                    inspiration_Destroy(ppowWasCurrent);
                }
                else
                {
                    // This needs to be done before CompactInspirations.
                    p->ppowCurrent = NULL;
                }
            }
            else
            {
                // Start the power recharging.
                power_ChangeRechargeTimer(p->ppowCurrent, p->ppowCurrent->ppowBase->fRechargeTime);
                power_ChangeActiveStatus(p->ppowCurrent, false);
                powlist_Add(&p->listRechargingPowers, p->ppowCurrent, 0);
                //    free up the "tell" power so it can be used for another chain
                if (p->ppowCurrent->chainsIntoPower)
                {
                    assertmsg(p->ppowCurrent->chainsIntoPower == p->ppowQueued, "Queued power does not match next link of chained power.");
                    if (p->ppowCurrent->chainsIntoPower != p->ppowQueued)
                    {
                        //    free up the build up power
                        character_ReleasePowerChain(p, p->ppowCurrent);
                        p->ppowQueued = NULL;    //    kill the chain into power
                    }
                }
                p->ppowCurrent->chainsIntoPower = NULL;
                if (p->entParent)    //    only do this we are running a zone event script
                {
                    extern int *g_zoneEventScriptStageAwardsKarma;
                    if (eaiSize(&g_zoneEventScriptStageAwardsKarma))
                    {
                        Entity *e = p->entParent;
                        if (ENTTYPE(e) == ENTTYPE_PLAYER)
                        {
                            int currentTime = g_currentKarmaTime;
                            const BasePower *basePower = p->ppowCurrent->ppowBase;
                            int isTeamBuff = 0;
                            //    seperated these 2 so it is easier to debug
                            int **pAutoHit = &((int*)basePower->pAutoHit);
                            int **pAffected = &((int*)basePower->pAffected);
                            if ((eaiFind(pAutoHit, kTargetType_Teammate) != -1)
                                || (eaiFind(pAutoHit, kTargetType_DeadOrAliveTeammate) != -1)
                                || (eaiFind(pAutoHit, kTargetType_DeadTeammate) != -1)
                                || (eaiFind(pAutoHit, kTargetType_Friend) != -1)
                                || (eaiFind(pAutoHit, kTargetType_DeadOrAliveFriend) != -1)
                                || (eaiFind(pAutoHit, kTargetType_DeadFriend) != -1)
                                    )
                            {
                                isTeamBuff = 1;
                            }
                            else if ((eaiFind(pAffected, kTargetType_Teammate) != -1)
                                    || (eaiFind(pAffected, kTargetType_DeadOrAliveTeammate) != -1)
                                    || (eaiFind(pAffected, kTargetType_DeadTeammate) != -1)
                                    || (eaiFind(pAffected, kTargetType_Friend) != -1)
                                    || (eaiFind(pAffected, kTargetType_DeadOrAliveFriend) != -1)
                                    || (eaiFind(pAffected, kTargetType_DeadFriend) != -1))
                            {
                                isTeamBuff = 1;
                            }
                            if (isTeamBuff)
                            {
                                CharacterAddKarma(p, CKR_BUFF_POWER_CLICK, currentTime);
                            }
                            else
                            {
                                CharacterAddPowerKarma(p, currentTime, basePower);
                            }
                        }
                    }
                }
            }

            p->ppowCurrent = NULL;
        }
        else
        {
            PERFINFO_AUTO_START("TimerUpdate",1)
            p->ppowCurrent->fTimer += fRate * p->ppowCurrent->pattrStrength->fTimeToActivate;
            //AI_LOG(AI_LOG_POWERS, (p->entParent, "CurPower:       Winding up (%f left).\n", p->ppowCurrent->ppowBase->fTimeToActivate-(p->ppowCurrent->fTimer)));
        }
        PERFINFO_AUTO_STOP();
    }
}

/**********************************************************************func*
 * AutoPowers_Tick
 *
 */
static void AutoPowers_Tick(Character *p, float fRate)
{
    PowerListItem *ppowListItem;
    PowerListIter iter;

    // Loop over all the auto powers and get them processed
    for(ppowListItem = powlist_GetFirst(&p->listAutoPowers, &iter);
        ppowListItem != NULL;
        ppowListItem = powlist_GetNext(&iter))
    {
        const PowerFX* fx;

        if (!ppowListItem->ppow)
            continue;
        
        fx = power_GetFX(ppowListItem->ppow);
        assert(fx);

        if(power_CheckUsageLimits(ppowListItem->ppow) || !character_AllowedToActivatePower(p, ppowListItem->ppow, false))
        {
            character_ShutOffContinuingPower(p, ppowListItem->ppow);
            powlist_RemoveCurrent(&iter);
            continue;
        }
        else
        {
            if(ppowListItem->ppow->ppowBase->fUsageTime>0)
            {
                ppowListItem->ppow->fUsageTime-=fRate;
            }
        }

        if(ppowListItem->ppow->fTimer <= 0.0f)
        {
            // AI_LOG(AI_LOG_POWERS, (p->entParent, "AutPower:  Auto power %s activated.\n", ppowref->ppow->ppowBase->pchName));

            if(character_SpendCost(p, ppowListItem->ppow))
            {
                character_ApplyPower(p, ppowListItem->ppow, erGetRef(p->entParent), ENTPOS(p->entParent), ppowListItem->uiID, 0 );
                power_IncrementBoostTimers(ppowListItem->ppow);
                ppowListItem->ppow->bOnLastTick = true;
            }
            else
            {
                ppowListItem->ppow->bOnLastTick = false;
            }

            ppowListItem->ppow->fTimer += ppowListItem->ppow->ppowBase->fActivatePeriod;
        }

        ppowListItem->ppow->fTimer -= fRate;
        power_DecrementBoostTimers(ppowListItem->ppow, fRate);

        character_AddStickyStates(p, fx->piModeBits);

        // AI_LOG(AI_LOG_POWERS, (p->entParent, "AutPower:  %s (%f left before reactivation).\n", ppowref->ppow->ppowBase->pchName, ppowref->ppow->fTimer));
    }
}


/**********************************************************************func*
 * TogglePowers_Tick
 *
 */
static void TogglePowers_Tick(Character *p, float fRate)
{
    PowerListItem *ppowListItem;
    PowerListIter iter;
    bool bFirst = true;
    bool bShield=0, bHover=0;

    if(TSTB(p->entParent->seq->sticky_state, STATE_SHIELD))
        bShield = 1;
    if(TSTB(p->entParent->seq->sticky_state, STATE_HOVER))
        bHover = 1;

    // Sheild and hover stance will always win
    CLRB(p->entParent->seq->sticky_state, STATE_SHIELD);
    CLRB(p->entParent->seq->sticky_state, STATE_HOVER);

    // Loop over all the Toggle powers and get them processed
    for(ppowListItem = powlist_GetFirst(&p->listTogglePowers, &iter);
        ppowListItem != NULL;
        ppowListItem = powlist_GetNext(&iter))
    {
        bool bShutOff = false;
        bool bFailed = false;
        bool bLimitReached = false;
        const BasePower *bPower;
        if (!ppowListItem->ppow)
            continue;

        bPower = ppowListItem->ppow->ppowBase;

        if(ppowListItem->ppow->bActive && ppowListItem->ppow->fTimer <= 0.0f)
        {
            //AI_LOG(AI_LOG_POWERS, (p->entParent, "TogPower:  Toggle power %s activated.\n", bPower->pchName));
            Entity* pentTarget = erGetEnt(ppowListItem->erTarget);

            // Toggle power range check
#define MAX_TOGGLE_RANGE SQR(300)

            if(!bPower->bIgnoreToggleMaxDistance && bPower->eTargetType != kTargetType_Caster
                && pentTarget != NULL
                && distance3Squared(ENTPOS(p->entParent), ENTPOS(pentTarget)) >= MAX_TOGGLE_RANGE)
            {
                sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "ToggleTargetOutOfRange", bPower->pchDisplayName);
                bFailed = true;
            }
            else if (!bPower->bIgnoreToggleMaxDistance && (bPower->eTargetType == kTargetType_Location || bPower->eTargetType == kTargetType_Teleport || bPower->eTargetType == kTargetType_Position) &&
                distance3Squared(ENTPOS(p->entParent), ppowListItem->vecTarget) >= MAX_TOGGLE_RANGE)
            {
                sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "ToggleTargetOutOfRange", bPower->pchDisplayName);
                bFailed = true;
            }
            else if (bPower->eTargetType != kTargetType_Caster
                && pentTarget != NULL
                && pentTarget->pchar && !character_TargetIsPerceptible(p,pentTarget->pchar))
            {
                sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "ToggleTargetImperceptible", bPower->pchDisplayName);
                bFailed = true;
            }
            else if (ppowListItem->ppow->bDisabledManually)
            {
                // ARM NOTE: Can we switch this over to check bEnabled safely?
                // Do we want a notification?
                bFailed = true;
            }
            else
            {
                if (character_SpendCost(p, ppowListItem->ppow))
                {
                    bFailed = !character_ApplyPower(p, ppowListItem->ppow, ppowListItem->erTarget, ppowListItem->vecTarget, ppowListItem->uiID, 0);

                    ppowListItem->ppow->fTimer += bPower->fActivatePeriod;
                    if (ppowListItem->ppow->fTimer<-bPower->fActivatePeriod)
                    {
                        // This is here in case the server gets behind or the activate
                        // period is very short. It makes sure the timer doesn't keep
                        // going further and further negative.
                        ppowListItem->ppow->fTimer = -bPower->fActivatePeriod;
                    }

                    power_IncrementBoostTimers(ppowListItem->ppow);

                    ppowListItem->ppow->bOnLastTick = true;
                }
                else
                {
                    if (getPopHelpStateByTag(p->entParent, "CODEPH_Toggle_Ran_Out_Of_Endurance") == PopHelpState_Untriggered)
                    {
                        triggerPopHelpEventHappenedByTag(p->entParent, "CODEPH_Toggle_Ran_Out_Of_Endurance");
                    }

                    bShutOff = true;
                }
            }
        }

        if(!(bLimitReached = power_CheckUsageLimits(ppowListItem->ppow)))
        {
            if(bPower->fUsageTime>0)
            {
                ppowListItem->ppow->fUsageTime -= fRate;
            }
        }

        if (bFailed || bShutOff || bLimitReached || !ppowListItem->ppow->bActive || !character_AtLevelCanUsePower(p, ppowListItem->ppow))
        {
            // ExpirePowers_Tick deals with powers which are limited.
            if(bShutOff)
            {
                sendInfoBox(p->entParent, INFO_COMBAT_ERROR, "NotEnoughEndurance", bPower->pchDisplayName);
                if(bFirst)
                {
                    serveFloatingInfo(p->entParent, p->entParent, "FloatNoEndurance");
                    bFirst=false;
                }
            }

            if (ppowListItem->ppow->bActive)
            {
                sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "ShuttingOffToggle", bPower->pchDisplayName);
            }

            ppowListItem->ppow->bOnLastTick = false;
            character_ShutOffTogglePower(p, ppowListItem->ppow);
            powlist_RemoveCurrent(&iter);
            // AI_LOG(AI_LOG_POWERS, (p->entParent, "TogPower:    Fizzled. Shutting off power.\n"));
        }
        else
        {
            int i;
            const PowerFX* fx = power_GetFX(ppowListItem->ppow);
            assert(fx);

            ppowListItem->ppow->fTimer -= fRate;
            power_DecrementBoostTimers(ppowListItem->ppow, fRate);
            character_AddStickyStates(p, fx->piModeBits);

            for(i = eaiSize(&fx->piModeBits)-1; i >= 0; --i)
            {
                if( fx->piModeBits[i] == STATE_SHIELD )
                    SETB(p->entParent->seq->sticky_state, STATE_SHIELD);
                if( fx->piModeBits[i] == STATE_HOVER )
                    SETB(p->entParent->seq->sticky_state, STATE_HOVER);
            }
        }
    }

    if( bHover != TSTB(p->entParent->seq->sticky_state, STATE_HOVER) ||
        bShield != TSTB(p->entParent->seq->sticky_state, STATE_SHIELD) )
        p->entParent->move_update = 1;
}

static void GlobalBoosts_Tick(Character *p, float fRate)
{
    PowerListItem *ppowListItem;
    PowerListIter iter;

    // Loop over all the global boosts and get them processed
    for(ppowListItem = powlist_GetFirst(&p->listGlobalBoosts, &iter);
        ppowListItem != NULL;
        ppowListItem = powlist_GetNext(&iter))
    {
        if (!ppowListItem->ppow->bEnabled)
        {
            character_ShutOffGlobalBoost(p, ppowListItem->ppow);
            powlist_RemoveCurrent(&iter);
            continue;
        }

        if (ppowListItem->ppow->fTimer > 0.0f)
            ppowListItem->ppow->fTimer -= fRate;
                //    so this is a bit odd. we only dec the timer here
                //    if a non click power activates (when the timer is <= 0),
                //    it will set the timer back to the activation period
    }
}


/**********************************************************************func*
 * PowerStrength_Tick
 *
 */
static void PowerStrength_Tick(Character *p, float fRate)
{
    if(ENTTYPE(p->entParent)==ENTTYPE_PLAYER
        || p->entParent->erCreator!=0)
    {
        int i, j;
        bool bDiff = p->bRecalcStrengths
            || memcmp(&p->attrStrengthLast, &p->attrStrength, sizeof(CharacterAttributes));

        for(i=eaSize(&p->ppPowerSets)-1; i>=0; i--)
        {
            Power **pppows = p->ppPowerSets[i]->ppPowers;
            for(j=eaSize(&pppows)-1; j>=0; j--)
            {
                Power *ppow = pppows[j];
                if(ppow!=NULL && (bDiff || !ppow->bBoostsCalcuated))
                {
                    PERFINFO_AUTO_START("character_AccrueBoosts",1);
                       character_AccrueBoosts(p, ppow, fRate);
                       ppow->bBoostsCalcuated = true;
                    PERFINFO_AUTO_STOP();
                }
            }
        }

        if(bDiff)
        {
            memcpy(&p->attrStrengthLast, &p->attrStrength, sizeof(CharacterAttributes));
            p->bRecalcStrengths = false;
        }
    }
    else
    {
        // Critters don't have boosts, and I made sure that their character's
        // strength is clamped in AccrueMods already.
    }
}

/**********************************************************************func*
 * ExpirePowers_Tick
 *
 */
static void ExpirePowers_Tick(Character *p, float fRate)
{
    int i, j;

    for(i=eaSize(&p->ppPowerSets)-1; i>=0; i--)
    {
        if (!stricmp(p->ppPowerSets[i]->psetBase->pcatParent->pchName, "Inspirations"))
        {
            if (p->countInspirationPowersetRemoval < 10)
            {
                LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Power:RemoveInspiration Removing an Inspiration powerset from ppPowerSets!");
                character_RemovePowerSet(p, p->ppPowerSets[i]->psetBase, "");
                p->countInspirationPowersetRemoval++;
            }
            else if (p->countInspirationPowersetRemoval == 10)
            {
                LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Power:RemoveInspiration I've removed an Inspiration powerset too many times!");
                p->countInspirationPowersetRemoval++;
            }
            continue;
        }

        for(j=eaSize(&p->ppPowerSets[i]->ppPowers)-1; j>=0; j--)
        {
            Power *ppow=p->ppPowerSets[i]->ppPowers[j];
            if(ppow!=NULL)
            {
                if(ppow->ppowBase->bHasLifetime && ppow->ppowBase->fLifetimeInGame>0)
                {
                    ppow->fAvailableTime += fRate;
                }

                if(!ppow->bActive
                    && ppow->ppowBase->bDestroyOnLimit
                    && power_CheckUsageLimits(ppow))
                {
                    eaPushConst( &p->expiredPowers, ppow->ppowBase );
                    character_RemovePower(p, ppow);
                }
            }
        }
    }
}

/**********************************************************************func*
 * Regeneration_Tick
 *
 */
static void Regeneration_Tick(Character *p, float fRate)
{
    // TODO: These numbers are arbitrary
    float fHPPeriod = 3.0f;
    float fHPAmount = fHPPeriod/60.0f;

    float fEndPeriod = 4.0f;
    float fEndAmount = fEndPeriod/60.0f;

    float fInsightPeriod = 1.0f;
    float fInsightAmount = fInsightPeriod/60.0f;

    // No regenning while Defiant
    if(character_IsDefiant(p))
        return;

    // This function is called before character_AccrueMods, which will
    // clamp HitPoints and Endurance properly.

    while(p->fRegenerationTimer < 0.0f)
    {
        //AI_LOG(AI_LOG_POWERS, (p->entParent, "Regenera:  +%4.2f Hit Points Regenerated.\n", fHPAmount*100));
        p->fRegenerationTimer += fHPPeriod;
        p->attrCur.fHitPoints += fHPAmount * p->attrMax.fHitPoints;
        PERFINFO_AUTO_START("DamageDecay", 1);
        if(p->entParent)
            damageTrackerRegen(p->entParent, fHPAmount*p->attrMax.fHitPoints);
        PERFINFO_AUTO_STOP();
    }

    while(p->fRecoveryTimer < 0.0f)
    {
        //AI_LOG(AI_LOG_POWERS, (p->entParent, "Regenera:  +%4.2f Endurance Recovered.\n", fEndAmount*100));
        p->fRecoveryTimer += fEndPeriod;
        p->attrCur.fEndurance += fEndAmount * p->attrMax.fEndurance;
    }

    while(p->fInsightRecoveryTimer < 0.0f)
    {
        //AI_LOG(AI_LOG_POWERS, (p->entParent, "Regenera:  +%4.2f Endurance Recovered.\n", fEndAmount*100));
        p->fInsightRecoveryTimer += fInsightPeriod;
        p->attrCur.fInsight += fInsightAmount * p->attrMax.fInsight;
    }

    p->fRegenerationTimer -= fRate*p->attrCur.fRegeneration;
    p->fRecoveryTimer -= fRate*p->attrCur.fRecovery;
    p->fInsightRecoveryTimer -= fRate*p->attrCur.fInsightRecovery;
}

/**********************************************************************func*
 * character_TickPhaseOne
 *
 */
static void rewardtoken_DestroyAdapter(void* arg0)
{
    rewardtoken_Destroy((RewardToken *)arg0);
}

void character_TickPhaseOne(Character *p, float fRate)
{
    Entity *eTarget = NULL;
    ClientLink *pClient;
    U32 now = timerSecondsSince2000();
    Entity *e = p->entParent;

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    PERFINFO_AUTO_START("Housecleaning", 1);
        assert(p!=NULL);
        assert(fRate>=0.0);

        // Clean out damagers for the tick
        eaSetSizeConst(&p->ppLastDamageFX, 0);

        // Clean out anim bits for this guy
        eaiSetSize(&p->piStickyAnimBits, 0);

        // Knock terrorize power use timer if it's running, don't go below 0.0
        //  because negative indicates we're allowed to attack
        if (p->fTerrorizePowerTimer > 0.0)
        {
            p->fTerrorizePowerTimer = MAX(0.0,p->fTerrorizePowerTimer - fRate);
        }
        else
        {
            // We're negative, which means we've been hit.  Counting up towards
            //  0.0 lets us know how long we can run away, and potentially use a power
            p->fTerrorizePowerTimer = MIN(0.0,p->fTerrorizePowerTimer + fRate);
        }

        // Process the PvP zoning timer
        if (ENTTYPE(p->entParent)==ENTTYPE_PLAYER && p->entParent->pvpZoneTimer > 0.0)
        {
            p->entParent->pvpZoneTimer -= fRate;
            if (p->entParent->pvpZoneTimer <= 0.0)
            {
                p->entParent->pvpZoneTimer = 0.0;
                p->entParent->pvp_update = true;
                if (p->bPvPActive != server_visible_state.isPvP)
                {
                    p->bPvPActive = server_visible_state.isPvP;
                    sendInfoBox(p->entParent, INFO_COMBAT_SPAM, p->bPvPActive ? "YouAreNowPvPActive" : "YouAreNowPvPInactive");
                }
            }
        }

        // Update my instantaneous target for anyone assisting me.
        pClient = clientFromEnt(p->entParent);
        if(pClient!=NULL)
        {
            eTarget = validEntFromId(pClient->controls.selected_ent_server_index);

            if (p->entParent->clear_target
                || (eTarget && eTarget->pchar && !character_TargetIsPerceptible(p,eTarget->pchar)))
            {
                eTarget = NULL;
            }

            if(erGetRef(eTarget)!=p->erTargetInstantaneous)
            {
                // It's safe to erGetRef on NULL
                p->erTargetInstantaneous = erGetRef(eTarget);
                p->entParent->target_update = 1;
            }
        }

        // Fire the moved event if the character has moved.
        if(!sameVec3(ENTPOS(p->entParent), p->vecLastLoc))
        {
            character_RecordEvent(p, kPowerEvent_Moved, timerSecondsSince2000());
            copyVec3(ENTPOS(p->entParent), p->vecLastLoc);
        }

        if (e->teamup && e->teamup->probationalActivePlayerDbidExpiration
            && e->teamup->probationalActivePlayerDbidExpiration < now
            && e->teamup->probationalActivePlayerDbid == e->db_id)
        {
            if (teamLock(e, CONTAINER_TEAMUPS))
            {
                int tokenIndex, tokenCount = eaSize(&e->pl->activePlayerRewardTokens);
                RewardToken *destToken, *srcToken;

                e->teamup->activePlayerDbid = e->db_id;
                eaClearEx(&e->teamup->activePlayerRewardTokens, rewardtoken_DestroyAdapter);
                for (tokenIndex = 0; tokenIndex < tokenCount; tokenIndex++)
                {
                    srcToken = e->pl->activePlayerRewardTokens[tokenIndex];
                    destToken = rewardtoken_Copy(srcToken);
                    eaPush(&e->teamup->activePlayerRewardTokens, destToken);
                }
                
                e->teamup->activePlayerRevision++;

                e->teamup->probationalActivePlayerDbidExpiration = 0;
                e->teamup->activePlayerDbid = e->teamup->probationalActivePlayerDbid;
                // don't reset probationalActivePlayerDbid!

                teamUpdateUnlock(e, CONTAINER_TEAMUPS);
            }
        }

        if (p->entParent->teamupTimer_activePlayer 
            && p->entParent->teamupTimer_activePlayer < now)
        {
            if (p->entParent->teamup_activePlayer && p->entParent->teamup_activePlayer != p->entParent->teamup)
            {
                // This matches with the createTeamup() call in unpackTeamup().
                destroyTeamup(p->entParent->teamup_activePlayer);
            }

            p->entParent->teamup_activePlayer = p->entParent->teamup;
            p->entParent->teamupTimer_activePlayer = 0;
        }

        if (!p->entParent->teamupTimer_activePlayer
            && (!p->entParent->teamup || !p->entParent->teamup->probationalActivePlayerDbidExpiration)
            && (!p->entParent->teamup_activePlayer || !p->entParent->teamup_activePlayer->probationalActivePlayerDbidExpiration)
            && (p->entParent->teamup_activePlayer 
                ? p->entParent->teamup_activePlayer->activePlayerRevision != p->entParent->activePlayerRevision
                : p->entParent->activePlayerRevision))
        {
            character_RefreshPowersOnActivePlayerChange(p);

            if (p->entParent->teamup)
            {
                p->entParent->activePlayerRevision = p->entParent->teamup->activePlayerRevision;
            }
            else
            {
                p->entParent->activePlayerRevision = 0;
            }
        }

    PERFINFO_AUTO_STOP();

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    // Update buddy stuff
    PERFINFO_AUTO_START("BuddyTick", 1);
    Buddy_Tick(p, fRate);
    PERFINFO_AUTO_STOP();

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    // Calculate the character's effective strength when using his various
    //    powers.
    PERFINFO_AUTO_START("PowerStrength", 1);
    PowerStrength_Tick(p, fRate);
    PERFINFO_AUTO_STOP();

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    // Update the list of powers which are recharging
    PERFINFO_AUTO_START("Recharge", 1);
    Recharge_Tick(p, fRate);
    PERFINFO_AUTO_STOP();

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    // Update the power queue, potentially activating a new current power.
    PERFINFO_AUTO_START("PowerQueue", 1);
    PowerQueue_Tick(p);
    PERFINFO_AUTO_STOP();

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    // Update the current power
    // If it's ready for full activation, update modifiers.
    // If the current power is done, retire it.
    PERFINFO_AUTO_START("CurrentPower", 1);
    CurrentPower_Tick(p, fRate);
    PERFINFO_AUTO_STOP();

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    // Update the modifiers from auto powers.
    PERFINFO_AUTO_START("AutoPowers", 1);
    AutoPowers_Tick(p, fRate);
    PERFINFO_AUTO_STOP();

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    if(p->attrCur.fHitPoints>0)
    {
        // Update the modifiers from toggle powers, expiring them if out
        // of endurance.
        PERFINFO_AUTO_START("TogglePowers", 1);
        TogglePowers_Tick(p, fRate);
        PERFINFO_AUTO_STOP();

        if (VALIDATE_CHARACTER_POWER_SETS)
        {
            ValidateCharacterPowerSets(p);
        }

        // Update HP, Endurance, and Insight.
        // This function is called before character_AccrueMods, which will
        // clamp HitPoints, Endurance, and Insight properly.
        PERFINFO_AUTO_START("Regeneration", 1);
        Regeneration_Tick(p, fRate);
        PERFINFO_AUTO_STOP();

        if (VALIDATE_CHARACTER_POWER_SETS)
        {
            ValidateCharacterPowerSets(p);
        }
    }

    // Update the modifiers from global boosts.
    //    must be after auto/toggle tick since global boost timer update can make some powers never fire
    PERFINFO_AUTO_START("GlobalBoosts", 1);
    GlobalBoosts_Tick(p, fRate);
    PERFINFO_AUTO_STOP();

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    // Remove any powers which are at their usage limit.
    // Must be at end  (other _Ticks properly shuts off expired powers first).
    PERFINFO_AUTO_START("ExpirePowers", 1);
    ExpirePowers_Tick(p, fRate);
    PERFINFO_AUTO_STOP();

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }
}

/**********************************************************************func*
 * Knock
 *
 */
static void Knock(Entity *etarget, Entity *eattacker, float fHStrength, float fVStrength, bool bDoKnockbackAnim)
{
    extern int velIsZero(Vec3 vel);

    MotionState* motion = etarget->motion;
    Vec3 newVel;

    assert(etarget!=NULL);
    assert(eattacker!=NULL);

    subVec3(ENTPOS(etarget), ENTPOS(eattacker), newVel);
    normalVec3(newVel);
    if(velIsZero(newVel))
    {
        newVel[0] = (float)rand()-(float)rand();
        newVel[2] = (float)rand()-(float)rand();
        normalVec3(newVel);
    }

    newVel[0] *= fHStrength;
    newVel[2] *= fHStrength;

    newVel[1] = -fVStrength;

    if(ENTTYPE(etarget) == ENTTYPE_PLAYER
        && etarget->client)
    {
        FuturePush* fp = addFuturePush(&etarget->client->controls, SEC_TO_ABS(0));
        copyVec3(newVel, fp->add_vel);
        fp->do_knockback_anim = bDoKnockbackAnim ? 1 : 0;
    }
    else
    {
        if(motion->input.flying)
        {
            newVel[1] = 0;
        }
        else
        {
            motion->on_surf = 0;
            motion->was_on_surf = 0;
            motion->falling = 1;
        }
        copyVec3(newVel, motion->vel);
    }

    LOG_ENT( etarget, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Knocked from %s. velocity %s", dbg_NameStr(eattacker), dbg_LocationStr(motion->vel));
}

/**********************************************************************func*
 * KnockVStrengthForHeight
 *
 */
static float KnockVStrengthForHeight(float fHeight)
{
    // This gravity setting seems to work.

    static float GRAVITY = 0.08;

    // Use actual physics since the gravity works correctly now.  fStrength is taken as the height to launch to.

    return sqrt(2 * fHeight * GRAVITY);
}

/**********************************************************************func*
 * KnockBack
 *
 */
static void KnockBack(Entity *etarget, Entity *eattacker, float fStrength)
{
    // MS: Changed these to statics for easier debugging if the file is optimized.
    //     Also, I changed the values to account for the new gravity situation in the physics code.

    static float Y_MIN_HEIGHT = 3.0f;
    static float Y_STRENGTH_SCALE = 0.25f;

    static float MIN_H_VEL = 1.0f;
    static float BASE_H_VEL = 0.025f;

    assert(etarget!=NULL);
    assert(eattacker!=NULL);

    if(fStrength>=0.75f)
    {
        float fVStrength = KnockVStrengthForHeight(Y_MIN_HEIGHT + fStrength * Y_STRENGTH_SCALE);

        seqOrState(etarget->seq->state, 1, STATE_KNOCKBACK);
        Knock(etarget, eattacker,
            MIN_H_VEL + BASE_H_VEL*fStrength,
            -fVStrength,
            true);
    }
    else
    {
        seqOrState(etarget->seq->state, 1, STATE_SWEEP);
        seqOrState(etarget->seq->state, 1, STATE_LOW);
    }
}

/**********************************************************************func*
 * KnockUp
 *
 */
static void KnockUp(Entity *etarget, Entity *eattacker, float fStrength)
{
    assert(etarget!=NULL);
    assert(eattacker!=NULL);

    if(fStrength>=1.5f)
    {
        float fVStrength = KnockVStrengthForHeight(6 + fStrength);

        seqOrState(etarget->seq->state, 1, STATE_KNOCKBACK);
        Knock(etarget, eattacker,
            0.001, // This can't be 0 for some reason.
            -fVStrength,
            true);
    }
    else
    {
        seqOrState(etarget->seq->state, 1, STATE_SWEEP);
        seqOrState(etarget->seq->state, 1, STATE_HIGH);
    }
}

/**********************************************************************func*
 * Repel
 *
 */
static void Repel(Entity *etarget, Entity *eattacker, float fStrength)
{
    #define MIN_Y_VEL   0.8f
    #define BASE_Y_VEL  0.2f

    #define MIN_H_VEL   0.5f
    #define BASE_H_VEL  0.2f

    assert(etarget!=NULL);
    assert(eattacker!=NULL);

    Knock(etarget, eattacker,
        MIN_H_VEL + BASE_H_VEL*fStrength,
        0,
        0);

    #undef MIN_Y_VEL
    #undef BASE_Y_VEL
    #undef MIN_X_VEL
    #undef BASE_X_VEL
}

static int s_iDefiantMode = -1;

// if attrLast is NULL, assume that we haven't actually updated the attributes
//   right before this call.  We probably just directly altered the Character's hit points.
static void damageTrackerDestroyAdapter(void* arg0)
{
    damageTrackerDestroy((DamageTracker*)arg0);
}

void character_HandleDeathAndResurrection(Character *p, CharacterAttributes *attrLast)
{
    U32 ulNow = timerSecondsSince2000();

    if(character_IsDefiant(p))
    {
        character_SetPowerMode(p, s_iDefiantMode);

        character_RecordEvent(p, kPowerEvent_Defiant, ulNow);
    }
    else if(p->attrCur.fHitPoints<=0.0f)
    {
        character_UnsetPowerMode(p, s_iDefiantMode);

        if(entAlive(p->entParent))
        {
            int iDelay = 0;

            if(ENTTYPE(p->entParent)!=ENTTYPE_PLAYER)
            {
                iDelay = p->entParent->seq->type->ticksToLingerAfterDeath;
            }

            character_RecordEvent(p, kPowerEvent_Defeated, ulNow);

            LOG_ENT( p->entParent, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Died %s", dbg_LocationStr(p->vecLocationCurrent));
            dieNow(p->entParent, kDeathBy_HitPoints, true, iDelay);
        }

        p->bRecalcStrengths = true; // should this be inside the above entAlive block?  ARM
        p->bNoModsLastTick = false;

        // Insurance
        p->attrCur.fHitPoints = 0;
        p->attrCur.fEndurance = 0;
        p->attrCur.fInsight   = 0;
        p->attrCur.fRage      = 0;

        // All these attribmods were removed in dieNow, so record that they're
        //   over.
        if(attrLast && attrLast->fHeld > 0.0f)
            badge_RecordHeldTime(p->entParent, ulNow-p->ulTimeHeld);
        if(attrLast && attrLast->fStunned > 0.0f)
            badge_RecordStunnedTime(p->entParent, ulNow-p->ulTimeStunned);
        if(attrLast && attrLast->fImmobilized > 0.0f)
            badge_RecordImmobilizedTime(p->entParent, ulNow-p->ulTimeImmobilized);
        if(attrLast && attrLast->fSleep > 0.0f)
            badge_RecordSleepTime(p->entParent, ulNow-p->ulTimeSleep);

        // If they've been dead for two minutes in a pvp zone, force them to respawn
        if (server_visible_state.isPvP && !OnArenaMap() && !RaidIsRunning() && // This should only be zone PvP
            p->aulEventTimes[kPowerEvent_Defeated] + MAX_DEATH_TIME < ulNow &&
            p->entParent->pl) // Make sure they're a player
        {
            dbBeginGurneyXfer(p->entParent, 0);
        }
    }
    else
    {
        character_UnsetPowerMode(p, s_iDefiantMode);

        if(!entAlive(p->entParent))
        {
            LOG_ENT( p->entParent, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Entity:Resurrect");

            // Clear out the damage list.
            if(p->entParent->who_damaged_me)
            {
                eaClearEx(&p->entParent->who_damaged_me, damageTrackerDestroyAdapter);
            }

            modStateBits(p->entParent, ENT_DEAD, 0, FALSE);
            EncounterEntUpdateState(p->entParent, 0);

            // Yank the client out of whatever interim state it's in.
            // Get rid of the hospital dialog, too.
            setClientState(p->entParent, STATE_RESURRECT);

            p->bRecalcStrengths = true;
            p->bNoModsLastTick = false;

            if(OnArenaMap() && p->entParent->pl)
            {
                p->entParent->pl->arenaDeathProcessed = 0;
            }

            // ARM NOTE:  This code doesn't reset attrib mods on resurrect!  Notice that!  (Cause it took me forever to notice it...)

            // Shut off any auto powers which work only after death.
            character_ShutOffAllAutoPowers(p);

            // Trigger onDisable mods for death powers and onEnable mods for live powers.
            character_UpdateAllEnabledState(p);

            // Turn on new auto powers which work only when alive.
            character_ActivateAllAutoPowers(p);
        }
    }
}

void character_TickPhaseTwo(Character *p, float fRate)
{
    static const CharacterClass *pclassStalker = NULL;
    static const CharacterClass *pclassArachnosWidow = NULL;
    static const CharacterClass *pclassArachnosSoldier = NULL;

    float xlucency;
    bool bDisallowDefeat = false;
    bool bStunImmobilizeSleepMessage = false;
    CharacterAttributes attrLast = p->attrCur;
    float fLastRechargeStrength = p->attrStrength.fRechargeTime;
    int numDesignerStatusesLast;
    U32 ulNow = timerSecondsSince2000();

    assert(p!=NULL);
    assert(fRate>=0.0);

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    // TODO: There should be a cleaner way to do this.
    if(s_iDefiantMode<0)
    {
        extern DefineContext *g_pParsePowerDefines;
        const char *pch = DefineLookup(g_pParsePowerDefines, "kDefiant");
        if(pch)
        {
            s_iDefiantMode = atoi(pch);
        }
    }

    // TODO: There should be a cleaner way to do this too
    xlucency = p->entParent->xlucency;
    p->entParent->xlucency = -1.0f; // Must be before AccrueMods
    numDesignerStatusesLast = eaSize(&p->designerStatuses);
    eaClearEx(&p->designerStatuses, NULL);
    if (p->bitfieldCombatPhasesLastFrame != p->bitfieldCombatPhases)
    {
        p->entParent->status_effects_update = true;
    }
    p->bitfieldCombatPhasesLastFrame = p->bitfieldCombatPhases;
    p->bitfieldCombatPhases = 1; // default to be in the prime phase
    p->fMagnitudeOfBestCombatPhase = 0.0f;

    if (!p->bCombatModShiftedThisFrame)
    {
        p->iCombatModShift = 0;
        p->iCombatModShiftToSendToClient = 0;
        p->bCombatModShiftedThisFrame = false;
    }

    /*******************************************************************/

    // Don't let players get taken from max health to defeat in one tick.
    // This keeps players from being insta-killed, which I hear isn't fun.
    // Disable this if they only have one hit point max, which is needed
    // because Arenas' Sudden Death reduces Max HP to one (eventually)
    if(ENTTYPE(p->entParent)==ENTTYPE_PLAYER)
    {
        if(p->bCannotDie || (p->attrCur.fHitPoints >= p->attrMax.fHitPoints && p->attrMax.fHitPoints > 1.0f))
        {
            bDisallowDefeat = true;
        }
    }

    PERFINFO_AUTO_START("AccrueMods", 1);
    // Put together all the modifiers and calculate the final
    //   character attributes.
    character_AccrueMods(p, fRate, &bDisallowDefeat);
    PERFINFO_AUTO_STOP();

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    if(bDisallowDefeat && p->attrCur.fHitPoints<=0.0f)
    {
        p->attrCur.fHitPoints = 1.0f;
    }

    if (numDesignerStatusesLast != eaSize(&p->designerStatuses))
    {
        p->entParent->status_effects_update = true;
    }

    /*******************************************************************/

    PERFINFO_AUTO_START("UpdateEnableStatus", 1);
    if(p->bRecalcEnabledState)
    {
        character_UpdateAllEnabledState(p);
        p->bRecalcEnabledState = false;
        p->entParent->team_buff_update = true;
        p->entParent->power_modes_update = true;
    }

    PERFINFO_AUTO_STOP();

    /*******************************************************************/

    PERFINFO_AUTO_START("ConditionalFX", 1);
    // Play mods which should only be played when the accrued attrib is
    //   non-zero (aka true). This makes stun FX and Anims only show up
    //   when the entity is actually stunned.
    character_HandleConditionalFXMods(p);
    PERFINFO_AUTO_STOP();

    /*******************************************************************/

    PERFINFO_AUTO_START("Results", 1);

    // Clear out tick info.
    memcpy(p->aulPhaseOneEventCounts, p->aulEventCounts, sizeof(p->aulEventCounts));
    memset(p->aulEventCounts, 0, sizeof(p->aulEventCounts));

    // If the translucency has changed from last time, send it to the client.
    if(p->entParent->xlucency < 0)
    {
        p->entParent->xlucency = 1.0f;
    }

    if(xlucency != p->entParent->xlucency)
    {
        p->entParent->send_xlucency = 1;
    }

    /*******************************************************************/

    if(!p->entParent->unstoppable)
    {
        if(p->erKnockedFrom!=0)
        {
            Entity *eSrc = erGetEnt(p->erKnockedFrom);
            if(eSrc!=NULL)
            {
                bool bKnocked = false;

                if(p->attrCur.fKnockback > 0.0f)
                {
                    KnockBack(p->entParent, eSrc,  p->attrCur.fKnockback);
                    bKnocked = true;
                }
                else if(p->attrCur.fKnockup > 0.0f)
                {
                    KnockUp(p->entParent, eSrc,  p->attrCur.fKnockup);
                    bKnocked = true;
                }
                else if(p->attrCur.fRepel > 0.0f)
                {
                    Repel(p->entParent, eSrc,  p->attrCur.fRepel);
                    bKnocked = true;
                }

                if(bKnocked)
                {
                    seqOrState(p->entParent->seq->state, 1, STATE_HIT);

                    if(ENTTYPE(p->entParent)!=ENTTYPE_PLAYER)
                    {
                        entity_FaceTarget(p->entParent, eSrc);
                    }
                    eSrc->pchar->stats.iCntKnockbacks++;
                    p->erKnockedFrom = 0;

                    character_RecordEvent(p, kPowerEvent_Knocked, ulNow);
                }
            }

        }
    }

    /*******************************************************************/
    if( p->entParent && p->entParent->erOwner && p->entParent->pchar && p->entParent->pchar->ppowCreatedMe ) // am I a pet created by a power?
    {
        Entity *parent = erGetEnt(p->entParent->erOwner);

        // if my parent's last mentored time has changed since I was created, and they're flagged to kill pets, kill myself
        if( parent    && parent->pchar->buddy.uTimeLastMentored > p->buddy.uTimeLastMentored )
            p->attrCur.fHitPoints = 0.0f;  // safe way to die, handled by following code
    }

    character_HandleDeathAndResurrection(p, &attrLast);

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    if(!p->entParent->unstoppable)
    {
        /*******************************************************************/

        if(p->attrCur.fHeld > 0.0f)
        {
            character_AddStickyState(p, STATE_STUN);
            character_RecordEvent(p, kPowerEvent_Held, ulNow);

            if(attrLast.fHeld <= 0.0f)
            {
                seqOrState(p->entParent->seq->state, 1, STATE_HIT);
                sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreHeld");
                LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Held on");
                bStunImmobilizeSleepMessage = true;
                p->entParent->status_effects_update = true;

                character_ShutOffTogglePowers_Hold(p);
                character_KillCurrentPower(p);
                character_KillQueuedPower(p);
                character_KillRetryPower(p);

                p->ulTimeHeld = dbSecondsSince2000();
            }
        }
        else
        {
            if(attrLast.fHeld > 0.0f)
            {
                sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreNotHeld");
                LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Held off");
                bStunImmobilizeSleepMessage = true;
                p->entParent->status_effects_update = true;

                badge_RecordHeldTime(p->entParent, dbSecondsSince2000()-p->ulTimeHeld);
            }
        }

        /*******************************************************************/

        if(p->attrCur.fImmobilized > 0.0f)
        {
            character_RecordEvent(p, kPowerEvent_Immobilized, ulNow);

            if(attrLast.fImmobilized <= 0.0f)
            {
                p->entParent->status_effects_update = true;

                if(!bStunImmobilizeSleepMessage)
                {
                    // Different last time, send a message
                    sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreImmobilized");
                    LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Immobilized on");
                }

                p->ulTimeImmobilized = dbSecondsSince2000();
            }
        }
        else
        {
            if(attrLast.fImmobilized > 0.0f)
            {
                p->entParent->status_effects_update = true;

                if(!bStunImmobilizeSleepMessage)
                {
                    // Different last time, send a message
                    sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreNotImmobilized");
                    LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Immobilized off");
                }

                badge_RecordImmobilizedTime(p->entParent, dbSecondsSince2000()-p->ulTimeImmobilized);
            }
        }

        /*******************************************************************/

        if(p->attrCur.fStunned > 0.0f)
        {
            character_AddStickyState(p, STATE_STUN);
            character_RecordEvent(p, kPowerEvent_Stunned, ulNow);

            if(attrLast.fStunned <= 0.0f)
            {
                setStunned(p->entParent, true);
                seqOrState(p->entParent->seq->state, 1, STATE_HIT);
                p->entParent->status_effects_update = true;

                if(!bStunImmobilizeSleepMessage)
                {
                    // Different last time, send a message
                    sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouCannotUsePowers");
                    LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Stunned on");
                }

                character_ShutOffTogglePowers_Stun(p);
                character_KillCurrentPower(p);
                character_KillQueuedPower(p);
                character_KillRetryPower(p);

                p->ulTimeStunned = dbSecondsSince2000();
            }
        }
        else
        {
            if(attrLast.fStunned > 0.0f)
            {
                setStunned(p->entParent, false);
                p->entParent->status_effects_update = true;

                if(!bStunImmobilizeSleepMessage)
                {
                    // Different last time, send a message
                    sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouCanUsePowers");
                    LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Stunned off");
                }

                badge_RecordStunnedTime(p->entParent, dbSecondsSince2000()-p->ulTimeStunned);
            }
        }

        /*******************************************************************/

        if(p->attrCur.fSleep > 0.0f)
        {
            character_AddStickyState(p, STATE_STUN);
            character_RecordEvent(p, kPowerEvent_Sleep, ulNow);

            if(attrLast.fSleep <= 0.0f)
            {
                // Different last time, send a message
                seqOrState(p->entParent->seq->state, 1, STATE_HIT);
                sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreAsleep");
                p->entParent->status_effects_update = true;

                LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Sleep on");

                character_ShutOffTogglePowers_Sleep(p);
                character_KillCurrentPower(p);
                character_KillQueuedPower(p);
                character_KillRetryPower(p);

                p->ulTimeSleep = dbSecondsSince2000();
            }
        }
        else
        {
            if(attrLast.fSleep > 0.0f)
            {
                // Different last time, send a message
                sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreNotAsleep");
                LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Sleep off");
                p->entParent->status_effects_update = true;

                badge_RecordSleepTime(p->entParent, dbSecondsSince2000()-p->ulTimeSleep);
            }
        }

        /*******************************************************************/
        if(p->attrCur.fTerrorized > 0.0f)
        {
            if (ENTTYPE(p->entParent) == ENTTYPE_PLAYER
                && p->fTerrorizePowerTimer >= 0.0)
            {
                character_AddStickyState(p, STATE_FEAR);
            }

            character_RecordEvent(p, kPowerEvent_Terrorized, ulNow);

            if(attrLast.fTerrorized <= 0.0f)
            {
                // Different last time, send a message
                seqOrState(p->entParent->seq->state, 1, STATE_HIT);
                sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreTerrorized");
                p->entParent->status_effects_update = true;

                LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Terrorized on");

                character_KillCurrentPower(p);
                character_KillQueuedPower(p);
                character_KillRetryPower(p);
            }
        }
        else
        {
            if(attrLast.fTerrorized > 0.0f)
            {
                // Different last time, send a message
                sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreNotTerrorized");
                LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Terrorized off");
                p->entParent->status_effects_update = true;
            }
        }

        /*******************************************************************/

        if(p->attrCur.fConfused > 0.0f)
        {
            character_RecordEvent(p, kPowerEvent_Confused, ulNow);

            if(attrLast.fConfused <= 0.0f)
            {
                // Throw away their target
                p->erTargetInstantaneous = erGetRef(NULL);
                p->entParent->target_update = 1;
                p->entParent->clear_target = 1;

                // Different last time, send a message
                sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreConfused");
                LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0,"Confused on");
                p->entParent->status_effects_update = true;
            }
        }
        else
        {
            if(attrLast.fConfused > 0.0f)
            {
                // Different last time, send a message
                sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreNotConfused");
                LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Confused off");
                p->entParent->status_effects_update = true;
            }
        }

    }
    else
    {
        p->attrCur.fKnockback = 0;
        p->attrCur.fKnockup = 0;
        p->attrCur.fRepel = 0;
        p->attrCur.fConfused = 0;
        p->attrCur.fTerrorized = 0;
        p->attrCur.fSleep = 0;
        p->attrCur.fStunned = 0;
        p->attrCur.fImmobilized = 0;
        p->attrCur.fHeld = 0;
    }

    /*******************************************************************/

    if(p->attrCur.fFly > 0.0f)
    {
        if(attrLast.fFly <= 0.0f)
        {
            // Different last time, send a message
            sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreFlying");
            setFlying(p->entParent, true);
            LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Fly on");
        }
    }
    else
    {
        if(attrLast.fFly > 0.0f)
        {
            // Different last time, send a message
            sendInfoBox(p->entParent, INFO_COMBAT_SPAM, "YouAreNotFlying");
            setFlying(p->entParent, false);
            LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Fly off");
        }
    }

    /*******************************************************************/

    if(!pclassStalker)
    {
        pclassStalker = classes_GetPtrFromName(&g_CharacterClasses, "Class_Stalker");
        pclassArachnosWidow = classes_GetPtrFromName(&g_CharacterClasses,"Class_Arachnos_Widow");
        pclassArachnosSoldier = classes_GetPtrFromName(&g_CharacterClasses,"Class_Arachnos_Soldier");
    }
    if(p->pclass == pclassStalker || p->pclass == pclassArachnosWidow || p->pclass == pclassArachnosSoldier )
    {
        if(p->attrCur.fMeter >= 1.0f)
        {
            if(attrLast.fMeter < 1.0f)
            {
                p->entParent->status_effects_update = true;
            }
        }
        else
        {
            if(attrLast.fMeter >= 1.0f)
            {
                    p->entParent->status_effects_update = true;
            }
        }
    }

    /*******************************************************************/

    if(p->attrCur.fOnlyAffectsSelf > 0.0f)
    {
        character_RecordEvent(p, kPowerEvent_OnlyAffectsSelf, ulNow);
        if(attrLast.fOnlyAffectsSelf <= 0.0f)
        {
            character_MakePetsUnaggressive(p);
            p->entParent->status_effects_update = true;
        }
    }
    else
    {
        if(attrLast.fOnlyAffectsSelf > 0.0f)
        {
            p->entParent->status_effects_update = true;
        }
    }

    /*******************************************************************/

    if(p->attrCur.fUntouchable > 0.0f)
    {
        character_RecordEvent(p, kPowerEvent_Untouchable, ulNow);
    }

    /*******************************************************************/

    if(p->attrCur.fJumppack > 0.0f)
    {
        if(attrLast.fJumppack <= 0.0f)
        {
            setJumppack(p->entParent, true);
            LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Jumppack on");
        }
    }
    else
    {
        if(attrLast.fJumppack > 0.0f)
        {
            // Different last time, send a message
            setJumppack(p->entParent, false);
            LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Jumppack off");
        }
    }

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    /*******************************************************************/
    if(p->attrCur.fTeleport > 0.0f)
    {
        if (p->pchTeleportDest)
        {
            character_NamedTeleport(p, p->pchTeleportPower, p->pchTeleportDest);
        } else {
            character_Teleport(p, p->vecTeleportDest);
        }
    }

    if (p->vecLocationForceMove[0])
    {
        if (ENTTYPE(p->entParent) == ENTTYPE_PLAYER)
        {
            START_PACKET(pak, p->entParent, SERVER_START_FORCED_MOVEMENT);
            pktSendF32(pak, p->vecLocationForceMove[0]);
            pktSendF32(pak, p->vecLocationForceMove[1]);
            pktSendF32(pak, p->vecLocationForceMove[2]);
            END_PACKET
        }
        else
        {
            char *behaviorString = NULL;
            estrPrintf(&behaviorString, "MoveToPos(pos(%f,%f,%f))", p->vecLocationForceMove[0],
                            p->vecLocationForceMove[1], p->vecLocationForceMove[2]);
            aiBehaviorAddStringEx(p->entParent, ENTAI(p->entParent)->base, behaviorString, true);
            estrDestroy(&behaviorString);
        }

        zeroVec3(p->vecLocationForceMove);
    }

    /*******************************************************************/

    if(p->attrStrength.fRechargeTime != fLastRechargeStrength)
    {
        PowerListItem *ppowListItem;
        PowerListIter iter;

        // Update the powers' recharge times
        PowerStrength_Tick(p, fRate);

        // Recharge powers recently used.
        for(ppowListItem = powlist_GetFirst(&p->listRechargingPowers, &iter);
            ppowListItem != NULL;
            ppowListItem = powlist_GetNext(&iter))
        {
            power_ChangeRechargeTimer(ppowListItem->ppow, ppowListItem->ppow->fTimer);
        }
    }

    /*******************************************************************/

    setEntCollision(p->entParent, p->attrCur.fIntangible>0.0f ? true : false);
    if(p->attrCur.fIntangible>0)
    {
        character_RecordEvent(p, kPowerEvent_Intangible, ulNow);
        if(attrLast.fIntangible<=0)
        {
            LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Intangible on");
        }
    }
    if(p->attrCur.fIntangible<=0 && attrLast.fIntangible>0)
    {
        LOG_ENT( p->entParent, LOG_POWERS, LOG_LEVEL_VERBOSE, 0, "Intangible off");
    }

    /*******************************************************************/

    // Set the server control state variables when they change.

    if(p->attrCur.fMovementControl!=attrLast.fMovementControl
        || p->attrCur.fMovementFriction!=attrLast.fMovementFriction)
    {
        setSurfaceModifiers(p->entParent, SURFPARAM_AIR,
            p->attrCur.fMovementControl, p->attrCur.fMovementFriction, 0.0f, 1.0f);
    }

    setJumpHeight(p->entParent, p->attrCur.fJumpHeight);

    /*******************************************************************/

    if(p->attrCur.fFly > 0)
    {
        setSpeed(p->entParent, SURFPARAM_AIR, p->attrCur.fSpeedFlying);
    }
    else
    {
        setSpeed(p->entParent, SURFPARAM_AIR, p->attrCur.fSpeedJumping);
    }
    setSpeed(p->entParent, SURFPARAM_GROUND, p->attrCur.fSpeedRunning);


    /*******************************************************************/

    if (p->entParent->idDetail)
    {
        if (roomDetailLookUnpowered(baseGetDetail(&g_base,p->entParent->idDetail,NULL)))
            character_AddStickyState(p, STATE_OFF);
    }

    if( attrLast.fPerceptionRadius != p->attrCur.fPerceptionRadius )
        p->entParent->status_effects_update = 1;

    // OK, set all the sticky states we put together.
    character_SetStickyStates(p);

    /*******************************************************************/

    // Only players have costume constraints from power choices.
    if( p->bJustBoughtPower && p->entParent && p->entParent->pl )
    {
        costume_AdjustAllCostumesForPowers( p->entParent );
        p->bJustBoughtPower = false;
    }

    if (VALIDATE_CHARACTER_POWER_SETS)
    {
        ValidateCharacterPowerSets(p);
    }

    PERFINFO_AUTO_STOP();
}

/**********************************************************************func*
 * character_OffTick
 *
 */
void character_OffTick(Character *p, float fRate)
{
    character_SetStickyStates(p);
}

/* End of File */
