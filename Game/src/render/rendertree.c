#define RT_ALLOW_VBO // Needs access to check model sanity.
#include <string.h>
#include <stdlib.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include "graphics/gfxwindow.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/file.h>
#include "cmdparse/cmdcommon.h"
#include "assert.h"
#include <utilitieslib/utils/memcheck.h>
#include "seq/gfxtree.h"
#include "graphics/camera.h" 
#include "cmdparse/cmdgame.h" 
#include "graphics/font.h"
#include "seq/seq.h"
#include "render/render.h"
#include "render/rendertricks.h"
#include "edit/edit_cmd.h"
#include "graphics/groupdraw.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>
#include "UI/uiCursor.h"
#if CLOTH_HACK
#include "render/rendercloth.h"
#include "graphics/clothnode.h"
#endif
#include <utilitieslib/utils/qsortG.h>
#include "render/renderprim.h"
#include "render/tex.h"
#include "seq/tricks.h"
#include "render/model_cache.h"
#include "render/renderstats.h"
#include "entity/entity.h"
#include "UI/uiConsole.h"
#include "graphics/gfx.h"
#include "NovodeX/NwWrapper.h"
#include "graphics/sun.h"
#include "graphics/groupMiniTrackers.h"
#include "graphics/viewport.h"
#include "graphics/gfxSettings.h"
#include "render/cubemap.h"
#include "graphics/view_cache.h"
#include "render/rendershadowmap.h"
#include "render/thread/rt_shadowmap.h"
#include "graphics/gfxDebug.h"

#define HQ_BUMP_DIST 25

#ifndef FINAL
static const U8 tintColors[TINT_COUNT][4] =
{
    {   0,   0,   0,   0 },            // TINT_NONE
    { 255,  20,  20, 255 },            // TINT_RED
    {  20, 255,  20, 255 },            // TINT_GREEN
    {  20,  20, 255, 255 },            // TINT_BLUE
    { 255,  20, 255, 255 },            // TINT_PURPLE
    { 255, 255,  20, 255 },            // TINT_YELLOW
    {  20, 255, 255, 255 },            // TINT_CYAN
};
#endif

int num_anim_mults;//mm$$temp 
int drawGfxNodeCalls;


//Debug 
int sortedByDist;
int sortedByType;
extern int call, drawCall;
int startTypeDrawing;
int startDistDrawing; 
int startAUWDrawing; 
int startShadowDrawing; 
int endDrawModels;
int drawStartTypeDrawing;
int drawStartDistDrawing; 
int drawStartAUWDrawing; 
int drawStartShadowDrawing; 
int drawEndDrawModels;
//End Debug 

#define MIN_GFXNODE_ALPHA_TO_BOTHER_WITH 5

#define SORTTHING_GFXTREENODE 0
#define SORTTHING_WORLDMODEL 1

#define SORT_BY_TYPE    0
#define SORT_BY_DIST    1

typedef struct
{
    Mat4            viewMat;

    union
    {
        GfxNode * gfxnode;
        int vsIdx;
    };
    union
    {
        F32                dist;
        struct                    // used by shadowmapping until it gets own sort thing structure
        {
            U16 tri_start;        // start triangle for batch
            U16 tri_count;        // number of triangles in batch (can be an aggregate batch, e.g. opaques)
        };
    };
    Model            *model;
    TexBind            *tex_bind;
    BlendModeType    blendMode;

    S16                tex_index;

    U8                alpha;
    U8                tint;


    U8                modelSource:1;  // SORTTHING_GFXTREENODE or SORTTHING_WORLDMODEL
    U8                draw_white:1;
    U8                was_type_sorted:1;
    U8                alpha_test:1;    // for sorting shadow models
    U8                is_tree:1;        // for sorting shadow models
    U8                is_gfxnode:1;    // for distinguishing costume pieces from world geo
    U8                has_translucency:1;
} SortThing;

typedef struct
{
    SortThing * typeSortThings;
    int typeSortThingsCount;
    int typeSortThingsMax;

    SortThing * distSortThings;
    int distSortThingsCount;
    int distSortThingsMax;

    int nodeTotal; //debug

    ViewSortNode * vsList;
    int vsListCount;
    int vsListMax;

    ShadowNode    *shadow_nodes;
    int            shadow_node_count,shadow_node_max;
} DisplayList;

static DisplayList    display_list[2],*fg_list,*bg_list;
static int            curr_list_idx;

int sortedDraw;
int immediateDraw;

// BEGIN Reflection plane management
#define MAX_REFLECTION_PLANES 1024
static Vec4    s_reflection_plane_pool[MAX_REFLECTION_PLANES];
static U32     s_reflection_plane_next_free_index;

static INLINEDBG Vec4 *GetReflectionPlanes(unsigned int num_planes)
{
    Vec4 *rc = NULL;
    if ((s_reflection_plane_next_free_index + num_planes) <= MAX_REFLECTION_PLANES)
    {
        rc = &s_reflection_plane_pool[s_reflection_plane_next_free_index];
        s_reflection_plane_next_free_index += num_planes;
    }
    else
    {
        assert("MAX_REFLECTION_PLANES exceeded" == 0);
    }

    return rc;
}
// END Reflection plane management

////////////////////////////////////////////////////////////////////////////
// BEGIN Debugging dump
// @todo is there a better home for this? Requires exposing SortThing definition

#ifndef FINAL // DEBUG CODE

void dbg_LogBatchPass(SortThing sortThings[], int sortThingCount, ViewportInfo* viewport, char const* nameSuffix )
{
    static int s_bBatchLogPass = 0;
    static int s_bBatchLogFrameStart = 0;

    if (game_state.dbg_log_batches)
    {
        // first logging for this frame?
        if (!s_bBatchLogPass)
        {
            // keep track of the frame so we can disable on next frame
            s_bBatchLogFrameStart = game_state.client_frame;
        }

        // if this is a new frame then turn off the batch logging
        if (s_bBatchLogFrameStart != game_state.client_frame)
        {
            game_state.dbg_log_batches = false;
            s_bBatchLogPass = 0;
            return;
        }

        s_bBatchLogPass++;    // to guarantee file name change on each pass

        if (sortThingCount)
        {
            char filename[MAX_PATH];
            FILE* file;
            int i;
            time_t now;
            char the_date[MAX_PATH] = "";


            // add date/frame to filename so don't overwite old results
            now = time(NULL);
            if (now != -1)
            {
                strftime(the_date, MAX_PATH, "%Y_%m_%d", gmtime(&now));
            }
            snprintf(filename, sizeof(filename), "C:/coh_batch_log_%s_f%d-v%02d_%s_%s.txt",
                                the_date, s_bBatchLogFrameStart, s_bBatchLogPass, nameSuffix, viewport->name);
            file = fopen( filename, "wt");
            fprintf( file, "   #\ttree?,\talpha test?,\talpha,\tdraw white,\tShader,\tBlend bits,\tTexBind,\tModel,\t\tTex_index(sub-obj),\tsubobj tris,\tsubobj count,\ttris,\tverts,\tmatrix,\ttexbind name,\tname,\tfile,\tnode,\tbones\n");
            for( i = 0 ; i < sortThingCount; i++ ) 
            {
                SortThing* st;
                static char buf[512];

                st = &sortThings[i];
                if (st->model)
                {
                    int bones = 0;
                    bool node = false;
                    int subobj_tris = st->model->tri_count;
                    if (st->tex_index >=0 && st->tex_index < st->model->tex_count)
                    {
                        subobj_tris =  st->model->tex_idx[st->tex_index].count;
                    }
                    fprintf(file, "%4d\t%d\t%d\t%d\t%d\t", i, st->is_tree, st->alpha_test, st->has_translucency, st->draw_white );
                    fprintf(file, "%d\t%d\t0x%08x\t0x%08x\t", st->blendMode.shader, st->blendMode.blend_bits ,st->tex_bind, st->model );
                    fprintf(file, "\t%4d%7d\t%7d\t%7d\t%7d\t", st->tex_index, subobj_tris, st->model->tex_count, st->model->tri_count, st->model->vert_count );
                    fprintf(file, "%f %f %f  %f %f %f  %f %f %f  %f %f %f\t",
                                                st->viewMat[0][0], st->viewMat[0][1], st->viewMat[0][2],
                                                st->viewMat[1][0], st->viewMat[1][1], st->viewMat[1][2],
                                                st->viewMat[2][0], st->viewMat[2][1], st->viewMat[2][2],
                                                st->viewMat[3][0], st->viewMat[3][1], st->viewMat[3][2]
                                    );
                    // @todo currently have a hack which overrides meaning of tex_bind for shadow map
                    fprintf(file, "%-32s %s %s\t", st->tex_bind && (int)st->tex_bind > 0x00100000 ? st->tex_bind->name : "", st->model->name, st->model->filename );
                    if (st->modelSource == SORTTHING_GFXTREENODE && st->gfxnode)
                    {
                        node = true;
                        bones = st->gfxnode->model->boneinfo ? st->gfxnode->model->boneinfo->numbones: 0;
                    }
                    fprintf(file, "%2d\t%4d\t", node, bones );
                    fprintf(file, "\n" );
                }
                else
                {
                    fprintf(file, "%4d error: no model", i);
                }
            }
            fclose(file);
        }
    }
}

#define DBG_LOG_BATCHES( sortList, count, viewport, nameSuffix ) dbg_LogBatchPass( (sortList), (count), (viewport),  (nameSuffix))
#else
#define DBG_LOG_BATCHES( sortList, count, viewport, nameSuffix )
#endif
// END DEBUGGING DUMP
///////////////////////////////////////////////////////////////////////////

ViewSortNode *getViewSortNode(int index)
{
    if (index < bg_list->vsListCount)
        return &bg_list->vsList[index];
    return 0;
}

void gfxTreeFrameInit()
{
    if (!fg_list)
    {
        fg_list = display_list;
        bg_list = display_list+1;
    }
    fg_list->typeSortThingsCount=0;
    fg_list->distSortThingsCount=0;
    fg_list->vsListCount = 0;
    fg_list->nodeTotal = 0;
    fg_list->shadow_node_count = 0;

    s_reflection_plane_next_free_index = 0;
}

//###########################################################################
//3. Game Specific GfxTree Stuff ############################################

void gfxTreeSetBlendMode( GfxNode * node )
{
    node->node_blend_mode = BlendMode(0,0);

    if (node->model) {
        if (node->model->common_blend_mode.shader)
            node->node_blend_mode = node->model->common_blend_mode;
    }

    if( node->flags & GFXNODE_CUSTOMTEX && node->customtex.base ) {
        TexBind * bind = node->customtex.base;

        node->node_blend_mode = promoteBlendMode(node->node_blend_mode, bind->bind_blend_mode);
    }
}


//Custom Textures make modelects have alpha that didn't before.  Thus this function
//figures it out and sets the GfxNode's flags according to the needs of the new textures
void gfxTreeSetCustomTextures( GfxNode * gfx_node, TextureOverride binds) // Struct copy on stack, because we change it
{
    if(gfx_node && (binds.base || binds.generic) )
    {
        gfx_node->flags |= GFXNODE_CUSTOMTEX;

        //// This is called only once per node (i.e. nodes get recreated)
        // JE: This now gets called multiple times because of FX in the costume screen
        //assert(!gfx_node->customtex.base && !gfx_node->customtex.generic);
        gfx_node->customtex = binds;

        //Set the alpha sort flag
        if (0 && gfx_node->model) {
            // This doesn't work because a lot of player textures have an alpha channel even though
            // it is completely opaque, this would cause them to be sorted/disappear in water
            // Perhaps we can turn this on after the artists fix things?
            BlendModeType    blend;
            BasicTexture *b = binds.base->tex_layers[TEXLAYER_BASE];
            bool needsAlphaSort=false;

            if (b) {
                blend = binds.base->bind_blend_mode;

                if (blend.shader == BLENDMODE_ADDGLOW || blend.shader == BLENDMODE_COLORBLEND_DUAL || blend.shader == BLENDMODE_ALPHADETAIL)
                    b = binds.generic;
                if (gfx_node->model->flags & OBJ_DRAWBONED)
                    b = binds.generic;
                if (b && b->flags & TEX_ALPHA)
                    needsAlphaSort = true;
            }
            if (needsAlphaSort)
                gfx_node->flags |= GFXNODE_ALPHASORT;
            else
                gfx_node->flags &= ~GFXNODE_ALPHASORT;
        }
    }
}

void gfxTreeSetCustomTexturesRecur( GfxNode * gfx_node, TextureOverride *binds )
{
    GfxNode *walk;
    gfxTreeSetCustomTextures(gfx_node, *binds);
    for (walk = gfx_node->child; walk; walk = walk->next)
        gfxTreeSetCustomTexturesRecur(walk, binds);
}

void gfxTreeSetCustomColors( GfxNode * gfx_node, const char rgba[], const char rgba2[], int gfxNodeFlag )
{
    int i;
    gfx_node->flags |= gfxNodeFlag; // GFXNODE_CUSTOMCOLOR or GFXNODE_TINTCOLOR
    for(i = 0 ; i < 3 ; i++)
    {        
        gfx_node->rgba[i]  = rgba[i];
        gfx_node->rgba2[i] = rgba2[i];
    }
}

void gfxTreeSetCustomColorsRecur( GfxNode * gfx_node, const char rgba[], const char rgba2[], int gfxNodeFlag )
{
    GfxNode *walk;
    gfxTreeSetCustomColors(gfx_node, rgba, rgba2, gfxNodeFlag);
    for (walk = gfx_node->child; walk; walk = walk->next)
        gfxTreeSetCustomColorsRecur(walk, rgba, rgba2, gfxNodeFlag);
}

/*/This scale is for visibility purposes, not the size...
void gfxNodeSetScale(GfxNode * node, F32 scale, int root )
{
    for(;node;node = node->next)
    {
        node->scale = scale;
        
        if (node->child)
            gfxNodeSetScale(node->child, scale,0);
        if (root)
            break;
    }
}*/

void gfxTreeSetScale(GfxNode *node)
{
    int        i;
    F32        t,scale2 = 0;

    for(i=0;i<3;i++)
    {
        t = lengthVec3Squared(node->mat[i]);
        if (t > scale2)
            scale2 = t;
    }
    //node->scale = sqrtf(scale2);
}

static INLINEDBG void initViewSortModelInline2(ViewSortNode *vs)
{
    vs->model        = avsn_params.model;
    vs->alpha        = avsn_params.alpha;
    vs->rgbs        = avsn_params.rgbs;
    vs->light        = avsn_params.light;
    vs->brightScale    = avsn_params.brightScale;
    vs->is_gfxnode    = avsn_params.is_gfxnode;
    if (avsn_params.mid)
        copyVec3(avsn_params.mid,vs->mid);

    if (avsn_params.has_tint)
    {
        vs->has_tint = 1;
        memcpy(vs->tint_colors,avsn_params.tint_colors,8);
        vs->has_fx_tint = 0;
    }
    else if (avsn_params.has_fx_tint)
    {
        if (avsn_params.tint_colors[4] != 255 || avsn_params.tint_colors[5] != 255
            || avsn_params.tint_colors[6] != 255)
        {
            // FX Geos are dual-tinted and can't use the original fx tinting system
            vs->has_tint = 1;
            memcpy(vs->tint_colors,avsn_params.tint_colors,8);
            vs->has_fx_tint = 0;
        }
        else
        {
            memcpy(vs->tint_colors,avsn_params.tint_colors,4);
            vs->has_fx_tint = 1;
        }
    }

    copyMat4(avsn_params.mat,vs->mat);

    if (avsn_params.tricks)
    {
        vs->has_tricks = 1;
        vs->tricks = avsn_params.tricks;
    } else {
        vs->has_tricks = 0;
    }
    vs->stipple_alpha = avsn_params.stipple_alpha;
    vs->uid = avsn_params.uid;
    vs->materials = avsn_params.materials;
    vs->use_fallback_material = avsn_params.use_fallback_material;
    vs->from_tray = avsn_params.from_tray;
}

