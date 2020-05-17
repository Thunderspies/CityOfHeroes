/*\
 *
 *    mission.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    top level mission handling functions
 *
 */

#include "mission.h"
#include "storyarcprivate.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/dbnamecache.h"
#include "svr/svr_player.h"
#include "container/team.h"
#include "cmdparse/cmdserver.h"
#include "container/containerbroadcast.h"
#include "dbcomm/dbdoor.h"
#include "comm_game.h"
#include "entity/character_base.h"
#include "svr/svr_tick.h"
#include "entity/character_level.h"
#include "gameSys/arenamap.h"
#include "entity/entity.h"
#include <utilitieslib/utils/eval.h>
#include "gameSys/sgraid.h"
#include "gameSys/raidmapserver.h"
#include "entity/supergroup.h"
#include "door.h"
#include "reward.h"
#include "taskforce.h"
#include "entity/TaskforceParams.h"
#include "storyarc/missionMapCommon.h"
#include "playerCreatedStoryarcServer.h"
#include "dbcomm/staticMapInfo.h"
#include "gameSys/Turnstile.h"
#include "LWC_common.h"
#include "script/ZoneEvents/ScriptedZoneEvent.h"

#include "dbcomm/logcomm.h"

// *********************************************************************************
//  Active Mission Information
// *********************************************************************************

MissionInfo* g_activemission = NULL;
int g_ArchitectTaskForce_dbid;
int g_ArchitectCheated;
TaskForce *g_ArchitectTaskForce;
static int g_forcemissionload = 0;        // Development Mode flag to treat map as a mission map with a blank mission inside it

char* MissionConstructInfoString(int owner, int sgowner, int taskforce, ContactHandle contact, StoryTaskHandle *sahandle, 
                                 U32 seed, int level, StoryDifficulty *pDifficulty, U32 completeSideObjectives, EncounterAlliance encAlly, VillainGroupEnum villainGroup)
{
    static char buf[256];

    snprintf(buf, sizeof(buf), "%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i", owner, sgowner, taskforce, contact, 
        sahandle?sahandle->context:0, sahandle?sahandle->subhandle:0, sahandle?sahandle->compoundPos:0, sahandle?sahandle->bPlayerCreated:0, 
        seed, level, pDifficulty?pDifficulty->levelAdjust:0, pDifficulty?pDifficulty->teamSize:0, pDifficulty?pDifficulty->dontReduceBoss:0, pDifficulty?pDifficulty->alwaysAV:0, 
        completeSideObjectives, encAlly, villainGroup);
    buf[sizeof(buf) - 1] = 0;
    return buf;
}
int MissionDeconstructInfoString(char* str, int* owner, int* sgowner, int* taskforce, ContactHandle* contact, StoryTaskHandle *sahandle,
                                 U32* seed,  int* level, StoryDifficulty *pDifficulty, U32* completeSideObjectives, EncounterAlliance* encAlly, VillainGroupEnum* villainGroup)
{
    return 17 == sscanf(str, "%i:%i:%i:%Ii:%Ii:%i:%i:%hhi:%i:%i:%i:%i:%i:%i:%i:%i:%i", owner, sgowner, taskforce, contact, &sahandle->context, &sahandle->subhandle, &sahandle->compoundPos, &sahandle->bPlayerCreated, 
        seed, level, &pDifficulty->levelAdjust, &pDifficulty->teamSize, &pDifficulty->dontReduceBoss, &pDifficulty->alwaysAV, completeSideObjectives, encAlly, villainGroup);
}

void SetDayNightCycle(DayNightCycleEnum daynight)
{
    switch (daynight)
    {
        case DAYNIGHT_NORMAL:
            server_visible_state.timescale = DAY_TIMESCALE;
            break;
        case DAYNIGHT_ALWAYSNIGHT:
            server_visible_state.timescale = 0.0;
            svrTimeSet(0.0);
            break;
        case DAYNIGHT_ALWAYSDAY:
            server_visible_state.timescale = 0.0;
            svrTimeSet(12.0);
            break;
        case DAYNIGHT_FAST:
            server_visible_state.timescale = 50 * DAY_TIMESCALE;
            break;
    }
}

static void SetFog(F32 dist, const Vec3 fog_color )
{
    char buf[128];

    if( dist > 0 )
    {
        sprintf(buf,"svr_fog_dist %f",dist);
        cmdOldServerReadInternal(buf);
    }

    if( fog_color[0] > 0 )
    {
        sprintf(buf,"svr_fog_color %f %f %f",fog_color[0], fog_color[1], fog_color[2] );
        cmdOldServerReadInternal(buf);
    }
}

void MissionSetInfoString(char* info, StoryArc *playerCreatedArc)
{
    int owner, sgowner, level, taskforce;
    U32 seed, sideObjs;
    ContactHandle contact;
    StoryTaskHandle sahandle;
    StoryDifficulty difficulty;
    VillainGroupEnum villainGroup;
    EncounterAlliance encAlly;

    memset(&sahandle, 0, sizeof(StoryTaskHandle));

    if (!info || !info[0])
        return;
    if (g_activemission)
        return;

    eaiCreate(&g_teamupids);
    g_activemission = calloc(sizeof(MissionInfo), 1);

    log_printf("missiondoor.log", "Map %i got info string %s", db_state.map_id, info);
    if (!MissionDeconstructInfoString(info, &owner, &sgowner, &taskforce, &contact, &sahandle, &seed, &level, &difficulty, &sideObjs, &encAlly, &villainGroup))
    {
        log_printf("missiondoor.log", "Map %i got bad string %s", db_state.map_id, info);
        g_activemission = NULL;
        dbAsyncReturnAllToStaticMap("MissionSetInfoString() bad info",info);
        return;
    }

    if (!sahandle.context && !g_forcemissionload)
    {
        LOG(LOG_ERROR, LOG_LEVEL_ALERT, LOG_CONSOLE_ALWAYS, "WTF: Map %i bad sahandle.context, info string: %s", db_state.map_id, info);
    }

    // init info we were given
    assert(!db_state.is_endgame_raid);        //    end game raids should not have their owner changed
                                            //    trying to catch the culprit
    if (!db_state.is_endgame_raid)
    {
        g_activemission->ownerDbid = owner;
    }
    g_activemission->supergroupDbid = sgowner;
    g_activemission->taskforceId = taskforce;
    g_activemission->contact = contact;
    StoryHandleCopy( &g_activemission->sahandle, &sahandle);
    g_activemission->seed = seed;
    difficultyCopy( &g_activemission->difficulty, &difficulty);
    g_activemission->missionAllyGroup = encAlly;
    g_activemission->missionGroup = villainGroup;
    g_activemission->completeSideObjectives = sideObjs;
    if ((!owner || !contact || !sahandle.context) && !g_forcemissionload) // || !sahandle.subhandle) 
    {
        log_printf("missiondoor.log", "Map %i not initialized correctly, shutting down", db_state.map_id);
        dbAsyncReturnAllToStaticMap("MissionSetInfoString() bad init",info);
        return;
    }

    if(g_activemission->sahandle.bPlayerCreated)
    {
        StoryArc *pArc;
        int i, n, taskindex;

        g_ArchitectTaskForce_dbid = g_activemission->sahandle.context;
        if(    !dbSyncContainerRequest(CONTAINER_TASKFORCES, g_ArchitectTaskForce_dbid, CONTAINER_CMD_DONT_LOCK, 0) ||
            !g_ArchitectTaskForce)
        {
            dbAsyncReturnAllToStaticMap("MissionSetInfoString() could not load taskforce", "No taskforce");
            return;
        }

        pArc = playerCreatedStoryArc_GetStoryArc(g_ArchitectTaskForce_dbid);
        if(!pArc)
        {
            dbAsyncReturnAllToStaticMap("MissionSetInfoString() could not load player created story arc", "No storyarc");
            return;
        }

        if (g_activemission->sahandle.subhandle < 0) 
        {
            dbAsyncReturnAllToStaticMap("MissionSetInfoString() invalid player created subhandle", "Bad subhandle");
            return;
        }

        // have to cycle through episodes counting tasks
        taskindex = g_activemission->sahandle.subhandle;
        n = eaSize(&pArc->episodes);
        for (i = 0; i < n; i++)
        {
            int numtasks = eaSize(&pArc->episodes[i]->tasks);
            if (taskindex < numtasks) 
            {
                g_activemission->task = pArc->episodes[i]->tasks[taskindex];
                break;
            }
            else 
                taskindex -= numtasks;
        }

        if (g_activemission->task->missionref) 
            g_activemission->def = g_activemission->task->missionref;
        if (eaSize(&g_activemission->task->missiondef)) 
            g_activemission->def = g_activemission->task->missiondef[0];

        if(!g_activemission->def)
        {
            dbAsyncReturnAllToStaticMap("MissionSetInfoString() no player created mission def", "No mission");
            return;
        }
    }
    else
    {
        g_activemission->task = TaskDefinition(&g_activemission->sahandle);
        g_activemission->def = TaskMissionDefinition(&g_activemission->sahandle);
    }

    if (!g_activemission->task && !g_forcemissionload)
    {
        log_printf("missiondoor.log", "Map %i couldn't get task def from (%i,%i,%i,%i), shutting down", db_state.map_id, SAHANDLE_ARGSPOS(g_activemission->sahandle) );
        dbAsyncReturnAllToStaticMap("MissionSetInfoString() no task def",info);
        return;
    }
    if (!g_activemission->def && !g_forcemissionload)
    {
        log_printf("missiondoor.log", "Map %i couldn't get mission def from (%i,%i,%i,%i), shutting down", db_state.map_id, SAHANDLE_ARGSPOS(g_activemission->sahandle) );
        dbAsyncReturnAllToStaticMap("MissionSetInfoString() no mission def",info);
        return;
    }
    // figure out level and pacing
    if( g_ArchitectTaskForce )
    {
        g_activemission->missionLevel = CLAMP(level+g_activemission->difficulty.levelAdjust,  CLAMP(gArchitectMapMinLevel+1, 1, 54), 
                                                                    CLAMP(gArchitectMapMaxLevel+1, 1, 54) );
        g_activemission->baseLevel = g_activemission->missionLevel;
    }
    else
    {
        g_activemission->baseLevel = level;
        g_activemission->missionLevel = CLAMP(level+g_activemission->difficulty.levelAdjust, 1, 54);
    }
    
    // MAK - move all initialization here eventually??  only remaining problem is task force name variable setup..
    if(g_activemission->def)
    {
        g_activemission->missionPacing = g_activemission->def->villainpacing;
        SetDayNightCycle(g_activemission->def->daynightcycle);
        SetFog( g_activemission->def->fFogDist, g_activemission->def->fogColor);
    }

    // Making the PlayTogetherMission functionality default
    server_state.allowHeroAndVillainPlayerTypesToTeamUp = 1;
    server_state.allowPrimalAndPraetoriansToTeamUp = 1;
}

