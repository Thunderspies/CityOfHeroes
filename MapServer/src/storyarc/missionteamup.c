/*\
 *
 *    missionteamup.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    handles the mission-teamup interaction (tricky)
 *    deals with how to start and stop mission maps and when
 *    to send updates to teamups.  Also constructs mission status
 *    strings
 *
 */

#include "mission.h"
#include "entity/SgrpStats.h"
#include "storyarcprivate.h"
#include "container/team.h"
#include "entity/teamup.h"
#include "dbcomm/dbcontainer.h"
#include "container/containerbroadcast.h"
#include "svr/svr_player.h"
#include "dbcomm/dbnamecache.h"
#include "dbcomm/dbcomm.h"
#include "cmdparse/cmdserver.h"
#include "comm_game.h"
#include "door.h"
#include "reward.h"
#include "gameComm/svr_chat.h"
#include "entity/entity.h"
#include "player/badges_server.h"
#include "entity/supergroup.h"
#include "entity/origins.h"
#include "entity/character_level.h"
#include "entity/buddy_server.h"
#include "taskforce.h"
#include "dbcomm/logcomm.h"
#include "entity/entGameActions.h"

#define MISSION_HELPER_REQUIREMENT 60 // in percentage points, time on mission map

// *********************************************************************************
//  Teamup list broadcasts - send events to every teamup this mission has seen
// *********************************************************************************

int* g_teamupids = 0;        // list of all the id's I've seen on this map

// MissionIdStrings are used in broadcast messages to be sure we're talking about the same mission
char* MissionConstructIdString(int owner, StoryTaskHandle *sahandle)
{
    static char buf[100];
    sprintf(buf, "%i:%i:%i:%i", owner, sahandle->context, sahandle->subhandle, sahandle->compoundPos);
    return buf;
}

int MissionDeconstructIdString(const char* str, int* owner, StoryTaskHandle* sahandle)
{
    return sscanf(str, "%i:%Ii:%i:%i", owner, &sahandle->context, &sahandle->subhandle, &sahandle->compoundPos) == 4;
}

// this player has logged into the map, record his teamup id for future use
void MissionTeamupListAdd(Entity* player)
{
    if (!OnMissionMap()) return;
    if (!g_teamupids)
    {
        log_printf("storyerrs.log", "MissionTeamupListAdd: I got here without having g_teamupids initted?");
        return;
    }
    if (player && player->teamup_id)
    {
        int i, n;

        n = eaiSize(&g_teamupids);
        for (i = 0; i < n; i++)
        {
            if (player->teamup_id == g_teamupids[i])
                return; // already have it
        }
        eaiPush(&g_teamupids, player->teamup_id);

        // A new team is joining the mapserver.  Tell the team
        // about the current status of the mission.
        MissionRefreshTeamupStatus(player);
    }
    // at this point, the mission has started AND player is ready
    TaskStatusSendAll(player);
}

// send a message to every team this mission has seen
void MissionTeamupListBroadcast(char* message)
{
    if (!g_teamupids)
    {
        log_printf("missiondoor.log", "MissionTeamupListBroadcast: asked to broadcast before I've even initialized my teamupid's");
        return;
    }
    dbBroadcastMsg(CONTAINER_TEAMUPS, g_teamupids, INFO_SVR_COM, 0, eaiSize(&g_teamupids), message);
}

// *********************************************************************************
//  Mission objective broadcasts
// *********************************************************************************

// MissionObjectiveStrings are used to broadcast an objective event to teams
char* MissionConstructObjectiveString(int owner, StoryTaskHandle *sahandle, int objnum, int success)
{
    static char buf[100];
    sprintf(buf, "%i,%i,%i,%i,%i,%i", owner, sahandle->context, sahandle->subhandle, sahandle->compoundPos, objnum, success);
    return buf;
}
int MissionDeconstructObjectiveString(char* str, int* owner, StoryTaskHandle *sahandle, int* objnum, int* success)
{
    return sscanf(str, "%i,%Ii,%i,%i,%i,%i", owner, &sahandle->context,  &sahandle->subhandle, &sahandle->compoundPos, objnum, success);
}

// send information on a completed objective to team and global info
void MissionObjectiveInfoSave(const MissionObjective* def, int succeeded)
{
    char buf[1000];
    int objnum = MissionObjectiveFindIndex(g_activemission->def, def);
    if (objnum < 0)
    {
        log_printf("storyerrs.log", "MissionObjectiveInfoSave: asked to save objective %s for mission %s, but I can't find it",
            def->name, g_activemission->task->logicalname);
        return;
    }

    // save info to my local global
    ObjectiveInfoAdd(g_activemission->completeObjectives, &g_activemission->sahandle, objnum, succeeded);

    // now send to all teams
    strcpy(buf, DBMSG_MISSION_OBJECTIVE);
    strcat(buf, MissionConstructObjectiveString(g_activemission->ownerDbid, &g_activemission->sahandle, objnum, succeeded));
    MissionTeamupListBroadcast(buf);
}

// in response to DBMSG_MISSION_OBJECTIVE
void MissionHandleObjectiveInfo(Entity* player, int cpos, int objnum, int success)
{
    if (teamLock(player, CONTAINER_TEAMUPS))
    {
        StoryTaskInfo* task = player->teamup->activetask;
        ObjectiveInfoAdd(task->completeObjectives, &task->sahandle, objnum, success);
        teamUpdateUnlock(player, CONTAINER_TEAMUPS);
    }
}

// *********************************************************************************
//  Mission-owner interaction
// *********************************************************************************