static INLINEDBG BlendModeType nodeBlendMode(GfxNode *node, Model *model, int tex_index, F32 dist, TexBind **eff_bind)
{
    BlendModeType blend_mode;

    if (!model) {
        blend_mode = node->node_blend_mode;
    } else if (node->node_blend_mode.shader) {
        blend_mode = node->node_blend_mode;
    } else {
        if (tex_index==-1)
            tex_index=0;
        blend_mode = model->blend_modes[tex_index];
    }

    if (node->customtex.base) {
        *eff_bind = node->customtex.base;
        //if (!node->node_blend_mode.shader) // JE: Removed this to fix Full Helemts | Steel Helm, whose Model is Multi, but texture swapped to DualColorBump
            blend_mode = (*eff_bind)->bind_blend_mode;
    } else if (model) {
        *eff_bind = model->tex_binds[tex_index];
    } else {
        *eff_bind = NULL;
    }

    // Let cloth use this logic too
    //if( model && (model->flags & OBJ_DRAWBONED) && (node->flags & GFXNODE_SEQUENCER))
    {
        const ScrollsScales *scrollsScales = (*eff_bind) ? (*eff_bind)->scrollsScales : NULL;
        bool bUsesCubemap = false;
        bool bUseHighQuality = false;
        bool bHighQualityCapable = false;

        // Skinned needs at least BLENDMODE_COLORBLEND_DUAL
        // Additionally skinned objects can have the HIGH_QUALITY bit set for higher quality bump calculations
        if (blend_mode.shader==BLENDMODE_MULTI)
        {
            if (rdr_caps.features & GFXF_MULTITEX_DUAL) {
                blend_mode = BlendMode(BLENDMODE_MULTI,blend_mode.blend_bits);
            } else {
                blend_mode = BlendMode(BLENDMODE_MULTI,blend_mode.blend_bits|BMB_SINGLE_MATERIAL);
            }

            if (rdr_caps.features & GFXF_MULTITEX_HQBUMP && gfx_state.mainViewport)
            {
                bHighQualityCapable = true;

                if (ABS(dist) < HQ_BUMP_DIST)
                    bUseHighQuality = true;
            }

            bUsesCubemap = isReflective(scrollsScales, true);

        } else if (blendModeHasBump(blend_mode)) {
            blend_mode = BlendMode(BLENDMODE_BUMPMAP_COLORBLEND_DUAL,0);

            if (rdr_caps.features & GFXF_HQBUMP && gfx_state.mainViewport)
            {
                bHighQualityCapable = true;

                if (ABS(dist) < HQ_BUMP_DIST)
                    bUseHighQuality = true;
            }

            // Dont apply cubemaps to water
            if (blend_mode.shader != BLENDMODE_WATER)
                bUsesCubemap = isReflective(scrollsScales, true);

        } else {
            blend_mode = BlendMode(BLENDMODE_COLORBLEND_DUAL,0);
        }
        
        // set cubemap blend_bits flag if this model includes reflection and we support cubemaps on this render pass
        if (bUsesCubemap)
        {
            blend_mode.blend_bits |= BMB_CUBEMAP;

            // Always force the high quality cubemap shader mode (ignore distance check)
            if (bHighQualityCapable && !bUseHighQuality)
                bUseHighQuality = true;
        }
        else
        {
            // Explicitly clear cubemap mode if it should not be a feature of this render.
            // Explicit "Cubemap" specifications in a material trick will have already
            // set the BMB_CUBEMAP flag on the tricks 'default' BlendMode during 'optimization'
            // and that will have propagated down here. So, it needs to be cleared out if
            // cubemaps should not be used for some other reason (e.g. not main viewport, edit mode, etc.)
            blend_mode.blend_bits &= ~BMB_CUBEMAP;
        }

        if (bUseHighQuality) {
            blend_mode.blend_bits |= BMB_HIGH_QUALITY;
        } else {
            blend_mode.blend_bits &= ~BMB_HIGH_QUALITY;
        }
    }

    // If this node should do shadowmap lookups set the bit to choose the correct shader variant
    if ( rdrShadowMapCanEntityRenderWithShadows() && (!model || !(model->flags & OBJ_DONTRECEIVESHADOWMAP)) )
    {
        blend_mode.blend_bits |= BMB_SHADOWMAP;
    }

    assert(blend_mode.shader); // Is this valid?
    return blend_mode;
}

static INLINEDBG BlendModeType modelBlendMode(Model *model, int tex_index, F32 dist, TexBind **materials, bool draw_white, TexBind **eff_bind)
{
    BlendModeType blend_mode;

    *eff_bind = avsn_params.materials[tex_index==-1?0:tex_index];

    if (draw_white)
    {
        blend_mode = BlendMode(0,0);
    }
    else
    {
        const ScrollsScales *scrollsScales = (*eff_bind) ? (*eff_bind)->scrollsScales : NULL;
        unsigned int bUpgradeToPerPixel = (game_state.shadowDebugFlags & kUpgradeAlwaysToPerPixel);

        blend_mode = (*eff_bind)->bind_blend_mode;

        // Used to be done in modelDrawDirect() in rt_model.c
        if (blend_mode.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL && avsn_params.rgbs)
            blend_mode.shader = BLENDMODE_COLORBLEND_DUAL;

        // If this model should do shadowmap lookups set the bit to choose the correct shader variant
        // @todo here is where we can check if there is an asset flag that does not do shadowmaps on particular objects
        // Currently we are not drawing any shadows on 'interior' (objects in Trays) so don't use
        // the more expensive shadowmap shader variants.
        // @todo this logic will have to change if we ever start drawing world model geo shadows on interiors
        // (though it is more likely we would start by drawing character shadows, but may need to do props/attachments
        // like hats and weapons as well...though players probably wouldn't care about the omission)
        if (!avsn_params.from_tray && rdrShadowMapCanWorldRenderWithShadows() && (!model || !(model->flags & OBJ_DONTRECEIVESHADOWMAP)))
        {
            blend_mode.blend_bits |= BMB_SHADOWMAP;

            if ( (game_state.shadowDebugFlags & kUpgradeShadowedToPerPixel) )
            {
                bUpgradeToPerPixel = true;
            }
        }

        // We now have a means to optionally 'upgrade' some of the older materials to do per pixel
        // lighting. It also allows more control when rendering with shadowmaps in order to reduce artifacts
        // We don't upgrade if material is using baked vertex lighting from an ambient group
        if (bUpgradeToPerPixel && !avsn_params.rgbs)
        {
            if ( blend_mode.shader >= BLENDMODE_MODULATE &&  blend_mode.shader <= BLENDMODE_BUMPMAP_MULTIPLY )
            {
                // "High Quality" mode for these simple non bump mapping materials means to
                // use per pixel lighting.
                blend_mode.blend_bits |= BMB_HIGH_QUALITY;
            }
        }

        // set cubemap blendbits if this trick includes reflection and we support cubemaps on this render pass
        if (isReflective(scrollsScales, false))
        {
            if( blend_mode.shader == BLENDMODE_MULTIPLY || blend_mode.shader == BLENDMODE_MULTI )
            {
                // enable cubemap shader bit, which tells shader to use cubemap instead of reflection texture
                blend_mode.blend_bits |= BMB_CUBEMAP;
                if( blend_mode.shader == BLENDMODE_MULTI && rdr_caps.features & GFXF_MULTITEX_HQBUMP && game_state.reflectNormals )
                    blend_mode.blend_bits |= BMB_HIGH_QUALITY; // force high quality for cubemaps on multi shader (to get per pixel normal reflections)
            }
            else if ( blend_mode.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL )
            {
                // use cubemap variant of BLENDMODE_BUMPMAP_COLORBLEND_DUAL shader
                blend_mode.blend_bits |= BMB_CUBEMAP;

                if (rdr_caps.features & GFXF_HQBUMP)
                    blend_mode.blend_bits |= BMB_HIGH_QUALITY;
            }
            else
            {
                blend_mode.blend_bits &= ~BMB_CUBEMAP;
            }
        }
        else
        {
            blend_mode.blend_bits &= ~BMB_CUBEMAP;
        }

        // force high quality mode for non-skinned character parts.
        if (blend_mode.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL &&
            (rdr_caps.features & GFXF_HQBUMP) &&
            rdr_view_state.renderPass == RENDERPASS_COLOR && 
            !(blend_mode.blend_bits&BMB_HIGH_QUALITY))
        {
            blend_mode.blend_bits |= BMB_HIGH_QUALITY;
        }

        // special case for non-skinned character parts (eg shield).  Still want to use high_quality mode 
        //    when close, same as we do for skinned objects.
        if( gfx_state.mainViewport &&
                (blend_mode.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL && /*ABS(dist) < HQ_BUMP_DIST &&*/ (rdr_caps.features & GFXF_HQBUMP)) ||
                (blend_mode.shader == BLENDMODE_MULTI && (rdr_caps.features & GFXF_MULTITEX_HQBUMP) && gfx_state.doCubemaps && game_state.reflectNormals) )
            blend_mode.blend_bits |= BMB_HIGH_QUALITY;

        if (gfx_state.lod_model_override > 0)
            blend_mode.blend_bits &= ~BMB_HIGH_QUALITY;
    }

    return blend_mode;
}
/*
// Unused code, commented out 2/10/10
BlendModeType modelSubBlendMode(Model *model, TextureOverride *custom_texbinds, int require_replaceable, int swapIndex, int tex_index, TexBind **tex_bind)
{
    bool temp;
    *tex_bind = modelSubGetBaseTexture(model, custom_texbinds, require_replaceable, swapIndex, tex_index, &temp);
    if (tex_index==-1)
        return blendModeFilter(model->blend_modes[0], true);
    return swappedBlendMode(model->blend_modes[tex_index==-1?0:tex_index], (*tex_bind)->bind_blend_mode, true);
}
*/

static INLINEDBG bool nodeAlphaSort(GfxNode *node)
{
    return node && (node->flags & GFXNODE_ALPHASORT);
}

static INLINEDBG bool modelAlphaSort(Model *model)
{
    return model && ((model->flags & OBJ_ALPHASORT) && !(model->flags & OBJ_FANCYWATER));
}

static INLINEDBG bool modelAlphaSortEx(Model *model, TexBind **binds)
{
    int i;
    if (((model->flags & OBJ_ALPHASORT) && !(model->flags & OBJ_FANCYWATER)))
        return true;
    if (model->flags & (OBJ_FORCEOPAQUE|OBJ_TREEDRAW))
        return false;
    for (i=0; i<model->tex_count; i++) 
        if (binds[i]->needs_alphasort)
            return true;
    return false;
}

AVSNParams avsn_params;

static INLINEDBG float distanceToPlane2(const Vec3 pos, const Vec4 plane)
{
    float dist = dotVec3(pos, plane) - plane[3];

    return dist;
}


// Takes four points from a quad in view space and returns a reflection score
// p0 = lr, p1 = ll, p2 = ur, p3 = ul
static INLINEDBG F32 calculateQuadReflectionScore(const Vec3 p0, const Vec3 p1, const Vec3 p2, const Vec3 p3, bool water)
{
#define MAX_CLIPPED_TRIS 40
    Vec3 cross, normal;
    F32 skew;
    F32 cross_length;
    F32 distance;
    F32 scale;
    Vec3 center;
    F32 visibleSize;
    Vec3 clipped_tris[MAX_CLIPPED_TRIS][3];
    int num_clipped_tris = 2;
    int i;

    copyVec3(p0, clipped_tris[0][0]);
    copyVec3(p1, clipped_tris[0][1]);
    copyVec3(p3, clipped_tris[0][2]);

    copyVec3(p3, clipped_tris[1][0]);
    copyVec3(p2, clipped_tris[1][1]);
    copyVec3(p0, clipped_tris[1][2]);

    // Clip the quad corners to the frustum
    for (i = 0; i < 6; i++)
    {
        int j;
        int next_num_clipped_tris = num_clipped_tris;

        /*
        if (game_state.test6 && (i + 1) != game_state.test6)
            continue;
        */

        for (j = 0; j < num_clipped_tris; j++)
        {
            F32 d0 = distanceToPlane2(clipped_tris[j][0], cam_info.view_frustum[i]);
            F32 d1 = distanceToPlane2(clipped_tris[j][1], cam_info.view_frustum[i]);
            F32 d2 = distanceToPlane2(clipped_tris[j][2], cam_info.view_frustum[i]);

            if (d0 >= 0 && d1 >= 0 && d2 >= 0)
                continue;

            if (d0 < 0 && d1 < 0 && d2 < 0)
            {
                // remove the triangle from the list
                num_clipped_tris--;
                if (num_clipped_tris > 0 && num_clipped_tris > j)
                {
                    copyVec3(clipped_tris[num_clipped_tris][0], clipped_tris[j][0]);
                    copyVec3(clipped_tris[num_clipped_tris][1], clipped_tris[j][1]);
                    copyVec3(clipped_tris[num_clipped_tris][2], clipped_tris[j][2]);
                }

                next_num_clipped_tris--;
                if (next_num_clipped_tris > num_clipped_tris)
                {
                    copyVec3(clipped_tris[next_num_clipped_tris][0], clipped_tris[num_clipped_tris][0]);
                    copyVec3(clipped_tris[next_num_clipped_tris][1], clipped_tris[num_clipped_tris][1]);
                    copyVec3(clipped_tris[next_num_clipped_tris][2], clipped_tris[num_clipped_tris][2]);
                }

                j--;
                continue;
            }

            // At least one point is in the plane and at least one is out
            // so rotate the points until the first point is visible and second is not
            while (d0 < 0 || d1 >= 0)
            {
                F32 tempd;
                Vec3 tempvec;

                tempd = d0;
                copyVec3(clipped_tris[j][0], tempvec);

                d0 = d1;
                copyVec3(clipped_tris[j][1], clipped_tris[j][0]);

                d1 = d2;
                copyVec3(clipped_tris[j][2], clipped_tris[j][1]);

                d2 = tempd;
                copyVec3(tempvec, clipped_tris[j][2]);
            }

            if (d1 < 0 && d2 < 0)
            {
                // make point 1 = clip 0->1 and point 2 = clip 0->2
                intersectPlane2(clipped_tris[j][0], clipped_tris[j][1], cam_info.view_frustum[i], d0, d1, clipped_tris[j][1]);
                intersectPlane2(clipped_tris[j][0], clipped_tris[j][2], cam_info.view_frustum[i], d0, d2, clipped_tris[j][2]);
            }
            else
            {
                assert(d1 < 0 && d2 >= 0);
                assert(next_num_clipped_tris + 1 < MAX_CLIPPED_TRIS);

                // need a new triangle
                intersectPlane2(clipped_tris[j][0], clipped_tris[j][1], cam_info.view_frustum[i], d0, d1, clipped_tris[next_num_clipped_tris][0]);
                intersectPlane2(clipped_tris[j][2], clipped_tris[j][1], cam_info.view_frustum[i], d2, d1, clipped_tris[next_num_clipped_tris][1]);
                copyVec3(clipped_tris[j][2], clipped_tris[next_num_clipped_tris][2]);

                // Modify old triangle
                copyVec3(clipped_tris[next_num_clipped_tris][0], clipped_tris[j][1]);

                next_num_clipped_tris++;
            }
        }

        num_clipped_tris = next_num_clipped_tris;
    }

    if (num_clipped_tris == 0)
        return -FLT_MAX;

    zeroVec3(center);
    zeroVec3(cross);
    cross_length = 0.0f;

    scale = 1.0f / 3.0f;//(3.0f * num_clipped_tris);
    for (i = 0; i < num_clipped_tris; i++)
    {
        Vec3 temp1, temp2, tempcross;
        F32 tempcrosslength;

        // Calculate visible size
        subVec3(clipped_tris[i][2], clipped_tris[i][0], temp1);
        subVec3(clipped_tris[i][1], clipped_tris[i][0], temp2);
        crossVec3(temp1, temp2, tempcross);

        addVec3(cross, tempcross, cross);

        tempcrosslength = lengthVec3(tempcross);

        // Calculate face center distance
        scaleAddVec3(clipped_tris[i][0], scale * tempcrosslength, center, center);
        scaleAddVec3(clipped_tris[i][1], scale * tempcrosslength, center, center);
        scaleAddVec3(clipped_tris[i][2], scale * tempcrosslength, center, center);

        cross_length += tempcrosslength;
    }

    scaleVec3(center, 1.0f/cross_length, center);

    // If the quad faces away from the camera, ignore it
    if (dotVec3(center, cross) < 0)
        return -FLT_MAX;

    distance = lengthVec3(center);

    // Ignore large buildings in the distance
    if (!water && distance > 400)
        return -FLT_MAX;

    visibleSize = lengthVec3(cross);

    scaleVec3(cross, 1.0f/visibleSize, normal);

    // skew is to skew the score to pick better planes
    skew = -normal[2];                // prefer planes which are more perpendicular to the camera
    skew += -center[2]/distance;    // prefer planes which are closer to the center of the screen
    skew /= SQR(distance);            // give bonus for being closer to the camera

    if (game_state.reflectionDebug == 2)
    {
        Vec3 center_ws, center_offset_ws;
        Vec3 center_offset;

        scaleAddVec3(normal, -10.0f, center, center_offset);

        mulVecMat4(center, cam_info.inv_viewmat, center_ws);
        mulVecMat4(center_offset, cam_info.inv_viewmat, center_offset_ws);
        drawLine3D(center_ws, center_offset_ws, 0xFF0000FF);

        for (i = 0; i < num_clipped_tris; i++)
        {
            Vec3 verts[3];

            mulVecMat4(clipped_tris[i][0], cam_info.inv_viewmat, verts[0]);
            mulVecMat4(clipped_tris[i][1], cam_info.inv_viewmat, verts[1]);
            mulVecMat4(clipped_tris[i][2], cam_info.inv_viewmat, verts[2]);

            drawLine3D(verts[0], verts[1], 0xFFFFFFFF);
            drawLine3D(verts[2], verts[1], 0xFFFFFFFF);
            drawLine3D(verts[0], verts[2], 0xFFFFFFFF);
        }
    }

    return visibleSize * skew;
#undef MAX_CLIPPED_TRIS
}

