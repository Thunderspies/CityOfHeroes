
#include <utilitieslib/utils/mathutil.h>
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "entity/entVarUpdate.h"   // for TIMESTEP
#include "language/langClientUtil.h"
#include "UI/uiInput.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilMenu.h"
#include "sound/sound.h"
#include <utilitieslib/utils/timing.h>
#include "win/input.h"
#include "UI/uiGame.h"
#include "cmdparse/cmdcommon.h"
#include "graphics/textureatlas.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/utils.h>
#include "UI/uiUtilGame.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiWindows.h"
#include "auth/authUserData.h"
#include "UI/uiLogin.h"
#include "win/win_init.h"
#include "UI/uiDialog.h"
#include <utilitieslib/version/AppRegCache.h>
#include "entity/entity.h"
#include "player/player.h"
#include "clientcomm/dbclient.h"
#include "UI/uiQuit.h"
#include "UI/Hybrid/uiWebStoreFrame.h"

#define BACKGROUND_GLOW_SPEED        .025f

//
//
void drawBackground( AtlasTex * inputTexture )
{
    static float interpTimer = 1;
    static AtlasTex *prevTexture = NULL;
    static AtlasTex *targetTexture = NULL;

    if (!targetTexture)
        targetTexture = atlasLoadTexture("background_skyline_normal.tga");

    if (!inputTexture)
        inputTexture = targetTexture;


    if (inputTexture != targetTexture)
    {
        if (interpTimer >= 1.0f)
        {
            targetTexture = inputTexture;
        }
    }
    if (prevTexture != targetTexture)
    {
        if (interpTimer >= 1.0f)
        {
            interpTimer = 0.0f;
        }
    }
    interpTimer += 0.025 * TIMESTEP;
    interpTimer = CLAMP(interpTimer, 0, 1);
    if (interpTimer < 1.f)
    {
        int fadeOutAlpha = (0x000000ff * ((cos(PI*interpTimer)+1)/2.f));
        int fadeOutColor = 0xffffff00 | fadeOutAlpha;
        display_sprite(prevTexture, 0, 0, 3, 1.f, 1.f, fadeOutColor );
        display_sprite(targetTexture, 0, 0, 2, 1.f, 1.f, CLR_WHITE );
    }
    else
    {
        prevTexture = targetTexture;
        display_sprite(targetTexture, 0, 0, 3, 1.f, 1.f, CLR_WHITE);
    }
}


void drawMenuHeader( float x, float y, float z, int clr1, int clr2, const char *txt, float sc )
{
    float wd;
    AtlasTex *dot = atlasLoadTexture( "EnhncTray_powertitle_dot.tga" );

      font_color( clr1, clr2 );
    cprntEx( x, y, z, sc, sc, (CENTER_X | CENTER_Y), txt );

    wd = str_wd( font_grp, sc, sc, txt );
       display_sprite( dot, x - (wd/2 + ((dot->width + 5)*sc)),    y - dot->height*sc/2, z, sc, sc, CLR_WHITE );
    display_sprite( dot, x + (wd/2 + (5*sc)),                    y - dot->height*sc/2, z, sc, sc, CLR_WHITE );
}


//
//
void drawMenuFrame( int radius, float x, float y, float z, float wd, float ht, int color, int back_color, int noBottom )
{
    float w;
    int size = 4;
    int offset = (radius == R25);
    AtlasTex *ul, *ll, *lr, *ur;
    float shrink = 1.0;

    AtlasTex *ypipe;
    AtlasTex *xpipe;
    AtlasTex *bul;
    AtlasTex *bll;
    AtlasTex *blr;
    AtlasTex *bur;
    AtlasTex * back = white_tex_atlas;
    float screenScaleX, screenScaleY;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);

    bul        = atlasLoadTexture( "frame_blue_background_UL.tga" );
    bll        = atlasLoadTexture( "frame_blue_background_LL.tga" );
    blr        = atlasLoadTexture( "frame_blue_background_LR.tga" );
    bur        = atlasLoadTexture( "frame_blue_background_UR.tga" );

    if (game_state.skin == UISKIN_HEROES)
    {
        ypipe    = atlasLoadTexture( "frame_blue_4px_vert.tga" );
        xpipe    = atlasLoadTexture( "frame_blue_4px_horiz.tga" );
    }
    else
    {
        ypipe    = atlasLoadTexture( "frame_blue_4px_vert_tintable.tga" );
        xpipe    = atlasLoadTexture( "frame_blue_4px_horiz_tintable.tga" );
    }

    if( radius == R12 )
    {
        if (game_state.skin == UISKIN_HEROES)
        {
            ul = atlasLoadTexture( "frame_blue_4px_12r_UL.tga" );
            ll = atlasLoadTexture( "frame_blue_4px_12r_LL.tga" ); 
            lr = atlasLoadTexture( "frame_blue_4px_12r_LR.tga" );
            ur = atlasLoadTexture( "frame_blue_4px_12r_UR.tga" );
        }
        else
        {
            ul = atlasLoadTexture( "frame_blue_4px_12r_UL_tintable.tga" );
            ll = atlasLoadTexture( "frame_blue_4px_12r_LL_tintable.tga" ); 
            lr = atlasLoadTexture( "frame_blue_4px_12r_LR_tintable.tga" );
            ur = atlasLoadTexture( "frame_blue_4px_12r_UR_tintable.tga" );
        }
    }
    else
    {
        if (game_state.skin == UISKIN_HEROES)
        {
            ul = atlasLoadTexture( "frame_blue_4px_25r_UL.tga" );
            ll = atlasLoadTexture( "frame_blue_4px_25r_LL.tga" ); 
            lr = atlasLoadTexture( "frame_blue_4px_25r_LR.tga" );
            ur = atlasLoadTexture( "frame_blue_4px_25r_UR.tga" );
        }
        else
        {
            ul = atlasLoadTexture( "frame_blue_4px_25r_UL_tintable.tga" );
            ll = atlasLoadTexture( "frame_blue_4px_25r_LL_tintable.tga" ); 
            lr = atlasLoadTexture( "frame_blue_4px_25r_LR_tintable.tga" );
            ur = atlasLoadTexture( "frame_blue_4px_25r_UR_tintable.tga" );
        }
    }

    x *= screenScaleX;
    y *= screenScaleY;
    wd *= screenScaleX;
    ht *= screenScaleY;

    w = ul->width - offset;

    shrink = MIN(shrink, wd/(w*2));
    shrink = MIN(shrink, ht/(w*2));
    w *= shrink;
    size *= shrink;

    // top left
    display_sprite_menu( ul, x, y, z+2, shrink, shrink, color );
    display_sprite_menu( bul, x , y , z, shrink, shrink, back_color );

    // top
    display_sprite_menu( xpipe, x + w, y, z+1, (wd - 2*w)/xpipe->width, shrink, color );
    display_sprite_menu( back , x + w, y + size, z, (wd - 2*w)/back->width, (w - size)/back->height, back_color );

    // top right
    display_sprite_menu( ur,  x + wd - w, y, z+2, shrink, shrink, color );
    display_sprite_menu( bur, x + wd - w, y , z, shrink, shrink, back_color );

    // right
    display_sprite_menu( ypipe, x + wd - size + offset, y + w, z+1, shrink, (ht - 2*w)/ypipe->height, color );
    display_sprite_menu( back,  x + wd - w, y + w, z, (w-size)/back->width, (ht - 2*w)/back->height, back_color );

    // left
    display_sprite_menu( ypipe, x, y + w, z+1, shrink, (ht - 2*w)/ypipe->height, color );
    display_sprite_menu( back, x + size, y + w, z, (w - size)/back->width, (ht - 2*w)/back->height, back_color );

    // middle
    display_sprite_menu(back, x + w, y + w, z, (wd - 2*w)/back->width, (ht - 2*w)/back->height, back_color );

    if ( !noBottom )
    {
        // lower right
        display_sprite_menu( lr,  x + wd - w, y + ht - w, z+2, shrink, shrink, color );
        display_sprite_menu( blr, x + wd - w, y + ht - w, z, shrink, shrink, back_color );

        // bottom
        display_sprite_menu( xpipe, x + w, y + ht - size + offset, z+1, (wd - 2*w)/xpipe->width, 1.f, color );
        display_sprite_menu( back, x + w, y + ht - w, z, (wd - 2*w)/back->width, (w - size)/back->height, back_color );

        // lower left
        display_sprite_menu( ll,  x, y + ht - w, z+2, shrink, shrink, color );
        display_sprite_menu( bll, x , y + ht - w, z, shrink, shrink, back_color );
    }
}

//
//
void drawMenuCapsule( float x, float y, float z, float wd, float scale, int color, int back_color )
{
    int size = 4;

    AtlasTex *left        = atlasLoadTexture( "bodytype_4px_25r_halfring_L.tga" );
    AtlasTex *right        = atlasLoadTexture( "bodytype_4px_25r_halfring_R.tga" );
    AtlasTex *bleft        = atlasLoadTexture( "bodytype_4px_25r_halfringback_L.tga" );
    AtlasTex *bright    = atlasLoadTexture( "bodytype_4px_25r_halfringback_R.tga" );
    AtlasTex *xpipe        = atlasLoadTexture( "frame_blue_4px_horiz.tga" );
    AtlasTex *back        = white_tex_atlas;

    float w = left->width;

    if( wd < w*2 )
        wd = 2*w;

    // left
    display_sprite(  left, x, y, z+1, scale, scale, color );
    display_sprite( bleft, x, y, z,  scale, scale, back_color );

    // top pipe
    display_sprite( xpipe, x + w*scale, y, z+1, scale*(wd - 2*w)/xpipe->width, scale, color );

    // botttom pipe
    display_sprite( xpipe, x + w*scale, y + (left->height - size)*scale, z+1, scale*(wd - 2*w)/xpipe->width, scale, color );

    // right
    display_sprite(  right, x + (wd-w)*scale, y, z+1, scale, scale, color );
    display_sprite( bright, x + (wd-w)*scale, y, z,   scale, scale, back_color );

    //background
    display_sprite( back, x + w*scale, y + size*scale, z, scale*(wd-2*w)/back->width, scale*(left->height- 2*size)/back->height, back_color );
}


enum
{
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
};

