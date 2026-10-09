#include "statgroups.h"
#include "gameData/statgroupstruct.h"
#include "entity/teamCommon.h"
#include <utilitieslib/components/StashTable.h>
#include "dbcomm/dbcontainer.h"
#include "container/dbcontainerpack.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>
#include "comm_backend.h"
#include <utilitieslib/utils/mathutil.h>
#include "statcmd.h"
#include "entity/entVarUpdate.h"
#include "dbcomm/dbcomm.h"
#include "entity/teamup.h"
#include "container/league.h"
#include "statdb.h"
#include <utilitieslib/utils/log.h>

static StashTable s_leagues;
static StashTable s_leagues_byent;
static StashTable s_leagues_byinstance;

static int *s_leagues_tosend;

static void turnstileStatserver_generateGroupUpdate(int oldLeaderDBID, int newLeaderDBID, int quitterDBID)
{
    Packet *pak_out;

    pak_out = pktCreateEx(&db_comm_link, DBCLIENT_GROUP_UPDATE);
    pktSendBitsAuto(pak_out, oldLeaderDBID);
    pktSendBitsAuto(pak_out, newLeaderDBID);
    pktSendBitsAuto(pak_out, quitterDBID);
    pktSend(&pak_out, &db_comm_link);

}
static void s_UpdateLeagueTeam(League *league)
{
    devassert(league);
    if (league)
    {
        int i;
        int oldcount = league->members.count;
        league->members.count = eaiSize(&league->members.ids);
        for (i = 0; i < eaiSize(&league->teamLeaderList); ++i)
        {
            league->teamLeaderIDs[i] = ABS(league->teamLeaderList[i]);    //    the leader list can be negative when the team status is in flux
                                                                        //    we don't send this to the client though
            league->lockStatus[i] = league->teamLockList[i];
        }
        for (i = eaiSize(&league->teamLeaderList); i < MAX_LEAGUE_MEMBERS; ++i)
        {
            league->teamLeaderIDs[i] = 0;
            league->lockStatus[i] = 0;
        }

        league->revision++;
        eaiSortedPushUnique(&s_leagues_tosend, league->container_id);
    }
}
static League* s_AddLeague(int container_id)
{
    devassert(container_id);
    if (container_id)
    {
        League *league = calloc(1, sizeof(*league));
        int addSuccess = 0;
        league->container_id = container_id;
        league->requestSent = 0;
        league->revision = 0;
        league->instance_id = 0;
        addSuccess = stashIntAddPointer(s_leagues, container_id, league, false);
        devassert(addSuccess);
        return league;
    }
    return NULL;
}
static void s_AddLeagueMember(League *league, int joiner_id, int teamLeaderID, int teamLockStatus)
{
    devassert(league);
    devassert(joiner_id);
    devassert(teamLeaderID);
    if (league && joiner_id && teamLeaderID)
    {
        int addSuccess = 0;
        eaiPushUnique(&league->members.ids, joiner_id);
        eaiPush(&league->teamLeaderList, teamLeaderID);
        eaiPush(&league->teamLockList, teamLockStatus);
        s_UpdateLeagueTeam(league);
        addSuccess =stashIntAddPointer(s_leagues_byent, joiner_id, league, false);
        devassert(addSuccess);
    }
}

static void s_RemoveLeagueMember(League *league, int quitter_id)
{
    devassert(league);
    devassert(quitter_id);
    if (league && quitter_id)
    {
        League *cached;
        int idx = eaiFindAndRemove(&league->members.ids, quitter_id);
        int removeSuccess = 0;
        eaiRemove(&league->teamLeaderList, idx);
        eaiRemove(&league->teamLockList, idx);
        s_UpdateLeagueTeam(league);
        removeSuccess = (stashIntRemovePointer(s_leagues_byent, quitter_id, &cached) && cached == league);
        devassert(removeSuccess);
    }
}


static void s_DestroyLeague(League *league)
{
    devassert(league);
    if (league)
    {
        League *cached;
        int removeSuccess =0;
        removeSuccess = (stashIntRemovePointer(s_leagues, league->container_id, &cached) && cached == league);
        if (league->instance_id)
            stashIntRemovePointer(s_leagues_byinstance, league->instance_id, NULL);
        devassert(removeSuccess);
        //make a log of the league you're destroying.
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, "League: Destroying league %d", league->container_id);
        eaiDestroy(&league->teamLeaderList);
        eaiDestroy(&league->members.ids);
        eaiDestroy(&league->teamLockList);
        free(league);
    }
}

