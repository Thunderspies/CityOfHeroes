
#include <utilitieslib/components/ArrayOld.h>
#include "cmdparse/cmdgame.h"
#include "UI/uiConsole.h"
#include "render/tex.h"
#include "UI/uiCursor.h"
#include "graphics/font.h"
#include "graphics/ttFont.h"
#include "render/renderUtil.h"
#include "render/render.h"
#include "win/win_init.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiGame.h"
#include "graphics/gfxwindow.h"
#include "player/player.h"
#include "graphics/camera.h"
#include "utils/position.h"
#include "gridcoll/gridcoll.h"
#include "cmdparse/cmdcommon.h"
#include "win/input.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/MemoryPool.h>
#include "gameComm/initClient.h"
#include "clientcomm/clientcomm.h"
#include "assert.h"
#include "entity/entrecv.h"
#include "edit/edit_cmd.h"
#include "entity/entclient.h"
#include "entity/character_base.h"
#include "UI/uiKeybind.h"
#include "render/renderstats.h"
#include "gameComm/Npc.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/strings_opt.h>
#include "entity/motion.h"
#include "cmdparse/cmdcontrols.h"
#include "entity/scriptDebugClient.h"
#include "entity/entDebug.h"
#include "edit/edit_drawlines.h"
#include "entity/entDebugPrivate.h"
#include "UI/uiUtil.h"
#include "render/renderprim.h"
#include "graphics/gfx.h"
#include "graphics/gfxLoadScreens.h"
#include "entity/entity.h"
#include "player/pmotion.h"
#include "NovodeX/NwWrapper.h"
#include "NovodeX/NwRagdoll.h"
#include "group/group.h"
#include "group/groupfileload.h"
#include <utilitieslib/utils/StringUtil.h>
#include "demo.h"
#include "edit/ClickToSource.h"
#include <utilitieslib/components/StashTable.h>
#include "cmdparse/cmdDebug.h"
#include "UI/uiUtilGame.h"
#include <utilitieslib/utils/Quat.h>

#include "edit/edit_library.h"
#include "edit/edit_select.h"
#include "edit/edit_net.h"
#include "edit/edit_cmd_select.h"
#include "edit/edit_select.h"
#include "graphics/groupdrawutil.h"
#include "seq/anim.h"
#include "edit/edit_cmd_file.h"
#include "entity/powers.h"
#include "entity/character_net.h"
#include "UI/uiInput.h"
#include <utilitieslib/utils/sysutil.h>
#include "UI/uiKeymapping.h"
#include "UI/uiNet.h"
#include "entity/entPlayer.h"
#include "UI/uiCustomWindow.h"

typedef struct ClientEntityDebugPoint
{
    Vec3    pos;
    int        argb;
    char*    tag;
    U32        lineToEnt        : 1;
    U32        lineToPrevious    : 1;
} ClientEntityDebugPoint;

typedef struct ClientEntityDebugPathPoint
{
    Vec3    pos;
    F32        minHeight;
    F32        maxHeight;
    U32        isFlying : 1;
} ClientEntityDebugPathPoint;

typedef struct ClientEntityDebugInfo 
{
    char*                        text;

    struct 
    {
        int                        count;
        ClientEntityDebugPathPoint* points;
    } path;

    struct 
    {    
        int                        count;
        int                        maxCount;
        ClientEntityDebugPoint*    pos;
    } points;

    int*                        teamMembers;
} ClientEntityDebugInfo;

MP_DEFINE(DebugMenuItem);
MP_DEFINE(DebugButton);

MP_DEFINE(AStarRecordingSet);

DebugState debug_state = {0};

enum
{
    CMD_DEBUGMENU_UP= 1,
    CMD_DEBUGMENU_DOWN,
    CMD_DEBUGMENU_PAGEUP,
    CMD_DEBUGMENU_PAGEDOWN,
    CMD_DEBUGMENU_SELECT,
    CMD_DEBUGMENU_OPEN,
    CMD_DEBUGMENU_CLOSE,
    CMD_DEBUGMENU_CLOSEPARENT,
    CMD_DEBUGMENU_EXIT,
    CMD_DEBUGMENU_COPY,
    CMD_DEBUGMENU_BIND,
    CMD_DEBUGMENU_MACRO,
    CMD_DEBUGMENU_CUSTOMWINDOW,
};

#define PERF_INFO_WIDTH 850

KeyBindProfile        ent_debug_binds_profile;
F32                    g_BeaconDebugRadius = 250.0f;

int entDebugCmdParse(char *str, int x, int y);

Cmd ent_debug_cmds[] =
{
    { 0, "debugmenu_up",            CMD_DEBUGMENU_UP, {{ PARSETYPE_S32, &debug_state.upHeld }} },

    { 0, "debugmenu_down",            CMD_DEBUGMENU_DOWN, {{ PARSETYPE_S32, &debug_state.downHeld }} },

    { 0, "debugmenu_pageup",        CMD_DEBUGMENU_PAGEUP, {{ PARSETYPE_S32, &debug_state.pageupHeld }} },

    { 0, "debugmenu_pagedown",        CMD_DEBUGMENU_PAGEDOWN, {{ PARSETYPE_S32, &debug_state.pagedownHeld }} },

    { 0, "debugmenu_select",        CMD_DEBUGMENU_SELECT, {{ PARSETYPE_S32, &debug_state.closeMenu }, { PARSETYPE_S32, &debug_state.fromMouse }} },

    { 0, "debugmenu_open",            CMD_DEBUGMENU_OPEN, {{ 0 }} },

    { 0, "debugmenu_close",            CMD_DEBUGMENU_CLOSE, {{ 0 }} },

    { 0, "debugmenu_closeparent",    CMD_DEBUGMENU_CLOSEPARENT, {{ 0 }} },

    { 0, "debugmenu_exit",            CMD_DEBUGMENU_EXIT, {{ 0 }} },

    { 0, "debugmenu_copy",            CMD_DEBUGMENU_COPY, {{ 0 }} },

    { 0, "debugmenu_bind",            CMD_DEBUGMENU_BIND, {{ 0 }} },

    { 0, "debugmenu_macro",            CMD_DEBUGMENU_MACRO, {{ 0 }} },

    { 0, "debugmenu_customwindow",    CMD_DEBUGMENU_CUSTOMWINDOW, {{ 0 }} },
    { 0 },
};

CmdList ent_debug_cmdlist =
{
    {{ ent_debug_cmds },
    { 0 }}
};

#define MP_BASIC(type, size)        \
    MP_DEFINE(type);                \
    type* create##type(){            \
        MP_CREATE(type, size);        \
                                    \
        return MP_ALLOC(type);        \
    }                                \
    void destroy##type(type* chunk){\
        MP_FREE(type, chunk);        \
    }

MP_BASIC(AStarConnectionInfo, 4096);
MP_BASIC(AStarPoint, 500);

void destroyAStarRecordingSet(AStarRecordingSet* set){
    eaDestroyEx(&set->points, destroyAStarPoint);
    eaDestroy(&set->nextPoints);
    eaClearEx(&set->nextConns, destroyAStarConnectionInfo);

    MP_FREE(AStarRecordingSet, set);
}

MP_DEFINE(AStarBeaconBlock);

AStarBeaconBlock* createAStarBeaconBlock(){
    MP_CREATE(AStarBeaconBlock, 100);
    
    return MP_ALLOC(AStarBeaconBlock);
}

void destroyAStarBeaconBlock(AStarBeaconBlock* block){
    eaClearEx(&block->beacons, destroyAStarPoint);

    MP_FREE(AStarBeaconBlock, block);
}

int entDebugMenuVisible(){
    return debug_state.mouseOverUI || debug_state.menuItem ? 1 : 0;
}

void setMouseOverDebugUI(int on)
{
    debug_state.mouseOverUI = on;
}

void initEntDebug()
{
    static int entDebugInitialized = 0;

    if(!entDebugInitialized)
    {
        entDebugInitialized = 1;

        cmdOldInit(ent_debug_cmds);

        memset(&debug_state, 0, sizeof(debug_state));

        debug_state.moveMouseToSel = -1;
        debug_state.retainMenuState = 1;
        debug_state.motdVisible = 1;

        bindKeyProfile(&ent_debug_binds_profile);
        memset(&ent_debug_binds_profile, 0, sizeof(ent_debug_binds_profile));
        ent_debug_binds_profile.name = "Debug Commands";
        ent_debug_binds_profile.parser = entDebugCmdParse;
        ent_debug_binds_profile.trickleKeys = 0;

        bindKey("up",        "+debugmenu_up",            0);
        bindKey("down",        "+debugmenu_down",            0);
        bindKey("pageup",    "+debugmenu_pageup",        0);
        bindKey("pagedown",    "+debugmenu_pagedown",        0);
        bindKey("right",    "debugmenu_open",            0);
        bindKey("left",        "debugmenu_close",            0);
        bindKey("enter",    "debugmenu_select 1 0",        0);
        bindKey("space",    "debugmenu_select 0 0",        0);
        bindKey("esc",        "debugmenu_exit",            0);
        bindKey("m",        "debugmenu_exit",            0);
        bindKey("lbutton",    "debugmenu_select 1 1",        0);
        bindKey("mbutton",    "debugmenu_closeparent",    0);
        bindKey("rbutton",    "debugmenu_select 0 1",        0);
        bindKey("ctrl+c",    "debugmenu_copy",            0);
        bindKey("ctrl+x",    "debugmenu_copy",            0);
        bindKey("ctrl+b",    "debugmenu_bind",            0);
        bindKey("ctrl+m",    "debugmenu_macro",            0);
        bindKey("ctrl+w",    "debugmenu_customwindow",    0);
        unbindKeyProfile(&ent_debug_binds_profile);
    }
}

void entDebugClearLines(){
    debug_state.linesCount = 0;
    debug_state.curLine = 0;

    debug_state.textCount = 0;
    debug_state.curText = 0;
}

void entDebugAddLine(const Vec3 p1, int argb1, const Vec3 p2, int argb2)
{
    #define MAX_DEBUG_LINES (10000)
    
    EntDebugLine* line;

    initEntDebug();
    
    if(!debug_state.lines){
        debug_state.lines = calloc(sizeof(*debug_state.lines), MAX_DEBUG_LINES);
    }

    debug_state.linesCount = min(debug_state.linesCount + 1, MAX_DEBUG_LINES);

    line = debug_state.lines + debug_state.curLine;

    copyVec3(p1, line->p1);
    line->argb1 = argb1;

    copyVec3(p2, line->p2);
    line->argb2 = argb2;

    debug_state.curLine = (debug_state.curLine + 1) % MAX_DEBUG_LINES;
}

void entDebugAddText(Vec3 pos, const char* textString, int flags){
    EntDebugText* text;
    int len = strlen(textString);

    initEntDebug();

    debug_state.textCount = min(debug_state.textCount + 1, ARRAY_SIZE(debug_state.text));

    text = debug_state.text + debug_state.curText;

    copyVec3(pos, text->pos);
    strncpy(text->text, textString, min(len, sizeof(text->text) - 1) + 1);
    text->flags = flags;

    debug_state.curText = (debug_state.curText + 1) % ARRAY_SIZE(debug_state.text);
}

void freeDebugMenuItem(DebugMenuItem* item)
{
    if(!item)
        return;

    while(item->subItems.head)
    {
        DebugMenuItem* next = item->subItems.head->nextSibling;
        freeDebugMenuItem(item->subItems.head);
        item->subItems.head = next;
    }

    SAFE_FREE(item->rolloverText);

    MP_FREE(DebugMenuItem, item);
}

static DebugMenuItem *matchMenuItem( DebugMenuItem *item, int *sel, char * str )
{
     if( strnicmp( item->displayText, str, strlen(str) ) == 0 )
    {
        return item;
    }
    (*sel)++;

    if( item->open )
    {
        for(item = item->subItems.head; item; item = item->nextSibling)
        {
            DebugMenuItem* selItem;
            selItem = matchMenuItem(item, sel, str);
            if(selItem)
                return selItem;
        }
    }
    return NULL;

}

static DebugMenuItem* getCurMenuItem(DebugMenuItem* item, int* sel)
{
    if(!item || !*sel)
        return item;

    if(item->open)
    {
        for(item = item->subItems.head; *sel && item; item = item->nextSibling)
        {
            DebugMenuItem* selItem;
            (*sel)--;
            selItem = getCurMenuItem(item, sel);
            if(selItem)
                return selItem;
        }
    }

    return NULL;
}

static void parseDebugCommands(char* commands)
{
    char* str;
    char* cur;
    char delim;
    char buffer[10000];

    strcpy(buffer, commands);

    cur = buffer;

    for(str = strsep2(&cur, "\n", &delim); str; str = strsep2(&cur, "\n", &delim)){
        cmdParse(str);
        if(delim){
            cur[-1] = delim;
        }
    }
}

static StashElement menuItemGetElement(DebugMenuItem* item, int create){
    char buffer[10000];
    DebugMenuItem* curItem = item;
    StashElement element;

    if(!debug_state.menuItemNameTable){

        debug_state.menuItemNameTable = stashTableCreateWithStringKeys(100,  StashDeepCopyKeys | StashCaseSensitive );
    }

    STR_COMBINE_BEGIN(buffer);

    while(curItem){
        char* displayText = strstr(curItem->displayText, "$$");

        if(displayText)
            *displayText = 0;

        if(item != curItem){
            STR_COMBINE_CAT("::");
        }
        
        STR_COMBINE_CAT(curItem->displayText);

        curItem = curItem->parent;

        if(displayText)
            *displayText = '$';
    }
    
    STR_COMBINE_END();

    stashFindElement(debug_state.menuItemNameTable, buffer, &element);

    if(!element && create){
        stashAddPointerAndGetElement(debug_state.menuItemNameTable, buffer, 0, false, &element);
    }

    return element;
}

static void menuItemSetOpen(DebugMenuItem* item, int open){
    StashElement element = menuItemGetElement(item, 1);

    item->open = open;

    stashElementSetPointer(element, (void*)open);
}

static void menuItemInitOpenState(DebugMenuItem* item){
    if(debug_state.retainMenuState){
        StashElement element = menuItemGetElement(item, 0);

        if(element){
            item->open = (int)stashElementGetPointer(element) ? 1 : 0;
        }
    }
}

static void addDebugMenuItemTail(DebugMenuItem* parent, DebugMenuItem* child)
{
    if (parent)
    {
        if(parent->subItems.tail)
            parent->subItems.tail->nextSibling = child;
        else
            parent->subItems.head = child;

        parent->subItems.tail = child;
    }

    child->nextSibling = NULL;
}

DebugMenuItem* receiveDebugMenuItem(Packet* pak, DebugMenuItem* parent, char prefixes[][1000], int* isPrefixOut){
    char buffer[1000];
    DebugMenuItem* item;
    int hasSubItems;
    int open;

    // Receive the end of group bit.

    if(!pktGetBits(pak, 1)){
        return NULL;
    }

    hasSubItems = pktGetBits(pak, 1);

    open = pktGetBits(pak, 1);

    // HACK ALERT!!!  If pre-opening a non-subgroup, this means set the prefixes. YUCK!

    if(open && !hasSubItems){
        if(prefixes){
            strcpy(prefixes[0], pktGetString(pak));
            strcpy(prefixes[1], pktGetString(pak));
        }else{
            pktGetString(pak);
            pktGetString(pak);
        }

        if(isPrefixOut){
            *isPrefixOut = 1;
        }

        return NULL;
    }

    // This is an actual menu item, so create it.

    item = MP_ALLOC(DebugMenuItem);

    item->parent = parent;

    if(hasSubItems){
        item->open = open;
    }

    // Get the rollover text.

    if(!open || hasSubItems){
        if(pktGetBits(pak, 1)){
            item->rolloverText = strdup(pktGetString(pak));
        }
    }

    // Get the display text, and tack on the prefix.

    Strncpyt(item->displayText, pktGetString(pak));

    if(prefixes && prefixes[0][0]){
        STR_COMBINE_BEGIN(buffer);
        STR_COMBINE_CAT(prefixes[0]);
        STR_COMBINE_CAT(item->displayText);
        STR_COMBINE_END();
        Strncpyt(item->displayText, buffer);
    }

    if(hasSubItems){
        menuItemInitOpenState(item);
    }

    // Get the command string.

    if(!hasSubItems){
        Strncpyt(item->commandString, pktGetString(pak));

        if(prefixes && prefixes[1][0]){
            STR_COMBINE_BEGIN(buffer);
            STR_COMBINE_CAT(prefixes[1]);
            STR_COMBINE_CAT(item->commandString);
            STR_COMBINE_END();
            strcpy(item->commandString, buffer);
        }
    }else{
        DebugMenuItem* subItem;
        char prefixArray[2][1000] = {"",""};
        int isPrefix = 0;
        U32 startSize = pak->stream.cursor.byte * 8 + pak->stream.cursor.bit;

        while((subItem = receiveDebugMenuItem(pak, item, prefixArray, &isPrefix)) || isPrefix){
            if(!isPrefix){
                subItem->bitsUsed = pak->stream.cursor.byte * 8 + pak->stream.cursor.bit - startSize;

                addDebugMenuItemTail(item, subItem);
            }else{
                isPrefix = 0;
            }

            startSize = pak->stream.cursor.byte * 8 + pak->stream.cursor.bit;
        }
    }

    return item;
}

static TokenizerParseInfo parseDebugCommand[] = {
    { "",                    TOK_STRUCTPARAM | TOK_STRING(FileDebugMenu,name, 0) },
    { "",                    TOK_STRUCTPARAM | TOK_STRING(FileDebugMenu,command, 0) },
    { "\n",                    TOK_END,                            0 },
    { "", 0, 0 }
};

static TokenizerParseInfo parseDebugAddCommand[] = {
    { "",                    TOK_STRUCTPARAM | TOK_STRING(FileDebugMenu,command, 0) },
    { "\n",                    TOK_END,                            0 },
    { "", 0, 0 }
};

static TokenizerParseInfo parseDebugMenu[] = {
    { "",                    TOK_STRUCTPARAM | TOK_STRING(FileDebugMenu,name, 0) },
    { "Open:",                TOK_INT(FileDebugMenu,open,0) },
    { "Menu",                TOK_STRUCT(FileDebugMenu,menus,parseDebugMenu) },
    { "Command:",            TOK_STRUCT(FileDebugMenu,menus,parseDebugCommand) },
    { "AddCommand:",        TOK_STRUCT(FileDebugMenu,menus,parseDebugAddCommand) },
    { "End",                TOK_END,            0 },
    { "", 0, 0 }
};

static TokenizerParseInfo parseAllDebugMenus[] = {
    { "Menu",                TOK_STRUCT(FileDebugMenu,menus,    parseDebugMenu) },
    { "Command:",            TOK_STRUCT(FileDebugMenu,menus,    parseDebugCommand) },
    { "AddCommand:",        TOK_STRUCT(FileDebugMenu,menus,    parseDebugAddCommand) },
    { "", 0, 0 }
};

static void addDebugMenus(DebugMenuItem* parent, FileDebugMenu** menus);

DebugMenuItem* addDebugMenuItem(DebugMenuItem* parent, const char* displayText, const char* commandString, int open){
    DebugMenuItem* newItem;

    MP_CREATE(DebugMenuItem, 1000);
    newItem = MP_ALLOC(DebugMenuItem);

    Strncpyt(newItem->displayText, displayText);

    newItem->parent = parent;

    if(commandString){
        strcpy(newItem->commandString, commandString);
    }else{
        newItem->open = open;
    }
    
    addDebugMenuItemTail(parent, newItem);

    menuItemInitOpenState(newItem);
    
    return newItem;
} 

static void addDebugMenu(DebugMenuItem* parent, FileDebugMenu* menu){
    if(menu->name){
        DebugMenuItem* newItem = addDebugMenuItem(parent, menu->name, menu->command, menu->open);

        if(!menu->command){
            addDebugMenus(newItem, menu->menus);
        }
    }
    else if(menu->command && parent->subItems.tail){
        DebugMenuItem* prevItem = parent->subItems.tail;
        strcat(prevItem->commandString, "\n");
        strcat(prevItem->commandString, menu->command);
    }
}

static void addDebugMenus(DebugMenuItem* parent, FileDebugMenu** menus){
    int count = eaSize(&menus);
    int i;

    for(i = 0; i < count; i++){
        FileDebugMenu* menu = menus[i];

        addDebugMenu(parent, menu);
    }
}

void entDebugLoadFileMenus(int forced)
{
    if(forced || !debug_state.loadedFileMenus)
    {
        debug_state.loadedFileMenus = 1;
        memset(&debug_state.allFileMenus, 0, sizeof(debug_state.allFileMenus));
        ParserLoadFiles(NULL, "c:/entdebugmenu.txt", NULL, 0, parseAllDebugMenus, &debug_state.allFileMenus, NULL, NULL, NULL);
    }
}

static int getTotalOpenItemCount(DebugMenuItem* item)
{
    int total = 1;

    if(item->open)
    {
        for(item = item->subItems.head; item; item = item->nextSibling)
            total += getTotalOpenItemCount(item);
    }

    return total;
}

void entDebugInitMenu(void)
{
    debug_state.cursel = min(debug_state.cursel, getTotalOpenItemCount(debug_state.menuItem) - 1);
    debug_state.firstLine = min(debug_state.firstLine, getTotalOpenItemCount(debug_state.menuItem) - 1);
    debug_state.closingTime = 0;
    debug_state.mouseMustMove = 1;

    debug_state.menuItem->open = 1;
    bindKeyProfile(&ent_debug_binds_profile);
    debug_state.openingTime = 1;
    debug_state.lastTime = timeGetTime();
}

void entDebugReceiveCommands(Packet* pak)
{
    U32 startSize;
    char* motd;

    initEntDebug();

    MP_CREATE(DebugMenuItem, 1);

    if(debug_state.menuItem && !debug_state.closingTime){
        return;
    }
    
    freeDebugMenuItem(debug_state.menuItem);

    startSize = pak->stream.cursor.byte * 8 + pak->stream.cursor.bit;
    debug_state.menuItem = receiveDebugMenuItem(pak, NULL, NULL, NULL);
    debug_state.menuItem->bitsUsed = pak->stream.cursor.byte * 8 + pak->stream.cursor.bit - startSize;

    motd = pktGetString(pak);

    estrPrintCharString(&debug_state.motdText, motd);

    if(debug_state.menuItem)
    {
        DebugMenuItem* localMenu;
        
        entDebugLoadFileMenus(0);
        localMenu = addDebugMenuItem(debug_state.menuItem, "Local", NULL, 0);
        addDebugMenuItem(localMenu, "Reload ^2c:\\entdebugmenu.txt", "reloadentdebugfilemenus", 0);
        addDebugMenus(localMenu, debug_state.allFileMenus.menus);

        cmdParse("-mouse_look");
        entDebugInitMenu();
    }
}

int entDebugCmdParse(char *str, int x, int y)
{
    Cmd            *cmd;
    CmdContext    output = {0};

    if(debug_state.closingTime || debug_state.openingTime)
        return 1;
    
    output.found_cmd = false;
    output.access_level = cmdAccessLevel();
    cmd = cmdOldRead(&ent_debug_cmdlist,str,&output);

    if (output.msg[0])
    {
        if (strncmp(output.msg,"Unknown",7)==0)
            return 0;

        conPrintf("%s",output.msg);
        return 1;
    }

    if (!cmd)
        return 0;

    switch(cmd->num)
    {
        case CMD_DEBUGMENU_UP:
        case CMD_DEBUGMENU_PAGEUP:
        {
            if(debug_state.upHeld || debug_state.pageupHeld)
            {
                int sel;

                debug_state.keyDelayMsecStart = timeGetTime();
                debug_state.keyDelayMsecWait = 200;

                debug_state.cursel -= cmd->num == CMD_DEBUGMENU_UP ? 1 : 20;

                if(debug_state.cursel < 0)
                    debug_state.cursel = 0;
                
                sel = debug_state.cursel;

                while(debug_state.cursel > 0 && !getCurMenuItem(debug_state.menuItem, &sel))
                {
                    debug_state.cursel--;
                    sel = debug_state.cursel;
                }

                debug_state.mouseMustMove = 1;
            }

            break;
        }

        case CMD_DEBUGMENU_DOWN:
        case CMD_DEBUGMENU_PAGEDOWN:
        {
            if(debug_state.downHeld || debug_state.pagedownHeld)
            {
                int sel;

                debug_state.cursel += cmd->num == CMD_DEBUGMENU_DOWN ? 1 : 20;

                debug_state.keyDelayMsecStart = timeGetTime();
                debug_state.keyDelayMsecWait = 200;

                sel = debug_state.cursel;

                while(debug_state.cursel > 0 && !getCurMenuItem(debug_state.menuItem, &sel))
                {
                    debug_state.cursel--;
                    sel = debug_state.cursel;
                }

                debug_state.mouseMustMove = 1;
            }

            break;
        }

        case CMD_DEBUGMENU_SELECT:
        {
            int sel;
            DebugMenuItem* item;
            int w, h;

            windowSize(&w, &h);

            if(debug_state.fromMouse)
            {
                if(debug_state.mouseOverUI)
                    break;
                else
                {
                    int mouse_x, mouse_y;
                    int maxLines = (h - 20) / 16;
                    int totalLines = getTotalOpenItemCount(debug_state.menuItem);
                    int mouseLine;
                    int x_off = 0;

                    inpLastMousePos(&mouse_x, &mouse_y);

                    if(mouse_x > 500 && mouse_y > 0)
                    {
                        unbindKeyProfile(&ent_debug_binds_profile);
                        debug_state.closingTime = 150;
                        debug_state.lastTime = timeGetTime();
                        break;
                    }

                    if(mouse_y < 0 || mouse_y >= h)
                        break;
                                        
                    if (mouse_x < 0)
                        break; // Don't detect clicks on the other monitor to a different app!

                    mouseLine = debug_state.firstLine + (mouse_y - 22) / 16;

                    if(mouseLine < 0 || mouseLine >= totalLines)
                        break;
                    else
                        debug_state.cursel = mouseLine;
                }
            }

            sel = debug_state.cursel;
            item = getCurMenuItem(debug_state.menuItem, &sel);

            if(item)
            {
                if(item->subItems.head || !item->commandString[0])
                {
                    menuItemSetOpen(item, !item->open);
                    item->flash = 1;
                    item->flashStartTime = timeGetTime();
                }
                else
                {
                    unbindKeyProfile(&ent_debug_binds_profile);
                    parseDebugCommands(item->commandString);
                    bindKeyProfile(&ent_debug_binds_profile);

                    item->flash = 1;
                    item->flashStartTime = timeGetTime();

                    if(debug_state.closeMenu)
                    {
                        unbindKeyProfile(&ent_debug_binds_profile);
                        debug_state.closingTime = 1;
                        debug_state.lastTime = timeGetTime();
                    }
                    else
                        item->usedCount++;
                }
            }

            break;
        }

        case CMD_DEBUGMENU_OPEN:
        {
            int sel = debug_state.cursel;
            DebugMenuItem* item = getCurMenuItem(debug_state.menuItem, &sel);

            if(item)
            {
                if(item->subItems.head)
                {
                    if(item->open)
                        debug_state.cursel++;
                    else
                    {
                        menuItemSetOpen(item, 1);
                        item->flash = 1;
                        item->flashStartTime = timeGetTime();
                    }
                }

                debug_state.mouseMustMove = 1;
            }

            break;
        }

        case CMD_DEBUGMENU_CLOSE:
        {
            int sel = debug_state.cursel;
            DebugMenuItem* item = getCurMenuItem(debug_state.menuItem, &sel);

            if(item)
            {
                if(item->open)
                {
                    menuItemSetOpen(item, 0);
                    item->flash = 1;
                    item->flashStartTime = timeGetTime();
                }
                else if(item->parent)
                {
                    DebugMenuItem* cur;
                    int total = 0;

                    for(cur = item->parent->subItems.head; cur; cur = cur->nextSibling)
                    {
                        if(cur == item)
                        {
                            debug_state.cursel -= total + 1;
                            break;
                        }
                        else
                            total += getTotalOpenItemCount(cur);
                    }
                }

                debug_state.mouseMustMove = 1;
            }

            break;
        }

        case CMD_DEBUGMENU_CLOSEPARENT:
        {
            int sel = debug_state.cursel;
            int mouse_x, mouse_y;
            DebugMenuItem* item = getCurMenuItem(debug_state.menuItem, &sel);

            inpLastMousePos(&mouse_x, &mouse_y);

            if(mouse_x >= 0 && mouse_x < 500)
            {
                if(item && (item->open || item->parent) )
                {
                    if(!item->open)
                    {
                        sel = 1;
                        while(1)
                        {
                            DebugMenuItem* findItem;
                            int index = sel;

                            findItem = getCurMenuItem(item->parent, &index);

                            if(findItem == item)
                                break;
                            sel++;
                        }
                        debug_state.cursel -= sel;
                        item = item->parent;
                    }

                    menuItemSetOpen(item, 0);
                    item->flash = 1;
                    item->flashStartTime = timeGetTime();

                    debug_state.mouseMustMove = 1;
                    debug_state.moveMouseToSel = debug_state.cursel;
                }
            }
            break;
        }

        case CMD_DEBUGMENU_EXIT:
        {
            if(debug_state.menuItem)
            {
                unbindKeyProfile(&ent_debug_binds_profile);
                debug_state.closingTime = 150;
                debug_state.lastTime = timeGetTime();
            }
            break;
        }

        case CMD_DEBUGMENU_COPY:
        {
            int sel = debug_state.cursel;
            DebugMenuItem* item = getCurMenuItem(debug_state.menuItem, &sel);

            if( item && item->commandString && *item->commandString )
                winCopyToClipboard(item->commandString);
            break;
        }

        case CMD_DEBUGMENU_BIND:
        {
            debug_state.waitingForBindKey = 1;
            break;
        }
        case CMD_DEBUGMENU_MACRO:
        {
            int sel = debug_state.cursel;
            DebugMenuItem* item = getCurMenuItem(debug_state.menuItem, &sel);

            if( item && item->commandString && *item->commandString ) 
            {
                unbindKeyProfile(&ent_debug_binds_profile);
                cmdParsef( "macro \"%s\" \"%s\"", item->displayText, item->commandString );
                bindKeyProfile(&ent_debug_binds_profile);
            }
            break;
        }
        case CMD_DEBUGMENU_CUSTOMWINDOW:
            {
                int sel = debug_state.cursel;
                DebugMenuItem* item = getCurMenuItem(debug_state.menuItem, &sel);

                if( item && item->commandString && *item->commandString && isDevelopmentMode() ) 
                    addCustomWindowButtonToAll(item->displayText, item->commandString);
                break;
            }
    }
    return 1;
}

