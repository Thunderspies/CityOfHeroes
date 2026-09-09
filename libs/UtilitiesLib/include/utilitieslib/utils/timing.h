#ifndef _TIMING_H
#define _TIMING_H

#include "../stdtypes.h"
#include "error.h"

#define SECONDS_PER_MINUTE    60
#define MINUTES_PER_HOUR    60
#define HOURS_PER_DAY        24

#define SECONDS_PER_HOUR    (SECONDS_PER_MINUTE*MINUTES_PER_HOUR)
#define SECONDS_PER_DAY        (SECONDS_PER_HOUR*HOURS_PER_DAY)

#ifdef _XBOX
    #include <ppcintrinsics.h>
#endif

C_DECLARATIONS_BEGIN

#ifdef _WIN64
    unsigned __int64 __rdtsc(void);  // from intrin.h, but intrin.h doesn't compile with our string defines
#endif

typedef struct PerformanceInfo    PerformanceInfo;

typedef enum PerformanceInfoType {
    PERFINFO_TYPE_CPU    = 0,
    PERFINFO_TYPE_BITS    = 1,
    PERFINFO_TYPE_MISC    = 2,
} PerformanceInfoType;
 
typedef struct PerformanceInfoHistory {
    S64                            cycles;
    U32                            count;    
} PerformanceInfoHistory;

typedef struct PerformanceInfo { 
    const char*                    locName;
    
    PerformanceInfo**            rootStatic;        // The static root of this timer for all branches.
    PerformanceInfo*            nextMaster;        // The next timer in the master list.
    PerformanceInfo*            parent;            // My parent group.

    PerformanceInfo*            nextSibling;    // My next sibling.
    
    struct {
        PerformanceInfo*        head;            // My first child.
        PerformanceInfo*        tail;            // My last child.
    } child;
    
    struct {
        PerformanceInfo*        hi;                // Branches that have higher parent pointers.
        PerformanceInfo*        lo;                // Branches that have lower parent pointers.
    } branch;
    
    S64                            opCountInt;
    S64                            totalTime;

    S64                            startTime;
    
    S64                            maxTime;
    PerformanceInfoHistory        history[200];
    S32                            historyPos;
    
    U32                            uid;

    const char*                    functionName;

    PerformanceInfoType            infoType    : 3;
    U32                            started     : 1;
    U32                            open        : 1;
    U32                            hidden        : 1;
    U32                            breakMe        : 1;
    U32                            inited        : 1;
} PerformanceInfo;

#if defined(_XBOX)
    #define GET_CPU_TICKS_64(x) x = __mftb();
#elif defined (_WIN64) || (defined(__GNUC__) && defined(_WIN32) && defined(__i386__) && !defined(_XBOX))
    #define GET_CPU_TICKS_64(x) x = __rdtsc();
#else
    #define GET_CPU_TICKS_64(x){                                        \
        {__asm    rdtsc                        }                            \
        {__asm    mov        dword ptr[x],    eax    }                            \
        {__asm    mov        dword ptr[x]+4,    edx    }                            \
    }
#endif

#ifdef FINAL
#define DISABLE_PERFORMANCE_COUNTERS 1
#else
#define DISABLE_PERFORMANCE_COUNTERS 0
#endif

#if DISABLE_PERFORMANCE_COUNTERS
    #define PERFINFO_RUN_CONDITIONS (0)
    #define PERFINFO_RUN(x)
#else
    #define PERFINFO_RUN_CONDITIONS (timing_state.runperfinfo && PERFINFO_IS_GOOD_THREAD)
#define PERFINFO_RUN(x) { \
	if (!(PERFINFO_RUN_CONDITIONS)) { \
		COH_NOP(); \
	} else { \
		x \
	} \
}
#endif

//- TIMER CORE CRAP -------------------------------------------------------------------------------

// Change this to 1 to enable checking if if AUTO_START and AUTO_STOP were called
// from inside the same function.  This is just a safety check to catch orphaned STARTs
// or STOPs.

#if defined(FULLDEBUG) || 0
    #define PERFINFO_AUTO_START_STORE_FUNCTION                                \
        timing_state.autoStackTop->functionName = __FUNCTION__
    
    #define PERFINFO_AUTO_STOP_CHECK_FUNCTION {                                \
        const char* functionName = __FUNCTION__;                            \
        if(strcmp(timing_state.autoStackTop->functionName, functionName)){    \
            timerPerfInfoAssert(__FILE__, __LINE__,                            \
                                timing_state.autoStackTop,                    \
                                functionName);                                \
        }                                                                    \
    }
