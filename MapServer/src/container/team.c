#include "containerbroadcast.h"
#include "containerloadsave.h"
#include "containersupergroup.h"
#include "entity/entity.h"
#include "team.h"
#include "gameComm/svr_chat.h"
#include "cmdparse/cmdserver.h"
#include "svr/svr_player.h"
#include "storyarc/storyarcprivate.h"
#include "entity/teamup.h"
#include <utilitieslib/utils/memlog.h>
#include "dbcomm/staticMapInfo.h"
#include "entity/buddy_server.h"
#include "dbcomm/shardcomm.h"
#include "dbcomm/dbnamecache.h"
#include "gameSys/arenamapserver.h"
#include "entity/Supergroup.h"
#include "entity/SgrpServer.h"
#include "language/langServerUtil.h"
#include "gameSys/raidmapserver.h"
#include "gameSys/arenamap.h"
#include "bases/bases.h"
#include "comm_game.h"
#include "storyarc/taskforce.h"
#include "league.h"
#include "entity/TaskforceParams.h"
#include "TeamReward.h"
#include "mapgroup.h"
#include "player/badges_server.h"
#include "player/badges.h"
#include "entity/character_level.h"
#include "entity/alignment_shift.h"
#include "dbcomm/dbquery.h"
#include "gameSys/turnstile.h"
#include "dbcomm/logcomm.h"
//-----------------------------------------------------------------------------
// Container Code courtesy of Bruce ///////////////////////////////////////////
//-----------------------------------------------------------------------------

int teamGetIdFromEnt(Entity *e,ContainerType team_type)
{
    switch(team_type)
    {
        xcase CONTAINER_TEAMUPS:        return e->teamup_id;
        xcase CONTAINER_SUPERGROUPS:    return e->supergroup_id;
        xcase CONTAINER_TASKFORCES:        return e->taskforce_id;
        xcase CONTAINER_RAIDS:            return e->raid_id;
        xcase CONTAINER_LEVELINGPACTS:    return e->levelingpact_id;
        xcase CONTAINER_LEAGUES:        return e->league_id;
    }
    devassertmsg(0, "unknown group container %d", team_type);
    return 0;
}

TeamMembers *teamMembers(Entity *e,ContainerType team_type)
{
    switch(team_type)
    {
        xcase CONTAINER_TEAMUPS:        return e->teamup ? &e->teamup->members : NULL;
        xcase CONTAINER_SUPERGROUPS:    return e->supergroup ? &e->supergroup->members : NULL;
        xcase CONTAINER_TASKFORCES:        return e->taskforce ? &e->taskforce->members : NULL;
//        xcase CONTAINER_RAIDS:            return e->raid ? &e->raid->members : NULL;
        xcase CONTAINER_LEVELINGPACTS:    return e->levelingpact ? &e->levelingpact->members : NULL;
        xcase CONTAINER_LEAGUES:        return e->league ? &e->league->members : NULL;
    }
    devassertmsg(0, "unknown group container %d", team_type);
    return 0;
}

int teamMaxMembers(ContainerType team_type)
{
    switch(team_type)
    {
        xcase CONTAINER_TEAMUPS:        return MAX_TEAM_MEMBERS;
        xcase CONTAINER_SUPERGROUPS:    return MAX_SUPER_GROUP_MEMBERS;
        xcase CONTAINER_TASKFORCES:        return MAX_TASKFORCE_MEMBERS;
        xcase CONTAINER_RAIDS:            return MAX_RAID_MEMBERS;
        xcase CONTAINER_LEVELINGPACTS:    return MAX_LEVELINGPACT_MEMBERS;
        xcase CONTAINER_LEAGUES:        return MAX_LEAGUE_MEMBERS;
    }
    devassertmsg(0, "unknown group container %d", team_type);
    return 0;
}

// Recount the players on a map, happens when a player joins a team
// the team should be locked at this point
void teamRecount(Entity* e)
{
    // count all players owned by a mission map
    if (OnMissionMap() && e->teamup_id)
    {
        PlayerEnts* players = &player_ents[PLAYER_ENTS_OWNED];
        int i, count = 0;
        for (i=0; i<players->count; i++)
        {
            if (players->ents[i]->teamup_id == e->teamup_id)
                count++;
        }
        e->teamup->map_playersonmap = count;
        printf("%i players in mission\n", e->teamup->map_playersonmap);
    }
}

// use isTeamupMixedClientside in teamup.h for client-side checks
bool areMembersAlignmentMixed(TeamMembers *membersList)
{
    int index;
    int vill_count = 0;
    int hero_count = 0;
    bool retval;

    retval = false;

    if (membersList)
    {
        for (index = 0; index < membersList->count; index++)
        {
            Entity* member = entFromDbId(membersList->ids[index]);

            if (member)
            {
                if (ENT_IS_ON_RED_SIDE(member))
                {
                    vill_count++;
                }
                else
                {
                    hero_count++;
                }
            }
            else
            {
                int type = dbPlayerTypeByLocationFromId(membersList->ids[index]);

                // cached and possibly out of date
                if (type == kPlayerType_Hero)
                {
                    hero_count++;
                }
                else if (type == kPlayerType_Villain)
                {
                    vill_count++;
                }
            }
        }

        if (hero_count && vill_count)
        {
            retval = true;
        }
    }

    return retval;
}

bool areMembersUniverseMixed(TeamMembers *membersList)
{
    int index;
    int prae_count = 0;
    int primal_count = 0;
    bool retval;

    retval = false;

    if (membersList)
    {
        for (index = 0; index < membersList->count; index++)
        {
            Entity* member = entFromDbId(membersList->ids[index]);

            if (member)
            {
                if (ENT_IS_PRAETORIAN(member))
                {
                    prae_count++;
                }
                else
                {
                    primal_count++;
                }
            }
            else
            {
                int prae = dbPraetorianProgressFromId(membersList->ids[index]);

                // cached and possibly out of date
                if (prae == kPraetorianProgress_Tutorial || prae == kPraetorianProgress_Praetoria)
                {
                    prae_count++;
                }
                else
                {
                    primal_count++;
                }
            }
        }

        if (prae_count && primal_count)
        {
            retval = true;
        }
    }

    return retval;
}

bool areAllMembersOnThisMap(TeamMembers *members)
{
    int i, vill_count = 0, hero_count = 0;

    if (!members) 
        return true;

    for (i = 0; i < members->count; i++)
    {
        if (!entFromDbId(members->ids[i]))
            return false;
    }

    return true;
}

bool isTeamMissionCrossAlliance(Teamup *team)
{
    if ( team && team->activetask && team->activetask->def && team->activetask->def->alliance == SA_ALLIANCE_BOTH)
        return true;

    return false;
}

// locks the team and adds the new member, this function should contain
// all calls needed to insure data consistency
int teamAddMember(Entity* e, ContainerType type, int team_id, int invited_by)
{
    TeamMembers *members;

    assert(e);
    assert(!CONTAINER_IS_STATSERVER_OWNED(type));

    members = teamMembers(e, type);

    teamlogPrintf(__FUNCTION__, "adding %s(%i) to %s:%i inviter %i",
        e->name, e->db_id, teamTypeName(type), team_id, invited_by);
    if (teamGetIdFromEnt(e, type)) {
        teamlogPrintf(__FUNCTION__, "%s(%i) has old team %i",
            e->name, e->db_id, teamGetIdFromEnt(e,type));
        return 0; // need to leave old team first
    }

    // add a member. Does a lock
    if (!dbContainerAddDelMembers(type, 1, 1, team_id, 1, &e->db_id, NULL)) {
        teamlogPrintf(__FUNCTION__, "add failed %s(%i) to %s:%i",
            e->name, e->db_id, teamTypeName(type), team_id);
        return 0;
    }

    // backwards way to maintain a max member count, but guarantees lock safety
    members = teamMembers(e, type);
    if (members->count > teamMaxMembers(type) && e->access_level < 2) //allow access level 2+ people to form groups larger than max (for csr purposes)
    {
        teamlogPrintf(__FUNCTION__, "exceeded max, removing %s(%i) from %s:%i",
            e->name, e->db_id, teamTypeName(type), team_id);
        // remove a member. does an update and unlock
        dbContainerAddDelMembers(type, 0, 1, team_id, 1, &e->db_id, NULL);
        return 0;
    }
    if (members->count == 1)
    {
        members->leader = e->db_id;
        if( e->teamup && !e->teamup->team_mentor && (type == CONTAINER_TEAMUPS) )
        {
            e->teamup->team_mentor = e->db_id;
            e->teamup->team_level = character_CalcExperienceLevel(e->pchar);
        }
    }

    // consistency updates
    if (type == CONTAINER_TEAMUPS)
    {
        if (members->count > e->teamup->maximumPlayerCount)
            e->teamup->maximumPlayerCount = members->count;
        if (IsKheldian(e)) 
            e->teamup->kheldianCount++;
        TeamHandleMemberChange(1, e);
        teamRecount(e);
        teamSendMapUpdate(e);    //    This may be redundant if you are in a league. Todo: add a check. if you are in a league, don't need to do teamSendMapUpdate, since everyone in your team is in your league
        leagueSendMapUpdate(e);
        svrSendEntListToDb(&e, 1);

        if (e->taskforce_id)
        {
            // MAK - we reassert leadership every time we add a member,
            //   he temporarily took leadership when he logged in
            TaskForcePatchTeamTask(e, e->teamup->members.leader);
        }
    }
    else if (type == CONTAINER_SUPERGROUPS)
    {
        SupergroupMemberInfo* rank = createSupergroupMemberInfo();
        int i;
        bool found = false;

        for(i = eaSize(&e->supergroup->memberranks)-1;i >= 0; i--)
        {
            if(e->supergroup->memberranks[i]->dbid == e->db_id)
            {
                found = true;
                // Disable autopromote to hidden rank for access level 9 and above (i.e. dev)
                // My thinking here is that the hidden rank is mainly intended for CS.
                e->supergroup->memberranks[i]->rank = e->access_level && e->access_level <= 9 ? NUM_SG_RANKS - 1 : 0;
                break;
            }
        }

        if(!devassertmsg(!found, "Memberrank already existed for entity that was joining"))
        {
            dbLog("sgrpmemberranks", e, "Memberrank already existed for entity that was joining");
            destroySupergroupMemberInfo(rank);
        }
        else // correct case
        {
            rank->dbid = e->db_id;
            // Disable autopromote to hidden rank for access level 9 and above (i.e. dev)
            // My thinking here is that the hidden rank is mainly intended for CS.
            rank->rank = e->access_level && e->access_level <= 9 ? NUM_SG_RANKS - 1 : 0;
            eaPush(&e->supergroup->memberranks, rank);
        }

        checkSgrpMembers(e, "teamAddMember");
    }
    else if (type == CONTAINER_LEAGUES)
    {        
        leagueSendMapUpdate(e);
    }

    e->team_buff_full_update = true; // Make sure his buffs get sent to his new teammates

    teamUpdateUnlock(e, type);
    LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, type==CONTAINER_LEAGUES? "League:AddPlayer"
        : type==CONTAINER_SUPERGROUPS? "Supergroup:AddPlayer"
        : type==CONTAINER_TASKFORCES? "TaskForce:AddPlayer"
        : "Team:AddPlayer %i");

    if (type == CONTAINER_TEAMUPS)
    {
        if (e->taskforce_id)
        {
            TaskForceMakeLeader(e, e->teamup->members.leader);
        }
        //Script hook
        ScriptPlayerJoinsTeam(e);
    }
    else if (type == CONTAINER_SUPERGROUPS)
    {
        sgroup_initiate(e);
        e->supergroup_update = true;
        sgroup_UpdateAllMembers(e->supergroup);
    }

    return 1;
}

