#include "utilitieslib/utils/mutex.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/sysutil.h"
#ifndef _XBOX
    #include <tlhelp32.h>
#endif
#include "utilitieslib/components/earray.h"

static HANDLE threadAgnosticMutex_handle=NULL;
static unsigned threadAgnosticMutex_threadID;
typedef struct MutexEventPair {
    HANDLE hMutex;
    HANDLE hEvent;
    const char *debug_name; // Can not reference, pointer may go bad
} MutexEventPair;
static MutexEventPair **threadAgnosticMutex_actions=NULL; // Only modified/accessed in-thread

static unsigned __stdcall threadAgnosticMutexThread( LPVOID lpParam )
{
    EXCEPTION_HANDLER_BEGIN
        static volatile LONG count_of_threads;
        int result;
        result = InterlockedIncrement(&count_of_threads);
        assert(result==1); // Two of these threads started!
        PERFINFO_AUTO_START("threadAgnosticMutexThread", 1);
            SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_ABOVE_NORMAL);
            for(;;)
            {
                int count;
                if (count=eaSize(&threadAgnosticMutex_actions)) {
                    // Wait for these objects or a request
                    static HANDLE *handles=NULL;
                    static int handles_max=0;
                    int i;
                    DWORD ret;
                    bool bAbandoned=false;
                    dynArrayFit((void**)&handles, sizeof(HANDLE), &handles_max, count);
                    for (i=0; i<count; i++) 
                        handles[i] = threadAgnosticMutex_actions[i]->hMutex;
                    ret = WaitForMultipleObjectsEx(count, handles, FALSE, INFINITE, TRUE);
                    assert(ret != WAIT_FAILED);
                    if (ret != WAIT_IO_COMPLETION) {
                        assert(eaSize(&threadAgnosticMutex_actions)==count); // Otherwise we processed an APC while waiting!
                    }
                    if (ret >= WAIT_ABANDONED_0 && ret < WAIT_ABANDONED_0 + count) {
                        bAbandoned = true;
                        ret = ret - WAIT_ABANDONED_0 + WAIT_OBJECT_0;
                    }
                    if (ret >= WAIT_OBJECT_0 && ret < WAIT_OBJECT_0 + count) {
                        MutexEventPair *pair;
                        i = ret - WAIT_OBJECT_0;
                        // Finished waiting on index i
                        pair = eaRemove(&threadAgnosticMutex_actions, i);
                        // Signal waiting thread
                        SetEvent(pair->hEvent); // Cannot reference pair after this point
                    }
                } else {
                    // Wait for a request
                    SleepEx(INFINITE, TRUE);
                }
                //memlog_printf(NULL, "threadAgnosticMutexThread():Returned from waiting");
            }
        PERFINFO_AUTO_STOP();
        return 0; 
    EXCEPTION_HANDLER_END
    return 0;
} 

static VOID CALLBACK threadAgnosticMutexAcquireFunc( ULONG_PTR dwParam)
{
    MutexEventPair *pair = (MutexEventPair*)dwParam;
    //memlog_printf(NULL, "threadAgnosticMutexAcquireFunc(%08x):WaitForSingleObject", pair->hMutex);
    eaPush(&threadAgnosticMutex_actions, pair);
}

static VOID CALLBACK threadAgnosticMutexReleaseFunc( ULONG_PTR dwParam)
{
    MutexEventPair *pair = (MutexEventPair*)dwParam;
    BOOL result;
    DWORD ret;
    //memlog_printf(NULL, "threadAgnosticMutexReleaseFunc(%08x):Release", pair->hMutex);
    result = ReleaseMutex(pair->hMutex);
    assert(result);
    //memlog_printf(NULL, "threadAgnosticMutexReleaseFunc(%08x):SetEvent", pair->hMutex);
    ret = SetEvent(pair->hEvent);
    //memlog_printf(NULL, "threadAgnosticMutexReleaseFunc(%08x):Done, returned %d", pair->hMutex, ret);
}

DWORD WaitForEventInfiniteSafe(HANDLE hEvent)
{
    DWORD result;
    int retries=5;
    do {
        result = WaitForSingleObject(hEvent, INFINITE);
        if (result == WAIT_FAILED) {
            DWORD err = GetLastError();
            UNUSED(err);
            devassertmsg(0, "WaitForSingleObject failed");
            Sleep(100);
            retries--;
            if (retries<=0) {
                assertmsg(0, "WaitForSingleObject failed 5 times");
            }
        } else {
            assert(result==WAIT_OBJECT_0);
        }
    } while (result!=WAIT_OBJECT_0);
    return result;
}

