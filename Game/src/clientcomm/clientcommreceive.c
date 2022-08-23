//
// clientcommreceive.c 1/2/2003
//
// This file is helper for clientcomm, that deals with incoming data
// All receiving code should be located here, in order to make the external
// test client compile with few dependencies.
//-----------------------------------------------------------------------------

#include <utilitieslib/stdtypes.h>
#include "clientcomm/clientcommreceive.h"
#include "bases/DetailRecipe.h"
#include <utilitieslib/utils/utils.h>
#include "player/player.h"
#include "entity/entclient.h"
#include "UI/uiChat.h"
#include "UI/uiCursor.h"
#include "UI/uiDialog.h"
#include "UI/uiGame.h"
#include "UI/uiConsole.h"
#include "UI/uiCompass.h"
#include "UI/uiAutomapFog.h"
#include "UI/uiRecipeInventory.h"
#include "UI/uiIncarnate.h"
#include "UI/uiLogin.h"
#include <utilitieslib/network/netio.h>
#include "clientcomm/clientcomm.h"
#include "varutils.h"
#include "cmdparse/cmdgame.h"
#include "graphics/camera.h"
#include <utilitieslib/utils/timing.h>
#include "player/playerState.h"
#include "UI/sprite/sprite_font.h" // for font_9
#include "sound/sound.h"       // for sndPlay
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "UI/uiNet.h"
#include "win/win_init.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiUtil.h"
#include "gameComm/dooranimclient.h"
#include "demo.h"
#include "UI/uiFx.h"
#include "gameComm/wdwbase.h"
#include "UI/uiTitleSelect.h"
#include "UI/sprite/sprite_text.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "UI/uiOptions.h"
#include "UI/uiQuit.h"
#include "graphics/sun.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiReSpec.h"
#include "UI/uiLevelPower.h"
#include "UI/uiLevelSpec.h"
#include "win/hwlight.h"
#include "UI/uiContactFinder.h"
#include "UI/uiWindows.h"
#include "UI/Hybrid/uiWebStoreFrame.h"
#include <utilitieslib/components/EString.h>

// Remove these...
// Move related functionality into another file.
#include "storyarc/contactClient.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiContactDialog.h"
#include "UI/uiAutomap.h"

//
//
void receiveControlPlayer( Packet *pak )
{
    int controlled;
    int enable;
    int idx;
    Entity* e;

    controlled = pktGetBits(pak,1);

    enable = pktGetBits(pak,1);

    if(controlled)
    {
        setPlayerIsSlave(enable);
    }
    else
    {
        if(enable)
        {
            int control = pktGetBits(pak,1);

            idx = pktGetBitsPack(pak,1);

            e = ent_xlat[idx];

            setMySlave(e, pak, control);
        }
        else
        {
            setMySlave(NULL, NULL, 0);
        }
    }
}

//
//
void receiveInfoBox( Packet *pak )
{
    char *pchTmp;
    int type;

    type = pktGetBitsPack( pak, 2 );
    pchTmp = pktGetString( pak ); // Pointer good until the next pktGetString.

    InfoBox(type, "%s", pchTmp);
}

void ReceiveDoorAskYesHandler(void *cmd)
{
    cmdParse((char *) cmd);
}

//
//
void receiveDoorMsg( Packet *pak )
{
    int     cmd;
    char    *msg;
    // Need persistent storage for this.  256 bytes is enough, since the code that cooks this up, which live towards the bottom of enterDoor(...) over in door.c,
    // guarantees that this will be not be exceeded.  Even so, I use strncpyt( ... ) to copy into this guy.  You just can't be too careful.
    static char cmdstr[256];

    cmd = pktGetBitsPack( pak, 1 );
    msg = pktGetString(pak); // Pointer good until the next pktGetString.

    switch( cmd )
    {
        xcase DOOR_MSG_OK:
            doneWaitForDoorMode();
        xcase DOOR_MSG_FAIL:
        {
            doneWaitForDoorMode();
            if( msg[0] )
                dialogStd( DIALOG_OK, msg, NULL, NULL, NULL, NULL, 1 );
        }
        xcase DOOR_MSG_ASK:
        {
            // Make a persistent copy of the first string.  This kills two birds with one stone.  Firstly it frees up the buffer that pktGetString(pak)
            // uses, so we can call it again safely, and secondly, it means that when we finally call ReceiveDoorAskYesHandler() in response to a "Yes"
            // selection, the payload we care about will still be around.
            strncpyt(cmdstr, msg, sizeof(cmdstr));
            msg = pktGetString(pak); // Pointer good until the next pktGetString.
            doneWaitForDoorMode();
            dialog(DIALOG_YES_NO, -1, -1, -1, -1, msg, NULL, ReceiveDoorAskYesHandler, NULL, NULL, 0, NULL, NULL, 0, 0, 0, cmdstr );
        }

        xcase DOOR_MSG_DOOR_OPENING:
            setWaitForDoorMode();
    }
}


