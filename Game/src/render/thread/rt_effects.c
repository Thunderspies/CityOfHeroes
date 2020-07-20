#define RT_PRIVATE
#define RT_ALLOW_BINDTEXTURE
#include "render/thread/rt_effects.h"
#include "render/thread/ogl.h"
#include "render/thread/wcw_statemgmt.h"
#include "render/thread/rt_state.h"
#include "render/thread/rt_tex.h"
#include "render/thread/rt_font.h"
#include "render/thread/rt_cgfx.h"
#include <utilitieslib/utils/mathutil.h>
#include "render/thread/rt_model.h"
#include "assert.h"
#include "cmdparse/cmdgame.h"
#include "render/thread/rt_pbuffer.h"
#include "rt_prim.h"
#include <utilitieslib/utils/timing.h>
#include "graphics/font.h"
#include "rt_shaderMgr.h"


typedef enum EffectShader {
    SHADER_SHRINK_EXTEND,
    SHADER_HBLUR,
    SHADER_VBLUR,
    SHADER_TONEMAP,
    SHADER_SHRINK2,
    SHADER_SHRINK2DOF,
    SHADER_SHRINK4,
    SHADER_SHRINK4LUM,
    SHADER_SHRINK4EXP,
    SHADER_LIGHTADAPTATION,
    SHADER_LOG,
    SHADER_BRIGHTPASS,
    SHADER_TONEMAP2,
    SHADER_SUNFLAREADAPTATION,
    SHADER_PERFORMANCE_TEST,
    SHADER_DOF_FINAL,
    SHADER_DOF_BLOOM_FINAL,
    SHADER_TONEMAP2_DESATURATE,
    SHADER_DOF_FINAL_DESATURATE,
    SHADER_DOF_BLOOM_FINAL_DESATURATE,
    SHADER_SIMPLE_DESATURATE,                  // vanilla shader
    SHADER_NUM_SPECIAL_SHADERS,
} EffectShader;

int shaderEffectsPrograms[SHADER_NUM_SPECIAL_SHADERS];
const char* shaderEffectNames[2][SHADER_NUM_SPECIAL_SHADERS] = {
    {   //ARB
      "shrink",                    
      "hblur",                    
      "vblur",                    
      "tonemap",                  
        "shrink2x",                 
      "shrink2xDof",              
      "shrink4x",                 
      "shrink4xLum",              
      "shrink4xExp",              
      "lightAdaptation",          
      "log",                      
      "brightpass",               
      "tonemap2",                 
      "sunflareAdaptation",       
      "performanceTest",          
      "dofFinal",                 
      "dofBloomFinal",            
      "tonemap2_desaturate",      
      "dofFinal_desaturate",      
      "dofBloomFinal_desaturate", 
      "simple_desaturate"
    },
    {   //Cg
      "shrink",                    
      "blur",                    
      "blur",                    
      "tonemap",                  
      "shrink",                 
      "shrink",              
      "shrink4x",                 
      "shrink4x",              
      "shrink4x",              
      "lightAdaptation",          
      "log",                      
      "brightpass",               
      "tonemap2",                 
      "sunflareAdaptation",       
      "performanceTest",          
      "dofFinal",                 
      "dofBloomFinal",            
      "tonemap2",      
      "dofFinal",      
      "dofBloomFinal", 
      "simple_desaturate"
    }
};

//NOTE: depends on EffectShader ordering.. if that changes, you must change this as well
const char* shaderExtraDefines[SHADER_NUM_SPECIAL_SHADERS] = {
    "HIGH_RANGE",               //SHADER_SHRINK_EXTEND
    "BLUR_HOR",                 //SHADER_HBLUR
    "BLUR_VER",                 //SHADER_VBLUR
    NULL,                       //SHADER_TONEMAP
    NULL,                       //SHADER_SHRINK2
    "DOF_INTO_BLURRED_TEXTURE", //SHADER_SHRINK2DOF
    NULL,                       //SHADER_SHRINK4
    "USE_LUMINANCE",            //SHADER_SHRINK4LUM
    "USE_EXP",                  //SHADER_SHRINK4EXP
    NULL,                       //SHADER_LIGHTADAPTATION
    NULL,                       //SHADER_LOG
    NULL,                       //SHADER_BRIGHTPASS
    NULL,                       //SHADER_TONEMAP2
    NULL,                       //SHADER_SUNFLAREADAPTATION
    NULL,                       //SHADER_PERFORMANCE_TEST
    NULL,                       //SHADER_DOF_FINAL
    NULL,                       //SHADER_DOF_BLOOM_FINAL
    "DESATURATE",               //SHADER_TONEMAP2_DESATURATE
    "DESATURATE",               //SHADER_DOF_FINAL_DESATURATE
    "DESATURATE",               //SHADER_DOF_BLOOM_FINAL_DESATURATE
    "DESATURATE"                //SHADER_SIMPLE_DESATURATE
};

bool getSpecialShaderName(int shader, char* nameBuff, size_t nameBuffLen, U32* pnFlags /*tShaderCompileFlags*/, const char** extraDefineSet)
{
    bool bIsCG = rt_cgGetCgShaderMode()!=0;
    if (shader < 0 || shader >= ARRAY_SIZE(shaderEffectNames[bIsCG]))
        return false;
        
    if (( nameBuff != NULL ) && ( nameBuffLen > 0 ))
    {
        static const char* kCgRootPath    = RT_CGFX_SHADER_PATH_EFFECTS "/";
        static const char* kArbRootPath    = "shaders/arb/effects/";
        static const char* kCgFileExt    = "fp.cg";
        static const char* kArbFileExt    = ".fp";
        
        const char* szRootPath    = ( bIsCG ) ? kCgRootPath : kArbRootPath;
        const char* szFileExt    = ( bIsCG ) ? kCgFileExt : kArbFileExt;

        assert( nameBuffLen > ( strlen( szRootPath ) + strlen(shaderEffectNames[bIsCG][shader]) + strlen( szFileExt ) ));
        sprintf_s( nameBuff, nameBuffLen, "%s%s%s", szRootPath, shaderEffectNames[bIsCG][shader], szFileExt );
    }
    if ( pnFlags != NULL )
    {
        *pnFlags = ( bIsCG ) ? kCompileFlag_Cg : kCompileFlag_ARB;
    }
    if ( extraDefineSet != NULL && bIsCG )
    {
        (*extraDefineSet) = shaderExtraDefines[shader];
    }
    return true;
}


int gamma_tex;

PBuffer pbBrightPass={0};
PBuffer pbBlurBuffer2={0};
PBuffer pbBlurBuffer={0};
PBuffer pb64x64={0};
PBuffer pb16x16={0};
PBuffer pb4x4={0};
PBuffer pb1x1={0};
PBuffer pb1x1LightA={0};
PBuffer pb1x1LightB={0};
PBuffer *pb1x1LightAdaptation=&pb1x1LightA;
PBuffer *pb1x1LightAdaptationSpare=&pb1x1LightB;
int blur_scale = 2;
F32 blur_amount=0.5;
F32 exposure = 1.0;

const char *thumbnail_names[16];
int thumbnails[16] = {0};
int thumbnail_index=0;

static int getNextThumbnailTexture(const char *name, int width, int height, bool depth)
{
    int handle = 0;
    bool created = false;

    assert(thumbnail_index < 16);

    if (!thumbnails[thumbnail_index]) {
        glGenTextures(1, &thumbnails[thumbnail_index]); CHECKGL;
        created = true;
    }
    WCW_BindTexture(GL_TEXTURE_2D, 0, thumbnails[thumbnail_index]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECKGL;

    if (depth) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0); CHECKGL;
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); CHECKGL;
    }

    handle = thumbnails[thumbnail_index];

    thumbnail_names[thumbnail_index] = name;
    thumbnail_index++;

    return handle;
}

void saveThumbnail(PBuffer *pb, const char *name)
{
    int w = pb->virtual_width;
    int h = pb->virtual_height;
    bool floatbuffer = pb->isFloat;
    int handle = 0;

    if (!game_state.hdrThumbnail)
        return;

    if (!rdr_caps.supports_arb_np2)
    {
        w = pow2(w);
        h = pow2(h);
    }

    handle = getNextThumbnailTexture(name, w, h, false);

    winCopyFrameBufferDirect(GL_TEXTURE_2D, handle, 0, 0, 0, w, h, 0, false, false);
}

