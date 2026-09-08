#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/mathutil.h>
#include "graphics/gfxwindow.h"
#include <utilitieslib/utils/memcheck.h>
#include "graphics/camera.h"
#include "cmdparse/cmdgame.h"
#include "win/win_init.h"
#include "render/render.h"
#include "edit/edit_cmd.h"
#include "UI/uiGame.h"
#include "render/renderUtil.h"
#include "graphics/viewport.h"
#include "graphics/gfx.h"

GfxWindow gfx_window;
extern PBuffer pbRenderTexture;

static void gfxWindowReshapeOrthographic(F32 left, F32 right, F32 bottom, F32 top, F32 znear, F32 zfar, const Mat4 skewmat);

static void gfxWindowSetFrustum(Vec3 p_tln, Vec3 p_trn, Vec3 p_brn, Vec3 p_bln, Vec3 p_tlf, Vec3 p_trf, Vec3 p_brf, Vec3 p_blf)
{
    makePlane(p_tln, p_trn, p_brn, gfx_window.frustum[FRUST_NEAR]);
    makePlane(p_brf, p_trf, p_tlf, gfx_window.frustum[FRUST_FAR]);
    makePlane(p_tlf, p_tln, p_bln, gfx_window.frustum[FRUST_LEFT]);
    makePlane(p_trn, p_trf, p_brf, gfx_window.frustum[FRUST_RIGHT]);
    makePlane(p_trf, p_trn, p_tln, gfx_window.frustum[FRUST_TOP]);
    makePlane(p_bln, p_brn, p_brf, gfx_window.frustum[FRUST_BOTTOM]);

    gfx_window.use_frustum = 1;
}

void gfxWindowSetAng(F32 fovy, F32 fovx, F32 znear, F32 zfar, bool bFlipX, bool bFlipY, int setMatrix)
{
    GfxWindow    *w = &gfx_window;
    F32            viewang,cosval,hang,vang, aspect = fovx / fovy;

    if (setMatrix)
        rdrPerspective(fovy,aspect,znear,zfar,bFlipX,bFlipY);
    
    w->fovy = fovy;
    w->fovx = fovx;

    vang = RAD(fovy);
    hang = 2 * (fatan(aspect * ftan(vang/2.0)));

    viewang    = hang * .5;
    cosval    = cosf(viewang);
    w->hvam = (sinf(viewang)/cosval);
    w->hcos    = cosval;
    
    viewang    = vang * .5 ;
    cosval    = cosf(viewang);
    w->vvam    = (sinf(viewang)/cosval);
    w->vcos    = cosval;

    w->znear= -znear;
    w->zfar    = -zfar;

    w->use_frustum = 0;
}

static F32 gfxFixGameFOV(F32 fov, F32 aspect)
{
    F32 max_aspect = 800.0 / 600.0;
    
    if(aspect > max_aspect){
        fov *= max_aspect / aspect;
    }
    
    return fov;
}

void gfxWindowReshape()
{
    // JP: Adding letterbox support because this function is called in response to WM_SIZE,
    //     meaning we'd lose letterboxing if a user resized the window during a map move.
    //     Side effects may occur...
    if (isLetterBoxMenu())
    {
        gfxSetupLetterBoxViewport();
    }
    else
    {
        gfxWindowReshapeForViewport(NULL, -1);
    }
}

