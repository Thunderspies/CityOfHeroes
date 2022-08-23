//
//    rt_cubemap.c
//
#define RT_PRIVATE

#include <utilitieslib/utils/SuperAssert.h>
#include "graphics/failtext.h"
#include "cmdparse/cmdgame.h"
#include "render/cubemap.h"
#include "render/renderWater.h"
#include "render/thread/rt_cubemap.h"
#include "render/thread/rt_tune.h"
#include "graphics/gfx.h"
#include "render/thread/ogl.h"
#include "render/tex.h"
#include "render/thread/rt_init.h"
#include "UI/sprite/sprite_base.h"
#include <utilitieslib/utils/mathutil.h>
#include "graphics/camera.h"
#include "render/renderUtil.h"
#include "render/thread/wcw_statemgmt.h"
#include "seq/seq.h" //MAX_LODS
#include "render/renderssao.h"
#include "graphics/gfxSettings.h"
#include "rt_filter.h"
#include "render/thread/rt_pbuffer.h"

typedef struct CubemapRenderGlobals
{
    int        blurMode;
    int        blurRadius;
    float    blurSigma[4];
    int        blurImplementation;
} CubemapRenderGlobals;

static CubemapRenderGlobals g_rt_CubemapOpts    = {FILTER_NO_BLUR, 1, {2.0f, 1.0f, 0.0003f, 0.0f}, 0 };
static CubemapRenderGlobals g_rt_PlanarRefOpts    = {FILTER_NO_BLUR, 1, {2.0f, 1.0f, 0.0003f, 0.0f}, 0 };

void rt_cubemapViewportPreCallback(ViewportInfo** ppViewport)
{
    ViewportInfo* pViewportInfo = *ppViewport;
    PBuffer* pbuf = viewport_GetPBuffer(pViewportInfo);
    
    rdrBeginMarker(__FUNCTION__);

    // let the pbuffer know which face is current so it renders to the correct face
    pbuf->currentCubemapTarget = pViewportInfo->target;
    assert(pbuf->currentCubemapTarget>=GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB && pbuf->currentCubemapTarget<=GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB);
    if(pViewportInfo->bFlipYProjection ^ pViewportInfo->bFlipXProjection) {
        glFrontFace(GL_CCW); CHECKGL;
    }
    rdrEndMarker();
}

void rt_cubemapViewportPostCallback(ViewportInfo** ppViewport)
{
    int blurMode = g_rt_CubemapOpts.blurMode;
    ViewportInfo* pViewportInfo = *ppViewport;
    PBuffer* pbuf = viewport_GetPBuffer(pViewportInfo);

    rdrBeginMarker(__FUNCTION__);

    if(pViewportInfo->bFlipYProjection ^ pViewportInfo->bFlipXProjection) {
        glFrontFace(GL_CW); CHECKGL;
    }

    // blur the cubemap face
    // @todo this is the evaluation prototype blur code which is not
    // used in production. If this is going to be enabled it should be
    // reworked.
    if(blurMode)
    {
        RdrFilterParams params;

        params.srcTex = pbuf->color_handle[pbuf->curFBO];
        params.srcTarget = pbuf->currentCubemapTarget;

        params.mode    = blurMode;
        params.width = pViewportInfo->width;
        params.height = pViewportInfo->height;
        
        rdrFilterRenderDirect( &params );
    }

    if (game_state.cubemapUpdate3Faces)
    {
        int target = -1;

        if (pbuf->currentCubemapTarget == GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB)
        {
            target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB;
        }
        else if (pbuf->currentCubemapTarget == GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB)
        {
            target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB;
        }

        if (target != -1)
        {
            winCopyFrameBufferDirect(target, pbuf->color_handle[pbuf->curFBO], 0, 0, 0, pbuf->width, pbuf->height, 0, true, false);
        }
    }

    // Force the blit to the cubemap texture
    winMakeCurrentDirect();
    rdrEndMarker();
}

