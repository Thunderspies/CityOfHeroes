//
//
// live_menu.c -- master UI loop
//-------------------------------------------------------------------------------------------------
#include "sound/sound.h"
#include "UI/sprite/sprite_text.h"    // for font stuff
#include "UI/sprite/sprite_font.h"
#include "UI/uiUtil.h"            // for colors
#include "player/player.h"            // for playerPtr
#include "player/playerState.h"    // for runState
#include "win/win_init.h"        // for windowSize
#include "clientcomm/clientcomm.h"        // for lastRecvTime
#include "UI/uiEditText.h"        // for editting_on
#include "cmdparse/cmdgame.h"        // for game_state
#include "gameComm/initClient.h"        // for checkForCharacterCreate
#include "graphics/font.h"
#include "player/pmotion.h"
#include "uiPowers.h"
#include "UI/uiGender.h"
#include "UI/uiNet.h"
#include "UI/uiCostume.h"
#include "uiTradeLogic.h"
#include "entity/entPlayer.h"
#include "UI/uiWindows.h"
#include "UI/uiWindows_init.h"
#include "UI/uiLogin.h"
#include "comm_game.h"
#include "UI/uiKeymapping.h"
#include "UI/uiLevelPower.h"
#include "UI/uiLevelSpec.h"
#include "uiCombineSpec.h"
#include "UI/uiSuperRegistration.h"
#include "UI/uiSupercostume.h"
#include "entity/costume.h"
#include "UI/uiTailor.h"
#include "UI/uiReSpec.h"
#include "UI/uiOptions.h"
#include "uiLevelSkill.h"
#include "uiLevelSkillSpec.h"
#include "UI/uiFx.h"
#include "uiLoadCostume.h"
#include "uiSaveCostume.h"
#include "uiPCCPowers.h"
#include "uiPCCProfile.h"
#include "uiPCCRank.h"
#include "uiNPCCostume.h"
#include "uiNPCProfile.h"
#include <utilitieslib/utils/timing.h>            // for timerSecondsSince2000
#include <utilitieslib/language/AppLocale.h>
#include "UI/uiBaseInput.h"
#include "bases/bases.h"
#include "formatter/smf_main.h"
#include "entity/teamCommon.h"
#include "storyarc/playerCreatedStoryarcClient.h"
#include "UI/uiPowerCust.h"

#include "UI/uiCursor.h"
#include "UI/uiDialog.h"
#include "UI/uiToolTip.h"
#include "UI/uiInput.h"
#include "UI/uiContextMenu.h"
#include "UI/uiScrollBar.h"
#include "UI/uiGame.h"
#include "UI/uiAutomapFog.h"
#include "UI/uiQuit.h"
#include "entity/character_animfx_client.h"
#include "entity/character_base.h"
#include "entity/powers.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdcontrols.h"

#include "UI/sprite/sprite_base.h"
#include "gameComm/dooranimclient.h"
#include "graphics/truetype/ttFontDraw.h"
#include "graphics/seqgraphics.h"
#include "win/input.h"
#include "entity/entity.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiFocus.h"
#include "uiLoadPowerCust.h"
#include "auth/authUserData.h"
#include "UI/uiUtilMenu.h"
#include <utilitieslib/utils/MemoryMonitor.h>

#include "UI/Hybrid/uiOrigin.h"
#include "UI/Hybrid/uiPlaystyle.h"
#include "UI/Hybrid/uiArchetype.h"
#include "UI/Hybrid/uiPower.h"
#include "UI/Hybrid/uiBody.h"
#include "UI/Hybrid/uiRegister.h"
#include "UI/Hybrid/uiRedirect.h"

#include "UI/Hybrid/uiWebStoreFrame.h"
#include "UI/uiEmail.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "graphics/gfxLoadScreens.h"
#include "uiNewFeatures.h"


static int gCurrentMenu = 0;
static int gNextMenu = -1;
static int gPrevMenu = -1;        // NOT RELIABLE, for development