int teamDelMember(Entity* e, ContainerType type, int kicked, int kicked_by)
{
    char    *container_str;
    int        team_id = teamGetIdFromEnt(e, type);
    int        old_leader_id = 0;
    int        new_leader_id = 0;
    int        old_members_count = 0;
    int        old_teamup_max_member_count = 0;
    int        quitter_id = e->db_id;

    assert(!CONTAINER_IS_STATSERVER_OWNED(type));

    teamlogPrintf(__FUNCTION__, "removing %s(%i) from %s:%i kicked %i, by %i",
        e->name, e->db_id, teamTypeName(type), team_id, kicked, kicked_by);

    dbLog("Team:Leave", e, "removing %s(%i) from %s:%i kicked %i, by %i",
        e->name, e->db_id, teamTypeName(type), team_id, kicked, kicked_by);

    if (!team_id) 
    {
        teamlogPrintf(__FUNCTION__, "aborting");
        return 0; // ok
    }

    if (type == CONTAINER_TASKFORCES)
    {
        if (TaskForce_PlayerLeaving(e))
            return 1;
    }

    //
    // Locking Team here
    //
    if (!teamLock(e, type)) 
    {
        teamlogPrintf(__FUNCTION__, "failed to get lock %s(%i) %s:%i",    e->name, e->db_id, teamTypeName(type), team_id);
        return 0;
    }

    // consistency updates
    if (type == CONTAINER_TEAMUPS)
    {
        if (IsKheldian(e)) e->teamup->kheldianCount--;

        MissionPlayerLeavingTeam(e);
        TeamHandleMemberChange(0, e);

        // if owner leaves and no map started
        if(    e->teamup->activetask->assignedDbId == e->db_id &&
            !e->teamup->map_id)
            TeamTaskClearNoLock(e->teamup);

        old_teamup_max_member_count = e->teamup->maximumPlayerCount;
    }
    else if (type == CONTAINER_SUPERGROUPS)
    {
        int i;
        bool deleted = false;

        sgroup_SetMode(e, 0, 0);
        e->send_costume = TRUE;

        // I'm not sure about this, it gets allocated in playerVarAlloc, but
        // it appears to be alloced and freed all the time too? --poz
        destroySupergroupStats( e->superstat );
        e->superstat = NULL;

        checkSgrpMembers(e, "teamDelMember");

        for(i = 0; i < eaSize(&e->supergroup->memberranks); i++)
        {
            if(e->supergroup->memberranks[i]->dbid == e->db_id)
            {
                // using eaRemoveFast to keep dbserver from shifting all entries in the table after i
                destroySupergroupMemberInfo((SupergroupMemberInfo*)eaRemoveFast(&e->supergroup->memberranks, i));
                deleted = true;
                break;
            }
        }
        devassertmsg(deleted, "Supergroup member that's leaving had no rank");
        ArenaClearSupergroupRatings(e);

        // Whether this Praetorian granted a bonus to the SG or not, the SG
        // doesn't get to keep it when they leave.
        if (ENT_IS_EX_PRAETORIAN(e))
        {
            int count;
            int found = 0;

            for (count = 0; !found && count < MAX_SUPER_GROUP_MEMBERS; count++)
            {
                if (e->supergroup->praetBonusIds[count] == e->db_id)
                {
                    found = 1;
                }
            }
        }

        e->supergroup_update = true;
    }
    else if (type == CONTAINER_TASKFORCES)
    {
        // preemptively try and change the leader to match what the
        // teamup will do, this covers exemplars that will remove themselves
        // from the task force before leaving the team
        if (e->teamup && e->teamup->members.count >= 2)
        {
            int i, dbid = 0;
            for (i = 0; i < e->teamup->members.count; i++)
            {
                if (e->teamup->members.ids[i] == e->db_id) 
                    continue;
                dbid = e->teamup->members.ids[i];
                break;
            }
            if (dbid) 
                TaskForceMakeLeaderUnlocked(e, dbid);
        }
    }

    // elect a new leader
    if (type == CONTAINER_TEAMUPS)
    {
        TeamMembers* members = teamMembers(e, type);
        // Start with these both set to current leader.  In the event that the quitter is not the leader,
        // this causes the correct messsage to be sent to the turnstile server.
        old_leader_id = members->leader;
        new_leader_id = members->leader;
        if (members->leader == e->db_id && members->count > 1)
        {
            int i;
            for( i = 0; i < members->count; i++ )
            {
                if( members->leader != members->ids[i] )
                {
                    league_updateTeamLeaderTeam(e, members->leader, members->ids[i]);
                    turnstileMapserver_generateGroupUpdate(members->leader, members->ids[i], e->db_id);
                    members->leader = members->ids[i];
                    // Aha - we promoted someone - set up a new leader id
                    new_leader_id = members->leader;
                    old_members_count = members->count;
                    if( !e->teamup->activetask || !e->teamup->activetask->sahandle.context && e->teamup->team_mentor == e->db_id )
                    {
                        Entity *leader = entFromDbId(members->leader);
                        e->teamup->team_mentor = members->leader;
                        if(leader)
                            e->teamup->team_level = character_CalcExperienceLevel(e->pchar);
                    }
                    break;
                }
            }

            TaskForcePatchTeamTask(e, members->leader);
        }
    }
    else if (type == CONTAINER_LEAGUES)
    {
        if (e && e->league)
        {
            //    I lied. we cannot handle league leader promotion
            //    league leader promotion requires the container lock, but we've already locked it by the time we get here
            //    devassert if we're the leader because this case is not very stable and needs to be fixed
            if (e->league->members.count > 1)
            {
                devassert(e->league->members.leader != e->db_id);
            }
            turnstileMapserver_generateGroupUpdate(e->league->members.leader, e->db_id, e->db_id);
        }
    }

    // do update and remove
    if (type == CONTAINER_TEAMUPS)
        container_str = packageTeamup(e);
    else if (type == CONTAINER_SUPERGROUPS)
        container_str = packageSuperGroup(e->supergroup);
    else if (type == CONTAINER_TASKFORCES)
        container_str = packageTaskForce(e);
    else
        return 0;
    if (!dbContainerAddDelMembers(type, 0, 1, team_id, 1, &e->db_id, container_str)) {
        teamlogPrintf(__FUNCTION__, "failed del member %s(%i) %s:%i",
            e->name, e->db_id, teamTypeName(type), team_id);
        free(container_str);
        return 0;
    }
    free(container_str);
    assert(teamGetIdFromEnt(e,type) != team_id); // BER 4-16-04 - if this asserts like crazy, remove it

    // after out of team lock
    if (type == CONTAINER_TEAMUPS)
    {
        // new task force leader was chosen above inside the teamup lock
        TaskForceMakeLeader(e, new_leader_id);
        dbLog("Team:Leader", entFromDbId(new_leader_id), "%s is new leader (previous leader (%s) left)",
            dbPlayerNameFromId(new_leader_id), e->name);
        if (old_members_count > 2) // be silent if new leader will be alone
        {
            sendEntsMsg( type, team_id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(e,"isNewLeader", dbPlayerNameFromId(new_leader_id)) );
        }

        MissionPlayerLeftTeam(e, kicked);
        TaskForceModeChange(e, 0);
        TaskAdvanceCompleted(e);
        MissionSendRefresh(e); // refresh my mission state
        shardCommStatus(e);

        //Script hook
        ScriptPlayerLeavesTeam(e);

        // don't drop the player from the turnstile queue if they weren't ever on a real team
        // teams of one aren't really teams from the player's perspective.
        if (old_teamup_max_member_count > 1)
        {
            // update the turnstile server
            turnstileMapserver_generateGroupUpdate(old_leader_id, new_leader_id, quitter_id);
        }
    }
    else if (type == CONTAINER_SUPERGROUPS)
    {
        // refresh everyone on group
        dbBroadcastMsg(CONTAINER_SUPERGROUPS, &team_id, INFO_SVR_COM, 0, 1, DBMSG_SUPERGROUP_REFRESH);
        sgroup_clearStats(e);
    }
    else if (type == CONTAINER_TASKFORCES)
    {
        TaskForceModeChange(e, 0);
        MissionPlayerLeftTaskForce(e);
    }

    LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, type==CONTAINER_LEAGUES? "League:RemovePlayer"
        : type==CONTAINER_SUPERGROUPS? "Supergroup:RemovePlayer"
        : type==CONTAINER_TASKFORCES? "TaskForce:RemovePlayer"
        : "Team:RemovePlayer");
    return 1;
}