int drawNextCircle( float x, float y, float z, float scale, int dir, int locked )
{
    AtlasTex * arrow;
    char d, l[32], tmp[128];
    CBox box;

    if ( dir == DIRECTION_LEFT )
        d = 'L';
    else
        d = 'R';

    if ( locked )
        strcpy( l, "locked_" );
    else
        strcpy( l, "" );

    sprintf( tmp, "next_%sbutton_%c.tga", l, d );
    arrow    = atlasLoadTexture( tmp );

    BuildCBox( &box, x - arrow->width*scale/2, y - arrow->height*scale/2, arrow->width*scale, arrow->height*scale );
     if( mouseCollision( &box ) && !locked && !isDown(MS_LEFT) )
        scale *= 1.1f;

    display_sprite( arrow, x - arrow->width*scale/2, y - scale*arrow->height/2, z+2, scale, scale, CLR_WHITE );
    
    if( !locked && mouseClickHit( &box, MS_LEFT ) )
        return TRUE;
    else
        return FALSE;
}

// designed for only two at once, three will break mouseover animation
//
int drawNextButton( float x, float y, float z, float scale, int dir, int locked, int glow, const char * txt )
{
    char d, l[32], tmp[128];
    CBox box;

    AtlasTex * arrow;
    AtlasTex * after_image;
    AtlasTex * tail;
    AtlasTex * arrow_glow;

    AtlasTex * glowL = atlasLoadTexture( "next_glowkit_L.tga" );
    AtlasTex * glowM = atlasLoadTexture( "next_glowkit_MID.tga" );
    AtlasTex * glowR = atlasLoadTexture( "next_glowkit_R.tga" );

    static int Llit = FALSE, Rlit = FALSE;
    static float timer = -PI/2;

    // first load the sprites in
    //--------------------------------------------------------------------
    if ( dir == DIRECTION_LEFT )
        d = 'L';
    else
        d = 'R';

    if ( locked )
        strcpy( l, "locked_" );
    else
        strcpy( l, "" );

    sprintf( tmp, "next_%sbutton_%c.tga", l, d );
    arrow    = atlasLoadTexture( tmp );

    sprintf( tmp, "next_%sfade_afterimage_%c.tga", l, d );
    after_image = atlasLoadTexture( tmp );

    sprintf( tmp, "next_%sfade_tail_%c.tga", l, d );
    tail = atlasLoadTexture( tmp );

    sprintf( tmp, "next_button_%c_glow.tga", d );
    arrow_glow = atlasLoadTexture( tmp );

    font( &game_12 );

     if ( dir == DIRECTION_LEFT )
    {
        float sc = 1.f;

        if(!shell_menu() )
             BuildCBox( &box, x - arrow->width*scale/2, y- arrow->height * scale/4, (arrow->width/2 + tail->width) * scale, arrow->height * scale/2 );
        else
             BuildCBox( &box, x - arrow->width*scale/2, y- arrow->height * scale/2, (arrow->width/2 + tail->width) * scale, arrow->height * scale);

        if( mouseCollision( &box ) && !locked )
        {
            if ( !Llit )
                timer = -PI/2;
            Llit = TRUE;
            timer += TIMESTEP*.1f;
            sc = (1.1f + .1*sinf( timer )) * scale;
            display_sprite( arrow_glow, x - sc*arrow_glow->width/2, y - sc*arrow_glow->height/2, z+2, sc, sc, CLR_WHITE );
             font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x8888ffff, 0xff4444ff, CLR_WHITE ) );
        }
        else
        {
            Llit = FALSE;
            display_sprite( arrow, x - scale * arrow->width/2, y - scale*arrow->height/2, z+2, scale, scale, CLR_WHITE );

            if( locked )
                font_color( CLR_WHITE, 0xaaaaaaff );
            else
            {
                if( game_state.skin == UISKIN_PRAETORIANS )
                    font_color( CLR_TXT_LROGUE, CLR_TXT_LROGUE );
                else
                    font_color( CLR_YELLOW, CLR_ORANGE );
            }
        }

        display_sprite( tail, x, y - scale*tail->height/2, z, scale, scale, CLR_WHITE );
        display_sprite( after_image, x, y - scale*after_image->height/2, z+1, scale, scale, CLR_WHITE );
        prnt( x + (arrow->width/2 + 5) * scale, y + 7 * scale, z+3, 1.5 * scale, 1.5 * scale, txt );
    }
    else
    {
        float sc = 1.f;
        int wd;

        if(!shell_menu() )
            BuildCBox( &box, x - tail->width * scale, y- arrow->height * scale/4, (arrow->width/2 + tail->width) * scale, arrow->height * scale/2 );
        else
            BuildCBox( &box, x - tail->width * scale, y- arrow->height * scale/2, (arrow->width/2 + tail->width) * scale, arrow->height * scale);
        if( mouseCollision( &box ) && !locked )
        {
            if ( !Rlit )
                timer = -PI/2;
            Rlit = TRUE;
            timer += TIMESTEP*.1f;
            sc = (1.1f + .1*sinf( timer )) * scale;
            display_sprite( arrow_glow, x - sc*arrow_glow->width/2, y - sc*arrow_glow->height/2, z+2, sc, sc, CLR_WHITE );
              font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0x8888ffff, 0xff4444ff, CLR_WHITE ) );
        }
        else
        {
            Rlit = FALSE;
            display_sprite( arrow, x - scale * arrow->width/2, y - scale*arrow->height/2, z+2, scale, scale, CLR_WHITE );
            if( locked )
                font_color( CLR_WHITE, 0xaaaaaaff );
            else
                if( game_state.skin == UISKIN_PRAETORIANS )
                    font_color( CLR_TXT_LROGUE, CLR_TXT_LROGUE );
                else
                    font_color( CLR_YELLOW, CLR_ORANGE );
        }

         display_sprite( tail, x - scale * tail->width, y - scale*tail->height/2, z, scale, scale, CLR_WHITE );
         display_sprite( after_image, x - scale * after_image->width, y - scale*after_image->height/2, z+1, scale, scale, CLR_WHITE );

         wd = str_wd( &game_12, 1.5f * scale, 1.5f * scale, txt );
        prnt( x - arrow->width * scale/2 - wd, y + 7 * scale, z+3, 1.5 * scale, 1.5 * scale, txt );

         if( glow && !locked )
        {
            static float timer = 0;
            int alpha;
             int cl = SELECT_FROM_UISKIN(0x7aff7400, 0xffff7a00, 0xffff7a00);
 
             timer += TIMESTEP*.1;
             alpha = 170 + 30*sinf(timer);
            cl |= alpha;

               display_sprite( glowL,  x - (wd+((20+arrow->width/2)*scale)), y - scale*glowL->height/2, z+1, scale, scale, cl );
               display_sprite( glowM,  x + scale*glowL->width - (wd+((20+arrow->width/2)*scale)), y - scale*glowL->height/2, z+1, (float)(wd-(20*scale))/glowM->width, scale, cl );
               display_sprite( glowR,  x + scale*glowL->width - (((arrow->width/2) + 40)*scale), y - scale*glowL->height/2, z+1, scale, scale, cl  );
        }
    }

    if( !locked && mouseClickHit( &box, MS_LEFT ) )
        return TRUE;
    else
        return FALSE;

}

#define TITLE_SCALE 1.5f
#define TITLE_SPEED 30
//
//
void drawMenuTitle( float x, float y, float z, const char * txt, float scale, int newMenu )
{
    AtlasTex *dot        = atlasLoadTexture( "titlebar_dot.tga" );
    AtlasTex *dotGlow    = atlasLoadTexture( "titlebar_dot_glow.tga" );
    AtlasTex *pipe        = atlasLoadTexture( "titlebar_frame_horiz.tga" );
    AtlasTex *cap        = atlasLoadTexture( "titlebar_frame_cap_R.tga" );
    AtlasTex *back        = white_tex_atlas;
    AtlasTex *blr        = atlasLoadTexture( "frame_blue_background_LR.tga" );
    AtlasTex *bur        = atlasLoadTexture( "frame_blue_background_UR.tga" );

    static float acwd = 0;
    static float timer = 0;
    float strwd;
    float wd = 0;
    float pipex = 0.0f;
    float pipewidth = 0.0f;

    timer += TIMESTEP*.1f;

    if( newMenu )
        acwd = (dot->width/2 + cap->width)*scale;

    acwd += (TIMESTEP*TITLE_SPEED)*scale;

     strwd = str_wd( &title_18, scale*TITLE_SCALE, scale*TITLE_SCALE, txt );

    wd = strwd + scale*((dot->width/2)+cap->width);

    if( acwd > wd )
        acwd = wd;

    // the background

     display_sprite( blr, x + acwd - (scale*cap->width), y - scale*(cap->height/2 - PIX4), z, scale, scale*(cap->height - 2*PIX4)/blr->height, CLR_BLACK );
     display_sprite( bur, x + acwd - (scale*cap->width), y - scale*(cap->height/2 - PIX4), z, scale, scale*(cap->height - 2*PIX4)/bur->height, CLR_BLACK );

    // dot
    if( game_state.skin == UISKIN_PRAETORIANS )
    {
        display_sprite( back, 0.0f, y - scale*((cap->height/2) - PIX4), z, 
            (acwd-(scale*(cap->width-PIX3-x)))/back->width, 
             scale*(cap->height - 2*PIX4)/back->height, CLR_BLACK );
        pipex = 0.0f;
        pipewidth = (x+acwd-(scale*cap->width))/pipe->width;
    }
    else
    {
        display_sprite( back, x +scale+ dot->width/2*scale, y - scale*((cap->height/2) - PIX4), z, 
            (acwd-(scale*(((dot->width+cap->width)/2)-PIX3)))/back->width, 
             scale*(cap->height - 2*PIX4)/back->height, CLR_BLACK );

        display_sprite( dot, x, y - scale*dot->height/2, z+3, scale, scale, CLR_WHITE );
        {
            float sc = 1.25 + .25*( sinf(timer));
            display_sprite( dotGlow, x - (sc*dotGlow->width - dot->width)*scale/2, y - sc*scale*dotGlow->height/2, z+2, sc*scale, sc*scale, 0xffffff88 );
        }

        pipex = x + scale*dot->width/2;
        pipewidth = (acwd-(scale*((dot->width/2)+cap->width)))/pipe->width;
    }

    // top pipe
    display_sprite( pipe, pipex, y - (scale*cap->height/2), z+1, 
        pipewidth, scale, CLR_WHITE );

    // bottom pipe
    display_sprite( pipe, pipex, y + scale*(cap->height/2 - PIX4), z+1, 
        pipewidth, scale, CLR_WHITE );

    // cap
    display_sprite( cap, x + acwd - (scale*cap->width), y - scale*cap->height/2, z+2, scale, scale, CLR_WHITE );

    font( &title_18 );
    if( game_state.skin == UISKIN_PRAETORIANS )
        font_color( CLR_TXT_LROGUE, CLR_TXT_LROGUE );
    else
        font_color( CLR_YELLOW, CLR_ORANGE );
    if( acwd < wd )
    {
        set_scissor( TRUE );
        scissor_dims( x+ (dot->width*scale/2) - (10*scale), 0, (acwd - ((cap->width+50)*scale)), 500*scale );
    }

    prnt( x + (dot->width*scale/2) - (10*scale), y + scale*9*TITLE_SCALE, z+3, scale*TITLE_SCALE, scale*TITLE_SCALE, txt );
    if (acwd < wd)
        set_scissor( FALSE );
}

