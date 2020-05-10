#include "graphics/zOcclusion.h"
#include "render/renderprim.h"
#include "render/tex_gen.h"
#include "render/sprite.h"
#include "render/thread/ogl.h"
#include "win/win_init.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/SuperAssert.h>
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/utils/timing.h>
#include "graphics/font.h"
#include "graphics/textureatlas.h"
#include "graphics/gfxwindow.h"
#include <utilitieslib/components/BitArray.h>
#include "edit/edit_drawlines.h"
#include <utilitieslib/utils/utils.h>

#include <utilitieslib/utils/ssemath.h>

#define RT_ALLOW_VBO
#include "render/model_cache.h"
#include "render/model.h"
#include "render/tex.h"

#include "zOcclusionTypes.h"
#include "zOcclusionClip.h"

#if 0
#define CHECKHEAP     {assert(heapValidateAll());}
#else
#define CHECKHEAP {}
#endif

MP_DEFINE(Occluder);

#define MAX_UNIQUE_OBJECTS 2000000

////////////////////////////////////////////////////
////////////////////////////////////////////////////

static void zoDrawModel(Model *m, Mat4 matModelToEye);

////////////////////////////////////////////////////
////////////////////////////////////////////////////

// frame info
static Mat44 zo_projection_mat;
static Mat4 zo_eye_mat, zo_inv_eye_mat;
float zo_near_plane = 0.7;

// zbuffer
ZType *zo_zbuf = 0;
static int zo_size;
static float zo_width_mul, zo_height_mul;

// zbuffer debug texture
static BasicTexture *zo_zbuf_tex = 0;

// occluders
static OccluderPtr zo_last_occluders[MAX_OCCLUDERS];
static OccluderPtr zo_occluders[MAX_OCCLUDERS];
static float zo_min_occluder_screenspace = 0;
static int zo_num_occluders = 0, zo_last_num_occluders = 0;
static float zo_last_screen_space = 0;

int zo_occluder_tris_drawn = 0;
static int zo_num_tests = 0, zo_num_test_calls = 0;

// hierarchical z buffer
static HierarchicalZBuffer *zo_zhierarchy_arrays = 0;
static int zo_zhierarchy_numarrays = 0;

static int zo_frame = 0;
static BitArray zo_occluded_bits[3] = {0};
float zo_jitter_amount = 0;

static ZType zo_minz, zo_maxz;

////////////////////////////////////////////////////
////////////////////////////////////////////////////

static INLINEDBG Occluder *createOccluder(Model *m, Mat4 modelToWorld, float screenSpace, int uid)
{
    Occluder *o;

    MP_CREATE(Occluder, 100);

    o = MP_ALLOC(Occluder);
    o->m = m;
    copyMat4(modelToWorld, o->modelToWorld);
    o->screenSpace = screenSpace;
    o->uid = uid;
    return o;
}

