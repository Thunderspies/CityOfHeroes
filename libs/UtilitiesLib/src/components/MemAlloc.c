#define MEMALLOC_C
#include <string.h>
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/file.h"
#include <stdio.h>
#include <limits.h>
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/mathutil.h"
#include <time.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/MemoryMonitor.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/utils.h"

#ifdef ENABLE_LEAK_DETECTION
#pragma comment(lib, "../3rdparty/gc-7.2alpha6/gc.lib")
#elif !defined(_DEBUG)
#define ENABLE_SMALL_ALLOC
#endif

//////////////////////////////////////////////////////////////////////////
// CRT memory fill values

#define MALLOC_UNINITIALIZED_VALUE        0xCDCDCDCD
#define MALLOC_NOMANSLAND_VALUE            0xFDFDFDFD
#define FREE_VALUE                        0xDDDDDDDD

#define HEAPALLOC_UNINITIALIZED_VALUE    0xBAADF00D
#define HEAPFREE_VALUE                    0xFEEEFEEE


//////////////////////////////////////////////////////////////////////////
// SmallAlloc memory fill values

#define SA_UNDERRUN_VALUE                0xA110C0DE
#define SA_OVERRUN_VALUE                0x2BADBEEF
#define SA_UNINITIALIZED_VALUE            0xACACACAC

//////////////////////////////////////////////////////////////////////////
// Other Fill Values

#define MEMORY_POOL_FREED                0xFAFAFAFA

//////////////////////////////////////////////////////////////////////////

// TODO: use new pools for all process, not just the client
#ifdef CLIENT
#define USE_NEW_POOLS
#endif

//////////////////////////////////////////////////////////////////////////

volatile LONG g_inside_pool_malloc = 0;

void assertOnAllocError(const char* filename, int linenumber, const char* format, ...){
    char buffer[300];
    char* pos = buffer;
    
    pos += sprintf_s(pos, sizeof(buffer), "(%s:%d) ", filename, linenumber);
    VA_START(va, format);
    vsprintf_s(pos, buffer + sizeof(buffer) - pos, format, va);
    VA_END();
    assertmsg(0, buffer);
}

void* tls_zero_malloc(size_t size)
{
#ifdef ENABLE_LEAK_DETECTION
    return GC_malloc_uncollectable(size);
#else
    return calloc(1, size);
#endif
}


#if _CRTDBG_MAP_ALLOC

#undef _malloc_dbg
#undef _calloc_dbg
#undef _realloc_dbg
#undef _strdup_dbg
#undef _free_dbg
#undef _aligned_malloc_dbg
#undef _aligned_realloc_dbg
#undef _aligned_free_dbg

#ifdef ENABLE_LEAK_DETECTION
#define _malloc_dbg(_Size, _BlockType, _Filename, _LineNumber) GC_debug_malloc(_Size, _Filename, _LineNumber)
#define _calloc_dbg(_NumOfElements, _SizeOfElements, _BlockType, _Filename, _LineNumber) GC_debug_malloc(_NumOfElements*_SizeOfElements, _Filename, _LineNumber)
#define _realloc_dbg(_Memory, _NewSize, _BlockType, _Filename, _LineNumber) GC_debug_realloc(_Memory, _NewSize, _Filename, _LineNumber)
#define _strdup_dbg(_Str, _BlockType, _Filename, _LineNumber) GC_debug_strdup(_Str, _Filename, _LineNumber)
#define _free_dbg(_Memory, _BlockType) GC_debug_free(_Memory)
#elif defined(__MINGW32__)
#define _malloc_dbg(_Size, _BlockType, _Filename, _LineNumber) malloc(_Size)
#define _calloc_dbg(_NumOfElements, _SizeOfElements, _BlockType, _Filename, _LineNumber) calloc((_NumOfElements), (_SizeOfElements))
#define _realloc_dbg(_Memory, _NewSize, _BlockType, _Filename, _LineNumber) realloc((_Memory), (_NewSize))
#define _strdup_dbg(_Str, _BlockType, _Filename, _LineNumber) _strdup(_Str)
#define _free_dbg(_Memory, _BlockType) free(_Memory)
#define _aligned_malloc_dbg(_Size, _Alignment, _Filename, _LineNumber) _aligned_malloc((_Size), (_Alignment))
#define _aligned_realloc_dbg(_Memory, _NewSize, _Alignment, _Filename, _LineNumber) _aligned_realloc((_Memory), (_NewSize), (_Alignment))
#define _aligned_free_dbg(_Memory) _aligned_free(_Memory)
#endif

