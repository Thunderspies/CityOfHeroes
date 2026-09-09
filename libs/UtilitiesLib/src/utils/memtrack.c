#include "utilitieslib/utils/memtrack.h"

#if MEMTRACK_BUILD

#include "utilitieslib/assert/assert.h"
#include "utilitieslib/crt.h"
#include <stdio.h>
#include <time.h>
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/Stackdump.h"      // for sdDumpStackToBuffer used by optional text output
  
#define TIGHT_CRTDBG_INTEGRATION 0

#if TIGHT_CRTDBG_INTEGRATION && _CRTDBG_MAP_ALLOC
// WARNING!!! CRT dependency!
//    Ripped from VS7 CRT library internal header file.

#define nNoMansLandSize 4
typedef struct CrtMemBlockHeader
{
    struct CrtMemBlockHeader * pBlockHeaderNext;
    struct CrtMemBlockHeader * pBlockHeaderPrev;
    char *                      szFileName;
    int                         nLine;
#ifdef _WIN64
    /* These items are reversed on Win64 to eliminate gaps in the struct
    * and ensure that sizeof(struct)%16 == 0, so 16-byte alignment is
    * maintained in the debug heap.
    */
    int                         nBlockUse;
    size_t                      nDataSize;
#else  /* _WIN64 */
    size_t                      nDataSize;
    int                         nBlockUse;
#endif  /* _WIN64 */
    long                        lRequest;
    unsigned char               gap[nNoMansLandSize];
    /* followed by:
    *  unsigned char           data[nDataSize];
    *  unsigned char           anotherGap[nNoMansLandSize];
    */
} _CrtMemBlockHeader;

bool    memtrack_is_crt_hook_enabled(void);
int        memtrack_hook_func( int allocType, void *userData, size_t size, int blockType, long requestNumber, const unsigned char *filename, int lineNumber);

#endif

//-----------------------------------------------------------------------------
// MEMORY MANAGER EVENT LOGGING
//-----------------------------------------------------------------------------

/*
    Code to track and log every memory manager event with a full
    callstack. Captured logs can be data mined for for detailed metrics and
    also 'replayed' to any point in time using a memory manager
    simulator

    We employ a simple scheme where given an allocation event we determine the
    hash code of the event's call stack trace. If we haven't seen this particular
    trace before we add the call stack to our call stick dictionary under the given
    hash.

    We then proceed to log the details of the event and use the stack trace hash
    to link it to the full call stack details we know is already present in the
    call stack dictionary.

    As the signature dictionary should become fairly complete early on in the
    session run this helps to significantly reduce the amount of data we need to
    record on each individual event.

    The system will work with both final and debug builds.

    Optionally we can use some of the information contained in the debug
    CRT data structures. As the CRT debug mem manager adds ~40 bytes of overhead
    to each allocation tracking memory in the 'debug' builds does not provide a
    precise view of memory tracking in a 'release' build.

    With some changes this system can be made to work for 'release' type builds
    (but you would want to do them as a specially compiled build, i.e. not be
    able to toggle the tracking on/off.) But callstack information may be less
    useful, etc.

    CRT allocation hook lets you easily capture information on memory manager events.
    But there are some limitations which make it unable to provide everything needed for
    a robust tracker.
    -    It is only available when _DEBUG and _CRTDBG_MAP_ALLOC are defined.
    -    The hook happens before the event so in the case of an allocation
    the resulting allocation is not available (hence userData supplied to the hook is NULL)
    There is no 'post' hook available so we need manually make a call that records allocation
    addresses after the fact which then get matched up with the hook information during data
    analysis.

    So, it is more straightforward to do all the allocation recording directly at the
    allocation functions after they complete. But to do so we need to wrap every allocation
    function with the tracking code. Since most allocation functions for COH are already
    wrapped this is not be such a big deal.

    Advantage of hook is to record information on alloc events which is useful when you want
    to make sure you are capturing all allocations and aren't sure if some allocs are bypassing
    the wrappers.

    @todo This code has been quickly written to grab data to be analyzed. It isn't
    built to be optimal with regard to impact on the running application or
    flexibility and efficiency.
*/


