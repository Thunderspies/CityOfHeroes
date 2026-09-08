#define RT_PRIVATE
#include <utilitieslib/utils/utils.h>
#include "render/thread/ogl.h"
#include "render/thread/wcw_statemgmt.h"
#include <utilitieslib/utils/mathutil.h>
#include "render/bump.h"
#include "render/thread/rt_tex.h"
#include "render/thread/rt_model.h"
#include "render/thread/rt_tricks.h"
#include "render/thread/rt_cgfx.h"
#include "render/thread/shadersATI.h"
#include "cmdparse/cmdgame.h"
#include "rt_stats.h"
#include "render/thread/rt_water.h"
#include "render/thread/rt_effects.h"
#include "render/thread/rt_state.h"
#include "render/thread/rt_shadow.h"
#include "render/tex.h"
#include "render/thread/rt_cubemap.h"
#include "render/thread/rt_shadowmap.h"
#include "render/thread/rt_util.h"
#include "graphics/gfxDebug.h"

// This temporary include is for tree shadow hack
#include "seq/anim.h"

#ifndef COLOR_SCALEUB
#define COLOR_SCALEUB(x) (x >> 2)
#endif

#ifndef FINAL
    #define NORMAL_MAP_TEST_PREP()                                    \
        if ( game_state.normalmap_test_mode > 0 ) {                    \
            Vec4 paramVal;                                            \
            paramVal[0] = (float)game_state.normalmap_test_mode;    \
            WCW_SetCgShaderParam4fv( kShaderPgmType_FRAGMENT,        \
                     kShaderParam_TestFlagsFP, paramVal );            \
        }
#else
    #define NORMAL_MAP_TEST_PREP()
#endif


static INLINEDBG void drawElements(int tri_count, int ele_base, int ele_count, int *tris)
{
    MODEL_PERFINFO_AUTO_START("drawElements", 1);
    WCW_PrepareToDraw();
    glDrawElements(GL_TRIANGLES,ele_count,GL_UNSIGNED_INT,&tris[ele_base]); CHECKGL;
    RT_STAT_DRAW_TRIS(tri_count);
    MODEL_PERFINFO_AUTO_STOP();
}

void setupFixedFunctionVertShader(TrickNode *trick)
{
    if (rdr_caps.use_vertshaders)
    {
        Vec4 reflectParameter = {0,0,1,1};
        if (trick && trick->flags1 & TRICK_REFLECT)
        {
            reflectParameter[0] = 1;
            reflectParameter[2] = 0;
        }
        if (trick && trick->flags1 & TRICK_REFLECT_TEX1)
        {
            reflectParameter[1] = 1;
            reflectParameter[3] = 0;
        }
        WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_ReflectionParamVP, reflectParameter );
    }
}

static INLINEDBG void setupReflectionParams(ScrollsScales *scrollsScales, bool bSkinnedModel, F32 reflection_attenuation)
{
    F32 fresnelBias  = 0.0f;
    F32 fresnelScale = 0.0f;
    F32 fresnelPower = 0.0f;
    bool usingDefaultValues = false;

    if (!scrollsScales)
    {
        usingDefaultValues = true;
    }

    if (!game_state.reflectivityOverride && !usingDefaultValues)
    {
        fresnelBias  = scrollsScales->reflectivityBase;
        fresnelScale = scrollsScales->reflectivityScale;
        fresnelPower = scrollsScales->reflectivityPower;

        if (fresnelBias == -1.0f && fresnelScale == -1.0f && fresnelPower == -1.0f)
            usingDefaultValues = true;

        // Support old reflectivity
        // TODO: Remove this
        if (usingDefaultValues && scrollsScales->reflectivity)
        {
            fresnelBias = scrollsScales->reflectivity;

            if (!bSkinnedModel)
            {
                fresnelScale = game_state.buildingFresnelScale;
                fresnelPower = game_state.buildingFresnelPower;
            }
            else
            {
                fresnelScale = game_state.skinnedFresnelScale;
                fresnelPower = game_state.skinnedFresnelPower;
            }

            usingDefaultValues = false;
        }
    }

    if (game_state.reflectivityOverride || usingDefaultValues)
    {
        if (!bSkinnedModel)
        {
            fresnelBias = game_state.buildingFresnelBias;
            fresnelScale = game_state.buildingFresnelScale;
            fresnelPower = game_state.buildingFresnelPower;
        }
        else
        {
            fresnelBias = game_state.skinnedFresnelBias;
            fresnelScale = game_state.skinnedFresnelScale;
            fresnelPower = game_state.skinnedFresnelPower;
        }
    }

    // Attenuate reflectivity
    fresnelBias *= reflection_attenuation;
    fresnelScale *= reflection_attenuation;

    {
        // fresnel bias, scale, power for equation
        // saturate(bias + scale * pow( 1 - (I dot N), power )
        Vec4 fresnelParams =  {fresnelBias, fresnelScale, fresnelPower, game_state.reflection_distortion_strength };
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_FresnelParamsFP, fresnelParams);
    }
}

void drawNormalDir(float* v,float* n)
{
    Vec3    dv;
    scaleVec3(n, game_state.normal_scale, dv);
    addVec3(v,dv,dv);

    glBegin(GL_LINE_LOOP);
    glVertex3f(v[0],v[1],v[2]);
    glVertex3f(dv[0],dv[1],dv[2]);
    glEnd(); CHECKGL;
    RT_STAT_DRAW_TRIS(2)
}

#ifndef FINAL

// This function is used by the gfx debug system to draw colored tangent space vectors
// at each vertex. T-tangent RED, B-binormal GREEN, N-Normal BLUE
// The vbo data is used unless override data is provided (e.g. from skinning)
// N.B. that these won't display unless you have disabled hardware VBOs with '/novbos 1' or something similar
void modelDrawVertexBasisTBN( VBO* vbo, Vec3* override_positions, Vec3* override_normals, Vec3* override_tangents )
{
    int        i;
    bool    bDrawNormals = rdr_view_state.gfxdebug & GFXDEBUG_NORMALS || rdr_view_state.wireframe == 3;
    bool    bDrawTangents = rdr_view_state.gfxdebug & GFXDEBUG_TANGENTS && vbo->tangents;
    bool    bDrawBinormals = rdr_view_state.gfxdebug & GFXDEBUG_BINORMALS && vbo->tangents;    // tangents required to calc binormal

    if (!bDrawNormals && !bDrawTangents && !bDrawNormals)
        return;    // nothing is selected to render

    MODEL_PERFINFO_AUTO_START(__FUNCTION__, 1);

    for(i=0;i<vbo->vert_count;i++)
    {
        // vbo->tangents is a Vec4
        // w component of tangent has the TBN handedness as determined by the
        // geo processing pipeline (e.g. in case of mirrored UVs)
        float handedness = vbo->tangents ? vbo->tangents[i][3] : -1.0f;
        bool bLeftHanded = handedness < 0.0f;

        if ( bLeftHanded && rdr_view_state.gfxdebug & GFXDEBUG_TBN_SKIP_LEFT_HANDED)
            continue;
        if ( !bLeftHanded && rdr_view_state.gfxdebug & GFXDEBUG_TBN_SKIP_RIGHT_HANDED)
            continue;

        {
            Vec3* position    = override_positions    ? &override_positions[i] : &vbo->verts[i];

            // Draw basis axis
            // TBN colored RGB, as is a typical convention
            if ( bDrawNormals )
            {
                Vec3* normal    = override_normals        ? &override_normals[i] : &vbo->norms[i];
                WCW_Color4(0,0,255,255);    // BLUE normal
                drawNormalDir((float*)position,(float*)normal);
            }
            if (bDrawTangents)
            {
                Vec3* tangent    = override_tangents        ? &override_tangents[i] : (Vec3*)(&vbo->tangents[i]);
                WCW_Color4(255,0,0,255);    // RED tangent
                drawNormalDir((float*)position,(float*)tangent);
            }
            if (bDrawBinormals)
            {
                Vec3* normal    = override_normals        ? &override_normals[i] : &vbo->norms[i];
                Vec3* tangent    = override_tangents        ? &override_tangents[i] : (Vec3*)(&vbo->tangents[i]);
                Vec3    binormal;

                // calculate binormal from normal,tangent and handedness
                crossVec3(*tangent,*normal,binormal);
                scaleVec3(binormal, handedness, binormal);
                normalVec3(binormal);

                WCW_Color4(0,255,0,255);    // GREEN binormal
                drawNormalDir((float*)position,(float*)&binormal);
            }
            WCW_Color4(255,255,255,255);
        }
    }
    MODEL_PERFINFO_AUTO_STOP();
}

#endif // FINAL

static void modelDrawLightDirs(VBO* vbo, Vec3* verts, Vec4 lightdir)
{
    int        i;
    MODEL_PERFINFO_AUTO_START(__FUNCTION__, 1);

    modelDrawState( DRAWMODE_COLORONLY, FORCE_SET);
    modelBlendState(BlendMode(BLENDMODE_MODULATE,0), ONLY_IF_NOT_ALREADY);

    for(i=0;i<vbo->vert_count;i++)
    {
        drawNormalDir(verts[i],lightdir);
    }
    MODEL_PERFINFO_AUTO_STOP();
}