// you must call this before changing any shared team data
int teamLock(Entity *e,ContainerType team_type)
{
    int ret;
    int container = teamGetIdFromEnt(e,team_type);

    assert(!CONTAINER_IS_STATSERVER_OWNED(team_type));

    teamlogPrintf(__FUNCTION__, "%s(%i) %s:%i",
        e->name, e->db_id, teamTypeName(team_type), container);

    if (container <= 0)
        return 0;
    ret = dbSyncContainerRequest(team_type,container,CONTAINER_CMD_LOCK,0);

    if (!ret)
    {
        // Added additional logging for COH-16177.
        // This assert causes a crash.  So, log as much info as possible
        LOG(LOG_ERROR, LOG_LEVEL_ALERT, LOG_CONSOLE|LOG_LOCAL, "teamLock() call to dbSyncContainerRequest() in : last_db_error: %s, entity: %s(%i) team: %s(%i) container: %i\n", last_db_error, e->name, e->db_id, teamTypeName(team_type), team_type, container );
        
        assert(ret && "dbSyncContainerRequest failed in teamLock");
    }

    return ret;
}

// you must always call this if you call teamLock
int teamUpdateUnlock(Entity *e,ContainerType team_type)
{
    int        ret,container = teamGetIdFromEnt(e,team_type);
    char    *container_str;

    assert(!CONTAINER_IS_STATSERVER_OWNED(team_type));

    teamlogPrintf(__FUNCTION__, "%s(%i) %s:%i",
        e->name, e->db_id, teamTypeName(team_type), container);

    if (team_type == CONTAINER_TEAMUPS)
        container_str = packageTeamup(e);
    else if (team_type == CONTAINER_SUPERGROUPS)
        container_str = packageSuperGroup(e->supergroup);
    else if (team_type == CONTAINER_TASKFORCES)
        container_str = packageTaskForce(e);
    else
        return 0;
    ret = dbSyncContainerUpdate(team_type,container,CONTAINER_CMD_UNLOCK,container_str);
    free(container_str);
    return ret;
}

// automatically makes e a member of the group
int teamCreate(Entity *e, ContainerType type)
{
    int    team_id;

    assert(!CONTAINER_IS_STATSERVER_OWNED(type));

    if (teamGetIdFromEnt(e,type))
    {
        teamlogPrintf(__FUNCTION__, "already have team %s(%i) %s:%i",
            e->name, e->db_id, teamTypeName(type), teamGetIdFromEnt(e,type));
        return 0; // you already have one
    }

    teamlogPrintf(__FUNCTION__, "%s(%i) creating %s",
        e->name, e->db_id, teamTypeName(type));
    dbSyncContainerCreate(type,&team_id);    // BER - if this fails, the dbserver is down, so you're screwed anyhow
    teamlogPrintf(__FUNCTION__, "%s(%i) got %s:%i",
        e->name, e->db_id, teamTypeName(type), team_id);

    LOG_ENT( e, LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, type==CONTAINER_LEAGUES ? "League:Create" :
                type==CONTAINER_SUPERGROUPS ? "SuperGroup:Create" :
                type==CONTAINER_TASKFORCES ? "TaskForce:Create" :
                "Team:Create" );

    teamAddMember(e,type,team_id,0);

    return team_id;
}

int teamDestroy(ContainerType type, int team_id, char* reason)
{
    assert(!CONTAINER_IS_STATSERVER_OWNED(type));

    LOG( LOG_ENTITY, LOG_LEVEL_VERBOSE, 0, type==CONTAINER_LEAGUES ? "League:Delete" :
        type==CONTAINER_SUPERGROUPS ? "Supergroup:Delete" : 
        type==CONTAINER_TASKFORCES ? "TaskForce:Delete" : "Team:Delete" );

    teamlogPrintf(__FUNCTION__, "%s:%i", teamTypeName(type), team_id);
    return dbSyncContainerUpdate(type,team_id,CONTAINER_CMD_DELETE,"goodbye!");
}

int teamGetLatest(Entity *e,ContainerType type)
{
    int ret,team_id = teamGetIdFromEnt(e,type);

    if (team_id <= 0)
        return 0;
    ret = dbSyncContainerRequest(type,team_id,CONTAINER_CMD_DONT_LOCK,0);
    return ret;
}

// dbserver says we're not in this team anymore
void teamHandleRemoveMembership(Entity *e,ContainerType type,int containerDeleted)
{
    TeamMembers *members = teamMembers(e,type);

    teamlogPrintf(__FUNCTION__, "clearing %s for %s(%i), containerDeleted %i",
        teamTypeName(type), e->name, e->db_id, containerDeleted);
    if (members)
    {
        destroyTeamMembersContents(members);
    }

    if (type == CONTAINER_TEAMUPS)
    {
        teamInfoDestroy(e);
    }
    else if (type == CONTAINER_SUPERGROUPS)
    {
        destroySupergroup(e->supergroup);
        e->supergroup_id = 0;
        e->supergroup = NULL;
    }
    else if (type == CONTAINER_TASKFORCES)
    {
        if( e->taskforce && e->taskforce->architect_flags && !(e->taskforce->architect_flags&ARCHITECT_TEST) )
        {
            START_PACKET( pak1, e, SERVER_ARCHITECT_COMPLETE );
            pktSendBitsAuto( pak1, e->taskforce->architect_id );
            END_PACKET
        }

        if (e->taskforce)
        {
            destroyTaskForce(e->taskforce);
            e->taskforce = 0;
        }
        e->taskforce_id = 0;
    }
    else if(type == CONTAINER_LEVELINGPACTS)
    {
        // there's probably a small window here where the player could miss out
        // on some xp, but since they're leaving anyway, who cares?
        SAFE_FREE(e->levelingpact);
        e->levelingpact_id = 0;
        e->levelingpact_update = 1;
        if(e->pl)
        {
                MarkModifiedBadges(g_hashBadgeStatUsage, e->pl->aiBadges, "*char");

                //BitFieldSet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, pdef->iIdx, 0);

        }
    }
    else if(type == CONTAINER_LEAGUES)
    {
        leagueInfoDestroy(e);
    }
    else
    {
        devassertmsg(0, "unknown group container %d", type);
    }
}

void teamInfoDestroy(Entity* e)
{
    U32 now = timerSecondsSince2000();
    if (e->teamupTimer_activePlayer >= now || !e->teamupTimer_activePlayer)
    {
        if (e->teamup != e->teamup_activePlayer)
        {
            destroyTeamup(e->teamup_activePlayer);
        }
        e->teamup_activePlayer = NULL;
    }
    // else leave teamup_activePlayer alone until the timer expires, will be cleaned up in character_TickPhaseOne.

    if (e->teamup)
    {
        destroyTeamup(e->teamup);
        e->teamup = NULL;
    }
    e->teamup_id = 0;
    e->teamlist_uptodate = 0;
}

//-----------------------------------------------------------------------------
// Map instancing code - stuff independent of missions or arena
//-----------------------------------------------------------------------------

// if this is successful, you can use DBFLAG_MISSION_ENTER and dbAsyncMapMove to move to map
// TEAM MUST BE LOCKED
int teamStartMap(Entity *e,const char *mapname,const char* mapinfo)
{
    static PerformanceInfo* perfInfo;

    char    buf[500];
    Teamup    *team = e->teamup;
    int        team_id = e->teamup_id;

    teamlogPrintf(__FUNCTION__, "%s(%i) info %s",
        e->name, e->db_id, mapinfo);

    // Don't do anything if the entity is not in a team of the specified type.
    if (!team_id)
        goto fail;
    if (team->map_id)
        goto fail;
    if (!mapname)
        goto fail;

    printf("async starting map server %s linked to team %d\n",mapname,team_id);
    sprintf(buf,"MapName \"%s\"\nMissionInfo \"%s\"\n",mapname,mapinfo);
    dbAsyncContainerUpdate(CONTAINER_MAPS,-1,CONTAINER_CMD_CREATE,buf,team_id);
    if (!dbMessageScanUntil(__FUNCTION__, &perfInfo))
        goto fail;
    LOG_OLD("missiondoor requesting dbserver create map %s, returned %i", mapinfo, db_state.last_id_acked);

    team->map_id = db_state.last_id_acked;
    team->map_type = MAPINSTANCE_MISSION;
    if (!team->map_id)
        goto fail;
    LOG_OLD("missiondoor Teamup %i got instanced map id %i\n", e->teamup_id, team->map_id);
    return 1;
fail:
    LOG_OLD("missiondoor !!Teamup %i failed to create instanced map, map_id %i\n", e->teamup_id, e->teamup? e->teamup->map_id: 0);
    printf("failed to start instanced map!\n");
    teamlogPrintf(__FUNCTION__, "failed %s(%i) info %s",
        e->name, e->db_id, mapinfo);
    return 0;
}

