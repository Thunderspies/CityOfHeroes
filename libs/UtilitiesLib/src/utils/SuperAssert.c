/* File assert.c
 *    Ripped and modified version of assert.c found in the MS CRT lib.
 *    
 */
#include "utilitieslib/utils/assert_dump.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/memlog.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/Stackdump.h"
#include "utilitieslib/utils/osdependent.h"
#include "tchar.h"
#include <tlhelp32.h>
#include <crashrpt/CrashRpt.h>
#include "utilitieslib/network/netio.h"
#include "utilitieslib/utils/log.h"

extern char g_pigErrorBuffer[2048];    // piglib.c

#if USE_NEW_TIMING_CODE
    void autoTimerDisableRecursion(S32 increment);
#else
    void autoTimerDisableRecursion(S32 increment){}
#endif

// exception table for looking up our structured exception
#define EXCPT_NAME(x) { x, #x },
typedef struct ExceptionName {
    DWORD code;
    char* name;
} ExceptionName;
ExceptionName g_exceptnames[] = {
    EXCPT_NAME(EXCEPTION_ACCESS_VIOLATION)
    EXCPT_NAME(EXCEPTION_DATATYPE_MISALIGNMENT)
    EXCPT_NAME(EXCEPTION_BREAKPOINT)
    EXCPT_NAME(EXCEPTION_SINGLE_STEP)
    EXCPT_NAME(EXCEPTION_ARRAY_BOUNDS_EXCEEDED)
    EXCPT_NAME(EXCEPTION_FLT_DENORMAL_OPERAND)
    EXCPT_NAME(EXCEPTION_FLT_DIVIDE_BY_ZERO)
    EXCPT_NAME(EXCEPTION_FLT_INEXACT_RESULT)
    EXCPT_NAME(EXCEPTION_FLT_INVALID_OPERATION)
    EXCPT_NAME(EXCEPTION_FLT_OVERFLOW)
    EXCPT_NAME(EXCEPTION_FLT_STACK_CHECK)
    EXCPT_NAME(EXCEPTION_FLT_UNDERFLOW)
    EXCPT_NAME(EXCEPTION_INT_DIVIDE_BY_ZERO)
    EXCPT_NAME(EXCEPTION_INT_OVERFLOW)
    EXCPT_NAME(EXCEPTION_PRIV_INSTRUCTION)
    EXCPT_NAME(EXCEPTION_IN_PAGE_ERROR)
    EXCPT_NAME(EXCEPTION_ILLEGAL_INSTRUCTION)
    EXCPT_NAME(EXCEPTION_NONCONTINUABLE_EXCEPTION)
    EXCPT_NAME(EXCEPTION_STACK_OVERFLOW)
    EXCPT_NAME(EXCEPTION_INVALID_DISPOSITION)
    EXCPT_NAME(EXCEPTION_GUARD_PAGE)
    EXCPT_NAME(EXCEPTION_INVALID_HANDLE)
};

#define ASSERT_BUF_SIZE 1024 * 10    // Be prepared for a large stackdump.
#define VERSION_BUF_SIZE 512
char assertbuf[ASSERT_BUF_SIZE * 2] = "";
char versionbuf[VERSION_BUF_SIZE] = "";

/*
 * assertion string components for message box
 */
#define BOXINTRO    "Assertion failed"
#define PROGINTRO   "Program: "
#define VERINTRO    "Version: "
#define TIMEINTRO    "Time: "
#define PROCESSID    "Process ID: "
#define FILEINTRO   "File: "
#define LINEINTRO   "Line: "
#define EXTRAINFO    "Extra Info: "
#define EXTRAINFO2    "\nExtra Info:\n"
#define GAMEPROGRESS    "\nGame Progress:\n"
#define EXPRINTRO   "Expression: "
#define MSGINTRO    "Error Message: "

#define DOTDOTDOTSZ 3
#define NEWLINESZ   1
#define DBLNEWLINESZ   2

#define MAXLINELEN 60
#define GAMEPROGRESSSTRINGSIZE 100

static char * dotdotdot = "...";
static char * newline = "\n";
static char * dblnewline = "\n\n";

static char* staticExtraInfo = NULL;
static char* staticExtraInfo2 = NULL;
static char  staticGameProgressString[GAMEPROGRESSSTRINGSIZE] = ""; // This gets updated repeatedly.  So, use a static buffer
static char* staticOpenGLReportFileName = NULL;
static char* staticLauncherLogFileName = NULL;
static char* staticAuthName = NULL;
static char* staticEntityName = NULL;
static char* staticShardName = NULL;
static char* staticShardTime = NULL;
static bool staticIsUnitTesting = false;

char* GetExceptionName(DWORD code)
{
    int i;
    static char buf[100];
    for (i = 0; i < ARRAY_SIZE(g_exceptnames); i++)
    {
        if (g_exceptnames[i].code == code)
            return g_exceptnames[i].name;
    }
    sprintf_s(SAFESTR(buf), "%x", code);
    return buf;
}

static char* TimeStr()
{
    __time32_t ts;
    struct tm newtime;
    char am_pm[] = "AM";
    static char result[100];

    ts = _time32(NULL);
    _localtime32_s(&newtime, &ts);

    if( newtime.tm_hour >= 12 )
            strcpy( am_pm, "PM" );
    if( newtime.tm_hour > 12 )        
            newtime.tm_hour -= 12;    
    if( newtime.tm_hour == 0 )        
            newtime.tm_hour = 12;

    asctime_s(SAFESTR(result),&newtime);
    strcpy_s(result+20, ARRAY_SIZE(result)-20, am_pm);
    return result; // returns static buffer
}

static void addExtraInfo(char *assertbuf, int n)
{
    /*
    * Extra information
    */

    if(staticExtraInfo)
    {
        strncat_s( assertbuf, n, newline, _TRUNCATE );
        strncat_s( assertbuf, n, staticExtraInfo, _TRUNCATE );
        strncat_s( assertbuf, n, newline, _TRUNCATE );
    }

    /*
    * More Extra information
    */

    if(staticExtraInfo2)
    {
        strncat_s( assertbuf, n, EXTRAINFO2, _TRUNCATE );
        strncat_s( assertbuf, n, staticExtraInfo2, _TRUNCATE );
        strncat_s( assertbuf, n, newline, _TRUNCATE );
    }

    /*
    * Even More Extra information
    */

    if(staticGameProgressString[0] != 0)
    {
        strncat_s( assertbuf, n, GAMEPROGRESS, _TRUNCATE );
        strncat_s( assertbuf, n, staticGameProgressString, _TRUNCATE );
        strncat_s( assertbuf, n, newline, _TRUNCATE );
    }

    /*
    * Memory corruption detection
    */
    {
        int memoryGood = heapValidateAll(); 
        if (memoryGood) {
            strncat_s( assertbuf, n, "Heap is NOT corrupted\n", _TRUNCATE);
        } else {
            strncat_s( assertbuf, n, "Heap is CORRUPTED\n", _TRUNCATE);
        }
    }

    /*
    * Pigg errors
    */
    strncat_s( assertbuf, n, "Pigg Error Buffer: ", _TRUNCATE);
    strncat_s( assertbuf, n, g_pigErrorBuffer, _TRUNCATE);
    strncat_s( assertbuf, n, newline, _TRUNCATE );
}

static void fillAssertBuffer(unsigned int code, PEXCEPTION_POINTERS info, DWORD dLastError, char *cWindowsErrorMessage)
{
    char * pch;
    char curtime[100] = "";
    char processID[100] = "";
    char progname[MAX_PATH + 1] = "";
    char stackdump[ASSERT_BUF_SIZE] = "";
    U32 local_ip;
    char ipaddr[100] = "";

    memlog_printf(0, "Exception caught: %s", GetExceptionName(code));
    /*
    * Line 0: exception line
    */
    sprintf_s(SAFESTR(assertbuf), "Exception caught: %s\n", GetExceptionName(code));

    /*
    * Line 1: program line
    */
    Strncatt( assertbuf, PROGINTRO );

    progname[0] = '\0';
    if ( !GetModuleFileNameA( NULL, progname, MAX_PATH ))
        strcpy( progname, "<program name unknown>");

    pch = (char *)progname;

    /* sizeof(PROGINTRO) includes the NULL terminator */
    if ( sizeof(PROGINTRO) + strlen(progname) + NEWLINESZ > MAXLINELEN )
    {
        size_t offset = (sizeof(PROGINTRO) + strlen(progname) + NEWLINESZ) - MAXLINELEN;
        size_t bufsize = ARRAY_SIZE_CHECKED(progname) - offset;
        pch += offset;
        strncpy_s( pch, bufsize, dotdotdot, DOTDOTDOTSZ );
    }

    local_ip = getHostLocalIp();
    sprintf_s(SAFESTR(ipaddr), "IP addr: %s\n", makeIpStr(local_ip));

    Strncatt( assertbuf, pch );
    Strncatt( assertbuf, newline );
    Strncatt( assertbuf, ipaddr );

    if(strlen(versionbuf))
    {
        Strncatt( assertbuf, VERINTRO );
        Strncatt( assertbuf, versionbuf );
        Strncatt( assertbuf, newline );
    }

    /*
    * Line 1.5: svn revision
    */
    Strncatt( assertbuf, "SVN Revision: " );
    Strncatt( assertbuf, build_version );
    Strncatt( assertbuf, newline );

    /*
    * Line 2: time
    */

    sprintf_s(SAFESTR(curtime), "%s%s\n", TIMEINTRO, TimeStr() );
    Strncatt( assertbuf, curtime );


    /*
    * Line 3: process ID
    */

    sprintf_s(SAFESTR(processID), "%s%d\n", PROCESSID, _getpid() );
    Strncatt( assertbuf, processID );
    Strncatt( assertbuf, dblnewline );




    /*
    * Print the stack
    */

    sdDumpStackToBuffer(stackdump, ASSERT_BUF_SIZE, info->ContextRecord);
    Strncatt(assertbuf, stackdump);

    // Extra info
    addExtraInfo(assertbuf, ARRAY_SIZE(assertbuf));

    /*
    * Line 4: windows error?
    */
    if ( dLastError )
    {
        Strncatt(assertbuf, "Last windows SYSTEM error: ");
        Strncatt(assertbuf, cWindowsErrorMessage);
    }
    else
    {
        Strncatt(assertbuf, "No windows SYSTEM error code found.\n");
    }
}

