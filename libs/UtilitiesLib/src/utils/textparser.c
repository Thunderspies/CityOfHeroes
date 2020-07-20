// textparser.c - provides text and token processing functions
// NOTE - the textparser can NOT be used from a second thread.
// Please let Mark know if this needs to be changed

#include "utilitieslib/utils/superassert.h"
#include "utilitieslib/components/memorypool.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/components/earray.h"
#include <stdio.h>
#include <string.h>
#include "utilitieslib/utils/fileutil.h"
#include <time.h>
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/serialize.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/qsortG.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/utils/quat.h"
#include "utilitieslib/utils/structInternals.h"
#include "utilitieslib/utils/tokenstore.h"
#include "utilitieslib/utils/structTokenizer.h"
#include "utilitieslib/utils/structDefines.h"
#include "utilitieslib/network/netio.h"
#include "utilitieslib/utils/structNet.h"
#include "utilitieslib/components/referencesystem.h"        
#include <limits.h>

// project specific
#include "utilitieslib/network/crypt.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/network/sock.h"
#include "utilitieslib/utils/UnitSpec.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/MemoryMonitor.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/components/stringtable.h"
#include "utilitieslib/components/SharedMemory.h"
#include "utilitieslib/components/SharedHeap.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/FolderCache.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/Estring.h"
#include "utilitieslib/utils/rand.h"
#include "utilitieslib/utils/endian.h"

#undef ParserAllocStruct

// Forward declares:
void nonarray_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed);


#if PRIVATE_PARSER_HEAPS
static int heap_flags = 0; // thread-safe, resizable
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// StructLink cache tracking and clearing
static StashTable structlink_cache_tracker = 0;

static void AddToStructLinkCacheTracker(struct StructLinkCache *pCache)
{
    if (!structlink_cache_tracker)
    {
        structlink_cache_tracker = stashTableCreateAddress(32);
    }

    // If you do a find on the stash to see if it exists before adding,
    // the add does a find anyways.  So, just do the add.
    stashAddressAddPointer(structlink_cache_tracker, pCache, pCache, false);
}

// Forward declare this function
static void linkcacheelt_Destroy( void *elt );

static void structLinkCache_destroy(void *elt)
{
    struct StructLinkCache *pCache = elt;

    stashTableDestroyEx(pCache->hashPtrFromPath, NULL, linkcacheelt_Destroy );
    stashTableDestroy(pCache->hashPathFromPtr);

    pCache->hashPathFromPtr = 0;
    pCache->hashPtrFromPath = 0;
}

void TextParser_ClearCaches()
{
    stashTableDestroyEx(structlink_cache_tracker, NULL, structLinkCache_destroy);
    structlink_cache_tracker = 0;
}
        
///////////////////////////////////////////////////////////////////////////////////////// Parser

// clear any remaining tokens on a single line
// don't eat the eol
static void ClearParameters(TokenizerHandle tok)
{
    const char* nexttoken;
    while (1)
    {
        nexttoken = TokenizerPeek(tok, 0, 1);
        if (!nexttoken || IsEol(nexttoken)) return;
        TokenizerGet(tok, 0, 1);    // eat the token
    }
}

// report error if we don't have another parameter
static const char* GetNextParameter(TokenizerHandle tok)
{
    const char *result = TokenizerPeek(tok, 0, 1);
    if (!result || IsEol(result))
    {
        int line = TokenizerGetCurLine(tok);
        ErrorFilenamef(TokenizerGetFileName(tok),
            "Parser: missing parameter value in %s, line %i\n", TokenizerGetFileName(tok), line);
        g_parseerr = 1;
        return NULL;
    }
    else
    {
        TokenizerGet(tok, 0, 1); // eat token
    }
    return result;
}

// just reports with error message if there are extra parameters
static void VerifyParametersClear(TokenizerHandle tok)
{
    const char* nexttoken = TokenizerGet(tok, 0, 1);
    if (!nexttoken || IsEol(nexttoken)) return; // fine

    // get rid of any extra params
    ErrorFilenamef(TokenizerGetFileName(tok),
        "Parser: extra parameter value %s in %s\n", nexttoken, TokenizerGetFileAndLine(tok));
    ClearParameters(tok);
    g_parseerr = 1;
    TokenizerGet(tok, 0, 1); // clear eol
}

// parse the token into an integer, reports errors
static S64 GetInteger64(TokenizerHandle tok, const char* token)
{
    char* end;
    S64 result;

    errno = 0;
    result = _strtoi64(token, &end, 10);
    if (errno || *end)
    {
        ErrorFilenamef(TokenizerGetFileName(tok),
            "Parser: got %s, expected integer value in %s\n", token, TokenizerGetFileAndLine(tok));
        g_parseerr = 1;
    }
    return result;
}

static int GetInteger(TokenizerHandle tok, const char* token)
{
    char* end;
    int result;

    errno = 0;
    result = (int)strtol(token, &end, 10);
    if (errno || *end)
    {
        ErrorFilenamef(TokenizerGetFileName(tok),
            "Parser: got %s, expected integer value in %s\n", token, TokenizerGetFileAndLine(tok));
        g_parseerr = 1;
    }
    return result;
}

// parse the token into an IP, reports errors
static int GetIP(TokenizerHandle tok, const char* token)
{
    int result;

    result = ipFromString(token);
    if (!result && stricmp(token, "0.0.0.0")!=0)
    {
        ErrorFilenamef(TokenizerGetFileName(tok),
            "Parser: got %s, expected IP String in the form of x.x.x.x in %s\n", token, TokenizerGetFileAndLine(tok));
        g_parseerr = 1;
    }
    return result;
}

// parse the token into an unsigned integer, reports errors
static unsigned int GetUInteger(TokenizerHandle tok, const char* token)
{
    char* end;
    unsigned int result;

    errno = 0;
    result = (unsigned int)strtoul(token, &end, 10);
    if (errno || *end)
    {
        ErrorFilenamef(TokenizerGetFileName(tok),
            "Parser: got %s, expected integer value in %s\n", token, TokenizerGetFileAndLine(tok));
        g_parseerr = 1;
    }
    return result;
}

// parse the token into an float, reports errors
static float GetFloat(TokenizerHandle tok, const char* token)
{
    char* end;
    float result;

    errno = 0;
    result = (float)strtod(token, &end);
    if (errno || *end)
    {
        ErrorFilenamef(TokenizerGetFileName(tok),
            "Parser: got %s, expected float value in %s\n", token, TokenizerGetFileAndLine(tok));
        g_parseerr = 1;
    }
    return result;
}

static void TokenizerParseFunctionCall(TokenizerHandle tok, StructFunctionCall*** callstructs)
{
    StructFunctionCall* nextstruct;
    const char* nexttoken;
    int length, n;

    while (1)
    {
        nexttoken = TokenizerPeekEx(tok, 0, 1, "()", &length);
        if (!nexttoken || IsEol(nexttoken)) break;
        if (nexttoken[0] == ')' && length == 1)
        {
            TokenizerGetEx(tok, 0, 1, "()", &length); // eat it, and end recursion
            return;
        }

        // have a string, or an open paren now, going to need at least one struct
        if (!*callstructs)
            eaCreate(callstructs);
        nexttoken = TokenizerGetEx(tok, 0, 1, "()", &length); // eat the token

        // open paren means we need to recurse
        if (nexttoken[0] == '(' && length == 1)
        {
            n = eaSize(callstructs);
            if (!n) // error if we didn't actually get a function name
            {
                ErrorFilenamef(TokenizerGetFileName(tok),
                    "Parser: open paren without function name in %s\n", TokenizerGetFileAndLine(tok));
                g_parseerr = 1;
                ClearParameters(tok); // just clear to eol
                return;
            }
            TokenizerParseFunctionCall(tok, &((*callstructs)[n-1]->params));
        }
        else
        {
            // otherwise, add the string like we were a string array
            nextstruct = StructAllocRawDbg(sizeof(*nextstruct), parser_relpath, -1);
            nextstruct->function = StructAllocStringLen(nexttoken, length);
            eaPush(callstructs, nextstruct);
        }
    }
}

#if PRIVATE_PARSER_HEAPS
//////////////////////////////////////////////////////////////////////////
// Memory tracking
//////////////////////////////////////////////////////////////////////////
static StashTable alloc_struct_tracker = 0;

typedef struct ParserTracker
{
    int alloc_count, free_count;
    size_t total_size;
} ParserTracker;

MP_DEFINE(ParserTracker);

typedef struct StructAlloc
{
    const char *str;
    ParserTracker *track;
} StructAlloc;

static StructAlloc **alloc_array = 0;
static int printStructAlloc(StashElement elem)
{
    ParserTracker *track;
    StructAlloc *sa;
    if (!(track = stashElementGetPointer(elem)))
        return 1;
    sa = malloc(sizeof(StructAlloc));
    sa->str = stashElementGetStringKey(elem);
    sa->track = track;
    eaPush(&alloc_array, sa);
    return 1;
}

static int sacmp(const StructAlloc **sa1, const StructAlloc **sa2)
{
    if ((*sa1)->track->total_size < (*sa2)->track->total_size)
        return -1;
    return (*sa1)->track->total_size > (*sa2)->track->total_size;
}

void StructDumpAllocs(void)
{
    int i;
    size_t total = 0, overhead_total = 0;

    eaCreate(&alloc_array);
    stashForEachElement(alloc_struct_tracker, printStructAlloc);
    qsortG(alloc_array, eaSize(&alloc_array), sizeof(*alloc_array), sacmp);

    printf("\n\n");
    printf("% -50s  % 12s     % 8s  % 8s  % 20s\n", "ParserAllocStruct Source", "Size", "Allocs", "Frees", "Tracker Overhead");
    printf("-------------------------------------------------------------------------------------------------------------\n");
    for (i = 0; i < eaSize(&alloc_array); i++)
    {
        int overhead;
        ParserTracker *track = alloc_array[i]->track;
        const char *s = strrchr(alloc_array[i]->str, '/');
        if (!s)
            s = strrchr(alloc_array[i]->str, '\\');
        if (s)
            s++;
        else
            s = alloc_array[i]->str;
        total += track->total_size;
        overhead = (track->alloc_count - track->free_count) * 4;
        overhead_total += overhead;
        printf("% -50s  % 12s     % 8d  % 8d  % 20d\n", s, friendlyBytes(track->total_size), track->alloc_count, track->free_count, overhead);
    }
    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("% -50s  % 12s     % 8s  % 8s  % 20Id\n", "Total:", friendlyBytes(total), "", "", overhead_total);

    eaDestroyEx(&alloc_array, NULL);
}

static char* trackParserMemAlloc(const char *file, int line, const char *structname, size_t size)
{
    static char buf[1024];
    ParserTracker *track;
    char* ret = 0;
    buf[0] = 0;

    STR_COMBINE_BEGIN(buf);
    if (!file)
    {
        STR_COMBINE_CAT("Unknown");
    }
    else
    {
        STR_COMBINE_CAT(file);
        if (line > 0)
        {
            STR_COMBINE_CAT(" (");
            STR_COMBINE_CAT_D(line);
            STR_COMBINE_CAT(")");
        }
    }
    if (structname)
    {
        STR_COMBINE_CAT(" <");
        STR_COMBINE_CAT(structname);
        STR_COMBINE_CAT(">");
    }
    STR_COMBINE_END();

    stashFindPointer( alloc_struct_tracker, buf, &track );
    if (!track)
    {
        MP_CREATE(ParserTracker, 4096);
        track = MP_ALLOC(ParserTracker);
        stashAddPointer(alloc_struct_tracker, buf, track, false);
    }
    track->alloc_count++;
    track->total_size += size;

    stashGetKey(alloc_struct_tracker, buf, &ret);
    return ret;
}

static void trackParserMemFree(char* key, size_t size)
{
    ParserTracker *track = stashFindPointerReturnPointer(alloc_struct_tracker, key);
    if (!track)
    {
        MP_CREATE(ParserTracker, 4096);
        track = MP_ALLOC(ParserTracker);
        stashAddPointer(alloc_struct_tracker, key, track, false);
    }
    track->free_count++;
    track->total_size -= size;
}

#else

void StructDumpAllocs(void)
{
    printf("\n\nNot using private parser heaps.\n");
}

#endif

//////////////////////////////////////////////////////////////////////////

// globals
const char *parser_relpath = 0;
const char *parser_structname = 0;

int g_parseerr = 0;    // util functions used by ParserReadTokenizer may set this
FileList g_parselist = 0; // if non-zero, tokenizer will keep track of every file parsed


////////////////////////////////////////////////////////////////////////////////////// StructLink util functions

static int makeLinkStringRecur(StructLinkOffset *parse_link,U8 **earray,void *ptr,char *link_str,size_t link_str_size)
{
    int        size,i;
    char    *name;
    char    **subarray,*str_end;
    size_t    link_str_len = strlen(link_str);

    str_end = link_str_len + link_str;
    size = eaSize(&earray);
    for(i=size-1;i>=0;i--)
    {
        name = *(char **)(earray[i] + parse_link->name_offset);
        strcpy_s(str_end, link_str_size - link_str_len, name);
        if (ptr == earray[i])
            return 1;
        if (parse_link->struct_offset)
        {
            strcat_s(link_str, link_str_size, ".");
            subarray = *(char ***)(earray[i] + parse_link->struct_offset);
            if (makeLinkStringRecur(parse_link+1,subarray,ptr,link_str, link_str_size))
                return 1;
        }
    }
    return 0;
}

typedef struct LinkCacheElt
{
    union
    {
        char ***psubarray;
        void *value;
    };
    StashTable hashChildren;
} LinkCacheElt;

MP_DEFINE(LinkCacheElt);
static LinkCacheElt* linkcacheelt_Create( char ***psubarray )
{
    LinkCacheElt *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(LinkCacheElt, 64);
    res = MP_ALLOC( LinkCacheElt );
    if( verify( res ) && psubarray )
    {
        res->psubarray = psubarray;
        res->hashChildren = stashTableCreateWithStringKeys( 32, StashDeepCopyKeys );
    }

    // --------------------
    // finally, return

    return res;
}

static void linkcacheelt_Destroy( void *elt )
{
    LinkCacheElt *hItem = elt;
    if(verify(hItem))
    {
        if (hItem->hashChildren)
            stashTableDestroyEx( hItem->hashChildren, NULL, linkcacheelt_Destroy );

        MP_FREE(LinkCacheElt, hItem);
    }
}

static void *findLinkFromStringRecur(StructLinkOffset *parse_link,U8 **earray,char *link_str, StashTable cachedLinks)
{
    int        size,i;
    char    *name,*next_str = NULL;
    char    ***psubarray;
    LinkCacheElt *cachedLink = NULL;

    // --------------------
    // get the next string, and truncate it
    // unless this is a leaf node, in which case we shouldn't split it any more

    if (parse_link->struct_offset != 0)
    {    
        next_str = strchr(link_str,'.');
        if (next_str)
            *next_str++ = 0;
    }

    // --------------------
    // check if link string is already cached

    stashFindPointer(  cachedLinks, link_str , &cachedLink );
     if( cachedLink )
     {
         return !parse_link->struct_offset || !next_str ?
             cachedLink->value
             : findLinkFromStringRecur(parse_link+1,*(cachedLink->psubarray),next_str, cachedLink->hashChildren );
     }

    size = eaSize(&earray);
    for(i=size-1;i>=0;i--)
    {
        name = *(char **)(earray[i] + parse_link->name_offset);
        if (stricmp(name,link_str)!=0)
            continue;

        // check if any other labels are identical, they
        // will be shadowed by the current one otherwise
        {
            int j;
            for(j=i-1;j>=0;--j)
            {
                char *tmpname = *(char **)(earray[j] + parse_link->name_offset);
                if(stricmp(tmpname,link_str)==0)
                {
                    Errorf("Tokenizer: label '%s' is not unique in link path.", tmpname);
                }
            }
        }

        if (!parse_link->struct_offset || !next_str)
        {
            cachedLink = linkcacheelt_Create( NULL );
            cachedLink->value = earray[i];
            stashAddPointer(cachedLinks, name, cachedLink , false);
            return cachedLink->value;
        }
        psubarray = (char ***)(earray[i] + parse_link->struct_offset);

        // create the hash for this
        cachedLink = linkcacheelt_Create( psubarray );
        stashAddPointer(cachedLinks, name, cachedLink , false);

        // pass it to the child calls
        return findLinkFromStringRecur(parse_link+1,*psubarray,next_str, cachedLink->hashChildren);
    }
    return 0;
}

#ifndef FINAL
static void s_ParserLinkFromStringBreakpoint(char const *link_str_p)
{
    if( isDevelopmentMode() && strstr(link_str_p, "!!S_AeonTech") )
    {
        //__asm {int 3};
        printf("don't want to break into the debugger if not debugging\n");
    }
}
#endif

char *StructLinkToString(StructLink *struct_link,void *ptr,char *link_str,size_t link_str_size)
{
    char *tmp;
    link_str[0] = 0;

    // check the hashtable first
    if(struct_link->cache.hashPathFromPtr && stashAddressFindPointer( struct_link->cache.hashPathFromPtr, ptr, &tmp))
    {
#ifndef FINAL
        s_ParserLinkFromStringBreakpoint(tmp);
#endif
        strcpy_s(link_str,link_str_size,tmp);
        return tmp; // BREAK IN FLOW
    }
    else if (makeLinkStringRecur(struct_link->links,(U8**)*struct_link->earray_addr,ptr,link_str,link_str_size))
    {
#ifndef FINAL
        s_ParserLinkFromStringBreakpoint(link_str);
#endif
        return link_str;
    }

    return 0;
}


void *StructLinkFromString(StructLink *struct_link,char const *link_str_p)
{
    char    link_str[128];
    void    *link = NULL;

#ifndef FINAL
    s_ParserLinkFromStringBreakpoint(link_str_p);
#endif

    if(devassert(ARRAY_SIZE(link_str) > strlen(link_str_p)))
    {
        if( !struct_link->cache.hashPtrFromPath )
        {
            struct_link->cache.hashPtrFromPath = stashTableCreateWithStringKeys( 128, StashDeepCopyKeys );
            struct_link->cache.hashPathFromPtr = stashTableCreateAddress(1000);
        }

        // --------------------
        // find the link

        strcpy(link_str,link_str_p);
        link = findLinkFromStringRecur(struct_link->links,(U8**)*struct_link->earray_addr,link_str, struct_link->cache.hashPtrFromPath);

        // --------------------
        // add it to the cache, if found and not already there.

        if( link && !stashAddressFindPointer(struct_link->cache.hashPathFromPtr, link, NULL) )
        {
            char *path = strdup(link_str_p);
            stashAddressAddPointer(struct_link->cache.hashPathFromPtr, link, path, false);
        }

        // Add the cache to our tracker so we can clear it later
        AddToStructLinkCacheTracker(&struct_link->cache);
    }

    return link;
}

////////////////////////////////////////////////////////////////////////////////////// Parser strings and structs
void*   StructCreate_dbg(ParseTable pti[], const char *file, int line)
{
    ParseTableInfo *info = ParserGetTableInfo(pti);
    void *created = StructAlloc_dbg(pti,info,file,line);
//     void *pTemplate = NULL;

    if (created)
        StructInitFields(pti,created);

// ab: not supported: a 'template' struct is one that all structs are copied
//     from when created
//     if (pTemplate = info->pTemplateStruct)
//         StructCopyFields(pti, pTemplate, created, 0, 0);

    return created;
}

#if PRIVATE_PARSER_HEAPS
HANDLE g_parserheap = 0;
HANDLE g_stringheap = 0;
extern int heapGetSize(HANDLE hHeap);
static const char STRUCT_MEMMONITOR_NAME[] = "StructAlloc";
static const char STRING_MEMMONITOR_NAME[] = "StructAllocString";
#endif

int ParserValidateHeap()
{
    int ret1=1, ret2=1;
#if PRIVATE_PARSER_HEAPS
    if (g_parserheap)
        ret1 = HeapValidate(g_parserheap,heap_flags,0);
    if (g_stringheap)
        ret2 = HeapValidate(g_stringheap,heap_flags,0);
#endif
    return ret1 && ret2;
}

void*    StructAlloc_dbg(ParseTable pti[], ParseTableInfo *optPtr, const char *file, int line)
{
    int iSize;
    const char *pName;

    if(!optPtr)
        optPtr = ParserGetTableInfo(pti);
    if(!optPtr)
    {
        devassertmsg(0,"no parsetableinfo found for pti");
        return NULL;
    }
    
    pName = optPtr->name;
    iSize = optPtr->size;

    if (!iSize)
    {
        devassertmsg(0, "StructAlloc called on a struct that wasn't initialized with ParserSetTableInfo!");
        return 0;
    }

    return StructAllocRawDbg(iSize, pName ? pName : file, line);
}

// allocate memory for a structure
void*    StructAllocRawDbg(size_t size, const char *file, int line)
{
#if PRIVATE_PARSER_HEAPS
    char *data;
    size += sizeof(size_t);    // room for a 64-bit pointer

    if(!g_parserheap)
    {
        g_parserheap = HeapCreate(heap_flags, 0, 0);
        disableRtlHeapChecking(g_parserheap);
        alloc_struct_tracker = stashTableCreateWithStringKeys(4096, StashDeepCopyKeys | StashCaseSensitive );
    }

    memMonitorTrackUserMemory(STRUCT_MEMMONITOR_NAME, 1, MOT_ALLOC, size);
    data = HeapAlloc(g_parserheap, HEAP_ZERO_MEMORY, size);
    *((size_t*)data) = (size_t)trackParserMemAlloc(file, line, parser_structname, size);

    return data+sizeof(size_t);
#else
    return _calloc_dbg(1, size, _NORMAL_BLOCK, file, line);
#endif
}

// release memory for a structure
void    StructFree(void* structptr)
{
    if (!structptr)
        return;
#if PRIVATE_PARSER_HEAPS
    structptr = ((char *)structptr) - sizeof(size_t);
    if (memMonitorActive()) {
        size_t size = HeapSize(g_parserheap, heap_flags, structptr);
        assert(size < 0x10000000); // Either you're freeing 256MB of data, or more likely you're freeing something allocated off the Crt heap
        memMonitorTrackUserMemory(STRUCT_MEMMONITOR_NAME, 1, MOT_FREE, size);
        trackParserMemFree((char*)(*((size_t *)structptr)), size);
    }

    HeapFree(g_parserheap, 0, structptr);
#else
    _free_dbg(structptr, _NORMAL_BLOCK);
#endif
}

// allocate memory for a string and copy parameter in
char* StructAllocStringLenDbg(const char* string, int len, const char *file, int line)
{
    char* ret;
    if(!string)
        return NULL;
    if(len < 0)
        len = (int)strlen(string);
#if PRIVATE_PARSER_HEAPS
    if(!g_stringheap)
    {
        g_stringheap = HeapCreate(heap_flags, 0, 0);
        disableRtlHeapChecking(g_stringheap);
    }
    ret = HeapAlloc(g_stringheap, 0, len+1);
    assert(ret);
    memMonitorTrackUserMemory(STRING_MEMMONITOR_NAME, 1, MOT_ALLOC, len+1);
#else
    ret = _malloc_dbg(len+1, _NORMAL_BLOCK, file, line);
#endif
    strncpy_s(ret, len+1, string, len);
    return ret;
}

// release memory for a string
void StructFreeString(char const* string)
{
    if (!string)
        return;
#if PRIVATE_PARSER_HEAPS
    if (memMonitorActive()) {
        size_t size = HeapSize(g_stringheap, heap_flags, string);
        assert(size < 0x10000000); // Either you're freeing 256MB of data, or more likely you're freeing something allocated off the Crt heap
        memMonitorTrackUserMemory(STRING_MEMMONITOR_NAME, 1, MOT_FREE, size);
    }
    //assert(!isSharedMemory(string));
    HeapFree(g_stringheap, 0, string);
#else
    _free_dbg((void*)string, _NORMAL_BLOCK);
#endif
}

void StructFreeStringConst(const char* string)
{
    StructFreeString((char*)string);
}

void StructReallocStringDbg(char **ppch, const char *string, const char *file, int line)
{
    assert(ppch);
    if(!*ppch)
    {
        *ppch = StructAllocString(string);
    }
    else if(!string)
    {
        StructFreeString(*ppch);
        *ppch = NULL;
    }
    else
    {
        size_t len = strlen(string);
#if PRIVATE_PARSER_HEAPS
        if(memMonitorActive())
        {
            size_t size = HeapSize(g_stringheap, heap_flags, *ppch);
            assert(size < 0x10000000); // Either you're freeing 256MB of data, or more likely you're freeing something allocated off the Crt heap
            memMonitorTrackUserMemory(STRING_MEMMONITOR_NAME, 1, MOT_REALLOC, len+1 - size);
        }
        //assert(!isSharedMemory(string));
        *ppch = HeapReAlloc(g_stringheap, 0, *ppch, len+1);
        assert(*ppch);
#else
        *ppch = _realloc_dbg(*ppch, len+1, _NORMAL_BLOCK, file, line);
#endif
        strncpy_s(*ppch, len+1, string, len);
    }
}

void StructFreeFunctionCall(StructFunctionCall* callstruct)
{
    if (callstruct->function)
        StructFreeString(callstruct->function);
    if (callstruct->params)
    {
        eaDestroyEx(&callstruct->params, StructFreeFunctionCall);
    }
    StructFree(callstruct);
}

// fill in default values for a newly created struct
void StructInit(void* structptr, size_t size, ParseTable pti[])
{
    int i;

    TEST_PARSE_TABLE(pti);
    if (size)
        memset(structptr, 0, size);
    FORALL_PARSEINFO(pti, i)
    {
        TOKARRAY_INFO(pti[i].type).initstruct(pti, i, structptr, 0);
    } // each token
}

void StructInitFields(ParseTable pti[], void* structptr)
{
//    TextParserAutoFixupCB *pFixupCB;
    int i;
    FORALL_PARSETABLE(pti, i)
    {
        TOKARRAY_INFO(pti[i].type).initstruct(pti, i, structptr, 0);
    } // each token

//    pFixupCB = ParserGetTableFixupFunc(pti);

//    if (pFixupCB)
//    {
//        pFixupCB(structptr, FIXUPTYPE_CONSTRUCTOR);
//    }
}

// free a tree of structures created by parser
void StructClear(ParseTable pti[], void* structptr)
{
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        TOKARRAY_INFO(pti[i].type).destroystruct(pti, i, structptr, 0);
    }
}

void StructDestroy(ParseTable pti[], void* structptr)
{
    StructClear(pti,structptr);
    StructFree(structptr);
}

static void ParserUpdateCRCFunctionCall(StructFunctionCall* callstruct)
{
    int len, i;

    if (callstruct->function)
    {
        len = (int)strlen(callstruct->function);
        cryptAdler32Update(callstruct->function, len);
    }
    else
        cryptAdler32Update((U8*)&callstruct->function, sizeof(char*)); // zero

    for (i = 0; i < eaSize(&callstruct->params); i++)
    {
        ParserUpdateCRCFunctionCall(callstruct->params[i]);
    }
}

// used by StructCRC
static void StructUpdateCRC(ParseTable pti[], void* structptr)
{
    int i;
    // look through token to crc each data member
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue;
        TOKARRAY_INFO(pti[i].type).updatecrc(pti, i, structptr, 0);
    }
}

// get a CRC of all data in structure tree
U32 StructCRC(ParseTable pti[], void* structptr)
{
    cryptAdler32Init();
    StructUpdateCRC(pti, structptr);
    return cryptAdler32Final();
}

static int cmp8(U8 *v0, U8 *v1, int len) 
{
    int i, ret = 0;
    for (i = 0; i < len && ret == 0; i++)
    {
        ret = v0[i] - v1[i];
    }
    return ret;
}

int TokenCompare(ParseTable tpi[], int column, void *structptr1, void *structptr2)
{
    return TOKARRAY_INFO(tpi[column].type).compare(tpi, column, structptr1, structptr2, 0);
}

int StructCompare(ParseTable pti[], void *structptr1, void *structptr2)
{
    int i;

    FORALL_PARSEINFO(pti, i)
    {
        int ret = TokenCompare(pti, i, structptr1, structptr2);
        if (ret)
            return ret;
    }
    return 0;
}


/////////////////////////////////////////////////// Parser structure copying (for shared memory)
static size_t ParserGetFunctionCallMemoryUsage(StructFunctionCall* callstruct)
{
    int i;
    size_t memory_usage = sizeof(StructFunctionCall);
    if (callstruct->function)
        memory_usage += (int)strlen(callstruct->function) + 1;
    if (callstruct->params)
    {
        for (i = 0; i < eaSize(&callstruct->params); i++)
            memory_usage += ParserGetFunctionCallMemoryUsage(callstruct->params[i]);
        memory_usage += eaMemUsage(&callstruct->params);
    }
    return memory_usage;
}

size_t StructGetMemoryUsage(ParseTable pti[], const void* structptr, size_t iSize) // Counts how much memory is used to store an entire structure
{
    int i;
    size_t memory_usage = iSize; // Start out with the size of the structure itself
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME)
            continue;
        memory_usage += TOKARRAY_INFO(pti[i].type).memusage(pti, i, (void*)structptr, 0);
    }
    return memory_usage;
}

int StructCopyAll(ParseTable pti[], const void* source, size_t size, void* dest)
{
    StructClear(pti, dest);
    return (StructCompress(pti, source, size, dest, NULL, NULL))? 1: 0;
}

// int StructCopyFields(ParseTable pti[], const void* source, void* dest, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
// {
//     int i;
//     assert(source && dest);

//     FORALL_PARSETABLE(pti, i)
//     {
//         if (pti[i].type & TOK_REDUNDANTNAME)
//         {
//             continue;
//         }
//         if (!FlagsMatchAll(pti[i].type,iOptionFlagsToMatch))
//         {
//             continue;
//         }

//         if (!FlagsMatchNone(pti[i].type,iOptionFlagsToExclude))
//         {
//             continue;
//         }
//         TOKARRAY_INFO(pti[i].type).copyfield(pti, i, dest, (void*)source, 0, NULL, NULL, iOptionFlagsToMatch,iOptionFlagsToExclude);
//     }
//     return 1;
// }

int StructCopy(ParseTable pti[], const void* source, void* dest, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int i;
    assert(source && dest);

    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME)
        {
            continue;
        }
        if (iOptionFlagsToMatch && !(pti[i].type & iOptionFlagsToMatch))
        {
            continue;
        }
        if (iOptionFlagsToMatch && (pti[i].type & iOptionFlagsToExclude))
        {
            continue;
        }
        TOKARRAY_INFO(pti[i].type).copyfield(pti, i, dest, (void*)source, 0, NULL, NULL, iOptionFlagsToMatch,iOptionFlagsToExclude);
    }
    return 1;
}

