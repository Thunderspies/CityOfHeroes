#include <stdio.h>
#include <string.h>
#include "stdlib.h"

#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/fpmacros.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/components/Frustum.h"
#include "utilitieslib/components/Octree.h"

#define OCT_NUMCELLS            2
#define OCT_NUMCELLBITS            1  // log2i(OCT_NUMCELLS)
#define OCT_CELLSHRINK            (1.f / (float)OCT_NUMCELLS)
#define OCT_NUMCELLS_SQUARE        (OCT_NUMCELLS * OCT_NUMCELLS)
#define OCT_NUMCELLS_CUBE        (OCT_NUMCELLS * OCT_NUMCELLS * OCT_NUMCELLS)
#define OCT_SETGRIDSIZE(g,s)    (g->size = (int)s, g->inv_size = (int)(1.f/s), g->num_bits = (int)log2i((int)s))
#define OCT_BIGGEST_BRICK        32768

typedef void *OctreeNodePtr;

typedef struct OctreeEntries
{
    OctreeNodePtr            entries[7];
    struct OctreeEntries    *next;
} OctreeEntries;

typedef struct OctreeIdxList
{
    OctreeNodePtr            *entries[7];
    struct OctreeIdxList    *next;
} OctreeIdxList;

typedef struct OctreeCell
{
    struct OctreeCell        **children;
    OctreeEntries            *entries;
    U8                        filled;
} OctreeCell;

typedef struct
{
    OctreeNodePtr            node[2];
    U32                        tag[2];
} TagEnt;

typedef struct Octree
{
    OctreeCell                *cell;
    Vec3                    pos;
    U32                        *tags;
    size_t                    tag_max;
    int                        rndo;
    F32                        size;
    F32                        inv_size;
    int                        tag;
    int                        num_bits;
    U32                        valid_id;
    int                        mempool_size;
    MemoryPool                entries_mempool;
    MemoryPool                childptrs_mempool;
    MemoryPool                idxlist_mempool;
    MemoryPool                cells_mempool;
    TagEnt                    tag_cache[128];
} Octree;

typedef struct
{
    Octree                    *octree;
    F32                        fit_size;
    Vec3                    min;
    OctreeNodePtr            node;
    Vec3                    size;
    OctreeIdxList            *idx_list;
} OctreeInsertState;


MP_DEFINE(Octree);

Octree *octreeCreateEx(int mempool_size, char *filename, int linenumber)
{
    Octree *octree;
    if (mempool_size <= 0)
        mempool_size = 4096;
    MP_CREATE(Octree, 16);
    octree = MP_ALLOC(Octree);
    octree->mempool_size = mempool_size;

    octree->entries_mempool = createMemoryPoolNamed("OctreeEntries", filename, linenumber);
    initMemoryPool(octree->entries_mempool,sizeof(OctreeEntries),octree->mempool_size);

    octree->childptrs_mempool = createMemoryPoolNamed("OctreeChildPtrs", filename, linenumber);
    initMemoryPool(octree->childptrs_mempool,OCT_NUMCELLS_CUBE * sizeof(OctreeCell *),octree->mempool_size/2);

    octree->cells_mempool = createMemoryPoolNamed("OctreeCell", filename, linenumber);
    initMemoryPool(octree->cells_mempool,sizeof(OctreeCell),octree->mempool_size);

    octree->idxlist_mempool = createMemoryPoolNamed("OctreeIdxList", filename, linenumber);
    initMemoryPool(octree->idxlist_mempool,sizeof(OctreeIdxList),octree->mempool_size);

    return octree;
}

void octreeDestroy(Octree *octree)
{
    if (!octree)
        return;
    octreeRemoveAll(octree);
    MP_FREE(Octree, octree);
}

static INLINEDBG OctreeEntries *octreeAllocEntries(Octree *octree)
{
    return mpAlloc(octree->entries_mempool);
}

