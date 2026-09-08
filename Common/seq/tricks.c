#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include "seq/tricks.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include "seq/seqsequence.h"
#include <string.h>
#include <utilitieslib/assert/assert.h>
#include "seq/animtrackanimate.h"
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/SharedHeap.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/timing.h>
#include "group/groupfileload.h"
#include "seq/seqstate.h"
#include "seq/anim.h"
#include "render/thread/rt_state.h"
#include "render/rendercgfx.h"
#include "seq/AutoLOD.h"
#include "render/cubemap.h"
#include <utilitieslib/components/StringCache.h>

#if CLIENT
#include "render/tex.h"
#include "graphics/seqgraphics.h"
#include "entity/entclient.h"
#include "graphics/fog.h"
#include "clientError.h"
#include "win/win_init.h" // For winMsgAlert
#include "graphics/groupMiniTrackers.h"
#include "LWC.h"
#endif

#ifdef SERVER
#include "cmdparse/cmdserver.h"
#endif

typedef struct TexOptList
{
    TexOpt        **texopts;
    TrickInfo    **tricks;
    StashTable    texopt_name_hashes;
    StashTable    trick_name_hashes;
} TrickList;

SERVER_SHARED_MEMORY TrickList trick_list;

static StaticDefineInt tex_flags[] = {
    DEFINE_INT
    { "None",            0 },
    { "ClampS",            TEXOPT_CLAMPS },
    { "ClampT",            TEXOPT_CLAMPT },
    { "RepeatS",        TEXOPT_REPEATS },
    { "RepeatT",        TEXOPT_REPEATT },
    { "MirrorS",        TEXOPT_MIRRORS },
    { "MirrorT",        TEXOPT_MIRRORT },
    { "Cubemap",        TEXOPT_CUBEMAP },
    { "Truecolor",        TEXOPT_TRUECOLOR },
    { "Replaceable",    TEXOPT_REPLACEABLE },
    { "NoMip",            TEXOPT_NOMIP },
    { "Jpeg",            TEXOPT_JPEG },
    { "IsBumpMap",        TEXOPT_BUMPMAP },
    { "NoDither",        TEXOPT_NODITHER },
    { "NoColl",            TEXOPT_NOCOLL },
    { "SurfaceSlick",    TEXOPT_SURFACESLICK },
    { "SurfaceIcy",        TEXOPT_SURFACEICY },
    { "SurfaceBouncy",    TEXOPT_SURFACEBOUNCY },
    { "NoRandomAddGlow",    TEXOPT_NORANDOMADDGLOW },
    { "AlwaysAddGlow",        TEXOPT_ALWAYSADDGLOW },
    { "FullBright",        TEXOPT_FULLBRIGHT },
    { "Border",            TEXOPT_BORDER },
    { "OldTint",        TEXOPT_OLDTINT },
    { "PointSample",    TEXOPT_POINTSAMPLE },
    { "FallbackForceOpaque",    TEXOPT_FALLBACKFORCEOPAQUE },
    DEFINE_END
};

static StaticDefineInt stanim_flags[] = {
    DEFINE_INT
    { "FRAMESNAP",        STANIM_FRAMESNAP },
    { "PINGPONG",        STANIM_PINGPONG },
    { "LOCAL_TIMER",    STANIM_LOCAL_TIMER },
    { "GLOBAL_TIMER",    STANIM_GLOBAL_TIMER },
    DEFINE_END
};

static StaticDefineInt texblend_flags[] = {
    DEFINE_INT
    { "Multiply",        BLENDMODE_MODULATE },
    { "ColorBlendDual",    BLENDMODE_COLORBLEND_DUAL },
    { "AddGlow",        BLENDMODE_ADDGLOW },
    { "AlphaDetail",    BLENDMODE_ALPHADETAIL },
    { "SunFlare",        BLENDMODE_SUNFLARE },
    DEFINE_END
};

static StaticDefineInt trick_flags[] = {
    DEFINE_INT
    // Legacy, not good practice, or unused
    { "NoDraw",            TRICK_HIDE },
    { "SimpleAlphaSort",TRICK_SIMPLEALPHASORT },
    { "FancyWaterOffOnly",TRICK_FANCYWATEROFFONLY },
    { "LightmapsOffOnly",TRICK_LIGHTMAPSOFFONLY },

    // Use these
    { "FrontFace",        TRICK_FRONTYAW },
    { "CameraFace",        TRICK_FRONTFACE },
    { "NightLight",        TRICK_NIGHTLIGHT },
    { "NoColl",            TRICK_NOCOLL },
    { "EditorVisible",    TRICK_EDITORVISIBLE },
    { "BaseEditVisible",TRICK_BASEEDITVISIBLE },
    { "NoZTest",        TRICK_NOZTEST },
    { "NoZWrite",        TRICK_NOZWRITE },
    { "DoubleSided",    TRICK_DOUBLESIDED },
    { "Additive",        TRICK_ADDITIVE },
    { "Subtractive",    TRICK_SUBTRACTIVE },
    { "NoFog",            TRICK_NOFOG },
    { "LightFace",        TRICK_LIGHTFACE },
    { "SelectOnly",        TRICK_PLAYERSELECT },
    { "ReflectTex0",    TRICK_REFLECT },
    { "ReflectTex1",    TRICK_REFLECT_TEX1 },
    { "NoCameraCollide",TRICK_NOCAMERACOLLIDE },
    { "NotSelectable",    TRICK_NOT_SELECTABLE },
    { "AlphaCutout",    TRICK_ALPHACUTOUT },
    DEFINE_END
};

static StaticDefineInt model_flags[] = {
    DEFINE_INT
    // Legacy, not good practice, or unused
    { "ForceAlphaSort",    OBJ_ALPHASORT },
    { "ForceOpaque",    OBJ_FORCEOPAQUE },
    { "TreeDraw",        OBJ_TREEDRAW },
    { "SunFlare",        OBJ_SUNFLARE },
    
    // Use these
    { "WorldFx",        OBJ_WORLDFX },
    { "StaticFx",        OBJ_STATICFX },
    { "FullBright",        OBJ_FULLBRIGHT },
    { "NoLightAngle",    OBJ_NOLIGHTANGLE },
    { "FancyWater",        OBJ_FANCYWATER },

    { "DontCastShadowMap",    OBJ_DONTCASTSHADOWMAP },
    { "DontReceiveShadowMap",    OBJ_DONTRECEIVESHADOWMAP },
    DEFINE_END
};

static StaticDefineInt group_flags[] = {
    DEFINE_INT
    { "VisOutside",        GROUP_VIS_OUTSIDE },
    { "VisBlocker",        GROUP_VIS_BLOCKER },
    { "VisAngleBlocker",GROUP_VIS_ANGLEBLOCKER },
    { "VisWindow",        GROUP_VIS_WINDOW },
    { "VisDoorFrame",    GROUP_VIS_DOORFRAME },    //Used to get drawn/traversed in group tree even if beyond visible range?
    { "RegionMarker",    GROUP_REGION_MARKER },      //If set as nocoll still leaves it in the collision grid (for volume testing?)
    { "VisOutsideOnly",    GROUP_VIS_OUTSIDE_ONLY },
    { "SoundOccluder",    GROUP_SOUND_OCCLUDER },

    { "VisTray",        GROUP_VIS_TRAY },
    { "VolumeTrigger",    GROUP_VOLUME_TRIGGER },
    { "WaterVolume",    GROUP_WATER_VOLUME },
    { "LavaVolume",        GROUP_LAVA_VOLUME },
    { "SewerWaterVolume",GROUP_SEWERWATER_VOLUME },
    { "RedWaterVolume",    GROUP_REDWATER_VOLUME },
    { "DoorVolume",        GROUP_DOOR_VOLUME },
    { "MaterialVolume",    GROUP_MATERIAL_VOLUME },
    { "ParentFade",        GROUP_PARENT_FADE },
    { "KeyLight",        GROUP_KEY_LIGHT },
    { "StackableFloor",    GROUP_STACKABLE_FLOOR },
    { "StackableCeiling",GROUP_STACKABLE_CEILING },
    { "StackableWall",    GROUP_STACKABLE_WALL },
    DEFINE_END
};

TokenizerParseInfo parse_rgb3[] = {
    { "",                TOK_STRUCTPARAM | TOK_U8(TexOpt,scrollsScales.rgba3[0],0)},
    { "",                TOK_STRUCTPARAM | TOK_U8(TexOpt,scrollsScales.rgba3[1],0)},
    { "",                TOK_STRUCTPARAM | TOK_U8(TexOpt,scrollsScales.rgba3[2],0)},
    { "",                TOK_U8(TexOpt,scrollsScales.hasRgb34,1)},
    { "\n",                TOK_END,            0 },
    { "", 0, 0 }
};

