/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/Color.h>
#include <utilitieslib/components/earray.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"

#include <utilitieslib/utils/cmdoldparse.h>
#include "cmdparse/cmdgame.h"
#include "gridcoll/gridcache.h"
#include "graphics/gfx.h"

#include "player/player.h"
#include "UI/uiKeybind.h"
#include "UI/uiWindows.h"
#include "UI/uiConsole.h"
#include "bases/baseedit.h"
#include "gameComm/wdwbase.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include "bases/baseclientsend.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/language/MessageStore.h>
#include <utilitieslib/language/MessageStoreUtil.h>

#include "UI/uiBaseInput.h"
#include "UI/uiBaseRoom.h"
#include "UI/uiBaseInventory.h"
#include "UI/uiNet.h"
#include "UI/uiCursor.h"
#include "UI/uiTray.h"
#include <utilitieslib/utils/file.h>
#include "UI/uiGame.h"
#include "UI/uiScrollBar.h"
#include "UI/uiCursor.h"

static bool s_bWasLooking = false;

enum
{
    BCMD_QUIT,
    BCMD_ZOOM,
    BCMD_ZOOMIN,
    BCMD_ZOOMOUT,
    BCMD_FORWARD,
    BCMD_BACKWARD,
    BCMD_LEFT,
    BCMD_RIGHT,
    BCMD_TURNLEFT,
    BCMD_TURNRIGHT,
    BCMD_MOUSELOOK,
    BCMD_DRAG,
    BCMD_SELECT,
    BCMD_DRAGROOM,
    BCMD_CENTER_VIEW,
    BCMD_ROTATE,
    BCMD_CENTER_SEL,
    BCMD_SELL,
    BCMD_TOTAL,
    BCMD_SELECT_NEXT,
    BCMD_SELECT_LAST,
};


KeyBindProfile baseEdit_binds_profile;
BaseEditState baseEdit_state = {0};
static float s_fOriginalVisScale = 1.0f;


static char tmp_str[1024], tmp_str2[1024];
static int tmp_int, tmp_int2, tmp_int3;

Cmd baseEdit_cmds[] =
{
    { 0, "quit",        BCMD_QUIT,            {{0}}, 0, "Stop base editing."},
    { 0, "mousedrag",    BCMD_DRAG,            {{PARSETYPE_S32, &tmp_int}},            0, "Start a mouse drag." },
    { 0, "baseselect",    BCMD_SELECT,        {0},                                            0, "Selects at mouse position." },
    { 0, "dragroom",    BCMD_DRAGROOM,        {{PARSETYPE_S32, &tmp_int},{PARSETYPE_S32, &tmp_int2}},    0, "Drags a room around <x,y>" },
    { 0, "center",        BCMD_CENTER_VIEW,    {0},                                            0, "Centers view on current x/y." },
    { 0, "rotate",        BCMD_ROTATE,        {{PARSETYPE_S32, &tmp_int}},                            0, "Rotate Selection." },
    { 0, "centersel",    BCMD_CENTER_SEL,    {0},                                            0, "Center on current selection."},
    { 0, "sell",        BCMD_SELL,            {{0}},                                            0, "Sell current selection." },
    { 0, "select_next",    BCMD_SELECT_NEXT,    {{0}},                                            0, "Select next visible detail" },
    { 0, "select_last",    BCMD_SELECT_LAST,    {{0}},                                            0, "Select previous visible detail" },
    { 0 },
};


CmdList baseEdit_cmdlist =
{
    {{ baseEdit_cmds },
    { 0 }}
};

int ignoreInput() {
    if( control_state.canlook || collisions_off_for_rest_of_frame || cursor.dragging || gWindowDragging || gScrollBarDragging)
        return true;
    if (window_collision())
        return true;
    return false;
}

/**********************************************************************func*
 * baseEditCmdParse
 *
 */
int baseEditCmdParse(char *str, int x, int y)
{
    Cmd            *cmd;
    CmdContext    output = {0};
    bool        didEditCommand=false;
    bool        needRefresh=false;

    output.found_cmd = false;
    output.access_level = cmdAccessLevel();
    cmd = cmdOldRead(&baseEdit_cmdlist,str,&output);

    if (output.msg[0])
    {
        const char *unknowncmd = msGetUnformattedMessageConst(menuMessages,"UnknownCommandString");
        const char *head = strchr(unknowncmd, '{');

        // Compare everything up to the first parameter.
        if (unknowncmd && head && strncmp(output.msg,unknowncmd,head-unknowncmd)==0)
            return 0;

        conPrintf("%s",output.msg);
        return 1;
    }

    if (!cmd || !g_base.rooms)
        return 0;

    switch(cmd->num)
    {
        case BCMD_QUIT:
            baseedit_CancelDrags();
            baseedit_CancelSelects();
            break;

        case BCMD_ZOOM:
            baseedit_SetCamDist();
            break;

        case BCMD_SELECT:
            if(!ignoreInput())
                baseedit_Select(x, y);
            break;

        case BCMD_DRAGROOM:
            if(!ignoreInput())
                baseedit_StartRoomDrag(tmp_int, tmp_int2, NULL);
            break;

        case BCMD_CENTER_VIEW:
            if(!ignoreInput())
                baseCenterXY(x, y);
            break;
        case BCMD_CENTER_SEL:
            baseedit_LookAtCur();
            break;

        case BCMD_DRAG:
              if(tmp_int && !ignoreInput() && baseedit_Mode() != kBaseEdit_AddPersonal)
            {
                baseEdit_state.dragging = true;
                g_iXDragStart = x;
                g_iYDragStart = y;
            }
            else
            {
                baseEdit_state.dragging = false;
            }
            break;

        case BCMD_ROTATE:
            baseedit_Rotate(tmp_int);
            break;

        case BCMD_SELL:
            baseedit_DeleteCurDetail();
            baseedit_SetCurDetail(NULL,NULL);
            break;

        case BCMD_SELECT_NEXT:
            baseedit_SelectNext(false);
            break;
        case BCMD_SELECT_LAST:
            baseedit_SelectNext(true);
            break;

        default:
            break;
    }

    return 1;
}


