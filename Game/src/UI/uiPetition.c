/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "gameComm/wdwbase.h"
#include <utilitieslib/utils/utils.h>
#include "comm_game.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/osdependent.h>

#include "graphics/textureatlas.h"
#include "win/input.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiWindows.h"
#include "UI/uiBox.h"
#include "UI/uiUtil.h"
#include "UI/uiGame.h"
#include "UI/uiUtilGame.h"
#include "UI/uiFocus.h"
#include "UI/uiScrollBar.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiInput.h"
#include "UIEdit.h"
#include "entity/entVarUpdate.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"

#include "UI/uiPetition.h"

#define TEXTAREA_LEN_SUMMARY    256
#define TEXTAREA_LEN_DESC        512



static int s_iCategory1;
static int s_iCategory2;
static UIEdit *s_peditSummary;
static UIEdit *s_peditDescription;

static char * s_pchSummary; // allows Summary field to be pre-set (for /bugs)
static PetitionWindowMode s_WindowMode = PETITION_NORMAL; // controls whether window handles Petitions or /bugs


/**********************************************************************func*
 * setPetitionMode
 *
 */
void setPetitionMode(PetitionWindowMode mode)
{
    s_WindowMode = mode;
}

/**********************************************************************func*
 * setPetitionSummary
 *
 */
void setPetitionSummary(const char * summary)
{
    if(s_pchSummary)
        free(s_pchSummary);

    s_pchSummary = strdup(summary);
}

/**********************************************************************func*
 * textarea_Edit
 *
 */
