//
//
// initCLient.c - client start up functions
//----------------------------------------------------------------------------------------------------------------

#include "gameComm/initCommon.h"
#include "gameComm/initClient.h"
#include <utilitieslib/network/netio.h>
#include "varutils.h"
#include "fxinfo.h"
#include "clientcomm/clientcomm.h"
#include "comm_game.h"
#include "entity/entVarUpdate.h"
#include "UI/uiDialog.h"
#include "entity/entclient.h"
#include "player/player.h"
#include "UI/uiCostume.h"
#include <utilitieslib/utils/StringUtil.h>
#include <utilitieslib/components/HashFunctions.h>
#include "win/input.h"
#include "graphics/gfx.h"
#include "graphics/gfxLoadScreens.h"
#include "UI/uiGame.h"
#include "UI/uiEditText.h"
#include "UI/uiTarget.h"
#include "cmdparse/cmdgame.h"
#include "entity/costume_client.h"
#include <utilitieslib/assert/assert.h>
#include "UI/Hybrid/uiRegister.h"
#include "entity/entrecv.h"
#include "UI/uiCursor.h"
#include "UI/uiLogin.h"
#include "clientcomm/dbclient.h"
#include "sound/sound.h"
#include <utilitieslib/utils/error.h>
#include "testClient/TestClient.h"
#include "gameComm/dooranimclient.h"
#include "demo.h"
#include "UI/sprite/sprite_text.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiWindows_init.h"
#include "auth/authUserData.h"
#include <utilitieslib/utils/file.h>
#include "entity/costume.h"
#include "graphics/zOcclusion.h"
#include "UI/uiCompass.h"
#include "edit/DebugLocation.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "storyarc/missionClient.h"
#include "storyarc/zowieClient.h"
#include "UI/uiPopHelp.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "comm_backend.h"
#if defined TEST_CLIENT
#include "player/inventory_client.h" //for unlocking slot when creating new random character
#endif

//-----------------------------------------------------------------------------------------
//
//
//

//
//
//
static int waiting_for_full_update = 0;
static int enter_game_now = 0;
static int force_fade = 0;

int startFadeInScreen(int set)
{
    int force = force_fade;
    force_fade = 0;
    if (set) {
        force = force_fade = 1;
    }
    return force;
}

void clearWaitingForFullUpdate(void)
{
    waiting_for_full_update = 0;
}

int okayToChangeGameMode(void)
{
    // We don't want the DoorAnim stuff changing the game mode while we're in the middle of the load screen
    //  unless it's to cancel out of a map transfer before we started loading the new one.
    return !waiting_for_full_update && !enter_game_now;
}


// This is called when we've received the world geometry, and are ready to 
//   begin receiving entities
// Only called in commReqScene
void setWaitingForFullUpdate(void)
{
    waiting_for_full_update = 1;
    //start_menu("");

    //showBgAdd(2);
    //printf("setWaitingForFullUpdate()\n");
}

// Tell the server we have received our character (and the first full entity update)
// This gets called *during* the entity update, and what it sets gets checked out in the main() loop
void notifyReceivedCharacter()
{
    if (!waiting_for_full_update) return;
    
    //printf("sending CLIENTINP_RECEIVED_CHARACTER\n");
    EMPTY_INPUT_PACKET( CLIENTINP_RECEIVED_CHARACTER );

    enter_game_now = 1;
    waiting_for_full_update = 0;

}

void checkDoneResumingMapServerEntity()
{
    extern int glob_have_camera_pos;
    if (!enter_game_now)
        return;
    enter_game_now = 0;

    clearWaitingForFullUpdate();

    //printf("setDoneResumeingMapServerEntity()\n");
    assert(playerPtr());

    //reapplyPlayerFxAfterMapXfer();
    //game_state.game_mode = SHOW_GAME;

    glob_have_camera_pos = MAP_LOADED_FINISHING_LOADING_BAR;
    force_fade = 1;
    gLoggingIn = false;
    showBgAdd(-2); // Matches up with the one in commReqScene()
    writeConsole(OUTPUT_INFO, "Established successful MapServer connection");
}

//
// Gets called on way into 3d game.  This way new account is created here & not at start of menus.
//
int    player_being_created;

void newCharacter()
{
    player_being_created = TRUE;
}