static StructFunctionCall* ParserCompressFunctionCall(StructFunctionCall* src, CustomMemoryAllocator memAllocator, void* customData)
{
    StructFunctionCall* dst;
    int len, i;

    // allocate struct
    if (memAllocator)
        dst = memAllocator(customData, sizeof(StructFunctionCall));
    else
        dst = StructAllocRawDbg(sizeof(StructFunctionCall), parser_relpath, -1);
    if (!dst)
        return NULL;

    // copy string
    if (src->function)
    {
        if (memAllocator)
        {
            len = (int)strlen(src->function)+1;
            dst->function = memAllocator(customData, len);
            if (!dst->function)
                return dst;
            strcpy_s(dst->function, len, src->function);
        }
        else
            dst->function = StructAllocString(src->function);
    }

    // copy substructures
    if (src->params)
    {
        eaCompress(&dst->params, &src->params, memAllocator, customData);
        for (i = 0; i < eaSize(&src->params); i++)
        {
            dst->params[i] = ParserCompressFunctionCall(src->params[i], memAllocator, customData);
        }
    }
    return dst;
}

// MAK - made this work with normal ParserXxxStruct semantics when memAllocator is NULL
void *StructCompress(ParseTable pti[], const void* pSrcStruct, size_t iSize, void *pDestStruct, CustomMemoryAllocator memAllocator, void *customData)  // Returns the new structure, or NULL if copying failed (ran out of memory?)
{
    int i;

    assert(iSize || pDestStruct);
    assert(pSrcStruct != pDestStruct);
    if (!pDestStruct) {
        if (memAllocator)
            pDestStruct = memAllocator(customData, iSize);
        else
            pDestStruct = StructAllocRawDbg(iSize, parser_relpath, -1);
        if (!pDestStruct)
            return NULL;
    }

    if (iSize) {
        // Copy the "static" members of the structure
        memcpy(pDestStruct, pSrcStruct, iSize);
        // This isn't done with embedded structs, since the layer above already copied the data
    }

    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME)
            continue;
        TOKARRAY_INFO(pti[i].type).copystruct(pti, i, pDestStruct, (void*)pSrcStruct, 0, memAllocator, customData);
    }
    return pDestStruct;
}


// Functions for auto-reloading files

// Assumes the first "string" parameter defined in pti is a "name"
static char *structGetFirstString(ParseTable *pti, void *structptr)
{
    int i;
    if (!structptr)
        return NULL;
    FORALL_PARSEINFO(pti, i)
    {
        if (TOK_GET_TYPE(pti[i].type) == TOK_STRING_X ||
            TOK_GET_TYPE(pti[i].type) == TOK_CURRENTFILE_X)
            return TokenStoreGetString(pti, i, structptr, 0);
    }
    printf("Warning: reload called on structure missing any string fields to use as a unique ID!\n");
    return NULL;
}

static char *structGetFilename(ParseTable *pti, void *structptr)
{
    int i;
    if (!structptr)
        return NULL;
    FORALL_PARSEINFO(pti, i)
    {
        if (TOK_GET_TYPE(pti[i].type) == TOK_CURRENTFILE_X)
            return TokenStoreGetString(pti, i, structptr, 0);
    }
    printf("Warning: reload called on structure missing filename field!\n");
    return NULL;
}

static int hasTokenType(ParseTable *pti, TokenizerTokenType type)
{
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        if (TOK_GET_TYPE(pti[i].type) == (U32)type)
            return 1;
    }
    return 0;
}

#define SCRIPTS_DIR_SUBSTR    "SCRIPTS.LOC/"
#define SCRIPTS_DIR_LEN        12

static int relaxedCompare(const char* path, const char* path2)
{
    const char* relaxedPath = strnicmp(path, SCRIPTS_DIR_SUBSTR, SCRIPTS_DIR_LEN)?path:path+SCRIPTS_DIR_LEN;
    const char* relaxedPath2 = strnicmp(path2, SCRIPTS_DIR_SUBSTR, SCRIPTS_DIR_LEN)?path2:path2+SCRIPTS_DIR_LEN;
    return stricmp(relaxedPath, relaxedPath2);
}


// Updates all fields in a structure that exist in pNewStruct, and prunes any that exist
//  in pOldStruct and have the same filename as "relpath"
// relpath is the file that was modified, for checking for orphaned structures
static int ParserReloadStructHelper(const char *relpath, ParseTable pti[], void *pOldStruct, void *pNewStruct, ParserReloadCallback subStructCallback)
{
    int i, sNew, sOld;
    
    FORALL_PARSEINFO(pti, i)
    {
        if (TOK_GET_TYPE(pti[i].type) == TOK_STRUCT_X) {
            // handle all the elements in this structure
            if (hasTokenType(pti[i].subtable, TOK_CURRENTFILE_X)) 
            {
                if (TokenStoreGetStorageType(pti[i].type) == TOK_STORAGE_DIRECT_SINGLE) 
                {
                    ParserReloadStructHelper(relpath, pti[i].subtable, 
                        TokenStoreGetPointer(pti, i, pOldStruct, 0), 
                        TokenStoreGetPointer(pti, i, pNewStruct, 0), NULL); // just recurse through any top-level embedded structs
                } 
                else 
                {
                    char *oldName, *newName;
                    char *path;
                    void **eaRelevantStructs=NULL;

                    // list containing all structures in the old data that match our filename
                    eaCreate(&eaRelevantStructs);
                    for (sOld = 0; sOld < TokenStoreGetNumElems(pti, i, pOldStruct); sOld++) 
                    {
                        void *substructOld = TokenStoreGetPointer(pti, i, pOldStruct, sOld);
                        assert(substructOld);
                        path = structGetFilename(pti[i].subtable, substructOld);
                        if (relaxedCompare(path, relpath)!=0) // Not in our file
                            continue;
                        eaPush(&eaRelevantStructs, substructOld);
                    }

                    // Copy new elements over old elements
                    for (sNew = 0; sNew < TokenStoreGetNumElems(pti, i, pNewStruct); sNew++)
                    {
                        void *substructNew = TokenStoreGetPointer(pti, i, pNewStruct, sNew);
                        bool bFoundOldOne=false;
                        assert(substructNew);

                        // Find element with this name in the destination list and overwrite it
                        newName = structGetFirstString(pti[i].subtable, substructNew);
                        path = structGetFilename(pti[i].subtable, substructNew);
                        if (relaxedCompare(path, relpath)!=0) {
                            printf("Error: reloading for %s, but struct had path of %s\n", relpath, path);
                            continue;
                        }

                        // Look through the old list for this guy
                        for (sOld = eaSizeUnsafe(&eaRelevantStructs)-1; sOld >= 0; sOld--) 
                        {
                            void *substructOld = eaRelevantStructs[sOld];
                            oldName = structGetFirstString(pti[i].subtable, substructOld);
                            if (relaxedCompare(oldName, newName)==0) {
                                void *oldStructCopy = StructAllocRaw(pti[i].param);
                                // Found him!
                                bFoundOldOne = true;
                                // Make a copy of the old structure to pass to the reload callback before we clear it 
                                StructCopyAll(pti[i].subtable, substructOld, pti[i].param, oldStructCopy);
                                // Compare check doesn't work because of (at least) CURRENTFILE_TIMESTAMP has changed
                                //if (ParserCompareStruct(pti[i].subtable, substructOld, substructNew)==0) {
                                //    printf("Reload: no changes to existing structure named: %s\n", newName);
                                //    break;
                                //}
                                //assert(ParserValidateHeap());
                                if (!isSharedMemory(substructOld)) {
                                    StructClear(pti[i].subtable, substructOld);
                                } else {
                                    memset(substructOld, 0, pti[i].param);
                                }
                                StructCopyAll(pti[i].subtable, substructNew, pti[i].param, substructOld);
                                eaRemove(&eaRelevantStructs, sOld);
                                verbose_printf("Reload: updating existing structure named: %s\n", newName);
                                if (subStructCallback)
                                    subStructCallback(substructOld, oldStructCopy, pti[i].subtable, eParseReloadCallbackType_Update);
                                StructClear(pti[i].subtable, oldStructCopy);
                                break;
                            }
                        }
                        if (!bFoundOldOne) {
                            // New structure, add it in!
                            void *substructOld;
                            TokenStoreMakeLocalEArray(pti, i, pOldStruct);
                            substructOld = TokenStoreAlloc(pti, i, pOldStruct, -1, pti[i].param, NULL, NULL);
                            StructCopyAll(pti[i].subtable, substructNew, pti[i].param, substructOld);
                            verbose_printf("Reload: adding new structure named: %s\n", newName);
                            if (subStructCallback)
                                subStructCallback(substructOld, NULL, pti[i].subtable, eParseReloadCallbackType_Add);
                        }
                    }
                    // Search for any old structures that have this filename, but were not modified, remove them from list (leak them)
                    if (eaSizeUnsafe(&eaRelevantStructs)) 
                    {
                        verbose_printf("Reload: pruning %d structures deleted from %s: ", eaSizeUnsafe(&eaRelevantStructs), relpath);
                        for (sOld=0; sOld < eaSizeUnsafe(&eaRelevantStructs); sOld++) 
                        {
                            void *removedStruct = TokenStoreRemovePointer(pti, i, pOldStruct, eaRelevantStructs[sOld]);
                            // Could free this with a flag passed in, but for reloading we want to leak it, as
                            //  external things might still be pointing at this structure!
                            verbose_printf("%s ", structGetFirstString(pti[i].subtable, removedStruct));
                            if (subStructCallback)
                                subStructCallback(removedStruct, NULL, pti[i].subtable, eParseReloadCallbackType_Delete);
                        }
                        verbose_printf("\n");
                    }
                    eaDestroy(&eaRelevantStructs);
                }
            } else {
                // Could just recurse, this must be a structure of substructures?
                printf("Warning: reload called on structure with its substructures missing a TOK_CURRENTFILE field\n");
            }
        } else if ((TOK_GET_TYPE(pti[i].type) != TOK_START) && (TOK_GET_TYPE(pti[i].type) != TOK_END)){
            bool bIgnore=false;
            if (TOK_GET_TYPE(pti[i].type) == TOK_STASHTABLE_X && subStructCallback)
                bIgnore = true; // The caller will update his hashtables, presumably
            if (!bIgnore)
                printf("Warning: reload called on structure with unsupported high-level elements\n");
        }
    }
    return 1;
}

// Assumes the following:
// the base structure just contains one or more earrays of substructures
// the sub structures all are set up as such:
//   the first string parameter in the TokenizerParserInfo is a "Name" string, or some kind of
//      unique identifier (this is used when finding old elements that need to get replaced
//        with new ones)
//   somewhere there is defined a TOK_CURRENTFILE parameter (ideally, the second entry)
//   the filename and the "Name" parameter can, in fact, be the same string (assuming only one entry per file, like FX)
// any structure that has the same Filename field as the file reloaded will either
//   be overwritten or removed from the list (but not freed, although a flag for this could
//   be trivially added).
// The callback is called on each reloaded substruct
int ParserReloadFile(const char *relpath_in, ParseTable pti[], size_t sizeOfBaseStruct, void *pOldStruct, ParserReloadCallback subStructCallback, ParserLoadPreProcessFunc preprocessor )
{
    int ret=1;
    char relpath[MAX_PATH];
    void *tempBase = StructAllocRawDbg(sizeOfBaseStruct, relpath_in, -1);

    while (relpath_in[0]=='/')
        relpath_in++;
    strcpy(relpath, relpath_in);
    forwardSlashes(relpath);
    _strupr(relpath);

    if (!ParserLoadFiles(NULL, relpath, NULL, PARSER_EXACTFILE, pti, tempBase, 0, NULL, preprocessor))
    {
        ret = 0;
    } else {
        ParserReloadStructHelper(relpath, pti, pOldStruct, tempBase, subStructCallback);
    }
    StructClear(pti, tempBase);
    StructFree(tempBase);
    return ret;
}



// Utility functions only used in ListView and structNet stuff
StashTable htFieldCountCache = 0; // Cache the size of structures based on the address of the parse info, which must be static and unique, or registered upon deletion
void ParseTableClearCachedInfo(ParseTable *pti)
{
    if (htFieldCountCache) {
        stashAddressRemovePointer(htFieldCountCache, pti, NULL);
    }
}

int ParseTableCountFields(ParseTable *pti)
{
    int i;
    if (htFieldCountCache == 0) {
        htFieldCountCache = stashTableCreateAddress(4);
    }


    if (stashAddressFindInt(htFieldCountCache, (void*)pti, &i))
        return i;

    FORALL_PARSEINFO(pti, i)
        ;
    stashAddressAddInt(htFieldCountCache, (void*)pti, i, false);
    return i;
}

int TokenizerParseToken(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, ParserTextCallback callback)
{
    int done = 0;

    if (TYPE_INFO(tpi[column].type).interpretfield(tpi, column, SubtableField) == StaticDefineList)
        TokenizerSetStaticDefines(tok, tpi[column].subtable);
    done = TOKARRAY_INFO(tpi[column].type).parse(tok, tpi, column, structptr, 0, callback);
    TokenizerSetStaticDefines(tok, NULL);

    return done;
}

// public ParserReadTokenizer function
int ParserReadTokenizer(TokenizerHandle tok, ParseTable pti[], void* structptr, ParserTextCallback callback)
{
    int ok = 1;
    const char* nexttoken;
    int i;
    int matched;
    int done = 0;
    U32* usedBitField = NULL;

    PERFINFO_AUTO_START("ParserReadTokenizer", 1);

    // init filenames, etc. before parsing through tokens
    FORALL_PARSEINFO(pti, i)
    {
        if (TYPE_INFO(pti[i].type).preparse)
        {
            TYPE_INFO(pti[i].type).preparse(pti, i, structptr, tok);
        }
        if (TOK_GET_TYPE(pti[i].type) == TOK_USEDFIELD_X)
        {
            usedBitField = TokenStoreGetPointer(pti, i, structptr, 0);
        }
    }

    // grab struct param fields immediately following the structure name
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_STRUCTPARAM)
        {
            nexttoken = TokenizerPeek(tok, 0, 1);
            if (nexttoken && !IsEol(nexttoken))
            {
                if (usedBitField)
                    SETB(usedBitField, i);
                if (TokenizerParseToken(tok, pti, i, structptr, callback)) done = 1;
            }
        }
    }

    // allow a structure to close with eol
    FORALL_PARSEINFO(pti, i)
    {
        if ((TOK_GET_TYPE(pti[i].type) == TOK_END) && pti[i].name && pti[i].name[0] == '\n' && !pti[i].name[1])
            done = 1;
        // we don't eat the eol here - this allows for structures to be params to other structures
    }

    while (!done) // main loop
    {
        g_parseerr = 0; // catch any errors for this token

        // try to match in our list
        matched = 0;
        nexttoken = TokenizerPeek(tok, 1, 1);
        
        if (!nexttoken) break; // ran out of tokens

        FORALL_PARSEINFO(pti, i)
        {
            if (!(pti[i].type & TOK_PARSETABLE_INFO))
            {
                // don't include metadata as a match.
                if (stricmp(nexttoken, pti[i].name) == 0)
                {
                    matched = 1;
                    break;
                }
            }
        }
        if (g_parseerr) ok = 0;

        if (!matched)    // unknown token
        {
            if (!callback) done = 1;
            else if (!callback(tok, nexttoken, structptr)) done = 1;
        }
        else
        {
            if (usedBitField)
                SETB(usedBitField, i);
            TokenizerGet(tok, 1, 1); // get rid of the name
            if (TokenizerParseToken(tok, pti, i, structptr, callback)) done = 1;
            if (!done && (TOK_GET_TYPE(pti[i].type) != TOK_STRUCT_X) ) VerifyParametersClear(tok); // clear the eol
        }
        if (g_parseerr) ok = 0;
    } // while !done

    if (!ok)
        g_parseerr = 1;

    PERFINFO_AUTO_STOP();

    return ok;
}

static int StringNeedsEscaping(const char* str)
{
    if (strchr(str, '\n')) return 1;
    if (strchr(str, '\r')) return 1;
    if (strchr(str, '\"')) return 1;
    return 0;
}

static int StringNeedsQuotes(const char* str)
{
    if (strlen(str)==0) return 1;   // need quotes around empty string
    if (strchr(str, ' ')) return 1;    // standard delimeters between tokens
    if (strchr(str, ',')) return 1;
    if (strchr(str, '\n')) return 1;
    if (strchr(str, '\r')) return 1;
    if (strchr(str, '\t')) return 1;
    if (str[0] == '#') return 1; // could cause confusion with comments
    if (str[0] == '/') return 1;
    if (str[0] == '<') return 1; // could cause confusion with escaping
    return 0;
}

static void WriteString(FILE* out, const char* str, int tabs, int eol)
{
    int i;
    for (i = 0; i < tabs; i++) fwrite("\t", 1, sizeof(char), out);
    fwrite(str, strlen(str), sizeof(char), out);
    if (eol) fwrite("\r\n", 2, sizeof(char), out);
}

static void WriteQuotedString(FILE* out, const char* str, int tabs, int eol)
{
    int escaped, quoted;
    char buf[TOKEN_BUFFER_LENGTH]; 

    if (!str) str = "";
    escaped = StringNeedsEscaping(str);
    if (escaped)
    {
        TokenizerEscape(buf, str);
        WriteString(out, buf, tabs, eol);
    }
    else
    {
        quoted = StringNeedsQuotes(str);
        if (quoted) WriteString(out, "\"", tabs, 0);
        WriteString(out, str, 0, 0);
        if (quoted) WriteString(out, "\"", 0, 0);
        WriteString(out, "", 0, eol);
    }
}

static void WriteInt64(FILE* out, S64 i, int tabs, int eol, void* subtable)
{
    const char* define = 0;
    char str[20];
    sprintf_s(SAFESTR(str), "%I64d", i);
    if (subtable) define = StaticDefineRevLookup((StaticDefine*)subtable, str);
    WriteString(out, define? define: str, tabs, eol);
}

static void WriteInt(FILE* out, int i, int tabs, int eol, void* subtable)
{
    const char* define = 0;
    char str[20];
    sprintf_s(SAFESTR(str), "%i", i);
    if (subtable) define = StaticDefineRevLookup((StaticDefine*)subtable, str);
    WriteString(out, define? define: str, tabs, eol);
}

static void WriteUInt(FILE* out, unsigned int i, int tabs, int eol, void* subtable)
{
    const char* define = 0;
    char str[20];
    sprintf_s(SAFESTR(str), "%u", i);
    if (subtable) define = StaticDefineRevLookup((StaticDefine*)subtable, str);
    WriteString(out, define? define: str, tabs, eol);
}

static void WriteFloat(FILE* out, float f, int tabs, int eol, void* subtable)
{
    const char* define = 0;
    char str[20];
    sprintf_s(SAFESTR(str), "%.4g", f);
    if (subtable) define = StaticDefineRevLookup((StaticDefine*)subtable, str);
    WriteString(out, define? define: str, tabs, eol);
}

static void WriteTextFunctionCalls(FILE* out, StructFunctionCall** structarray)
{
    int escaped, quoted;
    int i, n;
    char buf[TOKEN_BUFFER_LENGTH]; 

    n = eaSize(&structarray);
    for (i = 0; i < n; i++)
    {
        char* str = structarray[i]->function;
        if (!str) continue; // not really a valid input
        if (i) WriteString(out, ", ", 0, 0);

        // make sure to quote our string if it has a paren
        escaped = StringNeedsEscaping(str);
        if (escaped)
        {
            TokenizerEscape(buf, str);
            WriteString(out, buf, 0, 0);
        }
        else
        {
            quoted = StringNeedsQuotes(str) || strchr(str, '(') || strchr(str, ')');
            if (quoted) WriteString(out, "\"", 0, 0);
            WriteString(out, str, 0, 0);
            if (quoted) WriteString(out, "\"", 0, 0);
        }

        // any of my children
        if (structarray[i]->params)
        {
            WriteString(out, "( ", 0, 0); // space in case we start with escaped string
            WriteTextFunctionCalls(out, structarray[i]->params);
            WriteString(out, " )", 0, 0);
        }
    }
}

static int ParseBase85(TokenizerHandle tok, const char *token, void *dst, int dst_size, int allow_size_mismatch)
{
    // simple base-85 decode
    U32 *data = dst;
    int size = dst_size;
    int ok = 1;

    if(*(token++) != 'U') // data error
    {
        ErrorFilenamef(TokenizerGetFileName(tok), "Parser: raw data missing head token in %s\n", TokenizerGetFileAndLine(tok));
        ok = 0;
        g_parseerr = 1;
    }
    for(; ok && size > 0; size -= 4)
    {
        int i;
        U32 block = 0;

        if(*token != 'z')
        {
            for(i = 0; i < 5 && *token; i++, token++)
            {
                U32 p85[5] = { 1, 85, 85*85, 85*85*85, 85*85*85*85 };
                U32 c = *token == '$' ? ',' : *token; // stupid hack around comma
                if( (int)c > 121 || (int)c < (i < 4 ? 37 : 39) ) // data error (highest 85igit in a block can only go to 82)
                {
                    ErrorFilenamef(TokenizerGetFileName(tok), "Parser: raw data has invalid characters in %s\n", TokenizerGetFileAndLine(tok));
                    ok = 0;
                    g_parseerr = 1;
                }

                c = block + p85[i] * (121 - c);
                if(c < block)
                {
                    ErrorFilenamef(TokenizerGetFileName(tok), "Parser: raw data token overflow in %s\n", TokenizerGetFileAndLine(tok));
                    ok = 0;
                    g_parseerr = 1;
                }
                block = c;
            }
        }
        else
        {
            // simple compression for 0-runs
            token++;
            i = size + 1; // 'z' works for any tail
        }

        if(size >= 4)
        {
            *(data++) = block;
            if(i < 5)
            {
                if(allow_size_mismatch)
                {
                    memset(data, 0, size); // default (zero) the rest
                    break;
                }
                else
                {
                    ErrorFilenamef(TokenizerGetFileName(tok), "Parser: raw data underrun in %s\n", TokenizerGetFileAndLine(tok));
                    ok = 0;
                    g_parseerr = 1;
                }
            }
        }
        else
        {
            memcpy(data, &block, size);
            if(i != size+1)
            {
                if(allow_size_mismatch)
                {
                    break;
                }
                else
                {
                    ErrorFilenamef(TokenizerGetFileName(tok), "Parser: raw data overrun or underrun in %s\n", TokenizerGetFileAndLine(tok));
                    ok = 0;
                    g_parseerr = 1;
                }
            }
        }
    }
    if(!allow_size_mismatch && *token)
    {
        ErrorFilenamef(TokenizerGetFileName(tok), "Parser: raw data overrun in %s\n", TokenizerGetFileAndLine(tok));
        ok = 0;
        g_parseerr = 1;
    }
    return ok;
}

static void WriteBase85(FILE *out, void *src, int src_size)
{
    // simple base-85 encode
    U32 *data = src;
    int size = src_size;
    char str[6] = "";
    WriteString(out, "U", 0, 0); // ASCII 85, get it? this'll make the tokenizer skip most of the weird handling
    for(; size > 0; size -= 4)
    {
        int i = 0;
        U32 block = 0;
        if(size >= 4)
            block = *(data++);
        else
            memcpy(&block, data, size);
        if(block)
        {
            for(; i < 5 && i < size+1; i++)
            {
                str[i] = 121 - block%85; // keep lower numbers in non-escaped characters. [%-y]
                if(str[i] == ',') // stupid hack around comma
                    str[i] = '$'; // 36
                block /= 85;
            }
        }
        else
        {
            // simple compression for zero-runs
            str[i++] = 'z';
        }
        str[i] = '\0';
        WriteString(out, str, 0, 0);
    }
}

int InnerWriteTextToken(FILE* out, ParseTable tpi[], int column, void* structptr, int level, int showname, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    TOKARRAY_INFO(tpi[column].type).writetext(out, tpi, column, structptr, 0, showname, level,iOptionFlagsToMatch,iOptionFlagsToExclude);
    return 1;
}

int InnerWriteTextFile(FILE* out, ParseTable pti[], const void* structptr, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int i;
    int ok = 1;
    int singlelinestruct = 0;
    int startedbody = 0;

// don't write the initial eol until we need to (this is for single-line structs)
#define START_BODY    if (!startedbody) { WriteString(out, "", 0, 1); startedbody = 1; }

    // write any structure parameters
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue;
        if (iOptionFlagsToMatch && !(iOptionFlagsToMatch & pti[i].type)) continue;
        if (iOptionFlagsToExclude && (iOptionFlagsToExclude & pti[i].type)) continue;
        if (pti[i].type & TOK_STRUCTPARAM)
        {
            TOKARRAY_INFO(pti[i].type).writetext(out, pti, i, structptr, 0, 0, level,iOptionFlagsToMatch,iOptionFlagsToExclude);
        }
    }

    // write the start token
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue;
        if (TOK_GET_TYPE(pti[i].type) == TOK_START)
        {
            START_BODY;
            WriteString(out, pti[i].name, level, 1);
            break;
        }
    }

    // write each field
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) 
            continue;
        if (pti[i].type & TOK_STRUCTPARAM) 
            continue;
        if (TOK_GET_TYPE(pti[i].type) == TOK_END) 
            continue;
        
        if (iOptionFlagsToMatch && !(iOptionFlagsToMatch & pti[i].type)) 
            continue;        
        if (iOptionFlagsToExclude && (iOptionFlagsToExclude & pti[i].type)) 
            continue;    

        START_BODY;
        TOKARRAY_INFO(pti[i].type).writetext(out, pti, i, structptr, 0, 1, level,iOptionFlagsToMatch,iOptionFlagsToExclude);
    }

    // write the end token
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) 
            continue;
        if (TOK_GET_TYPE(pti[i].type) == TOK_END)
        {
            if (pti[i].name && pti[i].name[0] == '\n' && !pti[i].name[1])
                singlelinestruct = 1;
            else
            {
                START_BODY;
                WriteString(out, pti[i].name, level, 1);
            }
            break;
        }
    }
    if (!singlelinestruct) 
        WriteString(out, "", 0, 1);    // extra eol for neatness

    return ok;
}

int ParserWriteTextFile(const char* filename, ParseTable pti[], const void* structptr, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int ok = 1;
    FILE* out;
    char* realFilename;
    char buf[MAX_PATH];

    // fileLocateWrite might return a directory that does not exist. To ensure we write the file, create it.
    realFilename = fileLocateWrite(filename, buf);
    mkdirtree(realFilename);

    // init file
    out = fileOpen(filename, "wb");
    if (!out) return 0;
    ok = InnerWriteTextFile(out, pti, structptr, -1, iOptionFlagsToMatch, iOptionFlagsToExclude);
    fclose(out);
    return ok;
}

//////////////////////////////////////////////////////////////////////////////////// Serialized Parsed Files
static void UpdateCrcFromStaticDefines(StaticDefine defines[])
{
    int curtype = DM_NOTYPE;
    StaticDefine* cur = defines;

    while (1)
    {
        // look for key markers first
        if (cur->key == U32_TO_PTR(DM_END))
            return;
        else if (cur->key == U32_TO_PTR(DM_INT))
            curtype = DM_INT;
        else if (cur->key == U32_TO_PTR(DM_STRING))
            curtype = DM_STRING;
        else if (cur->key == U32_TO_PTR(DM_DYNLIST))
        {
            if (*(DefineContext**)cur->value)
                UpdateCrcFromDefineList(*(DefineContext**)cur->value);
        }
        else
        {
            // crc the key and value
            cryptAdler32Update(cur->key, (int)strlen(cur->key));
            if (curtype == DM_INT)
            {
                int val = PTR_TO_S32(cur->value);
                cryptAdler32Update((U8*)&val, sizeof(int));
            }
            else if (curtype == DM_STRING)
                cryptAdler32Update(cur->value, (int)strlen(cur->value));
        }
        // keep looking for keys
        cur++;
    }
}

static void UpdateCRCFromParseInfo(ParseTable pti[], ParseTable parent[] )    // get crc to use as build number based on parse tree
{
    int i;
    FORALL_PARSEINFO(pti, i)
    {
        int usage;
        int type = TOK_GET_TYPE(pti[i].type);
        if (pti[i].type & TOK_REDUNDANTNAME) 
            continue; // ignore redundant fields

        // name field
        if (pti[i].name) 
            cryptAdler32Update(pti[i].name, (int)strlen(pti[i].name));
        cryptAdler32Update((void*)&pti[i].type, sizeof(int));

        // default value field 
        usage = TYPE_INFO(type).interpretfield(pti, i, ParamField);
        switch (usage)
        {
        case OffsetOfSizeField: break; // don't care about offset reposition
        case SizeOfSubstruct: break; // don't care about size of struct changing
        case NumberOfElements: // fall
        case DefaultValue: // fall
        case EmbeddedStringLength: // fall
        case SizeOfRawField: 
            {
                int param = (int)pti[i].param; // make ourselves insensitive to 64-bit while crc'ing
                cryptAdler32Update((U8*)&param, sizeof(int));
            }
            break;
        case PointerToDefaultString:
            {
                char* str = (char*)pti[i].param;
                if (str) 
                    cryptAdler32Update(str, (int)strlen(str));
            }
            break;
        }

        // subtable
        if (type == TOK_STRUCT_X && pti[i].subtable != pti && // ignore simple recursion
            (!parent || parent != pti[i].subtable) ) // also ignore slightly more complicated recursion
        {
            UpdateCRCFromParseInfo(pti[i].subtable, pti );
        }// Note: recursion can still cause this to go on forever if its re-entrant more than two levels deep.
        

        // static defines
        if (pti[i].subtable)
        {
            usage = TYPE_INFO(type).interpretfield(pti, i, SubtableField);
            if (usage == StaticDefineList)
            {
                UpdateCrcFromStaticDefines(pti[i].subtable);
            }
        }
    }
}

int ParseTableCRC(ParseTable pti[], DefineContext* defines)
{
    cryptAdler32Init();
    UpdateCRCFromParseInfo(pti, 0);
    if (defines) 
        UpdateCrcFromDefineList(defines);
    return (int)cryptAdler32Final();
}

// return success
static int WriteBinaryFunctionCalls(SimpleBufHandle file, StructFunctionCall** structarray, int* sum)
{
    int succeeded = 1;
    int i, wr, number = eaSize(&structarray);

    if (!SimpleBufWriteU32(number, file)) succeeded = 0;
    *sum += sizeof(int);
    for (i = 0; i < number; i++)
    {
        if (!(wr = WritePascalString(file, structarray[i]->function))) succeeded = 0;
        *sum += wr;
        if (!WriteBinaryFunctionCalls(file, structarray[i]->params, sum)) succeeded = 0;
    }
    return succeeded;
}

