#define WCW_STATEMANAGER
#define RT_PRIVATE
#include "render/thread/ogl.h"
#include "render/thread/wcw_statemgmt.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include "assert.h"
#include "render/bump.h"
#include "render/thread/shadersATI.h"
#include "rt_shaderMgr.h"
#include "shadersTexEnv.h"
#include <utilitieslib/utils/file.h>
#include "render/thread/rt_tex.h"
#include "rt_stats.h"
#include "rt_prim.h"
#include "render/tex.h"
#include "render/thread/rt_cubemap.h"
#include "render/thread/rt_shadowmap.h"
#include "render/thread/rt_cgfx.h"
#include "render/thread/rt_util.h"
#include "cmdparse/cmdgame.h"

DrawModeType curr_draw_state;
ModelStateSetFlags curr_draw_state_flags;
BlendModeType curr_blend_state;

//Debug 
int setBlendStateCalls[BLENDMODE_NUMENTRIES];
int setBlendStateChanges[BLENDMODE_NUMENTRIES];
int setDrawStateCalls[DRAWMODE_NUMENTRIES];
int setDrawStateChanges[DRAWMODE_NUMENTRIES];
int call = 0, drawCall = 0;
int blendModeSwitchOrder[200];
int drawModeSwitchOrder[200];
//End Debug

char *blend_mode_names[BLENDMODE_NUMENTRIES] = {
    "MODULATE",
    "MULTIPLY",
    "COLORBLEND_DUAL",
    "ADDGLOW",
    "ALPHADETAIL",
    "BUMPMAP_MULTIPLY",
    "BUMPMAP_COLORBLEND_DUAL",
    "WATER",
    "MULTI",
    "SUNFLARE",
};

F32 constColor0[4],constColor1[4];

typedef struct
{
    int        id,id2;
    char    *fileName;
    char    *fileNameNv20;
    char    *fileNameTexEnv;
} CombinerInfo;

CombinerInfo combiner_infos[] =
{
    {
        BLENDMODE_COLORBLEND_DUAL, BLENDMODE_BUMPMAP_COLORBLEND_DUAL,

        "shaders/colorBlendDual.rcp",
        "shaders/colorBlendDualNv20.rcp",
        "shaders/texenv/colorBlendDual.tec",
    },
    {
        BLENDMODE_MULTIPLY, BLENDMODE_BUMPMAP_MULTIPLY,

        "shaders/multiplyReg.rcp",
        NULL,
        "shaders/texenv/multiplyReg.tec"
    },
    {
        BLENDMODE_ADDGLOW,-1,

        "shaders/addGlow.rcp",
        NULL,
        "shaders/texenv/addGlow.tec"
    },
    {
        BLENDMODE_ALPHADETAIL,-1,

        "shaders/alphaDetail.rcp",
        NULL,
        "shaders/texenv/alphaDetail.tec"
    },
    {
        BLENDMODE_BUMPMAP_MULTIPLY,-1,

        "shaders/bumpmapVertDiffuse.rcp",
        NULL,
        NULL,
    },
    {
        BLENDMODE_BUMPMAP_COLORBLEND_DUAL,-1,

        "shaders/colorBlendDualBump.rcp",
        NULL,
        NULL,
    },
};

int blender_dlists[BLENDMODE_NUMENTRIES];
int blender_dlists_special[1];

void modelBlendStateInit()
{
    int        i,j,ret;
    char    *s;

    if (rdr_caps.chip & (ARBFP|GLSL)) {
        shaderMgr_InitFPs();
        return;
    }

    if (rdr_caps.chip & R200) {
        atiFSColorBlendDual();
        atiFSAddGlow();
        atiFSAlphaDetail();
        atiFSMultiply();
        atiFShaderBumpMultiply();
        atiFSBumpColorBlend();
        return;
    }

    // Free old ones
    for (i=0; i<BLENDMODE_NUMENTRIES; i++) {
        glDeleteLists(blender_dlists[i], 1); CHECKGL;
        blender_dlists[i] = 0;
    }
    if (blender_dlists_special[0]) {
        glDeleteLists(blender_dlists_special[0], 1); CHECKGL;
        blender_dlists_special[0] = 0;
    }

    //assert( heapValidateAll() ); 
    for(i=0;i<BLENDMODE_NUMENTRIES;i++)
    {
        //assert( heapValidateAll() ); 
        blender_dlists[i] = glGenLists(1); CHECKGL;
        glNewList(blender_dlists[i],GL_COMPILE); CHECKGL;
        for(j=0;j<ARRAY_SIZE(combiner_infos);j++)
        {
            if (!rdr_caps.chip) // Unknown graphics chip
                continue;
            if (((rdr_caps.chip&(NV1X|NV2X)) == NV1X && combiner_infos[j].id2 == i) || 
                ((rdr_caps.chip & (NV1X)) && combiner_infos[j].id == i))
            {
                glEnable(GL_REGISTER_COMBINERS_NV); CHECKGL;
                //assert( heapValidateAll() );
                if ((rdr_caps.chip & NV2X) && combiner_infos[j].fileNameNv20) {
                    s = fileAlloc(combiner_infos[j].fileNameNv20, NULL);
                    if (!s) {
                        FatalErrorf("Error loading %s", combiner_infos[j].fileNameNv20);
                    }
                } else {
                    s = fileAlloc(combiner_infos[j].fileName, NULL);
                    if (!s) {
                        FatalErrorf("Error loading %s", combiner_infos[j].fileName);
                    }
                }
                ret = renderNvparse(s);
                fileFree(s);
                if (!ret) {
                    s = fileAlloc("shaders/error.rcp", NULL);
                    renderNvparse(s);
                    fileFree(s);
                }

                if (combiner_infos[j].id == BLENDMODE_BUMPMAP_COLORBLEND_DUAL) {
                    glEnable(GL_PER_STAGE_CONSTANTS_NV); CHECKGL;
                }
                //assert( heapValidateAll() ); 
                break;
            } else if (rdr_caps.chip == TEX_ENV_COMBINE && (combiner_infos[j].id2 == i || combiner_infos[j].id == i) && combiner_infos[j].fileNameTexEnv) {
                ret = renderTexEnvparse(combiner_infos[j].fileNameTexEnv, i);
                if (!ret) {
                    // error case!
                    ret = renderTexEnvparse("shaders/texenv/error.tec", i);
                }
                if (ret) {
                    break;
                }
            }
        }

        if (j >= ARRAY_SIZE(combiner_infos))
        {
            //assert( heapValidateAll() ); 
            if (rdr_caps.chip & NV1X) { // Need to do this on ATI too?
                glDisable(GL_REGISTER_COMBINERS_NV); CHECKGL;
            }
            
            if(glActiveTextureARB) {
                glActiveTextureARB(GL_TEXTURE1); CHECKGL;
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); CHECKGL;
                glActiveTextureARB(GL_TEXTURE0); CHECKGL;
            }
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); CHECKGL;

            //assert( heapValidateAll() );
            if (!rdr_caps.chip)    // This is just a quick hack to make things appear ugly
            {                    // rather than not appear at all
                if (i == BLENDMODE_COLORBLEND_DUAL || i == BLENDMODE_ADDGLOW) {
                    glDisable(GL_BLEND); CHECKGL;
                } else {
                    glEnable(GL_BLEND); CHECKGL;
                }
            }
        }

        // Return to tex 0 at the end
        if(glActiveTextureARB) glActiveTextureARB(GL_TEXTURE0); CHECKGL;
        glEndList(); CHECKGL;
        //assert( heapValidateAll() ); 
    }

    if ((rdr_caps.chip&(NV1X|NV2X)) == NV1X) {
        char *special_name = "shaders/colorBlendDualNv1xWorld.rcp";
        blender_dlists_special[0] = glGenLists(1); CHECKGL;
        glNewList(blender_dlists_special[0],GL_COMPILE); CHECKGL;
        glEnable(GL_REGISTER_COMBINERS_NV); CHECKGL;
        s = fileAlloc(special_name, NULL);
        if (!s) {
            FatalErrorf("Error loading %s", special_name);
        }
        ret = renderNvparse(s);
        fileFree(s);
        if (!ret) {
            s = fileAlloc("shaders/error.rcp", NULL);
            renderNvparse(s);
            fileFree(s);
        }

        // Return to tex 0 at the end
        glActiveTextureARB(GL_TEXTURE0); CHECKGL;
        glEndList(); CHECKGL;
    }
    //assert( heapValidateAll() ); 
}

