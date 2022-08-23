
#include "UI/uiQuit.h"
#include "UI/uiUtil.h"
#include "UI/uiChat.h"
#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "UI/uiSuperRegistration.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiDialog.h"
#include "UI/uiCompass.h"
#include "UI/uiEditText.h"
#include "UI/uiLogin.h"
#include "UI/uiEmail.h"
#include "UI/uiContextMenu.h"
#include "UI/uiGroupWindow.h"
#include "cmdparse/cmdgame.h"
#include "clientcomm/clientcomm.h"
#include "clientcomm/authclient.h"
#include "win/win_init.h"
#include "entity/entrecv.h"
#include "graphics/gfx.h"
#include "graphics/gfxLoadScreens.h"
#include "player/player.h"
#include "gameComm/initClient.h"
#include "sound/sound.h"
#include "graphics/FX/fx.h"
#include "UI/uiNet.h"
#include "edit/edit_cmd.h"
#include "entity/costume_client.h"
#include "group/groupscene.h"
#include "graphics/sun.h"
#include "UI/uiTailor.h"
#include "UI/uiSupercostume.h"
#include "arena/arenagame.h"
#include "entity/entity.h"
#include "bases/baseedit.h"
#include "group/group.h"
#include "entity/entclient.h"
#include "gameData/raidstruct.h"
#include "gameData/sgraidClient.h"
#include "UI/uiPlaque.h"
#include "UI/uiFx.h"
#include "UI/uiMissionSearch.h"
#include "UI/uiAutomap.h"
#include "storyarc/zowieClient.h"
#include "UI/uiAuction.h"
#include "UI/uiLogin.h"
#include "entity/entDebug.h"

extern int gClickToMoveButton;

void quitToLogin(void * data)
{
    Entity *e = playerPtr();

    if(e)
    {
        e->logout_login = 0;
        e->logout_timer = 0;
    }

    game_state.pendingTSMapXfer = 0;
    window_closeAlways();
    clearCutScene();
    entDebugClearServerPerformanceInfo();
    resetArenaVars();
    editSetMode(0, 0);
    commDisconnect();
    resetStuffOnMapMove();
    plaqueClearQueue();
    authLogout();
    chatCleanup();
    restartLoginScreen();
    clearDestination( &activeTaskDest );
    clearDestination( &waypointDest );
    clearDestination( &serverDest );
    dialogClearQueue( 1 );
    contextMenu_closeAll();
    emailResetHeaders(1);
    srClearAll();
    costumereward_clear( 0 );
    costumereward_clear( 1 );
    fadingText_clearAll();
    electric_clearAll();
    attentionText_clearAll();
    priorityAlertText_clearAll();
    movingIcon_clearAll();
    fxReInit();
    zowieReset();
    sunSetSkyFadeClient(0, 1, 0.0);
    sceneLoad("scenes/default_scene.txt");
    sndStopAll();
    commNewInputPak();
    searchClearComment();
    entReset();
    playerSetEnt(0);
    groupReset();
    server_visible_state.timestepscale = 1;
    clearAuctionFields();

    loadScreenResetBytesLoaded();
    showBgReset();
    gSentMoTD = 0;
    gSentRespecMsg = 0;
    if( gTailoredCostume )
        costume_destroy( gTailoredCostume );
    gTailoredCostume = 0;
    if( gSuperCostume )
        costume_destroy( gSuperCostume );
    gSuperCostume = 0;

    missionsearch_clearAllPages();

    basedit_Clear();
    gClickToMoveButton = 0;
    gPlayerNumber = 0;
    BaseRaidClearAll();
    if (g_raidinfos)
        eaClearEx(&g_raidinfos, SupergroupRaidInfoDestroy);
    gChatLogonTimer = 0;

}

extern int g_keepPassword;

void quitToCharacterSelect(void *data)
{
    int i;
    char err_msg[1000];

    g_keepPassword = true;
    quitToLogin(data);
    g_keepPassword = false;
    
    s_loggedIn_serverSelected = loginToAuthServer(10); // retry 10 times
    if (s_loggedIn_serverSelected != LOGIN_STAGE_START) // successful login to auth server
    {
        for (i = 0; i < auth_info.server_count; i++)
        {
            if (auth_info.server_count == 1
                || auth_info.servers[i].id == auth_info.last_login_server_id)
            {
                s_loggedIn_serverSelected = loginToDbServer(i, err_msg);
                respondToDbServerLogin(i, err_msg, auth_info.servers[i].name);
            }
        }
    }
}

void promptQuit(char *reason)
{
    dialog(DIALOG_TWO_RESPONSE,-1,-1,-1,-1,reason,"QuitToLogin",quitToLogin,"QuitToDesktop",windowExitDlg,0,0,0,0,0,0,0);
}

int quitWindow()
{
    float x, y, z, wd, ht, scale;
    int color, bcolor;
    Entity * e = playerPtr();

     if( !window_getDims( WDW_QUIT, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
        return 0;

    drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, 0x00000088 );

       if( D_MOUSEHIT == drawStdButton( x + wd/2, y + 20*scale + PIX3*scale, z, 190*scale, 30*scale, CLR_ORANGE, "QuitToLogin", 1.3f*scale, 0 ) )
    {
        window_setMode( WDW_QUIT, WINDOW_DOCKED );
        e->logout_login = 1;
        commSendQuitGame(0);
        return 1;
    }

    if( D_MOUSEHIT == drawStdButton( x + wd/2, y + 55*scale + PIX3*scale, z, 190*scale, 30*scale, CLR_RED, "QuitToCharacterSelect", 1.1f*scale, 0 ) )
    {
        window_setMode( WDW_QUIT, WINDOW_DOCKED );
        e->logout_login = 2;
        commSendQuitGame(0);
        return 1;
    }

      if( D_MOUSEHIT == drawStdButton( x + wd/2, y + 95*scale - PIX3*scale, z, 190*scale, 30*scale, CLR_DARK_RED,  "QuitToDesktop", 1.3f*scale, 0 ) )
    {
        window_setMode( WDW_QUIT, WINDOW_DOCKED );
         e->logout_login = 0;
        commSendQuitGame(0);
    }
    return 0;
}
