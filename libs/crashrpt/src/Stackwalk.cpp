#pragma warning(disable: 4127 4100)

#include "pch.h"

//#define _WIN32_WINNT 0x0400
//#include <windows.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

extern "C"{
#include "../include/crashrpt/Stackwalk.h"
// imagehlp.h must be compiled with packing to eight-byte-boundaries,
// but does nothing to enforce that.
#pragma pack( push, before_imagehlp, 8 )
#include <dbghelp.h>
#pragma pack( pop, before_imagehlp )


#define gle (GetLastError())
#define lenof(a) (sizeof(a) / sizeof((a)[0]))
#define MAXNAMELEN 1024 // max name length for found symbols
#ifdef _M_X64
#define IMGSYMLEN ( sizeof IMAGEHLP_SYMBOL64 )
#else
#define IMGSYMLEN ( sizeof IMAGEHLP_SYMBOL )
#endif
#define TTBUFLEN 65536 // for a temp buffer

#define WAIT_TOO_LONG 120000    // 2 minutes


// SymCleanup()
typedef BOOL (__stdcall *tSC)( IN HANDLE hProcess );
tSC pSC = NULL;

//! @note    According to the MSDN, SymFunctionTableAccess64(), and other 64-bit extended functions, can be used in 32-bit apps.
//!            At a later date, we can remove the old versions completely.
#ifdef _M_X64
// SymFunctionTableAccess64()
typedef PVOID (__stdcall *tSFTA)( HANDLE hProcess, DWORD64 AddrBase );
tSFTA pSFTA = NULL;

// SymGetLineFromAddr64()
typedef BOOL (__stdcall *tSGLFA)( IN HANDLE hProcess, IN DWORD64 dwAddr,
                                 OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE64 Line );
tSGLFA pSGLFA = NULL;

// SymGetModuleBase()
typedef DWORD64 (__stdcall *tSGMB)( IN HANDLE hProcess, IN DWORD64 dwAddr );
tSGMB pSGMB = NULL;

// SymGetModuleInfo()
typedef BOOL (__stdcall *tSGMI)( IN HANDLE hProcess, IN DWORD dwAddr, OUT PIMAGEHLP_MODULE64 ModuleInfo );
tSGMI pSGMI = NULL;
#else
// SymFunctionTableAccess()
typedef PVOID (__stdcall *tSFTA)( HANDLE hProcess, DWORD AddrBase );
tSFTA pSFTA = NULL;

// SymGetLineFromAddr()
typedef BOOL (__stdcall *tSGLFA)( IN HANDLE hProcess, IN DWORD dwAddr,
                                 OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE Line );
tSGLFA pSGLFA = NULL;

// SymGetModuleBase()
typedef DWORD (__stdcall *tSGMB)( IN HANDLE hProcess, IN DWORD dwAddr );
tSGMB pSGMB = NULL;

// SymGetModuleInfo()
typedef BOOL (__stdcall *tSGMI)( IN HANDLE hProcess, IN DWORD dwAddr, OUT PIMAGEHLP_MODULE ModuleInfo );
tSGMI pSGMI = NULL;
#endif

// SymGetOptions()
typedef DWORD (__stdcall *tSGO)( VOID );
tSGO pSGO = NULL;

#ifdef _M_X64
// SymGetSymFromAddr64()
typedef BOOL (__stdcall *tSGSFA)( IN HANDLE hProcess, IN DWORD64 dwAddr,
                                 OUT PDWORD64 pdwDisplacement, OUT PIMAGEHLP_SYMBOL64 Symbol );
#else
// SymGetSymFromAddr()
typedef BOOL (__stdcall *tSGSFA)( IN HANDLE hProcess, IN DWORD dwAddr,
                                 OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_SYMBOL Symbol );
#endif
tSGSFA pSGSFA = NULL;

// SymInitialize()
typedef BOOL (__stdcall *tSI)( IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess );
tSI pSI = NULL;

// SymLoadModule()
#ifdef _M_X64
typedef DWORD (__stdcall *tSLM)( IN HANDLE hProcess, IN HANDLE hFile,
                                IN PSTR ImageName, IN PSTR ModuleName, IN DWORD64 BaseOfDll, IN DWORD SizeOfDll );
#else
typedef DWORD (__stdcall *tSLM)( IN HANDLE hProcess, IN HANDLE hFile,
                                IN PSTR ImageName, IN PSTR ModuleName, IN DWORD BaseOfDll, IN DWORD SizeOfDll );
#endif
tSLM pSLM = NULL;

// SymSetOptions()
typedef DWORD (__stdcall *tSSO)( IN DWORD SymOptions );
tSSO pSSO = NULL;

#ifdef _M_X64
typedef BOOL (__stdcall *sSymbolIterator)(IN HANDLE hProcess, IN OUT PIMAGEHLP_SYMBOL64  Symbol);
#else
typedef BOOL (__stdcall *sSymbolIterator)(IN HANDLE hProcess, IN OUT PIMAGEHLP_SYMBOL  Symbol);
#endif

sSymbolIterator symGetNextSymbol = NULL;
sSymbolIterator symGetPrevSymbol = NULL;


// StackWalk()
#ifdef _M_X64
typedef BOOL (__stdcall *tSW)( DWORD MachineType, HANDLE hProcess,
                              HANDLE hThread, LPSTACKFRAME64 StackFrame, PVOID ContextRecord,
                              PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine,
                              PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine,
                              PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine,
                              PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress );
#else
typedef BOOL (__stdcall *tSW)( DWORD MachineType, HANDLE hProcess,
                              HANDLE hThread, LPSTACKFRAME StackFrame, PVOID ContextRecord,
                              PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryRoutine,
                              PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine,
                              PGET_MODULE_BASE_ROUTINE GetModuleBaseRoutine,
                              PTRANSLATE_ADDRESS_ROUTINE TranslateAddress );
#endif

tSW pSW = NULL;

// UnDecorateSymbolName()
typedef DWORD (__stdcall WINAPI *tUDSN)( PCSTR DecoratedName, PSTR UnDecoratedName,
                                        DWORD UndecoratedLength, DWORD Flags );
tUDSN pUDSN = NULL;

int swStartedThreadID = 0; // Flag for whether it's started and holds the specific thread ID

struct ModuleEntry
{
    std::string imageName;
    std::string moduleName;
    uintptr_t baseAddress;
    DWORD size;
};
typedef std::vector< ModuleEntry > ModuleList;
typedef ModuleList::iterator ModuleListIter;

int threadAbortFlag = 0;
HANDLE hTapTapTap = NULL;

void enumAndLoadModuleSymbols( HANDLE hProcess, DWORD pid );
bool fillModuleList( ModuleList& modules, DWORD pid, HANDLE hProcess );
bool fillModuleListTH32( ModuleList& modules, DWORD pid );
bool fillModuleListPSAPI( ModuleList& modules, DWORD pid, HANDLE hProcess );

HINSTANCE hDebugHelpDll = 0;    // The dll to get exe debug symbol lookup related functions.
HANDLE stackDumpThread = 0;        // The thread that will create the stack dump for all other threads.
static int g_dumpingStack = 0;    // just a global to prevent recursion

/***********************************************************************************************
* Worker thread stuff
*/
/* Function WorkerThreadProc()
*    This is the main function of the Image Extraction thread.  It is a worker thread
*    whose main purpose is to create missing images for a mirror patch server.  The
*    function itself does not do anything useful.  It simply asks the thread to goto
*    sleep until a new task has been given to it through QueueUserAPC().
*/
DWORD WINAPI StackDumpWorkerThreadProc(LPVOID useless);
DWORD WINAPI StackDumpWorkerThreadProc(LPVOID useless){
    while(1){
        SleepEx(INFINITE, 1);
    }
}

/* Function WorkerThreadExitProc()
*    Ask the worker thread to kill itself.
*/
void CALLBACK WorkerThreadExitProc(LPVOID useless);
void CALLBACK WorkerThreadExitProc(LPVOID useless){
    ExitThread(0);
}
/*
* Worker thread stuff
***********************************************************************************************/

// Checks if all functions required by stack dump has been acquired.
int swReady()
{
    if ( pSC == NULL || pSFTA == NULL || pSGMB == NULL || pSGMI == NULL ||
        pSGO == NULL || pSGSFA == NULL || pSI == NULL || pSSO == NULL ||
        pSW == NULL || pUDSN == NULL || pSLM == NULL )
    {
        return 0;
    }else
        return 1;
}

int swStartup()
{
    char *tt = 0;
    if(!swStartedThreadID)
    {
        // Original documentation:
        // We load imagehlp.dll dynamically because the NT4-version does not
        // offer all the functions that are in the NT5 lib.
        // JS: We also don't want to require debug dll's to be present for the executable to run.
        // JS: If the dll's are not present, trying to dump the stack would just do nothing.

        // Try to load the debug help dll first.
        //    dbghelp.dll is the new and approved way to get at debug info.  Its functionality also appears to
        //    be the superset of imagehlp.dll.
        //    
        hDebugHelpDll = LoadLibraryA("dbghelp.dll");

        // If that doesn't work, then try imagehlp.dll.
        if(hDebugHelpDll == NULL)
        {
            hDebugHelpDll = LoadLibraryA("imagehlp.dll");
            return 0;
        }

        // Grab the addresses of functions we are going to be using.
        // All of these functions are present in both dll's.
#ifdef _M_X64
        pSGLFA =    (tSGLFA)    GetProcAddress(hDebugHelpDll, "SymGetLineFromAddr64");
        pSGMB =        (tSGMB)        GetProcAddress(hDebugHelpDll, "SymGetModuleBase64");
        pSGSFA =    (tSGSFA)    GetProcAddress(hDebugHelpDll, "SymGetSymFromAddr64");
        pSW =        (tSW)        GetProcAddress(hDebugHelpDll, "StackWalk64");
        pSFTA =        (tSFTA)        GetProcAddress(hDebugHelpDll, "SymFunctionTableAccess64");
        pSLM =        (tSLM)        GetProcAddress(hDebugHelpDll, "SymLoadModule64");
#else
        pSGLFA =    (tSGLFA)    GetProcAddress(hDebugHelpDll, "SymGetLineFromAddr");
        pSGSFA =    (tSGSFA)    GetProcAddress(hDebugHelpDll, "SymGetSymFromAddr");
        pSW =        (tSW)        GetProcAddress(hDebugHelpDll, "StackWalk");
        pSGMB =        (tSGMB)        GetProcAddress(hDebugHelpDll, "SymGetModuleBase");
        pSFTA =        (tSFTA)        GetProcAddress(hDebugHelpDll, "SymFunctionTableAccess");
        pSLM =        (tSLM)        GetProcAddress(hDebugHelpDll, "SymLoadModule");
#endif
        pSC =        (tSC)        GetProcAddress(hDebugHelpDll, "SymCleanup");
        pSGMI =        (tSGMI)        GetProcAddress(hDebugHelpDll, "SymGetModuleInfo");
        pSGO =        (tSGO)        GetProcAddress(hDebugHelpDll, "SymGetOptions");
        pSI =        (tSI)        GetProcAddress(hDebugHelpDll, "SymInitialize");
        pSSO =        (tSSO)        GetProcAddress(hDebugHelpDll, "SymSetOptions");
        pUDSN =        (tUDSN)        GetProcAddress(hDebugHelpDll, "UnDecorateSymbolName");

        // Did we get all the functions we needed?
        if(!swReady())
            goto Cleanup;

        // Create a worker thread that will be used for all other threads to dump their stacks on request.
        DWORD threadID;
        stackDumpThread = CreateThread(0, 0, StackDumpWorkerThreadProc, NULL, 0, &threadID);
        swStartedThreadID = threadID;
    }

    {
        HANDLE hProcess = GetCurrentProcess();    // hProcess normally comes from outside
        DWORD symOptions;                        // symbol handler settings
        std::string symSearchPath;
        char* p;

        STACKFRAME s; // in/out stackframe
        memset( &s, '\0', sizeof s );


        // NOTE: normally, the exe directory and the current directory should be taken
        // from the target process. The current dir would be gotten through injection
        // of a remote thread; the exe fir through either ToolHelp32 or PSAPI.

        tt = new char[TTBUFLEN]; // this is a _sample_. you can do the error checking yourself.

        // build symbol search path from:
        symSearchPath = "";
        // current directory
        if ( GetCurrentDirectoryA( TTBUFLEN, tt ) )
            symSearchPath += tt + std::string( ";" );
        // dir with executable
        if ( GetModuleFileNameA( 0, tt, TTBUFLEN ) )
        {
            for ( p = tt + strlen( tt ) - 1; p >= tt; -- p )
            {
                // locate the rightmost path separator
                if ( *p == '\\' || *p == '/' || *p == ':' )
                    break;
            }
            // if we found one, p is pointing at it; if not, tt only contains
            // an exe name (no path), and p points before its first byte
            if ( p != tt ) // path sep found?
            {
                if ( *p == ':' ) // we leave colons in place
                    ++ p;
                *p = '\0'; // eliminate the exe name and last path sep
                symSearchPath += tt + std::string( ";" );
            }
        }
        // environment variable _NT_SYMBOL_PATH
        if ( GetEnvironmentVariableA( "_NT_SYMBOL_PATH", tt, TTBUFLEN ) )
            symSearchPath += tt + std::string( ";" );
        // environment variable _NT_ALTERNATE_SYMBOL_PATH
        if ( GetEnvironmentVariableA( "_NT_ALTERNATE_SYMBOL_PATH", tt, TTBUFLEN ) )
            symSearchPath += tt + std::string( ";" );
        // environment variable SYSTEMROOT
        if ( GetEnvironmentVariableA( "SYSTEMROOT", tt, TTBUFLEN ) )
            symSearchPath += tt + std::string( ";" );

        if ( symSearchPath.size() > 0 ) // if we added anything, we have a trailing semicolon
            symSearchPath = symSearchPath.substr( 0, symSearchPath.size() - 1 );

        //sdcWrite(&sdContext,  "symbols path: %s\n", symSearchPath.c_str() );

        // why oh why does SymInitialize() want a writeable string?
        strncpy_s( tt, TTBUFLEN, symSearchPath.c_str(), _TRUNCATE );

        // init symbol handler stuff (SymInitialize())
        if ( ! pSI( hProcess, tt, false ) )
        {
            goto Cleanup;
        }

        // SymGetOptions()
        symOptions = pSGO();
        // symOptions |= SYMOPT_DEBUG;            // Turn this on, to see what the symbol server is doing.
        symOptions |= SYMOPT_LOAD_LINES;        // Load line information
        symOptions &= ~SYMOPT_UNDNAME;            // Do not show function names in undecorated form.  Irrelevant in c either way.
        symOptions |= SYMOPT_DEFERRED_LOADS;    // Do not load symbol files until a symbol is referenced.
        pSSO( symOptions );                        // SymSetOptions()

        // Enumerate modules and tell imagehlp.dll about them.
        // FIXME!!!
        // JS:  This module symbol loading + unloading should not be done *everytime* the stack is dumped.
        enumAndLoadModuleSymbols( hProcess, GetCurrentProcessId() );
    }
    if(tt)
        delete[] tt;
    return swStartedThreadID;

Cleanup:
    if(tt)
        delete[] tt;
    // If not, then it'll be impossible to do a stackdump.
    swShutdown();
    return 0;
}

// FIXME!!!
//    The shutdown code is definitely not complete.
//    It needs to kill the worker thread correctly!
//  Otherwise, the worker thread will just crash while trying to use functions from the freed dll.
//    Simply terminating the thread here is not safe because the worker thread might be holding on to an
//  event object that a thread is waiting a dump on.  Maybe this case should just be ignored.  It should 
//  be the library users responsibilty to see to it that all dumps have been completed before shutting
//  the stackwalk module off.
//    Even then, it's probably a better if the program didn't just sit there waiting on an event that will
//  never be signaled forever.
void swShutdown()
{
    if(swStartedThreadID){
        TerminateThread(stackDumpThread, 0);
        FreeLibrary(hDebugHelpDll);
        swStartedThreadID = 0;
    }
}

/*******************************************************************************
* StackDumpContext
*/
typedef enum{
    SDT_BUFFER,
    SDT_STREAM,
} StackDumpTarget;

typedef struct{
    HANDLE threadHandle;        // Thread handle
    HANDLE stackDumpDoneEvent;    // Signaled when the stack dump is complete.
    LPCONTEXT stack;            // if available, caller's execution context

    StackDumpTarget dumpTarget;
    union{
        struct{
            char* buffer;
            int bufferSize;

            char* writeCursor;
            int remainingBufferSize;
        } targetBuffer;

        struct{
            FILE* stream;
        } targetStream;
    };
} StackDumpContext;

void initStackDumpContext(StackDumpContext* context){
    // Fill out the stack dump context
    //    GetCurrentThread() seems to return a dummy handle that probably means "this/current thread".
    //  To get at the real handle, ask for a copy of the handle.
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
        GetCurrentProcess(), &context->threadHandle, 0, false, DUPLICATE_SAME_ACCESS);
    context->stackDumpDoneEvent = CreateEvent(NULL, false, false, NULL);
    context->stack = NULL;
}

