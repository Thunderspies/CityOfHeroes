#include <utilitieslib/utils/wininclude.h>
#include "bases/BaseEntry.h"
#include "entity/SgrpServer.h"
#include "bases/baseupkeep.h"
#include <stdio.h>
#include "door.h"
#include "entity/entity.h"
#include <utilitieslib/network/netio.h>
#include "entity/entVarUpdate.h"
#include "comm_game.h"
#include "dbcomm/dbdoor.h"
#include "dbcomm/dbmapxfer.h"
#include "entity/pvp.h"
#include "gameSys/arenamap.h" // for OnArenaMap()

#include <utilitieslib/utils/error.h>
#include "varutils.h"
#include "entity/entserver.h"
#include "dbcomm/dbcomm.h"
#include <utilitieslib/utils/utils.h>
#include "svr/svr_player.h"
#include "player/parseClientInput.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "container/team.h"
#include <utilitieslib/components/earray.h>
#include "storyarc/mission.h"
#include "storyarc/task.h"
#include "storyarc/storyarcprivate.h"
#include "dbcomm/staticMapInfo.h"
#include "gamesys/dooranim.h"
#include "gameComm/sendToClient.h"
#include "storyarc/TeamTask.h"
#include "entity/teamup.h"
#include "ai/entai.h"
#include "ai/entaiprivate.h"
#include "entity/entplayer.h"
#include "player/pmotion.h"
#include "TeamReward.h"
#include "script/ZoneEvents/HalloweenEvent.h"
#include "gamesys/arenamapserver.h"
#include "gameData/arenastruct.h"
#include "dbcomm/dbnamecache.h"
#include "cmdparse/cmdserver.h"
#include "taskRandom.h"
#include "gameSys/sgraid.h"
#include "gameData/raidstruct.h"
#include "group/groupnetdb.h"
#include "missionmapinit.h"
#include "bases/baseserver.h"
#include "auth/authUserData.h"
#include "entity/Supergroup.h"
#include "bases/basedata.h"
#include "gameSys/raidmapserver.h"
#include "entity/supergroup.h"
#include "bases/basesystems.h"
#include "bases/bases.h"
#include "gameSys/sgraid_V2.h"
#include "seq/seq.h"
#include "seq/seqstate.h"
#include "container/containerloadsave.h"
#include "entity/SgrpBasePermissions.h"
#include <utilitieslib/components/StringCache.h>
#include "entity/mapHistory.h"
#include "DoorStrings.h"
#include <ailib/aiBehaviorPublic.h>
#include "entity/character_eval.h"
#include "entity/character_tick.h"
#include "storyarc/taskforce.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/missionMapCommon.h"
#include "dbcomm/logcomm.h"
#include "LWC_common.h"

enum
{
    MAP_IGNORED,
    MAP_OPEN_LOW,
    MAP_OPEN_HIGH,
    MAP_EMPTY,
    MAP_FULL,
    MAP_MISSION,
    MAP_TOO_HIGH,
    MAP_FAILED_REQUIRES,
};

typedef enum DoorRequestType
{
    DOOR_REQUEST_CLICK,
    DOOR_REQUEST_VOLUME,
    DOOR_REQUEST_EXIT_MISSION,
    DOOR_REQUEST_MAP_MOVE,
    DOOR_REQUEST_EXIT_MISSION_AND_MAP_MOVE,
    DOOR_REQUEST_SCRIPT_DOOR,
} DoorRequestType;

typedef struct MapSelection
{
    int map_id;
    int mowner;
    StoryTaskHandle sahandle;
    char spawn_name[32];
} MapSelection;

typedef struct ArchitectDoorRequest
{
    U32 architectId;
    EntityRef eRef;
    U32 timeEnqueued;
    Vec3 pos;
} ArchitectDoorRequest;

typedef struct BaseEntryInfo BaseEntryInfo;
static MapSelection* checkDoorOptions(Entity* e, int dest_map_id, Vec3 pos, DoorEntry* door, const char *selected_map, int forceGood, DoorRequestType requestType, int detailID);
static bool s_EnterBases(Entity *e, Vec3 pos);
static void s_StartBaseEntry(Entity *e, Vec3 pos);


void sendDoorMsg( Entity *e, int msg_type, char *errmsg, ... )
{
    char *doorcmd;

    if (msg_type == DOOR_MSG_ASK)
    {
        va_list argptr;
        
        va_start(argptr, errmsg);
        doorcmd = va_arg(argptr, char *);
        va_end(argptr);
    }
    else
    {
        doorcmd = "";
    }

    if (!errmsg)
    {
        errmsg = "";
    }
    START_PACKET( pak1, e, SERVER_DOOR_CMD );
    pktSendBitsPack( pak1, 1, msg_type );
    // send the doorcmd first.  That way, it's read out first and saved.  The code that fields this is in routine void receiveDoorMsg( Packet *pak )
    // over in clientcommreceive.c - take a look there to see how this is handled.
    if (msg_type == DOOR_MSG_ASK)
    {
        pktSendString(pak1, doorcmd);
    }
    pktSendString(pak1, errmsg);
    END_PACKET
}

// Changed from being a local variable so it can't get freed or stomped on.
static ContainerStatus c_list[1];

// does a synchronous db call to verify this map has the correct supergroup as the player
// and to make sure that the map does not have too many people
// Only returns 0 if you explicitly need to be restricted from entering the map
static int CheckSupergroupMission(int map_id, int playerSG, char* errmsg)
{
    U32 udontcare;
    int dontcare, missionSG;
    ContactHandle unused = 0;
    VillainGroupEnum villainGroup;
    EncounterAlliance encAlly;
    StoryDifficulty  doesntMatter;

    c_list[0].id = map_id;
    dbSyncContainerStatusRequest(CONTAINER_MAPS, &(c_list[0]), 1);
    if (!c_list[0].valid)
        return 1;
    if (!MissionDeconstructInfoString(c_list[0].mission_info, &dontcare, &missionSG, &dontcare, &unused, tempStoryTaskHandle(0,0,0,0), &udontcare, 
        &dontcare, &doesntMatter, &udontcare, &encAlly, &villainGroup))
    {
        return 1;
    }

    //if (missionSG && (missionSG != playerSG))
    //{
    //    strcpy(errmsg, saUtilLocalize(0,"MissionWrongSG"));
    //    return 0;
    //}
    if (missionSG && (dbSyncMissionPlayerCount(c_list[0].mission_info) >= SGMISSION_MAX_PLAYERS))
    {
        strcpy(errmsg, saUtilLocalize(0,"MissionFull"));
        return 0;
    }

    return 1;
}

// walk through door, assumes teamup has valid attachment to door already
static int enterMissionDoor(Entity *e, DoorEntry* door, Vec3 doorpos, const char * mapname, char* errmsg)
{
    Teamup* t = e->teamup;
    int sgowner = 0;
    assert(e->teamup);

    log_printf("missiondoor.log", "enterMissionDoor: %s(%i) door %i, task %i:%i:%i:%i, owner %i, task name %s, mission def? %i", 
        e->name, e->db_id, (door ? door->db_id : -1), SAHANDLE_ARGSPOS(t->activetask->sahandle), 
        t->activetask->assignedDbId, t->activetask->def? t->activetask->def->logicalname: "(none)",    TaskMissionDefinition(&t->activetask->sahandle)? 1: 0);

    teamlogPrintf(__FUNCTION__, "%s(%i) door:%i task %i:%i:%i:%i owner:%i", e->name, e->db_id, (door ? door->db_id : -1), 
        SAHANDLE_ARGSPOS(e->teamup->activetask->sahandle),    e->teamup->activetask->assignedDbId); 

    // if the team's active mission is a SG mission and this player's SG is on a mission and
    //        the mission is the same as the one the SG is on, override the team's map with the map of the SG mission
    if (!t->map_id && TASK_IS_SGMISSION(t->activetask->def) )
    {
        t->map_id = t->activetask->missionMapId;
    }


    if (!CheckSupergroupMission(t->map_id, e->supergroup_id, errmsg))
    {
        log_printf("missiondoor.log", "Player %i on teamup %i could not enter mission door to %i; err: %s\n", 
            e->db_id, e->teamup_id, t->map_id, errmsg);
        return 0;
    }

    // Figure out which sg this is attached to, if it is
    if (TASK_IS_SGMISSION(t->activetask->def))
    {
        StoryTaskInfo* sgmission = t->activetask;
        if (door && sgmission && TASK_INPROGRESS(sgmission->state) && sgmission->assignedDbId == t->activetask->assignedDbId )
        {
            sgowner = e->supergroup_id;
            if( g_base.supergroup_id )
                sgowner = g_base.supergroup_id; 
        }
    }


    // check that map_id is still running
    if (t->map_id)
    {
        int ready = 0;
        if (CheckMissionMap(t->map_id, t->activetask->assignedDbId, sgowner, e->taskforce_id, &t->activetask->sahandle, &ready))
        {
            teamlogPrintf(__FUNCTION__, "mission %i running", t->map_id); 
        }
        else
        {
            dbLog("Team", e, "enterMissionDoor map_id %d map_type %d", t->map_id, t->map_type );
            t->map_id = 0;
        }
    }

    // start the map if it's not running
    if (!t->map_id)
    {
        Entity* owner = entFromDbId(t->activetask->assignedDbId);
        char * missioninfo = NULL;
        VillainGroupEnum villainGroup;

        teamlogPrintf(__FUNCTION__, "%s(%i) starting map", e->name, e->db_id); 

        // Reassert notoriety every time we start a map
        difficultySetFromPlayer(&t->activetask->difficulty, owner);
        difficultyUpdateTask(owner, &t->activetask->sahandle);

        villainGroup=getVillainGroupFromTask(t->activetask->def);
        if (villainGroup==villainGroupGetEnum("Random"))
        {
            villainGroup=t->activetask->villainGroup;
        }
        else if (villainGroup==villainGroupGetEnum("Var"))
        {
            ScriptVarsTable vars = {0};
            // Potential mismatch
            saBuildScriptVarsTable(&vars, owner ? owner : e, t->activetask->assignedDbId, t->mission_contact, t->activetask, NULL, NULL, NULL, NULL, 0, 0, false);
            villainGroup = StaticDefineIntGetInt(ParseVillainGroupEnum, ScriptVarsTableLookup(&vars, (TaskMissionDefinition(&t->activetask->sahandle))->villaingroupVar));
        }

        if(!mapname)
            mapname = MissionGetMapName(owner ? owner : e, t->activetask, t->mission_contact);
        if (t->activetask)
        {
            EncounterAlliance encounterAlliance;

            if (ENT_IS_ON_RED_SIDE(e))
            {
                encounterAlliance = ENC_FOR_VILLAIN;
            }
            else
            {
                encounterAlliance = ENC_FOR_HERO;
            }

            missioninfo = MissionConstructInfoString(t->activetask->assignedDbId, sgowner, e->taskforce_id, t->mission_contact,
                &t->activetask->sahandle, t->activetask->seed, t->activetask->level,
                &t->activetask->difficulty, t->activetask->completeSideObjectives, encounterAlliance, villainGroup);
        }
        if (teamStartMap(e, mapname, missioninfo))
        {
            if (sgowner)
                TaskSetMissionMapId(e, &t->activetask->sahandle, t->map_id);
            else if (owner)
                TaskSetMissionMapId(owner, &t->activetask->sahandle, t->map_id);
            else
                log_printf("missiondoor.log", "!!! Team %i attached to door, but owner %i not on this server", e->teamup_id, t->activetask->assignedDbId);
        }
    }

    // enter the map
    if (t->map_id)
    {
        teamlogPrintf(__FUNCTION__, "%s(%i) entering map", e->name, e->db_id); 
        DoorAnimEnter(e, door, doorpos, t->map_id, NULL, NULL, XFER_MISSION, NULL);

        log_printf("missiondoor.log", "Player %i on teamup %i entering mission door to %i\n", 
            e->db_id, e->teamup_id, t->map_id);
        return 1;
    }

    // Changed this error message since it gets issued when overload protection is enabled.
    // The mapserver currently does not know if the overload protection is enabled.
    // So, use the more accurate, but still applicable for most situtaions message.
    //strcpy(errmsg, localizedPrintf(e,"CouldntStartMission"));
    // "Map failed to start.<br>The server is busy, please try again later."
    strcpy(errmsg, localizedPrintf(e,"MapFailedOverloadProtection"));

    return 0;
}

// attempts to lock the teamup so we can enter the door, creates a one man team
// if necessary
int lockTeamupForDoor(Entity* e)
{
    // if I don't have a teamup
    if (!e->teamup_id)
    {
        
        // create a teamup of one for mission
        teamlogPrintf(__FUNCTION__, "creating team for %s(%i)",    e->name, e->db_id); 
        teamCreate(e, CONTAINER_TEAMUPS);
        
        if (teamLock(e, CONTAINER_TEAMUPS))
        {
            e->teamup->members.leader = e->db_id;
            // ARM This function call shouldn't be necessary:
            // if you weren't on a teamup, you can't be on a taskforce.
            // if you aren't on a taskforce, this function does nothing.
            // Commenting out the function because you should never call this inside a teamup lock.
            //TaskForceMakeLeader(e, e->db_id);
            return 1;
        }
        return 0; // couldn't acquire lock
    }
    else
    {
        teamlogPrintf(__FUNCTION__, "locking team %i for %s(%i)", e->teamup_id, e->name, e->db_id); 
        return teamLock(e, CONTAINER_TEAMUPS);
    }
}

