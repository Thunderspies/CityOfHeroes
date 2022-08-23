#if 0
#pragma inline_recursion(on)
#pragma inline_depth(16)
#endif

#include "graphics/groupdraw.h"
#include "graphics/groupdrawutil.h"
#include "graphics/groupdrawinline.h"
#include "render/rendermodel.h"
#include "render/rendertree.h"
#include "render/rendershadow.h"
#include "cmdparse/cmdgame.h"
#include "graphics/gfxwindow.h"
#include "graphics/light.h"
#include "group/grouptrack.h"
#include "graphics/camera.h"
#include <utilitieslib/utils/utils.h>
#include "graphics/FX/fx.h"
#include "seq/tricks.h"
#include "group/groupdyn.h"
#include "group/groupProperties.h"
#include "edit/edit_cmd.h"
#include "render/tex.h"
#include <utilitieslib/components/StashTable.h>
#include "seq/tricks.h"
#include "seq/anim.h"
#include "graphics/vistray.h"
#include "group/groupfileload.h"
#include "render/renderstats.h"
#include "graphics/zOcclusion.h"
#include "graphics/gfx.h"
#include "graphics/gfxSettings.h"
#include "bases/basedraw.h"
#include "render/renderprim.h"
#include "edit/edit_drawlines.h"
#include "render/renderUtil.h"
#include "graphics/FX/fxgeo.h"
#include "graphics/groupMiniTrackers.h"
#include "graphics/sun.h"
#include "seq/AutoLOD.h"
#include "render/model_cache.h"
#include "graphics/viewport.h"
#include "graphics/view_cache.h"
#include "render/cubemap.h"

#if 0
#define CALL_INC group_draw.calls[group_draw.calls_idx++]++;
#define CALL_INIT    group_draw.calls_idx=0;
#else
#define CALL_INIT
#define CALL_INC
#endif

#define GROUPDRAW_DEBUG_SHOW_OBB 0

#define PULSE_PERIOD 20
#define PULSE_BRIGHTNESS 1.8f

//burning buildings stuff
int                buildingIsBurning;
DynGroupStatus    burningBuildingStatus;
char *            burningFX;
DefTracker *    burningLastBuilding;


static int modelCalcAlpha(GroupDef *def,F32 dist,F32 scale,DrawParams *draw,AutoLOD *lod,int *beyond_fog, VisType vis, U8 *stipple_alpha)
{
    F32 alpha=1.f, alpha2=1.f;
    F32 lod_near,lod_nearfade,lod_far,lod_farfade;
    F32 fog_far_dist_plus_radius;
    Model *model = def->model;
    U8 alpha3 = 255;

    scale *= game_state.lod_scale;

    if (lod)
    {
        lod_near        = lod->lod_near        * scale; 
        lod_nearfade    = lod->lod_nearfade    * scale; 
        lod_far            = lod->lod_far        * scale;
        lod_farfade        = lod->lod_farfade    * scale;
    }
    else
    {
        lod_near        = def->lod_near        * scale; 
        lod_nearfade    = def->lod_nearfade * scale; 
        lod_far            = def->lod_far        * scale;
        lod_farfade        = def->lod_farfade    * scale;
    }

#ifndef FINAL
    // ability to shrink the lod fade distances (to reduce alpha mesh count)
    if(game_state.lod_fade_range != 1.0f )
    {
        lod_near -= lod_nearfade * (1-game_state.lod_fade_range);
        lod_far += lod_farfade * (1-game_state.lod_fade_range);
        lod_nearfade *= game_state.lod_fade_range;
        lod_farfade *= game_state.lod_fade_range;
    }
#endif

    if (draw->lod_fadenode && (def->parent_fade || draw->lod_fadenode==2)) {
        fog_far_dist_plus_radius = group_draw.fog_far_dist + draw->node_radius;
    } else {
        fog_far_dist_plus_radius = group_draw.fog_far_dist + def->radius;
    }

    if (model && model->trick && (model->trick->flags1 & TRICK_NOFOG))
        *beyond_fog = 0;
    else
        *beyond_fog = vis != VIS_DRAWALL && dist > fog_far_dist_plus_radius;

    if (*beyond_fog && !draw->no_fog_clip && group_draw.fog_clip)
    {
        alpha2 = 1 - ((dist - fog_far_dist_plus_radius) / 100.f);
        if (alpha2 > 1)
            alpha2 = 1;
        if (alpha2 < 0)
            alpha2 = 0;
    }

    if (dist < lod_near)
    {
        dist = lod_near - dist;
        if (dist >= lod_nearfade)
            return 0;
        alpha = 1.f - dist / lod_nearfade;
    }
    else if (dist > lod_far)
    {
        if (def->vis_window)
            return 255;
        dist -= lod_far;
        if (dist >= lod_farfade)
            return 0;
        alpha = 1.f - dist / lod_farfade;
    }
    else if (def->vis_window)
    {
        alpha = dist / lod_far;
        if (alpha < 0.31f)
            alpha = 0.31f;
    }

    if (model && model->trick && (model->trick->flags1 & TRICK_NIGHTLIGHT) && !(model->trick->flags2 & TRICK2_HASADDGLOW)) {
        // NightLight, without AddGlow, alpha the object!  (here rather than in gfxNodeTricks)
        alpha3 = g_sun.lamp_alpha_byte;
    }

    if (draw->alpha) {
        if (draw->tricks && (draw->tricks->flags2 & (TRICK2_STIPPLE1|TRICK2_STIPPLE2)))
        {
            *stipple_alpha = round(draw->alpha*255);
            if (*stipple_alpha==255)
                *stipple_alpha = 254; // 255 means to use draw->alpha
            if (draw->alpha < 0.0001)
                return 0; // Don't draw it!
        } else {
            alpha *= draw->alpha;
            *stipple_alpha = 255;
        }
    } else {
        *stipple_alpha = 255;
    }
    
    return alpha * alpha2 * alpha3;
}

static F32 g_glowBrightScale = 1.0;