// If the owner logged, assign a new one if anyone is left
static void MissionFindNewSGOwner()
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_OWNED];
    int i;

    // Make sure we get someone online
    for(i = 0; i < players->count; i++)
    {
        if (entFromDbId(players->ents[i]->db_id))
        {
            MissionChangeOwner(players->ents[i]->db_id, &g_activemission->sahandle);
            break;
        }
    }
}

// need to keep a history of messages sent to owner, so we can replay them if 
// our owner changes
char** ownerMessages = 0;

// all messages to owner should go through here.. they will be rebroadcast to
// the new owner if the owner changes, the command should have a %i in it for the
// owner dbid
void MissionSendToOwner(char* cmd)
{
    char cmdbuf[4000];

    if (!ownerMessages) 
        eaCreate(&ownerMessages);
    eaPush(&ownerMessages, strdup(cmd));
    if (g_activemission && g_activemission->ownerDbid)
    {
        if (!entFromDbId(g_activemission->ownerDbid) && g_activemission->supergroupDbid)
            MissionFindNewSGOwner();
        sprintf(cmdbuf, cmd, g_activemission->ownerDbid);
        serverParseClient(cmdbuf, NULL);
    }
}

// we were asked to adjust the owner of the mission - for taskforces currently
void MissionChangeOwner(int new_owner, StoryTaskHandle *sahandle)
{
    char cmdbuf[4000];
    int i;

    // do we really need to change anything?
    if (g_activemission && g_activemission->ownerDbid != new_owner &&
        isSameStoryTaskPos( &g_activemission->sahandle, sahandle) && 
        !db_state.is_endgame_raid)    //    this is a companion check that compliments the same story task pos check
    {
        // ok, reset owner and resend all messages
        g_activemission->ownerDbid = new_owner;

        for (i = 0; i < eaSize(&ownerMessages); i++)
        {
            sprintf(cmdbuf, ownerMessages[i], g_activemission->ownerDbid);
            serverParseClient(cmdbuf, NULL);
        }
    }
}

// *********************************************************************************
//  Mission-teamup interaction
// *********************************************************************************

// called whenever the teamup changes for this entity
// - teamup leaders will then decide whether teamup should be detached from mission
void MissionTeamupUpdated(Entity* e)
{
    // prevent recursive calls - they happen because we update the team inside (arg)
    static int insideMissionTeamupUpdated = 0;
    if (insideMissionTeamupUpdated) return;
    if(!e->teamup)
    {
        dbLog("MissionTeamupUpdated", e, "entity without teamup");
        return;
    }
    insideMissionTeamupUpdated = 1;

    // Decide whether to shutdown the mission mapserver or not.
    if (e->teamup->members.leader == e->db_id &&
        TaskIsMission(e->teamup->activetask))
    {
        // is owner still on team?
        if (team_IsMember(e, e->teamup->activetask->assignedDbId) || (TASK_IS_SGMISSION(e->teamup->activetask->def)))
        {
            // detach if mission is complete, and no one on map
            if (!TASK_INPROGRESS(e->teamup->activetask->state) &&
                e->teamup->map_playersonmap == 0)
            {
                teamlogPrintf(__FUNCTION__, "closing mission (complete) %s(%i) team:%i task %i:%i:%i:%i owner:%i", 
                    e->name, e->db_id, e->teamup_id, SAHANDLE_ARGSPOS(e->teamup->activetask->sahandle),    e->teamup->activetask->assignedDbId);
                teamDetachMap(e);
                log_printf("missiondoor.log", " - because mission is complete");
            }
        }
        else // owner not on team
        {
            // detach if no one is on map
            if (e->teamup->map_playersonmap == 0)
            {
                teamlogPrintf(__FUNCTION__, "closing mission (owner gone) %s(%i) team:%i task %i:%i:%i:%i owner:%i", 
                    e->name, e->db_id, e->teamup_id, SAHANDLE_ARGSPOS(e->teamup->activetask->sahandle),    e->teamup->activetask->assignedDbId);
    
                teamDetachMap(e);
                log_printf("missiondoor.log", " - because owner is not on team");
            }
        }

        // hook for mission kicking system to notice team change too
        MissionCheckForKickedPlayers(e);
    }

    // If the team mission owner is processing this message...
    //    This needs to be done after the mission has detached from the team up, because
    //    it changes the completion status of the task.
    if(!e->teamup)
    {
        dbLog("MissionTeamupUpdated", e, "Entity without teamup");
    }
    else if(!e->teamup->activetask)
    {
        dbLog("MissionTeamupUpdated", e, "Teamup without active task");
    }
    else if(e->db_id == e->teamup->activetask->assignedDbId && TaskIsMission(e->teamup->activetask))
    {
        MissionTeamupAdvanceTask(e);
    }

    // set you to the active player...
    if (e->teamup && e->teamup->activetask && e->pl
            // if active player changed to you
        && ((e->db_id == e->teamup->activetask->assignedDbId
                && e->teamup->activetask->assignedDbId != e->teamup->probationalActivePlayerDbid)
            // or if you're the leader and...
            || (e->db_id == e->teamup->members.leader
                    // active player hasn't yet been assigned
                && (!e->teamup->probationalActivePlayerDbid)
                    // or if the active player is no longer on the team (and you don't have an active task)
                    || (!e->teamup->activetask->assignedDbId && !team_IsMember(e, e->teamup->probationalActivePlayerDbid)))))
    {
        if (teamLock(e, CONTAINER_TEAMUPS))
        {
            e->teamup->probationalActivePlayerDbid = e->db_id;
            e->teamup->probationalActivePlayerDbidExpiration = timerSecondsSince2000() + ACTIVE_PLAYER_PROBATION_PERIOD_SECONDS;

            teamUpdateUnlock(e, CONTAINER_TEAMUPS);
        }
    }

    insideMissionTeamupUpdated = 0;
}