#define kMaxFramesToCapture        60    // must be < 63 on Windows XP

// filled in with date and time of init
#define kLogFileNameText        "memtrack_%s_log.txt"
#define kLogFileNameBinary        "memtrack_%s_log.dat"
#define kDictFileNameText        "memtrack_%s_dict.txt"
#define kDictFileNameBinary        "memtrack_%s_dict.dat"

// define this to also output a text version of the tracking log for development/debugging tracking
// Requires _CRTDBG_MAP_ALLOC
#define MEMTRACK_OPTION_TEXT_LOG    0 && TIGHT_CRTDBG_INTEGRATION && _CRTDBG_MAP_ALLOC
#define MEMTRACK_OPTION_TEXT_DICT    0    // extremely slow as it expands each new call stack signature to symbols

#define kEventsToFlush    1000            // force flush the log files after this many events

static bool            s_memtrack_init;
static bool            s_memtrack_capture_enabled;    // true if event capturing is enabled

// Set the following flag to true to have the CRT hook also place events into the log stream
// This is useful for debugging/validating the normal event stream and identifying
// what events we are missing.
// It will ~double the size of the event stream so only use it when necessary to resolve
// tracking issues or to periodically validate results.
static bool            s_memtrack_use_crt_hook = false;

static FILE*        s_memtrack_log_dat_file;
static FILE*        s_memtrack_dict_dat_file;

#if MEMTRACK_OPTION_TEXT_DICT
static FILE*        s_memtrack_dict_txt_file;
static char            s_memtrack_stack_text_buffer[0x8000];
#endif

static CRITICAL_SECTION s_memtrack_lock;

static volatile int    s_memtrack_inside_hook = 0;
static volatile int    s_memtrack_inside_logger = 0;

static StashTable    s_memtrack_callstack_dictionary;

typedef USHORT (WINAPI *PFN_CaptureStackBackTrace)(ULONG, ULONG, PVOID*, PULONG);
PFN_CaptureStackBackTrace    s_pfnCaptureStackBackTrace = 0;

//@todo these should be in a common header shared with the analysis program

enum MemTrackEvent_Types
{
    kMemTrackEvent_Hook = 0,
    kMemTrackEvent_Alloc,
    kMemTrackEvent_Realloc,
    kMemTrackEvent_Free,
    kMemTrackEvent_Marker,
};

// we write the events to the log file as chunks with a simple header so that
// we can easily parse and skip from one block to the next if necessary
typedef struct
{
    unsigned int event_size;        // size of complete chunk (header + payload)
    unsigned int event_type;        // type of payload
    unsigned int request_id;    // sequence number of the event or id
    unsigned int stack_id;        // event callstack hash
    unsigned int thread_id;        // id of thread causing the event
} MemTrackEvent_Header;

typedef struct
{
    void*            pData;
    size_t            allocSize;
    unsigned int    allocType;
    unsigned int    blockType;
    unsigned int    blocks_request_id;    // sequence number of pData that is being free'd or realloc'd
    unsigned int    blocks_size;        // current size of pData block that is being free'd or realloc'd
} MemTrackEvent_Hook;


typedef struct
{
    void*            pData;
    size_t            allocSize;
} MemTrackEvent_Alloc;

typedef struct
{
    void*            pData;
    size_t            allocSize;
    void*            pDataPrev;        // used by realloc to link to original allocation
} MemTrackEvent_Realloc;

typedef struct
{
    void*            pData;
} MemTrackEvent_Free;

typedef struct
{
    int                marker_type;
    // rest of marker payload is marker tag string
} MemTrackEvent_Marker;

typedef struct
{
    int                marker_type;
    const char         *marker_msg;
} MemTrack_MarkerData;    // Used internally

