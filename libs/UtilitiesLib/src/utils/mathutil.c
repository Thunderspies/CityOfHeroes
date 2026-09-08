#include "utilitieslib/utils/mathutil.h"
#include <time.h> //for rand seed
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef FULLDEBUG
#include "utilitieslib/assert/assert.h"
#endif

U32 oldControlState;

// Matrix memory layout
// XX XY XZ
// YX YY YZ
// ZX ZY ZZ
// PX PY PZ


const Mat4    unitmat        = {{ 1.0,0,0},{0, 1.0,0},{0,0, 1.0},{0,0,0}};
const Mat44    unitmat44    = {{ 1.0,0,0,0},{0, 1.0,0,0},{0,0, 1.0,0},{0,0,0,1}};
const Mat4    lrmat        = {{-1.0,0,0},{0, 1.0,0},{0,0,-1.0},{0.0,0.0,0.0}};
const Vec3    zerovec3    = { 0,0,0 };
const Vec3    onevec3        = { 1,1,1 };
const Vec3    unitvec3    = { 1,1,1 };
const Vec3    upvec3        = { 0,1,0 };
const Vec4    zerovec4    = { 0,0,0,0 };
const Vec4    onevec4        = { 1,1,1,1 };
const Vec4    unitvec4    = { 1,1,1,1 };

F32 floatseed = 1.0;
long holdrand = 1L;

F32 sintable[TRIG_TABLE_ENTRIES];
F32 costable[TRIG_TABLE_ENTRIES];


/* Function getRandom()
 *    Gets a random number.
 *
 * Returns:
 *    A random integer.
 *
 */
static int getRandom()
{
    static int seeded = 0;
    
    if(seeded== 0)
    {
        srand(_time32(NULL));
        seeded = 1;
    }
    
    return rand();
}

void initQuickTrig() 
{    
    int i;
    F32 rad;
    for( i = 0 ; i < TRIG_TABLE_ENTRIES ; i++)
    {
        rad = (F32)(TWOPI * ((F32)i / TRIG_TABLE_ENTRIES));
        mathutil_sincosf(rad, &(sintable[i]), &(costable[i]));
    }
}

// Does a linear interpolation between entries in sintable
F32 qsin(F32 theta)
{
    F32 interpParam;
    U32 thetaInt;
    
    // Lazy init
    if ( !costable[0])
        initQuickTrig();

    thetaInt = (U32)theta;
    interpParam = theta - (F32)thetaInt;

    return sintable[thetaInt%TRIG_TABLE_ENTRIES] * (1.0f-interpParam) 
        +  sintable[(thetaInt+1)%TRIG_TABLE_ENTRIES] * interpParam;
}
F32 qcos(F32 theta)
{
    F32 interpParam;
    U32 thetaInt;

    // Lazy init
    if ( !costable[0])
        initQuickTrig();

    thetaInt = (U32)theta;
    interpParam = theta - (F32)thetaInt;

    return costable[thetaInt%TRIG_TABLE_ENTRIES] * (1.0f-interpParam) 
        +  costable[(thetaInt+1)%TRIG_TABLE_ENTRIES] * interpParam;
}

void getQuickRandomPointOnCylinder(const Vec3 start_pos, F32 radius, F32 height, Vec3 final_pos )
{
    int r = qrand() % TRIG_TABLE_ENTRIES;
    final_pos[0] = start_pos[0] + (costable[r] * radius) ;
    final_pos[1] = start_pos[1] + (qufrand() * height);
    final_pos[2] = start_pos[2] + (sintable[r] * radius) ;
}

void getRandomPointOnLine(const Vec3 vStart, const Vec3 vEnd, Vec3 vResult)
{
    posInterp(qufrand(), vStart, vEnd, vResult);
}



/* Function getCappedRandom()
 *    Gets a random number that is smaller than the specified maximum.
 *
 * Parameters:
 *    max - a maximum cap for the returned random integer
 *
 * Returns:
 *    A random integer.
 *
 */
//interesting bug, if you change int maxq to int max, Intellisense goes crazy. 
int getCappedRandom(int maxq)
{
    if(0 == maxq)
        return 0;
#ifdef FULLDEBUG
    assert(maxq <= SHRT_MAX);
#endif
    return (int)(getRandom() % maxq);
}


void camLookAt(const Vec3 lookdir, Mat4 mat)
{
    Vec3 dv;

    dv[2] = 0;
    getVec3YP(lookdir,&dv[1],&dv[0]);
    createMat3RYP(mat,dv);
}

void mat3FromUpVector(const Vec3 upVec, Mat3 mat)
{
    // We need an orientation matrix that has a certain up vector,
    // but we just don't care about the X or Z vector (as long as it's orthonormal)
    copyVec3( upVec, mat[1] );
    zeroVec3( mat[0] );

    // This is just to insure that the x-axis vector is not colinear with the y-axis vector
    if ( mat[1][0] == 1.0f )
        mat[0][2] = 1.0f;
    else
        mat[0][0] = 1.0f;

    // Since a lot of this is zeros, we can optimize, but for now just make it work
    crossVec3( mat[0], mat[1], mat[2] );
    crossVec3( mat[1], mat[2], mat[0] ); 
}

float distance3( const Vec3 a, const Vec3 b )
{
    return (F32)sqrt(SQR(a[0] - b[0]) + SQR(a[1] - b[1]) + SQR(a[2] - b[2]));
}

float distance3XZ( const Vec3 a, const Vec3 b )
{
    return (F32)sqrt(SQR(a[0] - b[0]) + SQR(a[2] - b[2]));
}

float distance3SquaredXZ( const Vec3 a, const Vec3 b )
{
    return (F32)SQR(a[0] - b[0]) + SQR(a[2] - b[2]);
}

void getScale(const Mat3 mat, Vec3 scale )
{
    scale[0] = lengthVec3(mat[0]);
    scale[1] = lengthVec3(mat[1]);
    scale[2] = lengthVec3(mat[2]);
}

void extractScale(Mat3 mat,Vec3 scale)
{
    scale[0] = normalVec3(mat[0]);
    scale[1] = normalVec3(mat[1]);
    scale[2] = normalVec3(mat[2]);
}

