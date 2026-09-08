#define RT_ALLOW_BINDTEXTURE
#define WCW_STATEMANAGER
#include "render/thread/ogl.h"
#include "assert.h"
#include "render/render.h"
#include "render/thread/wcw_statemgmt.h"
#include "render/thread/rt_cgfx.h"
#include "render/thread/shadersATI.h"
#include "edit/edit_cmd.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include "rt_stats.h"
#include <utilitieslib/utils/osdependent.h>
#include "graphics/failtext.h"
#include "render/renderWater.h"    // for kReflectProjClip_All, etc.
#include "graphics/gfxDebug.h"

#if defined(_FULLDEBUG)
    #define GLSTATEDEBUG 2
#elif defined(_DEBUG)
    #define GLSTATEDEBUG 1
#endif

#define ENABLE_SSE_OPTIMIZATIONS

#define ENABLE_ASSERTIONS (0 && (!defined(FINAL)))
#if ENABLE_ASSERTIONS
    #define WCW_ASSERT(_cond_)    assert(_cond_)
#else
    #define WCW_ASSERT(_cond_)
#endif

#ifdef ENABLE_SSE_OPTIMIZATIONS
#include <mmintrin.h>
#define SSE_PREFETCH_L1(ptr) _mm_prefetch(ptr, 1)
#define SSE_PREFETCH_L2(ptr) _mm_prefetch(ptr, 2)
#else
#define SSE_PREFETCH_L1(ptr) 
#define SSE_PREFETCH_L2(ptr) 
#endif

#ifndef FINAL
    int g_wcw_disable_state_management = 0;    // useful to toggle this on and off to test if rendering problems are due to state mirroring
    #define NO_STATEMANAGEMENT g_wcw_disable_state_management
    int g_wcw_disable_checkgl = 0;            // during an gl API capture it can be useful to disable all the extra API calls from checkgl
#else
    #define NO_STATEMANAGEMENT 0
#endif

#ifdef GLSTATEDEBUG
    #define ASSERT_GLINT(_pname, _value) do { GLint t; glGetIntegerv(_pname, &t); CHECKGL; assertmsgf(t == _value, "GLSTATEDEBUG error (wcw:0x%x gl:0x%x)", _value, t); } while(0)
    #define ASSERT_GLON(_pname, _value) do { GLboolean b = glIsEnabled(_pname); CHECKGL; assertmsgf(b == _value, "GLSTATEDEBUG error (wcw:%d gl:%d)", _value, b); } while(0)
#else
    #define ASSERT_GLINT(_pname, _value) do {} while(0)
    #define ASSERT_GLON(_pname, _value) do {} while(0)
#endif

typedef enum { 
    kDataSrc_PgmLocalVariable = 0,    // param binding is via a naming convention.
    kDataSrc_EnvVariable,            // not cleared when a new shader program is bound
    kDataSrc_OpenGLMatrix,            // dataKey1 of the param spec = tMatrixType
    kDataSrc_FogState,                // dataKey2 of the param spec = tFogStateType
    kDataSrc_LightingModel,            // dataKey1 is the type of lighting info, dataKey2 is the light index
    kDataSrc_Clipping,                // dataKey1 is the type of clipping info, dataKey2 is the index
    kDataSrc_TexSampler,            // dataKey1 is the texture unit #.
    //------------------------
    kDataSrc_COUNT
} tDataSrcType;

typedef enum {
    kMatrix_ModelView = 400,    // GL_MODELVIEW
    kMatrix_ModelViewIT,        // GL_MODELVIEW inverse transpose
    kMatrix_Projection,            // GL_PROJECTION
    kMatrix_ModelViewProj,        // GL_MODELVIEW & GL_PROJECTION
    kMatrix_Texture,            // GL_TEXTURE
    kMatrix_Color                // GL_COLOR
} tMatrixType;

typedef enum {
    kFogState_Params = 500,
    kFogState_Color
} tFogStateType;

typedef enum {
    kLightVal_Position = 600,        
    kLightVal_Half,            
    kLightVal_Diffuse,        
    kLightVal_Ambient,        
    kLightVal_Specular,
    kLightVal_MatShininess,
    kLightVal_ProdDiffuse,
    kLightVal_ProdAmbient,    
    kLightVal_ProdSpecular
} tLightValueType;

typedef enum {
    kClipInfo_Plane = 700
} tClipPlaneType;

#define kOpenGLDirtyBit_Matrix_ModelView            ((U64)1 <<  0)
#define kOpenGLDirtyBit_Matrix_ModelViewIT            ((U64)1 <<  1)
#define kOpenGLDirtyBit_Matrix_Projection            ((U64)1 <<  2)
#define kOpenGLDirtyBit_Matrix_ModelViewProjection    ((U64)1 <<  3)
#define kOpenGLDirtyBit_Matrix_Texture                ((U64)1 <<  4)
#define kOpenGLDirtyBit_Matrix_Color                ((U64)1 <<  5)

#define kOpenGLDirtyBit_FogState_Params                ((U64)1 <<  6)
#define kOpenGLDirtyBit_FogState_Color                ((U64)1 <<  7)

#define kOpenGLDirtyBit_LightVal_Position            ((U64)1 <<  8)
#define kOpenGLDirtyBit_LightVal_Half0                ((U64)1 <<  9)
#define kOpenGLDirtyBit_LightVal_Half1                ((U64)1 << 10)
#define kOpenGLDirtyBit_LightVal_Diffuse0            ((U64)1 << 11)
#define kOpenGLDirtyBit_LightVal_Diffuse1            ((U64)1 << 12)
#define kOpenGLDirtyBit_LightVal_Ambient0            ((U64)1 << 13)
#define kOpenGLDirtyBit_LightVal_Ambient1            ((U64)1 << 14)
#define kOpenGLDirtyBit_LightVal_Specular0            ((U64)1 << 15)
#define kOpenGLDirtyBit_LightVal_Specular1            ((U64)1 << 16)
#define kOpenGLDirtyBit_LightVal_MatShininess        ((U64)1 << 17)
#define kOpenGLDirtyBit_LightVal_ProdDiffuse        ((U64)1 << 18)
#define kOpenGLDirtyBit_LightVal_ProdAmbient        ((U64)1 << 19)
#define kOpenGLDirtyBit_LightVal_ProdSpecular        ((U64)1 << 20)

#define kOpenGLDirtyBit_ClipPlane_0                    ((U64)1 << 21)
#define kOpenGLDirtyBit_ClipPlane_1                    ((U64)1 << 22)
#define kOpenGLDirtyBit_ClipPlane_2                    ((U64)1 << 23)
#define kOpenGLDirtyBit_ClipPlane_3                    ((U64)1 << 24)
#define kOpenGLDirtyBit_ClipPlane_4                    ((U64)1 << 25)
#define kOpenGLDirtyBit_ClipPlane_5                    ((U64)1 << 26)

#define kOpenGLDirtyBit_ALL                            _UI64_MAX

//
// Holds the data for one shader param in our descriptive table
//
typedef struct tShaderParamSpec
{
    ShaderParamId            id;                        // mainly here to validate that the table is in enum order
    tShaderProgramType      nProgramType;
    const char*             szStdParamName;
    int                        elementFloatRows;        // IOW, for a vec4, this would be 1. for a float3x4 this would be 3.
    int                     elementFloatCols;        // IOW, for a vec4, this would be 4.
    int                        numElements;            // >1 if it's an array type.
    int                        dataCacheFloatOfs;        // if >= 0, an offset (in floats) into an arrays of values and flags used to store the current value.
    tDataSrcType            dataSrcType;
    int                        dataKey1;                // value used to get/set this param, given its dataSrcType.
    int                        dataKey2;                // value used to get/set this param, given its dataSrcType.
    int                        paramNmCacheKey;        // an index into our cached param handle list
    U64                        openGlDirtyBit;
} tShaderParamSpec;

#define ShaderParamSpec_FloatsPerElement( _tShaderParamSpec_p_ )    ((_tShaderParamSpec_p_)->elementFloatRows * (_tShaderParamSpec_p_)->elementFloatCols )

//
//    Holds info for a shader param in use by a loaded shader
//
typedef struct tShaderParamHandleCacheEntry
{
    const char*             szParamName;
    tShaderProgramType        pgmType;
    CGprogram                programHdl;            // last program which used this param
    CGparameter*            paramHdlList;        // if ( programHdl==the_current_program ) this is the param hdl list
    GLuint                    nListSz;
} tShaderParamHandleCacheEntry;


static U64 sOpenGLShaderParamDirtyFlags = 0;
static U64 sOpenGLShaderParamDirtyBitsApplied = 0;
static bool sNothingToPush = true;
static U64 sCurrentShaderOpenGLFlags = 0;

