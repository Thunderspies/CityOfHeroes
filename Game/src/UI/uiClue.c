/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "assert.h"
#include <utilitieslib/components/earray.h>
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/mathutil.h>
#include "storyarc/storyarcClient.h"

#include "gameComm/wdwbase.h"

#include "UI/uiUtil.h"
#include "UI/uiClue.h"
#include "UI/uiInput.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiWindows.h"
#include "UI/uiContact.h"
#include "UI/uiScrollBar.h"
#include "UI/uiBox.h"
#include "UI/uiClipper.h"
#include "UI/uiConsole.h"
#include "UI/uiDialog.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"

#include "formatter/smf_main.h"
#include "edit/ClickToSource.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/sysutil.h>
#include "cmdparse/cmdgame.h"
#include "storyarc/playerCreatedStoryarcValidate.h"


typedef enum
{
    CCT_CLUE,
    CCT_SOUVENIR,
    CCT_MM_SOUVENIR,
    CCT_COUNT,
} ClueContentType;
static ClueContentType contentType;

static TextAttribs s_taDefaults =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};


// Caches the formatting for each contact. Parsing and formatting are rather
// slow, so it's good to cache it.
static SMFBlock **clueFormatCache = NULL;



//------------------------------------------------------------------------

#define CLUE_HT        64
#define CLUE_SPACE    5

#define FADE_WIDTH  (150.0f)
#define FADE_HEIGHT (28.0f)
#define ICON_WIDTH  (32.0f)
#define ICON_HEIGHT (32.0f)

static int gSelectedClue = -1;

#define BORDER_SPACE    15
#define TAB_HEIGHT        15

#define TAB_INSET       20 // amount to push tab in from edge of window
#define BACK_TAB_OFFSET 3  // offset to kee back tab on the bar
#define OVERLAP         5  // amount to overlap tabs

//----------------------------------------------------------------------------------------------
// Souvenir clues
//----------------------------------------------------------------------------------------------

typedef struct
{
    SMFBlock *formatCache;
    int expanded;
} SouvenirDisplayState;

typedef struct SouvenirData
{
    int lastExpandedItemUID;
    ScrollBar souvenirScollbar;
    int player_created;
    SouvenirDisplayState** souvenirDisplayStates;
    SouvenirClue** souvenirClues;
}SouvenirData;

SouvenirData souvenirData[2] = { { -1, { WDW_CLUE, }, }, { -1, { WDW_CLUE, }, 1, } };


TokenizerParseInfo parsePlayerSouvenirClue[] =
{
    { "",                TOK_STRUCTPARAM|TOK_INT(SouvenirClue, uid, 0 ) },
    { "Title",            TOK_STRING(SouvenirClue, displayName, 0 ) },
    { "Description",    TOK_STRING(SouvenirClue, description, 0 ) },
    { "End",            TOK_END },
    {0},
};

TokenizerParseInfo parseSouvenirData[] =
{
    { "Souvenir",    TOK_STRUCT(SouvenirData, souvenirClues, parsePlayerSouvenirClue ) },
    {0},
};

static void scDestroy(SouvenirClue* clue)
{
    if(!clue)
        return;
    SAFE_FREE(clue->name);
    SAFE_FREE(clue->displayName);
    SAFE_FREE(clue->icon);
    SAFE_FREE(clue->description);
    free(clue);
}


static void souvenirClueDelete( SouvenirClue * pClue )
{
    SouvenirData *pData = &souvenirData[1];
     int idx = eaFindAndRemove(&souvenirData[1].souvenirClues, pClue);
    SouvenirDisplayState *pState = eaRemove( &souvenirData[1].souvenirDisplayStates, idx );
    char * file;
    char dir[MAX_PATH];

    if( pState )
    {
        if( pState->formatCache )
            smfBlock_Destroy(pState->formatCache);
        SAFE_FREE(pState);
    }
    StructDestroy(parsePlayerSouvenirClue, pClue);


    estrCreate(&file);
    estrConcatf(&file, "%s/PlayerCreatedSouvenirClues.txt", getExecutableDir(dir) );
    ParserWriteTextFile(file, parseSouvenirData, &souvenirData[1], 0, 0);
    estrDestroy(&file);
}