// Crash report related fields.
struct {
    int running;
    void* handle;

    pfnCrashRptInstall                Install;
    pfnCrashRptGenerateErrorReport2    Report;
    pfnCrashRptAbortErrorReport        StopReport;
    pfnCrashRptUninstall            Uninstall;
} cr;

static int CallReportFault(PEXCEPTION_POINTERS info, const char *authName, const char *entityName, const char *shardName, const char *shardTime, const char *message, const char *glReportFileName, const char *launcherLogFileName)
{
    HMODULE hDll = NULL;
    int ret = EXCEPTION_CONTINUE_SEARCH; // let OS try to take exception

    hDll = loadCrashRptDll();
    if(!hDll)
        return ret;

    cr.Install        = (pfnCrashRptInstall)GetProcAddress(hDll, "CrashRptInstall");
    cr.Report        = (pfnCrashRptGenerateErrorReport2)GetProcAddress(hDll, "CrashRptGenerateErrorReport2");
    cr.StopReport    = (pfnCrashRptAbortErrorReport)GetProcAddress(hDll, "CrashRptAbortErrorReport");
    cr.Uninstall    = (pfnCrashRptUninstall)GetProcAddress(hDll, "CrashRptUninstall");
    if(cr.Install && cr.Report && cr.StopReport && cr.Uninstall)
    {
        cr.handle = cr.Install(NULL);
        if(cr.handle)
        {
            cr.running = 1;
            cr.Report(cr.handle, info, authName, entityName, shardName, shardTime, versionbuf, message, glReportFileName, launcherLogFileName);
            cr.running = 0;
            cr.Uninstall(cr.handle);
            ret = EXCEPTION_EXECUTE_HANDLER;

            ZeroStruct(&cr);
        }
    }
    FreeLibrary(hDll);
    return ret;
}

typedef struct ThreadHandleId
{
    HANDLE handle; // hold on to handles to make sure our ids stay valid
    DWORD id;
} ThreadHandleId;
static CRITICAL_SECTION freeze_threads_section;
static CRITICAL_SECTION freeze_threads_section_two;
static bool freeze_threads_initialized = false;
static ThreadHandleId freeze_threads[128];    
static volatile long freeze_threads_count=0;
static ThreadHandleId safe_threads[128];
static volatile long safe_threads_count=0;
static bool canFreezeThisThread(DWORD threadID)
{
    int i;
    for (i=0; i < safe_threads_count && i < ARRAY_SIZE(safe_threads); i++)
        if(safe_threads[i].id==threadID)
            return false;
    for (i=0; i < freeze_threads_count && i < ARRAY_SIZE(freeze_threads); i++) 
        if(freeze_threads[i].id==threadID)
            return true;
    return false;
}
static void assertFreezeAllOtherThreads(int resume)
{

    DWORD            dwOwnerPID    = GetCurrentProcessId();
    DWORD            dwOwnerTID    = GetCurrentThreadId();
    HANDLE            hOwnerThread= GetCurrentThread();
    HANDLE            hThreadSnap    = NULL; 
    BOOL            bRet        = FALSE; 
    THREADENTRY32    te32        = {0}; 
    typedef HANDLE (WINAPI *tOpenThread)(DWORD dwDesiredAccess, BOOL bInheritHandle, unsigned dwThreadId);
    tOpenThread        pOpenThread;
    HMODULE            hKernel32Dll = LoadLibrary(_T("kernel32.dll"));

    if (staticIsUnitTesting)
        return;

    if (!hKernel32Dll)
        return;
    pOpenThread = (tOpenThread)GetProcAddress(hKernel32Dll, "OpenThread");
    if (!pOpenThread)
    {
        CloseHandle(hKernel32Dll);
        return;
    }

    assertDoNotFreezeThisThread(hOwnerThread,dwOwnerTID);

    // Take a snapshot of all threads currently in the system. 
    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); 
    if (hThreadSnap == INVALID_HANDLE_VALUE) 
        return;
 
    te32.dwSize = sizeof(THREADENTRY32); 
 
    if (Thread32First(hThreadSnap, &te32))
    {
        do {
            if (te32.th32OwnerProcessID == dwOwnerPID)
            { 
                if (canFreezeThisThread(te32.th32ThreadID))
                {
                    HANDLE hThread = pOpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                    if (hThread) {
                        if (resume) {
                            // Resmue it!
                            ResumeThread(hThread);
                        } else {
                            // Suspend it!
                            SuspendThread(hThread);
                        }
                        CloseHandle(hThread);
                    }
                }
            } 
        } 
        while (Thread32Next(hThreadSnap, &te32)); 
        bRet = TRUE; 
    } 
    else 
        bRet = FALSE;          // could not walk the list of threads 
 
    CloseHandle (hThreadSnap); 
 
    return; 
}
void assertInitThreadFreezing()
{
    assert(!staticIsUnitTesting);

    if(devassertmsg(!freeze_threads_initialized,"You must not call assertInitThreadFreezing more than once"))
    {
        InitializeCriticalSection(&freeze_threads_section);
        InitializeCriticalSection(&freeze_threads_section_two);
        freeze_threads_initialized = true;
    }
}
void assertYouMayFreezeThisThread(HANDLE thread, DWORD threadId)
{
    long index;
    int i;

    if (staticIsUnitTesting)
        return;

    for (i=0; i<freeze_threads_count; i++) 
        if (freeze_threads[i].id==threadId)
            return;

    if(freeze_threads_count < ARRAY_SIZE(freeze_threads))
    {
        index = InterlockedIncrement(&freeze_threads_count);
        if(index <= ARRAY_SIZE(freeze_threads))
        {
            if( !thread ||
                !devassert(DuplicateHandle(GetCurrentProcess(),thread,GetCurrentProcess(),&freeze_threads[index-1].handle,0,FALSE,DUPLICATE_SAME_ACCESS)) )
            {
                freeze_threads[index-1].handle = NULL; // don't reuse this slot
            }
            freeze_threads[index-1].id = threadId;
            return;
        }
    }

    if(devassertmsg(freeze_threads_initialized,"You must call memCheckInit before assertYouMayFreezeThisThread"))
    {
        EnterCriticalSection(&freeze_threads_section);
        for(i = 0; i < ARRAY_SIZE(freeze_threads); ++i)
        {
            if(freeze_threads[i].handle && WaitForSingleObject(freeze_threads[i].handle,0) != WAIT_TIMEOUT)
            {
                CloseHandle(freeze_threads[i].handle);
                if( !thread ||
                    !devassert(DuplicateHandle(GetCurrentProcess(),thread,GetCurrentProcess(),&freeze_threads[i].handle,0,FALSE,DUPLICATE_SAME_ACCESS)) )
                {
                    freeze_threads[i].handle = NULL; // don't reuse this slot
                }
                freeze_threads[i].id = threadId;
                break;
            }
        }
        LeaveCriticalSection(&freeze_threads_section);
        devassertmsg(i < ARRAY_SIZE(freeze_threads), "freeze_threads overflow");
    }
    // else we won't be freezing this thread
}
void assertDoNotFreezeThisThread(HANDLE thread, DWORD threadID)
{
    long index;
    int i;

    if (staticIsUnitTesting)
        return;

    for (i=0; i<safe_threads_count; i++)
        if (safe_threads[i].id==threadID)
            return;

    index = InterlockedIncrement(&safe_threads_count);
    if(index <= ARRAY_SIZE(freeze_threads))
    {
        if(thread)
            DuplicateHandle(GetCurrentProcess(),thread,GetCurrentProcess(),&safe_threads[index-1].handle,0,FALSE,DUPLICATE_SAME_ACCESS);
        safe_threads[index-1].id = threadID;
    }
}

void setAssertProgramVersion(const char* versionName)
{
    Strncpyt(versionbuf, versionName);
}

void setAssertExtraInfo(const char* info)
{
    if(staticExtraInfo)
    {
        free(staticExtraInfo);
        staticExtraInfo = NULL;
    }
    
    if(info)
    {
        staticExtraInfo = strdup(info);
    }
}

void setAssertExtraInfo2(const char* info)
{
    if(staticExtraInfo2)
    {
        free(staticExtraInfo2);
        staticExtraInfo2 = NULL;
    }

    if(info)
    {
        staticExtraInfo2 = strdup(info);
    }
}

void setAssertGameProgressString(const char* info)
{
    strncpy(staticGameProgressString, info, GAMEPROGRESSSTRINGSIZE);
    staticGameProgressString[GAMEPROGRESSSTRINGSIZE-1] = 0;
}

void setAssertUnitTesting(bool enable)
{
    staticIsUnitTesting = enable;
}

void setAssertOpenGLReport(const char* fileName)
{
    if(staticOpenGLReportFileName)
    {
        free(staticOpenGLReportFileName);
        staticOpenGLReportFileName = NULL;
    }

    if(fileName)
    {
        staticOpenGLReportFileName = strdup(fileName);
    }
}

void setAssertLauncherLogs(const char* fileName)
{
    if(staticLauncherLogFileName)
    {
        free(staticLauncherLogFileName);
        staticLauncherLogFileName = NULL;
    }

    if(fileName)
    {
        staticLauncherLogFileName = strdup(fileName);
    }
}


void setAssertAuthName(const char* authName)
{
    if(staticAuthName)
    {
        free(staticAuthName);
        staticAuthName = NULL;
    }

    if(authName)
    {
        staticAuthName = strdup(authName);
    }
}

