//
//
// target.c -- targeting window functions go here
//----------------------------------------------------------------------------------------------------
#include "bases/DetailRecipe.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"

#include "UI/uiUtil.h"
#include "UI/uiInfo.h"
#include "UI/uiChat.h"
#include "UI/uiGame.h"
#include "UI/uiTeam.h"
#include "uiLeague.h"
#include "UI/uiTurnstile.h"
#include "UI/uiArena.h"
#include "UI/uiTrade.h"
#include "UI/uiInput.h"
#include "UI/uiCursor.h"
#include "UI/uiFriend.h"
#include "UI/uiTarget.h"
#include "UI/uiReticle.h"
#include "UI/uiCompass.h"
#include "UI/uiToolTip.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "UI/uiSupergroup.h"
#include "UI/uiContextMenu.h"
#include "UI/uiChannel.h"
#include "UI/uiPlayerNote.h"
#include "UI/uiStatus.h"

#include "language/langClientUtil.h"
#include "entity/character_base.h"
#include "player/playerSticky.h"
#include "graphics/ttFontUtil.h"
#include "clientcomm/clientcomm.h" // for input_pak
#include "cmdparse/cmdcommon.h"
#include "player/pmotion.h"
#include "cmdparse/cmdgame.h"
#include "player/player.h"
#include "win/input.h"
#include "entity/powers.h"
#include "entity/origins.h"
#include "entity/classes.h"
#include "gameComm/npc.h"
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include "uiSMFView.h"
#include "formatter/smf_main.h"
#include "graphics/textureatlas.h"
#include "entity/friendCommon.h"
#include "arena/arenagame.h"
#include "gameComm/itemselect.h"

#include "gameComm/chatClient.h" // only for UsingChatServer() call (temporary)
#include "entity/entity.h"
#include "entity/entclient.h"
#include "entity/EntPlayer.h"
#include <utilitieslib/utils/mathutil.h>
#include "UI/uiPet.h"
#include "entity/Supergroup.h"
#include "UI/uiOptions.h"
#include "bases/bases.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "uiLevelingpact.h"
#include "entity/character_target.h"
#include "UI/uiPopHelp.h"

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

#define MAX_ACTIONS         25

enum
{
    INTERACT_NONE,
    INTERACT_PLAYER,
    INTERACT_PLAYER_NOT_ON_MAP,
    INTERACT_VILLAIN,
    INTERACT_NPC,
    INTERACT_ITEM,
    INTERACT_OFFLINE,
    INTERACT_PET,
    INTERACT_TOTAL
};

enum
{
    ACTION_NONE,
    ACTION_READY,
    ACTION_EXPANDING,
    ACTION_OPEN,
    ACTION_RETRACTING,
};

enum
{
    ACTION_FADE_IN,
    ACTION_FADE_OUT,
    ACTION_DEPRESSED,
    ACTION_COLORED,
};

enum
{
    INTERACT_TRAY_CLOSED,
    INTERACT_TRAY_OPENING,
    INTERACT_TRAY_OPEN,
    INTERACT_TRAY_CLOSING,
};

typedef struct ActionButton
{
    int         state;
    int         clrState;

    int         (* visible)(void*);
    void        (* code )(void*);

    void        *visData;
    void        *codeData;

    int         color;
    float       timer;
    float       wd;
    float       yposition;
    char        *txt;
    ToolTip     tip;

} ActionButton;

typedef struct InteractTray
{
    int             state;
    int             arrow_clr;
    float           y;
    float           ysc;
    int             timer;
    ActionButton**    action;

}InteractTray;

InteractTray interact[INTERACT_TOTAL];

int gSelectedDBID = 0;
char gSelectedName[64] = {0};
char gSelectedHandle[64] = {0};

static TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.1f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_9,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

static TextAttribs gUglyTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xff8800ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x00ffffff,
    /* piScale       */  (int *)(int)(1.1f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_18,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

SMFView *gClassView;

//-------------------------------------------------------------------------------------------------------------------
// Init stuff  //////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------

static int canInteractWithNPC(void *foo)
{
    Entity* e = controlledPlayerPtr();
    if (!e || !current_target)
        return CM_HIDE;
    if ( distance3Squared(ENTPOS(e), ENTPOS(current_target)) > SQR(15) )
        return CM_HIDE;
    if(ENTTYPE(current_target) == ENTTYPE_NPC)
        return CM_AVAILABLE;
    if(ENTTYPE(current_target) == ENTTYPE_CRITTER && current_target->contactOrPnpc && !character_TargetIsFoe(e->pchar, current_target->pchar))
        return CM_AVAILABLE;
    return CM_HIDE;
        
}

void CodeFollow(void *foo)
{
    playerToggleFollowMode();
}

void CodeChatNPC(void *foo)
{
    if (!game_state.pendingTSMapXfer)
    {
        START_INPUT_PACKET(pak, CLIENTINP_TOUCH_NPC );
        pktSendBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX, current_target->svr_idx);
        END_INPUT_PACKET
    }
}

static void CodeInteractItem(void *foo)
{
    if (!game_state.pendingTSMapXfer)
    {
        START_INPUT_PACKET(pak, CLIENTINP_TOUCH_NPC );
        pktSendBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX, current_target->svr_idx);
        END_INPUT_PACKET
    }
}

int isCurrentTarget()
{
    if (current_target)
        return TRUE;
    else
        return FALSE;
}

void assistTarget(void *foo)
{
    if(current_target && ENTTYPE(current_target)==ENTTYPE_PLAYER)
    {
        char buf[256];
        sprintf(buf, "assist \"%s\"", current_target->name);
        cmdParse(buf);
    }
    else
    {
        addSystemChatMsg(textStd("NoTargetError"), INFO_USER_ERROR, 0);
        return;
    }
}

enum
{
    TARGET_TIP_HEALTH,
    TARGET_TIP_END,
    TARGET_TIP_ORIGIN,
    TARGET_TIP_CLASS,
    TARGET_TIP_WINDOW,
    TARGET_TIP_TOTAL,
};

static ToolTip targetTip[TARGET_TIP_TOTAL] = {0};

static ToolTipParent targetParentTip = {0};

ContextMenu *interactPlayer = 0;
ContextMenu *interactPlayerNotOnMap = 0;
ContextMenu *interactPlayerOffline = 0;
ContextMenu *interactVillain = 0;
ContextMenu *interactNPC = 0;
ContextMenu *interactItem = 0;
ContextMenu *interactPet = 0;
ContextMenu *interactSimplePet = 0;