static struct {
    BlendModeShader blend_mode;
    char *name;
} blend_names[] = {
    { BLENDMODE_MODULATE, "Modulate" },
    { BLENDMODE_MULTIPLY, "Multiply" },
    { BLENDMODE_COLORBLEND_DUAL, "ColorBlendDual" },
    { BLENDMODE_ADDGLOW, "AddGlow" },
    { BLENDMODE_ALPHADETAIL, "AlphaDetail" },
    { BLENDMODE_BUMPMAP_MULTIPLY, "BumpMultiply" },
    { BLENDMODE_BUMPMAP_COLORBLEND_DUAL, "BumpColorBlend" },
    { BLENDMODE_WATER, "Water" },
    { BLENDMODE_MULTI, "MultiTexture" },
    { BLENDMODE_SUNFLARE, "SunFlare" }
};

char *renderBlendName(BlendModeType blend)
{
    static char buffer[256];
    int        i;

    for(i=0;i<ARRAY_SIZE(blend_names);i++)
    {
        if (blend_names[i].blend_mode == blend.shader) {
            // SHADERTODO: Add bits?
            if (blend.blend_bits) {
                strcpy(buffer, blend_names[i].name);
                if (blend.blend_bits & BMB_SINGLE_MATERIAL) {
                    strcat(buffer, " SM");
                }
                if (blend.blend_bits & BMB_HIGH_QUALITY) {
                    strcat(buffer, " HQ");
                }
                if (blend.blend_bits & BMB_BUILDING) {
                    strcat(buffer, " Bldg");
                }
                if (blend.blend_bits & BMB_OLDTINTMODE) {
                    strcat(buffer, " OTM");
                }
                if (blend.blend_bits & BMB_CUBEMAP) {
                    strcat(buffer, " CUBE");
                }
                if (blend.blend_bits & BMB_PLANAR_REFLECTION) {
                    strcat(buffer, " PR");
                }
                if (blend.blend_bits & BMB_SHADOWMAP) {
                    strcat(buffer, " SHADOW");
                }
                return buffer;
            } else {
                return blend_names[i].name;
            }
        }
    }
    return "UnknownBlend";
}

BlendModeType BlendMode(BlendModeShader shader, BlendModeBits blend_bits)
{
    BlendModeType ret;
    ret.shader = shader;
    ret.blend_bits = blend_bits;
    return ret;
}