void setAssertEntityName( const char* entityName)
{
    if(staticEntityName)
    {
        free(staticEntityName);
        staticEntityName = NULL;
    }

    if(entityName)
    {
        staticEntityName = strdup(entityName);
    }
}

void setAssertShardName(const char* shardName)
{
    if(staticShardName)
    {
        free(staticShardName);
        staticShardName = NULL;
    }

    if(shardName)
    {
        staticShardName = strdup(shardName);
    }
}

void setAssertShardTime(int shardTime)
{
    if(staticShardTime)
    {
        free(staticShardTime);
        staticShardTime = NULL;
    }

    if(shardTime)
    {
        static char shardTimeName[12];
        sprintf_s(shardTimeName, sizeof(shardTimeName), "%d", shardTime);
        staticShardTime = strdup(shardTimeName);
    }
}

#ifndef DISABLE_ASSERTIONS
#include "utilitieslib/utils/memcheck.h"
#include <signal.h>
#include <stdio.h>
#include <process.h>
#include "utilitieslib/assert/assert.h"
#include "signal.h"
#include "utilitieslib/network/sock.h"
#include <time.h>
#include "errorrep.h"
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/fileutil.h"
#include "utilitieslib/utils/winutil.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/components/SharedHeap.h"
#include "utilitieslib/stdtypes.h"
#include <share.h>

//Added this NULLPTR to fix ASSERT
int *g_NULLPTR = 0;

int __cdecl __crtMessageBoxA(LPCSTR, LPCSTR, UINT);

static int g_isAsserting = 0;
int isCrashed(void)
{
    return g_isAsserting;
}

/*
 * assertion format string for use with output to stderr
 */
static char _assertstring[] = "Assertion failed: %s, file %s, line %d\n";

/*      Format of MessageBox for assertions:
*
*       ================= Microsft Visual C++ Debug Library ================
*
*       Assertion Failed!
*
*       Program: c:\test\mytest\foo.exe
*       File: c:\test\mytest\bar.c
*       Line: 69
*
*       Expression: <expression>
*
*        <Stack dump>
*
*       ===================================================================
*/

#undef _beginthreadex // don't go through the utils.h handler

static int g_assertmode = ASSERTMODE_DEBUGBUTTONS | ASSERTMODE_MINIDUMP; // default for internal projects
static int g_autoResponse = -1;
static int g_disableIgnore = 0;
int g_ignoredivzero = 0;            // if set, we advance and ignore integer divide-by-zero errors
static int g_ignoreexceptions = 0;    // if set we don't catch an exception.  Used by assert to fall into OS faster
static int generatingErrorRep = 0;

static DWORD dwListenThreadID=0;
static HANDLE hListenThread=0;

static int g_sendEmailonAssert = 0;
static char g_assertEmailAddresses[1024] = "";
static char g_machineName[256] = "";

// The default set of flags to pass to the minidump writer
MINIDUMP_TYPE minidump_base_flags = (MINIDUMP_TYPE)(MiniDumpNormal | MiniDumpWithProcessThreadData | MiniDumpWithDataSegs | MiniDumpWithIndirectlyReferencedMemory);
// The "safe" set of flags, call these with an Assert so that it doesn't corrupt memory!
MINIDUMP_TYPE minidump_assert_flags = (MINIDUMP_TYPE)(MiniDumpWithProcessThreadData | MiniDumpWithDataSegs);
// The set of flags that work on older versions of dbghelp.dll
MINIDUMP_TYPE minidump_old_flags = (MINIDUMP_TYPE)(MiniDumpNormal | MiniDumpWithDataSegs);
// The set to actually use
MINIDUMP_TYPE minidump_flags = 0;

int assertbuf_filled = 0; // If it's already been filled from the call to superassert
ErrorCallback assertCallback = NULL;
int g_programIsShuttingDown=0;

struct {
    struct {
        size_t lo;
        size_t hi;
        char* flag;
    } regions[8192];
    
    U32        regionCount;
    char*    bitNames[2][32];
} virtualMemory;

static int getBitNumber(U32 x){
    int i;
    
    for(i = 0; x & ~1; x >>= 1, i++);
    
    return i;
}

int getVirtualMemoryRegion(void* mem)
{
    U32 i;
    for(i = 0; i < virtualMemory.regionCount; i++)
    {
        if((uintptr_t)mem >= virtualMemory.regions[i].lo && (uintptr_t)mem < virtualMemory.regions[i].hi)
        {
            return i;
        }
    }
    return -1;
}

static void generateVirtualMemoryLayout()
{
    #define FLAG_MASK (    PAGE_EXECUTE|\
                        PAGE_EXECUTE_READ|\
                        PAGE_EXECUTE_READWRITE|\
                        PAGE_EXECUTE_WRITECOPY|\
                        PAGE_NOACCESS|\
                        PAGE_READONLY|\
                        PAGE_READWRITE|\
                        PAGE_WRITECOPY)

    char* lastAddress = (char*)0;
    char* curAddress = (char*)0x10016;  // Everything below this value is off limits.
    int    sum=0;
        
    virtualMemory.regionCount = 0;
    
    memset(virtualMemory.bitNames, 0, sizeof(virtualMemory.bitNames));
    
    #define SET_NAME(x) virtualMemory.bitNames[0][getBitNumber(x)] = #x
    SET_NAME(PAGE_EXECUTE);
    SET_NAME(PAGE_EXECUTE_READ);
    SET_NAME(PAGE_EXECUTE_READWRITE);
    SET_NAME(PAGE_EXECUTE_WRITECOPY);
    SET_NAME(PAGE_NOACCESS);
    SET_NAME(PAGE_READONLY);
    SET_NAME(PAGE_READWRITE);
    SET_NAME(PAGE_WRITECOPY);
    SET_NAME(PAGE_GUARD);
    SET_NAME(PAGE_NOCACHE);
    #undef SET_NAME

    #define SET_NAME(x) virtualMemory.bitNames[1][getBitNumber(x)] = "guard|"#x
    SET_NAME(PAGE_EXECUTE);
    SET_NAME(PAGE_EXECUTE_READ);
    SET_NAME(PAGE_EXECUTE_READWRITE);
    SET_NAME(PAGE_EXECUTE_WRITECOPY);
    SET_NAME(PAGE_NOACCESS);
    SET_NAME(PAGE_READONLY);
    SET_NAME(PAGE_READWRITE);
    SET_NAME(PAGE_WRITECOPY);
    SET_NAME(PAGE_GUARD);
    SET_NAME(PAGE_NOCACHE);
    #undef SET_NAME

    while(1)
    {
        MEMORY_BASIC_INFORMATION mbi;
        
        VirtualQuery(curAddress, &mbi, sizeof(mbi));
        
        if(mbi.BaseAddress == lastAddress)
        {
            break;
        }
        
        lastAddress = mbi.BaseAddress;
        
        if(mbi.State == MEM_COMMIT && !(mbi.Protect & PAGE_NOACCESS))
        {
            virtualMemory.regions[virtualMemory.regionCount].lo = (uintptr_t)mbi.BaseAddress;
            virtualMemory.regions[virtualMemory.regionCount].hi = virtualMemory.regions[virtualMemory.regionCount].lo + mbi.RegionSize;
            virtualMemory.regions[virtualMemory.regionCount].flag = virtualMemory.bitNames[(mbi.Protect & PAGE_GUARD) ? 1 : 0][getBitNumber(mbi.Protect & FLAG_MASK)];
            
            if(++virtualMemory.regionCount >= ARRAY_SIZE(virtualMemory.regions))
            {
                break;
            }
            sum += mbi.RegionSize;
        }
                
        curAddress += mbi.RegionSize;
    }
}

void setAssertCallback(ErrorCallback func)
{
    assertCallback = func;
}

void setAssertMode(int assertmode) { g_assertmode = assertmode; }
int getAssertMode() { return g_assertmode; }

int programIsShuttingDown() { return g_programIsShuttingDown; }
void setProgramIsShuttingDown(int val) {
    g_programIsShuttingDown=val;
}

void setAssertResponse(int idc_response)
{
    g_autoResponse = idc_response;
}

