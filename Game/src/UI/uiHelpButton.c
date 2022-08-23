
#include <utilitieslib/stdtypes.h>
#include "uiHelpButton.h"

#include "UI/uiClipper.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiInput.h"
#include "UI/uiBox.h"
#include "UI/uiGame.h"

#include "win/win_init.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "language/langClientUtil.h"

#include "formatter/smf_main.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/Cbox.h>
#include "graphics/ttFontUtil.h"

#define IDEAL_WD 300
#define BORDER_WD 10

static TextAttribs s_taHelpText =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)CLR_MM_HELP_TEXT,
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

void freeHelpButton(HelpButton *pHelp)
{
    if( pHelp && pHelp->pBlock)
        smfBlock_Destroy(pHelp->pBlock);

    SAFE_FREE(pHelp);
}

void helpButtonFrame( F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc, int color, int back_col, int back_col2 )
{
    static AtlasTex *corner, *fcorner, *fcenter, *fside, *back;
    static int initTex;

    if(!initTex)
    {
        corner = atlasLoadTexture("MessageWindow_corner"); 
        fcorner = atlasLoadTexture("MessageWindow_frame_corner");  
        fcenter = atlasLoadTexture("MessageWindow_frame_mid_center");  
        fside = atlasLoadTexture("MessageWindow_frame_mid_side"); 
        back = atlasLoadTexture("white");
        initTex = 1;
    }

    // UL
      display_sprite(corner, x, y, z, sc, sc, back_col );
     display_sprite_blend(back, x + fcorner->width*sc, y, z, (wd-2*fcorner->width*sc)/(2*back->width), fcorner->height*sc/back->height, back_col, back_col2, back_col2, back_col );
    display_sprite_blend(back, x + wd/2, y, z, (wd-2*fcorner->width*sc)/(2*back->width), fcorner->height*sc/back->height, back_col2, back_col, back_col, back_col2 );
     display_spriteFlip(corner, x + wd - corner->width*sc, y, z, sc, sc, back_col, FLIP_HORIZONTAL );
    
     display_sprite(back, x, y + corner->height*sc, z, corner->width*sc/back->width, (ht - 2*fcorner->height*sc)/back->height, back_col );
       display_sprite_blend(back, x+corner->width*sc, y + corner->height*sc, z, (wd-2*corner->width*sc)/(2*back->width), (ht - 2*fcorner->height*sc)/back->height, back_col, back_col2, back_col2, back_col  );
    display_sprite_blend(back, x + wd/2, y + corner->height*sc, z, (wd-2*corner->width*sc)/(2*back->width), (ht - 2*fcorner->height*sc)/back->height, back_col2, back_col, back_col, back_col2 );
     display_sprite(back, x + wd - corner->width*sc, y + corner->height*sc, z, corner->width*sc/back->width, (ht - 2*fcorner->height*sc)/back->height, back_col );

      display_spriteFlip(corner, x, y + ht - corner->height*sc, z, sc, sc, back_col, FLIP_VERTICAL );
    display_sprite_blend(back, x + fcorner->width*sc, y + ht - fcorner->height*sc, z, (wd-2*fcorner->width*sc)/(2*back->width), fcorner->height*sc/back->height, back_col, back_col2, back_col2, back_col );
    display_sprite_blend(back, x + wd/2, y + ht - fcorner->height*sc, z, (wd-2*fcorner->width*sc)/(2*back->width), fcorner->height*sc/back->height, back_col2, back_col, back_col, back_col2 );
    display_spriteFlip(corner, x + wd - corner->width*sc, y + ht - fcorner->height*sc, z, sc, sc, back_col, FLIP_BOTH );
    
       x += 15*sc; 
    y += 15*sc;
    wd -= 30*sc;
    ht -= 30*sc;

    display_sprite(fcorner, x, y, z, sc, sc, color );
    display_sprite(fcenter, x + fcorner->width*sc, y, z, (wd-2*fcorner->width*sc)/fcenter->width, sc, color );
    display_spriteFlip(fcorner, x + wd - fcorner->width*sc, y, z, sc, sc, color, FLIP_HORIZONTAL );

    display_sprite(fside, x, y + fcorner->height*sc, z, sc, (ht - 2*fcorner->height*sc)/fside->height, color );
    display_spriteFlip(fside, x + wd - fside->width, y + fcorner->height*sc, z, sc, (ht - 2*fcorner->height*sc)/fside->height, color, FLIP_HORIZONTAL );

    display_spriteFlip(fcorner, x, y + ht - fcorner->height*sc, z, sc, sc, color, FLIP_VERTICAL );
    display_spriteFlip(fcenter, x + fcorner->width*sc, y + ht - fcenter->height*sc, z, (wd-2*fcorner->width*sc)/fcenter->width, sc, color, FLIP_VERTICAL );
    display_spriteFlip(fcorner, x + wd - fcorner->width*sc, y + ht - fcorner->height*sc, z, sc, sc, color, FLIP_BOTH );
}


float helpButtonEx( HelpButton **ppHelpButton, F32 x, F32 y, F32 z, F32 sc, char * txt, CBox * bounds, int flags )
{
    return helpButtonFullyConfigurable( ppHelpButton, x, y, z, sc, txt, bounds, flags,
                                    CLR_MM_HELP_TEXT, CLR_MM_HELP_DARK, CLR_MM_HELP_MID, CLR_MM_HELP_LIGHT, CLR_MM_HELP_DARK);
}