void MissionForceLoad(int set)
{
    g_forcemissionload = set;
}

// Returns the leveladjust from the missions base level
int MissionGetLevelAdjust(void)
{
    if (g_activemission)
        return g_activemission->missionLevel - g_activemission->baseLevel;
    return 0;
}

// initializes active mission information for this map, should be done once map is loaded
MissionInfo* MissionInitInfo(Entity* player)
{
    if (g_forcemissionload)
    {
        if (!g_activemission)
            MissionSetInfoString(MissionConstructInfoString(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),0);
        g_activemission->task = 0;
        g_activemission->def = 0;
        g_activemission->initialized = 1;
        g_activemission->starttime = 0;
        ScriptVarsTableClear(&g_activemission->vars);
        MissionItemSetVarsTable(&g_activemission->vars);
        return g_activemission;
    }

    if (!g_activemission) 
        return NULL;
    if (g_activemission->initialized) 
        return g_activemission;
    eaDestroy(&objectiveInfoList);

    // checking for override of mission level
    if (g_activemission->def && g_activemission->def->missionLevel >=0 )
    {
        g_activemission->baseLevel = g_activemission->def->missionLevel;
        g_activemission->missionLevel = CLAMP(g_activemission->baseLevel + g_activemission->difficulty.levelAdjust, 1, 54);
    }

    // If g_activemission->ownerDbid is zero, we're using the placeholder mission for end game raids, which means the var scopes are not valid
    if (g_activemission->ownerDbid)
    {
        // ScriptVarsTableClear called in MissionForceUninit, possible mismatch
        saBuildScriptVarsTable(&g_activemission->vars, player, g_activemission->ownerDbid, g_activemission->contact, NULL, g_activemission->task, NULL, NULL, NULL, g_activemission->missionGroup, g_activemission->seed, true);
    }

    MissionItemSetVarsTable(&g_activemission->vars);
    g_activemission->initialized = 1;
    g_activemission->starttime = dbSecondsSince2000();

    // Initialize the timeout based on the task from the player
    if (player && player->teamup && player->teamup->activetask)
    {
        g_activemission->failOnTimeout = player->teamup->activetask->failOnTimeout;
        g_activemission->timerType = player->teamup->activetask->timerType;
        g_activemission->timeout = player->teamup->activetask->timeout;
        g_activemission->timezero = player->teamup->activetask->timezero;
    }

    if (TASK_DELAY_START(g_activemission->task))
    {
        U32 timeLimit = g_activemission->starttime + g_activemission->task->timeoutMins * 60;
        if (!g_activemission->timeout || g_activemission->timeout > timeLimit)
        {
            char startTaskTimerCmd[100];
//            g_activemission->timeout = timeLimit;
            sprintf(startTaskTimerCmd, "taskstarttimer %d %d %d %d", g_activemission->ownerDbid, g_activemission->sahandle.context, g_activemission->sahandle.subhandle, g_activemission->task->timeoutMins * 60);
            serverParseClient(startTaskTimerCmd, NULL);
        }
    }
    
    // setting up flashback flag
    if (player)
    {
        g_activemission->flashback = TaskForceIsOnFlashback(player) || TaskForceIsOnArchitect(player);
        g_activemission->scalableTf = TaskForceIsScalable(player);

        TaskForceInitMap(player);
    }

    // do a lookup for the correct villain group
    if ( g_activemission->missionGroup <= VG_NONE )
    {
        // try again for good measure
        g_activemission->missionGroup = villainGroupGetEnum(g_activemission->def->villaingroup);

        if ( g_activemission->missionGroup <= VG_NONE && !g_ArchitectTaskForce ) // allow empty villain groups
        {
            if (g_activemission->def->CVGIdx == 0) 
            {
                Errorf("MissionGetInfo - failed lookup of villain group for mission %s - saw %s", g_activemission->def->filename, g_activemission->def->villaingroup);
            }
            //    just in case the cvg is bad, let's have a backup. set the missionGroup to warriors
            g_activemission->missionGroup = villainGroupGetEnum("Warriors");
        }
    }

    // place objectives and spawndefs
    ObjectiveDoPlacement();
    EncounterDoPlacement();
    if (g_activemission->def->numrandomnpcs)
        EncounterSpawnMissionNPCs(g_activemission->def->numrandomnpcs);

    // Drop in some skill objects
    SkillDoPlacement();

    // finally, kick off any mission scripts
    StartMissionScripts(g_activemission->def, &g_activemission->vars);

    // add in defeatall objective here
    if( g_activemission &&  g_activemission->def && g_activemission->def->pchDefeatAllText )
        MissionObjectiveCreate( "DefeatAll", g_activemission->def->pchDefeatAllText, g_activemission->def->pchDefeatAllText );

    // need to refresh objective state here, owners didn't get a full update before we filled out g_activemission
    MissionRefreshAllTeamupStatus();

    printf("Mission initialized: %s\n", g_activemission->task->logicalname);
    log_printf("missiondoor.log", "Map %i initializing mission %s with owner %s",
        db_state.map_id, g_activemission->task->logicalname, dbPlayerNameFromId(g_activemission->ownerDbid));
    return g_activemission;
}

// *********************************************************************************
//  Debug Uninit/Renint mission functions
// *********************************************************************************

static char g_initstring[100] = {0};