/* 'C' Style Array Dump created by Abalone Hex editor - Copyright 2000-2001 Winsor Computing */
unsigned char DialogResource[] = 
    {0x01, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0a, 0xc0, 0x10,
    0x09, 0x00, 0x16, 0x00, 0x11, 0x00, 0xe2, 0x00, 0xba, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x00,
    0x69, 0x00, 0x74, 0x00, 0x79, 0x00, 0x20, 0x00, 0x6f, 0x00, 0x66, 0x00, 0x20, 0x00, 0x48, 0x00,
    0x65, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x65, 0x00, 0x73, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x50, 0xa5, 0x00, 0xa6, 0x00,
    0x37, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x26, 0x00, 0x53, 0x00,
    0x74, 0x00, 0x6f, 0x00, 0x70, 0x00, 0x20, 0x00, 0x50, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x67, 0x00,
    0x72, 0x00, 0x61, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x50, 0x08, 0x00, 0x93, 0x00, 0x40, 0x00, 0x0b, 0x00,
    0x0b, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x26, 0x00, 0x52, 0x00, 0x65, 0x00, 0x6d, 0x00,
    0x6f, 0x00, 0x74, 0x00, 0x65, 0x00, 0x20, 0x00, 0x44, 0x00, 0x65, 0x00, 0x62, 0x00, 0x75, 0x00,
    0x67, 0x00, 0x67, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x50, 0x55, 0x00, 0xa6, 0x00, 0x34, 0x00, 0x0b, 0x00,
    0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x26, 0x00, 0x44, 0x00, 0x65, 0x00, 0x62, 0x00,
    0x75, 0x00, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x50, 0x08, 0x00, 0xa6, 0x00, 0x34, 0x00, 0x0b, 0x00, 0x03, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x80, 0x00, 0x26, 0x00, 0x49, 0x00, 0x67, 0x00, 0x6e, 0x00, 0x6f, 0x00, 0x72, 0x00,
    0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x02, 0x50, 0x0e, 0x00, 0x07, 0x00, 0xc4, 0x00, 0x14, 0x00, 0xff, 0xff, 0x00, 0x00,
    0xff, 0xff, 0x82, 0x00, 0x43, 0x00, 0x69, 0x00, 0x74, 0x00, 0x79, 0x00, 0x20, 0x00, 0x6f, 0x00,
    0x66, 0x00, 0x20, 0x00, 0x48, 0x00, 0x65, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x65, 0x00, 0x73, 0x00,
    0x20, 0x00, 0x68, 0x00, 0x61, 0x00, 0x73, 0x00, 0x20, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x63, 0x00,
    0x6f, 0x00, 0x75, 0x00, 0x6e, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x65, 0x00, 0x64, 0x00,
    0x20, 0x00, 0x61, 0x00, 0x6e, 0x00, 0x20, 0x00, 0x65, 0x00, 0x72, 0x00, 0x72, 0x00, 0x6f, 0x00,
    0x72, 0x00, 0x2e, 0x00, 0x0a, 0x00, 0x50, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x61, 0x00, 0x73, 0x00,
    0x65, 0x00, 0x20, 0x00, 0x72, 0x00, 0x65, 0x00, 0x70, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x74, 0x00,
    0x20, 0x00, 0x74, 0x00, 0x68, 0x00, 0x69, 0x00, 0x73, 0x00, 0x20, 0x00, 0x74, 0x00, 0x6f, 0x00,
    0x20, 0x00, 0x50, 0x00, 0x61, 0x00, 0x72, 0x00, 0x61, 0x00, 0x67, 0x00, 0x6f, 0x00, 0x6e, 0x00,
    0x20, 0x00, 0x53, 0x00, 0x74, 0x00, 0x75, 0x00, 0x64, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x73, 0x00,
    0x2e, 0x00, 0x20, 0x00, 0x20, 0x00, 0x54, 0x00, 0x68, 0x00, 0x61, 0x00, 0x6e, 0x00, 0x6b, 0x00,
    0x20, 0x00, 0x79, 0x00, 0x6f, 0x00, 0x75, 0x00, 0x2e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x50, 0x0f, 0x00, 0x20, 0x00,
    0xcd, 0x00, 0x09, 0x00, 0x0f, 0x00, 0x00, 0x00, 0xff, 0xff, 0x82, 0x00, 0x54, 0x00, 0x68, 0x00,
    0x69, 0x00, 0x73, 0x00, 0x20, 0x00, 0x69, 0x00, 0x73, 0x00, 0x20, 0x00, 0x61, 0x00, 0x20, 0x00,
    0x74, 0x00, 0x65, 0x00, 0x73, 0x00, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x50, 0x0f, 0x00, 0x20, 0x00, 0xcd, 0x00, 0x6a, 0x00,
    0x0a, 0x00, 0x00, 0x00, 0xff, 0xff, 0x82, 0x00, 0x53, 0x00, 0x74, 0x00, 0x61, 0x00, 0x74, 0x00,
    0x69, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x50, 0x52, 0x00, 0x93, 0x00, 0x3f, 0x00, 0x0b, 0x00, 0x0c, 0x00, 0x00, 0x00,
    0xff, 0xff, 0x80, 0x00, 0x26, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x70, 0x00, 0x79, 0x00, 0x20, 0x00,
    0x74, 0x00, 0x6f, 0x00, 0x20, 0x00, 0x43, 0x00, 0x6c, 0x00, 0x69, 0x00, 0x70, 0x00, 0x62, 0x00,
    0x6f, 0x00, 0x61, 0x00, 0x72, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x50, 0x9d, 0x00, 0x93, 0x00, 0x3f, 0x00, 0x0b, 0x00,
    0x0d, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 0x44, 0x00, 0x75, 0x00, 0x6d, 0x00, 0x70, 0x00,
    0x20, 0x00, 0x61, 0x00, 0x6e, 0x00, 0x64, 0x00, 0x20, 0x00, 0x53, 0x00, 0x74, 0x00, 0x6f, 0x00,
    0x70, 0x00, 0x00, 0x00, 0x00, 0x00};

/////////////////////////////////// from assertdlg
#define IDC_MAINTEXT 10
#define IDC_RUNDEBUGGER 11
#define IDC_COPYTOCLIPBOARD 12
#define IDC_DUMPWITHHEAP 13

#define BASE_LISTEN_PORT 314159 // Why?  Because I like pie.
bool g_listenThreadRunning=false;
int g_listenThreadResponse=0;

void sendError(SOCKET s, char *error) {
    int i = 0;
    send(s, (char*)&i, sizeof(i), 0);
    i = (int)strlen(error);
    send(s, (char*)&i, sizeof(i), 0);
    send(s, error, i, 0);
}