static INLINEDBG void addViewSortNode_Water(Model *waterModel, const Mat4 mat)
{
    Vec3 min, max, eye_bounds[8];
    Vec3 min_ws, max_ws;
    Mat4 matModelToWorld;
    F32 score;

    assert(waterModel && (waterModel->flags & OBJ_FANCYWATER));
    
    // Reset the blend mode if the water mode changed
    if ((game_state.waterMode >  WATER_OFF && waterModel->tex_binds[0]->bind_blend_mode.shader != BLENDMODE_WATER) ||
        (game_state.waterMode == WATER_OFF && waterModel->tex_binds[0]->bind_blend_mode.shader == BLENDMODE_WATER))
    {
        texSetBindsSubComposite(waterModel->tex_binds[0]);
    }

    // Only do reflections on main viewport
    // gfx_state.waterPlane needs to be computed for refractions and reflections
    if (!gfx_state.mainViewport)
        return;

    // Calculate Bounding box corners in world space
    copyVec3(waterModel->min, min);
    copyVec3(waterModel->max, max);
    mulBounds(min, max, mat, eye_bounds);

    // See if the water is not level
    mulMat4Inline(cam_info.inv_viewmat, avsn_params.mat, matModelToWorld);
    
    mulVecMat4(min, matModelToWorld, min_ws);
    mulVecMat4(max, matModelToWorld, max_ws);

    // Ignore water with a large slope
    if (SQR(max_ws[1] - min_ws[1])/(SQR(max_ws[0] - min_ws[0]) + SQR(max_ws[2] - min_ws[2])) > 0.1f)
    {
        return;
    }

    // calculate the reflection score for this water model
    score = calculateQuadReflectionScore(eye_bounds[7], eye_bounds[6], eye_bounds[3], eye_bounds[2], true);

    // grab the water plane of the water model closest to the camera
    if (score > gfx_state.waterReflectionScore)
    {
        gfx_state.waterReflectionScore = score;

        gfx_state.waterPlane[0] = 0.0f;
        gfx_state.waterPlane[1] = 1.0f;
        gfx_state.waterPlane[2] = 0.0f;
        gfx_state.waterPlane[3] = (max_ws[1] + min_ws[1]) * -0.5f;

        gfx_state.waterThisFrame = 1; // water pass needs to be done

    }

    if (game_state.reflectionDebug == 1 || game_state.reflectionDebug == 100)
    {
        Vec3 temp1, temp2;
        Vec3 bounds_ws[8];
        unsigned int color = 0xFF8080FF;
        int i;

        for (i = 0; i < 8; i++)
        {
            mulVecMat4(eye_bounds[i], cam_info.inv_viewmat, temp1);

            // Move the vert slightly closer to the camera so they 
            // show up on top of water
            subVec3(cam_info.cammat[3], temp1, temp2);
            normalVec3(temp2);
            scaleVec3(temp2, 3.0f, temp2);
            addVec3(temp1, temp2, bounds_ws[i]);
        }

        drawLine3DWidth(bounds_ws[0], bounds_ws[1], color, 1.0f);
        drawLine3DWidth(bounds_ws[2], bounds_ws[3], color, 1.0f);
        drawLine3DWidth(bounds_ws[4], bounds_ws[5], color, 1.0f);
        drawLine3DWidth(bounds_ws[6], bounds_ws[7], color, 1.0f);

        drawLine3DWidth(bounds_ws[0], bounds_ws[2], color, 1.0f);
        drawLine3DWidth(bounds_ws[1], bounds_ws[3], color, 1.0f);
        drawLine3DWidth(bounds_ws[4], bounds_ws[6], color, 1.0f);
        drawLine3DWidth(bounds_ws[5], bounds_ws[7], color, 1.0f);

        drawLine3DWidth(bounds_ws[0], bounds_ws[4], color, 1.0f);
        drawLine3DWidth(bounds_ws[1], bounds_ws[5], color, 1.0f);
        drawLine3DWidth(bounds_ws[2], bounds_ws[6], color, 1.0f);
        drawLine3DWidth(bounds_ws[3], bounds_ws[7], color, 1.0f);
    }
}


#define MAX_REFLECTION_QUADS_PER_MODEL 40
static INLINEDBG U32 processReflectors(Model *model, const Mat4 mat, const Vec3 * reflection_quads_verts, int num_reflection_quads, Vec4 **reflection_planes)
{
    Vec3 quad_verts[MAX_REFLECTION_QUADS_PER_MODEL*4 + 4];
    Vec3 quad_verts_ws[MAX_REFLECTION_QUADS_PER_MODEL*4 + 4];
    int numQuads = 0;
    int i;
    int best_reflector_index = -1;
    register Vec3 *current_quad_vert;
    register Vec3 *current_quad_vert_ws;
    register Vec4 *current_plane;

    // If the cubemap attenuation for the entire model will be 0,
    // don't bother processing any reflection quads of the model.
    if (cubemap_CalculateAttenuation(true, mat[3], model->radius, false) <= 0.0f)
        return 0;

    current_quad_vert = quad_verts;
    current_quad_vert_ws = quad_verts_ws;

    for (i = 0; i < num_reflection_quads; i++)
    {
        F32 score;

        // Transform quad verts
        // NOTE: verts are re-ordered here to what we expect
        mulVecMat4(reflection_quads_verts[i * 4 + 1], mat, current_quad_vert[0]);
        mulVecMat4(current_quad_vert[0], cam_info.inv_viewmat, current_quad_vert_ws[0]);

        mulVecMat4(reflection_quads_verts[i * 4 + 0], mat, current_quad_vert[1]);
        mulVecMat4(current_quad_vert[1], cam_info.inv_viewmat, current_quad_vert_ws[1]);

        mulVecMat4(reflection_quads_verts[i * 4 + 2], mat, current_quad_vert[2]);
        mulVecMat4(current_quad_vert[2], cam_info.inv_viewmat, current_quad_vert_ws[2]);

        mulVecMat4(reflection_quads_verts[i * 4 + 3], mat, current_quad_vert[3]);
        mulVecMat4(current_quad_vert[3], cam_info.inv_viewmat, current_quad_vert_ws[3]);

        // TODO: Might be able to check and see if the reflector is occluded

        // Find the 'best' reflector
        score = calculateQuadReflectionScore(current_quad_vert[0], current_quad_vert[1], current_quad_vert[2], current_quad_vert[3], false);

        // If the reflection quad faces away from the camera, ignore it
        // Minimum score so we can ignore small reflectors
        //if (score < 0)
        if (score < 0.00000001f)
        {
            // Yes, we avoid incrementing the pointers below
            continue;
        }

        if (score > gfx_state.reflectionCandidateScore)
        {
            gfx_state.reflectionCandidateScore = score;
            best_reflector_index = numQuads;
        }

        numQuads++;

        if (numQuads > MAX_REFLECTION_QUADS_PER_MODEL)
        {
            ErrorFilenamef(model->filename, 
                "Too many visible reflection quads in model: %s. Maximum is %d.", model->name, 
                MAX_REFLECTION_QUADS_PER_MODEL);
            return 0;
        }

        current_quad_vert += 4;
        current_quad_vert_ws += 4;
    }

    if (numQuads == 0)
        return 0;

    // Populate reflection plane
    *reflection_planes = GetReflectionPlanes(numQuads);
    if (!*reflection_planes)
        return 0;

    current_quad_vert_ws = quad_verts_ws;
    current_plane = *reflection_planes;

    for (i = 0; i < numQuads; i++)
    {
        Vec3 temp1, temp2, cross;

        // Calculate the candidate reflection plane in world space
        subVec3(current_quad_vert_ws[2], current_quad_vert_ws[0], temp1);
        subVec3(current_quad_vert_ws[1], current_quad_vert_ws[0], temp2);
        crossVec3(temp1, temp2, cross);

        normalVec3(cross);
        copyVec3(cross, *current_plane);

        (*current_plane)[3] = -dotVec3(current_quad_vert_ws[3], *current_plane);

        current_quad_vert_ws += 4;
        current_plane++;
    }

    if (best_reflector_index > -1)
    {
        copyVec4((*reflection_planes)[best_reflector_index], gfx_state.nextFrameReflectionPlane);

        // For debugging
        copyVec3(quad_verts_ws[best_reflector_index*4+0], gfx_state.reflectionQuadVerts[0]);
        copyVec3(quad_verts_ws[best_reflector_index*4+1], gfx_state.reflectionQuadVerts[1]);
        copyVec3(quad_verts_ws[best_reflector_index*4+2], gfx_state.reflectionQuadVerts[2]);
        copyVec3(quad_verts_ws[best_reflector_index*4+3], gfx_state.reflectionQuadVerts[3]);
    }
    
    // Debugging
    if (game_state.reflectionDebug == 1 || game_state.reflectionDebug == 100)
    {
        unsigned int color = 0xFF8080FF;

        current_quad_vert_ws = quad_verts_ws;

        for (i = 0; i < numQuads; i++)
        {
            Vec3 p0_ws, p1_ws, p2_ws, p3_ws;
            Vec3 temp;
            
            // Move the vert slightly closer to the camera so they 
            // show up on top of walls
            subVec3(cam_info.cammat[3], current_quad_vert_ws[0], temp);
            normalVec3(temp);
            addVec3(current_quad_vert_ws[0], temp, p0_ws);

            subVec3(cam_info.cammat[3], current_quad_vert_ws[1], temp);
            normalVec3(temp);
            addVec3(current_quad_vert_ws[1], temp, p1_ws);

            subVec3(cam_info.cammat[3], current_quad_vert_ws[2], temp);
            normalVec3(temp);
            addVec3(current_quad_vert_ws[2], temp, p2_ws);

            subVec3(cam_info.cammat[3], current_quad_vert_ws[3], temp);
            normalVec3(temp);
            addVec3(current_quad_vert_ws[3], temp, p3_ws);

            drawLine3DWidth(p0_ws, p1_ws, color, 1.0f);
            drawLine3DWidth(p1_ws, p3_ws, color, 1.0f);
            drawLine3DWidth(p3_ws, p2_ws, color, 1.0f);
            drawLine3DWidth(p2_ws, p0_ws, color, 1.0f);

            // Draw a line sticking out of the center of the quad
            {
                Vec3 temp1, temp2, cross1, cross2, cross, normal;
                Vec3 center, centerOffset;

                // Calculate visible size
                // dot product ( Cross product of two sides, view space backward ) / distance
                subVec3(current_quad_vert_ws[2], current_quad_vert_ws[0], temp1);
                subVec3(current_quad_vert_ws[1], current_quad_vert_ws[0], temp2);
                crossVec3(temp1, temp2, cross1);

                subVec3(current_quad_vert_ws[1], current_quad_vert_ws[3], temp1);
                subVec3(current_quad_vert_ws[2], current_quad_vert_ws[3], temp2);
                crossVec3(temp1, temp2, cross2);

                cross[0] = cross1[0] + cross2[0];
                cross[1] = cross1[1] + cross2[1];
                cross[2] = cross1[2] + cross2[2];

                copyVec3(cross, normal);
                normalVec3(normal);

                center[0] = (current_quad_vert_ws[0][0] + current_quad_vert_ws[1][0] + current_quad_vert_ws[2][0] + current_quad_vert_ws[3][0]) * 0.25f;
                center[1] = (current_quad_vert_ws[0][1] + current_quad_vert_ws[1][1] + current_quad_vert_ws[2][1] + current_quad_vert_ws[3][1]) * 0.25f;
                center[2] = (current_quad_vert_ws[0][2] + current_quad_vert_ws[1][2] + current_quad_vert_ws[2][2] + current_quad_vert_ws[3][2]) * 0.25f;

                centerOffset[0] = center[0] + normal[0] * 5.0f;
                centerOffset[1] = center[1] + normal[1] * 5.0f;
                centerOffset[2] = center[2] + normal[2] * 5.0f;

                drawLine3DWidth(center, centerOffset, color, 1.0f);
            }

            current_quad_vert_ws += 4;
        }
    }

    return numQuads;
}

void checkLowestPoint(Model *model, const Mat4 mat)
{
    Vec3 min_ws, max_ws;
    Mat4 matModelToWorld;

    assert(model);
    
    // Only do this on main viewport
    // TODO: don't do if shadow maps are not enabled?
    if (!gfx_state.mainViewport)
        return;

    mulMat4Inline(cam_info.inv_viewmat, mat, matModelToWorld);

    // Calculate Bounding box corners in world space
    mulBoundsAA(model->min, model->max, matModelToWorld, min_ws, max_ws);

    if (min_ws[1] < gfx_state.lowest_visible_point_this_frame)
        gfx_state.lowest_visible_point_this_frame = min_ws[1];
}

