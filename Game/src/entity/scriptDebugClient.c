/*\
 *
 *    scriptDebugClient.h/c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides client UI for debugging scripts
 *
 */

#include "gameComm/debugCommon.h"
#include "entity/scriptDebugClient.h" 
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/utils.h>
#include "cmdparse/cmdgame.h"
#include "entity/entDebug.h"
#include "edit/edit_cmd.h"
#include "win/win_init.h"
#include "win/input.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/mathutil.h>

#define SCRIPT_NAME_LEN 100
#define SCRIPT_VAR_DISPLAY 20

typedef struct ScriptInfoClient
{
    char    name[SCRIPT_NAME_LEN];
    int        type;
    U32        timeDelta;
    U32        pauseTime;
    int        runState;
    int        id;
    int        varTop;
    int        luaMemory;
} ScriptInfoClient;

ScriptInfoClient** g_scripts;
int g_selectedId = 0;
int g_showingVars = 0;
U32 g_curTime;

typedef struct SignalInfoClient
{
    char    name[SCRIPT_NAME_LEN];
    char    signal[SCRIPT_NAME_LEN];
} SignalInfoClient;
SignalInfoClient** g_signals;

typedef struct VarInfoClient
{
    char    name[SCRIPT_NAME_LEN];
    char    value[SCRIPT_NAME_LEN];
} VarInfoClient;
VarInfoClient** g_scriptvars;

static void ResizeEArray(mEArrayHandle* handle, int size, int num)
{
    if (!g_scripts)    eaCreate(handle);
    while (eaSizeUnsafe(handle) < num)
    {
        eaPush(handle, calloc(size, 1));
    }
    while (eaSizeUnsafe(handle) > num)
    {
        free(eaPop(handle));
    }
}

static ScriptInfoClient* FindInfo(int idx)
{ 
    int i, n;
    n = eaSize(&g_scripts);
    for (i = 0; i < n; i++)
    {
        if (g_scripts[i]->id == idx)
            return g_scripts[i];
    }
    return NULL;
}

static ScriptInfoClient* FindSelectedInfo(void)
{
    return FindInfo(g_selectedId);
}

static int compareScriptVars(const VarInfoClient** lhs, const VarInfoClient** rhs)
{
    if (lhs[0]->name[0] == '_' && rhs[0]->name[0] != '_')
        return -1;
    if (lhs[0]->name[0] != '_' && rhs[0]->name[0] == '_')
        return 1;
    return strcmp(lhs[0]->name, rhs[0]->name);
}

void receiveScriptDebugInfo(Packet *pak)
{
    int num, i;

    // list of all scripts
    num = pktGetBitsPack(pak, 1);
    ResizeEArray(&g_scripts, sizeof(ScriptInfoClient), num);
    for (i = 0; i < num; i++)
    {
        ScriptInfoClient* info = g_scripts[i];
        strncpyt(info->name, pktGetString(pak), SCRIPT_NAME_LEN);
        info->type = pktGetBitsPack(pak, 1);
        info->timeDelta = pktGetBitsPack(pak, 1);
        info->pauseTime = pktGetBitsPack(pak, 1);
        info->runState = pktGetBitsPack(pak, 1);
        info->id = pktGetBitsPack(pak, 1);
        info->luaMemory = pktGetBitsPack(pak, 1);
    }
    g_showingVars = pktGetBits(pak, 1);

    // details on selected script
    g_selectedId = pktGetBitsPack(pak, 1);
    if (g_selectedId)
    {
        num = pktGetBitsPack(pak, 1);
        ResizeEArray(&g_signals, sizeof(SignalInfoClient), num);
        for (i = 0; i < num; i++)
        {
            strncpyt(g_signals[i]->name, pktGetString(pak), SCRIPT_NAME_LEN);
            strncpyt(g_signals[i]->signal, pktGetString(pak), SCRIPT_NAME_LEN);
        }

        num = pktGetBitsPack(pak, 1);
        ResizeEArray(&g_scriptvars, sizeof(VarInfoClient), num);
        for (i = 0; i < num; i++)
        {
            strncpyt(g_scriptvars[i]->name, pktGetString(pak), SCRIPT_NAME_LEN);
            strncpyt(g_scriptvars[i]->value, pktGetString(pak), SCRIPT_NAME_LEN);
        }
        eaQSort(g_scriptvars, compareScriptVars);
    }
}