// if anyone on team should have access to door, attach the teamup to the door
// returns 1 if the teamup is attached (may already have been attached previously)
static int attachTeamupToMission(Entity* e, int door_id, int mowner, StoryTaskHandle *sahandle, char* errmsg)
{
#define CORRECT_MISSION(x) \
    (door_id? (x->doorId == door_id) : \
    (x->assignedDbId == mowner && isSameStoryTaskPos( &x->sahandle, sahandle)))
#define CORRECT_SGMISSION(x) \
    (door_id? (x->doorId == door_id) : \
    (x->assignedDbId == e->supergroup_id && isSameStoryTaskPos( &x->sahandle, sahandle)) )

    StoryTaskInfo* teamtask = e->teamup->activetask;
    StoryTaskInfo* mission = NULL;
    int i, j, k;

    assert(door_id || mowner); // need to have one or the other
    log_printf("missiondoor.log", "attachTeamupToMission: %s(%i) team has door %i, task %i:%i:%i:%i, owner %i, task name %s, mission def? %i",
        e->name, e->db_id, teamtask->doorId, SAHANDLE_ARGSPOS(teamtask->sahandle), 
        teamtask->assignedDbId, teamtask->def? teamtask->def->logicalname: "(none)", 
        TaskMissionDefinition(&teamtask->sahandle)? 1: 0);

    // hack for stupid designers - we were assigned a door ID, but now the task
    // isn't even a mission
    if (teamtask->doorId && (!TaskIsMission(teamtask) || !TaskMissionDefinition(&teamtask->sahandle)))
    {
        log_printf("missiondoor.log", "attachTeamupToMission: had to strip door id from %s %i:%i:%i:%i owner %s",
            e->teamup->activetask->def? e->teamup->activetask->def->logicalname: "(No Def)",
            SAHANDLE_ARGSPOS(e->teamup->activetask->sahandle), dbPlayerNameFromId(e->teamup->activetask->assignedDbId));
        e->teamup->activetask->doorId = 0;
        e->teamup->activetask->doorMapId = 0;
        setVec3(e->teamup->activetask->doorPos, 0.0, 0.0, 0.0);
    }

    // already attached to right mission
    if ( CORRECT_MISSION(e->teamup->activetask) )
    {
        // Can't have too many players, even if you've already selected the mission.
        if ((e->teamup->activetask->def && e->teamup->activetask->def->maxPlayers != 0 && e->teamup->activetask->def->maxPlayers < e->teamup->members.count)
             || (e->teamup->activetask->compoundParent && e->teamup->activetask->compoundParent->maxPlayers != 0 && e->teamup->activetask->compoundParent->maxPlayers < e->teamup->members.count))
        {
            strcpy(errmsg, localizedPrintf(e,"TooManyPlayersForMission"));
            return 0;
        }

        if (TASK_INPROGRESS(e->teamup->activetask->state))
        {
            log_printf("missiondoor.log", "attachTeamupToMission: %s(%i) team already attached to door %i, with %i:%i:%i:%i owner %i",
                e->name, e->db_id, door_id, SAHANDLE_ARGSPOS(teamtask->sahandle), teamtask->assignedDbId);

            teamlogPrintf(__FUNCTION__, "team already attached %s(%i) door:%i task %i:%i:%i:%i owner:%i", 
                e->name, e->db_id, door_id, SAHANDLE_ARGSPOS(e->teamup->activetask->sahandle), e->teamup->activetask->assignedDbId); 
            return 1; // if the team is already attached to door
        }
        if(errmsg)
        {
            strcpy(errmsg, localizedPrintf(e,"MissionHasBeenCompleted"));
        }
        return 0; // can't reenter the mission after it is completed
    }
    // attached to wrong mission
    if (TaskIsMission(teamtask) || (e->teamup->map_id && !isBaseMap()))
    {
        // If the team no longer has a task, but has a map with no players on it, shut it down
        // This seems to be happening with newspaper missions because the task is cleared immediately
        if (!e->teamup->map_playersonmap && e->teamup->map_id && (!e->teamup->activetask || (!e->teamup->activetask->def && !e->teamup->activetask->playerCreatedID)))
            teamDetachMapUnlocked(e); 
        else
        {
            teamlogPrintf(__FUNCTION__, "team attached to different door %s(%i) wanted:%i attached:%i", 
                e->name, e->db_id, door_id, e->teamup->activetask->doorId);
            if(errmsg)
            {
                strcpy(errmsg, localizedPrintf(e,"MustCompleteActive"));
            }
            return 0; // team attached to wrong door
        }
    }
    // not allowed to reselect the team task
    if (!TeamCanSelectMission(e))
    {
        if(errmsg)
        {
            strcpy(errmsg, localizedPrintf(e,"MustCompleteActive"));
        }
        return 0;
    }



    // Check to see if it is a supergroup mission, only check owner
    if (e->supergroup) 
        mission = e->supergroup->activetask;
    if (mission && CORRECT_SGMISSION(mission) && TASK_INPROGRESS(mission->state))
    {
        if( mission->def && mission->def->logicalname && strstriConst( mission->def->logicalname, "CathedralOfPain" ) ) //TO DO make this a tag in mission
        {
            if( RaidIsRunning() )
            {
                if(errmsg)
                {
                    strcpy(errmsg, localizedPrintf(e,"CantDoCathedralBeacuseARaidIsRunning"));
                }
                return 0;
            }
        }

        if (!TaskMissionDefinition(&mission->sahandle))
        {
            log_printf("missiondoor.log", "attachTeamupToMission: Using SG Mission: something truely bizarre is happening %i:%i:%i:%i (%i)",
                SAHANDLE_ARGSPOS(mission->sahandle), mission->assignedDbId);
        }
        else
        {
            teamlogPrintf(__FUNCTION__, "attaching %s(%i) task %i:%i:%i:%i owner:%i", 
                e->name, e->db_id, SAHANDLE_ARGSPOS(mission->sahandle), mission->assignedDbId); 
            MissionAttachTeamupUnlocked(e, e, mission, 0);
            TeamBroadcastTeamTaskSelected(e->teamup_id);
            return 1;
        }
    }

    // see if anyone on team has mission
    for (i = 0; i < e->teamup->members.count; i++)
    {
        Entity* owner = entFromDbId(e->teamup->members.ids[i]);
        StoryTaskInfo* mission;
        if (!owner)
            continue; // not on this map server, can't easily look at door 

        // see if this guy has the mission
        for (j = 0; mission = TaskGetMissionTask(owner, j); j++)
        {
            int good_mission_for_door = (CORRECT_MISSION(mission) && TASK_INPROGRESS(mission->state));

            if( !good_mission_for_door && TASK_INPROGRESS(mission->state) )
            {
                const MissionDef *missionDef = TaskMissionDefinition(&mission->sahandle);
                DoorEntry * door = dbFindDoorWithDoorId( door_id );
                if( missionDef && missionDef->doortype && door && door->name && stricmp( missionDef->doortype, "architect" ) == 0 && stricmp(door->name, "Architect" ) == 0 )
                    good_mission_for_door = 1;
            }

            if( good_mission_for_door )
            {
                const char **teamRequires;

                // Can't have too many players
                if ((mission->def && mission->def->maxPlayers != 0 && mission->def->maxPlayers < e->teamup->members.count)
                    || (mission->compoundParent && mission->compoundParent->maxPlayers != 0 && mission->compoundParent->maxPlayers < e->teamup->members.count))
                {
                    strcpy(errmsg, localizedPrintf(e,"TooManyPlayersForMission"));
                    return 0;
                }

                if (teamRequires = storyTaskHandleGetTeamRequires(&mission->sahandle, NULL))
                {
                    // check to see if all members of the team can start this mission
                    for(k = 0; k < e->teamup->members.count; k++)
                    {
                        Entity *teammate = entFromDbIdEvenSleeping(e->teamup->members.ids[k]);

                        // If the teammate is found but isn't on this mapserver, do nothing.
                        if(!teammate || TeamTaskValidateStoryTask(teammate, &mission->sahandle) != TTSE_SUCCESS)
                        {
                            const char* failedText = storyTaskHandleGetTeamRequiresFailedText(&mission->sahandle);
                            strcpy(errmsg, failedText ? saUtilScriptLocalize(failedText) : localizedPrintf(e,"DoorLockedInvalidTeammateMission"));
                            return 0;
                        }

                    }
                }

                // check to see if this is an alliance restricted mission 
                if (mission && mission->def->alliance != SA_ALLIANCE_BOTH && areMembersAlignmentMixed(e->teamup ? &e->teamup->members : NULL))
                {
                    strcpy(errmsg, localizedPrintf(e,"DoorLockedAllianceLocked"));
                    return 0;
                }

                if (!TaskMissionDefinition(&mission->sahandle))
                {
                    log_printf("missiondoor.log", "attachTeamupToMission: ok something truely bizarre is happening %i:%i:%i:%i (%i)",
                        SAHANDLE_ARGSPOS(mission->sahandle), mission->assignedDbId);
                    continue;
                }
                teamlogPrintf(__FUNCTION__, "attaching %s(%i) task %i:%i:%i:%i owner:%i", 
                    e->name, e->db_id, SAHANDLE_ARGSPOS(mission->sahandle), mission->assignedDbId); 
                MissionAttachTeamupUnlocked(e, owner, mission, 0);
                TeamBroadcastTeamTaskSelected(e->teamup_id);
                return 1;
            }
        }
    }
    if(errmsg)
    {
        strcpy(errmsg, localizedPrintf(e,"DoorLocked"));
    }
    return 0;
}

/// Script Managed Doors ##################################################################################
// Scripts can set a named door locked.  Works just like a KeyDoor, but the script does it...
//TO DO Right now this can only lock a door, it can't unlock doors locked by other things
//like key doors.  Make this more clear or fix.
typedef struct ScriptDoor
{
    Vec3 pos; //Doors are uniquely ided by their position. great
    int status;
    char msg[256];        //What to say when you can't open this door
} ScriptDoor;

ScriptDoor ** g_DoorsAScriptHasExplicitlyLocked;

void ScriptLockOrUnlockDoor( Vec3 doorPos, const char * msg, int lockOrUnlock ) 
{
    ScriptDoor * door = 0;
    int count, i;

    //If this door is in the Array get it, if not, add it.
    if( !g_DoorsAScriptHasExplicitlyLocked )
        eaCreate(&g_DoorsAScriptHasExplicitlyLocked);

    count = eaSize( &g_DoorsAScriptHasExplicitlyLocked );
    for( i = 0 ; i < count ; i++ )
    {
        if( nearSameVec3Tol( doorPos, g_DoorsAScriptHasExplicitlyLocked[i]->pos, 15.0f ) )
            door = g_DoorsAScriptHasExplicitlyLocked[i];
    }
    if( !door )
    {
        door = calloc( sizeof( ScriptDoor ), 1 );
        copyVec3( doorPos, door->pos );
        eaPush( &g_DoorsAScriptHasExplicitlyLocked, door );
    }

    //Then set it's status and msg
    if( msg )
        strncpy( door->msg, msg, ARRAY_SIZE(door->msg)-1 );
    else
        door->msg[0] = 0;

    door->status = lockOrUnlock;

    //Frankly I dont know why we did this elaborate ScriptDoor system instead of just finding the 
    //Nearby door and locking it.
    //Note that getTheDoorThisGeneratorspawned is a bad name -- it just finds the nearest door to this point
    {
        Entity* doorent = getTheDoorThisGeneratorSpawned(doorPos);
        if( doorent )
        {
            //FLASH the locking and unlocking bits (TO DO add these flashes to keydoor as well
            if( lockOrUnlock ) 
                SETB( doorent->seq->state, STATE_LOCKING );
            else
                SETB( doorent->seq->state, STATE_UNLOCKING );
        }
    }
}

//Called when map needs to be cleaned up and reset
void ClearAllDoorsLockedByScript()
{
    ScriptDoor * door = 0;
    int count, i;

    if( !g_DoorsAScriptHasExplicitlyLocked )
        return;

    count = eaSize( &g_DoorsAScriptHasExplicitlyLocked );
    for( i = 0 ; i < count ; i++ )
    {
        free(g_DoorsAScriptHasExplicitlyLocked[i]);
    }
    
    eaDestroy(&g_DoorsAScriptHasExplicitlyLocked);
}

//If door is locked, returns the locked message
//I have a two foot tolerance from marker to door
#define TOLERANCE_BETWEEN_SCRIPTMARKER_WITH_DOORS_NAME_AND_DOOR_ENTITY 15.0f
//TO DO I really should move the script marker from the door entity to the spawnlocation object 
char * IsDoorLockedByScript( const Vec3 doorPos )
{
    ScriptDoor * door;
    int i, count;
    
    count = eaSize( &g_DoorsAScriptHasExplicitlyLocked );
    for( i = 0 ; i < count ; i++ )
    {
        door = g_DoorsAScriptHasExplicitlyLocked[i];
        if( nearSameVec3Tol( door->pos, doorPos, 15.0f ) && door->status == DOOR_LOCKED )
        {
            if( !door->msg )
                return "DoorLocked";
            return door->msg;
        }
    }
    return 0; //Door is open
}

/// End Script Managed Doors ###############################################################################

static void doorPathFollowersThroughDoor(Entity* e, DoorEntry* doorIn, DoorEntry* doorOut)
{
    int i;
    static char* behaviorString = NULL;
    EntityRef leaderRef = erGetRef(e);

    estrClear(&behaviorString);

    for(i = e->petList_size-1; i >= 0; i--)
    {
        Entity* pet = erGetEnt(e->petList[i]);

        if(pet && pet->pchar)
        {
            AIVars* ai = ENTAI(pet);
            if (ai && ai->followTarget.type == AI_FOLLOWTARGET_POSITION)
            {
                aiDiscardFollowTarget(pet, "Entering door", 0);
            }
            estrPrintf(&behaviorString, "RunThroughDoor(DoorIn(%d),DoorOut(%d),Leader(%I64x))",
                doorIn, doorOut, leaderRef);
            aiBehaviorAddString(pet, ENTAI(pet)->base, behaviorString);
        }
    }
}

static void doorTransferToTarget( Entity * e, DoorEntry * door, char * spawnTarget, char * mapTarget, char * errmsg, Vec3 pos, char* selected_map_id, int is_volume ) 
{
    MapSelection* mapSelect;
    StaticMapInfo* mapInfo;
    int missionReturn = 0;
    int mapID = 0;
    Vec3    nowhere = {0};

    if( !stricmp(spawnTarget, "MissionReturn") || !stricmp(spawnTarget, "BaseReturn"))
    {
        MapHistory *hist = mapHistoryLast(e->pl);
        if (!stricmp(spawnTarget, "MissionReturn") && hist && hist->mapType == MAPTYPE_BASE)
        { //if we just came from a base and we're doing a mission return, go to base
            DoorAnimEnter(e, door, nowhere, 0, SGBaseConstructInfoString(hist->mapID), spawnTarget, 0, errmsg);
            return;
        }
        else if (!stricmp(spawnTarget, "MissionReturn") && hist && hist->mapType == MAPTYPE_MISSION)
        { //If we came from a mission and we're on a mission, go back
            // NOTE: figure out how to do this, for now go back to static map
            mapInfo = staticMapInfoFind(e->static_map_id);
        }
        else
        { // Go back to the static map
            mapInfo = staticMapInfoFind(e->static_map_id);
        }        
        missionReturn = 1;
    }
    else
    {
        mapID = dbGetMapId(mapTarget);
        mapInfo = staticMapInfoFind(mapID);
    }

    if(mapInfo || door->monorail_line || (door->detail_id && door->type == DOORTYPE_BASE_PORTER))
    {
        mapSelect = checkDoorOptions(e,    mapInfo ? mapInfo->baseMapID : 0, pos,door, selected_map_id, missionReturn, 
                                        is_volume ? DOOR_REQUEST_VOLUME : DOOR_REQUEST_CLICK, door->type == DOORTYPE_BASE_PORTER ? door->detail_id : 0);
        if (mapSelect->spawn_name[0] && ((door->monorail_line && door->monorail_line[0]) || door->type == DOORTYPE_BASE_PORTER))
            strcpy(spawnTarget,mapSelect->spawn_name); //if we picked an option with a spawn, use it
        if(mapSelect->map_id && DoorAnimEnter(e, door, pos, mapSelect->map_id, NULL, spawnTarget, XFER_STATIC, errmsg))
        {
            // success
        }
        else if (mapSelect->mowner) // doing a mission transfer
        {
            if(db_state.local_server)
            {
                enterLocalMission(e, door->db_id, mapSelect->mowner, NULL, errmsg);
            }
            else if (lockTeamupForDoor(e))
            {
                if (attachTeamupToMission(e, 0, mapSelect->mowner, &mapSelect->sahandle, errmsg))
                {
                    if (enterMissionDoor(e, door, pos, 0, errmsg))
                    {
                        // success
                    }
                }
                teamlogPrintf(__FUNCTION__, "unlocking team");
                teamUpdateUnlock(e, CONTAINER_TEAMUPS);
            }
            else
            {
                log_printf("missiondoor.log", "enterDoor: I wasn't able to lock the teamup??");
                strcpy(errmsg, localizedPrintf(e,"DoorLocked")); // something's terribly wrong
            }
        }
    }
    else // not a static map id
    {
        if (door->map_id > 0) 
            mapID = door->map_id;
        if (mapID > 0 && DoorAnimEnter(e, door, pos, mapID, NULL, spawnTarget, XFER_STATIC, errmsg))
        {
            // success
            doorPathFollowersThroughDoor(e, door, dbFindSpawnLocation(spawnTarget));
        }
    }
}

// Old map info from before entering the map
char oldmap[1000] = {0};
Vec3 oldpos = {0};

int inLocalMission = false;            // used to correctly shut down local missions
extern void    GlowieClearAll(void);            

