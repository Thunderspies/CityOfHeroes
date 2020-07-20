#include <utilitieslib/stdtypes.h>
#include "group/groupscene.h"
#include <string.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/timing.h>
#include "group/group.h"

#if CLIENT
#include "graphics/sun.h"
#include "graphics/light.h"
#include "render/tex.h"
#include "clientError.h"
#include "graphics/groupMiniTrackers.h"
#include "render/thread/rt_queue.h"
#endif

#define DEFAULT_MAX_HEIGHT 900
SceneInfo scene_info;    //Parsed scene file and name of sky file
static char last_scenefile[MAX_PATH] = "";

TokenizerParseInfo parse_tex_swap[] = {
    { "Source",            TOK_POOL_STRING|TOK_STRING(TexSwap,src, 0)    },
    { "Dest",            TOK_POOL_STRING|TOK_STRING(TexSwap,dst, 0)    },
    { "{",                TOK_START,        0                        },
    { "}",                TOK_END,            0                        },
    { "", 0, 0 }
};

TokenizerParseInfo parse_color_swap[] = {
    { "MatchHue",        TOK_VEC2(ColorSwap,match_hue)    },
    { "Hue",            TOK_F32(ColorSwap,hsv[0], 0)    },
    { "Saturation",        TOK_F32(ColorSwap,hsv[1], 0)    },
    { "Brightness",        TOK_F32(ColorSwap,hsv[2], 0)    },
    { "Magnify",        TOK_F32(ColorSwap,magnify, 0)    },
    { "Reset",            TOK_U8(ColorSwap,reset, 0)    },
    { "{",                TOK_START,        0                        },
    { "}",                TOK_END,            0                        },
    { "", 0, 0 }
};

TokenizerParseInfo parse_scene[] = {
    { "CubeMap",                                TOK_STRING(SceneInfo,cubemap, 0)    },
    { "Sky",                                    TOK_STRINGARRAY(SceneInfo,sky_names)    },
    { "SkyFadeRate",                            TOK_F32(SceneInfo,sky_fade_rate, 0)},
    { "ClipFoggedGeometry",                        TOK_INT(SceneInfo,clip_fogged_geometry, 0)},
    { "ScaleSpecularity",                        TOK_F32(SceneInfo,scale_specularity, 0)},
    { "FogRampColor",                            TOK_F32(SceneInfo,fog_ramp_color, 0)}, // Never been used
    { "FogRampDistance",                        TOK_F32(SceneInfo,fog_ramp_distance, 0)}, // Never been used
    { "TexSwap",                                TOK_STRUCT(SceneInfo,tex_swaps, parse_tex_swap) },
    { "MaterialSwap",                            TOK_STRUCT(SceneInfo,material_swaps, parse_tex_swap) },
    { "ColorSwap",                                TOK_STRUCT(SceneInfo,color_swaps, parse_color_swap) },
    { "MaxHeight",                                TOK_F32(SceneInfo,maxHeight, 0)},
    { "MinHeight",                                TOK_F32(SceneInfo,minHeight, 0)},
    { "FogColor",                                TOK_VEC3(SceneInfo,force_fog_color)},
    { "FogNear",                                TOK_F32(SceneInfo,force_fog_near, 0)},
    { "FogFar",                                    TOK_F32(SceneInfo,force_fog_far, 0)},
    { "ColorFogNodes",                            TOK_INT(SceneInfo,colorFogNodes, 0)},
    { "DoNotOverrideFogNodes",                    TOK_INT(SceneInfo,doNotOverrideFogNodes, 0)},
    { "AmbientColor",                            TOK_VEC3(SceneInfo,force_ambient_color)},
    { "FallingDamagePercent",                    TOK_F32(SceneInfo,fallingDamagePercent, 0)},
    { "ToneMapWeight",                            TOK_F32(SceneInfo,toneMapWeight, 0)},
    { "BloomWeight",                            TOK_F32(SceneInfo,bloomWeight, 0)},
    { "Desaturate",                                TOK_F32(SceneInfo,desaturateWeight, 0)},
    { "IndoorLuminance",                        TOK_F32(SceneInfo,indoorLuminance, 0)},
    { "CharacterMinAmbient",                    TOK_VEC3(SceneInfo,character_min_ambient_color)},
    { "CharacterAmbientScale",                    TOK_VEC3(SceneInfo,character_ambient_scale)},
    { "StippleFadeMode",                        TOK_INT(SceneInfo,stippleFadeMode, 0)},
    { "AmbientOcclusionWeight",                    TOK_F32(SceneInfo,ambientOcclusionWeight, 1)},
    { "ShadowMaps_Off",                            TOK_INT(SceneInfo,shadowMaps_Off, -1) },
    { "ShadowMaps_ModulationClamp_ambient",        TOK_F32(SceneInfo,shadowMaps_modulationClamp_ambient, -1.0f)},
    { "ShadowMaps_ModulationClamp_diffuse",        TOK_F32(SceneInfo,shadowMaps_modulationClamp_diffuse, -1.0f)},
    { "ShadowMaps_ModulationClamp_specular",    TOK_F32(SceneInfo,shadowMaps_modulationClamp_specular, -1.0f)},
    { "ShadowMaps_ModulationClamp_vertex_lit",    TOK_F32(SceneInfo,shadowMaps_modulationClamp_vertex_lit, -1.0f)},
    { "ShadowMaps_ModulationClamp_NdotL_rolloff",TOK_F32(SceneInfo,shadowMaps_modulationClamp_NdotL_rolloff, -1.0f)},
    { "ShadowMaps_LastCascadeFade",                TOK_F32(SceneInfo,shadowMaps_lastCascadeFade, -1.0f)},
    { "ShadowMaps_Distance",                    TOK_F32(SceneInfo,shadowMaps_farMax, -1.0f)},
    { "ShadowMaps_SplitLambda",                    TOK_F32(SceneInfo,shadowMaps_splitLambda, -1.0f)},
    { "ShadowMaps_MaxSunAngleDegrees",            TOK_F32(SceneInfo,shadowMaps_maxSunAngleDeg, -1.0f)},
    { "", 0, 0 }
};