// Now called only for skinned nodes, cloth objects and splats
// Some of the code below may be able to be pruned if it was left over from world models
void addViewSortNodeSkinned(void)
{
    int                sortType;
    int                debug_tint=0;
    bool            water=false;
    bool            draw_white=false;
    bool            okToSplit=true;
    bool            forceSortType=false;
    Model            *model = avsn_params.model;
    int                needs_alpha;
    int i;

    // This should not be called for the shadow map render pass
    onlydevassert(gfx_state.renderPass < RENDERPASS_SHADOW_CASCADE1 || gfx_state.renderPass > RENDERPASS_SHADOW_CASCADE_LAST);

#ifndef FINAL
    if (game_state.reflectionDebug == 100)
    {
        //if (strcmp(model->name, game_state.reflectiondebugname) != 0)
        if (!model || strstr(model->name, game_state.reflectiondebugname) == NULL)
            goto avsn_exit;
    }
#endif

    //onlydevassert( (fg_list->vsListCount + fg_list->nodeTotal) == (fg_list->typeSortThingsCount + fg_list->distSortThingsCount ) );

    //onlydevassert( !avsn_params.node || !avsn_params.custom_texbinds); // node -> !custom_texbinds

    //onlydevassert( avsn_params.node ); // World geometry no longer goes through here

    //onlydevassert(!_isnan(avsn_params.mid[2]));

    // Filter out small objects in reflection viewport
    if (gfx_state.current_viewport_info->renderPass == RENDERPASS_REFLECTION && model && (avsn_params.mid[2] < 0))
    {
        if ((model->radius / -avsn_params.mid[2]) < game_state.water_reflection_min_screen_size)
            goto avsn_exit;
    }

    //Figure out how this node will be sorted: 
    if (model && model->flags & OBJ_FANCYWATER) {
        addViewSortNode_Water(model, avsn_params.mat);
        water = true;
    }

    needs_alpha = nodeAlphaSort(avsn_params.node) || modelAlphaSort(model);
    if (avsn_params.alpha==255 && water)
    {
        sortType = SORT_BY_DIST;
        forceSortType = true;
    }
    else if (avsn_params.alpha == 255 && !needs_alpha)
    {
        sortType = SORT_BY_TYPE;    
    }
    else
    {
        if (avsn_params.alpha!=255)
            forceSortType=true;
        sortType = SORT_BY_DIST;
    }

#ifndef FINAL
    if (game_state.perf)
    {
        if (game_state.perf & GFXDEBUG_PERF_SKIP_SORT_BY_TYPE && sortType == SORT_BY_TYPE)
            goto avsn_exit;

        if (game_state.perf & GFXDEBUG_PERF_SKIP_SORT_BY_DIST && sortType == SORT_BY_DIST)
            goto avsn_exit;

        if (game_state.perf & GFXDEBUG_PERF_FORCE_SORT_BY_TYPE)
            sortType = SORT_BY_TYPE;
        else if (game_state.perf & GFXDEBUG_PERF_FORCE_SORT_BY_DIST) {
            sortType = SORT_BY_DIST;
            forceSortType = true;
        }
    }
#endif

    assert( !model || model->loadstate & LOADED );

    if (draw_white)
        okToSplit=false;

    // Toss yourself into the right bin 
    {
        F32 dist = avsn_params.mid[2];
        SortThing st = { 0 };
        SortThing* stp = NULL;

        // Setup SortThing
        if( sortType == SORT_BY_DIST )
        {
            if (model && !avsn_params.noUseRadius)
            {
                int useRadius = 1;
                TrickNode *trick;

                if (trick=model->trick)
                {
                    if (trick->info) {
                        dist += trick->info->alpha_sort_mod;
                        if (trick->info->group_flags & GROUP_PARENT_FADE)
                            useRadius = 0;
                    }
                    if( trick->flags1 & TRICK_SIMPLEALPHASORT )
                        useRadius = 0;
                }
                if( useRadius )
                    dist -=  model->radius * 2.f; 
            }
        }
        st.dist = dist;
        st.model = model;
        st.alpha = avsn_params.alpha;
        st.draw_white = draw_white;
        st.is_gfxnode = avsn_params.is_gfxnode;

        copyMat4(avsn_params.mat, st.viewMat);

        fg_list->nodeTotal++; //debug
        st.modelSource    = SORTTHING_GFXTREENODE;
        st.gfxnode        = avsn_params.node;
        if (draw_white)
        {
            st.blendMode.intval = 0;
            st.tex_bind            = NULL;
        }

        // Don't split models flagged as FORCEOPAQUE, they probably have a specific draw order
        //   because of alpha'd sub-objects
        // Exception if AllowMultiSplit is on (automatically on for Trays, since they are automatically ForceOpaque)
        if (sortType == SORT_BY_TYPE && model && (model->flags & (OBJ_FORCEOPAQUE|OBJ_ALLOW_MULTISPLIT))==OBJ_FORCEOPAQUE)
            okToSplit = false;
        if (!model)
            okToSplit = false;
        if (model && (model->flags & OBJ_ALPHASORTALL))
            forceSortType = true;
        if (avsn_params.node->customtex.generic) // May change alpha sort needs and code below doesn't know about it
            forceSortType = true;

        {
            int count=1;
            // Loop and insert each sub individually into the appropriate list
            if (okToSplit)
                count = model->tex_count;

            for (i=0; i<count; i++)
            {
                int mySortType=sortType;
                BlendModeType blend_mode;
                TexBind *tex_bind;
                int tex_index;

                if (okToSplit || model && model->tex_count==1)
                    tex_index    = i;
                else
                    tex_index    = -1;

                // We need the accurate blend mode and tex_bind for each tex_index on
                //    this node/model in order to sort right
                blend_mode = nodeBlendMode(avsn_params.node, model, tex_index, dist, &tex_bind);
                //tex_bind set by function

                // If SORT_BY_DIST, and a sub does not require alpha sorting, sort it by type
                if (mySortType==SORT_BY_DIST && !forceSortType && okToSplit && tex_bind && !texNeedsAlphaSort(tex_bind, blend_mode))
                {
                    mySortType = SORT_BY_TYPE;
                }

                // Insert an entry per object/sub-object
                if( mySortType == SORT_BY_DIST )
                {
                    stp = dynArrayAdd( &fg_list->distSortThings, sizeof(SortThing), &fg_list->distSortThingsCount, &fg_list->distSortThingsMax, 1);
                    sortedByDist++;
                }
                else if( mySortType == SORT_BY_TYPE )
                {
                    stp = dynArrayAdd( &fg_list->typeSortThings, sizeof(SortThing), &fg_list->typeSortThingsCount, &fg_list->typeSortThingsMax, 1);
                    sortedByType++;
                }
                if (i>0)
                    fg_list->nodeTotal++; // Not a node, but not adding to vsListCount, so use this for debug

                *stp = st;
                stp->tex_index    = tex_index;
                stp->blendMode = blend_mode;
                stp->tex_bind = tex_bind;
                stp->has_translucency = tex_bind && texNeedsAlphaSort(tex_bind, blend_mode) && okToSplit && !(avsn_params.node->tricks && (avsn_params.node->tricks->flags1 & TRICK_ALPHACUTOUT));
                stp->was_type_sorted = mySortType==SORT_BY_TYPE;
                stp->alpha_test = stp->was_type_sorted && (avsn_params.node->tricks && (avsn_params.node->tricks->flags1 & TRICK_ALPHACUTOUT));
                stp->is_tree = false;
                
                stp->tint = TINT_NONE;

#ifndef FINAL
                if (gfx_state.mainViewport && game_state.tintAlphaObjects)
                {
                    stp->tint = (mySortType == SORT_BY_DIST) ? TINT_RED : TINT_GREEN;

                    if ((game_state.tintAlphaObjects > 1) && model && model->trick && (model->trick->flags2 & TRICK2_ALPHAREF))
                    {
                        stp->tint = TINT_PURPLE;
                    }

                    if ((game_state.tintAlphaObjects > 1) && model && model->trick && (model->trick->flags1 & TRICK_ALPHACUTOUT))
                    {
                        stp->tint = TINT_BLUE;
                    }
                }


                if (gfx_state.mainViewport && game_state.tintOccluders)
                {
                    switch (avsn_params.isOccluder)
                    {
                    case -1:
                        stp->tint = TINT_BLUE;
                        break;

                    case 0:
                        stp->tint = TINT_RED;
                        break;

                    case 1:
                        stp->tint = TINT_GREEN;
                        break;
                    }
                }
#endif // !FINAL
            }
        }
    }

avsn_exit:

    //onlydevassert( (fg_list->vsListCount + fg_list->nodeTotal) == (fg_list->typeSortThingsCount + fg_list->distSortThingsCount) );

    // Zero the params
    ZeroStruct(&avsn_params);
    avsn_params.uid = -1;

    return;
}


void addViewSortNodeWorldModel(void)
{
    // Anything that causes a forced non-split does *not* require alpha sorting
    // Otherwise look at the flag on the TexBind

    ViewSortNode    *vs = 0;
    int                sortType;
    int                vsIndex = -1;
    bool            water=false;
    bool            draw_white=false;
    bool            okToSplit=true;
    bool            forceSortType=false;
    Model            *model = avsn_params.model;
    bool            forceOpaque = (model && (model->flags & (OBJ_FORCEOPAQUE|OBJ_ALLOW_MULTISPLIT))==OBJ_FORCEOPAQUE);
    int                needs_alpha;
    int i;

    // This should not be called for the shadow map render pass
    onlydevassert(gfx_state.renderPass < RENDERPASS_SHADOW_CASCADE1 || gfx_state.renderPass > RENDERPASS_SHADOW_CASCADE_LAST);

#ifndef FINAL
    if (game_state.reflectionDebug == 100)
    {
        //if (strcmp(model->name, game_state.reflectiondebugname) != 0)
        if (!model || strstr(model->name, game_state.reflectiondebugname) == NULL)
            goto avsn2_exit;
    }
#endif

    //onlydevassert( (fg_list->vsListCount + fg_list->nodeTotal) == (fg_list->typeSortThingsCount + fg_list->distSortThingsCount ) );

    //onlydevassert( !avsn_params.node );

    //onlydevassert(!_isnan(avsn_params.mid[2]));

    // Filter out small objects in reflection viewport
    if (gfx_state.current_viewport_info && gfx_state.current_viewport_info->renderPass == RENDERPASS_REFLECTION && model && (avsn_params.mid[2] < 0))
    {
        if ((model->radius / -avsn_params.mid[2]) < game_state.water_reflection_min_screen_size)
            goto avsn2_exit;
    }

    //Figure out how this node will be sorted: 
    if (model && model->flags & OBJ_FANCYWATER) {
        addViewSortNode_Water(model, avsn_params.mat);
        water = true;
    }

    if (model)
    {
        checkLowestPoint(model, avsn_params.mat);
    }

    // TODO: Deal with GfxNodes (do not make a ViewSortNode?  Make new SortThing type?)
    needs_alpha = modelAlphaSortEx(model, avsn_params.materials);

    if (avsn_params.alpha==255 && water)
    {
        sortType = SORT_BY_DIST;
        forceSortType = true;
    }
    else if (avsn_params.alpha == 255 && !needs_alpha)
    {
        sortType = SORT_BY_TYPE;    
    }
    else
    {
        if (avsn_params.alpha!=255)
            forceSortType=true;
        sortType = SORT_BY_DIST; // At least one sub needs sorting by distance
    }

#ifndef FINAL
    if (game_state.perf)
    {
        if (game_state.perf & GFXDEBUG_PERF_SKIP_SORT_BY_TYPE && sortType == SORT_BY_TYPE)
            goto avsn2_exit;

        if (game_state.perf & GFXDEBUG_PERF_SKIP_SORT_BY_DIST && sortType == SORT_BY_DIST)
            goto avsn2_exit;

        if (game_state.perf & GFXDEBUG_PERF_FORCE_SORT_BY_TYPE)
            sortType = SORT_BY_TYPE;
        else if (game_state.perf & GFXDEBUG_PERF_FORCE_SORT_BY_DIST) {
            sortType = SORT_BY_DIST;
            forceSortType = true;
        }
    }
#endif

    //Start If you are a world node, construct a node for yourself
    vs = dynArrayAdd(&fg_list->vsList,sizeof(ViewSortNode),&fg_list->vsListCount,&fg_list->vsListMax,1);
    vsIndex = fg_list->vsListCount - 1;
    modelSetupVertexObject(model, VBOS_USE, avsn_params.materials[0]->bind_blend_mode);
    initViewSortModelInline2(vs);
    draw_white = vs->draw_white = avsn_params.beyond_fog && !needs_alpha && !(model && model->flags & OBJ_TREEDRAW) && !(model && model->trick && (model->trick->flags1 & (TRICK_ALPHACUTOUT|TRICK_ADDITIVE|TRICK_SUBTRACTIVE)));
    assert( model && model->loadstate & LOADED );

    if (draw_white || forceOpaque)
        okToSplit = false;

    // Tint the beyond fog geometry with the fog color so they are not solid white.
    // If they are solid white and MSAA is enabled and SSAO is enabled,
    // Foreground models may end up with a white edge.
    if (avsn_params.beyond_fog && draw_white)
    {
        static U8 fogTintColors[8];

        vs->tint_colors[0][0] = vs->tint_colors[1][0] = g_sun.fogcolor[0] * 255;
        vs->tint_colors[0][1] = vs->tint_colors[1][1] = g_sun.fogcolor[1] * 255;
        vs->tint_colors[0][2] = vs->tint_colors[1][2] = g_sun.fogcolor[2] * 255;
        vs->tint_colors[0][3] = vs->tint_colors[1][3] = 255;

        vs->has_tint = 1;
    }

    // Need to know if any sub is going to need SORT_BY_DIST by this point

    // Toss yourself into the right bin 
    {
        F32 dist = avsn_params.mid[2];
        SortThing st = { 0 };
        SortThing* stp = NULL;

        // Setup SortThing
        if( sortType == SORT_BY_DIST )
        {
            if (model && !avsn_params.noUseRadius)
            {
                int useRadius = 1;
                TrickNode *trick;

                if (trick=model->trick)
                {
                    if (trick->info) {
                        dist += trick->info->alpha_sort_mod;
                        if (trick->info->group_flags & GROUP_PARENT_FADE)
                            useRadius = 0;
                    }
                    if( trick->flags1 & TRICK_SIMPLEALPHASORT )
                        useRadius = 0;
                }
                if( useRadius )
                    dist -=  model->radius * 2.f; 
            }
        }
        st.dist = dist;
        st.model = model;
        st.alpha = avsn_params.alpha;
        st.draw_white = draw_white;
        st.is_gfxnode = avsn_params.is_gfxnode;
        
        copyMat4(avsn_params.mat, st.viewMat);

        st.modelSource    = SORTTHING_WORLDMODEL;
        st.vsIdx        = vsIndex;
        vs->hide = avsn_params.hide;
        if (draw_white)
        {
            st.blendMode.intval = 0;
            st.tex_bind            = NULL;
        }

        // Don't split models flagged as FORCEOPAQUE, they probably have a specific draw order
        //   because of alpha'd sub-objects
        // Exception if AllowMultiSplit is on (automatically on for Trays, since they are automatically ForceOpaque)

        //if (!model)
        //    okToSplit = false;
        if (model && (model->flags & OBJ_ALPHASORTALL))
            forceSortType = true;

        {
            int count=1;
            // Loop and insert each sub individually into the appropriate list
            if (okToSplit)
                count = model->tex_count;

            for (i=0; i<count; i++)
            {
                int mySortType=sortType;
                BlendModeType blend_mode;
                TexBind *tex_bind;
                int tex_index;

                if (okToSplit || model && model->tex_count==1)
                    tex_index    = i;
                else
                    tex_index    = -1;

                // We need the accurate blend mode and tex_bind for each tex_index on
                //    this node/model in order to sort right
                blend_mode = modelBlendMode(model, tex_index, dist, avsn_params.materials, draw_white, &tex_bind);
                //tex_bind set by function

                // If SORT_BY_DIST, and a sub does not require alpha sorting, sort it by type
                if (mySortType==SORT_BY_DIST && !forceSortType && okToSplit && tex_bind && !tex_bind->needs_alphasort)
                {
                    mySortType = SORT_BY_TYPE;
                }

                // Insert an entry per object/sub-object
                if( mySortType == SORT_BY_DIST )
                {
                    stp = dynArrayAdd( &fg_list->distSortThings, sizeof(SortThing), &fg_list->distSortThingsCount, &fg_list->distSortThingsMax, 1);
                    sortedByDist++;
                }
                else if( mySortType == SORT_BY_TYPE )
                {
                    stp = dynArrayAdd( &fg_list->typeSortThings, sizeof(SortThing), &fg_list->typeSortThingsCount, &fg_list->typeSortThingsMax, 1);
                    sortedByType++;
                }
                if (i>0)
                    fg_list->nodeTotal++; // Not a node, but not adding to vsListCount, so use this for debug

                *stp = st;
                stp->tex_index    = tex_index;
                stp->blendMode = blend_mode;
                stp->tex_bind = tex_bind;
                stp->was_type_sorted = mySortType==SORT_BY_TYPE;
                stp->alpha_test = stp->was_type_sorted && model && model->trick && (model->trick->flags1 & TRICK_ALPHACUTOUT);
                stp->is_tree = (model != NULL) && (model->flags & OBJ_TREEDRAW) != 0;

                stp->tint = TINT_NONE;

#ifndef FINAL
                if (gfx_state.mainViewport && game_state.tintAlphaObjects)
                {
                    stp->tint = (mySortType == SORT_BY_DIST) ? TINT_RED : TINT_GREEN;

                    if ((game_state.tintAlphaObjects > 1) && model->trick && (model->trick->flags2 & TRICK2_ALPHAREF))
                    {
                        stp->tint = TINT_PURPLE;
                    }

                    if ((game_state.tintAlphaObjects > 1) && model->trick && (model->trick->flags1 & TRICK_ALPHACUTOUT))
                    {
                        stp->tint = TINT_BLUE;
                    }
                }


                if (gfx_state.mainViewport && game_state.tintOccluders)
                {
                    switch (avsn_params.isOccluder)
                    {
                    case -1:
                        stp->tint = TINT_BLUE;
                        break;

                    case 0:
                        stp->tint = TINT_RED;
                        break;

                    case 1:
                        stp->tint = TINT_GREEN;
                        break;
                    }
                }
#endif // !FINAL
            }
        }

        // Don't process reflections quads for building/objects beyond the fog
        if (gfx_state.doPlanarReflections && !avsn_params.beyond_fog && avsn_params.num_reflection_quads)
        {
            vs->num_reflection_planes = processReflectors(model, avsn_params.mat, avsn_params.reflection_quad_verts, avsn_params.num_reflection_quads, &vs->reflection_planes);
        }
    }

avsn2_exit:

    //onlydevassert( (fg_list->vsListCount + fg_list->nodeTotal) == (fg_list->typeSortThingsCount + fg_list->distSortThingsCount) );

    // Zero the params
    ZeroStruct(&avsn_params);
    avsn_params.uid = -1;
}


static int cmpRandom(const void* aData, const void* bData)
{
    const SortThing* a = (const SortThing*)aData;
    const SortThing* b = (const SortThing*)bData;
    return rand()*3/(RAND_MAX+1) - 1;
}

static int cmpSortThingsType(const void* aData, const void* bData)
{
    const SortThing* sta = (const SortThing*)aData;
    const SortThing* stb = (const SortThing*)bData;
    int t;

    t = sta->has_translucency - stb->has_translucency;
    if (t)
        return t;

    t = sta->draw_white - stb->draw_white;
    if (t)
        return t;

    t = sta->blendMode.shader - stb->blendMode.shader;
    if (t)
        return t;

    t = sta->blendMode.blend_bits - stb->blendMode.blend_bits;
    if (t)
        return t;

    t = sta->tex_bind - stb->tex_bind;
    if (t)
        return t;
    
    return sta->model - stb->model;
}

