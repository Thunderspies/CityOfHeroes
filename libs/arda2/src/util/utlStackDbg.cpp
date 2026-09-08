/*****************************************************************************
    created:    2002/04/03
    copyright:    2002, NCSoft. All Rights Reserved
    author(s):    Tom Gambill
    
    purpose:    Provides the ability to walk the stack with symbols during the
                execution of the application

                This code was borrowed from BugSlayer MSJ
*****************************************************************************/

#include "../../include/arda2/core/corFirst.h"

#if CORE_SYSTEM_WIN32 && !CORE_COMPILER_GNU
// Equivalent to this functionality on 360 is Dm*, like DmGetSymbolFromAddress()


#include "../../include/arda2/util/utlStackDbg.h"
#include <stdio.h>

//#include <string.h>
#include <dbghelp.h>

// ======================================================================

typedef DWORD (__stdcall *SymGetOptionsFunctionType)();
typedef DWORD (__stdcall *SymSetOptionsFunctionType)(DWORD SymOptions);
typedef BOOL  (__stdcall *SymInitializeFunctionType)(HANDLE hProcess, PSTR UserSearchPath, BOOL fInvadeProcess);
typedef BOOL  (__stdcall *SymCleanupFunctionType)(HANDLE hProcess);
typedef DWORD (__stdcall *SymLoadModule64FunctionType)(HANDLE hProcess, HANDLE hFile, PSTR ImageName, PSTR ModuleName, DWORD64 BaseOfDll, DWORD SizeOfDll);
typedef PVOID (__stdcall *SymFunctionTableAccess64FunctionType)(HANDLE hProcess, DWORD64 AddrBase);
typedef DWORD (__stdcall *SymGetModuleBase64FunctionType)(HANDLE hProcess, DWORD64 dwAddr);
typedef BOOL  (__stdcall *SymGetModuleInfoFunctionType)(HANDLE hProcess, DWORD dwAddr, PIMAGEHLP_MODULE ModuleInfo);
typedef BOOL  (__stdcall *SymGetSymFromAddrFunctionType)(HANDLE hProcess, DWORD Address, PDWORD Displacement, PIMAGEHLP_SYMBOL Symbol);
typedef BOOL  (__stdcall *SymGetLineFromAddrFunctionType)(HANDLE hProcess, DWORD dwAddr, PDWORD pdwDisplacement, PIMAGEHLP_LINE Line);
typedef BOOL  (__stdcall *StackWalk64FunctionType)(DWORD MachineType, HANDLE hProcess, HANDLE hThread, LPSTACKFRAME64 StackFrame, PVOID ContextRecord, PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine, PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine, PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine, PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress);

static SymGetOptionsFunctionType            SymGetOptionsFunction;
static SymSetOptionsFunctionType            SymSetOptionsFunction;
static SymInitializeFunctionType            SymInitializeFunction;  
static SymCleanupFunctionType               SymCleanupFunction;  
static SymLoadModule64FunctionType          SymLoadModuleFunction;  
static PFUNCTION_TABLE_ACCESS_ROUTINE64     SymFunctionTableAccessFunction;
static PGET_MODULE_BASE_ROUTINE64           SymGetModuleBaseFunction;
static SymGetModuleInfoFunctionType         SymGetModuleInfoFunction;
static SymGetSymFromAddrFunctionType        SymGetSymFromAddrFunction;
static SymGetLineFromAddrFunctionType       SymGetLineFromAddrFunction;
static StackWalk64FunctionType              StackWalkFunction;