void displayEntDebugInfoTextBegin()
{
    rdrSetup2DRendering();
}

void displayEntDebugInfoTextEnd()
{
    rdrFinish2DRendering();
}


static TTDrawContext* getDebugFont(){
    static TTDrawContext* debugFont = NULL;

    if(!debugFont){
        TTCompositeFont* font = createTTCompositeFont();

        debugFont = createTTDrawContext();
        initTTDrawContext(debugFont, 64);
        ttFontAddFallback(font, ttFontLoadCached("tahomabd.ttf", 1));
        ttFontAddFallback(font, ttFontLoadCached("mingliu.ttc", 1));
        ttFontAddFallback(font, ttFontLoadCached("gulim.ttc", 1));
        ttFMAddFont(fontManager, font);
        debugFont->font = font;
        debugFont->dynamic = 1;
        debugFont->renderParams.renderSize = 11;
        debugFont->renderParams.outline = 1;
        debugFont->renderParams.outlineWidth = 1;
    }

    return debugFont;
}

void printDebugString( char* outputString, int x, int y, float scale, int lineHeight, int overridecolor, int defaultcolor, U8 alpha, int* getWidthHeightBuffer)
{
    TTDrawContext* debugFont = getDebugFont();
    unsigned int curColor[4];
    unsigned int curColorARGB;
    int tabWidth = max(350 * scale, 1);
    char* cur = outputString;
    int i;
    char* str;
    char delim;
    int colorCount;
    int doDraw = getWidthHeightBuffer ? 0 : 1;
    char outputStringCopy[20000];
    float fontScale = 1;

    unsigned int colors[] = {
        0xeef8ff00,        // ^0 WHITE
        0xff99aa00,        // ^1 PINK
        0x88ff9900,        // ^2 GREEN     
        0xeebbbb00,        // ^3 LIGHT PINK      
        0xf9ab7700,        // ^4 ORANGE    
        0x43b6fa00,     // ^5 BLUE    
        0xff945200,        // ^6 DARK ORANGE     
        0xffff3200,        // ^7 YELLOW     
        0x68ffff00,        // ^8 LIGHT BLUE     
        0xaa99ff00,        // ^9 PURPLE    
        0xffde8c00,        
    };
    
    if(!outputString)
        return;

    if(strlen(outputString) > ARRAY_SIZE(outputStringCopy) - 1)
        cur = outputString;
    else
    {
        strcpy(outputStringCopy, outputString);
        cur = outputStringCopy;
    }

    if(tabWidth <= 0)
        tabWidth = 1;

    colorCount = ARRAY_SIZE(colors);
    overridecolor = max(-1, min(overridecolor, colorCount - 1));
    defaultcolor = max(0, min(defaultcolor, colorCount - 1));

    if(getWidthHeightBuffer)
    {
        getWidthHeightBuffer[0] = 0;
        getWidthHeightBuffer[1] = 0;
    }

    for(i = 0, str = strsep2(&cur, "\n", &delim); str; str = strsep2(&cur, "\n", &delim), i++)
    {
        char* str2;
        char delim2;
        int xofs = 0;
        unsigned int highlightColorARGB = 0;

        {
            int i;
            curColorARGB = colors[overridecolor >= 0 ? overridecolor : defaultcolor] | alpha;
            for(i = 0; i < 4; i++)
                curColor[i] = curColorARGB;
            curColorARGB = (curColorARGB >> 8) | (curColorARGB << 24);
        }

        for(str2 = strsep2(&str, "^", &delim2); str2; str2 = strsep2(&str, "^", &delim2))
        {
            char* str3;
            char delim3;

            for(str3 = strsep2(&str2, "\t", &delim3); str3; str3 = strsep2(&str2, "\t", &delim3))
            {
                wchar_t buffer[1000];
                int width = 0;
                int length;

                length = UTF8ToWideStrConvert(str3, buffer, ARRAY_SIZE(buffer)-1);
                buffer[length] = '\0';
                ttGetStringDimensionsWithScaling(debugFont, scale * fontScale, scale * fontScale, buffer, length, &width, NULL, NULL, true);

                if(doDraw)
                {
                    if(highlightColorARGB)
                        drawFilledBox( x + xofs, y - i * lineHeight * scale - 2, x + xofs + width, y - (i-1) * lineHeight * scale - 2, highlightColorARGB);
                    ttDrawText2DWithScaling( debugFont, x + xofs, y - i * lineHeight * scale, -1, scale * fontScale, scale * fontScale, curColor, buffer, wcslen(buffer));
                }

                xofs += width;

                if(delim3)
                {
                    int old_xofs = xofs;

                    str2[-1] = delim3;
                    xofs = ((xofs + tabWidth) / tabWidth) * tabWidth;

                    if(doDraw && highlightColorARGB)
                        drawFilledBox( x + old_xofs, y - i * lineHeight * scale - 2, x + xofs, y - (i-1) * lineHeight * scale - 2, highlightColorARGB);
                }
            }

            if(delim2)
            {
                int code = tolower(*str);

                str[-1] = delim2;

                if(code >= '0' && code <= '9')
                {
                    int idx = *str - '0';
                    if(overridecolor < 0)
                    {
                        int i;
                        curColorARGB = colors[idx] | alpha;
                        for(i = 0; i < 4; i++)
                            curColor[i] = curColorARGB;
                        curColorARGB = (curColorARGB >> 8) | (curColorARGB << 24);
                    }

                    str++;
                }
                else if(code == '#')
                {
                    // RGB coloring in the form ^#(r,g,b)
                    // So, the string ^#(255,0,0) will change the text to red
                    int r, g, b, num;
                    num = sscanf(str+1, "(%d,%d,%d)", &r, &g, &b);
                    if (num == 3) {
                        if(overridecolor < 0)
                        {
                            int i;
                            curColorARGB = (b<<8) | (g<<16) | (r<<24) | alpha;
                            for(i = 0; i < 4; i++)
                                curColor[i] = curColorARGB;
                            curColorARGB = (curColorARGB >> 8) | (curColorARGB << 24);
                        }
                        str=strchr(str, ')');
                        assert(str);
                        str++;
                    }
                }
                else if(code == 'd')
                {
                    if(overridecolor < 0)
                    {
                        int i;
                        curColorARGB = colors[overridecolor >= 0 ? overridecolor : defaultcolor] | alpha;
                        for(i = 0; i < 4; i++)
                            curColor[i] = curColorARGB;
                        curColorARGB = (curColorARGB >> 8) | (curColorARGB << 24);
                    }

                    str++;
                }
                else if(code == 'h')
                {
                    highlightColorARGB = (((curColorARGB >> 24) / 2) << 24) | (curColorARGB & 0xffffff);
                    str++;
                }
                else if(code == 't')
                {
                    char buffer[100];
                    char* curpos = buffer;
                    for(str++; *str && tolower(*str) != 't' && curpos - buffer < ARRAY_SIZE(buffer) - 1; str++)
                        *curpos++ = *str;
                    *curpos = '\0';
                    if(code == 't')
                        str++;

                    tabWidth = atoi(buffer) * scale;

                    if(tabWidth <= 0)
                        tabWidth = 1;
                }
                else if(code == 'r')
                {
                    if(overridecolor < 0)
                    {
                        int i;
                        curColorARGB =    0x000000ff |
                                        (0xff - (((global_state.global_frame_count + 0) * 10) & 0x7f)) << 24 |
                                        (0x80 | (((global_state.global_frame_count + 0) * 10) & 0x7f)) << 16;
                        for(i = 0; i < 4; i++)
                            curColor[i] = curColorARGB;
                        curColorARGB = (curColorARGB >> 8) | (curColorARGB << 24);
                    }

                    str++;
                }
                else if(code == 'b')
                {
                    int dx, dx2;
                    char buffer[100];
                    char buffer2[100] = "";
                    char* curpos = buffer;
                    for(str++; *str && tolower(*str) != 'b' && tolower(*str) != '/' && curpos - buffer < ARRAY_SIZE(buffer) - 1; str++)
                        *curpos++ = *str;
                    *curpos = '\0';
                    if(tolower(*str) == '/')
                    {
                        curpos = buffer2;
                        for(str++; *str && tolower(*str) != 'b' && curpos - buffer2 < ARRAY_SIZE(buffer2) - 1; str++)
                            *curpos++ = *str;
                        *curpos = '\0';
                    }

                    if(tolower(*str) == 'b')
                        str++;

                    dx = atoi(buffer) * scale;

                    if(dx < 0)
                        dx = 0;

                    if(atoi(buffer2) > 0)
                    {
                        dx2 = atoi(buffer2) * scale;
                        if(dx > dx2)
                            dx = dx2;
                    }
                    else
                        dx2 = dx;

                    if(doDraw)
                    {
                        if(highlightColorARGB)
                            drawFilledBox(    x + xofs, y - i * lineHeight * scale - 2, x + xofs + dx2 + 4, y - (i-1) * lineHeight * scale - 2, highlightColorARGB);

                        drawFilledBox(    x + xofs, y - i * lineHeight * scale - 1, x + xofs + dx2 + 4, y - (i - 1) * lineHeight * scale - 4, (alpha << 24) | 0xcccccc);
                        drawFilledBox( x + xofs + 1, y - i * lineHeight * scale, x + xofs + dx2 + 3, y - (i - 1) * lineHeight * scale - 5, (alpha << 24) | 0x000000);
                        drawFilledBox( x + xofs + 2, y - i * lineHeight * scale + 1, x + xofs + dx + 2, y - (i - 1) * lineHeight * scale - 6, (curColorARGB & 0x00ffffff) | ((alpha / 2) << 24));

                        if(dx > 2)
                            drawFilledBox( x + xofs + 3, y - i * lineHeight * scale + 2, x + xofs + dx + 1, y - (i - 1) * lineHeight * scale - 7, curColorARGB);

                        if(dx < dx2)
                            drawFilledBox( x + xofs + dx + 2, y - i * lineHeight * scale + 1, x + xofs + dx2 + 2, y - (i - 1) * lineHeight * scale - 6, (alpha << 24) | 0x334433);
                    }

                    xofs += dx2 + 4;
                }
                else if(code == 'l')
                {
                    int dx;
                    char buffer[100];
                    char* curpos = buffer;
                    for(str++; *str && tolower(*str) != 'l' && curpos - buffer < ARRAY_SIZE(buffer) - 1; str++)
                        *curpos++ = *str;
                    *curpos = '\0';

                    if(tolower(*str) == 'l')
                        str++;

                    dx = atoi(buffer) * scale;

                    if(doDraw)
                    {
                        drawFilledBox( x + xofs, y - i * lineHeight * scale + scale * (lineHeight / 2 - 3) - 1, x + xofs + dx + 2, y - i * lineHeight * scale + scale * (lineHeight / 2 - 2) + 1, curColorARGB & 0xff000000);
                        drawFilledBox( x + xofs + 1, y - i * lineHeight * scale + scale * (lineHeight / 2 - 3), x + xofs + dx + 1, y - i * lineHeight * scale + scale * (lineHeight / 2 - 2), curColorARGB);
                    }

                    xofs += dx + 2;
                }
                else if(code == 's')
                {
                    fontScale = 0.7;
                    str++;
                }
                else if(code == 'n')
                {
                    fontScale = 1;
                    str++;
                }
            }
        }

        if(getWidthHeightBuffer)
        {
            if(xofs > getWidthHeightBuffer[0])
                getWidthHeightBuffer[0] = xofs;
            getWidthHeightBuffer[1] += lineHeight * scale;
        }

        if(delim)
            cur[-1] = delim;
    }
}

void printDebugStringAlign(char* outputString, int x, int y, int dx, int dy, int align, float scale, int lineHeight, int overridecolor, int defaultcolor, U8 alpha)
{
    int widthheight[2];
    printDebugString(outputString, x, y, scale, lineHeight, overridecolor, defaultcolor, alpha, widthheight);
    if (ALIGN_HCENTER & align)
    {
        x = x + (dx - widthheight[0]) / 2;
    }
    else if (ALIGN_RIGHT & align)
    {
        x = x + dx - widthheight[0];
    }

    if (ALIGN_VCENTER & align)
    {
        y = y + (dy - widthheight[1]) / 2;
    }
    else if (ALIGN_TOP & align)
    {
        y = y + dy - widthheight[1];
    }
    printDebugString(outputString, x, y, scale, lineHeight, overridecolor, defaultcolor, alpha, 0);
}

static int mouseOverLastButtonState;

int mouseOverLastButton(){
    return mouseOverLastButtonState;
}

int drawButton2D(int x1, int y1, int dx, int dy, int centered, char* text, float scale, char* command, int* setMe){
    int w, h;
    int mouse_x, mouse_y;
    int over = 0;
    int x2 = x1 + dx;
    int y2 = y1 + dy;
    int widthHeight[2];
    
    mouseOverLastButtonState = 0;

    windowSize(&w,&h);

    if(x1 > w || x2 < 0 || y1 > h || y2 < 0)
        return 0;

    inpMousePos(&mouse_x, &mouse_y);

    mouse_y = h - mouse_y;

    if(!inpIsMouseLocked() && (mouse_x >= x1 && mouse_x < x2 && mouse_y >= y1 && mouse_y < y2)){
        over = 1;
        debug_state.mouseOverUI = 1;
        mouseOverLastButtonState = 1;
    }

    drawFilledBox(x1, y1, x2, y2, (0xd0 << 24) | (over ? 0x7f + abs(0x80 - ((global_state.global_frame_count * 10) & 0xff)) / 2 : 0x223322));
    drawFilledBox(x1, y1, x1 + 1, y2 + 1, 0xff888888);
    drawFilledBox(x1, y1, x2 + 1, y1 + 1, 0xff888888);
    drawFilledBox(x2, y1, x2 + 1, y2 + 1, 0xff888888);
    drawFilledBox(x1, y2, x2 + 1, y2 + 1, 0xff888888);

    if(over){
        drawFilledBox(x1 + 1, y1 + 1, x1 + 2, y2,    0xffffff00);
        drawFilledBox(x1 + 1, y1 + 1, x2, y1 + 2,    0xffffff00);
        drawFilledBox(x2 - 1, y1 + 1, x2, y2,        0xffffff00);
        drawFilledBox(x1 + 1, y2 - 1, x2, y2,        0xffffff00);
    }

    if(centered){
        printDebugString(text, 0, 0, scale, 11, -1, -1, 255, widthHeight);
        
        printDebugString(    text,
                            x1 + (x2 - x1 - widthHeight[0]) / 2,
                            y1 + (y2 - y1 - widthHeight[1]) / 2 + widthHeight[1] - scale * 9,
                            scale,
                            11,
                            over ? 7 : -1,
                            -1,
                            255,
                            NULL);
    }else{
        printDebugString(text, 0, 0, scale, 7, -1, -1, 255, widthHeight);

        printDebugString(    text,
                            x1 + 3,
                            y1 + (y2 - y1 - widthHeight[1]) / 2,
                            scale,
                            11,
                            over ? 7 : -1,
                            -1,
                            255,
                            NULL);
    }

    if(over && !inpIsMouseLocked()){
        if(inpEdge(INP_LBUTTON)){
            eatEdge(INP_LBUTTON);

            if(command)
                parseDebugCommands(command);

            if(setMe)
                *setMe = 1;

            return 1;
        }
    }

    return 0;
}

static int drawDefaultButton2D(int x1, int y1, int dx, int dy, char* text){
    return drawButton2D(x1, y1, dx, dy, 1, text, 1, NULL, NULL);
}

static void displayEntDebugInfoText()
{
    static int hideText;
    
    int w, h, i, j;
    int mouse_x, mouse_y;
    Entity* player = playerPtr();
    int showButton = 0;

    if( !isMenu(MENU_GAME) ){
        return;
    }

    if(!game_state.hasEntDebugInfo && !game_state.ent_debug_client){
        return;
    }

    windowSize(&w,&h);

    if(!w || !h)
        return;

    {
        int y = 100;
        
        windowSize(&w, &h);

        if(control_state.server_state->controls_input_ignored){
            printDebugString( "^1server says controls are ignored", w / 2, h - (y += 15), 1, 0, -1, -1, 255, NULL);
        }

        if(control_state.server_state->shell_disabled){
            printDebugString( "^1server says shell is disabled", w / 2, h - (y += 15), 1, 0, -1, -1, 255, NULL);
        }

        if(control_state.controls_input_ignored_this_csc){
            printDebugString( "^1client says controls are ignored", w / 2, h - (y += 15), 1, 0, -1, -1, 255, NULL);
        }
    }
    
    inpMousePos(&mouse_x, &mouse_y);

    for(j = 0, i = (debug_state.curText + j) % ARRAY_SIZE(debug_state.text);
        j < debug_state.textCount;
        j++, i = (debug_state.curText + j) % ARRAY_SIZE(debug_state.text))
    {
        Vec3 pos;
        Vec2 screenPos;
        EntDebugText* text = debug_state.text + j;
        float distSquared = distance3Squared(cam_info.cammat[3], text->pos);

        if(distSquared < SQR(200)){
            Vec3 toPoint;

            subVec3(text->pos, cam_info.cammat[3], toPoint);

            if(dotVec3(toPoint, cam_info.cammat[2]) < 0){
                mulVecMat4(text->pos, cam_info.viewmat, pos);

                gfxWindowScreenPos(pos, screenPos);

                //drawFilledBox(screenPos[0], screenPos[1], screenPos[0] + 100, screenPos[1] + 100, 0x000000);
                printDebugString(    text->text,
                                    screenPos[0],
                                    screenPos[1],
                                    1.5f - sqrt(distSquared) / 200.0f,
                                    11,
                                    -1,
                                    0,
                                    255,
                                    NULL);
            }
        }
    }

    for(i = 1; i < entities_max; i++){
        if(entity_state[i] & ENTITY_IN_USE){
            Entity* e = entities[i];
            ClientEntityDebugInfo* info = e->debugInfo;
            EntType entType = ENTTYPE_BY_ID(i);

            if(!info)
                continue;

            if(    e == current_target ||
                e == player)
            {
                Vec3    steady_top;
                Vec2    screenTop;
                Vec3    screenUL, screenLR;
                char*    outputString = info->text;
                char    outputTemp[1000];
                char*    cur;
                int        lineHeight = 13;

                if(!e->seq)
                    continue;

                seqGetSides( e->seq, ENTMAT(e),  screenUL, screenLR, NULL, NULL );
                seqGetSteadyTop( e->seq, steady_top );

                gfxWindowScreenPos( steady_top, screenTop );

                if(game_state.ent_debug_client & 1 && (!outputString || !outputString[0]) && entType == ENTTYPE_CRITTER){
                    STR_COMBINE_BEGIN(outputTemp);
                    STR_COMBINE_CAT("ID: ^4");
                    STR_COMBINE_CAT_D(e->svr_idx);
                    STR_COMBINE_END();
                    outputString = outputTemp;
                }

                 if(outputString && outputString[0] && (entType == ENTTYPE_CRITTER || steady_top[2] < 0)){
                    int        j, xp, yp;
                    Vec3    diff;
                    float    dist;
                    float    scale;
                    int        widthHeight[2];

                    subVec3(cam_info.mat[3], ENTPOS(e), diff);

                    dist = 1 + lengthVec3(diff) / 50;

                    scale = 1;

                    if(scale > 2){
                        scale = 2;
                    }

                    for(j = 0, cur = outputString; *cur; cur++) {
                        if(*cur == '\n')
                            j++;
                    }
                    j++;

                    if(entType == ENTTYPE_PLAYER && e == player){
                        xp = screenTop[0] + 100;
                        yp = ( h - screenTop[1] ) + 30;
                        scale = 1;

                        if(yp + lineHeight * j > h){
                            yp = h - lineHeight * j;
                        }

                        if(yp < lineHeight){
                            if(lineHeight * j > h){
                                scale = (float)h / (float)(lineHeight * j);
                            }

                            yp = lineHeight * scale;
                        }
                    }else{
                        static float cur_scale = 0;
                        static float last_dest_scale = 0;
                        static int ent_id = 0;
                        float dest_scale = min(200 - mouse_x, 200 - mouse_y) / 200.0;
                        float max_scale = (float)h / (float)(lineHeight * j);
                        int alpha;
                        float diff;

                        if(debug_state.mouseOverUI || mouse_x < 0 || mouse_x >= w || mouse_y < 0 || mouse_y >= h)
                            dest_scale = 0;

                        max_scale = min(max_scale, 3);

                        if(e->owner != ent_id){
                            ent_id = e->owner;
                            cur_scale = 0;
                        }

                        if(!debug_state.menuItem && !inpIsMouseLocked()){
                            dest_scale = max(0, min(1, dest_scale));
                        }else{
                            dest_scale = last_dest_scale;
                        }

                        last_dest_scale = dest_scale;

                        diff = dest_scale - cur_scale;

                        if(diff < -0.3)
                            diff = -0.3;
                        else if(diff > 0.3)
                            diff = 0.3;

                        cur_scale += diff * 0.2 * min(TIMESTEP, 5);

                        alpha = 0.9 * cur_scale * 255;

                        alpha <<= 24;

                        if(!hideText && alpha){
                            drawFilledBox4(    0, 0, 200, h,
                                            alpha | 0x003066, alpha | 0x003066, alpha | 0x003077, alpha | 0x003077);
                            drawFilledBox4(    200, 0, w, h,
                                            alpha | 0x003066, 0x003066, alpha | 0x003077, 0x003077);
                        }

                        dist = 0;
                        xp = 3;
                        yp = 5;

                        scale = max(scale, max_scale * (cur_scale * 1.2));
                        scale = min(scale, 2);

                        if(yp + scale * lineHeight * j > h){
                            yp = h - scale * lineHeight * j;
                        }

                        if(yp < scale * lineHeight){
                            if(scale * lineHeight * j > h){
                                scale = (float)h / (float)(lineHeight * j);
                            }

                            yp = lineHeight * scale;
                        }
                    }

                    showButton = 1;
                    
                    if(!hideText){
                        printDebugString(outputString, 0, 0, scale, lineHeight, -1, 0, 255, widthHeight);

                        if(xp + widthHeight[0] > w){
                            xp = max(0, w - widthHeight[0]);
                        }

                        printDebugString(outputString, xp, h - yp, scale, lineHeight, -1, 0, 255, NULL);
                    }
                }

                for(j = 0; j < info->points.count; j++){
                    Vec3 pos;
                    Vec2 screenPos;
                    float distSquared = distance3Squared(cam_info.cammat[3], info->points.pos[j].pos);

                    if(distSquared < SQR(200)){
                        Vec3 toPoint;

                        subVec3(info->points.pos[j].pos, cam_info.cammat[3], toPoint);

                        if(dotVec3(toPoint, cam_info.cammat[2]) < 0){
                            mulVecMat4(info->points.pos[j].pos, cam_info.viewmat, pos);

                            gfxWindowScreenPos(pos, screenPos);

                            //drawFilledBox(screenPos[0], screenPos[1], screenPos[0] + 100, screenPos[1] + 100, 0x000000);
                            printDebugString(    info->points.pos[j].tag,
                                                screenPos[0],
                                                screenPos[1],
                                                2.0f - sqrt(distSquared) / 100.0f,
                                                11,
                                                -1,
                                                0,
                                                255,
                                                NULL);
                        }
                    }
                }
            }

            if(0){
                Vec3 pos;
                Vec2 screenPos;
                float distSquared = distance3Squared(cam_info.cammat[3], e->net_pos);
                char buffer[1000];

                if(distSquared < SQR(100)){
                    Vec3 toPoint;

                    subVec3(e->net_pos, cam_info.cammat[3], toPoint);

                    if(dotVec3(toPoint, cam_info.cammat[2]) < 0){
                        mulVecMat4(e->net_pos, cam_info.viewmat, pos);

                        gfxWindowScreenPos(pos, screenPos);

                        sprintf(buffer, "Timestep:\n%1.2f", e->timestep);

                        printDebugString(buffer, screenPos[0], screenPos[1], 2.0f - sqrt(distSquared) / 50.0f, 11, -1, 0, 255, NULL);
                    }
                }
            }
        }
    }
    
    if(showButton){
        if(drawButton2D(5, h - 30 + max(0, sqrt(SQR(mouse_x - 50) + SQR(mouse_y - 30)) - 100), 100, 25, 1, hideText ? "^2Show Text" : "^1Hide Text", 1, NULL, NULL)){
            hideText ^= 1;
        }
    }
}

