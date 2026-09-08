#include "utilitieslib/components/MemoryPool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>
#include "utilitieslib/components/ArrayOld.h"
#include "utilitieslib/utils/MemoryMonitor.h"
#include "utilitieslib/utils/Breakpoint.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/qsortG.h"
#include "utilitieslib/UtilsNew/lock.h"


#ifdef FULLDEBUG
    #define MEMPOOL_DEBUG 1
#else
    #define MEMPOOL_DEBUG 0
#endif

#if defined(NDEBUG)
// Copied from #include <crtdbg.h>
// TODO: I have no idea why somebody would use _calloc_dbg directly but this is a topic for later
#define _calloc_dbg(c, s, t, f, l)      calloc(c, s)
#define _malloc_dbg(s, t, f, l)         malloc(s)
#endif

#define CHUNK_ALIGNMENT            (16)
#define CHUNK_ALIGNMENT_MASK    (CHUNK_ALIGNMENT - 1)

int mpSuperFreeDebugging=0; // Verify every free operation
int mpLightFreeDebugging=0; // Track allocations with the MemoryMonitor

typedef struct MemoryPoolNode MemoryPoolNode;
struct MemoryPoolNode{
    MemoryPoolNode* address;    // stores it's own address
    MemoryPoolNode* next;        // stores the next node's address
};

static struct MemoryPoolImp* memPoolList;

typedef struct MemoryPoolImp {
    struct {
        struct MemoryPoolImp*    next;
        struct MemoryPoolImp*    prev;
    } poolList;
    
    const char*                    name;
    const char*                    fileName;
    int                            fileLine;
    size_t                        structSize;
    size_t                        structCount;
    Array                        chunkTable;    // A list of memory chunks
    MemoryPoolNode*                freelist;    // points to the next free MemoryPoolNode
    size_t                        allocationCount; // Number of structs allocated.
    size_t                        chunkSize;
    size_t                        alignmentOffset;
    int                            compactFreq;        // How often (in mpFree calls) to attempt compacting the memory pool.  A value of 0 turns it off.
    int                            compactCounter;        // How many mpFree calls have occured since last attempt to compact
    float                        compactLimit;
    unsigned int                zeroMemory : 1;
    unsigned int                needsCompaction : 1;
}MemoryPoolImp;

static bool mpDelayedCompaction=false; // Delay memory pool compaction until mpCompactPools() is called
static MemoryPoolImp **mpNeedCompaction;
static LazyLock mpCritSect = {0};

void assertOnAllocError(const char* filename, int linenumber, const char* format, ...);

MemoryPoolMode mpGetMode(MemoryPoolImp* pool){
    MemoryPoolMode mode = TurnOffAllFeatures;
    if(pool->zeroMemory)
        mode |= ZERO_MEMORY_BIT;
    return mode;
}

void mpSetMode(MemoryPoolImp* pool, MemoryPoolMode mode)
{
    if(mode & ZERO_MEMORY_BIT)
        pool->zeroMemory = 1;
    else
        pool->zeroMemory = 0;
}

void mpGetCompactParams(MemoryPoolImp *pool, int *frequency, float *limit)
{
    *frequency = pool->compactFreq;
    *limit = pool->compactLimit;
}

void mpSetCompactParams(MemoryPoolImp *pool, int frequency, float limit)
{
    if (frequency < 0)
        frequency = 0;
    pool->compactFreq = frequency;
    pool->compactLimit = limit;
}

static void insertIntoPoolList(MemoryPool pool)
{
    MemoryPool cur;
    MemoryPool prev = NULL;

    lazyLock(&mpCritSect);

    cur = memPoolList;
    
    while(cur){
        if(cur->name && stricmp(cur->name, pool->name) > 0){
            break;
        }
        
        prev = cur;
        cur = cur->poolList.next;
    }
    
    if(prev){
        prev->poolList.next = pool;
        pool->poolList.prev = prev;
    }else{
        pool->poolList.prev = NULL;
        memPoolList = pool;
    }
    
    if(cur){
        cur->poolList.prev = pool;
    }
    
    pool->poolList.next = cur;
    lazyUnlock(&mpCritSect);
}

MemoryPool createMemoryPoolNamed(const char* name, const char* fileName, int fileLine)
{
    MemoryPool pool = calloc(1, sizeof(MemoryPoolImp));
    
    pool->name = name ? name : "*** UnnamedMemoryPool";
    pool->fileName = fileName;
    pool->fileLine = fileLine;
    
    insertIntoPoolList(pool);
    
    return pool;
}


