
//extern "C" {
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/timing.h"
//}

void operator delete(void* memory)
{
#if defined(__MINGW32__) && _CRTDBG_MAP_ALLOC && !defined(ENABLE_LEAK_DETECTION)
    free_timed(memory, _NORMAL_BLOCK);
#else
    free(memory);
#endif
}

void operator delete[](void *memory)
{
#if defined(__MINGW32__) && _CRTDBG_MAP_ALLOC && !defined(ENABLE_LEAK_DETECTION)
    free_timed(memory, _NORMAL_BLOCK);
#else
    free(memory);
#endif
}

#if _CRTDBG_MAP_ALLOC
void* operator new( size_t cb, int nBlockUse, const char* szFileName, int nLine)
{
#ifdef ENABLE_LEAK_DETECTION
    return GC_debug_malloc_uncollectable(cb, szFileName, nLine);
#else
    return malloc_timed(cb, nBlockUse, szFileName, nLine);
#endif
}

void* operator new[](size_t cb, int nBlockUse, const char* szFileName, int nLine)
{
#ifdef ENABLE_LEAK_DETECTION
    return GC_debug_malloc_uncollectable(cb, szFileName, nLine);
#else
    return malloc_timed(cb, nBlockUse, szFileName, nLine);
#endif
}
#endif

void * operator new( size_t cb )
{
#ifdef ENABLE_LEAK_DETECTION
    return GC_MALLOC_UNCOLLECTABLE(cb);
#elif defined(__MINGW32__) && _CRTDBG_MAP_ALLOC
    return malloc_timed(cb, _NORMAL_BLOCK, __FILE__, __LINE__);
#else
    return malloc(cb);
#endif
}

void * operator new[]( size_t cb )
{
#ifdef ENABLE_LEAK_DETECTION
    return GC_MALLOC_UNCOLLECTABLE(cb);
#elif defined(__MINGW32__) && _CRTDBG_MAP_ALLOC
    return malloc_timed(cb, _NORMAL_BLOCK, __FILE__, __LINE__);
#else
    return malloc(cb);
#endif
}
