#include "utilitieslib/utils/MemoryMonitor.h"
#include "utilitieslib/assert/assert.h"
#include <stdio.h>
#include "utilitieslib/components/StashTable.h"
#include <stdlib.h>
#include <crtdbg.h>
#include "utilitieslib/components/earray.h"
#include <math.h>

#include <string.h>
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/UnitSpec.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/components/StringTable.h"
#include "utilitieslib/utils/log.h"

//-------------------------------------------------------------------------------------
// Variable history tracking
//-------------------------------------------------------------------------------------

size_t totalProcessMemoryUsage;
double totalProcessMemoryTraffic;


HistoryCollection* hcCreate()
{
    HistoryCollection* hc;
    hc = calloc(1, sizeof(HistoryCollection));
    eaCreateSmallocSafe(&hc->collection);
    eafCreateSmallocSafe(&hc->convergenceTimes);
    hc->lastSampleTime = timerCpuTicks64();
    return hc;
}

void hcSample(HistoryCollection* hc)
{
    int i;
    int exponentCount;
    int valueCursor;
    float timeSinceLastSample;

    assert(hc);
    assert(hc->convergenceTimes);        // must set convergence times before sampling can begin!
    if(!hc->convergenceExponents)
    {
        eafCreateSmallocSafe(&hc->convergenceExponents);

        // Make sure there is room for a convergence exponent per convergence time.
        eafSetSize(&hc->convergenceExponents, eafSize(&hc->convergenceTimes));
    }

    // Once sampling begins, the number of convergence times can never change.
    //    This is because all value histories that belongs to this collection has already been
    //    initialized using the the number of convergence times.  If the number of convergence times
    //    is changed, all value histories should change along with it as well.
    assert(eafSize(&hc->convergenceTimes) == eafSize(&hc->convergenceExponents));


    // How much time has elapsed since last sample time?
    {
        __int64 currentTime = timerCpuTicks64();

        timeSinceLastSample = timerSeconds64(currentTime - hc->lastSampleTime);
        //timeSinceLastSample = 1 * 1.0;
        hc->lastSampleTime = currentTime;
    }

    // Compute the convergence exponents for this time period.
    exponentCount =    eafSize(&hc->convergenceExponents) ;
    for(i = 0; i < exponentCount; i++)
    {
        hc->convergenceExponents[i] = (float)pow(0.1f, timeSinceLastSample / hc->convergenceTimes[i]);    
    }


    for(valueCursor = 0; valueCursor < eaSize(&hc->collection); valueCursor++)
    {
        ValueHistory* vh = hc->collection[valueCursor];
        float instantaneousValue;
        assert(vh);        // Always remove a VauleHistory through vhDetach().

        instantaneousValue = (float)vh->value;
        for(i = 0; i < exponentCount; i++)
        {
            float factor = hc->convergenceExponents[i];

            vh->history[i] = vh->history[i] * factor + instantaneousValue * (1 - factor);
        }
    }
}

void hcAddHistory(HistoryCollection* hc, ValueHistory* vh)
{
    assert(hc && hc->collection && vh);
    eaPush(&hc->collection, vh);

    vh->def = hc;
    vh->value = 0.0;
    eafCreateSmallocSafe(&vh->history);
    eafSetSize(&vh->history, eafSize(&hc->convergenceTimes));
}

//-------------------------------------------------------------------------------------
// Memory Monitor
//-------------------------------------------------------------------------------------

S64 MonitorWorkTime = 0;

MemoryMonitor MemMonitor;

static volatile LONG staticInsideHook = 0;

void mmCRTHeapLock()
{
    // Lock the CRT heap.
    HeapLock((HANDLE)_get_heap_handle());
}

void mmCRTHeapUnlock()
{
    // Unlock the CRT heap.
    HeapUnlock((HANDLE)_get_heap_handle());
}

/* Function mmGetModuleOperationStats()
*    Given a collection, retrieve the object that records the memory operation stats for the
*    module.
*
*    Returns:
*        existing stats object    - if one for the specified module already exists.
*        new stats object        - if one for the specified module does not exist.
*
*/
// MAK - staticModuleName supports evil: if set, I'm allowed to cache the moduleName _pointer_
ModuleMemOperationStats* mmGetModuleOperationStats(MemoryMonitor* statsCollection, char* moduleName, int staticModuleName)
{
    static ModuleMemOperationStats *last_stats = NULL;
    static ModuleMemOperationStats *last_stats2 = NULL;
    static char* last_module;
    static char* last_module2;

    ModuleMemOperationStats* stats;

    mmCRTHeapLock();

    InterlockedIncrement(&staticInsideHook);

    // Look the the stats object for the specified module.
    if (staticModuleName && last_module == moduleName)
    {
        stats = last_stats;
    }
    else if (staticModuleName && last_module2 == moduleName)
    {
        stats = last_stats2;
    }
    else
    {
        stashFindPointer( statsCollection->moduleStats, moduleName, &stats );

        // If such an object doesn't exist...
        if(!stats)
        {
            // Create and initialize one and add it to the collection.

            StashElement element;
            int i;
            
            stats = calloc(1, sizeof(ModuleMemOperationStats));
            
            stashAddPointerAndGetElement(statsCollection->moduleStats, moduleName, stats, false, &element);
            stats->moduleName = stashElementGetStringKey(element);

            for(i = 0; i < MOT_COUNT - 1; i++)
            {
                hcAddHistory(statsCollection->history, &stats->stats[i].count);
                hcAddHistory(statsCollection->history, &stats->stats[i].elapsedTime);
                hcAddHistory(statsCollection->history, &stats->stats[i].memTraffic);
            }
        }
        
        last_stats2 = last_stats;
        last_module2 = last_module;
        last_stats = stats;
        last_module = moduleName;
    }

    InterlockedDecrement(&staticInsideHook);

    mmCRTHeapUnlock();

    return stats;
}


