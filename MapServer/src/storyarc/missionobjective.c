/*\
 *
 *    missionobjective.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    - handles creating, interacting with, destroying mission items
 *    - functions for handling objective info structs - objective infos
 *      are placed on mission items and hostage ents that are objectives
 *
 */

#include "mission.h"
#include "storyarcprivate.h"
#include "dbcomm/dbcomm.h"
#include "player/pl_stats.h"
#include "Reward.h"
#include "player/clientEntityLink.h"
#include "comm_game.h"
#include "ai/entai.h"
#include "gameSys/dooranim.h"            // for SERVER_INTERACT_DISTANCE
#include "gameComm/NpcServer.h"
#include "TeamReward.h"
#include "entity/character_level.h"
#include "gameComm/svr_chat.h"
#include "entity/entGameActions.h"
#include "entity/netfx.h"
#include "entity/entity.h"
#include "gameSys/sgraid.h"
#include "script/scriptengine.h"
#include "entity/teamCommon.h"
#include "dbcomm/dbdoor.h"
#include "svr/svr_player.h"
#include "player/badges_server.h"
#include "entity/character_eval.h"
#include "zowie.h"
#include "storyarc/pnpcCommon.h"
#include "entity/character_animfx.h"

int g_playersleftmap = 0;
char g_overridestatus[OVERRIDE_STATUS_LEN];        // will override the normal objective string if set

// *********************************************************************************
//  Mission Objective Infos - put on anything that is logically an objective
// *********************************************************************************

MissionObjectiveInfo** objectiveInfoList;        // All known mission objective infos
int g_bSendItemLocations = 0;

MissionObjectiveInfo* MissionObjectiveInfoCreate()
{
    MissionObjectiveInfo* obj = calloc(1, sizeof(MissionObjectiveInfo));
    obj->forceFieldTimer = 1; // first spawn requested immediately
    return obj;
}

void MissionObjectiveInfoDestroy(MissionObjectiveInfo* info)
{
    // clean up non-parser attributes
    if (info->name)
        free(info->name);
    if (info->description)
        free(info->description);
    if (info->singulardescription)
        free(info->singulardescription);

    free(info);
}

// get a logical number for this objective within the active mission
//  - either the index in the list of objectives, or a numbered objective
//    from one of the spawndefs
int MissionObjectiveFindIndex(const MissionDef* mission, const MissionObjective* objective)
{
    int i, j, n;

    n = eaSize(&mission->objectives);
    for (i = 0; i < n; i++)
    {
        if (mission->objectives[i] == objective)
            return i;
    }
    // i is size of mission->objectives now
    n = eaSize(&mission->spawndefs);
    for (j = 0; j < n; j++)
    {
        const SpawnDef* spawn = mission->spawndefs[j];
        if (!spawn->objective) continue;
        if (spawn->objective[0] == objective)
            return i;
        i++;
    }
    return -1;
}

const MissionObjective* MissionObjectiveFromIndex(const MissionDef* mission, int index)
{
    int i, n;

    if (index < 0) 
        return NULL; 
    if (!mission) 
        return NULL;
    if (index < eaSize(&mission->objectives))
    {
        return mission->objectives[index];
    }
    else 
    {
        index -= eaSize(&mission->objectives);
        n = eaSize(&mission->spawndefs);
        for (i = 0; i < n; i++)
        {
            const SpawnDef* spawn = mission->spawndefs[i];
            if (!spawn->objective) 
                continue;
            if (index == 0)
                return spawn->objective[0];
            else
                index--;
        }
    }
    return NULL;
}

// Returns 1 if the objective is a side objective
int MissionObjectiveIsSideMission(const MissionObjective* objective)
{
    int i, n = eaSize(&g_activemission->def->sideMissions);

    // No group name means not part of a side mission
    if (!objective->groupName)
        return 0;

    // Otherwise make sure its in the list of potential side missions
    for (i = 0; i < n; i++)
        if (!stricmp(objective->groupName, g_activemission->def->sideMissions[i]))
            return 1;

    // It must not be a side mission
    return 0;
}

static int* selectedObjectives = 0;

void MissionObjectiveResetSideMissions(void)
{
    eaiDestroy(&selectedObjectives);
}

static void MissionObjectiveChooseSideMissions(void)
{
    // Initialize the selected objectives array
    if (g_activemission->def->numSideMissions && eaSize(&g_activemission->def->sideMissions) && !selectedObjectives)
    {
        int i, count = eaSize(&g_activemission->def->sideMissions);
        int numObjectives = MIN(count, g_activemission->def->numSideMissions);
        int* possibleValues = 0;

        // Create an array with the possible random objectives
        for (i = 0; i < count; i++)
            eaiPush(&possibleValues, i);

        // Now randomly choose numObjective objectives
        saSeed(g_activemission->seed);
        for (i = 0; i < numObjectives; i++)
        {
            int valIndex = saRoll(eaiSize(&possibleValues));
            eaiPush(&selectedObjectives, possibleValues[valIndex]);
            eaiRemove(&possibleValues, valIndex);
        }
        eaiDestroy(&possibleValues);

        // Remove any that have already been completed
        for (i = eaiSize(&selectedObjectives) - 1; i >= 0; i--)
        {
            int bitMask = (1 << selectedObjectives[i]);
            if (g_activemission->completeSideObjectives & bitMask)
                eaiRemove(&selectedObjectives, i);
        }
    }
}

int MissionObjectiveIsEnabled(const MissionObjective* objective)
{
    // Make sure we have our side missions selected first
    MissionObjectiveChooseSideMissions();

    // If its a side mission, see if its enabled
    if (MissionObjectiveIsSideMission(objective))
    {
        int i, n = eaiSize(&selectedObjectives);
    
        for (i = 0; i < n; i++)
            if (!stricmp(objective->groupName, g_activemission->def->sideMissions[selectedObjectives[i]]))
                return 1;
        return 0;
    }
    return 1;
}