static INLINEDBG OctreeCell **octreeAllocChildPtrs(Octree *octree)
{
    return mpAlloc(octree->childptrs_mempool);
}

static INLINEDBG OctreeCell *octreeAllocCell(Octree *octree)
{
    return mpAlloc(octree->cells_mempool);
}

static INLINEDBG OctreeIdxList *octreeAllocIdxList(Octree *octree)
{
    return mpAlloc(octree->idxlist_mempool);
}

void octreeRemove(Octree *octree,OctreeIdxList **entry)
{
    OctreeIdxList    *list,*next;
    int            i;

    if (!(*entry))
        return;
    for(list = *entry;list;list=list=next)
    {
        for(i=0;i<ARRAY_SIZE(list->entries);i++)
        {
            if (list->entries[i])
                *list->entries[i] = 0;
        }
        next = list->next;
        memset(list,0,sizeof(*list));
        mpFree(octree->idxlist_mempool,list);
    }
    *entry = 0;
}

static void freeEntries(Octree *octree,OctreeEntries *ents)
{
    OctreeEntries *next;

    for(;ents;ents=next)
    {
        next = ents->next;
        mpFree(octree->entries_mempool,ents);
    }
}

static OctreeNodePtr *findEntry(OctreeEntries *ents)
{
    int        i;

    for(;ents;ents = ents->next)
    {
        for(i=0;i<ARRAY_SIZE(ents->entries);i++)
        {
            if (!ents->entries[i])
                return &ents->entries[i];
        }
    }
    return 0;
}

static OctreeNodePtr **findListIdx(OctreeIdxList *list)
{
    int        i;

    for(;list;list = list->next)
    {
        for(i=0;i<ARRAY_SIZE(list->entries);i++)
        {
            if (!list->entries[i])
                return &list->entries[i];
        }
    }
    return 0;
}

int octreeGrow(Octree *octree,const Vec3 min,const Vec3 max,const Vec3 size)
{
    int        i,grow;

    // TODO make this function do something?
    grow = 0;
    for(i=0;i<3;i++)
    {
        if (size[i] > octree->size)
            grow = 1;
        if (min[i] < octree->pos[i])
            grow = 1;
        if (max[i] > octree->pos[i] + octree->size)
            grow = 1;
    }
    if (grow)
        return 0;

    return 1;
}

static void octreeInsert(OctreeInsertState *state,OctreeCell *cell,Vec3 pos,F32 cell_size)
{
    int            i,x,y,z,filled=1;
    OctreeCell    **child_ptr;
    Vec3        ppos;
    int            idxa[3],idxb[3];//,tp[3];
    F32            cell_test = cell_size * 0.5,tf;

    for(i=0;i<3;i++)
    {
        tf = state->min[i] - pos[i];
        idxa[i] = 0;
        if (tf >= cell_test)
            idxa[i] = 1;

        idxb[i] = 0;
        if (tf + state->size[i] >= cell_test)
            idxb[i] = 1;

        if (tf > 0 || tf + state->size[i] < cell_size)
            filled = 0;
    }
    //subVec3(idxb,idxa,tp);
    //filled = tp[0] + tp[1] + tp[2];
    if (filled || cell_size <= state->fit_size)
    {
        OctreeNodePtr        *mptr,**mmptr;

        mptr = findEntry(cell->entries);
        if (!mptr)
        {
            OctreeEntries    *ents = octreeAllocEntries(state->octree);

            ents->next = cell->entries;
            cell->entries = ents;
            mptr = findEntry(cell->entries);
        }
        *mptr = state->node;
        if (state->idx_list)
        {
            mmptr = findListIdx(state->idx_list);
            if (!mmptr)
            {
                OctreeIdxList    *list = octreeAllocIdxList(state->octree);

                list->next = state->idx_list;
                state->idx_list = list;
                mmptr = findListIdx(state->idx_list);
            }
            *mmptr = mptr;
        }
        return;
    }
    else
    {
        cell_size *= 0.5f;
        if (!cell->children)
        {
            cell->children = octreeAllocChildPtrs(state->octree);
        }
        for(x=idxa[0];x<=idxb[0];x++)
        {
            for(y=idxa[1];y<=idxb[1];y++)
            {
                for(z=idxa[2];z<=idxb[2];z++)
                {
                    child_ptr = &cell->children[OCT_NUMCELLS_SQUARE * y + OCT_NUMCELLS * z + x];
                    if (!*child_ptr)
                    {
                        cell->filled |= 1 << (child_ptr - cell->children);
                        *child_ptr = octreeAllocCell(state->octree);
                    }
                    ppos[0] = pos[0] + x * cell_size;
                    ppos[1] = pos[1] + y * cell_size;
                    ppos[2] = pos[2] + z * cell_size;
                    octreeInsert(state,*child_ptr,ppos,cell_size);
                }
            }
        }
    }
}


