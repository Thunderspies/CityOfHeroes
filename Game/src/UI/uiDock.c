
#include "UI/uiWindows.h"
#include "UI/uiInput.h"
#include "UI/uiUtilGame.h"        // for draw frame
#include "UI/uiUtil.h"            // for color definitions
#include "UI/uiGame.h"            // for start_menu
#include "UI/sprite/sprite_font.h"    // for font definitions
#include "UI/sprite/sprite_text.h"    // for font functions
#include "UI/sprite/sprite_base.h"    // for sprites
#include "cmdparse/cmdcommon.h"        // for TIMESTEP
#include <utilitieslib/utils/mathutil.h>        // for ceil()
#include "entity/entVarUpdate.h"    // for CLIENT_INP
#include "clientcomm/clientcomm.h"        // for input_pak
#include "UI/uiNet.h"
#include "UI/uiToolTip.h"
#include "UI/uiContextMenu.h"
#include "assert.h"
#include "gameComm/chatClient.h"
#include "UI/uiChatUtil.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiFriend.h"
#include "UI/uiInfo.h"
#include "player/player.h"
#include "cmdparse/cmdgame.h"
#include "account/AccountData.h"
#include "player/inventory_client.h"

//------------------------------------------------------------------------------------------------------
// Init ////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------

static ContextMenu * dockContext = {0};
static int windowNums[MAX_WINDOW_COUNT] = {0};
static int menuNums[MENU_TOTAL] = {0};

void dock_open()
{
    float x, y;

    childButton_getCenter( wdwGetWindow(WDW_STAT_BARS), 4, &x, &y );
    contextMenu_set( dockContext, x, y, NULL );
}

int dockwindow_isOpen( void * data )
{
    int win = *((int*)data);

    if( windowUp( win ) )
        return CM_CHECKED;
    else
        return CM_AVAILABLE;
}

int dockwindow_usingChatServer( void * data )
{
    if(!UsingChatServer())
        return CM_HIDE;
    else
        return CM_AVAILABLE; // for non-checkbox items
}


int dockwindow_isVIP( void * data )
{
    if(!AccountIsVIP(inventoryClient_GetAcctInventorySet(), inventoryClient_GetAcctStatusFlags()))
        return CM_HIDE;
    else
        return CM_AVAILABLE; // for non-checkbox items
}

void dockwindow_selfInfo(void *data)
{
    if( playerPtr() )
        info_Entity(playerPtr(), 0);
}


void dockwindow_open( void * data )
{
    int win = *((int*)data);

    window_openClose( win );
}

void dockwindow_launch( void * data )
{
    int menu = *((int*)data);
    start_menu( menu );
}

static void dockRunCommand(void* command)
{
    cmdParse((char*)command);
}

void addChannelWindowToMenu(ContextMenu *menu)
{
    contextMenu_addCode(menu, dockwindow_usingChatServer, NULL,    channelWindowOpen, NULL, "CMChannelWindow", NULL );    
    contextMenu_addCode(menu, dockwindow_usingChatServer, NULL,    dockwindow_open, &windowNums[WDW_CHANNEL_SEARCH], "CMChanSearchWindow", NULL );
}

