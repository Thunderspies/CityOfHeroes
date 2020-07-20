/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <stdio.h>
#include "entity/rewardtoken.h"
#include "bases/baseupkeep.h"

#include "bases/DetailRecipe.h"


#include "entity/Invention.h"
#include <stdlib.h>

#include <utilitieslib/components/Earray.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/network/netio.h>
#include "entity/EntPlayer.h"
#include "UI/uiInspiration.h"
#include "entity/entVarUpdate.h"
#include "clientcomm/clientcomm.h"
#include "entity/entclient.h"
#include "language/langClientUtil.h"
#include "player/player.h"
#include "cmdparse/cmdgame.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_net_client.h"
#include "entity/character_inventory.h"
#include "UI/uiNet.h"
#include "UI/uiChat.h"
#include "UI/uiGame.h"
#include "UI/uiDock.h"
#include "UI/uidialog.h"
#include "UI/uiConsole.h"
#include "uiSalvage.h"
#include "UI/uiCompass.h"
#include "UI/uiAutomap.h"
#include "UI/uiSupergroup.h"
#include "UI/uiEmail.h"
#include "UI/uiTrade.h"
#include "UI/uiWindows.h"
#include "UI/uiInfo.h"
#include "UI/uiPet.h"
#include "UI/uiLevelSpec.h"
#include "UI/uiContactDialog.h"
#include "UI/uiRecipeInventory.h"
#include "gameComm/wdwbase.h"
#include "group/groupnetrecv.h"
#include "entity/costume.h"
#include "entity/classes.h"
#include "entity/origins.h"
#include "entity/character_net.h"
#include "UI/uiRespec.h"
#include <utilitieslib/utils/timing.h>
#include "UI/sprite/sprite_text.h"
#include "UI/uiKeybind.h"
#include "UI/uiTray.h"
#include "UI/uiFriend.h"
#include <utilitieslib/components/StringCache.h>
#include "UI/uiSuperRegistration.h"
#include "UI/uiBrowser.h"
#include "uiEmote.h"
#include "UI/uiToolTip.h"
#include "UI/uiReticle.h"
#include "UI/uiNet.h"
#include "UI/uiLfg.h"
#include "gameData/store.h"
#include "uiStore.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/truetype/ttFontDraw.h"
#include "gameData/BodyPart.h"
#include "UI/uiGroupWindow.h"
#include "UI/uiSupercostume.h"
#include "UI/uiSuperRegistration.h"
#include "entity/teamup.h"
#include "uiCombineSpec.h"
#include "UI/uiCostumeSelect.h"
#include "UI/uiOptions.h"
#include "uiRewardChoice.h"
#include "UI/uiStatus.h"
#include "UI/uiTailor.h"
#include "UI/uiSupergroup.h"
#include "uiListView.h"
#include "UI/uiToolTip.h"
#include "UI/uiBuff.h"
#include "gameData/arenastruct.h"
#include "entity/friendCommon.h"
#include "entity/entity.h"
#include "UI/uiArenaList.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/estring.h>
#include <utilitieslib/components/Earray.h>
#include "gameComm/petCommon.h"
#include <utilitieslib/network/netcomp.h>
#ifdef TEST_CLIENT
#define srReportSGCreationResult(a)
#define lfg_setResults(a,b)
#include "testAccountServer.h"
#include "testMissionSearch.h"
#endif
#include "entity/Supergroup.h"
#include "entity/character_workshop.h"
#include "auth/authUserData.h"
#include "clientcomm/dbclient.h"
#include "bases/basedata.h"
#include "UI/uiBaseInput.h"
#include "UI/uiPlaque.h"
#include "storyarc/contactclient.h"
#include "sound/sound.h"
#include <utilitieslib/language/AppLocale.h>
#include "bases/bases.h"
#include "gamedata/costume_critter.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/StashTable.h>
#include "UI/uiLogin.h"
#include "UI/uiServerTransfer.h"
#include "UI/uiContactDialog.h"
#include "entity/attrib_description.h"
#include "UI/uiOptions.h"
#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcClient.h"
#include "uiLevelingpact.h"
#include "UI/uiMissionReview.h"
#include "UI/uiClue.h"
#include "UI/uiMissionSearch.h"
#include "UI/uiRecipeInventory.h"
#include "uiMissionComment.h"
#include "UI/uiTeam.h"
#include "uiLeague.h"
#include "UI/uiPopHelp.h"
#include "UI/uiAuction.h"
#include "UI/uiUtilMenu.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "account/AccountData.h"
#include <utilitieslib/language/MultiMessageStore.h>
#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/Hybrid/uiBody.h"
#include "UI/uiGame.h"
#include "uiSalvageOpen.h"
#include "UI/Hybrid/uiWebStoreFrame.h"
#include <utilitieslib/utils/log.h>
#include "uiSalvage.h"

int gSentMoTD = 0;

static int g_PrestigeDiff;
static int g_CurrentPrestige;

// Chat stuff
//-----------------------------------------------------------------------------------------------

//
//
void receiveChatMsg( Packet *pak )
{
    Vec2 position;
    int id            = pktGetBitsPack(pak, 10);
    int type        = pktGetBitsPack(pak, 3);
    float duration  = pktGetF32(pak);
    char *s            = pktGetString(pak);
    position[0]        = pktGetF32(pak);
    position[1]        = pktGetF32(pak);
     addSystemChatMsgEx(s, type, duration, id, position);
}

//
//
//
void receiveConsoleOutput( Packet *pak )
{
    char    tmp[1024];

    strcpy( tmp, pktGetString( pak ) );
    printf( tmp );
}

//
//
//
void receiveConPrintf( Packet *pak )
{
    conPrintf("%s", pktGetString( pak ) );
}


//------------------------------------------------------------------------------------------------------
// uiDock
//------------------------------------------------------------------------------------------------------

void sendDockMode( int mode)
{
    START_INPUT_PACKET( pak, CLIENTINP_DOCK_MODE );
    pktSendBits( pak, 32, mode );
    END_INPUT_PACKET
}

void receiveDockMode( Packet *pak )
{
    int foo = pktGetBits( pak, 32 );
}

//------------------------------------------------------------------------------------------------------
// uiInspiration
//------------------------------------------------------------------------------------------------------

void sendInspirationMode( int mode )
{
    START_INPUT_PACKET(pak, CLIENTINP_INSPTRAY_MODE);
    pktSendBits( pak, 32, mode );
    END_INPUT_PACKET
}

void receiveInspirationMode( Packet *pak )
{
    inspiration_setMode( pktGetBits(pak, 32) );
}

//------------------------------------------------------------------------------------------------------
// uiTray
//------------------------------------------------------------------------------------------------------

void sendTrayMode( int mode )
{
    START_INPUT_PACKET(pak, CLIENTINP_TRAY_MODE );
    pktSendBits( pak, NUM_CURTRAYTYPE_BITS, mode );
    END_INPUT_PACKET
}

void receiveTrayMode( Packet *pak )
{
    //----------
    // get the mode bits

    // backwards compat hack: need to get the bits seperately and combine them
    int mode = pktGetBits(pak, 1);
    int mode2 = pktGetBits(pak, 1 );
    assert( NUM_CURTRAYTYPE_BITS == 2 );
    assert( !(mode & ~1) && !(mode2 & ~1));

    // combine the bits
    mode = mode | mode2 << 1;
    
    tray_setMode( mode );
}

void receiveTrayObjAdd( Packet *pak )
{
    int iset = pktGetBitsPack( pak, 1 );
    int ipow = pktGetBitsPack( pak, 1 );
    TrayObj to;

    buildPowerTrayObj( &to, iset, ipow, 0 );
    tray_AddObj( &to );
}

//------------------------------------------------------------------------------------------------------
// uiChat
//------------------------------------------------------------------------------------------------------

void sendChatDivider(float Chat_Divider)
{
    START_INPUT_PACKET(pak, CLIENTINP_CHAT_DIVIDER);
    pktSendF32( pak, Chat_Divider );
    END_INPUT_PACKET
}

void receivePlaqueDialog(Packet* pak)
{
    char* plaqueString = pktGetString(pak);
    plaquePopup(plaqueString,0);
}

//------------------------------------------------------------------------------------------------------
// uiDialog
//------------------------------------------------------------------------------------------------------
void receiveDialog( Packet *pak )
{
    char *pch = pktGetString( pak );

    dialogStd( DIALOG_OK, pch, NULL, NULL, NULL, NULL, 0 );
}

void receiveDialogWithWidth( Packet *pak )
{
    char *pch = pktGetString( pak );
    float wd = pktGetF32( pak );

    dialogStdWidth( DIALOG_OK, pch, NULL, NULL, NULL, NULL, 0, wd );
}

void receiveDialogNag( Packet *pak )
{
    char *pch = pktGetString( pak );
    int count = pktGetBitsPack(pak,4);

    dialogTimed( DIALOG_OK, pch, NULL, NULL, NULL, NULL, DLGFLAG_NAG,count );
}

static DialogCheckbox IgnorableCB[] = { { "HideCoopPrompt", 0, kUO_HideCoopPrompt } };
void receiveDialogIgnorable( Packet *pak )
{
    char *pch = pktGetString( pak );
    int type = pktGetBitsPack(pak, 16);

    if (pch && type < ARRAY_SIZE_CHECKED(IgnorableCB) && optionGet(IgnorableCB[type].optionID) == 0 )
    {
        dialog( DIALOG_OK, -1, -1, -1, -1, pch, NULL, NULL, NULL, NULL, 0, 
                sendOptions, &(IgnorableCB[type]), 1, 0, 0, 0 );
    }
}


static int gLastPowReqId = 0;

static void acceptPowReq(void *data)
{
    START_INPUT_PACKET(pak, CLIENTINP_ACCEPT_POWREQ );
    pktSendBitsPack(pak, 16, gLastPowReqId );
    pktSendBits( pak, 1, 1 );
    END_INPUT_PACKET
    gLastPowReqId = 0;
}

static void declinePowReq(void *data)
{
    START_INPUT_PACKET(pak, CLIENTINP_ACCEPT_POWREQ );
    pktSendBitsPack(pak, 16, gLastPowReqId );
    pktSendBits( pak, 1, 0 );
    END_INPUT_PACKET
    gLastPowReqId = 0;
}

void receiveDialogPowerRequest( Packet *pak )
{
    char *str    = pktGetString( pak );
    int id        = pktGetBitsPack( pak, 16 );
    int timeout    = pktGetBitsPack( pak, 10 );

    if( !gLastPowReqId && id )
    {
        gLastPowReqId = id;
        dialogTimed( DIALOG_ACCEPT_CANCEL, str, NULL, NULL, acceptPowReq, declinePowReq, DLGFLAG_GAME_ONLY | DLGFLAG_NO_TRANSLATE, timeout );
    }
}

int gLevelAccepted;

static void sendTeamLevelAccept(void * unused)
{
    START_INPUT_PACKET(pak, CLIENTINP_ACCEPT_TEAMLEVEL );
    pktSendBitsAuto(pak, gLevelAccepted );
    END_INPUT_PACKET

}

void receiveTeamChangeDialog( Packet *pak )
{
    int teamLevel = pktGetBitsAuto( pak );
    int yourLevel = pktGetBitsAuto( pak );
    gLevelAccepted = teamLevel;

    // remove existing dialog
    dialogRemove( 0, sendTeamLevelAccept, team_Quit ); 

    // update with new timer/level
    dialogTimed( DIALOG_TWO_RESPONSE, textStd("TeamLevelChange", teamLevel+1, yourLevel+1, optionGet(kUO_TeamLevelAbove), optionGet(kUO_TeamLevelBelow) ), "QuitTeamString", "AcceptNow", team_Quit, sendTeamLevelAccept, 1, 30 );
    
}


//------------------------------------------------------------------------------------------------------
// supergroup - teams stuff
//------------------------------------------------------------------------------------------------------

int teamOfferingDbId = 0; // stored as a global because of dialog system
char offererName[64];

enum // reasions why a person would be busy and unable to accept a team offer
{
    TEAMBUSY_ANOTHER_OFFER,
    TEAMBUSY_ALREADY_ON_TEAM,
    TEAMBUSY_BUSY,
};

DialogCheckbox teamInviteDCB[] = { { "DeclineTeamGifts", 0, kUO_DeclineTeamGifts },
                                   { "PromptTeamTeleport", 0, kUO_PromptTeamTeleport },};

// receive a teamup offer
void receiveTeamOffer(Packet *pak )
{
    int        offerer;
    char    *temp_name;
    int        type;

    offerer = pktGetBits( pak, 32 );
    temp_name = strdup(pktGetString( pak ) );
    type = pktGetBits( pak, 2 );

    // TODO teamup offer can't interupt trading or contacts
    // check to see if they are already on a team?

    // if somone has already offered, send a busy signal back
    if ( teamOfferingDbId )
    {
        assert(!"Recieved a TeamOffer while we were already considering one!  Server-side check gone awry?");
        free(temp_name);
        return;
    }
    teamOfferingDbId = offerer;
    strcpy(offererName, temp_name);
      free(temp_name);

    // otherwise display a dialog
    if ( type == 1 )
    {
        dialog( DIALOG_YES_NO, -1, -1, -1, -1, textStd( "OfferTeamupWithMission", offererName ), NULL, sendTeamAccept, NULL, sendTeamDecline, 
               DLGFLAG_GAME_ONLY, sendOptions, teamInviteDCB, 2, 0, 0, 0 );
    }
    else if( type == 2 )
    {
        teamOfferingDbId = 0;
        dialogStd( DIALOG_OK, textStd( "YouMustLeaveMission", offererName ), NULL, NULL, NULL, NULL, DLGFLAG_GAME_ONLY );
    }
    else
    {
        dialog( DIALOG_YES_NO, -1, -1, -1, -1, textStd( "OfferTeamup", offererName ), NULL, sendTeamAccept, NULL, sendTeamDecline, 
                DLGFLAG_GAME_ONLY, sendOptions, teamInviteDCB, 2, 0, 0, 0 );
    }
}

static char taskforce_kick_name[MAX_NAME_LEN];
static int taskforce_kickid;

static void sendTaskforceKick(void *data)
{
    char msg[64];
    sprintf( msg, "team_kick_internal %i %s", taskforce_kickid, taskforce_kick_name );
    cmdParse( msg );
}

static void sendTaskforceQuit(void *data)
{
    cmdParse( "team_quit_internal" );
}


void receiveTaskforceKick(Packet *pak )
{
    char *name = strdup(pktGetString( pak ));
    int isEndGameRaid;
    strncpy( taskforce_kick_name, name,  MAX_NAME_LEN);
    taskforce_kickid = pktGetBits( pak, 32 );
    isEndGameRaid = pktGetBits(pak, 1);
    dialogStd( DIALOG_YES_NO, textStd(isEndGameRaid ? "IncarnateTrialKick" : "TaskforceKick", taskforce_kick_name), NULL, NULL, sendTaskforceKick, NULL, 1 );

    free(name);
}

void receiveTaskforceQuit(Packet *pak )
{
    dialogStd( DIALOG_YES_NO, "TaskforceQuit", NULL, NULL, sendTaskforceQuit, NULL, 1 );
}

// Leveling Pacts //////////////////////////////////////////////////////////////

static int s_levelingpact_inviter_dbid;

static void sendLevelingPactAccept(void*data)
{
    char buf[128];
    sprintf(buf, "levelingpact_accept %d", s_levelingpact_inviter_dbid);
    cmdParse(buf);
    //show the leveling pact tab
#ifndef TEST_CLIENT
    levelingpact_openWindow(NULL);
#endif
}

static void sendLevelingPactDecline(void*data)
{
    char buf[128];
    sprintf(buf, "levelingpact_decline %d \"%s", s_levelingpact_inviter_dbid, playerPtr()->name);
    cmdParse(buf);
}

void receiveLevelingPactInvite(Packet *pak_in)
{
    char buf[16];
    char *inviter_name;
    s_levelingpact_inviter_dbid = pktGetBitsAuto(pak_in);
    inviter_name = pktGetString(pak_in);
    sprintf(buf, "%d", LEVELINGPACT_MAXLEVEL);
    dialog(    DIALOG_YES_NO, -1, -1, -1, -1, textStd("LevelingPactInvite", inviter_name, buf),
            NULL, sendLevelingPactAccept, NULL, sendLevelingPactDecline, 
            DLGFLAG_GAME_ONLY, NULL, NULL, 0, 0, 0, 0 );
}

// receive a teamup offer
void receiveLeagueOffer(Packet *pak )
{
    int        offerer;
    char    *temp_name;
    int        type;

    offerer = pktGetBits( pak, 32 );
    temp_name = strdup(pktGetString( pak ) );
    type = pktGetBits( pak, 2 );

    // TODO teamup offer can't interupt trading or contacts
    // check to see if they are already on a team?

    // if somone has already offered, send a busy signal back
    if ( teamOfferingDbId )
    {
        assert(!"Recieved a LeagueOffer while we were already considering one!  Server-side check gone awry?");
        free(temp_name);
        return;
    }
    teamOfferingDbId = offerer;
    strcpy(offererName, temp_name);
    free(temp_name);

    // otherwise display a dialog
    if ( type == 1 )
    {
        dialog( DIALOG_YES_NO, -1, -1, -1, -1, textStd( "OfferLeagueupWithMission", offererName ), NULL, sendLeagueAccept, NULL, sendLeagueDecline, 
            DLGFLAG_GAME_ONLY, NULL, NULL, 0, 0, 0, 0 );
    }
    else if( type == 2 )
    {
        teamOfferingDbId = 0;
        dialogStd( DIALOG_OK, textStd( "YouMustLeaveMission", offererName ), NULL, NULL, NULL, NULL, DLGFLAG_GAME_ONLY );
    }
    else
    {
        dialog( DIALOG_YES_NO, -1, -1, -1, -1, textStd( "OfferLeagueup", offererName ), NULL, sendLeagueAccept, NULL, sendLeagueDecline, 
            DLGFLAG_GAME_ONLY, NULL, NULL, 0, 0, 0, 0 );
    }
}

static char league_kick_name[MAX_NAME_LEN];
static int league_kickid;

static void sendLeagueTeamKick(void *data)
{
    char msg[64];
    sprintf( msg, "league_kick_internal %i 1 %s", league_kickid, league_kick_name );
    cmdParse( msg );
}
static void sendLeagueERQuit(void *data)
{
    cmdParse("team_quit_internal");
}
void recieveLeagueTeamKick(Packet *pak )
{
    char *name = strdup(pktGetString( pak ));
    int isTeamKick;
    strncpy( league_kick_name, name, MAX_NAME_LEN);
    league_kickid = pktGetBits( pak, 32 );
    isTeamKick = pktGetBits(pak, 1);
    dialogStd( DIALOG_YES_NO, textStd(isTeamKick ? "LeagueTeamKick" : "LeagueConfirmERKick", league_kick_name), NULL, NULL, sendLeagueTeamKick, NULL, 1 );
    free(name);
}