static void reloadSceneCallback(const char *relpath, int when) {
    if (last_scenefile[0]) {
        fileWaitForExclusiveAccess(relpath);
        errorLogFileIsBeingReloaded(relpath);
        sceneLoad(last_scenefile);
#if CLIENT
        texCheckSwapList();
        lightReapplyColorSwaps();
        groupDrawBuildMiniTrackers();
        status_printf("Scene file reloaded");        
#endif
    }
}

static sceneGraphicsCustomizations(void)
{
#if CLIENT
    rdrQueue(DRAWCMD_SHADOWMAP_SCENE_SPECS,&scene_info,sizeof(scene_info));
#endif
}

void sceneLoad(char *fname)
{
    TokenizerHandle tok;
    int        fileisgood;
    int i;
    static    int inited=0;

    if (!fname) {
        last_scenefile[0]=0;
        return;
    }

    PERFINFO_AUTO_START("sceneLoad", 1);

    if (!fileExists(fname)) {
        ErrorFilenamef(group_info.scenefile, "Map references missing scene file: %s", fname);
        fname = "scenes/default_scene.txt";
    }

    ParserDestroyStruct(parse_scene, &scene_info);    
    ZeroStruct(&scene_info);
    scene_info.scale_specularity = 1;
    scene_info.fog_ramp_color = 0.02; // Never been set to anything else in a scene file
    scene_info.fog_ramp_distance = 25; // Never been set to anything else in a scene file
    scene_info.force_ambient_color[0] = -8e8;
    scene_info.force_fog_color[0] = -8e8;
    scene_info.force_fog_far = -8e8;
    scene_info.force_fog_near = -8e8;
    scene_info.character_min_ambient_color[0] = -8e8;
    scene_info.character_ambient_scale[0] = -8e8;
    scene_info.bloomWeight = 1.0;
    scene_info.toneMapWeight = 1.0;
    scene_info.indoorLuminance = 0;
    scene_info.sky_fade_rate = 0.02;
    scene_info.desaturateWeight = 0.0;
    scene_info.ambientOcclusionWeight = 1.0;
    
    // Scene specific overrides
    scene_info.shadowMaps_Off = 0;
        // For these values, the default value of -1.0f means "not a valid override" (i.e., ignore it).
    scene_info.shadowMaps_modulationClamp_ambient = -1.0f;
    scene_info.shadowMaps_modulationClamp_diffuse = -1.0f;
    scene_info.shadowMaps_modulationClamp_specular = -1.0f;
    scene_info.shadowMaps_modulationClamp_vertex_lit = -1.0f;
    scene_info.shadowMaps_modulationClamp_NdotL_rolloff = -1.0f;
    scene_info.shadowMaps_lastCascadeFade = -1.0f;
    scene_info.shadowMaps_farMax = -1.0f;
    scene_info.shadowMaps_splitLambda = -1.0f;
    scene_info.shadowMaps_maxSunAngleDeg = -1.0f;
        
    scene_info.fallingDamagePercent = 1.0;

    tok = TokenizerCreate(fname);
    if( !tok )
        FatalErrorf( "This is not a valid scene file %s ", fname );
    fileisgood = TokenizerParseList(tok, parse_scene, &scene_info, TokenizerErrorCallback);
    TokenizerDestroy(tok);

    if( scene_info.maxHeight == 0 )
        scene_info.maxHeight = DEFAULT_MAX_HEIGHT;

    scene_info.indoorLuminance *= 1/255.f;

#if CLIENT
    scaleVec3(scene_info.force_ambient_color,1.f/255.f,scene_info.force_ambient_color);
    scaleVec3(scene_info.force_fog_color,1.f/255.f,scene_info.force_fog_color);
    scaleVec3(scene_info.character_min_ambient_color,1.f/255.f,scene_info.character_min_ambient_color);

    if (eaSize(&scene_info.sky_names)==0) {
        ErrorFilenamef(fname, "Scene file references no sky files.");
    } else {
        skyLoad( scene_info.sky_names, fname );
    }
#endif

    for (i=eaSize(&scene_info.material_swaps)-1; i>=0; i--) {
        if (!scene_info.material_swaps[i]->dst ||
            !scene_info.material_swaps[i]->dst[0] ||
            !scene_info.material_swaps[i]->src ||
            !scene_info.material_swaps[i]->src[0])
        {
            ErrorFilenamef(fname, "MaterialSwap is missing either a source or destination material name.");
            eaRemove(&scene_info.material_swaps, i);
        }
    }
    for (i=eaSize(&scene_info.tex_swaps)-1; i>=0; i--) {
        if (!scene_info.tex_swaps[i]->dst ||
            !scene_info.tex_swaps[i]->dst[0] ||
            !scene_info.tex_swaps[i]->src ||
            !scene_info.tex_swaps[i]->src[0])
        {
            ErrorFilenamef(fname, "TexSwap is missing either a source or destination material name.");
            eaRemove(&scene_info.tex_swaps, i);
        }
    }

    strcpy(last_scenefile, fname);

    if (!inited) {
        // Add callback for re-loading scenes
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "scenes/*.txt", reloadSceneCallback);
        inited = 1;
    }
    
    sceneGraphicsCustomizations();
    
    PERFINFO_AUTO_STOP();
}