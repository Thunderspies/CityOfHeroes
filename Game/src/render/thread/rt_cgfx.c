/****************************************************************************
    rt_cgfx.c

    Base support for CGFX.
    
    Author: LStLezin, 01/2009
****************************************************************************/

#include "render/thread/ogl.h"
#include <utilitieslib/utils/SuperAssert.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/mathutil.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <utilitieslib/utils/textparser.h>
#include "render/thread/wcw_statemgmt.h"
#include "rt_cgfx_statemgmt.h"
#include "render/thread/rt_cgfx.h"
#include "render/thread/rt_ssao.h"
#include "rt_shaderMgr.h"
#include "rt_filter.h"
#include <utilitieslib/utils/utils.h>
#include "graphics/failtext.h"

//---------------------------------------------------------------------------
//
//    Typedefs and static variables
//
//---------------------------------------------------------------------------

#define kEffectCacheInitalSz                32

#define CGFX_SHADER_FILE_EXT                ".cgfx"

// If 1, we use Cg CG_DEFERRED_PARAMETER_SETTING
#define CGFX_USE_DEFERRED_PARAMETER_SETTING 1

// We log cases where a shader was requested but not bound. Typically, that
// means a second call was made to rt_cgBindCgProgram() before the first call
// was acted upon in rt_cgFinalShaderPrep().
#define CGFX_TRACK_UNHANDLED_PENDING_BINDS    SHADER_DBG_CODE_ENABLED && 0


#if CGFX_USE_DEFERRED_PARAMETER_SETTING
    #define kCgFxParamSettingMode                CG_DEFERRED_PARAMETER_SETTING
    #define CGFX_UPDATE_PROGRAM_PARAMS(_pgm_)    if (_pgm_) cgUpdateProgramParameters(_pgm_)
#else
    #define kCgFxParamSettingMode                CG_IMMEDIATE_PARAMETER_SETTING
    #define CGFX_UPDATE_PROGRAM_PARAMS(_pgm_)
#endif

#define kBadTexLayer                        TEXLAYER_MAX_LAYERS

#define kBadParamKey                        -1

#define    kVirtualShader_NotSet                0
#define kVirtualShader_UseFixedFunction        1

#define VALID_SHADER_PGM(_hdl_)                (((_hdl_)!=kVirtualShader_NotSet)&&((_hdl_)!=kVirtualShader_UseFixedFunction))

static struct {
    const char*            shaderProfileName;
    tCgShaderProfile    nProfileConst;
    const char*            compilerParams;    // additional params to pass to the shader compiler (TAB separated list)
} sShaderProfileTbl[] = {
    { "fp30",    CG_PROFILE_FP30,    "-DPROFILE_FP30"  },
    { "vp30",    CG_PROFILE_VP30,    "-DPROFILE_VP30"  },
    { "fp40",    CG_PROFILE_FP40,    "-DPROFILE_FP40"  },
    { "vp40",    CG_PROFILE_VP40,    "-DPROFILE_VP40"  },
    { "gpu_fp",    CG_PROFILE_GPU_FP,    "-DPROFILE_NV8_GPU_FP"  },
    { "gpu_vp",    CG_PROFILE_GPU_VP,    "-DPROFILE_NV8_GPU_VP"  },
    { "glslf",    CG_PROFILE_GLSLF,    "" },
    { "glslv",    CG_PROFILE_GLSLV,    "" },
    { "arbfp1",    CG_PROFILE_ARBFP1,    "" },
    { "arbvp1",    CG_PROFILE_ARBVP1,    "" }
};


static bool ValidateCgProfileConstant( tCgShaderProfile profile );


//---------------------------------------------------------------------------
//
//    A collection of data about a paticular effect defintion.
//
//---------------------------------------------------------------------------
#define kMaxShaderParams    32
#define kParamNameBuffSz    128

//---------------------------------------------------------------------------
//    tCgEffectSpec - Describes CgFX effect
//---------------------------------------------------------------------------
typedef struct {
    char*            szEffectNm;        // malloc'd
    CGeffect        hEffect;
    CGtechnique        hTechnique;
} tCgEffectSpec;

static StashTable        sEffectCache            = NULL;        // contains tCgEffectSpec
static CGcontext        sCgContext                = NULL;
static tCgEffectSpec*    sCurrentEffectInst        = NULL;
static CGerror            sLastCgError            = CG_NO_ERROR;
static bool                sDisplayWarnings        = true;

tCgShaderMode g_rtCgFX_CgShaderMode = tCgShaderMode_Disabled;

//---------------------------------------------------------------------------
//
//    Static function declarations
//
//---------------------------------------------------------------------------
static void                CgProgramIdTable_Init();
static void                CgProgramIdTable_ReleaseMem();
static void                CgProgramIdTable_OnReset();

static void                OnCgErrorCB( void );
static tCgFxError        CheckForCgError(const char *situation, const char** ppErrStr);
static tCgFxError        ReviewShaderCompileResults( const char* szProgramName, CGprogram prog );
static void                CheckAndReportCgErrors( const char* szProgramName, const char* szActionDescr /*e.g., "compiling" */, CGprogram prog, tCgFxError* pRslt, bool* pWarnings );
static void                SaveProgramAssembly( const char* szProgramName, CGprogram pgm );
static void                UpdateOpenGLStateStack( bool bPush );
static void                PushOpenGLStateStack()        { UpdateOpenGLStateStack( true ); }
static void                PopOpenGLStateStack()        { UpdateOpenGLStateStack( false ); }

//
// Static functions supporting the use of CgFX effects and passes
//
static tCgFxError        cgfx_InitEffect( tCgEffectSpec* pEffectSpec /*OUT*/ );
static tCgFxError        cgfx_InitEffectHandle( tCgEffectSpec* pEffectSpec /*OUT*/, const char** args /* compile args or NULL */  );
static void                cgfx_DeInitEffect( tCgEffectSpec* pSpec );
static void                cgfx_DeInitEffectHandle( tCgEffectSpec* pSpec );
static void                cgfx_InitEffectCache( void );
static tCgEffectSpec*    cgfx_QueryEffectCache( const char* szEffectNm, bool bAddIfMissing );
static tCgEffectSpec*    cgfx_FindEffectByName( const char* szEffectNm );
static tCgFxError        cgfx_PreCompilePrograms( CGtechnique technique );
static void                cgfx_NotifyCgFxClientsOfEffectLoad( const char* szEffectName );
static void                cgfx_EffectNameToCgfxFullPath( const char* szName, char* pathBuff, size_t nBuffSz );
static void                cgfx_RelativePathToEffectName( const char* szRelPath, char* nameBuff, size_t nBuffSz );
static void                cgfx_KillEffectCache( void );
static void                cgfx_DestroyEffectSpecPtr( tCgEffectSpec** ppSpec );    // sets ppSpec to NULL, too.
static U32                cgfx_GetEffectId( const char* szEffectName );
static CGpass            cgfx_GetPassByIndex( CGtechnique technique, int nIndex );
static tCgFxError        cgfx_EnablePass( CGpass pass );
static void                cgfx_DisablePass( CGpass pass );
static tCgFxError        cgfx_SetEffect( const char* szEffectNm );




/****************************************************************************
    rt_cgfxGetGlobalContext
    
    Creates it, if it hasn't already been created.
****************************************************************************/
CGcontext rt_cgfxGetGlobalContext( void )
{
    if ( sCgContext == NULL )
    {
        sCgContext = cgCreateContext();
    }
    return sCgContext;
}

/****************************************************************************
    CGIncludeCallback

    Callback to handle includes, since cg doesn't know how to read our pigg
    file system.
****************************************************************************/
static void CGIncludeCallback( CGcontext context, const char *filename )
{
    // read the file into a buffer and send back.  tbd, need to clean up buffers.
    char partialPath[2048];
    const char* fileNameNoLeadSlash = filename;
    char *source = NULL;

    if(*filename == '/')
        fileNameNoLeadSlash = filename+1;
    #ifndef FINAL
        if ( game_state.use_prev_shaders ) {
            _snprintf( partialPath, sizeof(partialPath), RT_CGFX_SHADER_PATH_BASE "/prev/%s", fileNameNoLeadSlash );
        } else
    #endif
    _snprintf( partialPath, sizeof(partialPath), RT_CGFX_SHADER_PATH_BASE "/%s", fileNameNoLeadSlash );

    shaderMgr_GetShaderSrcFileTextCG(partialPath, &source);
    
    if(!source) {
        // try effects subdir (since cg doesn't tell us what source dir it is currently parsing)
        _snprintf( partialPath, sizeof(partialPath), RT_CGFX_SHADER_PATH_EFFECTS "/%s", fileNameNoLeadSlash );
        shaderMgr_GetShaderSrcFileTextCG(partialPath, &source);
    }

    if(source)
    {
        cgSetCompilerIncludeString( rt_cgfxGetGlobalContext(), filename, source);
        fileFree(source);
    }
}

/****************************************************************************
    rt_cgfxInit
    
    Initialize CGFX support
****************************************************************************/
tCgFxError rt_cgfxInit( void )
{
    rt_cgfxClearLastError();
    cgSetErrorCallback( OnCgErrorCB );

    assertmsg(( sCgContext == NULL ), "Duplicate CGFX initialization" ); 
    rt_cgfxGetGlobalContext();    // creates it, if it hasn't already been created
    
    // Borrowed from NVIDIA cgfx_bumpdemo2.c
    //
    //    "First we create a Cg context, register the standard states (if you fail to do this, your Cg
    //    context will lack the standard states needed for processing standard 3D API state
    //    assignments—alternatively you could load your own implementations of the standard
    //    CgFX states), and request that the Cg runtime manage texture binds (saving your
    //    application the trouble when applying techniques)."
    //
    cgGLSetDebugMode( CG_FALSE );
    cgSetParameterSettingMode(sCgContext, kCgFxParamSettingMode );
    rt_cgfx_statemgmt_init(rt_cgfxGetGlobalContext());
    
    //
    // But... from the Cg docs:
    //
    //        "By default, cgGL does not manage any texture state in OpenGL.
    //        It is up to the user to enable and disable textures using 
    //        cgGLEnableTextureParameter and cgGLDisableTextureParameter
    //        respectively. This behavior is the default in order to avoid
    //        conflicts with texture state on geometry that’s rendered with the
    //        fixed function pipeline or without cgGL.
    //
    //        "If automatic texture management is desired, cgGLSetManageTextureParameters
    //        may be called with flag set to CG_TRUE before cgGLBindProgram is called. 
    //        Whenever cgGLBindProgram is called, the cgGL runtime will make all the 
    //        appropriate texture parameter calls on the application’s behalf."
    //
    //cgGLSetManageTextureParameters(sCgContext, CG_TRUE);

    // Prevent Cg from automatically compiling shaders, that way we make sure to
    // compile shaders on the load screen and not when the camera is moving and
    // sensitive to framerate spikes.
    cgSetAutoCompile(sCgContext, CG_COMPILE_MANUAL);
    
    cgfx_InitEffectCache();
    
    CgProgramIdTable_Init();

    // set callback for parsing include files 
    cgSetCompilerIncludeCallback( sCgContext, CGIncludeCallback );

    return CheckForCgError("rt_cgfxInit",NULL);
}

/****************************************************************************
    OnCgErrorCB
    
    Cg error callback.
****************************************************************************/
static void OnCgErrorCB( void )
{
    sLastCgError = cgGetError();
    if ( sLastCgError != CG_NO_ERROR )
    {
        const char* szErrMsg = cgGetErrorString( sLastCgError );
        printf( "%s\n", szErrMsg );    
    }
}

/****************************************************************************
    rt_cgfxClearLastError
    
****************************************************************************/
void rt_cgfxClearLastError( void )
{
    sLastCgError = CG_NO_ERROR;
}

/****************************************************************************
    rt_cgfxGetLastCgError
    
****************************************************************************/
int rt_cgfxGetLastCgError( void )
{
    return sLastCgError;
}

/****************************************************************************
    rt_cgfxSetDisplayWarnings
    
****************************************************************************/
void rt_cgfxSetDisplayWarnings( bool bShowWarnings )
{
    sDisplayWarnings = bShowWarnings;
}

/****************************************************************************
    rt_cgfxGetDisplayWarnings
    
****************************************************************************/
bool rt_cgfxGetDisplayWarnings( void )
{
    return sDisplayWarnings;
}

/****************************************************************************
    rt_cgfxDeInit
    
    Deinitialize CGFX support
****************************************************************************/
tCgFxError rt_cgfxDeInit( void )
{
    cgfx_KillEffectCache();
    CgProgramIdTable_ReleaseMem();
    if ( sCgContext != NULL )
    {
        cgSetCompilerIncludeCallback( sCgContext, NULL );
        cgDestroyContext( sCgContext);    
        sCgContext = NULL;
    }
    cgSetErrorCallback( NULL );
    return CheckForCgError("rt_cgfxDeInit",NULL);
}