#ifdef ENABLE_SMALL_ALLOC
static CRITICAL_SECTION malloc_pool_critsec;
static int malloc_pool_critsec_init = 0;

typedef struct SmallAllocHeader
{
    U32 poolsize;
    U32 underrun;
} SmallAllocHeader;

typedef struct SmallAllocFooter
{
    U32 overrun;
} SmallAllocFooter;

#define MAX_POOL_SIZE 512
STATIC_ASSERT(MAX_POOL_SIZE < MEMALLOC_NOT_SMALLOC);
#define SA_COMPACT_LIMIT 0.75f
#define SA_COMPACT_FREQ 500

#define SA_NAME(size) SmallAlloc##size
#define SAMP_NAME(size) memPoolSmallAlloc##size

#define SA_ALIGNMENT_BUFFER_SIZE (16 - sizeof(SmallAllocHeader) - sizeof(SmallAllocFooter))

#define TYPEDEF_SMALLALLOC(size)    typedef struct SA_NAME(size)                \
                                    {                                            \
                                        SmallAllocHeader header;                \
                                        U8 data[size];                            \
                                        SmallAllocFooter footer;                \
                                        U8 aligner[SA_ALIGNMENT_BUFFER_SIZE];    \
                                    } SA_NAME(size);                            \
                                    MemoryPool SAMP_NAME(size) = 0;

#define CREATE_SMALLALLOC(size)        SAMP_NAME(size) = createMemoryPoolNamed("SmallAlloc" #size,    \
                                                                            __FILE__,            \
                                                                            __LINE__);            \
                                    initMemoryPoolOffset(    SAMP_NAME(size),                    \
                                                            sizeof(SA_NAME(size)),                \
                                                            POOL##size##_INITSIZE,                \
                                                            sizeof(SmallAllocHeader));            \
                                    mpSetMode(SAMP_NAME(size), TurnOffAllFeatures);                \
                                    mpSetCompactParams(    SAMP_NAME(size),                        \
                                                        SA_COMPACT_FREQ,                        \
                                                        SA_COMPACT_LIMIT);

//#define POOL8_INITSIZE 4096
//#define POOL16_INITSIZE 4096
//#define POOL32_INITSIZE 4096
//#define POOL64_INITSIZE 4096
//#define POOL128_INITSIZE 4096


#ifdef USE_NEW_POOLS
// It doesn't make sense to make all of the init sizes the same.
// In general, there will be lots of smaller allocations.
// The init size is also used for growing the pool.
#define POOL8_INITSIZE 1024
#define POOL12_INITSIZE 1024
#define POOL16_INITSIZE 1024
#define POOL20_INITSIZE 1024
#define POOL24_INITSIZE 1024
#define POOL28_INITSIZE 1024
#define POOL32_INITSIZE 512
#define POOL40_INITSIZE 512
#define POOL48_INITSIZE 512
#define POOL56_INITSIZE 512
#define POOL64_INITSIZE 256
#define POOL80_INITSIZE 256
#define POOL96_INITSIZE 256
#define POOL112_INITSIZE 256
#define POOL128_INITSIZE 128
#define POOL160_INITSIZE 128
#define POOL192_INITSIZE 128
#define POOL224_INITSIZE 128
#define POOL256_INITSIZE 64
#define POOL320_INITSIZE 64
#define POOL384_INITSIZE 64
#define POOL448_INITSIZE 64
#define POOL512_INITSIZE 64