HANDLE acquireThreadAgnosticMutex(const char *name)
{
    static bool inited=false;
    MutexEventPair *pair = calloc(sizeof(MutexEventPair), 1);
    char name_fixed[MAX_PATH];

    assert(name);

    if (!inited) {
        static volatile LONG num_people_initing=0;
        int result = InterlockedIncrement(&num_people_initing);
        if (result != 1) {
            while (!inited)
                Sleep(1);
        } else {
            // First person trying to init this
            threadAgnosticMutex_handle = (HANDLE)_beginthreadex( 
                NULL,                        // no security attributes 
                0,                           // use default stack size  
                threadAgnosticMutexThread,     // thread function 
                NULL,                         // argument to thread function 
                0,                           // use default creation flags 
                &threadAgnosticMutex_threadID); // returns the thread identifier 
            assert(threadAgnosticMutex_handle != NULL);
            inited = true;
        }
    }
    strcpy(name_fixed, name);
    {
        char *s = strchr(name_fixed, '\\'); // Preserve the namespace prefix.
        if (!s)
            s = name_fixed;
        _strupr_s(s, sizeof(name_fixed) - (size_t)(s - name_fixed));
    }
    pair->hMutex = CreateMutexA(NULL, FALSE, name_fixed); // initially not owned
    assert(pair->hMutex);
    pair->hEvent = CreateEventA( NULL, FALSE, FALSE, NULL); // Auto-reset, not signaled
    assert(pair->hEvent != NULL);
    pair->debug_name = name;
    //memlog_printf(NULL, "acquireThreadAgnosticMutex(%08x,%08x)", pair->hMutex, pair->hEvent);

    QueueUserAPC(threadAgnosticMutexAcquireFunc, threadAgnosticMutex_handle, (ULONG_PTR)pair );
    //memlog_printf(NULL, "acquireThreadAgnosticMutex(%08x):Waiting", pair->hMutex);
    WaitForEventInfiniteSafe(pair->hEvent);
    //memlog_printf(NULL, "acquireThreadAgnosticMutex(%08x):Done", pair->hMutex);

    return pair;
}

void releaseThreadAgnosticMutex(HANDLE hPair)
{
    MutexEventPair *pair = (MutexEventPair*)hPair;
    DWORD ret;
    //memlog_printf(NULL, "releaseThreadAgnosticMutex(%08x)", pair->hMutex);
    ret = QueueUserAPC(threadAgnosticMutexReleaseFunc, threadAgnosticMutex_handle, (ULONG_PTR)pair );
    //memlog_printf(NULL, "releaseThreadAgnosticMutex(%08x):Waiting (Queue returned %d)", pair->hMutex, ret);
    WaitForEventInfiniteSafe(pair->hEvent);
    //memlog_printf(NULL, "releaseThreadAgnosticMutex(%08x):Done", pair->hMutex);
    CloseHandle(pair->hMutex);
    CloseHandle(pair->hEvent);
    free(pair);
}

#define TTAM_NUM_TO_LAUNCH 3
#define TTAM_NUM_MUTEXES 2
#define TTAM_NUM_THREADS 3
static int ttam_timer;
static F32 ttam_last_result[TTAM_NUM_THREADS];

static unsigned __stdcall testThreadAgnosticMutexThread(void *dwParam)
{
    int threadnum = PTR_TO_S32(dwParam);
    do {
        char name[10];
        int mutex = rand() * TTAM_NUM_MUTEXES / (RAND_MAX + 1);
        HANDLE h;
        sprintf_s(SAFESTR(name), "TTAM_%d", mutex);
        h = acquireThreadAgnosticMutex(name);
        Sleep(rand() * 100 / (RAND_MAX + 1));
        releaseThreadAgnosticMutex(h);
        ttam_last_result[threadnum] = timerElapsed(ttam_timer);
    } while (true);
    return 0;
}

#ifndef _XBOX
// usage: put return testThreadAgnosticMutex(argc, argv); in main()
int testThreadAgnosticMutex(int argc, char **argv)
{
    int i;
    if (argc==1) {
        // launcher
        for (i=0; i<TTAM_NUM_TO_LAUNCH; i++) {
            char buf[MAX_PATH];
            sprintf_s(SAFESTR(buf), "%s %d", getExecutableName(), i);
            system_detach(buf, 0);
            Sleep(10);
        }
        return 0;
    } else {
        srand(_time32(NULL));
        ttam_timer = timerAlloc();
        for (i=0; i<TTAM_NUM_THREADS; i++) {
            _beginthreadex(NULL, 0, testThreadAgnosticMutexThread, S32_TO_PTR(i), 0, NULL);
        }
        do {
            for (i=0; i<TTAM_NUM_THREADS; i++) {
                printf("%4.1f ", ttam_last_result[i]);
            }
            printf("\n");
            Sleep(500);
        } while (true);
    }
    return 0;
}

HANDLE acquireMutexHandleByPID(const char* prefix, S32 pid, const char* suffix){
    char    buffer[1000];
    HANDLE    hMutex;
    
    if(pid < 0){
        pid = _getpid();
    }
    
    STR_COMBINE_SSDS(buffer, "Global\\", prefix ? prefix : "", pid, suffix ? suffix : "");
    
    hMutex = CreateMutexA(NULL, TRUE, buffer);

    if(hMutex){
        if(GetLastError() == ERROR_ALREADY_EXISTS){
            // Another process created it first.
            
            CloseHandle(hMutex);
        }else{
            // I got it!
            
            return hMutex;
        }
    }else{
        // Another process has it, and I don't have access rights.  Do nothing.
    }

    return NULL;
}

S32 countMutexesByPID(const char* prefix, const char* suffix)
{
    PROCESSENTRY32        pe32 = {0};
    HANDLE                hProcessSnap;
    S32                    notDone;
    S32                    count = 0;

    // Find all the processs in the system.

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(hProcessSnap == INVALID_HANDLE_VALUE)
    {
        // This should never happen, but you never know.
        
        return 0;
    }
    
    pe32.dwSize = sizeof(pe32);
    
    for(notDone = Process32First(hProcessSnap, &pe32); notDone; notDone = Process32Next(hProcessSnap, &pe32))
    {
        HANDLE hMutex = acquireMutexHandleByPID(prefix, pe32.th32ProcessID, suffix);

        if(hMutex){
            ReleaseMutex(hMutex);
            CloseHandle(hMutex);
        }else{
            count++;
        }
    }
    
    CloseHandle(hProcessSnap);
    
    return count;
}

#endif