//
//
void doCreatePlayer(int connectionless)
{
    Entity *e = playerPtr();

    if( !e )
    {
        e = entCreate("");
        playerSetEnt(e);
        //entSetSeq(e, seqLoadInst( "male", 0, SEQ_LOAD_FULL, e->randSeed, e  ));
        SET_ENTTYPE(e) = ENTTYPE_PLAYER;
        playerVarAlloc( e, ENTTYPE_PLAYER );    //must be after playerSetEnt
        // JW: Added so powers system can test auth data during character creation
    }

    memcpy(e->pl->auth_user_data,db_info.auth_user_data,sizeof(e->pl->auth_user_data));

    switch( game_state.skin )
    {
        case UISKIN_HEROES:
        default:
            e->pl->playerType = kPlayerType_Hero;
            e->pl->praetorianProgress = kPraetorianProgress_PrimalBorn;
            break;
        case UISKIN_VILLAINS:
            e->pl->playerType = kPlayerType_Villain;
            e->pl->praetorianProgress = kPraetorianProgress_PrimalBorn;
            break;
        case UISKIN_PRAETORIANS:
            e->pl->playerType = kPlayerType_Hero;
            e->pl->praetorianProgress = kPraetorianProgress_Tutorial;
            break;
    }

    // don't keep old settings
    costumereward_clear( 0 );
    costumeAwardAuthParts(e);

    setGender( e, "male" );
    if (!connectionless)
        newCharacter();
    initWindows();
}

//
//
//
//
// Are they creating a new character?  Called when entering 3d game from character create.
//   TRUE for new account, new character & simple new character.
//
int    player_slot;

int choosePlayerWrapper(int player_slot, int createLocation)
{
    U32        ip=0;
    UISkin    new_skin;

    if(db_info.players[player_slot].invalid)
    {
        return 0;
    }

    // Praetorian UI skin only works in Praetoria proper because it makes
    // the currency be "Information" and so on.
    if( db_info.players[player_slot].praetorian == kPraetorianProgress_Tutorial ||
        db_info.players[player_slot].praetorian == kPraetorianProgress_Praetoria )
        new_skin = UISKIN_PRAETORIANS;
    else if( db_info.players[player_slot].type )
        new_skin = UISKIN_VILLAINS;
    else if( !db_info.players[player_slot].type )
        new_skin = UISKIN_HEROES;

    if( new_skin != game_state.skin )
    {
        game_state.skin = new_skin;
        resetVillainHeroDependantStuff(0);
    }

#ifndef TEST_CLIENT
    // Resetting pop help state.
    // Is there a better spot for this?
    initializePopHelp();
#endif

    if (game_state.local_map_server)
        ip = ipFromString(game_state.address);

#if defined TEST_CLIENT
    //Test clients that are creating new characters will fail if it takes too long for mapserver to finish loading before timeout ends.
    //Therefore, timeout for test clients is set MUCH longer (240 seconds) than the timeout period for regular clients
    return dbChoosePlayer( player_slot, ip, createLocation, control_state.notimeout?NO_TIMEOUT:240);
#else
    return dbChoosePlayer( player_slot, ip, createLocation, control_state.notimeout?NO_TIMEOUT:90);
#endif
}

int chooseVisitingPlayerWrapper(int dbid)
{
#ifndef TEST_CLIENT
    // Resetting pop help state.
    // Is there a better spot for this?
    // DGNOTE 8/9/2010 - Do I even need to do this?
    initializePopHelp();
#endif

    if (game_state.local_map_server)
    {
        // Shard visitor tech simply doesn't work with local mapserver.  However, I simply assert 0, so I don't embed any incriminating strings in the client.
        assert(0);
    }
    return dbChooseVisitingPlayer(dbid, control_state.notimeout ? NO_TIMEOUT : 90);
}

static int gCreateLocation = 1;

void pickHeroTutorial(void)
{
	playerPtr()->pl->praetorianProgress = kPraetorianProgress_PrimalBorn;
	playerPtr()->pl->playerType = kPlayerType_Hero;
	gCreateLocation = DBGAMECLIENT_CREATE_PLAYER_PRIMAL_HERO_TUTORIAL;
}

void pickVillainTutorial(void)
{
	playerPtr()->pl->praetorianProgress = kPraetorianProgress_PrimalBorn;
	playerPtr()->pl->playerType = kPlayerType_Villain;
	gCreateLocation = DBGAMECLIENT_CREATE_PLAYER_PRIMAL_VILLAIN_TUTORIAL;
}