TYPEDEF_SMALLALLOC(8)
TYPEDEF_SMALLALLOC(12)
TYPEDEF_SMALLALLOC(16)
TYPEDEF_SMALLALLOC(20)
TYPEDEF_SMALLALLOC(24)
TYPEDEF_SMALLALLOC(28)
TYPEDEF_SMALLALLOC(32)
TYPEDEF_SMALLALLOC(40)
TYPEDEF_SMALLALLOC(48)
TYPEDEF_SMALLALLOC(56)
TYPEDEF_SMALLALLOC(64)
TYPEDEF_SMALLALLOC(80)
TYPEDEF_SMALLALLOC(96)
TYPEDEF_SMALLALLOC(112)
TYPEDEF_SMALLALLOC(128)
TYPEDEF_SMALLALLOC(160)
TYPEDEF_SMALLALLOC(192)
TYPEDEF_SMALLALLOC(224)
TYPEDEF_SMALLALLOC(256)
TYPEDEF_SMALLALLOC(320)
TYPEDEF_SMALLALLOC(384)
TYPEDEF_SMALLALLOC(448)
TYPEDEF_SMALLALLOC(512)

#else // not USE_NEW_POOLS

#define POOL8_INITSIZE 1024
#define POOL16_INITSIZE 1024
#define POOL32_INITSIZE 1024
#define POOL64_INITSIZE 1024
#define POOL128_INITSIZE 1024
#define POOL192_INITSIZE 1024
#define POOL256_INITSIZE 1024
#define POOL384_INITSIZE 1024
#define POOL512_INITSIZE 1024

TYPEDEF_SMALLALLOC(8)
TYPEDEF_SMALLALLOC(16)
TYPEDEF_SMALLALLOC(32)
TYPEDEF_SMALLALLOC(64)
TYPEDEF_SMALLALLOC(128)
TYPEDEF_SMALLALLOC(192)
TYPEDEF_SMALLALLOC(256)
TYPEDEF_SMALLALLOC(384)
TYPEDEF_SMALLALLOC(512)
#endif

#define ALLOC_SMALLALLOC(size)            { \
                                            SA_NAME(size) *alloc; \
                                            alloc = mpAlloc_dbg(SAMP_NAME(size), filename, linenumber); \
                                            alloc->header.poolsize = size; \
                                            alloc->header.underrun = SA_UNDERRUN_VALUE; \
                                            alloc->footer.overrun = SA_OVERRUN_VALUE; \
                                            memset(alloc->data, fillvalue, size); \
                                            return &alloc->data; \
                                        }

#define FREE_SMALLALLOC(size, header)    { \
                                            SA_NAME(size) *alloc = (SA_NAME(size)*)header; \
                                            assertmsg(alloc->footer.overrun == SA_OVERRUN_VALUE, "Buffer overrun detected"); \
                                            mpFree(SAMP_NAME(size), header); \
                                        }


