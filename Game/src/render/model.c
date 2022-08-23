#include <stdio.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/memcheck.h>
#include "assert.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>

#include "cmdparse/cmdgame.h"
#include "graphics/gfxwindow.h"
#include "graphics/gfxSettings.h"
#include "UI/uiConsole.h"
#include "graphics/font.h"
#include "render/renderUtil.h"
#include "render/model.h"
#include "render/tex.h"
#include "seq/tricks.h"
#include "seq/anim.h"
#include "graphics/light.h"
#include "render/thread/rt_state.h"
#include "render/bump.h"
#include "render/renderbonedmodel.h"

/* MW thoughts on model.c: One slightly awkward aspect is that the trick setting done here doesn't know anything about
custom textures (either from the grouping or from the character customization).  Custom stuff has 
to be handled closer to draw time, but it's a bit haphazard, with each customizer handling its own special cases, often 
making assumptions about the state coming in.  For example if you put a trick an modelects default texture, then in the world 
editor change its texture, the trick may still affect the new texture, causing craziness.  */
//1/18/03 MW 


//2.  ##################### Do this in the thread ##########################

/* IN THREAD Return OBJ_ALPHASORT flag if the modelect this texture is on will need alpha sorting. */
U32 texNeedsAlphaSort(TexBind *bind, BlendModeType blend)
{
    BasicTexture *b = bind->tex_layers[TEXLAYER_BASE];

    if (!bind)
        return 0;

    if (bind->is_fallback_material && (bind->texopt && bind->texopt->flags & TEXOPT_FALLBACKFORCEOPAQUE))
        return 0;

    if (blend.shader == BLENDMODE_MULTI)
    {
        // Multi 8 gets alpha from lots of places, fun!
        b = bind->tex_layers[TEXLAYER_DUALCOLOR1];
        if (b && b->flags & TEX_ALPHA)
            return OBJ_ALPHASORT;
        b = bind->tex_layers[TEXLAYER_MULTIPLY1];
        if (b && b->flags & TEX_ALPHA)
            return OBJ_ALPHASORT;
        if (bind->tex_layers[TEXLAYER_MASK] == white_tex)
            return 0;
        b = bind->tex_layers[TEXLAYER_DUALCOLOR2];
        if (b && b->flags & TEX_ALPHA)
            return OBJ_ALPHASORT;
        b = bind->tex_layers[TEXLAYER_MULTIPLY2];
        if (b && b->flags & TEX_ALPHA)
            return OBJ_ALPHASORT;
        if (bind->bind_blend_mode.shader==BLENDMODE_MODULATE || bind->bind_blend_mode.shader==BLENDMODE_MULTIPLY) {
            b = bind->tex_layers[TEXLAYER_BASE1];
            if (b && b->flags & TEX_ALPHA)
                return OBJ_ALPHASORT;
        }
        return 0;
    }

    // addglow shader sets alpha from the constant color, not a texture
    if (blend.shader == BLENDMODE_ADDGLOW)
        return 0;

    if (blend.shader == BLENDMODE_COLORBLEND_DUAL || blend.shader == BLENDMODE_BUMPMAP_COLORBLEND_DUAL || blend.shader == BLENDMODE_ALPHADETAIL)
        b = bind->tex_layers[TEXLAYER_GENERIC];
    if (b && b->flags & TEX_ALPHA)
        return OBJ_ALPHASORT;

    if (blend.shader == BLENDMODE_MODULATE || blend.shader == BLENDMODE_MULTIPLY || blend.shader == BLENDMODE_BUMPMAP_MULTIPLY)
    {
        // We use alpha from both textures for multiply.
        // Already checked the first, so check the second texture.
        b = bind->tex_layers[TEXLAYER_GENERIC];
        if (b && b->flags & TEX_ALPHA)
            return OBJ_ALPHASORT;
    }

    return 0;
}

U32 modelNeedsAlphaSort(Model *model)
{
    int j;
    int ret=0;
    if (model->flags & OBJ_TREEDRAW || model->trick && (model->trick->flags1&TRICK_ALPHACUTOUT))
        return ((model->flags&(OBJ_FORCEOPAQUE|OBJ_ALLOW_MULTISPLIT))==OBJ_FORCEOPAQUE)?0:(OBJ_FORCEOPAQUE|OBJ_ALLOW_MULTISPLIT);
    for(j=0;j<model->tex_count;j++)
    {    
        TexBind *bind = model->tex_binds[j];

        ret |= texNeedsAlphaSort(bind, model->blend_modes[j]); //if any texture needs alpha sort, sort this modelect 
    }
    return ret;
}

