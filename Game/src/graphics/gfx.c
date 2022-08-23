#include "graphics/gfx.h"
#include "graphics/gfxSettings.h"
#include <utilitieslib/utils/mathutil.h>
#include "seq/gfxtree.h"
#include "graphics/font.h"
#include "UI/uiConsole.h"
#include "cmdparse/cmdcommon.h"
#include "graphics/sun.h"
#include "graphics/gfxwindow.h"
#include "render/renderstats.h"
#include "cmdparse/cmdgame.h"
#include "graphics/camera.h"
#include "edit/edit.h"
#include "entity/entity.h"
#include "entity/entclient.h"
#include "entity/entrecv.h"
#include "player/player.h"
#include "win/win_init.h"
#include "graphics/FX/fx.h"
#include "render/render.h"
#include <utilitieslib/utils/error.h>
#include "graphics/light.h"
#include "edit/edit_drawlines.h"
#include "graphics/groupdraw.h"
#include <utilitieslib/utils/utils.h>
#include "entity/entDebug.h"
#include "edit/edit_cmd.h"
#include "UI/uiReticle.h"
#include "assert.h"
#include "gameComm/initClient.h"
#include "gameComm/dooranimclient.h"
#include "graphics/seqgraphics.h"
#include "UI/uiGender.h"
#include "render/sprite.h"
#include "entity/motion.h"
#include "render/texWords.h"
#include "render/renderprim.h"
#include "render/tex.h"
#include "seq/anim.h"
#include "graphics/textureatlas.h"
#include "render/renderWater.h"
#include "render/renderssao.h"
#include "render/pbuffer.h"
#include "render/renderEffects.h"
#include "graphics/gfxLoadScreens.h"
#include "graphics/gfxDebug.h"
#include "graphics/zOcclusion.h"
#include "graphics/vistray.h"
#include "demo.h"
#include <utilitieslib/utils/timing.h>
#include "entity/PowerInfo.h"
#include "fxinfo.h"
#include "graphics/FX/fxdebris.h"
#include "graphics/groupMiniTrackers.h"
#include "render/texUnload.h"
#include <utilitieslib/utils/osdependent.h>
#include "graphics/viewport.h"
#include "graphics/groupdrawutil.h"
#include "render/rendershadowmap.h"
#include "graphics/failtext.h"
#include "render/perfcounter.h"
#include "render/cubemap.h"
#include "graphics/view_cache.h"
#include "bases/baseedit.h"
#include "UI/uiGame.h"
#include "player/pmotion.h"
#include "graphics/gfxLoadScreens.h"
#include "graphics/gfxDevHUD.h"
#include "UI/sprite/sprite_base.h"

GfxState gfx_state;

static ViewportInfo s_mainviewport;    // this gets referenced by the gfx_state for use by shadowmap viewport (for example)

static int sMainViewportZOcclusion = 0;
static int sIsLetterBoxThisFrame;

extern int glob_have_camera_pos;

void gfxAutoSetPerformanceSettings(void);

//WOuld be static, but edit_select cals it for some reason
void gfxSetViewMat(Mat4 cammat,Mat4 viewmat,Mat4 inv_viewmat)
{
    Vec3    dv;

    copyMat4(cammat,viewmat);
    viewmat[3][1] += 0;
    transposeMat3(viewmat);
    rdrFixMat(viewmat);
    subVec3(zerovec3,viewmat[3],dv);
    mulVecMat3(dv,viewmat,viewmat[3]);

    if (inv_viewmat)
        invertMat4Copy(viewmat,inv_viewmat);
}

static void gfxExtractViewFrustums()
{
    Vec3 frustum_corners_vs[4];
    F32 zfar = -gfx_window.zfar;
    F32 znear = -gfx_window.znear;
    F32 scale = zfar / znear;
    F32 aspect = gfx_window.fovx / gfx_window.fovy;
    F32 top = znear * ftan(RAD(gfx_window.fovy * 0.5f));
    F32 bottom = -top;
    F32 left = bottom * aspect;
    F32 right = top * aspect;
    Vec3 origin;

    /*
    // For testing, this will pull in the clip planes a bit
    top *= 0.9f;
    bottom *= 0.9f;
    left *= 0.9f;
    right *= 0.9f;
    */

    zeroVec3(origin);
    setVec3(frustum_corners_vs[0], left  * scale, bottom * scale, -zfar);
    setVec3(frustum_corners_vs[1], right * scale, bottom * scale, -zfar);
    setVec3(frustum_corners_vs[2], right * scale, top    * scale, -zfar);
    setVec3(frustum_corners_vs[3], left  * scale, top    * scale, -zfar);

    setVec4(cam_info.view_frustum[FRUST_NEAR], 0, 0, -1, znear);
    setVec4(cam_info.view_frustum[FRUST_FAR], 0, 0, 1, -zfar);

    makePlane(origin, frustum_corners_vs[0], frustum_corners_vs[3], cam_info.view_frustum[FRUST_LEFT]);
    makePlane(origin, frustum_corners_vs[2], frustum_corners_vs[1], cam_info.view_frustum[FRUST_RIGHT]);
    makePlane(origin, frustum_corners_vs[1], frustum_corners_vs[0], cam_info.view_frustum[FRUST_BOTTOM]);
    makePlane(origin, frustum_corners_vs[3], frustum_corners_vs[2], cam_info.view_frustum[FRUST_TOP]);
}

// Loads all textures that can be seen from the current player location
void gfxLoadRelevantTextures()
{
    Vec3 pyr;
    Mat4 cam_save;
    extern int show_console;
    int show_console_save = show_console;
    F32 fov_save = game_state.fov;
    int no_draw = 0; // Have to "draw" to get textures loaded
    Vec3 pos;
    U32 sleeptime = 0; //1000;

    //printf("begin gfxLoadRelevantTextures()\n");
    if (demoIsPlaying()) {
        copyVec3(cam_info.cammat[3], pos);
    } else {
        if (!playerPtr()) {
            return;
        }
        copyVec3(ENTPOS(playerPtr()), pos);
    }

    show_console = 0;
    copyMat4(cam_info.cammat, cam_save);
    game_state.fov = 90;

    copyVec3(pos,cam_info.mat[3]);
    copyVec3(pos,cam_info.cammat[3]);
    cam_info.cammat[3][1]+=5; // Otherwise it'd be at the player's feet, it seems

    sunUpdate(&g_sun, 0); // this must be after the camera matrix is set, or else fog will be wrong

    if(sleeptime)
    {
        showBgAdd(-1);
        game_state.game_mode = SHOW_GAME;
        windowUpdate();
    }
    
    zeroVec3(pyr);
    createMat3RYP(cam_info.cammat,pyr);
    if(sleeptime)
    {
        printf(    "step1: (%1.f, %1.f, %1.f) facing (%1.1f, %1.1f, %1.1f) ------------------------------------\n",
                vecParamsXYZ(cam_info.cammat[3]),
                vecParamsXYZ(cam_info.cammat[2]));
    }
    showBgAdd(2);
    gfxUpdateFrame(1, 0, no_draw);
    showBgAdd(-2);

    if(sleeptime)
    {
        windowUpdate();
        {U32 t=timeGetTime();while(timeGetTime()-t<sleeptime);}
    }
    else
    {
        loadUpdate("gfxLoadRelevantTextures", 1000000);
    }

    pyr[1] += RAD(90);
    createMat3RYP(cam_info.cammat,pyr);
    if(sleeptime)
    {
        printf(    "step2: (%1.f, %1.f, %1.f) facing (%1.1f, %1.1f, %1.1f) ------------------------------------\n",
                vecParamsXYZ(cam_info.cammat[3]),
                vecParamsXYZ(cam_info.cammat[2]));
    }
    showBgAdd(2);
    gfxUpdateFrame(1, 0, no_draw);
    showBgAdd(-2);

    if(sleeptime)
    {
        windowUpdate();
        {U32 t=timeGetTime();while(timeGetTime()-t<sleeptime);}
    }
    else
    {
        loadUpdate("gfxLoadRelevantTextures", 1000000);
    }

    pyr[1] += RAD(90);
    createMat3RYP(cam_info.cammat,pyr);
    if(sleeptime)
    {
        printf(    "step3: (%1.f, %1.f, %1.f) facing (%1.1f, %1.1f, %1.1f) ------------------------------------\n",
                vecParamsXYZ(cam_info.cammat[3]),
                vecParamsXYZ(cam_info.cammat[2]));
    }
    showBgAdd(2);
    gfxUpdateFrame(1, 0, no_draw);
    showBgAdd(-2);

    if(sleeptime)
    {
        windowUpdate();
        {U32 t=timeGetTime();while(timeGetTime()-t<sleeptime);}
    }
    else
    {
        loadUpdate("gfxLoadRelevantTextures", 1000000);
    }

    pyr[1] += RAD(90);
    createMat3RYP(cam_info.cammat,pyr);
    if(sleeptime)
    {
        printf(    "step4: (%1.f, %1.f, %1.f) facing (%1.1f, %1.1f, %1.1f) ------------------------------------\n",
                vecParamsXYZ(cam_info.cammat[3]),
                vecParamsXYZ(cam_info.cammat[2]));
    }
    showBgAdd(2);
    gfxUpdateFrame(1, 0, no_draw);
    showBgAdd(-2);

    if(sleeptime)
    {
        windowUpdate();
        {U32 t=timeGetTime();while(timeGetTime()-t<sleeptime);}

        printf("done:-----------------------------------------------------------\n");
    }
    else
    {
        loadUpdate("gfxLoadRelevantTextures", 1000000);
    }

    show_console = show_console_save;
    copyMat4(cam_save, cam_info.cammat);

    game_state.fov = fov_save;

    //printf("Done with gfxLoadRelevantTextures\n");
}


