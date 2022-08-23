/*\
 *
 *    scripthookencounter.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *        essentially a list of hooks into game systems.
 *
 *    This file contains all functions related to encounters
 *
 */

#include "script/script.h"
#include "script/scriptengine.h"
#include "script/scriptutil.h"

#include "cmdparse/cmdserver.h"
#include "storyarc/storyarcprivate.h"
#include "generator/encounterPrivate.h"
#include "ai/entai.h"
#include "ai/entaiLog.h"
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
#include "entity/entity.h"
#include "group/group.h"
#include "entity/motion.h"

#include "script/Scripthook/ScriptHookInternal.h"
#include "entity/PCC_Critter.h"

//Internal Functions
void ReserveEncounterInternal(EncounterGroup* group);

// *********************************************************************************
//  Encounters
// *********************************************************************************

static char* EncounterNameInternal(EncounterGroup* group)
{
    char buf[200];
    int handle;

    if (!group) return LOCATION_NONE;

    handle = EncounterGetHandle(group);
    if (!handle)
    {
        ScriptError("Internal error in EncounterNameInternal()");
        return LOCATION_NONE;
    }

    sprintf(buf, "encounterid:%i", handle);
    return StringDupe(buf);
}

EncounterGroup* FindEncounterGroupInternal(AREA area, STRING layout, int inactiveonly, int deserted)
{
    Vec3 vec;

    // degenerate cases
    if (area == NULL)
        return NULL;
    if (stricmp(area, LOCATION_NONE)==0)
        return NULL;
    if (strnicmp(area, "encounterid:", strlen("encounterid:"))==0)
        return EncounterGroupFromHandle(atoi(area+strlen("encounterid:")));

    // several different special cases
    if (layout && !layout[0]) layout = NULL;
    if (strnicmp(area, "encountergroup:", strlen("encountergroup:"))==0)
    {
        return EncounterGroupByName(area + strlen("encountergroup:"), layout, inactiveonly);
    }
    else if (strnicmp(area, "missionroom:", strlen("missionroom:"))==0)
    {
        RoomInfo* room = GetRoomInfo( (area + strlen("missionroom:") ) );
        if (room)
            return MissionFindEncounter(room, layout, inactiveonly);
    }
    else // everything else we approximate by point, including triggers
    {
        if (!GetPointFromLocation(area, vec)) return NULL;
        return EncounterGroupByRadius(vec, LOCATION_RADIUS, layout, inactiveonly,
            deserted? FEWEST_NEARBY_PLAYERS: NEARBY_PLAYERS_OK, 0);
    }
    return NULL;
}



// randomly picks a matching encounter group
ENCOUNTERGROUP FindEncounterGroup(AREA area, STRING layout, int inactiveonly, int deserted)
{
    int handle = 0;
    EncounterGroup* group = FindEncounterGroupInternal(area, layout, inactiveonly, deserted);

    // compose result
    if (group) 
        handle = EncounterGetHandle(group);
    if (handle)
    {
        char buf[100];
        sprintf(buf, "%i", handle);
        return StringAdd("encounterid:", buf);
    }
    return LOCATION_NONE;
}

ENCOUNTERGROUP CloneEncounter( ENCOUNTERGROUP egroup )
{
    EncounterGroup* group = FindEncounterGroupInternal(egroup, NULL, 0, 0);
    int handle = 0;

    //printf("CloneEncounter(%s) called.\n", egroup);

    if (group) 
    {
        group = EncounterGroupClone(group);
        handle = EncounterGetHandle(group);
        group->active = 0;
    }
    if (handle)
    {
        char buf[100];
        sprintf(buf, "%i", handle);
        return StringAdd("encounterid:", buf);
    }
    return LOCATION_NONE;

}