TrickNode *allocModelTrick(Model *model)
{
    if (!model->trick) {
        model->trick = calloc(sizeof(TrickNode),1);
        setVec3(model->trick->trick_rgba, 255, 255, 255);
        setVec3(model->trick->trick_rgba2, 255, 255, 255);
    }
    return model->trick;
}

void modelResetFlags(Model *model)
{
    int        j,colorblend_alpha_hack=0;
    TexBind *bind;
    int visibleTextures;
    VBO        *vbo = model->vbo;
    BlendModeType min_blend_mode;
    BlendModeType merged_blend_mode;

    model->flags = 0; // Reset the flags (should already be zero unless this is a reload)

    // Trick flags need to be copied before texture setup

    // This assert is not true if the model was loaded before the trick existed.
    // assert((model->trick?model->trick->info:NULL) == trickFromObjectName(model->name, NULL));

    // happens first in animLoadStubs once, and used here to re-apply on trick reload
    modelInitTrickNodeFromName(model->name,model);

    if (model->trick && model->trick->info) //if the modelect's tricks need to set any model->flags to work right, do that
        model->flags |= model->trick->info->model_flags;

    // Set model to most common blend mode.  In world drawing, the blend mode is not changed; on gfxnodes, the
    // replaceable textures may set a different blend mode
    min_blend_mode = BlendMode(BLENDMODE_MULTIPLY, 0);
    merged_blend_mode = BlendMode(BLENDMODE_MULTIPLY, 0);

    if( strnicmp(model->name, "GEO_",4) == 0 && !(model->flags & (OBJ_WORLDFX | OBJ_STATICFX)) )  //TO DO, figure out how to make this a trick flag or something
    {
        colorblend_alpha_hack = 1;
        min_blend_mode = BlendMode(BLENDMODE_COLORBLEND_DUAL, 0);
        if (strstri(model->name,"eyes"))
            allocModelTrick(model)->flags1 |= TRICK_DOUBLESIDED;
        //if (strstri(model->name,"GEO_EMBLEM") || strstri(model->name, "GEO_NECK") || strstri(model->name, "GEO_FACE") || strstri(model->name, "GEO_EYES") )
        //|| strstri(model->name, "GEO_HAIR"))
        //    model->flags |= OBJ_ALPHASORT | OBJ_TREEDRAW;
        //Purpose of the OBJ_TREEDRAW flag is for some hair having an alpha cut off at low distances
    }
    if (model->boneinfo)
        model->flags |= OBJ_DRAWBONED;

    // Texture setup
    visibleTextures = 0;

    // Set model flags based on textures
    // Find any texture blend modes that spread to all sub objects (ADDGLOW)

    for(j=0;j<model->tex_count;j++)
    {
        bind        = model->tex_binds[j];

        if (bind->bind_blend_mode.shader == BLENDMODE_ADDGLOW)
            min_blend_mode = bind->bind_blend_mode;

        if (blendModeHasBump(bind->bind_blend_mode)) {
            model->flags |= OBJ_BUMPMAP;
        }
        if (blendModeHasDualColor(bind->bind_blend_mode))
            allocModelTrick(model)->flags2 |= TRICK2_SETCOLOR; 
        if (bind->bind_blend_mode.shader == BLENDMODE_ADDGLOW)
        {
            allocModelTrick(model)->flags2 |= TRICK2_HASADDGLOW;
            allocModelTrick(model)->flags2 |= TRICK2_SETCOLOR;
        }
        if (bind->bind_blend_mode.shader == BLENDMODE_WATER)
            allocModelTrick(model)->flags2 |= TRICK2_SETCOLOR; 

#if 0 // old cubemap code
        if ((bind->flags & TEX_CUBEMAPFACE) || (bind_bump && bind_bump->flags & TEX_CUBEMAPFACE))
            model->flags |= OBJ_CUBEMAP;
#endif

        //Dumb trick to not draw things that are frikin' invisible anyway. 
        if( 0 != strnicmp( "clearalpha", bind->name, 10 ) && 0 != stricmp( "invisible", bind->name ) && 0 != stricmp( "churn_col_alpha_01", bind->name ) ) //Josh's idea of an invisible texture
            visibleTextures++;

        // Texture trick trickle down here!
        if (bind->texopt) {
            if (bind->texopt->flags & TEXOPT_NORANDOMADDGLOW)
                allocModelTrick(model)->flags2 |= TRICK2_NORANDOMADDGLOW; 
            if (bind->texopt->flags & TEXOPT_ALWAYSADDGLOW)
                allocModelTrick(model)->flags2 |= TRICK2_ALWAYSADDGLOW; 
            model->flags |= bind->texopt->model_flags;
            if (bind->texopt->scrollsScales.multiply1Reflect && (bind->bind_blend_mode.shader==BLENDMODE_MULTIPLY || bind->bind_blend_mode.shader==BLENDMODE_MODULATE))
                allocModelTrick(model)->flags1 |= TRICK_REFLECT_TEX1;
            if (bind->texopt->flags & TEXOPT_FALLBACKFORCEOPAQUE)
                allocModelTrick(model)->flags2 |= TRICK2_FALLBACKFORCEOPAQUE; 
        }
        merged_blend_mode = promoteBlendMode(merged_blend_mode, bind->bind_blend_mode);
    }

    if (model->flags & OBJ_FANCYWATER && (game_state.waterMode > WATER_OFF))
    {
        min_blend_mode = BlendMode(BLENDMODE_WATER, 0);
    } else {
        // Do not clear this flag.  If water is changed, we need this flag to be set properly.
        // Otherwise, we need to reload the model.  The code should rely on game_state.waterMode
        // to determine how to render the water models.
        //model->flags &=~OBJ_FANCYWATER;
    }

    if (blendModeHasDualColor(min_blend_mode))
        allocModelTrick(model)->flags2 |= TRICK2_SETCOLOR; 

    // Set individual blend modes on model
    model->common_blend_mode = BlendMode(-1, 0);
    for(j=0;j<model->tex_count;j++)
    {
        bind        = model->tex_binds[j];
        if (min_blend_mode.shader<=BLENDMODE_MULTIPLY && bind->bind_blend_mode.shader > 0) {
            model->blend_modes[j] = bind->bind_blend_mode;
        } else if (min_blend_mode.shader == BLENDMODE_COLORBLEND_DUAL) {
            // Ups to at least something with dualcolor
            if (blendModeHasDualColor(bind->bind_blend_mode))
                model->blend_modes[j] = bind->bind_blend_mode;
            else
                model->blend_modes[j] = min_blend_mode;
        } else if (min_blend_mode.shader == BLENDMODE_ADDGLOW) {
            // Only ups those who are MULTIPLY
            if (bind->bind_blend_mode.shader <= BLENDMODE_MULTIPLY)
                model->blend_modes[j] = min_blend_mode;
            else 
                model->blend_modes[j] = bind->bind_blend_mode;
        } else {
            // Others override everything (WATER, SUNFLARE, etc)
            model->blend_modes[j] = min_blend_mode;
        }

        if (model->blend_modes[j].shader == BLENDMODE_COLORBLEND_DUAL && blendModeHasBump(bind->bind_blend_mode))
            model->blend_modes[j] = BlendMode(BLENDMODE_BUMPMAP_COLORBLEND_DUAL, 0);
        if (model->common_blend_mode.shader == -1) {
            model->common_blend_mode = model->blend_modes[j];
        } else if (model->common_blend_mode.shader != model->blend_modes[j].shader) {
            model->common_blend_mode = BlendMode(0,0);
        }
    }

    if (merged_blend_mode.shader && (model->flags & (OBJ_FORCEOPAQUE|OBJ_ALLOW_MULTISPLIT))==OBJ_FORCEOPAQUE)
    {
        // Not actually the blend mode of the first sub, but it's the one that should be
        // used for drawing the entire model
        model->blend_modes[0] = merged_blend_mode;
    }

    // After setting model->blend_mode
    if (!colorblend_alpha_hack)
        model->flags |= modelNeedsAlphaSort(model);
    else {
        // Player geometry should get alpha sort flags based on whether or not the specific
        // custom textures need alpha sorting, but currently never gets alphasorted without
        // a specific trick...
    }


    //Dumb hack that really should be handled in the tricks, but artists don't do it, so I'm putting this in
    if( !visibleTextures )
        model->flags |= OBJ_HIDE;


    if (!modelHasNorms(model))
        model->flags |= OBJ_FULLBRIGHT;

    if (model->trick && model->trick->flags1 & (TRICK_ADDITIVE|TRICK_SUBTRACTIVE))
        model->flags |= OBJ_ALPHASORT|OBJ_ALPHASORTALL;

    if (model->trick && model->trick->info && (model->trick->info->model_flags & OBJ_ALPHASORT)) // ForceAlphaSort
        model->flags |= OBJ_ALPHASORTALL;

    if (model->flags & OBJ_FORCEOPAQUE)
        model->flags &= ~OBJ_ALPHASORT;

    if(model->flags & ( OBJ_WORLDFX | OBJ_STATICFX ) )
        model->flags &= ~OBJ_DRAWBONED;

}

