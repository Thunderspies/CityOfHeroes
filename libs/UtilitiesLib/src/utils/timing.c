#define TIMING_C

#include <string.h>
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/file.h"

#if defined(_WIN32) || defined(_XBOX)
#include <stdio.h>
#include <limits.h>
#include "utilitieslib/utils/wininclude.h"
#include <sqltypes.h>
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/mathutil.h"
#include <time.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/osdependent.h"


#ifndef _XBOX
    // This pragma forces including winmm.lib, which is needed for the timeGetTime below.
    #pragma comment(lib, "winmm.lib")
#endif




//----------------------------------------------------------------------------------------------
//
// These have to be at the top, because these are used by timing code, so we don't want them timed.
//
// #undef EnterCriticalSection
// #undef LeaveCriticalSection
// #undef Sleep
//
//----------------------------------------------------------------------------------------------

TimingState timing_state;

int    timing_s2000_delta = 0;
int timing_s2000_nonLocalDelta = 0;
int timing_debug_offset = 0;        // offset added to timerGetTime()
static U32 seconds_offset=0;


/*
    Time functions:
    timerCpuTicks / timerCpuSpeed
        32 bit timers that are scaled to flip over about once every 24 hours
    timerCpuTicks64 / timerCpuSpeed64
        the raw result from the high speed cpu timer
    timerAlloc, timerFree, timerStart, timerElapsed, timerPause, timerUnpause, timerAdd
        float convenience functions that use the 64 bit timer internally

    Date functions:
        timerSecondsSince2000, timerGetSecondsSince2000FromString, timerMakeDateStringFromSecondsSince2000
            Should be good till 2136
*/

int shiftval;

S64 getRegistryMhz()
{
// Get the processor speed info.
    static S64    mhz;

    if(!mhz)
    {
#ifndef _XBOX
        int        result;
        HKEY    hKey;
        int        data = 0;
        int        dataSize = sizeof(int);
        result = RegOpenKeyExA(HKEY_LOCAL_MACHINE,"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
        if (result != ERROR_SUCCESS)
            return 0;
        result = RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE)&data, &dataSize);
        RegCloseKey (hKey);

        mhz = data;
        mhz *= 1000000;
#else
        mhz = 3000000; // umm i think it was 3 mhz... just putting some random number in :)
#endif
    }

    return mhz;
}

void timerPerfInfoAssert(const char* file, int line, PerformanceInfo* curTimer, const char* functionName)
{
    if(!timing_state.assert_shown)
    {
        devassertmsg(0, "Performance counter stopped in a different function than started.\n"
                "Started in: %s/%s\n"
                "Stopped in: %s\n",
                curTimer->functionName,
                curTimer->locName,
                functionName);

        timing_state.assert_shown = 1;
    }
}

MP_DEFINE(PerformanceInfo);

static PerformanceInfo* createPerformanceInfo()
{
    MP_CREATE(PerformanceInfo, 100);

    return MP_ALLOC(PerformanceInfo);
}

static void destroyPerformanceInfo(PerformanceInfo* info)
{
    if(info)
        MP_FREE(PerformanceInfo, info);
}

typedef struct RegisteredAutoTimer
{
    struct RegisteredAutoTimer*        next;
    PerformanceInfo**                timer;
} RegisteredAutoTimer;

MP_DEFINE(RegisteredAutoTimer);

static RegisteredAutoTimer* createRegisteredAutoTimer()
{
    MP_CREATE(RegisteredAutoTimer, 100);

    return MP_ALLOC(RegisteredAutoTimer);
}

static void destroyRegisteredAutoTimer(RegisteredAutoTimer* timer)
{
    MP_FREE(RegisteredAutoTimer, timer);
}

static RegisteredAutoTimer* registeredTimerList;

static PerformanceInfo* __fastcall timerInitAutoTimer(PerformanceInfo** ptimer, PerformanceInfo** proot, const char* locName, int line)
{
    int old_runperfinfo;
    PerformanceInfo* timer;
    
    // Turn off the auto timers when allocating stuff, to prevent recursive calls from memory hooks.
    
    old_runperfinfo = timing_state.runperfinfo;
    timing_state.runperfinfo = 0;
    
    timer = *ptimer = createPerformanceInfo();

    if(ptimer == proot)
    {
        RegisteredAutoTimer* regTimer = createRegisteredAutoTimer();

        regTimer->next = registeredTimerList;
        registeredTimerList = regTimer;
        regTimer->timer = ptimer;
    }
    
    // Turn it back on.

    timing_state.runperfinfo = old_runperfinfo;

    if(timing_state.autoTimerMasterList.tail)
    {
        timing_state.autoTimerMasterList.tail->nextMaster = timer;
        timing_state.autoTimerMasterList.tail = timer;
    }
    else
    {
        timing_state.autoTimerMasterList.head = timer;
        timing_state.autoTimerMasterList.tail = timer;
    }
    
    timer->rootStatic = proot;
    timer->nextMaster = NULL;

    timer->uid = timing_state.autoTimerCount++;
    timer->locName = locName;
    
    timer->child.head = NULL;
    timer->child.tail = NULL;

    timer->open = 0;
    timer->hidden = 0;

    assert(!timer->parent);

    timing_state.autoTimersLayoutID++;

    if(timing_state.autoStackTop)
    {
        PerformanceInfo* parent = timing_state.autoStackTop;
        
        if(parent->child.tail)
            parent->child.tail->nextSibling = timer;
        else
            parent->child.head = timer;

        parent->child.tail = timer;
        timer->parent = parent;
        
        assert(parent);
    }
    else
    {
        timer->parent = NULL;

        // This timer has no parent, so put it into the root list.

        if(timing_state.autoTimerRootList)
        {
            PerformanceInfo* tail = timing_state.autoTimerRootList;
            
            while(tail->nextSibling)
            {
                tail = tail->nextSibling;
            }
                
            tail->nextSibling = timer;
            timer->nextSibling = NULL;
        }
        else
        {
            timing_state.autoTimerRootList = timer;
        }
    }
    
    timer->nextSibling = NULL;
    timer->child.head = NULL;
    timer->child.tail = NULL;
    
    return timer;
}

void __fastcall timerCheckLocName(const char* locNameStop)
{
    if(!timing_state.assert_shown)
    {
        const char* locNameStart = timing_state.autoStackTop->locName;
        
        if(strcmp(locNameStop, locNameStart))
        {
            if(!timing_state.autoTimerDisablePopupErrors){
                devassertmsg(0, "AutoTimer start/stop location mismatch:\n"
                        "\n"
                        "Stop: %s\n"
                        "\n"
                        "Start: %s\n",
                        locNameStop,
                        locNameStart);
            }
            
            timing_state.assert_shown = 1;
        }
    }
}

void __fastcall timerStackUnderFlow()
{
    if(!timing_state.autoTimerDisablePopupErrors){
        devassertmsg(0, "AutoTimer stack underflow.");
    }
    
    timing_state.assert_shown = 1;
}

int __fastcall timerGetCurrentThread()
{
    return (int)GetCurrentThreadId();
}


void timerSetBreakPoint(U32 layoutID, U32 uid, S32 set)
{
    if(layoutID >= timing_state.autoTimersLastResetLayoutID && layoutID <= timing_state.autoTimersLayoutID)
    {
        PerformanceInfo* cur;
        
        for(cur = timing_state.autoTimerMasterList.head; cur; cur = cur->nextMaster)
        {
            if(cur->uid == uid)
            {
                cur->breakMe = set ? 1 : 0;
                break;
            }
        }
    }
}

static U8*                writeBuffer = NULL;
static int                writeBufferSize = 0;
static const int        writeBufferMaxSize = 128 * 1024;
static FILE*            recordFileHandle;

static U8*                readBuffer = NULL;
static int                readBufferSize = 0;
static int                readBufferCursor;
static const int        readBufferMaxSize = 128 * 1024;
static FILE*            playbackFileHandle;
static U32                playbackFileVersion;
static StashTable        playbackLocNamesHT;
static int                printedReadError;

static void invertDataScramble(U8* buffer, int bufferSize)
{
    int i;
    for(i = 0; i < bufferSize; i++)
    {
        buffer[i] ^= (i*i + 128) & 0xff;
    }
}

static void fwriteFlushData(){
    if(!writeBufferSize || !recordFileHandle){
        return;
    }
    
    invertDataScramble(writeBuffer, writeBufferSize);
    fwrite(writeBuffer, writeBufferSize, 1, recordFileHandle);
    
    writeBufferSize = 0;
}

static void timerRecordCloseFile(){
    if(!recordFileHandle)
    {
        return;
    }

    fwriteFlushData();
    fileClose(recordFileHandle);
    recordFileHandle = NULL;
    timing_state.autoTimerRecordLayoutID = 0;

    if(timing_state.runperfinfo)
    {
        timing_state.resetperfinfo_init = 0;
    }
}

