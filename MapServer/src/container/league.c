#include "league.h"
#include "entity/entity.h"
#include "entity/teamCommon.h"
#include "gameComm/svr_chat.h"
#include "language/langServerUtil.h"
#include "dbcomm/dbnamecache.h"
#include "team.h"
#include "entity/teamup.h"
#include "svr/svr_player.h"
#include "entity/entPlayer.h"
#include "cmdparse/cmdserver.h"
#include "cmdparse/cmdstatserver.h"
#include "gameComm/sendToClient.h"
#include "dbcomm/logcomm.h"
#include "containerbroadcast.h"
#include "dbcomm/staticMapInfo.h"
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "gameSys/Turnstile.h"
#include "door.h"

//    returns -1 if not found
int league_getTeamNumberFromIndex(Entity *e, int index)
{
    devassert(e);
    if (!e || !e->league)
    {
        return -1;
    }
    else
    {
        int i;
        League *league = e->league;
        int *uniqueTeams = NULL;
        int retVal = -1;
        //    go up to this player, and build a team list
        //    if the teamLeaderId isn't found, add it (this is why we only need to go up to the index)
        for (i = 0; i < MIN((index+1), league->members.count); ++i)
        {
            int j;
            int matchFound = 0;
            for (j = 0; j < eaiSize(&uniqueTeams) && !matchFound; ++j)
            {
                if (league->teamLeaderIDs[i] == uniqueTeams[j])
                {
                    matchFound = 1;
                }
            }
            if (!matchFound)
            {
                eaiPush(&uniqueTeams, league->teamLeaderIDs[i]);
            }
        }

        //    check the list for the players team
        for (i = 0; i < eaiSize(&uniqueTeams); ++i)
        {
            if (uniqueTeams[i] == league->teamLeaderIDs[index])
            {
                retVal = i;
                break;
            }
        }
        eaiDestroy(&uniqueTeams);
        devassert((retVal >= 0) && (retVal < MAX_LEAGUE_TEAMS));
        if ((retVal < 0) || (retVal >= MAX_LEAGUE_TEAMS))
        {
            retVal = -1;
        }
        return retVal;
    }
}

int league_IsLeader( Entity* leaguemate, int db_id  )
{
    if(!leaguemate || !leaguemate->league_id )
        return FALSE;

    if (leaguemate->league && leaguemate->league->members.leader == db_id) {
        return TRUE;
    }
    return FALSE;
}

int league_CalcMaxSizeFromMembership( League* league )
{
    int retval = MAX_LEAGUE_MEMBERS;

    return retval;
}


int league_AcceptOffer( Entity *leader, int new_dbid, int team_leader_id, int teamLockStatusJoiner )
{
    int teamLockStatusLeader = 0;
    if ( leader->pl->inTurnstileQueue ) 
    {
        chatSendToPlayer( leader->db_id, localizedPrintf(leader,"couldNotJoinYourTeam", dbPlayerNameFromId(new_dbid)), INFO_USER_ERROR, 0 );
        chatSendToPlayer( new_dbid, localizedPrintf(leader,"youCouldNotJoinTeam", leader->name), INFO_USER_ERROR, 0 );
        return 0;
    }

    // the person that was inviting, has joined a league they are not the leader of in the meantime
    if( leader->league_id && leader->league->members.leader != leader->db_id )
    {
        chatSendToPlayer( leader->db_id, localizedPrintf(leader,"couldNotJoinYourLeague", dbPlayerNameFromId(new_dbid)), INFO_USER_ERROR, 0 );
        chatSendToPlayer( new_dbid, localizedPrintf(leader,"youCouldNotJoinLeague", leader->name), INFO_USER_ERROR, 0 );
        return 0;
    }

    if (leader->pl && leader->pl->taskforce_mode)
    {
        teamLockStatusLeader = 1;
    }
    else if (leader->teamup && leader->teamup->teamSwapLock)
    {
        teamLockStatusLeader = 1;
    }
    SgrpStat_SendPassthru(leader->db_id,0, "statserver_league_join %d %d %d \"%s\" %d %d",
        leader->db_id, new_dbid, team_leader_id ? team_leader_id : new_dbid, dbPlayerNameFromId(new_dbid), 
        teamLockStatusLeader, teamLockStatusJoiner);

    // Remove the entire membership of the current team from the queue
    turnstileMapserver_generateGroupUpdate(new_dbid, 0, 0);

    return 1;
}

int league_AcceptOfferTurnstile(int instanceID, int db_id, char *invitee_name, int desiredTeam, int isTeamLeader)
{
    SgrpStat_SendPassthru(db_id, 0, "statserver_league_join_turnstile %d %d \"%s\" %d %d", instanceID, db_id, invitee_name, desiredTeam, isTeamLeader);
    return 0;
}