static int s_events_logged;
static int s_callstacks_logged;

#if MEMTRACK_OPTION_TEXT_LOG
static FILE*        s_memtrack_log_txt_file;

static void memtrack_write_text_log_header( MemTrackEvent_Header* pHeader )
{
    const char* type_names[] =
    {
        "hook",
        "alloc",
        "realloc",
        "free",
        "marker",
    };

    char text[512];
    sprintf_s( text, sizeof(text), "%-8s, %8d, 0x%p, 0x%p, ", type_names[pHeader->event_type], pHeader->request_id, pHeader->stack_id, pHeader->thread_id );

    //    printf( text );
    fprintf( s_memtrack_log_txt_file, text );
}

static void memtrack_write_text_log_hook( MemTrackEvent_Hook* pET, const unsigned char* filename, int lineNumber )
{
    static char* s_crt_alloc_types[] =
    {
        "unk_zero",
        "alloc",
        "realloc",
        "free",
        "marker",
    };

    static char* s_crt_block_types[] =
    {
        "free_block",
        "normal_block",
        "crt_block",
        "ignore_block",
        "client_block"
    };
    char text[512];
    sprintf_s( text, sizeof(text), "allocType: '%-8s' pData: 0x%p size: %d, %s, block_rq_id: %d, block_size: %d, %s(%d)\n", s_crt_alloc_types[pET->allocType], pET->pData, pET->allocSize,
                    s_crt_block_types[pET->blockType], pET->blocks_request_id, pET->blocks_size, filename, lineNumber );

    //    printf( text );
    fprintf( s_memtrack_log_txt_file, text );
}

static void memtrack_write_text_log_alloc( MemTrackEvent_Alloc* pET, const unsigned char* filename, int lineNumber )
{
    char text[512];
    sprintf_s( text, sizeof(text), "0x%p, %d, %s, %d\n", pET->pData, pET->allocSize, filename, lineNumber );

//    printf( text );
    fprintf( s_memtrack_log_txt_file, text );
}

static void memtrack_write_text_log_realloc( MemTrackEvent_Realloc* pET, const unsigned char* filename, int lineNumber )
{
    char text[512];
    sprintf_s( text, sizeof(text), "0x%p, %d, 0x%p, %s, %d\n", pET->pData, pET->allocSize, pET->pDataPrev, filename, lineNumber );

    //    printf( text );
    fprintf( s_memtrack_log_txt_file, text );
}

static void memtrack_write_text_log_free( MemTrackEvent_Free* pET, const unsigned char* filename, int lineNumber )
{
    char text[512];
    sprintf_s( text, sizeof(text), "0x%p, %s, %d\n", pET->pData, filename, lineNumber );

    //    printf( text );
    fprintf( s_memtrack_log_txt_file, text );
}

static void memtrack_write_text_log_marker( MemTrack_MarkerData* pMD )
{
    char text[512];
    sprintf_s( text, sizeof(text), "0x%08x, \"%s\"\n", pMD->marker_type, pMD->marker_msg );

    //    printf( text );
    fprintf( s_memtrack_log_txt_file, text );
}

#endif    // MEMTRACK_OPTION_TEXT_LOG


// periodically force a flush of the logging files
static void memtrack_flush_kick(void)
{
    static unsigned int s_flush_ops;
    if ( ((s_flush_ops++) % kEventsToFlush) == 0)
    {
        memtrack_flush();
    }

    // periodically log capture stats
    if ( ((s_flush_ops++) % 100000) == 0)
    {
        printf("[memtrack] captured %d events, %d callstacks", s_events_logged, s_callstacks_logged);
    }
}