static char* entDebugGetCommaSeparatedInt(__int64 x){
    static int curBuffer = 0;
    static char bufferArray[10][50];
    char* buffer = bufferArray[curBuffer = (curBuffer + 1) % 10];
    int j = 0;
    
    buffer += ARRAY_SIZE(bufferArray[0]) - 1;
    
    *buffer-- = 0;

    do{
        *buffer-- = '0' + (x % 10);

        x = x / 10;

        if(x && ++j == 3){
            j = 0;
            *buffer-- = ',';
        }
    }while(x);

    return buffer + 1;
}

static char* getCommaSeparatedFloat(float x, int decimalPlaces)
{
    static int curBuffer = 0;
    static char bufferArray[10][50];
    char* buffer = bufferArray[curBuffer = (curBuffer + 1) % 10];
    char* bufferPtr = buffer + ARRAY_SIZE(bufferArray[0]) - 2 - decimalPlaces;
    char* bufferPtr2 = buffer + ARRAY_SIZE(bufferArray[0]) - 2 - decimalPlaces;
    int j = 0;
    int temp = x;

    *bufferPtr-- = 0;

    // building the whole numbers
    do
    {
        *bufferPtr-- = '0' + (temp % 10);

        temp = temp / 10;

        if (temp && ++j == 3)
        {
            j = 0;
            *bufferPtr-- = ',';
        }
    } 
    while (temp);

    // now building the decimal part, reusing above variables
    *bufferPtr2++ = '.';

    for (j = 0; j < decimalPlaces; j++)
    {
        x *= 10.0f;
        temp = x; // casting to int

        *bufferPtr2++ = '0' + (temp % 10);
    }

    return bufferPtr + 1;
}

static StashElement perfInfoGetElement(const char* source, PerformanceInfo* info, int create){
    char buffer[10000];
    PerformanceInfo* curInfo = info;
    StashElement element;

    if(!debug_state.perfInfoStateTable){
        debug_state.perfInfoStateTable = stashTableCreateWithStringKeys(100,  StashDeepCopyKeys | StashCaseSensitive );
    }

    STR_COMBINE_BEGIN(buffer);


    STR_COMBINE_CAT(source);
    STR_COMBINE_CAT(":");

    while(curInfo){
        if(info != curInfo){
            STR_COMBINE_CAT(":");
        }
        
        if((U32)curInfo->rootStatic){
            STR_COMBINE_CAT_D((U32)curInfo->rootStatic);
        }else{
            STR_COMBINE_CAT(curInfo->locName);
        }
        
        curInfo = curInfo->parent;         
    }
    
    STR_COMBINE_END();

    stashFindElement(debug_state.perfInfoStateTable, buffer, &element);

    if(!element && create){
        stashAddPointerAndGetElement(debug_state.perfInfoStateTable, buffer, 0, false, &element);
    }

    return element;
}

static void perfInfoSetState(const char* source, PerformanceInfo* info, int flag, int state){
    StashElement element = perfInfoGetElement(source, info, 1);

    if(element){
        int value = (int)stashElementGetPointer(element);

        if(state){
            value |= flag;
        }else{
            value &= ~flag;
        }

        stashElementSetPointer(element, (void*)value);
    }
}

static void perfInfoSetOpen(const char* source, PerformanceInfo* info, int set){
    info->open = set;

    perfInfoSetState(source, info, 1, set);
}

static void perfInfoSetHidden(const char* source, PerformanceInfo* info, int set){
    info->hidden = set;

    perfInfoSetState(source, info, 2, set);
}

static void perfInfoInitState(const char* source, PerformanceInfo* info){
    StashElement element = perfInfoGetElement(source, info, 0);

    if(element){
        info->open = ((int)stashElementGetPointer(element) & 1) ? 1 : 0;
        info->hidden = ((int)stashElementGetPointer(element) & 2) ? 1 : 0;
    }

    info->inited = 1;
}

static const char* getPercentColor(float percent){
    if(percent >= 20)
        return "^1";
    else if(percent >= 10)
        return "^4";
    else if(percent >= 5)
        return "^7";
    else if(percent >= 1)
        return "^2";
    else
        return "^5";
}

static const char* getPercentString(float percent){
    static char buffer[15];
    
    STR_COMBINE_BEGIN(buffer);
    if(percent < 1){
        STR_COMBINE_CAT("^s");
    }
    STR_COMBINE_CAT(getPercentColor(percent));
    STR_COMBINE_CAT(getPercentColor(percent));
    if(percent >= 100){
        STR_COMBINE_CAT_D(100);
    }
    else if(percent >= 10){
        STR_COMBINE_CAT_D((int)percent);
        STR_COMBINE_CAT(".");
        STR_COMBINE_CAT_D((int)(percent * 10) % 10);
    }
    else{
        STR_COMBINE_CAT_D((int)percent);
        STR_COMBINE_CAT(".");
        STR_COMBINE_CAT_D((int)(percent * 10) % 10);
        STR_COMBINE_CAT_D((int)(percent * 100) % 10);
    }
    if(percent < 1){
        STR_COMBINE_CAT("^n");
    }
    STR_COMBINE_END();

    return buffer;
}

static int perfDrawY = 15;
static int selectedPerfDrawY;
static int maxPerfDrawY;
static int minPerfDrawY;

static void displayPerformanceInfoHelper( PerformanceInfo* info, int* y, int level, __int64 totalTime, __int64 cpuSpeed, char* prefix)
{
    int j, k;
    char buffer[1000];
    int graph_x = level * 100;
    int mouse_x, mouse_y;
    int w, h;
    int index = info->uid;
    PerformanceInfo* child;
    int hiddenChild = 0;
    int draw_y = *y;
    int level_color = max(0, 0x50 - 16 * level);
    int boxColor = info->breakMe ? 0xffff00 : (level_color | level_color << 8 | level_color << 16);
    int alpha;
    int selected = 0;
    int size = ARRAY_SIZE(info->history);
    float scale = debug_state.perfInfoScaleF32;
    int lineCount = 0;
    int recentRuns = 0;
    __int64 recentCycles = 0;
    int recentCount = 0;
    
    if(cpuSpeed <= 0){
        cpuSpeed = 2000000000;
    }

    inpMousePos(&mouse_x, &mouse_y);
    windowSize(&w, &h);

    if(!info->inited)
        perfInfoInitState(prefix, info);
    
    if(info->hidden)
    {
        *y -= 15;
        return;
    }

    // Draw children first, in order to collect info about them.

    if(info->open)
    {
        for(child = info->child.head; child; child = child->nextSibling)
        {
            int oldy = *y;
            *y += 15;
            displayPerformanceInfoHelper(child, y, level + 1, totalTime, cpuSpeed, prefix);

            if(*y == oldy)
                hiddenChild = 1;
        }
    }
    
    if(draw_y > maxPerfDrawY)
        maxPerfDrawY = draw_y;
    
    
    if(draw_y < minPerfDrawY)
        minPerfDrawY = draw_y;
    

    if(draw_y < 15 || draw_y > h)
        return;
    

    // Draw self after children.
    if(!inpIsMouseLocked())
    {
        alpha = 355 - mouse_x;
        alpha = min(255, max(0, alpha));
    }
    else
        alpha = 0;

    if(!inpIsMouseLocked() && mouse_x >= 0 && mouse_x < PERF_INFO_WIDTH && mouse_y >= draw_y && mouse_y < draw_y + 15)
    {
        if(!inpIsMouseLocked())
        {
            debug_state.mouseOverUI = 1;
            selected = 1;
            selectedPerfDrawY = draw_y;
            alpha = max(alpha, 192);
            boxColor =    (info->breakMe ? 0xffff00 : 0x000000) |
                        (127 + abs(128 - ((global_state.global_frame_count) % 257))) << 16 |
                        (abs(255 - (((global_state.global_frame_count * 2) + 255)) % 510) / 2);
        }

        if(inpEdge(INP_LBUTTON))
        {
            eatEdge(INP_LBUTTON);
            if(mouse_x >= 0 && mouse_x < 12)
            {
                if(debug_state.perfInfoBreaks)
                {
                    if(!stricmp(prefix, "s"))
                    {
                        char buffer[100];
                        info->breakMe ^= 1;
                        sprintf(buffer, "perfinfosetbreak_server %d %d %d", debug_state.serverTimersLayoutID, info->uid, info->breakMe);
                        cmdParse(buffer);
                    }
                    else if(!stricmp(prefix, "c"))
                        info->breakMe ^= 1;
                }
                else
                    perfInfoSetHidden(prefix, info, 1);
            }
            else if(hiddenChild && mouse_x >= 14 && mouse_x < 26)
            {
                for(child = info->child.head; child; child = child->nextSibling)
                    perfInfoSetHidden(prefix, child, 0);
            }
            else if(info->child.head)
                perfInfoSetOpen(prefix, info, !info->open);
        }
    }

    if(selected)
    {
        if(mouse_x >= 0 && mouse_x < 252)
        {
            debug_state.perfInfoZoomed = info;
            debug_state.perfInfoZoomedAlpha = max(0, min(255, 255 - mouse_x));
        }
    }
     
     if(mouse_x >= 252 && mouse_x < 252 + ARRAY_SIZE(info->history))
    {
         alpha = 100;
         
         if(selected)
        {
            debug_state.perfInfoZoomed = info;
            debug_state.perfInfoZoomedHilight = ARRAY_SIZE(info->history) - (mouse_x - 252);
            debug_state.perfInfoZoomedAlpha = 128;
        }
    }

    alpha <<= 24;

    if(alpha)
    {
        int mouse_over = selected ? mouse_x >= 0 ? mouse_x < 12 ? 1 : mouse_x < 26 ? 2 : 0 : 0 : 0;

        if(mouse_over)
            debug_state.mouseOverUI = 1;
        
        drawFilledBox4(    0, h - 15 - draw_y, PERF_INFO_WIDTH, h - draw_y, 0x111111 | alpha, 0x111111 | alpha, boxColor | alpha, boxColor | alpha);
        drawFilledBox4(    1, h - 14 - draw_y, 12, h - draw_y - 1, alpha | 0xcccccc, alpha | 0xdddddd, alpha | 0xaaaaaa, alpha | 0xbbbbbb);
        drawFilledBox4(    2, h - 13 - draw_y, 11, h - draw_y - 2, 
            alpha | (mouse_over == 1 ? rand() : hiddenChild ? 0x880000 : 0x009900),
            alpha | (mouse_over == 1 ? rand() : hiddenChild ? 0x770000 : 0x00aa00), 
            alpha | (mouse_over == 1 ? rand() : hiddenChild ? 0x440000 : 0x007700), 
            alpha | (mouse_over == 1 ? rand() : hiddenChild ? 0x990000 : 0x008800));

        if(selected && hiddenChild)
        {
            drawFilledBox4(    14, h - 14 - draw_y, 25, h - draw_y - 1, alpha | 0xcccccc, alpha | 0xdddddd, alpha | 0xaaaaaa, alpha | 0xbbbbbb);
            drawFilledBox4(    15, h - 13 - draw_y, 24, h - draw_y - 2, 
                alpha | (mouse_over == 2 ? rand() : 0x009900), 
                alpha | (mouse_over == 2 ? rand() : 0x00aa00), 
                alpha | (mouse_over == 2 ? rand() : 0x007700), 
                alpha | (mouse_over == 2 ? rand() : 0x008800));
        }
    }

    for (j = 0; j < size; j++)
    {
        if (!info->history[j].cycles)
            break;

        recentRuns += info->history[j].count;
        recentCycles += info->history[j].cycles;
        recentCount++;
    }

    if(!debug_state.hidePerfInfoText || selected)
    {
        if(totalTime)
        {
            float percent = (float)info->totalTime * 100.0f / totalTime;

            STR_COMBINE_BEGIN(buffer);
            STR_COMBINE_CAT(getPercentString(percent));
            STR_COMBINE_CAT("^t30t\t^0");
            STR_COMBINE_CAT(info->child.head ? info->open ? "[^1--^d]^" : "[^2+^d]^" : "     ^d^");
            STR_COMBINE_CAT_D(level % 10);
            switch(info->infoType)
            {
                case PERFINFO_TYPE_BITS:
                    STR_COMBINE_CAT("b:");
                    break;
                case PERFINFO_TYPE_MISC:
                    STR_COMBINE_CAT("c:");
                    break;
            }
            STR_COMBINE_CAT(info->locName);
            if(selected && hiddenChild)
            {
                STR_COMBINE_CAT(" ^2[*** HIDDEN CHILD ***]");
            }
            STR_COMBINE_END();
        }
        else
        {
            sprintf(buffer, "^%d%s%s%s\n", level % 10, info->infoType ? "b:" : "", info->locName,
                    info->child.head ? info->open ? " ^d[^1--^d]" : " ^d[^2+^d]" : selected && hiddenChild ? " ^2[*** HIDDEN CHILD ***]" : "");
        }

        printDebugString( buffer, 15 + level * 10 + (selected && hiddenChild ? 12 : 0), h - 15 - draw_y + 3, 1, 11, -1, 0, 255, NULL);

        if(totalTime)
        {
            STR_COMBINE_BEGIN(buffer);
            STR_COMBINE_CAT("^4");
            STR_COMBINE_CAT(entDebugGetCommaSeparatedInt(info->opCountInt));
            STR_COMBINE_CAT("^t100t\t");
            STR_COMBINE_CAT(entDebugGetCommaSeparatedInt(info->opCountInt ? info->totalTime / info->opCountInt : 0));
            STR_COMBINE_CAT("^t100t\t");
            STR_COMBINE_CAT(getCommaSeparatedFloat(recentCount ? ((float) recentRuns) / recentCount : 0.0f, 2));
            STR_COMBINE_CAT("^t100t\t");
            STR_COMBINE_CAT(entDebugGetCommaSeparatedInt(recentCount ? recentCycles / recentCount : 0));
            if(debug_state.showMaxCPUCycles)
            {
                STR_COMBINE_CAT("^t100t\t^0(^s^4");
                STR_COMBINE_CAT(entDebugGetCommaSeparatedInt(info->maxTime));
                STR_COMBINE_CAT("^0^n)");
            }
            STR_COMBINE_END();
            
            printDebugString( buffer, 460, h - 15 - draw_y + 3, 1, 15, -1, 0, 255, NULL);
        }
    }

    if(info->historyPos < 0 || info->historyPos >= size)
        info->historyPos = 0;
    

    if(alpha)
        drawFilledBox(251, h - draw_y - 15, 450, h - draw_y, alpha);
    

    for(j = (info->historyPos + 1) % size, k = 0; k < size - 1;    j = (j + 1) % size, k++)
    {
        float percent = (float)info->history[j].cycles * 100 / (float)cpuSpeed;
        int color;
        int height = scale * percent * 2 * 100;
        int true_color;
        int base_color = 0xff00ffff;

        percent = min(percent, 100);

        color = 255 * percent / 10;
        color = max(0, min(255, color));
        true_color = color << 16 | (max(min(64 + 255 - color, 255), 64) << 8) | (255 - color);
        
        if(selected && debug_state.perfInfoZoomed && debug_state.perfInfoZoomedHilight - 1 == k)
        {
            base_color = 0xffff00;
            true_color = 0xffff00;
        }

        if(height > 0)
        {
            int alpha = (64 + (191 * (height % 100)) / 99) << 24;
            if(height >= 100)
            {
                drawLine2( 449 - k, h - draw_y - 15, 449 - k, h - draw_y - 15 + (height / 100), 0xff000000 | base_color, 0xff000000 | true_color);
            }

            if(height % 100)
            {
                drawLine2( 449 - k, h - draw_y - 15 + (height / 100), 449 - k, h - draw_y - 15 + (height / 100) + 1, alpha | true_color, alpha | true_color);
            }
        }
    }
}

static void displayPerformanceInfoZoomed(__int64 cpuSpeed)
{
    PerformanceInfo* info = debug_state.perfInfoZoomed;
    int j;
    int k;
    int size = ARRAY_SIZE(info->history);
    float scale = debug_state.perfInfoScaleF32;
    int w, h;
    char tempString[1000];
    int colorIndex;
    PerformanceInfo* cur;

    if(!info || debug_state.perfInfoZoomedAlpha <= 0)
        return;
        
    if(cpuSpeed <= 0){
        cpuSpeed = 2000000000;
    }

    windowSize(&w, &h);

    cur = info;
    colorIndex = 0;

    while(cur->parent){
        colorIndex++;
        cur = cur->parent;
    }

    colorIndex %= 10;

    if(debug_state.perfInfoZoomedHilight > 0 && debug_state.perfInfoZoomedHilight <= size){
        PerformanceInfoHistory* hist = info->history + (info->historyPos + debug_state.perfInfoZoomedHilight) % size;
        sprintf(tempString, "Line: ^4%d\nCycles: ^4%s\nCount: ^4%s^dx", debug_state.perfInfoZoomedHilight, entDebugGetCommaSeparatedInt(hist->cycles), entDebugGetCommaSeparatedInt(hist->count));
        drawFilledBox4(0, 0, 300, 140, 0x80000000, 0x80000000, 0xa0000000, 0xa0000000);
        printDebugString(tempString, 10, 100, 1.5, 15, -1, colorIndex, 255, NULL);
    }

    STR_COMBINE_BEGIN(tempString);
    switch(info->infoType){
        case PERFINFO_TYPE_BITS:
            STR_COMBINE_CAT("bits:");
            break;
        case PERFINFO_TYPE_MISC:
            STR_COMBINE_CAT("count:");
            break;
    }
    STR_COMBINE_CAT(info->locName);
    STR_COMBINE_END();

    printDebugString(tempString, 10, 10, 2.5, 1, colorIndex, -1, debug_state.perfInfoZoomedAlpha, NULL);

    drawFilledBox4(w - size * 4, 0, w, 100, 0, 0, debug_state.perfInfoZoomedAlpha << 24, debug_state.perfInfoZoomedAlpha << 24);
    drawFilledBox4(w - size * 4 - 30, 0, w - size * 4, 100, 0, 0, 0, debug_state.perfInfoZoomedAlpha << 24);

    for(j = (info->historyPos + 1) % size, k = 0;
        k < size - 1;
        j = (j + 1) % size, k++)
    {
        float percent = (float)info->history[j].cycles * 100 / (float)cpuSpeed;
        int color;
        int height = scale * percent * 2 * 100;
        int master_alpha = debug_state.perfInfoZoomedAlpha;
        int hilight = k == debug_state.perfInfoZoomedHilight - 1;

        percent = min(percent, 100);

        color = 255 * percent / 10;
        color = max(0, min(255, color));

        percent = (float)info->history[j].cycles * 10000 / (float)cpuSpeed;

        height = scale * percent * 2 * 100;

        percent = min(percent, 100);

        color = 255 * percent / 100;
        color = max(0, min(255, color));
        
        if(height > 0 || hilight){
            int alpha = ((master_alpha * (height % 100)) / 99) << 24;
            int top_color = color << 16 | (max(min(64 + 255 - color, 255), 64) << 8) | (255 - color);
            int bottom_color = 0x00ffff;
            
            if(hilight){
                top_color = bottom_color = 0xffff00;
                master_alpha = 0xff;
            }

            master_alpha = master_alpha << 24;

            if(hilight){
                drawFilledBox4(    w - (k + 1) * 4,
                                0,
                                w - k * 4,
                                h,
                                (64 << 24) | 0xff0000,
                                (64 << 24) | 0xff0000,
                                (64 << 24) | 0xff0000,
                                (64 << 24) | 0xff0000);
            }

            if(height >= 100){
                drawFilledBox4(    w - (k + 1) * 4,
                                0,
                                w - k * 4,
                                (height / 100),
                                master_alpha | top_color,
                                master_alpha | top_color,
                                master_alpha | bottom_color,
                                master_alpha | bottom_color);
            }

            if(height % 100){
                drawFilledBox4(    w - (k + 1) * 4,
                                (height / 100),
                                w - k * 4,
                                (height / 100) + 4,
                                alpha | top_color,
                                alpha | top_color,
                                master_alpha | top_color,
                                master_alpha | top_color);
            }
        }
    }

    {
        U64 cycles100;
        U64 order;
        float line = 50;
        int alpha = debug_state.perfInfoZoomedAlpha;
        int y;
        U64 cur_order;

        cycles100 = 100 * (line / (scale * 2 * 100)) * cpuSpeed / 10000;

        for(order = 1; cycles100; order *= 10){
            cycles100 /= 10;
        }

        line = 10000 * scale * 200 * order / cpuSpeed / 100;

        for(y = 0, cur_order = 0; y < h; y += line, cur_order += order){
            char buffer[100];
            int wh[2];
            int i;

            drawFilledBox4(w - 800, y, w, y + 1, 0xffffff, (alpha << 24) | 0xffffff, 0xffffff, (alpha << 24) | 0xffffff);

            for(i = 1; i < 10; i++){
                if(i % 5)
                    drawFilledBox4(w - 50, y + i * line / 10, w, y + i * line / 10 + 1, 0xffffff, ((alpha / 2) << 24) | 0xffffff, 0xffffff, ((alpha / 2) << 24) | 0xffffff);
                else
                    drawFilledBox4(w - 150, y + i * line / 10, w, y + i * line / 10 + 1, 0xff6666, ((alpha) << 24) | 0xffffff, 0xffffff, ((alpha / 2) << 24) | 0xff6666);
            }

            strcpy(buffer, entDebugGetCommaSeparatedInt(cur_order));

            printDebugString(buffer, 0, 0, 1, 0, -1, -1, 0, wh);
            
            printDebugString(buffer, w - wh[0] - 5, y + 5, 1, 10, -1, -1, alpha, NULL);
        }
    }
}

static void drawPerformanceButtons(int isServer, int showUnhide){
    char buffer[1000];
    int w, h;
    int mouse_x, mouse_y;
    float draw_y;
    
    if(inpIsMouseLocked())
        return;

    windowSize(&w, &h);
    inpMousePos(&mouse_x, &mouse_y);
    
    draw_y = h + max(0, sqrt(SQR(mouse_y) + SQR(mouse_x - PERF_INFO_WIDTH - 100)) - 200);
    drawFilledBox(PERF_INFO_WIDTH + 5, draw_y - 155, PERF_INFO_WIDTH + 165, draw_y, 0x80000080);
    // First row.
    drawButton2D(PERF_INFO_WIDTH + 10, draw_y - 30, 150, 25, 1, "^1Off", 1.8, isServer ? "runperfinfo_server 0\nsendautotimers 0" : "runperfinfo_game 0", NULL);
    // Second row.
    drawButton2D(PERF_INFO_WIDTH + 10, draw_y - 60, 75, 25, 1, "Reset", 1, isServer ? "resetperfinfo_server 15" : "resetperfinfo_game 15", NULL);
    if(drawButton2D(PERF_INFO_WIDTH + 90, draw_y - 60, 70, 25, 1, debug_state.perfInfoPaused ? "^rPLAY" : "^1PAUSE", 1, NULL, NULL))
        debug_state.perfInfoPaused ^= 1;
    // Third row.
    if(drawButton2D(PERF_INFO_WIDTH + 10, draw_y - 90, 50, 25, 1, debug_state.hidePerfInfo ? "^rShow" : "^1Hide", 1, NULL, NULL))
        debug_state.hidePerfInfo ^= 1;
    if(drawButton2D(PERF_INFO_WIDTH + 65, draw_y - 90, 50, 25, 1, debug_state.hidePerfInfoText ? "^1Text" : "^2Text", 1, NULL, NULL))
        debug_state.hidePerfInfoText ^= 1;
    if(drawButton2D(PERF_INFO_WIDTH + 120, draw_y - 90, 40, 25, 1, debug_state.showMaxCPUCycles ? "^2Max" : "^1Max", 1, NULL, NULL))
        debug_state.showMaxCPUCycles ^= 1;
    // Fourth row.
    if(drawButton2D(PERF_INFO_WIDTH + 10, draw_y - 120, 25, 25, 1, "-", 2, NULL, NULL))
        debug_state.perfInfoScale--;
    if(drawButton2D(PERF_INFO_WIDTH + 40, draw_y - 120, 25, 25, 1, "+", 2, NULL, NULL))
        debug_state.perfInfoScale++;
    STR_COMBINE_BEGIN(buffer);
    STR_COMBINE_CAT("Scale: ^4");
    STR_COMBINE_CAT_D(debug_state.perfInfoScale);
    STR_COMBINE_END();            
    // Fifth row.
    if(drawButton2D(PERF_INFO_WIDTH + 70, draw_y - 120, 90, 25, 1, buffer, 1, NULL, NULL))
        debug_state.perfInfoScale = 0;
    if(showUnhide)
        drawButton2D(PERF_INFO_WIDTH + 10, draw_y - 150, 40, 25, 1, "^rTops", 1, NULL, &debug_state.resetPerfInfoTops);
    if(drawButton2D(PERF_INFO_WIDTH + 10 + 45 * showUnhide, draw_y - 150, 150 - 45 * showUnhide, 25, 1,
        debug_state.perfInfoBreaks ? "Mode: ^rBREAK!!!" : "Mode: ^2Hide", 1, NULL, NULL))
        debug_state.perfInfoBreaks ^= 1;
}

static void updatePerfDrawY(){
    int w, h;
    int bottom_y;
    
    windowSize(&w, &h);
    
    h = (h / 15) * 15;
    
    bottom_y = h - 15;
    
    if(selectedPerfDrawY > 0 && selectedPerfDrawY < 115){
        int add = selectedPerfDrawY - 115;
        
        perfDrawY += add;
    }

    if(maxPerfDrawY < bottom_y){
        if(minPerfDrawY < 15){
            perfDrawY = maxPerfDrawY - minPerfDrawY - bottom_y;
        }
    }
    else if(selectedPerfDrawY > 0 && selectedPerfDrawY > h - 105){
        int add = selectedPerfDrawY - (h - 105);
        
        if(add > maxPerfDrawY - bottom_y){
            add = maxPerfDrawY - bottom_y;
        }
        
        perfDrawY += add;
    }
    
    perfDrawY = (perfDrawY / 15) * 15;
    
    if(perfDrawY < 15)
        perfDrawY = 15;
}

