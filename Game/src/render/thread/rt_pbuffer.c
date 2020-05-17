#define RT_PRIVATE
#define RT_ALLOW_BINDTEXTURE
#include <utilitieslib/utils/error.h>
#include "render/thread/ogl.h"
#include "render/thread/wcw_statemgmt.h"
#include "render/renderUtil.h"
#include "render/thread/rt_state.h"
#include "render/thread/rt_pbuffer.h"
#include "render/thread/rt_tex.h"
#include "render/thread/rt_pbuffer.h"
#include <utilitieslib/utils/mathutil.h>
#include "assert.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/genericlist.h>
//#include "win/win_init.h"
#include "UI/sprite/sprite_text.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h>
#include "rt_win_init.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "graphics/failtext.h"

#define WGL_SAMPLE_BUFFERS_ARB    0x2041
#define WGL_SAMPLES_ARB        0x2042

#define PBUFFER_FALLBACK (isProductionMode())

// Change this for extra debugging
#if 0
#undef CHECKGL
#define CHECKGL rdrCheckThread(); checkgl(__FILE__,__LINE__);
#define DOING_CHECKGL
#endif

extern char *GLErrorFromInt(int v);

#ifdef DOING_CHECKGL
#define CHECKFBO(name) checkFBO(name, __FILE__, __LINE__)
#else
#define CHECKFBO(name)
#endif

FogContext main_fog_context;

PBuffer *pbuf_list=NULL;
static PBuffer * current_pbuffer;

static void checkSLIFBO(PBuffer *pbuf, const char *fname, int line)
{
    // Don't need to check if not using FBOs
    if (!(pbuf->flags & PB_FBO))
        return;

    assert(pbuf->curFBO < MAX_SLI);
    assert(pbuf->curFBO < pbuf->allocated_fbos);

    if(rdr_frame_state.numFBOs > pbuf->allocated_fbos) {
        failText("SLI count %d larger than PBUF FBO count %d from %s @ %d",
                 rdr_frame_state.numFBOs, pbuf->allocated_fbos, fname, line);
    }
}

#define CHECK_SLI_FBO(_pbuf) checkSLIFBO(_pbuf, __FILE__, __LINE__);

void pbufDeleteDirect(PBuffer **pbuf_ptr)
{
    PBuffer    *pbuf = *pbuf_ptr;
    rdrBeginMarker(__FUNCTION__ ":%p", *pbuf_ptr);

    //texSetWhite(1);
    //texSetWhite(0);
    WCW_ResetTextureState();

    if (pbuf->flags & PB_FBO) {
        if (pbuf->color_rbhandle) {
            glDeleteRenderbuffersEXT(pbuf->allocated_fbos, pbuf->color_rbhandle); CHECKGL;
        }
        if (pbuf->depth_rbhandle) {
            glDeleteRenderbuffersEXT(pbuf->allocated_fbos, pbuf->depth_rbhandle); CHECKGL;
        }
        //if (pbuf->stencil_rbhandle)
        //    glDeleteRenderbuffersEXT(pbuf->allocated_fbos, pbuf->stencil_rbhandle); CHECKGL;
        glDeleteFramebuffersEXT(pbuf->allocated_fbos, pbuf->fbo); CHECKGL;

        if (pbuf->blit_fbo[0]) {
            glDeleteFramebuffersEXT(pbuf->allocated_fbos, pbuf->blit_fbo); CHECKGL;
        }
    } else if (!pbuf->isFakedPBuffer) {
        if (!pbuf->myGLctx) {
            assert(!listInList(pbuf_list, pbuf));
            rdrEndMarker();
            return;
        }
        assert(listInList(pbuf_list, pbuf));
        listRemoveMember(pbuf, &pbuf_list);

        wglDeleteContext(pbuf->myGLctx );
        wglReleasePbufferDCARB( pbuf->buffer,pbuf->myDC );
        wglDestroyPbufferARB( pbuf->buffer );
    }
    if (pbuf->depth_handle[0]) {
        glDeleteTextures(pbuf->allocated_fbos, pbuf->depth_handle); CHECKGL;
    }
    if (pbuf->color_handle[0]) {
        glDeleteTextures(pbuf->allocated_fbos, pbuf->color_handle); CHECKGL;
    }
    if (pbuf->color_handle_aux[0]) {
        glDeleteTextures(pbuf->allocated_fbos, pbuf->color_handle_aux); CHECKGL;
    }

    if (current_pbuffer == pbuf)
        winMakeCurrentDirect();

    SAFE_FREE(pbuf->fog_context);

    memset(pbuf,0,sizeof(*pbuf)); // Must be removed from list before this is called!

    rdrEndMarker();
}

void pbufDeleteAllDirect(void)
{
    while (pbuf_list) {
        pbufDeleteDirect(&pbuf_list);
    }
}


static void setTextureParameters(int target, bool linear, int max_mip_levels)
{
    glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); CHECKGL;
    glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); CHECKGL;

    if (target == GL_TEXTURE_CUBE_MAP)
    {
        glTexParameterf(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); CHECKGL;

#if 0
        // need this?
        if (rdr_caps.supports_anisotropy) {
            glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, rdr_view_state.texAnisotropic); CHECKGL;
        }
#endif
    }

    if (linear)
    {
        glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECKGL;
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, (max_mip_levels > 0) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR); CHECKGL;
    }
    else
    {
        glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECKGL;
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECKGL;
    }

    // Provide a hint that this texture does not need mipmapping in hopes of keeping the driver
    // from allocating extra memory for mipmaps
    glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, (max_mip_levels > 0) ? max_mip_levels : 0); CHECKGL;
}


PBuffer *pbufGetCurrentDirect(void)
{
    return current_pbuffer;
}

static void updateTextureFromBuffer(int *handle, GLenum buffer, bool copyTextureShifted, int x0, int y0, int w, int h, bool depth)
{
    bool init=false;

    if (!*handle) {
        init = true;
        glGenTextures(1, handle); CHECKGL;
    }
    WCW_BindTexture(GL_TEXTURE_2D, 0, *handle);
    glReadBuffer(buffer); CHECKGL;
    if (init) {
        GLenum internalFormat = depth ? GL_DEPTH_COMPONENT : (current_pbuffer->isFloat?GL_RGBA_FLOAT16_ATI:GL_RGBA8);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, internalFormat, 0, 0, current_pbuffer->virtual_width, current_pbuffer->virtual_height, 0); CHECKGL;
    }
    if (!init || copyTextureShifted) {
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, x0, y0, x0, y0, w, h); CHECKGL;
    }
}