void loadPlayerCreatedSouvenirClues()
{
    char * file;
    char dir[MAX_PATH];

    estrCreate(&file);
    estrConcatf(&file, "%s/PlayerCreatedSouvenirClues.txt", getExecutableDir(dir) );
    ParserLoadFiles(NULL, file, NULL, 0, parseSouvenirData, &souvenirData[1], NULL, NULL, NULL);
    estrDestroy(&file);
}


static void collapseAllSouvenirClues(SouvenirData *pData)
{
    int i;
    for(i = eaSize(&pData->souvenirDisplayStates)-1; i >= 0; i--)
    {
        pData->souvenirDisplayStates[i]->expanded = 0;
    }
}

static void expandSouvenirClue(SouvenirData *pData, int uid)
{
    int i;
    for(i = eaSize(&pData->souvenirClues)-1; i >= 0; i--)
    {
        if(pData->souvenirClues[i]->uid == uid)
        {
            assert(eaSize(&pData->souvenirDisplayStates) > i);
            pData->souvenirDisplayStates[i]->expanded = 1;
        }
    }
}


#define COLLAPSED_CLUE_HEIGHT ICON_HEIGHT+PIX3*2+4
void scrollToSouvenirClue(SouvenirData *pData, int uid)
{
    int i;
    for(i = eaSize(&pData->souvenirClues)-1; i >= 0; i--)
    {
        if(pData->souvenirClues[i]->uid == uid)
        {
            // Assuming that all clues are collapsed, where should the scrollbar be to
            // see this clue?
            pData->souvenirScollbar.offset = (COLLAPSED_CLUE_HEIGHT + CLUE_SPACE) * i;
        }
    }
}

void souvinerClueAdd( SouvenirClue* clue )
{
    SouvenirData *pData = &souvenirData[0];
    eaPush(&pData->souvenirClues, clue);
}

void addPlayerCreatedSouvenir(int uid, char * title, char * description )
{
    SouvenirData *pData = &souvenirData[1];
    SouvenirClue* clue=0;
    int i;
    char * file;
    char dir[MAX_PATH];

    // remove the clue being updated.
    for(i = eaSize(&pData->souvenirClues)-1; i >= 0; i--)
    {
        if(pData->souvenirClues[i]->uid == uid)
            clue = pData->souvenirClues[i];
    }

    if(!clue)
        clue = StructAllocRaw(sizeof(SouvenirClue));

    if( title && title[0] )
    {
        if( clue->displayName )
            StructFreeStringConst(clue->displayName);
        clue->displayName = StructAllocString(title);
    }

    if( description && description[0] )
    {
        if( clue->description )
            StructFreeStringConst(clue->description);
        clue->description = StructAllocString(description);
    }
    eaPush(&pData->souvenirClues, clue);

    estrCreate(&file);
    estrConcatf(&file, "%s/PlayerCreatedSouvenirClues.txt", getExecutableDir(dir) );
    ParserWriteTextFile(file, parseSouvenirData, &souvenirData[1], 0, 0);
    estrDestroy(&file);
}

void updateSouvenirClue( int uid, char * description )
{
    int i;
    SouvenirClue* clue;    
    SouvenirData *pData = &souvenirData[0];

    // Find the clue being updated.
    for(i = eaSize(&pData->souvenirClues)-1; i >= 0; i--)
    {
        if(pData->souvenirClues[i]->uid == uid)
            break;
    }

    // Didn't find the clue to be updated?
    if(i == eaSize(&pData->souvenirClues))
        return;

    clue = pData->souvenirClues[i];
    SAFE_FREE(clue->description);
    clue->description = strdup(description);

    scrollToSouvenirClue(pData, clue->uid);

}

SouvenirClue* getClueByID( int uid )
{
    SouvenirData *pData = &souvenirData[0];
    int i;
    // Find the clue being updated.
    for(i = eaSize(&pData->souvenirClues)-1; i >= 0; i--)
    {
        if(pData->souvenirClues[i]->uid == uid)
            return pData->souvenirClues[i];
    }
    return 0;
}


static void scDestroyAdapter(void* arg0)
{
    scDestroy((SouvenirClue*)arg0);
}

