/***************************************************************************
*     Copyright (c) 2000-2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
***************************************************************************/

#ifndef WDWBASE_H__
#define WDWBASE_H__

#include <utilitieslib/utils/Cbox.h>

typedef enum WindowName
{
    WDW_DOCK,
    WDW_STAT_BARS,
    WDW_TARGET,
    WDW_TRAY,
    WDW_CHAT_BOX,
    WDW_POWERLIST,
    WDW_GROUP,
    WDW_COMPASS,
    WDW_MAP,
    WDW_CHAT_OPTIONS,
    // 10
    WDW_FRIENDS,
    WDW_CONTACT_DIALOG,
    WDW_INSPIRATION,
    WDW_SUPERGROUP,
    WDW_EMAIL,
    WDW_EMAIL_COMPOSE,
    WDW_CONTACT,
    WDW_MISSION,
    WDW_CLUE,
    WDW_TRADE,
    // 20
    WDW_QUIT,
    WDW_INFO,
    WDW_HELP,
    WDW_MISSION_SUMMARY,
    WDW_TARGET_OPTIONS,
    WDW_BROWSER,
    WDW_LFG,
    WDW_STORE,
    WDW_DIALOG,
    WDW_BETA_COMMENT,
    // 30
    WDW_PETITION,
    WDW_TITLE_SELECT,
    WDW_DEATH,
    WDW_MAP_SELECT,
    WDW_COSTUME_SELECT,
    WDW_ENHANCEMENT,
    WDW_BADGES,
    WDW_REWARD_CHOICE,
    WDW_CHAT_CHILD_1,
    WDW_CHAT_CHILD_2,
    // 40
    WDW_CHAT_CHILD_3,
    WDW_CHAT_CHILD_4,
    WDW_DEPRECATED_1, // can't use this any more because window doesn't exist
    WDW_ARENA_CREATE,
    WDW_ARENA_LIST,
    WDW_ARENA_RESULT,
    WDW_ARENA_JOIN,
    WDW_UNUSED_1, // unused
    WDW_RENDER_STATS,
    WDW_BASE_PROPS,
    // 50
    WDW_BASE_INVENTORY,
    WDW_BASE_ROOM,
    WDW_INVENTORY,
    WDW_SALVAGE,
    WDW_CONCEPTINV,
    WDW_RECIPEINV,
    WDW_INVENT,
    WDW_SUPERGROUP_LIST,
    WDW_PET,
    WDW_ARENA_GLADIATOR_PICKER,
    // 60
    WDW_WORKSHOP, // defunct, replaced by RECIPEINVENTORY
    WDW_OPTIONS,
    WDW_SGRAID_LIST,
    WDW_SGRAID_TIME,
    WDW_SGRAID_SIZE,
    WDW_EDITOR_UI_WINDOW_1,
    WDW_EDITOR_UI_WINDOW_2,
    WDW_EDITOR_UI_WINDOW_3,
    WDW_EDITOR_UI_WINDOW_4,
    WDW_EDITOR_UI_WINDOW_5,
    // 70
    WDW_CHANNEL_SEARCH,
    WDW_BASE_STORAGE,
    WDW_BASE_LOG,
    WDW_EDITOR_UI_WINDOW_6,
    WDW_EDITOR_UI_WINDOW_7,
    WDW_EDITOR_UI_WINDOW_8,
    WDW_EDITOR_UI_WINDOW_9,
    WDW_EDITOR_UI_WINDOW_10,
    WDW_PLAQUE,
    WDW_SGRAID_STARTTIME,
    // 80
    WDW_RAIDRESULT,
    WDW_RECIPEINVENTORY,
    WDW_AUCTIONHOUSE,
    WDW_STOREDSALVAGE,
    WDW_AMOUNTSLIDER,
    WDW_DEPRECATED_2, //was WDW_GENERICPAYMENT
    WDW_COMBATNUMBERS,
    WDW_COMBATMONITOR,
    WDW_TRIALREMINDER, 
    WDW_TRAY_1,
    // 90
    WDW_TRAY_2,
    WDW_TRAY_3,
    WDW_TRAY_4,
    WDW_TRAY_5,
    WDW_TRAY_6,
    WDW_TRAY_7,
    WDW_TRAY_8,    
    WDW_COLORPICKER,    
    WDW_PLAYERNOTE,
    WDW_RECENTTEAM,
    //100
    WDW_MISSIONMAKER,
    WDW_MISSIONSEARCH,
    WDW_MISSIONREVIEW,
    WDW_BADGEMONITOR,
    WDW_CUSTOMVILLAINGROUP,
    WDW_BASE_STORAGE_PERMISSIONS,
    WDW_ARENA_OPTIONS,
    WDW_MISSIONCOMMENT,
    WDW_INCARNATE,
    WDW_INCARNATE_BAR,        //    removed
    //110
    WDW_POP_HELP,
    WDW_POP_HELP_TEXT,
    WDW_SCRIPT_UI,
    WDW_AUCTION,
    WDW_KARMA_UI,
    WDW_LEAGUE,
    WDW_TURNSTILE,
    WDW_TURNSTILE_DIALOG,
    WDW_TRAY_RAZER,
    WDW_CONTACT_FINDER,
    //120
    WDW_LOYALTY_TREE,
    WDW_WEB_STORE, // not used anymore, enum left in here to maintain protocol compatibility
    WDW_MAIN_STORE_ACCESS, // not used anymore, enum left in here to maintain protocol compatibility
    WDW_LWC_UI,
    WDW_LOYALTY_TREE_ACCESS,
    WDW_SALVAGE_OPEN,
    WDW_CONVERT_ENHANCEMENT,
    WDW_NEW_FEATURES,

    /*!@!--> Only add to the bottom of the list, or else bad things will happen <--!@!*/
    /*!@!--> Add new windows above this line <--!@!*/
    MAX_WINDOW_COUNT,
} WindowName;

