/**
 *  copyright:  (c) 2005, NCsoft. All Rights Reserved
 *  author(s):  Peter Freese
 *  purpose:    Math utility functions
 *  modified:   $DateTime: 2006/07/12 14:21:10 $
 *              $Author: cthurow $
 *              $Change: 273860 $
 *              @file
 */

#ifndef   INCLUDED_matUtils
#define   INCLUDED_matUtils
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cmath>
#include "matConstants.h"


template <typename T> inline const T matSquare(const T& val)
{
    return val * val;
}

template <typename T> inline const T& matClamp(const T & val,
                                               const T & min, 
                                               const T & max)
{
    if(val < min)
        return min;
    if(val > max)
        return max;
    return val;
}

template <typename T> inline const T matLerp(const T &Start,
                                             const T &End,
                                             float i)
{ 
    return Start + (End - Start) * i;
}

template < typename T> inline const T matClampedLerp(const T &Start,
                                                     const T &End,
                                                     float i)
{
    return matLerp( Start, End, matClamp(i, 0.0f, 1.0f));
}


template <typename T> inline const T matSmoothStep(const T &StartVal, const T &EndVal, float i)
{
    i = matClamp(i, 0.0f, 1.0f);
    i = i * i * (3.0f - 2.0f * i);

    return StartVal * (1.0f - i) + EndVal * i;
}


/**
 *  Bilinear interpolation. For a simple rect, this can be considered to be
 *  Upper left, upper right, lower left, lower right, h interpolant, v 
 *  interpolant.
 *
 *  @param  v0      upper left
 *  @param  v1      upper right
 *  @param  v2      lower left
 *  @param  v3      lower right
 *  @param  frac1   horizontal interpolant
 *  @param  frac2   vertical interpolant
 *   
 *  @return interpolated value
 */
template <typename T> inline const T matBilerp(    const T &v0, 
                                               const T &v1,
                                               const T &v2,
                                               const T &v3,
                                               float frac1, 
                                               float frac2 )
{
    T v01 = matLerp(v0, v1, frac1);
    T v23 = matLerp(v2, v3, frac1);
    return matLerp(v01, v23, frac2);
}


inline float matDegreesToRadians( float fDegrees )
{
    return fDegrees * matDegToRad;
}

inline float matRadiansToDegrees( float fRadians )
{
    return fRadians * matRadToDeg;
}


/**
 *  Interpolate between the two given angles, using the shortest circle
 *  distance. If the two angles span the zero crossing, the results will
 *  interpolate from negative to positive, giving a continuous result.
 *  Otherwise, all results are in the positive 0..2pi range.
 *
 *  @param  <edit>
 *   
 *  @return <edit>
 */
inline float matAngleInterp(float fThetaStart, float fThetaEnd, float fFrac)
{
    fThetaStart = fmodf(fThetaStart, mat2Pi);
    if ( fThetaStart < 0 )
        fThetaStart += mat2Pi;
    fThetaEnd = fmodf(fThetaEnd, mat2Pi);
    if ( fThetaEnd < 0 )
        fThetaEnd += mat2Pi;

    float fDelta = fmodf(fThetaEnd - fThetaStart, mat2Pi);

    if ( fDelta > matPi )
    {
        fDelta -= mat2Pi;
    }
    else if ( fDelta < -matPi )
    {
        fDelta += mat2Pi;
        if ( fThetaStart > matPi )
        {
            fThetaStart -= mat2Pi;
        }
    }

    return fThetaStart + fFrac * fDelta;
}


inline bool matIsNegative( float f )
{
#if CORE_ARCH_X86
    return ( (force_cast<int32>(f) & 0x80000000) != 0 );
#else
    return ( f < 0 );
#endif
}


/*
    | s | exponent | mantissa |
    | 1 |    8     |    23    |

    Exponent bias is 127 for normalized #s

                   Special values
    exp | unbiased | value                       | type
    255 |          |                           | Infinity or NaN
    254 |   127    |(-1)^s*(1.f1f2...)*2^127   | Normalized
     2  |  -125    |(-1)^s*(1.f1f2...)*2^-125  | Normalized
     1  |  -126    |(-1)^s*(1.f1f2...)*2^-126  | Normalized
     0  |  -126    |(-1)^s*(0.f1f2...)*2^-126  | Denormalized
*/


/**
 *  Convert a floating point number to an integer, truncation towards zero.
 *  The maximum range of this function is +/- 2^23
 *  @param  float value
 *   
 *  @return integer result
 */
inline int matFloatToInt( float f )
{
    errAssert( fabsf(f) <= 8388608.0f );

#if CORE_COMPILER_MSVC && CORE_SYSTEM_WIN32 // need some additional test to detect SSE

    int retval;
    __asm 
    {
        mov ecx, [f];
        shr ecx, 23;
        and ecx, 0xFF;
        neg ecx;
        add ecx, 150;     // ecx = number of mantissa bits to mask

        sbb eax, eax;
        mov edx, eax;
        not edx;          // eax = 0, edx == -1 iff shift < 0 (don't mask any bits)

        shl eax, cl;
        or eax, edx;     // no mask if shift < 0
        and [f], eax;
        fld [f];
        fistp retval;
    }

    return retval;
#else
    return static_cast<int>(f);
#endif
}


inline int matFloatToIntRound( float f )
{
    int retval;
#if CORE_COMPILER_MSVC && CORE_SYSTEM_WIN32
    __asm fld f
    __asm fistp retval
#elif defined(__GNUC__) && defined(_WIN32) && defined(__i386__) && !defined(_XBOX)
    __asm__ __volatile__("flds %1; fistpl %0" : "=m" (retval) : "m" (f) : "st");
#else
    if ( matIsNegative(f) )
        retval = static_cast<int>(f - 0.5f);
    else
        retval = static_cast<int>(f + 0.5f);
#endif
    return retval;
}


inline int matFloatToIntFloor( float f )
{
    return matFloatToInt(floorf(f));
}

inline int matFloatToIntCeil( float f )
{
    return matFloatToInt(ceilf(f));
}


/**
 *  Numerical equivalence test. This test uses both relative and absolute error
 *  epsilons in order to give practical tests for equivalence with floating point
 *  numbers. The default epsilons work for most operations with single precision
 *  floating point numbers, although cumulative calculations with subsequent 
 *  equivalency tests may require large epsilons.
 *  @param f1           value 1
 *  @param f2           value 2
 *  @param fRelError    The maximum fractional error allowable between the two values
 *  @param fAbsError    The maximum absolute error allowable between the two values
 *   
 *  @return     true if the difference between the values is below the threshold for absolute
 *              or relative error
 */
const float kDefaultRelError = 0.000001f;
const float kDefaultAbsError = 0.000001f;
inline bool IsEqual(float f1, float f2, 
                   float fRelError = kDefaultRelError, 
                   float fAbsError = kDefaultAbsError )
{
    float fError = fabsf(f1 - f2);
    return  fError <= fAbsError || fError <= fRelError * Max(fabsf(f1), fabsf(f2));
}


#endif // INCLUDED_matUtils