int octreeAddBox(Octree *octree, void *node, const Vec3 min, const Vec3 max, OctreeIdxList **entry, OctreeGranularity granularity)
{
    Vec3            size;
    int                i;
    F32                fit_size;
    OctreeInsertState state;

    subVec3(max,min,size);
    for(i=0;i<3;i++)
        if (size[i] < 0.f)
            size[i] = 0.f;
    if (!octree->cell)
    {
        setVec3(octree->pos,-OCT_BIGGEST_BRICK*(OCT_NUMCELLS/2),-OCT_BIGGEST_BRICK*(OCT_NUMCELLS/2),-OCT_BIGGEST_BRICK*(OCT_NUMCELLS/2));
        octree->cell = octreeAllocCell(octree);
        OCT_SETGRIDSIZE(octree,OCT_BIGGEST_BRICK*OCT_NUMCELLS);
    }
    if (!octreeGrow(octree,min,max,size))
        return 0;

    fit_size = MAX(size[0],size[1]);
    fit_size = MAX(fit_size,size[2]);
    fit_size *= 0.5f;

    switch (granularity)
    {
        xcase OCT_FINE_GRANULARITY:
            fit_size = MAX(fit_size,4.f); // smallest block: 4
        xcase OCT_MEDIUM_GRANULARITY:
            fit_size = MAX(fit_size,16.f); // smallest block: 16
        xcase OCT_ROUGH_GRANULARITY:
            fit_size = MAX(fit_size,64.f); // smallest block: 64
        xcase OCT_VERY_ROUGH_GRANULARITY:
            fit_size = MAX(fit_size,128.f); // smallest block: 128
    }

    state.octree = octree;
    state.fit_size = fit_size;
    copyVec3(min,state.min);
    copyVec3(size,state.size);
    state.node = node;
    state.idx_list = 0;
    if (entry)
        state.idx_list = octreeAllocIdxList(octree);
    octreeInsert(&state,octree->cell,octree->pos,octree->size);
    if (entry)
        *entry = state.idx_list;

    return 1;
}

void octreeAddSphere(Octree *octree,void *node,const Vec3 point,F32 radius,OctreeIdxList **entry,OctreeGranularity granularity)
{
    Vec3    min,max,dv;

    setVec3(dv,radius,radius,radius);
    subVec3(point,dv,min);
    addVec3(point,dv,max);
    octreeAddBox(octree,node,min,max,entry,granularity);
}

static void octreeFreeCell(Octree *octree,OctreeCell *cell)
{
    int        i;

    if (!cell)
        return;
    if (!cell->children)
        return;
    for(i=0;i<OCT_NUMCELLS_CUBE;i++)
    {
        if (cell->children[i])
            octreeFreeCell(octree,cell->children[i]);
    }
    freeEntries(octree,cell->entries);
    cell->entries = 0;
}