#define MAX_CUSTOM_WINDOW_COUNT 25
// These windows will now be saved to server ever so they can exist outside the window count range
C_DECLARATIONS_BEGIN
extern int custom_window_count;
C_DECLARATIONS_END

typedef enum WindowDisplayMode
{
    WINDOW_UNINITIALIZED,
    WINDOW_GROWING,
    WINDOW_DISPLAYING,
    WINDOW_SHRINKING,
    WINDOW_DOCKED,
} WindowDisplayMode;

typedef enum WindowStartState
{
    DEFAULT_OPEN,
    DEFAULT_CLOSED,
    ALWAYS_CLOSED,
} WindowStartState;

typedef enum WindowDragState
{
    NOT_RESIZABLE,
    RESIZABLE,
    VERTONLY,
    HORZONLY,
    TRAY_FIXED_SIZES,
}WindowDragState;

typedef struct WdwBase
{
    int xp; // now stores the x-pos of the window's anchor point, as defined by WindowDefault.
    int yp; // now stores the y-pos "
    // Please use xp and yp directly, but be sure you're writing your code knowing that those
    // aren't necessarily the upper-left corner of the window!
    // If you're still in upper-left world, ask window_getUpperLeftPos() to get the coordinates.
    // It will do the anchor logic for you.  I don't want to spread the use of that function
    // further than it is now, though, because that extra layer of indirection is unnecessary.

    int wd;
    int ht;
    int draggable_frame;
    int    locked;
    int start_shrunk;
    int    color;
    int    back_color;
    float sc;
    int    maximized;

    WindowDisplayMode   mode;

} WdwBase;

typedef struct Wdw Wdw;

typedef struct ChildWindow
{
    int        idx;
    Wdw        *wdw;

    //float    wd;
    float    opacity;

    //int        arrow;
    char    name[32];
    char    *command;

    struct {
        float width;
        float scale;
    } cached;

}ChildWindow;

#define WINDOW_MAX_CHILDREN 11

typedef struct Wdw
{
    // Server tracks these
    WdwBase loc;

    // Server doesn't track these
    int    (* code )();  // function pointer to execution code

    WdwBase loc_minimized; // used for custom windows, allowing them to have a separate location when minimized

    char*    icon;    // unused?
    int        id;        // self-reference
    int     window_depth;        //highest # is top
    int        editor;                //whether it is displayed in or out of the editor

    // resizing the frame
    int max_ht;
    int max_wd;
    int min_ht;
    int min_wd;
    int    drag_mode;

    // drag variables
    int     being_dragged;
    int        mouseDownX;            // These used to keep track of the point on screen where the mouse was pressed
    int        mouseDownY;            //
    int        relX;                // The coordinates of the mousedown relative to the window itself
    int        relY;                //

    CBox    cbox;
    float    timer;

    // drawing parameters
    int        radius;            // frame curvature
    int        below;
    int        left;
    int        flip;
    float    opacity, min_opacity;
    float    fadeTimer;
    int        save;
    int        forceColor;
    int        noCloseButton;
    int        useTitle;
    int        forceNoFlip;
    int        noFrame;
    char    title[50];
    int        maximizeable;

    // children/parents
    Wdw *parent;
    int num_children;
    int open_child;
    int child_id;

    ChildWindow        children[WINDOW_MAX_CHILDREN];

} Wdw;

extern Wdw winDefs[MAX_CUSTOM_WINDOW_COUNT+MAX_WINDOW_COUNT];

typedef struct Packet Packet;

void sendWindow(Packet *pak, WdwBase *pwdw, int idx); // Just sends the data

#if CLIENT
void sendWindowToServer(WdwBase *pwdw, int idx, int save); // Sends command ID also
#endif

void receiveWindowIdx(Packet *pak, int *idx);
void receiveWindow(Packet *pak, WdwBase *pwdw);

Wdw* wdwCreate(WindowName id, int custom);
Wdw* wdwGetWindow(WindowName name);
#endif /* #ifndef WDWBASE_H__ */

/* End of File */