void saveThumbnailColorDepth(PBuffer *pb, const char *color_name, const char *depth_name)
{
    int w = pb->virtual_width;
    int h = pb->virtual_height;
    bool floatbuffer = pb->isFloat;
    int color_texture = 0;
    int depth_texture = 0;

    if (!game_state.hdrThumbnail)
        return;

    if (!rdr_caps.supports_arb_np2)
    {
        w = pow2(w);
        h = pow2(h);
    }

    color_texture = getNextThumbnailTexture(color_name, w, h, false);
    depth_texture = getNextThumbnailTexture(depth_name, w, h, true);

    pbufCopyDirect(pb, GL_TEXTURE_2D, color_texture, depth_texture, 0, 0, w, h);
}

void saveThumbnailPBAux(PBuffer *pb, int buf, const char *name) // this buf is 1 off from the index into the aux buffers
{
    static GLenum buflookup[] = {GL_FRONT_LEFT, GL_AUX0, GL_AUX1, GL_AUX2, GL_AUX3};
    static char *aux_buff = NULL;
    static int aux_buff_size = 0;
    int size = pb->virtual_width * pb->virtual_height * 4;
    if (size > aux_buff_size) {
        SAFE_FREE(aux_buff);
        aux_buff = malloc(aux_buff_size = size);
    }
    if (pb->flags & PB_FBO) {
        if (buf == 0) {
            WCW_BindTexture(GL_TEXTURE_2D, 0, pb->color_handle[pb->curFBO]);
        } else {
            WCW_BindTexture(GL_TEXTURE_2D, 0, pb->aux_handles[buf-1]);
        }
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, aux_buff); CHECKGL;
    } else {
        glReadBuffer(buflookup[buf]); CHECKGL;
        glReadPixels(0, 0, pb->virtual_width, pb->virtual_height, GL_RGBA, GL_UNSIGNED_BYTE, aux_buff); CHECKGL;
    }
    if (!thumbnails[thumbnail_index]) {
        glGenTextures(1, &thumbnails[thumbnail_index]); CHECKGL;
    }
    WCW_BindTexture(GL_TEXTURE_2D, 0, thumbnails[thumbnail_index]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECKGL;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pb->virtual_width, pb->virtual_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, aux_buff); CHECKGL;
    thumbnail_names[thumbnail_index] = name;
    thumbnail_index++;
    if (pb->flags & PB_FBO) {
    } else {
        glReadBuffer(GL_FRONT_LEFT); CHECKGL;
    }
}

void saveThumbnailPB(PBuffer *pb, const char *name)
{
    if (!game_state.hdrThumbnail)
        return;
    if (pb->num_aux_buffers)
    {
        saveThumbnailPBAux(pb, 0, "Aux0");
        saveThumbnailPBAux(pb, 1, "Aux1");
    } else {
        saveThumbnail(pb, name);
    }
}

void rdrHDRThumbnailDebugDirect(void)
{
    int i;
    WCW_FetchGLState();
    WCW_ResetState();
    rdrSetup2DRenderingDirect();
    WCW_BlendFuncPush(GL_ONE, GL_ZERO);
    glDisable(GL_ALPHA_TEST); CHECKGL;
    texSetWhite(TEXLAYER_GENERIC);
    WCW_PrepareToDraw();
    //for (i=0; i<ARRAY_SIZE(thumbnails); i++) if (thumbnails[i])
    for (i=0; i<thumbnail_index; i++) if (thumbnails[i])
    {
        int x0 = 8+(128+8)*i;
        int x1 = x0 + 128;
        int y0 = 8;
        int y1 = y0 + 128;
        texBind(TEXLAYER_BASE, thumbnails[i]);
        WCW_BlendFunc(GL_ONE, GL_ZERO);
        glBegin( GL_QUADS );
        glTexCoord2f( 0, 1 );
        glVertex2f( x0, y1 );
        glTexCoord2f( 1, 1 );
        glVertex2f( x1, y1 );
        glTexCoord2f( 1, 0 );
        glVertex2f( x1, y0 );
        glTexCoord2f( 0, 0 );
        glVertex2f( x0, y0 );
        glEnd(); CHECKGL;

        xyprintf(x0/8, (rdr_view_state.origHeight - y1) / 8 - 1, thumbnail_names[i] ? thumbnail_names[i] : "");
    }


    WCW_Color4(255, 255, 255, 255);

    texSetWhite(TEXLAYER_BASE);

    glEnable(GL_ALPHA_TEST); CHECKGL;
    WCW_BlendFuncPop();
    rdrFinish2DRenderingDirect();
    thumbnail_index = 0;
}

void rdrSetRenderScaleFilter(void)
{
    if (rdr_view_state.renderScaleFilter==0) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECKGL;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECKGL;
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECKGL;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECKGL;
    }
}

void debugReadFrameBuffer(void)
{
    unsigned char buf[128*128*4];
    int i, j;
    glReadPixels(0, 0, 128, 128, GL_RGBA, GL_UNSIGNED_BYTE, buf); CHECKGL;
    for (i=0; i<16; i++) {
        for (j=0; j<4; j++) {
            printf("%2x%2x%2x%2x ", buf[i*128*4 + j*4], buf[i*128*4 + j*4+1], buf[i*128*4 + j*4+2], buf[i*128*4 + j*4+3]);
        }
        printf("\n");
    }
}


F32 distanceToDepthValue(F32 dist)
{
#define M rdr_view_state.projectionMatrix
    F32 effdist = -dist*2;
    return (effdist*M[2][2] + M[3][2]) / (effdist*M[2][3] + M[3][3]);
#undef M
}

F32 depthValueToDistance(F32 depth)
{
#define M rdr_view_state.projectionMatrix
    F32 effdist;
    F32 dist;
    effdist = (M[3][3]*depth - M[3][2]) / (M[2][3] * depth - M[2][2]);
    dist = 0.5*effdist; // Why no negative needed here?
    //assert(nearSameF32(depth, distanceToDepthValue(dist)));
    return dist;
#undef M
}




static void setOrthoProjection(int w, int h)
{
    WCW_LoadProjectionMatrixIdentity();
    glOrtho(0, w, 0, h, -1.0, 1.0); CHECKGL;
    WCW_LoadTextureMatrixIdentity(0);
    WCW_LoadModelViewMatrixIdentity();
    glViewport(0, 0, w, h); CHECKGL;
}


// create gamma lookup table texture
GLuint createGammaTexture(int size, float gamma)
{
    GLuint texid;
    int i;
    float *img = _alloca(sizeof(float) * size);
    if (!img) return 0;

    glGenTextures(1, &texid); CHECKGL;

    WCW_BindTexture(GL_TEXTURE_1D, 0, texid);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECKGL;
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECKGL;
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); CHECKGL;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); CHECKGL;

    for(i=0; i<size; i++) {
        float x = i / (float) size;
        img[i] = pow(x, gamma);
    }

    glTexImage1D(GL_TEXTURE_1D, 0, GL_LUMINANCE, size, 0, GL_LUMINANCE, GL_FLOAT, img); CHECKGL;

    return texid;
}

static void setupFixedPBuffer(PBuffer *pb, int w, int h, bool useFloat)
{
    PBufferFlags flags = (rdr_caps.supports_pixel_format_float&&useFloat)?PB_FLOAT:0;
    flags |= (rdr_caps.supports_fbos?PB_FBO:0)|PB_ALPHA|PB_RENDER_TEXTURE|PB_COLOR_TEXTURE;
    pbufInitDirect2(pb,w,h, flags,1,1,0,0,0);
    pbufMakeCurrentDirect(pb);
    glDisable(GL_DEPTH_TEST); CHECKGL;
    setOrthoProjection(w, h);
    rdrClearScreenDirect();
}