/* Function moUpdateStats()
*    Given the stats to update, the operation type, and other operation related variables, this function
*    updates the given stats object accordingly.
*
*    The function will update the MOT_AGGREGATE stats.
*
*    This function is meant to be called everytime a memory allocation occurs in a particular module.
*
*    Paramters:
*        type - Must be MOT_ALLOC, MOT_REALLOC, or MOT_FREE.
*
*/
void mmoUpdateStats(ModuleMemOperationStats* stats, MemOperationTypes type, float elapsedTime, intptr_t memTraffic)
{
    MemOperationStat* opStat;
    assert(MOT_NONE != type);    // Make sure the operation type is valid.
    assert(elapsedTime == 0.0f);

    opStat = &stats->stats[type - 1];
    opStat->count.value    += 1.0;
    opStat->elapsedTime.value += elapsedTime;
    opStat->memTraffic.value += (double)memTraffic;

    // Track aggregate values.
    opStat = &stats->stats[MOT_AGGREGATE - 1];
    opStat->count.value += 1.0;
    opStat->elapsedTime.value += elapsedTime;
    opStat->memTraffic.value += (double)memTraffic;

    totalProcessMemoryTraffic += (double)memTraffic;

    // Track balance values.
    switch(type)
    {
        xcase MOT_ALLOC:
            opStat = &stats->stats[MOT_BALANCE - 1];
            opStat->count.value += 1.0;
            opStat->memTraffic.value += (double)memTraffic;
            totalProcessMemoryUsage += memTraffic;

        xcase MOT_REALLOC:
            opStat = &stats->stats[MOT_BALANCE - 1];
            opStat->memTraffic.value += (double)memTraffic;
            totalProcessMemoryUsage += memTraffic;

        xcase MOT_FREE:
            opStat = &stats->stats[MOT_BALANCE - 1];
            opStat->count.value -= 1.0;
            opStat->memTraffic.value -= (double)memTraffic;
            totalProcessMemoryUsage -= memTraffic;
    }
}

void memMonitorTrackUserMemory(const char *moduleName, int staticModuleName, MemOperationTypes type, intptr_t memTraffic)
{
    if (memMonitorActive()) {
        ModuleMemOperationStats* stats;
        stats = mmGetModuleOperationStats(&MemMonitor, (char*)moduleName, staticModuleName);
        mmoUpdateStats(stats, type, 0, memTraffic);
    }
}

static S32 sharedMemoryContribution=0;
void memMonitorTrackSharedMemory(int  v)
{
    sharedMemoryContribution+=v;
}

void memMointorUpdate()
{
    __int64 beginTime = timerCpuTicks64();
    hcSample(MemMonitor.history);
    MonitorWorkTime += timerCpuTicks64() - beginTime;
}

//-------------------------------------------------------------------------------------
// Memory Allocation Hook
//-------------------------------------------------------------------------------------
/**************************************************************************
* Memory Allocation Hook
*    Hooks into the visual studios c-runtime library to receive information
*    on what kind of memory allocation/deallocation requests.
*
*    WARNING!!! CRT dependency!
*        This hook relies on one of VS7 CRT memory allocator's internal
*        structures.  It is untest with other versions of visual studio
*        CRT.
*/

#include <stddef.h>

// WARNING!!! CRT dependency!
//    Ripped from VS7 CRT library internal header file.

#define nNoMansLandSize 4
typedef struct CrtMemBlockHeader
{
    struct CrtMemBlockHeader * pBlockHeaderNext;
    struct CrtMemBlockHeader * pBlockHeaderPrev;
    char *                      szFileName;
    int                         nLine;
#ifdef _WIN64
    /* These items are reversed on Win64 to eliminate gaps in the struct
    * and ensure that sizeof(struct)%16 == 0, so 16-byte alignment is
    * maintained in the debug heap.
    */
    int                         nBlockUse;
    size_t                      nDataSize;
#else  /* _WIN64 */
    size_t                      nDataSize;
    int                         nBlockUse;
#endif  /* _WIN64 */
    long                        lRequest;
    unsigned char               gap[nNoMansLandSize];
    /* followed by:
    *  unsigned char           data[nDataSize];
    *  unsigned char           anotherGap[nNoMansLandSize];
    */
} _CrtMemBlockHeader;