//##########################################################################
//Timer stuff
void    waitFps(int fps)
{
    static int frame_timer = -1;
    F32        elapsed,frame_time,extra;

    //if (!fps)
    //    return;
    if (!fps)
    {
        Sleep(0);
        return;
    }
    frame_time = 1.f / fps;
    if (frame_timer < 0)
    {
        frame_timer = timerAlloc();
        timerStart(frame_timer);
    }
    elapsed = timerElapsed(frame_timer);
    extra = frame_time - elapsed;
    if (extra > 0)
        Sleep(extra * 1000);
    timerStart(frame_timer);
}


/*calculates TIMESTEP here, too */
void showFrameRate()
{
    U32        curr,delta;
    F32        time;
    static    U32 last_ticks;
    static    U32 frame_count;
    static    U32 last_fps_ticks;
    static  F32 largest_time_step = 0;
    static    F32 smallest_time_step = 100;
    static  F32 sum_time_step = 0;
    static  F32 sum_frames = 0;
    static  F32    longtime;
    static  F32 shorttime;
    static  F32 avgtime;
    static    int blip = 0;
    int        line = 1;

    //Calculate the TIMESTEP - length of last frame in 30ths of a sec
    curr = timerCpuTicks();
    delta = curr - last_ticks;
    if (delta == 0)
        delta = 1;    // MAK - we can get 0 if we just started the game
    if(game_state.frame_delay)
    {
        Sleep(min(game_state.frame_delay, 1000));
        last_ticks = timerCpuTicks();
    }
    else
    {
        last_ticks = curr;
    }
    time = (F32)delta / (F32)timerCpuSpeed();

    if(server_visible_state.timestepscale <= 0)
        server_visible_state.timestepscale = 1;

    TIMESTEP_NOSCALE = 30.f * time;
    TIMESTEP = TIMESTEP_NOSCALE;
    
    global_state.cur_timeGetTime = timeGetTime();
    
    //Check for large and small timesteps.
    if(largest_time_step < TIMESTEP)
        largest_time_step = TIMESTEP;
    if(smallest_time_step > TIMESTEP)
        smallest_time_step = TIMESTEP;
    sum_time_step += TIMESTEP;
    sum_frames++;

    if(isDevelopmentMode()){
        TIMESTEP *= server_visible_state.timestepscale;
    }

    //Set Maximum Timestep
    if (TIMESTEP > 30)
        TIMESTEP = 30;

    if (game_state.spin360)
    {
        TIMESTEP = 0.00001;
        global_state.frame_time_30_real = TIMESTEP;
    }
    // Calc FPS over a few frames
    delta = curr - last_fps_ticks;
    time = (F32)delta / (F32)timerCpuSpeed();
    if (time > game_state.showfps)
    {
        game_state.fps = frame_count / time;
        frame_count = 0;
        last_fps_ticks = last_ticks;
        if (game_state.cryptic && game_state.showfps>1) {
            char        titleBuffer[1000];
            sprintf(titleBuffer, "City of Heroes : PID: %d FPS: %1.2f", _getpid(), game_state.fps);
            winSetText(titleBuffer);
        }
    }
    frame_count++;

    if(sum_frames == 30)
    {
        longtime = largest_time_step;
        shorttime = smallest_time_step;
        avgtime = sum_time_step / sum_frames;
        sum_time_step = 0;
        sum_frames = 0;
        largest_time_step = 0;
        smallest_time_step = 100;
    }

 
#if defined(_DEBUG) || defined(_OPTDEBUG)
    // TODO: Add override check for character select.
    if (!game_state.e3screenshot && !isDevelopmentMode() && (current_menu() == MENU_LOGIN || game_state.showfps) && !doingHeadShot())
    {
        int r, g, b;
        r = g = b = 255;

        if (blip % 40 >= 36)
            g = b = 0;

        xyprintfcolor(30 + TEXT_JUSTIFY,line++,r,g,b,"Training Room Diagnostics enabled.");
        xyprintfcolor(10 + TEXT_JUSTIFY,line++,r,g,b,"Your framerate will be slower with this build than when it goes live.");
        blip++;
    }
#endif
    if (!game_state.e3screenshot && game_state.showActiveVolume && !doingHeadShot())
    {
        xyprintf(38 + TEXT_JUSTIFY,line++,"active volume: %s", g_activeMaterialTrackerName);
    }
    if (!game_state.e3screenshot && game_state.fps && game_state.showfps && !doingHeadShot())
    {
           xyprintf(38 + TEXT_JUSTIFY,line++,"% 2.2f (%0.1fm/fm)", game_state.fps, 1000.f/game_state.fps);
    }
     if (!game_state.e3screenshot && game_state.showfps && !doingHeadShot())
    {
        int pps = texWordGetPixelsPerSecond();
        if (pps) {
            xyprintf(35 + TEXT_JUSTIFY,line++,"% 9d TexWordsPPS", pps);
        }
    }
     if (!game_state.e3screenshot && game_state.showtime && !doingHeadShot())
    {
        xyprintf(35 + TEXT_JUSTIFY,line++,"time % 2.2f     timescale % 2.2f", server_visible_state.time, server_visible_state.timescale);
    }
    if(game_state.info)
    {
        line += 2;
        if( longtime > 6 )
            xyprintf(45 + TEXT_JUSTIFY,line,"## < 5 ##");
        xyprintf(55 + TEXT_JUSTIFY,line++,"Slow: % 2.2f", 30 / longtime);    
        xyprintf(55 + TEXT_JUSTIFY,line++,"Avg:  % 2.2f", 30 / avgtime);
        xyprintf(55 + TEXT_JUSTIFY,line++,"Fast: % 2.2f", 30 / shorttime);
    }
}

//###########################################################################################
// Basic graphics functions

void gfxReload(int unloadAllModels)
{
    PERFINFO_AUTO_START("gfxReload", 1);
        ErrorfResetCounts();
        fxReInit();
        
        #if NOVODEX
            fxDeinitDebrisManager();
            fxInitDebrisManager();
        #endif
        
        entReset();
        playerSetEnt(0);
        gfxTreeInit();
        PERFINFO_AUTO_START("groupReset", 1);
            groupReset(); // We're freeing all the models below, free the world groups that reference them first!
        PERFINFO_AUTO_STOP_CHECKED("groupReset");
        modelFreeAllCache(unloadAllModels?(GEO_USED_BY_GFXTREE|GEO_USED_BY_WORLD|GEO_INIT_FOR_DRAWING|GEO_DONT_INIT_FOR_DRAWING):GEO_USED_BY_GFXTREE);
        fxPreloadGeometry();
        rdrInit();
        loadBG();
        // Load a scene so we get some default ambient values for character creation, etc.
        sunSetSkyFadeClient(0, 1, 0.0);
        sceneLoad("scenes/default_scene.txt");
    PERFINFO_AUTO_STOP();
}