//
//
int drawCheckBox( float x, float y, float z, float scale, int color, int isDown, int selectable, int gray, int mouseover, int isPower, AtlasTex *icon )
{
     int clrBase, clrFill, clrHigh, retValue = D_NONE;
    CBox box;
    AtlasTex* cbase;
    AtlasTex* cfill = NULL;
    AtlasTex* cbhigh;
    AtlasTex* cblow;
    AtlasTex* cring;
    AtlasTex* crds;
    AtlasTex* crhigh;
    AtlasTex* crlow;
    AtlasTex* cmark_outer;
    AtlasTex* cbunder = NULL;

    cbase    = atlasLoadTexture( "checkbox_base.tga" );
    cbhigh    = atlasLoadTexture( "checkbox_base_highlight.tga" );
    cblow    = atlasLoadTexture( "checkbox_base_shadow.tga" );
    cring    = atlasLoadTexture( "checkbox_ring.tga" );
    crds    = atlasLoadTexture( "checkbox_ring_dropshadow.tga" );
    crhigh    = atlasLoadTexture( "checkbox_ring_highlight.tga" );
    crlow    = atlasLoadTexture( "checkbox_ring_shadow.tga" );

    if ( isDown )
    {
            cmark_outer    = atlasLoadTexture( "checkedbox_checkmark.tga" );

        if( !gray )
        {
            clrBase = uiColors.selected.base;
            clrFill = uiColors.selected.fill;
            clrHigh = SELECT_FROM_UISKIN(CLR_HIGH_SELECT, CLR_HIGH_SELECT, 0xffffffbf);
        }
        else
        {
            clrBase = uiColors.unselectable.base;
            clrFill = uiColors.unselectable.fill;
            clrHigh = CLR_HIGH;
        }
    }
    else
    {
        cfill    = atlasLoadTexture( "checkbox_base_fill.tga" );
        cmark_outer    = atlasLoadTexture( "EnhncTray_RingHole.tga" );

        if( !gray )
        {
            clrBase = uiColors.standard.base;
            clrFill = uiColors.standard.fill;
            clrHigh = uiColors.standard.highlight;
        }
        else
        {
            clrBase = uiColors.unselectable.base;
            clrFill = uiColors.unselectable.fill;
            clrHigh = CLR_HIGH;
        }
    }

    if( game_state.skin == UISKIN_PRAETORIANS )
    {
        cbunder = atlasLoadTexture( "checkbox_base_underlight" );
    }

     BuildCBox( &box, x - cbase->width*scale*1.1 /2, y - cbase->height*scale*1.1 /2,
        cbase->width*scale*1.1, cbase->height*scale*1.1);
    if( mouseCollision( &box ) )
    {
        if ( mouseDown( MS_LEFT) )
            retValue = D_MOUSEDOWN;
        else
            retValue = D_MOUSEOVER;
    }

    if( (retValue || mouseover) && selectable )
    {
        brightenColor( &clrBase, 60 );
        scale += .1f;
    }

     if( icon )
        display_sprite( icon, x - icon->width*scale/2, y - icon->height*scale/2, z+10, scale, scale, CLR_WHITE );

    if( isPower )
    {
           float sc = scale*.5;
           AtlasTex* border = atlasLoadTexture( "tray_ring_power.tga" );
         AtlasTex* cmark_local;
        int clr = CLR_WHITE;

        if( !gray )
        {
            cmark_local  = atlasLoadTexture( "EnhncTray_RingHole_highlight.tga" );

            if( game_state.skin == UISKIN_PRAETORIANS )
            {
                color = 0xffe700ff; // uiColors.selected.frame;
            }
        }
        else
        {
             clr = 0xccccccff;
            // Praetorian skin has a non-underlit version of the 'hole' for
            // disabled checkbars/checkboxes.
            if( game_state.skin != UISKIN_PRAETORIANS )
            {
                   cmark_local  = atlasLoadTexture( "EnhncTray_RingHole.tga" );
            }
            else
            {
                cmark_local    = atlasLoadTexture( "P_EnhncTray_RingHole_Disable.tga" );
            }
        }

           display_sprite( cmark_local, x - cmark_local->width*sc/2, y - cmark_local->height*sc/2, z+1, sc, sc, clr );
        display_sprite( border, x - border->width*scale/2, y - border->height*scale/2, z+9, scale, scale, color );
    }

    x -= cbase->width/2*scale;
    y -= cbase->height/2*scale;

     if( !isPower && !icon )
    {
        display_sprite( cbase,  x - .1*cbase->width/2, y - .1*cbase->height/2, z,   scale*1.1, scale*1.1, clrBase );
        display_sprite( cblow,    x, y, z+1, scale, scale, CLR_CBLOW );
        display_sprite( cbhigh, x, y, z+2, scale, scale, clrHigh );

        display_sprite( crds,   x, y, z+5, scale, scale, CLR_CRDS );
        display_sprite( cring,  x, y, z+6, scale, scale, clrBase );
        display_sprite( crlow,  x, y, z+7, scale, scale, CLR_SHADOW );
        display_sprite( crhigh, x, y, z+8, scale, scale, clrHigh );

        if( !gray && game_state.skin == UISKIN_PRAETORIANS )
            display_sprite( cbunder, x, y, z+9, scale, scale, CLR_WHITE );

        if (isDown)
            display_sprite( cmark_outer, x, y,  z+10, scale, scale, CLR_WHITE );
        else
        {
             display_sprite( cmark_outer, x + (cbase->width - cmark_outer->width*.25)*scale/2, y + (cbase->height - cmark_outer->height*.25)*scale/2,  z+3, .25*scale, .25*scale, clrBase );
             display_sprite( cfill, x, y, z+4, scale, scale, clrFill );
        }
    }

    if( mouseClickHit( &box, MS_LEFT ) && selectable )
        return D_MOUSEHIT;
    else
        return retValue;
}

enum
{
    SM_CHECKBAR,
    LG_CHECKBAR,
};
//