void teamHandleMapStoppedRunning(Entity* e)
{
    teamDetachMap(e);
}

// detach the team from the instanced map it is running
void teamDetachMap(Entity *e)
{
    if (!e->teamup_id) return;
    teamlogPrintf(__FUNCTION__, "%s(%i) team %i", e->name, e->db_id, e->teamup_id);
    if (teamLock(e, CONTAINER_TEAMUPS))
    {
        teamDetachMapUnlocked(e);
        teamUpdateUnlock(e, CONTAINER_TEAMUPS);
    }
}

void teamDetachMapUnlocked(Entity* e)
{
    if (!e->teamup) return;

    dbLog("Team", e, "teamDetachMapUnlocked map_id %d map_type %d", e->teamup->map_id, e->teamup->map_type );

    if (e->teamup->map_type == MAPINSTANCE_MISSION)
        MissionMapDetaching(e);

    e->teamup->map_playersonmap = 0;
    e->teamup->map_id = 0;
    e->teamup->map_type = 0;

}

// update the playersonmap count for the entity's teamup
void teamUpdatePlayerCount(Entity* e)
{
    if (!OnInstancedMap() || isBaseMap()) return;
    if (!e->teamup_id) return;
    if (teamLock(e, CONTAINER_TEAMUPS))
    {
        // count all players owned by this mapserver on this teamup
        PlayerEnts* players = &player_ents[PLAYER_ENTS_OWNED];
        int i, count = 0, totalcount = 0;
        for (i=0; i<players->count; i++)
        {
            totalcount++;
            if (players->ents[i]->teamup_id == e->teamup_id)
                count++;
        }
        e->teamup->map_playersonmap = count;
        teamUpdateUnlock(e, CONTAINER_TEAMUPS);
        printf("%i players in map\n", totalcount); 
        return;
    }
    printf("mission.c - couldn't lock the team!\n");
}


//-----------------------------------------------------------------------------
// Team Logic Code courtesy of CW - needs to be in new file ///////////////////
//-----------------------------------------------------------------------------

int team_IsLeader( Entity* teammate, int db_id  )
{
    if(!teammate || !teammate->teamup_id )
        return FALSE;

    if (teammate->teamup && teammate->teamup->members.leader == db_id) {
        return TRUE;
    }
    return FALSE;
}

int team_CalcMaxSizeFromMembership( Teamup* teamup )
{
    return MAX_TEAM_SIZE;
    // This code calculates the team size based on if there's anyone in
    // Praetoria on the team. Now the team size is 8 for everybody.
#if 0
    int i;
    int retval = MAX_TEAM_SIZE;

    // Teams in Praetoria proper can't grow beyond 4 people.
    for( i = 0; i < teamup->members.count; i++ )
    {
        OnlinePlayerInfo* opi;

        opi = dbGetOnlinePlayerInfo( teamup->members.ids[i] );

        if( opi && opi->team_area == MAP_TEAM_PRAETORIANS )
        {
            retval = MAX_PRAETORIAN_TEAM_SIZE;
            break;
        }
    }

    return retval;
#endif
}

int team_AcceptOffer(Entity *leader, int new_dbid, int addEvenIfNotLeader)
{
    int     team_id;
    char tmp[512];
    int max_size;

    // the person that was inviting has formed a task force in the meantime
    if ( leader->pl->taskforce_mode == TASKFORCE_DEFAULT || leader->pl->inTurnstileQueue ) 
    {
        chatSendToPlayer( leader->db_id, localizedPrintf(leader,"couldNotJoinYourTeam", dbPlayerNameFromId(new_dbid)), INFO_USER_ERROR, 0 );
        chatSendToPlayer( new_dbid, localizedPrintf(leader,"youCouldNotJoinTeam", leader->name), INFO_USER_ERROR, 0 );
        return 0;
    }

    // the person that was inviting, has joined a team they are not the leader of in the meantime
    if (!addEvenIfNotLeader && leader->teamup_id && leader->teamup->members.leader != leader->db_id)
    {
        chatSendToPlayer( leader->db_id, localizedPrintf(leader,"couldNotJoinYourTeam", dbPlayerNameFromId(new_dbid)), INFO_USER_ERROR, 0 );
        chatSendToPlayer( new_dbid, localizedPrintf(leader,"youCouldNotJoinTeam", leader->name), INFO_USER_ERROR, 0 );
        return 0;
    }

    // first check to see if the "leader" already has a team
    if( !leader->teamup_id )
    {
        team_id = teamCreate( leader, CONTAINER_TEAMUPS );

        // now make the leader actually the leader
        if ( teamLock( leader, CONTAINER_TEAMUPS ))
        {
            svrSendEntListToDb(&leader, 1);
            //    any teams created on an endgame raid map need the map_id of the map
            //    this is normally done by the door entry code, but turnstile events
            //    automatically destroy teams, so this has to be done on the map itself
            if (db_state.is_endgame_raid)
                leader->teamup->map_id = db_state.map_id;
            leader->teamup->members.leader = leader->db_id;
            if( !teamUpdateUnlock( leader, CONTAINER_TEAMUPS ))
                printf( "Failed to unlock teamup container, BIG TROUBLE, TELL BRUCE!!");
        }
        else
        {
            printf("Failed to lock teamup container, BIG TROUBLE, TELL BRUCE!!");
            return 0;
        }
        leader->pl->lfg = 0;
    }
    else
    {
        team_id = leader->teamup_id;
        leader->pl->lfg = 0;
    }

    max_size = team_CalcMaxSizeFromMembership(leader->teamup);
    devassert(max_size >= MIN_TEAM_SIZE);

    if(leader->teamup->members.count >= max_size) // verify there is room to add new member
    {
        sendChatMsg( leader, localizedPrintf(leader,"CouldNotActionReason", "InviteString", "TeamFull"), INFO_USER_ERROR, 0 );
        return 0;
    }

    if (new_dbid != leader->db_id)
    {
        // must be done with relay
        sprintf(tmp, "team_accept_relay %i %i %i %i %i", new_dbid, leader->teamup_id, leader->db_id, leader->pl->pvpSwitch, leader->taskforce_id);
        serverParseClient(tmp, NULL);
    }
    return 1;
}

