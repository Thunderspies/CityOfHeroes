#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/qsortG.h>
#include "gridcoll/tritri_isectline.h"
#include "clip.h"
#include "taucs.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

float dp3(Vec3 a, Vec3 b)
{
    return dotVec3(a, b);
}

static void    calcFaceNormal(Vec3 v0,Vec3 v1,Vec3 v2,Vec3 norm)
{
    Vec3    tvec1,tvec2;

    subVec3(v1,v0,tvec1);
    subVec3(v2,v1,tvec2);
    crossVec3(tvec1,tvec2,norm);
    normalVec3(norm);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include "uvpack.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static int cpCompare(const void* cp1Data, const void* cp2Data)
{
    const CombinedPoly ** cp1 = (const CombinedPoly **)cp1Data;
    const CombinedPoly ** cp2 = (const CombinedPoly **)cp2Data;
    // biggest first
    if ((*cp1)->areaWithBorder > (*cp2)->areaWithBorder)
        return -1;
    return (*cp1)->areaWithBorder < (*cp2)->areaWithBorder;
}

static int primCompare(const void* p1Data, const void* p2Data)
{
    const Prim ** p1 = (const Prim **)p1Data;
    const Prim ** p2 = (const Prim **)p2Data;
#if 0
    // biggest first in the list, so smallest will be popped off first
    if ((*p1)->area > (*p2)->area)
        return -1;
    return (*p1)->area < (*p2)->area;
#else
    // smallest first in the list, so biggest will be popped off first
    if ((*p1)->area < (*p2)->area)
        return -1;
    return (*p1)->area > (*p2)->area;
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

typedef struct _DynVec3Array
{
    Vec3 *vecs;
    int count;
    int max;
} DynVec3Array;

__inline static void dvaFreeData(DynVec3Array *dva)
{
    if (dva->vecs)
        free(dva->vecs);
    memset(dva, 0, sizeof(*dva));
}

__inline static void dvaReset(DynVec3Array *dva)
{
    dva->count = 0;
}

static int dvaFindVec3(DynVec3Array *dva, Vec3 v)
{
    int i;

    for (i = 0; i < dva->count; i++)
    {
        if (dva->vecs[i][0] == v[0] && dva->vecs[i][1] == v[1] && dva->vecs[i][2] == v[2])
            return i;
    }

    return -1;
}

__inline static void dvaAdd(DynVec3Array *dva, Vec3 v)
{
    float *vnew;

    if (dvaFindVec3(dva, v) >= 0)
        return;
    
    vnew = dynArrayAdd((void **)&dva->vecs, sizeof(Vec3), &dva->count, &dva->max, 1);
    copyVec3(v, vnew);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


__inline static int uvsValid(Vec2 uvs[3])
{
    return FINITE(uvs[0][0]) && FINITE(uvs[0][1]) && FINITE(uvs[1][0]) && FINITE(uvs[1][1]) && FINITE(uvs[2][0]) && FINITE(uvs[2][1]);
}

__inline static int sizeValid(Vec2 mn, Vec2 mx, Vec2 mn2, Vec2 mx2)
{
    float difx = mx[0]-mn[0];
    float dify = mx[1]-mn[1];
    float difx2 = mx2[0]-mn2[0];
    float dify2 = mx2[1]-mn2[1];

    if (difx2 > difx && difx2 > MAXCPSIZE)
        return 0;

    if (dify2 > dify && dify2 > MAXCPSIZE)
        return 0;

    return 1;
}

static int remap(Prim *src, Prim *found, int eidx, Vec2 uvs[3], float *scale_out, double *angle)
{
    int idx1_0, idx1_1, idx2_0, idx2_1, idx2_2;

    *angle = 0;

    *scale_out = 1;

    if (eidx == 0)
    {
        idx1_0 = 0;
        idx1_1 = 1;
    }
    else if (eidx == 1)
    {
        idx1_0 = 1;
        idx1_1 = 2;
    }
    else
    {
        idx1_0 = 2;
        idx1_1 = 0;
    }

    if (found->neighbors[0] == src)
    {
        idx2_0 = 1;
        idx2_1 = 0;
        idx2_2 = 2;
    }
    else if (found->neighbors[1] == src)
    {
        idx2_0 = 2;
        idx2_1 = 1;
        idx2_2 = 0;
    }
    else
    {
        idx2_0 = 0;
        idx2_1 = 2;
        idx2_2 = 1;
    }


    {
        DVec2 fvec, svec;
        Vec2 tvec;
        float scale = 1;

        subVec2(found->uv[idx2_1], found->uv[idx2_0], fvec);
        subVec2(src->uv[idx1_1], src->uv[idx1_0], svec);

        // scale
        scale = sqrt(dotVec2(svec, svec)) / sqrt(dotVec2(fvec, fvec));
#if 0
        if (nearSameF32(scale, 1))
            scale = 1;
#endif
        *scale_out = scale;

        uvs[idx2_0][0] = found->uv[idx2_0][0] * scale;
        uvs[idx2_1][0] = found->uv[idx2_1][0] * scale;
        uvs[idx2_2][0] = found->uv[idx2_2][0] * scale;
        uvs[idx2_0][1] = found->uv[idx2_0][1] * scale;
        uvs[idx2_1][1] = found->uv[idx2_1][1] * scale;
        uvs[idx2_2][1] = found->uv[idx2_2][1] * scale;

#if 0
        if (!nearSameDVec2(fvec, svec))
#endif
        {
            DVec2 m0, m1, tvec, tempvec, tempvec2;

            normalDVec2(fvec);
            normalDVec2(svec);
            *angle = acos(dotVec2(fvec, svec));

            // set up rotation matrix
            m0[0] = cos(*angle);    m1[0] = sin(*angle);
            m0[1] = -sin(*angle);    m1[1] = cos(*angle);

            // make sure the rotation is going the correct direction
            mulDVecMat2(m0, m1, fvec, tvec);
            if (!nearSameDVec2(tvec, svec))
            {
                *angle = -*angle;
                m1[0] = -m1[0];
                m0[1] = -m0[1];
            }

            // rotate
            copyVec2(uvs[idx2_0], tempvec2);
            mulDVecMat2(m0, m1, tempvec2, tempvec);
            copyVec2(tempvec, uvs[idx2_0]);

            copyVec2(uvs[idx2_1], tempvec2);
            mulDVecMat2(m0, m1, tempvec2, tempvec);
            copyVec2(tempvec, uvs[idx2_1]);

            copyVec2(uvs[idx2_2], tempvec2);
            mulDVecMat2(m0, m1, tempvec2, tempvec);
            copyVec2(tempvec, uvs[idx2_2]);
        }

        // offset
        subVec2(uvs[idx2_0], src->uv[idx1_0], tvec);
        subVec2(uvs[idx2_0], tvec, uvs[idx2_0]);
        subVec2(uvs[idx2_1], tvec, uvs[idx2_1]);
        subVec2(uvs[idx2_2], tvec, uvs[idx2_2]);

#if 0
        copyVec2(src->uv[idx1_0], uvs[idx2_0]);
        copyVec2(src->uv[idx1_1], uvs[idx2_1]);
#endif
    }

    return uvsValid(uvs);
}

__inline static int nearsame(Prim *p1, Prim *p2, int idx1, int idx2)
{
    return nearSameVec3(p1->pos[idx1], p2->pos[idx2]) && distance2Squared(p1->uv[idx1], p2->uv[idx2]) < 2.f;
}

static void fixup(CombinedPoly *cp)
{
    int i, j;
    for (i = 0; i < eaSize(&cp->primitives); i++)
    {
        Prim *p1 = cp->primitives[i];
        for (j = i+1; j < eaSize(&cp->primitives); j++)
        {
            Prim *p2 = cp->primitives[j];

            if (nearsame(p1, p2, 0, 0))
                copyVec2(p1->uv[0], p2->uv[0]);
            if (nearsame(p1, p2, 0, 1))
                copyVec2(p1->uv[0], p2->uv[1]);
            if (nearsame(p1, p2, 0, 2))
                copyVec2(p1->uv[0], p2->uv[2]);

            if (nearsame(p1, p2, 1, 0))
                copyVec2(p1->uv[1], p2->uv[0]);
            if (nearsame(p1, p2, 1, 1))
                copyVec2(p1->uv[1], p2->uv[1]);
            if (nearsame(p1, p2, 1, 2))
                copyVec2(p1->uv[1], p2->uv[2]);

            if (nearsame(p1, p2, 2, 0))
                copyVec2(p1->uv[2], p2->uv[0]);
            if (nearsame(p1, p2, 2, 1))
                copyVec2(p1->uv[2], p2->uv[1]);
            if (nearsame(p1, p2, 2, 2))
                copyVec2(p1->uv[2], p2->uv[2]);
        }
    }
}

static void mergeRects(Rect ***rect_list)
{
    int i, j;

    // merge rects
    for (;;)
    {
        Rect *aRect = 0, *bRect = 0;
        float largest_area = 0;

        // find biggest merge
        for (i = 0; i < eaSize(rect_list); i++)
        {
            Rect *a = (*rect_list)[i];

            for (j = i+1; j < eaSize(rect_list); j++)
            {
                float area;
                Rect *b = (*rect_list)[j];

                area = rectArea(a) + rectArea(b);
                if (area <= largest_area)
                    continue;

                if (!rectCanMerge(a, b))
                    continue;

                largest_area = area;
                aRect = a;
                bRect = b;
            }
        }

        if (!aRect || !bRect)
            break;

        // merge
        rectCombine(aRect, aRect, bRect);
        eaFindAndRemove(rect_list, bRect);
        rectFree(bRect);
    }
}

static void fillCPAvailableList(CombinedPoly *cp)
{
    int count = eaSize(&cp->primitives);
    Rect **occupied = 0;
    int i, j, k;
    Rect **largest_pieces = 0, **non_overlapping = 0;
    Rect *start_rect = rectCreate(0, 0, cp->widthWithBorder, cp->heightWithBorder);

    eaCreate(&largest_pieces);
    eaCreate(&non_overlapping);

    eaPush(&cp->availableRects, start_rect);

    eaCreate(&occupied);

    // fill occupied list
    for (i = 0; i < count; i++)
    {
        Rect *r2 = rectCreate(0,0,0,0);
        primCalcIntegerUVExtentsRect(cp->primitives[i], r2);
        rectOffset(r2, -cp->minU, -cp->minV);
        r2->x2 += DOUBLE_BORDER_SIZE;
        r2->y2 += DOUBLE_BORDER_SIZE;
        eaPush(&occupied, r2);
    }

    // fill available list
    for (;;)
    {
        float largest_area = 0;
        Rect *largest = 0;

        for (i = 0; i < eaSize(&cp->availableRects); i++)
        {
            Rect *workingRect = cp->availableRects[i];

            for (j = 0; j < eaSize(&occupied); j++)
            {
                float max_area;
                Rect *occupiedRect = occupied[j];

                if (!rectBooleanSubtract(workingRect, occupiedRect, &non_overlapping))
                {
                    largest = workingRect;
                    eaClearEx(&largest_pieces, rectFree);
                    break;
                }

                if (!eaSize(&non_overlapping))
                    continue;

                max_area = rectArea(non_overlapping[0]);
                for (k = 1; k < eaSize(&non_overlapping); k++)
                {
                    float a = rectArea(non_overlapping[k]);
                    max_area = MAX(max_area, a);
                }

                if (max_area > largest_area)
                {
                    largest_area = max_area;
                    largest = workingRect;
                    eaClearEx(&largest_pieces, rectFree);
                    eaCopy(&largest_pieces, &non_overlapping);
                    eaSetSize(&non_overlapping, 0);
                }

                eaClearEx(&non_overlapping, rectFree);
            }
        }

        if (!largest)
            break;

        eaFindAndRemove(&cp->availableRects, largest);

        for (j = 0; j < eaSize(&largest_pieces); j++)
        {
            if (rectArea(largest_pieces[j]))
                eaPush(&cp->availableRects, largest_pieces[j]);
            else
                rectFree(largest_pieces[j]);
        }

        eaSetSize(&largest_pieces, 0);
    }

    eaDestroy(&largest_pieces);
    eaDestroy(&non_overlapping);
    eaDestroyEx(&occupied, rectFree);

    mergeRects(&cp->availableRects);

    for (i = 0; i < eaSize(&cp->availableRects); i++)
    {
        Rect *r = cp->availableRects[i];
        if (rectWidth(r) <= DOUBLE_BORDER_SIZE || rectHeight(r) <= DOUBLE_BORDER_SIZE)
        {
            rectFree(r);
            eaRemove(&cp->availableRects, i);
            i--;
        }
    }
}


float primAngle(Prim *p1, Prim *p2)
{
    return dotVec3(p1->plane, p2->plane);
}

static void buildCombinedPolygons(Prim ***primitives, CombinedPoly ***combinedpolys)
{
    Prim **prims = 0;
    Prim **verboten = 0;
    int i, j, size = eaSize(primitives);

    if (!size)
        return;

    // copy the list so we can sort it and remove things from it
    eaCopy(&prims, primitives);
    eaCreate(&verboten);

    // build neighbor lists
    for (i = 0; i < size; i++)
    {
        Prim *prim0 = prims[i];
        if (prim0->is_bad)
            continue;
        for (j = i + 1; j < size; j++)
        {
            int e0, e1;
            Prim *prim1 = prims[j];
            if (prim1->is_bad)
                continue;
            if (share2verts(prim0, prim1, &e0, &e1))
            {
                prim0->neighbors[e0] = prim1;
                prim1->neighbors[e1] = prim0;
            }
        }
    }

    // combine
    qsortG(prims, size, sizeof(*prims), primCompare);
    while (eaSize(&prims))
    {
        Vec2 mn, mx;
        CombinedPoly *cp = cpCreate();
        Prim *p = eaPop(&prims);
        float cp_cumulative_scale = 1;

        if (p->is_bad)
            continue;

        eaPush(combinedpolys, cp);

        assert(!p->in_cp);

        cpPushPrim(cp, p);
        p->in_cp = 1;
        primCalcUVExtents(p, mn, mx);

        eaSetSize(&verboten, 0);

#if COMBINE_POLYS
        // expand combined polygon to as many neighbors as possible
        for (;;)
        {
            Prim *src=0;
            int eidx;

            // try to find a neighbor with the same plane
            for (i = 0; i < eaSize(&cp->primitives); i++)
            {
                p = cp->primitives[i];
                if (p->neighbors[0] && !p->neighbors[0]->in_cp && nearSameVec3(p->plane, p->neighbors[0]->plane) && eaFind(&verboten, p->neighbors[0]) < 0)
                {
                    src = p;
                    eidx = 0;
                    break;
                }
                if (p->neighbors[1] && !p->neighbors[1]->in_cp && nearSameVec3(p->plane, p->neighbors[1]->plane) && eaFind(&verboten, p->neighbors[1]) < 0)
                {
                    src = p;
                    eidx = 1;
                    break;
                }
                if (p->neighbors[2] && !p->neighbors[2]->in_cp && nearSameVec3(p->plane, p->neighbors[2]->plane) && eaFind(&verboten, p->neighbors[2]) < 0)
                {
                    src = p;
                    eidx = 2;
                    break;
                }
            }

            if (!src)
            {
                // try to find a neighbor
                for (i = 0; i < eaSize(&cp->primitives); i++)
                {
                    p = cp->primitives[i];
                    if (p->neighbors[0] && !p->neighbors[0]->in_cp && dotVec3(p->plane, p->neighbors[0]->plane) > 0 && eaFind(&verboten, p->neighbors[0]) < 0)
                    {
                        src = p;
                        eidx = 0;
                        break;
                    }
                    if (p->neighbors[1] && !p->neighbors[1]->in_cp && dotVec3(p->plane, p->neighbors[1]->plane) > 0 && eaFind(&verboten, p->neighbors[1]) < 0)
                    {
                        src = p;
                        eidx = 1;
                        break;
                    }
                    if (p->neighbors[2] && !p->neighbors[2]->in_cp && dotVec3(p->plane, p->neighbors[2]->plane) > 0 && eaFind(&verboten, p->neighbors[2]) < 0)
                    {
                        src = p;
                        eidx = 2;
                        break;
                    }
                }
            }

            if (src)
            {
                int ok;
                Vec2  mn2, mx2;
                Vec2 remappedUVs[3];
                Prim *found = src->neighbors[eidx];
                float scale = 1;
                double angle;
                float cumulative_scale = cp_cumulative_scale;

#if REMAPDEBUG
                if (src->debug_id == DEBUG_PRIM || found->debug_id == DEBUG_PRIM)
                    scale = 1;
#endif

                ok = remap(src, found, eidx, remappedUVs, &scale, &angle);
                if (scale < 1)
                {
                    scale = 1.f / scale;
                    cumulative_scale *= scale;
                }
                else
                {
                    cumulative_scale = MAX(scale, cp_cumulative_scale);
                    scale = 1;
                }

                MINVEC2(remappedUVs[0], mn, mn2);
                MINVEC2(remappedUVs[1], mn2, mn2);
                MINVEC2(remappedUVs[2], mn2, mn2);
                MAXVEC2(remappedUVs[0], mx, mx2);
                MAXVEC2(remappedUVs[1], mx2, mx2);
                MAXVEC2(remappedUVs[2], mx2, mx2);

                scaleVec2(mn2, scale, mn2);
                scaleVec2(mx2, scale, mx2);

                if (!ok || cumulative_scale > MAX_SCALE || !sizeValid(mn, mx, mn2, mx2) || !cpTestOverlapAndMap(cp, found, remappedUVs))
                {
                    eaPush(&verboten, found);
                }
                else
                {
                    eaFindAndRemove(&prims, found);
                    cpPushPrim(cp, found);
                    found->in_cp = 1;
                    cpScale(cp, scale);
                    cp_cumulative_scale = cumulative_scale;
                    //fixup(cp);
                    cpCalcExtents2(cp, mn, mx);
                }
            }
            else
            {
                cpCalcExtents(cp);
#if FILL_HOLES
                fillCPAvailableList(cp);
#endif
                break;
            }
        }
#else
        cpCalcExtents(cp);
#endif // COMBINE_POLYS
    }

    // free the lists
    eaDestroy(&verboten);
    eaDestroy(&prims);
}



/*****************************************************************************
*****************************************************************************/

#define MAX_ALLOWED_COST 200000.0f
#define MIN_CP_AREA 10.0f

typedef struct
{
    CombinedPoly *cpA, *cpB;
    float planarity;
    float compactness;
    float cost;
} MergeOperation;

MergeOperation **availableMergeOps = NULL;


/*****************************************************************************
*****************************************************************************/

int assertPrimsAreInCorrectCPs( CombinedPoly **cpList )
{
    int i, j, numCps = eaSize(&cpList), numPrims;

    for ( i = 0; i < numCps; ++i )
    {
        numPrims = eaSize(&cpList[i]->primitives);
        for ( j = 0; j < numPrims; ++j )
        {
            if ( cpList[i]->primitives[j]->cp != cpList[i] )
            {
                assert( "Primitive not in correct CP!"==0);
                return 0;
            }
        }
    }

    return 1;
}


int assertQueueContainsValidCPs( MergeOperation ***queue, CombinedPoly ***cpList )
{
    int i, numMOs = eaSize(queue);
    for ( i = 0; i < numMOs; ++i )
    {
        //if ( !cpFind(cpList, (*queue)[i]->cpA) )
        if ( !(*queue)[i]->cpA )
        {
            assert("Invalid CP in Queue"==0);
            return 0;
        }
        //if ( !cpFind(cpList, (*queue)[i]->cpB) )
        if ( !(*queue)[i]->cpB )
        {
            assert("Invalid CP in Queue"==0);
            return 0;
        }
    }
    return 1;
}

int assertQueueDoesntContainDupes( MergeOperation ***queue )
{
    int i, j, numMOs = eaSize(queue);
    for ( i = 0; i < numMOs; ++i )
    {
        for ( j = i + 1; j < numMOs; ++j )
        {
            if ( (*queue)[i] == (*queue)[j] )
            {
                assert( "Duplicate found in queue"==0 );
                return 0;
            }
        }
    }
    return 1;
}

int assertCPListIsValid( CombinedPoly ***cpList )
{
    int i, numCps = eaSize((void***)(&cpList));

    for ( i = 0; i < numCps; ++i )
    {
        if ((*cpList)[i] == NULL )
        {
            assert( "Invalid CP in CP List"==0 );
            return 0;
        }
    }
    return 1;
}

int assertCPHasAtLeast1Neighbor( CombinedPoly *cp )
{
    int i, numPrims = eaSize(&cp->primitives);
    CombinedPoly **neighbors;

    eaCreate(&neighbors);

    for ( i = 0; i < numPrims; ++i )
    {
        if ( !cp->primitives[i]->neighbors[0] || (cp->primitives[i]->neighbors[0]->cp != cp->primitives[i]->cp) )
            return 1;
        if ( !cp->primitives[i]->neighbors[1] || (cp->primitives[i]->neighbors[1]->cp != cp->primitives[i]->cp) )
            return 1;
        if ( !cp->primitives[i]->neighbors[2] || (cp->primitives[i]->neighbors[2]->cp != cp->primitives[i]->cp) )
            return 1;
    }
    printf("numPrims: %d\n", numPrims);
    for ( i = 0; i < numPrims; ++i )
    {
        printf("Prim #%d\n\tcp: %d\tn0: %d\tn1: %d\tn2: %d\n", i, cp->primitives[i]->cp, 
            cp->primitives[i]->neighbors[0]->cp,
            cp->primitives[i]->neighbors[1]->cp,
            cp->primitives[i]->neighbors[2]->cp );
    }

    assert("CP has no neighbors"==0);
    return 0;
}

int assertCPHasAtLeast3Neighbors( CombinedPoly *cp )
{
    int i, j, numPrims = eaSize(&cp->primitives);
    CombinedPoly **neighbors;

    eaCreate(&neighbors);

    for ( i = 0; i < numPrims; ++i )
    {
        if ( !cp->primitives[i]->neighbors[0] || (cp->primitives[i]->neighbors[0]->cp != cp->primitives[i]->cp) )
        {
            int found = 0;
            for ( j = 0; j < eaSize(&neighbors); ++j )
            {
                if ( !cp->primitives[i]->neighbors[0] && !neighbors[j] )
                    found = 1;
                else if ( cp->primitives[i]->neighbors[0]->cp == neighbors[j] )
                    found = 1;
            }
            if ( !found )
            { 
                if ( !cp->primitives[i]->neighbors[0] )
                    eaPush(&neighbors, 0);
                else 
                    eaPush(&neighbors, cp->primitives[i]->neighbors[0]->cp);
            }
        }
        if ( !cp->primitives[i]->neighbors[1] || (cp->primitives[i]->neighbors[1]->cp != cp->primitives[i]->cp) )
        {
            int found = 0;
            for ( j = 0; j < eaSize(&neighbors); ++j )
            {
                if ( !cp->primitives[i]->neighbors[1] && !neighbors[j] )
                    found = 1;
                else if ( cp->primitives[i]->neighbors[1]->cp == neighbors[j] )
                    found = 1;
            }
            if ( !found )
            {
                if ( !cp->primitives[i]->neighbors[1] )
                    eaPush(&neighbors, 0);
                else 
                    eaPush(&neighbors, cp->primitives[i]->neighbors[1]->cp);
            }
        }
        if ( !cp->primitives[i]->neighbors[2] || (cp->primitives[i]->neighbors[2]->cp != cp->primitives[i]->cp) )
        {
            int found = 0;
            for ( j = 0; j < eaSize(&neighbors); ++j )
            {
                if ( !cp->primitives[i]->neighbors[2] && !neighbors[j] )
                    found = 1;
                else if ( cp->primitives[i]->neighbors[2]->cp == neighbors[j] )
                    found = 1;
            }
            if ( !found )
            {
                if ( !cp->primitives[i]->neighbors[2] )
                    eaPush(&neighbors, 0);
                else 
                    eaPush(&neighbors, cp->primitives[i]->neighbors[2]->cp);
            }
        }
    }

    if ( eaSize(&neighbors) < 3 )
    {
        assert( "Not enough neighbors in CP"==0);
        return 0;
    }

    return 1;
}


int assertAtLeast3NeighborsPerCP( CombinedPoly ***cpList )
{
    int i, numCps = eaSize(cpList);

    for ( i = 0; i < numCps; ++i )
    {
        if ( !assertCPHasAtLeast3Neighbors((*cpList)[i]) )
            return 0;
    }

    return 1;
}

int assertValidMapping( CombinedPoly *cp )
{
    int i;

    for ( i = 0; i < eaSize(&cp->primitives); ++i )
    {
        Prim *p = cp->primitives[i];

        assert(_finite(p->uv[0][0]));
        assert(_finite(p->uv[0][1]));
        assert(_finite(p->uv[1][0]));
        assert(_finite(p->uv[1][1]));
        assert(_finite(p->uv[2][0]));
        assert(_finite(p->uv[2][1]));
    }

    return 1;
}



/*****************************************************************************
*****************************************************************************/


MergeOperation *newMergeOperation(CombinedPoly *cpA, CombinedPoly *cpB)
{
    MergeOperation *newMO;

    if ( !availableMergeOps )
        eaCreate(&availableMergeOps);

    if ( eaSize(&availableMergeOps) )
        newMO = (MergeOperation*)eaPop(&availableMergeOps);
    else
        newMO = (MergeOperation*)malloc(sizeof(MergeOperation));

    newMO->cpA = cpA;
    newMO->cpB = cpB;
    return newMO;
}

void deleteMergeOperation(MergeOperation *mo)
{
    eaPush(&availableMergeOps, mo);
}

void freeAllMergeOperations()
{
    int i = 0, moSize = eaSize(&availableMergeOps);
    for ( i = 0; i < moSize; ++i )
    {
        free(availableMergeOps[i]);
    }
    eaDestroy(&availableMergeOps);
    availableMergeOps = NULL;
}

//int mergeOpCmp( const MergeOperation **a, const MergeOperation **b )
//{
//    return (*a)->cost < (*b)->cost ? -1 : ((*a)->cost > (*b)->cost ? 1 : 0);
//}
//
//// returns the index of the first element with the same or higher cost as toFind in queue, -1 on fail
//int mergeOpListFindInsert( MergeOperation ***queue, MergeOperation *toFind )
//{
//    MergeOperation *ret = bsearch( &toFind, *queue, eaSize(queue), sizeof(MergeOperation*), mergeOpCmp );
//    int i;
//
//    if ( !ret )
//        return -1;
//
//    i = (int)(((U64)ret - (U64)(*queue)) >> (sizeof(void*)>>1));
//
//    if ( i < 0 || i > eaSize(queue) || (*queue)[i]->cost != toFind->cost )
//        return -1;
//
//    while ( i >= 0 && (*queue)[i]->cost == toFind->cost ) --i;
//    ++i;
//    while ( i < eaSize(queue) && (*queue)[i]->cost == toFind->cost ) ++i;
//
//    if ( i == eaSize(queue) )
//        return -1;
//
//    return i;
//}

int isAlreadyQueued( MergeOperation ***queue, MergeOperation *toAdd )
{
    int i = 0, qSize = eaSize(queue);
    for ( i = 0; i < qSize; ++i )
    {
        if ( (toAdd->cpA == (*queue)[i]->cpA && toAdd->cpB == (*queue)[i]->cpB) ||
            (toAdd->cpB == (*queue)[i]->cpA && toAdd->cpA == (*queue)[i]->cpB) )
            return 1;
    }
    return 0;
}

void bestFitPlane( CombinedPoly *cp, Vec4 p )
{
    int i, cp_size = eaSize(&cp->primitives);
    Vec3 avgNorm, centroid, norm;

    centroid[0] = centroid[1] = centroid[2] = avgNorm[0] = avgNorm[1] = avgNorm[2] = 0.0f;

    // average all of the positions and normals(scaled by the primitive's area)
    for ( i = 0; i < cp_size; ++i )
    {
        Prim * prim = cp->primitives[i];
        centroid[0] += prim->pos[0][0] + prim->pos[2][0] + prim->pos[2][0];
        centroid[1] += prim->pos[0][1] + prim->pos[2][1] + prim->pos[2][1];
        centroid[2] += prim->pos[0][2] + prim->pos[2][2] + prim->pos[2][2];

        calcFaceNormal(prim->pos[0], prim->pos[1], prim->pos[2], norm );
        avgNorm[0] += norm[0] * prim->area;
        avgNorm[1] += norm[1] * prim->area;
        avgNorm[2] += norm[2] * prim->area;
    }

    centroid[0] /= cp_size * 3;
    centroid[1] /= cp_size * 3;
    centroid[2] /= cp_size * 3;

    avgNorm[0] /= cp_size;
    avgNorm[1] /= cp_size;
    avgNorm[2] /= cp_size;

    makePlane2(centroid, avgNorm, p);
}

const float nonplanar_add = 1000.0f;

float findPlanarity( CombinedPoly *cp )
{
    Vec4 p;
    float planarity = 0.0f;
    int i;
    bestFitPlane(cp, p);
    // average distance from points to best fit planes
    for ( i = 0; i < eaSize(&cp->primitives); ++i )
    {
        planarity += fabs(distanceToPlane( cp->primitives[i]->pos[0], p ));
        planarity += fabs(distanceToPlane( cp->primitives[i]->pos[1], p ));
        planarity += fabs(distanceToPlane( cp->primitives[i]->pos[2], p ));
    }
    planarity /= eaSize(&cp->primitives) * 3;
    // if the dot of the plane normal and the triangle normal is negative, this isnt very planar
    for ( i = 0; i < eaSize(&cp->primitives); ++i )
    {
        if ( dotVec3(p, cp->primitives[i]->plane) < 0.0f )
            planarity += nonplanar_add;
    }
    return planarity;
}
float minCompact=1e15, maxCompact=0.0f, minPlanar=1e15, maxPlanar=0.0f;
float minCost=1e15, maxCost=0.0f, avgCost=0.0f;
int numCosts = 0;

int enqueueMergeOperation( MergeOperation ***queue, CombinedPoly ***cpList, MergeOperation *toAdd )
{
    int i, queue_size = eaSize(queue), cplist_size = eaSize(cpList);
    CombinedPoly *cpA, *cpB;
    static CombinedPoly cp;

//PERFINFO_AUTO_START("is_mergeop_valid",1);
    cpA = toAdd->cpA;
    cpB = toAdd->cpB;
    if ( toAdd->cpA == toAdd->cpB || !cpA || !cpB || isAlreadyQueued(queue, toAdd) )
    {
    PERFINFO_AUTO_STOP();
        return 0;
    }
//PERFINFO_AUTO_STOP();

//PERFINFO_AUTO_START("test_combine",1);

    cpCopy(&cp, cpA);
    cpAdd(&cp, cpB);
    cpCorrectPrimPointers(&cp);

//PERFINFO_AUTO_START("cpIsLegal",1);
    if ( !cpIsLegal(&cp) )
    {
        cpCorrectPrimPointers(cpA);
        cpCorrectPrimPointers(cpB);
    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
        return 0;
    }
//PERFINFO_AUTO_STOP();
//PERFINFO_AUTO_STOP();

//PERFINFO_AUTO_START("compute_cost",1);

    toAdd->planarity = findPlanarity(&cp);
    if ( cpArea(cpA) < 0.1f || cpArea(cpB) < 0.1f && toAdd->planarity < nonplanar_add )
        toAdd->compactness = 1e-5f;
    else
        toAdd->compactness = fabs(1.0f-(cpPerimeter(&cp)/cpArea(&cp)));

    minCompact = MIN(minCompact, toAdd->compactness);
    minPlanar = MIN(minPlanar, toAdd->planarity);
    maxCompact = MAX(maxCompact, toAdd->compactness);
    maxPlanar = MAX(maxPlanar, toAdd->planarity);
    
    // TODO: this is a completely arbitrary cost computation.
    toAdd->cost = SQR(toAdd->planarity) * (toAdd->compactness);

    minCost = MIN(minCost, toAdd->cost);
    maxCost = MAX(maxCost, toAdd->cost);
    numCosts++;

//PERFINFO_AUTO_STOP();

//PERFINFO_AUTO_START("enqueue",1);

    // priority insert, lowest cost towards the end (for popping)
    i = 0;
    while ( i < queue_size && (*queue)[i]->cost > toAdd->cost ) ++i;
    eaInsert(queue, toAdd, i);

//PERFINFO_AUTO_STOP();

//PERFINFO_AUTO_START("correct_pointers",1);
    cpCorrectPrimPointers(cpA);
    cpCorrectPrimPointers(cpB);
//PERFINFO_AUTO_STOP();

    return 1;
}

int mergeBestAndUpdateNeighbors( MergeOperation ***queue, CombinedPoly ***cpList )
{
    MergeOperation *op, *oldOp;
    CombinedPoly *cpA, *cpB, *cpACopy;
    int i, qSize, branchHit = 0, fails = 0;
    static MergeOperation **savedOps = NULL;
    static int numMerges = 0;

    if ( savedOps == NULL )
        eaCreate(&savedOps);

    qSize = eaSize(queue);

    op = eaPop(queue);
    assert(qSize != eaSize(queue));

    if ( !op || op->cost > MAX_ALLOWED_COST )
        return 0;

//PERFINFO_AUTO_START("do_merge",1);
    numMerges++;

    cpA = op->cpA;
    cpB = op->cpB;

    assert( cpA && cpB );
    cpACopy = cpCreate();
    cpCopy(cpACopy, cpA);
    cpAdd( cpA, cpB );

    if ( 0 )
    {
        cpProject(cpACopy);
        cpProject(cpB);
        cpNormalizeOffsetsFinalize(cpACopy);
        cpNormalizeOffsetsFinalize(cpB);
        drawTGAStart();
        drawCPTGANoFinal(cpACopy, 2, red);
        drawCPTGANoFinal(cpB, 2, blue);
        drawTGAEnd("C:\\cps.tga");
    }
    cpDestroy(cpACopy);

    eaRemove( cpList, cpFindIdx(cpList,op->cpB->id) );
    cpDestroy(cpB);

//PERFINFO_AUTO_STOP();

//PERFINFO_AUTO_START("fix_invalid_merges",1);

    qSize = eaSize(queue);
    // find all of the merge operations that are now obsolete since this merge and recalculate them
    for ( i = qSize-1; i >= 0; --i )
    {
        if ( (*queue)[i]->cpA == op->cpB )
        {
            oldOp = (*queue)[i];
            assert( oldOp != op );
            eaRemove(queue, i);
            oldOp->cpA = op->cpA;
            eaPush(&savedOps, oldOp);
        }
        else if ( (*queue)[i]->cpB == op->cpB )
        {
            oldOp = (*queue)[i];
            assert( oldOp != op );
            eaRemove(queue, i);
            oldOp->cpB = op->cpA;
            eaPush(&savedOps, oldOp);
        }
        else if ( (*queue)[i]->cpA == op->cpA || (*queue)[i]->cpB == op->cpA )
        {
            oldOp = (*queue)[i];
            assert( oldOp != op );
            eaRemove(queue, i);
            eaPush(&savedOps, oldOp);
        }
    }

//PERFINFO_AUTO_STOP();
    
//PERFINFO_AUTO_START("requeue_fixed merges",1);

    while ( oldOp = eaPop(&savedOps) )
    {
        if ( !enqueueMergeOperation(queue, cpList, oldOp) )
            deleteMergeOperation(oldOp);
    }

//PERFINFO_AUTO_STOP();

    deleteMergeOperation(op);
    qSize = eaSize(queue);
    return 1;
}


typedef struct _VertexGraphNode VertexGraphNode;
typedef struct _VertexGraphNodeConnection VertexGraphNodeConnection;

typedef struct _VertexGraphNodeConnection
{
    F32 k;
    VertexGraphNode *node;
} VertexGraphNodeConnection;

typedef struct _VertexGraphNode
{
    int idx;
    Vec3 vert;
    Vec2 map;
    VertexGraphNodeConnection **connections;
} VertexGraphNode;

typedef struct
{
    VertexGraphNode *head;
    VertexGraphNode **nodes;
} VertexGraph;


VertexGraph *newVertexGraph()
{
    VertexGraph *ret = (VertexGraph*)malloc(sizeof(VertexGraph));
    ret->head = NULL;
    eaCreate(&ret->nodes);
    return ret;
}

VertexGraphNodeConnection *newVertexGraphNodeConnection(VertexGraphNode *con, float k)
{
    VertexGraphNodeConnection *ret = (VertexGraphNodeConnection *)malloc(sizeof(VertexGraphNodeConnection));
    ret->node = con;
    ret->k = k;
    return ret;
}

VertexGraphNode *newVertexGraphNode(VertexGraph *graph, Vec3 v)
{
    VertexGraphNode *ret = (VertexGraphNode*)malloc(sizeof(VertexGraphNode));
    memcpy(ret->vert, v, sizeof(Vec3));
    eaCreate(&ret->connections);
    ret->idx = eaSize(&graph->nodes);
    eaPush(&graph->nodes, ret);
    return ret;
}

// does not remove the node from the graph, merely frees the memory associated with a node
void freeVertexGraphNode(VertexGraphNode *node)
{
    int i;
    for ( i = 0; i < eaSize(&node->connections); ++i )
    {
        free(node->connections[i]);
    }
    eaDestroy(&node->connections);
    free(node);
}

void freeVertexGraph( VertexGraph *graph )
{
    int i;
    for ( i = 0; i < eaSize(&graph->nodes); ++i )
    {
        freeVertexGraphNode(graph->nodes[i]);
    }
    eaDestroy(&graph->nodes);
    free(graph);
}

VertexGraphNode *vertexGraphNodeFind( VertexGraph *graph, Vec3 vert )
{
    int i;
    for ( i = 0; i < eaSize(&graph->nodes); ++i )
    {
        if ( sameVec3(graph->nodes[i]->vert, vert) )
            return graph->nodes[i];
    }
    return NULL;
}

// adds a connection between two nodes.
// creates the added node for vert if it doesnt already exist
// if 'connection' does not exist in the graph, the function fails and returns 0
VertexGraphNode *vertexGraphNodeAdd( VertexGraph *graph, Vec3 connection, Vec3 vert )
{
    int i; 
    VertexGraphNode *newNode = vertexGraphNodeFind(graph, vert), 
        *node = vertexGraphNodeFind(graph, connection);

    if ( !newNode )
        newNode = newVertexGraphNode(graph, vert);

    if ( !node )
    {
        // maybe they got vert and connection mixed up
        node = newNode;
        newNode = newVertexGraphNode(graph, connection);
        if ( !node )
        {
            freeVertexGraphNode(newNode);
            freeVertexGraphNode(node);
            return 0;
        }
    }

    // make sure the connection doesnt already exist
    for ( i = 0; i < eaSize(&node->connections); ++i )
    {
        if ( node->connections[i]->node == newNode )
            return newNode;
    }

    eaPush(&newNode->connections, newVertexGraphNodeConnection(node, 0));
    eaPush(&node->connections, newVertexGraphNodeConnection(newNode, 0));

    return newNode;
}

int vgncCmp(const void* aData, const void* bData)
{
    const VertexGraphNodeConnection ** a = (const VertexGraphNodeConnection **)aData;
    const VertexGraphNodeConnection ** b = (const VertexGraphNodeConnection **)bData;
    return (*a)->node->idx < (*b)->node->idx ? -1 : ((*a)->node->idx > (*b)->node->idx ? 1 : 0) ;
}

void vertexGraphNodeSortConnections( VertexGraphNode *node )
{
    qsort(node->connections, eaSize(&node->connections), sizeof(VertexGraphNodeConnection*), vgncCmp);
}

void vertexGraphGetSharedNodes( VertexGraphNode *a, VertexGraphNode *b, VertexGraphNode **ret1, VertexGraphNode **ret2 )
{
    int i, j;
    (*ret1) = (*ret2) = NULL;

    for ( i = 0; i < eaSize(&a->connections); ++i )
    {
        for ( j = 0; j < eaSize(&b->connections); ++j )
        {
            if ( sameVec3(a->connections[i]->node->vert, b->connections[j]->node->vert) )
            {
                if ( !(*ret1) )
                    (*ret1) = a->connections[i]->node;
                else
                {
                    (*ret2) = a->connections[i]->node;
                    return;
                }
            }
        }
    }
}

// this assumes at least one vertex in the prim is in the graph already
void addPrimToVertexGraph( VertexGraph *graph, Prim *p )
{
    int i;

    p->in_vertex_graph = 1;

    if ( !vertexGraphNodeAdd(graph, p->pos[0], p->pos[1]) )
    {
        vertexGraphNodeAdd(graph, p->pos[2], p->pos[0]);
        vertexGraphNodeAdd(graph, p->pos[2], p->pos[1]);
        vertexGraphNodeAdd(graph, p->pos[0], p->pos[1]);
    }
    else
    {
        vertexGraphNodeAdd(graph, p->pos[0], p->pos[2]);
        vertexGraphNodeAdd(graph, p->pos[1], p->pos[2]);
    }

    for ( i = 0; i < 3; ++i )
    {
        if ( p->neighbors[i] && p->cp == p->neighbors[i]->cp && !p->neighbors[i]->in_vertex_graph )
            addPrimToVertexGraph( graph, p->neighbors[i] );
    }
}

VertexGraph *createCPVertexGraph( CombinedPoly *cp )
{
    VertexGraph *graph;
    Prim *p = cp->primitives[0];

    graph = newVertexGraph();
    graph->head = newVertexGraphNode(graph, p->pos[0]);
    addPrimToVertexGraph(graph, p);
    return graph;
}

float genPartialConstant( Vec3 i, Vec3 j, Vec3 k )
{
    return (distance3Squared(i, k) + distance3Squared(j, k) - distance3Squared(i, j)) / triArea(i, j, k);
}

float genConstant( Vec3 i, Vec3 j, Vec3 k1, Vec3 k2 )
{
    float distijSq = distance3Squared(i, j);
    return (distance3Squared(i, k1) + distance3Squared(j, k1) - distijSq) / triArea(i, j, k1) +
        (distance3Squared(i, k2) + distance3Squared(j, k2) - distijSq) / triArea(i, j, k2);
}

void generateVertexGraphConstants( VertexGraph *graph )
{
    int i, j;
    for ( i = 0; i < eaSize(&graph->nodes); ++i )
    {
        //eafSetSize(&graph->nodes[i]->constants, eaSize(&graph->nodes[i]->connections) );
        for ( j = 0; j < eaSize(&graph->nodes[i]->connections); ++j )
        {
            VertexGraphNode *a, *b;
            vertexGraphGetSharedNodes(graph->nodes[i], graph->nodes[i]->connections[j]->node, &a, &b);
            assert(a);
            if ( a && b )
            {
                graph->nodes[i]->connections[j]->k = genConstant(graph->nodes[i]->vert, graph->nodes[i]->connections[j]->node->vert, a->vert, b->vert);
                assert( _finite(graph->nodes[i]->connections[j]->k) );
            }
            else
            {
                graph->nodes[i]->connections[j]->k = genPartialConstant(graph->nodes[i]->vert, graph->nodes[i]->connections[j]->node->vert, a->vert);
                assert( _finite(graph->nodes[i]->connections[j]->k) );
            }
        }
    }
}

int countConstants( VertexGraph *graph )
{
    int i, ret = 0;
    for ( i = 0; i < eaSize(&graph->nodes); ++i )
    {
        ret += eaSize(&graph->nodes[i]->connections) + 1;
    }
    return ret;
}

float vertexConstantSum( VertexGraphNode *node )
{
    int i;
    float ret = 0;
    for ( i = 0; i < eaSize(&node->connections); ++i )
    {
        ret += node->connections[i]->k;
    }
    return ret;
}


#ifndef taucs_values
#define taucs_values values.s
#endif
#define TAUCS_VALUES_SIZE TAUCS_SINGLE

void fillMatrix( VertexGraph *graph, F32 **mat )
{
    int i, j, r_idx = 0, c_idx = 0;
    
    for ( i = 0; i < eaSize(&graph->nodes); ++i )
    {
        r_idx = graph->nodes[i]->idx;
        vertexGraphNodeSortConnections(graph->nodes[i]);

        mat[r_idx][r_idx] = vertexConstantSum(graph->nodes[i]);

        for ( j = 0; j < eaSize(&graph->nodes[i]->connections); ++j )
        {
            c_idx = graph->nodes[i]->connections[j]->node->idx;
            mat[r_idx][c_idx] = -graph->nodes[i]->connections[j]->k;
        }
    }
}

void fillTaucsMatrix( VertexGraph *graph, taucs_ccs_matrix *A, int uniform_constants )
{
    int i, j, thisNodeInserted = 0, r_idx = 0, col_idx = 0;
    
    for ( i = 0; i < eaSize(&graph->nodes); ++i )
    {
        vertexGraphNodeSortConnections(graph->nodes[i]);

        thisNodeInserted = 0;

        A->colptr[col_idx++] = r_idx;
        for ( j = 0; j < eaSize(&graph->nodes[i]->connections); ++j )
        {
            if ( graph->nodes[i]->idx < graph->nodes[i]->connections[j]->node->idx && !thisNodeInserted )
            {
                if ( uniform_constants )
                    A->taucs_values[r_idx] = eaSize(&graph->nodes[i]->connections);
                else
                    A->taucs_values[r_idx] = vertexConstantSum(graph->nodes[i]);
                assert( /*FINITE*/_finite(A->taucs_values[r_idx]) );
                A->rowind[r_idx++] = graph->nodes[i]->idx;
                thisNodeInserted = 1;
            }

            if ( uniform_constants )
                A->taucs_values[r_idx] = -1.0f;
            else
                A->taucs_values[r_idx] = -graph->nodes[i]->connections[j]->k;

            assert( /*FINITE*/_finite(A->taucs_values[r_idx]) );
            A->rowind[r_idx++] = graph->nodes[i]->connections[j]->node->idx;

            if ( j == eaSize(&graph->nodes[i]->connections) - 1 && !thisNodeInserted )
            {
                if ( uniform_constants )
                    A->taucs_values[r_idx] = eaSize(&graph->nodes[i]->connections);
                else
                    A->taucs_values[r_idx] = vertexConstantSum(graph->nodes[i]);
                assert( /*FINITE*/_finite(A->taucs_values[r_idx]) );
                A->rowind[r_idx++] = graph->nodes[i]->idx;
                thisNodeInserted = 1;
            }
        }
    }

    A->colptr[col_idx++] = r_idx-1;
}

void drawCorners( F32 *b, int sz, float offX, float offY )
{
    int i;
    static int out = 0;
    char path[MAX_PATH];

    drawTGAStart();

    for ( i = 0; i < sz - 1; ++i )
    {
        if ( (b[i] == 0.0f && b[i+sz] == 0.0f) || (b[i+1] == 0.0f && b[i+sz+1] == 0.0f) )
            continue;
        drawLineTGA(b[i]+offX, b[i+sz]+offY, b[i+1]+offX, b[i+sz+1]+offY, blue);
    }

    sprintf(path, "C:\\tga\\%d.tga", out++);
    drawTGAEnd(path);
}

void printCorners( F32 *b, int sz )
{
    int i;
    for ( i = 0; i < sz; ++i )
    {
        printf("%f, %f\n", b[i], b[i+sz]);
    }
}


// creates a matrix that rotates around the Z axis
void zRotationMatrix( Mat3 mat, float angle )
{
    mat[0][0] = cos(angle); mat[0][1] = -sin(angle);    mat[0][2] = 0.0f;
    mat[1][0] = sin(angle); mat[1][1] = cos(angle);        mat[1][2] = 0.0f;
    mat[2][0] = 0.0f;        mat[2][1] = 0.0f;            mat[2][2] = 0.0f;
}

// map the corners of the cp into pre-determined positions, so that
// the solution to the linear equation doesnt resolve to all 0's
void setCorners( VertexGraph *graph, CombinedPoly *cp, taucs_ccs_matrix *A, F32 *b, int nnz )
{
    int i, j, runs, cur;
    U32 *cornerIndicies;
    //Vec3 norm;
    Mat3 mat;
    float perim = 0.0f;// = cpPerimeter(cp);
    float radius;// = (perim/PI)*0.5f;
    float dist = 0.0f;
    Vec3 pointVec = {0.0f, 0.0f, 0.0f};
    Vec3D **cornerPoints;

    eaCreate(&cornerPoints);

    cpUpdateEdges(cp);

    if ( 0 )
    {
        drawTGAStart();
        for ( i = 0; i < eaSize(&cp->edges); ++i )
        {
        }
        drawTGAEnd("C:\\cps.tga");
    }
    cpUpdateCorners(cp);

    for ( i = 0; i < eaSize(&cp->edges); ++i )
    {
        perim += cp->edges[i]->len;
    }
    radius = perim/(PI*2.0f);
    pointVec[1] = radius;

    if ( 0 && eaiSize(&cp->corners) < 3 )
    {
        //cpProject(cp);
        //cpCalcExtents(cp);
        //cpNormalizeOffsetsFinalize(cp);
        //drawCPTGA(cp, 100, "C:\\tga", 0);
        assert("Not enough corners in CP"==0);
    }
    eaiCreate(&cornerIndicies);

    // figure out the corner indicies into b
    for ( i = 0; i < eaSize(&cp->edges); ++i )
    {
        for ( j = 0; j < eaSize(&graph->nodes); ++j )
        {
            if ( sameVec3(graph->nodes[j]->vert, cp->edges[i]->pt[0]) )
                eaiPush(&cornerIndicies, j);
        }
    }

    assert( eaiSize(&cornerIndicies) >= 3 );

    // pre-compute the corner points
    for ( runs = 0, i = cp->corners[0]; runs < eaSize(&cp->edges); 
        ++runs, i = (i >= eaSize(&cp->edges) - 1 ? 0 : i+1) )
    {
        if ( i == cp->corners[0] )
        {
            Vec3D *newVec3D = malloc(sizeof(Vec3D));
            newVec3D->vec[0] = pointVec[0];
            newVec3D->vec[1] = pointVec[1]; 
            newVec3D->vec[2] = pointVec[2];
            eaPush(&cornerPoints, newVec3D);
        }
        else
        {
            dist += cp->edges[i]->len;
            if ( cp->edges[i]->flags == 1 )
            {
                Vec3 out;
                Vec3D *newVec3D = malloc(sizeof(Vec3D));
                zRotationMatrix(mat, (dist/perim)*(2.0f*PI));
                mulVecMat3(pointVec, mat, out);
                dist = 0.0f;
                newVec3D->vec[0] = pointVec[0] = out[0];
                newVec3D->vec[1] = pointVec[1] = out[1]; 
                newVec3D->vec[2] = pointVec[2] = out[2];
                eaPush(&cornerPoints, newVec3D);
            }
        }
    }

    cur = 0;
    dist = 0.0f;
    for ( runs = 0, i = cp->corners[0]; runs < eaiSize(&cornerIndicies); 
        ++runs, i = (i >= eaiSize(&cornerIndicies) - 1 ? 0 : i+1) )
    {
        Vec3 out;
        int idx = cornerIndicies[i], idxminus1;
        int nextCur = (cur+1>eaiSize(&cp->corners)-1?0:cur+1),
            lastCur = (cur-1<0?eaiSize(&cp->corners)-1:cur-1);

        if ( i != cp->corners[0] )
        {
            idxminus1 = cornerIndicies[i-1];
            
            if ( cp->edges[i]->flags == 1 )
            {
                copyVec3(cornerPoints[cur]->vec, out);
                dist = 0.0f;
                cur = nextCur;
            }
            else
            {
                dist += cp->edges[i]->len/cp->cornerDistances[cur];
                subVec3( cornerPoints[cur]->vec, cornerPoints[lastCur]->vec, out );
                scaleVec3(out, dist, out);
                addVec3(cornerPoints[lastCur]->vec, out, out);
            }
        }
        else
        {
            copyVec3(cornerPoints[cur]->vec, out);
            cur = nextCur;
        }

        b[graph->nodes[idx]->idx] = out[0];
        b[graph->nodes[idx]->idx + eaSize(&graph->nodes)] = out[1];

        for ( j = 0; j < nnz; ++j )
        {
            // if this row is the row that the corner is on
            if ( A->rowind[j] == graph->nodes[idx]->idx )
            {
                // if we are in the column that the corner is on
                if ( (A->colptr[graph->nodes[idx]->idx] <= j && A->colptr[graph->nodes[idx]->idx+1] > j)
                    || (A->colptr[graph->nodes[idx]->idx+1] == nnz - 1 && j == A->colptr[graph->nodes[idx]->idx+1]) )
                    A->taucs_values[j] = 1;
                else
                    A->taucs_values[j] = 0;
            }
        }
    }
    j = 1;
    
    if ( 0 )
    {
        float offX = 0.0f, offY = 0.0f, scale = TGASIZE/1;

        for ( i = 0; i < A->n; ++i )
        {
            if ( b[i] < offX )
                offX = b[i];
            if ( b[i+A->n] < offY )
                offY = b[i+A->n];
        }

        drawTGAStart();
        for ( i = 1; i < eaiSize(&cornerIndicies); ++i )
        {
            drawLineTGA((b[graph->nodes[cornerIndicies[i-1]]->idx]-offX) * scale, 
                (b[graph->nodes[cornerIndicies[i-1]]->idx + eaSize(&graph->nodes)]-offY) * scale,
                (b[graph->nodes[cornerIndicies[i]]->idx]-offX) * scale,
                (b[graph->nodes[cornerIndicies[i]]->idx + eaSize(&graph->nodes)]-offY) * scale, red);
        }
        drawTGAEnd("C:\\cps.tga");

        offY = 0;
    }
}

static int lastNNZ = 0;

// remove 0's from the matrix and reallocate it
void fixMatrix(taucs_ccs_matrix **A, int numNonZero)
{
    taucs_ccs_matrix *temp = *A;
    int i, j = 0, idx = 0, nnz = 0;

    for ( i = 0; i < numNonZero; ++i )
    {
        if (temp->taucs_values[i] != 0.0)
            ++nnz;
    }

    lastNNZ = nnz;
    *A = taucs_ccs_create( temp->m, temp->n, nnz, TAUCS_VALUES_SIZE | TAUCS_SYMMETRIC | TAUCS_LOWER );

    j = 0;
    for ( i = 0; i < numNonZero; ++i )
    {
        if (temp->colptr[j] == i)
            (*A)->colptr[j++] = idx;
        
        if (temp->taucs_values[i] != 0.0)
        {
            (*A)->taucs_values[idx] = temp->taucs_values[i];
            (*A)->rowind[idx++] = temp->rowind[i];
        }
    }

    (*A)->colptr[j-1] = nnz;

    taucs_ccs_free(temp);
}

void clearMat( F32 **mat, int sz )
{
    int i, j;

    for ( i = 0; i < sz; ++i )
    {
        for ( j = 0; j < sz; ++j )
        {
            mat[i][j] = 0.0f;
        }
    }
}

void clearVec( F32 *vec, int sz )
{
    int i;

    for ( i = 0; i < sz; ++i )
    {
        vec[i] = 0.0f;
    }
}

void freeMat( F32 **mat, int sz )
{
    int i;

    for ( i = 0; i < sz; ++i )
        free(mat[i]);
    free(mat);
}

void copyVec( F32 *src, F32 *dest, int sz )
{
    int i;

    for ( i = 0; i < sz; ++i )
    {
        dest[i] = src[i];
    }
}

void allocVec( F32 **vec, int sz )
{
    (*vec) = (F32*)malloc(sz * sizeof(F32));
}

void copyMat( F32 **src, F32 **dest, int sz )
{
    int i, j;

    for ( i = 0; i < sz; ++i )
    {
        for ( j = 0; j < sz; ++j )
        {
            dest[i][j] = src[i][j];
        }
    }
}

void transposeMat( F32 **mat, int sz )
{
    int i, j;

    for ( i = 0; i < sz; ++i )
    {
        for ( j = i; j < sz; ++j )
        {
            SWAPF32(mat[i][j], mat[j][i]);
        }
    }
}

void mulMat( F32 **mat1, F32 **mat2, F32 **matOut, int sz )
{
    int i, j, k;

    clearMat(matOut, sz);

    for ( i = 0; i < sz; ++i )
    {
        for ( j = 0; j < sz; ++j )
        {
            for ( k = 0; k < sz; ++k )
            {
                matOut[i][j] += mat2[i][k] * mat1[k][j];
            }
        }
    }
}

void mulMatVec( F32 **mat, F32 *vec, F32 *vecOut, int sz )
{
    int i, j;

    clearVec(vecOut, sz);

    for ( i = 0; i < sz; ++i )
    {
        for ( j = 0; j < sz; ++j )
        {
            vecOut[i] += mat[j][i] * vec[j];
        }
    }
}

void allocMatrix(F32 ***mat, int sz)
{
    int i;

    (*mat) = (F32**)calloc(sz, sizeof(F32*));
    for ( i = 0; i < sz; ++i )
        (*mat)[i] = (F32*)calloc(sz, sizeof(F32));
}

int makeMatrix(taucs_ccs_matrix *A, F32 **mat)
{
    int i, cur_colptr = 0, sz;

    sz = A->n;
    clearMat(mat, sz);

    for ( i = 0; i < A->colptr[sz]; ++i )
    {
        if ( i >= A->colptr[cur_colptr+1] )
            cur_colptr++;
        mat[A->rowind[i]][cur_colptr] = A->taucs_values[i];
    }

    return sz;
}

taucs_ccs_matrix * makeCCSMatrix( F32 **mat, int sz )
{
    taucs_ccs_matrix *A;
    int i, j, nnz = 0, row = 0, col = 0, cur = 0;

    for ( i = 0; i < sz; ++i )
    {
        for ( j = 0; j < i+1; ++j )
        {
            if ( mat[i][j] != 0.0f )
                ++nnz;
        }
    }

    A = taucs_ccs_create( sz, sz, nnz, TAUCS_VALUES_SIZE | TAUCS_SYMMETRIC | TAUCS_LOWER  );

    A->colptr[col++] = 0;

    for ( i = 0; i < sz; ++i )
    {
        //for ( j = i; j < sz; ++j )
        for ( j = 0; j < i+1; ++j )
        {
            if ( mat[i][j] != 0.0f )
            {
                A->taucs_values[cur] = mat[i][j];
                A->rowind[cur++] = j;
            }
        }
        A->colptr[col++] = cur;
    }

    A->colptr[sz] = nnz;

    return A;
}

void printMatrix(F32 **mat, int sz)
{
    int i, j;
    for ( i = 0; i < sz; ++i )
    {
        for ( j = 0; j < sz; ++j )
            printf("%f ", mat[i][j]);
        printf("\n");
    }
}

int primIntersectsNeighbor(Prim *p, int neighbor)
{
    Prim *n = p->neighbors[neighbor];
    int i;
    int nonSharedUVs[2] = {-1,-1}; // [0] == the non shared UV in p, [1] == the one in n
    int shared1[2], shared2[2]; // same as above

    if ( !n || n->cp != p->cp )
        return 0;

    neighborSharedVerts(neighbor, &shared1[0], &shared2[0]);
    nonSharedUVs[0] = 3 - (shared1[0] + shared2[0]);

    for ( i = 0; i < 3; ++i )
    {
        if ( n->neighbors[i] == p )
        {
            neighborSharedVerts(i, &shared1[1], &shared2[1]);
            nonSharedUVs[1] = 3 - (shared1[1] + shared2[1]);
            break;
        }
    }

    if ( lines_cross(p->uv[shared1[0]][0], p->uv[shared1[0]][1], 
        p->uv[nonSharedUVs[0]][0], p->uv[nonSharedUVs[0]][1], 
        n->uv[shared1[1]][0], n->uv[shared1[1]][1], 
        n->uv[nonSharedUVs[1]][0], n->uv[nonSharedUVs[1]][1])
        ||
        lines_are_collinear(p->uv[shared1[0]][0], p->uv[shared1[0]][1], 
        p->uv[nonSharedUVs[0]][0], p->uv[nonSharedUVs[0]][1], 
        n->uv[shared1[1]][0], n->uv[shared1[1]][1], 
        n->uv[nonSharedUVs[1]][0], n->uv[nonSharedUVs[1]][1]) )
        return 1;
    if ( lines_cross(p->uv[shared2[0]][0], p->uv[shared2[0]][1], 
        p->uv[nonSharedUVs[0]][0], p->uv[nonSharedUVs[0]][1], 
        n->uv[shared1[1]][0], n->uv[shared1[1]][1], 
        n->uv[nonSharedUVs[1]][0], n->uv[nonSharedUVs[1]][1])
        ||
        lines_are_collinear(p->uv[shared2[0]][0], p->uv[shared2[0]][1], 
        p->uv[nonSharedUVs[0]][0], p->uv[nonSharedUVs[0]][1], 
        n->uv[shared1[1]][0], n->uv[shared1[1]][1], 
        n->uv[nonSharedUVs[1]][0], n->uv[nonSharedUVs[1]][1]))
        return 1;
    if ( lines_cross(p->uv[shared1[0]][0], p->uv[shared1[0]][1], 
        p->uv[nonSharedUVs[0]][0], p->uv[nonSharedUVs[0]][1], 
        n->uv[shared2[1]][0], n->uv[shared2[1]][1], 
        n->uv[nonSharedUVs[1]][0], n->uv[nonSharedUVs[1]][1])
        ||
        lines_are_collinear(p->uv[shared1[0]][0], p->uv[shared1[0]][1], 
        p->uv[nonSharedUVs[0]][0], p->uv[nonSharedUVs[0]][1], 
        n->uv[shared2[1]][0], n->uv[shared2[1]][1], 
        n->uv[nonSharedUVs[1]][0], n->uv[nonSharedUVs[1]][1]))
        return 1;
    if ( lines_cross(p->uv[shared2[0]][0], p->uv[shared2[0]][1], 
        p->uv[nonSharedUVs[0]][0], p->uv[nonSharedUVs[0]][1], 
        n->uv[shared2[1]][0], n->uv[shared2[1]][1], 
        n->uv[nonSharedUVs[1]][0], n->uv[nonSharedUVs[1]][1])
        ||
        lines_are_collinear(p->uv[shared2[0]][0], p->uv[shared2[0]][1], 
        p->uv[nonSharedUVs[0]][0], p->uv[nonSharedUVs[0]][1], 
        n->uv[shared2[1]][0], n->uv[shared2[1]][1], 
        n->uv[nonSharedUVs[1]][0], n->uv[nonSharedUVs[1]][1]))
        return 1;

    return 0;
}

int mappingIsLegal( CombinedPoly *cp )
{
    int i;

    for ( i = 0; i < eaSize(&cp->primitives); ++i )
    {
        Prim *p = cp->primitives[i];

        if ( primIntersectsNeighbor(p, 0) )
            return 0;
        if ( primIntersectsNeighbor(p, 1) )
            return 0;
        if ( primIntersectsNeighbor(p, 2) )
            return 0;
    }

    return 1;
}


void solveCP( CombinedPoly *cp )
{
    taucs_ccs_matrix *A;
    F32 *x = NULL, *b = NULL;
    int numB = 2, matSize, numNonZero = 0, i, j, uniform_constants = 0;
    F32 **mat, **matTemp, **matTrans, *vec;
    static int numCalls = 0;
    char *options[] = {"taucs.factor.LLT=true"/*,"taucs.factor.ordering=metis"*/, NULL};
    VertexGraph *graph;

    graph = createCPVertexGraph(cp);
    matSize = eaSize(&graph->nodes);
    generateVertexGraphConstants(graph);
    numNonZero = countConstants(graph);

    cp->fails = 0;

    allocMatrix(&mat, matSize);
    allocMatrix(&matTrans, matSize);
    allocMatrix(&matTemp, matSize);

    b = (F32*)calloc(eaSize(&graph->nodes) * 2, sizeof(F32));
    x = (F32*)calloc(eaSize(&graph->nodes) * 2, sizeof(F32));


redo_solve:

    if ( uniform_constants )
    {
        memset(b, 0, sizeof(float) * eaSize(&graph->nodes) * 2);
        memset(x, 0, sizeof(float) * eaSize(&graph->nodes) * 2);
    }

    A = taucs_ccs_create( matSize, matSize, numNonZero, TAUCS_VALUES_SIZE );
    fillTaucsMatrix( graph, A, uniform_constants );

    setCorners( graph, cp, A, b, numNonZero );
    fixMatrix(&A, numNonZero);
    
    makeMatrix( A, mat );
    copyMat(mat, matTrans, matSize);
    transposeMat(matTrans, matSize);
    mulMat(mat, matTrans, matTemp, matSize);
    allocVec(&vec, matSize);

    mulMatVec(mat, b, vec, matSize);
    memcpy(b, vec, sizeof(F32) * matSize);
    mulMatVec(mat, &b[matSize], vec, matSize);
    memcpy(&b[matSize], vec, sizeof(F32) * matSize);
    taucs_ccs_free(A);
    A = makeCCSMatrix(matTemp, matSize);
    
    i = taucs_linsolve( A, NULL, numB, x, b, options, NULL );
    ++numCalls;
    if (i != TAUCS_SUCCESS)
    {
        makeMatrix(A, mat);
        printMatrix(mat, A->n);
        printf ("Solution error.\n");
        if (i==TAUCS_ERROR)
        printf ("Generic error.");
        if (i==TAUCS_ERROR_NOMEM)
        printf ("NOMEM error.");
        if (i==TAUCS_ERROR_BADARGS)
        printf ("BADARGS error.");
        if (i==TAUCS_ERROR_MAXDEPTH)
        printf ("MAXDEPTH error.");
        if (i==TAUCS_ERROR_INDEFINITE)
        printf ("NOT POSITIVE DEFINITE error.");
        assert(0);
    }
    taucs_ccs_free(A);

    for ( i = 0; i < eaSize(&graph->nodes); ++i )
    {
        graph->nodes[i]->map[0] = x[i];
        graph->nodes[i]->map[1] = x[i + eaSize(&graph->nodes)];
    }

    // apply the mapping to the primitives in the cp
    for ( i = 0; i < eaSize(&cp->primitives); ++i )
    {
        Prim * prim = cp->primitives[i];
        for ( j = 0; j < eaSize(&graph->nodes); ++j )
        {
            VertexGraphNode *node = graph->nodes[j];
            if ( node->vert[0] == prim->pos[0][0] && node->vert[1] == prim->pos[0][1] && node->vert[2] == prim->pos[0][2] )
            {
                prim->uv[0][0] = node->map[0];
                prim->uv[0][1] = node->map[1];
            }
            if ( node->vert[0] == prim->pos[1][0] && node->vert[1] == prim->pos[1][1] && node->vert[2] == prim->pos[1][2] )
            {
                prim->uv[1][0] = node->map[0];
                prim->uv[1][1] = node->map[1];
            }
            if ( node->vert[0] == prim->pos[2][0] && node->vert[1] == prim->pos[2][1] && node->vert[2] == prim->pos[2][2] )
            {
                prim->uv[2][0] = node->map[0];
                prim->uv[2][1] = node->map[1];
            }
        }
    }

    cpCalcExtents(cp);
    cp->offsetU = cp->minU;
    cp->offsetV = cp->minV;

    if ( !mappingIsLegal(cp) )
    {
        // the mapping failed, redo the algorithm with uniform constants
        uniform_constants = 1;
        ++cp->fails;
        goto redo_solve;
    }

    cp->fails = 0;

    freeMat(mat, matSize);
    freeMat(matTrans, matSize);
    freeMat(matTemp, matSize);
    free(vec);
    free(b);
    free(x);
    freeVertexGraph(graph);

    if ( 0 )
    {
        int num = 0;
        cpNormalizeOffsetsFinalize(cp);
        drawTGAStart();
        drawCPTGANoFinal(cp, 10, red);
        drawTGAEnd("C:\\cps.tga");
        num = 0;
    }
}


static void buildCombinedPolygons2(Prim ***primitives, CombinedPoly ***combinedpolys)
{
    Prim **prims = 0;
    MergeOperation **mergeQueue, *newMergeOp;
    int i, j, size = eaSize(primitives);
    static int calls = 0;

    minCompact=1e15, maxCompact=0.0f, minPlanar=1e15, maxPlanar=0.0f;
    minCost=1e15, maxCost=0.0f;
    numCosts = 0;

    eaCopy(&prims, primitives);

//PERFINFO_AUTO_START("build_neighbor_lists",1);
    // build neighbor lists
    for (i = 0; i < size; i++)
    {
        Prim *prim0 = (*primitives)[i];
        if (prim0->is_bad)
            continue;
        for (j = i + 1; j < size; j++)
        {
            int e0, e1;
            Prim *prim1 = (*primitives)[j];
            if (prim1->is_bad)
                continue;
            if (share2verts(prim0, prim1, &e0, &e1))
            {
                prim0->neighbors[e0] = prim1;
                prim1->neighbors[e1] = prim0;
            }
        }
    }
//PERFINFO_AUTO_STOP();

//PERFINFO_AUTO_START("initialize_merges",1);
    eaCreate(combinedpolys);
    eaSetCapacity(combinedpolys, size);

    // initialize all of the triangles into their own cps
    for ( i = 0; i < size; ++i )
    {
        CombinedPoly * newCp;
        newCp = cpCreate();
        cpPushPrim(newCp, (*primitives)[i]);
        (*primitives)[i]->cp = newCp;
        eaPush(combinedpolys, newCp);
    }
    //assertPrimsAreInCorrectCPs(*combinedpolys);
    // i know its the same... i am paranoid, OK?
    size = eaSize(combinedpolys);
    eaCreate(&mergeQueue);
    // enqueue the merge operations between neighbors
    for ( i = 0; i < size; ++i )
    {
        for ( j = 0; j < 3; ++j )
        {
            CombinedPoly *cpA, *cpB;
            cpA = (*combinedpolys)[i];
            
            if ( cpA->primitives[0]->neighbors[j] )
            {
                cpB = (CombinedPoly*)cpA->primitives[0]->neighbors[j]->cp;
                newMergeOp = newMergeOperation( cpA , cpB );
                if ( !enqueueMergeOperation(&mergeQueue, combinedpolys, newMergeOp) )
                    deleteMergeOperation( newMergeOp );
            }
        }
    }

//PERFINFO_AUTO_STOP();

//PERFINFO_AUTO_START("mergeBest",1);

    // merge until the queue is empty
    while ( mergeBestAndUpdateNeighbors( &mergeQueue, combinedpolys ) )
    {
        ++calls;
    }

//PERFINFO_AUTO_STOP();

//PERFINFO_AUTO_START("solveCPs",1);
    size = eaSize(combinedpolys);
    // solve for the projection of the CPs into 2D
    for ( i = 0; i < size; ++i )
    {
        solveCP((*combinedpolys)[i]);
    }
//PERFINFO_AUTO_STOP();

    avgCost = (maxCost - minCost)/numCosts;
    freeAllMergeOperations();
}


float uvunwrap(Prim ***primitives)
{
    int i, prim_count, cp_count, min_size;
    float cp_size, prim_area, increment = 40;
    CombinedPoly **cp_list = 0;

    //freopen("C:\\uvunwrap.txt", "w", stdout);

    // assign ids to the primitves so we can reorder them
    prim_count = eaSize(primitives);

    //for (i = 0; i < prim_count; i++)
    //{
    //    (*primitives)[i]->debug_id = i;
    //    primProjectUVs((*primitives)[i]);
    //}
    timerRecordStart(NULL);
    PERFINFO_AUTO_START("UV",1);
    PERFINFO_AUTO_START("uvunwrap",1);
    //eaCreate(&cp_list);
    //buildCombinedPolygons(primitives, &cp_list);
    buildCombinedPolygons2(primitives, &cp_list);
    PERFINFO_AUTO_STOP();

    cp_count = eaSize(&cp_list);

    for ( i = 0; i < cp_count; ++i )
    {
        if ( *((int*)&cp_list[i]->primitives[0]->uv[0][0]) == -4194304 )
        {
            assert( 0 );
            i = i;
        }
    }

    if (!cp_count)
        return 2;

    // generate texcoords for each CombinedPolygon, find initial min size
    cp_size = 0;
    for (i = 0; i < cp_count; i++)
    {
        cp_size = MAX(cp_size, cp_list[i]->maxU - cp_list[i]->minU);
        cp_size = MAX(cp_size, cp_list[i]->maxV - cp_list[i]->minV);
    }

    qsortG(cp_list, cp_count, sizeof(*cp_list), cpCompare);

    prim_area = 0;
    for (i = 0; i < prim_count; i++)
    {
        if ((*primitives)[i]->is_bad)
            continue;
        prim_area += (*primitives)[i]->area;
    }
    prim_area = sqrtf(prim_area);

    min_size = ceil(MAX(cp_size, prim_area));

PERFINFO_AUTO_START("uvpack",1);

#if POWEROFTWO_OUTPUT
    min_size = pow2(min_size);
    while (1)
    {
        if (tryPopulateTextureMap2(&cp_list, min_size))
            break;

        min_size *= 2;
#else
    increment = 40;
    while (1)
    {
        if (min_size > 0 && tryPopulateTextureMap2(&cp_list, min_size))
        {
            if (increment <= 2.5)
                break;
            min_size -= increment;
            increment *= 0.5f;
        }

        min_size += increment;
#endif
    }

PERFINFO_AUTO_STOP();
PERFINFO_AUTO_STOP();
timerRecordEnd();
//
//simpleLogClear();
//printPerformanceInfo();

    for (i = 0; i < cp_count; i++)
        cpFinalize(cp_list[i]);

    for (i = 0; i < prim_count; i++)
    {
        Prim *p = (*primitives)[i];
        if (p->is_bad)
            continue;
        primScaleUVs(p, min_size);
        p->debug_id = 0;
    }

    //printAllCPs(&cp_list, "C:\\cps");
    eaDestroyEx(&cp_list, cpDestroy);

    return min_size;
}