static void initDock()
{
    int i;

    for( i = 0; i < MAX_WINDOW_COUNT; i++ )
        windowNums[i] = i;

    for( i = 0; i < MENU_TOTAL; i++ )
        menuNums[i] = i;

    dockContext = contextMenu_Create( NULL );

    contextMenu_addTitle( dockContext, "CMMenuTitle" );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_CHAT_BOX],        dockwindow_open, &windowNums[WDW_CHAT_BOX],        "CMChatWindow"    );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_FRIENDS],        dockwindow_open, &windowNums[WDW_FRIENDS],        "CMFriendWindow");
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_GROUP],        dockwindow_open, &windowNums[WDW_GROUP],        "CMTeamWindow"    );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_LEAGUE],        dockwindow_open, &windowNums[WDW_LEAGUE],        "CMLeagueWindow"    );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_TURNSTILE],    dockwindow_open, &windowNums[WDW_TURNSTILE],    "CMLFGWindow"    );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_SUPERGROUP],    dockwindow_open, &windowNums[WDW_SUPERGROUP],    "CMSuperWindow" );
//    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_LEVELINGPACT], dockwindow_open, &windowNums[WDW_LEVELINGPACT], "CMLevelingpactWindow");
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_EMAIL],        dockwindow_open, &windowNums[WDW_EMAIL],        "CMEmailWindow" );

    contextMenu_addDivider( dockContext );

    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_TRAY],            dockwindow_open, &windowNums[WDW_TRAY],            "CMTrayWindow"    );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_POWERLIST],    dockwindow_open, &windowNums[WDW_POWERLIST],    "CMPowerWindow" );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_INSPIRATION],    dockwindow_open, &windowNums[WDW_INSPIRATION],    "CMInspWindow"    );
    contextMenu_addCode( dockContext, alwaysAvailable, &menuNums[MENU_COMBINE_SPEC], dockwindow_launch, &menuNums[MENU_COMBINE_SPEC], "CMEnhanceMenu", 0 );

    contextMenu_addDivider( dockContext );

    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_COMPASS],    dockwindow_open, &windowNums[WDW_COMPASS],    "CMCompassWindow" );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_MAP],        dockwindow_open, &windowNums[WDW_MAP],        "CMMapWindow"      );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_CONTACT],    dockwindow_open, &windowNums[WDW_CONTACT],    "CMContactWindow" );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_MISSION],    dockwindow_open, &windowNums[WDW_MISSION],    "CMMissionWindow" );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_CLUE],        dockwindow_open, &windowNums[WDW_CLUE],        "CMClueWindow"      );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen, &windowNums[WDW_BADGES],    dockwindow_open, &windowNums[WDW_BADGES],    "CMBadgeWindow"      );

    contextMenu_addDivider( dockContext );

    contextMenu_addCheckBox( dockContext, dockwindow_isOpen,            &windowNums[WDW_TARGET],            dockwindow_open,            &windowNums[WDW_TARGET],            "CMTargetWindow"    );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen,            &windowNums[WDW_HELP],                dockwindow_open,            &windowNums[WDW_HELP],                "CMHelpWindow"        );
    contextMenu_addCheckBox( dockContext, dockwindow_isOpen,            &windowNums[WDW_COSTUME_SELECT],    dockwindow_open,            &windowNums[WDW_COSTUME_SELECT],    "CMCostumeWindow"    );
    contextMenu_addCode(     dockContext, alwaysAvailable,                &windowNums[WDW_ARENA_LIST],        dockRunCommand,                "arena_list",        "CMArenaWindow",    0);
    contextMenu_addCode(     dockContext, alwaysAvailable,                &menuNums[MENU_REGISTER],            dockwindow_launch,            &menuNums[MENU_REGISTER],            "CMIDCard",            0);
    contextMenu_addCode(     dockContext, alwaysAvailable,                0,                                    dockwindow_selfInfo,        0,                                    "CMPersonalInfo",    0);
    contextMenu_addCode(     dockContext, dockwindow_usingChatServer,    0,                                    displayChatHandleDialog,    0,                                    "CMChatHandle",        0);    
    contextMenu_addCode(     dockContext, alwaysAvailable,                &windowNums[WDW_LOYALTY_TREE],        dockwindow_open,            &windowNums[WDW_LOYALTY_TREE],        "CMLoyaltyTreeWindow",    0);
    contextMenu_addCode(     dockContext, alwaysAvailable,                &windowNums[WDW_OPTIONS],            dockwindow_open,            &windowNums[WDW_OPTIONS],            "CMOptionsMenu",    0);
    contextMenu_addCode(     dockContext, alwaysAvailable,                &windowNums[WDW_PETITION],            dockwindow_open,            &windowNums[WDW_PETITION],            "CMSupport",        0);
    contextMenu_addCode(     dockContext, alwaysAvailable,                &windowNums[WDW_QUIT],                dockwindow_open,            &windowNums[WDW_QUIT],                "CMQuitWindow",        0);
}

//
// master control for the dock
//
int dock_draw()
{
    static int init = 0;

    if( !init )
    {
        initDock();
        init = 1;
    }

    return 0;
}





