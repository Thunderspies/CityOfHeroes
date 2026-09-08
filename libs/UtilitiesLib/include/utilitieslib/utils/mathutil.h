/* mathutil.h
 *    This file has been split up to General Math util and 3D Math Util.
 */

#ifndef _FMATH_H
#define _FMATH_H

#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

#include "fpmacros.h"
#include <math.h>
#include <stdlib.h>
#define FINITE(x) ((x) <=0 || (x) >= 0)
#define EPSILON 0.00001f
#define NEARZERO(x) (fabs(x) < EPSILON)


// Most of the math code uses the gcc single-precision version of the standard
// math functions. These defines can be commented out for any function the
// particular compiler you're using supports in single-precision format
#if _MSC_VER < 1400
    #define sinf(a)        sin(a)
    #define cosf(a)        cos(a)
    #define tanf(a)        tan(a)
    #define asinf(a)    asin(a)
    #define acosf(a)    acos(a)
    #define atanf(a)    atan(a)
    #define atan2f(y,x)    atan2(y,x)
    #define sqrtf(a)    sqrt(a)
    #define fabsf(a)    fabs(a)
    #define expf(a)        exp(a)
    #define powf(a,b)    pow(a,b)
    #define ceilf(a)    ceil(a)
    #define floorf(a)    floor(a)
#endif
#define fsqrt(a)    (float)sqrt(a)
#define fsin(a)        sin(a)
#define fcos(a)        cos(a)
#define ftan(a)        tan(a)
#define fasin(a)    asin(a)
#define facos(a)    acos(a)
#define fatan(a)    atan(a)
#define fatan2(y,x)    atan2(y,x)



extern  const Mat4 unitmat;        // Unit matrix
extern  const Mat4 lrmat;            // L-hand to R-hand transform
extern  const Vec3 zerovec3;
extern  const Vec3 onevec3;
extern  const Vec3 upvec3;
extern  const Vec3 unitvec3; // same as onevec
extern  const Vec4 zerovec4;
extern  const Vec4 onevec4;
extern  const Vec4 unitvec4;
extern    const Mat44 unitmat44;
extern  F32 sintable[];
extern  F32 costable[];


// General macros
#define PI            3.14159265358979323846
#define TWOPI        6.28318530717958647692
#define HALFPI        1.57079632679489661923
#define QUARTERPI    0.78539816339744830962
#define ONEOVERPI    0.31830988618379067154

#define TRIG_TABLE_ENTRIES  256

#ifndef ABS
    #define ABS(a)        (((a)<0) ? (-(a)) : (a))
#endif
#define ABS_UNS_DIFF(a, b)    ((a > b)? (a - b) : (b - a))

#define SQR(a)        ((a)*(a))
#ifndef MIN
#define MIN(a,b)    (((a)<(b)) ? (a) : (b))
#define MAX(a,b)    (((a)>(b)) ? (a) : (b))
#endif
#define MIN1(a,b)    if ((b)<(a)) (a) = (b)
#define MAX1(a,b)    if ((b)>(a)) (a) = (b)
#define MINMAX1(a,b,c)    if ((b)>(a)) (a) = (b);else if((c)<(a)) (a) = (c);
#define SIGN(a)        (((a)>=0)  ?  1  : -1)
#define MINMAX(a,min,max) ((a) < (min) ? (min) : (a) > (max) ? (max) : (a))
#ifndef RAD
#define RAD(a)     (((a)*PI)*(1.0f/180.0f))        /* convert degrees to radians */
#endif
#ifndef DEG
#define DEG(a)    (((a)*180.0f)*(1.0f/PI))        /* convert radians to degrees */
#endif


#define DEGVEC3(v) (v[0] = DEG(v[0]), v[1] = DEG(v[1]), v[2] = DEG(v[2]))
#define RADVEC3(v) (v[0] = RAD(v[0]), v[1] = RAD(v[1]), v[2] = RAD(v[2]))

#define MINVEC2(v1,v2,v3) (v3[0] = MIN((v1)[0],(v2)[0]),v3[1] = MIN((v1)[1],(v2)[1]))
#define MAXVEC2(v1,v2,v3) (v3[0] = MAX((v1)[0],(v2)[0]),v3[1] = MAX((v1)[1],(v2)[1]))

#define MINVEC3(v1,v2,v3) (v3[0] = MIN((v1)[0],(v2)[0]),v3[1] = MIN((v1)[1],(v2)[1]),v3[2] = MIN((v1)[2],(v2)[2]))
#define MAXVEC3(v1,v2,v3) (v3[0] = MAX((v1)[0],(v2)[0]),v3[1] = MAX((v1)[1],(v2)[1]),v3[2] = MAX((v1)[2],(v2)[2]))

// Math macros
#define SIMPLEANGLE(ang)            ((ang) >   RAD(180) ? (ang)-RAD(360) : \
                                     (ang) <= -RAD(180) ? (ang)+RAD(360) : (ang))

#define nearf(a,b) ((a)-(b) >= -0.00001 && (a)-(b) <= 0.00001)

#define ALIGN_DOWN(x,a) ((x)&(~((a)-1)))
#define ALIGN_UP(x,a) ALIGN_DOWN((x)+((a)-1),a)

#define vecXidx 0
#define vecYidx 1
#define vecZidx 2
#define vecX(v) ((v)[vecXidx])
#define vecY(v) ((v)[vecYidx])
#define vecZ(v) ((v)[vecZidx])
#define vecParamsXYZ(pos) vecX(pos), vecY(pos), vecZ(pos)
#define vecParamsXZ(pos) vecX(pos), vecZ(pos)

#define copyVec4(src,dst)            (((dst)[0]) = ((src)[0]), ((dst)[1]) = ((src)[1]), ((dst)[2]) = ((src)[2]), ((dst)[3]) = ((src)[3]))
#define copyVec3(src,dst)            (((dst)[0]) = ((src)[0]), ((dst)[1]) = ((src)[1]), ((dst)[2]) = ((src)[2])) 
#define copyVec3XZ(src,dst)            (((dst)[0]) = ((src)[0]), ((dst)[2]) = ((src)[2])) 
#define copyVec3Vec3i(src,dst)        ((dst)[0] = qtrunc((src)[0]), (dst)[1] = qtrunc((src)[1]), (dst)[2] = qtrunc((src)[2]))
#define copyVec2(src,dst)            ((dst)[0] = (src)[0], (dst)[1] = (src)[1])
#define setVec3same(dst,a)            ((dst)[0] = (a), (dst)[1] = (a), (dst)[2] = (a))
#define setVec4same(dst,a)            ((dst)[0] = (a), (dst)[1] = (a), (dst)[2] = (a), (dst)[3] = (a))
#define zeroVec3(v1)                setVec3same(v1, 0)
#define zeroVec3XZ(v1)                ((v1)[0] = 0, (v1)[2] = 0)
#define zeroVec4(v1)                ((v1)[0] = 0, (v1)[1] = 0, (v1)[2] = 0, (v1)[3] = 0)
#define unitVec3(v1)                setVec3same(v1, 1)
#define unitVec4(v1)                setVec4same(v1, 1)
#define vec3IsZero(v)                ((v)[0] == 0 && (v)[1] == 0 && (v)[2] == 0)
#define vec3IsZeroXZ(v)                ((v)[0] == 0 && (v)[2] == 0)
#define vec4IsZero(v)                ((v)[0] == 0 && (v)[1] == 0 && (v)[2] == 0 && (v)[3] == 0)
#define setVec2(dst,a,b)            ((dst)[0] = (a), (dst)[1] = (b))
#define setVec3(dst,a,b,c)            ((dst)[0] = (a), (dst)[1] = (b), (dst)[2] = (c))
#define setVec4(dst,a,b,c,d)        ((dst)[0] = (a), (dst)[1] = (b), (dst)[2] = (c), (dst)[3] = (d))