static void displayPerformanceInfo()
{
    int i;
    int y = 45 - perfDrawY;
    char buffer[1000];
    int w, h;
    char* titleFormat = "Proc %% : Name(^4%d^dx)^t180t\t%s %s^b%d/25b ^dCPU(^4%1.1f^dGhz)%s^t450t\tRuns^t550t\tCycles/Run^t650t\tRuns/Tick^t750t\tCycles/Tick";
    char* subtitleFormat = "^t447t\t(Total)^t547t\t(Total)^t647t\t(~Last Minute)^t747t\t(~Last Minute)";
    int resetTops = debug_state.resetPerfInfoTops;
    int mouse_x, mouse_y;
    PerformanceInfo* cur;
    int showUnhide = 0;
    S64 clientCPUSpeed;

    if(    debug_state.menuItem ||
        editMode() ||
        !playerPtr() ||
        !playerPtr()->access_level ||
        timing_state.autoTimerRecordLayoutID)
    {
        return;
    }

    windowSize(&w, &h);
    inpMousePos(&mouse_x, &mouse_y);

    if(debug_state.resetPerfInfoTops)
    {
        debug_state.resetPerfInfoTops = 0;
    }

    debug_state.perfInfoZoomed = NULL;
    debug_state.perfInfoZoomedHilight = 0;
    
    debug_state.perfInfoScaleF32 = pow(2, debug_state.perfInfoScale - 2);

    selectedPerfDrawY = -1;
    maxPerfDrawY = INT_MIN;
    minPerfDrawY = INT_MAX;
    
    if(!timing_state.autoTimerRootList)
    {
        if(!debug_state.serverTimersCount)
            return;
        
        if(!debug_state.hidePerfInfo)
        {
            for(i = 0; i < debug_state.serverTimersCount; i++)
            {
                PerformanceInfo* info = debug_state.serverTimers + i;
                if(!info->parent)
                {
                    if(info->hidden)
                    {
                        showUnhide = 1;
                    }
                    else
                    {
                        displayPerformanceInfoHelper(    info,
                                                        &y,
                                                        0,
                                                        debug_state.serverTimersTotal,
                                                        debug_state.serverCPUSpeed,
                                                        "s");
                        
                        y += 15;
                    }
                }
            }
#define FILLEDBOX_COLORS 0xff008000, 0x80008000, 0xff008000, 0x80008000
            drawFilledBox4(    0, h - 30, PERF_INFO_WIDTH, h, FILLEDBOX_COLORS);
            
            if(!debug_state.hidePerfInfoText)
            {
                int percent = 100.0 * (float)debug_state.serverTimeUsed / (float)debug_state.serverTimePassed + 0.5;
                
                sprintf(buffer,
                        titleFormat,
                        debug_state.serverTimersCount,
                        "^2Server^d",
                        getPercentColor(percent / 4),
                        percent / 4,
                        0.001 * (debug_state.serverCPUSpeed / 1000000),
                        debug_state.perfInfoPaused ? " ^rPAUSED!^d" : "");
                
                printDebugString(buffer, 15, h - 12, 1, 11, -1, 0, 255, NULL);

                printDebugString(subtitleFormat, 15, h - 27, 1, 11, -1, 0, 255, NULL);
            }
        }
        
        if(resetTops){
            for(i = 0; i < debug_state.serverTimersCount; i++){
                if(debug_state.serverTimers[i].hidden && !debug_state.serverTimers[i].parent){
                    perfInfoSetHidden("s", &debug_state.serverTimers[i], 0);
                }
            }
        }
        
        updatePerfDrawY();
        
        if(debug_state.serverCPUSpeed)
            displayPerformanceInfoZoomed(debug_state.serverCPUSpeed);
        drawPerformanceButtons(1, showUnhide);
        return;
    }

    // Now all the client stuff if there's no server info.

    clientCPUSpeed = timing_state.runperfinfo ? getRegistryMhz() : timing_state.autoTimerPlaybackCPUSpeed;

    if(resetTops){
        for(cur = timing_state.autoTimerRootList; cur; cur = cur->nextSibling){
            perfInfoSetHidden("client", cur, 0);
        }
    }

    if(!debug_state.hidePerfInfo)
    {
        for(cur = timing_state.autoTimerRootList; cur; cur = cur->nextSibling)
        {
            if(!cur->parent)
            {
                if(cur->hidden)
                {
                    showUnhide = 1;
                }
                else
                {
                    displayPerformanceInfoHelper(cur, &y, 0, timing_state.totalTime, clientCPUSpeed, timing_state.runperfinfo ? "c" : "r");
                    y += 15;
                }
            }
        }

        drawFilledBox4(    0, h - 15, PERF_INFO_WIDTH, h, 0xff008000, 0x80008000, 0xff008000, 0x80008000);

        i = (timing_state.totalCPUTimePos + ARRAY_SIZE(timing_state.totalCPUTimePassed) - 1) % ARRAY_SIZE(timing_state.totalCPUTimePassed);

        if(!debug_state.hidePerfInfoText){
            int percent = 100.0 * (float)timing_state.totalCPUTimeUsed[i] / (float)timing_state.totalCPUTimePassed[i] + 0.5;
            sprintf(buffer,
                    titleFormat,
                    timing_state.autoTimerCount,
                    timing_state.runperfinfo ? "^2Client^d" : "^rPlayback!^d",
                    getPercentColor(percent / 4),
                    percent,
                    0.001 * (clientCPUSpeed / 1000000),
                    debug_state.perfInfoPaused ? " ^rPAUSED!^d" : "");

            printDebugString(buffer, 15, h - 12, 1, 11, -1, 0, 255, NULL);

            printDebugString(subtitleFormat, 15, h - 27, 1, 11, -1, 0, 255, NULL);
        }
    }

    updatePerfDrawY();
    
    displayPerformanceInfoZoomed(clientCPUSpeed);

    drawPerformanceButtons(0, showUnhide);
    
    if(!timing_state.runperfinfo)
    {
        sprintf(buffer, "Steps: ^4%d", timing_state.autoTimerPlaybackStepSize);
        if(drawButton2D(660, h - 200, 100, 25, 1, buffer, 1, NULL, NULL))
            timing_state.autoTimerPlaybackStepSize = 1;
        if(drawButton2D(765, h - 200, 25, 25, 1, "+", 1, NULL, NULL))
            timing_state.autoTimerPlaybackStepSize++;
        if(drawButton2D(795, h - 200, 35, 25, 1, "+ 10", 1, NULL, NULL))
            timing_state.autoTimerPlaybackStepSize += 10;

        if(drawButton2D(765, h - 230, 25, 25, 1, "-", 1, NULL, NULL))
            timing_state.autoTimerPlaybackStepSize = max(timing_state.autoTimerPlaybackStepSize - 1, 1);
        if(drawButton2D(795, h - 230, 35, 25, 1, "- 10", 1, NULL, NULL))
            timing_state.autoTimerPlaybackStepSize = max(timing_state.autoTimerPlaybackStepSize - 10, 1);
            
        if(drawButton2D(660, h - 230, 100, 25, 1, timing_state.autoTimerPlaybackState == 0 ? "^2play" : "^7PAUSE", 1, NULL, NULL))
            timing_state.autoTimerPlaybackState = timing_state.autoTimerPlaybackState ? 0 : -1;
            
        if(drawButton2D(660, h - 260, 100, 25, 1, "^7Single Step", 1, NULL, NULL))
            timing_state.autoTimerPlaybackState = 1;

        drawButton2D(660, h - 290, 100, 25, 1, "^1STOP", 1, "profiler_stop", NULL);
    }

    if(debug_state.perfInfoPaused && timing_state.runperfinfo)
    {
        PerformanceInfo* cur;

        for(cur = timing_state.autoTimerMasterList.head; cur; cur = cur->nextMaster)
        {
            cur->historyPos = (cur->historyPos + ARRAY_SIZE(cur->history) - 1) % ARRAY_SIZE(cur->history);
            ZeroStruct(&cur->history[cur->historyPos]);
        }
    }
}

#include <utilitieslib/utils/MemoryMonitor.h>


static void getFloorPoint(Vec3 start, Vec3 floor){
    CollInfo info;
    Vec3 rayEndPt;

    copyVec3(start, rayEndPt);
    rayEndPt[1] -= 100;

    if(collGrid(NULL, start, rayEndPt, &info, 0, COLL_DISTFROMSTART)){
        copyVec3(posPoint(&info), floor);
    }else{
        copyVec3(rayEndPt, floor);
    }
}

static int displayMenuItem(DebugMenuItem* item, int* line, int left, int x, int top, float scale, U8 alpha)
{
    char buffer[1000];
    int curLine = (*line)++;
    int selected = curLine == debug_state.cursel;
    int y;
    int flash = 0;
    int x_off = 0;
    float openAmount = 1;
    int openable;
    char* displayText;

    if(item->flash)
    {
        int timeDiff = timeGetTime() - item->flashStartTime;
        if(timeDiff < 300)
        {
            if(timeDiff >= 0)
            {
                openAmount = (float)timeDiff / 300;
                flash = 1;
                x_off = (sin(openAmount * PI)) * 10;
            }
        }
        else
            item->flash = 0;
    }

    openable = item->subItems.head || !item->commandString[0];

    if(openable)
    {
        sprintf(buffer, "%s", item->open ? "^1--" : "^2+");
    }
    else
    {
        wchar_t temp[4];
        sprintf(buffer, "^5");
        mbtowc(temp, "?", 1);
        strcat(buffer, WideToUTF8CharConvert(temp[0]));
    }

    y = top - (curLine + 1) * 16;

    if(curLine == debug_state.moveMouseToSel)
    {
        int w, h;
        int mouse_x, mouse_y;
        int win_x, win_y;

        if(debug_state.moveMouseToSel < debug_state.firstLine)
        {
            debug_state.firstLine = debug_state.moveMouseToSel - 4;
        }
        else
        {
            int new_x, new_y;

            windowSize(&w, &h);
            inpMousePos(&mouse_x, &mouse_y);
            windowPosition(&win_x, &win_y);

            new_x = win_x + mouse_x + GetSystemMetrics(SM_CXSIZEFRAME);
            new_y = win_y + h - y + GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYSIZE);

            SetCursorPos(new_x, new_y);

            debug_state.cursel = debug_state.moveMouseToSel;
            debug_state.moveMouseToSel = -1;
        }
    }

    if(y < -16)
        return 0;
    else if(y < debug_state.windowHeight)
    {
        float startScale = item->commandString[0] ? 1.2 : 1.5;

        printDebugString( buffer,left + x + x_off, y, 1.5 + 0.2 - 0.2 * min(1, 3 * scale), 11, flash ? (global_state.global_frame_count & 3 ? 1 : 0) : selected ? 7 : -1, 0, alpha, NULL);

        displayText = strstr(item->displayText, "$$");

        if(displayText)
            displayText += 2;
        else
            displayText = item->displayText;

        sprintf(buffer, "%s%s", displayText, openable ? "^0..." : "");

        if(item->usedCount)
            strcatf(buffer, " ^r[x%d]", item->usedCount);

         printDebugString( buffer, left + x + x_off + 20, y, startScale + 0.2 - 0.2 * min(1, 3 * scale), 11, flash ? (global_state.global_frame_count & 3 ? 1 : 0) : selected ? 7 : -1, openable ? 9 : 0, alpha, NULL);

        if(selected){
            int w, h;
            int widthHeight[2];
            
            windowSize(&w, &h);
            
            sprintf(buffer, item->bitsUsed % 8 ? "^4%d ^0bytes, ^4%d ^0bits" : "^4%d ^0bytes", item->bitsUsed / 8, item->bitsUsed % 8);
            
            printDebugString(buffer, 0, 0, 1, 1, -1, -1, 0, widthHeight);

            printDebugString(buffer, w - widthHeight[0] - 4, h - 13, 1, 1, -1, -1, 192, NULL);

            if(item->commandString[0])
            {
                printDebugString( displayText, left + x_off + 485, h - 28, 2.5, 11, flash ? (global_state.global_frame_count & 3 ? 1 : 0) : -1, 0, alpha, NULL);
                printDebugString( item->commandString, left + x_off + 495, h - 60, 1.8, 11, flash ? (global_state.global_frame_count & 3 ? 1 : 0) : 7, 0, alpha, NULL);
            }
            else if(item->rolloverText && item->rolloverText[0])
            {
                printDebugString( displayText, left + x_off + 485, h - 28, 2.5, 11, flash ? (global_state.global_frame_count & 3 ? 1 : 0) : -1, 0, alpha, NULL);

                printDebugString( item->rolloverText, left + 485, h - 60, 1.5, 11, -1, 0, alpha, NULL);
            }
            else
            {
                char* commands =    "^t50t"
                                    "Mouse Buttons^s\n"
                                    "  Left:\t^8^sOpen/Close Group ^0OR ^8Execute Command\n"
                                    "  Right:\t^8Open/Close Group ^0OR ^8Execute Command, Don't Close Menu\n"
                                    "  Middle:\t^8Close Group ^0OR ^8Close Selection's Group\n"
                                    "^l400l\n"
                                    "^nKeyboard^s\n"
                                    "  Arrows:\t^8^sNavigate\t ^0Ctrl+C: ^8^sCopy Current Command\t ^0Ctrl+B: ^8^sBind Current Command\t ^0Ctrl+M: ^8^sMacro Current Command\t ^0Ctrl+W: ^8^Add to Custom Window\n"
                                    "  Enter:\t^8^sOpen/Close Group ^0OR ^8Execute Command\n"
                                    "  Space:\t^8^sOpen/Close Group ^0OR ^8Execute Command, Don't Close Menu\n";  

                printDebugString( "Menu Help", left+ 485, h - 28, 2.5, 11, -1, 0, alpha, NULL);
                printDebugString( commands, left + 485, h - 60, 1.5, 11, -1, 0, alpha, NULL);
            }
        }
    }

    if(item->open)
    {
        int done = 0;
        float subScale = scale * openAmount;
        DebugMenuItem* subItem;

        for(subItem = item->subItems.head; subItem; subItem = subItem->nextSibling)
        {
            if(!done && !displayMenuItem(subItem, line, left, x + 25, top, subScale, alpha))
                    done = 1;
        }
        if(done)
            return 0;
    }

    return 1;
}

void debugMenuKeyboardNavigation(int curTime)
{
    static int last_input;
    static char matchString[22];
    char * matchPtr = matchString + strlen(matchString); 
    KeyInput *input;
    DebugMenuItem *item;

    if( curTime - last_input > 2000)
        *matchString = 0;

    input = inpGetKeyBuf();
     while(input && strlen(matchString)<20)
    {
        if(input->type == KIT_Ascii)
        {
            *matchPtr = input->asciiCharacter;
            matchPtr++;
            *matchPtr = 0;

            if( debug_state.waitingForBindKey )
            {
                int sel = debug_state.cursel;
                DebugMenuItem* item = getCurMenuItem(debug_state.menuItem, &sel);
                
                if( item && item->commandString && *item->commandString )
                {
                    char * bindkey;
                    estrCreate(&bindkey);

                    if( input->attrib&KIA_ALT )
                        estrConcatStaticCharArray(&bindkey, "alt+");
                    if( input->attrib&KIA_CONTROL )
                        estrConcatStaticCharArray(&bindkey, "ctrl+");
                    if( input->attrib&KIA_SHIFT )
                        estrConcatStaticCharArray(&bindkey, "shift+");

                    estrConcatChar( &bindkey, input->asciiCharacter); 
                    unbindKeyProfile(&ent_debug_binds_profile);
                    bindKey( bindkey, item->commandString, 1 );
                    bindKeyProfile(&ent_debug_binds_profile);
                    estrDestroy(&bindkey);
                }

                debug_state.waitingForBindKey = 0;
            }
        }
        last_input = curTime;
        inpGetNextKey(&input);
    }

    if( strlen(matchString) )
    {
        int sel = 0;
        item = matchMenuItem(debug_state.menuItem, &sel, matchString );
        if(item)
            debug_state.cursel = sel;

    }
}

void displayDebugMenu()
{
    int curTime;
    int w, h;


    if(!debug_state.menuItem)
        return;
        
    setCursor(NULL, NULL, FALSE, 2, 2);
    curTime = (int)timeGetTime();
    windowSize(&w,&h);
    debug_state.windowHeight = h;

    if(    ( debug_state.downHeld || debug_state.upHeld ||    debug_state.pagedownHeld || debug_state.pageupHeld) 
        && curTime - debug_state.keyDelayMsecStart > debug_state.keyDelayMsecWait )
    {
        int sel;

        debug_state.mouseMustMove = 1;
        debug_state.keyDelayMsecStart += debug_state.keyDelayMsecWait;
        debug_state.cursel += debug_state.downHeld + 20 * debug_state.pagedownHeld - (debug_state.upHeld + 20 * debug_state.pageupHeld);

        while(curTime - debug_state.keyDelayMsecStart > 30)
        {
            debug_state.keyDelayMsecStart += 30;
            debug_state.cursel += debug_state.downHeld + 20 * debug_state.pagedownHeld - (debug_state.upHeld + 20 * debug_state.pageupHeld);
        }

        debug_state.keyDelayMsecWait = 30;

        if(debug_state.cursel < 0)
            debug_state.cursel = 0;
        

        sel = debug_state.cursel;

        while(debug_state.cursel > 0 && !getCurMenuItem(debug_state.menuItem, &sel))
        {
            debug_state.cursel--;
            sel = debug_state.cursel;
        }
    }

    // Keyboard text navigation
    debugMenuKeyboardNavigation(curTime);

    if(debug_state.menuItem)
    {
        int line = 0;
        int width = 500;
        int maxLines = (h - 20) / 16;
        int totalLines = getTotalOpenItemCount(debug_state.menuItem);
        int mouseLine;
        int mouse_x, mouse_y;
        int x_off = 0;
        int alpha = 255;
        int lines_from_bottom;

        inpMousePos(&mouse_x, &mouse_y);

        if(    !debug_state.closingTime && !debug_state.openingTime &&
            (!debug_state.mouseMustMove ||    mouse_x != debug_state.old_mouse_x || mouse_y != debug_state.old_mouse_y) )
        {
            debug_state.mouseMustMove = 0;

            mouseLine = debug_state.firstLine + (mouse_y - 22) / 16;

            if(mouse_x < width && mouse_x > 0 && mouseLine >= 0 && mouseLine < totalLines)
                debug_state.cursel = mouseLine;
            
        }

        if((debug_state.cursel - debug_state.firstLine) > maxLines - 5)
            debug_state.firstLine += (debug_state.cursel - debug_state.firstLine) - (maxLines - 5);
        

        if(debug_state.cursel < debug_state.firstLine + 4)
            debug_state.firstLine -= 4 - (debug_state.cursel - debug_state.firstLine);
        

        if(debug_state.firstLine + maxLines > totalLines)
            debug_state.firstLine = totalLines - maxLines;
        

        if(debug_state.firstLine < 0)
            debug_state.firstLine = 0;
        

        lines_from_bottom = max(0, totalLines - (debug_state.firstLine + maxLines));

        debug_state.old_mouse_x = mouse_x;
        debug_state.old_mouse_y = mouse_y;

        if(debug_state.closingTime)
        {
            float enterTime = 150.0;
            debug_state.closingTime += curTime - debug_state.lastTime;

            if(debug_state.closingTime > enterTime)
            {
                float maxTime = min(enterTime, debug_state.closingTime - 150);
                x_off = (2.0 - (1.0 + cos(maxTime * (PI) / enterTime))) * -250;
                alpha = ((enterTime - maxTime) * alpha) / enterTime;
            }
        }
        else if(debug_state.openingTime)
        {
            float enterTime = 100.0;
            debug_state.openingTime += curTime - debug_state.lastTime;

            if(debug_state.openingTime < enterTime)
            {
                float maxTime = min(enterTime, debug_state.openingTime);
                x_off = -500 + (2.0 - (1.0 + cos(maxTime * (PI) / enterTime))) * 250;
                alpha = (maxTime * alpha) / enterTime;
            }
            else
                debug_state.openingTime = 0;
        
        }

        // Save positions into debug_state
        debug_state.size.menuWidth = width;
        debug_state.size.x = x_off;
        debug_state.size.y = h;
        debug_state.size.greenx = x_off + width;
        debug_state.size.greeny = h - 201;
        debug_state.size.greenw = w - debug_state.size.greenx;
        debug_state.size.greenh = h - 201;
        debug_state.lastTime = curTime;

        drawFilledBox4(    x_off + 0, 0, x_off + width, h, (((alpha * 7) / 8) << 24) | 0x402040, (((alpha * 7) / 8) << 24) | 0x302040, (((alpha * 7) / 8) << 24) | 0x503060, (((alpha * 7) / 8) << 24) | 0x403080);
        drawFilledBox(x_off + width, 0, x_off + width + 1, h, (alpha << 24) | 0xff8000);
        drawFilledBox(x_off + width + 1, h - 34, w, h, ((alpha / 2) << 24) | 0xff6600);
        drawFilledBox(x_off + width + 1, h - 35, w, h - 34, (alpha << 24) | 0xff8000);
        drawFilledBox(x_off + width + 1, h - 200, w, h - 35, ((alpha / 2) << 24) | 0xffff00);
        drawFilledBox(x_off + width + 1, h - 201, w, h - 200, (alpha << 24) | 0xff8000);
        if (!game_state.texWordEdit) 
        {
            // Green box in lower right
            drawFilledBox(x_off + width + 1, 0, w, h - 201, ((alpha/4) << 24) | 0x00ff00 );
        }

        if(!debug_state.openingTime && !debug_state.closingTime)
        {
            drawFilledBox(    x_off + 2, h - 25 - (debug_state.cursel - debug_state.firstLine + 1) * 16,
                            x_off + width - 2, h - 19 - (debug_state.cursel - debug_state.firstLine) * 16, 0x40000000);

            drawFilledBox(    x_off + 3, h - 24 - (debug_state.cursel - debug_state.firstLine + 1) * 16,
                            x_off + width - 3,h - 20 - (debug_state.cursel - debug_state.firstLine) * 16,
                            debug_state.mouseMustMove || (mouse_x > 0 && mouse_x < 500) ? 0x80ffff00 | abs(((global_state.global_frame_count * 10) & 0xff) - 127) : 0x40ffff80);
        }

        displayMenuItem(debug_state.menuItem, &line, x_off + 20, 0, h - 20 + debug_state.firstLine * 16, 1, alpha);

        drawFilledBox4(    x_off + 0, 0, x_off + width, h / 6,0, 0,
                        (((min(lines_from_bottom, 10) * alpha * 7 / 10) / 16) << 24),
                        (((min(lines_from_bottom, 10) * alpha * 7 / 10) / 16) << 24));

        drawFilledBox4(    x_off + 0, h - h / 6, x_off + width, h,
                        (((min(debug_state.firstLine, 10) * alpha * 7 / 10) / 16) << 24),
                        (((min(debug_state.firstLine, 10) * alpha * 7 / 10) / 16) << 24),
                        0, 0);

        if(debug_state.closingTime > 300)
        {
            freeDebugMenuItem(debug_state.menuItem);
            debug_state.menuItem = NULL;
        }

        if(!debug_state.closingTime && !debug_state.openingTime && !game_state.texWordEdit)
        {
            if(drawButton2D(width + 5, h - 232, 130, 25,    1,    debug_state.retainMenuState ? "Keep State Is ^2ON" : "Keep State Is ^1OFF", 1, NULL, NULL))
                debug_state.retainMenuState ^= 1;

            if(drawButton2D(width + 140, h - 232, 130, 25, 1, debug_state.motdVisible ? "MOTD Is ^2ON" : "MOTD Is ^1OFF", 1, NULL, NULL))
                debug_state.motdVisible ^= 1;
        }

        if(debug_state.motdVisible && debug_state.motdText)
        {
            printDebugString(    debug_state.motdText, 505 + 500 * powf((float)max(0, debug_state.closingTime - 150) / 150, 3),
                                h - 260 - 300 * powf((float)(debug_state.openingTime ? (100 - debug_state.openingTime) : 0) / 100, 2), 1.5, 15, -1, -1, alpha, NULL);
        }
    }
}