// same as the above, but doesn't copy
void normalMat3(Mat3 mat)
{
    normalVec3(mat[0]);
    normalVec3(mat[1]);
    normalVec3(mat[2]);
}

F32 near_same_vec3_tol_squared = 0.001f;
void setNearSameVec3Tolerance(F32 tol)
{
    near_same_vec3_tol_squared = tol*tol;
}

int nearSameDVec2(const DVec2 a,const DVec2 b)
{
    int        i;

    for(i=0;i<2;i++)
        if (!nearSameDouble(a[i], b[i]))
            return 0;
    return 1;
}

void transposeMat3(Mat3 uv)
{
F32 tmp;
    
    tmp = uv[0][1];
    uv[0][1] = uv[1][0];
    uv[1][0] = tmp;

    tmp = uv[0][2];
    uv[0][2] = uv[2][0];
    uv[2][0] = tmp;

    tmp = uv[1][2];
    uv[1][2] = uv[2][1];
    uv[2][1] = tmp;
}

int invertMat3Copy(const Mat3 a, Mat3 b)  
{
F32        fDetInv;

    fDetInv    =    a[0][0] * ( a[1][1] * a[2][2] - a[1][2] * a[2][1] ) -
                a[0][1] * ( a[1][0] * a[2][2] - a[1][2] * a[2][0] ) +
                a[0][2] * ( a[1][0] * a[2][1] - a[1][1] * a[2][0] );

    if (fabs(fDetInv) < EPSILON)
        return 0;
    fDetInv = 1.f / fDetInv;

    b[0][0] =  ( a[1][1] * a[2][2] - a[1][2] * a[2][1] );
    b[0][1] =  ( a[0][1] * a[2][2] - a[0][2] * a[2][1] );
    b[0][2] =  ( a[0][1] * a[1][2] - a[0][2] * a[1][1] );

    b[0][0] *= fDetInv;
    b[0][1] *= -fDetInv;
    b[0][2] *= fDetInv;

    b[1][0] = -fDetInv * ( a[1][0] * a[2][2] - a[1][2] * a[2][0] );
    b[1][1] =  fDetInv * ( a[0][0] * a[2][2] - a[0][2] * a[2][0] );
    b[1][2] = -fDetInv * ( a[0][0] * a[1][2] - a[0][2] * a[1][0] );

    b[2][0] =  fDetInv * ( a[1][0] * a[2][1] - a[1][1] * a[2][0] );
    b[2][1] = -fDetInv * ( a[0][0] * a[2][1] - a[0][1] * a[2][0] );
    b[2][2] =  fDetInv * ( a[0][0] * a[1][1] - a[0][1] * a[1][0] );
  
  return 1;
}

void invertMat4Copy(const Mat4 mat,Mat4 inv)
{
Vec3    dv;

    invertMat3Copy(mat,inv);
    subVec3(zerovec3,mat[3],dv);
    mulVecMat3(dv,inv,inv[3]);
}

int invertMat44Copy(const Mat44 m, Mat44 r)
{
    float max, t, det, pivot, oneoverpivot;
    int i, j=0, k;
    Mat44 A;
    copyMat44(m, A);


    /*---------- forward elimination ----------*/

    identityMat44(r);

    det = 1.0f;
    for (i = 0; i < 4; i++)
    {
        /* eliminate in column i, below diag */
        max = -1.f;
        for (k = i; k < 4; k++)
        {
            /* find pivot for column i */
            t = A[i][k];
            if (fabs(t) > max)
            {
                max = fabs(t);
                j = k;
            }
        }

        if (max <= 0.f)
            return 0;         /* if no nonzero pivot, PUNT */

        if (j != i)
        {
            /* swap rows i and j */
            for (k = i; k < 4; k++)
            {
                t = A[k][i];
                A[k][i] = A[k][j];
                A[k][j] = t;
            }
            for (k = 0; k < 4; k++)
            {
                t = r[k][i];
                r[k][i] = r[k][j];
                r[k][j] = t;
            }
            det = -det;
        }
        pivot = A[i][i];
        oneoverpivot = 1.f / pivot;
        det *= pivot;
        for (k = i + 1; k < 4; k++)           /* only do elems to right of pivot */
            A[k][i] *= oneoverpivot;
        for (k = 0; k < 4; k++)
            r[k][i] *= oneoverpivot;

        /* we know that A(i, i) will be set to 1, so don't bother to do it */

        for (j = i + 1; j < 4; j++)
        {
            /* eliminate in rows below i */
            t = A[i][j];                /* we're gonna zero this guy */
            for (k = i + 1; k < 4; k++)       /* subtract scaled row i from row j */
                A[k][j] -= A[k][i] * t;   /* (ignore k<=i, we know they're 0) */
            for (k = 0; k < 4; k++)
                r[k][j] -= r[k][i] * t;   /* (ignore k<=i, we know they're 0) */
        }
    }

    /*---------- backward elimination ----------*/

    for (i = 4 - 1; i > 0; i--)
    {
        /* eliminate in column i, above diag */
        for (j = 0; j < i; j++)
        {
            /* eliminate in rows above i */
            t = A[i][j];                /* we're gonna zero this guy */
            for (k = 0; k < 4; k++)         /* subtract scaled row i from row j */
                r[k][j] -= r[k][i] * t;   /* (ignore k<=i, we know they're 0) */
        }
    }

    if (det < 1e-8 && det > -1e-8)
        return 0;

    return 1;
}


void transposeMat3Copy(const Mat3 uv,Mat3 uv2)
{
    uv2[0][0] = uv[0][0];
    uv2[0][1] = uv[1][0];
    uv2[0][2] = uv[2][0];

    uv2[1][0] = uv[0][1];
    uv2[1][1] = uv[1][1];
    uv2[1][2] = uv[2][1];

    uv2[2][0] = uv[0][2];
    uv2[2][1] = uv[1][2];
    uv2[2][2] = uv[2][2];
}

void transposeMat44(Mat44 mat)
{
    F32 tempf;
#define SWAP(f1, f2) tempf = f1; f1 = f2; f2 = tempf;
    SWAP(mat[0][1], mat[1][0]);
    SWAP(mat[0][2], mat[2][0]);
    SWAP(mat[0][3], mat[3][0]);
    SWAP(mat[1][2], mat[2][1]);
    SWAP(mat[1][3], mat[3][1]);
    SWAP(mat[2][3], mat[3][2]);
#undef SWAP
}