/****************************************************************************
    rt_cgfxReloadEffect
    
****************************************************************************/
void rt_cgfxReloadEffect( const char* szCgFxRelPath )
{
    tCgEffectSpec* pSpec = NULL;
    //
    //    Realoding an effect presents a couple of problems which have been
    //    solved (I think.... :) ) by giving index values, rather than Cg
    //    handles to the effect instance items. That allows us to change
    //    the param handle (by reloading) without invalidating param handles
    //    in the instance data stored outside this module in the tricks.
    //
    //    But when we reload the effect we have to make sure we re-use the
    //    same slots. We use the param name to match old and new.
    //
    char effectName[256];
    cgfx_RelativePathToEffectName( szCgFxRelPath, effectName, ARRAY_SIZE(effectName) );
    
    pSpec = cgfx_FindEffectByName( effectName );
    if ( pSpec != NULL )
    {
        cgfx_DeInitEffect( pSpec );
        cgfx_InitEffect( pSpec );
        cgfx_NotifyCgFxClientsOfEffectLoad( effectName );
    }
}

/****************************************************************************
    cgfx_NotifyCgFxClientsOfEffectLoad
    
****************************************************************************/
static void cgfx_NotifyCgFxClientsOfEffectLoad( const char* szEffectName )
{
    // SSAO
    rdrHandleEffectReloadDirect( szEffectName );
    // FILTER
    rdrFilterHandleEffectReloadDirect(szEffectName);
}

/****************************************************************************
    UpdateOpenGLStateStack
    
****************************************************************************/
static void UpdateOpenGLStateStack( bool bPush )
{
    static int snStackSize = 0;
    if ( bPush )
    {
        assert( snStackSize == 0 );
        // See http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/pushattrib.html
        // for list of bits and what they push
        glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT | GL_FOG_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT); CHECKGL;
        snStackSize++;
    }
    else
    {
        assert( snStackSize == 1 );
        glPopAttrib(); CHECKGL;
        snStackSize--;
    }
}

/****************************************************************************
    cgfx_GetPassByIndex
    
****************************************************************************/
static CGpass cgfx_GetPassByIndex( CGtechnique technique, int nIndex )
{
    int passI = 0;
    tCgFxError nError = kCgFxError_NONE;
    CGpass pass = cgGetFirstPass(technique);
    while (( passI < nIndex ) && ( pass != NULL ))
    {
        pass = cgGetNextPass( pass );
        passI++;
    }
    if ( pass == NULL ) 
    {
        failText( "Bad render pass index: %d.\n", nIndex );
    }
    return pass;
}

/****************************************************************************
    cgfx_EnablePass
    
    Makes the Cg program active.
****************************************************************************/
static tCgFxError cgfx_EnablePass( CGpass pass )
{
    cgSetPassState(pass);
    return CheckForCgError( "cgfx_EnablePass", NULL );
}

/****************************************************************************
    cgfx_DisablePass
    
****************************************************************************/
static void cgfx_DisablePass( CGpass pass )
{
    cgResetPassState( pass );
    PopOpenGLStateStack();

    //
    // We need to make sure the state mechanism doesn't think that everything
    // is the same as it was before the previous piece of geo was drawn. We
    // may have messed around with it.
    //
    // If there are possible state problems, try calling the full WCW_ResetState()
    // instead of the stripped down WCW_ResetState_CgFx()
#if 1
    WCW_ResetState_CgFx();
#else
    WCW_SyncGLState();
#endif
}

/****************************************************************************
    cgfx_SetEffect
    
    Make this effect current.
****************************************************************************/
static tCgFxError    cgfx_SetEffect( const char* szEffectNm )
{
    tCgEffectSpec* pEffectSpec = cgfx_FindEffectByName( szEffectNm );
    if ( pEffectSpec == NULL )
    {
        assertmsg(( pEffectSpec != NULL ), "Calling for a non-existent effect." );
        return kCgFxError_GENERAL_FAILURE;
    }
    
    if ( pEffectSpec->hEffect == NULL )
    {
        cgfx_InitEffect( pEffectSpec );
    }

    sCurrentEffectInst = pEffectSpec;
    return kCgFxError_NONE;
}

/****************************************************************************
    CheckForCgError
    
    Query CG error state.
****************************************************************************/
static tCgFxError CheckForCgError(const char *situation, const char** ppErrStr )
{
    tCgFxError nLocalErrorCode = kCgFxError_NONE;
    if ( sLastCgError != CG_NO_ERROR ) 
    {
        const char* szErrorStr = cgGetErrorString( sLastCgError );
        if ( ppErrStr != NULL )
        {
            *ppErrStr = szErrorStr;
        }
        
        // CGFX_TBD - Map CGFX errors to more specific local enums?
        nLocalErrorCode = kCgFxError_GENERAL_FAILURE;
    }
    rt_cgfxClearLastError();
    return nLocalErrorCode;
}

/****************************************************************************
    rt_cgfxCompileProgram
    
    Report compile warnings and errors for the last compile operation.
****************************************************************************/
tCgFxError rt_cgfxCompileProgram( CGprogram prog )
{
    tCgFxError nError = kCgFxError_NONE;

    CGannotation ann;
    const char * szProgramName = NULL;
    const char * const * szProgramOptions = cgGetProgramOptions(prog);

    ann = cgGetNamedProgramAnnotation( prog, "name" );
    if ( ann )
    {
        szProgramName = cgGetStringAnnotationValue( ann );
    }
    else
    {
        szProgramName = "<failed-compile>";
    }

    assertmsg(!cgIsProgramCompiled(prog), "Program is already compiled.");

    cgCompileProgram(prog);
    nError = ReviewShaderCompileResults( szProgramName, prog );
    if (( game_state.saveCgShaderListing ) &&
        ( nError == kCgFxError_NONE ))
    {
        SaveProgramAssembly( szProgramName, prog );
    }
    
    return nError;
}

/****************************************************************************
    ReviewShaderCompileResults

****************************************************************************/
static tCgFxError ReviewShaderCompileResults( const char* szProgramName, CGprogram prog )
{
    tCgFxError nRslt = kCgFxError_NONE;
    bool bWarnings = false;
    
    CheckAndReportCgErrors( szProgramName, "compiling", prog, &nRslt, &bWarnings );
    if (( game_state.shaderInitLogging ) && ( nRslt == kCgFxError_NONE ) && ( ! bWarnings ))
    {
        const char * const * arg;
        const char * const * szProgramOptions = cgGetProgramOptions(prog);
        printf( "Compiled Cg/CgFX Shader '%s' ", szProgramName);
        for(arg = szProgramOptions; arg && *arg; arg++)
        {
            printf("%s ", *arg);
        }
        printf( "\n" );
    }
    return nRslt;    
}

/****************************************************************************
    CheckAndReportCgErrors

****************************************************************************/
static void CheckAndReportCgErrors( const char* szProgramName,
                                    const char* szActionDescr /*e.g., "compiling" */, 
                                    CGprogram prog, tCgFxError* pRslt, bool* pWarnings  )
{
    tCgFxError nError = kCgFxError_NONE;
    const char* szCompileError = NULL;
    const char * const * szProgramOptions = cgGetProgramOptions(prog);
    const char* szLastListing = cgGetLastListing(sCgContext);
    
    *pWarnings = false;
    if (( prog == NULL ) || (( nError = CheckForCgError(szProgramName,&szCompileError)) != kCgFxError_NONE ))
    {
        const char * const * arg;
        consoleSetColor(COLOR_RED|COLOR_BRIGHT, 0);
        printf( "Error %s CgFX Shader %s: %s\nOptions: ", 
                    szActionDescr, 
                    szProgramName, 
                    (( szCompileError == NULL ) ? "" : szCompileError) );
        for(arg = szProgramOptions; arg && *arg; arg++)
        {
            printf("%s ", *arg);
        }
        printf( "\n%s", ((szLastListing==NULL) ? "" : szLastListing) );
        consoleSetDefaultColor();
    }
    else if( szLastListing && *szLastListing )
    {
        const char * const * arg;
        if ( rt_cgfxGetDisplayWarnings() )
        {
            consoleSetColor(COLOR_RED|COLOR_GREEN|COLOR_BRIGHT, 0);
            if ( game_state.shaderInitLogging )
            {
                printf( "Cg/CgFX Shader %s generated warnings:\nOptions: ", szProgramName);
                for(arg = szProgramOptions; arg && *arg; arg++)
                {
                    printf("%s ", *arg);
                }
                printf( "\n%s", szLastListing );
            }
            else
            {
                printf( "Cg/CgFX Shader %s generated warnings.\n  Run with \"shader_init_logging 1\" for more info.\n", szProgramName);
            }
            consoleSetDefaultColor();
        }
        *pWarnings = true;
    }
    
    *pRslt = nError;
}

/****************************************************************************
    SaveProgramAssembly

****************************************************************************/
static void SaveProgramAssembly( const char* szProgramName, CGprogram pgm )
{
    char path[1024];
    FILE * f;
    getcwd( path, sizeof(path) );        
    _snprintf( path + strlen(path), sizeof(path) - strlen(path), "\\%s.lst", szProgramName );
    //printf( "Saving Assembly to '%s'\n", path);
    f = fopen( path, "wb" );

    // at least don't crash if the file can't be saved. we'll run with the
    // version we built in memory.
    if ( f )
    {
        fprintf( f, "%s", cgGetProgramString( pgm, CG_COMPILED_PROGRAM ) );
        fclose( f );
    }
}

/****************************************************************************
    cgfx_InitEffect

    Prepare the effect for use.    Handles lazy initialization.
****************************************************************************/
static tCgFxError cgfx_InitEffect( tCgEffectSpec* pEffectSpec )
{
    tCgFxError nError = kCgFxError_NONE;
    
    do {    // fake try/catch - always exits at the bottom
    
        //
        //    Check to see if we need to any work.
        //    
        if ( pEffectSpec->hEffect != NULL ) break;
        if (( nError = cgfx_InitEffectHandle( pEffectSpec, NULL )) != kCgFxError_NONE )    break;

        // find usable technique    
        pEffectSpec->hTechnique = cgGetFirstTechnique(pEffectSpec->hEffect);
        while (( pEffectSpec->hTechnique ) &&
                ( cgfx_PreCompilePrograms( pEffectSpec->hTechnique ) != kCgFxError_NONE ) &&
                ( cgValidateTechnique(pEffectSpec->hTechnique) == CG_FALSE ) ) 
        {
            pEffectSpec->hTechnique = cgGetNextTechnique(pEffectSpec->hTechnique);
        }
        if (pEffectSpec->hTechnique == NULL)
        {
            failText( "Can't find a usable 'technique' in CgFX file for '%s'.\n", pEffectSpec->szEffectNm );
            nError = kCgFxError_NoValidTechnique;
            break;
        }
    
    } while ( false );    // always exit
    
    if ( nError != kCgFxError_NONE )
    {
        failText( "Can't initialize CgFX effect.\n" );
    }
    
    return nError;
}

/****************************************************************************
    cgfx_PreCompilePrograms

****************************************************************************/
static tCgFxError cgfx_PreCompilePrograms( CGtechnique technique )
{
    tCgFxError nError = kCgFxError_NONE;
    CGeffect effect = cgGetTechniqueEffect(technique);

    //
    //    Make sure the programs compile and make diagnostic info
    //    available.
    //
    {
        CGpass pass = cgGetFirstPass(technique);
        bool bCompileErrors = false;
        while ( pass != NULL )
        {
            CGprogram pgm = NULL;
            CGdomain domainList[] = { CG_VERTEX_DOMAIN, CG_FRAGMENT_DOMAIN };
            int domainI;
            for ( domainI=0; domainI < ARRAY_SIZE(domainList); domainI++ )
            {
                if (( pgm = cgGetPassProgram( pass, domainList[domainI] )) != NULL )
                {
                    // Store a name on the program indicating the name for this effect_technique_pass_program
                    CGannotation ann;
                    char name[1024];
                    snprintf( name, sizeof(name), "%s_%s_%s_%s",
                        cgGetEffectName(effect), cgGetTechniqueName(technique), cgGetPassName(pass), cgGetProgramString(pgm, CG_PROGRAM_ENTRY) );
                    ann = cgCreateProgramAnnotation( pgm, "name", CG_STRING );
                    cgSetStringAnnotation( ann, name );

                    if ( ! cgIsProgramCompiled( pgm ) )
                    {        
                        if ( rt_cgfxCompileProgram( pgm ) != kCgFxError_NONE )
                        {
                            bCompileErrors = true;    // but keep going...
                        }
                    }
                }
            }
            pass = cgGetNextPass( pass );
        }
        if ( bCompileErrors )
        {
            nError = kCgFxError_BadEffectFormat;
        }
    }
    return nError;
}