StaticDefineInt    parse_texopt_scroll_type[] =
{
    DEFINE_INT
    { "Normal",    TEXOPTSCROLL_NORMAL},
    { "PingPong",    TEXOPTSCROLL_PINGPONG},
    { "Oval",    TEXOPTSCROLL_OVAL},
    DEFINE_END
};

TokenizerParseInfo Vec2Default1[] =
{
    { "", TOK_STRUCTPARAM    |    TOK_F32_X,    0,                1 },
    { "", TOK_STRUCTPARAM    |    TOK_F32_X,    sizeof(F32),    1 },
    { "\n",                        TOK_END },
    { "", 0, 0 }
};

TokenizerParseInfo Vec3Default1[] =
{
    { "", TOK_STRUCTPARAM    |    TOK_F32_X,    0,                1 },
    { "", TOK_STRUCTPARAM    |    TOK_F32_X,    sizeof(F32),    1 },
    { "", TOK_STRUCTPARAM    |    TOK_F32_X,    2*sizeof(F32),    1 },
    { "\n",                        TOK_END },
    { "", 0, 0 }
};

TokenizerParseInfo RGBDefault255[] =
{
    { "", TOK_STRUCTPARAM    |    TOK_U8_X,    0,                255 },
    { "", TOK_STRUCTPARAM    |    TOK_U8_X,    sizeof(U8),        255 },
    { "", TOK_STRUCTPARAM    |    TOK_U8_X,    2*sizeof(U8),    255 },
    { "\n",                        TOK_END },
    { "", 0, 0 }
};