void MissionObjectiveAttachEncounter(const MissionObjective* objective, EncounterGroup* group, const SpawnDef* spawndef)
{
    MissionObjectiveInfo* info;

    info = MissionObjectiveInfoCreate();
    info->def = objective;
    info->name = strdup(objective->name ? objective->name : "");
    assert( info->name );
    MissionItemSetState(info, MOS_WAITING_INITIALIZATION);
    info->encounter = group;
    group->objectiveInfo = info;
    info->optional = MissionObjectiveIsSideMission(objective);
    eaPush(&objectiveInfoList, info);

    // Delay creating a waypoint if the target is activated on another objective complete
    if (!spawndef->activateOnObjectiveComplete && !spawndef->createOnObjectiveComplete)
        MissionWaypointCreate(info);

    MissionRefreshAllTeamupStatus();
}

// when any objective info completes
void MissionObjectiveComplete(Entity* player, MissionObjectiveInfo* info, int success)
{
    Entity* objectiveEnt = erGetEnt(info->objectiveEntity); // may be NULL
    int wasCompleted;
    int grantReward = 1;

    wasCompleted = info->state >= MOS_SUCCEEDED;    // was this objective succeeded or failed already?
    MissionItemSetState(info, success? MOS_SUCCEEDED: MOS_FAILED);

    if (info->zowie)
    {
        ZowieComplete(player, info);
        MissionItemSetState(info, MOS_INITIALIZED);
    }
    else if (info->script)
    {
        ScriptGlowieComplete(player, info, success && !wasCompleted);
    }
    else // normal mission objective
    {
        MissionCountAllPlayers(5); // everyone gets five ticks for objective completion

        // check and see if this is a logged objective
        if (success && !wasCompleted && player && player->db_id > 0 && player->pl && g_activemission->def->race && !stricmp(info->name, "RaceObjective"))
        {
            //If this is a race, record if this is the fastest you've run it.
            int secondsTaken = g_activemission->task->timeoutMins * 60 - (g_activemission->timeout - dbSecondsSince2000());
            stat_AddRaceTime(player->pl, secondsTaken, g_activemission->def->race);
        }

        // make a note of objective completion
        if (info->def && info->def->reward && !wasCompleted)
        {
            MissionObjectiveInfoSave(info->def, success); // all objectives succeed right now
        }

        // Check to see if we should only reward once the set is complete
        if (info->def && info->def->name && info->def->flags & MISSIONOBJECTIVE_REWARDONCOMPLETESET)
            grantReward = MissionObjectiveIsComplete(info->def->name, success);

        // MAKTODO - put the following in storyReward.c:
        if (success && !wasCompleted && grantReward && info->def && info->def->reward && player && player->db_id > 0) // can't issue rewards if we can't figure out the player
        {
            // issue clues to mission owner, or interpret them as key clues
            int i, taskIndex;
            char command[400];
            StoryReward* reward = info->def->reward[0];
            RewardAccumulator* accumulator = rewardGetAccumulator();
            for (i = eaSize(&reward->addclues)-1; i >= 0; i--)
            {
                if (!MissionAddRemoveKeyClue(player, reward->addclues[i], 1))
                {
                    sprintf(command, "clueaddremoveinternal 1 %%i %i %i %s", 
                        g_activemission->sahandle.context, g_activemission->sahandle.subhandle, reward->addclues[i]);
                    MissionSendToOwner(command);
                }
            }
            for (i = eaSize(&reward->removeclues)-1; i >= 0; i--)
            {
                if (!MissionAddRemoveKeyClue(player, reward->removeclues[i], 0))
                {
                    sprintf(command, "clueaddremoveinternal 0 %%i %i %i %s", 
                        g_activemission->sahandle.context, g_activemission->sahandle.subhandle, reward->removeclues[i]);
                    MissionSendToOwner(command);
                }
            }

            StoryRewardHandleTokens(player, reward->addTokens, reward->addTokensToAll, reward->removeTokens, reward->removeTokensFromAll);

            if (player->storyInfo)
            {
                for (i = eaSize(&reward->abandonContacts) - 1; i >= 0; i--)
                {
                    for (taskIndex = eaSize(&player->storyInfo->tasks) - 1; taskIndex >= 0; taskIndex--)
                    {
                        StoryContactInfo *contactInfo = TaskGetContact(player->storyInfo, player->storyInfo->tasks[taskIndex]);
                        if (contactInfo && contactInfo->contact)
                        {
                            char *contactName = strrchr(contactInfo->contact->def->filename, '/');
                            if (contactName)
                            {
                                contactName++;
                                if (!strnicmp(contactName, reward->abandonContacts[i], strlen(reward->abandonContacts[i])))
                                {
                                    TaskAbandon(player, player->storyInfo, contactInfo, player->storyInfo->tasks[taskIndex], 0);
                                }
                            }
                        }
                    }
                }
            }

            StoryRewardHandleSounds(player, reward);

            if (reward->rewardPlayFXOnPlayer && player)
            {
                character_PlayFX(player->pchar, player->pchar, reward->rewardPlayFXOnPlayer, colorPairNone, 0, 0, PLAYFX_NO_TINT);
            }

            // give the dialog to the person who finished the objective
            if (reward->rewardDialog)
            {
                ScriptVarsTablePushVarEx(&g_activemission->vars, "Rescuer", player, 'E', 0);
                sendInfoBox(player, INFO_SVR_COM, "%s", saUtilTableAndEntityLocalize(reward->rewardDialog, &g_activemission->vars, player));
                ScriptVarsTablePopVar(&g_activemission->vars);
            }

            // give the fame string to the person who finished the objective
            if (reward->famestring)
            {
                ScriptVarsTablePushVarEx(&g_activemission->vars, "Rescuer", player, 'E', 0);
                RandomFameAdd(player, saUtilTableAndEntityLocalize(reward->famestring, &g_activemission->vars, player));
                ScriptVarsTablePopVar(&g_activemission->vars);
            }

            // badge stat
            if (reward->badgeStat)
                badge_StatAdd(player, reward->badgeStat, 1);

            // architect badges
            if( g_ArchitectTaskForce )
            {
                if( reward->architectBadgeStatTest && (g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST) )
                    badge_StatAddArchitectAllowed(player, reward->architectBadgeStatTest, 1, 0);
                if(reward->architectBadgeStat && !(g_ArchitectTaskForce->architect_flags&ARCHITECT_TEST) )
                    badge_StatAddArchitectAllowed(player, reward->architectBadgeStat, 1, 0);
            }
            
            // bonus time
            if (reward->bonusTime)
                StoryRewardBonusTime(reward->bonusTime);

            // float text
            if (reward->floatText)
                StoryRewardFloatText(player, saUtilTableAndEntityLocalize(reward->floatText, &g_activemission->vars, player));

            // have reward code apply the experience, items
            rewardSetsAward(reward->rewardSets, accumulator, player, -1, -1, NULL, 0, NULL);
            rewardApplyLogWrapper(accumulator, player, true, !villainWillGiveItems(g_activemission->missionLevel, character_GetCombatLevelExtern(player->pchar)), REWARDSOURCE_MISSION);

            // Give out table-driven primary rewards to person who did objective
            if(eaSize(&reward->primaryReward))
            {
                rewardStoryApplyToDbID(player->db_id, reward->primaryReward, g_activemission->missionGroup,
                                            g_activemission->baseLevel, MissionGetLevelAdjust(), &g_activemission->vars, REWARDSOURCE_MISSION);
            }

            // Give out table-driven rewards to teammates
            if(eaSize(&reward->secondaryReward) && player->teamup)
            {
                // Loop over teammates
                for(i=0; i<player->teamup->members.count; i++)
                {
                    // If the teammate isn't the primary task person, award
                    // the secondary reward
                    if(player->teamup->members.ids[i]!=player->db_id)
                    {
                        rewardStoryApplyToDbID(player->teamup->members.ids[i], 
                            reward->secondaryReward, g_activemission->missionGroup, 
                            g_activemission->baseLevel, MissionGetLevelAdjust(), &g_activemission->vars, REWARDSOURCE_MISSION);
                    }
                }
            }

            // If there is a primary or secondary reward, we mark the side objective to no longer be issued
            if (info->def && info->def->groupName && (eaSize(&reward->primaryReward) || eaSize(&reward->secondaryReward)))
            {
                if (g_activemission && g_activemission->def && eaSize(&g_activemission->def->sideMissions))
                {
                    int sideMissionIndex, numSideMissions = eaSize(&g_activemission->def->sideMissions);
                    Entity* owner = entFromDbId(g_activemission->ownerDbid);
                    if (owner)
                    {
                        StoryTaskInfo* taskToUpdate = TaskGetInfo(owner, &g_activemission->sahandle);
                        if (taskToUpdate)
                        {
                            for (sideMissionIndex = 0; sideMissionIndex < numSideMissions; sideMissionIndex++)
                                if (!stricmp(g_activemission->def->sideMissions[sideMissionIndex], info->def->groupName))
                                    taskToUpdate->completeSideObjectives |= (1 << sideMissionIndex);
                        }
                    }
                }
            }
        }
        MissionRefreshAllTeamupStatus();
    } // normal mission objective


    // Assuming that an objective can only be completed by interacting with it
    // Perform interact outcome when the objective is complete.
    if (info->def && info->def->locationtype != LOCATION_PERSON && info->def->interactOutcome == MIO_REMOVE)
    {
        if (objectiveEnt) 
            entFree(objectiveEnt);
    }

    //MW go through all the mission encounters and seet if any are triggered or set active by 
    //when this objective is met.
    EncounterNotifyInterestedEncountersThatObjectiveIsComplete( info->name, player );
    RecheckActivateRequires();
    MissionWaypointDestroy(info);
    KeyDoorUpdate(info);

    MissionRefreshAllTeamupStatus();
    MissionObjectiveCheckThreshold();
}