void sendPDB(SOCKET s) {
    char progname[MAX_PATH+1];
    char buffer[2048] = "";
    char *ss;
    int i, i2;
    int fh;
    struct stat status;

    progname[MAX_PATH] = '\0';
    if ( !GetModuleFileNameA( NULL, progname, MAX_PATH )) {
        sendError(s, "<program name unknown>");
        return;
    }
    // Convert to .pdb
    ss = strrchr(progname, '.');
    if (!ss) {
        sendError(s, "<invalid format of program name>");
        return;
    }
    strcpy_s(ss, ARRAY_SIZE(progname) - (ss - progname), ".pdb");

    _sopen_s(&fh, progname, _O_BINARY | _O_RDONLY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
    if (fh==-1) {
        sendError(s, "Cannot open PDB file for reading");
        return;
    }

    // Send size
    if(!stat(progname, &status)){
        if(!(status.st_mode & _S_IFREG)) {
            sendError(s, "Bad PDB file (it's a directory?");
            return;
        }
        // Send filename
        i = (int)strlen(progname);
        send(s, (char*)&i, sizeof(i), 0);
        send(s, progname, i, 0);

        i = status.st_size;
        printf("Sending %d bytes\n", i);
        send(s, (char*)&i, sizeof(i), 0);
    }
    do {
        i = _read(fh, buffer, 1400);
        if (i) {
            i2 = send(s, buffer, i, 0);
            if (i!=i2) {
                printf("Error calling send, only sent %d bytes!\n", i2);
                break;
            }
        }
    } while (i==1400);
    _close(fh);
    printf("Send complete\n");
}

static int ipIsInternal(struct sockaddr_in* ip)
{
#define IP_BYTE(ip, byteID) (ip->sin_addr.S_un.S_un_b.s_b##byteID)
    if(IP_BYTE(ip, 1) == 10)
    {
        return 1;
    }
    else if(IP_BYTE(ip,1) == 172)
    {
        int val = IP_BYTE(ip,2);
        if(val >= 16 && val <= 31)
            return 1;
    }
    else if(IP_BYTE(ip,1) == 192)
    {
        if(IP_BYTE(ip,2) == 168)
            return 1;
    }
    return 0;
}

static void    wsockStart()
{
    WORD wVersionRequested;  
    WSADATA wsaData; 
    int err; 
    wVersionRequested = MAKEWORD(2, 2); 

    err = WSAStartup(wVersionRequested, &wsaData); 
}

unsigned __stdcall listenThreadMain(void *data) {
    struct sockaddr_in    addr_in;
    int port=BASE_LISTEN_PORT;
    int result;
    SOCKET s;
    SOCKET s2;

    dwListenThreadID = GetCurrentThreadId();
    assertDoNotFreezeThisThread(hListenThread,dwListenThreadID);

    wsockStart();

    if (assertCallback && !(g_assertmode & ASSERTMODE_CALLBACK)) // If the mode includes CALLBACK this was executed in the main thread
        assertCallback(assertbuf);

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    do {
        sockSetAddr(&addr_in,htonl(INADDR_ANY),port);
        if (sockBind(s,&addr_in))
            break;
        port++;
        if (port > BASE_LISTEN_PORT + 8) {// If these ports are all aready taken, just give up!
            // Intentionally left this running so we don't get a large number of threads spiraling out of control
            //printf("Couldn't find an open port");
            dwListenThreadID = 0;
            return 0;
        }
    } while (1);
    //printf("Bound to port %d\n", port);
    result = listen(s, 15);
    if(result)
    {
        // Intentionally left this running so we don't get a large number of threads spiraling out of control
        //printf("listen error..\n");
        dwListenThreadID = 0;
        return 0;
    }
    do {
        char buffer[16];
        int len;
        struct sockaddr_in incomingIP;
        int addrLen = sizeof(incomingIP);
        s2 = accept(s, (SOCKADDR*)&incomingIP, &addrLen);
        if (s2==INVALID_SOCKET)
            continue;
        if(0 && !ipIsInternal(&incomingIP)) // Doesn't always work!  Need this to work on NCSoft machines
        {
            closesocket(s2);
            continue;
        }

        len = (int)strlen(assertbuf)+1;
        send(s2, (char*)&len, sizeof(len), 0);
        send(s2, assertbuf, len, 0);
        do {
            // Wait for the main thread to grab the value
            while (g_listenThreadResponse!=0) {
                Sleep(10);
            }
            do {
                result = recv(s2, buffer, 1, 0);
                if (result==SOCKET_ERROR) {
                    //printf("read error...\n");
                    g_listenThreadRunning = false;
                    dwListenThreadID = 0;
                    return 0;
                }
            } while (result==0);
            switch (buffer[0]) {
                case 1:
                    g_listenThreadResponse = IDC_STOP+1;
                    //printf("Remote debuggerer said STOP\n");
                    break;
                case 2:
                    g_listenThreadResponse = IDC_DEBUG+1;
                    if(cr.running)
                        cr.StopReport(cr.handle);
                    //printf("Remote debuggerer said DEBUG\n");
                    break;
                case 3:
                    g_listenThreadResponse = IDC_IGNORE+1;
                    //printf("Remote debuggerer said IGNORE\n");
                    break;
                case 4:
                    g_listenThreadResponse = IDC_RUNDEBUGGER+1;
                    //printf("Remote debuggerer said RUNDEBUGGER\n");
                    break;
                case 5:
                    // Get PDB
                    sendPDB(s2);
                    break;
                case 0:
                    //printf("Remote debuggerer said No more commands\n");
                    break;
                default:
                    sendError(s2, "Unhandled command (old version of the assert dialog?)");
                    //printf("Invalid data from remote debugger debugger client...\n");
                    break;
            }
        } while (buffer[0]);
        closesocket(s2);
    } while (1);
    //printf("ListenThread exiting...\n");
    g_listenThreadRunning = false;
    dwListenThreadID = 0;
    return 0;
}

void listenThreadBegin() {
    g_listenThreadResponse=0;
    if (g_listenThreadRunning) {
        //printf("Thread already running\n");
        return;
    }
    g_listenThreadRunning = true;
    hListenThread = (HANDLE)_beginthreadex(NULL, 0, listenThreadMain, NULL, 0, NULL);
    if (hListenThread > 0 && (hListenThread != INVALID_HANDLE_VALUE)) {
        while (dwListenThreadID == 0)
            Sleep(1); // Wait for listen thread to start
        CloseHandle(hListenThread);
    } else {
        // Thread failed to start, call the callback in this thread!
        g_assertmode |= ASSERTMODE_CALLBACK;
    }
}

void enableEmailOnAssert(int yesno)
{
    g_sendEmailonAssert = yesno;
}

void addEmailAddressToAssert(char *address)
{
    Strncatt(g_assertEmailAddresses, " ");
    Strncatt(g_assertEmailAddresses, address);
}

void setMachineName(char *name)
{
    Strcpy(g_machineName, name);
}

static void sendAssertEmail(void)
{
    int i = 0;
    char command[4096 + ASSERT_BUF_SIZE * 2] = "";
    char *cur = g_assertEmailAddresses;

    Strncatt(command, "c:\\game\\tools\\util\\sendemail.exe");

    if ( !fileExists(command) )
        return;

    i = (int)strlen(command);

#define APPEND_TO {command[i++] = ' '; command[i++] = '-'; command[i++] = 't'; command[i++] = 'o'; command[i] = ' ';}

    while ( cur && *cur )
    {
        if ( *cur != ' ' )
            command[i] = *cur;
        else
            APPEND_TO;
        ++i;
        ++cur;
    }

    Strncatt(command, " -from assert@ncsoft-url-stripped-todo.com");
    Strncatt(command, " -subj \"Assert on ");
    Strncatt(command, g_machineName[0] ? g_machineName : "Unknown");
    Strncatt(command, "\"");
    Strncatt(command, " -msg \"");
    Strncatt(command, escapeString(assertbuf));
    Strncatt(command, "\"");

    system(command);
}

// Message handler for about box.
INT_PTR CALLBACK Assert(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int firsttime = TRUE;
    switch (message)
    {
    case WM_INITDIALOG:
        {
            HFONT hf;
            LOGFONTA lf;
            RECT rc;
            int heightneeded;
            int delta;

            // log assert
            //logNAddEntry("assert", "%s\n", assertbuf);

            // if we have an auto-response..
            if (g_autoResponse > 0)
            {
                SendMessageA(hDlg, WM_COMMAND, g_autoResponse, 0);
                EndDialog(hDlg, g_autoResponse);
                return FALSE;
            }

            // figure out the height and width needed for text
            ZeroStruct(&lf);
            lf.lfHeight = 14;
            lf.lfWeight = FW_NORMAL;
            hf = CreateFontIndirectA(&lf);
            heightneeded = (14) * NumLines(assertbuf);
            
            // Set name.
            
            if(0){
                char orig[100];
                char newName[100];
                GetWindowTextA(hDlg, orig, ARRAY_SIZE(orig) - 1);
                STR_COMBINE_BEGIN(newName);
                STR_COMBINE_CAT_D(_getpid());
                STR_COMBINE_CAT(" : ");
                STR_COMBINE_CAT(orig);
                STR_COMBINE_END();
                SetWindowTextA(hDlg, newName);
            }
            
            // resize text box
            GetWindowRect(GetDlgItem(hDlg, IDC_MAINTEXT), &rc);
            delta = 6 + heightneeded - rc.bottom + rc.top;
            if (delta > 100) delta = 100; // cap growth
            MapWindowPoints(NULL, hDlg, (LPPOINT)&rc, 2);
            MoveWindow(GetDlgItem(hDlg, IDC_MAINTEXT), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top+delta, FALSE);
            
            // resize main window
            GetWindowRect(hDlg, &rc);
            MoveWindow(hDlg, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top+delta, FALSE);

            // move buttons
            OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_STOP), 0, delta);
            OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_DEBUG), 0, delta);
            OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_IGNORE), 0, delta);
            OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_RUNDEBUGGER), 0, delta);
            OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_COPYTOCLIPBOARD), 0, delta);
            OffsetWindow(hDlg, GetDlgItem(hDlg, IDC_DUMPWITHHEAP), 0, delta);

            // show what I'm doing..
            //TCHAR buf[2000];
            //wsprintf(buf, "Need to be %i high, delta %i", heightneeded, delta);
            //MessageBox(hDlg, buf, "debug", 0);

            // set text
            SendDlgItemMessage(hDlg, IDC_MAINTEXT, WM_SETFONT, (WPARAM)hf, 0);
            SetWindowTextA(GetDlgItem(hDlg, IDC_MAINTEXT), assertbuf);

            // disable other buttons for a bit
            EnableWindow(GetDlgItem(hDlg, IDCANCEL), FALSE);
            if (firsttime)
            {
                EnableWindow(GetDlgItem(hDlg, IDC_STOP), FALSE);
                EnableWindow(GetDlgItem(hDlg, IDC_DEBUG), FALSE);
                EnableWindow(GetDlgItem(hDlg, IDC_IGNORE), FALSE);
                EnableWindow(GetDlgItem(hDlg, IDC_RUNDEBUGGER), FALSE);
                EnableWindow(GetDlgItem(hDlg, IDC_COPYTOCLIPBOARD), FALSE);
                EnableWindow(GetDlgItem(hDlg, IDC_DUMPWITHHEAP), FALSE);
                SetTimer(hDlg, 1, 500, NULL);
            } else if (g_disableIgnore) {
                // JE: Without this, any Exception *after* an assert will have the Ignore option,
                //     which isn't what we want!  Disable it all the time on exceptions.
                EnableWindow(GetDlgItem(hDlg, IDC_IGNORE), FALSE);
            }
            firsttime = FALSE;

            // disable some buttons if we aren't in development mode
            if (!(g_assertmode & ASSERTMODE_DEBUGBUTTONS))
            {
                ShowWindow(GetDlgItem(hDlg, IDC_DEBUG), SW_HIDE);
                ShowWindow(GetDlgItem(hDlg, IDC_IGNORE), SW_HIDE);
                ShowWindow(GetDlgItem(hDlg, IDC_RUNDEBUGGER), SW_HIDE);
            }

            // Polling for network activity
            SetTimer(hDlg, 2, 100, NULL);
            ShowCursor(TRUE);

            // send email
            if ( g_sendEmailonAssert )
                sendAssertEmail();
        
            return FALSE;
        }

    case WM_TIMER:
        if (wParam==1) {
            // Enable buttons timer
            EnableWindow(GetDlgItem(hDlg, IDC_STOP), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_DEBUG), TRUE);
            if (!g_disableIgnore) EnableWindow(GetDlgItem(hDlg, IDC_IGNORE), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_RUNDEBUGGER), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_COPYTOCLIPBOARD), TRUE);
            EnableWindow(GetDlgItem(hDlg, IDC_DUMPWITHHEAP), TRUE);
            SetFocus(GetDlgItem(hDlg, IDC_DEBUG));
            KillTimer(hDlg, 1);
        } else if (wParam==2) {
            // Poll for network here
            if (g_listenThreadResponse) {
                int response = g_listenThreadResponse-1;
                g_listenThreadResponse = 0; // Clear it so they can send another response
                Sleep(500); // Wait for the background thread to exit if necessary (otherwise a second assert dialog will not start up the thread)
                SendMessageA(hDlg, WM_COMMAND, MAKEWPARAM(response, 1), (LPARAM)NULL);
            }
        }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_RUNDEBUGGER)
        {
            PROCESS_INFORMATION pi;
            STARTUPINFOA si;
            CHAR buf[1000];
            int i;
            char debuggerpaths[][MAX_PATH] = 
            {
                // VS 2005 remote debuggers
#ifdef _M_X64
                "\"C:/Program Files/Microsoft Visual Studio 8/Common7/IDE/Remote Debugger/x64/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"C:/Program Files (x86)/Microsoft Visual Studio 8/Common7/IDE/Remote Debugger/x64/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"C:/game/tools/util/remote debugger/x64/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"./remote debugger/x64/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"C:/City of Heroes/bin/remote debugger/x64/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"D:/City of Heroes/bin/remote debugger/x64/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"C:/City of Heroes/src/util/remote debugger/x64/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"D:/City of Heroes/src/util/remote debugger/x64/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
#else
                // VS 2005 remote debuggers
                "\"C:/Program Files/Microsoft Visual Studio 8/Common7/IDE/Remote Debugger/x86/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"C:/Program Files (x86)/Microsoft Visual Studio 8/Common7/IDE/Remote Debugger/x86/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"C:/game/tools/util/remote debugger/x86/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"./remote debugger/x86/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"C:/City of Heroes/bin/remote debugger/x86/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"D:/City of Heroes/bin/remote debugger/x86/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"C:/City of Heroes/src/util/remote debugger/x86/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"D:/City of Heroes/src/util/remote debugger/x86/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"C:/game/tools/util/remote debugger/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"./remote debugger/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"C:/City of Heroes/src/util/remote debugger/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
                "\"D:/City of Heroes/src/util/remote debugger/msvsmon.exe\" /anyuser /noauth /nosecuritywarn /nowowwarn",
#endif
            };
            bool success=false;


            for (i=0; i<ARRAY_SIZE(debuggerpaths); i++)
            {
                memset(&si, 0, sizeof(si));
                si.cb = sizeof(si);
                memset(&pi, 0, sizeof(pi));
                if (CreateProcessA(NULL, debuggerpaths[i], NULL, NULL, 0, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
                    success = true;
                    break;
                }
            }
            if (!success)
            {
                FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, buf, 2000, NULL);
                MessageBoxA(hDlg, buf, "Error running debugger", MB_SETFOREGROUND);
            }
        }
        else if (LOWORD(wParam) == IDC_COPYTOCLIPBOARD)
        {
            winCopyToClipboard(assertbuf);
        }
        else if (LOWORD(wParam) == IDC_DUMPWITHHEAP || LOWORD(wParam) == IDC_STOP || LOWORD(wParam) == IDC_DEBUG || LOWORD(wParam) == IDC_IGNORE) 
        {
            ShowCursor(FALSE);
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    }
    return FALSE;
}
//////////////////////////////////////////// from assertdlg

