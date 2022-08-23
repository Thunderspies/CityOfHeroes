#include "graphics/gfxDebug.h"
#include "render/model.h"
#include <utilitieslib/utils/mathutil.h>
#include "render/renderUtil.h"
#include "cmdparse/cmdgame.h"
#include "render/renderprim.h"
#include "graphics/gfx.h"
#include "graphics/font.h"
#include "graphics/groupdraw.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/tga.h>
#include <utilitieslib/utils/tiff.h>
#include "graphics/gfxSettings.h"
#include "win/win_init.h"
#include <utilitieslib/utils/utils.h>
#include "edit/edit_cmd.h"
#include "clientError.h"
#include "graphics/seqgraphics.h"
#include "graphics/textureatlas.h"
#include "graphics/zOcclusion.h"
#include "graphics/groupThumbnail.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiWindows.h"
#include "gameComm/wdwbase.h"
#include "UI/uiConsole.h"
#include "player/player.h"
#include "UI/uiGame.h"
#include "render/pbuffer.h"
#include "edit/DebugLocation.h"
#include "render/tex.h"
#include "render/tex_gen.h"
#include <utilitieslib/components/GenericPoly.h>
#include "graphics/camera.h"
#include "graphics/jpeg.h"
#include <utilitieslib/components/StashTable.h>
#include "render/renderssao.h"
#include "graphics/gfxDevHUD.h"

//##########################################################################
//Odds and ends
//
//a little backwards, as it uses light lines.  It should have it's own pool and it's own drawEditorLines( call
//and dynamica allocs
extern Line    lightlines[1000];
extern int        lightline_count;

GPolySet *debug_polyset=0;

void addDebugLine( Vec3 va, Vec3 vb, U8 r, U8 b, U8 g, U8 a, F32 width )
{
    Line * line;
    if( lightline_count < 1000 ) 
    {
        line = &(lightlines[lightline_count++]);    
        copyVec3( va, line->a);    
        copyVec3( vb, line->b);   
        line->color[0] = r;    
        line->color[1] = g; 
        line->color[2] = b; 
        line->color[3] = a;
        line->width = width; 
    }
}

void fixCamMat(Mat4 mat)
{
    Mat4    temp;
    Vec3    dv;

    copyMat4(mat,temp);
    transposeMat3(temp);
    rdrFixMat(temp);
    subVec3(zerovec3,temp[3],dv);
    mulVecMat3(dv,temp,temp[3]);
    copyMat4(temp,mat);
}


//Minor utilities
void gfxDump360()
{
    F32        ang,angstep = 360.f / game_state.spin360;
    char    fname[100];

    for(ang=0;ang<360;ang+=angstep)
    {
        control_state.pyr.cur[1] += RAD(angstep);
        createMat3PYR(cam_info.cammat, control_state.pyr.cur);
        gfxSetViewMat(cam_info.cammat, cam_info.viewmat, cam_info.inv_viewmat);
        gfxUpdateFrame(1, 0, 0);
        sprintf(fname, "c:/temp/spin_%03f.bmp",ang);
        rdrPixBufDumpBmp(fname);
    }

    game_state.spin360 = 0;
}

//Debug
#include "render/thread/rt_state.h"
//ENd Debug

