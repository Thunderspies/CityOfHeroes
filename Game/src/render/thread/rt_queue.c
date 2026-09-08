#define RT_PRIVATE
#include "win/win_init.h"
#include "render/thread/rt_queue.h"
#include "assert.h"
#include "render/rendertree.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/file.h>
#include "graphics/gfx.h"
#include <utilitieslib/utils/timing.h>
#include "graphics/light.h"
#include "graphics/sun.h"
#include "UI/uiCursor.h"
#include "render/renderUtil.h"
#include "render/thread/ogl.h"
#include <utilitieslib/utils/osdependent.h>
#include "win/win_init.h"
#include "render/renderfont.h"
#include "render/sprite.h"
#include "win/win_cursor.h"
#include "render/rendermodel.h"
#include "render/renderbonedmodel.h"
#include "render/pbuffer.h"
#include "render/rendershadow.h"
#include "render/model_cache.h"
#include "rt_prim.h"
#include "render/renderparticles.h"
#include "render/rendercloth.h"
#include "render/thread/rt_cgfx.h"
#include "render/renderUtil.h"
#include "render/thread/wcw_statemgmt.h"
#include "render/thread/rt_sprite.h"
#include "render/thread/rt_tex.h"
#include "render/thread/rt_perf.h"
#include "rt_stats.h"
#include "render/thread/rt_effects.h"
#include "render/thread/rt_water.h"
#include "rt_win_init.h"
#include "render/thread/rt_tricks.h"
#include "render/thread/rt_ssao.h"
#include "render/thread/rt_tune.h"
#include "render/thread/rt_shadowmap.h"
#include "render/thread/rt_cubemap.h"
#include "rt_shaderMgr.h"
#include "rt_filter.h"
#include "render/thread/rt_util.h"

WorkerThread *render_thread = 0;
static int g_run_render_thread = 0;

RdrFrameState rdr_frame_state;
RdrViewState rdr_view_state;
static GLuint tex_handles[1024];
static volatile int tex_handle_count;

static GLuint buffer_handles[1024];
static volatile int buffer_handle_count;

static void rdrGenTexHandles()
{
    int i;
    glGenTextures(ARRAY_SIZE(tex_handles),tex_handles); CHECKGL;
    // Flip the order of them so they get used starting from 1 again
    for (i=0; i<ARRAY_SIZE(tex_handles)/2; i++)
    {
        GLuint t = tex_handles[i];
        tex_handles[i] = tex_handles[ARRAY_SIZE(tex_handles) - i - 1];
        tex_handles[ARRAY_SIZE(tex_handles) - i - 1] = t;
    }
    tex_handle_count = ARRAY_SIZE(tex_handles);
}

static void rdrSetup(void)
{
    windowInitDisplayContextsDirect();
    rdrInitExtensionsDirect();
    rt_cgfxInit();
    rt_InitDebugTuningMenuItems();
}

static void rt_ViewportGenericPreCallbackDirect(ViewportInfo** ppViewport)
{
}

static void rt_ViewportGenericPostCallbackDirect(ViewportInfo** ppViewport)
{
    ViewportInfo* pViewportInfo = *ppViewport;
    if (rdr_view_state.renderPass == RENDERPASS_REFLECTION2)
    {
        rt_reflectionViewportPostCallback(pViewportInfo);
    }
}


