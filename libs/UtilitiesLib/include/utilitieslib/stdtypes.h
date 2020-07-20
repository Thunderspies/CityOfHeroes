#ifndef _STDTYPES_H
#define _STDTYPES_H

// Turn this on to enable realtime memory leak detection
//#define ENABLE_LEAK_DETECTION

#ifdef __cplusplus
#    define C_DECLARATIONS_BEGIN    extern "C"{
#    define C_DECLARATIONS_END    }
#else
#    define C_DECLARATIONS_BEGIN
#    define C_DECLARATIONS_END
#endif

#include <stddef.h>
#include <stdarg.h>
#include <string.h>

//headers to make our secure CRT stuff work somewhat transparently
#include <ctype.h>
#include <io.h>
#include <direct.h>
#include <time.h>
#ifdef __cplusplus
#    include <cstdio>
#else
#    include <stdio.h>
#endif
#include <stdlib.h>
#include <conio.h>
#include <process.h>

// Need to avoid including assert.h just yet
#define _DVEC_H_INCLUDED
#define _FVEC_H_INCLUDED
#define _IVEC_H_INCLUDED
#define _interlockedbittestandset _interlockedbittestandset_is_busted
#define _interlockedbittestandreset _interlockedbittestandreset_is_busted
#define _interlockedbittestandset64 _interlockedbittestandset64_is_busted
#define _interlockedbittestandreset64 _interlockedbittestandreset64_is_busted
#include <intrin.h>
#undef _interlockedbittestandset
#undef _interlockedbittestandreset
#undef _interlockedbittestandset64
#undef _interlockedbittestandreset64
#undef _DVEC_H_INCLUDED
#undef _FVEC_H_INCLUDED
#undef _IVEC_H_INCLUDED

#if defined(FULLDEBUG)
#define INLINEDBG
#elif defined(_DEBUG)
#define INLINEDBG __forceinline
#else
#define INLINEDBG __inline
#endif

/*********** For use with STRINGS  ***************/
#define NN_STR_MAKE                    __checkReturn __notnull __post __nullterminated __post __valid
#define OPT_STR_MAKE                __checkReturn __maybenull __post __nullterminated __post __valid

#define NN_STR_GOOD                    __notnull   __pre __valid __pre __nullterminated __post __nullterminated __post __valid
#define OPT_STR_GOOD                __maybenull __pre __valid __pre __nullterminated __post __nullterminated __post __valid

#define NN_STR_FREE                    __notnull   __post __notvalid
#define OPT_STR_FREE                __maybenull __post __notvalid

/*********** For use with NON-STRINGS  ***************/
#define PTR_PRE_NOTVALID            __pre __deref __notvalid
#define PTR_POST_NOTVALID            __post __notvalid __post __deref __notvalid

#define PTR_PRE_VALID                __pre __valid __pre __deref __valid
#define PTR_PRE_VALID_COUNT(x)        __pre __valid __pre __deref __pre __elem_readableTo(x) __pre __elem_writableTo(x)
#define PTR_PRE_VALID_BYTES(x)        __pre __valid __pre __deref __pre __byte_readableTo(x) __pre __byte_writableTo(x)

#define PTR_POST_VALID                __post __valid
#define PTR_POST_VALID_COUNT(x)        PTR_POST_VALID __post __elem_readableTo(x) __post __elem_writableTo(x)
#define PTR_POST_VALID_BYTES(x)        PTR_POST_VALID __post __byte_readableTo(x) __post __byte_writableTo(x)


#define NN_PTR_MAKE                      __checkReturn __notnull PTR_PRE_NOTVALID PTR_POST_VALID
#define NN_PTR_MAKE_COUNT(x)          __checkReturn __notnull __in_ecount(x) PTR_PRE_NOTVALID PTR_POST_VALID_COUNT(x)
#define NN_PTR_MAKE_BYTES(x)          __checkReturn __notnull __in_bcount(x) PTR_PRE_NOTVALID PTR_POST_VALID_BYTES(x)

#define OPT_PTR_MAKE                  __checkReturn __maybenull PTR_PRE_NOTVALID PTR_POST_VALID
#define OPT_PTR_MAKE_COUNT(x)         __checkReturn __maybenull __in_ecount_opt(x) PTR_PRE_NOTVALID PTR_POST_VALID_COUNT(x)
#define OPT_PTR_MAKE_BYTES(x)         __checkReturn __maybenull __in_bcount_opt(x) PTR_PRE_NOTVALID PTR_POST_VALID_BYTES(x)

#define NN_PTR_GOOD                      __notnull PTR_PRE_VALID PTR_POST_VALID
#define NN_PTR_GOOD_COUNT(x)          __notnull __in_ecount(x) PTR_PRE_VALID_COUNT(x) PTR_POST_VALID
#define NN_PTR_GOOD_BYTES(x)          __notnull __in_bcount(x) PTR_PRE_VALID_BYTES(x) PTR_POST_VALID