//TO DO make first part inline.
void modelBlendState(BlendModeType type, int force_set)
{
    BlendModeShader num = type.shader;
    setBlendStateCalls[num]++;  //Debug global
    
    RT_STAT_INC(blendstate_calls);

    if (!force_set && curr_blend_state.intval == type.intval)
        return;
    
    RT_STAT_BLEND_CHANGE;

#ifndef FINAL //Debug
    setBlendStateChanges[num]++;
    if( call < ARRAY_SIZE(blendModeSwitchOrder) )
        blendModeSwitchOrder[call++] = num;
#endif

    if (rdr_caps.chip & (ARBFP|GLSL))
    {
        //glDisable(GL_REGISTER_COMBINERS_NV);  //TODO remove
        if(num == BLENDMODE_MODULATE && (rdr_caps.use_fixedfunctionfp || rt_cgGetCgShaderMode()==tCgShaderMode_Disabled))
        {
            // CD: BLENDMODE_MODULATE is multiplying textures by each other and by the vertex colors.
            //     Does not do the multiply by constant color or the scale by 8 that BLENDMODE_MULTIPLY does.
            WCW_DisableFragmentProgram();
            //glCallList(blender_dlists[num]); CHECKGL;
        } 
        else
        {
            int pgmID;

            WCW_EnableFragmentProgram();
            pgmID = shaderMgr_lookup_fp( type );
            WCW_BindFragmentProgram(pgmID);

            // Bind utility textures
            if (num == BLENDMODE_ADDGLOW) {
                texBind(2, rdr_view_state.building_lights_tex_id);
            } else if(num == BLENDMODE_MULTI) {
                texBind(15, rdr_view_state.building_lights_tex_id);
            }
            // Gfx Dev HUD special utility/visualization binds
            // @todo wrap this up cleaner
#ifndef FINAL
            if (g_mt_shader_debug_state.flag_use_debug_shaders && g_mt_shader_debug_state.visualize_tex_id)
            {
                texBind(14,g_mt_shader_debug_state.visualize_tex_id);    // yes, bind it to unused global sampler
            }
#endif    
        }
    }
    else if (rdr_caps.chip & R200)        // USE ATI 'register combiner' programs on Radeon 8500
    {
        if (num != BLENDMODE_MODULATE)
        {
            WCW_Fog(1);
            WCW_EnableFragmentProgram();
            if (num == BLENDMODE_MULTIPLY) {
                glBindFragmentShaderATI(fsMultiply); CHECKGL;
            } else if (num == BLENDMODE_COLORBLEND_DUAL) {
                glBindFragmentShaderATI(fsColorBlendDual); CHECKGL;
            } else if (num == BLENDMODE_ADDGLOW) {
                glBindFragmentShaderATI(fsAddGlow); CHECKGL;
            } else if (num == BLENDMODE_ALPHADETAIL) {
                glBindFragmentShaderATI(fsAlphaDetail); CHECKGL;
            } else if (num == BLENDMODE_BUMPMAP_MULTIPLY) {
                glBindFragmentShaderATI(fsBumpMultiply); CHECKGL;
            } else if (num == BLENDMODE_BUMPMAP_COLORBLEND_DUAL) {
                glBindFragmentShaderATI(fsBumpColorBlend); CHECKGL;
            }
        }
        else
        {
            WCW_DisableFragmentProgram();
        }
    }
    else if (rdr_caps.chip == TEX_ENV_COMBINE)        // generic OpenGL texture stage 'register combiner' mode (used on low end Intel gpus)
    {
        preRenderTexEnvSetupBlendmode(num);
        glCallList(blender_dlists[num]); CHECKGL;
        WCW_CheckGLState();
    }
    else    // must be using nVidia register combiner extensions, blends have been precompiled into display lists
    {
        // All lists return to this active texture at the end
        WCW_ActiveTexture(0);
        if (num == BLENDMODE_COLORBLEND_DUAL && type.blend_bits&BMB_NV1XWORLD && ((rdr_caps.chip&(NV1X|NV2X))==NV1X)) {
            glCallList(blender_dlists_special[0]); CHECKGL;
        } else {
            glCallList(blender_dlists[num]); CHECKGL;
        }
        WCW_CheckGLState();
    }
    curr_blend_state = type;
    g_renderstate_dirty = true;
}

static void setupLights()
{
    WCW_EnableGL_LIGHTING(1);
    WCW_EnableColorMaterial();
    glEnable(GL_LIGHT0); CHECKGL;
    WCW_Color4(255,255,255,255);
}