void cleanStackDumpContext(StackDumpContext* context){
    CloseHandle(context->threadHandle);
    CloseHandle(context->stackDumpDoneEvent);
}

/* Function sdcWrite()
*    This is the function that is called for all outputs from the actual stackdump.  Since there
*    are different kinds of stackdump output targets, the functions frees the stackdumping
*    code from worrying about how to perform the actual outputting.
*/
void sdcWrite(StackDumpContext* context, char const* format, ...){
    va_list args;
    va_start(args, format);

    switch(context->dumpTarget){
        case SDT_BUFFER:
            {
                // If we're supposed to write to a buffer...
                int charactersPrinted;

                // Print the specified string into the buffer if possible.
                charactersPrinted = _vsnprintf_s(context->targetBuffer.writeCursor, context->targetBuffer.remainingBufferSize, _TRUNCATE, format, args);

                // If the specified string cannot be printed because the buffer isn't large enough, do nothing.
                if(-1 == charactersPrinted)
                    return;

                // Account for the characters printed.
                context->targetBuffer.writeCursor += charactersPrinted;
                context->targetBuffer.remainingBufferSize -= charactersPrinted;
                break;
            }
        case SDT_STREAM:
            // If we're supposed to print to a stream, do it.
            vfprintf(context->targetStream.stream, format, args);
            break;
        default:
            break;
    }

    va_end(args);
}
/*
* StackDumpContext
*******************************************************************************/

