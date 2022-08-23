

#include <utilitieslib/stdtypes.h>
#include "UI/Hybrid/uiRedirect.h"
#include "UI/uiGame.h"                // for start_menu

#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"

#include "win/input.h"
#include <utilitieslib/components/earray.h>
#include "win/win_init.h" // for windowClientSize
#include "player/player.h"      // for playerPtr
#include "graphics/font.h"
#include "graphics/ttFontUtil.h"
#include "sound/sound.h"
#include "language/langClientUtil.h"
#include "graphics/textureatlas.h"
#include "cmdparse/cmdgame.h"  // for timestep
#include <utilitieslib/components/EString.h>
#include <utilitieslib/language/MessageStoreUtil.h>

#include "formatter/smf_parse.h"
#include "formatter/smf_format.h"
#include "formatter/smf_main.h"

#include "UI/uiInput.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/Hybrid/uiHybridMenu.h"

int g_redirectedFrom;
F32 s_blinkTimer;

int redirectedFromMenu(int menu)
{
    return isMenu(MENU_REDIRECT) && menu == g_redirectedFrom;
}

typedef struct HybridRedirect
{
    int menu_dest;
    char * text;
    RedirectCallback fixedCallback;
    int blinkRegion;

}HybridRedirect;

HybridRedirect **ppRedirects;

void freeRedirect(HybridRedirect *hr)
{
    SAFE_FREE(hr->text);
    SAFE_FREE(hr);
}

void clearRedirects()
{
    while(eaSize(&ppRedirects))
    {
        HybridRedirect * hr = eaRemove(&ppRedirects, 0);
        freeRedirect( hr );
    }
}

void addRedirect(int menu, char * text, int blink_region, RedirectCallback fixedCallback )    
{
    int i;
    HybridRedirect * hr;

    for( i = 0; i < eaSize(&ppRedirects); i++ )
    {
        if( blink_region == ppRedirects[i]->blinkRegion )
            return;
    }

    hr = malloc(sizeof(HybridRedirect));
    hr->menu_dest = menu;
    hr->text = strdup(text);
    hr->blinkRegion = blink_region;
    hr->fixedCallback = fixedCallback;
    eaPush(&ppRedirects, hr);
}




void updateRedirect()
{
    int i;
     s_blinkTimer += TIMESTEP*.3;

    for( i = eaSize(&ppRedirects)-1; i >= 0; i-- )
    {
        HybridRedirect *hr = ppRedirects[i];

        if( isMenu(hr->menu_dest) )
        {
            if( hr->fixedCallback(0) )  // they fixed mistake
            {
                hr = eaRemove(&ppRedirects, i);
                freeRedirect( hr );
            }
        }
        else if( !isMenu(MENU_REDIRECT) ) // or they went to different menu
        {
            hr = eaRemove(&ppRedirects, i);
            freeRedirect( hr );
        }
    }
}

F32 getRedirectBlinkScale(int blinkRegion)
{
    int i;
    for( i = eaSize(&ppRedirects)-1; i >= 0; i-- )
    {
        if( ppRedirects[i]->blinkRegion == blinkRegion )
            return ((cosf(s_blinkTimer)+1)/2);
    }
    return 1.f;
}

void redirectMenuTrigger(int region)
{
    int i;
    for( i = 0; i < eaSize(&ppRedirects); i++ )
    {
        if( region == ppRedirects[i]->blinkRegion )
            hybrid_start_menu(ppRedirects[i]->menu_dest);
    }
}

void redirectMenu()
{
    static SMFBlock *smDesc;
    AtlasTex *floor = atlasLoadTexture("floorpattern_screenredirect");
    char *estr;
    int i, buttonClick = 0;
    F32 y = 300;
    int incomplete = 0;
    static HybridElement sButtonRedirOrigin = {0, NULL, NULL, "icon_redirect_orgin"};
    static HybridElement sButtonRedirArchetype = {0, NULL, NULL, "icon_redirect_archetype"};
    static HybridElement sButtonRedirPower = {0, NULL, NULL, "icon_redirect_power"};
    F32 xposSc, yposSc, xp, yp;

    drawHybridCommon(HYBRID_MENU_CREATION);
    
    // draw floor icon
    display_sprite_positional(floor, DEFAULT_SCRN_WD / 2.f, DEFAULT_SCRN_HT, 3, 1.f, 1.f, 0xffffff14, H_ALIGN_CENTER, V_ALIGN_BOTTOM );

    setScalingModeAndOption(SPRITE_Y_UNIFORM_SCALE, SSO_SCALE_TEXTURE);
    calculatePositionScales(&xposSc, &yposSc);
    xp = DEFAULT_SCRN_WD / 2.f;
    yp = 0.f;
    applyPointToScreenScalingFactorf(&xp, &yp);
     if(!eaSize(&ppRedirects))
    {
        start_menu(MENU_LOGIN);
        return;
    }
    // draw text 
    if( !smDesc ) // init block
        smDesc = smfBlock_Create();

      estrCreate(&estr);
       for(i = 0; i < eaSize(&ppRedirects); i++ )
    {
         y -= 30;
          estrConcatf(&estr, "<span align=center>%s</span>", textStd(ppRedirects[i]->text) );
        incomplete |= (1<<ppRedirects[i]->menu_dest);
    }
    hybridMenuFlash(incomplete);
       drawHybridDesc( smDesc, xp - 450.f*xposSc, y*yposSc, 5, 2.f, 900, 300, estr, 0, 0xff  );

    switch( ppRedirects[0]->menu_dest )
    {
           case MENU_ORIGIN: buttonClick = D_MOUSEHIT == drawHybridButton(&sButtonRedirOrigin, xp, 450.f*yposSc, 5, 1.f, CLR_WHITE, 0); break;
        case MENU_ARCHETYPE: buttonClick = D_MOUSEHIT == drawHybridButton(&sButtonRedirArchetype, xp, 450.f*yposSc, 5, 1.f, CLR_WHITE, 0); break;
        case MENU_POWER: buttonClick = D_MOUSEHIT == drawHybridButton(&sButtonRedirPower, xp, 450.f*yposSc, 5, 1.f, CLR_WHITE, 0); break;
    }

    if( buttonClick )
        hybrid_start_menu(ppRedirects[0]->menu_dest);

    setScalingModeAndOption(SPRITE_XY_SCALE, SSO_SCALE_ALL);
}