void souvenirCluePrepareUpdate()
{
    int i;
    SouvenirData *pData = &souvenirData[0];
    // Record the uid of the first expanded item.  It should be the only
    // expanded item.
    for(i = eaSize(&pData->souvenirDisplayStates)-1; i >= 0; i--)
    {
        if(pData->souvenirDisplayStates[i]->expanded)
        {
            assert(eaSize(&pData->souvenirClues) > i);
            pData->lastExpandedItemUID = pData->souvenirClues[i]->uid;
        }
    }

    eaClearEx(&pData->souvenirClues, scDestroyAdapter);

    collapseAllSouvenirClues(pData);
}

void scPrintAll(void)
{
    int i;
    SouvenirData *pData = &souvenirData[0];
    int iSize = eaSize(&pData->souvenirClues);

    if(iSize==0)
    {
        conPrintf("No souvenir clues\n");
        return;
    }

    for(i = 0; i < iSize; i++)
    {
        SouvenirClue* clue = pData->souvenirClues[i];
        conPrintf("%2i\t Name: %s\n\tIcon: %s\n\tDescription: %s\n", i, clue->displayName, clue->icon, clue->description);
    }
    conPrintf("\n");
}



static void displayTabs()
{
    float x, y, z, wd, ht, scale;
    int i, color, bcolor, count = CCT_COUNT;
    CBox box;
    UIBox uibox;

    static AtlasTex *active_l, *active_m, *active_r, *inactive_l, *inactive_m, *inactive_r; 
    static char * tabNames[] = { "ClueTab", "SouvenirTab", "MMSouvenirTab" };
 
     if( !active_l )
    {
        active_l = atlasLoadTexture( "map_tab_active_L.tga" );
        active_m = atlasLoadTexture( "map_tab_active_MID.tga" );
        active_r = atlasLoadTexture( "map_tab_active_R.tga" );

        inactive_l = atlasLoadTexture( "map_tab_inactive_L.tga" );
        inactive_m = atlasLoadTexture( "map_tab_inactive_MID.tga" );
        inactive_r = atlasLoadTexture( "map_tab_inactive_R.tga" );
    }

    if( !window_getDims( WDW_CLUE, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor) )
        return;

    uiBoxDefine(&uibox, x + PIX3*scale, y + PIX3*scale, wd - 2*PIX3*scale, ht - 2*PIX3*scale );
    clipperPush(&uibox);

      x += TAB_INSET*scale;
    font( &game_12 );

    //count = CCT_MM_SOUVENIR;

    for( i = 0; i < count; i++ )
    {
        AtlasTex *l, *m, *r;
        F32 text_wd;
        F32 sc = scale, tz = z, ty = y;
 
        if( i == contentType )
        {
            font_color( CLR_WHITE, CLR_WHITE );
             l = active_l;
            m = active_m;
            r = active_r;
            tz += 5;
             sc = 1.1*scale;
        }
        else
        {
            font_color( 0xaaaaaaff, 0xaaaaaaff );
            l = inactive_l;
            m = inactive_m;
            r = inactive_r;
            tz += 4;
        }

         text_wd = str_wd(font_grp, 1.1*scale, 1.1*scale, tabNames[i]);

          display_sprite( l, x ,ty, tz, scale, scale, color);
        display_sprite( m, x + l->width*scale, ty, tz, text_wd/m->width, scale, color);
        display_sprite( r, x + l->width*scale + text_wd, ty, tz, scale, scale, color);

          cprnt( x + l->width*scale + text_wd/2, ty + 17*scale, tz, sc, sc, tabNames[i] );
  
        BuildCBox(&box, x, y, text_wd + (l->width + r->width - OVERLAP)*scale, m->height*scale );
        if(mouseDownHit(&box, MS_LEFT))
            contentType = i;

        x += text_wd + (l->width + r->width - OVERLAP)*scale;
    }

    clipperPop();
}