void ShowStackEx(StackDumpContext& sdContext);


void CALLBACK WorkerThreadDumpStackProc(ULONG_PTR contextPtr){
    StackDumpContext* context = (StackDumpContext*)contextPtr;

    // Dump the stack.
    ShowStackEx(*context);

    // Tell the waiting thread that we're done dumping the stack.
    SetEvent(context->stackDumpDoneEvent);
}

/* Function dumpStack()
*    This function can be call by any thread at any time to produce a dump of the calling threads stack.
*
*    This function sends the stack dump worker thread the data it requires to produce a stack dump for
*    the calling thread, then goes into a waiting state for the worker thread to finish dumping the stack.
*    When the worker thread is done, the calling thread returns to execution as normal.
*
*/
void swDumpStack(){
    StackDumpContext context = {0};
    DWORD result;

    // Make sure the stackwalk module has been started.
    // If not, start it.
    // If it cannot be started, do not proceed.
    if(!swStartup())
        return;

    // prevent recursive calls
    if (g_dumpingStack)
    {
        printf("Can't dump stack - swDumpStack was called recursively\n");
        return;
    }
    g_dumpingStack = 1;

    initStackDumpContext(&context);
    context.dumpTarget = SDT_STREAM;
    context.targetStream.stream = stdout;

    // Ask the worker thread to dump this thread's stack.
    QueueUserAPC(WorkerThreadDumpStackProc, stackDumpThread, (ULONG_PTR)&context);
    result = WaitForSingleObject(context.stackDumpDoneEvent, WAIT_TOO_LONG);
    if (result != WAIT_OBJECT_0)
    {
        printf("ERROR: swDumpStack - timeout (%i) waiting for dump\n", result);
    }

    cleanStackDumpContext(&context);
    g_dumpingStack = 0;
}