void timerFlushFileBuffers()
{
    PERFINFO_RUN(
        if(recordFileHandle)
        {
            timerRecordCloseFile();
        }
    );
}

static void fwriteData(const char* data, int size){
    if(!recordFileHandle){
        return;
    }
    
    if(!writeBuffer){
        writeBuffer = malloc(writeBufferMaxSize);
    }
    
    while(size){
        int remaining = writeBufferMaxSize - writeBufferSize;
        int writeSize = min(remaining, size);
        
        memcpy(writeBuffer + writeBufferSize, data, writeSize);
        
        writeBufferSize += writeSize;
        data += writeSize;
        size -= writeSize;
        
        if(writeBufferSize == writeBufferMaxSize){
            fwriteFlushData();
        }
    }
}

#define FWRITE(x) fwriteData(&x, sizeof(x))
#define FWRITE_U8(x) {U8 i__ = (x);FWRITE(i__);}
#define FWRITE_U32(x) {fwriteCompressedU32(x);}
#define FWRITE_U64(x) {fwriteCompressedU64(x);}
#define FWRITE_STRING(x) fwriteData(x, (int)strlen(x) + 1);

static void freadGetMoreData(){
    if(!readBuffer){
        readBuffer = malloc(readBufferMaxSize);
    }
    
    readBufferSize = fread(readBuffer, 1, readBufferMaxSize, playbackFileHandle);
    readBufferCursor = 0;
    
    invertDataScramble(readBuffer, readBufferSize);
}

static void freadData(char* data, int size){
    if(!playbackFileHandle){
        return;
    }
    
    while(size){
        int remaining = readBufferSize - readBufferCursor;
        int readSize = min(remaining, size);
        
        memcpy(data, readBuffer + readBufferCursor, readSize);
        
        readBufferCursor += readSize;
        data += readSize;
        size -= readSize;
        
        if(readBufferCursor == readBufferSize){
            freadGetMoreData();
            
            if(!readBufferSize)
            {
                return;
            }
        }
    }
}

#define FREAD(x) freadData(&x, sizeof(x))
#define FREAD_U8(x) {U8 i__;FREAD(i__);(x) = i__;}
#define FREAD_U32(x) {x = freadCompressedU32();}
#define FREAD_U64(x) {x = freadCompressedU64();}
#define FREAD_STRING(x) devassert((char*)&x == x),freadString(x, ARRAY_SIZE(x))
#define printfReadError if(!printedReadError)printedReadError=1,printf("Profiler Read ERROR: "),printf

static void fwriteCompressedU32(U32 value){
    int i;
    
    for(i = 0; i < sizeof(value); i++){
        if(i == sizeof(value) - 1 || value < (U32)1 << (6 + (i * 8))){
            U32 valueWrite = i | (value << 2);
            int j;
            for(j = 0; j <= i; j++){
                FWRITE_U8((valueWrite >> (j * 8)) & 0xff);
            }
            if(i == sizeof(value) - 1){
                FWRITE_U8((value >> (sizeof(value) * 8 - 2)) & 0xff);
            }
            break;
        }
    }
}

static U32 freadCompressedU32(){
    U32 value;
    U32 i;
    U32 valueSize;
    
    FREAD_U8(valueSize);
    
    value = valueSize >> 2;
    valueSize &= 3;
    
    if(valueSize == 3)
    {
        valueSize = 4;
    }
    
    for(i = 0; i < valueSize; i++)
    {
        U32 aByte;
        FREAD_U8(aByte);
        value |= aByte << (6 + (i * 8));
    }
    
    return value;
}

static void fwriteCompressedU64(U64 value){
    int i;
    
    for(i = 0; i < sizeof(value); i++){
        if(i == sizeof(value) - 1 || value < (U64)1 << (5 + (i * 8))){
            U64 valueWrite = i | (value << 3);
            int j;
            for(j = 0; j <= i; j++){
                FWRITE_U8((U8)((valueWrite >> (j * 8)) & 0xff));
            }
            if(i == sizeof(value) - 1){
                FWRITE_U8((U8)((value >> (sizeof(value) * 8 - 3)) & 0xff));
            }
            break;
        }
    }
}    

static U64 freadCompressedU64(){
    U64 value;
    U32 i;
    U32 valueSize;
    
    FREAD_U8(valueSize);
    
    value = valueSize >> 3;
    valueSize &= 7;
    
    if(valueSize == 7)
    {
        valueSize = 8;
    }
    
    for(i = 0; i < valueSize; i++)
    {
        U64 aByte;
        FREAD_U8(aByte);
        value |= aByte << (5 + (i * 8));
    }
    
    return value;
}

static void freadString(char* output, int maxSize)
{
    if(!maxSize)
    {
        return;
    }
        
    for(; maxSize; output++, maxSize--)
    {
        FREAD_U8(*output);
        
        if(!*output)
        {
            return;
        }
    }
    
    output[-1] = '\0';
}

static void timerRecordWriteSiblingsLayout(PerformanceInfo* first){
    PerformanceInfo* cur;
    int sibCount = 0;
    
    for(cur = first; cur; cur = cur->nextSibling)
    {
        sibCount++;
    }

    FWRITE_U32(sibCount);

    for(cur = first; cur; cur = cur->nextSibling)
    {
        FWRITE_STRING(cur->locName);
        FWRITE_U8(cur->infoType);
        
        timerRecordWriteSiblingsLayout(cur->child.head);
    }
}

static void timerRecordWriteLayout(){
    timerRecordWriteSiblingsLayout(timing_state.autoTimerRootList);
}

static void timerRecordWriteCPU(PerformanceInfo* cur){
    for(; cur; cur = cur->nextSibling){
        FWRITE_U64(cur->history[cur->historyPos].cycles);
        
        if(cur->history[cur->historyPos].cycles){
            FWRITE_U32(cur->history[cur->historyPos].count);
        }
        
        timerRecordWriteCPU(cur->child.head);
    }
}

static void timerGetProfilerFileName(const char* input, char* output, size_t output_size, int makeDirs)
{
    if(strncmp(input,"\\\\",2)==0 || strchr(input,':'))
    {
        strcpy_s(output, output_size, input);
    }
    else
    {
        char* prefix = ".cohprofiler";
        U32 prefixLen = (U32)strlen(prefix);
        char buf[1000];
        char* s;
        
        strcpy(buf, fileDataDir());

        s = strrchr(buf, '/');

        if(s)
        {
            *s = 0;
        }

        sprintf_s(output, output_size,
                "%s/profiler/%s%s",
                buf,
                input,
                strlen(input) <= prefixLen || stricmp(strchr(input, 0)/* + strlen(fname) - prefixLen*/, prefix) ? prefix : "");
    }

    if(makeDirs)
    {
        char fileDir[1000];
        strcpy(fileDir, output);
        makeDirectories(getDirectoryName(fileDir));
    }
}

static void timerRecordInit()
{
    char fileName[1000];
    
    if(!timing_state.autoTimerRecordFileName)
    {
        return;
    }
 
     timerGetProfilerFileName(timing_state.autoTimerRecordFileName, fileName, ARRAY_SIZE(fileName), 1);
    SAFE_FREE(timing_state.autoTimerRecordFileName);

    recordFileHandle = fileOpen(fileName, "wb");
    
    if(recordFileHandle)
    {
        timing_state.autoTimerRecordLayoutID = -1;
        timing_state.runperfinfo_init = -1;
    }

    timing_state.autoTimerProfilerCloseFile = 0;

    // Write the version number.

    FWRITE_U32(0);
    
    // Write the CPU speed.
    
    FWRITE_U64(getRegistryMhz());
}

static void timerRecordStep()
{
    if(timing_state.autoTimerRecordLayoutID    != timing_state.autoTimersLayoutID)
    {
        timing_state.autoTimerRecordLayoutID = timing_state.autoTimersLayoutID;
        
        FWRITE_U8(1);
        FWRITE_STRING("newlayout");
        
        timerRecordWriteLayout();
    }
    else
    {
        FWRITE_U8(0);
    }

    FWRITE_STRING("frameBegin");
    
    FWRITE_U64(timing_state.totalTime);
    
    {
        int size = ARRAY_SIZE(timing_state.totalCPUTimePassed);
        // int pos = (int)((timing_state.totalCPUTimePos - 1 + size) % size);
        FWRITE_U64(timing_state.totalCPUTimePassed[size - 1]);
    }
    
    timerRecordWriteCPU(timing_state.autoTimerRootList);
    FWRITE_STRING("frameEnd");

    if(timing_state.autoTimerProfilerCloseFile)
    {
        timing_state.autoTimerProfilerCloseFile = 0;
        timerRecordCloseFile();
    }

    SAFE_FREE(timing_state.autoTimerRecordFileName);
}