_inline static int isBorder(VBO *vbo, int a, int b)
{
    int        i,count,edges=0;

    count = vbo->tri_count * 3;
    for (i = 0; i < count; i += 3)
    {
        if (vbo->tris[i] == a || vbo->tris[i+1] == a || vbo->tris[i+2] == a)
        {
            if (vbo->tris[i] == b || vbo->tris[i+1] == b || vbo->tris[i+2] == b)
                edges++;
        }
    }

    return edges == 1;
}

static INLINEDBG void drawWireframeSeam(VBO *vbo, int a, int b)
{
    F32        *v;

    if (isBorder(vbo, vbo->tris[a], vbo->tris[b])) {
        WCW_Color4(100,255,128,255);
    } else {
        WCW_Color4(100,128,255,255);
    }

    glBegin(GL_LINES);
    v = vbo->verts[vbo->tris[a]];
    glVertex3f(v[0],v[1],v[2]);

    v = vbo->verts[vbo->tris[b]];
    glVertex3f(v[0],v[1],v[2]);
    glEnd(); CHECKGL;
}

static void modelDrawWireframeSeams( VBO *vbo )
{
    int        i,count;
    MODEL_PERFINFO_AUTO_START(__FUNCTION__, 1);

    modelDrawState(DRAWMODE_COLORONLY, FORCE_SET);
    modelBlendState(BlendMode(BLENDMODE_MODULATE,0), ONLY_IF_NOT_ALREADY);
    WCW_PrepareToDraw();
    glLineWidth( 1.0 ); CHECKGL;

    count = vbo->tri_count * 3;
    for(i=0;i<count;i+=3)
    {
        drawWireframeSeam(vbo, i, i+1);
        drawWireframeSeam(vbo, i+1, i+2);
        drawWireframeSeam(vbo, i+2, i);
        RT_STAT_DRAW_TRIS(3)
    }
    MODEL_PERFINFO_AUTO_STOP();
}


static void modelDrawWireframe(VBO *vbo,U8 *color,int tex_index)
{
    int i,ele_count,tex_count=1,ele_base=0;
    if (!vbo->norms)
        return;
    MODEL_PERFINFO_AUTO_START(__FUNCTION__, 1);
    WCW_Fog(0);
    //TO DO 1/15/03 MArtin figured out that this state (DRAWMODE_COLORONLY) is getting messed up somewhere
    //resetting it each time fixes it, but somebody needs to figure out where the state is 
    //getting messed up.  The symptom is that commenting this if back in causes the lines to be too dark.
    //if (curr_draw_state != DRAWMODE_COLORONLY)

    modelDrawState(DRAWMODE_COLORONLY, FORCE_SET);
    modelBlendState(BlendMode(BLENDMODE_MODULATE,0), ONLY_IF_NOT_ALREADY);
    glLineWidth( 1.0 ); CHECKGL;
    WCW_Color4(color[0],color[1],color[2],color[3]);

    if (modelBindBuffer(vbo))
    {
        WCW_VertexPointer(3,GL_FLOAT,0,vbo->verts);
    }

    glDisable(GL_CULL_FACE); CHECKGL;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); CHECKGL;

    WCW_PrepareToDraw();

    tex_count = vbo->tex_count;
    for (i=0; i<tex_count; i++)
    {
        ele_count = vbo->tex_ids[i].count*3;
        if (tex_index==-1 || tex_index==i)
            drawElements(vbo->tex_ids[i].count, ele_base, ele_count, vbo->tris);
        ele_base += ele_count;
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); CHECKGL;
    glEnable(GL_CULL_FACE); CHECKGL;
    WCW_Fog(1);
    MODEL_PERFINFO_AUTO_STOP();
}

static INLINEDBG void drawLoopInline(VBO *vbo,RdrTexList *texlists, U32 texlist_size, int tex_index)
{
    int        i,ele_base = 0;
    int ele_count;
    int * tris;
    int tex_count;
    TexID * texIDs;

    tris  = vbo->tris;
    tex_count = vbo->tex_count;
    texIDs = vbo->tex_ids;
    if (!vbo->sts)
        return;

    assert(!blendModeHasBump(curr_blend_state));

    for(i=0;i<tex_count;i++)
    {
        ele_count = texIDs[i].count*3;
        if (tex_index==-1||tex_index==i)
        {
            rdrBeginMarker("TEX:%d", i);

            if (texlist_size)
            {
                RdrTexList *texlist;
                U32 texlist_index = (tex_index==-1)?i:0;

                assert(texlist_index < texlist_size);

                texlist = &texlists[texlist_index];

                texBind(TEXLAYER_BASE, texlist->texid[TEXLAYER_BASE]);
                texBind(TEXLAYER_GENERIC, texlist->texid[TEXLAYER_GENERIC]);
            }

            drawElements(texIDs[i].count, ele_base, ele_count, tris);

            rdrEndMarker();
        }
        ele_base += ele_count;
    }
}

static INLINEDBG void drawLoopInlineNoTextures(VBO *vbo, int tex_index)
{
    int        i,ele_base = 0;
    int ele_count;
    int * tris;
    int tex_count;
    TexID * texIDs;

    tris  = vbo->tris;
    tex_count = vbo->tex_count;
    texIDs = vbo->tex_ids;
    if (!vbo->sts)
        return;

    assert(!blendModeHasBump(curr_blend_state));

    if( tex_count == 1 || tex_index == 0 )
    {
        // shortcut for just one texture, draw the whole thing always
        assert(tex_index <= 0); // shouldnt be asking for a texture higher than exists in the vbo
        drawElements(texIDs[0].count, 0, texIDs[0].count*3, tris);
    }
    else if(tex_index != -1)
    {
        // just draw a single submesh
        for(i=0;i<tex_count;i++)
        {
            ele_count = texIDs[i].count*3;
            if (tex_index==i)
            {
                drawElements(texIDs[i].count, ele_base, ele_count, tris);
                break;
            }
            ele_base += ele_count;
        }
    }
    else if (game_state.shadowDebugFlags&512)
    {
        // drawing the entire model either as one material (e.g all white past fog, or into shadow map)
        // we can do the entire model in one draw call
        int totalTris = vbo->tri_count;
        ele_count = totalTris*3;
        drawElements(totalTris, 0, ele_count, tris);
    }
    else
    {
        for(i=0;i<tex_count;i++)
        {
            ele_count = texIDs[i].count*3;
            drawElements(texIDs[i].count, ele_base, ele_count, tris);
            ele_base += ele_count;
        }
    }
}

void modelDrawSolidColor(VBO *vbo, U32 color)
{
    MODEL_PERFINFO_AUTO_START(__FUNCTION__, 1);

    modelDrawState(DRAWMODE_COLORONLY, ONLY_IF_NOT_ALREADY);
    modelBlendState(BlendMode(BLENDMODE_MODULATE,0), ONLY_IF_NOT_ALREADY);

    if (modelBindBuffer(vbo))
        WCW_VertexPointer(3,GL_FLOAT,0,vbo->verts);

    WCW_ColorU32(color);

    drawLoopInlineNoTextures(vbo,-1);

    MODEL_PERFINFO_AUTO_STOP();
}

void modelDrawDepthOnly(RdrModel *draw, TrickNode * trick)
{
    VBO *vbo = draw->vbo;
    int alphaTest = (trick && ((trick->flags1 & TRICK_ALPHACUTOUT) || (trick->flags2 & TRICK2_ALPHAREF)));

    MODEL_PERFINFO_AUTO_START(__FUNCTION__, 1);

    modelDrawState(alphaTest ? DRAWMODE_DEPTHALPHA_ONLY : DRAWMODE_DEPTH_ONLY, ONLY_IF_NOT_ALREADY);
    modelBlendState(BlendMode(BLENDMODE_MODULATE,0), ONLY_IF_NOT_ALREADY);

    if (modelBindBuffer(vbo)) {
        if(alphaTest)
            texBindTexCoordPointer(TEXLAYER_BASE,vbo->sts);
        WCW_VertexPointer(3,GL_FLOAT,0,vbo->verts);
    }

    // Now done in rt_shadowViewportPreCallback()
    WCW_Color4(    255, 255, 255, 255);

    drawLoopInlineNoTextures(vbo,draw->tex_index);

    MODEL_PERFINFO_AUTO_STOP();
}