static DrawType cmdDispatch_last_type;
static void *cmdDispatch_last_data;
static void cmdDispatch(void *unused, DrawType type, void *data)
{
    // Copy to globals for debugging
    cmdDispatch_last_type = type;
    cmdDispatch_last_data = data;    
    INEXACT_CHECKGL;
    switch(type)
    {
        xcase DRAWCMD_RDRSETUP:
            rdrSetup();

        // general
        xcase DRAWCMD_CLEAR:
            rdrClearScreenDirectEx(*(int*)data);
        xcase DRAWCMD_PERSPECTIVE:
            rdrPerspectiveDirect(data);
        xcase DRAWCMD_VIEWPORT:
            rdrViewportDirect(data);
        xcase DRAWCMD_SWAPBUFFER:
            wtSetFlushRequested(render_thread, 0);
            windowUpdateDirect();
        xcase DRAWCMD_INITDEFAULTS:
            rdrInitDirect();
        xcase DRAWCMD_FOG:
            rdrSetFogDirect(data);
        xcase DRAWCMD_BGCOLOR:
            rdrSetBgColorDirect(data);

        // get data from gl
        xcase DRAWCMD_GETFRAMEBUFFER:
            rdrGetFrameBufferDirect(data);
        xcase DRAWCMD_GETTEXINFO:
            rdrGetTexInfoDirect(data);

        // texture loading
        xcase DRAWCMD_TEXFREE:
            rdrTexFreeDirect(*(int*)data);
        xcase DRAWCMD_TEXCOPY:
            texCopyDirect(data);
        xcase DRAWCMD_TEXSUBCOPY:
            texSubCopyDirect(data);
        xcase DRAWCMD_GENTEXHANDLES:
            rdrGenTexHandles();

        // buffers
        xcase DRAWCMD_GENBUFFERHANDLES:
            glGenBuffersARB(ARRAY_SIZE(buffer_handles),buffer_handles); CHECKGL;
            buffer_handle_count = ARRAY_SIZE(buffer_handles);
        xcase DRAWCMD_FREEBUFFER:
            WCW_DeleteBuffers(GL_ARRAY_BUFFER_ARB,1,data,(char*)data+sizeof(int));
        xcase DRAWCMD_FREEMEM:
            free(*(void**)data);

        // blits
        xcase DRAWCMD_SETUP2D:
            rdrSetup2DRenderingDirect();
        xcase DRAWCMD_FINISH2D:
            rdrFinish2DRenderingDirect();
        xcase DRAWCMD_SPRITE:
            drawSpriteDirect(data);
        xcase DRAWCMD_SPRITES:
            drawAllSpritesDirect(data);
        xcase DRAWCMD_SIMPLETEXT:
            rdrSimpleTextDirect(data);
        xcase DRAWCMD_DYNAMICQUAD:
            rdrDynamicQuad(data);

        // lines, tris, boxes (debug utils mainly)
        xcase DRAWCMD_LINE2D:
            drawLine2DDirect(data);
        xcase DRAWCMD_LINE3D:
            drawLine3DDirect(data);
        xcase DRAWCMD_FILLEDBOX:
            drawFilledBoxDirect(data);
        xcase DRAWCMD_FILLEDTRI:
            drawTriangle3DDirect(data);

        // cursor
        xcase DRAWCMD_CURSORCLEAR:
            hwcursorClearDirect();
        xcase DRAWCMD_CURSORBLIT:
            hwcursorBlitDirect(data);

        // 3d models
        xcase DRAWCMD_MODEL:
            modelDrawDirect(data);
        xcase DRAWCMD_MODELALPHASORTHACK:
            modelDrawAlphaSortHackDirect(data);
        xcase DRAWCMD_BONEDMODEL:
            modelDrawBonedNodeDirect(data);
        xcase DRAWCMD_CONVERTRGBBUFFER:
            modelConvertRgbBufferDirect(data);
        xcase DRAWCMD_CREATEVBO:
            createVboDirect(data);
        xcase DRAWCMD_FREEVBO:
            modelFreeVboDirect(data);
        xcase DRAWCMD_RESETVBOS:
            modelResetVBOsDirect();

        // performance testing
        xcase DRAWCMD_PERFTEST:
            rtPerfTest(data);
        xcase DRAWCMD_PERFSET:
            rtPerfSet(data);

        // pbuffer
        xcase DRAWCMD_PBUFMAKECURRENT:
            pbufMakeCurrentDirect(*((PBuffer **)data));
        xcase DRAWCMD_WINMAKECURRENT:
            winMakeCurrentDirect();
        xcase DRAWCMD_PBUFINIT:
            pbufInitDirect(data);
        xcase DRAWCMD_PBUFRELEASE:
            pbufReleaseDirect(*((PBuffer **)data));
        xcase DRAWCMD_PBUFBIND:
            pbufBindDirect(*((PBuffer **)data));
        xcase DRAWCMD_PBUFDELETE:
            pbufDeleteDirect(data);
        xcase DRAWCMD_PBUFDELETEALL:
            pbufDeleteAllDirect();
        xcase DRAWCMD_PBUFSETUPCOPYTEXTURE:
            pbufSetCopyTextureDirect(data);

        // Shadows
        xcase DRAWCMD_SHADOWFINISHFRAME:
            shadowFinishSceneDirect();
        xcase DRAWCMD_STENCILSHADOW:
            modelDrawShadowVolumeDirect(data);
        xcase DRAWCMD_SPLAT:
            modelDrawShadowObjectDirect(data);
        xcase DRAWCMD_STENCIL:
            rdrSetStencilDirect(data);

        // viewport (generic, specific ports may have their own custom)
        xcase DRAWCMD_VIEWPORT_PRE:
            rt_ViewportGenericPreCallbackDirect(data);
        xcase DRAWCMD_VIEWPORT_POST:
            rt_ViewportGenericPostCallbackDirect(data);

        // ShadowMaps
        xcase DRAWCMD_INITSHADOWMAPMENU:
            rt_initShadowMapMenu();
        xcase DRAWCMD_SHADOWMAP_RENDER:        // Render an entire shadowmap, data block has all the model instances
            rt_shadowmap_render( data );
        xcase DRAWCMD_SHADOWVIEWPORT_PRECALLBACK:
            rt_shadowViewportPreCallback(*(char*)data);
        xcase DRAWCMD_SHADOWVIEWPORT_POSTCALLBACK:
            rt_shadowViewportPostCallback(*(char*)data);
        xcase DRAWCMD_REFRESHSHADOWMAPS:
            rt_shadowmap_init(data);
        xcase DRAWCMD_SHADOWMAP_BEGIN_USING:
            rt_shadowmap_begin_using();
        xcase DRAWCMD_SHADOWMAP_FINISH_USING:
            rt_shadowmap_end_using();
        xcase DRAWCMD_SHADOWMAPDEBUG_2D:
            rt_rdrShadowMapDebug2D();
        xcase DRAWCMD_SHADOWMAPDEBUG_3D:
            rt_rdrShadowMapDebug3D();
        xcase DRAWCMD_SHADOWMAP_SCENE_SPECS:
            rt_shadowmap_scene_customizations(data);
        
        // Cubemaps
        xcase DRAWCMD_INITCUBEMAPMENU:
            rt_initCubeMapMenu();
        xcase DRAWCMD_CUBEMAP_PRECALLBACK:
            rt_cubemapViewportPreCallback(data);    
        xcase DRAWCMD_CUBEMAP_POSTCALLBACK:
            rt_cubemapViewportPostCallback(data);
        xcase DRAWCMD_CUBEMAP_SETINVCAMMATRIX:
            rt_cubemapSetInvCamMatrixDirect();
        xcase DRAWCMD_CUBEMAP_SETSET3FACEMATRIX:
            rt_cubemapSet3FaceMatrixDirect();

        // Clip Planes
        xcase DRAWCMD_CLIPPLANE:
            rdrSetClipPlanesDirect(data);

        // Particles
        xcase DRAWCMD_PARTICLESETUP:
            rdrPrepareToRenderParticleSystemsDirect(data);
        xcase DRAWCMD_PARTICLES:
            modelDrawParticleSysDirect(data);
        xcase DRAWCMD_PARTICLEFINISH:
            rdrCleanUpAfterRenderingParticleSystemsDirect();

        // gamey
        xcase DRAWCMD_INIT:
            rdrInitOnceDirect();
        xcase DRAWCMD_INITTOPOFFRAME:
            rdrInitTopOfFrameDirect(data);
        xcase DRAWCMD_INITTOPOFVIEW:
            rdrInitTopOfViewDirect(data);
        xcase DRAWCMD_RELOADSHADER:
            reloadShaderCallbackDirect(data);
        xcase DRAWCMD_REBUILDCACHEDSHADERS:
            rebuildCachedShadersCallbackDirect(data);
        xcase DRAWCMD_SHADERDEBUGCONTROL:        // commands for the shader debug facility
            shaderMgr_ShaderDebugControl(data);
        xcase DRAWCMD_SETCGSHADERMODE:
            rt_cgSetCgShaderMode(*(unsigned int*)data);
            if(rdr_caps.filled_in) {
                reloadShaderCallbackDirect(NULL);
            }
        xcase DRAWCMD_SETNORMALMAPTESTMODE :
            if ( rdr_caps.filled_in ) {
                bool bWasActive = ( game_state.normalmap_test_mode > 0 );
                bool bNowActive = ( *(unsigned int*)data != 0 );
                game_state.normalmap_test_mode = *(unsigned int*)data;
                if ( bWasActive != bNowActive ) { 
                    reloadShaderCallbackDirect(NULL);
                }
            }
        xcase DRAWCMD_SETDXT5NMNORMALS:
            if ( rdr_caps.filled_in ) {
                reloadShaderCallbackDirect(NULL);
            }
        xcase DRAWCMD_SETCGPATH:
            shaderMgr_SetCgShaderPath(data);
        xcase DRAWCMD_CLOTH:
            modelDrawClothObjectDirect(data);
        xcase DRAWCMD_CLOTHFREE:
            freeClothNodeDirect(*((ClothObject **)data));
        xcase DRAWCMD_SETANISO:
            texSetAnisotropicDirect(*(int*)data);
        xcase DRAWCMD_RESETANISO:
            texResetAnisotropicDirect(*(int*)data, false);
        xcase DRAWCMD_RESETANISO_CUBEMAP:
            texResetAnisotropicDirect(*(int*)data, true);
        xcase DRAWCMD_WRITEDEPTHONLY:
        {
            int write_color = !(*((int *)data));
            rdr_view_state.write_depth_only = !write_color;
            glColorMask(write_color, write_color, write_color, write_color); CHECKGL;
        }

        // Water
        xcase DRAWCMD_WATERSETPARAMS:
            rdrWaterSetParamsDirect(data);
        xcase DRAWCMD_FRAMEGRAB:
            rdrFrameGrabDirect(data);
        xcase DRAWCMD_SETREFLECTIONPLANE:
            rdrSetReflectionPlaneDirect(data);

        // Ambient
        xcase DRAWCMD_SSAOSETPARAMS:
            rdrAmbientSetParamsDirect(data);
        xcase DRAWCMD_SSAORENDER:
            rdrAmbientRenderDirect();
        xcase DRAWCMD_SSAORENDERDEBUG:
            rdrAmbientRenderDebugDirect();
        xcase DRAWCMD_SSAODEBUGINPUT:
            tuneInput(data);

        // Effects
        xcase DRAWCMD_POSTPROCESSING:
            rdrPostprocessingDirect(*((PBuffer **)data));
        xcase DRAWCMD_HDRDEBUG:
            rdrHDRThumbnailDebugDirect();
        xcase DRAWCMD_SUNFLAREUPDATE:
            rdrSunFlareUpdateDirect(data);
        xcase DRAWCMD_RENDERSCALED:
            rdrRenderScaledDirect(*((PBuffer **)data));
        xcase DRAWCMD_ADDTEXTURES:
            rdrAddTexturesDirect(data);
        xcase DRAWCMD_MULTEXTURES:
            rdrMulTexturesDirect(data);
        xcase DRAWCMD_ADDITIVE_ALPHA_WRITE_MASK:
            rdrSetAdditiveAlphaWriteMaskDirect(*((int*)data));
        xcase DRAWCMD_SETUP_STIPPLE:
            rdrSetupStippleStencilDirect();

        // Debug
        xcase DRAWCMD_FRAMEGRAB_SHOW:
            rdrFrameGrabShowDirect(data);
        xcase DRAWCMD_DEBUGCOMMANDS:
            rdrDebugCommandsDirect(data);
        xcase DRAWCMD_FRAMESTART:
            rdrStatsFrameStart();
        xcase DRAWCMD_RESETSTATS:
            rdrStatsClear(*((int *)data));
        xcase DRAWCMD_GETSTATS:
            rdrStatsGet(*((RTStats **)data));

        // Marker
        xcase DRAWCMD_BEGINMARKERFRAME:
            rdrBeginMarkerThreadFrame(*((bool*)data));
        xcase DRAWCMD_BEGINMARKER:
            rdrBeginMarker((char*)data);
        xcase DRAWCMD_ENDMARKER:
            rdrEndMarker();
        xcase DRAWCMD_SETMARKER:
            rdrSetMarker((char*)data);

        // win init
        xcase DRAWCMD_SETGAMMA:
            rdrSetGammaDirect(*((float *)data));
        xcase DRAWCMD_RESTOREGAMMA:
            rdrRestoreGammaRampDirect();
        xcase DRAWCMD_NEEDGAMMARESET:
            rdrNeedGammaResetDirect();
        xcase DRAWCMD_DESTROYDISPLAYCONTEXTS:
            windowDestroyDisplayContextsDirect();
        xcase DRAWCMD_RELEASEDISPLAYCONTEXTS:
            windowReleaseDisplayContextsDirect();
        xcase DRAWCMD_REINITDISPLAYCONTEXTS:
            windowReInitDisplayContextsDirect();
        xcase DRAWCMD_SETVSYNC:
            rdrSetVSyncDirect(*((int *)data));

        xdefault:
            assert(0);
    }
    INEXACT_CHECKGL;
    WCW_CheckGLState();
}