void gfxDoNothingFrame()
{
    gfxTreeFrameInit();
    fxRunEngine(); //TO DO why does this crash if not updated while minimized?
    #if NOVODEX
        fxUpdateDebrisManager();
    #endif
    partRunEngine();
    init_display_list();
    queuedLinesReset();
    rdrQueueFlush();
    Sleep(1);
}

void gfxResetTextures()
{
    atlasFreeAll();
    texFreeAll();
    texMakeWhite();
    texLoad("font.tga",TEX_LOAD_NOW_CALLED_FROM_MAIN_THREAD, TEX_FOR_UTIL);
    gfxLoadRelevantTextures();
}


static displayLetterBox( void )
{  
    static F32 time = 0;
    int w, h;
    int x1, y1, x2, y2, argb = 0;  
    F32 timeToClose = 15;
    F32 percent_covered = 0.15;
    F32 percent_complete;

    if( !game_state.viewCutScene && !game_state.forceCutSceneLetterbox )
        time -= TIMESTEP;
    else 
        time += TIMESTEP;
    time = CLAMP( time, 0.0, timeToClose );

    if( time > 0 )
    {
        percent_complete = CLAMP( time/timeToClose, 0.0, 1.0 );

        x1 = 0;        
        x2 = 3000;
        windowSize(&w,&h);

        rdrSetup2DRendering();

        //TOP BOX  
        y1 = h - (h*percent_covered) * percent_complete;
        y2 = h;
        drawFilledBox(x1, y1, x2, y2, 0xff000000); 

        //BOTTOM BOX
        y1 = 0; 
        y2 = (h*percent_covered) * percent_complete;
        drawFilledBox(x1, y1, x2, y2, 0xff000000);

        rdrFinish2DRendering();
    }
}

static void gfxClearBackgroundToBlack(void)
{
    Vec3 blackBG = {0, 0, 0};
    rdrSetBgColor(blackBG);
    rdrClearScreen();
}

int glob_force_buffer_flip=0; // For debugging headshot stuff

// These being set up as 4 variables is really just for debugging, it could just be a comparison in the middle of the gfxUpdateFrame
int sprite_firstpass=0;
int sprite_firstpass_clip=0;
int sprite_secondpass=1;
int sprite_secondpass_clip=0;

PBuffer pbRenderTexture={0};

extern float zoBBBias;

static bool callback_ViewCacheVisibility(ViewCacheEntry *entry)
{
    GroupDef *def = entry->groupEnt.def;
    int clip;

    /*
    // Edit mode stuff
    DefTracker *tracker = &entry->tracker;

    if (tracker->def == NULL)
        tracker = NULL;

    if (tracker)
    {
        if (tracker->tricks && (tracker->tricks->flags1 & TRICK_HIDE))
            return false;
        if (tracker->invisible)
            return false;
    }
    */

    // sphereInFrustum
    /*
    // This appears to be stencil shadow specific check
    if (entry->groupEnt.flags_cache & HAS_SHADOW)
    {
        if (!gfxSphereVisible(entry->mid, def->radius) && !shadowVolumeVisible(entry->mid, def->radius, def->shadow_dist))
            clip = 0;
        else
            clip = CLIP_NONE;
    }
    else
    */
    {
        //clip = gfxSphereVisible(entry->mid, entry->groupEnt.radius);
        //clip = gfxSphereVisible(entry->mid, def->radius);
        clip = gfxSphereVisible(entry->mid, entry->radius);
    }

    if (!clip)
        return false;
    
    if (!def->shadow_dist)
    {
        if (group_draw.zocclusion && (def->model || def->count>1 || def->welded_models))
        {
            int nearClipped;
            Vec3 min, max, eye_bounds[8];
            copyVec3(def->min, min);
            copyVec3(def->max, max);
            min[0] -= zoBBBias;
            min[1] -= zoBBBias;
            min[2] -= zoBBBias;
            max[0] += zoBBBias;
            max[1] += zoBBBias;
            max[2] += zoBBBias;

            mulBounds(min, max, entry->parent_mat, eye_bounds);

            nearClipped = isBoxNearClipped(eye_bounds);
            if (nearClipped == 2)
                return false;
            else if (!nearClipped && !zoTest2(eye_bounds, nearClipped, entry->uid, entry->groupEnt.recursive_count_cache))
                return false;
        }
        else if (clip != CLIP_NONE && !checkBoxFrustum(clip, def->min, def->max, entry->parent_mat))
            return false;
    }

    return true;
}


// Called once
static void gfxSetupStuffToDraw(ViewportInfo *viewport)//int force_render_world, int headShot, int no_drawing)
{
    // Init stuff
    extern int splatShadowsDrawn; 
    static bool last_render_to_pbuffers=false;

    gfxTreeFrameInit();
    splatShadowsDrawn = 0; //not debug, used to fix z-fighting for splats

    if (gfx_state.doStencilShadows)
    {
        shadowStartScene();

        if (last_render_to_pbuffers != gfx_state.renderToPBuffer) {
            last_render_to_pbuffers = gfx_state.renderToPBuffer;
            rdrStippleStencilNeedsRebuild();
        }
    }

    vistrayClearVisibleList();
    group_draw.see_outside = 1; //since see outside is used in multiple places now

    // Keep water from flickering to black when doing headshots
    if (!viewport->doHeadshot)
    {
        gfx_state.waterThisFrame = 0;
    }

    // Set sprite rendering mode
    if (game_state.game_mode == SHOW_TEMPLATE && !green_blobby_fx_id) {
        sprite_firstpass = 1;
        sprite_firstpass_clip = 1;
        sprite_secondpass = 1;
        sprite_secondpass_clip = 1;
    } else {
        // Either Green blobby is running or we're in the game
        //  draw sprites last
        sprite_firstpass = 0;
        sprite_firstpass_clip = 0;
        sprite_secondpass = 1;
        sprite_secondpass_clip = 0;
    }

    if(game_state.game_mode == SHOW_TEMPLATE || game_state.game_mode == SHOW_GAME)
    {
        bool runUpdates = ViewCache_Update(viewport->renderPass);
        if (runUpdates)
        {
            // Traverse the map scene graph, etc
            PERFINFO_AUTO_START("Traverse World",1);
            if( viewport->renderEnvironment && !viewport->doHeadshot && !GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_GROUPDEF_TRAVERSAL)) 
                groupDrawRefs(cam_info.viewmat, cam_info.inv_viewmat);
            PERFINFO_AUTO_STOP();
        }

        ViewCache_Use(callback_ViewCacheVisibility, viewport->renderPass);

        vistraySortVisibleList();

        PERFINFO_AUTO_START("entClientProcessVisibility",1);
        if (!viewport->doHeadshot && viewport->renderCharacters)
            entClientProcessVisibility();
        PERFINFO_AUTO_STOP_CHECKED("entClientProcessVisibility");
    } 
    else if(playerPtr() && playerPtr()->powerInfo && !viewport->doHeadshot)
    { //make sure we update the power timers from game menus
        powerInfo_UpdateTimers(playerPtr()->powerInfo, TIMESTEP / 30);
    }

    PERFINFO_AUTO_START("fxRunEngine",1);
    // must be after gfxSetViewMat and entClientProcess
    if ((gfx_state.mainViewport || (viewport->renderPass == RENDERPASS_IMAGECAPTURE)) && TIMESTEP > 0.000001f)
    {
        fxRunEngine(); //maybe move this out, but so much fx stuff is camera relative...hmm
    }
    PERFINFO_AUTO_STOP_CHECKED("fxRunEngine");

    PERFINFO_AUTO_START("fxUpdateDebris",1);
    // must be after gfxSetViewMat and entClientProcess
    if (gfx_state.mainViewport && TIMESTEP > 0.000001f)
    {
        #if NOVODEX
            fxUpdateDebrisManager();
        #endif
    }
    PERFINFO_AUTO_STOP();

    if(game_state.game_mode == SHOW_TEMPLATE || game_state.game_mode == SHOW_GAME)
    {
        if (game_state.debug_collision) {
            //PERFINFO_AUTO_STOP_START("entMotionDrawDebugCollisionTrackers",1);
            entMotionDrawDebugCollisionTrackers();
        }

        PERFINFO_AUTO_START("Traverse Gfxtree",1);
        if( viewport->renderCharacters && !GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_GFXNODE_TREE_TRAVERSAL))
        {
            gfxTreeDrawNode(gfx_tree_root,cam_info.viewmat);
        }
        PERFINFO_AUTO_STOP();
    }

