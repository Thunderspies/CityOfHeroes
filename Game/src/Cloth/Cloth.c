#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <utilitieslib/utils/timing.h>
#include "cmdparse/cmdgame.h"
#include "render/renderUtil.h"

#include "Cloth/Cloth.h"
#include "Cloth/ClothPrivate.h"
#include "Cloth/ClothCollide.h"
#include "Cloth/Wind.h"

//============================================================================
// Stuff for debugging ClothCheckMaxMotion()
//============================================================================

#define CLOTH_DEBUG 0

#if CLOTH_DEBUG
static F32 ClothDebugMaxDist = 15.0f;
static int ClothDebugError = 0;
#define CLOTH_DEBUG_DPOS 1
# if CLOTH_DEBUG_DPOS
static F32 ClothDebugMaxDpos = 3.0f;
# endif
#endif

//============================================================================
// ERROR HANDLING
//============================================================================

S32  ClothError = 0; // 0 = no error
char ClothErrorMessage[1024] = "";

int ClothErrorf(char const *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vsprintf(ClothErrorMessage, fmt, ap);
    va_end(ap);

    Errorf(ClothErrorMessage);
    
    return -1;
}

//============================================================================
// TUNING PARAMATERS
//============================================================================

// This determines what order the particles are constrained in.
// The particles constrained first have the most 'stretch'
// TOP_FIRST == 1 seems to produce the least collision poke-through
#define CONSTRAIN_TOP_FIRST 1
// ColAmt is used to give weight to particles that collided,
// reducint poke-through artifacts.
#define USE_COLAMT   1

// These limit the motion of the cloth.
// Each #def matches a variable which determines the limit in units/second.
#define USE_MAXPDPOS 1
#define USE_MAXDPOS  1
#define USE_MAXDANG  1
#define USE_MAXACCEL 1

#define cTPF 30.f // per frame -> per second
F32 MAXPDPOS = 2.5f*cTPF;    // Max Particle dpos / sec
F32 MAXDPOS =  0.5f*cTPF;    // Max Body dpos / sec
F32 MAXDANG = (RAD(6.0f))*cTPF;
F32 MAXACCEL = 2.5f*cTPF;    // Max Delta Velocity / sec

// These limits are per-frame, regardless of frame-rate.
// If the motion exceeds these limits, the cloth is not animated, just
// transformed to the new location.
#define MAX_FRAME_DANG_COS 0.707f
//#define MAX_FRAME_DPOS 15.f
#define MAX_FRAME_DPOS 30.f // roughly terminal velocity at 10fps falling off tallest builing in AP

// Misc Constants
#define MAX_DRAG 0.5f    // Maximum drag amount

//============================================================================
// QUATERNION SUPPORT
// These routines are not super-optimized, but are only used per-cloth,
// not per-particle.
// Defined as statics here so that they can be inlined by the compiler
//============================================================================

#define dotVec4(v1,v2)    ((v1)[0]*(v2)[0] + (v1)[1]*(v2)[1] + (v1)[2]*(v2)[2] + (v1)[3]*(v2)[3])

#ifdef EPSILON
#undef EPSILON
#endif
F32 EPSILON = 0.0000001f;
#define NEARZERO(x) (fabs(x) < EPSILON)
#define QUAT_TRACE_QZERO_TOLERANCE        0.1

static void NormalizeQuat(Vec4 q)
{
    F32 Nq,s;
    Nq = q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3];
    if (Nq > EPSILON)
    {
        F32 s = 1.0f / sqrtf(Nq);
        scaleVec4(q, s, q);
    }
    else
    {
        zeroVec3(q);
        q[3] = 1.0f;
    }
    // Keep quaternions "right-side-up"
    if (q[1] < 0.0f)
        scaleVec4(q, -1.0f, q);
}

// q is assumed to be normalized
static void CreateQuatMat(Mat3 mat, Vec4 q)
{
    F32 s;
    F32 xs,    ys,    zs;

    //F32 Nq;
    //Nq = q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3];
    //s = (Nq > 0.0f) ? (2.0f / Nq) : 0.0f;
    s = 2.0f;
    
    xs = q[0]*s;
    ys = q[1]*s;
    zs = q[2]*s;
    
    {
        F32 xx = q[0]*xs;
        F32 yy = q[1]*ys;
        F32 zz = q[2]*zs;
        mat[0][0]= 1.0 - (yy + zz);
        mat[1][1]= 1.0 - (xx + zz);
        mat[2][2]= 1.0 - (xx + yy);
    }
    {
        F32 xy = q[0]*ys;
        F32 wz = q[3]*zs;
        mat[1][0]= (xy + wz);
        mat[0][1]= (xy - wz);
    }
    {
        F32 yz = q[1]*zs;
        F32 wx = q[3]*xs;
        mat[2][1]= (yz + wx);
        mat[1][2]= (yz - wx);
    }
    {
        F32 xz = q[0]*zs;
        F32 wy = q[3]*ys;
        mat[2][0]= xz - wy;
        mat[0][2]= xz + wy;
    }
}

// I tried several different quaternion extraction functions.
// This one is much more reliable than others I tried, which
// is critical since quaternions between potentially arbitrary
// matrices is being performed.
static void ExtractQuat(Mat3 mat, Vec4 q)
{
    F32 tr,s,inv_s;
    F32 X,Y,Z,W;

    tr = mat[0][0] + mat[1][1] + mat[2][2];
    if (tr > 0.0f)
    {
        s = sqrtf(tr + 1.0f);
        W = s * 0.5f;
        inv_s = 0.5f / s;

        X = (mat[2][1] - mat[1][2]) * inv_s;
        Y = (mat[0][2] - mat[2][0]) * inv_s;
        Z = (mat[1][0] - mat[0][1]) * inv_s;
    }
    else
    {
        S32 i, biggest;
        if (mat[2][2] > mat[0][0] && mat[2][2] > mat[1][1])
            biggest = 2;
        else if (mat[0][0] > mat[1][1])
            biggest = 0;
        else
            biggest = 1;

        inv_s = 0.0f; // invalid state
        for (i=0; i<3; i++)
        {
            if (biggest == 0)
            {
                s = sqrtf(mat[0][0] - (mat[1][1] + mat[2][2]) + 1.0f);
                if (s > QUAT_TRACE_QZERO_TOLERANCE)
                {
                    X = s * 0.5f;
                    inv_s = 0.5f / s;
                    W = (mat[2][1] - mat[1][2]) * inv_s;
                    Y = (mat[0][1] + mat[1][0]) * inv_s;
                    Z = (mat[0][2] + mat[2][0]) * inv_s;
                    break;
                }
            }
            else if (biggest == 1)
            {
                s = sqrtf(mat[1][1] - (mat[2][2] + mat[0][0]) + 1.0f);
                if (s > QUAT_TRACE_QZERO_TOLERANCE)
                {
                    Y = s * 0.5f;
                    inv_s = 0.5f / s;
                    W = (mat[0][2] - mat[2][0]) * inv_s;
                    Z = (mat[1][2] + mat[2][1]) * inv_s;
                    X = (mat[1][0] + mat[0][1]) * inv_s;
                    break;
                }
            }
            else
            {
                s = sqrtf(mat[2][2] - (mat[0][0] + mat[1][1]) + 1.0f);
                if (s > QUAT_TRACE_QZERO_TOLERANCE)
                {
                    Z = s * 0.5f;
                    inv_s = 0.5f / s;
                    W = (mat[1][0] - mat[0][1]) * inv_s;
                    X = (mat[2][0] + mat[0][2]) * inv_s;
                    Y = (mat[2][1] + mat[1][2]) * inv_s;
                    break;
                }
            }
            if (++biggest > 2)
                biggest = 0;
        }
#if CLOTH_DEBUG
        assert(inv_s != 0.0f);
#else
        if (inv_s == 0.0f)
            X = Y = Z = 0.0f, W = 1.0f;
#endif
    }
    q[0] = X; q[1] = Y; q[2] = Z; q[3] = W;
    NormalizeQuat(q); // Matrix might be scaled
}



//============================================================================
// CREATION FUNCTIONS
//============================================================================

static void ClothInitialize(Cloth *cloth);
static void ClothFreeData(Cloth *cloth);

// Create an empty Cloth structure (i.e. does not allocate particles)
Cloth *ClothCreateEmpty(int minsublod)
{
    Cloth *cloth = CLOTH_CALLOC(Cloth, 1);
    if (cloth)
    {
        ClothInitialize(cloth);
        minsublod = MINMAX(minsublod,0,CLOTH_SUB_LOD_NUM-1);
        cloth->commonData.SubLOD = minsublod;
        cloth->MinSubLOD = minsublod;
    }
    else
    {
        ClothErrorf("Unable to create cloth");
    }
    return cloth;
}

// Handles all memory de-allocation
void ClothDelete(Cloth * cloth)
{
    if (cloth)
        ClothFreeData(cloth);
    CLOTH_FREE(cloth);
}

//////////////////////////////////////////////////////////////////////////////

// See Cloth.h for descriptions of Cloth members
static void ClothInitialize(Cloth *cloth) 
{
    int i;

    memset(cloth, 0, sizeof(*cloth));

    ClothSetNumIterations(cloth, -1, -1); // default
    

    setVec3(cloth->MinPos, 999999999.f, 999999999.f, 999999999.f);
    setVec3(cloth->MaxPos, -999999999.f, -999999999.f, -999999999.f);

    copyMat4(unitmat, cloth->Matrix);
    zeroVec4(cloth->mQuat);
    zeroVec3(cloth->Vel);
    cloth->Gravity = -32.0f;
    cloth->Drag = 0.10f;
    cloth->PointColRad = 1.0f;
    cloth->Time = 0.0f;

    zeroVec3(cloth->WindDir);
    cloth->WindMag = 0.0f;
    cloth->WindScaleFactor = 64.0f;
    cloth->WindRippleAmt = 0.0f;
    cloth->WindRippleRepeat = 1.5f;
    cloth->WindRipplePeriod = 0.5f;
};

static void ClothFreeData(Cloth *cloth)
{
    int i;
    CLOTH_FREE(cloth->OrigPos);
    CLOTH_FREE(cloth->PosBuffer1);
    CLOTH_FREE(cloth->PosBuffer2);
    CLOTH_FREE(cloth->InvMasses);
    CLOTH_FREE(cloth->renderData.RenderPos);
    CLOTH_FREE(cloth->renderData.Normals);
#if CLOTH_CALC_BINORMALS
    CLOTH_FREE(cloth->renderData.BiNormals);
#endif
#if CLOTH_CALC_TANGENTS
    CLOTH_FREE(cloth->renderData.Tangents);
#endif
    CLOTH_FREE(cloth->renderData.NormalScale);
    CLOTH_FREE(cloth->renderData.hookNormals);
    CLOTH_FREE(cloth->commonData.ApproxNormalLens);
    CLOTH_FREE(cloth->renderData.TexCoords);
    CLOTH_FREE(cloth->ColAmt);
    CLOTH_FREE(cloth->LengthConstraintsInit);
    cloth->commonData.LengthConstraints = NULL;
    CLOTH_FREE(cloth->commonData.Attachments);
    for (i=0; i<cloth->NumCollidables; i++)
        ClothColDelete(&cloth->Collidables[i]);
    CLOTH_FREE(cloth->Collidables);
}

//////////////////////////////////////////////////////////////////////////////