/* Function mpInitMemoryChunk()
 *  Cuts up given memory chunk into the proper size for the given memory pool and
 *    adds the cut up pieces to the free-list in the memory pool.
 *
 *    Note that this function does *not* add the memory chunk to the memory pool's
 *    memory chunk table.  Remember to do this or else the memory chunk will be leaked
 *    when the pool is destroyed.
 *
 */
static void mpInitMemoryChunk(MemoryPoolImp* pool, void* memoryChunk){
    MemoryPoolNode* frontNode;
    MemoryPoolNode* backNode;
    MemoryPoolNode* lastNode;
    size_t structSize = pool->structSize;
    size_t structCount = pool->structCount;
    
    // Cannot add a memory chunk to a memory pool if there is no pool.
    assert(pool);
    
    // At least 8 bytes are needed to store some integrity check value + a next element link.
    // MS: Uh, no, I'll just bump up my struct size when initialized.
    //assert(structSize >= sizeof(MemoryPoolNode));
    
    // Build a linked list of free MemoryPool nodes in their array element order by
    // constructing the list in reverse order.
    //    Get the the beginning of the last structure (allocation unit) in the array and use it as the beginning
    //    of a MemoryPool node.
    //        Cast memory to char* to perform arithmetic in unit of 1 byte instead of 4 bytes for
    //        normal pointers.
    lastNode = frontNode = backNode = (MemoryPoolNode*)((char*)memoryChunk + structSize * (structCount - 1));
    
    //    Initialize the last element.
    backNode->address = backNode;
    backNode->next = NULL;
    
    //  Keep constructing memory pool nodes in the reverse order until we've reached where
    //  "memory" is pointing.
    while((void*)backNode > memoryChunk){
        // Deduce the starting address of the structure that's right before the one being
        // held by the back node.  Call it the front node.
        frontNode = (MemoryPoolNode*)((char*)backNode - structSize);
        
        // Initialize the front node
        frontNode->address = frontNode;
        frontNode->next = backNode;
        
        // The current front node becomes the back node for the next iteration.
        backNode = frontNode;
    }
    
    // Add the linked list of free MemoryPool nodes to the memory pool
    //    If the memory pool is not completely empty, add the new elements to the front of the
    //    linked list.  The only reason the list is added to the front instead of the back of the
    //    list is because it is easier to get to the front of the existing freelist.
    if(pool->freelist){
        lastNode->next = pool->freelist;
    }
    
    pool->freelist = frontNode;
}



static char* mpGetAlignedAddress(MemoryPoolImp* pool, char* address){
    intptr_t alignmentOffset = (intptr_t)address & CHUNK_ALIGNMENT_MASK;
    
    assert(!((intptr_t)address & 3));
    
    if(alignmentOffset){
        address += CHUNK_ALIGNMENT - alignmentOffset;
    }
    
    if(pool->alignmentOffset){
        address += CHUNK_ALIGNMENT - pool->alignmentOffset;
    }
    
    return address;
}

/* Function mpAddMemoryChunk()
 *    Adds a new chunk of memory into the memory pool based on the existing structSize and structCount
 *    settings.  The memory is then initialized then added to the pool's memory chunk table.
 *
 *
 */
static void mpAddMemoryChunk_dbg(MemoryPoolImp* pool, const char* callerName, int line){
    char* memoryChunk;
    
    PERFINFO_AUTO_START("mpAddMemoryChunk_dbg", 1);
        // Cannot add a memory chunk to a memory pool if there is no pool.
        assert(pool);
        
        // Create the memory chunk to be added.
        if (pool->zeroMemory)
            memoryChunk = _calloc_dbg(pool->chunkSize, 1, _NORMAL_BLOCK, callerName, line);
        else
            memoryChunk = _malloc_dbg(pool->chunkSize, _NORMAL_BLOCK, callerName, line);
        if (!memoryChunk) {
            assertOnAllocError(callerName, line, "memory pool could not grow by %d bytes", pool->chunkSize);
        }

        mpInitMemoryChunk(pool, mpGetAlignedAddress(pool, memoryChunk));

        // Add the memory chunk to a list in the pool.
        arrayPushBack(&pool->chunkTable, memoryChunk);
    PERFINFO_AUTO_STOP();
}

static void mpAddMemoryChunk(MemoryPoolImp* pool){
    char* memoryChunk;

    PERFINFO_AUTO_START("mpAddMemoryChunk", 1);
        // Cannot add a memory chunk to a memory pool if there is no pool.
        assert(pool);
    
        // Create the memory chunk to be added.
        if (pool->zeroMemory)
            memoryChunk = calloc(pool->chunkSize, 1);
        else
            memoryChunk = malloc(pool->chunkSize);

        mpInitMemoryChunk(pool, mpGetAlignedAddress(pool, memoryChunk));

        // Add the memory chunk to a list in the pool.
        arrayPushBack(&pool->chunkTable, memoryChunk);
    PERFINFO_AUTO_STOP();
}

