

//
#include <utilitieslib/components/Earray.h>
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"

#include "entity/entVarUpdate.h"   // for TIMESTEP

#include "UI/uiInput.h"
#include "graphics/ttFont.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "language/langClientUtil.h"
#include <utilitieslib/utils/mathutil.h>
#include "UI/uiBox.h"
#include "UI/uiFriend.h"
#include "UI/uiSupergroup.h"
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/strings_opt.h>
#include "graphics/ttFontUtil.h"
#include "UI/uiWindows.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "graphics/textureatlas.h"
#include "UI/uiClipper.h"
#include "render/renderprim.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "UI/uiPictureBrowser.h"
#include "UI/uiCursor.h"
#include "UI/uiPowerCust.h"

int drawCloseButton( F32 x, F32 y, F32 z, F32 sc, int back_color )
{
    CBox xbox;
    int xclr = 0xffffff88;
    AtlasTex * border = atlasLoadTexture( "Jelly_close_bevel.tga" );
    AtlasTex * click = atlasLoadTexture( "Jelly_close_click.tga" );
    AtlasTex * button = atlasLoadTexture( "Jelly_close_rest.tga" );

     BuildCBox( &xbox, x - border->width*sc/2 - 5*sc, y - border->height*sc/2 - 5*sc, (border->width + 10)*sc, (border->height + 10)*sc );

    if( mouseCollision( &xbox ) )
    {
        xclr = 0xffffffff;
        if( isDown( MS_LEFT ) )
            display_sprite( click, x - click->width*sc/2, y - click->height*sc/2, z+3, sc, sc, CLR_WHITE );
    }

    display_sprite( button, x - button->width*sc/2, y - button->height*sc/2, z+2, sc, sc, xclr );
    display_sprite( border, x - border->width*sc/2, y - border->height*sc/2, z+1, sc, sc, back_color );

    if( mouseClickHit( &xbox, MS_LEFT) )
        return 1;

    return 0;
}

void drawHorizontalLine( float beginX, float beginY, float width, int z, float sc, int color )
{
    AtlasTex *line = white_tex_atlas;

    display_sprite( white_tex_atlas, beginX, beginY, z, (float)width/line->width, sc/line->height, color );
}

void drawVerticalLine( float beginX, float beginY, float height, int z, float sc, int color )
{
    AtlasTex *line = white_tex_atlas;

    display_sprite( line, beginX, beginY, z, sc/line->width, (float)height/line->height, color );
}

void drawBox( CBox * box, int z, int border_color, int interior_color )
{
    drawOutlinedBox(box, z, border_color, interior_color, 1.f);
}

void drawOutlinedBox( CBox * box, int z, int border_color, int interior_color, float width)
{
    AtlasTex *line = white_tex_atlas;

    drawHorizontalLine( box->lx, box->ly, (box->hx-box->lx)+1, z, width, border_color );
    drawHorizontalLine( box->lx, box->hy - width+1, (box->hx-box->lx)+1, z, width,  border_color );

    drawVerticalLine( box->lx, box->ly + width, (box->hy-box->ly-2*width+1), z, width, border_color );
    drawVerticalLine( box->hx-width+1, box->ly + width, (box->hy-box->ly-2*width+1), z, width, border_color );

    if( interior_color )
        display_sprite( line, box->lx, box->ly, z-1, (box->hx-box->lx)/line->width, (box->hy-box->ly)/line->height, interior_color );
}

void drawBoxEx( CBox * box, int z, int border_color, int interior_color, int above, int right, int below, int left )
{
    AtlasTex *line = white_tex_atlas;

    if( above )
        drawHorizontalLine( box->lx, box->ly, (box->hx-box->lx)+1, z, 1.f, border_color );
    if( below )
        drawHorizontalLine( box->lx, box->hy, (box->hx-box->lx)+1, z, 1.f,  border_color );

    if( left )
        drawVerticalLine( box->lx, box->ly, (box->hy-box->ly), z, 1.f, border_color );
    if( right )
        drawVerticalLine( box->hx, box->ly, (box->hy-box->ly), z, 1.f, border_color );

    if( interior_color )
        display_sprite( line, box->lx, box->ly, z-1, (box->hx-box->lx)/line->width, (box->hy-box->ly)/line->height, interior_color );
}


void drawUiBox( UIBox * box, int z, int border_color )
{
    AtlasTex *line = white_tex_atlas;

    drawHorizontalLine( box->x, box->y, (box->width), z, 1.f, border_color );
    drawHorizontalLine( box->x, box->y, (box->width), z, 1.f, border_color );

    drawVerticalLine( box->x, box->y, (box->height), z, 1.f, border_color );
    drawVerticalLine( box->x, box->y, (box->height), z, 1.f, border_color );
}

//--------------------------------------------------------------------------------------------------------------------
// button ////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------

int drawGenericRotatedButton( AtlasTex * spr, float x, float y, float z, float scale, int color, int over_color, int available, float angle )
{
    CBox box;
     int cl = color, retVal = FALSE;
    float diff = 0;

       if( ABS(angle) == (float)(PI/2) )
           BuildCBox( &box, x - (spr->height-spr->width)/2, y - (spr->width-spr->height)/2, spr->height*scale, spr->width*scale );
    else
           BuildCBox( &box, x, y, spr->width*scale, spr->height*scale );

     if( mouseCollision( &box ) && available )
    {
          cl = over_color;
           scale += .2f;
        diff += -.2f;

        retVal = D_MOUSEOVER;

        if( isDown( MS_LEFT ) )
        {
            scale -= .1f;
            diff += .1f;
        }

        if( mouseLeftDrag( &box ) )
            retVal = D_MOUSEDOWN;
    }

    if( mouseClickHit( &box, MS_LEFT) && available )
        retVal = D_MOUSEHIT;

    display_rotated_sprite( spr, x + diff*spr->width/2, y + diff*spr->height/2, z, scale, scale, cl, angle, 0 );

     return retVal;
}

int drawGenericButton( AtlasTex * spr, float x, float y, float z, float scale, int color, int over_color, int available )
{
    return drawGenericRotatedButton( spr, x, y, z, scale, color, over_color, available, 0 );
}


// same as drawGenericButton() but adds separate X & Y scaling factors
int drawGenericButtonEx( AtlasTex * spr, float x, float y, float z, float scaleX, float scaleY, int color, int over_color, int available )
{
    CBox box;
    int cl = color, retVal = FALSE;
    float diff = 0;

     BuildCBox( &box, x, y, spr->width*scaleX, spr->height*scaleY );

    if( mouseCollision( &box ) && available )
    {
        cl = over_color;
        scaleX += .2f;
        scaleY += .2f;
        diff += -.2f;

        retVal = D_MOUSEOVER;

        if( isDown( MS_LEFT ) )
        {
            scaleX -= .1f;
            scaleY -= .1f;
            diff += .1f;
        }

        if( mouseLeftDrag( &box ) )
            retVal = D_MOUSEDOWN;
    }

    if( mouseClickHit( &box, MS_LEFT) && available )
        retVal = D_MOUSEHIT;

    if(!available)
        cl = 0xffffff88;

    display_sprite( spr, x + diff*spr->width/2, y + diff*spr->height/2, z, scaleX, scaleY, cl );

    return retVal;
}

//------------------------------------------------------------
//  Draw a standard text button.
// param txt: if NULL don't draw anything, just get the scaled dims
//  relative to the passed x and y.
// param dims: if not NULL, set to the dims of the button
//----------------------------------------------------------
int drawTextButton( char *txt, float x, float y, float z, float scale, int color, int over_color,
                    int txt_color, int available, UIBox *dims )
{
    CBox box = {0};
    int cl = color, retVal = FALSE;
    int bar_color = CLR_WHITE;
    float diff = 0;
    float wd = 0.f;
    F32 ht = 0.f; 
    AtlasTex *left, *right, *mid;
       float yscale = scale;
    F32 wd_txt = 0.f;

    left    = atlasLoadTexture("Conningmenu_button_end_L.tga");
    right    = atlasLoadTexture("Conningmenu_button_end_R.tga");
    mid        = atlasLoadTexture("Conningmenu_button_mid.tga");

    if(txt)
    {
        str_dims(&game_9, scale, scale, true, &box, txt );
    }
    else
    {
        box.bottom = ttGetFontHeight(&game_9, scale, scale );
    }
    wd_txt = box.right-box.left;
    wd = wd_txt + (left->width + right->width + 2*PIX2)*scale;
    ht = box.bottom-box.top + 2*PIX3*scale; // (mid->height*scale);// + PIX2)*2*scale;
     BuildCBox( &box, x, y, wd, ht);

    if( mouseCollision( &box ) && available )
    {
        cl = over_color;
        scale += .03f;
        diff += -.2f;

        retVal = D_MOUSEOVER;

        if( isDown( MS_LEFT ) )
        {
            scale -= .03f;
            diff += .1f;
        }

        if( mouseLeftDrag( &box ) )
            retVal = D_MOUSEDOWN;
    }

    if( mouseClickHit( &box, MS_LEFT) && available )
        retVal = D_MOUSEHIT;

    if(!available)
        cl = 0xffffff88;

    if( wd < left->width + right->width + 2*PIX2 )
        wd = left->width + right->width + 2*PIX2;

    if( txt )
    {
        char *tmp;
          int ystart = y + scale*PIX2 + mid->height + (-2);
        int startPt = 0;
        UIBox b = { x, y, wd, ht };
        tmp = textStd( txt );

        // frame
        drawFrame( PIX2, R6, x, y, z+2, wd, mid->height*yscale + 2*scale*PIX2, 1.f, cl, 0x00000000 );

        // left
        display_sprite( left, x, y + scale*PIX2, z, scale, yscale, bar_color );

        // right
        display_sprite( right, x + wd - scale*(right->width), y + scale*PIX2, z, scale, yscale, bar_color );

        // mid
        display_sprite( mid, x + scale*left->width, y + scale*PIX2, z,
            (wd_txt + 2*PIX2*scale)/mid->width, yscale, bar_color );

        font( &game_9 );
        font_color( CLR_WHITE, CLR_WHITE );
        clipperPushRestrict(&b);
        {
            cprnt( x + wd/2, ystart, z+4, scale, scale, &tmp[startPt] );
        }
        clipperPop();
    }

    if( dims )
    {
        ZeroStruct( dims );
        dims->x = x;
        dims->y = y;
        dims->width = wd;
        dims->height = ht;
    }
    // --------------------
    // finally

    return retVal;
}


void drawButton( float x, float y, float z, float wd, float scale, int color, int bar_color, int isDown,
                char* txt, int txt_color)
{
    AtlasTex *left, *right, *mid;
       float yscale = scale;

    ////if(g_tweakui)
    ////{
    //    yscale *= 1.5f;
    ////}

    if( isDown )
    {
        left    = atlasLoadTexture("Conningmenu_button_end_L_press.tga");
        right    = atlasLoadTexture("Conningmenu_button_end_R_press.tga");
        mid        = atlasLoadTexture("Conningmenu_button_mid_press.tga");
    }
    else
    {
        left    = atlasLoadTexture("Conningmenu_button_end_L.tga");
        right    = atlasLoadTexture("Conningmenu_button_end_R.tga");
        mid        = atlasLoadTexture("Conningmenu_button_mid.tga");
    }

    if( wd < left->width + right->width + 2*PIX2 )
        wd = left->width + right->width + 2*PIX2;

    drawFrame( PIX2, R6, x - wd/2, y, z+2, wd, mid->height*yscale + 2*scale*PIX2, 1.f, color, 0x00000000 );

    // left
    display_sprite( left, x - scale*(wd/2), y + scale*PIX2, z, scale, yscale, bar_color );

    // right
    display_sprite( right, x + scale*(wd/2 - right->width), y + scale*PIX2, z, scale, yscale, bar_color );

    // mid
    display_sprite( mid, x - scale*(wd/2) + left->width, y + scale*PIX2, z,
                     scale*(wd - left->width - right->width)/mid->width, yscale, bar_color );

       font( &game_12 );
     if ( txt )
    {
         char *tmp;
          int ystart = y + scale*PIX2 + mid->height + (-2);
        int startPt = 0;

        tmp = textStd( txt );
         while( str_wd(&game_12, scale, scale, &tmp[startPt]) > (scale*(wd - left->width - right->width)) && (UINT)startPt < strlen(tmp) )
        {
            startPt++;
        }

        font_color( txt_color, txt_color );
        cprnt( x, ystart, z+4, scale, scale, &tmp[startPt] );
    }

}

//--------------------------------------------------------------------------------------------------------------------
// capsule ///////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------

void drawCapsuleFrame( float x, float y, float z, float scale, float wd, int color, int back_color )
{
    AtlasTex *left    = atlasLoadTexture("healthbar_framemain_L.tga");
    AtlasTex *right    = atlasLoadTexture("healthbar_framemain_R.tga");
    AtlasTex *pipe    = atlasLoadTexture("healthbar_framemain_horiz.tga");
    AtlasTex *back    = white_tex_atlas;

    if( wd < (left->width + right->width)*scale ) // bound check
        wd = (left->width + right->width)*scale;

    // left bracket
      display_sprite( left, x, y, z, scale, scale, color );

    // right bracket
    display_sprite( right, x + wd - right->width*scale, y, z, scale, scale, color );

    // top pipe
    display_sprite( pipe, x + left->width*scale, y + 2*scale, z,
          (wd - scale*(left->width + right->width))/pipe->width, scale, color );
    // bottom pipe
    display_sprite( pipe, x + left->width*scale, y - 2*scale + (left->height - pipe->height)*scale, z,
        (wd - scale*(left->width + right->width))/pipe->width, scale, color );

    // background
    display_sprite( back, x + (2 + pipe->width)*scale, y + (pipe->height + 2)*scale, z-2,
        (wd - scale*4*pipe->width)/back->width, scale*(left->height - 4*pipe->height)/back->height, back_color );
}

void drawSpinningCircle( F32 radius, const Mat4 mat, int color )
{
    Vec3 corners[100];
    int i;

    Vec3 pyr ={ 0, 0, 0 };
    const int count = min(ARRAY_SIZE(corners), max(radius, 12));

    for(i = 0; i < count; i++)
    {
        corners[i][0] = radius * sin(RAD(360) * i / count + RAD(global_state.global_frame_count * 1));
        corners[i][1] = 0;
        corners[i][2] = radius * cos(RAD(360) * i / count + RAD(global_state.global_frame_count * 1));
    }

    for(i = 0; i < count; i++)
    {
        Vec3 p1, p2, p3, p4;

        //low circle
        copyVec3(corners[i],            p1);
        copyVec3(corners[(i+1)%count],    p2);
        p1[1] += 0;
        p2[1] += 0;
        mulVecMat4(p1, mat, p3);
        mulVecMat4(p2, mat, p4);
        drawLine3D(p3, p4, (0x60 << 24) | color);
    }
}

//Draw an actual capsule
void drawSpinningCapsule( F32 radius, F32 length, const Mat4 collMat, int color )
{
    Vec3 corners[100];
    int i;

    Vec3 pyr ={ 0, 0, 0 };
    const int count = min(ARRAY_SIZE(corners), max(radius, 12));

    for(i = 0; i < count; i++){
        corners[i][0] = radius * sin(RAD(360) * i / count + RAD(global_state.global_frame_count * 1));
        corners[i][1] = 0;
        corners[i][2] = radius * cos(RAD(360) * i / count + RAD(global_state.global_frame_count * 1));
    }

    for(i = 0; i < count; i++){
        Vec3 p1, p2, p3, p4;
        Vec3 lastPos;
        int j;

        //low circle
        copyVec3(corners[i],            p1);
        copyVec3(corners[(i+1)%count],    p2);
        p1[1] += 0;
        p2[1] += 0;
        mulVecMat4(p1, collMat, p3);
        mulVecMat4(p2, collMat, p4);
        drawLine3D(p3, p4, (0x60 << 24) | color);

        //high circle
        copyVec3(corners[i],            p1);
        copyVec3(corners[(i+1)%count],    p2);
        p1[1] += length;
        p2[1] += length;
        mulVecMat4(p1, collMat, p3);
        mulVecMat4(p2, collMat, p4);
        drawLine3D(p3, p4, (0x60 << 24) | color);

        //connector lines
        copyVec3(corners[i], p1);
        copyVec3(corners[i], p2);
        p1[1] += 0;
        p2[1] += length;
        mulVecMat4(p1, collMat, p3);
        mulVecMat4(p2,    collMat, p4);

        drawLine3D(p3, p4, (0xff << 24) | color);

        //bottom tip
        copyVec3(corners[i], p1);
        p2[0] = 0;
        p2[1] = radius * -1;
        p2[2] = 0;

        for(j = 0; j < 10; j++){
            Vec3 pos;
            Vec3 temp;

            scaleVec3(p1, fcos(RAD(j*10)), pos);
            scaleVec3(p2, fsin(RAD(j*10)), temp);
            addVec3(pos, temp, p3);

            mulVecMat4(p3, collMat, pos);

            if(j){
                drawLine3D(lastPos, pos, (0xff << 24) | color);
            }

            copyVec3(pos, lastPos);
        }

        //top tip
        copyVec3(corners[i], p1);
        p2[0] = 0;
        p2[1] = radius;
        p2[2] = 0;

        for(j = 0; j < 10; j++){
            Vec3 pos;
            Vec3 temp;

            scaleVec3(p1, fcos(RAD(j*10)), pos);
            scaleVec3(p2, fsin(RAD(j*10)), temp);
            addVec3(pos, temp, p3);

            p3[1] += length;

            mulVecMat4(p3, collMat, pos);

            if(j){
                drawLine3D(lastPos, pos, (0xff << 24) | color);
            }

            copyVec3(pos, lastPos);
        }

        //mulVecMat4(p1, collMat, p3);
        //mulVecMat4(p2, collMat, p4);

        //drawLine3D(p3, p4, 0xffff8855);
    }
}

#define CAPSULE_SLIDER_SPEED .01

void drawCapsule( float x, float y, float z, float wd, float scale, int color, int back_color, int bar_color,
                 float curr, float total, CapsuleData * data )
{
    drawCapsuleEx(x,y,z,wd,scale,color,back_color,CLR_WHITE,bar_color,curr,total,data);
}