static U32 s_LastInteractTime = 0;
static bool s_bMadeAFKDialog = false;

// Menu Management
//------------------------------------------------------------------------------------------------------------------
//
// This is where they enter the 3d game.  If the player is brand new then
// a call to the account server is necc. to create the player account.
//
static int begin_game( int send_character )
{
    if( !checkForCharacterCreate() ) //1 = resuming a character, 0 = new character or failure
        return FALSE;

    resetCharacterCreationFlag();  //clear the current creation information
    toggle_3d_game_modes( SHOW_GAME );
    gCurrentMenu = MENU_GAME;
    newFeatures_queryWebData();
    memtrack_mark(0, getMenuName(gCurrentMenu));
    return TRUE;
}

//
//
int shell_menu()
{
    if (!game_state.viewCutScene)
    {
        return gCurrentMenu != MENU_GAME || doingHeadShot();
    }
    else
    {
        // the shell menu should not be visible if a cutscene is playing
        return false;
    }
}

//
//
int current_menu()
{
    return gCurrentMenu;
}

int previous_menu()    // NOT RELIABLE, for development
{
    return gPrevMenu;
}

//
//
int isMenu( int menu )
{
    if ( gCurrentMenu == menu )
        return TRUE;

    return FALSE;
}

//
//
const char* getMenuName(int menu)
{
    static int init;
    
    if(!init)
    {
        int i;
        init = 1;
        for(i = 0; i < MENU_TOTAL; i++)
        {
            getMenuName(i);
        }
    }
    
    if(INRANGE0(menu, MENU_TOTAL))
    {
        switch(menu)
        {
            #define CASE(x) case x:return #x
            CASE(MENU_LOGIN);
            CASE(MENU_ORIGIN);
            CASE(MENU_PLAYSTYLE);
            CASE(MENU_ARCHETYPE);
            CASE(MENU_POWER);
            CASE(MENU_BODY);
            CASE(MENU_REGISTER);
            CASE(MENU_GENDER);
            CASE(MENU_COSTUME);
            CASE(MENU_LEVEL_POWER);
            CASE(MENU_LEVEL_POOL_EPIC);
            CASE(MENU_LEVEL_SPEC);
            CASE(MENU_COMBINE_SPEC);
            CASE(MENU_SUPER_REG);
            CASE(MENU_SUPERCOSTUME);
            CASE(MENU_TAILOR);
            CASE(MENU_RESPEC);
            CASE(MENU_LOADCOSTUME);
            CASE(MENU_PCC_POWERS);
            CASE(MENU_PCC_PROFILE);
            CASE(MENU_NPC_COSTUME);
            CASE(MENU_NPC_PROFILE);
            CASE(MENU_PCC_RANK);
            CASE(MENU_SAVECOSTUME);
            CASE(MENU_POWER_CUST);
            CASE(MENU_POWER_CUST_POWERPOOL);
            CASE(MENU_POWER_CUST_INCARNATE);
            CASE(MENU_LOAD_POWER_CUST);
            CASE(MENU_REDIRECT);
            CASE(MENU_GAME);
            #undef CASE
            xdefault:
                Errorf("Undefined menu name: %d\n", menu);
        }
    }
    
    return "unknownMenu";
}

int isLetterBoxMenu()
{
    if( loadingScreenVisible() ||
        (isMenu(MENU_LOGIN) && !(s_loggedIn_serverSelected == LOGIN_STAGE_IN_QUEUE || s_loggedIn_serverSelected == LOGIN_STAGE_CHARACTER_SELECT )) ||
        (isMenu(MENU_REGISTER) && (gLoggingIn || gWaitingToEnterGame)))
    {
        return true;
    }

    return false;
}

int isPowerCustomizationMenu(int menu)
{
    return    menu == MENU_POWER_CUST ||
            menu == MENU_POWER_CUST_POWERPOOL || 
            menu == MENU_POWER_CUST_INCARNATE;
}