void league_MemberQuit( Entity *quitter, int voluntaryLeave )
{
    int league_id;
//    int old_leader_id;
    League *league;

    assert(quitter);
    league_id = quitter->league_id;

    league = quitter->league;
    if( league )
    {
        SgrpStat_SendPassthru(quitter->db_id,0, "statserver_league_quit %d %i 1", quitter->db_id, voluntaryLeave);
        turnstileMapserver_generateGroupUpdate(league->members.leader, league->members.leader, quitter->db_id);
        quitter->pl->league_accept_blocking_time = dbSecondsSince2000();
    }
    
    if (db_state.is_endgame_raid)
        leaveMission(quitter, quitter->static_map_id, 1);
}

//
//
void league_KickMember( Entity *leader, int kickedID, char *player_name )
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

    // first make sure they are on the league
    // The second clause is because if the person just joined another league and quit this one,
    // the leader's league information might not be up to date, so we'll add this extra
    // check (which only works if they're both on the same map), additionally this will be
    // checked in league_kick_relay.
    if( !league_IsMember( leader, kickedID) || (kicked && (kicked->league_id != leader->league_id)))
    {
        sendChatMsg( leader, localizedPrintf(leader, "playerNotOnLeague", name), INFO_USER_ERROR, 0 );
        return;
    }

    // verify this person is the leader and that the leader is not trying to kick himself
    if( leader->db_id != leader->league->members.leader )
    {
        sendChatMsg( leader, localizedPrintf(leader,"YouAreNotLeagueLeader"), INFO_USER_ERROR, 0 );
        return;
    }

    if( leader == kicked )
    {
        sendChatMsg( leader, localizedPrintf(leader,"LeagueLeaderCannotKickSelf"), INFO_USER_ERROR, 0 );
        return;
    }

    if (!kicked || (kicked && kicked->teamup && kicked->teamup_id))
    {
        //    if there is not ent, server can't tell if the ent is on a team, so error on the safe side and just do a blind kick in case
        //    if the ent is not on a team, the command will get ignored anyways, so its okie to do..
        dbLog("Team:KickFromLeagueLeader", leader, "%s kicked.", name);
        sprintf(tmp, "team_kick_relay %i %i", kickedID, leader->db_id);
        serverParseClient(tmp, NULL);
    }

    // must be done with relay
    dbLog("League:Kick", leader, "%s kicked.", name);
    sprintf(tmp, "league_kick_relay %i %i", kickedID, leader->db_id);
    serverParseClient(tmp, NULL);
    turnstileMapserver_generateGroupUpdate(leader->db_id, leader->db_id, kickedID);
}

// I was sent a message by another mapserver requesting that I quit this league
void league_KickRelay(Entity* e, int kicked_by)
{
    int league_id = e->league_id;

    // Verify that the kicked_by is the leader of the league that we're currently on, in case
    // we've changed leagues since this message was sent
    if (!league_IsLeader(e, kicked_by) && !team_IsLeader(e, kicked_by))
    {
        // We're being kicked by someone who isn't our leader!  This should only occur in race conditions
        chatSendToPlayer( kicked_by, localizedPrintf(e,"failedToKickLeague", e->name), INFO_USER_ERROR, 0 );
    }
    else 
    {
        sendEntsMsg( CONTAINER_LEAGUES, league_id, INFO_SVR_COM, 0, "%s%s", DBMSG_CHAT_MSG, localizedPrintf(e,"wasKickedFromLeague", e->name) );
        league_MemberQuit(e, 1);
    }
}

void league_changeLeader( Entity *e, char * name )
{
    int newLeaderId;
    int i;

    if(!e || !e->pl)
        return;

    if( name[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "leagueLeaderChangeString", "playerNameString", "emptyString", "leagueLeaderChangeStringSyn"), INFO_USER_ERROR, 0);
        return;
    }

    newLeaderId = dbPlayerIdFromName(name);

    if( newLeaderId < 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"PlayerDoesNotExist", e->name), INFO_USER_ERROR, 0);
        return;
    }

    if( !e->league || !e->league_id || e->league->members.count <= 1 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotChangeLeaderNoLeague"), INFO_USER_ERROR, 0);
        return;
    }

    if( e->league->members.leader != e->db_id )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotChangeLeaderNotLeader"), INFO_USER_ERROR, 0);
        return;
    }

    if (e->league->members.leader == newLeaderId)
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotChangeLeaderIsLeader"), INFO_USER_ERROR, 0);
        return;
    }

    for( i = 0; i < e->league->members.count; i++ )
    {
        if( newLeaderId == e->league->members.ids[i] )
        {
            int teamLeader = e->league->teamLeaderIDs[i];
            if (teamLeader != newLeaderId)
            {
                //    new leader is not the leader of a team so promote him
                //    if he is already on the leader team, it will promote him
                team_changeLeaderRelay(teamLeader, name);
            }
            
            // Always send this message now, this player may not be a team leader yet
            // because we only relayed the request, but haven't processed it.
            SgrpStat_SendPassthru(e->db_id,0, "statserver_league_promote %d %d", e->db_id, newLeaderId);
            
            turnstileMapserver_generateGroupUpdate(e->db_id, newLeaderId, 0);
            return;
        }
    }
    chatSendToPlayer(e->db_id, localizedPrintf(e,"NoLeaderChangeNotAMember", name ), INFO_USER_ERROR, 0);
}
void league_updateTeamLeaderTeam(Entity *e, int old_leader_id, int new_leader_id)
{
    if (e && e->league)
    {
        SgrpStat_SendPassthru(e->db_id,0, "statserver_league_change_team_leader_team %d %d %d", e->league_id, old_leader_id, new_leader_id);
    }
}
void league_updateTeamLeaderSolo(Entity *e, int db_id, int new_leader_id)
{
    if (e && e->league)
    {
        SgrpStat_SendPassthru(e->db_id,0, "statserver_league_change_team_leader_solo %d %d %d", e->league_id, db_id, new_leader_id);
    }
}