// Reloads the map for a mission when in local mapserver
void enterLocalMission(Entity* player, int doorId, int mowner, char* mapfile, char* errmsg)
{
    StoryInfo* info = StoryInfoFromPlayer(player);
    int n = eaSize(&info->tasks);
    VillainGroupEnum villainGroup;
    int i;

    assert(db_state.local_server);
    
    // Find and load the new map
    for (i=0; i<n; i++)
    {
        StoryTaskInfo* task = info->tasks[i];
        if (eaSize(&task->def->missiondef) && task->doorId == doorId)
        {
            DoorEntry* missionStart;
            const MissionDef* mission = task->def->missiondef[0];
            char* missioninfo;
            char tempMap[255];
            char* nextMap = NULL;
            EncounterAlliance encounterAlliance;
            ScriptVarsTable vars = {0};

            // Shouldn't be a potential mismatch
            saBuildScriptVarsTable(&vars, player, task->assignedDbId, TaskGetContactHandle(info, task), task, NULL, NULL, NULL, NULL, 0, 0, false);

            inLocalMission = true;

            // Find the villain group
            villainGroup = getVillainGroupFromTask(task->def);
            if (villainGroup == villainGroupGetEnum("Random"))
                villainGroup = task->villainGroup;
            else if (villainGroup == villainGroupGetEnum("Var"))
                villainGroup = StaticDefineIntGetInt(ParseVillainGroupEnum, ScriptVarsTableLookup(&vars, mission->villaingroupVar));

            // Get the correct map
            if (mapfile)
                nextMap = mapfile;
            if (!nextMap)
            {
                strcpy_s(SAFESTR(tempMap), MissionGetMapName(player, task, TaskGetContactHandle(info, task)));
                nextMap = tempMap;
            }
            
            // Don't reload the same map
            // This may fail because groupLoadMap() messes with the strings
            if (nextMap && stricmp(nextMap, db_state.map_name))
            {
                // Save off the old map name and load the map
                strcpy(oldmap, db_state.map_name);
                groupLoadMap(nextMap, 0, 1);
            }
            
            // Attach the mission to the team
            if (lockTeamupForDoor(player))
            {
                if (task->def && !task->timeout && TASK_DELAY_START(task->def))
                {
                    task->failOnTimeout = true;
                    task->timerType = -1;
                    task->timeout = dbSecondsSince2000() + task->def->timeoutMins * 60;
                    task->timezero = task->timeout;
                }
                attachTeamupToMission(player, doorId, mowner, tempStoryTaskHandle(0, 0, 0,0), errmsg);
                teamlogPrintf(__FUNCTION__, "unlocking team");
                teamUpdateUnlock(player, CONTAINER_TEAMUPS);
            }
            else
            {
                log_printf("missiondoor.log", "enterDoor: I wasn't able to lock the teamup??");
                strcpy(errmsg, localizedPrintf(player, "DoorLocked")); // something's terribly wrong
            }

            if (ENT_IS_ON_RED_SIDE(player))
            {
                encounterAlliance = ENC_FOR_VILLAIN;
            }
            else
            {
                encounterAlliance = ENC_FOR_HERO;
            }

            // Initialize the mission map
            missioninfo   = MissionConstructInfoString(player->db_id, 0, 0, TaskGetContactHandle(info, task),
                            &task->sahandle, task->seed, task->level,
                            &task->difficulty, task->completeSideObjectives, encounterAlliance, villainGroup);
            MissionSetInfoString(missioninfo,0);
            GlowieClearAll();            
            initMap(1);
            MissionInitMap();
            
            // Move the character to the mission start
            copyVec3(ENTPOS(player), oldpos);
            missionStart = dbFindSpawnLocation("MissionStart");
            if (missionStart)
                entUpdatePosInstantaneous(player, missionStart->mat[3]);
            MissionPlayerEnteredMap(player);
            ScriptPlayerEnteringMap(player);
            pvp_ApplyPvPPowers(player);
            return;
        }
    }
    if (errmsg)
        strcpy(errmsg, localizedPrintf(player, "DoorLocked"));
}

// Reloads the map for a mission when in local mapserver
void exitLocalMission(Entity* player)
{
    // If no player was passed in, we need to find him
    // Should only be one player on the map when in local mapserver mode
    if (!player)
    {
        PlayerEnts *players = &player_ents[PLAYER_ENTS_ACTIVE];
        if (!players->count || !players->ents[0])
            return;
        player = players->ents[0];
    }

    if (oldmap && oldmap[0])
    {
        inLocalMission = false;


        sendDoorMsg( player, DOOR_MSG_OK, 0 );
        groupLoadMap(oldmap, 0, 1);
        EncounterSetMissionMode(0);
        GlowieClearAll();            
        initMap(1);
        eaClearEx(&objectiveInfoList, NULL);

        if (oldpos)
            entUpdatePosInstantaneous(player, oldpos);
        StoryArcPlayerEnteredMap(player);
    }
    else 
    {
        sendDoorMsg( player, DOOR_MSG_FAIL, localizedPrintf(player,"DoorLocked") );
    }
}

bool CanEnterDoor(Entity *e)
{
    return !(e->dbcomm_state.in_map_xfer || e->pl->door_anim || e->adminFrozen);
}

#define ARCHITECT_DOOR_WAIT 3
MP_DEFINE(ArchitectDoorRequest);
static ArchitectDoorRequest **s_architectDoors = 0;
static int *s_architectIdsRequested = 0;
static char archerrmsg[1000] = "";

static void s_enterArchitectMap(Entity *e, Vec3 pos, char *errmsg)
{
    DoorEntry    *door = NULL;
    if(pos)    //how could pos be NULL??
        door = dbFindGlobalMapObj(db_state.base_map_id, pos);    //if this is a seamless zoning beacon, architect doors need to be rethought.
    if(!e || !door)
    {
        if(!door)
        {
            log_printf("missiondoor.log", "s_enterArchitectMap: I lost my door during MA evaluation.  How did that happen?");
        }
        return;
    }

    if(errmsg)
        errmsg[0] = 0;

    if (lockTeamupForDoor(e))
    {
        if (attachTeamupToMission(e, door->db_id, 0, tempStoryTaskHandle(0, 0, 0, 0), errmsg))
        {
            if (enterMissionDoor(e, door, pos, 0, errmsg))
            {
                // success
            }
        }
        else
        {
            // try to trick or treat if this team didn't have a mission here
            if (TrickOrTreat(e, pos))
                errmsg[0] = 0;
        }
        teamlogPrintf(__FUNCTION__, "unlocking team");
        teamUpdateUnlock(e, CONTAINER_TEAMUPS);
    }
    else
    {
        log_printf("missiondoor.log", "enterDoor: I wasn't able to lock the teamup??");
        strcpy(errmsg, localizedPrintf(e,"DoorLocked")); // something's terribly wrong
    }

    if (db_state.local_server)
        exitLocalMission(e);
    else
    {
        if (errmsg && errmsg[0])
            sendDoorMsg( e, DOOR_MSG_FAIL, errmsg );
        else
            sendDoorMsg( e, DOOR_MSG_OK, 0 );
    }
}

void DoorArchitectEntryTick()
{
    static U32 lastCheck = 0;
    U32 now = dbSecondsSince2000();
    if(now != lastCheck)
    {
        int totalRequests = eaSize(&s_architectDoors);
        int i;
        for(i = totalRequests-1; i >= 0; i--)
        {
            ArchitectDoorRequest *request = s_architectDoors[i];
            if(request && request->timeEnqueued<= now-ARCHITECT_DOOR_WAIT)
            {
                Entity *e = 0;
                //we need to remove the id since we're probably not expecting a response at this point.
                eaiSortedFindAndRemove(&s_architectIdsRequested, request->architectId);
                e = erGetEnt(request->eRef);
                if(e)
                    s_enterArchitectMap(e, request->pos, archerrmsg);
                MP_FREE(ArchitectDoorRequest, request);
                eaRemove(&s_architectDoors, i);
            }
        }
        if(!totalRequests && eaiSize(&s_architectIdsRequested))
            eaiPopAll(&s_architectIdsRequested);
        lastCheck = now;
    }
}

int DoorArchitectIsValid(Entity *e, int honors)
{
    int validateImmune = missionhonors_isInvalidationImmune(honors);
    if(!validateImmune)
    {
        return playerCreatedStoryArc_ValidateFromString((char*)unescapeAndUnpack(e->taskforce->playerCreatedArc), 1);
    }
    return 1;
}

void DoorArchitectBanUpdate(U32 arcId, int banstatus, int honors)
{
    int totalRequests = eaSize(&s_architectDoors);
    int i;
    for(i = totalRequests-1; i >= 0; i--)
    {
        ArchitectDoorRequest *request = s_architectDoors[i];
        if(request && request->architectId==arcId)
        {
            Entity *e = erGetEnt(request->eRef);
            if(!e || !e->taskforce || !e->taskforce->playerCreatedArc)
            {
                //don't send anything, just fail.
            }
            else if(!DoorArchitectIsValid(e, honors))
            {
                sendDoorMsg( e, DOOR_MSG_FAIL, localizedPrintf(e,"MADoorInvalid") );
                architectForceQuitTaskforce(e);
            }
            else if(missionban_isVisible(banstatus))
                s_enterArchitectMap(e, request->pos, archerrmsg);
            else
            {
                sendDoorMsg( e, DOOR_MSG_FAIL, localizedPrintf(e,"MADoorBanned") );
                architectForceQuitTaskforce(e);
            }
            MP_FREE(ArchitectDoorRequest, request);
            eaRemove(&s_architectDoors, i);
        }
    }
    eaiSortedFindAndRemove(&s_architectIdsRequested, arcId);
}

static void s_enqueueArchitectDoor(Entity *e, Vec3 pos)
{
    ArchitectDoorRequest *request = 0;
    int valid = 0;
    if(!e || !e->taskforce || !e->taskforce->playerCreatedArc)
        return;

    assert(TaskForceIsOnArchitect(e));

    if(e->taskforce->architect_id)
    {
        MP_CREATE(ArchitectDoorRequest, 64);
        request = MP_ALLOC( ArchitectDoorRequest );
        request->eRef =erGetRef(e);
        request->timeEnqueued = dbSecondsSince2000();
        request->architectId = e->taskforce->architect_id;
        copyVec3(pos,request->pos);
        eaPush(&s_architectDoors, request);
        if(eaiSortedFind(&s_architectIdsRequested, e->taskforce_id) == -1)
        {
            eaiSortedPushUnique(&s_architectIdsRequested, e->taskforce->architect_id);
            missionserver_map_requestBanStatus(e->taskforce->architect_id);
        }
        
    }
    else
    {
        s_enterArchitectMap(e, pos, archerrmsg);
    }
}

