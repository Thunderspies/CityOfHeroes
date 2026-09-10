#define RT_PRIVATE
#define WCW_STATEMANAGER

#include <utilitieslib/utils/error.h>
#include "render/thread/ogl.h"
#include "render/thread/wcw_statemgmt.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/fileutil.h>
#include "render/thread/rt_state.h"
#include <utilitieslib/assert/assert.h>
#include "render/texEnums.h"
#include <utilitieslib/utils/mathutil.h>
#include "win/win_init.h" // For winMsgAlert
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/osdependent.h>
#include "render/thread/rt_cgfx.h"
#include "rt_shaderMgr.h"
#include "render/perfcounter.h"
#include "rt_report.h"


RenderCaps    rdr_caps;

void rdrInitDirect(void)
{
    glClearColor( 0.0, 0.0, 0.0, 0.0 ); CHECKGL;

    /* set viewing projection */
    WCW_LoadProjectionMatrixIdentity();
    // DGNOTE 4/27/2009 - This sets the near clip to 4.0, although this is not #defined anywhere.
    // I need to know this so I can place cameraspace FX at the near clip.  Therefore if you ever
    // change the near zNear parameter (#5), look for a DGNOTE with the same date tag in fxgeo.c
    // and adjust the Vec3 there.
    glFrustum(-4.0f, 4.0f, -2.704225f, 2.704225f, 4.0f, 4096.0f); CHECKGL;

    WCW_LoadColorMatrixIdentity();
    if(rdr_caps.num_texunits > 0)
        WCW_LoadTextureMatrixIdentity(0);
    if(rdr_caps.num_texunits > 1)
        WCW_LoadTextureMatrixIdentity(1);
    WCW_LoadModelViewMatrixIdentity();

    /* position viewer */
    glCullFace(GL_BACK); CHECKGL;
    glFrontFace(GL_CW); CHECKGL;
    glEnable(GL_CULL_FACE); CHECKGL;

    glDepthFunc(GL_LEQUAL); CHECKGL;
    glEnable(GL_DEPTH_TEST); CHECKGL;
// wcw    glEnable(GL_VERTEX_WEIGHTING_EXT); CHECKGL; //mm 

    glShadeModel(GL_SMOOTH); CHECKGL;

    WCW_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    WCW_EnableBlend();
    glEnable(GL_ALPHA_TEST); CHECKGL;
    glAlphaFunc(GL_GREATER,0 ); CHECKGL;

    WCW_Fog(1);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); CHECKGL;

WCW_EnableTexture(GL_TEXTURE_2D, 0);
glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); CHECKGL;

    WCW_Color4(255,255,255,255);
    glEnable(GL_LIGHT0); CHECKGL;
    glEnable(GL_LIGHT1); CHECKGL; // GGFIXME: remove this?
    WCW_EnableColorMaterial();
    // wcw glEnable( GL_COLOR_MATERIAL ); CHECKGL;
}

//#define OGL_OVERRIDE_FILE "C:\\temp\\NV_6150.txt"
#ifdef OGL_OVERRIDE_FILE
// Enable this to load a list of OpenGL extensions from an external file
bool extInitCheck(const char *extension, bool windows_extension)
{
    char stringToFind[1024];
    char buffer[1024];
    FILE *file;
    bool found = false;

    if (windows_extension)
    {
        sprintf(stringToFind, "WGL_%s", extension);
    }
    else
    {
        sprintf(stringToFind, "GL_%s", extension);
    }


    if ((file = fopen(OGL_OVERRIDE_FILE, "rtS")) != NULL)
    {
        while(fgets(buffer, 1024, file) != NULL)
        {
            // Odd, I'm seeing the line-feed at the end of the buffer
            int len = strlen(buffer);
            if (buffer[len - 1] == 10)
                buffer[len - 1] = 0;

            if (strcmp(buffer, stringToFind) == 0)
            {
                found = true;
                break;
            }
        }

        fclose(file);
    }

    return found;
}
#define extInit(extension,fatal) ((GLEW_##extension && extInitCheck(#extension, false)) ? 1 : (fatal ? (extInitFail(#extension), 0) : 0))
#define extInitW(extension,fatal) ((WGLEW_##extension && extInitCheck(#extension, true)) ? 1 : (fatal ? (extInitFail(#extension), 0) : 0))