int isCurrentMenu_PowerCustomization()
{
    return isPowerCustomizationMenu(current_menu());
}

//
//
void start_menu( int menu )
{
    if( control_state.server_state->shell_disabled )
        return;

    if(    !isMenu( MENU_GAME ) )
    {
        if ( game_state.pendingTSMapXfer )
            return;

        if( menu == MENU_GAME )
        {
            if( !begin_game( TRUE ))
            {
                //gCurrentMenu = menu;
                return;
            }

            if( playerPtr()->db_id <=0 )
                dialogClearQueue( 0 );
        }
    }
    else
    {
          if( menu != MENU_GAME )
        {
             if( entMode( playerPtr(), ENT_DEAD ) )
                return;

            if ( game_state.pendingTSMapXfer )
                return;

            if( window_getMode( WDW_STORE ) == WINDOW_DISPLAYING )
                window_setMode( WDW_STORE, WINDOW_DOCKED );

            if( window_getMode( WDW_TRADE ) == WINDOW_DISPLAYING )
            {
                sendTradeCancel( TRADE_FAIL_I_LEFT );
                window_setMode( WDW_TRADE, WINDOW_DOCKED );
            }
        }
    }

    gNumEnhancesLeft = 0;
    enhancementAnim_clear();
    fadingText_clearAll();
    electric_clearAll();
    uiReturnAllFocus();

    gNextMenu = menu;
}

void returnToGame(void)
{
     start_menu(MENU_GAME);
     dialogClearQueue(0);
    dialogClearQueue(1);
    closeUnimportantWindows();
    uiGetFocus(NULL, FALSE);
    baseEditToggle( kBaseEdit_None );
}

void uigameDemoForceMenu() // BER - if would be safe to say I don't know what I'm doing here
{
    gCurrentMenu = MENU_GAME;
    memtrack_mark(0, getMenuName(gCurrentMenu));
}

void forceLoginMenu()
{
    gNextMenu = MENU_LOGIN;
    gCurrentMenu = MENU_LOGIN;
    memtrack_mark(0, getMenuName(gCurrentMenu));
}

// These functions serve as the primary menu loop
//-------------------------------------------------------------------------------------------------------------

int    collisions_off_for_rest_of_frame; //switch to turn off all menu collisions.

// array of function pointers, each function corresponds to a menu
void (* menuFuncs[] )() =    {
                                loginMenu,
                                originMenu,
                                playstyleMenu,
                                archetypeMenu,
                                powerMenu,
                                bodyMenu,
                                registerMenu,
                                genderMenu,
                                costumeMenu,
                                levelPowerMenu,
                                levelPowerPoolEpicMenu,
                                specMenu,
                                combineSpecMenu,
                                superRegMenu,
                                supercostumeMenu,
                                tailorMenu,
                                respecMenu,
                                loadCostume_menu,
                                PCCPowersMenu,
                                PCCProfileMenu,
                                NPCCostumeMenu,
                                NPCProfileMenu,
                                PCCRankMenu,
                                saveCostume_menu,
                                powerCustomizationMenu,
                                powerCustomizationMenu,
                                powerCustomizationMenu,
                                loadPowerCust_menu,
                                redirectMenu,
                                serveWindows,
                            };


//Set the known mode bits on the player so he can predict movement animations correctly
// MMSEQ made the character_SetAnimClientStanceBits work with animation delay
void setPlayerStanceBits()
{
    Entity *player = playerPtr();
    if(player && player->pchar && character_IsInitialized(player->pchar) &&
        player->pchar->ppowStance)
    {
        character_SetAnimClientStanceBits(player->pchar, 
            power_GetFX(player->pchar->ppowStance)->piModeBits);
    }
}


void DoNotLogout(void * data)
{
    U32 ulNow = timerSecondsSince2000();
    s_LastInteractTime = ulNow;
    s_bMadeAFKDialog = false;

    if(commCheckConnection() == CS_CONNECTED)
    {
        commSendQuitGame(1);
        cmdParse("emote thumbsup");
    }
}