void modelDrawState(DrawModeType num, ModelStateSetFlags flags)
{
    Vec4 def_reflect = {0,0,1,1};
    setDrawStateCalls[num]++; //Debug global

    RT_STAT_INC(drawstate_calls);

#if 0
    // Every call in this function is state managed separately, so I can remove the curr_draw_state management.
    // This way, stuff that changes these states after setting the DrawState will not screw something else up.
    // i.e. Obj1 -> DRAWMODE_FILL
    //      Obj1 -> change lighting to on
    //      Obj2 -> DRAWMODE_FILL (state managed away)
    //      Obj2 -> Oh no, now lighting is on!  WHY?!?!?!?!  WHY?!?!?!  THE PAIN!!!!!!
    //
    // TODO: Figure out why this is so slow

    if (curr_draw_state != num)
    {
        setDrawStateChanges[num]++; //Debug global
        if( drawCall < ARRAY_SIZE(drawModeSwitchOrder) )
            drawModeSwitchOrder[drawCall++] = num;
        
        RT_STAT_DRAW_CHANGE;
    }

#else

    if ( !(flags&FORCE_SET) && ( curr_draw_state == num ) && ( curr_draw_state_flags == flags))
        return;

    setDrawStateChanges[num]++; //Debug global
    if( drawCall < ARRAY_SIZE(drawModeSwitchOrder) )
        drawModeSwitchOrder[drawCall++] = num;

    RT_STAT_DRAW_CHANGE;

#endif    

    #define BINDVP WCW_BindVertexProgram((flags&BIT_HIGH_QUALITY_VERTEX_PROGRAM)?shaderMgrVertexProgramsHQ[num]:shaderMgrVertexPrograms[num])

    switch(num)
    {
        xcase DRAWMODE_BUMPMAP_SKINNED:
            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BASE );
            texEnableTexCoordPointer(TEXLAYER_BASE, true);

            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_GENERIC );
            texEnableTexCoordPointer(TEXLAYER_GENERIC, true);

            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BUMPMAP1 );

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_DisableClientState(GLC_COLOR_ARRAY);
            WCW_Color4_Force(255,255,255,255);            // we need to force color to GL after disabling color array
            WCW_EnableClientState(GLC_NORMAL_ARRAY);

            WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );
            WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );

            WCW_EnableVertexProgram();
            BINDVP;
            WCW_EnableGL_NORMALIZE(0);

        xcase DRAWMODE_BUMPMAP_SKINNED_MULTITEX:
            texEnableTexCoordPointer(TEXLAYER_BASE, true);
            texEnableTexCoordPointer(TEXLAYER_GENERIC, false);

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_DisableClientState(GLC_COLOR_ARRAY);
            WCW_Color4_Force(255,255,255,255);            // we need to force color to GL after disabling color array
            WCW_EnableClientState(GLC_NORMAL_ARRAY);

            WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );    // formerly used to supply pre-calculated binormal
            WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );

            WCW_EnableVertexProgram();
            BINDVP;
            WCW_EnableGL_NORMALIZE(0);

        xcase DRAWMODE_BUMPMAP_DUALTEX:
            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BASE );
            texEnableTexCoordPointer(TEXLAYER_BASE, true);

            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_GENERIC );
            texEnableTexCoordPointer(TEXLAYER_GENERIC, true);

            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BUMPMAP1 );

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_DisableClientState(GLC_COLOR_ARRAY);
            WCW_Color4(255,255,255,255);
            WCW_EnableClientState(GLC_NORMAL_ARRAY);

            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );
            WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );

            WCW_EnableVertexProgram();
            BINDVP;
            WCW_EnableGL_NORMALIZE(0);

        xcase DRAWMODE_BUMPMAP_MULTITEX_RGBS:  // indoor
        case DRAWMODE_BUMPMAP_MULTITEX:  // outside
            texEnableTexCoordPointer(TEXLAYER_BASE, true); // Only one texture coordinate pointer
            texEnableTexCoordPointer(TEXLAYER_GENERIC, false);

            //WCW_EnableTexture( GL_TEXTURE_2D, ... ); // ARBvp ignores glEnable(GL_TEXTURE_2D) calls

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_DisableClientState(GLC_COLOR_ARRAY);
            WCW_Color4_Force(255,255,255,255);            // we need to force color to GL after disabling color array
            WCW_EnableClientState(GLC_NORMAL_ARRAY);

            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );
            WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );

            WCW_EnableVertexProgram();
            BINDVP;
            if (num == DRAWMODE_BUMPMAP_MULTITEX)
            {
                WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );
            }
            else
            {
                WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );
            }
            WCW_EnableGL_NORMALIZE(0);

        xcase DRAWMODE_BUMPMAP_NORMALS:        // outside, vertex lit with specular bump per pixel
        case DRAWMODE_BUMPMAP_RGBS:            // indoor (ambient group), baked lighting and specular bump per pixel
        case DRAWMODE_BUMPMAP_NORMALS_PP:    // full per pixel lighting in view space    
            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BASE );
            texEnableTexCoordPointer(TEXLAYER_BASE, true);

            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_GENERIC );
            texEnableTexCoordPointer(TEXLAYER_GENERIC, true);

            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BUMPMAP1 );

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_DisableClientState(GLC_COLOR_ARRAY);
            WCW_Color4_Force(255,255,255,255);            // we need to force color to GL after disabling color array
            WCW_EnableClientState(GLC_NORMAL_ARRAY);

            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );    // formerly used to supply pre-calculated binormal
            WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );

            WCW_EnableVertexProgram();
            BINDVP;
            if (num == DRAWMODE_BUMPMAP_RGBS)
            {
                WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );
            }
            else
            {
                WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );
            }
            WCW_EnableGL_NORMALIZE(0);

        xcase DRAWMODE_HW_SKINNED:
            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BASE );
            texEnableTexCoordPointer(TEXLAYER_BASE, true);

            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_GENERIC );
            texEnableTexCoordPointer(TEXLAYER_GENERIC, true);

            WCW_DisableTexture( TEXLAYER_BUMPMAP1 );

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_DisableClientState(GLC_COLOR_ARRAY);
            WCW_Color4_Force(255,255,255,255);            // we need to force color to GL after disabling color array

            WCW_EnableClientState(GLC_NORMAL_ARRAY);

            WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_EnableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );

            WCW_EnableVertexProgram();
            BINDVP;
            setupLights();
            WCW_EnableGL_NORMALIZE(0);

        xcase DRAWMODE_SPRITE:
            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BASE );
            texEnableTexCoordPointer(TEXLAYER_BASE, true);
            
            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_GENERIC );
            texSetWhite(TEXLAYER_GENERIC);
            texEnableTexCoordPointer(TEXLAYER_GENERIC, false);

            WCW_DisableTexture( TEXLAYER_BUMPMAP1 );

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_EnableClientState(GLC_COLOR_ARRAY);
            WCW_DisableClientState(GLC_NORMAL_ARRAY);

            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );

            if (rdr_caps.use_fixedfunctionvp)
            {
                WCW_EnableVertexProgram();
                BINDVP;
                WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_ReflectionParamVP, def_reflect );
            }
            else
            {
                WCW_DisableVertexProgram();
            }
            WCW_EnableGL_LIGHTING(0);
            WCW_EnableGL_NORMALIZE(0);

        xcase DRAWMODE_DUALTEX:
            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BASE );
            texEnableTexCoordPointer(TEXLAYER_BASE, true);

            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_GENERIC );
            texEnableTexCoordPointer(TEXLAYER_GENERIC, true);

            WCW_DisableTexture( TEXLAYER_BUMPMAP1 );

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_EnableClientState(GLC_COLOR_ARRAY);
            WCW_DisableClientState(GLC_NORMAL_ARRAY);

            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );

            if (rdr_caps.use_fixedfunctionvp)
            {
                WCW_EnableVertexProgram();
                BINDVP;
                WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_ReflectionParamVP, def_reflect );
            }
            else
            {
                WCW_DisableVertexProgram();
            }
            WCW_EnableGL_LIGHTING(0);
            WCW_EnableGL_NORMALIZE(0);

        xcase DRAWMODE_DUALTEX_NORMALS:
        case DRAWMODE_DUALTEX_LIT_PP:
            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BASE );
            texEnableTexCoordPointer(TEXLAYER_BASE, true);

            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_GENERIC );
            texEnableTexCoordPointer(TEXLAYER_GENERIC, true);

            WCW_DisableTexture( TEXLAYER_BUMPMAP1 );

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_DisableClientState(GLC_COLOR_ARRAY);
            WCW_Color4_Force(255,255,255,255);            // we need to force color to GL after disabling color array
            WCW_EnableClientState(GLC_NORMAL_ARRAY);

            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );

            if (rdr_caps.use_fixedfunctionvp)
            {
                WCW_EnableVertexProgram();
                BINDVP;
                WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_ReflectionParamVP, def_reflect );
            }
            else
            {
                WCW_DisableVertexProgram();
            }

            setupLights();
            WCW_EnableGL_NORMALIZE(1);

        xcase DRAWMODE_COLORONLY:
            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BASE );
            texSetWhite(TEXLAYER_BASE);
            texEnableTexCoordPointer(TEXLAYER_BASE, false);

            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_GENERIC );
            texSetWhite(TEXLAYER_GENERIC);
            texEnableTexCoordPointer(TEXLAYER_GENERIC, false);

            WCW_DisableTexture( TEXLAYER_BUMPMAP1 );

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_DisableClientState(GLC_COLOR_ARRAY);
            WCW_Color4_Force(255,255,255,255);            // we need to force color to GL after disabling color array
            WCW_DisableClientState(GLC_NORMAL_ARRAY);

            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );

            if (rdr_caps.use_fixedfunctionvp)
            {
                WCW_EnableVertexProgram();
                BINDVP;
                WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_ReflectionParamVP, def_reflect );
            }
            else
            {
                WCW_DisableVertexProgram();
            }
            WCW_EnableGL_LIGHTING(0);
            WCW_EnableGL_NORMALIZE(0);

        xcase DRAWMODE_FILL:
            WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BASE );
            texEnableTexCoordPointer(TEXLAYER_BASE, false);

            WCW_DisableTexture( TEXLAYER_GENERIC );
            texEnableTexCoordPointer(TEXLAYER_GENERIC, false);

            WCW_DisableTexture( TEXLAYER_BUMPMAP1 );

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_DisableClientState(GLC_COLOR_ARRAY);
            WCW_Color4_Force(255,255,255,255);            // we need to force color to GL after disabling color array
            WCW_DisableClientState(GLC_NORMAL_ARRAY);

            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );

            if (rdr_caps.use_fixedfunctionvp)
            {
                WCW_EnableVertexProgram();
                BINDVP;
                WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_ReflectionParamVP, def_reflect );
            }
            else
            {
                WCW_DisableVertexProgram();
            }
            WCW_EnableGL_LIGHTING(0);
            WCW_EnableGL_NORMALIZE(0);

        xcase DRAWMODE_DEPTH_ONLY:
        case DRAWMODE_DEPTHALPHA_ONLY:
            if (num == DRAWMODE_DEPTHALPHA_ONLY) {
                WCW_EnableTexture( GL_TEXTURE_2D, TEXLAYER_BASE );
                texEnableTexCoordPointer(TEXLAYER_BASE, true);
            } else {
                WCW_DisableTexture( TEXLAYER_BASE );
                texEnableTexCoordPointer(TEXLAYER_BASE, false);
            }

            WCW_DisableTexture( TEXLAYER_GENERIC );
            texEnableTexCoordPointer(TEXLAYER_GENERIC, false);

            WCW_DisableTexture( TEXLAYER_BUMPMAP1 );

            WCW_EnableClientState(GLC_VERTEX_ARRAY);
            WCW_DisableClientState(GLC_COLOR_ARRAY);
            WCW_DisableClientState(GLC_NORMAL_ARRAY);

            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY1_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY5_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY6_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY7_NV );
            WCW_DisableClientState( GLC_VERTEX_ATTRIB_ARRAY11_NV );

            if (0 && rdr_caps.use_fixedfunctionvp)
            {
                WCW_EnableVertexProgram();
                BINDVP;
                WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_ReflectionParamVP, def_reflect );
            }
            else
            {
                WCW_DisableVertexProgram();
            }
            WCW_EnableGL_LIGHTING(0);
            WCW_EnableGL_NORMALIZE(0);
    }

    curr_draw_state = num;
    curr_draw_state_flags = flags & ~(FORCE_SET);
    g_renderstate_dirty = true;
}

