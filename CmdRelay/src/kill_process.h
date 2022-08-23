#ifndef KILL_PROCESS_H
#define KILL_PROCESS_H

#include <windows.h>

#include <tlhelp32.h>


BOOL KillProcess(DWORD dwProcessId);
BOOL KillProcessEx(DWORD dwProcessId, BOOL bTree);


#endif // KILL_PROCESS_H
