#ifndef _GENERICMESH_H_
#define _GENERICMESH_H_

#include "../stdtypes.h"
#include "../utils/mathutil.h"
#include "gridpoly.h"

#ifdef __cplusplus
    extern "C" {
#endif

enum
{
    USE_POSITIONS    = 1<<0,
    USE_NORMALS        = 1<<1,
    USE_TEX1S        = 1<<2,
    USE_TEX2S        = 1<<3,
    USE_BONEWEIGHTS    = 1<<4,
    USE_NOMERGE        = 1<<5,
};

typedef struct GTriIdx
{
    int idx[3];
    int tex_id;
} GTriIdx;

typedef U8 GBoneMats[4];

typedef struct GMesh
{
    int            usagebits;

    int            vert_count, vert_max;
    Vec3        *positions;
    Vec3        *normals;
    Vec2        *tex1s;
    Vec2        *tex2s;
    Vec4        *boneweights;
    GBoneMats    *bonemats;

    int            tri_count, tri_max;
    GTriIdx        *tris;

    PolyGrid    grid;

} GMesh;

typedef struct GMeshReductions
{
    int num_reductions;

    int *num_tris_left;  // one for each reduction step
    float *error_values; // one for each reduction step
    int *remaps_counts;  // number of tri remaps for each reduction step
    int *changes_counts; // number of vert changes for each reduction step

    // tri remaps
    int total_remaps;
    int *remaps; // sequence of [oldidx,newidx,numtris]
    int total_remap_tris;
    int *remap_tris;

    // vert changes
    int total_changes;
    int *changes; // vert idxs
    F32 *positions; // Vec3 array
    F32 *tex1s; // Vec2 array

} GMeshReductions;

typedef enum { TRICOUNT_RMETHOD, ERROR_RMETHOD } ReductionMethod;

//////////////////////////////////////////////////////////////////////////

void gmeshSetUsageBits(GMesh *mesh, int bits);
void gmeshSetVertCount(GMesh *mesh, int vcount);
void gmeshSetTriCount(GMesh *mesh, int tcount);

int gmeshAddVert(GMesh *mesh, const Vec3 pos, const Vec3 norm, const Vec2 tex1, const Vec2 tex2, const Vec2 boneweight, const GBoneMats bonemat, int check_exists);
int gmeshAddInterpolatedVert(GMesh *mesh, int idx1, int idx2, F32 ratio_1to2);
int gmeshAddTri(GMesh *mesh, int idx0, int idx1, int idx2, int tex_id, int check_exists);

int gmeshSplitTri(GMesh *mesh, const Vec4 plane, int tri_idx); // returns 1 if triangle was split
void gmeshRemoveTri(GMesh *mesh, int tri_idx);
void gmeshMarkBadTri(GMesh *mesh, int tri_idx);
int gmeshMarkDegenerateTris(GMesh *mesh);
void gmeshSortTrisByTexID(GMesh *mesh, int (*cmp)(const void *, const void *));
void gmeshRemapVertex(GMesh *mesh, int old_idx, int new_idx);
void gmeshRemapTriVertex(GMesh *mesh, int tri_idx, int old_idx, int new_idx);

void gmeshCopy(GMesh *dst, const GMesh *src, int fill_grid);
void gmeshMerge(GMesh *dst, const GMesh *src, int pool_verts, int pool_tris);
void gmeshFreeData(GMesh *mesh);

void gmeshUpdateGrid(GMesh *mesh, int pool_tris);

// returns last error
float gmeshReduce(GMesh *dst, const GMesh *src, const GMeshReductions *reductions, float desired_error, ReductionMethod method, F32 shrink_amount);
void freeGMeshReductions(GMeshReductions *gmr);

int gmeshWarnIfFullyDegenerate(int warn);

static INLINEDBG F32 gmeshGetTriArea(const GMesh *mesh, int tri_idx)
{
    Vec3 a, b, n;
    GTriIdx *tri = &mesh->tris[tri_idx];
    subVec3(mesh->positions[tri->idx[1]], mesh->positions[tri->idx[0]], a);
    subVec3(mesh->positions[tri->idx[2]], mesh->positions[tri->idx[0]], b);
    crossVec3(a, b, n);
    return 0.5f * lengthVec3(n);
}

#ifdef __cplusplus
    }
#endif

#endif//_GENERICMESH_H_