// Dummy exception handler so that Assert can still get the assert message and pass it to CrashRpt
//int superassertExcept(unsigned int code, PEXCEPTION_POINTERS info)
//{
//    int ret = CallReportFault(info, assertbuf);
//    return ret;
//}

int __cdecl superassertf(const char* expr, const char* errormsg_fmt, const char* filename, unsigned lineno, ...)
{

    /*
    * Build the assertion message, then write it out. The exact form
    * depends on whether it is to be written out via stderr or the
    * MessageBox API.
    */


    char errormsg[1000] = "";
    va_list ap;

    va_start(ap, lineno);
    if (errormsg_fmt)
    {
        vsprintf(errormsg, errormsg_fmt, ap);
    }
    va_end(ap);

    return superassert(expr, errormsg, filename, lineno);
}

/***
*_assert() - Display a message and abort
*
*Purpose:
*       The assert macro calls this routine if the assert expression is
*       true.  By placing the assert code in a subroutine instead of within
*       the body of the macro, programs that call assert multiple times will
*       save space.
*
*Entry:
*
*Exit:    Returns 1 if we want an exception to be thrown
*
*Exceptions:
*
*******************************************************************************/

int __cdecl superassert(const char* expr, const char* errormsg, const char* filename, unsigned lineno)
{

    /*
    * Build the assertion message, then write it out. The exact form
    * depends on whether it is to be written out via stderr or the
    * MessageBox API.
    */


    int nCode;
    char * pch;
    char stackdump[ASSERT_BUF_SIZE] = "";
    char progname[MAX_PATH + 1] = "";
    char curtime[100] = "";
    char processID[100] = "";
    char cWindowsErrorMessage[1000];
    DWORD dLastError = GetLastError();
    U32 local_ip;
    char ipaddr[100] = "";

    g_isAsserting = 1;

    if ( dLastError )
    {
        if (!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dLastError, 0, cWindowsErrorMessage, 1000, NULL))
            sprintf_s(SAFESTR(cWindowsErrorMessage), "Error code: %d\n", dLastError);
    }

    
    autoTimerDisableRecursion(1);
    
    #if !USE_NEW_TIMING_CODE
        timerFlushFileBuffers();
    #endif
    
    generateVirtualMemoryLayout();

//    ShowWindow(compatibleGetConsoleWindow(), SW_SHOW);

    memlog_printf(0, "Assertion occured: %s", errormsg);

    /*
    * Line 1: box intro line
    */
    Strncpyt( assertbuf, BOXINTRO );
    Strncatt( assertbuf, dblnewline );

    /*
    * Line 2: program line
    */
    Strncatt( assertbuf, PROGINTRO );

    progname[MAX_PATH] = '\0';
    if ( !GetModuleFileNameA( NULL, progname, MAX_PATH ))
        strcpy( progname, "<program name unknown>");

    pch = (char *)progname;

    /* sizeof(PROGINTRO) includes the NULL terminator */
    if ( sizeof(PROGINTRO) + strlen(progname) + NEWLINESZ > MAXLINELEN )
    {
        size_t offset = (sizeof(PROGINTRO) + strlen(progname) + NEWLINESZ) - MAXLINELEN;
        size_t bufsize = ARRAY_SIZE_CHECKED(progname) - offset;
        pch += offset;
        strncpy_s( pch, bufsize, dotdotdot, DOTDOTDOTSZ );
    }

    local_ip = getHostLocalIp();
    sprintf_s(SAFESTR(ipaddr), "IP addr: %s\n", makeIpStr(local_ip));

    Strncatt( assertbuf, pch );
    Strncatt( assertbuf, newline );
    Strncatt( assertbuf, ipaddr );

    if(strlen(versionbuf))
    {
        Strncatt( assertbuf, VERINTRO );
        Strncatt( assertbuf, versionbuf );
        Strncatt( assertbuf, newline );
    }

    /*
    * Line 2.5: svn revision
    */
    Strncatt( assertbuf, "SVN Revision: " );
    Strncatt( assertbuf, build_version );
    Strncatt( assertbuf, newline );

    /*
    * Line 3: time
    */
    
    sprintf_s(SAFESTR(curtime), "%s%s\n", TIMEINTRO, TimeStr() );
    Strncatt( assertbuf, curtime );

    



    /*
    * Line 5: process ID
    */

    sprintf_s(SAFESTR(processID), "%s%d\n", PROCESSID, _getpid() );
    Strncatt( assertbuf, processID );

    /*
    * Line 6: file line
    */
    Strncatt( assertbuf, FILEINTRO );

    /* sizeof(FILEINTRO) includes the NULL terminator */
    if ( sizeof(FILEINTRO) + strlen(filename) + NEWLINESZ > MAXLINELEN )
    {
        size_t p, len, ffn;

        pch = (char *) filename;
        ffn = MAXLINELEN - sizeof(FILEINTRO) - NEWLINESZ;

        for ( len = strlen(filename), p = 1;
            pch[len - p] != '\\' && pch[len - p] != '/' && p < len;
            p++ );

        /* keeping pathname almost 2/3rd of full filename and rest
        * is filename
        */
        if ( (ffn - ffn/3) < (len - p) && ffn/3 > p )
        {
            /* too long. using first part of path and the
            filename string */
            strncat( assertbuf, pch, ffn - DOTDOTDOTSZ - p );
            Strncatt( assertbuf, dotdotdot );
            Strncatt( assertbuf, pch + len - p );
        }
        else if ( ffn - ffn/3 > len - p )
        {
            /* pathname is smaller. keeping full pathname and putting
            * dotdotdot in the middle of filename
            */
            p = p/2;
            strncat( assertbuf, pch, ffn - DOTDOTDOTSZ - p );
            Strncatt( assertbuf, dotdotdot );
            Strncatt( assertbuf, pch + len - p );
        }
        else
        {
            /* both are long. using first part of path. using first and
            * last part of filename.
            */
            strncat( assertbuf, pch, ffn - ffn/3 - DOTDOTDOTSZ );
            Strncatt( assertbuf, dotdotdot );
            strncat( assertbuf, pch + len - p, ffn/6 - 1 );
            Strncatt( assertbuf, dotdotdot );
            Strncatt( assertbuf, pch + len - (ffn/3 - ffn/6 - 2) );
        }

    }
    else
        /* plenty of room on the line, just append the filename */
        Strncatt( assertbuf, filename );

    Strncatt( assertbuf, newline );

    /*
    * Line 7: line line
    */
    Strncatt( assertbuf, LINEINTRO );
    _itoa_s( lineno, assertbuf + strlen(assertbuf), ARRAY_SIZE_CHECKED(assertbuf) - strlen(assertbuf), 10 );
    Strncatt( assertbuf, newline );




    /*
    * Line 8: message line
    */
    Strncatt( assertbuf, EXPRINTRO );

    if (strlen(assertbuf) + strlen(expr) + 2 * DBLNEWLINESZ > ASSERT_BUF_SIZE )
    {
        strncat( assertbuf, expr,
            ASSERT_BUF_SIZE -
            (strlen(assertbuf) +
            DOTDOTDOTSZ +
            2*DBLNEWLINESZ));
        Strncatt( assertbuf, dotdotdot );
    }
    else
        Strncatt( assertbuf, expr );

    Strncatt( assertbuf, dblnewline );

    
    if(errormsg){
        Strncatt(assertbuf, MSGINTRO);
        Strncatt(assertbuf, errormsg);
        Strncatt(assertbuf, dblnewline);
    }

    /*
     * Print the stack
     */

    sdDumpStackToBuffer(stackdump, ASSERT_BUF_SIZE, NULL);
    Strncatt(assertbuf, stackdump);

    // Extra info
    addExtraInfo(assertbuf, ARRAY_SIZE(assertbuf));

    /*
    * Line 4: windows error?
    */
    if ( dLastError )
    {
        Strncatt(assertbuf, "Last windows SYSTEM error: ");
        Strncatt(assertbuf, cWindowsErrorMessage);
    }
    else
    {
        Strncatt(assertbuf, "No windows SYSTEM error code found.\n");
    }

    // Throw an exception (used by TestClients)
    if (g_assertmode & ASSERTMODE_THROWEXCEPTION)
    {
        //int ImGonnaCrash = 1 / g_assertzero;
        assertbuf_filled = 1;
        autoTimerDisableRecursion(0);
        g_isAsserting = 0;
        return 1;
    }

    // Send error report only in production mode.
    if (g_assertmode & ASSERTMODE_ERRORREPORT)
    {
        assertbuf_filled = 1;
        autoTimerDisableRecursion(0);
        g_isAsserting = 0;
        return 1;
    }

    listenThreadBegin(); // Needs to be before assertCallback

    if (g_assertmode & ASSERTMODE_STDERR)
    {
        fflush(fileWrap(stdout));
        fputs(assertbuf, stderr);
        fflush(fileWrap(stderr));
    }

    if (g_assertmode & ASSERTMODE_CALLBACK && assertCallback)
    {
        assertCallback(assertbuf);
    }

    sharedHeapEmergencyMutexRelease();

    // Write dumps *after* starting the listenThread, so that the DbServer is notified immediately upon MapServer crash
    if (g_assertmode & ASSERTMODE_FULLDUMP) {        
        minidump_flags = MiniDumpWithProcessThreadData;
        assertWriteFullDumpSimple(NULL);
    }

    if (g_assertmode & ASSERTMODE_MINIDUMP) {
        minidump_flags = minidump_assert_flags;        
        assertWriteMiniDumpSimple(NULL);
    }

    if (g_assertmode & ASSERTMODE_EXIT) {
        nCode = IDABORT;
    } else {
        // Freeze all threads at this point!
        //assertFreezeAllOtherThreads(0);
        if (IsUsingCider())
        {
            MessageBoxA(NULL, assertbuf, "Assertion Failure",
                MB_OK | MB_ICONEXCLAMATION);
            nCode = IDABORT;
        }
        else
        {
            g_disableIgnore = (g_assertmode & ASSERTMODE_NOIGNOREBUTTONS)?1:0;
            nCode = DialogBoxIndirect(winGetHInstance(), (LPDLGTEMPLATE)DialogResource, NULL, Assert);

            if (nCode == IDC_STOP)
            {
                nCode = IDABORT;
            }
            else if (nCode == IDC_DEBUG)
            {
                nCode = IDRETRY;
            }
            else if (nCode == IDC_IGNORE)
            {
                nCode = IDIGNORE;
            }
            else if (nCode == IDC_DUMPWITHHEAP)
            {
                minidump_flags = MiniDumpWithProcessThreadData;
                assertWriteFullDumpSimple(NULL);
                nCode = IDABORT;
            }
            else if (nCode == -1)
            {
                nCode = IDABORT; // Dialog box failed to initialize
            }
            else
            {
                nCode = IDABORT;
            }
        }
        // Resume them so that the log writer can finish writing and/or the debugger can continue if ignored
        //assertFreezeAllOtherThreads(1);
    }

    /*
     * Write out via MessageBox
     */
    //nCode = __crtMessageBoxA(assertbuf,
    //    "Microsoft Visual C++ Runtime Library",
    //    MB_ABORTRETRYIGNORE|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL);

    /* Abort: abort the program */
    if (nCode == IDABORT)
    {
        logShutdownAndWait();
        /* raise abort signal */
        raise(SIGABRT);

        /* We usually won't get here, but it's possible that
        SIGABRT was ignored.  So exit the program anyway. */

        _exit(3);
    }

    /* Retry: call the debugger */
    if (nCode == IDRETRY)
    {
        g_ignoreexceptions = 1;    // dumb hack to make us fall into OS faster
        _DbgBreak();
        /* return to user code */
        autoTimerDisableRecursion(0);
        g_isAsserting = 0;
        return 0;
    }

    /* Ignore: continue execution */
    if (nCode == IDIGNORE)
    {
        autoTimerDisableRecursion(0);
        g_isAsserting = 0;
        return 0;
    }

    abort();
    autoTimerDisableRecursion(0);
    g_isAsserting = 0;
    return 0;
}

