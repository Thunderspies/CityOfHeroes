/****************************************************************************
    rt_cgfx.h
    
    Main header for CGFX support.
    
    Author: LStLezin, 01/2009
****************************************************************************/
#ifndef __rt_cgfx_h__
#define __rt_cgfx_h__

#include "render/thread/ogl.h"
#include <Cg/cgGL.h>
#include "seq/tricks.h"
#include "render/thread/rt_tricks.h"
#include "render/thread/rt_model.h"
#include "cmdparse/cmdgame.h"
#include "../rendercgfx.h"
#include "render/thread/wcw_statemgmt.h"

typedef enum
{
    kCgFX_SetFloat_AsMatrix            = 1 << 1
} tCgFX_SetFloatFlags;

//---------------------------------------------------------------------------
//
//    Macros
//
//---------------------------------------------------------------------------
// Locations of shader files.
#define RT_CGFX_SHADER_PATH_BASE            "shaders/cgfx"
#define RT_CGFX_SHADER_PATH_EFFECTS            RT_CGFX_SHADER_PATH_BASE "/effects"

//---------------------------------------------------------------------------
//
//    FUNCTION PROTOTYPES
//
//---------------------------------------------------------------------------
tCgFxError    rt_cgfxInit( void );
tCgFxError    rt_cgfxDeInit( void );
CGcontext    rt_cgfxGetGlobalContext( void );

//
// CG API error checking
//
CGerror        rt_cgfxGetLastCgError( void );
void        rt_cgfxClearLastError( void );


//
//    Basic Cg API
//
typedef CGprofile tCgShaderProfile; // Valid constants: CG_PROFILE_ARBVP1, CG_PROFILE_ARBFP1

typedef enum tCgShaderMode
{
    tCgShaderMode_Disabled = 0,
    tCgShaderMode_ARB,
    tCgShaderMode_GLSL,
    tCgShaderMode_3,
    tCgShaderMode_4,
    tCgShaderMode_GPU,
    tCgShaderMode_MAX
} tCgShaderMode;

extern tCgShaderMode g_rtCgFX_CgShaderMode;
void        rt_cgSetCgShaderMode( tCgShaderMode shaderMode );
static __inline tCgShaderMode rt_cgGetCgShaderMode() { return g_rtCgFX_CgShaderMode; }

typedef struct tCgProgramProfileMapping {
    tCgShaderProfile profile[kShaderPgmType_COUNT];
} tCgProgramProfileMapping;

extern const tCgProgramProfileMapping sCgProgramProfileMapping[];
static __inline tCgShaderProfile rt_cgGetCgShaderProfileForMode(tCgShaderMode shaderMode, tShaderProgramType target)
{
    assert(shaderMode >= 0 && shaderMode < tCgShaderMode_MAX);
    assert(target >= 0 && target <= kShaderPgmType_COUNT);
    return sCgProgramProfileMapping[shaderMode].profile[target];
}

tCgShaderProfile rt_cgGetCgShaderProfileForMode(tCgShaderMode shaderMode, tShaderProgramType target);

    // Shader Ids are used to access the cached program handles that we manage.
    //    1. Reserve a set of shader ID numbers,
    //    2. Build a shader program with rt_cgCreateProgramFromBuffer() (or some other way),
    //     and then cache the program handle using rt_cgPrepareAndCacheProgramHdl().
void        rt_cgGenerateCgShaderIds( tShaderProgramType target, GLuint nCount /*IN*/, GLuint* pgmIds /*OUT*/ );
void        rt_cgResetCgShaderIds( void );    // afterwards, you must call rt_cgGenerateCgShaderIds() again.
void        rt_cgResetComboShaderHandles();    // forces the module to rebuild combination frag/vert program handles on the fly.
CGprogram    rt_cgCreateProgramFromBuffer( const char* szProgramName, CGenum program_type, 
                const char *program, tShaderProgramType target, 
                tCgShaderProfile profile, const char *entry, 
                const char **args);                
void        rt_cgPrepareAndCacheProgramHdl( tShaderProgramType target, tCgShaderProfile profile,
                                            GLuint pgmId, CGprogram program, 
                                            const char* szSrcFile /*for reporting errors only*/ );

    // Using a shader for drawing:
    //    1. Enable the profile (vertex or fragment program)
    //    2. Bind the program by referencing the cached location (obtained with rt_cgGenerateCgShaderIds()).
void        rt_cgEnableProgram(tShaderProgramType target, tCgShaderProfile profile );
void        rt_cgDisableProgram(tShaderProgramType target );
void        rt_cgResetProgram(tShaderProgramType target );
CGprogram    rt_cgGetActiveProgram();    // may return NULL if none is bound

void        rt_cgBindCgProgram(tShaderProgramType target, GLuint pgmId );
void        rt_cgFinalShaderPrep( void );

    // Set the shader param. The elementRows and elementCols imply a data type (e.g., float4, float3x4, etc.)
void        rt_cgSetFloatParam( CGparameter param, int elementRows, int elementCols, 
                                    const GLfloat* pData, GLuint flags /* tCgFX_SetFloatFlags */ );
                                    
void        rt_cgfxSetDisplayWarnings( bool bShowWarnings );
bool        rt_cgfxGetDisplayWarnings( void );

//---------------------------------------------------------------------------
//    tCgParamCacheSpec
//---------------------------------------------------------------------------
typedef struct tCgParamCacheSpec
{
    int                paramNmCacheKey;    // used for caching the param
    CGparameter*    paramHdlList;        // an array param can have more than one handle
    GLuint            nNumParamHdls;
    GLuint            nParamHdlArraySz;
} tCgParamCacheSpec;

    // Returns true if match was found.
bool        rt_cgfxGetProfileInfo(     tCgShaderProfile profile,
                                    char const ** ppProfileDescr,    /* OUT */
                                    char* pCompilerCmdLineStrs,        /* OUT: pCompilerCmdLineStrs is set to a space-separated
                                                                        list of command line args to include when calling the
                                                                        shader compiler for this profile (or "" if none is 
                                                                        needed) */
                                    size_t compilerCmdLineStrsBuffSz );

//
//    Basic CgFX API
//
CGeffect    rt_cgfxGetEffect( const char* effectName /* just the root name, no path or ext */, 
                                const char** args, bool bForceReload );
CGtechnique    rt_cgfxGetTechnique( CGeffect effect, const char* techniqueName );
CGpass        rt_cgfxGetPass( CGtechnique technique, const char* passName );

tCgFxError    rt_cgfxSetRenderPass( CGtechnique technique, CGpass pass  /* NULL=first pass found */);
tCgFxError    rt_cgfxUnsetRenderPass( CGtechnique technique, CGpass pass  /* NULL=first pass found */);

tCgFxError    rt_cgfxCompileProgram( CGprogram prog );
void        rt_cgfxReloadEffect( const char* szCgFxRelPath );

//
//    API for Cg replacement of legacy ARB shaders
//    


#endif // __rt_cgfx_h__