void pbufDoneWithCurrentDirect(void)
{
    rdrBeginMarker(__FUNCTION__ ":%p", current_pbuffer);
    // We are done rendering to this PBuffer.  If it's a fake RTT PB, copy it now!
    if (current_pbuffer)
    {
        int x0, y0, w, h, x0dest, y0dest;
        bool copyTextureShifted=false;
        x0 = y0 = x0dest = y0dest = 0;
        w = current_pbuffer->width;
        h = current_pbuffer->height;
        if (rdr_caps.copy_subimage_from_rtt_invert_hack)
        {
            if (!current_pbuffer->isFakedPBuffer && !current_pbuffer->isFakedRTT)
            {
                y0dest = y0 = current_pbuffer->virtual_height - h;
                copyTextureShifted = true;
            } else if (current_pbuffer->flags & PB_FBO)
            {
                y0 = 0;
                y0dest = current_pbuffer->virtual_height - h;
                copyTextureShifted = true;
            }
        }

        if (current_pbuffer->flags & PB_ONESHOTCOPY && current_pbuffer->copy_texture_id > -1)
        {
            winCopyFrameBufferDirect(GL_TEXTURE_2D, current_pbuffer->copy_texture_id, 0, x0, y0, w, h, 0, false, false);
            current_pbuffer->copy_texture_id = -1;
            current_pbuffer->flags &= ~PB_ONESHOTCOPY;
        }

        // Update the color and/or depth texture from the FBO render buffers
        if (current_pbuffer->flags & PB_FBO)
        {
            if ((current_pbuffer->flags & (PB_COLOR_TEXTURE|PB_DEPTH_TEXTURE)) && current_pbuffer->blit_fbo[current_pbuffer->curFBO])
            {
                CHECK_SLI_FBO(current_pbuffer);

                glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, current_pbuffer->blit_fbo[current_pbuffer->curFBO]); CHECKGL;
                glBlitFramebufferEXT(0, 0, w, h, 0, 0, w, h, current_pbuffer->blit_flags, GL_NEAREST); CHECKGL;
            }

            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); CHECKGL;
        }

        if (!(current_pbuffer->flags & PB_FBO) && current_pbuffer->isDirty)
        {
            GLenum buffer = current_pbuffer->isFakedPBuffer ? GL_BACK_LEFT : GL_FRONT_LEFT;

            if (current_pbuffer->flags & PB_DEPTH_TEXTURE)
                updateTextureFromBuffer(&current_pbuffer->depth_handle[current_pbuffer->curFBO], buffer, copyTextureShifted, x0, y0, w, h, true);

            if (current_pbuffer->isFakedRTT)
            {
                // Not using FBOs so should not have to check this
                //CHECK_SLI_FBO(current_pbuffer);

                if (current_pbuffer->flags & PB_COLOR_TEXTURE)
                    updateTextureFromBuffer(&current_pbuffer->color_handle[current_pbuffer->curFBO], buffer, copyTextureShifted, x0, y0, w, h, false);

                if (current_pbuffer->num_aux_buffers) {
                    int i;
                    for (i=0; i<current_pbuffer->num_aux_buffers; i++) {
                        updateTextureFromBuffer(&current_pbuffer->aux_handles[i], GL_AUX0+i, copyTextureShifted, x0, y0, w, h, false);
                    }
                }
            }
        }
        current_pbuffer->isDirty = false;
    }
    // Clean up some state
    WCW_BindTexture(GL_TEXTURE_2D, 0, 0);
    WCW_BindTexture(GL_TEXTURE_2D, 1, 0);
    WCW_BindTexture(GL_TEXTURE_CUBE_MAP, 0, 0);
    WCW_BindFragmentProgram(0);
    WCW_DisableFragmentProgram();
    WCW_BindVertexProgram(0);
    WCW_DisableVertexProgram();
    if (rdr_caps.use_vbos) {
        WCW_BindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
        WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, 0);
    }

    // Generate mipmaps
    if (current_pbuffer && (current_pbuffer->flags & (PB_MIPMAPPED|PB_COLOR_TEXTURE)) == (PB_MIPMAPPED|PB_COLOR_TEXTURE))
    {
        CHECK_SLI_FBO(current_pbuffer);

        if (current_pbuffer->flags & PB_CUBEMAP)
        {
            WCW_BindTexture(GL_TEXTURE_CUBE_MAP, 0, current_pbuffer->color_handle[current_pbuffer->curFBO]);
            glGenerateMipmapEXT(GL_TEXTURE_CUBE_MAP); CHECKGL;
            WCW_BindTexture(GL_TEXTURE_CUBE_MAP, 0, 0);
        }
        else
        {
            WCW_BindTexture(GL_TEXTURE_2D, 0, current_pbuffer->color_handle[current_pbuffer->curFBO]);
            glGenerateMipmapEXT(GL_TEXTURE_2D); CHECKGL;
            WCW_BindTexture(GL_TEXTURE_2D, 0, 0);
        }
    }
    rdrEndMarker();
}

void winMakeCurrentDirect()
{
    bool needViewportReset=false;
    Viewport vp;

    if (!current_pbuffer)
        return;

    rdrBeginMarker(__FUNCTION__);
    pbufDoneWithCurrentDirect();

    if (current_pbuffer && (current_pbuffer->flags & PB_FBO || current_pbuffer->isFakedPBuffer))
        needViewportReset = true;
    if (current_pbuffer && !(current_pbuffer->flags & PB_FBO) && !current_pbuffer->isFakedPBuffer)
        makeCurrentCBDirect();
    WCW_SetFogContext(&main_fog_context);
    if (rdr_view_state.override_window_size) {
        rdr_view_state.override_window_size = 0;
        rdr_view_state.width = rdr_view_state.origWidth;
        rdr_view_state.height = rdr_view_state.origHeight;
    }
    current_pbuffer = NULL;
    if (needViewportReset) {
        // Reset the viewport, if we're using FBOs it may have changed
        vp.x = 0;
        vp.y = 0;
        vp.width = rdr_view_state.width;
        vp.height = rdr_view_state.height;
        vp.bNeedScissoring = false;
        rdrViewportDirect(&vp);
    }
    rdrEndMarker();
}

static void rwglSetAppropriateDrawBuffers(PBuffer *pbuffer)
{
    if (pbuffer->num_aux_buffers) {
        int i;
        int buffers[10] = {0};
        GLenum aux_base;
        if (pbuffer->flags & PB_FBO) {
            buffers[0] = GL_COLOR_ATTACHMENT0_EXT;
            aux_base = GL_COLOR_ATTACHMENT1_EXT;
        } else {
            buffers[0] = GL_FRONT_LEFT;
            aux_base = GL_AUX0;
        }
        for (i=0; i<pbuffer->num_aux_buffers; i++) {
            buffers[1+i] = aux_base+i;
        }
        glDrawBuffersARB(pbuffer->num_aux_buffers+1, buffers); CHECKGL;
    }
}

void pbufMakeCurrentDirect(PBuffer *pbuf)
{
    if (!pbuf) {
        winMakeCurrentDirect();
        return;
    }

    if (pbuf == current_pbuffer)
        return;

    pbufDoneWithCurrentDirect();

    rdrBeginMarker(__FUNCTION__ ":%p", pbuf);
    texSetWhite(TEXLAYER_GENERIC);
    texSetWhite(TEXLAYER_BASE);
    if (pbuf->flags & PB_FBO) {
        // This will not work as intended if the current pbuffer has not been allocated with the correct number of render targets
        pbuf->curFBO = (rdr_frame_state.curFrame % MIN(rdr_frame_state.numFBOs, pbuf->allocated_fbos));

        CHECK_SLI_FBO(pbuf);

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pbuf->fbo[pbuf->curFBO]); CHECKGL;

        // if cubemap, need to bind the appropriate face
        if(pbuf->flags & PB_CUBEMAP)
        {
            // caller needs to set currentCubemapTarget before rendering each face so we know which one to set as current face
            assert(pbuf->currentCubemapTarget>=GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB && pbuf->currentCubemapTarget<=GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB);
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, pbuf->currentCubemapTarget, pbuf->color_handle[pbuf->curFBO], 0); CHECKGL;
        }

        if(rdr_frame_state.sliClear && (rdr_frame_state.curFrame != pbuf->curFrame)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); CHECKGL;
        }

        if (pbuf->fog_context)
        {
            WCW_SetFogContext(pbuf->fog_context);
        }
        else
        {
            pbuf->fog_context = calloc(1, sizeof(FogContext));
            WCW_InitFogContext(pbuf->fog_context);
        }
        if (pbuf->num_aux_buffers) {
            rwglSetAppropriateDrawBuffers(pbuf);
        }
    } else {
        if (!pbuf->isFakedPBuffer) {
            wglMakeCurrentSafeDirect( pbuf->myDC, pbuf->myGLctx, 2 );
        }
        if (pbuf->fog_context)
        {
            WCW_SetFogContext(pbuf->fog_context);
        }
        else
        {
            pbuf->fog_context = calloc(1, sizeof(FogContext));
            WCW_InitFogContext(pbuf->fog_context);
        }
    }
    if (!rdr_view_state.override_window_size) {
        rdr_view_state.override_window_size = 1;
        rdr_view_state.origWidth = rdr_view_state.width;
        rdr_view_state.origHeight = rdr_view_state.height;
        rdr_view_state.width = pbuf->width;
        rdr_view_state.height = pbuf->height;
    }
    pbuf->curFrame = rdr_frame_state.curFrame;
    pbuf->isDirty = true;
    current_pbuffer = pbuf;
    rdrEndMarker();
}

static void FatalPbufferError(char *message) {
    FatalErrorf(textStd("FatalPBufferError", message));
}

// Null-terminated arrays of pairs
static int a2cmp(int *a0, int *a1)
{
    while(*a0==*a1 && *a0!=0) {
        a0++; a1++;
        if (*a0!=*a1)
            return *a0-*a1;
        a0++; a1++;
    }
    return *a0-*a1;
}

static int a2len(int *a)
{
    int i;
    for (i=0; a[i]!=0; i+=2);
    return i;
}

static int *a2dup(int *a)
{
    int len = (a2len(a)+2)*sizeof(int);
    int *ret = malloc(len);
    memcpy(ret, a, len);
    return ret;
}