// Allocate the various buffers
int ClothCreateParticles(Cloth *cloth, int width, int height)
{
    int npoints,rpoints;
    npoints = width * height;
    cloth->commonData.NumParticles = npoints;
    cloth->commonData.Width = width;
    cloth->commonData.Height = height;

    if (!width || !height)
        return 0;
    
    if (!(cloth->OrigPos = CLOTH_CALLOC(Vec3, npoints)))
        return 0;
    if (!(cloth->InvMasses = CLOTH_CALLOC(F32, npoints)))
        return 0;
    if (!(cloth->commonData.ApproxNormalLens = CLOTH_CALLOC(F32, npoints)))
        return 0;
    if (!(cloth->ColAmt = CLOTH_CALLOC(F32, npoints)))
        return 0;
    
    if (!(cloth->PosBuffer1 = CLOTH_CALLOC(Vec3, npoints)))
        return 0;
    if (!(cloth->PosBuffer2 = CLOTH_CALLOC(Vec3, npoints)))
        return 0;

    rpoints = ClothNumRenderedParticles(&cloth->commonData, cloth->MinSubLOD);
    if (!(cloth->renderData.RenderPos = CLOTH_CALLOC(Vec3, rpoints)))
        return 0;

    cloth->CurPos = cloth->PosBuffer1;
    cloth->OldPos = cloth->PosBuffer2;

    if (!(cloth->renderData.Normals = CLOTH_CALLOC(Vec3, rpoints)))
        return 0;
#if CLOTH_CALC_BINORMALS
    if (!(cloth->renderData.BiNormals = CLOTH_CALLOC(Vec3, rpoints)))
        return 0;
#endif
#if CLOTH_CALC_BINORMALS
    if (!(cloth->renderData.Tangents = CLOTH_CALLOC(Vec3, rpoints)))
        return 0;
#endif
    if (!(cloth->renderData.NormalScale = CLOTH_CALLOC(F32, rpoints)))
        return 0;
    if (!(cloth->renderData.TexCoords = CLOTH_CALLOC(Vec2, rpoints)))
        return 0;

    return rpoints;
}

//////////////////////////////////////////////////////////////////////////////
// Interfaces to Cloth creation + Cloth Particle allocation

Cloth *ClothCreate(int minsublod, int width, int height)
{
    Cloth *cloth = ClothCreateEmpty(minsublod);
    if (!cloth)
        return 0;
    if (!ClothCreateParticles(cloth, width, height))
    {
        ClothErrorf("Unable to create cloth particles");
        ClothDelete(cloth);
        cloth = 0;
    }
    return cloth;
}

Cloth *ClothCreateFromPoints(int minsublod, int width, int height, Vec3 *points, F32 *masses)
{
    Cloth *cloth = ClothCreate(minsublod, width, height);
    if (cloth)
    {
        ClothSetPointsMasses(cloth, points, masses);
    }
    return cloth;
}

Cloth *ClothCreateFromGrid(int minsublod, int width, int height, Vec3 dxyz, Vec3 origin, F32 xscale, int arc)
{
    Cloth *cloth = ClothCreateEmpty(minsublod);
    if (cloth)
    {
        ClothBuildGrid(cloth, width, height, dxyz, origin, xscale, arc);
    }
    return cloth;
}

//////////////////////////////////////////////////////////////////////////////
// Set an individual Cloth Particle

void ClothSetParticle(Cloth *cloth, int pidx, Vec3 pos, F32 mass)
{
    if (pos)
    {
        copyVec3(pos, cloth->OrigPos[pidx]);
        copyVec3(pos, cloth->OldPos[pidx]);
        copyVec3(pos, cloth->CurPos[pidx]);
        MINVEC3(pos, cloth->MinPos, cloth->MinPos);
        MAXVEC3(pos, cloth->MaxPos, cloth->MaxPos);
    }
    if (mass >= 0.0f)
        cloth->InvMasses[pidx] = mass > 0.0f ? 1.0f / mass : 0.0f; // Use 0 for infinite mass (fixed)
}

//////////////////////////////////////////////////////////////////////////////
// Add an 'Eyelet' to the cloth
// An Attachment contains the Particle index of the Eylet, plus two hook
// indices and a fraction (weight) in order to support Cloths where the
// Eyelets don't line up correctly with the Hooks (e.g. LOD's).

int ClothAddAttachment(Cloth *cloth, int pidx, int hook1, int hook2, F32 frac)
{
    int attachidx;
    attachidx = cloth->commonData.NumAttachments;
    cloth->commonData.NumAttachments++;
    cloth->commonData.MaxAttachments = MAX(cloth->commonData.MaxAttachments, hook1+1);
    if (frac != 1.0f)
        cloth->commonData.MaxAttachments = MAX(cloth->commonData.MaxAttachments, hook2+1);
    cloth->commonData.Attachments = CLOTH_REALLOC(cloth->commonData.Attachments, ClothAttachment, cloth->commonData.NumAttachments);
    cloth->commonData.Attachments[attachidx].PIdx = pidx;
    cloth->commonData.Attachments[attachidx].Hook1 = hook1;
    cloth->commonData.Attachments[attachidx].Hook2 = hook2;
    cloth->commonData.Attachments[attachidx].Frac = frac;
    cloth->InvMasses[pidx] = 0.0f;
    return attachidx;
}

//============================================================================
// CALCULATE CLOTH INFORMATION
//============================================================================

//////////////////////////////////////////////////////////////////////////////
// Calculate number of rendered particles for a SubLod

S32 ClothNumRenderedParticles(ClothCommonData *commonData, int sublod)
{
    int npoints = commonData->NumParticles;
    if (sublod <= 0)
    {
        npoints += 4*(commonData->Width-1); // Split Horizontal Border Edges into 4
        npoints += 4*(commonData->Height-1); // Split Vertical Border Edges into 4
    }
    if (sublod <= 1)
    {
        npoints += (commonData->Width-1)*(commonData->Height-1); // Centers
    }
    if (sublod <= 1)
    {
        npoints += (commonData->Width-1)*(commonData->Height); // Split Horizontal Edges
        npoints += (commonData->Width)*(commonData->Height-1); // Split Vertical Edges
    }
    return npoints;
}

//////////////////////////////////////////////////////////////////////////////
// Calculates horizontal, vertical, and (optionally) diagonal length constraints
// If nconnections > 1, additional constraints are created for points
// further away, scaled by frac scale.
// i.e. if nconnections = 2, and fracscale = .5, given A-B-C,
// the B-A constraint is created normally, and a second constraint,
// C-A is created of length |C-A|*.5
// NOTE: Diagonal constraints have so far not proven very useful as they
//  produce cloth that is more likely to become bunched up (this could be a bug...)
// NOTE: The code supports nfracscales > 1, but it is not currently used
//  for performance reasons. The addional constraints produce somewhat stiffer
//  cloth, but the results are not significantly better.