void SendDiffFunctionCalls(Packet* pak, StructFunctionCall** structarray)
{
    int i, n = eaSize(&structarray);
    pktSendBitsAuto(pak, n);
    for (i = 0; i < n; i++)
    {
        pktSendString(pak, structarray[i]->function);
        SendDiffFunctionCalls(pak, structarray[i]->params);
    }
}

int ParserWriteBinaryTable(SimpleBufHandle file, ParseTable pti[], void* structptr, int* sum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude) // returns success
{
    int succeeded = 1;
    long loc, dataloc;
    int i, datasum;

    if (!structptr) return 0; // fail out
    *sum = 0;

    // skip data segment length
    datasum = 0;
    dataloc = SimpleBufTell(file);
    SimpleBufSeek(file, sizeof(int), SEEK_CUR);
    *sum += sizeof(int);

    // data segment
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue; // don't allow TOK_REDUNDANTNAME
        if (iOptionFlagsToMatch && !(iOptionFlagsToMatch & pti[i].type)) continue;
        if (iOptionFlagsToExclude && (iOptionFlagsToExclude & pti[i].type)) continue;
        succeeded = succeeded && TOKARRAY_INFO(pti[i].type).writebin(file, pti, i, structptr, 0, &datasum, iOptionFlagsToMatch, iOptionFlagsToExclude);
        if (!succeeded)
            break;
    } // each data token

    // fixup length of data segment
    loc = SimpleBufTell(file);
    SimpleBufSeek(file, dataloc, SEEK_SET);
    if (!SimpleBufWriteU32(datasum, file)) succeeded = 0;
    *sum += datasum;
    SimpleBufSeek(file, loc, SEEK_SET);

    return succeeded;
}

int ParserWriteBinaryFile(char* filename, ParseTable pti[], void* structptr, FileList* filelist, DefineContext* defines, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude) // returns success
{
    int crc;
    SimpleBufHandle file;
    int wr = 0;
    int success;

    crc = ParseTableCRC(pti, defines);
    file = SerializeWriteOpen(filename, PARSE_SIG, crc);
    if (!file) return 0; // can't create

    success = FileListWrite(filelist, file);
    if (!success) Errorf("ParserWriteBinaryFile: could not write to %s\n", filename);
    success = ParserWriteBinaryTable(file, pti, structptr, &wr, iOptionFlagsToMatch, iOptionFlagsToExclude);
    if (!success) Errorf("ParserWriteBinaryFile: could not write to %s\n", filename);
    SerializeClose(file);
    return success;
}

static int ReadBinaryFunctionCalls(SimpleBufHandle file, StructFunctionCall*** structarray, int* sum)
{
    char tempstr[MAX_STRING_LENGTH];
    int succeeded = 1;
    int number, re, i;
    StructFunctionCall* newstruct;

    if (!SimpleBufReadU32((U32*)&number, file)) succeeded = 0;
    *sum += sizeof(int);
    if (number)
    {
        if (structarray) eaDestroy(structarray);
        eaCreate(structarray);
        for (i = 0; i < number; i++)
        {
            if (!(re = ReadPascalString(file, tempstr, MAX_STRING_LENGTH))) succeeded = 0;
            *sum += re;
            if (tempstr[0])
            {
                newstruct = StructAllocRawDbg(sizeof(*newstruct), parser_relpath, -1);
                newstruct->function = StructAllocString(tempstr);
                if (!ReadBinaryFunctionCalls(file, &newstruct->params, sum)) succeeded = 0;
                eaPush(structarray, newstruct);
            }
        }
    }
    return succeeded;
}

void RecvDiffFunctionCalls(Packet* pak, StructFunctionCall*** structarray)
{
    int i, n;

    // destroy any existing data
    for (i = 0; i < eaSize(structarray); i++)
        StructFreeFunctionCall((*structarray)[i]);

    n = pktGetBitsAuto(pak);
    if (n == 0)
    {
        eaDestroy(structarray);
        return;
    }

    eaSetSize(structarray, n);
    for (i = 0; i < n; i++)
    {
        char* function = pktGetString(pak);
        if (function && function[0])
            (*structarray)[i]->function = StructAllocString(function);
        RecvDiffFunctionCalls(pak, &(*structarray)[i]->params);
    }
}

int ParserReadBinaryTable(SimpleBufHandle file, ParseTable pti[], void* structptr, int* sum) // returns success
{
    int succeeded = 1;
    long dataloc;
    int i, datasum;
    int readdatasum;

    if (!structptr) return 0; // fail out

    // read data segment length
    if (!SimpleBufReadU32((U32*)&readdatasum, file)) return 0;
    *sum += sizeof(int);
    dataloc = SimpleBufTell(file);
    datasum = 0;

    // read data segment
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue; // don't allow TOK_REDUNDANTNAME
        succeeded = succeeded && TOKARRAY_INFO(pti[i].type).readbin(file, pti, i, structptr, 0, &datasum);
        if (!succeeded)
        {
            printf("ParserReadBinaryTable: unexpected end of file\n");
            return 0;
        }
    } // each data token

    // check data segment length
    if (datasum != readdatasum)
    {
        printf("ParserReadBinaryTable: datasum length not correct\n");
        return 0;
    }
    *sum += readdatasum;

    return succeeded;
}

int ParserReadBinaryFile(SimpleBufHandle binfile, char *filename, ParseTable pti[], void* structptr, FileList* filelist, DefineContext* defines)    // returns success
{
    SimpleBufHandle file;
    int size = 0;
    int success;
    long loc, endloc;

    if (binfile) {
        file = binfile;
    } else {
        int crc = ParseTableCRC(pti, defines);
        file = SerializeReadOpen(filename, PARSE_SIG, crc, isProductionMode());
    }

    if (!file) return 0;  // no file, or failed crc number
    if (!FileListRead(filelist, file)) {
        SerializeClose(file);
        return 0; // couldn't read file table
    }
    loc = SimpleBufTell(file);

    parser_relpath = filename ? allocAddString(filename) : 0;
    success = ParserReadBinaryTable(file, pti, structptr, &size);
    parser_relpath = 0;

    SimpleBufSeek(file, 0, SEEK_END);
    endloc = SimpleBufTell(file);
    if (endloc - loc != size)
    {
        printf("ParserReadBinaryFile: unexpected end of file\n");
        success = 0;
    }
    SerializeClose(file);
    return success;
}



///////////////////////////////////////////////////////////////////////////////// ParserLoadFiles

// globals to pass information (arg)
// lf_ == LoadFiles_xx, not Leonard
static char* lf_filemasks[16];
ParseTable* lf_pti;
void* lf_structptr;
int lf_loadedok, lf_ignoreempty;
DefineContext* lf_globaldefines;
int lf_loadedonefile;
__time32_t g_lasttime;
int lf_forcebincreate = 0;
FileList lf_filedates = 0;            // a list of the top-level files I should load w/ dates

static int matchesFileMask(const char *str)
{
    int i;
    if (!lf_filemasks[0])
        return 1;

    for (i = 0; lf_filemasks[i]; i++)
    {
        if (strEndsWith(str, lf_filemasks[i]))
            return 1;
    }

    return 0;
}
    
// turn on/off forced .bin creation by ParserLoadFiles
void ParserForceBinCreation(int set)
{
    lf_forcebincreate = set;
}

// set flag letting us know whether there were any errors in parsing
int ParserLoadErrorCallback(TokenizerHandle tok, const char* nexttoken, void* structptr)
{
    lf_loadedok = 0;
    return ParserErrorCallback(tok, nexttoken, structptr);
}

static bool ParserLoadFile(const char* fullpath, int ignore_empty)
{
    bool ret = 0;
    TokenizerHandle tok;

    tok = TokenizerCreateEx(fullpath, ignore_empty);
    if (!tok)
    {
        if (!ignore_empty)
            lf_loadedok = 0;
        return 0;
    }
    lf_loadedonefile = 1;
    TokenizerSetGlobalDefines(tok, lf_globaldefines);
    ret = ParserReadTokenizer(tok, lf_pti, lf_structptr, ParserLoadErrorCallback);
    TokenizerDestroy(tok);
    if (!ret)
        lf_loadedok = 0;
    return ret;
}

static FileScanAction ParserLoadCallback(char* dir, struct _finddata32_t* data)
{
    char buffer[512];

    if (strStartsWith(dir, fileDataDir()))
        dir += strlen(fileDataDir());
    if (dir[0]=='/')
        dir++;


    if(!strstr(dir, "/_") && !(data->name[0] == '_') && matchesFileMask(data->name) )
    {
        sprintf_s(SAFESTR(buffer), "%s/%s", dir, data->name);
        ParserLoadFile(buffer, lf_ignoreempty);
    }
    return FSA_EXPLORE_DIRECTORY;
}

// store latest time in g_lasttime
static FileScanAction DateCheckCallback(char* dir, struct _finddata32_t* data)
{
    char buffer[512];

    if (data->attrib & _A_SUBDIR) {
        // If it's a subdirectory, we don't care about checking times, etc
        if (data->name[0]=='_') {
            return FSA_NO_EXPLORE_DIRECTORY;
        } else {
            return FSA_EXPLORE_DIRECTORY;
        }
    }

    if(!strstr(dir, "/_") && !(data->name[0] == '_') && matchesFileMask(data->name) ) // This strstr probably isn't needed because we check directories above ?
    {
        assert(lf_filedates);
        STR_COMBINE_SSS(buffer, dir, "/", data->name);
        FileListInsert(&lf_filedates, buffer, data->time_write); // seems like we should call statTimeToUTC here, see comment in ParserIsPersistNewer below
    }
    return FSA_EXPLORE_DIRECTORY;
}

SimpleBufHandle ParserIsPersistNewer(const char* dir, const char* filemask, const char* persistfile, ParseTable pti[], DefineContext* defines)
{
    FileList binlist = NULL;
    SimpleBufHandle binfile;
    int crc, i, n;
    int savedpos;
    int haveFalseFiles = 0;        // do I have files in lf_filedates that don't exist?

    // open the bin file
    crc = ParseTableCRC(pti, defines);
    binfile = SerializeReadOpen(persistfile, PARSE_SIG, crc, isProductionMode());
    if (!binfile)
    {
        verbose_printf("bin file %s is incorrect version\n", persistfile);
        return 0; // don't have bin file or crc wrong
    }

    savedpos = SimpleBufTell(binfile);

    // always assume .bin file is ok if not in development mode
    if(isProductionMode())
        return binfile;

    // read the file table from the bin file first
    if (!FileListRead(&binlist, binfile))
        goto IsPersist_Fail;

    // assemble file list from directories
    FileListCreate(&lf_filedates);
    if (dir) // prefixes are handled just by mask matching
        fileScanAllDataDirs(dir, DateCheckCallback);
    else if (filemask)
    {
        char** prefix;

        // load basic file
        FileListInsert(&lf_filedates, filemask, 0);

        // load each of the optional versions
        haveFalseFiles = 1;    // I need to check later if these actually exist
        prefix = g_StdAdditionalFilePrefixes;
        while (**prefix)
        {
            char* withPrefix = addFilePrefix(filemask, *prefix);
            FileListInsert(&lf_filedates, withPrefix, 0);
            prefix++;
        }
    }
    // OK if dir and filemask are both NULL, we just compare to files on disk then

    // compare lists -
    // iterate through files in .bin
    n = eaSize(&binlist);
    for (i = 0; i < n; i++)
    {
        FileEntry* binfile = binlist[i];
        FileEntry* scanfile = FileListFind(&lf_filedates, binfile->path);
        if (scanfile) // if scanned for
        {
            if (!fileDatesEqual(scanfile->date, binfile->date, true))
            {
                verbose_printf("%s in bin file has different date than %s scanned for\n",
                    binfile->path, scanfile->path);
                goto IsPersist_Fail;
            }
            scanfile->seen = 1; // color the scanned files
        }
        else // file wasn't scanned for specifically
        {
            __time32_t diskdate = fileLastChanged(binfile->path);
            if (!diskdate && binfile->date)
            {
                verbose_printf("%s in bin file no longer exists on disk\n", binfile->path);
                goto IsPersist_Fail;
            }
            else if (!fileDatesEqual(diskdate,binfile->date, true))
            {
                verbose_printf("%s in bin file has different date than one on disk\n",
                    binfile->path);
                goto IsPersist_Fail;
            }
        }
    }
    // so, every file in bin has same date as one on disk

    // check that top-level files are in or not in bin file appropriately
    n = eaSize(&lf_filedates);
    for (i = 0; i < n; i++)
    {
        FileEntry* scanfile = lf_filedates[i];
        if (!haveFalseFiles || fileExists(scanfile->path)) // small optimization
        {
            if (!scanfile->seen)
            {
                assert(!FileListFind(&binlist, scanfile->path));
                verbose_printf("%s on disk isn't in bin file\n", scanfile->path);
                goto IsPersist_Fail;
            }
        }
        else // not on disk, and shouldn't be in bin file
        {
            if (scanfile->seen)
            {
                assert(FileListFind(&binlist, scanfile->path));
                verbose_printf("%s is in bin file, but should not be\n", scanfile->path);
                goto IsPersist_Fail;
            }
        }
    }

    // ok then..
    FileListDestroy(&lf_filedates);
    FileListDestroy(&binlist);
    SimpleBufSeek(binfile, savedpos, SEEK_SET);
    return binfile;
IsPersist_Fail:
    FileListDestroy(&lf_filedates);
    FileListDestroy(&binlist);
    SerializeClose(binfile);
    return 0;
}

// filemasks are separated by semicolons
bool ParserLoadFiles(const char* dir, const char* filemask, const char* persistfile, int flags, ParseTable pti[], void* structptr, DefineContext* globaldefines, ParserLoadInfo* pli, ParserLoadPreProcessFunc preprocessor)
{
    static char *old_filemask;
    char persistfilepath[MAX_PATH], buf[MAX_PATH];
    int forcerebuild = flags & PARSER_FORCEREBUILD;
    static bool recursive_call = false;
    int i;
    StructTypeField excludeFlags = 0;

    if (flags & PARSER_SERVERSIDE || flags & PARSER_SERVERONLY)
    {
        excludeFlags = TOK_SERVER_ONLY;
    }
    else if (flags & PARSER_CLIENTSIDE)
    {
        excludeFlags = TOK_CLIENT_ONLY;
    }

    TEST_PARSE_TABLE(pti);

    if (isProductionMode())
    {
        forcerebuild = false;
    }

    PERFINFO_AUTO_START("ParserLoadFiles", 1);

    assert(!(flags & PARSER_ERRFLAG)); // ab: testing if I missed anything
    assert(!recursive_call); // Does not support being called recursively!  (Or from multiple threads)
    recursive_call = true;

    // we know the client doesn't actually mean a non-relative path
    if (dir && (dir[0] == '/' || dir[0] == '\\')) 
        dir++;
    if (filemask && (filemask[0] == '/' || filemask[0] == '\\')) 
        filemask++;
    if (persistfile && (persistfile[0] == '/' || persistfile[0] == '\\')) 
        persistfile++;

    // init info
    i = 0;
    free(old_filemask);
    old_filemask = strdup(filemask);
    lf_filemasks[i++] = strtok_quoted(old_filemask, ";", ";");
    while (lf_filemasks[i] = strtok_quoted(NULL, ";", ";"))
        i++;

    lf_pti = pti;
    lf_structptr = structptr;
    lf_loadedonefile = 0;
    lf_loadedok = 1;
    lf_globaldefines = globaldefines;
    if (pli)
        pli->updated = 0;

    // build the path to the bin file
    if (persistfile)
    {
        if (flags & PARSER_SERVERONLY)
            strcpy(persistfilepath, "server/bin/");
        else
            strcpy(persistfilepath, "bin/");
        strcat(persistfilepath, persistfile);
    }
    else
    {
        *persistfilepath = 0;
    }

    if (persistfile && !lf_forcebincreate && !forcerebuild)
    {
        char *path=NULL;

        writeConsole(OUTPUT_DEBUG, "\tParsing %s from %s", filemask, persistfile);
        path = fileLocateRead(persistfilepath, buf);
        if (path)
        {
            SimpleBufHandle binfile = ParserIsPersistNewer(dir,filemask,persistfilepath,pti,globaldefines);
            if (binfile)
            {
                // try loading persist file
                if (ParserReadBinaryFile(binfile, persistfilepath, pti, structptr, NULL, globaldefines)) {
                    recursive_call = false;
                    PERFINFO_AUTO_STOP();
                    return 1;
                } else verbose_printf("ParserLoadFiles: error loading %s, loading text files instead\n", persistfile);
            }
            else
            {
                verbose_printf("ParserLoadFiles: %s outdated, loading text files instead\n", persistfile);
            }
        }
        else
        {
            verbose_printf("ParserLoadFiles: couldn't find %s, loading text files instead\n", persistfile);
        }
    }

    // Do not attempt to parse data files for any reason if the program is running in production mode.
    //  unless we weren't given a persistfile (bin file) name, then they must want to parse the source
    //  files (used to load config files)
    if( isProductionMode() && persistfile) 
    {
        recursive_call = false;
        PERFINFO_AUTO_STOP();
        StructClear(pti, structptr);
        return false;
    }

    // otherwise, we have to load from text files
    if (pli)
        pli->updated = 1;
    FileListCreate(&g_parselist);
    lf_ignoreempty = !!(flags & PARSER_EMPTYOK);
    if (!dir)
    {
        // load basic file
        if(persistfile)
            parser_relpath = allocAddString(persistfile);
        else if(filemask)
            parser_relpath = allocAddString(filemask);
        else
            parser_relpath = 0;
        ParserLoadFile(filemask, lf_ignoreempty);

        if (!(flags & PARSER_EXACTFILE)) {
            char** prefix;
            // load each of the optional versions
            prefix = g_StdAdditionalFilePrefixes;
            while (**prefix)
            {
                char* withPrefix = addFilePrefix(filemask, *prefix);
                if (fileExists(withPrefix))
                    ParserLoadFile(withPrefix, lf_ignoreempty);
                prefix++;
            }
        }
        parser_relpath = 0;
    }
    else // recurse subdirs, optional prefixes are handled just by mask matching
    {
        parser_relpath = persistfile ? allocAddString(persistfile) : 0;
        fileScanAllDataDirs(dir, ParserLoadCallback);
        parser_relpath = 0;
    }
    lf_ignoreempty = 0;
    // run the preprocessor to edit data loaded
    if (preprocessor)
    {
        if (!preprocessor(pti, structptr))
            lf_loadedok = 0;
    }

    // don't write out persist file if we had an error during load
    if (!lf_loadedonefile) {
        if (persistfile && !(flags & PARSER_OPTIONALFLAG)) {
            Errorf("ParserLoadFiles: couldn't find any files while creating %s", persistfile);
        } else {
            writeConsole(OUTPUT_VERBOSE, "ParserLoadFiles: couldn't find any files in directory %s", dir);
        }
    }
    if (persistfile)
    {
        if (!lf_loadedok) verbose_printf("ParserLoadFiles: error during parsing, not creating binary\n");
        if ((lf_loadedonefile || flags & PARSER_OPTIONALFLAG) && lf_loadedok) {
            mkdirtree(fileLocateWrite(persistfilepath, buf));
            ParserWriteBinaryFile(persistfilepath, pti, structptr, &g_parselist, globaldefines,0,excludeFlags);
        }
        else if (lf_forcebincreate)
        {
            FatalErrorf("Failed creation of %s file\n", persistfilepath);
        }
    }
    FileListDestroy(&g_parselist);
    recursive_call = false;

    PERFINFO_AUTO_STOP();

    return (lf_loadedonefile || flags & PARSER_OPTIONALFLAG) && lf_loadedok;
}

#ifndef EXTERNAL_TEST

typedef struct PLFSHeader {
    U32 crc;
} PLFSHeader;

/**
* Attempts to acquire the data from shared memory, otherwise 
* load it from the data.
*  
* @note If you want to do any processing on the data after it is 
*       loaded, you should should do it in the one of the
*       callback functions.
*  
* @param structptr The read only location in memory to write the 
*                  changes to.
*/
bool ParserLoadFilesShared(const char* sharedMemoryName, const char* dir, const char* filemask, const char* persistfile, int flags, ParseTable pti[], SHARED_MEMORY_PARAM void* sm_structptr, size_t iSize, DefineContext* globaldefines, ParserLoadInfo* pli, ParserLoadPreProcessFunc preprocessor, ParserLoadPostProcessFunc postprocessor, ParserLoadFinalProcessFunc finalprocessor)
{
    void *structptr = (void*)sm_structptr;
    void *pTemp;
    SharedMemoryHandle *shared_memory=NULL;
    SM_AcquireResult shared_memory_status;
    int forcerebuild = flags & PARSER_FORCEREBUILD;
    bool ret = false;
    PLFSHeader header;

    PERFINFO_AUTO_START("ParserLoadFilesShared", 1);

#define DefaultAction ParserLoadFiles(dir, filemask, persistfile, flags, pti, structptr, globaldefines, pli, preprocessor)

    PERFINFO_AUTO_START("top", 1);

    if (!persistfile || forcerebuild) {
        int ret = DefaultAction;
        if (postprocessor)
            ret &= postprocessor(pti, structptr) && ret;
        if (finalprocessor)
            ret &= finalprocessor(pti, structptr, false) && ret;

        PERFINFO_AUTO_STOP();
        PERFINFO_AUTO_STOP();

        return ret;
    }

    PERFINFO_AUTO_STOP_START("middle", 1);

    shared_memory_status = sharedMemoryAcquire(&shared_memory, sharedMemoryName);

    PERFINFO_AUTO_STOP_START("middle2", 1);

    switch (shared_memory_status) {
        xcase SMAR_DataAcquired:
        {
            U32 crc;
    
            PERFINFO_AUTO_STOP_START("middle3", 1);
    
            header = *(PLFSHeader*)sharedMemoryGetDataPtr(shared_memory);
            crc = ParseTableCRC(pti, globaldefines);
            if (crc!=header.crc) {
                printf("WARNING: Detected data in shared memory (%s) from a different version.  Restarting the game/mapserver/tools should fix this.", sharedMemoryName);
                shared_memory_status = SMAR_Error;
                sharedMemorySetMode(SMM_DISABLED);
            } else {
                // Data there already, just need to point to it!
                memcpy(structptr, (char*)sharedMemoryGetDataPtr(shared_memory) + sizeof(PLFSHeader), iSize);
                ret = true;
            }
        }
        xcase SMAR_Error:
        {
            PERFINFO_AUTO_STOP_START("middle4", 1);

            // An error occurred with the shared memory, just do what we would normally do
            ret = DefaultAction;
            if (postprocessor)
                ret &= postprocessor(pti, structptr);
            if (finalprocessor)
                ret &= finalprocessor(pti, structptr, false);
        }
        xcase SMAR_FirstCaller:
        {
            // Load data and copy to shared memory
            size_t size;

            PERFINFO_AUTO_STOP_START("middle5", 1);

            // Load data
            ret = DefaultAction;
            if (postprocessor) {
                ret &= postprocessor(pti, structptr);
            }

            PERFINFO_AUTO_STOP_START("middle6", 1);

            // Copy to shared memory
            header.crc = ParseTableCRC(pti, globaldefines);
            size = StructGetMemoryUsage(pti, structptr, iSize) + sizeof(PLFSHeader);
            sharedMemorySetSize(shared_memory, size);

            PERFINFO_AUTO_STOP_START("middle7", 1);

            memcpy(sharedMemoryAlloc(shared_memory, sizeof(PLFSHeader)), &header, sizeof(PLFSHeader));

            PERFINFO_AUTO_STOP_START("middle8", 1);

            pTemp = StructCompress(pti, structptr, iSize, NULL, sharedMemoryAlloc, shared_memory);

            PERFINFO_AUTO_STOP_START("middle9", 1);

            if (!(flags & PARSER_DONTFREE))
            {            
                StructClear(pti, structptr);
            }

            sharedMemoryCommit(shared_memory);
            memcpy(structptr, pTemp, iSize);

            if (finalprocessor) {
                // Allow shared heap usage in the final processor
                sharedHeapMemoryManagerLock();

                ret &= finalprocessor(pti, structptr, true);

                sharedHeapMemoryManagerUnlock();

                TODO(); // if we fix up shared memory segments to work, this copy is no longer needed
#if 1 //ndef DISABLE_SHARED_MEMORY_SEGMENT
                // Make sure the shared memory blocks are in sync
                memcpy(pTemp, structptr, iSize);
#endif
            }

            PERFINFO_AUTO_STOP_START("middle10", 1);

            sharedMemoryUnlock(shared_memory);
        }
    }

    PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();

    return ret;
}

// Attempts to load the data from shared memory. Returns 1 on success, 0 on failure
// shared_memory is modified to point to the handle returned
bool ParserLoadFromShared(const char* sharedMemoryName, int flags, ParseTable pti[], SHARED_MEMORY_PARAM void* sm_structptr, size_t iSize, DefineContext* globaldefines, SharedMemoryHandle **shared_memory)
{
    void* structptr = (void*)sm_structptr;
    SM_AcquireResult ret;
    PLFSHeader header;

    assert(shared_memory);

    ret = sharedMemoryAcquire(shared_memory, sharedMemoryName);

    if (ret==SMAR_DataAcquired) 
    {
        U32 crc;

        header = *(PLFSHeader*)sharedMemoryGetDataPtr(*shared_memory);
        crc = ParseTableCRC(pti, globaldefines);
        if (crc!=header.crc) {
            printf("WARNING: Detected data in shared memory (%s) from a different version.  Restarting the game/mapserver/tools should fix this.", sharedMemoryName);
            return false;
            sharedMemorySetMode(SMM_DISABLED);
        } else {
            // Data there already, just need to point to it!
            memcpy(structptr, (char*)sharedMemoryGetDataPtr(*shared_memory) + sizeof(PLFSHeader), iSize);
            return true;
        }
    }
    else if (ret == SMAR_Error) 
    {
        *shared_memory = NULL;
    }
    return false;
}

// Attempts to move the data to the shared memory handle that is passed in. We assume we have write access
bool ParserMoveToShared(SharedMemoryHandle *shared_memory, int flags, ParseTable pti[], SHARED_MEMORY_PARAM void* sm_structptr, size_t iSize, DefineContext* globaldefines)
{
    void* structptr = (void*)sm_structptr;
    void *pTemp;
    PLFSHeader header;    
    size_t size;    

    // Copy to shared memory
    header.crc = ParseTableCRC(pti, globaldefines);
    size = StructGetMemoryUsage(pti, structptr, iSize) + sizeof(PLFSHeader);
    sharedMemorySetSize(shared_memory, size);        
    memcpy(sharedMemoryAlloc(shared_memory, sizeof(PLFSHeader)), &header, sizeof(PLFSHeader));        
    pTemp = StructCompress(pti, structptr, iSize, NULL, sharedMemoryAlloc, shared_memory);

    if (!(flags & PARSER_DONTFREE))
    {            
        StructClear(pti, structptr);
    }
    memcpy(structptr, pTemp, iSize);

    return true;
}

void ParserCopyToShared(SharedMemoryHandle *shared_memory, SHARED_MEMORY_PARAM void* sm_structptr, size_t iSize)
{
    memcpy((char*)sharedMemoryGetDataPtr(shared_memory) + sizeof(PLFSHeader), (void*)sm_structptr, iSize);
}

void ParserUnload(ParseTable pti[], SHARED_MEMORY_PARAM void* sm_structptr, size_t iSize)
{
    void *structptr = (void*)sm_structptr;

    TODO();  // fix this next line to check to see if "(intptr_t)structptr + pti[0].storeoffset)" is in shared memory rather than structptr (which is likely a static) 
    if (!isSharedMemory(structptr))
    {
        ParserDestroyStruct(pti, structptr);
    }
    else
    {
        int i;
    
        sharedHeapMemoryManagerLock();

        FORALL_PARSEINFO(pti, i)
        {
            if (pti[i].type & TOK_REDUNDANTNAME)
                continue;
            if (pti[i].type & TOK_INDIRECT)
                sharedMemoryUnshare((void*)((intptr_t)structptr + pti[i].storeoffset));
        }

        sharedHeapMemoryManagerUnlock();
    }

    memset(structptr, 0, iSize);
}

int ParserReadText(const char *str,int str_len,ParseTable *tpi,void *struct_mem)
{
    TokenizerHandle tok;
    int                fileisgood;

    if (!str)
        return 0;
    tok = TokenizerCreateString(str, str_len);
    if (!tok)
        return 0;
    g_disableLastAuthor = 1;
    fileisgood = ParserReadTokenizer(tok, tpi, struct_mem, ParserErrorCallback);
    g_disableLastAuthor = 0;
    TokenizerDestroy(tok);
    return fileisgood;
}

int ParserWriteText(char **estr,ParseTable *tpi,void *struct_mem, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    StuffBuff    sbBuff;
    FILE        *fpBuff;
    int            ok;

    if (!estr)
        return 0;
    initStuffBuff(&sbBuff, 128);
    fpBuff = fileOpenStuffBuff(&sbBuff);
    ok = InnerWriteTextFile(fpBuff, tpi, struct_mem, -1, iOptionFlagsToMatch, iOptionFlagsToExclude);

    estrConcatFixedWidth(estr, sbBuff.buff, sbBuff.idx);
    fileClose(fpBuff);
    freeStuffBuff(&sbBuff);

    return ok;
}

int ParserReadTextEscaped(char **str, ParseTable *tpi, void *struct_mem)
{
    char *start, *end;
    char *tempbuffer;
    int len;
    TokenizerHandle tok;
    int                fileisgood;

    if (!str || !*str)
        return 0;
    start = *str;
    start = strstr(start,"<&");
    if (!start)
    {
        // No start found
        return 0;
    }
    start++; start++;
    end = strstr(start,"&>");
    if (!end)
    {
        // Incomplete
        return 0;
    }
    *end = '\0';
    len = end - start;
    tempbuffer = malloc(len + 1);
    len = TokenizerUnescape(tempbuffer,start);
    *end = '&';
    end++;
    end++;

    // tempbuffer contains the unescaped string, and end is pointing after the struct

    tok = TokenizerCreateString(tempbuffer, len);
    if (!tok)
        return 0;

    g_disableLastAuthor = 1;
    fileisgood = ParserReadTokenizer(tok, tpi, struct_mem, ParserErrorCallback);
    g_disableLastAuthor = 0;
    TokenizerDestroy(tok);
    SAFE_FREE(tempbuffer);

    *str = end;

    return fileisgood;
}