static ULONG memtrack_log_callstack( void )
{
    ULONG traceHash = 0;
    PVOID frames[kMaxFramesToCapture];
    int numEntries;
    int found_int;

    // Calculate the hash of the current call stack
    numEntries = s_pfnCaptureStackBackTrace(0, kMaxFramesToCapture, (PVOID*)&frames, &traceHash );

    // if we don't already have this call stack signature in our dictionary add it
    if ( !stashIntFindInt(s_memtrack_callstack_dictionary, traceHash, &found_int) )
    {
        stashIntAddInt(s_memtrack_callstack_dictionary, traceHash, 1, true);

        // write the stack chunk out to data file
        fwrite( &traceHash, sizeof(traceHash), 1, s_memtrack_dict_dat_file );
        fwrite( &numEntries, sizeof(numEntries), 1, s_memtrack_dict_dat_file );
        fwrite( frames, sizeof(PVOID), numEntries, s_memtrack_dict_dat_file );

#if MEMTRACK_OPTION_TEXT_DICT
        // use crashrpt.dll code to expand stack and write it out for debugging/development
        *s_memtrack_stack_text_buffer = 0;
        sdDumpStackToBuffer( s_memtrack_stack_text_buffer, sizeof(s_memtrack_stack_text_buffer), NULL );

        fprintf(s_memtrack_dict_txt_file, "0x%p\n", traceHash);
        fprintf(s_memtrack_dict_txt_file, "{{\n");
        fprintf(s_memtrack_dict_txt_file, s_memtrack_stack_text_buffer);
        fprintf(s_memtrack_dict_txt_file, "\n}}\n\n");
#endif
        s_callstacks_logged++;
    }

    return traceHash;
}