int context;

static void contactSendYesResponse(void *data)
{
    START_INPUT_PACKET( pak, CLIENTINP_CONTACT_DIALOG_RESPONSE );
    pktSendBitsPack( pak, 1, TRUE );
    pktSendBitsPack( pak, 1, context);
    END_INPUT_PACKET
}

static void contactSendNoResponse(void *data)
{
    START_INPUT_PACKET( pak, CLIENTINP_CONTACT_DIALOG_RESPONSE );
    pktSendBitsPack( pak, 1, FALSE );
    pktSendBitsPack( pak, 1, context);
    END_INPUT_PACKET
}

void ContactDialog( Packet *pak, int yesno )
{
    char* descstr = pktGetString(pak);
    int context = pktGetBitsPack(pak, 1);

    dialogStd(yesno?DIALOG_YES_NO:DIALOG_OK, descstr, NULL, NULL, contactSendYesResponse, yesno?contactSendNoResponse:NULL, 1 );
}

static void ContactDialogContextSendReply(int link, ContactDialogContext* dialogContext)
{
    START_INPUT_PACKET(pak, CLIENTINP_CONTACT_DIALOG_RESPONSE);
    pktSendBitsPack(pak, 1, link);
    END_INPUT_PACKET
}

void ContactSendCellCall(int handle)
{
    START_INPUT_PACKET( pak, CLIENTINP_CONTACT_CELL_CALL);
    pktSendBitsPack(pak, 1, handle);
    END_INPUT_PACKET
}

void ContactSendFlashbackListRequest(void)
{
    START_INPUT_PACKET( pak, CLIENTINP_CONTACT_FLASHBACK_LIST_REQUEST);
    END_INPUT_PACKET
}

void ContactSendFlashbackDetailRequest(char *fileID)
{
    START_INPUT_PACKET( pak, CLIENTINP_CONTACT_FLASHBACK_DETAIL_REQUEST);
    pktSendString(pak, fileID);
    END_INPUT_PACKET
}

void ContactSendFlashbackEligibilityCheckRequest(char *fileID)
{
    START_INPUT_PACKET( pak, CLIENTINP_CONTACT_FLASHBACK_CHECK_ELIGIBILITY_REQUEST);
    pktSendString(pak, fileID);
    END_INPUT_PACKET
}

void ContactSendFlashbackAcceptRequest(char *fileID, int timeLimits, int limitedLives, int powerLimits, int debuff, 
                                       int buff, int noEnhancements, int noInspirations)
{
    START_INPUT_PACKET( pak, CLIENTINP_CONTACT_FLASHBACK_ACCEPT_REQUEST);
    pktSendString(pak, fileID);
    pktSendBitsPack(pak, 3, timeLimits);
    pktSendBitsPack(pak, 3, limitedLives);
    pktSendBitsPack(pak, 3, powerLimits);
    pktSendBitsPack(pak, 1, debuff);
    pktSendBitsPack(pak, 1, buff);
    pktSendBitsPack(pak, 1, noEnhancements);
    pktSendBitsPack(pak, 1, noInspirations);
    END_INPUT_PACKET
}

void ContactSendTaskforceAcceptRequest(int timeLimits, int limitedLives, int powerLimits, int debuff, 
                                       int buff, int noEnhancements, int noInspirations)
{
    START_INPUT_PACKET( pak, CLIENTINP_CONTACT_TASKFORCE_ACCEPT_REQUEST);
    pktSendBitsPack(pak, 3, timeLimits);
    pktSendBitsPack(pak, 3, limitedLives);
    pktSendBitsPack(pak, 3, powerLimits);
    pktSendBitsPack(pak, 1, debuff);
    pktSendBitsPack(pak, 1, buff);
    pktSendBitsPack(pak, 1, noEnhancements);
    pktSendBitsPack(pak, 1, noInspirations);
    END_INPUT_PACKET
}

void receiveContactDialog( Packet *pak )
{
    int i;
    ContactResponseReceive(&dialogContext.response, pak);

    if(!dialogContext.responseOptions)
        eaCreate(&dialogContext.responseOptions);
    else
        eaClearEx(&dialogContext.responseOptions, ContactResponseOptionDestroy);

    for(i = 0; i < dialogContext.response.responsesFilled; i++)
    {
        ContactResponseOption* option = ContactResponseOptionCreate();
        eaPush(&dialogContext.responseOptions, option);
        Strncpyt(option->responsetext, dialogContext.response.responses[i].responsetext);
        Strncpyt(option->rightResponseText, dialogContext.response.responses[i].rightResponseText);
        option->link = dialogContext.response.responses[i].link;
        option->rightLink = dialogContext.response.responses[i].rightLink;
    }

    cd_SetBasic(dialogContext.response.type, dialogContext.response.dialog, dialogContext.responseOptions, ContactDialogContextSendReply, &dialogContext);
}