#else

#define extInit(extension,fatal) (GLEW_##extension ? 1 : (fatal ? (extInitFail(#extension), 0) : 0))
#define extInitW(extension,fatal) (WGLEW_##extension ? 1 : (fatal ? (extInitFail(#extension), 0) : 0))

#endif

void extInitFail(char *extension)
{
    writeConsole(OUTPUT_VERBOSE, "Your video driver is missing support for %s", extension);
}

void rebuildCachedShadersCallbackDirect(const char* relpath)
{
    reloadShaderCallbackDirect(relpath);
}

char reloading_shader[MAX_PATH];
void reloadShaderCallbackDirect(const char *relpath)
{
    shaderMgr_SetOverrideShaderDir();
    if (relpath )
    {
        // make sure this is a file we care about
        if( strEndsWith(relpath, ".bak") ||
            (rt_cgGetCgShaderMode() && !(strEndsWith(relpath, ".cg") || strEndsWith(relpath, ".cgh") || strEndsWith(relpath, ".cgfx")))  || 
            (!rt_cgGetCgShaderMode() && !(strEndsWith(relpath, ".fp") || strEndsWith(relpath, ".fph") || strEndsWith(relpath, ".vp")))
        )
        {
            return;
        }

        if ( strEndsWith(relpath, ".cgh"))
        {
            shaderMgr_ClearShaderCache();
        }
    }

    // Clear the Cg 'virtual file system' (VFS) so it will get
    // updated includes from the callback system or disk
    // @todo - where is the best place to put this?
//    cgSetCompilerIncludeFile( rt_cgfxGetGlobalContext(), NULL, NULL ); // <-- This CRASHES in the Cg DLL
    cgSetCompilerIncludeString(  rt_cgfxGetGlobalContext(), NULL, "" );        // <--this one works as advertised to clear the VFS

    if (relpath) {
        fileWaitForExclusiveAccess(relpath);
        errorLogFileIsBeingReloaded(relpath);
        strcpy(reloading_shader, relpath);
        if (strEndsWith(relpath, ".cgfx")) {
            rt_cgfxReloadEffect(reloading_shader);
        }
    } else {
        reloading_shader[0]=0;
    }
    
    shaderMgr_ReleaseShaderIds();

    modelBlendStateInit();
    bumpInit();
}

char *rdrChipToString(int chipbit) {
    static struct {
        int bitmask;
        char *name;
    } mapping[] = {
#define CHIPNAME(a) {a, #a },
        CHIPNAME(TEX_ENV_COMBINE)
        CHIPNAME(ARBVP)
        CHIPNAME(ARBFP)
        CHIPNAME(GLSL)
        CHIPNAME(DX10_CLASS)
        CHIPNAME(NV1X)
        CHIPNAME(NV2X)
        CHIPNAME(NV3X)
        CHIPNAME(NV4X)
        CHIPNAME(R200)
        CHIPNAME(R300)
        CHIPNAME(R430)
    };
    int i;
    for (i=0; i<ARRAY_SIZE(mapping); i++) {
        if (chipbit & mapping[i].bitmask) {
            assert(chipbit == mapping[i].bitmask); // It's only supposed to take one bit, not multiple
            return mapping[i].name;
        }
    }
    return NULL;
}