void rdrPrepareForDraw(void)
{
    if (!g_renderstate_dirty)
        return;
    assert(curr_blend_state.intval != -1 && curr_draw_state != -1);
    // Last minute fix-up stuff here!
    // Make sure appropriate texture units are enabled and have the appropriate pointers
    renderTexEnvSetupBlendmode(curr_blend_state.shader);
    g_renderstate_dirty = false;
}

void modelSetAlpha(U8 a)
{
    constColor0[3] = a * 1.f/255.f;
    WCW_ConstantCombinerParamerterfv(0, constColor0);
}

#ifndef FINAL
// setup debug toggles used by the debug/dev shaders
static void setupDebugParams(void)
{
    {
        // These should match the "Debug Flag Macros" in debug_fp.cgh
        // Note that g_DebugEnableFlagSetsFP is a float4 so only about 24 bits
        // are usable in each component. 
        Vec4 debug_flags;
        unsigned int flags[4] = {0,0,0,0};

        // lighting enables
        flags[0] |= g_mt_shader_debug_state.use_ambient            ? (1 << 0) : 0;
        flags[0] |= g_mt_shader_debug_state.use_diffuse            ? (1 << 1) : 0;
        flags[0] |= g_mt_shader_debug_state.use_specular        ? (1 << 2) : 0;
        flags[0] |= g_mt_shader_debug_state.use_light_color        ? (1 << 3) : 0;
        flags[0] |= g_mt_shader_debug_state.use_gloss_factor    ? (1 << 4) : 0;
        flags[0] |= g_mt_shader_debug_state.use_bump            ? (1 << 5) : 0;
        flags[0] |= g_mt_shader_debug_state.use_shadowed        ? (1 << 6) : 0;

        // material enables
        flags[1] |= g_mt_shader_debug_state.use_vertex_color        ? (1 << 0) : 0;
        flags[1] |= g_mt_shader_debug_state.use_blend_base            ? (1 << 1) : 0;
        flags[1] |= g_mt_shader_debug_state.use_blend_dual            ? (1 << 2) : 0;
        flags[1] |= g_mt_shader_debug_state.use_blend_multiply        ? (1 << 3) : 0;
        flags[1] |= g_mt_shader_debug_state.use_blend_addglow        ? (1 << 4) : 0;
        flags[1] |= g_mt_shader_debug_state.use_blend_alpha            ? (1 << 5) : 0;
        flags[1] |= g_mt_shader_debug_state.use_lod_alpha            ? (1 << 6) : 0;

        // feature enables
        flags[2] |= g_mt_shader_debug_state.use_fog                    ? (1 << 0) : 0;
        flags[2] |= g_mt_shader_debug_state.use_reflection            ? (1 << 1) : 0;
        flags[2] |= g_mt_shader_debug_state.use_sub_material_1        ? (1 << 2) : 0;
        flags[2] |= g_mt_shader_debug_state.use_sub_material_2        ? (1 << 3) : 0;
        flags[2] |= g_mt_shader_debug_state.use_material_building    ? (1 << 4) : 0;

        flags[2] |= g_mt_shader_debug_state.mode_texture_display    ? (1 << 8) : 0;
        flags[2] |= g_mt_shader_debug_state.tex_display_rgba        ? (1 << 9) : 0;
        flags[2] |= g_mt_shader_debug_state.tex_display_rgb            ? (1 << 10) : 0;
        flags[2] |= g_mt_shader_debug_state.tex_display_alpha        ? (1 << 11) : 0;
//        flags[2] |= g_mt_shader_debug_state.use_material            ? (1 << 1) : 0;
//        flags[2] |= g_mt_shader_debug_state.use_faux_reflection        ? (1 << 2) : 0;

        flags[2] |= g_mt_shader_debug_state.mode_visualize            ? (1 << 15) : 0;

        // aux/selector number
        if (g_mt_shader_debug_state.mode_texture_display)    // texture has precedence over visualize
        {
            flags[3] = g_mt_shader_debug_state.selected_texture;
        }
        else if (g_mt_shader_debug_state.mode_visualize)
        {
            flags[3] = g_mt_shader_debug_state.selected_visualization;
            flags[2] |= g_mt_shader_debug_state.visualize_tex_id            ? (1 << 16) : 0;    // use visualization texture?
            if (g_mt_shader_debug_state.visualize_tex_id)
            {
                texBind(14,g_mt_shader_debug_state.visualize_tex_id);    // yes, bind it to unused global sampler
            }
        }

        debug_flags[0] = flags[0];
        debug_flags[1] = flags[1];
        debug_flags[2] = flags[2];
        debug_flags[3] = flags[3];

        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_DebugEnableFlagSetsFP, debug_flags );
    }
    {
        Vec4 disabled_color = { 1.0f, 0.5f, 0.5f, 1.0f };
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_DebugDisabledColorFP, disabled_color );
    }
}
#endif // !FINAL