static void timerPlaybackInit()
{
    char fileName[1000];
    
    if(playbackFileHandle || !timing_state.autoTimerPlaybackFileName)
    {
        return;
    }
 
     timerGetProfilerFileName(timing_state.autoTimerPlaybackFileName, fileName, ARRAY_SIZE(fileName), 1);
    SAFE_FREE(timing_state.autoTimerPlaybackFileName);
    
    playbackFileHandle = fileOpen(fileName, "rb");
    
    if(playbackFileHandle)
    {
        printf("playing profiler file: %s\n", fileName);
        
        timing_state.autoTimerRecordLayoutID = 0;
        timing_state.runperfinfo = 0;
        
        printedReadError = 0;
        readBufferCursor = 0;
        readBufferSize = 0;
        readBuffer = NULL;
    }


    timing_state.autoTimerProfilerCloseFile = 0;

    // Read the version number.

    FREAD_U32(playbackFileVersion);
    
    // Read the cpu speed.
    
    FREAD_U64(timing_state.autoTimerPlaybackCPUSpeed);
}

static const char* timerPlaybackAllocateName(char* name)
{
    StashElement element;
    
    if(!playbackLocNamesHT)
    {
        playbackLocNamesHT = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys | StashCaseSensitive );
    }
    
    stashFindElement(playbackLocNamesHT, name, &element);
    
    if(!element)
    {
        stashAddPointerAndGetElement(playbackLocNamesHT, name, NULL, false, &element);
    }

    assert(element);
    
    return stashElementGetStringKey(element);
}

static int timerPlaybackReadLayout(PerformanceInfo* parent, PerformanceInfo** piPtr)
{
    char readName[1000];
    
    int sibCount;
    int i;
    
    FREAD_U32(sibCount);
    
    for(i = 0; i < sibCount; i++)
    {
        FREAD_STRING(readName);
        
        if(!*piPtr)
        {
            timing_state.autoTimerCount++;
            
            *piPtr = createPerformanceInfo();
            (*piPtr)->locName = timerPlaybackAllocateName(readName);
            
            (*piPtr)->parent = parent;
            (*piPtr)->nextMaster = timing_state.autoTimerMasterList.head;
            timing_state.autoTimerMasterList.head = *piPtr;

            if(!timing_state.autoTimerMasterList.tail)
            {
                timing_state.autoTimerMasterList.tail = *piPtr;
            }
        }
        else
        {
            if(stricmp((*piPtr)->locName, readName))
            {
                printfReadError("Mismatched timer names: %s, %s\n", (*piPtr)->locName, readName);
                return 0;
            }
        }
        
        FREAD_U8((*piPtr)->infoType);

        if(!timerPlaybackReadLayout(*piPtr, &(*piPtr)->child.head))
        {
            printfReadError("Can't read children: %s\n", (*piPtr)->locName);
            return 0;
        }
        
        if(!(*piPtr)->child.tail)
        {
            (*piPtr)->child.tail = (*piPtr)->child.head;
        }
        
        while((*piPtr)->child.tail && (*piPtr)->child.tail->nextSibling)
        {
            (*piPtr)->child.tail = (*piPtr)->child.tail->nextSibling;
        }
        
        piPtr = &(*piPtr)->nextSibling;
    }
    
    return 1;
}

static void timerPlaybackReadCPU(PerformanceInfo* cur)
{
    for(; cur; cur = cur->nextSibling)
    {
        cur->historyPos = (cur->historyPos + 1) % ARRAY_SIZE(cur->history);
        FREAD_U64(cur->history[cur->historyPos].cycles);
        cur->totalTime += cur->history[cur->historyPos].cycles;
        
        if(cur->history[cur->historyPos].cycles)
        {
            FREAD_U32(cur->history[cur->historyPos].count);
            cur->opCountInt += cur->history[cur->historyPos].count;
        }
        
        timerPlaybackReadCPU(cur->child.head);
    }
}

static void timerPlaybackReadStep()
{
    char buffer[100];
    int newLayout;
    
    if(!playbackFileHandle)
    {
        return;
    }

    FREAD_U8(newLayout);
    
    if(newLayout){
        FREAD_STRING(buffer);
        
        if(strcmp(buffer, "newlayout"))
        {
            printfReadError("Didn't find keyword: newlayout\n");
            return;
        }
        
        if(!timerPlaybackReadLayout(NULL, &timing_state.autoTimerRootList))
        {
            printfReadError("Can't read layout.\n");
            return;
        }
    }
    
    FREAD_STRING(buffer);
    if(strcmp(buffer, "frameBegin"))
    {
        U8 tempU8;
        
        FREAD_U8(tempU8);
        FREAD_U8(tempU8);

        FREAD_STRING(buffer);
        if(strcmp(buffer, "frameBegin"))
        {
            printfReadError("Didn't find keyword: frameBegin\n");
            return;
        }
    }
    
    FREAD_U64(timing_state.totalTime);
    FREAD_U64(timing_state.totalCPUTimePassed[0]);
    timing_state.totalCPUTimePos = 0;

    timerPlaybackReadCPU(timing_state.autoTimerRootList);
    
    FREAD_STRING(buffer);
    if(strcmp(buffer, "frameEnd"))
    {
        printfReadError("Didn't find keyword: frameEnd\n");
        return;
    }
}

static void timerPlaybackRun()
{
    int stepCount = 0;
    int i;
    
    if(!playbackFileHandle)
    {
        return;
    }

    if(timing_state.autoTimerPlaybackState)
    {
        stepCount = timing_state.autoTimerPlaybackStepSize;

        if(timing_state.autoTimerPlaybackState > 0)
        {
            timing_state.autoTimerPlaybackState = 0;
        }
    }
    
    for(i = 0; i < stepCount; i++){
        timerPlaybackReadStep();
    }
    
    if(timing_state.autoTimerProfilerCloseFile)
    {
        fileClose(playbackFileHandle);
        playbackFileHandle = NULL;
        SAFE_FREE(readBuffer);
        
        timing_state.resetperfinfo_init = 1;
    }
}

void timerTickBegin()
{
    static int init = 0;
    
    S64 oldTotalTimeStart;
    
    if(!init)
    {
        init = 1;
        
        timing_state.autoStackMaxDepth = 9;
        timing_state.autoStackMaxDepth_init = INT_MIN;
        
        timing_state.autoTimerThreadID = timerGetCurrentThread();
    }

    if(timing_state.runperfinfo_init != INT_MIN)
    {
        timing_state.runperfinfo = timing_state.runperfinfo_init;
        timing_state.runperfinfo_init = INT_MIN;
        
        timing_state.autoStackDepth = 0;
        timing_state.autoTimersLayoutID++;
        
        if(!timing_state.runperfinfo)
        {
            timing_state.resetperfinfo_init = 1;
            timerRecordCloseFile();
        }
    }
    
    if(timing_state.resetperfinfo_init != INT_MIN)
    {
        int old_runperfinfo;
        
        timing_state.resetperfinfo_init = INT_MIN;
        timing_state.assert_shown = 0;

        timing_state.autoTimerCount = 0;
        timing_state.autoStackDepth = 0;
        timing_state.autoTimersLastResetLayoutID = ++timing_state.autoTimersLayoutID;
        timing_state.totalTime = 0;
        timing_state.send_reset = 1;
        
        old_runperfinfo = timing_state.runperfinfo;
        timing_state.runperfinfo = 0;
        
        while(timing_state.autoTimerMasterList.head)
        {
            PerformanceInfo* next = timing_state.autoTimerMasterList.head->nextMaster;
            destroyPerformanceInfo(timing_state.autoTimerMasterList.head);
            timing_state.autoTimerMasterList.head = next;
        }

        timing_state.autoTimerRootList = NULL;

        timing_state.autoTimerMasterList.head = NULL;
        timing_state.autoTimerMasterList.tail = NULL;
        
        // Clear all the registered timers.
        
        while(registeredTimerList)
        {
            RegisteredAutoTimer* next = registeredTimerList->next;
            *registeredTimerList->timer = NULL;
            destroyRegisteredAutoTimer(registeredTimerList);
            registeredTimerList = next;
        }
        
        // Free memory pool memory.
        
        if(!mpGetAllocatedCount(MP_NAME(RegisteredAutoTimer)))
        {
            mpFreeAllocatedMemory(MP_NAME(RegisteredAutoTimer));
        }
        
        if(!mpGetAllocatedCount(MP_NAME(PerformanceInfo)))
        {
            mpFreeAllocatedMemory(MP_NAME(PerformanceInfo));
        }

        timing_state.runperfinfo = old_runperfinfo;
    }
    
    if(!recordFileHandle)
    {
        timerPlaybackInit();
        
        timerPlaybackRun();
    }
    else
    {
        SAFE_FREE(timing_state.autoTimerPlaybackFileName);
    }
    
    if(timing_state.runperfinfo)
    {
        PerformanceInfo* cur;

        // Goto next history position.
        
        for(cur = timing_state.autoTimerMasterList.head; cur; cur = cur->nextMaster)
        {
            cur->historyPos = (cur->historyPos + 1) % ARRAY_SIZE(cur->history);
            ZeroStruct(&cur->history[cur->historyPos]);
        }
    }
    
    if(timing_state.autoStackMaxDepth_init != INT_MIN)
    {
        timing_state.autoStackMaxDepth = timing_state.autoStackMaxDepth_init;
        timing_state.autoStackMaxDepth_init = INT_MIN;
    }
    
    if(timing_state.autoStackMaxDepth < 1)
    {
        timing_state.autoStackMaxDepth = 1;
    }
    
    oldTotalTimeStart = timing_state.totalTimeStart;
    GET_CPU_TICKS_64(timing_state.totalTimeStart);
    timing_state.autoStackDepth = 0;
    timing_state.autoStackTop = NULL;

    PERFINFO_RUN(timing_state.totalCPUTimePassed[timing_state.totalCPUTimePos] = timing_state.totalTimeStart - oldTotalTimeStart;);
    PERFINFO_RUN(timing_state.totalCPUTimePos = (timing_state.totalCPUTimePos + 1) % ARRAY_SIZE(timing_state.totalCPUTimePassed););
}