/****************************************************************************
    cgfx_InitEffectHandle

****************************************************************************/
static tCgFxError cgfx_InitEffectHandle( tCgEffectSpec* pEffectSpec, const char** args /* compile args or NULL */  )
{
    tCgFxError nError = kCgFxError_NONE;
    
    const char* szCompileError = NULL;
    const char* szProgramOptions = NULL;
    const char* szLastListing = NULL;
    char fullCgfxPath[_MAX_PATH];

    //
    //    Check to see if we need to any work.
    //    
    if ( pEffectSpec->hEffect == NULL )
    {
        char    *source;
        //
        //    Load the effect from the file.
        //
        cgSetLastListing( sCgContext, "" );
        cgfx_EffectNameToCgfxFullPath( pEffectSpec->szEffectNm, fullCgfxPath, ARRAY_SIZE(fullCgfxPath) );
        source = fileAlloc(fullCgfxPath,0); 
        pEffectSpec->hEffect = cgCreateEffect(sCgContext, source, args );
        fileFree(source);

        cgSetEffectName( pEffectSpec->hEffect, pEffectSpec->szEffectNm );

        szLastListing = cgGetLastListing(sCgContext);
        if (( pEffectSpec->hEffect == NULL ) &&
            ( nError = CheckForCgError(pEffectSpec->szEffectNm,&szCompileError)) != kCgFxError_NONE )
        {
            const char * const * arg;
            consoleSetColor(COLOR_RED|COLOR_BRIGHT, 0);
            printf( "Error Compiling CgFX Shader '%s': %s\nOptions: ", pEffectSpec->szEffectNm, szCompileError );
            for(arg = args; (arg && *arg); arg++)
            {
                printf("%s ", *arg);
            }
            printf( "\n%s", ( szLastListing ) ? szLastListing : "" );
            consoleSetDefaultColor();
        }
        else if( szLastListing && *szLastListing )
        {
            const char * const * arg;
            consoleSetColor(COLOR_RED|COLOR_GREEN|COLOR_BRIGHT, 0);
            if ( game_state.shaderInitLogging )
            {
                printf( "Compiled CgFX Effect '%s' with warnings:\nOptions: ", pEffectSpec->szEffectNm);
                for(arg = args; (arg && *arg); arg++)
                {
                    printf("%s ", *arg);
                }
                printf("\n%s\n", szLastListing );
            }
            else
            {
                printf( "CgFX Effect '%s' generated warnings.\n  Run with \"shader_init_logging 1\" for more info.\n", pEffectSpec->szEffectNm);
            }
            consoleSetDefaultColor();
        }
        else
        {
            printf( "Compiled CgFX Shader '%s'\n", pEffectSpec->szEffectNm);
        }
    }
    
    return nError;
}

/****************************************************************************
    cgfx_DeInitEffectHandle
    
****************************************************************************/
static void cgfx_DeInitEffectHandle( tCgEffectSpec* pSpec )
{
    if ( pSpec->hEffect != NULL )
    {
        cgDestroyEffect( pSpec->hEffect );
        pSpec->hEffect = NULL;
        pSpec->hTechnique = NULL;
    }
}

/****************************************************************************
    cgfx_DeInitEffect
    
****************************************************************************/
static void cgfx_DeInitEffect( tCgEffectSpec* pSpec )
{
    // NOTE: See also, cgfx_DestroyEffectSpecPtr(), which also releases allocated
    // memory in the data struct...
    cgfx_DeInitEffectHandle( pSpec );    
}

/****************************************************************************
    cgfx_QueryEffectCache
    
    Look for this item in the list of loaded effects. Possibly add an
    entry if the item is not found.
****************************************************************************/
tCgEffectSpec* cgfx_QueryEffectCache( const char* szEffectNm, bool bAddIfMissing )
{
    tCgEffectSpec* pEffectSpec = cgfx_FindEffectByName( szEffectNm );
    if (( pEffectSpec == NULL ) && ( bAddIfMissing ))
    {
        pEffectSpec = calloc( 1, sizeof( tCgEffectSpec ) );
        
        //
        // This is a new entry
        //
        pEffectSpec->szEffectNm    = _strdup( szEffectNm );
        stashAddPointer( sEffectCache, (void*)szEffectNm, pEffectSpec, false );
    }
    return pEffectSpec;    
}

/****************************************************************************
    cgfx_FindEffectByName
    
****************************************************************************/
tCgEffectSpec* cgfx_FindEffectByName( const char* szEffectNm )
{
    StashElement elem;
    assertmsg(( sEffectCache != NULL ), "Searching effect cache before creating it." );
    if ( stashFindElement( sEffectCache, szEffectNm, &elem ) )
    {
        return (tCgEffectSpec*)stashElementGetPointer(elem);
    }
    return NULL;
}

/****************************************************************************
    cgfx_EffectNameToCgfxFullPath

    "foo" --> "c:/game/data/shaders/cgfx/foo.cgfx"
****************************************************************************/
static void cgfx_EffectNameToCgfxFullPath( const char* szName, char* pathBuff, size_t nBuffSz )
{
    char partialPath[_MAX_PATH];
    sprintf_s( partialPath, ARRAY_SIZE(partialPath), "%s/%s.cgfx",
        RT_CGFX_SHADER_PATH_BASE, szName );

    shaderMgr_MakeFullShaderSrcPath( partialPath, pathBuff, nBuffSz );
}

/****************************************************************************
    cgfx_RelativePathToEffectName
    
    "shaders/cgfx/foo.cgfx" --> "foo"
****************************************************************************/
static void cgfx_RelativePathToEffectName( const char* szRelPath, char* nameBuff, size_t nBuffSz )
{
    size_t basePathSz = strlen(RT_CGFX_SHADER_PATH_BASE);
    const char* pSrc = szRelPath;
    size_t resultSz = 0;
    
    if (( strStartsWith( pSrc, RT_CGFX_SHADER_PATH_BASE ) ) &&
        (( *(pSrc + basePathSz) == '/' ) ||
         ( *(pSrc + basePathSz) == '\\' )))
    {
        pSrc += ( basePathSz + 1 );
    }
    resultSz = strlen( pSrc );
    if ( strEndsWith(pSrc, CGFX_SHADER_FILE_EXT) )
    {
        resultSz -= strlen(CGFX_SHADER_FILE_EXT);
    }
    if ( resultSz < nBuffSz )
    {
        strncpy_s( nameBuff, nBuffSz, pSrc, resultSz );    
        nameBuff[resultSz] = '\0';
    }
    else
    {
        // assert( false ) ???
        *nameBuff = '\0';
    }
}

/****************************************************************************
    cgfx_InitEffectCache
    
****************************************************************************/
static void cgfx_InitEffectCache( void )
{
    assert( sEffectCache == NULL );
    sEffectCache = stashTableCreateWithStringKeys( 
                            kEffectCacheInitalSz, 
                            STASH_DEEP_COPY_KEY_NAMES_BIT 
                        );
}

/****************************************************************************
    cgfx_KillEffectCache
    
    Remove and release cached effects.
****************************************************************************/
static void cgfx_KillEffectCache( void )
{
    if ( sEffectCache != NULL )
    {
        StashTableIterator iter;
        StashElement elem;

        stashGetIterator(sEffectCache, &iter);
        while ( stashGetNextElement(&iter, &elem) )
        {
            tCgEffectSpec* pSpec = (tCgEffectSpec*)stashElementGetPointer(elem);
            cgfx_DestroyEffectSpecPtr( &pSpec );
            stashElementSetPointer(elem, NULL);
        }
        stashTableDestroy( sEffectCache );
        sEffectCache = NULL;
    }
}

/****************************************************************************
    cgfx_DestroyEffectSpecPtr
    
****************************************************************************/
static void cgfx_DestroyEffectSpecPtr( tCgEffectSpec** ppSpec )
{
    cgfx_DeInitEffect( *ppSpec );
    if ( (*ppSpec)->szEffectNm != NULL )
    {
        free( (*ppSpec)->szEffectNm );
    }
    
    free( *ppSpec );
    *ppSpec = NULL;
}

/****************************************************************************
    rt_cgfxGetEffect
    
****************************************************************************/
CGeffect rt_cgfxGetEffect( const char* effectName /* just the root name, no path or ext */, 
                            const char** args, bool bForceReload )
{
    tCgEffectSpec* pSpec = cgfx_QueryEffectCache( effectName, true );
    if (( pSpec ) && ( bForceReload ))
    {
        cgfx_DeInitEffectHandle( pSpec );
    }
    cgfx_InitEffectHandle( pSpec, args );
    return pSpec->hEffect;
}

/****************************************************************************
    rt_cgfxGetTechnique
    
****************************************************************************/
CGtechnique    rt_cgfxGetTechnique( CGeffect effect, const char* techniqueName )
{
    CGtechnique hTechnique = NULL;
    hTechnique = cgGetNamedTechnique( effect, techniqueName );
    if (( hTechnique == NULL ) ||
        ( CheckForCgError( "rt_cgfxGetTechnique", NULL ) != kCgFxError_NONE ))
    {
        failText( "Can't locate '%s' technique in CgFX effect definition.\n", techniqueName );
    }
    else if ( cgfx_PreCompilePrograms( hTechnique ) != kCgFxError_NONE )
    {
        failText( "Technique '%s', failed to compile for this shader and hardware.\n"
                "%s", techniqueName, cgGetLastListing(sCgContext) );
    }
    else if ( ! cgValidateTechnique( hTechnique ) )
    {
        failText( "Technique '%s', is not valid for this shader and hardware.\n"
                "%s", techniqueName, cgGetLastListing(sCgContext) );
    }
    return hTechnique;
}

/****************************************************************************
    rt_cgfxGetPass
    
****************************************************************************/
CGpass rt_cgfxGetPass( CGtechnique technique, const char* passName )
{
    CGpass hPass = cgGetNamedPass( technique, passName );
    if (( hPass == NULL ) ||
        ( CheckForCgError( "rt_cgfxGetPass", NULL ) != kCgFxError_NONE ))
    {
        failText( "Can't locate '%s' pass in CgFX effect definition.\n", passName );
    }
    return hPass;
}

/****************************************************************************
    rt_cgfxSetRenderPass
    
****************************************************************************/
tCgFxError rt_cgfxSetRenderPass( CGtechnique technique, CGpass pass  /* NULL=first pass found */)
{
    sCurrentEffectInst = NULL;
    // TODO: Fix bad symmetry between this and the effect instance entry point.
    PushOpenGLStateStack();
    return cgfx_EnablePass( ( pass != NULL ) ? pass : cgfx_GetPassByIndex( technique, 0 ) );
}

/****************************************************************************
    rt_cgfxUnsetRenderPass
    
****************************************************************************/
tCgFxError rt_cgfxUnsetRenderPass( CGtechnique technique, CGpass pass  /* NULL=first pass found */ )
{
    cgfx_DisablePass( ( pass != NULL ) ? pass : cgfx_GetPassByIndex( technique, 0 ) );
    return kCgFxError_NONE;
}




//===========================================================================
//
//
//    Cg Shader Interface
//
//
//===========================================================================

#define CGFX_DEBUG_HANDLE_SPECS_TBL 1

//---------------------------------------------------------------------------
//    tCgShaderSpec
//---------------------------------------------------------------------------
typedef struct tCgShaderSpec
{
    CGprogram            srcPgm;                // as loaded from disk
    char                shaderFile[24];        // in case we need to report errors
    tShaderProgramType    target;                // e.g., kShaderPgmType_FRAGMENT. Mostly for validation
    tCgShaderProfile    profile;            // e.g., CG_PROFILE_ARBVP1, CG_PROFILE_GLSLV, ...
    GLuint                shaderHdlTblOfs;    // how to locate it in sCGProgramTable.fragmentShaderVertexCombos.
} tCgShaderSpec;

//----------------------------------------------------------------------------
//    tComboShaderHandleState
//
//    Info about a combination CGprogram made from a vertex and fragment shader.
//----------------------------------------------------------------------------
typedef struct tComboShaderHandleState
{
    CGprogram            pgm;                // might be a combo vertex/fragment handle, or might == srcPgm
    GLuint                srcFragmentPgmId;
    GLuint                srcVertexPgmId;
    GLuint                nParamListNumUsed;
    GLuint                nParamListArraySz;
    tCgParamCacheSpec*    pParamList;     // an array of params, each of which (if it's an array) may have multiple CGparam handles
} tComboShaderHandleState;

//---------------------------------------------------------------------------
//    tFragShaderVertexCombos
//
//    Stores the list of vertex shaders that are used in combination
//    with a fragment shader.
//---------------------------------------------------------------------------
typedef struct tFragmentShaderVertCombos {
    GLuint                        vertexHdlStatesArrSz;
    GLuint                        vertexHdlStatesNum;
    tComboShaderHandleState*    handleStates;
} tFragmentShaderVertCombos;