void printGeneralDebug()
{
    extern int seqLoadInstCalls;
    extern int seqFreeInstCalls;
    extern int drawGfxNodeCalls;
    extern int drawOrder;
    extern int next;
    extern int boned;
    extern int nonboned;
    extern int nonboned2;
    extern int maxSimultaneousFx;
    extern int fxCreatedCount;
    extern int fxDestroyedCount;
    extern int pbufInitCalls;


    int y = 5;

    xyprintf(10, y++, "maxSimultaneousFx %d", maxSimultaneousFx);
    xyprintf(10, y++, "fxCreatedCount %d", fxCreatedCount);
    xyprintf(10, y++, "fxDestroyedCount %d", fxDestroyedCount);
    y++;
    xyprintf(10, y++, "seqLoadInstCalls %d", seqLoadInstCalls);
    xyprintf(10, y++, "seqFreeInstCalls %d", seqFreeInstCalls);
    y++;
    xyprintf(10, y++, "drawGfxNodeCalls %d", drawGfxNodeCalls);
    drawGfxNodeCalls = 0;

    xyprintf(10, y++, "drawOrder %d", drawOrder);
    drawOrder = next = 0;

    xyprintf(10, y++, "boned        %d", boned);
    xyprintf(10, y++, "nonboned    %d", nonboned);
    xyprintf(10, y++, "nonboned2    %d", nonboned2);
    boned = nonboned = nonboned2 = 0;
    y++;

    xyprintf(10, y++, "pbufInitCalls %d", pbufInitCalls);
    y++;

    //showRender State change info
    {
        extern int call, drawCall;
        extern int blendModeSwitchOrder[200];
        extern int drawModeSwitchOrder[200];
        extern int startTypeDrawing;
        extern int startDistDrawing;
        extern int startAUWDrawing;
        extern int startShadowDrawing;
        extern int endDrawModels;
        extern int drawStartTypeDrawing;
        extern int drawStartDistDrawing;
        extern int drawStartAUWDrawing;
        extern int drawStartShadowDrawing;
        extern int drawEndDrawModels;
        int totalBlendStateCalls=0, totalBlendStateChanges=0, i;
        int totalDrawStateCalls=0, totalDrawStateChanges=0;
        int y_save;
        int x_2 = 60;

        y_save = y;
        xyprintfcolor(10, y++, 255, 20, 20, "                                 calls / changes");
        for (i=0; i<BLENDMODE_NUMENTRIES; i++) {
            xyprintf(10, y++, "%02x: BLENDMODE_%24s %d / %d", i, blend_mode_names[i], setBlendStateCalls[i], setBlendStateChanges[i]);
            totalBlendStateCalls   += setBlendStateCalls[i];
            totalBlendStateChanges += setBlendStateChanges[i];
        }
        xyprintfcolor(10, y++, 255, 20, 20, "   Totals                              %d / %d", totalBlendStateCalls, totalBlendStateChanges );

        for (i=0; i<DRAWMODE_NUMENTRIES; i++) {
            totalDrawStateCalls   += setDrawStateCalls[i];
            totalDrawStateChanges += setDrawStateChanges[i];
        }

        y = y_save;
        xyprintfcolor(x_2, y++, 255, 20, 20, "                               calls / changes");
        xyprintf(x_2, y++, "%02x: DRAWMODE_SPRITE                   %d / %d", DRAWMODE_SPRITE, setDrawStateCalls[DRAWMODE_SPRITE], setDrawStateChanges[DRAWMODE_SPRITE]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_DUALTEX                  %d / %d", DRAWMODE_DUALTEX, setDrawStateCalls[DRAWMODE_DUALTEX], setDrawStateChanges[DRAWMODE_DUALTEX]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_COLORONLY                %d / %d", DRAWMODE_COLORONLY, setDrawStateCalls[DRAWMODE_COLORONLY], setDrawStateChanges[DRAWMODE_COLORONLY]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_DUALTEX_NORMALS          %d / %d", DRAWMODE_DUALTEX_NORMALS, setDrawStateCalls[DRAWMODE_DUALTEX_NORMALS], setDrawStateChanges[DRAWMODE_DUALTEX_NORMALS]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_DUALTEX_LIT_PP           %d / %d", DRAWMODE_DUALTEX_LIT_PP, setDrawStateCalls[DRAWMODE_DUALTEX_LIT_PP], setDrawStateChanges[DRAWMODE_DUALTEX_LIT_PP]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_FILL                     %d / %d", DRAWMODE_FILL, setDrawStateCalls[DRAWMODE_FILL], setDrawStateChanges[DRAWMODE_FILL]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_BUMPMAP_SKINNED          %d / %d", DRAWMODE_BUMPMAP_SKINNED, setDrawStateCalls[DRAWMODE_BUMPMAP_SKINNED], setDrawStateChanges[DRAWMODE_BUMPMAP_SKINNED]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_BUMPMAP_SKINNED_MULTITEX %d / %d", DRAWMODE_BUMPMAP_SKINNED_MULTITEX, setDrawStateCalls[DRAWMODE_BUMPMAP_SKINNED], setDrawStateChanges[DRAWMODE_BUMPMAP_SKINNED]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_HW_SKINNED               %d / %d", DRAWMODE_HW_SKINNED, setDrawStateCalls[DRAWMODE_HW_SKINNED], setDrawStateChanges[DRAWMODE_HW_SKINNED]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_BUMPMAP_NORMALS          %d / %d", DRAWMODE_BUMPMAP_NORMALS, setDrawStateCalls[DRAWMODE_BUMPMAP_NORMALS], setDrawStateChanges[DRAWMODE_BUMPMAP_NORMALS]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_BUMPMAP_NORMALS_PP       %d / %d", DRAWMODE_BUMPMAP_NORMALS_PP, setDrawStateCalls[DRAWMODE_BUMPMAP_NORMALS_PP], setDrawStateChanges[DRAWMODE_BUMPMAP_NORMALS_PP]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_BUMPMAP_DUALTEX          %d / %d", DRAWMODE_BUMPMAP_DUALTEX, setDrawStateCalls[DRAWMODE_BUMPMAP_DUALTEX], setDrawStateChanges[DRAWMODE_BUMPMAP_DUALTEX]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_BUMPMAP_RGBS             %d / %d", DRAWMODE_BUMPMAP_RGBS, setDrawStateCalls[DRAWMODE_BUMPMAP_RGBS], setDrawStateChanges[DRAWMODE_BUMPMAP_RGBS]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_BUMPMAP_MULTITEX         %d / %d", DRAWMODE_BUMPMAP_MULTITEX, setDrawStateCalls[DRAWMODE_BUMPMAP_MULTITEX], setDrawStateChanges[DRAWMODE_BUMPMAP_MULTITEX]);
        xyprintf(x_2, y++, "%02x: DRAWMODE_BUMPMAP_MULTITEX_RGBS    %d / %d", DRAWMODE_BUMPMAP_MULTITEX_RGBS, setDrawStateCalls[DRAWMODE_BUMPMAP_MULTITEX_RGBS], setDrawStateChanges[DRAWMODE_BUMPMAP_MULTITEX_RGBS]);
        xyprintfcolor(x_2, y++, 255, 20, 20, "   Totals                              %d / %d", totalDrawStateCalls, totalDrawStateChanges );

        y++;
        xyprintf( 2,y++, "Order Blend Mode Switches are being made");
        for( i = 0 ; i < call ; i++ )
            xyprintf(i+2, y, "%x", blendModeSwitchOrder[i] );
        y++;
        if(startTypeDrawing)    xyprintf(startTypeDrawing    + 1, y, "T");
        if(startAUWDrawing)        xyprintf(startAUWDrawing     + 1, y, "W");
        if(startDistDrawing)    xyprintf(startDistDrawing    + 1, y, "D");
        if( startShadowDrawing && !(rdr_caps.disable_stencil_shadows || game_state.shadowvol == 2)) 
            xyprintf(startShadowDrawing  + 1, y, "S");
        xyprintf(endDrawModels       + 1, y, "E");
        y++;

        y++;
        xyprintf( 2,y++, "Order Draw Mode Switches are being made");
        for( i = 0 ; i < drawCall ; i++ )
            xyprintf(i+2, y, "%x", drawModeSwitchOrder[i] );
        y++;
        if(drawStartTypeDrawing)    xyprintf(drawStartTypeDrawing    + 1, y, "T");
        if(drawStartAUWDrawing)        xyprintf(drawStartAUWDrawing     + 1, y, "W");
        if(drawStartDistDrawing)    xyprintf(drawStartDistDrawing    + 1, y, "D");
        if( drawStartShadowDrawing && !(rdr_caps.disable_stencil_shadows || game_state.shadowvol == 2) )
            xyprintf(drawStartShadowDrawing  + 1, y, "S");
        xyprintf(drawEndDrawModels       + 1, y, "E");
        y++;

        drawCall = call = 0;
        startTypeDrawing = startAUWDrawing = startDistDrawing = startShadowDrawing = endDrawModels = 0;
        drawStartTypeDrawing = drawStartDistDrawing = drawStartAUWDrawing = drawStartShadowDrawing = drawEndDrawModels = 0;
        memset( blendModeSwitchOrder, 0, sizeof(blendModeSwitchOrder) );
        memset( drawModeSwitchOrder, 0, sizeof(drawModeSwitchOrder) );
        memset( setBlendStateCalls, 0 , sizeof(setBlendStateCalls) );
        memset( setBlendStateChanges, 0 , sizeof(setBlendStateChanges) );
        memset( setDrawStateCalls, 0 , sizeof(setDrawStateCalls) );
        memset( setDrawStateChanges, 0 , sizeof(setDrawStateChanges) );
    }
    {
        extern int sortedByDist;
        extern int sortedByType;
        y++;
        xyprintf(10, y++, "sortedByDist           %d", sortedByDist);
        xyprintf(10, y++, "sortedByType           %d", sortedByType);

        sortedByDist = sortedByType = 0;
    }
    y++;
    if(1){ //debug
        extern int sortedDraw; //debug
        xyprintf( 40, y++, "Nodes Drawn     %d", sortedDraw );
        sortedDraw = 0;
    } //end debug */

    {
        xyprintf( 10, y++, "callsToDrawDefInternal[0] %d", group_draw.calls[0] );
        xyprintf( 10, y++, "callsToDrawDefInternal[1] %d", group_draw.calls[1] );
        xyprintf( 10, y++, "callsToDrawDefInternal[2] %d", group_draw.calls[2] );
        xyprintf( 10, y++, "callsToDrawDefInternal[3] %d", group_draw.calls[3] );
        xyprintf( 10, y++, "callsToDrawDefInternal[4] %d", group_draw.calls[4] );
        xyprintf( 10, y++, "callsToDrawDefInternal[5] %d", group_draw.calls[5] );
        xyprintf( 10, y++, "callsToDrawDefInternal[6] %d", group_draw.calls[6] );
        xyprintf( 10, y++, "callsToDrawDefInternal[7] %d", group_draw.calls[7] );

        memset(group_draw.calls, 0, sizeof(group_draw.calls));
    }
    {
        extern int texLoadCalls;
        extern int texLoadStdCalls;
        extern int texBindCalls;
        y++;
        xyprintf( 10, y++, "texLoadCalls %d", texLoadCalls);
        xyprintf( 10, y++, "texLoadStdCalls %d", texLoadStdCalls);
        xyprintf( 10, y++, "texBindCalls %d", texBindCalls);
        texLoadCalls = texLoadStdCalls = texBindCalls = 0;
    }

    {
        extern PBuffer pbRenderTexture;
        y++;
        xyprintf( 10, y++, "gfx_state.waterThisFrame  %d", gfx_state.waterThisFrame);
        xyprintf( 10, y++, "gfx_state.postprocessing  %d", gfx_state.postprocessing);
        xyprintf( 10, y++, "gfx_state.renderscale     %d", gfx_state.renderscale);
        xyprintf( 10, y++, "gfx_state.renderToPBuffer %d", gfx_state.renderToPBuffer);
        xyprintf( 10, y++, "antialiasing              %d", gfx_state.renderToPBuffer?pbRenderTexture.hardware_multisample_level:0);
    }

}    //End debug


void gfxShaderPerfTest()
{
    F32 timePerShader=10;
    int timer = timerAlloc();
    int numInstructions, i;
    for (numInstructions=3; numInstructions<84; numInstructions++) {
        F32 t;
        extern int shader_perf_test_hack;
        shader_perf_test_hack = numInstructions;
        reloadShaderCallback("shaders/arb/multi9.fp", 1);
        rdrQueueFlush();
        timerStart(timer);
        for (i=0; (t = timerElapsed(timer)) < timePerShader; i++) {
            rdrClearScreen();
            gfxUpdateFrame(1, 0, 0);
        }
        printf("%d, %f     %d instructions: %d frames in %f seconds, %f fps, %f ms/frame\n", numInstructions, i/t, numInstructions, i, t, i/t, 1000*t/i);
    }
    timerFree(timer);
}

void gfxScreenDump(char *subdir,int is_movie, char * format_extension)
{
    gfxScreenDumpEx(subdir, is_movie, NULL, 0, format_extension);
}

void gfxScreenDumpEx(char *subdir,int is_movie, char *extra_jpeg_data, int extra_jpeg_data_len, char * format_extension)
{
    U8        *pix,*top,*bot,*t;
    int        disable2d,showfps,w,h,y,linesize;
    static    int    curr_tga_idx;
    static    int    curr_jpg_idx;
    int    * curr_idx_ptr;
    char    fname[1000],basedir[1000],*s;
    extern void engine_update();

    if (0) {
        // Debug for creating random texture
        pix = calloc(128*128*4,1);
        if (1) {
            int i, j, k;
            for (i=0; i<128; i++) 
                for (j=0; j<128; j++) {
                    int v = rule30FloatPct()*0.01*256;;
                    for (k=0; k<3; k++) 
                        pix[(i*128+j)*4+k] = MIN(255,v);
                    pix[(i*128+j)*4+3]=255;
                }
            tgaSave("C:\\game\\src\\texture_library\\btest\\buildingLightPattern.tga", pix, 128, 128, 3);
            return;
        }
    }

    if (!is_movie)
    {
        showfps = game_state.showfps;
        disable2d = game_state.disable2Dgraphics;
        game_state.showfps = 0;
        game_state.disable2Dgraphics = !game_state.screenshot_ui;
        gfx_state.screenshot = 1;
        engine_update();
        gfx_state.screenshot = 0;
        game_state.showfps = showfps;
        game_state.disable2Dgraphics = disable2d;
    }
    pix = rdrPixBufGet(&w,&h);

    strcpy(basedir,fileDataDir());
    s = strrchr(basedir,'/');
    if (s)
        *s = 0;
    strcat(basedir, "/");
    strcat(basedir, subdir);

    linesize = w*4;
    t = malloc(linesize);
    for(y=0;y<h/2;y++)
    {
        top = pix+y*linesize;
        bot = pix+(h-y-1)*linesize;
        memcpy(t,top,linesize);
        memcpy(top,bot,linesize);
        memcpy(bot,t,linesize);
    }
    free(t);

    if( 0 == stricmp("tga", format_extension ) )
        curr_idx_ptr = &curr_tga_idx;
    else if( 0 == stricmp("jpg", format_extension ) )
        curr_idx_ptr = &curr_jpg_idx;;

    if (is_movie)
    {
        static int frame_num;

        sprintf(fname,"%s/movie_%05d.%s",basedir,++frame_num,format_extension);
    }
    else
    {
        char    datestr[100],*s;
        static    char    last_datestr[100];
        static    int        subsecond;

//        timerMakeDateStringFromSecondsSince2000(datestr,timerSecondsSince2000());
        timerMakeLogDateStringFromSecondsSince2000(datestr,timerSecondsSince2000());
        for(s=datestr;*s;s++)
        {
            if (*s == ':' || *s == ' ')
                *s = '-';
        }
        if (stricmp(datestr,last_datestr)==0)
            sprintf(fname,"%s/screenshot_%s_%d.%s",basedir,datestr,++subsecond,format_extension);
        else
        {
            sprintf(fname,"%s/screenshot_%s.%s",basedir,datestr,format_extension);
            subsecond = 0;
        }
        strcpy(last_datestr,datestr);
    }

    makeDirectories(basedir);

    if( 0 == stricmp("tga", format_extension ) )
        tgaSave(fname,pix,w,h,3);
    else if( 0 == stricmp("jpg", format_extension ) )
        jpgSaveEx(fname,pix,4,w,h,extra_jpeg_data,extra_jpeg_data_len);

    free(pix);
}

void gfxTextureDump(int texid, char *fn)
{
    char filename[MAX_PATH];
    int w, h;
    U8 *pix;

    if (fn) {
        sprintf(filename, "C:/%s.tga", fn);
    } else {
        sprintf(filename, "C:/texture_%d.tga", texid);
    }

    pix = rdrGetTex(texid, &w, &h, 1, 1, 0);
    tgaSave(filename, pix, w, h, 4);

    free(pix);
}

void gfxRenderDump(char *fn, ...)
{
    char filename[MAX_PATH];
    char buf[MAX_PATH];
    void * color, * depth, * stencil;
    int w,h;
    va_list va;

    va_start(va, fn);
    vsnprintf(buf, sizeof(buf), fn, va);
    va_end(va);    

    windowSize(&w, &h);
    color = malloc(w * h * 4);
    depth = malloc(w * h * 4);
    stencil = malloc(w * h * 4);

    rdrGetFrameBufferAsync(0, 0, w, h, GETFRAMEBUF_RGBA, color);
    rdrGetFrameBufferAsync(0, 0, w, h, GETFRAMEBUF_DEPTH, depth);
    rdrGetFrameBufferAsync(0, 0, w, h, GETFRAMEBUF_STENCIL, stencil);
    rdrQueueFlush();

    snprintf(filename, sizeof(filename), "C:/%s_color.tiff", buf);
    tiffSave(filename, color, w, h, 4, 1, 1, 0, 1);
    snprintf(filename, sizeof(filename), "C:/%s_depth.tiff", buf);
    tiffSave(filename, depth, w, h, 1, 4, 1, 0, 0);
    snprintf(filename, sizeof(filename), "C:/%s_stencil.tiff", buf);
    tiffSave(filename, stencil, w, h, 1, 4, 1, 0, 0);

    free(stencil);
    free(depth);
    free(color);
}

// dumps the color buffer as TGA
void gfxRenderDumpColor( char *fn, ...)
{
    char filename[MAX_PATH];
    char buf[MAX_PATH];
    void * buffer;
    int w,h;
    va_list va;

    va_start(va, fn);
    vsnprintf(buf, sizeof(buf), fn, va);
    va_end(va);    


    windowSize(&w, &h);
    buffer = malloc(w * h * 4);

    rdrGetFrameBufferAsync(0, 0, w, h, GETFRAMEBUF_RGBA, buffer);
    rdrQueueFlush();

    snprintf(filename, sizeof(filename), "C:/%s.tga", buf);
    tgaSave(filename, buffer, w, h, 4);

    free(buffer);
}

void printEntityDrawingInfo()
{ //debug
    extern int g_EntsUpdatedByClient;
    extern int g_EntsProcessedForDrawing;
    extern int g_splatShadowsUpdated;
    extern int splatShadowsDrawn; 

    xyprintf( 0, 12, "entsUpdatedByClient     : %d ", g_EntsUpdatedByClient ); 
    xyprintf( 0, 13, "entsProcessedForDrawing : %d ", g_EntsProcessedForDrawing );
    //xyprintf( 0, 14, "g_splatShadowsUpdated  : %d ", g_splatShadowsUpdated );
    //xyprintf( 0, 15, "splatShadowsDrawn    : %d ", splatShadowsDrawn );

    g_EntsProcessedForDrawing = 0;
    g_splatShadowsUpdated = 0;
    g_EntsUpdatedByClient = 0;
} //debug

static BasicTexture *gfx_debug_tex = 0;
void gfxSetDebugTexture(int handle)
{
    U8 *data;
    int w,h;
    if (gfx_debug_tex)
        texGenFree(gfx_debug_tex);
    data = rdrGetTex(handle, &w, &h, 1, 1, 0);
    gfx_debug_tex = texGenNew(w, h, "DEBUG TEXTURE");
    texGenUpdate(gfx_debug_tex, data);
    free(data);
}

void gfxDrawGPolySet(GPolySet *set)
{
    int i, j, prevJ;
    for (i = 0; i < set->count; i++)
    {
        GPoly *poly = &set->polys[i];
        Vec4 plane;
        F32 dist;

        makePlane(poly->points[0], poly->points[1], poly->points[2], plane);
        dist = distanceToPlane(cam_info.cammat[3], plane);

        prevJ = poly->count-1;
        for (j = 0; j < poly->count; j++)
        {
            drawLine3D(poly->points[prevJ], poly->points[j], 0xffffffff);
            prevJ = j;
        }

        for (j = 2; j < poly->count; j++)
        {
            drawTriangle3D(poly->points[0], poly->points[j-1], poly->points[j], (dist < 0) ? 0x50ff0000 : 0x5000ff00);
        }
    }
}

#ifndef FINAL
typedef struct
{
    char name[100];
    U32 secondsSince2000;
} PiggLoggingEntry;

static StashTable g_pigg_logging_table = 0;

static void piggLoggingCallback(const char *file_name)
{
    char *index;
    char *end;
    char temp[100];
    int length;
    StashElement element;
    U32 time = timerSecondsSince2000();

    // Look for ".pigg:" in the file name
    index = strstri((char *)file_name, ".pigg:");
    if (!index)
        return;

    // Find the previous folder delimiter
    end = index;
    while (index != file_name && *index != '/')
        index--;

    if (*index == '/')
        index++;

    length = end - index;
    assert(length < 100);

    // extract the pigg file name without the ".pigg" extension
    strncpy(temp, index, length);
    temp[length] = 0;

    // Add or update the stash table
    {
        PiggLoggingEntry *logEntry = malloc(sizeof(PiggLoggingEntry));
        logEntry->secondsSince2000 = time;
        strcpy(logEntry->name, temp);

        stashAddPointerAndGetElement(g_pigg_logging_table, temp, logEntry, true, &element);
    }
}

static int piggLoggingEntryCompare(const void *a, const void *b)
{
    const PiggLoggingEntry *a1 = *(const PiggLoggingEntry **)a;
    const PiggLoggingEntry *b1 = *(const PiggLoggingEntry **)b;

    return b1->secondsSince2000 - a1->secondsSince2000;
}

static void piggLoggingDisplay()
{
    U32 time = timerSecondsSince2000();
    U32 elapsedTime;
    int numPiggs = stashGetValidElementCount(g_pigg_logging_table);
    int i = 0;
    int x = 20 + TEXT_JUSTIFY;
    int y = 50;
    StashTableIterator iter;
    StashElement elem;
    PiggLoggingEntry *logEntry;

    PiggLoggingEntry **entryArray;

    if (numPiggs < 1)
        return;

    // Make an array of pointers to the stash table elements
    entryArray = malloc(sizeof(PiggLoggingEntry*) * numPiggs);

    stashGetIterator(g_pigg_logging_table, &iter);
    for (i = 0; i < numPiggs; i++)
    {
        stashGetNextElement(&iter, &elem);
        logEntry = stashElementGetPointer(elem);

        entryArray[i] = logEntry;
    }

    // Sort them from newest to oldest
    qsort(entryArray, numPiggs, sizeof(PiggLoggingEntry*), piggLoggingEntryCompare);

    // Loop through the sorted list
    for (i = 0; i < numPiggs; i++)
    {
        logEntry = entryArray[i];
        elapsedTime = time - logEntry->secondsSince2000;

        // Fade from white to green over the first 10 seconds
        if (elapsedTime <= 10)
        {
            float fadeRatio = (10.0f - elapsedTime) / 10.0f;
            fontSysText(x, y, logEntry->name, 255 * fadeRatio, 255, 255 * fadeRatio);
            y += 10;
        }
        // Fade from green to blue over the next 20 seconds
        else if (elapsedTime < 30)
        {
            float fadeRatio = (20.0f - (elapsedTime - 10)) / 20.0f;
            fontSysText(x, y, logEntry->name, 0, 255 * fadeRatio, 255 * (1-fadeRatio));
            y += 10;
        }
        // Fade from blue to black over the next 30 seconds
        else if (elapsedTime < 60)
        {
            float fadeRatio = (30.0f - (elapsedTime - 30)) / 30.0f;
            fontSysText(x, y, logEntry->name, 0, 0, 255 * fadeRatio);
            y += 10;
        }
        else
        {
            break;
        }
    }


    free(entryArray);
    
}

static void piggLoggingUpdate()
{
    if (g_client_debug_state.pigg_logging > 0)
    {
        if (g_pigg_logging_table == 0)
        {
            g_pigg_logging_table = stashTableCreateWithStringKeys( 100, StashDeepCopyKeys );
        }

        fileSetExtraLogging(piggLoggingCallback);

        piggLoggingDisplay();
    }
    else
    {
        fileSetExtraLogging(NULL);

        // TODO: Free the stash table?
    }
}
#else
static void piggLoggingUpdate()
{
}
#endif


void gfxShowDebugIndicators(void)
{
    int y=4;

    if (game_state.debug_tex && gfx_debug_tex)
        display_sprite_tex(gfx_debug_tex, 0, 0, 2, game_state.debug_tex, game_state.debug_tex, 0xffffffff);
    if(game_state.tintAlphaObjects > 1)
        xyprintf(1, y++, "Tinting objects by alpha: RED = ALPHA, GREEN = OPAQUE, BLUE = ALPHACUTOUT, PURPLE = ALPHAREF");
    else if(game_state.tintAlphaObjects)
        xyprintf(1, y++, "Tinting objects by alpha: RED = ALPHA, GREEN = OPAQUE");
    if(game_state.tintOccluders)
        xyprintf(1, y++, "Tinting occluders: RED = not considered as occluder, BLUE = possible occluder, GREEN = occluder");
    if(server_visible_state.pause)
        xyprintf(1, y++, "Paused");
    if( game_state.wireframe && !editMode() )
        xyprintf( 10, y++, "TIME: %f", server_visible_state.time ); 
    if(game_state.oldVideoTest)
        xyprintf(1, y++, "GeForce4 Simulation mode");
    if( game_state.showSpecs )
    {
        GfxSettings gfxSettings;
        y = 15;

        rdrPrintScreenSystemSpecs();

        gfxGetSettings( &gfxSettings );

        xyprintf( 20, 20, "%f", game_state.gamma );

        xyprintf( 10, y++, "CURRENT SETTINGS" );
        xyprintf( 10, y++, "mipLevel:          %d", gfxSettings.advanced.mipLevel  );
        xyprintf( 10, y++, "entityMipLevel:    %d", gfxSettings.advanced.entityMipLevel  );
        xyprintf( 10, y++, "texLodBias:        %d", gfxSettings.advanced.texLodBias  );
        xyprintf( 10, y++, "texAniso:          %d", gfxSettings.advanced.texAniso  );
        xyprintf( 10, y++, "worldDetailLevel:  %f", gfxSettings.advanced.worldDetailLevel  );
        xyprintf( 10, y++, "entityDetailLevel: %f", gfxSettings.advanced.entityDetailLevel  );
        xyprintf( 10, y++, "gamma:               %f", gfxSettings.gamma  );
        xyprintf( 10, y++, "shadowMode:        %d", gfxSettings.advanced.shadowMode  );
        xyprintf( 10, y++, "cubemapMode:       %d", gfxSettings.advanced.cubemapMode  );
        xyprintf( 10, y++, "ageiaOn:           %d", gfxSettings.advanced.ageiaOn  );
        xyprintf( 10, y++, "physicsQuality:       %d", gfxSettings.advanced.physicsQuality  );
        xyprintf( 10, y++, "maxParticles:      %d", gfxSettings.advanced.maxParticles  );
        xyprintf( 10, y++, "maxParticleFill:   %f", gfxSettings.advanced.maxParticleFill  );
        xyprintf( 10, y++, "screenX:           %d", gfxSettings.screenX  );
        xyprintf( 10, y++, "screenY:           %d", gfxSettings.screenY  );
        xyprintf( 10, y++, "refreshRate:       %d", gfxSettings.refreshRate  );
        xyprintf( 10, y++, "screenX_pos:       %d", gfxSettings.screenX_pos  );
        xyprintf( 10, y++, "screenY_pos:       %d", gfxSettings.screenY_pos  );
        xyprintf( 10, y++, "maximized:         %d", gfxSettings.maximized  );
        xyprintf( 10, y++, "fullScreen:        %d", gfxSettings.fullScreen  );
        xyprintf( 10, y++, "fxSoundVolume:     %f", gfxSettings.fxSoundVolume  );
        xyprintf( 10, y++, "musicSoundVolume:  %f", gfxSettings.musicSoundVolume  );
        xyprintf( 10, y++, "voSoundVolume:     %f",    gfxSettings.voSoundVolume  );
        xyprintf( 10, y++, "enableVBOS:        %d", gfxSettings.advanced.enableVBOs  );

        gfxGetSettingsForNextTime( &gfxSettings );
        y++;
        xyprintf( 10, y++, "SETTINGS TO BE SAVED" );
        xyprintf( 10, y++, "mipLevel:          %d", gfxSettings.advanced.mipLevel  );
        xyprintf( 10, y++, "entityMipLevel:    %d", gfxSettings.advanced.entityMipLevel  );
        xyprintf( 10, y++, "texLodBias:        %d", gfxSettings.advanced.texLodBias  );
        xyprintf( 10, y++, "texAniso:          %d", gfxSettings.advanced.texAniso  );
        xyprintf( 10, y++, "worldDetailLevel:  %f", gfxSettings.advanced.worldDetailLevel  );
        xyprintf( 10, y++, "entityDetailLevel: %f", gfxSettings.advanced.entityDetailLevel  );
        xyprintf( 10, y++, "gamma:               %f", gfxSettings.gamma  );
        xyprintf( 10, y++, "shadowMode:        %d", gfxSettings.advanced.shadowMode  );
        xyprintf( 10, y++, "cubemapMode:       %d", gfxSettings.advanced.cubemapMode  );
        xyprintf( 10, y++, "ageiaOn:           %d", gfxSettings.advanced.ageiaOn  );
        xyprintf( 10, y++, "physicsQuality:       %d", gfxSettings.advanced.physicsQuality  );
        xyprintf( 10, y++, "maxParticles:      %d", gfxSettings.advanced.maxParticles  );
        xyprintf( 10, y++, "maxParticleFill:   %f", gfxSettings.advanced.maxParticleFill  );
        xyprintf( 10, y++, "screenX:           %d", gfxSettings.screenX  );
        xyprintf( 10, y++, "screenY:           %d", gfxSettings.screenY  );
        xyprintf( 10, y++, "refreshRate:       %d", gfxSettings.refreshRate  );
        xyprintf( 10, y++, "screenX_pos:       %d", gfxSettings.screenX_pos  );
        xyprintf( 10, y++, "screenY_pos:       %d", gfxSettings.screenY_pos  );
        xyprintf( 10, y++, "maximized:         %d", gfxSettings.maximized  );
        xyprintf( 10, y++, "fullScreen:        %d", gfxSettings.fullScreen  );
        xyprintf( 10, y++, "fxSoundVolume:     %f", gfxSettings.fxSoundVolume  );
        xyprintf( 10, y++, "musicSoundVolume:  %f", gfxSettings.musicSoundVolume  );
        xyprintf( 10, y++, "voSoundVolume:     %f", gfxSettings.voSoundVolume  );
        xyprintf( 10, y++, "enableVBOS:        %d", gfxSettings.advanced.enableVBOs  );
    }
    if( game_state.renderinfo == 1 )
        printGeneralDebug(); //general debug stuff unrelated necessarily to fx

    {
        y = 42;
        if (game_state.lod_entity_override)
            xyprintf(1, y-1, "lod_entity_override %d", game_state.lod_entity_override);
        if (game_state.conorhack)
            xyprintf(1, y, "conorhack %d", game_state.conorhack);
#define TEST_PRINTOUT(var) \
        if (game_state.test##var)                                            \
            xyprintf(1, y+var, "test" #var " %f", game_state.test##var);    \
        if (game_state.test##var##flicker) game_state.test##var = !game_state.test##var;
        TEST_PRINTOUT(1);
        TEST_PRINTOUT(2);
        TEST_PRINTOUT(3);
        TEST_PRINTOUT(4);
        TEST_PRINTOUT(5);
        TEST_PRINTOUT(6);
        TEST_PRINTOUT(7);
        TEST_PRINTOUT(8);
        TEST_PRINTOUT(9);
        TEST_PRINTOUT(10);
    }

    if (game_state.waterDebug)
        xyprintf(1, 61, "waterDebug: %d", game_state.waterDebug);

    if( game_state.seq_info )
        printEntityDrawingInfo();
    displayLog(); 
    if( game_state.showPerf ) 
        printGfxSettings();
    statusLineDraw();
    if (game_state.atlas_stats)
        atlasDisplayStats();
    if (game_state.zocclusion_debug)
        zoShowDebug();

    if (!conVisible() && playerPtr() && isMenu(MENU_GAME))
    {
        static int previous_canedit = 0;
        if (game_state.can_edit && !previous_canedit && game_state.iAmAnArtist)
            window_setMode(WDW_RENDER_STATS, WINDOW_GROWING);
        previous_canedit = game_state.can_edit;
    }
    if( game_state.groupShot[0] )
    {
        AtlasTex* groupbind = 0;
        groupbind = groupThumbnailGet(game_state.groupShot, true, false,0);
        display_sprite( groupbind, 384, 256, 5000, 1.0, 1.0, 0xffffffff );
    }
    if (DebugLocationCount())
        DebugLocationDisplayAll();

    if (debug_polyset)
        gfxDrawGPolySet(debug_polyset);

    piggLoggingUpdate();
}

void gfxShowDebugIndicators2(void)
{
    if( game_state.headShot > 0 )
    {
        static AtlasTex * bind = 0;
        if( !bind )
            bind = white_tex_atlas;
        if( game_state.headShot > 1 )
        {
            static int dummyUniqueId;
            const cCostume * costume;
            dummyUniqueId++;
            costume = npcDefsGetCostume( game_state.headShot, 0 );
            bind = seqGetHeadshot( costume, dummyUniqueId );
            //seqMakeABodyShotTga( costume, "c:/matthew/test.tga" );
            game_state.headShot = 1;
        }
        display_sprite( bind, 128, 256, 5000, 1.0, 1.0, 0xffffffff );
    }
}

// a frame tick for graphics debug services
void gfxDebugUpdate(void)
{
    if (!isDevelopmentOrQAMode()) {
        return;
    }
    gfxDevHudUpdate();
}

// get input for debug services where appropriate
void gfxDebugInput(void)
{
    if (!isDevelopmentOrQAMode()) {
        return;
    }

    rdrAmbientDebugInput();
    gfxDevHudInput();
}