// can be called with a NULL stack, in which case it will look at the current stack
void swDumpStackToBuffer(char* buffer, int bufferSize, PCONTEXT stack){
    StackDumpContext context = {0};
    DWORD result;

    // Make sure the stackwalk module has been started.
    // If not, start it.
    // If it cannot be started, do not proceed.
    if(!swStartup())
        return;

    // prevent recursive calls
    if (g_dumpingStack)
    {
        sprintf_s(buffer, bufferSize, "Couldn't generate stack - swDumpStackToBuffer was called recursively");
        return;
    }
    g_dumpingStack = 1;

    initStackDumpContext(&context);
    context.dumpTarget = SDT_BUFFER;
    context.targetBuffer.writeCursor = context.targetBuffer.buffer = buffer;
    context.targetBuffer.remainingBufferSize = context.targetBuffer.bufferSize = bufferSize;
    context.stack = stack;

    // Ask the worker thread to dump this thread's stack.
    QueueUserAPC(WorkerThreadDumpStackProc, stackDumpThread, (ULONG_PTR)&context);
    result = WaitForSingleObject(context.stackDumpDoneEvent, WAIT_TOO_LONG);
    if (result != WAIT_OBJECT_0)
    {
        sprintf_s(buffer, bufferSize, "ERROR: swDumpStackToBuffer - timeout (%i) waiting for dump", result);
    }

    cleanStackDumpContext(&context);
    g_dumpingStack = 0;
}