void timerTickEnd()
{
    S64 curTicks;

    GET_CPU_TICKS_64(curTicks);

    if(!playbackFileHandle)
    {
        if(recordFileHandle)
        {
            timerRecordStep();
        }
        else
        {
            timerRecordInit();
        }
    }
    
    if(PERFINFO_RUN_CONDITIONS)
    {
        timing_state.totalCPUTimeUsed[timing_state.totalCPUTimePos] = curTicks - timing_state.totalTimeStart;
        
        timing_state.totalTime += curTicks - timing_state.totalTimeStart;
        
        if(!timing_state.assert_shown && timing_state.autoStackDepth != 0)
        {
            timing_state.assert_shown = 1;
            
            if(!timing_state.autoTimerDisablePopupErrors)
            {                
                devassertmsg(0, "AutoTimer stack overflow.");
            }
        }

        if(timing_state.runperfinfo > 0)
        {
            timing_state.runperfinfo--;
        }
    }
}

void __fastcall timerAutoTimerAlloc(PerformanceInfo** root, const char* locName)
{
    PerformanceInfo* parent;
    PerformanceInfo* cur = *root;
    PerformanceInfo** pcur = root;

    if(timing_state.autoStackDepth > 0)
        parent = timing_state.autoStackTop;
    else
        parent = NULL;

    while(1)
    {
        if(!cur)
        {
            cur = timerInitAutoTimer(pcur, root, locName, __LINE__);
            break;
        }
        else
        {
            if(parent == cur->parent)
            {
                break;
            }
            else if(parent < cur->parent)
            {
                pcur = &cur->branch.lo;
                cur = cur->branch.lo;
            }
            else
            {
                pcur = &cur->branch.hi;
                cur = cur->branch.hi;
            }
        }
    }

    if(cur->breakMe)
    {
        int old_runperfinfo = timing_state.runperfinfo;
        
        cur->breakMe = 0;
        
        // Hit a break.  Go ahead and continue.
        
        timing_state.runperfinfo = 0;

        assertmsg(    0,
                    "Hey, you hit an auto timer break.\n"
                    "\n"
                    "Click DEBUG to do some stuff to it.\n"
                    "\n"
                    "Click IGNORE to continue running normally.\n");

        timing_state.runperfinfo = old_runperfinfo;
    }
    
    timing_state.autoStackTop = cur;
}                                                                    

void __fastcall timerAutoTimerAllocBits(PerformanceInfo** root, const char* locName)
{
    timerAutoTimerAlloc(root, locName);
    
    timing_state.autoStackTop->infoType = PERFINFO_TYPE_BITS;
}

void __fastcall timerAutoTimerAllocMisc(PerformanceInfo** root, const char* locName)
{
    timerAutoTimerAlloc(root, locName);
    
    timing_state.autoStackTop->infoType = PERFINFO_TYPE_MISC;
}

//#define USE_LOW_PRECISION_TIMERS 1

#if 0
    static S64 calcMhz()
    {
        int t1 = time(0);
        int t2;
        unsigned int high1, low1;
        unsigned int high2, low2;
        S64 mhz;

        do{
            t2 = time(0);
        }while(t1 == t2);

        __asm{
            rdtsc
            mov        low1,        eax
            mov        high1,        edx
        }

        //printf("Timing started at %u, %u.\n", high1, low1);

        Sleep(1000);
        //do{
        //    t1 = time(0);
        //}while(t1 == t2);

        __asm{
            rdtsc
            mov        low2,        eax
            mov        high2,        edx
        }

        mhz = ((S64)high2 << 32 | (S64)low2) - ((S64)high1 << 32 | (S64)low1);
        return mhz;
    }

    __declspec(naked) S64 __fastcall timerCpuTicks64()
    {
        // MS: This function is naked because an S64 is returned in EDX:EAX in __fastcall mode
        //     which is the same registers that are set by RDTSC.  So just leave them there and return.
        //     By the way, RDTSC takes 80 cycles to run, because, uh... Intel hates me?  I mean,
        //     come on.  80 cycles seems a bit much to read a register.  That's the equivalent of
        //     a hundred other operations.  And that's the efficient timer.  Oh damn, I just read on
        //     some website that it's only 80 cycles on a P4.  I can't swear enough sometimes.
        //
        //     http://www.uwsg.iu.edu/hypermail/linux/kernel/0109.3/0736.html

        __asm rdtsc
        __asm ret
    }

    S64 timerCpuSpeed64()
    {
        static S64 mhz;

        if (!mhz)
        {
            mhz = getRegistryMhz();
            if (!mhz)
                mhz = calcMhz();
            //printf("Timing initialized to %I64u MHz.\n", mhz / 1000000);
        }
        return mhz;
    }
#else
    S64 __fastcall timerCpuTicks64()
    {
        S64 x = 0;
        static S32 isBadCPU;
        static S64 baseBadCPU;

        if(!isBadCPU){
            static DWORD tls_testtime_index=0;
            S64 *ptesttime;
            STATIC_THREAD_ALLOC(ptesttime);
            
            QueryPerformanceCounter((LARGE_INTEGER *)&x);
            
            if(*ptesttime){
                S64 diff = x - *ptesttime;
                if(diff < 0){
                    baseBadCPU = *ptesttime;
                    isBadCPU = 1;
                }
            }
            *ptesttime = x;
        }

        #ifndef _XBOX
            // NOT an "else", because isBadCPU can change in above block.
            if(isBadCPU){
                static S32 baseTimeMS;
                S32 curTimeMS = timeGetTime();
                
                if(!baseTimeMS){
                    timeBeginPeriod(1);
                    baseTimeMS = curTimeMS;
                }
                
                x = baseBadCPU + (S64)(curTimeMS - baseTimeMS) * timerCpuSpeed64() / 1000;
            }
        #endif
        
        return x;
    }

    S64 timerCpuSpeed64()
    {
        
        S64 x;

        QueryPerformanceFrequency((LARGE_INTEGER *)&x);
        return x;
    }

#endif

unsigned long timerCpuTicks()
{
    S64 x;
    U32 t;

    if (!shiftval)
    {
        timerCpuSpeed();
    }
    x = timerCpuTicks64();
    t = (U32)((x >> shiftval) & 0xffffffff);
     return t;
}

U32 timerCpuSeconds()
{
    return timerCpuTicks() / timerCpuSpeed();
}

F32 timerSeconds64(S64 dt)
{
    return (F32)dt / (F32)timerCpuSpeed64();
}

F32 timerSeconds(U32 dt)
{
    return (F32)dt / (F32)timerCpuSpeed();
}

unsigned long timerCpuSpeed()
{
    static S32 freq = 0;

    if (!freq)
    {
        S64 mhz = timerCpuSpeed64();

        if (!shiftval)
            shiftval = log2i((int)(mhz / 20000)); // U32 timer should go for at least 24 hours without flipping over.
         freq = (S32)(mhz >> shiftval);
    }
    return freq;
}
#else

#include <sys/time.h>
#include <unistd.h>


unsigned long   timerCpuSpeed()
{
    return 1000000;
}

unsigned long   timerCpuTicks()
{
    struct timeval  tv;

    gettimeofday(&tv,0);
    return (tv.tv_sec & 1023) * 1000000 + tv.tv_usec;
}

#endif

typedef struct
{
    S64            start;
    S64            elapsed;
    U8            paused;
    U8            in_use;
    U32            uid_mask;
    const char* fileName;
    int            fileLine;
    int            threadID;
} TimerInfo;

static    TimerInfo timerInfos[128];