// Looks for encounter groups within a doughnut around this location
ENCOUNTERGROUP FindEncounterGroupByRadius(LOCATION location, int innerRadius, int outerRadius, STRING layout, int inactiveonly, int deserted)
{
    int handle = 0;
    EncounterGroup* group;
    Vec3 vec;

    GetPointFromLocation(location, vec);
    group = EncounterGroupByRadius(vec, outerRadius, layout, inactiveonly,
        deserted? FEWEST_NEARBY_PLAYERS: NEARBY_PLAYERS_OK, innerRadius);

    // compose result
    if (group)
        handle = EncounterGetHandle(group);
    if (handle)
    {
        char buf[100];
        sprintf(buf, "%i", handle);
        return StringAdd("encounterid:", buf);
    }
    return LOCATION_NONE;
}


/*/TO DO make a unified one of these
NEAREST_MATCHING
FAREST_MATCHING
FEWEST_NEARBY_PLAYERS
MOST_NEARBY_PLAYERS
ACTIVE_ONLY
INACTIVE_ONLY
ALERTED_ONLY
NOT_ALERTED_ONLY
*/

ENCOUNTERGROUP FindEncounter( LOCATION loc, FRACTION innerRadius, FRACTION outerRadius, AREA area, STRING missionEncounterName, STRING layout, STRING groupName, SEFindEGFlags flags )
{
    EncounterGroup *results[ENT_FIND_LIMIT];
    Vec3 pos;
    int count;
    F32 * posx = 0;
    EncounterGroup* group = 0;
    Entity * locEnt = 0;
    int oldCollState = 0;

    //printf("FindEncounter(%s, %f, %f, %s, %s, %s, %s, %d) called.\n", loc, innerRadius, outerRadius, area, missionEncounterName, layout, groupName, flags);
    //outputEncounterGroups();

    //Only pass in pos if it's a good location
    if( GetPointFromLocation( loc, pos ) )
        posx = pos;

    //Disable world collisions on the entity you are trying to path to (if applicable).  
    //For the pathfinding.  This is not the most elegant solution to this problem, but a better one involves the 
    //ai and pathfinding naturally knowing about who they are going to find, which we don't have right now
    if( flags & ( SEF_HAS_PATH_TO_LOC | SEF_HAS_FLIGHT_PATH_TO_LOC ) )
    {
        locEnt = EntTeamInternal( loc, 0, NULL );
        if( locEnt && locEnt->coll_tracker )
        {
            oldCollState = locEnt->coll_tracker->no_collide; //Accurate but not elegant
            setNoCollideOnTracker( locEnt->coll_tracker, 1 );
        }
    }

    //Get list of encounter groups that fulfill spec
    count = FindEncounterGroups( results, 
        posx,                //Center point to start search from
        innerRadius,        //No nearer than this
        outerRadius,        //No farther than this
        area,                //inside this area
        layout,                //Layout required in the EncounterGroup
        groupName,            //Name of the encounter group ( Property GroupName )
        flags,
        missionEncounterName );  //missionEncounterName

    //Reenable collisions on path to ent if you disabled them before
    if( locEnt && locEnt->coll_tracker )
    {
        setNoCollideOnTracker( locEnt->coll_tracker, oldCollState ); //Accurate but not elegant
    }

    if( count > 0 ) //count should always be zero or one, since I'm not asking for the full list
        group = results[0];
    else
        group = 0;

    // compose result and return
    if( group )
    {
        int handle = 0;
        
        if (group) handle = EncounterGetHandle(group);
        if (handle)
        {
            char buf[100];
            sprintf(buf, "%i", handle);
            return StringAdd("encounterid:", buf);
        } else {
            return LOCATION_NONE;
        }
    } 

    return LOCATION_NONE;
}


void StopReservingAllEncounters(void)
{
    EncounterStopReservingAll(g_currentScript);
}

//GetAllEncounterGroups(
void ReserveEncountersInRadius( LOCATION loc, FRACTION radius )
{
    EncounterGroup *results[ENT_FIND_LIMIT];
    Vec3 pos;
    int count,i;

    if( !GetPointFromLocation( loc, pos ) )
        return;

    count = FindEncounterGroups( results, 
        pos,                //Center point to start search from
        0,                    //No nearer than this
        radius,                //No farther than this
        0,                    //inside this area
        0,                    //Layout required in the EncounterGroup
        0,                    //Name of the encounter group ( Property GroupName )
        SEF_FULL_LIST,        //Flags
        0 );                //missionEncounterName

    for( i = 0 ; i < count ; i++ )
    {
        //TO DO: kill current occupants?
        ReserveEncounterInternal(results[i]);
    }
}