// Debug structure holding the last few memory operations
// Not thread-reliable (but won't crash), but just for debugging
struct {
    size_t size;
    const char *type;
    const unsigned char *filename;
    int line;
    void *data;
} lastMemoryOps[32];
int lastMemoryOpsIndex=-1;

typedef struct MemorySizeAllocation {
    // Nothing up here.  maxAllocationRange must be first.

    S32 maxAllocationRange;    // The maximum allocation size for this range.
    S32 minAllocationRange;    // The minimum allocation size for this range.
    
    // Other stuff can go down here.
    
    S32 curCount;            // Current # of allocations in this size range.
    S32 maxCount;            // Max value of curCount.
    S32 lastCount;
    
    intptr_t maxAllocSize;        // Max size of an allocation.
    intptr_t minAllocSize;        // Min size of an allocation.
    
    intptr_t curTotalSize;        // Current size of all allocations.
    intptr_t maxTotalSize;        // Max size of curTotalSize.
    intptr_t lastTotalSize;
    
    intptr_t curTraffic;
    intptr_t lastTraffic;
} MemorySizeAllocation;

static MemorySizeAllocation memoryAllocations[] = {
    { 0 },
    { 10 },
    { 20 },
    { 30 },
    { 40 },
    { 50 },
    { 60 },
    { 70 },
    { 80 },
    { 90 },
    { 100 },
    { 200 },
    { 300 },
    { 400 },
    { 500 },
    { 600 },
    { 700 },
    { 800 },
    { 900 },
    { 1000 },
    { 2000 },
    { 3000 },
    { 4000 },
    { 5000 },
    { 6000 },
    { 7000 },
    { 8000 },
    { 9000 },
    { 10000 },
    { 20000 },
    { 30000 },
    { 40000 },
    { 50000 },
    { 60000 },
    { 70000 },
    { 80000 },
    { 90000 },
    { 100000 },
    { 200000 },
    { 300000 },
    { 400000 },
    { 500000 },
    { 600000 },
    { 700000 },
    { 800000 },
    { 900000 },
    { 1000000 },
    { 2000000 },
    { 3000000 },
    { 4000000 },
    { 5000000 },
    { 6000000 },
    { 7000000 },
    { 8000000 },
    { 9000000 },
    { 10000000 },
    { 20000000 },
    { 30000000 },
    { 40000000 },
    { 50000000 },
    { 60000000 },
    { 70000000 },
    { 80000000 },
    { 90000000 },
    { 100000000 },
    { -1 },
};

static MemorySizeAllocation* getMemAllocation(int size)
{
    static int init;
    
    MemorySizeAllocation* cur;
    
    if(!init)
    {
        init = 1;
        for(cur = memoryAllocations; cur->maxAllocationRange >= 0; cur++)
        {
            cur->minAllocSize = 0x7fffffff;
            
            if(cur->maxAllocationRange){
                cur->minAllocationRange = (cur - 1)->maxAllocationRange + 1;
            }
        }
    }
    
    for(cur = memoryAllocations; cur->maxAllocationRange >= 0 && size > cur->maxAllocationRange; cur++);
    
    return cur;
}

static void updateMemoryAllocation(intptr_t deltaBytes, S32 isFree)
{
    MemorySizeAllocation* memSizeAlloc = getMemAllocation(abs(deltaBytes));

#if 0
    if(!isFree){
        if(!deltaBytes){
            int x0 = 1;
        }
        else if(memSizeAlloc->maxAllocationRange == 10){
            int x10 = 1;
        }
        else if(memSizeAlloc->maxAllocationRange == 50){
            int x50 = 1;
        }
        else if(memSizeAlloc->maxAllocationRange == 500){
            int x500 = 1;
        }
        else if(memSizeAlloc->maxAllocationRange == 10000){
            int x10000 = 1;
        }
    }
#endif

    memSizeAlloc->curTotalSize += isFree ? -deltaBytes : deltaBytes;
    memSizeAlloc->curTraffic++;

    if(!isFree){
        memSizeAlloc->curCount++;

        if(memSizeAlloc->curCount > memSizeAlloc->maxCount){
            memSizeAlloc->maxCount = memSizeAlloc->curCount;
        }
        
        if(memSizeAlloc->curTotalSize > memSizeAlloc->maxTotalSize){
            memSizeAlloc->maxTotalSize = memSizeAlloc->curTotalSize;
        }
        
        if(deltaBytes > memSizeAlloc->maxAllocSize){
            memSizeAlloc->maxAllocSize = deltaBytes;
        }
        
        if(deltaBytes < memSizeAlloc->minAllocSize){
            memSizeAlloc->minAllocSize = deltaBytes;
        }
    }else{
        memSizeAlloc->curCount--;
    }
}

#include <crtdbg.h>

static _CRT_ALLOC_HOOK next_hook_func = NULL;