void drawCapsuleEx( float x, float y, float z, float wd, float scale, int color, int back_color, int shine_color, int bar_color,
                 float curr, float total, CapsuleData * data )
{
    char txt[10];
    float sc, percent;
     AtlasTex *bar    = atlasLoadTexture( "healthbar_slider.tga" );
    AtlasTex *shine    = atlasLoadTexture( "healthbar_sliderhighlight.tga" );
    AtlasTex *glow    = atlasLoadTexture( "healthbar_glow.tga" );
    AtlasTex *cap    = atlasLoadTexture( "healthbar_slider_cap.tga" );

     if( wd <= cap->width*2*scale )
        return;

    // draw the frame
       drawCapsuleFrame( x, y, z+2, scale, wd, color, back_color );

    // draw the slider bar
    percent = curr / total;

    if( data->instant )
        data->curr = percent;
    else
    {
        // so slider bar won't pop
        if( percent > data->curr )
            data->curr = MIN(data->curr + TIMESTEP*CAPSULE_SLIDER_SPEED, percent);    // Do not exceed "percent"
        if ( percent < data->curr )
            data->curr = MAX(data->curr - TIMESTEP*CAPSULE_SLIDER_SPEED, percent);    // Do not dip below "percent"
    }

    // The first time the progressbar is drawn, skip interpolation of curr value.
    if( !data->drawn )
    {
        data->curr = 1.0;
        data->drawn = 1;
    }

    //data->curr = sc;

    sc = (data->curr*(wd-(8+cap->width)*scale))/bar->width;

    if( sc >= 0 )
    {
         display_sprite( bar,   x + 4*scale, y + 4*scale, z,   (data->curr*(wd-(8+cap->width)*scale))/bar->width,   scale, bar_color );
          display_sprite( shine, x + 4*scale, y + 4*scale, z+1, (data->curr*(wd-(8)*scale))/shine->width, scale, shine_color );
         display_sprite( cap,   x + 4*scale + (data->curr*(wd-(8+cap->width)*scale)), y + 4*scale, z, scale, scale, bar_color );
    }
    else if ( data->curr > 0.0f )
    {
        float cross_sc = cap->width*scale/(wd-8*scale);
        sc /= cross_sc;
        display_sprite( cap, x + 4*scale, y + 4*scale, z, scale*sc, scale*sc, bar_color );
    }

    // draw the numbers
    if( data->showNumbers )
    {
        CBox txtBox = {0};
        font( &game_9 );
        font_color(    data->txt_clr1, data->txt_clr2 );

        sprintf( txt, "%i/%i", (int)(data->curr), (int)total );
        str_dims( &game_9, scale, scale, FALSE, &txtBox, txt );
        prnt( x + wd - (txtBox.hx-txtBox.lx) - 6*scale, y + (txtBox.hy-txtBox.ly)+2*scale, z+2, scale*.9, scale*.9, txt );
    }

    // draw the text
     if( data->showText )
    {
        CBox txtBox = {0};
        font( &game_9 );
        font_color( data->txt_clr1, data->txt_clr2 );
         cprnt( x + wd/2, y + (bar->height+3)*scale, z+2, scale*.9, scale*.9, smf_DecodeAllCharactersGet(data->txt) );
    }
}

//--------------------------------------------------------------------------------------------------------------------
// Connecting Line //////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------

void drawConnectingLine(int size, int radius, float x1, float y1, float x2, float y2, float z, int upFromBelowPoint, float sc, int color)
{
    drawConnectingLineStyle(size, radius, x1, y1, x2, y2, z, upFromBelowPoint, sc, color, kFrameStyle_3D);
}

//--------------------------------------------------------------------------------------------------------------------
// Frame ////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------

void drawFrameBox( int size, int radius, CBox * box, float z, int color, int back_color )
{
    drawFrame( size, radius, box->lx, box->ly, z, box->hx-box->lx, box->hy-box->ly, 1.f, color, back_color );
}

void drawFlatFrameBox( int size, int radius, CBox * box, float z, int color, int back_color )
{
     drawFlatFrame( size, radius, box->lx, box->ly, z, box->hx-box->lx, box->hy-box->ly, 1.f, color, back_color );
}

void drawFrame( int size, int radius, float x, float y, float z, float wd, float ht, float sc, int color, int back_color )
{
    drawFrameStyle(size, radius, x, y, z, wd, ht, sc, color, back_color, kFrameStyle_3D);
}

void drawFlatFrame( int size, int radius, float x, float y, float z, float wd, float ht, float sc, int color, int back_color )
{
    drawFrameStyle(size, radius, x, y, z, wd, ht, sc, color, back_color, kFrameStyle_Flat);
}


strcati(char *pch, int i)
{
    char buf[64];
    itoa(i, buf, 10);
    strcat(pch, buf);
}

void MakeBaseFrameTexName(char *pch, char *pchStyle, int size, int radius)
{
    strcpy(pch, "Frame_");
    strcat(pch, pchStyle);
    strcati(pch, size);
    strcat(pch, "px_");
    if(radius>0)
    {
        strcati(pch, radius);
        strcat(pch, "r_");
    }
}

void MakeFrameTexName(char *pch, char *pchCorner, int start)
{
    strcpy(pch+start, pchCorner);
    strcat(pch, ".tga");
}

typedef struct FrameSet
{
    FrameStyle style;
    int size;
    int radius;
    TabDir dir;
    AtlasTex *ul;
    AtlasTex *ll;
    AtlasTex *lr;
    AtlasTex *ur;
    AtlasTex *ypipe;
    AtlasTex *xpipe;
    AtlasTex *bul;
    AtlasTex *bll;
    AtlasTex *blr;
    AtlasTex *bur;
} FrameSet;
static FrameSet **s_ppFrameSets = NULL;

static FrameSet * getFrameSet( FrameStyle style, int size, int radius, TabDir dir )
{
    FrameSet * pFrameSet = 0;

    if(!s_ppFrameSets)
        eaCreate(&s_ppFrameSets);
    else
    {
        int n = eaSize(&s_ppFrameSets);
        int i;
        for(i=0; i<n; i++)
        {
            FrameSet *pTmpFrameSet = s_ppFrameSets[i];
            if(pTmpFrameSet->style == style && pTmpFrameSet->size == size && pTmpFrameSet->radius == radius && pTmpFrameSet->dir == dir)
            {
                pFrameSet = pTmpFrameSet;
                break;
            }
        }
    }

    if(!pFrameSet)
    {
        char temp[128];
        char *pchStyle;
        int start;

        pFrameSet = (FrameSet *)calloc(1, sizeof(FrameSet));
        eaPush(&s_ppFrameSets, pFrameSet);

        pFrameSet->style = style;
        pFrameSet->size = size;
        pFrameSet->radius = radius;
        pFrameSet->dir = dir;

        switch(style)
        {
        case kFrameStyle_3D:
        default:
            pchStyle = "";
            break;
        case kFrameStyle_Flat:
            pchStyle = "Flat_";
            break;
        }

        MakeBaseFrameTexName(temp, pchStyle, size, radius);
        start = strlen(temp);

        // load the proper corner textures
        if ( dir == kTabDir_None || dir == kTabDir_Bottom || dir == kTabDir_Right )
        {
            MakeFrameTexName(temp, "UL", start);
            pFrameSet->ul = atlasLoadTexture( temp );
        }
        else
        {
            MakeFrameTexName(temp, "UR", start);
            pFrameSet->ul = atlasLoadTexture( temp );
        }

        if ( dir == kTabDir_None || dir == kTabDir_Top|| dir == kTabDir_Right )
        {
            MakeFrameTexName(temp, "LL", start);
            pFrameSet->ll = atlasLoadTexture( temp ); 
        }
        else
        {
            MakeFrameTexName(temp, "LR", start);
            pFrameSet->ll = atlasLoadTexture( temp );
        }

        if ( dir == kTabDir_None || dir == kTabDir_Top || dir == kTabDir_Left )
        {
            MakeFrameTexName(temp, "LR", start);
            pFrameSet->lr = atlasLoadTexture( temp );
        }
        else
        {
            MakeFrameTexName(temp, "LL", start);
            pFrameSet->lr = atlasLoadTexture( temp );
        }

        if ( dir == kTabDir_None || dir == kTabDir_Bottom || dir == kTabDir_Left )
        {
            MakeFrameTexName(temp, "UR", start);
            pFrameSet->ur = atlasLoadTexture( temp );
        }
        else
        {
            MakeFrameTexName(temp, "UL", start);
            pFrameSet->ur = atlasLoadTexture( temp );
        }

        // bg textures
        MakeBaseFrameTexName(temp, "", size, radius);
        start = strlen(temp);

        if ( dir == kTabDir_None || dir == kTabDir_Bottom || dir == kTabDir_Right )
        {
            MakeFrameTexName(temp, "Background_UL", start);
            pFrameSet->bul = atlasLoadTexture( temp );
        }
        else
        {
            MakeFrameTexName(temp, "Background_UR_INV", start);
            pFrameSet->bul = atlasLoadTexture( temp );
        }

        if ( dir == kTabDir_None || dir == kTabDir_Top|| dir == kTabDir_Right )
        {
            MakeFrameTexName(temp, "Background_LL", start);
            pFrameSet->bll = atlasLoadTexture( temp );
        }
        else
        {
            MakeFrameTexName(temp, "Background_LR_INV", start);
            pFrameSet->bll = atlasLoadTexture( temp );
        }

        if ( dir == kTabDir_None || dir == kTabDir_Top || dir == kTabDir_Left )
        {
            MakeFrameTexName(temp, "Background_LR", start);
            pFrameSet->blr = atlasLoadTexture( temp );
        }
        else
        {
            MakeFrameTexName(temp, "Background_LL_INV", start);
            pFrameSet->blr = atlasLoadTexture( temp );
        }

        if ( dir == kTabDir_None || dir == kTabDir_Bottom || dir == kTabDir_Left )
        {
            MakeFrameTexName(temp, "Background_UR", start);
            pFrameSet->bur = atlasLoadTexture( temp );
        }
        else
        {
            MakeFrameTexName(temp, "Background_UL_INV", start);
            pFrameSet->bur = atlasLoadTexture( temp );
        }

        // Edge textures
        MakeBaseFrameTexName(temp, pchStyle, size, 0);
        start = strlen(temp);

        MakeFrameTexName(temp, "vert", start);
        pFrameSet->ypipe = atlasLoadTexture( temp );

        MakeFrameTexName(temp, "horiz", start);
        pFrameSet->xpipe = atlasLoadTexture( temp );

    }

    return pFrameSet;
}

float drawConnectingLineStyle(int size, int radius, float x1, float y1, float x2, float y2, float z, int upFromBelowPoint, float sc, int color, FrameStyle style)
{
    FrameSet *pFrameSet = getFrameSet( style, size, radius, kTabDir_None );
    float cornerWidth;
    float wd, ht;
    float xsc = sc, ysc = sc;
    float xAbove, yAbove, xBelow, yBelow;
    int offset = 0;
    AtlasTex * back = white_tex_atlas;
    float screenScaleX, screenScaleY;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);

    x1 *= screenScaleX;
    y1 *= screenScaleY;
    x2 *= screenScaleX;
    y2 *= screenScaleY;

    wd = (x1 - x2);
    if (wd < 0)    wd *= -1;
    ht = (y1 - y2);
    if (ht < 0)    ht *= -1;
    if (wd && ht)
    {
        cornerWidth = pFrameSet->ul->width;
    }
    else
    {
        cornerWidth = 0;
    }

    if ( wd < cornerWidth*2*sc )
        xsc = (float)wd/(cornerWidth*2); //wd = w*2;
    if ( ht < cornerWidth*2*sc )
        ysc = (float)ht/(cornerWidth*2); //ht = w*2;

    xsc = ysc = MIN(xsc, ysc);
    if (y1 < y2)
    {
        xAbove = x1;
        yAbove = y1;
        xBelow = x2;
        yBelow = y2;
    }
    else
    {
        xAbove = x2;
        yAbove = y2;
        xBelow = x1;
        yBelow = y1;
    }

    if (upFromBelowPoint)
    {
        if (xAbove > xBelow)
        {
            xBelow -= size*sc*0.5f;
            yAbove -= size*sc*0.5f;

            // top left corner
            if (cornerWidth)
            {
                display_sprite_menu(pFrameSet->ul, xBelow, yAbove, z, xsc, ysc, color);
            }
            if (wd)            display_sprite_menu(pFrameSet->xpipe, xBelow + cornerWidth*xsc, yAbove, z, ((xAbove-xBelow) - cornerWidth*xsc)/pFrameSet->xpipe->width, ysc, color);
            if (ht)            display_sprite_menu(pFrameSet->ypipe, xBelow, yAbove + cornerWidth*ysc, z, xsc, ((yBelow-yAbove) - cornerWidth*ysc)/pFrameSet->ypipe->height, color);
        }
        else
        {
            xAbove += size*sc*0.5f;
            yAbove -= size*sc*0.5f;

            // top right corner
            if (cornerWidth)
            {
                display_sprite_menu(pFrameSet->ur, xBelow - cornerWidth*xsc, yAbove, z, xsc, ysc, color);
            }
            if (wd)            display_sprite_menu(pFrameSet->xpipe, xAbove, yAbove, z, ((xBelow-xAbove) - cornerWidth*xsc)/pFrameSet->xpipe->width, ysc, color);
            if (ht)            display_sprite_menu(pFrameSet->ypipe, xBelow - size*xsc, yAbove + cornerWidth*ysc, z, xsc, ((yBelow-yAbove) - cornerWidth*ysc)/pFrameSet->ypipe->height, color);
        }
    }
    else
    {
        if (xAbove > xBelow)
        {
            xAbove += size*sc*0.5f;
            yBelow += size*sc*0.5f;
            // bottom right corner
            if (cornerWidth)
            {
                display_sprite_menu(pFrameSet->lr, xAbove - cornerWidth*xsc, yBelow - cornerWidth*ysc, z, xsc, ysc, color);
            }
            if (wd)            display_sprite_menu(pFrameSet->xpipe, xBelow, yBelow - size*ysc, z, ((xAbove-xBelow) - cornerWidth*xsc)/pFrameSet->xpipe->width, ysc, color);
            if (ht)            display_sprite_menu(pFrameSet->ypipe, xAbove - size*xsc, yAbove, z, xsc, ((yBelow-yAbove) - cornerWidth*ysc)/pFrameSet->ypipe->height, color);
        }
        else
        {
            xAbove -= size*sc*0.5f;
            yBelow += size*sc*0.5f;

            // bottom left corner
            if (cornerWidth)
            {
                display_sprite_menu(pFrameSet->ll, xAbove, yBelow - cornerWidth*ysc, z, xsc, ysc, color);
            }
            if (wd)            display_sprite_menu(pFrameSet->xpipe, xAbove + cornerWidth*xsc, yBelow - size*ysc, z, ((xBelow-xAbove) - cornerWidth*xsc)/pFrameSet->xpipe->width, ysc, color);
            if (ht)            display_sprite_menu(pFrameSet->ypipe, xAbove, yAbove, z, xsc, ((yBelow-yAbove) - cornerWidth*ysc)/pFrameSet->ypipe->height, color);
        }
    }
    return cornerWidth*xsc;
}

void drawFrameStyle( int size, int radius, float x, float y, float z, float wd, float ht, float sc, int color, int back_color, FrameStyle style )
{
    FrameSet *pFrameSet = getFrameSet( style, size, radius, kTabDir_None );
    float w;
    float xsc = sc, ysc = sc;
    int offset = 0;
    AtlasTex * back = white_tex_atlas;
    float screenScaleX, screenScaleY;

    calc_scrn_scaling(&screenScaleX, &screenScaleY);

    x *= screenScaleX;
    y *= screenScaleY;
    wd *= screenScaleX;
    ht *= screenScaleY;

    w = pFrameSet->ul->width;
    if ( wd < w*2*sc )
        xsc = (float)wd/(w*2); //wd = w*2;
    if ( ht < w*2*sc )
        ysc = (float)ht/(w*2); //ht = w*2;

    // middle
    display_sprite_menu( back, x + w*xsc, y + w*ysc, z, (wd - 2*w*xsc)/back->width, (ht - 2*w*ysc)/back->height, back_color);

     // top left
    display_sprite_menu( pFrameSet->bul, x - offset, y - offset, z, xsc, ysc, back_color);
    display_sprite_menu( pFrameSet->ul, x, y, z, xsc, ysc, color );

    // top
    display_sprite_menu( back , x + w*xsc, y + size*ysc, z, (wd - 2*w*xsc)/back->width, (w - size)*ysc/back->height, back_color);
    display_sprite_menu( pFrameSet->xpipe, x + w*xsc, y, z, (wd - 2*w*xsc)/pFrameSet->xpipe->width, ysc, color );

    // top right
    display_sprite_menu( pFrameSet->bur, x + wd - w*xsc, y - offset, z, xsc, ysc, back_color);
    display_sprite_menu( pFrameSet->ur,  x + wd - w*xsc, y, z, xsc, ysc, color );

    // right
    display_sprite_menu( back,  x + wd - w*xsc, y + w*ysc, z, (w-size)*xsc/back->width, (ht - 2*w*ysc)/back->height, back_color);
    display_sprite_menu( pFrameSet->ypipe, x + wd - size*xsc, y + w*ysc, z, xsc, (ht - 2*w*ysc)/pFrameSet->ypipe->height, color );

    // lower right
    display_sprite_menu( pFrameSet->blr, x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, back_color);
    display_sprite_menu( pFrameSet->lr,  x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, color );

    // bottom
    display_sprite_menu( back, x + w*xsc, y + ht - w*ysc, z, (wd - 2*w*xsc)/back->width, (w - size)*ysc/back->height, back_color);
    display_sprite_menu( pFrameSet->xpipe, x + w*xsc, y + ht - size*ysc, z, (wd - 2*w*xsc)/pFrameSet->xpipe->width, ysc, color );

    // lower left
    display_sprite_menu( pFrameSet->bll, x - offset, y + ht - w*ysc, z, xsc, ysc, back_color);
    display_sprite_menu( pFrameSet->ll,  x, y + ht - w*ysc, z, xsc, ysc, color );

    // left
    display_sprite_menu( back, x + size*xsc, y + w*ysc, z, (w - size)*xsc/back->width, (ht - 2*w*ysc)/back->height, back_color);
    display_sprite_menu( pFrameSet->ypipe, x, y + w*ysc, z, xsc, (ht - 2*w*ysc)/pFrameSet->ypipe->height, color );
}