TokenizerParseInfo parse_tex_opt_fallback[] = {
    //{ "ScaleST0",        TOK_VEC2(TexOptFallback,scaleST[1], 1)},
    //{ "ScaleST1",        TOK_VEC2(TexOptFallback,scaleST[0], 1)},
    { "ScaleST0",        TOK_EMBEDDEDSTRUCT(TexOptFallback,scaleST[1],Vec2Default1) },
    { "ScaleST1",        TOK_EMBEDDEDSTRUCT(TexOptFallback,scaleST[0],Vec2Default1) },

    { "Base",            TOK_STRING(TexOptFallback,base_name,0)},
    { "Blend",            TOK_STRING(TexOptFallback,blend_name,0)},
    { "BumpMap",        TOK_STRING(TexOptFallback,bumpmap,0)},
    { "BlendType",        TOK_INT(TexOptFallback,blend_mode,0),texblend_flags},
    { "UseFallback",    TOK_U8(TexOptFallback,useFallback,0)},

    { "AmbientScale",        TOK_REDUNDANTNAME|TOK_F32(TexOptFallback,ambientScaleTrick[0], 0)},
    { "DiffuseScale",        TOK_REDUNDANTNAME|TOK_F32(TexOptFallback,diffuseScaleTrick[0], 0)},
    { "AmbientMin",            TOK_REDUNDANTNAME|TOK_F32(TexOptFallback,ambientMin[0], 0)},
    //{ "DiffuseScaleVec",    TOK_VEC3(TexOptFallback,diffuseScaleTrick, 1)},
    //{ "AmbientScaleVec",    TOK_VEC3(TexOptFallback,ambientScaleTrick, 1)},
    { "DiffuseScaleVec",    TOK_EMBEDDEDSTRUCT(TexOptFallback,diffuseScaleTrick,Vec3Default1)},
    { "AmbientScaleVec",    TOK_EMBEDDEDSTRUCT(TexOptFallback,ambientScaleTrick,Vec3Default1)},
    { "AmbientMinVec",        TOK_VEC3(TexOptFallback,ambientMin)},

    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

TokenizerParseInfo parse_name_value_set[] = {
    { "",                TOK_STRUCTPARAM|TOK_STRING(CgFxParamVal,paramName,0)},
    { "",                TOK_STRUCTPARAM|TOK_F32(CgFxParamVal,vals[0],0)   },
    { "",                TOK_STRUCTPARAM|TOK_F32(CgFxParamVal,vals[1],0)   },
    { "",                TOK_STRUCTPARAM|TOK_F32(CgFxParamVal,vals[2],0)   },
    { "",                TOK_STRUCTPARAM|TOK_F32(CgFxParamVal,vals[3],0)   },
    { "\n",                TOK_END },
    { "", 0, 0 }
};

TokenizerParseInfo parse_tex_opt[] = {
    { "",                TOK_STRUCTPARAM | TOK_STRING(TexOpt,name,0)},
    { "F",                TOK_CURRENTFILE(TexOpt,file_name)},
    { "TS",                TOK_TIMESTAMP(TexOpt,fileAge)},
    { "InternalName",    TOK_STRING(TexOpt,name,0)},
    { "Gloss",            TOK_F32(TexOpt,gloss,1.f)   },
    { "Surface",        TOK_STRING(TexOpt,surface_name,0) },
    { "Fade",            TOK_VEC2(TexOpt,texopt_fade) },
    { "ScaleST0",        TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_OLD_TEX_GENERIC_BLEND],Vec2Default1)}, // JE: These two, apparently, have always been backwards.
    { "ScaleST1",        TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_OLD_TEX_BASE],Vec2Default1)},
    { "Blend",            TOK_STRING(TexOpt,blend_names[BLEND_OLD_TEX_GENERIC_BLEND],0)},
    { "BumpMap",        TOK_STRING(TexOpt,blend_names[BLEND_BUMPMAP1],0)},
    { "BlendType",        TOK_INT(TexOpt,blend_mode,0),texblend_flags},
    { "Flags",            TOK_FLAGS(TexOpt,flags,0),tex_flags},
    { "ObjFlags",        TOK_FLAGS(TexOpt,model_flags,0),model_flags},
    { "DF_ObjName",     TOK_STRING(TexOpt,df_name,0) },

    // New Texture blending definitions
    { "Base1",                TOK_STRING(TexOpt,blend_names[BLEND_BASE1],0)},
    { "Base1Scale",            TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_BASE1],Vec2Default1)},
    { "Base1Scroll",        TOK_VEC2(TexOpt,scrollsScales.texopt_scroll[BLEND_BASE1])},
    { "Base1ScrollType",    TOK_INT(TexOpt,scrollsScales.texopt_scrollType[BLEND_BASE1],TEXOPTSCROLL_NORMAL), parse_texopt_scroll_type},
    { "Base1Swappable",        TOK_U8(TexOpt,swappable[BLEND_BASE1],0) },

    { "Multiply1",            TOK_STRING(TexOpt,blend_names[BLEND_MULTIPLY1],0)},
    { "Multiply1Scale",        TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_MULTIPLY1],Vec2Default1)},
    { "Multiply1Scroll",    TOK_VEC2(TexOpt,scrollsScales.texopt_scroll[BLEND_MULTIPLY1])},
    { "Multiply1ScrollType",TOK_INT(TexOpt,scrollsScales.texopt_scrollType[BLEND_MULTIPLY1], TEXOPTSCROLL_NORMAL), parse_texopt_scroll_type},
    { "Multiply1Swappable",    TOK_U8(TexOpt,swappable[BLEND_MULTIPLY1],0) },

    { "DualColor1",            TOK_STRING(TexOpt,blend_names[BLEND_DUALCOLOR1],0)},
    { "DualColor1Scale",    TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_DUALCOLOR1],Vec2Default1)},
    { "DualColor1Scroll",    TOK_VEC2(TexOpt,scrollsScales.texopt_scroll[BLEND_DUALCOLOR1])},
    { "DualColor1ScrollType",TOK_INT(TexOpt,scrollsScales.texopt_scrollType[BLEND_DUALCOLOR1], TEXOPTSCROLL_NORMAL), parse_texopt_scroll_type},
    { "DualColor1Swappable",TOK_U8(TexOpt,swappable[BLEND_DUALCOLOR1],0) },

    { "AddGlow1",            TOK_STRING(TexOpt,blend_names[BLEND_ADDGLOW1],0)},
    { "AddGlow1Scale",        TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_ADDGLOW1],Vec2Default1)},
    { "AddGlow1Scroll",        TOK_VEC2(TexOpt,scrollsScales.texopt_scroll[BLEND_ADDGLOW1])},
    { "AddGlow1ScrollType",    TOK_INT(TexOpt,scrollsScales.texopt_scrollType[BLEND_ADDGLOW1], TEXOPTSCROLL_NORMAL), parse_texopt_scroll_type},
    { "AddGlow1Swappable",    TOK_U8(TexOpt,swappable[BLEND_ADDGLOW1],0) },

    { "BumpMap1",            TOK_STRING(TexOpt,blend_names[BLEND_BUMPMAP1],0)},
    { "BumpMap1Scale",        TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_BUMPMAP1],Vec2Default1)},
    { "BumpMap1Scroll",        TOK_VEC2(TexOpt,scrollsScales.texopt_scroll[BLEND_BUMPMAP1])},
    { "BumpMap1ScrollType",    TOK_INT(TexOpt,scrollsScales.texopt_scrollType[BLEND_BUMPMAP1], TEXOPTSCROLL_NORMAL), parse_texopt_scroll_type},
    { "BumpMap1Swappable",    TOK_U8(TexOpt,swappable[BLEND_BUMPMAP1],0) },

    { "Mask",                TOK_STRING(TexOpt,blend_names[BLEND_MASK],0)},
    { "MaskScale",            TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_MASK],Vec2Default1)},
    { "MaskScroll",            TOK_VEC2(TexOpt,scrollsScales.texopt_scroll[BLEND_MASK])},
    { "MaskScrollType",        TOK_INT(TexOpt,scrollsScales.texopt_scrollType[BLEND_MASK], TEXOPTSCROLL_NORMAL), parse_texopt_scroll_type},
    { "MaskSwappable",        TOK_U8(TexOpt,swappable[BLEND_MASK],0) },

    { "Base2",                TOK_STRING(TexOpt,blend_names[BLEND_BASE2],0)},
    { "Base2Scale",            TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_BASE2],Vec2Default1)},
    { "Base2Scroll",        TOK_VEC2(TexOpt,scrollsScales.texopt_scroll[BLEND_BASE2])},
    { "Base2ScrollType",    TOK_INT(TexOpt,scrollsScales.texopt_scrollType[BLEND_BASE2], TEXOPTSCROLL_NORMAL), parse_texopt_scroll_type},
    { "Base2Swappable",        TOK_U8(TexOpt,swappable[BLEND_BASE2],0) },

    { "Multiply2",            TOK_STRING(TexOpt,blend_names[BLEND_MULTIPLY2],0)},
    { "Multiply2Scale",        TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_MULTIPLY2],Vec2Default1)},
    { "Multiply2Scroll",    TOK_VEC2(TexOpt,scrollsScales.texopt_scroll[BLEND_MULTIPLY2])},
    { "Multiply2ScrollType",TOK_INT(TexOpt,scrollsScales.texopt_scrollType[BLEND_MULTIPLY2], TEXOPTSCROLL_NORMAL), parse_texopt_scroll_type},
    { "Multiply2Swappable",    TOK_U8(TexOpt,swappable[BLEND_MULTIPLY2],0) },

    { "DualColor2",            TOK_STRING(TexOpt,blend_names[BLEND_DUALCOLOR2],0)},
    { "DualColor2Scale",    TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_DUALCOLOR2],Vec2Default1)},
    { "DualColor2Scroll",    TOK_VEC2(TexOpt,scrollsScales.texopt_scroll[BLEND_DUALCOLOR2])},
    { "DualColor2ScrollType",TOK_INT(TexOpt,scrollsScales.texopt_scrollType[BLEND_DUALCOLOR2], TEXOPTSCROLL_NORMAL), parse_texopt_scroll_type},
    { "DualColor2Swappable",TOK_U8(TexOpt,swappable[BLEND_DUALCOLOR2],0) },

    { "BumpMap2",            TOK_STRING(TexOpt,blend_names[BLEND_BUMPMAP2],0)},
    { "BumpMap2Scale",        TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_BUMPMAP2],Vec2Default1)},
    { "BumpMap2Scroll",        TOK_VEC2(TexOpt,scrollsScales.texopt_scroll[BLEND_BUMPMAP2])},
    { "BumpMap2ScrollType",    TOK_INT(TexOpt,scrollsScales.texopt_scrollType[BLEND_BUMPMAP2], TEXOPTSCROLL_NORMAL), parse_texopt_scroll_type},
    { "BumpMap2Swappable",    TOK_U8(TexOpt,swappable[BLEND_BUMPMAP2],0) },

    { "CubeMap",            TOK_STRING(TexOpt,blend_names[BLEND_CUBEMAP],0)},
    { "CubeMapScale",        TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.texopt_scale[BLEND_CUBEMAP],Vec2Default1)},
    { "CubeMapScroll",        TOK_VEC2(TexOpt,scrollsScales.texopt_scroll[BLEND_CUBEMAP])},
    { "CubeMapScrollType",    TOK_INT(TexOpt,scrollsScales.texopt_scrollType[BLEND_CUBEMAP], TEXOPTSCROLL_NORMAL), parse_texopt_scroll_type},
    { "CubeMapSwappable",    TOK_U8(TexOpt,swappable[BLEND_CUBEMAP],0) },

    { "Color3",                TOK_NULLSTRUCT(parse_rgb3) }, // Using embedded struct to flag
    { "Color4",                TOK_FIXED_ARRAY | TOK_U8_X, offsetof(TexOpt,scrollsScales.rgba4),    3},
    { "SpecularColor",        TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.specularRgba1,RGBDefault255) },
    { "SpecularColor1",        TOK_REDUNDANTNAME|TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.specularRgba1,RGBDefault255)},
    { "SpecularColor2",        TOK_FIXED_ARRAY | TOK_U8_X, offsetof(TexOpt,scrollsScales.specularRgba2), 3 },
    { "SpecularExponent",    TOK_F32(TexOpt,specularExponent1_parser,8)},
    { "SpecularExponent1",    TOK_REDUNDANTNAME|TOK_F32(TexOpt,specularExponent1_parser,8)},
    { "SpecularExponent2",    TOK_F32(TexOpt,specularExponent2_parser,0)},
    { "Reflectivity",        TOK_F32(TexOpt,scrollsScales.reflectivity,0)}, // deprecated, to be removed
    { "ReflectivityBase",    TOK_F32(TexOpt,scrollsScales.reflectivityBase,-1.0f)},
    { "ReflectivityScale",    TOK_F32(TexOpt,scrollsScales.reflectivityScale,-1.0f)},
    { "ReflectivityPower",    TOK_F32(TexOpt,scrollsScales.reflectivityPower,-1.0f)},

    { "AlphaMask",            TOK_U8(TexOpt,scrollsScales.alphaMask,0)},
    { "MaskWeight",            TOK_F32(TexOpt,scrollsScales.maskWeight, 1)},
    { "Multiply1Reflect",    TOK_U8(TexOpt,scrollsScales.multiply1Reflect,0)},
    { "Multiply2Reflect",    TOK_U8(TexOpt,scrollsScales.multiply2Reflect,0)},
    { "BaseAddGlow",        TOK_U8(TexOpt,scrollsScales.baseAddGlow, 0)},
    { "MinAddGlow",            TOK_U8(TexOpt,scrollsScales.minAddGlow, 0)},
    { "MaxAddGlow",            TOK_U8(TexOpt,scrollsScales.maxAddGlow, 128)},

    { "AddGlowMat2",        TOK_U8(TexOpt,scrollsScales.addGlowMat2, 0)},
    { "AddGlowTint",        TOK_U8(TexOpt,scrollsScales.tintGlow, 0)},
    { "ReflectionTint",        TOK_U8(TexOpt,scrollsScales.tintReflection, 0)},
    { "ReflectionDesaturate",TOK_U8(TexOpt,scrollsScales.desaturateReflection, 0)},
    { "AlphaWater",            TOK_U8(TexOpt,scrollsScales.alphaWater, 0)},

    { "AmbientScale",        TOK_REDUNDANTNAME|TOK_F32(TexOpt,scrollsScales.ambientScaleTrick[0], 0)},
    { "DiffuseScale",        TOK_REDUNDANTNAME|TOK_F32(TexOpt,scrollsScales.diffuseScaleTrick[0], 0)},
    { "AmbientMin",            TOK_REDUNDANTNAME|TOK_F32(TexOpt,scrollsScales.ambientMin[0], 0)},
    { "DiffuseScaleVec",    TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.diffuseScaleTrick,Vec3Default1)},
    { "AmbientScaleVec",    TOK_EMBEDDEDSTRUCT(TexOpt,scrollsScales.ambientScaleTrick,Vec3Default1)},
    { "AmbientMinVec",        TOK_VEC3(TexOpt,scrollsScales.ambientMin)},

    { "Fallback",            TOK_EMBEDDEDSTRUCT(TexOpt,fallback,parse_tex_opt_fallback) },

    { "End",                TOK_END,            0},
    { "", 0, 0 }
};

TokenizerParseInfo parse_st_anim[] = {
    { "",                TOK_STRUCTPARAM | TOK_F32(StAnim,speed_scale,0)},
    { "",                TOK_STRUCTPARAM | TOK_F32(StAnim,st_scale,0)},
    { "",                TOK_STRUCTPARAM | TOK_STRING(StAnim,name,0)},
    { "",                TOK_STRUCTPARAM | TOK_FLAGS(StAnim,flags,0),stanim_flags},
    { "\n",                TOK_END,            0 },
    { "", 0, 0 }
};