void team_AcceptRelay(Entity* e, int team_id, int invited_by, int taskforce_id )
{
    if (teamAddMember(e, CONTAINER_TEAMUPS, team_id, invited_by))
    {
        // still need to check if this new player can do the currently selected task
        // if not loaded, we'll check when it loads
        if (e->pl->account_inv_loaded && e->teamup && e->teamup->activetask && TeamTaskValidateStoryTask(e, &e->teamup->activetask->sahandle) != TTSE_SUCCESS)
        {
            chatSendToTeamup( e, localizedPrintf( e, "couldNotJoinYourTeam", e->name ), INFO_SVR_COM );
            team_MemberQuit(e);
            return;
        }

        e->teamupTimer_activePlayer = timerSecondsSince2000() + ACTIVE_PLAYER_PROBATION_PERIOD_SECONDS;

        e->pl->lfg = 0;
        chatSendToTeamup( e, localizedPrintf( e, "hasJoinedYourTeam", e->name ), INFO_SVR_COM );

        // Remove the accepting player from any queue they might be in
        turnstileMapserver_generateGroupUpdate(e->db_id, 0, 0);

        // send message if currently on alignment mission but new player cannot earn alignment points
        if (alignmentshift_secondsUntilCanEarnAlignmentPoint(e))
        {
            char command[50];
            sprintf(command, "%s %i %i", "sendMessageIfOnAlignmentMission", invited_by, e->db_id);
            serverParseClient(command, NULL);
        }
    }
    else if (invited_by) // failure, if invited_by == 0, don't print any messages (task force internal)
    {
        // just let the leader and the offered user know something wasn't valid
        chatSendToPlayer( invited_by, localizedPrintf(e,"couldNotJoinYourTeam", e->name  ), INFO_USER_ERROR, 0 );
        sendChatMsg( e, localizedPrintf( e,"youCouldNotJoinTeam", dbPlayerNameFromId(invited_by) ), INFO_USER_ERROR, 0 );
        return;
    }

    //If you are a hero and you get yourself on a team with a villain, and aren't in a special zone, just quit it.
    //You will be seen to join and then quit.  We need to wait this late, because the inviter on the server that allows mixed
    //teams doesn't know whether the invitee's server allows mixed teams, and the invitee doesn't know till after he accepts whether
    //there are other teammates besides the inviter that are illegal for him to team with
    if( !OnArenaMap() )
    {
        if(team_MinorityQuit(e,server_state.allowHeroAndVillainPlayerTypesToTeamUp,server_state.allowPrimalAndPraetoriansToTeamUp))
            return;
    }

    if(e->taskforce_id != taskforce_id && taskforce_id) 
    {
        if( e->taskforce_id )
        {
            if (TaskForceIsOnFlashback(e))
            {
                const StoryArc *arc = e->taskforce->storyInfo->storyArcs[0]->def;
                StoryReward **reward = arc->rewardFlashbackLeft;

                if (reward)
                    StoryRewardApply(*reward, e, e->taskforce->storyInfo, e->taskforce->storyInfo->tasks[0], e->taskforce->storyInfo->contactInfos[0], NULL, NULL);
            }
            
            TaskForceModeChange(e,0);
        }

        teamAddMember(e, CONTAINER_TASKFORCES, taskforce_id, e->db_id);
        TaskForceModeChange(e, TASKFORCE_ARCHITECT); // Only Architect TF makes sense here (for now) 
        TaskForceLogCharacter(e);
        e->tf_params_update = 1;

        if (e->taskforce_id && e->taskforce && e->pl->taskforce_mode == TASKFORCE_ARCHITECT)
        {
            LOG_ENT(e, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Taskforce:Architect Player added to taskforce. Taskforce ID: %d Arc ID: %d", e->taskforce_id, e->taskforce->architect_id);
        }

        // Set any player state as a consequence of being on the taskforce
        // with the parameters (eg. turn off enhancements).
        if( e->taskforce )
        {    
            RewardToken *pToken = NULL;

            TaskForceResumeEffects(e);
            TaskForceInitDebuffs(e);

            // reward token
            if ((pToken = getRewardToken(e, "OnArchitect")) == NULL) 
                pToken = giveRewardToken(e, "OnArchitect");
            if (pToken)
            {
                pToken->val = 1;
                pToken->timer = timerSecondsSince2000();
            }
        }
    }
}

//
//
void team_KickMember( Entity *leader, int kickedID, char *player_name )
{
    char tmp[512];
    char name[128];
    Entity *kicked = entFromDbId( kickedID );

    if (kickedID <= 0)
        return;

    // get the name of the kicked person
    name[0] = 0;
    if( kicked )
        strcpy( name, kicked->name );
    else
        strcpy( name, player_name );

    // first make sure they are on the team
    // The second clause is because if the person just joined another team and quit this one,
    // the leader's teamup information might not be up to date, so we'll add this extra
    // check (which only works if they're both on the same map), additionally this will be
    // checked in team_kick_relay.
    if( !team_IsMember( leader, kickedID) || (kicked && (kicked->teamup_id != leader->teamup_id)))
    {
        sendChatMsg( leader, localizedPrintf(leader, "playerNotOnTeam", name), INFO_USER_ERROR, 0 );
        return;
    }

    // verify this person is the leader and that the leader is not trying to kick himself
    if( leader->db_id != leader->teamup->members.leader )
    {
        sendChatMsg( leader, localizedPrintf(leader,"YouAreNotTeamLeader"), INFO_USER_ERROR, 0 );
        return;
    }

    if( leader == kicked )
    {
        sendChatMsg( leader, localizedPrintf(leader,"TeamLeaderCannontKickSelf"), INFO_USER_ERROR, 0 );
        return;
    }

    // must be done with relay
    LOG_ENT(leader, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Team:Kick %s kicked.", name);
    sprintf(tmp, "team_kick_relay %i %i", kickedID, leader->db_id);
    serverParseClient(tmp, NULL);

    //    moved the league kick out of team kick since team kick is also used to swap players within the league
    if (!leader->league_id)
    {
        turnstileMapserver_generateGroupUpdate(leader->db_id, leader->db_id, kickedID);
    }
}

// I was sent a message by another mapserver requesting that I quit this team
void team_KickRelay(Entity* e, int kicked_by)
{
    int team_id = e->teamup_id;

    // Verify that the kicked_by is the leader of the team that we're currently on, in case
    // we've changed teams since this message was sent
    if (!team_IsLeader(e, kicked_by) && !league_IsLeader(e, kicked_by))
    {
        // We're being kicked by someone who isn't our leader!  This should only occur in race conditions
        chatSendToPlayer( kicked_by, localizedPrintf(e,"failedToKick", e->name), INFO_USER_ERROR, 0 );
    }
    else if (teamDelMember(e, CONTAINER_TEAMUPS, 1, kicked_by))
    {
        sendEntsMsg( CONTAINER_TEAMUPS, team_id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(e,"wasKickedFromTeam", e->name) );
        chatSendToPlayer( e->db_id, localizedPrintf(e, "youWereKickedFromTeam"), INFO_SVR_COM, 0 );
    }
    else
        chatSendToPlayer( kicked_by, localizedPrintf(e,"failedToKick", e->name ), INFO_USER_ERROR, 0 );
}

//
//
void team_MemberQuit( Entity *quitter )
{
    int team_id;
//    int old_leader_id;
    Teamup *teamup;
//    TeamMembers *members;

    assert(quitter);
    team_id = quitter->teamup_id;

    // client sometimes uses 'team_quit' command to mean 'leave my task force'  
    if( quitter->pl && quitter->pl->taskforce_mode && quitter->owned )
    {
        if (TaskForceIsOnFlashback(quitter) && quitter->taskforce)
        {
            const StoryArc *arc = quitter->taskforce->storyInfo->storyArcs[0]->def;
            StoryReward **reward = arc->rewardFlashbackLeft;

            if (reward)
                StoryRewardApply(*reward, quitter, quitter->taskforce->storyInfo, quitter->taskforce->storyInfo->tasks[0], quitter->taskforce->storyInfo->contactInfos[0], NULL, NULL);
        }

        TaskForceModeChange(quitter, 0);
    }

    teamup = quitter->teamup;
    if( teamup )
    {
//        members = teamup->members;
//        old_leader_id = members.leader;
        // now remove them
        LOG_ENT(quitter, LOG_ENTITY, LOG_LEVEL_IMPORTANT, 0, "Team:Quit");

        if( teamDelMember( quitter, CONTAINER_TEAMUPS, 0, 0 ))
        {
            sendEntsMsg(CONTAINER_TEAMUPS, team_id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(quitter,"hasQuitTeam", quitter->name));
            sendChatMsg( quitter, localizedPrintf(quitter,"youHaveQuitTeam"), INFO_SVR_COM, 0 );
            quitter->teamupTimer_activePlayer = timerSecondsSince2000() + ACTIVE_PLAYER_PROBATION_PERIOD_SECONDS;
//            turnstileMapserver_generateGroupUpdate(old_leader_id, members.leader, quitter->db_id);
        }
        else
            sendChatMsg( quitter, localizedPrintf(quitter,"UnableToQuitTeam" ), INFO_USER_ERROR, 0 );
    }
}

static int common_MinorityQuit(Entity * e, TeamMembers *members, bool mixedFactions, bool mixedPraetorians)
{
    int i, vill_count = 0, hero_count = 0, primal_count = 0, praet_count = 0;

    for( i = 0; i < members->count; i++ )
    {
        Entity * teamMember = entFromDbId(members->ids[i]);
        if( teamMember )
        {
            if (ENT_IS_ON_RED_SIDE(teamMember))
                vill_count++;
            else
                hero_count++;

            if (ENT_IS_IN_PRAETORIA(teamMember))
                praet_count++;
            else
                primal_count++;
        }
        else
        {    // cached and possibly out of date
            if( dbPlayerTypeByLocationFromId(members->ids[i]) == kPlayerType_Hero )
                hero_count++;
            if( dbPlayerTypeByLocationFromId(members->ids[i]) == kPlayerType_Villain )
                vill_count++;
            if( dbPraetorianProgressFromId(members->ids[i]) == kPraetorianProgress_Tutorial ||
                dbPraetorianProgressFromId(members->ids[i]) == kPraetorianProgress_Praetoria )
                praet_count++;
            else
                primal_count++;
        }
    }

    if( praet_count && primal_count && !mixedPraetorians )
    {
        return MINORITYQUIT_UNIVERSE;
    }
    if( hero_count && vill_count && !mixedFactions && OnPvPMap() )
    {
        return MINORITYQUIT_FACTION;
    }
    return MINORITYQUIT_NONE;
}

int team_MinorityQuit(Entity * e, bool mixedFactions, bool mixedPraetorians)
{
    if (e->teamup)
    {
        int retVal = common_MinorityQuit(e, &e->teamup->members, mixedFactions, mixedPraetorians);
        if (retVal != MINORITYQUIT_NONE)
        {
            quitLeagueAndTeam(e, 1);
        }
        return retVal;
    }
    return MINORITYQUIT_NONE;
}

int league_MinorityQuit(Entity * e, bool mixedFactions, bool mixedPraetorians)
{
    if (e->league)
    {
        int retVal = common_MinorityQuit(e, &e->league->members, mixedFactions, mixedPraetorians);
        if (retVal != MINORITYQUIT_NONE)
        {
            quitLeagueAndTeam(e, 1);
        }
        return retVal;
    }
    return MINORITYQUIT_NONE;
}

void team_Lfg( Entity *e )
{
    if(e->pl->lfg && !(e->pl->lfg & LFG_NOGROUP))
        e->pl->lfg = 0;
    else
        e->pl->lfg = LFG_ANY;
    if( e->pl->lfg && e->teamup && e->teamup->members.count > 1 )
    {
        chatSendToPlayer( e->db_id, localizedPrintf( e, "CantLFGonTeam"), INFO_USER_ERROR, 0 );
        e->pl->lfg = 0;
        svrSendEntListToDb(&e, 1);
        return;
    }
    else
        svrSendEntListToDb(&e, 1);

    if( e->pl->lfg )
        chatSendToPlayer( e->db_id, localizedPrintf( e, "NowLFG"), INFO_SVR_COM, 0 );
    else
        chatSendToPlayer( e->db_id, localizedPrintf( e, "NowNotLFG"), INFO_SVR_COM, 0 );

}

void team_Lfg_Set( Entity *e, int mode )
{
    e->pl->lfg = mode;
    if( e->pl->lfg && !(e->pl->lfg & LFG_NOGROUP) && e->teamup && e->teamup->members.count > 1 )
    {
        chatSendToPlayer( e->db_id, localizedPrintf( e,"CantLFGonTeam"), INFO_USER_ERROR, 0 );
        e->pl->lfg = 0;
        svrSendEntListToDb(&e, 1);
        return;
    }
    else
        svrSendEntListToDb(&e, 1);

    if( e->pl->lfg && !(e->pl->lfg & LFG_NOGROUP) )
        chatSendToPlayer( e->db_id, localizedPrintf( e,"NowLFG"), INFO_USER_ERROR, 0 );
    else
        chatSendToPlayer( e->db_id, localizedPrintf( e,"NowNotLFG"), INFO_USER_ERROR, 0 );
}

void team_changeLeader( Entity *e, char * name )
{
    int newLeaderId;
    int i;

    if(!e || !e->pl)
        return;

    if( name[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "teamLeaderChangeString", "playerNameString", "emptyString", "teamLeaderChangeStringSyn"), INFO_USER_ERROR, 0);
        return;
    }

    newLeaderId = dbPlayerIdFromName(name);

    if( newLeaderId < 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"PlayerDoesNotExist", e->name), INFO_USER_ERROR, 0);
        return;
    }

    if( !e->teamup || !e->teamup_id || e->teamup->members.count <= 1 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotChangeLeaderNoTeam"), INFO_USER_ERROR, 0);
        return;
    }

    if(e->pl->taskforce_mode)
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotChangeTaskForceLeader"), INFO_USER_ERROR, 0);
        return;
    }

    if( e->teamup->members.leader != e->db_id )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotChangeLeaderNotLeader"), INFO_USER_ERROR, 0);
        return;
    }

    if (e->teamup->members.leader == newLeaderId)
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotChangeLeaderIsLeader"), INFO_USER_ERROR, 0);
        return;
    }

    for( i = 0; i < e->teamup->members.count; i++ )
    {
        if( newLeaderId == e->teamup->members.ids[i] )
        {
            if( teamLock( e, CONTAINER_TEAMUPS ) )
            {
                if( !e->teamup->activetask || !e->teamup->activetask->sahandle.context && e->teamup->team_mentor == e->teamup->members.leader )
                {
                    Entity *leader = entFromDbId(newLeaderId);
                    e->teamup->team_mentor = newLeaderId;
                    if(leader)
                        e->teamup->team_level = character_CalcExperienceLevel(e->pchar);
                }
                league_updateTeamLeaderTeam(e, e->teamup->members.leader, newLeaderId);
                e->teamup->members.leader = newLeaderId;
                if (league_IsLeader(e, e->db_id))
                {
                    league_changeLeader(e, name);
                }
                
                teamUpdateUnlock( e, CONTAINER_TEAMUPS );
                chatSendToTeamup( e, localizedPrintf(e,"NewTeamLeader", name), INFO_SVR_COM );
                turnstileMapserver_generateGroupUpdate(e->db_id, newLeaderId, 0);
                return;
            }
        }
    }
    chatSendToPlayer(e->db_id, localizedPrintf(e,"NoLeaderChangeNotAMember", name ), INFO_USER_ERROR, 0);
}