void drawFrameStyleColor( int size, int radius, float x, float y, float z, float wd, float ht, float sc, FrameColors fcolors, FrameStyle style, int sectionFlags )
{
    FrameSet *pFrameSet = getFrameSet( style, size, radius, kTabDir_None );
    float w;
    float xsc = sc, ysc = sc;
    int offset = 0;
    AtlasTex * back = white_tex_atlas;
    int lExist = !!(sectionFlags & (kFramePart_UpL | kFramePart_MidL | kFramePart_BotL)),
        rExist = !!(sectionFlags & (kFramePart_UpR | kFramePart_MidR | kFramePart_BotR)),
        tExist = !!(sectionFlags & (kFramePart_UpL | kFramePart_UpC | kFramePart_UpR)),
        bExist = !!(sectionFlags & (kFramePart_BotL | kFramePart_BotC | kFramePart_BotR));
    int sidesLR = lExist + rExist;
    int sidesTB = tExist + bExist;
    //the following are the edges of the center block
    float cleft,cright,ctop,cbottom;

    w = pFrameSet->ul->width;
    if ( wd < w*sidesLR*sc  && sidesLR)
        xsc = (float)wd/(w*2); //wd = w*2;
    if ( ht < w*2*sc && sidesTB)
        ysc = (float)ht/(w*2); //ht = w*2;

    cleft = x + (lExist?w*xsc:0);
    cright = x + wd - (rExist?w*xsc:0);
    ctop = y + (tExist?w*ysc:0);
    cbottom = y + ht - (bExist?w*ysc:0);

    // middle
    if(sectionFlags &kFramePart_MidC)
    {
        if( style&kFrameStyle_GradientH && style&kFrameStyle_GradientV )
            display_sprite_blend( back, cleft, ctop, z, (wd - sidesLR*w*xsc)/back->width, (ht - sidesTB*w*ysc)/back->height, fcolors.topLeft, fcolors.topRight, fcolors.botRight, fcolors.botLeft );
        else if( style&kFrameStyle_GradientH )
        {
            display_sprite_blend( back, cleft, ctop, z, (wd/2 - w*xsc)/(back->width), (ht - sidesTB*w*ysc)/back->height, fcolors.midLeft, fcolors.midCenter, fcolors.midCenter, fcolors.midLeft );
               display_sprite_blend( back, x + wd/2, ctop, z, (wd/2 - w*xsc)/(back->width), (ht - sidesTB*w*ysc)/back->height, fcolors.midCenter, fcolors.midRight, fcolors.midRight, fcolors.midCenter );
        }
        else if( style&kFrameStyle_GradientV )
        {
            display_sprite_blend( back, cleft, ctop, z, (wd - sidesLR*w*xsc)/back->width, (ht/2 - w*ysc)/(back->height), fcolors.topCenter, fcolors.topCenter, fcolors.midCenter, fcolors.midCenter );
            display_sprite_blend( back, cleft, y + ht/2, z, (wd - sidesLR*w*xsc)/back->width, (ht/2 - w*ysc)/(back->height), fcolors.midCenter, fcolors.midCenter, fcolors.botCenter, fcolors.botCenter );
        }
        else
            display_sprite( back, cleft, ctop, z, (wd - sidesLR*w*xsc)/back->width, (ht - sidesTB*w*ysc)/back->height, fcolors.midCenter);
    }

    // top left
    if(sectionFlags &kFramePart_UpL)
    {
        display_sprite( pFrameSet->bul, x - offset, y - offset, z, xsc, ysc, fcolors.topLeft);
        display_sprite( pFrameSet->ul, x, y, z, xsc, ysc, fcolors.border?fcolors.border:fcolors.topLeft );
    }

    // top
    if(sectionFlags &kFramePart_UpC)
    {
        if( style&kFrameStyle_GradientH )
        {
            display_sprite_blend( back , cleft, y + size*ysc, z, (wd/2 - w*xsc)/(back->width), (w - size)*ysc/back->height,fcolors.topLeft, fcolors.topCenter, fcolors.topCenter, fcolors.topLeft );
            display_sprite_blend( back , x + wd/2, y+ size*ysc, z, (wd/2 - w*xsc)/(back->width), (w - size)*ysc/back->height,fcolors.topCenter, fcolors.topRight, fcolors.topRight, fcolors.topCenter );
        }
        else
            display_sprite( back , cleft, y + size*ysc, z, (wd - sidesLR*w*xsc)/back->width, (w - size)*ysc/back->height, fcolors.topCenter);

        display_sprite( pFrameSet->xpipe, cleft, y, z, (wd - sidesLR*w*xsc)/pFrameSet->xpipe->width, ysc, fcolors.border?fcolors.border:fcolors.topCenter );
    }

    // top right
    if(sectionFlags &kFramePart_UpR)
    {
        display_sprite( pFrameSet->bur, cright, y - offset, z, xsc, ysc, fcolors.topRight);
        display_sprite( pFrameSet->ur, cright, y, z, xsc, ysc, fcolors.border?fcolors.border:fcolors.topRight );
    }

    // right
    if(sectionFlags &kFramePart_MidR)
    {
        if( style&kFrameStyle_GradientV )
        {
            display_sprite_blend( back, cright, ctop, z, (w-size)*xsc/back->width, (ht/2 - w*ysc)/(back->height), fcolors.topRight, fcolors.topRight, fcolors.midRight, fcolors.midRight );
            display_sprite_blend( back, cright, y+ht/2, z, (w-size)*xsc/back->width, (ht/2 - w*ysc)/(back->height), fcolors.midRight, fcolors.midRight, fcolors.botRight, fcolors.botRight );
        }
        else
            display_sprite( back, cright, ctop, z, (w-size)*xsc/back->width, (ht - sidesTB*w*ysc)/back->height, fcolors.midRight);

        display_sprite( pFrameSet->ypipe, cright+(w-size)*xsc, ctop, z, xsc, (ht - sidesTB*w*ysc)/pFrameSet->ypipe->height, fcolors.border?fcolors.border:fcolors.midRight );
    }

    // lower right
     if(sectionFlags &kFramePart_BotR)
    {
        display_sprite( pFrameSet->blr, cright, cbottom, z, xsc, ysc, fcolors.botRight);
        display_sprite( pFrameSet->lr, cright, cbottom, z, xsc, ysc, fcolors.border?fcolors.border:fcolors.botRight);
    }

    // bottom
    if(sectionFlags &kFramePart_BotC)
    {
        if( style&kFrameStyle_GradientH )
        {
            display_sprite_blend( back, cleft, cbottom, z, (wd/2 - w*xsc)/(back->width), (w - size)*ysc/back->height, fcolors.botLeft, fcolors.botCenter, fcolors.botCenter, fcolors.botLeft );
            display_sprite_blend( back, x + wd/2, cbottom, z, (wd/2 - w*xsc)/(back->width), (w - size)*ysc/back->height, fcolors.botCenter, fcolors.botRight, fcolors.botRight, fcolors.botCenter );
        }
        else
            display_sprite( back, cleft, cbottom, z, (wd - sidesLR*w*xsc)/back->width, (w - size)*ysc/back->height, fcolors.botCenter);
        display_sprite( pFrameSet->xpipe, cleft, cbottom+(w-size)*ysc, z, (wd - sidesLR*w*xsc)/pFrameSet->xpipe->width, ysc, fcolors.border?fcolors.border:fcolors.botCenter);
    }

    // lower left
    if(sectionFlags &kFramePart_BotL)
    {
        display_sprite( pFrameSet->bll, x - offset, cbottom, z, xsc, ysc, fcolors.botLeft);
        display_sprite( pFrameSet->ll,  x, cbottom, z, xsc, ysc, fcolors.border?fcolors.border:fcolors.botLeft );
    }

    // left
    if(sectionFlags &kFramePart_MidL)
    {
        if( style&kFrameStyle_GradientV )
        {
            display_sprite_blend( back, x + size*xsc, ctop, z, (w - size)*xsc/back->width, (ht/2 - w*ysc)/(2*back->height), fcolors.topLeft, fcolors.topLeft, fcolors.midLeft, fcolors.midLeft);
            display_sprite_blend( back, x + size*xsc, y+ht/2, z, (w - size)*xsc/back->width, (ht/2 - w*ysc)/(2*back->height), fcolors.midLeft, fcolors.midLeft, fcolors.botLeft, fcolors.botLeft);
        }
        else
            display_sprite( back, x + size*xsc, ctop, z, (w - size)*xsc/back->width, (ht - sidesTB*w*ysc)/back->height, fcolors.midLeft);
        display_sprite( pFrameSet->ypipe, x, ctop, z, xsc, (ht - sidesTB*w*ysc)/pFrameSet->ypipe->height, fcolors.border?fcolors.border:fcolors.midLeft );
    }
}
void drawFlatThreeToneFrame( int size, int radius, float x, float y, float z, float wd, float ht, float sc, int colorLeft, int colorCenter, int colorRight )
{
    FrameColors colors = {colorLeft, colorCenter, colorRight,
                            colorLeft, colorCenter, colorRight,
                            colorLeft, colorCenter, colorRight, 0};
    //int lcr[] = {colorLeft, colorCenter, colorRight;
    //int i, j;

    /*for(i = 0; i<3; i++)
    {
        for(j = 0; j<3;j++)
        {
            colors[i*3+j] = lcr[j];
        }
    }*/
    drawFrameStyleColor( size, radius, x, y, z, wd, ht, sc, colors, kFrameStyle_Flat,FRAMEPARTS_ALL);
}

void drawFlatSectionFrame( int size, int radius, float x, float y, float z, float wd, float ht, float sc, int color, int sectionFlags )
{
    FrameColors colors = {color,color,color,color,color,color,color,color,color, 0};
    drawFrameStyleColor( size, radius, x, y, z, wd, ht, sc, colors, kFrameStyle_Flat,sectionFlags );
}

void drawSectionFrame( int size, int radius, float x, float y, float z, float wd, float ht, float sc, int color, int back_color, int sectionFlags )
{
    FrameColors colors = {back_color,back_color,back_color,back_color,back_color,back_color,back_color,back_color,back_color, color};
    drawFrameStyleColor( size, radius, x, y, z, wd, ht, sc, colors, kFrameStyle_3D, sectionFlags );
}

void drawFlatMultiToneFrame( int size, int radius, float x, float y, float z, float wd, float ht, float sc, FrameColors colors )
{
    drawFrameStyleColor( size, radius, x, y, z, wd, ht, sc, colors, kFrameStyle_Flat,FRAMEPARTS_ALL );
}

void drawTabbedFlatFrameBox( int size, int radius, CBox * box, float z, float sc, int color, int back_color, TabDir dir )
{
    drawTabbedFrameStyle( size, radius, box->lx, box->ly, z, box->hx-box->lx, box->hy-box->ly, sc, color, back_color, kFrameStyle_Flat, dir);
}

void drawTabbedFrameStyle( int size, int radius, float x, float y, float z, float wd, float ht, float sc, int color, int back_color, FrameStyle style, TabDir dir )
{
    FrameSet *pFrameSet = getFrameSet( style, size, radius, dir );
    float w, h;
    float xsc = sc, ysc = sc;
    int offset = 0;
    AtlasTex * back = white_tex_atlas;

    w = pFrameSet->ul->width;
    h = pFrameSet->ul->height;
    if ( wd < w*2*sc )
        xsc = (float)wd/(w*2); //wd = w*2;
    if ( ht < w*2*sc )
        ysc = (float)ht/(w*2); //ht = w*2;

    // middle
    display_sprite( back, x + w*xsc, y + h*ysc, z, (wd - 2*w*xsc)/back->width, (ht - 2*w*ysc)/back->height, back_color);

    if ( dir == kTabDir_Top )
    {
        display_sprite( back, x + size*sc, y, z, (wd - 2*size*sc)/back->width, h*sc/back->height, back_color);
    }
    else if ( dir == kTabDir_Left )
    {
    }
    else if ( dir == kTabDir_Bottom )
    {
    }
    else if ( dir == kTabDir_Right )
    {
    }

    // top left
     if ( dir == kTabDir_Top )
    {
        display_sprite( pFrameSet->bul, x - (offset + w)*sc, y - offset, z, xsc, ysc, back_color);
        //display_sprite( pFrameSet->ul, x - w + size, y, z, xsc, ysc, color );
    }
    else if ( dir == kTabDir_Left )
    {
    }
    else
    {
        display_sprite( pFrameSet->bul, x - offset, y - offset, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ul, x, y, z, xsc, ysc, color );
    }

    // top right
    if ( dir == kTabDir_Top )
    {
        display_sprite( pFrameSet->bur, x + wd, y - offset, z, xsc, ysc, back_color);
        //display_sprite( pFrameSet->ur,  x + wd - size, y, z, xsc, ysc, color );
    }
    else if ( dir == kTabDir_Right )
    {
    }
    else
    {
        display_sprite( pFrameSet->bur, x + wd - w*xsc, y - offset, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ur,  x + wd - w*xsc, y, z, xsc, ysc, color );
    }

    // lower right
    if ( dir == kTabDir_Bottom )
    {
    }
    else if ( dir == kTabDir_Right )
    {
    }
    else
    {
        display_sprite( pFrameSet->blr, x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->lr,  x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, color );
    }

    // lower left
    if ( dir == kTabDir_Bottom )
    {
    }
    else if ( dir == kTabDir_Left )
    {
    }
    else
    {
        display_sprite( pFrameSet->bll, x - offset, y + ht - w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ll,  x, y + ht - w*ysc, z, xsc, ysc, color );
    }


    // top
    if ( dir != kTabDir_Top )
    {
        display_sprite( back , x + w*xsc, y + size*ysc, z, (wd - 2*w*xsc)/back->width, (w - size)*ysc/back->height, back_color);
        display_sprite( pFrameSet->xpipe, x + w*xsc, y, z, (wd - 2*w*xsc)/pFrameSet->xpipe->width, ysc, color );
    }

    // right
    if ( dir != kTabDir_Right )
    {
        display_sprite( back,  x + wd - w*xsc, y + w*ysc, z, (w-size)*xsc/back->width, (ht - 2*w*ysc)/back->height, back_color);
        display_sprite( pFrameSet->ypipe, x + wd - size*xsc, y, z, xsc, (ht - w*ysc)/pFrameSet->ypipe->height, color );
    }

    // bottom
    if ( dir != kTabDir_Bottom )
    {
        display_sprite( back, x + w*xsc, y + ht - w*ysc, z, (wd - 2*w*xsc)/back->width, (w - size)*ysc/back->height, back_color);
        display_sprite( pFrameSet->xpipe, x + w*xsc, y + ht - size*ysc, z, (wd - 2*w*xsc)/pFrameSet->xpipe->width, ysc, color );
    }

    // left
    if ( dir != kTabDir_Left )
    {
        display_sprite( back, x + size*xsc, y + w*ysc, z, (w - size)*xsc/back->width, (ht - 2*w*ysc)/back->height, back_color);
        display_sprite( pFrameSet->ypipe, x, y, z, xsc, (ht - w*ysc)/pFrameSet->ypipe->height, color );
    }
}


void drawFrameOpenTopStyle( int size, int radius, float x, float y, float z, float wd, float ht, float xa, float xb, float sc, int color, int back_color, FrameStyle style )
{
    FrameSet *pFrameSet = getFrameSet( style, size, radius, kTabDir_None );
    float w, wd2;
    float xsc = sc, ysc = sc;
    int offset = 0;
    AtlasTex * back = white_tex_atlas;

    w = pFrameSet->ul->width;
    if ( wd < w*2*sc )
        xsc = (float)wd/(w*2); //wd = w*2;
    if ( ht < w*2*sc )
        ysc = (float)ht/(w*2); //ht = w*2;

    // middle
    display_sprite( back, x + w*xsc, y + w*ysc, z, (wd - 2*w*xsc)/back->width, (ht - 2*w*ysc)/back->height, back_color);

    // top left
    display_sprite( pFrameSet->bul, x - offset, y - offset, z, xsc, ysc, back_color);
    display_sprite( pFrameSet->ul, x, y, z, xsc, ysc, color );

    // top 1
    if ((xa - x) > w*xsc)
    {
        float dx = xa - x;
        wd2 = dx - w*xsc;
        display_sprite( back , x + w*xsc, y + size*ysc, z, wd2/back->width, (w - size)*ysc/back->height, back_color);
        display_sprite( pFrameSet->xpipe, x + w*xsc, y, z, wd2/pFrameSet->xpipe->width, ysc, color );
    }

    // top 2
    if ((xb - x) < (wd - w*xsc))
    {
        float dx = xb - x;
        MAX1(dx, w*xsc);
        wd2 = wd - dx - w*xsc;
        display_sprite( back , x+dx, y + size*ysc, z, wd2/back->width, (w - size)*ysc/back->height, back_color);
        display_sprite( pFrameSet->xpipe, x+dx, y, z, wd2/pFrameSet->xpipe->width, ysc, color );
    }

    // top right
    display_sprite( pFrameSet->bur, x + wd - w*xsc, y - offset, z, xsc, ysc, back_color);
    display_sprite( pFrameSet->ur,  x + wd - w*xsc, y, z, xsc, ysc, color );

    // right
    display_sprite( back,  x + wd - w*xsc, y + w*ysc, z, (w-size)*xsc/back->width, (ht - 2*w*ysc)/back->height, back_color);
    display_sprite( pFrameSet->ypipe, x + wd - size*xsc, y + w*ysc, z, xsc, (ht - 2*w*ysc)/pFrameSet->ypipe->height, color );

    // lower right
    display_sprite( pFrameSet->blr, x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, back_color);
    display_sprite( pFrameSet->lr,  x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, color );

    // bottom
    display_sprite( back, x + w*xsc, y + ht - w*ysc, z, (wd - 2*w*xsc)/back->width, (w - size)*ysc/back->height, back_color);
    display_sprite( pFrameSet->xpipe, x + w*xsc, y + ht - size*ysc, z, (wd - 2*w*xsc)/pFrameSet->xpipe->width, ysc, color );

    // lower left
    display_sprite( pFrameSet->bll, x - offset, y + ht - w*ysc, z, xsc, ysc, back_color);
    display_sprite( pFrameSet->ll,  x, y + ht - w*ysc, z, xsc, ysc, color );

    // left
    display_sprite( back, x + size*xsc, y + w*ysc, z, (w - size)*xsc/back->width, (ht - 2*w*ysc)/back->height, back_color);
    display_sprite( pFrameSet->ypipe, x, y + w*ysc, z, xsc, (ht - 2*w*ysc)/pFrameSet->ypipe->height, color );
}

void drawFrameTabStyle( int size, int radius, float x, float y, float z, float wd, float ht, float sc, int color, FrameStyle style )
{
    FrameSet *pFrameSet = getFrameSet( style, size, radius, kTabDir_None );
    float w;
    float xsc = sc, ysc = sc;

    w = pFrameSet->ul->width;

    // top left
    display_sprite( pFrameSet->ul, x+(w-size)*xsc, y, z, xsc, ysc, color );

    // top
    display_sprite( pFrameSet->xpipe, x + 2*(w-(size-1))*xsc, y, z, (wd - 3*(w+(size-1))*xsc)/pFrameSet->xpipe->width, ysc, color );

    // top right
    display_sprite( pFrameSet->ur,  x + wd - w*xsc - (w-size)*xsc, y, z, xsc, ysc, color );

    // lower left
    display_sprite( pFrameSet->lr,  x, y + w*ysc, z, xsc, ysc, color );

    // lower right
    display_sprite( pFrameSet->ll,  x + wd - w*xsc, y + w*ysc, z, xsc, ysc, color );
}