#ifndef FINAL // debug drawing code?
    PERFINFO_AUTO_START("displayDebugInterface3D",1);
    gfxShowDebugIndicators2();
    displayDebugInterface3D();
    PERFINFO_AUTO_STOP();
#endif

    gfxTreeFrameFinishQueueing();

#ifndef FINAL // debug drawing code?
    {
        extern Line    lightlines[1000];
        extern int    lightline_count;
        int i;
        
        for(i = 0; i < lightline_count; i++){
            drawLine3DWidth(lightlines[i].a, lightlines[i].b, *(int*)lightlines[i].color, lightlines[i].width);
        }
        
        lightline_count = 0;
    }
#endif
}


static void setupPBuffers(ViewportInfo *viewport)
{
    int w, h;
    PBufferFlags flags;
    bool bUseFloatBuffers = !!(rdr_caps.features&GFXF_FPRENDER);
    int num_aux_buffers=0;
    int stencil_bits = 0;

    windowSize(&w, &h);

    if (gfx_state.renderscale)
    {
        if (game_state.useRenderScale==RENDERSCALE_FIXED)
        {
            if (game_state.renderScaleX>=16 && game_state.renderScaleX<w*4)
                w = game_state.renderScaleX;
            if (game_state.renderScaleY>=16 && game_state.renderScaleY<h*4)
                h = game_state.renderScaleY;
        } else {
            if (game_state.renderScaleX<2 && game_state.renderScaleX>0)
                w *= game_state.renderScaleX;
            if (game_state.renderScaleY<2 && game_state.renderScaleY>0)
                h *= game_state.renderScaleY;
        }
        if (w<16)
            w = 16;
        if (h<16)
            h = 16;
    }
    if (game_state.pbuftest) {
        pbufDelete(&pbRenderTexture);
    }

    if (game_state.antialiasing<1)
        game_state.antialiasing = 1;

    flags = PB_RENDER_TEXTURE|PB_COLOR_TEXTURE|PB_ALPHA|
        (bUseFloatBuffers?PB_FLOAT:0)|
        (rdr_caps.supports_fbos?PB_FBO:0)|    // Always use FBOs if available
        (((rdr_caps.features&GFXF_DOF)||(game_state.waterMode > WATER_LOW))?PB_DEPTH_TEXTURE:0);

    if (game_state.useMRTs) {
        flags &=~PB_DEPTH_TEXTURE;
        num_aux_buffers = 1;
    }

    if (!rdr_caps.use_pbuffers)
        flags |= PB_ALLOW_FAKE_PBUFFER;

    if (gfx_state.doStencilShadows)
        stencil_bits = 8;

    if (pbRenderTexture.width != w || pbRenderTexture.height != h || pbRenderTexture.isFloat != bUseFloatBuffers ||
        pbRenderTexture.flags != flags || pbRenderTexture.desired_multisample_level!=game_state.antialiasing ||
        num_aux_buffers > pbRenderTexture.num_aux_buffers ||
        (pbRenderTexture.hasStencil>0) != gfx_state.doStencilShadows)
    {
        // init
        if (pbRenderTexture.isFloat != bUseFloatBuffers)
            reloadShaderCallback(NULL, 1);
        pbufInit(&pbRenderTexture,w,h,flags,game_state.antialiasing,1,stencil_bits,24,num_aux_buffers, 0);
    }
}

void gfxDrawSetupDrawing(ViewportInfo *viewport)
{
    gfx_state.renderToPBuffer = (gfx_state.renderscale || gfx_state.postprocessing) && viewport && !viewport->isRenderToTexture;

    if (viewport && viewport->isRenderToTexture)
    {
        pbufMakeCurrent( viewport_GetPBuffer(viewport) );
    }
    else if (gfx_state.renderToPBuffer) {
        setupPBuffers(viewport);
        pbufMakeCurrent(&pbRenderTexture);
    }

    // Must setup sun before rdrInitTopOfFrame()
    // Set Light    
    if( game_state.game_mode == SHOW_TEMPLATE ) //Override parts of sunUpdate
        setLightForCharacterEditor();    
    mulVecMat3(g_sun.direction, cam_info.viewmat, g_sun.direction_in_viewspace); 

    // Must be before gfxTreeDrawNode
    if(!viewport || !viewport->renderSky)
        sunGlareDisable();
    else
        sunGlareEnable();
    
    // Calculate view-space position of cubemap origin
    mulVecMat4(gfx_state.cubemap_origin, cam_info.viewmat, gfx_state.cubemap_origin_vs);

    rdrInitTopOfView(cam_info.viewmat,cam_info.inv_viewmat);
}


// Called once

void gfxDraw2DStuffPre3D()
{
    rdrBeginMarker(__FUNCTION__);
    // Draw sprites that need to be drawn before Geometry/FX (only in SHOW_TEMPLATE mode)
    if (sprite_firstpass  && !GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_2D)) {
        if (sprite_firstpass_clip) {
            spriteZClip(ZCLIP_INFRONT, game_state.nearz);
        } else {
            spriteZClip(ZCLIP_NONE, 0);
        }

        fontRender(); //render UI and all other sprites  
    }
    rdrEndMarker();
}

void gfxDraw2DStuffPost3D(int show2D, int headShot)
{
    rdrBeginMarker(__FUNCTION__);
    if (!headShot && show2D)
    {
        PERFINFO_AUTO_START("displayLetterBox",1);  
        displayLetterBox();
        PERFINFO_AUTO_STOP_START("drawStuffOnEntities",1);  
        drawStuffOnEntities(); // Needs to be after setting matrices, traversing entities, but before fontRender()
        PERFINFO_AUTO_STOP();
    }

    // render the 'sprite' list which is predominantly the UI display quads
    if (show2D)
    {
        //setup letterbox viewport
        if (sIsLetterBoxThisFrame)
        {
            gfxSetupLetterBoxViewport();
        }

        PERFINFO_AUTO_START("fontRender",1);  
        if (sprite_secondpass && !GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_2D)) {
            if (sprite_secondpass_clip) {
                spriteZClip(ZCLIP_BEHIND, game_state.nearz);
            } else {
                spriteZClip(ZCLIP_NONE, 0);
            }
            fontRender(); //render UI and all other sprites  
        }
        PERFINFO_AUTO_STOP();
    }

    // development and informational displays
    if (!headShot && show2D)
    {
        //Render assorted debug/console info
        gfxDevHudRender();
        PERFINFO_AUTO_START("rdrAmbientDebug", 1);
        rdrAmbientDebug();
        PERFINFO_AUTO_STOP_START("rdrShadowMapDebug", 1);
        rdrShadowMapDebug2D();
        PERFINFO_AUTO_STOP_START("failTextDisplay", 1);
        failTextDisplay();
        PERFINFO_AUTO_STOP_START("displayDebugUI",1);
        displayDebugInterface2D();

        // this renders the direct xyprintf text
        PERFINFO_AUTO_STOP_START("fontRenderEditor", 1);
        fontRenderEditor();

        PERFINFO_AUTO_STOP_START("DoorAnimCheckFade",1);
        DoorAnimCheckFade(); 
        PERFINFO_AUTO_STOP_START("rdrStatsDisplay",1);
        displayEntDebugInfoTextBegin();
        rdrStatsDisplay(); // Needs to be after DoorAnimCheckFade()
        displayEntDebugInfoTextEnd();
        PERFINFO_AUTO_STOP();
    }

    if (!headShot) {
        PERFINFO_AUTO_START("showFramerate",1);
        showFrameRate(); //TO DO this is where timestep is calculated : not a good place for it, but I'm afraid to move it
        PERFINFO_AUTO_STOP();
    }
    rdrEndMarker();
}

