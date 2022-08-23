#include "entity/entity.h"
#include "player/player.h"
#include "entity/teamCommon.h"
#include "UI/uiCursor.h"
#include "UI/uiContextMenu.h"
#include "UI/uiChat.h"
#include "UI/uiTarget.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "gameComm/wdwbase.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiWindows.h"
#include "UI/uiGroupWindow.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiLeague.h"
#include "entity/teamup.h"

int league_CanOfferMembership(void *foo)
{
    Entity *e = playerPtr();
    Entity *pInvitee = NULL;

    // must have a player target that is not a league member or a team member
    if ( current_target )
    {    
        if( ENTTYPE(current_target) != ENTTYPE_PLAYER || league_IsMember( e, current_target->db_id) || team_IsMember(e, current_target->db_id))
            return CM_HIDE;
    }
    else if ( !gSelectedDBID || league_IsMember( e, gSelectedDBID ) || team_IsMember(e, gSelectedDBID) )
        return CM_HIDE;

    if ( e->teamup_id )
    {
        if (e->db_id != e->teamup->members.leader)
        {
            return CM_HIDE;
        }
    }

    // if there is no league, an offer can be made
    if( !e->league_id ) 
        return CM_AVAILABLE;

    // otherwise, must be the group leader to offer and have room on the league
    if ( e->db_id == e->league->members.leader )
    {
        if ( e->league->members.count < MAX_LEAGUE_MEMBERS )
            return CM_AVAILABLE;
        else
            return CM_VISIBLE;
    }
    return CM_HIDE;

}

void league_OfferMembership(void *foo)
{
    char buf[256];
    // double check that they can
    if( league_CanOfferMembership(NULL) == CM_AVAILABLE )
    {
        if( current_target )
            sprintf( buf, "li %s", current_target->name );    //    li is league invite
        else if ( gSelectedDBID )
            sprintf( buf, "li %s", gSelectedName );            //    li is league invite
        else
        {
            addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
            return;
        }

        // Force up the league window
        window_setMode( WDW_LEAGUE, WINDOW_GROWING );

        cmdParse( buf );
    }
}

//
//
int league_CanKickMember(void *foo)
{
    Entity *e = playerPtr();

    // must have a player target that is a league member
    if ( current_target )
    {    
        if( ENTTYPE(current_target) != ENTTYPE_PLAYER || !league_IsMember( e, current_target->db_id) )
            return CM_HIDE;
    }
    else if ( !gSelectedDBID || !league_IsMember( e, gSelectedDBID ) )
        return CM_HIDE;

    // if there is no league, there is no one to kick
    if( !e->league_id )
        return CM_HIDE;

    // now only the leader can kick
    if( e->db_id == e->league->members.leader )
        return CM_AVAILABLE;
    else
        return CM_HIDE;    
}


//
//
void league_KickMember(void *foo)
{
    // double check that they can
    if( league_CanKickMember(NULL) == CM_AVAILABLE)
    {
        char buf[128];

        if( current_target )
            sprintf( buf, "lk %s", current_target->name );    //    league kick
        else if ( gSelectedDBID)
            sprintf( buf, "lk %s", gSelectedName );            //    league kick
        else
        {
            addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
            return;
        }

        cmdParse( buf );
    }
}