//---------------------------------------------------------------------------
//    tCgProgramTable
//
//    Our storehouse of information on the current shaders. The table
//    is indexed by pgmId.
//---------------------------------------------------------------------------
typedef struct tCgProgramTable {
    //
    //    Program IDs
    //  --------------
    //    We generate pgmId's for client code in rt_cgGenerateCgShaderIds(). The numbers
    //    generated are offsets in our shaderSpecs[] table. So when a caller sends a program
    //    ID as a function argument, we can easily locate information about it. The shader spec
    //    for program ID "n" is shaderSpecs[n].
    //
    //    The list is    is in (unique) pgmId order, but not in tShaderProgramType order. We 
    //    don't control in what order it WCW_statemgmt.c asks for ID's. It might get all
    //    vertex program id's at once, and then all fragment program id's. Or it might get
    //    some vertex program id's, some fragment program id's, and then some more vertex
    //    program id's. So if you are looking for the n'th *fragment* shader, for example,
    //    you need more help.
    //
    //    Combination Shaders - how to find the info
    //    -------------------------------------------
    //    This issue comes into play when we are looking for the data structure for a combination
    //    of vertex and fragment program. 
    //        - The spec for the fragment program is at sCGProgramTable.shaderSpecs[fragmentPgmId]
    //        - sCGProgramTable.shaderSpecs[fragmentPgmId].shaderHdlTblOfs is the index to use
    //          to find fragment program's data struct in sCGProgramTable.fragmentShaderVertCombos.
    //        - Look in the tFragmentShaderVertCombos struct to find the vertex program's
    //          entry.
    //
    tCgShaderSpec*                shaderSpecs;    // indexed by pgmId - not in tShaderProgramType order
    GLuint                        shaderSpecArraySz;
    GLuint                        shaderSpecNumUsed;
    GLuint                        numSpecsUsedByType[kShaderPgmType_COUNT];
    tComboShaderHandleState*    activeComboHandleState;
    tCgShaderProfile            lastProfileEnableRequest[kShaderPgmType_COUNT];
    tCgShaderProfile            enabledProfile[kShaderPgmType_COUNT];
    GLuint                        lastShaderBindRequest[kShaderPgmType_COUNT];
    int                            boundShaderSpec[kShaderPgmType_COUNT];
    
    //
    //    Info used to access the current program handle and param handles (see comments, above)
    //
    GLuint                        fragmentShaderVertCombosArrSz;
    GLuint                        fragmentShaderVertCombosNum;        // number of fragment programs we have seen
    tFragmentShaderVertCombos*    fragmentShaderVertCombos;            // NOT indexed by pgmID but by shaderHdlTblOfs
} tCgProgramTable;

static tCgProgramTable sCGProgramTable = { 0 };


//---------------------------------------------------------------------------
//
//  WCW_statemgmt.c ---> pgmID --+
//                               |
//                               v
//  sCGProgramTable.shaderSpecs[pgmId]           ==> e.g., "skin_bumpvp.cg"
//      +------------------------|
//      |
//      v
//      tCgShaderSpec.srcPgm               ==> e.g., "skin_bumpvp.cg"
//      tCgShaderSpec.target               ==> e.g., kShaderType_VERTEX
//      tCgShaderSpec.profile              ==> e.g., CG_PROFILE_GLSLV
//      tCgShaderSpec.shaderHdlTblOfs      ==> a row or column index
//                                 |
//            /======================================\
//            || If this is a fragment shader, this ||
//            || provides an index into the         ||
//            || fragmentShaderVertCombos array.    ||
//            \======================================/
//                                              |
//                                              v
//  sCGProgramTable.fragmentShaderVertCombos[nShaderHdlTblOfs]
//                                 |
//                                 v
//            /======================================\
//            || Within a tFragmentShaderVertCombos ||
//            || you can search to find the         ||
//            || vertex program's element, "n".     ||
//            \======================================/
//                                                 |
//                                              v
//        tFragmentShaderVertCombos::handleStates[n]
//                                 |
//       +-------------------------|
//       |
//       v
//       tComboShaderHandleState.pgm;           // if NULL, this combination is not initialized
//       tComboShaderHandleState.pParamList[n]    ==> e.g., "g_BoneMatrixArrVP"
//           +-----------------------|
//           |
//           v
//           tCgParamCacheSpec.paramNmCacheKey (place to set data for params named "g_BoneMatrixArrVP" in WCW_statemgmt.c)
//           tCgParamCacheSpec.paramHdlList[n]  ==> e.g., "g_BoneMatrixArrVP[2]"
//               +--------------------------|
//               |
//               v
//               CGParameter
//---------------------------------------------------------------------------

const tCgProgramProfileMapping sCgProgramProfileMapping[] = {
    {0, 0}, // tCgShaderMode_Disabled
    {CG_PROFILE_ARBFP1, CG_PROFILE_ARBVP1}, // tCgShaderMode_ARB
    {CG_PROFILE_GLSLF, CG_PROFILE_GLSLV}, // tCgShaderMode_GLSL
    {CG_PROFILE_FP30, CG_PROFILE_VP30}, // tCgShaderMode_3
    {CG_PROFILE_FP40, CG_PROFILE_VP40}, // tCgShaderMode_4
    {CG_PROFILE_GPU_FP, CG_PROFILE_GPU_VP}, // tCgShaderMode_GPU
};

#define kCgProgramIdTableInitialSz    32
#define kCgProgramIdTableGrowSz        32

#define RoundUp( _Val_, _Alignment_ )    ((( _Val_ + ( _Alignment_ - 1 ) ) /  _Alignment_ ) * _Alignment_ )

static bool            StructList_Resize( void** ppList, size_t nListItemStructSz, GLuint* pnArrSz, GLuint nNewSize, GLuint nGrowSz );

static void            CgShaderSpecList_ReleaseMem( tCgShaderSpec* pPgmTblItem );

static void            ShaderPrep_EnableSelectedProfiles( void );
static void            ShaderPrep_EnableSelectedProfile( tShaderProgramType nPgmTypeI );

#if RT_SUPPORT_CG_COMBO_SHADERS
    static void        ShaderPrep_BindCgProgram_Deferred(tShaderProgramType target, GLuint pgmId );
    static void        ShaderPrep_BindSelectedShadersAsCombo( void );
#endif
static void            ShaderPrep_BindCgProgram_Immediate(tShaderProgramType target, GLuint pgmId );
static void            ShaderPrep_BindSelectedShader( tShaderProgramType nPgmTypeI );

static void            ComboShaderHandleState_ResizeParamList( tComboShaderHandleState* pHandleState, GLuint nNewSize );
static void            ComboShaderHandleState_ReleaseMem( tComboShaderHandleState* pHandleState );
static void            ComboShaderHandleState_CatalogCgParams( tComboShaderHandleState* pHandleState, GLuint vertexPmId, GLuint fragmentPgmId );
static tComboShaderHandleState*
                    ComboShaderHandleState_Find( GLuint vertPgmID, GLuint fragPgmID, bool bAdd );

static void            CgProgramTable_ResizeFragmentShaderCombos( GLuint nNewSize );
static void            FragmentShaderVertCombos_ReleaseMem( tFragmentShaderVertCombos* pComboInfo );

static void            CgParamCacheSpec_ParamHdlList_Resize( tCgParamCacheSpec* ppSpec, GLuint nNewSize );
static void            CgParamCacheSpec_ParamHdlList_ReleaseMem( tCgParamCacheSpec* ppSpec );

static void            ParseRawCgParamName( const char* szRaw, char* buffer, size_t buffSz, int* pIndex );

static tCgFxError    ReviewShaderLoadResults( const char* szProgramName, CGprogram prog );
static tCgFxError    ReviewShaderBindResults( const char* szProgramName, CGprogram prog );
static tCgFxError    ReviewProfileEnableResults( const char* szProgramName, CGprogram prog );

static void            CgProgramIdTable_Resize( tShaderProgramType target, GLuint nNewSize );
static void            CgProgramIdTable_ReleaseComboShaderHandles();

static void            DisableProgramIdForTarget( tShaderProgramType target, tCgShaderProfile profile );

                    //
                    //    Our table uses the pgmId from WCW_statemgmt.c as an index. We just need
                    //    to verify that we have a good target type and pgmId before accessing
                    //    the array.
                    //
#define             ParamTable_GetProgramTblIndexForPgmId(_tShaderProgramType_, _GLuint_pgmId_ )    \
                            ( ValidateShaderProgramTypeConstant(_tShaderProgramType_),                \
                            onlydevassert((_GLuint_pgmId_) < sCGProgramTable.shaderSpecNumUsed ),            \
                            (_GLuint_pgmId_) )

#define                ParamTable_GetProgramTblItemForPgmId(_tShaderProgramType_, _GLuint_pgmId_ )        \
                            (sCGProgramTable.shaderSpecs + ( ParamTable_GetProgramTblIndexForPgmId(_tShaderProgramType_, _GLuint_pgmId_ )) )

#define                ParamTable_GetCurrentProgramTblIndex(_tShaderProgramType_)                        \
                            ( ValidateShaderProgramTypeConstant(_tShaderProgramType_),                \
                            sCGProgramTable.boundShaderSpec[_tShaderProgramType_] )

#define                ParamTable_GetCurrentProgramTblItem(_tShaderProgramType_)                        \
                            (sCGProgramTable.shaderSpecs + ( ParamTable_GetCurrentProgramTblIndex(_tShaderProgramType_) ))

#define                ParamTable_IsComboShaderActive()                \
                            (sCGProgramTable.boundShaderSpec[kShaderPgmType_FRAGMENT] ==    \
                             sCGProgramTable.boundShaderSpec[kShaderPgmType_VERTEX] )
                             
#define                ParamTable_ShaderFileName(_GLuint_pgmId_)    sCGProgramTable.shaderSpecs[_GLuint_pgmId_].shaderFile


#if CGFX_DEBUG_HANDLE_SPECS_TBL
    #define VALIDATE_COMBO_SHADER_HANDLE_STATE( _tComboShaderHandleState_p_, _GLuint_vertexPmId_, _GLuint_fragmentPgmId_ )    \
        assert(( (_tComboShaderHandleState_p_)->srcVertexPgmId   == (_GLuint_vertexPmId_) ) &&                                \
               ( (_tComboShaderHandleState_p_)->srcFragmentPgmId == (_GLuint_fragmentPgmId_) ) )
#else
    #define VALIDATE_COMBO_SHADER_HANDLE_STATE( _tComboShaderHandleState_, _GLuint_vertexPmId_, _GLuint_fragmentPgmId_ )
#endif

/****************************************************************************
    rt_cgSetCgRenderModeSelected
    
****************************************************************************/
void rt_cgSetCgShaderMode( tCgShaderMode shaderMode )
{
    WCW_BindVertexProgram(0);
    WCW_DisableVertexProgram();
    WCW_BindFragmentProgram(0);
    WCW_DisableFragmentProgram();
    WCW_PrepareToDraw();
    rt_cgResetComboShaderHandles();

    g_rtCgFX_CgShaderMode = shaderMode;
}

/****************************************************************************
    CgProgramIdTable_Init
    
****************************************************************************/
static void CgProgramIdTable_Init( void )
{
    CgProgramIdTable_Resize( kShaderPgmType_FRAGMENT,   kCgProgramIdTableInitialSz );
    CgProgramIdTable_Resize( kShaderPgmType_VERTEX,   kCgProgramIdTableInitialSz );
    {
        // Set aside elements kVirtualShader_NotSet (0) and kVirtualShader_UseFixedFunction (1)
        // so that app code can virtually bind these dummy shader ID's.
        int nNullHandleIds[] = { 0, 0 };
        
        // Make sure no one mistakenly changed these constants
        assert( kVirtualShader_NotSet == 0 );
        assert( kVirtualShader_UseFixedFunction == 1 );
        
        rt_cgGenerateCgShaderIds( kShaderPgmType_FRAGMENT, ARRAY_SIZE(nNullHandleIds), nNullHandleIds );
        rt_cgGenerateCgShaderIds( kShaderPgmType_VERTEX, ARRAY_SIZE(nNullHandleIds), nNullHandleIds );
    }
    
    {
        int i;
        for (i=0; i < kShaderPgmType_COUNT; i++ )
        {
            sCGProgramTable.lastProfileEnableRequest[i]        = 0;
            sCGProgramTable.lastShaderBindRequest[i]        = -1;
            sCGProgramTable.boundShaderSpec[i]                = kVirtualShader_NotSet;
        }
    }
}

/****************************************************************************
    CgProgramIdTable_OnReset
    
****************************************************************************/
static void CgProgramIdTable_OnReset()
{
    CgProgramIdTable_ReleaseMem();
    CgProgramIdTable_Init();
}

/****************************************************************************
    CgProgramIdTable_ReleaseMem
    
****************************************************************************/
static void CgProgramIdTable_ReleaseMem( void )
{
    GLuint i;
    if ( sCGProgramTable.shaderSpecs != NULL )
    {
        for ( i=0; i < sCGProgramTable.shaderSpecNumUsed; i++ )
        {
            CgShaderSpecList_ReleaseMem( &sCGProgramTable.shaderSpecs[i] );
        }
        free( sCGProgramTable.shaderSpecs );
        sCGProgramTable.shaderSpecs = NULL;
        sCGProgramTable.shaderSpecArraySz = 0;
        sCGProgramTable.shaderSpecNumUsed = 0;
    }
    
    if ( sCGProgramTable.fragmentShaderVertCombos != NULL )
    {
        CgProgramIdTable_ReleaseComboShaderHandles();
        free( sCGProgramTable.fragmentShaderVertCombos );
        sCGProgramTable.fragmentShaderVertCombos = NULL;
        sCGProgramTable.fragmentShaderVertCombosNum = 0;
        sCGProgramTable.fragmentShaderVertCombosArrSz = 0;
    }
    
    for ( i=0; i < kShaderPgmType_COUNT; i++ )
    {
        sCGProgramTable.numSpecsUsedByType[i] = 0;
        sCGProgramTable.lastProfileEnableRequest[i] = 0;
        sCGProgramTable.enabledProfile[i] = 0;
        sCGProgramTable.lastShaderBindRequest[i] = -1;
        sCGProgramTable.boundShaderSpec[i] = -1;
    }
    
    sCGProgramTable.activeComboHandleState = NULL;
}