void transposeMat44Copy(const Mat44 mIn, Mat44 mOut)
{
    mOut[0][0] = mIn[0][0];
    mOut[0][1] = mIn[1][0];
    mOut[0][2] = mIn[2][0];
    mOut[0][3] = mIn[3][0];

    mOut[1][0] = mIn[0][1];
    mOut[1][1] = mIn[1][1];
    mOut[1][2] = mIn[2][1];
    mOut[1][3] = mIn[3][1];

    mOut[2][0] = mIn[0][2];
    mOut[2][1] = mIn[1][2];
    mOut[2][2] = mIn[2][2];
    mOut[2][3] = mIn[3][2];

    mOut[3][0] = mIn[0][3];
    mOut[3][1] = mIn[1][3];
    mOut[3][2] = mIn[2][3];
    mOut[3][3] = mIn[3][3];
}


void scaleMat3(const Mat3 a,Mat3 b,F32 sfactor)
{
F32        t0, t1, t2;
int        i;

    for (i=0;i < 3;++i) 
    {
        t0 = a[i][0]*sfactor;
        t1 = a[i][1]*sfactor;
        t2 = a[i][2]*sfactor;
        b[i][0] = t0;
        b[i][1] = t1;
        b[i][2] = t2;
    }
}

//scale a Mat3 into another Mat3
void scaleMat3Vec3Xfer(const Mat3 a, const Vec3 sfactor, Mat3 b)
{
//int        i;

    copyMat3( a, b );
    scaleMat3Vec3(b,sfactor); 
    
    /*for (i=0;i < 3;++i) 
    {
        b[i][0] = a[i][0]*sfactor[0];
        b[i][1] = a[i][1]*sfactor[1];
        b[i][2] = a[i][2]*sfactor[2];
    }*/
}


// Note This is wrong, it's scaling the rows instead of the columns..
void scaleMat3Vec3(Mat3 a, const Vec3 sfactor)
{
int        i;

    for (i=0;i < 3;++i) 
    {
        a[i][0] = a[i][0]*sfactor[0];
        a[i][1] = a[i][1]*sfactor[1];
        a[i][2] = a[i][2]*sfactor[2];
    }
}

// Please use this one until we can branch FIXMEAFTERBRANCH
void scaleMat3Vec3Correct(Mat3 a, const Vec3 sfactor)
{
    scaleVec3(a[0], sfactor[0], a[0]);
    scaleVec3(a[1], sfactor[1], a[1]);
    scaleVec3(a[2], sfactor[2], a[2]);
}

void rotateMat3(const F32 *rpy, Mat3 uvs)
{
    if(rpy[1])
        yawMat3(rpy[1],uvs);

    if(rpy[0])
        pitchMat3(rpy[0],uvs);

    if(rpy[2])
        rollMat3(rpy[2],uvs);
}


void yawMat3World(F32 angle, Mat3 uv )
{
    F32 ut,sint,cost;
    int i;

    mathutil_sincosf(angle, &sint, &cost);

    for(i=0;i<3;i++)
    {
        ut     = uv[i][0]*cost - uv[i][2]*sint;
        uv[i][2] = uv[i][2]*cost + uv[i][0]*sint;
        uv[i][0] = ut; 
    }
}

void pitchMat3World(F32 angle, Mat3 uv)
{
F32 ut,sint,cost;
int i; 

    mathutil_sincosf(angle, &sint, &cost);
    for(i=0;i<3;i++)
    { 
        ut     = uv[i][1]*cost - uv[i][2]*sint;
        uv[i][2] = uv[i][2]*cost + uv[i][1]*sint;
        uv[i][1] = ut; 
    }
} 


void rollMat3World(F32 angle, Mat3 uv)
{
F32 ut,sint,cost;
int i;

    mathutil_sincosf(angle, &sint, &cost);
    for(i=0;i<3;i++)
    { 
        ut     = uv[i][0]*cost - uv[i][1]*sint;
        uv[i][1] = uv[i][1]*cost + uv[i][0]*sint;
        uv[i][0] = ut; 
    }
} 


void yawMat3(F32 angle, Mat3 uv)
{
F32 ut,sint,cost;
int i; 

    mathutil_sincosf(angle, &sint, &cost);
    for(i=0;i<3;i++)
    { 
        ut     = uv[0][i]*cost - uv[2][i]*sint;
        uv[2][i] = uv[2][i]*cost + uv[0][i]*sint;
        uv[0][i] = ut; 
    }
}


void pitchMat3(F32 angle, Mat3 uv)
{
F32 ut,sint,cost;
int i; 

    mathutil_sincosf(angle, &sint, &cost);
    for(i=0;i<3;i++)
    { 
        ut     = uv[1][i]*cost - uv[2][i]*sint;
        uv[2][i] = uv[2][i]*cost + uv[1][i]*sint;
        uv[1][i] = ut; 
    }
} 


void rollMat3(F32 angle, Mat3 uv)
{
F32 ut,sint,cost;
int i;

    mathutil_sincosf(angle, &sint, &cost);
    for(i=0;i<3;i++) { 
        ut     = uv[0][i]*cost - uv[1][i]*sint;
        uv[1][i] = uv[1][i]*cost + uv[0][i]*sint;
        uv[0][i] = ut; 
    }
} 


// Multiply a vector times a transposed matrix. Normally this implies the input
// matrix contains only rotatations, so the transpose is the same as the inverse.
void mulVecMat3Transpose( const Vec3 uvec, const Mat3 uvs,Vec3 bodvec)
{
    bodvec[0] = uvec[0]*uvs[0][0] + uvec[1]*uvs[0][1] + uvec[2]*uvs[0][2];
    bodvec[1] = uvec[0]*uvs[1][0] + uvec[1]*uvs[1][1] + uvec[2]*uvs[1][2];
    bodvec[2] = uvec[0]*uvs[2][0] + uvec[1]*uvs[2][1] + uvec[2]*uvs[2][2];
}