static int AdvancePastDivideInstruction(CONTEXT* co)
{
#pragma warning (push)
#pragma warning (disable: 4312) // warning about 64-bit, but this code is only defined for x86 anyway
#if defined(_X86_)
    LPBYTE ip = (LPBYTE)co->Eip;
    BYTE advance = 2; // initial bytecode + mod R/M byte
    BYTE mod, rm, base;

    // we only recognize DIV/IDIV initial byte:
    if (ip[0] != 0xF6 && ip[0] != 0xF7)
        return 0;
    
    // break down mod R/M & SIB bytes to get length
    mod = (ip[1] & 0xC0) >> 6;
    rm = (ip[1] & 0x07);
    base = (ip[2] & 0x07); 
    switch (mod)
    {
    case 3:    break;    // referring to register
    case 2: 
        advance += 4;    // 32-bit displacement
        if (rm == 4)
            advance += 1; // SIB byte
        break;
    case 1:
        advance += 1;    // 8-bit displacement
        if (rm == 4)
            advance += 1; // SIB byte
        break;
    case 0:
        if (rm == 5)
            advance += 4; // 32-bit displacement
        else if (rm == 4)
        {
            advance += 1; // SIB byte
            if (base == 5)
                advance += 4; // 32-bit displacement
        }
        break;
    default:
        return 0;
    }

    // just going to set the result to INT_MAX in the interest
    // of causing a detectable error
    co->Eax = 0x7fffffff;

    co->Eip += advance;
    return 1;

#else // !_X86_
    return 0;
#endif
#pragma warning (pop)
}

// handle a structure exception, we do the same thing as
// for an assert, except that the message is built differently
int assertExcept(unsigned int code, PEXCEPTION_POINTERS info)
{
    int nCode;
    static int handlingAssertion = 0;
    int recursiveCall = 0;    // is this copy a recursive child?
    DWORD dLastError = GetLastError();
    char cWindowsErrorMessage[1000];

    g_isAsserting = 1;

    if ( dLastError )
    {
        if (!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dLastError, 0, cWindowsErrorMessage, 1000, NULL))
            sprintf_s(SAFESTR(cWindowsErrorMessage), "Error code: %d\n", dLastError);
    }


    autoTimerDisableRecursion(1);

    assertFreezeAllOtherThreads(0);

    #if !USE_NEW_TIMING_CODE
        timerFlushFileBuffers();
    #endif

    generateVirtualMemoryLayout();

    writeConsole(OUTPUT_ERROR, "Program crash detected");

    // MAK - why do hardware engineers feel that div-zero is so important?

    if (g_ignoredivzero && code == EXCEPTION_INT_DIVIDE_BY_ZERO)
    {
        if (AdvancePastDivideInstruction(info->ContextRecord))
        {
            assertFreezeAllOtherThreads(1);
            autoTimerDisableRecursion(0);
            g_isAsserting = 0;
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        // otherwise, we couldn't recognize the instruction.. fall through
    }

    if (programIsShuttingDown())
    {
        assertFreezeAllOtherThreads(1);
        autoTimerDisableRecursion(0);
        g_isAsserting = 0;
        return IsDebuggerPresent()?EXCEPTION_CONTINUE_SEARCH:EXCEPTION_EXECUTE_HANDLER;
    }

    // detect recursion
    if (handlingAssertion) recursiveCall = 1;
    handlingAssertion = 1;

    // if we got called by assert, just fall through instead of popping again
    if (g_ignoreexceptions)
    {
        g_ignoreexceptions = 0;
        handlingAssertion = 0;
        assertFreezeAllOtherThreads(1);
        autoTimerDisableRecursion(0);
        g_isAsserting = 0;
        return EXCEPTION_CONTINUE_SEARCH; // fall through
    }

    if (!assertbuf_filled) {
        fillAssertBuffer(code, info, dLastError, cWindowsErrorMessage);
    } else {
        assertbuf_filled = 0;
    }

    g_disableIgnore = 1;

    // Send error report only in production mode.
    if (g_assertmode & ASSERTMODE_ERRORREPORT)
    {
        int ret;
        if (g_assertmode & ASSERTMODE_CALLBACK && assertCallback)
        {
            assertCallback(assertbuf);
        }

        if (IsUsingCider())
        {
            MessageBoxA(NULL, assertbuf, "Assertion Failure",
                MB_OK | MB_ICONEXCLAMATION);
            return EXCEPTION_EXECUTE_HANDLER;
        }

        ret = CallReportFault(info, staticAuthName, staticEntityName, staticShardName, staticShardTime, assertbuf, staticOpenGLReportFileName, staticLauncherLogFileName);
        handlingAssertion = 0;
        assertFreezeAllOtherThreads(1);
        autoTimerDisableRecursion(0);
        g_isAsserting = 0;
        return IsDebuggerPresent()?EXCEPTION_CONTINUE_SEARCH:ret;
    }

    // otherwise, do assert dialog
    listenThreadBegin();    // Start listening for a RemoteDebuggerer so that
                            // it will work while waiting in the error report window.

    if (g_assertmode & ASSERTMODE_STDERR)
    {
        fflush(fileWrap(stdout));
        fputs(assertbuf, stderr);
        fflush(fileWrap(stderr));
    }

    if (g_assertmode & ASSERTMODE_CALLBACK && assertCallback)
    {
        assertCallback(assertbuf);
    }

    sharedHeapEmergencyMutexRelease();


    if (g_assertmode & ASSERTMODE_FULLDUMP) 
    {
        minidump_flags = MiniDumpWithProcessThreadData;
        assertWriteFullDumpSimple(info);        
    }

    if (g_assertmode & ASSERTMODE_MINIDUMP && !recursiveCall)    // we may have crashed in dbghelp.dll, don't make minidump if recursing
    {
        minidump_flags = minidump_base_flags;
        assertWriteMiniDumpSimple(info);
    }

    if (g_assertmode & ASSERTMODE_EXIT) {
        assertFreezeAllOtherThreads(1);
        logShutdownAndWait();
        raise(SIGABRT);
        _exit(3);
    }

    // Freeze all threads at this point!
    // Done above: assertFreezeAllOtherThreads(0);

    if (IsUsingCider())
    {
        MessageBoxA(NULL, assertbuf, "Assertion Failure",
            MB_OK | MB_ICONEXCLAMATION);
        nCode = EXCEPTION_EXECUTE_HANDLER;
    }
    else
    {
        nCode = DialogBoxIndirect(winGetHInstance(), (LPDLGTEMPLATE)DialogResource, NULL, Assert);
        if (nCode == IDC_STOP)
        {
            nCode = EXCEPTION_EXECUTE_HANDLER;
        }
        else if (nCode == IDC_DEBUG)
        {
            nCode = EXCEPTION_CONTINUE_SEARCH;
        }
        else if (nCode == IDC_DUMPWITHHEAP)
        {
            minidump_flags = MiniDumpWithProcessThreadData;
            assertWriteFullDumpSimple(info);
            nCode = EXCEPTION_EXECUTE_HANDLER;
        }
        else if (nCode == -1)
        {
            nCode = EXCEPTION_EXECUTE_HANDLER; // Dialog box failed to initialize
        }
        else
        {
            nCode = EXCEPTION_CONTINUE_SEARCH;
        }
    }

    // Resume them so that the log writer can finish writing and/or the debugger can continue if ignored
    assertFreezeAllOtherThreads(1);

    handlingAssertion = 0;
    autoTimerDisableRecursion(0);
    g_isAsserting = 0;
    return nCode;
}