// Super messy.
// Clean it up sometime.
void ShowStackEx(StackDumpContext& sdContext)
{

    CONTEXT threadContext = {0};
    int passed_context = 0;

    // Get the thread execution context so we can do a stack dump.
    if (sdContext.stack)
    {
        memcpy(&threadContext, sdContext.stack, sizeof(CONTEXT));
        passed_context = 1;
    }
    else
    {
        threadContext.ContextFlags = CONTEXT_FULL;
        if(!GetThreadContext(sdContext.threadHandle, &threadContext))
        {
            sdcWrite(&sdContext,  "GetThreadContext(): gle = %lu\n", gle );
            return;
        }
    }

    HANDLE hThread = sdContext.threadHandle;

    // normally, call ImageNtHeader() and use machine info from PE header
#ifdef _M_X64
    DWORD imageType = IMAGE_FILE_MACHINE_AMD64;
#else
    DWORD imageType = IMAGE_FILE_MACHINE_I386;
#endif
    HANDLE hProcess = GetCurrentProcess();    // hProcess normally comes from outside
    int frameNum;                            // counts walked frames
    char undName[MAXNAMELEN];                // undecorated name
#ifdef _M_X64
    DWORD64 offsetFromSymbol64;                // tells us how far from the symbol we were
    DWORD offsetFromSymbol32;                // tells us how far from the symbol we were
    IMAGEHLP_SYMBOL64_PACKAGE SymPack;        // IMAGEHLP_SYMBOL64 + string
    IMAGEHLP_MODULE64 Module;
    IMAGEHLP_LINE64 Line;
#else
    // For backwards compatibility with 32-bit apps, offsetFromSymbol64 is 32 bits.
    DWORD offsetFromSymbol64;                // tells us how far from the symbol we were
    DWORD offsetFromSymbol32;                // tells us how far from the symbol we were
    IMAGEHLP_SYMBOL_PACKAGE SymPack;        // IMAGEHLP_SYMBOL + string
    IMAGEHLP_MODULE Module;
    IMAGEHLP_LINE Line;
#endif
    std::string symSearchPath;
    char *tt = 0;

#ifdef _M_X64
    STACKFRAME64 s; // in/out stackframe
#else
    STACKFRAME s; // in/out stackframe
#endif
    memset( &s, '\0', sizeof s );


    // init STACKFRAME for first call
#ifdef _M_X64
    s.AddrPC.Offset = threadContext.Rip;
    s.AddrPC.Mode = AddrModeFlat;
    s.AddrFrame.Offset = threadContext.Rbp;
    s.AddrFrame.Mode = AddrModeFlat;

    memset( &SymPack, '\0', sizeof(IMAGEHLP_SYMBOL64_PACKAGE) );
#else
    s.AddrPC.Offset = threadContext.Eip;
    s.AddrPC.Mode = AddrModeFlat;
    s.AddrFrame.Offset = threadContext.Ebp;
    s.AddrFrame.Mode = AddrModeFlat;

    memset( &SymPack, '\0', sizeof(IMAGEHLP_SYMBOL_PACKAGE) );
#endif
    SymPack.sym.SizeOfStruct = IMGSYMLEN;
    SymPack.sym.MaxNameLength = MAX_SYM_NAME;

    memset( &Line, '\0', sizeof Line );
    Line.SizeOfStruct = sizeof Line;

    memset( &Module, '\0', sizeof Module );
    Module.SizeOfStruct = sizeof Module;

    offsetFromSymbol32 = 0;
    offsetFromSymbol64 = 0;

    for ( frameNum = 0; ; ++ frameNum )
    {
        // get next stack frame (StackWalk(), SymFunctionTableAccess(), SymGetModuleBase())
        // if this returns ERROR_INVALID_ADDRESS (487) or ERROR_NOACCESS (998), you can
        // assume that either you are done, or that the stack is so hosed that the next
        // deeper frame could not be found.
        if ( ! pSW( imageType, hProcess, hThread, &s, &threadContext, NULL,
            pSFTA, pSGMB, NULL ) )
            break;

        //The first frame will always be SymGetSymFromAddr().
        //The 2nd frame will always be WaitForSingleObject(), because the thread will be waiting for the stackdump to complete.
        //.. if we were passed a stack, this assumption won't be true
        if(!passed_context && frameNum < 2) 
            continue;

        sdcWrite(&sdContext, "%3i ", passed_context? frameNum : frameNum - 2);

        if ( s.AddrPC.Offset == 0 )
        {
            sdcWrite(&sdContext,  "(-nosymbols- PC == 0)\n" );
        }
        else
        { // we seem to have a valid PC
            // show procedure info (SymGetSymFromAddr())
#ifdef _M_X64
            if ( ! pSGSFA( hProcess, s.AddrPC.Offset, &offsetFromSymbol64, (PIMAGEHLP_SYMBOL64)&SymPack ) )
#else
            if ( ! pSGSFA( hProcess, s.AddrPC.Offset, &offsetFromSymbol64, (PIMAGEHLP_SYMBOL)&SymPack ) )
#endif
            {
                if ( gle != 487 ) {
                    sdcWrite(&sdContext, "0x%08X (%x,%x,%x,%x)", s.AddrPC.Offset, s.Params[0], s.Params[1], s.Params[2], s.Params[3]);
                    //sdcWrite(&sdContext,  "SymGetSymFromAddr(): gle = %lu", gle );
                }
            }
            else
            {
                char buf[2000];
                // UnDecorateSymbolName()
                // undecorated is just fine for c.
                pUDSN( SymPack.sym.Name, undName, MAXNAMELEN, UNDNAME_NAME_ONLY );
#ifdef _M_X64
                sprintf_s(buf, sizeof(buf), "%s (%llx,%llx,%llx,%llx)", undName, s.Params[0], s.Params[1], s.Params[2], s.Params[3]);
#else
                sprintf_s(buf, sizeof(buf), "%s (%x,%x,%x,%x)", undName, s.Params[0], s.Params[1], s.Params[2], s.Params[3]);
#endif
                sdcWrite(&sdContext, buf);
                //sdcWrite(&sdContext,  "%-25s", undName );
            }

            // show line number info, NT5.0-method (SymGetLineFromAddr())
            if ( pSGLFA != NULL )
            {    
                // yes, we have SymGetLineFromAddr()
                if ( ! pSGLFA( hProcess, s.AddrPC.Offset, &offsetFromSymbol32, &Line ) )
                {
                    if ( gle != 487 ) {
                        //sdcWrite(&sdContext,  "\nSymGetLineFromAddr(): gle = %lu", gle );
                    }
                }
                else
                {
                    sdcWrite(&sdContext,  "\n\t\tLine: %s(%lu)",
                        Line.FileName, Line.LineNumber);
                }
            } // yes, we have SymGetLineFromAddr()

            sdcWrite(&sdContext, "\n");

        } // we seem to have a valid PC

        // no return address means no deeper stackframe
        if ( s.AddrReturn.Offset == 0 )
        {
            // avoid misunderstandings in the sdcWrite(&sdContext, ) following the loop
            SetLastError( 0 );
            break;
        }

    } // for ( frameNum )

    if ( gle != 0 )
        sdcWrite(&sdContext,  "\nStackWalk(): gle = %lu\n", gle );

    // Don't do this!
    // flushall flushes even files that are meant for reading.  This would definitely screw something up if the
    // thread being stack dumped is reading a file!
    // If you want tio flush stdout immediately, do it some other way!
    //flushall();


    // de-init symbol handler etc. (SymCleanup())
    pSC( hProcess );
    delete [] tt;
}