void team_changeLeaderRelay( int oldTeamLeader_dbid, char * newLeaderName)
{
    char tmp[100];
    _snprintf_s(tmp, sizeof(tmp), sizeof(tmp)-1, "makeleader_relay %i %s", oldTeamLeader_dbid, newLeaderName);
    serverParseClient(tmp, NULL);
}

void sendMemberCommon( Packet * pak, TeamMembers * members, int i, int full_update )
{
    Entity *mate;

    // send their db_id
    pktSendBits( pak, 32, members->ids[i]);

    // send whether the are on the mapserver or not, if they are not send their name and map name
    mate = entFromDbId(members->ids[i]);

    if( mate )                            //    teammate is on the same map as client, so the client can handle this in uiGroupWindow.c
        pktSendBits( pak, 1, 1 );
    else
    {
        pktSendBits( pak, 1, 0 );

        if ( ( members->mapIds[i] != members->oldmapIds[i] ) || full_update )
        {
            pktSendBits( pak, 1, 1 );
            pktSendString( pak, members->names[i] );
            pktSendString( pak, getTranslatedMapName(members->mapIds[i]) );
            members->oldmapIds[i] = members->mapIds[i];
        }
        else
        {
            pktSendBits( pak, 1, 0 );
        }
    }

}

static void sendTeammate( Packet * pak, Entity * e, int i, int full_update )
{
    sendMemberCommon( pak, &e->teamup->members, i, full_update );
}
//
//
void team_sendList( Packet * pak, Entity * e, int full_update )
{
    int i;

    // send if there even is a team
    pktSendIfSetBitsPack( pak, PKT_BITS_TO_REP_DB_ID, e->teamup_id );
    pktSendBits( pak, 2, e->pl->taskforce_mode );
    pktSendBits( pak, 10, e->pl->lfg );

    if( !e->teamup_id )
        return;

    // send the leader db_id
    pktSendBitsPack( pak, 32, e->teamup->members.leader);

    // now send the team count
    pktSendBitsPack(pak, 1, e->teamup->members.count);

    pktSendBitsAuto(pak, e->teamup->team_level);
    pktSendBitsAuto(pak, e->teamup->team_mentor);

    // The client doesn't have a second teamup pointer, and only ever needs the other one...
    pktSendBitsAuto(pak, e->teamup_activePlayer ? e->teamup_activePlayer->activePlayerDbid : 0);

    // send mentor first
    for( i = 0; i < e->teamup->members.count; i++ )
    {
        if( e->teamup->team_mentor == e->teamup->members.ids[i] )
            sendTeammate(pak, e, i, full_update);
    }
    
    // now send everyone else
    for( i = 0; i < e->teamup->members.count; i++ )
    {
        if( e->teamup->team_mentor != e->teamup->members.ids[i] )
            sendTeammate(pak, e, i, full_update);
    }
}

void teamUpdateMaplist( Entity * e, char *map_num, int member_id )
{
    if( e->teamup )
    {
        int i;
        for( i = 0; i < e->teamup->members.count; i++ )
        {
            if( member_id == e->teamup->members.ids[i] )
            {
                e->teamup->members.mapIds[i] = atoi(map_num);
                e->teamlist_uptodate = 0;
            }
        }
    }
}

void teamSendMapUpdate( Entity * e )
{
    if(e->teamup_id)
    {
        char buf[256];
        sprintf(buf, "%s%i", DBMSG_MAPLIST_REFRESH, e->map_id );
        dbBroadcastMsg(CONTAINER_TEAMUPS, &e->teamup_id, INFO_SVR_COM, e->db_id, 1, buf);
    }
}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//ALLIANCES ////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------

char *alliance_CheckInviterConditions(Entity *inviter, char *invitee_name)
{
    int i;

    if( !inviter->supergroup_id ) // not in supergroup
        return localizedPrintf(inviter,"AllianceNotInSuperGroup");


    if( !sgroup_hasPermission(inviter, SG_PERM_ALLIANCE) ) // not high enough rank to invite alliance
        return localizedPrintf(inviter,"AllianceNotHighEnoughRank");

    if( strcmp(inviter->name, invitee_name)==0 ) // trying to invite self
        return localizedPrintf(inviter,"CannotAllySelf");

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        if (inviter->supergroup->allies[i].db_id == 0)
            break;
    }

    if (i == MAX_SUPERGROUP_ALLIES)
        return localizedPrintf(inviter,"MaxAlliesReached");

    return 0;
}

char *alliance_CheckInviteeConditions(Entity *invitee, int inviter_dbid, int inviter_sg_dbid, int inviter_playertype)
{
    int i;

    if( invitee->pl->hidden&(1<<HIDE_INVITE) )
        return localizedPrintf(invitee,"playerNotFound", invitee->name);

    if (!invitee->supergroup_id) // target not in a supergroup
        return localizedPrintf(invitee,"TargetNotInSG", invitee->name);

    if (invitee->supergroup_id == inviter_sg_dbid) // target in own supergroup
        return localizedPrintf(invitee,"TargetInYourSG", invitee->name);

    if (invitee->pl->playerType != inviter_playertype)
        return localizedPrintf(invitee,"NoAllianceEnemy");

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        if (invitee->supergroup->allies[i].db_id == inviter_sg_dbid)
            return localizedPrintf(invitee,"AlreadyAllies", invitee->name);
        if (invitee->supergroup->allies[i].db_id == 0)
            break;
    }
    if (i == MAX_SUPERGROUP_ALLIES)
        return localizedPrintf(invitee,"TargetMaxAlliesReached", invitee->name);

    return 0;
}