void recieveLeagueERQuit(Packet *pak)
{
    dialogStd( DIALOG_YES_NO, textStd("LeagueConfirmERQuit"), NULL, NULL, sendLeagueERQuit, NULL, 1 );
}

void receiveServerDialog(Packet *pak_in)
{
    char *text = pktGetString(pak_in);
    dialogStd(DIALOG_OK,text,NULL,NULL,NULL,NULL,0);
}

static char *s_moralChoice_leftText = 0;
static char *s_moralChoice_rightText = 0;
static char *s_moralChoice_leftWatermark = 0;
static char *s_moralChoice_rightWatermark = 0;
static int s_moralChoice_requireConfirmation;

void moralChoiceDisplayChoice(char *data);

void moralChoiceDecisionSendLeft(char *data)
{
    START_INPUT_PACKET(pak, CLIENTINP_MORAL_CHOICE_RESPONSE);
    pktSendBitsPack(pak, 1, 1);
    END_INPUT_PACKET
}

void moralChoiceDecisionLeft(char *data)
{
    if (!s_moralChoice_requireConfirmation)
    {
        moralChoiceDecisionSendLeft(data);
    }
    else
    {
        // Isn't DIALOG_ACCEPT_CANCEL because the CANCEL option gets run when you log out,
        // which would display the Moral Choice dialog on the login/character select screen.
        dialogStd(DIALOG_TWO_RESPONSE,s_moralChoice_leftText,"AcceptString","CancelString",
            moralChoiceDecisionSendLeft,moralChoiceDisplayChoice,0);        
    }
}

void moralChoiceDecisionSendRight(char *data)
{
    START_INPUT_PACKET(pak, CLIENTINP_MORAL_CHOICE_RESPONSE);
    pktSendBitsPack(pak, 1, 2);
    END_INPUT_PACKET
}

void moralChoiceDecisionRight(char *data)
{
    if (!s_moralChoice_requireConfirmation)
    {
        moralChoiceDecisionSendRight(data);
    }
    else
    {
        // Isn't DIALOG_ACCEPT_CANCEL because the CANCEL option gets run when you log out,
        // which would display the Moral Choice dialog on the login/character select screen.
        dialogStd(DIALOG_TWO_RESPONSE,s_moralChoice_rightText,"AcceptString","CancelString",
            moralChoiceDecisionSendRight,moralChoiceDisplayChoice,0);        
    }
}

void moralChoiceDisplayChoice(char *data)
{
#ifdef TEST_CLIENT
    dialogTimed(DIALOG_TWO_RESPONSE,s_moralChoice_leftText,NULL,NULL,
        moralChoiceDecisionLeft,moralChoiceDecisionRight,0,0);
#else
    if (s_moralChoice_leftText && s_moralChoice_leftText[0])
    {
        dialogMoralChoice(s_moralChoice_leftText, s_moralChoice_rightText,
            moralChoiceDecisionLeft, moralChoiceDecisionRight, 
            s_moralChoice_leftWatermark, s_moralChoice_rightWatermark);
    }
#endif
}

void receiveMoralChoice(Packet *pak_in)
{
    int llen, rlen;

    if (s_moralChoice_leftText) free(s_moralChoice_leftText);
    if (s_moralChoice_rightText) free(s_moralChoice_rightText);
    if (s_moralChoice_leftWatermark) free(s_moralChoice_leftWatermark);
    if (s_moralChoice_rightWatermark) free(s_moralChoice_rightWatermark);

    s_moralChoice_leftText = strdup(pktGetStringAndLength(pak_in, &llen));
    s_moralChoice_rightText = strdup(pktGetStringAndLength(pak_in, &rlen));
    s_moralChoice_leftWatermark = strdup(pktGetString(pak_in));
    s_moralChoice_rightWatermark = strdup(pktGetString(pak_in));
    s_moralChoice_requireConfirmation = pktGetBitsAuto(pak_in);

    dialogClearQueue( 0 );
    moralChoiceDisplayChoice(0);
}

//
//This receives a command from the server to cause a pop_help dialog to happen.
// intended for debug purposes.
//
void receivePopHelpTag(Packet *pak_in)
{
    char *tag;
    int tag_len;

    tag = pktGetStringAndLength(pak_in, &tag_len);
#ifndef TEST_CLIENT
    popHelpEventHappenedByTag(tag);
#endif
}

void receivePopHelpN(Packet *pak_in)
{
    int pop_help_n;

    pop_help_n = pktGetBits( pak_in, 32 );
#ifndef TEST_CLIENT
    popHelpEventHappened(pop_help_n);
#endif
}


void receivePopHelpReset(Packet *pak_in)
{
#ifndef TEST_CLIENT
    resetPopHelp();
#endif
}

void receivePopHelpEventHappenedByTag(Packet *pak)
{
    char *tag = pktGetString(pak);

#ifndef TEST_CLIENT
    popHelpEventHappenedByTag(tag);
#endif
}


void receiveRewardToken( Packet * pak )
{
    Entity *e = playerPtr();
    char *token = pktGetString(pak);
    int val = pktGetBitsAuto(pak);
    int activePlayer = pktGetBitsAuto(pak);

    if( e && e->pl )
    {
        int i;
        if (activePlayer)
        {
            i = rewardtoken_Award(&e->pl->activePlayerRewardTokens, token, 0);
            assert(i >= 0);
            e->pl->activePlayerRewardTokens[i]->val = val;
        }
        else
        {
            i = rewardtoken_Award(&e->pl->rewardTokens, token, 0);
            assert(i >= 0);
            e->pl->rewardTokens[i]->val = val;
        }
    }
}

void sendGetSalvageImmediateUseStatus( const char* salvageName )
{
    START_INPUT_PACKET( pak, CLIENTINP_GET_SALVAGE_IMMEDIATE_USE_STATUS )
    pktSendString(pak, salvageName );
    END_INPUT_PACKET
}

void receiveSalvageImmediateUseStatus(Packet* pak)
{
    Entity *e            = playerPtr();
    char *salvageName    = NULL;
    U32 flags            = 0;
    char* uiMsgStr        = NULL;
    
    strdup_alloca( salvageName, pktGetString(pak) );
    flags = pktGetBitsAuto(pak);
    strdup_alloca( uiMsgStr, pktGetString(pak) );
    
    uiSalvage_ReceiveSalvageImmediateUseResp( salvageName, flags, uiMsgStr );
}

void localizeCombatMessage(char *outputMessage, int bufferLength, const char* messageID, ...)
{
    static Array* CombatMessage = 0;
    int messageLength;

    // If no messages are given, do nothing.
    if (!messageID || messageID[0] == '\0')
        return;

    if(!CombatMessage)
    { 
        CombatMessage = msCompileMessageType("{PlayerDest, %s}, {PowerName, %r}, {Damage, %g}, {PlayerSource, %s}");
    }

    VA_START(va, messageID);
    messageLength = msvaPrintfInternalEx(menuMessages, outputMessage, bufferLength, messageID, CombatMessage, NULL, 0, 0, va);
    VA_END();
}

void receiveCombatMessage(Packet *pak)
{
    Entity *e = playerPtr();
    int chatType = pktGetBitsAuto(pak);
    CombatMessageType messageType = pktGetBitsAuto(pak);
    int isVictimMessage = pktGetBits(pak, 1);
    int powerCRC = pktGetBitsAuto(pak);
    int modIndex = pktGetBitsAuto(pak);
    char *otherEntityName = strdup(pktGetString(pak));
    float float1 = pktGetF32(pak);
    float float2 = pktGetF32(pak);
    char *petName = 0;
    const BasePower *power = powerdict_GetBasePowerByCRC(&g_PowerDictionary, powerCRC);
    char message[1024] = {0};
    char *finalMessage;
    char *thisEntityName = e->name;

    if (pktGetBits(pak, 1))
    {
        petName = strdup(pktGetString(pak));
        thisEntityName = petName;
    }

    if (messageType == CombatMessageType_DamagingMod)
    {
        const AttribModTemplate *mod = power->ppTemplates[modIndex];
        if (isVictimMessage)
        {
            localizeCombatMessage(message, ARRAY_SIZE(message), mod->pchDisplayVictimHit,
                thisEntityName,
                power->pchDisplayName,
                float1,
                otherEntityName);
        }
        else
        {
            localizeCombatMessage(message, ARRAY_SIZE(message), mod->pchDisplayAttackerHit,
                otherEntityName,
                power->pchDisplayName,
                float1,
                thisEntityName);
        }
    }
    else if (messageType == CombatMessageType_PowerActivation)
    {
        if (isVictimMessage)
        {
            localizeCombatMessage(message, ARRAY_SIZE(message), power->pchDisplayVictimHit,
                thisEntityName,
                power->pchDisplayName,
                0.f,
                otherEntityName);
        }
        else
        {
            localizeCombatMessage(message, ARRAY_SIZE(message), power->pchDisplayAttackerHit,
                otherEntityName,
                power->pchDisplayName,
                0.f,
                thisEntityName);
        }
    }
    else if (messageType == CombatMessageType_PowerHitRoll)
    {
        float fToHit = float1;
        float fRoll = float2;

        if (isVictimMessage)
        {
            if (float2 > 2.5f) // == 3.0f
            {
                strcpy_s(message, ARRAY_SIZE(message), textStd("EnemyForceHit", otherEntityName, power->pchDisplayName));
            }
            else if (float2 > 1.5f) // == 2.0f
            {
                strcpy_s(message, ARRAY_SIZE(message), textStd("EnemyAutoHit", otherEntityName, power->pchDisplayName));
            }
            else if(fRoll < fToHit)
            {
                strcpy_s(message, ARRAY_SIZE(message), textStd("EnemyPowerHit", otherEntityName, power->pchDisplayName, fToHit*100, fRoll*100));    
            }
            else
            {
                strcpy_s(message, ARRAY_SIZE(message), textStd("EnemyPowerMiss", otherEntityName, power->pchDisplayName, fToHit*100, fRoll*100));    
            }
        }
        else
        {
            if (float2 > 2.5f) // == 3.0f
            {
                strcpy_s(message, ARRAY_SIZE(message), textStd("PlayerForceHit", otherEntityName, power->pchDisplayName));
            }
            else if (float2 > 1.5f) // == 2.0f
            {
                strcpy_s(message, ARRAY_SIZE(message), textStd("PlayerAutoHit", otherEntityName, power->pchDisplayName));
            }
            else if(fRoll < fToHit)
            {
                strcpy_s(message, ARRAY_SIZE(message), textStd("PlayerPowerHit", otherEntityName, power->pchDisplayName, fToHit*100, fRoll*100));    
            }
            else
            {
                strcpy_s(message, ARRAY_SIZE(message), textStd("PlayerPowerMiss", otherEntityName, power->pchDisplayName, fToHit*100, fRoll*100));    
            }
        }
    }

    if (strlen(message) > 0)
    {
        estrCreate(&finalMessage);
        if (petName)
        {
            estrConcatf(&finalMessage, "%s %s", textStd("PetCombatMsg", petName), message);
        }
        else
        {
            estrConcatCharString(&finalMessage, message);
        }
        InfoBox(chatType, "%s", finalMessage);
        estrDestroy(&finalMessage);
    }

    free(otherEntityName);
    if (petName)
    {
        free(petName);
    }
}

void levelingpactListUiUpdate()
{
#ifndef TEST_CLIENT
    levelingpactListPrepareUpdate();
#endif
}

////////////////////////////////////////////////////////////////////////////////

void sgroup_sendCreate(    Supergroup* sg)
{
    int i;
    START_INPUT_PACKET(pak, CLIENTINP_SGROUP_CREATE);
    pktSendString( pak, sg->name );
    pktSendString( pak, sg->emblem );
    pktSendBits( pak, 32, sg->colors[0] );
    pktSendBits( pak, 32, sg->colors[1] );
    pktSendString( pak, sg->description );
    pktSendString( pak, sg->motto );
    pktSendString( pak, sg->msg );
    pktSendBits( pak, 32, sg->tithe );
    pktSendBits( pak, 32, sg->demoteTimeout );
    for(i = 0; i < NUM_SG_RANKS; i++)
    {
        pktSendString( pak, sg->rankList[i].name );
        pktSendBitsArray( pak, sizeof(sg->rankList[i].permissionInt) * 8, sg->rankList[i].permissionInt );
    }
    pktSendBitsAuto( pak, sg->entryPermission );
    
    END_INPUT_PACKET
}

void sgroup_requestCostume(void)
{
    EMPTY_INPUT_PACKET(CLIENTINP_SGROUP_COSTUMEREQ);
}

// receive a teamup offer
void receiveSuperGroupOffer(Packet *pak )
{
    int offerer;
    char *temp_name;

    offerer = pktGetBits( pak, 32 );
    temp_name = strdup(pktGetString( pak ) );

    // TODO teamup offer can't interupt trading or contacts
    // check to see if they are already on a team?

    // if somone has already offered, send a busy signal back
    if ( teamOfferingDbId )
    {
        assert(!"Recieved a TeamOffer while we were already considering one!  Server-side check gone awry?");
        free(temp_name);
        return;
    }
    teamOfferingDbId = offerer;
    strcpy(offererName, temp_name);
    free(temp_name);

    // otherwise display a dialog
    dialogStd( DIALOG_YES_NO, textStd("OfferSuperGroup", offererName), NULL, NULL, sendSuperGroupAccept, sendSuperGroupDecline, 1 );
}


void receiveSuperResponse(Packet *pak)
{
    Entity *e = playerPtr();
    int result;
    result = pktGetBits(pak, 1);
    srReportSGCreationResult(result);

     if( result )
    {
        if( !gSuperCostume )
            gSuperCostume = costume_create(GetBodyPartCount());

        costume_receive( pak, gSuperCostume );
    }
}

void receiveSuperCostume(Packet *pak)
{
    Entity *e = playerPtr();
    costume_receive( pak, e->pl->superCostume );
}

void sendHideEmblemChange(int hide)
{
    START_INPUT_PACKET( pak, CLIENTINP_HIDE_EMBLEM_UPDATE )
    pktSendBits(pak, 1, hide );
    END_INPUT_PACKET
}

void receiveHideEmblemChange(Packet *pak)
{
    Entity *e = playerPtr();
    costume_recieveEmblem(pak, e, gSuperCostume );
}


//-----------------------------------------------------------------------------
//
//

static int offererDBID = 0;

static void sendAllianceAccept(void *data)
{
    char buf[128];

    // send offerer's name and offerer's supergroup dbid
    sprintf( buf, "coalition_accept \"%s\" %d %d", offererName, offererDBID, teamOfferingDbId);
    cmdParse( buf );

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
    offererDBID = 0;
}

//
//
static void sendAllianceDecline(void *data)
{
    char buf[128];

    // send offerer's name and offerer's supergroup dbid
    sprintf( buf, "coalition_decline \"%s\" %d \"%s\"", offererName, offererDBID, playerPtr()->name);
    cmdParse( buf );

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
    offererDBID = 0;
}

// receive an alliance offer
void receiveAllianceOffer(Packet *pak )
{
    int        offerer;
    char    *offerer_name, *offerer_sg_name;

    offererDBID = pktGetBits(pak, 32);
    offerer = pktGetBits(pak, 32);
    offerer_name = strdup(pktGetString(pak));
    offerer_sg_name = pktGetString(pak);

    // if somone has already offered, send a busy signal back
    if ( teamOfferingDbId )
    {
        assert(!"Recieved a TeamOffer while we were already considering one!  Server-side check gone awry?");
        free(offerer_name);
        return;
    }
    teamOfferingDbId = offerer;
    strcpy(offererName, offerer_name);
      free(offerer_name);

    dialog( DIALOG_YES_NO, -1, -1, -1, -1, textStd( "OfferAlliance", offererName, offerer_sg_name ), NULL, sendAllianceAccept, NULL, sendAllianceDecline, 
            DLGFLAG_GAME_ONLY, sendOptions, NULL, 2, 0, 0, 0 );
}

// Register
void receiveRegisterSupergroup(Packet *pak)
{
    char *pchName;

    // This is the name of the NPC. We should eventually hook this up so that
    // it's sent to the server when the register happens as a verification
    // that they're near a registrar.
    pchName = pktGetString(pak);

    srEnterRegistrationScreen();
}

void receiveTeamBuffMode( Packet *pak )
{
    setTeamBuffMode( pktGetBits( pak, 1 ) );
}

void sendTeamBuffMode( int mode )
{
    char buf[32];
    sprintf( buf, "buffs %i", mode );
    cmdParse( buf );
}

//-----------------------------------------------------------------------------
//
//
void sendTeamAccept(void *data)
{
    char buf[128];

    sprintf( buf, "team_accept \"%s\" %i %i \"%s\" %i", offererName, teamOfferingDbId, playerPtr()->db_id, playerPtr()->name, playerPtr()->pl->pvpSwitch );
    cmdParse( buf );

    // Force up the team window
    window_setMode( WDW_GROUP, WINDOW_GROWING );

    if( getCurrentLocale() == LOCALE_ID_KOREAN )
        sndPlay( "COH_25", SOUND_GAME );

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
    playerPtr()->pl->lfg = 0;
}

//
//
void sendTeamDecline(void *data)
{
    char buf[128];

    if(offererName[0] && playerPtr() && playerPtr()->name)
    {
        sprintf( buf, "team_decline \"%s\" %i \"%s\"", offererName, teamOfferingDbId, playerPtr()->name );
        cmdParse( buf );
    }

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
}

void sendLeagueAccept(void *data)
{
    char buf[128];

    sprintf( buf, "league_accept \"%s\" %i %i \"%s\" %i", offererName, teamOfferingDbId, playerPtr()->db_id, playerPtr()->name, playerPtr()->pl->pvpSwitch );
    cmdParse( buf );

    // Force up the team window
    window_setMode( WDW_LEAGUE, WINDOW_GROWING );

    if( getCurrentLocale() == LOCALE_ID_KOREAN )
        sndPlay( "COH_25", SOUND_GAME );

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
    playerPtr()->pl->lfg = 0;
}

void sendLeagueDecline(void *data)
{
    char buf[128];

    if(offererName[0] && playerPtr() && playerPtr()->name)
    {
        sprintf( buf, "league_decline \"%s\" %i \"%s\"", offererName, teamOfferingDbId, playerPtr()->name );
        cmdParse( buf );
    }

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
}

//-----------------------------------------------------------------------------
//
//
void sendSuperGroupAccept(void *data)
{
    char buf[128];

    sprintf( buf, "sg_accept \"%s\" %i %i \"%s\"", offererName, teamOfferingDbId, playerPtr()->db_id, playerPtr()->name );
    cmdParse( buf );

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
}