static void modelDrawTexRgb(RdrModel *draw, U8 *rgbs, RdrTexList *texlist, BlendModeType blend_mode, TrickNode *trick, F32 reflection_attenuation)
{
    VBO *vbo = draw->vbo;

    // We have to have normals to have a reflection map!  Even if we're not doing lighting
    int enable_normals = trick && (trick->flags1 & (TRICK_REFLECT | TRICK_REFLECT_TEX1));

    rdrBeginMarker(__FUNCTION__);

    MODEL_PERFINFO_AUTO_START(__FUNCTION__, 1);

    modelDrawState(DRAWMODE_DUALTEX, ONLY_IF_NOT_ALREADY);
    setupFixedFunctionVertShader(trick);
    WCW_EnableGL_LIGHTING(0);

    //modelBindDefaultBuffer();
    //glColorPointer(4,GL_UNSIGNED_BYTE,0,rgbs); CHECKGL;

    //if (tint_color) // this does not work because we're passing a color pointer, we need a new vertex program to do this
    //    glColor3ubv(tint_color); CHECKGL;

    if (enable_normals) {
        WCW_EnableClientState(GLC_NORMAL_ARRAY);
    }

    if (modelBindBuffer(vbo))
    {
        texBindTexCoordPointer(TEXLAYER_GENERIC,vbo->sts2);
        texBindTexCoordPointer(TEXLAYER_BASE,vbo->sts);
        WCW_VertexPointer(3,GL_FLOAT,0,vbo->verts);
        // This NormalPointer call has to be done all the time becase
        //  modelBindBuffer might get called on the same model later, with a different draw
        //  mode that needs normals.  TODO: statemanage this better so that we don't have to
        //  send normals here (and if normals are disabled they might not make it down the
        //  pipe anyway?
        WCW_NormalPointer(GL_FLOAT, 0, vbo->norms);
    }

    if (rdr_caps.use_vbos) {
        WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, (int)rgbs);
        if (rdr_caps.rgbbuffers_as_floats_hack) {
            glColorPointer(3,GL_FLOAT,0,0); CHECKGL;
        } else {
            glColorPointer(4,GL_UNSIGNED_BYTE,0,0); CHECKGL;
        }
        WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, vbo->vertex_array_id);
    } else {
        glColorPointer(4,GL_UNSIGNED_BYTE,0,rgbs); CHECKGL;
    }

    drawLoopInline(vbo, texlist, draw->texlist_size, draw->tex_index);

    if (enable_normals) {
        WCW_DisableClientState(GLC_NORMAL_ARRAY); // We have to have normals to have a reflection map!
    }

    MODEL_PERFINFO_AUTO_STOP();

    rdrEndMarker();
}

static INLINEDBG void modelDrawTexNormals(RdrModel *draw, RdrTexList *texlist, BlendModeType blend_mode, TrickNode *trick, F32 reflection_attenuation)
{
    VBO *vbo = draw->vbo;

    rdrBeginMarker(__FUNCTION__);
    MODEL_PERFINFO_AUTO_START(__FUNCTION__, 1);

    // This simple material may have been 'upgraded' to use per pixel lighting
    // if that is the case then use the appropriate vertex shader to feed the
    // necessary information down to the pixel shader
    // like multiply to do per pixel lighting so shadows look better
    if (blend_mode.blend_bits&BMB_HIGH_QUALITY)
    {
        modelDrawState(DRAWMODE_DUALTEX_LIT_PP, ONLY_IF_NOT_ALREADY);
    }
    else
    {
        modelDrawState(DRAWMODE_DUALTEX_NORMALS, ONLY_IF_NOT_ALREADY);
    }

    setupFixedFunctionVertShader(trick);
    if (vbo->flags & OBJ_NOLIGHTANGLE) //this seems a bit flaky.
    {
        Vec4 amb;
        scaleVec3(rdr_view_state.sun_no_angle_light, draw->brightScale, amb);
        amb[3] = rdr_view_state.sun_no_angle_light[3];
        WCW_Light(GL_LIGHT0, GL_AMBIENT, amb);
        WCW_Light(GL_LIGHT0, GL_DIFFUSE, zerovec3);

        // since there is no diffuse lighting here we need to have the shadows attenuate the ambient
        // much more if we want cast shadows to show up on these objects
        rt_shadowmap_override_ambient_clamp();
    }
    else if (draw->has_lightdir)
    {
        //notice goofy GL thing where sun.direction[3] == 0 means this is a directional light, not a positional light at all
        WCW_LightPosition(draw->lightdir, rdr_view_state.viewmat);
    } else
        WCW_SunLightDir();

    if (draw->has_fx_tint) {
        WCW_Color4(draw->tint_colors[0][0], draw->tint_colors[0][1], draw->tint_colors[0][2], 255);
    }

    if( blend_mode.shader == BLENDMODE_MULTIPLY && (blend_mode.blend_bits&BMB_CUBEMAP) )
        setupReflectionParams(texlist->scrollsScales, false, reflection_attenuation);

    if (modelBindBuffer(vbo))
    {
        if (vbo->sts2)
            texBindTexCoordPointer(TEXLAYER_GENERIC,vbo->sts2);
        texBindTexCoordPointer(TEXLAYER_BASE,vbo->sts);
        WCW_VertexPointer(3,GL_FLOAT,0,vbo->verts);
        WCW_NormalPointer(GL_FLOAT, 0, vbo->norms);
    }

    drawLoopInline(vbo, texlist, draw->texlist_size, draw->tex_index);

    if (vbo->flags & OBJ_NOLIGHTANGLE)
    {
        rt_shadowmap_restore_viewport_ambient_clamp();
    }

    if (draw->has_fx_tint) {
        WCW_Color4(255,255,255,255);
    }

    MODEL_PERFINFO_AUTO_STOP();
    rdrEndMarker();
}

FORCEINLINE F32 diffuseBasedGlossScale(Vec4 diffuse)
{
    F32 diffuse_sum = diffuse[0]+diffuse[1]+diffuse[2];
#define BOTTOM_CAP 0.28
#define BOTTOM_VAL 0.30
#define TOP_CAP 0.38
    if (diffuse_sum < BOTTOM_CAP) {
        return BOTTOM_VAL;
    } else if (diffuse_sum < TOP_CAP) {
        // Scale gloss down as things get awfully dark
        return BOTTOM_VAL + (diffuse_sum - BOTTOM_CAP)*((1-BOTTOM_VAL)/(TOP_CAP-BOTTOM_CAP));
    }
    return 1.0;
}

FORCEINLINE static void setupSpecularColor(ScrollsScales *scrollsScales, Vec4 directLightColor)
{
    Vec4 specularParam1;
    Vec4 specularParam2;
    Vec3 adjustedLightColor;

    switch (game_state.specLightMode) {
    case 1:
        copyVec3(directLightColor, adjustedLightColor);
    xcase 2:
        {
            F32 scale = MAX(directLightColor[0], directLightColor[1]);
            scale = MAX(scale, directLightColor[2]);
            if (scale == 0.f)
                setVec3(adjustedLightColor, 1.f, 1.f, 1.f);
            else {
                scale = 1.f / scale;
                scaleVec3(directLightColor, scale, adjustedLightColor);
            }
        }
    xdefault:
        setVec3(adjustedLightColor, 1.f, 1.f, 1.f);
    } 

    if (scrollsScales) {
        scaleVec3(scrollsScales->specularRgba1, 1.f/255.f, specularParam1);
        mulVecVec3(specularParam1, adjustedLightColor, specularParam1);
        specularParam1[3]=scrollsScales->specularExponent1;
        scaleVec3(scrollsScales->specularRgba2, 1.f/255.f, specularParam2);
        mulVecVec3(specularParam2, adjustedLightColor, specularParam2);
        specularParam2[3]=scrollsScales->specularExponent2;
    } else {
        copyVec3(adjustedLightColor, specularParam1);
        specularParam1[3] = 8;
        copyVec3(adjustedLightColor, specularParam2);
        specularParam2[3] = 8;
    }
    if (rdr_caps.chip & ARBFP) {
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Specular1ColorAndExponentFP, specularParam1);  // Specular color+exponent
        if (curr_blend_state.shader == BLENDMODE_MULTI && !(curr_blend_state.blend_bits & BMB_SINGLE_MATERIAL))
            WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_Specular2ColorAndExponentFP, specularParam2);  // Specular color+exponent
    } else if (rdr_caps.chip & NV2X) {
        specularParam1[3]=0; // Not used
        if (curr_blend_state.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL) {
            glCombinerStageParameterfvNV(GL_COMBINER2_NV,GL_CONSTANT_COLOR0_NV,specularParam1); CHECKGL;
        } else if (curr_blend_state.shader == BLENDMODE_BUMPMAP_MULTIPLY) {
            WCW_ConstantCombinerParamerterfv(1, specularParam1);
        }
    }
}