void drawBustedOutFrame( int style, int size, int radius, float x, float y, float z, float sc, float wd, float mid_ht, float top_ht, float bot_ht, int color, int bcolor, int fallback )
{
    FrameSet *pFrameSet = getFrameSet( style, size, radius, kTabDir_None );
    float w, fz = z+10;
    float xsc = sc, ysc = sc;
    AtlasTex * back = white_tex_atlas;

     w = pFrameSet->ul->width*sc;

    //if( mid_ht )
    //{
    //    if( bot_ht < 2*w*sc + mid_ht/2) 
    //        mid_ht = MAX(0, bot_ht - 2*w*sc );
    //    if (top_ht < 2*w*sc + mid_ht/2)
    //        mid_ht = MAX(0, top_ht - 2*w*sc );
    //}

    if( fallback )
    {
        if( (bot_ht < 2*w*sc + mid_ht/2) ||
            (top_ht < 2*w*sc + mid_ht/2))
        {
            drawFrameStyle( size, radius, x, y - top_ht, z, wd, top_ht+bot_ht, sc, color, bcolor, style );
            return;
        }
    }
    else
    {
        if( bot_ht < 2*w*sc + mid_ht/2) 
            bot_ht = 2*w*sc + mid_ht/2;
        if (top_ht < 2*w*sc + mid_ht/2)
            top_ht = 2*w*sc + mid_ht/2;
    }

    //Top--------------------------------------------------------------------------

    // top left
      display_sprite( pFrameSet->bul, x, y - top_ht, z, sc, sc, bcolor);
    display_sprite( pFrameSet->ul,  x, y - top_ht, fz, sc, sc, color );

    // top - back down
    display_sprite( back, x+w*xsc, y - top_ht+size*sc, z, (wd - 2*w*sc)/back->width, (top_ht - mid_ht/2 - size)*sc/back->height, bcolor);
    display_sprite( pFrameSet->xpipe, x+w*sc, y - top_ht, fz, (wd - 2*w*sc)/pFrameSet->xpipe->width, sc, color );

    // top right
    display_sprite( pFrameSet->bur, x+wd - w*sc, y - top_ht, z, sc, sc, bcolor);
    display_sprite( pFrameSet->ur,  x+wd - w*sc, y - top_ht, fz, sc, sc, color );

    // left
    display_sprite( back, x + size*sc, y - top_ht + w*sc, z, (w - size)*sc/back->width, (top_ht - 2*w*sc - mid_ht/2)/back->height, bcolor);
    display_sprite( pFrameSet->ypipe, x, y - top_ht + w*sc, fz, sc, (top_ht - 2*w*sc - mid_ht/2)/pFrameSet->ypipe->height, color );

    // right
    display_sprite( back,  x + wd - w*sc, y - top_ht + w*sc, z, (w-size)*sc/back->width, (top_ht - 2*w*sc - mid_ht/2)/back->height, bcolor);
    display_sprite( pFrameSet->ypipe, x + wd - size*sc, y - top_ht + w*sc, fz, sc, (top_ht - 2*w*sc - mid_ht/2)/pFrameSet->ypipe->height, color );


    //Busted--------------------------------------------------------------------------

    // this may go outside frame if somone uses these for real
       display_sprite( back, x-w+size, y - mid_ht/2 - size-1, z, (w-size)/back->width, (mid_ht+2*size+2)/back->height, bcolor);
     display_sprite( back, x+wd, y - mid_ht/2 - size-1, z, (w-size)/back->width, (mid_ht+2*size+2)/back->height, bcolor);

    // busted out top left
       display_sprite( back, x, y-w-mid_ht/2, z, (w)/back->width, (2*w+mid_ht)/back->height, bcolor);
       display_sprite( pFrameSet->lr,  x-w+size, y-w-mid_ht/2, fz, sc, sc, color );

    // busted out lower left
     display_sprite( pFrameSet->ur,  x-w+size, y+mid_ht/2, fz, sc, sc, color );

    // busted out top right
    display_sprite( back, x+wd-w, y-w-mid_ht/2, z, (w)/back->width, (2*w+mid_ht)/back->height, bcolor);
     display_sprite( pFrameSet->ll,  x+wd-size, y-w-mid_ht/2, fz, sc, sc, color );

    // busted out lower right
      display_sprite( pFrameSet->ul,  x+wd-size, y+mid_ht/2, fz, sc, sc, color );

    // middle
    display_sprite( back, x+w, y - mid_ht/2, z, (wd-2*w)/back->width, (mid_ht)/back->height, bcolor);

 
    //Bottom---------------------------------------------------------------------------

    // left
     display_sprite( back, x + size*sc, y + mid_ht/2 + w*sc, z, (w - size)*sc/back->width, (bot_ht - 2*w*sc - mid_ht/2)/back->height, bcolor);
    display_sprite( pFrameSet->ypipe, x, y + mid_ht/2 + w*sc, fz, sc, (bot_ht - 2*w*sc - mid_ht/2)/pFrameSet->ypipe->height, color );

    // right
    display_sprite( back,  x + wd - w*sc, y + mid_ht/2 + w*sc, z, (w-size)*sc/back->width, (bot_ht - 2*w*sc - mid_ht/2)/back->height, bcolor);
    display_sprite( pFrameSet->ypipe, x + wd - size*sc, y + mid_ht/2 + w*sc, fz, sc, (bot_ht - 2*w*sc - mid_ht/2)/pFrameSet->ypipe->height, color );


    // lower right
    display_sprite( pFrameSet->blr, x+wd - w*sc, y+bot_ht - w*sc, z, sc, sc, bcolor);
    display_sprite( pFrameSet->lr,  x+wd - w*sc, y+bot_ht - w*sc, fz, sc, sc, color );

    // bottom - back up
    display_sprite( back, x + w*sc, y + mid_ht/2, z, (wd - 2*w*sc)/back->width, (bot_ht - mid_ht/2 - size)*sc/back->height, bcolor);
    display_sprite( pFrameSet->xpipe, x + w*sc, y + bot_ht - size*sc, fz, (wd - 2*w*sc)/pFrameSet->xpipe->width, sc, color );

    // lower left
    display_sprite( pFrameSet->bll, x, y + bot_ht - w*sc, z, sc, sc, bcolor);
    display_sprite( pFrameSet->ll,  x, y + bot_ht - w*sc, fz, sc, sc, color );
}


void drawFrameWithBounds(int size, int radius, float x, float y, float z, float wd, float ht, int color, int back_color, UIBox* bounds)
{
    drawFrame(size, radius, x, y, z, wd, ht, 1.f, color, back_color);
    if(bounds)
    {
        bounds->x = x;
        bounds->y = y-1;
        bounds->height = ht;
        bounds->width = wd-1;
        uiBoxAlter(bounds, UIBAT_SHRINK, UIBAD_ALL, size);
    }
}

void drawJoinedFrame2Panel( int size, int radius, float x, float y, float z, float sc, float wd, float ht, float ht2,
                            int color, int back_color, UIBox* bounds1, UIBox* bounds2)
{
    drawJoinedFrame2PanelStyle(size, radius, x, y, z, sc, wd, ht, ht2, color, back_color, bounds1, bounds2, kFrameStyle_3D);
}

void drawJoinedFlatFrame2Panel( int size, int radius, float x, float y, float z, float sc, float wd, float ht, float ht2,
                                int color, int back_color, UIBox* bounds1, UIBox* bounds2)
{
    drawJoinedFrame2PanelStyle(size, radius, x, y, z, sc, wd, ht, ht2, color, back_color, bounds1, bounds2, kFrameStyle_Flat);
}

void drawJoinedFrame2PanelStyle( int size, int radius, float x, float y, float z, float sc, float wd, float ht, float ht2,
                                 int color, int back_color, UIBox* bounds1, UIBox* bounds2, FrameStyle style)
{
    char temp[128];
    float w, xsc = sc, ysc = sc;
    char *pchStyle;
    AtlasTex *ul, *ll, *lr, *ur, *ypipe, *xpipe, *bul, *bll, *blr, *bur, *jl, *jr, *bjl, *bjr;
    AtlasTex * back = white_tex_atlas;

    if( style&kFrameStyle_Flat)
        pchStyle = "Flat_";
    else
        pchStyle = "";

    // load the proper textures
    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_UL.tga");
    STR_COMBINE_END();
    ul = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_LL.tga");
    STR_COMBINE_END();
    ll = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_LR.tga");
    STR_COMBINE_END();
    lr = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_UR.tga");
    STR_COMBINE_END();
    ur = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_join_L.tga");
    STR_COMBINE_END();
    jl = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_join_R.tga");
    STR_COMBINE_END();
    jr = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_vert");
    STR_COMBINE_END();
    ypipe = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_horiz.tga");
    STR_COMBINE_END();
    xpipe = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_Background_UL.tga");
    STR_COMBINE_END();
    bul = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_Background_LL.tga");
    STR_COMBINE_END();
    bll = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_Background_LR.tga");
    STR_COMBINE_END();
    blr = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_Background_UR.tga");
    STR_COMBINE_END();
    bur = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_join_Background_L.tga");
    STR_COMBINE_END();
    bjl = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(size);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_join_Background_R.tga");
    STR_COMBINE_END();
    bjr = atlasLoadTexture( temp );

    w = ul->width;
      if ( wd < w*2*sc )
        xsc = wd/(w*2*sc); //wd = w*2;
    if ( ht < w*2*sc )
        ysc = ht/(w*2*sc); //ht = w*2;

    //---------------------------------------------------------------
    // top pane
    //---------------------------------------------------------------

    // top left
    display_sprite( ul, x, y, z, xsc, ysc, color );
    display_sprite( bul, x, y, z-3, xsc, ysc, back_color );

    // top
    display_sprite( xpipe, x + w*xsc, y, z, (wd - 2*w*xsc)/xpipe->width, ysc, color );
    display_sprite( back , x + w*xsc, y + size*ysc, z-3, (wd - 2*w*xsc)/back->width, (w - size)*ysc/back->height, back_color );

    // top right
    display_sprite( ur,  x + wd - w*xsc, y, z, xsc, ysc, color );
    display_sprite( bur, x + wd - w*xsc, y, z-3, xsc, ysc, back_color );

    // right
    display_sprite( ypipe, x + wd - size*xsc, y + w*ysc, z, xsc, (ht - 2*w*ysc)/ypipe->height, color );
    display_sprite( back,  x + wd - w*xsc, y + w*ysc, z-3, (w-size)*xsc/back->width, (ht - 2*w*ysc)/back->height, back_color );

    // lower right - JOIN
    display_sprite( jr,  x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, color );
    display_sprite( bjr, x + wd - w*xsc, y + ht - w*ysc, z-3, xsc, ysc, back_color );

    // bottom
    display_sprite( xpipe, x + w*xsc, y + ht - size*ysc, z, (wd - 2*w*xsc)/xpipe->width, ysc, color );
    display_sprite( back, x + w*xsc, y + ht - w*ysc, z-3, (wd - 2*w*xsc)/back->width, (w - size)*ysc/back->height, back_color );

    // lower left - JOIN
    display_sprite( jl,  x, y + ht - w*ysc, z, xsc, ysc, color );
    display_sprite( bjl, x, y + ht - w*ysc, z-3, xsc, ysc, back_color );

    // left
    display_sprite( ypipe, x, y + w*ysc, z, xsc, (ht - 2*w*ysc)/ypipe->height, color );
    display_sprite( back, x + size*xsc, y + w*ysc, z-3, (w - size)*xsc/back->width, (ht - 2*w*ysc)/back->height, back_color );

    // middle
    display_sprite(back, x + w*xsc, y + w*ysc, z-3, (wd - 2*w*xsc)/back->width, (ht - 2*w*ysc)/back->height, back_color );

    if(bounds1)
    {
        bounds1->x = x;
        bounds1->y = y-1*sc;
        bounds1->height = ht;
        bounds1->width = wd-1*sc;
        uiBoxAlter(bounds1, UIBAT_SHRINK, UIBAD_ALL, size*sc);
    }

    //---------------------------------------------------------------
    // middle pane
    //---------------------------------------------------------------

    y    += ht;
    ht    = ht2;

    if ( ht < w*2*sc )
        ysc = (float)ht/(w*2*sc); //ht = w*2;

    //left
    display_sprite( ypipe, x, y + w*ysc - size*ysc, z, xsc, (ht - 2*w*ysc + size*ysc)/ypipe->height, color );
    display_sprite( back, x + size*xsc, y + w*ysc - size*ysc, z-3, (w - size)*xsc/back->width, (ht - 2*w*ysc + size*ysc)/back->height, back_color );

    // right
    display_sprite( ypipe, x + wd - size*xsc, y + w*ysc - size*ysc, z, xsc, (ht - 2*w*ysc + size*ysc)/ypipe->height, color );
    display_sprite( back,  x + wd - w*xsc, y + w*ysc - size*ysc, z-3, (w-size)*xsc/back->width, (ht - 2*w*ysc + size*ysc)/back->height, back_color );

    // lower right
    display_sprite( lr,  x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, color );
    display_sprite( blr, x + wd - w*xsc, y + ht - w*ysc, z-3, xsc, ysc, back_color );

    // bottom
     display_sprite( xpipe, x + w*xsc, y + ht - size*ysc, z, (wd - 2*w*xsc)/xpipe->width, ysc, color );
    display_sprite( back, x + w*xsc, y + ht - w*ysc, z-3, (wd - 2*w*xsc)/back->width, (w - size)*ysc/back->height, back_color );

    // lower left
    display_sprite( ll,  x, y + ht - w*ysc, z, xsc, ysc, color );
    display_sprite( bll, x, y + ht - w*ysc, z-3, xsc, ysc, back_color );

    // middle
    display_sprite(back, x + w*xsc, y, z-3, (wd - 2*w*xsc)/back->width, (ht - w*ysc)/back->height, back_color );

    if(bounds2)
    {
        bounds2->x = x;
        bounds2->y = y-3*sc;
        bounds2->height = ht2+2*sc;
        bounds2->width = wd-1*sc;
        uiBoxAlter(bounds2, UIBAT_SHRINK, UIBAD_ALL, size*sc);
    }
}


//
//
void drawJoinedFrame3Panel( int size, int radius, float x, float y, float z, float sc, float wd, float ht, float ht2, float ht3,
                            int color, int back_color )
{
    drawJoinedFrame3PanelStyle(size, radius, x, y, z, sc, wd, ht, ht2, ht3, color, back_color, kFrameStyle_3D);
}

void drawJoinedFlatFrame3Panel( int size, int radius, float x, float y, float z, float sc, float wd, float ht, float ht2, float ht3,
                                int color, int back_color )
{
    drawJoinedFrame3PanelStyle(size, radius, x, y, z, sc, wd, ht, ht2, ht3, color, back_color, kFrameStyle_Flat);
}

void drawJoinedFrame3PanelStyle( int isize, int radius, float x, float y, float z, float sc, float wd, float ht, float ht2, float ht3,
                                 int color, int back_color, FrameStyle style )
{
    char temp[128];
    float w, size;
    char *pchStyle;
    AtlasTex *ul, *ll, *lr, *ur, *ypipe, *xpipe, *bul, *bll, *blr, *bur, *jl, *jr, *bjl, *bjr;
    AtlasTex * back = white_tex_atlas;


    if(style&kFrameStyle_Flat)
        pchStyle = "Flat_";
    else
        pchStyle = "";

    // load the proper textures
    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_UL.tga");
    STR_COMBINE_END();
    ul = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_LL.tga");
    STR_COMBINE_END();
    ll = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_LR.tga");
    STR_COMBINE_END();
    lr = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_UR.tga");
    STR_COMBINE_END();
    ur = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_join_L.tga");
    STR_COMBINE_END();
    jl = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_join_R.tga");
    STR_COMBINE_END();
    jr = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_vert");
    STR_COMBINE_END();
    ypipe = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT(pchStyle);
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_horiz.tga");
    STR_COMBINE_END();
    xpipe = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_Background_UL.tga");
    STR_COMBINE_END();
    bul = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_Background_LL.tga");
    STR_COMBINE_END();
    bll = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_Background_LR.tga");
    STR_COMBINE_END();
    blr = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_Background_UR.tga");
    STR_COMBINE_END();
    bur = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_join_Background_L.tga");
    STR_COMBINE_END();
    bjl = atlasLoadTexture( temp );

    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("Frame_");
    STR_COMBINE_CAT_D(isize);
    STR_COMBINE_CAT("px_");
    STR_COMBINE_CAT_D(radius);
    STR_COMBINE_CAT("r_join_Background_R.tga");
    STR_COMBINE_END();
    bjr = atlasLoadTexture( temp );

    w = ul->width*sc;
    if ( wd < w*2 )
        wd = w*2;
    if ( ht < w*2 )
        ht = w*2;

    size = (float)isize * sc;

    //---------------------------------------------------------------
    // top pane
    //---------------------------------------------------------------
 
    // top left
    display_sprite( ul, x, y, z, sc, sc, color );
    display_sprite( bul, x, y, z-3, sc, sc, back_color );

    // top
    display_sprite( xpipe, x + w, y, z, (wd - 2*w)/xpipe->width, sc, color );
    display_sprite( back , x + w, y + size, z-3, (wd - 2*w)/back->width, (w - size)/back->height, back_color );

    // top right
    display_sprite( ur,  x + wd - w, y, z, sc, sc, color );
    display_sprite( bur, x + wd - w, y, z-3, sc, sc, back_color );

    // right
    display_sprite( ypipe, x + wd - size, y + w, z, sc, (ht - 2*w)/ypipe->height, color );
    display_sprite( back,  x + wd - w, y + w, z-3, (w-size)/back->width, (ht - 2*w)/back->height, back_color );

    // lower right - JOIN
    display_sprite( jr,  x + wd - w, y + ht - w, z, sc, sc, color );
    display_sprite( bjr, x + wd - w, y + ht - w, z-3, sc, sc, back_color );

    // bottom
    display_sprite( xpipe, x + w, y + ht - size, z, (wd - 2*w)/xpipe->width, sc, color );
    display_sprite( back, x + w, y + ht - w, z-3, (wd - 2*w)/back->width, (w - size)/back->height, back_color );

    // lower left - JOIN
    display_sprite( jl,  x, y + ht - w, z, sc, sc, color );
    display_sprite( bjl, x, y + ht - w, z-3, sc, sc, back_color );

    // left
    display_sprite( ypipe, x, y + w, z, sc, (ht - 2*w)/ypipe->height, color );
    display_sprite( back, x + size, y + w, z-3, (w - size)/back->width, (ht - 2*w)/back->height, back_color );

    // middle
    display_sprite(back, x + w, y + w, z-3, (wd - 2*w)/back->width, (ht - 2*w)/back->height, back_color );

    //---------------------------------------------------------------
    // middle pane
    //---------------------------------------------------------------

    y    += ht;
    ht    = ht2;

    if ( ht < w*2 )
        ht = w*2;

    //left
      display_sprite( ypipe, x, y + w - size, z, sc, (ht - 2*w + size)/ypipe->height, color );
      display_sprite( back, x + size, y + w - size, z-3, (w - size)/back->width, (ht - 2*w + size)/back->height, back_color );

    // right
    display_sprite( ypipe, x + wd - size, y + w - size, z, sc, (ht - 2*w + size)/ypipe->height, color );
    display_sprite( back,  x + wd - w, y + w - size, z-3, (w-size)/back->width, (ht - 2*w + size)/back->height, back_color );

    // lower right - JOIN
    display_sprite( jr,  x + wd - w, y + ht - w, z, sc, sc, color );
    display_sprite( bjr, x + wd - w, y + ht - w, z-3, sc, sc, back_color );

    // bottom
     display_sprite( xpipe, x + w, y + ht - size, z, (wd - 2*w)/xpipe->width, sc, color );
    display_sprite( back, x + w, y + ht - w, z-3, (wd - 2*w)/back->width, (w - size)/back->height, back_color );

    // lower left - JOIN
    display_sprite( jl,  x, y + ht - w, z, sc, sc, color );
    display_sprite( bjl, x, y + ht - w, z-3, sc, sc, back_color );

    // middle
     display_sprite(back, x + w, y, z-3, (wd - 2*w)/back->width, (ht - w)/back->height, back_color );

    //---------------------------------------------------------------
    // bottom pane
    //---------------------------------------------------------------

    y    += MAX(ht2,ht);
     ht    = ht3;

    if ( ht < w*2 )
        ht = w*2;

    //left
    display_sprite( ypipe, x, y + w - size, z, sc, (ht - 2*w + size)/ypipe->height, color );
    display_sprite( back, x + size, y + w - size, z-3, (w - size)/back->width, (ht - 2*w + size)/back->height, back_color );

    // right
    display_sprite( ypipe, x + wd - size, y + w - size, z, sc, (ht - 2*w + size)/ypipe->height, color );
    display_sprite( back,  x + wd - w, y + w - size, z-3, (w-size)/back->width, (ht - 2*w + size)/back->height, back_color );

    // lower right
    display_sprite( lr,  x + wd - w, y + ht - w, z, sc, sc, color );
    display_sprite( blr, x + wd - w, y + ht - w, z-3, sc, sc, back_color );

    // bottom
    display_sprite( xpipe, x + w, y + ht - size, z, (wd - 2*w)/xpipe->width, sc, color );
    display_sprite( back, x + w, y + ht - w, z-3, (wd - 2*w)/back->width, (w - size)/back->height, back_color );

    // lower left
    display_sprite( ll,  x, y + ht - w, z, sc, sc, color );
    display_sprite( bll, x, y + ht - w, z-3, sc, sc, back_color );

    // middle
     display_sprite(back, x + w, y, z-3, (wd - 2*w)/back->width, (ht - w)/back->height, back_color );

}