void MissionTeamupAdvanceTask(Entity* e)
{
    if (!e->teamup) 
        return;
    if (e->teamup->activetask->assignedDbId != e->db_id) 
        return;
    if (!TaskIsMission(e->teamup->activetask)) 
        return;

    if (!TASK_INPROGRESS(e->teamup->activetask->state) && e->teamup->map_playersonmap == 0)
    {
        char cmdbuf[2000];
        sprintf(cmdbuf, "taskadvancecomplete %i", e->db_id); // MAK - must relay here, because this ent may not be owned any more
        serverParseClient(cmdbuf, NULL);
    }
}

static int g_debugshowfakecontact = 0; // if set, every player will see the "Mark's Mission" contact

// update the client with the current status of the mission,
// called whenever the teamup changes for this entity
void MissionSendRefresh(Entity* e)
{
    // if player is not on teamup, clear mission info
    if (!e->teamup_id || (!e->teamup->activetask->sahandle.context && !e->teamup->activetask->sahandle.subhandle && !e->teamup->activetask->sahandle.bPlayerCreated))
    {
        ClearMissionStatus(e);
    }
    // if the mission belongs to me, send all info
    else if (!g_debugshowfakecontact && e->teamup->activetask->assignedDbId == e->db_id)
    {
        SendMissionStatus(e);
        ContactStatusSendAll(e);
        TaskStatusSendUpdate(e);
    }
    // otherwise, send mission info update to player
    else
    {
        SendMissionStatus(e);
    }
    e->storyInfo->keyclueChanged = 1; // get a refresh of team clues always
}

void MissionSetShowFakeContact(int set)
{
    g_debugshowfakecontact = set;
}

void MissionRefreshTeamupStatus(Entity* e)
{
    if (teamLock(e, CONTAINER_TEAMUPS))
    {
        MissionConstructStatus(e->teamup->mission_status, e, e->teamup->activetask);
        e->teamup->keyclues = g_keyclues;
        teamUpdateUnlock(e, CONTAINER_TEAMUPS);
    }
}

void MissionRefreshAllTeamupStatus(void)
{
    int i;
    for(i = eaiSize(&g_teamupids)-1; i >= 0; i--)
    {
        Entity* e = svrFindPlayerInTeamup(g_teamupids[i], PLAYER_ENTS_ACTIVE);
        if(e && e->teamup && isSameStoryTask(  &g_activemission->sahandle, &e->teamup->activetask->sahandle ))
            MissionRefreshTeamupStatus(e);
    }
}

// just set the complete flag on the mission
void MissionSetTeamupComplete(Entity* e, int success)
{
    if (teamLock(e, CONTAINER_TEAMUPS))
    {
        if (TASK_INPROGRESS(e->teamup->activetask->state) && e->teamup->activetask->sahandle.context)
        {
            e->teamup->activetask->state = success? TASK_SUCCEEDED : TASK_FAILED;
            MissionConstructStatus(e->teamup->mission_status, e, e->teamup->activetask);
        }
        teamUpdateUnlock(e, CONTAINER_TEAMUPS);
    }
}

// team is detaching from a mission map - team should be locked
void MissionMapDetaching(Entity* e)
{
    if (!e->teamup) 
        return;
    if (TaskIsMission(e->teamup->activetask))
    {
        log_printf("missiondoor.log", "Detaching teamup %i from map %i, owner %s", e->teamup_id, e->teamup->map_id,
            e->teamup->activetask->assignedDbId? dbPlayerNameFromId(e->teamup->activetask->assignedDbId): "(No Owner)");
    }
    if(e->teamup->members.count == 1 || !team_IsMember(e, e->teamup->activetask->assignedDbId))
        TeamTaskClearNoLock(e->teamup);
    e->teamup->keyclues = 0;
}

// does a syncronous db call to verify this map is running
// (even if not ready yet) and the correct mission
int CheckMissionMap(int map_id, int owner, int sgowner, int taskforce, StoryTaskHandle *sahandle, int* ready)
{
    int mowner, msgowner, mtaskforce, dontcare;
    U32 udontcare;
    ContactHandle unused = 0;
    VillainGroupEnum villainGroup;
    ContainerStatus c_list[1];
    EncounterAlliance encAlly;
    StoryTaskHandle mhandle;
    StoryDifficulty doesntMatter;
    c_list[0].id = map_id;
    dbSyncContainerStatusRequest(CONTAINER_MAPS, c_list, 1);
    if (!c_list[0].valid) 
        return 0;

    if (!MissionDeconstructInfoString(c_list[0].mission_info, &mowner, &msgowner, &mtaskforce, &unused, &mhandle,
        &udontcare, &dontcare, &doesntMatter, &dontcare, &encAlly, &villainGroup))
        return 0;
    if (ready) *ready = c_list[0].ready;
    if (!taskforce && (owner != mowner)) 
        return 0; // task forces don't care if the owner has changed
    //if (msgowner && (sgowner != msgowner)) 
    //    return 0; // only check sg if its a sg mission
    return (mtaskforce == taskforce) && isSameStoryTask( &mhandle, sahandle);
}