void receiveContactDialogClose( Packet *pak )
{
    extern int gEnterTailor;
    eaClearEx(&dialogContext.responseOptions, ContactResponseOptionDestroy);
    cd_clear();
    if (gEnterTailor)
    {
        window_setMode(WDW_COSTUME_SELECT, WINDOW_SHRINKING);
        gEnterTailor = 0;
    }
}

void receiveContactDialogOk( Packet *pak )
{
    ContactDialog(pak, 0);
}

void receiveContactDialogYesNo( Packet *pak )
{
    ContactDialog(pak, 1);
}

void receiveMissionEntryText( Packet *pak )
{
    char* text;

    text = pktGetString(pak);
    dialogStd(DIALOG_OK, text, NULL, NULL, NULL, NULL, 1);
}

void receiveMissionExitText( Packet *pak )
{
    char* text;

    text = pktGetString(pak);
    dialogStd(DIALOG_OK, text, NULL, NULL, NULL, NULL, 1);
}

void receiveBrokerAlert( Packet *pak )
{
    char* text = pktGetString(pak);
    dialogStd(DIALOG_OK, text, NULL, NULL, NULL, NULL, 1);
}

static bool tcNeverAsk = 0;
static DialogCheckbox teamCompleteDCB[] = { { "NeverAsk", &tcNeverAsk } };

static void respondTeamCompleteAccept(void *data)
{
    START_INPUT_PACKET( pak , CLIENTINP_TEAMCOMPLETE_RESPONSE )
    pktSendBits(pak, 1, 1);
    pktSendBits(pak, 1, tcNeverAsk);
    END_INPUT_PACKET
    if (tcNeverAsk)
    {
        optionSet(kUO_TeamComplete, 1, 1); // Always complete it
        dialogStd( DIALOG_OK, textStd("TeamCompleteCanChange"), NULL, NULL, NULL, NULL, 1 );
    }
}

static void respondTeamCompleteDecline(void *data)
{
    START_INPUT_PACKET( pak , CLIENTINP_TEAMCOMPLETE_RESPONSE )
    pktSendBits(pak, 1, 0);
    pktSendBits(pak, 1, tcNeverAsk);
    END_INPUT_PACKET
    if (tcNeverAsk)
    {
        optionSet(kUO_TeamComplete, 2, 1); // Never complete it
        dialogStd( DIALOG_OK, textStd("TeamCompleteCanChange"), NULL, NULL, NULL, NULL, 1 );
    }
}

void receiveTeamCompletePrompt( Packet *pak )
{
    tcNeverAsk = 0;
    dialog(DIALOG_YES_NO, -1, -1, -1, -1, textStd("TeamCompleteDescription"), textStd("YesString"),
           respondTeamCompleteAccept, textStd( "NoString" ), respondTeamCompleteDecline, 
           0, 0, teamCompleteDCB, 1, 0, 0, 0 );
}

static void responseToDeath(void *data)
{
    EMPTY_INPUT_PACKET( CLIENTINP_DEAD_NOGURNEY_RESPONSE );
}

void receiveDeadNoGurney( Packet *pak )
{
    dialogStd(DIALOG_OK, textStd("YouAreDead"), NULL, NULL, responseToDeath, NULL, 1 );
}

void receiveFaceEntity( Packet* pak )
{
    int targetID = pktGetBitsPack(pak, 3);

    setPlayerFacing(entFromId(targetID), TICKS_FOR_COMBAT_CAM_TRANSITIONS);
}

void receiveFaceLocation( Packet* pak )
{
    Vec3 vecTarget;

    vecTarget[0] = pktGetF32(pak);
    vecTarget[1] = pktGetF32(pak);
    vecTarget[2] = pktGetF32(pak);

    setPlayerFacingLocation(vecTarget, TICKS_FOR_COMBAT_CAM_TRANSITIONS);
}