TokenizerParseInfo parse_trick[] = {
    { "",                TOK_STRUCTPARAM | TOK_STRING(TrickInfo,name,0)},
    { "",                TOK_CURRENTFILE(TrickInfo,file_name)},
    { "",                TOK_TIMESTAMP(TrickInfo,fileAge)},
    { "LodFar",            TOK_F32(TrickInfo,lod_far,0)   },
    { "LodFarFade",        TOK_F32(TrickInfo,lod_farfade,0)   },
    { "LodNear",        TOK_F32(TrickInfo,lod_near,0)   },
    { "LodNearFade",    TOK_F32(TrickInfo,lod_nearfade,0)   },

    { "TrickFlags",        TOK_FLAGS(TrickInfo,tnode.flags1,0),trick_flags   },
    { "ObjFlags",        TOK_FLAGS(TrickInfo,model_flags,0),model_flags   },
    { "GroupFlags",        TOK_FLAGS(TrickInfo,group_flags,0),group_flags   },

    { "Sway",            TOK_VEC2(TrickInfo,sway)   },
    { "SwayRandomize",    TOK_VEC2(TrickInfo,sway_random)   },
    { "Rotate",            TOK_REDUNDANTNAME | TOK_F32(TrickInfo,sway[0],0)   },
    { "RotateRandomize",TOK_REDUNDANTNAME | TOK_F32(TrickInfo,sway_random[0],0)   },
    { "SwayPitch",        TOK_VEC2(TrickInfo,sway_pitch)   },
    { "SwayRoll",        TOK_VEC2(TrickInfo,sway_roll)   },
    { "AlphaRef",        TOK_F32(TrickInfo,alpha_ref_parser,0)   },
    { "SortBias",        TOK_F32(TrickInfo,alpha_sort_mod,0)   },
    { "WaterReflectionSkew",        TOK_F32(TrickInfo,water_reflection_skew,30)   },
    { "WaterReflectionStrength",    TOK_F32(TrickInfo,water_reflection_strength,60)   },
    { "ScrollST0",        TOK_FIXED_ARRAY | TOK_F32_X, offsetof(TrickInfo,texScrollAmt[0]),    2   },
    { "ScrollST1",        TOK_FIXED_ARRAY | TOK_F32_X, offsetof(TrickInfo,texScrollAmt[1]),    2   },
    { "ShadowDist",        TOK_F32(TrickInfo,shadow_dist,0)   },
    { "NightGlow",        TOK_VEC2(TrickInfo,nightglow_times)   },
    { "TintColor0",        TOK_FIXED_ARRAY | TOK_U8_X, offsetof(TrickInfo,tnode.trick_rgba),    3   },
    { "TintColor1",        TOK_FIXED_ARRAY | TOK_U8_X, offsetof(TrickInfo,tnode.trick_rgba2),  3  },
    { "ObjTexBias",        TOK_F32(TrickInfo,tex_bias,0)    },
    { "CameraFaceTightenUp",TOK_F32(TrickInfo,tighten_up,0)   },

    { "StAnim",            TOK_STRUCT(TrickInfo,stAnim,parse_st_anim) },

    { "AutoLOD",        TOK_STRUCT(TrickInfo,auto_lod,parse_auto_lod) },

    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

TokenizerParseInfo parse_trick_list[] = {
    { "Texture",        TOK_STRUCT(TrickList,texopts,parse_tex_opt) },
    { "Trick",            TOK_STRUCT(TrickList,tricks,parse_trick) },
    { "", 0, 0 }
};

TexLayerIndex blendIndexToTexLayerIndex(BlendIndex bi)
{
    if (bi==BLEND_OLD_TEX_GENERIC_BLEND)
        return TEXLAYER_GENERIC;
    if (bi==BLEND_OLD_TEX_BASE)
        return TEXLAYER_BASE;
    return bi - 2;
}
BlendIndex texLayerIndexToBlendIndex(TexLayerIndex tli)
{
    return tli + 2;
}


void trickRecreateTextures(void);

TexOptFlags texOptFlagsFromName(const char *cName, TexOptFlags origFlags)
{
    TexOptFlags flags=origFlags;
    char namebuf[MAX_PATH];
    char* name;
    strcpy(namebuf, cName);
    name = strrchr(namebuf, '.');
    if (name)
        *name = 0;
    name = namebuf;
    if (strstriConst(name,"PLAYERS/") || strstriConst(name,"ENEMIES/") || strstriConst(name,"NPCS/"))
        flags |= TEXOPT_CLAMPS|TEXOPT_CLAMPT;
    if (strEndsWith(name,"_bump"))
        flags |= TEXOPT_BUMPMAP;

// TODO - Add "_nm" to this list??
    if( strEndsWith(name, "_ns") || strEndsWith(name, "_n") || strEndsWith(name,"_normal") ) {
        flags |= TEXOPT_NODITHER;    // fpe 9/30/09 -- don't dither normal maps.  
        flags |= TEXOPT_NORMALMAP;
        // TODO: Do we want to do this?
        //        Virtually all normal maps are currently authored with
        //        specular in the alpgha channel, even if they aren't
        //        named "_ns.tga", so...
        flags |= TEXOPT_SPECINALPHA;
    }

    // fpe 12/2/09 -- implicity assignment of cubemap flag, since we will have alot of these auto-generated, don't require
    //    that artists create a trick for each one, instead imply by naming convention.  Only the first face (which must end
    //    with 0.tga) gets assigned the cubemap flag.
    // 06/29/10 -- allow all cubemap faces to have the TEXOPT_CUBEMAP flag set.
    //  Need this to update unloadCriteria() to keep it from unloading non-0 faces.
    if( strstriConst(name, "cubemap") ) {
        char lastChar = name[strlen(name)-1];
        if(lastChar >= '0' && lastChar <= '5')
            flags |= TEXOPT_CUBEMAP;
    }

    if (origFlags & TEXOPT_REPEATS) // Explicit repeat to override default clamp
        flags &= ~TEXOPT_CLAMPS;
    if (origFlags & TEXOPT_REPEATT)
        flags &= ~TEXOPT_CLAMPT;
    return flags;
}

char const* blendIndexToName(BlendIndex bi)
{
    int i;
    for (i=0; i<ARRAY_SIZE(parse_tex_opt); i++) {
        if (parse_tex_opt[i].storeoffset == offsetof(TexOpt,blend_names[bi])) {
            return parse_tex_opt[i].name;
        }
    }
    return "UNKNOWN";
}

TexOpt *trickFromTextureName(const char *name, TexOptFlags *texopt_flags)
{
    char    buf[1200],*s,*s2,*slash;
    TexOpt    *tex;

    strcpy(buf,name);
    forwardSlashes(buf);
    slash = strrchr(buf,'/');
    if (!slash++)
        slash = buf;

    // Remove extension
    s = strrchr(buf,'.');
    if (s && s > slash)
        *s = 0;
    // Remove texture locale
    s = strrchr(buf, '#');
    if (s)
        *s = 0;

    // look for direct match
    stashFindPointer( trick_list.texopt_name_hashes,slash, &tex );
    if (tex) {
        if (texopt_flags)
            *texopt_flags = texOptFlagsFromName(name, tex->flags);
        return tex;
    }

    // look for directory match
    strcpy(buf,name);
    forwardSlashes(buf);
    s = strstri(buf,"texture_library");
    if (!s) {
        s = buf;
    } else {
        s += strlen("texture_library");
    }
    if (s[0]=='/') s++;

    for(;;)
    {
        s[strlen(s)-1] = 0;
        s2 = strrchr(s,'/');
        if (!s2)
            break;
        s2[1] = 0;

        stashFindPointer( trick_list.texopt_name_hashes,s, &tex );
        if (tex) {
            if (texopt_flags)
                *texopt_flags = texOptFlagsFromName(name, tex->flags);
            return tex;
        }
    }
    if (texopt_flags)
        *texopt_flags = texOptFlagsFromName(name, 0);
    return 0;
}

static int trick_report_exist_errors = 1;
void trickSetExistenceErrorReporting(int report_errors)
{
    trick_report_exist_errors = report_errors;
}

/*Gived the name of a model, return a TrickInfo the matching model's __tricknamesuffix.
Return zero if no suffix or no matching loaded TrickInfo.
*/
TrickInfo *trickFromObjectName(const char *name, const char *fullpath)
{
    char        *s;
    TrickInfo    *trick;
    char        dirname[MAX_PATH];

    assert(trick_list.trick_name_hashes);

    s = strstr(name,"__");
    if (s) {
        // Has an __
        stashFindPointer( trick_list.trick_name_hashes,s+2, &trick );
        if (trick || !trick_report_exist_errors)
            return trick;
    } else {
        // Look for trick referencing object name
        stashFindPointer( trick_list.trick_name_hashes, name, &trick );
        return trick;
    }
    strcpy(dirname, fullpath);
    getDirectoryName(dirname);
    if (fullpath && fullpath[0])
        ErrorFilenamef(fullpath, "Can't find trick for %s/%s",dirname,name);
    else
        Errorf("Can't find trick for %s", name);
    return 0;
}

TrickInfo *trickFromName(const char *name, char *fullpath)
{
    TrickInfo    *trick;

    assert(trick_list.trick_name_hashes);
    stashFindPointer( trick_list.trick_name_hashes,name, &trick );
    if (trick)
        return trick;
    //Errorf("Can't find trick %s for %s", name, fullpath ? fullpath : "no path given");
    return 0;
}

static bool texOptNeedsMultiTex(TexOpt *tex)
{
    int i;
    if (// tex->blend_names[BLEND_BASE1] ||
        tex->blend_names[BLEND_MULTIPLY1] ||
        //tex->blend_names[BLEND_DUALCOLOR1] || // DualColor with bumpmap can still use old texture blending mode
        //tex->blend_names[BLEND_BUMPMAP1] || // Single bumpmap still gets old texture blending mode
        tex->blend_names[BLEND_ADDGLOW1] ||
        tex->blend_names[BLEND_MASK] ||
        tex->blend_names[BLEND_BASE2] ||
        tex->blend_names[BLEND_MULTIPLY2] ||
        tex->blend_names[BLEND_DUALCOLOR2] ||
        tex->blend_names[BLEND_BUMPMAP2])
        return true;
    for (i=0; i<BLEND_NUM_BLEND_NAMES; i++) 
        if ((i>1 && (tex->scrollsScales.texopt_scale[i][0]!=1 || // Scale on old-style (0 and 1) are fine
            tex->scrollsScales.texopt_scale[i][1]!=1)) ||
            tex->scrollsScales.texopt_scroll[i][0]!=0 ||
            tex->scrollsScales.texopt_scroll[i][1]!=0)
            return true;
    return false;
}

#define SAFE_STRUCT_FREE_STRING_CONST(_x) do { if ((_x) && !shared_memory) StructFreeStringConst((_x)); if (_x) onlyfulldebugdevassert(shared_memory == isSharedMemory((_x))); (_x) = NULL; } while(0,0)
static void setupTexOpt(TexOpt *tex, bool shared_memory)
{
    int        i;
    TexOpt    *dup_tex;

    assert( tex );
    tex->flags &= ~(TEXOPT_MULTITEX|TEXOPT_FADE|TEXOPT_TREAT_AS_MULTITEX); // Reset flags set only in this function

    tex->scrollsScales.debug_backpointer = tex;

    if(!tex->name)
        return;

    if (tex->texopt_fade[0] || tex->texopt_fade[1])
        tex->flags |= TEXOPT_FADE;

    if (tex->flags & TEXOPT_NORANDOMADDGLOW) {
        tex->scrollsScales.minAddGlow = 255;
        tex->scrollsScales.maxAddGlow = 255;
    }
    if (tex->flags & TEXOPT_ALWAYSADDGLOW) {
        tex->scrollsScales.baseAddGlow = 255;
        tex->scrollsScales.minAddGlow = 255;
        tex->scrollsScales.maxAddGlow = 255;
    }

    if (tex->scrollsScales.hasRgb34) {
        if (!tex->scrollsScales.rgba3[0] && !tex->scrollsScales.rgba3[1] && !tex->scrollsScales.rgba3[2]) {
            tex->scrollsScales.hasRgb34 = false;
        }
    }

    if (tex->scrollsScales.diffuseScaleTrick[1] == 1 && tex->scrollsScales.diffuseScaleTrick[2] == 1)
        tex->scrollsScales.diffuseScaleTrick[1] = tex->scrollsScales.diffuseScaleTrick[2] = tex->scrollsScales.diffuseScaleTrick[0];
    if (tex->scrollsScales.ambientScaleTrick[1] == 1 && tex->scrollsScales.ambientScaleTrick[2] == 1)
        tex->scrollsScales.ambientScaleTrick[1] = tex->scrollsScales.ambientScaleTrick[2] = tex->scrollsScales.ambientScaleTrick[0];
    if (tex->scrollsScales.ambientMin[1] == 0 && tex->scrollsScales.ambientMin[2] == 0)
        tex->scrollsScales.ambientMin[1] = tex->scrollsScales.ambientMin[2] = tex->scrollsScales.ambientMin[0];
    if (tex->fallback.diffuseScaleTrick[1] == 1 && tex->fallback.diffuseScaleTrick[2] == 1)
        tex->fallback.diffuseScaleTrick[1] = tex->fallback.diffuseScaleTrick[2] = tex->fallback.diffuseScaleTrick[0];
    if (tex->fallback.ambientScaleTrick[1] == 1 && tex->fallback.ambientScaleTrick[2] == 1)
        tex->fallback.ambientScaleTrick[1] = tex->fallback.ambientScaleTrick[2] = tex->fallback.ambientScaleTrick[0];
    if (tex->fallback.ambientMin[1] == 0 && tex->fallback.ambientMin[2] == 0)
        tex->fallback.ambientMin[1] = tex->fallback.ambientMin[2] = tex->fallback.ambientMin[0];
    if (tex->flags & TEXOPT_FULLBRIGHT) {
        if (0) {
            // per-material fullbright - doesn't work on indoor lighting
            setVec3(tex->scrollsScales.diffuseScaleTrick, 0, 0, 0);
            setVec3(tex->scrollsScales.ambientMin, 1, 1, 1);
        } else {
            // Make the whole object fullbright - doesn't work on multi-sub
            tex->model_flags |= OBJ_FULLBRIGHT;
        }
    }

    for (i=0; i<BLEND_NUM_BLEND_NAMES; i++) {
        if (tex->blend_names[i]) {
            // note: textures are not yet loaded at this point
            if (strStartsWith(tex->blend_names[i], "texture_name") || !stricmp(tex->blend_names[i], "none"))
                SAFE_STRUCT_FREE_STRING_CONST(tex->blend_names[i]);
            else if (stricmp(tex->blend_names[i], "SWAPPABLE")==0)
                tex->swappable[i] = 1;
        }
    }

    // Verify OldTint -> no DualColor texture
    if (tex->flags & TEXOPT_OLDTINT && tex->blend_names[BLEND_DUALCOLOR1]) {
        ErrorFilenamef(tex->file_name, "Trick %s is flagged as \"OldTint\", but has a DualColor texture (%s).", tex->name, tex->blend_names[BLEND_DUALCOLOR1]);
    }

    // Clean up silliness
    if ((!tex->blend_names[BLEND_MASK] || stricmp(tex->blend_names[BLEND_MASK], "white")==0 || strStartsWith(tex->blend_names[BLEND_MASK], "white."))
        && !(tex->model_flags & OBJ_FANCYWATER))
    {
        // No mask
        SAFE_STRUCT_FREE_STRING_CONST(tex->blend_names[BLEND_MASK]);
        SAFE_STRUCT_FREE_STRING_CONST(tex->blend_names[BLEND_BASE2]);
        SAFE_STRUCT_FREE_STRING_CONST(tex->blend_names[BLEND_MULTIPLY2]);
        SAFE_STRUCT_FREE_STRING_CONST(tex->blend_names[BLEND_BUMPMAP2]);
        SAFE_STRUCT_FREE_STRING_CONST(tex->blend_names[BLEND_DUALCOLOR2]);
    }
    // Clean up scales/scrolls on irrelevant textures
    for (i=0; i<BLEND_NUM_BLEND_NAMES; i++) {
        if (!tex->blend_names[i]) {
            tex->scrollsScales.texopt_scale[i][0] = tex->scrollsScales.texopt_scale[i][1] = 1;
            tex->scrollsScales.texopt_scroll[i][0] = tex->scrollsScales.texopt_scroll[i][1] = 0;
        }
    }


    // Determine texture type
    if (tex->blend_names[BLEND_OLD_TEX_GENERIC_BLEND]) {
        // Old style dual texture
        if (!tex->blend_names[BLEND_BASE1]) {
            SAFE_STRUCT_FREE_STRING_CONST(tex->blend_names[BLEND_BASE1]);
#if SERVER
            tex->blend_names[BLEND_BASE1] = NULL; // Can't set it to anything, otherwise either it won't free or it won't work with shared memory
#else
            // Warning: this isn't friendly with shared memory, but the server doesn't look at texture tricks?
            tex->blend_names[BLEND_BASE1] = shared_memory ? allocAddSharedString(tex->name) : ParserAllocString(tex->name);
#endif
        }
    } else if (texOptNeedsMultiTex(tex)) {
        tex->flags |= TEXOPT_MULTITEX|TEXOPT_TREAT_AS_MULTITEX;
    } else {
        if (tex->blend_names[BLEND_DUALCOLOR1]) {
            // Convert a skeleton of a multi-tex into an old-style texture
            SAFE_STRUCT_FREE_STRING_CONST(tex->blend_names[BLEND_OLD_TEX_GENERIC_BLEND]);
            tex->blend_names[BLEND_OLD_TEX_GENERIC_BLEND] = tex->blend_names[BLEND_DUALCOLOR1];
            tex->blend_names[BLEND_DUALCOLOR1] = NULL;

            tex->blend_mode = BLENDMODE_COLORBLEND_DUAL;
            if (tex->blend_names[BLEND_BUMPMAP1]) {
                tex->blend_mode = BLENDMODE_BUMPMAP_COLORBLEND_DUAL;
            } else {
                // JE: These values should probably be 0.5, 1.0 instead, but things are already tweaked for these values :(
                scaleVec3(tex->scrollsScales.ambientScaleTrick, 0.25, tex->scrollsScales.ambientScaleTrick);
                scaleVec3(tex->scrollsScales.diffuseScaleTrick, 0.5, tex->scrollsScales.diffuseScaleTrick);
            }
            tex->flags |= TEXOPT_TREAT_AS_MULTITEX;
        } else if (tex->blend_names[BLEND_BASE1]) {
            // Explicit base specification, assume this is a new shader and use at least ColorBlendDual stuff
            SAFE_STRUCT_FREE_STRING_CONST(tex->blend_names[BLEND_OLD_TEX_GENERIC_BLEND]);
            tex->blend_names[BLEND_OLD_TEX_GENERIC_BLEND] = shared_memory ? allocAddSharedString("white") : ParserAllocString("white");
            tex->blend_mode = BLENDMODE_COLORBLEND_DUAL;
            if (tex->blend_names[BLEND_BUMPMAP1]) {
                tex->blend_mode = BLENDMODE_BUMPMAP_COLORBLEND_DUAL;
            } else {
                scaleVec3(tex->scrollsScales.ambientScaleTrick, 0.25, tex->scrollsScales.ambientScaleTrick);
                scaleVec3(tex->scrollsScales.diffuseScaleTrick, 0.5, tex->scrollsScales.diffuseScaleTrick);
            }
            tex->flags |= TEXOPT_TREAT_AS_MULTITEX;
        }

        // Doing this causes textures to be reloaded as dualcolorblend textures... I think the original need for these lines was fixed somewhere else
        //if (!tex->blend_names[BLEND_BASE1]) {
        //    // Warning: this isn't friendly with shared memory, but the server doesn't look at texture tricks?
        //    tex->blend_names[BLEND_BASE1] = shared_memory ? allocAddSharedString(tex->name) : ParserAllocString(tex->name);
        //}
    }

#if SERVER || CLIENT
    if (tex->surface_name)
    {
        int surface;
        surface = seqGetStateNumberFromName(tex->surface_name);
        if( surface < 0 )  //default surface should be zero, not -1
            surface = 0;
        if( surface > 255 )
        {
            //coll->surf is a U8, but STATE_BITS can go over 256, so we need to be careful not to have any surface bits over 255 in seq_states. (Just move them around)
            Errorf( "Surface bit %s added that's greater than 256.  Have a programmer fix this oddity in seq_states", tex->surface_name );
            surface = 0;
        }
        tex->surface = surface;
    }
#endif // SERVER || CLIENT

    if (tex->scrollsScales.specularRgba2[0] == 0 &&
        tex->scrollsScales.specularRgba2[1] == 0 &&
        tex->scrollsScales.specularRgba2[2] == 0)
    {
        copyVec3(tex->scrollsScales.specularRgba1, tex->scrollsScales.specularRgba2);
    }

    tex->scrollsScales.specularExponent1 = tex->specularExponent1_parser*2;
    tex->scrollsScales.specularExponent2 = tex->specularExponent2_parser*2;

    if (tex->scrollsScales.specularExponent2 == 0)
        tex->scrollsScales.specularExponent2 = tex->scrollsScales.specularExponent1;

    tex->flags = texOptFlagsFromName(tex->name, tex->flags); // add TEXOPT_BUMPMAP
    
    if (!stashAddPointer(trick_list.texopt_name_hashes,tex->name,tex, false))
    {
        stashFindPointer( trick_list.texopt_name_hashes,tex->name, &dup_tex );
        ErrorFilenameDup(tex->file_name, dup_tex->file_name, tex->name, "Texture Trick");
    }
}

static void setupTrick(TrickInfo *trick, bool shared_memory)
{
    TrickInfo    *dup_trick;
    U8            *c;

    trick->tnode.info = trick;

    c = trick->tnode.trick_rgba;
    if (!(c[0]|c[1]|c[2]))
        memset(trick->tnode.trick_rgba,255,4);
    c = trick->tnode.trick_rgba2;
    if (!(c[0]|c[1]|c[2]))
        memset(trick->tnode.trick_rgba2,255,4);
    trick->alpha_ref = trick->alpha_ref_parser / 255.f;
    if (trick->tex_bias)
        trick->tnode.flags2 |= TRICK2_TEXBIAS;
    if (trick->alpha_ref)
        trick->tnode.flags2 |= TRICK2_ALPHAREF;
    if (trick->shadow_dist)
        trick->tnode.flags2 |= TRICK2_CASTSHADOW;
    if (trick->nightglow_times[0] || trick->nightglow_times[1])
        trick->tnode.flags2 |= TRICK2_NIGHTGLOW;
    if (trick->sway[0] || trick->sway_pitch[0] || trick->sway_roll[0])
        trick->tnode.flags2 |= TRICK2_HAS_SWAY;


    if (trick->texScrollAmt[0][0] || trick->texScrollAmt[0][1])
        trick->tnode.flags2 |= TRICK2_STSCROLL0;
    if (trick->texScrollAmt[1][0] || trick->texScrollAmt[1][1])
        trick->tnode.flags2 |= TRICK2_STSCROLL1;

#ifndef OFFLINE
    if ( trick->stAnim ) //only ever one st_anim.  Since it's a struct, though, it needs this (TO DO do a Mark's get count thing)
    {
        if (shared_memory)
            sharedHeapMemoryManagerUnlock();

        if( setStAnim(*trick->stAnim) )
            trick->tnode.flags2 |= TRICK2_STANIMATE;

        if (shared_memory)
            sharedHeapMemoryManagerLock();
    }
#endif

    if (trick->group_flags & GROUP_VIS_TRAY)
        trick->model_flags |= OBJ_FORCEOPAQUE|OBJ_ALLOW_MULTISPLIT;


    if (!trick->name) {
        FatalErrorf("Trick in %s does not have a name!\n", trick->file_name);
    }
    
    if (!stashAddPointer(trick_list.trick_name_hashes,trick->name,trick, false))
    {
        char    author1[1000],author2[1000];

        stashFindPointer( trick_list.trick_name_hashes,trick->name, &dup_trick );
        Errorf("duplicate trick: %s\n1st %s (checked in by: %s)\n2nd %s (checked in by: %s)\n\n",
            trick->name,trick->file_name,author1,dup_trick->file_name,author2);
    }
}

static bool trickLoadPreProcess(ParseTable* pti, void* structptr)
{
    TrickList * tlist = (TrickList *)structptr;
    int i;
    for(i=eaSize(&tlist->texopts)-1;i>=0;i--)
    {
        char *s;
        TexOpt *tex = tlist->texopts[i];
        if(!tex->name)
            continue;
        if(tex->name[0]=='/')
            tex->name++;
            
        s = strrchr(tex->name,'.');
        if(s) *s = 0;

        tex->name = ParserAllocString(tex->name); // Memory leak when binning the file but who cares
    }
    return true;
}

static bool trickLoadPostProcess(TokenizerParseInfo pti[], TrickList *tlist, bool shared_memory)
{
    int    i;

    // Note: trickLoadPostProcess is called both at startup when just loaded all tricks, as well as in dev 
    //    mode at runtime when just reloaded a single trick (in which case the name hashes already exist).

    // Changing this hash table to a shallow copy, since it appears
    // the tex->name passed in as a key should always be valid
    assert(!tlist->trick_name_hashes);
    assert(!tlist->texopt_name_hashes);
    tlist->texopt_name_hashes = stashTableCreateWithStringKeys(stashOptimalSize(eaSize(&tlist->texopts)), stashShared(shared_memory));
    tlist->trick_name_hashes = stashTableCreateWithStringKeys(stashOptimalSize(eaSize(&tlist->tricks)), stashShared(shared_memory));

    for(i=eaSize(&tlist->texopts)-1;i>=0;i--)
        setupTexOpt(tlist->texopts[i], shared_memory);

    for(i=eaSize(&tlist->tricks)-1;i>=0;i--)
        setupTrick(tlist->tricks[i], shared_memory);

    return true;
}

void trickReload()
{
    TrickList* tricklist_writeable = cpp_const_cast(TrickList*)(&trick_list);
    assertmsg(sharedMemoryGetMode()==SMM_DISABLED, "Attempting to reload tricks in shared memory, will crash!");
    assert(!isSharedMemory(tricklist_writeable));

    // Prototype function, currently only works for GetTex
    // Dynamic reloading implemented down below
    stashTableDestroy(trick_list.texopt_name_hashes);
    stashTableDestroy(trick_list.trick_name_hashes);
    tricklist_writeable->texopt_name_hashes = NULL;
    tricklist_writeable->trick_name_hashes = NULL;

    ParserUnload(parse_trick_list, &trick_list, sizeof(trick_list));

    if (!ParserLoadFiles("tricks", ".txt", "tricks.bin", 0, parse_trick_list, (TrickList*)&trick_list, 0, NULL, trickLoadPreProcess))
    {
        Errorf("trick2 load error\n");
    }

    trickLoadPostProcess(NULL, tricklist_writeable, false);
}

int g_disableTrickReload=0;

void trickReloadPostProcess(void)
{
    PERFINFO_AUTO_START("trickReloadPostProcess",1);
#if CLIENT|SERVER // Not GetTex
#if CLIENT
    PERFINFO_AUTO_START("texResetBinds",1);
//    loadstart_printf("texResetBinds..");
    trickRecreateTextures();
    texResetBinds();
//    loadend_printf("done.");
    PERFINFO_AUTO_STOP();
#endif
    PERFINFO_AUTO_START("modelResetAllFlags",1);
//    loadstart_printf("modelResetAllFlags..");
    modelResetAllFlags(true);
//    loadend_printf("done.");
    PERFINFO_AUTO_STOP();
#if CLIENT
    PERFINFO_AUTO_START("animCalculateSortFlags",1);
//    loadstart_printf("animCalculateSortFlags..");
    animCalculateSortFlags( gfx_tree_root );
//    loadend_printf("done.");
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_START("gfxTreeResetTrickFlags",1);
//    loadstart_printf("gfxTreeResetTrickFlags..");
    gfxTreeResetTrickFlags( gfx_tree_root );
//    loadend_printf("done.");
    PERFINFO_AUTO_STOP();
#endif
    PERFINFO_AUTO_START("groupResetAllTrickFlags",1);
//    loadstart_printf("groupResetAllTrickFlags..");
#if CLIENT
    lodinfoClearLODsFromTricks();
#endif
    groupResetAll(RESET_TRICKS|RESET_LODINFOS);
//    loadend_printf("done.");
    PERFINFO_AUTO_STOP();
#if CLIENT
    PERFINFO_AUTO_START("entResetTrickFlags",1);
//    loadstart_printf("entResetTrickFlags..");
    entResetTrickFlags();
//    loadend_printf("done.");
    PERFINFO_AUTO_STOP();

    fogGather();
    groupDrawBuildMiniTrackers();
#endif
#endif
    PERFINFO_AUTO_STOP();
}

static TrickReloadCallback trickReloadCallback = 0;

void trickSetReloadCallback(TrickReloadCallback callback)
{
    trickReloadCallback = callback;
}

static void reloadTrickProcessorDummyCheck(const char *relpath, int when)
{
    char fullpath[MAX_PATH];
    sprintf(fullpath, "%s/%s", fileDataDir(), relpath);
    if (!(strEndsWith(fullpath, ".txt") || strEndsWith(fullpath, ".bak"))) {
        printf("File (\"%s\") in tricks/ does not end with .txt, NOT reloading!\n", fullpath);
#if CLIENT
        status_printf("Tricks NOT reloaded (not a .txt file).");
#endif
    }
}

char **eaTrickReloads=NULL;

static void reloadTrickProcessor(const char *relpath, int when)
{
    eaPush(&eaTrickReloads, strdup(relpath));
}

int gLODReloadCount = 0;
int gLODReloadTriCount = 0;

static void reloadTrick(const char *relpath)
{
    char fullpath[MAX_PATH];
    TrickList* tricklist_writeable = cpp_const_cast(TrickList*)(&trick_list);

    if (g_disableTrickReload)
        return;

    if (strstr(relpath, "/_")) {
        return;
    }

    sprintf(fullpath, "%s/%s", fileDataDir(), relpath);

    if (!fileExists(fullpath))
        ; // File was deleted, do we care here?

    fileWaitForExclusiveAccess(fullpath);
    //errorLogFileIsBeingReloaded(relpath);

    assertmsg(sharedMemoryGetMode()==SMM_DISABLED, "Attempting to reload tricks in shared memory, will crash!");
    assert(!isSharedMemory(tricklist_writeable));

    PERFINFO_AUTO_START("TrickReload",1);
//    printf("Trick file changed (%s), reloading..\n", relpath);
    PERFINFO_AUTO_START("ParserReloadFile",1);
    
    if (!ParserReloadFile((char*)relpath, parse_trick_list, sizeof(TrickList), tricklist_writeable, NULL, trickLoadPreProcess))
    {
        PERFINFO_AUTO_STOP();
        ErrorFilenamef(relpath, "Error reloading trick file: %s\n", relpath);
#if CLIENT
        status_printf("Error reloading trick file: %s\n.", relpath);
#endif
    } else {
        // Fix up hash tables, re-initialize structures, etc
        gLODReloadCount = 0;
        gLODReloadTriCount = 0;
        PERFINFO_AUTO_STOP_START("Post-process",1);
//        loadstart_printf("post-process..");
        PERFINFO_AUTO_START("trickLoadPostProcess",1);
//        loadstart_printf("trickLoadPostProcess..");

        // destroy stashtables, these get re-created in trickLoadPostProcess
        stashTableDestroy(tricklist_writeable->texopt_name_hashes);
        stashTableDestroy(tricklist_writeable->trick_name_hashes);
        tricklist_writeable->texopt_name_hashes = NULL;
        tricklist_writeable->trick_name_hashes = NULL;

        trickLoadPostProcess(NULL, tricklist_writeable, false);
//        loadend_printf("done.");
        PERFINFO_AUTO_STOP();
        trickReloadPostProcess();
//        loadend_printf("Done.");
        PERFINFO_AUTO_STOP();
#if CLIENT
        status_printf("Tricks reloaded (%d LODs/%.1fK tris)", gLODReloadCount, gLODReloadTriCount/1024.f);
        printf("Tricks reloaded (%d lods/%.1fK tris).\n", gLODReloadCount, gLODReloadTriCount/1024.f);
#endif
    }

    if (trickReloadCallback)
        trickReloadCallback(relpath);

    PERFINFO_AUTO_STOP();
}

void checkTrickReload()
{
    char *s;
    while (s = eaPop(&eaTrickReloads)) {
        if (!eaSize(&eaTrickReloads) || stricmp(s, eaTrickReloads[eaSize(&eaTrickReloads)-1])!=0) {
            // Not the same as the next one
            reloadTrick(s);
        }
        free(s);
    }
}

static void printLayerString(int i)
{
    int j;
    for (j=0; j<BLEND_NUM_BLEND_NAMES; j++) {
        if (i & (1<<j)) {
            printf("%s ", blendIndexToName(j));
        }
    }
}

static void trickStatsGather()
{
    int i, j;
    int count=0;
    int noscrollscalecount=0;
    int matchingdualcolor=0;
    int bacount[1<<BLEND_NUM_BLEND_NAMES]={0};
    printf("\n");
    for(i=eaSize(&trick_list.texopts)-1;i>=0;i--) {
        TexOpt *texopt = trick_list.texopts[i];
        if (texopt->flags & TEXOPT_MULTITEX) {
            int ba=0;
            bool noscrollscale=true;
            if (strstri(texopt->name, "jimb"))
                continue;
            count++;
            for (j=0; j<BLEND_NUM_BLEND_NAMES; j++) {
                if (texopt->blend_names[j]) {
                    if (stricmp(texopt->blend_names[j], "white")==0 || strStartsWith(texopt->blend_names[j], "white."))
                        continue;
                    ba |= 1 << j;
                    if (j!=BLEND_MULTIPLY1 && j!=BLEND_MULTIPLY2) {
                        if (texopt->scrollsScales.texopt_scale[j][0]==1 &&
                            texopt->scrollsScales.texopt_scale[j][1]==1 &&
                            texopt->scrollsScales.texopt_scroll[j][0]==0 &&
                            texopt->scrollsScales.texopt_scroll[j][1]==0)
                        {
                        } else {
                            noscrollscale = false;
                        }
                    }
                }
            }
            if (noscrollscale)
                noscrollscalecount++;
            if (texopt->blend_names[BLEND_DUALCOLOR1] && texopt->blend_names[BLEND_DUALCOLOR2] &&
                0==stricmp(texopt->blend_names[BLEND_DUALCOLOR1], texopt->blend_names[BLEND_DUALCOLOR2]))
            {
                matchingdualcolor++;
            }
            assert(ba < ARRAY_SIZE(bacount));
            bacount[ba]++;
//            printf(" TEX: \"%s\"  ", texopt->name);
//            printLayerString(ba);
//            printf("\n");
        }
    }
    printf("\n\n%d Multitex Textures\n", count);
    printf(" %d without scrolls/scales (ex. multiply)\n", noscrollscalecount);
    printf(" %d with matching dual color\n", matchingdualcolor);
    for (i=0; i<ARRAY_SIZE(bacount); i++) {
        if (bacount[i]) {
            printf("   %4X: %d ", i, bacount[i]);
            printLayerString(i);
            printf("\n");
        }
    }
    printf("");
}

static bool trickLoadPostProcessCallback(ParseTable pti[], void* structptr, bool shared_memory)
{
    return trickLoadPostProcess(pti, (TrickList *)structptr, shared_memory);
}

void trickLoad()
{
#if SERVER
    // Need copy on write for trick reloading and some pointers are patched up
    if (!ParserLoadFilesShared("SM_Tricks", "tricks", ".txt", "tricks.bin", 0, parse_trick_list, &trick_list, sizeof(trick_list), 0, NULL, trickLoadPreProcess, NULL, trickLoadPostProcessCallback))
    {
        Errorf("Trick load error\n");
    }
#else
    if (!ParserLoadFiles("tricks", ".txt", "tricks.bin", 0, parse_trick_list, &trick_list, 0, NULL, trickLoadPreProcess))
    {
        // Already got a popup
    }
    trickLoadPostProcess(NULL, &trick_list, false);
#endif

    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "tricks/*.txt", reloadTrickProcessor);
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "tricks/*", reloadTrickProcessorDummyCheck);

    if (0) {
        trickStatsGather();
    }
}