static INLINEDBG void *mallocPoolMemory(size_t size, int fillvalue, const char *filename, int linenumber)
{
    if (!SAMP_NAME(8))
    {
#ifdef USE_NEW_POOLS
        CREATE_SMALLALLOC(8)
        CREATE_SMALLALLOC(12)
        CREATE_SMALLALLOC(16)
        CREATE_SMALLALLOC(20)
        CREATE_SMALLALLOC(24)
        CREATE_SMALLALLOC(28)
        CREATE_SMALLALLOC(32)
        CREATE_SMALLALLOC(40)
        CREATE_SMALLALLOC(48)
        CREATE_SMALLALLOC(56)
        CREATE_SMALLALLOC(64)
        CREATE_SMALLALLOC(80)
        CREATE_SMALLALLOC(96)
        CREATE_SMALLALLOC(112)
        CREATE_SMALLALLOC(128)
        CREATE_SMALLALLOC(160)
        CREATE_SMALLALLOC(192)
        CREATE_SMALLALLOC(224)
        CREATE_SMALLALLOC(256)
        CREATE_SMALLALLOC(320)
        CREATE_SMALLALLOC(384)
        CREATE_SMALLALLOC(448)
        CREATE_SMALLALLOC(512)
#else // not USE_NEW_POOLS
        CREATE_SMALLALLOC(8)
        CREATE_SMALLALLOC(16)
        CREATE_SMALLALLOC(32)
        CREATE_SMALLALLOC(64)
        CREATE_SMALLALLOC(128)
        CREATE_SMALLALLOC(192)
        CREATE_SMALLALLOC(256)
        CREATE_SMALLALLOC(384)
        CREATE_SMALLALLOC(512)
#endif
    }

#define MEMALLOC_ALLOC(mp_size) if (size <= mp_size) ALLOC_SMALLALLOC(mp_size)

#ifdef USE_NEW_POOLS
    MEMALLOC_ALLOC(8)
    MEMALLOC_ALLOC(12)
    MEMALLOC_ALLOC(16)
    MEMALLOC_ALLOC(20)
    MEMALLOC_ALLOC(24)
    MEMALLOC_ALLOC(28)
    MEMALLOC_ALLOC(32)
    MEMALLOC_ALLOC(40)
    MEMALLOC_ALLOC(48)
    MEMALLOC_ALLOC(56)
    MEMALLOC_ALLOC(64)
    MEMALLOC_ALLOC(80)
    MEMALLOC_ALLOC(96)
    MEMALLOC_ALLOC(112)
    MEMALLOC_ALLOC(128)
    MEMALLOC_ALLOC(160)
    MEMALLOC_ALLOC(192)
    MEMALLOC_ALLOC(224)
    MEMALLOC_ALLOC(256)
    MEMALLOC_ALLOC(320)
    MEMALLOC_ALLOC(384)
    MEMALLOC_ALLOC(448)
    MEMALLOC_ALLOC(512)
#else // not USE_NEW_POOLS
    MEMALLOC_ALLOC(8)
    MEMALLOC_ALLOC(16)
    MEMALLOC_ALLOC(32)
    MEMALLOC_ALLOC(64)
    MEMALLOC_ALLOC(128)
    MEMALLOC_ALLOC(192)
    MEMALLOC_ALLOC(256)
    MEMALLOC_ALLOC(384)
    MEMALLOC_ALLOC(512)
#endif

#undef MEMALLOC_ALLOC

    assertmsg(0, "Bad pool size passed to mallocPoolMemory!");
    return 0;
}

static INLINEDBG SmallAllocHeader *getSAHeader(void *memory)
{
    SmallAllocHeader *header = (SmallAllocHeader *)memory - 1;
    if (header->underrun != SA_UNDERRUN_VALUE)
        return 0;
    return header;
}