#define CHECKBOX_WD     38     
//
//
int drawCheckBarEx( float x, float y, float z, float scale, float wd, const char * text, int down, int selectable, int gray, int isPower, int halfText, AtlasTex * icon )
{
    char sz[4] = "lg", sl[32] = {0}, tmp[128];

    AtlasTex *meat, *meatHi, *meatLo, *meatR, *meatHiR, *meatLoR, *frameL, *frameMID, *frameR;
    AtlasTex* meatUnder = NULL;
    AtlasTex* meatUnderL = NULL;
    AtlasTex* meatUnderR = NULL;
     int clrBase, clrText, clrText2, clrHigh, clrFrame, offset = 0, retValue = D_NONE;
    CBox box;

    // first load the proper sprites and pick colors
    //---------------------------------------------------------------------------------
       if( (down||isPower) && !gray )
    {
        clrText = uiColors.selected.text;
        clrText2 = uiColors.selected.text2;
         clrBase = uiColors.selected.base;
        clrFrame = uiColors.selected.frame;
        clrHigh = uiColors.selected.highlight;
        strcpy( sl, "selected_" );
    }
    else
    {
        if ( !gray )
        {
            clrText = uiColors.standard.text;
            clrText2 = uiColors.standard.text2;
            clrBase = uiColors.standard.base;
            clrFrame = uiColors.standard.frame;
            clrHigh = uiColors.standard.highlight;
        }
        else
        {
              clrText = uiColors.unselectable.text;
            clrText2 = uiColors.unselectable.text2;
             clrBase = uiColors.unselectable.base;
            clrFrame = uiColors.unselectable.frame;
            clrHigh = uiColors.unselectable.highlight;
        }
    }

    if( game_state.skin != UISKIN_HEROES )
    {
           sprintf( tmp, "checkbar_frame_%s_%sL_bulb_tintable.tga", sz, sl );
        frameL = atlasLoadTexture( tmp );

        sprintf( tmp, "checkbar_frame_%s_%sMID_tintable.tga", sz, sl );
        frameMID = atlasLoadTexture( tmp );

        sprintf( tmp, "checkbar_frame_%s_%sR_tintable.tga", sz, sl );
        frameR = atlasLoadTexture( tmp );
    }
    else
    {
        sprintf( tmp, "checkbar_frame_%s_%sL_bulb.tga", sz, sl );
        frameL = atlasLoadTexture( tmp );

        sprintf( tmp, "checkbar_frame_%s_%sMID.tga", sz, sl );
        frameMID = atlasLoadTexture( tmp );

        sprintf( tmp, "checkbar_frame_%s_%sR.tga", sz, sl );
        frameR = atlasLoadTexture( tmp );
    }

    meat    = atlasLoadTexture( "checkbar_meat_base_MID.tga" );
    meatR    = atlasLoadTexture( "checkbar_meat_base_R.tga" );
    meatHi    = atlasLoadTexture( "checkbar_meat_highlight_MID.tga" );
    meatHiR = atlasLoadTexture( "checkbar_meat_highlight_R.tga" );
    meatLo    = atlasLoadTexture( "checkbar_meat_shadow_MID.tga" );
    meatLoR = atlasLoadTexture( "checkbar_meat_shadow_R.tga" );

    // The yellow underlight is specific to Praetorian version of the UI.
    if( game_state.skin == UISKIN_PRAETORIANS )
    {
        meatUnder = atlasLoadTexture( "checkbar_meat_base_underlight_mid" );
        meatUnderL = atlasLoadTexture( "checkbar_meat_base_underlight_L" );
        meatUnderR = atlasLoadTexture( "checkbar_meat_base_underlight_R" );
    }

    // whew, now we can draw them
    //----------------------------------------------------------------------------

    BuildCBox( &box, x, y - frameL->height * scale/2, wd, frameL->height * scale);
    if( mouseCollision( &box ) )
    {
        if( isDown( MS_LEFT ) )
            retValue = D_MOUSEDOWN;
        else
            retValue = D_MOUSEOVER;
    }

    {
        int checkRetValue = drawCheckBox( x, y, z+4, scale, clrBase, down, selectable, gray, retValue, isPower, icon );

        if( checkRetValue > retValue )
            retValue = checkRetValue;
    }

     if ( retValue && selectable )
    {
        brightenColor( &clrBase, 60 );
        brightenColor( &clrText, 60 );

        if( !halfText )
            scale += .1f;
    }

    // left frame
     display_sprite( frameL, x - scale*frameL->width/2 - (3*scale), y - scale*frameL->height/2, z+5, scale, scale, clrFrame );

     // middle frame
    display_sprite( frameMID, x, y - scale*frameMID->height/2, z+3, 
        (wd - scale*frameR->width)/frameMID->width, scale, clrFrame );

    // right frame
    display_sprite( frameR, x + wd - scale*frameR->width, y - scale*frameR->height/2, z+4, scale, scale, clrFrame );

    // middle meat
    display_sprite( meat,   x, y - scale*meat->height/2,   z,   (wd - scale * frameR->width)/meat->width,   scale, clrBase    );
    display_sprite( meatHi, x, y - scale*meatHi->height/2, z+1, (wd - scale * frameR->width)/meatHi->width, scale, clrHigh   );
    display_sprite( meatLo, x, y - scale*meatLo->height/2, z+2, (wd - scale * frameR->width)/meatLo->width, scale, CLR_SHADOW );

    // right meat
    display_sprite( meatR,   x + (wd - scale * frameR->width ), y - scale*meatR->height/2,   z,   scale, scale, clrBase    );
    display_sprite( meatHiR, x + (wd - scale * frameR->width), y - scale*meatHiR->height/2, z+1, scale, scale, clrHigh   );
     display_sprite( meatLoR, x + (wd - scale * frameR->width), y - scale*meatLoR->height/2, z+2, scale, scale, CLR_SHADOW );

    // underlight for Praetorians
    if( !gray && game_state.skin == UISKIN_PRAETORIANS )
    {
        display_sprite( meatUnder, x, y - scale * meatUnder->height / 2, z + 3, (wd - scale * frameR->width) / meatUnder->width, scale, CLR_WHITE );
        display_sprite( meatUnderR, x + (wd - scale * frameR->width), y - scale * meatUnderR->height / 2, z + 3, scale, scale, CLR_WHITE );
    }

    // and lastly the text
      font_color( clrText, clrText2 );

    if( halfText )
    {
        font( &title_9 );
        prnt(x + scale*CHECKBOX_WD/2, y, z+5, scale, scale, text );
    }
    else
    {
        float sc;
        font( &title_12 );
           sc = MIN( scale, str_sc( font_grp, wd-1.5*frameR->width*scale, text ));
           prnt(x + (CHECKBOX_WD/2 + 5)*scale, y + (5*scale), z+5, sc, sc, text );
    }
 
    if( mouseClickHit( &box, MS_LEFT ) && selectable )
        return D_MOUSEHIT;
    else
        return retValue;

}

int drawCheckBar( float x, float y, float z, float scale, float wd, const char * text, int down, int selectable, int isPower, int halfText, AtlasTex * icon )
{
    return drawCheckBarEx(x, y, z, scale, wd, text, down, selectable, !selectable, isPower, halfText, icon);
}

//
//
int drawMenuBarSquished( float x, float y, float z, float sc, float wd, const char * text, int isOn, int drawLeft, int isLit, int grey, 
                        int unclickable, int invalid, float squish, int bordercolor, TTDrawContext *buttonFont, int useStandardColors )
{
    char sl[32] = {0}, tmp[128];
    int clrText, clrBase, retValue = D_NONE;
    AtlasTex *meat, *meatHi, *meatLo, *meatL, *meatHiL, *meatLoL, *meatR, *meatHiR, *meatLoR, *frameL, *frameMID, *frameR;
    AtlasTex* meatUnderL = NULL;
    AtlasTex* meatUnder = NULL;
    AtlasTex* meatUnderR = NULL;

    float w;
    CBox box;

    // first load the proper sprites and pick colors
    //---------------------------------------------------------------------------------
      if( isOn && !unclickable && !grey)
    {
        if(isMenu(MENU_GAME))
        {
            if( invalid )
                 clrText = 0xff8888ff;
            else
                clrText = CLR_WHITE;
            clrBase = CLR_WHITE;
        }
        else
        {
            clrText = uiColors.selected.text;
            clrBase = uiColors.selected.base;
        }
        strcpy( sl, "selected_" );
    }
    else if ( grey )
    {
        clrText = 0x888888ff;
        clrBase = DARK_GREY;
    }
    else if (invalid)
    {
        clrText = 0x888888ff;
        clrBase = CLR_RED;
    }
    else
    {
        clrText = uiColors.standard.text;
        clrBase = uiColors.standard.base;
    }

       if (game_state.skin != UISKIN_HEROES && !isMenu(MENU_GAME) )
    {
         sprintf( tmp, "checkbar_frame_lg_%sL_tintable.tga", sl );
        frameL = atlasLoadTexture( tmp );

        sprintf( tmp, "checkbar_frame_lg_%sMID_tintable.tga", sl );
        frameMID = atlasLoadTexture( tmp );

        sprintf( tmp, "checkbar_frame_lg_%sR_tintable.tga", sl );
        frameR = atlasLoadTexture( tmp );
    }
    else
    {
        sprintf( tmp, "checkbar_frame_lg_%sL.tga", sl );
        frameL = atlasLoadTexture( tmp );

        sprintf( tmp, "checkbar_frame_lg_%sMID.tga", sl );
        frameMID = atlasLoadTexture( tmp );

        sprintf( tmp, "checkbar_frame_lg_%sR.tga", sl );
        frameR = atlasLoadTexture( tmp );
    }

    meat    = atlasLoadTexture( "checkbar_meat_base_MID.tga" );
    meatR    = atlasLoadTexture( "checkbar_meat_base_R.tga" );
    meatHi    = atlasLoadTexture( "checkbar_meat_highlight_MID.tga" );
    meatHiR = atlasLoadTexture( "checkbar_meat_highlight_R.tga" );
    meatLo    = atlasLoadTexture( "checkbar_meat_shadow_MID.tga" );
    meatLoR = atlasLoadTexture( "checkbar_meat_shadow_R.tga" );
    meatL    = atlasLoadTexture( "checkbar_meat_base_L.tga" );
    meatHiL = atlasLoadTexture( "checkbar_meat_highlight_L.tga" );
    meatLoL = atlasLoadTexture( "checkbar_meat_shadow_L.tga" );

    if( game_state.skin == UISKIN_PRAETORIANS )
    {
        meatUnderL = atlasLoadTexture( "checkbar_meat_base_Underlight_L.tga" );
        meatUnder = atlasLoadTexture( "checkbar_meat_base_Underlight_Mid.tga" );
        meatUnderR = atlasLoadTexture( "checkbar_meat_base_Underlight_R.tga" );
    }

    w = sc*frameL->width;
    wd *= sc;
     BuildCBox( &box, x - sc*wd/2, y - w/2, sc*wd, w );

    if( (mouseCollision( &box ) || isLit) && !grey && !unclickable )
    {
        brightenColor( &clrBase, 45 );
        retValue    = D_MOUSEOVER;

        if( isDown( MS_LEFT ) )
        {
            retValue = D_MOUSEDOWN;
            sc *= .9f;
            clrText = uiColors.clicked.text;
            clrBase = uiColors.clicked.base;
            w *= 0.9;
            wd *= 0.9;
        }
    }

    // whew, now we can draw them
    //----------------------------------------------------------------------------

    sc *= squish;

    if( drawLeft )
    {
        // left frame
         display_sprite( frameL, x - wd/2, y - sc*frameL->height/2, z+5, sc, sc, grey? DARK_GREY:bordercolor );

        // left meat
        display_sprite( meatL,   x - (wd/2), y - sc*meatL->height/2,   z+1, sc, sc, clrBase    );
        display_sprite( meatHiL, x - (wd/2), y - sc*meatHiL->height/2, z+2, sc, sc, CLR_HIGH   );
        display_sprite( meatLoL, x - (wd/2), y - sc*meatLoL->height/2, z+3, sc, sc, CLR_SHADOW );

        if( game_state.skin == UISKIN_PRAETORIANS )
        {
            display_sprite( meatUnderL, x - (wd/2), y - sc*meatUnderL->height/2, z+4, sc, sc, CLR_WHITE );
        }
    }

    // middle frame
    display_sprite( frameMID, x - wd/2 + sc*meatL->width, y - sc*frameMID->height/2, z+6, 
           (wd - sc*2*meatL->width)/(frameMID->width), sc, grey ? DARK_GREY:bordercolor );

    // right frame
     display_sprite( frameR, x + wd/2 - sc*frameR->width, y - sc*frameR->height/2, z+5, sc, sc, grey? DARK_GREY:bordercolor );

    // middle meat
    display_sprite( meat,   x - wd/2 + sc*meatL->width, y - sc*meat->height/2,   z,   (wd - sc*meatL->width - sc*meatR->width)/(meat->width),   sc, clrBase    );
    display_sprite( meatHi, x - wd/2 + sc*meatL->width, y - sc*meatHi->height/2, z+1, (wd - sc*meatL->width - sc*meatR->width)/(meatHi->width), sc, CLR_HIGH   );
    display_sprite( meatLo, x - wd/2 + sc*meatL->width, y - sc*meatLo->height/2, z+2, (wd - sc*meatL->width - sc*meatR->width)/(meatLo->width), sc, CLR_SHADOW );

    if( game_state.skin == UISKIN_PRAETORIANS )
    {
        display_sprite( meatUnder, x - wd/2 + sc*meatUnderL->width, y - sc*meatUnder->height/2, z+3, (wd - sc*meatL->width - sc*meatR->width)/(meatUnder->width), sc, CLR_WHITE );
    }

    // right meat
    display_sprite( meatR,   x + (wd/2 - sc*meatR->width), y - sc*meatR->height/2,   z+1, sc, sc, clrBase    );
    display_sprite( meatHiR, x + (wd/2 - sc*meatR->width), y - sc*meatHiR->height/2, z+2, sc, sc, CLR_HIGH   );
    display_sprite( meatLoR, x + (wd/2 - sc*meatR->width), y - sc*meatLoR->height/2, z+3, sc, sc, CLR_SHADOW );

    if( game_state.skin == UISKIN_PRAETORIANS )
    {
        display_sprite( meatUnderR, x + (wd/2 - sc*meatR->width), y - sc*meatUnderR->height/2, z+4, sc, sc, CLR_WHITE );
    }

    // and lastly the text
     font( buttonFont );
    if( invalid ) {                        font_color( 0xff8888ff, 0xff8888ff ); }
    else if (useStandardColors) {        font_color(  uiColors.standard.text, uiColors.standard.text2 );    } 
    else {                                font_color( CLR_WHITE, clrText);        }
    cprnt( x, y + sc*6, z+6, sc*1.1f, sc*1.1f, text );

    if( mouseClickHit( &box, MS_LEFT ) && !unclickable )
        return D_MOUSEHIT;
    else
        return retValue;
}