void setupBumpPixelShader(Vec4 ambient, Vec4 diffuse, Vec4 lightdir, RdrTexList *texlist, BlendModeType blend_mode, bool bSkinnedModel, bool scaleLighting, F32 reflection_attenuation)
{
    F32            glossScale;
    Vec4        glossParameter={0}, ambientParameter={0}, diffuseParameter={0};
    ScrollsScales *scrollsScales = texlist->scrollsScales;

    // Set lighting color and combiner colors for Pixel Shader
    glossParameter[3] = texlist->gloss1 * rdr_view_state.gloss_scale; 
    glossScale = diffuseBasedGlossScale(diffuse);
    glossParameter[3] *= glossScale;

    scaleVec3(ambient,2.f,ambientParameter);
    scaleVec3(diffuse,4.f,diffuseParameter);
    if (scaleLighting && scrollsScales) {
        mulVecVec3(ambientParameter, scrollsScales->ambientScale, ambientParameter);
        MAXVEC3(ambientParameter, scrollsScales->ambientMin, ambientParameter);
        mulVecVec3(diffuseParameter, scrollsScales->diffuseScale, diffuseParameter);
    }
//    MINVEC3(ambientParameter,onevec3,ambientParameter);
//    MINVEC3(diffuseParameter,onevec3,diffuseParameter);

    if (rdr_caps.chip & ARBFP)
    {
        if( blend_mode.blend_bits & (BMB_CUBEMAP|BMB_PLANAR_REFLECTION) )
            setupReflectionParams(texlist->scrollsScales, bSkinnedModel, reflection_attenuation);

        if( blend_mode.blend_bits & BMB_HIGH_QUALITY )
            WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_LightDirFP, lightdir );

        // Set fragment program local parameters
        glossParameter[0] = glossParameter[1] = glossParameter[2] = glossParameter[3];

        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_AmbientColorFP, ambientParameter);  // Ambient color
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_DiffuseColorFP, diffuseParameter);  // Diffuse color
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_GlossParamFP, glossParameter);  // Gloss factor
        setupSpecularColor(texlist->scrollsScales, diffuseParameter);
    }
    else if (rdr_caps.chip & NV1X)
    {
        glEnable(GL_PER_STAGE_CONSTANTS_NV); CHECKGL; // State manage?
        glCombinerStageParameterfvNV(GL_COMBINER1_NV,GL_CONSTANT_COLOR0_NV,glossParameter); CHECKGL;
        setupSpecularColor(texlist->scrollsScales, diffuseParameter);
        glCombinerStageParameterfvNV(GL_COMBINER3_NV,GL_CONSTANT_COLOR1_NV,ambientParameter); CHECKGL;
        glCombinerStageParameterfvNV(GL_COMBINER3_NV,GL_CONSTANT_COLOR0_NV,diffuseParameter); CHECKGL;
        // these are the two per-sock blend colors
        glCombinerStageParameterfvNV(GL_COMBINER5_NV,GL_CONSTANT_COLOR0_NV,constColor0); CHECKGL;
        glCombinerStageParameterfvNV(GL_COMBINER5_NV,GL_CONSTANT_COLOR1_NV,constColor1); CHECKGL;
    }
    else if (rdr_caps.chip & R200)
    {
        atiDiffuseColorScale(diffuseParameter, ambientParameter);
        atiAmbientColorScale(ambientParameter);
        glSetFragmentShaderConstantATI(GL_CON_0_ATI, glossParameter); CHECKGL;
        glSetFragmentShaderConstantATI(GL_CON_1_ATI, ambientParameter); CHECKGL;
        glSetFragmentShaderConstantATI(GL_CON_2_ATI, diffuseParameter); CHECKGL;
        // these are the two per-sock blend colors
        glSetFragmentShaderConstantATI(GL_CON_3_ATI, constColor0); CHECKGL;
        glSetFragmentShaderConstantATI(GL_CON_4_ATI, constColor1); CHECKGL;
    }
}

static void drawLoopBumpDual(VBO *vbo, RdrModel *draw, Vec4 lightdir, RdrTexList *texlists, TrickNode *trick,
                                BlendModeType blend_mode, F32 reflection_attenuation)
{
    int i,ele_count,tex_count=1,ele_base=0;
    int tex_index = draw->tex_index;
    ModelStateSetFlags    dsFlags;

    // This is virtually the same code as what draws capes

    tex_count = vbo->tex_count;

    WCW_TexLODBias(0, -2.0);
    WCW_TexLODBias(1, -2.0);

    dsFlags = ONLY_IF_NOT_ALREADY;
    // if high quality fragment program make sure we select hq vertex program to feed it
    if (blend_mode.blend_bits&BMB_HIGH_QUALITY)
        dsFlags |= BIT_HIGH_QUALITY_VERTEX_PROGRAM;

    modelDrawState( DRAWMODE_BUMPMAP_DUALTEX, dsFlags );
    modelBlendState( BlendMode(BLENDMODE_BUMPMAP_COLORBLEND_DUAL,blend_mode.blend_bits), ONLY_IF_NOT_ALREADY);

    // Vertex shader inputs
    WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_LightDirVP, lightdir );

    for(i = 0 ; i < tex_count ; i++) 
    {
        ele_count = vbo->tex_ids[i].count*3;
        if (tex_index==-1 || tex_index==i)
        {
            RdrTexList *texlist;
            U32 texlist_index = (tex_index==-1)?i:0;

            assert(texlist_index < draw->texlist_size);

            texlist = &texlists[texlist_index];

            texBind(TEXLAYER_BASE, texlist->texid[TEXLAYER_BASE]);
            texBind(TEXLAYER_GENERIC, texlist->texid[TEXLAYER_GENERIC]);

            if (texlist->texid[TEXLAYER_BUMPMAP1])
                texBind(TEXLAYER_BUMPMAP1, texlist->texid[TEXLAYER_BUMPMAP1]);
            else
                texBind(TEXLAYER_BUMPMAP1, rdr_view_state.dummy_bump_tex_id);

            if( blend_mode.blend_bits & BMB_CUBEMAP && texlist->texid[TEXLAYER_CUBEMAP])
                texBindCube(TEXLAYER_CUBEMAP, texlist->texid[TEXLAYER_CUBEMAP]);

            // Pixel shader inputs 
            setupBumpPixelShader(draw->ambient, draw->diffuse, lightdir, texlist, blend_mode, false, false, reflection_attenuation);
            drawElements(vbo->tex_ids[i].count, ele_base, ele_count, vbo->tris);
        }
        ele_base += ele_count;
    }
}


static void drawLoopBump(VBO *vbo, Vec4 ambient, Vec4 diffuse, RdrTexList *texlists, U32 texlist_size, int tex_index, BlendModeType blend_mode, TrickNode *trick, int is_gfxnode)
{
    Vec4    ambientParameter={0}, diffuseParameter={0};
    int        i,ele_base = 0;
    int ele_count;
    int * tris;
    int tex_count;
    TexID * texIDs;

    tris  = vbo->tris;
    tex_count = vbo->tex_count;
    texIDs = vbo->tex_ids;

    // Varying vertex shader inputs
    copyVec3(ambient,ambientParameter);
    copyVec3(diffuse,diffuseParameter);
    MINVEC3(ambientParameter,onevec3,ambientParameter);
    MINVEC3(diffuseParameter,onevec3,diffuseParameter);
    diffuseParameter[3] = ambientParameter[3] = 1;
    WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_AmbientParameterVP, ambientParameter);
    WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_DiffuseParameterVP, diffuseParameter);

    if ( (rdr_caps.chip & ARBFP) && (blend_mode.blend_bits&BMB_HIGH_QUALITY) )
    {
        // If we are upgrading BUMPMAP_MULTIPLY from vertex lighting and per pixel specular bump
        // to full per pixel lighting then set the fragment program constants appropriately
        // @todo also see comment below that maybe we should be doing more along lines of what setupBumpPixelShader() does?

        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_AmbientColorFP, ambientParameter);  // Ambient color
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_DiffuseColorFP, diffuseParameter);  // Diffuse color
    }

    for(i=0;i<tex_count;i++)
    {
        ele_count = texIDs[i].count*3;
        if (tex_index==-1 || tex_index==i)
        {
            RdrTexList *texlist;
            ScrollsScales *scrollsScales;
            U32 texlist_index = (tex_index==-1)?i:0;

            rdrBeginMarker("TEX:%d", i);

            assert(texlist_index < texlist_size);

            texlist = &texlists[texlist_index];
            scrollsScales = texlist->scrollsScales;

            // Varying pixel shader inputs (bumpMultiply)
            setupSpecularColor(texlist->scrollsScales, diffuseParameter);

            texBind(TEXLAYER_BASE, texlist->texid[TEXLAYER_BASE]);
            texBind(TEXLAYER_GENERIC, texlist->texid[TEXLAYER_GENERIC]);

            if (texlist->texid[TEXLAYER_BUMPMAP1]==0)
                texBind(TEXLAYER_BUMPMAP1, rdr_view_state.dummy_bump_tex_id);
            else
                texBind(TEXLAYER_BUMPMAP1, texlist->texid[TEXLAYER_BUMPMAP1]);

            // @todo - to be consistent for the per pixel bump lighting on
            // BUMPMAP_MULTIPLY we should probably be doing more of the same kinds
            // of settings that setupBumpPixelShader() does?
            // e.g., probably should use gloss factor but this has been 1.0 down
            // this path historically

            // @todo set gloss factor which may have been adjusted by the bumpmap's texture trick?

            drawElements(texIDs[i].count, ele_base, ele_count, tris);

            rdrEndMarker();
        }
        ele_base += ele_count;
    }
}

static void calcLightDir(Vec4 lightdir, const RdrModel* draw, U8* rgbs)
{
    Vec4 lightdir_src = { 0.70710677,0.70710677,0,0};
    lightdir[3] = 0;
    if(draw->has_lightdir)
        copyVec4(draw->lightdir, lightdir);
    else if (rgbs) // Indoor, we don't have accurate light information for this object
        mulVecMat3(lightdir_src, rdr_view_state.viewmat, lightdir);
    else // Outdoor
        copyVec3(rdr_view_state.sun_direction_in_viewspace, lightdir);
}