// this can be triggered by clicking on a door, or walking into a door volume
void enterDoor(Entity *e,Vec3 pos,char* selected_map_id,int is_volume,StashTable volume_props)
{
    DoorEntry    *door = NULL;
    DoorEntry    doorBuffer;
    char        specialMapReturnData[SPECIAL_MAP_RETURN_DATA_LEN];
    char        errmsg[1000] = "";
    PropertyEnt    *infoMatch = NULL;
    int canEnterDoor = 0; //for the static doors and keydoors
    int needToAskFirst = 0; //for doors that require a yes/no confirmation
    char spawnTarget[256] = {0};
    char mapTarget[256] = {0};
    
     if (!CanEnterDoor(e))
        return;

    if (is_volume && volume_props)
    {
        stashFindPointer(volume_props, "SeamlessZoningBeacon", &infoMatch);
    }

    if (infoMatch && infoMatch->value_str && infoMatch->value_str[0])
    {
        door = dbFindLocalDoorWithName(DOORTYPE_GOTOSPAWNABSPOS,infoMatch->value_str);
    }
    else
    {
        door = dbFindLocalMapObj(pos,0,MAX_DOOR_BEACON_DIST);
    }

    if (!door)
    {
        // try to trick or treat at unlinked doors
        if (!TrickOrTreat(e, pos))
            strcpy(errmsg,localizedPrintf(e,"DoorLocked")); // 9/30/03 - not all doors have to have map links.  if not, they pretend they are locked
    }
    else if (door->type == DOORTYPE_LOCKED)
    {
        strcpy(errmsg,localizedPrintf(e,"DoorLocked"));
    }
    else if (door->type == DOORTYPE_FREEOPEN)
    {
        Entity* doorent = getTheDoorThisGeneratorSpawned(pos);
        char * msg = 0;

        if (doorent) 
        {
            msg = IsDoorLockedByScript(ENTPOS(doorent));
            if ( /* OnInstancedMap() && */ msg ) // maybe extend these to city zones later?
            {
                strcpy(errmsg, localizedPrintf(e,msg));
            }
            else
            {
                // Only open the door if the script says it's not locked.
                aiDoorSetOpen(doorent, 1);
            }
        }
    }
    else if (door->type == DOORTYPE_FLASHBACK_CONTACT)
    {
        // Am I on a flashback mission?
        if (TaskForceIsOnFlashback(e))
        {
            Vec3 pos;
            char moveCommand[1000];
            int mapID;
            const MapSpec* mapSpec;
            int haveloc = 0;
            int characterLevel = character_GetSecurityLevelExtern(e->pchar); // +1 since iLevel is zero-based.

            // move players to contact
            haveloc = ContactFindLocation(PlayerTaskForceContact(e), &mapID, &pos);
            if (!haveloc)
            {
                strcpy(errmsg, localizedPrintf(e,"DontKnowWhereContactIs"));
            } else {
                // use teleport helper function
                if(mapID == db_state.base_map_id)
                    mapID = db_state.map_id;

                mapSpec = MapSpecFromMapId(mapID);

                if (mapSpec)
                {
                    if (characterLevel < mapSpec->entryLevelRange.min)
                    {
                        strcpy(errmsg, localizedPrintf(e, "TooLowToEnter", 
                                mapSpec->entryLevelRange.min) );
                        haveloc = false;
                    }
                    else if (characterLevel > mapSpec->entryLevelRange.max)
                    {
                        strcpy(errmsg, localizedPrintf(e,"TooHighToEnter", 
                                mapSpec->entryLevelRange.max));
                        haveloc = false;
                    }
                }

                if (haveloc) 
                {
                    sprintf(moveCommand, "mapmovepos \"%s\" %d %f %f %f 0 1", e->name, mapID, pos[0], pos[1], pos[2]);
                    serverParseClient(moveCommand, NULL);
                }
            }

        } else {
            strcpy(errmsg, localizedPrintf(e,"NotOnFlashbackMission"));
        }
    }
    else if (door->type == DOORTYPE_SGBASE)
    {
        // Super Group Bases are not available until the last stage
        LWC_STAGE sgbaseStage;

        if (!LWC_CheckSGBaseReady(LWC_GetCurrentStage(e), &sgbaseStage))
        {
            strcpy(errmsg,localizedPrintf(e, "LWCDataNotReadyMap", sgbaseStage));
        }
        else
        {
            bool mayBeAbleToEnter = s_EnterBases(e,pos);
            
            if( !mayBeAbleToEnter )
            {
                strcpy(errmsg, localizedPrintf(e,"SGDoesNotOwnBase"));
            }
        }
    }
    else if (door->type == DOORTYPE_KEY)
    {
        Entity* doorent = getTheDoorThisGeneratorSpawned(pos);
        char * msg = 0;

        if( doorent ) //TO DO : can I use the door mat?
            msg = IsDoorLockedByScript(ENTPOS(doorent));

        if ( /* OnInstancedMap() && */ msg ) // maybe extend these to city zones later?
        {
            strcpy(errmsg, saUtilScriptLocalize(msg));
        }
        else if (OnMissionMap() && KeyDoorLocked(door->name)) // maybe extend these to city zones later?
        {
            strcpy(errmsg, localizedPrintf(e,"NeedKey"));
        }
        else if (door->special_info) // acting as GotoSpawn
        {
            canEnterDoor = 1;
            strcpy( spawnTarget, door->special_info );
        }
        else
        {
            if (doorent) 
                aiDoorSetOpen(doorent, 1);
        }
        // Checks to see if it was a keydoor, and then removes the waypoint from the player
        KeyDoorUnlockUpdate(e, door->name);
    }
    else if (door->type == DOORTYPE_MISSIONLOCATION)
    {
        door = dbFindGlobalMapObj(db_state.base_map_id, pos);    // need to get proper db_id, etc.

        if (!door)
        {
            // sometimes happens when a door has a lair type on a mission map
            strcpy(errmsg, localizedPrintf(e,"DoorLocked"));  
        }
        else if (db_state.local_server)
        {
            enterLocalMission(e, door->db_id, 0, NULL, errmsg);
        }
        else
        {
            assert(door->db_id);

            if(TaskForceIsOnArchitect(e))
            {
                //place the ent and door into a queue so that we can send them off once we know their Mission is valid and unbanned.
                s_enqueueArchitectDoor(e, pos);
                return;
            }

            if (lockTeamupForDoor(e))
            {
                if (attachTeamupToMission(e, door->db_id, 0, tempStoryTaskHandle(0, 0, 0, 0), errmsg))
                {
                    if (enterMissionDoor(e, door, pos, 0, errmsg))
                    {
                        // success
                    }
                }
                else
                {
                    // try to trick or treat if this team didn't have a mission here
                    if (TrickOrTreat(e, pos))
                        errmsg[0] = 0;
                }
                teamlogPrintf(__FUNCTION__, "unlocking team");
                teamUpdateUnlock(e, CONTAINER_TEAMUPS);
            }
            else
            {
                log_printf("missiondoor.log", "enterDoor: I wasn't able to lock the teamup??");
                strcpy(errmsg, localizedPrintf(e,"DoorLocked")); // something's terribly wrong
            }
        }
    } // mission door
    else if (door->type == DOORTYPE_BASE_PORTER && g_isBaseRaid && stricmp(door->name,"RaidTeleporter") == 0)
    {
        int i;
        Vec3 target;
        Vec3 offset;

        if (sgRaidFindOtherEndOfBaseRaidTeleportPair(pos, target, BASE_TELEPORT_RAIDPORT) &&
                                    !ScriptPlayerTeleporting(e))
        {
// TODO  This still needs work if we are to be interruptible.
// The easy way is to pass a second parameter to ScriptPlayerTeleporting that determines if this is a "desired" (i.e. click time)
// call (used to determine if teleport is possible), or an activation call (used to do whatever to a player on a port, e.g. drop the flag)
// The hard way is a second script closure, like current clickies
            // Evaluate the "slop" factor to move to one of the actual target locations
            sgRaidGetRaidPortOffset(target, offset);
            addVec3(target, offset, target);

            // And off we go.
            entUpdatePosInstantaneous(e, target);

            // Now drag all the pets along
            for(i = e->petList_size - 1; i >= 0; i--)
            {
                Entity* pet = erGetEnt(e->petList[i]);
                if (pet && pet->pchar)
                {
                    // HACK HACK - shift each pet by a little bit
                    // If this turns out to be a problem, we can get smarter.
                    target[i & 1 ? 2 : 0] += 0.5f;
                    entUpdatePosInstantaneous(pet, target);
                }
            }
        }
    }
    else if (door->type == DOORTYPE_BASE_PORTER && g_base.rooms && (!e->supergroup || e->supergroup_id != g_base.supergroup_id) && RaidIsRunning() )
    { //we're trying to use an enemies door
        strcpy(errmsg,localizedPrintf(e,"DoorLocked")); 
    }
    else if (door->type == DOORTYPE_BASE_PORTER && stricmp(door->name,"RaidTeleporter") == 0)
    {
        Vec3 zero = {0};
        U32 raidid = RaidSGIsAttacking(e->supergroup_id);
        U32 othersg = RaidGetDefendingSG(raidid);
        RoomDetail *det = baseGetDetail(&g_base,door->detail_id,NULL);

        if (roomDetailLookUnpowered(det))
        {            
            Strcpy(errmsg,localizedPrintf(e,"DetailPoweredOff"));
        }
        else if (raidid) // we have an active raid
        {
            if (RaidPlayerIsAttacking(e))
                DoorAnimEnter(e, NULL, zero, 0, SGBaseConstructInfoString(othersg), "PlayerSpawn", XFER_BASE, NULL);
            else if (RaidHasOpenSlot(e->supergroup_id))
            {
                RaidRequestParticipateAndTransfer(e, raidid, SGBaseConstructInfoString(othersg));
                return; // !!! - crazy, but request is going to handle sendDoorMsg later
            }
            else
                strcpy(errmsg, localizedPrintf(e, "RaidParticipateFull"));
        }
        else if (e->supergroup && e->supergroup->activetask) //if we should start a trial
        {
            
            if (lockTeamupForDoor(e))
            {
                StoryTaskInfo* mission = e->supergroup->activetask;
                if (attachTeamupToMission(e, 0, e->supergroup_id, &mission->sahandle, errmsg))
                    enterMissionDoor(e, door, pos, 0, errmsg);
                teamlogPrintf(__FUNCTION__, "unlocking team");
                teamUpdateUnlock(e, CONTAINER_TEAMUPS);
            }
            else
            {
                log_printf("missiondoor.log", "enterDoor: I wasn't able to lock the teamup??");
                strcpy(errmsg, localizedPrintf(e,"DoorLocked")); // something's terribly wrong
            }
        }
        else if (!e->supergroup || e->supergroup_id != g_base.supergroup_id)
        {
            // I'm not in this supergroup, but I'm taggin along on their raid
            if(    e->teamup && e->teamup->activetask && TASK_IS_SGMISSION(e->teamup->activetask->def) )
            {
                if (lockTeamupForDoor(e))
                {
                    StoryTaskInfo* mission = e->teamup->activetask;
                    if (attachTeamupToMission(e, 0, g_base.supergroup_id, &mission->sahandle, errmsg))
                        enterMissionDoor(e, door, pos, 0, errmsg);
                    teamlogPrintf(__FUNCTION__, "unlocking team");
                    teamUpdateUnlock(e, CONTAINER_TEAMUPS);
                }
                else
                {
                    log_printf("missiondoor.log", "enterDoor: I wasn't able to lock the teamup??");
                    strcpy(errmsg, localizedPrintf(e,"DoorLocked")); // something's terribly wrong
                }
            }
            else
                strcpy(errmsg,localizedPrintf(e,"DoorLocked"));
        }
        else 
        {
            strcpy(errmsg, localizedPrintf(e,"SGRaidNoRaid"));
        }
    }
    else if (door->type == DOORTYPE_GOTOSPAWN && g_isBaseRaid)
    {
        int i;
        Vec3 target;

        if (sgRaidFindOtherEndOfBaseRaidTeleportPair(pos, target, BASE_TELEPORT_ENTRANCE) && 
                            !ScriptPlayerTeleporting(e))
        {
            // Hack - offset 3 feet.  We should be more intelligent about this.
            target[0] += 5.0f;

            // And off we go.
            entUpdatePosInstantaneous(e, target);

            // Now drag all the pets along
            for(i = e->petList_size - 1; i >= 0; i--)
            {
                Entity* pet = erGetEnt(e->petList[i]);
                if (pet && pet->pchar)
                {
                    // HACK HACK - shift each pet by a little bit
                    target[2] += 0.5f;
                    entUpdatePosInstantaneous(pet, target);
                }
            }
        }
    }
    else // static door
    {
        int characterLevel = character_GetSecurityLevelExtern(e->pchar); // +1 since iLevel is zero-based.
        int mapID = 0;
        StaticMapInfo* mapInfo;
        const MapSpec* mapSpec;
        int minLevel;
        int maxLevel;
        
        canEnterDoor = 1;

        if (door->special_info && strnicmp(door->special_info, "StaticReturn", 12 /* == strlen("StaticReturn") */ ) == 0)
        {
            // This is a rather ugly hack, but it keeps the damage to the bare minimum.  StaticReturn doors don't
            // have the target gotomap/gotospawn directly baked in.  Instead, they have a tag, which is compared
            // to the player's specialMapReturnData string.  If that matches, the rest of the specialMapReturnData
            // string is parsed for a map and spawn target, these are injected into a copy of the original
            // DoorEntry, and we go to work with that.
            // If anything goes wrong with this, we just drop back ten and punt, sending heroes to Atlas, and
            // villains to Mercy.

            char *tag;
            char *mapName;
            char *spawn_target;

            // Make a copy, and point to it.
            doorBuffer = *door;
            door = &doorBuffer;
            // Grab a local copy of the player's specialMapReturnData
            strncpyt(specialMapReturnData, e->pl->specialMapReturnData, SPECIAL_MAP_RETURN_DATA_LEN);

            // decompose into tag, mapname, and spawn_target.  If anything goes wrong, spawn_target is
            // going to be NULL.
            tag = specialMapReturnData;
            mapName = strchr(tag, '=');
            if (mapName)
            {
                *mapName++ = 0;
                spawn_target = strchr(mapName, ';');
                if (spawn_target)
                {
                    *spawn_target++ = 0;
                }
            }
            else
            {
                spawn_target = NULL;
            }
            // If they're already in flight, do nothing.
            if (e->pl->specialMapReturnInProgress)
            {
                canEnterDoor = 0;
                errmsg[0] = 0;
            }
            // Check that the tag matches.
            else if (spawn_target != NULL && stricmp(door->name, tag) == 0)
            {
                door->name = spawn_target;
                door->special_info = mapName;
                // Do this in here so we don't disturb data that has a different tag.
                e->pl->specialMapReturnData[0] = 0;
            }
            // Something wrong.  Use some standard values for safe places in Atlas, Mercy and Praetoria.
            else if (e && e->pl && e->pl->praetorianProgress == kPraetorianProgress_Tutorial)
            {
                door->name = "NewPlayer";
                door->special_info = "P_City_00_05.txt";
            }
            else if (e && e->pl && e->pl->praetorianProgress == kPraetorianProgress_Praetoria)
            {
                door->name = "NewPlayer";
                door->special_info = "P_City_00_02.txt";
            }
            else if (ENT_IS_VILLAIN(e))
            {
                // This places them close to Kalinda
                door->name = "NewPlayer";
                door->special_info = "V_City_01_01.txt";
            }
            else
            {
                // Use the new player spawn close to Ms. Liberty.
                door->name = "NewPlayer";
                door->special_info = "City_01_01.txt";
            }
            // And finally flag that a special return is in progress.
            e->pl->specialMapReturnInProgress = 1;
        }

        mapID = dbGetMapId(door->special_info);
        mapInfo = staticMapInfoFind(mapID);
        mapSpec = MapSpecFromMapId(mapID);
        
        if(mapInfo)
        {
            int i,j;

            if (server_state.lock_doors) {
                sprintf(errmsg, "Doors are locked! Use /lockdoors 0 to unlock.");
                sendDoorMsg(e, DOOR_MSG_FAIL, errmsg);
                return;
            }

            // This is for inter-map doors
            // check for 
            if (mapSpec) {
                // Grab the character level range specified by the map, accounting for any overrides.
                minLevel = (door->entry_min == INVALID_DOOR_ENTRY_LEVEL ? mapSpec->entryLevelRange.min : door->entry_min);
                maxLevel = (door->entry_max == INVALID_DOOR_ENTRY_LEVEL ? mapSpec->entryLevelRange.max : door->entry_max);
            } else {
                sprintf(errmsg,"GotoMap error: map %s not found in maps.specs",door->special_info);
                log_printf("doors.log", "GotoMap error: map %s not found in maps.specs",door->special_info);
            }

            for(i = eaSize(&server_state.blockedMapKeys)-1; canEnterDoor && i >= 0; i--)
            {
                for(j = eaSize(&mapInfo->mapKeys)-1; canEnterDoor && j >= 0; j--)
                {
                    if(!stricmp(server_state.blockedMapKeys[i], mapInfo->mapKeys[j]))
                    {
                        // for now, this will be overridden by other messages that apply too...
                        // this seems to make sense as I'm assuming the blocking to be temporary
                        Strcpy(errmsg, localizedPrintf(e,"MapKeyIsBlocked"));
                        canEnterDoor = 0;
                    }
                }
            }
        }
        else
        {
            // This is for intra-map doors
            minLevel = (door->entry_min == INVALID_DOOR_ENTRY_LEVEL ? 0 : door->entry_min);
            maxLevel = (door->entry_max == INVALID_DOOR_ENTRY_LEVEL ? 9999 : door->entry_max);
        }

        if (mapID < 0 && door->special_info && !door->detail_id)
        {
            sprintf(errmsg,"GotoMap error: map %s not found in maps.db",door->special_info);
            log_printf("doors.log", "GotoMap error: map %s not found in maps.db",door->special_info);
            canEnterDoor = 0;
        }
        else if (characterLevel < minLevel)
        {
            Strcpy(errmsg,localizedPrintf(e,"TooLowToEnter", minLevel) );
            canEnterDoor = 0;
        }
        else if (characterLevel > maxLevel)
        {
            Strcpy(errmsg,localizedPrintf(e,"TooHighToEnter", maxLevel));
            canEnterDoor = 0;
        }
        else if (door->detail_id && g_base.rooms)
        {
            RoomDetail *det = baseGetDetail(&g_base,door->detail_id,NULL);
            if (roomDetailLookUnpowered(det))
            {
                canEnterDoor = 0;
                Strcpy(errmsg,localizedPrintf(e,"DetailPoweredOff"));
            }
        }

        //TO DO: make this able to handle rail lines and such 
        if( strstri( door->name, "MoleDoor" ) && 0 == getRewardToken( e, door->name ) )
        {
            Strcpy( errmsg, localizedPrintf(e,"DoorLocked") );
            canEnterDoor = 0;
        }

        //TO DO, this just needs to be a property on the goto spawn, not a separate thing
        { 
            char * msg2 = 0; 
            Entity* doorent2 = getTheDoorThisGeneratorSpawned(pos);

            if( doorent2 ) //TO DO : can I use the door mat?
            {
                if( msg2 = IsDoorLockedByScript(ENTPOS(doorent2)) ) 
                {
                    strcpy( errmsg, saUtilScriptLocalize(msg2) );
                    canEnterDoor = 0;
                }
            }
        }


        if (door->type == DOORTYPE_BASE_PORTER)
        {
            strcpy(spawnTarget,"BaseTeleportSpawn");
        }
        else
        {
            Vec3 pos = { 0.0f };

            copyVec3(ENTPOS(e), pos);

            if (door->type == DOORTYPE_GOTOSPAWNABSPOS)
                snprintf( spawnTarget, 255, "abspos:%.1f,%.1f,%.1f,%s", pos[0], pos[1], pos[2], door->name );
            else
                strcpy( spawnTarget, door->name );

            if( door->special_info )
                strcpy( mapTarget, door->special_info );
        }

            
        if( door->villainOnly && ENT_IS_HERO(e) )
        {
            Strcpy( errmsg, localizedPrintf(e,"VillainOnly") );
            canEnterDoor = 0;
        }
        if( door->heroOnly && ENT_IS_VILLAIN(e) )
        {
            Strcpy( errmsg, localizedPrintf(e,"HeroOnly") );
            canEnterDoor = 0;
        }
        if (door->required)
        {
            char        *required;
            char        *argv[100];
            StringArray expr;
            int            argc;
            int            i;

            strdup_alloca(required, door->required);
            argc = tokenize_line(required, argv, 0);
            if (argc > 0)
            {
                eaCreate(&expr);
                for (i = 0; i < argc; i++)
                    eaPush(&expr, argv[i]);

                if (!chareval_Eval(e->pchar, expr, door->filename))
                {
                    if (door->lockedText)
                        Strcpy( errmsg, localizedPrintf(e, door->lockedText) );
                    else 
                        Strcpy( errmsg, localizedPrintf(e,"DoorLocked") );
                    canEnterDoor = 0;
                }
            }
        }
        
        // OK, so what's with the '@' test?  The first time through here, it will be absent, so this loop goes through all it's work.  If we get a case where
        // allowThrough is true, but there's text, we don't allow passage immediately, instead the user is presented with a yes/no popup.  If they select yes,
        // it eventually reaches here again, but this time an '@' has been prepended to the selected map id
        if (canEnterDoor && door->doorRequires[0].required && selected_map_id[0] != '@' && atoi(selected_map_id) == 0)
        {
            // DGNOTE 1/7/2010
            // To handle the shenanigens of places like the Pocket D door to Port Oakes, we have a sequence of required, with a matching sequence of flags
            // and another matching sequence of "locked text" strings.
            int i;
            int j;
            char *translated;
            char *toolong1 = "Warning message \"";
            char *toolong2 = "\" is too long, maximum character count is 1000.  Please shorten it.";
            char *required;
            char *argv[100];
            StringArray expr;
            int argc;

            for (i = 0; i < MAX_DOOR_REQUIRES && door->doorRequires[i].required; i++)
            {
                strdup_alloca(required, door->doorRequires[i].required);
                argc = tokenize_line(required, argv, 0);
                if (argc > 0)
                {
                    eaCreate(&expr);
                    for (j = 0; j < argc; j++)
                    {
                        eaPush(&expr, argv[j]);
                    }

                    if (chareval_Eval(e->pchar, expr, door->filename))
                    {
                        // First test after determing we got a "required hit" is whether they get through or not.  This is the final decision, no other requires will be
                        // evaluated
                        if (door->doorRequires[i].allowThrough)
                        {
                            // Yes, except the locked text if present is used for a yes/no dialog.  They only actually go through if they answer yes to this guy.
                            if (door->doorRequires[i].lockedText)
                            {
                                canEnterDoor = 0;
                                needToAskFirst = 1;
                                translated = localizedPrintf(e, door->doorRequires[i].lockedText);
                                if (strlen(translated) > sizeof(errmsg) - 4)
                                {
                                    strncpyt(errmsg, toolong1, sizeof(errmsg));
                                    strncatt(errmsg, door->doorRequires[i].lockedText);
                                    strncatt(errmsg, toolong2);
                                }
                                else
                                {
                                    strncpyt(errmsg, translated, sizeof(errmsg));
                                }
                            }
                            // If there was no locked text, and they're allowed through, we're all done.
                        }
                        else
                        {
                            // Not allowed through, 
                            translated = localizedPrintf(e, door->doorRequires[i].lockedText ? door->doorRequires[i].lockedText : "DoorLocked");
                            if (strlen(translated) > sizeof(errmsg) - 4)
                            {
                                strncpyt(errmsg, toolong1, sizeof(errmsg));
                                strncatt(errmsg, door->doorRequires[i].lockedText);
                                strncatt(errmsg, toolong2);
                            }
                            else
                            {
                                strncpyt(errmsg, translated, sizeof(errmsg));
                            }
                            canEnterDoor = 0;
                        }
                        break;
                    }
                }
            }
        }
    }

    if (db_state.local_server && !strcmpi(spawnTarget, "MissionReturn") && canEnterDoor)
        exitLocalMission(e);
    else
    {
        if (canEnterDoor || selected_map_id[0] == '@')
        {
            if (selected_map_id[0] == '@')
            {
                selected_map_id++;
            }
            doorTransferToTarget( e, door, spawnTarget, mapTarget, errmsg, pos, selected_map_id, is_volume );
        }
        if (needToAskFirst)
        {
            char cmdstr[256];
            _snprintf(cmdstr, sizeof(cmdstr), "enterdoor %.1f %.1f %.1f @%s", pos[0], pos[1], pos[2], selected_map_id);
            cmdstr[sizeof(cmdstr) - 1] = 0;
            sendDoorMsg( e, DOOR_MSG_ASK, errmsg, cmdstr );
        }
        else if (errmsg[0])
        {
            sendDoorMsg( e, DOOR_MSG_FAIL, errmsg );
        }
        else
        {
            sendDoorMsg( e, DOOR_MSG_OK, 0 );
        }
    }
}