void octreeRemoveAll(Octree *octree)
{
    U32 t;

    if (!octree)
        return;

    octreeFreeCell(octree,octree->cell);
    if (octree->entries_mempool)
        destroyMemoryPool(octree->entries_mempool);
    if (octree->childptrs_mempool)
        destroyMemoryPool(octree->childptrs_mempool);
    if (octree->cells_mempool)
        destroyMemoryPool(octree->cells_mempool);
    if (octree->idxlist_mempool)
        destroyMemoryPool(octree->idxlist_mempool);

    SAFE_FREE(octree->tags);

    t = octree->valid_id;
    ZeroStruct(octree);
    octree->valid_id = t+1;
}

//////////////////////////////////////////////////////////////////////////
// Find

#define XLO 0x55
#define YLO 0x0f
#define ZLO 0x33

typedef struct OctreeFindState
{
    const Frustum *frustum;
    Vec3    point;
    F32        radius;
    int        tag;
    OctreeFindCallback    callback;
    OctreeSphereVisCallback sphere_vis_callback;
    OctreeFrustumVisCallback frustum_vis_callback;
    void    *user_data;
    void    ***nodes;
    Octree    *octree;
} OctreeFindState;

static void processOctreeEntries(Octree *octree,OctreeCell *cell,OctreeFindState *state)
{
    int i;
    U32 tag;
    size_t t;
    OctreeNodePtr node;
    OctreeEntries *ents;

    tag = state->tag;
    for(ents=cell->entries;ents;ents=ents->next)
    {
        for(i=0;i<ARRAY_SIZE(ents->entries);i++)
        {
            node = (void *)ents->entries[i];
            if (!node)
                continue;

            t = ((size_t)node) >> 1;
            if (t < 100000)
            {
                if (t >= octree->tag_max)
                {
                    assert(t < 100000);
                    octree->tag_max = 2*t;
                    octree->tags = realloc(octree->tags,octree->tag_max * sizeof(*octree->tags));
                }
                if (octree->tags[t] == tag)
                    continue;
                octree->tags[t] = tag;
            }
            else
            {
                TagEnt        *tagp;
                tagp = &octree->tag_cache[( (((size_t)node) >> 3) & (ARRAY_SIZE(octree->tag_cache)-1) )];
                if ( (tagp->node[0] == node && tagp->tag[0] == tag)
                    || (tagp->node[1] == node && tagp->tag[1] == tag) )
                    continue;
                octree->rndo = (~octree->rndo) & 1;
                tagp->node[octree->rndo] = node;
                tagp->tag[octree->rndo] = tag;
            }

            if (state->radius && state->sphere_vis_callback && !state->sphere_vis_callback(node, state->point, state->radius))
                continue;
            
            if (state->frustum && state->frustum_vis_callback && !state->frustum_vis_callback(node, state->frustum))
                continue;

            if (state->callback)
                state->callback(node, state->user_data);
            else
                eaPush(state->nodes, node);
        }
    }
}

static int octreeSplitPlanes(Octree *octree,OctreeCell *cell,Vec3 ppos,F32 size,OctreeFindState *state)
{
    Vec3 pos;
    F32 t,r,*point;
    int i;
    U8 mask, bit;
    static U8 masks[3] = {XLO,YLO,ZLO};

    if (!cell)
        return 0;

    r = state->radius;
    t = r + size;
    point = state->point;

    for(i=0;i<3;i++)
    {
        if (point[i] < ppos[i] - t)
            return 0;
        if (point[i] > ppos[i] + t)
            return 0;
    }

    if (cell->entries)
        processOctreeEntries(octree,cell,state);

    if (!cell->children)
        return 0;

    mask = 0xff;
    size *= 0.5;
    for(i=0;i<3;i++)
    {
        t = point[i] - ppos[i];

        if (!(t <= r)) // completely on neg side
            mask &= ~masks[i];

        if (!(t >= -r)) // completely on pos side
            mask &= masks[i];
    }
    if (!mask)
        return 0;
    bit = 1;
    for(i=0;i<8;i++,bit<<=1)
    {
        if (bit & mask)
        {
            copyVec3(ppos,pos);
            if (i & 1)
                pos[0] += size;
            else
                pos[0] -= size;

            if (i & 4)
                pos[1] += size;
            else
                pos[1] -= size;

            if (i & 2)
                pos[2] += size;
            else
                pos[2] -= size;

            if (cell->children[i] && octreeSplitPlanes(octree,cell->children[i],pos,size,state))
                return 1;
        }
    }
    return 0;
}