void choosePixelFormatCached(HDC hdc, int *formatAttributes, float *fattributes, int pformat_size, int *pformat, int *nformats, int multisample_level, int num_aux_buffers)
{
    static HDC cached_hdc=0;
    static int num_cached=0;
    static struct {
        int *formatAttributes;
        float *fattributes;
        int pformat;
    } cache[20];
    int i;
    if (hdc != cached_hdc) {
        for (i=0; i<num_cached; i++) {
            SAFE_FREE(cache[i].formatAttributes);
            SAFE_FREE(cache[i].fattributes);
        }
        num_cached = 0;
        cached_hdc = hdc;
    }
    for (i=0; i<num_cached; i++) {
        if (a2cmp(cache[i].formatAttributes, formatAttributes)==0 &&
            a2cmp((int*)cache[i].fattributes, (int*)fattributes)==0)
        {
            *nformats = 1;
            pformat[0] = cache[i].pformat;
            // DEBUG!
            //wglChoosePixelFormatARB( hdc, formatAttributes, fattributes, pformat_size, pformat, nformats );
            //assert(pformat[0]==cache[i].pformat);
            return;
        }
    }

    if ( !wglChoosePixelFormatARB( hdc, formatAttributes, fattributes, pformat_size, pformat, nformats ) ) {
        FatalPbufferError("pbuffer creation error:  Couldn't query pixel formats.");
    }
    // Search for closest match
    if ((multisample_level > 1  || num_aux_buffers>=1) && *nformats && wglGetPixelFormatAttribivARB) {
        int bestFormat = 0;
        int minDiff = 0x7FFFFFFF;
        int attribs[] = {WGL_SAMPLES_ARB, WGL_AUX_BUFFERS_ARB};
        int results[ARRAY_SIZE(attribs)];

        // Search for the pixel format with the closest number of multisample samples to the requested
        for (i = 0; i < *nformats; i++){
            int diff;
            wglGetPixelFormatAttribivARB(hdc, pformat[i], 0, ARRAY_SIZE(attribs), attribs, results);
            diff = ABS(results[0] - multisample_level) + ABS(results[1] - num_aux_buffers);
            if (diff < minDiff){
                minDiff = diff;
                bestFormat = i;
            }
        }
        pformat[0] = pformat[bestFormat];
    }

    if (*nformats && num_cached<ARRAY_SIZE(cache)) {
        cache[num_cached].formatAttributes = a2dup(formatAttributes);
        cache[num_cached].fattributes = (float*)a2dup((int*)fattributes);
        cache[num_cached].pformat = pformat[0];
        num_cached++;
    }
}

void checkFBO(const char *name, const char *fname, int line)
{
    INEXACT_CHECKGL;
    if (glCheckFramebufferStatusEXT) {
        if(rdr_caps.supports_ext_framebuffer_blit) {
            GLenum status = glCheckFramebufferStatusEXT( GL_READ_FRAMEBUFFER_EXT );
            if ( status != GL_FRAMEBUFFER_COMPLETE_EXT ) {
                Errorf( "%s:%d checkFBO read %s status : 0x%x (%s)\n", fname, line, name, status, GLErrorFromInt(status) );
            }
            status = glCheckFramebufferStatusEXT( GL_DRAW_FRAMEBUFFER_EXT );
            if ( status != GL_FRAMEBUFFER_COMPLETE_EXT ) {
                Errorf( "%s:%d checkFBO draw %s status : 0x%x (%s)\n", fname, line, name, status, GLErrorFromInt(status) );
            }
        } else {
            GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
            if ( status != GL_FRAMEBUFFER_COMPLETE_EXT ) {
                Errorf( "%s:%d checkFBO %s status : 0x%x (%s)\n", fname, line, name, status, GLErrorFromInt(status) );
            }
        }
    }
    INEXACT_CHECKGL;
}

void pbufCalcSizes(int width, int height, int *owidth, int *oheight, int *ovwidth, int *ovheight, bool makePowerOf2, int flags, int depth_bits)
{
    *ovwidth = width;
    *ovheight = height;
    // Pixel buffers do *not* need to be a power of two, unless they have a texture target
    *owidth = *ovwidth;
    *oheight = *ovheight;
    if (makePowerOf2) {
        *ovwidth = pow2(*ovwidth);
        *ovheight = pow2(*ovheight);
    }
    /*
    else if (rdr_caps.supports_arb_np2 && rdr_caps.features & GFXF_WATER && rdr_caps.chip & NV2X && isPower2(*ovwidth) && isPower2(*ovheight) && (flags & PB_RENDER_TEXTURE) && depth_bits && rdr_caps.supports_render_texture) {
        // Stupid crazy hack because reading the depth texture from a square RTT PBuffer is slow
        *ovwidth+=1;
    }
    */
}

int pbufInitCalls=0;

void pbufInitFake(PBuffer *pbuf, RdrPbufParams *params)
{
    pbuf->hardware_multisample_level = 1;
    pbuf->desired_multisample_level = params->desired_multisample_level;
    pbuf->required_multisample_level = 1;
    pbuf->software_multisample_level = 1;
    if (!rdr_caps.supports_arb_np2) {
        pbuf->virtual_width = pow2(params->width);
        pbuf->virtual_height = pow2(params->height);
    } else {
        pbuf->virtual_width = params->width;
        pbuf->virtual_height = params->height;
    }
    pbuf->width = params->width;
    pbuf->height = params->height;
    pbuf->isFakedPBuffer = 1;
    pbuf->isFakedRTT = !!(params->flags & PB_RENDER_TEXTURE);
    pbuf->flags &= ~PB_FBO;
}

// Gracefully handle and report GL_OUT_OF_MEMORY
#ifdef DOING_CHECKGL
#define CHECK_GL_OUT_OF_MEMORY check_GL_OUT_OF_MEMORY(__FILE__,__LINE__)
static bool check_GL_OUT_OF_MEMORY(char *fname,int line)
#else
#define CHECK_GL_OUT_OF_MEMORY check_GL_OUT_OF_MEMORY()
static bool check_GL_OUT_OF_MEMORY()
#endif
{
    GLenum err = gldGetError();

    if (err == GL_OUT_OF_MEMORY)
    {
        return true;
    }
#ifdef DOING_CHECKGL
    else if ( GL_NO_ERROR != err )
    {
        char err_string[1000];
        sprintf(err_string, "GL! %s:%d (0x%x) %s",fname,line,err, GLErrorFromInt(err));
        Errorf( err_string );
        return true;
    }
#endif

    return false;
}