/**********************************************************************func*
 * baseEditKeybindInit
 *
 */
void baseEditKeybindInit(void)
{
    static int baseEditInitialized = 0;

    if(!baseEditInitialized)
    {
        baseEditInitialized = 1;

        cmdOldInit(baseEdit_cmds);

        bindKeyProfile(&baseEdit_binds_profile);
        memset(&baseEdit_binds_profile, 0, sizeof(baseEdit_binds_profile));
        baseEdit_binds_profile.name = "baseEdit";
        baseEdit_binds_profile.parser = baseEditCmdParse;
        baseEdit_binds_profile.trickleKeys = 1;

        bindKey("esc",                "quit",                0);
        bindKey("leftdrag",            "+mousedrag",        0);
        bindKey("leftclick",        "baseselect",        0);
        bindKey("r",                "rotate 0",            0);
        bindKey("leftdoubleclick",    "center",            0);
        bindKey("RightClick",        "rotate 0",            0);
        bindKey("delete",            "sell",                0);
        bindKey("tab",                "select_next",        0);
        unbindKeyProfile(&baseEdit_binds_profile);
    }
}




/**********************************************************************func*
 * baseEditToggle
 *
 */
void baseEditToggle(int iOn)
{
    Entity *e = playerPtr();

    if(iOn == game_state.edit_base)
        return;

    if( !isDevelopmentMode() )
    {
        if( (iOn == kBaseEdit_Architect || iOn == kBaseEdit_Plot) && !playerCanModifyBase(e, &g_base) )
            return;

        if( iOn == kBaseEdit_AddPersonal && !playerCanPlacePersonal(e, &g_base) )
            return;
    }

    // clear world cursor
    cursor.target_world = kWorldTarget_None;
    s_ptsActivated = NULL;

    if( iOn )
    {
        if(e->access_level && game_state.local_map_server)
        {
            char mapname[1024];

            if(!e->supergroup)
                return;

            // TODO: Fix this to work right.
            // BASE_TODO: Fix this to work right.
            // This should load my supergroup's base from the DB.
            sprintf(mapname, "loadmap \"supergroupid=%i,userid=0\"", e->supergroup_id);
            cmdParse(mapname);
        }
    }
    baseClientSendMode(iOn);
    if (!iOn) // Let players leave base edit mode after map disconnect
        baseSetLocalMode(0);
}

void baseSetLocalMode(int mode)
{
    Entity *e = playerPtr();
    static int *openwindows;
    static float baseInvX, baseInvY;
    int tempX, tempY;
    int i;
    
    if(mode == game_state.edit_base)
        return;
    
    game_state.edit_base = mode;

    if( mode == kBaseEdit_Architect )
        baseedit_SetCamDistAngle(400.f, RAD(80));
    if( mode == kBaseEdit_Plot )
        baseedit_SetCamDistAngle(800.f, RAD(90));

    setBaseInventorySelectedItem(NULL);
    if( mode )
    {
        gridCacheInvalidate();
        e->xlucency = 0.25f;

        window_getUpperLeftPos(WDW_BASE_INVENTORY, &tempX, &tempY);
        baseInvX = tempX;
        baseInvY = tempY;

        // close all other windows
        for( i = 0; i < MAX_WINDOW_COUNT+custom_window_count; i++ )
        {
            if( window_getMode(i) == WINDOW_DISPLAYING )
            {
                window_setMode(i, WINDOW_SHRINKING );
                eaiPush(&openwindows, i);
            }
        }

        bindKeyProfile(&baseEdit_binds_profile);

        window_setMode(WDW_BASE_PROPS, WINDOW_GROWING);
        window_setMode(WDW_BASE_INVENTORY, WINDOW_GROWING);
        
        if( mode == kBaseEdit_Architect || mode == kBaseEdit_AddPersonal)
        {
            window_setMode(WDW_BASE_ROOM, WINDOW_GROWING);
        }

        s_fOriginalVisScale = game_state.vis_scale;
        setVisScale(30.0f, 1);
    }
    else
    {
        baseedit_CancelDrags();
        baseedit_CancelSelects();
        gridCacheInvalidate();
        if (e)
            e->xlucency = 1.0f;

        unbindKeyProfile(&baseEdit_binds_profile);

        window_setMode(WDW_BASE_PROPS, WINDOW_SHRINKING);
        window_setMode(WDW_BASE_ROOM, WINDOW_SHRINKING);

        while( eaiSize(&openwindows) )
            window_setMode( eaiPop(&openwindows), WINDOW_GROWING );

        window_setDims( WDW_BASE_INVENTORY, baseInvX, baseInvY, 150, -1 );

        setVisScale(s_fOriginalVisScale, 1);
    }
}

/* End of File */