/* Function initMemoryPool()
 *    Initializes the given memory pool.
 *
 *    Parameters:
 *        pool - The memory pool to be initialized.
 *        structSize - The size of an "allocation unit".  A piece of memory of this size will be returned everytime
 *                     mpAlloc() is called.
 *        structCount - The number of "allocation units" to put in the pool initially.  Together with structSize,
 *                      this number also dictates the amount of "allocation units" to grow by when the pool runs
 *                      dry.
 *
 *
 */
void initMemoryPoolOffset(MemoryPoolImp* pool, int structSize, int structCount, int offset)
{
    assert(!pool->structSize && !pool->structCount);
    assert(structSize && structCount);
    pool->structSize = max(structSize, sizeof(MemoryPoolNode));
    pool->structCount = structCount;
    assert(offset < CHUNK_ALIGNMENT);
    pool->chunkSize = pool->structCount * pool->structSize + CHUNK_ALIGNMENT - 4 + (offset ? CHUNK_ALIGNMENT : 0);
    pool->alignmentOffset = offset;
    mpSetMode(pool, Default);
}

MemoryPool initMemoryPoolLazy_dbg(MemoryPool pool, int structSize, int structCount, const char* callerName, int line)
{
    PERFINFO_AUTO_START("initMemoryPoolLazy_dbg", 1);
        if(!pool)
        {
            pool = createMemoryPoolNamed(NULL, callerName, line);
            initMemoryPool(pool, structSize, structCount);
        }
        else
        {
            mpFreeAll(pool);
        }
    PERFINFO_AUTO_STOP();
    return pool;
}

static void removeFromPoolList(MemoryPool pool)
{
    if(mpDelayedCompaction)
    {
        //if in compact list, remove it!
        lazyLock(&mpCritSect);
        eaFindAndRemoveFast(&mpNeedCompaction, pool);
        lazyUnlock(&mpCritSect);
    }

    if(pool->poolList.prev)
        pool->poolList.prev->poolList.next = pool->poolList.next;
    else
        memPoolList = pool->poolList.next;

    if(pool->poolList.next)
        pool->poolList.next->poolList.prev = pool->poolList.prev;
}


/* Function destroyMemoryPool()
 *    Frees all memory allocated by memory pool, including the pool itself.
 *
 *    After this call, all memory allocated out of the given pool is invalidated.
 *
 */
void destroyMemoryPool(MemoryPoolImp* pool){
    if(!pool){
        return;
    }
    
    PERFINFO_AUTO_START("destroyMemoryPool", 1);
        removeFromPoolList(pool);
        destroyArrayPartialEx(&pool->chunkTable, NULL); // BR - changed destroyArrayEx to destroyArrayPartialEx
        free(pool);
    PERFINFO_AUTO_STOP();
}

/* Special for the gfxtree for debug.  sets every node to -1 before freeing
*/
void destroyMemoryPoolGfxNode(MemoryPoolImp* pool){
    int i;
    removeFromPoolList(pool);
    for(i = 0 ; i < pool->chunkTable.size; i++){
        memset(mpGetAlignedAddress(pool, pool->chunkTable.storage[i]), -1, pool->structSize * pool->structCount);
    }
    destroyArrayPartialEx(&pool->chunkTable, NULL); // BR - changed destroyArrayEx to destroyArrayPartialEx
    free(pool);
}

static INLINEDBG int isInChunk(MemoryPoolImp *pool, char *memory, int chunkIndex)
{
    char *chunkmem = pool->chunkTable.storage[chunkIndex];
    return memory >= chunkmem && memory < (chunkmem + pool->chunkSize);
}

static void **mpStuffToFree=NULL; // Linked list of data to be freed.  Cannot use an EArray or anything else that allocates memory!
static void compactMemoryPoolFreeFunc(void *data)
{
    if (mpDelayedCompaction) {
        void **newhead = (void**)data;
        *newhead = mpStuffToFree;
        mpStuffToFree = newhead;
    } else {
        free(data);
    }
}
static void compactMemoryPoolDoFreeing(void)
{
    void **next = NULL;
    while (mpStuffToFree) {
        next = *mpStuffToFree;
        free(mpStuffToFree);
        mpStuffToFree = next;
    }
}