// Multiply a vector times a transposed matrix
void mulVecMat4Transpose(const Vec3 vin, const Mat4 m, Vec3 vout)
{
Vec3    dv;

    subVec3(vin,m[3],dv);
    mulVecMat3Transpose(dv,m,vout);
}

#if 0
#include "xmmintrin.h"
// some sse intrinsic experiments

__m128    t,t2;

    t2.m128_f32[0] = 1;
    t2.m128_f32[1] = 1;
    t2.m128_f32[2] = 1;
    t2.m128_f32[3] = 1;
    t2 = _mm_add_ps(t2,t2);

    t.m128_f32[0] = mag2;
    t2 = _mm_rsqrt_ss(t);

#endif

void mulMat3(const Mat3 a, const Mat3 b, Mat3 c)
{
    mulMat3Inline(a, b, c);
}

void mulMat4(const Mat4 a, const Mat4 b, Mat4 c)
{
    mulMat4Inline(a, b, c);
}

void mulMat44(const Mat44 a, const Mat44 b, Mat44 c)
{
    mulMat44Inline(a, b, c);
}

F32 normalVec3XZ(Vec3 v)
{
F32        mag2,mag,invmag;

    // Is there some way to let the compiler know to generate an atomic inverse
    // square root if the cpu supports it? (ie; sse, 3dnow, r10k, etc.)
    mag2 = SQR(v[0]) + SQR(v[2]);
    mag = fsqrt(mag2);

    if (mag > 0)
    {
        invmag = 1.f/mag;
        scaleVec3XZ(v,invmag,v);
    }

    return(mag);
}

F32 normalVec2(Vec2 v)
{
    F32        mag2,mag,invmag;

    // Is there some way to let the compiler know to generate an atomic inverse
    // square root if the cpu supports it? (ie; sse, 3dnow, r10k, etc.)
    mag2 = SQR(v[0]) + SQR(v[1]);
    mag = fsqrt(mag2);

    if (mag > 0)
    {
        invmag = 1.f/mag;
        scaleVec2(v,invmag,v);
    }

    return(mag);
}


double normalDVec2(DVec2 v)
{
    double        mag2,mag,invmag;

    // Is there some way to let the compiler know to generate an atomic inverse
    // square root if the cpu supports it? (ie; sse, 3dnow, r10k, etc.)
    mag2 = SQR(v[0]) + SQR(v[1]);
    mag = sqrt(mag2);

    if (mag > 0)
    {
        invmag = 1.0/mag;
        scaleVec2(v,invmag,v);
    }

    return(mag);
}

F32    det3Vec3(const Vec3 v0,const Vec3 v1,const Vec3 v2)
{
F32        a,b,c,d,e,f;

    a = v0[0] * v1[1] * v2[2];
    b = v0[1] * v1[2] * v2[0];
    c = v0[2] * v1[0] * v2[1];
    d = v2[0] * v1[1] * v0[2];
    e = v2[1] * v1[2] * v0[0];
    f = v2[2] * v1[0] * v0[1];
    return(a + b + c - d - e - f);
}

// Gets yaw with respect to positive z.
void getVec3YP(const Vec3 dvec,F32 *yawp,F32 *pitp)
{
F32        dist;
    
    *yawp = (F32)fatan2(dvec[0],dvec[2]);
    dist = fsqrt(SQR(dvec[0]) + SQR(dvec[2]));
    *pitp = (F32)fatan2(dvec[1],dist);
}

// Gets yaw with respect to negative z.
void getVec3YvecOut(const Vec3 dvec,F32 *yawp,F32 *pitp)
{
F32        dist;
    
    *yawp = (F32)fatan2(-dvec[0], -dvec[2]);
    dist = fsqrt(SQR(dvec[0]) + SQR(dvec[2]));
    *pitp = (F32)fatan2(dvec[1],dist);
}

void getVec3PY(const Vec3 dvec,F32 *pitp,F32 *yawp)
{
F32        dist;
    
    *pitp = (F32)fatan2(dvec[1],dvec[2]);
    dist = fsqrt(SQR(dvec[1]) + SQR(dvec[2]));
    *yawp = (F32)fatan2(dvec[0],dist);
}

F32 interpAngle(  F32 percent_a, F32 a, F32 b )
{
    return a + ( subAngle( b, a ) * percent_a );
}

void interpPYR( F32 percent_a, const Vec3 a, const Vec3 b, Vec3 result )
{
    result[0] = interpAngle( percent_a, a[0], b[0] );
    result[1] = interpAngle( percent_a, a[1], b[1] );
    result[2] = interpAngle( percent_a, a[2], b[2] );
}

F32 fixAngle(F32 a)
{
    if(a > RAD(180.0))
        a -= RAD(360.0);
    if(a <= RAD(-180.0))
        a += RAD(360.0);
    return(a);
}

F32 subAngle(F32 a, F32 b)
{
    return(fixAngle(a-b));
}

F32 addAngle(F32 a, F32 b)
{
    return(fixAngle(a+b));
}

void createMat3PYR(Mat3 mat,const F32 *pyr)
{
    F32 cosP,sinP,cosY,sinY,cosR,sinR,temp;

    // Matricies in the game are transposed, so the pitch, yaw, and roll matrices
    // are also transposed. An easy way to do this is just to change the sign of sin
    mathutil_sincosf(pyr[0], &sinP, &cosP);
    mathutil_sincosf(pyr[1], &sinY, &cosY);
    mathutil_sincosf(pyr[2], &sinR, &cosR);


    mat[0][0] =     cosY * cosR;
    mat[1][0] =     cosY * sinR;
    mat[2][0] =     sinY;

    temp =           sinP * sinY;
    mat[0][1] = -temp * cosR - cosP * sinR;
    mat[1][1] =  cosP * cosR - temp * sinR;
    mat[2][1] =  sinP * cosY;

    temp =           cosP * -sinY;
    mat[0][2] =  temp * cosR + sinP * sinR;
    mat[1][2] =  temp * sinR - sinP * cosR;
    mat[2][2] =  cosP * cosY;
}





