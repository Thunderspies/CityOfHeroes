/*\
 *
 *    scripthookmission.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *        essentially a list of hooks into game systems.
 *
 *    This file contains all functions related to missions
 *
 */

#include "script/script.h"
#include "script/scriptengine.h"
#include "script/scriptutil.h"

#include "storyarc/storyarcprivate.h"
#include "generator/encounterPrivate.h"
#include "ai/entai.h"
#include "ai/entaiScript.h"
#include "ai/entaivars.h"
#include "ai/entaiprivate.h"
#include "svr/svr_player.h"
#include "entity/entPlayer.h"
#include "entity/entGameActions.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_target.h"
#include "entity/character_tick.h"
#include "entity/mapHistory.h"
#include "cmdparse/cmdserver.h"
#include "dbcomm/staticMapInfo.h"
#include "door.h"
#include "gameSys/Turnstile.h"

#include "script/Scripthook/ScriptHookInternal.h"

// *********************************************************************************
//  Missions
// *********************************************************************************

void SetMissionStatus(STRING localizedStatus)
{
    MissionSetOverrideStatus(localizedStatus);
}

void SetMissionStatusWithCount(NUMBER count, STRING localizedStatus)
{    
    char buf[256];
    sprintf(buf, "%d ", count);
    strcat(buf, saUtilScriptLocalize(localizedStatus));
    MissionSetOverrideStatus(buf);
}

void SetMissionComplete(NUMBER success)
{
    if (!OnMissionMap())
        ScriptError("SetMissionComplete called outside of a mission");
    else
        MissionSetComplete(success);
}

int MissionIsComplete()
{
    if (!OnMissionMap()) {
        ScriptError("MissionIsComplete called outside of a mission");
        return false;
    }
    else
    {
        return MissionCompleted();
    }
}

int MissionIsSuccessful()
{
    if (!OnMissionMap()) {
        ScriptError("MissionIsSuccessful called outside of a mission");
        return false;
    } else {
        return MissionSuccess();
    }
}


int MissionOwnerDBID()
{
    if (!OnMissionMap()) {
        ScriptError("MissionOwnerDBID called outside of a mission");
        return 0;
    } else {
        return g_activemission->ownerDbid;
    }
}

int ObjectiveIsComplete(STRING objective)
{
    if (!OnMissionMap())
    {
        ScriptError("ObjectiveIsComplete called outside of a mission");
        return 0;
    }
    else
        return MissionObjectiveIsComplete(objective, 1);
}

int ObjectivePartialComplete(STRING objective, int numForCompletion)
{ 
    if (!OnMissionMap())
    {
        ScriptError("ObjectivePartialComplete called outside of a mission");
        return 0;
    }
    else
    {
        if (NumObjectivesInState(objective, MOS_COOLDOWN) >= numForCompletion)
        {
            MissionItemGiveEachCompleteString(objective);
            MissionItemStopEachInteraction(objective);
            MissionObjectiveCompleteEach(objective, false, true);
            return 1;
        }
        return 0;
    }
}

int ObjectiveHasFailed(STRING objective)
{
    if (!OnMissionMap())
    {
        ScriptError("ObjectiveIsComplete called outside of a mission");
        return 0;
    }
    else
        return MissionObjectiveIsComplete(objective, 0);
}

NUMBER CheckMissionObjectiveExpression(STRING expression)
{
    char        *required;
    char        *argv[100];
    StringArray expr;
    int            argc;
    int            i;
    NUMBER        retval = 0;

    required = strdup(expression);
    argc = tokenize_line(required, argv, 0);
    if (argc > 0)
    {
        eaCreate(&expr);
        for (i = 0; i < argc; i++)
            eaPush(&expr, argv[i]);

        retval = MissionObjectiveExpressionEval(expr);
    }
    if (required)
        free(required);    

    return retval;
}