#define TIMER_INDEX(x)        ((x) & 0xffff)
#define TIMER_UID(x)        ((x) & 0xffff0000)
#define VALID_TIMER(x)        ((    TIMER_INDEX(x) < ARRAY_SIZE(timerInfos) &&                \
                                        TIMER_UID(x) == timerInfos[TIMER_INDEX(x)].uid_mask &&    \
                                        timerInfos[TIMER_INDEX(x)].in_use) ?                    \
                                            (x = TIMER_INDEX(x)), 1 :(devassert(0),0))


void timerStart(U32 timer)
{
    if(!VALID_TIMER(timer)){
        return;
    }

    timerInfos[timer].elapsed = 0;
    timerInfos[timer].paused = 0;
    timerInfos[timer].start = timerCpuTicks64();
}

void timerAdd(U32 timer,F32 seconds)
{
    if(!VALID_TIMER(timer)){
        return;
    }

    timerInfos[timer].elapsed += (S64)((F32)timerCpuSpeed64() * seconds);
}

void timerPause(U32 timer)
{
    if(!VALID_TIMER(timer)){
        return;
    }
    
    if (timerInfos[timer].paused)
        return;

    timerInfos[timer].elapsed += timerCpuTicks64() - timerInfos[timer].start;
    timerInfos[timer].paused = 1;
}

void timerUnpause(U32 timer)
{
    if(!VALID_TIMER(timer)){
        return;
    }
    
    if (!timerInfos[timer].paused){
        return;
    }
    
    timerInfos[timer].start = timerCpuTicks64();
    timerInfos[timer].paused = 0;
}

F32 timerElapsed(U32 timer)
{
    S64        dt;
    F32        secs;

    if(!VALID_TIMER(timer)){
        return 0;
    }

    dt = timerInfos[timer].elapsed;
    if (!timerInfos[timer].paused)
        dt += timerCpuTicks64() - timerInfos[timer].start;
    secs = (F32)((F32)dt / (F32)timerCpuSpeed64());
    return secs;
}

F32 timerElapsedAndStart(U32 timer)
{
    S64 curTime;
    S64    dt;
    F32    secs;

    if(!VALID_TIMER(timer)){
        return 0;
    }
        
    dt = timerInfos[timer].elapsed;
    if (!timerInfos[timer].paused)
    {
        curTime = timerCpuTicks64();
        dt += curTime - timerInfos[timer].start;
        timerInfos[timer].start = curTime;
    }
    secs = (F32)((F32)dt / (F32)timerCpuSpeed64());
    return secs;
}

U32 timerAllocDbg(const char* fileName, int fileLine)
{
    static    CRITICAL_SECTION cs;
    static    S32 init;
    static    U32 uid;

    int        i;
    
    if(!init)
    {
        init = 1;
        InitializeCriticalSection(&cs);
    }

    EnterCriticalSection(&cs);
    
    for(i=0;i<ARRAY_SIZE(timerInfos);i++)
    {
        if(!timerInfos[i].in_use)
        {
            uid++;
            
            if(!uid || uid >= 0x8000){
                uid = 1;
            }
            
            timerInfos[i].in_use = 1;
            LeaveCriticalSection(&cs);
            timerInfos[i].fileName = fileName;
            timerInfos[i].fileLine = fileLine;
            timerInfos[i].threadID = GetCurrentThreadId();
            timerInfos[i].uid_mask = (U32)uid << 16;
            i |= timerInfos[i].uid_mask;
            timerStart(i);
            return i;
        }
    }

    LeaveCriticalSection(&cs);

    {
        static int doneonce=0;
        if (!doneonce) {
            devassert(!"ran out of timers!");
            doneonce=1;
        }
    }


    return ~0;
}

void timerFree(U32 timer)
{
    if(!VALID_TIMER(timer)){
        return;
    }
    
    timerInfos[timer].in_use = 0;
}

FORCEINLINE static void timerMakeDateString_s_ex(char *datestr, size_t datestr_size, char *format)
{
#if _WIN32 || defined(_XBOX)
    SYSTEMTIME    sys_time;

    GetLocalTime(&sys_time);
    sprintf_s(datestr, datestr_size, format,
        sys_time.wMonth,
        sys_time.wDay,
        sys_time.wYear,
        sys_time.wHour,
        sys_time.wMinute,
        sys_time.wSecond);
#else
    struct timeval tv;
    struct tm timevals;

    gettimeofday(&tv,0);
    timevals = *localtime(&tv.tv_sec);
    sprintf_s(datestr, datestr_size, format,
        timevals.tm_mon+1,
        timevals.tm_mday,
        timevals.tm_year+1900,
        timevals.tm_hour,
        timevals.tm_min
        timevals.tm_sec);
#endif
}

void timerMakeDateString_s(char *datestr, size_t datestr_size)
{
    timerMakeDateString_s_ex(datestr, datestr_size, "%02d-%02d-%04d %02d:%02d:%02d");
}

void timerMakeDateString_fileFriendly_s(char *datestr, size_t datestr_size)
{
    timerMakeDateString_s_ex(datestr, datestr_size, "%02d-%02d-%04d %02d'%02d'%02d");
}

void timerMakeDateNoTimeString_s( char * datestr, size_t datestr_size )
{
#if _WIN32 || defined(_XBOX)
    SYSTEMTIME    sys_time;

    GetLocalTime(&sys_time);
    sprintf_s(datestr, datestr_size, "%02d-%02d-%04d",
        sys_time.wMonth,
        sys_time.wDay,
        sys_time.wYear );
#else
    struct timeval tv;
    struct tm timevals;

    gettimeofday(&tv,0);
    timevals = *localtime(&tv.tv_sec);
    sprintf_s(datestr, datestr_size, "%02d-%02d-%04d",
        timevals.tm_mon+1,
        timevals.tm_mday,
        timevals.tm_year+1900 );
#endif
}

void timerMakeTimeNoDateString_s( char * timestr, size_t timestr_size )
{
#if _WIN32 || defined(_XBOX)
    SYSTEMTIME    sys_time;

    GetLocalTime(&sys_time);
    sprintf_s(timestr, timestr_size, "%02d:%02d:%02d",
        sys_time.wHour,
        sys_time.wMinute,
        sys_time.wSecond );
#else
    struct timeval tv;
    struct tm timevals;

    gettimeofday(&tv,0);
    timevals = *localtime(&tv.tv_sec);
    sprintf_s(timestr, timestr_size, "%02d:%02d:%02d",
        timevals.tm_hour,
        timevals.tm_min,
        timevals.tm_sec );
#endif
}

void timerGetDay(int * day)
{
#if _WIN32 || defined(_XBOX)
    SYSTEMTIME sys_time;

    GetLocalTime(&sys_time);
    *day = sys_time.wDay;
#else
    struct timeval tv;
    struct tm timevals;

    gettimeofday(&tv,0);
    timevals = *localtime(&tv.tv_sec);
    *day = timevals.tm_mday;
#endif
}

S64 y2kOffset()
{
    static S64 offset;

    if (!offset)
    {
        SYSTEMTIME y2k = {0};
        y2k.wYear    = 2000;
        y2k.wMonth    = 1;
        y2k.wDay    = 1;
        SystemTimeToFileTime(&y2k,(FILETIME*)&offset);
    }
    
    return offset;
}

// returns hours:mins:secs
char* timerMakeOffsetStringFromSeconds_s(char *offsetstr, size_t offsetstr_size, U32 seconds)
{
    U32 secs = seconds % 60;
    U32 mins = (seconds /= 60) % 60;
    U32 hours = (seconds /= 60);
    if (hours)
        sprintf_s(offsetstr, offsetstr_size, "%d:%02d:%02d", hours, mins, secs);
    else
        sprintf_s(offsetstr, offsetstr_size, "%d:%02d", mins, secs);
    return offsetstr;
}

char *timerMakeDateStringFromSecondsSince2000_s(char *datestr, size_t datestr_size, U32 seconds)
{
    S64            x;
    SYSTEMTIME    t;
    FILETIME    local;

    assert(datestr_size >= 20);

    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    FileTimeToLocalFileTime((FILETIME*)&x,&local);
    FileTimeToSystemTime(&local, &t);
    //sprintf(datestr,"%04d-%02d-%02d %02d:%02d:%02d",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond); // JE: sprintf is slow, I hates it
    STR_COMBINE_BEGIN_S(datestr, datestr_size)
        STR_COMBINE_CAT_D2((t.wYear / 100));
        STR_COMBINE_CAT_D2((t.wYear % 100));
        STR_COMBINE_CAT_C('-');
        STR_COMBINE_CAT_D2(t.wMonth);
        STR_COMBINE_CAT_C('-');
        STR_COMBINE_CAT_D2(t.wDay);
        STR_COMBINE_CAT_C(' ');
        STR_COMBINE_CAT_D2(t.wHour);
        STR_COMBINE_CAT_C(':');
        STR_COMBINE_CAT_D2(t.wMinute);
        STR_COMBINE_CAT_C(':');
        STR_COMBINE_CAT_D2(t.wSecond);
    STR_COMBINE_END();

    return datestr;
}