static void s_DestroyLeagueAdapter(void* arg0)
{
    s_DestroyLeague((League *)arg0);
}

void stat_LeagueReset(void)
{
    stat_LeagueUpdateTick(); // flush everything to the db
    devassert(!eaiSize(&s_leagues_tosend));

    LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, "League: Resetting all league hash tables on server.");


    if(s_leagues)
        stashTableClearEx(s_leagues, NULL, s_DestroyLeagueAdapter);
    else
        s_leagues = stashTableCreateInt(0);
    if(s_leagues_byent)
        stashTableClear(s_leagues_byent);
    else
        s_leagues_byent = stashTableCreateInt(0);
    if (s_leagues_byinstance)
        stashTableClear(s_leagues_byinstance);
    else
        s_leagues_byinstance =stashTableCreateInt(0);

}

void stat_LeagueDeleteMe(int container_id)
{
    League *league;
    if(!container_id)
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: delete a league with no dbid?\n");
    }
    else if(stashIntFindPointer(s_leagues, container_id, &league))
    {
        int i;
        League *cached;
        for(i = 0; i < eaiSize(&league->members.ids); i++)
        {
            int removeSuccess = (stashIntRemovePointer(s_leagues_byent, league->members.ids[i], &cached) && cached == league);
            devassert(removeSuccess);
        }
        s_DestroyLeague(league);
    }
    else
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: delete requested for unknown league %d\n", container_id);
    }
}

void stat_LeagueUnpack(char *container_data, int container_id, int *members, int member_count)
{
    int i;
    League *league;

    if(!container_id)
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: got a league with no dbid?\n");
        return;
    }

    //    if we do not have this league, add it (this can happen if statserver crashes)
    //    otherwise, the statserver is authoritative on which copy is accurate
    if(stashIntFindPointer(s_leagues, container_id, &league))
    {
          League incoming = {0};
          dbContainerUnpack(league_desc, container_data, &incoming);
          if (incoming.revision >= league->revision)
          {
              devassert(member_count == league->members.count);
              if (member_count != league->members.count)
              {
                  s_UpdateLeagueTeam(league);
              }
          }
    }
    else if (member_count)
    {
        char memberlog[512];
        char *position = memberlog;
        int validLeague = 1;
        League incoming = {0};
        dbContainerUnpack(league_desc, container_data, &incoming);
        devassert(incoming.members.leader);
        if (incoming.members.leader)
        {
            league = s_AddLeague(container_id);
            league->members.leader = incoming.members.leader;
            for(i = 0; i < member_count; i++)
            {
                devassert(members[i]);
                devassert(incoming.teamLeaderIDs[i]);
                devassert((incoming.lockStatus[i] == 1) || (incoming.lockStatus[i] == 0));
                if (members[i] && incoming.teamLeaderIDs[i])
                {
                    s_AddLeagueMember(league, members[i], incoming.teamLeaderIDs[i], incoming.lockStatus[i]); // if members[i] is 0, this'll crash... probably appropriate}
                    position += sprintf(position, "%d", members[i]);
                    if(i < member_count -1)
                        position += sprintf(position, ", ");
                }
                else
                {
                    validLeague = 0;
                    break;
                }
            }
        }
        if (!validLeague)
        {
            LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: Removing invalid league %d\n", container_id);
            stat_LeagueDeleteMe(container_id);
        }

        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, "League: Incoming league %d added to statserver with members {%s}.", container_id, memberlog);
    }
}