// receive an alliance offer
void receiveCutSceneUpdate( Packet *pak )
{
    int camUpdated; 

    //Recv toggle to keep going with the cutscene or to end it
    game_state.viewCutScene = pktGetBits( pak, 1 );
    if( !game_state.viewCutScene )
        SAFE_FREE(game_state.cutSceneMusic);

    //recv Updated camera position, if any
    camUpdated = pktGetBits( pak, 1 ); 
    if( camUpdated )
    {
        Vec3 pyr;

        pyr[0] = pktGetF32(pak);
        pyr[1] = pktGetF32(pak);
        pyr[2] = pktGetF32(pak);

        createMat3YPR(game_state.cutSceneCameraMat,pyr);

        game_state.cutSceneCameraMat[3][0] = pktGetF32(pak);
        game_state.cutSceneCameraMat[3][1] = pktGetF32(pak);
        game_state.cutSceneCameraMat[3][2] = pktGetF32(pak);

        if (game_state.cutSceneLERPEnd = pktGetBitsAuto( pak))
        {
            copyMat4(game_state.cutSceneCameraMat, game_state.cutSceneCameraMat1);
            game_state.cutSceneLERPStart = timerSecondsSince2000();
            game_state.cutSceneLERPEnd += timerSecondsSince2000();
            pyr[0] = pktGetF32(pak);
            pyr[1] = pktGetF32(pak);
            pyr[2] = pktGetF32(pak);

            createMat3YPR(game_state.cutSceneCameraMat2,pyr);

            game_state.cutSceneCameraMat2[3][0] = pktGetF32(pak);
            game_state.cutSceneCameraMat2[3][1] = pktGetF32(pak);
            game_state.cutSceneCameraMat2[3][2] = pktGetF32(pak);

            game_state.cutSceneDepthOfField2 = pktGetF32(pak);
        }
        else
        {
            game_state.cutSceneLERPStart = game_state.cutSceneLERPEnd = 0;
        }
    }

    if(pktGetBits( pak, 1 ))
    {
        char * music = pktGetString(pak);
        SAFE_FREE(game_state.cutSceneMusic);
        if(!streq(music, "EndMusic"))
        {
            if(sndIsLoopingSample(music))
                game_state.cutSceneMusic = strdup(music); // call sndPlay repeatedly in the main thread
            else
                sndPlay(music, SOUND_MUSIC); // call once
        }
    }

    if(pktGetBits( pak, 1 ))
    {
        sndPlay(pktGetString(pak), SOUND_GAME);
    }


    game_state.cutSceneDepthOfField1 = game_state.cutSceneDepthOfField = pktGetF32(pak);

    if( game_state.cutSceneDepthOfField )
        setCustomDepthOfField(DOFS_SNAP, game_state.cutSceneDepthOfField, 0, game_state.cutSceneDepthOfField*0.3, 1, game_state.cutSceneDepthOfField*2, 1, 10000, &g_sun);
    else
        unsetCustomDepthOfField(DOFS_SNAP, &g_sun);
    
    //cutscene started, change game visiblity (in case the player is in a full screen menu)
    toggle_3d_game_modes( SHOW_GAME );
}

void cmdReceiveShortcuts(Packet *pak)
{
    int     idx;
    char    *s;

    for(;;)
    {
        idx = pktGetBitsPack(pak,1);
        if (idx < 0)
            break;
        s = pktGetString(pak);
        cmdOldSetShortcut(s,idx);
    }
}

void cmdReceiveServerCommands(Packet *pak)
{
    char * s;
    int i = 0, num = pktGetBitsPack(pak,1);
    
    for (;i<num;++i) 
    {
        s = pktGetString(pak);
        cmdAddServerCommand(s);
    }
}

//
//
void receiveFloatingDmg( Packet *pak )
{
    int giver, receiver, dmg;
    bool getloc;
    Vec3 loc;
    Entity *gave;
    Entity *received;
    char *pch = NULL;
    bool wasAbsorb;

    giver    = pktGetBitsPack(pak, 1);
    receiver = pktGetBitsPack(pak, 1);
    dmg      = pktGetBitsPack(pak, 1);
    pch      = dmg ? NULL : pktGetString(pak);
    getloc   = pktGetBits(pak,1);
    if(getloc)
    {
        loc[0] = pktGetF32(pak);
        loc[1] = pktGetF32(pak);
        loc[2] = pktGetF32(pak);
    }

    gave     = entFromId(giver);
    received = entFromId(receiver);
    wasAbsorb = pktGetBits(pak, 1);

    if(received)
    {
        demoSetEntIdx(giver);
        demoRecord("floatdmg %d %d \"%s\"",receiver,dmg,pch?pch:"");
        addFloatingDamage(received, gave, -dmg, pch, getloc?loc:NULL, wasAbsorb);
    }
}