int ParserWriteTextEscaped(char **estr, ParseTable *tpi, void *struct_mem, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    StuffBuff    sbBuff;
    FILE        *fpBuff;
    int            ok;
    char *start;
    int    newlen;
    unsigned int estrlen;

    if (!estr)
    {
        return 0;
    }

    initStuffBuff(&sbBuff, 128);
    fpBuff = fileOpenStuffBuff(&sbBuff);
    ok = InnerWriteTextFile(fpBuff, tpi, struct_mem, -1, iOptionFlagsToMatch, iOptionFlagsToExclude);

    if (!ok)
        return ok;

    addSingleStringToStuffBuff(&sbBuff,""); //cap it with a null

    estrlen = estrLength(estr);

    estrReserveCapacity(estr,estrlen + sbBuff.idx * 2 + 4); //This is the theoretical max space usage
    start = (*estr)+estrlen;

    newlen = TokenizerEscape(start,sbBuff.buff);

    estrSetLengthNoMemset(estr,estrlen + newlen);

    fileClose(fpBuff);
    freeStuffBuff(&sbBuff);
    
    return ok;
}


int ParserReadBin(char *bin,U32 num_bytes,ParseTable *tpi,void *struct_mem)
{
    int sum = num_bytes;
    int                ok;
    SimpleBufHandle    sbuf;

    if (!bin)
        return 0;
    sbuf = SimpleBufSetData(bin,num_bytes);
    g_disableLastAuthor = 1;
    ok = ParserReadBinaryTable(sbuf, tpi, struct_mem, &sum);
    g_disableLastAuthor = 0;
    free(sbuf);

    return ok;
}

int ParserWriteBin(char **estr,ParseTable *tpi,void *struct_mem, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    U8                *mem;
    U32                ok,len,num_bytes;
    SimpleBufHandle    sbuf;

    sbuf = SimpleBufOpenWrite("",0);
    ok = ParserWriteBinaryTable(sbuf, tpi, struct_mem, &num_bytes,iOptionFlagsToMatch,iOptionFlagsToExclude);
    assert(num_bytes == (U32)SimpleBufTell(sbuf));
    SimpleBufSeek(sbuf, 0, 0);

    mem = SimpleBufGetData(sbuf,&len);
    estrConcatFixedWidth(estr, mem, len);
    SimpleBufClose(sbuf);
    return ok;
}




#endif // EXTERNAL_TEST

// *********************************************************************************
//  textparser internal tests
// *********************************************************************************

typedef struct _SingleLineStruct {
    int        first_int;
    char*    second_char;
    F32        third_float;
    REF_TO(RTHObject) fourth_ref;
} SingleLineStruct;

typedef struct _StructIntArray {
    int*    array;
} StructIntArray;

typedef struct _RawSubstructure {
    char    a;
    int        foo;
    int        yada;
} RawSubstructure;

// just some testing of parser capabilities stuck in here
typedef struct _TestBlock {
    int        maskfield;
    int**    intstruct;
    F32*    floatarray;
    F32        testinitfloat;
    int*    intarray;
    int        rawdata[4];
    char*    definefield;
    REF_TO(RTHObject) testref;

    RawSubstructure rawstructs;

    StructParams** structline;
    SingleLineStruct** singlelinestruct;
    StructIntArray** structintarray;
    StructFunctionCall** functioncall;

} TestBlock;

typedef struct TestBlockList {
    TestBlock**    testblocks;
} TestBlockList;

ParseTable ParseSingleLineStruct[] = {
    { "",    TOK_STRUCTPARAM | TOK_INT(SingleLineStruct,first_int,0) },
    { "",    TOK_STRUCTPARAM | TOK_STRING(SingleLineStruct,second_char,0) },
    { "",    TOK_STRUCTPARAM | TOK_F32(SingleLineStruct,third_float,0) },
//    { "",    TOK_STRUCTPARAM | TOK_REFERENCE(SingleLineStruct,fourth_ref,0,"TestHarness") },
    { "\n",            TOK_END,        0 },
    { "", 0, 0 }
};

ParseTable ParseStructIntArray[] = {
    { "",    TOK_STRUCTPARAM | TOK_INTARRAY(StructIntArray,array) },
    { "\n",            TOK_END,        0 },
    { "", 0, 0 }
};

DefineContext* pFlagDefines = NULL;
DefineContext* pDynDefines = NULL;

StaticDefineInt maskdefines[] = {
    DEFINE_INT
    { "flagOne",    1 },
    { "flagTwo",    2 },
    { "flagFour",    4 },
    DEFINE_EMBEDDYNAMIC_INT(pFlagDefines)
    DEFINE_END
};

STATIC_DEFINE_WRAPPER(ParseDynDefines, pDynDefines);

ParseTable ParseTestBlock[] = {
    { "{",            TOK_START,    0 },
    { "maskfield",    TOK_FLAGS(TestBlock,maskfield, 0),        maskdefines },
    { "definefield", TOK_STRING(TestBlock,definefield, 0), ParseDynDefines },
    { "floatarray", TOK_F32ARRAY(TestBlock,floatarray) },
    { "testinitfloat", TOK_F32(TestBlock,testinitfloat, -1) },
    { "intarray",        TOK_INTARRAY(TestBlock,intarray) },
//    { "reffield",    TOK_REFERENCE(TestBlock,testref,0,"TestHarness") },
    { "singlelinestruct",    TOK_STRUCT(TestBlock,singlelinestruct,ParseSingleLineStruct) },
    { "structintarray",        TOK_STRUCT(TestBlock,structintarray,ParseStructIntArray) },
    { "functioncall", TOK_FUNCTIONCALL(TestBlock,functioncall)    },
    { "}",            TOK_END,        0 },
    { "", 0, 0 }
};

ParseTable ParseTestBlockList[] = {
    { "TestBlock",  TOK_STRUCT(TestBlockList,testblocks, ParseTestBlock) },
    { "", 0, 0 }
};

DefineList g_moredefines[] = {
    { "One",            "1" },
    { "Two",            "2" },
    { 0, 0 }
};

DefineIntList g_flags[] = {
    { "flagEight",        8 },
    { "flagSixteen",    16 },
    { 0, 0 }
};

DefineList g_otherdefines[] = {
    { "Foo",            "Bar" },
    { "Good",            "Food" },
    { "Holy",            "Metal" },
    { 0, 0 }
};

static char* parserdebug_testinput = 
"TestBlock\n"
"{\n"
"    maskfield flagOne, flagTwo, flagFour, flagEight\n"
"    definefield Holy\n"
"    floatarray 1.0 1.5 2.0 2.5 3.0 -1 -2 -3 2 1 // 2.54 will be added\n"
"    // let the test float stay\n"
//"    reffield \"<Object 1>\"\n"
"    intarray 1 2 3 4\n"
//"    singlelinestruct 2, Hello, 2.5, \"<Object 2>\"\n"
//"    singlelinestruct 3, Jonathan, 3.5, \"<Object 3>\"\n"
"    singlelinestruct 2, Hello, 2.5\n"
"    singlelinestruct 3, Jonathan, 3.5\n"
"    structintarray 456 789 1000\n"
"    structintarray\n"
"    structintarray 2\n"
"} \n"
"\n"
"TestBlock\n"
"{ \n"
"    maskfield 2147483648, 1\n"
"    definefield Curmudgeon\n"
"    testinitfloat 2.5\n"
"    functioncall f1(\"a())))\", \"(b\", \")c\") f2(d, e, f) f3(g h i)\n"
"}\n";

static char* parserdebug_nodefines =
"TestBlock\n"
"{\n"
"    maskfield 1, 2, 4, 8\n"
"    definefield Metal\n"
"    floatarray 1.0 1.5 2.0 2.5 3.0 -1 -2 -3 2 1 // 2.54 will be added\n"
"    // let the test float stay\n"
//"    reffield \"<Object 1>\"\n"
"    intarray 1 2 3 4\n"
//"    singlelinestruct 2, Hello, 2.5, \"<Object 2>\"\n"
//"    singlelinestruct 3, Jonathan, 3.5, \"<Object 3>\"\n"
"    singlelinestruct 2, Hello, 2.5\n"
"    singlelinestruct 3, Jonathan, 3.5\n"
"    structintarray 456 789 1000\n"
"    structintarray\n"
"    structintarray 2\n"
"} \n"
"\n"
"TestBlock\n"
"{ \n"
"    maskfield 2147483648, 1\n"
"    definefield Curmudgeon\n"
"    testinitfloat 2.5\n"
"    functioncall f1(\"a())))\", \"(b\", \")c\") f2(d, e, f) f3(g h i)\n"
"}\n";

static TestBlockList parserdebug_fromtext, parserdebug_fromfile, parserdebug_frombin, parserdebug_fromschema;

void TestTokenTable(void)
{
    int i;
    for (i = TOK_IGNORE; i < NUM_TOK_TYPE_TOKENS; i++)
        assert(TYPE_INFO(i).type == (U32)i);
}

void TestTextParser(void)
{
    ParseTable** schema = 0;
    int size = 0;

    RefSystem_Init();
    //RTH_Test();
    TestTokenTable();

    // defines only for testing
    printf("setting up textparser defines & clearing old structs ..");

    if (pFlagDefines)
        DefineDestroy(pFlagDefines);
    if (pDynDefines)
        DefineDestroy(pDynDefines);
    pFlagDefines = DefineCreateFromIntList(g_flags);
    pDynDefines = DefineCreateFromList(g_otherdefines);
    DefineSetHigherLevel(pDynDefines, pFlagDefines);

    StructClear(ParseTestBlockList, &parserdebug_fromtext);
    StructClear(ParseTestBlockList, &parserdebug_fromfile);
    StructClear(ParseTestBlockList, &parserdebug_frombin);
    printf("success\n");

    printf("loading from test string ..");
    if (ParserReadText(parserdebug_testinput, -1, ParseTestBlockList, &parserdebug_fromtext))
        printf("success\n");
    else
        printf("failed!\n");

    printf("writing test file ..");
    if (ParserWriteTextFile("c:\\parsertest.txt", ParseTestBlockList, &parserdebug_fromtext, 0, 0))
        printf("success\n");
    else
        printf("failed!\n");

    printf("reading test file ..");
    if (ParserLoadFiles(NULL, "c:\\parsertest.txt", "parsertest.bin", PARSER_FORCEREBUILD, 
        ParseTestBlockList, &parserdebug_fromfile, NULL, NULL, NULL))
        printf("success\n");
    else
        printf("failed!\n");
    ParserWriteTextFile("c:\\parsertest.fromfile.txt", ParseTestBlockList, &parserdebug_fromfile, 0, 0);

    printf("comparing file to test string ..");
    if (ParserCompareStruct(ParseTestBlockList, &parserdebug_fromtext, &parserdebug_fromfile))
        printf("different!\n");
    else
        printf("success\n");

    printf("getting precompiled result from .bin file ..");
    if (ParserLoadFiles(NULL, "c:\\parsertest.txt", "parsertest.bin", 0, 
        ParseTestBlockList, &parserdebug_frombin, NULL, NULL, NULL))
        printf("success\n");
    else
        printf("failed!\n");
    ParserWriteTextFile("c:\\parsertest.frombin.txt", ParseTestBlockList, &parserdebug_frombin, 0, 0);

    printf("comparing bin file to text file ..");
    if (ParserCompareStruct(ParseTestBlockList, &parserdebug_fromfile, &parserdebug_frombin))
        printf("different!\n");
    else
        printf("success\n");

    printf("writing schema to disk ..");
    if (ParseInfoWriteTextFile("c:\\parsertest.schema.txt", ParseTestBlockList))
        printf("success\n");
    else
        printf("failed!\n");

    printf("reading schema from disk ..");
    if (ParseInfoReadTextFile("c:\\parsertest.schema.txt", &schema, &size))
        printf("success\n");
    else printf("failed!\n");

    if (schema)
    {
        printf("writing schema to disk again ..");
        if (ParseInfoWriteTextFile("c:\\parsertest.schema2.txt", schema[0]))
            printf("success\n");
        else
            printf("failed!\n");

        printf("using schema to load text ..");
        if (size > sizeof(TestBlockList))
            printf("failed - I need a larger block than it was stored in??\n");
        else if (ParserReadText(parserdebug_nodefines, -1, schema[0], &parserdebug_fromschema))
            printf("success\n");
        else printf("failed!\n");

        printf("writing to disk using schema ..");
        if (ParserWriteTextFile("c:\\parsertest.fromschema.txt", schema[0], &parserdebug_fromschema, 0, 0))
            printf("success\n");
        else printf("failed!\n");
    }

    printf("");

    // close out defines
    ParseInfoFreeAll(&schema);
    DefineDestroy(pFlagDefines);
    DefineDestroy(pDynDefines);
    pFlagDefines = 0;
    pDynDefines = 0;

    printf("Finished!\n");
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////// TOKEN PRIMITIVES

#define MEM_ALIGN(addr, size) if ((addr) % size) { addr += (size - ((addr) % size)); }

ParseInfoFieldUsage ignore_interpretfield(ParseTable tpi[], int column, ParseInfoField field)
{
    return NoFieldUsage;
}

int ignore_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    // ignore the rest of the line
    const char* nexttoken = TokenizerPeek(tok, 0, 1);
    while (nexttoken && !IsEol(nexttoken))
    {
        TokenizerGet(tok, 0, 1);
        nexttoken = TokenizerPeek(tok, 0, 1);
    }
    return 0;
}

// we give a warning and refuse to parse this field
int error_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    ErrorFilenamef(TokenizerGetFileName(tok),
        "Found unparsable field %s in %s\n", tpi[column].name, TokenizerGetFileAndLine(tok));
    return ignore_parse(tok, tpi, column, structptr, 0, callback); // ignore the rest of the (incorrect) line for convenience
}

// we give a warning and refuse to parse this field
int deprecated_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    ErrorFilenamef(TokenizerGetFileName(tok),
        "deprecated field %s in %s\n", tpi[column].name, TokenizerGetFileAndLine(tok));
    return ignore_parse(tok, tpi, column, structptr, 0, callback); // ignore the rest of the (incorrect) line for convenience
}

void error_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    Errorf("Trying to send an invalid token type!  (xx_senddiff not implemented)");
}

void error_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    Errorf("Trying to receive an invalid token type!  (xx_recvdiff not implemented)");
}

int end_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    return 1; // terminate the surrounding struct
}

ParseInfoFieldUsage number_interpretfield(ParseTable tpi[], int column, ParseInfoField field)
{
    switch (field)
    {
    case ParamField:
        if (tpi[column].type & TOK_FIXED_ARRAY)
            return NumberOfElements;
        if (tpi[column].type & TOK_EARRAY)
            return NoFieldUsage;
        return DefaultValue;
    case SubtableField:
        return StaticDefineList;
    }
    return NoFieldUsage;
}

void u8_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    TokenStoreSetU8(tpi, column, structptr, index, tpi[column].param);
}

int u8_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    int i;
    const char* nexttoken = GetNextParameter(tok);
    if (!nexttoken) return 0;
    i = GetInteger(tok, nexttoken);
    if (i < 0 || i > 255)
    {
        ErrorFilenamef(TokenizerGetFileName(tok),
            "Parser: U8 parameter exceeds 0..255 in %s\n", TokenizerGetFileAndLine(tok));
        i = 0;
        g_parseerr = 1;
    }
    TokenStoreSetU8(tpi, column, structptr, index, i);
    return 0;
}

void u8_writetext(FILE* out, ParseTable tpi[], int column, const void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    U8 value = TokenStoreGetU8(tpi, column, structptr, index);
    if (showname && !value && !tpi[column].param) return; // if defaulted to zero, not necessary to write field
    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);
    WriteInt(out, value, 0, showname, tpi[column].subtable);
}

int u8_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int success = 1;
    int value = TokenStoreGetU8(tpi, column, structptr, index);
    success = success && SimpleBufWriteU32(value, file);
    *datasum += sizeof(int);
    return success;
}

int u8_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int ok = 1;
    int value = 0;
    ok = ok && SimpleBufReadU32((U32*)&value, file);
    *datasum += sizeof(int);
    TokenStoreSetU8(tpi, column, structptr, index, (U8)value);
    return ok;
}

void u8_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    if (sendAbsolute)
        pktSendBitsPack(pak, TOK_GET_PRECISION_DEF(tpi[column].type, 1), TokenStoreGetU8(tpi, column, newstruct, index));
    else
    {
        int d = (int)TokenStoreGetU8(tpi, column, newstruct, index) - (int)TokenStoreGetU8(tpi, column, oldstruct, index);
        packDelta(pak, d);
    }
}

void u8_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    int out_of_order = 0;
    if (pktidptr) {
        out_of_order = PTR_TO_U32(*pktidptr) > pak->id;
        if (absValues && !out_of_order) // only store packet id's on full updates
            *pktidptr = U32_TO_PTR(pak->id);
    }
    if (absValues)
    {
        int data = pktGetBitsPack(pak, TOK_GET_PRECISION_DEF(tpi[column].type, 1));
        if (!out_of_order && structptr) TokenStoreSetU8(tpi, column, structptr, index, data);
    }
    else
    {
        int data = unpackDelta(pak);
        if (!out_of_order && structptr) 
        {
            data += (int)TokenStoreGetU8(tpi, column, structptr, index);
            TokenStoreSetU8(tpi, column, structptr, index, data);
        }
    }
}

bool u8_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    int val = TokenStoreGetU8(tpi, column, structptr, index);
    sprintf_s(str, str_size, "%d", val);
    return true;
}

bool u8_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    int val = 0;
    bool ret = false;
    if (sscanf(str, "%d", &val) == 1) ret = true;
    if (val < 0 || val > 255) ret = false;
    if (ret) TokenStoreSetU8(tpi, column, structptr, index, val);
    return ret;
}

void u8_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    U8 value = TokenStoreGetU8(tpi, column, structptr, index);
    cryptAdler32Update(&value, sizeof(U8));
}

int u8_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    U8 left = TokenStoreGetU8(tpi, column, lhs, index);
    U8 right = TokenStoreGetU8(tpi, column, rhs, index);
    return (int)left - (int)right;
}

void u8_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    tpi[column].storeoffset = *size;
    (*size) += sizeof(U8);
}

void u8_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    TokenStoreSetU8(tpi, column, dest, index, TokenStoreGetU8(tpi, column, src, index));
}

void u8_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam)
{
    U8 result = interpU8(interpParam, TokenStoreGetU8(tpi, column, structA, index), TokenStoreGetU8(tpi, column, structB, index));
    TokenStoreSetU8(tpi, column, destStruct, index, result);
}

void u8_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime)
{
    F32 result = ((F32)TokenStoreGetU8(tpi, column, structB, index) - (F32)TokenStoreGetU8(tpi, column, structA, index)) / deltaTime;
    TokenStoreSetU8(tpi, column, destStruct, index, round32(result));
}

void u8_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime)
{
    F32 result = (F32)TokenStoreGetU8(tpi, column, valueStruct, index) + deltaTime * (F32)TokenStoreGetU8(tpi, column, rateStruct, index);
    TokenStoreSetU8(tpi, column, destStruct, index, round32(result));
}

void u8_calccyclic(ParseTable tpi[], int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, int index, F32 fStartTime, F32 deltaTime)
{
    F32 freq = TokenStoreGetU8(tpi, column, freqStruct, index);
    F32 cycle = TokenStoreGetU8(tpi, column, cycleStruct, index);
    F32 fSinDiff = ( sinf( (fStartTime + deltaTime) * freq + cycle * TWOPI) - sinf( ( fStartTime * freq + cycle ) * TWOPI ));
    F32 result = TokenStoreGetU8(tpi, column, valueStruct, index) + TokenStoreGetU8(tpi, column, ampStruct, index) * fSinDiff;
    TokenStoreSetU8(tpi, column, destStruct, index, round32(result));
}

void u8_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed)
{
    switch (optype)
    {
    case DynOpType_Jitter:
    {
        int iToAdd = round32(*values * randomF32Seeded(seed, RandType_BLORN));
        U8 result = CLAMP(TokenStoreGetU8(tpi, column, dstStruct, index) + iToAdd, 0, 255);
        TokenStoreSetU8(tpi, column, dstStruct, index, result);
    } break;
    case DynOpType_Inherit:
    {
        if (srcStruct) TokenStoreSetU8(tpi, column, dstStruct, index, TokenStoreGetU8(tpi, column, srcStruct, index));
    } break;
    case DynOpType_Add:
    {
        TokenStoreSetU8(tpi, column, dstStruct, index, CLAMP((int)TokenStoreGetU8(tpi, column, dstStruct, index) + round32(*values), 0, 255));
    } break;
    case DynOpType_Multiply:
    {
        TokenStoreSetU8(tpi, column, dstStruct, index, CLAMP(round32((F32)TokenStoreGetU8(tpi, column, dstStruct, index) * *values), 0, 255));
    } break;
    case DynOpType_Min:
    {
        TokenStoreSetU8(tpi, column, dstStruct, index, MIN(CLAMP(round32(*values), 0, 255), TokenStoreGetU8(tpi, column, dstStruct, index)));
    } break;
    case DynOpType_Max:
    {
        TokenStoreSetU8(tpi, column, dstStruct, index, MAX(CLAMP(round32(*values), 0, 255), TokenStoreGetU8(tpi, column, dstStruct, index)));
    } break;
    }
}

void int16_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    TokenStoreSetInt16(tpi, column, structptr, index, tpi[column].param);
}

int int16_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    const char* nexttoken = GetNextParameter(tok);
    if (!nexttoken) return 0;
    TokenStoreSetInt16(tpi, column, structptr, index, GetInteger(tok, nexttoken));
    return 0;
}

void int16_writetext(FILE* out, ParseTable tpi[], int column, const void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    S16 value = TokenStoreGetInt16(tpi, column, structptr, index);
    if (showname && !value && !tpi[column].param) return; // if defaulted to zero, not necessary to write field
    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);
    WriteInt(out, value, 0, showname, tpi[column].subtable);
}

int int16_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int success = 1;
    int value = TokenStoreGetInt16(tpi, column, structptr, index);
    success = success && SimpleBufWriteU32(value, file);
    *datasum += sizeof(int);
    return success;
}

int int16_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int ok = 1;
    int value = 0;
    ok = ok && SimpleBufReadU32((U32*)&value, file);
    *datasum += sizeof(int);
    TokenStoreSetInt16(tpi, column, structptr, index, (S16)value);
    return ok;
}

void int16_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    if (sendAbsolute)
        pktSendBitsPack(pak, TOK_GET_PRECISION_DEF(tpi[column].type, 1), TokenStoreGetInt16(tpi, column, newstruct, index));
    else
    {
        int d = TokenStoreGetInt16(tpi, column, newstruct, index) - TokenStoreGetInt16(tpi, column, oldstruct, index);
        packDelta(pak, d);
    }
}

void int16_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    int out_of_order = 0;
    if (pktidptr) {
        out_of_order = PTR_TO_U32(*pktidptr) > pak->id;
        if (absValues && !out_of_order) // only store packet id's on full updates
            *pktidptr = U32_TO_PTR(pak->id);
    }
    if (absValues)
    {
        int data = pktGetBitsPack(pak, TOK_GET_PRECISION_DEF(tpi[column].type, 1));
        if (!out_of_order && structptr) TokenStoreSetInt16(tpi, column, structptr, index, data);
    }
    else
    {
        int data = unpackDelta(pak);
        if (!out_of_order && structptr)
        {
            data += (int)TokenStoreGetInt16(tpi, column, structptr, index);
            TokenStoreSetInt16(tpi, column, structptr, index, data);
        }
    }
}

bool int16_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    int val = TokenStoreGetInt16(tpi, column, structptr, index);
    sprintf_s(str, str_size, "%d", val);
    return true;
}

bool int16_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    int val = 0;
    bool ret = false;
    if (sscanf(str, "%d", &val) == 1) ret = true;
    if (val < SHRT_MIN || val > SHRT_MAX) ret = false;
    if (ret) TokenStoreSetInt16(tpi, column, structptr, index, val);
    return ret;
}

void int16_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    S16 value = TokenStoreGetInt16(tpi, column, structptr, index);
    cryptAdler32Update((U8*)&value, sizeof(S16));
}

int int16_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    S16 left = TokenStoreGetInt16(tpi, column, lhs, index);
    S16 right = TokenStoreGetInt16(tpi, column, rhs, index);
    return left - right;
}

void int16_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    MEM_ALIGN(*size, sizeof(S16));
    tpi[column].storeoffset = *size;
    (*size) += sizeof(S16);
}

void int16_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    TokenStoreSetInt16(tpi, column, dest, index, TokenStoreGetInt16(tpi, column, src, index));
}

void int16_endianswap(ParseTable tpi[], int column, void* structptr, int index)
{
    TokenStoreSetInt16(tpi, column, structptr, index, endianSwapU16(TokenStoreGetInt16(tpi, column, structptr, index)));
}

void int16_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam)
{
    S16 result = interpS16(interpParam, TokenStoreGetInt16(tpi, column, structA, index), TokenStoreGetInt16(tpi, column, structB, index));
    TokenStoreSetInt16(tpi, column, destStruct, index, result);
}

void int16_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime)
{
    F32 result = ((F32)TokenStoreGetInt16(tpi, column, structB, index) - (F32)TokenStoreGetInt16(tpi, column, structA, index)) / deltaTime;
    TokenStoreSetInt16(tpi, column, destStruct, index, round32(result));
}

void int16_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime)
{
    F32 result = (F32)TokenStoreGetInt16(tpi, column, valueStruct, index) + deltaTime * (F32)TokenStoreGetInt16(tpi, column, rateStruct, index);
    TokenStoreSetInt16(tpi, column, destStruct, index, round32(result));
}

void int16_calccyclic(ParseTable tpi[], int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, int index, F32 fStartTime, F32 deltaTime)
{
    F32 freq = TokenStoreGetInt16(tpi, column, freqStruct, index);
    F32 cycle = TokenStoreGetInt16(tpi, column, cycleStruct, index);
    F32 fSinDiff = ( sinf( (fStartTime + deltaTime) * freq + cycle * TWOPI) - sinf( ( fStartTime * freq + cycle ) * TWOPI ));
    F32 result = TokenStoreGetInt16(tpi, column, valueStruct, index) + TokenStoreGetInt16(tpi, column, ampStruct, index) * fSinDiff;
    TokenStoreSetInt16(tpi, column, destStruct, index, round32(result));
}

void int16_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed)
{
    switch (optype)
    {
    case DynOpType_Jitter:
    {
        int iToAdd = round32(*values * randomF32Seeded(seed, RandType_BLORN));
        S16 result = CLAMP(TokenStoreGetInt16(tpi, column, dstStruct, index) + iToAdd, SHRT_MIN, SHRT_MAX);
        TokenStoreSetInt16(tpi, column, dstStruct, index, result);
    } break;
    case DynOpType_Inherit:
    {
        if (srcStruct) TokenStoreSetInt16(tpi, column, dstStruct, index, TokenStoreGetInt16(tpi, column, srcStruct, index));
    } break;
    case DynOpType_Add:
    {
        TokenStoreSetInt16(tpi, column, dstStruct, index, CLAMP((int)TokenStoreGetInt16(tpi, column, dstStruct, index) + round32(*values), SHRT_MIN, SHRT_MAX));
    } break;
    case DynOpType_Multiply:
    {
        TokenStoreSetInt16(tpi, column, dstStruct, index, CLAMP(round32((F32)TokenStoreGetInt16(tpi, column, dstStruct, index) * *values), SHRT_MIN, SHRT_MAX));
    } break;
    case DynOpType_Min:
    {
        TokenStoreSetInt16(tpi, column, dstStruct, index, MIN(CLAMP(round32(*values), SHRT_MIN, SHRT_MAX), TokenStoreGetInt16(tpi, column, dstStruct, index)));
    } break;
    case DynOpType_Max:
    {
        TokenStoreSetInt16(tpi, column, dstStruct, index, MAX(CLAMP(round32(*values), SHRT_MIN, SHRT_MAX), TokenStoreGetInt16(tpi, column, dstStruct, index)));
    } break;
    }
}

void int_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    TokenStoreSetInt(tpi, column, structptr, index, tpi[column].param);
}

int int_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    int value;
    const char* nexttoken = GetNextParameter(tok);
    if (!nexttoken) return 0;
    if (TOK_GET_FORMAT_OPTIONS(tpi[column].format) == TOK_FORMAT_IP)
        value = GetIP(tok, nexttoken);
    else if(TOK_GET_FORMAT_OPTIONS(tpi[column].format) == TOK_FORMAT_DATESS2000)
    {
        value = timerGetSecondsSince2000FromString(nexttoken);
        if(!value)
            ErrorFilenamef(TokenizerGetFileName(tok),
                           "Parser:got %s, (parsed=%i) date value in %s\n", nexttoken, value, TokenizerGetFileAndLine(tok));
    }
    else
        value = GetInteger(tok, nexttoken);
    TokenStoreSetInt(tpi, column, structptr, index, value);
    return 0;
}

bool int_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint);

void int_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    char str[128];
    int value = TokenStoreGetInt(tpi, column, structptr, index);
    if (showname && !value && !tpi[column].param) return; // if defaulted to zero, not necessary to write field

    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);
    int_tosimple(tpi, column, structptr, index, SAFESTR(str), 0);
    WriteString(out, str, 0, showname);
}

int int_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int success = 1;
    int value = TokenStoreGetInt(tpi, column, structptr, index);
    success = success && SimpleBufWriteU32(value, file);
    *datasum += sizeof(int);
    return success;
}

int int_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int ok = 1;
    int value = 0;
    ok = ok && SimpleBufReadU32((U32*)&value, file);
    *datasum += sizeof(int);
    TokenStoreSetInt(tpi, column, structptr, index, value);
    return ok;
}

void int_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    if (sendAbsolute)
        pktSendBitsPack(pak, TOK_GET_PRECISION_DEF(tpi[column].type, 1), TokenStoreGetInt(tpi, column, newstruct, index));
    else
    {
        int d = TokenStoreGetInt(tpi, column, newstruct, index) - TokenStoreGetInt(tpi, column, oldstruct, index);
        packDelta(pak, d);
    }
}

void int_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    int out_of_order = 0;
    if (pktidptr) {
        out_of_order = PTR_TO_U32(*pktidptr) > pak->id;
        if (absValues && !out_of_order) // only store packet id's on full updates
            *pktidptr = U32_TO_PTR(pak->id);
    }
    if (absValues)
    {
        int data = pktGetBitsPack(pak, TOK_GET_PRECISION_DEF(tpi[column].type, 1));
        if (!out_of_order && structptr) TokenStoreSetInt(tpi, column, structptr, index, data);
    }
    else
    {
        int data = unpackDelta(pak);
        if (!out_of_order && structptr)
        {
            data += TokenStoreGetInt(tpi, column, structptr, index);
            TokenStoreSetInt(tpi, column, structptr, index, data);
        }
    }
}