//----------------------------------------------------------------------------------------------
// Task clues
//----------------------------------------------------------------------------------------------
void displayClue( StoryClue* clue, int cacheIndex, int xorig, float * y, int z, int wdorig, float sc )
{
    int ht = 0;
     AtlasTex *icon = atlasLoadTexture(clue->iconfile);
    AtlasTex *fade;
    AtlasTex *mid;
    float xscale;
    float yscale;
    int x = xorig;
    int wd = wdorig;
    int foreColor = CLR_NORMAL_FOREGROUND;
    int backColor = CLR_NORMAL_BACKGROUND;
    UIBox box;
 


    *y += PIX3*sc;
     x  += (PIX3+6)*sc;
    wd -= (PIX3*2+6)*sc;

     uiBoxDefine(&box, xorig, *y, wdorig - PIX3*sc, 5000);
    clipperPushRestrict(&box);

    if(stricmp(icon->name, "white")==0)
    {
        icon = atlasLoadTexture( "icon_clue_generic.tga" );
    }

    fade = atlasLoadTexture("Contact_fadebar_R.tga");
    xscale = FADE_WIDTH*sc/fade->width;
    yscale = FADE_HEIGHT*sc/fade->height;
     display_sprite(fade, x + wd - FADE_WIDTH*sc, *y+4*sc, z, xscale, yscale, 0x00000055);

    mid = atlasLoadTexture("Contact_fadebar_MID.tga");
    xscale = (wd - (FADE_WIDTH + ICON_WIDTH - 5)*sc)/mid->width; // +5 because it needs to go under the icon some.
    yscale = FADE_HEIGHT*sc/mid->height;
    display_sprite(mid, x+(ICON_WIDTH-5)*sc, *y+4*sc, z, xscale, yscale, 0x00000055);

    display_sprite(icon, x, *y+4*sc, z, ICON_WIDTH*sc/icon->width, ICON_HEIGHT*sc/icon->height, CLR_WHITE );

    x  += (CLUE_SPACE*2 + icon->width)*sc;
    wd -= (CLUE_SPACE*2 + icon->width)*sc;

    font( &game_14 );
    font_color(0x00deffff, 0x00deffff);
    if(clue->displayname)
        prnt( x + 10*sc, *y + 24*sc, z, .90f*sc, .90f*sc, smf_DecodeAllCharactersGet(clue->displayname));

    if (CTS_SHOW_STORY)
        ClickToSourceDisplay(x + 10*sc, *y + 32*sc, z, 0, 0xffffffff, clue->filename, NULL, CTS_TEXT_REGULAR);

    if(clue->detailtext)
    {
        s_taDefaults.piScale = (int*)((int)(sc*SMF_FONT_SCALE));
        ht = smf_ParseAndDisplay(clueFormatCache[cacheIndex],
            clue->detailtext,
            x+10*sc, *y+33*sc, z,
            wd-(10+CLUE_SPACE)*sc, 0,
            false, false, &s_taDefaults, NULL, 
            0, true);
    }
    
    ht = max(ht+(35+CLUE_SPACE)*sc, max((ICON_HEIGHT+PIX3*2+4+4)*sc, (CLUE_HT-5)*sc));
    clipperPop();
    drawFlatFrame(PIX2, R10, xorig, *y-PIX3*sc, z-1, wdorig, ht, sc, foreColor, backColor);

    *y += ht;
    *y += CLUE_SPACE*sc;
}


void clueTab()
{
    float x, y, z, wd, ht, scale;
    int i, color, bcolor;
    float oldy;
    int iSize, numKeys;
    static ScrollBar sb = { WDW_CLUE, 0 };

    if(!window_getDims(WDW_CLUE, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor))
        return;

    oldy = y;

    set_scissor(1);
    scissor_dims( x + PIX3*scale, y + PIX3*scale, wd - PIX3*2*scale, ht - PIX3*2*scale );

    y += (BORDER_SPACE + TAB_HEIGHT)*scale - sb.offset;

    // Make sure the formatting array is large enough.
    i = eaSize(&storyClues) + eaSize(&keyClues) - eaSize(&clueFormatCache);
    while(i>0)
    {
        eaPush(&clueFormatCache, smfBlock_Create());
        i--;
    }

    // display key clues first
    numKeys = eaSize(&keyClues);
    for (i = 0; i < numKeys; i++)
    {
        displayClue( keyClues[i], i, x + BORDER_SPACE*scale, &y, z, wd - BORDER_SPACE*2*scale, scale );
    }

    // Display rest of clues in order
    iSize = eaSize(&storyClues);
    for( i = 0; i < iSize; i++ )
    {
        displayClue( storyClues[i], i+numKeys, x + BORDER_SPACE*scale, &y, z, wd - BORDER_SPACE*2*scale, scale );
    }

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
    if( eaSize(&storyClues) == 0 && eaSize(&keyClues) == 0 )
        cprnt( x + wd/2, y+15*scale, z, scale, scale, "YouAreClueless" );

    set_scissor( 0 );

       doScrollBar( &sb, ht - (PIX3*2 + R10*2)*scale,(y-oldy) + sb.offset, wd, (PIX3+R10)*scale, z, 0, 0 );
}