// attach teamup to mission
int MissionAttachTeamup(Entity* owner, StoryTaskInfo* mission, int checkmap)
{
    int ready = 0;
    StoryInfo* info;
    info = StoryInfoFromPlayer(owner);

    teamlogPrintf(__FUNCTION__, "%s(%i) team %i", owner->name, owner->db_id, owner->teamup_id);
    if (teamLock(owner, CONTAINER_TEAMUPS))
    {
        MissionAttachTeamupUnlocked(owner, owner, mission, checkmap);
        
        teamUpdateUnlock(owner, CONTAINER_TEAMUPS);
        return owner->teamup->activetask->sahandle.context && owner->teamup->activetask->sahandle.subhandle;
    }
    printf("mission.c - couldn't lock the team!\n");
    return 0; // couldn't lock team
}

void MissionAttachTeamupUnlocked(Entity* teamholder, Entity* owner, StoryTaskInfo* mission, int checkmap)
{
    int ready = 0;
    int sgowner = 0;
    StoryInfo* info;
    Teamup* team = teamholder->teamup;
    info = StoryInfoFromPlayer(owner);

    // select the task first
    teamDetachMapUnlocked(teamholder);
    team->mission_contact = TaskGetContactHandle(StoryInfoFromPlayer(owner), mission);
    memcpy(team->activetask, mission, sizeof(StoryTaskInfo));
    team->mission_status[0] = 0;

    team->map_id = mission->missionMapId;

    // Is it the supergroup mission?
    if (teamholder->supergroup && TASK_IS_SGMISSION(mission->def))
        sgowner = teamholder->supergroup_id;

    // if we are attaching right when clicking on the door, this check would be redundant
    if (checkmap)
    {
        if (CheckMissionMap(team->map_id, team->activetask->assignedDbId, sgowner, owner->taskforce_id, &team->activetask->sahandle, &ready))
        {
            teamlogPrintf(__FUNCTION__, "mission %i running", team->map_id); 
        }
        else 
        {
            dbLog("Team", teamholder, "MissionAttachTeamupUnlocked map_id %d map_type %d", team->map_id, team->map_type );
            team->map_id = 0;
        }
    }
    if (team->map_id || team->activetask->sahandle.bPlayerCreated)
        team->map_type = MAPINSTANCE_MISSION;

    if( team->activetask->sahandle.bPlayerCreated ) // set the task level every mission
         team->activetask->level = character_GetExperienceLevelExtern(teamholder->pchar);

    team->team_level = character_CalcExperienceLevel(owner->pchar);
    team->team_mentor = owner->db_id;

    TeamTaskUpdateStatusInternal(team, owner);
    if(TaskIsMission(mission))
    {
        teamlogPrintf(__FUNCTION__, "attaching team %i to map %i, owner %s(%i)", owner->teamup_id, mission->missionMapId, owner->name, owner->db_id);
        log_printf("missiondoor.log", "Attaching team %i to task %i:%i:%i:%i, map %i, owner %s\n", owner->teamup_id, 
            SAHANDLE_ARGSPOS(mission->sahandle), mission->missionMapId, owner->name);
        LOG_ENT_OLD(owner, "Mission:Attach Attaching team %i to map %i", owner->teamup_id, team->map_id);
    }
}

// assumes the team is locked
void MissionPlayerLeavingTeam(Entity* e)
{
    if (!OnMissionMap()) 
        return;
    if (!e->teamup_id) 
        return;

    if (e->teamup->map_playersonmap)
        e->teamup->map_playersonmap--;
    printf("%i players in mission\n", e->teamup->map_playersonmap);
    MissionPreparePlayerLeftTeam(e);
}

// for MissionPlayerLeftTeam
typedef struct PlayerLeftTeamInfo
{
    int mission_owner;
    int member_count;
    int map_id;
} PlayerLeftTeamInfo;
PlayerLeftTeamInfo g_leftTeamInfo;

// make some notes on the teamup for MissionPlayerLeftTeam
void MissionPreparePlayerLeftTeam(Entity* entOnTeam)
{
    if (!entOnTeam->teamup_id)
    {
        log_printf("missiondoor.log", "MissionPreparePlayerLeftTeam: called even though %s doesn't have a team", entOnTeam->name);
        g_leftTeamInfo.mission_owner = g_leftTeamInfo.member_count = g_leftTeamInfo.map_id = 0;
        return;
    }
    g_leftTeamInfo.mission_owner = entOnTeam->teamup->activetask->assignedDbId;
    g_leftTeamInfo.member_count = entOnTeam->teamup->members.count;
    g_leftTeamInfo.map_id = entOnTeam->teamup->map_id;
}

// player is leaving team, may need to get attached to the mission or get kicked from map
void MissionPlayerLeftTeam(Entity* player, int wasKicked)
{
    StoryInfo* info;

    if (!g_leftTeamInfo.member_count)
    {
        log_printf("missiondoor.log", "MissionPlayerLeftTeam: ignored call, no member count");
        return;
    }

    // Recreate and attach the player in a supergroup mission
    if (g_activemission && player->supergroup && (g_activemission->supergroupDbid == player->supergroup_id) && (g_leftTeamInfo.member_count > 1) && g_leftTeamInfo.map_id)
    {
        if (player->supergroup->activetask)
        {
            teamCreate(player, CONTAINER_TEAMUPS);
            MissionAttachTeamup(player, player->supergroup->activetask, 1);
            MissionTeamupListAdd(player);
            teamUpdatePlayerCount(player);
        }
    }
    else if ((player->db_id == g_leftTeamInfo.mission_owner) && (g_leftTeamInfo.member_count > 1) && g_leftTeamInfo.map_id)
        // if player is owner of teamup, and isn't just destroying entire team, re-attach him to mission
    {
        MissionReattachOwner(player, g_leftTeamInfo.map_id);
    }
    else if( wasKicked )
        MissionKickPlayer(player);


    // If a player left, make sure we still have a good owner
    if (g_activemission && g_activemission->supergroupDbid)
        MissionFindNewSGOwner();

    // if the owner of the team task leaves, make sure to update his task info (needs to notice team map id change)
    info = player->storyInfo;
    info->contactInfoChanged = 1;
}