char *alliance_CheckAccepterConditions(Entity *accepter, int acceptee_dbid, char *acceptee_name)
{
    int i;

    if (accepter->pl->inviter_dbid != acceptee_dbid)
    {
        // TODO: Mark player is suspicious...and take away their candy
        return localizedPrintf(accepter,"youCouldNotJoinAlliance", acceptee_name);
    }

    if(!accepter->supergroup_id) // not in supergroup
        return localizedPrintf(accepter,"AllianceNotInSuperGroup");

    if(!sgroup_hasPermission(accepter, SG_PERM_ALLIANCE)) // not high enough rank to accept alliance
        return localizedPrintf(accepter, "AllianceNotHighEnoughRank");

    if(strcmp(accepter->name, acceptee_name)==0) // trying to accept self
        return localizedPrintf(accepter,"CannotAllySelf");

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        if (accepter->supergroup->allies[i].db_id == 0)
            break;
    }
    if (i == MAX_SUPERGROUP_ALLIES)
        return localizedPrintf(accepter,"MaxAlliesReached");

    return 0;
}

char *alliance_CheckAccepteeConditions(Entity *acceptee, int accepter_dbid, int accepter_sg_dbid, int accepter_playertype)
{
    int i;

    if (!acceptee->supergroup_id) // target not in a supergroup
        return localizedPrintf(acceptee,"TargetNotInSG", acceptee->name);

    if (acceptee->supergroup_id == accepter_sg_dbid) // target in own supergroup
        return localizedPrintf(acceptee,"TargetInYourSG", acceptee->name);

    if (acceptee->pl->playerType != accepter_playertype)
        return localizedPrintf(acceptee,"NoAllianceEnemy");

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        if (acceptee->supergroup->allies[i].db_id == accepter_sg_dbid)
            return localizedPrintf(acceptee,"AlreadyAllies", acceptee->name);
        if (acceptee->supergroup->allies[i].db_id == 0)
            break;
    }
    if (i == MAX_SUPERGROUP_ALLIES)
        return localizedPrintf(acceptee,"TargetMaxAlliesReached", acceptee->name);

    return 0;
}

char *alliance_CheckCancelerConditions(Entity *canceler, int cancelee_sg_dbid)
{
    static char buf[1024] = {0};
    int i;

    if(!canceler->supergroup_id) // not in supergroup
        return localizedPrintf(canceler,"AllianceNotInSuperGroup");

    if(!sgroup_hasPermission(canceler, SG_PERM_ALLIANCE)) // not high enough rank to cancel alliance
        return localizedPrintf(canceler,"AllianceNotHighEnoughRank");

    if(canceler->supergroup_id == cancelee_sg_dbid) // trying to cancel with self
        return localizedPrintf(canceler,"CannotCancelSelf");

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        if (canceler->supergroup->allies[i].db_id == 0)
            i = MAX_SUPERGROUP_ALLIES;
        else if (canceler->supergroup->allies[i].db_id == cancelee_sg_dbid)
            break;
    }
    if (i >= MAX_SUPERGROUP_ALLIES)
        return localizedPrintf(canceler,"NotYourAlly");

    return 0;
}

char *alliance_SetMinTalkRank(Entity *e, int mintalkrank)
{
    if(!e->supergroup_id) // not in supergroup
        return localizedPrintf(e,"CouldNotActionReason", "SetString", "NotInSuperGroup");

    if(!sgroup_hasPermission(e, SG_PERM_ALLIANCE)) // not high enough rank
        return localizedPrintf(e,"CouldNotActionReason", "SetString",  "NotHighEnoughRank");

    if (mintalkrank < 0)
        mintalkrank = 0;
    if (mintalkrank > 2)
        mintalkrank = 2;

    teamLock(e, CONTAINER_SUPERGROUPS);
    e->supergroup->alliance_minTalkRank = mintalkrank;
    teamUpdateUnlock(e, CONTAINER_SUPERGROUPS);

    sgroup_RefreshSGMembers(e->supergroup_id);
    return 0;
}

char *alliance_SetAllyMinTalkRank(Entity *e, int ally_dbid, int mintalkrank)
{
    int i;

    if(!e->supergroup_id) // not in supergroup
        return localizedPrintf(e,"CouldNotActionReason", "SetString", "NotInSuperGroup");

    if(!sgroup_hasPermission(e, SG_PERM_ALLIANCE)) // not high enough rank
        return localizedPrintf(e,"CouldNotActionReason", "SetString",  "NotHighEnoughRank");

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        if (e->supergroup->allies[i].db_id == 0)
            i = MAX_SUPERGROUP_ALLIES;
        else if (e->supergroup->allies[i].db_id == ally_dbid)
            break;
    }
    if (i == MAX_SUPERGROUP_ALLIES)
        return localizedPrintf(e,"NotYourAlly");

    if (mintalkrank < 0)
        mintalkrank = 0;
    if (mintalkrank > 2)
        mintalkrank = 2;

    teamLock(e, CONTAINER_SUPERGROUPS);
    e->supergroup->allies[i].minDisplayRank = mintalkrank;
    teamUpdateUnlock(e, CONTAINER_SUPERGROUPS);

    sgroup_RefreshSGMembers(e->supergroup_id);

    return 0;
}

char *alliance_SetAllyNoSend(Entity *e, int ally_dbid, int nosend)
{
    int i;

    if(!e->supergroup_id) // not in supergroup
        return localizedPrintf(e,"CouldNotActionReason", "SetString", "NotInSuperGroup");

    if(!sgroup_hasPermission(e, SG_PERM_ALLIANCE)) // not high enough rank
        return localizedPrintf(e, "CouldNotActionReason", "SetString",  "NotHighEnoughRank");

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        if (e->supergroup->allies[i].db_id == 0)
            i = MAX_SUPERGROUP_ALLIES;
        else if (e->supergroup->allies[i].db_id == ally_dbid)
            break;
    }
    if (i == MAX_SUPERGROUP_ALLIES)
        return localizedPrintf(e,"NotYourAlly");

    if (nosend < 0)
        nosend = 0;
    if (nosend > 1)
        nosend = 1;

    teamLock(e, CONTAINER_SUPERGROUPS);
    e->supergroup->allies[i].dontTalkTo = nosend;
    teamUpdateUnlock(e, CONTAINER_SUPERGROUPS);

    sgroup_RefreshSGMembers(e->supergroup_id);

    return 0;
}

void alliance_FormAlliance(int ally1_dbid, int ally2_dbid)
{
    int i;
    Supergroup ally1Sg = {0};
    Supergroup ally2Sg = {0};
    bool ally1Locked = false;
    bool ally2Locked = false;

    if (ally1_dbid == ally2_dbid)
    {
        assert(!"Trying to form alliance with self.  Server checks incorrect?");
        return;
    }

    ally1Locked = sgroup_LockAndLoad(ally1_dbid, &ally1Sg);
    ally2Locked = sgroup_LockAndLoad(ally2_dbid, &ally2Sg);

    if (!ally1Locked || !ally2Locked)
    {
        // need to free this if locked.
        if( ally1Locked )
            sgroup_UpdateUnlock( ally1_dbid, &ally1Sg );
        if( ally2Locked )
            sgroup_UpdateUnlock( ally2_dbid, &ally2Sg );

        assert(!"Could not load one of the allies.  Server checks incorrect?");
        return;
    }

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        if (ally1Sg.allies[i].db_id == ally2_dbid)
            break; // already allied

        if (ally1Sg.allies[i].db_id == 0)
        {
            ally1Sg.allies[i].db_id = ally2_dbid;
            ally1Sg.allies[i].dontTalkTo = 0;
            ally1Sg.allies[i].minDisplayRank = 0;
            strncpy(ally1Sg.allies[i].name, ally2Sg.name, SG_NAME_LEN);
            break;
        }
    }

    for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
    {
        if (ally2Sg.allies[i].db_id == ally1_dbid)
            break; // already allied

        if (ally2Sg.allies[i].db_id == 0)
        {
            ally2Sg.allies[i].db_id = ally1_dbid;
            ally2Sg.allies[i].dontTalkTo = 0;
            ally2Sg.allies[i].minDisplayRank = 0;
            strncpy(ally2Sg.allies[i].name, ally1Sg.name, SG_NAME_LEN);
            break;
        }
    }

    sgroup_UpdateUnlock(ally1_dbid, &ally1Sg);
    sgroup_UpdateUnlock(ally2_dbid, &ally2Sg);

    sgroup_RefreshSGMembers(ally1_dbid);
    sgroup_RefreshSGMembers(ally2_dbid);
}

void alliance_NotifyCancel(Entity *ally1, Entity *ally2, int ally1_sgid, int ally2_sgid)
{
    char * buffer;

    if (ally2)
    {
        buffer = localizedPrintf(ally2, "AllianceBrokenWith", ally2->supergroup->name);
        sendEntsMsg(CONTAINER_SUPERGROUPS, ally1_sgid, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, buffer);
    }

    if (ally1)
    {
        buffer = localizedPrintf(ally1, "AllianceBrokenWith", ally1->supergroup->name);
        sendEntsMsg(CONTAINER_SUPERGROUPS, ally2_sgid, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, buffer);
    }
}