static INLINEDBG int drawModel(Model *model, Model *lowLodModel, AutoLOD *lod, GroupDef *def,Mat4 mat,Vec3 mid,DefTracker *tracker,DrawParams *draw,F32 d,VisType vis, int uid, int do_occluder_check, int lod_idx, int lod_override, bool is_welded, int hide)
{
    U8                *rgbs = 0;
    int                alpha;
    U8                stipple_alpha = 0;
    int                beyond_fog = 0;
    int                checked_occluder = 0;

    if(!model || !(model->loadstate & LOADED))
        return checked_occluder;

    if (model->trick && vis != VIS_DRAWALL)
    {
        if ((model->trick->flags1 & TRICK_FANCYWATEROFFONLY) && (game_state.waterMode > WATER_OFF))
            return checked_occluder;
    }

    if (!group_draw.see_everything 
        && !game_state.edit_base 
        && model->trick 
        && (model->trick->flags1 & TRICK_BASEEDITVISIBLE))
    {
        return checked_occluder;
    }

    // Is this call the result of a request that we are drawing this model only with a specific override LOD?
    // (as opposed to rendering all the LODs and letting them draw at the correct opacity to handle transitions between LODs)
    // If so we want to special case handle calculating the opacity.
    if (lod_override)
    {
        // There are two cases.

        // 1. The "Edit LOD" window in the editor is asking us to show a specific LOD of a model
        // for development purposes. In that case we just always draw the lod at full opacity.
#ifndef FINAL
        if (tracker && tracker->lod_override)
        {
            // just always draw the override LOD completely opaque, it won't fade in or out
            // so the developer can see this specific lod regardless
            alpha = 255;
        }
        else
#endif
        // 2. A viewport wide lod override is in place.
        // Generally this is done in order to improve performance, e.g., drawing only with low LODs in reflection viewports.
        // At the moment it only makes sense for this to be set to use the lowest LOD available or we might end up drawing with
        // a higher lod than would have been selected by camera location etc.
        // @todo in the future it may be more flexible to implement a 'lod_model_bias" which will determine the lod that do render 
        // and instead of using the model for that LOD use the model for the biased LOD (e.g. draw with 1 lower LOD).

        // In this case we don't want to calculate the near distance fade-in alpha for this specific override LOD as we might actually be inside
        // the distance for when this lower LOD would naturally be rendered (i.e., the camera is in the range of a higher
        // detail LOD of this model). Instead we should just draw the requested override LOD completely opaque.
        // However, we should still check the far alpha fade of the lowest LOD so that the object can fade out as it would
        // as if the camera was actually far enough to fade out the final LOD and make the object disappear from view.
        {
            // @todo I believe the def ->lod_far value embodies the extrema for the visibility distances
            // so we should be able to check the desired calculation by providing a NULL lod and using the def's values
            // could check lowLodModel explicitly if it is non-NULL, but need to bypass the near test.
            
            // note that we pass NULL for the lod param so that it uses the def lod params for testing
            alpha = modelCalcAlpha(def,fsqrt(d),draw->view_scale,draw,NULL,&beyond_fog,vis,&stipple_alpha);
        }
    }
    else
    {
        // not an lod override situation so calculate the alpha of this model lod normally
        alpha = modelCalcAlpha(def,fsqrt(d),draw->view_scale,draw,lod,&beyond_fog,vis,&stipple_alpha);
    }

    if( group_draw.globFxMiniTracker && !tracker && group_draw.globFxMiniTracker->alpha < 255 ) 
        alpha *= (F32)group_draw.globFxMiniTracker->alpha / (F32)255.0;

    if (game_state.random_base || game_state.edit_base)
    {
        def = baseHackery(def,mat,mid,&alpha,draw->light_trkr,&tracker);
        if (!def)
            return checked_occluder;
        model = def->model;
    }

    if (!alpha)
        return checked_occluder;

    CALL_INC

    if (!(draw->tricks && draw->tricks->flags1 & TRICK_HIDE))
    {
        if (tracker && draw->light_trkr)
        {
            if (tracker->def && tracker->def->pulse_glow && tracker->src_rgbs)
            {
                int i;

                for (i = 0; i < eaSize(&tracker->src_rgbs); i++)
                {
                    modelFreeRgbBuffer(tracker->src_rgbs[i]);
                    tracker->src_rgbs[i] = NULL;
                }

                eaDestroy(&tracker->src_rgbs);
                tracker->src_rgbs = NULL;
            }

            if (!tracker->src_rgbs && !(model->flags & OBJ_FULLBRIGHT))
            {
                colorTracker(tracker,mat,draw->light_trkr,
                    game_state.edit_base || (game_state.edit && game_state.fullRelight) || game_state.forceFullRelight,
                    g_glowBrightScale);
            }

            if (tracker->src_rgbs && eaSize(&(tracker->src_rgbs)) >= lod_idx)
                rgbs = tracker->src_rgbs[lod_idx];
        }

        //If this is a shadow, add it and forget it
        if ( model->trick && (model->trick->flags2 & TRICK2_CASTSHADOW) )
        {
            //DOn't do shadows right now
            // GRPFIX
            if (g_sun.shadowcolor[3] >= MIN_SHADOW_WORTH_DRAWING)
                modelAddShadow( mat, MIN(alpha, stipple_alpha), model, 64 );
        }
        else
        {
            int ok_in_minspec = 1;
            int occluder = 0;
    
            if (vis != VIS_DRAWALL && group_draw.zocclusion && do_occluder_check && alpha == 255 && stipple_alpha == 255 && !(model->flags & (OBJ_TREEDRAW | OBJ_FANCYWATER)) && !(model->trick && (model->trick->flags1 & (TRICK_ALPHACUTOUT|TRICK_ADDITIVE|TRICK_SUBTRACTIVE))) && !def->shadow_dist && !def->no_occlusion)
            {
                occluder = zoCheckAddOccluder(lowLodModel?lowLodModel:model, mat, uid);
                checked_occluder = 1;
            }

            avsn_params.model = model;
            avsn_params.mat = mat;
            avsn_params.mid = (draw->lod_fadenode && (def->parent_fade || draw->lod_fadenode==2))?draw->node_mid:mid;
            avsn_params.alpha = alpha;
            avsn_params.stipple_alpha = stipple_alpha;
            avsn_params.rgbs = rgbs;
            avsn_params.tint_colors = draw->tint_color_ptr;
            avsn_params.has_tint = !!draw->tint_color_ptr;
            avsn_params.light = group_draw.globFxLight;
            avsn_params.brightScale = g_glowBrightScale;
            avsn_params.tricks = draw->tricks;
            avsn_params.isOccluder = occluder;
            avsn_params.noUseRadius = draw->lod_fadenode==2;
            avsn_params.beyond_fog = beyond_fog;
            avsn_params.uid = uid;
            avsn_params.hide = hide;
            avsn_params.is_gfxnode = false;
            avsn_params.from_tray = draw->tray_child;

            // force fallback material on some renderPasses
            if (!gfx_state.current_viewport_info || (gfx_state.current_viewport_info->renderPass == RENDERPASS_COLOR ||
                                                     gfx_state.current_viewport_info->renderPass == RENDERPASS_IMAGECAPTURE ||
                                                     gfx_state.current_viewport_info->renderPass == RENDERPASS_TAKEPICTURE ||
                                                     (gfx_state.current_viewport_info->renderPass == RENDERPASS_CUBEFACE && game_state.cubemapMode == CUBEMAP_DEBUG_SUPERHQ) ||
                                                     (gfx_state.current_viewport_info->renderPass == RENDERPASS_CUBEFACE && game_state.cubemap_flags&kGenerateStatic) ))
                avsn_params.use_fallback_material = (lod && (lod->flags & LOD_USEFALLBACKMATERIAL)) || game_state.force_fallback_material;
            else
                avsn_params.use_fallback_material = 1;

            if (!draw->no_planar_reflections && def->model && def->model->reflection_quad_count)
            {
                modelSetupPRQuads(def->model);
                avsn_params.num_reflection_quads = def->model->reflection_quad_count;
                avsn_params.reflection_quad_verts = def->model->unpack.reflection_quads;
            }

            {
                // AVSN v2
                int i = -1;
                MiniTracker *mini_tracker = groupDrawGetMiniTracker(uid, is_welded);
                if(mini_tracker)
                {
                    for(i = lod_idx; i >= 0; --i)
                    {
                        if(mini_tracker->tracker_binds[i])
                        {
                            avsn_params.materials = mini_tracker->tracker_binds[i];
                            break;
                        }
                    }
                }
                if(i < 0)
                    avsn_params.materials = model->tex_binds;
                addViewSortNodeWorldModel();
            }

            if (game_state.draw_scale != MIN_SPEC_DRAWSCALE)
            {
                if (group_draw.minspec_fog_far_dist && d > SQR(group_draw.minspec_fog_far_dist + def->radius))
                    ok_in_minspec = 0;
            }

#ifndef FINAL
            if (gfx_state.mainViewport && model->vbo)
                rdrStatsAddLevelModel(model->vbo, ok_in_minspec);
#endif
        }
    }

    return checked_occluder;
}

// CD: don't inline this function, it will screw up code cache locality
int drawModelNonInline(Model *model, Model *lowLodModel, AutoLOD *lod, GroupDef *def, Mat4 mat, Vec3 mid, DefTracker *tracker, DrawParams *draw, F32 d, VisType vis, int uid, int do_occluder_check, int lod_idx, int lod_override, bool is_welded, int hide)
{
    return drawModel(model, lowLodModel, lod, def, mat, mid, tracker, draw, d, vis, uid, do_occluder_check, lod_idx, lod_override, is_welded, hide);
}