void stat_LeagueJoin(int leader_id, int joiner_id, int team_leader_id, char *invitee_name, int teamLockStatus1, int teamLockStatus2)
{
    League *joinerLeague;
    League *league;

    int *members = NULL;    // leader_id is the inviter
    int newLeague = 0;                        // is this a new league?  Assume no.

    //    invalid person joining the league
    if(!leader_id || !joiner_id)
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: invalid League join %i %i\n", leader_id, joiner_id);
        if(leader_id)
            stat_EntLocalizedMessage(leader_id, INFO_USER_ERROR, "LeagueJoinError", NULL);
        if(joiner_id)
        {
            char quitStr[25];
            stat_EntLocalizedMessage(joiner_id, INFO_USER_ERROR, "LeagueJoinError", NULL);
            sprintf(quitStr, "team_quit_relay %i", joiner_id);
            stat_sendToEnt(joiner_id, quitStr);
        }
        return;
    }


    if(stashIntFindPointer(s_leagues_byent, joiner_id, &joinerLeague))
    {
        //    unless we are making ourselves a league of 1
        if (joinerLeague->members.leader != leader_id)
        {
            //    the joiner is already in a league
            //    if it's a league of one, have them quit
            //    otherwise.. they shouldn't be able to get here and we have a problem
            devassert(joinerLeague->members.count <= 1);
            if (joinerLeague->members.count > 1)
            {
                char quitStr[25];
                LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: entity invited to a league who's already a member of a league(%d %d)\n",
                    leader_id, joiner_id);
                stat_EntLocalizedMessage(leader_id, INFO_USER_ERROR, "LeagueAlreadyIn", invitee_name);
                stat_EntLocalizedMessage(joiner_id, INFO_USER_ERROR, "LeagueAlreadyInSelf", NULL);
                sprintf(quitStr, "team_quit_relay %i", joiner_id);
                stat_sendToEnt(joiner_id, quitStr);
                return;
            }
            else
            {
                stat_LeagueQuit(joiner_id, 1, 1);
            }
        }
        else
        {
            //    we are in the proper league
            return;
        }
    }

    //    make sure that their leader is in the league (or is them)
    if (joiner_id != team_leader_id)
    {
        if (leader_id != team_leader_id)
        {
            if(!stashIntFindPointer(s_leagues_byent, team_leader_id, NULL))
            {
                char quitStr[25];
                LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: entity invited to a league who's leader is not a member of a league(%d %d %d)\n",
                    leader_id, joiner_id, team_leader_id);
                stat_EntLocalizedMessage(leader_id, INFO_USER_ERROR, "LeagueInvalidInviter", invitee_name);
                stat_EntLocalizedMessage(joiner_id, INFO_USER_ERROR, "LeagueInvalidInviter", NULL);
                sprintf(quitStr, "team_quit_relay %i", joiner_id);
                stat_sendToEnt(joiner_id, quitStr);
                return;
            }
        }
    }

    if(!stashIntFindPointer(s_leagues_byent, leader_id, &league))
    {
        // new container
        int container_id;
        dbSyncContainerCreate(CONTAINER_LEAGUES, &container_id);
        if(!container_id)
        {
            devassertmsg(0, "Could not create league (%d %d)", leader_id, joiner_id);
        }
        league = s_AddLeague(container_id);

        newLeague = 1;
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, "League: Join created new league %d.", container_id);
    }
    else if (league->members.leader != leader_id)
    {
        char quitStr[25];
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: join requested for league not leader %d (count %d) (%d %d)\n",
            league->container_id, league->members.count, leader_id, joiner_id);
        stat_EntLocalizedMessage(leader_id, INFO_USER_ERROR, "LeagueNotLeader", NULL);
        stat_EntLocalizedMessage(joiner_id, INFO_USER_ERROR, "LeagueNotLeader", NULL);
        sprintf(quitStr, "team_quit_relay %i", joiner_id);
        stat_sendToEnt(joiner_id, quitStr);
        return;
    }

    if(league->members.count + (newLeague ? 2 : 1) > MAX_LEAGUE_MEMBERS)
    {
        char quitStr[25];
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: join requested for full league %d (count %d) (%d %d)\n",
            league->container_id, league->members.count, leader_id, joiner_id);
        stat_EntLocalizedMessage(leader_id, INFO_USER_ERROR, "LeagueTooManyMembers", NULL);
        stat_EntLocalizedMessage(joiner_id, INFO_USER_ERROR, "LeagueTooManyMembers", NULL);
        sprintf(quitStr, "team_quit_relay %i", joiner_id);
        stat_sendToEnt(joiner_id, quitStr);
        return;
    }

    if (joiner_id != team_leader_id)
    {
        int j;
        //    make sure he can fit on the team that he wants
        for (j = 0; j < MAX_LEAGUE_TEAMS; ++j)
        {
            if (league_getTeamLeadId(league, j) == team_leader_id)
            {
                if (league_getTeamMemberCount(league, j) >= MAX_TEAM_MEMBERS)
                {
                    //    if he can't, give him his own team
                    team_leader_id = joiner_id;
                }
                break;
            }
        }
    }

    //    joiner wants his own team
    if (joiner_id == team_leader_id)
    {
        //    if the team count is full
        if (league_teamCount(league) >= MAX_LEAGUE_TEAMS)
        {
            int j;
            int foundTeam = 0;
            for (j = 0; j < MAX_LEAGUE_TEAMS; ++j)
            {
                if (!league_getTeamLockStatus(league, j) && league_getTeamMemberCount(league, j) < MAX_TEAM_MEMBERS)
                {
                    char acceptStr[200];
                    foundTeam = 1;
                    team_leader_id = league_getTeamLeadId(league, j);
                    if (eaiFind(&league->teamLeaderList, -team_leader_id) == -1)
                    {
                        sprintf(acceptStr, "team_accept_offer_relay %i %i %i 0 0", team_leader_id, joiner_id, leader_id);
                        stat_sendToEnt(leader_id, acceptStr);
                        break;
                    }
                    else
                    {
                        //    this means that their team is frozen for now
                        //    skip them
                    }
                }
            }
            if (!foundTeam)
            {
                char quitStr[25];
                LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: join requested for full league %d (team count %d) (%d %d)\n",
                    league->container_id, league_teamCount(league), leader_id, joiner_id);
                stat_EntLocalizedMessage(leader_id, INFO_USER_ERROR, "LeagueTooManyTeams", NULL);
                stat_EntLocalizedMessage(joiner_id, INFO_USER_ERROR, "LeagueTooManyTeams", NULL);
                sprintf(quitStr, "team_quit_relay %i", joiner_id);
                stat_sendToEnt(joiner_id, quitStr);
                return;
            }
        }
    }

    //if this is a new league, add the inviter.  Otherwise, they should already be in the league.
    if(newLeague)
    {
        league->members.leader = league->members.leader = leader_id;
        s_AddLeagueMember(league, leader_id, leader_id, teamLockStatus1);
        eaiPush(&members, leader_id);
    }
    if (!newLeague || (joiner_id != leader_id))
    {
        s_AddLeagueMember(league, joiner_id, team_leader_id, teamLockStatus2);
        eaiPush(&members, joiner_id);
    }

    if (eaiSize(&members))
    {
        char *container_str = NULL;
        // send it off with the add request
        container_str = dbContainerPackage(league_desc, league);
        if(dbContainerAddDelMembers(CONTAINER_LEAGUES, 1, 0, league->container_id, eaiSize(&members), members, container_str))
        {
            int i;
            for (i = 0; i < league->members.count; ++i)
            {
                if (league->members.ids[i] != joiner_id)
                    stat_EntLocalizedMessage(league->members.ids[i], INFO_SVR_COM, "hasJoinedYourLeague", invitee_name);
                else
                    stat_EntLocalizedMessage(joiner_id, INFO_SVR_COM, "youHaveJoinedLeague", NULL);
            }
            for (i = 0; i < eaiSize(&members); ++i)
            {
                char removeBlockStr[200];
                sprintf(removeBlockStr, "league_remove_accept_block %i", members[i]);
                stat_sendToEnt( members[i], removeBlockStr);
            }
        }
        else
        {
            devassertmsg(0, "Could not add member to league %d", league->container_id);
        }
        if (container_str)
            free(container_str);    
    }
    eaiDestroy(&members);
    // if we created a new container, but didn't fill it, it'll get destroyed the next time the db restarts
}