static void displayEntDebugLines(){
    Entity* player = playerPtr();
    int i;


    if(player && game_state.ent_debug_client & 16){
        // Draw a line where the server's motion history says the player is.

        Mat4 mat;
        Vec3 pos, pos2;
        Vec3 dir;

        entCalcInterp(player, mat, global_state.client_abs, NULL);

        copyVec3(mat[3], pos);
        scaleVec3(mat[1], 5, dir);
        addVec3(dir, pos, pos);

        drawLine3D_2(mat[3], 0xffffffff, pos, 0xffff0000);

        copyVec3(pos, pos2);
        addVec3(pos2, mat[2], pos2);

        drawLine3D_2(pos, 0xffffffff, pos2, 0xffffffff);
    }

    for(i = 0; i < debug_state.linesCount; i++){
        EntDebugLine* line = debug_state.lines + i;

        drawLine3D_2(line->p1, line->argb1, line->p2, line->argb2);
    }

    for(i = 0; i < entities_max; i++){
        if(entity_state[i] & ENTITY_IN_USE){
            Vec3 p1, p2, p3, p4;
            int argb;
            Entity* e = entities[i];
            EntType entType = ENTTYPE_BY_ID(i);

            if(game_state.ent_debug_client & (1 | 2 | 8 | 32)){
                if(entType == ENTTYPE_CRITTER){
                    argb = 0xffff4455;
                }
                else if(entType == ENTTYPE_PLAYER){
                    argb = 0xff44ff55;
                }
                else if(entType == ENTTYPE_CAR){
                    argb = 0xff5544ff;
                }
                else if(entType == ENTTYPE_NPC){
                    argb = 0xffff00ff;
                }
                else if(entType == ENTTYPE_DOOR){
                    argb = 0xffffff00;
                }
                else{
                    argb =    0x80 << 24 |
                            (rand() & 0xff) << 16 |
                            (rand() & 0xff) << 8 |
                            (rand() & 0xff);
                }

                argb = (argb & 0xffffff) | (e->currfade << 24);

                if(entType != ENTTYPE_PLAYER && !(game_state.ent_debug_client & ~32))
                    continue;

                // Draw a colored line to the entity.

                if(game_state.ent_debug_client & (2 | 32)){
                    //Vec3 dir = { e->mat[1][0], e->mat[1][1], e->mat[1][2] };
                    //normalVec3( dir );
                    //copyVec3( e->mat[3], p1 );
                    //scaleVec3( dir, 20, dir );
                    //addVec3( dir, e->mat[3], p2 );

                    copyVec3(ENTPOS(player), p1);
                    copyVec3(ENTPOS(e), p2);
                    p1[1] += 2;
                    p2[1] += 2;
                    drawLine3D(p1, p2, argb);

                    if(e->seq && e->seq->gfx_root){
                        copyVec3(ENTPOS(e), p1);
                        copyVec3(posPoint(e->seq->gfx_root), p2);
                        p1[1] += 2;
                        drawLine3D(p1, p2, argb);
                    }

                    {
                        Vec3 dir1;
                        copyVec3(ENTMAT(e)[0], dir1);
                        normalVec3( dir1 );
                        copyVec3( ENTPOS(e), p1 );
                        scaleVec3( dir1, 20, dir1 );
                        addVec3( dir1, ENTPOS(e), p2 );
                        drawLine3D(p1, p2, (0x80 << 24) | (argb | 0xffffff));
                    }
                    {
                        Vec3 dir2;
                        copyVec3(ENTMAT(e)[2], dir2);
                        normalVec3( dir2 );
                        copyVec3( ENTPOS(e), p1 );
                        scaleVec3( dir2, 20, dir2 );
                        addVec3( dir2, ENTPOS(e), p2 );
                        drawLine3D(p1, p2, argb);
                    }

                    {
                        Vec3 pyr;
                        Mat3 mat;
                        copyVec3(control_state.pyr.cur, pyr);
                        pyr[0] = 0;
                        createMat3YPR(mat, pyr);
                        copyVec3(ENTPOS(player), p1);
                        copyVec3(p1, p2);
                        scaleVec3(mat[2], 30, mat[2]);
                        addVec3(p1, mat[2], p2);
                        drawLine3D(p1, p2, 0xffffff00);
                    }
                }

                // Draw xyz axes at the entity pos.

                if(game_state.ent_debug_client & (1 | 8)){
                    copyVec3(ENTPOS(e), p1);
                    copyVec3(ENTPOS(e), p2);
                    p2[1] += 20;
                    drawLine3D(p1, p2, argb);

                    copyVec3(ENTPOS(e), p1);
                    copyVec3(ENTPOS(e), p2);
                    p1[0] -= 2;
                    p2[0] += 2;
                    drawLine3D(p1, p2, argb);

                    copyVec3(ENTPOS(e), p1);
                    copyVec3(ENTPOS(e), p2);
                    p1[2] -= 2;
                    p2[2] += 2;
                    drawLine3D(p1, p2, argb);

                    copyVec3(ENTPOS(e), p1);
                    copyVec3(ENTPOS(e), p2);
                    p1[1] += 20;
                    p2[1] += 20;
                    p2[0] += ENTMAT(e)[2][0] * 5;
                    p2[2] += ENTMAT(e)[2][2] * 5;
                    drawLine3D(p1, p2, 0xffeeeeff);

                    if(e == current_target){
                        int j;

                        if(game_state.ent_debug_client & 1)
                        {
                            for(j = ARRAY_SIZE(e->motion_hist) - 1; j >= 0; j--){
                                Mat3 debugMat3;
                                int iDrawMat3;
                                int cur = (e->motion_hist_latest + j) % ARRAY_SIZE(e->motion_hist);
                                int next = (cur + 1) % ARRAY_SIZE(e->motion_hist);
                                
                                copyVec3(e->motion_hist[cur].pos, p1);
                                p1[1] += j ? 1.0 : 5.0;
                                copyVec3(e->motion_hist[cur].pos, p2);
                                drawLine3D(p1, p2, j ? 0xffff3300 : 0xff33ff88);

                                quatToMat(e->motion_hist[cur].qrot, debugMat3);
                                //createMat3YPR(debugMat3, e->motion_hist[cur].pyr);
                                for(iDrawMat3=0;iDrawMat3<3;++iDrawMat3)
                                {
                                    addVec3(p1,debugMat3[iDrawMat3],p2);
                                    drawLine3D(p1, p2, 0xff000000 | ((0xFF) << (iDrawMat3*8)));
                                }


                                if(j > 0){
                                    S32 time_diff = e->motion_hist[next].abs_time - e->motion_hist[cur].abs_time;

                                    if(time_diff >= 0 && time_diff < 3000)
                                    {
                                        U32 cur_time = e->motion_hist[cur].abs_time;
                                        Vec3 diff;

                                        copyVec3(e->motion_hist[cur].pos, p1);
                                        copyVec3(e->motion_hist[next].pos, p2);

                                        subVec3(p2, p1, diff);

                                        do{
                                            U32 end_time = cur_time + 100;
                                            float factor_1;
                                            float factor_2;
                                            int color;
                                            Vec3 diff_vec;

                                            end_time -= (end_time % 100);

                                            if(end_time > e->motion_hist[next].abs_time)
                                                end_time = e->motion_hist[next].abs_time;

                                            factor_1 = (float)(cur_time - e->motion_hist[cur].abs_time) / time_diff;
                                            factor_2 = (float)(end_time - e->motion_hist[cur].abs_time) / time_diff;

                                            if((cur_time / 100) & 1)
                                                color = 0xffffff00;
                                            else
                                                color = 0xffff8000;

                                            scaleVec3(diff, factor_1, diff_vec);
                                            addVec3(p1, diff_vec, p3);

                                            scaleVec3(diff, factor_2, diff_vec);
                                            addVec3(p1, diff_vec, p4);

                                            drawLine3D_2(p3, color, p4, color);

                                            if(end_time % 100 == 0)
                                            {
                                                copyVec3(p4, p3);

                                                p4[1] += 0.1;

                                                drawLine3D_2(p3, 0xffffffff, p4, 0xffffffff);
                                            }

                                            cur_time = end_time;
                                        }while(cur_time != e->motion_hist[next].abs_time);
                                    }
                                }
                            }
                        }

                        if(    game_state.ent_debug_client & 8 &&
                            e == player)
                        {
                            const int size = ARRAY_SIZE(control_state.svr_phys.step);
                            int k;

                            for(j = (control_state.svr_phys.end + 1) % size;
                                (j + 1) % size != control_state.svr_phys.end;
                                j = (j + 1) % size)
                            {
                                k = (j + 1) % size;
                                drawLine3D_2(    control_state.svr_phys.step[j].pos,
                                                0xffffffff,
                                                control_state.svr_phys.step[k].pos,
                                                0xff00ff00);
                            }
                        }
                    }
                }
            }

            // Draw a little red line on the current_target.

            if(0 && e == current_target){
                copyVec3(ENTPOS(e), p1);
                scaleVec3(ENTMAT(e)[1], 10, p2);
                addVec3(p2, ENTPOS(e), p2);

                drawLine3D(p1, p2, 0xffff4455);

                copyVec3(p2, p1);

                addVec3(p2, ENTMAT(e)[2], p2);

                drawLine3D(p1, p2, 0xffff9999);

                copyVec3(p1, p2);
                p2[0] += sin(RAD(global_state.global_frame_count * 30));
                p2[2] += cos(RAD(global_state.global_frame_count * 30));

                drawLine3D(p1, p2, 0xff44ff55);
            }

            // Draw a stupid spinning cylinder around the target under the mouse.
            if(    game_state.ent_debug_client &&
                (    e == current_target    || e == current_target_under_mouse ||
                    game_state.ent_debug_client & 64 &&    player &&
                    distance3(ENTPOS(player), ENTPOS(e)) < 10 + (e->motion->capsule.max_coll_dist + player->motion->capsule.max_coll_dist)))
            {
                SeqInst *seq = e->seq;
                Mat4 emat, collMat;
                int color = (e == current_target ? 0x88ff55 : e == current_target_under_mouse ? 0x5555ff : 0xff8855);
                int bones = eaSize(&seq->type->boneCapsules);

                if(ENTTYPE(e) == ENTTYPE_PLAYER && !bones)
                {
                    copyMat3(unitmat, emat);
                    copyVec3(ENTPOS(e), emat[3]);
                }
                else
                {
                    copyMat4(ENTMAT(e), emat);
                }

                positionCapsule( emat, &e->motion->capsule, collMat );
                drawSpinningCapsule( e->motion->capsule.radius, e->motion->capsule.length, collMat, bones ? 0x888888 : color);

                if(e == playerPtr())
                {
                    copyMat3(unitmat, emat);
                    copyVec3(e->net_pos, emat[3]);
                    positionCapsule(emat, &e->motion->capsule, collMat);
                    drawSpinningCapsule(e->motion->capsule.radius, e->motion->capsule.length, collMat, 0x99ccff);
                }

                if(bones)
                {
                    int i;
                    for(i = 0; i < bones; i++)
                    {
                        F32 len;
                        Mat4 mat;

                        seqGetBoneLine(mat[3], mat[1], &len, seq, seq->type->boneCapsules[i]->bone);

                        mat[1][0] ? setVec3(mat[2], 0, 0, 1) : setVec3(mat[2], 1, 0, 0);
                        crossVec3(mat[1], mat[2], mat[0]);    normalVec3(mat[0]);
                        crossVec3(mat[0], mat[1], mat[2]);    normalVec3(mat[2]);

                        drawSpinningCapsule(seq->type->boneCapsules[i]->radius, len, mat, color);
                    }
                }
            }


            if(e == current_target){
                int j;
                ClientEntityDebugInfo* info = e->debugInfo;
                int iSize;

                if(info){
                    // Draw my path.

                    for(j = 0; j < info->path.count; j++){
                        if(j < info->path.count - 1){
                            int color = entType == ENTTYPE_PLAYER ? 0x804455ff : 0x80ff4455;

                            if(info->path.points[j+1].minHeight){
                                copyVec3(info->path.points[j].pos, p1);
                                copyVec3(info->path.points[j+1].pos, p2);
                                copyVec3(p1, p3);
                                copyVec3(p2, p4);
                                p3[1] += 0.25 + info->path.points[j+1].maxHeight;
                                p4[1] = p3[1];
                                drawLine3D_2(p1, 0xffffffff, p3, color);
                                drawLine3D_2(p3, 0xffffffff, p4, color);
                                drawLine3D_2(p4, 0xffffffff, p2, color);
                                p3[1] -= 0.25;
                                p4[1] -= 0.25;
                                drawLine3D_2(p3, 0xffffffff, p4, color);

                                copyVec3(p3, p1);
                                copyVec3(p4, p2);
                                p3[1] += 0.25 + info->path.points[j+1].minHeight - info->path.points[j+1].maxHeight;
                                p4[1] = p3[1];

                                drawTriangle3D_3(p1, 0x80ffffff, p2, color, p3, 0x80ffffff);
                                drawTriangle3D_3(p2, color, p3, 0x80ffffff, p4, color);

                                drawLine3D_2(p3, 0xffffffff, p4, color);
                                p3[1] -= 0.25;
                                p4[1] -= 0.25;
                                drawLine3D_2(p3, 0xffffffff, p4, color);
                            }else{
                                if(info->path.points[j+1].isFlying){
                                    color = 0x804455ff;
                                }

                                copyVec3(info->path.points[j].pos, p1);
                                copyVec3(info->path.points[j+1].pos, p2);
                                p1[1] += 0.01;
                                p2[1] += 0.01;
                                drawLine3D_2(p1, 0xffffffff, p2, color);
                                p1[1] += 0.25;
                                p2[1] += 0.25;
                                drawLine3D_2(p1, 0xffffffff, p2, color);
                                p1[1] += 4.25;
                                p2[1] += 4.25;
                                drawLine3D_2(p1, 0x20ffffff, p2, color);
                            }
                        }

                        copyVec3(info->path.points[j].pos, p1);
                        copyVec3(info->path.points[j].pos, p2);
                        p2[1] += 4.5;
                        drawLine3D(p1, p2, 0x8044ff55);
                    }

                    // Show debug points.

                    for(j = 0; j < info->points.count; j++){
                        ClientEntityDebugPoint* point = info->points.pos + j;

                        if(point->lineToPrevious && j < info->points.count - 1){
                            copyVec3(point->pos, p1);
                            p1[1] += 2;
                            copyVec3((point+1)->pos, p2);
                            p2[1] += 2;
                            drawLine3D(p1, p2, 0xff8866ff);
                        }

                        copyVec3(point->pos, p1);
                        copyVec3(point->pos, p2);
                        vecY(p2) += 2;
                        drawLine3D(p1, p2, point->argb);
                        
                        if(point->lineToEnt){
                            drawLine3D(ENTPOS(e), p1, point->argb);
                        }
                    }

                    // Show team members.

                    iSize = eaiSize(&info->teamMembers);
                    for(j = 0; j < iSize; j++){
                        Entity* member = validEntFromId(info->teamMembers[j]);

                        if(member && member->pchar){
                            float hp_pct = member->pchar->attrCur.fHitPoints / member->pchar->attrMax.fHitPoints;
                            int color;

                            hp_pct = max(0, min(1, hp_pct));

                            copyVec3(ENTPOS(e), p1);
                            copyVec3(ENTPOS(member), p2);

                            p1[1] += 2;
                            p2[1] += 2;

                            if(hp_pct > 0){
                                color = ((0x80 + (int)(0x7f * hp_pct)) << 24) |
                                        ((0xff - (int)(0xbf * hp_pct)) << 0) |
                                        ((0x40 + (int)(0xbf * hp_pct)) << 8);
                            }else{
                                color = 0x80ff0000;
                            }

                            drawLine3D_2(    p1, (0xc0 << 24) | (~(0xff << 24) & color),
                                            p2, color);
                        }
                    }
                }
            }

            if (game_state.ent_debug_client & 256)
            {
                Mat4 mScaledMat;
                scaleMat3(ENTMAT(e), mScaledMat, e->seq->currgeomscale[0]);
                copyVec3(ENTPOS(e), mScaledMat[3]);
                
                #if NOVODEX
                    drawRagdollSkeleton( e->seq->gfx_root->child, mScaledMat, e->seq );
                #endif
            }

        }
    }
}

#define ENCOUNTER_BAR_HEIGHT 30
#define ENCOUNTER_BAR_WIDTH 500

static void displayEncounterStatsHelper(int x, int y, char* title, int* list, F32* flist, U32 alpha, int selected, int percentage)
{
    int scales[] = { 10, 25, 50, 100, 200, 400, 800, 1600, 3200, 6400, INT_MAX };
    char buf[2000];
    int xpos, lastentry, entry, val, high, max;

    // background box
    if (selected)
    {
        alpha = 0xff << 24;
        drawFilledBox4(    x, y, x+ENCOUNTER_BAR_WIDTH, y+ENCOUNTER_BAR_HEIGHT,
            0x442222 | alpha, 0x442222 | alpha, 0x220000 | alpha, 0x220000 | alpha );
    }
    else
    {
        drawFilledBox4(    x, y, x+ENCOUNTER_BAR_WIDTH, y+ENCOUNTER_BAR_HEIGHT,
            0x333333 | alpha, 0x333333 | alpha, 0x111111 | alpha, 0x111111 | alpha );
    }

    // title
    sprintf(buf, "^%i%s", selected? 7:2, title);
    printDebugStringAlign(buf, x, y, 250, ENCOUNTER_BAR_HEIGHT, ALIGN_VCENTER, 1.2, 12, -1, 0, 255);

    // find an appropriate scale
    high = 0;
    max = INT_MAX;
    for (entry = 0; entry < ENCOUNTER_HIST_LENGTH; entry++)
    {
        if (entry == debug_state.encounterTail && !debug_state.encounterWrap) break;
        if (list) high = max(high, list[entry]);
        else if (percentage) high = max(high, flist[entry]*100.0);
        else high = max(high, flist[entry]);
    }
    for (entry = 0; entry < ARRAY_SIZE(scales); entry++)
    {
        if (high <= scales[entry])
        {
            max = scales[entry];
            break;
        }
    }

    if (selected)
    {
        sprintf(buf, "^5%i", max);
        printDebugStringAlign(buf, x, y, 250, ENCOUNTER_BAR_HEIGHT, ALIGN_TOP | ALIGN_RIGHT, 0.8, 10, -1, 0, 255);
        printDebugStringAlign("^50", x, y, 250, ENCOUNTER_BAR_HEIGHT, ALIGN_BOTTOM | ALIGN_RIGHT, 0.8, 10, -1, 0, 255);
    }

    // lines
    if (debug_state.encounterTail) lastentry = debug_state.encounterTail - 1;
    else lastentry = ENCOUNTER_HIST_LENGTH - 1;
    entry = debug_state.encounterWrap? debug_state.encounterTail: 0;
    xpos = x + 250;
    while (1)
    {
        if (list) val = list[entry] * ENCOUNTER_BAR_HEIGHT / max;
        else if (percentage) val = (int)(flist[entry] * 100.0 * ENCOUNTER_BAR_HEIGHT / max);
        else val = (int)(flist[entry] * ENCOUNTER_BAR_HEIGHT / max);
        if (val > ENCOUNTER_BAR_HEIGHT) val = ENCOUNTER_BAR_HEIGHT;

        drawLine2(xpos, y, xpos, y+val, selected? 0xff0022ff: 0xff0033dd, selected? 0xffffffff: 0xff66aaff);
        if (entry == lastentry) break;
        xpos++;
        entry = (entry + 1) % ENCOUNTER_HIST_LENGTH;
    }

    // value
    if (list)
        sprintf(buf, "^3%d", list[lastentry]);
    else if (percentage)
        sprintf(buf, "^3%0.2f%%", flist[lastentry]*100.0);
    else
        sprintf(buf, "^3%0.2f", flist[lastentry]);
    printDebugStringAlign(buf, 425+x, y, 75, ENCOUNTER_BAR_HEIGHT, ALIGN_VCENTER, 1.2, 12, -1, 0, 255);
}

static void displayEncounterTweakNumbers(int x, int y, U32 alpha, int selected)
{
    char buf[2000];

    // show the tweaking numbers
    sprintf(buf, "^2Min distance from players: ^%i%i\n"
        "^2Min distance between encounters: ^%i%i\n"
        "^2Adjust spawn probability by: ^%i%i%%",
        debug_state.encounterPlayerRadiusIsDefault? 2: 1, debug_state.encounterPlayerRadius,
        debug_state.encounterVillainRadiusIsDefault? 2: 1, debug_state.encounterVillainRadius,
        debug_state.encounterAdjustProbIsDefault? 2: 1, debug_state.encounterAdjustProb);
    printDebugString(buf, x, y+ENCOUNTER_BAR_HEIGHT-14, 1.2, 12, -1, 0, 255, NULL);

    // show the panic system numbers
    sprintf(buf, "^2Panic threshold: %0.2f - %0.2f Enc/Active\n"
        "^2Min players to panic: %i",
        debug_state.encounterPanicLow,
        debug_state.encounterPanicHigh,
        debug_state.encounterPanicMinPlayers);
    printDebugString(buf, x, y+ENCOUNTER_BAR_HEIGHT-2-12*5, 1.2, 12, -1, 0, 255, NULL);

    // show the critter limit numbers
    sprintf(buf, "^2Current critters: %i\n"
        "^2Minimum critters: ^%i%i\n"
        "^2Maximum critters: ^%i%i",
        debug_state.encounterCurCritters,
        debug_state.encounterBelowMinCritters? 1: 2,
        debug_state.encounterMinCritters,
        debug_state.encounterAboveMaxCritters? 1: 2,
        debug_state.encounterMaxCritters);
    printDebugString(buf, x, y+ENCOUNTER_BAR_HEIGHT-2-12*8, 1.2, 12, -1, 0, 255, NULL);

}

static void displayEncounterStats()
{
    int w, h, mouse_x, mouse_y;
    int alpha;
    int selbar;

    if (editMode()) return;
    if (!game_state.hasEntDebugInfo || !debug_state.encounterReceivingStats) return;
    if(debug_state.menuItem) return;

    windowSize(&w, &h);
    inpMousePos(&mouse_x, &mouse_y);
    mouse_y = h-mouse_y;

    selbar = 0;
    if (!inpIsMouseLocked() && mouse_x >= 0 && mouse_x <= ENCOUNTER_BAR_WIDTH)
    {
        if (mouse_y >= h-ENCOUNTER_BAR_HEIGHT && mouse_y < h) selbar = 1;
        else if (mouse_y >= h-ENCOUNTER_BAR_HEIGHT*2 && mouse_y < h-ENCOUNTER_BAR_HEIGHT) selbar = 2;
        else if (mouse_y >= h-ENCOUNTER_BAR_HEIGHT*3 && mouse_y < h-ENCOUNTER_BAR_HEIGHT*2) selbar = 3;
        else if (mouse_y >= h-ENCOUNTER_BAR_HEIGHT*4 && mouse_y < h-ENCOUNTER_BAR_HEIGHT*3) selbar = 4;
        else if (mouse_y >= h-ENCOUNTER_BAR_HEIGHT*5 && mouse_y < h-ENCOUNTER_BAR_HEIGHT*4) selbar = 5;
        else if (mouse_y >= h-ENCOUNTER_BAR_HEIGHT*6 && mouse_y < h-ENCOUNTER_BAR_HEIGHT*5) selbar = 6;
        else if (mouse_y >= h-ENCOUNTER_BAR_HEIGHT*7 && mouse_y < h-ENCOUNTER_BAR_HEIGHT*6) selbar = 7;
    }
    if (selbar > 0)
        debug_state.mouseOverUI = 1;

    //if(!inpIsMouseLocked()){
    //    alpha = 355 - mouse_x;
    //    alpha = min(255, max(100, alpha));
    //}else{
    //    alpha = 100;
    //}
    alpha = 100;
    if (selbar > 0)
        alpha = 175;
    alpha <<= 24;

    displayEncounterStatsHelper(0, h-ENCOUNTER_BAR_HEIGHT, "Players In Zone", debug_state.encounterPlayerCount, NULL, alpha, selbar==1, 0);
    displayEncounterStatsHelper(0, h-ENCOUNTER_BAR_HEIGHT*2, "Encounters Running", debug_state.encounterRunning, NULL, alpha, selbar==2, 0);
    displayEncounterStatsHelper(0, h-ENCOUNTER_BAR_HEIGHT*3, "Encounters Active", debug_state.encounterActive, NULL, alpha, selbar==3, 0);
    displayEncounterStatsHelper(0, h-ENCOUNTER_BAR_HEIGHT*4, "Encounter Win Percentage", NULL, debug_state.encounterWin, alpha, selbar==4, 1);
    displayEncounterStatsHelper(0, h-ENCOUNTER_BAR_HEIGHT*5, "Encounters / Players", NULL, debug_state.encounterPlayerRatio, alpha, selbar==5, 0);
    displayEncounterStatsHelper(0, h-ENCOUNTER_BAR_HEIGHT*6, "Encounters / Active", NULL, debug_state.encounterActiveRatio, alpha, selbar==6, 0);
    displayEncounterStatsHelper(0, h-ENCOUNTER_BAR_HEIGHT*7, "Encounter Panic", debug_state.encounterPanicLevel, NULL, alpha, selbar==7, 0);

    displayEncounterTweakNumbers(0, h-ENCOUNTER_BAR_HEIGHT*8, alpha, 0);
}

static void encVert(Vec3 pos, F32 angle, Vec3 result, int off)
{
    Vec3 v = {0, 5, 0};
    v[0] = fcos(angle) * (off? 2.001: 2);
    v[2] = fsin(angle) * (off? 2.001: 2);
    addVec3(pos, v, result);
}

static void displayEncounterBeacons()
{
#define NUMSIDES 4
    int i, j;

    if (!game_state.hasEntDebugInfo) return;
    if (!debug_state.numEncounterBeacons) return;

    for (i = 0; i < debug_state.numEncounterBeacons && i < MAX_ENCOUNTER_BEACONS; i++)
    {
        EncounterBeacon* b = &debug_state.encounterBeacons[i];
        F32 angle = fixAngle(RAD(global_state.global_frame_count * b->speed / 5));

        if (b->filename[0])
            ClickToSourceDisplay(b->pos[0], b->pos[1]+7, b->pos[2], 0, 0xFFFFFFFF, b->filename, NULL, CTS_TEXT_DEBUG_3D);
        if (b->time)
        {
            char buf[100];
            timerMakeLogDateStringFromSecondsSince2000(buf, b->time);
            ClickToSourceDisplay(b->pos[0], b->pos[1]+9, b->pos[2], 0, 0xFFFFFFFF, NULL, buf, CTS_TEXT_DEBUG_3D);
        }
        for (j = 0; j < NUMSIDES; j++)
        {
            Vec3 p1, p2, top;

            encVert(b->pos, angle + 2*PI/NUMSIDES*j, p1, 0);
            encVert(b->pos, angle + 2*PI/NUMSIDES*(j+1), p2, 0);
            copyVec3(b->pos, top);
            top[1] += 7;
            drawTriangle3D_3(b->pos, 0xffffffff, p1, b->color, p2, b->color);
            drawTriangle3D_3(top, 0xffffffff, p1, b->color, p2, b->color);

            // edge lines
            //encVert(b->pos, angle + 2*PI/NUMSIDES*j, p3, 1);
            //drawLine3D_2(b->pos, 0xff555555, p3, 0xff555555);
            //drawLine3D_2(top, 0xff555555, p3, 0xff555555);
            //drawLine3D_2(p2, 0xff555555, p3, 0xff555555);
        }
    }
}

static void displayCrazyLoadCovering(){
    static float drawLoadThing = 0;
    float fadeInTime = 2.0 * 30;
    float maxLoadThing = fadeInTime + 0.2 * 30;

    if(startFadeInScreen(0)){
        drawLoadThing = fadeInTime;
    }

    if(drawLoadThing > 0){
        int w, h;
        int percent = 100 * (fadeInTime - drawLoadThing) / fadeInTime;

        percent = min(100, max(0, percent));

        drawLoadThing -= TIMESTEP;
        windowSize(&w, &h);

        {
            int alpha = 0xff - (0xff * (percent)) / 100;

            showBGAlpha(alpha | 0xffffff00);
        }
    }
}

static int __cdecl compareConnTotal(const void* i1pData, const void* i2pData){
    const AStarConnectionInfo** i1p = (const AStarConnectionInfo**)i1pData;
    const AStarConnectionInfo** i2p = (const AStarConnectionInfo**)i2pData;
    const AStarConnectionInfo* i1 = *i1p;
    const AStarConnectionInfo* i2 = *i2p;
    
    if(i1->totalCost < i2->totalCost){
        return -1;
    }
    else if(i1->totalCost == i2->totalCost){
        return 0;
    }
    else{
        return 1;
    }
}

static void destroyAStarRecordingSetAdapter(void* arg0)
{
    destroyAStarRecordingSet((AStarRecordingSet*)arg0);
}

static void destroyAStarBeaconBlockAdapter(void* arg0)
{
    destroyAStarBeaconBlock((AStarBeaconBlock*)arg0);
}

