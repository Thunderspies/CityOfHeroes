/*****************************************************************************
    created:    2002/04/03
    copyright:    2002, NCSoft. All Rights Reserved
    author(s):    Tom Gambill
    
    purpose:    Debug overloads for new and new[] that call the debug version
                of themselves and supply the actual source line info for output    
                later using _CrtSetDbgFlag()

*****************************************************************************/

#include "../../include/arda2/core/corFirst.h"

#if CORE_SYSTEM_WINAPI

#include "../../include/arda2/core/corStdString.h"

#include "../../include/arda2/memory/memMemoryDbg.h"
#include "../../include/arda2/util/utlStackDbg.h"
#include <stdio.h>
#include <crtdbg.h>

using namespace std;

#if CORE_DEBUG

//* Set to one to enable recording of call stack info
#define LEAK_TRACKING_DETAILED                0

//* Number of callers recorded for each allocation
#define LEAK_TRACKING_DETAILED_STACK_DEPTH    6

//* Maximum length of the string containing the function 
//* names and line numbers for each allocation
#define LEAK_TRACKING_DETAILED_STRING_LEN    4096


int MEMDBG_VERIFY_HEAP() { return _CrtCheckMemory();}

// Detailed tracking requires the compiler-specific stack walker.
#if LEAK_TRACKING_DETAILED && !CORE_COMPILER_GNU

//* This function will be called for each client block that is leaked at application termination
void ClientBlockDump(void *pBlock, size_t tSize)
{
    byte *pByteBlock = (byte*)pBlock;

    size_t actualSize = *(size_t *)(pByteBlock + tSize - sizeof(size_t));
    int iDepth = *(int *)(pByteBlock + actualSize);
    DWORD *pCallStack = (DWORD *)(pByteBlock + actualSize + sizeof(int));

    char buffer[2048];
    char *pChar = buffer;

    pChar += sprintf(pChar, "Block dump:\n");

    // print memory dump of allocated block
    for ( size_t i = 0; i < tSize && i < 64; i += 16 )
    {
        *pChar++ = ' ';
        *pChar++ = ' ';

        // hex dump
        for ( size_t j = 0; j < 16; ++j )
        {
            if ( i + j < tSize )
                pChar += sprintf(pChar, "%02X ", pByteBlock[i+j]);
            else
                pChar += sprintf(pChar, "   ");
            if ( j == 7 )
                *pChar++ = ' ';
        }

        pChar += sprintf(pChar, "   ");

        // ASCII dump
        for ( size_t j = 0; j < 16; ++j )
        {
            if ( i + j < tSize )
            {
                uchar c = pByteBlock[i+j];
                *pChar++ = isprint(c) ? c : '.';
            }
            else
                *pChar++ = ' ';
            if ( j == 7 )
                *pChar++ = ' ';
        }

        pChar += sprintf(pChar, "\n");
    }

    corOutputDebugString("%s", buffer);

    char szCallStack[LEAK_TRACKING_DETAILED_STRING_LEN];
    utlStackDbg::GetSingleton().BuildStackTraceStringFromProbe(iDepth, pCallStack, szCallStack, sizeof(szCallStack));
    corOutputDebugString(szCallStack);
}


//* This class installs the dump function upon the first debug new() called
class InstallDumpClient
{
public:
    InstallDumpClient() 
    { 
        _CrtSetDumpClient(ClientBlockDump); 
        _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
//        _CrtDumpMemoryLeaks(); 
    }

    static InstallDumpClient &GetInstalledDumpClient()
    {
        static InstallDumpClient theDumpClient;
        return theDumpClient;
    }
};
static InstallDumpClient s_DumpInstaller;


void * __cdecl operator new(size_t tSize)
{
    //* install the dump function in the debug heap if it's not already there
    InstallDumpClient::GetInstalledDumpClient();


    //* Build a stack string from the symbol info if available
    DWORD callStack[LEAK_TRACKING_DETAILED_STACK_DEPTH];
    int iDepth = utlStackDbg::GetSingleton().StackProbe(1, callStack, LEAK_TRACKING_DETAILED_STACK_DEPTH);

    //* actually allocate the memory block, increasing the size for the callStack and the length
    size_t extraSize = sizeof(iDepth) + iDepth * sizeof(callStack[0]) + sizeof(tSize);
    char *pAlloc = (char*)::operator new(tSize + extraSize, _CLIENT_BLOCK, "Detailed Allocation Info Below", 0);
    
    //* put the callstack and the size at the end of the block. delete() will
    //* automatically clean up if this allocation is not leaked
    memcpy(&pAlloc[tSize], &iDepth, sizeof(iDepth));
    memcpy(&pAlloc[tSize + sizeof(iDepth)], callStack, iDepth * sizeof(callStack[0]));
    memcpy(&pAlloc[tSize + extraSize - sizeof(tSize)], &tSize, sizeof(tSize));

    return pAlloc;
}


void * __cdecl operator new[](size_t tSize)
{
    //* install the dump function in the debug heap if it's not already there
    InstallDumpClient::GetInstalledDumpClient();

    //* Build a stack string from the symbol info if available
    DWORD callStack[LEAK_TRACKING_DETAILED_STACK_DEPTH];
    int iDepth = utlStackDbg::GetSingleton().StackProbe(1, callStack, LEAK_TRACKING_DETAILED_STACK_DEPTH);

    //* actually allocate the memory block, increasing the size for the callStack and the length
    size_t extraSize = sizeof(iDepth) + iDepth * sizeof(callStack[0]) + sizeof(tSize);
    char *pAlloc = (char*)::operator new(tSize + extraSize, _CLIENT_BLOCK, "Detailed Allocation Info Below", 0);
    
    //* put the callstack and the size at the end of the block. delete() will
    //* automatically clean up if this allocation is not leaked
    memcpy(&pAlloc[tSize], &iDepth, sizeof(iDepth));
    memcpy(&pAlloc[tSize + sizeof(iDepth)], callStack, iDepth * sizeof(callStack[0]));
    memcpy(&pAlloc[tSize + extraSize - sizeof(tSize)], &tSize, sizeof(tSize));

    return pAlloc;
}


#endif // LEAK_TRACKING_DETAILED && !CORE_COMPILER_GNU

#endif // CORE_DEBUG

#endif // WIN32