//  MM/DD/YYYY HH:MM
// This function is just the inverse of timerMakeDateStringFromSecondsSince2000NoSeconds,
U32 timerParseDateStringToSecondsSince2000NoSeconds(char *dateString)
{
    SYSTEMTIME    t = {0};
    FILETIME    local;
    S64            value = 0;
    U16            rawHour = 0;
    char        aorpm[128];
    U32            returnMe;

    int returnValue = sscanf(dateString ? dateString : "", "%hu/%hu/%hu %hu:%hu%127s", &t.wMonth, &t.wDay, &t.wYear, &rawHour, &t.wMinute, &aorpm, (unsigned int)sizeof(aorpm));

    if (aorpm[0] == 'p' || aorpm[0] == 'P')
        rawHour += 12;
    if (rawHour == 24)
        rawHour = 0;
    t.wHour = rawHour;

    SystemTimeToFileTime(&t, &local);
    LocalFileTimeToFileTime(&local, (FILETIME*)&value);

    value -= y2kOffset();
    value /= 10000000;
    returnMe = (U32)value;

    return value;
}

//  MM/DD/YY HH:MM
char *timerMakeDateStringFromSecondsSince2000NoSeconds(U32 seconds)
{
    static char datestr[256];
    S64            x;
    SYSTEMTIME    t;
    FILETIME    local;
    WORD        hour;
    bool        pm;

    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    FileTimeToLocalFileTime((FILETIME*)&x,&local);
    FileTimeToSystemTime(&local, &t);

    // am-pm adjust
    hour = t.wHour;
    pm = hour >= 12;

    if (hour == 0)
        hour = 24;
    if (hour > 12)
        hour -= 12;

    //sprintf(datestr,"%04d-%02d-%02d %02d:%02d:%02d",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond); // JE: sprintf is slow, I hates it
    STR_COMBINE_BEGIN_S(datestr, 255)

    STR_COMBINE_CAT_D2(t.wMonth);
    STR_COMBINE_CAT_C('/');
    STR_COMBINE_CAT_D2(t.wDay);
    STR_COMBINE_CAT_C('/');
    STR_COMBINE_CAT_D2((t.wYear / 100));
    STR_COMBINE_CAT_D2((t.wYear % 100));
    
    STR_COMBINE_CAT_C(' ');
    STR_COMBINE_CAT_D2(hour);
    STR_COMBINE_CAT_C(':');
    STR_COMBINE_CAT_D2(t.wMinute);

    STR_COMBINE_CAT_C(' ');
    STR_COMBINE_CAT_C(pm?'P':'A');
    STR_COMBINE_CAT_C('M');

    STR_COMBINE_END();

    return datestr;
}

int timerDayFromSecondsSince2000(U32 seconds)
{
    S64            x;
    SYSTEMTIME    t;
    FILETIME    local;
    int            date;

    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    FileTimeToLocalFileTime((FILETIME*)&x,&local);
    FileTimeToSystemTime(&local, &t);

    date = t.wDay;
    date += t.wMonth*100;
    date += t.wYear*10000;
    return date;
}

char *timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(U32 seconds)
{
    static char datestr[256];
    S64            x;
    SYSTEMTIME    t;
    FILETIME    local;
    WORD        hour;
    bool        pm;

    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    FileTimeToLocalFileTime((FILETIME*)&x,&local);
    FileTimeToSystemTime(&local, &t);

    datestr[0] = 0;
    STR_COMBINE_BEGIN(datestr)
    STR_COMBINE_CAT_D2(t.wMonth);
    STR_COMBINE_CAT_C('/');
    STR_COMBINE_CAT_D2(t.wDay);
    STR_COMBINE_CAT_C(' ');

    // am-pm adjust
    hour = t.wHour;
    pm = hour >= 12;

    if (hour == 0)
        hour = 24;
    if (hour > 12)
        hour -= 12;

    STR_COMBINE_CAT_D2(hour);
    STR_COMBINE_CAT_C(':');
    STR_COMBINE_CAT_D2(t.wMinute);
    STR_COMBINE_CAT_C(' ');
    STR_COMBINE_CAT_C(pm?'P':'A');
    STR_COMBINE_CAT_C('M');
    STR_COMBINE_END();

    return datestr;
}

// same as timerMakeDateStringFromSecondsSince2000() but uses format "YYMMDD HH:MM:SS"
char *timerMakeLogDateStringFromSecondsSince2000_s(char *datestr, size_t datestr_size, U32 seconds)
{
    S64            x;
    SYSTEMTIME    t;
    FILETIME    local;

    assert(datestr_size >= 16);

    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    FileTimeToLocalFileTime((FILETIME*)&x,&local);
    FileTimeToSystemTime(&local, &t);

    //sprintf(datestr,"%02d%02d%02d %02d:%02d:%02d", (t.wYear % 100) ,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond);     // JE: sprintf is slow, I hates it
    STR_COMBINE_BEGIN_S(datestr, datestr_size)
        STR_COMBINE_CAT_D2((t.wYear % 100));
        STR_COMBINE_CAT_D2(t.wMonth);
        STR_COMBINE_CAT_D2(t.wDay);
        STR_COMBINE_CAT_C(' ');
        STR_COMBINE_CAT_D2(t.wHour);
        STR_COMBINE_CAT_C(':');
        STR_COMBINE_CAT_D2(t.wMinute);
        STR_COMBINE_CAT_C(':');
        STR_COMBINE_CAT_D2(t.wSecond);
    STR_COMBINE_END();

    return datestr;
}

// writes ISO 8601 Timestamp "YYYY-MM-DDTHH:MM:SS.mmmmmm"
char *timerMakeISO8601TimeStampFromSecondsSince2000_s(char *datestr, size_t datestr_size, U32 seconds)
{
    S64            x;
    SYSTEMTIME    t;
    FILETIME    local;

    assert(datestr_size >= 16);

    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    FileTimeToLocalFileTime((FILETIME*)&x,&local);
    FileTimeToSystemTime(&local, &t);

    //using this instead of sprintf because sprintf is slow
    STR_COMBINE_BEGIN_S(datestr, datestr_size)
        STR_COMBINE_CAT_D((t.wYear));
        STR_COMBINE_CAT_C(('-'));
        STR_COMBINE_CAT_D2(t.wMonth);
        STR_COMBINE_CAT_C(('-'));
        STR_COMBINE_CAT_D2(t.wDay);
        STR_COMBINE_CAT_C('T');
        STR_COMBINE_CAT_D2(t.wHour);
        STR_COMBINE_CAT_C(':');
        STR_COMBINE_CAT_D2(t.wMinute);
        STR_COMBINE_CAT_C(':');
        STR_COMBINE_CAT_D2(t.wSecond);
        STR_COMBINE_CAT_C('.');
        STR_COMBINE_CAT_D(t.wMilliseconds);
    STR_COMBINE_END();

    return datestr;
}

void timerSetSecondsOffset(U32 seconds)
{
    seconds_offset = seconds;
}

void timerAutoSetSecondsOffset()
{
    timerSetSecondsOffset(timerSecondsSince2000() - timerCpuSeconds());
}

U32 timerMakeTimeFromCpuSeconds(U32 seconds)
{
    if (!seconds_offset)
        timerAutoSetSecondsOffset();
    return timerMakeTimeFromSecondsSince2000(seconds + seconds_offset);
}

U32 timerMakeTimeFromSecondsSince2000(U32 seconds)
{
    struct tm time;
    S64            x;
    SYSTEMTIME    t;
    FILETIME    local;

    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    FileTimeToLocalFileTime((FILETIME*)&x,&local);
    FileTimeToSystemTime(&local, &t);

    time.tm_hour = t.wHour;
    time.tm_mday = t.wDay;
    time.tm_min = t.wMinute;
    time.tm_mon = t.wMonth - 1;
    time.tm_sec = t.wSecond;
    time.tm_year = t.wYear - 1900;
    time.tm_isdst = -1;
    return _mktime32(&time);
}

// MAK - I need the day of the week filled out in my time struct. 
//   source: http://www.tondering.dk/claus/cal/node3.html
//   this returns 0 for sunday through 6 for saturday, as struct tm needs
int dayOfWeek(int year, int month, int day)
{
    // note: integer division is actually intended in all the following
    int a = (14 - month) / 12;
    int y = year - a;
    int m = month + 12*a - 2;
    return (day + y + y/4 - y/100 + y/400 + 31*m/12) % 7;
}

int timerDaysInMonth(int month, int year)
{
    static int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month == 1)
        return days[month] + ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
    else 
        return days[month];

}