#define OPT_PTR_GOOD                  __maybenull PTR_PRE_VALID PTR_POST_VALID
#define OPT_PTR_GOOD_COUNT(x)          __maybenull __in_ecount_opt(x) PTR_PRE_VALID_COUNT(x) PTR_POST_VALID
#define OPT_PTR_GOOD_BYTES(x)          __maybenull __in_bcount_opt(x) PTR_PRE_VALID_BYTES(x) PTR_POST_VALID

#define NN_PTR_FREE                      __notnull PTR_PRE_VALID PTR_POST_NOTVALID
#define NN_PTR_FREE_COUNT(x)          __notnull __in_ecount(x) PTR_PRE_VALID_COUNT(x) PTR_POST_NOTVALID
#define NN_PTR_FREE_BYTES(x)          __notnull __in_bcount(x) PTR_PRE_VALID_BYTES(x) PTR_POST_NOTVALID

#define OPT_PTR_FREE                  __maybenull PTR_PRE_VALID PTR_POST_NOTVALID
#define OPT_PTR_FREE_COUNT(x)         __maybenull __in_ecount_opt(x) PTR_PRE_VALID_COUNT(x) PTR_POST_NOTVALID
#define OPT_PTR_FREE_BYTES(x)         __maybenull __in_bcount_opt(x) PTR_PRE_VALID_BYTES(x) PTR_POST_NOTVALID


/*********** For use with Pointer Pointers such as EArrayHandles ***************/
#define PTR_OPT_PTR_MAKE            __pre __notnull __pre __deref __maybenull __post __deref __notnull __post __elem_readableTo(1) __post __deref __elem_readableTo(1)
#define PTR_OPT_PTR_GOOD            __pre __notnull __pre __deref __notnull __pre __deref __valid __exceptthat __deref __maybenull
#define PTR_OPT_PTR_FREE            __pre __notnull __pre __deref __maybenull __post __deref __null

#define PTR_NN_PTR_MAKE                __pre __notnull __pre __deref __notnull __post __deref __notnull __post __elem_readableTo(1) __post __deref __elem_readableTo(1)
#define PTR_NN_PTR_GOOD                __pre __notnull __pre __deref __notnull __pre __deref __valid
#define PTR_NN_PTR_FREE                __pre __notnull __pre __deref __notnull __post __deref __null

#define PTR_OPT_STR_GOOD            __pre __notnull __pre __deref __notnull __pre __deref __valid __exceptthat __deref __maybenull __pre __deref __nullterminated

#define PTR_NN_PTR_MAKE_COUNT(x)    __pre __notnull __pre __deref __notnull __post __deref __elem_readableTo(x) __post __deref __elem_writableTo(x)
#define PTR_OPT_PTR_MAKE_COUNT(x)    __pre __notnull __pre __deref __maybenull __post __deref __elem_readableTo(x) __post __deref __elem_writableTo(x) __exceptthat __deref __maybenull

/*********** Elsewhat ***************/
#define NORETURN                    __declspec(noreturn) void

// Note: *both* of these get hit during the preprocessor, once for /analyze, once for the compiler!
// Note: This is just for analysis, *not* the optimizer's __assume directive
#ifdef _PREFAST_
#    define ANALYSIS_ASSUME(expr)                (__analysis_assume(expr))
#else
#    define ANALYSIS_ASSUME(expr)                (0)
#endif


C_DECLARATIONS_BEGIN

#ifdef _WINBASE_
#error("winbase.h has already been included before stdtypes.h");
#endif

#ifndef _WIN32_WINNT
#    if defined(CLIENT) || defined(UTILITIESLIB)
#        define _WIN32_WINNT 0x0601 // Windows 7 and up
#    else
#        define _WIN32_WINNT 0x0601 // Windows 7 and up
#    endif
#else
#error("_WIN32_WINNT is already defined");
#endif

#pragma warning (disable:4244)        /* disable bogus conversion warnings */
#pragma warning (disable:4305)        /* disable bogus conversion warnings */

#pragma warning (disable:4100) // Unreferenced formal parameter
#pragma warning (disable:4211) // redefined extern to static
#pragma warning (disable:4057) // differs in indirection to slightly differen base types
#pragma warning (disable:4115) // named type definition in parentheses
#pragma warning (disable:4127) // conditional expression is constant (while (1))
#pragma warning (disable:4214) // nonstandard extension used : bit field types other than int (chars and enums)
#pragma warning (disable:4201) // nonstandard extension used : nameless struct/union
#pragma warning (disable:4055) // 'type cast' : from data pointer X to function pointer Y
#pragma warning (disable:4152) // nonstandard extension, function/data pointer conversion in expression
#pragma warning (disable:4706) // assignment within conditional expression
#pragma warning (disable:702)  // unreachable code (EXCEPTION_HANDLER_END mostly)
#pragma warning (disable:4204) // non-constant aggregate initializer
#pragma warning (disable:4221) // X: cannot be initialized using address of automatic variable Y (used in textparser stuff)
#pragma warning (disable:4505) // unreferenced local function has been removed (in cryptopp)
#pragma warning (disable:4130) // logical operation on address of string constant (assert("foo"==0))
#pragma warning (disable:4189) // X: local variable is initialized but not referenced (too many to fix)
#pragma warning (disable:4213) // nonstandard extension used : cast on l-value
#pragma warning (disable:4512) // X : assignment operator could not be generated (cryptopp, novadex)
#pragma warning (disable:4210) // nonstandard extension used : function given file scope
#pragma warning (disable:4709) // comma operator within array index expression (eaSize in arrays)
#pragma warning (disable:4131) // X : uses old-style declarator (wrongly flags macros used as parameter lists, i.e. CMDAGS)
#pragma warning (disable:4324) // X : structure was padded due to __declspec(align())
#pragma warning (disable:6255) // _alloca indicates failure by raising a stack overflow exception. Consider using _malloca instead