bool int_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    int value = TokenStoreGetInt(tpi, column, structptr, index);
    int format = TOK_GET_FORMAT_OPTIONS(tpi[column].format);

    // most formats should only be printed with prettyprint
    if (prettyprint)
    {
        // these are reversible
        if (format != TOK_FORMAT_IP && format != TOK_FORMAT_PERCENT && format != TOK_FORMAT_UNSIGNED)
            format = 0;
    }

    switch (format)
    {
    case TOK_FORMAT_IP:
        strcpy_s(str, str_size, makeIpStr(value));
    xcase TOK_FORMAT_BYTES:
        {
            UnitSpec* spec;
            spec = usFindProperUnitSpec(byteSpec, value);

            if(1 != spec->unitBoundary) {
                sprintf_s(str, str_size, "%6.2f", (float)value / spec->unitBoundary);
            } else {
                sprintf_s(str, str_size, "%6d", value);
            }
            strcat_s(str, str_size, " ");
            strcat_s(str, str_size, spec->unitName);
        }
    xcase TOK_FORMAT_KBYTES:
        {
            UnitSpec* spec;
            spec = usFindProperUnitSpec(kbyteSpec, value);

            if(1 != spec->unitBoundary) {
                sprintf_s(str, str_size, "%6.2f", (float)value / spec->unitBoundary);
            } else {
                sprintf_s(str, str_size, "%6d", value);
            }
            strcat_s(str, str_size, " ");
            strcat_s(str, str_size, spec->unitName);
        }
    xcase TOK_FORMAT_MBYTES:
        {
            UnitSpec* spec;
            spec = usFindProperUnitSpec(mbyteSpec, value);

            if(1 != spec->unitBoundary) {
                sprintf_s(str, str_size, "%6.2f", (float)value / spec->unitBoundary);
            } else {
                sprintf_s(str, str_size, "%6d", value);
            }
            strcat_s(str, str_size, " ");
            strcat_s(str, str_size, spec->unitName);
        }
    xcase TOK_FORMAT_MICROSECONDS:
        {
            UnitSpec* spec;
            spec = usFindProperUnitSpec(microSecondSpec, value);

            if(1 != spec->unitBoundary) {
                sprintf_s(str, str_size, "%6.2f", (float)value / spec->unitBoundary);
            } else {
                sprintf_s(str, str_size, "%6d", value);
            }
            strcat_s(str, str_size, " ");
            strcat_s(str, str_size, spec->unitName);
        }
    xcase TOK_FORMAT_FRIENDLYDATE:
        printDate_s(value, str, str_size);
    xcase TOK_FORMAT_FRIENDLYSS2000:
        printDate_s(timerMakeTimeFromSecondsSince2000(value), str, str_size);
    xcase TOK_FORMAT_DATESS2000:
    {
        char datebuf[100];
        timerMakeDateStringFromSecondsSince2000(datebuf, value);
        sprintf_s(str, str_size, "\"%u %s\"", value, datebuf);
    }
    xcase TOK_FORMAT_FRIENDLYCPU:
        printDate_s(timerMakeTimeFromCpuSeconds(value), str, str_size);
    xcase TOK_FORMAT_PERCENT:
        sprintf_s(str, str_size, "%02d%%", value);
    xcase TOK_FORMAT_UNSIGNED:
        sprintf_s(str, str_size, "%u", value);
    xdefault:
        // we do reverse define lookup here if possible
        {
            const char* rev = NULL;
            if (tpi[column].subtable)
                rev = StaticDefineIntRevLookup(tpi[column].subtable, value);
            if (rev)
                sprintf_s(str, str_size, "%s", rev);
            else
                sprintf_s(str, str_size, "%d", value);
        }
    };
    return true;
}

bool int_fromsimple(ParseTable tpi[], int column, void* structptr, int index, const char* str)
{
    int val;
    bool ret = false;

    if (TOK_GET_FORMAT_OPTIONS(tpi[column].format) == TOK_FORMAT_IP)
    {
        val = ipFromString(str);
        if (val || stricmp(str, "0.0.0.0")==0)
            ret = true;
    }
    else if (TOK_GET_FORMAT_OPTIONS(tpi[column].format) == TOK_FORMAT_UNSIGNED)
    {
        if (sscanf(str, "%u", &val) == 1) ret = true;
    }
    else
    {
        // do define lookup if possible
        if (tpi[column].subtable)
        {
            const char* repl = StaticDefineIntLookup(tpi[column].subtable, str);
            if (repl) str = repl; // then scan below
        }
        if (sscanf(str, "%d", &val) == 1) ret = true;
    }
    if (ret) TokenStoreSetInt(tpi, column, structptr, index, val);
    return ret;
}

void int_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    int value = TokenStoreGetInt(tpi, column, structptr, index);
    cryptAdler32Update((U8*)&value, sizeof(int));
}

int int_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    int left = TokenStoreGetInt(tpi, column, lhs, index);
    int right = TokenStoreGetInt(tpi, column, rhs, index);
    int i, format = TOK_GET_FORMAT_OPTIONS(tpi[column].format);
    if (format == TOK_FORMAT_IP)
    {
        // MAK - is this correct?  not endian compatible, but it looks like
        // makeIpStr does endian swapping later.  I'm just using the existing code.
        for (i = 0; i < sizeof(int); i++) {
            int ret = cmp8(((U8*)&left)+i, ((U8*)&right)+i, 1);
            if (ret)
                return ret;
        }
    }
    else
    {
        return left - right;
    }
    return 0;
}

void int_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    MEM_ALIGN(*size, sizeof(int));
    tpi[column].storeoffset = *size;
    (*size) += sizeof(int);
}

void int_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    TokenStoreSetInt(tpi, column, dest, index, TokenStoreGetInt(tpi, column, src, index));
}

void int_endianswap(ParseTable tpi[], int column, void* structptr, int index)
{
    TokenStoreSetInt(tpi, column, structptr, index, endianSwapU32(TokenStoreGetInt(tpi, column, structptr, index)));
}

void int_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam)
{
    int result = interpInt(interpParam, TokenStoreGetInt(tpi, column, structA, index), TokenStoreGetInt(tpi, column, structB, index));
    TokenStoreSetInt(tpi, column, destStruct, index, result);
}

void int_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime)
{
    F32 result = ((F32)TokenStoreGetInt(tpi, column, structB, index) - (F32)TokenStoreGetInt(tpi, column, structA, index)) / deltaTime;
    TokenStoreSetInt(tpi, column, destStruct, index, round32(result));
}

void int_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime)
{
    F32 result = (F32)TokenStoreGetInt(tpi, column, valueStruct, index) + deltaTime * (F32)TokenStoreGetInt(tpi, column, rateStruct, index);
    TokenStoreSetInt(tpi, column, destStruct, index, round32(result));
}

void int_calccyclic(ParseTable tpi[], int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, int index, F32 fStartTime, F32 deltaTime)
{
    F32 freq = TokenStoreGetInt(tpi, column, freqStruct, index);
    F32 cycle = TokenStoreGetInt(tpi, column, cycleStruct, index);
    F32 fSinDiff = sinf( (fStartTime + deltaTime) * freq + cycle * TWOPI) - sinf( ( fStartTime * freq + cycle ) * TWOPI );
    F32 result = TokenStoreGetInt(tpi, column, valueStruct, index) + TokenStoreGetInt(tpi, column, ampStruct, index) * fSinDiff;
    TokenStoreSetInt(tpi, column, destStruct, index, round32(result));
}

void int_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed)
{
    switch (optype)
    {
    case DynOpType_Jitter:
    {
        int iToAdd = round32(*values * randomF32Seeded(seed, RandType_BLORN));
        int result = TokenStoreGetInt(tpi, column, dstStruct, index) + iToAdd;
        TokenStoreSetInt(tpi, column, dstStruct, index, result);
    } break;
    case DynOpType_Inherit:
    {
        if (srcStruct) TokenStoreSetInt(tpi, column, dstStruct, index, TokenStoreGetInt(tpi, column, srcStruct, index));
    } break;
    case DynOpType_Add:
    {
        TokenStoreSetInt(tpi, column, dstStruct, index, TokenStoreGetInt(tpi, column, dstStruct, index) + round32(*values));
    } break;
    case DynOpType_Multiply:
    {
        TokenStoreSetInt(tpi, column, dstStruct, index, round32((F32)TokenStoreGetInt(tpi, column, dstStruct, index) * *values));
    } break;
    case DynOpType_Min:
    {
        TokenStoreSetInt(tpi, column, dstStruct, index, MIN(round32(*values), TokenStoreGetInt(tpi, column, dstStruct, index)));
    } break;
    case DynOpType_Max:
    {
        TokenStoreSetInt(tpi, column, dstStruct, index, MAX(round32(*values), TokenStoreGetInt(tpi, column, dstStruct, index)));
    } break;
    }
}

void int64_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    TokenStoreSetInt64(tpi, column, structptr, index, tpi[column].param);
}

int int64_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    const char* nexttoken = GetNextParameter(tok);
    if (!nexttoken) return 0;
    TokenStoreSetInt64(tpi, column, structptr, index, GetInteger64(tok, nexttoken));
    return 0;
}

void int64_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    S64 value = TokenStoreGetInt64(tpi, column, structptr, index);
    if (showname && !value && !tpi[column].param) return; // if defaulted to zero, not necessary to write field
    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);
    WriteInt64(out, value, 0, showname, tpi[column].subtable);
}

int int64_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int success = 1;
    S64 value = TokenStoreGetInt64(tpi, column, structptr, index);
    success = success && SimpleBufWriteU64(value, file);
    *datasum += sizeof(S64);
    return success;
}

int int64_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int ok = 1;
    S64 value = 0;
    ok = ok && SimpleBufReadU64((U64*)&value, file);
    *datasum += sizeof(S64);
    TokenStoreSetInt64(tpi, column, structptr, index, value);
    return ok;
}

void int64_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    if (sendAbsolute)
    {
        S64 val = TokenStoreGetInt64(tpi, column, newstruct, index);
        int low = val & 0xffffffff;
        int high = val >> 32;
        pktSendBitsPack(pak, TOK_GET_PRECISION_DEF(tpi[column].type, 1), low);
        pktSendBitsPack(pak, 1, high);
    }
    else
    {
        S64 oldval = TokenStoreGetInt64(tpi, column, oldstruct, index);
        S64 newval = TokenStoreGetInt64(tpi, column, newstruct, index);
        int low = (newval - oldval) & 0xffffffff;
        int high = (newval - oldval) >> 32;
        packDelta(pak, low);
        packDelta(pak, high);
    }
}

void int64_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    int out_of_order = 0;
    if (pktidptr) {
        out_of_order = PTR_TO_U32(*pktidptr) > pak->id;
        if (absValues && !out_of_order) // only store packet id's on full updates
            *pktidptr = U32_TO_PTR(pak->id);
    }
    if (absValues)
    {
        int low = pktGetBitsPack(pak, TOK_GET_PRECISION_DEF(tpi[column].type, 1));
        int high = pktGetBitsPack(pak, 1);
        S64 data = ((S64)high << 32) | ((S64)low & 0xffffffff);
        if (!out_of_order && structptr) TokenStoreSetInt64(tpi, column, structptr, index, data);
    }
    else
    {
        int low = unpackDelta(pak);
        int high = unpackDelta(pak);
        S64 data = (((S64)high << 32) | ((S64)low & 0xffffffff));
        if (!out_of_order && structptr)
        {
            data += TokenStoreGetInt64(tpi, column, structptr, index);
            TokenStoreSetInt64(tpi, column, structptr, index, data);
        }
    }
}

bool int64_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    S64 val = TokenStoreGetInt64(tpi, column, structptr, index);
    sprintf_s(str, str_size, "%I64d", val);
    return true;
}

bool int64_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    S64 val;
    bool ret = false;
    if (sscanf(str, "%I64d", &val) == 1) ret = true;
    if (ret) TokenStoreSetInt64(tpi, column, structptr, index, val);
    return ret;
}

void int64_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    S64 value = TokenStoreGetInt64(tpi, column, structptr, index);
    cryptAdler32Update((U8*)&value, sizeof(S64));
}

int int64_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    S64 left = TokenStoreGetInt64(tpi, column, lhs, index);
    S64 right = TokenStoreGetInt64(tpi, column, rhs, index);
    return (left < right)? -1: (left > right)? 1: 0;
}

void int64_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    MEM_ALIGN(*size, sizeof(S64));
    tpi[column].storeoffset = *size;
    (*size) += sizeof(S64);
}

void int64_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    TokenStoreSetInt64(tpi, column, dest, index, TokenStoreGetInt64(tpi, column, src, index));
}

void int64_endianswap(ParseTable tpi[], int column, void* structptr, int index)
{
    TokenStoreSetInt64(tpi, column, structptr, index, endianSwapU64(TokenStoreGetInt64(tpi, column, structptr, index)));
}

void int64_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam)
{
    S64 result = interpS64(interpParam, TokenStoreGetInt64(tpi, column, structA, index), TokenStoreGetInt64(tpi, column, structB, index));
    TokenStoreSetInt64(tpi, column, destStruct, index, result);
}

void int64_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime)
{
    F64 result = ((F64)TokenStoreGetInt64(tpi, column, structB, index) - (F64)TokenStoreGetInt64(tpi, column, structA, index)) / deltaTime;
    TokenStoreSetInt64(tpi, column, destStruct, index, round64(result));
}

void int64_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime)
{
    F64 result = (F64)TokenStoreGetInt64(tpi, column, valueStruct, index) + deltaTime * (F64)TokenStoreGetInt64(tpi, column, rateStruct, index);
    TokenStoreSetInt64(tpi, column, destStruct, index, round64(result));
}

void int64_calccyclic(ParseTable tpi[], int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, int index, F32 fStartTime, F32 deltaTime)
{
    F64 freq = TokenStoreGetInt64(tpi, column, freqStruct, index);
    F64 cycle = TokenStoreGetInt64(tpi, column, cycleStruct, index);
    F64 fSinDiff = sinf( (fStartTime + deltaTime) * freq + cycle * TWOPI) - sinf( ( fStartTime * freq + cycle ) * TWOPI );
    F64 result = TokenStoreGetInt64(tpi, column, valueStruct, index) + TokenStoreGetInt64(tpi, column, ampStruct, index) * fSinDiff;
    TokenStoreSetInt64(tpi, column, destStruct, index, round64(result));
}

void int64_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed)
{
    switch (optype)
    {
    case DynOpType_Jitter:
    {
        S64 iToAdd = round64((F64)*values * randomF32Seeded(seed, RandType_BLORN));
        S64 result = TokenStoreGetInt64(tpi, column, dstStruct, index) + iToAdd;
        TokenStoreSetInt64(tpi, column, dstStruct, index, result);
    } break;
    case DynOpType_Inherit:
    {
        if (srcStruct) TokenStoreSetInt64(tpi, column, dstStruct, index, TokenStoreGetInt64(tpi, column, srcStruct, index));
    } break;
    case DynOpType_Add:
    {
        TokenStoreSetInt64(tpi, column, dstStruct, index, TokenStoreGetInt64(tpi, column, dstStruct, index) + round32(*values));
    } break;
    case DynOpType_Multiply:
    {
        TokenStoreSetInt64(tpi, column, dstStruct, index, round64((F64)TokenStoreGetInt64(tpi, column, dstStruct, index) * *values));
    } break;
    case DynOpType_Min:
    {
        TokenStoreSetInt64(tpi, column, dstStruct, index, MIN(round64(*values), TokenStoreGetInt64(tpi, column, dstStruct, index)));
    } break;
    case DynOpType_Max:
    {
        TokenStoreSetInt64(tpi, column, dstStruct, index, MAX(round64(*values), TokenStoreGetInt64(tpi, column, dstStruct, index)));
    } break;
    }
}

void float_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    TokenStoreSetF32(tpi, column, structptr, index, tpi[column].param);
}

int float_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    const char* nexttoken = GetNextParameter(tok);
    if (!nexttoken) return 0;
    TokenStoreSetF32(tpi, column, structptr, index, GetFloat(tok, nexttoken));
    return 0;
}

void float_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    F32 value = TokenStoreGetF32(tpi, column, structptr, index);
    if (showname && (value == 0.0) && !tpi[column].param) return; // if defaulted to zero, not necessary to write field
    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);
    if (TOK_GET_FORMAT_OPTIONS(tpi[column].format) == TOK_FORMAT_PERCENT)
    {
        char str[20];
        sprintf_s(SAFESTR(str), "%02.0f%%", value*100);
        WriteString(out, str, 0, showname);
    }
    else
        WriteFloat(out, value, 0, showname, tpi[column].subtable);
}

int float_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int success = 1;
    F32 value = TokenStoreGetF32(tpi, column, structptr, index);
    success = success && SimpleBufWriteF32(value, file);
    *datasum += sizeof(F32);
    return success;
}

int float_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int ok = 1;
    F32 value = 0.0f;
    ok = ok && SimpleBufReadF32(&value, file);
    *datasum += sizeof(F32);
    TokenStoreSetF32(tpi, column, structptr, index, value);
    return ok;
}

void float_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int acc = TOK_GET_FLOAT_ROUNDING(tpi[column].type);
    if (!acc) // not using low accuracy packing
    {
        pktSendF32(pak, TokenStoreGetF32(tpi, column, newstruct, index));
    }
    else if (sendAbsolute)
    {
        pktSendBitsPack(pak, ParserFloatMinBits(acc), ParserPackFloat(TokenStoreGetF32(tpi, column, newstruct, index), acc));
    }
    else
    {
        int df = ParserPackFloat(TokenStoreGetF32(tpi, column, newstruct, index), acc) - 
            ParserPackFloat(TokenStoreGetF32(tpi, column, oldstruct, index), acc);
        packDelta(pak, df);
        if (pak->hasDebugInfo)
            pktSendBitsPack(pak, 1, ParserPackFloat(TokenStoreGetF32(tpi, column, newstruct, index), acc));
    }
}

void float_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    int acc = TOK_GET_FLOAT_ROUNDING(tpi[column].type);
    int out_of_order = 0;
    if (pktidptr) {
        out_of_order = PTR_TO_U32(*pktidptr) > pak->id;
        if (absValues && !out_of_order) // only store packet id's on full updates
            *pktidptr = U32_TO_PTR(pak->id);
    }
    if (!acc) // not using low accuracy packing
    {
        F32 data = pktGetF32(pak);
        if (!out_of_order && structptr) TokenStoreSetF32(tpi, column, structptr, index, data);
    }
    else if (absValues)
    {
        F32 data = ParserUnpackFloat(pktGetBitsPack(pak, ParserFloatMinBits(acc)), acc);
        if (!out_of_order && structptr) TokenStoreSetF32(tpi, column, structptr, index, data);
    }
    else
    {
        F32 data;
        int df = unpackDelta(pak);
        if (!out_of_order && structptr)
        {
            df += ParserPackFloat(TokenStoreGetF32(tpi, column, structptr, index), acc);
            data = ParserUnpackFloat(df, acc);
            TokenStoreSetF32(tpi, column, structptr, index, data);
        }
        if (pak->hasDebugInfo)
        {
            extern int g_bAssertOnBitStreamError;
            int debugpacked = pktGetBitsPack(pak, 1);
            if (g_bAssertOnBitStreamError && !out_of_order && structptr)
                assert(debugpacked == df);
        }
    }
}

bool float_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    F32 val = TokenStoreGetF32(tpi, column, structptr, index);
    sprintf_s(str, str_size, "%f", val);
    return true;
}

bool float_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    F32 val;
    bool ret = false;
    if (sscanf(str, "%f", &val) == 1) ret = true;
    if (ret) TokenStoreSetF32(tpi, column, structptr, index, val);
    return ret;
}

void float_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    F32 value = TokenStoreGetF32(tpi, column, structptr, index);
    cryptAdler32Update((U8*)&value, sizeof(F32));
}

int float_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    int fa = TOK_GET_FLOAT_ROUNDING(tpi[column].type);
    F32 left = TokenStoreGetF32(tpi, column, lhs, index);
    F32 right = TokenStoreGetF32(tpi, column, rhs, index);
    if (fa)
    {
        int l = ParserPackFloat(left, fa);
        int r = ParserPackFloat(right, fa);
        return l-r;
    }
    return (left < right)? -1: (left > right)? 1: 0;
}

void float_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    MEM_ALIGN(*size, sizeof(F32));
    tpi[column].storeoffset = *size;
    (*size) += sizeof(F32);
}

void float_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch)
{
    TokenStoreSetF32(tpi, column, dest, index, TokenStoreGetF32(tpi, column, src, index));
}

void float_endianswap(ParseTable tpi[], int column, void* structptr, int index)
{
    // hack - asking to get and set U32 value so I don't have to do conversion
    TokenStoreSetInt(tpi, column, structptr, index, endianSwapU32(TokenStoreGetInt(tpi, column, structptr, index)));
}

void float_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam)
{
    F32 result = interpF32(interpParam, TokenStoreGetF32(tpi, column, structA, index), TokenStoreGetF32(tpi, column, structB, index));
    TokenStoreSetF32(tpi, column, destStruct, index, result);
}

void float_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime)
{
    F32 result = (TokenStoreGetF32(tpi, column, structB, index) - TokenStoreGetF32(tpi, column, structA, index)) / deltaTime;
    TokenStoreSetF32(tpi, column, destStruct, index, result);
}

void float_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime)
{
    F32 result = TokenStoreGetF32(tpi, column, valueStruct, index) + deltaTime * TokenStoreGetF32(tpi, column, rateStruct, index);
    TokenStoreSetF32(tpi, column, destStruct, index, result);
}

void float_calccyclic(ParseTable tpi[], int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, int index, F32 fStartTime, F32 deltaTime)
{
    F32 freq = TokenStoreGetF32(tpi, column, freqStruct, index);
    F32 cycle = TokenStoreGetF32(tpi, column, cycleStruct, index);
    F32 fSinDiff = sinf( ( (fStartTime + deltaTime) * freq + cycle ) * TWOPI) - sinf( ( fStartTime * freq + cycle ) * TWOPI );
    F32 result = TokenStoreGetF32(tpi, column, valueStruct, index) + TokenStoreGetF32(tpi, column, ampStruct, index) * fSinDiff;
    TokenStoreSetF32(tpi, column, destStruct, index, result);
}

void float_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed)
{
    switch (optype)
    {
    case DynOpType_Jitter:
    {
        F32 iToAdd = *values * randomF32Seeded(seed, RandType_BLORN);
        F32 result = TokenStoreGetF32(tpi, column, dstStruct, index) + iToAdd;
        TokenStoreSetF32(tpi, column, dstStruct, index, result);
    } break;
    case DynOpType_Inherit:
    {
        if (srcStruct) TokenStoreSetF32(tpi, column, dstStruct, index, TokenStoreGetF32(tpi, column, srcStruct, index));
    } break;
    case DynOpType_Add:
    {
        TokenStoreSetF32(tpi, column, dstStruct, index, TokenStoreGetF32(tpi, column, dstStruct, index) + *values);
    } break;
    case DynOpType_Multiply:
    {
        TokenStoreSetF32(tpi, column, dstStruct, index, TokenStoreGetF32(tpi, column, dstStruct, index) * *values);
    } break;
    case DynOpType_Min:
    {
        TokenStoreSetF32(tpi, column, dstStruct, index, MIN(*values, TokenStoreGetF32(tpi, column, dstStruct, index)));
    } break;
    case DynOpType_Max:
    {
        TokenStoreSetF32(tpi, column, dstStruct, index, MAX(*values, TokenStoreGetF32(tpi, column, dstStruct, index)));
    } break;
    }
}

int degrees_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    const char* nexttoken = GetNextParameter(tok);
    if (!nexttoken) return 0;
    TokenStoreSetF32(tpi, column, structptr, index, RAD(GetFloat(tok, nexttoken)));
    return 0;
}

void degrees_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    F32 val = DEG(TokenStoreGetF32(tpi, column, structptr, index));
    if (showname && (val == 0.0) && !tpi[column].param) return; // if defaulted to zero, not necessary to write field
    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);
    WriteFloat(out, val, 0, showname, tpi[column].subtable);
}

bool degrees_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    F32 val = DEG(TokenStoreGetF32(tpi, column, structptr, index));
    sprintf_s(str, str_size, "%f", val);
    return true;
}

bool degrees_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    F32 val;
    bool ret = false;
    if (sscanf(str, "%f", &val) == 1) ret = true;
    if (ret) TokenStoreSetF32(tpi, column, structptr, index, RAD(val));
    return ret;
}

ParseInfoFieldUsage string_interpretfield(ParseTable tpi[], int column, ParseInfoField field)
{
    switch (field)
    {
    case ParamField:
        if (tpi[column].type & TOK_FIXED_ARRAY)
            return NumberOfElements;
        if (tpi[column].type & TOK_EARRAY)
            return NoFieldUsage;
        if (tpi[column].type & TOK_INDIRECT)
            return PointerToDefaultString;
        return EmbeddedStringLength; // otherwise direct, single
    case SubtableField:
        return StaticDefineList;
    }
    return NoFieldUsage;
}

void string_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    // a direct, single string holds its size in the param field, so can't be init'ed
    if (TokenStoreGetStorageType(tpi[column].type) == TOK_STORAGE_INDIRECT_SINGLE)
    {
        char* str = (char*)tpi[column].param;
        if (!str) return;
        assertmsg(str[0], "Parser does not support initing to an empty string."); // Will be inconsistent between bins and from text
        TokenStoreSetString(tpi, column, structptr, 0, str, NULL, NULL);
    }
}

void string_destroystruct(ParseTable tpi[], int column, void* structptr, int index)
{
    TokenStoreFreeString(tpi, column, structptr, index);
}

int string_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    const char* nexttoken = GetNextParameter(tok);
    if (!nexttoken) 
        return 0;
    TokenStoreSetString(tpi, column, structptr, index, nexttoken, NULL, NULL);
    return 0;
}

void string_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level)
{
    const char* str = TokenStoreGetString(tpi, column, structptr, index);

    if (showname && !tpi[column].param && (!str || !str[0])) 
        return; // if defaulted to zero, not necessary to write field
    if (showname) 
        WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);

    if (TOK_GET_FORMAT_OPTIONS(tpi[column].format) == TOK_FORMAT_NOPATH)
        str = getFileName(str);
    WriteQuotedString(out, str, 0, showname);
}

int string_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int wr = 0;
    int success = 1;
    char* str = TokenStoreGetString(tpi, column, structptr, index);
    wr = WritePascalString(file, str);
    success = success && wr;
    *datasum += wr;
    return success;
}

int string_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    char tempstr[MAX_STRING_LENGTH];
    int re, ok = 1;
    re = ReadPascalString(file, SAFESTR(tempstr));
    TokenStoreSetString(tpi, column, structptr, index, tempstr, NULL, NULL);
    ok = ok && re;
    *datasum += re;
    return ok;
}

void string_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    pktSendString(pak, TokenStoreGetString(tpi, column, newstruct, index));
}

void string_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    char* str;
    int out_of_order = 0;
    if (pktidptr) {
        out_of_order = PTR_TO_U32(*pktidptr) > pak->id;
        if (!out_of_order) // strings store packet id's on _every_ update (don't do diffs)
            *pktidptr = U32_TO_PTR(pak->id);
    }
    str = pktGetString(pak);
    if (!out_of_order && structptr) TokenStoreSetString(tpi, column, structptr, index, str, NULL, NULL);
}

bool string_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    char* val = TokenStoreGetString(tpi, column, structptr, index);
    if (val) strcpy_s(str, str_size, val);
    else str[0] = 0;
    return true;
}

bool string_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    if (str && str[0])
        TokenStoreSetString(tpi, column, structptr, index, str, NULL, NULL);
    else
        TokenStoreFreeString(tpi, column, structptr, index);
    return true;
}

void string_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    int zero = 0;
    char* str = TokenStoreGetString(tpi, column, structptr, index);
    if (str && str[0])
        cryptAdler32Update(str, (int)strlen(str));
    else
        cryptAdler32Update((U8*)&zero, sizeof(int));
}

int string_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    char* left = TokenStoreGetString(tpi, column, lhs, index);
    char* right = TokenStoreGetString(tpi, column, rhs, index);
    if (!left && !right) return 0;
    if (!left) return -1;
    if (!right) return 1;
    return stricmp(left, right);
}

size_t string_memusage(ParseTable tpi[], int column, void* structptr, int index)
{
    return TokenStoreGetStringMemUsage(tpi, column, structptr, index);
}

void string_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    int storage = TokenStoreGetStorageType(tpi[column].type);

    if (storage == TOK_STORAGE_DIRECT_SINGLE)
    {
        tpi[column].storeoffset = *size;
        (*size) += tpi[column].param; // length of buffer
    }
    else
    {
        MEM_ALIGN(*size, sizeof(char*));
        tpi[column].storeoffset = *size;
        (*size) += sizeof(char*);
    }
}

void string_copystruct(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData)
{
    char* srcstr = TokenStoreGetString(tpi, column, src, index);
    TokenStoreClearString(tpi, column, dest, index); // need to clear so we don't free on the next call
    TokenStoreSetString(tpi, column, dest, index, srcstr, memAllocator, customData);
}

void string_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    TokenStoreSetString(tpi, column, dest, index, TokenStoreGetString(tpi, column, src, index), memAllocator, customData);
}

void string_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed)
{
    switch (optype)
    {
    case DynOpType_Inherit:
    {
        if (srcStruct)
        {
            TokenStoreFreeString(tpi, column, dstStruct, index);
            TokenStoreSetString(tpi, column, dstStruct, index, TokenStoreGetString(tpi, column, srcStruct, index), NULL, NULL);
        }
    } break;
    };
}

void char_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    memset(TokenStoreGetPointer(tpi, column, structptr, index), 0, tpi[column].param);
}

int char_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    int i;
    const char* nexttoken = GetNextParameter(tok);
    char* data = TokenStoreGetPointer(tpi, column, structptr, index);
    if (!nexttoken) return 0;
    for (i=0; i<tpi[column].param; i++)
    {
        data[i] = *nexttoken;
        if (nexttoken)
            nexttoken++;
    }
    return 0;
}

void char_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    char buf[TOKEN_BUFFER_LENGTH]; 
    char *data = TokenStoreGetPointer(tpi, column, structptr, index);
    if (showname)
    {
        // if empty, not necessary to write field
        int i;
        for(i = 0; i < tpi[column].param; i++)
            if(data[i])
                break;
        if(i >= tpi[column].param)
            return;
    }

    memcpy(buf, data, tpi[column].param);
    buf[tpi[column].param] = 0;

    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);
    WriteQuotedString(out, buf, 0, 0);
    WriteString(out, "", 0, showname);
}

ParseInfoFieldUsage pointer_interpretfield(ParseTable tpi[], int column, ParseInfoField field)
{
    if (field == ParamField) 
        return OffsetOfSizeField;
    return NoFieldUsage;
}