static INLINEDBG void freePoolMemory(SmallAllocHeader *header)
{
#define MEMALLOC_FREE(mp_size) if (header->poolsize == mp_size) { FREE_SMALLALLOC(mp_size, header) return; }

#ifdef USE_NEW_POOLS
    MEMALLOC_FREE(8)
    MEMALLOC_FREE(12)
    MEMALLOC_FREE(16)
    MEMALLOC_FREE(20)
    MEMALLOC_FREE(24)
    MEMALLOC_FREE(28)
    MEMALLOC_FREE(32)
    MEMALLOC_FREE(40)
    MEMALLOC_FREE(48)
    MEMALLOC_FREE(56)
    MEMALLOC_FREE(64)
    MEMALLOC_FREE(80)
    MEMALLOC_FREE(96)
    MEMALLOC_FREE(112)
    MEMALLOC_FREE(128)
    MEMALLOC_FREE(160)
    MEMALLOC_FREE(192)
    MEMALLOC_FREE(224)
    MEMALLOC_FREE(256)
    MEMALLOC_FREE(320)
    MEMALLOC_FREE(384)
    MEMALLOC_FREE(448)
    MEMALLOC_FREE(512)
#else // not USE_NEW_POOLS
    MEMALLOC_FREE(8)
    MEMALLOC_FREE(16)
    MEMALLOC_FREE(32)
    MEMALLOC_FREE(64)
    MEMALLOC_FREE(128)
    MEMALLOC_FREE(192)
    MEMALLOC_FREE(256)
    MEMALLOC_FREE(384)
    MEMALLOC_FREE(512)
#endif

    assertmsg(0, "Bad pool size passed to freePoolMemory!");

#undef MEMALLOC_FREE
}

#define MEM(header) (((SA_NAME(8) *)header)->data)
static INLINEDBG void *reallocPoolMemory(SmallAllocHeader *header, size_t newSize, int blockType, const char *filename, int linenumber)
{
    void *newMemory = 0;

    if (newSize <= header->poolsize)
    {
        // no need to reallocate
        return MEM(header);
    }

    if ((InterlockedIncrement(&g_inside_pool_malloc)==1) && newSize <= MAX_POOL_SIZE)
    {
        newMemory = mallocPoolMemory(newSize, SA_UNINITIALIZED_VALUE, filename, linenumber);
    }

    if (!newMemory)
    {
        newMemory = (void*)_malloc_dbg(newSize, blockType, filename, linenumber);
        memtrack_alloc(newMemory, newSize);
    }

    if (newMemory)
    {
        memcpy(newMemory, MEM(header), header->poolsize);
        freePoolMemory(header);
    }
    InterlockedDecrement(&g_inside_pool_malloc);
    return newMemory;
}

typedef struct SMACUserData {
    int ret;
    U32 poolsize;
} SMACUserData;

#define FOOTER(header, poolsize) ((SmallAllocFooter*)(((char*)MEM(header)) + poolsize))
static void smallAllocCheckAlloc(MemoryPool pool, void *data, SMACUserData *userData)
{
    SA_NAME(8) *alloc = (SA_NAME(8)*)data;
    if (alloc->header.poolsize != userData->poolsize) {
        OutputDebugStringf("Memory corrupt off beginning of small alloc of pool size %d allocated at 0x%08X\n",
            userData->poolsize,
            &alloc->data);
        userData->ret = 0;
    }
    if (alloc->header.underrun != SA_UNDERRUN_VALUE) {
        OutputDebugStringf("Memory corrupt off beginning of small alloc of pool size %d allocated at 0x%08X\n",
            userData->poolsize,
            &alloc->data);
        userData->ret = 0;
    }
    if (FOOTER(alloc, userData->poolsize)->overrun != SA_OVERRUN_VALUE) {
        OutputDebugStringf("Memory corrupt off end of small alloc of pool size %d allocated at 0x%08X\n",
            userData->poolsize,
            &alloc->data);
        userData->ret = 0;
    }
}

#undef MEM