int textarea_Edit(UIEdit *pedit, float x, float y, float z, float wd, float ht, float sc)
{
    int iRet = TEXTAREA_SUCCESS;
    CBox box;
    int colorfg = CLR_NORMAL_FOREGROUND;
    int colorbg = CLR_NORMAL_BACKGROUND;
    UIBox uibox;

    BuildCBox(&box, x+sc*PIX3, y+sc*(PIX3), wd+sc*(-PIX3*2), ht+sc*(-PIX3*2));
    if(mouseCollision(&box))
    {
        if(mouseDown(MS_LEFT))
        {
            iRet = TEXTAREA_WANTS_FOCUS;
        }
    }

    if(uiInFocus(pedit))
    {
        KeyInput* input;

        // Handle keyboard input.
        input = inpGetKeyBuf();
         while(input)
        {
            if(input->key == INP_RETURN && windowUp(WDW_PETITION) )
            {
                iRet = TEXTAREA_LOSE_FOCUS;
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

       uiBoxDefine(&uibox, x+sc*PIX3, y+sc*PIX3, wd+sc*(-PIX3*2), ht+sc*(-PIX3*2));
     uiEditSetBounds(pedit, uibox);
     uiEditSetFontScale(pedit, sc);
    uiEditSetZ(pedit, z);
    uiEditProcess(pedit);

    return iRet;
}


/**********************************************************************func*
 * RadioButtons
 *
 */
int RadioButtons(float x, float y, float z, float sc, int iCurrentChoice, char *pchTitle, int iCntChoices, ...)
{
    int i;
    va_list args;

    font(&game_12);

    prnt(x, y, z, sc, sc, pchTitle);
    y+=sc*15;

    va_start(args, iCntChoices);

    for(i=0; i<iCntChoices; i++)
    {
        AtlasTex *ptex = atlasLoadTexture("context_checkbox_empty.tga");

        if(iCurrentChoice == i)
        {
            ptex = atlasLoadTexture("context_checkbox_filled.tga");
            display_sprite(ptex, x+sc*12, y-sc*12, z, sc, sc, CLR_GREEN);
        }
        else
        {
            ptex = atlasLoadTexture("context_checkbox_empty.tga");
            if(D_MOUSEHIT == drawGenericButton(ptex, x+sc*12, y-sc*12, z, sc, CLR_WHITE, CLR_GREEN, 1))
            {
                iCurrentChoice = i;
            }
        }

        if(selectableText(x+sc*30, y, z, sc, va_arg(args, char *), CLR_WHITE, CLR_WHITE, FALSE, TRUE))
        {
            iCurrentChoice = i;
        }
        y+=sc*15;
    }

    va_end(args);

    return iCurrentChoice;
}

static char* strPlatformMac = "Mac OS X: ";
static char* strPlatformPC = "Windows: ";

/**********************************************************************func*
 * petitionWindow
 *
 */
int petitionWindow(void)
{
    static UIEdit *s_ptaFocus;
    static bool init = true;
    float x, y, yorig, xorig, z, wd, ht, sc;
    int color, bcolor;
    int iRet;
    char * str;
    char * strPlatform = NULL;

    if(!window_getDims(WDW_PETITION, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
    {
        init = false;
        uiEditReturnFocus(s_peditSummary);
        uiEditReturnFocus(s_peditDescription);
        setPetitionMode(PETITION_NORMAL);        // default is 'petition' mode, /bug will override with setPetitionMode()
        return 0;
    }

    if(!init)
    {
        init = true;

        s_iCategory1 = -1;
        s_iCategory2 = 2;

        // if window opened by console command, insert user text into summary field
        if( s_WindowMode == PETITION_COMMAND_LINE || s_WindowMode == PETITION_BUG)
        {
            str = s_pchSummary;  
        }
        else
            str = 0;


        if(s_peditSummary)
            uiEditSetUTF8Text(s_peditSummary, str);
        else
        {
            s_peditSummary = uiEditCreateSimple(str, TEXTAREA_LEN_SUMMARY, &game_12, CLR_WHITE, NULL);
            s_peditSummary->minWidth = 9000; // Scroll the single line when editing.
        }

          if(s_peditDescription)
            uiEditClear(s_peditDescription);
        else
        {
            s_peditDescription = uiEditCreateSimple(NULL, TEXTAREA_LEN_DESC, &game_12, CLR_WHITE, NULL);
        }

        s_ptaFocus = s_peditSummary;
    }

    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor);

    if(window_getMode(WDW_PETITION) == WINDOW_DISPLAYING)
    {
        x+=sc*PIX3*2;
        y+=sc*18;
        wd+=sc*(-PIX3*4);
        ht+=sc*(-PIX3*2);
        yorig = y;
        xorig = x;

        font(&game_12);
        font_color(CLR_WHITE, CLR_WHITE);


        if(s_WindowMode != PETITION_BUG)
            s_iCategory1=RadioButtons(x, y, z, sc, s_iCategory1, "PetitionType", 3, "StuckString", "ExploitsandCheating", "FeedbackandSuggestions");
        else
            s_iCategory1=RadioButtons(x, y, z, sc, s_iCategory1, "BugType", 5, "ArtGraphics", "MissionContact", "PowersString", "TeamSupergroup", "InventionString");

        if(s_iCategory1 >= 0)
        {
            s_iCategory2 = -1;
        }

        x+=190*sc;

        if(s_WindowMode != PETITION_BUG)
            s_iCategory2=RadioButtons(x, y, z, sc, s_iCategory2, "", 3, "HarassmentandConduct", "TechnicalIssues", "GeneralHelp");
        else
            s_iCategory2=RadioButtons(x, y, z, sc, s_iCategory2, "", 3, "TextError", "SuggestionString", "OtherString");

        if(s_iCategory2 >= 0)
        {
            s_iCategory1 = -1;
        }


        x=xorig;
        y+=70*sc;

        if(s_WindowMode != PETITION_BUG)
        {
            if(D_MOUSEHIT==drawStdButton(x + wd/5, y, z, 120*sc, 25*sc, CLR_BLUE, "FileBugReport", sc, 0))
            {
                setPetitionMode(PETITION_BUG);
            }
        }

        y += 32*sc;

        prnt(x, y, z, sc, sc, "SummaryString");
        y+=5*sc;
        
        {
            bool focusChanged = false;
            iRet = textarea_Edit(s_peditSummary, x+sc*15, y, z, wd-sc*15, sc*25, sc);
            if(iRet==TEXTAREA_LOSE_FOCUS)
            {
                s_ptaFocus = s_peditDescription;
                focusChanged = true;
            }
            else if(iRet==TEXTAREA_WANTS_FOCUS)
            {
                s_ptaFocus = s_peditSummary;
                focusChanged = true;
            }
            
            y+=sc*(45);
            prnt(x, y, z, sc, sc, "FullDescription");
            y+=5*sc;
            
            iRet = textarea_Edit(s_peditDescription, x+sc*15, y, z, wd-sc*15, sc*100, sc);
            if(iRet==TEXTAREA_LOSE_FOCUS)
            {
                s_ptaFocus = s_peditSummary;
                focusChanged = true;
            }
            else if(iRet==TEXTAREA_WANTS_FOCUS)
            {
                s_ptaFocus = s_peditDescription;
                focusChanged = true;
            }

            if( focusChanged )
            {
                uiEditTakeFocus(s_ptaFocus);
            }
        }
        
        y+=sc*(107+15);

        str = ((s_WindowMode == PETITION_BUG) ? "SendBugReport" : "SendPetition");
        if(D_MOUSEHIT==drawStdButton(x + 4*wd/5, y, z, 120*sc, 25*sc, CLR_GREEN, str, sc, 0))
        {
            StuffBuff sb;
            char *pch;
            int iChoice;
            char *estrSummary;
            char *estrDesc;

            // Clean up the strings. No \r, \n, or <>
            estrSummary = uiEditGetUTF8Text(s_peditSummary);
            if(estrSummary)
            {
                for(pch=estrSummary; *pch!='\0'; pch++)
                {
                    if(*pch=='\r' || *pch=='\n' || *pch=='<' || *pch=='>')
                    {
                        *pch = ' ';
                    }
                }
            }

            estrDesc = uiEditGetUTF8Text(s_peditDescription);
            if(estrDesc)
            {
                for(pch=estrDesc; *pch!='\0'; pch++)
                {
                    if(*pch=='\r' || *pch=='\n' || *pch=='<' || *pch=='>')
                    {
                        *pch = ' ';
                    }
                }
            }

            iChoice = s_iCategory1;
            if(iChoice<0)
            {
                if(s_WindowMode == PETITION_BUG)
                    iChoice = 4+s_iCategory2;
                else
                    iChoice = 3+s_iCategory2;

            }

            if(s_WindowMode != PETITION_BUG)
                iChoice++; // no longer using '0 == Bug Report' checkbox, so increase all categories by to one remain compatible

            initStuffBuff(&sb, 1024);
            if (IsUsingCider())
                strPlatform = strPlatformMac;
            else
                strPlatform = strPlatformPC;

            if(s_WindowMode != PETITION_BUG)
                addStringToStuffBuff(&sb, "petition_internal <cat=%d> <summary=%s> <desc=%s%s>", iChoice, estrSummary?estrSummary:"", strPlatform?strPlatform:"", estrDesc?estrDesc:"");
            else
                addStringToStuffBuff(&sb, "bug_internal 1 <cat=%d> <summary=%s> <desc=%s%s>", iChoice, estrSummary?estrSummary:"", strPlatform?strPlatform:"", estrDesc?estrDesc:"");

            cmdParse(sb.buff);
            freeStuffBuff(&sb);

            setPetitionSummary(""); // clear for next time

            if(estrSummary)
                estrDestroy(&estrSummary);
            if(estrDesc)
                estrDestroy(&estrDesc);

            collisions_off_for_rest_of_frame = true;
            window_setMode(WDW_PETITION, WINDOW_SHRINKING);
        }
         if(D_MOUSEHIT==drawStdButton(x + 1*wd/5, y, z, 120*sc, 25*sc, CLR_RED, "CancelString", sc, 0))
        {
            collisions_off_for_rest_of_frame = true;
            window_setMode(WDW_PETITION, WINDOW_SHRINKING);
        }
     }

    return 0;
}

/* End of File */