// Note that the parameters are still pyr not ryp
void createMat3RYP(Mat3 mat,const F32 *pyr)
{
    F32 cosP,sinP,cosY,sinY,cosR,sinR,temp;

    mathutil_sincosf(pyr[0], &sinP, &cosP);
    mathutil_sincosf(pyr[1], &sinY, &cosY);
    mathutil_sincosf(pyr[2], &sinR, &cosR);

    temp =           cosR * sinY;
    mat[0][0] =     cosR * cosY;
    mat[1][0] =     sinR * cosP - temp * sinP;
    mat[2][0] =     sinR * sinP + temp * cosP;
    
    temp =          -sinR * sinY;
    mat[0][1] =  -sinR * cosY;
    mat[1][1] =  cosR * cosP - temp * sinP;
    mat[2][1] = cosR * sinP + temp * cosP;

    mat[0][2] = -sinY;
    mat[1][2] = cosY * -sinP;
    mat[2][2] = cosY * cosP;
}

// Note that the parameters are still pyr not ryp
void createMat3YPR(Mat3 mat,const F32 *pyr)
{
    F32 cosP,sinP,cosY,sinY,cosR,sinR,temp;

    mathutil_sincosf(pyr[0], &sinP, &cosP);
    mathutil_sincosf(pyr[1], &sinY, &cosY);
    mathutil_sincosf(pyr[2], &sinR, &cosR);

    temp =           sinY * sinP;
    mat[0][0] =  cosY * cosR + temp * sinR;
    mat[1][0] =  cosY * sinR - temp * cosR;
    mat[2][0] =  sinY * cosP;

    mat[0][1] =    -cosP * sinR;
    mat[1][1] =     cosP * cosR;
    mat[2][1] =     sinP;

    temp =          -cosY * sinP;
    mat[0][2] = -sinY * cosR - temp * sinR;
    mat[1][2] = -sinY * sinR + temp * cosR;
    mat[2][2] =  cosY * cosP;
}

// Note that the parameters are still pyr not ryp
void createMat3_0_YPR(Vec3 mat0,const F32 *pyr)
{
    F32 cosP,sinP,cosY,sinY,cosR,sinR;

    mathutil_sincosf(pyr[0], &sinP, &cosP);
    mathutil_sincosf(pyr[1], &sinY, &cosY);
    mathutil_sincosf(pyr[2], &sinR, &cosR);

    mat0[0] =  cosY * cosR + sinY * sinP * sinR;
    mat0[1] = -cosP * sinR;
    mat0[2] = -sinY * cosR + cosY * sinP * sinR;
}

// Note that the parameters are still pyr not ryp
void createMat3_1_YPR(Vec3 mat1,const F32 *pyr)
{
    F32 cosP,sinP,cosY,sinY,cosR,sinR;

    mathutil_sincosf(pyr[0], &sinP, &cosP);
    mathutil_sincosf(pyr[1], &sinY, &cosY);
    mathutil_sincosf(pyr[2], &sinR, &cosR);

    mat1[0] =  cosY * sinR - sinY * sinP * cosR;
    mat1[1] =  cosP * cosR;
    mat1[2] = -sinY * sinR - cosY * sinP * cosR;
}

// Note that the parameters are still pyr not ryp
void createMat3_2_YPR(Vec3 mat2,const F32 *pyr)
{
    F32 cosP,sinP,cosY,sinY;

    mathutil_sincosf(pyr[0], &sinP, &cosP);
    mathutil_sincosf(pyr[1], &sinY, &cosY);

    mat2[0] =  sinY * cosP;
    mat2[1] =  sinP;
    mat2[2] = cosY * cosP;
}

void getMat3PYR(const Mat3 mat,F32 *pyr)
{
    F32 R,P,Y;
    F32 cosY,cosP;

    if (NEARZERO(1.0 - fabs(mat[0][2])))    // Special case: Y = +- 90, R = 0
    {
        P = (F32)fatan2(mat[2][1],mat[1][1]);
        Y = (F32)(mat[0][2] > 0 ? -RAD(90.0) : RAD(90.0));
        R = 0;
    }
    else
    {
        P = (F32)fatan2(-mat[1][2],mat[2][2]);
        cosP = (F32)fcos(P);
        if (cosP == 0.0) {
            if (P > 0.0) {
                Y = (F32)fatan2(-mat[0][2], -mat[1][2]);
                R = (F32)fatan2(-mat[2][0], -mat[2][1]);
            } else {
                Y = (F32)fatan2(-mat[0][2], mat[1][2]);
                R = (F32)fatan2( mat[2][0], mat[2][1]);
            }
        } else {
            cosY = mat[2][2] / cosP;
            Y = (F32)fatan2(-mat[0][2],cosY);
            R = (F32)fatan2(-mat[0][1] / cosY, mat[0][0] / cosY);
        }
    }
    pyr[0] = P;
    pyr[1] = Y;
    pyr[2] = R;
}


void getMat3YPR(const Mat3 mat,F32 *pyr)
{
    F64 R,P,Y;
    F64 cosP,cosR;

    if (NEARZERO(1.0 - fabs(mat[2][1])))    // Special case: P = +- 90, R = 0
    {
        Y = atan2(-mat[0][2],mat[0][0]);
        P = mat[2][1] > 0 ? RAD(90.0) : -RAD(90.0);
        R = 0;
    }
    else
    {
        R = atan2(-mat[0][1],mat[1][1]);
        cosR = cos(R);
        if (cosR == 0.0) {
            if (R > 0.0) {
                P = atan2( mat[2][1],-mat[0][1]);
                Y = atan2( mat[1][2],-mat[1][0]);
            } else {
                P = atan2( mat[2][1], mat[0][1]);
                Y = atan2(-mat[1][2], mat[1][0]);
            }
        } else {
            cosP = mat[1][1] / cosR;
            P = atan2( mat[2][1],cosP);
            Y = atan2( mat[2][0] / cosP,mat[2][2] / cosP);
        }
    }
    if (P < -16.f)
        P = 0;
    if (Y < -16.f)
        Y = 0;
    if (R < -16.f)
        R = 0;
    pyr[0] = (F32)P;
    pyr[1] = (F32)Y;
    pyr[2] = (F32)R;
}



void getRandomPointOnSphereSurface(F32* theta, F32* phi )
{
    *theta = (F32)(qufrand() * TWOPI);
    *phi = acosf(2 * qufrand() - 1.0f);
}