void displaySouvenirClue( SouvenirData *pData, int idx, int xorig, float * y, int z, int wdorig, float sc, int deletable )
{
    SouvenirClue* clue = pData->souvenirClues[idx];
    SouvenirDisplayState* state = pData->souvenirDisplayStates[idx];
    int ht = 0;
    AtlasTex *icon = atlasLoadTexture(clue->icon?clue->icon:"icon_clue_generic.tga");
    AtlasTex *fade;
    AtlasTex *mid;
    float xscale;
    float yscale;
    int x = xorig;
    int wd = wdorig;
    int foreColor = CLR_NORMAL_FOREGROUND;
    int backColor = CLR_NORMAL_BACKGROUND;

    *y += PIX3*sc;
    x  += (PIX3+6)*sc;
    wd -= (PIX3*2+6)*sc;

    if(stricmp(icon->name, "white")==0)
    {
        icon = atlasLoadTexture( "icon_clue_generic.tga" );
    }

    fade = atlasLoadTexture("Contact_fadebar_R.tga");
    xscale = FADE_WIDTH*sc/fade->width;
    yscale = FADE_HEIGHT*sc/fade->height;
    display_sprite(fade, x + wd - FADE_WIDTH*sc, *y+4*sc, z, xscale, yscale, 0x00000055);

    mid = atlasLoadTexture("Contact_fadebar_MID.tga");
    xscale = (wd - (FADE_WIDTH + ICON_WIDTH - 5)*sc)/mid->width; // +5 because it needs to go under the icon some.
    yscale = FADE_HEIGHT*sc/mid->height;
    display_sprite(mid, x+(ICON_WIDTH-5)*sc, *y+4*sc, z, xscale, yscale, 0x00000055);

    display_sprite(icon, x, *y+4*sc, z, ICON_WIDTH*sc/icon->width, ICON_HEIGHT*sc/icon->height, CLR_WHITE );

    x  += (CLUE_SPACE*2 + icon->width)*sc;
    wd -= (CLUE_SPACE*2 + icon->width)*sc;

    font( &game_14 );
    font_color(0x00deffff, 0x00deffff);
     if(clue->displayName)
        prnt( x + 10*sc, *y + 24*sc, z, .90f*sc, .90f*sc, smf_DecodeAllCharactersGet(clue->displayName));

    if (CTS_SHOW_STORY)
        ClickToSourceDisplay(x + 10*sc, *y + 32*sc, z, 0, 0xffffffff, clue->filename, NULL, CTS_TEXT_REGULAR);

    // If the clue is not expanded, display a button for the user to get more details on the clue.
    if(!state->expanded)
    {
        ht = max(ht+(35+CLUE_SPACE+4+4)*sc, COLLAPSED_CLUE_HEIGHT*sc);

        if(D_MOUSEHIT == drawStdButton(x+290*sc, *y+ht-5*sc, (float)z, 25*sc, 20*sc, window_GetColor(WDW_CLUE), ">>", sc, 0))
        {
            // If the clue description is not immediately available...
            if(!clue->description)
            {
                // Send a request for clue details.
                scRequestDetails(clue);
            }

            collapseAllSouvenirClues(pData);
            scrollToSouvenirClue(pData, clue->uid);
            // Try to scroll the clue to the top of the window.
            // This may not do anything if the clues do not exceed the window display area.

            state->expanded = 1;
        }
    }
    else
    {
        const char* str = clue->description;

        if(!str)
        {
            if( !deletable )
                str = textStd("WaitingServerUpdate");
            else
                str = "";
        }
            

        if( window_getMode(WDW_CLUE) == WINDOW_DISPLAYING)
        {
            s_taDefaults.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
            ht = smf_ParseAndDisplay(state->formatCache, str, x+10*sc, *y+33*sc, z, wd-(10+CLUE_SPACE)*sc, 0, false, false, &s_taDefaults, NULL, 0, true);
        }

        // Scroll the clue being displayed to the top again when the description becomes available.
        if(state->expanded == 1 && clue->description)
        {
            scrollToSouvenirClue(pData, clue->uid);
            state->expanded = 2;
        }

        ht = max(ht+(35+CLUE_SPACE+4+4)*sc, COLLAPSED_CLUE_HEIGHT*sc);
        if(D_MOUSEHIT == drawStdButton(x+290*sc, *y+ht-5*sc, (float)z, 25*sc, 20*sc, window_GetColor(WDW_CLUE), "<<", sc, 0))
        {
            state->expanded = 0;
        }
    }

    drawFlatFrame(PIX2, R10, xorig, *y-PIX3*sc, z-1, wdorig, ht, sc, foreColor, backColor);

    if( deletable && drawCloseButton( xorig + wdorig - 12*sc,  *y + 10*sc, z, sc, backColor)  )  
        dialogStdCB(DIALOG_ACCEPT_CANCEL, "MMSouvenirReallyDelete", 0, 0, souvenirClueDelete, 0, 0, clue );

    *y += ht;
    *y += CLUE_SPACE;
}