//Possibly poorly named.
//Allows CanSpawns tagged a ManualSpawn to spawn as normal for a while
void DisableManualSpawn( STRING groupName )
{
    EncounterGroup *results[ENT_FIND_LIMIT];
    int count,i;

    count = FindEncounterGroups( results, 
        0,                //Center point to start search from
        0,                //No nearer than this
        0,                //No farther than this
        0,                //inside this area
        0,                //Layout required in the EncounterGroup
        groupName,        //Name of the encounter group ( Property GroupName )
        SEF_FULL_LIST,    //flags
        0 );            //missionEncounterName

    for( i = 0 ; i < count ; i++ )
    {
        if( results[i]->manualspawn )
        {
            results[i]->manualspawn = 0;
            if (!g_currentScript->encountersWithManualspawnDisabled)
                eaCreate(&g_currentScript->encountersWithManualspawnDisabled);
            eaPush(&g_currentScript->encountersWithManualspawnDisabled, results[i]);
        }
    }
}


//Frees all Encounters With Manualspawn Disabled (Automatically done when mission ends)
void EncounterReenableManualSpawnAll(ScriptEnvironment* script)
{
    int i;
    for (i = eaSize(&script->encountersWithManualspawnDisabled)-1; i >= 0; i--)
    {
        script->encountersWithManualspawnDisabled[i]->manualspawn = 1;
    }
    eaDestroy(&script->encountersWithManualspawnDisabled);
    script->encountersWithManualspawnDisabled = NULL;
}


////////////////////////////////////////////////////////////////////////////////////////
// Encounter Group Overrides

typedef struct EncounterGroupOverride
{
    char * layout;
    char * spawn;
    F32    chance;
    void * scriptOverriding;
} EncounterGroupOverride;

EncounterGroupOverride ** g_EncounterGroupOverrides;

//Scripts call this
void OverrideEncounterGroupsInMap( STRING layout, STRING spawn, FRACTION chance )
{
    EncounterGroupOverride * overRide;

    if(!spawn || !layout )
        return;

    overRide = calloc(1, sizeof( EncounterGroupOverride ) );
    overRide->scriptOverriding            = g_currentScript;
    overRide->layout                    = strdup( layout );
    overRide->spawn                        = strdup( spawn );
    overRide->chance                    = chance;

    if( !g_EncounterGroupOverrides )
        eaCreate(&g_EncounterGroupOverrides);
    eaPush( &g_EncounterGroupOverrides, overRide );
}


//Script management calls this.
//Clears all the overrides of a particular script, done everytime a script is freed
void ClearEncounterGroupOverRides()
{
    int i;
    EncounterGroupOverride * overRide;

    if( !g_EncounterGroupOverrides )
        eaCreate(&g_EncounterGroupOverrides);

    for(i = eaSize(&g_EncounterGroupOverrides)-1; i >= 0; i--)
    {
        overRide = g_EncounterGroupOverrides[i];
        if( overRide->scriptOverriding == g_currentScript ) 
        {
            eaRemove(&g_EncounterGroupOverrides, i);
            free(overRide->spawn);
            free(overRide->layout);
            free(overRide);
        }
    }
}