void getRandomPointOnLimitedSphereSurface(F32* theta, F32* phi, F32 phiDeflectionFromNorthPole)
{
    *theta = (F32)(qufrand() * TWOPI);
    *phi = acosf(2 * (1.0f - qufrand() * (phiDeflectionFromNorthPole / PI)) - 1.0f);
}

void getRandomAngularDeflection(Vec3 vOutput, F32 fAngle)
{
    // Generates a unit vector deflected from (0,1,0) by a uniform distribution across the angle specified (phi), and 2pi(theta) in spherical coordinates
    F32 theta, phi;
    getRandomPointOnLimitedSphereSurface(&theta, &phi, fAngle);

    sphericalCoordsToVec3(vOutput, theta, phi, 1.0f);
}

void sphericalCoordsToVec3(Vec3 vOut, F32 theta, F32 phi, F32 radius)
{
    F32 fSinPhi, fCosPhi, fSinTheta, fCosTheta;
    mathutil_sincosf(phi, &fSinPhi, &fCosPhi);
    mathutil_sincosf(theta, &fSinTheta, &fCosTheta);
    vOut[1] = radius * fCosTheta * fSinPhi;
    vOut[0] = radius * fSinTheta * fSinPhi;
    vOut[2] = radius * fCosPhi;
}


void posInterp(F32 t, const Vec3 T0, const Vec3 T1, Vec3 T)
{
    F32 vec[3];

    subVec3(T1, T0, vec);
    scaleVec3(vec, t, vec);
    addVec3(T0, vec, T);
}

int planeIntersect(const Vec3 start,const Vec3 end,const Mat4 pmat,Vec3 cpos)
{
Vec3    tpA,tpB,dv;
F32        v1x,v1z,d,dist,da,tpx,tpz;

    //subVec3(start,pmat[3],dv);
    mulVecMat4Transpose(start,pmat,tpA);

    //subVec3(end,pmat[3],dv);
    mulVecMat4Transpose(end,pmat,tpB);

    // Are points on opposite sides of the plane
    if((tpA[1] > 0.0) && (tpB[1] > 0.0))
        return 0;
    else if((tpA[1] < 0.0) && (tpB[1] < 0.0))
        return 0;

    v1x = tpB[0]-tpA[0];
    v1z = tpB[2]-tpA[2];
    d = fsqrt(SQR(v1x) + SQR(v1z));
    if(d > 0.01) /* pA != pB */
    {
        /** Find the distance from tpA to the collision position - similar triangles **/
        dist = fabsf(tpA[1]) + fabsf(tpB[1]);
        if (dist == 0.0F) 
            da = (F32)((fabsf(tpA[1])) * 1000.0);
        else
            da = (F32)((fabsf(tpA[1])) / dist);
        /* v1 does not need to be normalized because it gets multiplied back by d in da */
        /***** Find the collision position on the plane *****/
        tpx = tpA[0] + v1x*da;
        tpz = tpA[2] + v1z*da;
    } else /* pA = pB */ {
        tpx = tpA[0];
        tpz = tpA[2];
    }
    dv[0] = tpx;
    dv[1] = 0;
    dv[2] = tpz;
    mulVecMat4(dv,pmat,cpos);
    return 1;
}




#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/fpmacros.h"
#include <float.h>

int randInt(int max)
{
#define A            (48271)
#define RAND_10K    (399268537)        /* value of seed at ten-thousandth iteration */
#define M            (2147483647)    /* 2**31-1 */
static U32    seed_less_one;
U32            hprod,lprod,result,seed = seed_less_one + 1;

    if (max <= 0) return 0;
    /* first do two 16x16 multiplies */
    hprod = ((seed>>16)&0xFFFF) * A;
    lprod = (seed&0xFFFF)*A;

    /* combine the products (suitably shifted) to form 48-bit product,
    *  with bottom 32 bits in lprod and top 16 bits in hprod.
    */
    hprod += ((lprod>>16) & 0xFFFF);
    lprod = (lprod&0xFFFF)|((hprod&0xFFFF)<<16);
    hprod >>= 16;

    /* now subtract the top 17 bits from the bottom 31 bits to implement
    *  a deferred "end-around carry".
    */
    hprod = hprod + hprod + ((lprod>>31)&1);
    lprod += hprod;

    /* final "and" gives modulo(2^31-1) */
    seed = lprod & 0x7FFFFFFF;
    result = (seed_less_one = seed - 1);

    return result % max;
}

U64 randU64(U64 max)
{
    U64 result = ((U64)randInt(INT_MAX)) ^ (((U64)randInt(INT_MAX))<<22) ^ (((U64)randInt(INT_MAX))<<44);
    return result % max;
}

int log2i(int val)
{
    int i;

    for(i=0;i<32;i++)
    {
        if ((1 << i) >= val)
            return i;
    }
    return -1;
}

int pow2(int val)
{
    return 1 << log2i(val);
}


int randUIntGivenSeed(int * rand)
{
    return(((*rand = *rand * 214013L + 2531011L) >> 16) & 0x7fff);
}

int randIntGivenSeed(int * rand)
{
    return(((*rand = *rand * 214013L + 2531011L) >> 16) & 0xffff);
}

//mw a work in progress
F32 quick_fsqrt( F32 x )
{
    // the other version of this function didnt seem to work
    // at all after about 9, so i added one that does work,
    // but with a 5% margin of error, so the function can
    // be used while the other is being worked on
    // -bh
    U32 answer;
    answer = (((*(U32*)(&x)) - 0x3F800000) >> 1) + 0x3F800000;
    return *(F32*)(&(answer));


    // this is the original
    //return (2.0 + (0.25 * (x - 4) - (0.015625 * (x - 4) * (x - 4)) ));
}



//TO DO describe this thing
//this is really a math function.
F32    circleDelta( F32 org, F32 dest )
{
#define    RAD_NORM( a ) while( a < 0 ) a += ( PI * 2.f ); while( a > ( PI * 2.f ) ) a -= ( PI * 2.f );

    F32    delta;

    RAD_NORM( org );
    RAD_NORM( dest );

    if( org < dest )
    {
        delta = dest - org;

        if( delta <= PI )
            return delta;

        return (F32)(-( ( PI * 2.f ) - dest ) + org);
    }
    
    delta = org - dest;

    if( delta <= PI )
        return -delta;
    
    return (F32)(( ( PI * 2.f ) - org ) + dest);
}