typedef BOOL (__stdcall *MiniDumpWriter)(
    HANDLE hProcess,
    DWORD ProcessId,
    HANDLE hFile,
    MINIDUMP_TYPE DumpType,
    PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    PMINIDUMP_CALLBACK_INFORMATION CallbackParam
    );

typedef struct DumpThreadParams
{
    char*                    filename;
    DWORD                    threadId;
    PEXCEPTION_POINTERS        exceptInfo;
    BOOL                    error;
} DumpThreadParams;

static MiniDumpWriter pMiniDumpWriteDump = NULL;
unsigned WINAPI MiniDumpThread(void* pData);

void assertWriteMiniDump(char* filename, PEXCEPTION_POINTERS info)
{
    HMODULE debugDll;
    HANDLE hThread;
    unsigned threadId;
    DumpThreadParams params;

    // Try to load the debug help dll or imagehlp.dll
    debugDll = LoadLibraryA( "dbghelp.dll" );
    if(!debugDll)
    {
        debugDll = LoadLibraryA( "imagehlp.dll" );
        
        if(!debugDll)
        {
            return;
        }
    }
    pMiniDumpWriteDump = (MiniDumpWriter) GetProcAddress(debugDll, "MiniDumpWriteDump");
    if(!pMiniDumpWriteDump)
    {
        FreeLibrary(debugDll);
        return;
    }

    // Use the other thread to write with (some version of MiniDumpWriteDump need to pause this thread)
    params.exceptInfo = info;
    params.filename = filename;
    params.threadId = GetCurrentThreadId();
    hThread = (HANDLE)_beginthreadex(NULL, 0, MiniDumpThread, &params, 0, &threadId);
    if (hThread != NULL)
    {
        WaitForSingleObject(hThread, 60000);
        CloseHandle(hThread);
    }

    pMiniDumpWriteDump = NULL;
    FreeLibrary(debugDll);
}

unsigned WINAPI MiniDumpThread(void* pData)
{
    HANDLE hFile;
    DumpThreadParams* pParams = (DumpThreadParams*)pData;

    pParams->error = 0;

    // Create the file first.
    hFile = CreateFileA(pParams->filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE != hFile)
    {
        MINIDUMP_EXCEPTION_INFORMATION mdei;

        mdei.ThreadId = pParams->threadId;
        mdei.ExceptionPointers = pParams->exceptInfo;
        mdei.ClientPointers = TRUE;

        pParams->error = !pMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, 
            minidump_flags,
            pParams->exceptInfo? &mdei: NULL, NULL, NULL);
        if (pParams->error) // try again with simpler options
            pParams->error = !pMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, 
                minidump_old_flags,
                pParams->exceptInfo? &mdei: NULL, NULL, NULL);

        CloseHandle(hFile);
    }
    else
    {
        // Could not open the file!
        pParams->error = 1;
    }
    return !pParams->error;
}


/*void assertWriteFullDump(char* filename, PEXCEPTION_POINTERS info)
{
    extern char *findUserDump(void);
    char CommandLine[200];
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    BOOL ret;

    sprintf(
        CommandLine,
        "%s %u*%u \"%s\"",
        findUserDump(),
        GetCurrentThreadId(),
        info,
        filename
        );

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    ret = CreateProcess(NULL,CommandLine,
        NULL, NULL, 0, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

    if (!ret) {
        printf("Failed to spawn %s to create full user dump, switching to minidump mode...\n", CommandLine);
        g_assertmode &= ~ASSERTMODE_FULLDUMP;
        g_assertmode |= ASSERTMODE_MINIDUMP;
    } else {
        WaitForSingleObject( pi.hProcess, 60000 );
        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );
    }
}*/

void assertWriteFullDumpSimple(PEXCEPTION_POINTERS info)
{
    static char moduleFilename[MAX_PATH+5]; // This has to be static, we pass it to another thread!
    char *s,datestr[1000];
    static int recursive_call=0;

    if (recursive_call)
        return;
    recursive_call = 1;

    minidump_flags |= MiniDumpWithFullMemory;

    GetModuleFileNameA(NULL, moduleFilename, MAX_PATH);

    if (getAssertMode() & ASSERTMODE_DATEDMINIDUMPS) {
        timerMakeDateStringFromSecondsSince2000(datestr,timerSecondsSince2000());
        for(s=datestr;*s;s++)
        {
            if (*s == ':' || *s==' ')
                *s = '_';
        }
        Strncatt(moduleFilename, ".");
        Strncatt(moduleFilename, datestr);
    }

    Strncatt(moduleFilename, ".dmp");

    if(!info)
    {
        __try 
        {
            // Raise dummy exception to get proper exception information.
            //RaiseException(0, 0, 0, 0);
            *((int*)0x00) = 1;
        }
        __except(assertWriteMiniDump(moduleFilename, GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
        {
            // Pass through to the zipper
        }
    }
    else
        assertWriteMiniDump(moduleFilename, info);
    // Zip the dump
    if (g_assertmode & ASSERTMODE_ZIPPED) {
        // Use gzip, since the file can be over 2GB.
        fileGZip(moduleFilename);
    }

    recursive_call = 0;
}


void assertWriteFullDumpSimpleSetFlags(MINIDUMP_TYPE flags)
{
    minidump_flags = flags;
    assertWriteFullDumpSimple(NULL);
}


void assertWriteMiniDumpSimple(PEXCEPTION_POINTERS info)
{
    static char moduleFilename[MAX_PATH]; // This has to be static, we pass it to another thread!
    char *s,datestr[1000];
    static int recursive_call=0;

    if (recursive_call)
        return;
    recursive_call = 1;

    minidump_flags |= MiniDumpNormal;

    GetModuleFileNameA(NULL, moduleFilename, MAX_PATH);

    if (getAssertMode() & ASSERTMODE_DATEDMINIDUMPS) {
        timerMakeDateStringFromSecondsSince2000(datestr,timerSecondsSince2000());
        for(s=datestr;*s;s++)
        {
            if (*s == ':' || *s==' ')
                *s = '_';
        }
        Strncatt(moduleFilename, ".");
        Strncatt(moduleFilename, datestr);
    }

    Strncatt(moduleFilename, ".mdmp");            

    if(!info)
    {
        __try 
        {
            // Raise dummy exception to get proper exception information.
            //RaiseException(0, 0, 0, 0);
            *((int*)0x00) = 1;
        }
        __except(assertWriteMiniDump(moduleFilename, GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
        {
            // Pass through to the zipper
        }
    }
    else
        assertWriteMiniDump(moduleFilename, info);
    // Zip the minidump
    if (g_assertmode & ASSERTMODE_ZIPPED) {
        fileZip(moduleFilename);
    }

    recursive_call = 0;
}

int assertSubmitErrorReports()
{
    int crashRptExists = 0;
    HMODULE dll = loadCrashRptDll();
    crashRptExists = (dll ? 1 : 0);
    if(dll)
        FreeLibrary(dll);

    if ((g_assertmode & ASSERTMODE_ERRORREPORT) && crashRptExists)
        return 1;
    else
        return 0;
}

#ifndef FINAL
int assertIsDevelopmentMode(void)
{
    return isDevelopmentMode();
}
#endif

void AssertErrorfFL( char * file, int line )
{
    ErrorfFL(file, line);
}

void AssertErrorf(const char * file, int line, char const *fmt, ...)
{
    va_list ap;

    ErrorfFL(file, line);

    va_start(ap, fmt);
    ErrorvInternal(fmt, ap);
    va_end(ap);
}

void AssertErrorFilenamef(const char * file, int line, const char *filename, char const *fmt, ...)
{
    va_list ap;

    ErrorfFL(file, line);

    va_start(ap, fmt);
    ErrorFilenamevInternal(filename, fmt, ap);
    va_end(ap);
}
#else // DISABLE_ASSERTIONS is defined
int reportException(unsigned int code, PEXCEPTION_POINTERS info)
{
    DWORD dLastError = GetLastError();
    char cWindowsErrorMessage[1000];
    int ret;

    if ( dLastError )
    {
        if (!FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dLastError, 0, cWindowsErrorMessage, 1000, NULL))
            sprintf_s(SAFESTR(cWindowsErrorMessage), "Error code: %d\n", dLastError);
    }

    autoTimerDisableRecursion(1);
    EnterCriticalSection(&freeze_threads_section_two);

    if (IsUsingCider())
    {
        MessageBoxA(NULL, assertbuf, "Assertion Failure", MB_OK | MB_ICONEXCLAMATION);
        return EXCEPTION_EXECUTE_HANDLER;
    }

    fillAssertBuffer(code, info, dLastError, cWindowsErrorMessage);
    ret = CallReportFault(info, staticAuthName, staticEntityName, staticShardName, staticShardTime, assertbuf, staticOpenGLReportFileName, staticLauncherLogFileName);

    LeaveCriticalSection(&freeze_threads_section_two);
    autoTimerDisableRecursion(0);

    return IsDebuggerPresent()?EXCEPTION_CONTINUE_SEARCH:ret;
}
#endif

#if defined(_DEBUG) && _MSC_VER < 1400
// Override the CRT abort function for other libraries
void abort(void)
{
    assertmsg(0, "CRT abort() called");
}
#endif