void displayAStarRecording(){
    static int showNextPoints;
    static int showBlockPath;
    
    int size = eaSize(&debug_state.aStarRecording.sets);
    int curFrame = debug_state.aStarRecording.frame;
    int w, h;
    int mouse_x, mouse_y;
    int next = 0;
    int prev = 0;
    int clear = 0;
    int first = 0;
    int last = 0;
    Vec3 lastPos;
    AStarRecordingSet* set;
    int i;
    int count;
    Vec3 pos;
    Vec3 pos2;

    if(!size)
        return;

    windowSize(&w, &h);
    inpMousePos(&mouse_x, &mouse_y);

    if(showBlockPath){
        // Draw the block path.
        
        count = eaSize(&debug_state.aStarRecording.blocks);
        
        for(i = 0; i < count; i++){
            AStarBeaconBlock* block = debug_state.aStarRecording.blocks[i];
            int j;
            
            copyVec3(block->pos, pos2);

            if(i){
                AStarBeaconBlock* prevBlock = debug_state.aStarRecording.blocks[i - 1];
                
                copyVec3(prevBlock->pos, pos);
                
                drawLine3D_2(pos, 0xffffffff, pos2, 0xffff0000);
                
                pos[1] = prevBlock->searchY;
                pos2[1] = block->searchY;

                drawLine3D_2(pos, 0xffffffff, pos2, 0xff00ff66);
            }
            
            pos2[1] = block->searchY;
            drawLine3D_2(block->pos, 0x80ffff00, pos2, 0x80ffff00);

            for(j = 0; j < eaSize(&block->beacons); j++){
                AStarPoint* point = block->beacons[j];
                
                drawLine3D_2(block->pos, 0xc0ffffffff, block->beacons[j]->pos, 0xc000ff00);
            }
        }
    }
    
    if(curFrame < 0)
        curFrame = 0;
    if(curFrame >= size)
        curFrame = size - 1;

    set = debug_state.aStarRecording.sets[curFrame];
    
    count = eaSize(&set->points);

    for(i = 0; i < count; i++){
        Vec3 pos3;
        Vec3 pos4;

        copyVec3(set->points[i]->pos, pos);

        pos[1] += 5;

        drawLine3D(set->points[i]->pos, pos, 0xffff0000);

        if(i > 0){
            // Draw from pos to next pos.

            drawLine3D(set->points[i]->pos, set->points[i-1]->pos, 0xff00ff00);

            if(set->points[i-1]->minHeight){
                // Draw min height on current pos.

                copyVec3(set->points[i]->pos, pos);
                copyVec3(set->points[i]->pos, pos2);

                pos2[1] += set->points[i-1]->maxHeight;

                drawLine3D(pos, pos2, 0xff0000ff);

                // Draw the cross-bar.

                copyVec3(set->points[i-1]->pos, pos);

                pos2[0] = pos[0];
                pos[1] = pos2[1];
                pos2[1] = set->points[i-1]->pos[1];
                pos2[2] = pos[2];

                drawLine3D(pos, pos2, 0xff0000ff);

                pos[0] = set->points[i]->pos[0];
                pos2[1] = pos[1];
                pos[2] = set->points[i]->pos[2];

                drawLine3D(pos, pos2, 0xff0000ff);

                // Draw the red-white triangles.

                copyVec3(set->points[i]->pos, pos3);
                copyVec3(set->points[i-1]->pos, pos4);

                pos3[1] += set->points[i-1]->minHeight;
                pos4[1] = pos3[1];

                drawLine3D(pos3, pos4, 0xff0000ff);
                drawTriangle3D_3(pos3, 0x40ffffff, pos, 0x40ffffff, pos2, 0x40ff0000);
                drawTriangle3D_3(pos3, 0x40ffffff, pos4, 0x40ff0000, pos2, 0x40ff0000);
            }
        }

        // Draw the fly height.

        copyVec3(set->points[i]->pos, pos);

        pos[1] = set->points[i]->flyHeight + 0.1;

        drawLine3D(set->points[i]->pos, pos, 0xffff00ff);

        // Draw line to the target.

        if(i == 0){
            drawLine3D(pos, debug_state.aStarRecording.target, (0xff << 24) | rand());
        }

        // Draw a yellow cross at the fly height.

        copyVec3(pos, pos2);

        pos2[0] += 2;
        pos[0] -= 2;

        drawLine3D(pos, pos2, 0xffffff00);

        pos2[0] -= 2;
        pos2[2] += 2;
        pos[0] += 2;
        pos[2] -= 2;

        drawLine3D(pos, pos2, 0xffffff00);

        copyVec3(set->points[i]->pos, lastPos);
    }

    if(showNextPoints){
        int i;
        int otherCount;

        if(!set->foundNextPoints){
            set->foundNextPoints = 1;
            
            if(count > 1){
                for(i = curFrame - 1; i >= 0; i--){
                    AStarRecordingSet* otherSet = debug_state.aStarRecording.sets[i];
                    
                    otherCount = eaSize(&otherSet->points);
                    
                    if(otherCount == count - 1){
                        if(distance3Squared(otherSet->points[0]->pos, set->points[1]->pos) < 0.1){
                            set->parent = otherSet->points[0];
                            break;
                        }
                    }
                }
            }
                        
            for(i = curFrame + 1; i < size; i++){
                AStarRecordingSet* otherSet = debug_state.aStarRecording.sets[i];
                
                otherCount = eaSize(&otherSet->points);
                
                if(otherCount == count + 1){
                    if(distance3Squared(otherSet->points[1]->pos, set->points[0]->pos) < 0.1){
                        eaPush(&set->nextPoints, otherSet->points[0]);
                    }
                }
            }

            otherCount = eaSize(&set->nextConns);
            
            for(i = 0; i < otherCount; i++){
                AStarConnectionInfo* info = set->nextConns[i];
                int j;
                
                for(j = size - 1; j >= 0; j--){
                    AStarRecordingSet* otherSet = debug_state.aStarRecording.sets[j];
                
                    if(distance3Squared(otherSet->points[0]->pos, info->beaconPos) < 0.1){
                        info->point = otherSet->points[0];
                        break;
                    }
                }
            }
        }
        
        otherCount = eaSize(&set->nextPoints);
        
        for(i = 0; i < otherCount; i++){
            AStarPoint* point = set->nextPoints[i];
            
            copyVec3(set->points[0]->pos, pos);
            pos[1] = set->points[0]->flyHeight + 0.1;
            copyVec3(point->pos, pos2);
            pos2[1] = point->flyHeight + 0.1;
            
            drawLine3D_2(pos, 0x80ffffff, pos2, 0x80ff8000);
        }

        otherCount = eaSize(&set->nextConns);
        
        for(i = 0; i < otherCount; i++){
            AStarConnectionInfo* info = set->nextConns[i];

            if(info->point && info->totalCost){
                U32 flashColor = info->flash ? (0xff000000 | (rand() & 255) << 16 | (rand() & 255) << 8) : 0;

                copyVec3(set->points[0]->pos, pos);
                pos[1] = set->points[0]->flyHeight + 0.1;
                copyVec3(info->point->pos, pos2);
                pos2[1] = info->point->flyHeight + 0.1;
                
                if(flashColor){
                    int j;
                    for(j = 0; j < 3; j++){
                        pos[j] += ((rand() % 101) - 50) * 0.001;
                        pos2[j] += ((rand() % 101) - 50) * 0.001;
                    }
                }
                
                drawLine3D_2(pos, flashColor ? flashColor : 0x80ffffff, pos2, flashColor ? flashColor : 0x8000ff00);
            }else{
                U32 flashColor = info->flash ? (0xff000000 | (rand() & 255) << 16 | (rand() & 255) << 8) : 0;

                copyVec3(set->points[0]->pos, pos);
                pos[1] = set->points[0]->flyHeight + 0.1;
                copyVec3(info->beaconPos, pos2);
                
                if(flashColor){
                    int j;
                    for(j = 0; j < 3; j++){
                        pos[j] += ((rand() % 101) - 50) * 0.001;
                        pos2[j] += ((rand() % 101) - 50) * 0.001;
                    }
                }

                drawLine3D_2(pos, flashColor ? flashColor : 0x80ff0000, pos2, flashColor ? flashColor : 0x80ff0000);
            }
        }
    }

    if(!inpIsMouseLocked() && mouse_x >= 0 && mouse_x < w && mouse_y >= 0){
        char buffer[100];
        int widthHeight[2];

        int draw_y = 10 - max(0, sqrt(SQR((mouse_y - h) * 2) + SQR(mouse_x - w/2)) - 400);

        displayEntDebugInfoTextBegin();

        sprintf(buffer,
                "Frame: ^4%d^d/^4%d ^d: ^4%d ^dpoints, ^4%d ^dconns, costs: ^4%d ^dso far, ^4%d ^dto target, ^4%d ^dtotal",
                curFrame + 1,
                size,
                count,
                set->checkedConnectionCount,
                set->costSoFar,
                set->totalCost - set->costSoFar,
                set->totalCost);

        printDebugString(    buffer,
                            0,
                            0,
                            1,
                            7,
                            -1,
                            -1,
                            255,
                            widthHeight);

        drawFilledBox(w / 2 - 10 - max(300, widthHeight[0] / 2), draw_y - 10, w / 2 + 10 + max(300, widthHeight[0] / 2), draw_y + 85, 0x80000000);

        if(debug_state.aStarRecording.frame > 0){
            if(drawButton2D(w / 2 - 235, draw_y, 50, 30, 1, "^5|<", 2, NULL, &first)){
                debug_state.aStarRecording.frame = 0;
            }

            if(drawButton2D(w / 2 - 175, draw_y, 70, 30, 1, "^3100 <<", 1.2, NULL, &prev)){
                debug_state.aStarRecording.frame = max(debug_state.aStarRecording.frame - 100, 0);
            }

            if(drawButton2D(w / 2 - 95, draw_y, 50, 30, 1, "^2<<", 2, NULL, &prev)){
                debug_state.aStarRecording.frame = max(debug_state.aStarRecording.frame - 1, 0);
            }
        }

        drawButton2D(w / 2 - 35, draw_y, 70, 20, 1, "^7Clear", 1, NULL, &clear);

        drawButton2D(w / 2 - 35, draw_y + 25, 70, 20, 1, "^1Off", 1, "beaconpathdebug 0\nsetdebugentity 0", &clear);

        if(drawButton2D(w / 2 + 45, draw_y + 35, 70, 20, 1, showBlockPath ? "^2Block ON" : "^1Block OFF", 1, NULL, NULL)){
            showBlockPath ^= 1;
        }

        if(debug_state.aStarRecording.frame < eaSize(&debug_state.aStarRecording.sets) - 1){
            if(drawButton2D(w / 2 + 45, draw_y, 50, 30, 1, "^2>>", 2, NULL, &next)){
                debug_state.aStarRecording.frame = min(debug_state.aStarRecording.frame + 1, size - 1);
            }

            if(drawButton2D(w / 2 + 105, draw_y, 70, 30, 1, "^3>> 100", 1.2, NULL, &next)){
                debug_state.aStarRecording.frame = min(debug_state.aStarRecording.frame + 100, size - 1);
            }

            if(drawButton2D(w / 2 + 185, draw_y, 50, 30, 1, "^5>|", 2, NULL, &last)){
                debug_state.aStarRecording.frame = size - 1;
            }
        }
        
        printDebugString(    buffer,
                            w / 2 - widthHeight[0] / 2,
                            draw_y + 60,
                            1, 11, -1, -1, 255, NULL);

        if(showNextPoints){
            int i;
            int otherCount;

            if(set->parent){
                sprintf(buffer, "parent: %d", set->parent->set->index + 1);

                if(drawButton2D(w / 2 - 300, 100, 200, 25, 1, buffer, 1, NULL, NULL)){
                    debug_state.aStarRecording.frame = set->parent->set->index;
                }
            }
            
            otherCount = eaSize(&set->nextPoints);
            
            for(i = 0; i < otherCount; i++){
                AStarPoint* point = set->nextPoints[i];
                
                sprintf(buffer, "%d", point->set->index + 1);
                
                if(drawButton2D(w / 2 - 300 + (i % 5) * 110, 130 + (i / 5) * 30, 100, 25, 1, buffer, 1, NULL, NULL)){
                    debug_state.aStarRecording.frame = point->set->index;
                }
            }
            
            otherCount = eaSize(&set->nextConns);
            
            qsort(set->nextConns, otherCount, sizeof(set->nextConns[0]), compareConnTotal);

            for(i = 0; i < otherCount; i++){
                AStarConnectionInfo* info = set->nextConns[i];
                
                sprintf(buffer,
                        "^%d%d/%d: %d, %d",
                        info->point ? 2 : 1,
                        info->point ? info->point->set->index + 1 : 0,
                        info->queueIndex,
                        info->costSoFar,
                        info->totalCost);
                
                if(drawButton2D(150, 5 + i * 25, 165, 25, 1, buffer, 1, NULL, NULL)){
                    if(info->point){
                        debug_state.aStarRecording.frame = info->point->set->index;
                    }
                }
                
                info->flash = mouseOverLastButton();
            }

            sprintf(buffer, "^1Hide ^d(^4%d^d)", eaSize(&set->nextPoints));
        }
        
        if(drawButton2D(w / 2 - 320, draw_y, 70, 30, 1, showNextPoints ? buffer : "^2Show", 1, NULL, NULL)){
            showNextPoints ^= 1;
        }
        
        displayEntDebugInfoTextEnd();

        if(clear){
            eaClearEx(&debug_state.aStarRecording.sets, destroyAStarRecordingSetAdapter);
            eaClearEx(&debug_state.aStarRecording.blocks, destroyAStarBeaconBlockAdapter);
        }
    }
}

static void displayDebugButtons(){
    DebugButton* button;
    int mouse_x, mouse_y;
    int y = 0;
    int w, h;
    int x;

    if(!debug_state.debugButtonCount)
        return;

    inpMousePos(&mouse_x, &mouse_y);

    windowSize(&w, &h);

    if(mouse_x < 0)
        return;

    x = -sqrt(SQR(SQR(max(0, (mouse_x - 200)) * 0.2) * 1.1));

    if(debug_state.menuItem || inpIsMouseLocked())
        return;

    drawFilledBox(x, 0, x + 190, 10 + 20 * debug_state.debugButtonCount, 0xc0000044);

    for(button = debug_state.debugButtonList; button; button = button->next){
        int height = 20;

        drawButton2D(x + 5, y + 5, 180 + min(20, max(0, 40 - 2 * abs((h - mouse_y) - (y + 15)))), height, 0, button->displayText, 1.2, button->commandText, NULL);

        y += height;
    }
}

static int beacConn_cullLines = 1;

static void displayBeaconDebugButtons(){
    if(eaSize(&beaconConnection)){
        int w, h;
        windowSize(&w, &h);
        if(drawButton2D(w / 2 - 50, 5, 100, 25, 1, "Clear Lines", 1, NULL, NULL)){
            eaDestroyEx(&beaconConnection, destroyBeaconDebugLine);
        }
        if(drawButton2D(w / 2 - 75, 35, 150, 25, 1, beacConn_showWhenMouseDown ? "^1Hide ^dOn Mouse" : "^2Show ^dOn Mouse", 1, NULL, NULL)){            
            beacConn_showWhenMouseDown = !beacConn_showWhenMouseDown;
        }
        if(drawButton2D(w / 2 - 75, 65, 150, 25, 1, beacConn_cullLines ? "^1Disable ^dCulling" : "^2Enable ^dCulling", 1, NULL, NULL)){            
            beacConn_cullLines = !beacConn_cullLines;
        }
        if(drawButton2D(w / 2 - 75, 95, 150, 25, 1, (game_state.see_everything & 2) ? "^1Hide ^dLines" : "^2Show ^dLines", 1, NULL, NULL)){            
            if(game_state.see_everything & 2){
                game_state.see_everything &= ~2;
            }else{
                game_state.see_everything |= 2;
            }
        }
    }
}

static void displayClientStuff(){
    static int showthis = 1;
    char buffer[1000];

    if(game_state.ent_debug_client & (1 | 2 | 8)){
        int y = 7 - 15;
        
        if(showthis){
            Entity* player = playerPtr();
            MotionState* motion = player->motion;
            
            drawFilledBox(2, 2, 301, 151, 0xd0ffffff);
            drawFilledBox(3, 3, 300, 150, 0xd0442233);

            sprintf(buffer,
                    "Time: ^4%d:%2.2d^0, ABS TIME: ^4%d",
                    (int)server_visible_state.time,
                    (int)(fmod(server_visible_state.time,1) * 60),
                    global_state.client_abs);

            printDebugString(    buffer,
                                5,
                                y += 15,
                                1,
                                11,
                                -1,
                                0,
                                255,
                                NULL);

            sprintf(buffer, "Yaw: ^4%1.3f^0, Pitch: ^4%1.3f", control_state.pyr.cur[1], control_state.pyr.cur[0]);

            printDebugString(    buffer,
                                5,
                                y += 15,
                                1,
                                11,
                                -1,
                                0,
                                255,
                                NULL);

            sprintf(buffer,
                    lengthVec3(motion->vel) ?
                        "Pos: ^4%1.3f^0, ^4%1.3f^0, ^4%1.3f ^d(^4%1.3f^0, ^4%1.3f^0, ^4%1.3f^d) %1.3f (%1.3fxz)" :
                        "Pos: ^4%1.3f^0, ^4%1.3f^0, ^4%1.3f",
                    ENTPOSPARAMS(player),
                    vecParamsXYZ(motion->vel),
                    lengthVec3(motion->vel),
                    lengthVec3XZ(motion->vel));

            printDebugString(    buffer,
                                5,
                                y += 15,
                                1,
                                11,
                                -1,
                                0,
                                255,
                                NULL);

            if(current_target){
                sprintf(buffer, "Target Dist: ^4%1.2f\n", distance3(cam_info.cammat[3], ENTPOS(current_target)));
                printDebugString(    buffer,
                                    5,
                                    y += 15,
                                    1,
                                    11,
                                    -1,
                                    0,
                                    255,
                                    NULL);

                sprintf(buffer, "Target LOD (^3%s (%s:%d)^0, ^4%d^0, NPC %s): ^4%1.2f^0, ^4%1.2f^0, ^4%1.2f^0, ^4%1.2f\n",
                        current_target->seq->type->name,
                        current_target->seq->info->moves[current_target->move_idx]->name,
                        current_target->move_idx,
                        current_target->seq->lod_level,
                        npcDefList.npcDefs[current_target->npcIndex]->name,
                        current_target->seq->type->loddist[0],
                        current_target->seq->type->loddist[1],
                        current_target->seq->type->loddist[2],
                        current_target->seq->type->loddist[3]);
                printDebugString(    buffer,
                                    5,
                                    y += 15,
                                    1,
                                    11,
                                    -1,
                                    0,
                                    255,
                                    NULL);

                sprintf(buffer, "Target Pos: ^4%f^0, ^4%f^0, ^4%f ^0(^4%d^0, ^4%d^0, ^4%d^0)\n",
                        ENTPOSPARAMS(current_target),
                        vecParamsXYZ(current_target->net_ipos));
                printDebugString(    buffer,
                                    5,
                                    y += 15,
                                    1,
                                    11,
                                    -1,
                                    0,
                                    255,
                                    NULL);

                {
                    sprintf(buffer,
                            "Target Capsule: ^4%1.3f^0, ^4%1.3f^0, ^4%d\n",
                            current_target->motion->capsule.length,
                            current_target->motion->capsule.radius,
                            current_target->motion->capsule.dir);
                    printDebugString(    buffer,
                                        5,
                                        y += 15,
                                        1,
                                        11,
                                        -1,
                                        0,
                                        255,
                                        NULL);
                }

                sprintf(buffer, "Target: ^1%s^0, ID ^4%d^0, Server ID ^4%d\n",
                        current_target->name,
                        current_target->owner,
                        current_target->svr_idx);
                printDebugString(    buffer,
                                    5,
                                    y += 15,
                                    1,
                                    11,
                                    -1,
                                    0,
                                    255,
                                    NULL);

                if(current_target->coll_tracker){
                    sprintf(buffer, "Tracker Pos: ^4%1.3f^0, ^4%1.3f^0, ^4%1.3f\n",
                            posParamsXYZ(current_target->coll_tracker));
                    printDebugString(    buffer,
                                        5,
                                        y += 15,
                                        1,
                                        11,
                                        -1,
                                        0,
                                        255,
                                        NULL);
                }
            }else{
                sprintf(buffer,
                        "speed: x:^4%1.3f^0, y:^4%1.3f^0, z:^4%1.3f^0, back:^4%1.3f",
                        vecParamsXYZ(control_state.server_state->speed),
                        control_state.server_state->speed_back);
                printDebugString(    buffer,
                                    5,
                                    y += 15,
                                    1,
                                    11,
                                    -1,
                                    0,
                                    255,
                                    NULL);

                sprintf(buffer,
                        "surf: x:^4%1.3f^0, y:^4%1.3f^0, z:^4%1.3f^0",
                        vecParamsXYZ(motion->surf_normal));
                printDebugString(    buffer,
                                    5,
                                    y += 15,
                                    1,
                                    11,
                                    -1,
                                    0,
                                    255,
                                    NULL);
            }
        }
        
        {
            int mouse_x, mouse_y;
            int w, h;
            inpMousePos(&mouse_x, &mouse_y);
            windowSize(&w, &h);
            if(drawButton2D(310, 10 - max(0, sqrt(SQR(mouse_x - 350) + SQR(mouse_y - h)) - 100), 40, 40, 1, showthis ? "^2show" : "^1hide", 1, NULL, NULL)){
                showthis ^= 1;
            }
            if(drawButton2D(360, 10 - max(0, sqrt(SQR(mouse_x - 350) + SQR(mouse_y - h)) - 100), 40, 40, 1, "^1OFF", 1, NULL, NULL)){
                game_state.ent_debug_client = 0;
            }
        }
    }
}

static char * cam_name = "CutsceneCamera";

static int debugCameraAdd(GroupDef *def, DefTracker *tracker, Mat4 world_mat, void *draw)
{
    if( tracker && tracker->def->properties )
    {
        PropertyEnt *prop;
        stashFindPointer( tracker->def->properties, "CutSceneCamera", &prop );
        if (prop)
        {
            DebugCameraInfo* info = &debug_state.debugCamera;
            DebugCamera* cam;
            
            // After the Camera has been placed, give it a unique camera value
            // We pick up the new index when the mapserver processes the update
            if(stricmp(prop->value_str, "-1")==0)
            {
                char camNum[4];
                itoa( info->cameras.previousSize++, camNum, 10 );
                unSelect(2);
                selAdd(tracker,trackerRootID(tracker),0,0);
                addPropertiesToSelectedTrackers( "CutSceneCamera", camNum, NULL );

                
                if(!tracker->def->properties){
                    tracker->def->properties = stashTableCreateWithStringKeys(16,  StashDeepCopyKeys);
                    tracker->def->has_properties = 1;
                }
                prop = createPropertyEnt();
                strcpy(prop->name_str, "CutSceneCamera");
                estrPrintCharString(&prop->value_str, "-1");
                stashAddPointer(tracker->def->properties, prop->name_str, prop, false);
                unSelect(2);
            }

            dynArrayAdd(&info->cameras.camera, sizeof(info->cameras.camera[0]),    &info->cameras.size, &info->cameras.maxSize, 1);
            cam = info->cameras.camera + info->cameras.size - 1;
            cam->tracker = tracker;
            copyMat4( world_mat, cam->mat ); 
        }
    }

    return 1;
}

void updateCutSceneCameras()
{
    DebugCameraInfo* info = &debug_state.debugCamera;
    int previousSize = info->cameras.previousSize;

    memset( info->cameras.camera, 0, info->cameras.size*sizeof(DebugCamera));
    info->cameras.size=0;
    info->cameras.previousSize = previousSize;
    groupTreeTraverse( debugCameraAdd, 0 );

    info->cameras.previousSize = info->cameras.size;
}

static void displayDebugCameraEditor()
{
    static int hideTitle;
    DebugCameraInfo* info = &debug_state.debugCamera; 
    DebugCamera* cam;
    char buffer[1000];
    int i;
    int w, h;
    int widthHeight[2];
    int x;
    int y;
    int design_placement_mode = (info->editMode==2);

    if(!info->editMode || debug_state.menuItem)
        return;

    // preload camera object so its instantly placeable
    if( !sel.lib_load && !groupDefFind(cam_name) )
    {
        sel.lib_load = 1;
        editDefLoad(cam_name);
        return;
    }
    else
        sel.lib_load = 0;

    windowSize(&w, &h);
    if(!hideTitle)
    {
        if( design_placement_mode )
        {
              printDebugString("^2Design Camera Cutscene", 0, 0, 5, 1, -1, -1, 192, widthHeight);
            printDebugString("^2Design Camera Cutscene", w/2 - widthHeight[0]/2, h*0.75 - widthHeight[1]/2, 5, 1, -1, -1, 192, NULL);
        }
        else
        {
            printDebugString("^2Debug Camera Edit Mode", 0, 0, 5, 1, -1, -1, 192, widthHeight);
            printDebugString("^2Debug Camera Edit Mode", w/2 - widthHeight[0]/2, h*0.75 - widthHeight[1]/2, 5, 1, -1, -1, 192, NULL);
        }
    }
    
    if(drawButton2D(5, 5, 85, 25, 1, "^2New Camera", 1, NULL, NULL))
    {
        dynArrayAdd(&info->cameras.camera, sizeof(info->cameras.camera[0]),&info->cameras.size, &info->cameras.maxSize, 1 );
        cam = info->cameras.camera + info->cameras.size - 1;
        ZeroStruct(cam);
        copyMat4(cam_info.cammat, cam->mat);
        info->curCamera = info->cameras.size - 1;
        if( design_placement_mode )
        {
            // This load object into a fake tracker then pastes it
            loadObject( cam_name );
            copyMat4(unitmat,sel.offsetnode->mat);
            copyMat4( cam_info.cammat, sel.parent->mat);
            editPaste();
        }
    }
    
    if(drawButton2D(95, 5, 85, 25, 1, "^1Exit Editor", 1, NULL, NULL))
    {
        info->editMode = 0;
        return;
    }
    
    if(drawButton2D(185, 5, 85, 25, 1, hideTitle ? "^1Show Title" : "^2Hide Title", 1, NULL, NULL))
        hideTitle = !hideTitle;


    if( design_placement_mode )
    {
        if(drawButton2D(275, 5, 85, 25, 1, "Save Map File", 1, NULL, NULL))
            editCmdSaveFile();
    }
    else
    {
        if(drawButton2D(275, 5, 85, 25, 1, info->on ? "Camera ^2ON" : "Camera ^1OFF", 1, NULL, NULL))
            info->on = !info->on;
    }

    if(drawButton2D(460, 5, 85, 25, 1, game_state.forceCutSceneLetterbox ? "Letterbox ^1OFF" : "Letterbox ^2ON", 1, NULL, NULL))
            game_state.forceCutSceneLetterbox = !game_state.forceCutSceneLetterbox;

    for(i = 0; i < info->cameras.size; i++)
    {
        int y = 35 + 30 * i;
        PropertyEnt* prop=0;

        cam = info->cameras.camera + i;
        
        if( cam->tracker && cam->tracker->def && cam->tracker->def->properties )
            stashFindPointer( cam->tracker->def->properties, "CutSceneCamera", &prop );    

        if( design_placement_mode )
        {
            if( prop )
                sprintf(buffer, "%sCamera%s", i == info->curCamera ? "^r" : "", prop->value_str);
            else
                sprintf(buffer, "%s*NOT PLACED*", i == info->curCamera ? "^r" : "");
        }
        else
            sprintf(buffer, "%sCamera ^4%d", i == info->curCamera ? "^r" : "", i + 1);

        if(drawButton2D(5, y, 100, 25, 1, buffer, 1, NULL, NULL))
            info->curCamera = i;
        
        
        if(drawButton2D(110, y, 25, 25, 1, "^1X", 1.5, NULL, NULL)) 
        {
            if( design_placement_mode && cam->tracker )
            {
                unSelect(1);
                selAdd(cam->tracker,trackerRootID(cam->tracker),0,0);
                editCmdDelete();
            }

            CopyStructs(cam, info->cameras.camera + i + 1, info->cameras.size - i - 1);
            if(info->curCamera >= i)
                info->curCamera--;

            info->cameras.size--;
            i--;
            continue;
        }
        
        if(i == info->curCamera)
        {
            if(drawButton2D(140, y, 50, 25, 1, info->hideInEditor ? "^2Show" : "^1Hide", 1, NULL, NULL))
            {
                info->hideInEditor = !info->hideInEditor;
                    if( design_placement_mode && info->hideInEditor )
                {
                    Vec3 cam_pyr, cam_pos;
                      Entity * player = playerPtr();

                    getMat3YPR(cam->mat,cam_pyr);
                    cam_pyr[0] *= -1;
                    cam_pyr[1] = addAngle(cam_pyr[1], RAD(180));

                     copyVec3( cam->mat[3], cam_pos );
                    cam_pos[1] -= camGetPlayerHeight();

                    cmdParsef( "setpospyr %f %f %f %f %f %f", vecParamsXYZ(cam_pos), vecParamsXYZ(cam_pyr) );
                    
                    copyVec3( cam_pyr, control_state.pyr.cur );
                    camSetPyr( cam_pyr ); 

                    //entUpdatePosInstantaneous(player,cam->mat[3]);
                    //camSetPos( cam->mat[3], 0 );
                    game_state.camdist = 0;
                }
            }
        
            if( design_placement_mode )
            {
                if(drawButton2D(195, y, 110, 25, 1, prop?"Move":"Place", 1, NULL, NULL)) 
                {
                    if( prop ) 
                    {
                        unSelect(1);
                        selAdd(cam->tracker,trackerRootID(cam->tracker),0,0);

                        copyMat4(cam_info.cammat, cam->mat);
                        
                        edit_state.cut = 1;
                        editCmdCutCopy();
                        copyMat4(unitmat,sel.offsetnode->mat);
                        copyMat4(cam->mat, sel.parent->mat);
                        editPaste();
                    }
                }
            }
            else
            {
                if(drawButton2D(195, y, 110, 25, 1, "Take Current Pos", 1, NULL, NULL))
                {
                    copyMat4(cam_info.cammat, cam->mat);
                }
            }
        }
    }
    
    if(!info->cameras.size)
        return;
    
    info->curCamera = MINMAX(info->curCamera, 0, info->cameras.size - 1);
    cam = info->cameras.camera + info->curCamera;
    
    x = 400;
    y = 5;
    
    // Select myself.
     if( !design_placement_mode )
    {
        if(drawButton2D(x, y, 85, 25, 1, "Select Self", 1, NULL, NULL))
            current_target = playerPtr();
    
        // Selected entity name.
        sprintf(buffer, "Current Target: ");
        
        if(current_target)
            sprintf(buffer + strlen(buffer), "^7%s ^0(^4%d^0)", current_target->name, current_target->svr_idx);
        else
            sprintf(buffer + strlen(buffer), "^1NONE");
        
        
        printDebugString(buffer, x + 90, y + 8, 1, 1, -1, -1, 255, NULL);
        
        y += 30;
        
        // Relative entity.
        
        if(cam->relativeToThisEntity || current_target)
        {
            if(!cam->relativeToThisEntity)
            {
                sprintf(buffer, "^1Click to enable RELATIVE mode");
            }
            else
            {
                Entity* e = validEntFromId(cam->relativeToThisEntity);
                
                sprintf(buffer, "^2Relative: ^4%d", cam->relativeToThisEntity);
                
                if(e)
                    sprintf(buffer + strlen(buffer), " ^0(^7%s^0)", e->name);
                else
                    sprintf(buffer + strlen(buffer), " ^0(^1NOT VISIBLE^0)");
                
            }

            if(drawButton2D(x, y, 300, 25, 1, buffer, 1, NULL, NULL))
            {
                if(!cam->relativeToThisEntity && current_target)
                {
                    Vec3 temp;
                    Vec3 pyr;
                    cam->relativeToThisEntity = current_target->svr_idx;
                    subVec3(cam_info.cammat[3], ENTPOS(current_target), posPoint(cam));
                    scaleVec3(posPoint(cam), -1, temp);
                    ZeroStruct(pyr);
                    getVec3YP(temp, &pyr[1], &pyr[0]);
                    createMat3YPR(cam->mat, pyr);
                    scaleVec3(cam->mat[0], -1, cam->mat[0]);
                    scaleVec3(cam->mat[2], -1, cam->mat[2]);
                }
            }
            
            if(cam->relativeToThisEntity)
            {
                if(drawButton2D(x + 305, y, 25, 25, 1, "^1X", 1.5, NULL, NULL))
                    cam->relativeToThisEntity = 0;
                
                
                if(drawButton2D(x + 335, y, 50, 25, 1, "Center", 1, NULL, NULL))
                    zeroVec3(posPoint(cam));
            }
        }
            
        y += 30;
        
        // Tracked entity.
        
        if(cam->trackThisEntity || current_target)
        {
            if(!cam->trackThisEntity)
            {
                sprintf(buffer, "^1Click to enable TRACKING mode");
            }
            else
            {
                Entity* e = validEntFromId(cam->trackThisEntity);
                
                sprintf(buffer, "^2Tracked: ^4%d", cam->trackThisEntity);
                
                if(e)
                    sprintf(buffer + strlen(buffer), " ^0(^7%s^0)", e->name);
                else
                    sprintf(buffer + strlen(buffer), " ^0(^1NOT VISIBLE^0)");
                
            }

            if(drawButton2D(x, y, 300, 25, 1, buffer, 1, NULL, NULL))
            {
                if(!cam->trackThisEntity && current_target)
                    cam->trackThisEntity = current_target->svr_idx;
                
            }
            
            if(cam->trackThisEntity && drawButton2D(x + 305, y, 25, 25, 1, "^1X", 1.5, NULL, NULL))
            {
                cam->trackThisEntity = 0;
                copyMat4(cam_info.cammat, cam->mat);
            }
        }
    }
    y += 30;
}