static INLINEDBG int octreeSplitPlanesRoot(Octree *octree,OctreeCell *cell,Vec3 ppos,F32 size,OctreeFindState *state)
{
    int retCode;

    PERFINFO_AUTO_START("octreeSplitPlanes",1);
    retCode = octreeSplitPlanes(octree, cell, ppos, size, state);
    PERFINFO_AUTO_STOP();
    return retCode;
}

void octreeFindInSphereCB(Octree *octree, OctreeFindCallback callback, void *user_data, const Vec3 point, F32 radius)
{
    Vec3 pos;
    F32 size;
    OctreeFindState    state = {0};

    size = octree->size * 0.5f;
    setVec3(pos,size,size,size);
    subVec3(point,octree->pos,state.point);
    state.radius = radius;
    state.callback = callback;
    state.user_data = user_data;
    state.tag = ++octree->tag;
    state.octree = octree;
    octreeSplitPlanesRoot(octree,octree->cell,pos,size,&state);
}

void octreeFindInSphereEA(Octree *octree, void ***node_array, const Vec3 point, F32 radius, OctreeSphereVisCallback vis_callback)
{
    Vec3 pos;
    F32 size;
    OctreeFindState    state = {0};

    size = octree->size * 0.5f;
    setVec3(pos,size,size,size);
    subVec3(point,octree->pos,state.point);
    state.radius = radius;
    state.sphere_vis_callback = vis_callback;
    state.nodes = node_array;
    state.tag = ++octree->tag;
    state.octree = octree;
    octreeSplitPlanesRoot(octree,octree->cell,pos,size,&state);
}

void **octreeFindInSphere(Octree *octree, const Vec3 point, F32 radius, OctreeSphereVisCallback vis_callback)
{
    Vec3 pos;
    F32 size;
    OctreeFindState    state = {0};
    void **nodes = NULL;

    size = octree->size * 0.5f;
    setVec3(pos,size,size,size);
    subVec3(point,octree->pos,state.point);
    state.radius = radius;
    state.sphere_vis_callback = vis_callback;
    state.nodes = &nodes;
    state.tag = ++octree->tag;
    state.octree = octree;
    octreeSplitPlanesRoot(octree,octree->cell,pos,size,&state);
    return nodes;
}

static void octreeSplitPlanesFrustum(Octree *octree,OctreeCell *cell,Vec3 ppos,F32 size,F32 radius,OctreeFindState *state,int clipped)
{
    const Frustum *frustum = state->frustum;
    Vec3 pos;
    int i, no_frustum_check = (clipped == FRUSTUM_CLIP_NONE);

    if (!cell)
        return;

    if (cell->entries)
        processOctreeEntries(octree,cell,state);

    if (!cell->children)
        return;

    radius *= 0.5f;
    size *= 0.5f;

    for (i=0; i<8; i++)
    {
        if (cell->children[i])
        {
            if (i & 1)
                scaleAddVec3(frustum->viewmat[0], size, ppos, pos);
            else
                scaleAddVec3(frustum->viewmat[0], -size, ppos, pos);

            if (i & 4)
                scaleAddVec3(frustum->viewmat[1], size, pos, pos);
            else
                scaleAddVec3(frustum->viewmat[1], -size, pos, pos);

            if (i & 2)
                scaleAddVec3(frustum->viewmat[2], size, pos, pos);
            else
                scaleAddVec3(frustum->viewmat[2], -size, pos, pos);

            if (no_frustum_check || (clipped = frustumCheckSphere(frustum, pos, radius)))
                octreeSplitPlanesFrustum(octree,cell->children[i],pos,size,radius,state,clipped);
        }
    }
}