// *********************************************************************************
//  Mission Objective debugging
// *********************************************************************************

static char* MissionObjectiveGetStatus(MissionObjectiveInfo* info)
{
    if(!info)
        return "invalid";

    switch(info->state)
    {
    case MOS_NONE:
        return "uninitialized";
    case MOS_WAITING_INITIALIZATION:
        return "waiting for encounter spawn";
    case MOS_INITIALIZED:
        return "initialized";
    case MOS_INTERACTING:
        return "interacting";
    case MOS_SUCCEEDED:
    case MOS_FAILED:
        break; // more details follow
    default:
        return "invalid";
    }

    if (info->succeeded && info->failed)
        return "succeeded and failed";
    else if (info->succeeded)
        return "succeeded";
    else if (info->failed)
        return "failed";
    else
        return "invalid (2)";
}

void MissionPrintObjectives(ClientLink* client)
{
    int i;
    for(i = 0; i < eaSize(&objectiveInfoList); i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        conPrintf(client, "%i %s [%s]", i, info->name, MissionObjectiveGetStatus(info));
    }
}

int MissionMoveEntToObjective(MissionObjectiveInfo* info, Entity* player)
{
    if(!info || !player)
        return 0;

    if(info->objectiveEntity)
    {
        Entity* obj = erGetEnt(info->objectiveEntity);
        if(obj && player)
        {
            entUpdatePosInstantaneous(player, ENTPOS(obj));
            return 1;
        }
    }
    else if(info->encounter)
    {
        // If the encounter has already been spawned, move to any entity in the list.
        if(info->encounter->active && eaSize(&info->encounter->active->ents))
        {
            entUpdatePosInstantaneous(player, ENTPOS(info->encounter->active->ents[0]));
            return 1;
        }
        else
        {
            // If the encounter has not been spawned yet, or if there are no entities left,
            // move to where the encounter group is.
            entUpdatePosInstantaneous(player, info->encounter->mat[3]);
            return 1;
        }
    }

    return 0;
}

int MissionGotoObjective(ClientLink* client, int i)
{
    MissionObjectiveInfo* info;
    Entity* player = clientGetControlledEntity(client);

    if(!eaSize(&objectiveInfoList))
    {
        conPrintf(client, saUtilLocalize(player,"NoObjective"));
        return 0;
    }

    if(i >= eaSize(&objectiveInfoList))
    {
        conPrintf(client, saUtilLocalize(player,"InvalidObjective", eaSize(&objectiveInfoList)));
        return 0;
    }

    info = objectiveInfoList[i];
    if(MissionMoveEntToObjective(info, player))
    {
        conPrintf(client, saUtilLocalize(player,"GoingToObjective", i));
    }
    else
    {
        conPrintf(client, saUtilLocalize(player,"CantGoToObjective", i));
    }
    return 1;
}