void pickPrimalTutorial(void)
{
    playerPtr()->pl->praetorianProgress = kPraetorianProgress_NeutralInPrimalTutorial;
    playerPtr()->pl->playerType = kPlayerType_Hero;
    gCreateLocation = DBGAMECLIENT_CREATE_PLAYER_PRIMAL_TUTORIAL;
}

void pickHero(void)
{
    playerPtr()->pl->praetorianProgress = kPraetorianProgress_PrimalBorn;
    playerPtr()->pl->playerType = kPlayerType_Hero;
    gCreateLocation = DBGAMECLIENT_CREATE_PLAYER_PRIMAL_HERO;
}

void pickVillain(void)
{
    playerPtr()->pl->praetorianProgress = kPraetorianProgress_PrimalBorn;
    playerPtr()->pl->playerType = kPlayerType_Villain;
    gCreateLocation = DBGAMECLIENT_CREATE_PLAYER_PRIMAL_VILLAIN;
}

void pickPraetorianTutorial(void)
{
    // All players start out as Loyalists in the tutorial. They get to pick
    // their alignment in the door mission's moral choice.
    playerPtr()->pl->praetorianProgress = kPraetorianProgress_Tutorial;
    playerPtr()->pl->playerType = kPlayerType_Villain;
    gCreateLocation = DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_TUTORIAL;
}

void pickLoyalist(void)
{
    playerPtr()->pl->praetorianProgress = kPraetorianProgress_Praetoria;
    playerPtr()->pl->playerType = kPlayerType_Villain;
    gCreateLocation = DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_LOYALIST;
}

void pickResistance(void)
{
    playerPtr()->pl->praetorianProgress = kPraetorianProgress_Praetoria;
    playerPtr()->pl->playerType = kPlayerType_Hero;
    gCreateLocation = DBGAMECLIENT_CREATE_PLAYER_PRAETORIAN_RESISTANCE;
}

static int needToSendCharacter = 0;
void sendCharacterCreate(Packet *pak)
{
    pktSendBits(pak, 1, needToSendCharacter);
    if (needToSendCharacter) {

        sendCharacterToServer( pak, playerPtr() );    //This will update prefs, tray states, etc.
        needToSendCharacter = 0;
    }
}