void enumAndLoadModuleSymbols( HANDLE hProcess, DWORD pid )
{
    ModuleList modules;
    ModuleListIter it;

    // fill in module list
    fillModuleList( modules, pid, hProcess );

    for ( it = modules.begin(); it != modules.end(); ++ it )
    {
        // unfortunately, SymLoadModule() wants writeable strings
        std::vector<char> img((*it).imageName.begin(), (*it).imageName.end());
        img.push_back('\0');
        std::vector<char> mod((*it).moduleName.begin(), (*it).moduleName.end());
        mod.push_back('\0');

        if ( pSLM( hProcess, 0, &img[0], &mod[0], (*it).baseAddress, (*it).size ) == 0 )
            //if ( pSLM( hProcess, 0, (char*)(*it).imageName.c_str(), (*it).moduleName.c_str(), (*it).baseAddress, (*it).size ) == 0 )
        {
            //JE: I don't care what module symboles have *not* been loaded
            //printf( "Error %lu loading symbols for \"%s\"\n", gle, (*it).moduleName.c_str() );
        }
        else
        {
            //JS: I don't care what module symbols have been loaded.
            //printf( "Symbols loaded: \"%s\"\n", (*it).moduleName.c_str() );
        }
    }
}



bool fillModuleList( ModuleList& modules, DWORD pid, HANDLE hProcess )
{
    // try toolhelp32 first
    if ( fillModuleListTH32( modules, pid ) )
        return true;
    // nope? try psapi, then
    return fillModuleListPSAPI( modules, pid, hProcess );
}