/****************************************************************************
    CgProgramIdTable_ReleaseComboShaderHandles
    
****************************************************************************/
static void CgProgramIdTable_ReleaseComboShaderHandles( void )
{
    GLuint i;
    for ( i=0; i < sCGProgramTable.fragmentShaderVertCombosNum; i++ )
    {
        FragmentShaderVertCombos_ReleaseMem( &sCGProgramTable.fragmentShaderVertCombos[i] );
    }
    sCGProgramTable.activeComboHandleState = NULL;
}

/****************************************************************************
    CgProgramIdTable_Resize
    
****************************************************************************/
static void CgProgramIdTable_Resize( tShaderProgramType target, GLuint nNewNumItems )
{
    GLuint nGrowReq = ( nNewNumItems - sCGProgramTable.shaderSpecNumUsed );
    StructList_Resize(
                &sCGProgramTable.shaderSpecs,
                sizeof(tCgShaderSpec),
                &sCGProgramTable.shaderSpecArraySz,
                nNewNumItems,
                kCgProgramIdTableGrowSz );            

    if ( target == kShaderPgmType_FRAGMENT )
    {
        //
        // We also need to grow the table of combo shader programs
        //
        CgProgramTable_ResizeFragmentShaderCombos( 
            sCGProgramTable.numSpecsUsedByType[kShaderPgmType_FRAGMENT] + nGrowReq );
    }
}

/****************************************************************************
    rt_cgGenerateCgShaderIds
    
****************************************************************************/
void rt_cgGenerateCgShaderIds( tShaderProgramType target, GLuint nCount /*IN*/, GLuint* ids /*OUT*/ )
{
    GLuint i;
    ValidateShaderProgramTypeConstant(target);
    CgProgramIdTable_Resize( target, sCGProgramTable.shaderSpecNumUsed + nCount );
    if ( target == kShaderPgmType_FRAGMENT )
    {
        sCGProgramTable.fragmentShaderVertCombosNum += nCount;
    }
    
    for ( i=0; i < nCount; i++ )
    {
        //
        // Record the number of specs of this type which have been seen so
        // far as the shaderHdlTblOfs. Later, we'll be able to treat this
        // as a row/col offset for finding an item in the handleSpecs[] array.
        //
        sCGProgramTable.shaderSpecs[sCGProgramTable.shaderSpecNumUsed].shaderHdlTblOfs =
                sCGProgramTable.numSpecsUsedByType[target];
                
        ids[i] = sCGProgramTable.shaderSpecNumUsed;
        
        sCGProgramTable.numSpecsUsedByType[target]++;
        sCGProgramTable.shaderSpecNumUsed++;        
    }
}

/****************************************************************************
    rt_cgResetCgShaderIds
    
****************************************************************************/
void rt_cgResetCgShaderIds( void )
{
    CgProgramIdTable_OnReset();
}

/****************************************************************************
    rt_cgResetCgShaderIds
    
    Force the module to rebuild the combination fragment / vertex program
    handles on the fly.
****************************************************************************/
void rt_cgResetComboShaderHandles( void )
{
    CgProgramIdTable_ReleaseComboShaderHandles();
}

/****************************************************************************
    ParseRawCgParamName
    
    Returns zero in *pIndex if this is not an array-format name.
    Only knows about two dimensional arrays. (Do we need more??)
****************************************************************************/
static void ParseRawCgParamName( const char* szRaw, char* buffer, size_t buffSz, int* pIndex )
{
    int nParamArrayIndex = 0;
    const char* szParamNm = szRaw;
    
    //
    // Ignore the structure variable name, such as "IN." by looking
    // only at the portion following the final ".". So if the
    // param is reported as "IN.g_FooBar", we treat it as "g_FooBar".
    //
    {
        char* p;
        p = strrchr( szParamNm, '.' );
        if ( p != NULL )
        {
            szParamNm = ( p + 1 );
        }
    }
    strncpy_s( buffer, buffSz, szParamNm, _TRUNCATE );
    szParamNm = buffer;
        
    //
    //    Parse out the array index, if present (e.g., g_BoneIndex[4] -> "g_BoneIndex" and 4.
    //
    {
        char* p = strchr( szParamNm, '[' );
        if ( p != NULL )
        {
            *p = '\0';    // add a terminator to remove it from the name.
            p++;        // move to the start of the index value.
            while (( *p != ']' ) && ( *p != '\0' ))
            {
                if ( isdigit(*p) )
                {
                    nParamArrayIndex = (( nParamArrayIndex * 10 ) + ( *p - '0' ));
                }
                p++;
            }
        }
    }
    
    *pIndex = nParamArrayIndex;
}

/****************************************************************************
    rt_cgPrepareAndCacheProgramHdl
    
****************************************************************************/
void rt_cgPrepareAndCacheProgramHdl( tShaderProgramType target, tCgShaderProfile profile, 
                                    GLuint pgmId, CGprogram program, 
                                    const char* szSrcFile /*for reporting errors*/ )
{
    tCgShaderSpec* pTableData                = NULL;
    GLuint nHandleStateIndex                = 0;
    tComboShaderHandleState* pHandleState    = NULL;
    ValidateShaderProgramTypeConstant( target );

    if (!cgIsProgramCompiled( program ))
    {
        rt_cgfxCompileProgram( program );
    }
    
    assert( pgmId < sCGProgramTable.shaderSpecNumUsed );
    
    pTableData = &(sCGProgramTable.shaderSpecs[pgmId]);
    pTableData->srcPgm                = program;
    strncpy_s( pTableData->shaderFile, ARRAY_SIZE(pTableData->shaderFile), szSrcFile, _TRUNCATE );
    pTableData->target                = target;
    pTableData->profile                = profile;
}


/****************************************************************************
    ComboShaderHandleState_Find
    
****************************************************************************/
tComboShaderHandleState* ComboShaderHandleState_Find( GLuint vertPgmID, GLuint fragPgmID, bool bAdd )
{
    static const GLuint kFragComboGrowSz = 4;
    
    //
    //    Linear searching in this function:
    //
    //    Our combo shader list can be thought of as an array of fragment shader (ID) rows
    //    and vertex program (ID) columns, where a fragment shader ID is also its row index.
    //    Therefore, finding a fragment shader ID's data structure is trivial and quick.
    //
    //    The fragment shader's data contains a list of data structures for various vertex
    //    program IDs -- but only those which are actually in use and in no particular
    //    order. So to locate a fragment/vertex combo we go directly to the fragment ID's
    //    data struct and then search for a vertex ID match the hard way.
    //
    //    This arrangement has been chosen on the assumption that there are fewer cases of
    //    fragment shaders pairing up with multiple vertex shaders than vice versa. In
    //    the best case, every fragment shader will pair up with only one vertex shader.
    //
    //    Still, the for-next loop seems scary in this performance sensitive code. Should
    //    we use a hash table or other smart container object?
    //
    //    Here is a sample of runtime statistics collected from this function:
    //
    //        Nbr calls.................... 2451284
    //        Least items searched......... 1
    //        Most items searched.......... 4
    //        Percentage no-search calls... 91.81
    //        Average items searched....... 0.0993
    //
    //    (You can get new stats by changing GET_STATS_ON_COMBO_SHADER_FIND to 1.)
    //
    //    Approximately 90% of the time the item being requested had already been found
    //    and no searching was needed. When the code did need to search, the most items
    //    which needed to be reviewed    (via integer comparisons) was 4. So it seems that
    //    a more sophisticated container mechanism is unnecessary.
    //
    //    LSTL, 08/2009
    //
    #define GET_STATS_ON_COMBO_SHADER_FIND 0

    #if GET_STATS_ON_COMBO_SHADER_FIND
        static const GLuint kDEBUG_SpewFrequency = 100;
        static GLuint nDEBUG_NumCalls = 0;
        static GLuint nDEBUG_NumCallsSinceLastSpew = 0;
        static GLuint nDEBUG_LeastItems = 0;
        static GLuint nDEBUG_MostItems = 0;
        static GLuint nDEBUG_NumberRepeatFinds = 0;
        static GLuint nDEBUG_TotalItems = 0;
        GLuint nDEBUG_ItemsThisCall = 0;
    #endif

    GLuint vertI;
    tComboShaderHandleState* pHandleState = NULL;
    tFragmentShaderVertCombos* pFragmentShaderVertCombo    = NULL;
    GLuint shaderHdlTblOfs = sCGProgramTable.shaderSpecs[fragPgmID].shaderHdlTblOfs;
    
    #if GET_STATS_ON_COMBO_SHADER_FIND
        nDEBUG_NumCalls++;
    #endif
    
    if (( sCGProgramTable.activeComboHandleState != NULL ) &&
        ( sCGProgramTable.activeComboHandleState->srcVertexPgmId == vertPgmID ) &&
        ( sCGProgramTable.activeComboHandleState->srcFragmentPgmId == fragPgmID ))
    {
        //
        //    Same as the last one. Save some lookup time.
        //
        SHADER_DBG_PRINTF( "    ComboShaderHandleState_Find() - No change, so use previous rather than doing a lookup.\n" );
        #if GET_STATS_ON_COMBO_SHADER_FIND
            nDEBUG_NumberRepeatFinds++;
        #endif
        return sCGProgramTable.activeComboHandleState;
    }
    
    assert( shaderHdlTblOfs < sCGProgramTable.fragmentShaderVertCombosNum );
    pFragmentShaderVertCombo = &sCGProgramTable.fragmentShaderVertCombos[shaderHdlTblOfs];
    
    for ( vertI=0; vertI < pFragmentShaderVertCombo->vertexHdlStatesNum; vertI++ )
    {
        #if GET_STATS_ON_COMBO_SHADER_FIND
            nDEBUG_ItemsThisCall++;
        #endif
        if ( pFragmentShaderVertCombo->handleStates[vertI].srcVertexPgmId == vertPgmID )
        {
            pHandleState = &pFragmentShaderVertCombo->handleStates[vertI];
            break;
        }
    }
    
    #if GET_STATS_ON_COMBO_SHADER_FIND
        nDEBUG_TotalItems += nDEBUG_ItemsThisCall;
        if (( nDEBUG_LeastItems == 0 ) || ( nDEBUG_ItemsThisCall < nDEBUG_LeastItems ))
        {
            nDEBUG_LeastItems = nDEBUG_ItemsThisCall;
        }
        if ( nDEBUG_ItemsThisCall > nDEBUG_MostItems )
        {
            nDEBUG_MostItems = nDEBUG_ItemsThisCall;
        }
        nDEBUG_NumCallsSinceLastSpew++;
        if ( nDEBUG_NumCallsSinceLastSpew == kDEBUG_SpewFrequency )
        {
            printf( "STATS FOR ComboShaderHandleState_Find():\n" );
            printf( "  Nbr calls.................... %u\n", nDEBUG_NumCalls );
            printf( "  Least items searched......... %u\n", nDEBUG_LeastItems );
            printf( "  Most items searched.......... %u\n", nDEBUG_MostItems );
            printf( "  Percentage no-search calls... %0.4f\n", ((float)nDEBUG_NumberRepeatFinds / (float)nDEBUG_NumCalls) * 100.0f );
            printf( "  Average items searched....... %0.4f\n", (float)( (float)nDEBUG_TotalItems / (float)nDEBUG_NumCalls ) );
            nDEBUG_NumCallsSinceLastSpew = 0;
        }
    #endif
    
    if (( pHandleState == NULL ) && ( bAdd ))
    {
        // Don't have one for that fragment program ID, so we need to add one.
        StructList_Resize( 
            &pFragmentShaderVertCombo->handleStates,
            sizeof(tComboShaderHandleState),
            &pFragmentShaderVertCombo->vertexHdlStatesArrSz,
            (pFragmentShaderVertCombo->vertexHdlStatesNum + 1 ),
            kFragComboGrowSz );
        pHandleState = &pFragmentShaderVertCombo->handleStates[pFragmentShaderVertCombo->vertexHdlStatesNum];
        pHandleState->srcFragmentPgmId = fragPgmID;
        pHandleState->srcVertexPgmId = vertPgmID;    // mostly for debugging. technically, we don't need this

        pFragmentShaderVertCombo->vertexHdlStatesNum++;
    }
    return pHandleState;
}