// Found a bug, many false positives
#pragma warning (disable:4245) // conversion from X to Y, signed/unsigned mismatch

// Should keep this on, it's finding bugs but a lot of false positives too
//#pragma warning (disable:4389) // signed/unsigned mismatch (in inequalities)
//#pragma warning (disable:4701) // potentially uninitialized local variable X used

#include <sal.h>
//#define FORMAT [SA_FormatString(Style="printf")] const char *
#define FORMAT const char *
#if !defined(_LIB) && !SECURE_STRINGS
    // disable /analyze warnings, so we can at least get printf parameter checking
    // TODO: enable these
#    pragma warning(disable:6001)
#    pragma warning(disable:6011)
#    pragma warning(disable:6031)
#    pragma warning(disable:6053)
#    pragma warning(disable:6054)
#    pragma warning(disable:6201)
#    pragma warning(disable:6202)
#    pragma warning(disable:6203)
#    pragma warning(disable:6204)
#    pragma warning(disable:6235)
#    pragma warning(disable:6237)
#    pragma warning(disable:6240)
#    pragma warning(disable:6244)
#    pragma warning(disable:6246)
#    pragma warning(disable:6262)
#    pragma warning(disable:6263)
#    pragma warning(disable:6269)
#    pragma warning(disable:6286)
#    pragma warning(disable:6297)
#    pragma warning(disable:6309)
#    pragma warning(disable:6320)
#    pragma warning(disable:6322)
#    pragma warning(disable:6326)
#    pragma warning(disable:6335)
#    pragma warning(disable:6336)
#    pragma warning(disable:6358)
#    pragma warning(disable:6385)
#    pragma warning(disable:6386)
#    pragma warning(disable:6387)
#    pragma warning(disable:6335)
#endif

#ifdef ENABLE_LEAK_DETECTION
#define GC_DEBUG
#define GC_THREADS
#define GC_NO_THREAD_DECLS
#define GC_NO_THREAD_REDIRECTS
#include "../../3rdparty/gc-7.2alpha6/include/gc.h"
#endif

#ifndef UNUSED
#    define UNUSED(xx) (xx==xx)
#endif

#ifndef MAX_PATH
#    define MAX_PATH 260
#endif

typedef unsigned char U8;
typedef signed char S8;
typedef volatile signed char VS8;
typedef volatile unsigned char VU8;

typedef unsigned short U16;
typedef short S16;
typedef volatile unsigned short VU16;
typedef volatile short VS16;

typedef int S32;
typedef unsigned int U32;
typedef volatile int VS32;
typedef volatile unsigned int VU32;

typedef unsigned __int64 U64;
typedef __int64 S64;
typedef volatile __int64 VS64;
typedef volatile unsigned __int64 VU64;

typedef float F32;
typedef volatile float VF32;

typedef volatile double VF64;
typedef double F64;

typedef ptrdiff_t ssize_t;

#define S32_MIN    (-2147483647i32 - 1) /* minimum signed 32 bit value */
#define S32_MAX      2147483647i32 /* maximum signed 32 bit value */
#define U32_MAX     0xffffffffui32 /* maximum unsigned 32 bit value */
#define U16_MAX        0xffffui16
#define U8_MAX        0xffui8

#if defined(DISABLE_SHARED_MEMORY_SEGMENT)
    #define SHARED_MEMORY
    #define SHARED_MEMORY_PARAM
    #define SERVER_SHARED_MEMORY
#else
    // TODO: We could put the memory actually into shared memory, but we need to handle locking correctly first
    //#pragma section("shared_memory", read, write, shared) 
    #pragma section("shared_memory", read, write)
    #define SHARED_MEMORY __declspec(allocate("shared_memory")) const
    #define SHARED_MEMORY_PARAM const

    #if defined(SERVER)
        #define SERVER_SHARED_MEMORY SHARED_MEMORY
    #else
        #define SERVER_SHARED_MEMORY
    #endif
#endif

#ifdef __cplusplus
    #define cpp_static_cast(_type) static_cast<_type>
    #define cpp_const_cast(_type) const_cast<_type>
    #define cpp_reinterpret_cast(_type) reinterpret_cast<_type>
    #define cpponly_static_cast(_type) static_cast<_type>
    #define cpponly_const_cast(_type) const_cast<_type>
    #define cpponly_reinterpret_cast(_type) reinterpret_cast<_type>