void gfxDrawFinishFrame(int doFlip, int headShot, int renderPass)
{
    rdrBeginMarker(__FUNCTION__);
    if (!headShot)
        init_display_list(); // Clear the list of sprites (not done automatically if we're in 2-pass mode)

    if( doFlip )
    {
        if (game_state.framegrab_show)
            rdrFrameGrabShow(game_state.framegrab_show);

        //What to show
        //Flip the buffers, showing the game
        //if (glob_have_camera_pos == PLAYING_GAME)
        //Show the load screen finish, then next time show the game

        if (glob_have_camera_pos == MAP_LOADED_FINISHING_LOADING_BAR)
        {
            gfxClearBackgroundToBlack();
            showBgAdd(2);
            gfxSetupLetterBoxViewport();
            finishLoadScreen();
            if (comicLoadScreenEnabled())
            {
                showBG();
            }
            showBgAdd(-2);
            if (!comicLoadScreenEnabled())
            {
                hideLoadScreen();
            }
        }
        else if ( game_state.game_mode == SHOW_LOAD_SCREEN)
        {
            PERFINFO_AUTO_START("showLoadScreen",1);
                gfxClearBackgroundToBlack();
                showBgAdd(2);
                gfxSetupLetterBoxViewport();
                showBG();
                showBgAdd(-2);
            PERFINFO_AUTO_STOP();                
        }
        else
        {
            //printf("flipping buffer\n");
            if (game_state.framegrab) {
                rdrFrameGrab();
                if (game_state.framegrab==1)
                    game_state.framegrab=0;
            }
            PERFINFO_AUTO_START("windowUpdate",1);
                windowUpdate();
            PERFINFO_AUTO_STOP();                
        }
    } else if (glob_force_buffer_flip) {
        // Debug
        PERFINFO_AUTO_START("windowUpdate",1);
            windowUpdate();
        PERFINFO_AUTO_STOP();                
    }
    //else glob_have_camera_pos == LOADING_MAP which means let loadUpdate take care of showing the load screen

#if 0 // fpe, disable this since it just includes gpu time for frame swap to happen in windowUpdate call above
    {
        // This is mis-matched (multiple resets will happen)
        RTStats* pStatsOther = rdrStatsGetPtr(RTSTAT_OTHER, renderPass);
        rdrQueue(DRAWCMD_GETSTATS, &pStatsOther, sizeof(RenderStats *));
    }
#endif

    fxCleanUp();
    lightCheckReapplyColorSwaps();
    groupDrawMinitrackersFrameCleanup();
    rdrEndMarker();
}


void gfxClearScreenFor3D(ViewportInfo *viewport)
{
    rdrBeginMarker(__FUNCTION__);

    // @todo does this still need the special case code for stippling on main viewport?
    if (gfx_state.mainViewport)
    {
        rdrClearScreen();
    }
    else
    {
        rdrClearScreenEx(viewport->clear_flags); // use explicit viewport settings for clear
    }

    if (gfx_state.doStencilShadows)
        rdrSetupStippleStencil();

    if (viewport->backgroundimage)
    {
        rdrSetup2DRendering();

        display_sprite_immediate_ex(viewport->backgroundimage, NULL, 0.0f, 0.0f, 0.0f,
            viewport->width / viewport->backgroundimage->width,
            viewport->height / viewport->backgroundimage->height,
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0.0f, 0.0f, 1.0f, 1.0f, 0, 0,
            clipperGetCurrent(), 1 );

        rdrFinish2DRendering();
    }
    rdrEndMarker();
}

void drawReflector(void);

void gfxRenderViewport(ViewportInfo *viewport)
{
    bool bDidUnderwaterAlphaPass = false;
    rdrBeginMarker(__FUNCTION__);

    rdrStatsPassBegin(viewport->renderPass);

    clearMatsForThisFrame();

    gfx_state.current_viewport_info = viewport;

    // @todo global state isn't best thing to use here when desire to go on multiple threads
    // this would have to become thread local storage and some variables accessed directly from
    // game_state along with it
    gfx_state.renderPass = viewport->renderPass;
    gfx_state.lod_model_override = viewport->lod_model_override;
    gfx_state.vis_limit_entity_dist_from_player = viewport->vis_limit_entity_dist_from_player;
    game_state.disableSimpleShadows = true;

    if (viewport->renderPass != RENDERPASS_COLOR)
    {
        // entity LOD override disabled due to performance problems
        // (hmm...wonder what performance problems comment refers to
        // is that sequences have to reload or something if entity lod changes?)
        //game_state.lod_entity_override = viewport->lod_entity_override;

        game_state.lod_scale = viewport->lod_scale;

        gfx_state.stencilAvailable = pbufStencilSupported(viewport_GetPBuffer(viewport)->flags);
        gfx_state.mainViewport = 0;
    }
    else
    {
        gfx_state.mainViewport = 1;
        if (gfx_state.postprocessing)
            gfx_state.stencilAvailable = pbufStencilSupported(pbRenderTexture.flags);
        else
            gfx_state.stencilAvailable = pbufStencilSupported(viewport_GetPBuffer(viewport)->flags);

        // Enable simple shadows (aka splats) for stencil shadows and shadowmap + interior
        if ((game_state.shadowMode == SHADOW_STENCIL) || 
            (game_state.shadowMode >= SHADOW_SHADOWMAP_LOW && isIndoors()))
            game_state.disableSimpleShadows = false;
    }

    gfx_state.doStencilShadows = gfx_state.stencilAvailable && viewport->renderStencilShadows &&
        (!(rdr_caps.chip & R200) || game_state.ati_stencil_leak); // temp until ATI fixes memory leak in driver when FBO's use stencil
    gfx_state.doCubemaps = gfx_state.mainViewport && game_state.cubemapMode && !editMode() && game_state.useCg && !baseedit_Mode();
    gfx_state.doPlanarReflections = gfx_state.mainViewport && (rdr_caps.allowed_features & GFXF_CUBEMAP) && game_state.reflectionEnable >= 2 && !editMode() && game_state.useCg && !baseedit_Mode();

    game_state.zocclusion = gfx_state.mainViewport ? sMainViewportZOcclusion : 0;
    game_state.bFlipYProjection = viewport->bFlipYProjection;

    PERFINFO_AUTO_START("gfxSetViewMat", 1);

    copyMat4(viewport->cameraMatrix, cam_info.cammat);
    gfxSetViewMat(cam_info.cammat, cam_info.viewmat, cam_info.inv_viewmat);
    
    PERFINFO_AUTO_STOP_START("gfxDrawSetupDrawing", 1);

    gfxDrawSetupDrawing(viewport);

    PERFINFO_AUTO_STOP_START("gfxWindowReshape", 1);

    gfxWindowReshapeForViewport(viewport, -1);

    // Must happen after gfxWindowReshapeForViewport()
    if (gfx_state.mainViewport)
        gfxExtractViewFrustums();

    PERFINFO_AUTO_STOP_START("gfxSetupStuffToDraw", 1);

    gfxSetupStuffToDraw(viewport);

    PERFINFO_AUTO_STOP_START("sortModels", 1);

    sortModels(viewport->renderOpaquePass, viewport->renderAlphaPass);

    PERFINFO_AUTO_STOP_START("rdrClearScreen", 1);

    if (sIsLetterBoxThisFrame && (viewport->renderPass == RENDERPASS_COLOR || viewport->renderPass == RENDERPASS_IMAGECAPTURE))
    {
        Vec3 blackBG = {0, 0, 0};
        rdrSetBgColor(blackBG);

        // Reset the viewport so leftover garbage around the letterbox will be 3D-cleared.
        // The viewport will be restored to letterbox after the clear.
        gfxWindowReshapeForViewport(NULL, -1);
    }

    gfxClearScreenFor3D(viewport);

    //setup letterbox viewport
    if (sIsLetterBoxThisFrame && (viewport->renderPass == RENDERPASS_COLOR || viewport->renderPass == RENDERPASS_IMAGECAPTURE))
    {
        gfxSetupLetterBoxViewport();
    }

    PERFINFO_AUTO_STOP_START("gfxDraw2DStuffPre3D", 1);
    if (viewport->render2D)
        gfxDraw2DStuffPre3D();

    if (!viewport->isRenderToTexture && viewport->renderPass == RENDERPASS_COLOR)
        rdrAmbientStart();

    PERFINFO_AUTO_STOP_START("gfxTreeDrawNodeSky",1);
    if (viewport->renderSky && !GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_SKY))
    {
        gfxTreeDrawNodeSky(sky_gfx_tree_root,cam_info.viewmat); // Does GL drawing, must be first actual drawing
    }
    
    PERFINFO_AUTO_STOP_START("renderOpaquePass",1);
    if (viewport->renderOpaquePass && !GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_OPAQUE_PASS))
    {
        drawSortedModels_ex(DSM_OPAQUE, viewport->doHeadshot, viewport, false);
    }

    PERFINFO_AUTO_STOP_START("draw lines",1);  

    if (gfx_state.mainViewport)
        drawReflector();

    if (viewport->renderLines && !GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_LINES))
    {
        rdrBeginMarker(__FUNCTION__":draw lines");
        if (editMode())
            showGridExternal();
        
        queuedLinesDraw();
        rdrEndMarker();
    }

    if (game_state.renderdump) {
        gfxRenderDump("%s_opaque", viewport->name);
    }

    PERFINFO_AUTO_STOP_START("rdrAmbientOcclusion",1);  

    // SSAO is probably too slow to use for other viewports
    if (!viewport->isRenderToTexture && viewport->renderPass == RENDERPASS_COLOR) {
        rdrAmbientOcclusion();
    }

    PERFINFO_AUTO_STOP_START("renderUnderwaterAlphaObjectsPass",1); 
    if (viewport->renderUnderwaterAlphaObjectsPass && !GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_UNDERWATER_ALPHA_PASS))
    {
        // Render alpha objects which may be below the water line so they show up in the
        // screen capture done for the water texture
        if (gfx_state.waterThisFrame && viewport->clipping_info.num_clip_planes < MAX_CLIP_PLANES)
        {
            int newPlaneIndex = viewport->clipping_info.num_clip_planes;

            viewport->clipping_info.clip_planes[newPlaneIndex][0] = -gfx_state.waterPlane[0];
            viewport->clipping_info.clip_planes[newPlaneIndex][1] = -gfx_state.waterPlane[1];
            viewport->clipping_info.clip_planes[newPlaneIndex][2] = -gfx_state.waterPlane[2];
            viewport->clipping_info.clip_planes[newPlaneIndex][3] = -gfx_state.waterPlane[3];
            viewport->clipping_info.flags[newPlaneIndex] = ClipPlaneFlags_CostumesOnly;
            viewport->clipping_info.num_clip_planes++;

            drawSortedModels_ex(DSM_ALPHA_UNDERWATER, viewport->doHeadshot, viewport, false);

            viewport->clipping_info.num_clip_planes--;

            bDidUnderwaterAlphaPass = true;
        }
    }

    PERFINFO_AUTO_STOP_START("renderAlphaPass", 1);
    if (viewport->renderAlphaPass && !GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_ALPHA_PASS))
    {
        rdrBeginMarker(__FUNCTION__":renderAlphaPass");
        if (viewport->renderWater && ((game_state.waterMode > WATER_OFF) || (game_state.reflectionEnable > 1)))
        {
            rdrWaterGrabBuffer(gfx_state.mainViewport);
        }

        // After grabbing water fbo texture state is reset so we need to reapply
        // global shadow map sampler settings etc. as necessary
        rdrShadowMapBeginUsing();    // Enable shadowmap lookups and setup necessary sampler/constant state

        if (bDidUnderwaterAlphaPass)
        {
            int newPlaneIndex = viewport->clipping_info.num_clip_planes;

            viewport->clipping_info.clip_planes[newPlaneIndex][0] = gfx_state.waterPlane[0];
            viewport->clipping_info.clip_planes[newPlaneIndex][1] = gfx_state.waterPlane[1];
            viewport->clipping_info.clip_planes[newPlaneIndex][2] = gfx_state.waterPlane[2];
            viewport->clipping_info.clip_planes[newPlaneIndex][3] = gfx_state.waterPlane[3] + 0.01f;    // So we don't fight with the water geometry
            viewport->clipping_info.flags[newPlaneIndex] = ClipPlaneFlags_CostumesOnly;
            viewport->clipping_info.num_clip_planes++;
        }

        drawSortedModels_ex(DSM_ALPHA, viewport->doHeadshot, viewport, viewport->renderWater);

        if (bDidUnderwaterAlphaPass)
        {
            viewport->clipping_info.num_clip_planes--;
        }
        rdrEndMarker();
    }

    PERFINFO_AUTO_STOP_START("renderStencilShadows",1); 
    if (gfx_state.doStencilShadows)
    {
        // 05-08-09: FIXME: stencil is getting corrupted in SSAO on ATI cards
        // Take this out once stencil corruption is fixed.
//        if (rdr_caps.chip & R200)
//            rdrClearScreenEx(CLEAR_STENCIL);

        drawSortedModels_ex(DSM_SHADOWS, viewport->doHeadshot, viewport, viewport->renderWater);
    }

    PERFINFO_AUTO_STOP_START("stencilShadowFinishScene",1); 
    if (gfx_state.doStencilShadows)
    {
        rdrBeginMarker(__FUNCTION__":stencilShadowFinishScene");
        shadowFinishScene();
        rdrEndMarker();
    }

    rdrQueue(DRAWCMD_RESETSTATS, &game_state.stats_cards, sizeof(int));