// resilient to nulls, considers null and empty the same thing
static int strEqualSafe(const char* lhs, const char* rhs)
{
    if ((!lhs && !rhs) || (!lhs && !rhs[0]) || (!rhs && !lhs[0]))
        return 1;
    if (!lhs || !rhs)
        return 0;
    return strcmp(lhs, rhs) == 0;
}

void CreateMissionObjective(STRING objectiveName, STRING state, STRING singularState)
{
    MissionObjectiveCreate(objectiveName, state, singularState);
}

// SetObjectiveString - sets the objective strings for this objective.
//        state - String to use for plural case for this objective
//        singluarState - String to use for singular case for this objective
//    
//    NOTE: only works on encounter scripts
void SetObjectiveString(STRING state, STRING singularState)
{
    // is this an encounter script
    if (g_currentScript->encounter != NULL)
    {
        // does this encounter have an objective we can modify
        if (g_currentScript->encounter->objectiveInfo)
        {
            int changed = 0;
            if (!strEqualSafe(g_currentScript->encounter->objectiveInfo->description, state))
            {
                changed = 1;
                if (g_currentScript->encounter->objectiveInfo->description)
                    free(g_currentScript->encounter->objectiveInfo->description);
                g_currentScript->encounter->objectiveInfo->description = strdup(state);
            }
            if (!strEqualSafe(g_currentScript->encounter->objectiveInfo->singulardescription, singularState))
            {
                changed = 1;
                if (g_currentScript->encounter->objectiveInfo->singulardescription)
                    free(g_currentScript->encounter->objectiveInfo->singulardescription);
                g_currentScript->encounter->objectiveInfo->singulardescription = strdup(singularState);
            }
            //update display(s) with new status
            if (changed)
                MissionRefreshAllTeamupStatus();
        }
    }
}

void SetNamedObjectiveString(STRING objectiveName, STRING state, STRING singularState)
{
    MissionObjectiveSetTextEach(objectiveName, state, singularState);
}


void ScriptKickFromMap(ENTITY player)
{
    Entity *e = EntTeamInternal(player, 0, NULL);

    leaveMission(e, 0, 0);
}

//TO DO : This: Setting MissionObjectives Randomly Allowed
void SetMissionObjectiveComplete( int status, const char * ObjectiveName )
{
    assertmsg(OnMissionMap(), "Mission objectives should probably be using instanced vars");
    MissionObjectiveCompleteEach(ObjectiveName, true, status);
}


STRING GetShortMapName(void)
{
    char mapbase[MAX_PATH];
    saUtilFilePrefix(mapbase, world_name);
    return StringDupe(mapbase);
}

STRING GetMapName(void)
{
    return StringDupe(world_name);
}

NUMBER GetMapID(void)
{
    return getMapID();
}

NUMBER GetBaseMapID(void)
{
    return db_state.base_map_id;
}

NUMBER GetMapAllowsHeroes(void)
{
    if (OnInstancedMap())
    {
        return true;
    } else {
        const MapSpec* mapSpec = MapSpecFromMapId(db_state.base_map_id);
        if (!mapSpec)
            return true;

        return MAP_ALLOWS_HEROES(mapSpec);
    }
}

NUMBER GetMapAllowsVillains(void)
{
    if (OnInstancedMap())
    {
        return true;
    } else {
        const MapSpec* mapSpec = MapSpecFromMapId(db_state.base_map_id);
        if (!mapSpec)
            return true;

        return MAP_ALLOWS_VILLAINS(mapSpec);
    }
}

NUMBER GetMapAllowsPraetorians(void)
{
    if (OnInstancedMap())
    {
        return true;
    } else {
        const MapSpec* mapSpec = MapSpecFromMapId(db_state.base_map_id);
        if (!mapSpec)
            return true;

        return MAP_ALLOWS_PRAETORIANS(mapSpec);
    }
}


int MissionLevel(void)
{
    if (g_activemission)
        return g_activemission->missionLevel;
    return 0;
}