//
//
void sendSuperGroupDecline(void *data)
{
    char buf[128];

    sprintf( buf, "sg_decline \"%s\" %i \"%s\"", offererName, teamOfferingDbId, playerPtr()->name );
    cmdParse( buf );

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
}


//-----------------------------------------------------------------------------
//
//
void sendTeamKick( int db_id )
{
    START_INPUT_PACKET(pak, CLIENTINP_TEAM_KICK );
    pktSendBits( pak, 32, db_id );
    END_INPUT_PACKET
}

static void recieveMemberCommon(Packet *pak, TeamMembers *members, int i, MapName *names)
{
    members->ids[i] = pktGetBits(pak, 32 );

    if( !pktGetBits(pak, 1) ) // if they are not on the mapserver, get their name at least
    {
        members->onMapserver[i] = FALSE;
        if( pktGetBits(pak, 1) )
        {
            strcpy( members->names[i], pktGetString( pak ));
            strcpy( names[i], pktGetString( pak ) );
        }
    }
    else
    {
        Entity *eMate = entFromDbId(members->ids[i]) ;
        if(eMate)
        {
            strcpy( members->names[i], eMate->name);
            members->onMapserver[i] = TRUE;
        }
        else
        {
            members->onMapserver[i] = FALSE;
        }
    }
}
//
//
void receiveTeamList( Packet * pak, Entity *e )
{
    int i;
    // Be sure to update this right before returning from this function!
    static int oldActivePlayerDbid = 0;

    e->teamup_id = pktGetIfSetBitsPack( pak, PKT_BITS_TO_REP_DB_ID);
    e->pl->taskforce_mode = pktGetBits( pak, 2);
    e->pl->lfg = pktGetBits( pak, 10);

    if( !e->teamup_id ) // if there is no teamup
    {
        // free the team struct if there is one
        if( e->teamup )
        {
            destroyTeamup( e->teamup );
            e->teamup = 0;
        }
        oldActivePlayerDbid = 0;
        return;
    }

    // otherwise we do have a team, make sure we have a place for it
    if ( !e->teamup )
    {
        e->teamup = createTeamup();

        // should these allocations be moved into createTeamup()?
        // the members structure is used on both client and map,
        // but these allocations within the structure only happen on the client... ARM
        e->teamup->members.ids            = calloc( MAX_TEAM_MEMBERS, sizeof(int) );
        e->teamup->members.onMapserver  = calloc( MAX_TEAM_MEMBERS, sizeof(int) );
        e->teamup->members.names        = calloc( MAX_TEAM_MEMBERS, sizeof(MemberName) );
    }

    e->teamup->members.leader = pktGetBitsPack( pak, 32 );
    e->teamup->members.count  = pktGetBitsPack( pak, 1 );

    e->teamup->team_level = pktGetBitsAuto(pak);
    e->teamup->team_mentor = pktGetBitsAuto(pak);
    e->teamup->activePlayerDbid = pktGetBitsAuto(pak);

    if (e->teamup->members.count > MAX_TEAM_MEMBERS)
    {
        assert(!"You are on a team with more than max members, something has gone wrong, or you cheat.");
    }

    if( e->teamup->members.count > 1 )
        e->pl->lfg = 0;

    for( i = 0; i < e->teamup->members.count; i++ )
    {
        recieveMemberCommon(pak, &e->teamup->members, i, teamMapNames);
    }

    if (oldActivePlayerDbid != e->teamup->activePlayerDbid)
    {
        if (e->teamup->members.count > 1)
        {
            int memberIndex;
            for (memberIndex = e->teamup->members.count - 1; memberIndex >= 0; memberIndex--)
            {
                if (e->teamup->members.ids[memberIndex] == e->teamup->activePlayerDbid)
                {
                    if (e->teamup->activePlayerDbid == e->db_id)
                    {
                        addSystemChatMsg(textStd("YouAreActivePlayer"), INFO_SVR_COM, 0);
                    }
                    else
                    {
                        addSystemChatMsg(textStd("TeammateIsActivePlayer", e->teamup->members.names[memberIndex]), INFO_SVR_COM, 0);
                    }
                }
            }
        }
    }

    oldActivePlayerDbid = e->teamup->activePlayerDbid;
}

// sgroup_sendList
void receiveSuperStats(Packet *pak, Entity *e)
{
    int i, count;
    int previousPrestige = 0;
    Supergroup* sg;

    count = pktGetBitsPack(pak, 1 );

    if( !e->supergroup && count ) {
        e->supergroup = createSupergroup();
    }

    sg = e->supergroup;

    // Have the supergroup UI update its list.
    sgListPrepareUpdate();

    if( !count )
    {
        if( creatingSupergroup() && (isMenu(MENU_SUPER_REG) || isMenu(MENU_SUPERCOSTUME))  )
            return;

        destroySupergroup( e->supergroup );
        e->supergroup = 0;
        e->supergroup_id = 0;
        e->pl->supergroup_mode = 0;

        if( e->sgStats )
        {
            // Free up the old EArray
            eaDestroyEx( &e->sgStats, destroySupergroupStats );
        }

        srClearAll();

        if( isMenu( MENU_SUPERCOSTUME ) )
        {
            entSetMutableCostumeUnowned( e, costume_current(e) ); // e->costume = costume_as_const(e->pl->costume[e->pl->current_costume]);
            start_menu( MENU_GAME );
        }

        return;
    }

    previousPrestige = sg->prestige;

    sg->influence = pktGetBitsPack(pak, 1);

    sg->prestige = pktGetBitsPack(pak, 1);
    g_CurrentPrestige = sg->prestige;
    if(game_state.edit_base)
        sg->prestige -= g_PrestigeDiff;

    sg->prestigeBase = pktGetBitsPack(pak, 1);
    sg->prestigeAddedUpkeep = pktGetBitsPack(pak, 1);
    sg->upkeep.prtgRentDue = pktGetBitsPack(pak, 1);
    sg->upkeep.secsRentDueDate = pktGetBitsPack(pak, 1);
    sg->playerType = pktGetBitsPack(pak, 1);

#ifndef TEST_CLIENT
    if (sg->prestige != previousPrestige)
        recipeInventoryReparse();
#endif

    if (!pktGetBits(pak, 1)) // No update
        return;

    if( e->sgStats )
    {
        // Free up the old EArray
        eaDestroyEx( &e->sgStats, destroySupergroupStats );
    }

    if( count > 0 )
        eaCreate( &e->sgStats );

    for( i = 0; i < count; i++ )
    {
        SupergroupStats *stat = createSupergroupStats();

        strncpyt( stat->name, unescapeString(pktGetString(pak)), 32 );
        stat->db_id                = pktGetBitsPack(pak,1);
        stat->rank                = pktGetBitsPack(pak,1);
        stat->level                = pktGetBitsPack(pak,1);
        stat->last_online        = pktGetBits(pak,32);
        stat->online            = pktGetBits(pak,1);
        strncpyt( stat->arch, pktGetString(pak), 32 );
        strncpyt( stat->origin, pktGetString(pak), 32 );
        stat->member_since        = pktGetBits(pak, 32);
        stat->prestige            = pktGetBits(pak, 32);
        stat->playerType        = pktGetBitsPack(pak,1);

        if( stat->online )
            strncpyt( stat->zone, pktGetString(pak), 256 );

        eaPush( &e->sgStats, stat );

        if( stat->db_id == e->db_id )
        {
            if( !e->superstat )
                e->superstat = createSupergroupStats();
            e->superstat->rank = stat->rank;
        }
    }

    if( count > 0 )
    {
        int n;

        strcpy( sg->name,        pktGetString( pak ));
        strcpy( sg->motto,        pktGetString( pak ));
        strcpy( sg->msg,        pktGetString( pak ));
        strcpy( sg->emblem,        pktGetString( pak ));
        strcpy( sg->description, pktGetString( pak ));
        sg->colors[0]                = pktGetBitsPack( pak, 1 );
        sg->colors[1]                = pktGetBitsPack( pak, 1 );
        sg->tithe                    = pktGetBitsPack(pak, 1);
        sg->spacesForItemsOfPower    = pktGetBitsPack(pak, 1);
        sg->demoteTimeout            = pktGetBits(pak, 32);

        for (i = 0; i < NUM_SG_RANKS; i++)
        {
            strncpy(sg->rankList[i].name, pktGetString(pak), SG_TITLE_LEN);
            pktGetBitsArray( pak, sizeof(sg->rankList[i].permissionInt) * 8, sg->rankList[i].permissionInt );
        }

        // entry permissions
        sg->entryPermission = pktGetBitsAuto( pak );

        if(sg->memberranks)
            eaClearEx(&sg->memberranks, destroySupergroupMemberInfo);

        n = pktGetBitsPack(pak, 1);
        for (i = 0; i < n; i++)
        {
            SupergroupMemberInfo* member = createSupergroupMemberInfo();
            member->dbid = pktGetBitsPack(pak, PKT_BITS_TO_REP_DB_ID);
            member->rank = pktGetBitsPack(pak, 1);
            eaPush(&sg->memberranks, member);
        }

        for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
        {
            sg->allies[i].db_id = pktGetBits(pak, 32);
            if (sg->allies[i].db_id)
            {
                sg->allies[i].dontTalkTo = pktGetBits(pak, 1);
                sg->allies[i].minDisplayRank = pktGetBitsAuto(pak);
                strncpy(sg->allies[i].name, pktGetString(pak), SG_NAME_LEN);
            }
        }

        sg->alliance_minTalkRank = pktGetBitsAuto(pak);

        if( !gSentMoTD )
        {
            addSystemChatMsg(textStd( "SupergroupMOTD", e->supergroup->name, e->supergroup->msg ), INFO_SUPERGROUP_COM, 0);

            // --------------------
            // print out rent owed as well
            if( sg->upkeep.prtgRentDue > 0 )
            {
                int secsDueDate = sg->upkeep.secsRentDueDate + g_baseUpkeep.periodRent;
                char *dateStr = timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(secsDueDate);
                char *msgId = NULL;
                
                if( timerSecondsSince2000() < (U32)secsDueDate )
                {
                    msgId = "SupergroupRentDue";
                }
                else
                {
                    msgId = "SupergroupRentOverdue";
                }

                addSystemChatMsg(textStd( msgId, e->supergroup->name, e->supergroup->upkeep.prtgRentDue, dateStr ), INFO_SUPERGROUP_COM, 0);
            }
            gSentMoTD = true;
        }
    }
}

void sendSupergroupModeToggle()
{
    EMPTY_INPUT_PACKET( CLIENTINP_SUPERGROUP_TOGGLE );
}

void receiveLeagueList( Packet *pak, Entity *e )
{
    int oldLeagueId = e->league_id;
    e->league_id = pktGetIfSetBitsPack( pak, PKT_BITS_TO_REP_DB_ID);

    if( !e->league_id ) // if there is no league
    {
        // free the league struct if there is one
        if( e->league )
        {
            leagueClearFocusGroup(e);        //    we only clear here. not in destroy league because that happens on mapmoves
            destroyLeague( e->league );
            e->league = 0;
        }
        return;
    }
    else
    {
        int count, i;
        int updateTeamDisplay = 0;
        League *league;
        if ( !e->league )
        {
            e->league = createLeague();
            e->league->members.ids            = calloc( MAX_LEAGUE_MEMBERS, sizeof(int) );
            e->league->members.onMapserver  = calloc( MAX_LEAGUE_MEMBERS, sizeof(int) );
            e->league->members.names        = calloc( MAX_LEAGUE_MEMBERS, sizeof(MemberName) );
            for (i = 0; i < MAX_LEAGUE_MEMBERS; ++i)
            {
                e->league->teamLeaderIDs[i] = -1;
                e->league->lockStatus[i] = 0;
            }
        }
        league = e->league;
        league->members.leader = pktGetBitsPack(pak, 32);
        count = league->members.count;
        league->members.count = pktGetBitsPack(pak, 1);
        if (count != league->members.count)
        {
            updateTeamDisplay |= 1;
        }
        for( i = 0; i < e->league->members.count; i++ )
        {
            int oldId = league->members.ids[i];
            int oldTeamId = league->teamLeaderIDs[i];
            recieveMemberCommon(pak, &e->league->members, i, leagueMapNames);
            league->teamLeaderIDs[i] = pktGetBits(pak, 32);
            league->lockStatus[i] = pktGetBits(pak, 32);
            if ((league->teamLeaderIDs[i] != oldTeamId) || (league->members.ids[i] != oldId))
            {
                updateTeamDisplay |= 1;
            }
        }
        if (updateTeamDisplay)
        {
            leagueOrganizeTeamList(league);
            leagueCheckFocusGroup(e);
            league_updateTabCount();
        }
    }
}
// Trading
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

//
//
static void sendTradeAccept(void *data)
{
    char buf[128];

    sprintf( buf, "trade_accept \"%s\" %i %i \"%s\"", offererName, teamOfferingDbId, playerPtr()->db_id, playerPtr()->name );
    cmdParse( buf );

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
}

//
//
static void sendTradeDecline(void *data)
{
    char buf[128];

    sprintf( buf, "trade_decline \"%s\" %i \"%s\"", offererName, teamOfferingDbId, playerPtr()->name );
    cmdParse( buf );

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
}


// receive a teamup offer
void receiveTradeOffer(Packet *pak )
{
    int offerer;
    char *temp_name;

    offerer = pktGetBits( pak, 32 );
    temp_name = strdup(pktGetString( pak ) );

    // TODO teamup offer can't interupt trading or contacts
    // check to see if they are already on a team?

    // if somone has already offered, send a busy signal back
    if ( teamOfferingDbId )
    {
        assert(!"Recieved a TeamOffer while we were already considering one!  Server-side check gone awry?");
        free(temp_name);
        return;
    }
    teamOfferingDbId = offerer;
    strcpy(offererName, temp_name);
    free(temp_name);

    // otherwise display a dialog
    dialogStd( DIALOG_YES_NO, textStd("OfferTrade", offererName), NULL, NULL, sendTradeAccept, sendTradeDecline, 1 );
}

void recieveTradeInit( Packet *pak )
{
    trade_init( pktGetBitsPack( pak, 1 ) );
}

void sendTradeCancel( int reason )
{
    START_INPUT_PACKET(pak, CLIENTINP_TRADE_CANCEL );
    pktSendBitsPack( pak, 1, reason );
    END_INPUT_PACKET
}

void receiveTradeCancel( Packet *pak )
{
    trade_cancel( pktGetString(pak) );
}

void sendTradeUpdate()
{
    START_INPUT_PACKET(pak, CLIENTINP_TRADE_UPDATE );
    trade_sendUpdate( pak );
    END_INPUT_PACKET
}

void receiveTradeUpdate( Packet *pak )
{
    trade_receiveUpdate( pak );
}

void receiveTradeSuccess( Packet *pak )
{
    addSystemChatMsg( pktGetString(pak), INFO_USER_ERROR, 0 );
    trade_end();
}
// Friends stuff
//---------------------------------------------------------------------------------------

void receiveFriendsList(Packet *pak, Entity *e )
{
    int i;
    int count, full_update;
    int oo_packet = 0;
    static int last_pak_id = 0;

    friendListPrepareUpdate();


    full_update = pktGetBitsPack( pak, 1 );
    count = pktGetBitsPack( pak, 1 );

    if( full_update || (int)pak->id > last_pak_id )
        last_pak_id = pak->id;
    else
        oo_packet = TRUE;

    if(e && !oo_packet)
        e->friendCount = count;

    for( i = 0; i < count; i++ )
    {
        int tempint;
        char* tempstr;
        int online;
        const CharacterClass *pclass;
        const CharacterOrigin *porigin;

        if( !pktGetBits(pak, 1) )
            continue;

        if( e && (int)pak->id > e->friends[i].send )
            e->friends[i].send = pak->id;
        else
            oo_packet = TRUE;

        // DBID.
        tempint = pktGetBitsPack( pak, 1 );

        if(e && !oo_packet)
            e->friends[i].dbid = tempint;

        // Online.
        online = pktGetBits( pak, 1 );

        if(e && !oo_packet)
            e->friends[i].online = online;

        // Name.
        tempstr = pktGetString(pak);

        if(e && !oo_packet)
            strncpyta( &e->friends[i].name, tempstr, 32 );

        // Archetype.
        tempint = pktGetBitsPack(pak, 1);
        if( tempint >= 0 )
        {    pclass = g_CharacterClasses.ppClasses[tempint];

            if(e && !oo_packet)
                e->friends[i].arch = allocAddString( pclass->pchName );
        }

        // Origin.
        tempint = pktGetBitsPack(pak, 1);
        if( tempint >= 0 )
        {
            porigin = g_CharacterOrigins.ppOrigins[tempint];

            if(e && !oo_packet)
                e->friends[i].origin = allocAddString( porigin->pchName );
        }

        if ( online )
        {
            // Map ID.
            tempint = pktGetBitsPack( pak, 1 );

            if(e && !oo_packet)
                e->friends[i].map_id = tempint;

            // Map Name.
            tempstr = pktGetString(pak);

            if(e && !oo_packet)
                strncpyta( &e->friends[i].mapname, tempstr, 256 );
        }
    }
}

// Leveling
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void level_buyNewPower(const Power *ppow, PowerSystem ps)
{
    START_INPUT_PACKET(pak, CLIENTINP_LEVEL_BUY_POWER);
    character_SendBuyPower(pak, playerPtr()->pchar, ppow, ps);
    END_INPUT_PACKET
}

void level_increaseLevel()
{
    Character *c = playerPtr()->pchar;

    c->iBuildLevels[c->iCurBuild] = ++c->iLevel;

    character_GrantAutoIssuePowers(c);
}

// E-Mail
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void receiveEmailHeaders(Packet *pak)
{
    int        i,count,full_update,message_id,sent, auth_id,redacted;
    char    *sender,*subject;

     full_update = pktGetBitsPack(pak,1);
    emailHeaderListPrepareUpdate();

    count = pktGetBitsPack(pak,1);
    for(i=0;i<count;i++)
    {
        if( pktGetBits( pak, 1 ) )
        {
            message_id    = pktGetBits(pak,32);
            redacted    = pktGetBits(pak, 1);
            auth_id        = pktGetBits(pak,32);
            sender        = strdup(unescapeString(pktGetString(pak)));
            subject        = strdup(smf_DecodeAllCharactersGet((char*)unescapeString(pktGetString(pak))));
            sent        = pktGetBits(pak,32);
            emailAddHeader(message_id,auth_id,sender,subject,sent,redacted,0,0);
            if (!full_update)
                addSystemChatMsg(textStd("NewMail",sender), INFO_SVR_COM, 0 );
        }
    }

    if( full_update )
    {
        if( count == 1 )
            addSystemChatMsg(textStd("EmailCountOne"), INFO_SVR_COM, 0 );
        else if( count > 1 )
            addSystemChatMsg(textStd("EmailCount", count), INFO_SVR_COM, 0 );
    }
}