#if RDRMAYBEFIX
    //debug //use this to wrap anything you are fill interested in.  It might be buggy
    if( game_state.showDepthComplexity == 1 && !no_drawing)         
        rdrInitDepthComplexity();
#endif

    PERFINFO_AUTO_STOP_START("Run and Render Particle Engine",1);
    if (viewport->renderParticles)
    {
        if (!game_state.waterDebug || (game_state.waterDebug & 8))
        {
            partRunEngine(); 
        }
    }

    rdrShadowMapFinishUsing();    // Disable shadowmap lookups at the end of frame @todo probably not necessary?

#if RDRMAYBEFIX
    //debug
    if( game_state.showDepthComplexity == 1 && !no_drawing)          
        rdrDoDepthComplexity(game_state.fps);
#endif

    if (game_state.renderdump) {
        gfxRenderDump("%s_alpha", viewport->name);
    }

    game_state.bFlipYProjection = false;

    PERFINFO_AUTO_STOP();

    rdrStatsPassEnd(viewport->renderPass);

    gfx_state.current_viewport_info = NULL;
    rdrEndMarker();
}

static void gfxSetupLetterboxValues(void)
{
    F32 xsc, ysc, xOffset, yOffset;
    calc_scrn_scaling_with_offset(&xsc, &ysc, &xOffset, &yOffset);
    s_mainviewport.viewport_x = xOffset;
    s_mainviewport.viewport_y = yOffset;
    s_mainviewport.viewport_width = DEFAULT_SCRN_WD*xsc;
    s_mainviewport.viewport_height = DEFAULT_SCRN_HT*ysc;
    s_mainviewport.aspect = (F32) DEFAULT_SCRN_WD / DEFAULT_SCRN_HT;
    s_mainviewport.width = DEFAULT_SCRN_WD*xsc;
    s_mainviewport.height = DEFAULT_SCRN_HT*ysc;
    s_mainviewport.useMainViewportFOV = false;
}

void gfxSetupLetterBoxViewport(void)
{
    gfxSetupLetterboxValues();
    gfxWindowReshapeForViewport(&s_mainviewport, -1);
}

void gfxPrepareforLoadingScreen(void)
{
    gfxClearBackgroundToBlack();
    gfxSetupLetterBoxViewport();
}