static int cmpSortThingsModel(const void* aData, const void* bData)
{
    const SortThing* sta = (const SortThing*)aData;
    const SortThing* stb = (const SortThing*)bData;
    return sta->model - stb->model;
}

static int cmpSortThingsShadows(const void* aData, const void* bData)
{
    const SortThing* sta = (const SortThing*)aData;
    const SortThing* stb = (const SortThing*)bData;
    int t;
    
    // shadows will draw with either depth or depthalpha shader, depending
    //    on whether alphatest is set.  So we only need to sort into these 
    //    two groups.  For now also sort trees separately since they use
    //    the full color shader.    
    t = sta->is_tree - stb->is_tree;
    if (t)
        return t;

    if( sta->is_tree )
    {
        // both are trees, use regular type sort
        return cmpSortThingsType(sta, stb);
    }
    
    // non-tree, only need to sort by alpha test
    t = sta->alpha_test - stb->alpha_test;
    if (t)
        return t;

    return sta->model - stb->model;
}

static int cmpGfxNodeBones(GfxNode *node1, GfxNode *node2)
{
    // CD: I could put these in a static array, but I think this is faster

    if (node1->anim_id == BONEID_HAIR)
        return -1;
    if (node2->anim_id == BONEID_HAIR)
        return 1;

    if (node1->anim_id == BONEID_EMBLEM)
        return -1;
    if (node2->anim_id == BONEID_EMBLEM)
        return 1;

    if (node1->anim_id == BONEID_BELT)
        return -1;
    if (node2->anim_id == BONEID_BELT)
        return 1;

    if (node1->anim_id == BONEID_TOP)
        return -1;
    if (node2->anim_id == BONEID_TOP)
        return 1;

    if (node1->anim_id == BONEID_SKIRT)
        return -1;
    if (node2->anim_id == BONEID_SKIRT)
        return 1;

    if (node1->anim_id == BONEID_SLEEVES)
        return -1;
    if (node2->anim_id == BONEID_SLEEVES)
        return 1;

    if (node1->anim_id == BONEID_LLEGR || node1->anim_id == BONEID_LLEGL)
        return -1;
    if (node2->anim_id == BONEID_LLEGR || node2->anim_id == BONEID_LLEGL)
        return 1;

    // (for pants that cover the lower legs)
    if (node1->anim_id == BONEID_HIPS)
        return -1;
    if (node2->anim_id == BONEID_HIPS)
        return 1;

    if (node1->anim_id == BONEID_BOOTL || node1->anim_id == BONEID_BOOTR)
        return -1;
    if (node2->anim_id == BONEID_BOOTL || node2->anim_id == BONEID_BOOTR)
        return 1;

    // don't know how to sort these bones
    return 0;
}

//Notice that dist goes down negative z, so lower is farther
static int cmpSortThingsDist(const void* aData, const void* bData)
{
    const SortThing* sta = (const SortThing*)aData;
    const SortThing* stb = (const SortThing*)bData;
    if (sta->modelSource == SORTTHING_GFXTREENODE && stb->modelSource == SORTTHING_GFXTREENODE && sta->gfxnode != stb->gfxnode && sta->gfxnode->seqHandle && sta->gfxnode->seqHandle == stb->gfxnode->seqHandle) {
        int ret;
        // sort by bone
        if ((ret = cmpGfxNodeBones(sta->gfxnode, stb->gfxnode)) != 0)
            return ret;
    }
     if (sta->dist == stb->dist) {
        if (sta->alpha == stb->alpha)
            return cmpSortThingsType(sta, stb);
        return (sta->alpha > stb->alpha) ? -1 : 1; // JE: Sort things with a higher alpha first, better chance of looking good
    }
    return (sta->dist > stb->dist) ? 1 : -1;
}

static int cmpSortThingsRevDist(const void* aData, const void* bData)
{
    const SortThing* sta = (const SortThing*)aData;
    const SortThing* stb = (const SortThing*)bData;
    if (sta->dist == stb->dist) {
        if (sta->alpha == stb->alpha)
            return cmpSortThingsType(sta, stb);
        return (sta->alpha > stb->alpha) ? -1 : 1; // JE: Sort things with a higher alpha first, better chance of looking good
    }
    return (sta->dist < stb->dist) ? 1 : -1;
}

void modelAddShadow( Mat4 mat, U8 alpha, Model *shadowModel, U8 shadowMask )
{
    ShadowNode * sn;

    // This should not be called for the shadow map render pass
    onlydevassert(gfx_state.renderPass < RENDERPASS_SHADOW_CASCADE1 || gfx_state.renderPass > RENDERPASS_SHADOW_CASCADE_LAST);

    if (game_state.shadowvol == 2)
        return;

    sn = dynArrayAdd(&fg_list->shadow_nodes,sizeof(ShadowNode),&fg_list->shadow_node_count,&fg_list->shadow_node_max,1);
    copyMat4(  mat, sn->mat );//            = mat;
    sn->alpha        = alpha; 
    sn->model        = shadowModel;
    sn->shadowMask    = shadowMask;
}


void modelDrawGfxNode( GfxNode * node, BlendModeType blend_mode, int tex_index, TintColor tint, Mat4 viewMatrix )
{

    assert( node && node->model );
    assert( node->model->loadstate == LOADED );

PERFINFO_AUTO_START("modelDrawGfxNode",1); 
    drawGfxNodeCalls++;
    if( !(game_state.shadowvol == 2 || scene_info.stippleFadeMode) )
    {
        if ( node->seqHandle ) 
            rdrSetStencil(1,STENCILOP_KEEP,128);
        else
            rdrSetStencil(0,0,0);
    }

    if( node->splat )  
    {
        assert(tex_index<1); // 0 or -1
        if( ((Splat*)node->splat)->drawMe == global_state.global_frame_count )
        {
            assert(node->viewspace);
            modelDrawShadowObject( node->viewspace, (Splat*)node->splat );
        }
    }
    else if (node->clothobj)
    {
        assert(tex_index==-1||tex_index==0);
        modelDrawClothObject(node, blend_mode);
    }
    else
    {
#ifndef FINAL
        // allow tinting render for dev indicators (e.g. alphs)
        if (tint != TINT_NONE)
        {
            U8    rgba_save[4];
            U8    rgba_save2[4];
            int flags_save;

            memcpy(rgba_save, node->rgba, 4);
            memcpy(rgba_save2, node->rgba2, 4);
            flags_save = node->flags;
            node->flags |= GFXNODE_CUSTOMCOLOR;

            memcpy(node->rgba, tintColors[tint], 4);
            memcpy(node->rgba2, tintColors[tint], 4);

            modelDrawBonedNode( node, blend_mode, tex_index, viewMatrix );

            memcpy(node->rgba, rgba_save, 4);
            memcpy(node->rgba2, rgba_save2, 4);
            node->flags = flags_save;
        }
        else
        {
            modelDrawBonedNode( node, blend_mode, tex_index, viewMatrix );
        }
#else
        modelDrawBonedNode( node, blend_mode, tex_index, viewMatrix );
#endif
        

    }
PERFINFO_AUTO_STOP(); 
}


#define Y_HEIGHT 99
#define X_HEIGHT 10
int xx;

typedef int (*cmp_func_type)(const void *, const void *);

static void sortList(SortThing sortThings[], int sortThingCount, cmp_func_type cmp_func)
{
    //assert(heapValidateAll());
    if( cmp_func && !GFX_DEBUG_TEST(game_state.perf, GFXDEBUG_PERF_SKIP_SORTING) )
        qsortG( sortThings, sortThingCount, sizeof(SortThing), cmp_func);
    //qsort( sortThings, sortThingCount, sizeof(SortThing), cmp_func);
}

static INLINEDBG int isFakedOpaque(Model *model)
{
    return model && ((model->flags & (OBJ_TREEDRAW | OBJ_FANCYWATER | OBJ_ALPHASORT)) || (model->trick && (model->trick->flags1 & (TRICK_ALPHACUTOUT|TRICK_ADDITIVE|TRICK_SUBTRACTIVE))));
}

static void drawList_ex(SortThing sortThings[], int sortThingCount, int headshot, ClippingInfo *clipping_info, int drawWater, bool drawingUnderwater)
{
    SortThing    *st;
    int    i;
    bool clippingInUse = false;
#ifndef FINAL
    static int iKillItem = -1, iPrevMeshIndex = -1;
#endif

    //assert(heapValidateAll());
    for( i = 0 ; i < sortThingCount; i++ ) 
    {
        bool isCostumePiece = false;
        bool needClipping = false;

        st = &sortThings[i];

#ifndef FINAL
        if( iKillItem == i )
            continue;
        if( game_state.meshIndex != -1 ) 
        {
            // draw only a single mesh from the list, useful for debugging rendering issues
            if(i != game_state.meshIndex )
                continue;
            if(game_state.meshIndex != iPrevMeshIndex) {
                // changed selected mesh, print geo name and file to console
                if (st->model)
                {
                    printf("Selected mesh: \"%s\" (%d verts, %d tris) from file \"%s\"\n", st->model->name,
                        st->model->vert_count, st->model->tri_count, st->model->filename);
                }
                else
                {
                    printf("Selected mesh: NULL\n");
                }
            }

        }
        iPrevMeshIndex = game_state.meshIndex;
#endif

        // Only draw water in the second alpha pass
        if (!drawWater && st->model && st->model->flags & OBJ_FANCYWATER)
            continue;

        // In the first alpha pass (for underwater translucency), only draw translucent costume pieces
        //isCostumePiece = (st->model && st->model->trick && (st->model->trick->flags2 & TRICK2_SETCOLOR) && (st->model->radius < 10.0f)) ||
        //                 ((st->modelSource == SORTTHING_GFXTREENODE) && st->gfxnode && st->gfxnode->clothobj);
        isCostumePiece = st->is_gfxnode || ((st->modelSource == SORTTHING_GFXTREENODE) && st->gfxnode && st->gfxnode->clothobj);

        if (drawingUnderwater && !isCostumePiece)
            continue;

        // Do culling against clipping planes
        if (clipping_info && clipping_info->num_clip_planes)
        {
            // The bounding box information is not correct for boned models.
            if (st->model && !st->model->boneinfo)
            {
                // TODO: pre-transform the clip planes to view space and do this in view space?
                int j;
                bool culled = false;
                Mat4 matModelToWorld;
                Vec3 mid, midw;
                
                mulMat4Inline(cam_info.inv_viewmat, st->viewMat, matModelToWorld);

#if 0
// DEBUG CODE
                if (game_state.test8 > 0 && st->model->boneinfo)
                {
                    unsigned int color = 0xFFFFFFFF;
                    Vec3 min_ws, max_ws;
                    Vec3 corners_ws[8];

                    if (game_state.test7)
                    {
                        copyVec3(st->model->min, min_ws);
                        copyVec3(st->model->max, max_ws);
                    }
                    else
                    {
                        mulVecMat4(st->model->min, matModelToWorld, min_ws);
                        mulVecMat4(st->model->max, matModelToWorld, max_ws);
                    }

                    corners_ws[0][0] = min_ws[0];
                    corners_ws[0][1] = min_ws[1];
                    corners_ws[0][2] = min_ws[2];

                    corners_ws[1][0] = max_ws[0];
                    corners_ws[1][1] = min_ws[1];
                    corners_ws[1][2] = min_ws[2];

                    corners_ws[2][0] = min_ws[0];
                    corners_ws[2][1] = max_ws[1];
                    corners_ws[2][2] = min_ws[2];

                    corners_ws[3][0] = max_ws[0];
                    corners_ws[3][1] = max_ws[1];
                    corners_ws[3][2] = min_ws[2];

                    corners_ws[4][0] = min_ws[0];
                    corners_ws[4][1] = min_ws[1];
                    corners_ws[4][2] = max_ws[2];

                    corners_ws[5][0] = max_ws[0];
                    corners_ws[5][1] = min_ws[1];
                    corners_ws[5][2] = max_ws[2];

                    corners_ws[6][0] = min_ws[0];
                    corners_ws[6][1] = max_ws[1];
                    corners_ws[6][2] = max_ws[2];

                    corners_ws[7][0] = max_ws[0];
                    corners_ws[7][1] = max_ws[1];
                    corners_ws[7][2] = max_ws[2];

                    drawLine3DWidth(corners_ws[0], corners_ws[1], color, 1.0f);
                    drawLine3DWidth(corners_ws[2], corners_ws[3], color, 1.0f);
                    drawLine3DWidth(corners_ws[4], corners_ws[5], color, 1.0f);
                    drawLine3DWidth(corners_ws[6], corners_ws[7], color, 1.0f);
                    drawLine3DWidth(corners_ws[0], corners_ws[2], color, 1.0f);
                    drawLine3DWidth(corners_ws[1], corners_ws[3], color, 1.0f);
                    drawLine3DWidth(corners_ws[4], corners_ws[6], color, 1.0f);
                    drawLine3DWidth(corners_ws[5], corners_ws[7], color, 1.0f);
                    drawLine3DWidth(corners_ws[0], corners_ws[4], color, 1.0f);
                    drawLine3DWidth(corners_ws[1], corners_ws[5], color, 1.0f);
                    drawLine3DWidth(corners_ws[2], corners_ws[6], color, 1.0f);
                    drawLine3DWidth(corners_ws[3], corners_ws[7], color, 1.0f);
                }
#endif

                mid[0] = (st->model->min[0] + st->model->max[0]) * 0.5f;
                mid[1] = (st->model->min[1] + st->model->max[1]) * 0.5f;
                mid[2] = (st->model->min[2] + st->model->max[2]) * 0.5f;

                mulVecMat4(mid, matModelToWorld, midw);

                for (j = 0; j < clipping_info->num_clip_planes; j++)
                {
                    float dist;

                    if ((clipping_info->flags[j] & ClipPlaneFlags_CostumesOnly) && !isCostumePiece)
                        continue;

                    dist = midw[0] * clipping_info->clip_planes[j][0] + midw[1] * clipping_info->clip_planes[j][1] + midw[2] * clipping_info->clip_planes[j][2] + clipping_info->clip_planes[j][3];

                    if ((dist + st->model->radius * 2.0f) < 0.0f)
                    {
                        culled = true;
                        break;
                    }

                    if ( gfx_state.current_viewport_info && gfx_state.current_viewport_info->renderPass == RENDERPASS_REFLECTION2 )
                    {
                        // planar building reflectors only draw items near the reflector plane
                        if ( dist > game_state.reflection_planar_fade_end)
                        {
                            culled = true;
                            break;
                        }
                    }

                    if (dist < (st->model->radius * 2.0f))
                        needClipping = true;

                }

                if (culled)
                    continue;
            }
            else if (st->modelSource == SORTTHING_GFXTREENODE)
            {
                needClipping = true;
            }
        }

        // Enable clip planes
        if (needClipping && !clippingInUse)
        {
            rdrSetClipPlanes(clipping_info->num_clip_planes, clipping_info->clip_planes);
            clippingInUse = true;
        }
        else if (!needClipping && clippingInUse)
        {
            rdrSetClipPlanes(0, NULL);
            clippingInUse = false;
        }

        //xyprintfcolor( X_HEIGHT * (xx/Y_HEIGHT), xx%Y_HEIGHT, 255, 255, 10," %d ", xx );          
        if ( st->modelSource == SORTTHING_GFXTREENODE )        
        {
            //onlydevassert( st->gfxnode );

            modelDrawGfxNode( st->gfxnode, st->blendMode, st->tex_index, st->tint, st->viewMat );
        }
        else if (st->modelSource == SORTTHING_WORLDMODEL)
        {
            ViewSortNode *vs = &bg_list->vsList[st->vsIdx];

#ifndef FINAL
            if (st->tint != TINT_NONE)
            {
                memcpy(vs->tint_colors, tintColors[st->tint], 4);
                vs->has_fx_tint = 1;
            }
#endif
            if(!vs->hide || !headshot)
            {
                modelDrawWorldModel( vs, st->blendMode, st->tex_index );
            }
        } else {
            onlydevassert(false);
        }
    }

    if (clippingInUse)
    {
        rdrSetClipPlanes(0, NULL);
    }

    rdrSetStencil(0,0,0);

}

void drawShadows()
{
    ShadowNode    * sn;
    int            i;

    //if( game_state.fxdebug == 1 ) 
    //    xyprintf( 20, 20, "Shadow Node Count: %d", shadow_node_count );

    CHECKNOTGLTHREAD;
    for(i = 0 ; i < bg_list->shadow_node_count ; i++ ) 
    {
        sn = &bg_list->shadow_nodes[i]; 
        if (sn->model)
            modelDrawShadowVolume(sn->model,sn->mat,sn->alpha,sn->shadowMask, 0 ); //node removed to make the drawing be the old way
    }
    bg_list->shadow_node_count = 0;
}