// CD: don't inline this function, it will screw up code cache locality
void drawDefModel(GroupDef *def, Mat4 mat, Vec3 mid, DefTracker *tracker, DrawParams *draw, F32 d, VisType vis, int uid)
{
    TrickInfo        *trick = 0;
    Model *model = def->model;

    if (model->trick)
        trick = model->trick->info;

    if (group_draw.search_visible)
        group_draw.search_visible_callback(tracker, model, groupDrawGetMiniTracker(uid, 0));

    // AutoLOD
    if (def->auto_lod_models && model->lod_info && !game_state.edit_base)
    {
        int numLODs = eaSize(&def->auto_lod_models);
        int lod_override;

#ifndef FINAL
        // There may be a model lod override on the DefTracker if the "Edit LOD" window is open
        // and set to show a particular LOD. In this case we just want to force show the given
        // LOD (i.e.. it should just have lod alpha of 255 and not test against distances, etc.
        // This takes precedence over any viewport wide lod_model_override
        if (tracker && tracker->lod_override)
        {
            // tracker->lod_override is 0 for 'no override' or currently selected panel LOD#+1
            lod_override = tracker->lod_override;
        }
        else if (g_client_debug_state.lod_model_override)
        {
            // gfx HUD is overriding the model lod explicitly
            // we still use the group visibility distances to fade in/out as a whole
            lod_override = g_client_debug_state.lod_model_override;
        }
        else
#endif
        {
            // viewport wide lod override. 0 for 'no override' or MAXLOD for lowest lod
            lod_override = gfx_state.lod_model_override;
        }

        if (lod_override > 0)
        {
            int lod_index;

            // It has been requested that we draw with a specific LOD
            // Generally this is done for the "Edit LOD" window or to improve performance, e.g., drawing with low LODs in reflection viewports.
            // Notify drawModel() that this is happening with the lod_override argument so it can adjust lod fade calculations accordingly

            // clamp the override value to the maximum available lods
            if (lod_override > numLODs)
                lod_override = numLODs;

            lod_index = lod_override - 1;    // lod_override is offset + 1 from real lod index
            drawModel(def->auto_lod_models[lod_index], def->auto_lod_models[numLODs-1], model->lod_info->lods[lod_index], def, mat, mid, tracker, draw, d, vis, uid, 1, lod_index, 1, 0, def->hideForHeadshot);
        }
        else
        {
            int i, checked_occluder = 0;

            for (i = 0; i < numLODs; i++)
            {
                checked_occluder = drawModel(def->auto_lod_models[i], def->auto_lod_models[numLODs-1], model->lod_info->lods[i], def, mat, mid, tracker, draw, d, vis, uid, !checked_occluder, i, 0, 0, def->hideForHeadshot) || checked_occluder;
            }
        }
    }
    else
    {
        drawModel(model, 0, 0, def, mat, mid, tracker, draw, d, vis, uid, 1, 0, 0, 0, def->hideForHeadshot);
    }
}


// CD: don't inline this function, it will screw up code cache locality
static void drawChildren(DrawParams *draw,GroupDef *def,F32 d,VisType vis,Mat4 parent_mat,DefTracker *tracker,int uid,int useWelded)
{
    DefTracker    *child = 0;
    GroupEnt    *childGroupEnt;
    F32            dx = ((F32)sqrt(d) - def->radius);
    int            i;


    if (group_draw.do_welding && draw->do_welding && def->try_to_weld && !def->welded_models && draw->light_trkr)
        createWeldedModels(def, uid);

    if ((vis == VIS_EYE || vis == VIS_CONNECTOR))
    {
        if (gfx_state.mainViewport)
        {
            // Only do this on the main viewport to get objects rendering properly in the reflection viewport in the sewers
            vis = VIS_NORMAL;  // draw non-tray children of this tray
        }
    }

    if (uid >= 0)
    {
        // increment one for the parent
        uid += 1;
    }

    if (tracker)
        child = tracker->entries;
    for(childGroupEnt = def->entries,i=0;i<def->count;i++,childGroupEnt++)  
    {    
        childGroupEnt->def->hideForHeadshot = def->hideForHeadshot;
        if (!useWelded || eaiFind(&def->unwelded_children, i)>=0)
        {
            F32 vis_dist;

            //Possibly move this cacheing out to when the groupent is created?
            if( !(childGroupEnt->flags_cache & CACHEING_IS_DONE) ) // groups with a dyn_parent_tray get drawn with their parent tray
                makeAGroupEnt( childGroupEnt, childGroupEnt->def, childGroupEnt->mat, draw->view_scale, child && child->dyn_parent_tray ); 

            vis_dist = calculateVisDist(childGroupEnt);
            
            if (dx < vis_dist || childGroupEnt->flags_cache & HAS_FOG)
            {
                if (childGroupEnt->has_dynamic_parent_cache)
                {
                    if (uid >= 0)
                        vistraySetDetailUid(child, uid);
                }
                else
                {
                    drawDefInternal(childGroupEnt, parent_mat, child , vis, draw, uid);
                }
            }
        } else {
            // Still need recursive_count_cache updated
            if( !(childGroupEnt->flags_cache & CACHEING_IS_DONE) ) // groups with a dyn_parent_tray get drawn with their parent tray
                childGroupEnt->recursive_count_cache = childGroupEnt->def->recursive_count; 
        }

        if (child)
            child++;

        if (uid >= 0)
        {
            // increment for the child
            uid += 1 + childGroupEnt->recursive_count_cache;
        }
    }
}

static INLINEDBG void handleDefTracker(DrawParams *draw,GroupDef *def,DefTracker *tracker,VisType vis)
{
    if (tracker->inherit)
        draw->tricks = tracker->tricks;
    if (def->has_ambient)
    {
        if (!tracker->light_grid)
            lightTracker(tracker);
        draw->light_trkr = tracker;
    }

    if (!tracker->entries && (draw->light_trkr || def->open_tracker || group_draw.search_visible))
        trackerOpen(tracker);

    if (def->is_tray)
        vistrayOpenPortals(tracker);

    if (def->has_fx)
    {
        // @todo FX
        // Until fx are made to have an independent update and render we should only do this
        // update if the traversal is on the main viewport where the fx engine is run
        // @todo - do we also need to check for 'RENDERPASS_IMAGECAPTURE' renders here?
        if (gfx_state.mainViewport)
        {
            tracker->fx_id = fxManageWorldFx(
                tracker->fx_id,
                def->type_name,
                tracker->mat,
                draw->view_scale * game_state.lod_scale * def->vis_dist,
                draw->view_scale * game_state.lod_scale * def->lod_farfade,
                draw->light_trkr,
                draw->tint_color_ptr);
        }
    }
    if (def->outside)
        group_draw.see_outside = 1;
}

static INLINEDBG void softKillAllTrackerFX(DefTracker * tracker) {
    int i;
    if (tracker==NULL) return;
    if (tracker->fx_id)
        fxDelete(tracker->fx_id,SOFT_KILL);
    for (i=0;i<tracker->count;i++)
        softKillAllTrackerFX(&tracker->entries[i]);
}

static INLINEDBG int anyFXAlive(DefTracker * tracker) {
    int i;
    if (tracker==NULL) return 0;
    if (tracker->fx_id)
        if (fxIsFxAlive(tracker->fx_id))
            return 1;
    for (i=0;i<tracker->count;i++)
        if (anyFXAlive(&tracker->entries[i]))
            return 1;
    return 0;
}