//encounter.c calles this
int ScriptGetEncounter(EncounterGroup* group, Entity* player)
{
    int i;
    int foundOverride = 0;
    EncounterGroupOverride * overRide = 0;

    if( !g_EncounterGroupOverrides )
        eaCreate(&g_EncounterGroupOverrides);

    for(i = eaSize(&g_EncounterGroupOverrides)-1; i >= 0; i--)
    {
        overRide = g_EncounterGroupOverrides[i];
        if( EncounterGetMatchingPoint(group, overRide->layout ) != -1 ) 
        {
            if( !overRide->chance || (RandomNumber(0.0, 1.0) < overRide->chance ))
            {
                foundOverride = 1;
                break;
            }
        }
    }

    if( !foundOverride )
        return 0;
    
    //Set up EncounterGroup
    {
        int point;
        group->active->spawndef = SpawnDefFromFilename(overRide->spawn);
        if( !group->active->spawndef )
            return 0;
        point = EncounterGetMatchingPoint( group, overRide->layout );
        if (point != -1) 
            group->active->point = point;
        group->active->seed = rand();
        group->active->villainlevel = character_GetCombatLevelExtern(player->pchar); 
        //group->active->mustspawn = 1;
    }
    return 1;
}

/////////////////// End Group Overides /////////////////////////////////////////////////////////

// spawns the given spawndef at this location.  Implicitly calls FindEncounterGroup
// if a specific group isn't given as encounter.
LOCATION Spawn(int num, TEAM teamName, STRING spawnfile, ENCOUNTERGROUP encounter, STRING layout, int level, int size)
{ 
    EncounterGroup* group = NULL;
    const SpawnDef* spawndef;
    int i;
    int tempSpawnType = false;
    int tempMapFlag = false;

    //printf("Spawn(%d, %s, %s, %s, %s, %d, %d) called.\n", num, teamName, spawnfile, encounter, layout, level, size);
    //outputEncounterGroups();

    // find out what kind of spawn this is:
    if (StringEqual(spawnfile, "UseCanSpawns")) {
        spawndef = NULL;
    } else {
        if (strnicmp(spawnfile, "missionencounter:", strlen("missionencounter:"))==0)
        {
            // a mission specific spawn
            const char * name = spawnfile + strlen("missionencounter:");

            spawndef = getMissionSpawnByLogicalName(name);
        } else {
            // load the provided spawndef
            spawndef = SpawnDefFromFilename(spawnfile);
        }
        if (!spawndef)
        {
            ScriptError("Spawn() asked to load spawndef %s, but this doesn't exist", spawnfile);
            return LOCATION_NONE;
        }
    }

    for (i = 0; i < num; i++)
    {
        const char * override_spawntype = layout;

        // look repeatadly for an inactive encounter group
        if (spawndef) {
            if (spawndef->spawntype) {
                override_spawntype = NULL;
                group = FindEncounterGroupInternal(encounter, spawndef->spawntype, 1, 0);
            } else if (!StringEmpty(layout)) {
                group = FindEncounterGroupInternal(encounter, layout, 1, 0);
            } else {
                group = FindEncounterGroupInternal(encounter, NULL, 1, 0);
            }
        } else {
            group = FindEncounterGroupInternal(encounter, NULL, 1, 0);
        }
        if (!group)
        {
            ScriptError("Spawn() asked to create an encounter at %s, but there are no available groups there", encounter);
            return LOCATION_NONE;
        }

        EncounterCleanup(group);
        if (!EncounterNewSpawnDef(group, spawndef, override_spawntype))
        {
            ScriptError("Spawn() tried to put spawndef %s at location %s, but didn't get appropriate layout", spawnfile, encounter);
            return LOCATION_NONE;
        }

        // level and size are optional
        if (level > 0) {
            group->active->villainlevel = level;
        } else if (OnMissionMap()) {
            group->active->villainlevel = g_activemission->missionLevel;
        }

        if (size > 0) 
        {
            group->active->numheroes = size;
        } 
        else if (OnMissionMap()) 
        {
            group->active->numheroes = MAX( g_activemission->difficulty.teamSize, MissionNumHeroes() );
        }

        if (OnMissionMap()) {
            group->active->villaingroup = g_activemission->missionGroup;
        }

        // save current setting of script control flag
        tempMapFlag = EncounterGetScriptControlsSpawning();
        EncounterSetScriptControlsSpawning(false);

        EncounterSpawn(group);

        // restore saved state of script control flag
        EncounterSetScriptControlsSpawning(tempMapFlag);

        if (group->active)
        {
            Entity* leader = NULL;
            int en;
            for (en = eaSize(&group->active->ents)-1; en >= 0; en--)
            {
                Entity* curEnt = group->active->ents[en];

                if (!ENTAI(curEnt)) continue;

                // force them to make the ai team correctly, even if we call DetachSpawn() immediately after this
                if (!leader && curEnt->pchar) 
                    leader = curEnt;

                if (leader)
                {
                    AI_LOG(AI_LOG_TEAM, (curEnt, "Scripthook Spawn is joining \"%s\" to \"%s\"\n", curEnt->name, leader->name));
                    aiTeamJoin(&ENTAI(leader)->teamMemberInfo, &ENTAI(curEnt)->teamMemberInfo);
                }

                if (teamName && teamName[0] != 0)
                    ScriptTeamAddEnt(curEnt, teamName);
            }
            group->noautoclean = 1; // don't allow it to clean up
            if (g_ArchitectTaskForce)
            {
                if (teamName && teamName[0])
                {
                    if (stricmp(teamName, "WaveAttackers") == 0)
                    {
                        static float s_rewardScaleMod = 1.0f;
                        for (i = 0; i < eaSize(&group->active->ents); ++i)
                        {
                            group->active->ents[i]->fRewardScale *= s_rewardScaleMod;
                        }
                        s_rewardScaleMod *= g_PCCRewardMods.fAmbushScaling;
                    }
                }
            }
        }

        // if there is an objective with this, attach it
        if (spawndef && spawndef->objective)
            MissionObjectiveAttachEncounter(spawndef->objective[0], group, spawndef);

    }
    return EncounterNameInternal(group);
}