void souvenirClueTab( int souvenirDataIdx )
{
    float x, y, z, wd, ht, scale;
    int i, color, bcolor;
    float oldy;
    int iSize;
    SouvenirData * pData = &souvenirData[souvenirDataIdx];

    if(!window_getDims(WDW_CLUE, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor))
        return;

    oldy = y;

    set_scissor(1);
     scissor_dims( x + PIX3*scale, y + (20+PIX3)*scale, wd - PIX3*2*scale, ht - (20+PIX3*2)*scale );

    // Make sure the formatting array is large enough.
    i = eaSize(&pData->souvenirClues) - eaSize(&pData->souvenirDisplayStates);
    while(i>0)
    {
        SouvenirDisplayState *state = calloc(sizeof(SouvenirDisplayState), 1);
        state->formatCache = smfBlock_Create();
        eaPush(&pData->souvenirDisplayStates, state);
        i--;
    }


    if(pData->lastExpandedItemUID != -1)
    {
        expandSouvenirClue(pData, pData->lastExpandedItemUID);
        scrollToSouvenirClue(pData, pData->lastExpandedItemUID);
        pData->lastExpandedItemUID = -1;
    }

    y += (BORDER_SPACE + TAB_HEIGHT)*scale - pData->souvenirScollbar.offset;

    // Display all clues in order.
    iSize = eaSize(&pData->souvenirClues);
    for( i = 0; i < iSize; i++ )
    {
        displaySouvenirClue( pData, i, x + BORDER_SPACE, &y, z, wd - BORDER_SPACE*2, scale, souvenirDataIdx);
    }

    font( &game_12 );
    font_color( CLR_WHITE, CLR_WHITE );
     if( eaSize(&pData->souvenirClues) == 0 )
        cprnt( x + wd/2, y+15*scale, z, scale, scale, "YouAreSouvenirless" );

    set_scissor( 0 );

    doScrollBar( &pData->souvenirScollbar, ht - (PIX3*2+R10*2)*scale, y + pData->souvenirScollbar.offset - oldy - TAB_HEIGHT*scale, wd, (PIX3+R10)*scale, z, 0, 0 );
}


//
//
int clueWindow()
{
    float x, y, z, wd, ht, scale;
    int color, bcolor;
    static int init = 0;

    if( !window_getDims( WDW_CLUE, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor  ) )
        return 0;

    drawFrame( PIX3, R10, x, y, z-2, wd, ht, scale, color, bcolor );
    displayTabs();

    if(!init)
    {
        init = 1;
        loadPlayerCreatedSouvenirClues();
    }

    switch(contentType)
    {
        case CCT_CLUE:
            clueTab();
            break;
        case CCT_SOUVENIR:
            souvenirClueTab(0);
            break;
        case CCT_MM_SOUVENIR:
            souvenirClueTab(1);
            break;
    }
    return 0;
}
/* End of File */