int getDebugCameraPosition(CameraInfo* caminfo)
{
    DebugCameraInfo* info = &debug_state.debugCamera;
    DebugCamera* cam;
    Entity* e;

    if(    !info->cameras.size ||
        !(info->on && !info->editMode) && !(info->editMode && !info->hideInEditor))
    {
        if(!info->cameras.size){
            info->on = 0;
        }
        
        return 0;
    }
    
    info->curCamera = MINMAX(info->curCamera, 0, info->cameras.size - 1);
    
    cam = info->cameras.camera + info->curCamera;
    
    copyMat4(cam->mat, caminfo->cammat);
    
    // Relative entity.
    
    e = validEntFromId(cam->relativeToThisEntity);
    
    if(e){
        //Vec3 temp;
        mulMat4(ENTMAT(e), cam->mat, caminfo->cammat);
        //addVec3(posPoint(e), posPoint(cam), caminfo->cammat[3]);
    }
    else if(cam->relativeToThisEntity){
        return 0;
    }
    
    // Tracked entity.
    
    e = validEntFromId(cam->trackThisEntity);

    if(e){
        Vec3 y_axis = {0, 1, 0};
        Vec3 temp;
        copyVec3(ENTPOS(e), temp);
        temp[1] += 5;
        subVec3(temp, caminfo->cammat[3], temp);
        normalVec3(temp);
        copyVec3(temp, caminfo->cammat[2]);
        crossVec3(y_axis, temp, caminfo->cammat[0]);
        normalVec3(caminfo->cammat[0]);
        crossVec3(temp, caminfo->cammat[0], caminfo->cammat[1]);
        normalVec3(caminfo->cammat[1]);

        scaleVec3(caminfo->cammat[0], -1, caminfo->cammat[0]);
        scaleVec3(caminfo->cammat[2], -1, caminfo->cammat[2]);
    }
    
    return 1;
}

void receiveDebugPower(Packet *pak)
{
    debug_state.debugPower = basepower_Receive(pak, &g_PowerDictionary, playerPtr());
}


static void displayDebugPowerInfo(void)
{
    const BasePower * ppow;
    Entity *e = playerPtr();
    Vec3 min, max;
    Mat4 loc_mat;
    Vec3 loc_pos;
    int i;

    if(!debug_state.debugPower)
        return; 

     ppow = debug_state.debugPower;
    copyMat4( unitmat, loc_mat );

    if (ppow->eTargetType == kTargetType_Location) // Location Based Powers
    {
        CollInfo coll;
        Vec3 start, end;
        gfxCursor3d( (F32)gMouseInpCur.x, (F32)gMouseInpCur.y, 1000, start, end );

        // if the line collided with anything, get the collision point, otherwise use the end of the line
        if (collGrid(0,start,end,&coll,0,COLL_DISTFROMSTART|COLL_HITANY))
        {
            copyMat4( coll.mat, loc_mat);
            copyVec3( loc_mat[3], loc_pos );
        }
        else
        {
            copyVec3( end, loc_mat[3]);
            copyVec3( loc_mat[3], loc_pos );
        }
    }
    // Targeted AoE
    else if( current_target && ppow->eTargetType != kTargetType_Caster && ppow->eTargetType != kTargetType_MyOwner 
        && ppow->eTargetType != kTargetType_MyCreator && ppow->eEffectArea == kEffectArea_Sphere)
    {
        copyMat4( ENTMAT(current_target), loc_mat );
        copyVec3( loc_mat[3], loc_pos );
    }
    else // Self AoE or Targeted attack
    {
        copyMat4( ENTMAT(e), loc_mat );
        copyVec3( loc_mat[3], loc_pos );
    }


    switch( ppow->eEffectArea )
    {
        xcase kEffectArea_Character:
            copyMat4( ENTMAT(e), loc_mat );
            drawSpinningCircle( ppow->fRange, loc_mat, CLR_GREEN );
            // Any targeted entity
        xcase kEffectArea_Cone:
            loc_mat[3][1] += 2;
            drawSpinningCircle( ppow->fRadius, loc_mat, CLR_GREEN );
 
            if( current_target )// A cone centered around the ray connecting the source to the target.
            {
                Mat3 dirmat;
                Vec3 vecDir, vecPos;

                 copyVec3( ENTPOS(current_target), vecPos );
                copyVec3( ENTPOS(e), loc_pos );
                loc_pos[1] += 2;
                vecPos[1] += 2;
                  drawLine3D( loc_pos, vecPos, 0xffffff00);

                 subVec3(ENTPOS(current_target),loc_pos, vecDir);
                vecDir[1] = 0;
                normalVec3(vecDir);
                copyMat3( unitmat, dirmat );
                orientMat3( dirmat, vecDir );
                yawMat3( ppow->fArc/2, dirmat );
                 
                copyVec3( loc_pos, vecPos );
                moveVinZ( vecPos, dirmat, ppow->fRadius );
                drawLine3D( loc_pos, vecPos, 0xffffff00);
                
                orientMat3( dirmat, vecDir );
                yawMat3( -ppow->fArc/2, dirmat );
                copyVec3( loc_pos, vecPos );
                moveVinZ( vecPos, dirmat, ppow->fRadius );
                drawLine3D( loc_pos, vecPos, 0xffffff00);
            }

            for(i=0;i<eaSize(&ppow->ppTemplates);i++)
            {
                const AttribModTemplate *pTemplate = ppow->ppTemplates[i];
                 if( pTemplate->fRadiusInner > 0 )
                      drawSpinningCircle( pTemplate->fRadiusInner, loc_mat, CLR_WHITE );
                if( pTemplate->fRadiusOuter > 0 )
                     drawSpinningCircle( pTemplate->fRadiusOuter, loc_mat, CLR_WHITE );
            }
        xcase kEffectArea_Sphere:
            drawSpinningCapsule( ppow->fRadius, 0, loc_mat, CLR_GREEN );
            for(i=0;i<eaSize(&ppow->ppTemplates);i++)
            {
                const AttribModTemplate *pTemplate = ppow->ppTemplates[i];
                if( pTemplate->fRadiusInner > 0 )
                    drawSpinningCircle( pTemplate->fRadiusInner, loc_mat, CLR_WHITE );
                if( pTemplate->fRadiusOuter > 0 )
                    drawSpinningCircle( pTemplate->fRadiusOuter, loc_mat, CLR_WHITE );
            }
            // A sphere surrounding the target.
        xcase kEffectArea_Location:
            drawSpinningCapsule( MAX(1,ppow->fRadius), 0, loc_mat, CLR_GREEN );
            // A single spot on the ground.
        xcase kEffectArea_Box:
            addVec3( loc_pos, ppow->vecBoxOffset, min );
            addVec3( min, ppow->vecBoxSize, max );
            drawBox3D(min, max, loc_mat, 0xffff0f0f, 2);
    }
}


extern unsigned int g_packetsizes_one[33];
extern unsigned int g_packetsizes_success[33];
extern unsigned int g_packetsizes_failed[33];

static void showPackedArray(unsigned int array[33], int off, char* name)
{
    int i, max = 0;
    char buf[20];
    F32 height, height2;

    printDebugString(name, 150, 50+off, 1.0, 10, 0, 0xff00ff00, 255, NULL);
    for (i = 0; i < 33; i++)
        if (array[i] > (U32)max) max = array[i];
    for (i = 0; i < 33; i++)
    {
        sprintf(buf, "%i", i);
        height = 100.0 * (F32)array[i] / max;
        height2 = height * i;
        if (height2 > 100.0)
            height2 = 100.0;
        
        drawFilledBox(i*10, off+10, i*10+3, height+off+10, 0xff00ff00);
        if (height2 > height)
            drawFilledBox(i*10, height+off+10, i*10+3, height2+off+10, 0xffffff00);
        if (i % 2)
            printDebugString(buf, i*10-3, off, 1.0, 10, 0, 0xff00ff00, 255, NULL);
    }
}

static void displayBitGraph()
{
    if (!game_state.bitgraph || game_state.disable2Dgraphics)
        return;

    showPackedArray(g_packetsizes_success, 0, "success");
    showPackedArray(g_packetsizes_failed, 120, "failure");
    showPackedArray(g_packetsizes_one, 240, "pack(1)");
}

static void displayNetGraph()
{
    int        i,xpos;
    F32        height;
    int        w, h;

    if (!game_state.netgraph || game_state.disable2Dgraphics)
        return;

    fontSet(0);

    windowSize(&w, &h);

    for(i=0;i<ARRAY_SIZE(recv_hist);i++)
    {
        int argb;

        if(game_state.netgraph == 1)
        {
            height = recv_hist[i].time * 40;
            if (height > 40)
                height = 40;
        }
        else
        {
            height = recv_hist[i].bytes / 10;
            if (height <= 0)
                height = 1;
        }

        switch(recv_hist[i].type){
            case NST_LostOutgoingPacket:
                argb = 0xffffff00;
                break;
            case NST_LostIncomingPacket:
                argb = 0xffff0000;
                break;
            default:
                argb = 0xff00ff00;
        }

        xpos = ARRAY_SIZE(recv_hist) - (ARRAY_SIZE(recv_hist) - 1 + i - recv_idx) % ARRAY_SIZE(recv_hist);

        drawLine(w - xpos, 0, w - xpos + 1, height, argb);
    }
    xyprintf(TEXT_JUSTIFY+61,TEXT_JUSTIFY+49,"DUPLICATE IN : %d",comm_link.duplicate_packet_recv_count);
    xyprintf(TEXT_JUSTIFY+61,TEXT_JUSTIFY+50,"RETRANSMITTED: %d",comm_link.lost_packet_sent_count);
    xyprintf(TEXT_JUSTIFY+61,TEXT_JUSTIFY+51,"LOST IN      : %d",comm_link.lost_packet_recv_count);
    xyprintf(TEXT_JUSTIFY+68,TEXT_JUSTIFY+52,"PING: %d",pingAvgRate(&comm_link.pingHistory));
    xyprintf(TEXT_JUSTIFY+68,TEXT_JUSTIFY+53,"SEND: %d",pktRate(&comm_link.sendHistory));
    xyprintf(TEXT_JUSTIFY+68,TEXT_JUSTIFY+54,"RECV: %d",pktRate(&comm_link.recvHistory));
}

typedef struct PhysicsRecordingUIInfo {
    S32        curPos;
    
    S32        fadeOutLo;
    S32        fadeOutHi;
    S32        fadeOutAlpha;
    
    U32        visible            : 1;
    U32        visible3D        : 1;
    U32        doNotStayAtEnd    : 1;
    U32        syncToMe        : 1;
    U32        hilight            : 1;
} PhysicsRecordingUIInfo;

static PhysicsRecordingUIInfo* getPhysicsRecordingUIInfo(const char* hashName)
{
    static StashTable ht;
    
    StashElement element;
    
    if(!ht)
    {
        ht = stashTableCreateWithStringKeys(10, StashDeepCopyKeys);
        element = NULL;
    }
    
    stashFindElement(ht, hashName, &element);
    
    if(!element)
    {
        stashAddPointerAndGetElement(ht, hashName, calloc(sizeof(PhysicsRecordingUIInfo), 1), false, &element);
    }
    
    return stashElementGetPointer(element);
}

static struct {
    PhysicsRecording**    rec;
    int                    count;
    int                    maxCount;
} phys_recs;

static int gatherPhysicsRecordings(StashElement element){
    if(stashElementGetPointer(element)){
        dynArrayAddp(&phys_recs.rec, &phys_recs.count, &phys_recs.maxCount, stashElementGetPointer(element));
    }
    
    return 1;
}

static int compareRecordingName(const void* rec1Data, const void* rec2Data)
{
    const PhysicsRecording** rec1 = (const PhysicsRecording**)rec1Data;
    const PhysicsRecording** rec2 = (const PhysicsRecording**)rec2Data;
    return stricmp((*rec1)->name, (*rec2)->name);
}

static void displayPhysicsRecording(int show3D){
    static S32 syncToCSC = -1;
    static S32 lastTime;
    static S32 hideVel;
    static S32 hideInputVel;
    static S32 showUI = 1;
    
    char buffer[1000];

    int goPrev1 = 0;
    int goPrev2 = 0;
    int goPrev3 = 0;
    int goNext1 = 0;
    int goNext2 = 0;
    int goNext3 = 0;
    
    int clearAll = 0;
    int visibleY = 0;

    char* printName = NULL;
    S32 curTime;
    int w, h;
    int mouse_x, mouse_y;
    int i;
    
    if(debug_state.menuItem){
        return;
    }
    
    curTime = timeGetTime();

    if(!lastTime){
        lastTime = curTime;
    }
    
    windowSize(&w, &h);
    inpLastMousePos(&mouse_x, &mouse_y);
    
    phys_recs.count = 0;
    
    if ( global_pmotion_state.htPhysicsRecording ) stashForEachElement(global_pmotion_state.htPhysicsRecording, gatherPhysicsRecordings);
    
    qsort(phys_recs.rec, phys_recs.count, sizeof(phys_recs.rec[0]), compareRecordingName);
    
    for(i = 0; i < phys_recs.count; i++){
        PhysicsRecording* rec = phys_recs.rec[i];
        PhysicsRecordingUIInfo* ui = getPhysicsRecordingUIInfo(rec->name);
        PhysicsRecordingStep* step;
        int j;
        
        if(!ui->doNotStayAtEnd){
            ui->curPos = rec->steps.count - 1;
        }
        else if(syncToCSC > 0){
            for(j = 0; j < rec->steps.count; j++){
                if(rec->steps.step[j].csc_net_id == syncToCSC){
                    ui->curPos = j;
                    break;
                }
            }
        }
        
        ui->curPos = MINMAX(ui->curPos, 0, max(0, rec->steps.count - 1));

        step = rec->steps.count ? rec->steps.step + ui->curPos : NULL;

        if(show3D){
            if(step && ui->visible && ui->visible3D){
                Vec3 temp;
                Vec3 temp2;
                U32 overrideColor = (ui->hilight && !(global_state.global_frame_count & 4)) ? 0xffffff00 : 0;
                
                #define COLOR(x) (overrideColor ? overrideColor : x)
                
                if(ui->fadeOutAlpha > 0){
                    for(j = ui->fadeOutLo; j <= ui->fadeOutHi; j++){
                        if(j >= 0 && j < rec->steps.count){
                            PhysicsRecordingStep* fadeStep = rec->steps.step + j;
                            int alpha = (ui->fadeOutAlpha / 4) << 24;
                            drawLine3D_2(fadeStep->before.pos, alpha | 0xffffff, fadeStep->after.pos, alpha | 0xff0000);
                        }
                    }
                    
                    ui->fadeOutAlpha -= (curTime - lastTime);
                }
                
                drawLine3D_2(step->before.pos, COLOR(0xffffffff), step->after.pos, COLOR(0xffff0000));
                
                copyVec3(step->before.pos, temp);
                vecY(temp) -= 20;
                drawLine3D_2(step->before.pos, COLOR(0x40ffffff), temp, COLOR(0x40ffffff));
                
                copyVec3(step->after.pos, temp);
                vecY(temp) -= 20;
                drawLine3D_2(step->after.pos, COLOR(0x40ff0000), temp, COLOR(0x40ff0000));

                if(!hideVel){
                    copyVec3(step->before.pos, temp);
                    addVec3(temp, step->before.motion.vel, temp);
                    drawLine3D_2(step->before.pos, COLOR(0xffffffff), temp, COLOR(0xff00ffff));
                }
                
                if(!hideInputVel && lengthVec3Squared(step->before.motion.input.vel)){
                    addVec3(step->before.pos, step->before.motion.input.vel, temp);
                    drawLine3D_2(step->before.pos, COLOR(0xffffffff), temp, COLOR(0xff00ff00));

                    if(lengthVec3SquaredXZ(step->before.motion.input.vel)){
                        copyVec3(temp, temp2);
                        vecY(temp2) -= 20;
                        drawLine3D_2(temp, COLOR(0x4000ff00), temp2, COLOR(0x4000ff00));
                    }
                }
                
                #undef COLOR
            }else{
                ui->fadeOutAlpha = 0;
            }
        }else{
            int y = visibleY + 5;
            int oldCurPos;

            if(ui->syncToMe){
                ui->syncToMe = 0;
                syncToCSC = -1;
            }
            
            if(!visibleY && (showUI || rec->steps.count)){
                drawFilledBox(w / 2 - 405, visibleY ? y : visibleY,  w / 2 + 400, visibleY + 35, 0xc0000000);
                
                goPrev1 = drawDefaultButton2D(w / 2 - 130, y, 25, 25, "^2<");
                goPrev2 = drawDefaultButton2D(w / 2 - 160, y, 25, 25, "^1<<\n10");
                goPrev3 = drawDefaultButton2D(w / 2 - 190, y, 25, 25, "^7<<\n100");
                goNext1 = drawDefaultButton2D(w / 2 + 105, y, 25, 25, "^2>");
                goNext2 = drawDefaultButton2D(w / 2 + 135, y, 25, 25, "^1>>\n10");
                goNext3 = drawDefaultButton2D(w / 2 + 165, y, 25, 25, "^7>>\n100");
                
                if(drawDefaultButton2D(w / 2 - 400, y, 55, 25, "^1clear all")){
                    pmotionUpdateNetID(NULL);
                    clearAll = 1;
                }
                
                if(drawDefaultButton2D(w / 2 - 340, y, 25, 25, hideVel ? "^1^svel" : "^2vel")){
                    hideVel ^= 1;
                }
                
                if(drawDefaultButton2D(w / 2 - 310, y, 45, 25, hideInputVel ? "^1^sinpVel" : "^2inpVel")){
                    hideInputVel ^= 1;
                }
                
                if(drawDefaultButton2D(w / 2 + 300, y, 75, 25, control_state.record_motion ? "^1pause" : "^2record")){
                    cmdParse(control_state.record_motion ? "recordmotion 0" : "recordmotion 1");
                }
                
                if(!control_state.record_motion){
                    if(drawDefaultButton2D(w / 2 + 220, y, 75, 25, showUI ? "^1hide empty" : "^2show empty")){
                        showUI = !showUI;
                    }
                }else{
                    showUI = 1;
                }
                
                visibleY += 30;
            }
            
            if(clearAll){
                SAFE_FREE(rec->steps.step);
                ZeroStruct(&rec->steps);
            }
            
            if(!rec->steps.count){
                continue;
            }
            
            y = visibleY + 5;

            if(    !inpIsMouseLocked() &&
                ui->visible &&
                mouse_x >= w / 2 - 405 &&
                mouse_x < w / 2 + 400 &&
                h - mouse_y >= visibleY &&
                h - mouse_y < visibleY + 30)
            {
                ui->hilight = 1;
                printName = rec->name;
            }
            else
            {
                ui->hilight = 0;
            }
            
            drawFilledBox(w / 2 - 405, visibleY ? y : visibleY, w / 2 + 400, visibleY + 35, ui->hilight ? 0xc0008888 : 0xc0000000);
            
            if(drawDefaultButton2D(w / 2 - 400, y, 25, 25, ui->visible ? "^2ON" : "^1off")){
                ui->visible ^= 1;
            }
            
            if(!ui->visible){
                sprintf(buffer, "^%d%s ^d(^4%d^d)", ui->curPos == rec->steps.count - 1 ? 1 : 7, rec->name, rec->steps.count);
                printDebugString(buffer, w / 2 - 370, y + 6, 1, 11, -1, -1, 255, NULL);
                visibleY += 30;
                continue;
            }
            
            if(drawDefaultButton2D(w / 2 - 370, y, 45, 25, ui->visible3D ? "^23D ON" : "^13D off")){
                ui->visible3D ^= 1;
            }
            
            if(step && drawDefaultButton2D(w / 2 - 320, y, 25, 25, "^5go")){
                sprintf(buffer, "setpos %1.3f %1.3f %1.3f", vecParamsXYZ(step->before.pos));
                cmdParse(buffer);
                entUpdatePosInstantaneous(playerPtr(), step->before.pos);
            }

            sprintf(buffer,
                    "^%d%s^d: ^4%d ^d/ ^4%d",
                    ui->curPos == rec->steps.count - 1 ? 1 : 7,
                    rec->name,
                    ui->curPos,
                    max(0, rec->steps.count - 1));
                    
            if(step){
                sprintf(buffer + strlen(buffer), " ^d(^4%d^d)", step->csc_net_id);
            }
            
            if(drawDefaultButton2D(w / 2 - 100, y, 200, 25, buffer)){
                if(step){
                    ui->syncToMe = 1;
                    syncToCSC = step->csc_net_id;
                }
            }
            
            oldCurPos = ui->curPos;
            
            if(rec->steps.count){
                int count;
                int gotoMe;
                
                for(j = 0, count = 0; j < ui->curPos; j++){
                    if(rec->steps.step[j].csc_net_id == rec->steps.step[ui->curPos].csc_net_id){
                        count++;
                        gotoMe = j;
                    }
                }
                
                if(count){
                    sprintf(buffer, "^4%d^d<", count);
                    if(drawDefaultButton2D(w / 2 - 220, y, 25, 25, buffer)){
                        ui->curPos = gotoMe;
                    }
                }

                for(j = ui->curPos + 1, count = 0; j < rec->steps.count; j++){
                    if(rec->steps.step[j].csc_net_id == rec->steps.step[ui->curPos].csc_net_id){
                        count++;
                        if(count == 1){
                            gotoMe = j;
                        }
                    }
                }
                
                if(count){
                    sprintf(buffer, ">^4%d", count);
                    if(drawDefaultButton2D(w / 2 + 225, y, 25, 25, buffer)){
                        ui->curPos = gotoMe;
                    }
                }
                
                if(drawDefaultButton2D(w / 2 + 300, y, 45, 25, "^1break")){
                    assert(0);
                }
            }
            
            if(drawDefaultButton2D(w / 2 - 130, y, 25, 25, "^2<") || goPrev1){
                ui->doNotStayAtEnd = 1;
                ui->curPos--;
            }
            
            if(drawDefaultButton2D(w / 2 - 160, y, 25, 25, "^1<<\n10") || goPrev2){
                ui->doNotStayAtEnd = 1;
                ui->curPos -= 10;
            }
            
            if(drawDefaultButton2D(w / 2 - 190, y, 25, 25, "^7<<\n100") || goPrev3){
                ui->doNotStayAtEnd = 1;
                ui->curPos -= 100;
            }
            
            if(drawDefaultButton2D(w / 2 + 105, y, 25, 25, "^2>") || goNext1){
                ui->doNotStayAtEnd = 1;
                ui->curPos++;
            }

            if(drawDefaultButton2D(w / 2 + 135, y, 25, 25, "^1>>\n10") || goNext2){
                ui->doNotStayAtEnd = 1;
                ui->curPos += 10;
            }
            
            if(drawDefaultButton2D(w / 2 + 165, y, 25, 25, "^7>>\n100") || goNext3){
                ui->doNotStayAtEnd = 1;
                ui->curPos += 100;
            }
            
            if(drawDefaultButton2D(w / 2 + 195, y, 25, 25, ui->doNotStayAtEnd ? "^1>|" : "^2>|")){
                ui->doNotStayAtEnd ^= 1;
            }
            
            ui->curPos = MINMAX(ui->curPos, 0, max(0, rec->steps.count - 1));
            
            if(oldCurPos != ui->curPos){
                ui->fadeOutLo = min(oldCurPos, ui->curPos);
                ui->fadeOutHi = max(oldCurPos, ui->curPos);
                ui->fadeOutAlpha = 1023;
            }
            
            if(ui->hilight && step){
                char* pos = buffer;
                
                pos += sprintf(pos, "^1Before:\n");

                #define PRINT(cond, params) {if(cond)pos += sprintf params;pos += sprintf(pos, "\n");}
                PRINT(step->before.motion.jumping, (pos, "jumping"));
                #undef PRINT
                
                printDebugString(buffer, 5, h - 25, 2, 20, -1, -1, 255, NULL);

                pos = buffer;
                
                pos += sprintf(pos, "^2After:\n");
                
                #define PRINT(cond, params) {if(cond)pos += sprintf params;pos += sprintf(pos, "\n");}
                PRINT(step->after.motion.jumping, (pos, "jumping"));
                #undef PRINT
                
                printDebugString(buffer, 205, h - 25, 2, 20, -1, -1, 255, NULL);
            }

            visibleY += 30;
        }
    }
    
    if(!show3D){
        if(printName){
            int wh[2];
            
            printDebugString(printName, 0, 0, 3, 1, -1, -1, 0, wh);
            
            printDebugString(printName, (w - wh[0]) / 2, visibleY + 10, 3, 11, 7, -1, 128, NULL);
        }
    }
    
    lastTime = curTime;
}