// miscellaneous toolhelp32 declarations; we cannot #include the header
// because not all systems may have it
#define MAX_MODULE_NAME32 255
#define TH32CS_SNAPMODULE   0x00000008
#pragma pack( push, 8 )
typedef struct tagMODULEENTRY32
{
    DWORD   dwSize;
    DWORD   th32ModuleID;       // This module
    DWORD   th32ProcessID;      // owning process
    DWORD   GlblcntUsage;       // Global usage count on the module
    DWORD   ProccntUsage;       // Module usage count in th32ProcessID's context
    BYTE  * modBaseAddr;        // Base address of module in th32ProcessID's context
    DWORD   modBaseSize;        // Size in bytes of module starting at modBaseAddr
    HMODULE hModule;            // The hModule of this module in th32ProcessID's context
    char    szModule[MAX_MODULE_NAME32 + 1];
    char    szExePath[MAX_PATH];
} MODULEENTRY32;
typedef MODULEENTRY32 *  PMODULEENTRY32;
typedef MODULEENTRY32 *  LPMODULEENTRY32;
#pragma pack( pop )



bool fillModuleListTH32( ModuleList& modules, DWORD pid )
{
    // CreateToolhelp32Snapshot()
    typedef HANDLE (__stdcall *tCT32S)( DWORD dwFlags, DWORD th32ProcessID );
    // Module32First()
    typedef BOOL (__stdcall *tM32F)( HANDLE hSnapshot, LPMODULEENTRY32 lpme );
    // Module32Next()
    typedef BOOL (__stdcall *tM32N)( HANDLE hSnapshot, LPMODULEENTRY32 lpme );

    // I think the DLL is called tlhelp32.dll on Win9X, so we try both
    const char *dllname[] = { "kernel32.dll", "tlhelp32.dll" };
    HINSTANCE hToolhelp = nullptr;
    tCT32S pCT32S = nullptr;
    tM32F pM32F = nullptr;
    tM32N pM32N = nullptr;

    HANDLE hSnap;
    MODULEENTRY32 me = { sizeof me };
    bool keepGoing;
    ModuleEntry e;
    int i;

    for ( i = 0; i < lenof( dllname ); ++ i )
    {
        hToolhelp = LoadLibraryA( dllname[i] );
        if ( hToolhelp == 0 )
            continue;
        pCT32S = (tCT32S) GetProcAddress( hToolhelp, "CreateToolhelp32Snapshot" );
        pM32F = (tM32F) GetProcAddress( hToolhelp, "Module32First" );
        pM32N = (tM32N) GetProcAddress( hToolhelp, "Module32Next" );
        if ( pCT32S != 0 && pM32F != 0 && pM32N != 0 )
            break; // found the functions!
        FreeLibrary( hToolhelp );
        hToolhelp = 0;
    }

    if ( hToolhelp == 0 ) // nothing found?
        return false;

    hSnap = pCT32S( TH32CS_SNAPMODULE, pid );
    if ( hSnap == (HANDLE) -1 )
        return false;

    keepGoing = !!pM32F( hSnap, &me );
    while ( keepGoing )
    {
        // here, we have a filled-in MODULEENTRY32
        // JS: I don't care what modules are loaded.
        //printf( "%08lXh %6lu %-15.15s %s\n", me.modBaseAddr, me.modBaseSize, me.szModule, me.szExePath );
        e.imageName = me.szExePath;
        e.moduleName = me.szModule;
        e.baseAddress = (uintptr_t)me.modBaseAddr;
        e.size = me.modBaseSize;
        modules.push_back( e );
        keepGoing = !!pM32N( hSnap, &me );
    }

    CloseHandle( hSnap );

    FreeLibrary( hToolhelp );

    return modules.size() != 0;
}