// Somewhat ad hoc door entry.  This allows a player to transfer to an arbitrary location
// This does double duty - it handles the base request, which includes a zone name in zone.
// In that instance baseID and selectedID should both be zero.
// If the map is instanced, this causes the select screen to show, and then does nothing.
// The client will provide the user's choice, which in turn comes through here with
// zone == NULL, but baseID and selectedID holding the base map ID and the map id of the
// selected instance
void enterScriptDoor(Entity *e, const char *zone, const char *spawnTarget, int baseID, int selectedID)
{
    int characterLevel = character_GetSecurityLevelExtern(e->pchar); // +1 since iLevel is zero-based.
    int mapID = 0;
    StaticMapInfo *mapInfo;
    const MapSpec *mapSpec;
    MapSelection *mapSelection;
    int minLevel;
    int maxLevel;
    char selectedMap[256];

    // Pull the mapID from the baseID or zone
    mapID = baseID ? baseID : dbGetMapId(zone);
    mapInfo = staticMapInfoFind(mapID);
    mapSpec = MapSpecFromMapId(mapID);

    // error checking ...
    if (mapID == 0)
    {
        log_printf("doors.log", "enterScriptDoor error: map %s not found in maps.db", zone);
        return;
    }
    if (mapInfo == 0)
    {
        log_printf("doors.log", "enterScriptDoor error: map %s found in maps.db, id %d, but no mapInfo found", zone, mapID);
        return;
    }

    if (mapSpec == 0)
    {
        log_printf("doors.log", "enterScriptDoor error: map %s found in maps.db, id %d, but no mapSpec found", zone, mapID);
        return;
    }

    // Grab the character level range specified by the map.
    minLevel = mapSpec->entryLevelRange.min;
    maxLevel = mapSpec->entryLevelRange.max;

    // And check them.
    if (characterLevel < minLevel)
    {
        sendDoorMsg(e, DOOR_MSG_FAIL, localizedPrintf(e, "TooLowToEnter", minLevel));
        return;
    }
    else if (characterLevel > maxLevel)
    {
        sendDoorMsg(e, DOOR_MSG_FAIL, localizedPrintf(e, "TooHighToEnter", maxLevel));
        return;
    }

    if (selectedID == 0)
    {
        // selectedID is zero - first time through.  Use checkDoorOptions to either get the single instance
        // or present a selection to the user.
        // DGNOTE - I am not sure if this ever worked right with selectedMap as the 5th parameter.  Most notably
        // it didn't hold anything useful, which was a factor (not the only one) in causing winter zone present
        // transfer to secondary instances to fail.
        if ((mapSelection = checkDoorOptions(e, mapID, NULL, NULL, spawnTarget, 1, DOOR_REQUEST_SCRIPT_DOOR, 0)) == NULL)
        {
            return;
        }
        sprintf(selectedMap, "%d.%s", mapSelection->map_id, spawnTarget);
    }
    else
    {
        // Non-zero - they already chose, just set up the string.
        // DGNOTE 11/19/2009 The @ sign says this is a non-negotiable choice of map.  This is
        // scanned for towards the bottom of TeleportToMapSpawnPoint(...) over in
        // character_tick.c
        sprintf(selectedMap, "%d.@%s", selectedID, spawnTarget);
    }

    // start the transfer
    character_NamedTeleport(e->pchar, 0, selectedMap);
    sendDoorMsg(e, DOOR_MSG_OK, NULL);
}

// send entity off mission map and to last map
void leaveMission(Entity *e, int selected_map_id, int no_choice)
{
    char    errmsg[1000] = "";
    Vec3    nowhere = {0};
    StaticMapInfo* mapInfo;
    MapSelection* mapSelect;
    int        mapID = 0;
    char    selected_map[200];
    int        find_best_map = 0;

    if (!OnInstancedMap())
    {
        return;
    }

    if (db_state.local_server)
    {
        exitLocalMission(e);
    }
    else
    {
        if(!no_choice)
        {
            StaticMapInfo* myStaticMapInfo = staticMapInfoFind(e->static_map_id);

            mapInfo = staticMapInfoFind(selected_map_id ? selected_map_id : e->static_map_id);

            if (!mapInfo || myStaticMapInfo && mapInfo->baseMapID != myStaticMapInfo->baseMapID)
            {
                // If we get here, then the player is trying to cheat.

                dbLog(    "cheater",
                    e,
                    "I tried to exit mission to map %d, but my static map is %d (base %d).",
                    selected_map_id,
                    e->static_map_id, 
                    myStaticMapInfo->baseMapID);

                mapInfo = myStaticMapInfo;
                selected_map_id = 0;
            }

            if(mapInfo)
            {
                sprintf(selected_map, "%i", selected_map_id);
                mapSelect = checkDoorOptions(e, mapInfo->baseMapID, NULL, NULL, selected_map, 1, DOOR_REQUEST_EXIT_MISSION, 0);
                mapID = mapSelect->map_id;
            }
        }
        else
        {
            mapID = selected_map_id;
        }
         
        // NOTE! If mapID is not valid, it is NOT something horrible.
        // If mapID is 0, then we just popped up a map choice dialog to the user
        // DO NOT port them to mercy or atlas or whatever.

        {
            MapHistory *hist = mapHistoryLast(e->pl);
            if (hist && hist->mapType == MAPTYPE_BASE)
            { //if we just came from a base, go back there.
                // ARM Disabled this functionality to make Arenas from Anywhere play nicely with bases
                //DoorAnimEnter(e, NULL, nowhere, 0, SGBaseConstructInfoString(hist->mapID), "MissionReturn", 0, errmsg);
                //mapID = 0; //return; // don't return early, need to clear arena stuff at bottom
            }
            else if (hist && hist->mapType == MAPTYPE_MISSION)
            { //If we came from a mission and we're on a mission, go back
                // NOTE: figure out how to do this, for now go back to static map
                
            }
        }

        if(mapID)
        {
            if (DoorAnimEnter(e, NULL, nowhere, mapID, NULL, "MissionReturn", XFER_STATIC | XFER_FINDBESTMAP, errmsg))
                sendDoorMsg(e, DOOR_MSG_OK, 0);
            else 
                sendDoorMsg(e, DOOR_MSG_FAIL, errmsg);
        }
    }

    if (OnArenaMap())
    {
        ArenaRemoveTempPower(e, "PvP_SuddenDeath");
        team_MemberQuit(e);
    }
}

// Kick the character out via the parameters given on the specified door.
// If something goes wrong, punt and do a normal mission return.
void leaveMissionViaAlternateDoor(Entity *e, DoorEntry *door)
{
    int mapID;
    char errmsg[1000] = "";
    Vec3 nowhere = {0};
    StaticMapInfo* mapInfo;
    MapSelection* mapSelect;
    int find_best_map = 0;

    mapID = door->name && door->special_info ? dbGetMapId(door->special_info) : -1;
    mapInfo = mapID >= 0 ? staticMapInfoFind(mapID) : NULL;

    if (mapID < 0 || mapInfo == NULL)
    {
        // If this happens, it means design didn't set up the door correctly.
        // Log the fact, and kick the player out to the original location
        log_printf(NULL, "ERROR: alternate exit door on maps %s at %f,%f,%f has bad GotoMap <%s> or bad GotoSpawn <%s>", db_state.map_name, door->mat[3][0], door->mat[3][1], door->mat[3][2], door->special_info ? door->special_info : "(null)", door->name ? door->name : "(null)");
        if (isDevelopmentMode())
        {
            ErrorFilenamef(db_state.map_name, "ERROR: alternate exit door at %f,%f,%f has bad GotoMap <%s> or bad GotoSpawn <%s>", door->mat[3][0], door->mat[3][1], door->mat[3][2], door->special_info ? door->special_info : "(null)", door->name ? door->name : "(null)");
        }
        leaveMission(e, 0, 0);
        return;
    }

    if (!OnInstancedMap())
    {
        return;
    }

    if (db_state.local_server)
    {
        exitLocalMission(e);
        return;
    }

    mapSelect = checkDoorOptions(e, mapInfo->baseMapID, NULL, door, "0", 1, DOOR_REQUEST_EXIT_MISSION_AND_MAP_MOVE, 0);
    mapID = mapSelect->map_id;
         
    if(mapID)
    {
        if (DoorAnimEnter(e, NULL, nowhere, mapID, NULL, door->name, XFER_STATIC, errmsg))
            sendDoorMsg(e, DOOR_MSG_OK, 0);
        else 
            sendDoorMsg(e, DOOR_MSG_FAIL, errmsg);
    }
}    

int dbBeginGurneyXfer(Entity *e, bool bBase)
{
    char errmsg[1000] = "";
    Vec3 zero = {0};

    bool bDead = entMode(e, ENT_DEAD);

    if (!bDead) 
    {
        // Tried to transfer when they weren't actually dead (either a cheater or a resurrect power just healed them)
        strcpy(errmsg, localizedPrintf(e, "playerNotDead" ));
        sendChatMsg( e, errmsg, INFO_USER_ERROR, 0 );
        return FALSE;
    }

    // auto-resurrect in place if this is set
    if (server_visible_state.disablegurneys)
    {
        // tell the client to display I'm Dead dialog
        START_PACKET( pak1, e, SERVER_DEAD_NOGURNEY );
        END_PACKET

        // make me invincible until I respond to I'm Dead
        e->invincible |= UNTARGETABLE_TRANSFERING;

        return TRUE; // don't reset health yet
    }
    {
        int gurney_map_id;
        char * spawnTargetGurneyType = NULL;
        char * mapInfo = NULL;
        int flags = XFER_GURNEY | XFER_FINDBESTMAP;

        //I don't think gurney map is even Null, but just in case
        if (!e->gurney_map_id)
            e->gurney_map_id = 1; // MapID City_01_01 
        if (!e->villain_gurney_map_id)
            e->villain_gurney_map_id = 71; //MapID V_City 01_01 


        //added for custom mission gurneys like prisons
        if( e->mission_gurney_map_id )
        {
            gurney_map_id = e->mission_gurney_map_id;
            if(e->mission_gurney_target) 
                spawnTargetGurneyType = e->mission_gurney_target;
            else
                spawnTargetGurneyType = "Gurney";
        }
        else if (bBase)
        { //Player wants to spawn in their base
            Supergroup *sg = e->supergroup;
            int i;
            if (sg)
            {
                for (i = eaSize(&sg->specialDetails) - 1; i >= 0; i--)
                {
                    if (!sg->specialDetails[i] || !sg->specialDetails[i]->pDetail)
                        continue;
                    if (sg->specialDetails[i]->iFlags & DETAIL_FLAGS_IN_BASE && 
                        sg->specialDetails[i]->iFlags & DETAIL_FLAGS_ACTIVATED && 
                        sg->specialDetails[i]->pDetail->eFunction == kDetailFunction_Medivac)
                    {
                        StaticMapInfo *oldstatic = staticMapInfoFind(e->static_map_id);
                        const MapSpec* mapSpec = oldstatic?MapSpecFromMapName(oldstatic->name):NULL;
                        if (!oldstatic || oldstatic->introZone || !mapSpec || mapSpec->disableBaseHospital || !MissionAllowBaseHospital())
                        {
                            // Don't let players teleport out of tutorials
                            break;
                        }
                        flags = XFER_GURNEY | XFER_FINDBESTMAP | XFER_BASE;
                        spawnTargetGurneyType = "Gurney";
                        gurney_map_id = 0;
                        mapInfo = SGBaseConstructInfoString(e->supergroup_id);
                    }
                }
            }
            if (!(flags & XFER_BASE))
            { //We didn't find a gurney in their base
                strcpy(errmsg, localizedPrintf(e, "noBaseGurney" ));
                sendChatMsg( e, errmsg, INFO_USER_ERROR, 0 );
                return FALSE;
            }
        }
        else
        {
            if (ENT_IS_ON_RED_SIDE(e))
            {
                gurney_map_id = e->villain_gurney_map_id;
                spawnTargetGurneyType = "VillainGurney";
            }
            else
            {
                gurney_map_id = e->gurney_map_id;
                spawnTargetGurneyType = "Gurney";
            }

             if( OnPlayerCreatedMissionMap() )
            {
                spawnTargetGurneyType = "ArchitectGurney";
            }
        }

        if (!DoorAnimEnter(e, NULL, zero, gurney_map_id, mapInfo, spawnTargetGurneyType, flags, errmsg))
        {
            completeGurneyXferWork(e);
            if (errmsg[0])
                sendDoorMsg( e, DOOR_MSG_FAIL, errmsg );
        }
    }
    return TRUE;
}

void StartMission(ClientLink * client, char * missionName, char * mapFile)
{
    char errmsg[2000];
    Entity * e = client->entity;

    if (OnInstancedMap())
    {
        conPrintf(client,localizedPrintf(client->entity,"CantCommandInMission"));
        return;
    }

    if(TaskForceGetUnknownContact(client, missionName))
    {
        StoryTaskInfo * pSTI = TaskGetMissionTask(e, 0);

        if(!pSTI)
        {
            conPrintf(client, localizedPrintf(client->entity,"CantGetTask"));
            return;
        }

        if (pSTI->def->type != TASK_MISSION)
        {
            conPrintf(client, localizedPrintf(client->entity,"TaskNotMission"));
            return;
        }

        if(!strcmp(mapFile, "*"))
        {
            conPrintf(client, localizedPrintf(client->entity,"UsingDefaultMap"));
            mapFile = 0;
        }
        else
        {
            char * fullName;
            int valid;

            const MissionDef* mission = TaskMissionDefinition(&pSTI->sahandle);
            if(!mission)
            {
                conPrintf(client, localizedPrintf(client->entity,"CantGetMissionTask"));
                return;
            }

            // see if map meets mission requirements, and also allow partial name to be used
            valid = DebugMissionMapMeetsRequirements(mapFile, mission->mapset, mission->maplength, &fullName);
            
            if(fullName)
                mapFile = fullName;    // replace partial map name with full name
            else if(!fileExists(mapFile))
            {
                conPrintf(client, localizedPrintf(client->entity,"MapfileGone"));
                return;
            }

            if(!valid)
                conPrintf(client, localizedPrintf(client->entity,"MapDoesntMeetReq"));
        }

        if (db_state.local_server)
        {
            enterLocalMission(e, pSTI->doorId, 0, mapFile, NULL);
            return;
        }


        if (lockTeamupForDoor(e))
        {
            if (attachTeamupToMission(e, pSTI->doorId, 0, tempStoryTaskHandle(0, 0, 0, 0), errmsg))
            {
                Vec3 dummy;
                if (enterMissionDoor(e, NULL, dummy, mapFile, errmsg))
                {
                    // success
                }
            }
            else
            {
                conPrintf(client, "%s", errmsg);
            }
            teamUpdateUnlock(e, CONTAINER_TEAMUPS);
        }
        else
        {
            log_printf("missiondoor.log", "StartMission: I wasn't able to lock the teamup??");
            conPrintf(client, localizedPrintf(client->entity,"DoorLocked")); // something's terribly wrong
        }
    }
}