static void setupPBuffers(PBuffer *pbFrameBuffer)
{
    int w, h;
    int blur_w, blur_h;
    bool bUseFloatBuffers = !!(rdr_caps.features&GFXF_FPRENDER);
    w = pbFrameBuffer->width;
    h = pbFrameBuffer->height;

    rdrBeginMarker(__FUNCTION__);
    
    blur_w = (pbFrameBuffer->width+(blur_scale-1)) / blur_scale;
    blur_h = (pbFrameBuffer->height+(blur_scale-1))/ blur_scale;
    if (pbBlurBuffer.width != blur_w || pbBlurBuffer.height != blur_h || pbBlurBuffer.isFloat!=bUseFloatBuffers || pb1x1.isFloat != rdr_caps.supports_pixel_format_float)
    {
        bool reinit_fixed_size = pbBlurBuffer.isFloat!=bUseFloatBuffers || pb1x1.isFloat!=rdr_caps.supports_pixel_format_float || pb1x1.width==0;
        PBufferFlags flags = (bUseFloatBuffers?PB_FLOAT:0)|(rdr_caps.supports_fbos?PB_FBO:0)|PB_ALPHA|PB_RENDER_TEXTURE|PB_COLOR_TEXTURE;
        pbufInitDirect2(&pbBlurBuffer,blur_w, blur_h,flags,1,1,0,0,0);
        pbufMakeCurrentDirect(&pbBlurBuffer);
        glDisable(GL_DEPTH_TEST); CHECKGL;
        setOrthoProjection(blur_w, blur_h);
        rdrClearScreenDirect();

        pbufInitDirect2(&pbBlurBuffer2,blur_w, blur_h,flags,1,1,0,0,0);
        pbufMakeCurrentDirect(&pbBlurBuffer2);
        glDisable(GL_DEPTH_TEST); CHECKGL;
        setOrthoProjection(blur_w, blur_h);
        rdrClearScreenDirect();

        pbufInitDirect2(&pbBrightPass,blur_w, blur_h,(rdr_caps.supports_fbos?PB_FBO:0)|PB_ALPHA|PB_RENDER_TEXTURE|PB_COLOR_TEXTURE,1,1,0,0,0);
        pbufMakeCurrentDirect(&pbBrightPass);
        glDisable(GL_DEPTH_TEST); CHECKGL;
        setOrthoProjection(blur_w, blur_h);
        rdrClearScreenDirect();

        if (reinit_fixed_size)
        {
            setupFixedPBuffer(&pb64x64, 64, 64, 0);
            setupFixedPBuffer(&pb16x16, 16, 16, 0);
            setupFixedPBuffer(&pb4x4, 4, 4, 0);
            // Changed the following 1x1 textures to 2x2 since 1x1 doesn't seem to work with FBOs properly
            // If the 1x1 problem is resolved, also remove shrink4_1x1()
            setupFixedPBuffer(&pb1x1, 2, 2, 1);
            setupFixedPBuffer(&pb1x1LightA, 2, 2, 1);
            setupFixedPBuffer(&pb1x1LightB, 2, 2, 1);
        }

        winMakeCurrentDirect();
    }

    // set/reset orthographic to full frame buffer size
    setOrthoProjection(w, h);

    if (!gamma_tex) {
        gamma_tex = createGammaTexture(256, 1.0 / 2.2);
    }

    rdrEndMarker();
}

// w, h = screen coords (0... 1000, etc),
// tw, th = tex coords of bound texture (0..1)
static void drawQuad(int w, int h, float tw, float th, bool offset)
{
    float blur_tw, blur_th;
    float ox = 0.0, oy = 0.0;
    rdrBeginMarker(__FUNCTION__);

//    tw = 1.0;
//    th = 1.0;
    blur_tw = 1.0;
    blur_th = 1.0;
    if (offset) {
        // offset by half a texel
        ox = 0.5 / w;
        oy = 0.5 / h;
    }
    WCW_DepthMask(0);
    WCW_PrepareToDraw();
    glBegin(GL_QUADS);
    glTexCoord2f(0+ox, th+oy);  glMultiTexCoord2fARB(GL_TEXTURE1, 0, blur_th);
    glVertex3f(0, h, 0);
    glTexCoord2f(tw+ox, th+oy); glMultiTexCoord2fARB(GL_TEXTURE1, blur_tw, blur_th);
    glVertex3f(w, h, 0);
    glTexCoord2f(tw+ox, 0+oy);  glMultiTexCoord2fARB(GL_TEXTURE1, blur_tw, 0);
    glVertex3f(w, 0, 0);
    glTexCoord2f(0+ox, 0+oy);   glMultiTexCoord2fARB(GL_TEXTURE1, 0, 0);
    glVertex3f(0, 0, 0);
    glEnd(); CHECKGL;

    rdrEndMarker();
}


static void drawQuadPB(PBuffer *pb) // Draws a quad with the proportions and coordinates of a given PBuffer
{
    drawQuad(pb->width, pb->height, pb->width/(F32)pb->virtual_width, pb->height/(F32)pb->virtual_height, 0);
}

static void glow(PBuffer *pbFrameBuffer)
{
    // Scheme based on nVidia's demo
    int blur_w, blur_h;
    Vec4 texTransform = {0.5f/pbFrameBuffer->virtual_width, 0.5f/pbFrameBuffer->virtual_height, -0.5f/pbFrameBuffer->virtual_width, -0.5f/pbFrameBuffer->virtual_height};
    Vec4 texTransform2 = {1.f/pbBlurBuffer.virtual_width, 1.f/pbBlurBuffer.virtual_height, 0, 0};
    blur_w = (pbFrameBuffer->width+(blur_scale-1)) / blur_scale;
    blur_h = (pbFrameBuffer->height+(blur_scale-1))/ blur_scale;

    rdrBeginMarker(__FUNCTION__);

    // Shrink pbFrameBuffer -> pbBlurBuffer2
    pbufMakeCurrentDirect(&pbBlurBuffer2);
    glClearColor(0, 0, 0, 0); CHECKGL;
    rdrClearScreenDirect();
    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[SHADER_SHRINK_EXTEND]);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_TextTransformFP, texTransform);

    pbufBindDirect(pbFrameBuffer);
    drawQuad(blur_w, blur_h, pbFrameBuffer->width/(F32)pbFrameBuffer->virtual_width, pbFrameBuffer->height/(F32)pbFrameBuffer->virtual_height, false);
    pbufReleaseDirect(pbFrameBuffer);

    saveThumbnailPB(&pbBlurBuffer2, "glow1");

#if 1
    // horizontal blur (pbBlurBuffer2 -> pbBlurBuffer)
    pbufMakeCurrentDirect(&pbBlurBuffer);
    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[SHADER_HBLUR]);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_TextTransformFP, texTransform2);
    pbufBindDirect(&pbBlurBuffer2);
    drawQuad(blur_w, blur_h, pbBlurBuffer2.width/(F32)pbBlurBuffer2.virtual_width, pbBlurBuffer2.height/(F32)pbBlurBuffer2.virtual_height, false);
    pbufReleaseDirect(&pbBlurBuffer2);

    saveThumbnailPB(&pbBlurBuffer, "glow2");

#if 1
    // vertical blur (pbBlurBuffer -> pbBlurBuffer2)
    pbufMakeCurrentDirect(&pbBlurBuffer2);
    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[SHADER_VBLUR]);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_TextTransformFP, texTransform2);
    pbufBindDirect(&pbBlurBuffer);
    drawQuad(blur_w, blur_h, pbBlurBuffer.width/(F32)pbBlurBuffer.virtual_width, pbBlurBuffer.height/(F32)pbBlurBuffer.virtual_height, false);
    pbufReleaseDirect(&pbBlurBuffer);

    saveThumbnailPB(&pbBlurBuffer2, "glow3");

#endif
#endif
    winMakeCurrentDirect();
    texSetWhite(0);
    texSetWhite(1);

    rdrEndMarker();
}



void toneMappingPass(PBuffer *pbFrameBuffer)
{
    Vec4 param;

    rdrBeginMarker(__FUNCTION__);
    winMakeCurrentDirect();
    rdrSetup2DRenderingDirect();

    // bind textures
    pbufBindDirect2(pbFrameBuffer, 0);
    pbufBindDirect2(&pbBlurBuffer2, 1);

    WCW_BindTexture(GL_TEXTURE_1D, 2, gamma_tex);

    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[SHADER_TONEMAP]);
    param[0] = blur_amount;
    param[1] = exposure;
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_BlurAmtAndExposureFP, param);
    drawQuadPB(pbFrameBuffer);

    pbufReleaseDirect2(&pbBlurBuffer2, 1);
    pbufReleaseDirect2(pbFrameBuffer, 0);
    rdrFinish2DRenderingDirect();
    rdrEndMarker();
}


