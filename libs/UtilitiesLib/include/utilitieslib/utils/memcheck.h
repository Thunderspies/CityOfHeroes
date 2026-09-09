#ifndef _MEMCHECK_H
#define _MEMCHECK_H

#if defined(_XBOX)
#    error "Use Cryptic 2 fork if you want to compile for XBox."
#endif

#if defined(_WIN32)
#    include <stdlib.h>
#    include "../crt.h"
#    include <stdio.h>
#    include <string.h>
#    include <direct.h>
#   include <malloc.h>
#endif

#if _CRTDBG_MAP_ALLOC
#    define MEM_DBG_PARMS ,char const* caller_fname,int line
#    define MEM_DBG_PARMS_CALL ,caller_fname,line
#    define MEM_DBG_PARMS_INIT ,__FILE__,__LINE__

#    define smalloc(x) _malloc_dbg((x),_NORMAL_BLOCK,caller_fname,line)
#    define scalloc(x,y) _calloc_dbg(x,y,_NORMAL_BLOCK,caller_fname,line)
#    define srealloc(x,y) _realloc_dbg(x,y,_NORMAL_BLOCK,caller_fname,line)

// remap to our timed small alloc functions
#    undef _malloc_dbg
#    undef _calloc_dbg
#    undef _realloc_dbg
#    undef _strdup_dbg
#    undef _free_dbg
#    define _malloc_dbg malloc_timed
#    define _calloc_dbg calloc_timed
#    define _realloc_dbg realloc_timed
#    define _strdup_dbg strdup_timed
#    define _free_dbg free_timed

#else
#    define MEM_DBG_PARMS
#    define MEM_DBG_PARMS_CALL
#    define MEM_DBG_PARMS_INIT

#    define smalloc malloc
#    define scalloc calloc
#    define srealloc realloc

#    if defined(_DEBUG)
#        pragma message("_DEBUG is defined but not _CRTDBG_MAP_ALLOC=1, memory allocations/frees will fail\r\n")
#        error "You must define _CRTDBG_MAP_ALLOC=1 in project settings, as well as Force Include memcheck.h";
#    endif

#endif

// Keep these aliases after crtdbg.h, which defines its own debug allocation macros.
// The underscored names still expand through the CRT and timed allocation mappings.
#undef strdup
#define strdup _strdup
#undef wcsdup
#define wcsdup _wcsdup
#undef getcwd
#define getcwd _getcwd

#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

#if _CRTDBG_MAP_ALLOC
    void* malloc_timed(size_t size, int blockType, const char *filename, int linenumber);
    void* calloc_timed(size_t num, size_t size, int blockType, const char *filename, int linenumber);
    void* realloc_timed(void *userData, size_t newSize, int blockType, const char *filename, int linenumber);
    char* strdup_timed(const char *str, int blockType, const char *filename, int linenumber);
    void free_timed(void *userData, int blockType);
    int smallAllocCheckMemory(void);
#    define MEMALLOC_NOT_SMALLOC 513 // one bigger than the biggest small alloc
#else
#    define smallAllocCheckMemory() (1)
#    define MEMALLOC_NOT_SMALLOC 0
#endif

void addmsg(char *cmd,int size,char *filename,int line,void *mem);
void *chk_malloc(int amt,char *filename,int line);
void *chk_physicalmalloc(int amt,int alignment,char *filename,int line);
void *chk_calloc(int amt,int amt2,char *filename,int line);
void *chk_realloc(void *orig,int amt,char *filename,int line);
void chk_free(void *mem,char *filename,int line);
void memChunkAdd(void *mem);
void memChunkCheck(void);
void goterr(void);
void memCheckInit(void);
void memCheckDumpAllocs(void);

// free for constants
static INLINEDBG void freeConst(const void * ptr) { free((void*)ptr); }

void heapInfoPrint(void); // Debug function (might help determine if we have Heap Fragmentation
void heapInfoLog(void);
void heapCompactAll(void);
int heapValidateAll(void);

#include "memtrack.h"

C_DECLARATIONS_END

// uhh... don't use these.
#define memAlloc malloc
#define    memRealloc realloc
#define memFree free

#endif