void StartStoryArcMission(ClientLink* client, char* logicalname, char* episodename, char* mapname)
{
    char errmsg[2000];
    Entity* player = client->entity;

    if (OnInstancedMap())
    {
        conPrintf(client,localizedPrintf(client->entity,"CantCommandInMission"));
        return;
    }

    if (StoryArcForceGetTask(client, logicalname, episodename))
    {
        StoryInfo* info = StoryInfoFromPlayer(player);
        StoryTaskInfo* task = info->tasks[0];

        if (!strcmp(mapname, "*"))
        {
            const MissionDef* mission = TaskMissionDefinition(&task->sahandle);
            if (!mission)
            {
                conPrintf(client, localizedPrintf(client->entity,"CantGetMissionTask"));
                return;
            }
            conPrintf(client, localizedPrintf(player, "UsingDefaultMap"));
            mapname = NULL;
        }
        else
        {
            char* fullName;
            int valid;

            const MissionDef* mission = TaskMissionDefinition(&task->sahandle);
            if (!mission)
            {
                conPrintf(client, localizedPrintf(client->entity,"CantGetMissionTask"));
                return;
            }

            // see if map meets mission requirements, and also allow partial name to be used
            valid = DebugMissionMapMeetsRequirements(mapname, mission->mapset, mission->maplength, &fullName);

            if (fullName)
                mapname = fullName;    // replace partial map name with full name
            else if (!fileExists(mapname))
            {
                conPrintf(client, localizedPrintf(player, "MapfileGone"));
                return;
            }

            if (!valid)
                conPrintf(client, localizedPrintf(client->entity,"MapDoesntMeetReq"));
        }

        if (db_state.local_server)
        {
            enterLocalMission(player, task->doorId, 0, mapname, NULL);
            return;
        }

        if (lockTeamupForDoor(player))
        {
            if (attachTeamupToMission(player, task->doorId, 0, tempStoryTaskHandle(0, 0, 0, 0), errmsg))
            {
                Vec3 dummy;
                if (enterMissionDoor(player, NULL, dummy, mapname, errmsg))
                {
                    // success
                }
            }
            else
            {
                conPrintf(client, "%s", errmsg);
            }
            teamUpdateUnlock(player, CONTAINER_TEAMUPS);
        }
        else
        {
            log_printf("missiondoor.log", "StartStoryArcMission: I wasn't able to lock the teamup??");
            conPrintf(client, localizedPrintf(client->entity,"DoorLocked")); // something's terribly wrong
        }
    }
}

///////////////////////////////////////////////////////////////
//  EnterMission
int EnterMission(Entity *pPlayer, StoryTaskInfo *pTask)
{
    ClientLink* client = pPlayer->client;
    char errmsg[2000];

    if (OnInstancedMap())
    {
        log_printf("missiondoor.log", "EnterMision: Already on mission map");
        return false;
    }

    if (db_state.local_server)
    {
        enterLocalMission(pPlayer, 0, pTask->assignedDbId, 0, NULL);
        return true;
    }

    if (lockTeamupForDoor(pPlayer))
    {
        if (attachTeamupToMission(pPlayer, 0, pTask->assignedDbId, &pTask->sahandle, errmsg))
        {
            Vec3 dummy;
            if (enterMissionDoor(pPlayer, NULL, dummy, 0, errmsg))
            {
                // success
                teamUpdateUnlock(pPlayer, CONTAINER_TEAMUPS);
                return true;
            }
        }
        else
        {
            conPrintf(client, "%s", errmsg);
        }
        teamUpdateUnlock(pPlayer, CONTAINER_TEAMUPS);
    }
    else
    {
        log_printf("missiondoor.log", "StartMission: I wasn't able to lock the teamup??");
        conPrintf(client, localizedPrintf(pPlayer, "DoorLocked")); // something's terribly wrong
    }
    return false;
}


#define MAX_ARENA_DOOR_DIST        50

// should be triggered automatically when I'm in the right place, right time
int EnterArena(Entity* e, ArenaEvent* event)
{
    ArenaSeating* seat = NULL;
    ArenaParticipant* part = NULL;
    //DoorEntry* door;
    ClientLink* client = e->client;
    char buf[20];
    int i, n, s;

    if (db_state.local_server)
    {
        conPrintf(client, localizedPrintf(e,"CantEnterLocal"));
        return 0;
    }

    // figure out which map to transfer to
    n = eaSize(&event->participants);
    for (i = 0; i < n; i++)
    {
        if (event->participants[i]->dbid == e->db_id)
        {
            part = event->participants[i];
            break;
        }
    }
    if (!part) // assume they want into the first seat
        s = 1;
    else
        s = part->seats[event->round];

    if(!s)
        return 0;

    seat = event->seating[s];
    if (!seat || !seat->mapid)
    {
        // ArenaServerError is not defined in any message store!!!
        // conPrintf(client, localizedPrintf(e,"ArenaServerError"));

        // "Map failed to start.<br>The server is busy, please try again later."
        conPrintf(client, localizedPrintf(e,"MapFailedOverloadProtection"));
        return 0;
    }

    // just find the nearest doorway..
    //door = dbFindLocalMapObj(e->mat[3],0,MAX_ARENA_DOOR_DIST); // ok if door is NULL //MW rm, no door needed
    if(part)
        sprintf(buf, "ArenaStart%i", part->side);
    else
        strcpy(buf, "ArenaObserver");

    DoorAnimEnter(e, 0, ENTPOS(e), seat->mapid, NULL, buf, XFER_ARENA, NULL);

    return 1;
}

// defines for functions that return MapSelection*
#define RETURNMAP(x,spawn) { result.mowner = result.sahandle.context = result.sahandle.subhandle = result.sahandle.compoundPos = 0; result.map_id = x; strncpyt(result.spawn_name,spawn,31); return &result; }

MapSelection* AddZoneTransferMission(Entity* e, int owner, char* taskforce, StoryTaskInfo* mission, ContactHandle contact, MapSelection* sel, char** cur, char* commandline, int* displayCount, char* monorail)
{
    static MapSelection result;
    const char* locationName = "Neighborhood";
    char* ownerName;
    const MissionDef* def;
    char* rowStyle = "<tr>";

    if (monorail)
        rowStyle = "<tr style=gradient>";

    (*displayCount)++;

    if (owner == sel->mowner && isSameStoryTask( &mission->sahandle, &sel->sahandle ) )
    {
        result.mowner = sel->mowner;
        StoryHandleCopy( &result.sahandle, &sel->sahandle );
        result.map_id = 0; 
        sprintf(result.spawn_name,""); 
        return &result; 
    }

    def = TaskMissionDefinition(&mission->sahandle);
    if (def && def->locationname)
    {    
        ScriptVarsTable vars = {0};
        // Potential mismatch
        saBuildScriptVarsTable(&vars, e, owner, contact, mission, NULL, NULL, NULL, NULL, 0, 0, false);
        locationName = saUtilTableLocalize(def->locationname, &vars);
    }

    if (taskforce)
        ownerName = taskforce;
    else
        ownerName = dbPlayerNameFromId(owner);

    // We just incremented it, so this must be the first item.
    if(*displayCount == 1)
    {
        *cur += sprintf(*cur, "</table><span align=center valign=center>%s</span><table>",
            localizedPrintf(e, "ZoneTransferHeader"));
    }

    *cur += sprintf(*cur,
        "<a href='cmd:%s %s'>%s<td align=center>%s - %s</td></tr></a>",
        commandline,
        MissionConstructIdString(owner, &mission->sahandle),
        rowStyle,
        locationName,
        ownerName);

    return NULL;
}

// add any mission options to the zone transfer info list
MapSelection* AddZoneTransferMissions(Entity* e, char *monorail, MapSelection* sel, char** cur, char* commandline, int* displayCount)
{
    int i, j;
    StoryTaskInfo* mission;
    ContactHandle contact;
    MapSelection* result = NULL;
    MonorailLine *line = MonorailGetLine(monorail);

    if (!monorail || !line) return NULL; // MAK - you can only get to zone transfer missions from the monorail right now

    if (!e->teamup) 
    {
        // all my missions
        for (j = 0; mission = TaskGetMissionTask(e, j); j++)
        {
            if (!TaskIsZoneTransfer(mission)) continue;
            if (!TASK_INPROGRESS(mission->state)) continue;
            if (StringArrayFind(line->allowedZoneTransferType, TaskMissionDefinition(&mission->sahandle)->doortype) == -1) continue;
            contact = EntityTaskGetContactHandle(e, mission);
            result = AddZoneTransferMission(e, e->db_id, e->taskforce? e->taskforce->name: 0, mission, contact, sel, cur, commandline, displayCount, monorail);
            if (result)
                return result;
        }
    }
    else if (TeamCanSelectMission(e))
    {
        // everyone on this server
        for (i = 0; i < e->teamup->members.count; i++)
        {
            Entity* teammate = entFromDbId(e->teamup->members.ids[i]);
            
            if (!teammate) continue;
            for (j = 0; mission = TaskGetMissionTask(teammate, j); j++)
            {
                if (!TaskIsZoneTransfer(mission)) continue;
                if (!TASK_INPROGRESS(mission->state)) continue;
                if (StringArrayFind(line->allowedZoneTransferType, TaskMissionDefinition(&mission->sahandle)->doortype) == -1) continue;
                contact = EntityTaskGetContactHandle(teammate, mission);
                result = AddZoneTransferMission(e, teammate->db_id, e->taskforce? e->taskforce->name: 0, mission, contact, sel, cur, commandline, displayCount, monorail);
                if (result) return result;
            }
        }
    }
    else
    {
        // only team selected task
        if (e->teamup->activetask->sahandle.context &&
            TaskIsZoneTransfer(e->teamup->activetask) &&
            TASK_INPROGRESS(e->teamup->activetask->state) &&
            (StringArrayFind(line->allowedZoneTransferType, TaskMissionDefinition(&e->teamup->activetask->sahandle)->doortype) != -1))
            result = AddZoneTransferMission(e, e->teamup->activetask->assignedDbId, e->taskforce? e->taskforce->name: 0, e->teamup->activetask, e->teamup->mission_contact, sel, cur, commandline, displayCount, monorail);
        if (result) return result;
    }

    // We only need the separator for missions and static maps if we added a
    // mission to the list.
    if (*displayCount > 0)
    {
        *cur += sprintf(*cur, "</table><span align=center>%s</span><table>",
            localizedPrintf(e, "ZoneTransferFooter"));
    }

    return NULL;
}
#define MAXSPAWNSPERMAP 3

// You can now specify spawn names in the aux items for base teleporters.
typedef struct monorailDest {
    int state;
    StaticMapInfo *info;
    int mission;
    char spawn_name[MAXSPAWNSPERMAP*32]; //support up to around 8 spawns per map
    MonorailStop *monorail_stop;
} monorailDest;

typedef struct baseMapContext {
    StaticMapInfo *info;
    MonorailStop *stop;
} baseMapContext;