void shrink(PBuffer *pbFrameBuffer, PBuffer *blurred)
{
    int blur_w, blur_h;
    Vec4 texTransform = {0.5f/pbFrameBuffer->virtual_width, 0.5f/pbFrameBuffer->virtual_height, -0.5f/pbFrameBuffer->virtual_width, -0.5f/pbFrameBuffer->virtual_height};
    //Vec4 texTransform2 = {1.f/pbBlurBuffer.virtual_width, 1.f/pbBlurBuffer.virtual_height, 0, 0};
    blur_w = (pbFrameBuffer->width+(blur_scale-1)) / blur_scale;
    blur_h = (pbFrameBuffer->height+(blur_scale-1))/ blur_scale;

    rdrBeginMarker(__FUNCTION__);
    // Shrink pbFrameBuffer -> pbBlurBuffer2
    pbufMakeCurrentDirect(blurred);
    glClearColor(0, 0, 0, 0); CHECKGL;
    rdrClearScreenDirect();
    WCW_EnableFragmentProgram();
//    WCW_BindFragmentProgram(shaderEffectsPrograms[blur_scale==4?SHADER_SHRINK4:SHADER_SHRINK2]);
    WCW_BindFragmentProgram(shaderEffectsPrograms[SHADER_SHRINK2]); // *much* faster to do 4 samples on a shrink by 4 (actually slower to shrink by 4 accurately than shrink by 2 accurately for some reason)
    // Scaling down by 2 twice is exactly the same spped as by 4 once.
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_TextTransformFP, texTransform);
    WCW_PrepareToDraw();

    pbufBindDirect(pbFrameBuffer);
    drawQuad(blur_w, blur_h, pbFrameBuffer->width/(F32)pbFrameBuffer->virtual_width, pbFrameBuffer->height/(F32)pbFrameBuffer->virtual_height, false);
    pbufReleaseDirect(pbFrameBuffer);

    saveThumbnailPB(blurred, "shrink");
    rdrEndMarker();
}

void shrink4(PBuffer *pbSource, PBuffer *pbDest, EffectShader shader)
{
    // Needs to be 1/2 of texel in the source texture
    Vec4 texTransform = {0.5f/pbSource->virtual_width, 0.5f/pbSource->virtual_height, -0.5f/pbSource->virtual_width, -0.5f/pbSource->virtual_height};
    rdrBeginMarker(__FUNCTION__);
    pbufMakeCurrentDirect(pbDest);
    glClearColor(0, 0, 0, 0); CHECKGL;
    rdrClearScreenDirect();
    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[shader]);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_TextTransformFP, texTransform);

    pbufBindDirect(pbSource);
    drawQuad(pbDest->width, pbDest->height, pbSource->width/(F32)pbSource->virtual_width, pbSource->height/(F32)pbSource->virtual_height, false);
    pbufReleaseDirect(pbSource);
    rdrEndMarker();
}

// Fake going down to a 1x1 texture by using a 2x2 texture and
// force all of the texels the same by only using one texture coordinage (the middle)
void shrink4_1x1(PBuffer *pbSource, PBuffer *pbDest, EffectShader shader)
{
    // Needs to be 1/2 of texel in the source texture
    Vec4 texTransform = {0.5f/pbSource->virtual_width, 0.5f/pbSource->virtual_height, -0.5f/pbSource->virtual_width, -0.5f/pbSource->virtual_height};
    rdrBeginMarker(__FUNCTION__);
    pbufMakeCurrentDirect(pbDest);
    glClearColor(0.5, 0.5, 0.5, 0); CHECKGL;
    rdrClearScreenDirect();
    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[shader]);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_TextTransformFP, texTransform);
    WCW_PrepareToDraw();

    pbufBindDirect(pbSource);
    //drawQuad(pbDest->width, pbDest->height, pbSource->width/(F32)pbSource->virtual_width, pbSource->height/(F32)pbSource->virtual_height, false);
    //static void drawQuad(int w, int h, float tw, float th, bool offset)
    {
        float w = pbDest->width;
        float h = pbDest->height;
        float tw = pbSource->width/(F32)pbSource->virtual_width * 0.5;
        float th = pbSource->height/(F32)pbSource->virtual_height * 0.5;
        WCW_DepthMask(0);
        glBegin(GL_QUADS);
        glTexCoord2f(tw, th); glVertex3f(0, h, 0);
        glTexCoord2f(tw, th); glVertex3f(w, h, 0);
        glTexCoord2f(tw, th); glVertex3f(w, 0, 0);
        glTexCoord2f(tw, th); glVertex3f(0, 0, 0);
        glEnd(); CHECKGL;
    }
    pbufReleaseDirect(pbSource);
    rdrEndMarker();
}
    