int MissionNumObjectives(void)
{
    return eaSize(&objectiveInfoList);
}

void MissionGotoNextObjective(ClientLink* client)
{
}

// *********************************************************************************
//  Mission Items
// *********************************************************************************

static ScriptVarsTable* g_itemvars = 0;

// mission items can be used by either missions or supergroup raids, and need
// a different vars table for each
void MissionItemSetVarsTable(ScriptVarsTable* vars)
{
    g_itemvars = vars;
}

// returns NULL if no effect required
static char* MissionItemEffect(MissionObjectiveInfo* info, MissionObjectiveState state)
{
    char* result = NULL;

    // bail out if this is just a dummy objective
    if (!info->def)
        return NULL;

    switch (state)
    {
    case MOS_INITIALIZED:
        result = info->def->effectInactive? info->def->effectInactive: MISSION_ITEM_FX;
        break;
    case MOS_REQUIRES:
        result = info->def->effectRequires;
        break;
    case MOS_INTERACTING:
        result = info->def->effectActive;
        break;
    case MOS_COOLDOWN:        
        result = info->def->effectCooldown;    // only for simultaneous objectives
        break;
    case MOS_SUCCEEDED:
        result = info->def->effectCompletion;
        break;
    case MOS_FAILED:
        result = info->def->effectFailure;    // not actually needed yet..
        break;
    }
    if (result && (!result[0] || stricmp(result, "none")==0))
        result = NULL;
    return result;
}

// sets state, changes effects
void MissionItemSetState(MissionObjectiveInfo* info, MissionObjectiveState newstate)
{
    Entity* e;
    char *fromEffect, *toEffect;
    MissionObjectiveState prev = info->state;
    info->state = newstate;

    if (newstate == MOS_SUCCEEDED)
        info->succeeded = 1;
    if (newstate == MOS_FAILED)
        info->failed = 1;

    // dummy objective
    if (!info->def)
        return;

    if (info->def->locationtype == LOCATION_PERSON) // don't care
        return;

    // what are my from and to effects?
    fromEffect = MissionItemEffect(info, prev);
    toEffect = MissionItemEffect(info, info->state);

    // change them if necessary
    if (fromEffect == toEffect || (fromEffect && toEffect && stricmp(fromEffect, toEffect)==0)) 
        return;
    e = erGetEnt(info->objectiveEntity);
    if (!e) 
        return;
    if (fromEffect) 
        entDelFxByName(e, fromEffect);
    if (toEffect) 
        entAddFxByName(e, toEffect, CREATE_MAINTAINED_FX);
}

// send a message to client to start the interaction timer - 0.0 will stop timer
void MissionItemSetClientTimer(MissionObjectiveInfo* info, float expireTime)
{
    Entity* player;

    // safty check 
    if (!info->def)
        return;

    info->interactTimer = expireTime;

    player = erGetEnt(info->interactPlayerRef);
    if(player)
    {
        START_PACKET(pak, player, SERVER_MISSION_OBJECTIVE_TIMER_UPDATE);
        pktSendString(pak, saUtilTableAndEntityLocalize(info->def->interactActionString, g_itemvars, player));
        pktSendF32(pak, info->interactTimer);
        END_PACKET;
    }
}

// sets up a mission item to start interacting with player, global hooks
// will monitor interaction until it is complete or interrupted
void MissionItemInteract(Entity* objectiveEntity, Entity* interactPlayer)
{
    Entity* ff;
    MissionObjectiveInfo* info;
 
    if(!objectiveEntity || !objectiveEntity->missionObjective || !interactPlayer || ENTTYPE(interactPlayer) != ENTTYPE_PLAYER)
        return;

    info = objectiveEntity->missionObjective;
    if (!info || !info->def || info->state == MOS_SUCCEEDED || info->state == MOS_FAILED) return;

    if( !closeEnoughToInteract( interactPlayer, objectiveEntity, SERVER_INTERACT_DISTANCE ) )
    {
        // Tell the player he is too far away from the target.
        sendInfoBox(interactPlayer, INFO_USER_ERROR, "YoureTooFarToInteract");
        return;
    }

    // we are explicitly allowing people in different exclusive phases to interact!
    if (interactPlayer->pchar && (interactPlayer->pchar->attrCur.fUntouchable > 0.0f
                                 || !(interactPlayer->pchar->bitfieldCombatPhases & 1)
                                 || !(interactPlayer->bitfieldVisionPhases & 1)))
    {
        // Tell the player he is not allowed to interact in this state
        sendInfoBox(interactPlayer, INFO_USER_ERROR, "YouCantInteractInStealthOrUntouchable");
        return;
    }

    // Only one person can interact with an objective at a time.
    // If the objective is already being interacted with, subsequent interaction requests will be rejected.
    // Inform the user that someone is already interacting with the objective.
    if(info->interactPlayerDBID)
    {
        Entity* interactingPlayer = erGetEnt(info->interactPlayerRef);

        if(interactingPlayer)
        {
            if(interactingPlayer == interactPlayer)
            {
                // Todo - add this message
                sendInfoBox(interactPlayer, INFO_USER_ERROR, "YoureAlreadyInteractingWithObjective");
            }
            else
            {
                sendInfoBox(interactPlayer, INFO_USER_ERROR, "PlayerAlreadyInteractingWithObjective", interactingPlayer->name);
            }
            return;
        }
        else
        {
            Errorf("Bad interact player link");
        }
    }

    //If this objective is scriptControlled, players can't complete it by just clicking on it
    //For example another encounter's script could be "takeProfessorToComputer".  And this could be the computer, so we want it
    //to glow, and be in an objective location, but that other encounter's script will cause it to stop glowing, not this
    if( info->def->scriptControlled ) 
    {
        serveFloatingInfo(interactPlayer, interactPlayer, "YouCantInteractWithThisGlowy");
        return;
    }

    if (info->script && ScriptGlowieInteract(interactPlayer, info, 0))
    {
        return;
    }

    // can't interact with objectives that have active force fields
    ff = erGetEnt(info->forceFieldEntity);
    if (ff)
    {
        sendInfoBox(interactPlayer, INFO_SVR_COM, "YouCantInteractWithThisGlowy");
        return;
    }

    // If the object has a requires, and the player doesn't meet the requires,
    // then he can't click on it.
    // (client should prevent this too, but we can't trust the client)
    if (info->def && info->def->activateRequires && !(MissionObjectiveExpressionEval(info->def->activateRequires)))
    {
        sendInfoBox(interactPlayer, INFO_SVR_COM, "YouCantInteractWithThisGlowy");
        return;
    }

    // If the object has an char_requires, and the player doesn't meet the requires,
    // then he can't click on it.
    if (info->def && info->def->charRequires && !(chareval_Eval(interactPlayer->pchar, info->def->charRequires, info->def->filename)) )
    {
        if (info->def->charRequiresFailedText)
        {
            ScriptVarsTable vars = {0};
            sendInfoBox(interactPlayer, INFO_SVR_COM, saUtilTableLocalize(info->def->charRequiresFailedText, &vars));
        }
        else
        {
            sendInfoBox(interactPlayer, INFO_SVR_COM, "YouCantInteractWithThisGlowy");
        }
        return;
    }


    // Tell the player he starts to interact with the object.
    // Todo - Broadcast the message to the player's team.
    if(info->def->interactBeginString)   
    {
        const char* msg;
        ScriptVarsTablePushVarEx(g_itemvars, "Rescuer", interactPlayer, 'E', 0);
        msg = saUtilTableAndEntityLocalize(info->def->interactBeginString, g_itemvars, interactPlayer);
        ScriptVarsTablePopVar(g_itemvars);
        if (info->def->simultaneousObjective)
            chatSendServerBroadcast(msg);
        else
            sendInfoBox(interactPlayer, INFO_SVR_COM, "%s", msg);
    }

    // Link the objective to the entity.
    interactPlayer->interactingObjective = info;

    // Link the entity to the objective.
    info->interactPlayerDBID = interactPlayer->db_id;
    info->interactPlayerRef = erGetRef(interactPlayer);
    MissionItemSetState(info, MOS_INTERACTING);
    info->lastInteractCheckTime = timerCpuTicks64();
    info->interactTimer = 0.0;

    // Note in the power system that the entity clicked on a mission object
    if(interactPlayer->pchar)
    {
        character_RecordEvent(interactPlayer->pchar, kPowerEvent_MissionObjectClick, timerSecondsSince2000());
    }

    // Tell the player the timer will expire in this amount of time.
    MissionItemSetClientTimer(info, info->def->interactDelay);
}