static int __cdecl MemMonitorHook(int            allocType,
                                  void*            data,
                                  size_t        allocSize,
                                  int            blockType,
                                  long            lRequest,
                                  const unsigned char* filename,
                                  int            line
                                  )
{
    static char* operation[] = { "", "alloc", "realloc", "free" };
    static char* blockTypeName[] = { "Free", "Normal", "CRT", "Ignore", "Client" };
    static int hookEntryCount;
    static int crt_reset_counter=0;
    
    int loindex;

    ModuleMemOperationStats* stats;
    _CrtMemBlockHeader* memHeader = NULL;

    if (next_hook_func)
        next_hook_func(allocType, data, allocSize, blockType, lRequest, filename, line);

    #if USE_NEW_TIMING_CODE
    {
        void timerBreakIfTimerEnabled(void);
        timerBreakIfTimerEnabled();
    }
    #endif

    hookEntryCount++;

    // Ignore internal C runtime library allocations.
    
    if (blockType == _CRT_BLOCK)
    {
        return 1;
    }

    // Ignore if this is a recursive call (should never happen since the CRT heap is critical-sectioned).
    
    if(InterlockedIncrement(&staticInsideHook) != 1) 
    {
        InterlockedDecrement(&staticInsideHook);
        return 1;
    }

    PERFINFO_AUTO_START("MemMonitor", 1);
        if(data)
        {
            assert(_CrtIsValidHeapPointer(data));

            memHeader = (_CrtMemBlockHeader*)data - 1;
            filename = memHeader->szFileName;
        }

        if(!filename)
        {
            filename = "Unknown";
        }

        // A hack to coerce CRT into resetting allocation operation count so that it *never*
        // runs period memory checks.
#ifdef _DEBUG
        if(crt_reset_counter-- <= 0)
        {
            U32 curVal = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
            _CrtSetDbgFlag(curVal);
            crt_reset_counter = 12; // hack to only do this every 12 calls (it might run CrtCheckMemory every... 16?
        }
#endif

        loindex = ++lastMemoryOpsIndex;
        if (loindex >= ARRAY_SIZE(lastMemoryOps))
            loindex = lastMemoryOpsIndex = 0;
        lastMemoryOps[loindex].filename = filename;
        lastMemoryOps[loindex].size = allocSize;
        lastMemoryOps[loindex].line = line;
        lastMemoryOps[loindex].type = operation[allocType];
        lastMemoryOps[loindex].data = data;

        // Retrieve the stats object associated with the module allocating memory.
        stats = mmGetModuleOperationStats(&MemMonitor, (char *)filename, 1);
        assert(stats);

        assert(allocType != _HOOK_REALLOC || data);
            
        switch(allocType){
            #define IF(name, x, y) if(allocSize <= x){PERFINFO_AUTO_START(name" <= "y, 1);}
            #define ELSE_IF(name, x, y) else IF(name, x, y)
            #define START_TIMER(name)                                        \
                PERFINFO_RUN(                                                \
                    IF(name, 10,            "10")                            \
                    ELSE_IF(name, 100,        "100")                            \
                    ELSE_IF(name, 1000,        "1,000")                        \
                    ELSE_IF(name, 10000,    "10,000")                        \
                    ELSE_IF(name, 100000,    "100,000")                        \
                    ELSE_IF(name, 1000000,    "1,000,000")                    \
                    ELSE_IF(name, 10000000,    "10,000,000")                    \
                    else                                                    \
                    {                                                        \
                        PERFINFO_AUTO_START(name" > 10,000,000", 1);        \
                    }                                                        \
                                                                            \
                    PERFINFO_ADD_FAKE_CPU(1000000);                            \
                )
            #define STOP_TIMER() PERFINFO_AUTO_STOP()

            xcase _HOOK_ALLOC:
                START_TIMER("alloc");
                    //printf("%i %s:%d: %s %d\n", hookEntryCount, filename, line, operation[allocType], allocSize);
                    mmoUpdateStats(stats, MOT_ALLOC, 0, allocSize);
                    //if (!filename || stricmp(filename, "Unknown")==0)
                    //    printf("");
                STOP_TIMER();

            xcase _HOOK_REALLOC:
                START_TIMER("realloc");
                    //printf("%i %s:%d: %s additional %d\n", hookEntryCount, memHeader->szFileName, memHeader->nLine, operation[allocType], allocSize - memHeader->nDataSize);
                    
                    updateMemoryAllocation(memHeader->nDataSize, 1);
                    
                    mmoUpdateStats(stats, MOT_REALLOC, 0, allocSize - memHeader->nDataSize);
                STOP_TIMER();

            xcase _HOOK_FREE:
                allocSize = memHeader->nDataSize;
                START_TIMER("free");
                    lastMemoryOps[loindex].size = allocSize;
                    //printf("%i %s:%d: %s %d\n", hookEntryCount, memHeader->szFileName, memHeader->nLine, operation[allocType], memHeader->nDataSize);
                    mmoUpdateStats(stats, MOT_FREE, 0, allocSize);
                STOP_TIMER();

            xdefault:
                assert(0);
                
            #undef IF
        }

    PERFINFO_AUTO_STOP();
    
    updateMemoryAllocation(allocSize, allocType == _HOOK_FREE);

    // Always allow the memory operation to proceed
    
    InterlockedDecrement(&staticInsideHook);
    
    return 1;         
}


