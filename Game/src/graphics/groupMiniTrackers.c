#include "graphics/groupMiniTrackers.h"
#include "graphics/groupdrawutil.h"
#include "seq/anim.h"
#include "render/rendermodel.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/MemoryPool.h>
#include "render/tex.h"
#include "seq/AutoLOD.h"
#include "render/thread/rt_model.h"
#include "render/thread/rt_init.h"
#include "render/model.h"
#include <utilitieslib/utils/error.h>
#include "seq/gfxtree.h"
#include <utilitieslib/utils/utils.h>

#define DO_CACHING 1

typedef struct GroupMiniTrackerState {
    StashTable stWorldMiniTrackers; // Never dynamically freed, looked up by uid
    StashTable stWorldWeldedMiniTrackers; // Never dynamically freed, looked up by uid
    StashTable stTexBindCache; // Cache of most recent texture swap results so that multiple Defs with the same swap share the same TexBind
    StashTable stModelCache; // Cache of most recent enter MiniTracker for Defs with the same model and same swap
    MemoryPool mpMTTexBinds;
    MiniTracker **eaWorldMiniTrackersToFree; // List of world minitrackers to free (there may be duplicates by uid in stWorldMiniTrackers)
    int    initialStashtableSize;
    bool free_individual_minitrackers;
} GroupMiniTrackerState;

StashTable stNodeMiniTrackers; // May be dynamically freed

static GroupMiniTrackerState *gmts_stack = 0;
static int gmts_stack_count, gmts_stack_max;
static GroupMiniTrackerState *gmts=NULL;
static int dbg_different_count, dbg_same_count;
static int dbg_cache_texbind_hit, dbg_cache_texbind_miss, dbg_cache_texbind_hit_any;
static int dbg_cache_model_hit, dbg_cache_model_miss, dbg_cache_model_hit_any;
MP_DEFINE(MiniTracker);

MiniTracker *groupDrawGetMiniTracker(int uid, bool is_welded)
{
    MiniTracker *ret;
    // If we're spending any significant time in here, we should
    //  make this have a bitfield saying whether or not any given UID has a
    //  MiniTracker
    if (is_welded) {
        if (stashIntFindPointer(gmts->stWorldWeldedMiniTrackers, uid+1, &ret))
            return ret;
    } else {
        if (stashIntFindPointer(gmts->stWorldMiniTrackers, uid+1, &ret))
            return ret;
    }
    return NULL;
}

static MiniTracker *createMiniTracker(int uid, bool is_welded, MiniTracker *cached_mini_tracker)
{
    MiniTracker *ret = cached_mini_tracker;
    bool was_dup=false;

    assert(uid != -1);
    if (!ret) {
        ret = MP_ALLOC(MiniTracker);
        eaPush(&gmts->eaWorldMiniTrackersToFree, ret);
    }
    if (is_welded)
        was_dup = !stashIntAddPointer(gmts->stWorldWeldedMiniTrackers, uid+1, ret, false);
    else
        was_dup = !stashIntAddPointer(gmts->stWorldMiniTrackers, uid+1, ret, false);
    if (was_dup) {
        Errorf("UIDs gone bad!");
    }
    return ret;
}

static void initMiniTrackers(void)
{
    MP_CREATE(MiniTracker, 256);
    if (!gmts) {
        assert(gmts_stack==0 && gmts_stack_count==0);
        gmts = dynArrayAddStruct(&gmts_stack, &gmts_stack_count, &gmts_stack_max);
        gmts->initialStashtableSize = 8192;
    }
    assert(gmts->initialStashtableSize);
    if (!gmts->stWorldMiniTrackers) {
        gmts->stWorldMiniTrackers = stashTableCreateInt(gmts->initialStashtableSize);
    }
    if (!gmts->stWorldWeldedMiniTrackers) {
        gmts->stWorldWeldedMiniTrackers = stashTableCreateInt(gmts->initialStashtableSize);
    }
    if (!gmts->stTexBindCache) {
        gmts->stTexBindCache = stashTableCreateAddress(128);
    }
    if (!gmts->stModelCache) {
        gmts->stModelCache = stashTableCreateAddress(128);
    }
    if (!stNodeMiniTrackers) {
        stNodeMiniTrackers = stashTableCreateAddress(MAX(gmts->initialStashtableSize/64, 16));
    }
    if (!gmts->mpMTTexBinds) {
        gmts->mpMTTexBinds = createMemoryPoolNamed("MiniTrackerTexBinds", __FILE__, __LINE__);
        initMemoryPool(gmts->mpMTTexBinds, sizeof(TexBind), 256);
        mpSetMode(gmts->mpMTTexBinds, ZERO_MEMORY_BIT);
    }
}