// each entity is no longer attached to the spawn point
// 1. Won't try to return to this encounter location
// 2. Can't be used by missions for encounter success
// 3. Encounter Group with all dead or detached is free to be used again
void DetachSpawn(TEAM team)
{
    int i, n;
    n = NumEntitiesInTeam(team);
    for (i = n-1; i >= 0; i--)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e) 
            EncounterEntDetach(e);
    }
}

// attaches this entity to the encounter
// ONLY valid if the encounter is currently active - use Spawn() to set up an active encounter
void AttachSpawn(ENCOUNTERGROUP encounter, TEAM team)
{
    int i, n;

    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);
    if (!group)
    {
        ScriptError("AttachSpawn asked to attach to non-existent encounter %s", encounter);
        return;
    }
    if (!group->active)
    {
        ScriptError("AttachSpawn asked to attach to non-active encounter %s", encounter);
        return;
    }

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        if (e) EncounterEntAttach(group, e);
    }
}

STRING GetSpawnLayout(STRING spawnfile)
{
    const SpawnDef* spawndef;

    // check to see if there's anything there
    if (!spawnfile || StringEqual(spawnfile, ""))
        return "";

    // find out what kind of spawn this is:
    if (strnicmp(spawnfile, "missionencounter:", strlen("missionencounter:"))==0)
    {
        // a mission specific spawn
        const char * name = spawnfile + strlen("missionencounter:");

        spawndef = getMissionSpawnByLogicalName(name);
    } else {
        // load the provided spawndef
        spawndef = SpawnDefFromFilename(spawnfile);
    }    

    if (spawndef && spawndef->spawntype)
        return spawndef->spawntype;
    else 
        return "";
}


void CloseMyEncounterAndEndScript()
{
    g_currentScript->closeMyEncounter = 1;
    EndScript();
}

// closes given encounter group (free'ing any entities still attached)
void CloseEncounter(ENCOUNTERGROUP encounter)
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if( group == g_currentScript->encounter )
        ScriptError("CloseEncounter asked to close it's own encounter %s, you must use CloseMyEncounterAndEndScript() to do this", encounter);

    if (!group)
    {
        ScriptError("CloseEncounter asked to close encounter %s, but this doesn't exist", encounter);
    }
    else
    {
        EncounterCleanup(group);
    }
}