void modelResetVBOs(bool freeModels)
{
    if (freeModels)
        modelFreeAllCache(0);
    rdrQueueCmd(DRAWCMD_RESETVBOS);
}


//IN THREAD 
static void modelCreateObjectFromModel(Model *model, TexBind * binds[], int numbinds)
{
    int        j;
    VBO        *vbo = model->vbo;

    //Texture set up
    SAFE_FREE(model->tex_binds); // Allocate them both in one memory chunk
    model->tex_binds = calloc(model->tex_count*sizeof(model->tex_binds[0]) +
                    model->tex_count*sizeof(model->blend_modes[0]), 1);
    model->blend_modes = (void*)(model->tex_binds + model->tex_count); // Points right past the tex_binds

    for(j=0;j<model->tex_count;j++)
    {
        int bindid = model->tex_idx[j].id;
        if (bindid && !(bindid >= 0 && bindid < numbinds)) {
            assert(!"Bad .geo file (has invalid texture index)");
            bindid = 0;
        }
        model->tex_binds[j] = binds[bindid];
        assert(model->tex_binds[j]);
    }

    modelResetFlags(model);

}

//IN THREAD build an array of ptrs to TexBinds, one for each texture used anywhere in this ModelHeader
//(model->tex_ idx[i].id == the index into this array)
static TexBind ** buildTexBindArrayFromTexNames(PackNames * texnames, TexLoadHow tex_load_style, TexUsage tex_usage )
{
    TexBind ** binds;
    int i;
    char * s;
    binds = calloc(MAX(1,texnames->count),sizeof(binds[0]));

    for(i=0;i<texnames->count;i++)
    {
        s = strrchr(texnames->strings[i],'.'); //remove .tga if it's there
        if (s)
            *s = 0;

        if (strstri(texnames->strings[i],"PORTAL"))
            binds[i] = invisible_tex_bind;
        else {
            binds[i] = texLoad(texnames->strings[i], tex_load_style, tex_usage);
        }

        assert(binds[i]);
    }

    if (!texnames->count) //(if no textures use white for everything)
        binds[0] = white_tex_bind;
    return binds;
}