void rt_reflectionViewportPostCallback(ViewportInfo* pViewportInfo)
{
    rdrBeginMarker(__FUNCTION__);

    // blur the render target
    if (g_rt_PlanarRefOpts.blurMode)
    {
        PBuffer* pbuf = viewport_GetPBuffer(pViewportInfo);
        if (g_rt_PlanarRefOpts.blurImplementation)
        {
            RdrFilterParams params;
            PBuffer* pbuf = viewport_GetPBuffer(pViewportInfo);

            params.srcTex = pbuf->color_handle[pbuf->curFBO];
            params.srcTarget = GL_TEXTURE_2D;

            params.mode    = g_rt_PlanarRefOpts.blurMode;
            params.width = pViewportInfo->width;
            params.height = pViewportInfo->height;

            rdrFilterRenderDirect( &params );
        }
        else
        {
            rdrFilterRenderTargetDirect( pbuf, g_rt_PlanarRefOpts.blurMode );
        }
    }
    rdrEndMarker();
}

void rt_cubemapSetInvCamMatrixDirect(void)
{
    // Pass inverse camera matrix as a float3x3
    WCW_SetCgShaderParamArray3fv( kShaderPgmType_VERTEX, kShaderParam_InverseCubemapViewMatrixVP, rdr_view_state.inv_viewmat[0], 3 );
    WCW_SetCgShaderParamArray3fv( kShaderPgmType_FRAGMENT, kShaderParam_InverseCubemapViewMatrixFP, rdr_view_state.inv_viewmat[0], 3 );

}

void rt_cubemapSet3FaceMatrixDirect(void)
{
    static Mat3 invCubemapViewMatrix;
    static bool generateMatrix = true;

    if (generateMatrix)
    {
        Vec3 pyr;
        Mat3 cubemapViewMatrix;

        generateMatrix = false;

        zeroVec3(pyr);
        pyr[1] -= HALFPI * 0.5f;
        pyr[0] += HALFPI * 0.5f;

        createMat3PYR(cubemapViewMatrix, pyr);

        cubemapViewMatrix[0][0] *= -1;
        cubemapViewMatrix[1][0] *= -1;
        cubemapViewMatrix[2][0] *= -1;

        invertMat3(cubemapViewMatrix, invCubemapViewMatrix);
    }

    WCW_SetCgShaderParamArray3fv( kShaderPgmType_VERTEX, kShaderParam_InverseCubemapViewMatrixVP, invCubemapViewMatrix[0], 3 );
    WCW_SetCgShaderParamArray3fv( kShaderPgmType_FRAGMENT, kShaderParam_InverseCubemapViewMatrixFP, invCubemapViewMatrix[0], 3 );
}
static int g_TimesetValue;