#if SHADER_DBG_CODE_ENABLED

    typedef enum {
        kShaderDbgState_OFF,
        kShaderDbgState_REQUESTED,
        kShaderDbgState_LOGGING,
        kShaderDbgState_CANCELLED
    };
    static int gShaderDbgLoggingState = kShaderDbgState_OFF;
    static int gShaderDbgLoggingFrames = 0;
    
    // normally, called at the top of the frame
    static void MANAGE_SHADER_DEBUG_LOGING()
    {
        static int nLogFrameNumber = 0;
        if (( gShaderDbgLoggingState == kShaderDbgState_OFF ) &&
            ( GetAsyncKeyState( VK_MENU ) < 0 ))    // i.e., pressed
        {
            gShaderDbgLoggingState = kShaderDbgState_REQUESTED;
            if ( GetAsyncKeyState( VK_SHIFT ) < 0 )
            {
                gShaderDbgLoggingFrames = 50;
            }
            else
            {
                gShaderDbgLoggingFrames = 1;
            }
        }
        
        if ( gShaderDbgLoggingState == kShaderDbgState_REQUESTED )
        {
            char fpath[256];
            char the_date[32] = "";
            time_t now;

            // add date/frame to filename so we don't overwrite old results
            now = time(NULL);
            if (now != -1)
            {
                strftime(the_date, MAX_PATH, "%Y%m%d", gmtime(&now));
            }

            sprintf( fpath, "c:\\temp\\SHADER_DBG_LOG_%s_f%05d", the_date, game_state.client_frame);

            printf( "=========== BEGIN: Shader Debug Log: %s =============\n", fpath );
            gSHADER_DBG_LOG_FILE = fopen( fpath, "wt" );
            gShaderDbgLoggingState = kShaderDbgState_LOGGING;
            nLogFrameNumber = 0;
        }
        if ((( gShaderDbgLoggingState == kShaderDbgState_LOGGING ) ||
             ( gShaderDbgLoggingState == kShaderDbgState_CANCELLED )) &&
            ( nLogFrameNumber > 0 ))
        {
            fprintf( gSHADER_DBG_LOG_FILE, "----- END OF FRAME #%d -----\n", nLogFrameNumber );
        }
        if ( gShaderDbgLoggingState == kShaderDbgState_LOGGING )
        {
            if ( nLogFrameNumber == gShaderDbgLoggingFrames )
            {
                gShaderDbgLoggingState = kShaderDbgState_CANCELLED;
            }
            else
            {
                nLogFrameNumber++;
                fprintf( gSHADER_DBG_LOG_FILE, "----- TOP OF FRAME #%d -----\n", nLogFrameNumber );
            }
        }
        if ( gShaderDbgLoggingState ==     kShaderDbgState_CANCELLED )
        {
            fclose( gSHADER_DBG_LOG_FILE );
            gSHADER_DBG_LOG_FILE = NULL;
            gShaderDbgLoggingState = kShaderDbgState_OFF;
            nLogFrameNumber = 0;
            printf( "=========== END: Shader Debug Log: %s =============\n", SHADER_DBG_LOG_FILE_NAME );
        }
    }

    // not static...
    void SHADER_DBG_LOG_TO_END_OF_FRAME( void )
    {
        assert( gShaderDbgLoggingState == kShaderDbgState_OFF );
        gShaderDbgLoggingState = kShaderDbgState_REQUESTED;
        gShaderDbgLoggingFrames = 1;
        MANAGE_SHADER_DEBUG_LOGING();
    }
    void SHADER_DBG_LOGGING_ON( void )
    {
        assert( gShaderDbgLoggingState == kShaderDbgState_OFF );
        gShaderDbgLoggingState = kShaderDbgState_REQUESTED;
        gShaderDbgLoggingFrames = -1;
        MANAGE_SHADER_DEBUG_LOGING();
    }
    void SHADER_DBG_LOGGING_OFF( void )
    {
        gShaderDbgLoggingState = kShaderDbgState_CANCELLED;
        MANAGE_SHADER_DEBUG_LOGING();
    }


#else
    #define MANAGE_SHADER_DEBUG_LOGING()
#endif

void rdrInitTopOfFrameDirect(RdrFrameState *rfs)
{
    rdr_frame_state = *rfs;
}