void gfxTreeFrameFinishQueueing()
{
    curr_list_idx = (curr_list_idx+1) & 1;
    fg_list = display_list+curr_list_idx;
    bg_list = display_list+(1-curr_list_idx);
    xx = 0;
}

void sortModels(int opaque, int alpha)
{
    PERFINFO_AUTO_START("sortModels",1); 
    if(opaque) {
        cmp_func_type cmpFunc = (gfx_state.renderPass >= RENDERPASS_SHADOW_CASCADE1 && gfx_state.renderPass <= RENDERPASS_SHADOW_CASCADE_LAST)
            ? cmpSortThingsModel : cmpSortThingsType; 
#ifndef FINAL
        if( game_state.perf & GFXDEBUG_PERF_SORT_BY_REVERSE_DIST )
            cmpFunc = cmpSortThingsRevDist;
        else if( game_state.perf & GFXDEBUG_PERF_SORT_RANDOMLY )
            cmpFunc = cmpRandom;

        if (cmpFunc)
#endif
        sortList(bg_list->typeSortThings, bg_list->typeSortThingsCount, cmpFunc);
    } if(alpha) {
        sortList(bg_list->distSortThings, bg_list->distSortThingsCount, (game_state.perf&GFXDEBUG_PERF_SORT_RANDOMLY)?cmpRandom:cmpSortThingsDist);
    }
    PERFINFO_AUTO_STOP();
}

void drawSortedModels_ex(DrawSortMode mode, int headshot, ViewportInfo *viewport, bool render_water)
{
    RTStats* pStats = NULL;

    ClippingInfo *clipping_info = (viewport) ? &viewport->clipping_info : NULL;
    const int pass = (viewport) ? viewport->renderPass : gfx_state.renderPass;

    int drawOpaque = (mode == DSM_OPAQUE);
    int drawAlphaUnderwater = (mode == DSM_ALPHA_UNDERWATER);
    int drawAlpha = (mode == DSM_ALPHA) && !(game_state.disable_two_pass_alpha_water_objects);
    int drawShadowVolumes = (mode == DSM_SHADOWS);

    modelDemandLoadSetFlags();

    // check water debug flags
    drawOpaque &= game_state.waterDebug ? (game_state.waterDebug & 1) : 1;
    drawAlphaUnderwater &= game_state.waterDebug ? ((game_state.waterDebug >> 1) & 1) : 1;
    drawAlpha &= game_state.waterDebug ? ((game_state.waterDebug >> 2) & 1) : 1;

    if (drawOpaque)
    {
        PERFINFO_AUTO_START("drawTypeSortedModels opaque",1); 
        rdrBeginMarker("drawTypeSortedModels");
        startTypeDrawing = call; //DEBUG
        drawStartTypeDrawing = drawCall; //DEBUG
        rdrQueue(DRAWCMD_RESETSTATS, &game_state.stats_cards, sizeof(int));
        DBG_LOG_BATCHES( bg_list->typeSortThings, bg_list->typeSortThingsCount, viewport, "opaque" );
        drawList_ex(bg_list->typeSortThings, bg_list->typeSortThingsCount, headshot, clipping_info, 0, false);  //1. Draw nodes sorted by type
        pStats = rdrStatsGetPtr(RTSTAT_TYPE, pass);
        rdrQueue(DRAWCMD_GETSTATS, &pStats, sizeof(RenderStats *));
        rdrEndMarker();
        PERFINFO_AUTO_STOP();
    }

    if (drawAlphaUnderwater)
    {
        PERFINFO_AUTO_START("drawDistSortedModelsUnderwater",1); 
        rdrBeginMarker("drawDistSortedModelsUnderwater");
        startAUWDrawing = call; //DEBUG
        drawStartAUWDrawing = drawCall; //DEBUG
        rdrQueue(DRAWCMD_RESETSTATS, &game_state.stats_cards, sizeof(int));
        DBG_LOG_BATCHES( bg_list->distSortThings, bg_list->distSortThingsCount, viewport, "underwater" );
        drawList_ex(bg_list->distSortThings, bg_list->distSortThingsCount, headshot, clipping_info, 0, true);  //2. Draw nodes sorted by distance
        pStats = rdrStatsGetPtr(RTSTAT_UWATER, pass);
        rdrQueue(DRAWCMD_GETSTATS, &pStats, sizeof(RenderStats *));
        rdrEndMarker();
        PERFINFO_AUTO_STOP();
    }

    if (drawAlpha)
    {
        PERFINFO_AUTO_START("drawDistSortedModels",1); 
        rdrBeginMarker("drawDistSortedModels alpha");
        startDistDrawing = call; //DEBUG
        drawStartDistDrawing = drawCall; //DEBUG
        rdrQueue(DRAWCMD_RESETSTATS, &game_state.stats_cards, sizeof(int));
        DBG_LOG_BATCHES( bg_list->distSortThings, bg_list->distSortThingsCount, viewport, "alpha" );
        drawList_ex(bg_list->distSortThings, bg_list->distSortThingsCount, headshot, clipping_info, render_water, false);  //2. Draw nodes sorted by distance
        pStats = rdrStatsGetPtr(RTSTAT_DIST, pass);
        rdrQueue(DRAWCMD_GETSTATS, &pStats, sizeof(RenderStats *));
        rdrEndMarker();
        PERFINFO_AUTO_STOP();
    }

    if (drawShadowVolumes)
    {
        PERFINFO_AUTO_START("drawShadowVolumes",1); 
        rdrBeginMarker("drawShadowVolumes");
        startShadowDrawing = call; //DEBUG
        drawStartShadowDrawing = drawCall; //DEBUG
        rdrQueue(DRAWCMD_RESETSTATS, &game_state.stats_cards, sizeof(int));
        drawShadows();
        pStats = rdrStatsGetPtr(RTSTAT_SHADOW, pass);
        rdrQueue(DRAWCMD_GETSTATS, &pStats, sizeof(RenderStats *));
        rdrEndMarker();
        PERFINFO_AUTO_STOP();
    }

    endDrawModels = call; //DEBUG
    drawEndDrawModels = drawCall;
}


extern int totalNodesProcessed;
extern int totalNodesRejected;
#define NODE_PROCESSED 0
#define NODE_REJECTED 1
#define NODE_REJECTED_CHILD 2



//Another debug only function
#include "player/player.h"
#include "entity/entclient.h"

void printNodesProcessed( int seqHandle, GfxNode * node, int type)
{
    Entity * e;
    SeqInst * mySeq = 0;


    if( game_state.animdebug & ANIM_SHOW_MY_ANIM_USE ) 
    {
        mySeq = playerPtr()->seq;
    }
    else if( game_state.animdebug & ANIM_SHOW_SELECTED_ANIM_USE )
    {
        e = current_target;
        if( e )
            mySeq = e->seq;
    }

    if( mySeq && seqHandle == mySeq->handle ) 
    {
        if( type == NODE_PROCESSED )
            xyprintfcolor( 40, 2+node->anim_id, 255, 255, 255, "NodeProcessed: %d %s", node->anim_id, bone_NameFromId(node->anim_id) );
        else if( type == NODE_REJECTED )
            xyprintfcolor( 40, 2+node->anim_id, 255, 0, 0,     "NodeRejected:  %d %s", node->anim_id, bone_NameFromId(node->anim_id) );
        else if( type == NODE_REJECTED_CHILD )
            xyprintfcolor( 40, 2+node->anim_id, 255, 100, 100, "NodeRejectedC: %d %s", node->anim_id, bone_NameFromId(node->anim_id) );

    }
}

//Debug function for counting rejected nodes
static int countNonHidChildrenGfx(GfxNode * pNode, int seqHandle)
{
    GfxNode * node;
    int total = 0; 
    for( node = pNode ; node ; node = node->next )
    {
        assert( node->seqHandle == seqHandle );
          if( !(node->flags & GFXNODE_HIDE) )
        {
            total++;
            printNodesProcessed( node->seqHandle, node, NODE_REJECTED_CHILD );
            if( node->child )
                total+=countNonHidChildrenGfx( node->child, seqHandle );
        }
    }
    return total;
}

void animDebugInfo( GfxNode * node )
{
    if( node->seqHandle && !node->useFlags )
    {
        totalNodesRejected += 1 + countNonHidChildrenGfx(node->child, node->seqHandle);
        if( game_state.animdebug )
            printNodesProcessed( node->seqHandle, node, NODE_REJECTED );
    }
    else if( node->seqHandle ) //leave out the sky
    {
        totalNodesProcessed++;
        if( game_state.animdebug )
            printNodesProcessed( node->seqHandle, node, NODE_PROCESSED );
    }
}


//Simplified version of gfxTreeDrawNode for Sky To keep it out of the hair of the character drawing
// @todo this rendering should also be part of the batch dump logs
void gfxTreeDrawNodeSky(GfxNode *node,Mat4 parent_mat)
{
    Mat4 mat;
    Model *model;
    int x = 10;

    rdrBeginMarker(__FUNCTION__);
    for(;node;node = node->next)   
    {
        mulMat4(parent_mat, node->mat, mat);
        model = node->model;
        if( model                                                && 
            node->alpha    >= MIN_GFXNODE_ALPHA_TO_BOTHER_WITH    && 
            (model->loadstate & LOADED)                        &&
            (!(node->flags & GFXNODE_HIDESINGLE))            )
        {
            int i;
            ViewSortNode vs={0};
            //Debug
            if( game_state.wireframe && !editMode() ) 
                xyprintf( 10, x++, "%s %d", model->name, node->alpha  ); 
            //End debug

            vs.alpha = node->alpha;
            vs.has_tricks = !!node->tricks;
            vs.tricks = node->tricks;
            copyMat4(mat, vs.mat);
            copyVec3(mat[3], vs.mid);
            vs.model = model;
            vs.rgbs = node->rgbs;
            vs.hide = node->flags & GFXNODE_HIDE;
            vs.materials = model->tex_binds;
            vs.uid = -1;
            vs.brightScale = 1.f;
            for (i=0; i<model->tex_count; i++) 
                modelDrawWorldModel(&vs, model->tex_binds[i]->bind_blend_mode, i);
        }

        if (node->child)
            gfxTreeDrawNodeSky(node->child, mat);
    
    }
    rdrEndMarker();
}


#define MAX_VIEWSPACEMATS 2048 //not brilliant, but fast
Mat4 viewspaceMat[ MAX_VIEWSPACEMATS ];
int viewspaceMatCount;

MP_DEFINE(EntLight);

//make a memory pool?
void clearMatsForThisFrame()
{
    viewspaceMatCount = 0;
    MP_CREATE(EntLight, 32);
    mpFreeAll(MP_NAME(EntLight));
}

Mat4Ptr getAMatForThisFrame()
{
    assertmsg(viewspaceMatCount < MAX_VIEWSPACEMATS, "viewspaceMatCount >= MAX_VIEWSPACEMATS too many gfx nodes, or corruption?");

    return viewspaceMat[ viewspaceMatCount++ ];
}

EntLight * getALightForThisFrame(void)
{
    return MP_ALLOC(EntLight);
}

FORCEINLINE void drawNodeOldStyle(GfxNode *node)
{
    avsn_params.node = node;
    avsn_params.model = node->model;
    avsn_params.mid = node->viewspace[3];
    avsn_params.alpha = node->alpha;
    avsn_params.rgbs = node->rgbs;
    avsn_params.mat = node->viewspace;
    addViewSortNodeSkinned(); //this adds to both dist and type sort bins
}

void drawNodeAsWorldModel(GfxNode *node)
{
    EntLight * light = NULL;
    if( node->seqGfxData && node->seqGfxData->light.use != ENTLIGHT_DONT_USE ) {
        EntLight *orig_light = &node->seqGfxData->light;
        if ((orig_light->use & ( ENTLIGHT_CUSTOM_AMBIENT | ENTLIGHT_CUSTOM_DIFFUSE ))==orig_light->use) {
            // We just want custom ambient/diffuse, copy the starting ambient/diffuse values first
            light = getALightForThisFrame();
            *light = *orig_light;
            modelDrawGetCharacterLighting(node, light->ambient, light->diffuse, light->direction);
            light->use |= ENTLIGHT_USE_NODIR;
        }
        else if (node->seqGfxData->light.use & ENTLIGHT_INDOOR_LIGHT)
        {
            light = getALightForThisFrame();
            modelDrawGetCharacterLighting(node, light->ambient, light->diffuse, light->direction);
            light->use = ENTLIGHT_INDOOR_LIGHT;
        }
    } else {
        if (node->model->flags & OBJ_BUMPMAP) { // Ugly hack?  NPCs need this bit of lighting on their non-skinned bits, world FX really don't want this kind of lighting
            light = getALightForThisFrame();
            modelDrawGetCharacterLighting(node, light->ambient, light->diffuse, light->direction);
            light->use = ENTLIGHT_USE_NODIR; // Just sending down the absolute values to use
        } else {
            light = NULL;
        }
    }

    avsn_params.model = node->model;
    avsn_params.mat = node->viewspace; // TODO: This is copied locally, we don't need a global anymore
    avsn_params.mid = node->viewspace[3];
    avsn_params.alpha = node->alpha;
    avsn_params.tint_colors = node->rgba; // hacky, this points to rgba[4] and rgba2[4] which are sequential in the struct
    avsn_params.has_tint = !!(node->flags & GFXNODE_CUSTOMCOLOR);
    avsn_params.has_fx_tint = !!(node->flags & GFXNODE_TINTCOLOR && (node->rgba[0]!=255||node->rgba[1]!=255||node->rgba[2]!=255)); // see fpe 2/22/11 comment in fxgeo.c
    avsn_params.light = light;
    avsn_params.brightScale = 1.f;
    avsn_params.tricks = node->tricks;
    avsn_params.materials = node->mini_tracker?node->mini_tracker->tracker_binds[0]:node->model->tex_binds;

    // @todo
    // We want model object attachments to entities to currently shadow/not shadow along with their parent entity
    // For the time being this means to flag the 'in tray' accordingly
    avsn_params.from_tray = !rdrShadowMapCanEntityRenderWithShadows();

    addViewSortNodeWorldModel();
}

void gfxTreeDrawNodeInternal(GfxNode *node)
{
    avsn_params.is_gfxnode = true;

    // Split into skinned and non-skinned
    if((node->model->flags & OBJ_DRAWBONED) && (node->flags & GFXNODE_SEQUENCER) || node->clothobj || node->splat) {
        // skinned, draw old way
        drawNodeOldStyle(node);
    } else {
        // Non-skinned, draw as a WorldModel.
        drawNodeAsWorldModel(node);
    }
}

// An upper bound on the scaling done by the matrix
F32 getScaleBounds(Mat3 mat) {
    Vec3 xy;
    Vec3 xyz;
    F32 scale;
    F32 newScale;

    addVec3(mat[0], mat[1], xy);
    addVec3(xy, mat[2], xyz);
    scale = lengthVec3Squared(xyz);
    subVec3(xy, mat[2], xyz);
    newScale = lengthVec3Squared(xyz);
    scale = max(scale, newScale);
    subVec3(mat[0], mat[1], xy);
    addVec3(xy, mat[2], xyz);
    newScale = lengthVec3Squared(xyz);
    scale = max(scale, newScale);
    subVec3(xy, mat[2], xyz);
    newScale = lengthVec3Squared(xyz);
    return sqrt(max(scale, newScale));
}