void receiveEmailMessage(Packet *pak)
{
    int        i,count,recip_count=0,recip_max=0;
    U64        message_id;
    char    *msg,*recipient;
    char    *s,*recip_buf=0;

    message_id    = pktGetBits(pak,32);
    msg = strdup(unescapeString(pktGetString(pak)));
    count = pktGetBitsPack(pak,1);
    for(i=0;i<count;i++)
    {
        if (recip_count)
        {
            recip_buf[recip_count-2] = ';';
            recip_buf[recip_count-1] = ' ';
        }
        recipient = (char *)unescapeString(pktGetString(pak));
        s = dynArrayAdd(&recip_buf,1,&recip_count,&recip_max,strlen(recipient)+2);
        strcpy(s,recipient);
    }
    emailCacheMessage(message_id, kEmail_Local, recip_buf,msg);
    free(msg);
    free(recip_buf);
}

void receiveEmailMessageStatus(Packet *pak)
{
    int        status;
    char    *msg;

    status = pktGetBitsPack(pak,1);
    if (!status) {
        msg = pktGetString(pak);
    } else {
        msg = "";
    }
    emailSetNewMessageStatus(status,msg);
}

// Specializations
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void spec_sendSetInventory( int fromIdx, int toIdx )
{
    START_INPUT_PACKET(pak, CLIENTINP_SET_INVENTORY_BOOST );
    pktSendBitsPack( pak, 1, fromIdx );
    pktSendBitsPack( pak, 1, toIdx );
    END_INPUT_PACKET
}

void spec_sendSetPower( int pset, int pow, int fromIdx, int toIdx )
{
    START_INPUT_PACKET(pak, CLIENTINP_SET_POWER_BOOST );
    pktSendBitsPack( pak, 1, pset );
    pktSendBitsPack( pak, 1, pow );
    pktSendBitsPack( pak, 1, fromIdx );
    pktSendBitsPack( pak, 1, toIdx );
    END_INPUT_PACKET
}

void spec_sendRemove( int idx )
{
    START_INPUT_PACKET(pak, CLIENTINP_REMOVE_INVENTORY_BOOST );
    pktSendBitsPack( pak, 1, idx );
    END_INPUT_PACKET
}

void spec_sendRemoveFromPower( int iset, int ipow, int ispec )
{
    START_INPUT_PACKET(pak, CLIENTINP_REMOVE_POWER_BOOST );
    pktSendBitsPack( pak, 1, iset );
    pktSendBitsPack( pak, 1, ipow );
    pktSendBitsPack( pak, 1, ispec );
    END_INPUT_PACKET    
}

void spec_sendUnslotFromPower( int iset, int ipow, int ispec, int invSpot )
{
    START_INPUT_PACKET(pak, CLIENTINP_UNSLOT_POWER_BOOST );
    pktSendBitsPack( pak, 1, iset );
    pktSendBitsPack( pak, 1, ipow );
    pktSendBitsPack( pak, 1, ispec );
    pktSendBitsPack( pak, 1, invSpot );
    END_INPUT_PACKET    
}

void spec_sendAddSlot( int debug )
{
    START_INPUT_PACKET(pak, CLIENTINP_LEVEL_ASSIGN_BOOST_SLOT );
    enhancementAnim_send( pak, debug );
    END_INPUT_PACKET
}

void combineSpec_send( Boost * a, Boost *b )
{
    START_INPUT_PACKET(pak, CLIENTINP_COMBINE_BOOSTS );
    power_SendCombineBoosts( pak, a, b );
    END_INPUT_PACKET
}

void combineSpec_receiveResult( Packet * pak )
{
    int iSuccess = pktGetBitsPack( pak, 1 );
    int iSlot = pktGetBitsPack( pak, 1 );
    combo_ActOnResult( iSuccess, iSlot );
}

void combineBooster_send( Boost * pbA )
{
    START_INPUT_PACKET(pak, CLIENTINP_BOOSTER_BOOST );
    pktSendBitsPack( pak, 1, pbA->ppowParent->psetParent->idx );
    pktSendBitsPack( pak, 1, pbA->ppowParent->idx );
    pktSendBitsPack( pak, 1, pbA->idx );
    END_INPUT_PACKET
}

void combineSpec_receiveBoosterResult( Packet * pak ) 
{
    int iSuccess = pktGetBitsPack( pak, 1 );
    int iSlot = pktGetBitsPack( pak, 1 );
    combo_ActOnResult( iSuccess, iSlot );
}

void combineCatalyst_send( Boost * pbA )
{
    START_INPUT_PACKET(pak, CLIENTINP_CATALYST_BOOST );
    pktSendBitsPack( pak, 1, pbA->ppowParent->psetParent->idx );
    pktSendBitsPack( pak, 1, pbA->ppowParent->idx );
    pktSendBitsPack( pak, 1, pbA->idx );
    END_INPUT_PACKET
}

void combineSpec_receiveCatalystResult( Packet * pak )
{
    int iSuccess = pktGetBitsPack( pak, 1 );
    int iSlot = pktGetBitsPack( pak, 1 );
    combo_ActOnResult( iSuccess, iSlot );
}


//-----------------------------------------------------------------------

void sendEntityInfoRequest( int svr_idx, int primary_tab )
{
    START_INPUT_PACKET(pak, CLIENTINP_REQUEST_PLAYER_INFO );
    pktSendBitsPack( pak, PKT_BITS_TO_REP_SVR_IDX, svr_idx );
    pktSendBitsPack( pak, 3, primary_tab );
    END_INPUT_PACKET
}

void sendCombatMonitorUpdate(void)
{
    START_INPUT_PACKET(pak,CLIENTINP_UPDATE_COMBAT_STATS);
    sendCombatMonitorStats(playerPtr(),pak);
    END_INPUT_PACKET
}

void receiveEntityInfo( Packet *pak )
{
    int i, iNumTabs;

    if(pktGetBits(pak,1)) // valid entity
    {
        if(pktGetBits(pak,1)) // player, get badge bitfield
        {
            info_addBadges( pak );

            heroAlignmentPoints_other = pktGetBitsAuto(pak);
            vigilanteAlignmentPoints_other = pktGetBitsAuto(pak);
            villainAlignmentPoints_other = pktGetBitsAuto(pak);
            rogueAlignmentPoints_other = pktGetBitsAuto(pak);

            lastTimeWasSaved = pktGetBitsAuto(pak);
            timePlayedAsCurrentAlignmentAsOfTimeWasLastSaved = pktGetBitsAuto(pak);

            lastTimeAlignmentChanged = pktGetBitsAuto(pak);
            countAlignmentChanged = pktGetBitsAuto(pak);

            alignmentMissionsCompleted_Paragon = pktGetBitsAuto(pak);
            alignmentMissionsCompleted_Hero = pktGetBitsAuto(pak);
            alignmentMissionsCompleted_Vigilante = pktGetBitsAuto(pak);
            alignmentMissionsCompleted_Rogue = pktGetBitsAuto(pak);
            alignmentMissionsCompleted_Villain = pktGetBitsAuto(pak);
            alignmentMissionsCompleted_Tyrant = pktGetBitsAuto(pak);

            alignmentTipsDismissed = pktGetBitsAuto(pak);

            switchMoralityMissionsCompleted_Paragon = pktGetBitsAuto(pak);
            switchMoralityMissionsCompleted_Hero = pktGetBitsAuto(pak);
            switchMoralityMissionsCompleted_Vigilante = pktGetBitsAuto(pak);
            switchMoralityMissionsCompleted_Rogue = pktGetBitsAuto(pak);
            switchMoralityMissionsCompleted_Villain = pktGetBitsAuto(pak);
            switchMoralityMissionsCompleted_Tyrant = pktGetBitsAuto(pak);

            reinforceMoralityMissionsCompleted_Paragon = pktGetBitsAuto(pak);
            reinforceMoralityMissionsCompleted_Hero = pktGetBitsAuto(pak);
            reinforceMoralityMissionsCompleted_Vigilante = pktGetBitsAuto(pak);
            reinforceMoralityMissionsCompleted_Rogue = pktGetBitsAuto(pak);
            reinforceMoralityMissionsCompleted_Villain = pktGetBitsAuto(pak);
            reinforceMoralityMissionsCompleted_Tyrant = pktGetBitsAuto(pak);

            moralityTipsDismissed = pktGetBitsAuto(pak);
        }
        else
            info_addSalvageDrops(pak);

        iNumTabs = pktGetBits(pak,16);
        for(i = 0; i < iNumTabs; i++)
        {
            char name[32];
            char *text;
            int selected = pktGetBits(pak,1);

            strncpy( name, pktGetString(pak), 31); // tabName
            text = pktGetString(pak);     // tabText
            info_replaceTab(name, text, selected);
        }
    }
    else
    {
        //entity was gone on server by the time it got the request
        // update info box to let user know they wont ever get an update
        info_noResponse();
    }
}

void receiveClientCombatStats( Packet *pak )
{
    receiveCombatMonitorStats( playerPtr(), pak );
}
//-----------------------------------------------------------------------

void sendSuperCostume(SGColorBits primaryU, SGColorBits secondaryU, SGColorBits primary2U, SGColorBits secondary2U, SGColorBits tertiaryU, SGColorBits quaternaryU, bool hide_emblem)
{
    START_INPUT_PACKET(pak, CLIENTINP_SEND_SGCOSTUME );
    pktSendBitsPack( pak, 32, primaryU.SGBitSetLow );
    pktSendBitsPack( pak, 32, primaryU.SGBitSetHigh );
    pktSendBitsPack( pak, 32, secondaryU.SGBitSetLow );
    pktSendBitsPack( pak, 32, secondaryU.SGBitSetHigh );
    pktSendBitsPack( pak, 32, primary2U.SGBitSetLow );
    pktSendBitsPack( pak, 32, primary2U.SGBitSetHigh );
    pktSendBitsPack( pak, 32, secondary2U.SGBitSetLow );
    pktSendBitsPack( pak, 32, secondary2U.SGBitSetHigh );
    pktSendBitsPack( pak, 32, tertiaryU.SGBitSetLow );
    pktSendBitsPack( pak, 32, tertiaryU.SGBitSetHigh );
    pktSendBitsPack( pak, 32, quaternaryU.SGBitSetLow );
    pktSendBitsPack( pak, 32, quaternaryU.SGBitSetHigh );
    pktSendBits( pak, 1, hide_emblem );
    END_INPUT_PACKET
}

void sendSuperCostumeData(SGColorBits sgColorDataU[][6], bool hide_emblem, int num_slots)
{
    int i;
    START_INPUT_PACKET(pak, CLIENTINP_SEND_SGCOSTUME_DATA );
    pktSendBits( pak, 8, num_slots );
    for (i = 1; i <= num_slots; i++)
    {
        // Last sanity check before we send
        if ((sgColorDataU[i][0].SGBitSet | sgColorDataU[i][1].SGBitSet | sgColorDataU[i][2].SGBitSet | sgColorDataU[i][3].SGBitSet | sgColorDataU[i][4].SGBitSet | sgColorDataU[i][5].SGBitSet) == 0)
        {
            sgColorDataU[i][0].SGBitSet = 0x5555555555555555uLL;
            sgColorDataU[i][1].SGBitSet = 0x5555555555555555uLL;
            sgColorDataU[i][2].SGBitSet = 0x5555555555555555uLL;
            sgColorDataU[i][3].SGBitSet = 0x5555555555555555uLL;
            sgColorDataU[i][4].SGBitSet = 0x5555555555555555uLL;
            sgColorDataU[i][5].SGBitSet = 0x5555555555555555uLL;
        }
        pktSendBitsPack( pak, 32, sgColorDataU[i][0].SGBitSetLow );
        pktSendBitsPack( pak, 32, sgColorDataU[i][0].SGBitSetHigh );
        pktSendBitsPack( pak, 32, sgColorDataU[i][1].SGBitSetLow );
        pktSendBitsPack( pak, 32, sgColorDataU[i][1].SGBitSetHigh );
        pktSendBitsPack( pak, 32, sgColorDataU[i][2].SGBitSetLow );
        pktSendBitsPack( pak, 32, sgColorDataU[i][2].SGBitSetHigh );
        pktSendBitsPack( pak, 32, sgColorDataU[i][3].SGBitSetLow );
        pktSendBitsPack( pak, 32, sgColorDataU[i][3].SGBitSetHigh );
        pktSendBitsPack( pak, 32, sgColorDataU[i][4].SGBitSetLow );
        pktSendBitsPack( pak, 32, sgColorDataU[i][4].SGBitSetHigh );
        pktSendBitsPack( pak, 32, sgColorDataU[i][5].SGBitSetLow );
        pktSendBitsPack( pak, 32, sgColorDataU[i][5].SGBitSetHigh );
    }
    pktSendBits( pak, 1, hide_emblem );
    END_INPUT_PACKET
}


void requestSGColorData()
{
    // No data, the packet itself is the request.
    START_INPUT_PACKET(pak, CLIENTINP_REQUEST_SG_COLORS );
    END_INPUT_PACKET
}

//-----------------------------------------------------------------------

void sendTailorTransaction( int genderChange, Costume * costume, PowerCustomizationList *powerCust )
{
    START_INPUT_PACKET(pak, CLIENTINP_SEND_TAILOR );
    pktSendBits( pak, 2, genderChange );
    costume_send( pak, costume, 1);
    powerCustList_send(pak, powerCust);
    pktSendBits( pak, 2, (bUsingDayJobCoupon ? 2 : 0) | (bFreeTailor ? 1 : 0));
    
    END_INPUT_PACKET
}
void sendTailoredPowerCust( PowerCustomizationList *powerCust )
{
    START_INPUT_PACKET(pak, CLIENTINP_SEND_POWER_CUST);
    powerCustList_send(pak, powerCust);
    END_INPUT_PACKET
}
void receiveTailorOpen( Packet * pak )
{
    int genderChangeMode = 0;
    Entity *e = playerPtr();
    // verify here?
    FreeTailorCount = pktGetBits(pak, 32);
    bUltraTailor = pktGetBits(pak, 1);
    bFreeTailor = FreeTailorCount ? true : false;
    bVetRewardTailor = pktGetBits(pak, 1);
    genderChangeMode = pktGetBits(pak, 1);

    if( bUltraTailor )
    {
        if( playerPtr()->npcIndex )
            dialogStd( DIALOG_OK, "CantChangeShapshifted", NULL, NULL, NULL, NULL, 0 );
        else
            genderChangeMenuStart();
    }
    else
    {
        gEnterTailor = (genderChangeMode && (AccountCanAccessGenderChange(&e->pl->account_inventory, e->pl->auth_user_data, e->access_level) ) ) ? 2: 1;
        window_setMode( WDW_COSTUME_SELECT, WINDOW_GROWING );
    }
}

//-----------------------------------------------------------------------

void receiveRespecOpen( Packet * pak )
{
    // verify here?

    extern bool gWaitingVerifyRespec;

     gWaitingVerifyRespec = false;
    respec_allowLevelUp( false);

    if (respec_earlyOut())
    {
        return;
    }

    start_menu( MENU_RESPEC );
}

//-----------------------------------------------------------------------

void receiveAuctionOpen( Packet * pak )
{
    // verify here?
    int activate = pktGetBits(pak, 1);

    if (activate && !windowUp(WDW_AUCTION))
        window_setMode(WDW_AUCTION, WINDOW_GROWING);
    else 
        window_setMode(WDW_AUCTION, WINDOW_SHRINKING);
}


void sendCharacterRespec( Character *pchar, int patronID )
{
    START_INPUT_PACKET(pak, CLIENTINP_SEND_RESPEC );

    /* Basic Stuff ****************************************************/

    pktSendString(pak, pchar->pclass->pchName);
    pktSendString(pak, pchar->porigin->pchName);

    /* Patron *********************************************************/
    
    if (patronID >= 0)
    {
        pktSendBits(pak, 1, 1);
        pktSendBitsPack(pak, 4, patronID);
    } else {
        pktSendBits(pak, 1, 0);
    }

    /* Powers *********************************************************/

    respec_sendPowers( pak );

    /* Boosts ***************************************************/

    respec_sendBoostSlots(pak );
    respec_sendPowerBoosts( pak );
    repsec_sendCharBoosts( pak );
    respec_sendPile( pak );
    
    END_INPUT_PACKET
}

//------------------------------------------------------------------------------
// Keybinds ////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------

void keybinds_recieve( Packet *pak, char *profile, KeyBind *kbp )
{
    int i, key;

    strncpyt( profile, pktGetString( pak ), 32);

    if( isDevelopmentMode() )
        strcpy( profile, "Dev" );

    for( i = 0; i < BIND_MAX; i++ )
    {
        if( !kbp[i].command[0] )
            kbp[i].command[0] = calloc( 1, sizeof(char)*BIND_MAX );
        strncpyt( kbp[i].command[0],  pktGetString( pak ), BIND_MAX);

        key = pktGetBits( pak, 32 );
        kbp[i].modifier = pktGetBits( pak, 32 );

        kbp[i].key = (key & 0xff);
        kbp[i].secondary = (key & 0xf00 );
    }
}

void keybind_send( const char *profile, int key, int mod, int secondary, const char *command )
{
    START_INPUT_PACKET(pak, CLIENTINP_KEYBIND );

    pktSendString( pak, profile );

    if( !secondary )
        pktSendBits( pak, 32, key );
    else
        pktSendBits( pak, 32, (key | 0xf00) );

    pktSendBits( pak, 32, mod );
    if( command )
    {
        pktSendBits(pak,1,1);
        pktSendString( pak, command );
    }
    else
        pktSendBits(pak,1,0);
    
    END_INPUT_PACKET
}

void keybind_sendClear( const char *profile, int key, int mod )
{
    START_INPUT_PACKET(pak, CLIENTINP_KEYBIND_CLEAR );
    pktSendString( pak, profile );
    pktSendBits( pak, 32, key );
    pktSendBits( pak, 32, mod );
    END_INPUT_PACKET
}

void keybind_sendReset()
{
    EMPTY_INPUT_PACKET(CLIENTINP_KEYBIND_RESET );
}

void keyprofile_send( const char *profile )
{
    START_INPUT_PACKET(pak, CLIENTINP_KEYPROFILE );
    pktSendString( pak, profile );
    END_INPUT_PACKET
}

/**********************************************************************func*
* browserReceiveText
*
*/
void browserReceiveText(Packet *pak)
{
    browserOpen();
    browserSetText(pktGetString(pak));
}

/**********************************************************************func*
* browserReceiveClose
*
*/
void browserReceiveClose(Packet *pak)
{
    browserClose();
}


/**********************************************************************func*
* storeReceiveOpen
*
*/
void storeReceiveOpen(Packet *pak)
{
    int i;
    char **ppchStores;

    int idx = pktGetBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX);
    int iCnt = pktGetBitsPack(pak, 2);

    // These allocations are inherited by the storeOpen function.
    // It is responsible for freeing them.
    ppchStores = (char **)calloc(iCnt, sizeof(char *));

    for(i=0; i<iCnt; i++)
    {
        ppchStores[i] = strdup(pktGetString(pak));
    }

    storeOpen(idx, iCnt, ppchStores);
}