void pointer_destroystruct(ParseTable tpi[], int column, void* structptr, int index)
{
    int* count = TokenStoreGetCountField(tpi, column, structptr);
    TokenStoreFree(tpi, column, structptr, index);
    *count = 0;
}

int pointer_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    int size = 0;
    int* count = TokenStoreGetCountField(tpi, column, structptr);

    const char* nexttoken = GetNextParameter(tok);
    if (!nexttoken)
    {
        TokenStoreFree(tpi, column, structptr, index);
        *count = 0;
        return 0;
    }
    size = GetInteger(tok, nexttoken);

    // realloc as necessary
    if (*count != size)
    {
        TokenStoreFree(tpi, column, structptr, index);
        if (size)
            TokenStoreAlloc(tpi, column, structptr, index, size, NULL, NULL);
        *count = size;
    }

    if (size)
    {
        nexttoken = GetNextParameter(tok);
        if (!nexttoken)
        {
            ErrorFilenamef(TokenizerGetFileName(tok), "Parser: pointer parameter has size but no data %s\n", TokenizerGetFileAndLine(tok));
            g_parseerr = 1;
            TokenStoreFree(tpi, column, structptr, index);
            *count = 0;
        }
        else if (!ParseBase85(tok, nexttoken, TokenStoreGetPointer(tpi, column, structptr, index), size, 0))
        {
            TokenStoreFree(tpi, column, structptr, index);
            *count = 0;
        }
    }
    return 0;
}

void pointer_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int* count = TokenStoreGetCountField(tpi, column, structptr);
    if (showname && !*count) return; // if empty, not necessary to write field

    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);
    WriteInt(out, *count, 0, 0, NULL);

    if (*count)
    {
        WriteString(out, " ", 0, 0);
        WriteBase85(out, TokenStoreGetPointer(tpi, column, structptr, index), *count);
    }
    WriteString(out, "", 0, showname);
}

int pointer_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int success = 1;
    int* count = TokenStoreGetCountField(tpi, column, structptr);
    void* data = TokenStoreGetPointer(tpi, column, structptr, index);
    success = success && SimpleBufWriteU32(*count, file);
    *datasum += sizeof(int);
    if (*count)
    {
        success = success && SimpleBufWrite(data, *count, file);
        *datasum += *count;
    }
    return success;
}

int pointer_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int ok = 1;
    int size = 0;
    int* count = TokenStoreGetCountField(tpi, column, structptr);
    void* data;

    // read size of data
    ok = ok && SimpleBufReadU32((U32*)&size, file);
    *datasum += sizeof(int);

    // realloc as necessary
    if (*count != size)
    {
        TokenStoreFree(tpi, column, structptr, index);
        if (size)
            TokenStoreAlloc(tpi, column, structptr, index, size, NULL, NULL);
        *count = size;
    }
    if (size)
    {
        data = TokenStoreGetPointer(tpi, column, structptr, index);
        ok = ok && SimpleBufRead(data, size, file);
        *datasum += size;
    }
    return ok;
}

void pointer_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int* count = TokenStoreGetCountField(tpi, column, newstruct);
    pktSendBitsAuto(pak, *count);
    if (*count)
        pktSendBitsArray(pak, *count * 8, TokenStoreGetPointer(tpi, column, newstruct, index));
}

void pointer_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    int* count = TokenStoreGetCountField(tpi, column, structptr);
    int out_of_order = 0;
    int size;

    if (pktidptr) {
        out_of_order = PTR_TO_U32(*pktidptr) > pak->id;
        if (!out_of_order) // pointers store packet id's on _every_ update (don't do diffs)
            *pktidptr = U32_TO_PTR(pak->id);
    }
    size = pktGetBitsAuto(pak);

    // realloc as necessary
    if (!out_of_order && *count != size && structptr)
    {
        TokenStoreFree(tpi, column, structptr, index);
        if (size)
            TokenStoreAlloc(tpi, column, structptr, index, size, NULL, NULL);
        *count = size;
    }

    if (size) // being sent data
    {
        if (!out_of_order && structptr)
            pktGetBitsArray(pak, size * 8, TokenStoreGetPointer(tpi, column, structptr, index));
        else
        {
            char* temp = malloc(size);
            pktGetBitsArray(pak, size*8, temp);
            free(temp);
        }
    }
    // otherwise, I already free'd the data if necessary above
}

void pointer_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    int* count = TokenStoreGetCountField(tpi, column, structptr);
    void* data = TokenStoreGetPointer(tpi, column, structptr, index);
    if (*count) cryptAdler32Update((U8*)data, *count);
}

int pointer_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    int* lcount = TokenStoreGetCountField(tpi, column, lhs);
    int* rcount = TokenStoreGetCountField(tpi, column, rhs);
    void* left = TokenStoreGetPointer(tpi, column, lhs, index);
    void* right = TokenStoreGetPointer(tpi, column, rhs, index);
    if (!left && !right) return 0;
    if (!left) return -1;
    if (!right) return 1;
    if (*lcount != *rcount)
        return *lcount - *rcount;
    return cmp8(left, right, *lcount);
}

size_t pointer_memusage(ParseTable tpi[], int column, void* structptr, int index)
{
    int* count = TokenStoreGetCountField(tpi, column, structptr);
    void* data = TokenStoreGetPointer(tpi, column, structptr, index);
    if (data) return *count;
    return 0;
}

void pointer_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    // put the count field in first
    MEM_ALIGN(*size, sizeof(int));
    tpi[column].param = *size;
    (*size) += sizeof(int);

    // then the pointer
    MEM_ALIGN(*size, sizeof(void*));
    tpi[column].storeoffset = *size;
    (*size) += sizeof(void*);
}

void pointer_copystruct(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData)
{
    int* srccount = TokenStoreGetCountField(tpi, column, src);
    void* destpointer;
    void* srcpointer = TokenStoreGetPointer(tpi, column, src, index);

    TokenStoreSetPointer(tpi, column, dest, index, NULL); // clear so alloc won't free
    if (*srccount)
    {
        destpointer = TokenStoreAlloc(tpi, column, dest, index, *srccount, memAllocator, customData);
        memcpy(destpointer, srcpointer, *srccount);
    }
}

void pointer_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int* srccount = TokenStoreGetCountField(tpi, column, src);
    int* destcount = TokenStoreGetCountField(tpi, column, dest);

    // some optimization if source and target are already same size
    if (*srccount == *destcount)
    {
        void* srcpointer = TokenStoreGetPointer(tpi, column, src, index);
        void* destpointer = TokenStoreGetPointer(tpi, column, dest, index);
        if (!*srccount) return; // both empty
        memcpy(destpointer, srcpointer, *srccount);
    }
    else
    {
        // otherwise, basically the same as copystruct, but we need to free first
        TokenStoreFree(tpi, column, dest, index);
        pointer_copystruct(tpi, column, dest, src, index, memAllocator, customData);
        *destcount = *srccount;
    }
}

ParseInfoFieldUsage raw_interpretfield(ParseTable tpi[], int column, ParseInfoField field)
{
    if (field == ParamField)
        return SizeOfRawField;
    return NoFieldUsage;
}

int raw_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    const char* nexttoken = GetNextParameter(tok);
    if (nexttoken)
        ParseBase85(tok, nexttoken, TokenStoreGetPointer(tpi, column, structptr, index), tpi[column].param, 1);
    else
        memset(TokenStoreGetPointer(tpi, column, structptr, index), 0, tpi[column].param);
    return 0;
}

void raw_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    U8 *data = TokenStoreGetPointer(tpi, column, structptr, index);
    if (showname)
    {
        // if empty, not necessary to write field
        int i;
        for(i = 0; i < tpi[column].param; i++)
            if(data[i])
                break;
        if(i >= tpi[column].param)
            return;
    }

    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);
    WriteBase85(out, data, tpi[column].param);
    WriteString(out, "", 0, showname);
}

int raw_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    U32 zero = 0;
    int ok = 1;
    int padding = ((tpi[column].param + 3) & ~3) - tpi[column].param; // keep 4-byte alignment
    void* data = TokenStoreGetPointer(tpi, column, structptr, index);
    ok = ok && SimpleBufWrite(data, tpi[column].param, file);
    if (padding)
        ok = ok && SimpleBufWrite(&zero, padding, file);
    *datasum += tpi[column].param + padding;
    return ok;
}

int raw_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    U32 scratch = 0;
    int ok = 1;
    int padding = ((tpi[column].param + 3) & ~3) - tpi[column].param; // keep 4-byte alignment
    void* data = TokenStoreGetPointer(tpi, column, structptr, index);
    ok = ok && SimpleBufRead(data, tpi[column].param, file);
    if (padding)
        ok = ok && SimpleBufRead(&scratch, padding, file);
    *datasum += tpi[column].param + padding;
    return ok;
}

void raw_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    pktSendBitsArray(pak, tpi[column].param*8, TokenStoreGetPointer(tpi, column, newstruct, index));
}

void raw_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    char buf[1024]; // hopefully fits most raw fields
    char* str;
    int out_of_order = 0;
    if (pktidptr) {
        out_of_order = PTR_TO_U32(*pktidptr) > pak->id;
        if (!out_of_order) // raw store packet id's on _every_ update (don't do diffs)
            *pktidptr = U32_TO_PTR(pak->id);
    }
    if (!out_of_order && structptr) 
    {
        pktGetBitsArray(pak, tpi[column].param*8, TokenStoreGetPointer(tpi, column, structptr, index));
    }
    else if (tpi[column].param <= 1024)
    {
        pktGetBitsArray(pak, tpi[column].param*8, buf);
    }
    else // have to put bits someplace
    {
        str = malloc(tpi[column].param);
        pktGetBitsArray(pak, tpi[column].param*8, str);
        free(str);
    }
}

void raw_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    void* data = TokenStoreGetPointer(tpi, column, structptr, index);
    cryptAdler32Update(data, tpi[column].param);
}

int raw_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    void* left = TokenStoreGetPointer(tpi, column, lhs, index);
    void* right = TokenStoreGetPointer(tpi, column, rhs, index);
    return cmp8(left, right, tpi[column].param);
}

void raw_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    tpi[column].storeoffset = *size;
    (*size) += tpi[column].param;
}

void raw_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    void* srcdata = TokenStoreGetPointer(tpi, column, src, index);
    void* destdata = TokenStoreGetPointer(tpi, column, dest, index);
    memcpy(destdata, srcdata, tpi[column].param);
}

//////////////////////////////////////////////// built-ins

void currentfile_preparse(ParseTable* tpi, int column, void* structptr, TokenizerHandle tok)
{
    char* str;
    str = TokenStoreSetString(tpi, column, structptr, 0, TokenizerGetFileName(tok), NULL, NULL);
    if( str )
    {
        _strupr_s(str, strlen(str)+1);
        forwardSlashes(str);
    }
}

void timestamp_preparse(ParseTable* tpi, int column, void* structptr, TokenizerHandle tok)
{
    TokenStoreSetInt(tpi, column, structptr, 0, fileLastChanged(TokenizerGetFileName(tok)));
}

void linenum_preparse(ParseTable* tpi, int column, void* structptr, TokenizerHandle tok)
{
    TokenStoreSetInt(tpi, column, structptr, 0, TokenizerGetCurLine(tok));
}

int usedfield_GetAllocSize(ParseTable* tpi)
{
    return 4*((ParseTableCountFields(tpi) >> 5) + 1); // rounded to U32's
}

void usedfield_preparse(ParseTable* tpi, int column, void* structptr, TokenizerHandle tok)
{
    int* count = TokenStoreGetCountField(tpi, column, structptr);
    U32 size = usedfield_GetAllocSize(tpi);
    if (!count)
    {
        Errorf("Invalid parameter in TOK_USEDFIELD column");
        return;
    }
    TokenStoreAlloc(tpi, column, structptr, 0, size, NULL, NULL);
    *count = size;
}

int bool_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    int i;
    const char* nexttoken = GetNextParameter(tok);
    if (!nexttoken) return 0;
    i = GetInteger(tok, nexttoken);
    if (i < 0 || i > 1)
    {
        ErrorFilenamef(TokenizerGetFileName(tok),
            "Parser: bool parameter should be 0 or 1 in %s\n", TokenizerGetFileAndLine(tok));
        i = 0;
        g_parseerr = 1;
    }
    assert(sizeof(bool) == sizeof(U8));
    TokenStoreSetU8(tpi, column, structptr, index, i);
    return 0;
}

int flags_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    U32 value = 0;

    while (1)
    {
        const char* nexttoken = TokenizerPeek(tok, 0, 1);
        if (!nexttoken || IsEol(nexttoken)) break;
        nexttoken = TokenizerGet(tok, 0, 1);
        value |= GetUInteger(tok, nexttoken);
    }
    TokenStoreSetInt(tpi, column, structptr, 0, value);
    return 0;
}

void flags_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    U32 val = TokenStoreGetInt(tpi, column, structptr, 0);
    U32 mask = 1;
    int i;
    int first = 1;

    if (!val)
        return;

    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    for (i = 0; i < 32; i++)
    {
        if (val & 1)
        {
            if (first) { WriteString(out, " ", 0, 0); first = 0; }
            else { WriteString(out, ", ", 0, 0); }
            WriteUInt(out, mask, 0, 0, tpi[column].subtable);
        }
        mask <<= 1;
        val >>= 1;
    }
    if (showname) WriteString(out, "", 0, 1);
}

int flagarray_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    U32 size = tpi[column].param;
    U32 *tempArray = 0;
    U32 value = 0, intIndex = 0;

//    while(1);

    for (intIndex = 0; intIndex < size; intIndex++)
    {
        eaiPush(&tempArray, 0);
    }

    while (1)
    {
        const char* nexttoken = TokenizerPeek(tok, 0, 1);
        if (!nexttoken || IsEol(nexttoken)) break;
        nexttoken = TokenizerGet(tok, 0, 1);
        value = GetUInteger(tok, nexttoken);
        if (value > (size * 32))
        {
            devassert(0 && "integer value too big for flag array to parse");
            return 1;
        }
        tempArray[value / 32] |= (1 << (value % 32));
    }

    for (intIndex = 0; intIndex < size; intIndex++)
    {
        TokenStoreSetInt(tpi, column, structptr, intIndex, tempArray[intIndex]);
    }
    return 0;
}

void flagarray_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    U32 size = tpi[column].param;
    U32 intIndex;
    U32 mask = 0; // naturally overflows 32 for every iteration of intIndex
    int first = 1;

    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    for (intIndex = 0; intIndex < size; intIndex++)
    {
        U32 val = TokenStoreGetInt(tpi, column, structptr, intIndex);
        int i;

        if (!val)
            return;

        for (i = 0; i < 32; i++)
        {
            if (val & 1)
            {
                if (first) { WriteString(out, " ", 0, 0); first = 0; }
                else { WriteString(out, ", ", 0, 0); }
                WriteUInt(out, mask, 0, 0, tpi[column].subtable);
            }
            mask++;
            val >>= 1;
        }
    }
    if (showname) WriteString(out, "", 0, 1);
}

int boolflag_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    // if we hit the field at all, it gets set to 1
    TokenStoreSetU8(tpi, column, structptr, index, 1);
    return 0;
}

void boolflag_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    if (showname && TokenStoreGetU8(tpi, column, structptr, index))
    {
        WriteString(out, tpi[column].name, level+1, 1);
    }
}

int quatpyr_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    int v;
    F32 pyr[3];
    F32* quat;

    for (v = 0; v < 3; v++)
    {
        const char* nexttoken = GetNextParameter(tok);
        if (!nexttoken) return 0;
        pyr[v] = GetFloat(tok, nexttoken);
    }
    quat = (F32*)TokenStoreGetPointer(tpi, column, structptr, 0);
    RADVEC3(pyr);
    PYRToQuat(pyr, quat);
    return 0;
}

void quatpyr_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    F32 pyr[3];
    F32* quat = (F32*)TokenStoreGetPointer(tpi, column, structptr, 0);

    quatToPYR(quat, pyr);
    DEGVEC3(pyr);
    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);
    WriteFloat(out, pyr[0], 0, 0, tpi[column].subtable);
    WriteString(out, ", ", 0, 0);
    WriteFloat(out, pyr[1], 0, 0, tpi[column].subtable);
    WriteString(out, ", ", 0, 0);
    WriteFloat(out, pyr[2], 0, showname, tpi[column].subtable);
}

void quatpyr_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam)
{
    F32* quatA = (F32*)TokenStoreGetPointer(tpi, column, structA, 0);
    F32* quatB = (F32*)TokenStoreGetPointer(tpi, column, structB, 0);
    F32* quatDest = (F32*)TokenStoreGetPointer(tpi, column, destStruct, 0);
    quatInterp(interpParam, quatA, quatB, quatDest);
}

void quatpyr_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime)
{
    Quat qInterpQuat;
    Quat qInverseA;
    F32* quatA = (F32*)TokenStoreGetPointer(tpi, column, structA, 0);
    F32* quatB = (F32*)TokenStoreGetPointer(tpi, column, structB, 0);
    F32* quatDest = (F32*)TokenStoreGetPointer(tpi, column, destStruct, 0);

    // Convert it to angular velocity, and scale
    // Calculate interp quat by multiplying the inverse of a by b
    quatInverse(quatA, qInverseA);
    quatMultiply(quatB, qInverseA, qInterpQuat);
    quatToAxisAngle(qInterpQuat, quatDest, &quatDest[3]);
    quatDest[3] /= deltaTime;
}

void quatpyr_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime)
{
    F32* quatV = (F32*)TokenStoreGetPointer(tpi, column, valueStruct, 0);
    F32* quatR = (F32*)TokenStoreGetPointer(tpi, column, rateStruct, 0);
    F32* quatDest = (F32*)TokenStoreGetPointer(tpi, column, destStruct, 0);

    // Convert it from angular velocity, and scale
    Quat qIntegral;
    if (quatR[3] > 0.0 && axisAngleToQuat(quatR, quatR[3] * deltaTime, qIntegral) )
    {
        // axisAngleToQuat returns true only if it's a valid axis/angle, otherwise just copy
        quatMultiply(quatV, qIntegral, quatDest);
    }
    else
    {
        copyQuat(quatV, quatDest);
    }
}

void quatpyr_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed)
{
    switch (optype)
    {
    case DynOpType_Jitter:
    {
        F32* qTarget = (F32*)TokenStoreGetPointer(tpi, column, dstStruct, 0);
        Vec3 vRandom;
        Quat qTemp, qTemp2;
        randomVec3Seeded(seed, RandType_BLORN, vRandom);
        mulVecVec3(vRandom, values, vRandom);
        RADVEC3(vRandom);
        PYRToQuat(vRandom, qTemp);
        quatMultiply(qTarget, qTemp, qTemp2);
        copyQuat(qTemp2, qTarget);
    } break;
    case DynOpType_Inherit:
    {
        if (srcStruct)
        {
            F32* srcQuat = (F32*)TokenStoreGetPointer(tpi, column, srcStruct, 0);
            F32* dstQuat = (F32*)TokenStoreGetPointer(tpi, column, dstStruct, 0);
            Quat qTemp;
            quatMultiply((F32*)dstQuat, (F32*)srcQuat, qTemp);
            copyQuat(qTemp, (F32*)dstQuat);
        }
    } break;
    case DynOpType_Add:
    {
        Errorf("You can't add rotations, maybe you mean multiply?");
    } break;
    case DynOpType_Multiply:
    {
        F32* qTarget = (F32*)TokenStoreGetPointer(tpi, column, dstStruct, 0);
        Vec3 vTemp;
        Quat qTemp, qTemp2;
        copyVec3(values, vTemp);
        RADVEC3(vTemp);
        PYRToQuat(vTemp, qTemp);
        quatMultiply(qTarget, qTemp, qTemp2);
        copyQuat(qTemp2, qTarget);
    } break;
    case DynOpType_Min:
    {
        Errorf("You can't get the min of a rotation, maybe you mean multiply?");
    } break;
    case DynOpType_Max:
    {
        Errorf("You can't get the max of a rotation, maybe you mean multiply?");
    } break;
    }
}

int condrgb_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    Color c;
    int i;

    c.rgba[3] = 255; // If the color is specified at all its alpha is fully opaque.

    for (i = 0; i < 3; i++)
    {
        const char* nexttoken = GetNextParameter(tok);

        if (!nexttoken) 
            return 0;

        c.rgba[i] = GetUInteger(tok, nexttoken);
    }
    
    *(Color*)TokenStoreGetPointer(tpi, column, structptr, 0) = c;
    return 0;
}

void condrgb_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    Color* c = (Color*)TokenStoreGetPointer(tpi, column, structptr, 0);

    if (c->a == 0)
        return;

    if (showname)
        WriteString(out, tpi[column].name, level+1, 0);

    WriteString(out, " ", 0, 0);
    WriteUInt(out, c->r, 0, 0, tpi[column].subtable);
    WriteString(out, ", ", 0, 0);
    WriteUInt(out, c->g, 0, 0, tpi[column].subtable);
    WriteString(out, ", ", 0, 0);
    WriteUInt(out, c->b, 0, 0, tpi[column].subtable);
}

void vec3_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed)
{
    switch (optype)
    {
    case DynOpType_SphereJitter:
    {
        F32* vTarget = (F32*)TokenStoreGetPointer(tpi, column, dstStruct, 0);
        Vec3 vAdd;
        randomSphereSliceSeeded(seed, RandType_BLORN, RAD(values[0]), RAD(values[1]), values[2], vAdd);
        addVec3(vTarget, vAdd, vTarget);
    } break;
    case DynOpType_SphereShellJitter:
    {
        Vec3 vAdd;
        F32* vTarget = (F32*)TokenStoreGetPointer(tpi, column, dstStruct, 0);
        randomSphereShellSliceSeeded(seed, RandType_BLORN, RAD(values[0]), RAD(values[1]), values[2], vAdd);
        addVec3(vTarget, vAdd, vTarget);
    } break;

    // For most dynops, just treat it as a fixed array of F32s
    case DynOpType_Jitter:
    case DynOpType_Inherit:
    case DynOpType_Add:
    case DynOpType_Multiply:
    case DynOpType_Min:
    case DynOpType_Max:
    default:
        {
            int i;
            if (uiValuesSpecd >= 3) // values spread to components // changed so there is only one seed
            {
                for (i = 0; i < 3; i++)
                    nonarray_applydynop(tpi, column, dstStruct, srcStruct, i, optype, values+i, uiValuesSpecd, seed);
            }
            else // same value & seed to each component
            {
                for (i = 0; i < 3; i++)
                    nonarray_applydynop(tpi, column, dstStruct, srcStruct, i, optype, values, uiValuesSpecd, seed);
            }
        } break;
    }
}

int matpyr_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    int v;
    F32 pyr[3];
    Vec3* mat;

    for (v = 0; v < 3; v++)
    {
        const char* nexttoken = GetNextParameter(tok);
        if (!nexttoken) return 0;
        pyr[v] = GetFloat(tok, nexttoken);
    }
    mat = (Vec3*)TokenStoreGetPointer(tpi, column, structptr, 0);
//    RADVEC3(pyr);
    createMat3YPR(mat, pyr);
    return 0;
}

void matpyr_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    F32 pyr[3];
    Vec3* mat = (Vec3*)TokenStoreGetPointer(tpi, column, structptr, 0);

    getMat3YPR(mat, pyr);
//    DEGVEC3(pyr);
    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    WriteString(out, " ", 0, 0);
    WriteFloat(out, pyr[0], 0, 0, tpi[column].subtable);
    WriteString(out, ", ", 0, 0);
    WriteFloat(out, pyr[1], 0, 0, tpi[column].subtable);
    WriteString(out, ", ", 0, 0);
    WriteFloat(out, pyr[2], 0, showname, tpi[column].subtable);
}

int matpyr_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int i, ok = 1;
    Vec3    pyr;
    Vec3* mat = (Vec3*)TokenStoreGetPointer(tpi, column, structptr, 0);

    getMat3YPR(mat, pyr);
    for (i = 0; i < 3; i++)
        ok = ok && SimpleBufWriteF32(pyr[i], file);
    *datasum += sizeof(F32)*3;
    return ok;
}

int matpyr_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int i, ok = 1;
    Vec3    pyr;
    Vec3* mat = (Vec3*)TokenStoreGetPointer(tpi, column, structptr, 0);

    for (i = 0; i < 3; i++)
        ok = ok && SimpleBufReadF32(pyr+i, file);
    *datasum += sizeof(F32)*3;
    createMat3YPR(mat, pyr);
    return ok;
}

void matpyr_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    Vec3    pyr;
    Vec3* mat = (Vec3*)TokenStoreGetPointer(tpi, column, newstruct, 0);

    getMat3YPR(mat, pyr);
    pktSendF32(pak, pyr[0]);
    pktSendF32(pak, pyr[1]);
    pktSendF32(pak, pyr[2]);
}

void matpyr_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    Vec3* mat = structptr? (Vec3*)TokenStoreGetPointer(tpi, column, structptr, 0): 0;
    Vec3 pyr;
    int out_of_order = 0;
    if (pktidptr) {
        out_of_order = PTR_TO_U32(*pktidptr) > pak->id;
        if (!out_of_order) // matpyrs store packet id's on _every_ update (don't do diffs)
            *pktidptr = U32_TO_PTR(pak->id);
    }
    pyr[0] = pktGetF32(pak);
    pyr[1] = pktGetF32(pak);
    pyr[2] = pktGetF32(pak);
    if (!out_of_order && structptr)
        createMat3YPR(mat, pyr);
}

void matpyr_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam)
{
    Vec3* matA = (Vec3*)TokenStoreGetPointer(tpi, column, structA, 0);
    Vec3* matB = (Vec3*)TokenStoreGetPointer(tpi, column, structB, 0);
    Vec3* matDest = (Vec3*)TokenStoreGetPointer(tpi, column, destStruct, 0);
    Vec3 pyrA, pyrB, pyrDest;

    getMat3YPR(matA, pyrA);
    getMat3YPR(matB, pyrB);
    pyrDest[0] = interpF32(interpParam, pyrA[0], pyrB[0]);
    pyrDest[1] = interpF32(interpParam, pyrA[1], pyrB[1]);
    pyrDest[2] = interpF32(interpParam, pyrA[2], pyrB[2]);
    createMat3YPR(matDest, pyrDest);
}

void matpyr_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime)
{
    Vec3* matA = (Vec3*)TokenStoreGetPointer(tpi, column, structA, 0);
    Vec3* matB = (Vec3*)TokenStoreGetPointer(tpi, column, structB, 0);
    Vec3* matDest = (Vec3*)TokenStoreGetPointer(tpi, column, destStruct, 0);
    Vec3 pyrA, pyrB, pyrDest;

    getMat3YPR(matA, pyrA);
    getMat3YPR(matB, pyrB);
    pyrDest[0] = (pyrB[0] - pyrA[0]) / deltaTime;
    pyrDest[1] = (pyrB[1] - pyrA[1]) / deltaTime;
    pyrDest[2] = (pyrB[2] - pyrA[2]) / deltaTime;
    createMat3YPR(matDest, pyrDest);
}

void matpyr_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime)
{
    Vec3* matV = (Vec3*)TokenStoreGetPointer(tpi, column, valueStruct, 0);
    Vec3* matR = (Vec3*)TokenStoreGetPointer(tpi, column, rateStruct, 0);
    Vec3* matDest = (Vec3*)TokenStoreGetPointer(tpi, column, destStruct, 0);
    Vec3 pyrV, pyrR, pyrDest;

    getMat3YPR(matV, pyrV);
    getMat3YPR(matR, pyrR);
    pyrDest[0] = pyrV[0] + deltaTime * pyrR[0];
    pyrDest[1] = pyrV[1] + deltaTime * pyrR[1];
    pyrDest[2] = pyrV[2] + deltaTime * pyrR[2];
    createMat3YPR(matDest, pyrDest);
}

int filename_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    char* nexttoken = cpp_const_cast(char*)(GetNextParameter(tok));
    if (!nexttoken) return 0;
    _strupr_s(nexttoken, strlen(nexttoken)+1);
    TokenStoreSetString(tpi, column, structptr, index, forwardSlashes(nexttoken), NULL, NULL);
    return 0;
}

bool filename_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    char* val = TokenStoreGetString(tpi, column, structptr, index);
    if (val)
    {
        _strlwr_s(val, strlen(val)+1);
        strcpy_s(str, str_size, val);
    }
    else str[0] = 0;
    return true;
}

bool filename_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    if (str && str[0])
    {
        _strupr_s(str, strlen(str)+1);
        TokenStoreSetString(tpi, column, structptr, index, forwardSlashes(str), NULL, NULL);
    }
    else
        TokenStoreFreeString(tpi, column, structptr, index);
    return true;
}

/////////////////////////////////////////////// complex types

ParseInfoFieldUsage link_interpretfield(ParseTable tpi[], int column, ParseInfoField field)
{
    switch (field)
    {
    case ParamField:
        if (tpi[column].type & TOK_FIXED_ARRAY)
            return NumberOfElements;
        if (tpi[column].type & TOK_EARRAY)
            return NoFieldUsage;
        return PointerToDefaultString;
    case SubtableField:
        return PointerToLinkDictionary;
    }
    return NoFieldUsage;
}

void link_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    void* link;
    char* str = (char*)tpi[column].param;
    if (!str) return;
    link = StructLinkFromString(tpi[column].subtable, str);
    if (!link)
        Errorf("default value %s invalid for TOK_LINK", str);
    else
        TokenStoreSetPointer(tpi, column, structptr, index, link);
}

int link_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    void* link;
    const char* nexttoken = GetNextParameter(tok);
    if (!nexttoken) return 0;
    link = StructLinkFromString(tpi[column].subtable, nexttoken);
    if( !link )
        ErrorFilenamef(TokenizerGetFileName(tok),"Parser: couldn't link to parameter %s. see %s", nexttoken, TokenizerGetFileAndLine(tok));
    else
        TokenStoreSetPointer(tpi, column, structptr, index, link);

    return 0;
}

void link_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    char buf[MAX_LINK_LEN];
    void* link = TokenStoreGetPointer(tpi, column, structptr, index);

    if (!link) return; // links always default to zero
    if (!StructLinkToString(tpi[column].subtable, link, SAFESTR(buf)))
        Errorf("bad pointer given to ParserMakeLinkString!\n");
    else
    {
        if (showname) WriteString(out, tpi[column].name, level+1, 0);
        WriteString(out, " ", 0, 0);
        WriteQuotedString(out, buf, 0, showname);
    }
}

int link_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    char buf[MAX_LINK_LEN];
    int wr, success = 1;
    void* link = TokenStoreGetPointer(tpi, column, structptr, index);

    if (!link)
    {
        wr = WritePascalString(file, "");
        success = success && wr;
        *datasum += wr;
    }
    else if (!StructLinkToString(tpi[column].subtable, link, SAFESTR(buf)))
    {
        Errorf("bad pointer given to StructLinkToString!\n");
        success = 0;
    }
    else
    {
        wr = WritePascalString(file, buf);
        success = success && wr;
        *datasum += wr;
    }
    return success;    
}