void memMonitorInit()
{
    // A history collection is needed so that module stat tracking variables
    // can be properly initialized.
    if(!MemMonitor.history)
    {
        HistoryCollection* hc = hcCreate();
        assert(hc);
        MemMonitor.history = hc;

        eafPush(&hc->convergenceTimes, 1);
        eafPush(&hc->convergenceTimes, 5);
        eafPush(&hc->convergenceTimes, 10);
        eafPush(&hc->convergenceTimes, 30);
        eafPush(&hc->convergenceTimes, 60);
        eafPush(&hc->convergenceTimes, 10 * 60);
        eafPush(&hc->convergenceTimes, 30 * 60);
        eafPush(&hc->convergenceTimes, 60 * 60);
    }

    if(!MemMonitor.moduleStats)
    {
        MemMonitor.moduleStats = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys | StashCaseSensitive);
        assert(MemMonitor.moduleStats);

        // Make sure we have all the memory for tracking this module's memory allocations
        // before we set the memory allocation flag.  Otherwise, we'll get loop while trying
        // to allocate memory in this module later.
        mmGetModuleOperationStats(&MemMonitor, __FILE__, 1);
        mmGetModuleOperationStats(&MemMonitor, "Unknown", 0);
    }

#ifndef _XBOX
    // Add initial memory usage of program
    {
        unsigned long staticSize = getProcessPageFileUsage();
        if (staticSize) {
            ModuleMemOperationStats* stats;
            stats = mmGetModuleOperationStats(&MemMonitor, "StartupSize", 0);
            mmoUpdateStats(stats, MOT_ALLOC, 0, staticSize);
        }
    }
#endif

    // Set up the CRT alloc hook to catch all future memory allocations.
    if (!next_hook_func)
        next_hook_func = _CrtSetAllocHook(MemMonitorHook);

    // Add hashtable.c to the list so that it can't crash doing a realloc/resize
    stashTableDestroy(stashTableCreateWithStringKeys(16, 0));
    destroyStringTable(strTableCreate(0));

    // This disables doing a pre-fill of 0xFD when calling secure CRT string functions like strncpy_s.
    _CrtSetDebugFillThreshold(0);
}

int memMonitorActive() {
    return MemMonitor.moduleStats!=0;
}

//-------------------------------------------------------------------------------------
// Memory Monitor stat dumping
//-------------------------------------------------------------------------------------

static void defaultHandler(char *appendMe, void *junk)
{
    printf("%s", appendMe);
}

typedef void (*OutputHandler)(char *appendMe, void *userdata);

static void handlerPrintf(OutputHandler handler, void *userdata, const char *fmt, ...)
{
    va_list ap;
    char buf[2048];

    va_start(ap, fmt);
    _vsnprintf(buf,sizeof(buf),fmt,ap);
    buf[sizeof(buf)-1] = '\0';
    va_end(ap);
    handler(buf, userdata);
}

static void moPrintStats(OutputHandler handler, void *userdata, MemOperationStat* stat)
{
    UnitSpec* spec;
    spec = usFindProperUnitSpec(byteSpec, (S64)stat->memTraffic.value);

    if(1 != spec->unitBoundary)
        handlerPrintf(handler, userdata, "tot: %6.2f %-5s cnt: %7.f ", stat->memTraffic.value / spec->unitBoundary, spec->unitName, stat->count.value);
    else
        handlerPrintf(handler, userdata, "tot: %6.f %-5s cnt: %7.f ", stat->memTraffic.value / spec->unitBoundary, spec->unitName, stat->count.value);
}

static void mcPrintStats(StashTable statsCollection)
{
    StashElement element;
    StashTableIterator it;

    stashGetIterator(statsCollection, &it);
    while(stashGetNextElement(&it, &element))
    {
        ModuleMemOperationStats* stats;
        stats = stashElementGetPointer(element);
        assert(stats);

        printf("Module name: %s\n", stats->moduleName);

        printf("Alloc:     ");
        moPrintStats(defaultHandler, NULL, &stats->stats[MOT_ALLOC-1]);

        printf("\nRealloc:   ");
        moPrintStats(defaultHandler, NULL, &stats->stats[MOT_REALLOC-1]);

        printf("\nFree:      ");
        moPrintStats(defaultHandler, NULL, &stats->stats[MOT_FREE-1]);

        printf("\nAggregate: ");
        moPrintStats(defaultHandler, NULL, &stats->stats[MOT_AGGREGATE-1]);
    }
}

static void mmPrintStats()
{
    if(!MemMonitor.moduleStats)
        return;

    mcPrintStats(MemMonitor.moduleStats);
    printf("\nWorkTime: %f\n", timerSeconds64(MonitorWorkTime));
}


static int __cdecl MMOSTrafficCompare(const void* elem1Data, const void* elem2Data)
{
    const ModuleMemOperationStats** elem1 = (const ModuleMemOperationStats**)elem1Data;
    const ModuleMemOperationStats** elem2 = (const ModuleMemOperationStats**)elem2Data;
    double result = (*elem1)->stats[MOT_BALANCE-1].memTraffic.value - (*elem2)->stats[MOT_BALANCE-1].memTraffic.value;
    if(result < 0.0)
        return -1;
    else if(result > 0.0)
        return 1;
    else
        return 0;
}