void MissionForceUninit()
{
    if (!g_activemission) 
        return;

    ScriptEndAllRunningScripts(true);
    strcpy(g_initstring, MissionConstructInfoString(g_activemission->ownerDbid, g_activemission->supergroupDbid,
        g_activemission->taskforceId, g_activemission->contact, &g_activemission->sahandle,
        g_activemission->seed, g_activemission->baseLevel, &g_activemission->difficulty,
        g_activemission->completeSideObjectives, g_activemission->missionAllyGroup, g_activemission->missionGroup));

    if (g_activemission->currentServerMusicName)
    {
        free(g_activemission->currentServerMusicName);
    }

    ScriptVarsTableClear(&g_activemission->vars);

    free(g_activemission);
    g_activemission = 0;
}

void MissionTeamupListAddAll(int countAdmins)
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int foundplayer = 0;
    int i;

    for (i = 0; i < players->count; i++)
    {
        // don't count admins spying as players on the map
        if (players->ents[i]->admin && !countAdmins) continue;
        MissionTeamupListAdd(players->ents[i]);
    }
}

// Kill off all the npcs
void MissionDestroyAllNPCs(void)
{
    int i;
    for(i = 0;i < entities_max; i++)
        if (entities[i] && ENTTYPE(entities[i]) == ENTTYPE_NPC)
            entFree(entities[i]);
}

void MissionDestroyAllSkills(void)
{
    int i;
    extern Entity **g_SkillObjects;

    for(i=eaSize(&g_SkillObjects)-1; i>=0; i--)
    {
        Entity *e = g_SkillObjects[i];
        if(e)
        {
            entFree(e);
        }
    }
    eaSetSize(&g_SkillObjects, 0);
}


// debug only - reinitialize mission
void MissionForceReinit(Entity* player)
{
    if (!g_initstring[0]) 
        return;
    MissionSetInfoString(g_initstring,0);
    EncounterLoad();
    MissionDestroyAllObjectives();
    MissionDestroyAllSkills();
    MissionDestroyAllNPCs();
    MissionInitInfo(player);
    MissionTeamupListAddAll(1);

    // construct status of mission again
    if (teamLock(player, CONTAINER_TEAMUPS))
    {
        MissionConstructStatus(player->teamup->mission_status, player, player->teamup->activetask);
        teamUpdateUnlock(player, CONTAINER_TEAMUPS);
    }
}

// *********************************************************************************
//  Mission objective sets & mission completion
// *********************************************************************************

// Check if all given encounter groups are conquored.
int MissionEncountersCheckComplete(EncounterGroup** groups)
{
    int i;
    for(i = eaSize(&groups)-1; i >= 0; i--)
    {
        EncounterGroup* group = groups[i];
        if (!group->uncounted && 
            ((group->active && group->active->villaincount > 0) ||
            (!group->active && !group->conquered)))
            return 0;
    }

    return 1;
}

int MissionEncounterRoomCheckComplete(RoomInfo* room)
{
    if( MissionEncountersCheckComplete(room->standardEncounters) &&
        MissionEncountersCheckComplete(room->hostageEncounters))
        return 1;

    return 0;
}

EvalContext *pObjectiveExpressionEvalContext = NULL;


static void MissionObjectiveExpressionCompletedEval(EvalContext *pcontext)
{
    const char *objective = eval_StringPop(pcontext);
    int retval = !(MissionObjectiveIsComplete(objective, 1) || MissionObjectiveIsComplete(objective, 0));

    eval_IntPush(pcontext, retval);
}

static void MissionObjectiveExpressionFailedEval(EvalContext *pcontext)
{
    const char *objective = eval_StringPop(pcontext);

    eval_IntPush(pcontext, MissionObjectiveIsComplete(objective, 0));
}

static void MissionObjectiveExpressionSingleEval(EvalContext *pcontext)
{
    const char *objective = eval_StringPop(pcontext);

    eval_IntPush(pcontext, MissionObjectiveIsComplete(objective, 1));
}

int MissionObjectiveExpressionEval(const char **expr)
{
    if (pObjectiveExpressionEvalContext == NULL)
    {
        pObjectiveExpressionEvalContext = eval_Create();

        // initialize context
        eval_Init(pObjectiveExpressionEvalContext);
        eval_RegisterFunc(pObjectiveExpressionEvalContext, "int>", MissionObjectiveExpressionSingleEval, 1, 1);
        eval_RegisterFunc(pObjectiveExpressionEvalContext, "failed", MissionObjectiveExpressionFailedEval, 1, 1);
        eval_RegisterFunc(pObjectiveExpressionEvalContext, "completed", MissionObjectiveExpressionCompletedEval, 1, 1);
    }

    return eval_Evaluate(pObjectiveExpressionEvalContext, expr);
}

//
//
// Returns:            0 if objective has not been completed (any success flag)
//                    1 if objective has been completed successfully and true success flag
//                    0 if objective had been completed successfully and false success flag
//                    1 if objective has been completed and failed and false success flag
//                    0 if objective has been completed and failed and true success flag
//
int MissionObjectiveIsComplete(const char* objective, int success)
{
    int objCursor;
    assertmsg(OnMissionMap(), "Mission objectives should probably be using instanced vars");
    if(stricmp(objective, "DefeatAllVillains") == 0)
    {
        if( success )
        {
            if(!EncounterAllGroupsComplete() )
                return 0;
            MissionObjectiveCompleteEach( "DefeatAll", 0, 1 );
        }
    }
    else if(stricmp(objective, "DefeatAllVillainsInEndRoom") == 0)
    {
        // Check if all encounters in the end room has been completed.
        int roomCursor;
        for(roomCursor = eaSize(&g_rooms)-1; roomCursor >= 0; roomCursor--)
        {
            RoomInfo* room = g_rooms[roomCursor];

            if(!room->endroom)
                continue;

            if( success )
            {
                if(!MissionEncounterRoomCheckComplete(room) )
                    return 0;
                MissionObjectiveCompleteEach( "DefeatAll", 0, 1 );
            }
        }
    }
    else if (stricmp(objective, "ScriptControlled") == 0)
    {
        return 0;
    }
    else if (stricmp(objective, "EachPlayerLeavesMap") == 0)
    {
        if (!g_playersleftmap) 
            return 0;
    }
    else
    {
        // Process all mission objectives of the specified name.
        int foundObj = 0;
        for(objCursor = eaSize(&objectiveInfoList)-1; objCursor >= 0; objCursor--)
        {
            MissionObjectiveInfo* obj = objectiveInfoList[objCursor];
            if(stricmp(obj->name, objective) != 0)
                continue;

            // The mission objective is not complete.
            // Not all required mission objectives are complete.
            foundObj = 1;
            if (success && !obj->succeeded)
                return 0;
            if (!success && obj->failed) // if there a multiple objects with same name, any fail is a fail
                return 1;
        }

        if( foundObj && !success ) // we found our things, but they haven't failed yet
            return 0;

        // OLD WAY
        // if we don't have foundObj we weren't able to place it on the map..
        // treat success conditions as having been filled, failure as not
        //if (!foundObj  && !success)
        //    return 0;
        // END OLD WAY

        //MW changed that a nonexistant objective has neither succeeded nor failed. 
        //Because scripts can now add objectives mid-mission, a non existant objective is not necessarily a bug now
        if( !foundObj && success )
            return 0; //A non-existant objective has not succeeded 
        if( !foundObj && !success )
            return 0; //A non-existant objective has not failed
    }
    return 1;
}

// checking to see if the entire set is succeeded or failed
int MissionObjectiveIsSetComplete(char** set, int success)
{
    int curCrit, numCrit;
    numCrit = eaSize(&set);
    for (curCrit = 0; curCrit < numCrit; curCrit++)
    {
        if (!MissionObjectiveIsComplete(set[curCrit], success))
            return 0;
    }
    return 1; // if we got here, all criteria were filled
}

// Check if any required mission objective sets has been completed.
int MissionAnyObjectiveSetsComplete(int* success)
{
    int curSet, numSet;

    //7-27-05 Changed order of evaluation so if one mission set succeeds and another fails at the same time, the mission fails.  
    //This way Script set failures such as "Boss Escaped" resolve before success conditions like "Defeat All Villains"
    numSet = eaSize(&g_activemission->def->failsets);
    for (curSet = 0; curSet < numSet; curSet++)
    {
        if (MissionObjectiveIsSetComplete(g_activemission->def->failsets[curSet]->objectives, 0))
        {
            *success = 0;
            return 1;
        }
    }

    numSet = eaSize(&g_activemission->def->objectivesets);
    for (curSet = 0; curSet < numSet; curSet++)
    {
        if (MissionObjectiveIsSetComplete(g_activemission->def->objectivesets[curSet]->objectives, 1))
        {
            *success = 1;
            return 1;
        }
    }

    return 0; // none of the objective sets were completed
}