void modelDrawBump(RdrModel *draw, U8 * rgbs, RdrTexList *texlist, BlendModeType blend_mode, TrickNode *trick, F32 reflection_attenuation)
{
    VBO *vbo = draw->vbo;
    extern Vec4    tex_scrolls[4];

    rdrBeginMarker(__FUNCTION__);

    // It's drawn through a different codepath if we don't have bumpmaps
    assert((rdr_caps.features & GFXF_BUMPMAPS)); // TEXTODO make sure this doesn't go off

    if (!vbo->sts) // FRK_Juicer_Pak and other bad objects, causes crash if not using VBOs
        return;

    MODEL_PERFINFO_AUTO_START(__FUNCTION__, 1);

    modelBindBuffer(vbo);

    assert(curr_blend_state.intval == blend_mode.intval);

    /////Regular Array Pointers//////////////////////////////////////////////////////////
    texBindTexCoordPointer(TEXLAYER_BASE, vbo->sts);
    texBindTexCoordPointer(TEXLAYER_GENERIC, vbo->sts2);
    WCW_NormalPointer(GL_FLOAT, 0, vbo->norms);
    WCW_VertexPointer(3,GL_FLOAT,0, vbo->verts);

    /////Special Bumpmapping Array Pointers///////////////////////////////////////
    devassert(vbo->tangents);

    glVertexAttribPointerARB( 7, 4, GL_FLOAT, GL_FALSE, 0, vbo->tangents ); CHECKGL;

    if (draw->has_fx_tint) {
        int i;
        for (i=0; i<3; i++) {
            draw->diffuse[i] = draw->diffuse[i]*draw->tint_colors[0][i]/255;
            draw->ambient[i] = draw->ambient[i]*draw->tint_colors[0][i]/255;
        }
    }

    if (blend_mode.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL)
    {
        Vec4 lightdir;

        calcLightDir(lightdir, draw, rgbs);
        drawLoopBumpDual(vbo, draw, lightdir, texlist, trick, blend_mode, reflection_attenuation);
    }
    else if (rgbs)
    {
        // ambient group bumpmap multiply
        // Use baked vertex lighting from ambient group and just bump specular

        modelDrawState(DRAWMODE_BUMPMAP_RGBS, ONLY_IF_NOT_ALREADY);

        { // set viewer position so we can convert to model space in shader
            Mat4    matx;
            transposeMat4Copy(draw->mat,matx);
            WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_ViewerPositionVP /*was C2*/, matx[3] );
        }

        {
            // Static lighting, and we don't know where the light's coming from
            // But, we want a light position in model space!
            Mat4    m2;
            Vec4    light;
            Vec4    dummy_pos = {0, 5000, 0, 0};
            mulMat4(rdr_view_state.inv_viewmat,draw->mat,m2);
            mulVecMat4Transpose(dummy_pos,m2,light);
            light[3] = 1;
            WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_LightDirVP, light);
        }

        WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_TexScroll0VP, tex_scrolls[0] );
        WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_TexScroll1VP, tex_scrolls[1] );

        if (rdr_caps.use_vbos)
        {
            WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, (int)rgbs);
            glVertexAttribPointerARB( 11, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0 ); CHECKGL;
            WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, vbo->vertex_array_id);
        }
        else    
        {
            WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, 0);
            glVertexAttribPointerARB( 11, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, rgbs ); CHECKGL;
        }
        drawLoopBump(vbo, draw->ambient, draw->diffuse, texlist, draw->texlist_size, draw->tex_index,
            blend_mode, trick, draw->is_gfxnode);
    }
    else
    {
        // lit bumpmap multiply, choose legacy vertex lit w/ specular bump or
        // full per pixel bumped lighting (high quality)
        ModelStateSetFlags    dsFlags = ONLY_IF_NOT_ALREADY;

        // if high quality fragment program make sure we select HQ vertex program to feed it
        if (blend_mode.blend_bits&BMB_HIGH_QUALITY)
        {
            // In HQ mode we do everything in view space like the other materials
            Vec4 lightdir;

            dsFlags |= BIT_HIGH_QUALITY_VERTEX_PROGRAM;
            modelDrawState(DRAWMODE_BUMPMAP_NORMALS_PP, dsFlags);

            calcLightDir(lightdir, draw, rgbs);
            WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX,   kShaderParam_LightDirVP, lightdir );
            WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_LightDirFP, lightdir );
        }
        else
        {
            // legacy material mode
            // bumpmap_multiply normally works in model space, if there is a reason for
            // that distinction I'm not aware of it, may be historical with initially using
            // object based normal maps instead of tangent basis?

            modelDrawState(DRAWMODE_BUMPMAP_NORMALS, dsFlags);

            { // set viewer position so we can convert to model space in shader
                Mat4    matx;
                transposeMat4Copy(draw->mat,matx);
                WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_ViewerPositionVP /*was C2*/, matx[3] );
            }

            {
                Mat4    m;
                Vec4    light;
                mulMat4(rdr_view_state.inv_viewmat,draw->mat,m);
                mulVecMat4Transpose(rdr_view_state.sun_position,m,light);
                light[3] = 1;
                WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_LightDirVP, light);
            }
        }

        WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_TexScroll0VP, tex_scrolls[0] );
        WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_TexScroll1VP, tex_scrolls[1] );

        drawLoopBump(vbo, draw->ambient, draw->diffuse, texlist, draw->texlist_size, draw->tex_index,
            blend_mode, trick, draw->is_gfxnode);
    }
    MODEL_PERFINFO_AUTO_STOP();
    rdrEndMarker();
}

FORCEINLINE void setupScrolls(ScrollsScales *scrollsScales, TexLayerIndex ti, Vec4 scrollsScalesParameter)
{
    BlendIndex bi = texLayerIndexToBlendIndex(ti);
    switch (scrollsScales->texopt_scrollType[bi]) {
        xcase TEXOPTSCROLL_NORMAL:
            if (scrollsScales->texopt_scroll[bi][0]!=0) {
                scrollsScalesParameter[0] = scrollsScales->texopt_scroll[bi][0] * rdr_view_state.client_loop_timer;
                scrollsScalesParameter[0] -= round(scrollsScalesParameter[0]); // Range of -1-1
            } else 
                scrollsScalesParameter[0] = 0;
            if (scrollsScales->texopt_scroll[bi][1]!=0) { 
                scrollsScalesParameter[1] = scrollsScales->texopt_scroll[bi][1] * rdr_view_state.client_loop_timer;
                if (scrollsScalesParameter[1]!=0)
                    scrollsScalesParameter[1] -= round(scrollsScalesParameter[1]); // Range of -1-1
            } else
                scrollsScalesParameter[1] = 0;
        xcase TEXOPTSCROLL_PINGPONG:
            scrollsScalesParameter[0] = scrollsScales->texopt_scroll[bi][0] * rdr_view_state.client_loop_timer;
            scrollsScalesParameter[0] -= ((int)scrollsScalesParameter[0]) & ~1; // Range of 0-2 or -1-1 if negative
            if (scrollsScalesParameter[0] < 0)
                scrollsScalesParameter[0] = -scrollsScalesParameter[0];
            else if (scrollsScalesParameter[0] > 1)
                scrollsScalesParameter[0] = 2 - scrollsScalesParameter[0];
            scrollsScalesParameter[1] = scrollsScales->texopt_scroll[bi][1] * rdr_view_state.client_loop_timer;
            scrollsScalesParameter[1] -= ((int)scrollsScalesParameter[1]) & ~1; // Range of 0-2 or -1-1 if negative
            if (scrollsScalesParameter[1] < 0)
                scrollsScalesParameter[1] = -scrollsScalesParameter[1];
            else if (scrollsScalesParameter[1] > 1)
                scrollsScalesParameter[1] = 2 - scrollsScalesParameter[1];
        xcase TEXOPTSCROLL_OVAL:
            // TEXTODO: eliminate this cosine or something
            scrollsScalesParameter[0] = scrollsScales->texopt_scroll[bi][0] * cos(rdr_view_state.client_loop_timer*0.1);
            scrollsScalesParameter[1] = scrollsScales->texopt_scroll[bi][1] * sin(rdr_view_state.client_loop_timer*0.1);
            scrollsScalesParameter[0] -= round(scrollsScalesParameter[0]); // Range of -1-1
            scrollsScalesParameter[1] -= round(scrollsScalesParameter[1]); // Range of -1-1
    }
    scrollsScalesParameter[2] = scrollsScales->texopt_scale[bi][0];
    scrollsScalesParameter[3] = scrollsScales->texopt_scale[bi][1];
}

void setupBumpMultiVertShader(RdrTexList *texlist, Vec4 lightdir)
{
    WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_LightDirVP, lightdir );
    if (texlist->scrollsScales) {
        Vec4 reflectParameter;
        reflectParameter[0] = texlist->scrollsScales->multiply1Reflect;
        reflectParameter[1] = texlist->scrollsScales->multiply2Reflect;
        reflectParameter[2] = !texlist->scrollsScales->multiply1Reflect;
        reflectParameter[3] = !texlist->scrollsScales->multiply2Reflect;
        WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_ReflectionParamVP, reflectParameter );
    } else {
        Vec4 reflectParameter={0};
        WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_ReflectionParamVP, reflectParameter );
    }
}