// JE: Made static, since if it is called outside of this file, it needs to check some critical sections!
static void compactMemoryPool(MemoryPoolImp *pool)
{
    static MemoryPoolNode **nodelist = 0;
    int i, j, size = pool->chunkTable.size, needcompacting = 0;
    unsigned int *chunkCounts;
    MemoryPoolNode *node;

    PERFINFO_AUTO_START("compactMemoryPool", 1);

    chunkCounts = _alloca(size * sizeof(int));
    ZeroStructs(chunkCounts, size);

    if(!nodelist)
        eaCreateSmallocSafe(&nodelist);
    eaSetSize(&nodelist, 0);

    // create a flat list of free nodes, count the free nodes in each chunk
    for (node = pool->freelist; node; )
    {
        for (i = 0; i < size; i++)
        {
            if (isInChunk(pool, (char *)node, i))
            {
                chunkCounts[i]++;
                if (chunkCounts[i] >= pool->structCount)
                    needcompacting = 1;
                break;
            }
        }
        if(i != size) {
            eaPush(&nodelist, node);
        } else {
            devassertmsg(0, "Found the pointer %p in the freelist for the memory pool '%s', but it does not below to that memory pool", node, pool->name);
        }
        node = node->next;
    }

    if (!needcompacting)
    {
        pool->needsCompaction = 0;
        PERFINFO_AUTO_STOP();
        return;
    }

    PERFINFO_AUTO_START("compactMemoryPool:compacting", 1);

    // remove nodes from the freelist that are in completely empty chunks
    for (j = 0; j < eaSize(&nodelist); j++)
    {
        node = nodelist[j];
        for (i = 0; i < size; i++)
        {
            if (isInChunk(pool, (char *)node, i))
            {
                if(chunkCounts[i] >= pool->structCount) {
                    eaRemoveFast(&nodelist, j);
                    j--;
                }
                break;
            }
        }
        // should never happen because we handle it in the previous loop
        assert(i != size);
    }

    // free completely empty chunks
    for (i = 0, j = 0; i < size; i++)
    {
        if (chunkCounts[i] >= pool->structCount)
        {
            compactMemoryPoolFreeFunc(pool->chunkTable.storage[j]);
            arrayRemoveAndShift(&pool->chunkTable, j);
        }
        else
        {
            j++;
        }
    }

    // fixup freelist
    for (i = 1; i < eaSize(&nodelist); i++)
        nodelist[i-1]->next = nodelist[i];

    if (eaSize(&nodelist))
    {
        pool->freelist = nodelist[0];
        nodelist[eaSize(&nodelist)-1]->next = NULL;
    }
    else
    {
        pool->freelist = NULL;
    }

    pool->needsCompaction = 0;

    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_STOP();
}

static void compactMemoryPoolAdapter(void* arg0)
{
    compactMemoryPool((MemoryPoolImp *)arg0);
}

void mpCompactPools(void)
{
    PERFINFO_AUTO_START("mpCompactPools", 1);
    mpDelayedCompaction = true;
    mmCRTHeapLock(); // Because we do heap operations in here, we must enter the CRT heap before the memory pool critical section
    lazyLock(&mpCritSect);
    eaClearEx(&mpNeedCompaction, compactMemoryPoolAdapter);
    lazyUnlock(&mpCritSect);
    mmCRTHeapUnlock();
    compactMemoryPoolDoFreeing(); // Must free *outside* of critical section
    PERFINFO_AUTO_STOP();
}

// JE: Made static, since if it is called outside of this file, it needs to check some critical sections!
static void compactMemoryPoolIfUnder(MemoryPoolImp *pool, float limit)
{
    float currUtilization;
    if (pool->chunkTable.size < 3)
        return;

    currUtilization = ((float)pool->allocationCount) / (pool->chunkTable.size * pool->structCount);
    if(currUtilization < limit)
    {
        if(mpDelayedCompaction)
        {
            // If doing delayed compaction, simply add to another list to get compacted later
            mmCRTHeapLock(); // Because we do heap operations in here, we must enter the CRT heap before the memory pool critical section
            lazyLock(&mpCritSect);
            pool->needsCompaction = 1;
            if(!mpNeedCompaction)
                eaCreateSmallocSafe(&mpNeedCompaction);
            eaPush(&mpNeedCompaction, pool);
            lazyUnlock(&mpCritSect);
            mmCRTHeapUnlock();
        }
        else
        {
            // Compact now!
            compactMemoryPool(pool);
        }
    }
}

