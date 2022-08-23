

#include "UI/uiTeam.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiCursor.h"
#include "UI/uiTarget.h"
#include "UI/uiNet.h"
#include "UI/uiChat.h"
#include "UI/uiWindows.h"
#include "gameComm/wdwbase.h"
#include "entity/entVarUpdate.h"
#include "UI/uiContextMenu.h"
#include "UI/sprite/sprite_text.h"
#include "entity/teamCommon.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "storyarc/contactClient.h"
#include "UI/uiContactDialog.h"
#include "UI/uiGroupWindow.h"
#include "entity/teamup.h"

int team_TargetOnMap(void *foo)
{
    Entity *e = playerPtr();
    int i;

    if ( !e->teamup_id || !current_target )
        return CM_HIDE;

    for( i = 0; i < e->teamup->members.count; i++ )
    {
        if( current_target->db_id == e->teamup->members.ids[i] && e->teamup->members.onMapserver[i] )
            return CM_AVAILABLE;
    }

    return CM_VISIBLE;
}

int team_targetIsMember( Entity * e )
{
    if( current_target )
        return team_IsMember(e, current_target->db_id);
    else if ( gSelectedDBID )
        return team_IsMember(e, gSelectedDBID );

    return 0;
}

//
//
int team_CanOfferMembership(void *foo)
{
    Entity *e = playerPtr();
    TaskStatus* activetask = PlayerGetActiveTask();
    Entity *pInvitee = NULL;

    // must have a player target that is not a team member
    if ( current_target )
    {    
        if( ENTTYPE(current_target) != ENTTYPE_PLAYER || team_IsMember( e, current_target->db_id) )
            return CM_HIDE;
    }
    else if ( !gSelectedDBID || team_IsMember( e, gSelectedDBID ) )
        return CM_HIDE;

    if (activetask && activetask->alliance != SA_ALLIANCE_BOTH)
    {
        if (current_target)
            pInvitee = current_target;
        else
            pInvitee = entFromDbId(gSelectedDBID);

        if (pInvitee && pInvitee->pl && pInvitee->pl->playerType != e->pl->playerType)
            return CM_HIDE;
    }

    // if there is no team, an offer can be made
    if( !e->teamup_id ) 
        return CM_AVAILABLE;
    
    // otherwise, must be the group leader to offer and have room on the team
    if( e->db_id == e->teamup->members.leader && e->teamup->members.count < MAX_TEAM_MEMBERS )
        return CM_AVAILABLE;
    else if ( e->db_id == e->teamup->members.leader )
        return CM_VISIBLE;
    else
        return CM_HIDE;
}

//
//
int team_CanKickMember(void *foo)
{
    Entity *e = playerPtr();

    // must have a player target that is a team member
    if ( current_target )
    {    
        if( ENTTYPE(current_target) != ENTTYPE_PLAYER || !team_IsMember( e, current_target->db_id) )
            return CM_HIDE;
    }
    else if ( !gSelectedDBID || !team_IsMember( e, gSelectedDBID ) )
        return CM_HIDE;

    // if there is no team, there is no one to kick
    if( !e->teamup_id )
        return CM_HIDE;

    // now only the leader can kick
    if( e->db_id == e->teamup->members.leader )
        return CM_AVAILABLE;
    else
        return CM_HIDE;    
}

//
//    kicks the player from the team and then makes then join the league
int team_CanCreateLeagueMember(void *foo)
{
    Entity *e = playerPtr();

    // must have a player target that is a team member
    if ( current_target )
    {    
        if( ENTTYPE(current_target) != ENTTYPE_PLAYER || !team_IsMember( e, current_target->db_id) )
            return CM_HIDE;
    }
    else if ( !gSelectedDBID || !team_IsMember( e, gSelectedDBID ) )
        return CM_HIDE;

    // if there is no team, there is no one to move
    if( !e->teamup_id )
        return CM_HIDE;

    //    can only create the league if not already in one
    if (e->league_id && e->league)
    {
        return CM_HIDE;
    }

    //    can only use this command if not on a taskforce and on a static map
    if ((e->pl && e->pl->taskforce_mode) || game_state.mission_map)
        return CM_HIDE;

    // now only the leader can move
    if( e->db_id == e->teamup->members.leader )
        return CM_AVAILABLE;
    else
        return CM_HIDE;    

}
//
//
int team_CanQuit(void)
{
    // check to see if we are on a mission
    return TRUE;
}

//
//
void team_OfferMembership(void *foo)
{
    char buf[256];
    // double check that they can
     if( team_CanOfferMembership(NULL) == CM_AVAILABLE )
    {
        if( current_target )
            sprintf( buf, "i %s", current_target->name );
        else if ( gSelectedDBID )
            sprintf( buf, "i %s", gSelectedName );
        else
        {
            addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
            return;
        }

        // Force up the team window
        window_setMode( WDW_GROUP, WINDOW_GROWING );

        cmdParse( buf );
    }
}

//
//
void team_KickMember(void *foo)
{
    // double check that they can
    if( team_CanKickMember(NULL) == CM_AVAILABLE )
    {
        char buf[128];

        if( current_target )
            sprintf( buf, "k %s", current_target->name );
        else if ( gSelectedDBID)
            sprintf( buf, "k %s", gSelectedName );
        else
        {
            addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
            return;
        }

        cmdParse( buf );
    }
}

void team_CreateLeagueMember(void *foo)
{
    if (team_CanCreateLeagueMember(NULL) == CM_AVAILABLE)
    {
        char buf[128];

        if( current_target )
            sprintf( buf, "tmtl %s", current_target->name );
        else if ( gSelectedDBID)
            sprintf( buf, "tmtl %s", gSelectedName );
        else
        {
            addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
            return;
        }

        cmdParse( buf );
    }
}
//
//
void team_Quit(void * unused)
{
    // check for a mission map
    if( team_CanQuit() )
        cmdParse( "leaveTeam" );
}

int team_makeLeaderVisible(void *unused)
{
    Entity *e = playerPtr();

    if(    e->teamup && e->teamup->members.leader == e->db_id &&            // player is team leader
        ((current_target && team_IsMember(e, current_target->db_id)) ||    // onmap target is team member or 
         team_IsMember( e, gSelectedDBID ) )                            // off map target is team memeber
      )                            
        return CM_AVAILABLE;

    return CM_HIDE;
}

void team_makeLeader(void *unused)
{
    Entity * e = playerPtr();

    // redundant check, but oh well
    if( team_makeLeaderVisible(NULL) == CM_AVAILABLE )
    {
        char buf[128];

        if( current_target )
            sprintf( buf, "ml %s", current_target->name );
        else if ( gSelectedDBID )
            sprintf( buf, "ml %s", gSelectedName );
        else
        {
            addSystemChatMsg( textStd("NoTargetError"), INFO_USER_ERROR, 0 );
            return;
        }

        cmdParse( buf );
    }
}

//
//
int team_PlayerIsOnTeam()
{
    Entity *e = playerPtr();

    if( e->teamup )
        return true;
    else
        return false;
}