int FindDialog(STRING name)
{
    int count, i;

    if (g_activemission && g_activemission->task && g_activemission->task->dialogDefs)
    {
        const DialogDef **pDialogDefs = g_activemission->task->dialogDefs;

        count = eaSize(&pDialogDefs);
        for (i = 0; i < count; i++)
        {
            if (!strcmp(name, pDialogDefs[i]->name))
                return i;

        }
    }

    return -1;
}


STRING GetGenericMissionSpawn(void)
{
    const SpawnDef * spawndef;
    spawndef = getGenericMissionSpawn();
    if( !spawndef )
        return 0;

    return StringAdd("", spawndef->fileName );
}

// only works on groups with prefix encounterid:
NUMBER IsGenericMissionSpawn(ENCOUNTERGROUP grp)
{
    EncounterGroup* group = FindEncounterGroupInternal(grp, NULL, 0, 0);

    if (group && group->active && group->active->spawndef)
    {
        return (group->active->spawndef->missionplace == MISSION_REPLACEGENERIC ||
                group->active->spawndef->missionplace == MISSION_ANY);
    } else {
        return false;
    }
}

// time is now in seconds, not minutes!
void SetMissionFailingTimer(NUMBER time)
{
    char startTaskTimerCmd[100];

    if (OnMissionMap() && g_activemission)
    {
        sprintf(startTaskTimerCmd, "taskstarttimer %d %d %d %d", g_activemission->ownerDbid, g_activemission->sahandle.context, g_activemission->sahandle.subhandle, time);
        serverParseClient(startTaskTimerCmd, NULL);
    }
}

// Pass in -1 to timerType for a count-up timer
void SetMissionNonFailingTimer(NUMBER timeLimit, NUMBER timerType)
{
    char startTaskTimerCmd[100];

    if (OnMissionMap() && g_activemission)
    {
        // Encoding a hack to get a fifth variable in
        sprintf(startTaskTimerCmd, "taskstartnofailtimer %d %d %d %d", g_activemission->ownerDbid, g_activemission->sahandle.context, g_activemission->sahandle.subhandle, timeLimit * timerType);
        serverParseClient(startTaskTimerCmd, NULL);
    }
}

void ClearMissionTimer()
{
    char startTaskTimerCmd[100];

    if (OnMissionMap() && g_activemission)
    {
        sprintf(startTaskTimerCmd, "taskcleartimer %d %d %d", g_activemission->ownerDbid, g_activemission->sahandle.context, g_activemission->sahandle.subhandle);
        serverParseClient(startTaskTimerCmd, NULL);
    }
}


static TrialStatus s_TrialStatus = trialStatus_None;

void SetTrialStatus(TrialStatus status)
{
    s_TrialStatus = status;
}

TrialStatus GetTrialStatus()
{
    return s_TrialStatus;
}

int IncarnateTrialComplete()
{
    return s_TrialStatus != trialStatus_None;
}

void SendTrialStatus(TEAM team)
{
    int i;
    int n;
    Entity *e;
    TrialStatus status = GetTrialStatus();

    EntTeamInternal(team, -1, &n);
    for (i = 0; i < n; i++)
    {
        e = EntTeamInternal(team, i, NULL);
        sendIncarnateTrialStatus(e, status);
    }
    if(status != trialStatus_None)
        turnstileMapserver_generateTrialComplete();
}

int HasSouvenirClue(ENTITY entity, STRING clueName)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    if(e && e->storyInfo && clueName)
    {
        const SouvenirClue* clue = scFindByName(clueName);
        if(clue)
        {
            // Does the player already have this clue?
            if(eaFind(&e->storyInfo->souvenirClues, clue) >= 0)
            {
                return true;
            }
        }
    }
    return false;
}

int HasClue(ENTITY entity, STRING clueName)
{
    Entity * e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    if(e && e->storyInfo)
    {
        return CluePlayerHasByName(e, clueName);
    }
    return false;
}