// the owner of the mission is being reattached to a currently running mission
void MissionReattachOwner(Entity* player, int map_id)
{
    StoryTaskInfo* mission;
    int i;

    if (player->teamup_id) 
        return;    // weird race condition: if player has already joined a new team, don't have to worry about reattaching
    for (i = 0; mission = TaskGetMissionTask(player, i); i++)
    {
        if (mission->missionMapId != map_id) 
            continue;
        teamCreate(player, CONTAINER_TEAMUPS);
        MissionAttachTeamup(player, mission, 1);
        log_printf("missiondoor.log", "Reattaching owner %i to mission %i", player->db_id, player->teamup->map_id);
        return;
    }
    // similar race condition can bomb out here, it's ok
}

void MissionShowState(ClientLink* client)
{
    Entity* player;
    StoryInfo* info;
    int i,n, found = 0;

    if (!client->entity) 
        return;
    player = client->entity;
    if (player->teamup_id)
    {
        Teamup* teamup = player->teamup;
        conPrintf(client, saUtilLocalize(player,"TeamStateDash") );
        conPrintf(client, "\t%s", saUtilLocalize(player, "DoorId", teamup->activetask->doorId));
        conPrintf(client, "\t%s", saUtilLocalize(player, "MapId",  teamup->map_id));
        conPrintf(client, "\t%s", saUtilLocalize(player, "PlayersOnMap",  teamup->map_playersonmap));
        conPrintf(client, "\t%s", saUtilLocalize(player, "MissionStatus",  teamup->mission_status));
        conPrintf(client, "\t%s", saUtilLocalize(player, "MissionComplete",  !TASK_INPROGRESS(teamup->activetask->state)));
        conPrintf(client, "\t%s", saUtilLocalize(player, "MissionOwner",  teamup->activetask->assignedDbId));
        conPrintf(client, "\t%s", saUtilLocalize(player, "MissionLevel",  teamup->activetask->level));
        conPrintf(client, "\t%s", saUtilLocalize(player, "MissionId",  SAHANDLE_ARGSPOS(teamup->activetask->sahandle) ));
        conPrintf(client, "\t%s", saUtilLocalize(player, "MissionKheldianCount",  teamup->kheldianCount));
    }
    else
    {
        conPrintf(client, saUtilLocalize(player,"NoTeam"));
    }

    conPrintf(client, saUtilLocalize(player,"YourMissions") );
    info = StoryInfoFromPlayer(player);
    n = eaSize(&info->tasks);
    for (i = 0; i < n; i++)
    {
        StoryTaskInfo* task = info->tasks[i];
        if (task->def->type != TASK_MISSION) 
            continue;
        conPrintf(client, "\t%s (%i,%i,%i:%i) assigned map %i, door %i", task->def->logicalname, SAHANDLE_ARGSPOS(task->sahandle), task->missionMapId, task->doorId);
        found = 1;
    }
    if (!found)
        conPrintf(client, "\tYou have no missions");
}

// *********************************************************************************
//  Keeping track of active players
// *********************************************************************************

static StashTable player_counts = 0;

// increment counters for an individual
void MissionCountPlayer(int db_id, int ticks)
{
    int        prevcount = 0;

    if (!player_counts)
        player_counts = stashTableCreateInt(20);
    stashIntFindInt(player_counts, db_id, &prevcount);
    prevcount += ticks;
    stashIntAddInt(player_counts, db_id, prevcount, true);
}

// clear counters for an individual
void MissionCountPlayerReset(int db_id)
{
    if (!player_counts) 
        return;
    stashIntAddInt(player_counts, db_id, 0, true);
}

// increments participation counters for each player on the map
void MissionCountAllPlayers(int ticks)
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int foundplayer = 0;
    int i;

    for (i = 0; i < players->count; i++)
    {
        // don't count admins spying as players doing mission
        if (ENTINFO(players->ents[i]).hide) 
            continue;

        MissionCountPlayer(players->ents[i]->db_id, ticks);
    }
}

// check and see if all players have left the map
void MissionCountCheckPlayersLeft(void)
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int anyoneEntered = player_counts && stashGetValidElementCount(player_counts);

    if (anyoneEntered && !players->count)
        g_playersleftmap = 1;
    else
        g_playersleftmap = 0;
}

static int newspaperBrokerHandle = 0;

