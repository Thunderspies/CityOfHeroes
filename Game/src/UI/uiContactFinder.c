#include <utilitieslib/stdtypes.h>
#include "UI/uiContactFinder.h"
#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "formatter/smf_main.h"
#include "UI/uiScrollBar.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/EString.h>
#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_base.h"
#include "gameComm/Npc.h"
#include "storyarc/playerCreatedStoryarcClient.h"
#include "graphics/seqgraphics.h"

static int s_contactFinder_imageNumber = 0;
static char *s_contactFinder_displayName = NULL;
static char *s_contactFinder_profession = NULL;
static char *s_contactFinder_locationName = NULL;
static char *s_contactFinder_description = NULL;

static bool s_contactFinder_showPrev = 1;
static bool s_contactFinder_showTele = 1;
static bool s_contactFinder_showNext = 1;

static ScrollBar contactFinderScrollBar = {WDW_CONTACT_FINDER, 0};

static SMFBlock *contactFinderSMF_image = 0;
static SMFBlock *contactFinderSMF_name = 0;
static int contactFinderSMF_nameHeight = 0;
static float contactFinderSMF_nameScale = 0.0f;
static SMFBlock *contactFinderSMF_description = 0;
static SMFBlock *contactFinderSMF_relation = 0;

static void initializeContactFinderSMFBlocks(void)
{
    if (!contactFinderSMF_image)
    {
        contactFinderSMF_image = smfBlock_Create();
        smf_SetFlags(contactFinderSMF_image, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespace,
            SMFInputMode_None, 0, SMFScrollMode_ExternalOnly, SMFOutputMode_StripNoTags,
            SMFDisplayMode_AllCharacters, SMFContextMenuMode_Copy, SMAlignment_Left, 0, 0, 0);
        contactFinderSMF_name = smfBlock_Create();
        smf_SetFlags(contactFinderSMF_name, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespace,
            SMFInputMode_None, 0, SMFScrollMode_ExternalOnly, SMFOutputMode_StripNoTags,
            SMFDisplayMode_AllCharacters, SMFContextMenuMode_Copy, SMAlignment_Left, 0, 0, 0);
        contactFinderSMF_description = smfBlock_Create();
        smf_SetFlags(contactFinderSMF_description, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespace,
            SMFInputMode_None, 0, SMFScrollMode_ExternalOnly, SMFOutputMode_StripNoTags,
            SMFDisplayMode_AllCharacters, SMFContextMenuMode_Copy, SMAlignment_Left, 0, 0, 0);
        contactFinderSMF_relation = smfBlock_Create();
        smf_SetFlags(contactFinderSMF_relation, SMFEditMode_ReadOnly, SMFLineBreakMode_MultiLineBreakOnWhitespace,
            SMFInputMode_None, 0, SMFScrollMode_ExternalOnly, SMFOutputMode_StripNoTags,
            SMFDisplayMode_AllCharacters, SMFContextMenuMode_Copy, SMAlignment_Center, 0, 0, 0);
    }
}

void rebuildContactFinderSMFStrings()
{
    char *temp = NULL;
    float scale;

    window_getDims(WDW_CONTACT_FINDER, 0, 0, 0, 0, 0, &scale, 0, 0);

    estrPrintf(&temp, "<img border=0 src=npc:%d>", s_contactFinder_imageNumber);
    smf_SetRawText(contactFinderSMF_image, temp, 0);

    estrPrintf(&temp, "<font scale=%f>%s</font><br><br>%s<br>%s",
        1.5f * scale, textStd(s_contactFinder_displayName), textStd(s_contactFinder_profession), textStd(s_contactFinder_locationName));
    smf_SetRawText(contactFinderSMF_name, temp, 0);
    contactFinderSMF_nameHeight = -1;

    smf_SetRawText(contactFinderSMF_description, textStd(s_contactFinder_description), 0);
}

void contactFinderSetData(int imageNumber, char *displayName, char *profession, char *locationName, char *description, bool showPrev, bool showTele, bool showNext)
{
    initializeContactFinderSMFBlocks();

    s_contactFinder_imageNumber = imageNumber;
    
    estrPrintCharString(&s_contactFinder_displayName, displayName);
    estrPrintCharString(&s_contactFinder_profession, profession);
    estrPrintCharString(&s_contactFinder_locationName, locationName);
    estrPrintCharString(&s_contactFinder_description, description);

    s_contactFinder_showPrev = showPrev;
    s_contactFinder_showTele = showTele;
    s_contactFinder_showNext = showNext;

    if (s_contactFinder_showTele)
    {
        smf_SetRawText(contactFinderSMF_relation, textStd("ContactFinderUnknownContact"), 0);
    }
    else
    {
        smf_SetRawText(contactFinderSMF_relation, textStd("ContactFinderKnownContact"), 0);
    }

    contactFinderScrollBar.offset = 0;

    rebuildContactFinderSMFStrings();

    if (window_getMode(WDW_CONTACT_FINDER) != WINDOW_DISPLAYING)
    {
        window_setMode(WDW_CONTACT_FINDER, WINDOW_GROWING);
    }
    else
    {
        window_bringToFront(WDW_CONTACT_FINDER);
    }

    if (window_getMode(WDW_CONTACT) != WINDOW_DOCKED)
    {
        window_setMode(WDW_CONTACT, WINDOW_SHRINKING);
    }
}