int link_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    char buf[MAX_LINK_LEN];
    int re, ok = 1;

    buf[0] = 0;
    re = ReadPascalString(file, SAFESTR(buf));
    ok = ok && re;
    *datasum += re;
    if (buf[0])
        TokenStoreSetPointer(tpi, column, structptr, index, StructLinkFromString(tpi[column].subtable, buf));
    else
        TokenStoreSetPointer(tpi, column, structptr, index, NULL);
    return ok;
}

void link_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    char buf[MAX_LINK_LEN];
    void* link = TokenStoreGetPointer(tpi, column, newstruct, index);

    if (!link)
    {
        pktSendString(pak, "");
    }
    else if (StructLinkToString(tpi[column].subtable, link, SAFESTR(buf)))
    {
        Errorf("bad pointer given to StructLinkToString!\n");
        pktSendString(pak, "");
    }
    else
    {
        pktSendString(pak, buf);
    }
}

void link_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    char* str;
    int out_of_order = 0;
    if (pktidptr) {
        out_of_order = PTR_TO_U32(*pktidptr) > pak->id;
        if (!out_of_order) // strings store packet id's on _every_ update (don't do diffs)
            *pktidptr = U32_TO_PTR(pak->id);
    }
    str = pktGetString(pak);
    if (!out_of_order && structptr)
    {
        if (!str || !str[0])
            TokenStoreSetPointer(tpi, column, structptr, index, NULL);
        else
            TokenStoreSetPointer(tpi, column, structptr, index, StructLinkFromString(tpi[column].subtable, str));
    }
}

bool link_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    void* link = TokenStoreGetPointer(tpi, column, structptr, index);
    if (!link)
    {
        str[0] = 0;
        return true;
    }
    return StructLinkToString(tpi[column].subtable, link, str, str_size)? true: false;
}

bool link_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    void* link;
    
    if (!str || !str[0])
    {
        TokenStoreSetPointer(tpi, column, structptr, index, NULL);
        return true;
    }
    link = StructLinkFromString(tpi[column].subtable, str);
    if (!link)
        return false;
    TokenStoreSetPointer(tpi, column, structptr, index, link);
    return true;
}

void link_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    int zero = 0;
    void* link = TokenStoreGetPointer(tpi, column, structptr, index);
    char buf[MAX_LINK_LEN];

    if (link)
    {
        StructLinkToString(tpi[column].subtable, link, SAFESTR(buf));
        cryptAdler32Update(buf, (int)strlen(buf));
    }
    else
        cryptAdler32Update((U8*)&zero, sizeof(int));
}

int link_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    char left[MAX_LINK_LEN];
    char right[MAX_LINK_LEN];
    void* llink = TokenStoreGetPointer(tpi,    column, lhs, index);
    void* rlink = TokenStoreGetPointer(tpi, column, rhs, index);
    if (!llink && !rlink) return 0;
    if (!llink) return -1;
    if (!rlink) return 1;
    StructLinkToString(tpi[column].subtable, llink, SAFESTR(left));
    StructLinkToString(tpi[column].subtable, rlink, SAFESTR(right));
    return stricmp(left, right);
}

void link_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    MEM_ALIGN(*size, sizeof(void*));
    tpi[column].storeoffset = *size;
    (*size) += sizeof(void*);
}

void link_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    TokenStoreSetPointer(tpi, column, dest, index, TokenStoreGetPointer(tpi, column, src, index));
}

///// reference functions

ParseInfoFieldUsage reference_interpretfield(ParseTable tpi[], int column, ParseInfoField field)
{
    switch (field)
    {
    case ParamField:
        if (tpi[column].type & TOK_FIXED_ARRAY)
            return NumberOfElements;
        if (tpi[column].type & TOK_EARRAY)
            return NoFieldUsage;
        return PointerToDefaultString;
    case SubtableField:
        return PointerToDictionaryName;
    }
    return NoFieldUsage;
}

void reference_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    char* str = (char*)tpi[column].param;
    if (!str) return;
    TokenStoreSetRef(tpi, column, structptr, index, str);
}

void reference_destroystruct(ParseTable tpi[], int column, void* structptr, int index)
{
    TokenStoreClearRef(tpi, column, structptr, index);
}

int reference_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    const char* nexttoken = GetNextParameter(tok);
    if (!nexttoken) return 0;
    TokenStoreSetRef(tpi, column, structptr, index, nexttoken);
    return 0;
}

void reference_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    char buf[MAX_STRING_LENGTH];
    if (TokenStoreGetRefString(tpi, column, structptr, index, SAFESTR(buf)))
    {
        if (showname) WriteString(out, tpi[column].name, level+1, 0);
        WriteString(out, " ", 0, 0);
        WriteQuotedString(out, buf, 0, showname);
    } // otherwise, null
}

int reference_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    char buf[MAX_STRING_LENGTH];
    int wr, success = 1;
    if (TokenStoreGetRefString(tpi, column, structptr, index, SAFESTR(buf)))
    {
        wr = WritePascalString(file, buf);
        success = success && wr;
        *datasum += wr;
    }
    else
    {
        wr = WritePascalString(file, "");
        success = success && wr;
        *datasum += wr;
    }
    return success;    
}

int reference_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    char buf[MAX_STRING_LENGTH];
    int re, ok = 1;

    buf[0] = 0;
    re = ReadPascalString(file, SAFESTR(buf));
    ok = ok && re;
    *datasum += re;
    TokenStoreSetRef(tpi, column, structptr, index, buf);
    return ok;
}

void reference_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    char buf[MAX_STRING_LENGTH];

    if (TokenStoreGetRefString(tpi, column, newstruct, index, SAFESTR(buf)))
    {
        pktSendString(pak, buf);
    }
    else
    {
        pktSendString(pak, "");
    }
}

void reference_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    char* str;
    int out_of_order = 0;
    if (pktidptr) {
        out_of_order = PTR_TO_U32(*pktidptr) > pak->id;
        if (!out_of_order) // strings store packet id's on _every_ update (don't do diffs)
            *pktidptr = U32_TO_PTR(pak->id);
    }
    str = pktGetString(pak);
    if (!out_of_order && structptr)
    {
        TokenStoreSetRef(tpi, column, structptr, index, str);
    }
}

bool reference_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    TokenStoreGetRefString(tpi, column, structptr, index, str, str_size);
    return true;
}

bool reference_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    TokenStoreSetRef(tpi, column, structptr, index, str);
    return true;
}

void reference_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    int zero = 0;
    char buf[MAX_STRING_LENGTH];
    TokenStoreGetRefString(tpi, column, structptr, index, SAFESTR(buf));
    if (buf[0])
        cryptAdler32Update(buf, (int)strlen(buf));
    else
        cryptAdler32Update((U8*)&zero, sizeof(int));
}

int reference_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    char left[MAX_STRING_LENGTH];
    char right[MAX_STRING_LENGTH];
    TokenStoreGetRefString(tpi,    column, lhs, index, SAFESTR(left));
    TokenStoreGetRefString(tpi, column, rhs, index, SAFESTR(right));
    return stricmp(left, right);
}

void reference_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    MEM_ALIGN(*size, sizeof(ReferenceHandle));
    tpi[column].storeoffset = *size;
    (*size) += sizeof(ReferenceHandle);
}

void reference_copystruct(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData)
{
    TokenStoreSetPointer(tpi, column, dest, index, NULL);
    TokenStoreCopyRef(tpi, column, dest, src, index);
}

void reference_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    TokenStoreCopyRef(tpi, column, dest, src, index);
}

void functioncall_destroystruct(ParseTable tpi[], int column, void* structptr, int index)
{
    StructFunctionCall* callstruct = TokenStoreGetPointer(tpi, column, structptr, index);
    if (callstruct) StructFreeFunctionCall(callstruct);
}

int functioncall_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    StructFunctionCall*** store = (StructFunctionCall***)TokenStoreGetEArray(tpi, column, structptr);
    TokenizerParseFunctionCall(tok, store);
    return 0;
}

void functioncall_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    StructFunctionCall** ea = *(StructFunctionCall***)TokenStoreGetEArray(tpi, column, structptr);
    if (eaSize(&ea))
    {
        if (showname) WriteString(out, tpi[column].name, level+1, 0);
        WriteString(out, " ", 0, 0);
        WriteTextFunctionCalls(out, ea);
        if (showname) WriteString(out, "", 0, 1);
    }
}

int functioncall_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    StructFunctionCall** ea = *(StructFunctionCall***)TokenStoreGetEArray(tpi, column, structptr);
    return WriteBinaryFunctionCalls(file, ea, datasum);
}

int functioncall_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    StructFunctionCall*** ea = (StructFunctionCall***)TokenStoreGetEArray(tpi, column, structptr);
    return ReadBinaryFunctionCalls(file, ea, datasum);
}

void functioncall_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    StructFunctionCall** ea = *(StructFunctionCall***)TokenStoreGetEArray(tpi, column, newstruct);
    SendDiffFunctionCalls(pak, ea);
}

void functioncall_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    StructFunctionCall*** ea = (StructFunctionCall***)TokenStoreGetEArray(tpi, column, structptr);
    RecvDiffFunctionCalls(pak, ea);
}

void functioncall_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    StructFunctionCall* call = TokenStoreGetPointer(tpi, column, structptr, index);
    ParserUpdateCRCFunctionCall(call);
}

int CompareFunctionCalls(StructFunctionCall* left, StructFunctionCall* right)
{
    int ret = 0;
    int i, lcount, rcount;

    if (!left && !right) return 0;
    if (!left) return -1;
    if (!right) return 1;

    ret = stricmp(left->function, right->function);
    if (ret) return ret;

    lcount = eaSize(&left->params);
    rcount = eaSize(&right->params);
    if (lcount != rcount)
        return lcount - rcount;

    for (i = 0; i < lcount && ret == 0; i++)
        ret = CompareFunctionCalls(left->params[i], right->params[i]);
    return ret;
}

int functioncall_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    StructFunctionCall* left = TokenStoreGetPointer(tpi, column, lhs, index);
    StructFunctionCall* right = TokenStoreGetPointer(tpi, column, rhs, index);
    return CompareFunctionCalls(left, right);
}

size_t functioncall_memusage(ParseTable tpi[], int column, void* structptr, int index)
{
    StructFunctionCall* callstruct = TokenStoreGetPointer(tpi, column, structptr, index);
    return ParserGetFunctionCallMemoryUsage(callstruct);
}

void functioncall_copystruct(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData)
{
    StructFunctionCall* fromstruct = TokenStoreGetPointer(tpi, column, src, index);
    TokenStoreSetPointer(tpi, column, dest, index, ParserCompressFunctionCall(fromstruct, memAllocator, customData));
}

void functioncall_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    functioncall_destroystruct(tpi, column, dest, index);
    functioncall_copystruct(tpi, column, dest, src, index, memAllocator, customData);
}

void unparsed_destroystruct(ParseTable tpi[], int column, void* structptr, int index)
{
    int p;
    StructParams* paramstruct = TokenStoreGetPointer(tpi, column, structptr, index);
    if (!paramstruct) return;
    if (paramstruct->params)
    {
        for (p = 0; p < eaSize(&paramstruct->params); p++)
            StructFreeString(paramstruct->params[p]);
        eaDestroy(&paramstruct->params);
    }
    TokenStoreFree(tpi, column, structptr, index);
}

int unparsed_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    StructParams* paramstruct = TokenStoreAlloc(tpi, column, structptr, -1, sizeof(StructParams), NULL, NULL);

    // add each parameter
    const char* nexttoken = TokenizerPeek(tok, 0, 1);
    while (nexttoken && !IsEol(nexttoken))
    {
        nexttoken = TokenizerGet(tok, 0, 1);
        if (!paramstruct->params) eaCreate(&paramstruct->params);
        eaPush(&paramstruct->params, StructAllocString(nexttoken));
        nexttoken = TokenizerPeek(tok, 0, 1);
    }
    return 0;
}

void unparsed_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int i, numelems = TokenStoreGetNumElems(tpi, column, structptr);

    for (i = 0; i < numelems; i++)
    {
        int p;
        StructParams* paramstruct = TokenStoreGetPointer(tpi, column, structptr, i);
        if (showname) WriteString(out, tpi[column].name, level+1, 0);
        for (p = 0; p < eaSize(&paramstruct->params); p++)
        {
            if (p) WriteString(out, ",", 0, 0);
            WriteString(out, " ", 0, 0);
            WriteQuotedString(out, paramstruct->params[p], 0, 0);
        }
        if (showname) WriteString(out, "", 0, 1);
    }
}

int unparsed_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    StructParams* paramstruct = TokenStoreGetPointer(tpi, column, structptr, index);
    int i, size = eaSize(&paramstruct->params);
    int ok = 1;

    ok = ok && SimpleBufWriteU32(size, file);
    *datasum += sizeof(int);
    for (i = 0; i < size; i++)
    {
        int wr = WritePascalString(file, paramstruct->params[i]);
        ok = ok && wr;
        *datasum += wr;
    }
    return ok;
}

int unparsed_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    StructParams* paramstruct = TokenStoreAlloc(tpi, column, structptr, index, sizeof(StructParams), NULL, NULL);
    int re, i, size = 0;
    int ok = 1;
    char buf[MAX_STRING_LENGTH];

    ok = ok && SimpleBufReadU32((U32*)&size, file);
    *datasum += sizeof(int);
    for (i = 0; i < size; i++)
    {
        re = ReadPascalString(file, SAFESTR(buf));
        ok = ok && re;
        *datasum += re;
        if (!paramstruct->params) eaCreate(&paramstruct->params);
        eaPush(&paramstruct->params, StructAllocString(buf));
    }
    return ok;
}

void unparsed_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    int i, zero = 0;
    StructParams* paramstruct = TokenStoreGetPointer(tpi, column, structptr, index);
    for (i = 0; i < eaSize(&paramstruct->params); i++)
    {
        char* str = paramstruct->params[i];
        if (str)
            cryptAdler32Update(str, (int)strlen(str));
        else
            cryptAdler32Update((U8*)&zero, sizeof(int));
    }
}

int unparsed_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    int i, lcount, rcount, ret = 0;
    StructParams* left = TokenStoreGetPointer(tpi, column, lhs, index);
    StructParams* right = TokenStoreGetPointer(tpi, column, rhs, index);

    if (!left && !right) return 0;
    if (!left) return -1;
    if (!right) return 1;

    lcount = eaSize(&left->params);
    rcount = eaSize(&right->params);
    if (lcount != rcount) return lcount - rcount;

    for (i = 0; i < lcount && ret == 0; i++)
        ret = stricmp(left->params[i], right->params[i]);
    return ret;
}

size_t unparsed_memusage(ParseTable tpi[], int column, void* structptr, int index)
{
    StructParams* paramstruct = TokenStoreGetPointer(tpi, column, structptr, index);
    size_t size;
    int i;

    size = sizeof(StructParams);
    size += eaMemUsage(&paramstruct->params);
    for (i = 0; i < eaSize(&paramstruct->params); i++)
    {
        size += strlen(paramstruct->params[i])+1;
    }
    return size;
}

void unparsed_copystruct(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData)
{
    int p, len;
    StructParams* fromstruct = TokenStoreGetPointer(tpi, column, src, index);
    StructParams* deststruct;
    TokenStoreSetPointer(tpi, column, dest, index, NULL);
    if (!fromstruct) return;

    deststruct = TokenStoreAlloc(tpi, column, dest, index, sizeof(StructParams), memAllocator, customData);
    if (fromstruct->params)
    {
        eaCompress(&deststruct->params, &fromstruct->params, memAllocator, customData);
        for (p = 0; p < eaSize(&deststruct->params); p++)
        {
            if (memAllocator)
            {
                len = (int)strlen(fromstruct->params[p])+1;
                deststruct->params[p] = memAllocator(customData, len);
                if (deststruct->params[p])
                    strcpy_s(deststruct->params[p], len, fromstruct->params[p]);
            }
            else
                deststruct->params[p] = StructAllocString(fromstruct->params[p]);
        }
    }
}

void unparsed_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    unparsed_destroystruct(tpi, column, dest, index);
    unparsed_copystruct(tpi, column, dest, src, index, memAllocator, customData);
}

ParseInfoFieldUsage struct_interpretfield(ParseTable tpi[], int column, ParseInfoField field)
{
    switch (field)
    {
    case ParamField:
        return SizeOfSubstruct;
    case SubtableField:
        return PointerToSubtable;
    }
    return NoFieldUsage;
}

void struct_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    void* substruct = TokenStoreGetPointer(tpi, column, structptr, index);
    if (substruct) // if we already have a pointer, we're embedded, and should init
    {
        StructInit(substruct, tpi[column].param, tpi[column].subtable);
    }
}

void struct_destroystruct(ParseTable tpi[], int column, void* structptr, int index)
{
    void* substruct = TokenStoreGetPointer(tpi, column, structptr, index);
    if (substruct)
    {
        StructClear(tpi[column].subtable, substruct);
        TokenStoreFree(tpi, column, structptr, index);
    }
}

int struct_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    void* substruct = TokenStoreAlloc(tpi, column, structptr, -1, tpi[column].param, NULL, NULL);
    StructInit(substruct, tpi[column].param, tpi[column].subtable);
    if (!ParserReadTokenizer(tok, tpi[column].subtable, substruct, callback)) g_parseerr = 1;
    return 0;
}

void struct_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int i, numelems = TokenStoreGetNumElems(tpi, column, structptr);

    for (i = 0; i < numelems; i++)
    {
        void* substruct = TokenStoreGetPointer(tpi, column, structptr, i);
        if (substruct)
        {
            if (showname) WriteString(out, tpi[column].name, level+1, 0);
            InnerWriteTextFile(out, tpi[column].subtable, substruct, level+1, iOptionFlagsToMatch, iOptionFlagsToExclude);
            if (showname) WriteString(out, "", 0, 1);
        }
    }
}

int struct_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int ok = 1;
    int subsum = 0;
    void* substruct = TokenStoreGetPointer(tpi, column, structptr, index);
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

    if (storage == TOK_STORAGE_INDIRECT_SINGLE) // optional struct
    {
        int hassub = substruct? 1: 0;
        ok = ok && SimpleBufWriteU32(hassub, file);
        *datasum += sizeof(int);
        if (!hassub) return ok;
    }
    else devassert(substruct);

    ok = ok && ParserWriteBinaryTable(file, tpi[column].subtable, substruct, &subsum, iOptionFlagsToMatch, iOptionFlagsToExclude);
    *datasum += subsum;
    return ok;
}

int struct_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int ok = 1;
    int subsum = 0;
    void* substruct;
    U32 storage = TokenStoreGetStorageType(tpi[column].type);

    if (storage == TOK_STORAGE_INDIRECT_SINGLE) // optional struct
    {
        U32 hassub = 0;
        ok = ok && SimpleBufReadU32(&hassub, file);
        *datasum += sizeof(int);
        if (!hassub || !ok)
            return ok;
    }

    // if here, then we have a struct
    substruct = TokenStoreAlloc(tpi, column, structptr, index, tpi[column].param, NULL, NULL);
    ok = ok && ParserReadBinaryTable(file, tpi[column].subtable, substruct, &subsum);
    *datasum += subsum;
    return ok;
}

void struct_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    void* oldp = oldstruct? TokenStoreGetPointer(tpi, column, oldstruct, index): 0;
    void* newp = TokenStoreGetPointer(tpi, column, newstruct, index);

    if (newp && !oldp) sendAbsolute = 1;
    pktSendBits(pak, 1, newp? 1: 0);

    if (newp)
    {
        pktSendBits(pak, 1, sendAbsolute? 1: 0);
        if (sendAbsolute)
            sdPackStruct(tpi[column].subtable, pak, NULL, newp, forcePackAll, 0, iOptionFlagsToMatch, iOptionFlagsToExclude);
        else
            sdPackStruct(tpi[column].subtable, pak, oldp, newp, forcePackAll, allowDiffs, iOptionFlagsToMatch, iOptionFlagsToExclude);
    }
}

void struct_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    void* substruct = structptr? TokenStoreGetPointer(tpi, column, structptr, index): 0;
    int getabsolute, gettingstruct = pktGetBits(pak, 1);
    if (!gettingstruct)
    {
        if (pktidptr && *pktidptr) ParserFreePktIds(tpi[column].subtable, (void***)pktidptr);
        if (structptr) struct_destroystruct(tpi, column, structptr, index);
        return;
    }

    // now we're getting a struct
    if (!substruct && structptr) 
        substruct = TokenStoreAlloc(tpi, column, structptr, index, tpi[column].param, NULL, NULL);
    getabsolute = pktGetBits(pak, 1);
    sdUnpackStruct(tpi[column].subtable, pak, substruct, (void***)pktidptr, !getabsolute);
}

void struct_freepktids(ParseTable tpi[], int column, void** pktidptr)
{
    if (*pktidptr) ParserFreePktIds(tpi[column].subtable, (void***)pktidptr);
}

void struct_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    void* substruct = TokenStoreGetPointer(tpi, column, structptr, index);
    if (substruct) StructUpdateCRC(tpi[column].subtable, substruct);
}

int struct_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    void* left = TokenStoreGetPointer(tpi, column, lhs, index);
    void* right = TokenStoreGetPointer(tpi, column, rhs, index);
    if (!left && !right) return 0;
    if (!left) return -1;
    if (!right) return 1;
    return ParserCompareStruct(tpi[column].subtable, left, right);
}

size_t struct_memusage(ParseTable tpi[], int column, void* structptr, int index)
{
    void* substruct = TokenStoreGetPointer(tpi, column, structptr, index);
    U32 storage = TokenStoreGetStorageType(tpi[column].type);
    size_t size = (storage == TOK_STORAGE_DIRECT_SINGLE)? 0: tpi[column].param; // if direct, space counted in parent struct
    if (!substruct) return 0;
    return StructGetMemoryUsage(tpi[column].subtable, substruct, size);
}

void struct_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    int storage = TokenStoreGetStorageType(tpi[column].type);

    if (storage == TOK_STORAGE_DIRECT_SINGLE)
    {
        tpi[column].storeoffset = *size;
        (*size) += tpi[column].param; // size of structure
    }
    else
    {
        MEM_ALIGN(*size, sizeof(void*));
        tpi[column].storeoffset = *size;
        (*size) += sizeof(void*);
    }
}

void struct_copystruct(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData)
{
    void* fromstruct = TokenStoreGetPointer(tpi, column, src, index);
    void* substruct;
    int storage = TokenStoreGetStorageType(tpi[column].type);

    if (storage != TOK_STORAGE_DIRECT_SINGLE)
        TokenStoreSetPointer(tpi, column, dest, index, NULL); // make sure alloc doesn't free
    if (fromstruct)
    {
        substruct = TokenStoreAlloc(tpi, column, dest, index, tpi[column].param, memAllocator, customData);
        StructCompress(tpi[column].subtable, fromstruct, tpi[column].param, substruct, memAllocator, customData);
    }
}

void struct_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    void* srcstruct = TokenStoreGetPointer(tpi, column, src, index);
    void* deststruct = TokenStoreGetPointer(tpi, column, dest, index);

    if (!srcstruct)
    {
        if (deststruct)
            struct_destroystruct(tpi, column, dest, index);
        return;
    }

    if (!deststruct)
        deststruct = TokenStoreAlloc(tpi, column, dest, index, tpi[column].param, memAllocator, customData);
    StructCopy(tpi[column].subtable, srcstruct, deststruct, iOptionFlagsToMatch, iOptionFlagsToExclude);
}

void struct_endianswap(ParseTable tpi[], int column, void* structptr, int index)
{
    void* substruct = TokenStoreGetPointer(tpi, column, structptr, index);
    if (substruct)
        endianSwapStruct(tpi[column].subtable, substruct);
}

void struct_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam)
{
    void* subA = TokenStoreGetPointer(tpi, column, structA, index);
    void* subB = TokenStoreGetPointer(tpi, column, structB, index);
    void* subDest = TokenStoreGetPointer(tpi, column, destStruct, index);

    if (!subA || !subB) return;
    if (!subDest) subDest = TokenStoreAlloc(tpi, column, destStruct, index, tpi[column].param, NULL, NULL);
    StructInterpolate(tpi[column].subtable, subA, subB, subDest, interpParam);
}

void struct_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime)
{
    void* subA = TokenStoreGetPointer(tpi, column, structA, index);
    void* subB = TokenStoreGetPointer(tpi, column, structB, index);
    void* subDest = TokenStoreGetPointer(tpi, column, destStruct, index);

    if (!subA || !subB) return;
    if (!subDest) subDest = TokenStoreAlloc(tpi, column, destStruct, index, tpi[column].param, NULL, NULL);
    StructCalcRate(tpi[column].subtable, subA, subB, subDest, deltaTime);
}

void struct_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime)
{
    void* subV = TokenStoreGetPointer(tpi, column, valueStruct, index);
    void* subR = TokenStoreGetPointer(tpi, column, rateStruct, index);
    void* subDest = TokenStoreGetPointer(tpi, column, destStruct, index);

    if (!subV || !subR) return;
    if (!subDest) subDest = TokenStoreAlloc(tpi, column, destStruct, index, tpi[column].param, NULL, NULL);
    StructIntegrate(tpi[column].subtable, subV, subR, subDest, deltaTime);
}

void struct_calccyclic(ParseTable tpi[], int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, int index, F32 fStartTime, F32 deltaTime)
{
    void* subV = TokenStoreGetPointer(tpi, column, valueStruct, index);
    void* subA = TokenStoreGetPointer(tpi, column, ampStruct, index);
    void* subF = TokenStoreGetPointer(tpi, column, freqStruct, index);
    void* subC = TokenStoreGetPointer(tpi, column, cycleStruct, index);
    void* subDest = TokenStoreGetPointer(tpi, column, destStruct, index);

    if (!subV || !subA || !subF || !subC) return;
    if (!subDest) subDest = TokenStoreAlloc(tpi, column, destStruct, index, tpi[column].param, NULL, NULL);
    StructCalcCyclic(tpi[column].subtable, subV, subA, subF, subC, subDest, fStartTime, deltaTime);
}

void struct_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, F32* values, U8 uiValuesSpecd, U32* seed)
{
    void* subDest = TokenStoreGetPointer(tpi, column, dstStruct, index);
    void* subSrc = srcStruct? TokenStoreGetPointer(tpi, column, srcStruct, index): 0;
    StructApplyDynOp(tpi[column].subtable, optype, values, uiValuesSpecd, subDest, subSrc, seed);
}

void stashtable_destroystruct(ParseTable tpi[], int column, void* structptr, int index)
{
    StashTable st = TokenStoreGetPointer(tpi, column, structptr, index);
    if (st) stashTableDestroy(st);
    TokenStoreSetPointer(tpi, column, structptr, index, NULL);
}

size_t stashtable_memusage(ParseTable tpi[], int column, void* structptr, int index)
{
    StashTable st = TokenStoreGetPointer(tpi, column, structptr, index);
    if (st) return stashGetCopyTargetAllocSize(st);
    return 0;
}

void stashtable_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    MEM_ALIGN(*size, sizeof(void*));
    tpi[column].storeoffset = *size;
    (*size) += sizeof(void*);
}

void stashtable_copystruct(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData)
{
    StashTable srcst = TokenStoreGetPointer(tpi, column, src, index);
    if (srcst) TokenStoreSetPointer(tpi, column, dest, index, stashTableClone(srcst, memAllocator, customData));
}

void stashtable_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    stashtable_destroystruct(tpi, column, dest, index);
    stashtable_copystruct(tpi, column, dest, src, index, memAllocator, customData);
}

//////////////////////////////////////////////////////////////// array stuff

void nonarray_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    if (TYPE_INFO(tpi[column].type).initstruct)
        TYPE_INFO(tpi[column].type).initstruct(tpi, column, structptr, index);
}

void nonarray_destroystruct(ParseTable tpi[], int column, void* structptr, int index)
{
    if (TYPE_INFO(tpi[column].type).destroystruct)
        TYPE_INFO(tpi[column].type).destroystruct(tpi, column, structptr, index);
}

int nonarray_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    if (TYPE_INFO(tpi[column].type).parse)
        return TYPE_INFO(tpi[column].type).parse(tok, tpi, column, structptr, index, callback);
    return 0;
}

void nonarray_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    if (TYPE_INFO(tpi[column].type).writetext)
        TYPE_INFO(tpi[column].type).writetext(out, tpi, column, structptr, index, showname, level, iOptionFlagsToMatch, iOptionFlagsToExclude);
}

int nonarray_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    if (TYPE_INFO(tpi[column].type).writebin)
        return TYPE_INFO(tpi[column].type).writebin(file, tpi, column, structptr, index, datasum, iOptionFlagsToMatch, iOptionFlagsToExclude);
    return 1;
}

int nonarray_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    if (TYPE_INFO(tpi[column].type).readbin)
        return TYPE_INFO(tpi[column].type).readbin(file, tpi, column, structptr, index, datasum);
    return 1;
}

void nonarray_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    if (TYPE_INFO(tpi[column].type).senddiff)
        TYPE_INFO(tpi[column].type).senddiff(pak, tpi, column, oldstruct, newstruct, index, sendAbsolute, forcePackAll, allowDiffs, iOptionFlagsToMatch, iOptionFlagsToExclude);
}

void nonarray_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    if (TYPE_INFO(tpi[column].type).recvdiff)
        TYPE_INFO(tpi[column].type).recvdiff(pak, tpi, column, structptr, index, absValues, pktidptr);
}

void nonarray_freepktids(ParseTable tpi[], int column, void** pktidptr)
{
    if (TYPE_INFO(tpi[column].type).freepktids)
        TYPE_INFO(tpi[column].type).freepktids(tpi, column, pktidptr);
}

bool nonarray_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    if (TYPE_INFO(tpi[column].type).tosimple)
        return TYPE_INFO(tpi[column].type).tosimple(tpi, column, structptr, index, str, str_size, prettyprint);
    return false;
}

bool nonarray_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    if (TYPE_INFO(tpi[column].type).fromsimple)
        return TYPE_INFO(tpi[column].type).fromsimple(tpi, column, structptr, index, str);
    return false;
}

void nonarray_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    if (TYPE_INFO(tpi[column].type).updatecrc)
        TYPE_INFO(tpi[column].type).updatecrc(tpi, column, structptr, index);
}

int nonarray_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    if (TYPE_INFO(tpi[column].type).compare)
        return TYPE_INFO(tpi[column].type).compare(tpi, column, lhs, rhs, index);
    return 0;
}