void drawLargeCapsule( float centerX, float centerY, float z, float wd, float scale )
{
    AtlasTex *frameL    = atlasLoadTexture( "EnhncTray_tray_L.tga" );
    AtlasTex *frameMid    = atlasLoadTexture( "EnhncTray_tray_MID.tga" );
    AtlasTex *frameR    = atlasLoadTexture( "EnhncTray_tray_R.tga" );
    AtlasTex *backL        = atlasLoadTexture( "EnhncTray_trayback_L.tga" );
    AtlasTex *backR        = atlasLoadTexture( "EnhncTray_trayback_R.tga" );
    AtlasTex *back        = white_tex_atlas;

    float tx, ty;

    if( wd < frameL->width + frameR->width )
        scale = wd/(frameL->width + frameR->width);

    // left side
    tx = centerX - wd/2;
    ty = centerY - frameL->height*scale/2;
    display_sprite( frameL, tx, ty, z+1, scale, scale, CLR_WHITE );
    display_sprite( backL,  tx, ty, z,   scale, scale, CLR_BLACK );

    // right side
    tx = centerX + wd/2 - frameR->width*scale;
    display_sprite( frameR, tx, ty, z+1, scale, scale, CLR_WHITE );
    display_sprite( backR,  tx, ty, z,   scale, scale, CLR_BLACK );

    // middle
    if( wd > (frameL->width + frameR->width) )
    {
         tx = centerX - wd/2 + frameL->width*scale;
         display_sprite( frameMid, tx, ty, z+1, (wd-(frameL->width + frameR->width)*scale)/frameMid->width, scale, CLR_WHITE );
         display_sprite( back, tx, ty, z, (wd-(frameL->width + frameR->width)*scale)/back->width, frameMid->height/back->height, CLR_BLACK );
    }
}


//
//
int selectableText( float x, float y, float z, float scale, const char* txt, int color1, int color2, int center, int translate )
{
    CBox txtBox;
    char buf[128];
    int retVal = FALSE; 

    static int clr[4][3];
    static int up[4][3] = {0};
    static int init = FALSE;
      int rgba[4] = {color2, color1, color1, color2};

    assert(txt);

    if( !init )
    {
        int i, j;
        for( i = 0; i < 4; i++ )
            for( j = 0; j < 3; j++ )
            {
                clr[i][j]  = rand()%255;
            }
        init = TRUE;
    }

    txtBox.lx = x;
    txtBox.ly = y;

    if( translate )
    {
        msPrintf( menuMessages, SAFESTR(buf), txt );
    }
    else
        strcpy( buf, txt );

    str_dims( &game_12, scale, scale, center, &txtBox, buf );
    if( mouseCollision( &txtBox ) )
    {
        int i, j;
         for( i = 0; i < 4; i++ )
            for( j = 0; j < 3; j++ )
            {

                if( !up[i][j] )
                    clr[i][j] += rand()%10;
                else
                    clr[i][j] -= rand()%10;

                if ( clr[i][j] > 255 )
                {
                    up[i][j] = 1;
                    clr[i][j] = 255;
                }
                if ( clr[i][j] < 0 )
                {
                    up[i][j] = 0;
                    clr[i][j] = 0;
                }

            }

        rgba[0] = ((clr[0][0])<<24) + ((clr[0][1])<<16) + ((clr[0][2])<<8) + 255; //CLR_RED;
        rgba[1] = ((clr[1][0])<<24) + ((clr[1][1])<<16) + ((clr[1][2])<<8) + 255;//CLR_WHITE;
        rgba[2] = ((clr[2][0])<<24) + ((clr[2][1])<<16) + ((clr[2][2])<<8) + 255;//CLR_BLUE;
        rgba[3] = ((clr[3][0])<<24) + ((clr[3][1])<<16) + ((clr[3][2])<<8) + 255; //CLR_GREEN;

        if( mouseDown( MS_LEFT ) )
            scale -= .1f;

        if( mouseClickHit( &txtBox, MS_LEFT) )
        {
            retVal = TRUE;
        }
    }


    //determineTextColor(rgba);
    printBasic( &game_12, x, y, z+5, scale, scale, center, buf, strlen(buf), rgba );

    return retVal;
}

int gHelpOverlay = 0;
static AtlasTex *gHelpTexture = 0;
static float opacity = 0;

void setHelpOverlay( const char *texName )
{
    if( opacity == 0 )
        gHelpOverlay = TRUE;

    sndPlay( "Help", SOUND_GAME );

    gHelpTexture = atlasLoadTexture( texName );
}

void drawHelpOverlay()
{
    if( !gHelpTexture )
         return;

    if( !gHelpOverlay )
    {
        opacity -= TIMESTEP*30;
        if( opacity < 0 )
        {
            opacity = 0;
        }
    }
    else
    {
        opacity += TIMESTEP*30;
        if( opacity > 255 )
            opacity = 255;
    }

       if( inpGetKeyBuf() || isDown( MS_LEFT ) || isDown( MS_RIGHT ) )
        gHelpOverlay = 0;

    if( opacity > 0 )
        display_sprite( gHelpTexture, 0, 0, 6000, gHelpTexture->width/DEFAULT_SCRN_WD, gHelpTexture->height/DEFAULT_SCRN_HT, (0xffffff00 | (int)opacity) );
}

void colorRingGetSize( float* x, float* y )
{
    AtlasTex* back      = atlasLoadTexture("costume_color_ring_background.tga");
     AtlasTex* ring      = atlasLoadTexture("costume_color_ring_main.tga");
//    AtlasTex* swirl     = atlasLoadTexture("tray_ring_endurancemeter.tga");//costume_color_selected.tga");
//    AtlasTex* swirl2    = atlasLoadTexture("costume_color_selected.tga");
    AtlasTex* shadow    = atlasLoadTexture("costume_color_selected_3d.tga");
    AtlasTex* tex[] = {back, ring, shadow};

    if(x)
    {
        int maxWidth = 0;
        int i;
        for(i = 0; i < ARRAY_SIZE(tex); i++)
        {
            maxWidth = max(maxWidth, tex[i]->width);
        }
        *x = maxWidth;
    }
    if(y)
    {
        int maxHeight = 0;
        int i;
        for(i = 0; i < ARRAY_SIZE(tex); i++)
        {
            maxHeight = max(maxHeight, tex[i]->height);
        }
        *y = maxHeight;
    }
}

int colorRingDraw( float x, float y, float z, CRDMode mode, int color, float scale )
{
    static float timer = 0;
    int retval = 0;

    CBox box;

    AtlasTex* back      = atlasLoadTexture("costume_color_ring_background.tga");
    AtlasTex* ring      = atlasLoadTexture("costume_color_ring_main.tga");
    AtlasTex* shadow    = atlasLoadTexture("costume_color_selected_3d.tga");
    AtlasTex* selectionRing = atlasLoadTexture("select_color_ring.tga");

    BuildCBox( &box, x -ring->width*scale/2, y - ring->height*scale/2, ring->width*scale, ring->height*scale);

    display_sprite( back, x - scale*back->width/2, y - scale*back->height/2, z,   scale, scale, color );
    display_sprite( ring, x - scale*ring->width/2, y - scale*back->height/2, z+2, scale, scale, CLR_WHITE );

    if(mode & CRD_SELECTED)
    {
        display_sprite(shadow, x - scale*shadow->width/2, y - scale*shadow->width/2, z+1, scale, scale, CLR_WHITE);
        display_sprite(selectionRing, x-scale*selectionRing->width/2, y - scale*selectionRing->width/2, z+1, scale, scale, CLR_WHITE );
    }

    if(mode & CRD_MOUSEOVER)
    {
        if( mouseCollision( &box ) )
            display_sprite(selectionRing, x-scale*selectionRing->width/2, y - scale*selectionRing->width/2, z+1, scale, scale, CLR_CONSTRUCT_EX(255, 255, 255, 0.5));
    }

    if(mode & CRD_SELECTABLE)
    {
        if( mouseClickHit( &box, MS_LEFT ) )
        {    
            sndPlay( "color", SOUND_GAME );
            retval = 1;
        }
    }

    return retval;
}

