#ifndef _WCW_STATE_MANAGEMENT
#define _WCW_STATE_MANAGEMENT

#include "render/renderUtil.h"
#include "render/thread/rt_tex.h"
#include "render/thread/ogl.h"
#include <Cg/cgGL.h>

// Remove this when we no longer need to set ARB registers
#define RT_SUPPORT_ARB_SHADER_PATH 1

// Set this to 1 to enable combination CG shaders. This also
// activates code which caches shader bind requests, shader param
// updates, and OpenGL state updates until WCW_PreprareToDraw() is called.
#define RT_SUPPORT_CG_COMBO_SHADERS 0

// Macro which can be used in the code to determine whether to take the 
// hard-const (non-combo shader) path or the named-param / combo-shader path.
// (The two go hand-in-hand.)
#if RT_SUPPORT_CG_COMBO_SHADERS
    #define RT_USE_COMBO_SHADERS()    (!game_state.use_hard_shader_constants)
#else
    #define RT_USE_COMBO_SHADERS()    (false)
#endif
#define RT_USE_HARD_CONST_PARAM_MODE()    (!RT_USE_COMBO_SHADERS())


//
// If set to 1 this enables some additional debugging code, including
// SHADER_DBG_PRINTF() statements, which are dumped to a c:\temp\SHADER_DBG_LOG_%date%_f%frame%.txt
// log file for 1 frame, when you press the [ALT] key. Shift-ALT will log 50 frames.
//
// SHADER_DBG_CODE_ENABLED also
//      - causes extra asserts to be compiled in.
//      - gpumarkers (if enabled) will also output to the shader debug log for the recording duration
//
#ifndef FINAL
#define SHADER_DBG_CODE_ENABLED 0
#endif

typedef enum
{
    GLC_VERTEX_ATTRIB_ARRAY1_NV = 0,
    GLC_VERTEX_ATTRIB_ARRAY5_NV,
    GLC_VERTEX_ATTRIB_ARRAY6_NV,
    GLC_VERTEX_ATTRIB_ARRAY7_NV,
    GLC_TEXTURE_COORD_ARRAY_0,
    GLC_TEXTURE_COORD_ARRAY_1,
    GLC_TEXTURE_COORD_ARRAY_2,
    GLC_TEXTURE_COORD_ARRAY_3,
    GLC_TEXTURE_COORD_ARRAY_4,
    GLC_TEXTURE_COORD_ARRAY_5,
    GLC_TEXTURE_COORD_ARRAY_6,
    GLC_TEXTURE_COORD_ARRAY_7,
    GLC_VERTEX_ARRAY,
    GLC_NORMAL_ARRAY,
    GLC_COLOR_ARRAY,
    GLC_VERTEX_ATTRIB_ARRAY11_NV,
    GLC_COUNT,
} GLClientStateType;

#define MAX_TEXTURE_UNITS_TOTAL    16
extern int gl_tex_nums[];
extern bool g_renderstate_dirty;

typedef struct FogContext
{
    int on;
    int last_on;
    int fog_deferring;
    F32 curr_fog[4], old_fog[4];    // index is [GL_FOG_DENSITY or GL_FOG_START or GL_FOG_END or GL_FOG_MODE] - GL_FOG_DENSITY
    int fog_stack[4];
    int fog_stack_depth;
    int fog_doneonce;    
    Vec4 fog_color;
} FogContext;

void WCW_InitOnce(void);
void WCW_Shutdown(void);

void WCW_EnableClipPlanes(int numPlanes, GLfloat planes[6][4]);
void WCW_ManageClipPlaneProjection(void);

void WCW_EnableStencilTest(void);
void WCW_StencilFunc( GLenum func, GLint ref, GLuint mask );
void WCW_StencilOp( GLenum fail, GLenum zfail, GLenum zpass );
void WCW_DisableStencilTest(void);

#if RT_SUPPORT_CG_COMBO_SHADERS
    void WCW_PrepareToDrawCG();
    #define WCW_PrepareToDraw()    if ( RT_USE_COMBO_SHADERS() ) WCW_PrepareToDrawCG()
#else
    #define WCW_PrepareToDraw()
#endif

void WCW_LoadModelViewMatrix(const Mat4 m);
void WCW_LoadModelViewMatrixM44(const Mat44 m);
void WCW_LoadModelViewMatrixIdentity();
void WCW_SetModelViewMatrixDirty();

void WCW_LoadProjectionMatrix(const Mat44 m);
void WCW_LoadProjectionMatrixIdentity();
void WCW_SetProjectionMatrixDirty();

