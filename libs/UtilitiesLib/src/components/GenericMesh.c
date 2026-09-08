#include "utilitieslib/components/GenericMesh.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/memcheck.h"

#if 0
#define CHECKHEAP     {assert(heapValidateAll());}
#else
#define CHECKHEAP     {}
#endif

#define ReallocStructs(ptr, count) realloc((ptr), (count) * sizeof(*(ptr)))


// returns 0 for behind plane, 1 for on plane, 2 for in front of plane
static INLINEDBG int classifyPoint(const Vec3 pos, const Vec4 plane)
{
    float dist = distanceToPlane(pos, plane);
    if (dist > 0)
        return 2;
    return dist == 0;
}

static INLINEDBG int nearSameVert(const GMesh *mesh, int idx, const Vec3 pos, const Vec3 norm, const Vec2 tex1, const Vec2 tex2)
{
    if(mesh->usagebits & USE_NOMERGE)
        return 0; // dont merge these verts

    if (mesh->positions && pos && !nearSameVec3(mesh->positions[idx], pos))
        return 0;
    if (mesh->normals && norm && !nearSameVec3(mesh->normals[idx], norm))
        return 0;
    if (mesh->tex1s && tex1 && !nearSameVec2(mesh->tex1s[idx], tex1))
        return 0;
    if (mesh->tex2s && tex2 && !nearSameVec2(mesh->tex2s[idx], tex2))
        return 0;
    return 1;
}

static INLINEDBG int sameTri(const GMesh *mesh, int tri_idx, int idx0, int idx1, int idx2, int tex_id)
{
    GTriIdx *tri = &mesh->tris[tri_idx];
    if (tri->tex_id != tex_id)
        return 0;
    if (tri->idx[0] == idx0 && tri->idx[1] == idx1 && tri->idx[2] == idx2)
        return 1;
    if (tri->idx[1] == idx0 && tri->idx[2] == idx1 && tri->idx[0] == idx2)
        return 1;
    if (tri->idx[2] == idx0 && tri->idx[0] == idx1 && tri->idx[1] == idx2)
        return 1;
    return 0;
}