// Setup the main viewport information to render the main viewport
// Some of the other viewports may need to reference some of the information
// (e.g., shadowmap rendering culls to the main camera view frustum even though
// it renders from the shadow light view)
static void gfxInitializeMainViewport(int force_render_world, int headShot, int no_drawing)
{
    int        w,h;
    int        final_w, final_h;

    windowSize(&w,&h);
    windowPhysicalSize(&final_w, &final_h);

    viewport_InitStructToDefaults(&s_mainviewport);
    s_mainviewport.renderPass = (force_render_world || headShot) ? RENDERPASS_IMAGECAPTURE : RENDERPASS_COLOR;
    s_mainviewport.fov = (headShot) ? game_state.fov_custom : game_state.fov;
    s_mainviewport.aspect = (F32) final_w/(F32) final_h;
    s_mainviewport.width = w;
    s_mainviewport.height = h;
    s_mainviewport.nearZ = game_state.nearz;
    s_mainviewport.farZ = game_state.farz;
    s_mainviewport.lod_scale = game_state.lod_scale;

    // Camera shake
    if (gfx_state.mainViewport)
        s_mainviewport.fov -= game_state.camera_shake * (rand() % 31) / 5.0;

    game_state.fov_thisframe = s_mainviewport.fov;

    // TODO: Get the camera matrix someplace else?
    copyMat4(cam_info.cammat, s_mainviewport.cameraMatrix);

    if (no_drawing)
    {
        s_mainviewport.renderSky = false;
        s_mainviewport.renderCharacters = false;
        s_mainviewport.renderOpaquePass = false;
        s_mainviewport.renderAlphaPass = false;
        s_mainviewport.renderWater = false;
        s_mainviewport.renderUnderwaterAlphaObjectsPass = false;
        s_mainviewport.renderStencilShadows = false;
        s_mainviewport.renderLines = false;
        s_mainviewport.renderParticles = false;
        s_mainviewport.render2D = false;
    }
    else
    {
        s_mainviewport.renderSky = s_mainviewport.renderOpaquePass && !headShot && !editMode() && !game_state.ortho && !game_state.disableSky && group_draw.see_outside && (game_state.game_mode == SHOW_GAME || force_render_world);
        s_mainviewport.renderOpaquePass = (game_state.game_mode == SHOW_TEMPLATE || game_state.game_mode == SHOW_GAME || force_render_world);
        s_mainviewport.renderAlphaPass = (game_state.game_mode == SHOW_TEMPLATE || game_state.game_mode == SHOW_GAME || force_render_world);
        s_mainviewport.renderWater = !headShot;
        s_mainviewport.renderUnderwaterAlphaObjectsPass = !headShot && s_mainviewport.renderAlphaPass && (game_state.game_mode == SHOW_GAME);
        s_mainviewport.renderStencilShadows = !headShot && s_mainviewport.renderAlphaPass && (game_state.shadowvol != 2) && game_state.shadowMode == SHADOW_STENCIL;
        s_mainviewport.renderLines = true;
        s_mainviewport.renderParticles = s_mainviewport.renderAlphaPass;
        s_mainviewport.render2D = !headShot && !game_state.disable2Dgraphics;
    }

    s_mainviewport.doHeadshot = headShot;
    s_mainviewport.useMainViewportFOV = true;

    s_mainviewport.isRenderToTexture = false;

    s_mainviewport.name = "main";

    gfx_state.main_viewport_info = &s_mainviewport;
}

#if 0
static float randomFloat(float min, float max)
{
    U32 rand = rule30Rand();
    F32 dif = max - min;
    F32 at = dif * rand / UINT_MAX;
    return min + at;
}

// only called when stress testing active
enum {
    eDebugRenderState_Idle,
    eDebugRenderState_MoveToPos,
    eDebugRenderState_LookAround,
    eDebugRenderState_MapMove
};
enum {
    eDebugRenderTestType_None,
    eDebugRenderTestType_ReloadGeoAndTex,
    eDebugRenderTestType_FlyAroundMaps,
    eDebugRenderTestType_FireFX
};

typedef struct MapLayout
{
    int        map_id;
    F32        minx, maxx, minz, maxz, y;
} MapLayout;
static MapLayout sMaps[] = {
    {1, -1690.f, 2650.f, -2200.f, 800.f, 120.f},        // atlas park
    //{77, 1500.f, 4500.f, 550.f, 2000.f, -220.f},    // grandville
    //{71, -3400.f, 50.f, -2900.f, 300.f, 260.f},        // mercy
    //{41, -140.f, 350.f, 67.f, 820.f, 100.f},        // neutral tutorial
    //{40, -4900.f, -3600.f, 980.f, 1720.f, 110.f},    // precinct five
    //{36, -6400.f, -3300.f, -2000.f, 2200.f, 180.f},    // nova praetoria
    //{49, -2200.f, 2200.f, -3000.f, 2500.f, 210.f},    // first ward
};

static void gfxUpdateDebug()
{
    static int sDebugTimer = 0;
    static int sDebugTestType = eDebugRenderTestType_ReloadGeoAndTex;
    static F32 sTargetDuration;
    static int sCurState = eDebugRenderState_MapMove; // start with move to first map in list
    static int sPrevState = eDebugRenderState_Idle;
    static int sDebugCounter1 = 0, sDebugCounter2 = 0;
    static int sCurMapIndex = -1;
    char scratch_buf[256];

    if ( !sDebugTimer )
    {
        // one time creation
        sDebugTimer = timerAlloc();
        timerStart(sDebugTimer);
    }

    if( timerElapsed(sDebugTimer) < sTargetDuration)
        return;

    switch(sDebugTestType)
    {
        case eDebugRenderTestType_ReloadGeoAndTex:
        {
            int thisRand = rand() % 3;
            switch(thisRand)
            {
                case 0: gfxResetTextures();            
                xcase 1: modelFreeAllCache(0);
                xdefault: break;
            }    
            sTargetDuration = randomFloat(0.5f, 3.f);
            break;
        }

        case eDebugRenderTestType_FireFX:
        {
            cmdParse("fire1");    
            sTargetDuration = randomFloat(2.f, 4.f);
            break;
        }

        case eDebugRenderTestType_FlyAroundMaps:
        {
            int nextState = sCurState;
            switch(sCurState)
            {
                case eDebugRenderState_Idle:
                    cmdParse("untargetable 1");
                    cmdParse("invincible 1");
                    cmdParse("nocoll 1");
                    cmdParse("timeset 12");
                    sDebugCounter1++;                
                    nextState = eDebugRenderState_MoveToPos;
                    sTargetDuration = 1.f;
                    break;
                case eDebugRenderState_MoveToPos:
                {                    
                    F32 xpos, ypos, zpos;
                    MapLayout* ml = &sMaps[sCurMapIndex];
                    xpos = randomFloat(ml->minx, ml->maxx);
                    ypos = ml->y;
                    zpos = randomFloat(ml->minz, ml->maxz);
                    sprintf(scratch_buf, "setpospyr %.2f %.2f %.2f 0.1220 1.3188 0.0", xpos, ypos, zpos);
                    cmdParse(scratch_buf);
                    nextState = eDebugRenderState_LookAround;
                    sTargetDuration = randomFloat(0.1f, 5.f); // short pause before looking around
                    break;
                }
                case eDebugRenderState_LookAround:
                    control_state.turnleft = 1;
                    #define NUM_MOVES_PER_MAP 20
                    nextState = (sDebugCounter1%NUM_MOVES_PER_MAP == 0)? eDebugRenderState_MapMove : eDebugRenderState_Idle;
                    sTargetDuration = randomFloat(1.f, 10.f);
                    break;
                case eDebugRenderState_MapMove:
                    sDebugCounter2++;
                    sCurMapIndex++; // advance to next map
                    if(sCurMapIndex >= ARRAY_SIZE(sMaps)) sCurMapIndex = 0;
                    timerMakeLogDateStringFromSecondsSince2000(scratch_buf,timerSecondsSince2000());
                    printf("==============================\n%s: Graphics debug: mapmove to map %d (%d map moves total)\n=============================\n",
                        scratch_buf, sMaps[sCurMapIndex].map_id, sDebugCounter2);
                    sprintf(scratch_buf, "mapmove %d", sMaps[sCurMapIndex].map_id);
                    cmdParse(scratch_buf);
                    nextState = eDebugRenderState_Idle;
                    sTargetDuration = 25.f; // give time for mapmove to complete
                    break;
            }

            // we always switch states
            sPrevState = sCurState;
            sCurState = nextState;
            break;
        }

        default:
            break;
    }

    timerStart(sDebugTimer);
}
#endif