void WCW_LoadTextureMatrix(int texindex, const Mat4 mat);
void WCW_LoadTextureMatrixIdentity(int texindex);
void WCW_ScaleTextureMatrix(int texindex, const Vec3 scale);
void WCW_TranslateTextureMatrix(int texindex, const Vec3 translation);

void WCW_LoadColorMatrix(const Mat44 c);
void WCW_LoadColorMatrixIdentity();

void WCW_Light( GLenum l, GLenum pname, const float light[] );
void WCW_LightPosition( const float light[], const Mat4 lightMtx );

void WCW_Color4(U8 red, U8 green, U8 blue, U8 alpha);
#define WCW_Color3(red, green, blue) WCW_Color4(red, green, blue, 255)
static INLINEDBG void WCW_ColorU32(U32 AGBR)
{
    typedef union tempColorUnion
    {
        U8 components[4];
        U32 color;
    } tempColorUnion;

    tempColorUnion temp;
    temp.color = AGBR;

    WCW_Color4(temp.components[0], temp.components[1], temp.components[2], temp.components[3]);
}
// Call this if you had to call glColor() inside of a glBegin()/glEnd()
void WCW_Color4_Force(U8 red, U8 green, U8 blue, U8 alpha);

void WCW_Material(GLenum face, GLenum pname, const float *params);

void WCW_SunLightDir(void);
void WCW_ConstantCombinerParamerterfv( int idx, float constColor[] );
void WCW_DepthMask( GLboolean mask );

void WCW_ActiveTexture( int texindex );
void WCW_ClientActiveTexture( int texindex );

#ifdef RT_ALLOW_BINDTEXTURE
// this function should not be called directly. call texBind instead.
void WCW_BindTexture( GLenum target, int texindex, GLuint tex );
void WCW_BindTextureFast( GLenum target, int texindex, GLuint tex );
#endif

void WCW_TexCoordPointer( int texindex, GLint size, GLenum type,  GLsizei stride, const GLvoid *pointer );
void WCW_NormalPointer( GLenum type,  GLsizei stride, const GLvoid *pointer );
void WCW_VertexPointer( GLint size, GLenum type,  GLsizei stride, const GLvoid *pointer );

void WCW_FetchGLState(void);
void WCW_CheckGLState(void);
void WCW_ResetState(void);
void WCW_ResetTextureState(void);
void WCW_ResetState_CgFx(void);
void WCW_ClearTextureStateToWhite(void);

void WCW_EnableColorMaterial(void);

void WCW_EnableClientState(GLClientStateType array);
void WCW_DisableClientState(GLClientStateType array);
void WCW_DisableAllClientState(void);

void WCW_VertexWeighting( int on );
void WCW_EnableGL_NORMALIZE( int on );
void WCW_EnableGL_LIGHTING( int on );
void WCW_CancelOverrideGL_LIGHTING( void );
void WCW_OverrideGL_LIGHTING( int on );

void WCW_TexLODBias(int texindex, F32 newbias);

void WCW_Fog(int on);
void WCW_FogPush(int on);
void WCW_FogPop(void);
void WCW_Fogf(GLenum pname, GLfloat param);
void WCW_FogColor(Vec3 fog_color);

GLuint WCW_BindBuffer(GLsizei target, GLuint buffer);
void WCW_BufferData(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage,const char *memname);
void WCW_DeleteBuffers(GLenum target,GLsizei n, const GLuint *buffers,const char *memname);

void WCW_EnableVertexProgram(void);
void WCW_BindVertexProgram(GLuint id);
void WCW_DisableVertexProgram(void);
void WCW_ResetVertexProgram(void);

void WCW_EnableFragmentProgram(void);
void WCW_BindFragmentProgram(GLuint id);
void WCW_DisableFragmentProgram(void);
void WCW_ResetFragmentProgram(void);

void WCW_EnableTexture(GLenum tex_target, int texindex);
void WCW_DisableTexture(int texindex);
void WCW_DisableAllTextures(void);

void WCW_EnableBlend(void);
void WCW_DisableBlend(void);

void WCW_BlendFunc(GLenum sfactor, GLenum dfactor);
void WCW_BlendFuncPush(GLenum sfactor, GLenum dfactor);
void WCW_BlendFuncPushNop(void);
void WCW_BlendFuncPop(void);
void WCW_BlendStackFreeze(int freeze);

void WCW_InitFogContext(FogContext *fog); // also sets the context
void WCW_SetFogContext(FogContext *fog);
const FogContext *WCW_GetFogContext(void);

void WCW_SetFogDeferring(int deferring);
int WCW_FogDeferring();