int trickCompare(TrickInfo *trick1, TrickInfo *trick2)
{
    TrickInfo dummy1, dummy2;
    memcpy(&dummy1, trick1, sizeof(dummy1));
    memcpy(&dummy2, trick2, sizeof(dummy1));
    dummy1.name = dummy2.name = "";
    dummy1.auto_lod = dummy2.auto_lod = 0;
    dummy1.file_name = dummy2.file_name = 0;
    dummy1.fileAge = dummy2.fileAge = 0;
    return ParserCompareStruct(parse_trick, &dummy1, &dummy2);
}

#if CLIENT
static mEArrayHandle texopt_delayedLoad;

static bool IsValidTexName(const char* texname)
{
    if( !texname || !texname[0] || texname[0]=='%' || !stricmp(texname, "swappable") || !stricmp(texname, "none"))
        return false;
    return true;
}

static bool trickFindAllTextures(TexOpt *texopt, const char** missingTexName)
{
    int i;
    const char* firstMissing = NULL;

    for(i=0; i<BLEND_NUM_BLEND_NAMES; i++) {
        const char* texname = texopt->blend_names[i];
        if( IsValidTexName(texname) && !texFind(texname)) {
            firstMissing = texname;
            break;
        }
    }

    if(!firstMissing && texopt->fallback.useFallback)
    {
        // check fallback textures
        const char* fbTex[3] = {texopt->fallback.base_name, texopt->fallback.blend_name, texopt->fallback.bumpmap};
        for(i=0; i<3; i++) {
            if( IsValidTexName(fbTex[i]) && !texFind(fbTex[i])) {
                firstMissing = fbTex[i];
                break;
            }
        }
    }

    if(missingTexName)
        *missingTexName = firstMissing;
    
    return (firstMissing == NULL);
}