void gfxWindowReshapeForViewport(ViewportInfo * viewport, int fovMagic)
{
    int        viewport_width = -1;
    int        viewport_height = -1;
    int        viewport_x = 0;
    int        viewport_y = 0;
    F32        aspect;
    F32        fov;
    F32        nearz, farz;
    bool    bFlipYProjection = false;
    bool    bFlipXProjection = false;
    bool    bNeedScissoring = false;

    if(viewport && viewport->doHeadshot)
        fovMagic = viewport->fov;

    if (gfx_state.renderToPBuffer && !isLetterBoxMenu())
    {
        viewport_width = pbRenderTexture.width;
        viewport_height = pbRenderTexture.height;
        nearz = game_state.nearz;
        farz = game_state.farz;
    }
    else if (viewport)
    {
        viewport_width = (viewport->viewport_width != -1) ? viewport->viewport_width : viewport->width;
        viewport_height = (viewport->viewport_height != -1) ? viewport->viewport_height : viewport->height;

        if (viewport->viewport_width != -1 && viewport->viewport_height != -1)
        {
            viewport_x = viewport->viewport_x;
            viewport_y = viewport->viewport_y;
        }

        if (viewport_x != 0 || viewport_y != 0 || viewport_width != viewport->width || viewport_height != viewport->height)
            bNeedScissoring = true;

        nearz = viewport->nearZ;
        farz = viewport->farZ;
    }
    else
    {
        windowSize(&viewport_width,&viewport_height);
        nearz = game_state.nearz;
        farz = game_state.farz;
    }

    rdrViewport(viewport_x, viewport_y, viewport_width, viewport_height, bNeedScissoring);

    if (fovMagic > 0)
    {
        fov = ( (F32)viewport_height/ (F32)fovMagic ); //A magic number. It makes the fov right.  Don't touch it.  It's Magic!         
     
        aspect = (F32) viewport_width/(F32) viewport_height;       
    }
    else if (!viewport || viewport->useMainViewportFOV)
    {
        int        final_w, final_h;

        windowPhysicalSize(&final_w, &final_h);
        aspect = (F32) final_w/(F32) final_h;

        fov = game_state.fov_thisframe;

        if(!editMode() && !shell_menu())
        {
            fov = gfxFixGameFOV(fov, aspect);
        }

        if (!fov)
            fov = game_state.fov_3rd ? game_state.fov_3rd : 55; // fov_3rd is zero at startup before gamestateInit called, can get here that early
    }
    else
    {
        aspect = viewport->aspect;
        fov = viewport->fov;
    }

    if (viewport)
    {
        bFlipYProjection = viewport->bFlipYProjection;
        bFlipXProjection = viewport->bFlipXProjection;
    }

    if(fov)
        gfxWindowSetAng(fov, fov * aspect, nearz, farz, bFlipXProjection, bFlipYProjection, 1);
    else
    {
        if (!game_state.create_bins)
        {
            assert(viewport);
            gfxWindowReshapeOrthographic(viewport->left, viewport->right, viewport->bottom, viewport->top, viewport->nearZ, viewport->farZ, NULL);
        }
    }
}

void gfxWindowReshapeForHeadShot( int fovMagic )
{
    /*
    F32        aspect;
    int w1, h1;
    F32 fov;

    windowSize(&w1,&h1); 
    rdrViewport(w1, h1);   

    fov = ( (F32)h1/ (F32)fovMagic ); //A magic number. It makes the fov right.  Don't touch it.  It's Magic!         
 
    aspect = (F32) w1/(F32) h1;       
    
    gfxWindowSetAng(fov, fov * aspect, game_state.nearz,game_state.farz,false,false, 1);
    */

    gfxWindowReshapeForViewport(NULL, fovMagic);
}

static void gfxWindowReshapeOrthographic(F32 left, F32 right, F32 bottom, F32 top, F32 znear, F32 zfar, const Mat4 skewmat)
{
    Mat4 ident = {0};
    Mat44 a, b;
    Vec3 p, p_tln, p_trn, p_brn, p_bln, p_tlf, p_trf, p_brf, p_blf;
    ident[0][0] = ident[1][1] = ident[2][2] = 1;

    if (!skewmat)
        skewmat = ident;

    gfxWindowSetAng(160, 160, znear, zfar, false, false, 0);

    rdrSetupOrtho(left, right, bottom, top, znear, zfar);

    setVec3(p, left, top, znear);
    mulVecMat3(p, skewmat, p_tln);
    p_tln[2] = -p_tln[2];

    setVec3(p, right, top, znear);
    mulVecMat3(p, skewmat, p_trn);
    p_trn[2] = -p_trn[2];

    setVec3(p, right, bottom, znear);
    mulVecMat3(p, skewmat, p_brn);
    p_brn[2] = -p_brn[2];

    setVec3(p, left, bottom, znear);
    mulVecMat3(p, skewmat, p_bln);
    p_bln[2] = -p_bln[2];

    setVec3(p, left, top, zfar);
    mulVecMat3(p, skewmat, p_tlf);
    p_tlf[2] = -p_tlf[2];

    setVec3(p, right, top, zfar);
    mulVecMat3(p, skewmat, p_trf);
    p_trf[2] = -p_trf[2];

    setVec3(p, right, bottom, zfar);
    mulVecMat3(p, skewmat, p_brf);
    p_brf[2] = -p_brf[2];

    setVec3(p, left, bottom, zfar);
    mulVecMat3(p, skewmat, p_blf);
    p_blf[2] = -p_blf[2];

    copyMat44(gfx_state.projection_matrix, a);
    mat43to44(skewmat, b);
    mulMat44Inline(a, b, gfx_state.projection_matrix);

    gfxWindowSetFrustum(p_tln, p_trn, p_brn, p_bln, p_tlf, p_trf, p_brf, p_blf);

    rdrSendProjectionMatrix();
}