/****************************************************************************
    ComboShaderHandleState_CatalogCgParams
    
****************************************************************************/
static void ComboShaderHandleState_CatalogCgParams( tComboShaderHandleState* pHandleState, GLuint vertexPmId, GLuint fragmentPgmId )
{
    int nLastCacheKey = -1;
    tCgParamCacheSpec* pLastParamSpec = NULL;
    tShaderProgramType pgmType;
    int nDomains = cgGetNumProgramDomains(pHandleState->pgm);
    int domainI;
    for ( domainI=0; domainI < nDomains; domainI++ ) 
    {
        int nLastCacheKey =        -1;
        int nParamNmCacheKey;
        CGparameter param;
        GLenum nameSpaces[] = { CG_GLOBAL, CG_PROGRAM };
        int namespaceI;
        CGprogram srcPgm = cgGetProgramDomainProgram(pHandleState->pgm, domainI);
        CGdomain domainID =    cgGetProgramDomain( srcPgm );
        
        if ( domainID == CG_VERTEX_DOMAIN )
        {
            pgmType = kShaderPgmType_VERTEX;
        }
        else if ( domainID == CG_FRAGMENT_DOMAIN )
        {
            pgmType = kShaderPgmType_FRAGMENT;
        }
        else
        {
            printf( "Unknown domainID for shader! '%d'\n", domainID );
            continue;
        }
            
        //
        // Analyze the parameters of the *source* (non-combo) shader
        //
        for ( namespaceI = 0; namespaceI < ARRAY_SIZE(nameSpaces); namespaceI++ )
        {
            param = cgGetFirstLeafParameter( srcPgm, nameSpaces[namespaceI] );
            while ( param != NULL )
            {
                const char* szRawParamName = NULL;
                tCgParamCacheSpec* pParamSpec = NULL;
                size_t paramNameBuffSz = 0;
                char* szParamNm = NULL;
                GLuint nParamArrayIndex = 0;
                int nNumRows;
                int nNumCols;
                int nExpectedRows;
                int nExpectedCols;            
                
                do     // fake try/catch - always exits at the bottom
                {
                    if ( ! cgIsParameterReferenced( param ) )
                    {
                        // Not used in the compiler shader, so ignore it.
                        break;
                    }

                    szRawParamName = cgGetParameterName( param );
                    paramNameBuffSz = ( strlen(szRawParamName) + 1 );
                    szParamNm = _alloca( paramNameBuffSz );
                    nParamArrayIndex = 0;
                                                
                    //
                    //    szParamNm is the simple param name without a variable prefix (e.g., "IN.")
                    //    and without an array spec (e.g., "[4]").
                    //
                    ParseRawCgParamName( szRawParamName, szParamNm, paramNameBuffSz, &nParamArrayIndex );
                    //
                    // Names of params which the program has an interest in are already 
                    // registered. That happened when the global param table in WCW_statemgmt.c
                    // was initialized. At this point, if we find a shader param which has
                    // no pre-existing cache key, it means the program doesn't care about it.
                    // So when calling WCW_GetCacheIdForShaderParamName() we pass 'false' for
                    // the 'bAdd' argument (i.e., "if you don't find it, that's OK; don't add it").
                    //
                    nParamNmCacheKey = WCW_GetCacheIdForShaderParamName( szParamNm, pgmType, false );
                    if ( nParamNmCacheKey == -1 )
                    {
                        // WCW_statemgmt.c doesn't know it.
                        break;
                    }
                    
                    nNumRows = cgGetParameterRows( param );
                    nNumCols = cgGetParameterColumns( param );                                            
                    WCW_GetFloatParamExpectedDimensions( nParamNmCacheKey, &nExpectedRows, &nExpectedCols );
                    if (( nExpectedRows != nNumRows ) ||
                        ( nExpectedCols != nNumCols ))
                    {
                        Errorf( "Bad numeric param defintion for %s.\nElement shape is %dx%d but program expects %dx%d.\n",
                            szParamNm, nNumRows, nNumCols, nExpectedRows, nExpectedCols );
                        // Bad shader def.
                        break;
                    }
                        
                    //
                    // The definition seems correct, so process it.
                    //
                    //
                    //    Might already exist for another index value (foo[0] already seen
                    //    and we're looking at foo[1], for example); so search for it
                    //    First, check to see if it's the same as the last cache key
                    //    before embarking on a long sequential search.
                    //
                    if ( nParamNmCacheKey == nLastCacheKey )
                    {
                        pParamSpec = pLastParamSpec;
                    }
                    if ( pParamSpec == NULL )
                    {
                        // do it the hard way                
                        GLuint tableI;
                        for ( tableI=0; tableI < pHandleState->nParamListNumUsed; tableI++ )
                        {
                            if ( pHandleState->pParamList[tableI].paramNmCacheKey == nParamNmCacheKey )
                            {
                                pParamSpec = &(pHandleState->pParamList[tableI]);
                                break;
                            }
                        }
                    }

                    if ( pParamSpec == NULL )
                    {
                        // Nope. Never seen it, so add a new entry.
                        GLuint nParamCount = pHandleState->nParamListNumUsed;
                        ComboShaderHandleState_ResizeParamList( pHandleState, ( nParamCount + 1 ) );
                        pParamSpec = &(pHandleState->pParamList[nParamCount]);
                        pParamSpec->paramNmCacheKey = nParamNmCacheKey;
                        pHandleState->nParamListNumUsed++;
                    }

                    // Remember these for next time as a simple optimization                
                    nLastCacheKey = nParamNmCacheKey;
                    pLastParamSpec = pParamSpec;
                            
                    // Store this handle in the spec's CGparam array
                    CgParamCacheSpec_ParamHdlList_Resize( pParamSpec, ( nParamArrayIndex + 1 ));
                    pParamSpec->paramHdlList[nParamArrayIndex] = param;
                    if ( nParamArrayIndex >= pParamSpec->nNumParamHdls )
                    {
                        pParamSpec->nNumParamHdls = ( nParamArrayIndex + 1 );
                    }
                    
                } while ( false );    // always break out of this fake try/catch
                
                param = cgGetNextLeafParameter( param );
            }
        }    // namespace loop
    }    
}

/****************************************************************************
    rt_cgCreateProgramFromBuffer
    
****************************************************************************/
CGprogram rt_cgCreateProgramFromBuffer( const char* szProgramName, 
                                        CGenum program_type, 
                                        const char *program, 
                                        tShaderProgramType target, 
                                        tCgShaderProfile profile, 
                                        const char *entry, 
                                        const char **args)
{
    tCgFxError nError = kCgFxError_NONE;
    CGprogram pgmHandle = NULL;
    
    ValidateShaderProgramTypeConstant( target );
    
    pgmHandle = cgCreateProgram( rt_cgfxGetGlobalContext(), program_type, program, profile, entry, args );
    nError = ReviewShaderCompileResults( szProgramName, pgmHandle );
    
    if ( nError == kCgFxError_NONE )
    {
        if ( pgmHandle != NULL )
        {
            if ( game_state.saveCgShaderListing )
            {
                SaveProgramAssembly( szProgramName, pgmHandle );
            }
            cgGLLoadProgram( pgmHandle );
            if (ReviewShaderLoadResults( szProgramName, pgmHandle ) != kCgFxError_NONE)
            {
                // Shader may not be able to load because of driver/hw implementation
                // limitations (e.g. number of temporaries, dependent texture reads,etc.)
                // If that is the case destroy the program so that we correctly
                // propagate the error and load the error shader instead
                cgDestroyProgram( pgmHandle );
                pgmHandle = NULL;
            }
        }
    }
    return pgmHandle;
}

/****************************************************************************
    rt_cgEnableProgram
    
****************************************************************************/
void rt_cgEnableProgram(tShaderProgramType target, tCgShaderProfile profile )
{
    rdrSetMarker(__FUNCTION__"( target=%s, profile=%d ) queued.\n", SHADER_DBG_PGM_TYPE_STR(target), profile);
    ValidateCgProfileConstant( profile );
    sCGProgramTable.lastProfileEnableRequest[target] = profile;
    if ( RT_USE_HARD_CONST_PARAM_MODE() )
    {
        ShaderPrep_EnableSelectedProfiles();        // do this immediately if we aren't using combo shaders
    }
}

/****************************************************************************
    rt_cgDisableProgram
    
****************************************************************************/
void rt_cgDisableProgram(tShaderProgramType target)
{
    rdrSetMarker(__FUNCTION__"( target=%s )", SHADER_DBG_PGM_TYPE_STR(target) );
    DisableProgramIdForTarget( target, sCGProgramTable.enabledProfile[target] );
}

/****************************************************************************
    rt_cgResetProgram
    
****************************************************************************/
void rt_cgResetProgram(tShaderProgramType target)
{
    rdrSetMarker(__FUNCTION__"( target=%s )", SHADER_DBG_PGM_TYPE_STR(target) );

    if ( sCGProgramTable.enabledProfile[target] != 0 )
        cgGLDisableProfile( sCGProgramTable.enabledProfile[target] );
    sCGProgramTable.enabledProfile[target] = 0;
    sCGProgramTable.boundShaderSpec[target] = kVirtualShader_NotSet;
}


/****************************************************************************
    DisableProgramIdForTarget
    
****************************************************************************/
static void DisableProgramIdForTarget( tShaderProgramType target, tCgShaderProfile profile )
{
    rdrSetMarker(__FUNCTION__ "( target=%s, profile=%d ) queued.\n", SHADER_DBG_PGM_TYPE_STR(target), profile );
    sCGProgramTable.lastProfileEnableRequest[target] = 0;    // enable profile 0 means, disable whatever is set
    if ( RT_USE_HARD_CONST_PARAM_MODE() )
    {
        ShaderPrep_EnableSelectedProfiles();        // do this immediately if we aren't using combo shaders
    }
}

/****************************************************************************
    rt_cgBindCgProgram
    
****************************************************************************/
void rt_cgBindCgProgram(tShaderProgramType target, GLuint pgmId )
{
    #if RT_SUPPORT_CG_COMBO_SHADERS
        if ( RT_USE_COMBO_SHADERS() )
        {
                // We store this preference until both shaders are spec'd and
                // rt_cgFinalShaderPrep() is called.
                ShaderPrep_BindCgProgram_Deferred( target, pgmId );
        }
        else
    #endif
    {
        // Immediately bind the shader, since we're not putting a
        // combo shader together.
        ShaderPrep_BindCgProgram_Immediate( target, pgmId );
    }
}

/****************************************************************************
    ShaderPrep_BindCgProgram_Deferred
    
****************************************************************************/
#if RT_SUPPORT_CG_COMBO_SHADERS
void ShaderPrep_BindCgProgram_Deferred(tShaderProgramType target, GLuint pgmId )
{
    rdrSetMarker(__FUNCTION__"( %s, %d ) queued.\n", SHADER_DBG_PGM_TYPE_STR( target ), pgmId );
    if ( pgmId == 0 )
    {
        SHADER_DBG_PRINTF( "  - Disabling the profile instead.\n" );
        DisableProgramIdForTarget( target, sCGProgramTable.enabledProfile[target] );
        // Since the profile is being disabled, any pending shader
        // changes are cancelled.
        sCGProgramTable.lastShaderBindRequest[target] = -1;
        return;
    }

    #if CGFX_TRACK_UNHANDLED_PENDING_BINDS
        //
        // We log cases where a shader was requested but not bound. Typically, that
        // means a second call was made to rt_cgBindCgProgram() before the first call
        // was acted upon in rt_cgFinalShaderPrep(). The concern is that one piece of
        // code is drawing with the wrong shader. It's also legal for the client code
        // to select a default shader and then, on further consideration, set a different
        // one before drawing finally takes place. So the "ERROR! Bind request still p
        // ending!" warning message *might* be innocuous.
        //
        if ( ( sCGProgramTable.lastShaderBindRequest[target] != (GLuint)-1 ) &&
             ( sCGProgramTable.lastShaderBindRequest[target] > 1 ) &&
             ( sCGProgramTable.lastShaderBindRequest[target] != pgmId ))
        {
            char msg[512];
            sprintf_s( msg, ARRAY_SIZE(msg),
                "\n====> ERROR! Bind request still pending!\n"
                  "  Pending:   %d (%s)\n"
                  "  Requested: %d (%s)\n"
                  "  (Most likely cause: missing a call to WCW_PrepareToDraw() before drawing.)\n\n",
                    sCGProgramTable.lastShaderBindRequest[target],
                    ParamTable_ShaderFileName(sCGProgramTable.lastShaderBindRequest[target]),
                    pgmId,
                    ParamTable_ShaderFileName(pgmId)
                 );
            SHADER_DBG_PRINTF( msg );
            printf( msg );
        }
    #endif
    sCGProgramTable.lastShaderBindRequest[target] = pgmId;
}
#endif

/****************************************************************************
    ShaderPrep_BindCgProgram_Immediate
    
****************************************************************************/
static void ShaderPrep_BindCgProgram_Immediate(tShaderProgramType target, GLuint pgmId )
{
    rdrSetMarker(__FUNCTION__"( %s, %d ) bound.\n", SHADER_DBG_PGM_TYPE_STR( target ), pgmId );
    sCGProgramTable.lastShaderBindRequest[target] = pgmId;
    ShaderPrep_EnableSelectedProfile( target );
    if ( pgmId == 0 )
    {
        return;
    }
    ShaderPrep_BindSelectedShader( target );
}