//--------------------------------------------------------------------------
// MAINTENANCE NOTE: This table must be kept in ShaderParamId order.
// Will fail an assert() at runtime otherwise.
//--------------------------------------------------------------------------
static tShaderParamSpec sShaderParamSpecTbl[] = {
//                                                                                                    
//                                                                                                                            dataCacheVec4Ofs
//                                                                                                                        numElements                                                          openGlDirtyBit
//                                                                                                                    elementFloatCols                                                    paramNmCacheKey
//        id                                            nProgramType                szStdParamName                   elementFloatRows     dataSrcType                dataKey1                dataKey2
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //-----------------------------------------
    //    Matrix params
    //-----------------------------------------
    
    { kMatrixParam_ModelView,                        kShaderPgmType_VERTEX,        "g_ModelViewMatrix",            4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_ModelView,        -1,    -1,    kOpenGLDirtyBit_Matrix_ModelView },
    { kMatrixParam_ModelViewIT,                        kShaderPgmType_VERTEX,        "g_ModelViewMatrixInvTrans",    4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_ModelViewIT,    -1,    -1,    kOpenGLDirtyBit_Matrix_ModelViewIT },
    { kMatrixParam_Projection,                        kShaderPgmType_VERTEX,        "g_ProjectionMatrix",            4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Projection,        -1,    -1,    kOpenGLDirtyBit_Matrix_Projection },
    { kMatrixParam_ModelViewProj,                    kShaderPgmType_VERTEX,        "g_ModelViewProjection",        4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_ModelViewProj,    -1,    -1,    kOpenGLDirtyBit_Matrix_ModelViewProjection },
    { kMatrixParam_Texture0,                        kShaderPgmType_VERTEX,        "g_TextureMatrix0",                4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        0,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture1,                        kShaderPgmType_VERTEX,        "g_TextureMatrix1",                4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        1,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture2,                        kShaderPgmType_VERTEX,        "g_TextureMatrix2",                4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        2,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture3,                        kShaderPgmType_VERTEX,        "g_TextureMatrix3",                4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        3,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture4,                        kShaderPgmType_VERTEX,        "g_TextureMatrix4",                4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        4,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture5,                        kShaderPgmType_VERTEX,        "g_TextureMatrix5",                4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        5,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture6,                        kShaderPgmType_VERTEX,        "g_TextureMatrix6",                4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        6,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture7,                        kShaderPgmType_VERTEX,        "g_TextureMatrix7",                4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        7,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture8,                        kShaderPgmType_VERTEX,        "g_TextureMatrix8",                4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        8,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture9,                        kShaderPgmType_VERTEX,        "g_TextureMatrix9",                4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        9,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture10,                        kShaderPgmType_VERTEX,        "g_TextureMatrix10",            4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        10,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture11,                        kShaderPgmType_VERTEX,        "g_TextureMatrix11",            4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        11,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture12,                        kShaderPgmType_VERTEX,        "g_TextureMatrix12",            4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        12,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture13,                        kShaderPgmType_VERTEX,        "g_TextureMatrix13",            4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        13,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture14,                        kShaderPgmType_VERTEX,        "g_TextureMatrix14",            4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        14,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Texture15,                        kShaderPgmType_VERTEX,        "g_TextureMatrix15",            4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Texture,        15,    -1,    kOpenGLDirtyBit_Matrix_Texture },
    { kMatrixParam_Color,                            kShaderPgmType_VERTEX,        "g_ColorMatrix",                4,    4,    1,    -1,    kDataSrc_OpenGLMatrix,        kMatrix_Color,            -1,    -1,    kOpenGLDirtyBit_Matrix_Color },

    //-----------------------------------------
    //    OpenGL States (VP)
    //-----------------------------------------

    { kOpenGLParam_LightProd0DiffuseVP,                kShaderPgmType_VERTEX,        "g_LightProd0DiffuseVP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdDiffuse,    0,    -1,    kOpenGLDirtyBit_LightVal_ProdDiffuse },
    { kOpenGLParam_LightProd1DiffuseVP,                kShaderPgmType_VERTEX,        "g_LightProd1DiffuseVP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdDiffuse,    1,    -1,    kOpenGLDirtyBit_LightVal_ProdDiffuse },
    { kOpenGLParam_LightProd0AmbientVP,                kShaderPgmType_VERTEX,        "g_LightProd0AmbientVP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdAmbient,    0,    -1,    kOpenGLDirtyBit_LightVal_ProdAmbient },
    { kOpenGLParam_LightProd1AmbientVP,                kShaderPgmType_VERTEX,        "g_LightProd1AmbientVP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdAmbient,    1,    -1,    kOpenGLDirtyBit_LightVal_ProdAmbient },
    { kOpenGLParam_LightProd0SpecularVP,            kShaderPgmType_VERTEX,        "g_LightProd0SpecularVP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdSpecular,    0,    -1,    kOpenGLDirtyBit_LightVal_ProdSpecular },
    { kOpenGLParam_LightProd1SpecularVP,            kShaderPgmType_VERTEX,        "g_LightProd1SpecularVP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdSpecular,    1,    -1,    kOpenGLDirtyBit_LightVal_ProdSpecular },

    { kOpenGLParam_Light0PositionVP,                kShaderPgmType_VERTEX,        "g_Light0PositionVP",            1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_Position,        0,    -1,    kOpenGLDirtyBit_LightVal_Position },
    { kOpenGLParam_Light1PositionVP,                kShaderPgmType_VERTEX,        "g_Light1PositionVP",            1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_Position,        1,    -1,    kOpenGLDirtyBit_LightVal_Position },

    { kOpenGLParam_Light0HalfVP,                    kShaderPgmType_VERTEX,        "g_Light0HalfVP",                1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_Half,            0,    -1,    kOpenGLDirtyBit_LightVal_Half0 },
    { kOpenGLParam_Light1HalfVP,                    kShaderPgmType_VERTEX,        "g_Light1HalfVP",                1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_Half,            1,    -1,    kOpenGLDirtyBit_LightVal_Half1 },

    { kOpenGLParam_Light0DiffuseVP,                    kShaderPgmType_VERTEX,        "g_Light0DiffuseVP",            1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_Diffuse,        0,    -1,    kOpenGLDirtyBit_LightVal_Diffuse0 },
    { kOpenGLParam_Light1DiffuseVP,                    kShaderPgmType_VERTEX,        "g_Light1DiffuseVP",            1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_Diffuse,        1,    -1,    kOpenGLDirtyBit_LightVal_Diffuse1 },

    { kOpenGLParam_Light0AmbientVP,                    kShaderPgmType_VERTEX,        "g_Light0AmbientVP",            1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_Ambient,        0,    -1,    kOpenGLDirtyBit_LightVal_Ambient0 },
    { kOpenGLParam_Light1AmbientVP,                    kShaderPgmType_VERTEX,        "g_Light1AmbientVP",            1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_Ambient,        1,    -1,    kOpenGLDirtyBit_LightVal_Ambient1 },

    { kOpenGLParam_Light0SpecularVP,                kShaderPgmType_VERTEX,        "g_Light0SpecularVP",            1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_Specular,        0,    -1,    kOpenGLDirtyBit_LightVal_Specular0 },
    { kOpenGLParam_Light1SpecularVP,                kShaderPgmType_VERTEX,        "g_Light1SpecularVP",            1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_Specular,        1,    -1,    kOpenGLDirtyBit_LightVal_Specular1 },

    { kOpenGLParam_MaterialShininessVP,                kShaderPgmType_VERTEX,        "g_MaterialShininessVP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_MatShininess, 0,    -1,    kOpenGLDirtyBit_LightVal_MatShininess },

    { kOpenGLParam_ClipPlane0VP,                    kShaderPgmType_VERTEX,        "g_clipPlane0VP",                1,    4,    1,    -1,    kDataSrc_Clipping,            kClipInfo_Plane,        0,    -1,    kOpenGLDirtyBit_ClipPlane_0 },
    { kOpenGLParam_ClipPlane1VP,                    kShaderPgmType_VERTEX,        "g_clipPlane1VP",                1,    4,    1,    -1,    kDataSrc_Clipping,            kClipInfo_Plane,        1,    -1,    kOpenGLDirtyBit_ClipPlane_1 },
    { kOpenGLParam_ClipPlane2VP,                    kShaderPgmType_VERTEX,        "g_clipPlane2VP",                1,    4,    1,    -1,    kDataSrc_Clipping,            kClipInfo_Plane,        2,    -1,    kOpenGLDirtyBit_ClipPlane_2 },
    { kOpenGLParam_ClipPlane3VP,                    kShaderPgmType_VERTEX,        "g_clipPlane3VP",                1,    4,    1,    -1,    kDataSrc_Clipping,            kClipInfo_Plane,        3,    -1,    kOpenGLDirtyBit_ClipPlane_3 },
    { kOpenGLParam_ClipPlane4VP,                    kShaderPgmType_VERTEX,        "g_clipPlane4VP",                1,    4,    1,    -1,    kDataSrc_Clipping,            kClipInfo_Plane,        4,    -1,    kOpenGLDirtyBit_ClipPlane_4 },
    { kOpenGLParam_ClipPlane5VP,                    kShaderPgmType_VERTEX,        "g_clipPlane5VP",                1,    4,    1,    -1,    kDataSrc_Clipping,            kClipInfo_Plane,        5,    -1,    kOpenGLDirtyBit_ClipPlane_5 },

    //-----------------------------------------
    //    Vertex program params
    //-----------------------------------------

    { kShaderParam_LightDirVP,                        kShaderPgmType_VERTEX,        "g_LightDirVP",                    1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    0,    -1,    -1,    0 },
    { kShaderParam_ViewerPositionVP,                kShaderPgmType_VERTEX,        "g_ViewerPositionVP",            1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    2,    -1,    -1,    0 },
    { kShaderParam_AmbientParameterVP,                kShaderPgmType_VERTEX,        "g_AmbientParameterVP",            1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    4,    -1,    -1,    0 },
    { kShaderParam_DiffuseParameterVP,                kShaderPgmType_VERTEX,        "g_DiffuseParameterVP",            1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    5,    -1,    -1,    0 },
    { kShaderParam_TexScroll0VP,                    kShaderPgmType_VERTEX,        "g_TexScroll0VP",                1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    14,    -1,    -1,    0 },
    { kShaderParam_TexScroll1VP,                    kShaderPgmType_VERTEX,        "g_TexScroll1VP",                1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    15,    -1,    -1,    0 },

    { kShaderParam_ReflectionParamVP,                kShaderPgmType_VERTEX,        "g_ReflectionParamVP",            1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    1,    -1,    -1,    0 },
    { kShaderParam_InverseCubemapViewMatrixVP,        kShaderPgmType_VERTEX,        "g_InverseCubemapViewMatrixVP",    1,    3,    3,    -1,    kDataSrc_EnvVariable,        6,    -1,    -1,    0 },

    { kShaderParam_BoneMatrixArrVP,                    kShaderPgmType_VERTEX,        "g_BoneMatrixArrVP",            1,    4,    48,    -1,    kDataSrc_PgmLocalVariable,    16,    -1,    -1,    0 },

    { kShaderParam_PlanarReflectionPlaneVP,            kShaderPgmType_VERTEX,        "g_PlanarReflectionPlaneVP",    1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    9,    -1,    -1,    0 },
    
    // .... End of vertex shader params. New entries can be added anywhere; just make sure 
    //        the table order matches the enum order.

    //-----------------------------------------
    //    OpenGL States (FP)
    //-----------------------------------------

    { kOpenGLParam_FogParamsFP,                        kShaderPgmType_FRAGMENT,    "g_FogParamsFP",                1,    4,    1,    -1,    kDataSrc_FogState,            kFogState_Params,        -1,    -1,    kOpenGLDirtyBit_FogState_Params },
    { kOpenGLParam_FogColorFP,                        kShaderPgmType_FRAGMENT,    "g_FogColorFP",                    1,    4,    1,    -1,    kDataSrc_FogState,            kFogState_Color,        -1,    -1,    kOpenGLDirtyBit_FogState_Color },

    { kOpenGLParam_LightProd0DiffuseFP,                kShaderPgmType_FRAGMENT,    "g_LightProd0DiffuseFP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdDiffuse,    0,    -1,    kOpenGLDirtyBit_LightVal_ProdDiffuse },
    { kOpenGLParam_LightProd1DiffuseFP,                kShaderPgmType_FRAGMENT,    "g_LightProd1DiffuseFP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdDiffuse,    1,    -1,    kOpenGLDirtyBit_LightVal_ProdDiffuse },
    { kOpenGLParam_LightProd0AmbientFP,                kShaderPgmType_FRAGMENT,    "g_LightProd0AmbientFP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdAmbient,    0,    -1,    kOpenGLDirtyBit_LightVal_ProdAmbient },
    { kOpenGLParam_LightProd1AmbientFP,                kShaderPgmType_FRAGMENT,    "g_LightProd1AmbientFP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdAmbient,    1,    -1,    kOpenGLDirtyBit_LightVal_ProdAmbient },
    { kOpenGLParam_LightProd0SpecularFP,            kShaderPgmType_FRAGMENT,    "g_LightProd0SpecularFP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdSpecular,    0,    -1,    kOpenGLDirtyBit_LightVal_ProdSpecular },
    { kOpenGLParam_LightProd1SpecularFP,            kShaderPgmType_FRAGMENT,    "g_LightProd1SpecularFP",        1,    4,    1,    -1,    kDataSrc_LightingModel,        kLightVal_ProdSpecular,    1,    -1,    kOpenGLDirtyBit_LightVal_ProdSpecular },

    { kOpenGLParam_TexSampler0,                        kShaderPgmType_FRAGMENT,    "g_TexSampler0",                0,    0,    0,    -1,    kDataSrc_TexSampler,        0, -1,    -1, 0 },
    { kOpenGLParam_TexSampler1,                        kShaderPgmType_FRAGMENT,    "g_TexSampler1",                0,    0,    0,    -1,    kDataSrc_TexSampler,        1, -1,    -1, 0 },
    { kOpenGLParam_TexSampler2,                        kShaderPgmType_FRAGMENT,    "g_TexSampler2",                0,    0,    0,    -1,    kDataSrc_TexSampler,        2, -1,    -1, 0 },
    { kOpenGLParam_TexSampler3,                        kShaderPgmType_FRAGMENT,    "g_TexSampler3",                0,    0,    0,    -1,    kDataSrc_TexSampler,        3, -1,    -1, 0 },
    { kOpenGLParam_TexSampler4,                        kShaderPgmType_FRAGMENT,    "g_TexSampler4",                0,    0,    0,    -1,    kDataSrc_TexSampler,        4, -1,    -1, 0 },
    { kOpenGLParam_TexSampler5,                        kShaderPgmType_FRAGMENT,    "g_TexSampler5",                0,    0,    0,    -1,    kDataSrc_TexSampler,        5, -1,    -1, 0 },
    { kOpenGLParam_TexSampler6,                        kShaderPgmType_FRAGMENT,    "g_TexSampler6",                0,    0,    0,    -1,    kDataSrc_TexSampler,        6, -1,    -1, 0 },
    { kOpenGLParam_TexSampler7,                        kShaderPgmType_FRAGMENT,    "g_TexSampler7",                0,    0,    0,    -1,    kDataSrc_TexSampler,        7, -1,    -1, 0 },
    { kOpenGLParam_TexSampler8,                        kShaderPgmType_FRAGMENT,    "g_TexSampler8",                0,    0,    0,    -1,    kDataSrc_TexSampler,        8, -1,    -1, 0 },
    { kOpenGLParam_TexSampler9,                        kShaderPgmType_FRAGMENT,    "g_TexSampler9",                0,    0,    0,    -1,    kDataSrc_TexSampler,        9, -1,    -1, 0 },
    { kOpenGLParam_TexSampler10,                    kShaderPgmType_FRAGMENT,    "g_TexSampler10",                0,    0,    0,    -1,    kDataSrc_TexSampler,        10, -1,    -1, 0 },
    { kOpenGLParam_TexSampler11,                    kShaderPgmType_FRAGMENT,    "g_TexSampler11",                0,    0,    0,    -1,    kDataSrc_TexSampler,        11, -1,    -1, 0 },
    { kOpenGLParam_TexSampler12,                    kShaderPgmType_FRAGMENT,    "g_TexSampler12",                0,    0,    0,    -1,    kDataSrc_TexSampler,        12, -1,    -1, 0 },
    { kOpenGLParam_TexSampler13,                    kShaderPgmType_FRAGMENT,    "g_TexSampler13",                0,    0,    0,    -1,    kDataSrc_TexSampler,        13, -1,    -1, 0 },
    { kOpenGLParam_TexSampler14,                    kShaderPgmType_FRAGMENT,    "g_TexSampler14",                0,    0,    0,    -1,    kDataSrc_TexSampler,        14, -1,    -1, 0 },
    { kOpenGLParam_TexSampler15,                    kShaderPgmType_FRAGMENT,    "g_TexSampler15",                0,    0,    0,    -1,    kDataSrc_TexSampler,        15, -1,    -1, 0 },

    //-----------------------------------------
    // Fragment program params
    //-----------------------------------------

    { kShaderParam_Effects_TextTransformFP,            kShaderPgmType_FRAGMENT,    "g_Effects_TextTransformFP",    1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    0,    -1,    -1,    0 },
    { kShaderParam_Effects_ExpectedLumFP,            kShaderPgmType_FRAGMENT,    "g_Effects_ExpectedLumFP",        1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    0,    -1,    -1,    0 },
    { kShaderParam_Effects_BlurAmtAndExposureFP,    kShaderPgmType_FRAGMENT,    "g_BlurAmtAndExposureFP",        1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    0,    -1,    -1,    0 },
    { kShaderParam_Effects_TimeStepFP,                kShaderPgmType_FRAGMENT,    "g_TimeStepFP",                    1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    0,    -1,    -1,    0 },
    { kShaderParam_Effects_DofParam2FP,                kShaderPgmType_FRAGMENT,    "g_Effects_DofParam2FP",        1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    1,    -1,    -1,    0 },
    { kShaderParam_Effects_DofProjectFP,            kShaderPgmType_FRAGMENT,    "g_Effects_DofProjectFP",        1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    2,    -1,    -1,    0 },
    { kShaderParam_Effects_DesaturateParamFP,        kShaderPgmType_FRAGMENT,    "g_Effects_DesaturateParamFP",    1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    3,    -1,    -1,    0 },

    { kShaderParam_Effects_GfxPerfTestParam0FP,        kShaderPgmType_FRAGMENT,    "g_Effects_GfxPerfTestParam0FP",1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    0,    -1,    -1,    0 },
    { kShaderParam_Effects_GfxPerfTestParam1FP,        kShaderPgmType_FRAGMENT,    "g_Effects_GfxPerfTestParam1FP",1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    1,    -1,    -1,    0 },
    { kShaderParam_Effects_GfxPerfTestParam2FP,        kShaderPgmType_FRAGMENT,    "g_Effects_GfxPerfTestParam2FP",1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    2,    -1,    -1,    0 },

    { kShaderParam_AmbientColorFP,                    kShaderPgmType_FRAGMENT,    "g_AmbientColorFP",                1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    0,    -1,    -1,    0 },
    { kShaderParam_DiffuseColorFP,                    kShaderPgmType_FRAGMENT,    "g_DiffuseColorFP",                1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    1,    -1,    -1,    0 },
    { kShaderParam_GlossParamFP,                    kShaderPgmType_FRAGMENT,    "g_GlossParamFP",                1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    2,    -1,    -1,    0 },
    { kShaderParam_GlowParamFP,                        kShaderPgmType_FRAGMENT,    "g_GlowParamFP",                1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    3,    -1,    -1,    0 },
    { kShaderParam_ConstColor0FP,                    kShaderPgmType_FRAGMENT,    "g_ConstColor0FP",                1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    3,    -1,    -1,    0 },
    { kShaderParam_ConstColor1FP,                    kShaderPgmType_FRAGMENT,    "g_ConstColor1FP",                1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    4,    -1,    -1,    0 },
    { kShaderParam_Specular1ColorAndExponentFP,        kShaderPgmType_FRAGMENT,    "g_Specular1ColorAndExponentFP",1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    5,    -1,    -1,    0 },
    { kShaderParam_Specular2ColorAndExponentFP,        kShaderPgmType_FRAGMENT,    "g_Specular2ColorAndExponentFP",1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    6,    -1,    -1,    0 },
    { kShaderParam_WaterReflectionTransformFP,        kShaderPgmType_FRAGMENT,    "g_WaterReflectionTransformFP",    1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    6,    -1,    -1,    0 },
    { kShaderParam_WaterRefractionTransformFP,        kShaderPgmType_FRAGMENT,    "g_WaterRefractionTransformFP",    1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    7,    -1,    -1,    0 },
    { kShaderParam_MiscParamFP,                        kShaderPgmType_FRAGMENT,    "g_MiscParamFP",                1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    7,    -1,    -1,    0 },

    // NOTE: These are constColor0 and constColor1 as set by the engine for tinting, dual blend
    // In the old ARB path these are hard coded in the ARB programs to use env[0] and env[1]
    // as they are set outside of binding the fragment program.
    // The ARB path code still sets these directly and the register values in the table here are
    // ignored. The register values in the table only apply to the 'hard constants' Cg path
    // where we directly set ENV variables hard coded in the shaders.`
    { kShaderParam_EnvVar0FP,                        kShaderPgmType_FRAGMENT,    "g_Env0FP",                        1,    4,    1,    -1,    kDataSrc_EnvVariable,        8,    -1,    -1,    0 },
    { kShaderParam_EnvVar1FP,                        kShaderPgmType_FRAGMENT,    "g_Env1FP",                        1,    4,    1,    -1,    kDataSrc_EnvVariable,        9,    -1,    -1,    0 },

    // Revised MULTI9 constants (in lieu of interpolants) (...light dir also for high quality bumpmap color blend dual)
    { kShaderParam_BumpMultiFlagsFP,                kShaderPgmType_FRAGMENT,    "g_BumpMultiFlagsFP",            1,    4,    1,    -1,    kDataSrc_EnvVariable,        10, -1,    -1,    0 },
    { kShaderParam_LightDirFP,                        kShaderPgmType_FRAGMENT,    "g_LightDirFP",                    1,    4,    1,    -1,    kDataSrc_EnvVariable,        11, -1,    -1,    0 },

    // Texture unit scroll/scale constant array must be kept in sync with runtime and TEXLAYER_MAX_SCROLLABLE
    { kShaderParam_ScrollScaleArrFP,                kShaderPgmType_FRAGMENT,    "g_ScrollScaleArrFP",            1,    4,    TEXLAYER_MAX_SCROLLABLE,
                                                                                                                            -1,    kDataSrc_PgmLocalVariable,    12,    -1,    -1,    0 },

    { kShaderParam_WaterRefractionParamsFP,            kShaderPgmType_FRAGMENT,    "g_WaterRefractionParamsFP",    1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    22,    -1,    -1,    0 },
    { kShaderParam_WaterReflectionParamsFP,            kShaderPgmType_FRAGMENT,    "g_WaterReflectionParamsFP",    1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    23,    -1,    -1,    0 }, // @todo ultra const?

    //****
    // BEGIN ULTRA CONSTANTS
    //****

    // reflection and env mapping

    // @todo even though water reflections are ultra feature these are currently always set for water?
    { kShaderParam_WaterFresnelParamsFP,            kShaderPgmType_FRAGMENT,    "g_WaterFresnelParamsFP",        1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    24,    -1,    -1,    0 },
    { kShaderParam_FresnelParamsFP,                    kShaderPgmType_FRAGMENT,    "g_FresnelParamsFP",            1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    24,    -1,    -1,    0 },
    { kShaderParam_DistortionParamsFP,                kShaderPgmType_FRAGMENT,    "g_DistortionParamsFP",            1,    4,    1,    -1,    kDataSrc_PgmLocalVariable,    25,    -1,    -1,    0 },

    { kShaderParam_PlanarReflectionTransformFP,        kShaderPgmType_FRAGMENT,    "g_PlanarReflectionTransformFP",1,    4,    1,    -1,    kDataSrc_EnvVariable,        26, -1,    -1,    0 },
    { kShaderParam_InverseCubemapViewMatrixFP,        kShaderPgmType_FRAGMENT,    "g_InverseCubemapViewMatrixFP",    1,    3,    3,    -1,    kDataSrc_EnvVariable,        27,    -1,    -1,    0 },

    // Shadow mapping
    { kShaderParam_ShadowMap1MatrixFP,                kShaderPgmType_FRAGMENT,    "g_ShadowMap1MatrixFP",            1,    4,    4,    -1,    kDataSrc_EnvVariable,        32,    -1,    -1,    0 },
    { kShaderParam_ShadowMap2MatrixFP,                kShaderPgmType_FRAGMENT,    "g_ShadowMap2MatrixFP",            1,    4,    4,    -1,    kDataSrc_EnvVariable,        36,    -1,    -1,    0 },
    { kShaderParam_ShadowMap3MatrixFP,                kShaderPgmType_FRAGMENT,    "g_ShadowMap3MatrixFP",            1,    4,    4,    -1,    kDataSrc_EnvVariable,        40,    -1,    -1,    0 },
    { kShaderParam_ShadowMap4MatrixFP,                kShaderPgmType_FRAGMENT,    "g_ShadowMap4MatrixFP",            1,    4,    4,    -1,    kDataSrc_EnvVariable,        44,    -1,    -1,    0 },
    { kShaderParam_ShadowParamsFP,                    kShaderPgmType_FRAGMENT,    "g_ShadowParamsFP",                1,    4,    1,    -1,    kDataSrc_EnvVariable,        48,    -1,    -1,    0 },
    { kShaderParam_ShadowSplitsFP,                    kShaderPgmType_FRAGMENT,    "g_ShadowSplitsFP",                1,    4,    1,    -1,    kDataSrc_EnvVariable,        49,    -1,    -1,    0 },
    { kShaderParam_ShadowParams2FP,                    kShaderPgmType_FRAGMENT,    "g_ShadowParams2FP",            1,    4,    1,    -1,    kDataSrc_EnvVariable,        50,    -1,    -1,    0 },
    { kShaderParam_ShadowParams3FP,                    kShaderPgmType_FRAGMENT,    "g_ShadowParams3FP",            1,    4,    1,    -1,    kDataSrc_EnvVariable,        51,    -1,    -1,    0 },

    //****
    // DEBUG/TESTING (not used by retail shaders)
    //****

    { kShaderParam_NewFeatureFP,                    kShaderPgmType_FRAGMENT,    "g_NewFeatureFP",                1,    4,    1,    -1,    kDataSrc_EnvVariable,        60,    -1,    -1,    0 },
    { kShaderParam_TestFlagsFP,                        kShaderPgmType_FRAGMENT,    "g_TestFlagsFP",                1,    4,    1,    -1,    kDataSrc_EnvVariable,        61,    -1,    -1,    0 },
    
    { kShaderParam_DebugEnableFlagSetsFP,            kShaderPgmType_FRAGMENT,    "g_DebugEnableFlagSetsFP",        1,    4,    1,    -1,    kDataSrc_EnvVariable,        65,    -1,    -1,    0 },
    { kShaderParam_DebugDisabledColorFP,            kShaderPgmType_FRAGMENT,    "g_DebugDisabledColorFP",        1,    4,    1,    -1,    kDataSrc_EnvVariable,        66,    -1,    -1,    0 },

    // .... End of fragment shader params. New entries can be added anywhere; just make sure 
    //        the table order matches the enum order.

};

//
// Shader param handle cache
//
struct {
    GLuint    nArraySize;
    GLuint    nNumUsed;
    tShaderParamHandleCacheEntry* cacheData;
} sShaderParamHandleCache = { 0 };

static void GrowParamHandleCache( GLuint nNewElements );
static int AddParamHandleCacheKey( const char* szParamName, tShaderProgramType pgmType );
static void setFragmentProgramConstColor( GLuint index, const GLfloat* vec4 );
//
//    Data cache arrays
//
//    We store the values used by the params as one long array of each type (float, flags, etc.)
//    and then each parameter remembers the offset of its own data. This makes it quick and easy
//    to clear, for example, the "isSet[]" bools but not the "data[]" floats with one memset().
//    Quicker than walking an array of structs and hand-poking values.
//
//    Semantics:
//        isSet[]        The cached value has been set; it's real.
//        dirty[]        The cached value needs to be sent to the shader. The value was changed
//                    since we last updated the shader, or the shader we last fed this value
//                    to is gone.
//
//        Distinctions:
//            -    Once a value has been sent to the shader, it's no longer "dirty", but it is
//                still "set". If we do more drawing with the same shader we don't need to
//                send a parameter update. 
//            -    If another shader is bound and it uses the same value ("g_AmbientColor", lets' say)
//                we can send that "set" value to the new shader even though it isn't "dirty".
//
typedef struct tParamDataCache
{
        // data array set up for all params for this shader class (FP or VP)
    GLfloat*    data;
        // only the bools at the offsets representing the start of an element
        // (e.g., every 4th, if the basic element is a vec4) are valid.
    bool*        isSet;        // an array of arraySz items. See notes, above.
    bool*        dirty;        // an array of arraySz items. See notes, above.
    GLuint        arraySz;    // size of array, in FLOATS.
    GLuint        numItems;    // number of FLOATS stored.
} tParamDataCache;

tParamDataCache sLocalDataCache[kDataSrc_COUNT][kShaderPgmType_COUNT] = {
    { NULL, NULL, 0, 0 },
    { NULL, NULL, 0, 0 }
};


// Mainly to help clarify the code
#define kBytesInFloat    ( sizeof(float) )
#define kFloatsInVec4    4
#define kFloatsInVec3    3

#define cast_floatptr_to_mat44(_gl_float_)    ((Vec4*)(_gl_float_))


//state vars (put this in an structure?)
static GLenum sfunc  = GL_ALWAYS; 
static GLint  sref   = 0;
static GLuint smask  = 0xffffffff;
static GLenum sfail  = GL_KEEP;
static GLenum szfail = GL_KEEP;
static GLenum szpass = GL_KEEP;
static int    senabled = 0; 
static int    numclipplanesenabled = 0; 
static GLdouble clipplanes[6][4];

static float slight[2][4][4] =    
                        {
                            {   
                                {0.0, 0.0, 0.0, 1.0},       //GL_AMBIENT
                                {1.0, 1.0, 1.0, 1.0},        //GL_DIFFUSE
                                {1.0, 1.0, 1.0, 1.0},        //GL_SPECULAR
                                {0.0, 0.0, 1.0, 0.0}        //GL_POSITION
                            },    
                            {   
                                {0.0, 0.0, 0.0, 1.0},       //GL_AMBIENT
                                {1.0, 1.0, 1.0, 1.0},        //GL_DIFFUSE
                                {1.0, 1.0, 1.0, 1.0},        //GL_SPECULAR
                                {0.0, 0.0, 1.0, 0.0}        //GL_POSITION
                            }
                        };

static float slight_ogl[2][4][4] =
                        {
                            {   
                                {0.0, 0.0, 0.0, 1.0},       //GL_AMBIENT
                                {1.0, 1.0, 1.0, 1.0},        //GL_DIFFUSE
                                {1.0, 1.0, 1.0, 1.0},        //GL_SPECULAR
                                {0.0, 0.0, 1.0, 0.0}        //GL_POSITION
                            },    
                            {   
                                {0.0, 0.0, 0.0, 1.0},       //GL_AMBIENT
                                {1.0, 1.0, 1.0, 1.0},        //GL_DIFFUSE
                                {1.0, 1.0, 1.0, 1.0},        //GL_SPECULAR
                                {0.0, 0.0, 1.0, 0.0}        //GL_POSITION
                            }
                        };

static U8 scolor_red = 0;
static U8 scolor_green = 0;
static U8 scolor_blue = 0;
static U8 scolor_alpha = 0;

static float smaterial[4][4] = {   
                                {0.2, 0.2, 0.2, 1.0},       //GL_AMBIENT
                                {0.8, 0.8, 0.8, 1.0},        //GL_DIFFUSE
                                {0.0, 0.0, 0.0, 1.0},        //GL_SPECULAR
                                {0.0, 0.0, 0.0, 0.0}        //GL_SHININESS
                            };    

static GLfloat* s_modelview_matrix        = NULL;
static GLfloat* s_projection_matrix        = NULL;
static GLfloat* s_texture_matrix        = NULL;
static GLfloat* s_color_matrix            = NULL;

static GLboolean sdepthmask = GL_TRUE;

float combiconst[2][4];

static float texLODbias[MAX_TEXTURE_UNITS_TOTAL] = { 0 };

static int vertexProgramEnabled = 0;
static GLuint boundVertexProgram = 0xFFFFFFFF;
static int fragmentProgramEnabled = 0;
static GLuint boundFragmentProgram = 0xFFFFFFFF;
static int glNormalizeEnabled = 0;
static int glNormalizeOverride=-1;
static int glLightingEnabled = 0;
static int glLightingOverride=-1;
static int glLightingNonOverride=-1;
static int sun_lightdir_valid=0;
static int last_element_array_id,last_vertex_array_id;

static FogContext fake_fog_context_for_client_binning;
static FogContext *fog_context = &fake_fog_context_for_client_binning;

static struct
{
    GLenum sfactor, dfactor;
} glBlendFuncStack[32];
static int glBlendFuncStackIdx = -1;
static int glBlendFuncStackFrozen = 0;

typedef struct ClientState
{
    GLenum arrayid;
    int on;
} ClientState;

static ClientState clientstate[] =
{            
    { GL_VERTEX_ATTRIB_ARRAY1_NV,    0 }, //0
    { GL_VERTEX_ATTRIB_ARRAY5_NV,    0 }, //1
    { GL_VERTEX_ATTRIB_ARRAY6_NV,    0 }, //2
    { GL_VERTEX_ATTRIB_ARRAY7_NV,    0 }, //3
    { GL_TEXTURE_COORD_ARRAY,        0 }, //4
    { GL_TEXTURE_COORD_ARRAY,        0 }, //5
    { GL_TEXTURE_COORD_ARRAY,        0 }, //6
    { GL_TEXTURE_COORD_ARRAY,        0 }, //7
    { GL_TEXTURE_COORD_ARRAY,        0 }, //8
    { GL_TEXTURE_COORD_ARRAY,        0 }, //9
    { GL_TEXTURE_COORD_ARRAY,        0 }, //10
    { GL_TEXTURE_COORD_ARRAY,        0 }, //11
    { GL_VERTEX_ARRAY,                0 }, //12
    { GL_NORMAL_ARRAY,                0 }, //13
    { GL_COLOR_ARRAY,                0 }, //14
    { GL_VERTEX_ATTRIB_ARRAY11_NV,    0 }, //15
};

static const GLvoid *tex_pointer[MAX_TEXTURE_UNITS_TOTAL] = { (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, (void *)0xffffffff, };
static const GLvoid *norm_pointer = (void *)0xffffffff;
static const GLvoid *vert_pointer = (void *)0xffffffff;

#if SHADER_DBG_CODE_ENABLED
    //
    // Global file pointer for debug logging.
    //
    FILE* gSHADER_DBG_LOG_FILE = NULL;
#endif

static void resetLocalParameters(tShaderProgramType target);
static void setNamedParamValue( tShaderParamSpec* pSpec, const GLfloat *vec4Arr, GLuint nRows, GLuint nCols );
static void setArbRegOBSOLETE( tShaderParamSpec* pSpec, const GLfloat *vec4Arr, GLuint nNumVec4s );
static void setArbEnvReg( tShaderParamSpec* pSpec, const GLfloat *vec4Arr, GLuint nNumVec4s );
static void setArbEnvReg3( tShaderParamSpec* pSpec, const GLfloat *vec3Arr, GLuint nNumVec3s );

#define /* tParamDataCache* */ GetCacheDataForParamSpec( _tShaderParamSpec_pSpec_ )    \
    ( &sLocalDataCache[(_tShaderParamSpec_pSpec_)->dataSrcType][(_tShaderParamSpec_pSpec_)->nProgramType] )

#define /* tParamDataCache* */ GetCacheDataForCacheId( _int_nParamSpecTblIndex_ )    \
    ( GetCacheDataForParamSpec( &sShaderParamSpecTbl[_int_nParamSpecTblIndex_] ) )

#define /* GLfloat* */ GetCachedValueForParamSpec( _tShaderParamSpec_pSpec_ )        \
    ( GetCacheDataForParamSpec( _tShaderParamSpec_pSpec_ )->data + (_tShaderParamSpec_pSpec_)->dataCacheFloatOfs )

#define /* GLfloat* */ GetCachedValueForCacheId( _int_nParamSpecTblIndex_ )            \
    ( GetCachedValueForParamSpec( &sShaderParamSpecTbl[_int_nParamSpecTblIndex_] ) )


static void PrepareSamplerForCurrentShader( tShaderParamSpec* pSpec, CGparameter param );

static INLINEDBG void doDisableClientState( GLClientStateType array );

void WCW_ResetState( void )
{
    int i, j;
    rdrBeginMarker(__FUNCTION__);

    INEXACT_CHECKGL;
    WCW_ResetTextureState();
    sfunc  = -1;
    sref   = -1;
    smask  = -1;
    sfail  = -1;
    szfail = -1;
    szpass = -1;
    senabled = -1;  

    for (i=0; i<4; i++) 
    {
        for (j=0; j<4; j++) 
        {
            slight[0][i][j] = 4;
            slight[1][i][j] = 4;
        }
    }

    scolor_red = 0;
    scolor_green = 0;
    scolor_blue = 0;
    scolor_alpha = 0;

    sdepthmask = -1;
    for (i=0; i<MAX_TEXTURE_UNITS_TOTAL; i++) 
    {
        texLODbias[i] = -9999;
    }

    curr_draw_state = curr_blend_state.intval = -1;
    vertexProgramEnabled = -1;
    boundVertexProgram = 0xFFFFFFFF;
    fragmentProgramEnabled = -1;
    boundFragmentProgram = 0xFFFFFFFF;
    glNormalizeEnabled = -1;
    
    // Make sure vertex and fragment programs are disabled
    // instead of leaving them in an unknown state.
    // This can cause blit to fail.
    WCW_DisableVertexProgram();
    WCW_DisableFragmentProgram();

    if (fog_context) 
    {
        fog_context->on = -1;
        fog_context->fog_doneonce = 0;
        fog_context->old_fog[1]=-1;
        fog_context->old_fog[2]=-1;
    }

    glLightingEnabled = -1;

    for (i=0; i<ARRAY_SIZE(clientstate); i++)
    {    
        if(clientstate[i].arrayid >= GL_VERTEX_ATTRIB_ARRAY0_NV && clientstate[i].arrayid <= GL_VERTEX_ATTRIB_ARRAY15_NV) {
            if(!(rdr_caps.chip & NV1X)) {
                continue;
            }
        }

        // Have to check if texture unit is valid before disabling.
        // @todo make sure ineligible units aren't being activated in first place
        if ( i >= GLC_TEXTURE_COORD_ARRAY_0 && i <= GLC_TEXTURE_COORD_ARRAY_7 )
        {
            int unit_index = i - GLC_TEXTURE_COORD_ARRAY_0;
            if (unit_index < rdr_caps.num_texunits)
            {
                doDisableClientState(i);
            }
        }
        else
            doDisableClientState(i);
        clientstate[i].on = 0;
    }

    vert_pointer = (void *)0xffffffff;
    norm_pointer = (void *)0xffffffff;
    memset((void *)tex_pointer, 0xff, sizeof(tex_pointer));

    for (i=0; i<2; i++) 
        for (j=0; j<4; j++) 
            combiconst[i][j] = -2;
    curr_blend_state.intval = -1;
    curr_draw_state = -1;
    last_element_array_id = -1;
    last_vertex_array_id = -1;

    sun_lightdir_valid = 0;

    SHADER_DBG_PRINTF( "Calling resetLocalParameters() from WCW_ResetState()\n" );
    resetLocalParameters(kShaderPgmType_FRAGMENT);
    resetLocalParameters(kShaderPgmType_VERTEX);

    WCW_CheckGLState();
    rdrEndMarker();
}

typedef struct WCW_ShadowState
{
    int valid;

    // Sampler state
    GLenum texunit_bound;
    GLenum tex_target[MAX_TEXTURE_UNITS_TOTAL];
    GLuint tex_bound[MAX_TEXTURE_UNITS_TOTAL];
    GLenum tex_enabled[MAX_TEXTURE_UNITS_TOTAL];

    // Attribute state
    GLenum clienttexunit_bound;

    // ROP state
    GLboolean blend_enabled;
    GLenum blend_src;
    GLenum blend_dst;
} WCW_ShadowState;

static WCW_ShadowState state;

static INLINEDBG GLenum bindForTarget(GLenum target)
{
    switch(target) {
        case GL_TEXTURE_1D: return GL_TEXTURE_BINDING_1D;
        case GL_TEXTURE_2D: return GL_TEXTURE_BINDING_2D;
        case GL_TEXTURE_3D: return GL_TEXTURE_BINDING_3D;
        case GL_TEXTURE_CUBE_MAP: return GL_TEXTURE_BINDING_CUBE_MAP;
        case GL_TEXTURE_RECTANGLE_ARB:  return GL_TEXTURE_BINDING_RECTANGLE_ARB;
    }
    return 0;
}

void WCW_FetchGLState(void)
{
    int i;

    rdrBeginMarker(__FUNCTION__);
    state.valid = 0;

    // Sampler state
    if(glActiveTextureARB) {
    glGetIntegerv(GL_ACTIVE_TEXTURE, &state.texunit_bound); CHECKGL;
    } else {
        state.texunit_bound = GL_TEXTURE0;
    }

    for(i = 0; i < rdr_caps.num_texunits; i++) {
        glActiveTextureARB(GL_TEXTURE0 + i); CHECKGL;

        // Figure out which mode is enabled based on the rules in the glEnable man page
        state.tex_enabled[i] = 0;
        if(glIsEnabled(GL_TEXTURE_CUBE_MAP)) {
            state.tex_enabled[i] = GL_TEXTURE_CUBE_MAP;
        } else if(0 && glIsEnabled(GL_TEXTURE_3D)) {
            state.tex_enabled[i] = GL_TEXTURE_3D;
        } else if(rdr_caps.supports_arb_rect && glIsEnabled(GL_TEXTURE_RECTANGLE_ARB)) {
            state.tex_enabled[i] = GL_TEXTURE_RECTANGLE_ARB;
        } else if(glIsEnabled(GL_TEXTURE_2D)) {
            state.tex_enabled[i] = GL_TEXTURE_2D;
        } else if(glIsEnabled(GL_TEXTURE_1D)) {
            state.tex_enabled[i] = GL_TEXTURE_1D;
        }

        // If we know which one is enabled, use that for the cache, otherwise default to 2D
        if(!state.tex_target[i]) {
            state.tex_target[i] = state.tex_enabled[i] ? state.tex_enabled[i] : GL_TEXTURE_2D;
        }
        glGetIntegerv(bindForTarget(state.tex_target[i]), &state.tex_bound[i]); CHECKGL;
    }

    // Attribute state
    if(glClientActiveTextureARB) {
    glGetIntegerv(GL_CLIENT_ACTIVE_TEXTURE, &state.clienttexunit_bound); CHECKGL;
    } else {
        state.clienttexunit_bound = GL_TEXTURE0;
    }

    // ROP state
    state.blend_enabled = glIsEnabled(GL_BLEND); CHECKGL;
    glGetIntegerv(GL_BLEND_SRC, &state.blend_src);
    glGetIntegerv(GL_BLEND_DST, &state.blend_dst);

    // Restore modified values
    if(glActiveTextureARB) {
        glActiveTextureARB(state.texunit_bound); CHECKGL;
    }
    if(glClientActiveTextureARB) {
        glClientActiveTextureARB(state.clienttexunit_bound); CHECKGL;
    }

    state.valid = 1;    
    rdrEndMarker();
}

void WCW_CheckGLState()
{
#if GLSTATEDEBUG > 1
    if (!GFX_DEBUG_TEST(game_state.gfxdebug, GFXDEBUG_SKIP_CHECKGLSTATE))
    {
        WCW_ShadowState state_copy;
        if(!state.valid) {
            return;
        }
        memcpy(&state_copy, &state, sizeof(WCW_ShadowState));
        WCW_FetchGLState();
        assert(memcmp(&state_copy, &state, sizeof(WCW_ShadowState)) == 0);
    }
#endif
}

void WCW_ResetTextureState( void )
{
    int i;
    rdrBeginMarker(__FUNCTION__);
    // Reset all of the texture state
    for (i=0; i<rdr_caps.num_texunits; i++) {
        WCW_BindTexture(GL_TEXTURE_1D, i, 0);
        WCW_BindTexture(GL_TEXTURE_2D, i, 0);
        if(rdr_caps.supports_arb_rect) WCW_BindTexture(GL_TEXTURE_RECTANGLE_ARB, i, 0);
        if(0) WCW_BindTexture(GL_TEXTURE_3D, i, 0);
        WCW_BindTexture(GL_TEXTURE_CUBE_MAP, i, 0);
    }
    rdrEndMarker();
}

// A stripped down version of WCW_ResetState()
void WCW_ResetState_CgFx( void )
{
    WCW_FetchGLState();
    WCW_ResetTextureState();
    curr_draw_state = curr_blend_state.intval = -1;
    WCW_ResetVertexProgram();
    WCW_ResetFragmentProgram();
}

static void buildLocalDataCache()
{
    {
        //
        // Clear out old arrays (probably won't every be needed, since
        // this is only called at program initialization time.
        //
        int dataSrcTypeI;
        for ( dataSrcTypeI=0; dataSrcTypeI < kDataSrc_COUNT; dataSrcTypeI++ )
        {    
            size_t pgmTypeI;
            for ( pgmTypeI=0; pgmTypeI < ARRAY_SIZE(sLocalDataCache[dataSrcTypeI]); pgmTypeI++ )
            {
                if ( sLocalDataCache[dataSrcTypeI][pgmTypeI].data != NULL )  free ( sLocalDataCache[dataSrcTypeI][pgmTypeI].data );
                if ( sLocalDataCache[dataSrcTypeI][pgmTypeI].isSet != NULL ) free ( sLocalDataCache[dataSrcTypeI][pgmTypeI].isSet );
                if ( sLocalDataCache[dataSrcTypeI][pgmTypeI].dirty != NULL ) free ( sLocalDataCache[dataSrcTypeI][pgmTypeI].dirty );
                sLocalDataCache[dataSrcTypeI][pgmTypeI].arraySz = 0;
                sLocalDataCache[dataSrcTypeI][pgmTypeI].numItems = 0;
            }
        }
    }
    
    {
        //
        //    Compute the arraySz value for each cache type.
        //        
        size_t tableI;
        for ( tableI=0; tableI < ARRAY_SIZE(sShaderParamSpecTbl); tableI++ )
        {
            tParamDataCache* pCache;
            tDataSrcType dataSrcType;
            tShaderProgramType pgmType;
            ValidateShaderProgramTypeConstant( sShaderParamSpecTbl[tableI].nProgramType );
            dataSrcType    = sShaderParamSpecTbl[tableI].dataSrcType;
            pgmType        = sShaderParamSpecTbl[tableI].nProgramType;
            pCache        = &sLocalDataCache[dataSrcType][pgmType];
            sShaderParamSpecTbl[tableI].dataCacheFloatOfs = pCache->arraySz;
            pCache->arraySz += 
                    ( ShaderParamSpec_FloatsPerElement( &sShaderParamSpecTbl[tableI] ) *
                        sShaderParamSpecTbl[tableI].numElements );
        }
    }
        
    {
        //
        // Now build the arrays to match the computed array sizes.
        //
        int dataSrcTypeI;
        for ( dataSrcTypeI=0; dataSrcTypeI < kDataSrc_COUNT; dataSrcTypeI++ )
        {    
            //
            // Clear out old arrays (probably won't every be needed, since
            // this is only called at program initialization time.
            //
            size_t pgmTypeI;
            for ( pgmTypeI=0; pgmTypeI < ARRAY_SIZE(sLocalDataCache[dataSrcTypeI]); pgmTypeI++ )
            {
                tParamDataCache* pCache = &sLocalDataCache[dataSrcTypeI][pgmTypeI];
                if ( pCache->arraySz > 0 )
                {
                    size_t nBytesNeeded;
                    
                    // the data array
                    nBytesNeeded = ( kBytesInFloat * pCache->arraySz );
                    pCache->data = (GLfloat*)malloc( nBytesNeeded );
                    memset( pCache->data, 0, nBytesNeeded );
                    
                    // the "isSet" array
                    nBytesNeeded = ( sizeof(bool) * pCache->arraySz );
                    pCache->isSet = (bool*)malloc( nBytesNeeded );
                    memset( pCache->isSet, 0, nBytesNeeded );

                    // the "dirty" array
                    nBytesNeeded = ( sizeof(bool) * pCache->arraySz );
                    pCache->dirty = (bool*)malloc( nBytesNeeded );
                    memset( pCache->dirty, 0, nBytesNeeded );
                }
            }
        }
    }
    
    //
    //    Store some of these locations for use in grabbing OpenGL values.
    //
    s_modelview_matrix    = GetCachedValueForCacheId( kMatrixParam_ModelView );
    s_projection_matrix    = GetCachedValueForCacheId( kMatrixParam_Projection );
    s_texture_matrix    = GetCachedValueForCacheId( kMatrixParam_Texture0 );
    s_color_matrix        = GetCachedValueForCacheId( kMatrixParam_Color );
}

void WCW_InitOnce(void)
{
    int tableI;
    
    rdrBeginMarker(__FUNCTION__);
    WCW_FetchGLState();
    WCW_CheckGLState();

    buildLocalDataCache();
    
    //
    // Validate the param table.
    //
    GrowParamHandleCache( ARRAY_SIZE(sShaderParamSpecTbl) );    // make room in our param handle cache for these predefined values
    for ( tableI=0; tableI < ARRAY_SIZE(sShaderParamSpecTbl); tableI++ )
    {
        tShaderProgramType pgmType;
        tDataSrcType nDataSrcType;
        assert( sShaderParamSpecTbl[tableI].id == tableI );    // ...if not, the table is not in ShaderParamId order!
        ValidateShaderProgramTypeConstant( sShaderParamSpecTbl[tableI].nProgramType );
        pgmType  = sShaderParamSpecTbl[tableI].nProgramType;

        //
        // All entries in the sShaderParamSpecTbl[] have a paramNmCacheKey equal to their array offset.
        // IOW, given a "paramNmCacheKey" you can not only locate the param handle cache element but also the
        // param spec (descriptive) element. 
        //
        // For paramNmCacheKey values >= ARRAY_SIZE(sShaderParamSpecTbl) this handy double linking does not
        // apply. The paramNmCacheKey value is the offset in the param handle cache, but (of course) not the
        // offset in a param spec item. That would happen if other modules chose not to register their param 
        // specs in the master table, but did use the param handle cache.
        //
        sShaderParamSpecTbl[tableI].paramNmCacheKey = AddParamHandleCacheKey( sShaderParamSpecTbl[tableI].szStdParamName, pgmType );
        assert( sShaderParamSpecTbl[tableI].paramNmCacheKey == tableI );
        
        nDataSrcType = sShaderParamSpecTbl[tableI].dataSrcType;
        sShaderParamSpecTbl[tableI].dataCacheFloatOfs = sLocalDataCache[nDataSrcType][pgmType].numItems;
        sLocalDataCache[nDataSrcType][pgmType].numItems += 
            ( ShaderParamSpec_FloatsPerElement(&sShaderParamSpecTbl[tableI]) *
                sShaderParamSpecTbl[tableI].numElements );
    }
    // Make sure we didn't overrun the cache array range when assigning index values.
    {
        int i;
        for ( i=0; i < kDataSrc_COUNT; i++ )
        {
            assert( sLocalDataCache[i][kShaderPgmType_FRAGMENT].numItems <= sLocalDataCache[i][kShaderPgmType_FRAGMENT].arraySz );
            assert( sLocalDataCache[i][kShaderPgmType_VERTEX].numItems   <= sLocalDataCache[i][kShaderPgmType_VERTEX].arraySz );
        }
    }
    rdrEndMarker();
}

void WCW_Shutdown(void)
{
    state.valid = 0;
}

void WCW_EnableBlend(void)
{
    ASSERT_GLON(GL_BLEND, state.blend_enabled);

    if (NO_STATEMANAGEMENT || state.blend_enabled != GL_TRUE) {
        glEnable(GL_BLEND); CHECKGL;
        state.blend_enabled = GL_TRUE;
    }
}

void WCW_DisableBlend(void)
{
    ASSERT_GLON(GL_BLEND, state.blend_enabled);

    if (NO_STATEMANAGEMENT || state.blend_enabled != GL_FALSE) {
        glDisable(GL_BLEND); CHECKGL;
        state.blend_enabled = GL_FALSE;
    }
}

static INLINEDBG void WCW_ApplyBlendFuncInline(GLenum sfactor, GLenum dfactor)
{
    ASSERT_GLON(GL_BLEND, state.blend_enabled);
    ASSERT_GLINT(GL_BLEND_SRC, state.blend_src);
    ASSERT_GLINT(GL_BLEND_DST, state.blend_dst);

    if (sfactor == GL_ONE && dfactor == GL_ZERO) {
        WCW_DisableBlend();
    } else {
        if (NO_STATEMANAGEMENT || sfactor != state.blend_src || dfactor != state.blend_dst) {
            glBlendFunc(sfactor, dfactor); CHECKGL;
            state.blend_src = sfactor;
            state.blend_dst = dfactor;
        }

        WCW_EnableBlend();
    }
}

void WCW_BlendFunc(GLenum sfactor, GLenum dfactor)
{
    WCW_ApplyBlendFuncInline(sfactor, dfactor);
    glBlendFuncStackIdx = 0;
    glBlendFuncStack[0].sfactor = sfactor;
    glBlendFuncStack[0].dfactor = dfactor;
}

void WCW_BlendFuncPush(GLenum sfactor, GLenum dfactor)
{
    if (glBlendFuncStackFrozen)
        return;

    WCW_ApplyBlendFuncInline(sfactor, dfactor);

    glBlendFuncStackIdx++;
    assert(glBlendFuncStackIdx < 32);
    glBlendFuncStack[glBlendFuncStackIdx].sfactor = sfactor;
    glBlendFuncStack[glBlendFuncStackIdx].dfactor = dfactor;
}

void WCW_BlendFuncPushNop(void)
{
    if (glBlendFuncStackFrozen)
        return;

    glBlendFuncStackIdx++;
    assert(glBlendFuncStackIdx < 32);
    glBlendFuncStack[glBlendFuncStackIdx].sfactor = glBlendFuncStack[glBlendFuncStackIdx-1].sfactor;
    glBlendFuncStack[glBlendFuncStackIdx].dfactor = glBlendFuncStack[glBlendFuncStackIdx-1].dfactor;
}

void WCW_BlendFuncPop(void)
{
    if (glBlendFuncStackFrozen)
        return;

    if (glBlendFuncStackIdx > 0)
    {
        glBlendFuncStackIdx--;
        WCW_ApplyBlendFuncInline(glBlendFuncStack[glBlendFuncStackIdx].sfactor, glBlendFuncStack[glBlendFuncStackIdx].dfactor);
    }
}

void WCW_BlendStackFreeze(int freeze)
{
    glBlendFuncStackFrozen = freeze;
}


int fakeFogDisable=0; // Enables the faked method of disabling fog that we use for the ARB_assembly path

//#define FOG_DEBUG
#ifdef FOG_DEBUG
#include <utilitieslib/utils/memlog.h>
MemLog g_FogLog;
#endif

void WCW_InitFogContext(FogContext *fog)
{
    fog_context = fog;
    fog->on = 0;
    // setup state tracking as 'uninitialized' so that first calls go through to GL
    fog->old_fog[0] = 1;        // GL_FOG_DENSITY
    fog->old_fog[1] = -1;        // GL_FOG_START
    fog->old_fog[2] = -1;        // GL_FOG_END
    fog->old_fog[3] = -1;        // GL_FOG_MODE
    fog->curr_fog[0] = 1;        // GL_FOG_DENSITY
    fog->curr_fog[1] = -1;        // GL_FOG_START
    fog->curr_fog[2] = -1;        // GL_FOG_END
    fog->curr_fog[3] = GL_LINEAR;        // GL_FOG_MODE
    fog->fog_stack_depth = 0;
    fog->fog_deferring = 0;
    setVec4(fog->fog_color, 1.0f, 1.0f, 1.0f, 1.0f);
    WCW_Fog(1); // init fog values
    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_FogState_Params | kOpenGLDirtyBit_FogState_Color;
}

void WCW_SetFogContext(FogContext *fog)
{
    fog_context = fog;

    fog->on = ~fog->on;
    fog_context->fog_doneonce = 0;

    glFogf(GL_FOG_DENSITY, fog_context->curr_fog[0]); CHECKGL;
    glFogf(GL_FOG_START, fog_context->curr_fog[1]); CHECKGL;
    glFogf(GL_FOG_END, fog_context->curr_fog[2]); CHECKGL;
    glFogf(GL_FOG_MODE, fog_context->curr_fog[3]); CHECKGL;
    glFogfv(GL_FOG_COLOR, fog_context->fog_color); CHECKGL;

    WCW_Fog(~fog->on); // init fog values
    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_FogState_Params | kOpenGLDirtyBit_FogState_Color;
}

void WCW_SetFogDeferring(int deferring)
{
    fog_context->fog_deferring = deferring;
    WCW_Fog(1);
}

int WCW_FogDeferring()
{
    return fog_context->fog_deferring;
}

void WCW_Fogf(GLenum pname, GLfloat param)
{
    if ((pname == GL_FOG_START || pname == GL_FOG_END) && fog_context->on==0 && ((rdr_caps.chip & ARBFP) || fakeFogDisable))
    {
        // Fog is off, set the restore parameter
        fog_context->old_fog[pname - GL_FOG_DENSITY] = param;
    }
    else if (NO_STATEMANAGEMENT || param != fog_context->old_fog[pname - GL_FOG_DENSITY])
    {
        fog_context->old_fog[pname - GL_FOG_DENSITY] = param;
        fog_context->curr_fog[pname - GL_FOG_DENSITY] = param;
        glFogf(pname,param); CHECKGL;
    }
}

void WCW_FogColor(Vec3 fog_color)
{
    if (NO_STATEMANAGEMENT || memcmp(fog_color, fog_context->fog_color, 12) != 0)
    {
        copyVec3(fog_color, fog_context->fog_color); // fog_context->fog_color is a vec4, which glFogfv requires.  We always keep alpha component 1.0
        glFogfv(GL_FOG_COLOR, fog_context->fog_color); CHECKGL;
        sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_FogState_Color;
    }
}

void WCW_Fog(int on)
{
    fog_context->last_on = on;
    if (!(on && (!editMode() || edit_state.showfog) && !game_state.ortho) || game_state.noFog || fog_context->fog_deferring)
        on = 0;
    if (NO_STATEMANAGEMENT || on != fog_context->on)
    {
        if ((rdr_caps.chip & ARBFP) || fakeFogDisable)
        {
            // Regular Fog disable/enable doesn't work right unless you write custom pixel shaders
            if (!fog_context->fog_doneonce) {
                fog_context->fog_doneonce = 1;
                glEnable(GL_FOG); CHECKGL;
            }
            if (on)
            {
                if (fog_context->old_fog[1]==-1 && fog_context->old_fog[2]==-1)
                {
                    // State has been reset, restore defaults!
                    fog_context->old_fog[1] = 1000;
                    fog_context->old_fog[2] = 2000;
                }
                fog_context->curr_fog[1] = fog_context->old_fog[1];
                fog_context->curr_fog[2] = fog_context->old_fog[2];
                glFogf(GL_FOG_START,fog_context->curr_fog[1]); CHECKGL;
                glFogf(GL_FOG_END,fog_context->curr_fog[2]); CHECKGL;
            }
            else
            {
                fog_context->old_fog[1] = fog_context->curr_fog[1];
                fog_context->old_fog[2] = fog_context->curr_fog[2];
                fog_context->curr_fog[1] = 1000000;
                fog_context->curr_fog[2] = 1000001;
                glFogf(GL_FOG_START,fog_context->curr_fog[1]); CHECKGL;
                glFogf(GL_FOG_END,fog_context->curr_fog[2]); CHECKGL;
            }

            sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_FogState_Params;
        }
        else
        {
            // Normal mode
            if (on) {
                glEnable(GL_FOG); CHECKGL;
            } else {
                glDisable(GL_FOG); CHECKGL;
            }            
        }
        fog_context->on = on;
    }
}

void WCW_FogPush(int on)
{
    if (fog_context->fog_stack_depth >= ARRAY_SIZE(fog_context->fog_stack))
        failText("Fog stack overflow.\n");
    else
        fog_context->fog_stack[fog_context->fog_stack_depth] = fog_context->on;
    WCW_Fog(on);
    fog_context->fog_stack_depth++;
}

void WCW_FogPop(void)
{
    if (fog_context->fog_stack_depth==0)
        failText("Fog stack underflow.\n");
    else
        fog_context->fog_stack_depth--;
    WCW_Fog(fog_context->fog_stack[fog_context->fog_stack_depth]);
}

const FogContext *WCW_GetFogContext(void)
{
    return fog_context;
}

void WCW_EnableClipPlanes(int numPlanes, GLfloat planes[6][4])
{
    int i = 0;
    U64 dirtyBit = kOpenGLDirtyBit_ClipPlane_0;

    assert(numPlanes < 6);

    if (numPlanes)
    {
        glMatrixMode(GL_MODELVIEW); CHECKGL;
        glPushMatrix(); CHECKGL;
        glLoadIdentity(); CHECKGL;

        for (i = 0; i < numPlanes; i++, dirtyBit <<= 1)
        {
            // Store the clip-planes in view-space since the shaders will need them in view space
            Vec3 point_on_plane, point_on_plane_vs;
            Vec4 clipplane_vs;

            scaleVec3(planes[i], -planes[i][3], point_on_plane);
            mulVecMat3(planes[i], rdr_view_state.viewmat, clipplane_vs);
            mulVecMat4(point_on_plane, rdr_view_state.viewmat, point_on_plane_vs);
            clipplane_vs[3] = -dotVec3(clipplane_vs, point_on_plane_vs);

            clipplanes[i][0] = clipplane_vs[0];
            clipplanes[i][1] = clipplane_vs[1];
            clipplanes[i][2] = clipplane_vs[2];
            clipplanes[i][3] = clipplane_vs[3];

            glClipPlane(GL_CLIP_PLANE0 + i, clipplanes[i]); CHECKGL;
            glEnable(GL_CLIP_PLANE0 + i); CHECKGL;

            sOpenGLShaderParamDirtyFlags |= dirtyBit;
        }

        glPopMatrix(); CHECKGL;
    }


    for (; i < numclipplanesenabled; i++, dirtyBit <<= 1)
    {
        clipplanes[i][0] = 0.0f;
        clipplanes[i][1] = 0.0f;
        clipplanes[i][2] = 0.0f;
        clipplanes[i][3] = 0.0f;
        glClipPlane(GL_CLIP_PLANE0 + i, clipplanes[i]); CHECKGL;
        glDisable(GL_CLIP_PLANE0 + i); CHECKGL;

        // Do we care about the disabled planes?
        //sOpenGLShaderParamDirtyFlags |= dirtyBit;
    }

    numclipplanesenabled = numPlanes;

    WCW_ManageClipPlaneProjection();
}


// Shear the current projection matrix so that the near plane becomes the desired
// user clip plane. This is useful for clipping reflections when user clip planes
// are not available (e.g. skinned vertex program output on non nVidia hardware)
//
// Reference for code that was already here:
// Lengyel, Eric. Modifying the Projection Matrix to Perform Oblique Near-plane Clipping.
// Terathon Software 3D Graphics Library, 2004. http://www.terathon.com/code/oblique.html
//
// reworked some to fit with usage for skinned gfx nodes.
// supply clipPlane in view space, current normal OpenGL projection matrix is M_proj
// output is M_clip_proj which should be  glLoadMatrixf to GL_PROJECTION when clipping is desired

static float sgn(float a)
{
    if (a > 0.0F) return (1.0F);
    if (a < 0.0F) return (-1.0F);
    return (0.0F);
}

static void CalcClipProjectionMatrix( Mat44 M_clip_proj, const Mat44 M_proj, Vec4 clipPlane )
{
    float   matrix[16];
    Vec4    q,c;
    float    scale;

    // Grab the current projection matrix from OpenGL
    copyMat44(M_proj, *(Mat44*)&matrix);

    // make sure the plane is oriented the correct direction
    if(clipPlane[3] < 0)
        scaleVec4( clipPlane, -1.0f, clipPlane );

    // Calculate the clip-space corner point opposite the clipping plane
    // as (sgn(clipPlane.x), sgn(clipPlane.y), 1, 1) and
    // transform it into camera space by multiplying it
    // by the inverse of the projection matrix

    q[0] = (sgn(clipPlane[0]) + matrix[8]) / matrix[0];
    q[1] = (sgn(clipPlane[1]) + matrix[9]) / matrix[5];
    q[2] = -1.0F;
    q[3] = (1.0F + matrix[10]) / matrix[14];

    // Calculate the scaled plane vector
    scale = 2.0f/dotVec4(clipPlane,q);
    scaleVec4( clipPlane, scale, c );

    // Replace the third row of the projection matrix
    matrix[2] = c[0];
    matrix[6] = c[1];
    matrix[10] = c[2] + 1.0F;
    matrix[14] = c[3];

    // Load it back into OpenGL
    copyMat44(*(Mat44*)&matrix, M_clip_proj);
}

// This code is used to manage a special projection matrix
// to perform user clipping of reflections using the near plane
// We generally use it to clip skinned characters at the reflection
// plane on non-nVidia hardware (world geo uses gl clip planes through 'position invariance)
// Also see comments above for CalcClipProjectionMatrix()
void WCW_ManageClipPlaneProjection(void)
{
    // if this is a reflective viewport and we
    // need to use a sheared projection to do the
    // clipping generate that matrix and save it
    // for use as needed
    if (!rdr_caps.supports_vp_clip_planes_nv && numclipplanesenabled && rdr_view_state.renderPass == RENDERPASS_REFLECTION )
    {
        Vec4 clipPlane;
        clipPlane[0] = clipplanes[0][0];
        clipPlane[1] = clipplanes[0][1];
        clipPlane[2] = clipplanes[0][2];
        clipPlane[3] = clipplanes[0][3];

        CalcClipProjectionMatrix( rdr_view_state.projectionClipMatrix, rdr_view_state.projectionMatrix, clipPlane );

        // Check the current projection matrix mode, can apply to the whole viewport
        // or just to individual entities (since it is only skinned entities that we
        // currently need special entities for)
        if ( game_state.reflection_proj_clip_mode == kReflectProjClip_All )
        {
            WCW_LoadProjectionMatrix(rdr_view_state.projectionClipMatrix);
            glMatrixMode(GL_MODELVIEW); CHECKGL;
        }
    }
}

void WCW_EnableStencilTest(void)
{
    //if (senabled) return;
    glEnable( GL_STENCIL_TEST ); CHECKGL;
    senabled = 1;
}

void WCW_StencilFunc( GLenum func, GLint ref, GLuint mask )
{
    //if ( sfunc == func && sref == ref && smask == mask )
     //   return;
    glStencilFunc( func, ref, mask ); CHECKGL;
    sfunc = func;
    sref  = ref;
    smask = mask;
}

void WCW_StencilOp( GLenum fail, GLenum zfail, GLenum zpass )
{
    //if ( sfail == fail && szfail == zfail && szpass == zpass )
     //   return;
    glStencilOp( fail, zfail, zpass ); CHECKGL;
    sfail  = fail;
    szfail = zfail;
    szpass = zpass;
}

void WCW_DisableStencilTest(void)
{
   //if (!senabled) return;
    glDisable( GL_STENCIL_TEST ); CHECKGL;
    senabled = 0;
}


//Note that l is ignored and GL_LIGHT0 is assumed to always be the light in question
//Somewhere, GL_POSITION is getting reset to 0,0,0.

void WCW_Light( GLenum l, GLenum pname, const float light[] )
{
    F32 mlight[4];
    int idx = pname - GL_AMBIENT;
    int light_index = l - GL_LIGHT0;

    // GL_POSITION should be handled by calling WCW_LightPosition()
    assert(pname != GL_POSITION);
    assert(idx >= 0 && idx < 4);
    assert(light_index >= 0 && light_index < 2);
    
    if (NO_STATEMANAGEMENT || (0 != memcmp( slight[light_index][idx], light, 16 )) ) 
    {
        memcpy( slight[light_index][idx], light, 16 );
        if ( !rdr_caps.chip )
        {
            scaleVec3(light,4,mlight);
            light = mlight;
        }

        glLightfv( l, pname, light ); CHECKGL;
        memcpy(slight_ogl[light_index][idx], light, 16);

        if (pname == GL_AMBIENT)
            sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_LightVal_ProdAmbient | (kOpenGLDirtyBit_LightVal_Ambient0 << light_index);
        else if (pname == GL_DIFFUSE)
            sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_LightVal_ProdDiffuse | (kOpenGLDirtyBit_LightVal_Diffuse0 << light_index);
        else if (pname == GL_SPECULAR)
            sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_LightVal_ProdSpecular | (kOpenGLDirtyBit_LightVal_Specular0 << light_index);
    }
}

void WCW_LightPosition( const float light[], const Mat4 lightMtx )
{
    const GLenum l = GL_LIGHT0;
    const GLenum pname = GL_POSITION;

    F32 mlight[4];
    int idx = pname - GL_AMBIENT;
    int light_index = l - GL_LIGHT0;

    assert(idx >= 0 && idx < 4);
    assert(light_index >= 0 && light_index < 2);

    //memcpy( slight[light_index][idx], light, 16 );

    glMatrixMode(GL_MODELVIEW); CHECKGL;
    glPushMatrix(); CHECKGL;

    if (lightMtx)
    {
        Mat44 matx;
        mat43to44(unitmat,matx);
        glLoadMatrixf((F32 *)matx); CHECKGL;
    }
    else
    {
        glLoadIdentity(); CHECKGL;
    }

    glLightfv( l, pname, light ); CHECKGL;

    scaleVec3(light,-1,mlight);
    mlight[3] = light[3];
    glLightfv( GL_LIGHT1, pname, mlight ); CHECKGL;
    
    glPopMatrix(); CHECKGL;
        
    if (lightMtx)
    {
        mulVecMat4(light, lightMtx, slight_ogl[0][idx]);
        slight_ogl[0][idx][3] = light[3];

        mulVecMat4(mlight, lightMtx, slight_ogl[1][idx]);
        slight_ogl[1][idx][3] = light[3];
    }
    else
    {
        memcpy(slight_ogl[0][idx], light, 16);
        memcpy(slight_ogl[1][idx], mlight, 16);
    }

    sun_lightdir_valid = 0;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_LightVal_Position;
}

void WCW_Color4(U8 red, U8 green, U8 blue, U8 alpha)
{
    if (NO_STATEMANAGEMENT || (red != scolor_red) || (green != scolor_green) || (blue != scolor_blue || alpha != scolor_alpha))
    {
        glColor4ub(red, green, blue, alpha);
        CHECKGL;
        scolor_red = red;
        scolor_green = green;
        scolor_blue = blue;
        scolor_alpha = alpha;
    }
}

void WCW_Color4_Force(U8 red, U8 green, U8 blue, U8 alpha)
{
    glColor4ub(red, green, blue, alpha);
    scolor_red = red;
    scolor_green = green;
    scolor_blue = blue;
    scolor_alpha = alpha;
}


void WCW_Material(GLenum face, GLenum pname, const float *params)
{
    assert(face == GL_FRONT);

    if (pname == GL_SHININESS)
    {
        smaterial[3][0] = params[0];
        glMateriali(face, pname, params[0]); CHECKGL;
        sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_LightVal_MatShininess;
    }
    else
    {
        int idx = pname - GL_AMBIENT;
        assert(idx >= 0 && idx < 3);

        memcpy(smaterial[idx], params, 16);
        glMaterialfv(face, pname, params); CHECKGL;

        if (pname == GL_AMBIENT)
            sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_LightVal_ProdAmbient;
        else if (pname == GL_DIFFUSE)
            sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_LightVal_ProdDiffuse;
        else if (pname == GL_SPECULAR)
            sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_LightVal_ProdSpecular;
    }
}
void WCW_LoadModelViewMatrix(const Mat4 m)
{
    mat43to44(m, cast_floatptr_to_mat44(s_modelview_matrix));

    glMatrixMode(GL_MODELVIEW); CHECKGL;
    glLoadMatrixf(s_modelview_matrix); CHECKGL;
    
    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_ModelView | kOpenGLDirtyBit_Matrix_ModelViewIT | kOpenGLDirtyBit_Matrix_ModelViewProjection;
}

void WCW_LoadModelViewMatrixM44(const Mat44 m)
{
    rdrSetMarker(__FUNCTION__);
    memcpy(s_modelview_matrix, m, 64);

    glMatrixMode(GL_MODELVIEW); CHECKGL;
    glLoadMatrixf((F32 *)m); CHECKGL;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_ModelView | kOpenGLDirtyBit_Matrix_ModelViewIT | kOpenGLDirtyBit_Matrix_ModelViewProjection;
}

void WCW_LoadModelViewMatrixIdentity()
{
    identityMat44(cast_floatptr_to_mat44(s_modelview_matrix));

    glMatrixMode(GL_MODELVIEW); CHECKGL;
    glLoadIdentity(); CHECKGL;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_ModelView | kOpenGLDirtyBit_Matrix_ModelViewIT | kOpenGLDirtyBit_Matrix_ModelViewProjection;
}

void WCW_SetModelViewMatrixDirty()
{
    glGetFloatv(GL_MODELVIEW_MATRIX, s_modelview_matrix); CHECKGL;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_ModelView | kOpenGLDirtyBit_Matrix_ModelViewIT | kOpenGLDirtyBit_Matrix_ModelViewProjection;
}

void WCW_LoadProjectionMatrix(const Mat44 p)
{
    if ( s_projection_matrix == NULL ) return;    // calling before we've initialized
    memcpy(s_projection_matrix, p, 64);

    glMatrixMode(GL_PROJECTION); CHECKGL;
    glLoadMatrixf((F32 *)p); CHECKGL;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_Projection | kOpenGLDirtyBit_Matrix_ModelViewProjection;
}

void WCW_LoadProjectionMatrixIdentity()
{
    identityMat44(cast_floatptr_to_mat44(s_projection_matrix));

    glMatrixMode(GL_PROJECTION); CHECKGL;
    glLoadIdentity(); CHECKGL;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_Projection | kOpenGLDirtyBit_Matrix_ModelViewProjection;
}

void WCW_SetProjectionMatrixDirty()
{
    glGetFloatv(GL_PROJECTION_MATRIX, s_projection_matrix); CHECKGL;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_Projection | kOpenGLDirtyBit_Matrix_ModelViewProjection;
}

void WCW_LoadTextureMatrix(int texindex, const Mat4 mat)
{
    int textureUnitAsFloatOfs = ( texindex << 4 );    // times 16 to adjust index from Mat44's to floats
    assert(texindex >=0 && texindex < MAX_TEXTURE_UNITS_TOTAL);

    rdrBeginMarker(__FUNCTION__);
    mat43to44(mat, cast_floatptr_to_mat44( s_texture_matrix + textureUnitAsFloatOfs ));

    WCW_ActiveTexture(texindex);
    glMatrixMode(GL_TEXTURE); CHECKGL;
    glLoadMatrixf( s_texture_matrix + textureUnitAsFloatOfs ); CHECKGL;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_Texture;
    rdrEndMarker();
}

void WCW_LoadTextureMatrixIdentity(int texindex)
{
    int textureUnitAsFloatOfs = ( texindex << 4 );    // times 16 to adjust index from Mat44's to floats
    assert(texindex >=0 && texindex < MAX_TEXTURE_UNITS_TOTAL);

    rdrBeginMarker(__FUNCTION__);
    identityMat44(cast_floatptr_to_mat44(s_texture_matrix + textureUnitAsFloatOfs ));

    WCW_ActiveTexture(texindex);
    glMatrixMode(GL_TEXTURE); CHECKGL;
    glLoadIdentity(); CHECKGL;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_Texture;
    rdrEndMarker();
}

void WCW_ScaleTextureMatrix(int texindex, const Vec3 scale)
{
    GLfloat* pTextureMatFloat = ( s_texture_matrix + ( texindex << 4 ) );    // times 16 to adjust index from Mat44's to floats
    Vec4* pTextureMat44 = cast_floatptr_to_mat44( pTextureMatFloat );
    assert(texindex >=0 && texindex < MAX_TEXTURE_UNITS_TOTAL);

    rdrBeginMarker(__FUNCTION__);
    scaleVec4(pTextureMat44[0], scale[0], pTextureMat44[0]);
    scaleVec4(pTextureMat44[1], scale[1], pTextureMat44[1]);
    scaleVec4(pTextureMat44[2], scale[2], pTextureMat44[2]);

    WCW_ActiveTexture(texindex);
    glMatrixMode(GL_TEXTURE); CHECKGL;
    glLoadMatrixf( pTextureMatFloat ); CHECKGL;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_Texture;
    rdrEndMarker();
}

void WCW_TranslateTextureMatrix(int texindex, const Vec3 translation)
{
    GLfloat* pTextureMatFloat = ( s_texture_matrix + ( texindex << 4 ) );    // times 16 to adjust index from Mat44's to floats
    Vec4* pTextureMat44 = cast_floatptr_to_mat44( pTextureMatFloat );
    assert(texindex >=0 && texindex < MAX_TEXTURE_UNITS_TOTAL);

    rdrBeginMarker(__FUNCTION__);
    addToVec3(translation, pTextureMat44[3]);

    WCW_ActiveTexture(texindex);
    glMatrixMode(GL_TEXTURE); CHECKGL;
    glLoadMatrixf(pTextureMatFloat); CHECKGL;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_Texture;
    rdrEndMarker();
}

void WCW_LoadColorMatrix(const Mat44 c)
{
    rdrBeginMarker(__FUNCTION__);
    memcpy(s_color_matrix, c, 64);

    glMatrixMode(GL_COLOR); CHECKGL;
    glLoadMatrixf((F32 *)c); CHECKGL;

    sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_Color;
    rdrEndMarker();
}

void WCW_LoadColorMatrixIdentity()
{
    if( GLEW_ARB_imaging )
    {
        rdrBeginMarker(__FUNCTION__);
        identityMat44(cast_floatptr_to_mat44(s_color_matrix));

        glMatrixMode(GL_COLOR); CHECKGL;
        glLoadIdentity(); CHECKGL;

        sOpenGLShaderParamDirtyFlags |= kOpenGLDirtyBit_Matrix_Color;
        rdrEndMarker();
    }
}

void WCW_SunLightDir(void)
{
    if (NO_STATEMANAGEMENT || !sun_lightdir_valid)
    {
        rdr_view_state.sun_direction_in_viewspace[3] = 0;
        WCW_LightPosition(rdr_view_state.sun_direction_in_viewspace, NULL); //(w == 0) == directional
        sun_lightdir_valid = 1;
    }
}

// This function is used to setup const color 0 and 1 for tinting/dual blend
// as appropriate for the hardware features available
void WCW_ConstantCombinerParamerterfv( int idx, float constColor[] )
{
    static int color_ids[3][2] =
    {
        { GL_CONSTANT_COLOR0_NV, GL_CONSTANT_COLOR1_NV },
        { GL_CON_3_ATI, GL_CON_4_ATI },
        { 0, 1}
    };

    if ( 0 != memcmp( &(combiconst[idx]), constColor, 16 ) ) {
        rdrBeginMarker(__FUNCTION__);
        memcpy( combiconst[idx], constColor, 16 );

        if (rdr_caps.chip & ARBFP) {
            // For ARB fragment programs these map to ENV constants instead of LOCAL as they are
            // set outside binding a fragment program and are shared by all programs

            setFragmentProgramConstColor(color_ids[2][idx], constColor);
        } else if (rdr_caps.chip & NV1X) {
            glCombinerParameterfvNV(color_ids[0][idx], constColor); CHECKGL;
        } else if (rdr_caps.chip & R200) {
            glSetFragmentShaderConstantATI(color_ids[1][idx], constColor); CHECKGL;
        } else { //if (rdr_caps.chip == TEX_ENV_COMBINE
            if (idx == 0) {
                WCW_ActiveTexture(0);
                glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, constColor); CHECKGL;
            } else if (idx == 1) {
                // done in rdrPrepareForDraw
                //glColor4fv(constColor);
            }
        }
        rdrEndMarker();
    }
    g_renderstate_dirty = true;
}

void WCW_DepthMask( GLboolean mask )
{
    if (NO_STATEMANAGEMENT || sdepthmask != mask ) 
    {
        sdepthmask = mask;
        glDepthMask( mask ); CHECKGL;
    }
}

#include "cmdparse/cmdgame.h"
//This function shouldn't needto exist, and should flag an error if GL texture state isn't correct
void WCW_ClearTextureStateToWhite(void)
{
    rdrBeginMarker(__FUNCTION__);
    if ((rdr_caps.features & GFXF_BUMPMAPS) || rdr_caps.chip == TEX_ENV_COMBINE) {
        WCW_BindTexture(GL_TEXTURE_2D, 2, rdr_view_state.white_tex_id);
    }
    WCW_BindTexture(GL_TEXTURE_2D, 1, rdr_view_state.white_tex_id);
    WCW_BindTexture(GL_TEXTURE_2D, 0, rdr_view_state.white_tex_id);
    rdrEndMarker();
}

void WCW_ActiveTexture( int texindex )
{
    const GLenum texunit = GL_TEXTURE0 + texindex;

    assert(texindex >= 0 && texindex < MAX_TEXTURE_UNITS_TOTAL);
    ASSERT_GLINT(GL_ACTIVE_TEXTURE, state.texunit_bound);

    if(NO_STATEMANAGEMENT || state.texunit_bound != texunit) {
        glActiveTextureARB(texunit); CHECKGL;
        state.texunit_bound = texunit;
    }
}

void WCW_BindTextureFast( GLenum target, int texindex, GLuint tex )
{
    assert(texindex >= 0 && texindex < MAX_TEXTURE_UNITS_TOTAL);

    RT_STAT_INC(texbind_calls);

    if (NO_STATEMANAGEMENT || state.tex_bound[texindex] != tex || state.tex_target[texindex] != target)
    {
        WCW_ActiveTexture(texindex);

        ASSERT_GLINT(bindForTarget(state.tex_target[texindex]), state.tex_bound[texindex]);

        glBindTexture(target, tex); CHECKGL;
        state.tex_target[texindex] = target;
        state.tex_bound[texindex] = tex;

        RT_STAT_TEXBIND_CHANGE(texindex);
    }
}

void WCW_BindTexture( GLenum target, int texindex, GLuint tex )
{
    assert(texindex >= 0 && texindex < MAX_TEXTURE_UNITS_TOTAL);

    if (texindex >= rdr_caps.num_texunits)
        return;

    WCW_ActiveTexture(texindex);

    ASSERT_GLINT(bindForTarget(state.tex_target[texindex]), state.tex_bound[texindex]);

    WCW_BindTextureFast(target, texindex, tex);
}

void WCW_ClientActiveTexture( int texindex )
{
    const GLenum texunit = GL_TEXTURE0 + texindex;

    assert(texindex >= 0 && texindex < MAX_TEXTURE_UNITS_TOTAL);

    ASSERT_GLINT(GL_CLIENT_ACTIVE_TEXTURE, state.clienttexunit_bound);

    if(NO_STATEMANAGEMENT || state.clienttexunit_bound != texunit) {
        glClientActiveTextureARB(texunit); CHECKGL;
        state.clienttexunit_bound = texunit;
    }
}

//Note pointer managers assume that a given pointer will always have the same params
void WCW_TexCoordPointer( int texindex, GLint size, GLenum type,  GLsizei stride, const GLvoid *pointer )
{
    assert(texindex >= 0 && texindex < MAX_TEXTURE_UNITS_TOTAL);

    if (NO_STATEMANAGEMENT || tex_pointer[texindex] != pointer || !last_vertex_array_id)
    {
        WCW_ClientActiveTexture(texindex);
        glTexCoordPointer( size, type, stride, pointer ); CHECKGL;
        tex_pointer[texindex] = pointer;
    }
}

void WCW_NormalPointer( GLenum type,  GLsizei stride, const GLvoid *pointer )
{
    if (NO_STATEMANAGEMENT || norm_pointer != pointer || !last_vertex_array_id)
    {
        glNormalPointer( type, stride, pointer ); CHECKGL;
        norm_pointer = pointer;
    }
}

GLuint WCW_BindBuffer(GLsizei target, GLuint buffer)
{
    static bool used_vbos=false;

    if (!rdr_caps.use_vbos) {
        if (used_vbos && last_element_array_id==-1) {
            last_element_array_id = 0;
            glBindBufferARB(GL_ARRAY_BUFFER_ARB,0); CHECKGL;
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB,0); CHECKGL;
        }
        return buffer;
    }
    used_vbos = true;

    if (target == GL_ELEMENT_ARRAY_BUFFER_ARB)
    {
        if (buffer == last_element_array_id && !NO_STATEMANAGEMENT)
            return buffer;
        last_element_array_id = buffer;
    }
    if (target == GL_ARRAY_BUFFER_ARB)
    {
        RT_STAT_INC(buffer_calls);
        if (buffer == last_vertex_array_id && !NO_STATEMANAGEMENT)
            return buffer;
        last_vertex_array_id = buffer;
        vert_pointer = (void *)0xffffffff;
        norm_pointer = (void *)0xffffffff;
        memset((void *)tex_pointer, 0xff, sizeof(tex_pointer));
        RT_STAT_BUFFER_CHANGE;
    }
    glBindBufferARB(target,buffer); CHECKGL;
    return(buffer);
}

void WCW_VertexPointer( GLint size, GLenum type,  GLsizei stride, const GLvoid *pointer )
{   
    if (NO_STATEMANAGEMENT || vert_pointer != pointer || !last_vertex_array_id) 
    {
        glVertexPointer( size, type, stride, pointer ); CHECKGL;
        vert_pointer = pointer;
    }
}

void WCW_EnableColorMaterial(void)
{
    glEnable( GL_COLOR_MATERIAL ); CHECKGL;
}
void WCW_DisableColorMaterial(void)
{
    glDisable( GL_COLOR_MATERIAL ); CHECKGL;
}

void WCW_TexLODBias(int texindex, F32 newbias)
{    
    if (GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_GROUPDEF_TRAVERSAL))
        return;

#if 0
    // Aug 12, 2009 - The 0.01 value was causing the not-equal test below to not work properly
    // Brandon says "tex2D with LOD bias is a performance hog on some GPUs as well.
    // If the value is not exactly zero we could be hitting slower paths in the texture lookups."
    if (newbias > -8)
        newbias = MIN(0.01, newbias + game_state.texLodBias); // This number is minned to 0.01 because it's almost the same as 0, and avoids some bug in ATI's driver's statemanager, apparently.
#else
    if (newbias > -8)
        newbias = MIN(0.0f, newbias + game_state.texLodBias);
#endif

    if(NO_STATEMANAGEMENT || texLODbias[texindex] != newbias)
    {
        WCW_ActiveTexture(texindex);
        glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, newbias); CHECKGL;
        texLODbias[texindex] = newbias;
    }
}

static INLINEDBG void doEnableClientState(GLClientStateType array)
{
    if (rdr_caps.chip & ARBVP)
    {
        switch (array)
        {
        case GLC_VERTEX_ARRAY:
        case GLC_NORMAL_ARRAY:
        case GLC_COLOR_ARRAY:
            glEnableClientState(clientstate[array].arrayid); CHECKGL;
            break;
        case GLC_TEXTURE_COORD_ARRAY_0:
        case GLC_TEXTURE_COORD_ARRAY_1:
        case GLC_TEXTURE_COORD_ARRAY_2:
        case GLC_TEXTURE_COORD_ARRAY_3:
        case GLC_TEXTURE_COORD_ARRAY_4:
        case GLC_TEXTURE_COORD_ARRAY_5:
        case GLC_TEXTURE_COORD_ARRAY_6:
        case GLC_TEXTURE_COORD_ARRAY_7:
            WCW_ClientActiveTexture(array - GLC_TEXTURE_COORD_ARRAY_0); CHECKGL;
            glEnableClientState(GL_TEXTURE_COORD_ARRAY); CHECKGL;
            break;
        case GLC_VERTEX_ATTRIB_ARRAY1_NV:
            glEnableVertexAttribArrayARB(1); CHECKGL;
            break;
        case GLC_VERTEX_ATTRIB_ARRAY5_NV:
            glEnableVertexAttribArrayARB(5); CHECKGL;
            break;
        case GLC_VERTEX_ATTRIB_ARRAY6_NV:
            glEnableVertexAttribArrayARB(6); CHECKGL;
            break;
        case GLC_VERTEX_ATTRIB_ARRAY7_NV:
            glEnableVertexAttribArrayARB(7); CHECKGL;
            break;
        case GLC_VERTEX_ATTRIB_ARRAY11_NV:
            glEnableVertexAttribArrayARB(11); CHECKGL;
            break;
        }
    }
    else
    {
        if(array >= GLC_TEXTURE_COORD_ARRAY_0 && array <= GLC_TEXTURE_COORD_ARRAY_7) {
            WCW_ClientActiveTexture(array - GLC_TEXTURE_COORD_ARRAY_0); CHECKGL;
        }
        glEnableClientState(clientstate[array].arrayid); CHECKGL;
    }
}

static INLINEDBG void doDisableClientState( GLClientStateType array )
{
    if (rdr_caps.chip & ARBVP)
    {
        switch (array)
        {
        case GLC_VERTEX_ARRAY:
        case GLC_NORMAL_ARRAY:
        case GLC_COLOR_ARRAY:
            glDisableClientState(clientstate[array].arrayid); CHECKGL;
            break;
        case GLC_TEXTURE_COORD_ARRAY_0:
        case GLC_TEXTURE_COORD_ARRAY_1:
        case GLC_TEXTURE_COORD_ARRAY_2:
        case GLC_TEXTURE_COORD_ARRAY_3:
        case GLC_TEXTURE_COORD_ARRAY_4:
        case GLC_TEXTURE_COORD_ARRAY_5:
        case GLC_TEXTURE_COORD_ARRAY_6:
        case GLC_TEXTURE_COORD_ARRAY_7:
            WCW_ClientActiveTexture(array - GLC_TEXTURE_COORD_ARRAY_0); CHECKGL;
            glDisableClientState(GL_TEXTURE_COORD_ARRAY); CHECKGL;
            break;
        case GLC_VERTEX_ATTRIB_ARRAY1_NV:
            glDisableVertexAttribArrayARB(1); CHECKGL;
            break;
        case GLC_VERTEX_ATTRIB_ARRAY5_NV:
            glDisableVertexAttribArrayARB(5); CHECKGL;
            break;
        case GLC_VERTEX_ATTRIB_ARRAY6_NV:
            glDisableVertexAttribArrayARB(6); CHECKGL;
            break;
        case GLC_VERTEX_ATTRIB_ARRAY7_NV:
            glDisableVertexAttribArrayARB(7); CHECKGL;
            break;
        case GLC_VERTEX_ATTRIB_ARRAY11_NV:
            glDisableVertexAttribArrayARB(11); CHECKGL;
            break;
        }
    }
    else
    {
        if(array >= GLC_TEXTURE_COORD_ARRAY_0 && array <= GLC_TEXTURE_COORD_ARRAY_7) {
            WCW_ClientActiveTexture(array - GLC_TEXTURE_COORD_ARRAY_0); CHECKGL;
        }
        glDisableClientState(clientstate[array].arrayid); CHECKGL;
    }
}

void WCW_EnableClientState( GLClientStateType array )
{
    if (NO_STATEMANAGEMENT || clientstate[array].on == 0)
    {
        doEnableClientState(array);
        clientstate[array].on = 1;
    }
}

void WCW_DisableClientState( GLClientStateType array )
{
    if (NO_STATEMANAGEMENT || clientstate[array].on == 1)
    {
        doDisableClientState(array);
        clientstate[array].on = 0;
    }
}

void WCW_DisableAllClientState(void)
{
    int i;
    for(i = 0; i<ARRAY_SIZE(clientstate); i++) {
        if(NO_STATEMANAGEMENT || clientstate[i].on) {
            WCW_DisableTexture(i);
        }
    }
}

static INLINEDBG ASSERT_ALLTEXOFF(void)
{
        ASSERT_GLON(GL_TEXTURE_1D, GL_FALSE);
        ASSERT_GLON(GL_TEXTURE_2D, GL_FALSE);
        if(rdr_caps.supports_arb_rect) ASSERT_GLON(GL_TEXTURE_RECTANGLE_ARB, GL_FALSE);
        if(0) ASSERT_GLON(GL_TEXTURE_3D, GL_FALSE);
        ASSERT_GLON(GL_TEXTURE_CUBE_MAP, GL_FALSE);
}

void WCW_EnableTexture(GLenum textarget, int texindex)
{
    if (texindex >= rdr_caps.num_texunits)
        return;

    if (NO_STATEMANAGEMENT || state.tex_enabled[texindex] != textarget) {
        WCW_ActiveTexture(texindex);

        if (state.tex_enabled[texindex]) {
            ASSERT_GLON(state.tex_enabled[texindex], GL_TRUE);
            glDisable(state.tex_enabled[texindex]); CHECKGL;
        }

        ASSERT_ALLTEXOFF();

        glEnable(textarget); CHECKGL;
        state.tex_enabled[texindex] = textarget;
    }
}

void WCW_DisableTexture(int texindex)
{
    if (texindex >= rdr_caps.num_texunits)
        return;

    if (NO_STATEMANAGEMENT || state.tex_enabled[texindex]) {
        WCW_ActiveTexture(texindex);

        if(NO_STATEMANAGEMENT && !state.tex_enabled[texindex]) {
            ASSERT_ALLTEXOFF();

            glDisable(GL_TEXTURE_1D); CHECKGL;
            glDisable(GL_TEXTURE_2D); CHECKGL;
            if(rdr_caps.supports_arb_rect) glDisable(GL_TEXTURE_RECTANGLE_ARB); CHECKGL;
            if(0) glDisable(GL_TEXTURE_3D); CHECKGL;
            glDisable(GL_TEXTURE_CUBE_MAP); CHECKGL;
            return;
        }

        ASSERT_GLON(state.tex_enabled[texindex], GL_TRUE);
        glDisable(state.tex_enabled[texindex]); CHECKGL;
        ASSERT_ALLTEXOFF();
        state.tex_enabled[texindex] = 0;
    }
}

void WCW_DisableAllTextures(void)
{
    int i;
    rdrBeginMarker(__FUNCTION__);
    for(i = 0; i<MAX_TEXTURE_UNITS_TOTAL; i++) {
        if(NO_STATEMANAGEMENT || state.tex_enabled[i]) {
            WCW_DisableTexture(i);
        }
    }
    rdrEndMarker();
}

void WCW_BufferData(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage,const char *memname)
{
    // Transgaming claims that using DYNAMIC_DRAW is generally faster in nvidia Macs.
    if (!(rdr_caps.chip & R200) && IsUsingCider())
        usage = GL_DYNAMIC_DRAW_ARB;

    glBufferDataARB(target, size, data, usage); CHECKGL;
    memMonitorTrackUserMemory(memname, 1, MOT_ALLOC, size);
}

void WCW_DeleteBuffers(GLenum target,GLsizei n, const GLuint *buffers,const char *memname) 
{
    int        id,size;

    if (buffers[0]==0)
        return;

    assert(n==1);
    id = buffers[0];
    WCW_BindBuffer(target, id);
    glGetBufferParameterivARB(target,GL_BUFFER_SIZE_ARB,&size); CHECKGL;
    memMonitorTrackUserMemory(memname, 1, MOT_FREE, size);
    glDeleteBuffersARB(n,buffers); CHECKGL;
    last_element_array_id = -1;
    last_vertex_array_id = -1;

    vert_pointer = (void *)0xffffffff;
    norm_pointer = (void *)0xffffffff;
    memset((void *)tex_pointer, 0xff, sizeof(tex_pointer));
}

void WCW_EnableVertexProgram(void)
{
    rdrBeginMarker(__FUNCTION__);
    if (NO_STATEMANAGEMENT || vertexProgramEnabled!=1)
    {
        // NOTE GL_VERTEX_PROGRAM_ARB == GL_VERTEX_PROGRAM_NV; so it isn't 
        // necessary to check this conditional
        if(rdr_caps.chip & (ARBVP|GLSL))
        {
            tCgShaderMode shaderMode = rt_cgGetCgShaderMode();
            if ( shaderMode )
            {
                rt_cgEnableProgram(kShaderPgmType_VERTEX, rt_cgGetCgShaderProfileForMode(shaderMode, kShaderPgmType_VERTEX));
            }
            else
            {
                glEnable(GL_VERTEX_PROGRAM_ARB); CHECKGL;
            }            
        }

        vertexProgramEnabled = 1;
    }
    rdrEndMarker();
}

void WCW_BindVertexProgram(GLuint id)
{
    rdrSetMarker(__FUNCTION__ " : %s [%d]", "", id ); // ParamTable_ShaderFileName(id), id);
    if(NO_STATEMANAGEMENT || id != boundVertexProgram)
    {
        if(rdr_caps.chip & (ARBVP|GLSL))
        {
            RT_STAT_VP_CHANGE;
            if ( rt_cgGetCgShaderMode() )
            {
                rt_cgBindCgProgram(kShaderPgmType_VERTEX, id );
            }
            else
            {
                glBindProgramARB(GL_VERTEX_PROGRAM_ARB, id); CHECKGL;
                // JE: It appears we're not actually state managing this here, but we are in modelDrawState, 
                //   and we enable and disable vertex programs without clearing boundVertexProgram, so I'm
                //   just going to leave this as it is for now.
            }
        }
        boundVertexProgram = id;        
        // Don't do "sOpenGLShaderParamDirtyFlags = kOpenGLDirtyBit_ALL" here; wait until the
        // shader has been set up and the values fed to it.
    }
}

void WCW_DisableVertexProgram(void)
{
    rdrSetMarker(__FUNCTION__);
    if(NO_STATEMANAGEMENT || vertexProgramEnabled!=0)
    {
        if(rdr_caps.chip & (ARBVP|GLSL))
        {
            tCgShaderMode shaderMode = rt_cgGetCgShaderMode();
            if ( shaderMode )
            {
                rt_cgDisableProgram(kShaderPgmType_VERTEX);
            }
            else
            {
                glDisable(GL_VERTEX_PROGRAM_ARB); CHECKGL;
            }
        }

        vertexProgramEnabled = 0;
    }
}

void WCW_ResetVertexProgram(void)
{
    rdrBeginMarker(__FUNCTION__);
#if 0
    if(rdr_caps.chip & (ARBVP|GLSL))
    {
        if (vertexProgramEnabled == 1)
        {
            glEnable(GL_VERTEX_PROGRAM_ARB); CHECKGL;
            if (boundVertexProgram != 0xFFFFFFFF)
            {
                RT_STAT_VP_CHANGE;
                glBindProgramARB(GL_VERTEX_PROGRAM_ARB, boundVertexProgram); CHECKGL;
            }

        }
        else
        {
            glDisable(GL_VERTEX_PROGRAM_ARB); CHECKGL;
        }
    }
#else
    vertexProgramEnabled = -1;
    boundVertexProgram = 0xFFFFFFFF;
    SHADER_DBG_PRINTF( "Calling resetLocalParameters() from WCW_ResetVertexProgram().\n" );
    resetLocalParameters(kShaderPgmType_VERTEX);
#endif
    rdrEndMarker();
}

void WCW_EnableFragmentProgram(void)
{
    rdrBeginMarker(__FUNCTION__);
    if(NO_STATEMANAGEMENT || fragmentProgramEnabled!=1)
    {
        if(rdr_caps.chip & (ARBFP|GLSL))
        {
            tCgShaderMode shaderMode = rt_cgGetCgShaderMode();
            if ( shaderMode )
            {
                rt_cgEnableProgram(kShaderPgmType_FRAGMENT, rt_cgGetCgShaderProfileForMode(shaderMode, kShaderPgmType_FRAGMENT));
            }
            else
            {
                glEnable(GL_FRAGMENT_PROGRAM_ARB); CHECKGL;
            }
        }
        else if (rdr_caps.chip & R200)
        {
            glEnable( GL_FRAGMENT_SHADER_ATI ); CHECKGL;
        }
        fragmentProgramEnabled = 1;
    }
    rdrEndMarker();
}

void WCW_BindFragmentProgram(GLuint id)
{
    rdrSetMarker(__FUNCTION__ ":%s [%d]", "", id ); // ParamTable_ShaderFileName(id), id);
    if(NO_STATEMANAGEMENT || id != boundFragmentProgram)
    {
        if(rdr_caps.chip & (ARBFP|GLSL))
        {
            if ( rt_cgGetCgShaderMode() )
            {
                rt_cgBindCgProgram(kShaderPgmType_FRAGMENT, id);
            }
            else
            {
                glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, id); CHECKGL;
            }
        }
        else if(rdr_caps.chip & NV1X) //If NVidia
        {
            // get GL error on this call once per frame, only ever called with id 0 on reset
            // -- do we really need this?
            glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, id); CHECKGL;
        }
        boundFragmentProgram = id;
    }
}

void WCW_DisableFragmentProgram(void)
{
    rdrBeginMarker(__FUNCTION__);
    if(NO_STATEMANAGEMENT || fragmentProgramEnabled!=0)
    {
        if(rdr_caps.chip & (ARBFP|GLSL))
        {
            tCgShaderMode shaderMode = rt_cgGetCgShaderMode();
            if ( shaderMode )
            {
                rt_cgDisableProgram(kShaderPgmType_FRAGMENT);
            }
            else
            {    
                glDisable(GL_FRAGMENT_PROGRAM_ARB); CHECKGL;
            }
        }
        else if (rdr_caps.chip & R200)
        {
            glDisable( GL_FRAGMENT_SHADER_ATI ); CHECKGL;
        }
    }

    fragmentProgramEnabled = 0;
    rdrEndMarker();
}

void WCW_ResetFragmentProgram(void)
{
    rdrBeginMarker(__FUNCTION__);
#if 0
    if (fragmentProgramEnabled == 1)
    {
        if(rdr_caps.chip & (ARBFP|GLSL))
        {
            glEnable(GL_FRAGMENT_PROGRAM_ARB); CHECKGL;
            if (boundFragmentProgram != 0xFFFFFFFF)
            {
                glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, boundFragmentProgram); CHECKGL;
            }
        }
        else if(rdr_caps.chip & NV1X) //If NVidia
        {
            glEnable( GL_FRAGMENT_SHADER_ATI ); CHECKGL;
            if (boundFragmentProgram != 0xFFFFFFFF)
            {
                glBindProgramARB(GL_FRAGMENT_PROGRAM_NV, boundFragmentProgram); CHECKGL;
            }
        }
    }
    else
    {
        if(rdr_caps.chip & ARBFP)
        {
            glDisable(GL_FRAGMENT_PROGRAM_ARB); CHECKGL;
        }
        else if (rdr_caps.chip & R200)
        {
            glDisable( GL_FRAGMENT_SHADER_ATI ); CHECKGL;
        }
    }
#else
    fragmentProgramEnabled = -1;
    boundFragmentProgram = 0xFFFFFFFF;
    SHADER_DBG_PRINTF( "Calling resetLocalParameters() from WCW_ResetFragmentProgram().\n" );
    resetLocalParameters(kShaderPgmType_FRAGMENT);
#endif
    rdrEndMarker();
}


void WCW_EnableGL_NORMALIZE( int on )
{
    if (glNormalizeOverride!=-1)
        on = glNormalizeOverride;
    if (NO_STATEMANAGEMENT || on!=glNormalizeEnabled) {
        if (on) {
            glEnable(GL_NORMALIZE); CHECKGL;
        } else {
            glDisable(GL_NORMALIZE); CHECKGL;
        }
    }
    glNormalizeEnabled = on;
}

void WCW_OverrideGL_LIGHTING( int on )
{
    if (NO_STATEMANAGEMENT || glLightingOverride != on) {
        if (glLightingOverride == -1)
            glLightingNonOverride = glLightingEnabled;
        WCW_EnableGL_LIGHTING(on);
        glLightingOverride = on;
    }
}

void WCW_CancelOverrideGL_LIGHTING( void )
{
    if (glLightingOverride != -1) {
        glLightingOverride = -1;
        WCW_EnableGL_LIGHTING(glLightingNonOverride);
    }
}

void WCW_EnableGL_LIGHTING( int on )
{
    if (glLightingOverride!=-1) {
        // It was force set by override, save this for later
        glLightingNonOverride = on;
    } else {
        // Not overridden
        if (NO_STATEMANAGEMENT || on!=glLightingEnabled) {
            if (on) {
                glEnable(GL_LIGHTING); CHECKGL;
            } else {
                glDisable(GL_LIGHTING); CHECKGL;
            }
        }
        glLightingEnabled = on;
    }
}

static void resetLocalParameters(tShaderProgramType target)
{
    SHADER_DBG_PRINTF( "resetLocalParameters( %s )\n", SHADER_DBG_PGM_TYPE_STR(target) );
    memset( 
            sLocalDataCache[kDataSrc_PgmLocalVariable][target].isSet, 
            0, 
            ( sizeof(sLocalDataCache[kDataSrc_PgmLocalVariable][target].isSet[0]) * 
                sLocalDataCache[kDataSrc_PgmLocalVariable][target].numItems )
        );
}

void WCW_SetParamDirtyFlags( bool bVertexPgm, bool bFragmentPgm )
{
    int nDatsSrcI;
    SHADER_DBG_PRINTF( "WCW_SetParamDirtyFlags( bVertex=%d, bFragment=%d )\n", bVertexPgm, bFragmentPgm );
    
    for ( nDatsSrcI = 0; nDatsSrcI < kDataSrc_COUNT; nDatsSrcI++ )
    {
        int nPgmTypeI;
        for ( nPgmTypeI = 0; nPgmTypeI < kShaderPgmType_COUNT; nPgmTypeI++ )
        {
            if (( nPgmTypeI == kShaderPgmType_VERTEX )   && ( ! bVertexPgm ))   continue;
            if (( nPgmTypeI == kShaderPgmType_FRAGMENT ) && ( ! bFragmentPgm )) continue;
            memset( 
                    sLocalDataCache[nDatsSrcI][nPgmTypeI].dirty, 
                    1, 
                    ( sizeof(sLocalDataCache[nDatsSrcI][nPgmTypeI].dirty[0]) * 
                        sLocalDataCache[nDatsSrcI][nPgmTypeI].numItems )
                );
        }
    }

    sCurrentShaderOpenGLFlags = 0;
}

void WCW_SetCgShaderParamArray4fv(tShaderProgramType target, ShaderParamId id, const GLfloat *vec4Arr, GLuint nNumVec4s )
{
    tShaderParamSpec* pSpec = &sShaderParamSpecTbl[id];
    #if RT_SUPPORT_ARB_SHADER_PATH
    if ( ! rt_cgGetCgShaderMode() )
    {
        if ( pSpec->dataSrcType == kDataSrc_PgmLocalVariable )
        {
            setArbRegOBSOLETE( pSpec, vec4Arr, nNumVec4s );
        }
    }
    else
    #endif
    if ( RT_USE_HARD_CONST_PARAM_MODE() )
    {
        setArbEnvReg( pSpec, vec4Arr, nNumVec4s );
    }
    else
        setNamedParamValue( pSpec, vec4Arr, nNumVec4s, kFloatsInVec4 );
}

void WCW_SetCgShaderParamArray3fv(tShaderProgramType target, ShaderParamId id, const GLfloat *vec3Arr, GLuint nNumVec3s)
{
    tShaderParamSpec* pSpec = &sShaderParamSpecTbl[id];

    #if RT_SUPPORT_ARB_SHADER_PATH
        if ( ! rt_cgGetCgShaderMode() )
        {
            // We don't use or support vec3 based arrays for the old ARB pipeline
            return;
        }
    #endif
    if ( RT_USE_HARD_CONST_PARAM_MODE() )
    {
        setArbEnvReg3( pSpec, vec3Arr, nNumVec3s );
    }
    else
        setNamedParamValue( pSpec, vec3Arr, nNumVec3s, kFloatsInVec3 );
}

static void setFragmentProgramConstColor( GLuint index, const GLfloat* vec4 )
{
    #define kNumEnvParamsDefined 2
    assert( index < kNumEnvParamsDefined );
    
    #if RT_SUPPORT_ARB_SHADER_PATH
        if ( ! rt_cgGetCgShaderMode() )
        {
            glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, index, vec4 );
        }
        else
    #endif
    {
        static const ShaderParamId envParams[kNumEnvParamsDefined] = {
            kShaderParam_EnvVar0FP,    kShaderParam_EnvVar1FP
        };
        tShaderParamSpec* pSpec = &sShaderParamSpecTbl[ envParams[index] ];
        if ( RT_USE_HARD_CONST_PARAM_MODE() )
        {
            setArbEnvReg( pSpec, vec4, 1 );
        }
        else
        {
            setNamedParamValue( pSpec, vec4, 1, kFloatsInVec4 );
        }
    }
}

static void GrowParamHandleCache( GLuint nNewElements )
{
    // Add more elements
    size_t oldBytes = ( sShaderParamHandleCache.nArraySize * sizeof(tShaderParamHandleCacheEntry) );
    size_t newBytes = ( nNewElements * sizeof(tShaderParamHandleCacheEntry) );
    tShaderParamHandleCacheEntry* pNewEntries = (tShaderParamHandleCacheEntry*)malloc(newBytes);
    memset( pNewEntries, 0, newBytes );
    if ( oldBytes > 0 )
    {
        memcpy( pNewEntries, sShaderParamHandleCache.cacheData, oldBytes );
        free( sShaderParamHandleCache.cacheData );
    }
    sShaderParamHandleCache.cacheData = pNewEntries;
    sShaderParamHandleCache.nArraySize = nNewElements;
}

static int AddParamHandleCacheKey( const char* szParamName, tShaderProgramType pgmType )
{
    tShaderParamHandleCacheEntry* pEntry;

    //
    // Adds a new key without checking for an existing entry
    //
    int nIndex = sShaderParamHandleCache.nNumUsed;
    if ( sShaderParamHandleCache.nNumUsed == sShaderParamHandleCache.nArraySize )
    {
        static const GLuint kGrowSz = 16;
        GrowParamHandleCache( sShaderParamHandleCache.nArraySize + kGrowSz );
    }
    pEntry = ( sShaderParamHandleCache.cacheData + nIndex );
    pEntry->szParamName    = szParamName;
    pEntry->pgmType = pgmType;
    pEntry->programHdl    = NULL;
    pEntry->paramHdlList = NULL;
    pEntry->nListSz = 0;
    sShaderParamHandleCache.nNumUsed++;
    return nIndex;
}

int    WCW_GetCacheIdForShaderParamName( const char* szParamName, tShaderProgramType pgmType, bool bAdd )
{
    // Called at shader load time -- not every frame. :)
    GLuint i;
    for ( i=0; i < sShaderParamHandleCache.nNumUsed; i++ )
    {
        if (( sShaderParamHandleCache.cacheData[i].pgmType == pgmType ) &&
            ( stricmp( sShaderParamHandleCache.cacheData[i].szParamName, szParamName ) == 0 ))
        {
            return i;
        }
    }
    
    // Not found. Add it?
    if ( bAdd )
    {
        return AddParamHandleCacheKey( szParamName, pgmType );
    }
    
    return -1;
}

void WCW_GetFloatParamExpectedDimensions( int cacheKey, int* pElementRows, int* pElementCols )
{
    SHADER_DBG_ASSERT(( cacheKey >= 0 ) && ( cacheKey < ARRAY_SIZE(sShaderParamSpecTbl) ));
    *pElementRows = sShaderParamSpecTbl[cacheKey].elementFloatRows;
    *pElementCols = sShaderParamSpecTbl[cacheKey].elementFloatCols;
}

static INLINEDBG bool RefreshCachedMatrixValueFromOpenGL( tShaderParamSpec* pSpec, GLfloat* pTargetData )
{
    SHADER_DBG_ASSERT( pSpec->dataSrcType == kDataSrc_OpenGLMatrix );
    SHADER_DBG_ASSERT( ShaderParamSpec_FloatsPerElement( pSpec ) == 16 );
    
    SHADER_DBG_PRINTF( "  RefreshCachedMatrixValueFromOpenGL( %s )\n", pSpec->szStdParamName );
    
    // The dataKey1 is the matrix type.
    switch ( pSpec->dataKey1 )
    {
        case kMatrix_ModelView :
        {
            break;
        }
        case kMatrix_ModelViewIT :    // inverse transpose
        {
            invertMat44Copy(*((Mat44*)s_modelview_matrix), *((Mat44*)pTargetData));
            transposeMat44(*((Mat44*)pTargetData));
            break;
        }
        case kMatrix_Projection :
        {
            break;
        }
        case kMatrix_ModelViewProj :    // GL_MODELVIEW & GL_PROJECTION
        {
            mulMat44Inline( rdr_view_state.projectionMatrix, *((Mat44*)s_modelview_matrix), *((Mat44*)pTargetData));
            break;
        }
        case kMatrix_Texture :
        {
            break;
        }
        case kMatrix_Color :
        {
            break;
        }
        default :
            assert(0);
            break;
    }

    return true;
}

static INLINEDBG bool RefreshCachedFogStateValueFromOpenGL( tShaderParamSpec* pSpec, GLfloat* pTargetData )
{
    bool bNewData = false;
    GLfloat previousVal[kFloatsInVec4];
    SHADER_DBG_ASSERT( pSpec->dataSrcType == kDataSrc_FogState );
    SHADER_DBG_ASSERT( ShaderParamSpec_FloatsPerElement( pSpec ) == 4 );
    
    switch ( pSpec->dataKey1 )
    {
        case kFogState_Params :
            memcpy( previousVal, pTargetData, sizeof(previousVal) );
            pTargetData[0] = fog_context->curr_fog[0];
            pTargetData[1] = fog_context->curr_fog[1];
            pTargetData[2] = fog_context->curr_fog[2];
            pTargetData[3] = ( 1.0f / ( pTargetData[2] - pTargetData[1] ) );
            bNewData = ( memcmp( pTargetData, previousVal, sizeof(previousVal) ) != 0 );
            break;
        case kFogState_Color :
            bNewData = ( memcmp( fog_context->fog_color, pTargetData, sizeof(Vec3) ) != 0 );
            if ( bNewData )
            {
                copyVec3(fog_context->fog_color, pTargetData);
            }
            break;
        default :
            assert(0);
            break;
    }
    
    return bNewData;
}

static INLINEDBG bool RefreshCachedLightingValueFromOpenGL( tShaderParamSpec* pSpec, GLfloat* pTargetData )
{
    bool bNewData = false;
    SHADER_DBG_ASSERT( pSpec->dataSrcType == kDataSrc_LightingModel );
    SHADER_DBG_ASSERT( ShaderParamSpec_FloatsPerElement( pSpec ) == 4 );
    
    switch ( pSpec->dataKey1 )
    {
        case kLightVal_Position :
            bNewData = ( memcmp( pTargetData, slight_ogl[pSpec->dataKey2][GL_POSITION - GL_AMBIENT], 16) != 0 );
            if ( bNewData )
            {
                memcpy(pTargetData, slight_ogl[pSpec->dataKey2][GL_POSITION - GL_AMBIENT], 16);
            }
            break;
        case kLightVal_Half :
            assert(0); // unimplemented
            break;            
        case kLightVal_Diffuse :
            bNewData = ( memcmp(pTargetData, slight_ogl[pSpec->dataKey2][GL_DIFFUSE - GL_AMBIENT], 16) != 0 );
            if ( bNewData )
            {
                memcpy(pTargetData, slight_ogl[pSpec->dataKey2][GL_DIFFUSE - GL_AMBIENT], 16);
            }
            break;
        case kLightVal_Ambient :
            bNewData = ( memcmp(pTargetData, slight_ogl[pSpec->dataKey2][GL_AMBIENT - GL_AMBIENT], 16) != 0 );
            if ( bNewData )
            {
                memcpy(pTargetData, slight_ogl[pSpec->dataKey2][GL_AMBIENT - GL_AMBIENT], 16);
            }
            break;
        case kLightVal_Specular :
            bNewData = ( memcmp(pTargetData, slight_ogl[pSpec->dataKey2][GL_SPECULAR - GL_AMBIENT], 16) != 0 );
            if ( bNewData )
            {
                memcpy(pTargetData, slight_ogl[pSpec->dataKey2][GL_SPECULAR - GL_AMBIENT], 16);
            }
            break;
        case kLightVal_MatShininess :
        {
            Vec4 previousVal;
            copyVec4( pTargetData, previousVal );
            pTargetData[0] = smaterial[3][0];
            pTargetData[1] = 0.0f;
            pTargetData[2] = 0.0f;
            pTargetData[3] = 1.0f;
            bNewData = ( memcmp( previousVal, pTargetData, sizeof(Vec4) ) != 0 );
            break;
        }
        case kLightVal_ProdDiffuse :
        case kLightVal_ProdAmbient :
        case kLightVal_ProdSpecular :
        {
            int i;
            GLenum propEnum;
            Vec4 materialColor, lightColor;
            Vec4 previousVal;
            copyVec4( pTargetData, previousVal );
            switch ( pSpec->dataKey1 )
            {
                case kLightVal_ProdDiffuse :    propEnum = GL_DIFFUSE;    break;
                case kLightVal_ProdAmbient :    propEnum = GL_AMBIENT;    break;
                case kLightVal_ProdSpecular :    propEnum = GL_SPECULAR;    break;
            }        
            memcpy(materialColor, smaterial[propEnum - GL_AMBIENT], 16);
            memcpy(lightColor, slight_ogl[pSpec->dataKey2][propEnum - GL_AMBIENT], 16);
            for ( i=0; i < 3; i++ ) 
            {
                pTargetData[i] = ( materialColor[i] * lightColor[i] );
            }
            pTargetData[3] = materialColor[3];
            bNewData = ( memcmp( previousVal, pTargetData, sizeof(Vec4) ) != 0 );
            break;
        }
        default :
            assert(0);
            break;
    }

    return bNewData;
}

static INLINEDBG bool RefreshCachedClipValueFromOpenGL( tShaderParamSpec* pSpec, GLfloat* pTargetData )
{
    bool bNewData = false;
    SHADER_DBG_ASSERT( pSpec->dataSrcType == kDataSrc_Clipping );
    SHADER_DBG_ASSERT( ShaderParamSpec_FloatsPerElement( pSpec ) == 4 );
    
    switch ( pSpec->dataKey1 )
    {
        case kClipInfo_Plane :
        {
            int plane_index = pSpec->dataKey2;
            Vec4 previousValue;
            copyVec4( pTargetData, previousValue );
            pTargetData[0] = clipplanes[plane_index][0];
            pTargetData[1] = clipplanes[plane_index][1];
            pTargetData[2] = clipplanes[plane_index][2];
            pTargetData[3] = clipplanes[plane_index][3];
            bNewData = ( memcmp( pTargetData, previousValue, sizeof(Vec4) ) != 0 );
            break;
        }
        default :
            assert(0);
            break;
    }

    return bNewData;
}

void WCW_UpdateShaderParamsFromLocalCache( const tCgParamCacheSpec* paramSpecs, GLuint paramSpecsCount )
{
    if (RT_USE_HARD_CONST_PARAM_MODE())
    {
        return;
    }
    rdrBeginMarker(__FUNCTION__);

    sOpenGLShaderParamDirtyBitsApplied = 0;

    if (sOpenGLShaderParamDirtyFlags & sCurrentShaderOpenGLFlags || !sNothingToPush)
    {
        GLuint paramI;
        for ( paramI=0; paramI < paramSpecsCount; paramI++ )
        {
            const tCgParamCacheSpec* pParamData = ( paramSpecs + paramI );
            int nParamNmCacheId = pParamData->paramNmCacheKey;
            tShaderParamSpec* pSpec = NULL;

            CGparameter* paramHdlList = NULL;
            GLuint nParamHdlListSz = 0;
            bool bUpdateShaderNow = false;

#if 0
            if (paramI + 8 < paramSpecsCount)
                SSE_PREFETCH_L2(pParamData + 8);

            if (paramI + 6 < paramSpecsCount)
                SSE_PREFETCH_L2(&pParamData[6].paramNmCacheKey);

            if (paramI + 4 < paramSpecsCount)
            {
                int prefetchCacheId = pParamData[4].paramNmCacheKey;
                SSE_PREFETCH_L2(&sShaderParamSpecTbl[prefetchCacheId]);
                SSE_PREFETCH_L2(&sShaderParamHandleCache.cacheData[prefetchCacheId].paramHdlList);
            }

            if (paramI + 2 < paramSpecsCount)
            {
                int prefetchCacheId = pParamData[2].paramNmCacheKey;
                SSE_PREFETCH_L2(&sShaderParamSpecTbl[prefetchCacheId].openGlDirtyBit);
            }

#endif

            WCW_ASSERT( nParamNmCacheId < ARRAY_SIZE(sShaderParamSpecTbl) );
            
            pSpec        = &sShaderParamSpecTbl[nParamNmCacheId];

            // Update cached shader value from OpenGL
            if ( sOpenGLShaderParamDirtyFlags & pSpec->openGlDirtyBit )
            {
                bool bNewData = false;
                int nItemsSet = 0;
                int itemsSetI;
                GLfloat* cacheLoc;
                tParamDataCache* pDataCache;

                pDataCache    = GetCacheDataForParamSpec( pSpec );
                cacheLoc    = ( pDataCache->data  + pSpec->dataCacheFloatOfs );

                SHADER_DBG_PRINTF( "  WCW_UpdateShaderParamsFromLocalCache() - Item %2d of %2d: %s\n", paramI+1, paramSpecsCount, pSpec->szStdParamName );

                switch ( pSpec->dataSrcType )
                {
                    case kDataSrc_OpenGLMatrix :
                        SHADER_DBG_ASSERT(pSpec->elementFloatRows == 4);
                        SHADER_DBG_ASSERT(pSpec->elementFloatCols == 4);
                        nItemsSet = pSpec->elementFloatRows;
                        bNewData = RefreshCachedMatrixValueFromOpenGL( pSpec, cacheLoc );
                        break;
                    case kDataSrc_FogState :
                        SHADER_DBG_ASSERT(pSpec->elementFloatRows == 1);
                        SHADER_DBG_ASSERT(pSpec->elementFloatCols == 4);
                        nItemsSet = pSpec->elementFloatRows;
                        bNewData = RefreshCachedFogStateValueFromOpenGL( pSpec, cacheLoc );
                        break;
                    case kDataSrc_LightingModel :
                        SHADER_DBG_ASSERT(pSpec->elementFloatRows == 1);
                        SHADER_DBG_ASSERT(pSpec->elementFloatCols == 4);
                        nItemsSet = pSpec->elementFloatRows;
                        bNewData = RefreshCachedLightingValueFromOpenGL( pSpec, cacheLoc );
                        break;
                    case kDataSrc_Clipping :
                        SHADER_DBG_ASSERT(pSpec->elementFloatRows == 1);
                        SHADER_DBG_ASSERT(pSpec->elementFloatCols == 4);
                        nItemsSet = pSpec->elementFloatRows;
                        bNewData = RefreshCachedClipValueFromOpenGL( pSpec, cacheLoc );
                        break;
                    default :
                        // This default case is probably not reachable if pSpec->openGlDirtyBit is set correctly
                        // OK, since not all types of params need to be refreshed here.
                        // No need to set nItemsSet to zero here since it is initialized to 0
                        //nItemsSet = 0;
                        break;
                }

                if (bNewData)
                {
                    pDataCache->dirty[pSpec->dataCacheFloatOfs] = true;
                    sNothingToPush = false;
                }

                SHADER_DBG_PRINTF( "    Data %s.\n", ( bNewData ? "changed" : "did not change" ));
                
                sOpenGLShaderParamDirtyBitsApplied |= pSpec->openGlDirtyBit;

                for ( itemsSetI=0; itemsSetI < nItemsSet; itemsSetI++ )
                {
                    pDataCache->isSet[pSpec->dataCacheFloatOfs + itemsSetI] = true;
                }
            }

            SHADER_DBG_PRINTF( "  PushShaderParamValuesToTheShader( %s )\n", pSpec->szStdParamName );
            
            WCW_ASSERT( pSpec->paramNmCacheKey == nParamNmCacheId );
            
            // bUpdateShaderNow will be set if we have a shader set up and ready
            // to be updated. In other words: we have some param handles to use.
            // That should always be true.
            bUpdateShaderNow = WCW_GetCachedParamHandleList( 
                                                nParamNmCacheId, 
                                                &paramHdlList, 
                                                &nParamHdlListSz );

            assert( bUpdateShaderNow );
            if ( bUpdateShaderNow )
            {
                // The cache
                tParamDataCache* pDataCache    = GetCacheDataForParamSpec( pSpec );
                const GLfloat* paramData    = ( pDataCache->data  + pSpec->dataCacheFloatOfs );
                bool* isSetLoc                = ( pDataCache->isSet + pSpec->dataCacheFloatOfs );
                bool* isDirtyLoc            = ( pDataCache->dirty + pSpec->dataCacheFloatOfs );
                int cacheOfs                = 0;

                //
                // There will be a param handle for each "item", which might be a float4, float4x4, etc.
                // So the number of "rows" may not be the same as the number of params.
                //
                GLuint i;
                GLuint nFloatsPerParam        = ShaderParamSpec_FloatsPerElement( pSpec );
                GLuint nNumRowsPerParam        = ( nFloatsPerParam / pSpec->elementFloatCols );
                GLuint nNumParamsToSet        = pSpec->numElements;
                GLuint nFlags                = 0;
                
                if ( pSpec->dataSrcType == kDataSrc_OpenGLMatrix )
                {
                    nFlags |= kCgFX_SetFloat_AsMatrix;
                }
            
                nNumParamsToSet = min( nNumParamsToSet, nParamHdlListSz );
                
                assert( nNumParamsToSet <= (GLuint)pSpec->numElements );

                for ( i=0; i < nNumParamsToSet; i++ )
                {
                    if (( paramHdlList[i] != NULL ) && ( *isSetLoc ) && ( *isDirtyLoc ))
                    {
                        WCW_ASSERT( paramHdlList[i] != NULL );
                        SHADER_DBG_PRINTF( "    Updating param %s[%d]. ", pSpec->szStdParamName, i );
                        SHADER_DBG_DUMPFLOATS( paramData, (int)(pSpec->elementFloatRows * pSpec->elementFloatCols) );

                        rdrSetMarker("%s,%s,%d,%s,%d,%d",
                                     pSpec->szStdParamName,
                                     cgGetResourceString(cgGetParameterResource(paramHdlList[i])),
                                     cgGetParameterResourceIndex(paramHdlList[i]),
                                     cgGetTypeString(cgGetParameterType(paramHdlList[i])),
                                     pSpec->elementFloatRows,
                                     pSpec->elementFloatCols);

                        rt_cgSetFloatParam( paramHdlList[i],
                                                pSpec->elementFloatRows,
                                                pSpec->elementFloatCols,
                                                paramData,
                                                nFlags
                                            );
                        if ( pSpec->dataSrcType != kDataSrc_EnvVariable )
                        {
                            SHADER_DBG_PRINTF( "    Clearing dirty flag for %s[%d] in PushShaderParamValuesToTheShader().\n", pSpec->szStdParamName, i );
                            *isDirtyLoc = false;    // has been sent to the shader, so no longer dirty
                        }
                        else
                        {
                            SHADER_DBG_PRINTF( "    Not clearing dirty flag for env variable %s in PushShaderParamValuesToTheShader().\n", pSpec->szStdParamName );
                        }
                    } // set and dirty
                    else
                    {
                        SHADER_DBG_PRINTF( "    Not updating %s[%d]. IsSet=%d, Dirty=%d.\n", pSpec->szStdParamName, i, *isSetLoc, *isDirtyLoc );            
                        //SHADER_DBG_ASSERT( *isSetLoc );    // Somebody forgot to update this value?
                    }
                    paramData    += nFloatsPerParam;
                    isSetLoc    += pSpec->elementFloatCols;
                    isDirtyLoc    += pSpec->elementFloatCols;;
                }
            }    // bUpdateShaderNow
        }    // one tCgParamCacheSpec
    }

    //    
    // NOTE: One might be tempted to do this here:
    //        sOpenGLShaderParamDirtyFlags = 0;
    // But don't. First, the handlers called from WCW_UpdateShaderParamsFromLocalCache()
    // already take care of clearing the flags revelent to them. And more importantly,
    // at this point we have refreshed the params for *this* shader only. This shader may not
    // consume every type of OpenGL state data and therefore won't necesarily pull all
    // of that data into the param cache. If we do a blanket clearing of sOpenGLShaderParamDirtyFlags
    // the next shader, which might care about an OpenGL value that this shader doesn't use,
    // will think that the param cache is already up to date (dirty flag is clear) and we'll
    // push stale data to the shader.
    //
    sNothingToPush = true;

    sOpenGLShaderParamDirtyFlags &= ~sOpenGLShaderParamDirtyBitsApplied;

    rdrEndMarker();
}

#if RT_SUPPORT_CG_COMBO_SHADERS
    void WCW_PrepareToDrawCG()
    {
        if ( rt_cgGetCgShaderMode() ) {
            rt_cgFinalShaderPrep();
        }
    }
#endif

void WCW_CacheParamHandlesForShaderParam( CGprogram program, int nParamNmCacheId, CGparameter* paramList, int nListSz  )
{
    //
    //    Cache the handles for a shader param (identified by nParamNmCacheId). nListSz is 1 for a simple
    //    Vec4 param, but arrays will have multiple param handles: one for each element.
    //

    // We put the program handle and the param in the table. So when checking
    // for a cached param handle we can verify that the current program is
    // the one who cached the param handle. If not, it's a stale value and can
    // be ignored. This alleviates the need to keep the clearing old values.
    if (( nParamNmCacheId >= 0 ) && ( nParamNmCacheId < (int)sShaderParamHandleCache.nNumUsed ))
    {
        tShaderParamHandleCacheEntry* pEntry = &sShaderParamHandleCache.cacheData[nParamNmCacheId];
        assert( program != NULL );
        pEntry->programHdl        = program;
        pEntry->paramHdlList    = paramList;
        pEntry->nListSz            = nListSz;

        {
            sCurrentShaderOpenGLFlags |= sShaderParamSpecTbl[nParamNmCacheId].openGlDirtyBit;
        }
    }
}

#if RT_SUPPORT_ARB_SHADER_PATH
static void setArbRegOBSOLETE( tShaderParamSpec* pSpec, const GLfloat *vec4Arr, GLuint nNumVec4s )
{
    //    For ARB registers, the dataKey1 value is an ARB program.local[] variable index
    //    (i.e., it should match the C0...C255 semantic binding in the shader).
    static GLenum oglTargetTypes[] = {GL_FRAGMENT_PROGRAM_ARB, GL_VERTEX_PROGRAM_ARB};
    int target        = pSpec->nProgramType;

    // do the entire constant block w/ a single call if we can
    if ( GLEW_EXT_gpu_program_parameters )
    {
        int nArbRegister        = pSpec->dataKey1;
        glProgramLocalParameters4fvEXT(oglTargetTypes[target], nArbRegister, nNumVec4s, vec4Arr ); CHECKGL;
    }
    else
    {
        GLuint itemI    = 0;
        for ( itemI=0; itemI < nNumVec4s; itemI++ )
        {
            int nOfsOfVec4            = ( itemI * kFloatsInVec4 );
            int nArbRegister        = ( pSpec->dataKey1 + itemI );
            const GLfloat* vec4        = ( vec4Arr + nOfsOfVec4);

            assert(( target >= 0 ) && ( target <= 1 ));
            assert( itemI < (GLuint)pSpec->numElements  );

            rdrSetMarker("%s,%s,%d,%s,%f,%f,%f,%f",
                pSpec->szStdParamName,
                "LOC",
                nArbRegister,
                "float4",
                vec4[0],
                vec4[1],
                vec4[2],
                vec4[3]);

            glProgramLocalParameter4fvARB(oglTargetTypes[target], nArbRegister, vec4); CHECKGL;
        }
    }
}
#endif

static void setArbEnvReg( tShaderParamSpec* pSpec, const GLfloat *vec4Arr, GLuint nNumVec4s )
{
    //    For ARB registers, the dataKey1 value is an ARB program.env[] variable index
    //    (i.e., it should match the ENV0...ENV96 semantic binding in the shader).
    static GLenum oglTargetTypes[] = {GL_FRAGMENT_PROGRAM_ARB, GL_VERTEX_PROGRAM_ARB};
    int target        = pSpec->nProgramType;
    int nArbBaseRegister    =  pSpec->dataKey1;

    // do the entire constant block w/ a single call if we can
    if ( GLEW_EXT_gpu_program_parameters )
    {
        glProgramEnvParameters4fvEXT(oglTargetTypes[target], nArbBaseRegister, nNumVec4s, vec4Arr ); CHECKGL;
    }
    else
    {
        GLuint itemI    = 0;
        for ( itemI=0; itemI < nNumVec4s; itemI++ )
        {
            int nOfsOfVec4            = ( itemI * kFloatsInVec4 );
            int nArbRegister        = ( nArbBaseRegister + itemI );
            const GLfloat* vec4        = ( vec4Arr + nOfsOfVec4);

            assert(( target >= 0 ) && ( target <= 1 ));
            assert( itemI < (GLuint)pSpec->numElements  );

            rdrSetMarker("%s,%s,%d,%s,%f,%f,%f,%f",
                pSpec->szStdParamName,
                "ENV",
                nArbRegister,
                "float4",
                vec4[0],
                vec4[1],
                vec4[2],
                vec4[3]);

            glProgramEnvParameter4fvARB(oglTargetTypes[target], nArbRegister, vec4); CHECKGL;
        }
    }
}

// @todo probably better to not even support this and just use full 4 float wide
// values that will go to constants (e.g., we can't use EXT_gpu_program_parameters here)
static void setArbEnvReg3( tShaderParamSpec* pSpec, const GLfloat *vec3Arr, GLuint nNumVec3s )
{
    //    For ARB registers, the dataKey1 value is an ARB program.env[] variable index
    //    (i.e., it should match the ENV0...ENV96 semantic binding in the shader).
    static GLenum oglTargetTypes[] = {GL_FRAGMENT_PROGRAM_ARB, GL_VERTEX_PROGRAM_ARB};
    int target            = pSpec->nProgramType;
    int nArbBaseRegister=  pSpec->dataKey1;
    GLuint itemI    = 0;
    for ( itemI=0; itemI < nNumVec3s; itemI++ )
    {
        int nOfsOfVec3            = ( itemI * kFloatsInVec3 );
        int nArbRegister        = ( nArbBaseRegister + itemI );
        const GLfloat* vec3        = ( vec3Arr + nOfsOfVec3);
        GLfloat vec4[4];

        assert(( target >= 0 ) && ( target <= 1 ));
        assert( itemI < (GLuint)pSpec->numElements  );

        rdrSetMarker("%s,%s,%d,%s,%f,%f,%f",
            pSpec->szStdParamName,
            "ENV",
            nArbRegister,
            "float3",
            vec3[0],
            vec3[1],
            vec3[2]
            );

        vec4[0] = vec3[0];
        vec4[1] = vec3[1];
        vec4[2] = vec3[2];
        vec4[3] = 0;

        glProgramEnvParameter4fvARB(oglTargetTypes[target], nArbRegister, vec4); CHECKGL;
    }
}

static void setNamedParamValue( tShaderParamSpec* pSpec, const GLfloat *floatArr, GLuint nRows, GLuint nCols )
{
    CGparameter* paramHdlList = NULL;
    GLuint nParamHdlListSz = 0;
    
    SHADER_DBG_PRINTF( "setNamedParamValue( %s ). Values: ", pSpec->szStdParamName );
    SHADER_DBG_DUMPFLOATS( floatArr, (int)(nRows*nCols) );
    
    //
    //    The engine code wants to set a Cg param value for a shader. The goal here
    //    is to get the current param handle as efficiently as possible.
    //
    //    The approach to caching param handles is somewhat like the old spy
    //    movie trick of leaving a valuable in an airport locker and then passing
    //    the key to a co-consiprator.
    //
    //    When a shader is activated, it stores its param handles in our master
    //    table, which is analogous to a set of airport lockers. The array locations
    //    correspond with param names. They are like individual lockers. So any param
    //    named "g_Foo", for example, might use array element 12, say. The array index
    //    is our "key" for finding the "locker".
    //
    //    Here, we take that "key" (nIndex) and check the "locker". The param handle
    //    should be there. How do we know it's not a stale param handle left behind
    //    by some other shader? Because the shader activation code also records its
    //    program handle in the table. We can verify that the program handle matches
    //    the currently activated shader before using the param handle.
    //
    if ( pSpec->dataCacheFloatOfs >= 0 )
    {
        // Update the cache
        GLuint itemI;
        const GLfloat* itemData    = floatArr;
        tParamDataCache* pDataCache    = GetCacheDataForParamSpec( pSpec );
        GLfloat* cacheLoc        = ( pDataCache->data  + pSpec->dataCacheFloatOfs );
        bool* isSetLoc            = ( pDataCache->isSet + pSpec->dataCacheFloatOfs );
        bool* isDirtyLoc        = ( pDataCache->dirty + pSpec->dataCacheFloatOfs );
        int cacheOfs            = 0;
                
        for ( itemI=0; itemI < (GLuint)pSpec->numElements; itemI++ )
        {
            if ( itemI < nRows )
            {
                //
                //    Value being supplied by caller
                //
                bool bValueChg = false;
                assert( cacheOfs < ( ShaderParamSpec_FloatsPerElement( pSpec ) * pSpec->numElements ));

                //
                // In the case of OpenGL values, the "itemData" is the cacheLoc, so
                // doing the memcmp() is a waste of time! Therefore, we check the cacheLoc
                // vs. the itemData before trying determine if the value changed.
                // In cases where they are the same, the dirty flag must be set elsewhere.
                //
                if (( ! *isSetLoc ) ||
                    (( cacheLoc != itemData ) &&
                     ( memcmp( cacheLoc, itemData, (sizeof(float)* nCols) ) != 0 )))
                {
                    // data changed
                    SHADER_DBG_PRINTF( "  Setting dirty flag for %s[%d].\n", pSpec->szStdParamName, itemI );
                    *isDirtyLoc = true;
                    sNothingToPush = false;
                    if ( cacheLoc != itemData )
                    {
                        memcpy( cacheLoc, itemData, (sizeof(float)* nCols) );
                    }
                }
                *isSetLoc    = true;
            }
            else
            {
                //
                //    Item not being set by the caller
                //
                SHADER_DBG_PRINTF( "  Clearing 'isSet' and 'dirty' flags for %s[%d]. Data not supplied by caller.\n", pSpec->szStdParamName, itemI );
                *isSetLoc    = false;
                *isDirtyLoc = false;
            }
            
            itemData    += nCols;
            cacheLoc    += nCols;
            isSetLoc    += nCols;
            isDirtyLoc    += nCols;
            // for error checking...
            cacheOfs    += nCols;
        }
    }
}

INLINEDBG bool WCW_GetCachedParamHandleList(int paramNmCacheKey, CGparameter** ppParamHdlList, GLuint* pNumHdls )
{
    *ppParamHdlList = NULL;
    *pNumHdls = 0;

    WCW_ASSERT(( paramNmCacheKey >= 0 ) && ( paramNmCacheKey < (int)sShaderParamHandleCache.nNumUsed ));

    //
    // Cache key is a valid index. Now check to see "who" placed a param
    // handle there. If it's the current shader, we have a valid param handle.
    //
    WCW_ASSERT( sShaderParamHandleCache.cacheData[paramNmCacheKey].programHdl == rt_cgGetActiveProgram() );

    *ppParamHdlList = sShaderParamHandleCache.cacheData[paramNmCacheKey].paramHdlList;
    *pNumHdls       = sShaderParamHandleCache.cacheData[paramNmCacheKey].nListSz;

    return ( *pNumHdls > 0 );
}

void WCW_GetCgShaderParam4fv(tShaderProgramType target, GLuint n, GLfloat *vec4 /*OUT*/, bool* bDirty /*OUT*/)
{
// TBD - do we need this? I don't think. But if we do, we need to come up with a way to
// locate the proper cache table location for the ARB register the caller is asking for.
assert( 0 );
}

struct {
    int val;
    char *name;
} gl_errors[] = {
#define PAIR(x) {x, #x},
    PAIR(GL_NO_ERROR)
    PAIR(GL_INVALID_ENUM)
    PAIR(GL_INVALID_VALUE)
    PAIR(GL_INVALID_OPERATION)
    PAIR(GL_STACK_OVERFLOW)
    PAIR(GL_STACK_UNDERFLOW)
    PAIR(GL_OUT_OF_MEMORY)
    PAIR(GL_FRAMEBUFFER_COMPLETE_EXT                       )
    PAIR(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT          )
    PAIR(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT  )
    PAIR(GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT          )
    PAIR(GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT             )
    PAIR(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT         )
    PAIR(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT         )
    PAIR(GL_FRAMEBUFFER_UNSUPPORTED_EXT                    )
    PAIR(GL_INVALID_FRAMEBUFFER_OPERATION                  )
#undef PAIR
};

char *GLErrorFromInt(int v)
{
    int i;
    for (i=0; i<ARRAY_SIZE(gl_errors); i++) {
        if (gl_errors[i].val == v)
            return gl_errors[i].name;
    }
    return "Unknown";
}

//Remember, you can't put checkgl in between glBegin and glEnd
//Also, minimizing the game causes glerror to freak out, I don't know why
void checkgl(char *fname,int line)
{
    GLenum err;
    CGerror error;

#ifndef FINAL
    if (g_wcw_disable_checkgl)
    {
        return;    // has been explicitly disabled, probably for an API trace capture
    }
#endif

    if (!wglGetCurrentContext()) {
        // not inited yet
        return;
    }

    err = gldGetError();
    if ( GL_NO_ERROR != err )
    {
        char err_string[1000];
        /* ErrorCodes */
        //#define GL_INVALID_ENUM                   0x0500 (1280)
        //#define GL_INVALID_VALUE                  0x0501 (1281)
        //#define GL_INVALID_OPERATION              0x0502 (1282)
        //#define GL_STACK_OVERFLOW                 0x0503 (1283)
        //#define GL_STACK_UNDERFLOW                0x0504 (1284)
        //#define GL_OUT_OF_MEMORY                  0x0505 (1285)
        sprintf(err_string, "GL! %s:%d (0x%x) %s",fname,line,err, GLErrorFromInt(err));
        Errorf( err_string );
    }

    // Also check for Cg errors
    error = rt_cgfxGetLastCgError();
    if (error)
    {
        char err_string[1000];
        sprintf(err_string, "CG! %s:%d (0x%x) %s",fname,line,error, cgGetErrorString(error));
        Errorf( err_string );
        rt_cgfxClearLastError();
    }
}

// keep this last
#undef glGetError
GLenum gldGetError(void)
{
    GLenum error = glGetError();
    if(error && isDevelopmentOrQAMode()) {
        failText("opengl error 0x%04x: %s\n", error, GLErrorFromInt(error));
    }
    return error;
}