#define mulVecVec2(v1,v2,r)            ((r)[0] = (v1)[0]*(v2)[0], (r)[1] = (v1)[1]*(v2)[1])
#define mulVecVec3(v1,v2,r)            ((r)[0] = (v1)[0]*(v2)[0], (r)[1] = (v1)[1]*(v2)[1], (r)[2] = (v1)[2]*(v2)[2])
#define mulVecVec4(v1,v2,r)            ((r)[0] = (v1)[0]*(v2)[0], (r)[1] = (v1)[1]*(v2)[1], (r)[2] = (v1)[2]*(v2)[2]), (r)[3] = (v1)[3]*(v2)[3])
#define subVec2(v1,v2,r)            ((r)[0] = (v1)[0]-(v2)[0], (r)[1] = (v1)[1]-(v2)[1])
#define subVec3(v1,v2,r)            ((r)[0] = (v1)[0]-(v2)[0], (r)[1] = (v1)[1]-(v2)[1], (r)[2] = (v1)[2]-(v2)[2])
#define subVec3XZ(v1,v2,r)            ((r)[0] = (v1)[0]-(v2)[0], (r)[2] = (v1)[2]-(v2)[2])
#define subVec4(v1,v2,r)            ((r)[0] = (v1)[0]-(v2)[0], (r)[1] = (v1)[1]-(v2)[1], (r)[2] = (v1)[2]-(v2)[2], (r)[3] = (v1)[3]-(v2)[3])
#define subFromVec2(v,r)            ((r)[0] -= (v)[0], (r)[1] -= (v)[1])
#define subFromVec3(v,r)            ((r)[0] -= (v)[0], (r)[1] -= (v)[1], (r)[2] -= (v)[2])
#define subFromVec4(v,r)            ((r)[0] -= (v)[0], (r)[1] -= (v)[1], (r)[2] -= (v)[2], (r)[3] -= (v)[3])
#define addVec2(v1,v2,r)            ((r)[0] = (v1)[0]+(v2)[0], (r)[1] = (v1)[1]+(v2)[1])
#define addVec3(v1,v2,r)            ((r)[0] = (v1)[0]+(v2)[0], (r)[1] = (v1)[1]+(v2)[1], (r)[2] = (v1)[2]+(v2)[2])
#define addVec3XZ(v1,v2,r)            ((r)[0] = (v1)[0]+(v2)[0], (r)[2] = (v1)[2]+(v2)[2])
#define addVec4(v1,v2,r)            ((r)[0] = (v1)[0]+(v2)[0], (r)[1] = (v1)[1]+(v2)[1], (r)[2] = (v1)[2]+(v2)[2], (r)[3] = (v1)[3]+(v2)[3])
#define addToVec2(v,r)                ((r)[0] += (v)[0], (r)[1] += (v)[1])
#define addToVec3(v,r)                ((r)[0] += (v)[0], (r)[1] += (v)[1], (r)[2] += (v)[2])
#define addToVec4(v,r)                ((r)[0] += (v)[0], (r)[1] += (v)[1], (r)[2] += (v)[2], (r)[3] += (v)[3])
#define scaleVec2(v1,s,r)            ((r)[0] = (v1)[0]*(s), (r)[1] = (v1)[1]*(s))
#define scaleVec3(v1,s,r)            ((r)[0] = (v1)[0]*(s), (r)[1] = (v1)[1]*(s), (r)[2] = (v1)[2]*(s))
#define scaleVec3XZ(v1,s,r)            ((r)[0] = (v1)[0]*(s), (r)[2] = (v1)[2]*(s))
#define scaleVec4(v1,s,r)            ((r)[0] = (v1)[0]*(s), (r)[1] = (v1)[1]*(s), (r)[2] = (v1)[2]*(s), (r)[3] = (v1)[3]*(s))
#define scaleByVec2(r,s)            ((r)[0] *= (s), (r)[1] *= (s))
#define scaleByVec3(r,s)            ((r)[0] *= (s), (r)[1] *= (s), (r)[2] *= (s))
#define scaleByVec4(r,s)            ((r)[0] *= (s), (r)[1] *= (s), (r)[2] *= (s), (r)[3] *= (s))
#define scaleAddVec2(a,s,b,r)        ((r)[0] = ((a)[0]*(s)+(b)[0]), (r)[1] = ((a)[1]*(s)+(b)[1]))
#define scaleAddVec3(a,s,b,r)        ((r)[0] = ((a)[0]*(s)+(b)[0]), (r)[1] = ((a)[1]*(s)+(b)[1]), (r)[2] = ((a)[2]*(s)+(b)[2]))
#define scaleAddVec3XZ(a,s,b,r)        ((r)[0] = ((a)[0]*(s)+(b)[0]), (r)[2] = ((a)[2]*(s)+(b)[2]))
#define lerpVec2(a,t,b,r)            ((r)[0] = ((a)[0]*(t)+(b)[0]*(1-(t))), (r)[1] = ((a)[1]*(t)+(b)[1]*(1-(t))))
#define lerpVec3(a,t,b,r)            ((r)[0] = ((a)[0]*(t)+(b)[0]*(1-(t))), (r)[1] = ((a)[1]*(t)+(b)[1]*(1-(t))), (r)[2] = ((a)[2]*(t)+(b)[2]*(1-(t))))
#define lerpVec4(a,t,b,r)            ((r)[0] = ((a)[0]*(t)+(b)[0]*(1-(t))), (r)[1] = ((a)[1]*(t)+(b)[1]*(1-(t))), (r)[2] = ((a)[2]*(t)+(b)[2]*(1-(t))), (r)[3] = ((a)[3]*(t)+(b)[3]*(1-(t))))
#define scaleSubVec3(a,s,b,r)        ((r)[0] = ((a)[0]*(s)-(b)[0]), (r)[1] = ((a)[1]*(s)-(b)[1]), (r)[2] = ((a)[2]*(s)-(b)[2]))
#define crossVec2(v1,v2)            ((v1)[0]*(v2)[1] - (v1)[1]*(v2)[0])
#define crossVec3(v1,v2,r)            ((r)[0] = (v1)[1]*(v2)[2] - (v1)[2]*(v2)[1], \
                                    (r)[1] = (v1)[2]*(v2)[0] - (v1)[0]*(v2)[2], \
                                    (r)[2] = (v1)[0]*(v2)[1] - (v1)[1]*(v2)[0])
#define dotVec2(v1,v2)                ((v1)[0]*(v2)[0] + (v1)[1]*(v2)[1])
#define dotVec3(v1,v2)                ((v1)[0]*(v2)[0] + (v1)[1]*(v2)[1] + (v1)[2]*(v2)[2])
#define dotVec3XZ(v1,v2)            ((v1)[0]*(v2)[0] + (v1)[2]*(v2)[2])
#define dotVec4(v1,v2)                ((v1)[0]*(v2)[0] + (v1)[1]*(v2)[1] + (v1)[2]*(v2)[2] + (v1)[3]*(v2)[3])
#define dotVec32D(v1,v2)            ((v1)[0]*(v2)[0] + (v1)[2]*(v2)[2])
#define sameVec2(v1,v2)                (((v1)[0]==(v2)[0]) && ((v1)[1]==(v2)[1]))
#define sameVec3(v1,v2)                (((v1)[0]==(v2)[0]) && ((v1)[1]==(v2)[1]) && ((v1)[2]==(v2)[2]))
#define sameVec4(v1,v2)                (((v1)[0]==(v2)[0]) && ((v1)[1]==(v2)[1]) && ((v1)[2]==(v2)[2]) && ((v1)[3]==(v2)[3]))

#define lengthVec2Squared(v1)        (SQR((v1)[0]) + SQR((v1)[1]))
#define lengthVec2(v1)                (fsqrt(lengthVec2Squared(v1)))
#define lengthVec3Squared(v1)        (SQR((v1)[0]) + SQR((v1)[1]) + SQR((v1)[2]))
#define lengthVec3(v1)                (fsqrt(lengthVec3Squared(v1)))

#define lengthVec4Squared(v1)        (SQR((v1)[0]) + SQR((v1)[1]) + SQR((v1)[2]) + SQR((v1)[3]))
#define lengthVec4(v1)                (fsqrt(lengthVec4Squared(v1)))

#define lengthVec3SquaredXZ(v1)        (SQR((v1)[0]) + SQR((v1)[2]))
#define lengthVec3XZ(v1)            (fsqrt(lengthVec3SquaredXZ(v1)))

#define distanceY(v1,v2)            (fabs((v1)[1] - (v2)[1]))

#define moveVec3(p,v,amt)            (p[0] += (v)[0]*(amt), p[1] += (v)[1]*(amt), p[2] += (v)[2]*(amt))
#define moveinX(mat,amt)            (moveVec3((mat)[3],(mat)[0],amt))
#define moveinY(mat,amt)            (moveVec3((mat)[3],(mat)[1],amt))
#define moveinZ(mat,amt)            (moveVec3((mat)[3],(mat)[2],amt))
#define moveVinX(p,mat,amt)            (moveVec3(p,(mat)[0],amt))
#define moveVinY(p,mat,amt)            (moveVec3(p,(mat)[1],amt))
#define moveVinZ(p,mat,amt)            (moveVec3(p,(mat)[2],amt))