typedef enum tShaderProgramType {
    kShaderPgmType_FRAGMENT = 0,
    kShaderPgmType_VERTEX = 1,
    //-----
    kShaderPgmType_COUNT
} tShaderProgramType;

#define ValidateShaderProgramTypeConstant( _target_ ) \
    onlydevassert(( _target_ == kShaderPgmType_VERTEX ) || \
           ( _target_ == kShaderPgmType_FRAGMENT ))


typedef enum ShaderParamId {

    // ======================= Matrix params ====================== 

    kMatrixParam_ModelView,
    kMatrixParam_ModelViewIT,    // inverse transpose
    kMatrixParam_Projection,
    kMatrixParam_ModelViewProj,
    kMatrixParam_Texture0,
    kMatrixParam_Texture1,
    kMatrixParam_Texture2,
    kMatrixParam_Texture3,
    kMatrixParam_Texture4,
    kMatrixParam_Texture5,
    kMatrixParam_Texture6,
    kMatrixParam_Texture7,
    kMatrixParam_Texture8,
    kMatrixParam_Texture9,
    kMatrixParam_Texture10,
    kMatrixParam_Texture11,
    kMatrixParam_Texture12,
    kMatrixParam_Texture13,
    kMatrixParam_Texture14,
    kMatrixParam_Texture15,
    kMatrixParam_Color,

    // ===================== OpenGL State (VP) ==================== 

    kOpenGLParam_LightProd0DiffuseVP,    
    kOpenGLParam_LightProd1DiffuseVP,    
    kOpenGLParam_LightProd0AmbientVP,    
    kOpenGLParam_LightProd1AmbientVP,    
    kOpenGLParam_LightProd0SpecularVP,
    kOpenGLParam_LightProd1SpecularVP,

    kOpenGLParam_Light0PositionVP,    
    kOpenGLParam_Light1PositionVP,    

    kOpenGLParam_Light0HalfVP,        
    kOpenGLParam_Light1HalfVP,        

    kOpenGLParam_Light0DiffuseVP,        
    kOpenGLParam_Light1DiffuseVP,        

    kOpenGLParam_Light0AmbientVP,        
    kOpenGLParam_Light1AmbientVP,        

    kOpenGLParam_Light0SpecularVP,    
    kOpenGLParam_Light1SpecularVP,
    
    kOpenGLParam_MaterialShininessVP,

    kOpenGLParam_ClipPlane0VP,
    kOpenGLParam_ClipPlane1VP,
    kOpenGLParam_ClipPlane2VP,
    kOpenGLParam_ClipPlane3VP,
    kOpenGLParam_ClipPlane4VP,
    kOpenGLParam_ClipPlane5VP,

    // =================== Vertex program params =================== 

    kShaderParam_LightDirVP,                      // formerly, C0
    kShaderParam_ViewerPositionVP,                // formerly, C2
    kShaderParam_AmbientParameterVP,              // formerly, C4
    kShaderParam_DiffuseParameterVP,              // formerly, C5
    kShaderParam_TexScroll0VP,                    // formerly, C14
    kShaderParam_TexScroll1VP,                    // formerly, C15

    kShaderParam_ReflectionParamVP,               // formerly, C1
    kShaderParam_InverseCubemapViewMatrixVP,      // formerly, C3..C5

    kShaderParam_BoneMatrixArrVP,                 // formerly, C16...63

    // Planar reflection
    kShaderParam_PlanarReflectionPlaneVP,

    // ==================== OpenGL States (FP) ==================== 

    kOpenGLParam_FogParamsFP,
    kOpenGLParam_FogColorFP,
    
    kOpenGLParam_LightProd0DiffuseFP,    
    kOpenGLParam_LightProd1DiffuseFP,    
    kOpenGLParam_LightProd0AmbientFP,    
    kOpenGLParam_LightProd1AmbientFP,    
    kOpenGLParam_LightProd0SpecularFP,
    kOpenGLParam_LightProd1SpecularFP,
    
    kOpenGLParam_TexSampler0,
    kOpenGLParam_TexSampler1,
    kOpenGLParam_TexSampler2,
    kOpenGLParam_TexSampler3,
    kOpenGLParam_TexSampler4,
    kOpenGLParam_TexSampler5,
    kOpenGLParam_TexSampler6,
    kOpenGLParam_TexSampler7,
    kOpenGLParam_TexSampler8,
    kOpenGLParam_TexSampler9,
    kOpenGLParam_TexSampler10,
    kOpenGLParam_TexSampler11,
    kOpenGLParam_TexSampler12,
    kOpenGLParam_TexSampler13,
    kOpenGLParam_TexSampler14,
    kOpenGLParam_TexSampler15,

    // =================== Fragment program params =================== 

    kShaderParam_Effects_TextTransformFP,         // formerly, C0
    kShaderParam_Effects_ExpectedLumFP,           // formerly, C0
    kShaderParam_Effects_BlurAmtAndExposureFP,    // formerly, C0
    kShaderParam_Effects_TimeStepFP,              // formerly, C0
    kShaderParam_Effects_DofParam2FP,             // formerly, C1
    kShaderParam_Effects_DofProjectFP,            // formerly, C2
    kShaderParam_Effects_DesaturateParamFP,       // formerly, C3

    kShaderParam_Effects_GfxPerfTestParam0FP,     // formerly, C0
    kShaderParam_Effects_GfxPerfTestParam1FP,     // formerly, C1
    kShaderParam_Effects_GfxPerfTestParam2FP,     // formerly, C2

    kShaderParam_AmbientColorFP,                  // formerly, C0
    kShaderParam_DiffuseColorFP,                  // formerly, C1
    kShaderParam_GlossParamFP,                    // formerly, C2
    kShaderParam_GlowParamFP,                     // formerly, C3
    kShaderParam_ConstColor0FP,                   // formerly, C3
    kShaderParam_ConstColor1FP,                   // formerly, C3
    kShaderParam_Specular1ColorAndExponentFP,     // formerly, C5
    kShaderParam_Specular2ColorAndExponentFP,     // formerly, C6
    kShaderParam_WaterReflectionTransformFP,      // formerly, C6
    kShaderParam_WaterRefractionTransformFP,      // formerly, C7
    kShaderParam_MiscParamFP,                     // formerly, C7

    kShaderParam_EnvVar0FP,                          // constColor0, formerly ENV0
    kShaderParam_EnvVar1FP,                          // constColor1, formerly ENV1

    // Revised MULTI9 constants (in lieu of interpolants)
    kShaderParam_BumpMultiFlagsFP,                    // faux reflection uv selector for multiply texture (mat1, mat2)
    kShaderParam_LightDirFP,                    // Pass view space light direction for HQ multi9 and bumpmap colorblend dual

    kShaderParam_ScrollScaleArrFP,                // formerly, C8...C19

    kShaderParam_WaterRefractionParamsFP,         // formerly, C21
    kShaderParam_WaterReflectionParamsFP,         // formerly, C22

    //****
    // BEGIN ULTRA CONSTANTS
    //****

    // reflection and env mapping
    kShaderParam_WaterFresnelParamsFP,            // formerly, C23
    kShaderParam_FresnelParamsFP,
    kShaderParam_DistortionParamsFP,

    kShaderParam_PlanarReflectionTransformFP,
    kShaderParam_InverseCubemapViewMatrixFP,

    // Shadow mapping
    kShaderParam_ShadowMap1MatrixFP,
    kShaderParam_ShadowMap2MatrixFP,
    kShaderParam_ShadowMap3MatrixFP,
    kShaderParam_ShadowMap4MatrixFP,
    kShaderParam_ShadowParamsFP,
    kShaderParam_ShadowSplitsFP,
    kShaderParam_ShadowParams2FP,
    kShaderParam_ShadowParams3FP,

    //****
    // DEBUG/TESTING (not used by retail shaders)
    //****
    kShaderParam_NewFeatureFP,
    kShaderParam_TestFlagsFP,                    // Test mode control param

    // Debug shader controls
    kShaderParam_DebugEnableFlagSetsFP,
    kShaderParam_DebugDisabledColorFP,

    //-------------------------
    kShaderParam_COUNT

} ShaderParamId;