// set encounter completion state
void SetEncounterComplete(ENCOUNTERGROUP encounter, NUMBER success)
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)
    {
        ScriptError("SetEncounterComplete called for encouter %s, but this doesn't exist", encounter);
    }
    else
    {
        if (group->active) // check to make sure group is already complete
        {
            int oldComplete = group->scriptControlsCompletion;
            group->scriptHasNotHadTick = false;
            group->scriptControlsCompletion = 0;

            EncounterComplete(group, success);

            group->scriptControlsCompletion = oldComplete;
        }
    }
}

// set encounter completion state
void ScriptControlsCompletion( ENCOUNTERGROUP encounter )
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)
    {
        ScriptError("SetEncounterComplete called for encouter %s, but this doesn't exist", encounter);
    }
    else
    {
        group->scriptControlsCompletion = 1;
    }
}

int EncounterLevel(ENCOUNTERGROUP encounter)
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)
    {
        ScriptError("EncounterLevel called for encounter %s, but this doesn't exist", encounter);
        return 0;
    }
    if (!group->active)
    {
        ScriptError("EncounterLevel called for encounter %s, but this encounter isn't active", encounter);
        return 0;
    }
    return group->active->villainlevel;
}

int EncounterTeamSize(ENCOUNTERGROUP encounter)
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)
    {
        ScriptError("EncounterTeamSize called for encounter %s, but this doesn't exist", encounter);
        return 0;
    }
    if (!group->active)
    {
        ScriptError("EncounterTeamSize called for encounter %s, but this encounter isn't active", encounter);
        return 0;
    }
    return group->active->numheroes;
}

int EncounterSize(ENCOUNTERGROUP encounter)
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)
    {
        ScriptError("EncounterSize called for encounter %s, but this doesn't exist", encounter);
        return 0;
    }
    if (!group->active)
    {
        ScriptError("EncounterSize called for encounter %s, but this encounter isn't active", encounter);
        return 0;
    }
    return group->active->villaincount;
}



ENCOUNTERSTATE EncounterState(ENCOUNTERGROUP encounter)
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)
    {
        ScriptError("EncounterTeamSize called for encounter %s, but this doesn't exist", encounter);
        return 0;
    }
    return group->state;
}

ENCOUNTERSTATE SetEncounterActive(ENCOUNTERGROUP encounter)
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)
    {
        ScriptError("EncounterTeamSize called for encounter %s, but this doesn't exist", encounter);
        return 0;
    }
    setEncounterActive( group );
    return group->state;
}

ENCOUNTERSTATE SetEncounterInactive(ENCOUNTERGROUP encounter)
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)
    {
        ScriptError("EncounterTeamSize called for encounter %s, but this doesn't exist", encounter);
        return 0;
    }
    setEncounterInactive( group );
    return group->state;
}

ENTITY ActorFromEncounterPos(ENCOUNTERGROUP encounter, int position)
{
    int i, n;
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)
    {
        ScriptError("ActorFromEncounterPos called for encounter %s, but this doesn't exist", encounter);
        return TEAM_NONE;
    }
    if (!group->active)
    {
        return TEAM_NONE;
    }

    n = eaSize(&group->active->ents);
    for (i = 0; i < n; i++)
    {
        if (EncounterPosFromActor(group->active->ents[i]) == position)
            return EntityNameFromEnt(group->active->ents[i]);
    }
    return TEAM_NONE;
}


ENTITY ActorFromEncounterActorName(ENCOUNTERGROUP encounter, STRING actorName )
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);
    Entity * e = 0;

    if (!group)
    {
        ScriptError("ActorFromEncounterActorName called for encounter %s, but this doesn't exist", encounter);
        return TEAM_NONE;
    }
    if (!group->active)
    {
        return TEAM_NONE;
    }

    e = EncounterActorFromEncounterActorName( group, actorName );

    if( e )
        return EntityNameFromEnt(e);
    return TEAM_NONE;
}

LOCATION EntityPos(ENTITY ent)
{
    char buf[100];

    Entity * e = EntTeamInternal(ent, 0, NULL);
    if (e)
    {
        sprintf(buf, "coord:%f,%f,%f", ENTPOSPARAMS(e));
        return StringDupe(buf);
    }

    return LOCATION_NONE;
}