// if the player hasn't entered the map yet, they get a tick
// also handles other mission hooks for player entering map
void MissionPlayerEnteredMap(Entity* ent)
{
    int here = 0;
    PlayerEnts* players = &player_ents[PLAYER_ENTS_OWNED];

    if (!OnMissionMap()) 
        return;

    // Make sure there aren't too many on the map
    if (g_activemission->supergroupDbid && (players->count > SGMISSION_MAX_PLAYERS))
    {
        MissionKickPlayer(ent);
        return;
    }

    MissionTeamupListAdd(ent);    // grab their teamup id for broadcasts
    MissionSendEntryText(ent);    
    MissionWaypointSendAll(ent);
    KeyDoorNewPlayer(ent);
    // don't count admins spying as players doing mission
    if (ENTINFO(ent).hide)
        return;

    if (!player_counts)
        player_counts = stashTableCreateInt(20);
    stashIntFindInt(player_counts, ent->db_id, &here);
    if (!here)
    {
        stashIntAddInt(player_counts, ent->db_id, 1, true);
    }

    // If its a newspaper mission, snag who it is for from the owner
    if (ent->db_id == g_activemission->ownerDbid && ContactGetNewspaper(ent) == g_activemission->contact)
    {
        StoryInfo* info = StoryInfoFromPlayer(ent);
        StoryContactInfo* contactInfo = ContactGetInfo(info, g_activemission->contact);
        newspaperBrokerHandle = contactInfo ? contactInfo->brokerHandle : 0;
    }

    // Change the ownerid so there are updates for SGmissions
    if (g_activemission->supergroupDbid && (g_activemission->ownerDbid == g_activemission->supergroupDbid))
        MissionChangeOwner(ent->db_id, &g_activemission->sahandle);

    LOG_ENT_OLD(ent, "Mission:Character:Enter %s %s, Level:%d XPLevel:%d XP:%d HP:%.2f End:%.2f Debt: %d Inf:%d AccessLvl: %d WisLvl: %d Wis: %d",
        ent->pchar->porigin->pchName,
        ent->pchar->pclass->pchName,
        ent->pchar->iLevel+1,
        character_CalcExperienceLevel(ent->pchar)+1,
        ent->pchar->iExperiencePoints,
        ent->pchar->attrCur.fHitPoints,
        ent->pchar->attrCur.fEndurance,
        ent->pchar->iExperienceDebt,
        ent->pchar->iInfluencePoints,
        ent->access_level,
        ent->pchar->iWisdomPoints,
        ent->pchar->iWisdomLevel);

    if (g_activemission->currentServerMusicName)
    {
        servePlaySound(ent, g_activemission->currentServerMusicName, SOUND_MUSIC, g_activemission->currentServerMusicVolume);
    }
}

static int miapr_highticks = 0;
static int miapr_success = 0;

// find the highest tick number
static int highTicks(StashElement el)
{
    int ticks = (int)(intptr_t)stashElementGetPointer(el);
    if (ticks > miapr_highticks) 
        miapr_highticks = ticks;
    return 1;
}

// used by MissionRewardActivePlayers
static int eachActivePlayer(StashElement el)
{
    int db_id = 0;
    int ticks = 0;
    F32 percent;

    db_id = stashElementGetIntKey(el);
    if (!db_id) 
        return 1;

    ticks = (int)(intptr_t)stashElementGetPointer(el);
    percent = 100.0 * (F32)ticks / (F32)miapr_highticks;
    if (percent >= MISSION_HELPER_REQUIREMENT || g_activemission->taskforceId)
    {
        Entity* teammate = entFromDbId(db_id);
        const StoryReward *reward = NULL, *reward2 = NULL;
        const ContactDef* contactdef = ContactDefinition(g_activemission->contact);
        int secondary = 1;
        if (miapr_success)
        {
            if (eaSize(&g_activemission->task->taskSuccess))
            {
                reward = g_activemission->task->taskSuccess[0];
            }

            if (TaskForceIsOnFlashback(teammate))
            {
                if (eaSize(&g_activemission->task->taskSuccessFlashback))
                {
                    reward2 = g_activemission->task->taskSuccessFlashback[0];
                }
            }
            else
            {
                if (eaSize(&g_activemission->task->taskSuccessNoFlashback))
                {
                    reward2 = g_activemission->task->taskSuccessNoFlashback[0];
                }
            }
        }
        else
        {
            if (eaSize(&g_activemission->task->taskFailure))
            {
                reward = g_activemission->task->taskFailure[0];
            }
        }

        // Complete Team Complete-able tasks for the rest of the team, only on success
        // LH: Remove villain restriction when teamcomplete goes to CoH
        // VJD: Removed to enable teamcomplete for Heroes
//        if (g_activemission->missionAllyGroup == ENC_FOR_VILLAIN)
        {
            if (TASK_IS_TEAMCOMPLETE(g_activemission->task) && db_id != g_activemission->ownerDbid && miapr_success && !g_activemission->taskforceId && !g_activemission->supergroupDbid)
            {
                if (teammate)
                {
                    StoryInfo* teammateinfo = StoryArcLockInfo(teammate);
                    StoryTaskInfo* teammatetask = StoryArcGetAssignedTask(teammateinfo, &g_activemission->sahandle);
                    if (teammatetask && TASK_INPROGRESS(teammatetask->state))
                        secondary = TaskPromptTeamComplete(teammate, teammateinfo, teammatetask, g_activemission->difficulty.levelAdjust, g_activemission->missionLevel);
                    StoryArcUnlockInfo(teammate);
                }
                else
                {
                    char buf[100];
                    assertmsg(g_activemission->sahandle.context, "JP: This is crashing static maps on the other side. The debug info is better on this side, and it's also preferable to crash in an instance.");
                    sprintf(buf, "taskteamcompleteinternal %i:%i:%i:%i:%i:%i:%i:%i", db_id, SAHANDLE_ARGSPOS(g_activemission->sahandle), g_activemission->difficulty.levelAdjust, g_activemission->missionLevel, g_activemission->seed);
                    serverParseClient(buf, NULL);
                    secondary = 0; // Secondary reward will be handled later through relay
                }
            }
        }

        // owner gets rewarded by his task code, except for non-flashback taskforce missions
        if (db_id != g_activemission->ownerDbid || g_activemission->taskforceId || g_activemission->supergroupDbid)
        {
            if ((teammate && !TaskForceIsOnFlashback(teammate)) && secondary && reward && eaSize(&reward->secondaryReward))
            {
                MissionGrantSecondaryReward(db_id, reward->secondaryReward, g_activemission->missionGroup, g_activemission->baseLevel, MissionGetLevelAdjust(), &g_activemission->vars);
            }
            if ((teammate && !TaskForceIsOnFlashback(teammate)) && secondary && reward2 && eaSize(&reward2->secondaryReward))
            {
                MissionGrantSecondaryReward(db_id, reward2->secondaryReward, g_activemission->missionGroup, g_activemission->baseLevel, MissionGetLevelAdjust(), &g_activemission->vars);
            }
            if (contactdef && CONTACT_IS_NEWSPAPER(contactdef) && (reward && reward->contactPoints) || (reward2 && reward2->contactPoints))
            {
                char buf[100];
                ContactHandle newsHandle = newspaperBrokerHandle;
                if (!newsHandle && teammate)
                {
                    StoryInfo* teammateinfo = StoryInfoFromPlayer(teammate);
                    StoryContactInfo* newspaper = ContactGetInfo(teammateinfo, ContactGetNewspaper(teammate));
                    if (newspaper)
                        newsHandle = newspaper->brokerHandle;
                }
                if (newsHandle)
                {
                    sprintf(buf, "newspaperteamcompleteinternal %i %i %i", db_id, newsHandle, (reward ? reward->contactPoints : 0 + reward2 ? reward2->contactPoints : 0));
                    serverParseClient(buf, NULL);
                }
            }
        }

        // everyone on the task force gets a souvenir clue if they participated
        if ((g_activemission->supergroupDbid || g_activemission->taskforceId) && reward && reward->souvenirClues)
        {
            LOG_DBID( db_id, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Story]:Rcv:Clue %s", reward->souvenirClues[0]);
            scApplyToDbId(db_id, reward->souvenirClues);
        }
        if ((g_activemission->supergroupDbid || g_activemission->taskforceId) && reward2 && reward2->souvenirClues)
        {
            LOG_DBID( db_id, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Story]:Rcv:Clue %s", reward2->souvenirClues[0]);
            scApplyToDbId(db_id, reward2->souvenirClues);
        }

        // add the stats
        if (teammate) // otherwise it will be relayed
        {
            const char *name = g_activemission->task->logicalname ? g_activemission->task->logicalname : g_activemission->task->filename;
            badge_RecordMissionComplete(teammate, name, g_activemission->missionGroup );
        }
    }
    else
    {
        Entity *e = entFromDbId(db_id);
        LOG_DBID( db_id, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Mission]:Fail: (got %i of %i ticks)", ticks, miapr_highticks);
        chatSendToPlayer(db_id, localizedPrintf(e,"NoRewardNotOnMissionLongEnough"), INFO_USER_ERROR, 0);
    }
    return 1;
}

