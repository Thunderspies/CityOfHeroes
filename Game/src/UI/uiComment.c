/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "gameComm/wdwbase.h"
#include <utilitieslib/utils/utils.h>
#include "comm_game.h"
#include <utilitieslib/components/EString.h>

#include "graphics/textureatlas.h"
#include "win/input.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiFocus.h"
#include "UI/uiScrollBar.h"
#include "UI/uiEditText.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiInput.h"
#include "UIEdit.h"
#include "UI/uiPetition.h"

#include "entity/entVarUpdate.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"

#define TEXTAREA_LEN 256

static UIEdit *s_pedit;
static int s_radio1;
static int s_radio2;
static int s_radio3;

// #define SHOW_MISSION_SURVEY // Define if you want survey boxes to pop up.

extern void BugReport(const char * desc, int mode);

/**********************************************************************func*
 * initTextArea
 *
 */
static void initTextArea(char *pch)
{
    s_pedit = uiEditCreateSimple(pch, TEXTAREA_LEN, &game_12, CLR_WHITE, NULL);

    s_radio1 = 3;
    s_radio2 = 3;
    s_radio3 = 3;
}

/**********************************************************************func*
 * editTextArea
 *
 */
static void editTextArea(UIEdit *pedit, float x, float y, float z, float wd, float ht, float sc)
{
    CBox box;
    int colorfg = CLR_NORMAL_FOREGROUND;
    int colorbg = CLR_NORMAL_BACKGROUND;
    UIBox uibox;

    BuildCBox(&box, x+sc*PIX3, y+sc*(PIX3+15), wd+sc*(-PIX3*2), ht+sc*(-PIX3*2-15));
    if(mouseCollision(&box))
    {
        if(mouseDown(MS_LEFT))
        {
            uiEditTakeFocus(pedit);
        }
    }

    if(uiInFocus(pedit))
    {
        KeyInput* input;

        // Handle keyboard input.
        input = inpGetKeyBuf();
        while(input)
        {
            if(input->key == INP_RETURN)
            {
                uiEditReturnFocus(pedit);
            }
            else
            {
                uiEditDefaultKeyboardHandler(pedit, input);
            }
            inpGetNextKey(&input);
        }

        colorfg = CLR_MOUSEOVER_FOREGROUND;
        colorbg = CLR_MOUSEOVER_BACKGROUND;
    }
    uiEditDefaultMouseHandler(pedit);

    drawFlatFrame(PIX3, R4, x, y, z, wd, ht, sc, colorfg, colorbg);

    uiBoxDefine(&uibox, x+sc*PIX3, y+sc*(PIX3), wd+sc*(-PIX3*2), ht+sc*(-PIX3*2));
    uiEditSetBounds(pedit, uibox);
    uiEditSetZ(pedit, z);
    uiEditProcess(pedit);
}


/**********************************************************************func*
 * commentWindow
 *
 */
int commentWindow(void)
{
    static bool init = true;
    float x, y, yorig, z, wd, ht, sc;
    int color, bcolor;

    if(!window_getDims(WDW_BETA_COMMENT, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
    {
        init = false;
        uiReturnFocus(s_pedit);
        return 0;
    }

    if(!init)
    {
        init = true;
        initTextArea(NULL);
    }

    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor);

    x+=sc*PIX3*2;
    y+=sc*18;
    wd+=sc*(-PIX3*4);
    ht+=sc*(-PIX3*2);
    yorig = y;

    font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);

    cprnt(x+wd/2, y, z, sc, sc, "MissionOpinion");
    y+=sc*20;

    s_radio1 = RadioButtons(x+sc*5,   y, z, sc, s_radio1, "LengthString",     4, "Too ong", "Justright", "Tooshort", "Noanswer");
    s_radio2 = RadioButtons(x+wd/3,   y, z, sc, s_radio2, "DifficultyString", 4, "Toohard", "Justright", "Tooeasy",  "Noanswer");
    s_radio3 = RadioButtons(x+2*wd/3, y, z, sc, s_radio3, "CoolnessString",   4, "BoringString",   "OKString", "Verycool", "Noanswer");
    y+=sc*5*15;
    y+=sc*5;
    prnt(x, y, z, sc, sc, "Anyothercomments");
    y+=sc*5;
    editTextArea(s_pedit, x, y, z, wd, sc*90, sc);
    y+=sc*(90+15);

    if(D_MOUSEHIT==drawStdButton(x + wd/2, y, z, 120*sc, 25*sc, CLR_GREEN, "SendComment", sc, 0))
    {
        StuffBuff sb;
        char *pch;

        // Clean up the strings. No \r, \n, or <>
        char *estr = uiEditGetUTF8Text(s_pedit);
        if(!estr)
        {
            estrCreate(&estr);
            estrPrintCharString(&estr, "(no comment)");
        }

        // Clean up the string. No \r, \n, or <>
        for(pch=estr; *pch!='\0'; pch++)
        {
            if(*pch=='\r' || *pch=='\n' || *pch=='<' || *pch=='>')
            {
                *pch = ' ';
            }
        }

        initStuffBuff(&sb, 1024);
        addStringToStuffBuff(&sb, "<length=%d> <difficulty=%d> <fun=%d> <desc=%s>", s_radio1, s_radio2, s_radio3, estr);
        BugReport(sb.buff, BUG_REPORT_SURVEY);
        freeStuffBuff(&sb);

        window_setMode(WDW_BETA_COMMENT, WINDOW_SHRINKING);

        estrDestroy(&estr);
    }


    return 0;
}

/**********************************************************************func*
 * receiveMissionSurvey
 *
 */
void receiveMissionSurvey(Packet* pak)
{
#ifdef SHOW_MISSION_SURVEY
    window_setMode(WDW_BETA_COMMENT, WINDOW_GROWING);
#endif
}

/* End of File */

