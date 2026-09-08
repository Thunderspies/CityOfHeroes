#include <utilitieslib/stdtypes.h>
#include "render/rendershadowmap.h"
#include <utilitieslib/utils/SuperAssert.h>
#include "graphics/failtext.h"
#include "graphics/viewport.h"
#include "graphics/camera.h"
#include "player/player.h"
#include "graphics/sun.h"
#include "graphics/light.h"
#include "gridcoll/gridfind.h"
#include "group/group.h"
#include "cmdparse/cmdgame.h"
#include "graphics/gfx.h"
#include "render/renderUtil.h"
#include "render/thread/rt_shadowmap.h"
#include "graphics/gfxwindow.h"
#include "edit/edit_cmd.h"
#include "graphics/view_cache.h"
#include "Cloth/Cloth.h"
#include "graphics/clothnode.h"
#include "bases/bases.h"
#include "graphics/font.h"
#include "graphics/gfxDebug.h"

static bool shadowViewportPreCallback(ViewportInfo *viewport);
static bool shadowViewportPostCallback(ViewportInfo *viewport);
static bool shadowViewportCustomRenderCallback(ViewportInfo *viewport);

static float g_fFrustDepthSacle = 1.5f;
static float g_fFrustWidthScale = 2.0f;
static bool g_bForceOrigin = false;
static int g_bForceDir = 1;
static bool g_bFollowChar = true;
static bool g_bUseColorMap = false;

static float g_fMinModelRadius;

static shadowMapSet g_ShadowMaps = { 0 };

typedef struct shadowMapDef {
    int width;
    int height;
    Vec4 debugColor;
    float biasScale;
    ViewportInfo viewport;
} shadowMapDef;

static shadowMapDef shadowMapDefs[SHADOWMAP_MAX_SHADOWS] = {
    {1024, 1024, {0.5f, 0.0f, 0.0f, 0.25f}, 1.0f},
    {1024, 1024, {0.0f, 0.5f, 0.0f, 0.25f}, 1.0f},
    {1024, 1024, {0.0f, 0.0f, 0.5f, 0.25f}, 1.0f},
    {1024, 1024, {0.5f, 0.5f, 0.0f, 0.25f}, 1.0f},
};

// setup the default shadowmapping mode settings
void rdrShadowMapSetDefaults(void)
{
    game_state.shadowShaderInUse = game_state.shadowShaderSelection;
}

static initShadowMapMenu(void)
{
    rdrQueue(DRAWCMD_INITSHADOWMAPMENU,NULL,0);

    // Flush the queue here so the initialization is finished and game_state variables
    // have the desired values before manageShadowBuffers() is called.
    rdrQueueFlush();
}

bool createShadowMap(shadowMap * map, int shadowMapIndex, int width, int height, ViewportInfo * viewport)
{
    assert(!map->active);

    map->viewport = viewport;
    viewport_InitStructToDefaults(map->viewport);

    // Setup values
    assert(shadowMapIndex <= RENDERPASS_SHADOW_CASCADE_LAST - RENDERPASS_SHADOW_CASCADE1);
    map->viewport->renderPass = RENDERPASS_SHADOW_CASCADE1 + shadowMapIndex;
    map->viewport->width =2*width;        // this is the size of the atlas render target (4 cascades in one atlas)
    map->viewport->height = 2*height;
    map->viewport->renderSky = false;
    map->viewport->renderWater = false;
    map->viewport->renderAlphaPass = false;
    map->viewport->renderCharacters = (game_state.shadowDebugFlags & kRenderSkinnedIntoMap) != 0;
    map->viewport->name = map->name;
    map->viewport->fov = 0;
    map->viewport->left = -1.0f;
    map->viewport->right = 1.0f;
    map->viewport->top = -1.0f;
    map->viewport->bottom = 1.0f;
    map->viewport->nearZ = -1.0f;
    map->viewport->farZ = 1.0f;

    // Setup callbacks to control state during shadow map rendering
    map->viewport->pPreCallback = shadowViewportPreCallback;
    map->viewport->pCustomRenderCallback = shadowViewportCustomRenderCallback;
    map->viewport->pPostCallback = shadowViewportPostCallback;
    map->viewport->callbackData = map;

    map->viewport->needColorTexture = true; // ati fix, tbd, need a real fix -- g_bUseColorMap;
    map->viewport->needDepthTexture = !g_bUseColorMap;

    // shadowmaps render into one big render target texture atlas (which is created on the first cascade)
    // Assign this cascade to its quadrant in the atlas.
    map->viewport->viewport_x = width * (shadowMapIndex % 2);
    map->viewport->viewport_y = height * (shadowMapIndex / 2);
    map->viewport->viewport_width = width;
    map->viewport->viewport_height = height;

    // point to the shared render target for every cascade but the first
    map->viewport->pSharedPBufferTexture = (shadowMapIndex == 0 ? NULL : &g_ShadowMaps.maps[0].viewport->PBufferTexture);

    if(!viewport_InitRenderToTexture(map->viewport)) {
        failText("The viewport failed to initialize\n");
        viewport_Delete(map->viewport);
        return false;
    }

    if (g_bUseColorMap)
        memcpy(map->texture, viewport_GetPBuffer(map->viewport)->color_handle, sizeof(map->texture));
    else
        memcpy(map->texture, viewport_GetPBuffer(map->viewport)->depth_handle, sizeof(map->texture));

    if(!map->texture[0]) {
        failText("The pbuffer failed to return a depth texture\n");
        return false;
    }

    map->csm_eye_z_split_near = 0.0f;
    map->csm_eye_z_split_far = 0.0f;

    return true;
}

void deleteShadowMap(shadowMap * map)
{
    if(map->active) {
        viewport_Remove(map->viewport);
        map->active = false;
    }

    viewport_Delete(map->viewport);
}

void updateShadowMap(shadowMap * map, bool active, const char * name, const Mat4 mat, float coneAngle, float size, float height)
{
    Mat44 texMat, proj, mat44;

    if(!active) {
        if(map->active) {
            viewport_Remove(map->viewport);
            map->active = false;
        }
        return;
    }

    if(!map->active) {
        if(!viewport_Add(map->viewport)) {
            return;
        }
        map->active = true;
    }

    map->index = (int)(map - g_ShadowMaps.maps);
    snprintf(map->name, sizeof(map->name), "shadowmap_%d_%s", map->index, name);    
    
    copyMat4(mat, map->viewport->cameraMatrix);
        
    map->viewport->bottom = map->viewport->left = -0.5f * size;
    map->viewport->top = map->viewport->right = 0.5f * size;
    if(coneAngle) {
        map->viewport->fov = coneAngle;
        map->viewport->nearZ = height/100.0f;
        map->viewport->farZ = height;
    } else {
        map->viewport->fov = 0.0f;
        map->viewport->nearZ = -0.5f * height;
        map->viewport->farZ = 0.5f * height;
    }

    identityMat44(texMat);
    texMat[0][0] = texMat[1][1] = texMat[2][2] = 0.5f;
    texMat[3][0] = texMat[3][1] = texMat[3][2] = 0.5f;

    // use the same math as the viewport code
    if(map->viewport->fov) {
        rdrSetupPerspectiveProjection(map->viewport->fov, map->viewport->aspect, map->viewport->nearZ, map->viewport->farZ, false, false);
    } else {
        rdrSetupOrtho(map->viewport->left, map->viewport->right, map->viewport->bottom, map->viewport->top, map->viewport->nearZ, map->viewport->farZ);
    }

    gfxSetViewMat( map->viewport->cameraMatrix, map->shadowViewMat, map->shadowViewMatInv );

    mat43to44(map->shadowViewMat, mat44);
    copyMat44(gfx_state.projection_matrix, proj);
    mulMat44(proj, mat44, map->shadowMat);
    mulMat44(texMat, map->shadowMat, map->shadowTexMat);
}

