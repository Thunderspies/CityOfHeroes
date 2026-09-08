#include "tricututils.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/qsortG.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include "quadric.h"

#define MESH_INVERSION_COST 100
#define TEXEDGE_COST 100
#define BOUNDARY_COST 100

#define DEBUG_CHECKS 0
#define DO_LOGGING 0

typedef Vec5 IVec;
typedef Quadric5 IQuadric;
#define addIQ            addQ5
#define evaluateIQ        evaluateQ5
#define optimizeIQ        optimizeQ5
#define zeroIQ            zeroQ5
#define initFromTriIQ    initFromTriQ5
#define scaleByIQ        scaleByQ5
#define addToIQ            addToQ5
#define copyIVec        copyVec5

typedef Vec3 SVec;
typedef Quadric3 SQuadric;
#define addSQ            addQ3
#define evaluateSQ        evaluateQ3
#define zeroSQ            zeroQ3
#define initFromTriSQ    initFromTriQ3
#define scaleBySQ        scaleByQ3
#define addToSQ            addToQ3


typedef struct _VertRemaps
{
    // tri remaps
    int *remaps; // sequence of [oldidx,newidx,numtris]
    int *remap_tris;

    // vert changes
    int *changes; // vert idxs
    F32 *positions; // Vec3 array
    F32 *tex1s; // Vec2 array
} VertRemaps;

typedef struct _ReduceInstruction
{
    VertRemaps vremaps;
    int num_tris_left;
    float cost, error;
} ReduceInstruction;

typedef struct _MeshReduction
{
    ReduceInstruction **instructions;
} MeshReduction;

static void addTriRemap(VertRemaps *vremaps, int oldidx, int newidx);
static void addVertChange(VertRemaps *vremaps, int vertidx, Vec3 newpos, Vec2 newtex1);


typedef enum
{
    USE_BEST,
    USE_VERT1,
    USE_VERT2
} VertUse;

typedef struct _IEdge
{
    int ivert1, ivert2;
    struct _SEdge *sedge;
} IEdge;

typedef struct _SEdge
{
    int svert1, svert2;
    IEdge **iedges;
    int *faces;

    double cost;
    U32 connects_boundaries : 1;
    U32 connects_texedges : 1;
    U32 is_boundary : 1;
    IVec best;
    Vec3 bestv;
    VertUse vert_to_use;
} SEdge;

typedef struct _IVertInfo
{
    struct _SVertInfo *svert;
    IEdge **iedges;
    IQuadric error;
    IVec attributes;
    U32 is_dead : 1;
} IVertInfo;

typedef struct _SVertInfo
{
    int svert;
    SVec attributes;
    Vec3 v;
    int *iverts;
    SEdge **sedges;
    int *faces;

    SQuadric error;
    U32 is_dead : 1;
    U32 on_boundary : 1;
    U32 on_texedge : 1;
} SVertInfo;

typedef struct _TriInfo
{
    Vec4 orig_plane;
    SEdge *sedges[3];
    U32 is_dead : 1;
} TriInfo;

typedef struct _TriCutType
{
    GMesh *mesh;
    IVertInfo *ivertinfos;
    SVertInfo **svertinfos;
    TriInfo *triinfos;
    IEdge **internal_edges;
    SEdge **super_edges;
    int scale_by_area;
    int use_optimal_placement;
    int maintain_borders;
    Vec3 multiplier;
    Vec3 min, max;
} TriCutType;

MP_DEFINE(IEdge);
MP_DEFINE(SEdge);
MP_DEFINE(SVertInfo);
MP_DEFINE(ReduceInstruction);

//////////////////////////////////////////////////////////////////////////
__forceinline static void freeSVert(SVertInfo *svi)
{
    eaiDestroy(&svi->faces);
    eaiDestroy(&svi->iverts);
    eaDestroy(&svi->sedges);
    MP_FREE(SVertInfo, svi);
}
__forceinline static void freeSEdge(SEdge *e)
{
    eaiDestroy(&e->faces);
    eaDestroy(&e->iedges);
    MP_FREE(SEdge, e);
}

__forceinline static void freeIEdge(IEdge *e)
{
    eaFindAndRemove(&e->sedge->iedges, e);
    MP_FREE(IEdge, e);
}

//////////////////////////////////////////////////////////////////////////

__forceinline static int compareEdges2(const SEdge *e1, const SEdge *e2)
{
    if (!e1->connects_boundaries && e2->connects_boundaries)
        return -1;

    if (e1->connects_boundaries && !e2->connects_boundaries)
        return 1;

    if (!e1->is_boundary && e2->is_boundary)
        return -1;

    if (e1->is_boundary && !e2->is_boundary)
        return 1;

    if (e1->cost < e2->cost)
        return -1;

    return e1->cost > e2->cost;
}

static int compareEdges(const void* e1Data, const void* e2Data)
{
    const SEdge ** e1 = (const SEdge **)e1Data;
    const SEdge ** e2 = (const SEdge **)e2Data;
    return compareEdges2(*e1, *e2);
}