void rdrAddTexturesDirect(RdrAddTexturesParams *params)
{
    float s, t, s1, t1, s2, t2;

    rdrBeginMarker(__FUNCTION__);
    rdrClearScreenDirectEx(CLEAR_DEFAULT|CLEAR_ALPHA);

    rdrSetup2DRenderingDirect();
    WCW_BlendFuncPush(GL_ONE, GL_ONE);

    //////////////////////////////////////////////////////////////////////////

    texBind(TEXLAYER_BASE, params->tex1_handle);

    WCW_ActiveTexture(TEXLAYER_BASE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECKGL;
    WCW_Color4(255, 255, 255, 255);

    s = params->tex1_width / (float)params->tex1_texwidth;
    t = params->tex1_height / (float)params->tex1_texheight;

    glBegin( GL_QUADS );
    glTexCoord2f( 0, t );
    glVertex2f( params->tex1_uoffset, params->tex1_voffset + params->tex1_height );
    glTexCoord2f( s, t );
    glVertex2f( params->tex1_uoffset + params->tex1_width, params->tex1_voffset + params->tex1_height );
    glTexCoord2f( s, 0 );
    glVertex2f( params->tex1_uoffset + params->tex1_width, params->tex1_voffset );
    glTexCoord2f( 0, 0 );
    glVertex2f( params->tex1_uoffset, params->tex1_voffset );
    glEnd(); CHECKGL;

    //////////////////////////////////////////////////////////////////////////

    texBind(TEXLAYER_BASE, params->tex2_handle);

    WCW_ActiveTexture(TEXLAYER_BASE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECKGL;
    WCW_Color4(255, 255, 255, 255);

    s1 = t1 = 0;
    s2 = t2 = 1;

    s = params->tex2_width / (float)params->tex2_texwidth;
    t = params->tex2_height / (float)params->tex2_texheight;

    glBegin( GL_QUADS );
    glTexCoord2f( 0, t ); glMultiTexCoord2fARB(GL_TEXTURE1, s1, t2);
    glVertex2f( params->tex2_uoffset, params->tex2_voffset + params->tex2_height );
    glTexCoord2f( s, t ); glMultiTexCoord2fARB(GL_TEXTURE1, s2, t2);
    glVertex2f( params->tex2_uoffset + params->tex2_width, params->tex2_voffset + params->tex2_height );
    glTexCoord2f( s, 0 ); glMultiTexCoord2fARB(GL_TEXTURE1, s2, t1);
    glVertex2f( params->tex2_uoffset + params->tex2_width, params->tex2_voffset );
    glTexCoord2f( 0, 0 ); glMultiTexCoord2fARB(GL_TEXTURE1, s1, t1);
    glVertex2f( params->tex2_uoffset, params->tex2_voffset );
    glEnd(); CHECKGL;

    //////////////////////////////////////////////////////////////////////////

    WCW_BlendFuncPop();
    rdrFinish2DRenderingDirect();
    rdrEndMarker();
}

void rdrMulTexturesDirect(RdrAddTexturesParams *params)
{
    float s1, t1, s2, t2;

    rdrBeginMarker(__FUNCTION__);
    rdrClearScreenDirectEx(CLEAR_DEFAULT|CLEAR_ALPHA);

    rdrSetup2DRenderingDirect();
    WCW_BlendFuncPush(GL_ONE, GL_ZERO);

    //////////////////////////////////////////////////////////////////////////

    texBind(TEXLAYER_BASE, params->tex1_handle);

    WCW_ActiveTexture(TEXLAYER_BASE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECKGL;
    WCW_Color4(255, 255, 255, 255); CHECKGL;

    s1 = params->tex1_uoffset / (float)params->tex1_texwidth;
    t1 = params->tex1_voffset / (float)params->tex1_texheight;
    s2 = (params->tex1_uoffset + params->tex1_width) / (float)params->tex1_texwidth;
    t2 = (params->tex1_voffset + params->tex1_height) / (float)params->tex1_texheight;

    glBegin( GL_QUADS );
    glTexCoord2f( s1, t2 );
    glVertex2f( 0, params->tex1_height );
    glTexCoord2f( s2, t2 );
    glVertex2f( params->tex1_width, params->tex1_height );
    glTexCoord2f( s2, t1 );
    glVertex2f( params->tex1_width, 0 );
    glTexCoord2f( s1, t1 );
    glVertex2f( 0, 0 );
    glEnd(); CHECKGL;

    //////////////////////////////////////////////////////////////////////////

    WCW_BlendFuncPush(GL_DST_COLOR, GL_ZERO);

    texBind(TEXLAYER_BASE, params->tex2_handle);

    WCW_ActiveTexture(TEXLAYER_BASE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECKGL;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECKGL;
    WCW_Color4(255, 255, 255, 255);

    s1 = params->tex2_uoffset / (float)params->tex2_texwidth;
    t1 = params->tex2_voffset / (float)params->tex2_texheight;
    s2 = (params->tex2_uoffset + params->tex2_width) / (float)params->tex2_texwidth;
    t2 = (params->tex2_voffset + params->tex2_height) / (float)params->tex2_texheight;

    glBegin( GL_QUADS );
    glTexCoord2f( s1, t2 );
    glVertex2f( 0, params->tex2_height );
    glTexCoord2f( s2, t2 );
    glVertex2f( params->tex2_width, params->tex2_height );
    glTexCoord2f( s2, t1 );
    glVertex2f( params->tex2_width, 0 );
    glTexCoord2f( s1, t1 );
    glVertex2f( 0, 0 );
    glEnd(); CHECKGL;

    WCW_BlendFuncPop();

    //////////////////////////////////////////////////////////////////////////

    WCW_BlendFuncPop();
    rdrFinish2DRenderingDirect();
    rdrEndMarker();
}

void sampleLog(PBuffer *pbSrc)
{
    // Shrink the visible area into a 64x64 texture
    int w=64, h=64;
    Vec4 texTransform = {((pbSrc->width/(F32)w)/pbSrc->virtual_width)/8.f, ((pbSrc->height/(F32)w)/pbSrc->virtual_height)/8.f};
    bool useLogSampling=false;
    
    rdrBeginMarker(__FUNCTION__);
    // Sample log of from pbSrc -> pb64
    pbufMakeCurrentDirect(&pb64x64);
    glClearColor(0, 0, 0, 0); CHECKGL;
    rdrClearScreenDirect();
    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[useLogSampling?SHADER_LOG:SHADER_SHRINK4LUM]);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_TextTransformFP, texTransform);

    pbufBindDirect(pbSrc);
    drawQuad(w, h, pbSrc->width/(F32)pbSrc->virtual_width, pbSrc->height/(F32)pbSrc->virtual_height, false);
    pbufReleaseDirect(pbSrc);
    saveThumbnailPB(&pb64x64, "sampleLog1");

    shrink4(&pb64x64, &pb16x16, SHADER_SHRINK4);
    saveThumbnailPB(&pb16x16, "sampleLog2");
    shrink4(&pb16x16, &pb4x4, SHADER_SHRINK4);
    saveThumbnailPB(&pb4x4, "sampleLog3");
    //shrink4(&pb4x4, &pb1x1, useLogSampling?SHADER_SHRINK4EXP:SHADER_SHRINK4);
    shrink4_1x1(&pb4x4, &pb1x1, useLogSampling?SHADER_SHRINK4EXP:SHADER_SHRINK4);
    saveThumbnailPB(&pb1x1, "sampleLog4");
    rdrEndMarker();
}    

void lightAdaptation(PBuffer *pbSrc)
{
    Vec4 timestepParam = {TIMESTEP};
    PBuffer *temp;
    // Render into the spare, then swap
    rdrBeginMarker(__FUNCTION__);

    pbufMakeCurrentDirect(pb1x1LightAdaptationSpare);

    pbufBindDirect2(pbSrc, 0);
    pbufBindDirect2(pb1x1LightAdaptation, 1);
    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[SHADER_LIGHTADAPTATION]);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_TimeStepFP, timestepParam);
    WCW_PrepareToDraw();
    drawQuadPB(pb1x1LightAdaptationSpare);
    pbufReleaseDirect2(pb1x1LightAdaptation, 1);
    pbufReleaseDirect2(pbSrc, 0);

    temp = pb1x1LightAdaptation;
    pb1x1LightAdaptation = pb1x1LightAdaptationSpare;
    pb1x1LightAdaptationSpare = temp;

    saveThumbnailPB(pb1x1LightAdaptationSpare, "lightAdapt");
    rdrEndMarker();
}

static void calcExpectedLumParam(Vec4 lum)
{
//    Vec3 luminance_vector = {0.2125, 0.7154, 0.0721};
//    lum[0] = dotVec3(luminance_vector, rdr_frame_state.sun_ambient) +
//        dotVec3(luminance_vector, rdr_frame_state.sun_diffuse);
    lum[0] = rdr_view_state.luminance*0.25;
    // Mapping requirements:
    // 0.4 -> 0.50
    // 0.16 -> 0.18
    // < 0 -> 0.1
    // > 0.5 -> 0.7
    lum[0] -= 0.22;
    lum[0] *= (0.50-0.18) / (0.4-0.16);
    lum[0] += 0.18;
    lum[0] = MIN(MAX(lum[0], 0.1), 0.7);
    lum[1] = rdr_view_state.toneMapWeight; // % to tonemap
    lum[2] = rdr_view_state.bloomWeight; // % to bloom
    lum[3] = 1; // unused
}

void brightPass(PBuffer *pbBuf, PBuffer *pbDest)
{
    Vec4 expectedLum;
    // pbBlurBuffer2 has a 1/4th size image in it

    rdrBeginMarker(__FUNCTION__);
    // Bright pass into pbBlurBuffer
    pbufMakeCurrentDirect(pbDest);

    // bind textures
    pbufBindDirect2(pbBuf, 0);
    pbufBindDirect2(pb1x1LightAdaptation, 1);

    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[SHADER_BRIGHTPASS]);
    calcExpectedLumParam(expectedLum);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_ExpectedLumFP, expectedLum);
    WCW_PrepareToDraw();
    drawQuadPB(pbBuf);
    pbufReleaseDirect2(pb1x1LightAdaptation, 1);
    pbufReleaseDirect2(pbBuf, 0);

    saveThumbnailPB(pbDest, "brightPass");
    rdrEndMarker();
}

void doBlur(PBuffer *pbBuf, PBuffer *pbTemp, PBuffer *pbFinal)
{
    Vec4 texTransform2 = {1.f/pbBuf->virtual_width, 1.f/pbBuf->virtual_height, 0, 0};

    rdrBeginMarker(__FUNCTION__);
    // horizontal blur (pbBuf -> pbTemp)
    pbufMakeCurrentDirect(pbTemp);
    WCW_BlendFuncPush(GL_ONE, GL_ZERO);
    glDisable(GL_ALPHA_TEST); CHECKGL;
    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[SHADER_HBLUR]);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_TextTransformFP, texTransform2);
    pbufBindDirect(pbBuf);
    drawQuadPB(pbBuf);
    pbufReleaseDirect(pbBuf);
    glEnable(GL_ALPHA_TEST); CHECKGL;
    WCW_BlendFuncPop();