void ScriptDebugSet(int on)
{
    game_state.script_debug_client = on;
    if (on)
        cmdParse("scriptdebugserver 1 0");
    else
    {
        cmdParse("scriptdebugserver 0 0");
        ResizeEArray(&g_scripts, 0, 0);
        g_selectedId = 0;
    }
}

void ScriptDebugSetVarTop(int tmp_int, int tmp_int2)
{
    ScriptInfoClient* info = FindInfo(tmp_int);

    if (info)
        info->varTop = tmp_int2;
}

void ScriptDebugSelect(int id)
{
    char buf[2000];
    if (game_state.script_debug_client)
    {
        sprintf(buf, "scriptdebugserver 1 %i", id);
        cmdParse(buf);
    }
}

#define SCRIPTBAR_HEIGHT        20
#define SCRIPTBAR_TOPROLL        12
#define SCRIPTBAR_BOTROLL        3
#define SCRIPTBAR_WIDTH            350
#define SCRIPTBAR_NAMEWIDTH        200
#define SCRIPTBAR_HGAP            10
#define SCRIPTBAR_TYPEWIDTH        30
#define SCRIPTBAR_IDWIDTH        30
#define SCRIPTBAR_TIMEWIDTH        70
#define SCRIPTBAR_ALPHA            0x88
#define BUTTONBAR_WIDTH            380
#define BUTTONBAR_HEIGHT        30
#define BUTTON_FROMBOT            4
#define BUTTON_HEIGHT            22
#define BUTTON_WIDTH            100
#define SIGNALBUTTON_WIDTH        150

static char* scriptTimeString(ScriptInfoClient* info, U32 time)
{
    static char buf[50];
    U32 offset, result;
    int sec, min, hr;

    offset = info->runState == SCRIPTSTATE_PAUSED? info->pauseTime - info->timeDelta: g_curTime - info->timeDelta;
    result = offset >= time? offset - time: time - offset;

    if (result > 3600)
        printf("");

    sec = result % 60;
    result /= 60;
    min = result % 60;
    result /= 60;
    hr = result;
    sprintf(buf, "%02i:%02i:%02i", hr, min, sec);
    return buf;
}

static char* getScriptVar(char* scriptvar)
{
    int i, n;
    n = eaSize(&g_scriptvars);
    for (i = 0; i < n; i++)
    {
        if (stricmp(g_scriptvars[i]->name, scriptvar)==0)
            return g_scriptvars[i]->value;
    }
    return NULL;
}

static int ignoreScriptVar(char* varname)
{
    if (stricmp(varname, STATE_VAR)==0) return 1;
    if (stricmp(varname, STATE_TIME)==0) return 1;
    if (stricmp(varname, STATE_ENTRY)==0) return 1;
    return 0;
}