static void interactTray_addCode( InteractTray *tray, int(*visible)(void*), void *visData, void (*code)(void*), void *codeData, char *txt)
{
    ActionButton *action = calloc(1, sizeof(ActionButton));
    action->txt = malloc(sizeof(char)*128);
    strcpy(action->txt, txt);
    action->code = code;
    action->codeData = codeData;
    action->visible = visible;
    action->visData = visData;
    eaPush(&tray->action, action);
}
static void initInteractTrays(void)
{
    //-------------------------------------------------------------------
    // player interaction
    //-------------------------------------------------------------------
    interactPlayer = contextMenu_Create( isCurrentTarget );

    contextMenu_addCode( interactPlayer, canTradeWithTarget, 0, tradeWithTarget, 0, "CMTradeString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], canTradeWithTarget, 0, tradeWithTarget, 0, "TradeString");

    contextMenu_addCode( interactPlayer, 0, 0, CodeFollow, 0, "CMFollowString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], alwaysAvailable, 0, CodeFollow, 0, "FollowString");

    contextMenu_addCode( interactPlayer, 0, 0, chatInitiateTell, 0, "CMChatString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], alwaysAvailable, 0, chatInitiateTell, 0, "ChatString");

    contextMenu_addCode( interactPlayer, 0, 0, chatInitiateTellToTeamLeader, 0, "CMChatToTeamLeaderString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], alwaysAvailable, 0, chatInitiateTellToTeamLeader, 0, "ChatToTeamLeaderString");

    contextMenu_addCode( interactPlayer, 0, 0, chatInitiateTellToLeagueLeader, 0, "CMChatToLeagueLeaderString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], alwaysAvailable, 0, chatInitiateTellToLeagueLeader, 0, "ChatToLeagueLeaderString");

    contextMenu_addCode( interactPlayer, alwaysAvailable, 0, info_EntityTarget, 0, "CMInfoString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], alwaysAvailable, 0, info_EntityTarget, 0, "InfoString");

    contextMenu_addCode( interactPlayer, isNotFriend, 0, addToFriendsList, 0, "CMAddFriendString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], isNotFriend, 0, addToFriendsList, 0, "AdFriendString");

    contextMenu_addCode( interactPlayer, isFriend, 0, removeFromFriendsList, 0, "CMRemoveFriendString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], isFriend, 0, removeFromFriendsList, 0, "RemFriendString");

    // BEGIN GLOBAL FRIENDS
    contextMenu_addCode( interactPlayer, isNotGlobalFriend, 0, addToGlobalFriendsList, 0, "CMAddGlobalFriendString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], isNotGlobalFriend, 0, addToGlobalFriendsList, 0, "AddGlobalFriendString");

    contextMenu_addCode( interactPlayer, isGlobalFriend, 0, removeFromGlobalFriendsList, 0, "CMRemoveGlobalFriendString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], isGlobalFriend, 0, removeFromGlobalFriendsList, 0, "RemGlobalFriendString");
    // END GLOBAL FRIENDS 

    contextMenu_addCode( interactPlayer, team_CanOfferMembership, 0, team_OfferMembership, 0, "CMInviteTeamString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], team_CanOfferMembership, 0, team_OfferMembership, 0, "InviteTeamString");

    contextMenu_addCode( interactPlayer, team_CanKickMember, 0, team_KickMember, 0, "CMKickFromTeam", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], team_CanKickMember, 0, team_KickMember, 0, "KickFromTeam");

    contextMenu_addCode( interactPlayer, team_CanCreateLeagueMember, 0, team_CreateLeagueMember, 0, "CMCreateLeagueFromTeam", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], team_CanCreateLeagueMember, 0, team_CreateLeagueMember, 0, "CreateLeagueFromTeam");

    contextMenu_addCode( interactPlayer, league_CanOfferMembership, 0, league_OfferMembership, 0, "CMInviteLeagueString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], league_CanOfferMembership, 0, league_OfferMembership, 0, "InviteLeagueString");

    contextMenu_addCode( interactPlayer, league_CanKickMember, 0, league_KickMember, 0, "CMKickFromLeague", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], league_CanKickMember, 0, league_KickMember, 0, "KickFromLeague");

    contextMenu_addCode( interactPlayer, league_CanAddToFocus, 0, league_AddToFocus, 0, "CMLeagueAddToFocus", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], league_CanAddToFocus, 0, league_AddToFocus, 0, "LeagueAddToFocus");

    contextMenu_addCode( interactPlayer, league_CanRemoveFromFocus, 0, league_RemoveFromFocus, 0, "CMLeagueRemoveFromFocus", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], league_CanRemoveFromFocus, 0, league_RemoveFromFocus, 0, "LeagueRemoveFromFocus");

    contextMenu_addCode( interactPlayer, league_CanSwapMember, 0, league_SetLeftSwapMember, 0, "CMLeagueSetLeftSwap", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], league_CanSwapMember, 0, league_SetLeftSwapMember, 0, "LeagueSetAsLeftSide");

    contextMenu_addCode( interactPlayer, league_CanSwapMembers, 0, league_SwapMembers, 0, "CMLeagueSwap", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], league_CanSwapMembers, 0, league_SwapMembers, 0, "LeagueSwapMember");

    {
        int i;
        for (i = 0; i < MAX_LEAGUE_TEAMS; ++i)
        {
            char buff[20], buff2[20];
            sprintf(buff, "CMLeagueMove%i", i+1);
            sprintf(buff2, "LeagueMove%i", i+1);
            contextMenu_addCode( interactPlayer, league_CanMoveMember, (void*)i, league_MoveMembers, (void*)i, buff, 0 );
            interactTray_addCode( &interact[INTERACT_PLAYER], league_CanMoveMember, (void*)i, league_MoveMembers, (void*)i, buff2);
        }
    }

    contextMenu_addCode( interactPlayer, sgroup_canOfferMembership, 0, sgroup_invite, 0, "CMInviteSupergroupString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], sgroup_canOfferMembership, 0, sgroup_invite, 0, "InviteSupergroupString");

    contextMenu_addCode( interactPlayer, sgroup_canKick, 0, sgroup_kick, 0, "CMKickFromSgroup", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], sgroup_higherRanking, 0, sgroup_kick, 0, "KickFromSupergroup");

    contextMenu_addCode( interactPlayer, sgroup_canPromote, 0, sgroup_promote, 0, "CMPromote", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], sgroup_higherRanking, 0, sgroup_promote, 0, "PromoteString");

    contextMenu_addCode( interactPlayer, sgroup_canDemote, 0, sgroup_demote, 0, "CMDemote", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], sgroup_higherRanking, 0, sgroup_demote, 0, "DemoteString");

    contextMenu_addCode( interactPlayer, levelingpact_CanOfferMembership, 0, levelingpact_OfferMembership, 0, "CMInviteLevelingpactString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], levelingpact_CanOfferMembership, 0, levelingpact_OfferMembership, 0, "InviteLevelingpactString");

    contextMenu_addCode( interactPlayer, team_TargetOnMap, 0, compass_setTarget, 0, "CMSetWayPoint", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], team_TargetOnMap, 0, compass_setTarget, 0, "SetWayPoint");

    contextMenu_addCode( interactPlayer, team_makeLeaderVisible, 0, team_makeLeader, 0, "CMMakeLeader", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], team_makeLeaderVisible, 0, team_makeLeader, 0, "MakeLeader");

    contextMenu_addCode( interactPlayer, league_CanMakeLeader, 0, league_makeLeader, 0, "CMLeagueMakeLeader", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], league_CanMakeLeader, 0, league_makeLeader, 0, "LeagueMakeLeader");

    contextMenu_addCode( interactPlayer, allianceCanInviteTarget, 0, allianceInviteTarget, 0, "CMAllianceInvite", NULL);
    interactTray_addCode( &interact[INTERACT_PLAYER], allianceCanInviteTarget, 0, allianceInviteTarget, 0, "AllianceInvite");

    contextMenu_addCode( interactPlayer, arenaCanInviteTarget, 0, arenaInviteTarget, 0, "CMArenaInvite", NULL);
    interactTray_addCode( &interact[INTERACT_PLAYER], arenaCanInviteTarget, 0, arenaInviteTarget, 0, "ArenaInvite");

    //contextMenu_addCode( interactPlayer, raidCanInviteTarget, 0, raidInviteTarget, 0, "CMRaidInvite", NULL);
    //interactTray_addCode( &interact[INTERACT_PLAYER], raidCanInviteTarget, 0, raidInviteTarget, 0, "RaidInvite");

    contextMenu_addVariableTextCode( interactPlayer, alwaysAvailable, 0, playerNote_TargetEdit, 0, playerNote_TargetEditName, 0, 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], alwaysAvailable, 0, playerNote_TargetEdit, 0, "AddEditNote");

    contextMenu_addCode( interactPlayer, turnstileGameCM_onEndGameRaid, 0, turnstile_votekickTarget, 0, "TUTVoteKickRequest", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER], turnstileGameCM_onEndGameRaid, 0, turnstile_votekickTarget, 0, "TUTVoteKickRequest");

    contextMenu_addCode( interactPlayer, canInviteTargetToAnyChannel, 0, 0, 0, "CMChannelInvite", getChannelInviteMenu());

    //-------------------------------------------------------------------
    // Off mapserver player interaction
    //-------------------------------------------------------------------
    interactPlayerNotOnMap = contextMenu_Create( 0 );

    contextMenu_addCode( interactPlayerNotOnMap, 0, 0, chatInitiateTell, 0, "CMChatString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], alwaysAvailable, 0, chatInitiateTell, 0, "ChatString");

    contextMenu_addCode( interactPlayerNotOnMap, 0, 0, chatInitiateTellToTeamLeader, 0, "CMChatToTeamLeaderString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], alwaysAvailable, 0, chatInitiateTellToTeamLeader, 0, "ChatToTeamLeaderString");

    contextMenu_addCode( interactPlayerNotOnMap, 0, 0, chatInitiateTellToLeagueLeader, 0, "CMChatToLeagueLeaderString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], alwaysAvailable, 0, chatInitiateTellToLeagueLeader, 0, "ChatToLeagueLeaderString");

    contextMenu_addCode( interactPlayerNotOnMap, isNotFriend, 0, addToFriendsList, 0, "CMAddFriendString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], isNotFriend, 0, addToFriendsList, 0, "AdFriendString");

    contextMenu_addCode( interactPlayerNotOnMap, isFriend, 0, removeFromFriendsList, 0, "CMRemoveFriendString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], isFriend, 0, removeFromFriendsList, 0, "RemFriendString");

    // BEGIN GLOBAL FRIENDS
    contextMenu_addCode( interactPlayerNotOnMap, isNotGlobalFriend, 0, addToGlobalFriendsList, 0, "CMAddGlobalFriendString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], isNotGlobalFriend, 0, addToGlobalFriendsList, 0, "AddGlobalFriendString");

    contextMenu_addCode( interactPlayerNotOnMap, isGlobalFriend, 0, removeFromGlobalFriendsList, 0, "CMRemoveGlobalFriendString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], isGlobalFriend, 0, removeFromGlobalFriendsList, 0, "RemGlobalFriendString");

    contextMenu_addCode( interactPlayerNotOnMap, team_CanOfferMembership, 0, team_OfferMembership, 0, "CMInviteTeamString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], alwaysAvailable, 0, team_OfferMembership, 0, "InviteTeamString");

    contextMenu_addCode( interactPlayerNotOnMap, team_CanKickMember, 0, team_KickMember, 0, "CMKickFromTeam", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], team_CanKickMember, 0, team_KickMember, 0, "KickFromTeam");

    contextMenu_addCode( interactPlayerNotOnMap, team_CanCreateLeagueMember, 0, team_CreateLeagueMember, 0, "CMCreateLeagueFromTeam", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], team_CanCreateLeagueMember, 0, team_CreateLeagueMember, 0, "CreateLeagueFromTeam");

    contextMenu_addCode( interactPlayerNotOnMap, league_CanOfferMembership, 0, league_OfferMembership, 0, "CMInviteLeagueString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], alwaysAvailable, 0, league_OfferMembership, 0, "InviteLeagueString");

    contextMenu_addCode( interactPlayerNotOnMap, league_CanKickMember, 0, league_KickMember, 0, "CMKickFromLeague", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], league_CanKickMember, 0, league_KickMember, 0, "KickFromLeague");

    contextMenu_addCode( interactPlayerNotOnMap, league_CanAddToFocus, 0, league_AddToFocus, 0, "CMLeagueAddToFocus", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], league_CanAddToFocus, 0, league_AddToFocus, 0, "LeagueAddToFocus");

    contextMenu_addCode( interactPlayerNotOnMap, league_CanRemoveFromFocus, 0, league_RemoveFromFocus, 0, "CMLeagueRemoveFromFocus", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], league_CanRemoveFromFocus, 0, league_RemoveFromFocus, 0, "LeagueRemoveFromFocus");

    contextMenu_addCode( interactPlayerNotOnMap, sgroup_canOfferMembership, 0, sgroup_invite, 0, "CMInviteSupergroupString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], sgroup_exists, 0, sgroup_invite, 0, "InviteSupergroupString");

    contextMenu_addCode( interactPlayerNotOnMap, sgroup_higherRanking, 0, sgroup_kick, 0, "CMKickFromSgroup", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], sgroup_higherRanking, 0, sgroup_kick, 0, "KickFromSupergroup");

    contextMenu_addCode( interactPlayerNotOnMap, sgroup_higherRanking, 0, sgroup_promote, 0, "CMPromote", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], sgroup_higherRanking, 0, sgroup_promote, 0, "PromoteString");

    contextMenu_addCode( interactPlayerNotOnMap, sgroup_higherRanking, 0, sgroup_demote, 0, "CMDemote", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], sgroup_higherRanking, 0, sgroup_demote, 0, "DemoteString");

    //contextMenu_addCode( interactPlayer, levelingpact_CanOfferMembership, 0, levelingpact_OfferMembership, 0, "CMInviteLevelingpactString", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], levelingpact_CanOfferMembership, 0, levelingpact_OfferMembership, 0, "InviteLevelingpactString");

    contextMenu_addCode( interactPlayerNotOnMap, team_makeLeaderVisible, 0, team_makeLeader, 0, "CMMakeLeader", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], team_makeLeaderVisible, 0, team_makeLeader, 0, "MakeLeader");

    contextMenu_addCode( interactPlayerNotOnMap, league_CanMakeLeader, 0, league_makeLeader, 0, "CMLeagueMakeLeader", 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], league_CanMakeLeader, 0, league_makeLeader, 0, "LeagueMakeLeader");

    contextMenu_addVariableTextCode( interactPlayerNotOnMap, alwaysAvailable, 0, playerNote_TargetEdit, 0, playerNote_TargetEditName, 0, 0 );
    interactTray_addCode( &interact[INTERACT_PLAYER_NOT_ON_MAP], alwaysAvailable, 0, playerNote_TargetEdit, 0, "AddEditNote");

    contextMenu_addCode( interactPlayerNotOnMap, canInviteTargetToAnyChannel, 0, 0, 0, "CMChannelInvite", getChannelInviteMenu());

    //-------------------------------------------------------------------
    // Offline player interaction
    //-------------------------------------------------------------------
    interactPlayerOffline = contextMenu_Create( 0 );

    contextMenu_addCode( interactPlayerOffline, isNotFriend, 0, addToFriendsList, 0, "CMAddFriendString", 0 );
    interactTray_addCode( &interact[INTERACT_OFFLINE], isNotFriend, 0, addToFriendsList, 0, "AdFriendString");

    contextMenu_addCode( interactPlayerOffline, isFriend, 0, removeFromFriendsList, 0, "CMRemoveFriendString", 0 );
    interactTray_addCode( &interact[INTERACT_OFFLINE], isFriend, 0, removeFromFriendsList, 0, "RemFriendString");

    contextMenu_addCode( interactPlayerOffline, sgroup_higherRanking, 0, sgroup_kick, 0, "CMKickFromSgroup", 0 );
    interactTray_addCode( &interact[INTERACT_OFFLINE], sgroup_higherRanking, 0, sgroup_kick, 0, "KickFromSupergroup");

    contextMenu_addVariableTextCode( interactPlayerOffline, alwaysAvailable, 0, playerNote_TargetEdit, 0, playerNote_TargetEditName, 0, 0 );
    interactTray_addCode( &interact[INTERACT_OFFLINE], alwaysAvailable, 0, playerNote_TargetEdit, 0, "AddEditNote");

    //-------------------------------------------------------------------
    // villain interaction
    //-------------------------------------------------------------------
    interactVillain = contextMenu_Create( isCurrentTarget );

    contextMenu_addCode( interactVillain, 0, 0, CodeFollow, 0, "CMFollowString", 0 );
    interactTray_addCode( &interact[INTERACT_VILLAIN], alwaysAvailable, 0, CodeFollow, 0, "FollowString");

    contextMenu_addCode( interactVillain, alwaysAvailable, 0, info_EntityTarget, 0, "CMInfoString", 0 );
    interactTray_addCode( &interact[INTERACT_VILLAIN], alwaysAvailable, 0, info_EntityTarget, 0, "InfoString");

    //-------------------------------------------------------------------
    // NPC interaction
    //-------------------------------------------------------------------
    interactNPC = contextMenu_Create( isCurrentTarget );

    contextMenu_addCode( interactNPC, 0, 0, CodeFollow, 0, "CMFollowString", 0 );
    interactTray_addCode( &interact[INTERACT_NPC], alwaysAvailable, 0, CodeFollow, 0, "FollowString");

    contextMenu_addCode( interactNPC, canInteractWithNPC, 0, CodeChatNPC, 0, "CMChatString", 0 );
    interactTray_addCode( &interact[INTERACT_NPC], canInteractWithNPC, 0, CodeChatNPC, 0, "ChatString");

    //-------------------------------------------------------------------
    // Pet interaction
    //-------------------------------------------------------------------
    interactPet = interactPetMenu();
    interactSimplePet = interactSimplePetMenu();
    //-------------------------------------------------------------------
    // Item interaction
    //-------------------------------------------------------------------

    interactItem = contextMenu_Create( isCurrentTarget );

    contextMenu_addCode( interactItem, 0, 0, CodeInteractItem, 0, "CMInteractString", 0 );
    interactTray_addCode( &interact[INTERACT_ITEM], alwaysAvailable, 0, CodeInteractItem, 0, "InteractString");

    {
        int i;
        for( i = 0; i < TARGET_TIP_TOTAL; i++ )
            addToolTip( &targetTip[i] );
    }
}