//    saveThumbnailPB(pbTemp, "doBlur0");

    // vertical blur (pbTemp -> pbFinal)
    pbufMakeCurrentDirect(pbFinal);
    WCW_BlendFuncPush(GL_ONE, GL_ZERO);
    glDisable(GL_ALPHA_TEST); CHECKGL;
    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[SHADER_VBLUR]);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_TextTransformFP, texTransform2);
    pbufBindDirect(pbTemp);
    drawQuadPB(pbTemp);
    pbufReleaseDirect(pbTemp);
    glEnable(GL_ALPHA_TEST); CHECKGL;
    WCW_BlendFuncPop();

    saveThumbnailPB(pbFinal, "doBlur1");
    rdrEndMarker();
}

void doDOFBlur(PBuffer *pbBuf, PBuffer *pbTemp, PBuffer *pbFinal)
{
    doBlur(pbBuf, pbTemp, pbFinal);
}

static bool debug_manual_scaling=false;
void scale_nearest(unsigned char * dest, unsigned char *src, int w, int h)
{
    int x, y;
    int i=0, j;
    for (y=0; y<h; y++) {
        for (x=0; x<w; x++) {
            unsigned int *srcpix = (unsigned int *)&src[(w*y + x)*4];
            int pix = *srcpix;
            for (i=0; i<2; i++) {
                for (j=0; j<2; j++) {
                    *(int*)&dest[(w*2*(y*2+i) + x*2+j)*4] = pix;
//                    *(int*)&dest[(w*2*(y*2+1) + x*2+j)*4] = 0xff000000;
                }
            }
        }
    }
}

void scaleFB(PBuffer *pbFrameBuffer)
{
    int w, h;
    static unsigned char *pixbuf=0;
    static unsigned char *outbuf=0;
    static int pb_w, pb_h;
    rdrBeginMarker(__FUNCTION__);
    w = pbFrameBuffer->width;
    h = pbFrameBuffer->height;
    if (w*h != pb_w*pb_h) {
        SAFE_FREE(pixbuf);
        pixbuf = malloc((pb_w=w)*(pb_h=h)*4);
        SAFE_FREE(outbuf);
        outbuf = malloc(w*h*2*2*4);
    }
    WCW_DisableFragmentProgram();
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixbuf ); CHECKGL;
    // scale
    scale_nearest(outbuf, pixbuf, w, h);
    glRasterPos2i(0,0); CHECKGL;
    WCW_Color4(255, 255, 255, 255);
    glDrawPixels(w*2, h*2, GL_RGBA, GL_UNSIGNED_BYTE, outbuf); CHECKGL;
    rdrEndMarker();
}

static int dof_blur_lookup_width=256;
static int dof_blur_lookup_tex_id=0;
static F32 minDistance, maxDistance, distanceRange;

static DOFValues cached_dof_values;;

void setupDOFBlurLookup()
{
    F32 focusDistance = rdr_view_state.dofFocusDistance; // 120
    F32 focusDesired = rdr_view_state.dofFocusValue; // 0
    U8 *tex_data;
    F32 *distance_debug;
    F32 nearDofDesired = rdr_view_state.dofNearValue; // 0.5;
    F32 nearDofDistance = rdr_view_state.dofNearDist; // 0.73 (nearz)
    F32 nearDofValue;
    F32 farDofDesired = rdr_view_state.dofFarValue; // 0.8;
    F32 farDofDistance = rdr_view_state.dofFarDist; // 3000
    F32 farDofValue;
    F32 c0 = 0.007;
    F32 ratio1 = 0.5;
    F32 ratio2 = 1.0;
    int i;
    bool newValues=false;

    rdrBeginMarker(__FUNCTION__);

#define CHECKVALUE(v, v2, tol)                                    \
    if (ABS(cached_dof_values.v-rdr_view_state.v2)>tol) {        \
        newValues = true;                                        \
        cached_dof_values.v = rdr_view_state.v2;                \
    }

    CHECKVALUE(farDist, dofFarDist, 20);
    CHECKVALUE(farValue, dofFarValue, 0.01);
    CHECKVALUE(focusDistance, dofFocusDistance, 5);
    CHECKVALUE(focusValue, dofFocusValue, 0.01);
    CHECKVALUE(nearDist, dofNearDist, 0.5);
    CHECKVALUE(nearValue, dofNearValue, 0.01);
    if (!newValues && dof_blur_lookup_tex_id) {
        return;
    }

    // far must be > 0
    if (farDofDistance == 0)
        farDofDistance = focusDistance * 4;
    // far must be > focusDistance
    if (farDofDistance < focusDistance + 1)
        farDofDistance = focusDistance + 1;

    farDofDesired = MINMAX(farDofDesired, 0, 1);

    // near must be < focusDistance
    if (nearDofDistance > focusDistance - 1)
        nearDofDistance = focusDistance - 1;

    nearDofDesired = MINMAX(nearDofDesired, 0, 1);

    //maxDistance = 0.125f * rdr_frame_state.farz;
    minDistance = nearDofDistance;
    maxDistance = farDofDistance;
    // max must be > min (assured by setting near < focus and far > focus)

    distanceRange = maxDistance - minDistance;

    // Calculate farDofValue that yields farDofDesired blurring at maxDistance;
    farDofValue = farDofDesired / (focusDistance / maxDistance - 1);
    // Calculate nearDofValue that yields nearDofDesired blurring at minDistance;
    nearDofValue = nearDofDesired / (focusDistance / minDistance - 1);

    // Texture input:
    //   (depth buffer value - depthMin) / depthRange
    tex_data = _alloca(dof_blur_lookup_width*sizeof(U8));
    distance_debug = _alloca(dof_blur_lookup_width*sizeof(F32));
    for (i=0; i<dof_blur_lookup_width; i++) {
        F32 interpolator;
        F32 distance = distanceRange * i / (dof_blur_lookup_width-1) + minDistance;
        // Calculate circle of confusion value, sort of
        F32 circle;

        if (distance >= focusDistance) {
            if (0 && "log") {
                // log - more greys, fades up too quick
                farDofValue = farDofDesired / log(maxDistance - focusDistance);
                circle = log(distance - focusDistance + 1) * farDofValue;
            } else if (0 && "linear") {
                // linear - feels like you see less greys, abrupt change to 100%
                circle = (distance - focusDistance) / (maxDistance - focusDistance) * farDofDesired;
            } else {
                // "Circle of confusion" based - seems linear in screen space, looks best
                circle = (focusDistance / distance - 1.f) * farDofValue;
                circle += (1.f - circle / farDofDesired) * focusDesired;
            }
            
        } else {
            circle = (focusDistance / distance - 1.f) * nearDofValue;
            circle += (1.f - circle / nearDofDesired) * focusDesired;
        }
        circle = ABS(circle);

        if (circle <= c0)
        {
            interpolator = 0.0f;
        }
        else if (circle <= ratio1)
        {                       // interpolator is in 0.0 .. 0.5 range
            interpolator = (circle-c0)/((ratio1-c0));
            interpolator = MAX(0.0f, interpolator);
            interpolator = MIN(1.0f, interpolator);
            interpolator = 0.5f * interpolator;
        }
        else                    // interpolator is in 0.5 .. 1.0 range
        {
            interpolator = (circle-ratio1)/((ratio2-ratio1));
            interpolator = MAX(0.0f, interpolator);
            interpolator = MIN(1.0f, interpolator);
            interpolator = 0.5f * (1.0f + interpolator); 
        }
        tex_data[i] = MINMAX(round(interpolator*255), 0, 255);
        distance_debug[i] = distance;
    }
    if (!dof_blur_lookup_tex_id) {
        glGenTextures(1, &dof_blur_lookup_tex_id); CHECKGL;
    }
    WCW_BindTexture(GL_TEXTURE_1D, 0, dof_blur_lookup_tex_id);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); CHECKGL;
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); CHECKGL;
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECKGL;
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECKGL;
    glTexImage1D(GL_TEXTURE_1D, 0, GL_LUMINANCE8, dof_blur_lookup_width, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, tex_data); CHECKGL;
    rdrEndMarker();
}