ACButtonResult drawAcceptCancelButton(ACButton buttons, float screenScaleX, float screenScaleY)
{
    UIBox buttonBox;
    int flag;
    int buttonResult;
    ACButtonResult result = 0;
    float screenScale = MIN(screenScaleX, screenScaleY);
    flag = 0;
    if(buttons & ACB_CANCEL)
    {
        flag |= BUTTON_LOCKED;
    }
    uiBoxDefine(&buttonBox, 16*screenScaleX, 720*screenScaleY, 88*screenScaleX, 42*screenScaleY);
    buttonResult = uiBoxDrawStdButton(buttonBox, 5000, CLR_DARK_RED, "CancelString", 2.0*screenScale, flag);
    if(D_MOUSEHIT == buttonResult)
    {
        result |= ACBR_CANCEL;
    }
    else if(D_MOUSELOCKEDHIT == buttonResult)
    {
        result |= ACBR_CANCEL_LOCKED;
    }

    flag = 0;
    if(buttons & ACB_ACCEPT)
    {
        flag |= BUTTON_LOCKED;
    }
    uiBoxDefine(&buttonBox, 922*screenScaleX, 720*screenScaleY, 88*screenScaleX, 42*screenScaleY);
    buttonResult = uiBoxDrawStdButton(buttonBox, 5000, CLR_GREEN, "AcceptString", 2.0*screenScale, flag);
    if(D_MOUSEHIT == buttonResult)
    {
        result |= ACBR_ACCEPT;
    }
    else if(D_MOUSELOCKEDHIT == buttonResult)
    {
        result |= ACBR_ACCEPT_LOCKED;
    }

    return result;
}

void drawTextWithDot( float x, float y, float z, float scale, const char * txt )
{
    AtlasTex *dot = atlasLoadTexture( "EnhncTray_powertitle_dot.tga" );

       display_sprite( dot, x, y - (dot->height*scale), z, scale, scale, CLR_WHITE );

     font( &game_18 );
    font_color( CLR_WHITE, CLR_WHITE );
    if (getCurrentLocale() == 3) cprntEx( x + ((dot->width + 5)*scale), y - (dot->height*scale)/2, z, scale, 0.8f*scale, CENTER_Y, txt );
    else cprntEx( x + ((dot->width + 5)*scale), y - (dot->height*scale)/2, z, scale, scale, CENTER_Y, txt );
}


UIColors uiColors;

static void setElemColors(UIElemColors *colors, int base, int frame, int fill, int text, int text2, int highlight, int scrollbar)
{
    colors->base = base;
    colors->frame = frame;
    colors->fill = fill;
    colors->text = text;
    colors->text2 = text2;
    colors->highlight = highlight;
    colors->scrollbar = scrollbar;
}

void setupUIColors(void)
{
    if (game_state.skin == UISKIN_PRAETORIANS)
    {
        setElemColors(&uiColors.standard, 0x424242ff, CLR_WHITE, 0x151515ff, CLR_WHITE, CLR_WHITE, 0xffffff88, CLR_WHITE);
        setElemColors(&uiColors.selected, 0x989898ff, 0xe0ff00ff, DARK_GREY, CLR_WHITE, CLR_WHITE, 0xffffff88, CLR_GREY);
        setElemColors(&uiColors.clicked, 0x989898ff, 0xffe763ff, DARK_GREY, CLR_WHITE, CLR_WHITE, 0xffffff88, CLR_GREY);
        setElemColors(&uiColors.unselectable, CLR_BASE_GREY, CLR_FRAME_GREY, CLR_CFILL_GREY, CLR_WHITE, CLR_TXT_GREY, 0xffffff22, CLR_GREY);
    }
    else if (game_state.skin == UISKIN_VILLAINS)
    {
        setElemColors(&uiColors.standard, CLR_BASE_RED, CLR_WHITE, CLR_CFILL_RED, CLR_WHITE, CLR_TXT_RED, CLR_HIGH, CLR_WHITE);
        setElemColors(&uiColors.selected, CLR_BASE_LRED, CLR_VILLAGON, CLR_CFILL_LRED, CLR_WHITE, CLR_TXT_LRED, CLR_HIGH, CLR_RED);
        setElemColors(&uiColors.clicked, CLR_BASE_LRED, CLR_VILLAGON, CLR_CFILL_LRED, CLR_WHITE, CLR_WHITE, CLR_HIGH, CLR_RED);
        setElemColors(&uiColors.unselectable, CLR_BASE_GREY, CLR_FRAME_GREY, CLR_CFILL_GREY, CLR_WHITE, CLR_TXT_GREY, 0xffffff22, CLR_GREY);
    }
    else
    {
        setElemColors(&uiColors.standard, CLR_BASE_BLUE, CLR_FRAME_BLUE, CLR_CFILL_BLUE, CLR_WHITE, CLR_TXT_BLUE, CLR_HIGH, CLR_WHITE);
        setElemColors(&uiColors.selected, CLR_BASE_GREEN, CLR_FRAME_GREEN, CLR_CFILL_GREEN, CLR_WHITE, CLR_TXT_GREEN, CLR_HIGH, CLR_GREEN);
        setElemColors(&uiColors.clicked, CLR_BASE_GREEN, CLR_FRAME_GREEN, CLR_CFILL_GREEN, CLR_WHITE, CLR_WHITE, CLR_HIGH, CLR_GREEN);
        setElemColors(&uiColors.unselectable, CLR_BASE_GREY, CLR_FRAME_GREY, CLR_CFILL_GREY, CLR_WHITE, CLR_TXT_GREY, 0xffffff22, CLR_GREY);
    }
}

enum{
    NLM_SELECTED,
    NLM_MOUSE,
    NLM_BLOCKED,
    NLM_DEFAULT,
};