// resets the item's interaction fields
static void MissionItemStopInteraction(MissionObjectiveInfo* info)
{
    Entity* player;

    // If the objective is not being interacted with, do nothing.
    if(!info || !info->interactPlayerRef)
        return;

    // Unlink the interacting entity from the objective.
    player = erGetEnt(info->interactPlayerRef);
    if(player)
    {
        // Tell the player that the timer has stopped.
        MissionItemSetClientTimer(info, 0.0);

        // Unlink the entity from the objective.
        player->interactingObjective = NULL;
    }

    // Unlink the objective from the entity.
    MissionItemSetState(info, MOS_INITIALIZED);
    info->interactPlayerDBID = 0;
    info->interactPlayerRef = 0;
    info->interactTimer = 0.0;
}

// per-tick processing of objectives that have force fields
void ForceFieldProcess(MissionObjectiveInfo* info)
{
    Entity *ff, *obj;

    obj = erGetEnt(info->objectiveEntity);
    if (!info->def || info->def->locationtype == LOCATION_PERSON || !info->def->forceFieldVillain || !obj)
        return;

    // create our force field at the scheduled time
    ff = erGetEnt(info->forceFieldEntity);
    if (!ff && info->state < MOS_SUCCEEDED &&
        info->forceFieldTimer && dbSecondsSince2000() >= info->forceFieldTimer)
    {
        int level = info->def->level;
        const char* villain = ScriptVarsTableLookup(g_itemvars, info->def->forceFieldVillain);
        if (!level && g_activemission)
            level = g_activemission->missionLevel;

        ff = villainCreateByName(villain, level, NULL, 0, NULL, 0, NULL);
        if (ff)
        {
            entUpdateMat4Instantaneous(ff, ENTMAT(obj));
            aiInit(ff, NULL);
            aiPriorityQueuePriorityList(ff, "PL_BeEvilAndStill", 0, NULL);
            info->forceFieldEntity = erGetRef(ff);
        }
        else
            Errorf("Could not create force field entity %s", villain);

        info->forceFieldTimer = 0;
    }

    // if the force field is destroyed
    if (!ff && info->state < MOS_SUCCEEDED &&
        info->forceFieldTimer == 0 && info->def->forceFieldRespawn)
    {
        const char* objective = ScriptVarsTableLookup(g_itemvars, info->def->forceFieldObjective);

        // reschedule if we have a respawn time
        if (info->state < MOS_SUCCEEDED && info->def->forceFieldRespawn)
        {
            info->forceFieldTimer = dbSecondsSince2000() + info->def->forceFieldRespawn;
        }

        // signal the objective if we have one
        if (objective)
            MissionObjectiveCompleteEach(objective, 0, 1);
    }
}


void MissionObjectiveSendLocations( Entity *player )
{
    int i, incomplete = 0, n = eaSize(&objectiveInfoList);
    Entity * e;

    for (i = 0; i < n; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        if( info->state < MOS_SUCCEEDED )
        {
            e = erGetEnt(info->objectiveEntity);
            if(e)
                incomplete++;
        }
    }

    START_PACKET(pak, player, SERVER_SEND_ITEMLOCATIONS );
    pktSendBitsAuto(pak, db_state.map_id);
    pktSendBitsAuto(pak, incomplete);

    for (i = 0; i < n; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        if( info->state < MOS_SUCCEEDED )
        {
            e = erGetEnt(info->objectiveEntity);
            if(e)
                pktSendVec3(pak, ENTPOS(e) );
        }
    }
    END_PACKET;

    player->sentItemLocations = 1;
}