void shrinkForDof(PBuffer *pbFrameBuffer, PBuffer *blurred)
{
    // This needs the DOF_INTO_BLURRED_TEXTURE shader define, and looks good on some ways,
    //   but loses objecst who are too small to get into the blurred image (like street
    //   lamps against a blurred background)
    Vec4 dof_param2;
    Vec4 dof_project = {rdr_view_state.projectionMatrix[3][3], rdr_view_state.projectionMatrix[3][2],
        rdr_view_state.projectionMatrix[2][3]*2.f, rdr_view_state.projectionMatrix[2][2]*2.f};
    int blur_w, blur_h;
    Vec4 texTransform = {0.5f/pbFrameBuffer->virtual_width, 0.5f/pbFrameBuffer->virtual_height, -0.5f/pbFrameBuffer->virtual_width, -0.5f/pbFrameBuffer->virtual_height};
    Vec4 texTransform2 = {1.f/pbBlurBuffer.virtual_width, 1.f/pbBlurBuffer.virtual_height, 0, 0};
    blur_w = (pbFrameBuffer->width+(blur_scale-1)) / blur_scale;
    blur_h = (pbFrameBuffer->height+(blur_scale-1))/ blur_scale;

    rdrBeginMarker(__FUNCTION__);

    setupDOFBlurLookup();

    dof_param2[0] = minDistance;
    dof_param2[1] = maxDistance;
    dof_param2[2] = 1.f/distanceRange;

    // Shrink pbFrameBuffer -> pbBlurBuffer2
    pbufMakeCurrentDirect(blurred);
    glClearColor(0, 0, 0, 0); CHECKGL;
    rdrClearScreenDirect();
    WCW_BlendFuncPush(GL_ONE, GL_ZERO);
    glDisable(GL_ALPHA_TEST); CHECKGL;
    WCW_EnableFragmentProgram();
    WCW_BindFragmentProgram(shaderEffectsPrograms[SHADER_SHRINK2DOF]);

    pbufBindDirect(pbFrameBuffer);
    /*
    if (game_state.useMRTs && pbFrameBuffer->num_aux_buffers) {
        pbufBindAuxDirect(pbFrameBuffer, 3, WGL_AUX0_ARB);
    } else {
        pbufBindDepthDirect(pbFrameBuffer, 3);
    }
    WCW_BindTexture(GL_TEXTURE_1D, 4, dof_blur_lookup_tex_id);
    */
    // Parameters
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_TextTransformFP, texTransform);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_DofParam2FP, dof_param2);
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_DofProjectFP, dof_project);
    drawQuad(blur_w, blur_h, pbFrameBuffer->width/(F32)pbFrameBuffer->virtual_width, pbFrameBuffer->height/(F32)pbFrameBuffer->virtual_height, false);

    /*
    if (game_state.useMRTs && pbFrameBuffer->num_aux_buffers) {
        pbufReleaseAuxDirect(pbFrameBuffer, 3, WGL_AUX0_ARB);
    } else {
        pbufReleaseDepthDirect(pbFrameBuffer, 3);
    }
    */
    pbufReleaseDirect(pbFrameBuffer);

    glEnable(GL_ALPHA_TEST); CHECKGL;
    WCW_BlendFuncPop();
    saveThumbnailPB(blurred, "shrinkForDOF");
    rdrEndMarker();
}


void finalPass(PBuffer *pbFrameBuffer, PBuffer *pbBrightPass, bool doBloom, bool doDOF, bool doDesaturate)
{
    Vec4 dof_param2;
    Vec4 dof_project = {rdr_view_state.projectionMatrix[3][3], rdr_view_state.projectionMatrix[3][2],
        rdr_view_state.projectionMatrix[2][3]*2.f, rdr_view_state.projectionMatrix[2][2]*2.f};
    Vec4 expectedLum;
    Vec4 desatureate_param = {rdr_view_state.desaturateWeight, 0, 0, 0};

    rdrBeginMarker(__FUNCTION__);
    if (doDOF) {
        // DOF parameter setup
        setupDOFBlurLookup();

        dof_param2[0] = minDistance;
        dof_param2[1] = maxDistance;
        dof_param2[2] = 1.f/distanceRange;
    }
    if (doBloom) {
        calcExpectedLumParam(expectedLum);
    }

    winMakeCurrentDirect();
    rdrClearScreenDirect();
    rdrSetup2DRenderingDirect();

    // bind textures
    pbufBindDirect2(pbFrameBuffer, 0);
    rdrSetRenderScaleFilter();
    if (doBloom) {
        pbufBindDirect2(pb1x1LightAdaptation, 1);
    }
    pbufBindDirect2(pbBrightPass, 2);
    if (doDOF) {
        if (game_state.useMRTs && pbFrameBuffer->num_aux_buffers) {
            pbufBindAuxDirect(pbFrameBuffer, 3, WGL_AUX0_ARB);
        } else {
            pbufBindDepthDirect(pbFrameBuffer, 3);
        }
        WCW_BindTexture(GL_TEXTURE_1D, 4, dof_blur_lookup_tex_id);
    }

    // Bind fragment program
    WCW_EnableFragmentProgram();
    if (doBloom && doDOF) {
        WCW_BindFragmentProgram(shaderEffectsPrograms[doDesaturate?SHADER_DOF_BLOOM_FINAL_DESATURATE:SHADER_DOF_BLOOM_FINAL]);
    } else if (doBloom) {
        WCW_BindFragmentProgram(shaderEffectsPrograms[doDesaturate?SHADER_TONEMAP2_DESATURATE:SHADER_TONEMAP2]);
    } else if (doDOF) {
        WCW_BindFragmentProgram(shaderEffectsPrograms[doDesaturate?SHADER_DOF_FINAL_DESATURATE:SHADER_DOF_FINAL]);
    }
    else if(doDesaturate)
    {
        WCW_BindFragmentProgram(shaderEffectsPrograms[SHADER_SIMPLE_DESATURATE]);
    }

    if (doBloom) {
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_ExpectedLumFP, expectedLum);
    }
    if (doDOF) {
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_DofParam2FP, dof_param2);
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_DofProjectFP, dof_project);
    }
    if (doDesaturate) {
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Effects_DesaturateParamFP, desatureate_param);
    }
    if (debug_manual_scaling) {
        drawQuadPB(pbFrameBuffer);
    } else {
        drawQuad(rdr_view_state.width, rdr_view_state.height, pbFrameBuffer->width/(F32)pbFrameBuffer->virtual_width, pbFrameBuffer->height/(F32)pbFrameBuffer->virtual_height, 0);
    }

    // Release textures
    if (doDOF) {
        if (game_state.useMRTs && pbFrameBuffer->num_aux_buffers) {
            pbufReleaseAuxDirect(pbFrameBuffer, 3, WGL_AUX0_ARB);
        } else {
            pbufReleaseDepthDirect(pbFrameBuffer, 3);
        }
    }
    pbufReleaseDirect2(pbBrightPass, 2);
    if (doBloom) {
        pbufReleaseDirect2(pb1x1LightAdaptation, 1);
    }
    pbufReleaseDirect2(pbFrameBuffer, 0);

    if (debug_manual_scaling) {
        scaleFB(pbFrameBuffer);
    }

    rdrFinish2DRenderingDirect();
    rdrEndMarker();
}



void rdrDebugForceStall()
{
    int data;
    rdrBeginMarker(__FUNCTION__);
    glFlush(); CHECKGL;
    glReadPixels(0, 0, 1, 1, GL_RGBA,GL_UNSIGNED_BYTE,&data); CHECKGL;
    rdrEndMarker();
}