/**********************************************************************func*
* storeReceiveClose
*
*/
void storeReceiveClose(Packet *pak)
{
    storeClose();
}


/**********************************************************************func*
 * store_SendBuyItem
 *
 */
void store_SendBuyItem(const MultiStore *ms, int eType, int row, int col)
{
    START_INPUT_PACKET(pak, CLIENTINP_STORE_BUY_ITEM);

    pktSendBitsPack(pak, 12, ms->idNPC);

    if(eType == kPowerType_Inspiration)
    {
        pktSendBits(pak, 1, 0);
        pktSendBitsPack(pak, 3, row);
        pktSendBitsPack(pak, 3, col);
    }
    else
    {
        pktSendBits(pak, 1, 1);
        pktSendBitsPack(pak, 4, row);
    }
    
    END_INPUT_PACKET
}

/**********************************************************************func*
* store_SendBuySalvage
*
*/
void store_SendBuySalvage(const MultiStore *ms, int id, int amount)
{
    START_INPUT_PACKET(pak, CLIENTINP_STORE_BUY_SALVAGE);

    pktSendBitsPack(pak, 12, ms->idNPC);
    pktSendBitsPack(pak, 8, amount);
    pktSendBitsPack(pak, 12, id);

    END_INPUT_PACKET
}

/**********************************************************************func*
* store_SendBuyRecipe
*
*/
void store_SendBuyRecipe(const MultiStore *ms, int id, int amount)
{
    START_INPUT_PACKET(pak, CLIENTINP_STORE_BUY_RECIPE);

    pktSendBitsPack(pak, 12, ms->idNPC);
    pktSendBitsPack(pak, 8, amount);
    pktSendBitsPack(pak, 12, id);

    END_INPUT_PACKET
}


/**********************************************************************func*
 * store_SendSellItem
 *
 */
void store_SendSellItem(const MultiStore *ms, const char *pchItem)
{
    START_INPUT_PACKET(pak, CLIENTINP_STORE_SELL_ITEM);

    pktSendBitsPack(pak, 12, ms->idNPC);
    pktSendString(pak, pchItem);
    
    END_INPUT_PACKET
}


// Options stuff
//-----------------------------------------------------------------------------------------------
void sendThirdPerson()
{
     START_INPUT_PACKET(pak, CLIENTINP_THIRD );
    pktSendBits( pak, 1, control_state.first );
    END_INPUT_PACKET
}

void sendDividers(void)
{
    START_INPUT_PACKET(pak, CLIENTINP_DIVIDERS );
    pktSendBitsPack( pak, 9, gDivSuperName );
    pktSendBitsPack( pak, 9, gDivSuperMap );
    pktSendBitsPack( pak, 9, gDivSuperTitle );
    pktSendBitsPack( pak, 9, gDivEmailFrom );
    pktSendBitsPack( pak, 9, gDivEmailSubject );
    pktSendBitsPack( pak, 9, gDivFriendName );
    pktSendBitsPack( pak, 9, gDivLfgName );
    pktSendBitsPack( pak, 9, gDivLfgMap );
    END_INPUT_PACKET
}

void receiveDividers( Packet * pak )
{
    int i, tmp[8];
    
    for( i = 0; i < 8; i++ )
        tmp[i] = pktGetBitsPack( pak, 9 );

    if( tmp[0] ) // don't set dividers until they've been saved at least once
    {
        gDivSuperName        = tmp[0];
        gDivSuperMap        = tmp[1];
        gDivSuperTitle        = tmp[2];
        gDivEmailFrom        = tmp[3];
        gDivEmailSubject    = tmp[4];
        gDivFriendName        = tmp[5];
        gDivLfgName            = tmp[6];
        gDivLfgMap            = tmp[7];
        setSavedDividerSettings();
    }
}



void sendOptions(void *data)
{
    int i;
    int oneTimeCharCreate = (int)data;
    //    if you trip this assert, you are sending options before the options from the server are sent to the client
    //    delay until the settings arrive or you will override the server's versions with uninit'ed data -EL
    devassert(oneTimeCharCreate || game_state.game_mode != SHOW_LOAD_SCREEN);
    if (!oneTimeCharCreate && game_state.game_mode == SHOW_LOAD_SCREEN)
    {
        return;
    }

    START_INPUT_PACKET(pak, CLIENTINP_OPTIONS );

    for( i=1; i < kUO_OptionTotal; i++ )
    {
        UserOption * pUO = userOptionGet(i);
        assert( pUO );
        if(pUO->bitSize<0)
            pktSendF32( pak, pUO->fVal );
        else
            pktSendBits( pak, pUO->bitSize, pUO->iVal);
    }

    END_INPUT_PACKET
}

void receiveOptions( Packet * pak )
{
    int i;
    for( i=1; i < kUO_OptionTotal; i++ )
    {
        UserOption * pUO = userOptionGet(i);
        assert( pUO );
        if(pUO->bitSize<0)
            pUO->fVal = pktGetF32( pak );
        else
            pUO->iVal = pktGetBits( pak, pUO->bitSize);
    }
}


void sendTitles(int showThe, int commonIdx, int originIdx, int color1, int color2 )
{
    START_INPUT_PACKET(pak, CLIENTINP_TITLES );
    pktSendBitsPack( pak, 5, showThe );
    pktSendBitsPack( pak, 6, commonIdx );
    pktSendBitsPack( pak, 5, originIdx );
    pktSendBitsAuto( pak, color1 );
    pktSendBitsAuto( pak, color2 );
    END_INPUT_PACKET
}

void receiveTitles( Packet * pak )
{
    Entity * e = playerPtr();
    e->pl->titleThe = pktGetBitsPack( pak, 5 );
    strncpyt( e->pl->titleTheText, pktGetString( pak ), ARRAY_SIZE(e->pl->titleTheText));
    strncpyt( e->pl->titleCommon, pktGetString( pak ), ARRAY_SIZE(e->pl->titleCommon));
    strncpyt( e->pl->titleOrigin, pktGetString( pak ), ARRAY_SIZE(e->pl->titleOrigin));
    e->pl->titleColors[0] = pktGetBitsAuto(pak);
    e->pl->titleColors[1] = pktGetBitsAuto(pak);
    e->pl->titleBadge = pktGetBits( pak, 32 );
    strncpyt( e->pl->titleSpecial, pktGetString( pak ), ARRAY_SIZE(e->pl->titleSpecial));
}

void sendDescription( char * desc, char * motto )
{
    START_INPUT_PACKET(pak, CLIENTINP_PLAYERDESC );
    pktSendString( pak, motto );
    pktSendString( pak, desc );
    END_INPUT_PACKET
}

void receiveDescription( Packet * pak )
{
    Entity * e = playerPtr();
    strncpyt( e->strings->playerDescription, pktGetString(pak), ARRAY_SIZE(e->strings->playerDescription) );
    strncpyt( e->strings->motto, pktGetString(pak), ARRAY_SIZE(e->strings->motto) );
}

//
//
void gift_send( int dbid, int type, int icol, int irow, int ispec, int invIdx )
{
    START_INPUT_PACKET(pak, CLIENTINP_GIFTSEND );

    pktSendBitsPack( pak, 1, dbid );
    pktSendBitsPack( pak, 1, type );

    if( type == kTrayItemType_Inspiration || type == kTrayItemType_Power)
    {
        pktSendBitsPack( pak, 1, icol );
        pktSendBitsPack( pak, 1, irow );
    }
    else if( type == kTrayItemType_SpecializationInventory )
        pktSendBitsPack( pak, 1, ispec );
    else if( type == kTrayItemType_Salvage || type == kTrayItemType_Recipe  )
        pktSendBitsPack( pak, 1, invIdx );
        
    END_INPUT_PACKET
}

//
//
void receiveSearchResults(Packet *pak)
{
    int result_count = pktGetBits( pak, 32 );
    int count         = pktGetBits( pak, 32 );
    int i;

    window_setMode( WDW_LFG, WINDOW_GROWING );
     lfg_clearAll();

    lfg_setResults( result_count, count );

    // now send down data to client
    for( i = 0; i < count; i++ )
    {
        char name[32];
        char mapName[128];
        char comment[128];
        int lfg, hidden, archetype, origin, level, dbid, teamsize, leader, sameteam, leaguesize, league_leader, sameleague;
        int arena_map, mission_map, other_faction, other_universe;
        int faction_same_map, universe_same_map;

        strncpyt( name, pktGetString(pak), 32 );

        lfg                    = pktGetBitsPack( pak, 10 );
        hidden                = pktGetBitsPack( pak, 1 );
        teamsize            = pktGetBitsPack( pak, 4 );
        leader                = pktGetBitsPack( pak, 1 );
        sameteam            = pktGetBitsPack( pak, 1 );
        leaguesize            = pktGetBitsPack( pak, 8 );
        league_leader        = pktGetBitsPack( pak, 1 );
        sameleague            = pktGetBitsPack( pak, 1 );
        archetype            = pktGetBitsPack( pak, 4 );
        origin                = pktGetBitsPack( pak, 4 );
        level                = pktGetBitsPack( pak, 4 );
        dbid                = pktGetBitsPack( pak, 32);
        arena_map            = pktGetBitsPack( pak, 1 );
        mission_map            = pktGetBitsPack( pak, 1 );
        other_faction        = pktGetBitsPack( pak, 1 );
        faction_same_map    = pktGetBitsPack( pak, 1 );
        other_universe        = pktGetBitsPack( pak, 1 );
        universe_same_map    = pktGetBitsPack( pak, 1 );
        strncpyt( comment, pktGetString(pak), 128 );
        strncpyt( mapName, pktGetString(pak), 32 );
        
        lfg_add( name, archetype, origin, level, lfg, hidden, teamsize, leader, sameteam, leaguesize, league_leader, sameleague, 
                    mapName, dbid, arena_map, mission_map, other_faction, faction_same_map, other_universe, 
                    universe_same_map, comment );
    }

}

void receiveRewardChoice(Packet *pak)
{
    int i, count;

    clearRewardChoices();
     setRewardDesc( pktGetString(pak) );
    setRewardDisableChooseNothing( pktGetBits(pak,1) );

    count = pktGetBitsPack(pak, 3);
    if(!count)
    {
        clearRewardChoices();
        window_setMode(WDW_REWARD_CHOICE,WINDOW_SHRINKING);
        return;
    }

    for( i = 0; i < count; i++ )
    {
        int visible = pktGetBits(pak,1);
        if( visible )
        {
            int disabled = pktGetBits(pak,1);
            addRewardChoice(1,disabled,pktGetString(pak));
        }
        else
            addRewardChoice(0,0,0);
    }

    window_setMode( WDW_REWARD_CHOICE, WINDOW_GROWING );
}

void sendRewardChoice( int choice )
{
    START_INPUT_PACKET(pak, CLIENTINP_REWARDCHOICE );
    pktSendBitsPack( pak, 3, choice );
    END_INPUT_PACKET
}

void arenaSendCreateRequest(void)
{
    EMPTY_INPUT_PACKET( CLIENTINP_ARENA_REQ_CREATE );
}

void arenaSendViewResultRequest( int eventid, int uniqueid  )
{
    START_INPUT_PACKET(pak, CLIENTINP_ARENA_REQ_RESULT );
    pktSendBitsPack( pak, 6, eventid );
    pktSendBitsPack( pak, 6, uniqueid );
    END_INPUT_PACKET
}

void arenaSendObserveRequest( int eventid, int uniqueid  )
{
    START_INPUT_PACKET(pak, CLIENTINP_ARENA_REQ_OBSERVE );
    pktSendBitsPack( pak, 6, eventid );
    pktSendBitsPack( pak, 6, uniqueid );
    END_INPUT_PACKET
}

void arenaSendJoinRequest( int eventid, int uniqueid, int ignore_active  )
{
    START_INPUT_PACKET(pak, CLIENTINP_ARENA_REQ_JOIN );
    pktSendBitsPack( pak, 6, eventid );
    pktSendBitsPack( pak, 6, uniqueid );
    pktSendBits( pak, 1, ignore_active );
    END_INPUT_PACKET
}

void arenaSendCreatorUpdate( ArenaEvent * event )
{
    START_INPUT_PACKET(pak, CLIENTINP_ARENA_CREATOR_UPDATE );
    ArenaCreatorUpdateSend( event, pak );
    END_INPUT_PACKET
}

void arenaSendPlayerUpdate( ArenaEvent * event, ArenaParticipant * ap )
{
    START_INPUT_PACKET(pak, CLIENTINP_ARENA_PLAYER_UPDATE );
    ArenaParticipantUpdateSend( event, ap, pak );
    END_INPUT_PACKET
}

void arenaSendFullPlayerUpdate(ArenaEvent *event )
{
    START_INPUT_PACKET(pak, CLIENTINP_ARENA_FULL_PLAYER_UPDATE );
    ArenaFullParticipantUpdateSend( event, pak );
    END_INPUT_PACKET
}

void arenaSendPlayerDrop( ArenaEvent * event, int kicked_dbid )
{
    START_INPUT_PACKET(pak, CLIENTINP_ARENA_DROP );
    pktSendBitsPack( pak, 6, event->eventid );
    pktSendBitsPack( pak, 6, event->uniqueid );
    pktSendBits( pak, 32, kicked_dbid );
    END_INPUT_PACKET
}

//-----------------------------------------------------------------------------
//
//
static int invite_eventid;
static int invite_uniqueid;

static void sendArenaAccept(void *data)
{
    char buf[128];

    sprintf( buf, "arena_accept %i %i %i", teamOfferingDbId, invite_eventid, invite_uniqueid );
    cmdParse( buf );

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
}

//
//
static void sendArenaDecline(void *data)
{
    char buf[128];

    sprintf( buf, "arena_decline %i", teamOfferingDbId );
    cmdParse( buf );

    // clear this out so someone else can offer
    teamOfferingDbId = 0;
}

void receiveArenaInvite(Packet *pak)
{
    int offerer;
    char *temp_name;

    offerer = pktGetBits( pak, 32 );
    temp_name = strdup(pktGetString( pak ) );
    invite_eventid = pktGetBits( pak, 32 );
    invite_uniqueid = pktGetBits( pak, 32 );
    // if somone has already offered, send a busy signal back
    if ( teamOfferingDbId )
    {
        assert(!"Recieved a Arena Offer while we were already considering one!  Server-side check gone awry?");
        free(temp_name);
        return;
    }
    teamOfferingDbId = offerer;
    strcpy(offererName, temp_name);
    free(temp_name);

    // otherwise display a dialog
    dialogTimed( DIALOG_YES_NO, textStd("OfferArenaEvent", offererName), NULL, NULL, sendArenaAccept, sendArenaDecline, 1, ARENA_SCHEDULED_TELEPORT_WAIT);
}

static void sendArenaJoinAnyways(void *data)
{
     arenaSendJoinRequest( invite_eventid, invite_uniqueid, true );
    requestCurrentKiosk();
}


void receiveArenaDialog(Packet *pak)
{
    int command;
    char resulterror[256];

    command = pktGetBitsPack(pak, 1);
    invite_eventid = pktGetBitsPack( pak, 1 );
    invite_uniqueid = pktGetBitsPack( pak, 1 );
    strncpyt( resulterror, pktGetString(pak), ARRAY_SIZE(resulterror) );

    switch( command )
    {
        case CLIENTINP_ARENA_REQ_JOIN_INVITE:
        case CLIENTINP_ARENA_REQ_JOIN:
        {
            dialogStd( DIALOG_ACCEPT_CANCEL, textStd(resulterror), NULL, NULL, sendArenaJoinAnyways, NULL, 1 );
        }break;

        default:
            break;
    }
}

// Keep updates that are received before the widget was
typedef struct ScriptUIUpdate
{
    int widgetId;
    int index;
    int hidden;
    char* var;
    unsigned int packetID;
} ScriptUIUpdate;

static ScriptUIUpdate** scriptUIUpdates;

static void ScriptUIApplyReceivedUpdates(ScriptUIClientWidget* newWidget)
{
    int i, n = eaSize(&scriptUIUpdates);
    for (i = n-1; i >= 0; i--)
    {
        ScriptUIUpdate* update = scriptUIUpdates[i];
        if (newWidget->uniqueID == update->widgetId)
        {
            if (update->index == -1)
            {
                newWidget->hidden = update->hidden;
            }
            else if (update->index >= 0 && update->index < eaSize(&newWidget->varList) && update->packetID >= newWidget->packetOrder[update->index])
            {
                free(newWidget->varList[update->index]);
                newWidget->varList[update->index] = update->var;
                newWidget->packetOrder[update->index] = update->packetID;
            }
            else
            {
                free(update->var);
            }
            free(update);
            eaRemove(&scriptUIUpdates, i);
        }
    }
}

static void ScriptUIRemoveUpdates(int widgetId)
{
    int i, n = eaSize(&scriptUIUpdates);
    for (i = n-1; i >= 0; i--)
    {
        ScriptUIUpdate* update = scriptUIUpdates[i];
        if (widgetId == update->widgetId)
        {
            if (update->var)
                free(update->var);
            free(update);
            eaRemove(&scriptUIUpdates, i);
        }
    }
}

void ScriptUIDestroyWidget(ScriptUIClientWidget* widget)
{
    int i, n = eaSize(&widget->varList);
    for (i = 0; i < n; i++)
        free(widget->varList[i]);
    freeToolTip(widget->tooltip);
    eaiDestroy(&widget->packetOrder);
    eaDestroy(&widget->varList);
    free(widget);
}

void ScriptUIShutDown()
{
    int i, numUpdates, numWidgets = eaSize(&scriptUIWidgetList);
    for (i = 0; i < numWidgets; i++)
        ScriptUIDestroyWidget(scriptUIWidgetList[i]);
    eaDestroy(&scriptUIWidgetList);
    numUpdates = eaSize(&scriptUIUpdates);
    for (i = numUpdates-1; i >= 0; i--)
    {
        if (scriptUIUpdates[i]->var)
            free(scriptUIUpdates[i]->var);
        free(scriptUIUpdates[i]);
        eaRemove(&scriptUIUpdates, i);
    }
    eaDestroy(&scriptUIUpdates);
    for (i = 0; i < eaSize(&scriptUIKarmaWidgetList); i++)
        ScriptUIDestroyWidget(scriptUIKarmaWidgetList[i]);
    eaDestroy(&scriptUIKarmaWidgetList);
    scriptUIUpdated = 1;
}

static ScriptUIClientWidget* ScriptUIClientWidgetFromId(ScriptUIClientWidget **list, int uniqueID)
{
    int i, numWidgets = eaSize(&list);
    for (i = 0; i < numWidgets; i++)
        if (list[i]->uniqueID == uniqueID)
            return list[i];
    return NULL;
}