void MissionObjectiveCheckThreshold()
{
    int i, n, incomplete = 0;

    n = eaSize(&objectiveInfoList);
    for (i = 0; i < n; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        if( info->state < MOS_SUCCEEDED )
            incomplete++;
    }

    if( g_activemission && g_activemission->def && g_activemission->def->showItemThreshold >= incomplete )
        g_bSendItemLocations = LOCATION_SEND_NEW;
}


// Attempt to increment interaction timer on objectives that are
// already being interacted with.
void MissionItemProcessInteraction()
{
    int i;
    int iSize = eaSize(&objectiveInfoList);

    // Iterate through all objectives that are being interacted with.
    for(i = 0; i < iSize; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        Entity* player;
        Entity* objectiveEntity;
        Entity* ff;
        int removalRequired = 0;

        ForceFieldProcess(info);

        if (info->state != MOS_INTERACTING &&
            info->state != MOS_COOLDOWN)
            continue;

        // sanity check
        if (!info->def)
            continue;

        // Is the player that initiated the interaction still present?
        // If not, whatever interaction that was in progress has been aborted.
        // This might be possible if the interacting player gets disconnected.
        player = erGetEnt(info->interactPlayerRef);
        if(!player)
        {
            MissionItemStopInteraction(info);
            continue;
        }

        // Is the objective still here?
        objectiveEntity = erGetEnt(info->objectiveEntity);
        if(!objectiveEntity)
        {
            // If the objective entity is destroyed, the objective probably failed.
            MissionObjectiveComplete(player, info, 0);
            continue;
        }

        // has the force field respawned?
        ff = erGetEnt(info->forceFieldEntity);
        if (ff)
        {
            MissionItemInterruptInteraction(player);
            continue;
        }

        // Can the player continue to interact with the object?
        if( closeEnoughToInteract( player, objectiveEntity, SERVER_INTERACT_DISTANCE ) )
        {
            __int64 currentTime;

            // If so, advance the timer.
            currentTime = timerCpuTicks64();
            if (!info->lastInteractCheckTime)
                info->lastInteractCheckTime = currentTime;  // Should have been set when the interaction started.
            info->interactTimer = timerSeconds64(currentTime - info->lastInteractCheckTime);

            // Are we done with the interaction?
            if (info->interactTimer >= info->def->interactDelay)
            {
                if (info->def->simultaneousObjective)
                {
                    if (info->state == MOS_INTERACTING)
                    {
                        // give waiting string so players know something is happening
                        MissionItemSetState(info, MOS_COOLDOWN);
                        if (info->def->interactWaitingString)
                        {
                            ScriptVarsTablePushVarEx(g_itemvars, "Rescuer", player, 'E', 0);
                            sendInfoBox(player, INFO_SVR_COM, "%s", saUtilTableAndEntityLocalize(info->def->interactWaitingString, g_itemvars, player));
                            ScriptVarsTablePopVar(g_itemvars);
                        }
                    }
                    if (EveryObjectiveInState(info->name, MOS_COOLDOWN))
                    {
                        MissionItemGiveEachCompleteString(info->name);
                        MissionItemStopEachInteraction(info->name);
                        MissionObjectiveCompleteEach(info->name, false, true);
                    }
                }
                else
                {
                    // Tell the player interaction with the object has completed.
                    if(info->def->interactCompleteString)
                    {
                        ScriptVarsTablePushVarEx(g_itemvars, "Rescuer", player, 'E', 0);
                        sendInfoBox(player, INFO_SVR_COM, "%s", saUtilTableAndEntityLocalize(info->def->interactCompleteString, g_itemvars, player));
                        ScriptVarsTablePopVar(g_itemvars);
                    }

                    MissionItemStopInteraction(info);
                    MissionObjectiveComplete(player, info, 1);
                }
            }
            
            // Are we at the end of our cooldown period?
            if (info->state == MOS_COOLDOWN &&
                info->interactTimer >= info->def->interactDelay + OBJECTIVE_COOLDOWN_PERIOD)
            {
                // reset ourselves
                MissionItemSetState(info, MOS_INITIALIZED);
                MissionItemStopInteraction(info);
                ScriptVarsTablePushVarEx(g_itemvars, "Rescuer", player, 'E', 0);
                sendInfoBox(player, INFO_SVR_COM, "%s", saUtilTableAndEntityLocalize(info->def->interactResetString, g_itemvars, player));
                ScriptVarsTablePopVar(g_itemvars);
            }
        }
        else
        {
            // Tell the player he is too far away to interact with the entity.
            sendInfoBox(player, INFO_USER_ERROR, "YoureTooFarToInteract");
            MissionItemStopInteraction(info);
        }
    }

    // Iterate through all objectives and look for ones that should be removed.
    for (i = 0; i < iSize; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];

        if (info->state == MOS_REMOVE)
        {
            MissionObjectiveInfoDestroy(objectiveInfoList[i]);
            eaRemove(&objectiveInfoList, i);
            iSize--;
            i--;
        }
    }

}

void MissionItemInterruptInteraction(Entity* playerEntity)
{
    MissionObjectiveInfo* info;

    if(!playerEntity || ENTTYPE(playerEntity) != ENTTYPE_PLAYER || !playerEntity->interactingObjective)
        return;

    // Tell the player he's been interrupted.
    info = playerEntity->interactingObjective;
    if(info->def && info->def->interactInterruptedString)
    {
        const char* msg;
        ScriptVarsTablePushVarEx(g_itemvars, "Rescuer", playerEntity, 'E', 0);
        msg = saUtilTableAndEntityLocalize(info->def->interactInterruptedString, g_itemvars, playerEntity);
        ScriptVarsTablePopVar(g_itemvars);
        if (info->def->simultaneousObjective)
            chatSendServerBroadcast(msg);
        else
            sendInfoBox(playerEntity, INFO_SVR_COM, "%s", msg);
    }

    MissionItemStopInteraction(playerEntity->interactingObjective);
}

// *********************************************************************************
//  Working on objectives as aggregates - by logical name
// *********************************************************************************

int EveryObjectiveInState(const char* objectivename, MissionObjectiveState state)
{
    int i, n;
    n = eaSize(&objectiveInfoList);
    for (i = 0; i < n; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        if (!stricmp(info->name, objectivename) && 
            info->state != state)
            return 0;
    }
    return 1;
}

