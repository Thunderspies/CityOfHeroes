
#include "utilitieslib/utils/Quat.h"
#include "utilitieslib/utils/mathutil.h"

Quat     unitquat = { 0,0,0,-1 };

#define EPSILON 0.00001f



//
// Quat creation
//
void mat3ToQuat(const Mat3 R, Quat q)
{
    F32        trace, s;


    int    i, j, k;


    int nxt[3] = {1, 2, 0};


    trace = R[0][0] + R[1][1] + R[2][2];


    // check the diagonal
    if (trace > 0.0)
    {
        s = (F32)sqrt (trace + 1.0);
        quatW(q) = (F32)(-s / 2.0);
        s = (F32)(0.5 / s);
        quatX(q) = (F32)((R[1][2] - R[2][1]) * s);
        quatY(q) = (F32)((R[2][0] - R[0][2]) * s);
        quatZ(q) = (F32)((R[0][1] - R[1][0]) * s);
    }
    else
    {    
        // diagonal is negative
        i = 0;
        if (R[1][1] > R[0][0]) i = 1;
        if (R[2][2] > R[i][i]) i = 2;
        j = nxt[i];
        k = nxt[j];


        s = (F32)sqrt ((R[i][i] - (R[j][j] + R[k][k])) + 1.0);

        q[i] = (F32)(s * 0.5);

        if (s != 0.0) s = (F32)(0.5 / s);


        q[3] = -(R[j][k] - R[k][j]) * s;
        q[j] = (R[i][j] + R[j][i]) * s;
        q[k] = (R[i][k] + R[k][i]) * s;
    }
}

void PYRToQuat(const Vec3 pyr, Quat q)
{
    // ST: This could be made faster, but I'm hoping to eventually not need this
    Mat3 mTempSolution;
    createMat3YPR( mTempSolution, pyr); // ypr is the order, correct?
    mat3ToQuat( mTempSolution, q );
}

void quatToAxisAngle(const Quat quat, Vec3 axis, F32* angle)
{
    if ( quatW(quat) > 0.9999f )
    {
        axis[0] = axis[2] = 0.0f;
        axis[1] = 1.0f;
        *angle = 0.0f;
        return;
    }
    else
    {
        F32 fOneOverS = 1.0f / sqrtf(1 - quatW(quat)*quatW(quat));
        *angle = 2.0f * acosf(quatW(quat));
        axis[0] = quatX(quat) * fOneOverS;
        axis[1] = quatY(quat) * fOneOverS;
        axis[2] = quatZ(quat) * fOneOverS;
    }
}

F32 quatGetAngle(const Quat quat)
{
    if ( quatW(quat) > 0.9999f)
        return 0.0f;
    return 2.0f * acosf(quatW(quat));
}


bool axisAngleToQuat(const Vec3 axis, F32 angle, Quat quat)
{
    F32 halfAngle,div, sha, cha;


    div = axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2];
    if(div > (EPSILON*EPSILON))
    {
        halfAngle = angle * 0.5f;
        mathutil_sincosf(halfAngle, &sha, &cha);
        div = (F32)((1.f / sqrt(div)) * sha);
        quatW(quat) = cha;
        quatX(quat) = axis[0] * div;
        quatY(quat) = axis[1] * div;
        quatZ(quat) = axis[2] * div;
        return true;
    }
    else
    {
        quatW(quat) = -1.0f;
        quatX(quat) = 0.0f;
        quatY(quat) = 0.0f;
        quatZ(quat) = 0.0f;
        return false;
    }
}

void yawQuat(F32 yaw, Quat q)
{
    mathutil_sincosf(yaw*0.5f, &quatY(q), &quatW(q) );
    //quatW(q) = cosf(yaw*0.5f);
    quatX(q) = 0.0f;
    //quatY(q) = sinf(yaw*0.5f);
    quatZ(q) = 0.0f;
}
void pitchQuat(F32 yaw, Quat q)
{
    mathutil_sincosf(yaw*0.5f, &quatX(q), &quatW(q) );
    //quatW(q) = cosf(yaw*0.5f);
    //quatX(q) = sinf(yaw*0.5f);
    quatY(q) = 0.0f;
    quatZ(q) = 0.0f;
}

void rollQuat(F32 yaw, Quat q)
{
    mathutil_sincosf(yaw*0.5f, &quatZ(q), &quatW(q) );
    //quatW(q) = cosf(yaw*0.5f);
    quatX(q) = 0.0f;
    quatY(q) = 0.0f;
    //quatZ(q) = sinf(yaw*0.5f);;
}