// log a memory manager event
void memtrack_log_memory_event(int eventType, void* pAllocatedBlock, size_t size, void* pAllocatedBlockPrev )
{
    if (s_memtrack_init && s_memtrack_capture_enabled && pAllocatedBlock)
    {
        EnterCriticalSection(&s_memtrack_lock);
        // avoid recursion because logging will go through wrapped mem mgr calls
        if (!s_memtrack_inside_logger)        
        {
            MemTrackEvent_Header head;
#if TIGHT_CRTDBG_INTEGRATION && _CRTDBG_MAP_ALLOC
            _CrtMemBlockHeader* memHeader;
#endif

            s_memtrack_inside_logger = true;

            head.event_type    = eventType;
            head.stack_id    = memtrack_log_callstack();
            head.thread_id    = GetCurrentThreadId();

#if TIGHT_CRTDBG_INTEGRATION
            if (eventType != kMemTrackEvent_Marker)
            {
#if _CRTDBG_MAP_ALLOC
                assert(_CrtIsValidHeapPointer(pAllocatedBlock));
                memHeader = (_CrtMemBlockHeader*)pAllocatedBlock - 1;

                head.request_id    = memHeader->lRequest;
#else
                head.request_id = 0;
#endif
            }
            else
            {
#if _CRTDBG_MAP_ALLOC
                memHeader = NULL;
#endif
                head.request_id    = 0;
            }
#else
            {
                static unsigned int s_request_id = 0;
                head.request_id = ++s_request_id;
            }
#endif

            switch (eventType)
            {
            case kMemTrackEvent_Alloc:
                {
                    MemTrackEvent_Alloc eb;

                    head.event_size = sizeof(head) + sizeof(eb);
#if TIGHT_CRTDBG_INTEGRATION && _CRTDBG_MAP_ALLOC
                    assert(size == memHeader->nDataSize);
#endif
                    eb.allocSize = size;
                    eb.pData     = pAllocatedBlock;

                    // write the full chunk, i.e., header and payload
                    fwrite( &head, sizeof(head), 1, s_memtrack_log_dat_file );
                    fwrite( &eb, sizeof(eb), 1, s_memtrack_log_dat_file );

                    #if MEMTRACK_OPTION_TEXT_LOG
                        memtrack_write_text_log_header( &head );
                        memtrack_write_text_log_alloc( &eb, memHeader->szFileName, memHeader->nLine );
                    #endif
                }
                break;
            case kMemTrackEvent_Realloc:
                {
                    MemTrackEvent_Realloc eb;

                    head.event_size = sizeof(head) + sizeof(eb);
#if TIGHT_CRTDBG_INTEGRATION && _CRTDBG_MAP_ALLOC
                    assert(size == memHeader->nDataSize);
#endif
                    eb.allocSize = size;
                    eb.pData     = pAllocatedBlock;
                    eb.pDataPrev = pAllocatedBlockPrev;

                    // write the full chunk, i.e., header and payload
                    fwrite( &head, sizeof(head), 1, s_memtrack_log_dat_file );
                    fwrite( &eb, sizeof(eb), 1, s_memtrack_log_dat_file );

                    #if MEMTRACK_OPTION_TEXT_LOG
                        memtrack_write_text_log_header( &head );
                        memtrack_write_text_log_realloc( &eb, memHeader->szFileName, memHeader->nLine );
                    #endif
                }
                break;
            case kMemTrackEvent_Free:
                {
                    MemTrackEvent_Free eb;

                    head.event_size = sizeof(head) + sizeof(eb);
                    eb.pData     = pAllocatedBlock;

                    // write the full chunk, i.e., header and payload
                    fwrite( &head, sizeof(head), 1, s_memtrack_log_dat_file );
                    fwrite( &eb, sizeof(eb), 1, s_memtrack_log_dat_file );

                    #if MEMTRACK_OPTION_TEXT_LOG
                        memtrack_write_text_log_header( &head );
                        memtrack_write_text_log_free( &eb, memHeader->szFileName, memHeader->nLine );
                    #endif
                }
                break;
            case kMemTrackEvent_Marker:
                {
                    MemTrackEvent_Marker eb;
                    MemTrack_MarkerData *md = (MemTrack_MarkerData*) pAllocatedBlock;

                    head.event_size = sizeof(head) + sizeof(eb);
                    eb.marker_type = md->marker_type;

                    // write the full chunk, i.e., header and payload
                    fwrite( &head, sizeof(head), 1, s_memtrack_log_dat_file );
                    fwrite( &eb, sizeof(eb), 1, s_memtrack_log_dat_file );
                    fwrite( md->marker_msg, strlen(md->marker_msg) + 1, 1, s_memtrack_log_dat_file );

                    #if MEMTRACK_OPTION_TEXT_LOG
                        memtrack_write_text_log_header( &head );
                        memtrack_write_text_log_marker( md );
                    #endif
                }
                break;
            }

            memtrack_flush_kick();
            s_events_logged++;
            s_memtrack_inside_logger = false;
        }
        LeaveCriticalSection(&s_memtrack_lock);
    }
}

#if TIGHT_CRTDBG_INTEGRATION && _CRTDBG_MAP_ALLOC
static _CRT_ALLOC_HOOK next_hook_func = NULL;