//this is really a math function.
void circleDeltaVec3(const Vec3 org, const Vec3 dest, Vec3 delta)
{
    int    i;

    for( i = 0; i < 3; i++ )
        delta[i] = circleDelta( org[i], dest[i] );
}


int finiteVec3(const Vec3 y)
{
    return ( (FINITE((y)[0])) && (FINITE((y)[1])) && (FINITE((y)[2])) );
}

int finiteVec4(const Vec4 y)
{
    return ( (FINITE((y)[0])) && (FINITE((y)[1])) && (FINITE((y)[2])) && (FINITE((y)[3])) );
}

//Looks like it'll work for "squished" spheres too.  If you just assume rx = ry = rz = the radius, 
//it works for a normal sphere =).  I use http://astronomy.swin.edu.au/~pbourke/geometry/ for all my 
//intersection math and things like that, it's a good site =).
//(DefTracker *tracker,CollInfo *coll)
    /*sphere_radius = tracker->radius;
    line_len = coll->line_len;
    sphere_mid = tracker->mid;
    line_start = coll->start;
    line_dir   = coll->dir;*/
int sphereLineCollision( F32 sphere_radius, const Vec3 sphere_mid, const Vec3 line_start, const Vec3 line_end )
{
    F32        d,rad,t,line_len;
    Vec3    dv,dv_vecIn,dv_ln,line_pos,line_dir;
    
    subVec3( line_start, line_end, line_dir );
    line_len = normalVec3( line_dir );

    // First do a quick sphere-sphere test. Since a lot of the lines are short this discards a bunch
    rad = sphere_radius + line_len;
    subVec3(sphere_mid, line_start, dv_vecIn);
    t = line_len + rad;
    if (lengthVec3Squared(dv_vecIn) > t*t)
        return 0;

    // Get dot product of point and line seg
    // Is the sphere off either end of the line?
    d = dotVec3(dv_vecIn,line_dir);
    if (d < -rad || d > line_len + rad)
        return 0;

    // Get point on line closest to given point
    scaleVec3(line_dir,d,dv_ln);
    addVec3(dv_ln,line_start,line_pos);

    // How far apart are they?
    subVec3(line_pos,sphere_mid,dv);
    d = lengthVec3Squared(dv);

    return d < rad*rad;
}

static U32 rule30_c = 0xf244f343;

void rule30Seed(U32 c)
{
    rule30_c = c;
}

U32 rule30Rand()
{
      U32 l,r;
      l = r = rule30_c;
      l = _lrotr(l, 1);/* bit i of l equals bit just left of bit i in c */
      r = _lrotl(r, 1);/* bit i of r euqals bit just right of bit i in c */
      rule30_c |= r;
      rule30_c ^= l;           /* c = l xor (c or r), aka rule 30, named by wolfram */
      return rule30_c;
}

char *safe_ftoa_s(F32 f,char *buf, size_t buf_size)
{
    char    *s;

    if (!_finite(f))
        f = 0;
    sprintf_s(buf, buf_size, "%f",f);
    for(s=(buf + strlen(buf) -1);*s == '0';s--)
        ;
    s[1] = 0;
    if (s[0] == '.')
        *s = 0;
    if (buf[0] == 0)
        buf[0] = '0';
    return buf;
}

/* Function graphInterp
*    Give a set of x, y pairs and an x, return the appropriate y value using
*    linear interpolation beteween points.  The give x, y pairs must be
*    sorted in decreasing value on x.
*    
*    First the function determines which two points are appropriate for
*    for use for interpolation using a linear search.  It compares the given
*    x value with all graph point x values.  Note that this function does no 
*    bounds checking on the given x value.  It is assumed that the graph points 
*    define the range of all possible x values.
*
*/

float graphInterp(float x, const Vec3* interpPoints){
    int interpPointCursor = 0;
    float interpStrength;
    Vec3 interpResult;

    // Decide which interpolation points to use
    //    When the loop is done, we've found interp points, interpPointCursor - 1 and interpPointCursor,
    //    to use as proper interpolation points.
    //  Due to the way the interpPoints are assumed to be sorted, interpPointCursor - 1 should always
    //    index the x, y pair with the greater x value.
    while(x < interpPoints[interpPointCursor][0])
        interpPointCursor++;

    interpStrength = (x - interpPoints[interpPointCursor][0])/
        (interpPoints[interpPointCursor - 1][0] - interpPoints[interpPointCursor][0]);

    posInterp(interpStrength, interpPoints[interpPointCursor], interpPoints[interpPointCursor - 1], interpResult);
    return interpResult[1];
}


bool pointBoxCollision( const Vec3 point, const Vec3 min, const Vec3 max )
{
    int i;
    for( i = 0; i < 3; i++ )
    {
        if( point[i] < min[i] || point[i] > max[i] )
            return false;
    }
    return true;
}

bool lineBoxCollision( const Vec3 start, const Vec3 end, const Vec3 min, const Vec3 max, Vec3 intersect )
{
    int i, found[3] = {0};
    bool inside = true, whichplane = 0;
    float dist;
    Vec3 dir, MaxT;

    MaxT[0]=MaxT[1]=MaxT[2]=-1.0f;
    dist = distance3( start, end );

    setVec3( dir, (end[0]-start[0])/dist, (end[1]-start[1])/dist, (end[2]-start[2])/dist );

    // Find candidate planes.
    for(i=0;i<3;i++)
    {
        if(start[i] < min[i])
        {
            intersect[i] = min[i];
            inside = false;

            // Calculate T distances to candidate planes
            MaxT[i] = (min[i] - start[i]) / dir[i];
            found[i] = true;
        }
        else if(start[i] > max[i])
        {
            intersect[i] = max[i];
            inside = false;

            // Calculate T distances to candidate planes
            MaxT[i] = (max[i] - start[i]) / dir[i];
            found[i] = true;
        }
    }

    // Ray origin inside bounding box
    if(inside)
    {
        copyVec3(start, intersect);
        return true;
    }

    // Get largest of the maxT's for final choice of intersection
    if(MaxT[1] > MaxT[whichplane])    whichplane = 1;
    if(MaxT[2] > MaxT[whichplane])    whichplane = 2;

    if(!found[whichplane])
        return false;

    // Check final candidate actually inside box
    for(i=0;i<3;i++)
    {
        if(i!=whichplane)
        {
            intersect[i] = start[i] + MaxT[whichplane] * dir[i];
            if(intersect[i] < min[i] || intersect[i] > max[i])
                return false;
        }
    }
    return true;    // ray hits box
}