#else
    #define cpp_static_cast(_type) (_type)
    #define cpp_const_cast(_type) (_type)
    #define cpp_reinterpret_cast(_type) (_type)
    #define cpponly_static_cast(_type)
    #define cpponly_const_cast(_type)
    #define cpponly_reinterpret_cast(_type)
#endif

#define EXPECTED_WORST_CACHE_LINE_SIZE 64

#define _STRINGIFY(_s) #_s
#define STRINGIFY(_s) _STRINGIFY(_s)

void DebuggerPrint(const char * msg);

#ifdef FULLDEBUG
    #define TODO() do { static bool once=true; if(once) { __nop(); } else { DebuggerPrint(__FILE__ "(" STRINGIFY(__LINE__) "): TODO\n"); once=false; }} while(0,0)
#else
    #define TODO() do {} while(0,0)
#endif

#ifndef FINAL
    #define NEEDS_REVIEW() do { static bool once=true; if(!once) { __nop(); } else { DebuggerPrint(__FILE__ "(" STRINGIFY(__LINE__) "): NEEDS_REVIEW\n"); once=false; }} while(0,0)
#else
    // force compile error in FINAL to force fixing of these issues
    #define NEEDS_REVIEW() do { typedef char __NEEDS_REVIEW__[-1]; } while(0,0)
#endif

#ifndef NULL
#    define NULL ((void *)0)
#endif

#define xcase                                    break;case
#define xdefault                                break;default

#define THREADSAFE_STATIC __declspec(thread) static
#ifdef ENABLE_LEAK_DETECTION
    #define THREADSAFE_STATIC_MARK(var) GC_add_roots(&(var), (void*)(((uintptr_t)&(var)) + sizeof(var)))
#else
    #define THREADSAFE_STATIC_MARK(var) do {} while (0,0)
#endif

// not the clearest message, -1 sized array. Also, doesn't work in function scope.
#define STATIC_ASSERT(COND) typedef char __STATIC_ASSERT__[(COND)?1:-1];
#define STATIC_INFUNC_ASSERT(COND) { typedef char __STATIC_ASSERT__[(COND)?1:-1]; }

#define IS_ARRAY(a)                                ((char*)(a) == (char*)&(a))
#define ASSERT_IS_ARRAY(a)                        devassertmsg(IS_ARRAY(a), "Not an array!")
#define ARRAY_SIZE(n)                            (sizeof(n) / sizeof((n)[0]))
// ARRAY_SIZE_CHECKED causes a compile-time error if you pass it a char * instead of a char array
//  but has a false negative if it's a char[4].
#define ARRAY_SIZE_CHECKED(n)                    (sizeof(n) / ((sizeof(n)==sizeof(void*) || sizeof(n)==0)?0:sizeof((n)[0])))
#define SAFESTR(str) str, ARRAY_SIZE_CHECKED(str)
#define TYPE_ARRAY_SIZE(typeName, n)            ARRAY_SIZE(((typeName*)0)->n)
#define SIZEOF2(typeName, fieldname)            (sizeof(((typeName*)0)->fieldname))
#define SIZEOF2_DEREF(typeName, fieldname)        (sizeof(*((typeName*)0)->fieldname))
#define SIZEOF2_DEREF2(typeName, fieldname)        (sizeof(**((typeName*)0)->fieldname))
#define OFFSETOF(typeName, fieldname)            (uintptr_t)&(((typeName*)(0x0))->fieldname)
#define OFFSET_GET(p,offset,type)                    (*(type*)(((char*)p)+offset)) // get the typed element from the pointer.
#define SAFE_FREE(obj)                            if (obj) { freeConst(obj); (obj) = NULL; }
#ifdef __cplusplus
#    define SAFE_DELETE(obj)                    if (obj) { delete (obj); (obj) = NULL; }
#    define SAFE_DELETE_ARRAY(obj)                if (obj) { delete [] (obj); (obj) = NULL; }
#endif
#define SAFE_DEREF(p)                            ((p)?*(p):0)
#define SAFE_MEMBER(p, c1)                        ((p)?(p)->c1:0)
#define SAFE_MEMBER2(p, c1, c2)                    (((p)&&(p)->c1)?(p)->c1->c2:0)
#define SAFE_MEMBER3(p, c1, c2, c3)                (((p)&&(p)->c1&&(p)->c1->c2)?(p)->c1->c2->c3:0)
#define SAFE_MEMBER4(p, c1, c2, c3, c4)            (((p)&&(p)->c1&&(p)->c1->c2&&(p)->c1->c2->c3)?(p)->c1->c2->c3->c4:0)

// Handy macros for zeroing memory.

#define ZeroStruct(ptr)            memset((ptr), 0, (ptr)?sizeof(*(ptr)):0)
#define ZeroStructs(ptr,count)    memset((ptr), 0, (ptr)?sizeof(*(ptr)) * (count):0)
#define ZeroArray(x)            ASSERT_IS_ARRAY(x),memset((x), 0, sizeof(x))