int smallAllocCheckMemory(void)
{
    int ret = 1;
    SMACUserData temp_data;

    if ((InterlockedIncrement(&g_inside_pool_malloc)==1))
    {
        if (!malloc_pool_critsec_init)
        {
            InitializeCriticalSectionAndSpinCount(&malloc_pool_critsec, 4000);
            malloc_pool_critsec_init = 1;
        }
        mmCRTHeapLock();
        EnterCriticalSection(&malloc_pool_critsec);

#define CHECK_SMALLALLOC(size)                                                    \
            ret &= mpVerifyFreelist(SAMP_NAME(size));                                \
            temp_data.ret = 1;                                                        \
            temp_data.poolsize = size;                                                \
            mpForEachAllocation(SAMP_NAME(size), smallAllocCheckAlloc, &temp_data);    \
            ret &= temp_data.ret;

#ifdef USE_NEW_POOLS
        CHECK_SMALLALLOC(8);
        CHECK_SMALLALLOC(12);
        CHECK_SMALLALLOC(16);
        CHECK_SMALLALLOC(20);
        CHECK_SMALLALLOC(24);
        CHECK_SMALLALLOC(28);
        CHECK_SMALLALLOC(32);
        CHECK_SMALLALLOC(40);
        CHECK_SMALLALLOC(48);
        CHECK_SMALLALLOC(56);
        CHECK_SMALLALLOC(64);
        CHECK_SMALLALLOC(80);
        CHECK_SMALLALLOC(96);
        CHECK_SMALLALLOC(112);
        CHECK_SMALLALLOC(128);
        CHECK_SMALLALLOC(160);
        CHECK_SMALLALLOC(192);
        CHECK_SMALLALLOC(224);
        CHECK_SMALLALLOC(256);
        CHECK_SMALLALLOC(320);
        CHECK_SMALLALLOC(384);
        CHECK_SMALLALLOC(448);
        CHECK_SMALLALLOC(512);
#else // not USE_NEW_POOLS
        CHECK_SMALLALLOC(8);
        CHECK_SMALLALLOC(16);
        CHECK_SMALLALLOC(32);
        CHECK_SMALLALLOC(64);
        CHECK_SMALLALLOC(128);
        CHECK_SMALLALLOC(192);
        CHECK_SMALLALLOC(256);
        CHECK_SMALLALLOC(384);
        CHECK_SMALLALLOC(512);
#endif

#undef CHECK_SMALLALLOC

        LeaveCriticalSection(&malloc_pool_critsec);
        mmCRTHeapUnlock();
    }

    InterlockedDecrement(&g_inside_pool_malloc);

    return ret;
}
#else
int smallAllocCheckMemory(void)
{
    return 1;
}
#endif

#if FULLDEBUG
#include <utilitieslib/utils/timing.h>

static PerformanceInfo* memory_group;

#define PERFINFO_ALLOC_START(func, size) {                        \
    int old_runperfinfo=0;                                        \
    PERFINFO_RUN(                                                \
        PERFINFO_AUTO_START_STATIC("heap", &memory_group, 1);    \
            if(size < 0){                                        \
                PERFINFO_AUTO_START(func, 1);                    \
            }                                                    \
            else if(size <= 100){                                \
                PERFINFO_AUTO_START(func " <= 100", 1);            \
            }                                                    \
            else if(size <= 1000){                                \
                PERFINFO_AUTO_START(func " <= 1,000", 1);        \
            }                                                    \
            else if(size <= 10000){                                \
                PERFINFO_AUTO_START(func " <= 10,000", 1);        \
            }                                                    \
            else if(size <= 100000){                            \
                PERFINFO_AUTO_START(func " <= 100,000", 1);        \
            }                                                    \
            else if(size <= 1000000){                            \
                PERFINFO_AUTO_START(func " <= 1,000,000", 1);    \
            }                                                    \
            else{                                                \
                PERFINFO_AUTO_START(func " > 1,000,000", 1);    \
            }                                                    \
                old_runperfinfo = timing_state.runperfinfo;        \
                timing_state.runperfinfo = 0;                    \
            );

#define PERFINFO_ALLOC_END()                                    \
    if (old_runperfinfo) {                                        \
                timing_state.runperfinfo = old_runperfinfo;        \
            PERFINFO_AUTO_STOP();                                \
        PERFINFO_AUTO_STOP();                                    \
    } }

#else // !FULLDEBUG
#    define PERFINFO_ALLOC_START(func, size)            {
#    define PERFINFO_ALLOC_END()                        }
#    define PERFINFO_AUTO_START(locNameParam, inc)    {
#    define PERFINFO_AUTO_STOP()                        }
#endif