static void insertionSortSEdges(SEdge **array, int n, int (*cmp)(const SEdge *, const SEdge *))
{
    int i, j;
    SEdge *t;
    for (i = 1; i < n; i++)
    {
        for (j = i; j > 0 && cmp(array[j - 1], array[j]) > 0; j--)
        {
            t = array[j];
            array[j] = array[j - 1];
            array[j - 1] = t;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

__forceinline static int isTriDegenerate(GMesh *mesh, GTriIdx *tri)
{
    int count = 0;
    float *a, *b, *c;

    if (tri->idx[0] < 0 || tri->idx[1] < 0 || tri->idx[2] < 0)
        return 1;

    if (tri->idx[0] == tri->idx[1] || tri->idx[0] == tri->idx[2] || tri->idx[1] == tri->idx[2])
        return 1;

    a = mesh->positions[tri->idx[0]];
    b = mesh->positions[tri->idx[1]];
    c = mesh->positions[tri->idx[2]];

    if (nearSameVec3(a, b) || nearSameVec3(a, c) || nearSameVec3(b, c))
        return 1;

    if (a[0] == b[0] && b[0] == c[0])
        count++;

    if (a[1] == b[1] && b[1] == c[1])
        count++;

    if (a[2] == b[2] && b[2] == c[2])
        count++;

    return count > 1;

}


_inline static int remapTriVert(GTriIdx *tri, int dead_vert, int new_vert)
{
    int i, ret = 0;

    for (i = 0; i < 3; i++)
    {
        if (tri->idx[i] == dead_vert)
            tri->idx[i] = new_vert;
        else if (tri->idx[i] == new_vert)
            ret = 1;
    }

    return ret;
}

__forceinline static int triMatch(GTriIdx *tri, int idx0, int idx1)
{
    if (tri->idx[0] == idx0 && tri->idx[1] == idx1)
        return 1;
    if (tri->idx[0] == idx1 && tri->idx[1] == idx0)
        return 1;
    if (tri->idx[1] == idx0 && tri->idx[2] == idx1)
        return 1;
    if (tri->idx[1] == idx1 && tri->idx[2] == idx0)
        return 1;
    if (tri->idx[2] == idx0 && tri->idx[0] == idx1)
        return 1;
    if (tri->idx[2] == idx1 && tri->idx[0] == idx0)
        return 1;
    return 0;
}

__forceinline static int triMatch1(GTriIdx *tri, int idx)
{
    if (tri->idx[0] == idx || tri->idx[1] == idx || tri->idx[2] == idx)
        return 1;
    return 0;
}

__forceinline static int countLiveTris(TriCutType *tc)
{
    int i, count = 0;

    for (i = 0; i < tc->mesh->tri_count; i++)
    {
        if (!tc->triinfos[i].is_dead)
            count++;
    }

    return count;
}

//////////////////////////////////////////////////////////////////////////

__forceinline static SVertInfo *getCreateSVert(TriCutType *tc, Vec3 vert)
{
    int i;
    SVertInfo *svi;

    for (i = 0; i < eaSize(&tc->svertinfos); i++)
    {
        if (nearSameVec3(tc->svertinfos[i]->v, vert))
            return tc->svertinfos[i];
    }

    MP_CREATE(SVertInfo, 500);
    svi = MP_ALLOC(SVertInfo);
    copyVec3(vert, svi->v);
    svi->attributes[0] = vert[0] * tc->multiplier[0];
    svi->attributes[1] = vert[1] * tc->multiplier[1];
    svi->attributes[2] = vert[2] * tc->multiplier[2];
    svi->svert = eaPush(&tc->svertinfos, svi);
    
    return svi;
}

__forceinline static int sedgeMatch(SEdge *e, int svert1, int svert2, int *need_swap)
{
    if (e->svert1 == svert1 && e->svert2 == svert2)
    {
        *need_swap = 0;
        return 1;
    }
    if (e->svert1 == svert2 && e->svert2 == svert1)
    {
        *need_swap = 1;
        return 1;
    }

    return 0;
}

__forceinline static SEdge *getCreateSEdge(TriCutType *tc, int ivert1, int ivert2, int *need_swap)
{
    int i;
    SEdge *e;
    SVertInfo *svert1 = tc->ivertinfos[ivert1].svert;
    SVertInfo *svert2 = tc->ivertinfos[ivert2].svert;

    assert(!svert1->is_dead);
    assert(!svert2->is_dead);
    assert(svert1 != svert2);

    for (i = 0; i < eaSize(&svert1->sedges); i++)
    {
        e = svert1->sedges[i];
        if (sedgeMatch(e, svert1->svert, svert2->svert, need_swap))
            return e;
    }

    need_swap = 0;

    MP_CREATE(SEdge, 500);
    e = MP_ALLOC(SEdge);

    e->svert1 = svert1->svert;
    e->svert2 = svert2->svert;

    eaPush(&tc->super_edges, e);

    eaPushUnique(&svert1->sedges, e);
    eaPushUnique(&svert2->sedges, e);

    return e;
}

__forceinline static int edgeMatch(IEdge *e, int vert1, int vert2)
{
    if (e->ivert1 == vert1 && e->ivert2 == vert2)
        return 1;
    if (e->ivert2 == vert1 && e->ivert1 == vert2)
        return 1;

    return 0;
}

__forceinline static IEdge *getCreateIEdge(TriCutType *tc, int ivert1, int ivert2)
{
    int i, need_swap = 0;
    IEdge *e;

    assert(ivert1 != ivert2);
    assert(!tc->ivertinfos[ivert1].is_dead);
    assert(!tc->ivertinfos[ivert2].is_dead);

    for (i = 0; i < eaSize(&tc->ivertinfos[ivert1].iedges); i++)
    {
        e = tc->ivertinfos[ivert1].iedges[i];
        if (edgeMatch(e, ivert1, ivert2))
        {
            eaPushUnique(&e->sedge->iedges, e);
            return e;
        }
    }

    MP_CREATE(IEdge, 500);
    e = MP_ALLOC(IEdge);

    e->ivert1 = ivert1;
    e->ivert2 = ivert2;

    eaPush(&tc->internal_edges, e);
    eaPush(&tc->ivertinfos[ivert1].iedges, e);
    eaPush(&tc->ivertinfos[ivert2].iedges, e);

    e->sedge = getCreateSEdge(tc, ivert1, ivert2, &need_swap);
    eaPushUnique(&e->sedge->iedges, e);

    if (need_swap)
    {
        int temp = e->ivert1;
        e->ivert1 = e->ivert2;
        e->ivert2 = temp;
    }

    return e;
}

//////////////////////////////////////////////////////////////////////////

static float getMeshPenalty(TriCutType *tc, int idx, Vec3 old_pos, Vec3 new_pos)
{
    Vec4 plane;
    int v0 = tc->mesh->tris[idx].idx[0];
    int v1 = tc->mesh->tris[idx].idx[1];
    int v2 = tc->mesh->tris[idx].idx[2];
    F32 *pos0 = tc->mesh->positions[v0];
    F32 *pos1 = tc->mesh->positions[v1];
    F32 *pos2 = tc->mesh->positions[v2];

    if (nearSameVec3(pos0, old_pos))
        pos0 = new_pos;
    if (nearSameVec3(pos1, old_pos))
        pos1 = new_pos;
    if (nearSameVec3(pos2, old_pos))
        pos2 = new_pos;
    if (makePlane(pos0, pos1, pos2, plane))
    {
        if (dotVec3(plane, tc->triinfos[idx].orig_plane) < 0.3f)
            return MESH_INVERSION_COST;
    }

    return 0;
}

// e1 is the error of collapsing towards vert1
// e2 is the error of collapsing towards vert2
static void setEdgeCost(TriCutType *tc, SEdge *e)
{
    static int *faces_array1 = 0, *faces_array2 = 0;
    float e1=0, e2=0, e3=0;
    SVertInfo *svert1 = tc->svertinfos[e->svert1];
    SVertInfo *svert2 = tc->svertinfos[e->svert2];
    int i, use_best = 0, is_internal_edge = !svert1->on_texedge && !svert2->on_texedge && eaSize(&e->iedges) == 1;
    IQuadric iq;
    SQuadric sq;

    assert(eaSize(&e->iedges) > 0);

    e->connects_boundaries = 0;
    e->connects_texedges = 0;

    // calculate error for collapsing this edge in either direction
    if (0)
    {
        for (i = 0; i < eaSize(&e->iedges); i++)
        {
            float e1_1, e2_1;
            IVertInfo *ivert1 = &tc->ivertinfos[e->iedges[i]->ivert1];
            IVertInfo *ivert2 = &tc->ivertinfos[e->iedges[i]->ivert2];
            addIQ(&ivert1->error, &ivert2->error, &iq);
            e1_1 = evaluateIQ(&iq, ivert1->attributes);
            if (e1_1 < 0)
                e1_1 = -e1_1;
            e2_1 = evaluateIQ(&iq, ivert2->attributes);
            if (e2_1 < 0)
                e2_1 = -e2_1;
            e1 += e1_1;
            e2 += e2_1;
        }
    }
    else if (is_internal_edge)
    {
        IVertInfo *ivert1 = &tc->ivertinfos[e->iedges[0]->ivert1];
        IVertInfo *ivert2 = &tc->ivertinfos[e->iedges[0]->ivert2];
        addIQ(&ivert1->error, &ivert2->error, &iq);
        e1 = evaluateIQ(&iq, ivert1->attributes);
        if (e1 < 0)
            e1 = -e1;
        e2 = evaluateIQ(&iq, ivert2->attributes);
        if (e2 < 0)
            e2 = -e2;
    }
    else
    {
        addSQ(&svert1->error, &svert2->error, &sq);
        e1 = evaluateSQ(&sq, svert1->attributes);
        if (e1 < 0)
            e1 = -e1;
        e2 = evaluateSQ(&sq, svert2->attributes);
        if (e2 < 0)
            e2 = -e2;
    }

    // find affected tris
    eaiSetSize(&faces_array1, 0);
    eaiSetSize(&faces_array2, 0);
    for (i = 0; i < eaiSize(&svert1->faces); i++)
    {
        eaiPushUnique(&faces_array1, svert1->faces[i]);
    }
    for (i = 0; i < eaiSize(&svert2->faces); i++)
    {
        eaiPushUnique(&faces_array2, svert2->faces[i]);
    }
    for (i = 0; i < eaiSize(&e->faces); i++)
    {
        eaiFindAndRemove(&faces_array1, e->faces[i]);
        eaiFindAndRemove(&faces_array2, e->faces[i]);
    }

    // apply mesh inversion penalty
    for (i = 0; i < eaiSize(&faces_array1); i++)
        e2 += getMeshPenalty(tc, faces_array1[i], svert1->v, svert2->v); // make it expensive not to go to svert1
    for (i = 0; i < eaiSize(&faces_array2); i++)
        e1 += getMeshPenalty(tc, faces_array2[i], svert2->v, svert1->v); // make it expensive not to go to svert2

    e->is_boundary = eaSize(&e->iedges) == 1 && eaiSize(&e->faces) == 1;

    // optimal placement
    if (tc->use_optimal_placement && is_internal_edge)
    {
        use_best = optimizeIQ(&iq, e->best);
        if (use_best)
        {
            e3 = evaluateIQ(&iq, e->best);
            if (e3 < 0)
                e3 = -e3;

            e->bestv[0] = e->best[0] / tc->multiplier[0];
            e->bestv[1] = e->best[1] / tc->multiplier[1];
            e->bestv[2] = e->best[2] / tc->multiplier[2];

            // apply mesh inversion penalty
            for (i = 0; i < eaiSize(&faces_array1); i++)
                e3 += getMeshPenalty(tc, faces_array1[i], svert1->v, e->bestv);
            for (i = 0; i < eaiSize(&faces_array2); i++)
                e3 += getMeshPenalty(tc, faces_array2[i], svert2->v, e->bestv);
        }
    }

    // maintain boundary conditions
    if (svert1->on_texedge && svert2->on_texedge)
    {
        use_best = 0;

        // figure out if it is ok to collapse this edge
        for (i = 0; i < eaiSize(&svert1->iverts); i++)
        {
            int j;
            for (j = 0; j < eaSize(&e->iedges); j++)
            {
                IEdge *ie = e->iedges[j];
                if (ie->ivert1 == svert1->iverts[i])
                    break;
            }

            if (j >= eaSize(&e->iedges))
            {
                // one of the iverts is not included in this edge's iedges, not good to collapse
                e->connects_texedges = 1;
                break;
            }
        }
        for (i = 0; i < eaiSize(&svert2->iverts); i++)
        {
            int j;
            for (j = 0; j < eaSize(&e->iedges); j++)
            {
                IEdge *ie = e->iedges[j];
                if (ie->ivert2 == svert2->iverts[i])
                    break;
            }

            if (j >= eaSize(&e->iedges))
            {
                // one of the iverts is not included in this edge's iedges, not good to collapse
                e->connects_texedges = 1;
                break;
            }
        }

        if (e->connects_texedges)
        {
            e1 += TEXEDGE_COST;
            e2 += TEXEDGE_COST;
        }
    }
    else if (svert1->on_texedge)
    {
        use_best = 0;
        e2 += TEXEDGE_COST; // make it expensive not to go to svert1
    }
    else if (svert2->on_texedge)
    {
        use_best = 0;
        e1 += TEXEDGE_COST; // make it expensive not to go to svert2
    }

    if (svert1->on_boundary && svert2->on_boundary)
    {
        if (!e->is_boundary)
            e->connects_boundaries = 1;
        e1 += BOUNDARY_COST;
        e2 += BOUNDARY_COST;
        e3 += BOUNDARY_COST;
    }
    else if (svert1->on_boundary)
    {
        use_best = 0;
        e2 += e1 + BOUNDARY_COST;
    }
    else if (svert2->on_boundary)
    {
        use_best = 0;
        e1 += e2 + BOUNDARY_COST;
    }

    if (use_best && e3 < e1 && e3 < e2)
    {
        e->vert_to_use = USE_BEST;
        e->cost = e3;
    }
    else if (e1 <= e2)
    {
        e->vert_to_use = USE_VERT1;
        e->cost = e1;
    }
    else
    {
        e->vert_to_use = USE_VERT2;
        e->cost = e2;
    }

    if (!tc->maintain_borders)
        e->is_boundary = 0;
}

void computeQuadrics(TriCutType *tc, int **verts)
{
    int i, j, count, count2;
    SQuadric sq;
    IQuadric iq;
    int *changed_verts = *verts;
    static int *changed_faces = 0;
    static SEdge **changed_edges = 0;
    
    eaiSetSize(&changed_faces, 0);
    eaSetSize(&changed_edges, 0);

    // set all vertex quadrics to zero
    count = eaiSize(verts);
    for (i = 0; i < count; i++)
    {
        IVertInfo *ivert = &tc->ivertinfos[changed_verts[i]];
        SVertInfo *svert = ivert->svert;
        zeroSQ(&svert->error);
        zeroIQ(&ivert->error);

        count2 = eaiSize(&svert->faces);
        for (j = 0; j < count2; j++)
            eaiPushUnique(&changed_faces, svert->faces[j]);
    }

    // add faces to vertex quadrics
    count = eaiSize(&changed_faces);
    for (i = 0; i < count; i++)
    {
        int face = changed_faces[i];
        int v0 = tc->mesh->tris[face].idx[0], v1 = tc->mesh->tris[face].idx[1], v2 = tc->mesh->tris[face].idx[2];

        if (tc->triinfos[face].is_dead)
            continue;

        initFromTriIQ(&iq, tc->ivertinfos[v0].attributes, tc->ivertinfos[v1].attributes, tc->ivertinfos[v2].attributes);
        initFromTriSQ(&sq, tc->ivertinfos[v0].svert->attributes, tc->ivertinfos[v1].svert->attributes, tc->ivertinfos[v2].svert->attributes);

        if (tc->scale_by_area)
        {
            F32 area = gmeshGetTriArea(tc->mesh, face);
            scaleBySQ(&sq, area);
            scaleByIQ(&iq, area);
        }

        if (eaiFind(verts, v0) >= 0)
        {
            addToSQ(&sq, &tc->ivertinfos[v0].svert->error);
            addToIQ(&iq, &tc->ivertinfos[v0].error);
        }
        if (eaiFind(verts, v1) >= 0)
        {
            addToSQ(&sq, &tc->ivertinfos[v1].svert->error);
            addToIQ(&iq, &tc->ivertinfos[v1].error);
        }
        if (eaiFind(verts, v2) >= 0)
        {
            addToSQ(&sq, &tc->ivertinfos[v2].svert->error);
            addToIQ(&iq, &tc->ivertinfos[v2].error);
        }
    }

    // calculate edge contraction cost and vertex placement
    count = eaiSize(verts);
    for (i = 0; i < count; i++)
    {
        SVertInfo *svert = tc->ivertinfos[changed_verts[i]].svert;
        count2 = eaSize(&svert->sedges);
        for (j = 0; j < count2; j++)
            eaPushUnique(&changed_edges, svert->sedges[j]);
    }

    count = eaSize(&changed_edges);
    for (i = 0; i < count; i++)
        setEdgeCost(tc, changed_edges[i]);

    // sort edges
    insertionSortSEdges(tc->super_edges, eaSize(&tc->super_edges), compareEdges2);
}

void computeAllQuadrics(TriCutType *tc)
{
    int i, count;
    SQuadric sq;
    IQuadric iq;

    // set all vertex quadrics to zero
    for (i = 0; i < eaSize(&tc->svertinfos); i++)
        zeroSQ(&tc->svertinfos[i]->error);

    for (i = 0; i < tc->mesh->vert_count; i++)
        zeroIQ(&tc->ivertinfos[i].error);

    // add faces to vertex quadrics
    for (i = 0; i < tc->mesh->tri_count; i++)
    {
        int v0 = tc->mesh->tris[i].idx[0], v1 = tc->mesh->tris[i].idx[1], v2 = tc->mesh->tris[i].idx[2];

        if (tc->triinfos[i].is_dead)
            continue;

        initFromTriIQ(&iq, tc->ivertinfos[v0].attributes, tc->ivertinfos[v1].attributes, tc->ivertinfos[v2].attributes);
        initFromTriSQ(&sq, tc->ivertinfos[v0].svert->attributes, tc->ivertinfos[v1].svert->attributes, tc->ivertinfos[v2].svert->attributes);

        if (tc->scale_by_area)
        {
            F32 area = gmeshGetTriArea(tc->mesh, i);
            scaleBySQ(&sq, area);
            scaleByIQ(&iq, area);
        }

        addToSQ(&sq, &tc->ivertinfos[v0].svert->error);
        addToSQ(&sq, &tc->ivertinfos[v1].svert->error);
        addToSQ(&sq, &tc->ivertinfos[v2].svert->error);

        addToIQ(&iq, &tc->ivertinfos[v0].error);
        addToIQ(&iq, &tc->ivertinfos[v1].error);
        addToIQ(&iq, &tc->ivertinfos[v2].error);
    }

    // calculate edge contraction cost and vertex placement
    count = eaSize(&tc->super_edges);
    for (i = 0; i < count; i++)
        setEdgeCost(tc, tc->super_edges[i]);

    // sort edges
    qsortG(tc->super_edges, count, sizeof(*tc->super_edges), compareEdges);
}

static void checkVerts(TriCutType *tc)
{
    int i, j, k, idx;
    for (i = 0; i < eaSize(&tc->svertinfos); i++)
    {
        SVertInfo *svert = tc->svertinfos[i];
        for (j = 0; j < eaiSize(&svert->faces); j++)
        {
            TriInfo *tri = &tc->triinfos[svert->faces[j]];
            for (k = 0; k < 3; k++)
            {
                if (tri->is_dead)
                {
                    assert(!tri->sedges[k]);
                }
                else
                {
                    assert(tri->sedges[k]);
                    idx = eaiFind(&tri->sedges[k]->faces, svert->faces[j]);
                    assert(idx >= 0);
                }
            }
        }
    }
}

static void checkEdges(TriCutType *tc)
{
    int i, j, count;
    count = eaSize(&tc->internal_edges);
    for (i = 0; i < count; i++)
    {
        IEdge *e = tc->internal_edges[i];
        int v1 = e->ivert1;
        int v2 = e->ivert2;

        assert(v1 != v2);
        assert(!tc->ivertinfos[v1].is_dead);
        assert(!tc->ivertinfos[v2].is_dead);

        for (j = i+1; j < count; j++)
        {
            assert(!edgeMatch(tc->internal_edges[j], v1, v2));
        }

        j = eaiFind(&tc->svertinfos[e->sedge->svert1]->iverts, v1);
        assert(j >= 0);

        j = eaiFind(&tc->svertinfos[e->sedge->svert2]->iverts, v2);
        assert(j >= 0);
    }

    count = eaSize(&tc->super_edges);
    for (i = 0; i < count; i++)
    {
        SEdge *se = tc->super_edges[i];
        assert(!tc->svertinfos[se->svert1]->is_dead);
        assert(!tc->svertinfos[se->svert2]->is_dead);
    }
}

static void checkTris(TriCutType *tc)
{
    int i;
    for (i = 0; i < tc->mesh->tri_count; i++)
    {
        assert(tc->triinfos[i].is_dead || !isTriDegenerate(tc->mesh, &tc->mesh->tris[i]));
    }
}

static void killTri(TriCutType *tc, int idx)
{
    GTriIdx *tri = &tc->mesh->tris[idx];
    assert(!tc->triinfos[idx].is_dead);
    tc->triinfos[idx].is_dead = 1;
    if (tc->triinfos[idx].sedges[0])
        eaiFindAndRemove(&tc->triinfos[idx].sedges[0]->faces, idx);
    if (tc->triinfos[idx].sedges[1])
        eaiFindAndRemove(&tc->triinfos[idx].sedges[1]->faces, idx);
    if (tc->triinfos[idx].sedges[2])
        eaiFindAndRemove(&tc->triinfos[idx].sedges[2]->faces, idx);
    tc->triinfos[idx].sedges[0] = tc->triinfos[idx].sedges[1] = tc->triinfos[idx].sedges[2] = 0;
    eaiFindAndRemove(&tc->ivertinfos[tri->idx[0]].svert->faces, idx);
    eaiFindAndRemove(&tc->ivertinfos[tri->idx[1]].svert->faces, idx);
    eaiFindAndRemove(&tc->ivertinfos[tri->idx[2]].svert->faces, idx);
    gmeshMarkBadTri(tc->mesh, idx);
}

static void calculateTriInfo(TriCutType *tc, int idx, int init)
{
    int v0 = tc->mesh->tris[idx].idx[0];
    int v1 = tc->mesh->tris[idx].idx[1];
    int v2 = tc->mesh->tris[idx].idx[2];
    IEdge *e0, *e1, *e2;

    if (!tc->triinfos[idx].is_dead && isTriDegenerate(tc->mesh, &tc->mesh->tris[idx]))
    {
        killTri(tc, idx);
        return;
    }

    if (init)
    {
        makePlane(tc->mesh->positions[v0], tc->mesh->positions[v1], tc->mesh->positions[v2], tc->triinfos[idx].orig_plane);
    }
    else
    {
        Vec4 plane;
        F32 dp;
        if (0 && makePlane(tc->mesh->positions[v0], tc->mesh->positions[v1], tc->mesh->positions[v2], plane))
        {
            dp = dotVec3(plane, tc->triinfos[idx].orig_plane);
            assert(dp > 0);
        }
    }


    // set tri edges
    e0 = getCreateIEdge(tc, v0, v1);
    e1 = getCreateIEdge(tc, v1, v2);
    e2 = getCreateIEdge(tc, v2, v0);
    tc->triinfos[idx].sedges[0] = e0->sedge;
    tc->triinfos[idx].sedges[1] = e1->sedge;
    tc->triinfos[idx].sedges[2] = e2->sedge;

    // add tri to the face lists of its edges
    eaiPushUnique(&e0->sedge->faces, idx);
    eaiPushUnique(&e1->sedge->faces, idx);
    eaiPushUnique(&e2->sedge->faces, idx);

    // add tri to the face lists of its verts
    eaiPushUnique(&tc->ivertinfos[v0].svert->faces, idx);
    eaiPushUnique(&tc->ivertinfos[v1].svert->faces, idx);
    eaiPushUnique(&tc->ivertinfos[v2].svert->faces, idx);
}

static void setVertAttributes(TriCutType *tc, int idx)
{
    float *v = &tc->ivertinfos[idx].attributes[0];

    assert(tc->mesh->positions);
    v[0] = tc->mesh->positions[idx][0] * tc->multiplier[0];
    v[1] = tc->mesh->positions[idx][1] * tc->multiplier[1];
    v[2] = tc->mesh->positions[idx][2] * tc->multiplier[2];

    tc->ivertinfos[idx].svert = getCreateSVert(tc, tc->mesh->positions[idx]);
    eaiPushUnique(&tc->ivertinfos[idx].svert->iverts, idx);

    if (tc->mesh->tex1s)
    {
        v[3] = tc->mesh->tex1s[idx][0];
        v[4] = tc->mesh->tex1s[idx][1];
    }
    else
    {
        v[3] = 0;
        v[4] = 0;
    }
}

static void changeSVert(TriCutType *tc, SVertInfo *svert, IVec attributes, VertRemaps *vremaps)
{
    int j;

    assert(!svert->on_texedge);

    for (j = 0; j < eaiSize(&svert->iverts); j++)
    {
        int idx = svert->iverts[j];

        assert(idx >= 0 && idx < tc->mesh->vert_count);

        tc->mesh->positions[idx][0] = attributes[0] / tc->multiplier[0];
        tc->mesh->positions[idx][1] = attributes[1] / tc->multiplier[1];
        tc->mesh->positions[idx][2] = attributes[2] / tc->multiplier[2];

        if (tc->mesh->tex1s)
        {
            tc->mesh->tex1s[idx][0] = attributes[3];
            tc->mesh->tex1s[idx][1] = attributes[4];
        }

        copyIVec(attributes, tc->ivertinfos[idx].attributes);

        addVertChange(vremaps, idx, tc->mesh->positions[idx], tc->mesh->tex1s[idx]);
    }
}

static void combineSVerts(TriCutType *tc, SVertInfo *dead_svert, SVertInfo *new_svert, VertRemaps *vremaps)
{
    int j;
    F32 *pos = new_svert->v;

    assert(new_svert != dead_svert);

    dead_svert->is_dead = 1;

    assert(eaSize(&dead_svert->sedges) == 0);
    assert(eaiSize(&dead_svert->faces) == 0);

    for (j = 0; j < eaiSize(&dead_svert->iverts); j++)
    {
        int idx = dead_svert->iverts[j];
        IVertInfo *ivert = &tc->ivertinfos[idx];

        assert(idx >= 0 && idx < tc->mesh->vert_count);
        assert(!ivert->is_dead);
        assert(eaSize(&ivert->iedges) == 0);

        copyVec3(pos, tc->mesh->positions[idx]);
        copyVec3(new_svert->attributes, ivert->attributes);

        ivert->svert = new_svert;
        eaiPushUnique(&new_svert->iverts, idx);

        addVertChange(vremaps, idx, tc->mesh->positions[idx], tc->mesh->tex1s[idx]);
    }

    new_svert->on_texedge = eaiSize(&new_svert->iverts) > 1;

    eaiDestroy(&dead_svert->iverts);
}

static TriCutType *createTriCut(GMesh *mesh, Vec3 min, Vec3 max, int scale_by_area, int use_optimal_placement, int maintain_borders)
{
    int i;
    TriCutType *tc = malloc(sizeof(*tc));
    ZeroStruct(tc);

    tc->mesh = mesh;

    tc->scale_by_area = scale_by_area;
    tc->use_optimal_placement = use_optimal_placement;
    tc->maintain_borders = maintain_borders;

    tc->triinfos = calloc(sizeof(*tc->triinfos), mesh->tri_count);
    ZeroStructs(tc->triinfos, mesh->tri_count);

    tc->ivertinfos = calloc(sizeof(*tc->ivertinfos), mesh->vert_count);
    ZeroStructs(tc->ivertinfos, mesh->vert_count);

    copyVec3(min, tc->min);
    copyVec3(max, tc->max);
    subVec3(max, min, tc->multiplier);
    tc->multiplier[0] = tc->multiplier[0] ? (1.f / tc->multiplier[0]) : 1.f;
    tc->multiplier[1] = tc->multiplier[1] ? (1.f / tc->multiplier[1]) : 1.f;
    tc->multiplier[2] = tc->multiplier[2] ? (1.f / tc->multiplier[2]) : 1.f;

    for (i = 0; i < mesh->vert_count; i++)
        setVertAttributes(tc, i);

    for (i = 0; i < mesh->tri_count; i++)
        calculateTriInfo(tc, i, 1);

    // mark boundary verts
    for (i = 0; i < eaSize(&tc->super_edges); i++)
    {
        SEdge *e = tc->super_edges[i];
        if (eaSize(&e->iedges) == 1 && eaiSize(&e->faces) == 1)
        {
            tc->svertinfos[e->svert1]->on_boundary = 1;
            tc->svertinfos[e->svert2]->on_boundary = 1;
        }
    }

    for (i = 0; i < eaSize(&tc->svertinfos); i++)
    {
        SVertInfo *svert = tc->svertinfos[i];
        if (eaiSize(&svert->iverts) > 1)
            svert->on_texedge = 1;
    }

    computeAllQuadrics(tc);

    if (DEBUG_CHECKS)
    {
        checkVerts(tc);
        checkEdges(tc);
        checkTris(tc);
    }

    return tc;
}

__forceinline static void freeIEdge(IEdge *e);
static void freeIEdgeCallback(void* arg0)
{
    freeIEdge((IEdge *)arg0);
}

__forceinline static void freeSEdge(SEdge *e);
static void freeSEdgeCallback(void* arg0)
{
    freeSEdge((SEdge *)arg0);
}

__forceinline static void freeSVert(SVertInfo *svi);
static void freeSVertCallback(void* arg0)
{
    freeSVert((SVertInfo *)arg0);
}

static void freeTriCut(TriCutType *tc)
{
    int i;

    eaDestroyEx(&tc->internal_edges, freeIEdgeCallback);
    eaDestroyEx(&tc->super_edges, freeSEdgeCallback);
    eaDestroyEx(&tc->svertinfos, freeSVertCallback);

    for (i = 0; i < tc->mesh->vert_count; i++)
    {
        eaDestroy(&tc->ivertinfos[i].iedges);
    }

    free(tc->triinfos);
    free(tc->ivertinfos);
    free(tc);
}

#define LOG if (DO_LOGGING) log_printf

// remove references to old edge, free old edge
static void removeEdge(TriCutType *tc, SEdge *old)
{
    int i, count;
    IEdge *e_old;

    count = eaiSize(&old->faces);
    for (i = 0; i < count; i++)
    {
        // remove the dead edge on its tris
        int j;
        for (j = 0; j < 3; j++)
        {
            if (tc->triinfos[old->faces[i]].sedges[j] == old)
                tc->triinfos[old->faces[i]].sedges[j] = 0;
        }
    }
    eaiSetSize(&old->faces, 0);

    while (e_old = eaPop(&old->iedges))
    {
        // remove old edge from its verts
        eaFindAndRemove(&tc->ivertinfos[e_old->ivert1].iedges, e_old);
        eaFindAndRemove(&tc->ivertinfos[e_old->ivert2].iedges, e_old);

        // free old edge
        eaFindAndRemove(&tc->internal_edges, e_old);
        freeIEdge(e_old);
    }

    // remove old edge from its verts
    eaFindAndRemove(&tc->svertinfos[old->svert1]->sedges, old);
    eaFindAndRemove(&tc->svertinfos[old->svert2]->sedges, old);

    // free old edge
    eaFindAndRemove(&tc->super_edges, old);
    assert(eaSize(&old->iedges) == 0);
    freeSEdge(old);
}

// this is the complicated part...
static ReduceInstruction *collapseEdge(TriCutType *tc, SEdge *sedge)
{
    int i, j, dead_count;
    ReduceInstruction *ri;
    VertUse last_use = sedge->vert_to_use;
    Vec3 last_new = {0};
    float *v1 = tc->svertinfos[sedge->svert1]->v;
    float *v2 = tc->svertinfos[sedge->svert2]->v;
    int new_sidx, dead_sidx;
    SVertInfo *dead_svi;
    SVertInfo *new_svi;

    static SEdge **changed_edges = 0;
    static int *changed_verts = 0;
    static int *changed_faces = 0;
    static int *dead_idxs = 0;
    static int *new_idxs = 0;

    eaSetSize(&changed_edges, 0);
    eaiSetSize(&changed_faces, 0);
    eaiSetSize(&changed_verts, 0);
    eaiSetSize(&dead_idxs, 0);
    eaiSetSize(&new_idxs, 0);

    if (sedge->vert_to_use == USE_BEST)
    {
        new_sidx = sedge->svert1;
        dead_sidx = sedge->svert2;
        copyVec3(sedge->bestv, last_new);
        LOG("edgecollapse", "collapseEdge s%d -> s%d, best (%f, %f, %f), error %f", 
            dead_sidx, 
            new_sidx, 
            sedge->bestv[0], sedge->bestv[1], sedge->bestv[2], 
            sedge->cost);
    }
    else if (sedge->vert_to_use == USE_VERT1)
    {
        new_sidx = sedge->svert1;
        dead_sidx = sedge->svert2;
        copyVec3(tc->svertinfos[new_sidx]->v, last_new);
        LOG("edgecollapse", "collapseEdge s%d -> s%d, error %f", 
            dead_sidx, 
            new_sidx, 
            sedge->cost);
    }
    else
    {
        new_sidx = sedge->svert2;
        dead_sidx = sedge->svert1;
        copyVec3(tc->svertinfos[new_sidx]->v, last_new);
        LOG("edgecollapse", "collapseEdge s%d -> s%d, error %f", 
            dead_sidx, 
            new_sidx, 
            sedge->cost);
    }

    dead_svi = tc->svertinfos[dead_sidx];
    new_svi = tc->svertinfos[new_sidx];

    MP_CREATE(ReduceInstruction, 500);
    ri = MP_ALLOC(ReduceInstruction);
    ri->cost = sedge->cost;


    if (sedge->vert_to_use == USE_BEST)
        changeSVert(tc, new_svi, sedge->best, &ri->vremaps);


    for (j = 0; j < eaSize(&sedge->iedges); j++)
    {
        IEdge *e = sedge->iedges[j];
        int dead_idx, new_idx;

        if (sedge->vert_to_use == USE_BEST || sedge->vert_to_use == USE_VERT1)
        {
            new_idx = e->ivert1;
            dead_idx = e->ivert2;
        }
        else
        {
            new_idx = e->ivert2;
            dead_idx = e->ivert1;
        }

        addTriRemap(&ri->vremaps, dead_idx, new_idx);
    }

    // mark dead faces
    for (i = 0; i < eaiSize(&sedge->faces); i++)
        eaiPush(&changed_faces, sedge->faces[i]);
    for (i = 0; i < eaiSize(&changed_faces); i++)
        killTri(tc, changed_faces[i]);
    eaiDestroy(&sedge->faces);
    eaiSetSize(&changed_faces, 0);


    // collect list of changed faces
    for (i = 0; i < eaiSize(&dead_svi->faces); i++)
    {
        assert(!tc->triinfos[dead_svi->faces[i]].is_dead);
        eaiPushUnique(&changed_faces, dead_svi->faces[i]);
    }
    eaiDestroy(&dead_svi->faces);
    dead_count = eaiSize(&changed_faces);

    for (i = 0; i < eaiSize(&new_svi->faces); i++)
    {
        assert(!tc->triinfos[new_svi->faces[i]].is_dead);
        eaiPushUnique(&changed_faces, new_svi->faces[i]);
    }


    // collect list of changed edges and verts
    for (i = 0; i < eaSize(&dead_svi->sedges); i++)
    {
        SVertInfo *svi;
        eaPushUnique(&changed_edges, dead_svi->sedges[i]);

        svi = tc->svertinfos[dead_svi->sedges[i]->svert1];
        for (j = 0; j < eaiSize(&svi->iverts); j++)
            eaiPushUnique(&changed_verts, svi->iverts[j]);

        svi = tc->svertinfos[dead_svi->sedges[i]->svert2];
        for (j = 0; j < eaiSize(&svi->iverts); j++)
            eaiPushUnique(&changed_verts, svi->iverts[j]);
    }

    for (i = 0; i < eaSize(&new_svi->sedges); i++)
    {
        SVertInfo *svi;
        eaPushUnique(&changed_edges, new_svi->sedges[i]);

        svi = tc->svertinfos[new_svi->sedges[i]->svert1];
        for (j = 0; j < eaiSize(&svi->iverts); j++)
            eaiPushUnique(&changed_verts, svi->iverts[j]);

        svi = tc->svertinfos[new_svi->sedges[i]->svert2];
        for (j = 0; j < eaiSize(&svi->iverts); j++)
            eaiPushUnique(&changed_verts, svi->iverts[j]);
    }
    eaiFindAndRemove(&changed_verts, dead_sidx);
    assert(eaFind(&changed_edges, sedge) >= 0);

    // queue tri verts to remap
    for (i = 0; i < dead_count; i++)
    {
        GTriIdx *tri = &tc->mesh->tris[changed_faces[i]];
        assert(!tc->triinfos[changed_faces[i]].is_dead);
        for (j = 0; j < eaSize(&sedge->iedges); j++)
        {
            IEdge *e = sedge->iedges[j];
            int dead_idx, new_idx;
            if (sedge->vert_to_use == USE_BEST)
            {
                new_idx = e->ivert1;
                dead_idx = e->ivert2;
            }
            else if (sedge->vert_to_use == USE_VERT1)
            {
                new_idx = e->ivert1;
                dead_idx = e->ivert2;
            }
            else
            {
                new_idx = e->ivert2;
                dead_idx = e->ivert1;
            }

            if (triMatch(tri, dead_idx, new_idx))
            {
                eaiPush(&new_idxs, new_idx);
                eaiPush(&dead_idxs, dead_idx);
                break;
            }
        }
        if (j < eaSize(&sedge->iedges))
            continue;

        for (j = 0; j < eaSize(&sedge->iedges); j++)
        {
            IEdge *e = sedge->iedges[j];
            int dead_idx, new_idx;

            if (sedge->vert_to_use == USE_BEST)
            {
                new_idx = e->ivert1;
                dead_idx = e->ivert2;
            }
            else if (sedge->vert_to_use == USE_VERT1)
            {
                new_idx = e->ivert1;
                dead_idx = e->ivert2;
            }
            else
            {
                new_idx = e->ivert2;
                dead_idx = e->ivert1;
            }

            if (triMatch1(tri, dead_idx))
            {
                eaiPush(&new_idxs, new_idx);
                eaiPush(&dead_idxs, dead_idx);
                break;
            }
        }
        if (j < eaSize(&sedge->iedges))
            continue;

        assert(sedge->vert_to_use != USE_BEST);

        eaiPush(&new_idxs, -1);
        eaiPush(&dead_idxs, -1);
    }

    // destroy changed edges
    for (i = 0; i < eaSize(&changed_edges); i++)
        removeEdge(tc, changed_edges[i]);

    // remap tri verts
    assert(eaiSize(&dead_idxs) == dead_count);
    assert(eaiSize(&new_idxs) == dead_count);
    for (i = 0; i < dead_count; i++)
    {
        GTriIdx *tri = &tc->mesh->tris[changed_faces[i]];
        assert(!tc->triinfos[changed_faces[i]].is_dead);
        if (dead_idxs[i] >= 0)
        {
            eaiFindAndRemove(&dead_svi->iverts, dead_idxs[i]);
            tc->ivertinfos[dead_idxs[i]].is_dead = 1;
            remapTriVert(tri, dead_idxs[i], new_idxs[i]);
        }
    }

    if (sedge->vert_to_use != USE_BEST)
        combineSVerts(tc, dead_svi, new_svi, &ri->vremaps);

    // get new tri info
    for (i = 0; i < eaiSize(&changed_faces); i++)
        calculateTriInfo(tc, changed_faces[i], 0);

    // debug check
    if (DEBUG_CHECKS)
    {
        checkVerts(tc);
        checkEdges(tc);
        checkTris(tc);
    }


    // recalculate costs
    computeQuadrics(tc, &changed_verts);

    return ri;
}

static float cost_lookup[11] = 
{
    0.00000f,
        0.0000005f,
        0.000001f,
        0.000005f,
        0.00001f,
        0.00005f,
        0.0001f,
        0.0005f,
        0.1f,
        1.0f,
        10000000.f,
};

static float costLookup(float target_error)
{
    float err = target_error * 10.f;
    int lu = err;
    float t = err - (float)lu;

    float max_cost = cost_lookup[lu];

    assert(t >= 0 && t < 1);

    if (t)
        max_cost += t * (cost_lookup[lu+1] - cost_lookup[lu]);

    return max_cost;
}

static float inverseCostLookup(float max_cost)
{
    int i;
    float t, error;

    for (i = 0; i < 11; i++)
    {
        if (max_cost <= cost_lookup[i])
        {
            if (i == 0)
                return 0;

            t = (cost_lookup[i] - max_cost) / (cost_lookup[i] - cost_lookup[i-1]);

            error = (((float)i) - t) / 10.f;
            assert (error >= 0 && error <= 1);

            return error;
        }
    }

    return 1;
}

static float dist_lookup[11] = 
{
    0.0f,
    0.0f,
    25.0f,
    50.0f,
    75.f,
    100.f,
    150.f,
    200.f,
    400.f,
    800.f,
    3000.f,
};

static float distLookup(float target_error)
{
    float err = target_error * 10.f;
    int lu = err;
    float t = err - (float)lu;

    float dist = dist_lookup[lu];

    assert(t >= 0 && t < 1);

    if (t)
        dist += t * (dist_lookup[lu+1] - dist_lookup[lu]);

    return dist;
}

static float getDistanceForError(float target_error, float radius)
{
    float dist = distLookup(target_error);
    if (dist < 800.f && radius > 100.f)
        dist *= radius / 100.f;
    else if (dist < 800.f && radius < 30.f)
        dist *= radius / 30.f;
    return dist;
}

static void addTriRemap(VertRemaps *vremaps, int oldidx, int newidx)
{
    eaiPush(&vremaps->remaps, oldidx);
    eaiPush(&vremaps->remaps, newidx);
    eaiPush(&vremaps->remaps, 0);
}

static void addVertChange(VertRemaps *vremaps, int vertidx, Vec3 newpos, Vec2 newtex1)
{
    eaiPush(&vremaps->changes, vertidx);
    eafPush(&vremaps->positions, newpos[0]);
    eafPush(&vremaps->positions, newpos[1]);
    eafPush(&vremaps->positions, newpos[2]);
    eafPush(&vremaps->tex1s, newtex1[0]);
    eafPush(&vremaps->tex1s, newtex1[1]);
}

static void freeReduceInstruction(void* riData)
{
    ReduceInstruction * ri = (ReduceInstruction *)riData;
    eaiDestroy(&ri->vremaps.remaps);
    eaiDestroy(&ri->vremaps.remap_tris);
    eaiDestroy(&ri->vremaps.changes);
    eafDestroy(&ri->vremaps.positions);
    eafDestroy(&ri->vremaps.tex1s);
    MP_FREE(ReduceInstruction, ri);
}

static void freeMeshReduction(MeshReduction *mr)
{
    eaDestroyEx(&mr->instructions, freeReduceInstruction);
    free(mr);
}

static void convertMRtoGMR(GMeshReductions *gmr, const MeshReduction *mr)
{
    int i;
    int *remaps_ptr, *remap_tris_ptr, *changes_ptr;
    F32 *pos_ptr, *tex_ptr;

    gmr->num_reductions = eaSize(&mr->instructions);

    if (!gmr->num_reductions)
        return;

    gmr->num_tris_left = malloc(gmr->num_reductions*sizeof(int));
    gmr->error_values = malloc(gmr->num_reductions*sizeof(float));
    gmr->remaps_counts = malloc(gmr->num_reductions*sizeof(int));
    gmr->changes_counts = malloc(gmr->num_reductions*sizeof(int));

    gmr->total_remaps = 0;
    gmr->total_remap_tris = 0;
    gmr->total_changes = 0;

    for (i = 0; i < eaSize(&mr->instructions); i++)
    {
        gmr->num_tris_left[i] = mr->instructions[i]->num_tris_left;
        gmr->error_values[i] = mr->instructions[i]->error;
        gmr->remaps_counts[i] = eaiSize(&mr->instructions[i]->vremaps.remaps) / 3;
        gmr->changes_counts[i] = eaiSize(&mr->instructions[i]->vremaps.changes);

        gmr->total_remaps += gmr->remaps_counts[i];
        gmr->total_remap_tris += eaiSize(&mr->instructions[i]->vremaps.remap_tris);
        gmr->total_changes += gmr->changes_counts[i];
    }

    if (gmr->total_remaps)
    {
        gmr->remaps = malloc(3*gmr->total_remaps*sizeof(int));
    }
    if (gmr->total_remap_tris)
    {
        gmr->remap_tris = malloc(gmr->total_remap_tris*sizeof(int));
    }
    if (gmr->total_changes)
    {
        gmr->changes = malloc(gmr->total_changes*sizeof(int));
        gmr->positions = malloc(gmr->total_changes*sizeof(Vec3));
        gmr->tex1s = malloc(gmr->total_changes*sizeof(Vec2));
    }

    remaps_ptr = gmr->remaps;
    remap_tris_ptr = gmr->remap_tris;

    changes_ptr = gmr->changes;
    pos_ptr = gmr->positions;
    tex_ptr = gmr->tex1s;

    for (i = 0; i < eaSize(&mr->instructions); i++)
    {
        VertRemaps *vremaps = &mr->instructions[i]->vremaps;

        memcpy(remaps_ptr, vremaps->remaps, 3*gmr->remaps_counts[i]*sizeof(int));
        remaps_ptr += 3*gmr->remaps_counts[i];

        memcpy(remap_tris_ptr, vremaps->remap_tris, eaiSize(&vremaps->remap_tris)*sizeof(int));
        remap_tris_ptr += eaiSize(&vremaps->remap_tris);

        memcpy(changes_ptr, vremaps->changes, gmr->changes_counts[i]*sizeof(int));
        changes_ptr += gmr->changes_counts[i];

        memcpy(pos_ptr, vremaps->positions, gmr->changes_counts[i]*sizeof(Vec3));
        pos_ptr += 3*gmr->changes_counts[i];

        memcpy(tex_ptr, vremaps->tex1s, gmr->changes_counts[i]*sizeof(Vec2));
        tex_ptr += 2*gmr->changes_counts[i];
    }

    assert(remaps_ptr == gmr->remaps + 3*gmr->total_remaps);
    assert(remap_tris_ptr == gmr->remap_tris + gmr->total_remap_tris);
    assert(changes_ptr == gmr->changes + gmr->total_changes);
    assert(pos_ptr == gmr->positions + 3*gmr->total_changes);
    assert(tex_ptr == gmr->tex1s + 2*gmr->total_changes);
}

GMeshReductions *makeGMeshReductions(GMesh *mesh, F32 distances[3], Vec3 min, Vec3 max, int scale_by_area, int use_optimal_placement, int maintain_borders)
{
    int i, j, k, d = 0, debug_count = 0;
    TriCutType *tc;
    F32 rad, target_tripercent = 0.75f;
    GMesh meshcopy={0};
    GMeshReductions *gmr = calloc(sizeof(*gmr), 1);
    MeshReduction *mr = calloc(sizeof(*mr), 1);

    distances[0] = distances[1] = distances[2] = -1;
    rad = distance3(min, max) * 0.5f;

    gmeshCopy(&meshcopy, mesh, 0);
    tc = createTriCut(&meshcopy, min, max, scale_by_area, use_optimal_placement, maintain_borders);

    while (eaSize(&tc->super_edges) > 4 && !tc->super_edges[0]->connects_boundaries && !tc->super_edges[0]->is_boundary)
    {
        ReduceInstruction *ri;
        SEdge *e = tc->super_edges[0];
        ri = collapseEdge(tc, e);
        ri->num_tris_left = countLiveTris(tc);
        ri->error = inverseCostLookup(ri->cost);
        if (ri->num_tris_left < target_tripercent * mesh->tri_count)
        {
            assert(d < 3);
            distances[d++] = getDistanceForError(ri->error, rad);
            target_tripercent -= 0.25f;
        }
        eaPush(&mr->instructions, ri);
        debug_count++;
    }

    freeTriCut(tc);
    gmeshFreeData(&meshcopy);

    gmeshCopy(&meshcopy, mesh, 0);
    for (j = 0; j < eaSize(&mr->instructions); j++)
    {
        ReduceInstruction *ri = mr->instructions[j];

        for (i = 0; i < eaiSize(&ri->vremaps.remaps); i+=3)
        {
            int *tri_idxs=0;
            int old_idx = ri->vremaps.remaps[i];
            int new_idx = ri->vremaps.remaps[i+1];

            assert(old_idx >= 0);
            assert(new_idx >= 0);

            if (old_idx >= meshcopy.vert_count || new_idx >= meshcopy.vert_count)
                continue;

            for (k = 0; k < meshcopy.tri_count; k++)
            {
                GTriIdx *tri = &meshcopy.tris[k];
                if (tri->idx[0] == old_idx)
                {
                    eaiPushUnique(&tri_idxs, k);
                    tri->idx[0] = new_idx;
                }
                if (tri->idx[1] == old_idx)
                {
                    eaiPushUnique(&tri_idxs, k);
                    tri->idx[1] = new_idx;
                }
                if (tri->idx[2] == old_idx)
                {
                    eaiPushUnique(&tri_idxs, k);
                    tri->idx[2] = new_idx;
                }
            }

            ri->vremaps.remaps[i+2] = eaiSize(&tri_idxs);
            for (k = 0; k < eaiSize(&tri_idxs); k++)
                eaiPush(&ri->vremaps.remap_tris, tri_idxs[k]);
            eaiDestroy(&tri_idxs);
        }
    }
    gmeshFreeData(&meshcopy);

    convertMRtoGMR(gmr, mr);

    freeMeshReduction(mr);

    return gmr;
}