#define CopyArray(dest, src)    ASSERT_IS_ARRAY(src),memmove((dest),(src),sizeof(src))

// Copy "count" # of structs.

#define CopyStructs(dest,src,count)                    memmove((dest),(src),sizeof((dest)[0]) * (count))
#define CopyStructsFromOffset(dest,offset,count)    memmove((dest),(dest)+(offset),sizeof((dest)[0]) * (count))

// calloc structs.

#define callocStruct(ptr, type)                ((1/(S32)(sizeof(*ptr)==sizeof(type))),(ptr) = (type*)calloc(1, sizeof(*(ptr))))
#define callocStructs(ptr, type, count)        ((1/(S32)(sizeof(*ptr)==sizeof(type))),(ptr) = (type*)calloc(count, sizeof(*(ptr))))

// Macro for creating a bitmask.  Example: BIT_RANGE(3, 10) = 0x7f8 (binary: 111 1111 1000).
//                                                                      bit 10-^  bit 3-^

#define BIT_RANGE(lo,hi) (((1 << ((hi) - (lo) + 1)) - 1) << (lo))

// va_start/end wrapper.

#define VA_START(va, format)    {va_list va, *__vaTemp__ = &va;va_start(va, format)
#define VA_END()                va_end(*__vaTemp__);}

// cast ints to and from pointers safely
#define U32_TO_PTR(x)    ((void*)(uintptr_t)(U32)(x))    
#define    S32_TO_PTR(x)    U32_TO_PTR(x)    // don't sign-extend
#define PTR_TO_U32(x)    ((U32)(uintptr_t)(x))
#define PTR_TO_S32(x)    ((S32)PTR_TO_U32(x))

// a bit more clunky..
#define PTR_ASSIGNFROM_F32(ptr,fl)    (*((F32*)(&ptr)) = (fl))
#define PTR_TO_F32(x)    *((F32*)(&(x)))

typedef double  DVec2[2];
typedef double  DVec3[3];
typedef double  DVec4[4];
typedef F32  Vec2[2];
typedef F32  Vec3[3];
typedef F32  Vec4[4];
typedef Vec3 Mat4[4];
typedef Vec4 Mat44[4];
typedef Vec3 Mat3[3];
typedef Vec3* Mat3Ptr;
typedef Vec3* Mat4Ptr;
typedef Vec3 const* Mat4ConstPtr;
typedef Vec4* Mat44Ptr;
typedef F32 Quat[4];

#define SIZB(bitcount) (((bitcount)+31) >> 5)
#define SETB(mem,bitnum) ((mem)[(bitnum) >> 5] |= (1 << ((bitnum) & 31)))
#define CLRB(mem,bitnum) ((mem)[(bitnum) >> 5] &= ~(1 << ((bitnum) & 31)))
#define TSTB(mem,bitnum) (!!((mem)[(bitnum) >> 5] & (1 << ((bitnum) & 31))))

typedef union SGColorBits {
    U64 SGBitSet;
    struct
    {
        U32 SGBitSetLow;
        U32 SGBitSetHigh;
    };
} SGColorBits;

#ifndef BOOL
    typedef int BOOL;
#endif

#ifndef TRUE
#    define TRUE 1
#    define FALSE 0
#endif

#ifndef __cplusplus
    #ifndef bool
        typedef S8 bool;
        #define true 1
        #define false 0
    #endif
#endif

#define streq(a, b)    (!stricmp(a,b))

int quick_sprintf(char *buffer, size_t buf_size, FORMAT format, ...);
void quick_strcatf(char *buffer, size_t buf_size, FORMAT format, ...);
int quick_snprintf(char *buffer, size_t buf_size, size_t maxlen, FORMAT format, ...);
int quick_vsprintf(char *buffer, size_t buf_size, const char *format, va_list argptr);
int quick_vsnprintf(char *buffer, size_t buf_size, size_t maxlen,const char *format, va_list argptr);

#define sprintf_s        quick_sprintf
#define strcatf_s        quick_strcatf
#define snprintf_s        quick_snprintf
#define vsprintf_s        quick_vsprintf
#define vsnprintf_s        quick_vsnprintf
#define _vsnprintf_s    quick_vsnprintf

// TODO: remove sprintf_unsafe and strcatf_unsafe
#define sprintf(buffer, format, ...)                sprintf_s(buffer, ARRAY_SIZE_CHECKED(buffer), format, __VA_ARGS__)
#define sprintf_unsafe(buffer, format, ...)            sprintf_s(buffer, 0x7fffffff, format, __VA_ARGS__)
#define strcatf(buffer, format, ...)                strcatf_s(buffer, ARRAY_SIZE_CHECKED(buffer), format, __VA_ARGS__)
#define strcatf_unsafe(buffer, format, ...)            strcatf_s(buffer, 0x7fffffff, format, __VA_ARGS__)
#define snprintf(buffer, maxlen, format, ...)        snprintf_s(buffer, ARRAY_SIZE_CHECKED(buffer), maxlen, format, __VA_ARGS__)
#define vsprintf(buffer, format, argptr)            vsprintf_s(buffer, ARRAY_SIZE_CHECKED(buffer), format, argptr)
#define vsnprintf(buffer, maxlen, format, argptr)    vsnprintf_s(buffer, ARRAY_SIZE_CHECKED(buffer), maxlen, format, argptr)
#define _vsnprintf(buffer, maxlen, format, argptr)    _vsnprintf_s(buffer, ARRAY_SIZE_CHECKED(buffer), maxlen, format, argptr)