#else
    #define PERFINFO_AUTO_START_STORE_FUNCTION
    #define PERFINFO_AUTO_STOP_CHECK_FUNCTION
#endif

#define PERFINFO_IS_GOOD_THREAD (timing_state.autoTimerNoThreadCheck || timing_state.autoTimerThreadID == timerGetCurrentThread())

#define PERFINFO_AUTO_START_STATIC(locNameStatic,rootStatic,inc){                        \
    PERFINFO_RUN(                                                                        \
        if(timing_state.autoStackDepth < timing_state.autoStackMaxDepth){                \
            S64 startTime;                                                                \
            timerAutoTimerAlloc(rootStatic, locNameStatic);                                \
            PERFINFO_AUTO_START_STORE_FUNCTION;                                            \
            timing_state.autoStackTop->opCountInt += inc;                                \
            GET_CPU_TICKS_64(startTime);                                                \
            timing_state.autoStackTop->startTime = startTime;                            \
        }                                                                                \
        timing_state.autoStackDepth++;                                                    \
    )                                                                                    \
}

#define PERFINFO_AUTO_START(locNameParam,inc){                                            \
    static PerformanceInfo* rootStatic = NULL;                                            \
    static const char* locNameStatic = locNameParam;                                    \
    PERFINFO_AUTO_START_STATIC(locNameStatic, &rootStatic, inc);                        \
}

#define PERFINFO_ADD_FAKE_CPU(amount){                                                    \
    PERFINFO_RUN(                                                                        \
        if(timing_state.autoStackDepth < timing_state.autoStackMaxDepth){                \
            if(timing_state.autoStackTop){                                                \
                timing_state.autoStackTop->                                                \
                    history[timing_state.autoStackTop->historyPos].cycles += amount;    \
                timing_state.autoStackTop->                                                \
                    history[timing_state.autoStackTop->historyPos].count++;                \
            }                                                                            \
        }                                                                                \
    )                                                                                    \
}

#define PERFINFO_AUTO_STOP_MAIN {                                            \
    S64 addTime;                                                            \
    GET_CPU_TICKS_64(addTime);                                                \
    addTime = addTime - timing_state.autoStackTop->startTime;                \
    if(addTime > timing_state.autoStackTop->maxTime)                        \
        timing_state.autoStackTop->maxTime = addTime;                        \
    timing_state.autoStackTop->totalTime += addTime;                        \
    timing_state.autoStackTop->history[                                        \
        timing_state.autoStackTop->historyPos].cycles += addTime;            \
    timing_state.autoStackTop->history[                                        \
        timing_state.autoStackTop->historyPos].count++;                        \
    timing_state.autoStackTop = timing_state.autoStackTop->parent;            \
}


#define PERFINFO_AUTO_STOP(){                                                            \
    PERFINFO_RUN(                                                                        \
        if(timing_state.autoStackDepth > 0){                                            \
            timing_state.autoStackDepth--;                                                \
            if(timing_state.autoStackDepth < timing_state.autoStackMaxDepth){            \
                PERFINFO_AUTO_STOP_CHECK_FUNCTION                                        \
                PERFINFO_AUTO_STOP_MAIN                                                    \
            }                                                                            \
        }                                                                                \
        else if(!timing_state.assert_shown){                                            \
            timerStackUnderFlow();                                                        \
        }                                                                                \
    )                                                                                    \
}

#define PERFINFO_AUTO_STOP_CHECKED(locNameParam){                                        \
    const char* locName = locNameParam;                                                    \
    PERFINFO_RUN(                                                                        \
        if(timing_state.autoStackDepth > 0){                                            \
            timing_state.autoStackDepth--;                                                \
            if(timing_state.autoStackDepth < timing_state.autoStackMaxDepth){            \
                if(locName){                                                            \
                    timerCheckLocName(locName);                                            \
                }                                                                        \
                PERFINFO_AUTO_STOP_CHECK_FUNCTION                                        \
                PERFINFO_AUTO_STOP_MAIN                                                    \
            }                                                                            \
        }                                                                                \
        else if(!timing_state.assert_shown){                                            \
            timerStackUnderFlow();                                                        \
        }                                                                                \
    )                                                                                    \
}

//- MISC -------------------------------------------------------------------------------