int drawNonLinearMenu(NonLinearMenu * nlm, float startx, float y, float z, float wd, float ht, float sc, int currentMenu, int displayCurrentMenu, int locked)
{
    int i, oneCreated = 0;
    int x;
    AtlasTex *DefaultArrowL, *DefaultArrowR, *DefaultMid, *DefaultRound;
    AtlasTex *OnClickArrowL, *OnClickArrowR, *OnClickMid, *OnClickRound;
    AtlasTex *SelectedArrowL, *SelectedArrowR, *SelectedMid, *SelectedRound;
    AtlasTex *NonSelectedArrowL, *NonSelectedArrowR, *NonSelectedMid, *NonSelectedRound;
    AtlasTex* ArrowL;
    AtlasTex* ArrowR;
    AtlasTex* Mid;
    AtlasTex* Round = NULL;
    TTDrawContext *currentFont = &title_12;
    int currentStatus = NLM_DEFAULT, prevStatus = NLM_DEFAULT;
    int indexOfClicked = -1;
    float buttonScaleX = 1.0f, buttonScaleY = 1.0f;
    float currentButtonScaleX = buttonScaleX;
    float currentButtonScaleY = buttonScaleY;
    float currentX = 0;
    int textWidth = 0;
    float txt_sc = 1.0f;
    int defaultButtonSpacing = 150;
    float scalex, scaley; 
    static float pulseTimer = 0.0f;
    int pulseAlpha = 0;
    assertmsg(nlm, "No Menu");
    if (!nlm)
    {
        return 0;
    }

    calc_scrn_scaling(&scalex, &scaley);
    NonSelectedArrowL = atlasLoadTexture("CCNAV_NonSelected_Arrow_L");
    NonSelectedArrowR = atlasLoadTexture("CCNAV_NonSelected_Arrow_R");
    NonSelectedMid = atlasLoadTexture("CCNAV_NonSelected_Mid");
    NonSelectedRound = atlasLoadTexture("CCNAV_NonSelected_Round");
    if (game_state.skin == UISKIN_PRAETORIANS)
    {
        DefaultArrowL = atlasLoadTexture("CCNAV_P_Default_Arrow_L");
        DefaultArrowR = atlasLoadTexture("CCNAV_P_Default_Arrow_R");
        DefaultMid = atlasLoadTexture("CCNAV_P_Default_Mid");
        DefaultRound = atlasLoadTexture("CCNAV_P_Default_Round");

        OnClickArrowL = atlasLoadTexture("CCNAV_P_OnClick_Arrow_L");
        OnClickArrowR = atlasLoadTexture("CCNAV_P_OnClick_Arrow_R");
        OnClickMid = atlasLoadTexture("CCNAV_P_OnClick_Mid");
        OnClickRound = atlasLoadTexture("CCNAV_P_OnClick_Round");

        SelectedArrowL = atlasLoadTexture("CCNAV_P_Selected_Arrow_L");
        SelectedArrowR = atlasLoadTexture("CCNAV_P_Selected_Arrow_R");
        SelectedMid = atlasLoadTexture("CCNAV_P_Selected_Mid");
        SelectedRound = atlasLoadTexture("CCNAV_P_Selected_Round");
    }
    else if (game_state.skin == UISKIN_VILLAINS)
    {
        DefaultArrowL = atlasLoadTexture("CCNAV_V_Default_Arrow_L");
        DefaultArrowR = atlasLoadTexture("CCNAV_V_Default_Arrow_R");
        DefaultMid = atlasLoadTexture("CCNAV_V_Default_Mid");
        DefaultRound = atlasLoadTexture("CCNAV_V_Default_Round");

        OnClickArrowL = atlasLoadTexture("CCNAV_V_OnClick_Arrow_L");
        OnClickArrowR = atlasLoadTexture("CCNAV_V_OnClick_Arrow_R");
        OnClickMid = atlasLoadTexture("CCNAV_V_OnClick_Mid");
        OnClickRound = atlasLoadTexture("CCNAV_V_OnClick_Round");

        SelectedArrowL = atlasLoadTexture("CCNAV_V_Selected_Arrow_L");
        SelectedArrowR = atlasLoadTexture("CCNAV_V_Selected_Arrow_R");
        SelectedMid = atlasLoadTexture("CCNAV_V_Selected_Mid");
        SelectedRound = atlasLoadTexture("CCNAV_V_Selected_Round");
    }
    else
    {
        DefaultArrowL = atlasLoadTexture("CCNAV_H_Default_Arrow_L");
        DefaultArrowR = atlasLoadTexture("CCNAV_H_Default_Arrow_R");
        DefaultMid = atlasLoadTexture("CCNAV_H_Default_Mid");
        DefaultRound = atlasLoadTexture("CCNAV_H_Default_Round");

        OnClickArrowL = atlasLoadTexture("CCNAV_H_OnClick_Arrow_L");
        OnClickArrowR = atlasLoadTexture("CCNAV_H_OnClick_Arrow_R");
        OnClickMid = atlasLoadTexture("CCNAV_H_OnClick_Mid");
        OnClickRound = atlasLoadTexture("CCNAV_H_OnClick_Round");

        SelectedArrowL = atlasLoadTexture("CCNAV_H_Selected_Arrow_L");
        SelectedArrowR = atlasLoadTexture("CCNAV_H_Selected_Arrow_R");
        SelectedMid = atlasLoadTexture("CCNAV_H_Selected_Mid");
        SelectedRound = atlasLoadTexture("CCNAV_H_Selected_Round");
    }

    font(currentFont);
    //    see if the total width is enough
    nlm->currentMenu = currentMenu;
    buttonScaleY = ht*1.0f/(DefaultRound->height-20);
    buttonScaleX = buttonScaleY * scaley/scalex;
    currentButtonScaleX = buttonScaleX;
    currentButtonScaleY = buttonScaleY;

    wd -= (30*buttonScaleX);        //    34 = (round->width) - (2*8)    for extra white space
    startx -= 10*buttonScaleX+PIX3;    //    because the DefaultButton has some extra space on it
    y -= 1;                                            //    because the DefaultButton has some extra space as well
    x = startx;

    //    first pass determines if the we need to scale it down to fit in the width
    for (i = 0; i < eaSize(&nlm->elementList); i++)
    {
        const char *title;
        int txt_wd;
        if (nlm->elementList[i]->hiddenCode && nlm->elementList[i]->hiddenCode())
        {
            continue;
        }
        if (nlm->elementList[i]->title[game_state.skin])
            title = nlm->elementList[i]->title[game_state.skin];
        else
            title = nlm->elementList[i]->title[0];
        txt_wd= str_wd(currentFont, sc, sc, textStd(title));
        textWidth += txt_wd;
        x += txt_wd + defaultButtonSpacing;
    }
    if (x > wd)
    {
        if (textWidth > wd)
        {
            sc *= wd*1.0/x;
            txt_sc = sc;
        }
        else
        {
            float rem = wd - textWidth;
            float spacingRem = x - textWidth;
            sc = rem *1.0f/spacingRem;
        }
    }
    currentX = x = startx;

    //actual drawing
    for (i = 0; i < eaSize(&nlm->elementList); ++i)
    {
        const char *title;
        if (nlm->elementList[i]->title[game_state.skin])
            title = nlm->elementList[i]->title[game_state.skin];
        else
            title = nlm->elementList[i]->title[0];
        if (nlm->elementList[i]->hiddenCode && nlm->elementList[i]->hiddenCode())
        {
            continue;
        }
        else
        {
            CBox mouseBox;
            int strwd = str_wd(currentFont, txt_sc, txt_sc, textStd(title))+(defaultButtonSpacing*sc);
            float textureWidth = 0;
            float current_txt_sc = txt_sc;
            if (oneCreated)
            {
                BuildCBox( &mouseBox, x+(3*currentButtonScaleX*DefaultArrowR->width/4), y, strwd, ht);
            }
            else
            {
                BuildCBox( &mouseBox, x, y, strwd+(3*currentButtonScaleX*DefaultArrowR->width/4), ht);
            }
            currentStatus = NLM_DEFAULT;
            if (mouseCollision(&mouseBox))
            {
                currentStatus = NLM_MOUSE;
            }
            if (i == displayCurrentMenu)
            {
                font_color( CLR_WHITE, SELECT_FROM_UISKIN( CLR_FRAME_BLUE, 0xFF9900ff, CLR_WHITE ) );
                //    current_txt_sc = 0.03+txt_sc;
                currentStatus = NLM_SELECTED;
            }
            else if (locked || (nlm->elementList[i]->cantEnterCode && (nlm->elementList[i]->cantEnterCode()) ))
            {    
                font_color( CLR_WHITE, CLR_GREY);
                currentStatus = NLM_BLOCKED;
            }
            else if (currentStatus == NLM_MOUSE)
            {
                font_color( SELECT_FROM_UISKIN( CLR_YELLOW, CLR_YELLOW, CLR_WHITE ), SELECT_FROM_UISKIN( CLR_FRAME_BLUE, 0xFF9900ff, CLR_WHITE ) );
                //    current_txt_sc = 0.02 + txt_sc;
            }
            else if (nlm->elementList[i]->flashingTimer > 0.0f)
            {
                int color1, color2;

                nlm->elementList[i]->flashingTimer -= 0.05f *TIMESTEP;
                MAX1(nlm->elementList[i]->flashingTimer, 0.0f);

                interp_rgba((sinf(nlm->elementList[i]->flashingTimer*10)+1)/2.0f, CLR_WHITE, SELECT_FROM_UISKIN( 0xff9900ff, CLR_FRAME_BLUE, CLR_FRAME_ROGUE ), &color1);
                interp_rgba((sinf(nlm->elementList[i]->flashingTimer*10)+1)/2.0f, CLR_WHITE, CLR_GREY, &color2);

                font_color(color1, color2);

                if (sinf(nlm->elementList[i]->flashingTimer*15) > 0)
                {
                    currentStatus = NLM_MOUSE;
                }
            }
            else
            {
                font_color( CLR_WHITE, SELECT_FROM_UISKIN( CLR_FRAME_BLUE, 0xFF9900ff, CLR_WHITE ) );
            }
            switch (currentStatus)
            {
            case NLM_SELECTED:
                {
                    ArrowL = SelectedArrowL;
                    ArrowR = SelectedArrowR;
                    Mid = SelectedMid;
                    Round = SelectedRound;
                }
                break;
            case NLM_BLOCKED:
                {
                    ArrowL = NonSelectedArrowL;
                    ArrowR = NonSelectedArrowR;
                    Mid = NonSelectedMid;
                    Round = NonSelectedRound;
                }
                break;
            case NLM_MOUSE:
                {
                    ArrowL = OnClickArrowL;
                    ArrowR = OnClickArrowR;
                    Mid = OnClickMid;
                    Round = OnClickRound;
                }
                break;
            case NLM_DEFAULT:
            default:
                {
                    ArrowL = DefaultArrowL;
                    ArrowR = DefaultArrowR;
                    Mid = DefaultMid;
                    Round = DefaultRound;
                }
                break;
            }
            switch (prevStatus)
            {
            case NLM_SELECTED:
                {
                    ArrowR = SelectedArrowR;
                }
                break;
            case NLM_BLOCKED:
                {
                    ArrowR = NonSelectedArrowR;
                }
                break;
            case NLM_MOUSE:
                {
                    ArrowR = OnClickArrowR;
                }
                break;
            case NLM_DEFAULT:
            default:
                {
                    ArrowR = DefaultArrowR;
                }
                break;
            }
            if (oneCreated)
            {
                display_sprite(ArrowR, currentX, y+((ht-(currentButtonScaleY*DefaultMid->height))/2.0f), z, currentButtonScaleX, currentButtonScaleY, CLR_WHITE);
                display_sprite(OnClickArrowR, currentX, y+((ht-(currentButtonScaleY*DefaultMid->height))/2.0f), z+1, currentButtonScaleX, currentButtonScaleY, pulseAlpha);
                if (currentStatus == NLM_SELECTED)
                {
                    int alpha = 0x000000ff * (sin(pulseTimer * PI)+1)/2;
                    //currentButtonScaleY = buttonScaleY + (sin(pulseTimer*PI)*0.04f);
                    //currentButtonScaleX = buttonScaleX + (sin(pulseTimer*PI)*0.08f);
                    //currentX = x - ((ArrowL->width+((ArrowR->width-6)/2.f))*(currentButtonScaleX - buttonScaleX)/2.f);
                    pulseAlpha = 0xffffff00 | alpha;
                    pulseTimer += 0.035f * TIMESTEP;
                    if (pulseTimer > 10)    
                        pulseTimer -=10;
                }
                else
                {

                    pulseAlpha = 0;
                }
                currentButtonScaleY = buttonScaleY;
                currentButtonScaleX = buttonScaleX;
                currentX = x;
                display_sprite(ArrowL, currentX+(currentButtonScaleX*(ArrowR->width-6)/2.0f), y+((ht-(currentButtonScaleY*DefaultMid->height))/2.0f), z, currentButtonScaleX, currentButtonScaleY, CLR_WHITE);
                display_sprite(OnClickArrowL, currentX+(currentButtonScaleX*(ArrowR->width-6)/2.0f), y+((ht-(currentButtonScaleY*DefaultMid->height))/2.0f), z+1, currentButtonScaleX, currentButtonScaleY, pulseAlpha);
                textureWidth = (ArrowL->width+((ArrowR->width-6)/2.0f))*currentButtonScaleX;
            }
            else
            {
                oneCreated = 1;
                if (currentStatus == NLM_SELECTED)
                {
                    int alpha;
                    pulseTimer += 0.035f * TIMESTEP;
                    if (pulseTimer > 10)    
                        pulseTimer -=10;
                    //currentButtonScaleY = buttonScaleY + (sin(pulseTimer*PI)*0.04f);
                    //currentButtonScaleX = buttonScaleX + (sin(pulseTimer*PI)*0.08f);
                    //currentX = x - (Round->width*(currentButtonScaleX - buttonScaleX)/2.f);
                    alpha = 0x000000ff * (sin(pulseTimer * PI)+1)/2;
                    pulseAlpha = 0xffffff00 | alpha;
                }
                else
                {
                    pulseAlpha = 0;
                }
                currentButtonScaleY = buttonScaleY;
                currentButtonScaleX = buttonScaleX;
                currentX = x;
                display_spriteFlip(Round, currentX, y+((ht-(currentButtonScaleY*DefaultMid->height))/2.0f), z, currentButtonScaleX, currentButtonScaleY, CLR_WHITE, FLIP_HORIZONTAL);
                display_spriteFlip(OnClickRound, currentX, y+((ht-(currentButtonScaleY*DefaultMid->height))/2.0f), z+1, currentButtonScaleX, currentButtonScaleY, pulseAlpha, FLIP_HORIZONTAL);
                textureWidth = Round->width*currentButtonScaleX;
            }
            cprntEx( currentX+((strwd+textureWidth)/2.0f), y+(ht/2.0f), z+3, current_txt_sc, current_txt_sc, CENTER_X | CENTER_Y, textStd(title) );
            display_sprite(Mid, currentX+textureWidth, y+((ht-(currentButtonScaleY*DefaultMid->height))/2.0f), z, (strwd-textureWidth)*1.0f/Mid->width, currentButtonScaleY, CLR_WHITE);
            display_sprite(OnClickMid, currentX+textureWidth, y+((ht-(currentButtonScaleY*DefaultMid->height))/2.0f), z+1, (strwd-textureWidth)*1.0f/Mid->width, currentButtonScaleY, pulseAlpha);
            x += strwd;
            currentX += strwd;
            if (mouseClickHit(&mouseBox, MS_LEFT) && (currentStatus != NLM_BLOCKED) && (i != nlm->currentMenu))
            {
                indexOfClicked = i;
            }
            prevStatus = currentStatus;
        }
    }

    display_sprite(Round, currentX, y+((ht-(currentButtonScaleY*DefaultMid->height))/2), z, currentButtonScaleX, currentButtonScaleY, CLR_WHITE);
    display_sprite(OnClickRound, currentX, y + ((ht - (currentButtonScaleY * DefaultMid->height)) / 2), z + 1, currentButtonScaleX, currentButtonScaleY, pulseAlpha);
    
    if (indexOfClicked != -1)
    {
        int j;
        if (nlm->elementList[nlm->currentMenu]->exitCleanupCode)
            nlm->elementList[nlm->currentMenu]->exitCleanupCode();
        for (j = nlm->currentMenu+1; j < indexOfClicked; ++j)
        {
            if (nlm->elementList[j]->passThroughCode)
                nlm->elementList[j]->passThroughCode();
        }
        nlm->currentMenu = indexOfClicked;
        start_menu(nlm->elementList[indexOfClicked]->menuId);
        if (nlm->elementList[indexOfClicked]->enterCode)
            nlm->elementList[indexOfClicked]->enterCode();
        return 1;
    }
    return 0;
}
NonLinearMenu *NLM_allocNonLinearMenu()
{
    NonLinearMenu *nlm = calloc(1, sizeof(NonLinearMenu));
    assertmsg(nlm, "Couldn't allocate enough memory to create Menu.");
    if (!nlm)    return NULL;
    nlm->transactionId = 1;
    return nlm;
}
void NLM_pushNewNLMElement(NonLinearMenu *nlm, NonLinearMenuElement *nlmElement)
{
    assert(nlm);
    assert(nlmElement);
    nlmElement->listId = eaPushUnique(&nlm->elementList, nlmElement);

    assert(nlmElement->nlm_owner == NULL);    //    If not, it has already been assigned somewhere else
    nlmElement->nlm_owner = nlm;
}
void NLM_incrementTransactionID(NonLinearMenu *nlm)
{
    nlm->transactionId++;
}
static int drawNLMButton( float cx, float cy, float z, float wd, float ht, int locked )
{
    CBox mouseBox;
    AtlasTex *DefaultMid, *DefaultRound;
    AtlasTex *MouseOverMid, *MouseOverRound;
    AtlasTex *NonSelectedMid, *NonSelectedRound;
    AtlasTex *currentMid, *currentRound;
    float buttonScaleX, buttonScaleY;
    float scalex, scaley;
    float midWidth = 0;
    int mouseOver = 0;
    int retVal = 0;

    calc_scrn_scaling(&scalex, &scaley);

    NonSelectedMid = atlasLoadTexture("CCNAV_NonSelected_Mid");
    NonSelectedRound = atlasLoadTexture("CCNAV_NonSelected_Round");

    if (game_state.skin == UISKIN_PRAETORIANS)
    {
        DefaultMid = atlasLoadTexture("CCNAV_P_Default_Mid");
        DefaultRound = atlasLoadTexture("CCNAV_P_Default_Round");

        MouseOverMid = atlasLoadTexture("CCNAV_P_OnClick_Mid");
        MouseOverRound = atlasLoadTexture("CCNAV_P_OnClick_Round");
    }
    else if (game_state.skin == UISKIN_VILLAINS)
    {
        DefaultMid = atlasLoadTexture("CCNAV_V_Default_Mid");
        DefaultRound = atlasLoadTexture("CCNAV_V_Default_Round");

        MouseOverMid = atlasLoadTexture("CCNAV_V_OnClick_Mid");
        MouseOverRound = atlasLoadTexture("CCNAV_V_OnClick_Round");
    }
    else
    {
        DefaultMid = atlasLoadTexture("CCNAV_H_Default_Mid");
        DefaultRound = atlasLoadTexture("CCNAV_H_Default_Round");

        MouseOverMid = atlasLoadTexture("CCNAV_H_OnClick_Mid");
        MouseOverRound = atlasLoadTexture("CCNAV_H_OnClick_Round");
    }

    buttonScaleY = ht*1.0f/(DefaultRound->height-20);
    buttonScaleX = buttonScaleY * scaley/scalex;
    BuildCBox(&mouseBox, cx, cy, wd, ht);
    if (!locked)
    {
        currentMid = DefaultMid;
        currentRound = DefaultRound;
        if (mouseCollision(&mouseBox))
        {
            currentMid = MouseOverMid;
            currentRound = MouseOverRound;
        }
        if (mouseClickHit(&mouseBox, MS_LEFT))
        {
            retVal = 1;
        }
    }
    else
    {
        currentMid = NonSelectedMid;
        currentRound = NonSelectedRound;
    }
    
    midWidth = wd - (2*(DefaultRound->width-7)*buttonScaleX);
    display_spriteFlip(currentRound, cx-(7*buttonScaleX), cy-(10*buttonScaleY), z, buttonScaleX, buttonScaleY, CLR_WHITE, FLIP_HORIZONTAL);
    display_sprite(currentMid, cx + (DefaultRound->width*buttonScaleX)-(7*buttonScaleX), cy-(10*buttonScaleY), z, midWidth/currentMid->width, buttonScaleY, CLR_WHITE);
    display_sprite(currentRound, cx+wd-((DefaultRound->width-7)*buttonScaleX), cy-(10*buttonScaleY), z, buttonScaleX, buttonScaleY, CLR_WHITE);
    return retVal;
}
int drawNLMIconButton(float x, float y, float z, float wd, float ht, char *txt, AtlasTex *tex, int iconYOffSet, int locked)
{
    float iconScale = 0.5;
    float txt_scale = 1.0f;
    float scalex, scaley;
    int texture_wd = tex ? tex->width *iconScale: 0;
    int txt_wd;
    font(&game_12);
    font_color( 0xffffffbf, 0xffffffbf);
    calc_scrn_scaling(&scalex, &scaley);
    scalex = MIN(scalex, 1.f);
    scaley = MIN(scaley, 1.f);
    txt_wd = str_wd(&game_9, scalex, scaley, txt );
    if (txt_wd > (wd-(texture_wd+22)))
    {
        txt_scale = (wd-(texture_wd+22))*1.f/txt_wd;
    }
    if (tex)
        display_sprite(tex, x+3, y+iconYOffSet, z+10, MIN(scalex, scaley)*iconScale/scalex, MIN(scalex, scaley)*iconScale/scaley, CLR_WHITE);
    cprntEx( x+3+(texture_wd ? (texture_wd+3) : 0), y+(ht/2), z+10, txt_scale*scalex, txt_scale*scaley, CENTER_Y, txt );
    return drawNLMButton(x, y, z, wd, ht, locked);
}