void updateCascadedShadowMap(shadowMap * map, bool active, const char * name, const Mat4 mat, float coneAngle, float size, float height)
{
    Mat44 texMat, proj, mat44;

    if(!active) {
        if(map->active) {
            viewport_Remove(map->viewport);
            map->active = false;
        }
        return;
    }

    if(!map->active) {
        if(!viewport_Add(map->viewport)) {
            return;
        }
        map->active = true;
    }

    map->index = (int)(map - g_ShadowMaps.maps);
    snprintf(map->name, sizeof(map->name), "shadowmap_%d_%s", map->index, name);    

    copyMat4(mat, map->viewport->cameraMatrix);

    map->viewport->bottom = map->viewport->left = -0.5f * size;
    map->viewport->top = map->viewport->right = 0.5f * size;
    if(coneAngle) {
        map->viewport->fov = coneAngle;
        map->viewport->nearZ = height/100.0f;
        map->viewport->farZ = height;
    } else {
        map->viewport->fov = 0.0f;
        map->viewport->nearZ = -0.5f * height;
        map->viewport->farZ = 0.5f * height;
    }

    identityMat44(texMat);
    texMat[0][0] = texMat[1][1] = texMat[2][2] = 0.5f;
    texMat[3][0] = texMat[3][1] = texMat[3][2] = 0.5f;

    // use the same math as the viewport code
    if(map->viewport->fov) {
        rdrSetupPerspectiveProjection(map->viewport->fov, map->viewport->aspect, map->viewport->nearZ, map->viewport->farZ, false, false);
    } else {
        rdrSetupOrtho(map->viewport->left, map->viewport->right, map->viewport->bottom, map->viewport->top, map->viewport->nearZ, map->viewport->farZ);
    }

    gfxSetViewMat( map->viewport->cameraMatrix, map->shadowViewMat, map->shadowViewMatInv );

    mat43to44(map->shadowViewMat, mat44);
    copyMat44(gfx_state.projection_matrix, proj);
    mulMat44(proj, mat44, map->shadowMat);
    mulMat44(texMat, map->shadowMat, map->shadowTexMat);
}

void updateUnusedShadowMap(shadowMap * map)
{    
    updateShadowMap(map, false, "unused", unitmat, 0, 1, 1);
}

void manageShadowBuffers(int numBuffers, shadowMapDef def[])
{
    int i;

    // If map dimensions have changed delete all the maps at the old
    // size
    if (g_ShadowMaps.numShadows)
    {
        // We only have one actual shadowmap render target (4 cascades in one atlas)
        // which has 2x dimension requested for the map of each individual cascade
        if (g_ShadowMaps.maps[0].viewport->width != 2*game_state.shadowMapTexSize)
        {
            for (i=0; i < g_ShadowMaps.numShadows; ++i )
            {
                deleteShadowMap(g_ShadowMaps.maps + i);
            }
            g_ShadowMaps.numShadows = 0;
        }
    }

    // Delete extra maps
    for(i = g_ShadowMaps.numShadows-1; i >= numBuffers; i--) {
        deleteShadowMap(g_ShadowMaps.maps + i);
        g_ShadowMaps.numShadows--;
    }

    // Create missing maps
    for(i = g_ShadowMaps.numShadows; i < numBuffers; i++) {
        shadowMap * map = g_ShadowMaps.maps + i;
        int dim = game_state.shadowMapTexSize;
        if (dim < 512) dim = 512;
        if(!createShadowMap(map, i, dim, dim, &def[i].viewport )) { // def[i].width, def[i].height)) {
            failText("Failed to create shadowmap %d\n", i);
            g_ShadowMaps.numShadows = i;
            return;
        }

        // TODO should this be a function of texture size?
        map->biasScale = game_state.biasScale[i]; // def[i].biasScale;
        copyVec4(def[i].debugColor, map->debugColor);

        updateUnusedShadowMap(map);
        g_ShadowMaps.numShadows++;
//        break;    //@todo only increment shadow maps one per frame to work around weird bug for the moment
    }
}

static int lightSizeCmp(const void * a, const void *b)
{
    const DefTracker * const * dta = (const DefTracker * const *)a;
    const DefTracker * const * dtb = (const DefTracker * const *)b;

    // Sort by light size
    if((*dta)->def->light_radius < (*dtb)->def->light_radius) return 1;
    if((*dta)->def->light_radius > (*dtb)->def->light_radius) return -1;

    // Sort by id second
    if((*dta)->def->id < (*dtb)->def->id) return -1;
    if((*dta)->def->id > (*dtb)->def->id) return 1;

    // Sort by ptr last to enforce a stable sort
    if(dta < dtb) return -1;
    if(dta > dtb) return 1;

    // Should not happen
    return 0;
}

// it has the camera location in the translation and the camera orientation in the upper 3x3
// The camera system then turns this into a view matrix for rendering
static void buildLookMatrix(const Vec3 target, const Vec3 dir, Mat4 mat)
{
    Vec3 invDir;
    Mat4 rot, trans;

    // Translation
    identityMat4(trans);
    copyVec3(target, trans[3]);

    // Rotation
    identityMat4(rot);
    scaleVec3(dir, -1, invDir);

    if (game_state.shadowDebugFlags&orientOnView)
    {
        // try to axis align shadow maps with view direction to extent possible
        Vec3 smMapAxisV,smMapAxisU;
        copyVec3(cam_info.cammat[2], smMapAxisV);
        crossVec3(smMapAxisV,invDir,smMapAxisU);
        if (    lengthVec3Squared(smMapAxisU) > 1e-5f )
        {
            orientMat3Yvec(rot, invDir, cam_info.cammat[2] );
        }
        else
            orientMat3(rot, invDir);
    }
    else
        orientMat3(rot, invDir);

    mulMat4Inline(trans, rot, mat);
}

static void calcLightDirection( Vec3 rLightDir, float alt_degrees )
{
    float angle = RAD(alt_degrees);
    setVec3(rLightDir, -cosf(angle), -sinf(angle), 0.0f );
}

static void getShadlowLightDir( Vec3 shadowDir )
{
    #define SHADOWMAP_MAX_ANGLE_DEBUG 0
    int dirType = game_state.shadowDirType;
    
    switch (dirType)
    {
    case dirSun:
    {
        scaleVec3(g_sun.direction, -1, shadowDir);
        {
            Vec3 testSunVec;
            Vec3 maxVec;
            F32 sunAngleDot;
            F32 sunLimitDot;
            F32 maxAngle = RAD( 90.0f + game_state.shadowMaxSunAngleDeg ); // offset from straight up
            bool bClamped = false;

            copyVec3( g_sun.direction, testSunVec );
        
                // Vector at maximum sun angle on the x-y plane
            setVec3(maxVec, cosf(maxAngle), sinf(maxAngle), 0.0f );
                // Actual angle of the sun from straight up
            sunAngleDot = dotVec3( testSunVec, upvec3 );
                // Maximum allowed variance from straight up
            sunLimitDot = dotVec3( maxVec, upvec3 );

            if ( sunAngleDot < sunLimitDot )
            {
                Vec3 newSunVec;
                F32 sunAngleFromX = atan2f(shadowDir[2], shadowDir[0]);
                F32 newAngle = RAD(game_state.shadowMaxSunAngleDeg);
                setVec3( newSunVec,
                            ( sinf(newAngle) * cosf(sunAngleFromX) ),
                            -cosf(newAngle),
                            ( sinf(newAngle) * sinf(sunAngleFromX) )
                        );
                copyVec3( newSunVec, shadowDir );
                bClamped = true;
            }
        
            //------------------------------------------------------------------------
            #if SHADOWMAP_MAX_ANGLE_DEBUG
            //------------------------------------------------------------------------
            {
                static bWasClamped = false;
                F32 sunAngleDeg = DEG(acosf( sunAngleDot ));
                F32 sunLimitDeg = DEG(acosf( sunLimitDot ));        
                if (( ! bWasClamped ) && ( bClamped ))
                {
                    F32 sunVecOfsAngle2 = DEG(acosf( dotVec3( testSunVec, upvec3 ) ));
                    printf( "==========================================================\n" );
                    printf( "Shadow angle clamping: ON (Sun=%0.2f%, Limit=%0.2f)\n"
                            "\n"
                            "  shadowDir BEFORE..%3.3f,%3.3f,%3.3f\n"
                            "  shadowDir AFTER...%3.3f,%3.3f,%3.3f\n"
                            "  new sun angle.....%3.f\n",
                        sunAngleDeg, sunLimitDeg,
                        -g_sun.direction[0],-g_sun.direction[1],-g_sun.direction[2],
                        shadowDir[0],shadowDir[1],shadowDir[2],
                        sunVecOfsAngle2 );
                    printf( "==========================================================\n" );
                }
                else if (( bWasClamped ) && ( ! bClamped ))
                {
                    printf( "==========================================================\n" );
                    printf( "Shadow angle clamping: OFF (Sun=%0.2f%, Limit=%0.2f)\n",
                         sunAngleDeg, sunLimitDeg );
                    printf( "==========================================================\n" );
                }
                bWasClamped = bClamped;
            }
            //------------------------------------------------------------------------
            #endif
            //------------------------------------------------------------------------
        }        
        break;
    }
    case dirY:
        // -Y , not exactly cause look at code messes up
        setVec3(shadowDir, -.2f, -0.980f, 0.0f );
        break;
    case dirZ:
        setVec3(shadowDir, 0.0f, 0.0f, -1.0f );
        break;
    case dirView:
        scaleVec3(cam_info.cammat[2], -1.0f, shadowDir );
        break;
    case dir30:
        calcLightDirection(shadowDir, 30.0f );
        break;
    case dir45:
        calcLightDirection(shadowDir, 45.0f );
        break;
    case dir60:
        calcLightDirection(shadowDir, 60.0f );
        break;
    case dirTweak:
        calcLightDirection(shadowDir, game_state.shadowDirAltitude);
        break;
    }
}

