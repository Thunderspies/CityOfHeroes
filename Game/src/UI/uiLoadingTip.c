
#include "language/langClientUtil.h"
#include "UI/sprite/sprite_font.h"    // for font definitions
#include "UI/sprite/sprite_text.h"    // for font functions
#include "UI/sprite/sprite_base.h"    // for sprites
#include "graphics/textureatlas.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "graphics/ttFontUtil.h"

#include "formatter/smf_main.h"
#include "uiSMFView.h"

#include <utilitieslib/version/AppRegCache.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/error.h>
#include "win/win_init.h"

#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"

// Data structures
//---------------------------------------------------------------------
static TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.1f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

static int s_LoadingTipIdx;
static char s_LoadingTip[2000];


void loadLoadingTips()
{
    s_LoadingTipIdx = regGetAppInt("LoadintTip", 0);
}

void nextLoadingTip()
{
    char * tipKey;
    int missedTipCount = 0;
    estrCreate(&tipKey);

    do 
    {
        s_LoadingTipIdx++;
        missedTipCount++;
        estrClear(&tipKey);
        estrConcatf(&tipKey, "Tip_%i", s_LoadingTipIdx );
        msPrintf(loadingTipMessages, SAFESTR(s_LoadingTip), tipKey);

        // try a gap of 10 numbers, then start list over
        if(s_LoadingTipIdx > 10 && missedTipCount == 10 )
            s_LoadingTipIdx = missedTipCount = 0;
    }
    while( missedTipCount < 10 && stricmp(s_LoadingTip, tipKey)==0 );
    regPutAppInt("LoadintTip", s_LoadingTipIdx);

    if(missedTipCount == 10)
        Errorf("Loading Tips had too many gaps, check the file and make sure the tips are numerically ordered");
}

void displayLoadingTip( F32 z )
{
    static SMFBlock *s_LoadingBlock = 0;
    static int last_tip = -1;
     int w, h, text_ht = 20, text_wd,  reformat = last_tip != s_LoadingTipIdx;
    F32 x = DEFAULT_SCRN_WD/2, y = 580;
    F32 width = 600, sc;

    if (!s_LoadingBlock)
    {
        s_LoadingBlock = smfBlock_Create();
    }

    windowClientSize(&w, &h); 
    sc = SMF_FONT_SCALE;
    gTextAttr.piScale = (int *)((int)sc);
    
     font(&game_12);
    font_color(CLR_WHITE, CLR_WHITE);

    // str_wd dies loading textures so I'll I'm just estimating
    text_wd = strlen(s_LoadingTip)*10*sc;
    if( text_wd < 600 )
    {
        cprntEx( x, y, z, sc, sc, CENTER_X|CENTER_Y, s_LoadingTip );
        text_wd = str_wd(&game_12, sc, sc, s_LoadingTip );
        drawFrame( PIX3, R10, (x-text_wd/2-R10), y, z, (text_wd+2*R10), text_ht+(R10*2), 1.0f, CLR_WHITE, CLR_BLACK );
    }
    else
    {
        text_ht = smf_ParseAndDisplay( s_LoadingBlock, s_LoadingTip, (x-(width-2*R10)/2), (y+R10), z+10, (width-2*R10), 0, 1, 1, 
                                        &gTextAttr, NULL, 0, true);
        drawFrame( PIX3, R10, (x-width/2), y, z, width, text_ht+(R10*2), 1.0f, CLR_WHITE, CLR_BLACK );
    }

     last_tip = s_LoadingTipIdx;
}