// @todo Bumping these up for the moment to support new features
// but we will have to revisit what is an appropriate setting
#define CMD_QUEUE_SIZE    (1<<20)
#define MSG_QUEUE_SIZE    (1<<6)

static void cmdDispatchCallback(void *unused, int type, void *data)
{
    cmdDispatch(unused, (DrawType)type, data);
}

void renderThreadSetThreading(int run_thread)
{
    g_run_render_thread = run_thread;
    if (!render_thread)
        render_thread = wtCreate(cmdDispatchCallback, CMD_QUEUE_SIZE, winErrorDispatch, winRenderStall, MSG_QUEUE_SIZE, 0);
    wtSetThreaded(render_thread, run_thread);
}

void renderThreadStart(void)
{
    if (!render_thread)
    {
        render_thread = wtCreate(cmdDispatchCallback, CMD_QUEUE_SIZE, winErrorDispatch, winRenderStall, MSG_QUEUE_SIZE, 0);
        wtSetThreaded(render_thread, g_run_render_thread);
    }

    wtStart(render_thread);

    rdrQueueCmd(DRAWCMD_RDRSETUP);

    // Flush here to block until setup is complete because the rdr_caps
    // need to be accessed on the main thread and we need to make sure they are correct
    rdrQueueFlush();
}


void rdrAddTextures(RdrAddTexturesParams *params)
{
    rdrQueue(DRAWCMD_ADDTEXTURES, params, sizeof(*params));
}

void rdrMulTextures(RdrAddTexturesParams *params)
{
    rdrQueue(DRAWCMD_MULTEXTURES, params, sizeof(*params));
}

int rdrAllocTexHandle()
{
    CHECKNOTGLTHREAD;
    while(!tex_handle_count)
    {
        rdrQueueCmd(DRAWCMD_GENTEXHANDLES);
        rdrQueueFlush();
    }
    return tex_handles[--tex_handle_count];
}

int rdrAllocBufferHandle()
{
    CHECKNOTGLTHREAD;
    while(!buffer_handle_count)
    {
        rdrQueueCmd(DRAWCMD_GENBUFFERHANDLES);
        rdrQueueFlush();
    }
    return buffer_handles[--buffer_handle_count];
}

void rdrSetWriteDepthOnly(int write_depth_only)
{
    rdrQueue(DRAWCMD_WRITEDEPTHONLY, &write_depth_only, sizeof(int));
}