/* Function mpAlloc
 *    Allocates a piece of memory from the memory pool.  The size of the allocation will
 *    match the structSize given to the memory pool during initialization (initMemoryPool).
 *    
 *    Returns:
 *        NULL - cannot allocate memory because the memory pool ran dry and
 *               it is not possible to get more memory.
 *        otherwise - valid memory pointer of size mpStructSize(pool)
 */
void *mpAlloc_nolock(MemoryPoolImp *pool, const char *callerName, int line)
{
    void *memory;

    // If there is no memory pool, no memory can be allocated.
    assert(pool);

    // If there are no more free memory left in the pool, try to
    // allocate some more memory for the pool.
    if(!pool->freelist)
        mpAddMemoryChunk_dbg(pool, callerName, line);
    else
        assert(pool->chunkTable.size);

    memory = pool->freelist;

    // If we successfully got something from the freelist...
    if(memory)
    {
        void *addr = ((MemoryPoolNode*)memory)->address;

        // Make sure other parts of the program have been playing nice
        // with the memory allocated from the mem pool.
        assert(addr == memory);

        // Remove the allocated node from the freelist by
        // advancing the freelist pointer to the next node.
        pool->freelist = pool->freelist->next;

        // Initialize the returned memory to zero
        if(pool->zeroMemory)
        {
            memset(memory, 0, pool->structSize);
        }
        else
        {
#if MEMPOOL_DEBUG
            memset(memory, 0xfb, pool->structSize);
#endif

            ((MemoryPoolNode*)memory)->address = 0;
            ((MemoryPoolNode*)memory)->next = 0;
        }

        pool->allocationCount++;

        assert(pool->allocationCount > 0);
    }

    if(mpLightFreeDebugging && memMonitorActive())
    {
        char temp[260];
        strcpy(temp, callerName);
        strcat(temp, "-MP");
        memMonitorTrackUserMemory(temp, 0, MOT_ALLOC, pool->structSize);
    }

    if(pool->compactCounter > 0 && pool->compactFreq && !pool->needsCompaction)
        pool->compactCounter--;

    return memory;
}

void* mpAlloc_dbg(MemoryPoolImp* pool, const char* callerName, int line)
{
    void *memory;
    assertmsg(pool, "Memory pool is not initialized.");

    // If in need of compaction, enter critical section so we don't alloc while a pool is being compacted
    if(pool->needsCompaction)
    {
        PERFINFO_AUTO_START("mpAlloc_dbg (lock)", 1);
        mmCRTHeapLock(); // Because we do heap operations in here, we must enter the CRT heap before the memory pool critical section
        lazyLock(&mpCritSect);
        memory = mpAlloc_nolock(pool, callerName, line);
        lazyUnlock(&mpCritSect);
        mmCRTHeapUnlock();
        PERFINFO_AUTO_STOP();
    }
    else
    {
        PERFINFO_AUTO_START("mpAlloc_dbg (nolock)", 1);
        memory = mpAlloc_nolock(pool, callerName, line);
        PERFINFO_AUTO_STOP();
    }

    memtrack_alloc(memory,pool->structSize);

    return memory;
}

/* Function mpFree
*    Returns a piece of memory back into the pool.
*    
*    Parameters:
*        pool - a valid memory pool
*        memory - the piece of memory to return to the pool.  The size
*                 is implied by mpStructSize() of the given pool.
*
*    Returns:
*        1 - memory returned successfully
*        0 - some error encountered while returning memory
*
*    Warning:
*        This function does not know anything about the incoming memory chunk.  
*        Currently, it will accept any non-zero value and add it back into
*        the memory pool happily.  This will most likely cause an invalid memory
*        access somewhere down the line.  Since it will be hard to track the
*        source of this kind of bug, it will be a good idea to put some error
*        checking here, even a bad one.
*
*
*/
static int mpFreeInternal(MemoryPoolImp* pool, void* memory){

    MemoryPoolNode* node;
    
    PERFINFO_AUTO_START("mpFree", 1);
        assert(pool);

        // Take the memory and overlay a memory pool node on top of it.
        node = memory;

        // Insert the node as the head of the freelist.
        node->address = node;
        node->next = pool->freelist;
        pool->freelist = node;

        assert(pool->allocationCount > 0);

        pool->allocationCount--;
    PERFINFO_AUTO_STOP();
    
    return 1;
}