// This function actually does the creation of the p-buffer.
// It can only be called once a window has already been created.
static bool pbufInitInternal(RdrPbufParams *params)
{
    HDC hdc;
    HGLRC hglrc;
    int format;
    int pformat[256];
    unsigned int nformats;
    int multisample_level = params->desired_multisample_level;
    PBuffer *pbuf = params->pbuf;
    bool makePowerOf2=false;
    int floatDepth=16;

    // Attribute arrays must be "0" terminated
    float fattributes[] = {0,0} ;
    int formatAttributes[50] = {
        WGL_DRAW_TO_PBUFFER_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB,  WGL_FULL_ACCELERATION_ARB,
        WGL_DOUBLE_BUFFER_ARB, GL_FALSE,
        0,0
    };
    int faIndex;
    int bufferAttributes[50] = {
        WGL_PBUFFER_LARGEST_ARB, GL_TRUE,
        0,0
    };
    int baIndex;
    int multiSampleIndex=0;

    rdrBeginMarker(__FUNCTION__);

    if (!rdr_caps.supports_fbos)
        params->flags &= ~PB_FBO;

    // Determine if we need to re-create this pbuffer or just re-use it
    if (!game_state.pbuftest &&
        pbuf->width && rdr_caps.supports_pbuffers && !(params->flags & PB_FBO)
        && params->required_multisample_level<=1
        && params->desired_multisample_level == pbuf->desired_multisample_level
        && params->num_aux_buffers <= pbuf->num_aux_buffers)
    {
        int newvw, newvh, neww, newh;
        // Already inited and it's a vanilla PBuffer
        if (params->flags & PB_RENDER_TEXTURE) {
            // Make resolution a power of 2!
            if (!rdr_caps.supports_arb_np2)
                makePowerOf2 = true;
        }
        pbufCalcSizes(params->width, params->height, &neww, &newh, &newvw, &newvh, makePowerOf2, params->flags, params->depth_bits);
        if (params->flags==pbuf->flags &&
            pbuf->virtual_width == newvw &&
            pbuf->virtual_height == newvh)
        {
            // Re-use it!
            pbuf->width = neww;
            pbuf->height = newh;
            rdrEndMarker();
            return true;
        }
    }

    pbufInitCalls++;

    winMakeCurrentDirect();

    if (pbuf->width)
    {
        pbufDeleteDirect(&pbuf);
    }

    pbuf->flags = params->flags;
    pbuf->currentCubemapTarget = 0;
    pbuf->copy_texture_id = -1;
    pbuf->color_handle_aux[0] = 0;

    if (!rdr_caps.supports_pbuffers || (params->flags & PB_FBO) ||
        ((params->flags & PB_ALLOW_FAKE_PBUFFER) && !(params->flags & (PB_FLOAT|PB_ALPHA))))
    {
        // Fake PBuffer or FBO
        const FogContext *orgFogContext = WCW_GetFogContext();

        pbuf->fog_context = calloc(1, sizeof(FogContext));
        WCW_InitFogContext(pbuf->fog_context);

        // Calling WCW_InitFogContext() above also sets the current fog context
        // Restore the original fog context here:
        WCW_SetFogContext((FogContext *)orgFogContext);

        // Init dummy stuff
        pbufInitFake(pbuf, params);
        if (!(params->flags & PB_FBO)) {
            // Fake PBuffer
            pbuf->color_internal_format = GL_RGBA8;
            pbuf->color_format = GL_RGBA;
            pbuf->color_type = GL_UNSIGNED_BYTE;
            pbuf->depth_internal_format = GL_DEPTH_COMPONENT24;
            pbuf->depth_format = GL_DEPTH_COMPONENT;
            pbuf->depth_type = GL_UNSIGNED_INT;

            rdrEndMarker();
            return true;
        } else { // (params->flags & PB_FBO)
            bool bUseColorRenderBuffer;
            bool bUseDepthRenderBuffer;
            int i;

            pbuf->flags |= PB_FBO; // Cleared in pbufInitFake
            pbuf->num_aux_buffers = params->num_aux_buffers;
            pbuf->hasStencil = false;

            multisample_level = MIN(multisample_level, rdr_caps.max_fbo_samples);

            bUseColorRenderBuffer = ((multisample_level>1) || !(params->flags & PB_COLOR_TEXTURE));
            bUseDepthRenderBuffer = ((multisample_level>1) || !(params->flags & PB_DEPTH_TEXTURE));

            // For some reason, 1x1 FBOs won't work
            if (pbuf->virtual_width == 1)
                pbuf->virtual_width = 2;
            if (pbuf->virtual_height == 1)
                pbuf->virtual_height = 2;

            pbuf->desired_multisample_level = params->desired_multisample_level;
            pbuf->required_multisample_level = params->required_multisample_level;
            pbuf->hardware_multisample_level = multisample_level;
            pbuf->software_multisample_level = MAX(pbuf->required_multisample_level / pbuf->hardware_multisample_level, 1);;

            assert(rdr_frame_state.numFBOs > 0 && rdr_frame_state.numFBOs <= MAX_SLI);
            pbuf->allocated_fbos = rdr_frame_state.numFBOs;

            glGenFramebuffersEXT(pbuf->allocated_fbos, pbuf->fbo); CHECKGL;

            for(i=0; i<pbuf->allocated_fbos; i++)
            {
                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pbuf->fbo[i]); CHECKGL;

                // initialize color
                if (params->flags & PB_COLOR_TEXTURE)
                {
                    bool bIsMipmapped = false;
                    int maxMipLevel = 0;
                    bool bIsCubemap = (pbuf->flags & PB_CUBEMAP) != 0;
                    int textureTarget = bIsCubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
    
                    // 8-bit RGB
                    GLint internalFormat = GL_RGB8;
                    GLenum format = GL_RGB;
                    GLenum type = GL_UNSIGNED_BYTE;
    
                    // We only handle 8-bit components non-float, and 16-bit float components right now
    
                    if (params->flags & PB_FLOAT)
                    {
                        pbuf->isFloat = 1;
                        type = GL_FLOAT;
                        if (params->flags & PB_ALPHA)
                        {
                            // 16-bit float RGBA
                            internalFormat = GL_RGBA16F;
                            format = GL_RGBA;
                        }
                        else
                        {
                            // 16-bit float RGB
                            internalFormat = GL_RGB16F;
                        }
                    }
                    else
                    {
                        pbuf->isFloat = 0;
                        if (params->flags & PB_ALPHA)
                        {
                            // 8-bit RGBA
                            internalFormat = GL_RGBA8;
                            format = GL_RGBA;
                        }
                    }
    
                    pbuf->color_internal_format = internalFormat;
                    pbuf->color_format = format;
                    pbuf->color_type = type;
    
                    // setup mipmap flag
                    if (params->flags & PB_MIPMAPPED)
                    {
                        if (!(params->flags & PB_FLOAT) || rdr_caps.supports_pixel_format_float_mipmap)
                        {
                            pbuf->flags |= PB_MIPMAPPED;
                            bIsMipmapped = true;
                            maxMipLevel = params->max_mip_level;
                        }
                    }
    
                    if (bUseColorRenderBuffer)
                    {
                        // Initialize color render buffer
                        glGenRenderbuffersEXT(1, &pbuf->color_rbhandle[i]); CHECKGL;
                        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, pbuf->color_rbhandle[i]); CHECKGL;
                        if (multisample_level == 1)
                        {
                            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, internalFormat, pbuf->virtual_width, pbuf->virtual_height);
                            if (CHECK_GL_OUT_OF_MEMORY)
                            {
                                rdrEndMarker();
                                return false;
                            }
                        }
                        else
                        {
                            glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, multisample_level, internalFormat, pbuf->virtual_width, pbuf->virtual_height);
                            if (CHECK_GL_OUT_OF_MEMORY)
                            {
                                rdrEndMarker();
                                return false;
                            }
                        }
    
    #if 1
                        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, pbuf->color_rbhandle[i]); CHECKGL;
    #else
                        glFramebufferRenderbufferEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, pbuf->color_rbhandle[i]); CHECKGL;
                        glFramebufferRenderbufferEXT(GL_READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, pbuf->color_rbhandle[i]); CHECKGL;
    #endif
    
                        if (params->flags & PB_COLOR_TEXTURE)
                        {
                            // Initialize color texture
                            glGenTextures(1, &pbuf->color_handle[i]); CHECKGL;
                            WCW_BindTexture(textureTarget, TEXLAYER_BASE, pbuf->color_handle[i]);
                            setTextureParameters(textureTarget, !pbuf->isFloat || bIsMipmapped, maxMipLevel);
    
                            if(bIsCubemap) {
                                int face;
                                glTexParameterf(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); CHECKGL;
                                glTexParameterf(textureTarget, GL_TEXTURE_MIN_LOD, 0.5f); CHECKGL;
    
                                // generate texture memory for each face
                                for(face=0; face<6; face++) {
                                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+face, 0, internalFormat, pbuf->virtual_width, pbuf->virtual_height, 0, format, type, NULL); CHECKGL;
                                    
                                    if (bIsMipmapped) {
                                        glGenerateMipmapEXT(GL_TEXTURE_CUBE_MAP); CHECKGL;
                                    }
                                }
                            } else {
                                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, pbuf->virtual_width, pbuf->virtual_height, 0, format, type, NULL); CHECKGL;
                                if (bIsMipmapped) {
                                    glGenerateMipmapEXT(GL_TEXTURE_2D); CHECKGL;
                                }
                            }
    
                            // This FBO is used to blit from the color renderbuffer to the color texture
                            if (pbuf->blit_fbo[i] == 0) {
                                glGenFramebuffersEXT(1, &pbuf->blit_fbo[i]); CHECKGL;
                            }
    
                            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pbuf->blit_fbo[i]); CHECKGL;                        
                            pbuf->blit_flags |= GL_COLOR_BUFFER_BIT;                        
                            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, bIsCubemap ? GL_TEXTURE_CUBE_MAP_POSITIVE_X : GL_TEXTURE_2D, pbuf->color_handle[i], 0); CHECKGL;
                            CHECKFBO("color blit");

                            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pbuf->fbo[i]); CHECKGL;
                        }
                    }
                    else
                    {
                        // Don't use a renderbuffer (blitting is not supported and color texture is needed)
                        glGenTextures(1, &pbuf->color_handle[i]); CHECKGL;
                        WCW_BindTexture(textureTarget, TEXLAYER_BASE, pbuf->color_handle[i]); CHECKGL;
                        setTextureParameters(textureTarget, !pbuf->isFloat || bIsMipmapped, maxMipLevel);
    
                        if(bIsCubemap) {
                            int face;
                            glTexParameterf(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); CHECKGL;
                            glTexParameterf(textureTarget, GL_TEXTURE_MIN_LOD, 0.5f); CHECKGL;
    
                            // generate texture memory for each face
                            for(face=0; face<6; face++)  {
                                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+face, 0, internalFormat, pbuf->virtual_width, pbuf->virtual_height, 0, format, type, NULL); CHECKGL;
                                if (bIsMipmapped) {
                                    glGenerateMipmapEXT(GL_TEXTURE_CUBE_MAP); CHECKGL;
                                }
                            }
                        } else {
                            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, pbuf->virtual_width, pbuf->virtual_height, 0, format, type, NULL); CHECKGL;                          
                            if (bIsMipmapped) {
                                glGenerateMipmapEXT(GL_TEXTURE_2D); CHECKGL;
                            }
                        }
                        
                        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, bIsCubemap ? GL_TEXTURE_CUBE_MAP_POSITIVE_X : GL_TEXTURE_2D, pbuf->color_handle[i], 0); CHECKGL;
    
                        // multisample not available with FBO + texture
                        pbuf->color_rbhandle[i] = 0;

                        // Are we asked to also create an aux texture attachment (e.g. for filtering)
                        // @todo this is a hack at the moment for planar reflection filtering.
                        // the old deprecated MRT support with the aux_buffers should be repurposed
                        if (!bIsCubemap && params->flags & PB_COLOR_TEXTURE_AUX)
                        {
                            glGenTextures(1, &pbuf->color_handle_aux[i]); CHECKGL;
                            WCW_BindTexture(textureTarget, TEXLAYER_BASE, pbuf->color_handle_aux[i]); CHECKGL;
                            setTextureParameters(textureTarget, !pbuf->isFloat || bIsMipmapped, maxMipLevel);
                            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, pbuf->virtual_width, pbuf->virtual_height, 0, format, type, NULL); CHECKGL;                          
                            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, pbuf->color_handle_aux[i], 0); CHECKGL;
                        }

                    }
                }
                else
                {
                    // disable drawing/reading from the color buffers
                    glDrawBuffer(GL_NONE); CHECKGL;
                    glReadBuffer(GL_NONE); CHECKGL;
                } // end initialize color
    
    
                // Initialize Depth
                if (params->depth_bits)
                {
                    GLint depth_internal_format;
                    GLenum depth_format;
                    GLenum depth_type;
    
                    if (params->stencil_bits && rdr_caps.supports_packed_stencil)
                    {
                        depth_internal_format = GL_DEPTH24_STENCIL8;
                        depth_format = GL_DEPTH_STENCIL;
                        depth_type = GL_UNSIGNED_INT_24_8;
                        params->depth_bits = 24;
                        pbuf->hasStencil = true;
                    }
                    else
                    {
                        if (params->depth_bits==16)
                        {
                            depth_internal_format = GL_DEPTH_COMPONENT16;
                        }
                        else
                        {
                            depth_internal_format = GL_DEPTH_COMPONENT24;
                            params->depth_bits = 24;
                        }
                        depth_format = GL_DEPTH_COMPONENT;
                        depth_type = GL_UNSIGNED_INT;
                    }
                    pbuf->depth_internal_format = depth_internal_format;
                    pbuf->depth_format = depth_format;
                    pbuf->depth_type = depth_type;
    
                    if (bUseDepthRenderBuffer)
                    {
                        // Initialize depth render buffer
                        glGenRenderbuffersEXT(1, &pbuf->depth_rbhandle[i]); CHECKGL;
                        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, pbuf->depth_rbhandle[i]); CHECKGL;
    
                        if (multisample_level == 1)
                        {
                            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, depth_internal_format, pbuf->virtual_width, pbuf->virtual_height);
                            if (CHECK_GL_OUT_OF_MEMORY)
                            {
                                rdrEndMarker();
                                return false;
                            }
                        }
                        else
                        {
                            glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, multisample_level, depth_internal_format, pbuf->virtual_width, pbuf->virtual_height);
                            if (CHECK_GL_OUT_OF_MEMORY)
                            {
                                rdrEndMarker();
                                return false;
                            }
                        }
    
                        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, pbuf->depth_rbhandle[i]); CHECKGL; 
                        if (pbuf->hasStencil) {
                            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, pbuf->depth_rbhandle[i]); CHECKGL;
                        } else {
                            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0); CHECKGL;
                        }
    
                        if (params->flags & PB_DEPTH_TEXTURE)
                        {
                            // Initialize depth texture
                            glGenTextures(1, &pbuf->depth_handle[i]); CHECKGL;
    
                            WCW_BindTexture(GL_TEXTURE_2D, TEXLAYER_BASE, pbuf->depth_handle[i]);
                            setTextureParameters(GL_TEXTURE_2D, false, 0);
    
                            glTexImage2D(GL_TEXTURE_2D, 0, depth_internal_format, pbuf->virtual_width, pbuf->virtual_height, 0, depth_format, depth_type, NULL); CHECKGL;
    
                            // This FBO is used to blit from the depth renderbuffer to the depth texture
                            if (pbuf->blit_fbo[i] == 0) {
                                glGenFramebuffersEXT(1, &pbuf->blit_fbo[i]); CHECKGL;
                            }
    
                            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pbuf->blit_fbo[i]); CHECKGL;    
                            pbuf->blit_flags |= GL_DEPTH_BUFFER_BIT | (pbuf->hasStencil ? GL_STENCIL_BUFFER_BIT : 0); 
                            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, pbuf->depth_handle[i], 0); CHECKGL;
                            if (pbuf->hasStencil) {
                                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_TEXTURE_2D, pbuf->depth_handle[i], 0); CHECKGL;
                            } else {
                                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0); CHECKGL;
                            }
    
                            // disable drawing/reading from the color buffers
                            if (!(params->flags & PB_COLOR_TEXTURE))
                            {
                                glDrawBuffer(GL_NONE); CHECKGL;
                                glReadBuffer(GL_NONE); CHECKGL;
                            }
    
                            CHECKFBO("depth blit");
    
                            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, pbuf->fbo[i]); CHECKGL;
                        }
                    }
                    else
                    {
                        // Don't use a renderbuffer (blitting is not supported and depth texture is needed)
                        glGenTextures(1, &pbuf->depth_handle[i]); CHECKGL;
                        WCW_BindTexture(GL_TEXTURE_2D, TEXLAYER_BASE, pbuf->depth_handle[i]);
                        setTextureParameters(GL_TEXTURE_2D, false, 0);
                        glTexImage2D(GL_TEXTURE_2D, 0, depth_internal_format, pbuf->virtual_width, pbuf->virtual_height, 0, depth_format, depth_type, 0); CHECKGL;
                        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, pbuf->depth_handle[i], 0); CHECKGL;
                        if (pbuf->hasStencil) {
                            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_TEXTURE_2D, pbuf->depth_handle[i], 0); CHECKGL;
                        }
                    }
                } // End initialize depth
    
                // Only packed stencil buffers (packed into the depth buffer) appear to be
                // supported right now.  04/03/09
                /*
                if (params->stencil_bits && !pbuf->hasStencil) {
                    // initialize stencil renderbuffer
                    glGenRenderbuffersEXT(1, &pbuf->stencil_rbhandle); CHECKGL;
                    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, pbuf->stencil_rbhandle); CHECKGL;
                    if (multisample_level == 1)
                    {
                        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_STENCIL_INDEX, pbuf->virtual_width, pbuf->virtual_height);
                        if (CHECK_GL_OUT_OF_MEMORY)
                        {
                            rdrEndMarker();
                            return false;
                        }
                    }
                    else
                    {
                        glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, multisample_level, GL_STENCIL_INDEX, pbuf->virtual_width, pbuf->virtual_height);
                        if (CHECK_GL_OUT_OF_MEMORY)
                        {
                            rdrEndMarker();
                            return false;
                        }
                    }
                    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, pbuf->stencil_rbhandle); CHECKGL;
                }
                */
    
                if (params->num_aux_buffers) {
                    int i;
                    assert(params->num_aux_buffers <= ARRAY_SIZE(pbuf->aux_handles));
                    for (i=0; i<params->num_aux_buffers; i++) 
                    {
                        // initialize aux renderbuffer
                        glGenTextures(1, &pbuf->aux_handles[i]); CHECKGL;
                        // initialize color texture
                        WCW_BindTexture(GL_TEXTURE_2D, TEXLAYER_BASE, pbuf->aux_handles[i]);
                        setTextureParameters(GL_TEXTURE_2D, !(params->flags & PB_FLOAT), 0);
                        // TOOD: handle all these parameters?
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pbuf->virtual_width, pbuf->virtual_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); CHECKGL;
                        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT+i, GL_TEXTURE_2D, pbuf->aux_handles[i], 0); CHECKGL;    
                    }
                }
    
                CHECKFBO("main FBO");
            }

            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); CHECKGL;
            WCW_BindTexture(GL_TEXTURE_2D, TEXLAYER_BASE, 0);
            WCW_BindTexture(GL_TEXTURE_CUBE_MAP, TEXLAYER_BASE, 0);
            rdrEndMarker();
            return true;
        }
    } // End Fake PBuffer or FBOs


    for (faIndex=0; formatAttributes[faIndex]; faIndex+=2);
    for (baIndex=0; bufferAttributes[baIndex]; baIndex+=2);

    pbuf->buffer = 0;
    hdc = wglGetCurrentDC();
    hglrc = wglGetCurrentContext();

    if (params->stencil_bits) {
        formatAttributes[faIndex++] = WGL_STENCIL_BITS_ARB;
        formatAttributes[faIndex++] = params->stencil_bits;
        pbuf->hasStencil = 1;
    }
    formatAttributes[faIndex++] = WGL_DEPTH_BITS_ARB;
    formatAttributes[faIndex++] = params->depth_bits;
    if (params->flags & PB_FLOAT) {
        formatAttributes[faIndex++] = WGL_PIXEL_TYPE_ARB;
        formatAttributes[faIndex++] = WGL_TYPE_RGBA_FLOAT_ATI;
        formatAttributes[faIndex++] = WGL_RED_BITS_ARB;
        formatAttributes[faIndex++] = floatDepth;
        formatAttributes[faIndex++] = WGL_GREEN_BITS_ARB;
        formatAttributes[faIndex++] = floatDepth;
        formatAttributes[faIndex++] = WGL_BLUE_BITS_ARB;
        formatAttributes[faIndex++] = floatDepth;
        formatAttributes[faIndex++] = WGL_ALPHA_BITS_ARB;
        if (params->flags & PB_ALPHA) {
            formatAttributes[faIndex++] = floatDepth;
        } else {
            formatAttributes[faIndex++] = 0;
        }
    } else {
        formatAttributes[faIndex++] = WGL_ALPHA_BITS_ARB;
        if (params->flags & PB_ALPHA) {
            formatAttributes[faIndex++] = 8;
        } else {
            formatAttributes[faIndex++] = 0;
        }
        formatAttributes[faIndex++] = WGL_PIXEL_TYPE_ARB;
        formatAttributes[faIndex++] = WGL_TYPE_RGBA_ARB;
    }

    if (params->num_aux_buffers) {
        formatAttributes[faIndex++] = WGL_AUX_BUFFERS_ARB;
        formatAttributes[faIndex++] = params->num_aux_buffers;
    }

    if (params->flags & PB_RENDER_TEXTURE) {
        if (rdr_caps.supports_render_texture && params->desired_multisample_level<=1) {
            if (params->flags & PB_ALPHA) {
                formatAttributes[faIndex++] = WGL_BIND_TO_TEXTURE_RGBA_ARB;
            } else {
                formatAttributes[faIndex++] = WGL_BIND_TO_TEXTURE_RGB_ARB;
            }
            formatAttributes[faIndex++] = GL_TRUE;

            bufferAttributes[baIndex++] = WGL_TEXTURE_FORMAT_ARB;
            if (params->flags & PB_ALPHA) {
                bufferAttributes[baIndex++] = WGL_TEXTURE_RGBA_ARB;
            } else {
                bufferAttributes[baIndex++] = WGL_TEXTURE_RGB_ARB;
            }

            bufferAttributes[baIndex++] = WGL_TEXTURE_TARGET_ARB;
            bufferAttributes[baIndex++] = WGL_TEXTURE_2D_ARB;

            bufferAttributes[baIndex++] = WGL_MIPMAP_TEXTURE_ARB;
            bufferAttributes[baIndex++] = GL_FALSE;
        } else {
            pbuf->isFakedRTT = 1;
        }

        // Make resolution a power of 2!
        if (!rdr_caps.supports_arb_np2)
            makePowerOf2 = true;
    }

    if (multisample_level > 1) {
        multiSampleIndex = faIndex;
        // Check For Multisampling
        formatAttributes[faIndex++] = WGL_SAMPLE_BUFFERS_ARB;
        formatAttributes[faIndex++] = GL_TRUE; // ATI 8500/9000 does not support this :(
        formatAttributes[faIndex++] = WGL_SAMPLES_ARB;
        formatAttributes[faIndex++] = multisample_level;
    }

    // Terminate
    bufferAttributes[baIndex++] = 0;
    bufferAttributes[baIndex++] = 0;
    formatAttributes[faIndex++] = 0;
    formatAttributes[faIndex++] = 0;

    // Query for a suitable pixel format based on the specified mode.
    do {
        if (multiSampleIndex) {
            formatAttributes[multiSampleIndex+3] = multisample_level;
            if (multisample_level <= 1) {
                // No multisampling
                formatAttributes[multiSampleIndex+0] = formatAttributes[multiSampleIndex+1] = 0;
            }
        }
        choosePixelFormatCached(hdc, formatAttributes, fattributes, ARRAY_SIZE(pformat), pformat, &nformats, multisample_level, params->num_aux_buffers);
        if (!nformats)
            multisample_level >>= 1;
    } while (!nformats && multisample_level!=0);
    format = pformat[0];

    if (!nformats) {
        if (PBUFFER_FALLBACK) {
            printf("pbuffer creation error:  Couldn't find a suitable pixel format.\n  Falling back to RTBBCTT.\n" );
            pbufInitFake(pbuf, params);
            rdrEndMarker();
            return false;
        }
        FatalPbufferError("pbuffer creation error:  Couldn't find a suitable pixel format.\n" );
    }

    if (params->num_aux_buffers)
    {
        int queryFormatAttributes[] = {WGL_AUX_BUFFERS_ARB};
        int results[ARRAY_SIZE(queryFormatAttributes)];
        wglGetPixelFormatAttribivARB(hdc, format, 0, ARRAY_SIZE(queryFormatAttributes), queryFormatAttributes, results);
        //printf("# of Aux buffers: %d\n", results[0]);
        pbuf->num_aux_buffers = MIN(params->num_aux_buffers, results[0]);
    }

    // Create the p-buffer.
    pbuf->hardware_multisample_level = MAX(multisample_level, 1);
    pbuf->desired_multisample_level = MAX(params->desired_multisample_level, 1);
    pbuf->required_multisample_level = MAX(params->required_multisample_level, 1);
    pbuf->software_multisample_level = MAX(pbuf->required_multisample_level / pbuf->hardware_multisample_level, 1);
    pbufCalcSizes(params->width * pbuf->software_multisample_level, params->height * pbuf->software_multisample_level, &pbuf->width, &pbuf->height, &pbuf->virtual_width, &pbuf->virtual_height, makePowerOf2, params->flags, params->depth_bits);
    //printf("Requested: %d, hardware: %d, software: %d, (%dx%d)\n", pbuf->desired_multisample_level, pbuf->hardware_multisample_level, pbuf->software_multisample_level, pbuf->width, pbuf->height);
    pbuf->isFloat = !!(params->flags & PB_FLOAT);
    {
        int tries=4; // Try multiple times, Intel says this may work on their card when it sometimes fails the first time.
        do {
            tries--;
            pbuf->buffer = wglCreatePbufferARB( hdc, format, pbuf->virtual_width, pbuf->virtual_height, bufferAttributes );
            if ( !pbuf->buffer )
            {
                static DWORD err;
                err = GetLastError(); // static so it shows up in optimzed minidumps
                if ( tries==0 )
                {
                    if (PBUFFER_FALLBACK) {
                        printf("pbuffer creation error:  wglCreatePbufferARB returned NULL (error: 0x%08X).\n  Falling back to RTBBCTT.\n", err );
                        pbufInitFake(pbuf, params);
                        rdrEndMarker();
                        return false;
                    }

                    if ( err == ERROR_INVALID_PIXEL_FORMAT )
                        FatalPbufferError("error:  ERROR_INVALID_PIXEL_FORMAT\n" );
                    else if ( err == ERROR_NO_SYSTEM_RESOURCES )
                        FatalPbufferError("error:  ERROR_NO_SYSTEM_RESOURCES\n" );
                    else if ( err == ERROR_INVALID_DATA )
                        FatalPbufferError("error:  ERROR_INVALID_DATA\n" );
                    else {
                        char buf[32];
                        sprintf(buf, "error:  %08X\n", err);
                        FatalPbufferError( buf );
                    }
                }
            }
        } while (!pbuf->buffer);
    }

    // Get the device context.
    pbuf->myDC = wglGetPbufferDCARB( pbuf->buffer );
    if ( !pbuf->myDC ) // This happens if you ask for too large of a pbuffer
        FatalPbufferError("pbuffer creation error:  wglGetPbufferDCARB() failed\n" );

    // Create a gl context for the p-buffer.
    pbuf->myGLctx = wglCreateContext( pbuf->myDC );
    if ( !pbuf->myGLctx )
        FatalPbufferError("pbuffer creation error:  wglCreateContext() failed\n" );

    if( !(params->flags & PB_NOSHARE) )
    {
        if( !wglShareLists(hglrc, pbuf->myGLctx) )
            FatalPbufferError("pbuffer: wglShareLists() failed\n" );
    }
    // Determine the actual width and height we were able to create.
    wglQueryPbufferARB( pbuf->buffer, WGL_PBUFFER_WIDTH_ARB, &pbuf->virtual_width );
    wglQueryPbufferARB( pbuf->buffer, WGL_PBUFFER_HEIGHT_ARB, &pbuf->virtual_height );
    wglQueryPbufferARB( pbuf->buffer, WGL_SAMPLES_ARB, &pbuf->hardware_multisample_level);

    // Determine if render to texture is *really* supported
    if ((pbuf->flags & PB_RENDER_TEXTURE) && !pbuf->isFakedRTT)
    {
        GLint texFormat = WGL_NO_TEXTURE_ARB;
        wglQueryPbufferARB(pbuf->buffer, WGL_TEXTURE_FORMAT_ARB, &texFormat);
        if (texFormat == WGL_NO_TEXTURE_ARB)
            pbuf->isFakedRTT = true;
    }

    pbuf->fog_context = 0;
    pbufMakeCurrentDirect(pbuf);
    //glEnable(GL_DEPTH_TEST);
    rdrInitDirect();
    if (params->num_aux_buffers) {
        rwglSetAppropriateDrawBuffers(pbuf);
    }

    glGetIntegerv(GL_DEPTH_BITS, &pbuf->depth); CHECKGL;

    if (params->flags & PB_FLOAT) {
        pbuf->color_internal_format = GL_RGBA16F;
        pbuf->color_format = GL_RGBA;
        pbuf->color_type = GL_FLOAT;
    } else {
        pbuf->color_internal_format = GL_RGBA8;
        pbuf->color_format = GL_RGBA;
        pbuf->color_type = GL_UNSIGNED_BYTE;
    }

    if (pbuf->depth == 16)
        pbuf->depth_internal_format = GL_DEPTH_COMPONENT16;
    else
        pbuf->depth_internal_format = GL_DEPTH_COMPONENT24;
    pbuf->depth_format = GL_DEPTH_COMPONENT;
    pbuf->depth_type = GL_UNSIGNED_INT;

    if (pbuf->hardware_multisample_level > 1) {
        glEnable(GL_MULTISAMPLE_ARB); CHECKGL;
    }

    /*
    if (pbuf->flags & PB_COLOR_TEXTURE)
    {
        glActiveTextureARB(GL_TEXTURE0); CHECKGL;
        glGenTextures(1, &pbuf->color_handle[0]); CHECKGL;
        WCW_BindTexture(GL_TEXTURE_2D, 0, pbuf->color_handle[0]); CHECKGL;
        setTextureParameters(GL_TEXTURE_2D, !(pbuf->flags & PB_FLOAT), 0); CHECKGL;
        glTexImage2D(GL_TEXTURE_2D, 0, pbuf->color_internal_format, pbuf->virtual_width, pbuf->virtual_height, 0, pbuf->color_format, pbuf->color_type, NULL); CHECKGL;
    }
    */

    if (pbuf->flags & PB_DEPTH_TEXTURE)
    {
        glGenTextures(1, &pbuf->depth_handle[0]); CHECKGL;
        WCW_BindTexture(GL_TEXTURE_2D, 0, pbuf->depth_handle[0]);
        setTextureParameters(GL_TEXTURE_2D, !(pbuf->flags & PB_FLOAT), 0); CHECKGL;
        glTexImage2D(GL_TEXTURE_2D, 0, pbuf->depth_internal_format, pbuf->virtual_width, pbuf->virtual_height, 0, pbuf->depth_format, pbuf->depth_type, NULL); CHECKGL;
    }

    /*
    if (pbuf->num_aux_buffers)
    {
        int i;
        for (i=0; i<pbuf->num_aux_buffers; i++)
        {
            glActiveTextureARB(GL_TEXTURE0); CHECKGL;
            glGenTextures(1, &pbuf->aux_handles[i]); CHECKGL;
            WCW_BindTexture(GL_TEXTURE_2D, 0, pbuf->aux_handles[i]); CHECKGL;
            setTextureParameters(GL_TEXTURE_2D, !(pbuf->flags & PB_FLOAT), 0); CHECKGL;
            glTexImage2D(GL_TEXTURE_2D, 0, pbuf->color_internal_format, pbuf->virtual_width, pbuf->virtual_height, 0, pbuf->color_format, pbuf->color_type, NULL); CHECKGL;
        }
    }
    */

    winMakeCurrentDirect();

    listAddForeignMember(&pbuf_list, pbuf);
    rdrEndMarker();
    return true;
}