void rdrPostprocessingDirect(PBuffer *pbFrameBuffer)
{
    bool doBloom=!!(rdr_caps.features&GFXF_BLOOM);
    bool doDOF=!!(rdr_caps.features&GFXF_DOF);
    bool doDesaturate=!!(rdr_caps.features&GFXF_DESATURATE);

    rdrBeginMarker(__FUNCTION__);

    if (rdr_view_state.bloomWeight <= 0 &&
        rdr_view_state.toneMapWeight <= 0)
        doBloom = false;
    if (rdr_view_state.dofFarValue <= 0 &&
        rdr_view_state.dofNearValue <= 0 &&
        rdr_view_state.dofFocusValue <=0)
        doDOF = false;
    if (nearSameF32(rdr_view_state.desaturateWeight, 0))
        doDesaturate = false;

    if (!doBloom && !doDOF && !doDesaturate) {
        rdrRenderScaledDirect(pbFrameBuffer);
        rdrEndMarker();
        return;
    }

    if (rdr_view_state.bloomScale<3)
        blur_scale = 2;
    else
        blur_scale = 4;

    if (thumbnail_index > 1)
        thumbnail_index = 0;
    if (game_state.hdrThumbnail) {
        PERFINFO_AUTO_START("saveThumbnail",1);
        saveThumbnailColorDepth(pbFrameBuffer, "Color", "Depth");
        PERFINFO_AUTO_STOP();
    }

    PERFINFO_RUN(
        PERFINFO_AUTO_START("rdrDebugForceStall", 1)
        rdrDebugForceStall();
        PERFINFO_AUTO_STOP();
    );
    if (0 && "nVidia demo") 
    {
        blur_scale = 2;
        setupPBuffers(pbFrameBuffer);
        glow(pbFrameBuffer);
        toneMappingPass(pbFrameBuffer);
    } else if ("microsoft demo") {
        rdrSetup2DRenderingDirect();

        PERFINFO_AUTO_START("setupPBuffers",1);
        // Scheme based on MS's paper and then a lot of tweaking
//        blur_scale = 2;
        setupPBuffers(pbFrameBuffer);
        PERFINFO_AUTO_STOP_START("shrink",1);
//        if (!game_state.test1)
        if (doBloom || doDOF)
            shrink(pbFrameBuffer, &pbBlurBuffer2);
        PERFINFO_AUTO_STOP_START("sampleLog",1);
//        if (!game_state.test2)
        if (doBloom)
            sampleLog(&pbBlurBuffer2); // Computes average luminance // Needs FLOAT
        PERFINFO_AUTO_STOP_START("lightAdaptation",1);
//        if (!game_state.test3)
        if (doBloom)
            lightAdaptation(&pb1x1);  // Needs FLOAT
//        PERFINFO_AUTO_STOP_START("brightPass",1);
//        if (!game_state.test4)
//        if (doBloom)
//            brightPass(&pbBlurBuffer2, &pbBlurBuffer); // Want FLOAT
        PERFINFO_AUTO_STOP_START("doBlur",1);
//        if (!game_state.test5)
        if (doBloom) // or doing both
            doBlur(&pbBlurBuffer2, &pbBlurBuffer, &pbBrightPass);  // Final needs BYTE (for filtering)
        else if (doDOF)
            doDOFBlur(&pbBlurBuffer2, &pbBlurBuffer, &pbBrightPass);  // Final needs BYTE (for filtering)
        PERFINFO_AUTO_STOP_START("finalPass",1);
//        if (!game_state.test6)
            finalPass(pbFrameBuffer, &pbBrightPass, doBloom, doDOF, doDesaturate);
        PERFINFO_AUTO_STOP();

        rdrFinish2DRenderingDirect();
    }
    PERFINFO_RUN(
        PERFINFO_AUTO_START("rdrDebugForceStall", 1)
        rdrDebugForceStall();
        PERFINFO_AUTO_STOP();
    );
    rdrEndMarker();
}

void rdrSunFlareUpdateDirect(RdrSunFlareParams * params)
{
    static GLuint queries[MAX_FRAMES][2] = {0};
    static bool queryInProgress[MAX_FRAMES] = {0,};
    static int nextQuery = 0;
    int allPixels = 0;
    int visiblePixels = 0;
    int i;

    rdrBeginMarker(__FUNCTION__);
    assert(rdr_caps.supports_occlusion_query);

    if(!queries[0][0]) {
        glGenQueriesARB(sizeof(queries)/sizeof(GLuint), (GLuint*)queries); CHECKGL;
    }

    // Average all the completed results together
    for(i=0; i<MAX_FRAMES; i++) {
        GLuint queryFinished = 0;

        if(!queryInProgress[i]) {
            continue;
        }

        if(i != nextQuery) {
            glGetQueryObjectuivARB(queries[i][0], GL_QUERY_RESULT_AVAILABLE_ARB, &queryFinished); CHECKGL;
        } else {
            queryFinished = 1;
        }

        if(queryFinished) {
            int myAllPixels, myVisiblePixels;

            glGetQueryObjectuivARB(queries[i][0], GL_QUERY_RESULT_ARB, &myAllPixels); CHECKGL;
            glGetQueryObjectuivARB(queries[i][1], GL_QUERY_RESULT_ARB, &myVisiblePixels); CHECKGL;

            //assert(myAllPixels >= myVisiblePixels);
            //assert(allPixels <= visiblePixels);
            allPixels += myAllPixels;
            visiblePixels += myVisiblePixels;

            queryInProgress[i] = 0;
        }
    }
    if(allPixels) {
        *(params->visibility) = (float)visiblePixels / (float)allPixels;
    }
    else {
        *(params->visibility) = 0.0f;
    }

    assert(!queryInProgress[nextQuery]);

    WCW_LoadModelViewMatrix(params->mat);
    WCW_DisableBlend();
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); CHECKGL;
    WCW_DepthMask(GL_FALSE);

    // Submit the query with depth test off to count the maximum number of pixels
    glDisable(GL_DEPTH_TEST); CHECKGL;
    glBeginQueryARB(GL_SAMPLES_PASSED_ARB, queries[nextQuery][0]); CHECKGL;
    modelDrawSolidColor(params->vbo, 0xFFFFFFFF);
    glEndQueryARB(GL_SAMPLES_PASSED_ARB); CHECKGL;

    // Submit the depth with depth test on to count the actual number of pixels
    glEnable(GL_DEPTH_TEST); CHECKGL;
    glBeginQueryARB(GL_SAMPLES_PASSED_ARB, queries[nextQuery][1]); CHECKGL;
    modelDrawSolidColor(params->vbo, 0xFFFFFFFF);
    glEndQueryARB(GL_SAMPLES_PASSED_ARB); CHECKGL;

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); CHECKGL;
    WCW_DepthMask(GL_TRUE);
    WCW_EnableBlend();
    WCW_FetchGLState();
    WCW_ResetState();

    queryInProgress[nextQuery] = 1;
    nextQuery = (nextQuery+1) % MAX_FRAMES;
    rdrEndMarker();
}

void rdrRenderScaledDirect(PBuffer *pbFrameBuffer)
{
    rdrBeginMarker(__FUNCTION__);
    PERFINFO_AUTO_START("rdrRenderScaledDirect()", 1);
    PERFINFO_RUN(
        PERFINFO_AUTO_START("rdrDebugForceStall", 1)
        rdrDebugForceStall();
        PERFINFO_AUTO_STOP();
    );

    
//    rdrClearScreenDirect();
    rdrSetup2DRenderingDirect();

    // bind textures
    pbufBindDirect2(pbFrameBuffer, 0);
    rdrSetRenderScaleFilter();

    if ( !rt_cgGetCgShaderMode() )
        WCW_DisableFragmentProgram();
    WCW_BlendFuncPush(GL_ONE, GL_ZERO);
    WCW_PrepareToDraw();
    glDisable(GL_ALPHA_TEST); CHECKGL;
    if (debug_manual_scaling) {
        drawQuadPB(pbFrameBuffer);
    } else {
        drawQuad(rdr_view_state.width, rdr_view_state.height, pbFrameBuffer->width/(F32)pbFrameBuffer->virtual_width, pbFrameBuffer->height/(F32)pbFrameBuffer->virtual_height, 0);
    }
    glEnable(GL_ALPHA_TEST); CHECKGL;
    WCW_BlendFuncPop();
    pbufReleaseDirect2(pbFrameBuffer, 0);

    if (debug_manual_scaling) {
        scaleFB(pbFrameBuffer);
    }

    rdrFinish2DRenderingDirect();

    PERFINFO_RUN(
        PERFINFO_AUTO_START("rdrDebugForceStall", 1)
        rdrDebugForceStall();
        PERFINFO_AUTO_STOP();
    );
    PERFINFO_AUTO_STOP();
    rdrEndMarker();
}