void trickCreateTextures(void) // Creates "textures" that are stubbed in tricks but don't exist as .tgas
{
    int i, num;

    num = eaSize(&trick_list.texopts);
    for (i=0; i<num; i++) 
    {
        TexOpt *texopt = trick_list.texopts[i];
        bool bTexoptNameMatchesBase1;
        const char* missingTexName;

        assert( !texopt->blend_names[BLEND_OLD_TEX_BASE] ); // this entry is old and not used
        if (!texopt->blend_names[BLEND_BASE1])
            continue; // not a material trick

        // If texopt name matches base1 texture, we don't need to create a trick texture since the composite
        //    texture is already created from the base texture. 
        bTexoptNameMatchesBase1 = !stricmp(texopt->blend_names[BLEND_BASE1], texopt->name) ||
            !strnicmp(texopt->blend_names[BLEND_BASE1], texopt->name, strlen(texopt->name)); // blendname may end with .tga
        if(bTexoptNameMatchesBase1)
            continue;

        // Only create the trick (composite) texture if all of its textures exist.  This is needed for lightweight client staged loading.
        if( trickFindAllTextures(texopt, &missingTexName) ) {
            texCreateTrickTexture(texopt->file_name, texopt->name, texopt->blend_names[BLEND_BASE1]);
        } else {
            if( LWC_IsActive() ) {
                // This texture is probably in a later stage file.  Save the trick in a list to be processed when new stage data gets loaded
                if (!texopt_delayedLoad)
                    eaCreate(&texopt_delayedLoad);
                eaPush(&texopt_delayedLoad, texopt);
            } else {
                ErrorFilenamef(texopt->file_name, "Texture Trick \"%s\" references missing texture \"%s\".",
                    texopt->name, missingTexName);
            }
        }
    }

    if( LWC_IsActive() && texopt_delayedLoad) {
        printf("trickCreateTextures: added %d / %d tricks to delayed load list\n", eaSize(&texopt_delayedLoad), num);
    }
}