MapSelection* checkDoorOptions(Entity* e, int dest_map_id, Vec3 pos, DoorEntry* door, const char* selected_map, int forceGood, DoorRequestType requestType,int detailID)
{
    extern StaticMapInfo** staticMapInfos;
    static MapSelection result;

    int i;
    static monorailDest dests[1000];
    static baseMapContext baseInfos[1000];
    const char **mapNames = NULL;
    char *monorailLine = NULL;
    char *monorailStop = NULL;
    StaticMapInfo* info = NULL;
    char *monorailLogo = NULL;
    int mapCount = eaSize(&staticMapInfos);
    int goodCount = 0;
    int baseCount = 0;
    int foundEmpty = 0;
    int foundGood = 0;
    int foundAlmostGood = 0;
    MapSelection sel;
    int *missionMaps = NULL;
    char *rowStyle = "<tr>";

    if (door && door->monorail_line)
    {
        char *period;

        strdup_alloca(monorailLine, door->monorail_line);
        period = strchr(monorailLine, '.');

        if (period)
        {
            *period = '\0';
            strdup_alloca(monorailStop, period + 1);
        }
    }

    // selected_map may indicate a mission or a normal map_id
    sel.map_id = 0;
    if (selected_map && !MissionDeconstructIdString(selected_map, &sel.mowner, &sel.sahandle))
    {
        char *period;
        memset(&sel, 0, sizeof(sel));

        period = strchr(selected_map,'.'); //if we have a ., then we have a specified spawnlocation
        if (period)
            *period = '\0';
        sel.map_id = atoi(selected_map);
        if (period)
        {
            strncpyt(sel.spawn_name, period+1,31);
            *period = '.';
        }
    }

    // ----------------------------------------
    // gather applicable mapservers

    if(monorailLine || detailID)
    {
        int characterLevel = character_GetSecurityLevelExtern(e->pchar);

        if (monorailLine)
        {
            MonorailLine* line = MonorailGetLine(monorailLine);
            int j;

            missionMaps = PlayerGetTasksMapIDs(e);

            if(line && line->stops)
            {
                for(i = 0; i < eaSize(&line->stops); i++)
                {
                    for(j = 0; j < eaSize(&line->stops[i]->mapInfos); j++)
                    {
                        info = line->stops[i]->mapInfos[j];

                        if(info->baseMapID == info->mapID)
                        {
                            baseInfos[baseCount].info = info;
                            baseInfos[baseCount].stop = line->stops[i];
                            baseCount++;
                        }

                        dests[goodCount].info = info;
                        dests[goodCount].state = MAP_OPEN_LOW;
                        strncpy_s(dests[goodCount].spawn_name, sizeof(dests[goodCount].spawn_name) - 1, line->stops[i]->spawnName, strlen(line->stops[i]->spawnName));
                        dests[goodCount].monorail_stop = line->stops[i];
                        dests[goodCount].mission = 0;

                        if(missionMaps && info->baseMapID)
                        {
                            if(eaiSortedFind(&missionMaps, info->baseMapID) != -1)
                            {
                                dests[goodCount].mission = 1;
                            }
                        }

                        goodCount++;
                    }
                }
            }
        }
        else
        {            
            mapNames = baseGetMapDestinations(detailID);

            for(i = 0; i < mapCount; i++)
            {
                const MapSpec* spec;
                int j, found = 0, mapNameCount = eaSize(&mapNames);
                char temp[100];
                // Check to see if this is a detailID and we have the right auxitem:
                
                info = staticMapInfos[i];
                spec = MapSpecFromMapId(info->mapID);

                sprintf(dests[goodCount].spawn_name,"");
                dests[goodCount].monorail_stop = NULL;

                for (j = 0; j < mapNameCount; j++) {
                    char *period; //if there's a period, we need to save the spawnlocation
                    sprintf(temp,"/%s",mapNames[j]);
                    period = strchr(temp,'.');
                    if (period)
                        *period = '\0';
                    if (strstr(info->name,temp))
                    {
                        found++;                        
                        if (period && found <= MAXSPAWNSPERMAP)
                        { //only try to add if spawn is specified. if any specifed, all must be
                            char *start = dests[goodCount].spawn_name;
                            if (dests[goodCount].spawn_name[0])
                            { //we've already got a list going, add to it
                                start = strchr(dests[goodCount].spawn_name,'\0');
                                *start = ' ';
                                start++;
                            }
                            strncpyt(start, period+1,31);                        
                        } 
                    }
                    if (period)
                        *period = '.';
                }
                if (!found)
                    continue;

                // Add to the base map list.

                if(info->baseMapID == info->mapID)
                {
                    baseInfos[baseCount].info = info;
                    baseInfos[baseCount].stop = NULL;
                    baseCount++;
                }

                dests[goodCount].info = info;


                goodCount++;
            }
        }
        // We've now marked all the destinations with missions in them.
        eaiDestroy(&missionMaps);

        for(i = 0; i < goodCount; i++)
        {
            info = dests[i].info;

            if(    info->mapID == db_state.map_id ||
                !info->safePlayersLow ||
                info->playerCount && info->playerCount < info->safePlayersLow)
            {
                dests[i].state = MAP_OPEN_LOW;
            }
            else if(!info->safePlayersHigh ||
                info->playerCount && info->playerCount < info->safePlayersHigh)
            {
                if(sel.map_id == info->mapID && strncmp(sel.spawn_name,dests[i].spawn_name,31) == 0)
                    RETURNMAP(info->mapID,dests[i].spawn_name);

                dests[i].state = MAP_OPEN_HIGH;
            }
            else if(!info->playerCount)
            {
                dests[i].state = MAP_EMPTY;
            }
            else
            {
                dests[i].state = MAP_FULL;
            }
        }

        // Make sure there's at least one "good" map from each base ID.

        for(i = 0; i < baseCount; i++)
        {
            int j;
            int foundGood = 0;
            int foundEmpty = 0;
            int foundBase = 0;

            // Gather stats about this base ID.

            for(j = 0; j < goodCount; j++)
            {
                // Monorails can have multiple stops in the same map so we
                // have to match stops as well as just map IDs
                if(dests[j].info->baseMapID == baseInfos[i].info->mapID &&
                    (!baseInfos[i].stop || dests[j].monorail_stop == baseInfos[i].stop))
                {
                    switch(dests[j].state)
                    {
                    case MAP_OPEN_LOW:
                        foundGood = 1;
                        break;

                    case MAP_EMPTY:
                        if(!foundEmpty)
                        {
                            foundEmpty = 1;
                        }
                        else
                        {
                            dests[j].state = MAP_IGNORED;
                        }

                        break;
                    }
                }
            }

            // Update states based on gathered stats.

            for(j = 0; j < goodCount; j++)
            {
                if(dests[j].info->baseMapID == baseInfos[i].info->mapID &&
                    (!baseInfos[i].stop || dests[j].monorail_stop == baseInfos[i].stop))
                {
                    switch(dests[j].state)
                    {
                    case MAP_OPEN_HIGH:
                        if(foundGood || foundEmpty)
                        {
                            dests[j].state = MAP_FULL;
                        }
                        else
                        {
                            dests[j].state = MAP_OPEN_LOW;
                        }
                        break;

                    case MAP_EMPTY:
                        if(foundGood)
                        {
                            dests[j].state = MAP_IGNORED;
                        }
                        else
                        {
                            if(sel.map_id == dests[j].info->mapID && strncmp(sel.spawn_name,dests[j].spawn_name,31) == 0)
                                RETURNMAP(dests[j].info->mapID,dests[j].spawn_name);

                            dests[j].state = MAP_OPEN_LOW;
                        }
                        break;
                    }
                }
            }
        }
        for(i = 0; i < goodCount; i++)
        {
            StaticMapInfo* info = dests[i].info;
            const MapSpec* spec = MapSpecFromMapId(info->mapID);
            
            if (strchr(dests[i].spawn_name,' '))
            { //we need to add new entries for the other spawns
                char sepstring[32*MAXSPAWNSPERMAP];
                char *cur;
                strncpyt(sepstring,dests[i].spawn_name,32*MAXSPAWNSPERMAP-1);
                cur = strtok(sepstring," ");
                strncpyt(dests[i].spawn_name,cur,31);
                while (cur = strtok(NULL," ")) //add new entries to the end for additional spawn points
                {
                    dests[goodCount].info = dests[i].info;
                    dests[goodCount].state = dests[i].state;
                    strncpyt(dests[goodCount].spawn_name,cur,31);
                    goodCount++;
                }
            }

            if (dests[i].state != MAP_IGNORED) 
            {
                if (spec) 
                { 
                    if (characterLevel < spec->entryLevelRange.min) 
                    {
                        dests[i].state = MAP_TOO_HIGH;
                    }
                    else if (eaSize(&spec->monorailRequires))
                    {
                        
                        if (e && e->pchar && chareval_Eval(e->pchar, spec->monorailRequires, "specs/maps.spec"))
                        {
                            //    requirements satisfied. Let it through silently
                        }
                        else    
                        {
                            dests[i].state = MAP_FAILED_REQUIRES;
                        }
                    }
                }
            }
        }
        if (detailID && mapNames)
        {
            eaDestroyConst(&mapNames);
        }
    }
    else
    {
        for(i = 0; i < mapCount; i++)
        {
            StaticMapInfo* info = staticMapInfos[i];
            const MapSpec* mapSpec;

            if(dest_map_id != info->baseMapID)
                continue;

            dests[goodCount].info = info;
            sprintf(dests[goodCount].spawn_name,"");
            dests[goodCount].monorail_stop = NULL;

            mapSpec = MapSpecFromMapId(info->mapID);

            if (((ENT_IS_HERO(e) && (!ENT_IS_ROGUE(e) || !MAP_ALLOWS_VILLAINS_ONLY(mapSpec))) 
                    || (ENT_IS_ROGUE(e) && MAP_ALLOWS_HEROES_ONLY(mapSpec)))
                && info->maxHeroCount && info->heroCount >= info->maxHeroCount)
            {
                dests[goodCount].state = MAP_FULL;
            }
            else if(((ENT_IS_VILLAIN(e) && (!ENT_IS_ROGUE(e) || !MAP_ALLOWS_HEROES_ONLY(mapSpec)))
                    || (ENT_IS_ROGUE(e) && MAP_ALLOWS_VILLAINS_ONLY(mapSpec))) 
                && info->maxVillainCount && info->villainCount >= info->maxVillainCount)
            {
                dests[goodCount].state = MAP_FULL;
            }
            else if(!info->safePlayersLow ||
                        info->playerCount && info->playerCount < info->safePlayersLow)
            {
                foundGood = 1;
                dests[goodCount].state = MAP_OPEN_LOW;
            }
            else if(!info->safePlayersHigh ||
                info->playerCount && info->playerCount < info->safePlayersHigh)
            {
                foundAlmostGood = 1;
                dests[goodCount].state = MAP_OPEN_HIGH;
            }
            else if(!foundEmpty && !info->playerCount)
            {
                foundEmpty = 1;
                dests[goodCount].state = MAP_EMPTY;
            }
            else if(info->playerCount)
            {
                dests[goodCount].state = MAP_FULL;
            }
            else
            {
                dests[goodCount].state = MAP_IGNORED;
            }

            // Script doors don't have an associated Detail ID to provide the target spawn name.
            // Because of that we wind up here rather than the code higher up that fills in
            // spawn_name, so we manually copy the spawn_name over here.
            // 
            if (requestType == DOOR_REQUEST_SCRIPT_DOOR)
            {
                strncpyt(dests[goodCount].spawn_name, sel.spawn_name, 31);                        
            }
            goodCount++;
        }
    }

    // ----------------------------------------
    // write the formatting

    if(goodCount)
    {
        char buffer[20000];
        char* cur = buffer;
        int firstGood = -1;
        int displayCount = 0;
        char commandString[200];

        // ----------
        // the header 

        if (monorailLine)
            cur += sprintf(cur, "%s", monorailSelectFormatTitleHeader);
        else
            cur += sprintf(cur, "%s", mapSelectFormatTop1);

        // ----------
        // title menumessage 

        if (monorailLine)
        {
            const MapSpec* spec = MapSpecFromMapId(db_state.base_map_id);
            MonorailLine *line = MonorailGetLine(monorailLine);

            if (line && line->logo && line->logo[0])
            {
                monorailLogo = line->logo;
            }

            if (line->title && line->title[0])
            {
                strcat(cur, localizedPrintf(e, line->title));
                cur += strlen(localizedPrintf(e, line->title));
            }

            // Monorail uses the other background style.
            rowStyle = "<tr style=gradient>";
        }
        else if (detailID)
        {            
            strcat( cur, localizedPrintf(e,(char*)teleFormatTop2) );
            cur += strlen(localizedPrintf(e,(char*)teleFormatTop2) );
        }
        else
        {
            strcat( cur, localizedPrintf(e,(char*)plainFormatTop2) );
            cur += strlen(localizedPrintf(e,(char*)plainFormatTop2) );
        }

        // ----------
        // format

        if (monorailLine)
            cur += sprintf(cur, "%s", monorailSelectFormatTitleBodyDivider);
        else
            cur += sprintf(cur, "%s", mapSelectFormatTop3);

        // ----------
        // build the command string for each button

        switch(requestType)
        {
        case DOOR_REQUEST_CLICK:
            assert(pos);
            sprintf(commandString, "enterdoor %f %f %f", vecParamsXYZ(pos));
            break;

        case DOOR_REQUEST_VOLUME:
            strcpy(commandString, "enterdoorvolume");
            break;

        case DOOR_REQUEST_EXIT_MISSION:                
            strcpy(commandString, "requestexitmission");
            break;

        case DOOR_REQUEST_MAP_MOVE:
            // Monorails use the map and spawn from the individual stops.
            if(monorailLine)
                sprintf(commandString, "enterdoorteleport");
            else
                sprintf(commandString, "enterdoorteleport %s", selected_map);
            break;

        case DOOR_REQUEST_EXIT_MISSION_AND_MAP_MOVE:
            sprintf(commandString, "requestexitmissionalt %s", door->name);
            break;

        case DOOR_REQUEST_SCRIPT_DOOR:
            // We hit here if we come in from enterScriptDoor(...).  In that case, selected_map has been
            // hijacked to hold the spawn location, which ultimately becomes the second parameter to
            // the enterscriptdoor command over in cmdserver.c
            sprintf(commandString, "enterscriptdoor %d %s", dest_map_id, selected_map);
            break;

        default:
            assert(0);
        }

        // ----------
        // build the buttons
        // MAK - mission options are just inserted right now, so we don't have to mess with map info list
        if (requestType != DOOR_REQUEST_EXIT_MISSION && requestType != DOOR_REQUEST_MAP_MOVE && 
            requestType != DOOR_REQUEST_EXIT_MISSION_AND_MAP_MOVE && !OnMissionMap())
        {
            MapSelection* choice = AddZoneTransferMissions(e, monorailLine, &sel, &cur, commandString, &displayCount);
            if (choice) return choice;
        }

        for(i = 0; i < goodCount; i++)
        {
            StaticMapInfo* info = dests[i].info;
            char missionMarker[500] = { 0 };
            char fullMapName[500];
            char fullCommand[500];
            char* mapName;
            char * lwcColorTextOpen = "";
            char * lwcColorTextClose = "";

            if (!LWC_CheckMapIDReady(info->mapID, LWC_GetCurrentStage(e), 0))
            {
                lwcColorTextOpen = "<font color=#ff0000>";
                lwcColorTextClose = "</font>";
            }

            if (dests[i].mission)
            {
                sprintf(missionMarker, "<img src=\"Monorail_UI_MissionPin\">");
            }

            // ----------
            // if this isn't the first instance of Atlas Park, say, mangle the name with the id

            if(info->instanceID > 1)
            {
                sprintf(fullMapName, "%s_%d", info->name, info->instanceID);

                mapName = localizedPrintf(0,fullMapName);
            }
            else
            {
                mapName = localizedPrintf(0,info->name);
            }

            // ----------
            // append a qualifier to the map name if necessary

            if (dests[i].monorail_stop)
            {
                if (dests[i].monorail_stop->desc)
                {
                    sprintf(fullMapName, "%s %s", mapName, localizedPrintf(0, dests[i].monorail_stop->desc));
                }
                else
                {
                    strncpy_s(fullMapName, sizeof(fullMapName) - 1, mapName, _TRUNCATE);
                }
            }
            else if (dests[i].spawn_name[0])
            {
                sprintf(fullMapName, "%s %s", mapName, localizedPrintf(0, dests[i].spawn_name));
            }
            else
            {
                strncpy_s(fullMapName, sizeof(fullMapName) - 1, mapName, _TRUNCATE);
            }

            // ----------
            // create the command (if there is a spawn location, add it)

            if (requestType == DOOR_REQUEST_MAP_MOVE && dests[i].monorail_stop)
            {
                sprintf(fullCommand,"%s %s %d",commandString,dests[i].monorail_stop->spawnName,info->mapID);
            }
            else if (dests[i].spawn_name[0])
            {
                sprintf(fullCommand,"%s %d.%s",commandString,info->mapID,dests[i].spawn_name);
            }
            else
            {
                sprintf(fullCommand,"%s %d",commandString,info->mapID);
            }

            // Treat different monorail stops on the same map as separate places
            // as long as the door told us which stop it is.
            if(info->mapID == db_state.map_id && (!monorailStop || !dests[i].monorail_stop || (stricmp(dests[i].monorail_stop->name, monorailStop) == 0)))
            {
                displayCount++;

                if (requestType != DOOR_REQUEST_MAP_MOVE)
                    cur += sprintf(    cur,"%s<td align=center valign=center>%s%s", rowStyle, missionMarker, fullMapName);
                else 
                    cur += sprintf(    cur,"<a href='cmd:%s'>%s<td align=center valign=center>%s%s", fullCommand, rowStyle, missionMarker, fullMapName);
                strcat(cur, localizedPrintf(e, "YouAreHere" ));
                cur += strlen(localizedPrintf(e,"YouAreHere"));
                if (requestType != DOOR_REQUEST_MAP_MOVE)
                    cur += sprintf( cur,"</td></tr>");
                else 
                    cur += sprintf( cur,"</td></tr></a>");

                    continue;
            }

            switch(dests[i].state)
            {
            case MAP_OPEN_LOW:
                if(sel.map_id == info->mapID && strncmp(sel.spawn_name,dests[i].spawn_name,31) == 0)
                    RETURNMAP(info->mapID,dests[i].spawn_name);

                if(firstGood == -1)
                    firstGood = i;

                displayCount++;

                cur += sprintf(    cur,
                    "<a href='cmd:%s'>%s<td align=center valign=center>%s%s%s%s</td></tr></a>",
                    fullCommand, rowStyle, lwcColorTextOpen, missionMarker, fullMapName, lwcColorTextClose);
                break;

            case MAP_OPEN_HIGH:
                if(sel.map_id == info->mapID && strncmp(sel.spawn_name,dests[i].spawn_name,31) == 0)
                    RETURNMAP(info->mapID,dests[i].spawn_name);

                displayCount++;

                if(!foundGood && !foundEmpty)
                {
                    if(firstGood == -1)
                        firstGood = i;

                    cur += sprintf(    cur,
                        "<a href='cmd:%s'>%s<td align=center valign=center>%s%s%s%s</td></tr></a>",
                        fullCommand, rowStyle, lwcColorTextOpen, missionMarker, fullMapName, lwcColorTextClose);
                }
                else
                {
                    cur += sprintf(    cur, "%s<td align=center valign=center>%s%s - ", rowStyle, missionMarker, fullMapName );
                    strcat(cur, localizedPrintf(e,"Full"));
                    cur += strlen(localizedPrintf(e,"Full"));
                    cur += sprintf(    cur, "</td></tr>");
                }
                break;

            case MAP_EMPTY:
                if(!foundGood)
                {
                    if(sel.map_id == info->mapID && strncmp(sel.spawn_name,dests[i].spawn_name,31) == 0)
                        RETURNMAP(info->mapID,dests[i].spawn_name);

                    if(firstGood == -1)
                        firstGood = i;

                    displayCount++;

                    cur += sprintf(    cur,
                        "<a href='cmd:%s'>%s<td align=center valign=center>%s%s%s%s</td></tr></a>",
                        fullCommand, rowStyle, lwcColorTextOpen, missionMarker, fullMapName, lwcColorTextClose);
                }
                break;

            case MAP_FULL:
                displayCount++;

                if(forceGood && !foundGood && !foundAlmostGood && !foundEmpty)
                {
                    if(sel.map_id == info->mapID && strncmp(sel.spawn_name,dests[i].spawn_name,31) == 0)
                        RETURNMAP(info->mapID,dests[i].spawn_name);

                    if(firstGood == -1)
                        firstGood = i;
                    cur += sprintf(    cur,
                        "<a href='cmd:%s'>%s<td align=center valign=center>%s%s%s%s</td></tr></a>",
                        fullCommand, rowStyle, lwcColorTextOpen, missionMarker, fullMapName, lwcColorTextClose);
                }
                else
                {                            
                    cur += sprintf(    cur, "%s<td align=center valign=center>%s%s - ", rowStyle, missionMarker, fullMapName );
                    strcat(cur, localizedPrintf(e,"Full"));
                    cur += strlen(localizedPrintf(e,"Full"));
                    cur += sprintf(    cur, "</td></tr>");
                }
                break;
            case MAP_TOO_HIGH:
                cur += sprintf(    cur, "%s<td align=center valign=center>%s%s - ", rowStyle, missionMarker, fullMapName );
                strcat(cur, localizedPrintf(e,"TooLow"));
                cur += strlen(localizedPrintf(e,"TooLow"));
                cur += sprintf(    cur, "</td></tr>");
                break;
            case MAP_FAILED_REQUIRES:
                cur += sprintf(    cur, "%s<td align=center valign=center>%s%s - ", rowStyle, missionMarker, fullMapName );
                strcat(cur, localizedPrintf(e,"MissingRequires"));
                cur += strlen(localizedPrintf(e,"MissingRequires"));
                cur += sprintf(    cur, "</td></tr>");
                break;
            }
        }

        if(!detailID && firstGood != -1 && displayCount == 1 && 
            (!sel.map_id || sel.map_id == dests[firstGood].info->mapID || sel.map_id == dests[firstGood].info->baseMapID))
        {
            RETURNMAP(dests[firstGood].info->mapID,dests[firstGood].spawn_name);
        }

        // ----------
        // the cancel string (monorails get a client-size cancel button)

        if (!monorailLine)
        {
            cur += sprintf(    cur,"<p><a href='cmd:enterdoor 0 0 0 -1'><tr><td align=center valign=center>" );
            strcat(cur, localizedPrintf(e,"CancelString"));
            cur += strlen(localizedPrintf(e,"CancelString"));
            cur += sprintf(    cur,"</td></tr></a>");
        }

        // ----------
        // close out the table

        if (monorailLine)
            cur += sprintf(cur, "%s", monorailSelectFormatBodyFooter);
        else
            cur += sprintf(cur, "%s", mapSelectFormatBottom);

        // ----------------------------------------
        // send the packet

        START_PACKET(pak, e, SERVER_MAP_XFER_LIST);
        if(requestType == DOOR_REQUEST_CLICK)
        {
            pktSendBits(pak, 1, 1);

            pktSendF32(pak, pos[0]);
            pktSendF32(pak, pos[1]);
            pktSendF32(pak, pos[2]);
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }
        pktSendBits(pak,1,monorailLine ? 1 : 0);
        if(monorailLine)
        {
            // 0 = hero
            // 1 = villain
            // 2 = praetorian
            // 3 = custom, texture name sent
            if(monorailLogo)
            {
                pktSendBits(pak,1,1);
                pktSendString(pak, monorailLogo);
            }
            else
            {
                pktSendBits(pak,1,0);
            }
        }
        pktSendBits(pak,1,detailID ? 1 : 0);
        pktSendString(pak, buffer);
        END_PACKET
    }
    else if (detailID)
    { //no valid exits for teleporter
        char buffer[20000];
        char* cur = buffer;
        int displayCount = 0;

        cur += sprintf(cur, "%s", mapSelectFormatTop1);
        strcat( cur, localizedPrintf(e,(char*)teleFormatTop2) );
        cur += strlen(localizedPrintf(e,(char*)teleFormatTop2) );
        cur += sprintf(cur, "%s", teleEmptyFormatTop3);
        strcat( cur, localizedPrintf(e,"NoTeleportDests") );
        cur += strlen(localizedPrintf(e,"NoTeleportDests") );
        cur += sprintf(cur, "%s", teleEmptyFormatTop4);


        cur += sprintf(    cur,"<p><a href='cmd:enterdoor 0 0 0 -1'><tr><td align=center valign=center>" );
        strcat(cur, localizedPrintf(e,"CancelString"));
        cur += strlen(localizedPrintf(e,"CancelString"));
        cur += sprintf(    cur,"</td></tr></a>");

        cur += sprintf(cur, "%s", mapSelectFormatBottom);

        START_PACKET(pak, e, SERVER_MAP_XFER_LIST);
        pktSendBits(pak, 1, 0);
        pktSendBits(pak,1,!!monorailLine);
        pktSendBits(pak,1,!!detailID);
        pktSendString(pak, buffer);
        END_PACKET

    }

    result.map_id = result.mowner = result.sahandle.context = result.sahandle.subhandle = result.sahandle.compoundPos = 0;
    sprintf(result.spawn_name,"");
    return &result;
}