void stat_LeagueJoinTurnstile(int instanceId, int joiner_id, char *invitee_name, int desiredTeam, int isTeamLeader)
{
    League *league;

    if (desiredTeam == -1)        //    doesn't care which team to be in
    {
        desiredTeam = 0;    //    stat league join already handles the case of a team being full and where to go after that
    }
    else if ((desiredTeam < 0) || (desiredTeam > MAX_LEAGUE_TEAMS))
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: desired team %d is invalid\n", desiredTeam);
        stat_EntLocalizedMessage(joiner_id, INFO_USER_ERROR, "LeagueInternalError", NULL);
        return;
    }

    if (!stashIntFindPointer(s_leagues_byinstance, instanceId, &league))
    {
        //    league doesn't exist
        //    for someone who isn't the leader, have them join the league
        //    if team 0, team 0 is always leader team
        stat_LeagueJoin(joiner_id, joiner_id, joiner_id, invitee_name, 0, 0);
        if (!stashIntFindPointer(s_leagues_byent, joiner_id, &league))
        {
            //    make sure we were able to create it
            LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: league for leader %d could not be made\n", joiner_id);
            stat_EntLocalizedMessage(joiner_id, INFO_USER_ERROR, "LeagueInternalError", NULL);
            return;
        }
        else
        {
            char acceptStr[200];
            league->instance_id = instanceId;
            stashIntAddPointer(s_leagues_byinstance, instanceId, league, 0);
            //    map the team to the current team count

            league->turnstileTeamMapping[desiredTeam] = league_teamCount(league);
            sprintf(acceptStr, "team_accept_offer_relay %i %i %i 0 0", joiner_id, joiner_id, joiner_id);
            stat_sendToEnt(joiner_id, acceptStr);
        }
    }
    else
    {
        int leader_id = league->members.leader;    //    in case the leader changed on us
        //    turnstile team mapping is 1 based and 0 is used to mark an unset team
        //    if their team mapping is no longer valid, have them join an empty team
        if (league->turnstileTeamMapping[desiredTeam] && 
            (league->turnstileTeamMapping[desiredTeam] <= league_teamCount(league)))
        {
            int i;
            int teamLeaderId = -1;
            int mappedTeam = league->turnstileTeamMapping[desiredTeam]-1;

            //    add them to the team
            stat_LeagueJoin(leader_id, joiner_id, league_getTeamLeadId(league, mappedTeam), invitee_name, league_getTeamLockStatus(league, 0), league_getTeamLockStatus(league, mappedTeam));
            //    add into proper team
            for (i = 0; i < league->members.count; ++i)
            {
                if (league->members.ids[i] == joiner_id)
                {
                    teamLeaderId = league->teamLeaderIDs[i];
                }
            }
            devassert(teamLeaderId != -1);
            if (teamLeaderId != -1)
            {
                for (i = (league_teamCount(league)-1); i >= 0; --i)
                {
                    if (league_getTeamLeadId(league, i) == teamLeaderId )
                    {
                        char acceptStr[200];
                        // Make sure to not care if this person is still the team leader
                        //   by the time you get to the mapserver. I'm fairly sure odd race
                        //   conditions with the makeleader_relay call below are causing
                        //   players to not get onto their correct teams.
                        sprintf(acceptStr, "team_accept_offer_relay %i %i %i 0 1", teamLeaderId, joiner_id, leader_id);
                        stat_sendToEnt(joiner_id, acceptStr);
                        break;
                    }
                }
            }
            if (isTeamLeader)
            {
                char promoteStr[200];
                sprintf(promoteStr, "makeleader_relay %i %s", teamLeaderId, invitee_name);
                stat_sendToEnt(teamLeaderId, promoteStr);
                stat_LeagueChangeTeamLeaderTeam(league->container_id, league_getTeamLeadId(league, mappedTeam), joiner_id);
            }
        }
        else
        {
            char acceptStr[200];
            stat_LeagueJoin(leader_id, joiner_id, joiner_id, invitee_name, league_getTeamLockStatus(league, 0), 0);
            league->turnstileTeamMapping[desiredTeam] = league_teamCount(league);
            sprintf(acceptStr, "team_accept_offer_relay %i %i %i 0 0", joiner_id, joiner_id, leader_id);
            stat_sendToEnt(joiner_id, acceptStr);

        }
    }
    if (isTeamLeader == 2)        //    see entPlayer.h for description
    {
        if (league->members.leader != joiner_id)
        {
            stat_LeaguePromote(league->members.leader, joiner_id);
        }
    }
}