// Called when new stage data is loaded during gameplay.  Check for our missing trick texture and try loading them again.
void trickOnLWCAdvanceStage(LWC_STAGE stage)
{
    int i, numDelayedStart, numTexStart;
    
    // First load any new textures in this stage
    numTexStart = texGetNumBasicTextures();
    texScanForTexHeaders(false);

    if(!texopt_delayedLoad)
        return; // didnt have any delayed load textures that are part of tricks (unlikely)

    numDelayedStart = eaSize(&texopt_delayedLoad);

    for (i = (eaSize(&texopt_delayedLoad)-1); i >=0; --i) {
        TexOpt *texopt;
        
        // similar logic as above in trickCreateTextures
        texopt = texopt_delayedLoad[i];
        if( trickFindAllTextures(texopt, NULL) ) {
            TexBind* bind;

            //printf("Delayed trick creation for texopt \"%s\"\n", texopt->name);
            bind = texCreateTrickTexture(texopt->file_name, texopt->name, texopt->blend_names[BLEND_BASE1]);
            assert(bind);
            eaRemove(&texopt_delayedLoad, i); // remove before texSetBindsSubComposite since list gets checked there
            texSetBindsSubComposite(bind);    
        }
    }

    printf("trickOnLWCAdvanceStage: stage %d contained %d basic textures and %d / %d delayed trick loads.\n", stage,
        texGetNumBasicTextures() - numTexStart, numDelayedStart - eaSize(&texopt_delayedLoad), numDelayedStart);

    if(stage == LWC_STAGE_LAST) {
        // free our list since there is no more delayed loading
        if(eaSize(&texopt_delayedLoad)) {
            printf("trickOnLWCAdvanceStage: clearing list, should have zero items left, but still has %d\n", eaSize(&texopt_delayedLoad));
        }
        eaDestroy(&texopt_delayedLoad);
        texopt_delayedLoad = NULL;
    }
}

bool trickIsDelayedLoadTexOpt(TexOpt *texopt)
{
    if(!LWC_IsActive() || !texopt_delayedLoad)
        return false;

    return eaFind(&texopt_delayedLoad, texopt) != -1;
}

void trickRecreateTextures(void) // Creates "textures" that are stubbed in tricks but don't exist as .tgas
{
    int i;
    bool bNew=false;
    int num = eaSize(&trick_list.texopts);
    for (i=0; i<num; i++) 
    {
        TexOpt *texopt = trick_list.texopts[i];
        if (texopt->blend_names[BLEND_BASE1]) {
            if (texFind(texopt->name)) {
                // This is a texopt for an existing texture
            } else {
                // No base texture
                if (texFindComposite(texopt->name)) {
                    // Already created
                } else if (texFind(texopt->blend_names[BLEND_BASE1])) {
                    // Make a texture
                    TexBind *bind = texCreateTrickTexture(texopt->file_name, texopt->name, texopt->blend_names[BLEND_BASE1]);
                    // Assign tricks, etc
                    texSetBindsSubComposite(bind);
                    bNew = true;
                }
            }
        }
    }
    if (bNew)
        modelRebuildTextures();
}

#endif // CLIENT
