#include <stdio.h>
#include <stdlib.h>
#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/MemoryMonitor.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/utils.h"

#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/components/earray.h"

//
// set _CRTDBG_MAP_ALLOC=1
// for DevStudio malloc debug
//

#define MAX_MESSAGES 30

char msgs[MAX_MESSAGES][128];
int    msg_count;


void addmsg(char *cmd,int size,char *filename,int line,void *mem)
{
    if (++msg_count >= MAX_MESSAGES)
        msg_count = 0;
    sprintf_s(SAFESTR(msgs[msg_count]),"%s(%08p:%d) @ %s:%d",cmd,mem,size,filename,line);
}

void *chk_malloc(int amt,char *filename,int line)
{
void    *mem;    

    mem = malloc(amt);
    addmsg("malloc",amt,filename,line,mem);

    return mem;
}

void *chk_calloc(int amt,int amt2,char *filename,int line)
{
void    *mem;

    mem = calloc(amt,amt2);
    addmsg("calloc",amt*amt2,filename,line,mem);
    return mem;
}

#include "string.h"
void *chk_realloc(void *orig,int amt,char *filename,int line)
{
void    *mem;

#if 0
    // JS: Disabled this piece of voodoo magic because it crashes itself.
    mem = malloc(amt);
    if (orig)
    {
        memcpy(mem,orig,((int *)orig)[-4]);
        free(orig);        // JS: Voodoo code crashing on this line.
    }
#else
    mem = realloc(orig,amt);
#endif
    addmsg("realloc",amt,filename,line,mem);
    return mem;
}

void chk_free(void *mem,char *filename,int line)
{
    addmsg("free",0,filename,line,mem);
    free(mem);
}

typedef struct
{
    U8    *mem;
    int    len;
} MemChunk;

MemChunk mem_chunks[100];
int        mem_chunk_count;

void memChunkAdd(void *mem)
{
MemChunk    *chunk;

    mem_chunk_count = 1;
    chunk = &mem_chunks[0];
    chunk->mem = mem;
    memset(mem,0xf7,((U32 *)mem)[-4]);
    chunk->len = ((U32 *)mem)[-4];
}

void memChunkCheck()
{
int        i,j;
MemChunk    *chunk;

    for(i=0;i<mem_chunk_count;i++)
    {
        chunk = &mem_chunks[i];
        for(j=0;j<chunk->len;j++)
        {
            if (chunk->mem[j] != 0xf7)
                goterr();
        }
    }
}


void goterr()
{
volatile int        a=1,b=0;
    a = a/b;
    printf("",a);
}

#if _WIN32

#include "utilitieslib/utils/wininclude.h"

#ifdef _DEBUG
#define  SET_CRT_DEBUG_FIELD(a)   _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define  CLEAR_CRT_DEBUG_FIELD(a) _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif

static int __cdecl MyAllocHook(
   int      nAllocType,
   void   * pvData,
   size_t   nSize,
   int      nBlockUse,
   long     lRequest,
   const unsigned char * szFileName,
   int      nLine
   )
{
char *operation[] = { "", "allocating", "re-allocating", "freeing" };
//char *blockType[] = { "Free", "Normal", "CRT", "Ignore", "Client" };

    if ( nBlockUse == _CRT_BLOCK )   // Ignore internal C runtime library allocations
        return( 1 );

    printf("%s:%d: %s %d\n", szFileName, nLine, operation[nAllocType], nSize);

    return( 1 );         // Allow the memory operation to proceed
}

int CrtReportingFunction( int reportType, char *userMessage, int *retVal )
{
    if (reportType == _CRT_WARN) {
        return FALSE;
    }
    printf("\nCRT Error: %s\n", userMessage);

    // Disable minidumps because they have to allocate memory to start a thread and we have the heap locked
    if (getAssertMode() & ASSERTMODE_MINIDUMP && !strstri(userMessage, "Run-Time Check")) {
        // Can't do a mini, do a full!
        setAssertMode(getAssertMode() & ~(ASSERTMODE_MINIDUMP|ASSERTMODE_ZIPPED) | ASSERTMODE_FULLDUMP);
    } else {
        setAssertMode(getAssertMode() & ~(ASSERTMODE_ZIPPED));
    }
    assertmsg(!"CRT Error", userMessage);
    // Debug by default if we get here.
    *retVal = 1;
    return 1;
}


extern void test(void);
void memCheckInit()
{
    static bool inited=false;
    static _CrtMemState state;

    if (inited)
        return;
    inited = true;

#ifdef ENABLE_LEAK_DETECTION
    GC_find_leak = 1;
    GC_INIT();
    newConsoleWindow();
#endif

    // see if we should be generating a detail memory event log
    // do this first to capture most events
    memtrack_init();

    //_CrtSetAllocHook( MyAllocHook );
    memMonitorInit();
/*    // JE: These would print errors to stdout, but that doesn't exist in the game project, and we re-assign stdout in the newConsoleWindow call anyway!
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
*/
    _CrtSetReportHook(CrtReportingFunction);
    //_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    //_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

    _CrtMemCheckpoint( &state );

    assertInitThreadFreezing();
    assertYouMayFreezeThisThread(GetCurrentThread(),GetCurrentThreadId()); // presumably this is the main thread

    // Set the debug heap to report memory leaks when the process terminates,
    // and to keep freed blocks in the linked list.
    //SET_CRT_DEBUG_FIELD( _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF );

    // Open a log file for the hook functions to use 

    // Install the hook functions
    //_CrtSetDumpClient( MyDumpClientHook );
    //_CrtSetReportHook( MyReportHook );

}