// *********************************************************************************
// Monorail wrapper
// *********************************************************************************

bool CatchMonorail(Entity *e, char *line)
{
    DoorEntry door;
    MapSelection* mapSelect;


    door.monorail_line = line;

    mapSelect = checkDoorOptions(e, 0, NULL, &door, NULL, 1, DOOR_REQUEST_MAP_MOVE, 0);
    e->pl->usingTeleportPower = true;
    //    if mapSelect->map_id is non zero, 
    if(mapSelect && mapSelect->map_id)
    {
        //    that means there was only 1 map to zone to, so just go there, w/o bothering the player with a dialogue
        //    prior to this, we just did nothing which was the equivalent of no op to the player - EML 03/01/09
        char selectedMap[256];
        sprintf(selectedMap, "%d.%s", mapSelect->map_id, mapSelect->spawn_name);
        character_NamedTeleport(e->pchar, 0, selectedMap );
    }

    //    otherwise, there were multiple maps to choose from, so let the player choose first
    return true;
}


// *********************************************************************************
// sgrp doors.
// *********************************************************************************

static BaseAccess sgrp_FilterBaseAccess(Supergroup *sg, int idSgrp, int idEntRequesting, BaseAccess access)
{
    BaseAccess accessState = access;
    
    // ----------
    // check base upkeep
    
    if( accessState == kBaseAccess_Allowed  && sg && sg->upkeep.prtgRentDue>0)
    {
        const BaseUpkeepPeriod *period = sgroup_GetUpkeepPeriod( sg );
        if( period && period->denyBaseEntry )
        {
            accessState = kBaseAccess_RentOwed;
        }
    }
    
    // ----------
    // check raid active
    
    if( accessState == kBaseAccess_Allowed )
    {
        // MAK - only filter here if the participant list for raid is already full
        Entity *e = entFromDbId(idEntRequesting); 
        if( e && RaidSGIsDefending(idSgrp) && !RaidPlayerIsDefending(e))
        {
            if (!RaidHasOpenSlot(idSgrp))
                accessState = kBaseAccess_RaidScheduled;
        }
    }
    
    return accessState;
}


static BaseAccess s_BaseAccessFromSgrp(Supergroup *sg, int idSgrp, int idEntRequesting, SgrpBaseEntryPermission bep)
{
    BaseAccess res = sgrp_BaseAccessFromSgrp(sg, bep);
    return sgrp_FilterBaseAccess(sg, idSgrp, idEntRequesting, res);
}

//----------------------------------------
//  The sgrp bases that a player can enter
// must be fetched asynchronously from the db
// start the process here, and continue to fill 
// out the form as the results come back.
//----------------------------------------
static bool s_EnterBases(Entity *e, Vec3 pos)
{
    int i;
    bool res = false;
    
    if(verify( e ))
    {
        bool reqMade = false;
        int *idSgrps = NULL;
        char **baseNames = NULL; 
        int *baseAccesses = NULL; // BaseAccess

        // if your sg is in a raid, you can only go to your own base
          bool raid = RaidSGIsDefending(e->supergroup_id);

        // DGNOTE 1/11/2010
        // If the player is dark, they can't enter their own SG base, nor can they enter any coalition bases
        int isDark = sgroup_isPlayerDark(e);

        // ----------
        // if the player has a base, add it, and send them the window right away.

        if (e->supergroup && !isDark )
        {
//             sgrppendingrequestinfo_AddBaseInfo(info,e->supergroup_id,e->supergroup, kBaseAccess_Allowed);
            eaiPush(&idSgrps, e->supergroup_id );
            eaPush(&baseNames, e->supergroup->name );
            eaiPush(&baseAccesses, sgrp_FilterBaseAccess(e->supergroup, e->supergroup_id, e->db_id, kBaseAccess_Allowed));
        }
        
        // ---------------------------------------- 
        // build the request of coalition members

        if( e->supergroup && !raid && !isDark )
        {
            int *sgrpsToQuery = NULL;

            // -----
            // gather allies
            
            for( i = 0; i < ARRAY_SIZE( e->supergroup->allies ); ++i ) 
            {
                SupergroupAlly    *ally = &e->supergroup->allies[i];
                int ally_busy = RaidSGIsDefending(ally->db_id);

                if( ally->db_id > 0 && !ally_busy  )
                {
                    int idSgrp = ally->db_id;
                    Supergroup *sg = sgrpFromDbId(idSgrp);
                    
                    if( sg )
                    {
                        eaiPush(&idSgrps, idSgrp );
                        eaPush(&baseNames, sg->name );
                        eaiPush(&baseAccesses, s_BaseAccessFromSgrp(sg, idSgrp, e->db_id, kSgrpBaseEntryPermission_Coalition));
                    }
                    else
                    {    
                        eaiPush(&sgrpsToQuery, idSgrp);
                    }
                }
            }
            
            // -----
            // send the request
            
            if( eaiSize(&sgrpsToQuery) > 0 )
            {
//                 dbAsyncContainersRequest(CONTAINER_SUPERGROUPS, sgrpsToQuery, CONTAINER_CMD_TEMPLOAD, s_dbReceiveReqdSgrps);
//                 info->reqs[kReqTypes_Coalition] = kCoalitionReq_Requesting;
                // @todo -AB: request from statserver :11/04/05
                char *statcmd = NULL;
                int i;
                
                estrCreate(&statcmd);
                estrPrintf(&statcmd, "sgrp_baseentrypermissions_req %d \"", e->db_id);
                for( i = eaiSize( &sgrpsToQuery ) - 1; i >= 0; --i)
                {
                    estrConcatf(&statcmd, "%i\t", sgrpsToQuery[i]);
                }
                estrConcatChar(&statcmd, '\"');

                serverParseClient(statcmd, NULL);
                estrDestroy(&statcmd);

                reqMade = true;
            }

            // cleanup
            eaiDestroy( &sgrpsToQuery );
        }
        
        // ----------------------------------------
        // team leader

        if( e->teamup_id && e->teamup && e->teamup->members.leader != e->db_id && !raid )
        {
            int idEntTeamLeader = e->teamup->members.leader;
            Entity *entTeamLeader = entFromDbId(idEntTeamLeader);
            
            if( entTeamLeader && entTeamLeader->pl )
            {
                if (e->pl && entTeamLeader->supergroup)
                {
                    Supergroup *sg = entTeamLeader->supergroup;
                    int idSgrp = entTeamLeader->supergroup_id;
                    if( !RaidSGIsDefending(idSgrp) )
                    {
                        BaseAccess accessState = s_BaseAccessFromSgrp(sg, idSgrp, e->db_id, kSgrpBaseEntryPermission_LeaderTeammates);
                    
                        // we have the leader already, get them
                        eaiPush(&idSgrps, idSgrp);
                        eaPush(&baseNames, sg->name);
                        eaiPush(&baseAccesses, accessState);
                    }
                }
            }
            else
            {
                if (e->pl) 
                {
                    char *cmd = NULL;
                    estrCreate(&cmd);
                    estrPrintf(&cmd, "baseaccess_froment_relay %d %d %d %d", e->db_id, 
                               kSgrpBaseEntryPermission_LeaderTeammates, 
                               idEntTeamLeader,
                               e->pl->playerType);
                    serverParseClient( cmd, NULL );
                    estrDestroy(&cmd);

                    reqMade = true;
                }
            }
        }

        // ----------
        // add the request if anything pending


        // if we have anything, send it now.
        if( eaiSize(&idSgrps) > 0 || reqMade )
        {
            int i;
            int n = eaiSize(&idSgrps);
            
            // send what we have so far to the client
            // will also delete this if these are the only info.
            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "BaseAccess:EnterBases Starting base entry");
            s_StartBaseEntry(e, pos);

            // send all the info that we know currently
            for( i = 0; i < n; ++i)
            {
                SendBaseAccess( e, baseNames[i], idSgrps[i], baseAccesses[i] );
            }
            res = true;
        }

		// Hack: s_AppendBaseAccess looks for idSgrp -1 to send the "enter passcode" command.
		SendBaseAccess(e, "Enter Passcode...", -1, kBaseAccess_Allowed);

        eaiDestroy(&idSgrps);
        eaDestroy(&baseNames);
        eaiDestroy(&baseAccesses);
    }

    // ----------
    // finally

    return res;
}

// public wrapper in case there is some reason that the enterBases is static
bool EnterBases(Entity *e, Vec3 pos)
{
    return s_EnterBases(e, pos);
}


#define ENTER_BASE_CMDNAME "enter_base_from_sgid"

//----------------------------------------
//  Update the client with base access info
//----------------------------------------
static char* s_AppendBaseAccess(char *cur, Entity *e, char *baseName, int idSgrp, BaseAccess accessState)
{
    char fullCommand[500];
    char *commandString = ENTER_BASE_CMDNAME;
    
     if( idSgrp == e->supergroup_id )
     {
         baseName = localizedPrintf(e,"SgrpBaseSelectMyBase");
     }
        
    if( accessState == kBaseAccess_Allowed )
    {
		if (idSgrp == -1) {
			sprintf(fullCommand,"sg_enter_passcode");
		} else {
			sprintf(fullCommand,"%s %d 0",commandString,idSgrp);				
		}
		cur += sprintf(	cur,
						"<a href='cmd:%s'><tr><td align=center valign=center>%s</td></tr></a>",
                        fullCommand,
                        baseName);
    }
    else // entry denied state
    {
        char *msBase = NULL;
        
        switch ( accessState )
        {
        case kBaseAccess_None:
        case kBaseAccess_PermissionDenied:
        {
            msBase = "SgrpBaseSelectMsgPermissionDenied";
        }
        break;
        case kBaseAccess_RentOwed:
            msBase = (idSgrp == e->supergroup_id) ? "SgrpBaseSelectMsgYourBaseRentDue" : "SgrpBaseSelectMsgRentDue";
            break;
        case kBaseAccess_RaidScheduled:
            msBase = "SgrpBaseSelectMsgCannotEnterRaidScheduled";
            break;
        default:
            verify(0 && "invalid switch value.");
        };
        STATIC_INFUNC_ASSERT( kBaseAccess_Count == 5 ); // update if new enums added
        
        // base msg associated with this
        // translate it (always takes name of base as param)
        if( msBase )
        {
            baseName = localizedPrintf(e, msBase, baseName );
            cur += sprintf(    cur,
                            "<font face=computer outline=0 color=#ff3d3d>"
                            "<tr><td align=center valign=center>%s</td></tr>" // actual string
                            "</font>",
                            baseName);                    
        }
    }

    return cur;
}

//----------------------------------------
//  Update the player's base select window with the passed sgrp ids
//----------------------------------------
static void s_StartBaseEntry(Entity *e, Vec3 pos)
{    
    // ----------------------------------------
    // start the empty base door, send the anchor pos
    
    START_PACKET(pak, e, SERVER_MAP_XFER_LIST_INIT);
    pktSendBits(pak, 1, pos?1:0);                            // send anchor pos
    if(pos)
    {
        pktSendF32(pak, pos[0]);
        pktSendF32(pak, pos[1]);
        pktSendF32(pak, pos[2]);
    }
    pktSendBits(pak,1,0);                            // monorail line
    pktSendBits(pak,1,1);                            // are we a base teleporter line
    pktSendString(pak, localizedPrintf(e,"SgrpBaseSelectTitle"));
    END_PACKET;
}

void SendBaseAccess(Entity *e, char *nameBase, int idSgrp, BaseAccess access)
{
    if( e && nameBase && idSgrp && INRANGE0( access, kBaseAccess_Count ))
    {
        char txtUpdate[1024] = {0};
        s_AppendBaseAccess(txtUpdate, e, nameBase, idSgrp, access);
        
        START_PACKET(pak, e, SERVER_MAP_XFER_LIST_APPEND);
        pktSendBits(pak, 1, 0);                            // not a click door
        pktSendBits(pak,1,0);                            // monorail line
        pktSendBits(pak,1,1);                            // are we a base teleporter line
        pktSendBitsAuto(pak, idSgrp);                    // send the update id
        pktSendString(pak, localizedPrintf(e,"SgrpBaseSelectTitle"));        // title string
        pktSendString(pak, txtUpdate);                    // the update string
        END_PACKET;

        LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "BaseAccess:SendToClient %s(%i),idsg=%i,basename=%s %s",
            baseaccess_ToStr(access), access,idSgrp,nameBase,(e->supergroup_id==idSgrp?"(player base)":""));
    }
}