int printf_timed(FORMAT format, ...);
int _vscprintf_timed(const char *format, va_list argptr);
int sprintf_timed(char *buffer, size_t sizeOfBuffer, FORMAT format, ...);
int vsprintf_timed(char *buffer, size_t sizeOfBuffer,const char *format, va_list argptr);
#define printf printf_timed
#define _vscprintf _vscprintf_timed
int pprintf(int predicate, FORMAT format, ...); // print if predicate is true
int pprintfv(int predicate, const char *format, va_list argptr);
#if 0
#define sprintf sprintf_timed
#define vsprintf vsprintf_timed
#endif

// Overrides of insecure string functions
#if defined(_LIB)|SECURE_STRINGS
#    define strcpy(dst, src) strcpy_s(dst, ARRAY_SIZE_CHECKED(dst), src)
#    define strncpy(dst, src, count) strncpy_s(dst, ARRAY_SIZE_CHECKED(dst), src, count)
#    define strcat(dst, src) strcat_s(dst, ARRAY_SIZE_CHECKED(dst), src)
#    define strncat(dst, src, count) strncat_s(dst, ARRAY_SIZE_CHECKED(dst), src, count)
//#    define sprintf(dst, fmt, ...) sprintf_s(dst, ARRAY_SIZE_CHECKED(dst), fmt, __VA_ARGS__)
#    define sscanf(buf, fmt, ...) sscanf_s(buf, fmt, __VA_ARGS__)
#    define swscanf(buf, fmt, ...) swscanf_s(buf, fmt, __VA_ARGS__)
#    define gets(buf) gets_s(buf, ARRAY_SIZE_CHECKED(buf))
#    define _strupr(str) _strupr_s(str, ARRAY_SIZE_CHECKED(str))

#    define wcstombs(dst, src, count) wcstombs_s(NULL, dst, ARRAY_SIZE_CHECKED(dst), src, count)

#    define _strdate(str) _strdate_s(str, ARRAY_SIZE_CHECKED(str))
#    define _strtime(str) _strtime_s(str, ARRAY_SIZE_CHECKED(str))

#    define itoa(value, str, radix) _itoa_s(value, str, ARRAY_SIZE_CHECKED(str), radix)
#    define _itoa(value, str, radix) _itoa_s(value, str, ARRAY_SIZE_CHECKED(str), radix)
#    define _ltoa(value, str, radix) _ltoa_s(value, str, ARRAY_SIZE_CHECKED(str), radix)
#else
#    define itoa _itoa // to get VS2005 ISO compliancy complaining out of the way for functions that also have a secure version
#    undef sprintf
#    undef strcatf
#    define sprintf sprintf_unsafe
#    define strcatf strcatf_unsafe
// Can't use these yet, too much code to fix!
#endif
#define Strcpy(dst, src) strcpy_s(dst, ARRAY_SIZE_CHECKED(dst), src)
#define Strcat(dst, src) strcat_s(dst, ARRAY_SIZE_CHECKED(dst), src)

#if _MSC_VER < 1400
    // defined in utils.c
    typedef int errno_t;
    errno_t strcpy_s(char *dst, size_t dst_size, const char *src);
    errno_t strcat_s(char *dst, size_t dst_size, const char *src);
    errno_t strncpy_s(char* dst, size_t dst_size, const char* str, size_t count);

    // old names
#    define __time32_t time_t
#    define _time32 time
#    define _ctime32 ctime
#    define _finddata32_t _finddata_t
#    define _findfirst32 _findfirst
#    define _findnext32 _findnext
#    define _stat32 stat
#    define _mktime32 mktime

#else
    // VS2005 wants to use ISO compliant names for functions...
    int open_cryptic(const char* filename, int oflag);
#    define open open_cryptic
#    define mkdir _mkdir
#    define rmdir _rmdir
#    define chdir _chdir
#    define close _close
#    define unlink _unlink
#    define chmod _chmod
#    define getch _getch
#    define kbhit _kbhit
#    define strlwr _strlwr
#    define strcmpi _strcmpi
#    define getpid _getpid
#    define wcsicmp _wcsicmp
#endif

#define fopen include_file_h_for_fopen
#define _beginthreadex include_utils_h_for_threads
#define _beginthread include_utils_h_for_threads
#define CreateThread include_utils_h_for_threads

#ifndef __cplusplus

// *************************************************************************
// AUTO_COMMAND, AUTO_ENUM and AUTO_STRUCT are special tokens looked for 
// by StructParser
// *************************************************************************


// *************************************************************************
// AUTO_COMMAND
// *************************************************************************