void pbufInitDirect(RdrPbufParams *params)
{
    if (!pbufInitInternal(params))
    {
        // Intitialization failed.  See if we can disable something and make it work
        pbufDeleteDirect(&params->pbuf);

        // See if we can reduce the multisample level and make it work
        if (params->desired_multisample_level > 1)
        {
            params->desired_multisample_level >>= 1;

            // Recursively call self
            pbufInitDirect(params);
        }
        /*
        // TODO: try some other option
        else if ()
        {
        }
        */
        else
        {
            if (PBUFFER_FALLBACK)
            {
                printf("pbuffer creation error\n" );
                pbufInitFake(params->pbuf, params);
                return;
            }
            FatalPbufferError("pbuffer creation error.\n" );
        }
    }
}

void pbufBindAuxDirect(PBuffer *pbuf, int tex_unit, int buffer)
{
    int handle=0;
    if (rdr_caps.chip == TEX_ENV_COMBINE)
        rdrPrepareForDraw(); // Flush any outstanding texture binds, since we're making direct calls here

    if (pbuf->flags & PB_FBO)
    {
        // If you hit this assert, then you forgot to specify the PB_COLOR_TEXTURE flag
        assert(pbuf->flags & PB_COLOR_TEXTURE);
        CHECK_SLI_FBO(pbuf);
        assert(pbuf->color_handle[pbuf->curFBO]);
    }

    if (!pbuf->color_handle[pbuf->curFBO]) {
        assert(!pbuf->isFakedRTT); // If not, this should have already been generated
        glGenTextures(1, &pbuf->color_handle[pbuf->curFBO]); CHECKGL;
    }
    handle = pbuf->color_handle[pbuf->curFBO];
    if (buffer!=WGL_FRONT_LEFT_ARB) {
        int index = buffer - WGL_AUX0_ARB;
        if (!pbuf->aux_handles[index]) {
            assert(!pbuf->isFakedRTT); // If not, this should have already been generated
            glGenTextures(1, &pbuf->aux_handles[index]); CHECKGL;
        }
        handle = pbuf->aux_handles[index];
    }
    assert(handle);
    WCW_BindTexture(GL_TEXTURE_2D, tex_unit, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); CHECKGL;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); CHECKGL;
    if (pbuf->isFloat) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECKGL;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECKGL;
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECKGL;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECKGL;
    }

    if (!pbuf->isFakedRTT && !(pbuf->flags & PB_FBO)) {
        {
            GLint texFormat = WGL_NO_TEXTURE_ARB;
            wglQueryPbufferARB(pbuf->buffer, WGL_TEXTURE_FORMAT_ARB, &texFormat);

            if (texFormat == WGL_NO_TEXTURE_ARB)
                assert(0);
        }

        if (!wglBindTexImageARB(pbuf->buffer, buffer))
        {
            DWORD error = GetLastError();

             // pbuff->buffer is not a valid handle. 
            assert(error != ERROR_INVALID_HANDLE);

            // buffer is not a valid value. 
            assert(error != ERROR_INVALID_DATA);

            // The pbuffer attribute WGL_TEXTURE_FORMAT_ARB is set to
            // WGL_NO_TEXTURE_ARB or buffer is already bound to the texture
            assert(error != ERROR_INVALID_OPERATION);

            // Unknown error
            assert(0);
        }
    }

    pbuf->isBound = true;
}

