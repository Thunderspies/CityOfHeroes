

#include "kill_process.h"

#include <tlhelp32.h>
#include <crtdbg.h>


// FROM MSDN LIBRARY SAMPLE CODE
BOOL GetProcessModule (DWORD dwPID, DWORD dwModuleID, 
                       LPMODULEENTRY32 lpMe32, DWORD cbMe32) 
{ 
    BOOL          bRet        = FALSE; 
    BOOL          bFound      = FALSE; 
    HANDLE        hModuleSnap = NULL; 
    MODULEENTRY32 me32        = {0}; 

    // Take a snapshot of all modules in the specified process. 

    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID); 
    if (hModuleSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 

    // Fill the size of the structure before using it. 

    me32.dwSize = sizeof(MODULEENTRY32); 

    // Walk the module list of the process, and find the module of 
    // interest. Then copy the information to the buffer pointed 
    // to by lpMe32 so that it can be returned to the caller. 

    if (Module32First(hModuleSnap, &me32)) 
    { 
        do 
        { 
            if (me32.th32ModuleID == dwModuleID) 
            { 
                CopyMemory (lpMe32, &me32, cbMe32); 
                bFound = TRUE; 
            } 
        } 
        while (!bFound && Module32Next(hModuleSnap, &me32)); 

        bRet = bFound;   // if this sets bRet to FALSE, dwModuleID 
        // no longer exists in specified process 
    } 
    else 
        bRet = FALSE;           // could not walk module list 

    // Do not forget to clean up the snapshot object. 

    CloseHandle (hModuleSnap); 

    return (bRet); 
}