static int __cdecl MMOSOpCountCompare(const void* elem1Data, const void* elem2Data)
{
    const ModuleMemOperationStats** elem1 = (const ModuleMemOperationStats**)elem1Data;
    const ModuleMemOperationStats** elem2 = (const ModuleMemOperationStats**)elem2Data;
    double result = (*elem1)->stats[MOT_BALANCE-1].count.value - (*elem2)->stats[MOT_BALANCE-1].count.value;
    if(result < 0.0)
        return -1;
    else if(result > 0.0)
        return 1;
    else
        return 0;
}

bool filterAll(ModuleMemOperationStats* stats)
{
    return true;
}

bool filterOneMeg(ModuleMemOperationStats* stats)
{
    if (stats->stats[MOT_BALANCE-1].memTraffic.value < 1024*1024)
        return false;
    return true;
}

bool filterNotShared(ModuleMemOperationStats* stats)
{
    return (!stats->bShared);
}

bool filterOnlyShared(ModuleMemOperationStats* stats)
{
    return (stats->bShared);
}


static void memMonitorGetStatsEx(MMOSSortCompare compare, int width, memMonitorFilter filter, OutputHandler handler, void *userdata)
{
    static ModuleMemOperationStats** moduleStats = NULL;

    if (!memMonitorActive())
    {
        return;
    }

    if(!compare)
    {
        compare = MMOSTrafficCompare;
    }
    
    if(!filter)
    {
        filter = filterAll;
    }

    if(!moduleStats)
    {
        eaCreateSmallocSafe(&moduleStats);
    }
    else
    {
        eaSetSize(&moduleStats, 0);
    }

    mmCRTHeapLock();
    {
        StashElement element;
        StashTableIterator it;

        stashGetIterator(MemMonitor.moduleStats, &it);
        while(stashGetNextElement(&it, &element))
        {
            ModuleMemOperationStats* stats;
            stats = stashElementGetPointer(element);
            assert(stats);

            eaPush(&moduleStats, stats);
        }
    }
    mmCRTHeapUnlock();

    {
        S32 i;
        U32 totalOutstandingMemory = 0;
        U32 totalCount = 0;

        eaQSort(moduleStats, compare);
        for(i = 0; i < eaSize(&moduleStats); i++)
        {
            ModuleMemOperationStats* stats;
            stats = moduleStats[i];
            if (filter(stats)) {
                // Print it!

                // Determine the width of the filename string
                int strwidth = width;
                char format[128];
#define MMTEXT_HEADER "Module name: "
#define MMTEXT_PAD "  "
                strwidth -= (int)strlen(MMTEXT_HEADER);
                strwidth -= (int)strlen(MMTEXT_PAD);
                strwidth -= 46; // Length of the counts printed at the end of the line

                if (strlen(stats->moduleName) >= (unsigned int)strwidth) {
                    sprintf_s(SAFESTR(format), MMTEXT_HEADER "...%%%ds" MMTEXT_PAD, strwidth-3);
                    handlerPrintf(handler, userdata, format, stats->moduleName + strlen(stats->moduleName) - (strwidth-3));
                } else {
                    sprintf_s(SAFESTR(format), MMTEXT_HEADER "%%%ds" MMTEXT_PAD, strwidth);
                    handlerPrintf(handler, userdata, format, stats->moduleName);
                }
                moPrintStats(handler, userdata, &stats->stats[MOT_BALANCE-1]);
                totalCount += (U32) stats->stats[MOT_BALANCE-1].count.value;
                handlerPrintf(handler, userdata, " trfc: %7.f\n", stats->stats[MOT_ALLOC-1].count.value);
            }
            if (strncmp(stats->moduleName + strlen(stats->moduleName) - 3, "-MP", 3)!=0)
                totalOutstandingMemory += (U32) stats->stats[MOT_BALANCE-1].memTraffic.value;
        }

        handlerPrintf(handler, userdata, "\nCollected stats for %i files\n", eaSize(&moduleStats));

        {
            UnitSpec* spec;
            spec = usFindProperUnitSpec(byteSpec, totalOutstandingMemory);
            handlerPrintf(handler, userdata, "Accounted for %6.2f %s total\n", (float)totalOutstandingMemory / spec->unitBoundary, spec->unitName);
            spec = usFindProperUnitSpec(byteSpec, totalOutstandingMemory - sharedMemoryContribution);
            handlerPrintf(handler, userdata, "Accounted for %6.2f %s (excluding shared)\n", (float)(totalOutstandingMemory-sharedMemoryContribution) / spec->unitBoundary, spec->unitName);
            spec = usFindProperUnitSpec(byteSpec, totalCount * 48);
            handlerPrintf(handler, userdata, "Total active allocation count: %d with overhead %6.2f %s\n", totalCount, (float)(totalCount * 48) / spec->unitBoundary, spec->unitName);
        }
    }
}

static void estrConcatHandler(char *appendMe, void *data)
{
	char **estrBuffer = data;
    estrConcatCharString(estrBuffer, appendMe);
}