void ClothCalcLengthConstraints(Cloth *cloth, S32 flags, int nfracscales, F32 *in_fracscales)
{
    int Width = cloth->commonData.Width;
    int Height = cloth->commonData.Height;
    
    int nconstraints = 0;
    int nconnections = flags & CLOTH_FLAGS_CONNECTIONS_MASK;
    int i,j,k;
    int sublod = CLOTH_SUB_LOD_NUM-1;

    if (!cloth->commonData.NumParticles)
        return;
    if (nconnections < 1)
        return;

    cloth->NumConnections = nconnections;

    for (k=0; k<nconnections; k++)
    {
        nconstraints += (Width-(k+1))*Height; // horizontal
        nconstraints += (Height-(k+1))*Width; // vertical
        cloth->NumLengthConstraints[sublod] = nconstraints;
        if (sublod > 0)
            sublod--;
    }
    if (flags & CLOTH_FLAGS_DIAGONAL1)
        nconstraints += (Height-1)*(Width-1); // diagonal 1
    if (flags & CLOTH_FLAGS_DIAGONAL2)
        nconstraints += (Height-1)*(Width-1); // diagonal 2

    while (sublod >= 0) {
        cloth->NumLengthConstraints[sublod] = nconstraints;
        sublod--;
    }
    
    cloth->commonData.LengthConstraints = cloth->LengthConstraintsInit = CLOTH_CALLOC(ClothLengthConstraint, nconstraints);

    {
        int nc = 0;

        F32 fracscale = .5f;
        F32 frac = 1.0f;
        for (k=0; k<nconnections; k++)
        {
            int fracscaleidx = MIN(k, nfracscales-1);
            if (in_fracscales && fracscaleidx >= 0)
                fracscale = in_fracscales[fracscaleidx];
            if (k > 0)
                frac = -fracscale;

            // horizontal constraints
            for (i=0; i<Height; i++)
            {
                for (j=0; j<Width-(k+1); j++)
                {
                    S32 p1 = i*Width + j;
                    S32 p2 = p1 + (k+1);
                    ClothLengthConstraintInitialize(&cloth->LengthConstraintsInit[nc++], cloth, p1, p2, frac);
                }
            }
            // vertical constraints
            for (i=0; i<Height-(k+1); i++)
            {
                for (j=0; j<Width; j++)
                {
                    S32 p1 = i*Width + j;
                    S32 p2 = p1 + Width*(k+1);
                    ClothLengthConstraintInitialize(&cloth->LengthConstraintsInit[nc++], cloth, p1, p2, frac);
                }
            }
        }
        // diagonal constraints
        frac = 1.0f;
        if (flags & CLOTH_FLAGS_DIAGONAL1)
        {
            for (i=0; i<Height-1; i++)
            {
                for (j=0; j<Width-1; j++)
                {
                    S32 p1 = i*Width + j;
                    S32 p2 = p1 + Width + 1;
                    ClothLengthConstraintInitialize(&cloth->LengthConstraintsInit[nc++], cloth, p1, p2, frac);
                }
            }
        }
        if (flags & CLOTH_FLAGS_DIAGONAL2)
        {
            for (i=0; i<Height-1; i++)
            {
                for (j=1; j<Width; j++)
                {
                    S32 p1 = i*Width + j;
                    S32 p2 = p1 + Width - 1;
                    ClothLengthConstraintInitialize(&cloth->LengthConstraintsInit[nc++], cloth, p1, p2, frac);
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// Called once after creation or modification.
// Sets up Min/Max, ApproxNormalLengths, and TexCoords.

void ClothCalcConstants(Cloth *cloth, int calcTexCoords)
{
    int Width = cloth->commonData.Width;
    int Height = cloth->commonData.Height;
    Vec3 *CurPos = cloth->CurPos;
    Vec2 *TexCoords = cloth->renderData.TexCoords;
    
    int i,j,p;
    int nump1 = Width*Height; // centers
    int nump2 = nump1 + (Width-1)*(Height-1); // horiz edge midpoints
    int nump3 = nump2 + (Width-1)*(Height); // vert edge midpoints
    int nump4 = nump3 + (Width)*(Height-1); // horiz edge border points
    int nump5 = nump4 + 4*(Width-1); // vert edge border points
    //int nump6 = nump2 + 4*(Height-1); // end
    int numc1 = Height*(Width-1);

    int nump = cloth->commonData.NumParticles;

    // MinMax
    copyVec3(CurPos[0], cloth->MinPos);
    copyVec3(CurPos[0], cloth->MaxPos);

    for (p=1; p<nump; p++)
    {
        MINVEC3(CurPos[p], cloth->MinPos, cloth->MinPos);
        MAXVEC3(CurPos[p], cloth->MaxPos, cloth->MaxPos);
    }
    subVec3(cloth->MaxPos, cloth->MinPos, cloth->DeltaPos);
    scaleAddVec3(cloth->DeltaPos, .5f, cloth->MinPos, cloth->DeltaPos);

    // ApproxNormalLengths
    for (i=0; i<Height; i++)
    {
        for (j=0; j<Width; j++)
        {
            // Calculate Normal
            int pidx = i*Width + j;
            int px1 = j > 0 ? pidx-1 : pidx;
            int px2 = j < Width-1 ? pidx+1 : pidx;
            int py1 = i > 0 ? pidx-Width : pidx;
            int py2 = i < Height-1 ? pidx+Width : pidx;
            Vec3 vx,vy,norm;
            subVec3(CurPos[px2], CurPos[px1], vx);
            subVec3(CurPos[py2], CurPos[py1], vy);
            crossVec3(vx, vy, norm);
            cloth->commonData.ApproxNormalLens[pidx] = lengthVec3(norm);
        }
    }
    if (calcTexCoords) {
        // Tex Coords
        // Grid
        for (i=0; i<Height; i++)
        {
            for (j=0; j<Width; j++)
            {
                int p = i*Width + j;
                TexCoords[p][0] = (F32)j/(F32)(Width-1);
                TexCoords[p][1] = (F32)i/(F32)(Height-1);
            }
        }
    }
    // Centers
    if (cloth->MinSubLOD <= 1)
    {
        for (i=0; i<Height-1; i++)
        {
            for (j=0; j<Width-1; j++)
            {
                int cidx = nump1 + i*(Width-1) + j;
                // interpolate texcoords from i,j, i+1,j i,j+1, i+1,j+1
                int p = i*Width + j;
                TexCoords[cidx][0] = 0.25*(TexCoords[p][0] + TexCoords[p+1][0] + TexCoords[p + Width][0] + TexCoords[p+1 + Width][0]);
                TexCoords[cidx][1] = 0.25*(TexCoords[p][1] + TexCoords[p+1][1] + TexCoords[p + Width][1] + TexCoords[p+1 + Width][1]);
            }
        }
    }
    // Midpoints
    if (cloth->MinSubLOD <= 1)
    {
        // Horizontal midpoints
        for (i=0; i<Height; i++)
        {
            for (j=0; j<Width-1; j++)
            {
                int hidx = nump2 + i*(Width-1) + j;
                // interpolate texcoords from i,j i,j+1
                int p = i*Width + j;
                TexCoords[hidx][0] = 0.5*(TexCoords[p][0] + TexCoords[p+1][0]);
                TexCoords[hidx][1] = 0.5*(TexCoords[p][1] + TexCoords[p+1][1]);
            }
        }
        // Vertical midpoints
        for (i=0; i<Height-1; i++)
        {
            for (j=0; j<Width; j++)
            {
                int vidx = nump3 + i*Width + j;
                // interpolate texcoords from i,j, i+1,j
                int p = i*Width + j;
                TexCoords[vidx][0] = 0.5*(TexCoords[p][0] + TexCoords[p+Width][0]);
                TexCoords[vidx][1] = 0.5*(TexCoords[p][1] + TexCoords[p+Width][1]);
            }
        }
    }
    // Edge curves
    if (cloth->MinSubLOD <= 0)
    {
        // Horizontal midpoints
        for (j=0; j<Width-1; j++)
        {
            // Top edge
            {
                int hidx = nump4 + j*2;
                int p = j;
                // interpolate texcoords 75% i,j 25% i,j+1
                TexCoords[hidx][0] = 0.75*TexCoords[p][0] + 0.25*TexCoords[p+1][0];
                TexCoords[hidx][1] = 0.75*TexCoords[p][1] + 0.25*TexCoords[p+1][1];
                // interpolate texcoords 25% i,j 75% i,j+1
                TexCoords[hidx+1][0] = 0.25*TexCoords[p][0] + 0.75*TexCoords[p+1][0];
                TexCoords[hidx+1][1] = 0.25*TexCoords[p][1] + 0.75*TexCoords[p+1][1];
            }
            // Bottom edge
            {
                int hidx = nump4 + (Width-1)*2 + j*2;
                int p = (Height-1)*Width + j;
                // interpolate texcoords 75% i,j 25% i,j+1
                TexCoords[hidx][0] = 0.75*TexCoords[p][0] + 0.25*TexCoords[p+1][0];
                TexCoords[hidx][1] = 0.75*TexCoords[p][1] + 0.25*TexCoords[p+1][1];
                // interpolate texcoords 25% i,j 75% i,j+1
                TexCoords[hidx+1][0] = 0.25*TexCoords[p][0] + 0.75*TexCoords[p+1][0];
                TexCoords[hidx+1][1] = 0.25*TexCoords[p][1] + 0.75*TexCoords[p+1][1];
            }
        }
        for (i=0; i<Height-1; i++)
        {
            // Left edge
            {
                int vidx = nump5 + i*2;
                int p = i*Width + 0;
                // interpolate texcoords 75% i,j 25% i+1,j
                TexCoords[vidx][0] = 0.75*TexCoords[p][0] + 0.25*TexCoords[p+Width][0];
                TexCoords[vidx][1] = 0.75*TexCoords[p][1] + 0.25*TexCoords[p+Width][1];
                // interpolate texcoords 25% i,j 75% i+1,j
                TexCoords[vidx+1][0] = 0.25*TexCoords[p][0] + 0.75*TexCoords[p+Width][0];
                TexCoords[vidx+1][1] = 0.25*TexCoords[p][1] + 0.75*TexCoords[p+Width][1];
            }
            {
                // Right edge
                int vidx = nump5 + (Height-1)*2 + i*2;
                int p = i*Width + (Width-1);
                // interpolate texcoords 75% i,j 25% i+1,j
                TexCoords[vidx][0] = 0.75*TexCoords[p][0] + 0.25*TexCoords[p+Width][0];
                TexCoords[vidx][1] = 0.75*TexCoords[p][1] + 0.25*TexCoords[p+Width][1];
                // interpolate texcoords 25% i,j 75% i+1,j
                TexCoords[vidx+1][0] = 0.25*TexCoords[p][0] + 0.75*TexCoords[p+Width][0];
                TexCoords[vidx+1][1] = 0.25*TexCoords[p][1] + 0.75*TexCoords[p+Width][1];
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// ClothCopyCloth is used to transition from one LOD to another.
// Simply copies the positions from one cloth to another, 'tweening' as needed.

Cloth * ClothCopyCloth(Cloth *cloth, Cloth *pcloth)
{
    if (pcloth != cloth)
    {
        int Width = cloth->commonData.Width;
        int Height = cloth->commonData.Height;
    
        int pwidth  = pcloth->commonData.Width;
        int pheight = pcloth->commonData.Height;
        F32 dw = Width  > 1 ? ((F32)(pwidth  - 1) / (F32)(Width  - 1)) : 0;
        F32 dh = Height > 1 ? ((F32)(pheight - 1) / (F32)(Height - 1)) : 0;
        F32 w,h;
        int i,j;
        h = 0.0f;
        for (i=0; i<Height; i++)
        {
            int h1 = (int)h;
            int h2 = (h < pheight-1) ? h1+1 : h1;
            F32 hf1,hf2;
            hf2 = (h1 != h2) ? h - (F32)h1 : 0.0f;
            hf1 = 1.0f - hf2;
            
            w = 0.0f;
            for (j=0; j<Width; j++)
            {
                int w1 = (int)w;
                int w2 = (w < pwidth-1) ? w1+1 : w1;
                F32 wf2 = (w2 != w1) ? w - (F32)w1 : 0.0f;
                F32 wf1 = 1.0f - wf2;

                Vec3 curp,oldp;
                int pidx = i*Width + j;
                int p11 = h1*pwidth + w1;
                int p12 = h1*pwidth + w2;
                int p21 = h2*pwidth + w1;
                int p22 = h2*pwidth + w2;
                F32 sf11 = hf1*wf1;
                F32 sf12 = hf1*wf2;
                F32 sf21 = hf2*wf1;
                F32 sf22 = hf2*wf2;

                scaleVec3   (pcloth->CurPos[p11], sf11, curp);
                scaleAddVec3(pcloth->CurPos[p12], sf12, curp, curp);
                scaleAddVec3(pcloth->CurPos[p21], sf21, curp, curp);
                scaleAddVec3(pcloth->CurPos[p22], sf22, curp, cloth->CurPos[pidx]);

                scaleVec3   (pcloth->OldPos[p11], sf11, oldp);
                scaleAddVec3(pcloth->OldPos[p12], sf12, oldp, oldp);
                scaleAddVec3(pcloth->OldPos[p21], sf21, oldp, oldp);
                scaleAddVec3(pcloth->OldPos[p22], sf22, oldp, cloth->OldPos[pidx]);
            
                cloth->ColAmt[pidx] = 0;
                w += dw;
            }
            h += dh;
        }
        ClothSetMatrix(cloth, pcloth->Matrix);
        ClothSetGravity(cloth, pcloth->Gravity);
        ClothSetDrag(cloth, pcloth->Drag);
        ClothSetWind(cloth, pcloth->WindDir, pcloth->WindMag, pcloth->WindScaleFactor );
        ClothSetWindRipple(cloth, pcloth->WindRippleAmt, pcloth->WindRippleRepeat, pcloth->WindRipplePeriod );
        cloth->Time = pcloth->Time;
    }
    return cloth;
}

//////////////////////////////////////////////////////////////////////////////
// ClothBuildCopy is similar to ClothCopyCloth, but is used for building LODs.
// See ClothBuildLOD in ClothBuild.c for usage.
// NOTE: point_y_scale scales the X component based on the height, which 
//       could in fact vary in Y or in Z

static int find_hook(Cloth *cloth, int pidx)
{
    int i;
    for (i=0; i<cloth->commonData.NumAttachments; i++)
    {
        if (cloth->commonData.Attachments[i].PIdx == pidx)
        {
            if (cloth->commonData.Attachments[i].Frac >= .5f)
                return cloth->commonData.Attachments[i].Hook1;
            else
                return cloth->commonData.Attachments[i].Hook2;
        }
    }
    ClothErrorf("Unable To Find Hook for %d",pidx);
    return -1;
}

Cloth * ClothBuildCopy(Cloth *cloth, Cloth *pcloth, F32 point_y_scale)
{
    if (pcloth != cloth)
    {
        int Width = cloth->commonData.Width;
        int Height = cloth->commonData.Height;
    
        int pwidth  = pcloth->commonData.Width;
        int pheight = pcloth->commonData.Height;
        F32 dw = Width  > 1 ? ((F32)(pwidth  - 1) / (F32)(Width  - 1)) : 0;
        F32 dh = Height > 1 ? ((F32)(pheight - 1) / (F32)(Height - 1)) : 0;
        F32 w,h;
        int i,j;
        h = 0.0f;
        for (i=0; i<Height; i++)
        {
            F32 hscale;
            int h1 = (int)h;
            int h2 = (h < pheight-1) ? h1+1 : h1;
            F32 hf1,hf2;
            hf2 = (h1 != h2) ? h - (F32)h1 : 0.0f;
            hf1 = 1.0f - hf2;

            hscale = 1.0f + ((F32)i/(F32)(Height-1))*(point_y_scale - 1.0f) ;
            
            w = 0.0f;
            for (j=0; j<Width; j++)
            {
                int w1 = (int)w;
                int w2 = (w < pwidth-1) ? w1+1 : w1;
                F32 wf2 = (w2 != w1) ? w - (F32)w1 : 0.0f;
                F32 wf1 = 1.0f - wf2;
            
                Vec3 curp;
                Vec2 curtex;
                int pidx = i*Width + j;
                int p11 = h1*pwidth + w1;
                int p12 = h1*pwidth + w2;
                int p21 = h2*pwidth + w1;
                int p22 = h2*pwidth + w2;
                F32 sf11 = hf1*wf1;
                F32 sf12 = hf1*wf2;
                F32 sf21 = hf2*wf1;
                F32 sf22 = hf2*wf2;

                // Positions
                scaleVec3   (pcloth->CurPos[p11], sf11, curp);
                scaleAddVec3(pcloth->CurPos[p12], sf12, curp, curp);
                scaleAddVec3(pcloth->CurPos[p21], sf21, curp, curp);
                scaleAddVec3(pcloth->CurPos[p22], sf22, curp, curp);

                curp[0] *= hscale;
            
                copyVec3(curp, cloth->CurPos[pidx]);
                copyVec3(curp, cloth->OldPos[pidx]);
                copyVec3(curp, cloth->OrigPos[pidx]);
            
                cloth->ColAmt[pidx] = 0;

                // Texture Coordinates
                scaleVec2    (pcloth->renderData.TexCoords[p11], sf11, curtex);
                scaleAddVec2(pcloth->renderData.TexCoords[p12], sf12, curtex, curtex);
                scaleAddVec2(pcloth->renderData.TexCoords[p21], sf21, curtex, curtex);
                scaleAddVec2(pcloth->renderData.TexCoords[p22], sf22, curtex, curtex);
                copyVec2    (curtex, cloth->renderData.TexCoords[pidx]);

                // Masses
                {
                    F32 im11 = pcloth->InvMasses[p11];
                    F32 im12 = pcloth->InvMasses[p12];
                    F32 im21 = pcloth->InvMasses[p21];
                    F32 im22 = pcloth->InvMasses[p22];
                    F32 im1tot = im11 + im12;
                    F32 im2tot = im21 + im22;
                    if (im1tot > 0.0f && im2tot > 0.0f)
                    {
                        // Need to average masses, not invmasses;
                        F32 imtot = (im11*im12*im21*im22);
                        F32 imfrac =
                            sf11 *      im12*im21*im22 +
                            sf12 * im11*     im21*im22 +
                            sf21 * im11*im12*     im22 +
                            sf22 * im11*im12*im21        ;
                        cloth->InvMasses[pidx] = imtot / imfrac;
                    }
                    else
                    {
                        F32 frac;
                        int hook1,hook2;
                        cloth->InvMasses[pidx] = 0.0f;
                        if (im2tot == 0.0f) // prioritize lower row of hooks
                        {
                            hook1 = find_hook(pcloth, p21);
                            hook2 = find_hook(pcloth, p22);
                            frac = sf21 / (sf21+sf22);
                        }
                        else
                        {
                            hook1 = find_hook(pcloth, p11);
                            hook2 = find_hook(pcloth, p12);
                            frac = sf11 / (sf11+sf12);
                        }
                        if (hook1 >= 0 && hook2 >= 0)
                            ClothAddAttachment(cloth, pidx, hook1, hook2, frac);
                    }
                }
                w += dw;
            }
            h += dh;
        }
        ClothSetMatrix(cloth, pcloth->Matrix);
        ClothSetGravity(cloth, pcloth->Gravity);
        ClothSetColRad(cloth,  pcloth->PointColRad);
        ClothSetDrag(cloth, pcloth->Drag);
        ClothSetWind(cloth, pcloth->WindDir, pcloth->WindMag, pcloth->WindScaleFactor );
        ClothSetWindRipple(cloth, pcloth->WindRippleAmt, pcloth->WindRippleRepeat, pcloth->WindRipplePeriod );
        cloth->Time = pcloth->Time;
    }
    return cloth;
}            

//============================================================================
// SET CLOTH PARAMATERS
//============================================================================

void ClothSetNumIterations(Cloth *cloth, int sublod, int niterations)
{
    if (niterations <= 0)
        niterations = 2;
    if (sublod < 0)
    {
        // Set lod 0,1 to niterations, then decrament.
        // Default will be: 2,2,1,1
        int i;
        for (i=0; i<CLOTH_SUB_LOD_NUM; i++)
        {
            cloth->NumIterations[i] = niterations;
#if 0
            if (i >= 1 && niterations > 1)
                niterations--;
#endif
        }
    }
    else
    {
        sublod = MINMAX(sublod, 0, CLOTH_SUB_LOD_NUM-1);
        cloth->NumIterations[sublod] = niterations;
    }
}

void ClothSetColRad(Cloth *cloth, F32 r)
{
    cloth->PointColRad = r;
}

void ClothSetMatrix(Cloth *cloth, Mat4 mat)
{
    ExtractQuat(mat, cloth->mQuat);
    copyMat4(mat, cloth->Matrix);
}

void ClothSetGravity(Cloth *cloth, F32 g)
{
    cloth->Gravity = g;
};

void ClothSetDrag(Cloth *cloth, F32 drag)
{
    drag = MINMAX(drag, 0.0f, MAX_DRAG); // Limit drag to a reasonable range
    cloth->Drag = drag;
};

void ClothSetWind(Cloth *cloth, Vec3 dir, F32 speed, F32 scale)
{
    copyVec3(dir, cloth->WindDir);
    cloth->WindMag = speed;
    cloth->WindScaleFactor = scale;
}

void ClothSetWindRipple(Cloth *cloth, F32 amt, F32 repeat, F32 period)
{
    cloth->WindRippleAmt = amt;
    if (repeat > 0.0f)
        cloth->WindRippleRepeat = repeat;
    if (period > 0.0f)
        cloth->WindRipplePeriod = period;
}

//////////////////////////////////////////////////////////////////////////////
// SubLOD (See ClothObject.c for LOD/SubLOD description)

void ClothSetMinSubLOD(Cloth *cloth, int minsublod)
{
    cloth->MinSubLOD = minsublod;
}

S32 ClothSetSubLOD(Cloth *cloth, int sublod)
{
    cloth->commonData.SubLOD = MINMAX(sublod, cloth->MinSubLOD, CLOTH_SUB_LOD_NUM-1);
    return cloth->commonData.SubLOD;
}

S32  ClothGetSubLOD(Cloth *cloth)
{
    return cloth->commonData.SubLOD;
}

//============================================================================
// COLLISIONS
//============================================================================

// Setup Collisions (see ClothCollide.c)

int ClothAddCollidable(Cloth *cloth)
{
    int idx = cloth->NumCollidables;
    cloth->NumCollidables++;
    cloth->Collidables = CLOTH_REALLOC(cloth->Collidables, ClothCol, cloth->NumCollidables);
    ClothColInitialize(&cloth->Collidables[idx]);
    return idx;
}

ClothCol *ClothGetCollidable(Cloth *cloth, int idx)
{
    if (idx >= 0 && idx < cloth->NumCollidables)
        return cloth->Collidables + idx;
    return 0;
};

// Collision Interface

void ClothCollideCollidable(Cloth *cloth, ClothCol *clothcol)
{
    int p;
    F32 d;
    
    if ((cloth->commonData.SubLOD >= clothcol->MinSubLOD) &&
        (clothcol->MaxSubLOD < 0 || cloth->commonData.SubLOD <= clothcol->MaxSubLOD))
    {
        Vec3 *CurPos = cloth->CurPos;
        F32 *InvMasses = cloth->InvMasses;
        F32 *ColAmt = cloth->ColAmt;
        F32 PointColRad = cloth->PointColRad;
        F32 irad = 1.0f / PointColRad;
        int npoints = cloth->commonData.NumParticles;
        for (p=0; p<npoints; p++)
        {
            if (InvMasses[p] != 0.0f)
            {
                d = ClothColCollideSphere(clothcol, CurPos[p], PointColRad);
#if USE_COLAMT
                d *= irad; // (0, PointColRad] -> (0, 1]
                if (d > ColAmt[p])
                    ColAmt[p] = MIN(d, 1.0f);
#endif
            }
        }
    }
}

void ClothUpdateCollisions(Cloth *cloth)
{
    int i,p;
    int npoints = cloth->commonData.NumParticles;
    for (p=0; p<npoints; p++)
        cloth->ColAmt[p] = 0.0f;
    for (i=0; i<cloth->NumCollidables; i++)
    {
        ClothCol *clothcol = cloth->Collidables + i;
        if ((clothcol->Type != CLOTH_COL_NONE) && !(clothcol->Type & CLOTH_COL_SKIP))
            ClothCollideCollidable(cloth, clothcol);
    }
}

//============================================================================
// UPDATE FUNCTIONS
//============================================================================

//////////////////////////////////////////////////////////////////////////////
// ClothUpdateParticles

// Uses a modified Verlet Integration (from Thomas Jakobsen's 2003 GDC paper):
//   x' = x + (x-x*) + a*dt^2  // (x-x* ~= v*dt) // x* == cloth->OldPos
// With Drag(D,.99=1%):
//   x' = x + D(x-x*) + a*dt^2  // (x-x* ~= v*dt) // x* == cloth->OldPos
// Update OLD position, then swap old and new.

// Defined as a static here so that it can be inlined by the compiler
// Uses x^3 to create a wave similar to a sin-wave (and actually looks better)
// x: [0,2]
static F32 ClothWindGetRipple(F32 x)
{
    if (x > 1.0f)
        x -= 1.0f;
    if (x <= .25f) {
        x = x*4.f - 1.f;
        return 1.f + x*x*x;
    } else if (x <= .50f) {
        x = (x-.25f)*4.f;
        return 1.f - x*x*x;
    } else if (x <= .75f) {
        x = (x-.5f)*4.f - 1.f;
        return -1.f - x*x*x;
    } else {
        x = (x-.75f)*4.f;
        return -1.f + x*x*x;
    }
}

// CLOTH_FLAG_RIPPLE_VERTICAL == 1 creates nice ripples for flags
//  but costs more and doesn't look as good on capes.

void ClothUpdateParticles(Cloth *cloth, F32 dt)
{
    Vec3 *CurPos = cloth->CurPos;
    Vec3 *OldPos = cloth->OldPos;
    F32 *InvMasses = cloth->InvMasses;
    F32 Drag = cloth->Drag;
    F32 *ColAmt = cloth->ColAmt;
    Vec3 Wind;
    int i,j,p;
    int Width = cloth->commonData.Width;
    int Height = cloth->commonData.Height;
    F32 g = cloth->Gravity * dt * (1.0f / 60.0f);
    F32 dt2 = dt*dt;
    int nump = cloth->commonData.NumParticles;
    F32 deltamax = MAXPDPOS*dt;
    F32 windscale;
    F32 ripple_dt; // [0, 1]
    F32 i_ripple_idx_max;
    
#if CLOTH_DEBUG
    setVec3(cloth->MinPos, 999999999.f, 999999999.f, 999999999.f);
    setVec3(cloth->MaxPos, -999999999.f, -999999999.f, -999999999.f);
#endif

    if (cloth->WindRippleAmt > 0.0f)
    {
        ripple_dt = cloth->Time / cloth->WindRipplePeriod;
        ripple_dt -= floor(ripple_dt);
        if (cloth->Flags & CLOTH_FLAG_RIPPLE_VERTICAL)
            i_ripple_idx_max = 1.0f/(F32)(Width+Height-2);
        else
            i_ripple_idx_max = 1.0f/(F32)(Width-1);
    }
    else
    {
        windscale = cloth->WindMag * cloth->WindScaleFactor;
        scaleVec3(cloth->WindDir, windscale, Wind);
    }
    
    for (j=0; j<Width; j++)
    {
        if (!(cloth->Flags & CLOTH_FLAG_RIPPLE_VERTICAL))
        {
            if (cloth->WindRippleAmt > 0.0f)
            {
                F32 ripple,rippleamt,dx;
                dx = (F32)j * i_ripple_idx_max;
                dx *= cloth->WindRippleRepeat;
                dx -= floor(dx);
                dx += ripple_dt;
                rippleamt = ClothWindGetRipple(dx);
                ripple = cloth->WindRippleAmt * rippleamt;
                windscale = (cloth->WindMag + ripple) * cloth->WindScaleFactor;

                scaleVec3(cloth->WindDir, windscale, Wind);
            }
        }
        for (i=0; i<Height; i++)
        {
            int p = i*Width + j;

            if ((cloth->Flags & CLOTH_FLAG_RIPPLE_VERTICAL))
            {
                if (cloth->WindRippleAmt > 0.0f)
                {
                    F32 ripple,rippleamt,dx;
                    dx = (F32)(j+i) * i_ripple_idx_max;
                    dx *= cloth->WindRippleRepeat;
                    dx -= floor(dx);
                    dx += ripple_dt;
                    rippleamt = ClothWindGetRipple(dx);
                    ripple = cloth->WindRippleAmt * rippleamt;
                    windscale = (cloth->WindMag + ripple) * cloth->WindScaleFactor;

                    scaleVec3(cloth->WindDir, windscale, Wind);
                }
            }

            if (InvMasses[p] != 0.0f)
            {
                // Verlet Integration
                Vec3 f,fdt2;
                F32 x,y,z,drag;
                F32 dx,dy,dz;
                scaleVec3(Wind, InvMasses[p], f);
                scaleVec3(f, dt2, fdt2);
                fdt2[1] += g;

#if USE_COLAMT
                if (ColAmt[p] > 0.0f)
                    drag = 1.0f - (ColAmt[p]*.5f) - Drag; // 0 < ColAmt[p] <= 1
                else
#endif
                    drag = 1.0f - Drag;
                x = CurPos[p][0];
                y = CurPos[p][1];
                z = CurPos[p][2];
                dx = (x-OldPos[p][0]);
                dy = (y-OldPos[p][1]);
                dz = (z-OldPos[p][2]);
#if USE_MAXPDPOS
                dx = MINMAX(dx, -deltamax, deltamax);
                dy = MINMAX(dy, -deltamax, deltamax);
                dz = MINMAX(dz, -deltamax, deltamax);
#endif
                // Update OLD position, then swap old and new below
                OldPos[p][0] = x + drag*dx + fdt2[0];
                OldPos[p][1] = y + drag*dy + fdt2[1];
                OldPos[p][2] = z + drag*dz + fdt2[2];
            }
#if CLOTH_DEBUG
            MINVEC3(OldPos[p], cloth->MinPos, cloth->MinPos);
            MAXVEC3(OldPos[p], cloth->MaxPos, cloth->MaxPos);
#endif
        }
    }
    // Swap old and new
    cloth->CurPos = OldPos;
    cloth->OldPos = CurPos;    
#if CLOTH_DEBUG
    subVec3(cloth->MaxPos, cloth->MinPos, cloth->DeltaPos);
    if (cloth->DeltaPos[0] > ClothDebugMaxDist ||
        cloth->DeltaPos[1] > ClothDebugMaxDist ||
        cloth->DeltaPos[2] > ClothDebugMaxDist)
    {
        ClothDebugError++;
    }
#endif
}

// ClothFastUpdateParticles()
//   Used by lesser LODs.
//   Ignores Masses, ColAmt drag modification, Wind Ripple, and (currently) MAXPDPOS
void ClothFastUpdateParticles(Cloth *cloth, F32 dt)
{
    Vec3 *CurPos = cloth->CurPos;
    Vec3 *OldPos = cloth->OldPos;
    F32 *InvMasses = cloth->InvMasses;    
    int p,npoints;
    F32 drag,dt2;
    Vec3 f, fdt2;
    F32 deltamax = MAXPDPOS*dt;
    F32 windscale = cloth->WindMag * cloth->WindScaleFactor;
    
    dt2 = dt*dt;
    scaleVec3(cloth->WindDir, windscale, f); // No local wind
    f[1] += cloth->Gravity;
    scaleVec3(f, dt2, fdt2);
    drag = 1.0f - cloth->Drag;
    npoints = cloth->commonData.NumParticles;
    
    for (p=0; p<npoints; p++)
    {
        if (InvMasses[p] != 0.0f)
        {
            // Verlet Integration
            F32 x,y,z;
            F32 dx,dy,dz;
            x = CurPos[p][0];
            y = CurPos[p][1];
            z = CurPos[p][2];
            dx = (x-OldPos[p][0]);
            dy = (y-OldPos[p][1]);
            dz = (z-OldPos[p][2]);
#if 0 && USE_MAXPDPOS
            dx = MINMAX(dx, -deltamax, deltamax);
            dy = MINMAX(dy, -deltamax, deltamax);
            dz = MINMAX(dz, -deltamax, deltamax);
#endif
            // Update OLD position, then swap old and new below
            OldPos[p][0] = x + drag*dx + fdt2[0];
            OldPos[p][1] = y + drag*dy + fdt2[1];
            OldPos[p][2] = z + drag*dz + fdt2[2];
        }
    }
    // Swap old and new
    cloth->CurPos = OldPos;
    cloth->OldPos = CurPos;    
}

//////////////////////////////////////////////////////////////////////////////
// ClothUpdateAttachments
// Updates the Eyelets using the new Hook positions

// ffrac is used to interpolate between frames. It represents the weight
// between the previous frame and the current frame.
// E.g. if ClothUpdate() is called 3 times during a frame,
//   ClothUpdateAttachments() would be called 3 times with the folliwing ffracs':
//   .333 = 1/3 between old hook positions and new hook positions
//   .5   = 1/2 way between 1/3 position and new position = 2/3 between old and new
//   1.0  = Use new positions

void ClothUpdateAttachments(Cloth *cloth, Vec3 *hooks, F32 ffrac, int freeze)
{
    Vec3 *CurPos = cloth->CurPos;
    Vec3 *OldPos = cloth->OldPos;
    Vec3 tpos;
    Vec3 dposSum={0};
    Vec3 dpos;
    F32 pfrac;
    int i,pidx;
    for (i=0; i<cloth->commonData.NumAttachments; i++)
    {
        ClothAttachment *attach = cloth->commonData.Attachments+i;
        pidx = attach->PIdx;

        // Interpolate between Hook1 and Hook2
        pfrac = attach->Frac;
        if (pfrac >= 1.0f)
        {
            copyVec3(hooks[attach->Hook1], tpos);
        }
        else
        {
            scaleVec3(hooks[attach->Hook1], pfrac, tpos);
            scaleAddVec3(hooks[attach->Hook2], 1.0f-pfrac, tpos, tpos);
        }
        // Interpolate between pervious position and new position
        if (ffrac < 1.0f)
        {
            scaleVec3(tpos, ffrac, tpos);
            scaleAddVec3(CurPos[pidx], 1.0f-ffrac, tpos, tpos);
        }
        if (freeze) {
            subVec3(tpos, OldPos[pidx], dpos);
            addVec3(dpos, dposSum, dposSum);
        }
        copyVec3(tpos, OldPos[pidx]);
        copyVec3(tpos, CurPos[pidx]);
    }
    if (freeze) { // fix for first frame where hooks might "move" a large distance!
        int nump = cloth->commonData.NumParticles;
        scaleVec3(dposSum, 1.f/cloth->commonData.NumAttachments, dpos);
        // Transform all non-harness points by this distance
        for (i=0; i<nump; i++)
        {
            addVec3(CurPos[i], dpos, CurPos[i]);
            copyVec3(CurPos[i], OldPos[i]);
        }
        // Transform harness points back
        for (i=0; i<cloth->commonData.NumAttachments; i++)
        {
            pidx = cloth->commonData.Attachments[i].PIdx;
            subVec3(CurPos[pidx], dpos, CurPos[pidx]);
            copyVec3(CurPos[pidx], OldPos[pidx]);
        }
    }
}

static void xform_particles(Cloth *cloth, Mat4 newmat, int zerovel)
{
#if CLOTH_DEBUG_DPOS
    F32 maxdpos2 = ClothDebugMaxDpos*ClothDebugMaxDpos;
    Vec3 dpos;
#endif
    Vec3 *CurPos = cloth->CurPos;
    Vec3 *OldPos = cloth->OldPos;
    Mat4 imat,tmat;
    int p,nump;
    transposeMat4Copy(cloth->Matrix, imat);
    mulMat4(newmat, imat, tmat);
    nump = cloth->commonData.NumParticles;
    for (p=0; p<nump; p++)
    {
#if CLOTH_DEBUG_DPOS
        Vec3 prev_curpos;
        copyVec3(CurPos[p], prev_curpos);
#endif
        if (zerovel)
        {
            mulVecMat4(CurPos[p], tmat, OldPos[p]);
            copyVec3(OldPos[p], CurPos[p]); // zero velocity
        }
        else
        {
            Vec3 oldpos;
            copyVec3(OldPos[p], oldpos);
            mulVecMat4(CurPos[p], tmat, OldPos[p]); // xform newpos to oldpos
            mulVecMat4(oldpos, tmat, CurPos[p]);    // xform oldpos to newpos
        }
#if CLOTH_DEBUG_DPOS
        subVec3(OldPos[p], prev_curpos, dpos);
        if (lengthVec3Squared(dpos) > maxdpos2)
            ClothDebugError++;
#endif
    }
    cloth->CurPos = OldPos;
    cloth->OldPos = CurPos;    
}

void ClothOffsetInternalPosition(Cloth *cloth, const Vec3 newPositionOffset)
{
    Vec3 delta;
    Vec3 *CurPos = cloth->CurPos;
    Vec3 *OldPos = cloth->OldPos;
    int nump = cloth->commonData.NumParticles, p;
    subVec3(newPositionOffset, cloth->PositionOffset, delta);

    if (!delta[0] && !delta[1] && !delta[2])
        return;

    for (p=0; p<nump; p++)
    {
        subVec3(CurPos[p], delta, CurPos[p]);
        subVec3(OldPos[p], delta, OldPos[p]);
    }
    copyVec3(newPositionOffset, cloth->PositionOffset);
    subVec3(cloth->Matrix[3], delta, cloth->Matrix[3]);
    subVec3(cloth->EntMatrix[3], delta, cloth->EntMatrix[3]);
}


//////////////////////////////////////////////////////////////////////////////
// ClothCheckMaxMotion()
// Check for too much motion.
// 'inmat' = matrix of the primary attachment node
// If the attachment node has moved a LOT
//   (see MAX_FRAME_DPOS and MAX_FRAME_DANG_COS)
//   then just transform the particles to the new location / orientation
// If the attachment node has moved more than the physics can handle
//   (see MAXACCEL, MAXDANG, MAXDPOS)
//   then partially transform the particles such that the amount of motion
//   can be better handled by the physics (i.e. 'pretend' that there was
//   less motion than there really was)
// The 'freeze' paramater indicates that we know that there is a lot of
//   motion and we want to simply transform the particles.

int ClothCheckMaxMotion(Cloth *cloth, F32 dt, Mat4 inmat, int freeze)
{
    int res = 0;
    Vec3 vel;
    Vec4 quat;
    Mat4 newmat;

    // Remove scaling from matrix
    copyMat4(inmat, newmat);
    normalVec3(newmat[0]);
    normalVec3(newmat[1]);
    normalVec3(newmat[2]);

    // Extract Information from the matrix
    ExtractQuat(newmat, quat);
    subVec3(newmat[3], cloth->Matrix[3], vel);

    // Once 'res' is set, further checks are ignored.
    if (freeze)
    {
        xform_particles(cloth, newmat, 1);
        res = 1;
    }
    
#if USE_MAXACCEL
    // Check Velocity
    if (!res)
    {
        Vec3 dvel;
        F32 maxaccel, accelamt2;
        maxaccel = MAXACCEL * dt;
        subVec3(vel, cloth->Vel, dvel);
        accelamt2 = dotVec3(dvel,dvel);
        if (accelamt2 > maxaccel*maxaccel)
        {
              xform_particles(cloth, newmat, 1);
            res = 1;
        }
    }
#endif // USE_MAXACCEL
    
#if USE_MAXDANG
    // Check Rotation
    if (!res)
    {
        F32 dp;

        dp = dotVec4(quat, cloth->mQuat); // = cos (theta/2)
        dp = fabs(dp); // don't care if rotation is negative or positive
        if (dp < MAX_FRAME_DANG_COS) // .707 = 90 degrees in one frame
        {
            xform_particles(cloth, newmat, 1);
            res = 1;
        }
        else
        {
            F32 maxdang = MAXDANG*dt;
            F32 mincosdang = fcos(maxdang);
            if (dp < mincosdang)
            {
                // .707 < dp < MINCOSDANG
                F32 dang = acosf(dp);
                F32 frac1 = maxdang / dang;
                F32 frac2 = 1.0f - frac1;
                F32 n;
                Mat4 tmat;
                Vec4 tquat;
                tquat[0] = quat[0]*frac2 + cloth->mQuat[0]*frac1;
                tquat[1] = quat[1]*frac2 + cloth->mQuat[1]*frac1;
                tquat[2] = quat[2]*frac2 + cloth->mQuat[2]*frac1;
                tquat[3] = quat[3]*frac2 + cloth->mQuat[3]*frac1;
                NormalizeQuat(tquat);
                CreateQuatMat(tmat, tquat);
                copyVec3(cloth->Matrix[3], tmat[3]); // Copy pos
                xform_particles(cloth, tmat, 0);
                res = 0;
            }
        }
    }
#endif // USE_MAXDANG

#if USE_MAXDPOS
    // Check Position
    if (!res)
    {
        Vec3 dpos;
        F32 dist;
        F32 maxdpos = MAXDPOS * dt;

        copyVec3(vel, dpos);
        dist = normalVec3(dpos);
        
        if (dist > MAX_FRAME_DPOS)
        {
            xform_particles(cloth, newmat, 1);
            res = 1;
        }
        else if (dist > maxdpos)
        {
            int p;
            Vec3 *CurPos = cloth->CurPos;
            Vec3 *OldPos = cloth->OldPos;
            int nump = cloth->commonData.NumParticles;
            F32 frac = (dist - maxdpos);
              scaleVec3(dpos, frac, dpos);
            for (p=0; p<nump; p++)
            {
                addVec3(CurPos[p], dpos, CurPos[p]);
                addVec3(OldPos[p], dpos, OldPos[p]); // maintain velocity
            }
            res = 0;
        }
        else
        {
            res = 0;
        }
    }
#endif // USE_MAXDPOS

    // Store updated values
    copyMat4(newmat, cloth->Matrix);
    copyVec4(quat, cloth->mQuat);
    copyVec3(vel, cloth->Vel);

    cloth->SkipUpdatePhysics = res;
    
    return res;
}

//////////////////////////////////////////////////////////////////////////////
// Convenience function

void ClothUpdatePosition(Cloth *cloth, F32 dt, Mat4 newmat, Vec3 *hooks, int freeze)
{
    ClothCheckMaxMotion(cloth, dt, newmat, freeze);
    ClothUpdateAttachments(cloth, hooks, 1.0f, freeze);
}

//////////////////////////////////////////////////////////////////////////////
// Update Constraints
// See ClothConstraint.c

void ClothUpdateConstraints(Cloth *cloth)
{
    int c;
    int SubLOD = cloth->commonData.SubLOD;
    int iterations = cloth->NumIterations[SubLOD];
    if (iterations < 1)
        iterations = 1;
    for( ; iterations>0; iterations--)
    {
        int nconstraints = cloth->NumLengthConstraints[SubLOD];
        if (SubLOD >= 3)
        {
#if CONSTRAIN_TOP_FIRST
            for (c=0; c<nconstraints; c++)
#else
            for (c=nconstraints-1; c>=0; c--)
#endif
            {
                ClothLengthConstraintFastUpdate(&cloth->commonData.LengthConstraints[c], cloth);
            }
        }
        else
        {
#if CONSTRAIN_TOP_FIRST
            for (c=0; c<nconstraints; c++)
#else
            for (c=nconstraints-1; c>=0; c--)
#endif
            {
                ClothLengthConstraintUpdate(&cloth->commonData.LengthConstraints[c], cloth);
            }
        }
    }
}


//////////////////////////////////////////////////////////////////////////////
// Update Normals
// Calculates the Normals for the Modeled Particles using a cross product
//   from the deltas between neighboring Horizontal and Vertical particles.
// NormalScale is computed by averging the dot products of between the
//   normal and the delta between the Particle and each neighbor.
//   This value is inverted so that a positive value represents a relative
//   maximum and a negative value represents a relative minimum (with the
//   magnitude determing how steep of a peak or valley).
//   This value can be used to scale the normals or in a shader to darken
//   the valleys to simulate shadows.
// The Horizontal component used to calculate the normal is stored as a
//   BiNormal which can be used for bumpmapping.

static F32 calc_dp(Vec3 psegment, Vec3 p0, Vec3 norm, F32 ilength)
{
    Vec3 dv;
    F32 dp;
    subVec3(psegment, p0, dv);
    dp = dotVec3(dv, norm) * ilength;
#if 0
    if (dp >= 0.0f)
        dp = dp*dp;
    else
        dp = -dp*dp;
#endif
    return dp;
}

// Use a 1st order Taylor-expansion to approximate the square root result
void fastNormalVec3(Vec3 vec, F32 approxlen)
{
    F32 x = vec[0];
    F32 y = vec[1];
    F32 z = vec[2];
    F32 dp = x*x + y*y + z*z;
    F32 ilen = approxlen / (.5f*(approxlen*approxlen + dp));
    vec[0] = x * ilen;
    vec[1] = y * ilen;
    vec[2] = z * ilen;
}

// Uses fastNormalVec3 and does not calculate NormaleScale (sets to 1.0)
void ClothFastUpdateNormals(ClothRenderData *renderData, int calcFirstNormals)
{
    int Width = renderData->commonData.Width;
    int Height = renderData->commonData.Height;
    Vec3 *CurPos = renderData->RenderPos;
    int numc1 = Height*(Width-1); // Vert constraints
    
    int i,j;
    int pidx = 0;
    for (i=0; i<Height; i++)
    {
        int iw = i*Width;
#if CLOTH_CALC_BINORMALS || CLOTH_CALC_TANGENTS
        int ciw = iw - i; //i*(Width-1);
#endif
        for (j=0; j<Width; j++, pidx++)
        {
            // Calculate Normal
            int pidx = iw + j;
            int px1 = j > 0 ? pidx-1 : pidx;
            int px2 = j < Width-1 ? pidx+1 : pidx;
            int py1 = i > 0 ? pidx-Width : pidx;
            int py2 = i < Height-1 ? pidx+Width : pidx;
            Vec3 vx,vy;
            subVec3(CurPos[px2], CurPos[px1], vx);
            subVec3(CurPos[py2], CurPos[py1], vy);
            if (i!=0 || calcFirstNormals)
            {
                crossVec3(vx, vy, renderData->Normals[pidx]);
#                if 1 //CLOTH_NORMALIZE_NORMALS
                    fastNormalVec3(renderData->Normals[pidx], renderData->commonData.ApproxNormalLens[pidx]);
#                endif
            }
            renderData->NormalScale[pidx] = 1.0f;
#            if CLOTH_CALC_BINORMALS
            {
#                if CLOTH_NORMALIZE_BINORMALS
                int cidx = (j>0) ? ciw + j-1 : ciw; // horiz constraint index
                F32 rl = renderData->commonData.LengthConstraints[cidx].RestLength;
                if (j > 0 && j < Width-1)
                    rl *= 2.f;
                fastNormalVec3(vx, rl);
#                endif
                copyVec3(vx, renderData->BiNormals[pidx]);
            }
#            endif
#            if CLOTH_CALC_TANGENTS
            {
#                if CLOTH_NORMALIZE_TANGENTS
                    int cidx = (i>0) ? numc1 + (iw-Width) + j : numc1 + j; // vert constraint index
                    F32 rl = renderData->commonData.LengthConstraints[cidx].RestLength;
                    if (i > 0 && i < Height-1)
                        rl *= 2.f;
                    fastNormalVec3(vy, rl);
#                endif
                copyVec3(vy, renderData->Tangents[pidx]);
            }
#            endif
        }
    }
}

void ClothUpdateNormals(ClothRenderData *renderData)
{
    int SubLOD = renderData->commonData.SubLOD;
    Vec3 *hookNormals = renderData->hookNormals;
    // Copy normals from hooks if they exist
    if (hookNormals)
    {
        F32 pfrac;
        int i,pidx;
        for (i=0; i<renderData->commonData.NumAttachments; i++)
        {
            ClothAttachment *attach = &renderData->commonData.Attachments[i];
            pidx = attach->PIdx;

            // Interpolate between Hook1 and Hook2
            pfrac = attach->Frac;
            if (pfrac >= 1.0f)
            {
                copyVec3(hookNormals[attach->Hook1], renderData->Normals[pidx]);
            }
            else
            {
                scaleVec3(hookNormals[attach->Hook1], pfrac, renderData->Normals[pidx]);
                scaleAddVec3(hookNormals[attach->Hook2], 1.0f-pfrac, renderData->Normals[pidx], renderData->Normals[pidx]);
            }
        }
    }
    if (SubLOD >= 3) {
        ClothFastUpdateNormals(renderData, !hookNormals);
        return;
    }
    {
    int i,j;
    int Width = renderData->commonData.Width;
    int Height = renderData->commonData.Height;
    Vec3 *CurPos = renderData->RenderPos;
    int numc1 = Height*(Width-1);
    const ClothLengthConstraint *LengthConstraints = renderData->commonData.LengthConstraints;
    
    for (i=0; i<Height; i++)
    {
        int iw = i*Width;
        int ciw = iw - i; //i*(Width-1);
        for (j=0; j<Width; j++)
        {
            // Calculate Normal
            int pidx = iw + j;
            int px1 = j > 0 ? pidx-1 : pidx;
            int px2 = j < Width-1 ? pidx+1 : pidx;
            int py1 = i > 0 ? pidx-Width : pidx;
            int py2 = i < Height-1 ? pidx+Width : pidx;
            Vec3 vx,vy;
            F32 *norm = renderData->Normals[pidx];
            subVec3(CurPos[px2], CurPos[px1], vx);
            subVec3(CurPos[py2], CurPos[py1], vy);
            if (i!=0 || !hookNormals) {
                crossVec3(vx, vy, norm);
#                if 1 //CLOTH_NORMALIZE_NORMALS
                    if (SubLOD <= 1) {
                        normalVec3(norm); // !!!
                    } else {
                        fastNormalVec3(norm, renderData->commonData.ApproxNormalLens[pidx]);
                    }
#                endif
            }
#            if CLOTH_CALC_BINORMALS
            {
#                if CLOTH_NORMALIZE_BINORMALS
                int cidx = (j>0) ? ciw + j-1 : ciw; // horiz constraint index
                F32 rl = LengthConstraints[cidx].RestLength;
                if (j > 0 && j < Width-1)
                    rl *= 2.f;
                fastNormalVec3(vx, rl);
#                endif
                copyVec3(vx, renderData->BiNormals[pidx]);
            }
#            endif
#            if CLOTH_CALC_TANGENTS
            {
#                if CLOTH_NORMALIZE_TANGENTS
                    int cidx = (i>0) ? numc1 + (iw-Width) + j : numc1 + j; // vert constraint index
                    F32 rl = LengthConstraints[cidx].RestLength;
                    if (i > 0 && i < Height-1)
                        rl *= 2.f;
                    fastNormalVec3(vy, rl);
#                endif
                copyVec3(vy, renderData->Tangents[pidx]);
            }
#            endif
            // Calculate Normal scale
            if (LengthConstraints)
            {
                F32 dp = 0;
                if (j > 0)
                    dp += calc_dp(CurPos[px1], CurPos[pidx], norm, LengthConstraints[ciw + j - 1].InvLength);
                if (j < Width-1)
                    dp += calc_dp(CurPos[px2], CurPos[pidx], norm, LengthConstraints[ciw + j].InvLength);
                if (i > 0)
                    dp += calc_dp(CurPos[py1], CurPos[pidx], norm, LengthConstraints[numc1 + iw + j - Width].InvLength);
                if (i < Height-1)
                    dp += calc_dp(CurPos[py2], CurPos[pidx], norm, LengthConstraints[numc1 + iw + j].InvLength);
                renderData->NormalScale[pidx] = -dp*.25f;
            }
            else
            {
                renderData->NormalScale[pidx] = 1.0f;
            }
        }
    }
    }

}

//////////////////////////////////////////////////////////////////////////////
// Update Intermediate Points
// There are two levels of intermediate points calculated based on SubLod.
// Positions, Normals, and NormalScale are all calculated
// SubLod <= 1:
//  The centers, Horizontal midpoints, and Vertical Midpoints are calculated.
//  Positions are averaged, except along the edges where they are pushed
//    out according to the Normals and NormalScale values to smooth the curves
//  Normals are not averaged, but are calculated using the cross product of
//    deltas from the interpolated position to its neighbots.
//  NormalScale values are averaged.
// SubLod == 0:
//  Edges curves are smoothed by calculating intermediate points between
//    each existing point (Modeled Particles + midpoints).
//  Positions are calculated by using the NormalScale value to 'push' the
//    points away from the edges.
//  Normals are averaged and NOT re-normalized becase the normals will
//    be pretty similar so averging should produce reasonable normals.
//  NormalScale values are averaged.

void ClothUpdateIntermediatePoints(ClothRenderData *renderData)
{
    static F32 curvescale = 0.667; // Larger values poduce more exagerated curves
    int SubLOD = renderData->commonData.SubLOD;
    if (SubLOD < 2)
    {
        int Width = renderData->commonData.Width;
        int Height = renderData->commonData.Height;
        Vec3 *CurPos = renderData->RenderPos;
        Vec3 *Normals = renderData->Normals;
#if CLOTH_CALC_BINORMALS
        Vec3 *BiNormals = renderData->BiNormals;
#endif
#if CLOTH_CALC_TANGENTS
        Vec3 *Tangents = renderData->Tangents;
#endif
        F32 *NormalScale = renderData->NormalScale;
        const ClothLengthConstraint *LengthConstraints = renderData->commonData.LengthConstraints;
    
        int i,j;
        int nump1 = Width*Height; // centers
        int nump2 = nump1 + (Width-1)*(Height-1); // horiz edge midpoints
        int nump3 = nump2 + (Width-1)*(Height); // vert edge midpoints
        int nump4 = nump3 + (Width)*(Height-1); // horiz edge border points
        int nump5 = nump4 + 4*(Width-1); // vert edge border points
        //int nump6 = nump2 + 4*(Height-1); // end
        int numc1 = Height*(Width-1); // Vert constraints

        // Centers
        if (SubLOD <= 1)
        {
            for (i=0; i<Height-1; i++)
            {
                int iw = i*Width;
                int ciw = iw - i; //i*(Width-1);
                for (j=0; j<Width-1; j++)
                {
                    int pidx = iw + j;
                    int p00 = pidx;
                    int p01 = pidx+1;
                    int p10 = pidx + Width;
                    int p11 = p10 + 1;
                    int cidx;
                    Vec3 pmid;

                    // pmid = Center
                    addVec3(CurPos[p00], CurPos[p01], pmid);
                    addVec3(pmid, CurPos[p10], pmid);
                    addVec3(pmid, CurPos[p11], pmid);
                    scaleVec3(pmid, .25f, pmid); // avg
                    cidx = nump1 + ciw + j;
                    copyVec3(pmid, CurPos[cidx]);;

                    // Cross the diagonals to get the normal
                    {
                        Vec3 vx,vy;

                        subVec3(CurPos[p11], CurPos[p00], vx);
                        subVec3(CurPos[p10], CurPos[p01], vy);
                        crossVec3(vx, vy, Normals[cidx]);
                        if (CLOTH_NORMALIZE_NORMALS)
                        {
                            normalVec3(Normals[cidx]); // !!!
                        }
#                        if CLOTH_CALC_BINORMALS
                        {
                            subVec3(CurPos[p01], CurPos[p00], vx); // need horizontal component for binormal
#                            if CLOTH_NORMALIZE_BINORMALS
                            {
                                int cidx2 = (j>0) ? ciw + j-1 : ciw; // horiz constraint index
                                fastNormalVec3(vx, LengthConstraints[cidx2].RestLength);
                            }
#                            endif
                            copyVec3(vx, BiNormals[cidx]);
                        }
#                        endif
#                        if CLOTH_CALC_TANGENTS
                        {
                            subVec3(CurPos[p10], CurPos[p00], vy); // need vertical component for tangent
#                            if CLOTH_NORMALIZE_TANGENTS
                            {
                                int cidx2 = (i>0) ? numc1 + (iw-Width) + j : numc1 + j; // vert constraint index
                                fastNormalVec3(vy, LengthConstraints[cidx2].RestLength);
                            }
#                            endif
                            copyVec3(vy, Tangents[cidx]);
                        }
#                        endif
                    }
#if CLOTH_SCALE_NORMALS
                    // Average the NormalScales
                    NormalScale[cidx] = (NormalScale[pidx] + NormalScale[pidx + 1] +
                                        NormalScale[pidx + Width] + NormalScale[pidx + Width + 1]) * .25f;
#endif
                }
            }

            // Midpoints
            
            // Horizontal midpoints
            for (i=0; i<Height; i++)
            {
                int iw = i*Width;
                int ciw = iw - i; //i*(Width-1);
                for (j=0; j<Width-1; j++)
                {
                    int pidx = iw + j;
                    int p00 = pidx;
                    int p01 = pidx+1;
                    int hidx = nump2 + ciw + j;

                    // Calculate the midpoint first
                    addVec3(CurPos[p00], CurPos[p01], CurPos[hidx]);
                    scaleVec3(CurPos[hidx], .5f, CurPos[hidx]); // avg

                    if (i==0) {
                        // Just average the normals, etc (so it matches with what the harness connects to)
                        addVec3(Normals[p00], Normals[p01], Normals[hidx]);
                        scaleVec3(Normals[hidx],.5f,Normals[hidx]);
                        if (CLOTH_NORMALIZE_NORMALS)
                        {
                            normalVec3(Normals[hidx]); // !!!
                        }
#                        if CLOTH_CALC_BINORMALS
                        {
                            addVec3(BiNormals[p00], BiNormals[p01], BiNormals[hidx]);
                            scaleVec3(BiNormals[hidx],.5f,BiNormals[hidx]);
                        }
#                        endif
#                        if CLOTH_CALC_TANGENTS
                        {
                            addVec3(Tangents[p00], Tangents[p01], Tangents[hidx]);
                            scaleVec3(Tangents[hidx],.5f,Tangents[hidx]);
                        }
#                        endif
                    } else
                    // Calculate the normal second (uses midpoint along edges)
                    // Cross the horizontal and vertical neighbors to get the normal
                    {
                        int cidx = nump1 + ciw + j;
                        int py2 = (i<Height-1) ? cidx : hidx; // center
                        int py1 = (i>0) ? cidx - (Width-1) : hidx;
                        Vec3 vx,vy;
                        subVec3(CurPos[p01], CurPos[p00], vx);
                        subVec3(CurPos[py2], CurPos[py1], vy);
                        crossVec3(vx, vy, Normals[hidx]);
                        if (CLOTH_NORMALIZE_NORMALS)
                        {
                            normalVec3(Normals[hidx]); // !!!
                        }
#                        if CLOTH_CALC_BINORMALS
                        {
#                            if CLOTH_NORMALIZE_BINORMALS
                                int cidx2 = (j>0) ? ciw + j-1 : ciw; // horiz constraint index
                                fastNormalVec3(vx, LengthConstraints[cidx2].RestLength);
#                            endif
                            copyVec3(vx, BiNormals[hidx]);
                        }
#                        endif
#                        if CLOTH_CALC_TANGENTS
                        {
#                            if CLOTH_NORMALIZE_TANGENTS
                                int cidx2 = (i>0) ? numc1 + (iw-Width) + j : numc1 + j; // vert constraint index
                                F32 rl = LengthConstraints[cidx2].RestLength;
                                if (i==0 || i==Height-1)
                                    rl *= .5f; // half segment
                                fastNormalVec3(vy, rl);
#                            endif
                            copyVec3(vy, Tangents[hidx]);
                        }
#                        endif
                    }

                    {
                        F32 avg_norm_scale = (NormalScale[p00] + NormalScale[p01])*.5f;
#if CLOTH_SCALE_NORMALS
                        // Average the NormalScales
                        NormalScale[hidx] = avg_norm_scale;
#endif
                        // Push out the edge midpoints third (uses normal)
                        if (i==Height-1) // JE: Can't do this on the top edge or it won't match the harness!
                        {
                            F32 scale = LengthConstraints[ciw+j].RestLength*curvescale;
                            Vec3 nvec;
                            scale *= avg_norm_scale;
                            scaleVec3(Normals[hidx], scale, nvec);
                            addVec3(CurPos[hidx], nvec, CurPos[hidx]);
                        }
                    }
                }
            }
            // Vertical midpoints
            for (i=0; i<Height-1; i++)
            {
                int iw = i*Width;
                int ciw = iw - i; //i*(Width-1);
                for (j=0; j<Width; j++)
                {
                    int pidx = iw + j;
                    int p00 = pidx;
                    int p10 = pidx + Width;
                    int vidx = nump3 + iw + j;

                    // Calculate the midpoint first
                    addVec3(CurPos[p00], CurPos[p10], CurPos[vidx]);
                    scaleVec3(CurPos[vidx], .5f, CurPos[vidx]); // avg
                    
                    // Calculate the normal second (uses midpoint along edges)
                    // Cross the horizontal and vertical neighbors to get the normal
                    {
                        int cidx = nump1 + ciw + j;
                        int px2 = (j<Width-1) ? cidx : vidx; // center
                        int px1 = (j>0) ? cidx - 1 : vidx;
                        Vec3 vx,vy;
                        subVec3(CurPos[p10], CurPos[p00], vy);
                        subVec3(CurPos[px2], CurPos[px1], vx);
                        crossVec3(vx, vy, Normals[vidx]);
                        if (CLOTH_NORMALIZE_NORMALS) {
                            normalVec3(Normals[vidx]); // !!!
                        }
#                        if CLOTH_CALC_BINORMALS
                        {
#                            if CLOTH_NORMALIZE_BINORMALS
                                int cidx2 = (j>0) ? ciw + j-1 : ciw; // horiz constraint index
                                F32 rl = LengthConstraints[cidx2].RestLength;
                                if (j==0 || j==Width-1)
                                    rl *= .5f; // half segment
                                fastNormalVec3(vx, rl);
#                            endif
                            copyVec3(vx, BiNormals[vidx]);
                        }
#                        endif
#                        if CLOTH_CALC_TANGENTS
                        {
#                            if CLOTH_NORMALIZE_TANGENTS
                                int cidx2 = (i>0) ? numc1 + (iw-Width) + j : numc1 + j; // vert constraint index
                                fastNormalVec3(vy, LengthConstraints[cidx2].RestLength);
#                            endif
                            copyVec3(vy, Tangents[vidx]);
                        }
#                        endif
                    }

                    {
                        F32 avg_norm_scale = (NormalScale[p00] + NormalScale[p10])*.5f;
#if CLOTH_SCALE_NORMALS
                        // Average the NormalScales
                        NormalScale[vidx] = avg_norm_scale;
#endif
                    
                        // Push out the edge midpoints third (uses normal)
                        if (j==0 || j==Width-1)
                        {
                            Vec3 nvec;
                            F32 scale = LengthConstraints[numc1 + iw+j].RestLength*curvescale;
                            scale *= avg_norm_scale;
                            scaleVec3(Normals[vidx], scale, nvec);
                            addVec3(CurPos[vidx], nvec, CurPos[vidx]);
                        }
                    }
                }
            }
        }
        // Edge curves
        // Create 2 additional interpolated points per Particle along each edge
        //   to approximate a curve.
        // NOTE: Cheap hack:
        //  Because interior points interpolate intensities instead of normals,
        //  linearly interpolate the normals and do not normalize them (this is cheaper! :))
        if (SubLOD <= 0)
        {
            // Horizontal midpoints
            for (j=0; j<Width-1; j++)
            {
                // Top edge
                {
                    int pidx1 = j;
                    int pidx2 = pidx1+1;
                    int cidx = nump2 + j;
                    int hidx1 = nump4 + j*2;
                    int hidx2 = hidx1+1;
                    Vec3 tvec,nvec;

                    // Average the Normals
                    addVec3(Normals[cidx], Normals[pidx1], tvec);
                    scaleVec3(tvec,.5f,Normals[hidx1]);
                    addVec3(Normals[cidx], Normals[pidx2], tvec);
                    scaleVec3(tvec,.5f,Normals[hidx2]);
                    if (CLOTH_NORMALIZE_NORMALS)
                    {
                        //normalVec3(Normals[hidx1]); // !!
                        //normalVec3(Normals[hidx2]); // !!
                    }

#if CLOTH_CALC_BINORMALS
                    addVec3(BiNormals[cidx], BiNormals[pidx1], tvec);
                    scaleVec3(tvec,.5f,BiNormals[hidx1]);
                    addVec3(BiNormals[cidx], BiNormals[pidx2], tvec);
                    scaleVec3(tvec,.5f,BiNormals[hidx2]);
#if CLOTH_NORMALIZE_BINORMALS
                    //normalVec3(BiNormals[hidx1]); // !!
                    //normalVec3(BiNormals[hidx2]); // !!
#endif
#endif
#if CLOTH_CALC_TANGENTS
                    addVec3(Tangents[cidx], Tangents[pidx1], tvec);
                    scaleVec3(tvec,.5f,Tangents[hidx1]);
                    addVec3(Tangents[cidx], Tangents[pidx2], tvec);
                    scaleVec3(tvec,.5f,Tangents[hidx2]);
#if CLOTH_NORMALIZE_TANGENTS
                    //normalVec3(Tangents[hidx1]); // !!
                    //normalVec3(Tangents[hidx2]); // !!
#endif
#endif
                
#if CLOTH_SCALE_NORMALS
                    // Average the NormalScales
                    NormalScale[hidx1] = (NormalScale[cidx] + NormalScale[pidx1])*.5f;
                    NormalScale[hidx2] = (NormalScale[cidx] + NormalScale[pidx2])*.5f;
#endif

                    // Use the averaged Normals and NormalScales to push out the edge points
                    if (0) { // JE: Can't do this on the top edge or it won't match the harness!
                        int pm2 = cidx;
                        F32 scale = LengthConstraints[j].RestLength*curvescale;
                        F32 scale1 = scale * NormalScale[pidx1];                
                        F32 scale2 = scale * NormalScale[pidx2];
                
                        addVec3(CurPos[pidx1], CurPos[pm2], tvec);
                        scaleVec3(Normals[hidx1],scale1,nvec);
                        scaleAddVec3(tvec, .5f, nvec, CurPos[hidx1]);

                        addVec3(CurPos[pidx2], CurPos[pm2], tvec);
                        scaleVec3(Normals[hidx2],scale2,nvec);
                        scaleAddVec3(tvec, .5f, nvec, CurPos[hidx2]);
                    } else {
                        int pm2 = cidx;
                        addVec3(CurPos[pidx1], CurPos[pm2], CurPos[hidx1]);
                        scaleVec3(CurPos[hidx1], .5f, CurPos[hidx1]); // avg

                        addVec3(CurPos[pidx2], CurPos[pm2], CurPos[hidx2]);
                        scaleVec3(CurPos[hidx2], .5f, CurPos[hidx2]); // avg
                    }
                }
                // Bottom edge
                {
                    int pidx1 = (Height-1)*(Width) + j;
                    int pidx2 = pidx1+1;
                    int cidx = nump2 + (Height-1)*(Width-1) + j;
                    int hidx1 = nump4 + (Width-1)*2 + j*2;
                    int hidx2 = hidx1+1;
                    Vec3 tvec,nvec;
                
                    // Average the Normals
                    addVec3(Normals[cidx], Normals[pidx1], tvec);
                    scaleVec3(tvec,.5f,Normals[hidx1]);
                    addVec3(Normals[cidx], Normals[pidx2], tvec);
                    scaleVec3(tvec,.5f,Normals[hidx2]);
                    if (CLOTH_NORMALIZE_NORMALS)
                    {
                        //normalVec3(Normals[hidx1]); // !!
                        //normalVec3(Normals[hidx2]); // !!
                    }

#if CLOTH_CALC_BINORMALS
                    addVec3(BiNormals[cidx], BiNormals[pidx1], tvec);
                    scaleVec3(tvec,.5f,BiNormals[hidx1]);
                    addVec3(BiNormals[cidx], BiNormals[pidx2], tvec);
                    scaleVec3(tvec,.5f,BiNormals[hidx2]);
#if CLOTH_NORMALIZE_BINORMALS
                    //normalVec3(BiNormals[hidx1]); // !!
                    //normalVec3(BiNormals[hidx2]); // !!
#endif
#endif
#if CLOTH_CALC_TANGENTS
                    addVec3(Tangents[cidx], Tangents[pidx1], tvec);
                    scaleVec3(tvec,.5f,Tangents[hidx1]);
                    addVec3(Tangents[cidx], Tangents[pidx2], tvec);
                    scaleVec3(tvec,.5f,Tangents[hidx2]);
#if CLOTH_NORMALIZE_TANGENTS
                    //normalVec3(Tangents[hidx1]); // !!
                    //normalVec3(Tangents[hidx2]); // !!
#endif
#endif
                
#if CLOTH_SCALE_NORMALS
                    // Average the NormalScales
                    NormalScale[hidx1] = (NormalScale[cidx] + NormalScale[pidx1])*.5f;
                    NormalScale[hidx2] = (NormalScale[cidx] + NormalScale[pidx2])*.5f;
#endif

                    // Use the averaged Normals and NormalScales to push out the edge points
                    {
                        int pm2 = cidx;
                        F32 scale = LengthConstraints[j].RestLength*curvescale;
                        F32 scale1 = scale * NormalScale[pidx1];
                        F32 scale2 = scale * NormalScale[pidx2];
                
                        addVec3(CurPos[pidx1], CurPos[pm2], tvec);
                        scaleVec3(Normals[hidx1],scale1,nvec);
                        scaleAddVec3(tvec, .5f, nvec, CurPos[hidx1]);

                        addVec3(CurPos[pidx2], CurPos[pm2], tvec);
                        scaleVec3(Normals[hidx2],scale2,nvec);
                        scaleAddVec3(tvec, .5f, nvec, CurPos[hidx2]);
                    }
                }
            }
            for (i=0; i<Height-1; i++)
            {
                // Left edge
                {
                    int pidx1 = i*Width;
                    int pidx2 = pidx1+Width;
                    int cidx = nump3 + i*(Width);
                    int vidx1 = nump5 + i*2;
                    int vidx2 = vidx1+1;
                    Vec3 tvec,nvec;
                
                    // Average the Normals
                    addVec3(Normals[cidx], Normals[pidx1], tvec);
                    scaleVec3(tvec,.5f,Normals[vidx1]);
                    addVec3(Normals[cidx], Normals[pidx2], tvec);
                    scaleVec3(tvec,.5f,Normals[vidx2]);
                    if (CLOTH_NORMALIZE_NORMALS)
                    {
                        //normalVec3(Normals[vidx1]); // !!
                        //normalVec3(Normals[vidx2]); // !!
                    }

#if CLOTH_CALC_BINORMALS
                    addVec3(BiNormals[cidx], BiNormals[pidx1], tvec);
                    scaleVec3(tvec,.5f,BiNormals[vidx1]);
                    addVec3(BiNormals[cidx], BiNormals[pidx2], tvec);
                    scaleVec3(tvec,.5f,BiNormals[vidx2]);
#if CLOTH_NORMALIZE_BINORMALS
                    //normalVec3(BiNormals[vidx1]); // !!
                    //normalVec3(BiNormals[vidx2]); // !!
#endif
#endif
#if CLOTH_CALC_TANGENTS
                    addVec3(Tangents[cidx], Tangents[pidx1], tvec);
                    scaleVec3(tvec,.5f,Tangents[vidx1]);
                    addVec3(Tangents[cidx], Tangents[pidx2], tvec);
                    scaleVec3(tvec,.5f,Tangents[vidx2]);
#if CLOTH_NORMALIZE_TANGENTS
                    //normalVec3(Tangents[vidx1]); // !!
                    //normalVec3(Tangents[vidx2]); // !!
#endif
#endif
                
#if CLOTH_SCALE_NORMALS
                    // Average the NormalScales
                    NormalScale[vidx1] = (NormalScale[cidx] + NormalScale[pidx1])*.5f;
                    NormalScale[vidx2] = (NormalScale[cidx] + NormalScale[pidx2])*.5f;
#endif

                    // Use the averaged Normals and NormalScales to push out the edge points
                    {
                        int pm2 = cidx;
                        F32 scale = LengthConstraints[numc1 + i*Width].RestLength*curvescale;
                        F32 scale1 = scale * NormalScale[pidx1];
                        F32 scale2 = scale * NormalScale[pidx2];

                        addVec3(CurPos[pidx1], CurPos[pm2], tvec);
                        scaleVec3(Normals[vidx1],scale1,nvec);
                        scaleAddVec3(tvec, .5f, nvec, CurPos[vidx1]);

                        addVec3(CurPos[pidx2], CurPos[pm2], tvec);
                        scaleVec3(Normals[vidx2],scale2,nvec);
                        scaleAddVec3(tvec, .5f, nvec, CurPos[vidx2]);
                    }
                }
                {
                    // Right edge
                    int pidx1 = i*Width + (Width-1);
                    int pidx2 = pidx1+Width;
                    int cidx = nump3 + i*(Width) + Width-1;
                    int vidx1 = nump5 + (Height-1)*2 + i*2;
                    int vidx2 = vidx1+1;
                    Vec3 tvec,nvec;
                
                    // Average the Normals
                    addVec3(Normals[cidx], Normals[pidx1], tvec);
                    scaleVec3(tvec,.5f,Normals[vidx1]);
                    addVec3(Normals[cidx], Normals[pidx2], tvec);
                    scaleVec3(tvec,.5f,Normals[vidx2]);
                    if (CLOTH_NORMALIZE_NORMALS)
                    {
                        //normalVec3(Normals[vidx1]); // !!
                        //normalVec3(Normals[vidx2]); // !!
                    }

#if CLOTH_CALC_BINORMALS
                    addVec3(BiNormals[cidx], BiNormals[pidx1], tvec);
                    scaleVec3(tvec,.5f,BiNormals[vidx1]);
                    addVec3(BiNormals[cidx], BiNormals[pidx2], tvec);
                    scaleVec3(tvec,.5f,BiNormals[vidx2]);
#if CLOTH_NORMALIZE_BINORMALS
                    //normalVec3(BiNormals[vidx1]); // !!
                    //normalVec3(BiNormals[vidx2]); // !!
#endif
#endif
#if CLOTH_CALC_TANGENTS
                    addVec3(Tangents[cidx], Tangents[pidx1], tvec);
                    scaleVec3(tvec,.5f,Tangents[vidx1]);
                    addVec3(Tangents[cidx], Tangents[pidx2], tvec);
                    scaleVec3(tvec,.5f,Tangents[vidx2]);
#if CLOTH_NORMALIZE_TANGENTS
                    //normalVec3(Tangents[vidx1]); // !!
                    //normalVec3(Tangents[vidx2]); // !!
#endif
#endif
                
#if CLOTH_SCALE_NORMALS
                    // Average the NormalScales
                    NormalScale[vidx1] = (NormalScale[cidx] + NormalScale[pidx1])*.5f;
                    NormalScale[vidx2] = (NormalScale[cidx] + NormalScale[pidx2])*.5f;
#endif

                    // Use the averaged Normals and NormalScales to push out the edge points
                    {
                        int pm2 = cidx;
                        F32 scale = LengthConstraints[numc1 + i*Width].RestLength*curvescale;
                        F32 scale1 = scale * NormalScale[pidx1];
                        F32 scale2 = scale * NormalScale[pidx2];

                        addVec3(CurPos[pidx1], CurPos[pm2], tvec);
                        scaleVec3(Normals[vidx1],scale1,nvec);
                        scaleAddVec3(tvec, .5f, nvec, CurPos[vidx1]);

                        addVec3(CurPos[pidx2], CurPos[pm2], tvec);
                        scaleVec3(Normals[vidx2],scale2,nvec);
                        scaleAddVec3(tvec, .5f, nvec, CurPos[vidx2]);
                    }
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
// HACK!
// if the render hardware does normalization, this hack won't work.
// Instead, use the NormalScale values in the shader.
void ClothDarkenConcavePoints(ClothRenderData *renderData)
{
#if CLOTH_SCALE_NORMALS
    
    Vec3 *Normals = renderData->Normals;
    F32 *NormalScale = renderData->NormalScale;
    
    if (renderData->commonData.SubLOD <= 2)
    {
        int i;
        int nump = ClothNumRenderedParticles(&renderData->commonData, renderData->commonData.SubLOD);
        for (i=0; i<nump; i++)
        {
            if (NormalScale[i] <= 0.0f) {
                F32 s = (1.0f+NormalScale[i]);
                s = s*s;
                scaleVec3(Normals[i],s,Normals[i]);
            }
        }
    }
#endif
}

//////////////////////////////////////////////////////////////////////////////
// Convenience functions

void ClothUpdatePhysics(Cloth *cloth, F32 dt)
{
    cloth->Time += dt;

    //PERFINFO_AUTO_START("ClothUpdatePhysics",1);
    if (!cloth->SkipUpdatePhysics)
    {
        if (cloth->commonData.SubLOD <= 2)
            ClothUpdateParticles(cloth, dt);
        else
            ClothFastUpdateParticles(cloth, dt);
    }
    //PERFINFO_AUTO_STOP();
    //PERFINFO_AUTO_START("ClothUpdateCollisions",1);
    ClothUpdateCollisions(cloth);
    //PERFINFO_AUTO_STOP();
    //PERFINFO_AUTO_START("ClothUpdateConstraints",1);
    ClothUpdateConstraints(cloth);
    //PERFINFO_AUTO_STOP();
}

void ClothCopyToRenderData(Cloth *cloth, Vec3 *hookNormals, CCTRWhat what)
{
    int i;
    if (what & CCTR_COPY_DATA) {
        cloth->renderData.commonData = *&cloth->commonData;
        memcpy(cloth->renderData.RenderPos, cloth->CurPos, sizeof(Vec3)*cloth->renderData.commonData.NumParticles);
    }
    if (what & CCTR_COPY_HOOKS) {
        if (!hookNormals && cloth->renderData.hookNormals) {
            CLOTH_FREE(cloth->renderData.hookNormals);
        } else if (hookNormals && !cloth->renderData.hookNormals) {
            // TODO: Is this the right number of hook normals?
            cloth->renderData.hookNormals = CLOTH_CALLOC(Vec3, cloth->commonData.MaxAttachments);
        }
        if (hookNormals) {
            memcpy(cloth->renderData.hookNormals, hookNormals, cloth->commonData.MaxAttachments*sizeof(Vec3));
        }
    }
    cloth->renderData.debug_cloth_backpointer = cloth;
}


void ClothUpdateDraw(ClothRenderData *renderData)
{
    //PERFINFO_AUTO_START("ClothUpdateNormals",1);
    ClothUpdateNormals(renderData);
    //PERFINFO_AUTO_STOP();
    //PERFINFO_AUTO_START("ClothUpdateIntermediate",1);
    ClothUpdateIntermediatePoints(renderData);
    ClothDarkenConcavePoints(renderData);
    //PERFINFO_AUTO_STOP();

}

//////////////////////////////////////////////////////////////////////////////
