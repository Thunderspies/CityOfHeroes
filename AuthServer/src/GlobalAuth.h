#ifndef _GLOBALAUTH_H
#define _GLOBALAUTH_H
#pragma warning(disable: 4786)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601 // Windows 7 and up
//#define WIN32_LEAN_AND_MEAN 
#endif

#define CONFIG_FILENAME     "etc/config.txt"

#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <process.h>
#include <crtdbg.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <direct.h>
#include <sql.h>
#include <sqlext.h>

#include <vector>
#include <queue>
#include <map>
#include <string>
#include <set>
#include <strstream>
#include <fstream>
#include <cassert>

#include "XTREE"
#include "Exception.h"
#include "Protocol.h"
#include "Log.h"
#include "Memory.h"
#include "Reporter.h"
#include "Des.h"
#include "AuthType.h"
#include "Lock.h"


#ifdef DEBUG_MEMORY

#ifdef malloc
#undef malloc
#undef calloc
#undef realloc
#undef _expand
#undef free
#undef _msize
#endif

#define malloc(s)         _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define calloc(c, s)      _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define realloc(p, s)     _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _expand(p, s)     _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define free(p)           _free_dbg(p, _NORMAL_BLOCK)
#define _msize(p)         _msize_dbg(p, _NORMAL_BLOCK)

void *operator new(size_t size, LPCSTR fileName, int lineNumber);
void operator delete(void *ptr, LPCSTR fileName, int lineNumber);

#define new new(__FILE__, __LINE__)

#endif


#ifndef _IOCPHandle
#define  _IOCPHandle
extern HANDLE g_hIOCompletionPort;    // For Accept ( Used for World Server Connection )
extern HANDLE g_hIOCompletionPortInt; // For New Line Terminate Protocol ( Used for Interactive Socket )
extern HANDLE g_hSocketTimer;
extern long g_AcceptExThread;
extern long g_Accepts;
extern long g_nRunningThread;

#endif

extern bool globalTeminateEvent;

unsigned __stdcall TimerThread(void *);
#endif


// HISTORY
// 2003-02-20 by darkangel
// Add Config. parameter numServerThread, numServerExThread, numServerIntThread
// 2003-07-15 by darkangel
// Add CLogDSocket Class