void gfxTreeDrawNode(GfxNode *node,Mat4 parent_mat)
{
    SeqGfxData * seqGfxData;
    Mat4 viewspace;

    for(;node;node = node->next)
    {
#ifndef FINAL
        if( game_state.animdebug )
            animDebugInfo( node );
#endif

        if( ( node->flags & GFXNODE_HIDE ) || ( node->seqHandle && !node->useFlags ) ) 
            continue; 

#ifndef FINAL
        if (game_state.reflectionDebug == 100)
        {
            if (!node->model || strstr(node->model->name, game_state.reflectiondebugname) == NULL)
                continue;
        }

        if( game_state.wireframe && !editMode() && node->model && strstri( node->model->name, "sunglow" ) )
            xyprintf( 10, 40, "Drawing %s %d", node->model->name, node->alpha );

#if 0
        if( node->model && strstri(node->model->filename, "AssaultRifle") )
        {
            xyprintf( 10, 55, "Drawing %s (node 0x%08x, parentx4 0x%08x {%.2f,%.2f,%.2f}",
                node->model->filename, node, node->parent->parent->parent->parent, node->parent->parent->parent->parent->mat[3][0],
                node->parent->parent->parent->parent->mat[3][1], node->parent->parent->parent->parent->mat[3][2]);        
        }
#endif
#endif

        //gfxGetAnimMatrices
        if( bone_IdIsValid(node->anim_id) && node->seqHandle ) 
        {    
            Mat4Ptr viewspace_scaled; //will be the 
            Vec3 xlate_to_hips;
            Mat4 scaled_xform, scale_mat;
            
            seqGfxData = node->seqGfxData; 
            assert( seqGfxData ); 
            assert( bone_IdIsValid(node->anim_id) );

            viewspace_scaled = seqGfxData->bpt[node->anim_id];
    
            copyMat4(unitmat, scale_mat); 
            scaleMat3Vec3(scale_mat, node->bonescale);
            mulMat4(node->mat, scale_mat, scaled_xform);

            mulMat4(parent_mat, scaled_xform, viewspace_scaled );
            scaleVec3(seqGfxData->btt[node->anim_id], -1, xlate_to_hips);   //Prexlate this back to home
            mulVecMat4( xlate_to_hips, viewspace_scaled, viewspace_scaled[3] );

            //Added in here to avoid extra traversal by gfxtreesetalpha
            node->alpha = seqGfxData->alpha;
        }

        //If you are just a bone for skinning, you are done.
        if( !node->child && !node->model )
            continue;

        if( node->model && (node->clothobj || node->splat) )
            node->viewspace = getAMatForThisFrame(); //Need a global cuz the rendering code will use this
        else
            node->viewspace = viewspace;  //Just use the local cuz it's not used after this stack unwinds 

        if(node->model && node->flags & GFXNODE_APPLYBONESCALE) {
            Mat4 scale_mat, scaled_xform;
            // tbd, ideally would just set gfx_node->bonscale directly and use that here, but that won't work for char creator where parent scale can change each frame
            assert(node->parent && node->parent->parent); 
            assert(!vec3IsZero(node->parent->parent->bonescale));
            assert( !(node->model->flags & OBJ_DRAWBONED || node->flags & GFXNODE_SEQUENCER || node->clothobj || node->splat) );
            copyMat4(unitmat, scale_mat); 
            scaleMat3Vec3Correct(scale_mat, node->parent->parent->bonescale);
            mulMat4(node->mat, scale_mat, scaled_xform);
            mulMat4( parent_mat, scaled_xform, node->viewspace );
        }
        else
        {
            mulMat4( parent_mat, node->mat, node->viewspace );
        }

        //Add model to a draw hopper (must add to some hopper, cuz all bone xforms aren't done yet)
        if( node->model && !(node->flags & GFXNODE_HIDESINGLE) && (node->alpha >= MIN_GFXNODE_ALPHA_TO_BOTHER_WITH ) && (node->model->loadstate & LOADED) )
        {
            sortedDraw++; //debug

            if (node->clothobj) {
                if (!node->clothobj->GameData->system) {
                    avsn_params.node = node;
                    avsn_params.mid = node->viewspace[3];
                    avsn_params.alpha = node->alpha;
                    avsn_params.rgbs = node->rgbs;
                    avsn_params.mat = node->viewspace;
                    addViewSortNodeSkinned(); //this adds to both dist and type sort bins
                } else {
                    // Don't draw them here, draw them in particle systems! (but we still calculated the viewspace above, I think)
                }
            } else {
                F32 scale = getScaleBounds(node->viewspace);

                if (!(node->flags & GFXNODE_NEEDSVISCHECK) || gfxSphereVisible(node->viewspace[3], scale * node->model->radius))
                {
                    // draw characters
                    if (node->seqHandle)
                        gfxTreeDrawNodeInternal(node);
                    else if (!gfx_state.current_viewport_info)
                        gfxTreeDrawNodeInternal(node);
                    else
                    {
                        // Filter out debris which is 'small' on the screen
                        F32 wPixels, hPixels, maxPixels, scale;
                        const F32 debris_fadein_size = gfx_state.current_viewport_info->debris_fadein_size;
                        const F32 debris_fadeout_size = gfx_state.current_viewport_info->debris_fadeout_size;

                        //scale = node->model->radius / -node->viewspace[3][2];
                        scale = node->model->radius / ABS(node->viewspace[3][2]);

                        wPixels = gfx_state.current_viewport_info->width * scale;
                        hPixels = gfx_state.current_viewport_info->height * scale;

                        maxPixels = MAX(wPixels, hPixels);
                        if (maxPixels > debris_fadeout_size)
                        {
                            gfxTreeDrawNodeInternal(node);
                        }
                        else if (maxPixels > debris_fadein_size && debris_fadeout_size > debris_fadein_size)
                        {
                            F32 alphaLerp = (maxPixels - debris_fadein_size) / (debris_fadeout_size - debris_fadein_size);
                            U8 oldAlpha = node->alpha;

                            node->alpha *= alphaLerp;

                            if (node->alpha >= MIN_GFXNODE_ALPHA_TO_BOTHER_WITH)
                                gfxTreeDrawNodeInternal(node);

                            node->alpha = oldAlpha;
                        }
                    }
                }
            }
        }

        if (node->child)
            gfxTreeDrawNode(node->child, node->viewspace);

        if (node->viewspace == viewspace)
            node->viewspace = NULL;
    }
}

void setLightForCharacterEditor()
{
    g_sun.ambient_for_players[0] = 0.15;
    g_sun.ambient_for_players[1] = 0.15;
    g_sun.ambient_for_players[2] = 0.15;
    g_sun.ambient_for_players[3] = 1.0;
    copyVec4(g_sun.ambient_for_players, g_sun.ambient);

    g_sun.diffuse_for_players[0] = 0.19;
    g_sun.diffuse_for_players[1] = 0.19;
    g_sun.diffuse_for_players[2] = 0.19;
    g_sun.diffuse_for_players[3] = 0.19;
    copyVec4(g_sun.diffuse_for_players, g_sun.diffuse);

    g_sun.direction[0] = 1.0;
    g_sun.direction[1] = 1.0;
    g_sun.direction[2] = 1.0;

    g_sun.position[0] = 10000.0;
    g_sun.position[1] = 10000.0;
    g_sun.position[2] = 10000.0;

    g_sun.luminance = 1.0;
    g_sun.bloomWeight = 1.0;
    g_sun.toneMapWeight = 0.4;
    g_sun.desaturateWeight = 0.0;
    g_sun.gloss_scale = 1.0;
    g_sun.dof_values.farValue = g_sun.dof_values.nearValue = 0.0;

    g_sun.fogcolor[0] = 0.0;
    g_sun.fogcolor[1] = 0.0;
    g_sun.fogcolor[2] = 0.0;

    /*
    if (game_state.test1 && (global_state.global_frame_count & 1))
    {
        // Make some problem areas flicker
        g_sun.fogcolor[0] = 1.0;
        g_sun.fogcolor[1] = 1.0;
        g_sun.fogcolor[2] = 1.0;
    }
    */
}

///////////////////////////////////////////////////////////////////////////////
// SHADOWMAP RENDERING

/*
@todo
This is going to be replaced by custom code and data structures

Currently for shadowmaps we hack this up so that completely opaque models get put in the
sort by type list and geo that uses alpha test (alpha cutout trick or TreeDraw legacy assets)
get put in the 'dist' list.

The opaque list we can draw all the sub-object batches in a single draw call without setting
up any texture state.

I'm also going to assume for the moment that we can draw the entire model with alpha test as
a single batch until I can check the assets and put in the additional logic but still keep things
down to a minimum number of draw calls.

Ideally we sort the alpha test batches by model batch and the texture if that ever participates in
texture swaps?

*/

// @todo
// A multi material that we expect to render with cutout alpha into the shadowmap
// could potentially get its resultant alpha from the combination of many different textures.
// That isn't supported at this time and perhaps it shouldn't ever be when it probably makes
// more sense to have a simple fallback specified in the material which has the cutout alpha
// in a single texture which is easily determined.
// For the moment if a multi material is presented for rendering into the shadow map with
// alpha then we will just assume that one texture can be used for the shadow as selected below.

static BasicTexture* choose_multi_alpha_cutout_tex(TexBind *bind )
{
    BasicTexture *b;

    b = bind->tex_layers[TEXLAYER_DUALCOLOR1];
    if (b && b->flags & TEX_ALPHA)
        return b;
    b = bind->tex_layers[TEXLAYER_MULTIPLY1];
    if (b && b->flags & TEX_ALPHA)
        return b;
    if (bind->tex_layers[TEXLAYER_MASK] == white_tex)
        return NULL;
    b = bind->tex_layers[TEXLAYER_DUALCOLOR2];
    if (b && b->flags & TEX_ALPHA)
        return b;
    b = bind->tex_layers[TEXLAYER_MULTIPLY2];
    if (b && b->flags & TEX_ALPHA)
        return b;
    if (bind->bind_blend_mode.shader==BLENDMODE_MODULATE || bind->bind_blend_mode.shader==BLENDMODE_MULTIPLY) {
        b = bind->tex_layers[TEXLAYER_BASE1];
        if (b && b->flags & TEX_ALPHA)
            return b;
    }
    return NULL;
}

void addViewSortNodeWorldModel_Shadowmap(void)
{
    // Anything that causes a forced non-split does *not* require alpha sorting
    // Otherwise look at the flag on the TexBind

    ViewSortNode    *vs = 0;
    int                vsIndex = -1;
    Model            *model = avsn_params.model;
    bool            forceOpaque = true;
    int                uses_alpha_test;
    int                i_sub, tri_count = 0, tri_start = 0;
    SortThing        st, *stp;

    if (!model )    //@todo unnecessary?
        return;

    if (model->flags & OBJ_DONTCASTSHADOWMAP)
        return;

/*
    Some things that normally render into the distance sorted alpha list we also want to render into
    the shadowmaps. Examples include:
        - ALPHAREF trick objects (like big power towers)
        - Composite geo objects that include base opaque sub-objects followed by alpha subobjects.
        These are generally library pieces like the big Fort Hades in Port Oakes or road pieces with
        the details like lines, manhole covers, etc. overlaid as decals.

    Some model batches material textures with alpha but are intended to render in the 'opaque' pass
    with the use of alpha test as opposed to alpha blending (e.g. OBJ_TREEDRAW, TRICK_ALPHACUTOUT, TRICK2_ALPHAREF)

    In the case of models with both opaque and alpha sub-objects we want to render the
    opaque batches as one chunk into the shadow map and then render the batches that require
    alpha test individually so that we can use the appropriate texture.

    To do this we take advantage of the fact that the opaque subobjects are always
    grouped together before the subobjects with alpha so we count the number of tris in
    the subobjects until we hit the first alpha batch and render those as one batch
    into the shadowmap.

    note: Alpha test is explicitly indicated by either TRICK_ALPHACUTOUT or TRICK2_ALPHAREF. 
    TRICK2_ALPHAREF explicitly sets the alpha ref used while TRICK_ALPHACUTOUT uses a preset 0.6 alpharef.
    OBJ_TREEDRAW also implies alpha cutout with the 0.6 alpha ref (used by legacy trees)
*/

    uses_alpha_test = model->trick && ((model->trick->flags1 & TRICK_ALPHACUTOUT) || (model->trick->flags2 & TRICK2_ALPHAREF));

    // skip truly alpha objects/layers
    if ((model->flags & OBJ_ALPHASORTALL) || (model->flags & OBJ_FANCYWATER))
        return;

    // only shadow models that have a valid VBO
    // @todo if we are going to be using the fallback material then we should be doing
    // this setup with that in mind as it might fixup for extra sts (see comments below)
    // with regard to that.)
    modelSetupVertexObject(model, VBOS_USE, avsn_params.materials[0]->bind_blend_mode);
    if (!model->vbo)
        return;

    // If you are a world node, construct a view sort node for yourself...
    // this is so multiple batches can refer back to common model information
    // @todo this is not as useful and generally a waste of time and space for
    // shadowmap rendering since we render most objects as a single aggregate batch
    // without any material information.
    vs = dynArrayAdd(&fg_list->vsList,sizeof(ViewSortNode),&fg_list->vsListCount,&fg_list->vsListMax,1);
    vsIndex = fg_list->vsListCount - 1;

    // setup vs...we don't care about most of it for shadowmap rendering
    vs->model    = avsn_params.model;
    vs->alpha    = avsn_params.alpha;
    vs->rgbs    = avsn_params.rgbs;
    vs->light    = avsn_params.light;
    vs->is_gfxnode = avsn_params.is_gfxnode;
    if (avsn_params.mid)
        copyVec3(avsn_params.mid,vs->mid);
    vs->has_tint = 0;
    vs->has_fx_tint = 0;
    copyMat4(avsn_params.mat,vs->mat);
    vs->has_tricks = 0;
    vs->stipple_alpha = avsn_params.stipple_alpha;
    vs->uid = avsn_params.uid;
    vs->materials = avsn_params.materials;
    vs->use_fallback_material = avsn_params.use_fallback_material;
    vs->hide = avsn_params.hide;
    vs->draw_white = true;

    onlydevassert( model && model->loadstate & LOADED );

    // Setup SortThing 'template' that we will copy into our lists when we add a batch
    // @todo create a customized sort structure for shadowmaps since we don't need some
    // of the fields and have to awkwardly override some to hold specialized values which
    // is not very maintainable.
    st.model        = model;
    st.alpha        = avsn_params.alpha;
    copyMat4(avsn_params.mat, st.viewMat);
    st.modelSource    = SORTTHING_WORLDMODEL;
    st.vsIdx        = vsIndex;
    st.draw_white    = true;
    st.blendMode.intval = 0;
    
    // TreeDraw and alpha cutout need to be handled specially since we need to setup texture stage
    // with the correct alpha channel
    st.is_tree = (model->flags & OBJ_TREEDRAW) != 0;
    st.alpha_test = uses_alpha_test;

    // Our strategy is to walk through the models subobjects and add the opaque batches
    // as one cumulative batch to one list and then each alpha test
    // batch individually to the other.

    // as we start walking through the sub objects the opaque ones will be first
    // spin through them and keep a cumulative triangle count to issue as a batch
    for (i_sub=0; i_sub<model->tex_count; i_sub++)
    {
        if (avsn_params.materials[i_sub]->needs_alphasort)
            break;    // stop once we hit first alpha subobject
        tri_count += model->tex_idx[i_sub].count;
    }

    // add the aggregate opaque batch (if any) into the sort list
    if (tri_count)
    {
        stp = dynArrayAdd( &fg_list->typeSortThings, sizeof(SortThing), &fg_list->typeSortThingsCount, &fg_list->typeSortThingsMax, 1);
        *stp = st;
        stp->tri_start = tri_start;
        stp->tri_count = tri_count;

        onlydevassert(stp->tri_count <= stp->model->tri_count);

        tri_start = tri_count;        // next batch begins at this triangle
    }

    // continue scanning through the sub-objects and add the alpha test batches
    // individually. If we aren't doing alpha tests then we just skip all the
    // batches with alpha
    if ( st.alpha_test || st.is_tree )
    {
        // continue the count from where we left off with the opaque batches
        for (; i_sub<model->tex_count; i_sub++)
        {
            if (avsn_params.materials[i_sub]->needs_alphasort)
            {
                // need to setup texture stage with cutout alpha texture for batches material

                // figure out the texture that is being used for alphatest and demand load if necessary
                // @todo need to expand this and could probably combine with the texNeedsAlphaSort logic
                TexBind* bind = vs->materials[i_sub];
                if (vs->use_fallback_material)    // @todo is this relevant to shadows?
                    bind = texFindLOD(bind);

                switch (bind->bind_blend_mode.shader)
                {
                case BLENDMODE_MODULATE:
                case BLENDMODE_BUMPMAP_MULTIPLY:
                    st.tex_bind =    bind->tex_layers[TEXLAYER_BASE] ? (TexBind*)texDemandLoad( bind->tex_layers[TEXLAYER_BASE] ) : 0; // overriding normal meaning here
                    st.tint = TEXLAYER_BASE;    // overriding to hold the texture coordinate index
                    break;
                case BLENDMODE_ALPHADETAIL:
                case BLENDMODE_COLORBLEND_DUAL:
                case BLENDMODE_BUMPMAP_COLORBLEND_DUAL:
                    st.tex_bind =    bind->tex_layers[TEXLAYER_GENERIC] ? (TexBind*)texDemandLoad( bind->tex_layers[TEXLAYER_GENERIC] ) : 0; // overriding normal meaning here
                    // @todo
                    // If we are choosing this path because we have a fallback from a MULTI blend then it is likely the model
                    // vbo doesn't have an sts2 as we didn't go through the code path to do that fixup. So, without a more in depth
                    // review or reordering the code we'll have to rely on the single tex coord stream in the VBO. But if the fallback
                    // requested scalings on the different sts streams that may cause visual errors.
                    // The separate sts texture coord streams is largely a register combiner holdover so may be worthwhile to investigate
                    // doing away with that instead of adding additional hacks. (e.g. add tex coord transform to all shaders)
                    st.tint = model->vbo->sts2 ? TEXLAYER_GENERIC : TEXLAYER_BASE;    // overriding to hold the texture coordinate index
                    break;
                case BLENDMODE_MULTI:
                    {
                        // what do we use for the cutout alpha on a multi material? Lots of possibilities...
                        BasicTexture* cutout_tex = choose_multi_alpha_cutout_tex(bind);

                        // multi materials only have on set of tex coords in vbo
                        st.tint = TEXLAYER_BASE;    // overriding to hold the texture coordinate index
                        st.tex_bind = cutout_tex ? (TexBind*)texDemandLoad( cutout_tex ) : 0; // overriding normal meaning here
                    }
                    break;
                default:
                    st.tex_bind = 0;
                    break;
                }

                // add it to the 'dist sort' list just as a place to keep these separate for later rendering
                // we don't really dist sort it
                stp = dynArrayAdd( &fg_list->distSortThings, sizeof(SortThing), &fg_list->distSortThingsCount, &fg_list->distSortThingsMax, 1);
                *stp = st;
                tri_count = model->tex_idx[i_sub].count;    // number of triangles in this sub object
                stp->tri_start = tri_start;
                stp->tri_count = tri_count;

                onlydevassert(stp->tri_count <= stp->model->tri_count);

                tri_start += tri_count;                        // bump to the next starting triangle index
            }
        }
    }

}