#define BURNING_BUILDING_EFFECTS_PLAYED    (1<<0)    //have the effects already been played?
#define BURNING_BUILDING_EFFECTS_KILLED    (1<<1)    //have the effects been killed already?
#define BURNING_BUILDING_VALID            (1<<2)    //valid if it is in the valid health range
#define BURNING_BUILDING_FADEOUT        (1<<3)    //building is fading out
#define BURNING_BUILDING_FADEIN            (1<<4)    //building is fading in
#define FADEOUT_TIME    SEC_TO_ABS(3)
int drawBurningBuilding(GroupDef * def,DefTracker * tracker,DrawParams *draw) {
    char * value=0;
    StashElement e;
    int minHealth=-1;
    int maxHealth=101;
    int repair=-1;
    int ret=0;
    int flags;
    if (tracker==0) return 1;

    if (def->properties)
    {
        stashFindElement(def->properties,"MaxHealth", &e);
        if (e) {
            value=((PropertyEnt *)stashElementGetPointer(e))->value_str;
            maxHealth=atoi(value);
        }
        stashFindElement(def->properties,"MinHealth", &e);
        if (e) {
            value=((PropertyEnt *)stashElementGetPointer(e))->value_str;
            minHealth=atoi(value);
        }
        stashFindElement(def->properties,"Repairing", &e);
        if (e) {
            value=((PropertyEnt *)stashElementGetPointer(e))->value_str;
            repair=atoi(value);
        }
    }
    if (minHealth==-1 && maxHealth==101) return 1;
    flags = tracker->burning_buildings_flags;
    if (burningBuildingStatus.hp>=minHealth && burningBuildingStatus.hp<=maxHealth && 
            repair==burningBuildingStatus.repair)
    {
        ret=1;
        if (scene_info.stippleFadeMode && draw)
        {
            if (flags & BURNING_BUILDING_FADEOUT)
            {
                // Stop fading out
                if (tracker->tricks)
                    tracker->tricks->flags2 &= ~TRICK2_STIPPLE2;
                tracker->inherit = 0;
                // Start fading in
                allocTrackerTrick(tracker)->flags2 |= TRICK2_STIPPLE1;
                tracker->inherit = 1;
                flags|=BURNING_BUILDING_FADEIN;
                tracker->burning_buildings_fade_start_abs_time = 
                    global_state.client_abs - FADEOUT_TIME + global_state.client_abs - tracker->burning_buildings_fade_start_abs_time;
                //printf("Started fading %s IN from OUT at %d, using %d hp %d\n", def->name, global_state.client_abs, tracker->burning_buildings_fade_start_abs_time, burningBuildingStatus.hp);
            } else if (!(flags & BURNING_BUILDING_VALID))
            {
                // Start fading in
                allocTrackerTrick(tracker)->flags2 |= TRICK2_STIPPLE1;
                tracker->inherit = 1;
                flags|=BURNING_BUILDING_FADEIN;
                tracker->burning_buildings_fade_start_abs_time = global_state.client_abs;
                //printf("Started fading %s IN at %d hp %d\n", def->name, global_state.client_abs, burningBuildingStatus.hp);
            }
        }
        flags|=BURNING_BUILDING_VALID;
        flags&=~BURNING_BUILDING_FADEOUT;
        e = NULL;
        if (!(flags & BURNING_BUILDING_EFFECTS_PLAYED)) {
            if (def->properties)
                stashFindElement(def->properties,"Effect", &e);
            if (e) {
                FxParams fxp;
                fxInitFxParams(&fxp);
                copyMat4(tracker->mat, fxp.keys[0].offset);
                value=((PropertyEnt *)stashElementGetPointer(e))->value_str;
                fxCreate(value,&fxp);
                flags|=BURNING_BUILDING_EFFECTS_PLAYED;
            }
        }
    } else {
        if (flags & BURNING_BUILDING_VALID) {
            if (scene_info.stippleFadeMode && draw) {
                if (flags & BURNING_BUILDING_FADEIN) {
                    // Stop fading in
                    if (tracker->tricks)
                        tracker->tricks->flags2 &= ~TRICK2_STIPPLE1;
                    tracker->inherit = 0;
                    // Start fading out
                    allocTrackerTrick(tracker)->flags2 |= TRICK2_STIPPLE2;
                    tracker->inherit = 1;
                    flags|=BURNING_BUILDING_FADEOUT;
                    tracker->burning_buildings_fade_start_abs_time = 
                        global_state.client_abs - FADEOUT_TIME + global_state.client_abs - tracker->burning_buildings_fade_start_abs_time;
                    //printf("Started fading %s OUT from IN at %d, using %d hp %d\n", def->name, global_state.client_abs, tracker->burning_buildings_fade_start_abs_time, burningBuildingStatus.hp);
                } else {
                    // Start fading out
                    allocTrackerTrick(tracker)->flags2 |= TRICK2_STIPPLE2;
                    tracker->inherit = 1;
                    flags|=BURNING_BUILDING_FADEOUT;
                    tracker->burning_buildings_fade_start_abs_time = global_state.client_abs;
                    //printf("Started fading %s OUT at %d hp %d\n", def->name, global_state.client_abs, burningBuildingStatus.hp);
                }
            }
            flags&=~(BURNING_BUILDING_VALID|BURNING_BUILDING_EFFECTS_KILLED);
        }
    }
    if (!(flags & (BURNING_BUILDING_VALID|BURNING_BUILDING_FADEOUT))) {
        if (anyFXAlive(tracker)) {
            if (!(flags & BURNING_BUILDING_EFFECTS_KILLED)) {
                softKillAllTrackerFX(tracker);
                flags|=BURNING_BUILDING_EFFECTS_KILLED;
            }
            ret=1;
        } else {
            ret=0; //flags&BURNING_BUILDING_VALID;
        }
    }
    if (scene_info.stippleFadeMode && draw && (flags & BURNING_BUILDING_FADEOUT))
    {
        U32 time_since = global_state.client_abs - tracker->burning_buildings_fade_start_abs_time;
        F32 alpha = 1.0 - (F32)time_since / (F32)FADEOUT_TIME;
        if (tracker->tricks) // To override wireframe coloring from the editor
            ZeroStruct(&tracker->tricks->trick_rgba);
        if (alpha <= 0) {
            if (anyFXAlive(tracker)) {
                if (!(flags & BURNING_BUILDING_EFFECTS_KILLED)) {
                    softKillAllTrackerFX(tracker);
                    flags|=BURNING_BUILDING_EFFECTS_KILLED;
                }
                // Need to keep drawing until the fx finish dying (should be 1 frame, generally?)
                draw->alpha = 0.00001;
                ret = 1;
            } else {
                // Stop fading out
                if (tracker->tricks)
                    tracker->tricks->flags2 &= ~TRICK2_STIPPLE2;
                tracker->inherit = 0;
                flags=0; // Clear all flags
                tracker->burning_buildings_fade_start_abs_time = 0;
                ret = 0; // already 0?
            }
        } else {
            draw->alpha = alpha==0?0.0001:alpha;
            ret = 1;
        }
    } else if (scene_info.stippleFadeMode && draw && (flags & BURNING_BUILDING_FADEIN))
    {
        U32 time_since = global_state.client_abs - tracker->burning_buildings_fade_start_abs_time;
        F32 alpha = (F32)time_since / (F32)FADEOUT_TIME;
        if (tracker->tricks) // To override wireframe coloring from the editor
            ZeroStruct(&tracker->tricks->trick_rgba);
        if (alpha >= 1.0) {
            // Stop fading in
            if (tracker->tricks)
                tracker->tricks->flags2 &= ~TRICK2_STIPPLE1;
            tracker->inherit = 0;
            flags&=~BURNING_BUILDING_FADEIN;
            draw->alpha = 1.0;
            ret = 1;
        } else {
            draw->alpha = time_since==0?0.0001:alpha;
            ret = 1;
        }
    } else if (ret==0) {
        flags=0;
    }
    // overwrite it with new flags value
    tracker->burning_buildings_flags = flags;
    return ret;
}

int handleDynGroup(GroupDef *def, DefTracker *tracker)
{
    StashElement e;
    int burningBuildingSetFlag=true;
    burningBuildingStatus = groupDynGetStatus( tracker->dyn_group );
    buildingIsBurning=1;
    if (editMode())
    {
        stashFindElement(def->properties,"Health", &e);
        if (e)
        {
            char * value=((PropertyEnt *)stashElementGetPointer(e))->value_str;
            burningBuildingStatus.hp=atoi(value);
            if (value[0]!=0)
            {
                if (value[strlen(value)-1]=='-')
                    burningBuildingStatus.repair=0;
                else if (value[strlen(value)-1]=='+')
                    burningBuildingStatus.repair=1;
                else
                {
                    buildingIsBurning=false;
                    burningBuildingSetFlag=false;
                }
            }
        }
    }

    return burningBuildingSetFlag;
}

bool trackerIsHiddenFromBurningBuilding(DefTracker *tracker, bool *pParentIsBreakable)
{
    bool parent_is_breakable;
    if (tracker->dyn_group) {
        burningBuildingStatus = groupDynGetStatus( tracker->dyn_group );
    }
    if (tracker->def->breakable && pParentIsBreakable) {
        *pParentIsBreakable = true;
        return false; // Don't traverse any farther up!
    }
    if (!tracker->parent) {
        return false;
    }
    if (trackerIsHiddenFromBurningBuilding(tracker->parent, &parent_is_breakable)) {
        return true;
    }
    if (parent_is_breakable) {
        if (!drawBurningBuilding(tracker->def,tracker,NULL)) {
            return true;
        }
    }
    return false;
}