void setupBumpMultiPixelShader(Vec4 ambient, Vec4 diffuse, const Vec4 lightdir, RdrTexList *texlist, BlendModeType blend_mode,
                                bool static_lighting, bool scaleLighting, bool bSkinnedModel, F32 reflection_attenuation)
{
    int i;
    Vec4        glossParameter={0}, ambientParameter={0}, diffuseParameter={0}, scrollsScalesParameter={0,0,1,1}, alphaMask={0};
    F32            glossScale;
    ScrollsScales *scrollsScales = texlist->scrollsScales;

    assert(rdr_caps.chip & ARBFP);

    // Set lighting color and combiner colors for Pixel Shader
    glossParameter[0] = texlist->gloss1 * rdr_view_state.gloss_scale; 
    glossParameter[1] = texlist->gloss2 * rdr_view_state.gloss_scale; 
    glossScale = diffuseBasedGlossScale(diffuse);
    if (glossScale != 1) {
        glossParameter[0] *= glossScale;
        glossParameter[1] *= glossScale;
    }

    if (static_lighting) {
        setVec3(ambientParameter, 1, 1, 1);
        setVec3(diffuseParameter, 0, 0, 0);
    } else {
        scaleVec3(ambient,2.f,ambientParameter);
        scaleVec3(diffuse,4.f,diffuseParameter);
        if (scaleLighting && scrollsScales) {
            mulVecVec3(ambientParameter, scrollsScales->ambientScale, ambientParameter);
            MAXVEC3(ambientParameter, scrollsScales->ambientMin, ambientParameter);
            mulVecVec3(diffuseParameter, scrollsScales->diffuseScale, diffuseParameter);
        }
    }
//    MINVEC3(ambientParameter,onevec3,ambientParameter);
//    MINVEC3(diffuseParameter,onevec3,diffuseParameter);

    if( blend_mode.blend_bits & (BMB_CUBEMAP|BMB_PLANAR_REFLECTION) )
        setupReflectionParams(scrollsScales, bSkinnedModel, reflection_attenuation);

    // Set fragment program local parameters
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_AmbientColorFP, ambientParameter);  // Ambient color
    WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_DiffuseColorFP, diffuseParameter);  // Diffuse color

    if( blend_mode.blend_bits & BMB_HIGH_QUALITY )
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_LightDirFP, lightdir );

    {
        // These should match the "BumpMulti Flag Macros" in constants_fp.cgh
        // Note that g_BumpMultiFlagsFP is a float4 so only about 24 bits
        // are usuable in each component.  But, all 4 components should be available.
        Vec4 multi9flags;
        unsigned int flags[4] = {0,0,0,0};

        if (scrollsScales)
        {
            // reflection selector
            flags[0] |= scrollsScales->multiply1Reflect            ? (1 << 0) : 0;
            flags[0] |= scrollsScales->multiply2Reflect            ? (1 << 1) : 0;

            flags[0] |= scrollsScales->addGlowMat2                ? (1 << 2) : 0;
            flags[0] |= scrollsScales->alphaWater                ? (1 << 3) : 0;
            flags[0] |= scrollsScales->tintGlow                    ? (1 << 4) : 0;
            flags[0] |= scrollsScales->tintReflection            ? (1 << 5) : 0;
            flags[0] |= scrollsScales->desaturateReflection        ? (1 << 6) : 0;
        }

        // special blend mode control instead of having different compiled shader
        // variants
        flags[1] |= (blend_mode.blend_bits&BMB_OLDTINTMODE)            ? (1 << 0) : 0;

        multi9flags[0] = flags[0];
        multi9flags[1] = flags[1];
        multi9flags[2] = flags[2];
        multi9flags[3] = flags[3];

        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_BumpMultiFlagsFP, multi9flags );
    }


    // Scrolls/scales
    {
        Vec4 colorParam={0,0,0,1};

        if (scrollsScales) {
            glossParameter[2] = scrollsScales->alphaMask;
            glossParameter[3] = scrollsScales->maskWeight;
        } else {
            glossParameter[2] = glossParameter[3] = 0;
        }
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_GlossParamFP, glossParameter);  // Gloss factor + alphaMask

        // most models/layers don't actually use their scroll scale values so
        // only set shader constants on the ones that matter.
        // @todo !!!
        // Need to rework this so that we can only set the param array elements
        // actually used unless it comes out to be better to blast everything
        // down as a single EXT_gpu_program_parameters or uniform buffer updates
        {
            Vec4 scrollSpecs[TEXLAYER_MAX_SCROLLABLE];
            for (i=0; i<TEXLAYER_MAX_SCROLLABLE; i++)
            {
                if (excludedLayer(blend_mode, i)&EXCLUDE_SCROLLSSCALES)
                    continue;
                if (scrollsScales) {
                    setupScrolls(scrollsScales, i, scrollsScalesParameter);
                } // Else {0,0,1,1}
                // ...param was formerly indexed as C8+i...
                copyVec4( scrollsScalesParameter, scrollSpecs[i] );
            }
            WCW_SetCgShaderParamArray4fv(kShaderPgmType_FRAGMENT, kShaderParam_ScrollScaleArrFP, &scrollSpecs[0][0], TEXLAYER_MAX_SCROLLABLE);
        }

        if (!(blend_mode.blend_bits & BMB_SINGLE_MATERIAL)) {
            // Colors 3/4
            if (scrollsScales && scrollsScales->hasRgb34) {
                scaleVec3(scrollsScales->rgba3, 1.f/255.f, colorParam);
                colorParam[3]=1.0f;
                WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_ConstColor0FP, colorParam);
                scaleVec3(scrollsScales->rgba4, 1.f/255.f, colorParam);
                WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_ConstColor1FP, colorParam);
            } else {
                WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_ConstColor0FP, constColor0);
                WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_ConstColor1FP, constColor1);
            }
        }

        setupSpecularColor(scrollsScales, diffuseParameter);

        // AddGlow
        if (scrollsScales) {
            if (rdr_view_state.lamp_alpha_byte) {
                if (scrollsScales->minAddGlow == 255 && scrollsScales->baseAddGlow == 0) // NORANDOMADDGLOW
                {
                    colorParam[0] = (rdr_view_state.lamp_alpha_byte>127)?1.0:0.0;
                } else {
                    colorParam[0] = MAX(MIN(rdr_view_state.lamp_alpha/255.f, scrollsScales->maxAddGlow*1.f/255.f), scrollsScales->minAddGlow*1.f/255.f); 
                }
            } else {
                colorParam[0] = scrollsScales->baseAddGlow*1.f/255.f;
            }
        } else {
            colorParam[0] = MIN(rdr_view_state.lamp_alpha/255.f, 0.9); 
        }
        colorParam[1] = (((int)scrollsScales >> 2)&0xf)/128.f; // "unique" seed
        colorParam[2] = 0; // Must be zero
        WCW_SetCgShaderParam4fv(kShaderPgmType_FRAGMENT, kShaderParam_MiscParamFP, colorParam);
    }
}

#ifdef TRICK_PROFILE_TEST
StashTable htTrickCount;
static void addCount(TexOpt *texopt, int count)
{
    int i;

    if (!htTrickCount) {
        htTrickCount = stashTableCreateWithStringKeys(16, StashDeepCopyKeys);
    }
    if (!count)
        return;

    if (!stashFindInt(htTrickCount, texopt->name, &i))
        i=0;
    i+=count;
    stashAddInt(htTrickCount, texopt->name, i, true);
}

typedef struct {
    TexOpt *texopt;
    int count;
} Record;

Record **eaRecords;
#include <utilitieslib/components/earray.h>
#include "render/tex.h"

int tppproc(StashElement elem)
{
    Record *r = malloc(sizeof(Record));
    r->texopt = trickFromTextureName(stashElementGetStringKey(elem), NULL);
    r->count = (int)stashElementGetPointer(elem);
    eaPush(&eaRecords, r);
    return 1;
}

int cmpRecords(const void* aData, const void* bData)
{
    const Record ** a = (const Record **)aData;
    const Record ** b = (const Record **)bData;
    return (*b)->count - (*a)->count;
}

void trickProfilePrint(void)
{
    int i;
    printf("\n\n");
    stashForEachElement(htTrickCount, tppproc);

    eaQSort(eaRecords, cmpRecords);

    for (i=0; i<eaSize(&eaRecords); i++)  {
        TexOpt *texopt = eaRecords[i]->texopt;
        TexBind *texbind = texFindComposite(texopt->name);
        printf("%30s : %-8d%s\n", texopt->name, eaRecords[i]->count, (texbind&&(texbind->bind_blend_mode.blend_bits&BMB_SINGLE_MATERIAL))?" SM":"");
    }
    stashTableClear(htTrickCount);
    eaClearEx(&eaRecords, NULL);
}
#endif

static void drawLoopBumpMulti(VBO *vbo, Vec4 ambient, Vec4 diffuse, Vec4 lightdir,RdrTexList *texlists, U32 texlist_size, BlendModeType blend_mode, bool static_lighting, int tex_index, F32 reflection_attenuation)
{
    int i,ele_count,tex_count=vbo->tex_count,ele_base=0,j;

    WCW_TexLODBias(0, -2.0); // Since we're on a higher end card, they should have LOD Bias turned off anyway!
    WCW_TexLODBias(1, -2.0); // Since we're on a higher end card, they should have LOD Bias turned off anyway!

    NORMAL_MAP_TEST_PREP();

    for(i = 0; i < tex_count; i++) 
    {
        ele_count = vbo->tex_ids[i].count*3;

        if (tex_index==-1 || tex_index==i)
        {
            RdrTexList *texlist;
            U32 texlist_index = (tex_index==-1)?i:0;

            assert(texlist_index < texlist_size);

            texlist = &texlists[texlist_index];

            rdrBeginMarker("TEX:%d", i);
            for (j=0; j<rdr_caps.max_layer_texunits; j++)
            {
                int texid = texlist->texid[j];
                if (excludedLayer(blend_mode, j)&EXCLUDE_BIND)
                    continue;
                if( j==TEXLAYER_CUBEMAP) {
                    if( blend_mode.blend_bits&BMB_CUBEMAP && texid )
                    {
                        assert(rdr_view_state.cubemapMode);
                        texBindCube(TEXLAYER_CUBEMAP, texid);
                    }
                } else if (texid==0) {
                    texBind(j, (j==TEXLAYER_BUMPMAP1||j==TEXLAYER_BUMPMAP2)?rdr_view_state.dummy_bump_tex_id:(j==TEXLAYER_ADDGLOW1)?rdr_view_state.black_tex_id:rdr_view_state.white_tex_id);
                } else {
                    texBind(j, texid);
                }
            }
            
            // Vertex shader inputs
            setupBumpMultiVertShader(texlist, lightdir);
            // Pixel shader inputs
            setupBumpMultiPixelShader(ambient, diffuse, lightdir, texlist, blend_mode, static_lighting, false, false, reflection_attenuation);

#ifdef TRICK_PROFILE_TEST
            if (game_state.do_trick_profile) {
                glBeginQueryARB(GL_SAMPLES_PASSED_ARB, 1); CHECKGL;
            }
#endif

        drawElements(vbo->tex_ids[i].count, ele_base, ele_count, vbo->tris);

#ifdef TRICK_PROFILE_TEST
            if (game_state.do_trick_profile) {
                int sampleCount;
                glEndQueryARB(GL_SAMPLES_PASSED_ARB); CHECKGL;
                glFlush(); CHECKGL;
                glGetQueryObjectuivARB(1, GL_QUERY_RESULT_ARB, &sampleCount); CHECKGL;
                addCount(texlist->scrollsScales->debug_backpointer, sampleCount);
            }
#endif
            rdrEndMarker();
        }
        ele_base += ele_count;
    }
}