static int league_isMemberTeamLeader(Entity * e, int db_id)
{
    if (e && e->league)
    {
        League *league = e->league;
        int i;
        for (i = 0; i < league->members.count; ++i)
        {
            if (db_id == league->members.ids[i])
            {
                return (league->teamLeaderIDs[i] == db_id);
            }
        }
    }
    return 0;
}
static int leftSwapMember = 0;
int league_CanSwapMember(void *foo)
{
    Entity *e = playerPtr();

    // if there is no league, there is no one to swap
    if( !e->league_id )
        return CM_HIDE;

    // must have a player target that is a league member
    if ( current_target )
    {    
        if( ENTTYPE(current_target) != ENTTYPE_PLAYER || !league_IsMember( e, current_target->db_id) )
            return CM_HIDE;

        //    make sure they aren't already selected as swap member
        if (leftSwapMember == current_target->db_id)
            return CM_HIDE;

        //    for now, team leaders can't be swapped
        //    neither can teams that are locked
        if (league_isMemberTeamLeader(e, current_target->db_id) || league_isMemberTeamLocked(e, current_target->db_id))
        {
            return CM_VISIBLE;
        }
    }
    else
        return CM_HIDE;

    // now only the leader can swap
    if( e->db_id == e->league->members.leader )
        return CM_AVAILABLE;
    else
        return CM_HIDE;    
}
int league_CanSwapMembers(void *foo)
{
    if (!leftSwapMember)
        return CM_HIDE;
    return league_CanSwapMember(NULL);
}
void league_SetLeftSwapMember(void *foo)
{
    // double check that they can
    if( league_CanSwapMember(NULL) == CM_AVAILABLE)
    {
        leftSwapMember = current_target ? current_target->db_id : 0;
    }
}
void league_SwapMembers(void *foo)
{
    if( league_CanSwapMember(NULL) == CM_AVAILABLE)
    {
        if (current_target->db_id != leftSwapMember)
        {
            char buf[128];

            if( current_target )
                sprintf( buf, "league_teamswap %i %i", leftSwapMember, current_target->db_id );    //    league kick
            else
            {
                addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
                return;
            }

            cmdParse( buf );
            leftSwapMember = 0;
        }
    }
}
int league_CanMoveMember(void *foo)
{
    Entity *e = playerPtr();
    int target_team = (int)foo;
    // if there is no league, there is no one to swap
    if( !e->league_id )
        return CM_HIDE;

    // must have a player target that is a league member
    if ( current_target )
    {    
        int numTeams = eaSize(&e->league->teamRoster);
        if( ENTTYPE(current_target) != ENTTYPE_PLAYER || !league_IsMember( e, current_target->db_id) )
            return CM_HIDE;

        //    if the ents team is locked, forget it
        if (league_isMemberTeamLocked(e, current_target->db_id))
        {
            return CM_HIDE;
        }

        if (target_team < numTeams)
        {
            int leaderIndex = e->league->teamRoster[target_team][0];
            int teamSize = eaiSize(&e->league->teamRoster[target_team]);
            int i;
            for (i = 0; i < teamSize; ++i)
            {
                int memberIndex = e->league->teamRoster[target_team][i];
                if (e->league->members.ids[memberIndex] == current_target->db_id)
                {
                    return CM_HIDE;    //    already on the target team
                }
            }
            //    target team is full
            if (teamSize >= MAX_TEAM_MEMBERS)
            {
                return CM_VISIBLE;
            }
            //    the target team is locked
            if (e->league->lockStatus[leaderIndex])
            {
                return CM_VISIBLE;
            }
        }
        else if (target_team > numTeams)
        {
            return CM_HIDE;
        }
    }
    else
        return CM_HIDE;

    // now only the leader can swap
    if( e->db_id == e->league->members.leader )
        return CM_AVAILABLE;
    else
        return CM_HIDE;    
}
void league_MoveMembers(void *foo)
{
    if( league_CanMoveMember(foo) == CM_AVAILABLE)
    {
        char buf[128];

        if( current_target )
        {
            int teamToMoveTo = (int)foo;
            int teamLeaderId = -1;
            Entity *e = playerPtr();
            if (EAINRANGE(teamToMoveTo, e->league->teamRoster))
            {
                if (eaiSize(&e->league->teamRoster[teamToMoveTo]))
                {
                    int memberIndex = e->league->teamRoster[teamToMoveTo][0];
                    teamLeaderId = e->league->members.ids[memberIndex];
                }
                else
                {
                    addSystemChatMsg( textStd("InvalidTeam"), INFO_USER_ERROR, 0 );
                    return;
                }
            }
            sprintf( buf, "league_teammove %i %i %i", e->db_id, current_target->db_id, teamLeaderId );
        }
        else
        {
            addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
            return;
        }

        cmdParse( buf );
    }
}
int league_targetIsMember( Entity * e )
{
    if( current_target )
        return league_IsMember(e, current_target->db_id);
    else if ( gSelectedDBID )
        return league_IsMember(e, gSelectedDBID );

    return 0;
}

int league_CanMakeLeader(void *unused)
{
    Entity *e = playerPtr();

    if(    e->league && e->league->members.leader == e->db_id &&            // player is team leader
        ((current_target && league_IsMember(e, current_target->db_id)) ||    // onmap target is team member or 
        league_IsMember( e, gSelectedDBID ) )                            // off map target is team memeber
        )                            
        return CM_AVAILABLE;

    return CM_HIDE;
}

void league_makeLeader(void *unused)
{
    Entity * e = playerPtr();

    // redundant check, but oh well
    if( league_CanMakeLeader(NULL) == CM_AVAILABLE )
    {
        char buf[128];

        if( current_target )
            sprintf( buf, "lml %s", current_target->name );
        else if ( gSelectedDBID )
            sprintf( buf, "lml %s", gSelectedName );
        else
        {
            addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
            return;
        }

        cmdParse( buf );
    }
}
static int *focusGroup;

int league_CanAddToFocus(void *foo)
{
    Entity *e = playerPtr();

    if(e->league)
    {
        int db_id = current_target ? current_target->db_id : gSelectedDBID;
        if (league_IsMember(e, db_id))
        {
            if (eaiFind(&focusGroup, db_id) == -1)
                return CM_AVAILABLE;
        }
    }

    return CM_HIDE;
}