static INLINEDBG void drawDef(DrawParams *draw,F32 d,Vec3 mid,GroupDef *def,DefTracker *tracker,VisType vis,Mat4 parent_mat,int uid,GroupEnt *groupEnt)
{
    int burningBuildingSetFlag=false;
    int lastSwapIndex=0;
    int useWelded;

    if ((def && (def->child_breakable || def->breakable || buildingIsBurning))&& tracker && !tracker->count)
        trackerOpen(tracker);

    if (def && tracker && buildingIsBurning)
    {
        if (!drawBurningBuilding(def,tracker,draw))
            return;
    }

    if (def && groupHidden(def))
        return;

    if (tracker) 
    {
        if (tracker->tricks && (tracker->tricks->flags1 & TRICK_HIDE))
            return;
        if (tracker->invisible)
            return;

        handleDefTracker(draw,def,tracker,vis);

        if ( tracker->dyn_group )
            burningBuildingSetFlag = handleDynGroup(def, tracker);
    }

    if (def && def->has_volume)
    {
        int i;
        Vec3 scalevec;
        Mat4 mat={0}, scalemat={0};
        GroupDef *newdef = (def->entries && def->entries[0].def && def->entries[0].def->is_tray)?groupDefFindWithLoad("tray_collision_box_16x16"):groupDefFindWithLoad("collision_box_16x16");

        for(i=0;i<3;i++)
            scalevec[i] = (def->box_scale[i]) / (newdef->max[i] - newdef->min[i]);
        scaleMat3Vec3Xfer(unitmat,scalevec,scalemat);
        mulMat3(parent_mat,scalemat,mat);
        mulVecMat4(zerovec3,parent_mat,mat[3]);

        if (newdef->vis_dist)
            draw->view_scale *= def->vis_dist / newdef->vis_dist;
        drawDefModel(newdef,mat,mid,0,draw,d,vis,uid);
        return;
    }

    //Hack so fx can draw Defs that have fx without using trackers. Allows entities with groupDefs attached to draw fx in their groupDef correctly.
    if( !tracker && def->has_fx && group_draw.globFxMiniTracker )
    {
        // @todo FX
        // Until fx are made to have an independent update and render we should only do this
        // update if the traversal is on the main viewport where the fx engine is run
        // @todo - do we also need to check for 'RENDERPASS_IMAGECAPTURE' renders here?
        if (gfx_state.mainViewport)
        {
            handleGlobFxMiniTracker( group_draw.globFxMiniTracker, def, parent_mat, draw->view_scale * game_state.lod_scale );
        }
    }
    //Set custom color and texture madness
    if (def->has_tint_color)
        draw->tint_color_ptr = (U8*)def->color;

    if (def->has_alpha)
    {
        if (draw->alpha)
            draw->alpha *= def->groupdef_alpha;
        else
            draw->alpha = def->groupdef_alpha;
    }

    if (def->pulse_glow)
    {
        static F32 pulse_time;
        static int last_update;

        if (last_update != global_state.global_frame_count)
        {
            pulse_time += TIMESTEP;
            pulse_time = fmod(pulse_time, PULSE_PERIOD);
            last_update = global_state.global_frame_count;
        }

        if (pulse_time < PULSE_PERIOD / 2)
            g_glowBrightScale = pulse_time / (PULSE_PERIOD / 2);
        else
            g_glowBrightScale = (PULSE_PERIOD - pulse_time) / (PULSE_PERIOD / 2);

        g_glowBrightScale = 1.0f + g_glowBrightScale * (PULSE_BRIGHTNESS - 1.0f);
    }


    if (def->do_welding)
        draw->do_welding = 1;

    useWelded = group_draw.do_welding && draw->do_welding && def->welded_models && draw->light_trkr;

    if (useWelded)
    {
        int i;
        for (i = 0; i < eaSize(&def->welded_models); i++)
            drawModelNonInline(def->welded_models[i],0,0,def,parent_mat,mid,tracker,draw,d,vis,uid + def->welded_models_duids[i],1,i, 0, true, 0);
    }
    else if (def->model)
    {
        if (!group_draw.nosway && def->model->trick && def->model->trick->flags2 & TRICK2_HAS_SWAY)
            swayThisMat( parent_mat, def->model, uid ); //A little flaky: changes parent_mat, thus everything this is grouped with drawn after it
        drawDefModel(def,parent_mat,mid,tracker,draw,d,vis,uid);
    }

    // MW what? Why is this after the draw for one thing? // JE: Because this is a flag telling all of it's children to use it's midpoint for LOD calculations
    if (def->lod_fadenode)
    {
        copyVec3(mid,draw->node_mid);
        draw->node_radius = def->radius;
        draw->lod_fadenode = def->lod_fadenode;
    }

    if (def->count) {
        drawChildren(draw,def,d,vis,parent_mat,tracker,uid,useWelded);
    }
    g_glowBrightScale = 1.0f;

    // Draw tray details except in the DRAWALL case--ordinary hierarchy traversal is used instead
    if (tracker && def && def->is_tray && vis != VIS_DRAWALL)
        vistrayDrawDetails(tracker, draw, parent_mat, vis);

    if (burningBuildingSetFlag)
        buildingIsBurning=0;

    if (tracker && tracker->selected)
    {
        if (edit_state.showBounds == 1)
        {
            Mat4 matModelToWorld;
            mulMat4(group_draw.inv_cam_mat, parent_mat, matModelToWorld);
            drawBox3D(def->min, def->max, matModelToWorld, 0xffff0f0f, 2);
        }
        else if (edit_state.showBounds == 2)
        {
            Vec3 mid_world;
            mulVecMat4(mid, group_draw.inv_cam_mat, mid_world);
            drawSphereMidRad(mid_world, groupEnt->radius, 20, 0xff0f0fff);
        }
    }
}

static INLINEDBG F32 closeEnough(DrawParams *draw,Vec3 mid,GroupEnt * groupEnt)
{
    F32        d;
    F32        groupEntDist;

    CALL_INC
    //GroupEnt, has the midpoint xformed by the local_mat cached
    //Use this do do all visibility checking, and defer the mulMat till you know you need the def
    if ( draw->lod_fadenode && ((groupEnt->flags_cache & PARENT_FADE) || draw->lod_fadenode==2)) //this happens almost never?
        d = lengthVec3Squared(draw->node_mid);
    else
        d = lengthVec3Squared(mid);

    if (groupEnt->flags_cache & (HAS_FOG | VIS_DOORFRAME | CHILD_PARENT_FADE))
        return d;

    groupEntDist = calculateVisDist(groupEnt);

    //By far the most common rejection
    if ( d > SQR(groupEntDist))
        return 0;
    return d;
}

static INLINEDBG int sphereInFrustum(Vec3 mid,GroupEnt * groupEnt)
{
    int        clip;

    // Typically, another 30-40% get rejected here (tested in atlas park) Is this group on screen?
    CALL_INC
    if (groupEnt->flags_cache & HAS_SHADOW)
    {
        GroupDef *def = groupEnt->def;
        if (!gfxSphereVisible(mid,def->radius) && !shadowVolumeVisible(mid,def->radius,def->shadow_dist))
            return 0;
        clip = CLIP_NONE;
    }
    else
    {
        clip = gfxSphereVisible(mid,groupEnt->radius);
    }
    return clip;
}

float zoBBBias = 0.9f;