// Receives new scriptUI widgets, or receives the kill signal for a widget
void ScriptUIReceive(Packet* pak)
{
    int i, num;
    int uniqueID = pktGetBitsPack(pak, 1);
    scriptUIUpdated = 1; // Flag the scriptui as updated, so we recalc the compass
    if (pktGetBits(pak, 1))
    {
        ScriptUIClientWidget* newWidget = malloc(sizeof(ScriptUIClientWidget));
        memset(newWidget, 0, sizeof(ScriptUIClientWidget));
        newWidget->type = pktGetBitsPack(pak, 1);
        newWidget->hidden = pktGetBits(pak, 1);
        newWidget->uniqueID = uniqueID;

        // Get all the new vars
        num = pktGetBitsPack(pak, 1);
        for (i = 0; i < num; i++)
        {
            eaPush(&newWidget->varList, strdup(pktGetString(pak)));
            eaiPush(&newWidget->packetOrder, pak->id);
        }
        eaPush(&scriptUIWidgetList, newWidget);

        // Timer related info
        if ((newWidget->type == TimerWidget) && (atoi(newWidget->varList[0]) - timerSecondsSince2000WithDelta() >= 3600))
            newWidget->showHours = 1;

        // Apply updates
        ScriptUIApplyReceivedUpdates(newWidget);
    }
    else
    {
        int i, numWidgets = eaSize(&scriptUIWidgetList);
        for (i = 0; i < numWidgets; i++)
        {
            ScriptUIClientWidget* widget = scriptUIWidgetList[i];
            if (widget->uniqueID == uniqueID)
            {
                eaRemove(&scriptUIWidgetList, i);
                ScriptUIDestroyWidget(widget);
                break;
            }
        }
        // Remove stale updates
        ScriptUIRemoveUpdates(uniqueID);
    }
}

void ScriptUIReceiveUpdate(Packet* pak)
{
    while (pktGetBits(pak, 1))
    {
        int widgetId = pktGetBitsPack(pak, 1);
        int hidden = pktGetBits(pak, 1);
        ScriptUIClientWidget* widget = ScriptUIClientWidgetFromId(scriptUIWidgetList, widgetId);
        if (widget)
        {
            widget->hidden = hidden;
        }
        else
        {
            ScriptUIUpdate* update = malloc(sizeof(ScriptUIUpdate));
            update->widgetId = widgetId;
            update->index = -1;
            update->hidden = hidden;
            update->var = NULL;
            update->packetID = pak->id;
            eaPush(&scriptUIUpdates, update);
        }
        while(pktGetBits(pak, 1))
        {
            int index = pktGetBitsPack(pak, 1);
            char* var = pktGetString(pak);

            // If we don't have the widget yet, queue it, otherwise process it
            if (!widget)
            {
                ScriptUIUpdate* update = malloc(sizeof(ScriptUIUpdate));
                update->widgetId = widgetId;
                update->index = index;
                update->hidden = hidden;
                update->var = strdup(var);
                update->packetID = pak->id;
                eaPush(&scriptUIUpdates, update);
            }
            else if (index >= 0 && index < eaSize(&widget->varList) && pak->id >= widget->packetOrder[index])
            {
                free(widget->varList[index]);
                widget->varList[index] = strdup(var);
                widget->packetOrder[index] = pak->id;
            }
        }
    }
}

// Send a pet a command
void sendPetCommand( int svr_id, int stance, int action, Vec3 vec )
{
    START_INPUT_PACKET(pak, CLIENTINP_PETCOMMAND);
    pktSendBits( pak, 32, svr_id );
    pktSendBits( pak, 32, stance );
    pktSendBits( pak, 32, action );
    if( action == kPetAction_Goto )
        pktSendVec3( pak, vec );
    END_INPUT_PACKET;
}

// Send a pet message
void sendPetSay( int svr_id, char * msg )
{
    START_INPUT_PACKET(pak, CLIENTINP_PETSAY);
    pktSendBits( pak, 32, svr_id );
    pktSendString( pak, msg );
    END_INPUT_PACKET;
}

void sendPetRename( int svr_id, char * name )
{
    START_INPUT_PACKET(pak, CLIENTINP_PETRENAME);
    pktSendBits( pak, 32, svr_id );
    pktSendString( pak, name);
    END_INPUT_PACKET;
}

void receivePetUpdateUI(Packet * pak)
{
    int svr_idx = pktGetBitsPack(pak, 1);
    PetAction action = pktGetBits(pak, PET_ACTION_BITS);
    PetStance stance = pktGetBits(pak, PET_STANCE_BITS);
    pet_update(svr_idx,stance,action);
}