static INLINEDBG void octreeSplitPlanesFrustumRoot(Octree *octree,OctreeCell *cell,Vec3 ppos,F32 size,F32 radius,OctreeFindState *state,int clipped)
{
    PERFINFO_AUTO_START("octreeSplitPlanesFrustum",1);
    octreeSplitPlanesFrustum(octree, cell, ppos, size, radius, state, clipped);
    PERFINFO_AUTO_STOP();
}

void octreeFindInFrustumCB(Octree *octree, OctreeFindCallback callback, void *user_data, const Vec3 offset, const Frustum *frustum)
{
    Vec3 pos, pos_worldspace, pos_cameraspace;
    F32 size, radius;
    int clipped;
    OctreeFindState    state = {0};

    size = octree->size * 0.5f;
    radius = size * 1.73205f; // = sqrtf(SQR(size) + SQR(size) + SQR(size))
    state.frustum = frustum;
    state.callback = callback;
    state.user_data = user_data;
    state.tag = ++octree->tag;
    state.octree = octree;
    setVec3(pos,size,size,size);
    addVec3(pos,octree->pos,pos);
    addVec3(pos,offset,pos_worldspace);
    mulVecMat4(pos_worldspace, frustum->viewmat, pos_cameraspace);
    if (clipped = frustumCheckSphere(frustum, pos_cameraspace, radius))
        octreeSplitPlanesFrustumRoot(octree,octree->cell,pos_cameraspace,size,radius,&state,clipped);
}

void octreeFindInFrustumEA(Octree *octree, void ***node_array, const Vec3 offset, const Frustum *frustum, OctreeFrustumVisCallback vis_callback)
{
    Vec3 pos, pos_worldspace, pos_cameraspace;
    F32 size, radius;
    int clipped;
    OctreeFindState    state = {0};

    size = octree->size * 0.5f;
    radius = size * 1.73205f; // = sqrtf(SQR(size) + SQR(size) + SQR(size))
    state.frustum = frustum;
    state.frustum_vis_callback = vis_callback;
    state.nodes = node_array;
    state.tag = ++octree->tag;
    state.octree = octree;
    setVec3(pos,size,size,size);
    addVec3(pos,octree->pos,pos);
    addVec3(pos,offset,pos_worldspace);
    mulVecMat4(pos_worldspace, frustum->viewmat, pos_cameraspace);
    if (clipped = frustumCheckSphere(frustum, pos_cameraspace, radius))
        octreeSplitPlanesFrustumRoot(octree,octree->cell,pos_cameraspace,size,radius,&state,clipped);
}

void **octreeFindInFrustum(Octree *octree, const Vec3 offset, const Frustum *frustum, OctreeFrustumVisCallback vis_callback)
{
    Vec3 pos, pos_worldspace, pos_cameraspace;
    F32 size, radius;
    int clipped;
    OctreeFindState    state = {0};
    void **nodes = NULL;

    size = octree->size * 0.5f;
    radius = size * 1.73205f; // = sqrtf(SQR(size) + SQR(size) + SQR(size))
    state.frustum = frustum;
    state.frustum_vis_callback = vis_callback;
    state.nodes = &nodes;
    state.tag = ++octree->tag;
    state.octree = octree;
    setVec3(pos,size,size,size);
    addVec3(pos,octree->pos,pos);
    addVec3(pos,offset,pos_worldspace);
    mulVecMat4(pos_worldspace, frustum->viewmat, pos_cameraspace);
    if (clipped = frustumCheckSphere(frustum, pos_cameraspace, radius))
        octreeSplitPlanesFrustumRoot(octree,octree->cell,pos_cameraspace,size,radius,&state,clipped);
    return nodes;
}