#define getVec3Yaw(vec)                (fatan2((vec)[0],(vec)[2]))
#define getVec3Pitch(vec)            (fatan2((vec)[1],(vec)[2]))
#define getVec3Roll(vec)            (fatan2((vec)[1],(vec)[0]))

#define printVec3(vec)                printf("( %.2f, %.2f, %.2f )", vec[0], vec[1], vec[2] )
#define printVec4(vec)                printf("( %.2f, %.2f, %.2f, %.2f )", vec[0], vec[1], vec[2], vec[3] )

#define validateVec3(vec)            (FINITE(vec[0]) && FINITE(vec[1]) && FINITE(vec[2]))

#define validateMat3(mat)            (validateVec3(mat[0]) && validateVec3(mat[1]) && validateVec3(mat[2]))
#define validateMat4(mat)            (validateVec3(mat[0]) && validateVec3(mat[1]) && validateVec3(mat[2]) && validateVec3(mat[3]))

#define getMatRow(mat, row, vec)    (setVec4((vec), (mat)[0][(row)], (mat)[1][(row)], (mat)[2][(row)], (mat)[3][(row)]))

static INLINEDBG int round32(float a) { //return a;
#if defined(_WIN32) && !defined(_XBOX) && !defined(_WIN64)
    int i;
#if defined(__GNUC__) && defined(_WIN32) && defined(__i386__) && !defined(_XBOX)
    __asm__ __volatile__("flds %1; fistpl %0" : "=m" (i) : "m" (a) : "st");
#else
    __asm {
        fld   a
        fistp i
    }
#endif
    return i;
#elif defined(_XBOX) || defined(_WIN64)
    return (int)(a + .5);
#else
    return rint(a); // just hope it's an intrinsic.
#endif
}

static INLINEDBG S64 round64(F64 a) { return (S64)(a + 0.5); }

static INLINEDBG F32 interpF32( F32 scale, F32 val1, F32 val2 )
{
    return val2 * scale + val1 * (1-scale);
}

static INLINEDBG U8 interpU8( F32 scale, U8 val1, U8 val2 )
{
    return (U8)round32((F32)val2*scale + (F32)val1 * (1.0f-scale));
}

static INLINEDBG S16 interpS16( F32 scale, S16 val1, S16 val2 )
{
    return (S16)round32((F32)val2*scale + (F32)val1 * (1.0f-scale));
}

static INLINEDBG int interpInt( F32 scale, int val1, int val2 )
{
    return round32((F32)val2*scale + (F32)val1 * (1.0f-scale));
}

static INLINEDBG S64 interpS64( F32 scale, S64 val1, S64 val2 )
{
    return round64((F64)val2*scale + (F64)val1 * (1.0f-scale));
}

static INLINEDBG void interpVec2( F32 scale, Vec2 val1, Vec2 val2, Vec3 r )
{
    r[0] = interpF32( scale, val1[0], val2[0] );
    r[1] = interpF32( scale, val1[1], val2[1] );
}

static INLINEDBG void interpVec3( F32 scale, Vec3 val1, Vec3 val2, Vec3 r )
{
    r[0] = interpF32( scale, val1[0], val2[0] );
    r[1] = interpF32( scale, val1[1], val2[1] );
    r[2] = interpF32( scale, val1[2], val2[2] );
}

static INLINEDBG void interpVec4( F32 scale, Vec4 val1, Vec4 val2, Vec4 r )
{
    r[0] = interpF32( scale, val1[0], val2[0] );
    r[1] = interpF32( scale, val1[1], val2[1] );
    r[2] = interpF32( scale, val1[2], val2[2] );
    r[3] = interpF32( scale, val1[3], val2[3] );
}

// If you want an interp param (0.0 - 1.0) given the two extents and the result
// The inverse of the interpF32 calc above.
static INLINEDBG F32 calcInterpParam( F32 r, F32 val1, F32 val2 )
{
    F32 fDenom = (val2 - val1);
    if ( !fDenom )
        return 0.0f;
    return (r - val1) / fDenom;
}


static INLINEDBG float dotVec4Vec3( const Vec4 p, const Vec3 q )
{
    return (p[0] * q[0] + p[1] * q[1] + p[2] * q[2] + p[3]);
}

static INLINEDBG int qtrunc(F32 a)
{
#if 0
    return a;
#else
int        t,i,sign,exponent,mantissa,x;

    x = *((int *)&a);
    exponent = (127+23) - ((x >> 23)&255);
    t = exponent & (1<<30);                        // for exponent > 23 - gives wrong, but large answer
    exponent |= (31 - exponent) >> 31;            // for a < 1
    mantissa = (x & ((1<<23)-1)) + (1 << 23);
    i = (mantissa >> exponent) | t;
    sign = x>>31;
    i = i ^ sign;
    i += sign & 1;

    return i;
#endif
}

extern U32 oldControlState;

#if defined(_XBOX) || defined(_WIN64)
    static INLINEDBG void qtruncVec3(const Vec3 flt, int ilt[3])
    {
        ilt[0] = (int)flt[0];
        ilt[1] = (int)flt[1];
        ilt[2] = (int)flt[2];
    }
#else
    #if SERVER
        // Uses the SSE3 FISTTP instruction which is a replacement for the old
        // FISTP which does truncation without having to alter FPU control word.
        // Introduced with the 2004 Prescott revision of Pentium 4.
        // see http://software.intel.com/en-us/articles/how-to-implement-the-fisttp-streaming-simd-extensions-3-instruction/
        // All machines running servers will support SSE3

        // @todo
        // Look into using SSE SIMD instructions to gang convert all the values at once
        // but need to do unaligned loads and stores which may make it pointless
        static INLINEDBG void qtruncVec3(const Vec3 flt,int ilt[3])
        {
#if defined(__GNUC__) && defined(_WIN32) && defined(__i386__) && !defined(_XBOX)
            __asm__ __volatile__(
                "flds %3; fisttpl %0; "
                "flds %4; fisttpl %1; "
                "flds %5; fisttpl %2; "
                : "=m" (ilt[0]), "=m" (ilt[1]), "=m" (ilt[2])
                : "m" (flt[0]), "m" (flt[1]), "m" (flt[2])
                : "st", "memory");
#else
            _asm{
                mov eax,dword ptr[flt]
                mov ecx,dword ptr[ilt]
                fld dword ptr[eax]
                fisttp dword ptr[ecx]
                fld dword ptr[eax+4]
                fisttp dword ptr[ecx+4]
                fld dword ptr[eax+8]
                fisttp dword ptr[ecx+8]
            }
#endif
        }
    #else
        // @todo, _controlfp_s won't be inlined so this will be slow.
        // Use SSE when available or inline the FPU control word manipulations
        // crt routines do that check so it may be better to use those if
        // the spporting code and intrinsics get inlined

        // On a K7 this runs about 50% faster than the integer-only version
        // On a P3 it's about 10% faster
        static INLINEDBG void qtruncVec3(const Vec3 flt,int ilt[3])
        {
            unsigned int saved_fpu_control;
            _controlfp_s(&saved_fpu_control, 0, 0);    // retrieve current control word
            _controlfp_s(NULL, _RC_CHOP, _MCW_RC);    // set the fpu rounding mode to chop
#if defined(__GNUC__) && defined(_WIN32) && defined(__i386__) && !defined(_XBOX)
            __asm__ __volatile__(
                "flds %3; fistpl %0; "
                "flds %4; fistpl %1; "
                "flds %5; fistpl %2; "
                : "=m" (ilt[0]), "=m" (ilt[1]), "=m" (ilt[2])
                : "m" (flt[0]), "m" (flt[1]), "m" (flt[2])
                : "st", "memory");
#else
            _asm{
                mov eax,dword ptr[flt]
                mov ecx,dword ptr[ilt]
                fld dword ptr[eax]
                fistp dword ptr[ecx]
                fld dword ptr[eax+4]
                fistp dword ptr[ecx+4]
                fld dword ptr[eax+8]
                fistp dword ptr[ecx+8]
            }
#endif
            _controlfp_s(NULL, saved_fpu_control, _MCW_RC);    // restore the previous fpu rounding mode
        }
    #endif

#endif