//--------------------------------------------------------------------------------------------------------------------
// Fading bar ////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------

void drawFadeBar( float x, float y, float z, float wd, float ht, int color )
{
    float xsc;
    AtlasTex * left    = atlasLoadTexture( "Contact_fadebar_L.tga" );
    AtlasTex * right    = atlasLoadTexture( "Contact_fadebar_R.tga" );
    AtlasTex * mid    = atlasLoadTexture( "Contact_fadebar_MID.tga" );

    float ysc = ht/left->height;

    display_sprite( left, x, y, z, ysc, ysc, color );
    xsc = wd - right->width*ysc;
    if(xsc<left->width*ysc) xsc=left->width*ysc;
    display_sprite( right, x + xsc, y, z, ysc, ysc, color );
    xsc = (wd - (left->width+right->width)*ysc)/mid->width;
    if(xsc>0)
    {
        display_sprite( mid, x + left->width*ysc, y, z, xsc, ysc, color );
    }
}
//--------------------------------------------------------------------------------------------------------------------
// Header ////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------

typedef enum SortState
{
    SORT_NONE,
    SORT_UP,
    SORT_DOWN,
} SortState;


int drawHeader( float x, float y, float z, float wd, char * text, SortState sort_state )
{
    //CBox box;

    //BuildCBox( &box)

    return 0;
}

void uiReset()
{
    friendListReset();
    sgListReset();
    picturebrowser_clearEntities();
    resetPowerCustomizationPreview();
}


// Small checkbox that keeps cropping up
//-------------------------------------------------------------------------------------


int drawSmallCheckboxEx( int wdw, float x, float y, float z, float sc, char *text, int *on, void (*callback)(), int selectable, char *tiptxt, ToolTipParent *tipParent, int bBigFont, int txt_color )
{
    CBox box;
    AtlasTex *filled, *empty;
    float wx, wy, wz;
    int color;
 
    if( wdw && !window_getDims( wdw, &wx, &wy, &wz, NULL, NULL, &sc, &color, NULL ) )
        return 0;
    if( !wdw )
    {
        wx = wy = 0;
        wz = z;
        color = CLR_WHITE;
    }

    filled = atlasLoadTexture( "Context_checkbox_filled.tga" );
    empty = atlasLoadTexture( "Context_checkbox_empty.tga" );

    if( tiptxt )
          addWindowToolTip( x - empty->width*sc/2, y - empty->height*sc/2, empty->width*sc*3/2 + str_wd(font_grp, sc, sc, text), empty->height*sc, tiptxt, tipParent, MENU_GAME, wdw, 0 );
    
    x += wx;
    y += wy;

    BuildCBox( &box, x - empty->width*sc/2, y - empty->height*sc/2, empty->width*sc + str_wd(font_grp, sc, sc, text), empty->height*sc );

    if( !selectable )
        color = (color&0xffffff00)|0x88;

    if( *on )
    {
        display_sprite( filled, x - empty->width*sc/2, y - empty->height*sc/2, wz+1, sc, sc, color );
        display_sprite( empty, x - empty->width*sc/2, y - empty->height*sc/2, wz, sc, sc, color );
    }
    else
        display_sprite( empty, x - empty->width*sc/2, y - empty->height*sc/2, wz+1, sc, sc, color );

     if (bBigFont)
        font(&game_12);
    else
        font(&game_9);

    if(!txt_color)
        txt_color = CLR_WHITE;

    if( selectable )
        font_color( txt_color, txt_color );
    else
        font_color( txt_color&0xffffff88,  txt_color&0xffffff88 );

    if( text )
        cprntEx(x + empty->width*sc, y, wz+1, sc, sc, CENTER_Y, text );



    if( mouseDownHit( &box, MS_LEFT ) && selectable )
    {
        *on = !(*on);
        if( callback )
            callback();

        return true;
    }

    return false;
}

void drawMultiBar( MultiBar * bar, float have, float need, float x, float y, float z, float sc, float wd, int color )
{
    int bar_color, back_color;
    float percentage;
    int compile_hack = 0;

    if( compile_hack )
    {
        if( have >= need )
        {
            percentage = (have - need)/need;
            back_color = 0x00ff0022;
             interp_rgba( percentage, 0x00ff00ff, 0x00ff00ff, &bar_color );
            handleGlowState( &bar->need_mode, &bar->need_percent, percentage, &bar->need_glow_scale, 60 );
              drawBar( x, y-1, z, wd/2, sc, bar->need_percent, bar->need_glow_scale, bar_color, 0);
        }
        else
        {
            percentage = (need - have)/need;
            back_color = 0xff000022;
            interp_rgba( percentage, 0xff0000ff, 0xff0000ff, &bar_color );
            handleGlowState( &bar->need_mode, &bar->need_percent, percentage, &bar->need_glow_scale, 60 );
            drawBar( x, y-1, z, wd/2, sc, bar->need_percent, bar->need_glow_scale, bar_color, 1);
        }

        // draw the frame
        drawFrame( PIX2, R4, x - wd/2, y+1, z, wd, R6*2, sc, color, back_color );
        drawFrame( PIX2, R4, x - R6*2/2, y+1, z, R6*2, R6*2, sc, back_color|0xff, back_color|0xff );
    }
    else
    {
         int base = 125;
          while( need > base || have > base )
             base *= 2;

        if( have >= need )
                  back_color = 0x00000066;
        else
        {
            float s = sinf(bar->pulse_timer);
               back_color = 0xff000000|(int)(50 + s*25);
             bar->pulse_timer += TIMESTEP*.2;
        }

        percentage = need/base;
        handleGlowState( &bar->need_mode, &bar->need_percent, percentage, &bar->need_glow_scale, 60 );
         drawBar( x-wd/2-1*sc, y-1*sc, z, wd, sc, bar->need_percent, bar->need_glow_scale, 0xff0000ff, 0);

        percentage = have/base;
        handleGlowState( &bar->have_mode, &bar->have_percent, percentage, &bar->have_glow_scale, 60 );
         drawBar( x-wd/2-1*sc, y+8*sc, z, wd, sc, bar->have_percent, bar->have_glow_scale, 0x00ff00ff, 0);

          drawFrame( PIX2, R4, x - wd/2, y+1, z-1, wd, 20*sc, sc, 0, back_color );
         drawFrame( PIX2, R4, x - wd/2, y+1, z, wd, 20*sc, sc, color, 0 );
    }
}

#define GLOW_FADE_SPEED 5  // larger number is a slower speed

//
//
void handleGlowState( int * mode, float * last_percentage, float percentage, float * glow_scale, float speed )
{
    int sign;

    if ( *last_percentage < percentage )
        sign = 1;
    else
        sign = -1;

    switch( *mode )
    {
    case GLOW_NONE:
        {
            if( *last_percentage != percentage ) //ABS(*last_percentage-percentage) > .05f )
                *mode = GLOW_FADE_IN;
            //else if ( *last_percentage != percentage )
            //    *mode = GLOW_MOVE_NONE;
            else
                *last_percentage = percentage;

        } break;

    case GLOW_FADE_IN:
        {
            *glow_scale += TIMESTEP/GLOW_FADE_SPEED;
            if ( *glow_scale >1.f )
            {
                *glow_scale = 1.f;
                *mode = GLOW_MOVE;
            }
        } break;

    case GLOW_MOVE:
        {
            *last_percentage += sign * TIMESTEP/speed;

            if ( sign == -1 && *last_percentage < percentage || *last_percentage < 0 )
            {
                *last_percentage = percentage;
                *mode = GLOW_FADE_OUT;
            }
            else if ( sign == 1 && *last_percentage > percentage )
            {
                *last_percentage = percentage;
                *mode = GLOW_FADE_OUT;
            }

        }break;

    case GLOW_MOVE_NONE:
        {
            if ( sign == 1 )
            {
                *last_percentage += sign * TIMESTEP/speed;
                if( *last_percentage > percentage )
                {
                    *mode = GLOW_NONE;
                    *last_percentage = percentage;
                }
            }
            else
            {
                *last_percentage += sign * TIMESTEP/speed;
                if ( *last_percentage < percentage )
                {
                    *mode = GLOW_NONE;
                    *last_percentage = percentage;
                }
            }
        } break;

    case GLOW_FADE_OUT:
        {
            *last_percentage = percentage;
            *glow_scale -= TIMESTEP/GLOW_FADE_SPEED;

            if ( *glow_scale < 0.0f )
            {
                *glow_scale = 0.0f;
                *mode = GLOW_NONE;
            }
        }

    default:
        {
            *last_percentage = percentage;
        } break;
    }
}

void drawBar( float x, float y, float z, float width, float scale, float percentage, float glow_scale, int color, int negative )
{
    static AtlasTex *bar;
    static AtlasTex *shine;
    static AtlasTex *glow;
    static int texBindInit=0;
    if (!texBindInit) {
        texBindInit = 1;
        bar    = atlasLoadTexture( "healthbar_slider.tga" );
        shine    = atlasLoadTexture( "healthbar_sliderhighlight.tga" );
        glow    = atlasLoadTexture( "healthbar_glow.tga" );
    }

    if( negative )
    {
        float wd = percentage*(width-8*scale);
        // drop shadow
        display_sprite( bar, x - wd - 5*scale, y + 4*scale, z-1, wd/bar->width, scale, CLR_BLACK );

        // bar
        display_sprite( bar, x - wd - 4*scale, y + 4*scale, z, wd/bar->width, scale, color );

        // shine
        display_sprite( shine, x - wd - 4*scale, y + 4*scale, z+1, wd/shine->width, scale, CLR_WHITE );

        // draw the glow

        // base
        display_sprite( glow, x - wd - 4*scale - wd+ glow_scale*glow->width*scale/2,
            y + 6*scale + glow_scale*scale*(bar->width - glow->width)/2, z, glow_scale*scale, glow_scale*scale, color );

        // bright overlay
        glow_scale *= .8f;
        display_sprite( glow, x - wd - 4*scale - wd + glow_scale*glow->width*scale/2,
            y + 6*scale + glow_scale*scale*(bar->width - glow->width)/2, z, glow_scale*scale, glow_scale*scale, CLR_WHITE );
    }
    else
    {
        // drop shadow
        display_sprite( bar, x + 5*scale, y + 4*scale, z-1, percentage*(width-8*scale)/bar->width, scale, CLR_BLACK );

        // bar
        display_sprite( bar, x + 4*scale, y + 4*scale, z, percentage*(width-8*scale)/bar->width, scale, color );

        // shine
        display_sprite( shine, x + 4*scale, y + 4*scale, z+1, percentage*(width-8*scale)/shine->width, scale, CLR_WHITE );

        // draw the glow

        // base
        display_sprite( glow, x + 4*scale + percentage*(width-8*scale) - glow_scale*glow->width*scale/2,
            y + 6*scale + glow_scale*scale*(bar->width - glow->width)/2, z, glow_scale*scale, glow_scale*scale, color );

        // bright overlay
        glow_scale *= .8f;
        display_sprite( glow, x + 4*scale + percentage*(width-8*scale) - glow_scale*glow->width*scale/2,
            y + 6*scale + glow_scale*scale*(bar->width - glow->width)/2, z, glow_scale*scale, glow_scale*scale, CLR_WHITE );
    }
}