int NumObjectivesInState(const char* objectivename, MissionObjectiveState state)
{
    int i, n, count = 0;
    n = eaSize(&objectiveInfoList);
    for (i = 0; i < n; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        if (!stricmp(info->name, objectivename) && 
            info->state == state)
            count++;
    }
    return count;
}

void MissionItemStopEachInteraction(const char* objectivename)
{
    int i, n;
    n = eaSize(&objectiveInfoList);
    for (i = 0; i < n; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        if (!stricmp(info->name, objectivename)) 
            MissionItemStopInteraction(info);
    }
} 


MissionObjectiveInfo *MissionObjectiveCreate(const char* objectiveName, const char* state, const char* singularState)
{
    // create a dummy objective
    MissionObjectiveInfo* info = MissionObjectiveInfoCreate();
    
    info->name = strdup(objectiveName);

    if (state)
        info->description = strdup(state);

    if (singularState)
        info->singulardescription = strdup(singularState);

    MissionItemSetState(info, MOS_INITIALIZED);

    eaPush(&objectiveInfoList, info);

    MissionRefreshAllTeamupStatus();

    return info;
}

void MissionObjectiveCompleteEach(const char* objectivename, int CreateIfNotFound, int success)
{
    int i, n;
    int found = false;

    n = eaSize(&objectiveInfoList);
    for (i = 0; i < n; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        if (!stricmp(info->name, objectivename))
        {
            Entity* player = erGetEnt(info->interactPlayerRef);
            MissionObjectiveComplete(player, info, success);
            found = true;
        }
    }

    if (CreateIfNotFound && !found)
    {
        // create a dummy objective
        MissionObjectiveInfo *info = MissionObjectiveCreate(objectivename, NULL, NULL);

        // set it complete
        MissionObjectiveComplete(NULL, info, success);
    }
}

void MissionObjectiveSetTextEach(const char* objectivename, const char *description, const char *singularDescription)
{
    int i, n;

    n = eaSize(&objectiveInfoList);
    for (i = 0; i < n; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        if (!stricmp(info->name, objectivename))
        {
            if (description) {
                if (info->description)
                    free(info->description);
                info->description = strdup(description);
            }

            if (singularDescription) {
                if (info->singulardescription)
                    free(info->singulardescription);
                info->singulardescription = strdup(singularDescription);
            }
        }
    }

    // refresh UI
    MissionRefreshAllTeamupStatus();

}


void MissionItemGiveEachCompleteString(const char* objectivename)
{
    int i, n;
    n = eaSize(&objectiveInfoList);
    for (i = 0; i < n; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        if (!stricmp(info->name, objectivename))
        {
            Entity* player = erGetEnt(info->interactPlayerRef);
            if (player && info->def && info->def->interactCompleteString)
            {
                ScriptVarsTablePushVarEx(g_itemvars, "Rescuer", player, 'E', 0);
                sendInfoBox(player, INFO_SVR_COM, "%s", saUtilTableAndEntityLocalize(info->def->interactCompleteString, g_itemvars, player));
                ScriptVarsTablePopVar(g_itemvars);
            }
        }
    }
}

//Since I'm not sure how to decide what's interested, I just reparse ALL activateRequires.
// This used to get a player as a parameter, however that had to be ditched because scripts (e.g. chatty_boss) can complete
// objectives without a player.  So where we call this we just go ahead and do so no matter what.  Since the char_eval call
// was already commented out, that probably won't cause any major problems.
void RecheckActivateRequires()
{
    int i, n;
    int flag;

    n = eaSize(&objectiveInfoList);
    for (i = 0; i < n; i++)
    {
        MissionObjectiveInfo* info = objectiveInfoList[i];
        if (info->state == MOS_REQUIRES)
        {
            flag = true;
            if (info->def && info->def->activateRequires)
                flag = MissionObjectiveExpressionEval(info->def->activateRequires);

            if (flag)
            {
                MissionItemSetState(info, MOS_INITIALIZED);
            }
        }
    }
}


// *********************************************************************************
//  Mission item debugging
// *********************************************************************************

void DestroyAllItemObjectives()
{
    int i, n;
    n = eaSize(&objectiveInfoList);
    for (i = 0; i < n; i++)
    {
        Entity* e = erGetEnt(objectiveInfoList[i]->objectiveEntity);
        if (e)
            entFree(e);
    }
    eaClearEx(&objectiveInfoList, NULL);
}

// MAK - what is the difference between these two?

// destroy any entities that are objectives
void MissionDestroyAllObjectives()
{
    int i;

    // cycle through all entities, and free any marked as objectives
    for(i=1;i<entities_max;i++)
    {
        Entity* e = entities[i];
        if (!(entity_state[i] & ENTITY_IN_USE ) )
            continue;
        if (e->missionObjective)
            entFree(e);
    }
}

const ItemPosition* GetClosestItemPosition(RoomInfo* room, const Mat4 mat)
{
    const ItemPosition* bestpos;
    F32 bestdist;
    int i, n;

    n = eaSize(&room->items);
    if (n == 0) return NULL;
    bestpos = room->items[0];
    bestdist = distance3Squared(mat[3], bestpos->pos[3]);
    for (i = 1; i < n; i++)
    {
        F32 dist = distance3Squared(mat[3], room->items[i]->pos[3]);
        if (dist < bestdist)
        {
            bestdist = dist;
            bestpos = room->items[i];
        }
    }
    return bestpos;
}

// spawn a particular objective model at the nearest objective item position
void SpawnObjectiveItem(ClientLink* client, char* costume)
{
    Entity* e;
    RoomInfo* room = ItemTestingLoad(client); // load item position info
    const ItemPosition* pos;

    // destroy any existing items first
    DestroyAllItemObjectives();

    // try to get a position and model
    pos = GetClosestItemPosition(room, ENTMAT(client->entity));
    e = npcCreate(costume, ENTTYPE_MISSION_ITEM);
    if (!e)
        conPrintf(client, saUtilLocalize(e,"CantfindCostume", costume));
    if (!pos)
        conPrintf(client, saUtilLocalize(e,"NoItemPosition") );
    if (!e)
        return;

    // finally, set up model
    if(pos)
    {
        conPrintf(client, "%s %s, at (%0.2f, %0.2f, %0.2f)", saUtilLocalize(e,"Spawning"), costume, pos->pos[3][0], pos->pos[3][1], pos->pos[3][2]);
    }
    
    SET_ENTTYPE(e) = ENTTYPE_MISSION_ITEM;

    // set the position
    if(pos)
        entUpdateMat4Instantaneous(e, pos->pos);
    else
        entUpdateMat4Instantaneous(e, ENTMAT(client->entity));

    strcpy(e->name, costume);

    aiInit(e, NULL);
}