bool boxBoxCollision( const Vec3 min1, const Vec3 max1, const Vec3 min2, const Vec3 max2 )
{
    int i;
    for( i = 0; i < 3; i++ )
    {
        if( max1[i] < min2[i] || max2[i] < min1[i] )
            return false;
    }
    return true;
}

void calcTransformVectors(const Vec3 pos[3], const Vec2 uv[3], Vec3 utransform, Vec3 vtransform)
{
    Vec3 v01, v02;
    Vec2 uv01, uv02;
    Vec2 inv0, inv1;

    utransform[0] = utransform[1] = utransform[2] = 0;
    vtransform[0] = vtransform[1] = vtransform[2] = 0;

    subVec3(pos[1], pos[0], v01);
    subVec3(pos[2], pos[0], v02);
    subVec2(uv[1], uv[0], uv01);
    subVec2(uv[2], uv[0], uv02);

    // |d1| = |u1 v1| * |Tu|
    // |d2|   |u2 v2|   |Tv|
    if (!invertMat2(uv01[0], uv02[0], uv01[1], uv02[1], inv0, inv1))
        return;

    // solve for Tu and Tv for each dx,dy,dz
    mulVecMat2_special(inv0, inv1, v01[0], v02[0], &utransform[0], &vtransform[0]);
    mulVecMat2_special(inv0, inv1, v01[1], v02[1], &utransform[1], &vtransform[1]);
    mulVecMat2_special(inv0, inv1, v01[2], v02[2], &utransform[2], &vtransform[2]);
}

void calcTransformVectorsAccurate(const Vec3 pos[3], const Vec2 uv[3], Vec3 utransform, Vec3 vtransform)
{
    DVec3 v01, v02;
    DVec2 uv01, uv02;
    DVec2 inv0, inv1;
    double t0, t1;

    utransform[0] = utransform[1] = utransform[2] = 0;
    vtransform[0] = vtransform[1] = vtransform[2] = 0;

    subVec3(pos[1], pos[0], v01);
    subVec3(pos[2], pos[0], v02);
    subVec2(uv[1], uv[0], uv01);
    subVec2(uv[2], uv[0], uv02);

    // |d1| = |u1 v1| * |Tu|
    // |d2|   |u2 v2|   |Tv|
    if (!invertDMat2(uv01[0], uv02[0], uv01[1], uv02[1], inv0, inv1))
        return;

    // solve for Tu and Tv for each dx,dy,dz
    mulDVecMat2_special(inv0, inv1, v01[0], v02[0], &t0, &t1);
    utransform[0] = (F32)t0; vtransform[0] = (F32)t1;
    mulDVecMat2_special(inv0, inv1, v01[1], v02[1], &t0, &t1);
    utransform[1] = (F32)t0; vtransform[1] = (F32)t1;
    mulDVecMat2_special(inv0, inv1, v01[2], v02[2], &t0, &t1);
    utransform[2] = (F32)t0; vtransform[2] = (F32)t1;
}

#define NEAR_ONE 0.999999f

// This function sets the matrix's z axis to the given direction vector.
// Note that for camera matrices the view direction is the negative z axis,
// so you will need to pass in a negative direction vector.
void orientMat3(Mat3 mat, const Vec3 dir)
{
    Vec3 dirnorm;
    copyVec3(dir, dirnorm);
    normalVec3(dirnorm);

    if (dirnorm[1] >= NEAR_ONE)
    {
        setVec3(mat[0], 1.f, 0.f, 0.f);
        setVec3(mat[1], 0.f, 0.f,-1.f);
        setVec3(mat[2], 0.f, 1.f, 0.f);
    }
    else if (dirnorm[1] <= -NEAR_ONE)
    {
        setVec3(mat[0], 1.f, 0.f, 0.f);
        setVec3(mat[1], 0.f, 0.f, 1.f);
        setVec3(mat[2], 0.f,-1.f, 0.f);
    }
    else
    {
        Vec3 up,xuv,yuv;
        setVec3(up, 0.f,1.f,0.f);
        crossVec3(up,dirnorm,xuv);
        normalVec3(xuv);
        crossVec3(dirnorm,xuv,yuv);
        normalVec3(yuv);
        copyVec3(xuv, mat[0]);
        copyVec3(yuv, mat[1]);
        copyVec3(dirnorm, mat[2]);
    }
}

void orientMat3Yvec(Mat3 mat, const Vec3 dir, const Vec3 yvec)
{
    Vec3 up,xuv,yuv,dirnorm;

    copyVec3(dir, dirnorm);
    normalVec3(dirnorm);

    copyVec3(yvec, up);
    normalVec3(up);

    crossVec3(up,dirnorm,xuv);
    normalVec3(xuv);
    crossVec3(dirnorm,xuv,yuv);
    normalVec3(yuv);
    copyVec3(xuv, mat[0]);
    copyVec3(yuv, mat[1]);
    copyVec3(dirnorm, mat[2]);
}

F32 distanceToBoxSquared(const Vec3 boxmin, const Vec3 boxmax, const Vec3 point)
{
    float dist, d;

    if (point[0] < boxmin[0])
        d = point[0] - boxmin[0];
    else if (point[0] > boxmax[0] )
        d = point[0] - boxmax[0];
    else
        d = 0;
    dist = SQR(d);

    if (point[1] < boxmin[1])
        d = point[1] - boxmin[1];
    else if (point[1] > boxmax[1] )
        d = point[1] - boxmax[1];
    else
        d = 0;
    dist += SQR(d);

    if (point[2] < boxmin[2])
        d = point[2] - boxmin[2];
    else if (point[2] > boxmax[2] )
        d = point[2] - boxmax[2];
    else
        d = 0;
    dist += SQR(d);

    return dist;
}