int checkForCharacterCreate()
{
    if( player_being_created )
    {
        assert(playerPtr());
        assert(db_info.players);

        // See if the map can be started
        if (!dbCheckCanStartStaticMap( gCreateLocation, control_state.notimeout?NO_TIMEOUT:90))
        {
            gWaitingToEnterGame = FALSE;
            toggle_3d_game_modes(SHOW_NONE);
            dialogStd( DIALOG_OK, "MapFailedOverloadProtection", NULL, NULL, NULL, NULL, 0 );
            return FALSE;
        }

        loadScreenResetBytesLoaded();

        //This is a new player so they weren't logged in until this point because we didn't know the player
        //name until now.
        strcpy( db_info.players[gPlayerNumber].name, playerPtr()->name );
        db_info.players[gPlayerNumber].type = playerPtr()->pl->playerType;
        db_info.players[gPlayerNumber].subtype = playerPtr()->pl->playerSubType;
        db_info.players[gPlayerNumber].praetorian = playerPtr()->pl->praetorianProgress;

#if defined TEST_CLIENT

        if (inventoryClient_GetAcctAuthoritativeState() == ACCOUNT_SERVER_UP)
        {
            //Redeem a slot (if allowed) if there are no unlocked slots
            if (getTotalServerSlots() - getSlotUnlockedPlayerCount() < 1)
            {
                if (CanAllocateCharSlots())
                {
                    printf("Process to unlock slot initiated.  Please wait...\n");
                    dbRedeemSlot();
                    if (waitForRedeemSlot(60))
                    {
                        printf("Unlocking slot successful.\n");
                    }
                    else
                    {
                        printf("Waiting for slot to unlock took too long.\nCanceling create character process.\n");
                        gWaitingToEnterGame = FALSE;
                        return FALSE;
                    }

                }
                else
                {
                    printf("Unable to unlock slot.\nCheck if account created by test client is allowed to unlock a slot.\n");
                    printf("Canceling create character process.\n");
                    gWaitingToEnterGame = FALSE;
                    return FALSE;
                }
            }
        }
        else
        {
            printf("Account server is not running.\nCanceling create character process.\n");
            gWaitingToEnterGame = FALSE;
            return FALSE;
        }

        printf("Creating character...\n");
#endif
        if( !choosePlayerWrapper(gPlayerNumber, gCreateLocation) )    //Log in the new name.
        {
            char    *db_err = dbGetError();
            if (db_info.players) { // If we haven't disconnected
                 strcpy(db_info.players[gPlayerNumber].name, "EMPTY");
            }

            if (stricmp(db_err,textStd("MapFailedOverloadProtection")) == 0)
            {
                gWaitingToEnterGame = FALSE;
                toggle_3d_game_modes(SHOW_NONE);
                dialogStd( DIALOG_OK, db_err, NULL, NULL, NULL, NULL, 0 );
                return FALSE;
            }

            gWaitingToEnterGame = FALSE;
            gLoggingIn = FALSE;
            if (strnicmp(db_err,"DuplicateName",strlen("DuplicateName"))==0 || strnicmp(db_err,"InvalidName",strlen("InvalidName"))==0 ||
                stricmp(db_err,textStd("DuplicateName"))==0 || stricmp(db_err,textStd("InvalidName"))==0)
            {
                toggle_3d_game_modes(SHOW_NONE);
                dialogStd( DIALOG_OK, "DuplicateName", NULL, NULL, NULL, NULL, 0 );
            }
            else
            {
                dialogStd( DIALOG_OK, db_err, NULL, NULL, 0, NULL, 0 );
                restartLoginScreen();
            }
            return FALSE;
        }

        needToSendCharacter = 1;
        tryConnect();
        needToSendCharacter = 0;
        if( !commConnected() )
        {
            if (db_info.players)
                strcpy(db_info.players[gPlayerNumber].name, "EMPTY");
            dialogStd( DIALOG_OK, "NoMapserverConn", NULL, NULL, NULL, NULL, 0 );
            restartLoginScreen();
            gWaitingToEnterGame = FALSE;
            return FALSE;
        }

        dbDisconnect();        //Now connected to mapServer, don't need connServer anymore.

        gfxReload(1);                        //Blows away local player entity as part of reinitting gfx tree & loading a map.
        gfxPrepareforLoadingScreen();
        
        if(commReqScene(0))                    //Get the scene
        {
            player_being_created = FALSE;
        }

        return FALSE; // Because of stuff in commReqScene and queueResume, we will get the mode set appropriately later
    }
    else
    {
        costume_Apply( playerPtr() );
        return TRUE;
    }
}

void resetStuffOnMapMove()
{
    // Also called on quitToLogin, any globals that need to get cleared should be put here.

    extern int teamOfferingDbId;
    extern void ScriptUIShutDown();

    ScriptUIShutDown();
    DebugLocationShutdown();
    MissionClearAll();

    game_state.pretendToBeOnArenaMap = 0;
     entReceiveInit();
    dialogClearQueue( 0 );
    dialogClearQueue( 1 );
    teamOfferingDbId = 0;
    zoClearOccluders();
}

int doMapXfer()
{
    extern U32 local_time_bias;

    demoStop();
    zowieReset();
    testClientRandomDisconnect(TCS_doMapXfer_01);
    resetStuffOnMapMove();
    commNewInputPak();
    if(!tryConnect())
    {
        dialogStd( DIALOG_OK, "NoMapserverConn", NULL, NULL, NULL, NULL, 0 );
        restartLoginScreen();
        return FALSE;
    }
    testClientRandomDisconnect(TCS_doMapXfer_02);
    cmdOldSetSends(control_cmds,1);
    testClientRandomDisconnect(TCS_doMapXfer_03);
    gfxReload(1);                //creates an entity.
    if (commConnected())
        if(!commReqScene(0))        //Asks server to send a world to load
            return FALSE;
    testClientRandomDisconnect(TCS_doMapXfer_04);
    doneWaitForDoorMode();        //tell LF wait dialog to go away
    MissionKickClear();
    MissionObjectiveStopTimer();
    local_time_bias = 0; // Interpolation code seems to break without this
    if( !commConnected() )
    {
        dialogStd( DIALOG_OK, "NoMapserverConn", NULL, NULL, NULL, NULL, 0 );
        restartLoginScreen();
        return FALSE;
    }
    testClientRandomDisconnect(TCS_doMapXfer_05);
    return TRUE;
}