void mpFree_nolock(MemoryPoolImp* pool, void* memory, const char *callerName, int line)
{
    if(mpSuperFreeDebugging)
    {
        int i;
        int good;
        size_t freelist_size=0;
        size_t total_allocs = pool->structCount*pool->chunkTable.size;
        size_t expected_free = total_allocs - pool->allocationCount;

        // Search to see if it's already in the freeList
        MemoryPoolNode *node = pool->freelist;
        while (node)
        {
            freelist_size++;
            assert(node->address==node);
            if (memory==node)
                assert(!"This has already been freed!");
            node = node->next;
        }

        // Check to see if numbers match up
        assert(expected_free==freelist_size);

        // Search to see if it's in a valid range of addresses
        good=0;
        for (i=0; i<pool->chunkTable.size; i++)
        {
            char* chunk = mpGetAlignedAddress(pool, pool->chunkTable.storage[i]);
            if ((char*)memory >= chunk && ((char*)memory < chunk + pool->structCount*pool->structSize))
                good = 1;
        }

        if (!good)
            assert(!"Trying to free a chunk of memory that is not part of this pool!");
    }

    mpFreeInternal(pool, memory);

#if MEMPOOL_DEBUG
    {
        char *buf = (char*)memory + sizeof(MemoryPoolNode);
        size_t buf_len = pool->structSize - sizeof(MemoryPoolNode);
        memset(buf, 0xfa, buf_len);
        if(mpLightFreeDebugging) // Store the line number and caller of who freed this
            snprintf_s(buf, buf_len, _TRUNCATE, "%d:%s", line, callerName);
    }
#endif

    if(mpLightFreeDebugging && memMonitorActive())
    {
        char temp[260];
        strcpy(temp, callerName);
        strcat(temp, "-MP");
        memMonitorTrackUserMemory(temp, 0, MOT_FREE, pool->structSize);
    }

    if(pool->compactFreq && !pool->needsCompaction)
    {
        pool->compactCounter++;
        if(pool->compactCounter >= pool->compactFreq)
        {
            compactMemoryPoolIfUnder(pool, pool->compactLimit);
            pool->compactCounter = 0;
        }
    }
}

void mpFree_dbg(MemoryPoolImp* pool, void* memory, const char *callerName, int line)
{
    assertmsg(pool, "Memory pool is not initialized.");

    if(memory) // i don't know why we are ignoring this -GG
    {
        memtrack_free(memory);

        // If in need of compaction, enter critical section
        if (pool->needsCompaction)
        {
            PERFINFO_AUTO_START("mpFree_dbg (lock)", 1);
            mmCRTHeapLock(); // Because we do heap operations in here, we must enter the CRT heap before the memory pool critical section
            lazyLock(&mpCritSect);
            mpFree_nolock(pool, memory, callerName, line);
            lazyUnlock(&mpCritSect);
            mmCRTHeapUnlock();
            PERFINFO_AUTO_STOP();
        }
        else
        {
            PERFINFO_AUTO_START("mpFree_dbg (nolock)", 1);
            mpFree_nolock(pool, memory, callerName, line);
            PERFINFO_AUTO_STOP();
        }
    }
}

int mpIsValidPtr(MemoryPoolImp *pool, void *ptr)
{
    int i;
    for (i=0; i<pool->chunkTable.size; i++) {
        if (ptr >= pool->chunkTable.storage[i] && (char*)ptr < (char*)pool->chunkTable.storage[i] + pool->chunkSize) {
            return 1;
        }
    }
    return 0;
}

int mpVerifyFreelist(MemoryPoolImp* pool)
{
    size_t freeCount=0;
    size_t expectedFreeCount;
    int retry_count=5; // If we find an error, retry a few times, in case it's simply a threading issue
    bool bRetry=false;

    if (!pool)
        return 1;

    // Make sure each node in the memory pool still looks valid.
    //    Each node is supposed to store its own address, then the next pointer.
    //    Use this information to walk and verify the node.

    do {
        MemoryPoolNode* fastCursor;
        MemoryPoolNode* next;

        expectedFreeCount = pool->structCount * pool->chunkTable.size -  pool->allocationCount;

        bRetry = false;

        fastCursor = pool->freelist;

        while(fastCursor)
        {
#define POOL_MSG_START "%s:%s" 
#define POOL_ARGS pool->name,pool->fileName
#define ReportError(fmt, ...)                                \
                if (retry_count == 0) {                        \
                    OutputDebugStringf(fmt, __VA_ARGS__);    \
                    return 0;                                \
                } else {                                    \
                    /* Might be a problem in another thread, sleep, try again! */ \
                    retry_count--;                            \
                    Sleep(1);                                \
                    bRetry = true;                            \
                    break;                                    \
                }

            // Check self
            if(fastCursor->address != fastCursor) {
                ReportError(POOL_MSG_START, "%s:%s Freelist has a corrupt entry at 0x%x.\n", POOL_ARGS, fastCursor);
            }

            // Check if the next pointer is valid
            next = fastCursor->next; // Save it off so it doesn't change between the test and assignment
            if (next && !mpIsValidPtr(pool, next)) {
                ReportError(POOL_MSG_START "Freelist has a corrupt entry (bad next pointer) at 0x%08X.\n", POOL_ARGS,  fastCursor);
            }
            fastCursor = next;
            freeCount++;
            if (freeCount > expectedFreeCount) {
                ReportError(POOL_MSG_START "Freelist has either a loop or too many entries.\n", POOL_ARGS,  fastCursor);
            }
        }
        if (pool->structCount * pool->chunkTable.size -  pool->allocationCount != freeCount) {
            ReportError(POOL_MSG_START "Pool has too many elements in its freelist. %i\n", POOL_ARGS, freeCount);
        }
    } while (bRetry);
    return 1;
}