const char *memMonitorGetStats(MMOSSortCompare compare, int width, memMonitorFilter filter)
{
    static char *buffer=NULL;
    estrClear(&buffer);
    memMonitorGetStatsEx(compare, width, filter, estrConcatHandler, &buffer);
    return buffer?buffer:"";
}

static void mmds(void) // Abbreviated version for running in Command window
{
    memMonitorDisplayStats();
}

static struct {
    size_t unused;
    size_t used;
} memPoolTotals;

static int fileWidth;
static int poolPrintIndex;

static void printMemoryPoolInfoHelper(MemoryPool pool, void *userData)
{
    int bgColor;
    int brightness;
    char fmt[128];
    char fileNameBuffer[1000];
    char* fileName = fileNameBuffer;
    size_t usedMemory = mpGetAllocatedCount(pool) * mpStructSize(pool);
    F32 percent;
    
    bgColor = poolPrintIndex % 5 ? 0 : COLOR_BLUE;
    brightness = COLOR_BRIGHT;
    
    poolPrintIndex++;

    memPoolTotals.used += usedMemory;
    memPoolTotals.unused += mpGetAllocatedChunkMemory(pool) - usedMemory;

    if(mpGetAllocatedChunkMemory(pool)){
        percent = (F32)(100.0 * (F32)(usedMemory) / mpGetAllocatedChunkMemory(pool));
    }else{
        percent = 100;
    }
    
    if(percent < 10){
        consoleSetColor(brightness|COLOR_RED, bgColor);
    }
    else if(percent < 25){
        consoleSetColor(brightness|COLOR_RED|COLOR_GREEN, bgColor);
    }
    else if(percent < 50){
        consoleSetColor(brightness|COLOR_GREEN|COLOR_BLUE, bgColor);
    }
    else{
        consoleSetColor(brightness|COLOR_GREEN, bgColor);
    }
        
    if(mpGetFileName(pool)){
        strcpy_s(fileName, ARRAY_SIZE_CHECKED(fileNameBuffer), mpGetFileName(pool));

        fileName = (char*)getFileName(fileNameBuffer);
        
        strcatf_s(SAFESTR(fileNameBuffer), ":%-4d", mpGetFileLine(pool));
        if (strlen(fileName) > (size_t)fileWidth) {
            strcpy_s(fileName+2, ARRAY_SIZE_CHECKED(fileNameBuffer) - (fileName+2-fileNameBuffer), fileName + strlen(fileName)-(fileWidth - 2));
            fileName[0]=fileName[1]='.';
        }
    }else{
        strcpy_s(fileName, ARRAY_SIZE_CHECKED(fileNameBuffer), "");
    }
    
    sprintf_s(SAFESTR(fmt), "%%-30s:%%%ds  %%9s %%9s %%12s %%12s  %%5.1f%%%%\n", fileWidth);
    if(!userData)
    {
        printf(    fmt,
            mpGetName(pool) ? mpGetName(pool) : "???",
            fileName,
            getCommaSeparatedInt(mpGetAllocatedCount(pool)),
            getCommaSeparatedInt(mpStructSize(pool)),
            getCommaSeparatedInt(mpGetAllocatedChunkMemory(pool)),
            getCommaSeparatedInt(mpGetAllocatedChunkMemory(pool) - usedMemory),
            percent);
    }
    else
    {
        estrConcatf(userData, fmt,
            mpGetName(pool) ? mpGetName(pool) : "???",
            fileName,
            getCommaSeparatedInt(mpGetAllocatedCount(pool)),
            getCommaSeparatedInt(mpStructSize(pool)),
            getCommaSeparatedInt(mpGetAllocatedChunkMemory(pool)),
            getCommaSeparatedInt(mpGetAllocatedChunkMemory(pool) - usedMemory),
            percent);
    }
    
    consoleSetDefaultColor();
}

#ifndef _XBOX
static void printMemoryPoolInfo_ex(COORD coord, OutputHandler handler, void *userdata){
    char fmt[256];
    char header[256];
    char *estr = estrTemp();

    ZeroStruct(&memPoolTotals);
    
    fileWidth = CLAMP(coord.X-88,8,40);

    handler("\n", userdata);
    sprintf_s(SAFESTR(fmt), "--- MEMORY POOLS: -------Name-%%%ds--Allocated--StructSize----Memory--------Unused----Used--\n", fileWidth);
    sprintf_s(SAFESTR(header), fmt, "File");
    handler(header, userdata);
    //printf(fmt, "File");
    poolPrintIndex = 0;
    mpForEachMemoryPool(printMemoryPoolInfoHelper, userdata);
    
    estrPrintf(&estr, "Total Used:   %s bytes (%1.2f%%)\n"
        "Total Unused: %s bytes\n",
        getCommaSeparatedInt(memPoolTotals.used),
        100.0 * (F32)memPoolTotals.used / (memPoolTotals.used + memPoolTotals.unused),
        getCommaSeparatedInt(memPoolTotals.unused));
    handler(estr, userdata);
    handler("------------------------------------------------------------------------------\n", userdata);
    estrDestroy(&estr);
}
static void printMemoryPoolInfo()
{
    COORD coord = {0};
    consoleGetDims(&coord);
    printMemoryPoolInfo_ex(coord, defaultHandler, NULL);
}
#endif