void modelDrawMultiTex( RdrModel *draw, U8 * rgbs, RdrTexList *texlist, BlendModeType blend_mode, F32 reflection_attenuation)
{
    VBO *vbo = draw->vbo;
    Vec4    lightdir = { 1,0,0,0};
    static    F32 offset;
    extern Vec4    tex_scrolls[4];
    ModelStateSetFlags    flags;

    rdrBeginMarker(__FUNCTION__);
    MODEL_PERFINFO_AUTO_START(__FUNCTION__, 1);
    calcLightDir(lightdir, draw, rgbs);

    // It's drawn through a different codepath if we don't have bumpmaps
    assert((rdr_caps.features & GFXF_BUMPMAPS)); // TEXTODO make sure this doesn't go off

    modelBindBuffer(vbo);

    flags = ONLY_IF_NOT_ALREADY | ((blend_mode.blend_bits & BMB_HIGH_QUALITY) ? BIT_HIGH_QUALITY_VERTEX_PROGRAM : 0);
    if (rgbs) { 
        modelDrawState(DRAWMODE_BUMPMAP_MULTITEX_RGBS, flags);
//        setVec3(ambient, 0.5, 0.5, 0.5); // Multiplied by 2 in setup step
//        setVec3(diffuse, 0, 0, 0);
    } else
        modelDrawState(DRAWMODE_BUMPMAP_MULTITEX, flags);

    modelBlendState(blend_mode, ONLY_IF_NOT_ALREADY);

    /////Regular Array Pointers//////////////////////////////////////////////////////////
    texBindTexCoordPointer(TEXLAYER_BASE, vbo->sts);
    WCW_NormalPointer(GL_FLOAT, 0, vbo->norms);
    WCW_VertexPointer(3,GL_FLOAT,0, vbo->verts);

    /////Special Bumpmapping Array Pointers///////////////////////////////////////
    devassert(vbo->tangents);
    glVertexAttribPointerARB( 7, 4, GL_FLOAT, GL_FALSE, 0, vbo->tangents ); CHECKGL;
    if (rgbs)
    {
        if (rdr_caps.use_vbos)
        {
            WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, (int)rgbs);
            glVertexAttribPointerARB( 11, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0 ); CHECKGL;
            WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, vbo->vertex_array_id);
        }
        else
        {
            WCW_BindBuffer(GL_ARRAY_BUFFER_ARB, 0);
            glVertexAttribPointerARB( 11, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, rgbs ); CHECKGL;
        }
    }

    if (draw->has_fx_tint) {
        int i;
        for (i=0; i<3; i++) {
            draw->diffuse[i] = draw->diffuse[i]*draw->tint_colors[0][i]/255;
            draw->ambient[i] = draw->ambient[i]*draw->tint_colors[0][i]/255;
        }
    }

    // Setup planar reflection transform
    if (blend_mode.blend_bits&BMB_PLANAR_REFLECTION)
    {
        assert(draw->tex_index >= 0 && draw->texlist_size == 1); // assuming this is a single material, which it should be since only drawWhite mode sends -1 (which means to draw all materials here)
        texlist[0].texid[TEXLAYER_PLANAR_REFLECTION] = rdrGetBuildingReflectionTextureHandle();
        WCW_SetCgShaderParam4fv(kShaderPgmType_VERTEX, kShaderParam_PlanarReflectionPlaneVP, *rdrGetBuildingReflectionPlane());
    }

    MODEL_PERFINFO_AUTO_START("drawLoopBumpMulti", 1);
    drawLoopBumpMulti(vbo, draw->ambient, draw->diffuse, lightdir, texlist, draw->texlist_size, blend_mode, !!rgbs, draw->tex_index, reflection_attenuation);
    MODEL_PERFINFO_AUTO_STOP();
    MODEL_PERFINFO_AUTO_STOP();
    rdrEndMarker();
}




void modelResetVBOsDirect(void)
{
    modelBindDefaultBuffer();
}