void AFKTick(void)
{

#define AUTO_AFK_COMMAND (5*60)
#define AUTO_AFK_WARN    (18*60)
#define AUTO_AFK_LOGOUT  (20*60)
    Entity *e = playerPtr();
    static bool afk = false;

    if(e && e->pl && e->db_id && !game_state.cryptic && !game_state.mission_map && !e->pl->taskforce_mode && !e->access_level )
    {
        U32 ulNow = timerSecondsSince2000();

        if(inpEdgeThisFrame() || s_LastInteractTime==0)
        {
            s_LastInteractTime = ulNow;
            afk = false;
        }

        // The commands here bypass cmdParse since doing a cmdParse resets
        //    the timer.
        if( afk )
        {
            if( getCurrentLocale() != LOCALE_ID_KOREAN )
            {
                if(s_LastInteractTime+AUTO_AFK_LOGOUT < ulNow)
                {
                    // do a /quit
                    e->logout_login = 1;
                    commSendQuitGame(0);
                    sndPlay("Klaxon1", SOUND_GAME);
                    s_LastInteractTime = ulNow;
                }
                else if(!s_bMadeAFKDialog && s_LastInteractTime+AUTO_AFK_WARN < ulNow)
                {
                    dialogStd(DIALOG_OK, "AFKTooLongWarning", "OkString", NULL, DoNotLogout, NULL, 0);
                    s_bMadeAFKDialog = true;
                    sndPlay("M3", SOUND_GAME);
                }
            }
        }
        else
        {
            if(s_bMadeAFKDialog)
            {
                dialogRemove( NULL, DoNotLogout, NULL );
                s_bMadeAFKDialog = false;
            }

            if(s_LastInteractTime+AUTO_AFK_COMMAND < ulNow)
            {
                // send a /afk
                cmdParse("afk");
                afk = true;
            }
        }
    }
    else
    {
        s_LastInteractTime = 0;
        afk = false;
        if(s_bMadeAFKDialog)
        {
            dialogRemove( NULL, DoNotLogout, NULL );
            s_bMadeAFKDialog = false;
        }
    }
}

void TimersTick() 
{
    Entity *e = playerPtr();
    static int iLastSecond = -1;
    char strCount[10];
    unsigned long ulCurTime = timerCpuTicks();
    int iCurSecond = timerSeconds(ulCurTime);

    // Only show this timer on a pvp map
    if (server_visible_state.isPvP 
        && e && e->pvpClientZoneTimer && e->pvpUpdateTime)
    {
        if (iLastSecond < iCurSecond)
        {
            int iTimeToPvP = 1 + e->pvpClientZoneTimer - timerSeconds(ulCurTime - e->pvpUpdateTime);

            iLastSecond = iCurSecond;

            if (iTimeToPvP == 0) e->pvpClientZoneTimer = 0;
            
            if (iTimeToPvP <= 5 || (iTimeToPvP % 5) == 0) {
                int winwd, winht;
                sprintf(strCount, "%d", iTimeToPvP);
                windowClientSizeThisFrame( &winwd, &winht );
                fadingTextEx( winwd/2, winht/2, 5000, 6.0, CLR_RED, CLR_RED, CENTER_X|CENTER_Y, &title_18, 30, 0, strCount, .1f );
                sndPlay("BeepWarning", SOUND_GAME);
            }
        }
    }
}