static INLINEDBG int isDegenerate(Vec3 a, Vec3 b, Vec3 c)
{
    int count = 0;

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

void gmeshSetUsageBits(GMesh *mesh, int bits)
{
    if (mesh->usagebits == bits)
        return;

    mesh->usagebits = bits;

    if (!mesh->vert_max)
        return;

    if (!(bits & USE_POSITIONS))
    {
        SAFE_FREE(mesh->positions);
    }
    else if (!mesh->positions)
    {
        mesh->positions = ReallocStructs(mesh->positions, mesh->vert_max);
    }

    if (!(bits & USE_NORMALS))
    {
        SAFE_FREE(mesh->normals);
    }
    else if (!mesh->normals)
    {
        mesh->normals = ReallocStructs(mesh->normals, mesh->vert_max);
    }

    if (!(bits & USE_TEX1S))
    {
        SAFE_FREE(mesh->tex1s);
    }
    else if (!mesh->tex1s)
    {
        mesh->tex1s = ReallocStructs(mesh->tex1s, mesh->vert_max);
    }

    if (!(bits & USE_TEX2S))
    {
        SAFE_FREE(mesh->tex2s);
    }
    else if (!mesh->tex2s)
    {
        mesh->tex2s = ReallocStructs(mesh->tex2s, mesh->vert_max);
    }

    if (!(bits & USE_BONEWEIGHTS))
    {
        SAFE_FREE(mesh->boneweights);
        SAFE_FREE(mesh->bonemats);
    }
    else if (!mesh->boneweights)
    {
        mesh->boneweights = ReallocStructs(mesh->boneweights, mesh->vert_max);
        mesh->bonemats = ReallocStructs(mesh->bonemats, mesh->vert_max);
    }
}

void gmeshSetVertCount(GMesh *mesh, int vcount)
{
    if (vcount > mesh->vert_max)
    {
        int last_vmax = mesh->vert_max;
        mesh->vert_max = vcount << 1;

        if (mesh->usagebits & USE_POSITIONS)
        {
            mesh->positions = ReallocStructs(mesh->positions, mesh->vert_max);
            ZeroStructs(&mesh->positions[last_vmax], mesh->vert_max - last_vmax);
        }

        if (mesh->usagebits & USE_NORMALS)
        {
            mesh->normals = ReallocStructs(mesh->normals, mesh->vert_max);
            ZeroStructs(&mesh->normals[last_vmax], mesh->vert_max - last_vmax);
        }

        if (mesh->usagebits & USE_TEX1S)
        {
            mesh->tex1s = ReallocStructs(mesh->tex1s, mesh->vert_max);
            ZeroStructs(&mesh->tex1s[last_vmax], mesh->vert_max - last_vmax);
        }

        if (mesh->usagebits & USE_TEX2S)
        {
            mesh->tex2s = ReallocStructs(mesh->tex2s, mesh->vert_max);
            ZeroStructs(&mesh->tex2s[last_vmax], mesh->vert_max - last_vmax);
        }

        if (mesh->usagebits & USE_BONEWEIGHTS)
        {
            mesh->boneweights = ReallocStructs(mesh->boneweights, mesh->vert_max);
            ZeroStructs(&mesh->boneweights[last_vmax], mesh->vert_max - last_vmax);

            mesh->bonemats = ReallocStructs(mesh->bonemats, mesh->vert_max);
            ZeroStructs(&mesh->bonemats[last_vmax], mesh->vert_max - last_vmax);
        }
    }

    mesh->vert_count = vcount;

    if (mesh->grid.cell)
        polyGridFree(&mesh->grid);
}

void gmeshSetTriCount(GMesh *mesh, int tcount)
{
    if (tcount > mesh->tri_max)
    {
        int last_tmax = mesh->tri_max;
        mesh->tri_max = tcount << 1;

        mesh->tris = ReallocStructs(mesh->tris, mesh->tri_max);
        ZeroStructs(&mesh->tris[last_tmax], mesh->tri_max - last_tmax);
    }

    mesh->tri_count = tcount;

    if (mesh->grid.cell)
        polyGridFree(&mesh->grid);
}

static int gmeshAddVertInternal(GMesh *mesh, const Vec3 pos, const Vec3 norm, const Vec2 tex1, const Vec2 tex2, const Vec4 boneweight, const GBoneMats bonemat, int check_exists, PolyGrid *grid, int origcount)
{
    int i, j, idx;

    if (check_exists)
    {
        if (grid && grid->cell && pos)
        {
            PolyCell    *pcell;
            GTriIdx        *tri;
            Vec3        dv;
            F32            size;

            subVec3(pos, grid->pos, dv);
            pcell = polyGridFindCell(grid, dv, &size);
            for (i = 0; pcell && i < pcell->tri_count; i++)
            {
                tri = &mesh->tris[pcell->tri_idxs[i]];
                for(j=0;j<3;j++)
                {
                    idx = tri->idx[j];
                    if (idx >= 0 && nearSameVert(mesh, idx, pos, norm, tex1, tex2))
                        return idx;
                }
            }
        }
        else
        {
            origcount = 0;
        }

        for (i = origcount; i < mesh->vert_count; i++)
        {
            if (nearSameVert(mesh, i, pos, norm, tex1, tex2))
                return i;
        }
    }

    if (mesh->grid.cell)
        polyGridFree(&mesh->grid);

    idx = mesh->vert_count;
    gmeshSetVertCount(mesh, mesh->vert_count + 1);
    if (mesh->positions && pos)
        copyVec3(pos, mesh->positions[idx]);
    if (mesh->normals && norm)
        copyVec3(norm, mesh->normals[idx]);
    if (mesh->tex1s && tex1)
        copyVec2(tex1, mesh->tex1s[idx]);
    if (mesh->tex2s && tex2)
        copyVec2(tex2, mesh->tex2s[idx]);
    if (mesh->boneweights && boneweight)
        copyVec4(boneweight, mesh->boneweights[idx]);
    if (mesh->bonemats && bonemat)
        memcpy(mesh->bonemats[idx], bonemat, sizeof(GBoneMats));
    return idx;
}

int gmeshAddVert(GMesh *mesh, const Vec3 pos, const Vec3 norm, const Vec2 tex1, const Vec2 tex2, const Vec4 boneweight, const GBoneMats bonemat, int check_exists)
{
    return gmeshAddVertInternal(mesh, pos, norm, tex1, tex2, boneweight, bonemat, check_exists, &mesh->grid, mesh->vert_count);
}

int gmeshAddInterpolatedVert(GMesh *mesh, int idx1, int idx2, F32 ratio_1to2)
{
    F32 ratio_2to1 = 1.f - ratio_1to2;
    Vec3 pos, norm;
    Vec2 tex1, tex2;

    if (mesh->positions)
    {
        scaleVec3(mesh->positions[idx1], ratio_1to2, pos);
        scaleAddVec3(mesh->positions[idx2], ratio_2to1, pos, pos);
    }
    if (mesh->normals)
    {
        scaleVec3(mesh->normals[idx1], ratio_1to2, norm);
        scaleAddVec3(mesh->normals[idx2], ratio_2to1, norm, norm);
    }
    if (mesh->tex1s)
    {
        scaleVec2(mesh->tex1s[idx1], ratio_1to2, tex1);
        scaleAddVec2(mesh->tex1s[idx2], ratio_2to1, tex1, tex1);
    }
    if (mesh->tex2s)
    {
        scaleVec2(mesh->tex2s[idx1], ratio_1to2, tex2);
        scaleAddVec2(mesh->tex2s[idx2], ratio_2to1, tex2, tex2);
    }

    return gmeshAddVert(mesh, 
        mesh->positions?pos:0, 
        mesh->normals?norm:0, 
        mesh->tex1s?tex1:0, 
        mesh->tex2s?tex2:0, 
        mesh->boneweights?mesh->boneweights[idx1]:0, 
        mesh->bonemats?mesh->bonemats[idx1]:0,
        0);
}

int gmeshAddTri(GMesh *mesh, int idx0, int idx1, int idx2, int tex_id, int check_exists)
{
    int i, idx;

    if (check_exists)
    {
        for (i = 0; i < mesh->tri_count; i++)
        {
            if (sameTri(mesh, i, idx0, idx1, idx2, tex_id))
                return i;
        }
    }

    if (mesh->grid.cell)
        polyGridFree(&mesh->grid);

    idx = mesh->tri_count;
    gmeshSetTriCount(mesh, mesh->tri_count + 1);
    mesh->tris[idx].idx[0] = idx0;
    mesh->tris[idx].idx[1] = idx1;
    mesh->tris[idx].idx[2] = idx2;
    mesh->tris[idx].tex_id = tex_id;
    return idx;
}

void gmeshRemoveTri(GMesh *mesh, int tri_idx)
{
    if (tri_idx >= mesh->tri_count || tri_idx < 0)
        return;

    if (mesh->grid.cell)
        polyGridFree(&mesh->grid);

    memcpy(mesh->tris + tri_idx, mesh->tris + tri_idx + 1, sizeof(*mesh->tris) * (mesh->tri_count - tri_idx - 1));
    mesh->tri_count--;
}

void gmeshMarkBadTri(GMesh *mesh, int tri_idx)
{
    mesh->tris[tri_idx].idx[0] = -1;
    mesh->tris[tri_idx].idx[1] = -1;
    mesh->tris[tri_idx].idx[2] = -1;
    mesh->tris[tri_idx].tex_id = -1;
}

void gmeshRemapVertex(GMesh *mesh, int old_idx, int new_idx)
{
    int i;

    if (old_idx >= mesh->vert_count || old_idx < 0 || new_idx >= mesh->vert_count || new_idx < 0)
        return;

    if (mesh->grid.cell)
        polyGridFree(&mesh->grid);

    for (i = 0; i < mesh->tri_count; i++)
    {
        GTriIdx *tri = &mesh->tris[i];
        if (tri->idx[0] == old_idx)
            tri->idx[0] = new_idx;
        if (tri->idx[1] == old_idx)
            tri->idx[1] = new_idx;
        if (tri->idx[2] == old_idx)
            tri->idx[2] = new_idx;
    }
}

void gmeshRemapTriVertex(GMesh *mesh, int tri_idx, int old_idx, int new_idx)
{
    GTriIdx *tri;

    if (old_idx >= mesh->vert_count || old_idx < 0 || new_idx >= mesh->vert_count || new_idx < 0)
        return;

    if (mesh->grid.cell)
        polyGridFree(&mesh->grid);

    tri = &mesh->tris[tri_idx];
    if (tri->idx[0] == old_idx)
        tri->idx[0] = new_idx;
    if (tri->idx[1] == old_idx)
        tri->idx[1] = new_idx;
    if (tri->idx[2] == old_idx)
        tri->idx[2] = new_idx;
}

void gmeshFreeData(GMesh *mesh)
{
    SAFE_FREE(mesh->positions);
    SAFE_FREE(mesh->normals);
    SAFE_FREE(mesh->tex1s);
    SAFE_FREE(mesh->tex2s);
    SAFE_FREE(mesh->boneweights);
    SAFE_FREE(mesh->bonemats);
    SAFE_FREE(mesh->tris);
    if (mesh->grid.cell)
        polyGridFree(&mesh->grid);
    ZeroStruct(mesh);
}

void gmeshCopy(GMesh *dst, const GMesh *src, int fill_grid)
{
    gmeshFreeData(dst);
    gmeshSetUsageBits(dst, src->usagebits);
    gmeshSetTriCount(dst, src->tri_count);
    gmeshSetVertCount(dst, src->vert_count);
    if (src->positions)
        CopyStructs(dst->positions, src->positions, src->vert_count);
    if (src->normals)
        CopyStructs(dst->normals, src->normals, src->vert_count);
    if (src->tex1s)
        CopyStructs(dst->tex1s, src->tex1s, src->vert_count);
    if (src->tex2s)
        CopyStructs(dst->tex2s, src->tex2s, src->vert_count);
    if (src->boneweights)
        CopyStructs(dst->boneweights, src->boneweights, src->vert_count);
    if (src->bonemats)
        CopyStructs(dst->bonemats, src->bonemats, src->vert_count);
    CopyStructs(dst->tris, src->tris, src->tri_count);
    if (fill_grid)
        gmeshUpdateGrid(dst, 0);
}

// appends tris from src to dst
void gmeshMerge(GMesh *dst, const GMesh *src, int pool_verts, int pool_tris)
{
    int i, origcount = dst->vert_count;
    PolyGrid grid={0};

    if (pool_verts)
    {
        CopyStructs(&grid, &dst->grid, 1);
        ZeroStruct(&dst->grid);
    }

    gmeshSetUsageBits(dst, dst->usagebits | src->usagebits);

    for (i = 0; i < src->tri_count; i++)
    {
        int idx0, idx1, idx2;
        GTriIdx *srctri = &src->tris[i];
        int i0 = srctri->idx[0], i1 = srctri->idx[1], i2 = srctri->idx[2];

        if (i0 < 0 || i1 < 0 || i2 < 0)
            continue;

        idx0 = gmeshAddVertInternal(dst,
            src->positions?src->positions[i0]:0,
            src->normals?src->normals[i0]:0,
            src->tex1s?src->tex1s[i0]:0,
            src->tex2s?src->tex2s[i0]:0,
            src->boneweights?src->boneweights[i0]:0,
            src->bonemats?src->bonemats[i0]:0,
            pool_verts, &grid, origcount);
        idx1 = gmeshAddVertInternal(dst,
            src->positions?src->positions[i1]:0,
            src->normals?src->normals[i1]:0,
            src->tex1s?src->tex1s[i1]:0,
            src->tex2s?src->tex2s[i1]:0,
            src->boneweights?src->boneweights[i1]:0,
            src->bonemats?src->bonemats[i1]:0,
            pool_verts, &grid, origcount);
        idx2 = gmeshAddVertInternal(dst,
            src->positions?src->positions[i2]:0,
            src->normals?src->normals[i2]:0,
            src->tex1s?src->tex1s[i2]:0,
            src->tex2s?src->tex2s[i2]:0,
            src->boneweights?src->boneweights[i2]:0,
            src->bonemats?src->bonemats[i2]:0,
            pool_verts, &grid, origcount);
        gmeshAddTri(dst, idx0, idx1, idx2, srctri->tex_id, pool_tris);

        if (pool_verts && dst->vert_count > origcount + 800)
        {
            origcount = dst->vert_count;
            polyGridFree(&grid);
            gridPolys(&grid, dst);
        }
    }

    if (pool_verts)
        polyGridFree(&grid);

    if (dst->positions)
        gridPolys(&dst->grid, dst);
}

void gmeshUpdateGrid(GMesh *mesh, int pool_tris)
{
    GMesh    simple={0};

    polyGridFree(&mesh->grid);
    if (!mesh->positions)
        return;
    gridPolys(&mesh->grid, mesh);

    gmeshMerge(&simple, mesh, 1, pool_tris);
    gmeshFreeData(mesh);
    *mesh = simple;
    ZeroStruct(&simple);

    gmeshMerge(&simple, mesh, 1, pool_tris);
    gmeshFreeData(mesh);
    *mesh = simple;
}

static int s_warnIfFullyDegenerate = 0; // dont warn by default (tools may want to enable this warning)
int gmeshWarnIfFullyDegenerate(int warn)
{
    int oldval = s_warnIfFullyDegenerate;
    s_warnIfFullyDegenerate = warn;
    return oldval;
}

int gmeshMarkDegenerateTris(GMesh *mesh)
{
    int i, found = 0;

    for (i = 0; i < mesh->tri_count; i++)
    {
        int i0, i1, i2;

        i0 = mesh->tris[i].idx[0];
        i1 = mesh->tris[i].idx[1];
        i2 = mesh->tris[i].idx[2];

        if (i0 < 0 || i1 < 0 || i2 < 0)
        {
            found++;
            continue;
        }

        if (isDegenerate(mesh->positions[i0], mesh->positions[i1], mesh->positions[i2]))
        {
            // check to make sure we haven't marked the entire mesh degenerate.  Leave the last
            //    triangle intact if that's the case, so that downstream code has something to 
            //    process.  Report error though so that it can be fixed.
            if( i == found && i == (mesh->tri_count-1) ) {
                if(s_warnIfFullyDegenerate)
                    Errorf("ERROR: All triangles are degenerate, mesh is not valid!");
                continue;
            }
            gmeshMarkBadTri(mesh, i);
            found++;
        }
    }

    return found;
}

static void split(GMesh *mesh, GTriIdx *src, GTriIdx *out1, int i0, int i1, int i2, const Vec4 plane)
{
    // point i0 is on the plane
    float pval1, pval2, ratio_12;
    int i3;
    pval1 = distanceToPlane(mesh->positions[i1], plane);
    pval2 = distanceToPlane(mesh->positions[i2], plane);

    ratio_12 = pval1 / (pval1 - pval2);

    CHECKHEAP;
    i3 = gmeshAddInterpolatedVert(mesh, i1, i2, ratio_12);
    CHECKHEAP;

    // make this tri on the front of the plane
    if (pval1 > 0)
    {
        src->idx[0] = i0;
        src->idx[1] = i1;
        src->idx[2] = i3;

        out1->idx[0] = i0;
        out1->idx[1] = i3;
        out1->idx[2] = i2;
    }
    else
    {
        src->idx[0] = i0;
        src->idx[1] = i3;
        src->idx[2] = i2;

        out1->idx[0] = i0;
        out1->idx[1] = i1;
        out1->idx[2] = i3;
    }
    CHECKHEAP;
}

static void split2(GMesh *mesh, GTriIdx *src, GTriIdx *out1, GTriIdx *out2, int i0, int i1, int i2, const Vec4 plane)
{
    // point i0 is alone on one side of the plane
    float pval0, pval1, pval2, ratio_01, ratio_02;
    int i3, i4;
    pval0 = distanceToPlane(mesh->positions[i0], plane);
    pval1 = distanceToPlane(mesh->positions[i1], plane);
    pval2 = distanceToPlane(mesh->positions[i2], plane);

    ratio_01 = pval0 / (pval0 - pval1);
    ratio_02 = pval0 / (pval0 - pval2);

    CHECKHEAP;
    i3 = gmeshAddInterpolatedVert(mesh, i0, i1, ratio_01);
    i4 = gmeshAddInterpolatedVert(mesh, i0, i2, ratio_02);
    CHECKHEAP;

    // make this tri on the front of the plane
    if (pval0 > 0)
    {
        src->idx[0] = i0;
        src->idx[1] = i3;
        src->idx[2] = i4;

        out1->idx[0] = i3;
        out1->idx[1] = i1;
        out1->idx[2] = i4;

        out2->idx[0] = i1;
        out2->idx[1] = i2;
        out2->idx[2] = i4;
    }
    else
    {
        out1->idx[0] = i0;
        out1->idx[1] = i3;
        out1->idx[2] = i4;

        src->idx[0] = i3;
        src->idx[1] = i1;
        src->idx[2] = i4;

        out2->idx[0] = i1;
        out2->idx[1] = i2;
        out2->idx[2] = i4;
    }
    CHECKHEAP;
}

// return 1 if the triangle was split
int gmeshSplitTri(GMesh *mesh, const Vec4 plane, int tri_idx)
{
    int tri_idx1, tri_idx2, v0, v1, v2, c0, c1, c2, total;

    if (tri_idx >= mesh->tri_count || tri_idx < 0)
        return 0;

    v0 = mesh->tris[tri_idx].idx[0];
    v1 = mesh->tris[tri_idx].idx[1];
    v2 = mesh->tris[tri_idx].idx[2];
    c0 = classifyPoint(mesh->positions[v0], plane);
    c1 = classifyPoint(mesh->positions[v1], plane);
    c2 = classifyPoint(mesh->positions[v2], plane);

    // check if all on same side of plane
    if (c0 == c1 && c1 == c2)
        return 0;

    total = c0 + c1 + c2;

    if (c0 == 1 || c1 == 1 || c2 == 1)
    {
        // order does not matter
        switch (total)
        {
        case 1: // 0, 0, 1
        case 2: // 0, 1, 1
        case 4: // 1, 1, 2
        case 5: // 1, 2, 2
            return 0;
        }

        // 0, 1, 2
        // we need one new triangle
        tri_idx1 = gmeshAddTri(mesh, 0, 0, 0, mesh->tris[tri_idx].tex_id, 0);

        // find the on plane point:
        if (c0 == 1)
            split(mesh, &mesh->tris[tri_idx], &mesh->tris[tri_idx1], v0, v1, v2, plane);
        else if (c1 == 1)
            split(mesh, &mesh->tris[tri_idx], &mesh->tris[tri_idx1], v1, v2, v0, plane);
        else
            split(mesh, &mesh->tris[tri_idx], &mesh->tris[tri_idx1], v2, v0, v1, plane);
    }
    else
    {
        // 0, 0, 2
        // 0, 2, 2
        // we need two new triangles
        tri_idx1 = gmeshAddTri(mesh, 0, 0, 0, mesh->tris[tri_idx].tex_id, 0);
        tri_idx2 = gmeshAddTri(mesh, 0, 0, 0, mesh->tris[tri_idx].tex_id, 0);

        if (total == 2)
        {
            // find the on plane side point
            if (c0 == 2)
                split2(mesh, &mesh->tris[tri_idx], &mesh->tris[tri_idx1], &mesh->tris[tri_idx2], v0, v1, v2, plane);
            else if (c1 == 2)
                split2(mesh, &mesh->tris[tri_idx], &mesh->tris[tri_idx1], &mesh->tris[tri_idx2], v1, v2, v0, plane);
            else
                split2(mesh, &mesh->tris[tri_idx], &mesh->tris[tri_idx1], &mesh->tris[tri_idx2], v2, v0, v1, plane);
        }
        else // total == 4
        {
            // find the off plane side point
            if (c0 == 0)
                split2(mesh, &mesh->tris[tri_idx], &mesh->tris[tri_idx1], &mesh->tris[tri_idx2], v0, v1, v2, plane);
            else if (c1 == 0)
                split2(mesh, &mesh->tris[tri_idx], &mesh->tris[tri_idx1], &mesh->tris[tri_idx2], v1, v2, v0, plane);
            else
                split2(mesh, &mesh->tris[tri_idx], &mesh->tris[tri_idx1], &mesh->tris[tri_idx2], v2, v0, v1, plane);
        }
    }

    return 1;
}

void gmeshSortTrisByTexID(GMesh *mesh, int (*cmp) (const void *, const void *))
{
    int        *temp_ids,tc=0,i,j,tricount=0;
    GTriIdx    *temp_tris;

    temp_tris = calloc(sizeof(GTriIdx),mesh->tri_count);
    temp_ids = calloc(sizeof(int),mesh->tri_count);
    for(i=0;i<mesh->tri_count;i++)
    {
        for(j=0;j<tc;j++)
        {
            if (mesh->tris[i].tex_id == temp_ids[j])
                break;
        }
        if (j >= tc)
            temp_ids[tc++] = mesh->tris[i].tex_id;
    }

    if (cmp)
        qsort(temp_ids,tc,sizeof(int),cmp);

    for(i=0;i<tc;i++)
    {
        for(j=0;j<mesh->tri_count;j++)
        {
            if (temp_ids[i] == mesh->tris[j].tex_id)
            {
                temp_tris[tricount++] = mesh->tris[j];
            }
        }
    }
    assert(tricount == mesh->tri_count);

    memcpy(mesh->tris,temp_tris,sizeof(GTriIdx) * mesh->tri_count);
    free(temp_tris);
    free(temp_ids);
}

//////////////////////////////////////////////////////////////////////////

float gmeshReduce(GMesh *dst, const GMesh *src, const GMeshReductions *reductions, float target_error, ReductionMethod method, F32 shrink_amount)
{
    int i, j, r, tri_count, target_tris;
    float last_error = -1;
    GMesh tempmesh={0};

    int *remaps_ptr = reductions->remaps;
    int *remap_tris_ptr = reductions->remap_tris;

    int *changes_ptr = reductions->changes;
    F32 *pos_ptr = reductions->positions;
    F32 *tex_ptr = reductions->tex1s;

    assertmsg(method==TRICOUNT_RMETHOD || method==ERROR_RMETHOD, "gmeshReduce: Unknown triangle decimation method!");

    if (target_error > 1)
        target_error = 1;
    if (target_error < 0)
        target_error = 0;

    ZeroStruct(dst);
    gmeshCopy(&tempmesh, src, 0);

    tri_count = tempmesh.tri_count;
    target_tris = (int) (tempmesh.tri_count * (1.f - target_error));

    for (r = 0; r < reductions->num_reductions; r++)
    {
        if (method == TRICOUNT_RMETHOD && tri_count <= target_tris)
            break;
        if (method == ERROR_RMETHOD && reductions->error_values[r] > (target_error+0.0001f))
            break;

        // triangle remaps
        for (i = 0; i < reductions->remaps_counts[r]; i++)
        {
            int old_idx = *(remaps_ptr++);
            int new_idx = *(remaps_ptr++);
            int num_remap_tris = *(remaps_ptr++);
            for (j = 0; j < num_remap_tris; j++)
                gmeshRemapTriVertex(&tempmesh, *(remap_tris_ptr++), old_idx, new_idx);
        }

        // vertex changes
        for (i = 0; i < reductions->changes_counts[r]; i++)
        {
            int vert_idx = *(changes_ptr++);
            if (tempmesh.usagebits & USE_POSITIONS)
                copyVec3(pos_ptr, tempmesh.positions[vert_idx]);
            if (tempmesh.usagebits & USE_TEX1S)
                copyVec2(tex_ptr, tempmesh.tex1s[vert_idx]);
            pos_ptr += 3;
            tex_ptr += 2;
        }

        last_error = reductions->error_values[r];
        tri_count = reductions->num_tris_left[r];
    }

    //////////////////////////////////////////////////////////////////////////
    // remove degenerate triangles
    polyGridFree(&tempmesh.grid);
    gmeshMarkDegenerateTris(&tempmesh);

    //////////////////////////////////////////////////////////////////////////
    // merge into destination shape
    gmeshMerge(dst, &tempmesh, 1, 0);
    gmeshFreeData(&tempmesh);

    if (shrink_amount && (dst->usagebits & USE_POSITIONS) && (dst->usagebits & USE_NORMALS))
    {
        shrink_amount = (F32)fabs(shrink_amount);
        for (i = 0; i < dst->vert_count; i++)
        {
            scaleAddVec3(dst->normals[i], -shrink_amount, dst->positions[i], dst->positions[i]);
        }
    }

    return last_error;
}

void freeGMeshReductions(GMeshReductions *gmr)
{
    if (!gmr)
        return;

    SAFE_FREE(gmr->num_tris_left);
    SAFE_FREE(gmr->error_values);
    SAFE_FREE(gmr->remaps_counts);
    SAFE_FREE(gmr->changes_counts);
    SAFE_FREE(gmr->remaps);
    SAFE_FREE(gmr->remap_tris);
    SAFE_FREE(gmr->changes);
    SAFE_FREE(gmr->positions);
    SAFE_FREE(gmr->tex1s);
    SAFE_FREE(gmr);
}