static void displayBeaconLines(){
    // Draw beacon debug lines.
    
    if(    eaSize(&beaconConnection) &&
        game_state.see_everything & 3 &&
        isMenu(MENU_GAME) &&
        (    beacConn_showWhenMouseDown
            ||
            editMode() &&
            !edit_state.look &&
            !edit_state.pan &&
            !edit_state.zoom
            ||
            !editMode() &&
            !control_state.canlook))
    {
        int i;
        
        for(i = 0; i < eaSize(&beaconConnection); i++){
            if((!beacConn_cullLines || gfxIsPointVisible(beaconConnection[i]->a)) && distance3Squared(beaconConnection[i]->a, playerPtr()->fork[3]) < g_BeaconDebugRadius * g_BeaconDebugRadius)
            {
                drawLine3D_2(    beaconConnection[i]->a,
                                beaconConnection[i]->colorARGB1,
                                beaconConnection[i]->b,
                                beaconConnection[i]->colorARGB2);
            }
        }
    }
}

void displayDebugInterface3D()
{
    
    debug_state.mouseOverUI = 0;

    ent_debug_binds_profile.trickleKeys = conVisible();

    if(!playerPtr() || !isMenu(MENU_GAME))
        return;

     displayBeaconLines();

    displayDebugPowerInfo();

    // Draw various other lines.
    
    if(game_state.hasEntDebugInfo || game_state.ent_debug_client){
        //Vec3 pos;

        PERFINFO_AUTO_START("displayEntDebugLines",1);
            displayEntDebugLines();
        PERFINFO_AUTO_STOP();

        //copyVec3(cam_info.mat[3], pos);
        //pos[0] += 10;
        //drawLine3D(cam_info.mat[3], pos, 0xffff0000);

        //copyVec3(cam_info.lastPos, pos);
        //pos[2] += 10;
        //drawLine3D(cam_info.lastPos, pos, 0xff00ff00);

        //copyVec3(cam_info.cammat[3], pos);
        //pos[2] += 10;
        //drawLine3D(cam_info.mat[3], pos, 0xff00ff00);
    }

    PERFINFO_AUTO_START("displayAStarRecording",1);

        displayAStarRecording();

    PERFINFO_AUTO_STOP_START("displayEncounterBeacons",1);

        displayEncounterBeacons();

    PERFINFO_AUTO_STOP_START("displayPhysicsRecording3D",1);

        displayPhysicsRecording(1);

    PERFINFO_AUTO_STOP_START("displayMissionEditorObjects",1);

    PERFINFO_AUTO_STOP();
}

static void drawAccessLevelDebugInterfaces()
{

    PERFINFO_AUTO_START("displayBitGraph",1);

    displayBitGraph();

    PERFINFO_AUTO_STOP_START("displayScriptDebugInfo",1);

    displayScriptDebugInfo();

    PERFINFO_AUTO_STOP_START("displayPerformanceInfo",1);

    displayPerformanceInfo();

    PERFINFO_AUTO_STOP_START("displayEncounterStats",1);

    displayEncounterStats();

    PERFINFO_AUTO_STOP_START("displayClientStuff",1);

    displayClientStuff();

    PERFINFO_AUTO_STOP_START("displayDebugCameraEditor",1);

    displayDebugCameraEditor();

    PERFINFO_AUTO_STOP_START("displayDebugMenu",1);

    displayDebugMenu();

    PERFINFO_AUTO_STOP_START("displayCrazyLoadCovering",1);

    displayCrazyLoadCovering();

    PERFINFO_AUTO_STOP_START("displayDebugButtons",1);

    displayDebugButtons();

    PERFINFO_AUTO_STOP_START("camPrintInfo",1);

    camPrintInfo();

    PERFINFO_AUTO_STOP_START("displayBeaconDebugButtons",1);

    displayBeaconDebugButtons();

    PERFINFO_AUTO_STOP_START("displayPhysicsRecording2D",1);

    displayPhysicsRecording(0);

    PERFINFO_AUTO_STOP();
}
void displayDebugInterface2D()
{
    Entity *e = playerPtr();
    debug_state.mouseOverUI = 0;

    ent_debug_binds_profile.trickleKeys = conVisible();

    if (game_state.disable2Dgraphics || demoIsPlaying() || conVisible() || !e || !isMenu(MENU_GAME))
        return;

        
    PERFINFO_AUTO_START("displayEntDebugInfoTextBegin",1);
    
        displayEntDebugInfoTextBegin();
        {
            int w, h;
            windowSize(&w,&h);
            if (!gfx_state.screenshot && !game_state.beacons_loaded && !isProductionMode())
                printDebugStringAlign( "^1THIS MAP HAS NOT BEEN BEACONIZED", 
                    (w / 2)-40, h - 20, 80, 20, ALIGN_HCENTER, 1, 0, -1, -1, 255);
            // TODO: how do I beaconize?
        }

    PERFINFO_AUTO_STOP_START("displayEntDebugInfoText",1);

        displayEntDebugInfoText();

    PERFINFO_AUTO_STOP_START("displayNetGraph",1);

        displayNetGraph();

    PERFINFO_AUTO_STOP();

    if (e->access_level)
    {
        drawAccessLevelDebugInterfaces();
    }
        
    PERFINFO_AUTO_START("displayEntDebugInfoTextEnd",1);

        displayEntDebugInfoTextEnd();

    PERFINFO_AUTO_STOP();
}

static int receiveServerPerformanceUpdateLayout(Packet* pak, int* totalCount, PerformanceInfo* parent){
    PerformanceInfo* info;
    int i;

    if(!pktGetBits(pak, 1))
        return -1;

    i = pktGetBitsPack(pak, 7);

    assert(i < debug_state.serverTimersAllocated);

    info = debug_state.serverTimers + i;

    info->uid = i;

    strcpy(debug_state.serverTimerName[i], pktGetString(pak));

    info->locName = debug_state.serverTimerName[i];

    // This is nastay, but it gets the job done as a GUID.

    info->rootStatic = (PerformanceInfo**)pktGetBits(pak, 32);

    info->infoType = pktGetBits(pak, 1);
    
    if(info->infoType){
        // First bit == 1 means that it's either 1 or 2.
        
        info->infoType = 1 + pktGetBits(pak, 1);
    }

    info->parent = parent;
    info->child.head = NULL;
    info->child.tail = NULL;
    info->nextSibling = NULL;

    (*totalCount)++;

    while(1){
        PerformanceInfo* child;
        int j = receiveServerPerformanceUpdateLayout(pak, totalCount, info);

        if(j < 0)
            break;

        child = debug_state.serverTimers + j;

        if(!debug_state.serverTimers[i].child.head)
            debug_state.serverTimers[i].child.head = child;
        else
            debug_state.serverTimers[i].child.tail->nextSibling = child;

        debug_state.serverTimers[i].child.tail = child;
    }

    return i;
}

static U64 getBits64(Packet* pak){
    int byte_count = pktGetBits(pak, 3) + 1;
    U64 value64 = 0;
    U32* value32 = (U32*)&value64;

    if(byte_count > 4){
        *value32 = pktGetBits(pak, 32);
        value32++;
        byte_count -= 4;
    }

    *value32 = pktGetBits(pak, byte_count * 8);

    return value64;
}

void entDebugClearServerPerformanceInfo(){
    debug_state.serverTimersCount = 0;
    debug_state.serverTimersLayoutID = 0;
    memset(debug_state.serverTimers, 0, debug_state.serverTimersAllocated * sizeof(*debug_state.serverTimers));
}

void entDebugReceiveServerPerformanceUpdate(Packet* pak){
    int i;
    int count;
    int layoutID;
    int getRunCount;
    int getTotalTicks;

    layoutID = pktGetBits(pak, 32);

    if(layoutID == 0){
        entDebugClearServerPerformanceInfo();
        return;
    }
    
    if(layoutID < debug_state.serverTimersLayoutID){
        return;
    }

    if(pktGetBits(pak, 1)){
        int newCount;
        int reset;

        reset = pktGetBits(pak, 1);

        // Receive new layout.

        debug_state.serverTimersLayoutID = layoutID;

        newCount = pktGetBitsPack(pak, 1);

        if(newCount > debug_state.serverTimersAllocated){
            char (*newNames)[ARRAY_SIZE(*debug_state.serverTimerName)] = calloc(newCount, sizeof(newNames[0]));
            PerformanceInfo* newInfo = calloc(newCount, sizeof(newInfo[0]));

            memcpy(newNames, debug_state.serverTimerName, debug_state.serverTimersCount * sizeof(newNames[0]));
            memcpy(newInfo, debug_state.serverTimers, debug_state.serverTimersCount * sizeof(newInfo[0]));

            SAFE_FREE(debug_state.serverTimerName);
            SAFE_FREE(debug_state.serverTimers);
            
            debug_state.serverTimerName = newNames;
            debug_state.serverTimers = newInfo;

            debug_state.serverTimersAllocated = newCount;
        }

        if(reset){
            memset(debug_state.serverTimers, 0, debug_state.serverTimersAllocated * sizeof(*debug_state.serverTimers));
        }

        debug_state.serverTimersCount = newCount;
        newCount = 0;

        while(receiveServerPerformanceUpdateLayout(pak, &newCount, NULL) >= 0);

        assert(newCount == debug_state.serverTimersCount);
    }

    if(debug_state.serverTimersLayoutID != layoutID || debug_state.perfInfoPaused)
        return;

    count = pktGetBitsPack(pak, 4);

    debug_state.serverTimersTotal = getBits64(pak);
    debug_state.serverCPUSpeed = getBits64(pak);

    debug_state.serverTimePassed = getBits64(pak);
    debug_state.serverTimeUsed = getBits64(pak);

    getRunCount = pktGetBits(pak, 1);
    getTotalTicks = pktGetBits(pak, 1);

    for(i = 0; i < debug_state.serverTimersCount; i++){
        PerformanceInfo* info = debug_state.serverTimers + i;
        int j;

        for(j = 0; j < count; j++){
            if(pktGetBits(pak, 1)){
                info->history[info->historyPos].count = pktGetBitsPack(pak, 4);
                info->history[info->historyPos].cycles = getBits64(pak);
            }else{
                info->history[info->historyPos].count = 0;
                info->history[info->historyPos].cycles = 0;
            }

            //if(stricmp(info->name, "entPrepareUpdate") == 0){
            //    printf("%s - %I64u\n", info->name, info->history[info->historyPos]);
            //}

            info->historyPos = (info->historyPos + 1) % ARRAY_SIZE(info->history);
        }

        if(getRunCount){
            __int64 count = getBits64(pak);

            if(count != info->opCountInt){
                info->breakMe = 0;
            }

            info->opCountInt = count;
        }

        if(getTotalTicks)
        {
            info->totalTime = getBits64(pak);
            info->maxTime = getBits64(pak);
        }
    }
}

void entDebugReceiveAStarRecording(Packet* pak){
    MP_CREATE(AStarRecordingSet, 100);

    eaClearEx(&debug_state.aStarRecording.sets, destroyAStarRecordingSetAdapter);
    eaClearEx(&debug_state.aStarRecording.blocks, destroyAStarBeaconBlockAdapter);

    while(pktGetBits(pak, 1)){
        AStarBeaconBlock* block = createAStarBeaconBlock();
        int count;
        
        block->pos[0] = pktGetF32(pak);
        block->pos[1] = pktGetF32(pak);
        block->pos[2] = pktGetF32(pak);
        
        block->searchY = pktGetF32(pak);
        
        count = pktGetBitsPack(pak, 8);
        
        while(count--){
            AStarPoint* point = createAStarPoint();
            
            point->pos[0] = pktGetF32(pak);
            point->pos[1] = pktGetF32(pak);
            point->pos[2] = pktGetF32(pak);
            
            eaPush(&block->beacons, point);
        }
        
        eaPush(&debug_state.aStarRecording.blocks, block);
    }

    vecX(debug_state.aStarRecording.target) = pktGetF32(pak);
    vecY(debug_state.aStarRecording.target) = pktGetF32(pak);
    vecZ(debug_state.aStarRecording.target) = pktGetF32(pak);

    while(pktGetBits(pak, 1)){
        AStarRecordingSet* set = MP_ALLOC(AStarRecordingSet);

        set->checkedConnectionCount = pktGetBitsPack(pak, 10);
        
        set->costSoFar = pktGetBitsPack(pak, 10);
        set->totalCost = pktGetBitsPack(pak, 10);
        
        while(pktGetBits(pak, 1)){
            AStarConnectionInfo* info = createAStarConnectionInfo();
            
            if(pktGetBits(pak, 1)){
                info->queueIndex = pktGetBitsPack(pak, 8);
                info->totalCost = pktGetBitsPack(pak, 24);
                info->costSoFar = pktGetBitsPack(pak, 24);
            }
            
            info->beaconPos[0] = pktGetF32(pak);
            info->beaconPos[1] = pktGetF32(pak);
            info->beaconPos[2] = pktGetF32(pak);
            
            eaPush(&set->nextConns, info);
        }
        
        while(pktGetBits(pak, 1)){
            AStarPoint* point = MP_ALLOC(AStarPoint);
            
            point->set = set;
            point->pos[0] = pktGetF32(pak);
            point->pos[1] = pktGetF32(pak);
            point->pos[2] = pktGetF32(pak);
            point->flyHeight = pktGetF32(pak);

            if(pktGetBits(pak, 1)){
                point->minHeight = pktGetIfSetF32(pak);
                point->maxHeight = pktGetIfSetF32(pak);
            }else{
                point->minHeight = 0;
                point->maxHeight = 0;
            }

            eaPush(&set->points, point);
        }
        
        set->index = eaSize(&debug_state.aStarRecording.sets);

        eaPush(&debug_state.aStarRecording.sets, set);
    }

    debug_state.aStarRecording.frame = eaSize(&debug_state.aStarRecording.sets) - 1;
}

static ClientEntityDebugPoint* addClientDebugPoint(ClientEntityDebugInfo* info){
    ClientEntityDebugPoint* point;
    
    if(!info)
        return NULL;
        
    point = dynArrayAdd(&info->points.pos, sizeof(*info->points.pos), &info->points.count, &info->points.maxCount, 1);
    
    SAFE_FREE(point->tag);
    
    memset(point, 0, sizeof(*point));
        
    return point;
}

void receiveEntDebugInfo(Entity* e, Packet* pak){
    if(game_state.hasEntDebugInfo){
        int i;
        unsigned int infoId;
        ClientEntityDebugInfo* info = e->debugInfo;
        int pointCount;
        int teamMemberCount;
        char* tempString;
        int pathCount;

        infoId = pak->id;

        if(!e || infoId < e->pkt_id_debug_info || server_visible_state.pause){
            info = NULL;
        }
        else if(!info){
            info = calloc(1, sizeof(*info));
            e->debugInfo = info;
        }
        else{
            // Clear the debug info.

            if(info){
                if(info->text){
                    free(info->text);
                    info->text = NULL;
                }

                if(info->path.points){
                    free(info->path.points);
                    info->path.points = NULL;
                    info->path.count = 0;
                }
            }
        }

        if(info){
            e->pkt_id_debug_info = pak->id;
        }

        #define GET_INFO(x,val) if(info)x=val;else val;

        // Read the debug info text.

        tempString = pktGetString(pak);

        if(info){
            info->text = strdup(tempString);
        }

        // Read the path.

        pathCount = pktGetBitsPack(pak, 1);

        GET_INFO(info->path.count, pathCount);

        if(info && info->path.count)
        {
            info->path.points = malloc(info->path.count * sizeof(*info->path.points));
        }

        for(i = 0; i < pathCount; i++)
        {
            GET_INFO(info->path.points[i].pos[0], pktGetF32(pak));
            GET_INFO(info->path.points[i].pos[1], pktGetF32(pak));
            GET_INFO(info->path.points[i].pos[2], pktGetF32(pak));

            if(pktGetBits(pak, 1))
            {
                GET_INFO(info->path.points[i].isFlying, 0);
                GET_INFO(info->path.points[i].minHeight, pktGetIfSetF32(pak));
                GET_INFO(info->path.points[i].maxHeight, pktGetIfSetF32(pak));
            }
            else
            {
                GET_INFO(info->path.points[i].isFlying, pktGetBits(pak, 1));
                GET_INFO(info->path.points[i].minHeight, 0);
                GET_INFO(info->path.points[i].maxHeight, 0);
            }
        }

        // Read the point log.

        pointCount = pktGetBitsPack(pak, 1);
        
        GET_INFO(info->points.count, 0);

        for(i = 0; i < pointCount; i++){
            ClientEntityDebugPoint* point = addClientDebugPoint(info);
            
            GET_INFO(point->argb, pktGetBits(pak, 32));
            GET_INFO(point->lineToPrevious, i ? 1 : 0);
            GET_INFO(point->pos[0], pktGetF32(pak));
            GET_INFO(point->pos[1], pktGetF32(pak));
            GET_INFO(point->pos[2], pktGetF32(pak));

            tempString = pktGetString(pak);

            if(info){
                point->tag = strdup(tempString);
            }
        }

        // Read the movement target.

        while(pktGetBits(pak, 1) == 1){
            ClientEntityDebugPoint* point = addClientDebugPoint(info);

            GET_INFO(point->pos[0], pktGetF32(pak));
            GET_INFO(point->pos[1], pktGetF32(pak));
            GET_INFO(point->pos[2], pktGetF32(pak));
            GET_INFO(point->argb, pktGetBits(pak, 32));
            GET_INFO(point->lineToEnt, 1);
        }

        // Read the team member list.

        teamMemberCount = pktGetBitsPack(pak, 1);

        if(info){
            if(!info->teamMembers){
                eaiCreate(&info->teamMembers);
            }

            eaiSetSize(&info->teamMembers, teamMemberCount);
        }

        for(i = 0; i < teamMemberCount; i++)
        {
            GET_INFO(info->teamMembers[i], pktGetBitsPack(pak, 8));
        }

        // Read the net_ipos.

        {
            int ipos[3];

            ipos[0] = pktGetBits(pak, 24);
            ipos[1] = pktGetBits(pak, 24);
            ipos[2] = pktGetBits(pak, 24);

            if(0 && !sameVec3(ipos, e->net_ipos)){
                printf(    "ipos different: %d\t%s:\n\t%d\t%d\t%d\n\t%d\t%d\t%d\n",
                        global_state.client_abs,
                        e->name,
                        ipos[0],
                        ipos[1],
                        ipos[2],
                        e->net_ipos[0],
                        e->net_ipos[1],
                        e->net_ipos[2]);
            }
        }

        #undef GET_INFO
    }
}

void receiveGlobalEntDebugInfo(Packet *pak) {
    Vec3 last_pos;

    if(game_state.hasEntDebugInfo){
        zeroVec3(last_pos);

        // Get all the lines.

        while(pktGetBits(pak, 1))
        {
            Vec3 cur_pos;
            int argb;
            int connect;
            int drawupright;

            cur_pos[0] = pktGetF32(pak);
            cur_pos[1] = pktGetF32(pak);
            cur_pos[2] = pktGetF32(pak);
            argb = pktGetBits(pak, 32);
            connect = pktGetBits(pak, 1);
            drawupright = pktGetBits(pak, 1);

            if(connect){
                entDebugAddLine(last_pos, 0xffffffff, cur_pos, argb);
            }

            copyVec3(cur_pos, last_pos);

            if(drawupright){
                cur_pos[1] += 0.5;
                cur_pos[0] += ((rand() % 21) - 10) * 0.1 * 0.02;
                cur_pos[2] += ((rand() % 21) - 10) * 0.1 * 0.02;
                entDebugAddLine(last_pos, 0xffffffff, cur_pos, 0xffffff00);
                copyVec3(last_pos, cur_pos);
                cur_pos[1] -= 1;
                entDebugAddLine(last_pos, 0xffffffff, cur_pos, 0xffffffff);
            }
        }

        // Get all the text.

        while(pktGetBits(pak, 1))
        {
            Vec3 pos;
            const char* text;
            int flags;

            pos[0] = pktGetF32(pak);
            pos[1] = pktGetF32(pak);
            pos[2] = pktGetF32(pak);
            text = pktGetString(pak);
            flags = pktGetBitsPack(pak, 1);

            entDebugAddText(pos, text, flags);
        }

        // get encounter stats
        if (pktGetBits(pak,1))
        {
            int defaultplayer, defaultvillain, defaultadjust;
            debug_state.encounterPlayerCount[debug_state.encounterTail] = pktGetBitsPack(pak, 1);
            debug_state.encounterRunning[debug_state.encounterTail] = pktGetBitsPack(pak, 1);
            debug_state.encounterActive[debug_state.encounterTail] = pktGetBitsPack(pak, 1);
            debug_state.encounterWin[debug_state.encounterTail] = pktGetF32(pak);
            debug_state.encounterPlayerRatio[debug_state.encounterTail] = pktGetF32(pak);
            debug_state.encounterActiveRatio[debug_state.encounterTail] = pktGetF32(pak);
            debug_state.encounterPanicLevel[debug_state.encounterTail] = pktGetBitsPack(pak, 1);
            if (debug_state.encounterTail >= ENCOUNTER_HIST_LENGTH-1) debug_state.encounterWrap = 1;
            debug_state.encounterTail = (debug_state.encounterTail + 1) % ENCOUNTER_HIST_LENGTH;

            debug_state.encounterPlayerRadius = pktGetBitsPack(pak, 1);
            debug_state.encounterVillainRadius = pktGetBitsPack(pak, 1);
            debug_state.encounterAdjustProb = pktGetBitsPack(pak, 1);
            defaultplayer = pktGetBitsPack(pak, 1);
            defaultvillain = pktGetBitsPack(pak, 1);
            defaultadjust = pktGetBitsPack(pak, 1);

            debug_state.encounterPlayerRadiusIsDefault = (defaultplayer == debug_state.encounterPlayerRadius);
            debug_state.encounterVillainRadiusIsDefault = (defaultvillain == debug_state.encounterVillainRadius);
            debug_state.encounterAdjustProbIsDefault = (defaultadjust == debug_state.encounterAdjustProb);

            debug_state.encounterPanicLow = pktGetF32(pak);
            debug_state.encounterPanicHigh = pktGetF32(pak);
            debug_state.encounterPanicMinPlayers = pktGetBitsPack(pak, 1);

            debug_state.encounterCurCritters = pktGetBitsPack(pak, 1);
            debug_state.encounterMinCritters = pktGetBitsPack(pak, 1);
            debug_state.encounterBelowMinCritters = pktGetBits(pak, 1);
            debug_state.encounterMaxCritters = pktGetBitsPack(pak, 1);
            debug_state.encounterAboveMaxCritters = pktGetBits(pak, 1);

            debug_state.encounterReceivingStats = 1;
        }
        else
        {
            debug_state.encounterReceivingStats = 0;
        }

        // get encounter beacons
        if (pktGetBits(pak,1))
        {
            int i;

            debug_state.numEncounterBeacons = pktGetBitsPack(pak, 1);
            if (!debug_state.encounterBeacons)
                debug_state.encounterBeacons = calloc(sizeof(EncounterBeacon), MAX_ENCOUNTER_BEACONS);
            for (i = 0; i < debug_state.numEncounterBeacons; i++)
            {
                EncounterBeacon beacon = {0};
                char* filename = NULL;
                beacon.pos[0] = pktGetF32(pak);
                beacon.pos[1] = pktGetF32(pak);
                beacon.pos[2] = pktGetF32(pak);
                beacon.color = pktGetBits(pak, 32);
                beacon.speed = pktGetBitsPack(pak, 1);
                if (pktGetBits(pak, 1))
                    filename = pktGetString(pak);
                if (pktGetBits(pak, 1))
                    beacon.time = pktGetBits(pak, 32);
                if (i < MAX_ENCOUNTER_BEACONS)
                {
                    debug_state.encounterBeacons[i] = beacon;
                    if (filename)
                        strcpy(debug_state.encounterBeacons[i].filename, filename);
                    else
                        debug_state.encounterBeacons[i].filename[0] = '\0';
                }
            }
        }
        else
        {
            debug_state.numEncounterBeacons = 0;
        }

        // Get the debug button list.

        {
            DebugButton* button;
            DebugButton* lastButton = NULL;

            MP_CREATE(DebugButton, 50);

            for(button = debug_state.debugButtonList; button;){
                DebugButton* next = button->next;
                MP_FREE(DebugButton, button);
                button = next;
            }

            debug_state.debugButtonList = NULL;
            debug_state.debugButtonCount = 0;

            while(pktGetBits(pak,1)){
                button = MP_ALLOC(DebugButton);

                if(lastButton)
                    lastButton->next = button;
                else
                    debug_state.debugButtonList = button;

                Strncpyt(button->displayText, pktGetString(pak));
                Strncpyt(button->commandText, pktGetString(pak));

                lastButton = button;

                debug_state.debugButtonCount++;
            }
        }
    }else{
        DebugButton* button;

        for(button = debug_state.debugButtonList; button;){
            DebugButton* next = button->next;
            MP_FREE(DebugButton, button);
            button = next;
        }

        debug_state.debugButtonList = NULL;
        debug_state.debugButtonCount = 0;
    }
}


