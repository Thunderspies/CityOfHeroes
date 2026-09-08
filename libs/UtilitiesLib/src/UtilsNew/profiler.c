
#include "utilitieslib/UtilsNew/barrier.h"
#include "utilitieslib/UtilsNew/profiler.h"

#ifdef _CAP_PROFILING
#error "Do not compile this file with /fastcap or /callcap"
#endif

#ifdef xcase
#error "Do not include stdtypes.h"
#endif

#ifdef ENABLE_PROFILER
//#define DEBUG_PROFILER
#define ENABLE_THREADING
#define SUBTRACT_OVERHEAD
#endif

#ifdef ENABLE_PROFILER

#include <stdio.h>
#include <intrin.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#pragma warning(disable:4127)

typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

#define MAX_STACK 100

typedef struct ProfileEntry {
#ifdef ENABLE_THREADING
    uint32_t threadId;
#endif    
    uint32_t parent;
    void * from;
    void * to;
    uint64_t elapsed;
} ProfileEntry;

typedef struct ProfileData {
    ProfileEntry * volatile next;
    volatile long enableId;
#ifdef ENABLE_THREADING
    volatile long spinLock;
#endif
    uint64_t timingOverhead;
    volatile long missed;
    ProfileEntry * first;
    ProfileEntry * end;
    long lastEnableId;
} ProfileData;

typedef struct ProfileThreadEntry {
    ProfileEntry * entry;
    uint64_t before;
#ifdef SUBTRACT_OVERHEAD
    uint64_t overhead;
#endif
} ProfileThreadEntry;

typedef struct ProfileThreadData {
#ifdef ENABLE_THREADING
    uint32_t threadId;
#endif
    long enabledId;
    long depth;
    ProfileThreadEntry stack[MAX_STACK];
} ProfileThreadData;

static ProfileData profile = {0,};

#ifdef ENABLE_THREADING
static __declspec(thread) ProfileThreadData _profileThread = {0,};
#else
static ProfileThreadData _profileThread = {0,};
#endif

#ifdef DEBUG_PROFILER
    #define PASSERT(exp) if((exp)) { __nop(); } else { DebugBreak(); }
    #define PINLINE __inline
#else
    #define PASSERT(exp) do {} while(0)
    #define PINLINE __forceinline
#endif

void CAP_Start_Profiling(void * CurrentProcAddr, void * TargetProcAddr);
void CAP_End_Profiling(void * CurrentProcAddr);

#if defined(_M_IX86)

#pragma warning(disable:4054)

__declspec(naked) void _stdcall _CAP_Start_Profiling(void *CurrentProcAddr, void *TargetProcAddr)
{
    __asm {
        jmp bail
        nop
        nop
        bail:
        ret 8
    }

    __asm {
        push ebp
        mov ebp, esp
        sub esp, __LOCAL_SIZE

        push eax
        push ecx
        push edx
    }

    CAP_Start_Profiling(CurrentProcAddr, TargetProcAddr);

    __asm {
        pop edx
        pop ecx
        pop eax

        mov esp, ebp
        pop ebp
        ret 8
    }
}

__declspec(naked) void _stdcall _CAP_End_Profiling(void *CurrentProcAddr)
{
    __asm {
        jmp bail
        nop
        nop
        bail:
        ret 4
    }

    __asm {
        push ebp
        mov ebp, esp
        sub esp, __LOCAL_SIZE

        push eax
        push ecx
        push edx
    }

    CAP_End_Profiling(CurrentProcAddr);

    __asm {
        pop edx
        pop ecx
        pop eax

        mov esp, ebp
        pop ebp
        ret 4
    }
 }

#elif defined(_M_AMD64)

void _stdcall __CAP_Start_Profiling(void *CurrentProcAddr, void *TargetProcAddr)
{
    (void)CurrentProcAddr;
    (void)TargetProcAddr;
}

void _stdcall __CAP_End_Profiling(void *CurrentProcAddr)
{
    (void)CurrentProcAddr;
}

#else
#error "Platform support missing for profiler.c"
#endif

static int ResetProfilingThread(ProfileThreadData * ptd)
{
#ifdef ENABLE_THREADING
    if(!ptd->threadId) {
        ptd->threadId = GetCurrentThreadId();
        PASSERT(ptd->threadId);
    }
#endif

    ptd->enabledId = profile.enableId;
    ptd->depth = 0;
    return 1;
}

static PINLINE ProfileEntry * NewProfileEntry(uint32_t threadId, void * from, void * to, uint32_t parent)
{
    register ProfileEntry * entry;

    do {
        entry = (ProfileEntry*)profile.next;

        // Check if we are out of room
        if(entry == profile.end) {
#ifdef ENABLE_THREADING
            _InterlockedIncrement(&profile.missed);
#else
            profile.missed++;
#endif
            return NULL;
        }

    }
#ifdef ENABLE_THREADING
    while(((PVOID)(LONG_PTR)_InterlockedCompareExchange((LONG volatile *)(&profile.next), (LONG)(LONG_PTR)(entry+1), (LONG)(LONG_PTR)(entry))) != entry);
    entry->threadId = threadId;
#else
    while(0);
    profile.next++;
    (void)threadId;
#endif

    entry->parent = parent;
    entry->from = from;
    entry->to = to;
    entry->elapsed = 0;
    return entry;
}