int mpFindMemory(MemoryPoolImp* pool, void* memory){
    MemoryPoolNode* node;

    if (!pool)
        return 0;

    // Make sure each node in the memory pool still looks valid.
    //    Each node is supposed to store its own address, then the next pointer.
    //    Use this information to walk and verify the node.
    for(node = pool->freelist; node; node = node->next){
        if(node == memory){
            return 1;
        }
    }

    return 0;
}

/* Function mpFreeAll
 *    Return all memory controlled by the memory pool back into the pool.  This effectively
 *    destroys/invalidates all structure held by the memory pool.
 *
 *    Note that this function does not actually free any of the memory that is currently
 *    held by the pool.
 *
 */
void mpFreeAll(MemoryPoolImp* pool){
    int i;

    if(!pool)
        return;

    PERFINFO_AUTO_START("mpFreeAll", 1);
        if (pool->needsCompaction)
        {
            // If in need of compaction, remove it from the list, not needed anymore
            lazyLock(&mpCritSect);
            pool->needsCompaction = 0;
            eaFindAndRemoveFast(&mpNeedCompaction, pool);
            lazyUnlock(&mpCritSect);
        }

        pool->freelist = NULL;
        for(i = pool->chunkTable.size - 1; i >= 0; i--)
            mpInitMemoryChunk(pool, mpGetAlignedAddress(pool, pool->chunkTable.storage[i]));
        pool->allocationCount = 0;

    PERFINFO_AUTO_STOP();

}

void mpFreeAllocatedMemory(MemoryPoolImp* pool)
{
    if(pool)
    {
        // If in need of compaction, enter critical section
        if(pool->needsCompaction)
        {
            PERFINFO_AUTO_START("mpFreeAllocatedMemory (lock)", 1);
            mmCRTHeapLock(); // Because we do heap operations in here, we must enter the CRT heap before the memory pool critical section
            lazyLock(&mpCritSect);

            pool->freelist = NULL;
            destroyArrayPartialEx(&pool->chunkTable, NULL);
            pool->allocationCount = 0;
            // ditto below

            lazyUnlock(&mpCritSect);
            mmCRTHeapUnlock();
            PERFINFO_AUTO_STOP();
        }
        else
        {
            PERFINFO_AUTO_START("mpFreeAllocatedMemory (nolock)", 1);

            // ditto above
            pool->freelist = NULL;
            destroyArrayPartialEx(&pool->chunkTable, NULL);
            pool->allocationCount = 0;

            PERFINFO_AUTO_STOP();
        }
    }
}

size_t mpStructSize(MemoryPoolImp* pool){
    if(!pool)
        return 0;

    return pool->structSize;
}

size_t mpGetAllocatedCount(MemoryPoolImp* pool){
    if(!pool)
        return 0;
        
    return pool->allocationCount;
}

size_t mpGetAllocatedChunkMemory(MemoryPoolImp* pool){
    if(!pool)
        return 0;
        
    return pool->structSize * pool->structCount * pool->chunkTable.size;
}

const char* mpGetName(MemoryPool pool){
    return pool->name;
}

const char* mpGetFileName(MemoryPool pool){
    return pool->fileName;
}

int    mpGetFileLine(MemoryPool pool){
    return pool->fileLine;
}

void mpForEachMemoryPool(MemoryPoolForEachFunc callbackFunc, void *userData){
    MemoryPool cur;
    
    for(cur = memPoolList; cur; cur = cur->poolList.next){
        callbackFunc(cur, userData);
    }
}

/* Function mpReclaimed()
 *    Answers if a piece of memory has been reclaimed by a memory pool.  Note that this function
 *    is not fool-proof.  It does not know if the given memory is allocated out of a memory pool.
 *    This function merely checks if the piece of memory holds a valid integrity value.  It will 
 *    fail when a piece of memory that is not reclaimed just happens to be storing the expected 
 *    integrity value at the expected location.
 *
 *    Returns:
 *        0 - memory not reclaimed.
 *        1 - memory reclaimed.
 */