void receiveKarmaStats(Packet *pak)
{
    int active = pktGetBits(pak, 1);
    if (active)
    {
        int i, count;
        char *StatsSent[] = {    "PowerClick", "BuffPowerClick", "TeamObjComplete", "AllPlayObjComplete", "CurrentTotal",
            "Median", "Average", "ActiveKarmaPlayers", "NumBubbles" };
        count = pktGetBitsAuto(pak);
        for (i = 0; i < count; ++i)
        {
            ScriptUIClientWidget* karmaWidget = ScriptUIClientWidgetFromId(scriptUIKarmaWidgetList, i);
            if (!karmaWidget)
            {
                if (eaSize(&scriptUIKarmaWidgetList) < 2)
                {
                    karmaWidget = malloc(sizeof(ScriptUIClientWidget));
                    memset(karmaWidget, 0, sizeof(ScriptUIClientWidget));
                    karmaWidget->type = TitleWidget;
                    karmaWidget->uniqueID = -1;
                    eaPush(&karmaWidget->varList, strdup("Karma"));
                    eaPush(&karmaWidget->varList, strdup("Karma"));
                    eaPush(&scriptUIKarmaWidgetList, karmaWidget);
                    karmaWidget = malloc(sizeof(ScriptUIClientWidget));
                    memset(karmaWidget, 0, sizeof(ScriptUIClientWidget));
                    karmaWidget->type = ListWidget;
                    karmaWidget->uniqueID = -2;
                    eaPush(&karmaWidget->varList, strdup("KarmaList"));
                    eaPush(&karmaWidget->varList, strdup("KarmaList"));
                    eaPush(&scriptUIKarmaWidgetList, karmaWidget);

                }
                karmaWidget = malloc(sizeof(ScriptUIClientWidget));
                memset(karmaWidget, 0, sizeof(ScriptUIClientWidget));
                karmaWidget->type = KarmaWidget;
                karmaWidget->uniqueID = i;
                eaPush(&scriptUIKarmaWidgetList, karmaWidget);
            }
            else
            {
                free(karmaWidget->varList[0]);
                free(karmaWidget->varList[1]);
                eaDestroy(&karmaWidget->varList);
            }

            eaPush(&karmaWidget->varList, strdup(StatsSent[i]));
            {
                char buffer[20];
                _snprintf_s(buffer, sizeof(buffer), sizeof(buffer)-1, "%i", pktGetBitsAuto(pak));
                eaPush(&karmaWidget->varList, strdup(buffer));
            }
        }
    }
    else
    {
        if (eaSize(&scriptUIKarmaWidgetList))
        {
            int i;
            for (i = 0; i < eaSize(&scriptUIKarmaWidgetList); i++)
                ScriptUIDestroyWidget(scriptUIKarmaWidgetList[i]);
            eaDestroy(&scriptUIKarmaWidgetList);
        }
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////// PET ARMY MANAGEMENT//////////////////////////////////////////////////////////////////////////////////////
ArenaPetManagementInfo g_ArenaPetManagementInfo;

//------------------------------------------------------------
//  Ask server for info needed to open your pet management screen
//  server will send the info, handled by receive below
//----------------------------------------------------------
void arena_ManagePets(void)
{
    START_INPUT_PACKET(pak, CLIENTINP_ARENA_MANAGEPETS);
    END_INPUT_PACKET;
}

//------------------------------------------------------------
//  After you've finished with the pet management screen, update the server 
//  with your new pet army. Server will send ack handled by recieve below
//----------------------------------------------------------
void arena_CommitPetChanges(void)
{
    int i,j;
    int selectedCnt = 0;

    START_INPUT_PACKET(pak, CLIENTINP_ARENA_COMMITPETCHANGES);

    //pktSendBitsPack(pak,1, eaSize(&g_ArenaPetManagementInfo.currentPets) ); //I'm sending this many infos
    //for( i = 0 ; i < eaSize( &g_ArenaPetManagementInfo.currentPets ) ; i++ )
    //{
    //    pktSendBitsPack( pak, 1, g_ArenaPetManagementInfo.currentPets[i]->tokenID );
    //}

    for( i = 0 ; i < eaSize( &g_ArenaPetManagementInfo.availablePets ) ; i++ )
    {
        ArenaPet * pet = g_ArenaPetManagementInfo.availablePets[i]; 
        for( j = 0 ; j < MAX_OF_PET_TYPE ; j++ )
        {
            if( pet->showAsInCurrentlySelectedArmy[j] )
                selectedCnt++; 
        }
    }
    pktSendBitsPack(pak,1, selectedCnt ); //I'm sending this many infos
    for( i = 0 ; i < eaSize( &g_ArenaPetManagementInfo.availablePets ) ; i++ )
    {
        ArenaPet * pet = g_ArenaPetManagementInfo.availablePets[i]; 
        for( j = 0 ; j < MAX_OF_PET_TYPE ; j++ )
        {
            if( pet->showAsInCurrentlySelectedArmy[j] )
                pktSendBitsPack( pak, 1, g_ArenaPetManagementInfo.availablePets[i]->tokenID );
        }
    }
    END_INPUT_PACKET;
}

//------------------------------------------------------------
//  After you request pet army info, handle server sending you the info here
//  also if you update the pet army list, receive ack here
//----------------------------------------------------------

void receiveArenaManagePets(Packet * pak)
{
    int i;

    /////////Get all the available pets. 
    {
        int availablePetCount;

        //Clear out old
        for( i = 0 ; i < eaSize( &g_ArenaPetManagementInfo.availablePets ) ; i++ )
        {
            ArenaPet * info = g_ArenaPetManagementInfo.availablePets[i];
            if( info->name )
                free( info->name );
            free( info );
        }
        eaDestroy( &g_ArenaPetManagementInfo.availablePets );


        //Read in new 
        availablePetCount = pktGetBitsPack(pak,1); 
        for( i = 0 ; i < availablePetCount ; i++ )
        {
            ArenaPet * info = calloc( sizeof(ArenaPet), 1 );
            info->name        = strdup( pktGetString(pak) );
            info->cost        = pktGetBitsPack(pak,1);
            info->tokenID    = pktGetBitsPack(pak,1);
            eaPush( &g_ArenaPetManagementInfo.availablePets, info );
        }
    }

    /////////Then Get all pets currently in your army
    {
        int currentPetCount;

        //Clear out old
        for( i = 0 ; i < eaSize( &g_ArenaPetManagementInfo.currentPets ) ; i++ )
        {
            ArenaPet * info = g_ArenaPetManagementInfo.currentPets[i];
            if( info->name )
                free( info->name );
            free( info );
        }
        eaDestroy( &g_ArenaPetManagementInfo.currentPets );


        //Read in new 
        currentPetCount = pktGetBitsPack(pak,1); 
        for( i = 0 ; i < currentPetCount ; i++ )
        {
            ArenaPet * info = calloc( sizeof(ArenaPet), 1 );
            info->name        = strdup( pktGetString(pak) );
            info->cost        = pktGetBitsPack(pak,1);
            info->tokenID    = pktGetBitsPack(pak,1);  //TO DO this is all I really need, I think
            eaPush( &g_ArenaPetManagementInfo.currentPets, info );
        }
    }    

    //Tag whether this is one of your current pets (quick and dirty, because it the UI is almost sure to change
    for( i = 0 ; i < eaSize( &g_ArenaPetManagementInfo.availablePets ) ; i++ )
    {
        int j;
        ArenaPet * info = g_ArenaPetManagementInfo.availablePets[i];

        for( j = 0 ; j < eaSize( &g_ArenaPetManagementInfo.currentPets ) ; j++ )
        {
            ArenaPet * infoCurr = g_ArenaPetManagementInfo.currentPets[j];
            if( infoCurr->tokenID == info->tokenID )
            {
                int k; //Fill next slot
                for( k = 0 ; k < MAX_OF_PET_TYPE ; k++ )
                {
                    if( !info->showAsInCurrentlySelectedArmy[k] )
                    {
                        info->showAsInCurrentlySelectedArmy[k] = 1;
                        break;
                    }
                }
            }
        }
    }


    ///////// Set the UI WIndow state
    
    if( game_state.pickPets == PICKPET_NOWINDOW ) //
    {
        window_setMode( WDW_ARENA_GLADIATOR_PICKER, WINDOW_GROWING );
        game_state.pickPets = PICKPET_WINDOWOPEN;
    }
    else if( game_state.pickPets == PICKPET_WAITINGFORACKTOCLOSE ) 
    {
        //TO DO if I already have the window up, this is an ack, so say "Server says here is your new list"
        window_setMode( WDW_ARENA_GLADIATOR_PICKER, WINDOW_SHRINKING );
        game_state.pickPets = PICKPET_NOWINDOW; 
    }
    else if( game_state.pickPets == PICKPET_WINDOWOPEN )
    {
        //TO DO if I already have the window up, this is an ack, so say "Server says here is your new list"
    }
}

//Individual message to owner when pet's state changes in a way only owner needs to see in his UI
// I'm not sure it should go here
void receiveUpdatePetState(Packet * pak)
{
    Entity * pet;
    int svr_idx;
    int specialPowerState;

    svr_idx = pktGetBitsPack( pak, 1 );
    specialPowerState = pktGetBitsPack( pak, 1 );

    pet = validEntFromId( svr_idx );
    if( pet )
        pet->specialPowerState = specialPowerState; //Is your special power available?
}

/////END PET ARMY MANAGEMENT ////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------------------------------
// crafting
//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------
//  send the passed rarity and idx to the server as a reverse
// engineer command.
//
// -AB: created :2005 Feb 28 04:23 PM
//----------------------------------------------------------
void crafting_reverseengineer_send(int inv_idx )
{
    START_INPUT_PACKET(pak, CLIENTINP_CRAFTING_REVERSE_ENGINEER );
    salvageinv_ReverseEngineer_Send( pak, inv_idx );
    END_INPUT_PACKET;
}

//------------------------------------------------------------
//  select a recipe from inventory to use in invention,
// to be invented at the passed level (i.e. a level 20 boost)
//----------------------------------------------------------
void invent_SendSelectrecipe(int recipeInvIdx, int inventionlevel)
{
    START_INPUT_PACKET(pak, CLIENTINP_INVENT_SELECTRECIPE);
    pktSendBitsPack( pak, INVENTION_RECIPEINVIDX_PACKBITS, recipeInvIdx);
    pktSendBitsPack( pak, INVENTION_INVENTIONLEVEL_PACKBITS, inventionlevel); 
    END_INPUT_PACKET;
}

void invent_SendSlotconcept(int slotIdx, int conceptInvIdx)
{
    START_INPUT_PACKET(pak, CLIENTINP_INVENT_SLOTCONCEPT);
    pktSendBitsPack( pak, INVENTION_SLOTIDX_PACKBITS, slotIdx);
    pktSendBitsPack( pak, INVENTION_CONCEPTINVIDX_PACKBITS, conceptInvIdx);
    END_INPUT_PACKET;
}

void invent_SendUnSlot(int slotIdx)
{
    START_INPUT_PACKET(pak, CLIENTINP_INVENT_UNSLOT);
    pktSendBitsPack( pak, INVENTION_SLOTIDX_PACKBITS, slotIdx);
    END_INPUT_PACKET;
}

void invent_SendHardenslot(int slotIdx)
{
    START_INPUT_PACKET(pak, CLIENTINP_INVENT_HARDENSLOT);
    pktSendBitsPack( pak, INVENTION_SLOTIDX_PACKBITS, slotIdx); 
    END_INPUT_PACKET;
}

void invent_SendFinalize()
{
    START_INPUT_PACKET(pak, CLIENTINP_INVENT_FINALIZE);
    END_INPUT_PACKET;
}

void invent_SendCancel()
{
    START_INPUT_PACKET(pak, CLIENTINP_INVENT_CANCEL);
    END_INPUT_PACKET;
}

//------------------------------------------------------------
//  send the recipe to create a detail from the id specified
//----------------------------------------------------------
void workshop_SendCreate(const DetailRecipe *pRecipe, int iLevel, int bUseCoupon)
{
    START_INPUT_PACKET(pak, CLIENTINP_WORKSHOP_CREATE);
    pktSendString(pak, pRecipe->pchName);
    pktSendBitsAuto(pak, iLevel);
    pktSendBitsAuto(pak, bUseCoupon);
    END_INPUT_PACKET;
}

//------------------------------------------------------------
//  send the command to purchase the specified recipe
//----------------------------------------------------------
void workshop_SendBuy(const DetailRecipe *pRecipe)
{
    START_INPUT_PACKET(pak, CLIENTINP_BUY_RECIPE);
    pktSendString(pak, pRecipe->pchName);
    END_INPUT_PACKET;
}

//------------------------------------------------------------
//  send the command to purchase the specified recipe
//----------------------------------------------------------
void workshop_SendClose(void)
{
    START_INPUT_PACKET(pak, CLIENTINP_WORKSHOP_CLOSE);
    END_INPUT_PACKET;
}

// Close or open base interaction windows as needed
void receiveBaseInteract(Packet * pak)
{
    static struct DetailFunctionWdwPair
    {
        DetailFunction df;
        WindowName wdw;
    } s_baseWdws[] = 
        {
            { kDetailFunction_Workshop,                WDW_RECIPEINVENTORY },
            { kDetailFunction_StorageSalvage,        WDW_BASE_STORAGE },
            { kDetailFunction_StorageInspiration,    WDW_BASE_STORAGE },
            { kDetailFunction_StorageEnhancement,    WDW_BASE_STORAGE },
        };

    Entity *e = playerPtr();
    bool activate = pktGetBitsPack(pak, 1);
    int detailID =  pktGetBitsPack(pak, 5);
    int interactionType = pktGetBitsPack(pak,3);
    int i;
    bool interactionHandled = false;

    if(e && e->pl)
    {
        if(activate)
            e->pl->detailInteracting = detailID;
        else
            e->pl->detailInteracting = 0;
    }

    for( i = 0; i < ARRAY_SIZE( s_baseWdws ); ++i ) 
    {
        if( interactionType == s_baseWdws[i].df )
        {
            if (activate && !windowUp(s_baseWdws[i].wdw))
            {
                if (s_baseWdws[i].wdw == WDW_RECIPEINVENTORY)
                    requestArchitectInventory();

                window_setMode(s_baseWdws[i].wdw, WINDOW_GROWING);
            }
            else 
            {
                window_setMode(s_baseWdws[i].wdw, WINDOW_SHRINKING);
            }
            interactionHandled = true;
        }
    }

    
    if (interactionHandled)
    {
        // empty
    } 
    else if (interactionType == kDetailFunction_Contact || interactionType == kDetailFunction_AuxContact)
    {
        // Do something special, on the client
    }
    else if (interactionType == kDetailFunction_Store)
    {
        if (!activate)
            storeClose();
    }
}

void receiveBaseMode(Packet * pak)
{
    int mode = pktGetBitsPack(pak, 1);
    baseSetLocalMode(mode);
}

void receiveRaidState(Packet * pak)
{
    int mode = pktGetBitsPack(pak, 1);
    game_state.base_raid = mode;
}

//------------------------------------------------------------
//  Base stuff that doesn't directly modify base
//----------------------------------------------------------

void baseClientSetPos( float x, float y, float z )
{
    START_INPUT_PACKET(pak,CLIENTINP_BASE_SETPOS);
    pktSendF32( pak, x );
    pktSendF32( pak, y );
    pktSendF32( pak, z );
    END_INPUT_PACKET
}


void sendSearchRequest(void)
{
    START_INPUT_PACKET(pak,CLIENTINP_SEARCH);
    lfg_buildSearch(pak);
    END_INPUT_PACKET
}

// Player Rename if moved offline
void updatePlayerName(void*foo)
{

    START_INPUT_PACKET(pak,CLIENTINP_UPDATE_NAME);
    pktSendString(pak, dialogGetTextEntry());
    END_INPUT_PACKET

    dialogClearTextEntry();
}

void receiveUpdateName(Packet * pak)
{
    dialogChooseNameImmediate(0);
}


void receivePrestigeUpdate(Packet * pak)
{
    Entity * e = playerPtr();

    g_PrestigeDiff = pktGetBits(pak, 32 );
    if( e && e->supergroup )
        e->supergroup->prestige = g_CurrentPrestige-g_PrestigeDiff;
}


//------------------------------------------------------------
//  send the recipe to create a detail from the id specified
//----------------------------------------------------------
void basestorage_SendSalvageAdjust(int idStorage, const char *name, int amount)
{
    START_INPUT_PACKET(pak, CLIENTINP_BASESTORE_ADJ_SALVAGE_FROM_INV);
    pktSendBitsAuto(pak, idStorage);
    pktSendBitsPack(pak, 2, amount < 0 ? 1 : 0 ); // send sign seperately to save bits
    pktSendBitsAuto( pak, abs(amount) );
    pktSendString(pak, name);
    END_INPUT_PACKET;
}

void basestorage_SendInspirationAdjust(int idStorage, int col, int row, const char *pathInsp, int amount)
{
    START_INPUT_PACKET(pak, CLIENTINP_BASESTORAGE_ADJ_INSPIRATION_FROM_INV);
    pktSendBitsAuto(pak, idStorage);
    pktSendBitsPack(pak, 2, amount < 0 ? 1 : 0 ); // send sign seperately to save bits
    pktSendBitsAuto( pak, abs(amount) );
    pktSendBitsAuto( pak, col);
    pktSendBitsAuto( pak, row);
    if(amount < 0)
        pktSendString(pak, pathInsp);
    END_INPUT_PACKET;
}

void basestorage_SendEnhancementAdjust(int idStorage, const char *pathBasePower, int iLevel, int iNumCombines, int iInv, int amount)
{
    START_INPUT_PACKET(pak, CLIENTINP_BASESTORAGE_ADJ_ENHANCEMENT_FROM_INV);
    pktSendBitsAuto(pak, idStorage);
    pktSendBitsPack(pak, 2, amount < 0 ? 1 : 0 ); // send sign seperately to save bits
    pktSendBitsAuto( pak, abs(amount) );
    pktSendBitsAuto( pak, iInv);
    pktSendBitsAuto( pak, iLevel);
    pktSendBitsAuto( pak, iNumCombines);
    if(amount < 0)
        pktSendString(pak, pathBasePower);
    END_INPUT_PACKET;
}

void receiveEditCritterCostume(Packet *pak)
{
    setCritterCostume( pktGetBits(pak,32) );
}

void sendPowerDelete( int iPow )
{
    START_INPUT_PACKET(pak, CLIENTINP_DELETE_TEMPPOWER);
    pktSendBitsPack(pak, 8, iPow);
    END_INPUT_PACKET;
}

void sendSalvageDelete( const char * name, int amount, bool is_stored_salvage )
{
    START_INPUT_PACKET(pak, CLIENTINP_DELETE_SALVAGE);
    pktSendString(pak, name );
    pktSendBitsPack(pak, 1, amount);
    pktSendBitsAuto( pak, is_stored_salvage );
    END_INPUT_PACKET;
}

void sendRecipeDelete( const char * name, int amount )
{
    START_INPUT_PACKET(pak, CLIENTINP_DELETE_RECIPE);
    pktSendString(pak, name );
    pktSendBitsPack(pak, 1, amount);
    END_INPUT_PACKET;
}

void sendRecipeSell( const char * name, int amount )
{
    START_INPUT_PACKET(pak, CLIENTINP_SELL_RECIPE);
    pktSendString(pak, name );
    pktSendBitsPack(pak, 1, amount);
    END_INPUT_PACKET;
}

void sendSalvageSell( const char * name, int amount )
{
    START_INPUT_PACKET(pak, CLIENTINP_SELL_SALVAGE);
    pktSendString(pak, name );
    pktSendBitsPack(pak, 1, amount);
    END_INPUT_PACKET;
}

void sendSalvageOpen( const char * name, int amount )
{
    START_INPUT_PACKET(pak, CLIENTINP_OPEN_SALVAGE);
    pktSendString(pak, name );
    END_INPUT_PACKET;
}

void receiveSalvageOpenRequest(Packet *pak_in)
{
    char *salvage = pktGetString(pak_in); // strdup this if you get another string.
    int count = pktGetBitsAuto(pak_in);

#ifndef TEST_CLIENT
    // we're ignoring count for now, since all real-world applications of this tech will have 1.
    // ARM NOTE:  If we actually submit multiple opens, e->pl->last_certification_request is going to be a problem.
    salvage_confirmOpen(salvage_GetItem(salvage));
#endif
}

void receiveSalvageOpenResults(Packet *pak_in)
{
    char *package = strdup(pktGetString(pak_in));
    int productCount = pktGetBitsAuto(pak_in);
    int productIndex;
#ifdef TEST_CLIENT
    char *buffer = NULL;
    Entity *e = playerPtr();
#endif
    // HYBRID: Do appropriate window handling with rewards
#ifndef TEST_CLIENT
    setSalvageOpenResultsPackage(package);
#else
    addSystemChatMsg(textStd("received salvage open for %s", package), INFO_SVR_COM, 0);
    if (IsTestAccountServer_SuperPacksOptionActivated())
    {
        char *tempBuffer = 0;
        estrPrintf(&tempBuffer, ",%s", e->auth_name);
        estrConcatEString(&buffer, tempBuffer);
    }
#endif

    for (productIndex = 0; productIndex < productCount; productIndex++)
    {
        char *product = strdup(pktGetString(pak_in));
        int quantity = pktGetBitsAuto(pak_in);
        const char *rarity = StaticDefineIntRevLookup(AccountTypes_RarityEnum, pktGetBitsAuto(pak_in));

#ifndef TEST_CLIENT
        setSalvageOpenResultsDesc(product, quantity, rarity);
#else
        addSystemChatMsg(textStd("received new product %.8s %d %s", product, quantity, rarity), INFO_SVR_COM, 0);
        if (IsTestAccountServer_SuperPacksOptionActivated())
        {
            char *tempBuffer = 0;
            estrPrintf(&tempBuffer, ",card %d,%.8s,%d,%s",
                productIndex + 1, product, quantity, rarity);
            estrConcatEString(&buffer, tempBuffer);
        }
#endif

        SAFE_FREE(product);
    }

#ifndef TEST_CLIENT
    window_setMode( WDW_SALVAGE_OPEN, WINDOW_GROWING ); 
#else
    if (IsTestAccountServer_SuperPacksOptionActivated())
    {
        estrConcatStaticCharArray(&buffer, "\r\n");
        LOG(LOG_TEST_CLIENT, LOG_LEVEL_IMPORTANT, 0, buffer);
        estrDestroy(&buffer);
    }
#endif

    SAFE_FREE(package);
}

//----------------------------------------------------
// auction house stuff
//----------------------------------------------------

void auction_updateAuctionBannedStatus(void)
{
    START_INPUT_PACKET(pak, CLIENTINP_AUCTION_REQ_BANNEDUPDATE);
    END_INPUT_PACKET;
}

void auction_updateInventory(void)
{
    START_INPUT_PACKET(pak, CLIENTINP_AUCTION_REQ_INVUPDATE);
    END_INPUT_PACKET;
}

void auction_updateHistory(const char * pchIdent)
{
    START_INPUT_PACKET(pak, CLIENTINP_AUCTION_REQ_HISTORYINFO);
    pktSendString(pak, pchIdent);
    END_INPUT_PACKET;
}

void auction_addItem(const char * pchIdent, int index, int amt, int price, int status, int id)
{
    START_INPUT_PACKET(pak, CLIENTINP_AUCTION_ADDITEM);
    pktSendString(pak, pchIdent);
    pktSendBitsPack(pak, 1, index);
    pktSendBitsPack(pak, 1, amt);
    pktSendBitsPack(pak, 1, price);
    pktSendBitsPack(pak, 1, status);
    pktSendBitsPack(pak, 1, id);
    END_INPUT_PACKET;
}

void uiAuctionHandleBannedUpdate(Packet *pak)
{
    gAuctionDisabled = pktGetBits(pak, 1);
}


void auction_placeItemInInventory(int auction_id)
{
    START_INPUT_PACKET(pak, CLIENTINP_AUCTION_GETITEM);
    pktSendBitsPack(pak, 1, auction_id);
    END_INPUT_PACKET;
}

void auction_changeItemStatus(int auction_id, int price, int status)
{
    START_INPUT_PACKET(pak, CLIENTINP_AUCTION_CHANGEITEMSTATUS);
    pktSendBitsPack(pak, 1, auction_id);
    pktSendBitsPack(pak, 1, price);
    pktSendBitsPack(pak, 1, status);
    END_INPUT_PACKET;
}

void auction_getInfStored(int auction_id)
{
    START_INPUT_PACKET(pak, CLIENTINP_AUCTION_GETINFSTORED);
    pktSendBitsPack(pak, 1, auction_id);
    END_INPUT_PACKET;
}

void auction_getAmtStored(int auction_id)
{
    START_INPUT_PACKET(pak, CLIENTINP_AUCTION_GETAMTSTORED);
    pktSendBitsPack(pak, 1, auction_id);
    END_INPUT_PACKET;
}

void auction_batchRequestItemStatus(int startIdx, int reqSize)
{
    START_INPUT_PACKET(pak, CLIENTINP_AUCTION_BATCH_REQ_ITEMSTATUS);
    pktSendBitsPack(pak, 1, startIdx);
    pktSendBitsPack(pak, 1, reqSize);
    END_INPUT_PACKET;
}

void auction_listRequestItemStatus( int  *idlist)
{
    int i;
    START_INPUT_PACKET(pak, CLIENTINP_AUCTION_LIST_REQ_ITEMSTATUS);
    pktSendBitsAuto(pak, eaiSize(&idlist) );
    for( i = 0; i < eaiSize(&idlist); i++ )
        pktSendBitsAuto(pak, idlist[i] );
    END_INPUT_PACKET;
}

//----------------------------------------------------
// Mission Search
//----------------------------------------------------

#include "UI/uiMissionSearch.h"
#include "UI/uiMissionMaker.h"

#ifdef TEST_CLIENT
#    define missionsearch_TabClear(...)
#    define missionsearch_TabAddLine(...)
#    define missionsearch_TabAddSection(tab,sect)    (sect)    // let the pktGet call pass through
#    define missionsearch_SetArcInfo(...)
#    define missionsearch_ArcDeleted(...)
#    define missionsearch_SetArcData(...)
#    define missioncomment_ClearAdmin(...)
#endif

void missionserver_game_publishArc(int arcid, char *arcstr)
{
    START_INPUT_PACKET(pak, CLIENTINP_MISSIONSERVER_PUBLISHARC);
    pktSendBitsAuto(pak, arcid);
    pktSendString(pak, arcstr);
    END_INPUT_PACKET;
}

void missionserver_game_unpublishArc(int arcid)
{
    START_INPUT_PACKET(pak, CLIENTINP_MISSIONSERVER_UNPUBLISHARC);
    pktSendBitsAuto(pak, arcid);
    END_INPUT_PACKET;
}

void missionserver_game_voteForArc(int arcid, MissionRating vote)
{
    START_INPUT_PACKET(pak, CLIENTINP_MISSIONSERVER_VOTEFORARC);
    pktSendBitsAuto(pak, arcid);
    pktSendBitsAuto(pak, vote);
    END_INPUT_PACKET;
}

void missionserver_game_setKeywordsForArc( int arcid, int votefield )
{
    START_INPUT_PACKET(pak, CLIENTINP_MISSIONSERVER_SETKEYWORDSFORARC);
    pktSendBitsAuto(pak, arcid);
    pktSendBitsAuto(pak, votefield);
    END_INPUT_PACKET;
}

void missionserver_game_reportArc(int arcid, int type, char *comment)
{
    START_INPUT_PACKET(pak, CLIENTINP_MISSIONSERVER_REPORTARC);
    pktSendBitsAuto(pak, arcid);
    pktSendBitsAuto(pak, type);
    pktSendString(pak, comment);
    END_INPUT_PACKET;
}

void missionserver_game_requestSearchPage(MissionSearchPage category, char *context, int page, int playedFilter, int levelFilter, int authorArcId)
{
    START_INPUT_PACKET(pak, CLIENTINP_MISSIONSERVER_SEARCHPAGE);
    pktSendBitsAuto(pak, category);
    pktSendString(pak, context);
    pktSendBitsAuto(pak, page);
    pktSendBitsAuto(pak, playedFilter);
    pktSendBitsAuto(pak, levelFilter);
    pktSendBitsAuto(pak, authorArcId);
    END_INPUT_PACKET;
}

void missionserver_game_requestArcInfo(int arcid)
{
    START_INPUT_PACKET(pak, CLIENTINP_MISSIONSERVER_ARCINFO);
    pktSendBitsAuto(pak, arcid);
    END_INPUT_PACKET;
}

void missionserver_game_requestArcData_playArc(int arcid, int test, int dev_choice)
{
    START_INPUT_PACKET(pak, CLIENTINP_MISSIONSERVER_ARCDATA_PLAYARC);
    pktSendBitsAuto(pak, arcid);
    pktSendBits(pak, 1, test);
    pktSendBits(pak, 1, dev_choice);
    END_INPUT_PACKET;
}

void missionserver_game_requestArcData_viewArc(int arcid)
{
    START_INPUT_PACKET(pak, CLIENTINP_MISSIONSERVER_ARCDATA_VIEWARC);
    pktSendBitsAuto(pak, arcid);
    END_INPUT_PACKET;
}

void missionserver_game_comment(int arcid, char * msg)
{
    START_INPUT_PACKET(pak, CLIENTINP_MISSIONSERVER_COMMENT);
    pktSendBitsAuto(pak, arcid);
    pktSendString(pak, msg);
    END_INPUT_PACKET;
}

static void s_receiveArcInfo(Packet *pak_in, int arcid, MissionSearchPage category)
{
    int mine = pktGetBool(pak_in);
    int played = pktGetBool(pak_in);
    int myVote = pktGetBitsAuto(pak_in);
    int ratingi = pktGetBitsAuto(pak_in);
    int total_votes = pktGetBitsAuto(pak_in);
    U32 keyword_votes = pktGetBitsAuto(pak_in);
    U32 date = pktGetBitsAuto(pak_in);
    char *header =  strdup(pktGetString(pak_in));
    int count = pktGetBitsAuto(pak_in);
    while(count--)
    {
        int type = pktGetBitsAuto(pak_in);
        char * name = 0;
        char * comment;

        if( type == kComplaint_Comment )
            name = strdup(pktGetString(pak_in));

        comment = pktGetString(pak_in);
#ifndef TEST_CLIENT
        missioncomment_Add(mine, arcid, type, name, comment);
#endif
        SAFE_FREE(name);
    }
    missionsearch_TabAddLine(category, arcid, mine, played, myVote, ratingi, total_votes, keyword_votes, date, header);
#if defined(TEST_CLIENT)
    testMissionSearch_receiveArc(arcid, header, ratingi);
#endif
    free(header);
}

void missionserver_game_receiveSearchPage(Packet *pak_in)
{
    MissionSearchPage category = pktGetBitsAuto(pak_in);
    char *context = pktGetString(pak_in);
    int page = pktGetBitsAuto(pak_in);
    int pages = pktGetBitsAuto(pak_in);
    int arcid;

    missionsearch_TabClear(category, context, page, pages);
    missioncomment_ClearAdmin(0);
    while(arcid = pktGetBitsAuto(pak_in))
    {
        if(arcid < 0)
            missionsearch_TabAddSection(category, pktGetString(pak_in)); // destroys 'context'
        else
            s_receiveArcInfo(pak_in, arcid, category);
    }

#ifndef TEST_CLIENT
    missionsearch_SortResults(category);
#else
    testMissionSearch_registerPages(page, pages);
    #endif
}

void missionserver_game_receiveArcInfo(Packet *pak_in)
{
    int arcid = pktGetBitsAuto(pak_in);
    missioncomment_ClearAdmin(arcid);
    if(pktGetBool(pak_in))
        s_receiveArcInfo(pak_in, arcid, MISSIONSEARCH_NOPAGE);
    else
        missionsearch_ArcDeleted(arcid);
}

void missionserver_game_receiveArcData(Packet *pak_in)
{
    int arcid = pktGetBitsAuto(pak_in);
    if(pktGetBool(pak_in))
    {
        char *arcdata = pktGetZipped(pak_in, NULL);
#ifndef TEST_CLIENT
        missionsearch_SetArcData(arcid, arcdata);

        if(arcid == game_state.architect_dumpid)
        {
            char *buf = estrTemp();
            PlayerCreatedStoryArc *arc = playerCreatedStoryArc_FromString(arcdata, NULL, 0, 0, 0, 0);
            estrPrintf(&buf, "%s/arc%d%s", missionMakerPath(), arcid, missionMakerExt());
            if(!arc)
                conPrintf("Damaged arc, could not decode");
            else if(!playerCreatedStoryArc_Save(arc, buf))
                conPrintf("Error writing file\n");
            else
                conPrintf("Success");
            game_state.architect_dumpid = 0;
            StructDestroy(ParsePlayerStoryArc, arc);
            estrDestroy(&buf);
        }
#else
        testMissionSearch_registerData(arcdata);
#endif
        SAFE_FREE(arcdata);
    }
    else
    {
        missionsearch_ArcDeleted(arcid);
    }
}

void missionserver_game_receiveArcData_otherUser(Packet *pak_in)
{
    int arcid = pktGetBitsAuto(pak_in);
    char *arcdata = pktGetZipped(pak_in, NULL);
    char *buf = estrTemp();
    PlayerCreatedStoryArc *arc = playerCreatedStoryArc_FromString(arcdata, NULL, 0, 0, 0, 0);
    estrPrintf(&buf, "%s/csrarc%d%s", missionMakerPath(), arcid, missionMakerExt());
    if(!arc)
        conPrintf("Damaged arc, could not decode");
    else if(!playerCreatedStoryArc_Save(arc, buf))
        conPrintf("Error writing file\n");
    else
        conPrintf("Success");
    if(arc)
        StructDestroy(ParsePlayerStoryArc, arc);
    estrDestroy(&buf);
    SAFE_FREE(arcdata);
}

void receiveArchitectKioskSetOpen(Packet * pak)
{
    int open = pktGetBits(pak, 1);

#ifndef TEST_CLIENT
    if(open)
        missionsearch_PlayMission(0);
#endif
}

void receiveArchitectComplete(Packet *pak)
{
    int id = pktGetBitsAuto(pak);
    missionReview_Complete(id);
}

void receiveArchitectSouvenir(Packet *pak)
{
    char *name, *desc;
    int id = pktGetBitsAuto(pak);
    name = strdup(pktGetString(pak));
    desc = strdup(pktGetString(pak));
    addPlayerCreatedSouvenir( id, name, desc );
}

void receiveArchitectInventory(Packet *pak)
{
    Entity *e = playerPtr();
    U32 bannedUntil = pktGetBitsAuto(pak);
    U32 tickets = pktGetBitsAuto(pak);
    U32 publishSlots = pktGetBitsAuto(pak);
    U32 publishSlotsUsed = pktGetBitsAuto(pak);
    U32 permanent = pktGetBitsAuto(pak);
    U32 invalid = pktGetBitsAuto(pak);
    U32 playableErrors = pktGetBitsAuto(pak);
    U32 uncirculatedSlots = pktGetBitsAuto(pak);
    int unlockKeys = pktGetBitsAuto(pak);
    int count;
    char *key;

    if(e)
    {
        if(!e->mission_inventory)
        {
            e->mission_inventory = calloc(1, sizeof(MissionInventory));
            e->mission_inventory->stKeys = stashTableCreateWithStringKeys(8,StashDefault);
        }

        e->mission_inventory->banned_until = bannedUntil;
        e->mission_inventory->claimable_tickets = tickets;
        e->mission_inventory->publish_slots = publishSlots;
        e->mission_inventory->publish_slots_used = publishSlotsUsed;
        e->mission_inventory->permanent = permanent;
        e->mission_inventory->playableErrors = playableErrors;
        e->mission_inventory->invalidated = invalid;
        e->mission_inventory->uncirculated_slots = uncirculatedSlots;
        eaDestroyEx(&e->mission_inventory->unlock_keys, NULL);
        stashTableClear(  e->mission_inventory->stKeys );
    }

    for(count = 0; count < unlockKeys; count++)
    {
        key = pktGetString(pak);

        if(e && e->mission_inventory)
        {
            char * pKey = strdup(key);
            eaPush(&e->mission_inventory->unlock_keys, pKey );
            stashAddPointer( e->mission_inventory->stKeys, pKey, pKey, 0 );
            playerCreatedStoryArc_AwardPCCCostumePart( e, pKey);
        }
    }
    recipeInventoryReparse();
#ifndef TEST_CLIENT
    missionsearch_InvalidWarning(invalid, playableErrors);
#endif
    e->mission_inventory->dirty = false;
}

void requestArchitectInventory(void)
{
    START_INPUT_PACKET(pak, CLIENTINP_ARCHITECTKIOSK_REQUESTINVENTORY);
    END_INPUT_PACKET;
}

//----------------------------------------------------
// Stored Salvage
//----------------------------------------------------

void storedsalvage_SendMoveFromPlayerInv(int amt, char *name) // AmountSliderOKCallback
{
    START_INPUT_PACKET(pak, CLIENTINP_MOVE_SALVAGE_TO_PERSONALSTORAGE);
    pktSendBitsAuto( pak, amt);
    pktSendString(pak, name);
    END_INPUT_PACKET;
}

void storedsalvage_SendMoveToPlayerInv(int amt, char *name) // AmountSliderOKCallback
{
    START_INPUT_PACKET(pak, CLIENTINP_MOVE_SALVAGE_FROM_PERSONALSTORAGE);
    pktSendBitsAuto( pak, amt);
    pktSendString(pak, name);
    END_INPUT_PACKET;
}

//----------------------------------------------------
// AccountServer
//----------------------------------------------------

void receiveAccountServerClientAuth(Packet *pak)
{
#if !defined(TEST_CLIENT)

    U32 timeStamp = pktGetBitsAuto(pak);
    char * digest = pktGetString(pak);
    //PlaySpanStoreLauncher_SetDigest(timeStamp, digest);
    memset(digest, 0, strlen(digest));
#endif
}

void receiveAccountServerPlayNCAuthKey(Packet *pak)
{
#if !defined(TEST_CLIENT)
    int request_key = pktGetBitsAuto(pak);
    char * auth_key = pktGetString(pak);
    //PlaySpanStoreLauncher_ReceiveAuthKeyResponse(request_key, auth_key);
#endif
}

void receiveAccountServerNotifyRequest(Packet *pak)
{
    char *msg;
    AccountRequestType reqType = pktGetBitsAuto(pak);
    OrderId order_id = { pktGetBitsAuto2(pak), pktGetBitsAuto2(pak) };
    bool success = pktGetBool(pak);
    strdup_alloca(msg,pktGetString(pak));

    // We don't know if a name changed or not, so we have to re-grab.
    dbResendPlayers();
    if (success && (reqType == kAccountRequestType_Slot))
    {
        unlockCharacterRightAwayIfPossible();
    }

    // FIXME: format this nicely in one window
    if(msg[0])
        dialogStd( DIALOG_OK, msg, NULL, NULL, 0, NULL, 0 );
}


void receiveAccountServerInventory(Packet *pak)
{
    int db_id = pktGetBitsAuto(pak);
    Entity *_e = playerPtr();
    char *shardName;
    int size, i;
    AccountInventory *pItem = NULL;
    bool playerValid = (_e && _e->pl && (db_id == _e->db_id)) != 0;
    EntPlayer fakeEntPlayer;
    EntPlayer *pl = playerValid ? _e->pl : &fakeEntPlayer;

    // destroy any old inventory information
    if (playerValid)
        AccountInventorySet_Release( &pl->account_inventory );
    else
        memset(&fakeEntPlayer, 0, sizeof(EntPlayer));

    //    We're loading data into the entity, so we can get rid
    //    of the copy which was loaded at login.
    AccountInventorySet_Release( &db_info.account_inventory );

    strdup_alloca(shardName, pktGetString(pak));

    pl->accountInformationAuthoritative = pktGetBits(pak, 1);    
    if (pl->accountInformationAuthoritative == 0)
        pl->accountInformationCacheTime = timerSecondsSince2000();
    else
        pl->accountInformationCacheTime = 0;

    size = pktGetBitsAuto(pak);
    for (i = 0; i < size; i++)
    {
        AccountInventory* pItem = (AccountInventory *) calloc(1,sizeof(AccountInventory));
        AccountInventoryReceiveItem(pak, pItem);
        if (!AccountInventoryAddProductData(pItem))
        {
            free(pItem);
            continue;
        }

        if (playerValid)
            eaPush(&pl->account_inventory.invArr, pItem);
        else
            free(pItem);
    }

    pktGetBitsArray(pak, LOYALTY_BITS, pl->loyaltyStatus);
    pl->loyaltyPointsUnspent = pktGetBitsAuto(pak);
    pl->loyaltyPointsEarned = pktGetBitsAuto(pak);
    pl->virtualCurrencyBal = pktGetBitsAuto(pak);
    pl->lastEmailTime = pktGetBitsAuto(pak);
    pl->lastNumEmailsSent = pktGetBitsAuto(pak);
    pl->account_inventory.accountStatusFlags = pktGetBitsAuto(pak);
    db_info.bonus_slots = pktGetBitsAuto(pak);
    
    if (playerValid)
    {
        AccountInventorySet_Sort( &pl->account_inventory );
        pl->account_inv_loaded = true;
        updateCertifications(1);
        recipeInventoryReparse();
    }
}

void uiNetGetPlayNCAuthKey(int request_key, char* auth_name, char* digest_data)
{
    START_INPUT_PACKET( pak, CLIENTINP_GET_PLAYNC_AUTH_KEY);
    pktSendBitsAuto(pak, request_key);
    pktSendString(pak, auth_name);
    pktSendString(pak, digest_data);
    END_INPUT_PACKET
}

static void renameBuildOKFunc(void *garbage)
{
    START_INPUT_PACKET(pak, CLIENTINP_RENAME_BUILD_RESPONSE );
    pktSendString(pak, dialogGetTextEntry());
    END_INPUT_PACKET
}

void receiveRenameBuild(Packet *pak)
{
    dialogStd( DIALOG_OK_CANCEL_TEXT_ENTRY, "EnterNewNameStr", 0, 0, renameBuildOKFunc, 0, 0 );
}

void receiveRenameResponse(Packet *pak)
{
    char *msg;

    msg = pktGetString(pak);

#if !defined(TEST_CLIENT)
    characterRenameReceiveResult(msg);
#endif
}

void receiveUnlockCharacterResponse(Packet *pak)
{
#if !defined(TEST_CLIENT)
    characterUnlockReceiveResult();
#endif
}

void receiveCheckNameResponse(Packet *pak)
{
    int CheckNameSuccess, locked, dummy = 0;

    CheckNameSuccess = pktGetBitsAuto(pak);
    locked = pktGetBitsAuto(pak);

#if !defined(TEST_CLIENT)
    hybridCheckNameReceiveResult(CheckNameSuccess, locked);
#else
    dummy = pktGetBits(pak, 1);
#endif
}

void receiveLoginDialogResponse(Packet *pak)
{
    char *msg;

    msg = pktGetString(pak);

#if !defined(TEST_CLIENT)
    charSelectDialog(msg);
#endif
}



static void flashbackListDestructor(void *pData)
{
//    int i, count;
    StoryarcFlashbackHeader *pHeader = (StoryarcFlashbackHeader *) pData;

    SAFE_FREE(pHeader->name);
    SAFE_FREE(pHeader->fileID);

/*
    count = eaSize(&pHeader->completeRequires);
    for (i = 0; i < count; i++)
        SAFE_FREE(pHeader->completeRequires[i]);
*/

}

void flashbackListClear(void)
{
    if (storyarcFlashbackList != NULL)    
    {
        eaDestroyEx(&storyarcFlashbackList, flashbackListDestructor);
        flashbackListRequested = false;
    }
}

// receive flashback list from server
void receiveFlashbackListResponse(Packet *pak)
{
    int size, i;

    flashbackListClear();
    eaCreate(&storyarcFlashbackList);

    size = pktGetBitsPack(pak, 1);
    for (i = 0; i < size; i++)
    {
        StoryarcFlashbackHeader *pHeader = malloc(sizeof(StoryarcFlashbackHeader));
        memset(pHeader, 0, sizeof(StoryarcFlashbackHeader));

        pHeader->fileID = strdup(pktGetString( pak ) );
        pHeader->bronzeTime = pktGetBitsAuto(pak);
        pHeader->silverTime = pktGetBitsAuto(pak);
        pHeader->goldTime = pktGetBitsAuto(pak);
        pHeader->name = strdup(pktGetString( pak ) );
        pHeader->minLevel = pktGetBitsPack(pak, 4);
        pHeader->maxLevel = pktGetBitsPack(pak, 4);
        pHeader->alliance = pktGetBitsPack(pak, 1);
        pHeader->costMultiplier = pktGetF32(pak);

        pHeader->complete = pktGetBitsPack(pak, 1);
        pHeader->completeEvaluated = true;

        eaPush(&storyarcFlashbackList, pHeader);
    }
}

// receive flashback detail about a particular SA from the server
void receiveFlashbackDetailResponse(Packet *pak)
{
    char *pID = strdup(pktGetString( pak ));
    char *pTitle = strdup(pktGetString( pak ));
    char *pAccept = strdup(pktGetString( pak ));

    uiContactDialogAddTaskDetail(pID, pTitle, pAccept);

    SAFE_FREE(pID);
    SAFE_FREE(pTitle);
    SAFE_FREE(pAccept);
}

void receiveFlashbackEligibilityResponse(Packet *pak)
{
#ifndef TEST_CLIENT
    uiContactAdvanceToChallengeWindow();
#endif
}

// receive flashback details about the chosen mission and prompt the player to accept or reject the
//        current settings.
void receiveFlashbackConfirmResponse(Packet *pak)
{
/*
    char *pID = strdup(pktGetString( pak ));
    char *pTitle = strdup(pktGetString( pak ));
    char *pAccept = strdup(pktGetString( pak ));

    uiContactDialogAddTaskDetail(pID, pTitle, pAccept);

    SAFE_FREE(pID);
    SAFE_FREE(pTitle);
    SAFE_FREE(pAccept);
*/
}

void receiveTaskforceTimeLimits(Packet *pak)
{
    U32 bronze = pktGetBitsAuto( pak );
    U32 silver = pktGetBitsAuto( pak );
    U32 gold = pktGetBitsAuto( pak );

    uiContactDialogSetChallengeTimeLimits( bronze, silver, gold );
}

void sendTestStoryArc(PlayerCreatedStoryArc *pArc)
{
    START_INPUT_PACKET(pak, CLIENTINP_STORYARC_CREATE );
    playerCreatedStoryArc_Send(pak, pArc );
    END_INPUT_PACKET
}


void receiveAuthBits(Packet *pak)
{
    Entity* e = playerPtr();
    int i;

    if (devassert(e->pl))
    {
        for (i = 0; i < 4; i++)
        {
            e->pl->auth_user_data[i] = pktGetBitsAuto(pak);
        }
    }
}

void sendCostumeChangeEmoteListRequest()
{
    START_INPUT_PACKET(pak, CLIENTINP_COSTUMECHANGEEMOTELIST_REQUEST);
    END_INPUT_PACKET
}

static char sPromotedPlayerName[64];
static void promoteConfirmHandler(void *data)
{
    START_INPUT_PACKET(pak, CLIENTINP_DO_PROMOTE);
    pktSendString(pak, sPromotedPlayerName);
    END_INPUT_PACKET
}

void receiveConfirmSGPromote(Packet *pak)
{
    strncpyt(sPromotedPlayerName, pktGetString(pak), sizeof(sPromotedPlayerName));
    dialogStd( DIALOG_ACCEPT_CANCEL, textStd("PromoteConfirmStr", sPromotedPlayerName), NULL, NULL, promoteConfirmHandler, NULL, 0 );
}

//-----------------------------------------------------------------
// handle clientside knowledge of whether I can set SG permissions
//-----------------------------------------------------------------
static int s_canSetPermissions = -1;

void srRequestPermissionsSetBit()
{
    s_canSetPermissions = -1;
    START_INPUT_PACKET(pak, CLIENTINP_REQUEST_SG_PERMISSIONS);
    END_INPUT_PACKET
}

void srHandlePermissionsSetBit(Packet *pak)
{
    s_canSetPermissions = pktGetBits(pak, 1);
}

void srOverridePermissionsSetBit(int value)
{
    s_canSetPermissions = value;
}

int srCanISetPermissions()
{
    return s_canSetPermissions == 1;
}

int srCanISetThisPermission(SupergroupRank *ranks, int myRank, int permRank, int permIdx)
{
    int retval = 0;

    // Can only set permission for a valid lower rank than my own. No-one can
    // change permissions for Superleader, they can always do everything.
    if (ranks && myRank > permRank && permRank >= 0 && permRank < NUM_SG_RANKS_WITH_VARIABLE_PERMISSIONS)
    {
        // Even then, can only set permission if I have the ability to edit
        // rank properties in general and also that I have the permission in
        // question (eg. I can't affect the ability of someone lower than me
        // to edit the base if I don't have base editing privileges).
        if (s_canSetPermissions && TSTB(ranks[myRank].permissionInt, permIdx))
        {
            retval = 1;
        }
    }

    return retval;
}

void receiveGoingRogueNag(Packet *pak)
{
    GRNagContext ctx = pktGetBitsAuto(pak);

    if (okToShowGoingRogueNag(ctx))
        dialogGoingRogueNag(ctx);
}

void receiveIncarnateTrialStatus(Packet *pak)
{
    TrialStatus status = pktGetBitsAuto(pak);
    updateTrialStatus(status);

}

// DGNOTE - not needed yet.  Just don't remove this, I will want it eventually.
// Note to myself, use of this is commented out in clientcomm.c

// Handle command from server to turn on the "assert on bitstream error" flag
void receiveBSErrorsFlag(Packet *pak)
{
    int assertFlag = pktGetBits(pak, 1);
    bsAssertOnErrors(assertFlag);
}

void receiveChangeWindowColors(Packet *pak)
{
    int count;
    U32 oldcolor;
    U32 newcolor;
    int changed = 0;

    if (pak)
    {
        oldcolor = pktGetBitsAuto(pak);
        newcolor = pktGetBitsAuto(pak);

        for (count = 0; count < MAX_WINDOW_COUNT; count++)
        {
            if (winDefs[count].loc.color == oldcolor)
            {
                winDefs[count].loc.color = newcolor;
                sendWindowToServer(&winDefs[count].loc, count, 1);
            }
        }
    }
}

void displaySupportHome(Packet *pak)
{
#ifndef TEST_CLIENT
    //webStoreOpenSupport();
#endif
}

void displaySupportKB(Packet *pak)
{
#ifndef TEST_CLIENT
    pktGetBitsAuto(pak);
#endif
}

void sendNewFeatureOpenRequest(int id)
{
    START_INPUT_PACKET(pak, CLIENTINP_NEW_FEATURE_OPEN);
    pktSendBitsAuto(pak, id);
    END_INPUT_PACKET
}

void sendNewFeatureVersion(void)
{
    Entity *e = playerPtr();
    if (e && e->pl)
    {
        START_INPUT_PACKET(pak, CLIENTINP_UPDATE_NEW_FEATURE_VERSION);
        pktSendBitsAuto(pak, e->pl->newFeaturesVersion);
        END_INPUT_PACKET
    }
}

void displayProductPage(Packet *pak)
{
#ifndef TEST_CLIENT
    webStoreOpenProduct(pktGetString(pak));
#endif
}