void pbufBindDirect2(PBuffer *pbuf, int tex_unit)
{
    pbufBindAuxDirect(pbuf, tex_unit, WGL_FRONT_LEFT_ARB);
}

void pbufBindDepthDirect(PBuffer *pbuf, int tex_unit)
{
    assert(pbuf->flags & PB_DEPTH_TEXTURE);
    CHECK_SLI_FBO(pbuf);
    if (pbuf->depth_handle[pbuf->curFBO]) {
        WCW_BindTexture(GL_TEXTURE_2D, tex_unit, pbuf->depth_handle[pbuf->curFBO]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); CHECKGL;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); CHECKGL;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECKGL;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECKGL;
    } else {
        WCW_BindTexture(GL_TEXTURE_2D, tex_unit, rdr_view_state.white_tex_id);
    }
}

void pbufReleaseDepthDirect(PBuffer *pbuf, int tex_unit)
{
    assert(pbuf->flags & PB_DEPTH_TEXTURE);
    WCW_BindTexture(GL_TEXTURE_2D, tex_unit, rdr_view_state.white_tex_id);
}


void pbufBindDirect(PBuffer *pbuf)
{
    pbufBindDirect2(pbuf, 0);
}


void pbufReleaseAuxDirect(PBuffer *pbuf, int tex_unit, int buffer)
{
    int ret;
    int handle;
    CHECK_SLI_FBO(pbuf);
    handle = pbuf->color_handle[pbuf->curFBO];
    if (buffer!=WGL_FRONT_LEFT_ARB) {
        handle = pbuf->aux_handles[buffer - WGL_AUX0_ARB];
    }
    assert(handle);
    WCW_BindTexture(GL_TEXTURE_2D, tex_unit, handle);

    if (!pbuf->isFakedRTT && !(pbuf->flags & PB_FBO)) {
        ret = wglReleaseTexImageARB(pbuf->buffer, buffer);
        assert(ret);
    }

    pbuf->isBound = false;
}