void contextMenuForTarget( Entity *e )
{
    int x, y;

    if( !e ) 
        return;

     inpMousePos( &x, &y );

    //If you have a pet selected, then you want the pet to do it's default action on the 
    //selected thing, not show a context menu
    if( 0 )//g_petCommandMode == HAVE_PET_SELECTED )
    { 
    //    menu_Show("PetCommands", x, y);    
    }
    else if( entIsMyHenchman(e)) //If this is your pet, show the pet options
    {
        contextMenu_set( interactPet, x, y, e );
    }
    else if (entIsMyPet(e) || isPetDisplayed(e))
    {
        contextMenu_set( interactSimplePet, x, y, e );
    }
    else if( ENTTYPE(e) == ENTTYPE_PLAYER )
    {
        contextMenu_set( interactPlayer, x, y, NULL );
    }
    else if( ENTTYPE(e) == ENTTYPE_CRITTER )
    {
        if (e->contactOrPnpc)
            contextMenu_set( interactNPC, x, y, NULL );
        else
            contextMenu_set( interactVillain, x, y, NULL );
    }
    else if( ENTTYPE(e) == ENTTYPE_NPC )
    {
        contextMenu_set( interactNPC, x, y, NULL );
    }
    else if( ENTTYPE(e) == ENTTYPE_MISSION_ITEM )
    {
        contextMenu_set( interactItem, x, y, NULL );
    }
}