static void s_MissionRewardActivePlayersStat(int *sgIdsInMission) 
{
    if( g_activemission && g_activemission->task && sgIdsInMission )
    {
        int i;
        char *stat = NULL;
        const char *nameVgrp = villainGroupGetName(g_activemission->missionGroup);
        const char *missionName = g_activemission->task->logicalname ? g_activemission->task->logicalname : g_activemission->task->filename;
        // for each supergroup, record the stat
        for( i = eaiSize( &sgIdsInMission ) - 1; i >= 0; --i)
        {
            int sg_id = sgIdsInMission[i];
            sgrpstats_StatAdj( sg_id, "sg.group.missions", 1);
            
            // missions.name
            if( missionName )
            {
                estrPrintf(&stat,"sg.group.missions.%s", missionName);
                sgrpstats_StatAdj(sg_id, stat, 1);
            }
            
            // missions.villaingroup
            if( nameVgrp )
            {
                estrPrintf(&stat,"sg.group.missions.%s", nameVgrp);
                sgrpstats_StatAdj(sg_id, stat, 1);
            }
        }
        
        estrDestroy(&stat);
    }
}



// iterate through each player who was on for 60% of time (and not leader) and issue rewards
void MissionRewardActivePlayers(int success)
{
    StashTableIterator hi;
    StashElement he = NULL;
    int *sgIdsInMission = NULL;
    
    if (!player_counts) 
        return;
    if (!g_activemission) 
        return;
    miapr_success = success;

    miapr_highticks = 0;
    stashForEachElement(player_counts, highTicks);
    if (!miapr_highticks) 
        return;

    // ----------
    // award each player and supergroups
    for(stashGetIterator(player_counts, &hi);stashGetNextElement(&hi, &he);)
    {
        int db_id = stashElementGetIntKey(he);

        // normal processing
        eachActivePlayer(he);

        if (db_id)
        {
            Entity* e = entFromDbId(db_id);
            if( e && e->supergroup_id > 0 && eaiFind( &sgIdsInMission, e->supergroup_id ) < 0 ) 
            {
                eaiPush(&sgIdsInMission, e->supergroup_id);
            }
        }
    }

    s_MissionRewardActivePlayersStat( sgIdsInMission );
    eaiDestroy(&sgIdsInMission);
}