void gfxWindowScreenPos(Vec3 pos,Vec2 pixel_pos)
{
    int        w,h;

    windowSize(&w,&h);

    w >>= 1;
    h >>= 1;
    pixel_pos[0] = -pos[0] * w * 1.f/gfx_window.hvam / pos[2] + w;
    pixel_pos[1] = -pos[1] * h * 1.f/gfx_window.vvam / pos[2] + h;

#if 0
    xyprintf(5,16,"%f %f %f",pos[0],pos[1],pos[2]);
    xyprintf(5,17,"%f %f",pixel_pos[0],pixel_pos[1]);

    pixel_pos[0] = -pos[0] / pos[2];
    pixel_pos[1] = pos[1] / pos[2];
    xyprintf(5,18,"%f %f",pixel_pos[0],pixel_pos[1]);
#endif
}

// x,y are 2d screen positions
// len is how far to move endpoint
// start and end are outputs
void gfxCursor3d(F32 x,F32 y,F32 len,Vec3 start,Vec3 end)
{
    int        w,h;
    Vec3    dv;
    F32        yaw,pitch;
    Mat4    mat;
    F32        aspect;

    windowSize(&w,&h);
    aspect = (F32)w / (F32)h;
    copyMat4(cam_info.cammat,mat);
    {
    F32        hang,viewang,cosval,hvam,vang;

        vang = RAD(game_state.fov);
        
        if(!editMode() && !shell_menu())
        {
            vang = gfxFixGameFOV(vang, aspect);
        }
                        
        hang = 2 * (fatan(aspect * ftan(vang/2.0)));

        viewang    = hang * .5;
        cosval    = cosf(viewang);
        hvam = (sinf(viewang)/cosval);
        dv[2] = 1.f / hvam;
    }

    dv[0] = (x - w/2) / (F32)(w/2.f);
    dv[1] = (y - h/2) / (F32)(h * aspect/2.f);
    getVec3YP(dv,&yaw,&pitch);
    yawMat3(yaw,mat);
    pitchMat3(pitch,mat);

    copyVec3(mat[3],start);
    copyVec3(mat[3],end);
    moveVinZ(start,mat,-1.f);
    moveVinZ(end,mat,-len);
}

void gfxWindowGetFrustumBounds(Vec3 min, Vec3 max, F32 znear, F32 zfar)
{
    Vec3 p1, p;
    F32 scale = zfar / znear;
    F32 aspect = gfx_window.fovx / gfx_window.fovy;
    F32 top = znear * ftan(RAD(gfx_window.fovy * 0.5f));
    F32 bottom = -top;
    F32 left = bottom * aspect;
    F32 right = top * aspect;

    copyVec3(cam_info.inv_viewmat[3], min);
    copyVec3(cam_info.inv_viewmat[3], max);

    setVec3(p1, left, top, -znear);
    mulVecMat4(p1, cam_info.inv_viewmat, p);
    vec3RunningMinMax(p, min, max);

    setVec3(p1, right, top, -znear);
    mulVecMat4(p1, cam_info.inv_viewmat, p);
    vec3RunningMinMax(p, min, max);

    setVec3(p1, right, bottom, -znear);
    mulVecMat4(p1, cam_info.inv_viewmat, p);
    vec3RunningMinMax(p, min, max);

    setVec3(p1, left, bottom, -znear);
    mulVecMat4(p1, cam_info.inv_viewmat, p);
    vec3RunningMinMax(p, min, max);

    setVec3(p1, left * scale, top * scale, -zfar);
    mulVecMat4(p1, cam_info.inv_viewmat, p);
    vec3RunningMinMax(p, min, max);

    setVec3(p1, right * scale, top * scale, -zfar);
    mulVecMat4(p1, cam_info.inv_viewmat, p);
    vec3RunningMinMax(p, min, max);

    setVec3(p1, right * scale, bottom * scale, -zfar);
    mulVecMat4(p1, cam_info.inv_viewmat, p);
    vec3RunningMinMax(p, min, max);

    setVec3(p1, left * scale, bottom * scale, -zfar);
    mulVecMat4(p1, cam_info.inv_viewmat, p);
    vec3RunningMinMax(p, min, max);
}