static void handleAttentionFloater(U32 *color, char *pch) 
{
    int wdw = WDW_STAT_BARS;
    char *pchSound = NULL;

    if (*color == 0)
        *color = 0xffff00ff;

    if(stricmp(pch, "FloatLeveled")==0)
    {
        wdw = WDW_STAT_BARS;
        *color = 0xffff00ff;
        hwlightSignalLevelUp();
    }
    else if(stricmp(pch, "FloatFoundClue")==0)
    {
        wdw = WDW_CLUE;
        *color = CLR_PARAGON;
        pchSound = "Clue1";
    }
    else if(stricmp(pch, "FloatFoundEnhancement")==0 ||
        stricmp(pch, "FloatFoundBaseDetail")==0 )
    {
        wdw = WDW_TRAY;
        *color = CLR_PARAGON;
        pchSound = "Enhance1";
    }
    else if(stricmp(pch, "FloatEnhancementsFull")==0)
    {
        if (optionGet(kUO_HideEnhancementFull))
            pch = NULL;
        else
            wdw = WDW_TRAY;
    }            
    else if(stricmp(pch, "FloatFoundRecipe")==0)
    {
        wdw = WDW_RECIPEINVENTORY;
        pchSound = "Enhance1";
    }            
    else if(stricmp(pch, "FloatRecipesFull")==0)
    {
        if (optionGet(kUO_HideRecipeFull))
            pch = NULL;
        else
            wdw = WDW_RECIPEINVENTORY;
    }            
    else if(stricmp(pch, "FloatFoundSalvage")==0 || stricmp(pch, "FloatFoundInventSalvage")==0)
    {
        wdw = WDW_SALVAGE;
        //                *color = CLR_PARAGON;
        pchSound = "Enhance1";
    }
    else if(stricmp(pch, "FloatSalvageFull")==0)
    {
        if ( optionGet(kUO_HideSalvageFull) )
            pch = NULL;
        else
            wdw = WDW_SALVAGE;
    }
    else if(stricmp(pch, "FloatFoundInspiration")==0)
    {
        // Decided not to do a big visual fanfare for this.
        // wdw = WDW_INSPIRATION;
        // *color = CLR_PARAGON;
        // pchSound = Inspiration1";

        sndPlay("Inspiration1", SOUND_GAME);
        pch = NULL;
    }
    else if(stricmp(pch, "FloatInspirationsFull")==0)
    {
        if (optionGet(kUO_HideInspirationFull))
            pch = NULL;
        else
            wdw = WDW_INSPIRATION;
    }
    else if(stricmp(pch, "FloatMissionComplete")==0
        || stricmp(pch, "FloatTaskForceComplete")==0)
    {
        wdw = WDW_MISSION;
        *color = 0x22cc33ff;
        if( ENT_IS_IN_PRAETORIA(playerPtr()) )
            pchSound = "P_MissionComplete1";
        else if( ENT_IS_VILLAIN(playerPtr()) )
            pchSound = "V_MissionComplete1";
        else
            pchSound = "MissionComplete1";
        hwlightSignalMissionComplete();
    }
    else if(stricmp(pch, "FloatMissionFailed")==0)
    {
        wdw = WDW_MISSION;
        *color = 0xcc0000ff;
        pchSound = "MissionFailed1";
    }
    else if(stricmp(pch, "FloatEarnedBadge") == 0)
    {
        wdw = WDW_BADGES;
        *color = CLR_PARAGON;
        pchSound = "Clue1";    
#if !TEST_CLIENT
        recipeInventoryReparse();
        uiIncarnateReparse();
#endif
    }
    else if(stricmp(pch, "TreatFloat")==0)
    {
        wdw = WDW_TRAY;
        *color = CLR_ORANGE;
        pchSound = "Happy2";
    }
    else if(stricmp(pch, "TrickFloat")==0)
    {
        wdw = WDW_STAT_BARS;
        *color = CLR_RED;
        pchSound = "Reject1";
    }
    else if(stricmp(pch, "ArenaFight")==0)
    {
        pchSound = "MatchStart";
    }
    else if(stricmp(pch, "HaveWinner")==0)
    {
        pchSound = "EndMatch";
    }
    else if(stricmp(pch, "FloatCreateEnhancement")==0)
    {
        wdw = WDW_TRAY;
        pchSound = "EnhanceComplete";
    }
    else if( stricmp(pch, "FloatArchitectTicket") == 0 )
    {
        wdw = WDW_COMPASS;
    }
    else if(strstri(pch, "EnhancementConverted")) //assume the line "enhancement converted" should only appear for actual enhancement conversions
    {
        wdw = WDW_CONVERT_ENHANCEMENT;
        pchSound = "Enhance1";
    }

    if(pch)
        attentionText_add(pch, *color, wdw, pchSound);
}