void league_updateTeamLockStatus(Entity *e, int team_leader_id, int lock_status)
{
    if (e && e->league)
    {
        SgrpStat_SendPassthru(e->db_id,0, "statserver_league_update_team_lock %d %d %d", e->league_id, team_leader_id, lock_status);
    }
}
static void sendLeagueMate( Packet * pak, Entity * e, int i, int full_update )
{
    devassert(e->league->teamLeaderIDs[i] > 0);
    sendMemberCommon(pak, &e->league->members, i, full_update);
    pktSendBits( pak, 32, e->league->teamLeaderIDs[i]);
    pktSendBits( pak, 32, e->league->lockStatus[i]);
}

void league_sendList( Packet * pak, Entity * e, int full_update )
{
    int i;

    // send if there even is a team
    pktSendIfSetBitsPack( pak, PKT_BITS_TO_REP_DB_ID, e->league_id );

    if( !e->league )
    {
        devassert(!e->league_id);        //    make sure thay if they don't have a league, they don't have a league_id
        return;
    }
    else
    {
        devassert(e->league_id);    //    make sure that if we send a league, they have a league_id
    }
    //    send the leader
    pktSendBitsPack(pak, 32, e->league->members.leader );

    // now send the member count
    pktSendBitsPack(pak, 1, e->league->members.count);

    // send leader first
    for( i = 0; i < e->league->members.count; i++ )
    {
        if( e->league->members.leader == e->league->members.ids[i] )
        {
            sendLeagueMate(pak, e, i, full_update);
            break;            //    there can be only one
        }
    }

    // now send everyone else
    for( i = 0; i < e->league->members.count; i++ )
    {
        if( e->league->members.leader != e->league->members.ids[i] )
            sendLeagueMate(pak, e, i, full_update);
    }
}
void leagueInfoDestroy(Entity* e)
{
    if (e->league)
    {
        destroyLeague(e->league);
        e->league = NULL;
    }
    e->league_id = 0;
    e->leaguelist_uptodate = 0;
}
void leagueSendMapUpdate(Entity *e)
{
    if (e->league_id)
    {
        char buf[256];
        sprintf(buf, "%s%i", DBMSG_MAPLIST_REFRESH, e->map_id );
        dbBroadcastMsg(CONTAINER_LEAGUES, &e->league_id, INFO_SVR_COM, e->db_id, 1, buf);
    }
}
void leagueUpdateMaplist( Entity * e, char *map_num, int member_id )
{
    if ( e->league )
    {
        int i;
        for( i = 0; i < e->league->members.count; i++ )
        {
            if( member_id == e->league->members.ids[i] )
            {
                e->league->members.mapIds[i] = atoi(map_num);
                e->leaguelist_uptodate = 0;
            }
        }
    }
}

void league_teamswapLockToggle(Entity *e)
{
    if(!e || !e->pl)
        return;

    if( !e->league || !e->league_id || e->league->members.count <= 1 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotLockTeamNoLeague"), INFO_USER_ERROR, 0);
        return;
    }

    if( !e->teamup || !e->teamup_id || e->teamup->members.count <= 1 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotLockTeamNoTeam"), INFO_USER_ERROR, 0);
        return;
    }

    if ( e->teamup->members.leader != e->db_id )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"CannotLockTeamNotLeader"), INFO_USER_ERROR, 0);
        return;
    }

    if (teamLock(e, CONTAINER_TEAMUPS))
    {
        e->teamup->teamSwapLock = !e->teamup->teamSwapLock;
        teamUpdateUnlock(e, CONTAINER_TEAMUPS);
    }
    if (e->teamup->teamSwapLock)
    {
        league_updateTeamLockStatus(e, e->teamup->members.leader, 1);
    }
    else
    {
        league_updateTeamLockStatus(e, e->teamup->members.leader, e->pl && e->pl->taskforce_mode);
    }
}

void leagueUpdateTeamLock( Entity * e, char *teamLockStr, int member_id )
{
    if ( e->league )
    {
        int i;
        for( i = 0; i < e->league->members.count; i++ )
        {
            if( member_id == e->league->members.ids[i] )
            {
                int lockStatus = atoi(teamLockStr);
                assert(lockStatus >= 0);        //    Checking to see if the teamup is ever a bad number o.O (do we use unsigned ints for teamup ids this might be incorrect if we do)
                e->league->lockStatus[i] = lockStatus;
                e->leaguelist_uptodate = 0;
                break;
            }
        }
    }
}