int contactFinderWindow()
{
    float x, y, z, wd, ht, scale, xscale, yscale;
    int color, bcolor;
    static int totalSMFHeight;
    static int nameSMFHeight;
    static AtlasTex *contactImage;
    AtlasTex *fadeBar = atlasLoadTexture("Contact_fadebar_R.tga");
    int outsideChatScale = (int) gTextAttr_White12.piScale;
    const cCostume *pCostume = NULL;
    AtlasTex *icon = NULL;

    // Rebuild the string once the window is fully displayed and scale is correct,
    // so the contact's name will be displayed at the correct size.
    static int needToRebuildSMFStrings = 1;

    if (!window_getDims(WDW_CONTACT_FINDER, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor))
    {
        needToRebuildSMFStrings = 1;
        return 0;
    }

    bcolor |= 0x5f;

    drawFrame( PIX3, R10, x, y, z-2, wd, ht, scale, color, bcolor );

    if (window_getMode(WDW_CONTACT_FINDER) != WINDOW_DISPLAYING)
    {
        needToRebuildSMFStrings = 1;
        return 0;
    }

    gTextAttr_White12.piScale = (int *)(int)(scale*SMF_FONT_SCALE);

    initializeContactFinderSMFBlocks();

    if (contactFinderSMF_nameScale != scale || needToRebuildSMFStrings)
    {
        rebuildContactFinderSMFStrings();
    }

    needToRebuildSMFStrings = 0;

    doScrollBar(&contactFinderScrollBar, ht - 230*scale, totalSMFHeight, 0, 160*scale, z, 0, 0);

    // deal with the below commented lines if we need to get imageOverride to work
    // (this information isn't sent from the server right now as well)
//     if (cs->imageOverride && stricmp(cs->imageOverride, "none"))
//     {
//         icon = atlasLoadTexture(cs->imageOverride);
//     }
//     else if (!cs->imageOverride && s_contactFinder_imageNumber)
    {
        pCostume = npcDefsGetCostume(s_contactFinder_imageNumber, 0);
    }

    if (pCostume!=NULL)
    {
        icon = seqGetHeadshot( pCostume, 0 );
    }

    if (icon)
    {
        F32 iconScale = MIN(128*scale/icon->width, 128*scale/icon->height);

        display_sprite(icon, x + 10*scale, y + 10*scale, z, iconScale, iconScale, CLR_WHITE );
    }

    xscale = (wd - 148*scale) / fadeBar->width;
    yscale = 128*scale / fadeBar->height;
    display_sprite(fadeBar, x + 138*scale, y + 10*scale, z, xscale, yscale, color);

    smf_SetScissorsBox(contactFinderSMF_name, x + 160*scale, y + 10*scale, wd - 170*scale, 128*scale);
    if (contactFinderSMF_nameHeight == -1)
    {
        contactFinderSMF_nameHeight = smf_ParseAndFormat(contactFinderSMF_name, contactFinderSMF_name->rawString,
            x + 160*scale, y + 74*scale - contactFinderSMF_nameHeight/2, z, wd - 170*scale, 0, 0, 0, 0, &gTextAttr_White12, 0);
    }
    smf_Display(contactFinderSMF_name, x + 160*scale, y + 74*scale - contactFinderSMF_nameHeight/2, z, wd - 170*scale, 0, 0, 0, &gTextAttr_White12, 0);

    drawFrame( PIX3, R10, x, y, z-2, wd, 150*scale, scale, color, 0 );

    smf_SetScissorsBox(contactFinderSMF_description, x + 10, y + 150*scale, wd - 20, ht - 208*scale);
    totalSMFHeight = smf_Display(contactFinderSMF_description, x + 10, y + 160*scale - contactFinderScrollBar.offset, z, wd - 20, 0, 0, 0, &gTextAttr_White12, 0);

    drawHorizontalLine(x + 3*scale, y + ht - 58*scale, wd - 6*scale, z, scale, color);

    smf_SetScissorsBox(contactFinderSMF_relation, x + 10, y + ht - 60*scale, wd - 20, 20*scale);
    smf_Display(contactFinderSMF_relation, x + 10, y + ht - 55*scale, z, wd - 20, 0, 0, 0, &gTextAttr_White12, 0);

    if (s_contactFinder_showPrev)
    {
        if (D_MOUSEHIT == drawStdButton(x + 40*scale, y + ht - 25*scale, z, 60*scale, 25*scale, color, "PreviousContactButton", scale, 0))
        {
            cmdParse("contactfinder_showprevious");
        }
    }

    if (s_contactFinder_showTele)
    {
        if (D_MOUSEHIT == drawStdButton(x + wd / 2, y + ht - 25*scale, z, 150*scale, 25*scale, color, "TeleportToContactButton", scale, 0))
        {
            cmdParse("contactfinder_teleporttocurrent");
            window_setMode(WDW_CONTACT_FINDER, WINDOW_SHRINKING);
        }
    }
    else
    {
        if (D_MOUSEHIT == drawStdButton(x + wd / 2, y + ht - 25*scale, z, 150*scale, 25*scale, color, "SelectContactButton", scale, 0))
        {
            cmdParse("contactfinder_selectcurrent");
            window_setMode(WDW_CONTACT_FINDER, WINDOW_SHRINKING);
        }
    }

    if (s_contactFinder_showNext)
    {
        if (D_MOUSEHIT == drawStdButton(x + wd - 40*scale, y + ht - 25*scale, z, 60*scale, 25*scale, color, "NextContactButton", scale, 0))
        {
            cmdParse("contactfinder_shownext");
        }
    }

    gTextAttr_White12.piScale = (int*) outsideChatScale;
    contactFinderSMF_nameScale = scale;

    return 0;
}