// calculate cascade split distances and return in the rSplits array
static void csmCalcSplits( float* rSplits, int iNumSplits, float lambda, float view_near, float view_far )
{
    if (game_state.shadowSplitCalcType == shadowSplitCalc_StaticAssignment)
    {
        // force split values for selectable degrade on cascades that
        // limits draw distance but doesn't change shadow quality
        rSplits[0]=view_near;
        rSplits[1]=-game_state.shadowSplit1;
        rSplits[2]=-game_state.shadowSplit2;
        rSplits[3]=-game_state.shadowSplit3;
        rSplits[4]=view_far;
    }
    else
    {
        int i;
        int numSplitsToUse;

        // Currently for our purposes we allow the shadow distance and the split lambda to be set in the
        // scene file. If we want to allow the shadow draw distance to be controlled by the user by increasing
        // or decreasing the number of cascades then it is easiest at the moment to use a calculation mode where
        // we dynamically calculate the splits but always based on the max number of splits (4).
        // Then changing the number of active cascades at runtime allows us to decrease draw distance without
        // affecting look of shadows versus all 4 cascades
        if (game_state.shadowSplitCalcType == shadowSplitCalc_DynamicMaxSplits)
            numSplitsToUse = 4;
        else
            numSplitsToUse = iNumSplits;    // use the chosen number of splits to cover the entire shadowing distance

        // Practical split scheme:
        //
        // CLi = n*(f/n)^(i/numsplits)
        // CUi = n + (f-n)*(i/numsplits)
        // Ci = CLi*(lambda) + CUi*(1-lambda)
        //
        // lambda scales between logarithmic and uniform
        //
        for(i=0;i<numSplitsToUse;i++)
        {
            float fIDM=i/(float)numSplitsToUse;
            float fLog=view_near*powf((view_far/view_near),fIDM);
            float fUniform=view_near+(view_far-view_near)*fIDM;
            rSplits[i]=fLog*lambda+fUniform*(1-lambda);
        }

        // make sure border values are right
        rSplits[0]=view_near;
        rSplits[numSplitsToUse]=view_far;
    }



}

// calculate the world coordinates of the 8 corner points of the given view frustum
void calcFrustumPoints( Vec3* points, Mat4 view_to_world, float z_near, float z_far, float fov_x, float fov_y )
{
    Vec3 fc, nc;
    float fov_x_rad = RAD(fov_x);
    float fov_y_rad = RAD(fov_y);

    // these heights and widths are half the heights and widths of
    // the near and far plane rectangles
    float near_height = tanf(fov_y_rad/2.0f) * z_near;
    float near_width = tanf(fov_x_rad/2.0f) * z_near;
    float far_height = tanf(fov_y_rad/2.0f) * z_far;
    float far_width = tanf(fov_x_rad/2.0f) * z_far;

    // calculate the center point of the near and far plane rectangles
    scaleAddVec3( view_to_world[2], z_far, view_to_world[3], fc );    // center of far plane = eye_loc + view_dir*z_far
    scaleAddVec3( view_to_world[2], z_near, view_to_world[3], nc );    // center of far plane = eye_loc + view_dir*z_far

    // near
    scaleAddVec3( view_to_world[1], -near_height, nc, points[0] );    //  nc - up*near_height - right*near_width;
    scaleAddVec3( view_to_world[0], -near_width, points[0], points[0] );

    scaleAddVec3( view_to_world[1], +near_height, nc, points[1] );    //  nc + up*near_height - right*near_width;
    scaleAddVec3( view_to_world[0], -near_width, points[1], points[1] );

    scaleAddVec3( view_to_world[1], +near_height, nc, points[2] );    //  nc + up*near_height + right*near_width;
    scaleAddVec3( view_to_world[0], +near_width, points[2], points[2] );

    scaleAddVec3( view_to_world[1], -near_height, nc, points[3] );    //  nc - up*near_height + right*near_width;
    scaleAddVec3( view_to_world[0], +near_width, points[3], points[3] );

    // far
    scaleAddVec3( view_to_world[1], -far_height, fc, points[4] );    //  points[4] = fc - up*far_height - right*far_width;
    scaleAddVec3( view_to_world[0], -far_width, points[4], points[4] );

    scaleAddVec3( view_to_world[1], +far_height, fc, points[5] );    //  points[5] = fc + up*far_height - right*far_width;
    scaleAddVec3( view_to_world[0], -far_width, points[5], points[5] );

    scaleAddVec3( view_to_world[1], +far_height, fc, points[6] );    //  points[6] = fc + up*far_height + right*far_width;
    scaleAddVec3( view_to_world[0], +far_width, points[6], points[6] );

    scaleAddVec3( view_to_world[1], -far_height, fc, points[7] );    //  points[7] = fc - up*far_height + right*far_width;
    scaleAddVec3( view_to_world[0], +far_width, points[7], points[7] );
}

// calculate the centroid world coordinates of the given frustum corners
static void calcFrustumCentroid( Vec3 centroid, Vec3* points )
{
    int i;

    setVec3( centroid, 0.0f, 0.0f, 0.0f );

    for (i=0; i< 8; ++i)
    {
        addVec3( centroid, points[i], centroid );
    }
    scaleVec3( centroid, 1/8.0f, centroid );
}