static void printMemorySizeInfo()
{
    S32                     totalSize = 0;
    S32                     totalAllocs = 0;
    MemorySizeAllocation    total = {0};
    MemorySizeAllocation    delta = {0};
    MemorySizeAllocation*    cur;

    printf(    "%10s: %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s\n",
            "maxsize",
            "count",
            "max",
            "delta",
            "size",
            "max",
            "delta",
            "avg",
            "traffic",
            "delta",
            "maxSize",
            "minSize");
            
    for(cur = memoryAllocations; cur->maxAllocationRange >= 0; cur++)
    {
        S32 curCount = cur->curCount;
        S32 curTotalSize = cur->curTotalSize;
        S32 deltaCount = curCount - cur->lastCount;
        S32 deltaTotalSize = curTotalSize - cur->lastTotalSize;
        S32 curTraffic = cur->curTraffic;
        S32 deltaTraffic = curTraffic - cur->lastTraffic;
        
        total.curCount        += curCount;
        total.curTotalSize    += curTotalSize;
        total.curTraffic    += curTraffic;
        
        delta.curCount        += deltaCount;
        delta.curTotalSize    += deltaTotalSize;
        delta.curTraffic    += deltaTraffic;
        
        cur->lastCount = curCount;
        cur->lastTotalSize = curTotalSize;
        cur->lastTraffic = curTraffic;
        
        totalSize += curTotalSize;
        totalAllocs += curCount;

        printf(    "%10d: %10d %10d %10d %10d %10d %10d %10.1f %10d %10d %10d %10d\n",
                cur->maxAllocationRange,
                curCount,
                cur->maxCount,
                deltaCount,
                curTotalSize,
                cur->maxTotalSize,
                deltaTotalSize,
                cur->curCount ? (F32)cur->curTotalSize / cur->curCount : 0.0,
                curTraffic,
                deltaTraffic,
                cur->maxAllocSize,
                cur->maxAllocSize ? cur->minAllocSize : 0);
    }
    
    printf(    "%10s: %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s\n",
            "maxsize",
            "count",
            "max",
            "delta",
            "size",
            "max",
            "delta",
            "avg",
            "traffic",
            "delta",
            "maxSize",
            "minSize");

    consoleSetFGColor(COLOR_BRIGHT|COLOR_GREEN);
    
    printf(    "%10s: %10d %10s %10d %10d %10s %10d %10s %10d %10d\n",
            "Totals",
            total.curCount,
            "",
            delta.curCount,
            total.curTotalSize,
            "",
            delta.curTotalSize,
            "",
            total.curTraffic,
            delta.curTraffic);

    consoleSetDefaultColor();
}

FORCEINLINE static void memMonitorStats_ex(char **output)    //if there's output, then we're writing to a file
{
    COORD coord = {0};
    OutputHandler handler = (output)?estrConcatHandler:defaultHandler;

    if(!output)
        consoleGetDims(&coord);
    else
        coord.X=MEMLOGDUMP_PAGEWIDTH;


    printMemoryPoolInfo_ex(coord, handler, output);

    memMonitorGetStatsEx(MMOSTrafficCompare, CLAMP(coord.X,64,140), filterAll, handler, output);
    //printf("\nLooking for modules: %f\n", (F32)g_timespent / (F32)getRegistryMhz());

    if(0)
    {
        printMemorySizeInfo();
    }

    if (0) {
        mmds(); // Just to ensure this function gets linked in so we can call it from the Command window
        memCheckDumpAllocs();
    }
}

void memMonitorLogStats()
{
#ifndef _XBOX
    char *estr = estrTemp();
    char    date_buf[100];
    char filename_buf[150] = {0};
    timerMakeDateString_fileFriendly(date_buf);
    sprintf(filename_buf, "MemoryDump %s", date_buf);

    // Free up allocations from our previous call to filelog_printf
    logShutdownAndWait();

    memMonitorStats_ex(&estr);

    filelog_printf(filename_buf, "%s", estr);
    estrDestroy(&estr);
#endif
}

void memMonitorDisplayStats()
{
    memMonitorStats_ex(NULL);
}

U64 memMonitorBytesAlloced()
{
    U64 totalOutstandingMemory = 0;
    U32 totalCount = 0;

    if (!memMonitorActive())
    {
        return 0;
    }

    mmCRTHeapLock();
    {
        StashElement element;
        StashTableIterator it;

        stashGetIterator(MemMonitor.moduleStats, &it);
        while(stashGetNextElement(&it, &element))
        {
            ModuleMemOperationStats* stats;
            stats = stashElementGetPointer(element);
            assert(stats);

            totalCount += (U32)stats->stats[MOT_BALANCE-1].count.value;
            if (strncmp(stats->moduleName + strlen(stats->moduleName) - 3, "-MP", 3)!=0)
                totalOutstandingMemory += (U64)stats->stats[MOT_BALANCE-1].memTraffic.value;
        }
    }
    mmCRTHeapUnlock();

    return totalOutstandingMemory;
}