#define START_MISC_COUNT_STATIC(startVal, rootStatic, locNameStatic)                    \
    PERFINFO_RUN(                                                                        \
        if(timing_state.autoStackDepth < timing_state.autoStackMaxDepth){                \
            timerAutoTimerAllocMisc(rootStatic, locNameStatic);                            \
            PERFINFO_AUTO_START_STORE_FUNCTION;                                            \
            timing_state.autoStackTop->startTime = startVal;                            \
            timing_state.autoStackTop->opCountInt++;                                    \
        }                                                                                \
        timing_state.autoStackDepth++;                                                    \
    );
    
#define START_MISC_COUNT(startVal, locNameParam){                                        \
    static PerformanceInfo* rootStatic = NULL;                                            \
    static const char* locNameStatic = locNameParam;                                    \
    START_MISC_COUNT_STATIC(startVal, &rootStatic, locNameStatic);                        \
}

#define STOP_MISC_COUNT(stopVal)                                                        \
    PERFINFO_RUN(                                                                        \
        if(timing_state.autoStackDepth > 0){                                            \
            timing_state.autoStackDepth--;                                                \
            if(timing_state.autoStackDepth < timing_state.autoStackMaxDepth){            \
                S64 addCPU = stopVal - timing_state.autoStackTop->startTime;            \
                PERFINFO_ADD_FAKE_CPU(addCPU);                                            \
                timing_state.autoStackTop->totalTime += addCPU;                            \
                timing_state.autoStackTop = timing_state.autoStackTop->parent;            \
            }                                                                            \
        }                                                                                \
    );

#define ADD_MISC_COUNT(val, locNameParam)    \
    START_MISC_COUNT(0, locNameParam);        \
    STOP_MISC_COUNT(val)

//- BITS -------------------------------------------------------------------------------

#define START_BIT_COUNT_STATIC(pak, rootStatic, locNameStatic)                                                \
    PERFINFO_RUN(                                                                                            \
        if(timing_state.autoStackDepth < timing_state.autoStackMaxDepth){                                    \
            timerAutoTimerAllocBits(rootStatic, locNameStatic);                                                \
            PERFINFO_AUTO_START_STORE_FUNCTION;                                                                \
            timing_state.autoStackTop->startTime = pak->stream.cursor.byte * 8 + pak->stream.cursor.bit;    \
            timing_state.autoStackTop->opCountInt++;                                                        \
        }                                                                                                    \
        timing_state.autoStackDepth++;                                                                        \
    );

#define START_BIT_COUNT(pak, locNameParam){                                                                    \
    static PerformanceInfo* rootStatic = NULL;                                                                \
    static const char* locNameStatic = locNameParam;                                                        \
    START_BIT_COUNT_STATIC(pak, &rootStatic, locNameStatic);                                                \
}

#define STOP_BIT_COUNT(pak)                                                                                    \
    PERFINFO_RUN(                                                                                            \
        if(timing_state.autoStackDepth > 0){                                                                \
            timing_state.autoStackDepth--;                                                                    \
            if(timing_state.autoStackDepth < timing_state.autoStackMaxDepth){                                \
                S64 addCPU = 1000000 * (pak->stream.cursor.byte * 8 + pak->stream.cursor.bit -                \
                                        timing_state.autoStackTop->startTime) / 8;                            \
                PERFINFO_ADD_FAKE_CPU(addCPU);                                                                \
                timing_state.autoStackTop->totalTime += addCPU;                                                \
                timing_state.autoStackTop = timing_state.autoStackTop->parent;                                \
            }                                                                                                \
        }                                                                                                    \
    );

#define PERFINFO_AUTO_STOP_START(locName,inc) PERFINFO_AUTO_STOP();PERFINFO_AUTO_START(locName,inc)

typedef struct TimingState {
    S64                    totalTimeStart;
    S64                    totalTime;
    
    S64                    totalCPUTimePassed[10];
    S64                    totalCPUTimeUsed[10];
    S64                    totalCPUTimePos;

    S64                    tickHistory[30];

    int                    runperfinfo;
    int                    runperfinfo_init;

    int                    resetperfinfo_init;
    int                    send_reset;
    
    U32                    autoTimersLayoutID;
    U32                    autoTimersLastResetLayoutID;

    struct {
        PerformanceInfo*    head;
        PerformanceInfo*    tail;
    } autoTimerMasterList;
    
    PerformanceInfo*    autoTimerRootList;
    U32                    autoTimerCount;
    int                    autoStackDepth;
    int                    autoStackMaxDepth;
    int                    autoStackMaxDepth_init;
    PerformanceInfo*    autoStackTop;
    int                    autoTimerThreadID;
    int                    autoTimerNoThreadCheck;
    int                    autoTimerDisablePopupErrors;

    int                    autoTimerProfilerCloseFile;

    char*                autoTimerRecordFileName;
    U32                    autoTimerRecordLayoutID;
    
    char*                autoTimerPlaybackFileName;
    int                    autoTimerPlaybackStepSize;
    int                    autoTimerPlaybackState;
    U64                    autoTimerPlaybackCPUSpeed;
    
    int                    assert_shown;
} TimingState;