size_t nonarray_memusage(ParseTable tpi[], int column, void* structptr, int index)
{
    if (TYPE_INFO(tpi[column].type).memusage)
        return TYPE_INFO(tpi[column].type).memusage(tpi, column, structptr, index);
    return 0;
}

void nonarray_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    if (TYPE_INFO(tpi[column].type).calcoffset)
        TYPE_INFO(tpi[column].type).calcoffset(tpi, column, size);
}

void nonarray_copystruct(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData)
{
    if (TYPE_INFO(tpi[column].type).copystruct)
        TYPE_INFO(tpi[column].type).copystruct(tpi, column, dest, src, index, memAllocator, customData);
}

void nonarray_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    if (TYPE_INFO(tpi[column].type).copyfield)
        TYPE_INFO(tpi[column].type).copyfield(tpi, column, dest, src, index, memAllocator, customData, iOptionFlagsToMatch, iOptionFlagsToExclude);
}

void nonarray_endianswap(ParseTable tpi[], int column, void* structptr, int index)
{
    if (TYPE_INFO(tpi[column].type).endianswap)
        TYPE_INFO(tpi[column].type).endianswap(tpi, column, structptr, index);
}

void nonarray_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam)
{
    if (TYPE_INFO(tpi[column].type).interp)
        TYPE_INFO(tpi[column].type).interp(tpi, column, structA, structB, destStruct, index, interpParam);
}

void nonarray_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime)
{
    if (TYPE_INFO(tpi[column].type).calcrate)
        TYPE_INFO(tpi[column].type).calcrate(tpi, column, structA, structB, destStruct, index, deltaTime);
}

void nonarray_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime)
{
    if (TYPE_INFO(tpi[column].type).integrate)
        TYPE_INFO(tpi[column].type).integrate(tpi, column, valueStruct, rateStruct, destStruct, index, deltaTime);
}

void nonarray_calccyclic(ParseTable tpi[], int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, int index, F32 fStartTime, F32 deltaTime)
{
    if (TYPE_INFO(tpi[column].type).calccyclic)
        TYPE_INFO(tpi[column].type).calccyclic(tpi, column, valueStruct, ampStruct, freqStruct, cycleStruct, destStruct, index, fStartTime, deltaTime);
}

void nonarray_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed)
{
    if (TYPE_INFO(tpi[column].type).applydynop)
        TYPE_INFO(tpi[column].type).applydynop(tpi, column, dstStruct, srcStruct, index, optype, values, uiValuesSpecd, seed);
}

void fixedarray_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    // nothing to initialize for fixed arrays
}

void fixedarray_destroystruct(ParseTable tpi[], int column, void* structptr, int index)
{
    int i, numelems = tpi[column].param;
    for (i = numelems-1; i >= 0; i--)
    {
        nonarray_destroystruct(tpi, column, structptr, i);
    }
}

int fixedarray_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    int numelems = tpi[column].param;
    int i;
    int done = 0;
    int type = TOK_GET_TYPE(tpi[column].type);

    // these types get parsed differently
    if (type == TOK_QUATPYR_X ||    // parsed as pyr
        type == TOK_MATPYR_X ||        // parsed as pyr
        type == TOK_CONDRGB_X ||
        type == TOK_FLAGARRAY_X)
    {
        return nonarray_parse(tok, tpi, column, structptr, 0, callback);
    }

    for (i = 0; i < numelems; i++)
    {
        done = done || nonarray_parse(tok, tpi, column, structptr, i, callback);
    }
    return done;
}

void fixedarray_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int i, numelems = tpi[column].param;
    int type = TOK_GET_TYPE(tpi[column].type);

    // these types get parsed differently
    if (type == TOK_QUATPYR_X || // written as pyr
        type == TOK_MATPYR_X ||    // written as pyr
        type == TOK_CONDRGB_X)
    {
        nonarray_writetext(out, tpi, column, structptr, index, showname, level, iOptionFlagsToMatch, iOptionFlagsToExclude);
        return;
    }

    if (showname) WriteString(out, tpi[column].name, level+1, 0);
    for (i = 0; i < numelems; i++)
    {
        if (i > 0)
            WriteString(out, ",", 0, 0);
        WriteString(out, " ", 0, 0);
        nonarray_writetext(out, tpi, column, structptr, i, 0, 0, iOptionFlagsToMatch, iOptionFlagsToExclude);
    }
    if (showname) WriteString(out, "", 0, 1);
}

int fixedarray_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int ok = 1;
    int i, numelems = tpi[column].param;
    int type = TOK_GET_TYPE(tpi[column].type);

    if (type == TOK_MATPYR_X)    // persisted as pyr
        return nonarray_writebin(file, tpi, column, structptr, index, datasum, iOptionFlagsToMatch, iOptionFlagsToExclude);

    for (i = 0; i < numelems; i++)
        ok = ok && nonarray_writebin(file, tpi, column, structptr, i, datasum, iOptionFlagsToMatch, iOptionFlagsToExclude);
    return ok;
}

int fixedarray_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int ok = 1;
    int i, numelems = tpi[column].param;
    int type = TOK_GET_TYPE(tpi[column].type);

    if (type == TOK_MATPYR_X)    // persisted as pyr
        return nonarray_readbin(file, tpi, column, structptr, index, datasum);

    for (i = 0; i < numelems; i++)
        ok = ok && nonarray_readbin(file, tpi, column, structptr, i, datasum);
    return ok;
}

void fixedarray_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int i, numelems = tpi[column].param;
    int type = TOK_GET_TYPE(tpi[column].type);
    if (type == TOK_MATPYR_X) // send as pyr
    {
        nonarray_senddiff(pak, tpi, column, oldstruct, newstruct, index, sendAbsolute, forcePackAll, allowDiffs, iOptionFlagsToMatch, iOptionFlagsToExclude);
        return;
    }
    for (i = 0; i < numelems; i++)
        nonarray_senddiff(pak, tpi, column, oldstruct, newstruct, i, sendAbsolute, forcePackAll, allowDiffs, iOptionFlagsToMatch, iOptionFlagsToExclude);
}

void fixedarray_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    int i, numelems = tpi[column].param;
    int type = TOK_GET_TYPE(tpi[column].type);
    if (type == TOK_MATPYR_X) // send as pyr
    {
        nonarray_recvdiff(pak, tpi, column, structptr, index, absValues, pktidptr);
        return;
    }
    if (pktidptr && !*pktidptr)
    {
        *pktidptr = calloc(sizeof(void*)*numelems, 1);
    }
    for (i = 0; i < numelems; i++)
    {
        nonarray_recvdiff(pak, tpi, column, structptr, i, absValues, pktidptr? ((void**)*pktidptr)+i: NULL);
    }
}

void fixedarray_freepktids(ParseTable tpi[], int column, void** pktidptr)
{
    int i, numelems = tpi[column].param;

    if (!pktidptr || !*pktidptr) return;
    for (i = 0; i < numelems; i++)
    {
        nonarray_freepktids(tpi, column, ((void**)*pktidptr)+i);
    }
    SAFE_FREE(*pktidptr);
}

bool fixedarray_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    int i, len, numelems = tpi[column].param;
    str[0] = 0;
    for (i = 0; i < numelems; i++)
    {
        if (i > 0)
            strcat_s(str, str_size, ", ");
        len = (int)strlen(str);
        str_size -= len;
        str += len;
        if (str_size <= 1) 
            return false;
        if (!nonarray_tosimple(tpi, column, structptr, i, str, str_size, prettyprint))
            return false;
    }
    return true;
}

bool fixedarray_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    int i, numelems = tpi[column].param;
    char* param = calloc(strlen(str)+1, 1);
    char *next;
    char *strtokcontext = NULL;

    strcpy_s(param, strlen(str)+1, str);
    next = strtok_s(param, ",", &strtokcontext);
    for (i = 0; i < numelems && next; i++)
    {
        while (next[0] == ' ') next++;
        if (!next[0]) break;
        nonarray_fromsimple(tpi, column, structptr, i, next);
        next = strtok_s(NULL, ",", &strtokcontext);
    }
    free(param);
    return i == numelems;
}

void fixedarray_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    int i, numelems = tpi[column].param;
    for (i = 0; i < numelems; i++)
    {
        nonarray_updatecrc(tpi, column, structptr, i);
    }
}

int fixedarray_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    int i, numelems = tpi[column].param;
    int ret = 0;
    for (i = 0; i < numelems && ret == 0; i++)
    {
        ret = nonarray_compare(tpi, column, lhs, rhs, i);
    }
    return ret;
}

size_t fixedarray_memusage(ParseTable tpi[], int column, void* structptr, int index)
{
    size_t ret = 0;
    int i, numelems = tpi[column].param;
    for (i = 0; i < numelems; i++)
    {
        ret += nonarray_memusage(tpi, column, structptr, i);
    }
    return ret;
}

void fixedarray_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    int i, numelems = tpi[column].param;
    size_t firstelem;

    // first element has the correct offset (this code allows for mem alignment)
    nonarray_calcoffset(tpi, column, size);
    firstelem = tpi[column].storeoffset;

    for (i = 1; i < numelems; i++)
        nonarray_calcoffset(tpi, column, size);

    tpi[column].storeoffset = firstelem;
}

void fixedarray_copystruct(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData)
{
    int i, numelems = tpi[column].param;
    for (i = 0; i < numelems; i++)
        nonarray_copystruct(tpi, column, dest, src, i, memAllocator, customData);
}

void fixedarray_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int i, numelems = tpi[column].param;
    for (i = 0; i < numelems; i++)
        nonarray_copyfield(tpi, column, dest, src, i, memAllocator, customData, iOptionFlagsToMatch, iOptionFlagsToExclude);
}

void fixedarray_endianswap(ParseTable tpi[], int column, void* structptr, int index)
{
    int i, numelems = tpi[column].param;
    for (i = 0; i < numelems; i++)
        nonarray_endianswap(tpi, column, structptr, i);
}

void fixedarray_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam)
{
    int i, numelems = tpi[column].param;

    int type = TOK_GET_TYPE(tpi[column].type);
    if (type == TOK_MATPYR_X || type == TOK_QUATPYR_X) // custom interps
    {
        nonarray_interp(tpi, column, structA, structB, destStruct, index, interpParam);
        return;
    }

    for (i = 0; i < numelems; i++)
        nonarray_interp(tpi, column, structA, structB, destStruct, i, interpParam);
}

void fixedarray_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime)
{
    int i, numelems = tpi[column].param;

    int type = TOK_GET_TYPE(tpi[column].type);
    if (type == TOK_MATPYR_X || type == TOK_QUATPYR_X) // custom interps
    {
        nonarray_calcrate(tpi, column, structA, structB, destStruct, index, deltaTime);
        return;
    }

    for (i = 0; i < numelems; i++)
        nonarray_calcrate(tpi, column, structA, structB, destStruct, i, deltaTime);
}

void fixedarray_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime)
{
    int i, numelems = tpi[column].param;

    int type = TOK_GET_TYPE(tpi[column].type);
    if (type == TOK_MATPYR_X || type == TOK_QUATPYR_X) // custom interps
    {
        nonarray_integrate(tpi, column, valueStruct, rateStruct, destStruct, index, deltaTime);
        return;
    }

    for (i = 0; i < numelems; i++)
        nonarray_integrate(tpi, column, valueStruct, rateStruct, destStruct, i, deltaTime);
}

void fixedarray_calccyclic(ParseTable tpi[], int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, int index, F32 fStartTime, F32 deltaTime)
{
    int i, numelems = tpi[column].param;
    for (i = 0; i < numelems; i++)
        nonarray_calccyclic(tpi, column, valueStruct, ampStruct, freqStruct, cycleStruct, destStruct, i, fStartTime, deltaTime);
}

void fixedarray_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed)
{
    int i, numelems = tpi[column].param;
    int type = TOK_GET_TYPE(tpi[column].type);
    if (type == TOK_QUATPYR_X) // custom applydynop
    {
        nonarray_applydynop(tpi, column, dstStruct, srcStruct, 0, optype, values, uiValuesSpecd, seed);
        return;
    }
    else if (type == TOK_F32_X && numelems == 3)
    {
        vec3_applydynop(tpi, column, dstStruct, srcStruct, 0, optype, values, uiValuesSpecd, seed);
        return;
    }

    if (uiValuesSpecd >= numelems) // values spread to components // changed so there is only one seed
    {
        for (i = 0; i < numelems; i++)
            nonarray_applydynop(tpi, column, dstStruct, srcStruct, i, optype, values+i, uiValuesSpecd, seed);
    }
    else // same value & seed to each component
    {
        for (i = 0; i < numelems; i++)
            nonarray_applydynop(tpi, column, dstStruct, srcStruct, i, optype, values, uiValuesSpecd, seed);
    }
}

void earray_initstruct(ParseTable tpi[], int column, void* structptr, int index)
{
    // nothing to initialize for earrays
}

void earray_destroystruct(ParseTable tpi[], int column, void* structptr, int index)
{
    int i, numelems = TokenStoreGetNumElems(tpi, column, structptr);
    for (i = numelems-1; i >= 0; i--)
    {
        nonarray_destroystruct(tpi, column, structptr, i);
    }
    TokenStoreDestroyEArray(tpi, column, structptr);
}

int earray_parse(TokenizerHandle tok, ParseTable tpi[], int column, void* structptr, int index, ParserTextCallback callback)
{
    int i = TokenStoreGetNumElems(tpi, column, structptr); // earrays keep adding to the end of the list when parsing
    const char* nexttoken;
    int type = TOK_GET_TYPE(tpi[column].type);

    // these types get parsed differently
    if (type == TOK_STRUCT_X ||            // parsed one at a time
        type == TOK_FUNCTIONCALL_X ||    // parsed one at a time
        type == TOK_UNPARSED_X)            // parsed one at a time
        return nonarray_parse(tok, tpi, column, structptr, 0, callback);
    while (1)
    {
        nexttoken = TokenizerPeek(tok, 0, 1);
        if (!nexttoken || IsEol(nexttoken)) break;
        nonarray_parse(tok, tpi, column, structptr, i++, callback);
    }
    return 0;
}

void earray_writetext(FILE* out, ParseTable tpi[], int column, void* structptr, int index, bool showname, int level, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int s, size = TokenStoreGetNumElems(tpi, column, structptr);
    int type = TOK_GET_TYPE(tpi[column].type);

    // these types get parsed & written individually
    if (type == TOK_STRUCT_X ||
        type == TOK_FUNCTIONCALL_X ||
        type == TOK_UNPARSED_X)
    {
        nonarray_writetext(out, tpi, column, structptr, 0, showname, level, iOptionFlagsToMatch, iOptionFlagsToExclude);
        return;
    }

    if (size)
    {
        if (showname) WriteString(out, tpi[column].name, level+1, 0);
        for (s = 0; s < size; s++)
        {
            if (s > 0)
                WriteString(out, ",", 0, 0);
            WriteString(out, " ", 0, 0);
            nonarray_writetext(out, tpi, column, structptr, s, 0, 0, iOptionFlagsToMatch, iOptionFlagsToExclude);
        }
        if (showname) WriteString(out, "", 0, 1);
    }
}

int earray_writebin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int ok = 1;
    int s, size;
    int type = TOK_GET_TYPE(tpi[column].type);

    if (type == TOK_FUNCTIONCALL_X)
        return nonarray_writebin(file, tpi, column, structptr, 0, datasum, iOptionFlagsToMatch, iOptionFlagsToExclude);

    size = TokenStoreGetNumElems(tpi, column, structptr);
    ok = ok && SimpleBufWriteU32(size, file);
    *datasum += sizeof(int);
    for (s = 0; s < size; s++)
    {
        ok = ok && nonarray_writebin(file, tpi, column, structptr, s, datasum, iOptionFlagsToMatch, iOptionFlagsToExclude);
    }
    return ok;
}

int earray_readbin(SimpleBufHandle file, ParseTable tpi[], int column, void* structptr, int index, int* datasum)
{
    int ok = 1;
    int s, size = 0;
    int type = TOK_GET_TYPE(tpi[column].type);

    if (type == TOK_FUNCTIONCALL_X)
        return nonarray_readbin(file, tpi, column, structptr, 0, datasum);

    ok = ok && SimpleBufReadU32((U32*)&size, file);
    *datasum += sizeof(int);
    if (size > 0)
        TokenStoreSetCapacity(tpi, column, structptr, size);
    for (s = 0; s < size; s++)
    {
        ok = ok && nonarray_readbin(file, tpi, column, structptr, s, datasum);
    }
    return ok;
}

void earray_senddiff(Packet* pak, ParseTable tpi[], int column, void* oldstruct, void* newstruct, int index, bool sendAbsolute, bool forcePackAll, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int numold = oldstruct?TokenStoreGetNumElems(tpi, column, oldstruct):0;
    int numnew = TokenStoreGetNumElems(tpi, column, newstruct);
    int i;
    int type = TOK_GET_TYPE(tpi[column].type);

    if (type == TOK_FUNCTIONCALL_X)
    {
        nonarray_senddiff(pak, tpi, column, oldstruct, newstruct, index, sendAbsolute, forcePackAll, allowDiffs, iOptionFlagsToMatch, iOptionFlagsToExclude);
        return;
    }

    pktSendBitsAuto(pak, numnew);

    // if the size of the array has changed, refresh everything
    if (numold != numnew)
    {
        pktSendBits(pak, 1, 1); // full refresh
        for (i = 0; i < numnew; i++)
            nonarray_senddiff(pak, tpi, column, NULL, newstruct, i, true, true, false,  iOptionFlagsToMatch, iOptionFlagsToExclude);
    }
    else
    {
        pktSendBits(pak, 1, 0); // diffs allowed
        for (i = 0; i < numnew; i++)
            nonarray_senddiff(pak, tpi, column, oldstruct, newstruct, i, sendAbsolute, forcePackAll, allowDiffs,  iOptionFlagsToMatch, iOptionFlagsToExclude);
    }
}

void earray_recvdiff(Packet* pak, ParseTable tpi[], int column, void* structptr, int index, int absValues, void** pktidptr)
{
    int i, numnew, numold = structptr? TokenStoreGetNumElems(tpi, column, structptr): 1;
    int type = TOK_GET_TYPE(tpi[column].type);
    int sentabs;
    
    if (type == TOK_FUNCTIONCALL_X)
    {
        nonarray_recvdiff(pak, tpi, column, structptr, index, absValues, pktidptr);
        return;
    }

    numnew = pktGetBitsAuto(pak);
    sentabs = pktGetBits(pak, 1);
    absValues = absValues || sentabs; // full refresh

    // alloc pkt ids
    if (pktidptr && !*pktidptr)
        eaCreate((void***)pktidptr);
    if (pktidptr && (numnew != eaSizeUnsafe((void***)pktidptr)))
    {
        while (eaSizeUnsafe((void***)pktidptr) > numnew)
        {
            void* subpktid = eaPop((void***)pktidptr);
            nonarray_freepktids(tpi, column, &subpktid);
        }
        eaSetSize((void***)pktidptr, numnew);
    }

    // resize structs as necessary
    while (numold > numnew && structptr)
    {
        nonarray_destroystruct(tpi, column, structptr, --numold);
        TokenStoreSetEArraySize(tpi, column, structptr, numold);
    }
    // (growth will happen automatically)

    // finally recv each element
    for (i = 0; i < numnew; i++)
    {
        void** psubpktid = pktidptr? (*(void***)pktidptr + i): NULL;
        nonarray_recvdiff(pak, tpi, column, structptr, i, absValues, psubpktid);
    }
}

void earray_freepktids(ParseTable tpi[], int column, void** pktidptr)
{
    int i;
    if (!pktidptr || !*pktidptr) return;
    for (i = 0; i < eaSizeUnsafe((void***)pktidptr); i++)
    {
        nonarray_freepktids(tpi, column, (void**)eaGet((void***)pktidptr, i));
    }
    eaDestroy((void***)pktidptr);
}

bool earray_tosimple(ParseTable tpi[], int column, void* structptr, int index, char* str, int str_size, bool prettyprint)
{
    int i, len, numelems = TokenStoreGetNumElems(tpi, column, structptr);
    str[0] = 0;
    for (i = 0; i < numelems; i++)
    {
        if (i > 0)
            strcat_s(str, str_size, ", ");
        len = (int)strlen(str);
        str_size -= len;
        str += len;
        if (str_size <= 1) 
            return false;
        if (!nonarray_tosimple(tpi, column, structptr, i, str, str_size, prettyprint))
            return false;
    }
    return true;
}

bool earray_fromsimple(ParseTable tpi[], int column, void* structptr, int index, char* str)
{
    int numelems = TokenStoreGetNumElems(tpi, column, structptr);
    int e, i = 0;
    char* param = calloc(strlen(str)+1, 1);
    char *next;
    char *strtokcontext = NULL;

    strcpy_s(param, strlen(str)+1, str);
    next = strtok_s(str, ",", &strtokcontext);
    while (next && next[0])
    {
        while (next[0] == ' ') str++;
        if (!next[0]) break;
        nonarray_fromsimple(tpi, column, structptr, i, next);

        next = strtok_s(NULL, ",", &strtokcontext);
        i++;
    }
    free(param);

    // shorten array if necessary
    if (i < numelems)
    {
        for (e = numelems-1; e >= i; e--)
        {
            nonarray_destroystruct(tpi, column, structptr, e);
        }
        TokenStoreSetEArraySize(tpi, column, structptr, i);
    }

    return true;
}

void earray_updatecrc(ParseTable tpi[], int column, void* structptr, int index)
{
    int i, numelems = TokenStoreGetNumElems(tpi, column, structptr);
    for (i = 0; i < numelems; i++)
    {
        nonarray_updatecrc(tpi, column, structptr, i);
    }
}

int earray_compare(ParseTable tpi[], int column, void* lhs, void* rhs, int index)
{
    int numleft = TokenStoreGetNumElems(tpi, column, lhs);
    int numright = TokenStoreGetNumElems(tpi, column, rhs);
    int i, ret = 0;
    if (numleft != numright)
        return numleft - numright;
    for (i = 0; i < numleft && ret == 0; i++)
    {
        ret = nonarray_compare(tpi, column, lhs, rhs, i);
    }
    return ret;
}

size_t earray_memusage(ParseTable tpi[], int column, void* structptr, int index)
{
    size_t ret = TokenStoreGetEArrayMemUsage(tpi, column, structptr);
    int i, numelems = TokenStoreGetNumElems(tpi, column, structptr);
    for (i = 0; i < numelems; i++)
    {
        ret += nonarray_memusage(tpi, column, structptr, i);
    }
    return ret;
}

void earray_calcoffset(ParseTable tpi[], int column, size_t* size)
{
    // don't need to refer to subtype
    MEM_ALIGN(*size, sizeof(void*));
    tpi[column].storeoffset = *size;
    (*size) += sizeof(void*);
}

void earray_copystruct(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData)
{
    int i, numelems = TokenStoreGetNumElems(tpi, column, src);
    TokenStoreCopyEArray(tpi, column, dest, src, memAllocator, customData);
    for (i = 0; i < numelems; i++)
    {
        nonarray_copystruct(tpi, column, dest, src, i, memAllocator, customData);
    }
}

void earray_copyfield(ParseTable tpi[], int column, void* dest, void* src, int index, CustomMemoryAllocator memAllocator, void* customData, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    // small optimization if size of both fields the same
    int i, srcelems = TokenStoreGetNumElems(tpi, column, src);
    int destelems = TokenStoreGetNumElems(tpi, column, dest);

    // if not the same, destroy and recreate
    if (srcelems != destelems)
        earray_destroystruct(tpi, column, dest, index);

    for (i = 0; i < srcelems; i++)
        nonarray_copyfield(tpi, column, dest, src, i, memAllocator, customData, iOptionFlagsToMatch, iOptionFlagsToExclude);
}

void earray_endianswap(ParseTable tpi[], int column, void* structptr, int index)
{
    int i, numelems = TokenStoreGetNumElems(tpi, column, structptr);
    for (i = 0; i < numelems; i++)
        nonarray_endianswap(tpi, column, structptr, i);
}

void earray_interp(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 interpParam)
{
    int i;
    int numelemsA = TokenStoreGetNumElems(tpi, column, structA);
    int numelemsB = TokenStoreGetNumElems(tpi, column, structB);
    if (numelemsB < numelemsA) numelemsA = numelemsB; // use min
    for (i = 0; i < numelemsA; i++)
        nonarray_interp(tpi, column, structA, structB, destStruct, i, interpParam);
}

void earray_calcrate(ParseTable tpi[], int column, void* structA, void* structB, void* destStruct, int index, F32 deltaTime)
{
    int i;
    int numelemsA = TokenStoreGetNumElems(tpi, column, structA);
    int numelemsB = TokenStoreGetNumElems(tpi, column, structB);
    if (numelemsB < numelemsA) numelemsA = numelemsB; // use min
    for (i = 0; i < numelemsA; i++)
        nonarray_calcrate(tpi, column, structA, structB, destStruct, i, deltaTime);
}

void earray_integrate(ParseTable tpi[], int column, void* valueStruct, void* rateStruct, void* destStruct, int index, F32 deltaTime)
{
    int i;
    int numelemsA = TokenStoreGetNumElems(tpi, column, valueStruct);
    int numelemsB = TokenStoreGetNumElems(tpi, column, rateStruct);
    if (numelemsB < numelemsA) numelemsA = numelemsB; // use min
    for (i = 0; i < numelemsA; i++)
        nonarray_integrate(tpi, column, valueStruct, rateStruct, destStruct, i, deltaTime);
}

void earray_calccyclic(ParseTable tpi[], int column, void* valueStruct, void* ampStruct, void* freqStruct, void* cycleStruct, void* destStruct, int index, F32 fStartTime, F32 deltaTime)
{
    int i;
    int numelemsV = TokenStoreGetNumElems(tpi, column, valueStruct);
    int numelemsA = TokenStoreGetNumElems(tpi, column, ampStruct);
    int numelemsF = TokenStoreGetNumElems(tpi, column, freqStruct);
    int numelemsC = TokenStoreGetNumElems(tpi, column, cycleStruct);
    numelemsV = min(numelemsV, min(numelemsA, min(numelemsF, numelemsC)));
    for (i = 0; i < numelemsV; i++)
        nonarray_calccyclic(tpi, column, valueStruct, ampStruct, freqStruct, cycleStruct, destStruct, i, fStartTime, deltaTime);
}

void earray_applydynop(ParseTable tpi[], int column, void* dstStruct, void* srcStruct, int index, DynOpType optype, const F32* values, U8 uiValuesSpecd, U32* seed)
{
    int i, numelems;
    
    // for inherit, dstStruct may be empty, otherwise, dstStruct pretty much governs the operation
    if (optype == DynOpType_Inherit) 
        numelems = TokenStoreGetNumElems(tpi, column, srcStruct);
    else
        numelems = TokenStoreGetNumElems(tpi, column, dstStruct);

    if (uiValuesSpecd >= numelems) // values & seeds spread to components
    {
        for (i = 0; i < numelems; i++)
            nonarray_applydynop(tpi, column, dstStruct, srcStruct, i, optype, values+i, uiValuesSpecd, seed+i);
    }
    else // save value & seed to each component
    {
        for (i = 0; i < numelems; i++)
            nonarray_applydynop(tpi, column, dstStruct, srcStruct, i, optype, values, uiValuesSpecd, seed);
    }
}

int StructLinkValidName(char *name)
{
    return name && !strchr(name,'.');
}


void SimpleTokenSetSingle( char *pchVal, TokenizerParseInfo *tpi, int i, void * structptr, StaticDefineInt * pSDI )
{
    if( TOK_GET_TYPE(tpi[i].type) == TOK_STRING_X )
    {
        if( tpi[i].type & TOK_EARRAY )
            TokenStoreSetString(tpi, i, structptr, -1, pchVal, 0, 0 );
        else
            TokenStoreSetString(tpi, i, structptr, 0, pchVal, 0, 0 );
    }
    if (TOK_GET_TYPE(tpi[i].type) == TOK_INT_X)
    {
        int iVal = -1; atoi(pchVal);
        if( pSDI )
            iVal = StaticDefineIntGetInt(pSDI, pchVal);
        if( iVal < 0 )
            iVal = atoi(pchVal);
        TokenStoreSetInt(tpi, i, structptr, 0, iVal );
    }
    if (TOK_GET_TYPE(tpi[i].type) == TOK_F32_X)
        TokenStoreSetF32(tpi, i, structptr, 0, atof(pchVal) );

    if (TOK_GET_TYPE(tpi[i].type) == TOK_STRUCT_X)
        TokenStoreSetPointer(tpi, i, structptr, 0, (void*)pchVal );
}

void SimpleTokenStore( char * pchField, char *pchVal, TokenizerParseInfo *tpi, void * structptr, StaticDefineInt * pSDI )
{
    int i;
    FORALL_PARSEINFO( tpi, i)
    {
        if (!tpi[i].name || stricmp(tpi[i].name, pchField)!=0 )
            continue;

        SimpleTokenSetSingle( pchVal, tpi, i, structptr, pSDI );
    }
}

const char * SimpleTokenGetSingle( TokenizerParseInfo *tpi, int i, void * structptr, StaticDefineInt * pSDI )
{
    static char buffer[32];

    if( TOK_GET_TYPE(tpi[i].type) == TOK_STRING_X )
    {
        if( tpi[i].type & TOK_EARRAY )
        {
            // This is trickier, TokenStoreSetString(tpi, i, structptr, -1, pchVal, 0, 0 );
        }
        else
            return TokenStoreGetString(tpi, i, structptr, 0 );
    }
    if (TOK_GET_TYPE(tpi[i].type) ==  TOK_INT_X)
    {
        int iVal = TokenStoreGetInt(tpi, i, structptr, 0);
        if( pSDI )
        {    
            const char * str = StaticDefineIntRevLookup(pSDI, iVal);
            if(str)
                return str;
        }
        itoa( iVal, buffer, 10 );
        return buffer;
    }
    if (TOK_GET_TYPE(tpi[i].type) == TOK_F32_X)
    {     
        F32 fVal = TokenStoreGetF32(tpi, i, structptr, 0 );
        safe_ftoa( fVal, buffer );
        return buffer;
    }
    if (TOK_GET_TYPE(tpi[i].type) == TOK_STRUCT_X)
        return (char*)TokenStoreGetPointer(tpi, i, structptr, 0 );

    return 0;
}

const char *SimpleTokenGet( char * pchField, TokenizerParseInfo *tpi, void * structptr, StaticDefineInt * pSDI )
{
    int i;
    FORALL_PARSEINFO( tpi, i)
    {
        if (!tpi[i].name || stricmp(tpi[i].name, pchField)!=0 )
            continue;

        return SimpleTokenGetSingle(tpi, i, structptr, pSDI );
    }
    return 0;
}