U32 timerMakeTimeStructFromSecondsSince2000(U32 seconds, struct tm *ptime)
{
    S64            x;
    SYSTEMTIME    t;
    FILETIME    local;

    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    FileTimeToLocalFileTime((FILETIME*)&x, &local);
    FileTimeToSystemTime(&local, &t);

    ptime->tm_hour = t.wHour;
    ptime->tm_mday = t.wDay;
    ptime->tm_min = t.wMinute;
    ptime->tm_mon = t.wMonth - 1;
    ptime->tm_sec = t.wSecond;
    ptime->tm_year = t.wYear - 1900;
    ptime->tm_isdst = -1;
    ptime->tm_wday = dayOfWeek(t.wYear, t.wMonth, t.wDay);

    return _mktime32(ptime);
}

U32 timerMakeTimeStructFromSecondsSince2000NoLocalization(U32 seconds, struct tm *ptime)
{
    S64            x;
    SYSTEMTIME    t;
    
    x = seconds;
    x *= 10000000;
    x += y2kOffset();

    FileTimeToSystemTime((FILETIME*)&x, &t);

    ptime->tm_hour = t.wHour;
    ptime->tm_mday = t.wDay;
    ptime->tm_min = t.wMinute;
    ptime->tm_mon = t.wMonth - 1;
    ptime->tm_sec = t.wSecond;
    ptime->tm_year = t.wYear - 1900;
    ptime->tm_isdst = -1;
    ptime->tm_wday = dayOfWeek(t.wYear, t.wMonth, t.wDay);

    return _mktime32(ptime);
}

U32 timerGetSecondsSince2000FromTimeStruct(struct tm *ptime)
{
    SYSTEMTIME    t = {0};
    FILETIME    local;
    S64            x,y2koffset = y2kOffset();
    U32            seconds;

    t.wYear   = ptime->tm_year + 1900;
    t.wMonth  = ptime->tm_mon + 1;
    t.wDay    = ptime->tm_mday;
    t.wHour   = ptime->tm_hour;
    t.wMinute = ptime->tm_min;
    t.wSecond = ptime->tm_sec;

    SystemTimeToFileTime(&t,&local);
    LocalFileTimeToFileTime(&local,(FILETIME *)&x);

    if (x < y2koffset)
    {
        // If the time is negative, return 0 instead of large positive time
        return 0;
    }

    seconds = (U32)((x  - y2koffset) / 10000000);

    return seconds;
}

U32 timerGetSecondsSince2000FromTimeStructNoLocalization(struct tm *ptime)
{
    SYSTEMTIME    t = {0};
    S64            x,y2koffset = y2kOffset();
    U32            seconds;

    t.wYear   = ptime->tm_year + 1900;
    t.wMonth  = ptime->tm_mon + 1;
    t.wDay    = ptime->tm_mday;
    t.wHour   = ptime->tm_hour;
    t.wMinute = ptime->tm_min;
    t.wSecond = ptime->tm_sec;

    SystemTimeToFileTime( &t, (FILETIME *)&x );

    if (x < y2koffset)
    {
        // If the time is negative, return 0 instead of large positive time
        return 0;
    }

    seconds = (U32)((x  - y2koffset) / 10000000);

    return seconds;
}

/**
* timerGetSecondsSince2000FromSQLTimestamp
*/
U32 timerGetSecondsSince2000FromSQLTimestamp(SQL_TIMESTAMP_STRUCT *psqltime)
{
    struct tm timestamp;

    if (!psqltime->year || !psqltime->month)
        return 0;

    memset(&timestamp, 0, sizeof(struct tm));
    timestamp.tm_year = psqltime->year - 1900;
    timestamp.tm_mon  = psqltime->month - 1;
    timestamp.tm_mday = psqltime->day;
    timestamp.tm_hour = psqltime->hour;
    timestamp.tm_min  = psqltime->minute;
    timestamp.tm_sec  = psqltime->second;

    return timerGetSecondsSince2000FromTimeStruct(&timestamp);
}

/**
* timerGetMonthsSinceZeroFromSQLTimestamp
*/
U32 timerGetMonthsSinceZeroFromSQLTimestamp(SQL_TIMESTAMP_STRUCT *psqltime)
{
    if (!psqltime->year || !psqltime->month)
        return 0;

    return (psqltime->year * 12) + (psqltime->month - 1);
}

/**
* timerMakeSQLTimestampFromSecondsSince2000
*/
U32 timerMakeSQLTimestampFromSecondsSince2000(U32 seconds, SQL_TIMESTAMP_STRUCT *psqltime)
{
    U32 ret;
    struct tm timestamp;

    memset(psqltime, 0, sizeof(SQL_TIMESTAMP_STRUCT));

    if (!seconds)
        return 0;

    ret = timerMakeTimeStructFromSecondsSince2000(seconds, &timestamp);

    psqltime->year = timestamp.tm_year + 1900;
    psqltime->month = timestamp.tm_mon + 1;
    psqltime->day = timestamp.tm_mday;
    psqltime->hour = timestamp.tm_hour;
    psqltime->minute = timestamp.tm_min;
    psqltime->second = timestamp.tm_sec;

    return ret;
}

/**
* timerMakeSQLTimestampFromMonthsSinceZero
*/
void timerMakeSQLTimestampFromMonthsSinceZero(U32 months, SQL_TIMESTAMP_STRUCT *psqltime)
{
    memset(psqltime, 0, sizeof(SQL_TIMESTAMP_STRUCT));

    if (!months)
        return;

    psqltime->year = months / 12;
    psqltime->month = (months % 12) + 1;
    psqltime->day = 1;
    psqltime->hour = 0;
    psqltime->minute = 0;
    psqltime->second = 0;
}

U32 timerClampToHourSS2(U32 seconds, int rounded)
{
    struct tm tms;
    if (rounded)
        seconds += 29;    // totally arbritrary, but should help if we're getting any lag jitter
    timerMakeTimeStructFromSecondsSince2000NoLocalization(seconds, &tms);
    tms.tm_min = tms.tm_sec = tms.tm_isdst = 0;
    return timerGetSecondsSince2000FromTimeStructNoLocalization(&tms);
}

U32 timerGetSecondsSince2000FromString(const char *s)
{
    SYSTEMTIME    t = {0};
    FILETIME    local;
    S64            x, y2koffset = y2kOffset();
    U32            seconds;
    int            len = (int)strlen(s);

    // Canonical string is in this form:
    // "%04d-%02d-%02d %02d:%02d:%02d" ,t->year,t->month,t->day,t->hour,t->minute,t->second

    if(len>2 && s[2]=='-') // I need support for timerMakeDateString_s formatting -GG
    {
        if(len>0)
            t.wMonth    = (WORD)atoi(s+0);
        if(len>3)
            t.wDay        = (WORD)atoi(s+3);
        if(len>6)
            t.wYear        = (WORD)atoi(s+6);
    }
    else if(len > 4 && s[4] == '-')
    {
        if(len>0)
            t.wYear        = (WORD)atoi(s+0);
        if(len>5)
            t.wMonth    = (WORD)atoi(s+5);
        if(len>8)
            t.wDay        = (WORD)atoi(s+8);
    }
    else if(sscanf(s, "%u", &seconds) == 1) // assume it's just a timestamp
    {
        return seconds;
    }

    if(len>11)
        t.wHour        = (WORD)atoi(s+11);
    if(len>14)
        t.wMinute    = (WORD)atoi(s+14);
    if(len>17)
        t.wSecond    = (WORD)atoi(s+17);

    if (t.wYear == 0)
    {
        // Invalid format
        return 0;
    }

    SystemTimeToFileTime(&t,&local);
    LocalFileTimeToFileTime(&local,(FILETIME *)&x);

    if (x < y2koffset)
    {
        // If the time is negative, return 0 instead of large positive time
        return 0;
    }

    seconds = (U32)((x  - y2koffset) / 10000000);
    return seconds;
}


U32 timerSecondsSince2000()
{
    S64            x;
    U32            seconds;

     GetSystemTimeAsFileTime((FILETIME *)&x);
    seconds = (U32)((x  - y2kOffset()) / 10000000 + timing_debug_offset);

    return seconds;
}

U32 timerSecondsSince2000LocalTime()
{
    FILETIME ft_utc;
    S64            x;
    U32            seconds;

     GetSystemTimeAsFileTime(&ft_utc);
    FileTimeToLocalFileTime(&ft_utc,(FILETIME *)&x);

    seconds = (U32)((x  - y2kOffset()) / 10000000 + timing_debug_offset);

    return seconds;
}

S64 timerMsecsSince2000()
{
    S64        x;

     GetSystemTimeAsFileTime((FILETIME *)&x);
    return (x  - y2kOffset()) / 10000 + timing_debug_offset*1000;
}