char *rdrFeatureToString(int featurebit) {
    static struct {
        int bitmask;
        char *name;
    } mapping[] = {
#define FEATURENAME(a) {GFXF_##a, #a },
        FEATURENAME(WATER)
        FEATURENAME(BLOOM)
        FEATURENAME(TONEMAP)
        FEATURENAME(MULTITEX)
        FEATURENAME(MULTITEX_DUAL)
        FEATURENAME(MULTITEX_HQBUMP)
        FEATURENAME(FPRENDER)
        FEATURENAME(WATER_DEPTH)
        FEATURENAME(DOF)
        FEATURENAME(HQBUMP)
        FEATURENAME(BUMPMAPS)
        FEATURENAME(BUMPMAPS_WORLD)
        FEATURENAME(DESATURATE)
        FEATURENAME(AMBIENT)
        FEATURENAME(CUBEMAP)
        FEATURENAME(SHADOWMAP)
        FEATURENAME(WATER_REFLECTIONS)
    };
    int i;
    STATIC_INFUNC_ASSERT(ARRAY_SIZE( mapping ) == GFXF_NUM_FEATURES);

    for (i=0; i<ARRAY_SIZE(mapping); i++) {
        if (featurebit & mapping[i].bitmask) {
            assert(featurebit == mapping[i].bitmask); // It's only supposed to take one bit, not multiple
            return mapping[i].name;
        }
    }
    return NULL;
}