int drawDefInternal( GroupEnt *groupEnt, Mat4 parent_mat_p, DefTracker *tracker, VisType vis, DrawParams *draw, int uid )
{
    Vec3        mid;
    F32            draw_scale;
    DrawParams    local;
    Mat4        parent_mat;
    F32            d;
    int            clip;
    GroupDef    *def;
    Mat4Ptr        local_mat;

#ifdef DEBUG_UIDS_GOING_BAD
    if (tracker) {
        if (tracker->cached_uid_version != game_state.groupDefVersion) {
            tracker->cached_uid = uid;
            tracker->cached_uid_version = game_state.groupDefVersion;
        }
        if (game_state.test1)
            assert(tracker->cached_uid == uid);
    }
#endif

    if (!group_draw.see_everything && groupEnt->flags_cache & ONLY_EDITORVISIBLE)
        return 0;

    CALL_INIT
    mulVecMat4(groupEnt->mid_cache,parent_mat_p,mid);

    if (!(d=closeEnough(draw,mid,groupEnt)))
        return 0;
    if (!(clip=sphereInFrustum(mid,groupEnt)))
        return 0;

    // common rejections are above, try to avoid loading def until here because of memory stalls
    def = groupEnt->def; 
    local_mat    = groupEnt->mat;

    if (def->pre_alloc)
        groupFileLoadFromName(def->name);

    // If this group is beyond the fog, and noFogClip is not set, and ClipFoggedGeometry is set, discard it
    // don't fog clip if a parent or child has noFogClip set
    CALL_INC
    if (group_draw.fog_clip) {
        F32 fog_far_dist_plus_radius;
        if (draw->lod_fadenode && (def->parent_fade || draw->lod_fadenode==2)) {
            fog_far_dist_plus_radius = group_draw.fog_far_dist + draw->node_radius;
        } else {
            fog_far_dist_plus_radius = group_draw.fog_far_dist + def->radius;
        }
        if (vis != VIS_DRAWALL && !(draw->no_fog_clip|def->no_fog_clip|def->child_no_fog_clip) && d > SQR(fog_far_dist_plus_radius + 100)) // the +100 is for the fade out distance, so fog clipped objects don't pop
            return 0;
    }

    CALL_INC
    draw_scale = draw->view_scale * def->lod_scale;
    // reject near lod's
    if (d < SQR((def->lod_near-def->lod_nearfade) * draw_scale * game_state.lod_scale))
        return 0;
    
    CALL_INC
    local = *draw;
    draw = &local;
    draw->view_scale = draw_scale;
    draw->no_fog_clip |= def->no_fog_clip;
    draw->no_planar_reflections |= def->disable_planar_reflections;

    CALL_INC
    if (def->is_tray)
    {
        if (vis == VIS_NORMAL)
        {
            // this is a tray, but we are not supposed to be drawing a tray.
            if (tracker)
            {
                if (def->outside)
                    vistrayAddOutsideTray(tracker);
                if (uid >= 0)
                    vistraySetUid(tracker, uid);
            }
            return 0;
        }

        // we are drawing in the editor, so draw this with the same lighting as it would be normally
        draw->light_trkr = 0;
    }

    //matrix Multiply usually deferred to here
    if( groupEnt->flags_cache & CACHEING_IS_DONE ) //Maybe just always do this
        mulMat4Inline(parent_mat_p,local_mat,parent_mat); 

    if (!def->shadow_dist)
    {
        if (vis != VIS_DRAWALL && group_draw.zocclusion && (def->model || def->count>1 || def->welded_models))
        {
            int nearClipped;
            Vec3 min, max, eye_bounds[8];
            copyVec3(def->min, min);
            copyVec3(def->max, max);
            min[0] -= zoBBBias;
            min[1] -= zoBBBias;
            min[2] -= zoBBBias;
            max[0] += zoBBBias;
            max[1] += zoBBBias;
            max[2] += zoBBBias;

            mulBounds(min, max, parent_mat, eye_bounds);

            nearClipped = isBoxNearClipped(eye_bounds);
            if (nearClipped == 2)
                return 0;

            // only test non-leaf nodes if they are not clipped by the near plane
#if !GROUPDRAW_DEBUG_SHOW_OBB
            else if ((def->model || def->welded_models || !nearClipped) && !zoTest2(eye_bounds, nearClipped, uid, groupEnt->recursive_count_cache))
                return 0;
#else
            else if ((def->model || def->welded_models || !nearClipped))
            {
                if     (!zoTest2(eye_bounds, nearClipped, uid, groupEnt->recursive_count_cache))
                    return 0;
                else
                {
                    // HYPNOS_AKI: Draw lines around unoccluded objects
                    // passed occlusion test
                    // remove view matrix from eye_bounds
                    Mat4 modelToView;

                    mulMat4Inline(cam_info.inv_viewmat, parent_mat_p, modelToView); 
                    mulBounds(min, max, modelToView, eye_bounds);

                    drawLine3DWidth(eye_bounds[0], eye_bounds[1], 0xFFFFFFFF, 1.0f);
                    drawLine3DWidth(eye_bounds[0], eye_bounds[2], 0xFFFFFFFF, 1.0f);
                    drawLine3DWidth(eye_bounds[0], eye_bounds[4], 0xFFFFFFFF, 1.0f);

                    drawLine3DWidth(eye_bounds[1], eye_bounds[5], 0xFFFFFFFF, 1.0f);
                    drawLine3DWidth(eye_bounds[2], eye_bounds[6], 0xFFFFFFFF, 1.0f);

                    drawLine3DWidth(eye_bounds[3], eye_bounds[1], 0xFFFFFFFF, 1.0f);
                    drawLine3DWidth(eye_bounds[3], eye_bounds[2], 0xFFFFFFFF, 1.0f);

                    drawLine3DWidth(eye_bounds[4], eye_bounds[5], 0xFFFFFFFF, 1.0f);
                    drawLine3DWidth(eye_bounds[4], eye_bounds[6], 0xFFFFFFFF, 1.0f);

                    drawLine3DWidth(eye_bounds[7], eye_bounds[6], 0xFFFFFFFF, 1.0f);
                    drawLine3DWidth(eye_bounds[7], eye_bounds[5], 0xFFFFFFFF, 1.0f);
                    drawLine3DWidth(eye_bounds[7], eye_bounds[3], 0xFFFFFFFF, 1.0f);
                }
            }
#endif

        }
        else if (clip != CLIP_NONE && !checkBoxFrustum(clip, def->min, def->max, parent_mat))
            return 0;
    }


    CALL_INC

    drawDef(draw,d,mid,def,tracker,vis,parent_mat,uid,groupEnt);

    return 1;
}


/////////////////////////////////////////////////////////////////////////////
extern int g_hidden_def_discards;
extern int g_defs_processed;
extern int g_defs_rejected;

#include "render/thread/rt_shadowmap.h"

static INLINEDBG void drawChildren_shadowmap(DrawParams *draw,GroupDef *def,F32 d,VisType vis,Mat4 parent_mat,DefTracker *tracker,int uid,int useWelded)
{
    DefTracker    *child = 0;
    GroupEnt    *childGroupEnt;
    F32            dx = ((F32)sqrt(d) - def->radius);
    int            i;


    if (group_draw.do_welding && draw->do_welding && def->try_to_weld && !def->welded_models && draw->light_trkr)
        createWeldedModels(def, uid);

    if (vis == VIS_EYE || vis == VIS_CONNECTOR)
        vis = VIS_NORMAL;  // draw non-tray children of this tray

    if (uid >= 0)
    {
        // increment one for the parent
        uid += 1;
    }

    if (tracker)
        child = tracker->entries;
    for(childGroupEnt = def->entries,i=0;i<def->count;i++,childGroupEnt++)  
    {    
        childGroupEnt->def->hideForHeadshot = def->hideForHeadshot;
        if (!useWelded || eaiFind(&def->unwelded_children, i)>=0)
        {
            F32 vis_dist;

            //Possibly move this cacheing out to when the groupent is created?
            if( !(childGroupEnt->flags_cache & CACHEING_IS_DONE) ) // groups with a dyn_parent_tray get drawn with their parent tray
                makeAGroupEnt( childGroupEnt, childGroupEnt->def, childGroupEnt->mat, draw->view_scale, child && child->dyn_parent_tray ); 

            vis_dist = calculateVisDist(childGroupEnt);

            if (dx < vis_dist || childGroupEnt->flags_cache & HAS_FOG)
            {
                if (childGroupEnt->has_dynamic_parent_cache)
                {
                    if (uid >= 0)
                        vistraySetDetailUid(child, uid);
                }
                else
                {
                    drawDefInternal_shadowmap(childGroupEnt, parent_mat, child , vis, draw, uid);
                }
            }
        } else {
            // Still need recursive_count_cache updated
            if( !(childGroupEnt->flags_cache & CACHEING_IS_DONE) ) // groups with a dyn_parent_tray get drawn with their parent tray
                childGroupEnt->recursive_count_cache = childGroupEnt->def->recursive_count; 
        }

        if (child)
            child++;

        if (uid >= 0)
        {
            // increment for the child
            uid += 1 + childGroupEnt->recursive_count_cache;
        }
    }
}