void buildAndPrintMiniTracker(Model *model)
{
    int i, j;
    BlendModeType first_blend_mode;

    for (i = 0; i < model->tex_count; i++)
    {
        BasicTexture *actualTextures[TEXLAYER_MAX_LAYERS] = {0};
        RdrTexList rtexlist = {0};
        TexBind *basebind;
        bool bDifferentFromBaseBind=false;

        printf("%02d: %s: ", i, model->tex_binds[i]->name);
        basebind = modelGetFinalTextures(actualTextures, model, 0, 1, 0, i, &rtexlist, &first_blend_mode, true, 0);
        // If different (than basebind) allocate a new TexBind, otherwise, store pointer to existing TexBind
        if (basebind->bind_blend_mode.intval != rtexlist.blend_mode.intval) {
            printf("BlendMode different:%s->%s ", blend_mode_names[basebind->bind_blend_mode.shader], blend_mode_names[rtexlist.blend_mode.shader]);
        } else {
            printf("BlendMode %s ", blend_mode_names[basebind->bind_blend_mode.shader]);
        }
        printf("\n");
        for (j=0; j<TEXLAYER_MAX_LAYERS; j++) {
            if (actualTextures[j] && basebind->tex_layers[j]) {
                printf("  %s->%s\n", basebind->tex_layers[j]->name, actualTextures[j]->name);
            }
        }
        printf("  Alphasort: %d\n", !!texNeedsAlphaSort(basebind, rtexlist.blend_mode));
    }
}

static bool modelHasLODWithFallbackTexture(Model *model)
{
    int i;
    if (!model->lod_info)
        return false;
    for (i=eaSize(&model->lod_info->lods)-1; i>=0; i--) {
        if (model->lod_info->lods[i]->flags & LOD_USEFALLBACKMATERIAL)
            return true;
    }
    return false;
}