// *********************************************************************************
//  Key Doors - just a door that can be locked until players complete an objective
// *********************************************************************************

static int* keydoorsActivated = 0;

// Give the player a waypoint if he unlocked a key door
void KeyDoorUpdate(MissionObjectiveInfo* objective)
{
    int i, n;

    if (!OnMissionMap() || !g_activemission->initialized || !objective->def) return;

    // First find a keydoor whose objective contains the objective just completed
    n = eaSize(&g_activemission->def->keydoors);
    for (i = 0; i < n; i++)
    {
        const MissionKeyDoor* door = g_activemission->def->keydoors[i];
        if (StringArrayFind(door->objectives, objective->def->name) >= 0 && MissionObjectiveIsSetComplete(door->objectives, 1))
        {
            // The objective was part of a keydoor that is now complete, find the door and send a waypoint
            int j, numDoors;
            DoorEntry* doors = dbGetLocalDoors(&numDoors);
            for (j = 0; j < numDoors; j++)
            {
                if (!stricmp(doors[j].name, door->name))
                {
                    int k;
                    PlayerEnts *players = &player_ents[PLAYER_ENTS_ACTIVE];

                    // Now send to all players in the map
                    for (k = 0; k < players->count; k++)
                    {
                        if (players->ents[k])
                        {
                            START_PACKET(pak, players->ents[k], SERVER_MISSION_KEYDOOR)
                                pktSendBitsPack(pak, 1, 1);
                                pktSendBitsPack(pak, 1, j);
                                pktSendBits(pak, 1, 1);                // PAK: New Door to turn on
                                pktSendF32(pak, doors[j].mat[3][0]);
                                pktSendF32(pak, doors[j].mat[3][1]);
                                pktSendF32(pak, doors[j].mat[3][2]);
                            END_PACKET
                        }
                    }
                    eaiPush(&keydoorsActivated, j);
                    break;
                }
            }
        }
    }
}

// Sends the keydoors when a player enters the map
void KeyDoorNewPlayer(Entity* player)
{
    int i, numDoors, count = eaiSize(&keydoorsActivated);
    if (count)
    {
        DoorEntry* doors = dbGetLocalDoors(&numDoors);
        START_PACKET(pak, player, SERVER_MISSION_KEYDOOR)
        pktSendBitsPack(pak, 1, count);
        for (i = 0; i < count; i++)
        {
            int doorIndex = keydoorsActivated[i];
            if (doorIndex >= 0 && doorIndex < numDoors)
            {
                pktSendBitsPack(pak, 1, doorIndex);
                pktSendBits(pak, 1, 1);                // PAK: New Door to turn on
                pktSendF32(pak, doors[doorIndex].mat[3][0]);
                pktSendF32(pak, doors[doorIndex].mat[3][1]);
                pktSendF32(pak, doors[doorIndex].mat[3][2]);
            }
        }
        END_PACKET
    }
}

// When the player goes through the keydoor, we want to turn their waypoint off
// Notify them that the door has been "unlocked"
void KeyDoorUnlockUpdate(Entity* player, char* doorname)
{
    if (OnMissionMap() && g_activemission->initialized)
    {
        // Make sure the doorname is actually a key door, then if its unlocked, send the player an update
        int i, n = eaSize(&g_activemission->def->keydoors);
        for (i = 0; i < n; i++)
        {
            const MissionKeyDoor* door = g_activemission->def->keydoors[i];

            // It is a keydoor and is no longer locked, find the door and turn it off
            if (!stricmp(door->name, doorname) && !KeyDoorLocked(doorname))
            {
                int j, numDoors;
                DoorEntry* doors = dbGetLocalDoors(&numDoors);
                for (j = 0; j < numDoors; j++)
                {
                    if (!stricmp(doors[j].name, door->name))
                    {
                        START_PACKET(pak, player, SERVER_MISSION_KEYDOOR)
                            pktSendBitsPack(pak, 1, 1);
                            pktSendBitsPack(pak, 1, j);
                            pktSendBits(pak, 1, 0);                // PAK: Turn the door off
                        END_PACKET
                    }
                }
            }
        }
    }
}

int KeyDoorLocked(char* doorname)
{
    int i, n;

    if (!OnMissionMap() || !g_activemission->initialized) return 0;

    // find the corresponding keydoor
    n = eaSize(&g_activemission->def->keydoors);
    for (i = 0; i < n; i++)
    {
        const MissionKeyDoor* door = g_activemission->def->keydoors[i];
        if (!stricmp(door->name, doorname))
        {
            return !MissionObjectiveIsSetComplete(door->objectives, 1);
        }
    }

    return 0;
}

// *********************************************************************************
//  Mission Key Clues
// *********************************************************************************

U32 g_keyclues = 0;

// returns 1 if clue was a key clue
int MissionAddRemoveKeyClue(Entity* player, const char* cluename, int add)
{
    int i, n;

    if (!g_activemission) return 0;
    
    // find the clue index
    n = eaSize(&g_activemission->def->keyclues);
    for (i = 0; i < n; i++)
    {
        if (stricmp(cluename, g_activemission->def->keyclues[i]->name)==0)
        {
            if(player && add && !BitFieldGet(&g_keyclues, KEYCLUE_BITFIELD_SIZE, i))
                serveFloater(player, player, "FloatFoundKey", 0.0f, kFloaterStyle_Attention, 0);

            BitFieldSet(&g_keyclues, KEYCLUE_BITFIELD_SIZE, i, add);
            MissionRefreshAllTeamupStatus();
            return 1;
        }
    }
    return 0;
}