void RestoreEncounterEntity( EncounterGroup * group, int position )
{

}

LOCATION EncounterPos(ENCOUNTERGROUP encounter, int position)
{
    char buf[100];
    Mat4 mat;
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)
    {
        ScriptError("ActorFromEncounterPos called for encounter %s, but this doesn't exist", encounter);
        return LOCATION_NONE;
    }
    if (!EncounterMatFromPos(group, position, mat))
    {
        return LOCATION_NONE;
    }

    sprintf(buf, "coord:%f,%f,%f", mat[3][0], mat[3][1], mat[3][2]);
    return StringDupe(buf);
}

//Frees all Reserved Encounters (Automatically done when mission ends)
void EncounterStopReservingAll(ScriptEnvironment* script)
{
    int i;
    if (script != NULL && script->reservedEncounters != NULL)
    {
        for (i = eaSize(&script->reservedEncounters)-1; i >= 0; i--)
        {
            script->reservedEncounters[i]->manualspawn = 0;
        }
        eaDestroy(&script->reservedEncounters);
        script->reservedEncounters = NULL;
    }
}

void ReserveEncounterInternal(EncounterGroup* group)
{
    if (!group->manualspawn)
    {
        group->manualspawn = 1;
        if (!g_currentScript->reservedEncounters)
            eaCreate(&g_currentScript->reservedEncounters);
        eaPush(&g_currentScript->reservedEncounters, group);
    }
}

void SetDisableMapSpawns(NUMBER flag)
{
    EncounterSetScriptControlsSpawning(flag);
}

//This EncounterGroup can't be be used except by Script (CanSpawns or overrides (?))
void ReserveEncounter(ENCOUNTERGROUP encounter)
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)
    {
        ScriptError("CloseEncounter asked to reserve encounter %s, but this doesn't exist", encounter);
    }

    ReserveEncounterInternal( group );
}

ENCOUNTERGROUP MyEncounter()
{
    if (!g_currentScript->encounter)
    {
        ScriptError("MyEncounter called from non-encounter script %s", g_currentScript->function->name);
        return LOCATION_NONE;
    }
    return EncounterNameInternal(g_currentScript->encounter);
}

void RegenerateEncounter( ENCOUNTERGROUP encounter )
{
    EncounterGroup* group = FindEncounterGroupInternal(encounter, NULL, 0, 0);

    if (!group)

        ScriptError("RegenerateEncounter asked to regenerate encounter %s, but this doesn't exist", encounter);
    else
        EncounterRespawnActors( group );
}

void EncounterGroupsInitIterator(
                        ENCOUNTERGROUPITERATOR *iter,
                        LOCATION loc,            //Center point to start search from
                        FRACTION innerRadius,        //No nearer than this
                        FRACTION outerRadius,        //No farther than this
                        STRING area,            //Inside this area
                        STRING layout,            //Layout required in the EncounterGroup
                        STRING groupname        //Name of the encounter group ( Property GroupName )
                        )
{
    int i;
    int count;
    int handle;
    EncounterGroup *results[ENT_FIND_LIMIT];
    Vec3 center;

    iter->index = 0;
    iter->count = 0;
    count = FindEncounterGroups(results,
            GetPointFromLocation(loc, center) ? center : NULL,
            innerRadius,
            outerRadius,
            area,
            layout,
            groupname,
            SEF_FULL_LIST,
            NULL);

    for (i = 0; i < count && iter->count < ENCOUNTER_ITERATOR_MAX; i++)
    {
        if (results[i] && (handle = EncounterGetHandle(results[i])) != 0)
        {
            iter->handles[iter->count++] = handle;
        }
    }
}

ENCOUNTERGROUP EncounterGroupsFindNext(ENCOUNTERGROUPITERATOR *iter)
{
    return iter->index < iter->count ? StringAdd("encounterid:", NumberToString(iter->handles[iter->index++])) : LOCATION_NONE;
}