void alliance_CancelAlliance(int ally1_sgid, int ally2_sgid)
{
    int i, j;
    Supergroup ally1Sg = {0};
    Supergroup ally2Sg = {0};
    bool ally1Locked = false;
    bool ally2Locked = false;

    if (ally1_sgid == ally2_sgid)
    {
        assert(!"Trying to cancel alliance with self.  Server checks incorrect?");
        return;
    }

    ally1Locked = sgroup_LockAndLoad(ally1_sgid, &ally1Sg);
    ally2Locked = sgroup_LockAndLoad(ally2_sgid, &ally2Sg);

    if ( ally1Locked )
    {
        for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
        {
            if (ally1Sg.allies[i].db_id == 0)
            {
                break;
            }
            else if (ally1Sg.allies[i].db_id == ally2_sgid)
            {
                for (j = i+1; j < MAX_SUPERGROUP_ALLIES; j++)
                {
                    memcpy(&(ally1Sg.allies[j-1]), &(ally1Sg.allies[j]), sizeof(SupergroupAlly));
                }
                ally1Sg.allies[MAX_SUPERGROUP_ALLIES-1].db_id = 0;
                i--;
            }
        }
    }

    if ( ally2Locked )
    {
        for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
        {
            if (ally2Sg.allies[i].db_id == 0)
            {
                break;
            }
            else if (ally2Sg.allies[i].db_id == ally1_sgid)
            {
                for (j = i+1; j < MAX_SUPERGROUP_ALLIES; j++)
                {
                    memcpy(&(ally2Sg.allies[j-1]), &(ally2Sg.allies[j]), sizeof(SupergroupAlly));
                }
                ally2Sg.allies[MAX_SUPERGROUP_ALLIES-1].db_id = 0;
                i--;
            }
        }
    }

    // @todo -AB: who put this here? it could never work. no Ent info is passed :09/30/05
//     alliance_NotifyCancel(ally1, ally2, ally1_sgid, ally2_sgid);

    if (ally1Locked)
    {
        sgroup_UpdateUnlock(ally1_sgid, &ally1Sg);
        sgroup_RefreshSGMembers(ally1_sgid);
    }

    if (ally2Locked)
    {
        sgroup_UpdateUnlock(ally2_sgid, &ally2Sg);
        sgroup_RefreshSGMembers(ally2_sgid);
    }
}

void alliance_CancelDefunctAlliance(int ally1_sgid, int idx)
{
    int j;
    Supergroup ally1Sg = {0};
    bool ally1Locked = false;

    ally1Locked = sgroup_LockAndLoad(ally1_sgid, &ally1Sg);

    if ( ally1Locked )
    {
        for (j = idx + 1; j < MAX_SUPERGROUP_ALLIES; j++)
        {
            memcpy(&(ally1Sg.allies[j - 1]), &(ally1Sg.allies[j]), sizeof(SupergroupAlly));
        }
        ally1Sg.allies[MAX_SUPERGROUP_ALLIES - 1].db_id = 0;
        sgroup_UpdateUnlock(ally1_sgid, &ally1Sg);
        sgroup_RefreshSGMembers(ally1_sgid);
    }
}

void alliance_CancelAllAlliances(int ally1_dbid)
{
    Supergroup ally1Sg = {0};
    Supergroup ally2Sg = {0};
    bool ally1Locked = false;
    bool ally2Locked = false;
    int i, j, k;
    int ally2_dbid;

    ally1Locked = sgroup_LockAndLoad(ally1_dbid, &ally1Sg);

    if (ally1Locked)
    {
        for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
        {
            if (ally1Sg.allies[i].db_id == 0)
            {
                break;
            }
            else if(ally1Sg.allies[i].db_id == ally1_dbid )
            {
                assert(!"can't have alliance with self.");
            }
            else
            {
                ally2_dbid = ally1Sg.allies[i].db_id;
                ally2Locked = sgroup_LockAndLoad(ally2_dbid, &ally2Sg);
                if (ally2Locked)
                {
                    for (j = 0; j < MAX_SUPERGROUP_ALLIES; j++)
                    {
                        if (ally2Sg.allies[j].db_id == 0)
                        {
                            break;
                        }
                        else if (ally2Sg.allies[j].db_id == ally1_dbid)
                        {
                            for (k = j+1; k < MAX_SUPERGROUP_ALLIES; k++)
                                memcpy(&(ally2Sg.allies[k-1]), &(ally2Sg.allies[k]), sizeof(SupergroupAlly));
                            ally2Sg.allies[MAX_SUPERGROUP_ALLIES-1].db_id = 0;
                            // DG - could this become a break; ?
                            j--;
                        }
                    }
                }

                // @todo -AB: who put this here? it could never work. no Ent info is passed :09/30/05
//                 alliance_NotifyCancel(ally1, ally2, ally1_dbid, ally2_dbid);

                if (ally2Locked)
                {
                    sgroup_UpdateUnlock(ally2_dbid, &ally2Sg);
                    sgroup_RefreshSGMembers(ally1Sg.allies[i].db_id);
                }

                ally1Sg.allies[i].db_id = 0;
            }
        }
    }

    if (ally1Locked)
    {
        sgroup_UpdateUnlock(ally1_dbid, &ally1Sg);
        sgroup_RefreshSGMembers(ally1_dbid);
    }

}

//---------------------------------------------------------------------------------------------------------------
// Raids ////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------
char *raid_CheckInviterConditions(Entity *inviter, char *invitee_name)
{
    MapDataToken *pTok = NULL;

    if( !inviter->supergroup_id ) // not in supergroup
        return localizedPrintf(inviter,"RaidNotInSuperGroup");

    if( !sgroup_hasPermission(inviter, SG_PERM_RAID_SCHEDULE) )
        return localizedPrintf(inviter,"RaidNoPermission");

    if( strcmp(inviter->name, invitee_name)==0 ) // trying to invite self
        return localizedPrintf(inviter,"RaidSelf");

    if (!RaidSGCanScheduleInstant(inviter))
        return localizedPrintf(inviter,"RaidNoTimeToInvite");

    if (!mapGroupIsInitalized())
        mapGroupInit("AllStaticMaps");

    pTok = getMapDataToken("pvpDisabled");
//    if (pTok && pTok->val > 0) // all raids are disabled for now
        return localizedPrintf(inviter, "RaidPvPDisabled");  

    return 0;
}

char *raid_CheckInviteeConditions(Entity *invitee, int inviter_dbid, int inviter_sg_dbid)
{
    if( invitee->pl->hidden&(1<<HIDE_INVITE) )
        return localizedPrintf(invitee,"playerNotFound", invitee->name);

    if (!invitee->supergroup_id) // target not in a supergroup
        return localizedPrintf(invitee,"RaidTargetNotInSG");

    if (invitee->supergroup_id == inviter_sg_dbid) // target in own supergroup
        return localizedPrintf(invitee,"RaidTargetInYourSG");

    if (!sgroup_hasPermission(invitee, SG_PERM_RAID_SCHEDULE))
        return localizedPrintf(invitee,"RaidTargetNoPermission");

    if (!RaidSGCanScheduleInstant(invitee))
        return localizedPrintf(invitee,"RaidTargetNoTime");

    return 0;
}

//----------------------------------------------------------------------------------
// in-memory debug log for teamup actions

MemLog g_teamlog = {0};

StaticDefineInt ParseContainerType[] =
{
    DEFINE_INT
    { "Map",                CONTAINER_MAPS },
    { "Launcher",            CONTAINER_LAUNCHERS },
    { "Ent",                CONTAINER_ENTS },
    { "Door",                CONTAINER_DOORS },
    { "Teamup",                CONTAINER_TEAMUPS },
    { "Supergroup",            CONTAINER_SUPERGROUPS },
    { "Attribute",            CONTAINER_ATTRIBUTES },
    { "EMail",                CONTAINER_EMAIL },
    { "Taskforce",            CONTAINER_TASKFORCES },
    { "Petition",            CONTAINER_PETITIONS },
    { "CrashedMap",            CONTAINER_CRASHEDMAPS },
    { "ServerApps",            CONTAINER_SERVERAPPS },
    { "League",                CONTAINER_LEAGUES },
    DEFINE_END
};

const char* teamTypeName(ContainerType type)
{
    static char badid[200];
    const char* result = StaticDefineIntRevLookup(ParseContainerType, type);
    if (!result)
    {
        sprintf(badid, "(Bad type: %i", type);
        result = badid;
    }
    return result;
}

void teamlogPrintf(const char* func, const char* s, ...)
{
    va_list ap;
    char buf[MEMLOG_LINE_WIDTH];
    char buf2[MEMLOG_LINE_WIDTH];

    va_start(ap, s);
    _vsnprintf(buf,MEMLOG_LINE_WIDTH-1,s,ap);
    buf[MEMLOG_LINE_WIDTH-1] = 0;
    va_end(ap);

    _snprintf(buf2,MEMLOG_LINE_WIDTH-1,"%22s : %s",func,buf);
    buf2[MEMLOG_LINE_WIDTH-1] = 0;

    memlog_printf(&g_teamlog, "%s", buf2);
}

// print to the console, strip the thread id stuff
void teamlogEcho(const char* s, ...)
{
    va_list ap;
    char buf[MEMLOG_LINE_WIDTH+10];

    va_start(ap, s);
    if (vsprintf(buf,s,ap) < 10) return;
    va_end(ap);

    //printf("%s\n", buf+9); // HACK - get past memlog thread id
    printf("%s\n", buf);
}

void quitLeagueAndTeam(Entity *e, int voluntaryLeave)
{
    if (e->league_id)    league_MemberQuit(e, voluntaryLeave);
    if (e->teamup_id)    team_MemberQuit(e);
}