/*
    CRT allocation hook lets us easily capture information on memory manager events
    One caveat is that this hook happens before the event so in the case of an allocation
    the resulting allocation is not available (hence userData supplied to the hook is NULL)
    There is no 'post' hook available so we need manually make a call that records allocation
    addresses after the fact which then get matched up with the hook information during data
    analysis.
    
    So we don't use the hook for our primary capture mechanism and instead do the tracking
    events by wrapping the memory manager functions. But it can still be useful to generate
    'hook' events into our capture stream in order to debug/validate our wrappers. For example,
    if the normal event log is missing allocations we can use the hook events to help track
    down where they are coming from.
*/
int memtrack_hook_func( int allocType, void *userData, size_t size, int 
                        blockType, long requestNumber, const unsigned char *filename, int 
                        lineNumber)
{
    if (next_hook_func)
        next_hook_func(allocType, userData, size, blockType, requestNumber, filename, lineNumber);

    if (blockType == _CRT_BLOCK)
    {
        return TRUE;    // avoid recursion in handling blocks allocated by CRT
    }

    if (!s_memtrack_capture_enabled)    // don't record if capture is not enabled
    {
        return TRUE;
    }

    EnterCriticalSection(&s_memtrack_lock);
    // avoid recursion because logging will go through wrapped mem mgr calls
    if (!s_memtrack_inside_logger)        
    {
        MemTrackEvent_Header head;
        MemTrackEvent_Hook    eb;

        s_memtrack_inside_logger = true;

        head.event_type    = kMemTrackEvent_Hook;
        head.stack_id    = memtrack_log_callstack();
        head.request_id    = requestNumber;
        head.thread_id    = GetCurrentThreadId();


        head.event_size = sizeof(head) + sizeof(eb);
        eb.allocSize    = size;
        eb.pData        = userData;
        eb.allocType    = allocType;
        eb.blockType    = blockType;

        // As the hook is a 'pre' hook if the pData supplied will be for a block
        // to free or a block to realloc (alloc events haven't allocated yet so pData is NULL)
        if (userData)
        {
            _CrtMemBlockHeader* memHeader;
            assert(_CrtIsValidHeapPointer(userData));
            memHeader = (_CrtMemBlockHeader*)userData - 1;
            eb.blocks_request_id = memHeader->lRequest;
            eb.blocks_size = memHeader->nDataSize;
        }
        else
        {
            eb.blocks_request_id = 0;
            eb.blocks_size = 0;
        }

        // write the full chunk, i.e., header and payload
        fwrite( &head, sizeof(head), 1, s_memtrack_log_dat_file );
        fwrite( &eb, sizeof(eb), 1, s_memtrack_log_dat_file );

#if MEMTRACK_OPTION_TEXT_LOG
        memtrack_write_text_log_header( &head );
        memtrack_write_text_log_hook( &eb, filename, lineNumber );
#endif
        s_events_logged++;
        memtrack_flush_kick();
        s_memtrack_inside_logger = false;
    }
    LeaveCriticalSection(&s_memtrack_lock);

    return TRUE;
}
#endif


#if TIGHT_CRTDBG_INTEGRATION && _CRTDBG_MAP_ALLOC
void memtrack_crt_hook_install(void)
{
    if (memtrack_is_crt_hook_enabled())
    {
        // note that other threads might start taking the hook once we set it.
        next_hook_func = _CrtSetAllocHook(memtrack_hook_func);
    }
}
#endif // _CRTDBG_MAP_ALLOC

void memtrack_init(void)
{
    // should only be called by the main thread
    if (!s_memtrack_init)
    {
        const HMODULE hNtDll = GetModuleHandle("ntdll.dll");
        s_pfnCaptureStackBackTrace = (PFN_CaptureStackBackTrace)GetProcAddress(hNtDll, "RtlCaptureStackBackTrace");

        if (s_pfnCaptureStackBackTrace != 0)
        {
            time_t now;
            char the_date[MAX_PATH] = "";
            char filename[256];

            // add date/frame to filename so don't overwrite old results
            now = time(NULL);
            if (now != -1)
            {
                struct tm newtime;
                gmtime_s(&newtime,&now);
                strftime(the_date, MAX_PATH, "%Y%m%d_%H%M%S", &newtime );
            }

            snprintf(filename, sizeof(filename), kLogFileNameBinary, the_date );
            s_memtrack_log_dat_file = fopen( filename, "wb" );

            snprintf(filename, sizeof(filename), kDictFileNameBinary, the_date );
            s_memtrack_dict_dat_file = fopen( filename, "wb" );

#if MEMTRACK_OPTION_TEXT_LOG
            snprintf(filename, sizeof(filename), kLogFileNameText, the_date );
            s_memtrack_log_txt_file = fopen( filename, "wt" );
#endif
#if MEMTRACK_OPTION_TEXT_DICT
            snprintf(filename, sizeof(filename), kDictFileNameText, the_date );
            s_memtrack_dict_txt_file = fopen( filename, "wt" );
#endif

            // allocate hash table to track callstack signatures
            s_memtrack_callstack_dictionary = stashTableCreateInt(2000);    // @todo good initial size choice? growth?

            InitializeCriticalSection(&s_memtrack_lock);

            //@todo the files should get headers written with format versions, etc.
            {
                unsigned int version = 0x1;
                fwrite( &version, sizeof(version), 1, s_memtrack_log_dat_file );
                fwrite( &version, sizeof(version), 1, s_memtrack_dict_dat_file );
            }
            //@todo first event written should be a setup event which has module information
            // for symbol lookup and thread id symbol information, etc.

            s_memtrack_init = true;
            
#if TIGHT_CRTDBG_INTEGRATION && _CRTDBG_MAP_ALLOC
            // install optional hook on CRT?
            memtrack_crt_hook_install();
#endif

#if MEMTRACK_AUTO_BEGIN
            memtrack_capture_enable(true);    // begin capture immediately after initialization
#endif
        }
        else
        {
            printf("ERROR: detailed memory event logging could not be enabled\n");
            s_memtrack_init = false;
        }
    }
}