void MissionObjectiveCheckComplete()
{
    int success = 1;

    // done, or haven't seen a player and init'ed
    if (MissionCompleted() || !g_activemission || !g_activemission->initialized || !g_activemission->def) 
        return;

    if (MissionAnyObjectiveSetsComplete(&success))
        MissionSetComplete(success);

    // some default behavior if we didn't explicity specify the objectives
    else if (!eaSize(&g_activemission->def->objectivesets))
    {
        int completed = 1;
        int i;
        int iSize = eaSize(&objectiveInfoList);

        // Check if all objectives have completed
        for(i = 0; i < iSize; i++)
        {
            MissionObjectiveInfo* info = objectiveInfoList[i];
            if (info->state != MOS_SUCCEEDED && info->state != MOS_FAILED)
                completed = 0;
        }
        // and, all encounters completed
        if(!EncounterAllGroupsComplete())
            completed = 0;

        if (completed)
            MissionSetComplete(1);
    }
}

// Grants the secondary reward to the player
void MissionGrantSecondaryReward(int db_id, const char** reward, VillainGroupEnum vgroup, int baseLevel, int levelAdjust, ScriptVarsTable* vars)
{
    LOG_DBID( db_id, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Mission]:Rcv %s", 
        dbPlayerNameFromId(db_id), ScriptVarsTableLookup(vars, reward[0]));
    rewardStoryApplyToDbID(db_id, reward, vgroup, baseLevel, levelAdjust, vars, REWARDSOURCE_MISSION);
}

// Part of reseeding is reseting the side objectives
void MissionReseed(void)
{
    if (g_activemission)
    {
        g_activemission->seed = dbSecondsSince2000();
        g_activemission->completeSideObjectives = 0;
        MissionObjectiveResetSideMissions();
    }
}

// mark the currently active mission as complete
void MissionSetComplete(int success)
{
    char buf[1000], timeStr[1000];
    const char *mapStr;
    char* exittext = NULL;
    Entity * e = NULL;
    int i;
    int mapLength;

    if (!OnMissionMap())
        return;
    if (!g_activemission) 
        return;
    if (g_activemission->completed) 
        return;
    // MAK - g_activemission may not be fully set up yet!  it only has ID numbers (by design)

    if( server_state.doNotCompleteMission )
        return;
    if( db_state.is_endgame_raid )
    {
        return;
    }

    // mark the mission and team as complete
    g_activemission->completed = 1;
    g_activemission->success = success;
    if (success)
    {
        log_printf("missiondoor.log", "Map %i Mission Complete", db_state.map_id);
        printf("Mission Completed\n");
    }
    else
    {
        log_printf("missiondoor.log", "Map %i Mission Failed", db_state.map_id);
        printf("Mission Failed\n");
    }

    // print out db log message
    if (g_activemission->task) // only if mission was actually initialized and had some players on it
    {
        e = entFromDbId(g_activemission->ownerDbid);
        if(success)
            strcpy(buf, "Mission:Success");
        else
            strcpy(buf, "Mission:Failure");

        timerMakeOffsetStringFromSeconds(timeStr, (dbSecondsSince2000() - g_activemission->starttime));
        mapStr = StaticDefineIntRevLookup(ParseMapSetEnum, g_activemission->def->mapset);
        mapLength = g_activemission->def->maplength;
        LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "%s Name: %s %s, Time to Complete: %s (%s %s %d)", 
            buf,
            g_activemission->task->filename,
            g_activemission->task->logicalname,
            timeStr,
            db_state.map_name,
            mapStr,
            mapLength);

        // send messages to owner about each objective reached
        for (i = 0; i < MAX_OBJECTIVE_INFOS; i++)
        {
            if (!g_activemission->completeObjectives[i].set) 
                break;
            if (!(g_activemission->completeObjectives[i].compoundPos == g_activemission->sahandle.compoundPos))
                continue;

            sprintf(buf, "objectivecompleteinternal %%i %i %i %i %i %i %i", SAHANDLE_ARGSPOS(g_activemission->sahandle),
                g_activemission->completeObjectives[i].num, g_activemission->completeObjectives[i].success);
            MissionSendToOwner(buf);
        }

        // send rewards to active team members
        MissionRewardActivePlayers(success);

        // "Send" the exit mission text to all the players
        if (e && e->teamup)
        {
            int i;
            for(i=0; i<e->teamup->members.count; i++)
            {
                Entity* teammate = entFromDbId(e->teamup->members.ids[i]);
                if (teammate && (teammate->map_id == db_state.map_id))
                {
                    StoryInfo* teammateinfo = StoryArcLockPlayerInfo(teammate);
                    teammateinfo->exitMissionSuccess = success;
                    StoryHandleCopy( &teammateinfo->exithandle, &g_activemission->sahandle );
                    teammateinfo->exitMissionOwnerId = g_activemission->ownerDbid;
                    StoryArcUnlockPlayerInfo(teammate);
                }
            }
        }
    }

    // send message to owner telling him to close task
    sprintf(buf, "taskcompleteinternal %%i %i %i %i %i %i", SAHANDLE_ARGSPOS(g_activemission->sahandle), success);
    MissionSendToOwner(buf);

    // SGMission, restore the owner so that it knows to complete the teamups properly
    if (g_activemission->supergroupDbid)
    {
        assert(!db_state.is_endgame_raid);    //    end game raids should not have their owner changed
                                            //    trying to catch the culprit
        if (!db_state.is_endgame_raid)
        {
            g_activemission->ownerDbid = g_activemission->supergroupDbid;
        }
    }

    // send message to teamup telling them that mission is complete
    strcpy(buf, success? DBMSG_MISSION_COMPLETE: DBMSG_MISSION_FAILED);
    strcat(buf, MissionConstructIdString(g_activemission->ownerDbid, &g_activemission->sahandle));
    MissionTeamupListBroadcast(buf);

    if (!success)
        MissionShutdown("MissionSetComplete() mission failed");
}

// *********************************************************************************
//  Global hooks, and shutting down the mission
// *********************************************************************************

// hook into global loop
void MissionProcess()
{
    if ((OnMissionMap() && g_activemission) || ScriptGlowiesPlaced())
        MissionItemProcessInteraction();

    if (!OnMissionMap() || !g_activemission || db_state.server_quitting) 
        return;

    MissionObjectiveCheckComplete();
    MissionCheckKickedPlayers();
    MissionCountCheckPlayersLeft();

    // if on a local mission, the g_activemission can be closed here
    if (!g_activemission)
        return;

    // check mission timeout
    if (g_activemission->timeout && g_activemission->failOnTimeout == true
        && g_activemission->timerType == -1 && g_activemission->timeout < dbSecondsSince2000() 
        && (!g_activemission->completed || TASK_FORCE_TIMELIMIT(g_activemission->task)))
    {
        if (!g_activemission->completed)
            MissionSetComplete(0);
        else
            MissionShutdown("MissionProcess() mission timeout");
    }
}

// called when the first player logs onto the map
void MissionInitMap()
{
    Entity* player;

    player = firstActivePlayer();
    if (player)
    {
        MissionInitInfo(player);

        if (g_activemission)
            LOG_ENT_OLD(player,"Mission:Init %s)", g_activemission->task->logicalname);
    }
    else
    {
        log_printf("missiondoor.log", "Map %i, MissionInitMap called without a player logged on??", db_state.map_id);
    }
}

int OnMissionMap(void)
{
    return g_forcemissionload || g_activemission;
}

int OnPlayerCreatedMissionMap(void)
{
    return (g_ArchitectTaskForce || (g_activemission && g_activemission->sahandle.bPlayerCreated));
}

int OnInstancedMap(void)
{
    return OnMissionMap() || OnArenaMap() || OnSGBase();
}