// MODIFIED FROM MSDN LIBRARY SAMPLE CODE
BOOL GetChildProcess (DWORD parentProcessID, DWORD * outChildProcessID) 
{ 
    HANDLE         hProcessSnap = NULL; 
    BOOL           bRet      = FALSE; 
    PROCESSENTRY32 pe32      = {0}; 

    //  Take a snapshot of all processes in the system. 

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if (hProcessSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 

    //  Fill in the size of the structure before using it. 

    pe32.dwSize = sizeof(PROCESSENTRY32); 

    //  Walk the snapshot of the processes, and for each process, 
    //  display information. 

    if (Process32First(hProcessSnap, &pe32)) 
    { 
        //        DWORD         dwPriorityClass; 
        BOOL          bGotModule = FALSE; 
        MODULEENTRY32 me32       = {0}; 

        do 
        { 
            bGotModule = GetProcessModule(pe32.th32ProcessID, 
                pe32.th32ModuleID, &me32, sizeof(MODULEENTRY32)); 

            if (bGotModule) 
            { 
                if(pe32.th32ParentProcessID == parentProcessID)
                {
                    (*outChildProcessID) = pe32.th32ProcessID;
                    bRet = TRUE;
                }
            } 
        } 
        while (Process32Next(hProcessSnap, &pe32)); 
    } 
    else 
        bRet = FALSE;    // could not walk the list of processes 

    // Do not forget to clean up the snapshot object. 

    CloseHandle (hProcessSnap); 
    return (bRet); 
}

typedef struct SYSTEM_PROCESSES {
    ULONG            NextEntryDelta;
    ULONG            ThreadCount;
    ULONG            Reserved1[6];
    LARGE_INTEGER   CreateTime;
    LARGE_INTEGER   UserTime;
    LARGE_INTEGER   KernelTime;
    //    UNICODE_STRING  ProcessName;
    //    KPRIORITY        BasePriority;
    ULONG            ProcessId;
    ULONG            InheritedFromProcessId;
    ULONG            HandleCount;
    ULONG            Reserved2[2];
    //    VM_COUNTERS        VmCounters;
#if _WIN32_WINNT >= 0x500
    IO_COUNTERS        IoCounters;
#endif
    //    SYSTEM_THREADS  Threads[1];
} SYSTEM_PROCESSES, * PSYSTEM_PROCESSES;

typedef struct SYSTEM_PROCESS_INFORMATION {
    ULONG           NextEntryDelta;         // offset to the next entry
    ULONG           ThreadCount;            // number of threads
    ULONG           Reserved1[6];           // reserved
    LARGE_INTEGER   CreateTime;             // process creation time
    LARGE_INTEGER   UserTime;               // time spent in user mode
    LARGE_INTEGER   KernelTime;             // time spent in kernel mode
    //    UNICODE_STRING  ProcessName;            // process name
    //    KPRIORITY       BasePriority;           // base process priority
    ULONG           ProcessId;              // process identifier
    ULONG           InheritedFromProcessId; // parent process identifier
    ULONG           HandleCount;            // number of handles
    ULONG           Reserved2[2];           // reserved
    //    VM_COUNTERS     VmCounters;             // virtual memory counters
#if _WIN32_WINNT >= 0x500
    IO_COUNTERS     IoCounters;             // i/o counters
#endif
    //    SYSTEM_THREAD_INFORMATION Threads[1];   // threads
} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;



BOOL KillProcess(
                 IN DWORD dwProcessId
                 )
{
    // get process handle
    DWORD dwError = ERROR_SUCCESS;

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
    if (hProcess == NULL)
        return FALSE;



    // try to terminate the process
    if (!TerminateProcess(hProcess, (DWORD)-1))
        dwError = GetLastError();

    // close process handle
    CloseHandle(hProcess);

    SetLastError(dwError);
    return dwError == ERROR_SUCCESS;
}



// a helper function that walks a process tree recursively
// on Windows 9x and terminates all processes in the tree
static BOOL KillProcessTreeWinHelper(DWORD dwProcessId)
{
    HINSTANCE hKernel;
    HANDLE (WINAPI * pCreateToolhelp32Snapshot)(DWORD, DWORD);
    BOOL (WINAPI * pProcess32First)(HANDLE, PROCESSENTRY32 *);
    BOOL (WINAPI * pProcess32Next)(HANDLE, PROCESSENTRY32 *);
    HANDLE hSnapshot;
    PROCESSENTRY32 Entry;

    // get KERNEL32.DLL handle
    hKernel = GetModuleHandleA("kernel32.dll");
    _ASSERTE(hKernel != NULL);

    // find necessary entrypoints in KERNEL32.DLL
    *(FARPROC *)&pCreateToolhelp32Snapshot = GetProcAddress(hKernel, "CreateToolhelp32Snapshot");
    *(FARPROC *)&pProcess32First = GetProcAddress(hKernel, "Process32First");
    *(FARPROC *)&pProcess32Next = GetProcAddress(hKernel, "Process32Next");

    if (pCreateToolhelp32Snapshot == NULL || pProcess32First == NULL || pProcess32Next == NULL)
    {
        return ERROR_PROC_NOT_FOUND;
    }

    // create a snapshot of all processes
    hSnapshot = pCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }

    Entry.dwSize = sizeof(Entry);
    if (!pProcess32First(hSnapshot, &Entry))
    {
        DWORD dwError = GetLastError();
        CloseHandle(hSnapshot);
        return dwError;
    }

    // terminate children first
    do
    {
        if (Entry.th32ParentProcessID == dwProcessId)
            KillProcessTreeWinHelper(Entry.th32ProcessID);

        Entry.dwSize = sizeof(Entry);
    }
    while (pProcess32Next(hSnapshot, &Entry));

    CloseHandle(hSnapshot);

    // terminate the specified process
    if (!KillProcess(dwProcessId))
    {
        return GetLastError();
    }

    return ERROR_SUCCESS;
}

BOOL KillProcessEx(
                   IN DWORD dwProcessId,   // process identifier
                   IN BOOL bTree           // terminate tree flag
                   )
{
    DWORD dwError;

    if (!bTree)
        return KillProcess(dwProcessId);

    dwError = KillProcessTreeWinHelper(dwProcessId);

    SetLastError(dwError);
    return dwError == ERROR_SUCCESS;
}