//
// Quat conversion
//
void quatToMat(const Quat q,Mat3 R)
{
    F32        tx, ty, tz, twx, twy, twz, txx, txy, txz, tyy, tyz, tzz;

    tx  = 2.f*quatX(q);
    ty  = 2.f*quatY(q);
    tz  = 2.f*quatZ(q);
    twx = tx*quatW(q);
    twy = ty*quatW(q);
    twz = tz*quatW(q);
    txx = tx*quatX(q);
    txy = ty*quatX(q);
    txz = tz*quatX(q);
    tyy = ty*quatY(q);
    tyz = tz*quatY(q);
    tzz = tz*quatZ(q);

    R[0][0] = 1.f-(tyy+tzz);
    R[0][1] = txy-twz;
    R[0][2] = txz+twy;
    R[1][0] = txy+twz;
    R[1][1] = 1.f-(txx+tzz);
    R[1][2] = tyz-twx;
    R[2][0] = txz-twy;
    R[2][1] = tyz+twx;
    R[2][2] = 1.f-(txx+tyy);
}
void quatToMat3_0(const Quat q, Vec3 vec)
{
    F32        ty, tz;

    ty  = 2.0f*quatY(q);
    tz  = 2.0f*quatZ(q);

    vec[0] = 1.0f    -    ((ty*quatY(q))  +  (tz*quatZ(q)));
    vec[1] =            ((ty*quatX(q))  -  (tz*quatW(q)));
    vec[2] =            ((tz*quatX(q))  +  (ty*quatW(q)));
}
void quatToMat3_1(const Quat q, Vec3 vec)
{
    F32        tx, ty, tz;

    tx  = 2.f*quatX(q);
    ty  = 2.f*quatY(q);
    tz  = 2.f*quatZ(q);

    vec[0] =            ((ty*quatX(q))    +    (tz*quatW(q)));
    vec[1] = 1.0f    -    ((tx*quatX(q))    +    (tz*quatZ(q)));
    vec[2] =            ((tz*quatY(q))    -    (tx*quatW(q)));
}

void quatToMat3_2(const Quat q, Vec3 vec)
{
    F32        tx, ty, tz;

    tx  = 2.f*quatX(q);
    ty  = 2.f*quatY(q);
    tz  = 2.f*quatZ(q);

    vec[0] =            ((tz*quatX(q))    -    (ty*quatW(q)));
    vec[1] =            ((tz*quatY(q))    +    (tx*quatW(q)));
    vec[2] = 1.0f    -    ((tx*quatX(q))    +    (ty*quatY(q)));
}

void quatToPYR(const Quat q, Vec3 pyr)
{
    // ST: This could be made faster, but I'm hoping to eventually not need this
    Mat3 mTempSolution;
    quatToMat(q, mTempSolution);
    getMat3PYR(mTempSolution, pyr);
}











//
// Quat utils
//
void quatCalcWFromXYZ(Quat q)
{
    // Quaternions must be unit length, so we can calculate W from the XYZ components,
    // if we assume W is positive
    F32 wSquared = 1.0f - ( 
        quatX(q)*quatX(q) 
        + quatY(q)*quatY(q) 
        + quatZ(q)*quatZ(q)  
        );

    if ( wSquared <= 0.0f )
        quatW(q) = 0.0f;
    else
        quatW(q) = sqrtf(wSquared);
}

// this negates the quaternion if w is negative, so that we always know that W is positive
// for compression purposes
// Note that this does not change the rotation represented by the quaternion, because Q == -Q
// in terms of representing a SO3 rotation.
void quatForceWPositive(Quat q)
{
    if ( quatW(q) < 0.0f )
        scaleVec4(q, -1.0f, q);
}

void quatNormalize(Quat q)
{
    F32        mag2,mag,invmag;
    mag2 = SQR(quatX(q)) + SQR(quatY(q)) + SQR(quatZ(q)) + SQR(quatW(q));

    if ( mag2 <= 0.0f )
    {
        quatX(q) = 1.0f; // make it a unit quat...
        return;
    }
    mag = fsqrt(mag2);
    invmag = 1.f/mag;
    scaleVec4(q,invmag,q);
}

bool quatIsValid(const Quat q)
{
    float fMagSqd = 
        quatX(q)*quatX(q)
        + quatY(q)*quatY(q)
        + quatZ(q)*quatZ(q)
        + quatW(q)*quatW(q);

    if ( fabsf(fMagSqd - 1.0f) > 0.0001f )
        return false;
    return true;
}














//
// Quat multiplication
//
F32 quatInterp(F32 alpha, const Quat a, const Quat b, Quat q)
{
    F32        beta;
    F32        cos_theta;

    beta = 1.f - alpha;

    // cosine theta = dot product of A and B
    cos_theta = (quatX(a)*quatX(b) + quatY(a)*quatY(b) + quatZ(a)*quatZ(b) + quatW(a)*quatW(b));

    if(cos_theta < 0.f)
        alpha = -alpha;

    // interpolate
    quatX(q) = beta*quatX(a) + alpha*quatX(b);
    quatY(q) = beta*quatY(a) + alpha*quatY(b);
    quatZ(q) = beta*quatZ(a) + alpha*quatZ(b);
    quatW(q) = beta*quatW(a) + alpha*quatW(b);
    quatNormalize(q);

    return cos_theta;
}