void handleFloatingInfo(int iOwnerShownOver, char *pch, int iStyle, float fDelay, U32 color)
{
    if(iOwnerShownOver!=0)
    {
        demoSetEntIdx(iOwnerShownOver);
        demoSetAbsTimeOffset(fDelay * ABSTIME_SECOND * 60.0f);
        demoRecord("float %d \"%s\"",iStyle,pch);
        demoSetAbsTimeOffset(0);
        if(iStyle==kFloaterStyle_Info)
        {
            float fScale = (entFromId(iOwnerShownOver)==playerPtr() ? 1.0f : 1.8f);
            float fTime = (entFromId(iOwnerShownOver)==playerPtr() ? MAX_FLOAT_DMG_TIME/3.0f : MAX_FLOAT_DMG_TIME);

            if (color == 0)
                color = 0x90c8ea00; // steel blue

            if(stricmp(pch, "FloatNotEnoughEndurance")==0)
            {
                sndPlay( "endurance6", SOUND_GAME );
            }
            else if(stricmp(pch, "FloatOutOfRange")==0)
            {
                sndPlay( "range1", SOUND_GAME );
            }
            else if(stricmp(pch, "FloatRecharging")==0)
            {
                sndPlay( "recharge", SOUND_GAME );
            }
            if(stricmp(pch, "FloatNoEndurance")==0)
            {
                sndPlay( "Energy2", SOUND_GAME );
            }

            // Make taunt and confused floaters a bit more prominent
            if(stricmp(pch,"FloatTaunted")==0 || stricmp(pch,"FloatConfused")==0)
            {
                fScale *= 2.0;
                fTime *= 2.5;
            }

            addFloatingInfo(iOwnerShownOver, pch, color, color, color, fScale, fTime, fDelay * 60.0f, kFloaterStyle_Info, 0);
        }
        else if(iStyle==kFloaterStyle_Chat || iStyle==kFloaterStyle_DeathRattle)
        {
            addFloatingInfo(iOwnerShownOver, pch, 0x000000ff, 0xffffffff, 0x000000ff, 1.0f, 5*30.0f, fDelay, iStyle, 0);
        }
        else if(iStyle==kFloaterStyle_Attention)
        {
            handleAttentionFloater(&color, pch);
        }
        else if(iStyle==kFloaterStyle_PriorityAlert)
        {
            priorityAlertText_add(pch, color, NULL);
        }
    }
}

void receiveFloatingInfo( Packet *pak )
{
    int iOwnerShownOver;
    char *pch;
    int iStyle;
    float fDelay;
    U32    iColor;

    // TODO: This should probably not send strings
    //       It should send an ID instead.
    // SERVER_SEND_FLOATING_INFO
    // owner to show info over
    // string
    // seconds to delay

    iOwnerShownOver = pktGetBitsPack(pak, 1);
    pch = pktGetString(pak);
    iStyle = pktGetBitsPack(pak, 2);
    fDelay = pktGetF32(pak);
    iColor = pktGetBitsPack(pak, 32);
    handleFloatingInfo(iOwnerShownOver, pch, iStyle, fDelay, iColor);
}

void receivePlaySound( Packet *pak )
{
    char *filename;
    int channel;
    float volumeLevel;

    filename = pktGetString(pak);
    channel = pktGetBitsAuto(pak);
    volumeLevel = pktGetF32(pak);
    sndPlayEx(filename, channel, volumeLevel, SOUND_PLAY_FLAGS_INTERRUPT | SOUND_PLAY_FLAGS_SUSTAIN);
}

void receiveFadeSound( Packet *pak )
{
    int channel;
    float fadeTime;

    channel = pktGetBitsAuto(pak);
    fadeTime = pktGetF32(pak);
    sndStop(channel, fadeTime);
}

void receiveDesaturateInfo( Packet *pak )
{
    game_state.desaturateDelay = pktGetF32(pak);
    game_state.desaturateWeight = pktGetF32(pak);
    game_state.desaturateWeightTarget = pktGetF32(pak);
    game_state.desaturateFadeTime = pktGetF32(pak);
}

//
//
void receiveServerSetClientState( Packet *pak )
{
    int state = pktGetBitsPack( pak,1 );
    bool baseGurney = pktGetBitsPack(pak, 1);
    Entity* e = playerPtr();

    if(e)
    {
        if(e->pl->lastClientStatePktId > pak->id)
            return;
        else
            e->pl->lastClientStatePktId = pak->id;
    }

    setState( state, baseGurney );
}

void receiveVisitLocations(Packet *pak)
{
    if(visitLocations)
    {
        eaClearEx(&visitLocations, NULL);
        eaSetSize(&visitLocations, 0);
    }
    else
    {
        eaCreate(&visitLocations);
    }

    {
        int locationCount;
        int i;

        locationCount = pktGetBitsPack(pak, 1);

        for(i = 0; i < locationCount; i++)
        {
            char* locationName;
            locationName = pktGetString(pak);
            eaPush(&visitLocations, strdup(locationName));
        }
    }
}