void stat_LeaguePromote(int old_leader_id, int new_leader_id)
{
    League *league;
    if(!stashIntFindPointer(s_leagues_byent, old_leader_id, &league))
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: promote requested by entity %d who isn't in a league\n", old_leader_id);
        stat_EntLocalizedMessage(old_leader_id, INFO_USER_ERROR, "LeagueNotAMember", NULL);
        return;
    }
    if (league->members.leader != old_leader_id)
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: promote requested by entity %d who isn't league leader\n", old_leader_id);
        stat_EntLocalizedMessage(old_leader_id, INFO_USER_ERROR, "LeagueNotLeader", NULL);
        return;
    }

    if (eaiFind(&league->members.ids, new_leader_id) == -1)
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: promote requested for entity %d who isn't the same league as leader\n", new_leader_id);
        stat_EntLocalizedMessage(new_leader_id, INFO_USER_ERROR, "LeagueNotValidMember", NULL);
        return;
    }
    else
    {
        league->members.leader = new_leader_id;
    }
    s_UpdateLeagueTeam(league);
}

void stat_LeagueChangeTeamLeaderTeam(int league_id, int old_teamLeaderId, int newTeamLeaderId)
{
    League *league;
    int i;
    if(!stashIntFindPointer(s_leagues, league_id, &league))
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: League was not found %d\n", league_id);
        stat_EntLocalizedMessage(old_teamLeaderId, INFO_USER_ERROR, "LeagueNotFound", NULL);
        return;
    }

    if (eaiFind(&league->members.ids, newTeamLeaderId) < 0)
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: player %d not found in league %d\n", newTeamLeaderId, league_id);
        stat_EntLocalizedMessage(newTeamLeaderId, INFO_USER_ERROR, "LeagueNotFound", NULL);
        return;
    }
    
    for (i = 0; i < eaiSize(&league->members.ids); ++i)
    {
        if (ABS(league->teamLeaderList[i]) == old_teamLeaderId)
        {
            league->teamLeaderList[i] = newTeamLeaderId;
        }
    }

    s_UpdateLeagueTeam(league);
}
void stat_LeagueChangeTeamLeaderSolo(int league_id, int db_id, int newTeamLeaderId)
{
    League *league;
    int i;
    if(!stashIntFindPointer(s_leagues, league_id, &league))
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: League was not found %d\n", league_id);
        stat_EntLocalizedMessage(db_id, INFO_USER_ERROR, "LeagueNotFound", NULL);
        return;
    }

    if (eaiFind(&league->members.ids, db_id) < 0)
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS,"Warning: player %d not found in league %d\n", db_id, league_id);
        stat_EntLocalizedMessage(db_id, INFO_USER_ERROR, "LeagueNotFound", NULL);
        return;
    }

    if (eaiFind(&league->members.ids, newTeamLeaderId) < 0)
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: player %d not found in league %d\n", newTeamLeaderId, league_id);
        stat_EntLocalizedMessage(newTeamLeaderId, INFO_USER_ERROR, "LeagueNotFound", NULL);
        return;
    }

    for (i = 0; i < eaiSize(&league->members.ids); ++i)
    {
        if (league->members.ids[i] == db_id)
        {
            league->teamLeaderList[i] = newTeamLeaderId;
            break;
        }
    }

    s_UpdateLeagueTeam(league);
}