int mpReclaimed(void* memory){
    MemoryPoolNode* node;
    assert(memory);

    node = memory;
    if(node->address == node)
        return 1;
    else
        return 0;
}


void testMemoryPool(){
    MemoryPool pool;
    void* memory;

    pool = createMemoryPool();
    initMemoryPool(pool, 16, 16);


    printf("MemoryPool created\n");
    printf("Validating memory pool\n");
    if(mpVerifyFreelist(pool)){
        printf("The memory pool is valid\n");
    }else
        printf("The memory pool has been corrupted\n");

    memory = mpAlloc_dbg(pool, __FILE__, __LINE__);

    printf("\n");
    printf("Memory allocated\n");
    printf("Validating memory pool\n");
    if(mpVerifyFreelist(pool)){
        printf("The memory pool is valid\n");
    }else
        printf("The memory pool has been corrupted\n");

    mpFree_dbg(pool, memory, __FILE__, __LINE__);
    printf("\n");
    printf("Memory freed\n");
    printf("Validating memory pool\n");
    if(mpVerifyFreelist(pool)){
        printf("The memory pool is valid\n");
    }else
        printf("The memory pool has been corrupted\n");

    mpFree_dbg(pool, memory, __FILE__, __LINE__);
    printf("\n");
    printf("Memory freed again\n");
    printf("Validating memory pool\n");
    if(mpVerifyFreelist(pool)){
        printf("The memory pool is valid\n");
    }else
        printf("The memory pool has been corrupted\n");

}

void mpEnableSuperFreeDebugging()
{
    mpSuperFreeDebugging = 1;
}


static void verifyFreelistCallback(MemoryPool mempool, void *userData)
{
    int *verify_freelists_ret = userData;
    int ret = mpVerifyFreelist(mempool);

    if (!ret) {
        OutputDebugStringf("MemPool %s has corrupt freelist.\n", mempool->name);
    }
    (*verify_freelists_ret) &= ret;
}

int mpVerifyAllFreelists(void)
{
    int verify_freelists_ret = 1;
    mpForEachMemoryPool(verifyFreelistCallback, &verify_freelists_ret);
    return verify_freelists_ret;
}

static int __cdecl cmpPtr(const void* aData, const void* bData)
{
    const void ** a = (const void **)aData;
    const void** b = (const void**)bData;
    return *a > *b ? 1 : *b > *a ? -1 : 0;
}


void mpForEachAllocation(MemoryPool pool, MemoryPoolForEachAllocationFunc func, void *userData)
{
    // CANNOT do small ( < 1K ) memory allocations while in this function!
    MemoryPoolNode **list;
    MemoryPoolNode *walk;
    int freelist_size=0;
    bool fromHeap;

    if (!pool->chunkTable.size)
        return;

    // Sort the freelist (build a temporary array)
    walk = pool->freelist;
    while (walk) {
        freelist_size++;
        walk = walk->next;
    }
    freelist_size *= sizeof(void*);
    if (freelist_size > 1024) {
        fromHeap = true;
        list = malloc(freelist_size);
    } else {
        fromHeap = false;
        list = _alloca(freelist_size);
    }
    walk = pool->freelist;
    freelist_size=0;
    while (walk) {
        list[freelist_size++] = walk;
        walk = walk->next;
    }
    qsortG(list, freelist_size, sizeof(void*), cmpPtr);

    // Sort the chunk table
    qsortG(pool->chunkTable.storage, pool->chunkTable.size, sizeof(void*), cmpPtr);

    // Walk through the chunk table and freelist in parallel.
    {
        int iFreelist=0;
        int iChunk=0;
        void *chunk = mpGetAlignedAddress(pool, pool->chunkTable.storage[iChunk]);
        U32 iAlloc=0;
        while (chunk) {
            void *data = (char*)chunk + pool->structSize * iAlloc;
            if (iFreelist < freelist_size &&
                list[iFreelist] == data)
            {
                // In the freelist, skip it!
                iFreelist++;
            } else {
                func(pool, data, userData);
            }
            iAlloc++;
            if (iAlloc == pool->structCount) {
                iAlloc = 0;
                iChunk++;
                if (iChunk < pool->chunkTable.size)
                    chunk = mpGetAlignedAddress(pool, pool->chunkTable.storage[iChunk]);
                else
                    chunk = NULL;
            }
        }
    }

    if (fromHeap)
        free(list);
}