void receiveLevelUp(Packet *pak)
{
    Character *p = playerPtr()->pchar;
    PowerSystem eSys = pktGetBits(pak, kPowerSystem_Numbits);
    int correctLevel = pktGetBitsPack(pak,4);

    int power_menu = MENU_LEVEL_POWER;
    int spec_menu = MENU_LEVEL_SPEC;

    if (correctLevel != character_GetLevel(p))
    {
        //EMPTY_INPUT_PACKET(CLIENTINP_CONTACT_DIALOG_RESPONSE);
        promptQuit("BadLocalCharacter");        
        return;
    }

    // First find out if they got screwed up somehow and get them
    // what they deserve.
    if(character_CanBuyPower(p))
    {
#if !TEST_CLIENT
        levelPowerMenu_ForceInit();
#endif
        character_VerifyPowerSets(p);
        start_menu(power_menu);
    }
    else if(character_CanBuyBoost(p))
    {
#if !TEST_CLIENT
        specMenu_ForceInit();
#endif
        start_menu(spec_menu);
    }
    // Otherwise, they actually are leveling up, let them make their
    // choices for the new level.
    else if(character_CanBuyPowerForLevel(p, character_GetLevel(p)+1))
    {
#if !TEST_CLIENT
        levelPowerMenu_ForceInit();
#endif
        character_VerifyPowerSets(p);
        start_menu(power_menu);
    }
    else if(character_CanBuyBoostForLevel(p, character_GetLevel(p)+1))
    {
#if !TEST_CLIENT
        specMenu_ForceInit();
#endif
        start_menu(spec_menu);
    }
    else
    {
        START_INPUT_PACKET( pak, CLIENTINP_LEVEL);
            pktSendBits(pak, kPowerSystem_Numbits, eSys);
        END_INPUT_PACKET;
        EMPTY_INPUT_PACKET(CLIENTINP_CONTACT_DIALOG_RESPONSE);
        EMPTY_INPUT_PACKET(CONTACTLINK_HELLO);
    }
}

extern bool gWaitingVerifyRespec;

void receiveLevelUpRespec(Packet *pak)
{
    Character *p = playerPtr()->pchar;
    PowerSystem eSys = pktGetBits(pak, kPowerSystem_Numbits);
    int correctLevel = pktGetBitsPack(pak,4);

    int power_menu = MENU_LEVEL_POWER;
    int spec_menu = MENU_LEVEL_SPEC;

    if (correctLevel != character_GetLevel(p))
    {
        //EMPTY_INPUT_PACKET(CLIENTINP_CONTACT_DIALOG_RESPONSE);
        promptQuit("BadLocalCharacter");        
        return;
    }

     gWaitingVerifyRespec = false;
    respec_allowLevelUp( true );

    start_menu( MENU_RESPEC );
}


void receiveNewTitle(Packet *pak)
{
    bool bOrigin = pktGetBits(pak, 1);
    bool bVeteran = pktGetBits(pak, 1);

    titleselectOpen(bOrigin,bVeteran);
}

void receiveVisitMapCells(Packet *pak)
{
    int staticMapId = pktGetBitsPack(pak, 1);
    if (staticMapId)
    {
        //    load from file
        VisitedStaticMap *vsm = automap_getMapStruct(playerPtr()->db_id);
        VisitedMap *targetMap = NULL;
        int opaque_fog = pktGetBits(pak, 1);
        int clearFog = pktGetBits(pak, 1);
        int i;
        if (vsm)        //    test clients don't have a vsm
        {
            for (i = 0; i < eaSize(&vsm->staticMaps); ++i)
            {
                VisitedMap *vm = vsm->staticMaps[i];
                if (vm->map_id == staticMapId)
                {
                    targetMap = vm;
                    break;
                }
            }
            if (i >= eaSize(&vsm->staticMaps))
            {
                int cell_array_size = (MAX_MAPVISIT_CELLS+31)/32;
                U32 cell_array[4*(MAX_MAPVISIT_CELLS+31)/32];
                int j;
                for (j = 0; j < cell_array_size; j++)
                    cell_array[j] = 0;
                automap_addStaticMapCell(vsm, playerPtr()->db_id, staticMapId, opaque_fog, cell_array_size, cell_array);
                targetMap = eaLast(&vsm->staticMaps);
            }
            if (clearFog)
            {
                int j;
                for (j = 0; j < MAX_MAPVISIT_CELLS; j++)
                    SETB(targetMap->cells, j);
            }
            mapfogSetVisitedCellsFromServer(targetMap->cells, MAX_MAPVISIT_CELLS, targetMap->opaque_fog);
            automap_updateStaticMap(targetMap);
        }
    }
    else
    {
        int num_cells = pktGetBitsPack(pak,1);
        int cell_array_size = (num_cells+31)/32;
        U32 *cell_array = calloc(cell_array_size*4, 1);
        if (num_cells)
        {
            int i;
            for (i = 0; i < cell_array_size; i++)
                cell_array[i] = pktGetBits(pak, 32);
        }
        mapfogSetVisitedCellsFromServer(cell_array,num_cells,1);
        free(cell_array);
    }
}