void rdrInitTopOfViewDirect(RdrViewState *rvs)
{
    MANAGE_SHADER_DEBUG_LOGING();

    rdrBeginMarker(__FUNCTION__);
    WCW_FetchGLState();
    WCW_ResetState();

    {
        Mat44    matx;
        copyMat44(rdr_view_state.projectionMatrix, matx);
        rdr_view_state = *rvs;
        copyMat44(matx, rdr_view_state.projectionMatrix);
    }

    if (rvs->viewmat[0][0] || rvs->viewmat[0][1] || rvs->viewmat[0][2]) // Skip this invalid call on very first call to this
    {
        WCW_LoadModelViewMatrix(rvs->viewmat);
    }

    // Setup some environment shader parameters

    if ( rdr_caps.allowed_features & GFXF_CUBEMAP )
    {
        Vec4 planarReflectionTransform = {1.0f/rdr_view_state.width, 1.0f/rdr_view_state.height, 0, 0};
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_PlanarReflectionTransformFP, planarReflectionTransform);
    }

    WCW_EnableGL_LIGHTING(1);
    WCW_EnableColorMaterial();
    glEnable(GL_LIGHT0); CHECKGL;
    WCW_Color4(255,255,255,255);

    //These two should not be needed since every model handles it's own lighting, and 
    //(I think everything should set their own; it's currently a bit over complicated)
    { //Set light
        WCW_Light(GL_LIGHT0, GL_AMBIENT, rvs->sun_ambient);
        WCW_Light(GL_LIGHT0, GL_DIFFUSE, rvs->sun_diffuse);
    }

    //This is the only place specular ans specref gets set
    { //Set specular
        GLfloat specular[4];
        setVec3(specular,0,0,0);
        specular[3] = 1.0;
        WCW_Light(GL_LIGHT0, GL_SPECULAR, specular);
    }

    { //Set specref
        F32 specref[4];
        F32 shininess[1];
        
        setVec4(specref,0,0,0,1);
        shininess[0] = 128;

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, specref); CHECKGL;    

        WCW_Material(GL_FRONT, GL_SPECULAR, specref);
        WCW_Material(GL_FRONT, GL_SHININESS, shininess);
    }

    WCW_Fog(1); 
    WCW_ClearTextureStateToWhite();
    modelBindDefaultBuffer(); //need to reset this state every frame, or headshot can mess up if sky isn't drawn. I'm not entirely sure why.

    WCW_SunLightDir();
    rdrSetFogDirect2(rvs->fogdist[0], rvs->fogdist[1], rvs->fogcolor);
    if (game_state.iAmAnArtist && !game_state.edit && !game_state.wireframe) { // Exagerate z-fighting issues
        glDepthFunc((global_state.global_frame_count&1)?GL_LEQUAL:GL_LESS); CHECKGL;
    }

    if (rdr_view_state.write_depth_only) {
        glColorMask(0,0,0,0); CHECKGL;
    } else {
        glColorMask(1,1,1,1); CHECKGL;
    }

    glAlphaFunc(GL_GREATER,0); CHECKGL;

#if 0  // should be able to set just once here but doesn't seem to work, even with param set to kDataSrc_EnvVariable
    if( rvs->renderPass == RENDERPASS_COLOR )
    {
        extern void getNewFeatureForCurWindow(Vec4 * newFeature);
        Vec4 newFeature;
        getNewFeatureForCurWindow(&newFeature);
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_NewFeatureFP, newFeature );
    }
#endif
    
    // setup some global reflection shader params once
    if( (rdr_caps.chip & ARBFP) && rvs->renderPass == RENDERPASS_COLOR )
    {
        Vec4 param;
        param[0] = game_state.reflection_distort_mix;
        param[1] = game_state.reflection_base_normal_fade_near;
        param[2] = game_state.reflection_base_normal_fade_near + game_state.reflection_base_normal_fade_range;
        param[3] = game_state.reflection_base_strength;
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_DistortionParamsFP, param );
    }

    // prepare environment for rendering with shadowmaps (if applicable)
    // @todo we'll probably want to find a better place for this or
    // change the name of this to more accurately reflect 'top of' viewport
    if ( rdr_caps.allowed_features & GFXF_SHADOWMAP )
    {
        rt_shadowmap_begin_using();
    }

#ifndef FINAL
    setupDebugParams();    // setup common constants for gfx dev HUD and debug shaders
#endif // !FINAL

    rdrEndMarker();
}


#undef glDrawElements
void glDrawElementsWrapper(GLenum a, GLsizei b, GLenum c, const GLvoid *d)
{
    // Ignore glDrawElement() calls with one or fewer elements
    if (b > 1)
    {
        if (rdr_caps.chip == TEX_ENV_COMBINE)
            rdrPrepareForDraw();
        glDrawElements(a, b, c, d); CHECKGL;
    }
}

void glDrawTrisDebug(GLsizei vert_count, const GLvoid *ptr)
{
    glDrawElements(GL_TRIANGLES, vert_count, GL_UNSIGNED_INT, ptr);    CHECKGL;
}

#undef glDrawArrays
void glDrawArraysWrapper(GLenum mode, GLint first, GLsizei count)
{
    if (rdr_caps.chip == TEX_ENV_COMBINE)
        rdrPrepareForDraw();
    glDrawArrays(mode, first, count); CHECKGL;
}

#undef glBegin
void glBeginWrapper(GLenum a)
{
    if (rdr_caps.chip == TEX_ENV_COMBINE)
        rdrPrepareForDraw();
    glBegin(a);
}

void bumpInit()
{
    if(rdr_caps.chip & (ARBVP|GLSL))
    {
        shaderMgr_InitVPs();
    }
}

bool blendModeHasBump(BlendModeType blend)
{
    bool ret = blend.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL ||
        blend.shader == BLENDMODE_BUMPMAP_MULTIPLY ||
        blend.shader == BLENDMODE_MULTI ||
        blend.shader == BLENDMODE_WATER;
    if (!(rdr_caps.features & GFXF_BUMPMAPS)) {
        //create_bins run with no vert shaders or VBOs, so no GFXF_BUMPMAPS
        //assert(!ret || game_state.create_bins); // TEXTODO: can we ensure we just never get these blendmodes on old cards?
        return 0;
    }
    return ret;
}

bool blendModeHasDualColor(BlendModeType blend)
{
    return blend.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL || blend.shader == BLENDMODE_COLORBLEND_DUAL || blend.shader == BLENDMODE_MULTI;
}