static INLINEDBG void freeOccluder(Occluder *o)
{
    MP_FREE(Occluder, o);
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////

static INLINEDBG void clipToZ(Vec3 inside, Vec3 outside, Vec3 tpos)
{
    Vec3 diff;
    float t;

    subVec3(inside, outside, diff);
    t = (-zo_near_plane - outside[2]) / diff[2];

    scaleVec3(diff, t, diff);
    addVec3(outside, diff, tpos);
}

static int getScreenExtents(Vec3 transBounds[8], float *fminX, float *fminY, float *fmaxX, float *fmaxY, ZType *minZ)
{
    int        posClipped[8];
    int        x,y,z, zclipped = 0, idx, idx2;
    Vec3    tpos, tpos2;
    Vec3    tmin, tmax;

    setVec3(tmin, 1, 1, 1);
    setVec3(tmax, -1, -1, -1);

    for(idx=0;idx<8;idx++)
    {
        if (transBounds[idx][2] > -zo_near_plane)
        {
            posClipped[idx] = 1;
            zclipped++;
        }
        else
        {
            posClipped[idx] = 0;
        }
    }

    if (zclipped == 8)
        return 0;

    for(z=0;z<2;z++)
    {
        for(y=0;y<2;y++)
        {
            for(x=0;x<2;x++)
            {
                idx = boundsIndex(x, y, z);
                if (!posClipped[idx])
                {
                    mulVec3Mat44(transBounds[idx], zo_projection_mat, tpos2);
                    vec3RunningMinMax(tpos2, tmin, tmax);

                    idx2 = boundsIndex(x, y, !z);
                    if (posClipped[idx2])
                    {
                        clipToZ(transBounds[idx], transBounds[idx2], tpos);
                        mulVec3Mat44(tpos, zo_projection_mat, tpos2);
                        vec3RunningMinMax(tpos2, tmin, tmax);
                    }

                    idx2 = boundsIndex(x, !y, z);
                    if (posClipped[idx2])
                    {
                        clipToZ(transBounds[idx], transBounds[idx2], tpos);
                        mulVec3Mat44(tpos, zo_projection_mat, tpos2);
                        vec3RunningMinMax(tpos2, tmin, tmax);
                    }

                    idx2 = boundsIndex(!x, y, z);
                    if (posClipped[idx2])
                    {
                        clipToZ(transBounds[idx], transBounds[idx2], tpos);
                        mulVec3Mat44(tpos, zo_projection_mat, tpos2);
                        vec3RunningMinMax(tpos2, tmin, tmax);
                    }
                }
            }
        }
    }

    if (tmin[0] >= 1 || tmax[0] <= -1)
        return 0;

    if (tmin[1] >= 1 || tmax[1] <= -1)
        return 0;

    if (tmin[2] >= 1)
        return 0;

    if (tmin[0] < -1)
        tmin[0] = -1;
    if (tmin[1] < -1)
        tmin[1] = -1;
    if (tmax[0] > 1)
        tmax[0] = 1;
    if (tmax[1] > 1)
        tmax[1] = 1;

    *fminX = tmin[0] * 0.5f + 0.5f;
    *fminY = tmin[1] * 0.5f + 0.5f;
    *fmaxX = tmax[0] * 0.5f + 0.5f;
    *fmaxY = tmax[1] * 0.5f + 0.5f;

    *minZ = tmin[2] * ZMAX;

    // some are clipped by z, but not all, so pass the bounds as visible (camera is inside the bounds)
    if (zclipped)
        return 2;

    return 1;
}

static int getScreenExtentsNoZClip(Vec3 transBounds[8], float *fminX, float *fminY, float *fmaxX, float *fmaxY, ZType *minZ)
{
    Vec3    tpos;
    Vec3    tmin, tmax;

    setVec3(tmin, 1, 1, 1);
    setVec3(tmax, -1, -1, -1);

    mulVec3Mat44(transBounds[0], zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    mulVec3Mat44(transBounds[1], zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    mulVec3Mat44(transBounds[2], zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    mulVec3Mat44(transBounds[3], zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    mulVec3Mat44(transBounds[4], zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    mulVec3Mat44(transBounds[5], zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    mulVec3Mat44(transBounds[6], zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    mulVec3Mat44(transBounds[7], zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    if (tmin[0] >= 1 || tmax[0] <= -1)
        return 0;

    if (tmin[1] >= 1 || tmax[1] <= -1)
        return 0;

    if (tmin[2] >= 1)
        return 0;

    tmin[0] -= ZB_ONE_OVER_DIM;
    tmin[1] -= ZB_ONE_OVER_DIM;
    tmax[0] += ZB_ONE_OVER_DIM;
    tmax[1] += ZB_ONE_OVER_DIM;

    if (tmin[0] < -1)
        tmin[0] = -1;
    if (tmin[1] < -1)
        tmin[1] = -1;
    if (tmax[0] > 1)
        tmax[0] = 1;
    if (tmax[1] > 1)
        tmax[1] = 1;

    *fminX = tmin[0] * 0.5f + 0.5f;
    *fminY = tmin[1] * 0.5f + 0.5f;
    *fmaxX = tmax[0] * 0.5f + 0.5f;
    *fmaxY = tmax[1] * 0.5f + 0.5f;

    *minZ = tmin[2] * ZMAX;

    return 1;
}

static void getSphereScreenExtents(Vec3 centerEye, float radius, float *fminX, float *fminY, float *fmaxX, float *fmaxY, ZType *minZ)
{
    Vec3 center;
    Vec3 tpos, tmin, tmax;
    float radius2 = radius + radius;

    setVec3(tmin, 1, 1, 1);
    setVec3(tmax, -1, -1, -1);

    copyVec3(centerEye, center);

    center[2] += radius;
    mulVec3Mat44(center, zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    center[2] -= radius2;
    mulVec3Mat44(center, zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    center[0] -= radius;
    center[1] -= radius;
    center[2] += radius;
    mulVec3Mat44(center, zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    center[0] += radius2;
    mulVec3Mat44(center, zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    center[1] += radius2;
    mulVec3Mat44(center, zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

    center[0] -= radius2;
    mulVec3Mat44(center, zo_projection_mat, tpos);
    vec3RunningMinMax(tpos, tmin, tmax);

#if ZO_SAFE
    assert(tmin[0] <= 1 && tmax[0] >= -1);
    assert(tmin[1] <= 1 && tmax[1] >= -1);
#endif

    if (tmin[0] < -1)
        tmin[0] = -1;
    if (tmin[1] < -1)
        tmin[1] = -1;
    if (tmax[0] > 1)
        tmax[0] = 1;
    if (tmax[1] > 1)
        tmax[1] = 1;

    *fminX = tmin[0] * 0.5f + 0.5f;
    *fminY = tmin[1] * 0.5f + 0.5f;
    *fmaxX = tmax[0] * 0.5f + 0.5f;
    *fmaxY = tmax[1] * 0.5f + 0.5f;
    *minZ = tmin[2] * ZMAX;
}

// Stores the minimum value of pVal1 and pVal2 into pMin
// This uses SSE and is branchless
static INLINEDBG void MIN2SSE(float *pMin, const float *pVal1, const float *pVal2)
{
    register __m128 val1;
    register __m128 val2;

    val1 = _mm_load_ss(pVal1);
    val2 = _mm_load_ss(pVal2);

    val1 = _mm_min_ss(val1, val2);

    _mm_store_ss(pMin, val1);
}

// Stores the minimum value of pVal1 and pVal2 and pVal3 into pMin
// This uses SSE and is branchless
static INLINEDBG void MIN3SSE(float *pMin, const float *pVal1, const float *pVal2, const float *pVal3)
{
    register __m128 val1;
    register __m128 val2;
    register __m128 val3;

    val1 = _mm_load_ss(pVal1);
    val2 = _mm_load_ss(pVal2);
    val3 = _mm_load_ss(pVal3);

    val1 = _mm_min_ss(val1, val2);
    val1 = _mm_min_ss(val1, val3);

    _mm_store_ss(pMin, val1);
}


// Stores the maximum value of pVal1 and pVal2 into pMax
// This uses SSE and is branchless
static INLINEDBG void MAX2SSE(float *pMax, const float *pVal1, const float *pVal2)
{
    register __m128 val1;
    register __m128 val2;

    val1 = _mm_load_ss(pVal1);
    val2 = _mm_load_ss(pVal2);

    val1 = _mm_max_ss(val1, val2);

    _mm_store_ss(pMax, val1);
}


// Stores the maximum value of pVal1 and pVal2 and pVal3 into pMax
// This uses SSE and is branchless
static INLINEDBG void MAX3SSE(float *pMax, const float *pVal1, const float *pVal2, const float *pVal3)
{
    register __m128 val1;
    register __m128 val2;
    register __m128 val3;

    val1 = _mm_load_ss(pVal1);
    val2 = _mm_load_ss(pVal2);
    val3 = _mm_load_ss(pVal3);

    val1 = _mm_max_ss(val1, val2);
    val1 = _mm_max_ss(val1, val3);

    _mm_store_ss(pMax, val1);
}


static void filterDown(ZType *minStart, ZType *maxStart, int dim, ZType *minStart2, ZType *maxStart2, int dim2)
{
    int evenLine, x, y;
    ZType *line, *line2;

    // min
    evenLine = 1;
    for (y = 0; y < dim; y++)
    {
        line2 = minStart2 + ((int)(y/2)) * dim2;
        line = minStart + y * dim;

        if (evenLine)
        {
            for (x = 0; x < dim; x+=2)
            {
                MIN2SSE(line2, line + 0, line + 1);
                line+=2;
                line2++;
            }
        }
        else
        {
            for (x = 0; x < dim; x+=2)
            {
                MIN3SSE(line2, line2, line + 0, line + 1);
                line+=2;
                line2++;
            }
        }

        evenLine = !evenLine;
    }

    // max
    evenLine = 1;
    for (y = 0; y < dim; y++)
    {
        line2 = maxStart2 + ((int)(y/2)) * dim2;
        line = maxStart + y * dim;

        if (evenLine)
        {
            for (x = 0; x < dim; x+=2)
            {
                MAX2SSE(line2, line + 0, line + 1);
                line+=2;
                line2++;
            }
        }
        else
        {
            for (x = 0; x < dim; x+=2)
            {
                MAX3SSE(line2, line2, line + 0, line + 1);
                line+=2;
                line2++;
            }
        }

        evenLine = !evenLine;
    }
}

#define elem(x,y,width) ((x)+((y)*width))
static void createZHierarchy(void)
{
    int i;

    PERFINFO_AUTO_START("createZHierarchy", 1);

    if (!zo_zhierarchy_arrays)
    {
        zo_zhierarchy_numarrays = log2(ZB_DIM);
        zo_zhierarchy_arrays = malloc(zo_zhierarchy_numarrays * sizeof(*zo_zhierarchy_arrays));

        for (i = 0; i < zo_zhierarchy_numarrays; i++)
        {
            int size;
            zo_zhierarchy_arrays[i].dim = 1 << i;
            size = zo_zhierarchy_arrays[i].dim * zo_zhierarchy_arrays[i].dim;
            zo_zhierarchy_arrays[i].minZ = calloc(size, sizeof(*zo_zhierarchy_arrays[i].minZ));
            zo_zhierarchy_arrays[i].maxZ = calloc(size, sizeof(*zo_zhierarchy_arrays[i].maxZ));
        }
    }

    // do the high res buffer (one less than the zbuffer)
    i = zo_zhierarchy_numarrays-1;
    filterDown(zo_zbuf, zo_zbuf, ZB_DIM, zo_zhierarchy_arrays[i].minZ, zo_zhierarchy_arrays[i].maxZ, zo_zhierarchy_arrays[i].dim);

    // fill in the other buffers
    for (i = zo_zhierarchy_numarrays-2; i >= 0; i--)
    {
        filterDown(zo_zhierarchy_arrays[i+1].minZ, zo_zhierarchy_arrays[i+1].maxZ, zo_zhierarchy_arrays[i+1].dim, zo_zhierarchy_arrays[i].minZ, zo_zhierarchy_arrays[i].maxZ, zo_zhierarchy_arrays[i].dim);
    }

    zo_minz = *zo_zhierarchy_arrays[0].minZ;
    zo_maxz = *zo_zhierarchy_arrays[0].maxZ;

    PERFINFO_AUTO_STOP();
}
#undef elem

static INLINEDBG int testZRect(ZType z, int minX, int minY, int maxX, int maxY, ZType *buffer, int width)
{
    int y, n, linesize;
    ZType *line, *ptr;

    linesize = maxX - minX;
    line = buffer + minY * width + minX;

    for (y = minY; y < maxY; y++)
    {
        ptr = line;
        n = linesize;
        while (n > 3)
        {
            if (ZTEST(z, ptr[0]))
                return 1;
            if (ZTEST(z, ptr[1]))
                return 1;
            if (ZTEST(z, ptr[2]))
                return 1;
            if (ZTEST(z, ptr[3]))
                return 1;
            ptr+=4;
            n-=4;
        }
        while (n)
        {
            if (ZTEST(z, *ptr))
                return 1;
            ptr++;
            n--;
        }

        line += width;
    }

    return 0;
}

static int testZRectHierarchical(float x1, float y1, float x2, float y2, ZType z)
{
    int i, width, minX, minY, maxX, maxY;
    
    if (!zo_zhierarchy_arrays)
        return 1;
    
    if (ZTEST(z, zo_minz))
        return 1;
    if (!ZTEST(z, zo_maxz))
        return 0;

    for (i = 0; i < ZO_START_HOFFSET; i++)
    {
        x1 *= 2;
        y1 *= 2;
        x2 *= 2;
        y2 *= 2;
    }
    for (i = ZO_START_HOFFSET; i < zo_zhierarchy_numarrays - ZO_END_HOFFSET - 1; i++)
    {
        minX = floor(x1);
        minY = floor(y1);
        maxX = ceil(x2);
        maxY = ceil(y2);
        width = zo_zhierarchy_arrays[i].dim;

        // if it is in front of the minZ, it is visible
        if (testZRect(z, minX, minY, maxX, maxY, zo_zhierarchy_arrays[i].minZ, width))
            return 1;

        // if it is behind the maxZ, it is not visible
        if (!testZRect(z, minX, minY, maxX, maxY, zo_zhierarchy_arrays[i].maxZ, width))
            return 0;

        // between min and max, go to next level
        x1 *= 2;
        y1 *= 2;
        x2 *= 2;
        y2 *= 2;
    }

    // last level of the hierarchy, we can just test against max
    minX = floor(x1);
    minY = floor(y1);
    maxX = ceil(x2);
    maxY = ceil(y2);
    width = zo_zhierarchy_arrays[i].dim;

    return testZRect(z, minX, minY, maxX, maxY, zo_zhierarchy_arrays[i].maxZ, width);
}

static int testZRectNonHierarchal(int x1, int y1, int x2, int y2, ZType z)
{
    return testZRect(z, x1, y1, x2, y2, zo_zbuf, ZB_DIM);
}


////////////////////////////////////////////////////
////////////////////////////////////////////////////

int zoIsSupported(void)
{
    return sseAvailable & SSE_2;
}

BasicTexture *zoGetZBuffer(void)
{
    U8 *buf, *p;
    int x, y;
    ZType zval;

    if (!zo_zbuf)
        return 0;

    if (!zo_zbuf_tex)
        zo_zbuf_tex = texGenNew(ZB_DIM, ZB_DIM, "zOcclusion ZBuffer");

    buf = malloc(ZB_DIM * ZB_DIM * 4);
    for (y = 0; y < ZB_DIM; y++)
    {
        p = buf + ((ZB_DIM - 1 - y) * ZB_DIM * 4);
        for (x = 0; x < ZB_DIM; x++)
        {
            zval = zo_zbuf[x + y * ZB_DIM];
            p[0] = ZTOR(zval);
            p[1] = ZTOG(zval);
            p[2] = ZTOB(zval);
            p[3] = ZTOALPHA(zval);
            p+=4;
        }
    }

    texGenUpdateEx(zo_zbuf_tex, buf, GL_RGBA, 0);

    free(buf);

    return zo_zbuf_tex;
}

void zoShowDebug(void)
{
    int line = 10; // put it below any failtext

    if (!zo_zbuf)
        return;

    xyprintf(0,line, "Z-Occlusion: %s", game_state.zocclusion?"On":"Off");
    line++;

    if (zo_last_num_occluders < (MAX_OCCLUDERS * 0.99f))
        xyprintf(0,line, "Occluders: %d", zo_last_num_occluders);
    else
        xyprintfcolor(0,line, 255, 0, 0, "Occluders: %d", zo_last_num_occluders);
    line++;

    if (zo_occluder_tris_drawn < (MAX_OCCLUDER_TRIS * 0.99f))
        xyprintf(0,line, "Occluder Tris: %d", zo_occluder_tris_drawn);
    else
        xyprintfcolor(0,line, 255, 0, 0, "Occluder Tris: %d", zo_occluder_tris_drawn);
    line++;

    xyprintf(0,line, "Occlusion Tests: %d/%d", zo_num_tests, zo_num_test_calls);
    line++;

    if (game_state.zocclusion_debug == 2)
        display_sprite_tex(zoGetZBuffer(), 0, 0, 1, 1.f/zo_width_mul, 1.f/zo_height_mul, 0xffffff7f);
    else if (game_state.zocclusion_debug == 3)
        display_sprite_tex(zoGetZBuffer(), 0, 0, 1, 1.f/zo_width_mul, 1.f/zo_height_mul, 0xffffffff);
    else if (game_state.zocclusion_debug == 4)
        display_sprite_tex(zoGetZBuffer(), 128, 128, 1, 1.f, 1.f, 0xffffffff);

#if 1
    // Draw bounding boxes around active occluders
    {
        int i;
        for (i = 0; i < zo_last_num_occluders; i++)
        {
            Vec3 bounds[8];
            Model *pModel = zo_last_occluders[i]->m;
            mulBounds(pModel->min, pModel->max, zo_last_occluders[i]->modelToWorld, bounds);

            drawLine3DWidth(bounds[0], bounds[1], 0xFFFF0000, 1.0f);
            drawLine3DWidth(bounds[0], bounds[2], 0xFFFF0000, 1.0f);
            drawLine3DWidth(bounds[0], bounds[4], 0xFFFF0000, 1.0f);

            drawLine3DWidth(bounds[1], bounds[5], 0xFFFF0000, 1.0f);
            drawLine3DWidth(bounds[2], bounds[6], 0xFFFF0000, 1.0f);

            drawLine3DWidth(bounds[3], bounds[1], 0xFFFF0000, 1.0f);
            drawLine3DWidth(bounds[3], bounds[2], 0xFFFF0000, 1.0f);

            drawLine3DWidth(bounds[4], bounds[5], 0xFFFF0000, 1.0f);
            drawLine3DWidth(bounds[4], bounds[6], 0xFFFF0000, 1.0f);

            drawLine3DWidth(bounds[7], bounds[6], 0xFFFF0000, 1.0f);
            drawLine3DWidth(bounds[7], bounds[5], 0xFFFF0000, 1.0f);
            drawLine3DWidth(bounds[7], bounds[3], 0xFFFF0000, 1.0f);
        }
    }
#endif
}

void zoNotifyModelFreed(Model *m)
{
    int i,j;
    for (i = 0; i < zo_num_occluders; i++)
    {
        if (m == zo_occluders[i]->m)
        {
            freeOccluder(zo_occluders[i]);
            zo_num_occluders--;
            for (j = i; j < zo_num_occluders; j++)
                zo_occluders[j] = zo_occluders[j+1];
        }
    }
}

void zoClearOccluders(void)
{
    int i;

    zo_occluder_tris_drawn = 0;

    for (i = 0; i < zo_num_occluders; i++)
        freeOccluder(zo_occluders[i]);

    for (i = 0; i < zo_last_num_occluders; i++)
        freeOccluder(zo_last_occluders[i]);

    zo_last_num_occluders = zo_num_occluders = 0;
    zo_min_occluder_screenspace = 0;

#if ZO_USE_JITTER
    if (zo_occluded_bits[0])
    {
        baClearAllBits(zo_occluded_bits[0]);
        baClearAllBits(zo_occluded_bits[1]);
        baClearAllBits(zo_occluded_bits[2]);
    }
#endif
}

//#define ZO_DEBUG_STATS_FRAMES 100
#define ZO_DEBUG_STATS_FRAMES 0

void zoInitFrame(Mat44 matProjection, Mat4 matWorldToEye, Mat4 matEyeToWorld, F32 nearPlaneDist)
{
    int window_width, window_height, i;
    Mat4 matModelToEye;

#if ZO_DEBUG_STATS_FRAMES
    static int zoDebugCurrentFrame = 0;
    static int zoDebugNumFrames = 0;
    static LARGE_INTEGER zoDebugFrequency = {0,0};
    static LARGE_INTEGER zoDebugTotalTicks = {0,0};
    static LARGE_INTEGER zoDebugLargestTicks = {0,0};
    static LARGE_INTEGER zoDebugDrawTotalTicks = {0,0};
    LARGE_INTEGER zoDebugStartTicks;

    if (zoDebugFrequency.QuadPart == 0)
    {
        QueryPerformanceFrequency(&zoDebugFrequency);
    }

    QueryPerformanceCounter(&zoDebugStartTicks);
#endif

    PERFINFO_AUTO_START("zoInitFrame", 1);

    CHECKHEAP;

#if ZO_USE_JITTER
    zo_frame++;
    if (zo_frame == 3)
        zo_frame = 0;

    if (zo_frame == 0)
        zo_jitter_amount = 0.f;
    else if (zo_frame == 1)
        zo_jitter_amount = 0.5f;
    else
        zo_jitter_amount = -0.5f;


    if (!zo_occluded_bits[0])
    {
        zo_occluded_bits[0] = createBitArray(MAX_UNIQUE_OBJECTS);
        zo_occluded_bits[1] = createBitArray(MAX_UNIQUE_OBJECTS);
        zo_occluded_bits[2] = createBitArray(MAX_UNIQUE_OBJECTS);
    }
#endif

    // HYPNOS_AKI: SSE optimized initialization
#if 0
    if (!zo_zbuf)
    {
        zo_size = ZB_DIM * ZB_DIM;
        zo_zbuf = malloc(zo_size * sizeof(*zo_zbuf));
    }

    for (i = 0; i < zo_size; i++)
        zo_zbuf[i] = ZINIT;
#else
    // Assumes ZType is a float (drawZLine() also assumes this)
    assert((ZB_DIM & 3) == 0);

    if (!zo_zbuf)
    {
        zo_size = ZB_DIM * ZB_DIM;
        zo_zbuf = _aligned_malloc(zo_size * sizeof(*zo_zbuf), 16);
    }

    {
        register sseVec4 vecInit;
        register sseVec4 *pVecCurrent = (sseVec4 *)zo_zbuf;
        sse_init4_all(vecInit, ZINIT);

        for (i = 0; i < zo_size; i+=4)
        {
            sse_store4(vecInit, pVecCurrent);
            pVecCurrent++;
        }
    }
#endif

    windowClientSize(&window_width, &window_height);
    zo_width_mul = (float)ZB_DIM / (float)window_width;
    zo_height_mul = (float)ZB_DIM / (float)window_height;

    copyMat44(matProjection, zo_projection_mat);
    copyMat4(matWorldToEye, zo_eye_mat);
    copyMat4(matEyeToWorld, zo_inv_eye_mat);
    zo_near_plane = nearPlaneDist;

#if ZO_USE_JITTER
//    zo_projection_mat[0][0] += zo_jitter_amount * -0.25f;
//    zo_projection_mat[1][1] += zo_jitter_amount * -0.25f;

    baClearAllBits(zo_occluded_bits[zo_frame]);
#endif

    // draw and reset occluder queue
    for (i = 0; i < zo_last_num_occluders; i++)
    {
        freeOccluder(zo_last_occluders[i]);
    }
    zo_last_num_occluders = 0;

    zo_occluder_tris_drawn = 0;
    for (i = 0; i < zo_num_occluders; i++)
    {
        if ((zo_occluder_tris_drawn + zo_occluders[i]->m->tri_count) < MAX_OCCLUDER_TRIS && zo_occluders[i]->m->vert_count < MAX_OCCLUDER_VERTS)
        {
            int occluded = 0;
            static int zoDoOccluderOccludedTest = 0;

            mulMat4Inline(matWorldToEye, zo_occluders[i]->modelToWorld, matModelToEye);

            // Additional check to see if the occluder is occluded
#if 0
            if (zoDoOccluderOccludedTest)
            {
                // From groupdrawinline.c drawDefInternal()
                int nearClipped;
                Vec3 min, max, eye_bounds[8];

                Model *pModel = zo_occluders[i]->m;

                copyVec3(pModel->min, min);
                copyVec3(pModel->max, max);
                min[0] -= 0.9;
                min[1] -= 0.9;
                min[2] -= 0.9;
                max[0] += 0.9;
                max[1] += 0.9;
                max[2] += 0.9;

                mulBounds(min, max, matModelToEye, eye_bounds);
                nearClipped = isBoxNearClipped(eye_bounds);
                if (nearClipped == 2)
                {
                    occluded = 1;
                }
                else if (!zoTest2(eye_bounds, nearClipped, 0, 0))
                {
                    occluded = 1;
                }

            }
#endif

            if (!occluded)
            {
#if ZO_DEBUG_STATS_FRAMES
                LARGE_INTEGER zoDebugDrawStartTicks;
                LARGE_INTEGER zoDebugDrawEndTicks;
                LARGE_INTEGER diff;

                QueryPerformanceCounter(&zoDebugDrawStartTicks);
#endif

                zoDrawModel(zo_occluders[i]->m, matModelToEye);

#if ZO_DEBUG_STATS_FRAMES
                QueryPerformanceCounter(&zoDebugDrawEndTicks);
                diff.QuadPart = zoDebugDrawEndTicks.QuadPart - zoDebugDrawStartTicks.QuadPart;
                zoDebugDrawTotalTicks.QuadPart += diff.QuadPart;
#endif

                zo_last_occluders[zo_last_num_occluders++] = zo_occluders[i];
            }
            else
            {
                freeOccluder(zo_occluders[i]);
            }
        }
        else
        {
            freeOccluder(zo_occluders[i]);
        }
    }
    zo_num_occluders = 0;

    zo_min_occluder_screenspace = 0;
    zo_num_tests = 0;
    zo_num_test_calls = 0;

    createZHierarchy();

    CHECKHEAP;

    PERFINFO_AUTO_STOP();

#if ZO_DEBUG_STATS_FRAMES
    {
        LARGE_INTEGER zoDebugEndTicks;
        LARGE_INTEGER diff;

        QueryPerformanceCounter(&zoDebugEndTicks);
        diff.QuadPart = zoDebugEndTicks.QuadPart - zoDebugStartTicks.QuadPart;
        zoDebugTotalTicks.QuadPart += diff.QuadPart;
        if (zoDebugLargestTicks.QuadPart < diff.QuadPart)
            zoDebugLargestTicks.QuadPart = diff.QuadPart;
    }

    if (zoDebugCurrentFrame != global_state.global_frame_count)
    {
        zoDebugCurrentFrame = global_state.global_frame_count;
        zoDebugNumFrames++;

        if (zoDebugNumFrames >= ZO_DEBUG_STATS_FRAMES)
        {
            float totalMsPerFrame = (float)zoDebugTotalTicks.QuadPart / (float)zoDebugNumFrames / (float)zoDebugFrequency.QuadPart * 1000.0f;
            float largetsMs = (float)zoDebugLargestTicks.QuadPart / (float)zoDebugFrequency.QuadPart * 1000.0f;
            float drawMsPerFrame = (float)zoDebugDrawTotalTicks.QuadPart / (float)zoDebugNumFrames / (float)zoDebugFrequency.QuadPart * 1000.0f;

            printf("========== ZO TIMING over %d frames==================\n", zoDebugNumFrames);
            printf("%f mS per frame, worst %f mS\n", totalMsPerFrame, largetsMs);
            printf("%f mS per frame drawing\n", drawMsPerFrame );;

            zoDebugNumFrames = 0;
            zoDebugLargestTicks.QuadPart = 0;
            zoDebugTotalTicks.QuadPart = 0;
            zoDebugDrawTotalTicks.QuadPart = 0;
        }
    }
#endif
}

static int wasOccluder(int uid, Model *m)
{
    int i;

    for (i = 0; i < zo_last_num_occluders; i++)
    {
        if (zo_last_occluders[i]->uid == uid)
            return 1;

        if (uid < 0 && zo_last_occluders[i]->m == m)
            return 1;
    }

    return 0;
}

int zoCheckAddOccluder(Model *m, Mat4 matModelToEye, int uid)
{
    //int i, j, ret = 0;
    int i, ret = 0;

    if (m)
    {
        // fun hack!  The screen space is stored from the call to zoTest.
        float screenSpace = zo_last_screen_space;

        ret = -1;

        // HYPNOS_AKI: Filter out small objects (like beer cans)
        //if (m->radius < 3.0f)
        //    return ret;

        // Same check as in zoDrawModel()
        if (!m->tex_idx)
            return ret;

        if (screenSpace > MIN_OCCLUDER_SCREEN_SPACE && (screenSpace > zo_min_occluder_screenspace || zo_num_occluders < MAX_OCCLUDERS))
        {
            Mat4 matModelToWorld;
            mulMat4Inline(zo_inv_eye_mat, matModelToEye, matModelToWorld);

            if (zo_num_occluders == MAX_OCCLUDERS)
            {
                // remove the smallest
                freeOccluder(zo_occluders[--zo_num_occluders]);
            }

            // add in the appropriate place
            // HYPNOS_AKI: zo_occluders is sorted so we should do a binary search instead of a linear search here
#if 1
            {
                int high = zo_num_occluders - 1;
                int low = 0;
                while(low <= high)
                {
                    i = (low + high) / 2;
                    if (zo_occluders[i]->screenSpace > screenSpace)
                        low = i + 1;
                    else
                        high = i - 1;
                }

                i = low;
            }

            // Verify
            /*
            if (zo_num_occluders > 0 && i < zo_num_occluders)
                assert(screenSpace >= zo_occluders[i]->screenSpace);

            if (i > 0)
                assert(zo_occluders[i-1]->screenSpace >= screenSpace);
            */

#else
            for (i = 0; i < zo_num_occluders; i++)
            {
                if (screenSpace > zo_occluders[i]->screenSpace)
                {
                    break;
                }
            }
#endif
            // Bump occluders after this one down one in the list
#if 1
            if (i < zo_num_occluders)
                memmove(zo_occluders + i + 1, zo_occluders + i, sizeof(zo_occluders[0]) * (zo_num_occluders - i));
#else
            for (j = zo_num_occluders; j > i; j--)
                zo_occluders[j] = zo_occluders[j-1];
#endif

            zo_occluders[i] = createOccluder(m, matModelToWorld, screenSpace, uid);
            zo_min_occluder_screenspace = zo_occluders[zo_num_occluders]->screenSpace;
            zo_num_occluders++;

            if (game_state.tintOccluders && wasOccluder(uid, m))
                ret = 1;
        }

    }

    return ret;
}

int zoTestSphere(Vec3 centerEyeCoords, float radius, int isZClipped)
{
    float fx1, fx2, fy1, fy2;
    ZType z1;

    zo_last_screen_space = -1;

    zo_num_test_calls++;
    if (isZClipped)
        return 1;

    // if zoTestSphere is called, the sphere is never completely off screen
    getSphereScreenExtents(centerEyeCoords, radius, &fx1, &fy1, &fx2, &fy2, &z1);

    // test
    zo_num_tests++;
    return testZRectHierarchical(fx1, fy1, fx2, fy2, z1 - TEST_Z_OFFSET);
}


// SSE optimized version of mulBounds
#define ZOMULBOUNDS_USE_SSE 1
static INLINEDBG void zomulBounds(const Vec3 min, const Vec3 max, const Mat4 mat, Vec3 transformed[8])
{
#if ZOMULBOUNDS_USE_SSE
    register sseVec4 vecMinX, vecMinY, vecMinZ, vecMaxX, vecMaxY, vecMaxZ;
    register sseVec4 vecMat0, vecMat1, vecMat2, vecMat3;
    register sseVec4 temp0, temp1, temp2, temp3, temp;

    sse_init4_all(vecMinX, min[0]);
    sse_init4_all(vecMinY, min[1]);
    sse_init4_all(vecMinZ, min[2]);
    sse_init4_all(vecMaxX, max[0]);
    sse_init4_all(vecMaxY, max[1]);
    sse_init4_all(vecMaxZ, max[2]);

    sse_load4_unaligned(vecMat0, mat[0]);
    sse_load4_unaligned(vecMat1, mat[1]);
    sse_load4_unaligned(vecMat2, mat[2]);
    sse_init4(vecMat3, mat[3][0], mat[3][1], mat[3][2], 0.0f);

    sse_mul4(vecMinX, vecMat0, vecMinX);
    sse_mul4(vecMinY, vecMat1, vecMinY);
    sse_mul4(vecMinZ, vecMat2, vecMinZ);
    sse_mul4(vecMaxX, vecMat0, vecMaxX);
    sse_mul4(vecMaxY, vecMat1, vecMaxY);
    sse_mul4(vecMaxZ, vecMat2, vecMaxZ);

    sse_add4(vecMinX, vecMat3, vecMinX);
    sse_add4(vecMaxX, vecMat3, vecMaxX);

    sse_add4(vecMinX, vecMinY, temp0);
    sse_add4(temp0, vecMinZ, temp);
    sse_store4_unaligned(temp, transformed[0]);

    sse_add4(vecMaxX, vecMinY, temp1);
    sse_add4(temp1, vecMinZ, temp);
    sse_store4_unaligned(temp, transformed[1]);

    sse_add4(vecMinX, vecMaxY, temp2);
    sse_add4(temp2, vecMinZ, temp);
    sse_store4_unaligned(temp, transformed[2]);

    sse_add4(vecMaxX, vecMaxY, temp3);
    sse_add4(temp3, vecMinZ, temp);
    sse_store4_unaligned(temp, transformed[3]);

    sse_add4(temp0, vecMaxZ, temp);
    sse_store4_unaligned(temp, transformed[4]);

    sse_add4(temp1, vecMaxZ, temp);
    sse_store4_unaligned(temp, transformed[5]);

    sse_add4(temp2, vecMaxZ, temp);
    sse_store4_unaligned(temp, transformed[6]);

    sse_add4(temp3, vecMaxZ, temp);
    sse_store4_unaligned(temp, transformed[7]);
#else
    int        x, xbit, y, ybit, z, zbit;
    Vec3    extents[2];
    Vec3    tempX, tempXY;
    copyVec3(min, extents[0]);
    copyVec3(max, extents[1]);
    for(x=0,xbit=0;x<2;x++,xbit++)
    {
        tempX[0] = extents[x][0]*mat[0][0] + mat[3][0];
        tempX[1] = extents[x][0]*mat[0][1] + mat[3][1];
        tempX[2] = extents[x][0]*mat[0][2] + mat[3][2];
        for(y=0,ybit=0;y<2;y++,ybit+=2)
        {
            tempXY[0] = tempX[0] + extents[y][1]*mat[1][0];
            tempXY[1] = tempX[1] + extents[y][1]*mat[1][1];
            tempXY[2] = tempX[2] + extents[y][1]*mat[1][2];
            for(z=0,zbit=0;z<2;z++,zbit+=4)
            {
                transformed[xbit|ybit|zbit][0] = tempXY[0] + extents[z][2]*mat[2][0];
                transformed[xbit|ybit|zbit][1] = tempXY[1] + extents[z][2]*mat[2][1];
                transformed[xbit|ybit|zbit][2] = tempXY[2] + extents[z][2]*mat[2][2];
            }
        }
    }
#endif
}

int zoTest(Vec3 minBounds, Vec3 maxBounds, Mat4 matLocalToEye, int isZClipped, int uid, int num_children)
{
#if ZOMULBOUNDS_USE_SSE
    Vec3 bounds[9];        // The SSE load/store in zomulBounds will read one extra float
#else
    Vec3 bounds[8];
#endif
    zomulBounds(minBounds, maxBounds, matLocalToEye, bounds);
    return zoTest2(bounds, isZClipped, uid, num_children);
}

int zoTest2(Vec3 eyespaceBounds[8], int isZClipped, int uid, int num_children)
{
    int ret = 0;
    float fx1, fx2, fy1, fy2;
    ZType z1;

    zo_last_screen_space = -1;

    // transform and clip
    zo_num_test_calls++;
    if (isZClipped)
        ret = getScreenExtents(eyespaceBounds, &fx1, &fy1, &fx2, &fy2, &z1);
    else
        ret = getScreenExtentsNoZClip(eyespaceBounds, &fx1, &fy1, &fx2, &fy2, &z1);

    if (ret)
    {
        // fun hack!  Store the screen space so we don't have to transform the bounds again when zoCheckAddOccluder is called.
        zo_last_screen_space = (fx2-fx1) * (fy2-fy1) * (1-z1);

        // test
        if (ret == 1)
        {
            zo_num_tests++;
            ret = testZRectHierarchical(fx1, fy1, fx2, fy2, z1 - TEST_Z_OFFSET);
        }
    }

//    if (uid < 0)
//        return 1;

#if ZO_USE_JITTER
    if (uid >= 0 && !ret)
    {
        // not visible (occluded)
        if (num_children)
        {
            // set the occluded bit for all its children
            baSetBitRange(zo_occluded_bits[zo_frame], uid, 1 + num_children);
        }
        else
        {
            baSetBit(zo_occluded_bits[zo_frame], uid);
        }

        if (zo_frame == 0)
        {
            if (!baIsSetInline(zo_occluded_bits[1], uid) || !baIsSetInline(zo_occluded_bits[2], uid))
            {
                // occluded this frame but not last 2 frames, override visibility
                ret = 1;
            }
        }
        else if (zo_frame == 1)
        {
            if (!baIsSetInline(zo_occluded_bits[0], uid) || !baIsSetInline(zo_occluded_bits[2], uid))
            {
                // occluded this frame but not last 2 frames, override visibility
                ret = 1;
            }
        }
        else
        {
            if (!baIsSetInline(zo_occluded_bits[0], uid) || !baIsSetInline(zo_occluded_bits[1], uid))
            {
                // occluded this frame but not last 2 frames, override visibility
                ret = 1;
            }
        }
    }
#endif

    return ret;
}

void zoSetOccluded(int uid, int num_children)
{
#if ZO_USE_JITTER
    if (num_children)
    {
        // set the occluded bit for all its children
        baSetBitRange(zo_occluded_bits[zo_frame], uid, 1 + num_children);
    }
    else
    {
        baSetBit(zo_occluded_bits[zo_frame], uid);
    }
#endif
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////

int zoIsTexOk(TexBind *tex_bind, BlendModeType blend)
{
    if (tex_bind == invisible_tex_bind)
        return 0;
    if (texNeedsAlphaSort(tex_bind, blend))
        return 0;

    return 1;
}

static void zoDraw(Mat4 matToEye, Vec3 *verts, int vert_count, int *idxs, int tex_count, TexID *tex_idx, TexBind **tex_binds, BlendModeType *blends)
{
    ZBufferPoint *transformCache = 0;
    ZBufferPoint *p0, *p1, *p2;
    int idx, tex;
    Mat44 mat44ToEye;
    Mat44 matToClip;

    PERFINFO_AUTO_START("zoDraw", 1);

    mat43to44(matToEye, mat44ToEye);
    mulMat44Inline(zo_projection_mat, mat44ToEye, matToClip);

    transformCache = _alloca(sizeof(*transformCache) * vert_count);
    ZeroStructs(transformCache, vert_count);

    for (tex = 0; tex < tex_count; tex++)
    {
        int idx_count = tex_idx[tex].count * 3;

        if (zoIsTexOk(tex_binds[tex], blends[tex]))
        {
            for (; idx_count > 0; idx_count-=3)
            {
                idx = *idxs;
#if ZO_SAFE
                assert(INRANGE0(idx, vert_count));
#endif
                ++idxs;
                p0 = &transformCache[idx];
                if (!transformCache[idx].cached)
                {
                    transformCache[idx].cached = 1;
                    transformPoint(p0, verts[idx], matToClip);
                }

                idx = *idxs;
#if ZO_SAFE
                assert(INRANGE0(idx, vert_count));
#endif
                ++idxs;
                p1 = &transformCache[idx];
                if (!transformCache[idx].cached)
                {
                    transformCache[idx].cached = 1;
                    transformPoint(p1, verts[idx], matToClip);
                }

                idx = *idxs;
#if ZO_SAFE
                assert(INRANGE0(idx, vert_count));
#endif
                ++idxs;
                p2 = &transformCache[idx];
                if (!transformCache[idx].cached)
                {
                    transformCache[idx].cached = 1;
                    transformPoint(p2, verts[idx], matToClip);
                }

                drawZTriangleClip(p0, p1, p2, 0);
            }
        }
        else
        {
            idxs += idx_count;
        }
    }

    PERFINFO_AUTO_STOP();
}

static void zoDrawModel(Model *m, Mat4 matModelToEye)
{
    if (!m->tex_idx)
        return;
    modelSetupZOTris(m);
    zoDraw(matModelToEye, m->unpack.verts, m->vert_count, m->unpack.tris, m->tex_count, m->tex_idx, m->tex_binds, m->blend_modes);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////