// miscellaneous psapi declarations; we cannot #include the header
// because not all systems may have it
typedef struct MODULEINFO {
    LPVOID lpBaseOfDll;
    DWORD SizeOfImage;
    LPVOID EntryPoint;
} MODULEINFO, *LPMODULEINFO;



bool fillModuleListPSAPI( ModuleList& modules, DWORD pid, HANDLE hProcess )
{
    // EnumProcessModules()
    typedef BOOL (__stdcall *tEPM)( HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded );
    // GetModuleFileNameEx()
    typedef DWORD (__stdcall *tGMFNE)( HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize );
    // GetModuleBaseName() -- redundant, as GMFNE() has the same prototype, but who cares?
    typedef DWORD (__stdcall *tGMBN)( HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize );
    // GetModuleInformation()
    typedef BOOL (__stdcall *tGMI)( HANDLE hProcess, HMODULE hModule, LPMODULEINFO pmi, DWORD nSize );

    HINSTANCE hPsapi;
    tEPM pEPM;
    tGMFNE pGMFNE;
    tGMBN pGMBN;
    tGMI pGMI;

    unsigned int i;
    ModuleEntry e;
    DWORD cbNeeded;
    MODULEINFO mi;
    HMODULE *hMods = 0;
    char *tt = 0;

    hPsapi = LoadLibraryA( "psapi.dll" );
    if ( hPsapi == 0 )
        return false;

    modules.clear();

    pEPM = (tEPM) GetProcAddress( hPsapi, "EnumProcessModules" );
    pGMFNE = (tGMFNE) GetProcAddress( hPsapi, "GetModuleFileNameExA" );
    pGMBN = (tGMFNE) GetProcAddress( hPsapi, "GetModuleBaseNameA" );
    pGMI = (tGMI) GetProcAddress( hPsapi, "GetModuleInformation" );
    if ( pEPM == 0 || pGMFNE == 0 || pGMBN == 0 || pGMI == 0 )
    {
        // yuck. Some API is missing.
        FreeLibrary( hPsapi );
        return false;
    }

    hMods = new HMODULE[TTBUFLEN / sizeof HMODULE];
    tt = new char[TTBUFLEN];
    // not that this is a sample. Which means I can get away with
    // not checking for errors, but you cannot. :)

    if ( ! pEPM( hProcess, hMods, TTBUFLEN, &cbNeeded ) )
    {
        printf( "EPM failed, gle = %lu\n", gle );
        goto cleanup;
    }

    if ( cbNeeded > TTBUFLEN )
    {
        printf("More than %zu module handles. Huh?\n", (TTBUFLEN / sizeof HMODULE));
        goto cleanup;
    }

    for ( i = 0; i < cbNeeded / sizeof hMods[0]; ++ i )
    {
        // for each module, get:
        // base address, size
        pGMI( hProcess, hMods[i], &mi, sizeof mi );
        e.baseAddress = (uintptr_t)mi.lpBaseOfDll;
        e.size = mi.SizeOfImage;
        // image file name
        tt[0] = '\0';
        pGMFNE( hProcess, hMods[i], tt, TTBUFLEN );
        e.imageName = tt;
        // module name
        tt[0] = '\0';
        pGMBN( hProcess, hMods[i], tt, TTBUFLEN );
        e.moduleName = tt;
        printf( "%ph %6lu %-15.15s %s\n", reinterpret_cast<void*>(e.baseAddress),
            e.size, e.moduleName.c_str(), e.imageName.c_str() );

        modules.push_back( e );
    }

cleanup:
    if ( hPsapi )
        FreeLibrary( hPsapi );
    delete [] tt;
    delete [] hMods;

    return modules.size() != 0;
}


/***********************************************************************
* Stack dump testing code
*/
void ThreadFunc2(float alsoUseless)
{
    swDumpStack();
}

void ThreadFunc1(int useless)
{
    ThreadFunc2((float)useless);
}

DWORD __stdcall TargetThread( void *arg )
{
    (void) arg;
    ThreadFunc1(2002);
    return 0;
}

void TestStackDump()
{
    HANDLE hThread;
    DWORD idThread;
    DWORD result;

    swStartup();
    hThread = GetCurrentThread();

    hTapTapTap = CreateEvent( NULL, false, false, NULL );

    hThread = CreateThread( NULL, 5*524288, TargetThread, NULL, 0, &idThread );

    //Sleep(1000);
    result = WaitForSingleObject(hThread, WAIT_TOO_LONG); // to let the thread commit suicide
    if (result != WAIT_OBJECT_0)
    {
        printf("ERROR: TestStackDump timed out (%i)", result);
    }
    swShutdown();
}

/*
* Stack dump testing code
***********************************************************************/


}