static INLINEDBG int nearSameF32(F32 a, F32 b)
{
    F32 dif = a - b;
    return dif > -0.001f && dif < 0.001f;
}

static INLINEDBG int nearSameDouble(double a, double b)
{
    double dif = a - b;
    return dif > -0.001 && dif < 0.001;
}

static INLINEDBG int nearSameDoubleTol(double a, double b, double tol)
{
    double dif = a - b;
    return dif > -tol && dif < tol;
}

#if defined(_XBOX) || defined(_WIN64)
    static INLINEDBG void mathutil_sincosf( float angle, float* sinPtr, float* cosPtr )
    {
        *sinPtr = sinf(angle);
        *cosPtr = cosf(angle);
    }
#else
    static INLINEDBG void mathutil_sincosf( float angle, float* sinPtr, float* cosPtr )
    {
#if defined(__GNUC__) && defined(_WIN32) && defined(__i386__) && !defined(_XBOX)
        __asm__ __volatile__("flds %2; fsincos; fstps %0; fstps %1"
            : "=m" (*cosPtr), "=m" (*sinPtr) : "m" (angle) : "st", "st(1)");
#else
        __asm
        {
            fld    angle
                mov    ecx, [cosPtr]
                mov    edx, [sinPtr]
                fsincos
                    fstp [ecx]
                    fstp [edx]
        }
#endif
    }
#endif

void initQuickTrig();
F32 qsin(F32 theta);
F32 qcos(F32 theta);

// Generated by mkproto
int finiteVec3(const Vec3 y); 
int finiteVec4(const Vec4 y); 
float distance3( const Vec3 a, const Vec3 b );
static float distance3Squared( const Vec3 a, const Vec3 b );
float distance3XZ( const Vec3 a, const Vec3 b );
float distance3SquaredXZ( const Vec3 a, const Vec3 b );
void getScale(const Mat3 mat, Vec3 scale ); // doesn't change the matrix
void extractScale(Mat3 mat,Vec3 scale); // also normalizes the matrix
void normalMat3(Mat3 mat); // just normalizes the matrix
void setNearSameVec3Tolerance(F32 tol);
int nearSameDVec2(const DVec2 a,const DVec2 b);
//void copyMat3(const Mat3 a,Mat3 b); // inlined
static void copyMat4(const Mat4 a,Mat4 b);
static void copyMat44(const Mat44 mIn,Mat44 mOut);
void transposeMat3(Mat3 uv);
int invertMat3Copy(const Mat3 a, Mat3 b);
void invertMat4Copy(const Mat4 mat,Mat4 inv);
int invertMat44Copy(const Mat44 m, Mat44 r);
void transposeMat3Copy(const Mat3 uv,Mat3 uv2);
//void transposeMat4Copy(const Mat4 mat,Mat4 inv); // inlined
void transposeMat44(Mat44 mat);
void transposeMat44Copy(const Mat44 mIn, Mat44 mOut);
void scaleMat3(const Mat3 mIn,Mat3 mOut,F32 sfactor);
void scaleMat3Vec3(Mat3 a, const Vec3 sfactor);
void scaleMat3Vec3Correct(Mat3 a, const Vec3 sfactor);
void scaleMat3Vec3Xfer(const Mat3 a, const Vec3 sfactor,Mat3 b);
void rotateMat3(const F32 *rpy, Mat3 uvs);
void yawMat3World(F32 angle, Mat3 uv);
void pitchMat3World(F32 angle, Mat3 uv);
void rollMat3World(F32 angle, Mat3 uv);
void yawMat3(F32 angle, Mat3 uv);
void pitchMat3(F32 angle, Mat3 uv);
void rollMat3(F32 angle, Mat3 uv);
void orientMat3(Mat3 mat, const Vec3 dir);
void orientMat3Yvec(Mat3 mat, const Vec3 dir, const Vec3 yvec);
void mulMat3(const Mat3 m1,const Mat3 m2,Mat3 mOut);
void mulMat4(const Mat4 m1,const Mat4 m2,Mat4 mOut);
void mulMat44(const Mat44 m1,const Mat44 m2,Mat44 mOut);
void mulVecMat3Transpose(const Vec3 vIn,const Mat3 mIn,Vec3 vOut);
static void mulVecMat3(const Vec3 vIn,const Mat3 mIn,Vec3 vOut);
static void mulVecMat4(const Vec3 vIn,const Mat4 mIn,Vec3 vOut);
void mulVecMat4Transpose(const Vec3 vIn, const Mat4 mIn, Vec3 vOut);
//F32 normalVec3(Vec3 v); // inlined
F32 normalVec3XZ(Vec3 v);
F32 normalVec2(Vec2 v);
double normalDVec2(DVec2 v);
F32 det3Vec3(const Vec3 v0,const Vec3 v1,const Vec3 v2);
void getVec3YP(const Vec3 dvec,F32 *yawp,F32 *pitp);
void getVec3PY(const Vec3 dvec,F32 *pitp,F32 *yawp);
F32 fixAngle(F32 a);
F32 subAngle(F32 a, F32 b);
F32 addAngle(F32 a, F32 b);
void createMat3PYR(Mat3 mat,const F32 *pyr);
void createMat3RYP(Mat3 mat,const F32 *pyr);
void createMat3YPR(Mat3 mat,const F32 *pyr);
void createMat3_0_YPR(Vec3 mat0,const F32 *pyr);
void createMat3_1_YPR(Vec3 mat1,const F32 *pyr);
void createMat3_2_YPR(Vec3 mat2,const F32 *pyr);
void getMat3PYR(const Mat3 mat,F32 *pyr);
void getMat3YPR(const Mat3 mat,F32 *pyr);
static void mat43to44(const Mat4 in,Mat44 out);


void getRandomPointOnLimitedSphereSurface(F32* theta, F32* phi, F32 phiDeflectionFromNorthPole);
void getRandomPointOnSphereSurface(F32* theta, F32* phi );
void sphericalCoordsToVec3(Vec3 vOut, F32 theta, F32 phi, F32 radius);
void getRandomAngularDeflection(Vec3 vOutput, F32 fAngle);

void getQuickRandomPointOnCylinder(const Vec3 start_pos, F32 radius, F32 height, Vec3 final_pos );
void getRandomPointOnLine(const Vec3 vStart, const Vec3 vEnd, Vec3 vResult);



void posInterp(F32 t, const Vec3 T0, const Vec3 T1, Vec3 T);
int planeIntersect(const Vec3 start,const Vec3 end,const Mat4 pmat,Vec3 cpos);
void camLookAt(const Vec3 lookdir, Mat4 mat);
void mat3FromUpVector(const Vec3 upVec, Mat3 mat);
int getCappedRandom(int max);
F32 interpAngle(  F32 percent_a, F32 a, F32 b );
void interpPYR( F32 percent_a, const Vec3 a, const Vec3 b, Vec3 result );
void circleDeltaVec3( const Vec3 org, const Vec3 dest, Vec3 delta );
F32    circleDelta( F32 org, F32 dest );
F32 quick_fsqrt( F32 x );
int sphereLineCollision( F32 sphere_radius, const Vec3 sphere_mid, const Vec3 line_start, const Vec3 line_end );
void rule30Seed(U32 c);
U32 rule30Rand();
#define safe_ftoa(f, buf) safe_ftoa_s(f, SAFESTR(buf))
char *safe_ftoa_s(F32 f, char *buf, size_t buf_size);
float graphInterp(float x, const Vec3* interpPoints);    

// simple axis aligned collision tests
bool pointBoxCollision( const Vec3 point, const Vec3 min, const Vec3 max );
bool lineBoxCollision( const Vec3 start, const Vec3 end, const Vec3 min, const Vec3 max, Vec3 intersect );
bool boxBoxCollision( const Vec3 min1, const Vec3 max1, const Vec3 min2, const Vec3 max2 );
F32 distanceToBoxSquared(const Vec3 boxmin, const Vec3 boxmax, const Vec3 point);

//void tangentBasis(Mat3 basis,Vec3 v0,Vec3 v1,Vec3 v2,Vec2 t0,Vec2 t1,Vec2 t2,Vec3 n); //mm
// End mkproto


int randInt(int max);
U64 randU64(U64 max);
int log2i(int val);
int pow2(int val); // Returns the number rounded up to a power of two
int randIntGivenSeed(int * seed);
int randUIntGivenSeed(int * seed);

//* Quick random numbers of various sorts*/

extern F32 floatseed;
extern long holdrand;