void addViewSortNodeSkinned_Shadowmap(void)
{
    // Anything that causes a forced non-split does *not* require alpha sorting
    // Otherwise look at the flag on the TexBind

    ViewSortNode    *vs = 0;
    int                sortType;
    Model            *model = avsn_params.model;
    bool            forceOpaque = true;
    int                all_alpha;

    // don't render any alpha models into the shadowmap
    //@todo probably too exclusive, probably need to look at each batch
//    needs_alpha = modelAlphaSortEx(model, avsn_params.materials);
    all_alpha = (((model->flags & OBJ_ALPHASORTALL) && !(model->flags & OBJ_FANCYWATER)));
    if (all_alpha)
        return;

    //onlydevassert( (fg_list->vsListCount + fg_list->nodeTotal) == (fg_list->typeSortThingsCount + fg_list->distSortThingsCount ) );

    //onlydevassert( !avsn_params.node || !avsn_params.custom_texbinds); // node -> !custom_texbinds

    //onlydevassert( avsn_params.node ); // World geometry no longer goes through here

    //onlydevassert(!_isnan(avsn_params.mid[2]));

    modelSetupVertexObject(model, VBOS_USE, avsn_params.materials[0]->bind_blend_mode);
    if (!model || !model->vbo)
        return;

    if (model->flags & OBJ_DONTCASTSHADOWMAP)
        return;

    sortType = SORT_BY_TYPE;    

    assert( model && model->loadstate & LOADED );

    // Toss yourself into the right bin 
    {
        F32 dist = avsn_params.mid[2];
        SortThing st={0}, *stp;

        // Setup SortThing

        // TreeDraw and alpha cutout need to be handled specially since we need to setup texture stage
        // with the correct alpha channel
        st.alpha_test = model->trick && (model->trick->flags1 & TRICK_ALPHACUTOUT);
        st.is_tree = (model->flags & OBJ_TREEDRAW) != 0;

        st.dist = dist;
        st.model = model;
        st.alpha = avsn_params.alpha;
        copyMat4(avsn_params.mat, st.viewMat);

        // variation with world model
        fg_list->nodeTotal++; //debug
        st.modelSource    = SORTTHING_GFXTREENODE;
        st.gfxnode        = avsn_params.node;

        // @todo assume that we can render all model bathes (sub-objects) in one draw call
        // this won't work for batches that need an alpha test texture, etc.

        // @todo add alpha test support

        {
            // opaque drawing, don't need to setup any texture state
            // @todo need to walk the batch list to see if there are any alpha batches we should discard
            // (which can potentially make it more difficult to draw all the batches in few draw calls)
            st.draw_white = true;
            st.blendMode.intval = 0;

            stp = dynArrayAdd( &fg_list->typeSortThings, sizeof(SortThing), &fg_list->typeSortThingsCount, &fg_list->typeSortThingsMax, 1);
        }

        *stp = st;
        stp->tex_index    = -1;
        stp->was_type_sorted = true;
        stp->alpha_test = 0;
        stp->is_tree = 0;
    }

    //onlydevassert( (fg_list->vsListCount + fg_list->nodeTotal) == (fg_list->typeSortThingsCount + fg_list->distSortThingsCount) );
}

void gfxTreeDrawNodeInternal_shadowmap(GfxNode *node)
{
    avsn_params.is_gfxnode = true;

    // Split into skinned and non-skinned
    if((node->model->flags & OBJ_DRAWBONED) && (node->flags & GFXNODE_SEQUENCER) || node->clothobj || node->splat) {
        // skinned
        avsn_params.node = node;
        avsn_params.model = node->model;
        avsn_params.mid = node->viewspace[3];
        avsn_params.alpha = node->alpha;
        avsn_params.rgbs = NULL;
        avsn_params.mat = node->viewspace;
        avsn_params.materials = node->mini_tracker?node->mini_tracker->tracker_binds[0]:node->model->tex_binds;
        addViewSortNodeSkinned_Shadowmap(); //this adds to both dist and type sort bins
    } else {
        // Non-skinned, draw as a WorldModel.
        avsn_params.node = NULL;
        avsn_params.model = node->model;
        avsn_params.mat = node->viewspace; // TODO: This is copied locally, we don't need a global anymore
        avsn_params.mid = node->viewspace[3];
        avsn_params.alpha = node->alpha;
        avsn_params.tint_colors = NULL;
        avsn_params.materials = node->mini_tracker?node->mini_tracker->tracker_binds[0]:node->model->tex_binds;
        addViewSortNodeWorldModel_Shadowmap();
    }
}

static void rdr_shadowmap_populate_bone_xforms( void* dst, GfxNode *node, Mat4 shadowViewMat, Mat4 invertMainViewToWorld )
{
    F32*        offset;
    BoneId*    idx_table;
    Mat4*        bone_table;
    BoneInfo*        boneinfo;
    SeqGfxData*    seqGfxData = node->seqGfxData;
    Vec4*    pDst4    = (Vec4*)dst;
    int            j;

    //onlydevassert(seqGfxData);

    boneinfo            = node->model->boneinfo;
    offset                = seqGfxData->btt[node->anim_id];
    idx_table            = boneinfo->bone_ID;
    bone_table        = seqGfxData->bpt;                    

    for(j = 0 ; j < boneinfo->numbones ; j++)     
    {
        Mat4*    pSrcBoneXfm = (Mat4*)(&bone_table[idx_table[j]]);
        Mat4 bonemat;
        Vec3    translation;

        // need to change the matrices from main viewport camera
        // space to the cascade light view space
        // M_xform_to_shadow_space
        if (true)
        {
            Mat4 M_model_to_world;
            mulMat4Inline( invertMainViewToWorld, *pSrcBoneXfm, M_model_to_world );
            mulMat4Inline( shadowViewMat, M_model_to_world, bonemat );
        }
        else
        {
            copyMat4(*pSrcBoneXfm, bonemat);
        }

        // complete the bone transform translation
        mulVecMat4(offset,bonemat,translation);
        copyVec3(translation, bonemat[3]);

        // transpose the complete xform into a 3x4 matrix so that we
        // only need to set 3 float4 shader constants per matrix

        (*pDst4)[0] = bonemat[0][0];
        (*pDst4)[1] = bonemat[1][0];
        (*pDst4)[2] = bonemat[2][0];
        (*pDst4)[3] = translation[0];
        pDst4++;

        (*pDst4)[0] = bonemat[0][1];
        (*pDst4)[1] = bonemat[1][1];
        (*pDst4)[2] = bonemat[2][1];
        (*pDst4)[3] = translation[1];
        pDst4++;

        (*pDst4)[0] = bonemat[0][2];
        (*pDst4)[1] = bonemat[1][2];
        (*pDst4)[2] = bonemat[2][2];
        (*pDst4)[3] = translation[2];
        pDst4++;
    }
}

#include "render/thread/rt_shadowmap.h"    // for RdrShadowmapModel,etc types

static bool isValidAlphaTestWorldModel(SortThing* st) {
    return st->modelSource == SORTTHING_WORLDMODEL && 
        (st->tint ? st->model->vbo->sts2 : st->model->vbo->sts);
}

void drawSortedModels_shadowmap_new( ViewportInfo* viewport )
{
    SortThing    *batches_opaque, *batches_alphatest;
    int    i;
    int count_opaque, count_alphatest, count_skinned = 0;
    void* rdrData;
    RdrShadowmapItemsHeader* pHeader;
    int dataSize;
    Mat4 invertMainViewToWorld;
    Mat4 shadowViewMat;
    int num_static = 0, num_skinned = 0, num_alphatest = 0;
    int bone_memory = 0;

    PERFINFO_AUTO_START("drawSortedModels_shadowmap_new",1); 

    count_opaque = bg_list->typeSortThingsCount;
    batches_opaque = bg_list->typeSortThings;
    DBG_LOG_BATCHES( batches_opaque, count_opaque, viewport, "new_opaque" );

    count_alphatest = bg_list->distSortThingsCount;
    batches_alphatest = bg_list->distSortThings;
    DBG_LOG_BATCHES( batches_alphatest, count_alphatest, viewport, "new_alphatest" );

    //**
    // Run through the opaque list and get the counts for static
    // versus skinned models
    // @todo when we have our own sorting lists we will do this there in advance
    {
        for( i = 0 ; i < count_opaque; i++ ) 
        {
            SortThing    *st = &batches_opaque[i];

            if ( st->modelSource == SORTTHING_GFXTREENODE )        
            {
                assert( st->gfxnode );
                if (st->gfxnode->model->boneinfo)
                {
                    num_skinned++;
                    bone_memory += st->gfxnode->model->boneinfo->numbones * sizeof(Vec4)*3; // we send bones as 3x4 transposed
                }
            }
            else if (st->modelSource == SORTTHING_WORLDMODEL)
            {
                num_static++;
            }
        }

        for( i = 0 ; i < count_alphatest; i++ ) 
        {
            SortThing    *st = &batches_alphatest[i];

            if (isValidAlphaTestWorldModel(st))
            {
                num_alphatest++;
            }
#ifndef FINAL
            else if (st->modelSource == SORTTHING_WORLDMODEL)
            {
                ErrorFilenamef(st->model->filename, "Missing alpha UVs in model %s\n", 
                               st->model->name);
            }
#endif
        }
    }

    //**
    // Allocate the data block to hold data to hand off to rendering thread

    // make an entry for each vbo and xform matrix
    dataSize  = sizeof(RdrShadowmapItemsHeader) + num_static*sizeof(RdrShadowmapModelOpaque);
    dataSize += num_alphatest*sizeof(RdrShadowmapModelAlphaTest);
    dataSize += num_skinned*sizeof(RdrShadowmapModelSkinned) + bone_memory;

    rdrData = rdrQueueAlloc(DRAWCMD_SHADOWMAP_RENDER,dataSize);
    if (!rdrData)
    {
        // @todo error handling? reporting?
        return;
    }

    // We need to xform from the main viewport camera xforms
    // into the view from the light for this shadowmap
    // (recall that we let the group visibility/lod walk use
    // the view from the main camera)
    copyMat4(group_draw.inv_cam_mat, invertMainViewToWorld);
    gfxSetViewMat(viewport->cameraMatrix, shadowViewMat, NULL);

    //****
    // Setup header
    pHeader = (RdrShadowmapItemsHeader*)rdrData;
    pHeader->batch_count_opaque        = num_static;
    pHeader->batch_count_alphatest    = num_alphatest;
    pHeader->batch_count_skinned    = num_skinned;

    //**
    // First we process the opaque static world models
    {
        RdrShadowmapModelOpaque* pItem = (RdrShadowmapModelOpaque*)(pHeader+1);

        for( i = 0 ; i < count_opaque; i++ ) 
        {
            SortThing    *st = &batches_opaque[i];
            VBO* pNewVBO = NULL;
            U32        sameCount = 0;
            RdrShadowmapModelOpaque* pInstancesFirstItem = NULL;

            if (st->modelSource == SORTTHING_WORLDMODEL)
            {
                ViewSortNode *vs = &bg_list->vsList[st->vsIdx];
                VBO* pThisVBO = vs->model->vbo;
                
                // recall that we generally just draw the entire model as one
                // batch an ignore sub-objects for shadows. However, if the model
                // had some alpha sub objects at the end we only draw the tris in
                // the initial opaque sub objects.

                // @todo use a custom shadowmap sorting structure that is more efficient
                // and uses appropriately named fields to make code more maintainable
                pItem->tri_count = st->tri_count;

                pItem->instanceCount = 0;
                pItem->vbo = pThisVBO;
                pItem->lod_alpha = st->alpha;        // lod alpha fade, stipple it in to make transition less abrupt

                onlydevassert( pItem->tri_count <= vs->model->tri_count );

                // need to change the instance matrices from main viewport camera
                // space to the cascade light view space
                {
                    Mat4 M_model_to_world;
                    mulMat4Inline( invertMainViewToWorld, vs->mat, M_model_to_world );
                    mulMat4Inline( shadowViewMat, M_model_to_world, pItem->xform );
                }

                // do instance counting and back save the results
                if (pThisVBO != pNewVBO)
                {
                    if (pInstancesFirstItem)
                    {
                        // save the count of the instance group we are finishing
                        pInstancesFirstItem->instanceCount = sameCount;
                    }
                    pInstancesFirstItem = pItem;
                    sameCount = 1;
                    pNewVBO = pThisVBO;
                }
                else
                {
                    sameCount++;
                }

                // skip to next item to populate
                pItem++;
            }
        }
    }


    //**
    // Next we process the static models that use alpha test (e.g. tree canopies)
    {
        RdrShadowmapModelAlphaTest* pItem = (RdrShadowmapModelAlphaTest*)((RdrShadowmapModelOpaque*)(pHeader+1) + pHeader->batch_count_opaque);

        for( i = 0 ; i < count_alphatest; i++ ) 
        {
            SortThing    *st = &batches_alphatest[i];

            if (isValidAlphaTestWorldModel(st))
            {
                ViewSortNode *vs = &bg_list->vsList[st->vsIdx];
                VBO* pThisVBO = vs->model->vbo;

                pItem->tex_id = (int)st->tex_bind;    // alpha test ogl texture id
                pItem->tex_coord_idx = st->tint;    // texture coord index to use in VBO
                pItem->lod_alpha = st->alpha;        // lod alpha fade, stipple it in to make transition less abrupt
                pItem->vbo = pThisVBO;
                pItem->tri_count = st->tri_count;    // starting offset and count of batch triangles
                pItem->tri_start = st->tri_start;

                // need to change the instance matrices from main viewport camera
                // space to the cascade light view space
                {
                    Mat4 M_model_to_world;
                    mulMat4Inline( invertMainViewToWorld, vs->mat, M_model_to_world );
                    mulMat4Inline( shadowViewMat, M_model_to_world, pItem->xform );
                }
                // skip to next item to populate
                pItem++;
            }
        }
    }

    //**
    // Finally process the skinned items
    {
        RdrShadowmapModelAlphaTest* pItemAT = (RdrShadowmapModelAlphaTest*)((RdrShadowmapModelOpaque*)(pHeader+1) + pHeader->batch_count_opaque);
        RdrShadowmapModelSkinned*        pItem = (RdrShadowmapModelSkinned*)(pItemAT + pHeader->batch_count_alphatest);

        for( i = 0 ; i < count_opaque; i++ ) 
        {
            SortThing    *st = &batches_opaque[i];

            if ( st->modelSource == SORTTHING_GFXTREENODE )        
            {
                assert( st->gfxnode );
                if (st->gfxnode->model->boneinfo)
                {
                    unsigned int mtx_block_size = st->gfxnode->model->boneinfo->numbones * sizeof(Vec4)*3; // we send bones as 3x4 transposed
                    void* pDstXforms = pItem + 1;    // bone matrices follow immediately after the item header

                    pItem->vbo = st->gfxnode->model->vbo;
                    pItem->num_bones = st->gfxnode->model->boneinfo->numbones;
                    // populate the bone matrices (transposed)
                    rdr_shadowmap_populate_bone_xforms(pDstXforms, st->gfxnode, shadowViewMat, invertMainViewToWorld );

                    // skip to next item to populate
                    pItem = (RdrShadowmapModelSkinned*)( (char*)pItem + sizeof(RdrShadowmapModelSkinned) + mtx_block_size );
                }
            }
        }
    }

    //****
    // Send the entire workload to the render thread
    rdrQueueSend();
    PERFINFO_AUTO_STOP();
}