/****************************************************************************
    ShaderPrep_BindSelectedShadersAsCombo
    
****************************************************************************/
#if RT_SUPPORT_CG_COMBO_SHADERS
void ShaderPrep_BindSelectedShadersAsCombo( void )
{
    GLuint i;
    GLuint nCurrVertPgmID, nCurrFragPgmID;
    GLuint nPrevVertPgmID, nPrevFragPgmID;
    int nPgmTableIndex = -1;
    tComboShaderHandleState* pHandleState = NULL;
    bool bNewShader = false;
    
    SHADER_DBG_PRINTF( "BEGIN ShaderPrep_BindSelectedShadersAsCombo() - Vertex=%d, Fragment=%d\n",
        sCGProgramTable.lastShaderBindRequest[kShaderPgmType_VERTEX],
        sCGProgramTable.lastShaderBindRequest[kShaderPgmType_FRAGMENT] );

    nPrevVertPgmID    = sCGProgramTable.boundShaderSpec[kShaderPgmType_VERTEX];
    nPrevFragPgmID    = sCGProgramTable.boundShaderSpec[kShaderPgmType_FRAGMENT];
    
    nCurrVertPgmID    = nPrevVertPgmID;
    nCurrFragPgmID    = nPrevFragPgmID;

    if ( sCGProgramTable.enabledProfile[kShaderPgmType_VERTEX] == 0 )
    {
        nCurrVertPgmID = kVirtualShader_UseFixedFunction;
        bNewShader = true;
    }
    if ( sCGProgramTable.enabledProfile[kShaderPgmType_FRAGMENT] == 0 )
    {
        nCurrFragPgmID = kVirtualShader_UseFixedFunction;
        bNewShader = true;
    }
    if ( sCGProgramTable.lastShaderBindRequest[kShaderPgmType_VERTEX] != -1 )
    {
        SHADER_DBG_PRINTF( "    sCGProgramTable.lastShaderBindRequest[kShaderPgmType_VERTEX] != -1\n");
        nCurrVertPgmID = sCGProgramTable.lastShaderBindRequest[kShaderPgmType_VERTEX];
        if ( nCurrVertPgmID != nPrevVertPgmID )
        {
            SHADER_DBG_PRINTF( "    nCurrVertPgmID != nPrevFragPgmID\n");
            if ( nCurrVertPgmID == kVirtualShader_NotSet )
            {
                SHADER_DBG_PRINTF( "    nCurrVertPgmID = kVirtualShader_UseFixedFunction\n");
                nCurrVertPgmID = kVirtualShader_UseFixedFunction;
            }
            bNewShader = true;
        }
    }
    if ( sCGProgramTable.lastShaderBindRequest[kShaderPgmType_FRAGMENT] != -1 )
    {
        SHADER_DBG_PRINTF( "    sCGProgramTable.lastShaderBindRequest[kShaderPgmType_FRAGMENT] != -1\n");
        nCurrFragPgmID = sCGProgramTable.lastShaderBindRequest[kShaderPgmType_FRAGMENT];
        if ( nCurrFragPgmID != nPrevFragPgmID )
        {
            SHADER_DBG_PRINTF( "    nCurrFragPgmID != nPrevFragPgmID\n" );
            if ( nCurrFragPgmID == kVirtualShader_NotSet )
            {
                SHADER_DBG_PRINTF( "    nCurrFragPgmID == -1\n" );
                nCurrFragPgmID = kVirtualShader_UseFixedFunction;
            }
            bNewShader = true;
        }
    }

    rdrBeginMarker(__FUNCTION__ ":%s(%d),%s(%d)", ParamTable_ShaderFileName(nCurrVertPgmID),nCurrVertPgmID,ParamTable_ShaderFileName(nCurrFragPgmID),nCurrFragPgmID);

    //
    //    Find or build a shader handle
    //
    if (( nCurrVertPgmID != kVirtualShader_NotSet ) ||
        ( nCurrFragPgmID != kVirtualShader_NotSet ))
    {
        SHADER_DBG_PRINTF( "    Looking for combo shader:\n" );
        SHADER_DBG_PRINTF( "      Vertex...... %-3d - %s\n", nCurrVertPgmID, ParamTable_ShaderFileName(nCurrVertPgmID) );
        SHADER_DBG_PRINTF( "      Fragment.... %-3d - %s\n", nCurrFragPgmID, ParamTable_ShaderFileName(nCurrFragPgmID) );
        SHADER_DBG_PRINTF( "      NewShader... %s\n", (( bNewShader ) ? "true" : "false" ) );
        pHandleState = ComboShaderHandleState_Find( nCurrVertPgmID, nCurrFragPgmID, bNewShader );
        if ( pHandleState != sCGProgramTable.activeComboHandleState )
        {
            bNewShader = true;
        }
    }
    
    sCGProgramTable.activeComboHandleState = pHandleState;
    
    if ( pHandleState )
    {
        if ( bNewShader )
        {
            rdrBeginMarker("bNewShader");

            //
            //    We have some work to do.
            //
            SHADER_DBG_PRINTF( "    We have some work to do.\n" );    
            VALIDATE_COMBO_SHADER_HANDLE_STATE( pHandleState, nCurrVertPgmID, nCurrFragPgmID );
            WCW_SetParamDirtyFlags( 
                ( nCurrVertPgmID != kVirtualShader_NotSet ), 
                ( nCurrFragPgmID != kVirtualShader_NotSet ) );    // shader change, so we need to update the params        
            if ( pHandleState->pgm == NULL )
            {
                // This combination has not been used before.
                CGprogram vertPgm, fragPgm;

                rdrBeginMarker("combo not used before");
                SHADER_DBG_PRINTF( "    This combination has not been used before.\n" );
                ComboShaderHandleState_ReleaseMem( pHandleState );
                vertPgm = ParamTable_GetProgramTblItemForPgmId( kShaderPgmType_VERTEX, nCurrVertPgmID )->srcPgm;
                fragPgm = ParamTable_GetProgramTblItemForPgmId( kShaderPgmType_FRAGMENT, nCurrFragPgmID )->srcPgm;
                if (( VALID_SHADER_PGM( nCurrVertPgmID  )) &&
                    ( VALID_SHADER_PGM( nCurrFragPgmID )))
                {
                    //
                    //    Combo shader
                    //
                    SHADER_DBG_PRINTF( "    We have a combo: %s and %s.\n", 
                            sCGProgramTable.shaderSpecs[ParamTable_GetProgramTblIndexForPgmId( kShaderPgmType_VERTEX, nCurrVertPgmID )].shaderFile, 
                            sCGProgramTable.shaderSpecs[ParamTable_GetProgramTblIndexForPgmId( kShaderPgmType_FRAGMENT, nCurrFragPgmID )].shaderFile 
                        );
                    pHandleState->pgm = cgCombinePrograms2( vertPgm, fragPgm );
                    if(!pHandleState->pgm) CheckForCgError( "cgCombinePrograms2", NULL );
                }
                else
                {
                    //
                    //    Only one program type specified, so use whichever is not "virtual"...
                    //
                    pHandleState->pgm = vertPgm ? vertPgm : fragPgm;
                    SHADER_DBG_PRINTF( "    We have a non-combo shader.\n" );    
                }
                if ( pHandleState->pgm )
                {
                    if ( ! cgGLIsProgramLoaded( pHandleState->pgm ) )
                    {
                        cgGLLoadProgram( pHandleState->pgm ); CHECKGL;
                        ReviewShaderLoadResults( "Combination Shader", pHandleState->pgm );
                    }
                    ComboShaderHandleState_CatalogCgParams( pHandleState, nCurrVertPgmID, nCurrFragPgmID );
                }
                else
                {
                    SHADER_DBG_PRINTF( "  cgGLBindProgram() - no shader bound!\n" );
                }
                rdrEndMarker();
            }

            if ( pHandleState->pgm )
            {        
                //
                // Put the param handles for this shader in the appropriate slots
                // where the WCW_statemgmt.c module can find them when it needs to
                // update shader params.
                //
                SHADER_DBG_PRINTF( "    Caching program handles...\n" );
                for ( i=0; i < pHandleState->nParamListNumUsed; i++ )
                {
                    WCW_CacheParamHandlesForShaderParam( pHandleState->pgm,
                        pHandleState->pParamList[i].paramNmCacheKey,
                        pHandleState->pParamList[i].paramHdlList,
                        (int)pHandleState->pParamList[i].nNumParamHdls );
                }
            }

            rdrEndMarker();
        }    // bNewShader to set up
        else
        {
            SHADER_DBG_PRINTF( "No shader bind changes in ShaderPrep_BindSelectedShadersAsCombo().\n" );
        }

        //
        //    Allow the client code to make some last minute param updates.
        //
        WCW_UpdateShaderParamsFromLocalCache( pHandleState->pParamList, pHandleState->nParamListNumUsed );

        if (( bNewShader ) && ( pHandleState->pgm ))
        {
            SHADER_DBG_PRINTF( "  cgGLBindProgram( %d )\n", pHandleState->pgm );
            rdrBeginMarker("calling cgGLBindProgram");
            cgGLBindProgram( pHandleState->pgm ); CHECKGL;
            rdrEndMarker();
            ReviewShaderBindResults( "Combination Shader", pHandleState->pgm );
        }
        else
        {
            CGFX_UPDATE_PROGRAM_PARAMS( pHandleState->pgm );
        }

    } // pHandleState != NULL
    
    sCGProgramTable.boundShaderSpec[kShaderPgmType_VERTEX]                = nCurrVertPgmID;
    sCGProgramTable.boundShaderSpec[kShaderPgmType_FRAGMENT]            = nCurrFragPgmID;

    rdrEndMarker();

    SHADER_DBG_PRINTF( "END ShaderPrep_BindSelectedShadersAsCombo()\n" );
}
#endif // #if RT_SUPPORT_CG_COMBO_SHADERS

/****************************************************************************
    ShaderPrep_BindSelectedShader
    
****************************************************************************/
static void ShaderPrep_BindSelectedShader( tShaderProgramType nPgmTypeI )
{
    GLuint nCurrPgmID;
    GLuint nPrevPgmID;
    CGprogram cgPgm = NULL;
    int nPgmTableIndex = -1;
    bool bNewShader = false;
    
    SHADER_DBG_PRINTF( "BEGIN ShaderPrep_BindSelectedShader(%d) - shader=%d\n",
        nPgmTypeI, 
        sCGProgramTable.lastShaderBindRequest[nPgmTypeI] );

    nPrevPgmID    = sCGProgramTable.boundShaderSpec[nPgmTypeI];
    
    nCurrPgmID    = nPrevPgmID;

    if ( sCGProgramTable.enabledProfile[nPgmTypeI] == 0 )
    {
        nCurrPgmID = kVirtualShader_UseFixedFunction;
        bNewShader = true;
    }
    if ( sCGProgramTable.lastShaderBindRequest[nPgmTypeI] != -1 )
    {
        SHADER_DBG_PRINTF( "    sCGProgramTable.lastShaderBindRequest[%d] != -1\n", nPgmTypeI);
        nCurrPgmID = sCGProgramTable.lastShaderBindRequest[nPgmTypeI];
        if ( nCurrPgmID != nPrevPgmID )
        {
            SHADER_DBG_PRINTF( "    nCurrPgmID != nPrevPgmID\n");
            if ( nCurrPgmID == kVirtualShader_NotSet )
            {
                SHADER_DBG_PRINTF( "    nCurrPgmID = kVirtualShader_UseFixedFunction\n");
                nCurrPgmID = kVirtualShader_UseFixedFunction;
            }
            bNewShader = true;
        }
    }

    rdrBeginMarker(__FUNCTION__ ":%s(%d)", ParamTable_ShaderFileName(nCurrPgmID),nCurrPgmID);

    //
    //    Find or build a shader handle
    //
    if ( bNewShader )
    {
        //
        //    We have some work to do.
        //
        rdrBeginMarker("bNewShader");
        SHADER_DBG_PRINTF( "    We have some work to do.\n" );    
        #if 0    // With "hard constant" shader param updating we currently don't need this
            WCW_SetParamDirtyFlags( 
                ( nPgmTypeI == kShaderPgmType_FRAGMENT ), 
                ( nPgmTypeI == kShaderPgmType_VERTEX ) );    // shader change, so we need to update the params        
        #endif

        cgPgm = ParamTable_GetProgramTblItemForPgmId( nPgmTypeI, nCurrPgmID )->srcPgm;
        if ( VALID_SHADER_PGM( nCurrPgmID ))
        {
            if ( ! cgGLIsProgramLoaded( cgPgm ) )
            {
                cgGLLoadProgram( cgPgm ); CHECKGL;
                ReviewShaderLoadResults( "Combination Shader", cgPgm );
            }
        }
        else
        {
            SHADER_DBG_PRINTF( "  cgGLBindProgram() - no shader bound!\n" );
        }
        rdrEndMarker();
    }    // bNewShader to set up
    else
    {
        SHADER_DBG_PRINTF( "No shader bind changes in ShaderPrep_BindSelectedShadersAsCombo().\n" );
    }

    if (( bNewShader ) && ( cgPgm != NULL ))
    {
        SHADER_DBG_PRINTF( "  cgGLBindProgram( %d )\n", cgPgm );
        rdrBeginMarker("calling cgGLBindProgram");
        cgGLBindProgram( cgPgm ); CHECKGL;
        rdrEndMarker();
        ReviewShaderBindResults( "Combination Shader", cgPgm );
    }
    
    sCGProgramTable.boundShaderSpec[nPgmTypeI]    = nCurrPgmID;

    rdrEndMarker();

    SHADER_DBG_PRINTF( "END ShaderPrep_BindSelectedShader()\n" );
}

/****************************************************************************
    rt_cgGetActiveProgram
    
****************************************************************************/
CGprogram rt_cgGetActiveProgram()
{
    return ( sCGProgramTable.activeComboHandleState != NULL ) ? 
                sCGProgramTable.activeComboHandleState->pgm : NULL;
}

/****************************************************************************
    ShaderPrep_EnableSelectedProfiles
    
****************************************************************************/
static void ShaderPrep_EnableSelectedProfiles( void )
{
    int pgmTypeI;
    SHADER_DBG_PRINTF( "ShaderPrep_EnableSelectedProfiles(). Profile requests: Vertex=%d, Fragment=%d.\n",
                            sCGProgramTable.lastProfileEnableRequest[kShaderPgmType_VERTEX],
                            sCGProgramTable.lastProfileEnableRequest[kShaderPgmType_FRAGMENT] );
    for ( pgmTypeI=0; pgmTypeI < kShaderPgmType_COUNT; pgmTypeI++ )
    {
        ShaderPrep_EnableSelectedProfile( pgmTypeI );
    }
}