int gfxIsPointVisible(const Vec3 world_pos)
{
    Vec3 cam_pos;
    mulVecMat4(world_pos, cam_info.viewmat, cam_pos);
    return pointVisible(cam_pos) == 0;
}

int gfxBoxIsVisible(Vec3 min, Vec3 max, Mat4 mat)
{
    // NOTE: The same optimizations as applied to checkBoxFrustum() could be
    // applied here.  But, this function is currently only used by the
    // double fusion logic which is currently not enabled.
    int        x, y, z;
    Vec3    extents[2];

    copyVec3(min,extents[0]);
    copyVec3(max,extents[1]);

    for(x=0;x<2;x++)
    {
        for(y=0;y<2;y++)
        {
            for(z=0;z<2;z++)
            {
                Vec3    pos;
                Vec3    tpos;

                pos[0] = extents[x][0];
                pos[1] = extents[y][1];
                pos[2] = extents[z][2];
                mulVecMat4(pos, mat, tpos);
                if(gfxIsPointVisible(tpos))
                    return 1;
            }
        }
    }
    return 0;
}

int compare_corner( const Vec2 *a, const Vec2 *b )
{
    return lengthVec2(*b)-lengthVec2(*a);
}

int gfxScreenSquareArea(Vec3 a, Vec3 b, Vec3 c, Vec3 d )
{
    Vec2    corners[4];
    int i, area = 0, w, h;
    windowClientSize(&w,&h);
    gfxWindowScreenPos(a, corners[0] );
    gfxWindowScreenPos(b, corners[1] );
    gfxWindowScreenPos(c, corners[2] );
    gfxWindowScreenPos(d, corners[3] );

    for( i = 0; i < 4; i++ ) // clamp to screen
    {
        corners[i][0] = MINMAX( corners[i][0], 0, w );
        corners[i][1] = MINMAX( corners[i][1], 0, h );    
    }
    qsort( corners, 4, sizeof(corners[0]), compare_corner ); 

    if(    !((corners[0][0] == corners[1][0] && corners[0][1] == corners[1][1]) ||
          (corners[0][0] == corners[3][0] && corners[0][1] == corners[3][1]) ||
          (corners[1][0] == corners[3][0] && corners[1][1] == corners[3][1]) ))
    {
        area += triArea2d(corners[0],corners[1],corners[3]);
    }

    if(    !((corners[0][0] == corners[2][0] && corners[0][1] == corners[2][1]) ||
          (corners[0][0] == corners[3][0] && corners[0][1] == corners[3][1]) ||
          (corners[2][0] == corners[3][0] && corners[2][1] == corners[3][1]) ))
    {
        area += triArea2d(corners[0],corners[2],corners[3]);
    }

    return area;
}


int gfxScreenBoxBestArea(Vec3 min, Vec3 max, Mat4 mat)
{
    Vec3    minmax[2], pos;
    Vec3    corners[2][4];
    int        i,y, area, biggest_area = 0;

     copyVec3(min, minmax[0]);
    copyVec3(max, minmax[1]);
    for(y=0;y<2;y++)
    {
        for(i=0;i<4;i++)
        {
            Vec3 tpos;
            pos[0] = minmax[i==1 || i==2][0];
            pos[1] = minmax[y][1];
            pos[2] = minmax[i/2][2];
            mulVecMat4(pos, mat, tpos);
            mulVecMat4(tpos, cam_info.viewmat, corners[y][i]);
        }
    }
    
    area = gfxScreenSquareArea(corners[0][0],corners[0][1],corners[0][2],corners[0][3]);
    biggest_area = MAX( biggest_area, area );
    area = gfxScreenSquareArea(corners[1][0],corners[1][1],corners[1][2],corners[1][3]);
     biggest_area = MAX( biggest_area, area );
    for(i=0;i<4;i++)
    {
        area = gfxScreenSquareArea(corners[1][i],corners[1][(i+1)&3],corners[0][(i+1)&3],corners[0][i]);
        biggest_area = MAX( biggest_area, area );
    }
    return biggest_area;
}