// NOTE: ShaderParamId values are NOT the same as ARB register index values!
void        WCW_SetCgShaderParamArray4fv(tShaderProgramType target, ShaderParamId id, const GLfloat *vec4Arr, GLuint nNumVec4s );
#define        WCW_SetCgShaderParam4fv(_tShaderProgramType_target_, _ShaderParamId_id_, _const_GLfloat_p_vec4_ )    \
                WCW_SetCgShaderParamArray4fv( _tShaderProgramType_target_, _ShaderParamId_id_, _const_GLfloat_p_vec4_, 1  )
void        WCW_GetCgShaderParam4fv(tShaderProgramType target, GLuint n, GLfloat *vec4 /*OUT*/, bool* bDirty /*OUT*/);

void        WCW_SetCgShaderParamArray3fv(tShaderProgramType target, ShaderParamId id, const GLfloat *vec3Arr, GLuint nNumVec3s);
#define        WCW_SetCgShaderParam3fv(_tShaderProgramType_target_, _ShaderParamId_id_, _const_GLfloat_p_vec3_)    \
                WCW_SetCgShaderParamArray3fv( _tShaderProgramType_target_, _ShaderParamId_id_, ve_const_GLfloat_p_vec3_c3, 1 )

//--------------------------------------------------------------------------
//    CACHED Cg PARAM HANDLES
//
//    Client code can obtain a non-volatile "cache key" which can be used
//    to obtain shader param handles. Cache keys are automatically generated
//    for anything referenced via ShaderParamId. But if you manage shader
//    params "by hand" directly through Cg you can still take advantage
//    of the caching mechanism by obtaining cache keys for your shader's
//    param names. (Do this BEFORE shaders have been loaded in an init 
//    function at program startup.) There is one key for each name, so even
//    if two shaders use the same param name for completely different
//    purposes, that's OK; the key will still get you the right param for
//    the *current* shader at draw time. And the param handle will be correct
//    even if shaders have been reloaded or a new variant-compilation of the 
//    shader is now in place.
//
//--------------------------------------------------------------------------
int            WCW_GetCacheIdForShaderParamName( const char* szParamName, tShaderProgramType pgmType, bool bAdd );    // returns -1 if not found and bAdd is false.
         // Returns 'true' if we recognize the name and have set pElementRows and pElementCols.