void modelDrawDirect(RdrModel *draw)
{
    static U8    rgba[] = {255,255,255,255};
    VBO            *vbo = draw->vbo;
    U8            *rgbs;
    TrickNode    *trick;
    RdrTexList    *texlist;
    BlendModeType blend_mode;
    int write_depth_only = rdr_view_state.write_depth_only;
    int draw_solid = draw->draw_white;

    rdrBeginMarker(__FUNCTION__ ":%s", draw->debug_model_backpointer->name);

    // hack to fix tree shadows
    if( draw->is_tree && draw->alpha ) {
        write_depth_only = false;
    }

    MODEL_PERFINFO_AUTO_START("modelDrawDirect", 1);
#ifdef MODEL_PERF_TIMERS
    PERFINFO_AUTO_START_STATIC(draw->model_name, draw->model_perfInfo, 1);
#endif

    assert(draw->blend_mode.shader > 0 && draw->blend_mode.shader < BLENDMODE_NUMENTRIES);

    assert( vbo );

    if (draw->texlist_size)
        texlist = (RdrTexList*) (draw+1);
    else
        texlist = NULL;

    if (draw->has_trick)
        trick = &draw->trick;
    else
        trick = 0;
        
    rgbs = draw->rgbs;
    blend_mode = draw->blend_mode;

    // remove cubemap blend bit if cubemaps disabled (for cubemaps which are statically assigned in the material)
    if(blend_mode.blend_bits & BMB_CUBEMAP && !rdr_view_state.cubemapMode )
        blend_mode.blend_bits &= ~BMB_CUBEMAP;

    RT_STAT_INC(worldmodel_drawn);
    RT_STAT_ADDMODEL(vbo);

    if (!draw_solid && !write_depth_only)
    {
        modelBlendState(blend_mode, ONLY_IF_NOT_ALREADY);
        assert(blend_mode.shader != BLENDMODE_SUNFLARE);
    }

    setVec3(constColor0,1,1,1);
    constColor0[3] = draw->alpha * 1.f/255.f;
    setVec4(constColor1,1,1,1,1);

    WCW_TexLODBias(0, -0.5);
    WCW_TexLODBias(1, -0.5); 

    if (trick && (trick->flags2 & TRICK2_SETCOLOR) || draw->has_tint || draw->brightScale != 1.f)
    {
        static U8 dummy[2][4] = {255,255,255,255,255,255,255,255};
        U8 (*c)[4];

        if (draw->has_tint)
            c = draw->tint_colors;
        else if (draw->has_fx_tint)
            c = dummy; // FX using dual color blend or multi9 geometry and color tinting (rare)
        else 
            if (trick)
                c = (void *)trick->trick_rgba;
            else
                c = dummy;    //patch to prevent crashing here
        scaleVec3(c[0],(1.f/255.f),constColor0);
        scaleVec3(c[1],(1.f/255.f),constColor1);
        WCW_ConstantCombinerParamerterfv(1, constColor1);
    }
    WCW_ConstantCombinerParamerterfv(0, constColor0);
    MODEL_PERFINFO_AUTO_START("gfxNodeTricks", 1);
        
    if (gfxNodeTricks(trick,vbo,draw,blend_mode,draw->is_fallback_material) || draw_solid) 
    {
        ScrollsScales *scrollsScales;
        MODEL_PERFINFO_AUTO_STOP_START("Drawing", 1);
        if (!draw_solid && !write_depth_only)
        {
            if (/*draw->tex_index!=-1 &&*/ texlist && (scrollsScales = texlist[0].scrollsScales)) {
                mulVecVec3(draw->ambient, scrollsScales->ambientScale, draw->ambient);
                MAXVEC3(draw->ambient, scrollsScales->ambientMin, draw->ambient);
                mulVecVec3(draw->diffuse, scrollsScales->diffuseScale, draw->diffuse);
            }
            scaleVec3(draw->ambient, draw->brightScale, draw->ambient);
            scaleVec3(draw->diffuse, draw->brightScale, draw->diffuse);
            if (vbo->flags & OBJ_FULLBRIGHT)
            {
                rgbs = 0;
                if (blend_mode.shader == BLENDMODE_MULTI || blend_mode.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL) {
                    setVec3(draw->ambient, 0.5, 0.5, 0.5); // x2 in setup
                } else {
                    setVec3(draw->ambient, 0.25, 0.25, 0.25); // x4 in shader
                }
                setVec3(draw->diffuse, 0, 0, 0);
            }

            WCW_Light( GL_LIGHT0, GL_AMBIENT,  draw->ambient );
            WCW_Light( GL_LIGHT0, GL_DIFFUSE,  draw->diffuse );
        }

        WCW_LoadModelViewMatrix(draw->mat);

        if (rdr_view_state.wireframe != 2)
        {
            if (rdr_view_state.wireframe && blend_mode.shader != BLENDMODE_WATER)
            {
                glEnable(GL_POLYGON_OFFSET_FILL); CHECKGL;
                glPolygonOffset(1,1); CHECKGL;
            }

            if (trick && (trick->flags2 & TRICK2_WIREFRAME))
            {
                if (rdr_view_state.wireframe_seams && !rdr_caps.use_vbos)
                    modelDrawWireframeSeams(vbo);
                else
                    modelDrawWireframe(vbo,trick->trick_rgba,draw->tex_index);
            }
            else if( !( vbo->flags & OBJ_HIDE ) )
            {
                if (write_depth_only)
                {
                    modelDrawDepthOnly(draw, trick);
                }
                else if (draw_solid)
                {
                    U32 solidColor = 0;

                    // Respect the tint.
                    // Used for 'beyond fog' geometry to tint to full fog color
                    if (draw->has_tint)
                    {
                        solidColor |= ((U32) draw->tint_colors[0][0]) << 0;
                        solidColor |= ((U32) draw->tint_colors[0][1]) << 8;
                        solidColor |= ((U32) draw->tint_colors[0][2]) << 16;
                    }
                    /*
                    else if (game_state.test4)
                    {
                        solidColor |= ((U32) (255 * draw->ambient[0])) << 0;
                        solidColor |= ((U32) (255 * draw->ambient[1])) << 8;
                        solidColor |= ((U32) (255 * draw->ambient[2])) << 16;
                    }
                    else if (game_state.test3)
                    {
                        solidColor |= ((U32) (255 * (draw->ambient[0] + draw->diffuse[0]))) << 0;
                        solidColor |= ((U32) (255 * (draw->ambient[1] + draw->diffuse[1]))) << 8;
                        solidColor |= ((U32) (255 * (draw->ambient[2] + draw->diffuse[2]))) << 16;

                    }
                    */
                    else
                    {
                        // Should this be ambient or ambient + diffuse?
                        solidColor = 0x00ffffff;
                    }

                    // Always draws the entire model
                    modelDrawSolidColor(vbo, solidColor | (draw->alpha << 24));
                }
                else
                {
                    F32        reflection_attenuation = draw->cubemap_attenuation;

                    if (blend_mode.shader == BLENDMODE_MULTI)
                    {
                        modelDrawMultiTex(draw, rgbs, texlist, blend_mode, reflection_attenuation);
                    }
                    else if (blend_mode.shader == BLENDMODE_WATER)
                    {
                        modelDrawWater(draw, rgbs, texlist);
                    }
                    else if (blendModeHasBump(blend_mode))
                    {
                        assert( !(blend_mode.blend_bits&BMB_PLANAR_REFLECTION) );
                        modelDrawBump(draw, rgbs, texlist, blend_mode, trick, reflection_attenuation);
                    }
                    else if (rgbs)
                    {
                        assert( !(blend_mode.blend_bits&BMB_PLANAR_REFLECTION) );
                        modelDrawTexRgb(draw, rgbs, texlist, blend_mode, trick, reflection_attenuation);
                    }
                    else
                    {
                        assert( !(blend_mode.blend_bits&BMB_PLANAR_REFLECTION) );
                        modelDrawTexNormals(draw, texlist, blend_mode, trick, reflection_attenuation);
                    }
                }
            }

            if (rdr_view_state.wireframe && blend_mode.shader != BLENDMODE_WATER) {
                glDisable(GL_POLYGON_OFFSET_FILL); CHECKGL;
            }
        }

#ifndef FINAL
        if (rdr_view_state.wireframe && !(trick && (trick->flags2 & TRICK2_WIREFRAME)))
            modelDrawWireframe(vbo,rgba,draw->tex_index);

        if((rdr_view_state.gfxdebug || rdr_view_state.wireframe) && !rdr_caps.use_vbos && draw->tex_index < 1) {
            modelDrawVertexBasisTBN( vbo, NULL, NULL, NULL );
            if (rdr_view_state.gfxdebug & GFXDEBUG_LIGHTDIRS) {
                Vec4 lightdir, lightdirModel;
                Mat4 invModelView;
                invertMat4Copy(draw->mat, invModelView);
                calcLightDir(lightdir, draw, rgbs);
                mulVecMat3(lightdir, invModelView, lightdirModel);
                lightdirModel[3] = 0;
                WCW_Color4(255,255,127,255);
                modelDrawLightDirs( vbo, vbo->verts, lightdirModel);
                WCW_Color4(255,255,255,255);
            }
        }

        // draw an extrusion line from model origin showing the maximum model sweep
        // used for checking if models cast into the view frustum.
        // Useful for debugging the shadowmap system
        if (game_state.shadowMapDebug&showExtrudeCasters && rdr_view_state.renderPass == RENDERPASS_COLOR)
        {
            Vec3 offset_vs, offset, p2;
            Mat4 invModelView;
            Mat4 matModelToWorld;
            F32 extrusionDistance;
            Vec3 mid_ms, mid_ws;

            mulMat4Inline(rdr_view_state.inv_viewmat, draw->mat, matModelToWorld);

            if (draw->debug_model_backpointer)
            {
                mid_ms[0] = (draw->debug_model_backpointer->min[0] + draw->debug_model_backpointer->max[0]) * 0.5f;
                mid_ms[1] = (draw->debug_model_backpointer->min[1] + draw->debug_model_backpointer->max[1]) * 0.5f;
                mid_ms[2] = (draw->debug_model_backpointer->min[2] + draw->debug_model_backpointer->max[2]) * 0.5f;
            }
            else
            {
                setVec3(mid_ms, 0.0f ,0.0f ,0.0f );
            }

            mulVecMat3(mid_ms, matModelToWorld, mid_ws);

            extrusionDistance = (mid_ws[1] - rdr_view_state.lowest_visible_point_last_frame) * rdr_view_state.shadowmap_extrusion_scale;

            // current gl matrix maps this models coords to viewspace

            // have to convert the shadow direction offset from view to this model
            invertMat4Copy(draw->mat, invModelView);
            scaleVec3( rdr_view_state.shadowmap_light_direction_in_viewspace, extrusionDistance, offset_vs );
            mulVecMat3(offset_vs, invModelView, offset);

            addVec3( mid_ms, offset, p2 );
            WCW_Color4(255,0,0,255);
            glBegin(GL_LINES);
                glVertex3f(mid_ms[0],mid_ms[1], mid_ms[2]);
                glVertex3f(p2[0],p2[1], p2[2]);
            glEnd(); CHECKGL;
            WCW_Color4(255,255,255,255);
        }
#endif
        //WCW_SunLightDir();//undo custom light direction, if there was one (rare case that there is) RDRFIX

        // Only call gfxNodeTricksUndo() if gfxNodeTricks() returned 1
        // Otherwise, we may undo things which were not done.
        // This was causing a fog stack underflow
        MODEL_PERFINFO_AUTO_STOP_START("gfxNodeTricksUndo", 1);
        gfxNodeTricksUndo( trick,vbo,draw->is_fallback_material );
    }
    MODEL_PERFINFO_AUTO_STOP();
    assert(blend_mode.shader != BLENDMODE_SUNFLARE);
#ifdef MODEL_PERF_TIMERS
    PERFINFO_AUTO_STOP();
#endif
    MODEL_PERFINFO_AUTO_STOP();

    rdrEndMarker();
}

void modelDrawAlphaSortHackDirect(RdrModel *draw)
{
    F32        d;

    MODEL_PERFINFO_AUTO_START("modelDrawAlphaSortHackDirect", 1);
    if (!(game_state.shadowvol ==2 || scene_info.stippleFadeMode)) {
        WCW_EnableStencilTest();   
        WCW_StencilFunc(GL_ALWAYS, 64, ~0); 
        WCW_StencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    }
    if (draw->alpha == 255) {
        glAlphaFunc(GL_GREATER, 0.6); CHECKGL;
        WCW_BlendFuncPush(GL_ONE, GL_ZERO);
        modelDrawDirect(draw);
        glAlphaFunc(GL_GREATER, 0); CHECKGL;
        WCW_BlendFuncPop();
    } else {
        d = draw->alpha * (1.f/255.f) - 0.4f;
        if (d < 0)
            d = 0;
        glAlphaFunc(GL_GREATER, d); CHECKGL;
        modelDrawDirect(draw);
        glAlphaFunc(GL_GREATER, 0); CHECKGL;
    }
    if (!(game_state.shadowvol ==2 || scene_info.stippleFadeMode)) {
        WCW_EnableStencilTest();  
        WCW_StencilFunc(GL_ALWAYS, 128, ~0);
        WCW_StencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    }
    MODEL_PERFINFO_AUTO_STOP();
}