void setHeadingFontFromSkin(int light)
{
    switch (game_state.skin)
    {
        case UISKIN_HEROES:
        case UISKIN_VILLAINS:
        default:
            font( &title_12 );
            font_color( CLR_YELLOW, CLR_ORANGE );
            break;
        case UISKIN_PRAETORIANS:
            if (light)
            {
                font( &title_12 );
                font_color( CLR_WHITE, CLR_WHITE );
            }
            else
            {
                font( &title_12_no_outline );
                font_color( 0x190909ff, 0x190909ff );
            }
            break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// Going Rogue Expansion web purchase
//----------------------------------------------------------------------------------------------------------------------------------

static bool nagCbValue = false;
static int nagBitfield = 0;
static bool nagInited = false;

static DialogCheckbox nagCheckbox[] =
{
    {    "ExpansionNagDisable", &nagCbValue, 0    }
};

static void updateNag(GRNagContext ctx)
{
    if ((ctx == GRNAG_POWERSET || ctx == GRNAG_MORALITY) && nagCbValue)
    {
        nagBitfield = regGetAppInt("GRNagDisabled", 0);
        nagInited = true;

        nagBitfield |= 1 << ctx;
        regPutAppInt("GRNagDisabled", nagBitfield);
    }
}

static int isNagDisabled(GRNagContext ctx)
{
    int retval = 0;

    if (!nagInited)
        nagBitfield = regGetAppInt("GRNagDisabled", 0);

    if (ctx && (nagBitfield & (1 << ctx)))
        retval = 1;

    return retval;
}

static void buyExpansionCancel(void * data)
{
    updateNag((GRNagContext)data);

    // No point messing with the login/character select state machine if
    // we're being used from another screen.
    if (isMenu(MENU_LOGIN))
        GRNagCancelledLogin();
}

// the only way into this code anymore is via morality tip nags
static void buyExpansionWebSubmit(void * data)
{
#ifndef TEST_CLIENT
    webStoreOpenProduct("ctsygoro");
#endif
}

static void buyExpansionIngameSubmit(void * data)
{
    updateNag((GRNagContext)data);
}

void dialogGoingRogueNag(GRNagContext ctx)
{
    char *msg = "BuyGoingRogueWarning";
    int webpurchase = 1;
    int checkbox = 1;

    if (!isNagDisabled(ctx))
    {
        if (ctx == GRNAG_CHARSELECT || ctx == GRNAG_POWERSET)
            webpurchase = 0;

        if (ctx == GRNAG_CHARSELECT || ctx == GRNAG_NONE)
            checkbox = 0;

        // The nag dialog has to pop up immediately, so we have to kill off any
        // existing dialogs, including the character creation type dialog.
        dialogClearQueue(0);
        dialog(DIALOG_TWO_RESPONSE, -1, -1, -1, -1, textStd(msg),
            textStd("BuyExpansion"), webpurchase ? buyExpansionWebSubmit : buyExpansionIngameSubmit,
            textStd("CancelExpansion"), buyExpansionCancel,
            0, NULL, checkbox ? nagCheckbox : NULL, checkbox ? 1 : 0, 0, 0, (void *)ctx);
    }
}

int okToShowGoingRogueNag(GRNagContext ctx)
{
    int retval = 1;

    // The wording is different depending on which branch we're in, but it's
    // all shut off if the DbServer tells us so.
    if (!cfg_NagGoingRoguePurchase() ||
        dialogInQueue(textStd("BuyGoingRogueWarning"), NULL, NULL) ||
        dialogInQueue(textStd("MustExitToPurchase"), NULL, NULL))
    {
        retval = 0;
    }

    return retval;
}