float helpButtonFullyConfigurable( HelpButton **ppHelpButton, F32 x, F32 y, F32 z, F32 sc, char * txt, CBox * bounds, int flags,
                                    int color, int back_col, int back_col2, int icon_col, int icon_col2)
{
    HelpButton * pHelp;
    static AtlasTex * tip, *tip_glow, *tip_out, *tip_glow_bright;
    static int init;
    CBox cbox;
     F32 questionwidth;
     //int color = 0xffffff88;
    int opened = 0;
    float returnHeight = 0;
     if(!txt)
        return returnHeight;

    if(!init)
    {
        tip = atlasLoadTexture("tooltip"); 
        tip_glow = atlasLoadTexture("tooltip_glow"); 
        tip_out = atlasLoadTexture("tooltip_outline"); 
        tip_glow_bright = atlasLoadTexture("tooltip_glow_bright"); 
        init = 1;
    }
    questionwidth = tip->width*sc;
    // first see if we need to create and add this button
     if( !*ppHelpButton )
    {
        pHelp = calloc(sizeof(HelpButton),1);
        pHelp->pBlock = smfBlock_Create();
        smf_SetRawText( pHelp->pBlock, textStd(txt), 0 );
        *ppHelpButton = pHelp;
    }
    else
        pHelp = *ppHelpButton;

    // now draw the question mark
     BuildCBox(&cbox, x - questionwidth/2, y - questionwidth/2, questionwidth, questionwidth );

    if( mouseClickHit(&cbox, MS_LEFT) )
    {
        pHelp->state = !pHelp->state;
        opened = 1;
    }

       if( pHelp->state )
    {
         // now the hard part, try to fit the box within the clip bounds
        // and maximize the displayable area
         F32 tx = x, ty = y;
         F32 txt_x, txt_y = 0, txt_wd, txt_ht;
        F32 wd, ht, target_wd = IDEAL_WD*sc;
        int bFlowUp = 0;
        int w,h;
        windowClientSize(&w, &h);

         z += 30;

        if( !bounds )
        {
            BuildCBox(&cbox, 0, 0, w, h);
        }
        else
            memcpy(&cbox, bounds, sizeof(CBox));

        if( !point_cbox_clsn(x,y,&cbox) ) // we must've scrolled of the screen, or closed the window
            pHelp->state = 0;

        if ( flags & HELP_BUTTON_USE_BOX_WIDTH )
            target_wd = bounds->hx - bounds->lx;

        if( cbox.hx - x < target_wd ) 
        {
            if( x - cbox.lx > cbox.hx - x )
            {
                tx = x + BORDER_WD*sc + questionwidth/2 - target_wd;
                txt_x = tx + 2*BORDER_WD*sc;
                wd = target_wd;
            }
            else
            {
                wd = cbox.hx - x + BORDER_WD*sc + questionwidth/2;
                tx = x - BORDER_WD*sc - questionwidth/2;
                txt_x = tx + questionwidth + 2*BORDER_WD*sc;
            }
        }
        else
        {
            tx = x - BORDER_WD*sc - questionwidth/2;
            wd = target_wd + BORDER_WD*sc;
            txt_x = tx + questionwidth + BORDER_WD*sc;        
        }

        txt_wd = wd - 3*BORDER_WD*sc - questionwidth;

        // now get text height dimensions
        s_taHelpText.piScale = (int *)((int)(1.0f*sc*SMF_FONT_SCALE));
        txt_ht = smf_ParseAndFormat( pHelp->pBlock, textStd(txt), txt_x, y, z, txt_wd, 0, 0, 0, 0, &s_taHelpText, 0);
        ht = txt_ht + 3*BORDER_WD*sc + questionwidth;

        if( y - cbox.ly > cbox.hy - y || y + txt_ht > h)  // for now naivlely flip the hit to whichever side has the most space
        {                                 // there is more trickiness we can do to try and maximize space if this doesn't work
            txt_y = y - txt_ht - questionwidth/2;
            ty = y + questionwidth/2 + BORDER_WD*sc - ht;
        }
        else
        {
            ty = y - BORDER_WD*sc - questionwidth/2;
             txt_y = y + questionwidth/2;
        }

         clipperPush(NULL);
           helpButtonFrame(tx, ty, z, wd, ht, sc, color, back_col, back_col2);
        smf_ParseAndDisplay( pHelp->pBlock, textStd(txt), txt_x, txt_y, z, txt_wd, 0, 0, 0, &s_taHelpText, NULL, 0, 0);
        returnHeight = txt_y+ txt_ht;
        clipperPop();

        if( !opened && (mouseClickHitNoCol(MS_LEFT) || mouseClickHitNoCol(MS_RIGHT)) )
            pHelp->state = 0;

        BuildCBox(&cbox, tx, ty, wd, ht);
        if( mouseCollision(&cbox) )
            collisions_off_for_rest_of_frame = 1;
    }

       if( mouseCollision(&cbox) || pHelp->state)
         display_sprite( tip_glow_bright, x - tip_glow_bright->width*sc/2, y - tip_glow_bright->height*sc/2, z+2, sc, sc, 0xffffff44 );
    if( mouseCollision(&cbox) )
        collisions_off_for_rest_of_frame = 1;
      display_sprite( tip_glow, x - tip_glow->width*sc/2, y - tip_glow->height*sc/2, z+2, sc, sc, CLR_WHITE );
    display_sprite( tip, x - questionwidth/2, y - questionwidth/2, z+2, sc, sc, icon_col );
    display_sprite( tip_out, x - tip_out->width*sc/2, y - tip_out->height*sc/2, z+2, sc, sc, icon_col2 );
    return returnHeight;
}