void quatInverse(const Quat q, Quat qi)
{
    scaleVec3(q, -1.0f, qi);
    quatW(qi) = quatW(q);
}

void quatRotateVec3(const Quat q, const Vec3 vecIn, Vec3 vecOut)
{
    vecOut[0] = quatW(q)*quatW(q)*vecIn[0]
                - 2.0f*quatY(q)*quatW(q)*vecIn[2]
                + 2.0f*quatZ(q)*quatW(q)*vecIn[1]
                + quatX(q)*quatX(q)*vecIn[0]
                + 2.0f*quatY(q)*quatX(q)*vecIn[1]
                + 2.0f*quatZ(q)*quatX(q)*vecIn[2]
                - quatZ(q)*quatZ(q)*vecIn[0]
                - quatY(q)*quatY(q)*vecIn[0]
                ;

    vecOut[1] = 2.0f*quatX(q)*quatY(q)*vecIn[0]
                + quatY(q)*quatY(q)*vecIn[1]
                + 2.0f*quatZ(q)*quatY(q)*vecIn[2]
                - 2.0f*quatW(q)*quatZ(q)*vecIn[0]
                - quatZ(q)*quatZ(q)*vecIn[1]
                + quatW(q)*quatW(q)*vecIn[1]
                + 2.0f*quatX(q)*quatW(q)*vecIn[2]
                - quatX(q)*quatX(q)*vecIn[1]
                ;

    vecOut[2] = 2.0f*quatX(q)*quatZ(q)*vecIn[0]
                + 2.0f*quatY(q)*quatZ(q)*vecIn[1]
                + quatZ(q)*quatZ(q)*vecIn[2]
                + 2.0f*quatW(q)*quatY(q)*vecIn[0]
                - quatY(q)*quatY(q)*vecIn[2]
                - 2.0f*quatW(q)*quatX(q)*vecIn[1]
                - quatX(q)*quatX(q)*vecIn[2]
                + quatW(q)*quatW(q)*vecIn[2]
                ;
}

// Assumes that the quat only rotates about the z-axis, so assumes quatX and quatY are zero, and vec[2] is just copied
void quatRotateVec3ZOnly(const Quat q, const Vec3 vecIn, Vec3 vecOut)
{
    vecOut[0] = quatW(q)*quatW(q)*vecIn[0]
                //- 2.0f*quatY(q)*quatW(q)*vecIn[2]
                + 2.0f*quatZ(q)*quatW(q)*vecIn[1]
                //+ quatX(q)*quatX(q)*vecIn[0]
                //+ 2.0f*quatY(q)*quatX(q)*vecIn[1]
                //+ 2.0f*quatZ(q)*quatX(q)*vecIn[2]
                - quatZ(q)*quatZ(q)*vecIn[0]
                //- quatY(q)*quatY(q)*vecIn[0]
                ;

    vecOut[1] = //2.0f*quatX(q)*quatY(q)*vecIn[0]
                //+ quatY(q)*quatY(q)*vecIn[1]
                //+ 2.0f*quatZ(q)*quatY(q)*vecIn[2]
                - 2.0f*quatW(q)*quatZ(q)*vecIn[0]
                - quatZ(q)*quatZ(q)*vecIn[1]
                + quatW(q)*quatW(q)*vecIn[1]
                //+ 2.0f*quatX(q)*quatW(q)*vecIn[2]
                //- quatX(q)*quatX(q)*vecIn[1]
                ;

    vecOut[2] = vecIn[2];
}



// Note, I assume you want to apply a first, then b, so I handle switching the order for you
void quatMultiply(const Quat a, const Quat b, Quat c)
{
    quatW(c) = (quatW(a) * quatW(b)) - (quatX(a) * quatX(b)) - (quatY(a) * quatY(b)) - (quatZ(a) * quatZ(b)); 

    quatX(c) = (quatW(a) * quatX(b)) + (quatX(a) * quatW(b)) + (quatY(a) * quatZ(b)) - (quatZ(a) * quatY(b));

    quatY(c) = (quatW(a) * quatY(b)) + (quatY(a) * quatW(b)) + (quatZ(a) * quatX(b)) - (quatX(a) * quatZ(b)); 

    quatZ(c) = (quatW(a) * quatZ(b)) + (quatZ(a) * quatW(b)) + (quatX(a) * quatY(b)) - (quatY(a) * quatX(b)); 

    /*
    //b*a=c
    Vec3 vAScaled;
    Vec3 vBScaled;
    Vec3 vCrossAB;
    quatW(c) = quatW(a)*quatW(b) - dotVec3(a, b);
    scaleVec3(a, quatW(b), vAScaled);
    scaleVec3(b, quatW(a), vBScaled);
    crossVec3(a, b, vCrossAB);
    addVec3(vAScaled, vBScaled, c);
    addVec3(c, vCrossAB, c);
    */
}