void* malloc_timed(size_t size, int blockType, const char *filename, int linenumber)
{
    void* result;

    PERFINFO_ALLOC_START("malloc", size);
#ifdef ENABLE_SMALL_ALLOC
    if ((InterlockedIncrement(&g_inside_pool_malloc)==1) && size <= MAX_POOL_SIZE)
    {
        if (!malloc_pool_critsec_init)
        {
            InitializeCriticalSectionAndSpinCount(&malloc_pool_critsec, 4000);
            malloc_pool_critsec_init = 1;
        }
        mmCRTHeapLock();
        EnterCriticalSection(&malloc_pool_critsec);
        result = mallocPoolMemory(size, SA_UNINITIALIZED_VALUE, filename, linenumber);
        LeaveCriticalSection(&malloc_pool_critsec);
        mmCRTHeapUnlock();
    }
    else
#endif
    {
        result = (void*)_malloc_dbg(size, blockType, filename, linenumber);
        memtrack_alloc(result,size);
    }
#ifdef ENABLE_SMALL_ALLOC
    InterlockedDecrement(&g_inside_pool_malloc);
#endif
    if(!result){
        assertOnAllocError(filename, linenumber, "malloc failed to allocate %d bytes", size);
    }
    PERFINFO_ALLOC_END();

    return result;
}

#ifdef _XBOX
void* physicalmalloc_timed(size_t size, int alignment, const char *filename, int linenumber)
{
    void * result;
    PERFINFO_ALLOC_START("physicalmalloc", size);

    result = XPhysicalAlloc(size,MAXULONG_PTR,alignment,PAGE_READWRITE);
    memMonitorTrackUserMemory("XBOX_PHYSICAL", 1, MOT_ALLOC, size);

    PERFINFO_ALLOC_END();
    return result;
}

void physicalfree_timed(void *userData)
{
    PERFINFO_ALLOC_START("free", -1);
    XPhysicalFree(userData);
    memMonitorTrackUserMemory("XBOX_PHYSICAL", 1, MOT_FREE, XPhysicalSize(userData));
    PERFINFO_ALLOC_END();
}
#endif

void* calloc_timed(size_t num, size_t size, int blockType, const char *filename, int linenumber)
{
    void* result;
    size_t totalSize = size * num;

    PERFINFO_ALLOC_START("calloc", totalSize);
#ifdef ENABLE_SMALL_ALLOC
    if ((InterlockedIncrement(&g_inside_pool_malloc)==1) && totalSize <= MAX_POOL_SIZE)
    {
        if (!malloc_pool_critsec_init)
        {
            InitializeCriticalSectionAndSpinCount(&malloc_pool_critsec, 4000);
            malloc_pool_critsec_init = 1;
        }
        mmCRTHeapLock();
        EnterCriticalSection(&malloc_pool_critsec);
        result = mallocPoolMemory(totalSize, 0, filename, linenumber);
        LeaveCriticalSection(&malloc_pool_critsec);
        mmCRTHeapUnlock();
    }
    else
#endif
    {
        result = (void*)_calloc_dbg(num, size, blockType, filename, linenumber);
        memtrack_alloc(result,num*size);
    }
#ifdef ENABLE_SMALL_ALLOC
    InterlockedDecrement(&g_inside_pool_malloc);
#endif
    if(!result){
        assertOnAllocError(filename, linenumber, "calloc failed to allocate %d bytes", size * num);
    }
    PERFINFO_ALLOC_END();

    return result;
}