void setServerS2000(U32 servertime)
{
    //timing_s2000_delta takes the client's timezone into account
    {
        int newdelta = (int)servertime - (int)timerSecondsSince2000LocalTime();
        // we don't adjust by less than a second - this is to reduce jitter on the delta
        if (ABS_UNS_DIFF(newdelta, timing_s2000_delta) > 1)
            timing_s2000_delta = newdelta;
    }

    //(ServerTime is assumed to be GMT, thus timing_s2000_nonLocalDelta should be a tiny number) 
    //timing_s2000_nonLocalDelta doesn't take the client's timezone into account
    {
        int nonLocalDelta = (int)servertime - (int)timerSecondsSince2000();
        // we don't adjust by less than a second - this is to reduce jitter on the delta
        if (ABS_UNS_DIFF(nonLocalDelta, timing_s2000_nonLocalDelta) > 1)
            timing_s2000_nonLocalDelta = nonLocalDelta;
    }
}

U32 timerServerTime(void)
{
    return (U32)(timerSecondsSince2000() + timing_s2000_nonLocalDelta);
}

U32 timerSecondsSince2000WithDelta(void)
{
    return (U32)(timerSecondsSince2000() + timing_s2000_nonLocalDelta);
}

// uses server timing delta to estimate difference between client TZ and server TZ.
int timerServerTZAdjust(void)
{
    int tz, neg = timing_s2000_delta < 0;

    if (neg)
        tz = -1 * ((-timing_s2000_delta + 1800) / 3600);
    else
        tz = ((timing_s2000_delta + 1800) / 3600);
    
    // account for users who just have the wrong day set..
    while (tz < -23)
        tz += 24;
    while (tz > 23)
        tz -= 24;
    return tz;
}

/* Function timerGetTimeString()
 *    Returns a temporary string that indicates the current time.
 *
 *    WARNING!
 *        - Do not save the pointer to the returned string.
 *        - This function is not thread safe.
 */
char* timerGetTimeString(){
    static char buffer[80];
    return timerMakeDateStringFromSecondsSince2000(buffer, timerSecondsSince2000());
}

void timerTest()
{
    int        t = timerAlloc();
    int        i;

    S64* timer_buf = calloc(1000000, sizeof(S64));

    for(i=0;i<1000000;i++)
    {
        timer_buf[i] = timerCpuTicks64();
    }
    printf("%f\n",timerElapsed(t));
    printf("");
    timerFree(t);

    free( timer_buf );
}

// Here's a bunched of timed stdlib functions from stdtypes.h

#undef printf
#undef sprintf
#undef vsprintf
#undef _vscprintf

int printf_timed(const char *format, ...)
{
    int result;
    va_list argptr;
    
    PERFINFO_AUTO_START("printf", 1);
        va_start(argptr, format);
        result = vfprintf(fileGetStdout(), format, argptr);
        va_end(argptr);
        // Make sure the Cider pseudo-console stays up to date, but don't 
        //  bog down on whitespace.
        if (IsUsingCider() && !(format[0] == ' ' && format[1] == '\0'))
            fflush(fileGetStdout());
    PERFINFO_AUTO_STOP();
    
    return result;
}

int sprintf_timed(char *buffer, size_t sizeOfBuffer, const char *format, ...)
{
    int result;
    va_list argptr;
    
    PERFINFO_AUTO_START("sprintf", 1);
        va_start(argptr, format);
        result = vsprintf_s(buffer, sizeOfBuffer, format, argptr);
        va_end(argptr);
    PERFINFO_AUTO_STOP();
    
    return result;
}

int vsprintf_timed(char *buffer, size_t sizeOfBuffer, const char *format, va_list argptr)
{
    int result;
    
    PERFINFO_AUTO_START("vsprintf", 1);
        result = vsprintf_s(buffer, sizeOfBuffer, format, argptr);
    PERFINFO_AUTO_STOP();
    
    return result;
}

int _vscprintf_timed(const char *format, va_list argptr)
{
    int result;
    
    PERFINFO_AUTO_START("_vscprintf", 1);
        result = _vscprintf(format, argptr);
    PERFINFO_AUTO_STOP();
    
    return result;
}

// Make sure to only call this occasionally,
// or in a tight loop where you're not really doing anything else
// PeekMessage is an expensive call.
void FakeYieldToOS(void)
{
    MSG fakeYield_dummyMessage;
    PeekMessage(&fakeYield_dummyMessage, 0, 0, 0, PM_NOREMOVE); // fakes Windows out so it doesn't think we're unresponsive
}

#if defined(_DEBUG) || defined(PROFILE)

// Enter/LeaveCriticalSection timed wrappers.

void timed_EnterCriticalSection(CRITICAL_SECTION* cs, const char* name, WININCLUDE_PERFTYPE** piStatic)
{
    PERFINFO_AUTO_START("EnterCriticalSection", 1);
        PERFINFO_AUTO_START_STATIC(name, piStatic, 1);
            EnterCriticalSection(cs);
        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
}

void timed_LeaveCriticalSection(CRITICAL_SECTION* cs, const char* name, WININCLUDE_PERFTYPE** piStatic)
{
    PERFINFO_AUTO_START("LeaveCriticalSection", 1);
        PERFINFO_AUTO_START_STATIC(name, piStatic, 1);
            LeaveCriticalSection(cs);
        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
}

void timed_Sleep(U32 ms, const char* name, WININCLUDE_PERFTYPE** piStatic)
{
    PERFINFO_AUTO_START_STATIC(name, piStatic, 1);
        Sleep(ms);
    PERFINFO_AUTO_STOP();
}

#endif

void timerRecordStart(char *filename)
{    
    int i;

    //begin hack:  martin claims he has fixed this in the code he hasnt checked in yet
    timerTickBegin();
        PERFINFO_AUTO_START("nothing", 1);
        PERFINFO_AUTO_STOP();
    timerTickEnd();
    //end hack

    timing_state.autoTimerDisablePopupErrors = 1;
    timing_state.autoStackMaxDepth_init = 10;
    if ( filename )
    {
        SAFE_FREE(timing_state.autoTimerRecordFileName);
        timing_state.autoTimerRecordFileName = strdup(filename);
    }
    else
    {
        timing_state.runperfinfo_init = -1;
    }

    //begin hack
    for(i = 0; i < 20; i++){
        timerTickBegin();
        PERFINFO_AUTO_START("nothing", 1);
        PERFINFO_AUTO_STOP();
        timerTickEnd();
    }
    //end hack
    timerTickBegin();
}

void timerRecordEnd(void)
{
    int i;
    //begin hack
    timerTickEnd();
    for(i = 0; i < 20; i++){
        timerTickBegin();
        PERFINFO_AUTO_START("nothing", 1);
        PERFINFO_AUTO_STOP();
        timerTickEnd();
    }
    timerTickBegin();
    PERFINFO_AUTO_START("nothing", 1);
    PERFINFO_AUTO_STOP();
    //end hack
    timing_state.autoTimerProfilerCloseFile = 1;
    timing_state.runperfinfo_init = 0;
    timing_state.resetperfinfo_init = 1;
    timerTickEnd();
}
U32 timerSecondsSince2000Diff(U32 old_timerSecondsSince2000)
{
    return timerSecondsSince2000() - old_timerSecondsSince2000;
}

int timerSecondsSince2000CheckAndSet(U32 *old_timerSecondsSince2000, U32 threshold)
{
    assert(old_timerSecondsSince2000);
    if(timerSecondsSince2000Diff(*old_timerSecondsSince2000) >= threshold)
    {
        *old_timerSecondsSince2000 = timerSecondsSince2000();
        return TRUE;
    }
    return FALSE;
}

#define FILETIME_PER_SECOND (10*1000*1000) // 100ns per tick

void timeProcessTimes(double *upsecs, double *kernelsecs, double *usersecs)
{
    FILETIME start, end, now, kernel, user, zero = {0};
    U64 start_100ns, now_100ns, kernel_100ns, user_100ns;

    if(GetProcessTimes(GetCurrentProcess(), &start, &end, &kernel, &user))
        GetSystemTimeAsFileTime(&now);
    else
        start = end = now = kernel = user = zero;

    // theoretically, simply casting a FILETIME to U64 can cause an alignment fault
    start_100ns    = ((U64)start.dwHighDateTime    << 32) | (U64)start.dwLowDateTime;
    now_100ns    = ((U64)now.dwHighDateTime        << 32) | (U64)now.dwLowDateTime;
    kernel_100ns= ((U64)kernel.dwHighDateTime    << 32) | (U64)kernel.dwLowDateTime;
    user_100ns    = ((U64)user.dwHighDateTime        << 32) | (U64)user.dwLowDateTime;

    if(upsecs)
        *upsecs = (double)(now_100ns - start_100ns) / FILETIME_PER_SECOND;
    if(kernelsecs)
        *kernelsecs = (double)kernel_100ns / FILETIME_PER_SECOND;
    if(user_100ns)
        *usersecs = (double)user_100ns / FILETIME_PER_SECOND;
}