static void groupDrawBuildMiniTracker(Model *basemodel, GroupDef *def, TraverserDrawParams *draw, Mat4 mat, DefTracker *pParent, int uid, bool is_welded)
{
    int i, j, m;
    bool bNeedsMiniTracker;
    bool bDifferentTexBinds=false;
    MiniTracker *mini_tracker=NULL;
    BlendModeType first_blend_mode;
    bool bCachedMiniTracker=false;

    EArray eabuf = { 1, 1, EARRAY_FLAG_CUSTOMALLOC };
    Model **models = (Model**)eabuf.structptrs;
    void *key = basemodel ? (void*)basemodel : (void*)def;

    assert(!!basemodel != !!def); // (basemodel ^^ def) where's logical xor when you need it?
    if(DO_CACHING && stashFindPointer(gmts->stModelCache, key, &mini_tracker))
        dbg_cache_model_hit_any++;

    if(basemodel)
        eabuf.structptrs[0] = basemodel;
    else if(eaSize(&def->auto_lod_models))
        models = def->auto_lod_models;
    else
        eabuf.structptrs[0] = def->model;

    // Determine if any of the textures require a minitracker
    for(m = 0; m < eaSize(&models) && (!bDifferentTexBinds || mini_tracker); m++)
    {
        Model *model = models[m];
        TexBind **tracker_binds = mini_tracker ? mini_tracker->tracker_binds[m] : NULL;
        for(i = 0; i < model->tex_count && (!bDifferentTexBinds || mini_tracker); i++)
        {
            BasicTexture *actualTextures[TEXLAYER_MAX_LAYERS] = {0};
            RdrTexList rtexlist = {0};
            TexBind *basebind;

            basebind = modelGetFinalTextures(actualTextures, model,
                (draw->tex_binds.base||draw->tex_binds.generic) ? 
                &draw->tex_binds:0, 1, draw->swapIndex, i, &rtexlist,
                &first_blend_mode, true, pParent);

            if(basebind != model->tex_binds[i] || basebind->bind_blend_mode.intval != rtexlist.blend_mode.intval)
            {
                // Definitely not the default
                bDifferentTexBinds = true;
                if(tracker_binds)
                {
                    if (tracker_binds[i] == basebind)
                    {
                        // Definitely good
                    }
                    else
                    {
                        // Possibly bad
                        for (j = 0; j < TEXLAYER_MAX_LAYERS; j++)
                        {
                            if (actualTextures[j] != tracker_binds[i]->tex_layers[j])
                            {
                                mini_tracker = NULL; // Bad!
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                // Check for swap changes
                for(j = 0; j < TEXLAYER_MAX_LAYERS; j++)
                {
                    if(actualTextures[j] != model->tex_binds[i]->tex_layers[j])
                        bDifferentTexBinds = true;
                    if(tracker_binds && tracker_binds[i]->tex_layers[j] != actualTextures[j])
                        mini_tracker = NULL;
                }
            }
        }
    }

#ifdef DEBUG_UIDS_GOING_BAD
    bDifferentTexBinds = true;
#endif

    bNeedsMiniTracker = bDifferentTexBinds;

    if(bNeedsMiniTracker)
    {
        if(mini_tracker)
        {
            dbg_cache_model_hit++;
            bCachedMiniTracker=true;
        }
        else
        {
            dbg_cache_model_miss++;
        }
        mini_tracker = createMiniTracker(uid, is_welded, mini_tracker);
    }

    if(bDifferentTexBinds && !bCachedMiniTracker)
    {
        // Make new binds list
        TexBind **last_binds = NULL;
        for(m = 0; m < eaSize(&models); m++)
        {
            TexBind **tracker_binds = NULL;
            Model *model = models[m];
            for(i = 0; i < model->tex_count; i++)
            {
                BasicTexture *actualTextures[TEXLAYER_MAX_LAYERS] = {0};
                RdrTexList rtexlist = {0};
                TexBind *basebind, *bind;
                bool bDifferentFromBaseBind=false;

                basebind = modelGetFinalTextures(actualTextures, model, 
                    (draw->tex_binds.base||draw->tex_binds.generic) ? 
                    &draw->tex_binds:0, 1, draw->swapIndex, i, &rtexlist,
                    &first_blend_mode, true, pParent);
                // If different (than basebind) allocate a new TexBind, otherwise, store pointer to existing TexBind
                if(basebind->bind_blend_mode.intval != rtexlist.blend_mode.intval)
                {
                    bDifferentFromBaseBind = true;
                }
                else
                {
                    for(j=0; j<TEXLAYER_MAX_LAYERS; j++)
                        if (actualTextures[j] != basebind->tex_layers[j])
                            bDifferentFromBaseBind = true;
                }

                if(bDifferentFromBaseBind)
                {
                    bind = NULL;
                    if(DO_CACHING && stashFindPointer(gmts->stTexBindCache, basebind, &bind))
                    {
                        dbg_cache_texbind_hit_any++;
                        for(j=0; j<TEXLAYER_MAX_LAYERS; j++)
                        {
                            if(bind->tex_layers[j] != actualTextures[j])
                            {
                                bind = NULL;
                                break;
                            }
                        }
                    }

                    if(bind)
                    {
                        dbg_cache_texbind_hit++;
                    }
                    else
                    {
                        dbg_cache_texbind_miss++;
                        bind = mpAlloc(gmts_stack->mpMTTexBinds);
                        *bind = *basebind;
                        // don't mark allocated_byMiniTracker, we will free them all at once when we pop or reset, also it doesn't work with the caching
                        bind->bind_blend_mode = rtexlist.blend_mode;
                        bind->dirname = "<MiniTrackerSwapped>";
                        bind->tex_lod = NULL;
                        bind->scrollsScales = rtexlist.scrollsScales;
                        for(j=0; j<TEXLAYER_MAX_LAYERS; j++)
                            bind->tex_layers[j] = actualTextures[j];
                        bind->needs_alphasort = !!texNeedsAlphaSort(bind, bind->bind_blend_mode);
                        // Make tex_lod for lods with fallback textures
                        if(modelHasLODWithFallbackTexture(model))
                        {
                            texCreateLOD(bind);
                            basebind = modelGetFinalTextures(actualTextures, model, 
                                (draw->tex_binds.base||draw->tex_binds.generic)?
                                &draw->tex_binds:0, 1, draw->swapIndex, i, 
                                &rtexlist, &first_blend_mode, true, pParent);
                            //*bind->tex_lod = *basebind;  Do not copy over the basebind because it may have allocated scrollsScales.  If there ever is a swap which swaps the fallback's base, this will run into trouble!
                            for(j=0; j<TEXLAYER_MAX_LAYERS; j++)
                                bind->tex_lod->tex_layers[j] = actualTextures[j];
                        }
                        if(DO_CACHING)
                            stashAddPointer(gmts->stTexBindCache, basebind, bind, true);
                    }
                }
                else
                {
                    assert(basebind->needs_alphasort == !!texNeedsAlphaSort(basebind, basebind->bind_blend_mode));
                    bind = basebind;
                }

                if(!tracker_binds)
                {
                    if(!last_binds || eaGet(&last_binds, i) != bind) // base model or first different bind on an lod, we need a new list of binds
                    {
                        eaSetSize(&tracker_binds, model->tex_count);
                        for(j = i-1; j >= 0; --j) // if this is an lod, copy over the other matching binds
                            tracker_binds[j] = last_binds[j];
                    }
                }
                if(tracker_binds)
                    tracker_binds[i] = bind;
            }

            if(tracker_binds) // tracker_binds will be null if this is an lod and all it's binds match the next one up
                last_binds = mini_tracker->tracker_binds[m] = tracker_binds;
        }
    }

    if(DO_CACHING && mini_tracker)
        stashAddPointer(gmts->stModelCache, key, mini_tracker, true);

    if(bNeedsMiniTracker)
        dbg_different_count++;
    else
        dbg_same_count++;
}

static int groupDrawBuildMiniTrackersCallback(GroupDef *def, DefTracker *tracker, Mat4 world_mat, TraverserDrawParams *draw)
{
    int i;

    if (def->model) {
        groupDrawBuildMiniTracker(NULL, def, draw, world_mat, tracker?tracker->parent:0, draw->uid, false);
    }

    for (i=0; i<eaSize(&def->welded_models); i++) {
        groupDrawBuildMiniTracker(def->welded_models[i], NULL, draw, world_mat, tracker?tracker->parent:0, draw->uid + def->welded_models_duids[i], true);
    }

    return 1;
}

void gfxNodeBuildMiniTracker(GfxNode *node)
{
    int i, j;
    MiniTracker *mini_tracker;
    TexBind **tracker_binds;
    bool bPromote=false;
    bool bNeedAlphaSort = node && (node->flags & GFXNODE_ALPHASORT);
    BlendModeType first_blend_mode;

    initMiniTrackers();

    node->mini_tracker = NULL; // This was already freed when we freed the memory pool on reset, or did not exist on init
    if (!node->model || !(node->flags & GFXNODE_CUSTOMTEX) && !node->model->common_blend_mode.shader) {
        return;
    }

    mini_tracker = node->mini_tracker = MP_ALLOC(MiniTracker);
    assert(stashAddressAddPointer(stNodeMiniTrackers, mini_tracker, mini_tracker, false));

    if ((node->model->flags & OBJ_DRAWBONED) && (node->flags & GFXNODE_SEQUENCER)) {
        // Skinned needs at least BLENDMODE_COLORBLEND_DUAL
        // Additionally skinned objects can have a HQBUMP bit set dynamically
        bPromote = true;
    }

    if(node->model->tex_count)
        eaSetSize(&mini_tracker->tracker_binds[0], node->model->tex_count);
    tracker_binds = mini_tracker->tracker_binds[0];
    // Make new binds list
    for (i = 0; i < node->model->tex_count; i++)
    {
        BasicTexture *actualTextures[TEXLAYER_MAX_LAYERS] = {0};
        RdrTexList rtexlist = {0};
        TexBind *basebind;
        bool bDifferentFromBaseBind=false;

        basebind = modelGetFinalTextures(actualTextures, node->model,
            &node->customtex, 0, 0, i, &rtexlist, &first_blend_mode, true, 0);
        if (node->model->common_blend_mode.shader)
            rtexlist.blend_mode = promoteBlendMode(node->model->common_blend_mode, rtexlist.blend_mode);
        // If different (than basebind) allocate a new TexBind, otherwise, store pointer to existing TexBind
        if (basebind->bind_blend_mode.intval != rtexlist.blend_mode.intval) {
            bDifferentFromBaseBind = true;
        } else if (basebind->needs_alphasort != bNeedAlphaSort) {
            // Node doesn't want alpha sorting
            // This is only because of poorly set up textures
            bDifferentFromBaseBind = true;
        } else if (bPromote) {
            // Need to be able to poke in HQBUMP bit
            bDifferentFromBaseBind = true;
        } else {
            for (j=0; j<TEXLAYER_MAX_LAYERS; j++) {
                if (actualTextures[j] != basebind->tex_layers[j]) {
                    bDifferentFromBaseBind = true;
                }
            }
        }

        if (bDifferentFromBaseBind) {
            TexBind *bind;
            bind = tracker_binds[i] = mpAlloc(gmts_stack[0].mpMTTexBinds); // always at the front of the 'stack'
            *bind = *basebind;
            bind->allocated_byMiniTracker = 1;
            bind->bind_blend_mode = rtexlist.blend_mode;
            if (bPromote) {
                if (bind->bind_blend_mode.shader==BLENDMODE_MULTI)
                {
                    if (rdr_caps.features & GFXF_MULTITEX_DUAL) {
                        bind->bind_blend_mode = BlendMode(BLENDMODE_MULTI,bind->bind_blend_mode.blend_bits);
                    } else {
                        bind->bind_blend_mode = BlendMode(BLENDMODE_MULTI,bind->bind_blend_mode.blend_bits|BMB_SINGLE_MATERIAL);
                    }
                } else if (blendModeHasBump(bind->bind_blend_mode)) {
                    bind->bind_blend_mode = BlendMode(BLENDMODE_BUMPMAP_COLORBLEND_DUAL,0);
                } else {
                    bind->bind_blend_mode = BlendMode(BLENDMODE_COLORBLEND_DUAL,0);
                }
            }
            bind->dirname = "<MiniTrackerSwappedNode>";
            bind->tex_lod = NULL;
            bind->scrollsScales = rtexlist.scrollsScales;
            for (j=0; j<TEXLAYER_MAX_LAYERS; j++) {
                bind->tex_layers[j] = actualTextures[j];
            }
            bind->needs_alphasort = bNeedAlphaSort;
        } else {
            tracker_binds[i] = basebind;
            assert(basebind->needs_alphasort == !!texNeedsAlphaSort(basebind, basebind->bind_blend_mode));
        }
    }
}

void gfxNodeFreeMiniTracker(GfxNode *node)
{
    int i;
    TexBind **binds;

    if(!node->mini_tracker)
        return;

    assert(stashAddressRemovePointer(stNodeMiniTrackers, node->mini_tracker, NULL));
    binds = node->mini_tracker->tracker_binds[0];
    for(i = eaSize(&binds)-1; i >= 0; --i)
        if(binds[i]->allocated_byMiniTracker)
            mpFree(gmts_stack[0].mpMTTexBinds, binds[i]); // shouldn't have any allocated tex lods, always the front of the 'stack'
    eaDestroy(&binds); // left behind on mini_tracker, but we're about to free it

    for(i = 1; i < MAX_LODS; i++)
        assertmsg(!node->mini_tracker->tracker_binds[i], "LOD MiniTracker binds on a non-world tracker");

    MP_FREE(MiniTracker, node->mini_tracker);
    node->mini_tracker = NULL;
}


void gfxNodeBuildMiniTrackerRecurse(GfxNode *gfxnode)
{
    GfxNode *   node;
    for( node = gfxnode ; node ; node = node->next )
    {
        gfxNodeBuildMiniTracker(node);
        if( node->child )
            gfxNodeBuildMiniTrackerRecurse( node->child );        
    }
}

int g_worlduids_last = 0;
int g_worlduids_reset = 0;

static void clearTexBind(MemoryPool pool, TexBind *tracker_bind, void *userData)
{
    texFreeLOD(tracker_bind);
}

static void clearMiniTracker(MiniTracker *mini_tracker)
{
    int i;
    for(i = 0; i < MAX_LODS; i++)
        eaDestroy(&mini_tracker->tracker_binds[i]);
}

static void destroyMiniTracker(MiniTracker *mini_tracker)
{
    clearMiniTracker(mini_tracker);
    MP_FREE(MiniTracker, mini_tracker);
}

static void clearMiniTrackerAdapter(void* arg0)
{
    clearMiniTracker((MiniTracker *)arg0);
}

static void destroyMiniTrackerAdapter(void* arg0)
{
    destroyMiniTracker((MiniTracker *)arg0);
}

static void clearStackFrame(void)
{
    // clear allocated texbinds and free quickly
    mpForEachAllocation(gmts->mpMTTexBinds, clearTexBind, NULL);
    mpFreeAll(gmts->mpMTTexBinds);

    // clear mini trackers
    if(gmts == &gmts_stack[0]) // front of the 'stack', clear everything
    {
        // clear MiniTrackers and free quickly
        eaClearEx(&gmts->eaWorldMiniTrackersToFree, clearMiniTrackerAdapter);
        stashTableClearEx(stNodeMiniTrackers, NULL, clearMiniTrackerAdapter);
        mpFreeAll(MP_NAME(MiniTracker)); // Note this memory pool will not normally get compacted
    }
    else
    {
        // destroy MiniTrackers individually
        eaClearEx(&gmts->eaWorldMiniTrackersToFree, destroyMiniTrackerAdapter);
    }

    // clear indexes/caches
    stashTableClear(gmts->stWorldMiniTrackers);
    stashTableClear(gmts->stWorldWeldedMiniTrackers);
    stashTableClear(gmts->stTexBindCache);
    stashTableClear(gmts->stModelCache);
}

void groupDrawBuildMiniTrackers(void)
{
    TraverserDrawParams draw={0};

    writeConsole(OUTPUT_DEBUG, "Applying texture swaps");

    // Free old TexBinds and MiniTrackers
    if (gmts) {
        assert(gmts_stack_count == 1);
        assert(gmts == &gmts_stack[0]);
        clearStackFrame();
    }

    // Initialize stuff
    initMiniTrackers();

    dbg_cache_texbind_hit = dbg_cache_texbind_miss = dbg_cache_texbind_hit_any = 0;
    dbg_cache_model_hit = dbg_cache_model_miss = dbg_cache_model_hit_any = 0;
    dbg_different_count = dbg_same_count = 0;
    draw.callback = groupDrawBuildMiniTrackersCallback;
    draw.need_texAndTintCrc = false;
    draw.need_matricies = false;
    draw.need_texswaps = true;
    groupTreeTraverseEx(NULL, &draw);

    g_worlduids_last = draw.uid;
    g_worlduids_reset = 1;

    gfxNodeBuildMiniTrackerRecurse(gfx_tree_root);

    writeConsole(OUTPUT_INFO, "Applied %d different, %d same texture swaps", dbg_different_count, dbg_same_count);
}

void groupDrawPushMiniTrackers(void)
{
    // This isn't quite a stack, just a per-frame list...
    gmts = dynArrayAddStruct(&gmts_stack, &gmts_stack_count, &gmts_stack_max);
    // Dynamic stuff wants a much smaller table!
    gmts->initialStashtableSize = 8;
    gmts->free_individual_minitrackers = true;
    initMiniTrackers();
}

void groupDrawPopMiniTrackers(void) // Frees whatever was popped
{
    // Just set index back to 0

    // Free minitrackers and stashtables later
    gmts = &gmts_stack[0];
}

void groupDrawBuildMiniTrackersForDef(GroupDef *def, int uid)
{
    TraverserDrawParams draw={0};

    // fxGeoUpdateWorldGroup() uses this.
    // assertmsg(gmts != &gmts_stack[0], "Using temporary minitrackers on the front of the stack");

    dbg_different_count = dbg_same_count = 0;
    draw.callback = groupDrawBuildMiniTrackersCallback;
    draw.need_texAndTintCrc = false;
    draw.need_matricies = false;
    draw.need_texswaps = true;
    draw.uid = uid;
    groupTreeTraverseDefEx(NULL, &draw, def, NULL);
}

void groupDrawMinitrackersFrameCleanup(void)
{
    int i;
    if (!gmts_stack_count)
        return;

    for (i=1; i<gmts_stack_count; i++) {
        gmts = &gmts_stack[i];
        clearStackFrame();

        // we need to free the stash tables since frames are
        //        alloc'ed using dynArrayAddStruct which clears 
        //        the elements of the struct without freeing
        if (gmts->stModelCache)
        {
            stashTableDestroy(gmts->stModelCache);
            gmts->stModelCache = NULL;
        }
        if (gmts->stTexBindCache)
        {
            stashTableDestroy(gmts->stTexBindCache);
            gmts->stTexBindCache = NULL;
        }
        if (gmts->stWorldMiniTrackers)
        {
            stashTableDestroy(gmts->stWorldMiniTrackers);
            gmts->stWorldMiniTrackers = NULL;
        }
        if (gmts->stWorldWeldedMiniTrackers)
        {
            stashTableDestroy(gmts->stWorldWeldedMiniTrackers);
            gmts->stWorldWeldedMiniTrackers = NULL;
        }
    }
    gmts = &gmts_stack[0];
    gmts_stack_count = 1;
}
