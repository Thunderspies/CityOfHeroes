#ifndef SYSUTIL_H
#define SYSUTIL_H

#include "wininclude.h"

C_DECLARATIONS_BEGIN

typedef struct HWND__ *HWND;

char* getComputerName(void);
char* getExecutableName(void);
char* getExecutableVersion(int dots);
char* getExecutableVersionEx(char* executableName, int dots);
int versionCompare(char* version1, char* version2);
char *getExecutableDir(char *buf);
DWORDLONG getPhysicalMemory(DWORDLONG *max, DWORDLONG *avail); // Returns max when NULLs are passed in
unsigned long getProcessPageFileUsage();
void winCopyToClipboard(const char* s);
const char *winCopyFromClipboard(void); // Returns pointer to static buffer
void hideConsoleWindow(void);
void showConsoleWindow(void);
HWND compatibleGetConsoleWindow(void);
void disableRtlHeapChecking(void *heap);
void preloadDLLs(int silent);
#ifndef _XBOX
    HANDLE CreateFileMappingSafe( DWORD lpProtect, int size, const char* handleName, int silent);
    HANDLE OpenFileMappingSafe(DWORD dwDesiredAccess, bool bInheritHandle, const char* handleName, int silent );
    LPVOID MapViewOfFileExSafe(HANDLE handle, const char* handleName, void* desiredAddress, int silent);
#endif


void trickGoogleDesktopDll(int silent);


#define WAIT_FOR_DEBUGGER \
    {                                                                \
        int i;                                                        \
                                                                    \
        for (i = 1; i < argc; i++)                                    \
        {                                                            \
            if (strstr(argv[i], "WaitForDebugger"))                    \
            {                                                        \
                while( !IsDebuggerPresent())                        \
                {                                                    \
                    Sleep(1);                                        \
                }                                                    \
                Sleep(1);                                            \
            }                                                        \
        }                                                            \
    }                                                                

#define WAIT_FOR_DEBUGGER_LPCMDLINE                                    \
    {                                                                \
                                                                    \
            if (strstr(lpCmdLine, "WaitForDebugger"))                \
            {                                                        \
                while( !IsDebuggerPresent())                        \
                {                                                    \
                    Sleep(1);                                        \
                }                                                    \
                Sleep(1);                                            \
            }                                                        \
                                                                    \
    }    

C_DECLARATIONS_END

#endif