/* seed the random numberator */
static INLINEDBG void qsrand ( int seed )
{
    holdrand = (long)seed;
}

/*quick unsigned int between 0 and ~32000 (same as rand()) */
static INLINEDBG int qrand()
{
    return(((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);
}

/*quick signed rand int*/
static INLINEDBG int qsirand()
{
    return(holdrand = (holdrand * 214013L + 2531011L));
}

/* quick F32 between -1.0 and 1.0*/// 
static INLINEDBG F32 qfrand()
{
    return (F32)(((F32)(holdrand = (holdrand * 214013L + 2531011L))) * ((F32)(1.f/(F32)(0x7fffffff))));
}

/* quick F32 between -1.0 and 1.0*/// 
static INLINEDBG F32 qfrandFromSeed( int * seed )
{
    return (F32)(((F32)((*seed) = ((*seed) * 214013L + 2531011L))) * ((F32)(1.f/(F32)(0x7fffffff))));
}

// returns between -1.0 and 1.0
static INLINEDBG F32 rule30Float()
{
      U32 l,r;
      static U32 c = 0xf244f343;
      l = r = c;
      l = _lrotr(l, 1);/* bit i of l equals bit just left of bit i in c */
      r = _lrotl(r, 1);/* bit i of r euqals bit just right of bit i in c */
      c |= r;
      c ^= l;           /* c = l xor (c or r), aka rule 30, named by wolfram */
      return -1.f + (c * 1.f / (F32)0x7fffffffUL);
}

// returns between 0.0 and 99.999999
static INLINEDBG F32 rule30FloatPct()
{
    F32 r = rule30Float();
    if (r < 0.0) r *= -1;
    return r * 99.99999f;
}

/* quick F32 between -1.0 and 1.0 
__forceinline F32 qfrand()
{
    int s;
    s = (0x3f800000 | ( 0x007fffff & qsirand() ) );
    return ( (1.5 - (*((F32 * )&s))) * 2.0 ) ;
}*/

/* quick F32 between 0 and 1 TO DO re__forceinline */
/* quick F32 between 0 and 1 */
static INLINEDBG F32 qufrand()
{
    int s;
    s = (0x3f800000 | ( 0x007fffff & qsirand() ) );
    return ( (*((F32 * )&s))) - 1 ;
}

    
/* quick true or false *///
static INLINEDBG int qbrand()
{
    return qsirand() & 1;
}

/* quick 1 or -1 *///
static INLINEDBG int qnegrand()
{
    return (qsirand() >> 31)|1;
}

static INLINEDBG bool isPower2(unsigned int x)
{
    if ( x < 1 )
        return false;

    if ( x & (x-1) )
        return false;

    return true;
}


static INLINEDBG void mat43to44(const Mat4 in,Mat44 out)
{
    copyVec3(in[0],out[0]);
    copyVec3(in[1],out[1]);
    copyVec3(in[2],out[2]);
    copyVec3(in[3],out[3]);
    out[0][3] = out[1][3] = out[2][3] = 0;
    out[3][3] = 1;
}

static INLINEDBG void mat44to43(const Mat44 in,Mat4 out)
{
    copyVec3(in[0],out[0]);
    copyVec3(in[1],out[1]);
    copyVec3(in[2],out[2]);
    copyVec3(in[3],out[3]);
}

static INLINEDBG void mulMat3Inline(const Mat3 m1,const Mat3 m2,Mat3 mOut)
{
    (mOut)[0][0] = (m2)[0][0] * (m1)[0][0] + (m2)[0][1] * (m1)[1][0] + (m2)[0][2] * (m1)[2][0];
    (mOut)[0][1] = (m2)[0][0] * (m1)[0][1] + (m2)[0][1] * (m1)[1][1] + (m2)[0][2] * (m1)[2][1];
    (mOut)[0][2] = (m2)[0][0] * (m1)[0][2] + (m2)[0][1] * (m1)[1][2] + (m2)[0][2] * (m1)[2][2];
    (mOut)[1][0] = (m2)[1][0] * (m1)[0][0] + (m2)[1][1] * (m1)[1][0] + (m2)[1][2] * (m1)[2][0];
    (mOut)[1][1] = (m2)[1][0] * (m1)[0][1] + (m2)[1][1] * (m1)[1][1] + (m2)[1][2] * (m1)[2][1];
    (mOut)[1][2] = (m2)[1][0] * (m1)[0][2] + (m2)[1][1] * (m1)[1][2] + (m2)[1][2] * (m1)[2][2];
    (mOut)[2][0] = (m2)[2][0] * (m1)[0][0] + (m2)[2][1] * (m1)[1][0] + (m2)[2][2] * (m1)[2][0];
    (mOut)[2][1] = (m2)[2][0] * (m1)[0][1] + (m2)[2][1] * (m1)[1][1] + (m2)[2][2] * (m1)[2][1];
    (mOut)[2][2] = (m2)[2][0] * (m1)[0][2] + (m2)[2][1] * (m1)[1][2] + (m2)[2][2] * (m1)[2][2];
}

static INLINEDBG void mulMat4Inline(const Mat4 m1,const Mat4 m2,Mat4 mOut)
{
    // This is the fastest way to do it in C for x86
    (mOut)[0][0] = (m2)[0][0] * (m1)[0][0] + (m2)[0][1] * (m1)[1][0] + (m2)[0][2] * (m1)[2][0];
    (mOut)[0][1] = (m2)[0][0] * (m1)[0][1] + (m2)[0][1] * (m1)[1][1] + (m2)[0][2] * (m1)[2][1];
    (mOut)[0][2] = (m2)[0][0] * (m1)[0][2] + (m2)[0][1] * (m1)[1][2] + (m2)[0][2] * (m1)[2][2];
    (mOut)[1][0] = (m2)[1][0] * (m1)[0][0] + (m2)[1][1] * (m1)[1][0] + (m2)[1][2] * (m1)[2][0];
    (mOut)[1][1] = (m2)[1][0] * (m1)[0][1] + (m2)[1][1] * (m1)[1][1] + (m2)[1][2] * (m1)[2][1];
    (mOut)[1][2] = (m2)[1][0] * (m1)[0][2] + (m2)[1][1] * (m1)[1][2] + (m2)[1][2] * (m1)[2][2];
    (mOut)[2][0] = (m2)[2][0] * (m1)[0][0] + (m2)[2][1] * (m1)[1][0] + (m2)[2][2] * (m1)[2][0];
    (mOut)[2][1] = (m2)[2][0] * (m1)[0][1] + (m2)[2][1] * (m1)[1][1] + (m2)[2][2] * (m1)[2][1];
    (mOut)[2][2] = (m2)[2][0] * (m1)[0][2] + (m2)[2][1] * (m1)[1][2] + (m2)[2][2] * (m1)[2][2];
    (mOut)[3][0] = (m2)[3][0] * (m1)[0][0] + (m2)[3][1] * (m1)[1][0] + (m2)[3][2] * (m1)[2][0] + (m1)[3][0];
    (mOut)[3][1] = (m2)[3][0] * (m1)[0][1] + (m2)[3][1] * (m1)[1][1] + (m2)[3][2] * (m1)[2][1] + (m1)[3][1];
    (mOut)[3][2] = (m2)[3][0] * (m1)[0][2] + (m2)[3][1] * (m1)[1][2] + (m2)[3][2] * (m1)[2][2] + (m1)[3][2];
}

static INLINEDBG void mulMat44Inline(const Mat44 m1,const Mat44 m2,Mat44 mOut)
{
    (mOut)[0][0] = (m2)[0][0] * (m1)[0][0] + (m2)[0][1] * (m1)[1][0] + (m2)[0][2] * (m1)[2][0] + (m2)[0][3] * (m1)[3][0];
    (mOut)[0][1] = (m2)[0][0] * (m1)[0][1] + (m2)[0][1] * (m1)[1][1] + (m2)[0][2] * (m1)[2][1] + (m2)[0][3] * (m1)[3][1];
    (mOut)[0][2] = (m2)[0][0] * (m1)[0][2] + (m2)[0][1] * (m1)[1][2] + (m2)[0][2] * (m1)[2][2] + (m2)[0][3] * (m1)[3][2];
    (mOut)[0][3] = (m2)[0][0] * (m1)[0][3] + (m2)[0][1] * (m1)[1][3] + (m2)[0][2] * (m1)[2][3] + (m2)[0][3] * (m1)[3][3];

    (mOut)[1][0] = (m2)[1][0] * (m1)[0][0] + (m2)[1][1] * (m1)[1][0] + (m2)[1][2] * (m1)[2][0] + (m2)[1][3] * (m1)[3][0];
    (mOut)[1][1] = (m2)[1][0] * (m1)[0][1] + (m2)[1][1] * (m1)[1][1] + (m2)[1][2] * (m1)[2][1] + (m2)[1][3] * (m1)[3][1];
    (mOut)[1][2] = (m2)[1][0] * (m1)[0][2] + (m2)[1][1] * (m1)[1][2] + (m2)[1][2] * (m1)[2][2] + (m2)[1][3] * (m1)[3][2];
    (mOut)[1][3] = (m2)[1][0] * (m1)[0][3] + (m2)[1][1] * (m1)[1][3] + (m2)[1][2] * (m1)[2][3] + (m2)[1][3] * (m1)[3][3];

    (mOut)[2][0] = (m2)[2][0] * (m1)[0][0] + (m2)[2][1] * (m1)[1][0] + (m2)[2][2] * (m1)[2][0] + (m2)[2][3] * (m1)[3][0];
    (mOut)[2][1] = (m2)[2][0] * (m1)[0][1] + (m2)[2][1] * (m1)[1][1] + (m2)[2][2] * (m1)[2][1] + (m2)[2][3] * (m1)[3][1];
    (mOut)[2][2] = (m2)[2][0] * (m1)[0][2] + (m2)[2][1] * (m1)[1][2] + (m2)[2][2] * (m1)[2][2] + (m2)[2][3] * (m1)[3][2];
    (mOut)[2][3] = (m2)[2][0] * (m1)[0][3] + (m2)[2][1] * (m1)[1][3] + (m2)[2][2] * (m1)[2][3] + (m2)[2][3] * (m1)[3][3];

    (mOut)[3][0] = (m2)[3][0] * (m1)[0][0] + (m2)[3][1] * (m1)[1][0] + (m2)[3][2] * (m1)[2][0] + (m2)[3][3] * (m1)[3][0];
    (mOut)[3][1] = (m2)[3][0] * (m1)[0][1] + (m2)[3][1] * (m1)[1][1] + (m2)[3][2] * (m1)[2][1] + (m2)[3][3] * (m1)[3][1];
    (mOut)[3][2] = (m2)[3][0] * (m1)[0][2] + (m2)[3][1] * (m1)[1][2] + (m2)[3][2] * (m1)[2][2] + (m2)[3][3] * (m1)[3][2];
    (mOut)[3][3] = (m2)[3][0] * (m1)[0][3] + (m2)[3][1] * (m1)[1][3] + (m2)[3][2] * (m1)[2][3] + (m2)[3][3] * (m1)[3][3];
}

// Multiply a vector times a 3*3 matrix into another vector
static INLINEDBG void mulVecMat3(const Vec3 vIn,const Mat3 mIn,Vec3 vOut)
{
    vOut[0] = vIn[0]*mIn[0][0] + vIn[1]*mIn[1][0] + vIn[2]*mIn[2][0];
    vOut[1] = vIn[0]*mIn[0][1] + vIn[1]*mIn[1][1] + vIn[2]*mIn[2][1];
    vOut[2] = vIn[0]*mIn[0][2] + vIn[1]*mIn[1][2] + vIn[2]*mIn[2][2];
}

// Multiply a vector times a 3*3 + position matrix into another vector
static INLINEDBG void mulVecMat4(const Vec3 vIn,const Mat4 mIn,Vec3 vOut)
{
    vOut[0] = vIn[0]*mIn[0][0]+vIn[1]*mIn[1][0]+vIn[2]*mIn[2][0] + mIn[3][0];
    vOut[1] = vIn[0]*mIn[0][1]+vIn[1]*mIn[1][1]+vIn[2]*mIn[2][1] + mIn[3][1];
    vOut[2] = vIn[0]*mIn[0][2]+vIn[1]*mIn[1][2]+vIn[2]*mIn[2][2] + mIn[3][2];
}

static INLINEDBG void mulVecMat44(const Vec3 vIn,const Mat44 mIn,Vec4 vOut)
{
    vOut[0] = vIn[0]*mIn[0][0]+vIn[1]*mIn[1][0]+vIn[2]*mIn[2][0] + mIn[3][0];
    vOut[1] = vIn[0]*mIn[0][1]+vIn[1]*mIn[1][1]+vIn[2]*mIn[2][1] + mIn[3][1];
    vOut[2] = vIn[0]*mIn[0][2]+vIn[1]*mIn[1][2]+vIn[2]*mIn[2][2] + mIn[3][2];
    vOut[3] = vIn[0]*mIn[0][3]+vIn[1]*mIn[1][3]+vIn[2]*mIn[2][3] + mIn[3][3];
}

static INLINEDBG void mulVec3Mat44(const Vec3 vIn,const Mat44 mIn,Vec3 vOut)
{
    F32 scale = 1.f/(vIn[0]*mIn[0][3]+vIn[1]*mIn[1][3]+vIn[2]*mIn[2][3] + mIn[3][3]);
    vOut[0] = (vIn[0]*mIn[0][0]+vIn[1]*mIn[1][0]+vIn[2]*mIn[2][0] + mIn[3][0]) * scale;
    vOut[1] = (vIn[0]*mIn[0][1]+vIn[1]*mIn[1][1]+vIn[2]*mIn[2][1] + mIn[3][1]) * scale;
    vOut[2] = (vIn[0]*mIn[0][2]+vIn[1]*mIn[1][2]+vIn[2]*mIn[2][2] + mIn[3][2]) * scale;
}

static INLINEDBG void copyMat4(const Mat4 mIn,Mat4 mOut)
{
    copyVec3((mIn)[0],(mOut)[0]);
    copyVec3((mIn)[1],(mOut)[1]);
    copyVec3((mIn)[2],(mOut)[2]);
    copyVec3((mIn)[3],(mOut)[3]);
}

static INLINEDBG void copyMat44(const Mat44 mIn,Mat44 mOut)
{
    copyVec4((mIn)[0],(mOut)[0]);
    copyVec4((mIn)[1],(mOut)[1]);
    copyVec4((mIn)[2],(mOut)[2]);
    copyVec4((mIn)[3],(mOut)[3]);
}

static INLINEDBG float distance2Squared(const Vec2 a, const Vec2 b)
{
    return SQR(a[0] - b[0]) + SQR(a[1] - b[1]);
}

static INLINEDBG float distance3Squared( const Vec3 a, const Vec3 b )
{
    return SQR(a[0] - b[0]) + SQR(a[1] - b[1]) + SQR(a[2] - b[2]);
}

static INLINEDBG void vec3MinMax(const Vec3 a, const Vec3 b, Vec3 min, Vec3 max)
{
    if (a[0] < b[0])
    {
        min[0] = a[0];
        max[0] = b[0];
    }
    else
    {
        min[0] = b[0];
        max[0] = a[0];
    }

    if (a[1] < b[1])
    {
        min[1] = a[1];
        max[1] = b[1];
    }
    else
    {
        min[1] = b[1];
        max[1] = a[1];
    }

    if (a[2] < b[2])
    {
        min[2] = a[2];
        max[2] = b[2];
    }
    else
    {
        min[2] = b[2];
        max[2] = a[2];
    }
}

static INLINEDBG void vec2MinMax(const Vec2 a, const Vec2 b, Vec2 min, Vec2 max)
{
    if (a[0] < b[0])
    {
        min[0] = a[0];
        max[0] = b[0];
    }
    else
    {
        min[0] = b[0];
        max[0] = a[0];
    }

    if (a[1] < b[1])
    {
        min[1] = a[1];
        max[1] = b[1];
    }
    else
    {
        min[1] = b[1];
        max[1] = a[1];
    }
}

static INLINEDBG void vec2RunningMinMax(const Vec2 v, Vec2 min, Vec2 max)
{
    MIN1(min[0], v[0]);
    MAX1(max[0], v[0]);
    MIN1(min[1], v[1]);
    MAX1(max[1], v[1]);
}


static INLINEDBG void vec3RunningMin(const Vec3 v, Vec3 min)
{
    MIN1(min[0], v[0]);
    MIN1(min[1], v[1]);
    MIN1(min[2], v[2]);
}

static INLINEDBG void vec3RunningMax(const Vec3 v, Vec3 max)
{
    MAX1(max[0], v[0]);
    MAX1(max[1], v[1]);
    MAX1(max[2], v[2]);
}

static INLINEDBG void vec3RunningMinMax(const Vec3 v, Vec3 min, Vec3 max)
{
    MIN1(min[0], v[0]);
    MAX1(max[0], v[0]);
    MIN1(min[1], v[1]);
    MAX1(max[1], v[1]);
    MIN1(min[2], v[2]);
    MAX1(max[2], v[2]);
}

static INLINEDBG int nearSameVec3(const Vec3 va, const Vec3 vb)
{
    extern F32 near_same_vec3_tol_squared;
    Vec3    dv;

    subVec3(va,vb,dv);

    return (dv[0]*dv[0] + dv[1]*dv[1] + dv[2]*dv[2]) <= near_same_vec3_tol_squared;
}

static INLINEDBG int nearSameVec3Tol(const Vec3 a,const Vec3 b, F32 tolerance)
{
    Vec3    dv;

    subVec3(a,b,dv);
    return (dv[0]*dv[0] + dv[1]*dv[1] + dv[2]*dv[2]) <= (tolerance*tolerance);
}

static INLINEDBG int nearSameVec2(const Vec2 a,const Vec2 b)
{
    if (!nearSameF32(a[0], b[0]))
        return 0;
    if (!nearSameF32(a[1], b[1]))
        return 0;
    return 1;
}

static INLINEDBG F32 normalVec3(Vec3 v)
{
F32        mag2,mag,invmag;

    // Is there some way to let the compiler know to generate an atomic inverse
    // square root if the cpu supports it? (ie; sse, 3dnow, r10k, etc.)
    mag2 = SQR(v[0]) + SQR(v[1]) + SQR(v[2]);
    mag = fsqrt(mag2);

    if (mag > 0)
    {
        invmag = 1.f/mag;
        scaleVec3(v,invmag,v);
    }

    return(mag);
}




#define PLANE_EPSILON (1.0e-5)

// returns the area of the triangle
static INLINEDBG float makePlane(const Vec3 p0, const Vec3 p1, const Vec3 p2, Vec4 plane)
{
    float len;
    Vec3 a, b;
    subVec3(p1, p0, a);
    subVec3(p2, p0, b);
    crossVec3(a, b, plane);
    len = normalVec3(plane);
    plane[3] = dotVec3(plane, p0);
    return len * 0.5f;
}

static INLINEDBG void makePlane2(const Vec3 pos, const Vec3 normal, Vec4 plane)
{
    copyVec3(normal, plane);
    normalVec3(plane);
    plane[3] = dotVec3(plane, pos);
}

static INLINEDBG float distanceToPlane(const Vec3 pos, const Vec4 plane)
{
    float dist = dotVec3(pos, plane) - plane[3];

    if (dist < PLANE_EPSILON && dist > -PLANE_EPSILON)
        return 0;

    return dist;
}

static INLINEDBG void intersectPlane2(const Vec3 p0, const Vec3 p1, const Vec4 plane, F32 dist0, F32 dist1, Vec3 intersect)
{
    F32 ratio;
    Vec3 diff;
    dist0 = ABS(dist0);
    dist1 = ABS(dist1);
    ratio = dist0 / (dist0 + dist1);
    subVec3(p1, p0, diff);
    scaleAddVec3(diff, ratio, p0, intersect);
}

static INLINEDBG int intersectPlane(const Vec3 p0, const Vec3 p1, const Vec4 plane, Vec3 intersect)
{
    F32 d0 = distanceToPlane(p0, plane);
    F32 d1 = distanceToPlane(p1, plane);
    if (d0 > 0 && d1 > 0)
        return 0;
    if (d0 < 0 && d1 < 0)
        return 0;
    if (d0 == 0)
    {
        copyVec3(p0, intersect);
        return 1;
    }
    if (d1 == 0)
    {
        copyVec3(p1, intersect);
        return 1;
    }
    intersectPlane2(p0, p1, plane, d0, d1, intersect);
    return 1;
}

// |m00 m10|
// |m01 m11|
static INLINEDBG int invertMat2(F32 m00, F32 m01, F32 m10, F32 m11, Vec2 inv0, Vec2 inv1)
{
    F32 det, detinv;

    // determinant
    det = m00 * m11 - m01 * m10;
    if (nearSameF32(det, 0))
        return 0;

    // invert
    detinv = 1.f / det;
    inv0[0] = m11 * detinv;
    inv0[1] = -m01 * detinv;
    inv1[0] = -m10 * detinv;
    inv1[1] = m00 * detinv;

    return 1;
}

// |m00 m10|
// |m01 m11|
static INLINEDBG int invertDMat2(double m00, double m01, double m10, double m11, DVec2 inv0, DVec2 inv1)
{
    double det;

    // determinant
    det = m00 * m11 - m01 * m10;
    if (nearSameDoubleTol(det, 0, 1e-12))
        return 0;

    // invert
    inv0[0] = m11 / det;
    inv0[1] = -m01 / det;
    inv1[0] = -m10 / det;
    inv1[1] = m00 / det;

    return 1;
}

static INLINEDBG void mulVecMat2(const Vec2 m0, const Vec2 m1, const Vec2 v, Vec2 out)
{
    out[0] = m0[0] * v[0] + m1[0] * v[1];
    out[1] = m0[1] * v[0] + m1[1] * v[1];
}

static INLINEDBG void mulDVecMat2(const DVec2 m0, const DVec2 m1, const DVec2 v, DVec2 out)
{
    out[0] = m0[0] * v[0] + m1[0] * v[1];
    out[1] = m0[1] * v[0] + m1[1] * v[1];
}

static INLINEDBG void mulVecMat2_special(const Vec2 m0, const Vec2 m1, F32 v0, F32 v1, F32 *t, F32 *u)
{
    *t = m0[0] * v0 + m1[0] * v1;
    *u = m0[1] * v0 + m1[1] * v1;
}

static INLINEDBG void mulDVecMat2_special(const DVec2 m0, const DVec2 m1, double v0, double v1, double *t, double *u)
{
    *t = m0[0] * v0 + m1[0] * v1;
    *u = m0[1] * v0 + m1[1] * v1;
}

void calcTransformVectors(const Vec3 pos[3], const Vec2 uv[3], Vec3 utransform, Vec3 vtransform);
void calcTransformVectorsAccurate(const Vec3 pos[3], const Vec2 uv[3], Vec3 utransform, Vec3 vtransform);

static INLINEDBG float findCenter(const Vec3 p0, const Vec3 p1, const Vec3 p2, Vec3 center)
{
    float radiusSq, temp;
    addVec3(p0, p1, center);
    addVec3(center, p2, center);
    scaleVec3(center, 0.333333f, center);

    radiusSq = distance3Squared(center, p0);
    temp = distance3Squared(center, p1);
    radiusSq = MAX(radiusSq, temp);
    temp = distance3Squared(center, p2);
    radiusSq = MAX(radiusSq, temp);

    return (float)(sqrtf(radiusSq));
}

static INLINEDBG int index(int x, int y, int width)
{
    return y * width + x;
}

static INLINEDBG int boundsIndex(int x, int y, int z)
{
    return ((!!z)<<2)|((!!y)<<1)|(!!x);
}

static INLINEDBG void mulBounds(const Vec3 min, const Vec3 max, const Mat4 mat, Vec3 transformed[8])
{
#if 0
    int        x, xbit, y, ybit, z, zbit;
    Vec3    extents[2], pos;
    copyVec3(min, extents[0]);
    copyVec3(max, extents[1]);
    for(x=0,xbit=0;x<2;x++,xbit++)
    {
        for(y=0,ybit=0;y<2;y++,ybit+=2)
        {
            for(z=0,zbit=0;z<2;z++,zbit+=4)
            {
                pos[0] = extents[x][0];
                pos[1] = extents[y][1];
                pos[2] = extents[z][2];
                mulVecMat4(pos,mat,transformed[xbit|ybit|zbit]);
            }
        }
    }
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

static INLINEDBG void mulBoundsAA(const Vec3 min, const Vec3 max, const Mat4 mat, Vec3 newmin, Vec3 newmax)
{
    Vec3 transformed[8];
    mulBounds(min, max, mat, transformed);
    setVec3(newmin, 8e16, 8e16, 8e16);
    setVec3(newmax, -8e16, -8e16, -8e16);
    vec3RunningMinMax(transformed[0], newmin, newmax);
    vec3RunningMinMax(transformed[1], newmin, newmax);
    vec3RunningMinMax(transformed[2], newmin, newmax);
    vec3RunningMinMax(transformed[3], newmin, newmax);
    vec3RunningMinMax(transformed[4], newmin, newmax);
    vec3RunningMinMax(transformed[5], newmin, newmax);
    vec3RunningMinMax(transformed[6], newmin, newmax);
    vec3RunningMinMax(transformed[7], newmin, newmax);
}

static INLINEDBG void mulBounds44(const Vec3 min, const Vec3 max, const Mat44 mat, Vec4 transformed[8])
{
    int        x, xbit, y, ybit, z, zbit;
    Vec3    extents[2], pos;
    copyVec3(min, extents[0]);
    copyVec3(max, extents[1]);
    for(x=0,xbit=0;x<2;x++,xbit++)
    {
        for(y=0,ybit=0;y<2;y++,ybit+=2)
        {
            for(z=0,zbit=0;z<2;z++,zbit+=4)
            {
                pos[0] = extents[x][0];
                pos[1] = extents[y][1];
                pos[2] = extents[z][2];
                mulVecMat44(pos,mat,transformed[xbit|ybit|zbit]);
            }
        }
    }
}

static INLINEDBG void mulBoundsAA44(const Vec3 min, const Vec3 max, const Mat44 mat, Vec3 newmin, Vec3 newmax)
{
    Vec4 transformed[8];
    mulBounds44(min, max, mat, transformed);
    setVec3(newmin, 8e16, 8e16, 8e16);
    setVec3(newmax, -8e16, -8e16, -8e16);

#define minmax4(vec) if ((vec)[3] > 0) { F32 scale = 1.f / (vec)[3]; scaleVec3(vec, scale, vec); vec3RunningMinMax(vec, newmin, newmax); }

    minmax4(transformed[0])
    minmax4(transformed[1])
    minmax4(transformed[2])
    minmax4(transformed[3])
    minmax4(transformed[4])
    minmax4(transformed[5])
    minmax4(transformed[6])
    minmax4(transformed[7])

#undef minmax4

    if (newmin[0] > 8e15)
    {
        setVec3(newmin, 0, 0, 0);
        setVec3(newmax, 0, 0, 0);
    }
}

static INLINEDBG F32 boxCalcMid(Vec3 min, Vec3 max, Vec3 mid)
{
    F32 radius;
    subVec3(max, min, mid);
    scaleVec3(mid, 0.5f, mid);
    radius = lengthVec3(mid);
    addVec3(min, mid, mid);
    return radius;
}

static INLINEDBG void boxIntersect(const Vec3 min1, const Vec3 max1, const Vec3 min2, const Vec3 max2, Vec3 minOut, Vec3 maxOut)
{
    minOut[0] = MAX(min1[0], min2[0]);
    minOut[1] = MAX(min1[1], min2[1]);
    minOut[2] = MAX(min1[2], min2[2]);
    maxOut[0] = MIN(max1[0], max2[0]);
    maxOut[1] = MIN(max1[1], max2[1]);
    maxOut[2] = MIN(max1[2], max2[2]);
}

static INLINEDBG F32 sphereDistanceToBoxSquared(const Vec3 boxmin, const Vec3 boxmax, const Vec3 center, F32 radius)
{
    F32 dist = distanceToBoxSquared(boxmin, boxmax, center) - SQR(radius);
    MAX1(dist, 0);
    return dist;
} 

static INLINEDBG F32 triArea(Vec3 a, Vec3 b, Vec3 c)
{
    Vec3 tmp, tmp2, tmp3;
    subVec3(b, a, tmp);
    subVec3(c, tmp, tmp2);
    crossVec3(tmp, tmp2, tmp3);
    return 0.5f * lengthVec3(tmp3);
}

static INLINEDBG F32 triArea2d(Vec2 a, Vec2 b, Vec2 c)
{
    Vec2 tmp, tmp2;
    subVec2( b, a, tmp );
    subVec2( c, a, tmp2 );
    return ABS(0.5f * crossVec2(tmp,tmp2));
}

static INLINEDBG void zeroMat3(Mat3 m)
{
    memset(&m[0][0], 0, sizeof(Mat3));
}

static INLINEDBG void zeroMat4(Mat4 m)
{
    memset(&m[0][0], 0, sizeof(Mat4));
}

static INLINEDBG void zeroMat44(Mat44 m)
{
    memset(&m[0][0], 0, sizeof(Mat44));
}

static INLINEDBG void identityMat3(Mat3 m)
{
    zeroMat3(m);
    m[0][0] = m[1][1] = m[2][2] = 1;
}

static INLINEDBG void identityMat4(Mat4 m)
{
    zeroMat4(m);
    m[0][0] = m[1][1] = m[2][2] = 1;
}

static INLINEDBG void identityMat44(Mat44 m)
{
    zeroMat44(m);
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
}


static INLINEDBG void addMat3(const Mat3 m1, const Mat3 m2, Mat3 r)
{
    addVec3(m1[0], m2[0], r[0]);
    addVec3(m1[1], m2[1], r[1]);
    addVec3(m1[2], m2[2], r[2]);
}

static INLINEDBG void addMat4(const Mat4 m1, const Mat4 m2, Mat4 r)
{
    addVec3(m1[0], m2[0], r[0]);
    addVec3(m1[1], m2[1], r[1]);
    addVec3(m1[2], m2[2], r[2]);
    addVec3(m1[3], m2[3], r[3]);
}

static INLINEDBG void addToMat3(const Mat3 m, Mat3 r)
{
    addToVec3(m[0], r[0]);
    addToVec3(m[1], r[1]);
    addToVec3(m[2], r[2]);
}

static INLINEDBG void addToMat4(const Mat4 m, Mat4 r)
{
    addToVec3(m[0], r[0]);
    addToVec3(m[1], r[1]);
    addToVec3(m[2], r[2]);
    addToVec3(m[3], r[3]);
}

static INLINEDBG void subMat3(const Mat3 a, const Mat3 b, Mat3 r)
{
    subVec3(a[0], b[0], r[0]);
    subVec3(a[1], b[1], r[1]);
    subVec3(a[2], b[2], r[2]);
}

static INLINEDBG void subMat4(const Mat4 a, const Mat4 b, Mat4 r)
{
    subVec3(a[0], b[0], r[0]);
    subVec3(a[1], b[1], r[1]);
    subVec3(a[2], b[2], r[2]);
    subVec3(a[3], b[3], r[3]);
}

static INLINEDBG void subFromMat3(const Mat3 m, Mat3 r)
{
    subFromVec3(m[0], r[0]);
    subFromVec3(m[1], r[1]);
    subFromVec3(m[2], r[2]);
}

static INLINEDBG void subFromMat4(const Mat4 m, Mat4 r)
{
    subFromVec3(m[0], r[0]);
    subFromVec3(m[1], r[1]);
    subFromVec3(m[2], r[2]);
    subFromVec3(m[3], r[3]);
}

static INLINEDBG void scaleByMat3(Mat3 m, float scale)
{
    scaleByVec3(m[0], scale);
    scaleByVec3(m[1], scale);
    scaleByVec3(m[2], scale);
}

static INLINEDBG void scaleByMat4(Mat4 m, float scale)
{
    scaleByVec3(m[0], scale);
    scaleByVec3(m[1], scale);
    scaleByVec3(m[2], scale);
    scaleByVec3(m[3], scale);
}

static INLINEDBG int invertMat3(const Mat3 a, Mat3 b)  
{
    return invertMat3Copy(a, b);
}

static INLINEDBG int invertMat4(const Mat4 a, Mat4 b)  
{
    invertMat4Copy(a, b);
    return 1;
}

static INLINEDBG void outerProductVec3(const Vec3 a, const Vec3 b, Mat3 r)
{
    int x,y;
    for (y = 0; y < 3; y++)
    {
        for (x = 0; x < 3; x++)
            r[x][y] = a[y] * b[x];
    }
}

static INLINEDBG void copyMat3(const Mat3 a,Mat3 b)
{
    copyVec3(a[0],b[0]);
    copyVec3(a[1],b[1]);
    copyVec3(a[2],b[2]);
}

static INLINEDBG void transposeMat4Copy(const Mat4 mat,Mat4 inv)
{
Vec3    dv;

    transposeMat3Copy(mat,inv);
    subVec3(zerovec3,mat[3],dv);
    mulVecMat3(dv,inv,inv[3]);
}

C_DECLARATIONS_END

#endif

