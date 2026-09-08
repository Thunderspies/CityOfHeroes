/*****************************************************************************
    created:    2001/08/07
    copyright:  2001, NCSoft. All Rights Reserved
    author(s):  Peter M. Freese
    
    purpose:    
*****************************************************************************/

#include "../../include/arda2/core/corFirst.h"
#include "../../include/arda2/timer/timTimer.h"
#include <time.h>
#if defined(__GNUC__) && defined(_WIN32) && defined(__i386__) && !defined(_XBOX)
#include <intrin.h>
#endif

#if CORE_SYSTEM_PS3
#include <sys/timer.h>
#include <sys/time_util.h>
#include <sys/sys_time.h>
#endif

bool timTimer::s_initialized = false;
uint64 timTimer::s_systemFrequency = 0;

#if CORE_SYSTEM_WIN32
//#define USE_CPU_COUNTER 1    // MS recommends using QueryPerformanceCounter over rdtsc...
#endif

timTimer::timTimer( uint frequency ) :
    m_remainder(0),
    m_start(0),
    m_stop(0),
    m_running(0),
    m_divisor(0)
{
    if ( !s_initialized )
        InitializeSystemTimer();

    m_divisor = s_systemFrequency / frequency;

    // because the divisor above may have integer truncation, we need to store the
    // division remainder for calculating the error term
    m_remainder = s_systemFrequency - m_divisor * frequency;

    Start();
}


void timTimer::InitializeSystemTimer()
{
#if CORE_SYSTEM_WINAPI

#if USE_CPU_COUNTER

    uint64 qpcFreq;
    QueryPerformanceFrequency((LARGE_INTEGER *)&qpcFreq);

    uint64 qpcCounter0;
    QueryPerformanceCounter((LARGE_INTEGER *)&qpcCounter0);
    uint64 cpuCounter0 = GetSystemTimerValue();

    uint64 qpcCounterEnd = qpcCounter0 + qpcFreq / 16;   // 1/16 sec

    uint64 qpcCounter1;
    uint64 cpuCounter1;
    do 
    {
        QueryPerformanceCounter((LARGE_INTEGER *)&qpcCounter1);
        cpuCounter1 = GetSystemTimerValue();

    } while ( qpcCounter1 < qpcCounterEnd );

    uint64 cpuCycles = cpuCounter1 - cpuCounter0;
    uint64 qpcInterval = qpcCounter1 - qpcCounter0;

    s_systemFrequency = cpuCycles * qpcFreq / qpcInterval;

#else
    QueryPerformanceFrequency((LARGE_INTEGER *)&s_systemFrequency);
#endif

#elif CORE_SYSTEM_PS3
    s_systemFrequency = sys_time_get_timebase_frequency();
#else
    s_systemFrequency = 1000000;
#endif
    s_initialized = true;
}


uint64 timTimer::GetSystemTimerValue()
{
#if CORE_SYSTEM_WINAPI

    uint64 counter;

#if USE_CPU_COUNTER
#if defined(__GNUC__) && defined(_WIN32) && defined(__i386__) && !defined(_XBOX)
    counter = __rdtsc();
#else
    __asm
    {
        rdtsc
        mov [dword ptr counter], eax
        mov [dword ptr counter+4], edx
    }
#endif
#else
    QueryPerformanceCounter((LARGE_INTEGER *)&counter);
#endif

    return counter;

#elif CORE_SYSTEM_PS3
    
    uint64 counter;
    SYS_TIMEBASE_GET(counter);
    return counter;

#else
    // this is in milliseconds on linux
    struct timeval now;
    gettimeofday(&now, NULL);
    return (uint64)now.tv_sec*1000000 + (uint64)now.tv_usec;
#endif
}


void timTimer::Start()
{
    m_start = GetSystemTimerValue();
    m_running = true;
}


void timTimer::Stop()
{
    m_stop = GetSystemTimerValue();
    m_running = false;
}

/*
**  Reset
**
** reset the internal start position of this timer instance.
**
*/
void timTimer::Reset()
{
    Start();
}

/*
**  GetElapsed
**
** Get the time since this timer was reset or started.
**
*/
uint timTimer::GetElapsed() const
{
    int64 dTime;
    if ( m_running )
        dTime = GetSystemTimerValue() - m_start;
    else
        dTime = m_stop - m_start;

    dTime /= m_divisor;

    // error term adjustment since we're working with integer math
    dTime -= dTime * m_remainder / s_systemFrequency;

    return static_cast<uint>(dTime);
}

/**
**  GetFloatSeconds
**
** Get the value of this instance's start time in float seconds.
**
** @WARNING: Using floats for time stamps can lead to subtle problems
**          when calculating intervals between them. Subtracting two
**          large floats can result in a large absolute error due to
**          floating point granularity.
**
*/
float timTimer::GetFloatSeconds()
{
    uint64 now = m_start;
    int64 intSeconds = now / s_systemFrequency;
    int intRemainder = (int)(now - (intSeconds*s_systemFrequency));
    float remainder = (float)intRemainder / (float)s_systemFrequency;
    return intSeconds + remainder;
}


/**
**  GetFloatSecondsNow
**
** Get the value of NOW in float seconds.
**
** @WARNING: Using floats for time stamps can lead to subtle problems
**          when calculating intervals between them. Subtracting two
**          large floats can result is a large absolute error due to
**          floating point granularity.
**
*/
float timTimer::GetFloatSecondsNow()
{
    if ( !s_initialized )
        InitializeSystemTimer();

    uint64 now = GetSystemTimerValue();

    int64 intSeconds = now / s_systemFrequency;
    int intRemainder = (int)(now - (intSeconds*s_systemFrequency));
    float remainder = (float)intRemainder / (float)s_systemFrequency;
    return intSeconds + remainder;
}


void corSleep(int milliseconds)
{
#if CORE_SYSTEM_WINAPI
    Sleep(milliseconds);
#elif CORE_SYSTEM_PS3
    sys_timer_usleep(1000*milliseconds); // in u-seconds
#else
    struct timespec t;
    struct timespec rem;
    t.tv_sec = milliseconds/1000;
    t.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&t, &rem);
#endif
}



/**
 * Call this if the application relies on the system timer.
 * This sets the process's affinity to only use one CPU. On Multi CPU and
 * multi core systems, the QueryPreformanceCounter can produce garbage
 * results when both CPUs or cores are utilized.
 *
 * @return the old process affinity mask that was replaced, or zero if it fails
 */
uint32 timUseOneCPU()
{
#if CORE_SYSTEM_WINAPI && !(CORE_SYSTEM_XENON)
    HANDLE handle =::GetCurrentProcess();
    DWORD_PTR processMask = 0x0000;
    DWORD_PTR systemMask = 0x0000;
    int result = ::GetProcessAffinityMask(handle, &processMask, &systemMask);
    if (result)
    {
        DWORD_PTR newProcessMask = 0x0001;
        if (::SetProcessAffinityMask(handle, newProcessMask))
            return (uint32)processMask;
    }
#endif
    return 0;
}