//Core graphics renderer.  TO DO: make this not destructive (it clears the sprite list)
//The game mode is whether you are in show whole game, show just player, or show nothing mode
void gfxUpdateFrame(int force_render_world, int headShot, int no_drawing)
{
    static bool firstTime = true;
    int fxdebugsave = game_state.fxdebug;
    rdrBeginMarker(__FUNCTION__);

    rdrInitTopOfFrame();

    rdrStatsBeginFrame();

    if (firstTime)
    {
        gfx_state.lowest_visible_point_last_frame = 0.0f;
        firstTime = false;
    }

    if (!force_render_world && !headShot && !no_drawing)
    {
        gfx_state.lowest_visible_point_this_frame = FLT_MAX;
    }

    sIsLetterBoxThisFrame = isLetterBoxMenu() && !headShot;

    //if conditional is true, disable letterbox so that imageserver can take character snapshots
    //without black bars covering character's head and feet.  A gfxUpdateFrame call from seqgraphics.c's
    //gexThePixelBuffer function will set the parameters that will make this conditional true.
    if (!force_render_world && headShot == 1 && !no_drawing)
    {
        sIsLetterBoxThisFrame = 0;
    }

#if 0 // rendering stress test code for finding driver bugs
    if(game_state.ati_stencil_leak) { // hijacking ati_stencil_leak for now
        gfxUpdateDebug();
    }
#endif

    // setup the main viewport info struct as some of the other viewports
    // may need to reference some of the information
    // (e.g., shadowmap rendering culls to the main camera view frustum even though
    // it renders from the shadow light view)
    gfxInitializeMainViewport( force_render_world, headShot, no_drawing );

    if (!force_render_world && !headShot && !no_drawing && (!editMode() || game_state.gen_static_cubemaps))
    {
        if (rdr_caps.allowed_features&GFXF_CUBEMAP)    // if HW doesn't support reflections skip them entirely
        {
            cubemap_Update();
        }
        
        if (rdr_caps.allowed_features & (GFXF_CUBEMAP|GFXF_WATER_REFLECTIONS))
        {
            requestReflection();
        }

        // Render all the auxiliary viewports (e.g. shadows, reflections, etc.)
        // This does NOT render the main viewport that is rendered explicitly below
        PERFINFO_AUTO_START("viewport_RenderAll", 1);
        viewport_RenderAll();
        PERFINFO_AUTO_STOP();
    }

    if (game_state.zocclusion_off || !zoIsSupported() || (shellMode() && !loadingScreenVisible()) ||
        s_mainviewport.renderPass != RENDERPASS_COLOR)
        sMainViewportZOcclusion = 0;
    else
        sMainViewportZOcclusion = 1;

    // Whether or not to do the HDR stuff this frame
    gfx_state.postprocessing = !headShot && !no_drawing && !green_blobby_fx_id && !editMode() && (rdr_caps.features & (GFXF_BLOOM|GFXF_TONEMAP|GFXF_DOF|GFXF_DESATURATE)) &&
        (game_state.game_mode == SHOW_TEMPLATE || game_state.game_mode == SHOW_GAME) && !game_state.oldVideoTest &&
        !force_render_world;
    gfx_state.renderscale = !headShot && !no_drawing && (game_state.game_mode == SHOW_GAME) && game_state.useRenderScale;
    if (!headShot && !no_drawing && !game_state.useRenderScale && !gfx_state.postprocessing && (game_state.game_mode == SHOW_GAME) &&
        ((game_state.waterMode > WATER_LOW) && (rdr_caps.chip & R300) ||
        game_state.antialiasing > 1))
    {
        // To fix issue with ATI X850+ cards and reading the depth buffer being really slow
        // Also to enable Antialiasing (needs to render to PBuffer)
        gfx_state.renderscale = 1;
        game_state.renderScaleX = 1.0;
        game_state.renderScaleY = 1.0;
    }

    //Update global sway numbers used by trees and such
    initSwayTable(TIMESTEP);
    group_draw.sway_count+=TIMESTEP;
    if (group_draw.sway_count > 1000000.f)
        group_draw.sway_count = 0;

    if (headShot > 1)
        s_mainviewport.clear_flags |= CLEAR_ALPHA;

    PERFINFO_AUTO_START("gfxRenderViewport", 1);
    rdrBeginMarker(__FUNCTION__ " gfxRenderViewport(MAIN):%s", s_mainviewport.name);
        gfxRenderViewport(&s_mainviewport);
    rdrEndMarker();
    PERFINFO_AUTO_STOP();

    if (!force_render_world && !headShot)
    {
        PERFINFO_AUTO_START("sunVisible",1);
        sunVisible(); //calculate lens flare done after pix buf is full, and apply it next frame
        PERFINFO_AUTO_STOP();
    }

    rdrShadowMapDebug3D();

    PERFINFO_AUTO_START("winMakeCurrent",1);
    winMakeCurrent();
    PERFINFO_AUTO_STOP();

    if (gfx_state.renderscale)
    {
        int width, height;
        rdrBeginMarker(__FUNCTION__ " - renderscale" );
        windowSize(&width, &height);
        rdrViewport(0, 0, width, height, false);
        rdrEndMarker();
    }

    if (gfx_state.postprocessing) {
        // draw to screen
        PERFINFO_AUTO_START("rdrPostprocessing",1);
        rdrPostprocessing(&pbRenderTexture);
        winMakeCurrent();

        PERFINFO_AUTO_STOP();
    } else if (gfx_state.renderscale) {
        // draw to screen
        PERFINFO_AUTO_START("rdrRenderScaled",1);
        rdrRenderScaled(&pbRenderTexture);
        winMakeCurrent();
        PERFINFO_AUTO_STOP();
    }
    if (game_state.hdrThumbnail) {
        PERFINFO_AUTO_STOP_START("rdrHDRThumbnailDebug",1);
        rdrHDRThumbnailDebug();
    }

    if (game_state.renderdump) {
        gfxRenderDump("%s_after3d", s_mainviewport.name);
    }

    PERFINFO_AUTO_START("gfxDraw2DStuffPost3D", 1);

    if (!game_state.disable2Dgraphics) 
    {
        gfxDraw2DStuffPost3D(!no_drawing, headShot);

        if (game_state.renderdump) {
            gfxRenderDump("%s_after2d", s_mainviewport.name);
        }
    }
    else if( headShot != 2 )
        showFrameRate(); // timestep should probably be calculated even when there is no UI

    PERFINFO_AUTO_STOP_START("gfxDrawFinishFrame", 1);

    game_state.fxdebug = fxdebugsave;
    gfxDrawFinishFrame(!headShot && !no_drawing && !gfx_state.screenshot, headShot, s_mainviewport.renderPass);

    if (game_state.renderdump) {
        gfxRenderDump("%s_finish", s_mainviewport.name);
        game_state.renderdump = 0;
    }

    if (game_state.auto_perf && !shellMode() && !force_render_world && !loadingScreenVisible() && !editMode())
        gfxAutoSetPerformanceSettings();

    // Run texUnloadTexturesToFitMemory every 64 frames
    if ((!force_render_world && !headShot && !no_drawing) && (global_state.global_frame_count & 0x3f) == 0)
        texUnloadTexturesToFitMemory();

    perfCounterSample();

    gfx_state.postprocessing = 0;

    if (!force_render_world && !headShot && !no_drawing)
    {
        if (gfx_state.lowest_visible_point_this_frame < FLT_MAX)
            gfx_state.lowest_visible_point_last_frame = gfx_state.lowest_visible_point_this_frame;
        else
            gfx_state.lowest_visible_point_last_frame = 0.0f;
    }

    PERFINFO_AUTO_STOP();
    rdrEndMarker();
}

#define AUTOPERF_TIME 5
#define AUTOPERF_UPPER_FPS 40
#define AUTOPERF_LOWER_FPS 20
#define AUTOPERF_STEP 0.1f

void gfxAutoSetPerformanceSettings(void)
{
    static int init = 0;
    static int timer;
    static int frame_count;
    static float cur_visscale, old_visscale = -1;
    float time;

    if (!init)
    {
        timer = timerAlloc();
        init = 1;
    }

    if (game_state.vis_scale != old_visscale)
    {
        timerStart(timer);
        frame_count = global_state.global_frame_count;
        old_visscale = cur_visscale = game_state.vis_scale;
    }

    time = timerElapsed(timer);
    if (time > AUTOPERF_TIME)
    {
        float target_visscale = cur_visscale;
        float fps = (global_state.global_frame_count - frame_count) / time;
        
        if (fps > AUTOPERF_UPPER_FPS)
            target_visscale += AUTOPERF_STEP;
        else if (fps < AUTOPERF_LOWER_FPS)
            target_visscale -= AUTOPERF_STEP;

        if (target_visscale > game_state.vis_scale)
            target_visscale = game_state.vis_scale;
        else if (target_visscale < 0.5f)
            target_visscale = 0.5f;

        if (target_visscale != cur_visscale)
        {
            cur_visscale = target_visscale;
            setVisScale(cur_visscale, 0);
        }
        
        timerStart(timer);
        frame_count = global_state.global_frame_count;
    }
}

void setVisScale(float fScale, int setGameState)
{
    if (setGameState)
        game_state.vis_scale = fScale;
    if (fScale < 1)
    {
        game_state.draw_scale = fScale;
        game_state.lod_scale = 1 - ((1 - fScale) * 0.5f);
    }
    else
    {
        game_state.draw_scale = 1;
        game_state.lod_scale = fScale;
    }
}