void drawBendyTrayFrame( float x, float y, float z, float wd, float ht, float sc, int color, int back_color, int bend_type )
{
    FrameSet *pFrameSet = getFrameSet( kFrameStyle_Standard, PIX3, R22, kTabDir_None );
    float w, width, size = PIX3;
    float xsc = sc, ysc = sc;
    AtlasTex * back = white_tex_atlas;
    AtlasTex *eul    = atlasLoadTexture( "frame_blue_background_elbow_UL.tga" );
    AtlasTex *ell    = atlasLoadTexture( "frame_blue_background_elbow_LL.tga" );
    AtlasTex *elr    = atlasLoadTexture( "frame_blue_background_elbow_LR.tga" );
    AtlasTex *eur    = atlasLoadTexture( "frame_blue_background_elbow_UR.tga" );

    if( wd <= 50.1*sc || ht <= 50.1*sc )
    {
        drawFrame(PIX3, R22, x, y, z, MAX(50*sc, wd), MAX(50*sc, ht), sc, color, back_color );
        return;
    }

    // account for min size of frame
    if( wd < 100*sc )
    {
        wd = 100*sc;
        if( bend_type == kBend_UR || bend_type == kBend_LR )
            x -= 10*sc;
    }
    if( ht < 100*sc )
    {
        if( bend_type == kBend_LR || bend_type == kBend_LL )
            y -= 10*sc;
        ht = 100*sc;
    }

    w = pFrameSet->ul->width;
    if ( wd < w*2*sc )
        xsc = (float)wd/(w*2); //wd = w*2;
    if ( ht < w*2*sc )
        ysc = (float)ht/(w*2); //ht = w*2;

    width = 2*w*sc;

    if( bend_type == kBend_LR )
    {
        // Horizontal piece
        //----------------------------------------------------------
        // top left
        display_sprite( pFrameSet->bul, x, y+ht-width, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ul, x, y+ht-width, z, xsc, ysc, color );

        // lower left
        display_sprite( pFrameSet->bll, x, y + ht - w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ll,  x, y + ht - w*ysc, z, xsc, ysc, color );

        // middle
        display_sprite( pFrameSet->xpipe, x + w*xsc, y+ht-width, z, (wd - width - (2*w-size)*xsc)/pFrameSet->xpipe->width, ysc, color );
        display_sprite( back, x + w*xsc, y + ht - width + size*ysc, z, (wd - 2*w*xsc)/back->width, (width - 2*size*ysc)/back->height, back_color);
        display_sprite( pFrameSet->xpipe, x + w*xsc, y + ht - size*ysc, z, (wd - 2*w*xsc)/pFrameSet->xpipe->width, ysc, color );

        // lower right
        display_sprite( pFrameSet->blr, x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->lr,  x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, color );

        // interior curve
        display_sprite( pFrameSet->lr, x + wd - width - (w-size)*xsc, y + ht - width - (w-size)*ysc, z+1, xsc, ysc, color );
        display_sprite( elr, x + wd - width - elr->width*xsc*1.2 + size*xsc, y + ht - width + size*ysc - elr->height*ysc*1.2, z, xsc*1.2, ysc*1.2, back_color );
        
        // Vertical Piece
        //--------------------------------------------------------------

        // top left
        display_sprite( pFrameSet->bul, x + wd - width, y, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ul, x + wd - width, y, z, xsc, ysc, color );

        // top right
        display_sprite( pFrameSet->bur, x + wd - w*xsc, y, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ur,  x + wd - w*xsc, y, z, xsc, ysc, color );

        // Middle
        display_sprite( pFrameSet->ypipe, x + wd - width, y + w*ysc, z, xsc, (ht - width - (2*w-size)*ysc )/pFrameSet->ypipe->height, color );
        display_sprite( back,  x + wd - width + size*xsc, y + w*ysc, z, (width-size*2*xsc)/back->width, (ht - width - (w-size)*ysc)/back->height, back_color);
        display_sprite( back,  x + wd - w*xsc, y + ht - width + size*ysc, z, ((w-size)*xsc)/back->width, ((w-size)*ysc)/back->height, back_color);
        display_sprite( pFrameSet->ypipe, x + wd - size*xsc, y + w*ysc, z, xsc, (ht - 2*w*ysc)/pFrameSet->ypipe->height, color );

    }
    else if( bend_type == kBend_UR )
    {
        // Horizontal piece
        //----------------------------------------------------------
        // top left
        display_sprite( pFrameSet->bul, x, y, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ul, x, y, z, xsc, ysc, color );

        // lower left
        display_sprite( pFrameSet->bll, x, y + width - w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ll,  x, y + width - w*ysc, z, xsc, ysc, color );

        // middle
        display_sprite( pFrameSet->xpipe, x + w*xsc, y, z, (wd - 2*w*xsc)/pFrameSet->xpipe->width, ysc, color );
        display_sprite( back, x + w*xsc, y + size*ysc, z, (wd - 2*w*xsc)/back->width, (width - 2*size*ysc)/back->height, back_color);
        display_sprite( pFrameSet->xpipe, x + w*xsc, y + width - size*ysc, z, (wd - width - (2*w-size)*xsc)/pFrameSet->xpipe->width, ysc, color );

        // interior curve
        display_sprite( pFrameSet->ur, x + wd + size - width - w*xsc, y + width - size*ysc, z+1, xsc, ysc, color );
        display_sprite( eur, x + size*xsc + wd - width - elr->width*xsc*1.2, y + width - size*ysc, z, xsc*1.2, ysc*1.2, back_color );

        // top right
        display_sprite( pFrameSet->bur, x + wd - w*xsc, y, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ur,  x + wd - w*xsc, y, z, xsc, ysc, color );

        // Vertical Piece
        //--------------------------------------------------------------

        // lower left
        display_sprite( pFrameSet->bll, x + wd - width, y + ht - w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ll,  x + wd - width, y + ht - w*ysc, z, xsc, ysc, color );

        // lower right
        display_sprite( pFrameSet->blr, x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->lr,  x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, color );

        // Middle
        display_sprite( pFrameSet->ypipe, x + wd - width, y + width + (w-size)*ysc, z, xsc, (ht - width - (2*w-size)*ysc )/pFrameSet->ypipe->height, color );
        display_sprite( back, x + wd - width + size*xsc, y + width - size*ysc, z, (width - 2*size*xsc)/back->width, (ht - width - (w-size)*ysc)/back->height, back_color);
        display_sprite( back, x + wd - w*xsc, y + w*ysc, z, ((w - size)*xsc)/back->width, ((w-size)*ysc)/back->height, back_color);
        display_sprite( pFrameSet->ypipe, x + wd - size*xsc, y + w*ysc, z, xsc, (ht - 2*w*ysc)/pFrameSet->ypipe->height, color );

    }
    else if( bend_type == kBend_UL )
    {
        // Vertical Piece
        //--------------------------------------------------------------
        // lower left
        display_sprite( pFrameSet->bll, x, y + ht - w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ll,  x, y + ht - w*ysc, z, xsc, ysc, color );

        // lower right
        display_sprite( pFrameSet->blr, x + w*xsc, y + ht - w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->lr,  x + w*xsc, y + ht - w*ysc, z, xsc, ysc, color );

        // middle
        display_sprite( pFrameSet->ypipe, x, y + w*ysc, z, xsc, (ht - 2*w*ysc)/pFrameSet->ypipe->height, color );
        display_sprite( back, x + size*xsc, y + w*ysc, z, (width - 2*size*xsc)/back->width, (ht - 2*w*ysc)/back->height, back_color);
        display_sprite( pFrameSet->ypipe, x + width - size*xsc, y + width + (w-size)*ysc, z, xsc, (ht - width - (2*w-size)*ysc)/pFrameSet->ypipe->height, color );

        // top left
        display_sprite( pFrameSet->bul, x, y, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ul, x, y, z, xsc, ysc, color );

        // top inside right
        display_sprite( pFrameSet->ul, x + width - size*xsc, y + width - size*ysc, z+1, xsc, ysc, color );
        display_sprite( eul, x + width - size*xsc, y + width - size*ysc, z, xsc*1.2, ysc*1.2, back_color );

        // Horizontal piece
        //----------------------------------------------------------

        // middle
        display_sprite( pFrameSet->xpipe, x + w*xsc, y, z, (wd - 2*w*xsc)/pFrameSet->xpipe->width, ysc, color );
        display_sprite( back, x + width - size*xsc, y + size*ysc, z, (wd - width - (w-size)*xsc)/back->width, (width - 2*size*ysc)/back->height, back_color);
        display_sprite( back, x + w*xsc, y + size*ysc, z, ((w-size)*xsc)/back->width, ((w-size)*ysc)/back->height, back_color);
        display_sprite( pFrameSet->xpipe, x + width + (w-size)*xsc, y + width - size*ysc, z, (wd - width - (2*w-size)*xsc)/pFrameSet->xpipe->width, ysc, color );

        // top right
        display_sprite( pFrameSet->bur, x + wd - w*xsc, y, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ur,  x + wd - w*xsc, y, z, xsc, ysc, color );

        // lower right
        display_sprite( pFrameSet->blr, x + wd - w*xsc, y + w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->lr,  x + wd - w*xsc, y + w*ysc, z, xsc, ysc, color );

    }
    else if ( bend_type == kBend_LL)
    {
        // Vertical Piece
        //--------------------------------------------------------------
        // top left
        display_sprite( pFrameSet->bul, x, y, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ul, x, y, z, xsc, ysc, color );

        // top right
        display_sprite( pFrameSet->bur, x + w*xsc, y, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ur,  x + w*xsc, y, z, xsc, ysc, color );

        // middle
        display_sprite( pFrameSet->ypipe, x, y + w*ysc, z, xsc, (ht - 2*w*ysc)/pFrameSet->ypipe->height, color );
        display_sprite( back, x + size*xsc, y + w*ysc, z, (width - 2*size*xsc)/back->width, (ht - 2*w*ysc)/back->height, back_color);
        display_sprite( pFrameSet->ypipe, x + width - size*xsc, y + w*ysc, z, xsc, (ht - width - (2*w-size)*ysc)/pFrameSet->ypipe->height, color );

        // lower left
        display_sprite( pFrameSet->bll, x, y + ht - w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ll,  x, y + ht - w*ysc, z, xsc, ysc, color );

        // top inside right
        display_sprite( pFrameSet->ll, x + width - size*xsc, y + ht - width - (w-size)*ysc, z+1, xsc, ysc, color );
        display_sprite( ell, x + width - size*xsc, y + ht - width + size*ysc - ell->height*ysc*1.2, z, xsc*1.2, ysc*1.2, back_color );

        // Horizontal piece
        //----------------------------------------------------------

        // middle
        display_sprite( pFrameSet->xpipe, x + width + (w-size)*xsc, y + ht - width, z, (wd - width - (2*w-size)*xsc)/pFrameSet->xpipe->width, ysc, color );
        display_sprite( back, x + width - size*xsc, y + ht - width + size*ysc, z, (wd - width - (w-size)*xsc)/back->width, (width - 2*size*ysc)/back->height, back_color);
        display_sprite( back, x + w*xsc, y + ht - w*ysc, z, ((w-size)*xsc)/back->width, ((w-size)*ysc)/back->height, back_color);
        display_sprite( pFrameSet->xpipe, x + w*xsc, y + ht - size*ysc, z, (wd - 2*w*xsc)/pFrameSet->xpipe->width, ysc, color );

        // top right
        display_sprite( pFrameSet->bur, x + wd - w*xsc, y + ht - width, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->ur,  x + wd - w*xsc, y + ht - width, z, xsc, ysc, color );

        // lower right
        display_sprite( pFrameSet->blr, x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, back_color);
        display_sprite( pFrameSet->lr,  x + wd - w*xsc, y + ht - w*ysc, z, xsc, ysc, color );

    }
}


int drawMMBar( char * pchTitle, F32 x, F32 y, F32 z, F32 wd, F32 sc, int open, int selectable )
{
    static AtlasTex *meatL, *meatM, *meatR, *outlineL, *outlineM, *outlineR;
    static int texBindInit=0;
    int color;
    CBox box;
    F32 tsc = sc;

    if (!texBindInit)
    {
        texBindInit = 1; 
        meatL    = atlasLoadTexture( "Header_bar_002_L" );
        meatM    = atlasLoadTexture( "Header_bar_002_MID" );
        meatR    = atlasLoadTexture( "Header_bar_002_R" );
        outlineL    = atlasLoadTexture( "Header_bar_002_frame_L" );
        outlineM    = atlasLoadTexture( "Header_bar_002_frame_MID" );
        outlineR    = atlasLoadTexture( "Header_bar_002_frame_R" );
    }

    // color, scale, mousehits
    if( open )
    {
        color = CLR_MM_BAR_OPEN;
        font_color(CLR_MM_TITLE_OPEN, CLR_MM_TITLE_OPEN);
    }
    else
    {
        color = CLR_MM_BAR_CLOSED;
        font_color(CLR_MM_TITLE_CLOSED, CLR_MM_TITLE_CLOSED);
    }

      BuildCBox(&box, x - wd/2, y - meatL->height*sc/2, wd, meatL->height*sc );
    if( mouseCollision(&box) && selectable )
    {
        sc *= 1.01;
        color = CLR_MM_BAR_MOUSEOVER; 
        font_color(CLR_MM_TITLE_LIT, CLR_MM_TITLE_LIT);
    }
    
    // draw the bar
       display_sprite( meatL,  x - wd/2, y - meatL->height*sc/2, z, sc, sc, color );
       display_sprite( outlineL,  x - wd/2, y - outlineL->height*sc/2, z, sc, sc, color );

     display_sprite( meatM,  x - wd/2 + meatL->width*sc, y - meatM->height*sc/2, z, (wd-meatL->width*sc-meatR->width*sc)/meatM->width, sc, color );
    display_sprite( outlineM,  x - wd/2 + outlineL->width*sc, y - outlineM->height*sc/2, z, (wd-outlineL->width*sc-outlineR->width*sc)/outlineM->width, sc, color );

     display_sprite( meatR,  x + wd/2 - meatR->width*sc, y - meatR->height*sc/2, z, sc, sc, color );
     display_sprite( outlineR,  x + wd/2 - meatR->width*sc, y - outlineR->height*sc/2, z, sc, sc, color );

    font( &title_12 );
       cprntEx( x, y, z+1, .9f*sc, .9f*sc, CENTER_X|CENTER_Y, pchTitle );

    return mouseClickHit( &box, MS_LEFT );
}

void drawWaterMarkFrameCorner( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 ht, F32 headerHeight, F32 contentWidth, int left_color, int right_color, int back_color, int mark)
{
    static AtlasTex * gradient, *watermark, *back, *ul, *ur, *lr, *ll;
    static int texBindInit=0;
    F32 sideOffset = 0;

    F32 size, w, watermark_scale;

    if (!texBindInit)
    {
        gradient = atlasLoadTexture("gradient");
        watermark = atlasLoadTexture("MissionArchitect_Watermark");
        back = atlasLoadTexture("white");
        ul = atlasLoadTexture("Frame_3px_10r_Background_UL");
        ur = atlasLoadTexture("Frame_3px_10r_Background_UR");
        lr = atlasLoadTexture("Frame_3px_10r_Background_LR");
        ll = atlasLoadTexture("Frame_3px_10r_Background_LL");
        texBindInit = 1;
    }

    w = ul->width*sc;
    size = 3.f*sc;

    // use standard outline
    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, back_color, 0 );

    // gradient
    //display_sprite( gradient, x + w, y + size, z, (wd - 2*w)/gradient->width, (ht-2*size)/gradient->height, CLR_WHITE);contentWidth
    display_sprite_blend( gradient, x + w, y + size+headerHeight, z, (contentWidth- w)/gradient->width, (ht-headerHeight-2*size)/gradient->height, left_color, right_color, right_color, left_color );

    // top left 
    //display_sprite( ul, x, y, z, sc, sc, left_color);

    // left 
    sideOffset = MAX(2*w, w+headerHeight);
    display_sprite( back, x + size, y + headerHeight+size, z, (w - size)/back->width, (ht - sideOffset-size)/back->height, left_color);

    // lower left
    display_sprite( ll, x, y + ht - w, z, sc, sc, left_color);

    // top right
    //display_sprite( ur, x + wd - w, y, z, sc, sc, right_color);

    // right
    //display_sprite( back,  x + wd - w, y + w, z, (w-size)/back->width, (ht - 2*w)/back->height, right_color);

    // lower right
    //display_sprite( lr, x + wd - w, y + ht - w, z, sc, sc, right_color);

    // watermark
    if( mark )
    {
        watermark_scale = MIN( (wd - w*2)/watermark->width, (ht-w*2)/watermark->height );
        display_sprite(watermark, x + (wd - watermark->width*watermark_scale)/2, y + (ht - watermark->height*watermark_scale)/2, z, watermark_scale, watermark_scale, 0xffffff22 );  
    }
}

void drawWaterMarkFrame_ex( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 ht, int left_color, int right_color, int back_color, int mark)
{
    static AtlasTex * gradient, *watermark, *back, *ul, *ur, *lr, *ll;
    static int texBindInit=0;

    F32 size, w, watermark_scale;

    if (!texBindInit)
    {
        gradient = atlasLoadTexture("gradient");
        watermark = atlasLoadTexture("MissionArchitect_Watermark");
        back = atlasLoadTexture("white");
        ul = atlasLoadTexture("Frame_3px_10r_Background_UL");
        ur = atlasLoadTexture("Frame_3px_10r_Background_UR");
        lr = atlasLoadTexture("Frame_3px_10r_Background_LR");
        ll = atlasLoadTexture("Frame_3px_10r_Background_LL");
        texBindInit = 1;
    }

    w = ul->width*sc;
    size = 3.f*sc;

    // use standard outline
    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, back_color, 0 );

    // gradient
    //display_sprite( gradient, x + w, y + size, z, (wd - 2*w)/gradient->width, (ht-2*size)/gradient->height, CLR_WHITE);
    display_sprite_blend( gradient, x + w, y + size, z, (wd - 2*w)/gradient->width, (ht-2*size)/gradient->height, left_color, right_color, right_color, left_color );

     // top left 
    display_sprite( ul, x, y, z, sc, sc, left_color);

     // left 
    display_sprite( back, x + size, y + w, z, (w - size)/back->width, (ht - 2*w)/back->height, left_color);

    // lower left
    display_sprite( ll, x, y + ht - w, z, sc, sc, left_color);

    // top right
    display_sprite( ur, x + wd - w, y, z, sc, sc, right_color);

    // right
    display_sprite( back,  x + wd - w, y + w, z, (w-size)/back->width, (ht - 2*w)/back->height, right_color);

    // lower right
    display_sprite( lr, x + wd - w, y + ht - w, z, sc, sc, right_color);

    // watermark
    if( mark )
    {
        watermark_scale = MIN( (wd - w*2)/watermark->width, (ht-w*2)/watermark->height );
          display_sprite(watermark, x + (wd - watermark->width*watermark_scale)/2, y + (ht - watermark->height*watermark_scale)/2, z, watermark_scale, watermark_scale, 0xffffff22 );  
    }
}

int drawTextEntryFrame( F32 x, F32 y, F32 z, F32 wd, F32 ht, F32 sc, int selected )
{
    static AtlasTex * left, *right, *back, *ul, *ur, *lr, *ll, *lm, *um, *white;
    static int texBindInit=0;
    CBox box;
       int back_color = 0x122323ff&0xffffffcc;
    
    if( selected == 1 )
        back_color = CLR_MM_TEXT_BACK_LIT&0xffffffcc;
     if( selected >= 2 )
        back_color = 0x333333ff;

    if (!texBindInit)
    {
        back = atlasLoadTexture("TextField_corner"); 
        white = atlasLoadTexture("white"); 

        left = atlasLoadTexture("TextField_L_mid_shadow");
        right = atlasLoadTexture("TextField_R_mid_shadow");

        ul = atlasLoadTexture("TextField_L_corner_shadow");
        um = atlasLoadTexture("TextField_top_mid_shadow");
        ur = atlasLoadTexture("TextField_R_corner_shadow");

        ll = atlasLoadTexture("TextField_L_corner_highlight");
        lm = atlasLoadTexture("TextField_btm_mid_highlight");
        lr = atlasLoadTexture("TextField_R_corner_highlight");

        texBindInit = 1;
    }

    BuildCBox( &box, x, y, wd, ht );
      if( mouseCollision(&box) && selected <= 2)
    {
        back_color = CLR_MM_TEXT_BACK_MOUSEOVER&0xffffffcc;
        if(selected == 2)
            back_color = 0x555555ff;
    }

    //top
       display_spriteFlip( back, x, y, z, sc, sc, back_color, 0 );
       display_sprite( ul, x, y, z, sc, sc, CLR_WHITE );

     display_sprite( white, x+ul->width*sc, y, z, (wd-(ul->width*sc+ur->width*sc))/white->width, (ul->height*sc)/white->height, back_color );
    display_sprite( um, x+ul->width*sc, y, z, (wd-ul->width*sc-ur->width*sc)/um->width, sc, CLR_WHITE );
    
    display_spriteFlip( back, x + wd - ur->width*sc, y, z, sc, sc, back_color, FLIP_HORIZONTAL );
    display_sprite( ur, x + wd - ur->width*sc, y, z, sc, sc, CLR_WHITE );
     
    // side
       display_sprite( left, x, y+ul->height*sc, z, sc, (ht - ul->height*sc - ll->height*sc)/left->height, CLR_WHITE );
      display_sprite( white, x, y + ul->height*sc, z, (wd)/white->width, (ht - ul->height*sc - ll->height*sc)/white->height, back_color );
    //display_sprite( back, x, y, z, wd/back->width, ht/back->height, CLR_WHITE );
    display_sprite( right, x + wd - right->width*sc, y+ur->height*sc, z, sc, (ht - ur->height*sc - lr->height*sc)/right->height, CLR_WHITE );

    // Bottom
    display_spriteFlip( back, x, y + ht - ll->height*sc, z, sc, sc, back_color, FLIP_VERTICAL );
      display_sprite( ll, x, y + ht - ll->height*sc, z, sc, sc, CLR_WHITE );
    
    display_sprite( white, x + lr->width*sc, y + ht - ll->height*sc, z, (wd-ll->width*sc-lr->width*sc)/white->width, ll->height*sc/white->height, back_color );
       display_sprite( lm, x + lr->width*sc, y + ht - lm->height*sc, z, (wd-ll->width*sc-lr->width*sc)/lm->width, sc, CLR_WHITE );
    
    display_spriteFlip( back, x + wd - lr->width*sc, y + ht - lr->height*sc, z, sc, sc, back_color, FLIP_BOTH );
    display_sprite( lr, x + wd - lr->width*sc, y + ht - lr->height*sc, z, sc, sc, CLR_WHITE );

    return mouseClickHit(&box, MS_LEFT);
}