extern TimingState    timing_state;
extern int timing_debug_offset;        // offset added to timerGetTime()

// Use this instead of __declspec(thread)
#define STATIC_THREAD_ALLOC(var)            \
    static DWORD tls_##var##_index = 0;        \
    int dummy_##var = (((!tls_##var##_index?(tls_##var##_index=TlsAlloc()):0)),    \
        ((var = TlsGetValue(tls_##var##_index))?0:(TlsSetValue(tls_##var##_index, var = tls_zero_malloc(sizeof(*var))))))



// Generated by mkproto
unsigned long timerCpuSpeed(void);
unsigned long timerCpuTicks(void);
U32 timerCpuSeconds(void);

S64 getRegistryMhz(void);
void timerPerfInfoAssert(const char* file, int line, PerformanceInfo* curTimer, const char* functionName);
void __fastcall timerAutoTimerAlloc(PerformanceInfo** root, const char* locName);
void __fastcall timerAutoTimerAllocBits(PerformanceInfo** root, const char* locName);
void __fastcall timerAutoTimerAllocMisc(PerformanceInfo** root, const char* locName);
void __fastcall timerCheckLocName(const char* stopLocName);
void __fastcall timerStackUnderFlow(void);
int __fastcall timerGetCurrentThread(void);
void timerSetBreakPoint(U32 layoutID, U32 uid, S32 set);
void timerFlushFileBuffers(void);
void timerTickBegin(void);
void timerTickEnd(void);

#if !USE_NEW_TIMING_CODE
    static INLINEDBG void autoTimerInitThreads(void){}
    static INLINEDBG void autoTimerQueueTimingStart(void){}
    static INLINEDBG void autoTimerSetDepth(int x){}
    static INLINEDBG void autoTimerTickBegin(void) {timerTickBegin();}
    static INLINEDBG void autoTimerTickEnd(void) {timerTickEnd();}
#endif

S64 __fastcall timerCpuTicks64(void);
S64 timerCpuSpeed64(void);
F32 timerSeconds64(S64 dt);

void timeProcessTimes(double *upsecs, double *kernelsecs, double *usersecs);

void timerStart(U32 timer);
void timerAdd(U32 timer,F32 seconds);
void timerPause(U32 timer);
void timerUnpause(U32 timer);
F32 timerSeconds(U32 dt);
F32 timerElapsed(U32 timer);
F32 timerElapsedAndStart(U32 timer);
U32 timerAllocDbg(const char* fileName, int fileLine);
#define timerAlloc() timerAllocDbg(__FILE__, __LINE__)
void timerFree(U32 timer);
#define timerMakeDateString(datestr) timerMakeDateString_s(SAFESTR(datestr))
void timerMakeDateString_s(char *datestr, size_t datestr_size);
#define timerMakeDateNoTimeString(datestr) timerMakeDateNoTimeString_s(SAFESTR(datestr))
void timerMakeDateNoTimeString_s(char * datestr, size_t datestr_size);
#define timerMakeTimeNoDateString(timestr) timerMakeTimeNoDateString_s(SAFESTR(timestr))
void timerMakeTimeNoDateString_s(char * timestr, size_t timestr_size);
#define timerMakeDateString_fileFriendly(datestr) timerMakeDateString_fileFriendly_s(SAFESTR(datestr))
void timerMakeDateString_fileFriendly_s(char *datestr, size_t datestr_size);

void timerGetDay(int * day);
#define timerMakeDateStringFromSecondsSince2000(datestr, seconds) timerMakeDateStringFromSecondsSince2000_s(SAFESTR(datestr), seconds)
char *timerMakeDateStringFromSecondsSince2000_s(char *datestr, size_t datestr_size, U32 seconds);
#define timerMakeLogDateStringFromSecondsSince2000(datestr, seconds) timerMakeLogDateStringFromSecondsSince2000_s(SAFESTR(datestr), seconds)
char *timerMakeLogDateStringFromSecondsSince2000_s(char *datestr, size_t datestr_size, U32 seconds); // shows "YYMMDD HH:MM:SS" (for db log)
#define timerMakeISO8601TimeStampFromSecondsSince2000(datestr, seconds) timerMakeISO8601TimeStampFromSecondsSince2000_s(SAFESTR(datestr), seconds)
char *timerMakeISO8601TimeStampFromSecondsSince2000_s(char *datestr, size_t datestr_size, U32 seconds); // writes ISO 8601 Timestamp "YYYY-MM-DDTHH:MM:SS.mmmmmm"
static INLINEDBG char *timerMakeDateStringFromSecondsSince2000Static(U32 seconds) {THREADSAFE_STATIC char tmp_date_str[128]; return timerMakeLogDateStringFromSecondsSince2000(tmp_date_str,seconds);}
int timerDayFromSecondsSince2000(U32 seconds);
__int64 y2kOffset(void);
#define timerMakeOffsetStringFromSeconds(offsetstr, seconds) timerMakeOffsetStringFromSeconds_s(SAFESTR(offsetstr), seconds)
char* timerMakeOffsetStringFromSeconds_s(char *offsetstr, size_t offsetstr_size, U32 seconds); // shows hours:mins:secs
char* timerMakeOffsetDateStringNoYearNoSecFromSecondsSince2000(U32 seconds); 
U32 timerParseDateStringToSecondsSince2000NoSeconds(char *datestr);
char *timerMakeDateStringFromSecondsSince2000NoSeconds(U32 seconds);
char *timerMakeOffsetDateString(U32 seconds);
U32 timerMakeTimeFromSecondsSince2000(U32 seconds);
int timerDaysInMonth(int month, int year);

U32 timerMakeTimeStructFromSecondsSince2000(U32 seconds, struct tm *ptime);
U32 timerMakeTimeStructFromSecondsSince2000NoLocalization(U32 seconds, struct tm *ptime);

U32 timerGetSecondsSince2000FromTimeStruct(struct tm *ptime);
U32 timerGetSecondsSince2000FromTimeStructNoLocalization(struct tm *ptime);

typedef struct tagTIMESTAMP_STRUCT SQL_TIMESTAMP_STRUCT;
U32 timerGetSecondsSince2000FromSQLTimestamp(SQL_TIMESTAMP_STRUCT *psqltime);
U32 timerGetMonthsSinceZeroFromSQLTimestamp(SQL_TIMESTAMP_STRUCT *psqltime);
U32 timerMakeSQLTimestampFromSecondsSince2000(U32 seconds, SQL_TIMESTAMP_STRUCT *psqltime);
void timerMakeSQLTimestampFromMonthsSinceZero(U32 months, SQL_TIMESTAMP_STRUCT *psqltime);

U32 timerGetSecondsSince2000FromString(const char *s);

U32 timerSecondsSince2000(void);
U32 timerSecondsSince2000LocalTime();
char* timerGetTimeString(void);
__int64 timerMsecsSince2000(void);
U32 timerClampToHourSS2(U32 seconds, int rounded);
U32 timerSecondsSince2000Diff(U32 old_timerSecondsSince2000);
int timerSecondsSince2000CheckAndSet(U32 *old_timerSecondsSince2000, U32 threshold); // check and set

// Set offsets to use in timerMakeTimeFromCpuSeconds if the cpuSeconds value is not from this computer
void timerSetSecondsOffset(U32 seconds);
void timerAutoSetSecondsOffset(void);
U32 timerMakeTimeFromCpuSeconds(U32 seconds);

// client uses these functions to loosely syncronize with server for task timing
extern int timing_s2000_delta;
void setServerS2000(U32 servertime);
U32 timerServerTime(void); // uses server delta & local time
U32 timerSecondsSince2000WithDelta(void);   // old clunky name 
#define serverTimeFromLocal(time) ((time)+timing_s2000_delta)
#define localTimeFromServer(time) ((time)-timing_s2000_delta)
int timerServerTZAdjust(void); // get an hour offset for timezone adjustment (-11..12)

void timerRecordStart(char *filename);
void timerRecordEnd(void);

void FakeYieldToOS(void);

#if _XBOX
#define timeGetTime() GetTickCount()
#endif

C_DECLARATIONS_END

// End mkproto
#endif