static int modelCalcAlpha_shadowmap(GroupDef *def,F32 dist,F32 scale,DrawParams *draw,AutoLOD *lod,int *beyond_fog, VisType vis)
{
    F32 alpha;
    F32 lod_near,lod_nearfade,lod_far,lod_farfade;
    F32 fog_far_dist_plus_radius;
    Model *model = def->model;

    scale *= game_state.lod_scale;

    alpha = 1.0f;

    if (lod)
    {
        lod_near        = lod->lod_near        * scale; 
        lod_nearfade    = lod->lod_nearfade    * scale; 
        lod_far            = lod->lod_far        * scale;
        lod_farfade        = lod->lod_farfade    * scale;
    }
    else
    {
        lod_near        = def->lod_near        * scale; 
        lod_nearfade    = def->lod_nearfade * scale; 
        lod_far            = def->lod_far        * scale;
        lod_farfade        = def->lod_farfade    * scale;
    }

#ifndef FINAL
    // ability to shrink the lod fade distances (to reduce alpha mesh count)
    if(game_state.lod_fade_range != 1.0f )
    {
        lod_near -= lod_nearfade * (1-game_state.lod_fade_range);
        lod_far += lod_farfade * (1-game_state.lod_fade_range);
        lod_nearfade *= game_state.lod_fade_range;
        lod_farfade *= game_state.lod_fade_range;
    }
#endif

    // check alpha from lod and fog because this tells us if we should
    // draw this object for shadows
    if (dist < lod_near)
    {
        dist = lod_near - dist;
        if (dist >= lod_nearfade)
            return 0;
        alpha = 1.f - dist / lod_nearfade;
    }
    else if (dist > lod_far)
    {
        if (def->vis_window)
            return 255;
        dist -= lod_far;
        if (dist >= lod_farfade)
            return 0;
        alpha = 1.f - dist / lod_farfade;
    }
    else if (def->vis_window)
    {
        alpha = dist / lod_far;
        if (alpha < 0.31f)
            alpha = 0.31f;
    }

    // see if we are beyond fog
    if (draw->lod_fadenode && (def->parent_fade || draw->lod_fadenode==2)) {
        fog_far_dist_plus_radius = group_draw.fog_far_dist + draw->node_radius;
    } else {
        fog_far_dist_plus_radius = group_draw.fog_far_dist + def->radius;
    }

    //@todo even if a model is beyond the fog it may cast a shadow well into
    // the visible scene so we will need to change this test here
    if (model && model->trick && (model->trick->flags1 & TRICK_NOFOG))
        *beyond_fog = 0;
    else
        *beyond_fog = vis != VIS_DRAWALL && dist > fog_far_dist_plus_radius;

    if (*beyond_fog && !draw->no_fog_clip && group_draw.fog_clip)
    {
        F32 alpha2 = 1 - ((dist - fog_far_dist_plus_radius) / 100.f);
        if (alpha2 > 1)
            alpha2 = 1;
        if (alpha2 < 0)
            alpha2 = 0;
        alpha *= alpha2;
    }

    if (draw->alpha)
    {
            alpha *= draw->alpha;
    }

    return 255 * alpha;
}

static INLINEDBG void drawModel_Shadowmap(Model *model, Model *lowLodModel, AutoLOD *lod, GroupDef *def, Mat4 mat, Vec3 mid, DefTracker *tracker, DrawParams *draw, F32 d, VisType vis,  int uid,  int do_occluder_check, int lod_idx, int lod_override, bool is_welded, int hide)
{
    U8                *rgbs = 0;
    int                alpha;
    int                beyond_fog = 0;
    int                fadeMode = game_state.shadowStippleOpaqueFadeMode;

    // should we even be drawing this model into the shadowmap?
    // @todo some of this pruning maybe done up front once? (e.g. shadow/editor geo)
    // but it will need model to already have been streamed in
    // @todo shadows will force more models to stream in than in the past if they are going to cast
    // into the view frustum

    if (hide)
        return;

    if(!model || !(model->loadstate & LOADED))
        return;

    if (model->trick )
    {
        if ((model->trick->flags1 & TRICK_FANCYWATEROFFONLY) && (game_state.waterMode > WATER_OFF) && vis != VIS_DRAWALL)
            return;

        if ( model->trick->flags2 & TRICK2_CASTSHADOW )
            return;

        if (model->trick->flags1 & TRICK_BASEEDITVISIBLE )
            return;

        if (model->trick->flags1 & (TRICK_ADDITIVE|TRICK_SUBTRACTIVE))
            return;

        if ((model->trick->flags1 & TRICK_NIGHTLIGHT) && !(model->trick->flags2 & TRICK2_HASADDGLOW))
            return;

        // Alpha test objects use game_state.shadowStippleAlphaFadeMode
        if ((model->trick->flags1 & TRICK_ALPHACUTOUT) || (model->trick->flags2 & TRICK2_ALPHAREF))
        {
            fadeMode = game_state.shadowStippleAlphaFadeMode;
        }
    }

    alpha = modelCalcAlpha_shadowmap(def,fsqrt(d),draw->view_scale,draw,lod,&beyond_fog,vis);

    // Trees use game_state.shadowStippleAlphaFadeMode
    if (model->flags & OBJ_TREEDRAW)
    {
        fadeMode = game_state.shadowStippleAlphaFadeMode;
    }

    if ((fadeMode == stippleFadeAlways) ||
        ((fadeMode == stippleFadeHQOnly) && (game_state.shadowShaderSelection == SHADOWSHADER_HIGHQ)))    

    {
        // check overall lod/fog alpha of object against threshold so that we don't bother rendering into shadow map
        // if it shouldn't show up
        if (alpha < game_state.shadowmap_lod_alpha_begin)
            return;


        // can use a stipple pattern to 'fade in/out' lods as they transition
        // but we only need to do it a little of the ways in just so shadow doesn't 'pop'
        // 255 means don't do any stipple fading
        if (alpha >= game_state.shadowmap_lod_alpha_end)
            alpha = 255;
    }
    else
    {
        // check overall lod/fog alpha of object against threshold so that we don't bother rendering into shadow map
        // if it shouldn't show up
        if (alpha < game_state.shadowmap_lod_alpha_snap)
            return;

        alpha = 255;
    }

    {
        if (vis != VIS_DRAWALL && alpha == 255 && !(model->flags & (OBJ_TREEDRAW | OBJ_FANCYWATER)) && !(model->trick && (model->trick->flags1 & (TRICK_ALPHACUTOUT|TRICK_ADDITIVE|TRICK_SUBTRACTIVE))) && !def->shadow_dist)
        {
            avsn_params.view_cache_add_occluder = 1;
            avsn_params.view_cache_lowLodModel = lowLodModel;
        }
        else
        {
            avsn_params.view_cache_add_occluder = 0;
            avsn_params.view_cache_lowLodModel = NULL;
        }

        avsn_params.node = NULL;
        avsn_params.model = model;
        avsn_params.mat = mat;
        avsn_params.mid = (draw->lod_fadenode && (def->parent_fade || draw->lod_fadenode==2))?draw->node_mid:mid;
        avsn_params.alpha = alpha;                    // only test alpha for render or not, only draw opaque/alpha test into shadow map
        avsn_params.stipple_alpha = alpha;
        avsn_params.rgbs = rgbs;
        avsn_params.tint_colors = draw->tint_color_ptr;
        avsn_params.brightScale = 1.f;
        avsn_params.isOccluder = 0;
        avsn_params.noUseRadius = draw->lod_fadenode==2;
        avsn_params.beyond_fog = beyond_fog;
        avsn_params.uid = uid;
        avsn_params.hide = hide;
        avsn_params.is_gfxnode = false;
        avsn_params.materials = model->tex_binds;

        // analyze the full material or the fallback material for alpha test?
        // some materials may be slightly less work to look at using the fall back material
        // but we are relying on the artists to have setup the correct fallback material in all cases
        if (game_state.shadowDebugFlags&kForceFallbackMaterial)
            avsn_params.use_fallback_material = 1;
        else
            avsn_params.use_fallback_material = lod && (lod->flags & LOD_USEFALLBACKMATERIAL);

        addViewSortNodeWorldModel_Shadowmap();
    }
    return;
}

void drawModel_ShadowmapNonInline(Model *model, Model *lowLodModel, AutoLOD *lod, GroupDef *def, Mat4 mat, Vec3 mid, DefTracker *tracker, DrawParams *draw, F32 d, VisType vis, int uid, int do_occluder_check, int lod_idx, int lod_override, bool is_welded, int hide)
{
    drawModel_Shadowmap(model, lowLodModel, lod, def, mat, mid, tracker, draw, d, vis, uid, do_occluder_check, lod_idx, lod_override, is_welded, hide);
}