int drawMMButton( char * txt, char * icon1, char * icon2, F32 cx, F32 cy, F32 z, F32 wd, F32 sc, int flags, int ocolor )
{
    static AtlasTex * large_end, *large_end_h, *large_end_s, *large_mid, *large_mid_h, *large_mid_s;
    static AtlasTex * small_end, *small_end_h, *small_end_s, *small_mid, *small_mid_h, *small_mid_s;
    static int texBindInit=0;

    AtlasTex * end, *end_h, *end_s, *mid, *mid_h, *mid_s;
    int flip=0;
    F32 xsc = sc, icon_space = 0;
     CBox box;
    AtlasTex *icon=0, *icon_out=0;

    int color, color_lit, txt_color, icon_color, border_color, icon_mouseOver_color;

    color = CLR_MM_BUTTON;
    color_lit = CLR_MM_BUTTON_LIT;
    txt_color = CLR_MM_TITLE_OPEN;
    border_color = CLR_MM_BACK_DARK;
    icon_color = 0x0F1918FF;
    icon_mouseOver_color = 0x55784DFF;

    if( flags&MMBUTTON_COLOR )
    {
        color = ocolor&0xffffff88;
        color_lit = ocolor;
        txt_color = CLR_WHITE;
        border_color = 0x111111ff;
        icon_color = 0;
        icon_mouseOver_color = 0;
    }

    if( flags&MMBUTTON_ERROR )
    {
        color = CLR_MM_BUTTON_ERROR;
        txt_color = CLR_MM_BUTTON_ERROR_TEXT;
        border_color = CLR_MM_ERROR_DARK;
    }
    if( flags&MMBUTTON_LOCKED || flags&MMBUTTON_GRAYEDOUT)
    {
        color = 0x888888ff;
        txt_color = 0xaaaaaaff;
        border_color = 0x444444ff;
    }


     if (!texBindInit)
    {
        large_end = atlasLoadTexture("genericbutton_002_end"); 
        large_end_h = atlasLoadTexture("genericbutton_002_highlight_end");
        large_mid_h = atlasLoadTexture("genericbutton_002_highlight_mid");
        large_mid = atlasLoadTexture("genericbutton_002_mid");
        large_end_s = atlasLoadTexture("genericbutton_002_shadow_end");
        large_mid_s = atlasLoadTexture("genericbutton_002_shadow_mid");

        small_end = atlasLoadTexture("genericbutton_003_end"); 
        small_end_h = atlasLoadTexture("GenericButton_003_end_highlight");
        small_end_s = atlasLoadTexture("GenericButton_003_end_shadow");
        small_mid = atlasLoadTexture("genericbutton_003_mid");
        small_mid_h = atlasLoadTexture("genericbutton_003_mid_highlight");        
        small_mid_s = atlasLoadTexture("genericbutton_003_mid_shadow");
        texBindInit = 1;
    }

       if( flags&MMBUTTON_SMALL )
    {
        end = small_end;
        end_h = small_end_h;
        end_s = small_end_s;
        mid = small_mid;
        mid_h = small_mid_h;
        mid_s = small_mid_s;
    }
    else
    {
        end = large_end;
        end_h = large_end_h;
        end_s = large_end_s;
        mid = large_mid;
        mid_h = large_mid_h;
        mid_s = large_mid_s;
    }


       BuildCBox(&box, cx-wd/2, cy-end->height*sc/2, wd, end->height*sc);
      if( mouseCollision(&box) && !(flags&MMBUTTON_LOCKED) )
    {
        if(!(flags&MMBUTTON_GRAYEDOUT))
        {
            color = color_lit;
            icon_color = icon_mouseOver_color;
            if( flags&MMBUTTON_ERROR )
            {
                color = CLR_MM_BUTTON_ERROR_LIT;
                txt_color = CLR_MM_BUTTON_ERROR_TEXT_LIT;
            }
        }

        if( isDown(MS_LEFT) )
            flip = 1;
    }

    // left
       display_sprite( end, cx - wd/2, cy - end->height*sc/2, z, sc, sc, color ); 
     if( flip || (flags&MMBUTTON_DEPRESSED) )
    {
         display_spriteUV( end_s, cx - wd/2, cy - end->height*sc/2, z, sc, sc, CLR_WHITE, 0, 1, 1, 0 ); 
        display_spriteUV( end_h, cx - wd/2, cy + end->height*sc/2 - end_s->height*sc, z, sc, sc, CLR_WHITE, 0, 1, 1, 0 ); 
    }
    else
    {
           display_sprite( end_h, cx - wd/2, cy - end->height*sc/2, z, sc, sc, CLR_WHITE ); 
        display_sprite( end_s, cx - wd/2, cy + end->height*sc/2 - end_s->height*sc, z, sc, sc, CLR_WHITE ); 
    }

    // middle
       display_sprite( mid, cx - wd/2 + end->width*sc, cy - mid->height*sc/2, z, (wd-end->width*2*sc)/mid->width, sc, color );
    
     if( flip || (flags&MMBUTTON_DEPRESSED) )
    {
        display_spriteUV( mid_s, cx - wd/2 + end->width*sc, cy - mid->height*sc/2, z, (wd-end->width*2*sc)/mid_h->width, sc, CLR_BLACK, 0, 1, 1, 0 ); 
        display_spriteUV( mid_h, cx - wd/2 + end->width*sc, cy + mid->height*sc/2 - mid_s->height*sc, z, (wd-end->width*2*sc)/mid_s->width, sc, CLR_WHITE, 0, 1, 1, 0 ); 
    }
    else
    {
         display_sprite( mid_h, cx - wd/2 + end->width*sc, cy - mid->height*sc/2, z, (wd-end->width*2*sc)/mid_h->width, sc, CLR_WHITE ); 
        display_sprite( mid_s, cx - wd/2 + end->width*sc, cy + mid->height*sc/2 - mid_s->height*sc, z, (wd-end->width*2*sc)/mid_s->width, sc, CLR_BLACK ); 
    }

    // end
    display_spriteUV( end, cx + wd/2 - end->width*sc, cy - end->height*sc/2, z, sc, sc, color, 1, 0, 0, 1 ); 
    if( flip || (flags&MMBUTTON_DEPRESSED) )
    {
        display_spriteUV( end_s, cx + wd/2 - end->width*sc, cy - end->height*sc/2, z, sc, sc, CLR_WHITE, 1, 1, 0, 0 ); 
        display_spriteUV( end_h, cx + wd/2 - end->width*sc, cy + end->height*sc/2 - end_s->height*sc, z, sc, sc, CLR_WHITE, 1, 1, 0, 0 );  
    }
    else
    {
        display_spriteUV( end_h, cx + wd/2 - end->width*sc, cy - end->height*sc/2, z, sc, sc, CLR_WHITE, 1, 0, 0, 1 ); 
        display_spriteUV( end_s, cx + wd/2 - end->width*sc, cy + end->height*sc/2 - end_s->height*sc, z, sc, sc, CLR_WHITE, 1, 0, 0, 1 );  
    }

    if( flip || (flags&MMBUTTON_DEPRESSED) )
        cy += 2*sc;

    if( icon1 )
        icon = atlasLoadTexture(icon1);
    if( icon2 )
        icon_out = atlasLoadTexture(icon2);

      if( icon )  
    {
        F32 icon_out_h, icon_out_w, tx;
        if (icon_out)
        {
            icon_out_h = icon_out->height*sc;
            icon_out_w = icon_out->width*sc;
        }
        else
        {
            icon_out_h = icon->height*sc;
            icon_out_w = icon->width*sc;
        }
        
         if(flags&MMBUTTON_ICONRIGHT)
         {
              tx = cx + wd/2 - icon_out_w - end->width*sc/2;
            icon_space = icon_out_w + end->width*sc/2;
            if (flags&MMBUTTON_ICONALIGNTOTEXT)
            {
                int textWd;
                textWd = str_wd(&title_12, sc, sc, txt);
                tx -= MAX(30*sc, wd/2.0 - (textWd/2)) - (end->width*sc);
            }
        }
        else
        {
            tx = cx - wd/2 + end->width*sc - icon_out_w;
            if (flags&MMBUTTON_ICONALIGNTOTEXT)
            {
                int textWd;
                textWd = str_wd(&title_12, sc, sc, txt);
                tx += MAX(30*sc, wd/2.0 - (textWd/2)) - (end->width*sc);
            }
        }
         if( flags&MMBUTTON_CREATE )
        {
            border_color = CLR_WHITE;
            tx += 20*sc;
        }

        if(!(flags&MMBUTTON_ICONRIGHT))
        {
            if (flags&MMBUTTON_ICONALIGNTOTEXT)
            {
                icon_space = icon_out_w;
            }
            else
            {
                icon_space = (tx + icon->width*sc) - (cx - wd/2);
            }
            cx += icon_space/2;
        }

        display_sprite( icon, tx, cy - icon->height*sc/2, z, sc, sc, (flags & MMBUTTON_USEOVERRIDE_COLOR) ? icon_color : txt_color  );
        if( icon2 )
            display_sprite( icon_out, tx, cy - icon_out_h/2, z, sc, sc, border_color  );
    }

      font(&title_12);
    {
        int textWd;
        int writeAbleSpace;
        textWd = str_wd(&title_12, sc, sc, txt);
           writeAbleSpace = wd - (28*sc) - icon_space;
         if (textWd > writeAbleSpace)
        {
            xsc *= writeAbleSpace*1.0f/textWd;
        }
    }
     font_color(txt_color, txt_color);
    cprntEx( cx, cy, z, xsc, sc, CENTER_X|CENTER_Y, txt );

     return !(flags&MMBUTTON_LOCKED) && mouseClickHit(&box, MS_LEFT);
}

int drawMMArrow( F32 cx, F32 cy, F32 z, F32 sc, int dir, int lit )
{
    static AtlasTex * button, *button_h, *button_s, *arrow_in, *arrow_out;
    static int texBindInit=0;
     int flip = 0;
    int color = CLR_MM_BUTTON;
    int color_arrow = CLR_MM_TITLE_OPEN;
    int xoff = 0;
    F32 arrow_off = 0;

    CBox box;

     if (!texBindInit)
    {
        button = atlasLoadTexture("arrowbutton");
        button_h = atlasLoadTexture("arrowbutton_highlight");
        button_s = atlasLoadTexture("arrowbutton_shadow");
        arrow_in = atlasLoadTexture("arrow_inside");
        arrow_out = atlasLoadTexture("arrow_outline");
        texBindInit = 1;
    }

     BuildCBox( &box, cx - button->width*sc/2, cy - button->height*sc/2, button->width*sc, button->height*sc );
      
      if( lit || mouseCollision(&box) )
    {
        color = CLR_MM_BUTTON_LIT;
        color_arrow = CLR_MM_TITLE_LIT;
        if( isDown(MS_LEFT) )
        {
            flip = FLIP_VERTICAL;
            arrow_off = 2;
        }
    }
    cy -= 2*sc;
     display_sprite( button, cx - button->width*sc/2, cy - button->height*sc/2, z, sc, sc, color );

       if( flip )
    {
           display_spriteFlip( button_s, cx - button_h->width*sc/2, cy - button->height*sc/2 + 3*sc, z, sc, sc, CLR_WHITE, flip );
        display_spriteFlip( button_h, cx - button_s->width*sc/2, cy + button->height*sc/2 - button_s->height*sc + 3*sc, z, sc, sc, CLR_WHITE, flip );
    }
    else
    {
        display_sprite( button_h, cx - button_h->width*sc/2, cy - button->height*sc/2, z, sc, sc, CLR_WHITE );
        display_sprite( button_s, cx - button_s->width*sc/2, cy + button->height*sc/2 - button_s->height*sc + 1*sc, z, sc, sc, CLR_WHITE );
    }

      if( dir )
    {    
          flip = 0;
        xoff = 0*sc;
    }
    else
    {
        flip = FLIP_HORIZONTAL;
        xoff = 0*sc;
    }

       display_spriteFlip( arrow_in, xoff + cx - arrow_in->width*sc/2, cy - arrow_in->height*sc/2 + arrow_off*sc, z+1, sc, sc, color_arrow, flip );
       display_spriteFlip( arrow_out, xoff + cx - arrow_out->width*sc/2, cy - arrow_out->height*sc/2+ arrow_off*sc, z, sc, sc, CLR_MM_BACK_DARK, flip );

    return mouseClickHit(&box, MS_LEFT);
}


int drawMMArrowButton( char * pchText, F32 x, F32 y, F32 z, F32 sc, F32 wd, int dir )
{
    AtlasTex *gradient = atlasLoadTexture("white");
    AtlasTex *cap = atlasLoadTexture("barfaded");
    AtlasTex *highlight = atlasLoadTexture("barfaded_highlight");
    AtlasTex *button = atlasLoadTexture("arrowbutton");
    CBox box;
    int hit = 0;
    int fade_color = 0x4b8d81ff;
    int text_color = CLR_MM_TITLE_OPEN;
    F32 ht = (button->height)*sc, txt_off = 0;

       BuildCBox(&box, x, y, wd, button->height*sc );
    if( mouseCollision(&box) )
    {
        hit = 1;
        fade_color = 0x6bada1ff;
        text_color = CLR_MM_TITLE_LIT;
        if( isDown(MS_LEFT) )
            txt_off = 2*sc;
    }


    font( &title_12 );
    font_color( text_color, text_color ); 

       if( dir )
    {    
          F32 strwd = str_wd( font_grp, sc, sc, "%s", pchText );
        display_spriteFlip( cap, x + wd - cap->width*sc, y + button->height*sc/2 - cap->height*sc/2, z, sc, sc, fade_color, FLIP_HORIZONTAL );
//        display_sprite_blend( gradient, x, y, z, (wd - button->width*sc/2)/gradient->width, ht/gradient->height, fade_color&0xffffff00, fade_color, fade_color, fade_color&0xffffff00 );
          drawMMArrow(x + wd - button->width*sc/2 + 1*sc, y + button->height*sc/2, z, sc, 1, hit );
        cprntEx( x + wd - 10*sc - button->width*sc - strwd, y + button->height*sc/2 + txt_off, z, sc, sc, CENTER_Y, "%s", pchText );
    }
    else
    {    
           display_sprite( cap, x, y + button->height*sc/2 - cap->height*sc/2, z, sc, sc, fade_color );
         drawMMArrow(x + button->width*sc/2 - 1*sc, y + button->height*sc/2, z, sc, 0, hit );
        cprntEx( x + button->width*sc + 10*sc, y + button->height*sc/2 + txt_off, z, sc, sc, CENTER_Y, "%s", pchText );
    }
    
    return mouseClickHit(&box, MS_LEFT);
}

void drawMMProgressBar( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 percent )
{
    static AtlasTex *end, *left_s, *left_h, *mid, *mid_h, *mid_s, *right_h, *right_s, *fill; 
    static int texBindInit = 0;
    UIBox box;
       wd = 479*sc;

    if(!texBindInit )
    {
        left_s = atlasLoadTexture("Progressbar_groove_bar_L_shadow.tga"); 
        end = atlasLoadTexture("Progressbar_groove_end.tga"); 
        left_h = atlasLoadTexture("Progressbar_groove_L_highlight.tga"); 
        mid = atlasLoadTexture("Progressbar_groove_mid.tga");  
        mid_h = atlasLoadTexture("Progressbar_groove_mid_highlight.tga"); 
        mid_s = atlasLoadTexture("Progressbar_groove_mid_shadow.tga");  
        right_h = atlasLoadTexture("Progressbar_groove_R_highlight.tga"); 
        right_s = atlasLoadTexture("Progressbar_groove_R_shadow.tga");  
        fill = atlasLoadTexture("ProgressBar_fill.tga"); 
        texBindInit = 0;
    }

      display_sprite( end, x - wd/2, y, z,sc, sc, CLR_MM_BACK_DARK );
     display_sprite( mid, x - wd/2 + end->width*sc, y, z, (wd - 2*end->width*sc)/mid->width, sc, CLR_MM_BACK_DARK );
    display_spriteFlip( end, x + wd/2 - end->width*sc, y, z,sc, sc, CLR_MM_BACK_DARK, FLIP_HORIZONTAL );

    // left
    display_sprite( left_h, x - wd/2, y+(end->height-left_h->height)*sc, z,sc, sc, 0xffffff44 );
    display_sprite( left_s, x - wd/2, y, z,sc, sc, CLR_WHITE );

    // mid
    display_sprite( mid_s, x - wd/2 + left_s->width*sc, y, z, (wd - left_s->width*sc - right_s->width*sc)/mid_s->width, sc, CLR_WHITE );
     display_sprite( mid_h, x - wd/2 + left_h->width*sc, y + mid->height*sc - mid_h->height*sc, z, (wd - left_h->width*sc - right_h->width*sc)/mid_h->width, sc, 0xffffff44 );
    
    // right
    display_sprite( right_h, x + wd/2 - right_h->width*sc, y+ (end->height-right_h->height)*sc, z,sc, sc, 0xffffff44 );
    display_sprite( right_s, x + wd/2 - right_s->width*sc, y , z,sc, sc, CLR_WHITE );

     uiBoxDefine(&box, x - wd/2, 0, wd*percent, 10000);
    clipperPushRestrict(&box);
       display_sprite( fill, x - wd/2 + 6*sc, y + 2*sc, z, sc, sc, CLR_WHITE );
    clipperPop();

}

int drawMMFrameTabs( F32 x, F32 y, F32 z, F32 sc, F32 wd, F32 ht, F32 tab_ht, int style, char ***ppTabNames, int selected )
{
    FrameSet *pTab = getFrameSet(kFrameStyle_3D, PIX2, R27, 0);
    FrameSet *pSet = getFrameSet(kFrameStyle_3D, PIX2, R16, 0);

    static AtlasTex * tab_behind, *tab_end, *tab_mid, *tab_frame_corner, *tab_frame_end, *tab_frame_mid, *end, *mid; 
    static int init;

    F32 *fWd=0;
    F32 tx;
     F32 spacer = 25*sc, total_wd=0;
    int i, iCnt = eaSize(ppTabNames), tab_color;
     AtlasTex *back = atlasLoadTexture("white");
    CBox cbox;

    int color, back_color, text_color, text_color_lit, text_color_selected;

    if( style == kStyle_Architect )
    {
        color = 0xffffff22;
        back_color = 0x00ffff11;
        text_color_selected = CLR_MM_TITLE_CLOSED;
        text_color = CLR_MM_TITLE_OPEN;
        text_color_lit = CLR_MM_TITLE_LIT;
    }
    else
    {
          color = 0x44444488;
           back_color = 0x44444488;
        text_color_lit = CLR_WHITE;
        text_color = CLR_AH_TEXT;
        text_color_selected = CLR_AH_TEXT_SELECTED;
    }
    tab_color = back_color;


    if( !init )
    {
        tab_behind = atlasLoadTexture( "TabOverlap_24px_behind");
        tab_end = atlasLoadTexture( "TabOverlap_24px_end");
        tab_mid = atlasLoadTexture( "TabOverlap_24px_mid");
        tab_frame_corner = atlasLoadTexture( "TabOverlap_24px_frame_corner.");
        tab_frame_end = atlasLoadTexture( "TabOverlap_24px_frame_end");
        tab_frame_mid = atlasLoadTexture( "TabOverlap_24px_frame_mid");

        end = atlasLoadTexture("Tab_end.tga");
        mid = atlasLoadTexture("Tab_mid.tga");

    }

     //tab_ht = tab_frame_end->height*sc;
    tab_ht = end->height*sc;
    font(&title_12);
    font_outl(0);
    
     for( i = 0; i < iCnt; i++ )
    {
        F32 twd = str_wd(font_grp, sc, sc, (*ppTabNames)[i]);
        total_wd += twd + spacer;
        eafPush(&fWd, twd );
    }

    if( total_wd > wd )
    {
        spacer = (total_wd - wd)/iCnt;
        total_wd = wd;
    }

     tx = x;
       for( i = 0; i < iCnt; i++ )
    {

        if( i == selected )
        {
            font_color( text_color_selected, text_color_selected );
             display_sprite( pSet->xpipe, x, y, z, (tx-x)/pSet->xpipe->width, sc, color );
            display_sprite( back, tx, y, z, (fWd[i]+spacer)/back->width, PIX2*sc/back->height, back_color); 
             display_sprite( pSet->xpipe, tx + fWd[i] + spacer, y, z, (wd - (tx+fWd[i]+spacer-x) - pSet->ur->width*sc + PIX3*sc)/pSet->xpipe->width, sc, color );
        }
        else
            font_color( text_color, text_color );

         tab_color = back_color;
        BuildCBox(&cbox, tx, y - tab_ht, fWd[i] + 20*sc, tab_ht-1*sc);

        if( mouseCollision(&cbox) )
        {
            tab_color = 0x00ffff44;
            font_color( text_color_lit, text_color_lit );
        }

          display_sprite( end, tx + 1*sc, y - tab_ht, z, sc, sc, tab_color );
        display_sprite( mid, tx + end->width*sc + 1*sc, y - tab_ht, z, (fWd[i] + spacer - end->width*2*sc - 2*sc)/mid->width, sc, tab_color );
        display_spriteFlip( end, tx + fWd[i] + spacer - end->width*sc - 1*sc, y - tab_ht, z, sc, sc, tab_color, FLIP_HORIZONTAL );
        
         cprnt( tx + (fWd[i] + spacer)/2, y-4*sc, z, sc, sc, (*ppTabNames)[i] );

        if( mouseClickHit( &cbox, MS_LEFT ) )
            selected = i;

        tx += fWd[i] + spacer;
    }

    tx = x;

       display_sprite( back, x + PIX2*sc, y + PIX2*sc, z-1, (wd - pSet->ur->width*sc - PIX2*sc)/back->width, (pSet->ur->height*sc- PIX2*sc)/back->height, back_color );
      display_sprite( pSet->ypipe, x, y+PIX2*sc, z+1, sc, (pSet->ur->height*sc-PIX2*sc)/pSet->ypipe->height, color );
    display_sprite( pSet->bur, x + wd - pSet->ur->width*sc, y, z, sc, sc, back_color );
    display_sprite( pSet->ur, x + wd - pSet->ur->width*sc, y, z, sc, sc, color );
     drawSectionFrame( PIX2, R16, x, y + pSet->ur->height*sc, z, wd, ht - pSet->ur->height*sc, sc, color, back_color, kFramePart_MidL|kFramePart_MidC|kFramePart_MidR|kFramePart_BotL|kFramePart_BotC|kFramePart_BotR);


    eafDestroy(&fWd);
    font_outl(1);
     return selected;
}