void rt_initCubeMapMenu()
{
    static int separator1 = 0;
    static int separator2 = 0;

    static const int cmTypeValues[] = {1, 0};
    static const char * cmTypeNames[] = {"Static", "Dynamic", NULL};
    static const int cmPlanarReflectionValues[] = {0, 1, 2, 3};
    static const char * cmPlanarReflectionNames[] = {"None", "Water", "Buildings", "Water and Buildings", NULL};
    static const int cmReflectionDebugValues[] = {0, 1, 2};
    static const char * cmReflectionDebugNames[] = {"None", "Highlight", "Show Texture", NULL};
    static const int cmNoYesDebugValues[] = {0, 1};
    static const char * cmNoYesDebugNames[] = {"No", "Yes", NULL};
    static const int cubemapModeValues[] = {0, 1, 2, 3, 4, 5};
    static const char * cubemapModeNames[] = {"Off", "low quality", "medium quality", "high quality", "ultra quality", "debug super HQ", NULL};
    static const int cubemapForceFaceValues[] = {-1, 0, 1, 2, 3, 4, 5, 6};
    static const char * cubemapForceFaceNames[] = {"All faces each frame (slow)", "One per frame", "Face 1", 
                            "Face 2", "Face 3", "Face 4", "Face 5", "Face 6", NULL};

    static int blurModes[] = {FILTER_NO_BLUR, FILTER_FAST_BLUR, FILTER_GAUSSIAN};
    static const char * blurNames[] = { "None", "2D Fast Blur", "Two Pass 1D Gaussian", NULL };

    static int blurModesPlanar[] = {FILTER_NO_BLUR, FILTER_FAST_ALPHA_BLUR, FILTER_FAST_BLUR, FILTER_GAUSSIAN};
    static const char * blurNamesPlanar[] = { "None", "2D Alpha Blur", "2D Fast Blur", "Two Pass 1D Gaussian", NULL };

    static int blurImplementationPlanar[] = {0, 1};
    static const char * blurImplementationPlanarNames[] = { "New", "Old", NULL };

    static int reflectModesPlanar[] = {kReflectNearby, kReflectEntitiesOnly, kReflectAll};
    static const char * reflectModesPlanarNames[] = { "nearby", "entities only", "full", NULL };

    static int reflectShearProjClip[] = {kReflectProjClip_None, kReflectProjClip_EntitiesOnly, kReflectProjClip_All};
    static const char * reflectShearProjClipNames[] = { "none", "skinned only", "all", NULL };

    // Initial values are either set in gameStateInit() or values exposed to the user are
    // initialized in gfxGetInitialSettings() and possibly overridden by getAutoResumeInfoFromRegistry()
    // called from game_beforeParseArgs()

    // Some values may be overridden in gfxApplySettings() when graphics options change
    g_rt_PlanarRefOpts.blurMode = FILTER_FAST_ALPHA_BLUR; // default blur is the alpha blur if enabled ( env reflection cubemap mode quality is high enough)

    // planar reflections
    tunePushMenu("Planar Reflections");
    tuneEnum("Enable Reflections", &game_state.reflectionEnable, cmPlanarReflectionValues, cmPlanarReflectionNames, NULL);
    tuneInteger("Texture size", &game_state.reflection_generic_fbo_size, 128, 128, 1024, NULL);
    tuneEnum("Use two reflection viewports", &game_state.separate_reflection_viewport, cmNoYesDebugValues, cmNoYesDebugNames, NULL);
    tuneInteger("-----------------", &separator1, 1, 0, 0, NULL);
    tuneFloat("Water Min Screen Size", &game_state.water_reflection_min_screen_size, 0.01f, 0.0f, 1.0f, NULL);
    tuneFloat("Water Ent Vis Dist From Player)", &game_state.reflection_water_entity_vis_limit_dist_from_player, 1.0f, 0.0f, 1000.0f, NULL);
    tuneFloat("Water Fresnel Bias", &game_state.waterFresnelBias, 0.01f, 0.0f, 1.0f, NULL);
    tuneFloat("Water Fresnel Scale", &game_state.waterFresnelScale, 0.01f, 0.0, 10.0f, NULL);
    tuneFloat("Water Fresnel Power", &game_state.waterFresnelPower, 0.1f, 1.0f, 10.0f, NULL);
    tuneFloat("Water Night Reduction", &game_state.waterReflectionNightReduction, 0.05f, 0.0f, 1.0f, NULL);
    tuneFloat("Water Reflection Skew Scale", &game_state.waterReflectionSkewScale, 0.01f, 0.0f, 1.0f, NULL);
    tuneFloat("Water Lod Scale", &game_state.reflection_water_lod_scale, 0.001f, 0.001f, 1, NULL);    
    tuneInteger("Water Model Lod Override", &game_state.reflection_water_lod_model_override, 1, 0, MAX_LODS, NULL);
    tuneEnum("Water sky only reflection", &game_state.allow_simple_water_reflection, cmNoYesDebugValues, cmNoYesDebugNames, NULL);
    tuneInteger("------------------", &separator2, 1, 0, 0, NULL);
    tuneEnum("Planar Reflection Mode", &game_state.reflection_planar_mode, reflectModesPlanar, reflectModesPlanarNames, NULL);
    tuneBit("Enable Reflectivity Override", &game_state.reflectivityOverride, 1, NULL);
    tuneFloat("Building Fresnel Bias", &game_state.buildingFresnelBias, 0.01f, 0.0f, 1.0f, NULL);
    tuneFloat("Building Fresnel Scale", &game_state.buildingFresnelScale, 0.01f, 0.0, 10.0f, NULL);
    tuneFloat("Building Fresnel Power", &game_state.buildingFresnelPower, 0.1f, 1.0f, 10.0f, NULL);
    tuneFloat("Building Night Reduction", &game_state.buildingReflectionNightReduction, 0.05f, 0.0f, 1.0f, NULL);
    tuneFloat("Building Lod Scale", &game_state.reflection_planar_lod_scale, 0.0001f, 0.0f, 1, NULL);    
    tuneInteger("Building Model Lod Override", &game_state.reflection_planar_lod_model_override, 1, 0, MAX_LODS, NULL);
    tuneFloat("Building reflect fade begin", &game_state.reflection_planar_fade_begin, 0.5, 0.0f, 500.0f, NULL);    
    tuneFloat("Building reflect fade end", &game_state.reflection_planar_fade_end, 0.5, 0.0f, 1000.0f, NULL);    
    //tuneFloat("Planar reflection MIP bias", &game_state.planar_reflection_mip_bias, 0.1f, 0.0f, 4.0f, NULL);    
    tuneFloat("Entity Visible Dist From Player)", &game_state.reflection_planar_entity_vis_limit_dist_from_player, 1.0f, 0.0f, 1000.0f, NULL);
    tuneEnum("Reflection Debug", &game_state.reflectionDebug, cmReflectionDebugValues, cmReflectionDebugNames, NULL);
    tuneEnum("Blur Filter", &g_rt_PlanarRefOpts.blurMode, blurModesPlanar, blurNamesPlanar, NULL);
    tuneEnum("Blur Implementation", &g_rt_PlanarRefOpts.blurImplementation, blurImplementationPlanar, blurImplementationPlanarNames, NULL);
    tuneEnum("Projection Clipping Mode", &game_state.reflection_proj_clip_mode, reflectShearProjClip, reflectShearProjClipNames, NULL);

    tunePopMenu();

    // cubemaps
    tunePushMenu("Cubemap");
    tuneEnum("Cubemap Type - chars", &game_state.static_cubemap_chars, cmTypeValues, cmTypeNames, NULL);
    tuneEnum("Cubemap Type - terrain", &game_state.static_cubemap_terrain, cmTypeValues, cmTypeNames, NULL);
    tuneEnum("Blur Filter (Test)", &g_rt_CubemapOpts.blurMode, blurModes, blurNames, NULL);

    // disable on this menu since causes issues, uses submenu instead -- tuneBit("Generate Static Now", &game_state.cubemap_flags, kGenerateStatic, NULL);
    tunePushMenu("Static Generation Options");
    tuneBit("Generate Static Now", &game_state.cubemap_flags, kGenerateStatic, NULL);
    tuneInteger("Capture Texture size", &game_state.cubemap_size_capture, 128, 128, 1024, NULL);
    tuneInteger("Gen Texture size", &game_state.cubemap_size_gen, 128, 128, 1024, NULL);
    tuneBit("Reload Generic Static On Gen", &game_state.cubemap_flags, kReloadStaticOnGen, NULL);
    tuneBit("Save Faces On Gen", &game_state.cubemap_flags, kSaveFacesOnGen, NULL);
    tuneBit("Render Alpha Geometry", &game_state.cubemap_flags,  kCubemapStaticGen_RenderAlpha, NULL);
    tuneBit("Render Particles", &game_state.cubemap_flags,  kCubemapStaticGen_RenderParticles, NULL);
    tunePopMenu();

    tuneEnum("Dynamic Mode", &((int)game_state.cubemapMode), cubemapModeValues, cubemapModeNames, NULL);
    tuneInteger("Dynamic texture size", &game_state.cubemap_size, 128, 128, 1024, NULL);
    tuneEnum("Render single face", &game_state.forceCubeFace, cubemapForceFaceValues, cubemapForceFaceNames, NULL);
    tuneInteger("Display face", &game_state.display_cubeface, 1, 0, 6, NULL);
    tuneEnum("Enable 4 face update cycle", &game_state.cubemapUpdate4Faces, cmNoYesDebugValues, cmNoYesDebugNames, NULL);
    tuneEnum("Enable 3 face update cycle", &game_state.cubemapUpdate3Faces, cmNoYesDebugValues, cmNoYesDebugNames, NULL);
    tuneBit("Render chars in cubemap", &game_state.charsInCubemap, 1, NULL);
    tuneFloat("Entity Visible Dist From Player)", &game_state.reflection_cubemap_entity_vis_limit_dist_from_player, 1.0f, 0.0f, 1000.0f, NULL);
    tuneInteger("Lod Model Bias", &game_state.reflection_cubemap_lod_model_override, 1, 0, 300, NULL);    
    tuneBit("Enable Reflectivity Override", &game_state.reflectivityOverride, 1, NULL);
    tuneFloat("Building Fresnel Bias", &game_state.buildingFresnelBias, 0.01f, 0.0f, 1.0f, NULL);
    tuneFloat("Building Fresnel Scale", &game_state.buildingFresnelScale, 0.01f, 0.0, 10.0f, NULL);
    tuneFloat("Building Fresnel Power", &game_state.buildingFresnelPower, 0.1f, 1.0f, 10.0f, NULL);
    tuneFloat("Skinned Fresnel Bias", &game_state.skinnedFresnelBias, 0.01f, 0.0f, 1.0f, NULL);
    tuneFloat("Skinned Fresnel Scale", &game_state.skinnedFresnelScale, 0.01f, 0.0, 10.0f, NULL);
    tuneFloat("Skinned Fresnel Power", &game_state.skinnedFresnelPower, 0.1f, 1.0f, 10.0f, NULL);
    tuneFloat("Cubemap reflection MIP bias", &game_state.cubemap_reflection_mip_bias, 0.1f, 0.0f, 4.0f, NULL);    
    //tuneFloat("Planar reflection MIP bias", &game_state.planar_reflection_mip_bias, 0.1f, 0.0f, 4.0f, NULL);    
    tuneFloat("Reflection Attenuation Start", &game_state.cubemapAttenuationStart, 0.1f, 0.0f, 100.0f, NULL);
    tuneFloat("Reflection Attenuation End", &game_state.cubemapAttenuationEnd, 0.1f, 0.1f, 100.0f, NULL);
    tuneFloat("Reflection Attenuation Skin Start", &game_state.cubemapAttenuationSkinStart, 0.1f, 0.0f, 100.0f, NULL);
    tuneFloat("Reflection Attenuation Skin End", &game_state.cubemapAttenuationSkinEnd, 0.1f, 0.1f, 100.0f, NULL);
    tuneBit("Reflect using normalmap", &game_state.reflectNormals, 1, NULL);
    tuneFloat("Reflect distortion map strength", &game_state.reflection_distortion_strength, 0.005f, 0.0f, 1.0f, NULL);
    tuneFloat("Reflect base map strength", &game_state.reflection_base_strength, 0.005f, 0.0f, 1.0f, NULL);
    tuneFloat("Reflect distort mix", &game_state.reflection_distort_mix, 0.005f, 0.0f, 1.0f, NULL);
    tuneFloat("Base Normal Fade Begin", &game_state.reflection_base_normal_fade_near, 1.0f, 0.0f, 1000.0f, NULL);
    tuneFloat("Base Normal Fade Range", &game_state.reflection_base_normal_fade_range, 1.0f, 0.0f, 500.0f, NULL);

    tunePopMenu();
}