#define AUTO_COMMAND typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
#define AUTO_COMMAND_REMOTE typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
#define AUTO_COMMAND_REMOTE_SLOW(x)
#define AUTO_COMMAND_QUEUED(...)
#define ACMD_ACCESSLEVEL(x)
#define ACMD_APPSPECIFICACCESSLEVEL(globaltype,x)
#define ACMD_CLIENTCMD
#define ACMD_SERVERCMD
#define ACMD_EARLYCOMMANDLINE
#define ACMD_NAME(...)
#define ACMD_SENTENCE char*
#define ACMD_CATEGORY(...)
#define ACMD_LIST(...)
#define ACMD_HIDE
#define ACMD_I_AM_THE_ERROR_FUNCTION_FOR(x)
#define ACMD_POINTER
#define ACMD_FORCETYPE(x)
#define ACMD_IFDEF(x)
#define ACMD_MAXVALUE(x)
#define ACMD_PRIVATE
#define ACMD_SERVERONLY
#define ACMD_NOTESTCLIENT
#define ACMD_CLIENTONLY
#define ACMD_GLOBAL
#define ACMD_TESTCLIENT
#define ACMD_NAMELIST(...)
#define ACMD_IGNORE

#define ACMD_COMMANDLINE //these two things mean the same, to be both consisent with EARLYCOMMANDLINE and abbreviation-y
#define ACMD_CMDLINE

// namelist types
#define STATICDEFINE
#define REFDICTIONARY
#define STASHTABLE
#define COMMANDLIST

#define AUTO_CMD_INT(varName, commandName) extern void AutoGen_RegisterAutoCmd_##commandName(void *pVarAddress, int iSize);\
int AutoGen_AutoRun_RegisterAutoCmd_##commandName(void)\
{\
AutoGen_RegisterAutoCmd_##commandName(&(varName), sizeof(varName));\
return 0;\
};\
STATIC_ASSERT(sizeof(varName) == sizeof(U8) || sizeof(varName) == sizeof(U32) || sizeof(varName) == sizeof(U64));

#define AUTO_CMD_FLOAT(varName, commandName) extern void AutoGen_RegisterAutoCmd_##commandName(float *pVarAddress, int iSize);\
int AutoGen_AutoRun_RegisterAutoCmd_##commandName(void)\
{\
AutoGen_RegisterAutoCmd_##commandName(&(varName), sizeof(varName));\
return 0;\
};\
STATIC_ASSERT(sizeof(varName) == sizeof(F32));

#define AUTO_CMD_STRING(varName, commandName) extern void AutoGen_RegisterAutoCmd_##commandName(void *pVarAddress, int iSize);\
int AutoGen_AutoRun_RegisterAutoCmd_##commandName(void)\
{\
AutoGen_RegisterAutoCmd_##commandName((varName), sizeof(varName));\
return 0;\
};\
STATIC_ASSERT(sizeof(varName) != 4 && sizeof(varName) != 8);

#define AUTO_CMD_SENTENCE(varName, commandName) extern void AutoGen_RegisterAutoCmd_##commandName(void *pVarAddress, int iSize);\
int AutoGen_AutoRun_RegisterAutoCmd_##commandName(void)\
{\
AutoGen_RegisterAutoCmd_##commandName((varName), sizeof(varName));\
return 0;\
};\
STATIC_ASSERT(sizeof(varName) != 4 && sizeof(varName) != 8);

#endif //__cplusplus

// *************************************************************************
//  AUTO_ENUM
// *************************************************************************
#define AUTO_ENUM typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
#define ENAMES(x)
#define EIGNORE
#define AEN_NO_PREFIX_STRIPPING
#define AEN_APPEND_TO(x)
// #define AEN_WIKI(x)

// *************************************************************************
//  AUTO_STRUCT. Automatically generate run-time type info.
// Here are a series of AUTO_STRUCT examples:
/*
AUTO_STRUCT;
typedef struct Baz
{
    int field; AST(STRUCTPARAM)
} Baz;

AUTO_STRUCT AST_STARTTOK("") AST_ENDTOK("End");
typedef struct Foo
{
    char *name;         AST(STRUCTPARAM)
    Baz  **ppBazzies;   AST(NAME("Bazzie"))    // override parse name,
    char *filename;     AST(CURRENTFILE)        // get src file
    U32  num_chances;    AST(DEFAULT(5))         // default value
    U32  lastActive;    AST(FORMAT_DATESS2000)  // date -- friendlyss2000???
    Baz *baz1;  // let auto struct do the work for you
    Baz *baz2;  AST(STRUCT(parse_Baz))   // optional struct with TPI
    Baz *baz3;  AST(SUBTABLE(parse_Baz)) // exact same thing...
} Foo;

// foo.def : parsable by this
// assume Foo's struct name is Foo
Foo foo_name
    Bazzie 1
    Bazzie 2
    num_chances 0
    baz1 4
    baz2 5
    baz3 6
End
// end foo.def
*/
// *************************************************************************
/*Information about AUTO_STRUCT is located here:
http://code:8081/display/cs/AUTO_STRUCT
with full reference here:
http://code:8081/display/cs/AUTO_STRUCT+reference
*/
#define AUTO_STRUCT typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
#define AST(...)        // field-specific auto struct command
#define AST_NOT(...)
#define AST_FOR_ALL(...)
#define NO_AST
#define AST_IGNORE(...)
#define AST_IGNORE_STRUCTPARAM(...)
#define AST_START
#define AST_STOP
#define AST_STARTTOK(...)
#define AST_ENDTOK(...)
#define AST_SINGLELINE    // short hand for AST_STARTTOK("") AST_ENDTOK("\n") and marking all fields STRUCTPARAM
#define AST_COMMAND(...)
#define AST_FIXUPFUNC(...)
#define AST_STRIP_UNDERSCORES
#define AST_NONCONST_PREFIXSUFFIX(...)
#define WIKI(...)
#define AST_MACRO(...)
#define AST_PREFIX(...)
#define AST_SUFFIX(...)