void pbufReleaseDirect2(PBuffer *pbuf, int tex_unit)
{
    pbufReleaseAuxDirect(pbuf, tex_unit, WGL_FRONT_LEFT_ARB);
}

void pbufReleaseDirect(PBuffer *pbuf)
{
    pbufReleaseDirect2(pbuf, 0);
}


void pbufInitDirect2(PBuffer *pbuf,int w,int h,PBufferFlags flags,int desired_multisample_level, int required_multisample_level, int stencil_bits, int depth_bits, int num_aux_buffers)
{
    RdrPbufParams    params;
    params.pbuf                            = pbuf;
    params.width                        = w;
    params.height                        = h;
    params.flags                        = flags;
    params.desired_multisample_level    = desired_multisample_level;
    params.required_multisample_level    = required_multisample_level;
    params.stencil_bits                    = stencil_bits;
    params.depth_bits                    = depth_bits;
    params.num_aux_buffers                = num_aux_buffers;
    params.max_mip_level                = 0;
    pbufInitDirect(&params);
}

void pbufSetCopyTextureDirect(RdrPbufSetCopyTextureParams *params)
{
    PBuffer *pbuf = params->pbuf;

    assert(pbuf);
    assert(pbuf->flags & PB_COLOR_TEXTURE);
    assert(params->copy_texture_id >= 0);

    pbuf->flags |= PB_ONESHOTCOPY;
    pbuf->copy_texture_id = params->copy_texture_id;

    WCW_BindTexture(GL_TEXTURE_2D, 0, params->copy_texture_id);
    setTextureParameters(GL_TEXTURE_2D, !(pbuf->flags & PB_FLOAT), 0); CHECKGL;
    glTexImage2D(GL_TEXTURE_2D, 0, pbuf->color_internal_format, pbuf->virtual_width, pbuf->virtual_height, 0, pbuf->color_format, pbuf->color_type, NULL); CHECKGL;
}