//*****************************************************************
//*  Install()
//*
//*        Loads the help dll and initializes everything needed to 
//*        walk the stack on the current process.
//*
//*****************************************************************
bool utlStackDbg::Install()
{
  // try to load the libraries
  m_hLibrary = LoadLibraryA("dbghelp.dll");
  if (!m_hLibrary)
    m_hLibrary = LoadLibraryA("imagehlp.dll");
  if (!m_hLibrary)
    return false;

  // get the function pointers
  SymGetOptionsFunction          = reinterpret_cast<SymGetOptionsFunctionType>(GetProcAddress(m_hLibrary, "SymGetOptions"));
  SymSetOptionsFunction          = reinterpret_cast<SymSetOptionsFunctionType>(GetProcAddress(m_hLibrary, "SymSetOptions"));
  SymInitializeFunction          = reinterpret_cast<SymInitializeFunctionType>(GetProcAddress(m_hLibrary, "SymInitialize"));
  SymCleanupFunction             = reinterpret_cast<SymCleanupFunctionType>(GetProcAddress(m_hLibrary, "SymCleanup"));
  SymLoadModuleFunction          = reinterpret_cast<SymLoadModule64FunctionType>(GetProcAddress(m_hLibrary, "SymLoadModule64"));
  SymFunctionTableAccessFunction = reinterpret_cast<SymFunctionTableAccess64FunctionType>(GetProcAddress(m_hLibrary, "SymFunctionTableAccess64"));
  SymGetModuleBaseFunction       = reinterpret_cast<PGET_MODULE_BASE_ROUTINE64>(GetProcAddress(m_hLibrary, "SymGetModuleBase64"));
  SymGetModuleInfoFunction       = reinterpret_cast<SymGetModuleInfoFunctionType>(GetProcAddress(m_hLibrary, "SymGetModuleInfo"));
  SymGetSymFromAddrFunction      = reinterpret_cast<SymGetSymFromAddrFunctionType>(GetProcAddress(m_hLibrary, "SymGetSymFromAddr"));
  SymGetLineFromAddrFunction     = reinterpret_cast<SymGetLineFromAddrFunctionType>(GetProcAddress(m_hLibrary, "SymGetLineFromAddr"));
  StackWalkFunction              = reinterpret_cast<StackWalk64FunctionType>(GetProcAddress(m_hLibrary, "StackWalk64"));

  // set the symbol engine options 
  DWORD options = SymGetOptionsFunction();
  options |= SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_OMAP_FIND_NEAREST;
  SymSetOptionsFunction(options);

  // get the file name of the current running exe
  char szModuleFileName[MAX_PATH];
  if (!GetModuleFileNameA(NULL, szModuleFileName, sizeof(szModuleFileName)))
    return false;

  // get the directory name that has the executable
  char szSearchPath[MAX_PATH];
  strcpy(szSearchPath, szModuleFileName);
  char * slash = strrchr(szSearchPath, '\\');
  if (slash)
    *slash = '\0';

  // initialize the symbol engine
  m_hProcess = GetCurrentProcess();
  if (!SymInitializeFunction(m_hProcess, NULL, false))
    return false;

  // open the current exe image file
  HANDLE hFile = CreateFileA(szModuleFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
    return false;

  // load the image file into the symbol engine
  DWORD dwBase = SymLoadModuleFunction(m_hProcess, hFile, szModuleFileName, NULL, 0, 0);
  CloseHandle(hFile);
  if (dwBase == 0)
    return false;

  m_bInstalled = true;
  return true;
}


//*****************************************************************
//*  Release()
//*
//*        Unload the DLL and release
//*
//*****************************************************************
void utlStackDbg::Release()
{
  SymCleanupFunction(m_hProcess);
  FreeLibrary(m_hLibrary);
  m_hProcess = INVALID_HANDLE_VALUE;
  m_bInstalled = false;
  m_hLibrary = NULL;

  SymSetOptionsFunction = NULL;
  SymInitializeFunction = NULL;  
  SymLoadModuleFunction = NULL;  
  SymFunctionTableAccessFunction = NULL;
  SymGetModuleBaseFunction = NULL;
  SymGetModuleInfoFunction = NULL;
  SymGetSymFromAddrFunction = NULL;
  SymGetLineFromAddrFunction = NULL;
  StackWalkFunction = NULL;
}


//*****************************************************************
//*  GetSingleton()
//*
//*        Initializes the first time, then returns the same static 
//*        object after that
//*
//*****************************************************************
utlStackDbg& utlStackDbg::GetSingleton() 
{
    static utlStackDbg s_Singleton;

    if ( !s_Singleton.IsInstalled() )
    {
        s_Singleton.Install();
    }

    return s_Singleton;
}


//*****************************************************************
//*  GetCallerReturnAddress()
//*
//*        Gets the return address of the caller from the current
//*        stack location
//*
//*****************************************************************
DWORD utlStackDbg::GetCallerReturnAddress(int iStackFramesBack)
{
    if (!m_bInstalled)
        return 0;

    // setup the thread context information
    HANDLE hThread = GetCurrentThread();
    CONTEXT stCtx;
    stCtx.ContextFlags = CONTEXT_CONTROL;
    if (!GetThreadContext(hThread, &stCtx))
        return 0;

    // setup the current stack frame
    STACKFRAME64 stFrame;
    ZeroMemory(&stFrame, sizeof(stFrame));
    stFrame.AddrPC.Mode      = AddrModeFlat;
    stFrame.AddrPC.Offset    = stCtx.Eip;
    stFrame.AddrStack.Mode   = AddrModeFlat;
    stFrame.AddrStack.Offset = stCtx.Esp;
    stFrame.AddrFrame.Mode   = AddrModeFlat;
    stFrame.AddrFrame.Offset = stCtx.Ebp;

    // Don't count the call into 
    ++iStackFramesBack;

    // trace the stack back up to 512 frames
    for ( ; iStackFramesBack && StackWalkFunction(IMAGE_FILE_MACHINE_I386, m_hProcess, hThread, &stFrame, &stCtx, NULL, SymFunctionTableAccessFunction, SymGetModuleBaseFunction, NULL); --iStackFramesBack)
        ;

    // make sure we got back as far as they requested
    if (iStackFramesBack == 0)
        return (DWORD)stFrame.AddrPC.Offset;

    return 0;
}

// getprogramcounterix86 - Helper function that retrieves the program counter
//   (aka the EIP register) for getstacktrace() on Intel x86 architecture. There
//   is no way for software to directly read the EIP register. But it's value
//   can be obtained by calling into a function (in our case, this function) and
//   then retrieving the return address, which will be the program counter from
//   where the function was called.
//
//  Notes:
//  
//    a) Frame pointer omission (FPO) optimization must be turned off so that
//       the EBP register is guaranteed to contain the frame pointer. With FPO
//       optimization turned on, EBP might hold some other value.
//
//    b) Inlining of this function must be disabled. The whole purpose of this
//       function's existence depends upon it being a *called* function.
//
//  Return Value:
//
//    Returns the return address of the current stack frame.
//
unsigned long GetProgramCounterIntelX86()
{
    unsigned long programcounter;

    __asm mov eax, [ebp + 4]         // Get the return address out of the current stack frame
    __asm mov [programcounter], eax  // Put the return address into the variable we'll return

    return programcounter;
}

int    utlStackDbg::StackProbe( int iStartDepth, DWORD *pAddresses, int iProbeDepth )
{
    if (!m_bInstalled)
        return 0;

    // setup the thread context information
    HANDLE hThread = GetCurrentThread();
    CONTEXT stCtx;
    unsigned long framePointer;
    unsigned long programCounter;

    programCounter = GetProgramCounterIntelX86();
    __asm mov [framePointer], ebp  // Get the frame pointer (aka base pointer)

    // Initialize the STACKFRAME64 structure.
    STACKFRAME64 frame;
    memset(&frame, 0x0, sizeof(frame));
    frame.AddrPC.Offset    = programCounter;
    frame.AddrPC.Mode      = AddrModeFlat;
    frame.AddrFrame.Offset = framePointer;
    frame.AddrFrame.Mode   = AddrModeFlat;

    // NOTE: do we need these too?
    //frame.AddrStack.Mode   = AddrModeFlat;
    //frame.AddrStack.Offset = stCtx.Esp;

    int iAddresses = 0;
    for ( int iDepth = 0; iAddresses < iProbeDepth; ++iDepth )
    {
        if ( !StackWalkFunction(IMAGE_FILE_MACHINE_I386, m_hProcess, hThread, &frame, &stCtx, NULL, SymFunctionTableAccessFunction, SymGetModuleBaseFunction, NULL) )
            break;

        if ( iDepth > iStartDepth )
        {
            pAddresses[iAddresses++] = (DWORD)frame.AddrPC.Offset;
        }
    }

    return iAddresses;
}



//*****************************************************************
//*  GetModuleNameFromAddress()
//*
//*        Returns the EXE or DLL name that contains the method at the
//*        address.
//*
//*****************************************************************
void utlStackDbg::GetModuleNameFromAddress(DWORD dwAddress, char *szModuleName, int iMaxModuleNameLength)
{
  szModuleName[0] = '\0';

  if (!m_bInstalled)
  {
    return;
  }

  // setup the struct to receive the module name
  IMAGEHLP_MODULE module;
  ZeroMemory(&module, sizeof(module));
  module.SizeOfStruct = sizeof(module);

  // get the module name
  if (SymGetModuleInfoFunction(m_hProcess, dwAddress, &module))
  {
    // strip off the path
    const char *szName = strrchr(module.ImageName, '\\');
    if (szName)
      ++szName;
    else
      szName = module.ImageName;

    strncpy(szModuleName, szName, iMaxModuleNameLength-1);
    szModuleName[iMaxModuleNameLength-1] = '\0';
  }
}


//*****************************************************************
//*  GetSymbolNameFromAddress()
//*
//*        Returns the symbol name from the address
//*
//*****************************************************************
bool utlStackDbg::GetSymbolNameFromAddress(DWORD dwAddress, char *szSymbolName, int iMaxSymbolNameLength)
{
  szSymbolName[0] = '\0';

  if (!m_bInstalled)
  {
    return false;
  }

  // setup the struct to receive the symbol information
  char buffer[sizeof(IMAGEHLP_SYMBOL) + MAX_PATH];
  ZeroMemory(buffer, sizeof(buffer));
  PIMAGEHLP_SYMBOL symbol = reinterpret_cast<PIMAGEHLP_SYMBOL>(buffer);
  symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
  symbol->Address = dwAddress;
  symbol->MaxNameLength = sizeof(buffer) - sizeof(IMAGEHLP_SYMBOL);

  // get the symbol information
  DWORD displacement = 0;
  if (!SymGetSymFromAddrFunction(m_hProcess, dwAddress, &displacement, symbol))
    return false;

  strncpy(szSymbolName, symbol->Name, iMaxSymbolNameLength);
  szSymbolName[iMaxSymbolNameLength-1] = '\0';
  return true;
}


//*****************************************************************
//*  GetFileNameAndLineFromAddress()
//*
//*        Returns the filename and line number from the address
//*
//*****************************************************************
bool utlStackDbg::GetFileNameAndLineFromAddress(DWORD dwAddress, char *&szFileName, int &riLineNumber)
{
    szFileName = 0;
    riLineNumber = 0;
    if (!m_bInstalled || !SymGetLineFromAddrFunction)
        return false;

    // deal with bug where symbol engine only finds source line dwAddresses that fall exactly on a zero displacement
    const int     MAX_SEARCH = 5;
    IMAGEHLP_LINE line;
    DWORD         displacement;
    for (DWORD i = 0; i < MAX_SEARCH; ++i)
    {
        // setup the struct to receive the source file and line number
        ZeroMemory(&line, sizeof(line));
        line.SizeOfStruct = sizeof(line);
        displacement = 0;

        if (SymGetLineFromAddrFunction(m_hProcess, dwAddress - i, &displacement, &line))
        {
            szFileName = line.FileName;
            riLineNumber = line.LineNumber;
            return true;
        }
    }

    return false;
}

#define MAX_SYMBOL_LEN    96

bool utlStackDbg::BuildStackTraceStringFromProbe(int iNumAddresses, DWORD *pAddresses, char *szStackTrace, int iMaxLen)
{
    if (!SymGetLineFromAddrFunction)
        return false;

    int iCurPos = 0;
    for ( int i = 0; i < iNumAddresses; ++ i )
    {
        //* Get the address of the caller at this depth
        DWORD dwAddress = pAddresses[i];

        IMAGEHLP_LINE line;
        DWORD         displacement = 0;
        ZeroMemory(&line, sizeof(line));
        line.SizeOfStruct = sizeof(line);
        if (SymGetLineFromAddrFunction(m_hProcess, dwAddress, &displacement, &line) && line.FileName)
        {
            char szSymbol[MAX_SYMBOL_LEN]; szSymbol[0] = '\0';
            GetSymbolNameFromAddress(dwAddress, szSymbol, MAX_SYMBOL_LEN);

            //* We have something, add it to the current string
            iCurPos += snprintf(&szStackTrace[iCurPos], iMaxLen - iCurPos, "%s(%d) : %s\r\n", line.FileName, line.LineNumber, szSymbol);
        }
        else
        {
            //* We don't have the symbols for this function, put in a place holder
            iCurPos += snprintf(&szStackTrace[iCurPos], iMaxLen - iCurPos, "<Unknown> \r\n");
        }
    }

    return true;
}



//*****************************************************************
//*  BuildStackTraceStringFromAddress()
//*
//*        Fills a string with a text representation of a stack trace
//*        from the depth specified
//*
//*****************************************************************
bool utlStackDbg::BuildStackTraceStringFromAddress(int iStartDepth, int iMaxDepth, char *szStackTrace, int iMaxLen)
{
    if (!SymGetLineFromAddrFunction)
        return false;

    const int kMaxDepth = 20;
    iMaxDepth = Min(iMaxDepth, kMaxDepth);
    DWORD addresses[kMaxDepth];
    int iDepth = StackProbe(iStartDepth + 1, addresses, iMaxDepth);

    return BuildStackTraceStringFromProbe(iDepth, addresses, szStackTrace, iMaxLen);
}

#endif // CORE_SYSTEM_WIN32 && !CORE_COMPILER_GNU