static void displayScriptInfo(int x, int y, ScriptInfoClient* info, int running, int highlight, int active, int drawbox)
{
    char buf[2000];
    char *type = NULL;
    int alpha = SCRIPTBAR_ALPHA << 24;
    int high, mid, low;

    // background box, rounded shape
    if (drawbox)
    {
        if (highlight && active)
        {
            high = 0xff8855 | alpha;
            mid = 0xaa5500 | alpha;
            low = 0x662200 | alpha;
        }
        else if (highlight)
        {
            high = 0xffff55 | alpha;
            mid = 0xaaaa00 | alpha;
            low = 0x555500 | alpha;
        }
        else if (active)
        {
            high = 0xff5555 | alpha;
            mid = 0xaa0000 | alpha;
            low = 0x550000 | alpha;
        }
        else
        {
            high = 0x5555ff | alpha;
            mid = 0x0000aa | alpha;
            low = 0x000055 | alpha;
        }
        drawFilledBox4(x, y, x+SCRIPTBAR_WIDTH, y+SCRIPTBAR_BOTROLL, mid, mid, low, low);
        drawFilledBox(x, y+SCRIPTBAR_BOTROLL, x+SCRIPTBAR_WIDTH, y+SCRIPTBAR_HEIGHT-SCRIPTBAR_TOPROLL, mid);
        drawFilledBox4(x, y+SCRIPTBAR_HEIGHT-SCRIPTBAR_TOPROLL, x+SCRIPTBAR_WIDTH, y+SCRIPTBAR_HEIGHT, high, high, mid, mid);
    }

    // script name
    sprintf(buf, "^%i%s", running? 2:1, info->name); 
    printDebugStringAlign(buf, x + SCRIPTBAR_HGAP/2, y+1, SCRIPTBAR_NAMEWIDTH, SCRIPTBAR_HEIGHT, ALIGN_VCENTER, 1.2, 12, -1, 0, 255);

    // script id  
    sprintf(buf, "^%i%d", running? 2:1, info->id); 
    printDebugStringAlign(buf, x + SCRIPTBAR_HGAP + SCRIPTBAR_NAMEWIDTH, y+1, SCRIPTBAR_IDWIDTH, SCRIPTBAR_HEIGHT, ALIGN_VCENTER, 1.2, 12, -1, 0, 255);

    // script type
    switch (info->type)
    {
    case SCRIPT_MISSION:    type = "MI"; break;
    case SCRIPT_ZONE:        type = "ZE"; break;
    case SCRIPT_ENCOUNTER:    type = "EN"; break;
    case SCRIPT_LOCATION:    type = "LO"; break;
    case SCRIPT_ENTITY:        type = "ET"; break;
    }
    sprintf(buf, "^%i%s", running? 2:1, type);
    printDebugStringAlign(buf, x + SCRIPTBAR_NAMEWIDTH + SCRIPTBAR_HGAP + SCRIPTBAR_IDWIDTH, y+1, 
        SCRIPTBAR_TYPEWIDTH, SCRIPTBAR_HEIGHT, ALIGN_VCENTER, 1.2, 12, -1, 0, 255);

    // script time running
    sprintf(buf, "^%i%s", running? 2:1, scriptTimeString(info, 0));
    printDebugStringAlign(buf, x + SCRIPTBAR_NAMEWIDTH + 2*SCRIPTBAR_HGAP + SCRIPTBAR_TYPEWIDTH + SCRIPTBAR_IDWIDTH, y+1, 
        SCRIPTBAR_TIMEWIDTH , SCRIPTBAR_HEIGHT, ALIGN_VCENTER, 1.2, 12, -1, 0, 255);
}