//Here go render stuff that don't need to be recalled when gfx reload is called
void rdrInitExtensionsDirect()
{    
    int failure = 0;
    int required = 1;
    bool isNvidia = false;
    bool isATI = false;
    const char* vendor;

    if(game_state.create_bins)
        required = 0;

    INEXACT_CHECKGL;

    ////////// Initialize stuff all extensions should support

    if( !extInit(ARB_multitexture,required) )
        failure |= 1;
    if( !extInit(ARB_texture_compression,required) )
        failure |= 1;

    // Sanity Check : check for a few OpenGL functions
    if (!glActiveTextureARB || !glClientActiveTextureARB)
        failure |= 1;

    ///////// Figure out what we've got //////////////////////////////////////////////
    rdr_caps.chip = 0;
    rdr_caps.rgbbuffers_as_floats_hack = 0;

    if (( vendor = glGetString(GL_VENDOR)) != NULL )
    {
        if (strstr(vendor, "ATI Technologies Inc."))
        {
            isATI = true;

            // Macintosh R200s will fail this test.  The R200 flag specifies the
            // ATI_fragment_shader rendering path in the absence of ARBFP and also
            // specifies generic Radeon-specific tweaks.  This may potentially cause
            // problems for that specific configuration.
            if (extInit(ATI_fragment_shader, 0))
                rdr_caps.chip |= R200; // ATI 9000+ class

            // It's safe to restore the R200 flag here because we won't use the
            // ATI_fragment_shader path.
            if (extInit(ARB_depth_texture, 0))
                rdr_caps.chip |= R200 | R300; // ATI 9700+ class

            if (extInit(ATI_texture_compression_3dc, 0))
                rdr_caps.chip |= R430; // ATI X800+ class
        }
        else if (strstr(vendor, "NVIDIA Corporation"))
        {
            isNvidia = true;

            // NV1X and NV2X serve the dual purposes of specifying a render path and
            // also flagging nVidia specific behavior in other paths.  Treat nVidia
            // cards under MacOS as generic graphics cards until and unless nVidia
            // needs special attention under MacOS.
            if (extInit(NV_register_combiners,0) && 
                extInit(NV_vertex_program,0))
            {
                rdr_caps.chip |= NV1X;

                if (extInit(NV_register_combiners2,0))
                    rdr_caps.chip |= NV2X;

                if (extInit(NV_vertex_program2,0))
                    rdr_caps.chip |= NV3X;

                if (extInit(NV_vertex_program3,0))
                    rdr_caps.chip |= NV4X;
            }
        }
    }
    if (extInit(ARB_shading_language_100,0) && extInit(ARB_shader_objects,0) && extInit(ARB_fragment_shader,0) && extInit(ARB_vertex_shader,0))
        rdr_caps.chip |= GLSL;

    // now that we know which chipset is valid, init performance counters for that chipset
    //perfCounterInit(rdr_caps.chip & (NV1X | NV2X), rdr_caps.chip & (R200 | R300 | R430));

    // Use the recently defined RGTC and LATC texture compression modes to detect a DX10 class GPU
    if (extInit(EXT_texture_compression_rgtc,0) || extInit(EXT_texture_compression_latc,0))
        rdr_caps.chip |= DX10_CLASS;

    if (extInit(ARB_vertex_program, 0))
    {
        rdr_caps.chip |= ARBVP;

        if (extInit(ARB_fragment_program, 0))
        {
            rdr_caps.chip |= ARBFP;
            
            // No ARBFP with GeForce FX (5XXX) cards.  Possibly because of poor performance.  Fall back to NV2X instead.
            // fpe 12/6/09 -- verified that framerate sucks if we let FX5000 series card run with ARBFP.
            // fpe 3/7/2011 -- disable this check for mac now that simplified graphics logic is in place
            if( isNvidia && !IsUsingCider() && !(rdr_caps.chip & NV4X) ) // not at least a GeForce 6XXX
            {
                printf("Disabling ARBFP on GeForce FX (5XXX) because of poor performance\n");
                rdr_caps.chip &= ~(ARBFP|GLSL);
            }
        }
    }
    
    if (!(rdr_caps.chip & (ARBFP|NV1X|NV2X|R200)) &&
        extInit(ARB_texture_env_combine, 0))
    {
        rdr_caps.chip |= TEX_ENV_COMBINE;
    }

    if (!extInit(ARB_vertex_buffer_object, 0)) {
        game_state.noVBOs = 1;
    }

    if (extInit(ARB_texture_rectangle, 0))
        rdr_caps.supports_arb_rect = 1;
    if (extInit(ARB_texture_non_power_of_two, 0))
        rdr_caps.supports_arb_np2 = 1;

    // N.B. AMD drivers have a "CityOfHeroes" exclusion on reporting the ARB_texture_non_power_of_two extension.
    // This is true up through Catalyst 11.2 (8.821 video driver), the most recent that I have checked
    // However, the feature is still present and operational.
    // As ARB_texture_non_power_of_two is a required part of core OpenGL 2.0 we should be able to check for that instead.
    if (!rdr_caps.supports_arb_np2 && isATI && extInit(VERSION_2_0, 0))
        rdr_caps.supports_arb_np2 = 1;

    if ((extInit(VERSION_2_0, 0) && rdr_caps.supports_arb_rect) || rdr_caps.supports_arb_np2)
        rdr_caps.partially_supports_arb_np2 = 1;
    if (extInitW(ARB_pixel_format_float, 0))
        rdr_caps.supports_pixel_format_float = 1;

    // From http://www.opengl.org/wiki/Floating_point_and_mipmapping_and_filtering
    if ((isNvidia && extInit(NV_vertex_program3, 0)) || (isATI && extInit(ATI_shader_texture_lod, 0)))
        rdr_caps.supports_pixel_format_float_mipmap = 1;

    if (extInitW(ARB_render_texture, 0))
        rdr_caps.supports_render_texture = 1;

    if (extInitW(ARB_pbuffer, 0))
        rdr_caps.supports_pbuffers = 1;
    else
        game_state.noPBuffers = 1;

    if (extInitW(ARB_multisample, 0))
        rdr_caps.supports_multisample = 1;
    if (extInit(EXT_framebuffer_multisample, 0))
        rdr_caps.supports_ext_framebuffer_multisample = 1;
    if (extInit(EXT_framebuffer_blit, 0))
        rdr_caps.supports_ext_framebuffer_blit = 1;
    if (extInit(EXT_texture_filter_anisotropic, 0))
        rdr_caps.supports_anisotropy = 1;
    if (extInit(EXT_framebuffer_object, 0))
        rdr_caps.supports_fbos = 1;
    if (extInit(EXT_blend_minmax, 0) && extInit(EXT_blend_subtract, 0))
        rdr_caps.supports_subtract = 1;
    if (extInit(EXT_packed_depth_stencil, 0))
        rdr_caps.supports_packed_stencil = 1;
    if (extInit(ARB_occlusion_query, 0))
        rdr_caps.supports_occlusion_query = 1;
    if (extInit(ARB_shadow, 0))
        rdr_caps.supports_arb_shadow = 1;
    if (extInit(EXT_direct_state_access, 0))
        rdr_caps.supports_direct_state_access = 1;

    if (extInit(NV_fragment_program2, 0))
        rdr_caps.supports_cg_fp40 = 1;

    INEXACT_CHECKGL;

    // Check to see how many texture units and tex coords the card supports (16 on GF6+, and all ARBfp cards?)
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &rdr_caps.num_texunits); CHECKGL;

    // Otherwise texids[] and other things are the wrong size
    STATIC_INFUNC_ASSERT(TEXLAYER_MAX_LAYERS <= MAX_TEXTURE_UNITS_TOTAL);
    STATIC_INFUNC_ASSERT(TEXLAYER_ALL_LAYERS == MAX_TEXTURE_UNITS_TOTAL);

    rdr_caps.num_texunits        = MIN(rdr_caps.num_texunits, MAX_TEXTURE_UNITS_TOTAL);
    rdr_caps.max_layer_texunits    = MIN(rdr_caps.num_texunits, TEXLAYER_MAX_LAYERS);    // Max texture units that can be used by material specs on this HW platform

    glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &rdr_caps.num_texcoords); CHECKGL;
    rdr_caps.num_texcoords        = MAX(2, rdr_caps.num_texcoords);

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &rdr_caps.max_texture_size); CHECKGL;

    // check programmable GPU implementation limits
    if (rdr_caps.chip & ARBFP)
    {
        glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &rdr_caps.max_program_local_parameters); CHECKGL;
        glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, &rdr_caps.max_program_env_parameters); CHECKGL;
        glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB, &rdr_caps.max_program_native_attribs); CHECKGL;  
        glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB, &rdr_caps.max_program_native_instructions); CHECKGL;
        glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB, &rdr_caps.max_program_native_temporaries); CHECKGL;
        
    }
    else
    {
        rdr_caps.max_program_local_parameters = 0;
        rdr_caps.max_program_env_parameters = 0;
        rdr_caps.max_program_native_attribs = 0;
        rdr_caps.max_program_native_instructions = 0;
        rdr_caps.max_program_native_temporaries = 0;
    }

    if (rdr_caps.supports_anisotropy) {
        float largest_supported_anisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy); CHECKGL;
        rdr_caps.maxTexAnisotropic = largest_supported_anisotropy;
    }

    rdr_caps.max_samples = 1;
    if (rdr_caps.supports_multisample)
    {
        // There does not appear to be a simple way to finde the maximum number of samples
        // supported when GL_MULTISAMPLE_ARB is supported
        rdr_caps.max_samples = 4;
    }

    rdr_caps.max_fbo_samples = 1;
    if (rdr_caps.supports_ext_framebuffer_multisample)
    {
        glGetIntegerv(GL_MAX_SAMPLES_EXT, &rdr_caps.max_fbo_samples); CHECKGL;

        if (rdr_caps.max_fbo_samples < 1)
        {
            rdr_caps.max_fbo_samples = 1;
        }
    }

    rdr_caps.bad_card_or_driver = failure;

    setAssertOpenGLReport(makeReportFile());
}

extern FogContext main_fog_context;

#include "win/win_init.h"
void rdrInitOnceDirect()
{
    // Clear the shader cache on disk if safemode is enabled
    if (game_state.safemode)
    {
        shaderMgr_ClearShaderCache();
    }

    WCW_InitOnce();
    WCW_InitFogContext(&main_fog_context);
    rdrInitDirect();
    WCW_Fog(0);
    shaderMgr_PrepareShaderCache();
    modelBlendStateInit();
    bumpInit();
    windowSize(&rdr_view_state.width,&rdr_view_state.height);
}