void serve_menus()
{
    Entity *e = playerPtr();

    PERFINFO_AUTO_START("top", 1);

    smf_HandlePerFrame();

     if( gNextMenu >= 0 )
    {
        gPrevMenu = gCurrentMenu;    // not reliable, for development purposes
         gCurrentMenu = gNextMenu;
        memtrack_mark(0, getMenuName(gCurrentMenu));
        collisions_off_for_rest_of_frame = 1;
        gLastDownX = gLastDownX = 0;
        gNextMenu = -1;
    }

    ttSetCacheableFontScaling(winDefs[WDW_STAT_BARS].loc.sc, winDefs[WDW_STAT_BARS].loc.sc);
 
    if( control_state.server_state->shell_disabled )
    {
        gCurrentMenu = MENU_GAME;
        memtrack_mark(0, getMenuName(gCurrentMenu));
        gNextMenu = -1;
    }

    if( isMenu(MENU_GAME) )
    {
        int w, h;

        ConnectionStatus connStatus = commCheckConnection();

        if(CS_NOT_RESPONSIVE == connStatus )
        {
            // do we want this here??  Quick feedback, but players can see themselves behind doors
            //DoorAnimReceiveExit(NULL); // make sure to fade back in if disconnected

            windowSize(&w,&h);

            font( &title_18 );
            font_color( CLR_WHITE, CLR_BLUE );
            cprnt( w / 2, h / 2, 90, 1.f, 1.f, "MapserverDown");
        }
        else if(connStatus == CS_DISCONNECTED)
        {
            // MS: This should only happen in debugging mode.

            windowSize(&w,&h);

            font( &title_18 );
            font_color( CLR_WHITE, CLR_BLUE );
            cprnt( w / 2, h / 2, 90, 1.f, 1.f, "MapserverDisconnected");
        }


        toggle_3d_game_modes( SHOW_GAME );

        // martin stuff
        //----------------------------------------------------------------------------------------
        windowSize(&w,&h);

        if( playerIsSlave() || getMySlave() )
        {
            font( &game_12 );
            font_color( CLR_ORANGE, CLR_RED );
            cprnt( w / 2, 40, 90, 2.f, 2.f, playerIsSlave() ? "BeingModded" :
            controlledPlayerPtr() ? "ModdingPlayer" : "WatchingPlayer");

            if(getMySlave()){
                // JS:    Will only been seen by CSR guys.  Translation not needed.
                //        Message store also can't deal with embedded quoted strings yet.
                cprnt( w / 2, 60, 90, 1.f, 1.f, playerIsSlave() ? "" : "ReleasePlayer");
            }
        }

        if( game_state.place_entity )
        {
            font( &game_12 );
            font_color( CLR_ORANGE, CLR_RED );
            cprnt( w / 2, 40, 90, 2.f, 2.f, "Entplacement");

            // JS:    Will only been seen by CSR guys.  Translation not needed.
            //        Message store also can't deal with embedded quoted strings yet.
            cprnt( w / 2, 60, 90, 1.f, 1.f, "placeentity");
        }

        if(control_state.nosync || !control_state.predict)
        {
            font( &game_12 );
            font_color( CLR_RED, CLR_YELLOW );
            cprnt( w / 2, 15, 90, 1.f, 1.f, control_state.nosync ? control_state.predict ? "not syncing to server" : "not predicting movement or syncing to server" : "not predicting movement");
        }

        if( e && e->onArchitect && e->taskforce )
        {
            setClientSideMissionMakerContact(e->taskforce->architect_contact_override, e->taskforce->architect_contact_costume, 
                                    e->taskforce->architect_contact_name, e->taskforce->architect_useCachedCostume, e->taskforce->architect_contact_npc_costume );
            e->taskforce->architect_useCachedCostume = 1;
        }
        else
            setClientSideMissionMakerContact(-1, 0, 0, 0, 0);

        newFeatures_update();
        //----------------------------------------------------------------------------------------
    }

    collisions_off_for_rest_of_frame = FALSE;        //Used for turning of menu collisions once one has occurred on any
    gScrollBarDraggingLastFrame = gScrollBarDragging;
    gScrollBarDragging = FALSE;
    //given frame so two buttons aren't selected at once.

    PERFINFO_AUTO_STOP_START("runState", 1);

    // change entity state if nessecary
    runState();

    // AFK dead-man's switch
    AFKTick();

    certificationClaimAllTick();

    // Misc timers
    TimersTick();

    // this prevents enter from being registered on the same frame as it was hit to end a chat message
    // (keeps you from being stuck in chat)
    if( editting_on )
        editting_on--;

    PERFINFO_AUTO_STOP_START("updateToolTips", 1);

    clearHybridToolTipIfUnused();
    resetHybridToolTipAwareness();

     displayCurrentContextMenu();

    // show dialogs if there are any
    displayDialogQueue();

    if( gNextMenu >= 0 ) //handle menu changes from dialog boxes
    {
        gCurrentMenu = gNextMenu;
        memtrack_mark(0, getMenuName(gCurrentMenu));
        collisions_off_for_rest_of_frame = 1;
        gLastDownX = gLastDownX = 0;
        gNextMenu = -1;
    }

       if( windowUp( WDW_TRADE ) && shell_menu() )
    {
        sendTradeCancel( TRADE_FAIL_I_LEFT );
        window_setMode( WDW_TRADE, WINDOW_DOCKED );
    }

    mapfogSetVisited();

     updateToolTips();

    if (!isMenu( MENU_GAME ))
    {
        // Do this to allow the options menu to work in the front end
        if (!loadingScreenVisible() && menuFuncs[gCurrentMenu] != serveWindows)
            serveWindows();
    }

    PERFINFO_AUTO_STOP();

    {
        #if USE_NEW_TIMING_CODE
            static PerfInfoStaticData* menuInfos[MENU_TOTAL];
        #else
            static PerformanceInfo* menuInfos[MENU_TOTAL];
        #endif
        const char* startName = getMenuName(gCurrentMenu);
        PERFINFO_AUTO_START_STATIC(startName, &menuInfos[gCurrentMenu], 1);

        // execute the function for this particular menu
        if( !game_state.viewCutScene )
            menuFuncs[gCurrentMenu]();

        PERFINFO_AUTO_STOP_CHECKED(startName);
    }
        
    PERFINFO_AUTO_START("setPlayerStanceBits", 1);

    fx_doAll();
    setPlayerStanceBits();

    PERFINFO_AUTO_STOP_START("handleCursor", 1);

    handleCursor();

    PERFINFO_AUTO_STOP();

    if ( isMenu( MENU_ORIGIN ) ||
           isMenu( MENU_POWER ) ||
         isMenu( MENU_PLAYSTYLE ) ||
         isMenu( MENU_ARCHETYPE ) ||
         isMenu( MENU_REDIRECT ) ||
         ( isMenu( MENU_BODY ) && !getPCCEditingMode() && !e->db_id )||
         ( isMenu( MENU_COSTUME ) && !getPCCEditingMode() )||
         ( isMenu( MENU_REGISTER) && !e->db_id ) ||
         ( isMenu( MENU_LOADCOSTUME) && !getPCCEditingMode() && !e->db_id ) ||
         ( isMenu( MENU_SAVECOSTUME) && !getPCCEditingMode() && !e->db_id ) ||
         ( isCurrentMenu_PowerCustomization() && !e->db_id ) ||
         ( isMenu( MENU_LOAD_POWER_CUST ) && !e->db_id )
         )
    {
         sndPlay( "N_MenuMusic_loop", SOUND_MUSIC );
    }
    else if( game_state.cutSceneMusic )
    {
        sndPlay( game_state.cutSceneMusic, SOUND_MUSIC );
    }

     if( game_state.showMouseCoords )
    {
        float x, y;
        calc_scrn_scaling(&x, &y);
        xyprintf( 10, 8, "left %d", mouseDown(MS_LEFT));
        xyprintf( 10, 9, "scaled %.2f, %.2f", gMouseInpCur.x / x, gMouseInpCur.y / y);
        xyprintf( 10, 10, "screen %d, %d", gMouseInpCur.x, gMouseInpCur.y );
    }
}