void stat_LeagueUpdateTeamLock(int league_id, int team_leader_id, int teamLockStatus)
{
    League *league;
    int i;

    if (!stashIntFindPointer(s_leagues_byent, team_leader_id, &league))
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: league update requested for league %d which doesn't exist\n", league_id);
        stat_EntLocalizedMessage(team_leader_id, INFO_USER_ERROR, "LeagueNotFound", NULL);
        return;
    }

    if (league->container_id != league_id)
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: league update requested for a member %d who isn't in league %d\n", team_leader_id, league_id);
        stat_EntLocalizedMessage(team_leader_id, INFO_USER_ERROR, "LeagueNotAMember", NULL);
        return;
    }

    for (i = 0; i < league->members.count; ++i)
    {
        if (league->teamLeaderIDs[i] == team_leader_id)
        {
            league->teamLockList[i] = teamLockStatus;
        }
    }
    s_UpdateLeagueTeam(league);
}

void stat_LeagueQuit(int quitter_id, int voluntaryLeave, int removeContainer)
{
    League *league = NULL;
    int i;
    LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, 0, "League: Requested quit for member %d.",quitter_id);

    if(!stashIntFindPointer(s_leagues_byent, quitter_id, &league))
    {
        LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: quit requested for entity %d who isn't in a league\n", quitter_id);
        stat_EntLocalizedMessage(quitter_id, INFO_USER_ERROR, "LeagueNotAMember", NULL);
        return;
    }

    if(eaiSortedFindAndRemove(&s_leagues_tosend, league->container_id) >= 0)
    {
        // there was a pending update, flush it
        char *container_str = dbContainerPackage(league_desc, league);
        dbContainerSendList(CONTAINER_LEAGUES, &container_str, &league->container_id, 1, CONTAINER_CMD_CREATE_MODIFY);
        free(container_str);
    }

    if (quitter_id == league->members.leader)
    {
        int old_leader_id = league->members.leader;
        int new_leader_id;
        if (league->members.count > 1)
        {
            int promoteSuccess = 0;
            int passes = 0;
            for (passes = 0; passes < 4 && !promoteSuccess; passes++)
            {
                //    first try to promote someone on my team
                for (i = 0; i < league->members.count; ++i)
                {
                    if (quitter_id != league->members.ids[i])
                    {
                        switch(passes)
                        {
                        case 0:
                            {
                                promoteSuccess = league->teamLeaderIDs[i] == quitter_id;                //    part of the leader's team
                            }break;
                        case 1:
                            {
                                promoteSuccess = league->teamLeaderList[i] == league->members.ids[i];    //    team leader
                            }break;
                        case 2:
                            {
                                promoteSuccess = league->teamLeaderList[i] < 0;                            //    team that is in flux and will send an update as to who is the proper leader
                            }break;
                        case 3:
                            {
                                promoteSuccess = 1;
                            }break;
                        }
                    }
                    if (promoteSuccess)
                    {
                        stat_LeaguePromote(quitter_id, league->members.ids[i]);
                        break;
                    }
                }
            }
            devassert(promoteSuccess);
        }
        new_leader_id = league->members.leader;

        // update the turnstile server
        turnstileStatserver_generateGroupUpdate(old_leader_id, new_leader_id, quitter_id);
    }
    for (i = 0; i < eaiSize(&league->teamLeaderList); ++i)
    {
        if (ABS(league->teamLeaderList[i]) == quitter_id)
        {
            league->teamLeaderList[i] = -quitter_id;        //    when the leader just left, but his team in flux
                                                            //    this will prevent future additions from being added to his team
                                                            //    until the map tells us who the proper leader is
        }
    }
    s_RemoveLeagueMember(league, quitter_id);
    //    notify the turnstile server that a player has left the league
    dbForceRelayCmdToMapByEnt(quitter_id, "turnstile_player_left_league %i", voluntaryLeave ? 1 : 0);

    if (removeContainer)
    {
        char *container_str = NULL;
        if(eaiSortedFindAndRemove(&s_leagues_tosend, league->container_id) >= 0)
        {
            // there was a pending update, flush it
            container_str = dbContainerPackage(league_desc, league);
        }
        if(dbContainerAddDelMembers(CONTAINER_LEAGUES, 0, 0, league->container_id, 1, &quitter_id, container_str))
        {
            char removeBlockStr[200];
            sprintf(removeBlockStr, "league_remove_accept_block %i",quitter_id);
            stat_sendToEnt(quitter_id, removeBlockStr);
        }
        else
        {
            //this can happen if two quit requests are sent before the first one can get resolved by the dbserver.
            LOG( LOG_STATSERVER, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "Warning: Quit operation could not remove member from league %d\n", league->container_id);
        }
        if (container_str)
            free(container_str);
    }    
}
void stat_LeagueUpdateTick()
{
    static U32 s_sendtimer;

    int i;
    char **container_strs = NULL;

    if(!eaiSize(&s_leagues_tosend))
        return;

    if(!s_sendtimer)
        s_sendtimer = timerAlloc();

    timerStart(s_sendtimer);

    for(i = eaiSize(&s_leagues_tosend); i >= 0; --i)
    {
        League *league;
        if(stashIntFindPointer(s_leagues, s_leagues_tosend[i], &league))
        {
            eaPush(&container_strs, dbContainerPackage(league_desc, league));
            league->requestSent++;
        }
        else
        {
            eaiRemove(&s_leagues_tosend, i);
        }
    }
    eaReverse(&container_strs);
    if(eaSize(&container_strs))
        dbContainerSendList(CONTAINER_LEAGUES, container_strs, s_leagues_tosend, eaSize(&container_strs), CONTAINER_CMD_CREATE_MODIFY);

    for(i = eaSize(&container_strs)-1; i >= 0; --i)
        free(container_strs[i]);
    eaiSetSize(&s_leagues_tosend, 0);
    eaDestroy(&container_strs);
}