int MissionAllowBaseHospital(void)
{
    return !g_activemission || !g_activemission->def || !(g_activemission->def->flags & MISSION_NOBASEHOSPITAL);
}

int OnPraetorianTutorialMission(void)
{
    if (g_activemission && g_activemission->def)
        return (g_activemission->def->flags & MISSION_PRAETORIAN_TUTORIAL) != 0;
    return 0;
}


// returns true if the players have completed the mission
int MissionCompleted(void)
{
    if (OnMissionMap() && g_activemission && g_activemission->completed)
        return 1;
    return 0;
}

// returns true if the players have completed the mission successfully
int MissionSuccess(void)
{
    if (OnMissionMap() && g_activemission && g_activemission->completed && g_activemission->success)
        return 1;
    return 0;
}


int MissionPlayerAllowedTeleportExit(void)
{
    return (MissionCompleted() && !(g_activemission->def->flags & MISSION_NOTELEPORTONCOMPLETE)) || IncarnateTrialComplete();
}

// map is being requested to shut down by dbserver (no teams left)
void MissionRequestShutdown()
{
    char buf[300];

    if (!g_activemission || !g_activemission->initialized || !g_activemission->ownerDbid)
    {
        MissionShutdown("MissionRequestShutdown()");
        return;
    }

    // send request to owner of mission
    log_printf("missiondoor.log", "Map %i requesting shutdown from owner %s", db_state.map_id, dbPlayerNameFromId(g_activemission->ownerDbid));
    sprintf(buf, "missionrequestshutdown %i %%i %i %i %i %i", db_state.map_id, SAHANDLE_ARGSPOS(g_activemission->sahandle));
    MissionSendToOwner(buf);
}

void MissionShutdown(const char *msg)
{
    char buf[300];

    if (OnMissionMap() && g_activemission && g_activemission->initialized && g_activemission->ownerDbid)
    {
        // notify the team
        log_printf("missiondoor.log", "Map %i forcibly shutting down", db_state.map_id);
        sprintf(buf, "missionforceshutdown %i %%i %i %i %i %i", db_state.map_id, SAHANDLE_ARGSPOS(g_activemission->sahandle) );
        MissionSendToOwner(buf);

        strcpy(buf, DBMSG_MISSION_STOPPED_RUNNING);
        strcat(buf, MissionConstructIdString(g_activemission->ownerDbid, &g_activemission->sahandle));
        MissionTeamupListBroadcast(buf);
    }

    turnstileMapserver_completeInstance();
    // Can't shutdown the server in local mapserver, just have the player exit
    if (!db_state.local_server)
        dbAsyncReturnAllToStaticMap("MissionShutdown()",msg);
    else
        exitLocalMission(NULL);
}

// Mission Waypoint System for sending locations within a mission
static unsigned int uniqueWaypointID = 0;

// Sends a mission waypoint to the player
static void MissionWaypointSend(Entity* player, Packet* pak, MissionObjectiveInfo* objectiveInfo, int newWaypoint)
{
    Entity* objectiveEnt = erGetEnt(objectiveInfo->objectiveEntity);
    Vec3 objectiveLoc;
    if (objectiveEnt)
        copyVec3(ENTPOS(objectiveEnt), objectiveLoc);
    else if (objectiveInfo->encounter)
        copyVec3(objectiveInfo->encounter->mat[3], objectiveLoc);
    else
        return;
    
    pktSendBits(pak, 1, 1); // Sending a waypoint
    pktSendBitsPack(pak, 1, objectiveInfo->missionWaypointID);
    if (newWaypoint)
    {
        pktSendBitsAuto(pak, timerSecondsSince2000());
        pktSendF32(pak, objectiveLoc[0]);
        pktSendF32(pak, objectiveLoc[1]);
        pktSendF32(pak, objectiveLoc[2]);
    }
}

// Send all the mission waypoints to the player
void MissionWaypointSendAll(Entity* player)
{
    int i;
    START_PACKET(pak, player, SERVER_MISSION_WAYPOINT)
    pktSendBits(pak, 1, 1); // 1 signifies these are new waypoints
    for (i = 0; i < eaSize(&objectiveInfoList); i++)
        if (objectiveInfoList[i]->missionWaypointID)
            MissionWaypointSend(player, pak, objectiveInfoList[i], 1);
    pktSendBits(pak, 1, 0); // No more incoming waypoints
    END_PACKET
}

// After a group is completed, this function tells the players to get rid of the waypoint
void MissionWaypointDestroy(MissionObjectiveInfo* objectiveInfo)
{
    if (OnMissionMap() && objectiveInfo->missionWaypointID)
    {
        PlayerEnts *players = &player_ents[PLAYER_ENTS_ACTIVE];
        int i;

        // Let all players know that the waypoint needs to be removed
        for (i = 0; i < players->count; i++)
        {
            if (players->ents[i])
            {
                START_PACKET(pak, players->ents[i], SERVER_MISSION_WAYPOINT)
                pktSendBits(pak, 1, 0); // 0 signifies the waypoints are being deleted
                MissionWaypointSend(players->ents[i], pak, objectiveInfo, 0);
                pktSendBits(pak, 1, 0); // No more incoming waypoints
                END_PACKET
            }
        }

        // Clear the unique id for the waypoint so that it is no longer sent to the client
        objectiveInfo->missionWaypointID = 0;
    }
}

// Creates a new mission location and sends it to all the players
void MissionWaypointCreate(MissionObjectiveInfo* objectiveInfo)
{
    if (objectiveInfo->def && (objectiveInfo->def->flags & MISSIONOBJECTIVE_SHOWWAYPOINT))
    {
        PlayerEnts *players = &player_ents[PLAYER_ENTS_ACTIVE];
        int i;

        // Increase the unique id for the mission objective
        if (objectiveInfo->def->flags & MISSIONOBJECTIVE_SHOWWAYPOINT)
            objectiveInfo->missionWaypointID = ++uniqueWaypointID;

        // Now send to all players in the map
        for (i = 0; i < players->count; i++)
        {
            if (players->ents[i])
            {
                START_PACKET(pak, players->ents[i], SERVER_MISSION_WAYPOINT)
                pktSendBits(pak, 1, 1); // 1 signifies these are new waypoints
                MissionWaypointSend(players->ents[i], pak, objectiveInfo, 1);
                pktSendBits(pak, 1, 0); // No more incoming waypoints
                END_PACKET
            }
        }
    }
}

// send the player the mission entry text.  ignored if this isn't a mission map,
// only send information once to each player
void MissionSendEntryText(Entity* player)
{
    MissionInfo* mission;
    const char* entrytext;
    static int* textSentList = NULL;    // static list of db_id's for players we've sent entry text to
    int i, n;

    if (!OnMissionMap()) 
        return;
    mission = MissionInitInfo(player);
    if (!mission) 
        return;
    if (!mission->def) 
        return;

    // keep track of each player that we've sent the intro text to
    if (!textSentList) eaiCreate(&textSentList);

    // see if we've already send text
    n = eaiSize(&textSentList);
    for (i = 0; i < n; i++)
    {
        if (player->db_id == textSentList[i])
            return;
    }

    // otherwise, go ahead and show text
    eaiPush(&textSentList, player->db_id);

    entrytext = saUtilTableAndEntityLocalize(mission->def->entrytext, &mission->vars, player);
    if (entrytext)
    {
        START_PACKET(pak, player, SERVER_MISSION_ENTRY);
        pktSendString(pak, entrytext);
        END_PACKET
    }

    // Turn on/off the flashback sepia-tone too as necessary. Flashback
    // mission maps need the sepia on entry, everywhere else needs it
    // forcibly shut off.
    TaskForceMissionEntryDesaturate(player);
}

// *********************************************************************************
//  Mission pacing
// *********************************************************************************