void receiveStaticMapCells(Packet *pak)
{
    int        i;
    int db_id = pktGetBitsAuto(pak);
    int numMaps = pktGetBitsAuto(pak);
    VisitedStaticMap *vsm = automap_getMapStruct(db_id);
    automap_clearStaticMaps(vsm);
    for (i= 0; i < numMaps; ++i)
    {
        int map_id = pktGetBitsAuto(pak);
        if (map_id)
        {
            int opaque_fog = pktGetBits(pak, 1);
            int cell_array_size = (MAX_MAPVISIT_CELLS+31)/32;
            U32 cell_array[4*(MAX_MAPVISIT_CELLS+31)/32];
            int j;
            for (j = 0; j < cell_array_size; j++)
                cell_array[j] = pktGetBits(pak, 32);
            automap_addStaticMapCell(vsm, db_id, map_id, opaque_fog, cell_array_size, cell_array);
        }
    }
}
void receiveResendWaypointRequest(Packet* pak)
{
    // Nothing to receive, just update waypoint request.
    
    compass_UpdateWaypoints(0);
}

void receiveWaypointUpdate(Packet* pak)
{
    int uid;
    Vec3 pos;

    uid = pktGetBitsPack(pak, 1);
    pos[0] = pktGetF32(pak);
    pos[1] = pktGetF32(pak);
    pos[2] = pktGetF32(pak);

    compass_SetDestinationWaypoint(uid, pos);
}


// recieve a /bug report from server and write to local log file (for QA purposes)
void receiveBugReport(Packet * pak)
{
    char * report    = strdup(pktGetString(pak));
    log_printf("bug_report", "%s", report);
    free(report);
}

void receiveSelectBuild(Packet * pak)
{
    int buildNum;
    int level;
    Entity *e;

    buildNum = pktGetBits(pak, 4);
    level = pktGetBits(pak, 6);
    e = playerPtr();

    if (e)
    {
        assert (buildNum >= 0 && buildNum < MAX_BUILD_NUM);
        character_SelectBuildClient(e, buildNum);
        e->pchar->iLevel = level;
        // What do we do if the enhancements "manage" screen is up?
        // Ignore the possibility.  Needs either a chat window or a trainer interaction to get here, both of which preclude the enhancement manage screen
    }
}

void receiveShardVisitorData(Packet *pak)
{
    char *shardVisitorData;
    int dbid;
    int uid;
    int cookie;
    U32 addr;
    U32 port;

    shardVisitorData = pktGetString(pak);
    dbid = pktGetBitsAuto(pak);
    uid = pktGetBitsAuto(pak);
    cookie = pktGetBitsAuto(pak);
    addr = pktGetBitsAuto(pak);
    port = pktGetBitsAuto(pak);

    if (shardVisitorData[0] != 0 && dbid && cookie)
    {
        strncpyt(g_shardVisitorData, shardVisitorData, SHARD_VISITOR_DATA_SIZE);
        g_shardVisitorDBID = dbid;
        g_shardVisitorUID = uid;
        g_shardVisitorCookie = cookie;
        g_shardVisitorAddress = addr;
        g_shardVisitorPort = (U16) port;
    }
}

void receiveShardVisitorJump(Packet *pak)
{
    if (g_shardVisitorData[0] != 0 && g_shardVisitorDBID && g_shardVisitorCookie)
    {
        quitToLogin(NULL);
    }
}

void receiveShowContactFinderWindow(Packet *pak)
{
    int imageNumber;
    char *displayName = NULL;
    char *profession = NULL;
    char *locationName = NULL;
    char *description = NULL;
    int showPrev;
    int showTele;
    int showNext;

    imageNumber = pktGetBitsAuto(pak);

    estrPrintCharString(&displayName, pktGetString(pak));
    estrPrintCharString(&profession, pktGetString(pak));
    estrPrintCharString(&locationName, pktGetString(pak));
    estrPrintCharString(&description, pktGetString(pak));

    showPrev = pktGetBitsAuto(pak);
    showTele = pktGetBitsAuto(pak);
    showNext = pktGetBitsAuto(pak);

#ifndef TEST_CLIENT
    contactFinderSetData(imageNumber, displayName, profession, locationName, description, 
                            showPrev, showTele, showNext);
#endif
}

void receiveWebStoreOpenProduct(Packet *pak)
{
    char *productCode = pktGetString(pak);

#ifndef TEST_CLIENT
    webStoreOpenProduct(productCode);
#endif
}