BlendModeType promoteBlendMode(BlendModeType oldBlendMode, BlendModeType newBlendMode)
{
    bool hasBump;
    if (!newBlendMode.shader)
        return oldBlendMode;
    if (!oldBlendMode.shader)
        return newBlendMode;
    if (newBlendMode.intval == oldBlendMode.intval)
        return oldBlendMode;
    if (newBlendMode.shader==BLENDMODE_MULTIPLY)
        return oldBlendMode;
    if (oldBlendMode.shader==BLENDMODE_MULTIPLY)
        return newBlendMode;

    if (newBlendMode.shader == BLENDMODE_MULTI ||
        oldBlendMode.shader == BLENDMODE_MULTI)
    {
        // handle promotion of bits
        if (newBlendMode.shader == BLENDMODE_MULTI && oldBlendMode.shader == BLENDMODE_MULTI) {
            return BlendMode(BLENDMODE_MULTI, newBlendMode.blend_bits & oldBlendMode.blend_bits);
        } else if (newBlendMode.shader == BLENDMODE_MULTI) {
            return newBlendMode;
        } else {
            return oldBlendMode;
        }
    }
    if (newBlendMode.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL ||
        oldBlendMode.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL)
        return BlendMode(BLENDMODE_BUMPMAP_COLORBLEND_DUAL,0);
    hasBump = blendModeHasBump(oldBlendMode) || blendModeHasBump(newBlendMode);
    if (newBlendMode.shader == BLENDMODE_ADDGLOW ||
        oldBlendMode.shader == BLENDMODE_ADDGLOW)
    {
//        if (hasBump)
//            return BLENDMODE_COLORBLEND_DUAL;
//        else
            return BlendMode(BLENDMODE_ADDGLOW, 0);
    }
    if (hasBump) {
        if (newBlendMode.shader == BLENDMODE_COLORBLEND_DUAL ||
            oldBlendMode.shader == BLENDMODE_COLORBLEND_DUAL)
            return BlendMode(BLENDMODE_BUMPMAP_COLORBLEND_DUAL, 0);
        return BlendMode(BLENDMODE_BUMPMAP_MULTIPLY, 0);
    }
    if (newBlendMode.shader == BLENDMODE_COLORBLEND_DUAL ||
        oldBlendMode.shader == BLENDMODE_COLORBLEND_DUAL)
        return BlendMode(BLENDMODE_COLORBLEND_DUAL, 0);
    return newBlendMode;
}

void OnGLContextChange(void)
{
    rdrBeginMarker(__FUNCTION__);

    WCW_FetchGLState();

    texSetWhite(TEXLAYER_GENERIC);
    texSetWhite(TEXLAYER_BASE);
    WCW_FetchGLState();
    WCW_ResetState();

    WCW_BindVertexProgram(0);
    WCW_DisableVertexProgram();
    WCW_BindFragmentProgram(0);
    WCW_DisableFragmentProgram();
    WCW_PrepareToDraw();
    rdrEndMarker();
}

#ifdef MARKERS_ENABLED
#define MAX_MARKER_STRING 256
#define MAX_MARKER_DEPTH 64
__declspec(thread) int markersEnabled = false;
int markerDepth = 0;
char markerNames[MAX_MARKER_DEPTH][MAX_MARKER_STRING];

void _rdrBeginMarker(const char * name, ...)
{
    char buf[MAX_MARKER_STRING + 32];
    va_list va;

    assert(markersEnabled);

    if( rdrIsThreaded() && !rdrInThread() ) {
        va_start(va, name);
        vsnprintf(buf, MAX_MARKER_STRING, name, va);
        va_end(va);

        rdrQueue(DRAWCMD_BEGINMARKER, buf, strlen(buf)+1);
        return;
    }

    assert(markerDepth < 64);

    va_start(va, name);
    vsnprintf(markerNames[markerDepth], MAX_MARKER_STRING, name, va);
    va_end(va);

    SHADER_DBG_PRINTF("gre start(%d,%s)\n", markerDepth, markerNames[markerDepth]);

    if(GLEW_GREMEDY_string_marker) {
        snprintf(buf, sizeof(buf), "start(%d,%s)", markerDepth, markerNames[markerDepth]);
        glStringMarkerGREMEDY(0, buf); CHECKGL;
    }

    markerDepth++;
}

void _rdrEndMarker()
{
    char buf[MAX_MARKER_STRING + 32];

    assert(markersEnabled);

    if( rdrIsThreaded() && !rdrInThread() ) {
        rdrQueueCmd(DRAWCMD_ENDMARKER);
        return;
    }

    assert(markerDepth > 0);
    markerDepth--;

    SHADER_DBG_PRINTF("gre end(%d,%s)\n", markerDepth, markerNames[markerDepth]);

    if(GLEW_GREMEDY_string_marker) {
        snprintf(buf, sizeof(buf), "end(%d,%s)", markerDepth, markerNames[markerDepth]);
        glStringMarkerGREMEDY(0, buf); CHECKGL;
    }
}

void _rdrSetMarker(const char * msg, ...)
{
    char buf[MAX_MARKER_STRING + 1];
    va_list va;

    assert(markersEnabled);

    va_start(va, msg);
    vsnprintf(buf, MAX_MARKER_STRING, msg, va);
    va_end(va);

    SHADER_DBG_PRINTF("gre %s\n", buf);

    if( rdrIsThreaded() && !rdrInThread() ) {
        rdrQueue(DRAWCMD_SETMARKER, buf, strlen(buf)+1);
        return;
    }

    if(GLEW_GREMEDY_string_marker) {
        glStringMarkerGREMEDY(0, buf); CHECKGL;
    }
}

void rdrBeginMarkerFrame()
{
    rdrCheckNotThread();
    markersEnabled = isDevelopmentOrQAMode() && game_state.gpu_markers;
    rdrQueue(DRAWCMD_BEGINMARKERFRAME, &markersEnabled, sizeof(int));
}

void rdrBeginMarkerThreadFrame(int enable)
{
    rdrCheckThread();
    if ( markerDepth != 0 )
    {
        printf("ERROR: " __FUNCTION__ ": GPU marker nesting is incorrect: markerDepth = %d", markerDepth );
    }
    markerDepth = 0;
    markersEnabled = enable;
}

#endif