// *************************************************************************
//  AUTO_TRANSACTION : not supported
// // *************************************************************************
// #define AUTO_TRANSACTION typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
// #define ATR_BEGIN {
// #define ATR_END }
// #define ATR_ARGS char **autoTransResultString
// #define ATR_RECURSE autoTransResultString
// #define ATR_RESULT autoTransResultString
// #define AST_NO_PREFIX_STRIP
// #define AST_CONTAINER
// #define AST_FORCE_USE_ACTUAL_FIELD_NAME
// #define AUTO_RUN_ANON(...) typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop

// #define AUTO_TRANS_HELPER typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
// #define ATH_ARG

// *************************************************************************
//  AutoStruct field tags.
// *************************************************************************

#define NAME(...)
#define ADDNAMES(...)
#define REDUNDANT
#define STRUCTPARAM
#define DEFAULT(...)
#define SUBTABLE(...)
#define STRUCT(...)
#define FORMAT_IP
#define FORMAT_KBYTES
#define FORMAT_FRIENDLYDATE
#define FORMAT_FRIENDLYSS2000
#define FORMAT_FRIENDLYCPU
#define FORMAT_PERCENT
#define FORMAT_NOPATH
#define FORMAT_HSV
#define FORMAT_TEXTURE
#define FORMAT_UNSIGNED
#define FORMAT_DATESS2000
#define FORMAT_LVWIDTH(...)
#define MINBITS(...)
#define PRECISION(...)
#define FILENAME
#define TIMESTAMP
#define LINENUM
#define FLAGS
#define BOOLFLAG
#define USEDFIELD(...)
#define RAW
#define POINTER(...)
#define VEC2
#define VEC3
#define RG
#define RGBA
#define EMBEDDED_FLAT
#define REDUNDANT_STRUCT(...)
#define POOL_STRING
#define CURRENTFILE
#define PRIMARY_KEY
#define AUTO_INCREMENT

  // ----------
  // defintely not supported yet

// #define INDEX(...)
// #define AUTO_INDEX(...)
// #define USERFLAG(...)
// #define REFDICT(...)
// #define CLIENT_ONLY
// #define SERVER_ONLY
// #define NO_TRANSACT
// #define PERSIST
// #define NO_TRANSLATE
// #define VOLATILE_REF
// #define STRUCT_NORECURSE
// #define KEY
// #define INDEX_DEFINE
// #define ALWAYS_ALLOC
// #define LATEBIND
// #define FORCE_CONTAINER
// #define FORCE_NOT_CONTAINER
// #define WIKILINK
// #define REQUIRED
// #define NON_NULL_REF
// #define NO_WRITE
// #define NO_NETSEND
// #define NO_INDEX
// #define POLYPARENTTYPE
// #define POLYCHILDTYPE(...)
// #define UNOWNED
// #define SELF_ONLY
// #define NO_TEXT_SAVE

// #define LATELINK typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop

// *************************************************************************
// AUTO_RUN : run things automatically. don't use, it always leads to confusion
//      (works okay in libraries, sometimes)
// *************************************************************************

#define AUTO_RUN typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
#define AUTO_RUN_EARLY typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
#define AUTO_RUN_LATE typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
#define AUTO_RUN_FIRST typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
#define AUTO_FIXUPFUNC typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop

// This is called to run the auto runs. This is slightly less magical, but simpler, ab: handled by structparser
void do_auto_runs(void);
#define DO_AUTO_RUNS do_auto_runs();


// *************************************************************************
//  AUTO_TEST : not ported yet
// *************************************************************************

// #define AUTO_TEST(...) typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
// #define AUTO_TEST_CHILD(...) typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
// #define AUTO_TEST_GROUP(...) typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop
// #define AUTO_TEST_BLOCK(...) typedef int happyLongDummyMeaninglessNameThatMeansNothingLoopDeLoop

#ifdef _M_X64
#    define _DbgBreak() __debugbreak()
#else
#    define _DbgBreak() __asm { int 3 }
#endif

C_DECLARATIONS_END

#endif