void* realloc_timed(void *userData, size_t newSize, int blockType, const char *filename, int linenumber)
{
#ifdef ENABLE_SMALL_ALLOC
    SmallAllocHeader *sah;
#endif

    void* result;

    PERFINFO_ALLOC_START("realloc", newSize);
#ifdef ENABLE_SMALL_ALLOC
    if (userData && (sah = getSAHeader(userData)))
    {
        if (!malloc_pool_critsec_init)
        {
            InitializeCriticalSectionAndSpinCount(&malloc_pool_critsec, 4000);
            malloc_pool_critsec_init = 1;
        }
        mmCRTHeapLock();
        EnterCriticalSection(&malloc_pool_critsec);
        result = reallocPoolMemory(sah, newSize, blockType, filename, linenumber);
        LeaveCriticalSection(&malloc_pool_critsec);
        mmCRTHeapUnlock();
    }
    else
#endif
    {
        result = (void*)_realloc_dbg(userData, newSize, blockType, filename, linenumber);
        memtrack_realloc(result, newSize, userData);
    }
    if(!result && newSize){
        assertOnAllocError(filename, linenumber, "realloc failed to reallocate to %d bytes", newSize);
    }
    PERFINFO_ALLOC_END();

    return result;
}

char* strdup_timed(const char *s, int blockType, const char *filename, int linenumber)
{
    char *result = NULL;
    PERFINFO_ALLOC_START("strdup", -1);
    if(s)
    {
        // our strdup has never used small allocs...
        result = _strdup_dbg(s, blockType, filename, linenumber);
        memtrack_alloc(result, strlen(result)+1); 
        if(!result)
            assertOnAllocError(filename, linenumber, "strdup failed to allocate %d bytes", strlen(s));
    }
    PERFINFO_ALLOC_END();
    return result;
}

void free_timed(void *userData, int blockType)
{
#ifdef ENABLE_SMALL_ALLOC
    SmallAllocHeader *sah;
#endif

    if(!userData)
        return;

    PERFINFO_ALLOC_START("free", -1);
#ifdef ENABLE_SMALL_ALLOC
    if (sah = getSAHeader(userData))
    {
        if (!malloc_pool_critsec_init)
        {
            InitializeCriticalSectionAndSpinCount(&malloc_pool_critsec, 4000);
            malloc_pool_critsec_init = 1;
        }
        mmCRTHeapLock();
        EnterCriticalSection(&malloc_pool_critsec);
        freePoolMemory(sah);
        LeaveCriticalSection(&malloc_pool_critsec);
        mmCRTHeapUnlock();
    }
    else
#endif
    {
        memtrack_free(userData);
        _free_dbg(userData, blockType);
    }
    PERFINFO_ALLOC_END();
}

// Win32 Heap* timed wrappers.

LPVOID timed_HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
{
    LPVOID ret;
    PERFINFO_AUTO_START("HeapAlloc", 1);
    ret = (LPVOID)HeapAlloc(hHeap, dwFlags, dwBytes);
    PERFINFO_AUTO_STOP();
    return ret;
}

HANDLE timed_HeapCreate(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
{
    HANDLE hHeap;
    PERFINFO_AUTO_START("HeapCreate", 1);
    hHeap = HeapCreate(flOptions, dwInitialSize, dwMaximumSize);
    PERFINFO_AUTO_STOP();
    return hHeap;
}

BOOL timed_HeapDestroy(HANDLE hHeap)
{
    BOOL ret;
    PERFINFO_AUTO_START("HeapDestroy", 1);
    ret = HeapDestroy(hHeap);
    PERFINFO_AUTO_STOP();
    return ret;
}

LPVOID timed_HeapReAlloc(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes)
{
    LPVOID ret;
    PERFINFO_AUTO_START("HeapReAlloc", 1);
    ret = (LPVOID)HeapReAlloc(hHeap, dwFlags, lpMem, dwBytes);
    PERFINFO_AUTO_STOP();
    return ret;
}

BOOL timed_HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem)
{
    BOOL ret;
    PERFINFO_AUTO_START("HeapFree", 1);
    ret = HeapFree(hHeap, dwFlags, lpMem);
    PERFINFO_AUTO_STOP();
    return ret;
}

#endif