void CAP_Start_Profiling(void * CurrentProcAddr, void * TargetProcAddr)
{
    register ProfileThreadData * ptd;
    register ProfileEntry * entry;
#ifdef SUBTRACT_OVERHEAD
    register uint64_t localBefore = __rdtsc();
#endif

#ifdef ENABLE_THREADING
    _InterlockedIncrement(&profile.spinLock);
#endif

    ptd = &_profileThread;
    if(profile.enableId) {
        if(profile.enableId == ptd->enabledId) {
            __nop();
        } else {
            ResetProfilingThread(ptd);
        }
    } else {
#ifdef ENABLE_THREADING
        _InterlockedDecrement(&profile.spinLock);
#endif
        return;
    }

    // Record the call information
    entry = NewProfileEntry(
#ifdef ENABLE_THREADING
        ptd->threadId,
#else
        0,
#endif
        CurrentProcAddr,
        TargetProcAddr,
        ptd->depth ? (uint32_t)(ptd->stack[ptd->depth-1].entry - profile.first) : ~0U);

    if(!entry) {
        __nop();
    } else {
        // Store the new stack entry
        register long depth = ptd->depth;
        register uint64_t localAfter;
        register ProfileThreadEntry * threadEntry = ptd->stack + depth;

        PASSERT(depth < MAX_STACK);
        ptd->depth = depth+1;

        threadEntry->entry = entry;

        localAfter = __rdtsc();
        threadEntry->before = localAfter;
#ifdef SUBTRACT_OVERHEAD
        threadEntry->overhead = localAfter - localBefore;
#endif
    }

#ifdef ENABLE_THREADING
    _InterlockedDecrement(&profile.spinLock);
#endif
}

void CAP_End_Profiling(void * CurrentProcAddr)
{
    register ProfileThreadData * ptd;
    register ProfileEntry * entry;
    register uint64_t localBefore = __rdtsc();

#ifdef ENABLE_THREADING
    _InterlockedIncrement(&profile.spinLock);
#endif    

    ptd = &_profileThread;
    if(profile.enableId) {
        if(profile.enableId == ptd->enabledId) {
            __nop();
        } else {
            ResetProfilingThread(ptd);
        }
    } else {
#ifdef ENABLE_THREADING
        _InterlockedDecrement(&profile.spinLock);
#endif
        return;
    }

    ptd = &_profileThread;
    if(ptd->depth > 0) {
        register int nextDepth = ptd->depth-1;
        register ProfileThreadEntry * threadEntry = ptd->stack + nextDepth;
        register __int64 elapsed;

        PASSERT(threadEntry->entry->from == CurrentProcAddr);
        PASSERT(threadEntry->before);
        PASSERT(!threadEntry->entry->elapsed);

        elapsed = (localBefore - threadEntry->before) - profile.timingOverhead;
        elapsed = (elapsed < 1) ? 1 : elapsed;
        threadEntry->entry->elapsed = elapsed;

#ifdef SUBTRACT_OVERHEAD
        // trickle the profiling overhead up the stack
        if(nextDepth > 0) {
            register uint64_t localAfter = __rdtsc();
            register uint64_t overhead = threadEntry->overhead + (localAfter-localBefore) + profile.timingOverhead*2;
            ptd->stack[nextDepth-1].overhead += overhead;
            ptd->stack[nextDepth-1].before += overhead;
        }
#endif

        ptd->depth = nextDepth;
#ifdef ENABLE_THREADING
        _InterlockedDecrement(&profile.spinLock);
#endif
        return;
    }

    entry = NewProfileEntry(
#ifdef ENABLE_THREADING
        ptd->threadId,
#else
        0,
#endif
        CurrentProcAddr,
        NULL,
        ~0U);

    if(!entry) {
#ifdef ENABLE_THREADING
        _InterlockedIncrement(&profile.missed);
#else
        profile.missed++;
#endif
    }

#ifdef ENABLE_THREADING
    _InterlockedDecrement(&profile.spinLock);
#endif
}

#pragma optimize("", off)
uint64_t CalculateTimingOverhead(void)
{
    int i;
    int junk[4];
    uint64_t before, after;
    const int n = 10000;

    __cpuid(junk, 0);
    before = __rdtsc();
    for(i=0; i<n; i++) {
        uint64_t tsc;
        tsc = __rdtsc();
        tsc = __rdtsc();
        tsc = __rdtsc();
        tsc = __rdtsc();
        tsc = __rdtsc();
        tsc = __rdtsc();
        tsc = __rdtsc();
        tsc = __rdtsc();
        tsc = __rdtsc();
        tsc = __rdtsc();
    }
    __cpuid(junk, 0);
    after = __rdtsc();

    return (after-before) / (n*10);
}
#pragma optimize("", on)