void memtrack_capture_enable(bool bEnableCapture)
{
    s_memtrack_capture_enabled = bEnableCapture;
}

void memtrack_flush(void)
{
    if (s_memtrack_init)
    {
        fflush( s_memtrack_log_dat_file );
        fflush( s_memtrack_dict_dat_file );
#if MEMTRACK_OPTION_TEXT_LOG
        fflush( s_memtrack_log_txt_file );
#endif
#if MEMTRACK_OPTION_TEXT_DICT
        fflush( s_memtrack_dict_txt_file );
#endif
    }
}

bool memtrack_enabled(void)
{
    return s_memtrack_init;
}

bool memtrack_is_crt_hook_enabled(void)
{
    return s_memtrack_init && s_memtrack_use_crt_hook;
}

void memtrack_alloc( void* pAllocatedBlock, size_t size )
{
    memtrack_log_memory_event( kMemTrackEvent_Alloc, pAllocatedBlock, size, NULL );
}

void memtrack_realloc( void* pAllocatedBlock, size_t size, void* pPrevBlock )
{
    memtrack_log_memory_event( kMemTrackEvent_Realloc, pAllocatedBlock, size, pPrevBlock );
}

void memtrack_free( void* pAllocatedBlock )
{
    memtrack_log_memory_event( kMemTrackEvent_Free, pAllocatedBlock, 0, NULL );
}

void memtrack_mark( int type, const char* msg )
{
    MemTrack_MarkerData md;

    md.marker_type = type;
    md.marker_msg = msg;

    memtrack_log_memory_event( kMemTrackEvent_Marker, &md, 0, NULL );
}

#if !_CRTDBG_MAP_ALLOC
#undef malloc
#undef calloc
#undef realloc
#undef strdup
#undef free

void *memtrack_do_malloc(size_t size)
{
    void *ptr = malloc(size);
    memtrack_alloc(ptr, size);
    return ptr;
}

void *memtrack_do_calloc(size_t num, size_t size)
{
    void *ptr = calloc(num,size);
    memtrack_alloc(ptr, num*size);
    return ptr;
}

void *memtrack_do_realloc(void *old_ptr, size_t size)
{
    void *ptr = realloc(old_ptr,size);
    memtrack_realloc(ptr, size, old_ptr);
    return ptr;
}

char *memtrack_do_strdup(const char *s)
{
    size_t len = strlen(s)+1;
    char *copy = malloc(len);
    memcpy(copy, s, len);
    memtrack_alloc(copy, len);
    return copy;
}

void  memtrack_do_free(void *ptr)
{
    free(ptr);
    memtrack_free(ptr);
}

#endif // !_CRTDBG_MAP_ALLOC

#endif // MEMTRACK_BUILD