void        WCW_GetFloatParamExpectedDimensions( int cacheKey, int* pElementRows, int* pElementCols );
bool        WCW_GetCachedParamHandleList( int cacheKey, CGparameter** ppParamHdlList, GLuint* pNumHdls );    // for the "active" shader of "target" type

            // Called by the Cg module to initiate the transfer of cached params
            // values to the shader.
void        WCW_UpdateShaderParamsFromLocalCache();

//    The shader manager makes this call to place the current param
//    handles into the cache.
void        WCW_CacheParamHandlesForShaderParam( CGprogram program, int nCacheId, 
                            CGparameter* paramList, int nListSz );    // record current shader's handles

void        WCW_SetParamDirtyFlags( bool bVertexPgm, bool bFragmentPgm );

//                            
// Additional shader logic debugging
//
#if SHADER_DBG_CODE_ENABLED
    //
    // The shader debug logging logic enables SHADER_DBG_PRINTF() message spew for one frame
    // when you hold the Alt key down.
    //
    #include <utilitieslib/utils/file.h>
    extern FILE* gSHADER_DBG_LOG_FILE;
    #define SHADER_DBG_LOG_FILE_NAME                        "SHADER_DBG_LOG.txt"
    #define SHADER_DBG_LOGGING_THIS_FRAME()                    ( gSHADER_DBG_LOG_FILE != NULL )
    #define SHADER_DBG_PRINTF(_msg_,...)                    if ( SHADER_DBG_LOGGING_THIS_FRAME() ) fprintf( gSHADER_DBG_LOG_FILE, (_msg_), __VA_ARGS__ )
    #define SHADER_DBG_DUMPFLOATS(_floatArr_,_numFloats_)                    \
                if ( SHADER_DBG_LOGGING_THIS_FRAME() )                            \
                {                                                                \
                    int dataI;                                                    \
                    float* pData = (float*)_floatArr_;                            \
                    fprintf( gSHADER_DBG_LOG_FILE,  "%d vals: ", _numFloats_ );    \
                    for ( dataI=0; dataI < _numFloats_; dataI++ )                \
                        fprintf( gSHADER_DBG_LOG_FILE, "%0.4f ", *pData++ );    \
                    fprintf( gSHADER_DBG_LOG_FILE, "\n" );                        \
                }
        // programmatically start logging for the rest of this frame
    void SHADER_DBG_LOG_TO_END_OF_FRAME( void );
    void SHADER_DBG_LOGGING_ON( void );
    void SHADER_DBG_LOGGING_OFF( void );
    #define SHADER_DBG_ASSERT(_condition_)    assert(_condition_)
#else
    #define SHADER_DBG_PRINTF(_msg_,...)
    #define SHADER_DBG_DUMPFLOATS(_floatArr_,_numFloats_)
    #define SHADER_DBG_LOG_TO_END_OF_FRAME()
    #define SHADER_DBG_LOGGING_ON()
    #define SHADER_DBG_LOGGING_OFF()
    #define SHADER_DBG_ASSERT(_condition_)
#endif
#define SHADER_DBG_PGM_TYPE_STR(_aType_)                (((_aType_)==kShaderPgmType_FRAGMENT) ? "kShaderPgmType_FRAGMENT" : "kShaderPgmType_VERTEX")

#endif