int drawMMCheckBox( F32 x, F32 y, F32 z, F32 sc, const char *pchText, int *val, int txt_color, int button_color, int mouse_over_txt_color, int mouse_over_button_color, F32 *rwd, F32 *rht )
{
    static AtlasTex * base, *fill, *fill_highlight, *ring, *ring_highlight;
    static int texBindInit;
    F32 wd;
    CBox box;

    if(!texBindInit )
    {
        base = atlasLoadTexture("Bullet_base.tga");
        fill = atlasLoadTexture("Bullet_fill.tga"); 
        fill_highlight = atlasLoadTexture("Bullet_fill_highlight.tga"); 
        ring = atlasLoadTexture("Bullet_ring.tga"); 
        ring_highlight = atlasLoadTexture("Bullet_ring_highlight.tga");
        texBindInit = 1;
    }

    font( &game_12 );
    wd = ring->width*sc + R10*sc + str_wd(&game_12, sc, sc, "%s", pchText );

    BuildCBox(&box, x, y, wd, ring->height*sc);
    if(rwd)
        *rwd = wd;
    if(rht)
        *rht = ring->height*sc;
    if( mouseCollision(&box) )
    {
        txt_color = mouse_over_txt_color;
        button_color = mouse_over_button_color;
    }

     display_sprite(base, x, y, z, sc, sc, CLR_BLACK);
    display_sprite(ring, x, y, z, sc, sc, button_color);
    display_sprite(ring_highlight, x, y, z, sc, sc, CLR_WHITE);

     if( val && *val )
    {
        display_sprite(fill, x, y, z, sc, sc, button_color);
        display_sprite(fill_highlight, x, y, z, sc, sc, CLR_WHITE);
    }

    font_color(txt_color,txt_color);
     cprntEx( x + ring->width*sc +5*sc, y + ring->height*sc/2, z, sc, sc, CENTER_Y, "%s", pchText );

    if( mouseClickHit(&box, MS_LEFT) )
    {
        if( val )
            *val = !(*val);
        return 1;
    }

    return 0;
}



int drawMMBulbButton( char *pchText, char * pchIcon, char * pchIconGlow, F32 x, F32 y, F32 wd, F32 z, F32 sc, int flags )
{
    static AtlasTex * bulb, *bulb_s, *bulb_h, *bar_end, *bar_end_h, *bar_end_s, *meat, *meat_h, *meat_s, *ring, *ring_h, *ring_s;
    AtlasTex * icon = atlasLoadTexture(pchIcon), *glowIcon=0, *ring_base;
    static int texBindInit;
    F32 ht, off = 0, flip = 0;
    CBox box;
     int color = CLR_MM_BUTTON, txt_color = CLR_MM_TITLE_OPEN, icon_color = CLR_MM_BACK_DARK;

      if(!texBindInit )
    {
        bulb = atlasLoadTexture("BulbBar_bulb_end");
        bulb_s = atlasLoadTexture("BulbBar_bulb_end_shadow");
        bulb_h = atlasLoadTexture("BulbBar_bulb_highlight"); 
        bar_end = atlasLoadTexture("BulbBar_end");
        bar_end_h = atlasLoadTexture("BulbBar_end_highlight"); 
        bar_end_s = atlasLoadTexture("BulbBar_end_shadow"); 
        meat = atlasLoadTexture("BulbBar_mid"); 
        meat_h = atlasLoadTexture("BulbBar_mid_highlight");
        meat_s = atlasLoadTexture("BulbBar_mid_shadow");
        ring = atlasLoadTexture("BulbBar_ring");
        ring_h = atlasLoadTexture("BulbBar_ring_highlight"); 
        ring_s = atlasLoadTexture("BulbBar_ring_shadow");
        texBindInit = 1;
    }
    ring_base = atlasLoadTexture("BulbHeaderBar_ring_base");

    ht = bulb->height*sc;
    BuildCBox(&box, x - wd/2, y - ht/2, wd, ht );

    if(flags&MMBUTTON_LOCKED)
    {
          color = 0x888888ff;
        txt_color = 0x888888ff;
        icon_color = 0xccccccff;
    }
       else if( mouseCollision(&box))
    {
        // color change
        color = CLR_MM_BUTTON_LIT;
        txt_color = CLR_MM_TITLE_LIT;
        icon_color = 0x52754bff;
        glowIcon = atlasLoadTexture(pchIconGlow);

        if( isDown(MS_LEFT) )
        {
            off = 2*sc;
            // depress
            flip = FLIP_VERTICAL;
        }
    }

    // first draw the bar
      display_sprite( bar_end, x + wd/2 - bar_end->width*sc, y - ht/2, z, sc, sc, color );
     display_spriteFlip( bar_end_s, x + wd/2 - bar_end->width*sc, y - ht/2, z, sc, sc, CLR_WHITE, flip );
    display_spriteFlip( bar_end_h, x + wd/2 - bar_end->width*sc, y - ht/2, z, sc, sc, CLR_WHITE, flip );

    display_sprite( meat, x - wd/2 + bulb->width*sc/2, y - ht/2, z, (wd - (bar_end->width+bulb->width/2)*sc)/meat->width, sc, color );
    display_spriteFlip( meat_s, x - wd/2 + bulb->width*sc/2, y - ht/2, z, (wd - (bar_end->width+bulb->width/2)*sc)/meat->width, sc, CLR_WHITE, flip );
    display_spriteFlip( meat_h, x - wd/2 + bulb->width*sc/2, y - ht/2, z, (wd - (bar_end->width+bulb->width/2)*sc)/meat->width, sc, CLR_WHITE, flip );

     // now the bulb
       display_sprite( bulb, x - wd/2, y - ht/2, z, sc, sc, color );
    display_spriteFlip( bulb_s, x - wd/2, y - ht/2, z, sc, sc, CLR_WHITE, flip );
    display_spriteFlip( bulb_h, x - wd/2, y - ht/2, z, sc, sc, CLR_WHITE, flip );

      display_spriteFlip( ring_s, x - wd/2, y - ht/2 + off, z, sc, sc, CLR_WHITE, 0 );
    display_spriteFlip( ring_h, x - wd/2, y - ht/2 + off, z, sc, sc, CLR_WHITE, 0 );
     display_sprite( ring_base, x - wd/2 - 2*sc, y - ring_base->height/2 + off, z, sc, sc, color );
    display_sprite( ring, x - wd/2, y - ht/2 + off, z, sc, sc, 0x2c453fff );

    // Last the Icon
      if( glowIcon )
         display_sprite( glowIcon, x + (-wd + bulb->width*sc - icon->width*sc)/2, y - icon->height*sc/2 + off, z, sc, sc, 0xddffadff );
      if( icon )
        display_sprite( icon, x + (-wd + bulb->width*sc - icon->width*sc)/2, y - icon->height*sc/2 + off, z, sc, sc, icon_color );

    font_color( txt_color, txt_color );
    cprntEx( x, y + off, z, sc, sc, CENTER_X|CENTER_Y, "%s", pchText );


    return !(flags&MMBUTTON_LOCKED) && mouseClickHit(&box, MS_LEFT);
}

int drawMMBulbBar( char *pchText, char * pchIcon, char * pchIconGlow, F32 cx, F32 y, F32 wd, F32 z, F32 sc, int open, int flags, F32 *popout )
{
    static AtlasTex * bulb, *bulb_s, *bulb_out, *bar_end, *bar_end_h, *bar_end_s, *meat, *meat_s, *ring, *ring_base, *ring_h, *ring_s, *outline_end, *outline_mid, *dropshadow, *shadow;
    AtlasTex * icon = pchIcon?atlasLoadTexture(pchIcon):0, *iconGlow=0;
    static int texBindInit;
    F32 x = cx - wd/2, ht, off = 0;
    CBox box;
    int color = CLR_MM_BUTTON, txt_color = CLR_MM_TITLE_OPEN, icon_color = CLR_MM_BACK_DARK;
    int retval = 0, bar_hit =0;

    if(!texBindInit )
    {
        bulb = atlasLoadTexture("BulbHeaderBar_bulb_end");
        bulb_s = atlasLoadTexture("BulbHeaderBar_bulb_shadow");
        bulb_out = atlasLoadTexture("BulbHeaderBar_bulb_outline"); 
        
        bar_end = atlasLoadTexture("BulbHeaderBar_end");
        bar_end_h = atlasLoadTexture("BulbBar_end_highlight"); 
        bar_end_s = atlasLoadTexture("BulbHeaderBar_shadow"); 

        meat = atlasLoadTexture("BulbHeaderBar_mid"); 
        meat_s = atlasLoadTexture("BulbHeaderBar_mid_shadow");

        ring = atlasLoadTexture("BulbHeaderBar_ring");
        ring_base = atlasLoadTexture("BulbHeaderBar_ring_base");
        ring_h = atlasLoadTexture("BulbHeaderBar_ring_highlight"); 
        ring_s = atlasLoadTexture("BulbHeaderBar_ring_shadow");

        outline_end = atlasLoadTexture("BulbHeaderBar_ouline_end");
        outline_mid = atlasLoadTexture("BulbHeaderBar_ouline_mid");
        dropshadow = atlasLoadTexture("BulbHeaderBar_dropshadow");
        shadow = atlasLoadTexture("BulbHeaderBar_shadow");
        texBindInit = 1;
    }

     if( open )
    {
        color = CLR_MM_BAR_OPEN;
        font_color(CLR_MM_TITLE_OPEN, CLR_MM_TITLE_OPEN);
    }
    else
    {
        color = CLR_MM_BAR_CLOSED;
        font_color(CLR_MM_TITLE_CLOSED, CLR_MM_TITLE_CLOSED);
    }

    ht = bulb->height*sc;

       BuildCBox(&box, x, y - ht/2, wd, ht );
    bar_hit = mouseCollision(&box);
    //drawBox(&box, 50000, CLR_GREEN, 0 );

    BuildCBox(&box, x, y - ht/2, wd - *popout - 20*sc, ht );
    //drawBox(&box, 50000, CLR_RED, 0 );
    if( !cursor.dragging && mouseLeftDrag(&box) )
        retval = 3;
    else if( mouseClickHit(&box, MS_LEFT) )
        retval = 1;


       if( mouseCollision(&box) )
    {
        // color change
        color = CLR_MM_BAR_MOUSEOVER;
        txt_color = CLR_MM_TITLE_LIT;
        icon_color = 0x52754bff;

        if(pchIconGlow)
            iconGlow = atlasLoadTexture(pchIconGlow);

        collisions_off_for_rest_of_frame = 1;
    }
    
       if( open || bar_hit )
    {
        *popout += TIMESTEP*4;
        if( *popout > 30*sc )
            *popout = 30*sc;
    }
    else
    {
        *popout -= TIMESTEP*4;
         if( *popout < -10.f )
            *popout = -10.f;
    }

       if( drawMMButton("", "delete_button_inside", "delete_button_outside", x + wd - 44*sc, y, z, 80*sc, sc*.80, MMBUTTON_ERROR|MMBUTTON_SMALL|MMBUTTON_ICONRIGHT, 0  ) )
        retval = 2;
    
     ht /= 2;
     wd -= *popout;

    // first draw the bar
      
       display_sprite( dropshadow, x + wd - dropshadow->width*sc, y - ht, z, sc, sc, (int)((255.f)*((*popout + 10.f)/40.f)) );

       display_sprite( bar_end, x + wd - bar_end->width*sc, y - ht, z, sc, sc, color );
      display_sprite( bar_end_s, x + wd - bar_end->width*sc, y - ht, z, sc, sc, CLR_WHITE );
      display_sprite( outline_end, x + wd - bar_end->width*sc, y - ht, z, sc, sc, CLR_BLACK );
    
     display_sprite( meat, x + bulb->width*sc/2, y - ht, z, (wd - (bar_end->width+bulb->width/2)*sc)/meat->width, sc, color );
    display_sprite( meat_s, x + bulb->width*sc/2, y - ht, z, (wd - (bar_end->width+bulb->width/2)*sc)/meat->width, sc, CLR_WHITE );
    display_sprite( outline_mid, x + bulb->width*sc/2, y - ht, z, (wd - (bar_end->width+bulb->width/2)*sc)/meat->width, sc, CLR_BLACK );

     font_color(txt_color, txt_color);
     cprntEx( cx, y + off, z, sc, sc, CENTER_X|CENTER_Y, "%s", pchText );

    // now the bulb
     display_sprite( bulb, x, y - ht, z, sc, sc, color );
    display_sprite( bulb_s, x, y - ht, z, sc, sc, color );
      display_sprite( bulb_out, x, y - ht, z, sc, sc, CLR_BLACK );

      display_sprite( ring, x , y - ht, z, sc, sc, 0x2c453fff );
    display_sprite( ring_base, x, y - ht, z, sc, sc, color );
    display_sprite( ring_s, x, y - ht, z, sc, sc, CLR_WHITE );
    display_sprite( ring_h, x, y - ht, z, sc, sc, CLR_WHITE );

    // Last the Icon
    if( iconGlow )
         display_sprite( iconGlow, x + (bulb->width*sc - icon->width*sc)/2, y - icon->height*sc/2, z, sc, sc, 0xddffadff );
       if( icon )
        display_sprite( icon, x + (bulb->width*sc - icon->width*sc)/2, y - icon->height*sc/2, z, sc, sc, icon_color );


    return retval;
}

int drawListButton(char *text, float x, float y, float z, float width, float height, float scale)
{
    AtlasTex *end = atlasLoadTexture("generic_button_004_end");
    AtlasTex *end_h = atlasLoadTexture("generic_button_004_highlight_end");
    AtlasTex *mid_h = atlasLoadTexture("generic_button_004_highlight_mid");
    AtlasTex *end_s = atlasLoadTexture("generic_button_004_shadow_end");
    AtlasTex *mid_s = atlasLoadTexture("generic_button_004_shadow_mid");
    AtlasTex *white = atlasLoadTexture("white");
    float mouse_overScale;
    CBox cbox;
     int flip = 0;
    F32 txt_y;
     BuildCBox(&cbox, x, y, width, height);
    y+= scale*end->height/2;
    txt_y = y;

     if (mouseCollision(&cbox))
    {
        mouse_overScale = 1.0f;
        if(isDown(MS_LEFT))
            flip = FLIP_VERTICAL;
    }
    else
    {
         mouse_overScale = 0.8;
        y++;
    }

      font(&game_12);
    font_color(CLR_BLACK,CLR_BLACK);
    font_outl(0);
    cprntEx( x+ width/2, txt_y+(scale*end->height)/2 + (flip?2*scale:0), z+2, scale, scale, CENTER_X|CENTER_Y, text );
    font_outl(1);

    display_sprite(end, x, y, z, mouse_overScale, mouse_overScale * height/end->height, CLR_MM_TEXT);
    display_spriteFlip(end_h, x, y-1, z, mouse_overScale, mouse_overScale * height/end->height, CLR_WHITE, flip);
    display_spriteFlip(end_s, x, y+1, z, mouse_overScale, mouse_overScale * height/end->height, CLR_BLACK, flip);
    display_sprite(white, x+(mouse_overScale*end->width), y, z, (width-(mouse_overScale*end->width*2))/white->width, mouse_overScale * height/white->height, CLR_MM_TEXT);
    display_spriteFlip(mid_h, x+(mouse_overScale*end->width), y-1, z, (width-(mouse_overScale*end->width*2))/mid_h->width, mouse_overScale * height/mid_h->height, CLR_WHITE, flip);
    display_spriteFlip(mid_s, x+(mouse_overScale*end->width), y+1, z, (width-(mouse_overScale*end->width*2))/mid_s->width, mouse_overScale * height/mid_s->height, CLR_BLACK, flip);
    flip |= FLIP_HORIZONTAL;
    display_spriteFlip(end, x+width-(mouse_overScale*end->width), y, z, mouse_overScale, mouse_overScale * height/end->height, CLR_MM_TEXT, flip);
    display_spriteFlip(end_h, x+width-(mouse_overScale*end->width), y-1, z, mouse_overScale, mouse_overScale * height/end->height, CLR_WHITE, flip);
    display_spriteFlip(end_s, x+width-(mouse_overScale*end->width), y+1, z+50, mouse_overScale, mouse_overScale * height/end->height, CLR_BLACK, flip);
    if (mouseClickHit(&cbox, MS_LEFT))
    {
        return 1;
    }
    return 0;
}