void BeginProfile(size_t memory)
{
    if(profile.enableId) {
        printf("BeginProfile called without and EndProfile\n");
        EndProfile(NULL);
    }

#ifdef ENABLE_THREADING
    _ReadWriteBarrier();
    PASSERT(!profile.spinLock);
#endif

    PASSERT(!profile.enableId);
    PASSERT(!profile.first);
    PASSERT(!profile.end);
    PASSERT(!profile.next);

    profile.first = (ProfileEntry*)VirtualAlloc(NULL, memory, MEM_COMMIT, PAGE_READWRITE);
    if(!profile.first) {
        printf("BeginProfile could not allocate %zu bytes of memory\n", memory);
        return;
    }

    profile.end = profile.first + memory/sizeof(ProfileEntry);
    profile.next = profile.first;
    profile.missed = 0;

    if(!profile.timingOverhead) {
        profile.timingOverhead = CalculateTimingOverhead();
        printf("Processor timing overhead %I64u ticks\n", profile.timingOverhead);
        PASSERT(profile.timingOverhead > 0);
        PASSERT(profile.timingOverhead < 10000);
    }

#ifdef ENABLE_THREADING
    _ReadWriteBarrier();
#endif

#ifdef _M_IX86
    // Self modifying code, enable the profiler by writing a jmp over the return
    {
        static unsigned int enable_code = 0x909005EBU;
        BOOL ret;
        ret = WriteProcessMemory(GetCurrentProcess(), (LPVOID)_CAP_End_Profiling, &enable_code, 2, NULL);
        if(!ret) {
            printf("Could not enable the profiler by modifying _CAP_End_Profiling\n");
        }
        WriteProcessMemory(GetCurrentProcess(), (LPVOID)_CAP_Start_Profiling, &enable_code, 2, NULL);
        if(!ret) {
            printf("Could not enable the profiler by modifying _CAP_Start_Profiling\n");
        }
    }
#endif

    _InterlockedExchange(&profile.enableId, ++profile.lastEnableId);
}

void EndProfile(const char * filename)
{
    if(!profile.enableId) {
        printf("BeginProfile was not called before EndProfile\n");
        return;
    }

    PASSERT(profile.first);
    PASSERT(profile.end);
    PASSERT(profile.next);

    // Request all threads to stop profiling
    _InterlockedExchange(&profile.enableId, 0);

#ifdef _M_IX86
    // Self modifying code, enable the profiler by writing returns back over the nops
    {
        static unsigned int disable_code = 0x909002EBU;
        BOOL ret;
        ret = WriteProcessMemory(GetCurrentProcess(), (LPVOID)_CAP_End_Profiling, &disable_code, 2, NULL);
        if(!ret) {
            printf("Could not enable the profiler by modifying _CAP_End_Profiling\n");
        }
        WriteProcessMemory(GetCurrentProcess(), (LPVOID)_CAP_Start_Profiling, &disable_code, 2, NULL);
        if(!ret) {
            printf("Could not enable the profiler by modifying _CAP_Start_Profiling\n");
        }
    }
#endif

#ifdef ENABLE_THREADING
    _ReadWriteBarrier();
    while(profile.spinLock) {
        Sleep(0);
    }
    _ReadWriteBarrier();
    PASSERT(!profile.spinLock);
#endif

    if(profile.next == profile.end) {
        printf("Profile ran out of memory\n");
    }
    if(profile.missed > 10) {
        printf("Profile missed %d function calls\n", profile.missed);
    }

    if(filename) {
        FILE * f = fopen(filename, "wb");
        if(f) {
            HMODULE modules[256];
            DWORD returned = 0;
            DWORD d;
            BOOL ret;
            ProfileEntry * entry;

            #if 0
            ret = EnumProcessModulesEx(GetCurrentProcess(), modules, sizeof(modules), &returned, LIST_MODULES_ALL);
            #else
            ret = EnumProcessModules(GetCurrentProcess(), modules, sizeof(modules), &returned);
            #endif
            PASSERT(ret && returned && returned <= sizeof(modules));
            for(d=0; d<returned/sizeof(modules[0]); d++) {
                char modulepath[1024];
                MODULEINFO modinfo;

                GetModuleFileNameExA(GetCurrentProcess(), modules[d], modulepath, sizeof(modulepath));
                GetModuleInformation(GetCurrentProcess(), modules[d], &modinfo, sizeof(modinfo));

                fprintf(f, "\"%s\",0x%p,0x%u\n", modulepath, modinfo.lpBaseOfDll, modinfo.SizeOfImage);
            }

            fprintf(f, "----------\n");

            for(entry = profile.first; entry < profile.next; entry++)
            {
                long long first = entry - profile.first;
                fprintf(f, "%lld,%d,%d,0x%p,0x%p,%I64u\n",
                        first,
                        entry->parent,
#ifdef ENABLE_THREADING
                        entry->threadId,
#else
                        0,
#endif
                        entry->from,
                        entry->to,
                        entry->elapsed);
            }
        }
        fclose(f);
    }

    VirtualFree(profile.first, 0, MEM_RELEASE);
    profile.first = NULL;
    profile.next = NULL;
    profile.end = NULL;
}

#endif