static F32 calcLargestFrustumExtent(const Vec3 *points)
{
    int i;

    Vec3 min, max;
    float extentAll, extentX, extentY, extentZ;

    setVec3(min, FLT_MAX, FLT_MAX, FLT_MAX);
    setVec3(max, -FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (i=0; i< 8; ++i)
    {
        vec3RunningMinMax( points[i], min, max );
    }

    extentX = max[0] - min[0];
    extentY = max[1] - min[1];
    extentZ = max[2] - min[2];

    extentAll = MAX(extentX, extentY);
    extentAll = MAX(extentAll, extentZ);

    return extentAll;
}

// calculate the center, in world coordinates, and a radius of a sphere which bounds
// the given view frustum.
// This calculation currently assumes frustum is symmetric and that z_far > z_near
static void csmCalcFrustumBoundingSphere( Vec3 rCenter, float* rRadius, Mat4 view_to_world, float z_near, float z_far, float fov_x, float fov_y )
{
    Vec3 extrema;

    float fov_x_rad = RAD(fov_x);
    float fov_y_rad = RAD(fov_y);

    // these heights and widths are half the heights and widths of the far plane rectangles
    float far_height = tanf(fov_y_rad/2.0f) * z_far;
    float far_width = tanf(fov_x_rad/2.0f) * z_far;

    // calculate the center point of the frustum (assumes symmetric)
    scaleAddVec3( view_to_world[2], (z_far-z_near)*0.5f, view_to_world[3], rCenter );
    
    // from center to a far plane corner is a radius length
    // Far Corner Point - Center Point simplifies to the following:
    // Rv = far_width*U + far_height*V + (z_far + z_near)/2*N
    setVec3(extrema, 0.0f, 0.0f, 0.0f );
    scaleAddVec3( view_to_world[0], far_width, extrema, extrema );
    scaleAddVec3( view_to_world[1], far_height, extrema, extrema );    
    scaleAddVec3( view_to_world[2], (z_far+z_near)*0.5f, extrema, extrema );

    *rRadius = lengthVec3(extrema);
}

// equivalent to glOrtho( l, r, b, t, znear, zfar ):
void csmOrthoProjection( Mat44 T_proj, F32 l, F32 r, F32 b, F32 t, F32 znear, F32 zfar)
{
    memset(T_proj,0,16*sizeof(float));
    T_proj[0][0] = 2/(r-l);
    T_proj[1][1] = 2/(t-b);
    T_proj[2][2] = -2/(zfar-znear);
    T_proj[3][0] = -(r+l)/(r-l);
    T_proj[3][1] = -(t+b)/(t-b);
    T_proj[3][2] = -(zfar+znear)/(zfar-znear);
    T_proj[3][3] = 1;
}

// given a light direction, origin and the viewport extents of the light projection
// prepare the given shadow map structure (only does ortho light projection at the moment)
static void csmPrepareShadowMap( shadowMap * map, Vec3 lightDir, Vec3 lightOrigin, Vec3    tmin, Vec3 tmax )
{
    Mat4    T_light_view_to_world;
    Mat44 T_light_proj, T_tex, T_temp;

    buildLookMatrix(lightOrigin, lightDir, T_light_view_to_world);

    // create ortho projection for light, equivalent to glOrtho():
    csmOrthoProjection(T_light_proj, tmin[0], tmax[0], tmin[1], tmax[1], tmin[2], tmax[2] );

    // setup shadowMap record
    map->index = (int)(map - g_ShadowMaps.maps);
    snprintf(map->name, sizeof(map->name), "shadowmap_%d_%s", map->index, "csm");    

    map->viewport->left        = tmin[0];
    map->viewport->right        = tmax[0];
    map->viewport->bottom    = tmin[1];
    map->viewport->top            = tmax[1];
    map->viewport->nearZ        = tmin[2];
    map->viewport->farZ        = tmax[2];
    map->viewport->fov            = 0.0f;

    copyMat4(T_light_view_to_world, map->viewport->cameraMatrix);
    gfxSetViewMat( map->viewport->cameraMatrix, map->shadowViewMat, map->shadowViewMatInv );

    // Usually scale and offset to map proj clip space [-1,1] to [0,1] texture coords
    identityMat44(T_tex);
//    T_tex[0][0] = T_tex[1][1] = T_tex[2][2] = 0.5f;
//    T_tex[3][0] = T_tex[3][1] = T_tex[3][2] = 0.5f;
    // but now we want it to map to the correct quadrant of our atlas render target
    // scale
    T_tex[0][0] = T_tex[1][1] = 0.25;
    T_tex[2][2] = 0.5f;
    // translation
    T_tex[3][0] = 0.25f + 0.5f * (map->index % 2);
    T_tex[3][1] = 0.25f + 0.5f * (map->index / 2);
    T_tex[3][2] = 0.5f;

    // create the combined shadow map transform from world space to the shadow map texture coords
    // note: math library mulMat(M1, M2) multiples M2*M1 when considered row major
    mat43to44(map->shadowViewMat, T_temp);
    mulMat44(T_light_proj, T_temp, map->shadowMat);
    mulMat44(T_tex, map->shadowMat, map->shadowTexMat);

    if(!map->active) {
        if(!viewport_Add(map->viewport)) {
            return;
        }
        map->active = true;
    }

}

/*
    Given a world location target for the shadow map this routine will optionally 'quantize'
    that location so that as the target moves in world space the shadow map scrolls around
    by integer pixels so as to avoid flicker from different subpixel rasterizations.

    The basic idea is that if we consider the delta vector from the point the shadow map is
    currently focused on the the new location and project that into light space we can modify the
    step in light space to hit the nearest pixel center and then project that back to world space
    for our new 'target'. This way when the shadow map is actually rasterized it will just be a pixel
    shifted version of the map at the previous location where they 'overlap'. This eliminates shadow
    flicker or shimmer from movement (assuming that you are keeping the pixel density constant and not
    letting the density change with movement...e.g. using spherical instead of tight split bounds)
*/
static void CalcMapAnchor( Vec3 rMapAnchor, shadowMap* map, Vec3 newShadowDir, Vec3 newTargetAnchor, float mapWorldDimension )
{
    if ( game_state.shadowDeflickerMode == deflickerAlways ||
        ( game_state.shadowDeflickerMode == deflickerStatic && nearSameVec3Tol(map->shadowDir, newShadowDir, 1e-6f ) ) )
    {
        // if the new location and the old pre quant location are the same then target hasn't moved and keep same
        // quant location
        if (nearSameVec3Tol(map->splitAnchorRaw, newTargetAnchor, 1e-6f ))
        {
            copyVec3( map->splitAnchorQuant, rMapAnchor );
        }
        else
        {
            Mat4    world_to_light, light_to_world;
            float x,y, texels_per_world_unit;
            Vec3    quantOrigin;
            Vec3    deltaW, deltaL, deltaLquant, deltaWquant;

            // calculate the delta from our last real map anchor (quant) to the new location
            subVec3(newTargetAnchor, map->splitAnchorQuant, deltaW );

            // remap the origin so that it is in the center of the shadow map
            // texture and we avoid potential jitter
            texels_per_world_unit = (float)(map->viewport->viewport_width)/mapWorldDimension;

            // get the target delta coordinates using the light space axes
            setVec3(quantOrigin,0.0f,0.0f,0.0f);
            buildLookMatrix(quantOrigin, newShadowDir, light_to_world);
            invertMat4( light_to_world, world_to_light );

            mulVecMat4(deltaW,world_to_light,deltaL);

            // quantize on light delta xy so that we will step on pixel centers
            x = floorf(deltaL[0] * texels_per_world_unit) / texels_per_world_unit;
            y = floorf(deltaL[1] * texels_per_world_unit) / texels_per_world_unit;

            // convert back to a world space delta
            setVec3(deltaLquant, x, y, deltaL[2]);
            mulVecMat4(deltaLquant,light_to_world, deltaWquant);

            addVec3(deltaWquant, map->splitAnchorQuant, rMapAnchor );
        }
    }
    copyVec3(rMapAnchor, map->splitAnchorQuant);
    copyVec3(newTargetAnchor, map->splitAnchorRaw);
    copyVec3(newShadowDir, map->shadowDir);
    map->mapWorldDimension = mapWorldDimension;
}

static void csmUpdate(void)
{
    Vec3    shadowDir;
    int        iSplit;
    float splits[SHADOWMAP_MAX_SHADOWS+1];

    // how many cascades are active?
    int iNumCascades = MIN(game_state.shadowmap_num_cascades, SHADOWMAP_MAX_SHADOWS);
    iNumCascades = MIN(iNumCascades, g_ShadowMaps.numShadows);
    
    // the current main camera eye viewport 'frustum' data is contained in the gfx_window
    // calculate practical split z values for our shadow cascades
    // engine uses -z as into eye view along view direction (gl convention)
    //csmCalcSplits( splits, iNumCascades, game_state.shadowSplitLambda, gfx_window.znear, -MIN(game_state.shadowFarMax, -gfx_window.zfar) );
    csmCalcSplits( splits, iNumCascades, game_state.shadowSplitLambda, -game_state.nearz, -MIN(game_state.shadowFarMax, game_state.farz) );
    getShadlowLightDir(shadowDir);

    // need to populate some game state (g_sun) with current shadowmap light direction so that it can
    // be used for culling etc. when generating shadow maps
    copyVec3( shadowDir, gfx_state.shadowmap_light_direction_ws );
    gfx_state.shadowmap_extrusion_scale = fabs(1.0f / shadowDir[1]);

    for(iSplit=0;iSplit<iNumCascades;iSplit++)
    {
        Vec3 light_frust_min, light_frust_max;
        Vec3 sphere_origin, centroid, light_origin;
        float lightSplitRadius;
        Vec3    points[8];
        float largestExtent;

        // near and far planes for current frustum split
        float fNear=splits[iSplit];
        float fFar=splits[iSplit+1];

        // for splits past the first we move back the near plane in order to 
        // have a little overlap between cascades in the shadowmaps that we can blend
        // between if desired
        if (iSplit>0)
        {
            fNear -= -game_state.shadowmap_split_overlap[iSplit];    // recall z values are negative
        }

        // calculate a bounding sphere for the split
        csmCalcFrustumBoundingSphere( sphere_origin, &lightSplitRadius, cam_info.cammat, fNear,fFar,gfx_window.fovx,gfx_window.fovy );

        // calculate the splits view frustum corners in world space
        calcFrustumPoints(points,cam_info.cammat,fNear,fFar,gfx_window.fovx,gfx_window.fovy);

        // center of the split
        calcFrustumCentroid( centroid, points );

        copyVec3( centroid, light_origin );

        // minimum visible radius
        largestExtent = calcLargestFrustumExtent(points);

        // Seems to look better at lower texture sizes to always assume the largest texture size
        //g_ShadowMaps.maps[iSplit].minModelRadius = largestExtent / game_state.shadowMapTexSize;
        g_ShadowMaps.maps[iSplit].minModelRadius = largestExtent / 2048;

        if (game_state.shadowViewBound == viewBoundTight)
        {
            // convert each view frustum corner of the split to the lights
            // view so we can determine the max extent in light space in
            // order to maximize the use of our texels
            int i;
            Mat4    T_light_view_to_world, T_light_world_to_view;

            buildLookMatrix(light_origin, shadowDir, T_light_view_to_world);
            invertMat4Copy(T_light_view_to_world,T_light_world_to_view);

            setVec3(light_frust_min, FLT_MAX, FLT_MAX, FLT_MAX);
            setVec3(light_frust_max, -FLT_MAX, -FLT_MAX, -FLT_MAX);

            for (i=0; i< 8; ++i)
            {
                Vec3 p_light;

                mulVecMat4( points[i], T_light_world_to_view, p_light );
                vec3RunningMinMax( p_light, light_frust_min, light_frust_max );
            }

            // if we are orienting with view then we should try to keep
            // dimensions scaled to a map world extent unit to avoid
            // flickering
            if (game_state.shadowDebugFlags&quantizeLocal)
            {
                light_frust_min[0] = floorf( light_frust_min[0] );
                light_frust_min[1] = floorf( light_frust_min[1] );
                light_frust_max[0] = ceilf( light_frust_max[0] );
                light_frust_max[1] = ceilf( light_frust_max[1] );
            }
            // @todo pull near plane back to include casters which fall into split
            light_frust_min[2]    = -0.5f * game_state.shadowLightSizeZ;
            light_frust_max[2]    =  0.5f * game_state.shadowLightSizeZ;
        }
        else if (game_state.shadowViewBound == viewBoundSphere)
        {
        
            // use bounding sphere to avoid dynamic swimming (view and light changes)
            // the world space to texel space scaling remains constant
            // as the view changes (eliminating the flicker that occurs)
            // at the expense of wasting some of the available shadow map
            // space
            shadowMap* pMap = &g_ShadowMaps.maps[iSplit];
            float mapWorldDimension;

            // make sure radius is also quantized so scaling to the shadow map doesn't
            // change unnecessarily
            lightSplitRadius = ceilf(lightSplitRadius );
            mapWorldDimension = 2.0f*lightSplitRadius;

            CalcMapAnchor(light_origin, pMap, shadowDir, light_origin, mapWorldDimension );

            light_frust_min[0]    = -lightSplitRadius;
            light_frust_max[0]    =  lightSplitRadius;
            light_frust_min[1]    = -lightSplitRadius;
            light_frust_max[1]    =  lightSplitRadius;
            light_frust_min[2]    = -0.5f * game_state.shadowLightSizeZ;
            light_frust_max[2]    =  0.5f * game_state.shadowLightSizeZ;
        }
        else if (game_state.shadowViewBound == viewBoundApprox)
        {
            float texels_per_world_unit;
            int i;
            Mat4    T_light_view_to_world, T_light_world_to_view;
            float quantLevels = game_state.shadowScaleQuantLevels;
            float    quantDensity;
            float dx, dy, mx, my;
            Vec3    pos_light_space;

            // Here we want to tightly map the projected split frustum
            // onto the available shadowmap area but we limit ourselves
            // to a finite number of scalings in order to reduce flicker
            // of dynamic lights and viewpoint changes

            // the split radius gives us the basis for the density

            // make sure radius is also quantized so scaling to the shadow map doesn't
            // change unnecessarily
            lightSplitRadius = ceilf(lightSplitRadius );

            texels_per_world_unit = (float)(g_ShadowMaps.maps[iSplit].viewport->viewport_width) / (2.0f*lightSplitRadius);

            quantDensity = texels_per_world_unit/quantLevels;

            // convert each view frustum corner of the split to the lights
            // view so we can determine the max extent in light space in
            // order to maximize the use of our texels

            buildLookMatrix(light_origin, shadowDir, T_light_view_to_world);
            invertMat4Copy(T_light_view_to_world,T_light_world_to_view);

            setVec3(light_frust_min, FLT_MAX, FLT_MAX, FLT_MAX);
            setVec3(light_frust_max, -FLT_MAX, -FLT_MAX, -FLT_MAX);

            for (i=0; i< 8; ++i)
            {
                Vec3 p_light;

                mulVecMat4( points[i], T_light_world_to_view, p_light );
                vec3RunningMinMax( p_light, light_frust_min, light_frust_max );
            }

            // if we are orienting with view then we should try to keep
            // dimensions scaled to a map world extent unit to avoid
            // flickering
            if (game_state.shadowDebugFlags&quantizeLocal)
            {
                light_frust_min[0] = floorf( light_frust_min[0] );
                light_frust_min[1] = floorf( light_frust_min[1] );
                light_frust_max[0] = ceilf( light_frust_max[0] );
                light_frust_max[1] = ceilf( light_frust_max[1] );
            }

            dx = light_frust_max[0] - light_frust_min[0];
            dy = light_frust_max[1] - light_frust_min[1];

            // quantize those distances to reduce scale flicker
            dx = ceilf(dx*quantDensity)/quantDensity;
            dy = ceilf(dy*quantDensity)/quantDensity;
    
            // light space extent midpoint
            mx = (light_frust_max[0] + light_frust_min[0])*0.5f;
            my = (light_frust_max[1] + light_frust_min[1])*0.5f;
            mx = ceilf(mx*quantDensity)/quantDensity;
            my = ceilf(my*quantDensity)/quantDensity;

            // map the center of the shadowmap back into world space to get the correct light origin
            setVec3(pos_light_space,mx,my,0.0f);
            mulVecMat4(pos_light_space,T_light_view_to_world, light_origin);

            setVec3(light_frust_min,-dx*0.5f,-dy*0.5f,light_frust_min[2]);
            setVec3(light_frust_max,+dx*0.5f,+dy*0.5f,light_frust_max[2]);

            // @todo pull near plane back to include casters which fall into split
            light_frust_min[2]    = -0.5f * game_state.shadowLightSizeZ;
            light_frust_max[2]    =  0.5f * game_state.shadowLightSizeZ;
        }
        else // viewBoundFixed
        {
            light_frust_min[0]    = -0.5f * game_state.shadowWidthMax;
            light_frust_max[0]    =  0.5f * game_state.shadowWidthMax;
            light_frust_min[1]    = -0.5f * game_state.shadowWidthMax;
            light_frust_max[1]    =  0.5f * game_state.shadowWidthMax;
            light_frust_min[2]    = -0.5f * game_state.shadowLightSizeZ;
            light_frust_max[2]    =  0.5f * game_state.shadowLightSizeZ;
        }

        // Calculate light transforms and setup the shadow map viewport
        csmPrepareShadowMap( &g_ShadowMaps.maps[iSplit], shadowDir, light_origin, light_frust_min, light_frust_max );

        g_ShadowMaps.maps[iSplit].csm_eye_z_split_near = fNear;
        g_ShadowMaps.maps[iSplit].csm_eye_z_split_far = fFar;

    }

}

void rdrShadowMapUpdate(void)
{
    int wantNumShadows;

    // If shadowmaps are not a hardware supported feature just return from here
    // and never enter
    if ((rdr_caps.allowed_features & GFXF_SHADOWMAP) == 0 )
    {
        return;
    }

    {
        static bool once = true;
        if(once) {
            initShadowMapMenu();
            once = false;
        }
    }

    wantNumShadows = MIN(game_state.shadowmap_num_cascades, SHADOWMAP_MAX_SHADOWS);

    // see if we should recompile shaders to reflect a change in that setting
    // @todo need to find a way to precompile variations into assets or at
    // load time
    {
        if (game_state.shadowShaderSelection != game_state.shadowShaderInUse)
        {
            game_state.shadowShaderInUse = game_state.shadowShaderSelection;
            reloadShaderCallback(NULL,0);
        }
    }
    // freezing shadows lets us fly camera around to check them out without
    // any updates of the light maps or their transforms
    if (game_state.shadowMapFreeze)
    {
        return;
    }
    
    // if shadows are disabled then remove buffers and map rendering
    if (game_state.shadowMode < SHADOW_SHADOWMAP_LOW || editMode() /* || isIndoors() */ || isBaseMap())
    {
        // @todo - potential problem pulling out render targets from underneath render thread
        manageShadowBuffers(0, shadowMapDefs);
        rdrQueue(DRAWCMD_REFRESHSHADOWMAPS,&g_ShadowMaps,sizeof(g_ShadowMaps) );
        game_state.shadowMapsAllowed = 0;
        return;
    }

    game_state.shadowMapsAllowed = 1;

    // Setup the number of supported shadow maps
    manageShadowBuffers(wantNumShadows, shadowMapDefs);

    // update the cascade shadow maps
    csmUpdate();

    
    rdrQueue(DRAWCMD_REFRESHSHADOWMAPS,&g_ShadowMaps,sizeof(g_ShadowMaps) );

}

void rdrShadowMapDebug3D(void)
{
    if(game_state.shadowMapDebug & (showVolumes3D|showAxis|showFrustum|showSplits)) {
        rdrQueueCmd(DRAWCMD_SHADOWMAPDEBUG_3D);
    }
}
void rdrShadowMapDebug2D(void)
{
    if(game_state.shadowMapDebug & (showMaps2D|showOverview2D) ) {
        rdrSetup2DRendering();
        rdrQueueCmd(DRAWCMD_SHADOWMAPDEBUG_2D);
        rdrFinish2DRendering();
    }

    // debug text
    if(game_state.shadowMapDebug & showSplits)
    {
        float splits[SHADOWMAP_MAX_SHADOWS+1] = {-1.0f };

        // how many cascades are active?
        int iNumCascades = MIN(game_state.shadowmap_num_cascades, SHADOWMAP_MAX_SHADOWS);
        iNumCascades = MIN(iNumCascades, g_ShadowMaps.numShadows);
        csmCalcSplits( splits, iNumCascades, game_state.shadowSplitLambda, -game_state.nearz, -MIN(game_state.shadowFarMax, game_state.farz) );
        xyprintf(1, 62, "splits: %f %f %f %f %f", splits[0], splits[1], splits[2], splits[3], splits[4]);
    }

}


static bool shadowViewportPreCallback(ViewportInfo *viewport)
{
    shadowMap * map = (shadowMap*)viewport->callbackData;
    // freezing shadows lets us fly camera around to check them out without
    // any updates of the light maps or their transforms
    if (game_state.shadowMapFreeze)
    {
        return false;    // don't update the shadow map
    }

    gfx_state.writeDepthOnly = 1;
    rdrQueue(DRAWCMD_SHADOWVIEWPORT_PRECALLBACK,&map->index,sizeof(map->index));
    return true;
}

static bool shadowViewportPostCallback(ViewportInfo *viewport)
{
    shadowMap * map = (shadowMap*)viewport->callbackData;
    gfx_state.writeDepthOnly = 0;
    rdrQueue(DRAWCMD_SHADOWVIEWPORT_POSTCALLBACK,&map->index,sizeof(map->index));
    return true;
}

// called when we are beginning to render using shadowmaps
// sets up constants and samplers as necessary
void rdrShadowMapBeginUsing(void)
{
    // If shadowmaps are not a hardware supported feature just return from here
    // and never enter
    if ((rdr_caps.allowed_features & GFXF_SHADOWMAP) == 0 )
    {
        return;
    }
    rdrQueueCmd(DRAWCMD_SHADOWMAP_BEGIN_USING);
}

// called when we are done rendering with shadowmaps
void rdrShadowMapFinishUsing(void)
{
    // If shadowmaps are not a hardware supported feature just return from here
    // and never enter
    if ((rdr_caps.allowed_features & GFXF_SHADOWMAP) == 0 )
    {
        return;
    }
    rdrQueueCmd(DRAWCMD_SHADOWMAP_FINISH_USING);
}

///////////////////////////////////////////////////////////////////////////////
// CUSTOM SHADOWMAP RENDERING

// we have a custom render loop for the shadow maps to make it lean compared
// to the default

int g_hidden_def_discards;
int g_defs_processed;
int g_defs_rejected;
int g_numShadowDirectTris;

#include "render/renderstats.h"
#include "graphics/groupdraw.h"
#include "render/pbuffer.h"
#include "render/rendertree.h"
#include "render/renderprim.h"
#include "graphics/vistray.h"
#include "win/win_init.h"
#include "graphics/groupdrawinline.h"
#include "bases/basedraw.h"
#include "seq/tricks.h"
#include "seq/anim.h"
#include "entity/entclient.h"

static void groupDrawRefs_Shadowmap(Mat4 cam_mat, Mat4 inv_cam_mat)
{
    int            vis;
    DrawParams    draw;

    g_hidden_def_discards = g_defs_processed = g_defs_rejected = g_numShadowDirectTris = 0;

    //Init Draw structure ; only used internally by drawDefInternal
    memset(&draw,0,sizeof(draw));
    draw.view_scale = 1.0f;
    draw.view_cache_parent_index = -1;

    checkForGroupEntValidity();

    //Set Editor Visiblity overrides
    if ( game_state.see_everything & 1)
        group_draw.see_everything = 1;
    else
        group_draw.see_everything = 0;

    vis = VIS_NORMAL;
    if (group_draw.see_everything)
        vis = VIS_DRAWALL; 

    //Assorted inits
    group_draw.see_outside = 1;        //Just draw visible portals, or outside, too?
    ++group_draw.draw_id;                //Don't draw any portal more than once.

    copyMat4(inv_cam_mat, group_draw.inv_cam_mat);

    group_draw.zocclusion = false;    // currently no occlusion on shadow maps 

    group_draw.do_welding = !game_state.no_welding && !game_state.edit_base && vis != VIS_DRAWALL;

    baseStartDraw();

    //Draw portals
    // @todo currently we don't draw any shadows in interiors/trays so don't
    // traverse them
//     if (vis != VIS_DRAWALL)
//         vistrayDraw(&draw, cam_mat);

    if (group_draw.see_outside)
    {
        DefTracker    *ref;
        int    i;
        int current_group_uid = 0;

        PERFINFO_AUTO_START("Draw outside world - Shadowmap", 1);

        if (vis != VIS_DRAWALL)
            vistrayClearOutsideTrayList();

        for(i=0;i<group_info.ref_count;i++)
        {
            ref = group_info.refs[i];
            if (ref->def)
            {
                if (!ref->dyn_parent_tray)
                {
                    GroupEnt ent = {0};
                    makeAGroupEnt(&ent, ref->def, ref->mat, draw.view_scale, !!ref->dyn_parent_tray);
                    drawDefInternal_shadowmap(&ent, cam_mat, ref, vis, &draw, current_group_uid);
                }
                else
                {
                    vistraySetDetailUid(ref, current_group_uid);
                }

                current_group_uid += 1 + ref->def->recursive_count;
            }
        }

        if (vis != VIS_DRAWALL)
            vistrayDrawOutsideTrays(&draw, cam_mat);

        PERFINFO_AUTO_STOP();
    }
}

bool isShadowExtrusionVisible(Vec3 mid_vs, F32 radius)
{
    Vec3 mid_ws;
    Vec3 shadowmap_cast_extrusion;
    F32 extrusionDistance;

    if (radius < g_fMinModelRadius)
        return false;

    mulVecMat4(mid_vs, cam_info.inv_viewmat, mid_ws);

    extrusionDistance = (mid_ws[1] - gfx_state.lowest_visible_point_last_frame) * gfx_state.shadowmap_extrusion_scale;

    if (extrusionDistance > game_state.shadowLightSizeZ)
        extrusionDistance = game_state.shadowLightSizeZ;

    // For testing
    /*
    // Toggle between old fixed extrusion distance and new dynamic extrusion distance
    if (game_state.test6)
    {
        if (global_state.global_frame_count & 1)
        {
            extrusionDistance = game_state.shadowLightSizeZ;
        }
    }
    // Force old fixed extrusion distance
    else if (game_state.test5)
    {
        extrusionDistance = game_state.shadowLightSizeZ;
    }
    */

    scaleVec3(gfx_state.shadowmap_light_direction_vs, extrusionDistance, shadowmap_cast_extrusion );

    return !frustum_is_capsule_culled( &group_draw.shadowmap_cull_frustum, mid_vs, shadowmap_cast_extrusion, radius );
}

static void gfxTreeDrawNode_shadowmap(GfxNode *node,Mat4 parent_mat)
{
    SeqGfxData * seqGfxData;
    Mat4 viewspace;

    for(;node;node = node->next)
    {
        if( ( node->flags & GFXNODE_HIDE ) || ( node->seqHandle && !node->useFlags ) ) 
            continue; 

        //gfxGetAnimMatrices
        if( bone_IdIsValid(node->anim_id) && node->seqHandle ) 
        {    
            Mat4Ptr viewspace_scaled; //will be the 
            Vec3 xlate_to_hips;
            Mat4 scaled_xform;

            seqGfxData = node->seqGfxData; 
            assert( seqGfxData ); 
            assert( bone_IdIsValid(node->anim_id) );

            viewspace_scaled = seqGfxData->bpt[node->anim_id];

            //TO DO : I thought this optimization was good, but it messed up scaling.  
            //But now it's slow, so figure out a new optimized version
            // (above is an old Cryptic comment...investigate)
            if(0)
            { 
                copyVec3(node->mat[3], scaled_xform[3]);
                scaleMat3Vec3Xfer( node->mat, node->bonescale, scaled_xform ); 
            }
            else
            {    
                Mat4 scale_mat;
                copyMat4(unitmat, scale_mat); 
                scaleMat3Vec3(scale_mat,node->bonescale); //I think just Mat4 transfer will fix??
                mulMat4(node->mat, scale_mat, scaled_xform);

            }
            mulMat4(parent_mat, scaled_xform, viewspace_scaled );
            scaleVec3(seqGfxData->btt[node->anim_id], -1, xlate_to_hips);   //Prexlate this back to home
            mulVecMat4( xlate_to_hips, viewspace_scaled, viewspace_scaled[3] );//is this the right function

            //Added in here to avoid extra traversal by gfxtreesetalpha
            node->alpha = seqGfxData->alpha;
        }

        //If you are just a bone for skinning, you are done.
        if( !node->child && !node->model )
            continue;

        node->viewspace = viewspace;  //Just use the local cuz it's not used after this stack unwinds 

        mulMat4( parent_mat, node->mat, node->viewspace );

        //Add model to a draw hopper (must add to some hopper, cuz all bone xforms aren't done yet)
        // We also check the entity alpha against the snap value.
        // This is currently adjusted so that we don't render shadows on entity nodes which are
        // in stealth/phase shift/etc.
        if( node->model && !(node->flags & GFXNODE_HIDESINGLE) && (node->alpha >= game_state.shadowmap_lod_alpha_snap_entities ) && (node->model->loadstate & LOADED) )
        {
            if (node->clothobj && !ViewCache_InUse()) {
#if 0
                if (0 && ViewCache_InUse())
                {
                    if (!node->clothobj->GameData->system) {
                    
                        avsn_params.node = node;
                        avsn_params.mid = node->viewspace[3];
                        avsn_params.alpha = node->alpha;
                        avsn_params.rgbs = node->rgbs;
                        avsn_params.mat = node->viewspace;
                        addViewSortNodeSkinned(); //this adds to both dist and type sort bins
                    } else {
                        // Don't draw them here, draw them in particle systems! (but we still calculated the viewspace above, I think)
                    }
                }
#endif
            } else {
                // characters should still check if they cast into the view frustum but it can be much smaller test
                // though there are giant monsters and such to consider?

                // @todo shouldn't be relying on group_draw vars from the group traversal, move to higher level?

                // @todo need a screen space size metric to decide if we want character to draw into shadow or not
                // We need huge/boss characters to draw into the distant cascades
                F32 getScaleBounds(Mat3 mat);
                
                F32 scale = getScaleBounds(node->viewspace);

                if (/*!(node->flags & GFXNODE_NEEDSVISCHECK) || */isShadowExtrusionVisible(node->viewspace[3], scale * node->model->radius ))
                {
                    extern void gfxTreeDrawNodeInternal_shadowmap(GfxNode *node);
                    gfxTreeDrawNodeInternal_shadowmap(node);
                }
            }
        }

        if (node->child)
            gfxTreeDrawNode_shadowmap(node->child, node->viewspace);

        if (node->viewspace == viewspace)
            node->viewspace = NULL;
    }
}

static bool callback_ViewCacheVisibility(ViewCacheEntry *entry)
{
    if (game_state.shadowDebugFlags & kShadowCapsuleCull)
    {
        if ( !isShadowExtrusionVisible( entry->mid, entry->radius ) )
            return false;
    }
    else
    {
        if (!gfxSphereVisible(entry->mid, entry->radius))
            return false;
    }


    return true;
}

static void gfxRenderViewport_shadowmap(ViewportInfo *viewport)
{
    Mat4 M_view_main, M_inv_view_main;

    PERFINFO_AUTO_START("shadowmap traverse", 1);

    gfx_state.current_viewport_info = viewport;
    rdrStatsPassBegin(viewport->renderPass);    // recording time for this thread

    //******
    // 1. Gather up everything that is going to render into this shadowmap

    // @todo
    // At the moment we keep the lod scales the same as the main viewport
    // to that we are shadowing the same geo being rendered. This is necessary
    // to reduce unwanted shadow popping and also to allow higher quality
    // self shadowing. Probably helps or exacerbates acne artifacts depending on
    // the case.

    // While it would be nice to allow lower lod scaling and visibility for
    // shadowmaps to reduce load that probably wouldn't work out very well
    // in practice.

    //game_state.lod_entity_override = viewport->lod_entity_override;
    //game_state.lod_scale = viewport->lod_scale;

    // @todo at some point add this as a group_draw param instead of relying on the global state
    game_state.lod_scale = gfx_state.main_viewport_info->lod_scale;
    game_state.lod_entity_override = gfx_state.main_viewport_info->lod_entity_override;

    group_draw.see_outside = 1; //since see outside is used in multiple places now

    // do all the visibility and lod calculations with the main viewport camera
    // so that our results are relative to the main camera view frustum
    // @todo we will need to sweep bounding volumes along the light direction for
    // visibility culling and finally adjust the view matrices to the light
    // when we finally render the batches.
    // We just need to adjust the z near/far planes of the main view frustum for each 
    // cascade so we only consider the objects in the cascade's slice

    // setup the state we need for rendering with main camera view
    // settings but into shadowmap

    // Get the correct view/inv_view for traversal based on main viewport camera:
    // @todo should we just store these in the viewport when setup?
    gfxSetViewMat(gfx_state.main_viewport_info->cameraMatrix, M_view_main, M_inv_view_main);

    // copy into cam_info as well just in case for the moment (e.g. until ent and skinned objects
    // validated as ok w/o relying on these)
    copyMat4(gfx_state.main_viewport_info->cameraMatrix, cam_info.cammat);
    gfxSetViewMat(cam_info.cammat, cam_info.viewmat, cam_info.inv_viewmat);

    mulVecMat3(g_sun.direction, M_view_main, g_sun.direction_in_viewspace); 
    mulVecMat3(gfx_state.shadowmap_light_direction_ws, M_view_main, gfx_state.shadowmap_light_direction_vs); 
    sunGlareDisable(); // Must be before gfxTreeDrawNode?

    {
        shadowMap* map = (shadowMap*)viewport->callbackData;

        g_fMinModelRadius = map->minModelRadius;

        // Tweak the minimum model radius based on the cascade number.
        // This is to help reduce the number of objects which render into the far cascades
        //if (!game_state.test7 || global_state.global_frame_count & 1)
        {
            int cascadeNum = viewport->renderPass - RENDERPASS_SHADOW_CASCADE1;

            g_fMinModelRadius *= (cascadeNum + 1) * 2;
        }

        group_draw.znear = map->csm_eye_z_split_near;
        group_draw.zfar = map->csm_eye_z_split_far;
        // give a little overlap
        {
            float dz = (group_draw.zfar - group_draw.znear);
            group_draw.znear -= dz*game_state.shadowDebugPercent1;
            if ( group_draw.znear >= 0 )
                group_draw.znear = map->csm_eye_z_split_near;
            group_draw.zfar += dz*game_state.shadowDebugPercent2;
        }

        // @todo gfx_window has strange/wrong behavior with regard to FOV and aspect ratio
        // gfx_window does not actual store the correct fovx...but it's ratio to fovy is usually the correct
        // aspect ratio (as that is how they were passed)
        {
            float aspect = gfx_state.main_viewport_info->aspect;
            float fov_y  = gfx_state.main_viewport_info->fov + game_state.shadowmap_extra_fov;
            float fov_x = 2.0f * (fatan(aspect * ftan(RAD(fov_y)*0.5f)));
            frustum_build( &group_draw.shadowmap_cull_frustum, fov_x, aspect, fabs(group_draw.znear), fabs(group_draw.zfar) );

            // make sure visibility tests are going to be setup for main viewport
            // @todo the straight view cull relies on the gfx_window which needs to reshape based
            // on the main viewport info. Also, entity visibility updates for the gfx tree nodes
            // NOTE: we don't call the 'reshape' commands because those send render thread commands
            // we don't need or want
            {
                extern void gfxWindowSetAng(F32 fovy, F32 fovx, F32 znear, F32 zfar, bool bFlipY, bool bFlipX, int setMatrix);
                gfxWindowSetAng(fov_y, fov_y * aspect, fabs(group_draw.znear), fabs(group_draw.zfar),false, false, false );
            }
        }
    }

    //****
    // Setup grafx state and get render thread to init drawing to the fbo

    // @todo this has to be here to setup the projections/etc that is used
    // for lod and visibility...but we should extract that and move the render thread
    // stuff down to be with the bulk model rendering
    // we want to encapsulate all the drawing stuff and make the render thread work on us
    // until we are completely ready (when it could be doing someone elses work on multi-core)

    gfx_state.vis_limit_entity_dist_from_player = viewport->vis_limit_entity_dist_from_player;
    gfx_state.renderPass = viewport->renderPass;
    gfx_state.stencilAvailable = pbufStencilSupported(viewport_GetPBuffer(viewport)->flags);
    gfx_state.mainViewport = 0;
    gfx_state.stencilAvailable = false;
    gfx_state.waterThisFrame = 0;
    gfx_state.lod_model_override = 0;

    game_state.zocclusion = 0;

    //****
    // walk the scene graph and collect the models we are
    // going to render into the shadowmap

    gfxTreeFrameInit();

    vistrayClearVisibleList();

    if (game_state.game_mode == SHOW_GAME)
    {
        bool runUpdates = ViewCache_Update(viewport->renderPass);

        if (runUpdates)
        {
            // Traverse world geo scene graph (GroupDef tree)
            PERFINFO_AUTO_START("shadowmap traverse groups",1);
            {
                // do the traversal of world geo
                groupDrawRefs_Shadowmap( M_view_main, M_inv_view_main );
            }
            PERFINFO_AUTO_STOP();

        }

        ViewCache_Use(callback_ViewCacheVisibility, viewport->renderPass);
        {
            bool bRenderEntities = viewport->renderCharacters;
#ifndef FINAL
            bRenderEntities = bRenderEntities && !GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_GFXNODE_TREE_TRAVERSAL);
#endif
            // if we are in an interior then we currently arent' rendering entites (or anything else)
            // into the shadowmap
            // @todo Note that this implicitly uses fact that shadowmaps are rendering with the mainviewport
            // camera which isn't good.
            // Also this should be removed when we want to render character shadows into interiors
            if (isIndoors())
            {
                bRenderEntities = bRenderEntities && ((game_state.shadowDebugFlags&kRenderSkinnedInteriorIntoMap)!=0);
            }

            vistraySortVisibleList();

            if( bRenderEntities )
            {
                // @todo need a scaled down version of this that does
                // minimal and has minimal impact for shadowmaps
                PERFINFO_AUTO_START("shadowmap entClientProcessVisibility",1);
                    entClientProcessVisibility();
                PERFINFO_AUTO_STOP_CHECKED("shadowmap entClientProcessVisibility");

                // Traverse the entities 'gfx tree' nodes which will contain
                // the skinned nodes. Note that this will also cause some
                // static nodes to be added the same way as group models
                PERFINFO_AUTO_START("shadowmap traverse ent gfxtree",1);

                {
                    Mat4 M_shadow_node_traversal_view; 

                    if (1)
                    {
                        // do traversal and culling from main viewport camera view
                        // requires that bone matrices be 'fixed-up' to be relative to
                        // shadow light view before rendering
                        copyMat4(M_view_main,M_shadow_node_traversal_view);
                    }
                    else
                    {
                        // @todo is this workable opt?
                        // answer: not for the moment

                        // We already did the visibility and culling relative to the main camera view
                        // above, we can walk the tree now with the shadow light view as the parent matrix
                        // so we don't have to fix up the bone matrices after the fact
                        // Just need to be careful about world models that are added for this time for attached
                        // objects like caps/shields/weapons so we don't fix up those matrices when they go
                        // through the non-skinned world object path

                        // calculate view/inv_view for SHADOW LIGHT view (i.e. this viewports camera, not main view)

                        gfxSetViewMat(viewport->cameraMatrix, M_shadow_node_traversal_view, NULL);
                    }

                    gfxTreeDrawNode_shadowmap( gfx_tree_root, M_shadow_node_traversal_view); 
                }
                PERFINFO_AUTO_STOP();
            }
        }
    }

    gfxTreeFrameFinishQueueing();

    //******
    // 2.        Now that we know everything we are going to render
    //            sort it to reduce state changes and API calls (eg. use instancing if available)

    PERFINFO_AUTO_STOP_START("sortModels_shadow", 1);
    sortModels(viewport->renderOpaquePass, viewport->renderAlphaPass);

    //******
    // 3.    Deliver commands necessary to render shadow map to the Render Thread

    // we want to encapsulate all the drawing stuff and not have the render thread waiting on us
    // until we are completely ready (when it could be doing another render target task on multi-core)

    // setup for shadowmap rendering
    // @todo combine necessary information for setup into header block of single
    // command which encapsulates all of the data
    PERFINFO_AUTO_STOP_START("Send RT Cmds-Shadowmap Start", 1);
    {
        Mat4 M_view; 
        Mat4 M_inv_view;

        assert(viewport && viewport->isRenderToTexture);
        pbufMakeCurrent( viewport_GetPBuffer(viewport) );

        // calculate view/inv_view for SHADOW LIGHT view (i.e. this viewports camera, not main view)
        gfxSetViewMat(viewport->cameraMatrix, M_view, M_inv_view);

        rdrInitTopOfView(M_view,M_inv_view);
        if (viewport->viewport_width == -1 || viewport->viewport_height == -1)
            rdrViewport(0, 0, viewport->width,viewport->height, false);
        else
            rdrViewport(viewport->viewport_x, viewport->viewport_y, viewport->viewport_width,viewport->viewport_height, true);

        rdrSetupOrtho(viewport->left, viewport->right, viewport->bottom, viewport->top, viewport->nearZ, viewport->farZ );
        rdrSendProjectionMatrix();
        rdrClearScreen();
    }

    PERFINFO_AUTO_STOP_START("renderOpaquePass_shadow",1);
    drawSortedModels_shadowmap_new(viewport);

#ifndef FINAL
    if (game_state.renderdump)
    {
        gfxRenderDump("%s_shadowmap", viewport->name);
    }
#endif

    //**
    // Clean up

    // Zero the AVSN params because the normal graph walks expects this
    ZeroStruct(&avsn_params);
    avsn_params.uid = -1;

    winMakeCurrent();    // This also sends render queue command (i.e. DRAWCMD_WINMAKECURRENT)

    PERFINFO_AUTO_STOP();

    rdrStatsPassEnd(viewport->renderPass);    // recorded time for this thread

    gfx_state.current_viewport_info = NULL;
}