void validateAllTextures( GeoLoadData * gld, TexBind **binds, PackNames * texnames )
{
    int i;
    for(i=0;i<texnames->count;i++)
    {
        if( strstriConst( binds[i]->name, "white") && !strstriConst( texnames->strings[i], "white" ) )
        {
            Errorf( "Data Problem: %s wants the texture %s, which doesn't exist.",
                gld->name, texnames->strings[i]); 
        }
    }
}

/* MAIN THREAD For each Model in the Animlist, fill out its textures */
void addModelData(GeoLoadData * gld)
{
    int            i, j;
    TexBind **binds;

    binds = buildTexBindArrayFromTexNames(&gld->texnames, TEX_LOAD_DONT_ACTUALLY_LOAD, (gld->type&GEO_USED_BY_GFXTREE)?TEX_FOR_ENTITY:TEX_FOR_NOTSURE);

    //Debug only
    //TO DO Sometime comment this in and endure the whining and moaning till they are all fixed
    //validateAllTextures( gld, binds, &gld->texnames );

    for( i = 0 ; i < gld->modelheader.model_count ; i++ )
    {
        Model *model = gld->modelheader.models[i];
        modelCreateObjectFromModel(model, binds, gld->texnames.count);
        for (j = 0; j < eaSize(&model->lod_models); j++)
        {
            modelCreateObjectFromModel(model->lod_models[j]->model, binds, gld->texnames.count);
        }
    }
    
    free(binds);
} 

void addModelDataSingle(Model *model)
{
    GeoLoadData *gld = model->gld;
    TexBind **binds;

    binds = buildTexBindArrayFromTexNames(&gld->texnames, TEX_LOAD_DONT_ACTUALLY_LOAD, TEX_FOR_WORLD);
    modelCreateObjectFromModel(model, binds, gld->texnames.count);
    free(binds);
}