static int s_frame_grab_blit_fbo = 0;

void pbufCopyDirect(PBuffer *pbuf, int tex_target, int color_texture_handle, int depth_texture_handle, int x, int y, int width, int height)
{
    pbufMakeCurrentDirect(pbuf);

    winCopyFrameBufferDirect(tex_target, color_texture_handle, depth_texture_handle, x, y, width, height, 0, false, false);

    winMakeCurrentDirect();
}

// Note that flipX/flipY are not implemented if blit is not used
void winCopyFrameBufferDirect(int tex_target, int color_texture_handle, int depth_texture_handle, int x, int y, int width, int height, int miplevel, bool flipX, bool flipY)
{
    int blit_flags = 0;
    int bind_target = tex_target;
    GLuint savedactivetexture = 0;
    GLuint savedtexture = 0;
    PBuffer *pbuf = current_pbuffer;

    rdrBeginMarker(__FUNCTION__ ":%p", current_pbuffer);

    if (!(pbuf && pbuf->flags & PB_MIPMAPPED))
        miplevel = 0;

    if (bind_target >= GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB && bind_target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB)
        bind_target = GL_TEXTURE_CUBE_MAP_ARB;

    // color
    if (color_texture_handle)
    {
        if (pbuf && (pbuf->flags & PB_FBO) && miplevel == 0 && rdr_caps.supports_ext_framebuffer_blit)
        {
            blit_flags |= GL_COLOR_BUFFER_BIT;
        }
        else
        {
            WCW_BindTexture(bind_target, 0, color_texture_handle);
            // glCopyTexImage2D() will resize the destination texture which is not what we want here
            //glCopyTexImage2D(tex_target, miplevel, GL_RGBA, x, y, width, height, 0); CHECKGL;
            glCopyTexSubImage2D(tex_target, miplevel, 0, 0, x, y, width, height); CHECKGL;
        }
    }

    // depth
    if (depth_texture_handle)
    {
        if (pbuf && (pbuf->flags & PB_FBO) && miplevel == 0 && rdr_caps.supports_ext_framebuffer_blit)
        {
            blit_flags |= GL_DEPTH_BUFFER_BIT;
        }
        else
        {
            WCW_BindTexture(bind_target, 0, depth_texture_handle);
            // glCopyTexImage2D() will resize the destination texture which is not what we want here
            //glCopyTexImage2D(tex_target, 0, GL_DEPTH_COMPONENT24, x, y, width, height, 0); CHECKGL;
            glCopyTexSubImage2D(tex_target, 0, 0, 0, x, y, width, height); CHECKGL;
        }
    }

    // Make sure the depth format matches between source and destination
    if (blit_flags & GL_DEPTH_BUFFER_BIT)
    {
        int internal_format;

        WCW_EnableTexture( tex_target, 0 );
        WCW_BindTexture( bind_target, 0, depth_texture_handle );
        glGetTexLevelParameteriv(tex_target, 0, GL_TEXTURE_INTERNAL_FORMAT, &internal_format); CHECKGL;
        if (internal_format != pbuf->depth_internal_format)
        {
            setTextureParameters(tex_target, false, 0);
            glTexImage2D(tex_target, 0, pbuf->depth_internal_format, x+width, y+height, 0, pbuf->depth_format, pbuf->depth_type, NULL); CHECKGL;
        }
    }

    // Do any blitting required
    if (blit_flags)
    {
        int srcX1, srcX2, srcY1, srcY2;
        int dstX1, dstX2, dstY1, dstY2;

        assert(rdr_caps.supports_ext_framebuffer_blit);
        CHECK_SLI_FBO(current_pbuffer);

        srcX1 = dstX1 = x;
        srcX2 = dstX2 = x + width;
        srcY1 = dstY1 = y;
        srcY2 = dstY2 = y + height;

        if (flipX)
        {
            dstX1 = srcX2;
            dstX2 = srcX1;
        }

        if (flipY)
        {
            dstY1 = srcY2;
            dstY2 = srcY1;
        }

        glPushAttrib(GL_COLOR_BUFFER_BIT); CHECKGL;

        if (s_frame_grab_blit_fbo == 0) {
            glGenFramebuffersEXT(1, &s_frame_grab_blit_fbo); CHECKGL;
        }

        glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, s_frame_grab_blit_fbo); CHECKGL;

        if (blit_flags & GL_COLOR_BUFFER_BIT)
        {
            glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, tex_target, color_texture_handle, 0); CHECKGL;
            glReadBuffer(GL_COLOR_ATTACHMENT0_EXT); CHECKGL;
            glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT); CHECKGL;
        }
        else
        {
            glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, tex_target, 0, 0); CHECKGL;
            glReadBuffer(GL_NONE); CHECKGL;
            glDrawBuffer(GL_NONE); CHECKGL;
        }

        if (blit_flags & GL_DEPTH_BUFFER_BIT)
        {
            glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, tex_target, depth_texture_handle, 0); CHECKGL;
            if (pbuf->depth_format == GL_DEPTH_STENCIL) {
                blit_flags |= GL_STENCIL_BUFFER_BIT;
            }
        }
        else
        {
            glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, tex_target, 0, 0); CHECKGL;
        }

        if (blit_flags & GL_STENCIL_BUFFER_BIT) {
            glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, tex_target, depth_texture_handle, 0); CHECKGL;
        } else {
            glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, tex_target, 0, 0); CHECKGL;
        }

        CHECKFBO("winCopyFrameBufferDirect");

        glBlitFramebufferEXT(srcX1, srcY1, srcX2, srcY2, dstX1, dstY1, dstX2, dstY2, blit_flags, GL_NEAREST); CHECKGL;
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, current_pbuffer->fbo[current_pbuffer->curFBO]); CHECKGL;
        glPopAttrib(); CHECKGL;
    }

    WCW_ResetTextureState();

    rdrEndMarker();
}