static bool shadowViewportCustomRenderCallback(ViewportInfo *viewport)
{
    if (game_state.shadowDebugFlags&kUseCustomRenderLoop)
    {
        gfxRenderViewport_shadowmap(viewport);            // leaner
    }
    else
    {        
        gfxRenderViewport(viewport);            // use default
    }
    return true;
}

// These functions are used to decide if we are in a mode
// and location where batches should be rendered with shadow
// map lookups.
unsigned int rdrShadowMapCanWorldRenderWithShadows(void)
{
    if ( game_state.shadowMode < SHADOW_SHADOWMAP_LOW )
        return false;
    
    if ( gfx_state.renderPass != RENDERPASS_COLOR )
        return false;    // only do lookups on main viewport (i.e., not on reflections)

#ifndef FINAL
    if ( editMode() )
        return false;

    if ( !(game_state.shadowDebugFlags&renderUsingMaps) )
        return false;
#endif

    if ( isBaseMap() )
        return false;

    return true;    // ok to render with shadowmap lookups
}

unsigned int rdrShadowMapCanEntityRenderWithShadows(void)
{
    if ( !rdrShadowMapCanWorldRenderWithShadows() )
        return false;

    // We don't want entities to render with shadowmaps when we are
    // in a tray.
    // @todo - a more precise test would be to test each entity if it
    // is in a tray when setting the render params. This is more expedient
    // at the moment though.
    // It can be a noticeable limitation, see TTP #21110 
    // For example, you are inside Architect Entertainment building and looking
    // outside. None of the entities on the outside will cast shadows though the
    // outside world geometry will render shadows.
    if ( isIndoors() )
        return false;    

    return true;    // ok to render with shadowmap lookups
}