void league_AddToFocus(void *foo)
{
    Entity *e = playerPtr();
    if (league_CanAddToFocus(foo) == CM_AVAILABLE)
    {
        int j;
        int db_id = current_target ? current_target->db_id : gSelectedDBID;
        eaiPush(&focusGroup, db_id);
        for (j = 0; j < (eaSize(&e->league->teamRoster)); ++j)
        {
            int k;
            for (k = 0; k < eaiSize(&e->league->teamRoster[j]); ++k)
            {
                if (e->league->members.ids[e->league->teamRoster[j][k]] == db_id)
                {
                    eaiPush(&e->league->focusGroupRosterIndex, e->league->teamRoster[j][k]);
                    league_updateTabCount();
                    break;
                }
            }
        }
    }
}

int league_CanRemoveFromFocus(void *foo)
{
    Entity *e = playerPtr();

    if(e->league)
    {
        int db_id = current_target ? current_target->db_id : gSelectedDBID;
        if (league_IsMember(e, db_id))
        {
            if (eaiFind(&focusGroup, db_id) != -1)
                return CM_AVAILABLE;
        }
    }

    return CM_HIDE;
}

void league_RemoveFromFocus(void *foo)
{
    Entity *e = playerPtr();
    if (league_CanRemoveFromFocus(foo) == CM_AVAILABLE)
    {
        int j;
        int db_id = current_target ? current_target->db_id : gSelectedDBID;
        eaiFindAndRemove(&focusGroup, db_id);
        for (j = 0; j < (eaSize(&e->league->teamRoster)); ++j)
        {
            int k;
            for (k = 0; k < eaiSize(&e->league->teamRoster[j]); ++k)
            {
                if (e->league->members.ids[e->league->teamRoster[j][k]] == db_id)
                {
                    eaiFindAndRemove(&e->league->focusGroupRosterIndex, e->league->teamRoster[j][k]);
                    league_updateTabCount();
                    break;
                }
            }
        }
    }
}

void leagueOrganizeTeamList(League *league)
{
    //    this algorithm is definitely not the fastest..
    //    for every change to team membership, every player will recreate a team roster list
    //    it would be better if this would have only have to be done once
    //    if this turns out to be a bottleneck, there is definitely room to improve performance
    //    I moved this out of server and made it client side only, if the server has usage for a coherent team list, I don't have a problem adding it there too...
    //    I was originally going to do this on the person updating
    //    but I'm worried about the league lists getting out of synch and then.. who's to say which one is correct -EML
    int i;
    int *tempTeamList = NULL;

    assert(league);

    if (!league)
    {
        return;
    }

    //    clear the old league team roster
    for (i = 0; i < eaSize(&league->teamRoster); ++i)
    {
        eaiDestroy(&league->teamRoster[i]);
    }
    eaDestroy(&league->teamRoster);

    //    generate a new one
    for (i = 0; i < league->members.count; ++i)
    {
        int j;
        int matchFound = 0;
        for (j = 0; j < eaiSize(&tempTeamList); ++j)
        {
            if ((league->teamLeaderIDs[i] == tempTeamList[j]) && tempTeamList[j])
            {
                if (league->teamLeaderIDs[i] == league->members.ids[i])
                {
                    if (league->teamRoster[j][0] == league->members.leader)
                    {
                        eaiInsert(&league->teamRoster[j], i, 1);        //    put the team leader in the front, after the league leader
                        //    league leader should be first by virtue of send order
                    }
                    else
                        eaiInsert(&league->teamRoster[j], i, 0);        //    put the team leader in the front
                }
                else
                {
                    eaiPush(&league->teamRoster[j], i);
                }
                matchFound = 1;
                break;
            }
        }
        if (!matchFound)
        {
            int *newList = NULL;
            eaiPush(&newList, i);
            eaPush(&league->teamRoster, newList);
            eaiPush(&tempTeamList, league->teamLeaderIDs[i]);
        }
    }
    eaiDestroy(&tempTeamList);
}
void leagueCheckFocusGroup(Entity *e)
{
    //    go through league
    //    make sure that everyone in focus group is still in league
    int i;
    eaiDestroy(&e->league->focusGroupRosterIndex);
    if (league_IsMember(e, e->db_id))
    {
        for (i = (eaiSize(&focusGroup)-1); i >= 0; --i)
        {
            if (league_IsMember(e, focusGroup[i]))
            {
                int j;
                for (j = 0; j < (eaSize(&e->league->teamRoster)); ++j)
                {
                    int k;
                    for (k = 0; k < eaiSize(&e->league->teamRoster[j]); ++k)
                    {
                        if (e->league->members.ids[e->league->teamRoster[j][k]] == focusGroup[i])
                        {
                            eaiPush(&e->league->focusGroupRosterIndex, e->league->teamRoster[j][k]);
                            break;
                        }
                    }
                }
            }
            else
            {
                eaiRemove(&focusGroup, i);
            }
        }
    }
}
void leagueClearFocusGroup(Entity *e)
{
    eaiDestroy(&focusGroup);
}