bool OfflineTarget()
{
    Entity *e = playerPtr();
    int i;

    if( isFriend(NULL) )
    {
        for( i = 0; i < e->friendCount; i++ )
        {
            if( targetGetDBID() == e->friends[i].dbid )
            {
                if( e->friends[i].online )
                    return false;
                else
                    return true;
            }
        }
    }

    if( sgroup_isMember(e, targetGetDBID() ) )
    {
        for( i = 0; i < eaSize(&e->sgStats); i++ )
        {
            if( e->sgStats[i]->db_id == targetGetDBID() )
            {
                if(e->sgStats[i]->online)
                    return false;
                else
                    return true;
            }
        }
    }

    return false;
}

void contextMenuForOffMap(void)
{
    int x, y;

    inpMousePos( &x, &y);

    if( OfflineTarget() )
        contextMenu_set( interactPlayerOffline, x, y, NULL );
    else
        contextMenu_set( interactPlayerNotOnMap, x, y, NULL );
}

//-------------------------------------------------------------------------------------------------------------------
// TARGET OPTIONS WINDOW ////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------

#define TO_XOFF         15
#define TO_YOFF         15
#define TO_ACTION_HT    20
#define TO_SPEED        20

static float interactList( float x, float y, float z, float sc, float wd, InteractTray *it )
{
    int i;
    float height = 0;
    CBox box;
    int num_actions = eaSize(&it->action);

    font( &game_12 );

    for( i = 0; i < num_actions; i++ )
    {
        int clickable = 0;
        if( it->action[i]->visible(NULL) == CM_HIDE )
            continue;
        else if( it->action[i]->visible(NULL) == CM_VISIBLE )
            font_color( CLR_OFFLINE_ITEM, CLR_OFFLINE_ITEM );
        else if( it->action[i]->visible(NULL) == CM_AVAILABLE )
        {
            clickable = TRUE;
            font_color( CLR_WHITE, CLR_ONLINE_ITEM );
        }
        else
            continue;

        BuildCBox( &box, x + TO_XOFF*sc, y + TO_YOFF*sc + height, wd - 2*TO_XOFF*sc, (TO_ACTION_HT-1)*sc );

        if( clickable && mouseCollision(&box) )
        {
            if( isDown( MS_LEFT ) )
                drawFrame( PIX3, R10, x + TO_XOFF*sc/2, y + TO_YOFF*sc + height, z, wd - TO_XOFF*sc, TO_ACTION_HT*sc, sc, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
            else
            {
                drawFrame( PIX3, R10, x + TO_XOFF*sc/2, y + TO_YOFF*sc + height, z, wd - TO_XOFF*sc, TO_ACTION_HT*sc, sc, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
            }
        }

        if( clickable && mouseClickHit(&box, MS_LEFT) )
            it->action[i]->code(NULL);

         prnt( x + TO_XOFF*sc, y + TO_YOFF*sc + height + 15*sc, z, sc, sc, it->action[i]->txt );

        height += TO_ACTION_HT*sc;
    }

    return height + 2*TO_YOFF*sc;
}


int targetOptionsWindow( void )
{
    float x, y, z, wd, ht, scale;
    int color, bcolor;

     if( !window_getDims( WDW_TARGET_OPTIONS, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
          return 0;

    drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, bcolor );

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );

    set_scissor( TRUE );
    scissor_dims( x + PIX3*scale, y + PIX3*scale, wd - 2*PIX3*scale, ht - 2*PIX3*scale );

    if( !current_target && !gSelectedDBID )
    {
         cprntEx( x + TO_XOFF*scale, y + 18*scale, z, scale, scale, CENTER_Y, "NoActions" );

        if( window_getMode(WDW_TARGET_OPTIONS) == WINDOW_DISPLAYING )
            window_setDims( WDW_TARGET_OPTIONS, x, y, wd, (TO_YOFF+TO_ACTION_HT)*scale );
    }
    else
    {
        InteractTray *it = 0;
        float height;

        if( current_target )
        {
            switch( ENTTYPE(current_target) )
            {
                case ENTTYPE_PLAYER:
                    {
                        it = &interact[INTERACT_PLAYER];
                    }break;
                case ENTTYPE_CRITTER:
                    {
                        it = &interact[INTERACT_VILLAIN];
                    }break;
                case ENTTYPE_NPC:
                    {
                        it = &interact[INTERACT_NPC];
                    }break;
                default:
                    it = &interact[INTERACT_ITEM];
            }
        }
        else
        {
            if ( OfflineTarget() )
                it = &interact[INTERACT_OFFLINE];
            else
                it = &interact[INTERACT_PLAYER_NOT_ON_MAP];
        }

        height = interactList( x, y, z, scale, wd, it );

        if( ht < height )
        {
            ht += TIMESTEP*TO_SPEED;
            if( ht > height )
                ht = height;

            if( window_getMode(WDW_TARGET_OPTIONS) == WINDOW_DISPLAYING )
                window_setDims( WDW_TARGET_OPTIONS, x, y, wd, ht );
        }

        if( ht > height )
        {
            ht -= TIMESTEP*TO_SPEED;
            if( ht < height )
                ht = height;

            if( window_getMode(WDW_TARGET_OPTIONS) == WINDOW_DISPLAYING )
                window_setDims( WDW_TARGET_OPTIONS, x, y, wd, ht );
        }
    }

    set_scissor( FALSE );
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// TARGET WINDOW ////////////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------

static int higherLevelColors[5] =   {   0xffffffff, // white
                                        0xffff00ff, // yellow
                                        0xff8844ff, // orange
                                        0xff0000ff, // red
                                        0xff00ffff, // purple
                                    };

static int lowerLevelColors[5] =    {   0xffffffff, // white
                                        0x0077ddff, // blue
                                        0x66cc66ff, // green
                                        0xaaaaaaff, // grey
                                        0x888888ff, // grey
                                    };

int getConningColor( Entity *e, Entity *target )
{
    int vlvl;
    int plvl;
    int diff;
    int rank;

    if ( !e || !target || !e->pchar || !target->pchar )
        return CLR_WHITE;

    // get their level now
    if (target->pchar->bIgnoreCombatMods)
    {
        plvl = e->pchar->iCombatLevel;
        // This is slightly misleading for very low level characters.  They may see things conning lower than they actually are.
        rank = target->iRank + target->pchar->ignoreCombatModLevelShift;
        vlvl = plvl + rank;
        diff = ABS(rank); 
    } else {
        plvl = e->pchar->iCombatLevel + e->pchar->iCombatModShift;
        vlvl = target->pchar->iCombatLevel + target->pchar->iCombatModShift + target->iRank;
        diff = ABS(vlvl-plvl);
    }

    if(diff>4)
        diff=4;

    if( vlvl <= plvl )
        return lowerLevelColors[diff];
    else
        return higherLevelColors[diff];
}


void drawConningArrows(Entity *e, Entity *eTarget, float x, float y, float z, float scale, int arrow_color)
{
    int diff;
    float fAngle = 0;
    AtlasTex * arrow = atlasLoadTexture("Conning_arrow.tga");

    if( eTarget->pchar )
    {
        diff = (eTarget->pchar->iCombatLevel + eTarget->pchar->iCombatModShift + eTarget->iRank) - (e->pchar->iCombatLevel + e->pchar->iCombatModShift);
        if(diff<0)
        {
            fAngle = PI;
            diff = -diff;
        }
    }
    else
        diff = 0;

    if( diff == 1 )
        display_rotated_sprite( arrow, x - arrow->width*scale/2, y, z, scale, scale, arrow_color, fAngle, 0 ); // mid
    else if( diff == 2 )
    {
        display_rotated_sprite( arrow, x, y, z, scale, scale, arrow_color, fAngle, 0 ); //right
        display_rotated_sprite( arrow, x - arrow->width*scale, y, z, scale, scale, arrow_color, fAngle, 0 ); // left
    }
    else if( diff == 3 )
    {
        display_rotated_sprite( arrow, x - arrow->width*scale/2, y, z, scale, scale, arrow_color, fAngle, 0 ); // mid
        display_rotated_sprite( arrow, x + arrow->width*scale/2, y, z, scale, scale, arrow_color, fAngle, 0 ); // right
        display_rotated_sprite( arrow, x - arrow->width*scale/2 - arrow->width*scale, y, z, scale, scale, arrow_color, fAngle, 0 ); // left
    }

}

#define TXT_HT      20
#define CONN_SPACER 2
#define BAR_WD      116

CapsuleData target_health;
CapsuleData target_absorb;
CapsuleData target_end;

int targetWindow(void)
{
    Entity *e = playerPtr();

    float x, y, ty, z, wd, ht, scale;
    int color, back_color, txtColor;
    static AtlasTex * w;
    static AtlasTex * barH;
    static AtlasTex * barE;
    static AtlasTex * arrow;
    static int init = FALSE;
    
    int pulsecolor = 0;
    static float pulsefloat = 0;
    float pulseper;

    pulsefloat += 0.140*TIMESTEP;
    pulseper = (sinf(pulsefloat)+1)/2;

     interp_rgba(pulseper,0xff4444ff,0x88222222,&pulsecolor);

       if( !init )
    {
        w = white_tex_atlas;
        barH = atlasLoadTexture( "BAR_HEALTH.tga" );
        barE = atlasLoadTexture( "BAR_ENDURANCE.tga" );
        arrow = atlasLoadTexture("Conning_arrow.tga");
        initInteractTrays();
        init = TRUE;
    }

    if( !window_getDims( WDW_TARGET, &x, &y, &z, &wd, &ht, &scale, &color, &back_color ) )
        return 0;

    if (!e)
        return 0;

    BuildCBox( &targetParentTip.box, x, y, wd, ht );
    ty = y;

    drawFrame( PIX3, R22, x, ty, z-2, wd, ht, scale, color, back_color );
    set_scissor(true);
    scissor_dims( x+PIX3*scale, y+PIX3*scale, wd-PIX3*2*scale, ht-PIX3*2*scale);

    if (current_target )
    {
        int     arrow_color;
        char    msname[128];
        char    name[128];
        static  Entity *last_target = 0;
        float   titleSc = 1.f*scale;

        if( current_target != last_target )
        {
            if (current_target->pchar)
            {
                target_health.curr = (current_target->pchar->attrCur.fHitPoints + current_target->pchar->attrMax.fAbsorb) / (current_target->pchar->attrMax.fHitPoints + current_target->pchar->attrMax.fAbsorb);
                target_health.showNumbers = TRUE;

                target_absorb.curr = current_target->pchar->attrMax.fAbsorb / (current_target->pchar->attrMax.fHitPoints + current_target->pchar->attrMax.fAbsorb);
                target_absorb.showNumbers = TRUE;

                target_end.curr = current_target->pchar->attrCur.fEndurance/current_target->pchar->attrMax.fEndurance;
                target_end.showNumbers = TRUE;

                if (character_TargetIsFoe(e->pchar, current_target->pchar))
                    popHelpEventHappenedByTag("CODEPH_Target_Enemy");
            } 
            else 
            {
                // No pchar (i.e. NPC)
                target_health.curr = 1.0f;
                target_health.showNumbers = FALSE;

                target_absorb.curr = 0.0f;
                target_absorb.showNumbers = FALSE;

                target_end.curr = 1.0f;
                target_end.showNumbers = FALSE;
            }
        }

        // figure and display their name first
        if (((!optionGet(kUO_DisablePetNames) && entIsPlayerPet(current_target)) || entIsMyPet(current_target)) && 
            current_target->petName && current_target->petName[0] != '\0') 
        {
            strcpy(msname, current_target->petName);
        }
        else if ( current_target->name[0] )
            strcpy( msname, current_target->name );
        else
            strcpy( msname, npcDefList.npcDefs[current_target->npcIndex]->displayName ? textStd(npcDefList.npcDefs[current_target->npcIndex]->displayName) : "" );

        arrow_color = getConningColor( e, current_target );
        txtColor = arrow_color;
        brightenColor( &txtColor, 30);
        font( &game_12 );
        font_color( txtColor, arrow_color );

        // draw their colored name
        ty += TXT_HT*scale;
        if( ENTTYPE(current_target) == ENTTYPE_PLAYER )
        {
            strcpy( name, msname );
            txtColor = getReticleColor(current_target, 0, 1, 1);
            arrow_color = getReticleColor(current_target, 1, 1, 1);
        }
        else
            strcpy( name, msname );

          if( str_wd_notranslate( &game_12, titleSc, titleSc, name) > (wd - R27*2*scale) && winDefs[WDW_TARGET].loc.mode == WINDOW_DISPLAYING )
            titleSc = str_sc( &game_12, (wd - R27*2*scale), name  );

        {
            int rgba[4] = {  txtColor, txtColor, arrow_color, arrow_color };

              if( ENTTYPE(current_target) == ENTTYPE_NPC )
                cprntEx( x + wd/2, y + ht/2, z, titleSc, titleSc, (CENTER_X | CENTER_Y | NO_MSPRINT), name );
            else
                printBasic( &game_12, x + wd/2, ty - 4*scale, z, titleSc, titleSc, TRUE, name, strlen(name), rgba );
        }

        // now draw up or down arrows depending on their level
        ty += CONN_SPACER*scale;
        drawConningArrows(e, current_target, x+wd/2, ty - 4*scale, z, scale, arrow_color);
        ty += (CONN_SPACER + arrow->height + 4)*scale;

        // now draw their level and type if they are a critter - Eye spel gud
        if( ENTTYPE(current_target) == ENTTYPE_CRITTER )
        {
            const char * displayClassName;

            font( &game_9 );
            font_color( txtColor, arrow_color );

            cprnt( x + wd/2, ty-2*scale, z, scale, scale,  "Brackets", current_target->pchVillGroupName );
            clearToolTip( &targetTip[TARGET_TIP_ORIGIN] );
            clearToolTip( &targetTip[TARGET_TIP_CLASS] );

            font_color( CLR_WHITE, CLR_WHITE );

            //MW added so villaindef file could override the villain.
            if( current_target->pchDisplayClassNameOverride )
                displayClassName = current_target->pchDisplayClassNameOverride;
            else
                displayClassName = current_target->pchar->pclass->pchDisplayName;

            if(!gClassView)
            {
                gClassView = smfview_Create(WDW_TARGET);
                smfview_SetAttribs(gClassView, &gTextAttr);
            }

             if( displayClassName && stricmp( displayClassName, "NONE" ))
            {
                if( str_wd(font_grp, scale, scale, displayClassName) > wd/4 )
                {
                    gTextAttr.piScale = (int*)((int)(1.1f*SMF_FONT_SCALE*scale));
                       smfview_SetLocation(gClassView, wd/4 + (BAR_WD*scale),ty-y-3*scale, 0);
                    smfview_SetSize(gClassView, wd/4, 400);
                    smfview_SetText(gClassView, textStd(displayClassName));
                    smfview_Draw(gClassView);
                }
                else            
                    cprnt( x + wd - (wd - BAR_WD*scale)/4 - PIX3*scale, ty + 12*scale, z, scale, scale, displayClassName );
            }

            // Don't show the level on scary monsters.
            if(current_target->iRank < 50 && 
                !(current_target && current_target->pchar && current_target->pchar->bIgnoreCombatMods))
            {
                cprnt( x + PIX3*scale + (wd - BAR_WD*scale)/4, ty + 12*scale, z, scale, scale, "LevelLevel", current_target->pchar->iCombatLevel+1 );
                if (current_target->pchar && current_target->pchar->iCombatModShift > 0)
                {
                    cprnt( x + PIX3*scale + (wd - BAR_WD*scale)/4, ty + 24*scale, z, scale, scale, "LevelLevelUp", current_target->pchar->iCombatModShift );
                }
                else if (current_target->pchar && current_target->pchar->iCombatModShift < 0)
                {
                    cprnt( x + PIX3*scale + (wd - BAR_WD*scale)/4, ty + 24*scale, z, scale, scale, "LevelLevelDown", -current_target->pchar->iCombatModShift );
                }
            }
        }
        else if( ENTTYPE(current_target) == ENTTYPE_PLAYER )
        {
            AtlasTex  * archIcon;
            AtlasTex  * originIcon = 0;

            const char *displayName = current_target->pchar->pclass->pchDisplayName;    // P string in reality ..

            font( &game_9 );
            font_color(getReticleColor(e, 0, 1, 1), getReticleColor(e, 1, 1, 1) );

            if( current_target->supergroup_name && current_target->supergroup_name[0]   )
                cprnt( x + wd/2, ty, z, scale, scale,  "Brackets", current_target->supergroup_name );

            if (current_target->pchar->iCombatLevel < current_target->pchar->iLevel)
                font_color( CLR_VILLAGON, CLR_VILLAGON );    // exemplar
            else if (current_target->pchar->iCombatLevel > current_target->pchar->iLevel)
                font_color(CLR_PARAGON, CLR_PARAGON);        // sidekick
            else
                font_color( CLR_WHITE, CLR_WHITE );

            cprnt( x + PIX3*scale + (wd - BAR_WD*scale)/4, ty + 12*scale, z, scale, scale, "LevelLevel", current_target->pchar->iCombatLevel+1 );
            if (current_target->pchar->iCombatModShift > 0)
            {
                cprnt( x + PIX3*scale + (wd - BAR_WD*scale)/4, ty + 24*scale, z, scale, scale, "LevelLevelUp", current_target->pchar->iCombatModShift );
            }
            else if (current_target->pchar->iCombatModShift < 0)
            {
                cprnt( x + PIX3*scale + (wd - BAR_WD*scale)/4, ty + 24*scale, z, scale, scale, "LevelLevelDown", -current_target->pchar->iCombatModShift );
            }

            font_color(CLR_WHITE, CLR_WHITE);
            originIcon = GetOriginIcon( current_target );
            display_sprite( originIcon, x + (30 - originIcon->width/4)*scale, y + (22 - originIcon->height/4)*scale, z, scale/2, scale/2, CLR_WHITE );

            // HACK HACK HACK
            // Arachnos VEAT names are too long.  If we detect one, replace with a shorter name.
            if (stricmp(current_target->pchar->pclass->pchName, "Class_Arachnos_Soldier") == 0) // Spider
            {
                displayName = "ArachnosSpider";
            }
            if (stricmp(current_target->pchar->pclass->pchName, "Class_Arachnos_Widow") == 0)
            {
                displayName = "ArachnosWidow";
            }

            cprnt( x + wd - (wd - BAR_WD*scale)/4 - PIX3*scale, ty + 12*scale, z, scale, scale, current_target->pchar->porigin->pchDisplayName );

            archIcon = GetArchetypeIcon( current_target );
            display_sprite( archIcon, x + wd - (30 + archIcon->width/4)*scale, y + (22 - archIcon->height/4)*scale, z, scale/2, scale/2, CLR_WHITE );
            cprnt( x + wd - (wd - BAR_WD*scale)/4 - PIX3, ty + 22*scale, z, scale, scale, displayName );

            BuildCBox( &targetTip[TARGET_TIP_ORIGIN].bounds,x+20*scale,    y+14*scale, 20*scale, 20*scale );
            BuildCBox( &targetTip[TARGET_TIP_CLASS].bounds,    x+196*scale,    y+14*scale, 20*scale, 20*scale );
            setToolTip( &targetTip[TARGET_TIP_CLASS], &targetTip[TARGET_TIP_CLASS].bounds, textStd("classTip", current_target->pchar->pclass->pchDisplayName), &targetParentTip, MENU_GAME, WDW_TARGET );
            setToolTip( &targetTip[TARGET_TIP_ORIGIN], &targetTip[TARGET_TIP_ORIGIN].bounds, textStd("originTip", current_target->pchar->porigin->pchDisplayName), &targetParentTip, MENU_GAME, WDW_TARGET );
        }
        else
        {
            clearToolTip( &targetTip[TARGET_TIP_ORIGIN] );
            clearToolTip( &targetTip[TARGET_TIP_CLASS] );
            clearToolTip( &targetTip[TARGET_TIP_HEALTH] );
            clearToolTip( &targetTip[TARGET_TIP_END] );
        }

         

        if( !entMode(current_target, ENT_DEAD ) )
        {
            if (current_target->pchar)
            {
                bool detailspecialend = false;
                BuildCBox( &targetTip[TARGET_TIP_HEALTH].bounds,  x + (wd - BAR_WD*scale)/2, ty, BAR_WD*scale, 11*scale );
                BuildCBox( &targetTip[TARGET_TIP_END].bounds,  x + (wd - BAR_WD*scale)/2, ty+11*scale, BAR_WD*scale, 13*scale );
                drawCapsule( x + (wd - BAR_WD*scale)/2, ty, z+1, BAR_WD*scale, scale, color, CLR_BLACK, CLR_RED,
                     current_target->pchar->attrCur.fHitPoints + current_target->pchar->attrMax.fAbsorb, current_target->pchar->attrMax.fHitPoints + current_target->pchar->attrMax.fAbsorb, &target_health );
                if (current_target->pchar->attrMax.fAbsorb > 0.005f)
                {
                    drawCapsule( x + (wd - BAR_WD*scale)/2, ty, z+1, BAR_WD*scale, scale, color, 0x00000000, getAbsorbColor(current_target->pchar->attrMax.fAbsorb / current_target->pchar->attrMax.fHitPoints),
                        current_target->pchar->attrMax.fAbsorb, current_target->pchar->attrMax.fHitPoints + current_target->pchar->attrMax.fAbsorb, &target_absorb);
                }

                 setToolTip( &targetTip[TARGET_TIP_HEALTH], &targetTip[TARGET_TIP_HEALTH].bounds, textStd("HealthTip", current_target->pchar->attrCur.fHitPoints, current_target->pchar->attrMax.fHitPoints ), &targetParentTip, MENU_GAME, WDW_TARGET );

                ty += 10*scale;
                if(current_target->idDetail)
                {
                    RoomDetail *detail = baseGetDetail(&g_base,current_target->idDetail,NULL);
                    if(detail && !(detail->bPowered && detail->bControlled))
                    {
                        AtlasTex *engIcon = atlasLoadTexture("base_icon_power.tga");
                        AtlasTex *conIcon = atlasLoadTexture("base_icon_control.tga");
                        float iconscale = scale * 1.35;

                        font_color(pulsecolor,pulsecolor);

                        detailspecialend = true;

                        if(!detail->bControlled && !detail->bPowered)
                        {
                            display_sprite( engIcon, x+wd/2-((engIcon->width*iconscale)+1), ty-8.5*scale, z+5, iconscale, iconscale, pulsecolor );
                            display_sprite( conIcon, x+wd/2+1, ty-8.5*scale+0.5, z+5, iconscale, iconscale, pulsecolor );
                        }
                        else
                        {
                            display_sprite( detail->bControlled?engIcon:conIcon, x+wd/2-((engIcon->width * iconscale)/2), ty-8.5*scale+(detail->bControlled?0:0.5), z+5, iconscale, iconscale, pulsecolor );
                        }

                        setToolTip( &targetTip[TARGET_TIP_END], &targetTip[TARGET_TIP_END].bounds, textStd(detail->bPowered?"DetailNoControl":detail->bControlled?"DetailNoPower":"DetailNoPowerControl"), &targetParentTip, MENU_GAME, WDW_TARGET );
                        drawCapsuleEx( x + (wd - BAR_WD*scale)/2, ty, z+1, BAR_WD*scale, scale, color, CLR_BLACK, 0x444444ff, 0x222222ff,
                                current_target->pchar->attrCur.fEndurance, current_target->pchar->attrMax.fEndurance, &target_end );

                    }
                }
                if(!detailspecialend)
                {
                    setToolTip( &targetTip[TARGET_TIP_END], &targetTip[TARGET_TIP_END].bounds, textStd("EnduranceTip", (int)current_target->pchar->attrCur.fEndurance, (int)current_target->pchar->attrMax.fEndurance), &targetParentTip, MENU_GAME, WDW_TARGET );
                    drawCapsule( x + (wd - BAR_WD*scale)/2, ty, z+1, BAR_WD*scale, scale, color, CLR_BLACK, CLR_BLUE,
                            current_target->pchar->attrCur.fEndurance, current_target->pchar->attrMax.fEndurance, &target_end );
                }
            }
        }
        else
        {
            BuildCBox( &targetTip[TARGET_TIP_HEALTH].bounds,  x + (wd - BAR_WD*scale)/2, ty, BAR_WD*scale, 11*scale );
            BuildCBox( &targetTip[TARGET_TIP_END].bounds,  x + (wd - BAR_WD*scale)/2, ty+11*scale, BAR_WD*scale, 13*scale );
             drawCapsule( x + (wd - BAR_WD*scale)/2, ty, z+1, BAR_WD*scale, scale, color, CLR_BLACK, CLR_RED, 0, 1, &target_health );
            ty += 10*scale;
            drawCapsule( x + (wd - BAR_WD*scale)/2, ty, z+1, BAR_WD*scale, scale, color, CLR_BLACK, CLR_BLUE, 0, 1, &target_end );
        }

        last_target = current_target;
         clearToolTip( &targetTip[TARGET_TIP_WINDOW] );

        //now do the interact tray
        ty += (TXT_HT + CONN_SPACER)*scale;
    }
    else
    {
        int i;
        for( i = 0; i < TARGET_TIP_TOTAL; i++ )
            clearToolTip( &targetTip[i] );

        BuildCBox( &targetTip[TARGET_TIP_WINDOW].bounds, x, y, wd, ht );
        setToolTip( &targetTip[TARGET_TIP_WINDOW], &targetTip[TARGET_TIP_WINDOW].bounds, "targetWindowTip", NULL, MENU_GAME, WDW_TARGET );

        target_health.curr = 0.0f;
        target_health.showNumbers = FALSE;

        target_absorb.curr = 0.0f;
        target_absorb.showNumbers = FALSE;

        target_end.curr = 0.0f;
        target_end.showNumbers = FALSE;

        ty += TXT_HT*scale;
        font( &game_12 );
        font_color( CLR_WHITE, CLR_WHITE );

        if( gSelectedDBID > 0 )
        {
             cprntEx(  x + wd/2, ty, z, scale, scale, CENTER_X|NO_MSPRINT, gSelectedName );
            cprnt(  x + wd/2, ty + 15*scale, z, scale, scale, "NotInCity" );
        }
        else
            cprntEx( x + wd/2, y + ht/2, z, scale, scale, (CENTER_X | CENTER_Y), "NoTargetString" );
    }

    set_scissor(false);

    return 0;
}


//-------------------------------------------------------------------------------------------------
// Public target interface
//-------------------------------------------------------------------------------------------------
EntityRef currentTarget;
int targetSelect(Entity* e)
{
    if(e)
    {
        targetClear();
        current_target = e;
        if (entIsMyHenchman(e)) current_pet = erGetRef(e);
        return 1;
    }
    return 0;
}

int targetSelect2(int dbid, char* name)
{
    if(!dbid || !name)
        return 0;

    targetClear();
    gSelectedDBID = dbid;
    strncpyt(gSelectedName, name, sizeof(gSelectedName));
    gSelectedName[sizeof(gSelectedName)-1] = '\0';
    return 1;
}

void targetSetHandle(char * handle)
{
    strncpyt(gSelectedHandle, handle, sizeof(gSelectedHandle));
}

int targetSelectHandle(char * handle, int db_id)
{
    if(!handle)
        return 0;

    // Temporary, until new system is "officially" in
    if(!UsingChatServer())
        return 0;

    targetClear();
    strncpyt(gSelectedHandle, handle, sizeof(gSelectedHandle));
    gSelectedDBID = db_id;
    return 1;
}

int targetHasSelection(void)
{
    if(current_target || gSelectedDBID || gSelectedHandle[0])
        return 1;
    else
        return 0;
}

void targetClear(void)
{
    current_target = 0;
    gSelectedDBID = 0;
    gSelectedName[0] = '\0';
    gSelectedHandle[0] = '\0';
}

int targetGetDBID(void)
{
    if(current_target)
        return current_target->db_id;
    if(gSelectedDBID)
        return gSelectedDBID;

    return 0;
}

char* targetGetName(void)
{
    if(current_target)
        return current_target->name;
    if(gSelectedDBID && gSelectedName[0])
        return gSelectedName;

    return NULL;
}

char* targetGetHandle(void)
{
    if(gSelectedHandle[0])
        return gSelectedHandle;

    return NULL;
}

void setCurrentTargetByName(char * name)
{
    Entity * target = entFromId(selectNextEntity(current_target?current_target->svr_idx:0, 0, 0, 0, 0, 0, 0, name ));

     if( target && target != playerPtr() && strstri(target->name, name) )
        current_target = target;
    else
        addSystemChatMsg( textStd( "NoTargetMatch", name ), INFO_USER_ERROR, 0);
}

void targetAssist(char * name)
{
    Entity * e = playerPtr();

    if( server_visible_state.isPvP )
    {
        // no target assist in PvP
    }
    else if( name ) // target must be close enough for client to know about them
    {
        Entity * target = entFromId(selectNextEntity( 0, 0, -1, 1, 0, 0, 0, name ));
        if( target && target != playerPtr() )
        {
            Entity *targets_target = erGetEnt(target->pchar->erTargetInstantaneous);
            if( targets_target && targets_target != e )
                current_target = targets_target;
        }
        else
            addSystemChatMsg( textStd( "NoAlliedTargetMatch", name ), INFO_USER_ERROR, 0);
    }
    else
    {
        if(current_target && current_target->pchar)
        {
            Entity *targets_target = erGetEnt(current_target->pchar->erTargetInstantaneous);
            if( targets_target && targets_target != e )
                current_target = targets_target;
        }
    }
}
