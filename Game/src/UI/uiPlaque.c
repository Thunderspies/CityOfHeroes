#include <utilitieslib/utils/utils.h>
#include "UI/uiUtil.h"
#include "UI/uiWindows.h"
#include "UI/sprite/sprite_font.h"
#include "gameComm/wdwbase.h"
#include "UI/uiDialog.h"
#include "formatter/smf_format.h"
#include "uiSMFView.h"
#include "UI/uiUtilGame.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiChat.h"

static char** plaqueStrings = NULL;
static SMFView* plaqueViewer = NULL;
static char** plaqueTabNames = NULL;
static char** plaqueTabTexts = NULL;

TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)100,
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

static void plaquePopupPop(void * data);
static void plaquePopupLaunch(void);

static void plaqueChangeTab(void* data)
{
    int whichTab = (int)data;
    if (whichTab >= 0 && whichTab < eaSize(&plaqueTabTexts))
        smfview_SetText(plaqueViewer, plaqueTabTexts[whichTab]);
}

// If a plaque contains <tab name=PLAQUE_NAME> and </tab> setup a special plaque window
int plaqueShowDialog(void)
{
    float x, y, z, wd, ht, sc;
    int color, bcolor;

    if (!eaSize(&plaqueStrings))
        return 0;

    if(window_getMode(WDW_PLAQUE) != WINDOW_DISPLAYING)
        plaquePopupLaunch();    // this is messy, gets called every frame when a dialog-based plaque message is up
                                //        it won't get added to the dialog queue, though, because the message is already there

    if(!window_getDims(WDW_PLAQUE, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    // Draw the plaque window
    drawFrame(PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );
    smfview_SetSize(plaqueViewer, wd-(PIX3*2-5)*sc, ht-(PIX3+R10+32)*sc);
    smfview_Draw(plaqueViewer);

    // Draw OK, if hit close the window and destroy our current plaque display
    if (D_MOUSEHIT == drawStdButton(x+wd/2, y+ht-13*sc, z, 40*sc, 20*sc, color, "OkString", sc, 0))
    {
        window_setMode(WDW_PLAQUE, WINDOW_DOCKED);
        plaquePopupPop(0);
    }
    return 0;
}

static void plaquePopupLaunch(void)
{
    char *plaqueString = plaqueStrings[0];

    // See if this plaque needs to be tabbed
    if (strstri(plaqueString, "<tab name=") && strchr(plaqueString, '>') && strstri(plaqueString, "</tab>"))
    {
        char* plaqueParser = plaqueString; 
        char* nextBlockStart;
        char* nextBlockEnd;
        char* tabName;
        char lastChar;

        // If we dont have a plaque string currently, our window is closed, open it
        if (window_getMode(WDW_PLAQUE) != WINDOW_DISPLAYING)
        {
            window_setMode(WDW_PLAQUE, WINDOW_DISPLAYING);
            window_EnableCloseButton(WDW_PLAQUE, 0);
        }

        // Reparse the plaque text
        eaDestroyEx(&plaqueTabTexts, NULL);
        eaDestroyEx(&plaqueTabNames, NULL);
        while (plaqueParser && (tabName = strstri(plaqueParser, "<tab name=")) && (nextBlockEnd = strstri(plaqueParser, "</tab>")) && (nextBlockStart = strchr(plaqueParser, '>')))
        {
            // Parse up the block of text based on the <tab> </tab> format
            tabName += 10; // strlen("<tab name=") == 10
            plaqueParser = nextBlockEnd + 6; // strlen("</tab>") == 6;

            // Get the tab name
            lastChar = *nextBlockStart;
            *nextBlockStart = '\0';
            eaPush(&plaqueTabNames, strdup(tabName));
            *nextBlockStart = lastChar;

            // Get the tab block
            nextBlockStart++;
            lastChar = *nextBlockEnd;
            *nextBlockEnd = '\0';
            eaPush(&plaqueTabTexts, strdup(nextBlockStart));
            *nextBlockEnd = lastChar;
        }

        // If we didn't get anything out of that somehow, fail safely
        if (!eaSize(&plaqueTabTexts))
        {
            dialogStd(DIALOG_OK, plaqueString, NULL, NULL, plaquePopupPop, NULL, 0);
            return;
        }

        // Create an SMF viewer and setup the tabs
        if (!plaqueViewer)
            plaqueViewer = smfview_Create(WDW_PLAQUE);
        else
        {
            smfview_Destroy(plaqueViewer);
            plaqueViewer = smfview_Create(WDW_PLAQUE);
        }

        smfview_SetAttribs(plaqueViewer, &gTextAttr);
        smfview_AddTabs(plaqueViewer, plaqueTabNames, plaqueChangeTab);
    }
    else // This is a regular plaque, just throw up a dialog
    {
        dialogStd(DIALOG_OK, plaqueString, NULL, NULL, plaquePopupPop, NULL, 0);
    }
}

static void plaquePopupPop(void * data)
{
    if(eaSize(&plaqueStrings))
    {
        free(eaRemove(&plaqueStrings,0));
        if(eaSize(&plaqueStrings))
            plaquePopupLaunch();
    }
}

void plaquePopup(char* str, INFO_BOX_MSGS spam_chat)
{
    if(str)
    {
        int i;

        if(spam_chat)
            addSystemChatMsg(str,spam_chat,0);

        for(i = eaSize(&plaqueStrings)-1; i >= 0; i--)
            if(!strcmp(plaqueStrings[i],str))
                return;

        eaPush(&plaqueStrings,strdup(str));
        if(eaSize(&plaqueStrings) == 1)
            plaquePopupLaunch();
    }
}

void plaqueClearQueue(void)
{
    window_setMode(WDW_PLAQUE, WINDOW_DOCKED); // just in case
    eaDestroyEx(&plaqueStrings,0); // defaults to free
}