// Returns 1 if the player contributed enough to allow his task to be completed
int MissionEarnedTeamComplete(Entity *player)
{
    int ticks = 0;
    F32 percent;

    // Get the high ticks
    if (!player_counts) 
        return 0;
    if (!g_activemission) 
        return 0;
    miapr_highticks = 0;
    stashForEachElement(player_counts, highTicks);
    if (!miapr_highticks) 
        return 0;

    // Lookup the players ticks
    stashIntFindInt(player_counts, player->db_id, &ticks);

    // reward appropriately
    percent = 100.0 * (F32)ticks / (F32)miapr_highticks;
    return ((percent >= MISSION_HELPER_REQUIREMENT) || g_activemission->taskforceId)?1:0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 

static const char *s_rewardTable;

// used by MissionRewardActivePlayersSpecific
static int eachActivePlayerSpecific(StashElement el)
{
    int db_id = 0;
    int ticks = 0;
    F32 percent;

    db_id = stashElementGetIntKey(el);
    if (!db_id) 
        return 1;

    ticks = (int)(intptr_t)stashElementGetPointer(el);
    percent = 100.0 * (F32)ticks / (F32)miapr_highticks;
    if (percent >= MISSION_HELPER_REQUIREMENT || g_activemission->taskforceId)
    {
        if (s_rewardTable)
        {
            LOG_DBID( db_id, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Mission]:Rcv:Reward %s", ScriptVarsTableLookup(&g_activemission->vars, s_rewardTable));
            rewardStoryApplyToDbID(db_id, (const char**)eaFromPointerUnsafe(s_rewardTable), g_activemission->missionGroup, g_activemission->baseLevel, MissionGetLevelAdjust(), &g_activemission->vars, REWARDSOURCE_MISSION);
        }
    }
    else
    {
        Entity *e = entFromDbId(db_id);
        LOG_DBID( db_id, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Mission]:Fail: (got %i of %i ticks)", 
            dbPlayerNameFromId(db_id), ticks, miapr_highticks);
        if (e)
            chatSendToPlayer(db_id, localizedPrintf(e,"NoRewardNotOnMissionLongEnough"), INFO_USER_ERROR, 0);
    }
    return 1;
}

// iterate through each player who was on for 60% of time and issue reward
void MissionRewardActivePlayersSpecific(const char *reward)
{
    if (!player_counts) 
        return;
    if (!g_activemission) 
        return;
    if (!reward) 
        return;

    s_rewardTable = reward;

    miapr_highticks = 0;
    stashForEachElement(player_counts, highTicks);
    if (!miapr_highticks) 
        return;

    stashForEachElement(player_counts, eachActivePlayerSpecific);
}

// *********************************************************************************
//  Kicking players system
// *********************************************************************************

#define MISSION_TEAMKICK_TIME    10    // seconds until player kicked from team is transferred from map

// keep track of players to be kicked off the map
typedef struct PlayerKickList {
    EntityRef player;
    U32 kicktime;
} PlayerKickList;
PlayerKickList** g_playerkicklist = NULL;

// kick a player off the mission map
void MissionKickPlayer(Entity* player)
{
    PlayerKickList* rec;

    if (!OnMissionMap()) 
        return; // I don't care

    // set a 30 second timer for player
    rec = calloc(sizeof(PlayerKickList),1);
    rec->player = erGetRef(player);
    rec->kicktime = dbSecondsSince2000() + MISSION_TEAMKICK_TIME;
    eaPush(&g_playerkicklist, rec);

    // inform player of this timer
    START_PACKET(pak, player, SERVER_MISSION_KICKTIMER)
        pktSendBits(pak, 32, rec->kicktime);
    END_PACKET
        log_printf("missiondoor.log", "Kicking player %i from mission %i", player->db_id, db_state.map_id);
}

// check for players to kick from server
void MissionCheckKickedPlayers()
{
    int i, n;
    U32 time;

    if (!OnMissionMap()) 
        return;
    time = dbSecondsSince2000();
    n = eaSize(&g_playerkicklist);
    for (i = n-1; i >= 0; i--)
    {
        PlayerKickList* rec = g_playerkicklist[i];
        if (rec->kicktime <= time)
        {
            Entity* player = erGetEnt(rec->player);
            if (player)
                leaveMission(player, player->static_map_id, 1);
            eaRemove(&g_playerkicklist, i);
            free(rec);
        }
    }
}

// cancel the kick timer
void MissionUnkickPlayer(Entity* player)
{
    int i, n;

    // find the player on the kick list
    if (!player) 
        return;
    n = eaSize(&g_playerkicklist);
    for (i = 0; i < n; i++)
    {
        Entity* listplayer = erGetEnt(g_playerkicklist[i]->player);
        if (listplayer == player)
        {
            PlayerKickList* rec = (PlayerKickList*)eaRemove(&g_playerkicklist, i);
            free(rec);
            START_PACKET(pak, player, SERVER_MISSION_KICKTIMER)
                pktSendBits(pak, 32, 0);
            END_PACKET
                log_printf("missiondoor.log", "Stopped kicking player %i from mission %i", player->db_id, db_state.map_id);
            return;
        }
    }
    // otherwise, don't worry about it
}

// check when teamup changes -
//    if this team is attached to mission, and any members are on kick list, cancel the kick
void MissionCheckForKickedPlayers(Entity* player)
{
    int i;

    if (!OnMissionMap()) 
        return;
    if (!player->teamup_id || !(player->teamup->map_id == db_state.map_id)) 
        return; // have to attached to this mission

    // cycle through team members
    for (i = 0; i < player->teamup->members.count; i++)
    {
        Entity* ent = entFromDbId(player->teamup->members.ids[i]);
        MissionUnkickPlayer(ent); // doesn't matter if they aren't actually on kick list
    }
}

// if players leave a task force on a mission, they are immediately kicked
// and disqualified from rewards
void MissionPlayerLeftTaskForce(Entity* player)
{
    if (!OnMissionMap()) 
        return;
    MissionCountPlayerReset(player->db_id);
    if (player->owned && player->pl->desiredTeamNumber == -2) 
        leaveMission(player, player->static_map_id, 1);
}