static INLINEDBG void drawDef_shadowmap(DrawParams *draw, F32 d, Vec3 mid, GroupDef *def, DefTracker *tracker, VisType vis, Mat4 parent_mat, int uid, GroupEnt *groupEnt)
{
    int useWelded = false;
    int view_cache_index = -1;

    if (def && (def->volume_trigger || def->has_volume) && edit_state.hideCollisionVolumes)
        return;

    if (ViewCache_IsCaching())
    {
        //Set custom color and texture madness
        if (def->has_tint_color)
            draw->tint_color_ptr = (U8*)def->color;

        if (tracker && !tracker->invisible && !(tracker->tricks && (tracker->tricks->flags1 & TRICK_HIDE)))
            handleDefTracker(draw,def,tracker,vis);
    }

    if (def->has_alpha)
    {
        if (draw->alpha)
            draw->alpha *= def->groupdef_alpha;
        else
            draw->alpha = def->groupdef_alpha;
    }

    draw->do_welding = 1;

    if (def->model)
    {
        Model *model = def->model;
        if (!group_draw.nosway && def->model->trick && def->model->trick->flags2 & TRICK2_HAS_SWAY)
            swayThisMat( parent_mat, def->model, uid ); //A little flaky: changes parent_mat, thus everything this is grouped with drawn after it

        // drawDefModel
        if (!ViewCache_IsCaching())
        {
            // AutoLOD
            if (def->auto_lod_models && model->lod_info && !game_state.edit_base)
            {
                int numLODs = eaSize(&def->auto_lod_models);
                int lod_override;

#ifndef FINAL
                // There may be a model lod override on the DefTracker if the "Edit LOD" window is open
                // and set to show a particular LOD. In this case we just want to force show the given
                // LOD (i.e.. it should just have lod alpha of 255 and not test against distances, etc.
                // This takes precedence over any viewport wide lod_model_override
                if (tracker && tracker->lod_override)
                {
                    // tracker->lod_override is 0 for 'no override' or currently selected panel LOD#+1
                    lod_override = tracker->lod_override;
                }
                else if (g_client_debug_state.lod_model_override)
                {
                    // gfx HUD is overriding the model lod explicitly
                    // we still use the group visibility distances to fade in/out as a whole
                    lod_override = g_client_debug_state.lod_model_override;
                }
                else
#endif
                {
                    // viewport wide lod override. 0 for 'no override' or MAXLOD for lowest lod
                    lod_override = gfx_state.lod_model_override;
                }

                if (lod_override > 0)
                {
                    int lod_index;

                    // It has been requested that we draw with a specific LOD
                    // Generally this is done for the "Edit LOD" window or to improve performance, e.g., drawing with low LODs in reflection viewports.
                    // Notify drawModel() that this is happening with the lod_override argument so it can adjust lod fade calculations accordingly

                    // clamp the override value to the maximum available lods
                    if (lod_override > numLODs)
                        lod_override = numLODs;

                    lod_index = lod_override - 1;    // lod_override is offset + 1 from real lod index
                    drawModel_Shadowmap(def->auto_lod_models[lod_index], def->auto_lod_models[numLODs-1], model->lod_info->lods[lod_index], def, parent_mat, mid, tracker, draw, d, vis, uid, 1, lod_index, 1, 0, def->hideForHeadshot);
                }
                else
                {
                    int i, checked_occluder = 0;

                    for (i = 0; i < numLODs; i++)
                    {
                        drawModel_Shadowmap(def->auto_lod_models[i], def->auto_lod_models[numLODs-1], model->lod_info->lods[i], def, parent_mat, mid, tracker, draw, d, vis, uid, 0, i, 0, 0, def->hideForHeadshot);
                    }
                }
            }
            else
            {
                drawModel_Shadowmap(model, 0, 0, def, parent_mat, mid, tracker, draw, d, vis, uid, 1, 0, 0, 0, def->hideForHeadshot);
            }
        }
    }

    if (ViewCache_IsCaching())
    {
        //assert(def == groupEnt->def);

        view_cache_index = ViewCache_Add(draw, d, mid, tracker, vis, parent_mat, uid, groupEnt);
        draw->view_cache_parent_index = view_cache_index;
    }

    // MW what? Why is this after the draw for one thing? // JE: Because this is a flag telling all of it's children to use it's midpoint for LOD calculations
    if (def->lod_fadenode)
    {
        copyVec3(mid,draw->node_mid);
        draw->node_radius = def->radius;
        draw->lod_fadenode = def->lod_fadenode;
    }

    if (def->count)
    {
        drawChildren_shadowmap(draw,def,d,vis,parent_mat,tracker,uid,useWelded);

        if (ViewCache_IsCaching())
            ViewCache_UpdateChildCount(view_cache_index);
    }

}

int drawDefInternal_shadowmap( GroupEnt *groupEnt, Mat4 parent_mat_p, DefTracker *tracker, VisType vis, DrawParams *draw, int uid )
{
    Vec3        mid;
    F32            draw_scale;
    DrawParams    local;
    Mat4        parent_mat;
    F32            d;
    int            clip;
    GroupDef    *def;
    Mat4Ptr        local_mat;

    g_defs_processed++;

    if (groupEnt->flags_cache & ONLY_EDITORVISIBLE)
    {
        g_hidden_def_discards++;
        return 0;
    }
#ifndef FINAL    // debug model filtering
    if (tracker) 
    {
        if (tracker->tricks && (tracker->tricks->flags1 & TRICK_HIDE))
        {
            g_hidden_def_discards++;
            return 0;
        }
        if (tracker->invisible)
        {
            g_hidden_def_discards++;
            return 0;
        }
    }
    if (groupEnt->def->model && !strstri(groupEnt->def->model->name, g_client_debug_state.modelFilter))
        return 0;
#endif

    CALL_INIT
    
    // calculate the midpoint of the group bound sphere in the
    // parent's coordinate frame (usually this ends up being view space)
    mulVecMat4(groupEnt->mid_cache,parent_mat_p,mid);

    // do a sphere visibility test and test if group is close enough
    // to show up given its lod/visibility properties and current lod_scale
    // (uses cached GroupEnt vis quantities)
    if (!(d=closeEnough(draw,mid,groupEnt)))
        return 0;

    // test against cascade view frustum slice
    if (game_state.shadowDebugFlags & kShadowCapsuleCull)
    {
        if ( !isShadowExtrusionVisible( mid, groupEnt->radius ) )
        {
            return 0;
        }
    }
    else
    {
        // we setup the gfx_window so it has the view frust slice for the cascade
        // so this tests with that near/far z values of the slice
        clip = gfxSphereVisible(mid,groupEnt->radius);
        if (!clip)
            return 0;
    }

    // common rejections are above, try to avoid loading def until here because of memory stalls
    def = groupEnt->def; 
    local_mat    = groupEnt->mat;

    // @todo can we skip this check for shadow maps and instead bail earlier?
    if (def->pre_alloc)
        groupFileLoadFromName(def->name);

    // If this group is beyond the fog, and noFogClip is not set, and ClipFoggedGeometry is set, discard it
    // don't fog clip if a parent or child has noFogClip set
    CALL_INC
    if (group_draw.fog_clip) {
        F32 fog_far_dist_plus_radius;
        if (draw->lod_fadenode && (def->parent_fade || draw->lod_fadenode==2)) {
            fog_far_dist_plus_radius = group_draw.fog_far_dist + draw->node_radius;
        } else {
            fog_far_dist_plus_radius = group_draw.fog_far_dist + def->radius;
        }
        if (vis != VIS_DRAWALL && !(draw->no_fog_clip|def->no_fog_clip|def->child_no_fog_clip) && d > SQR(fog_far_dist_plus_radius + 100)) // the +100 is for the fade out distance, so fog clipped objects don't pop
            return 0;
    }

    CALL_INC
    draw_scale = draw->view_scale * def->lod_scale;
    // reject near lod's
    if (d < SQR((def->lod_near-def->lod_nearfade) * draw_scale * game_state.lod_scale))
        return 0;
    
    CALL_INC
    local = *draw;
    draw = &local;
    draw->view_scale = draw_scale;
    draw->no_fog_clip |= def->no_fog_clip;
    draw->no_planar_reflections |= def->disable_planar_reflections;

    CALL_INC
    if (def->is_tray)
    {
        if (vis == VIS_NORMAL)
        {
            // this is a tray, but we are not supposed to be drawing a tray.
            if (tracker)
            {
                if (def->outside)
                    vistrayAddOutsideTray(tracker);
                if (uid >= 0)
                    vistraySetUid(tracker, uid);
            }
            return 0;
        }

        // we are drawing in the editor, so draw this with the same lighting as it would be normally
        draw->light_trkr = 0;
    }

    //matrix Multiply usually deferred to here
    if( groupEnt->flags_cache & CACHEING_IS_DONE ) //Maybe just always do this
        mulMat4Inline(parent_mat_p,local_mat,parent_mat); 

    // here's where the main rendering code would either do z-occlusion test or test
    // def AABB against the view frustum.
    // @todo
    // For the time being we are going to keep the vis test based on the sphere as more
    // things will then render that 'cast into' the viewport until we start testing
    // swept spheres/AABB
//    if (clip != CLIP_NONE && !checkBoxFrustum(clip, def->min, def->max, parent_mat))
//        return 0;


    CALL_INC

    drawDef_shadowmap(draw,d,mid,def,tracker,vis,parent_mat,uid,groupEnt);

    return 1;
}