void memCheckDumpAllocs()
{
HFILE        warnfile;
OFSTRUCT    reopenbuf = {0};

    warnfile = OpenFile(  "c:\\memlog.txt",    // pointer to the filename
                          &reopenbuf,    // pointer to the file information struct
                          OF_CREATE);    // specifies the action and attributes

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, (_HFILE)U32_TO_PTR(warnfile));
    _CrtMemDumpAllObjectsSince(0);
    _lclose(warnfile);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}

#endif

int heapGetSize(HANDLE hHeap)
{
    PROCESS_HEAP_ENTRY phe = {0};
    int totaldata=0;

    while (HeapWalk(hHeap, &phe)) {
        totaldata += phe.cbData + phe.cbOverhead;
    }
    return totaldata;
}

size_t heapWalk(HANDLE hHeap, int silent, char *logfilename)
{
    PROCESS_HEAP_ENTRY phe = {0};
    size_t totaldata=0, totaloverhead=0, count=0, totalregions=0, uncommited=0, busy=0, other=0;
    size_t maxuncommited=0;
    void *maxaddr=NULL;
    FILE *logfile=NULL;

    if (logfilename) {
        logfile = fopen(logfilename, "wt");
        if (logfile)
            fprintf(logfile, "DataSize, OverHead,  Address,   Region,    Flags\n");
    }

    while (HeapWalk(hHeap, &phe)) {
        count++;
        totaldata += phe.cbData;
        totaloverhead += phe.cbOverhead;
        if (!silent)
            printf("Heap entry: %8d bytes, %8d overhead at %8Ix\n", phe.cbData, phe.cbOverhead, (intptr_t)phe.lpData);
        if (logfile) {
            fprintf(logfile, "%8d, %8d, %8Id, %8d, %8x\n", phe.cbData, phe.cbOverhead, (intptr_t)phe.lpData, phe.iRegionIndex, phe.wFlags);
        }
        if (phe.wFlags & PROCESS_HEAP_REGION) {
            totalregions++;
        }
        if (phe.wFlags & PROCESS_HEAP_UNCOMMITTED_RANGE) {
            uncommited += phe.cbData + phe.cbOverhead;
            maxuncommited = (maxuncommited>(phe.cbData + phe.cbOverhead))?maxuncommited:(phe.cbData + phe.cbOverhead);
        }
        if (phe.wFlags & PROCESS_HEAP_ENTRY_BUSY) {
            busy += phe.cbData + phe.cbOverhead;
        }
        if (phe.wFlags & (PROCESS_HEAP_ENTRY_MOVEABLE | PROCESS_HEAP_ENTRY_DDESHARE)) {
            other++;
        }

        maxaddr = (maxaddr>phe.lpData)?maxaddr:phe.lpData;
    }
    if (logfile) {
        fclose(logfile);
    }
    printf("Total: %Id allocations, %Id bytes, %Id overhead, max address: %Id, max uncommited: %Id\n", count, totaldata, totaloverhead, maxaddr, maxuncommited);
    printf("   Num regions: %Id, total uncommited: %Id, total busy (alloced): %Id, num other: %Id\n", totalregions, uncommited, busy, other);
    return (intptr_t)maxaddr;
}

void heapInfoPrint()
{
    HANDLE h = (HANDLE)_get_heap_handle();
    heapWalk(h, 1, 0);
}

void heapWalkLog(HANDLE heap)
{
    heapWalk(heap,1,"C:\\heap.log");
}
void heapCompact(HANDLE heap)
{
    HeapCompact(heap, 0);
}

#if PRIVATE_EARRAY_HEAP
extern HANDLE g_earrayheap;
#endif
#if PRIVATE_PARSER_HEAPS
extern HANDLE g_parserheap;
extern HANDLE g_stringheap;
#endif

void heapCompactAll(void)
{
#ifdef ENABLE_LEAK_DETECTION
    GC_gcollect();
#endif

    _heapmin();
    HANDLE h = (HANDLE)_get_heap_handle();
    heapCompact(h);
#if PRIVATE_EARRAY_HEAP
    heapCompact(g_earrayheap);
#endif
#if PRIVATE_PARSER_HEAPS
    heapCompact(g_parserheap);
    heapCompact(g_stringheap);
#endif
    heapCompact(GetProcessHeap());
}

void heapInfoLog()
{
    HANDLE h = (HANDLE)_get_heap_handle();
    heapWalk(h, 1, "c:\\heap_crt.log");
#if PRIVATE_EARRAY_HEAP
    heapWalk(g_earrayheap,1,"c:\\heap_earray.log");
#endif
#if PRIVATE_PARSER_HEAPS
    heapWalk(g_parserheap,1,"c:\\heap_parser.log");
    heapWalk(g_stringheap,1,"c:\\heap_string.log");
#endif
    heapWalk(GetProcessHeap(),1,"c:\\heap_process.log");
}

// Dummy wrappers for calling from command window

// data == NULL means validate the entire heap
int heapValidate(HANDLE heap, DWORD flags, void *data)
{
    return HeapValidate(heap, flags, data);
}

int heapValidateAll(void)
{
    int ret=1;
    HANDLE h = (HANDLE)_get_heap_handle();
#define DOIT(func) \
    if (!(func)) {    \
        ret = 0;    \
        OutputDebugStringA("Heap validation failed on " #func "\r\n"); \
        printf("Heap validation failed on %s\n", #func);\
    }
    DOIT(_CrtCheckMemory());
    DOIT(heapValidate(h, 0, NULL));
    DOIT(mpVerifyAllFreelists());
    DOIT(smallAllocCheckMemory());
    DOIT(heapValidate(GetProcessHeap(), 0, NULL));
#ifdef ENABLE_LEAK_DETECTION
    GC_gcollect();
#endif
    return ret;
}