#define BOOST_LEVEL_TEAMSIZE    9    // (9 == DISABLED) at this team size, we boost the effective level by one
#define NUM_PACING_TAPS            11    // odd so there is a midpoint
int g_levelpacing[NUM_PACING_TYPES][NUM_PACING_TAPS] =
{
      {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // PACING_UNMODIFIABLE "Unmodifiable"
      {  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  0 }, // PACING_FLAT "Flat"
      { -2, -1,  0,  0,  1,  0,  1, -1, -1,  1,  0 }, // PACING_SLOWRAMPUP "SlowRampUp"
      {  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  0 }, // PACING_SLOWRAMPDOWN "SlowRampDown"
      {  0,  1,  0, -1,  0,  0,  1,  0,  0,  1,  0 }, // PACING_STAGGERED "Staggered"
      {  0,  0,  1,  0,  0,  0,  1,  0,  1,  1,  0 }, // PACING_FRONTLOADED "FrontLoaded"
      { -2, -2, -1,  0, -2,  0,  0,  1,  1,  1,  0 }, // PACING_BACKLOADED "BackLoaded"
      {  0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0 }, // PACING_HIGHNOTORIETY "HighNotoriety"
    //{  1,  1,  1,  1, -1,  0,  0,  0, -1, -2, -3 }, // PACING_SLOWRAMPDOWN "SlowRampDown"
    //{  1,  1,  1,  1,  0,  0,  0, -1, -2, -2, -3 }, // PACING_FRONTLOADED "FrontLoaded"
};

int g_teampacing[NUM_PACING_TYPES][NUM_PACING_TAPS] =
{
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // PACING_UNMODIFIABLE "Unmodifiable"
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // PACING_FLAT "Flat"
    {  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // PACING_SLOWRAMPUP "SlowRampUp"
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // PACING_SLOWRAMPDOWN "SlowRampDown"
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // PACING_STAGGERED "Staggered"
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // PACING_FRONTLOADED "FrontLoaded"
    {  1,  1,  0,  0,  1,  0,  0,  0,  0,  0,  0 }, // PACING_BACKLOADED "BackLoaded"
    {  1,  0,  0,  0,  0,  1,  0,  0,  0,  0,  1 }, // PACING_HIGHNOTORIETY "HighNotoriety"
    //{  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2 }, // PACING_SLOWRAMPDOWN "SlowRampDown"
    //{  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  2 }, // PACING_FRONTLOADED "FrontLoaded"
};

// get an appropriate tap given the room pace and global extents
int PacingGetTap(int roompace)
{
    double d;
    int tap;

    if (g_lowpace == g_highpace) return NUM_PACING_TAPS / 2;    // degenerate case
    d = (double)(roompace - g_lowpace) / (double)(g_highpace - g_lowpace);
    tap = (int)floor(d * NUM_PACING_TAPS);
    return CLAMP(tap, 0, NUM_PACING_TAPS-1);
}

int PacingGetLevel(int baselevel, int pacetype, int roompace, int teamsize)
{
    int leveladj = g_levelpacing[pacetype][PacingGetTap(roompace)];
    if (teamsize >= BOOST_LEVEL_TEAMSIZE) leveladj++;
    baselevel += leveladj;
    if (g_ArchitectTaskForce)
    {
        return CLAMP(baselevel, CLAMP(gArchitectMapMinLevel+1, 1, 54), 
                                CLAMP(gArchitectMapMaxLevel+1, 1, 54));
    }
    else
    {
        return CLAMP(baselevel, 1, MAX_VILLAIN_LEVEL);
    }
}

int PacingGetTeam(int baseteam, int pacetype, int roompace)
{
    int teamadj = g_teampacing[pacetype][PacingGetTap(roompace)];
    baseteam += teamadj;
    return CLAMP(baseteam, 1, MAX_TEAM_MEMBERS);
}

// encounters will be spawned at the max of:
//  # players on map
//  # players in largest team on map
int MissionNumHeroes(void)
{
    int count, numonmap, i;
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];

    numonmap = players->count;
    count = 1;
    for (i = 0; i < players->count; i++)
    {
        // don't count admins spying as players on the map
        if (players->ents[i]->admin)
        {
            numonmap--;
            continue;
        }

        if (players->ents[i] &&
            players->ents[i]->teamup_id &&
            players->ents[i]->teamup->members.count > count)
        {
            count = players->ents[i]->teamup->members.count;
        }

        // In the case of TFs, set the spawn to the number of players still on the TF, no matter if they
        //        are logged in or not.
        if (players->ents[i] &&
            players->ents[i]->taskforce &&
            players->ents[i]->taskforce->members.count > count)
        {
            count = players->ents[i]->taskforce->members.count ;
        }
    }
    if (numonmap > count) 
        count = numonmap;
    if (g_encounterForceTeam > 0) 
        return g_encounterForceTeam;    // debug hack
    return CLAMP(count, 1, MAX_TEAM_MEMBERS);
}

// *********************************************************************************
//  Encounter hooks
// *********************************************************************************

// immediately before the encounter is spawned
int MissionSetupEncounter(EncounterGroup* group, Entity* player)
{
     MissionInfo* info;
    RoomInfo* roominfo;
    int level, numheroes;

    if (!OnMissionMap())
        return 0;

    info = MissionInitInfo(player);
    roominfo = RoomGetInfoEncounter(group);

    // if we couldn't load current mission info, or we can't find a nearby player
    // just ignore this encounter for now
    // (this shouldn't happen very often)
    if (!info || !info->def)
    {
        group->state = EG_OFF;
        group->conquered = 1;
        LOG_OLD("MissionSetupEncounter: had to ignore encounter due to missing player");
        return 1;
    }
    
    // is this a random mission spawn or one we've overridden?
    if (!group->missiongroup && !group->missionspawn)
        return 0; // let a normal spawn happen

    // decide the level & number of heroes
    level = PacingGetLevel(info->missionLevel, info->missionPacing, roominfo->roompace, MAX( info->difficulty.teamSize, MissionNumHeroes()) );
    numheroes = PacingGetTeam(MAX( info->difficulty.teamSize, MissionNumHeroes()), info->missionPacing, roominfo->roompace);

    // decide if we are doing boss reduction
    if (MissionNumHeroes() == 1 && !info->difficulty.dontReduceBoss && 
        (!info->taskforceId || info->flashback || info->scalableTf) )
    {
        group->active->bossreduction = 1;
    }

    // set up the encounter
    if (group->missionspawn) // get spawn from mission definition
    {
        group->active->spawndef = group->missionspawn;
    }
    else    // or just get a random one
    {
        if (roominfo == g_rooms[0]) // not in a tray
        {                                                                     
            group->state = EG_OFF;
            group->conquered = 1;
            return 1;
        }

        // get a generic from the mission or spawn tables
        group->active->spawndef = MissionSpawnReplaceGeneric(g_activemission->def);
        if (!group->active->spawndef)
            group->active->spawndef = MissionSpawnGet(info->missionGroup, level);
    }

    // Acceptable because we call ScriptVarsTableClear in EncounterActiveInfoFree
    ScriptVarsTableCopy(&group->active->vartable, &info->vars);
    ScriptVarsTablePushScope(&group->active->vartable, &group->active->spawndef->vs);

    group->active->seed = info->seed;
    group->active->mustspawn = 1;

    // attach to the right point
    if (group->active->spawndef->spawntype)
    {
        const char* spawntype = ScriptVarsTableLookup(&group->active->vartable, group->active->spawndef->spawntype);
        group->active->point = EncounterGetMatchingPoint(group, spawntype);
    }
    else
        group->active->point = EncounterGetMatchingPoint(group, "Mission");
    if (group->active->point == -1) // hostage spawns will be like this
    {
        group->active->point = rand() % group->children.size;
    }

    // let the encounter override the level
    if (group->active->spawndef->villainminlevel && group->active->spawndef->villainmaxlevel)
    {
        int minlevel = group->active->spawndef->villainminlevel;
        int levelrange = group->active->spawndef->villainmaxlevel - minlevel + 1;
        level = minlevel + rand() % levelrange;
        level = CLAMP(level, 1, MAX_VILLAIN_LEVEL);
    }
    group->active->villainlevel = level;
    group->active->numheroes = numheroes;

    // set the villain group
    group->active->villaingroup = info->missionGroup;

    if (group->objectiveInfo)
        MissionItemSetState(group->objectiveInfo, MOS_INITIALIZED);

    // Flag the encounter the same as the mission if spawndef does not specify
    group->active->allyGroup = group->active->spawndef->encounterAllyGroup;
    if(group->active->allyGroup == ENC_UNINIT)
        group->active->allyGroup = info->missionAllyGroup;

    return 1;
}

// let the mission adjust the level, team size for encounter if desired
void MissionSetEncounterLevel(EncounterGroup* group, Entity* player)
{
    RoomInfo* roominfo;
    MissionInfo* info;

    if (!OnMissionMap()) 
        return;

    roominfo = RoomGetInfoEncounter(group);
    info = MissionInitInfo(player);
    if (!info || !info->def) 
    {
        group->state = EG_OFF;
        group->conquered = 1;
        LOG_OLD("MissionSetEncounterLevel: had to ignore encounter due to missing player");
        return; // shouldn't happen very often
    }
    group->active->villainlevel = PacingGetLevel(info->missionLevel, info->missionPacing, roominfo->roompace, MAX( info->difficulty.teamSize, MissionNumHeroes()) );
    group->active->numheroes = PacingGetTeam(MAX( info->difficulty.teamSize, MissionNumHeroes()), info->missionPacing, roominfo->roompace);
}

extern int inLocalMission; // from door.c

// an encounter was completed
// Mark any objectives associated with this encounter group as complete.
void MissionEncounterComplete(EncounterGroup* group, int success)
{
    Entity* objective = NULL;
    Entity* player = NULL;
    MissionObjectiveInfo* info;
    if(!OnMissionMap())
        return;

    info = group->objectiveInfo;
    if(!info)
        return;

    if (db_state.local_server && !inLocalMission)
        return;

    player = erGetEnt(group->active->whokilledme);
    if (!player) 
        player = firstActivePlayer();
    MissionObjectiveComplete(player, info, success); // okay if player == NULL
}

// WARNING! The caller does not take ownership of the returned strings.
void MissionHostageRename(EncounterGroup* group, char** displayname, const char** model)
{
#define BUFFERSIZE 1000
    static char hostageName[BUFFERSIZE];
    MissionObjectiveInfo* info;
    info = group->objectiveInfo;

    if(!info || !OnMissionMap() || !info->def)
        return;

    // Assume that there is only one NPC in the encounter and that it is the hostage.
    // Assign a name to the hostage.
    if(info->def->modelDisplayName && displayname)
    {
        const char* str = saUtilTableLocalize(info->def->modelDisplayName, &g_activemission->vars);
        strncpy(hostageName, str, BUFFERSIZE);
        *displayname = hostageName;
    }

    // Assing a model to the hostage.
    if(info->def->model && model)
    {
        *model = ScriptVarsTableLookup(&g_activemission->vars, info->def->model);
    }
#undef BUFFERSIZE
}

// Hook for when encounter is transitioning an actor.  Any dialog returned will be
// said by the actor.  The default dialog is passed into this function and may be
// returned without adjustment.
const char* MissionAdjustDialog(const char* dialog, EncounterGroup* group, Entity* ent, const Actor* a, ActorState tostate, ScriptVarsTable* vars)
{
    MissionObjectiveInfo* info = group->objectiveInfo;
    if (!info || !g_activemission || !info->def)
        return dialog;

    if (tostate == ACT_THANK && info->def->interactCompleteString)
    {
        // "Rescuer" filled in by encounter now
        dialog = saUtilTableAndEntityLocalize(info->def->interactCompleteString, vars, ent);
    }
    return dialog;
}

// *********************************************************************************
//  Mission debugging
// *********************************************************************************

// Assign player a random mission of an appropriate stature level
void MissionAssignRandom(ClientLink * client)
{
    int i,k,m,n, level;
    const ContactDef **pContacts = g_contactlist.contactdefs;
    const StoryTask **missions = NULL;

    // MAK - I don't really understand the following.  Even in the days before 
    // SL 2.5 a simple division wasn't going to get you a stature level.  I'm not
    // going to use it and instead just select contacts based on their level ranges.
    /*

    // compute stature level from security level (security 1-5 == stature 1, 5-10 == 2, etc)
    int iStatureLevel = (client->entity->pchar->iLevel + 5) / 5;
    static int max = 0;  // max stature level available is computed only once



    // compute the max available stature (first time only)
    if(! max)
    {
        // get the max available stature level
        m = eaSize(&pContacts);
        for (i = 0; i < m; i++)
        {
            int s = pContacts[i]->stature;
            if(s > max
               && s < 99) // designers use high numbers like '99' for some reason.
            {
                max = s;
            }
        }
    }

    // if player's stature level is too high, use the max available one
    if(iStatureLevel > max)
    {
        conPrintf(client, "There are no available missions for your stature level (%d), so using level %d", iStatureLevel, max);
        iStatureLevel = max;
    }
    */


    eaCreateConst(&missions);

    // build up a list of all missions at your (player's) experience level
    level = character_GetExperienceLevelExtern(client->entity->pchar);
    m = eaSize(&pContacts);
    for (i = 0; i < m; i++)
    {
        if (pContacts[i]->stature >= MIN_STANDARD_CONTACTLEVEL &&
            pContacts[i]->stature <= MAX_STANDARD_CONTACTLEVEL &&
            pContacts[i]->minPlayerLevel <= level &&
            pContacts[i]->maxPlayerLevel >= level)
        {
            n = eaSize(&pContacts[i]->tasks);
            for(k = 0; k < n; k++)
            {
                if(   pContacts[i]->tasks[k]->logicalname
                   && TASK_MISSION == pContacts[i]->tasks[k]->type)
                {
                    eaPushConst(&missions, pContacts[i]->tasks[k]);
                }
            }
        }
    }

    if( ! eaSize(&missions))
    {
        conPrintf(client, "There are no available missions for your experience level (%d)", level);
    }
    else
    {
        conPrintf(client, "Available Experience Level %d Missions: %d", level, eaSize(&missions));
        i = rand() % eaSize(&missions);
        TaskForceGetUnknownContact(client, missions[i]->logicalname);
    }

    eaDestroyConst(&missions);
}

// Assign player a random mission of an appropriate stature level
// but only select from choices that would make the player transfer to a
// a mission instance map, used by the 'missionxmap' and test clients
// to generate server load
void MissionAssignRandomWithInstance(ClientLink * client)
{
    int i,k,m,n, level;
    const ContactDef **pContacts = g_contactlist.contactdefs;
    const StoryTask **missions = NULL;


    eaCreateConst(&missions);

    // build up a list of all missions at your (player's) experience level
    level = character_GetExperienceLevelExtern(client->entity->pchar);
    m = eaSize(&pContacts);
    for (i = 0; i < m; i++)
    {
        if (pContacts[i]->stature >= MIN_STANDARD_CONTACTLEVEL &&
            pContacts[i]->stature <= MAX_STANDARD_CONTACTLEVEL &&
            pContacts[i]->minPlayerLevel <= level &&
            pContacts[i]->maxPlayerLevel >= level)
        {
            n = eaSize(&pContacts[i]->tasks);
            for(k = 0; k < n; k++)
            {
                const StoryTask* st = pContacts[i]->tasks[k];
                if( st->logicalname && TASK_MISSION == st->type )
                {
                    const MissionDef* mdef = NULL;
                    if (st->missionref) 
                        mdef = st->missionref;
                    if (eaSize(&st->missiondef)) 
                        mdef = st->missiondef[0];
                    if (mdef && ((mdef->mapfile && *mdef->mapfile)))
                    {
                        eaPushConst(&missions, st);
                    }
                }
            }
        }
    }

    if( ! eaSize(&missions))
    {
        conPrintf(client, "There are no available mission with map instances for your experience level (%d)", level);
    }
    else
    {
        conPrintf(client, "Available Experience Level %d Instance Missions: %d", level, eaSize(&missions));
        i = rand() % eaSize(&missions);
        TaskForceGetUnknownContact(client, missions[i]->logicalname);
    }

    eaDestroyConst(&missions);
}

StoryTaskInfo *MissionGetCurrentMission(Entity *e)
{
    StoryTaskInfo * pSTI;

    if (!e)
        return NULL;

    if (e->teamup_id && e->teamup->activetask->assignedDbId)
    {
        pSTI = e->teamup->activetask;
    }
    else if (e->supergroup && e->supergroup->activetask)
    {
        pSTI = e->supergroup->activetask;
    }
    else
    {
        pSTI = TaskGetMissionTask(e, 0);
    }
    if (pSTI && pSTI->def && pSTI->def->type != TASK_MISSION)
        pSTI = NULL; // player isn't on mission subtask yet

    return pSTI;
}

bool MissionCanGetToMissionMap(Entity *e, StoryTaskInfo * pSTI)
{
    const MapSpec* mapSpec;
    DoorEntry *pEntry;
    int characterLevel = character_GetSecurityLevelExtern(e->pchar); // +1 since iLevel is zero-based.

    if (!pSTI)
        return true;

    pEntry = dbFindDoorWithDoorId(pSTI->doorId);

    if (!pEntry)
        return true;

    mapSpec = MapSpecFromMapId(pEntry->map_id);
    if(    !mapSpec ||
        (!ENT_IS_ROGUE(e) && ENT_IS_HERO(e) && !MAP_ALLOWS_HEROES(mapSpec)) ||
        (!ENT_IS_ROGUE(e) && ENT_IS_VILLAIN(e) && !MAP_ALLOWS_VILLAINS(mapSpec)) ||
        (pEntry->map_id == 16 && ENT_IS_VILLAIN(e)) || // Rogues can't go to Hero Ouro
        (pEntry->map_id == 17 && ENT_IS_HERO(e))) // Vigilantes can't go to Villain Ouro
    {
        return false;
    }


    if (characterLevel < mapSpec->entryLevelRange.min)
    {
        return false;
    }
    else if (characterLevel > mapSpec->entryLevelRange.max)
    {
        return false;
    } 
    else if ((stricmp(mapSpec->mapfile, "city_00_01.txt") == 0) ||        // can't go to tutorial zones
        (stricmp(mapSpec->mapfile, "v_city_00_01.txt") == 0))
    {
        return false;
    }

    return true;
}

static DoorEntry *findMissionDoor(StoryTaskInfo *task)
{
    DoorEntry *result = dbFindDoorWithDoorId(task->doorId);

    if (!result && task->def)
    {
        const MissionDef *pDef = task->def->missionref;

        if (pDef == NULL)
            pDef = *(task->def->missiondef);

        if (pDef->doorNPC)
            result = dbFindDoorWithName(DOORTYPE_PERSISTENT_NPC, pDef->doorNPC);
    }

    return result;
}

bool MissionGotoMissionDoor(ClientLink * client)
{
    Entity * e = client->entity;
    F32 * pos;
    char cmd[1000];
    StoryTaskInfo * pSTI;
    const MapSpec* mapSpec;

    if (e->teamup_id && e->teamup->members.count != 1
        && !e->teamup->activetask->assignedDbId)
    {
        // team leader hasn't selected an active task yet, don't teleport.
        sendInfoBox(e, INFO_USER_ERROR, "TeleportNoMission");
        return false;
    }

    pSTI = MissionGetCurrentMission(e);

    if(pSTI)
    {
        DoorEntry * pEntry = findMissionDoor(pSTI);
        int mapID;
        LWC_STAGE mapStage;

        if( ! pEntry)
        {
            // check to see if this is a train mission
            if (TaskIsZoneTransfer(pSTI))
            {
                if (areMembersAlignmentMixed(e->teamup ? &e->teamup->members : NULL) ||
                    areMembersUniverseMixed(e->teamup ? &e->teamup->members : NULL))
                {
                    conPrintf(client, saUtilLocalize(e,"MissionMixedTeam") );
                    return false;
                } else {
                    EnterMission(e, pSTI);
                    return true;
                }
            }

            conPrintf(client, saUtilLocalize(e,"MissionBadDoorId") );
            return false;
        }

        pos = pEntry->mat[3];
        mapID = pEntry->map_id;

        if (!LWC_CheckMapIDReady(mapID, LWC_GetCurrentStage(e), &mapStage))
        {
            sendDoorMsg(e, DOOR_MSG_FAIL, localizedPrintf(e, "LWCDataNotReadyMap", mapStage));
            return false;
        }
        if (e->access_level < 1 && !db_state.local_server)
        {
            int characterLevel = character_GetSecurityLevelExtern(e->pchar); // +1 since iLevel is zero-based.

            mapSpec = MapSpecFromMapId(mapID);
            if(    !mapSpec ||
                (!ENT_IS_ROGUE(e) && ENT_IS_HERO(e) && !MAP_ALLOWS_HEROES(mapSpec)) || 
                (!ENT_IS_ROGUE(e) && ENT_IS_VILLAIN(e) && !MAP_ALLOWS_VILLAINS(mapSpec)) ||
                (ENT_IS_IN_PRAETORIA(e) && !MAP_ALLOWS_PRAETORIANS(mapSpec)) ||
                (!ENT_IS_IN_PRAETORIA(e) && !MAP_ALLOWS_PRIMAL(mapSpec)))
            {
                conPrintf(client, saUtilLocalize(e, "MissionBadZone") );
                return false;
            }

            if (characterLevel < mapSpec->entryLevelRange.min)
            {
                conPrintf(client, localizedPrintf(e, "TooLowToEnter", 
                                                    mapSpec->entryLevelRange.min) );
                return false;
            }
            else if (characterLevel > mapSpec->entryLevelRange.max)
            {
                conPrintf(client, localizedPrintf(e, "TooHighToEnter", 
                                                    mapSpec->entryLevelRange.min) );
                return false;
            } 
            else if ((stricmp(mapSpec->mapfile, "city_00_01.txt") == 0) ||        // can't go to tutorial zones
                        (stricmp(mapSpec->mapfile, "v_city_00_01.txt") == 0))
            {
                conPrintf(client, saUtilLocalize(e, "MissionBadZone") );
                return false;
            }

        }

        if(mapID == db_state.base_map_id)
            mapID = db_state.map_id;
        sprintf(cmd,"mapmovepos \"%s\" %d %f %f %f 0 1", e->name, mapID, pos[0], pos[1], pos[2]);
        serverParseClientEx(cmd, client, ACCESS_INTERNAL);
        return true;
    }
    else
    {
        conPrintf(client, saUtilLocalize(e,"NoActiveMission"));
        return false;
    }
}


//For scripts who just want a generic spawn form this mission
const SpawnDef * getGenericMissionSpawn(void)
{
    int level;
    const SpawnDef * spawndef;

    if( !g_activemission )
        return NULL;

    level = PacingGetLevel(g_activemission->missionLevel, PACING_UNMODIFIABLE, 0, MAX( g_activemission->difficulty.teamSize, MissionNumHeroes()));
    spawndef = MissionSpawnGet(g_activemission->missionGroup, level);

    return spawndef;
}

// For scripts to find a specific spawndef in this mission by the logical name
const SpawnDef * getMissionSpawnByLogicalName(const char *logicalName)
{
    const SpawnDef * spawndef;
    int count, i;

    if( !g_activemission )
        return NULL;

    // go through all of the custom spawndefs that were included in this mission
    count = eaSize(&g_activemission->def->spawndefs);
    for (i = 0; i < count; i++)
    {
        spawndef = g_activemission->def->spawndefs[i];

        // sanity check 
        if (spawndef == NULL)
            return NULL;

        if (stricmp(logicalName, spawndef->logicalName) == 0)
            return spawndef;

    }


    return NULL;
}

void mission_StoreCurrentServerMusic(const char *musicName, float volumeLevel)
{
    int scriptIndex;
    ScriptEnvironment *temp = g_currentScript;

    if (!g_activemission || (musicName && !strstriConst(musicName, "_loop")))
        return;

    if (g_activemission->currentServerMusicName)
    {
        free(g_activemission->currentServerMusicName);
    }

    if (musicName)
    {
        g_activemission->currentServerMusicName = strdup(musicName);
    }
    else
    {
        g_activemission->currentServerMusicName = 0;
    }
    g_activemission->currentServerMusicVolume = volumeLevel;

    for (scriptIndex = eaSize(&g_runningScripts) - 1; scriptIndex >= 0; scriptIndex--)
    {
        if (g_runningScripts[scriptIndex] && stricmp(g_runningScripts[scriptIndex]->function->name, "ScriptedMissionEvent"))
        {
            g_currentScript = g_runningScripts[scriptIndex];
            ScriptedZoneEvent_SetCurrentBGMusic(musicName, volumeLevel);
        }
    }

    g_currentScript = temp;
}