/****************************************************************************
    ShaderPrep_EnableSelectedProfile
    
****************************************************************************/
static void ShaderPrep_EnableSelectedProfile( tShaderProgramType pgmTypeI )
{
    if (( sCGProgramTable.lastProfileEnableRequest[pgmTypeI] != sCGProgramTable.enabledProfile[pgmTypeI] ))
    {
        if ( sCGProgramTable.enabledProfile[pgmTypeI] != 0 )
        {
            SHADER_DBG_PRINTF( "  cgGLDisableProfile( %s (%d) )\n", SHADER_DBG_PGM_TYPE_STR(pgmTypeI), sCGProgramTable.enabledProfile[pgmTypeI] );
            cgGLDisableProfile( sCGProgramTable.enabledProfile[pgmTypeI] ); CHECKGL;
        }

        if ( sCGProgramTable.lastProfileEnableRequest[pgmTypeI] != 0 )
        {
            SHADER_DBG_PRINTF( "  cgGLEnableProfile( %s (%d) )\n", SHADER_DBG_PGM_TYPE_STR(pgmTypeI), sCGProgramTable.lastProfileEnableRequest[pgmTypeI] );
            cgGLEnableProfile( sCGProgramTable.lastProfileEnableRequest[pgmTypeI] ); CHECKGL;
        }
        sCGProgramTable.enabledProfile[pgmTypeI] = sCGProgramTable.lastProfileEnableRequest[pgmTypeI];
    }
}

/****************************************************************************
    rt_cgFinalShaderPrep
    
****************************************************************************/
#if RT_SUPPORT_CG_COMBO_SHADERS
void rt_cgFinalShaderPrep()
{
    rdrBeginMarker(__FUNCTION__);
    SHADER_DBG_PRINTF( "\n====================== BEGIN: rt_cgFinalShaderPrep ====================\n" );
    ShaderPrep_EnableSelectedProfiles();
    ShaderPrep_BindSelectedShadersAsCombo();
    SHADER_DBG_PRINTF( "======================= END: rt_cgFinalShaderPrep =====================\n\n" );
    rdrEndMarker();
}
#endif

/****************************************************************************
    rt_cgSetFloatParam

    szDataFmtSpec: "C" or "R" for row or column major matrices. 
    NULL is OK for other types.
****************************************************************************/
void rt_cgSetFloatParam( CGparameter param, 
                            int elementRows, int elementCols, 
                            const GLfloat* pData, 
                            GLuint flags )
{
    SHADER_DBG_PRINTF( "      rt_cgSetFloatParam( param=%d, elementRows=%d, elementCols=%d,...)\n", param, elementRows, elementCols );
    switch ( elementCols )
    {
        case 3 :
        {
            switch ( elementRows )
            {
                case 1 :        // float3
                    cgSetParameter3fv( param, pData ); CHECKGL;
                    break;
                case 3 :        // float 3x3
                case 4 :        // float 3x4
                    // The kCgFX_SetFloat_RowMajor flag indicates we're asking Cg to initialize the
                    // underlying matrix with data arranged in row-major order.
                    if ( flags & kCgFX_SetFloat_AsMatrix )
                    {
                        cgGLSetMatrixParameterfc( param, pData ); CHECKGL;
                    }
                    else
                    {
                        cgSetParameterValuefc( param, (elementRows*elementCols), pData ); CHECKGL;
                    }
                    break;
                default :
                    assert( 0 );    // unsupported type
                    break;
            }
            break;
        }

        case 4 :
        {
            switch ( elementRows )
            {
                case 1 :        // float4
                    rdrSetMarker("%s(%f,%f,%f,%f)",
                        "cgSetParameter4fv",
                        pData[0],
                        pData[1],
                        pData[2],
                        pData[3]);

                    cgSetParameter4fv( param, pData ); CHECKGL;
                    break;
                case 3 :        // float 3x4
                case 4 :        // float 4x4
                    // The kCgFX_SetFloat_RowMajor flag indicates we're asking Cg to initialize the
                    // underlying matrix with data arranged in row-major order.
                    if ( flags & kCgFX_SetFloat_AsMatrix )
                    {
                        cgGLSetMatrixParameterfc( param, pData ); CHECKGL;
                    }
                    else
                    {
                        cgSetParameterValuefc( param, (elementRows*elementCols), pData ); CHECKGL;
                    }                    
                    break;
                default :
                    assert( 0 );    // unsupported type
                    break;
            }
            break;
        }
        
        default :
            assert( 0 );    // unsupported type
            break;
    }
}

/****************************************************************************
    CgShaderSpecList_ReleaseMem
    
****************************************************************************/
static void CgShaderSpecList_ReleaseMem( tCgShaderSpec* pPgmTblItem )
{
    if ( pPgmTblItem->srcPgm != NULL )
    {
        cgDestroyProgram( pPgmTblItem->srcPgm );
    }
    pPgmTblItem->srcPgm = NULL;
    pPgmTblItem->shaderHdlTblOfs = -1;
}

/****************************************************************************
    ComboShaderHandleState_ResizeParamList
    
****************************************************************************/
static void ComboShaderHandleState_ResizeParamList( tComboShaderHandleState* pHandleState, GLuint nNewSize )
{
    static const int kGrowSz = 4;
    StructList_Resize( 
                &(void*)pHandleState->pParamList,
                sizeof(tCgParamCacheSpec),
                &pHandleState->nParamListArraySz,
                nNewSize,
                kGrowSz );
}

/****************************************************************************
    ComboShaderHandleState_ReleaseMem
    
****************************************************************************/
static void ComboShaderHandleState_ReleaseMem( tComboShaderHandleState* pHandleState )
{
    if (( pHandleState->pgm != NULL ) &&
        ( VALID_SHADER_PGM( pHandleState->srcFragmentPgmId ) ) &&
        ( VALID_SHADER_PGM( pHandleState->srcVertexPgmId ) ))
    {
        cgDestroyProgram( pHandleState->pgm );
        pHandleState->pgm = NULL;
    }    // ...Otherwise, the program handle is a non-combo shader and will
        // be released when the main shader list is destroyed.
        
    if ( pHandleState->nParamListArraySz > 0 )
    {
        GLuint i;
        for ( i=0; i < pHandleState->nParamListNumUsed; i++ )
        {
            CgParamCacheSpec_ParamHdlList_ReleaseMem( &pHandleState->pParamList[i] );
        }
        free( pHandleState->pParamList );
        pHandleState->pParamList = NULL;
        pHandleState->nParamListArraySz = 0;
        pHandleState->nParamListNumUsed = 0;
    }
}

/****************************************************************************
    CgProgramTable_ResizeFragmentShaderCombos
    
****************************************************************************/
static void CgProgramTable_ResizeFragmentShaderCombos( GLuint nNewSize )
{
    static const int kGrowSz = 4;
    StructList_Resize( 
                &(void*)sCGProgramTable.fragmentShaderVertCombos,
                sizeof(tFragmentShaderVertCombos),
                &sCGProgramTable.fragmentShaderVertCombosArrSz,
                nNewSize,
                kGrowSz );
}

/****************************************************************************
    FragmentShaderVertCombos_ReleaseMem
    
****************************************************************************/
static void FragmentShaderVertCombos_ReleaseMem( tFragmentShaderVertCombos* pComboInfo )
{
    if ( pComboInfo->handleStates != NULL )
    {
        GLuint i;
        for ( i=0; i < pComboInfo->vertexHdlStatesNum; i++ )
        {
            ComboShaderHandleState_ReleaseMem( &pComboInfo->handleStates[i] );
        }
        free( pComboInfo->handleStates );
        pComboInfo->handleStates = NULL;
        pComboInfo->vertexHdlStatesArrSz = 0;
        pComboInfo->vertexHdlStatesNum = 0;
    }
}

/****************************************************************************
    CgParamCacheSpec_Resize
    
****************************************************************************/
static void CgParamCacheSpec_ParamHdlList_Resize( tCgParamCacheSpec* pSpec, GLuint nNewSize )
{
    static const int kGrowSz = 4;
    StructList_Resize( 
                &(void*)pSpec->paramHdlList,
                sizeof(CGparameter),
                &pSpec->nParamHdlArraySz,
                nNewSize,
                kGrowSz );
}

/****************************************************************************
    StructList_Resize
    
****************************************************************************/
static bool StructList_Resize( void** ppList, size_t nListItemStructSz, GLuint* pnArrSz, GLuint nNewSize, GLuint nGrowSz )
{
    if ( nNewSize > *pnArrSz )
    {
        GLuint nNewNumItems = RoundUp( nNewSize, nGrowSz );
        size_t nOldBytes = ( nListItemStructSz * (*pnArrSz) );
        size_t nNewBytes = ( nListItemStructSz * nNewNumItems );
        void* pNewList = calloc( nNewBytes, 1 );
        if ( *pnArrSz > 0 )
        {
            memcpy( pNewList, *ppList, nOldBytes );
            free( *ppList );
        }        
        *ppList = pNewList;
        *pnArrSz = nNewNumItems;
        
        return true;
    }
    return false;
}

/****************************************************************************
    CgParamCacheSpec_ParamHdlList_ReleaseMem
    
****************************************************************************/
static void CgParamCacheSpec_ParamHdlList_ReleaseMem( tCgParamCacheSpec* pSpec )
{
    if ( pSpec->paramHdlList != NULL )
    {
        free( pSpec->paramHdlList );
    }
    pSpec->paramHdlList = NULL;
    pSpec->nNumParamHdls = 0;
}

/****************************************************************************
    ReviewShaderLoadResults

****************************************************************************/
static tCgFxError ReviewShaderLoadResults( const char* szProgramName, CGprogram prog )
{
    tCgFxError nRslt = kCgFxError_NONE;
    bool bWarnings = false;

    CheckAndReportCgErrors( szProgramName, "loading", prog, &nRslt, &bWarnings  );
    // If Cg had an error loading the ARB error string can be informative
    // of what implementation limitation is causing the trouble:
    if (nRslt != kCgFxError_NONE)
    {
        const char * errstring = glGetString(GL_PROGRAM_ERROR_STRING_ARB);
        if (errstring)
        {
            consoleSetColor(COLOR_RED|COLOR_BRIGHT, 0);
            printf("[ARB]-%s\n", errstring);
            consoleSetDefaultColor();
        }
    }
    return nRslt;
}

/****************************************************************************
    ReviewShaderBindResults

****************************************************************************/
static tCgFxError ReviewShaderBindResults( const char* szProgramName, CGprogram prog )
{
    tCgFxError nRslt = kCgFxError_NONE;
    bool bWarnings = false;

    CheckAndReportCgErrors( szProgramName, "binding", prog, &nRslt, &bWarnings  );
    return nRslt;
}

/****************************************************************************
    ReviewProfileEnableResults

****************************************************************************/
static tCgFxError ReviewProfileEnableResults( const char* szProgramName, CGprogram prog )
{
    tCgFxError nRslt = kCgFxError_NONE;
    bool bWarnings = false;

    CheckAndReportCgErrors( szProgramName, "enabling the profile for", prog, &nRslt, &bWarnings  );
    return nRslt;
}


/****************************************************************************
    ValidateCgProfileConstant

    Asserts false on failure.    
****************************************************************************/
static bool ValidateCgProfileConstant( tCgShaderProfile profile )
{
    int i;
    for ( i=0; i < ARRAY_SIZE(sShaderProfileTbl); i++ )
    {
        if ( sShaderProfileTbl[i].nProfileConst == profile )
        {
            return true;
        }
    }
    assert( 0 );
    return false;
}

/****************************************************************************
    rt_cgfxGetProfileInfo
    
    Returns true if a match was found.
****************************************************************************/
bool rt_cgfxGetProfileInfo(    tCgShaderProfile profile, 
                            char const ** ppProfileDescr,    /* OUT */
                            char* pCompilerCmdLineStrs,        /* OUT: pCompilerCmdLineStrs is set to a space-separated
                                                                list of command line args to include when calling the
                                                                shader compiler for this profile (or "" if none is 
                                                                needed) */
                            size_t compilerCmdLineStrsBuffSz )
{
    int i=0;
    bool bFound = false;
    assert( pCompilerCmdLineStrs );
    assert( compilerCmdLineStrsBuffSz > 0 );

    *pCompilerCmdLineStrs = '\0';
    for ( i=0; i < ARRAY_SIZE(sShaderProfileTbl); i++ )
    {
        if ( sShaderProfileTbl[i].nProfileConst == profile )
        {
            *ppProfileDescr = sShaderProfileTbl[i].shaderProfileName;
            if ( sShaderProfileTbl[i].compilerParams[0] != '\0' )
            {
                strncpy_s( pCompilerCmdLineStrs, 
                            compilerCmdLineStrsBuffSz, 
                            sShaderProfileTbl[i].compilerParams, 
                            _TRUNCATE );
            }
            bFound = true;
            break;
        }
    }
    return bFound;
}