void displayScriptDebugInfo(void)
{
    int w, h, mouse_x, mouse_y, inv_y, curx, cury;
    int i, n;
    int selbar;
    char buf[2000], cmd[200];
    ScriptInfoClient* info;

    if (editMode()) return;
    if (!game_state.script_debug_client) return;
    if (entDebugMenuVisible()) return;

    g_curTime = timerSecondsSince2000WithDelta();
    windowSize(&w, &h);
    inpMousePos(&mouse_x, &mouse_y);
    inv_y = mouse_y;
    mouse_y = h-mouse_y;

    // selection
    selbar = -1;
    if (!inpIsMouseLocked() && mouse_x >= 0 && mouse_x <= SCRIPTBAR_WIDTH && 
        inv_y >= 0 && inv_y < eaSize(&g_scripts)*SCRIPTBAR_HEIGHT)
    {
        selbar = (inv_y / SCRIPTBAR_HEIGHT);
        if(inpEdge(INP_LBUTTON))
        {
            eatEdge(INP_LBUTTON);
            if (g_selectedId == g_scripts[selbar]->id)
                ScriptDebugSelect(0);
            else
                ScriptDebugSelect(g_scripts[selbar]->id);
        }
    }
    if (selbar >= 0)
        setMouseOverDebugUI(1);

    // script bars
    n = eaSize(&g_scripts);
    for (i = 0; i < n; i++)
    {
        displayScriptInfo(0, h-(i+1)*SCRIPTBAR_HEIGHT, g_scripts[i], g_scripts[i]->runState == SCRIPTSTATE_RUNNING, 
            selbar == i, g_selectedId == g_scripts[i]->id, 1);
    }

    // selected script
    info = FindSelectedInfo();
    if (info)
    {
        int barcolor = 0x004400 | SCRIPTBAR_ALPHA << 24;
        int optioncolor = 0x008800 | SCRIPTBAR_ALPHA << 24;
        int running = info->runState == SCRIPTSTATE_RUNNING;
        int cursignal = 0;
        int curvar = 0;
        int endvar = 0;
        char *state, *statetime;

        curx = SCRIPTBAR_WIDTH;
        cury = h - BUTTONBAR_HEIGHT;
        drawFilledBox(curx, cury, curx + BUTTONBAR_WIDTH, cury+BUTTONBAR_HEIGHT, barcolor);
        displayScriptInfo(curx + SCRIPTBAR_HGAP*2, cury+(BUTTONBAR_HEIGHT-SCRIPTBAR_HEIGHT)/2, info, running, 0, 0, 0);
        
        // interaction buttons
        cury -= BUTTONBAR_HEIGHT;
        drawFilledBox(curx, cury, curx + BUTTONBAR_WIDTH, cury + BUTTONBAR_HEIGHT, barcolor);
        curx += SCRIPTBAR_HGAP;

        sprintf(buf, "scriptpause %i %i", info->id, running? 1: 0);
        drawButton2D(curx, cury+BUTTON_FROMBOT, BUTTON_WIDTH, BUTTON_HEIGHT,
            1, running? "Pause": "Play", 1.2, buf, NULL);
        curx += SCRIPTBAR_HGAP*2 + BUTTON_WIDTH;

        sprintf(buf, "scriptreset %i", info->id);
        drawButton2D(curx, cury+BUTTON_FROMBOT, BUTTON_WIDTH, BUTTON_HEIGHT,
            1, "Reset", 1.2, buf, NULL);
        curx += SCRIPTBAR_HGAP*2 + BUTTON_WIDTH;

        sprintf(buf, "scriptshowvars %i", g_showingVars? 0: 1);
        drawButton2D(curx, cury+BUTTON_FROMBOT, BUTTON_WIDTH, BUTTON_HEIGHT,
            1, g_showingVars? "Hide Vars": "Show Vars", 1.2, buf, NULL);

        // signals
        while (cursignal < eaSize(&g_signals)) 
        {
            cury -= BUTTONBAR_HEIGHT;
            curx = SCRIPTBAR_WIDTH;
            drawFilledBox(curx, cury, curx + BUTTONBAR_WIDTH, cury + BUTTONBAR_HEIGHT, barcolor);
            curx += 2*SCRIPTBAR_HGAP;

            for (i = 0; i < 2; i++) // two signals per row
            {
                sprintf(buf, "scriptsignal %i %s", info->id, g_signals[cursignal]->signal);
                drawButton2D(curx, cury+BUTTON_FROMBOT, SIGNALBUTTON_WIDTH, BUTTON_HEIGHT,
                    1, g_signals[cursignal]->name, 1.2, buf, NULL);
                curx += SCRIPTBAR_HGAP*2 + SIGNALBUTTON_WIDTH;

                cursignal++;
                if (cursignal >= eaSize(&g_signals)) break;
            }
        }
        curx = SCRIPTBAR_WIDTH;

        // lua memory usage
        if (info->luaMemory > 0)
        {
            cury -= SCRIPTBAR_HEIGHT;
            drawFilledBox(curx, cury, curx + BUTTONBAR_WIDTH, cury+SCRIPTBAR_HEIGHT, optioncolor);
            printDebugStringAlign("^5Lua Memory Usage:", curx + 2*SCRIPTBAR_HGAP, cury, SCRIPTBAR_WIDTH, SCRIPTBAR_HEIGHT, 
                ALIGN_VCENTER | ALIGN_LEFT, 1.2, 12, -1, 0, 255);

            sprintf(buf, "^5%d%s", info->luaMemory, " KB");
            printDebugStringAlign(buf, curx + 2*SCRIPTBAR_HGAP, cury, BUTTONBAR_WIDTH-4*SCRIPTBAR_HGAP, SCRIPTBAR_HEIGHT, 
                    ALIGN_VCENTER | ALIGN_RIGHT, 1.2, 12, -1, 0, 255);
        }

        // state
        state = getScriptVar(STATE_VAR);
        if (state)
        {
            cury -= SCRIPTBAR_HEIGHT;
            drawFilledBox(curx, cury, curx + BUTTONBAR_WIDTH, cury+SCRIPTBAR_HEIGHT, optioncolor);
            
            sprintf(buf, "^5State: %s", state);
            printDebugStringAlign(buf, curx + 2*SCRIPTBAR_HGAP, cury, SCRIPTBAR_WIDTH, SCRIPTBAR_HEIGHT, 
                ALIGN_VCENTER | ALIGN_LEFT, 1.2, 12, -1, 0, 255);

            statetime = getScriptVar(STATE_TIME);
            if (statetime)
            {
                sprintf(buf, "^5%s", scriptTimeString(info, atoi(statetime)));
                printDebugStringAlign(buf, curx + 2*SCRIPTBAR_HGAP, cury, BUTTONBAR_WIDTH-4*SCRIPTBAR_HGAP, SCRIPTBAR_HEIGHT, 
                    ALIGN_VCENTER | ALIGN_RIGHT, 1.2, 12, -1, 0, 255);
            }
        }

        /////////////////////////////////////////////////////////////
        // variables

        // display up arrow
        if (info->varTop > 0) 
        {
            cury -= SCRIPTBAR_HEIGHT;
            drawFilledBox(curx, cury, curx + BUTTONBAR_WIDTH, cury + SCRIPTBAR_HEIGHT, optioncolor);

            sprintf(buf, "^%i[more]", info->varTop>0? 2: 3);
            sprintf(cmd, "scriptvartop %i %i", info->id, MAX(0, info->varTop - SCRIPT_VAR_DISPLAY));
            drawButton2D(curx + 2*SCRIPTBAR_HGAP, cury, BUTTONBAR_WIDTH-4*SCRIPTBAR_HGAP, SCRIPTBAR_HEIGHT,
                1, buf, 1.2, cmd, NULL);
        }

        // variable list
        endvar = MIN(eaSize(&g_scriptvars), info->varTop + SCRIPT_VAR_DISPLAY);
        for (curvar = info->varTop; curvar < endvar; curvar++)
        {
            char *name, *value, *format;
            int isspecial = 0;
            if (ignoreScriptVar(g_scriptvars[curvar]->name)) 
                continue;

            name = g_scriptvars[curvar]->name;
            value = g_scriptvars[curvar]->value;
            format = "^%i%s";
            if (strncmp(name, PREFIX_TIMER, strlen(PREFIX_TIMER))==0)
            {
                isspecial = 1;
                name = name + strlen(PREFIX_TIMER);
                value = scriptTimeString(info, atoi(value));
                format = "^%iTimer: %s";
            }
            else if (strncmp(name, PREFIX_SIGNAL, strlen(PREFIX_SIGNAL))==0)
            {
                isspecial = 1;
                name = name + strlen(PREFIX_SIGNAL);
                format = "^%iSignal: %s";
            }

            cury -= SCRIPTBAR_HEIGHT;
            drawFilledBox(curx, cury, curx + BUTTONBAR_WIDTH, cury + SCRIPTBAR_HEIGHT, optioncolor);
            sprintf(buf, format, isspecial? 5: 3, name);
            printDebugStringAlign(buf, curx + 2*SCRIPTBAR_HGAP, cury, SCRIPTBAR_WIDTH, SCRIPTBAR_HEIGHT, 
                ALIGN_VCENTER | ALIGN_LEFT, 1.2, 12, -1, 0, 255);
            sprintf(buf, "^%i%s", isspecial? 5: 3, value);
            printDebugStringAlign(buf, curx + 2*SCRIPTBAR_HGAP, cury, BUTTONBAR_WIDTH-4*SCRIPTBAR_HGAP, SCRIPTBAR_HEIGHT, 
                ALIGN_VCENTER | ALIGN_RIGHT, 1.2, 12, -1, 0, 255);
        }
        // display down arrow
        if (eaSize(&g_scriptvars)>info->varTop+SCRIPT_VAR_DISPLAY)
        {
            cury -= SCRIPTBAR_HEIGHT;
            drawFilledBox(curx, cury, curx + BUTTONBAR_WIDTH, cury + SCRIPTBAR_HEIGHT, optioncolor);
            sprintf(buf, "^%i[more]", eaSize(&g_scriptvars)>info->varTop+SCRIPT_VAR_DISPLAY? 2: 3);
            sprintf(cmd, "scriptvartop %i %i", info->id, MIN(eaSize(&g_scriptvars), info->varTop + SCRIPT_VAR_DISPLAY));
            drawButton2D(curx + 2*SCRIPTBAR_HGAP, cury, BUTTONBAR_WIDTH-4*SCRIPTBAR_HGAP, SCRIPTBAR_HEIGHT,
                1, buf, 1.2, cmd, NULL);
        }
    }
    
    // num scripts
    sprintf(buf, "^2Number of scripts running: ^2%i\n", eaSize(&g_scripts));
    printDebugString(buf, SCRIPTBAR_WIDTH + 5, 2, 1.2, 12, -1, 0, 255, NULL);
}
