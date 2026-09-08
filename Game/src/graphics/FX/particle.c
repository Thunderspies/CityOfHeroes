#include <utilitieslib/utils/wininclude.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/error.h>
#include "cmdparse/cmdgame.h"
#include "render/model.h"
#include <utilitieslib/utils/memcheck.h> 
#include "assert.h" 
#include <utilitieslib/utils/utils.h>
#include "graphics/FX/particle.h"  
#include "render/render.h"
#include "cmdparse/cmdcommon.h"
#include "graphics/camera.h"
#include "assert.h"
#include "graphics/font.h"
#include "fxutil.h"
#include <utilitieslib/components/genericlist.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include "time.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/strings_opt.h>
#include "graphics/gfxwindow.h"
#include <utilitieslib/utils/fileutil.h>
#include "edit/edit_drawlines.h" //for sphere drawing
#include "graphics/FX/fx.h"
#include "player/player.h"
#include "render/tex.h"
#include "entity/entity.h"
#include "graphics/FX/fx.h"
#include "edit/edit_cmd.h"
#include "graphics/zOcclusion.h"
#include "demo.h"
#include "graphics/FX/fxlists.h"
#include "graphics/FX/fxgeo.h"
#include <utilitieslib/utils/rgb_hsv.h>

extern FxEngine fx_engine;
static int partialSystemRun_inited = 0;

#define PART_NO_DEBUG

int totalVertMemAlloced;
int    totalVertArraysAlloced;
int totalVertMemAllocedThisFrame;
int    totalVertArraysAllocedThisFrame;

int totalVertMemFreed;
int    totalVertArraysFreed;
int totalVertMemFreedThisFrame;
int    totalVertArraysFreedThisFrame;

int totalFillUsed;
int totalParticlesDrawn;


typedef struct ParticleInfoList {
    ParticleSystemInfo** list;
} ParticleInfoList;

static int s_particleDisableDrawMode = 0;
static ParticleInfoList particle_info;
ParticleEngine particle_engine; //put globals below in ParticleEngine....


#define PARTICLE_VISITED_ONCE (1 << 2)
static const char PARTICLE_VBO[] = "Particle_VBO";

#define USE_VH_HEAP 1

#if USE_VH_HEAP
// MS: I'll move this stuff to another file.

typedef enum VolatileHeapListIndex {
    VHEAP_LIST_ALLOCATED,
    VHEAP_LIST_FREE,
    VHEAP_LIST_ALL,
    VHEAP_LIST_COUNT,
} VolatileHeapListIndex;

typedef enum VolatileHeapNodeIndex {
    VHEAP_NODE_GROUP,
    VHEAP_NODE_ALL,
    VHEAP_NODE_COUNT,
} VolatileHeapNodeIndex;

typedef struct VolatileHeapAllocation    VolatileHeapAllocation;
typedef struct VolatileHeapChunk        VolatileHeapChunk;

typedef struct VolatileHeapAllocationList {
    VolatileHeapAllocation*            head;
    VolatileHeapAllocation*            tail;
    U32                                count;
    U32                                size;
} VolatileHeapAllocationList;

typedef struct VolatileHeapAllocationListNode {
    VolatileHeapAllocation*            next;
    VolatileHeapAllocation*            prev;
} VolatileHeapAllocationListNode;

#define VHEAP_STORED_COUNT_LEVEL            8
#define VHEAP_STORED_COUNT_ARRAY_SIZE        ((1 << VHEAP_STORED_COUNT_LEVEL) - 1 - 1)

typedef struct VolatileHeapChunk {
    VolatileHeapChunk*                next;
    VolatileHeapChunk*                self;
    U32                                totalSize;
    U32                                maxFreeSize;
    U32                                freeSizeCountTree[VHEAP_STORED_COUNT_ARRAY_SIZE];

    VolatileHeapAllocationList        allocList[VHEAP_LIST_COUNT];
} VolatileHeapChunk;

typedef struct VolatileHeapAllocation {
    VolatileHeapAllocation*            self;
    VolatileHeapChunk*                chunk;

    VolatileHeapAllocationListNode    listNode[VHEAP_NODE_COUNT];
    U8                                allocListIndex[VHEAP_NODE_COUNT];

    void**                            userPointer;
    U32                                size;
} VolatileHeapAllocation;

typedef struct VolatileHeapAllocationFooter {
    U32                                size;
} VolatileHeapAllocationFooter;

static struct {
    VolatileHeapChunk*                chunkHead;
} volatileHeap;

#define VHEAP_ALLOC_OVERHEAD        (sizeof(VolatileHeapAllocation) + sizeof(VolatileHeapAllocationFooter))
#define VHEAP_DEFAULT_CHUNK_SIZE    (SQR(1024) - sizeof(*chunk))

static void vhValidateHeap()
{
    if(0)
    {
        //VolatileHeapChunk* chunk;

        //for(chunk = volatileHeap.chunkHead; chunk; chunk = chunk->next)
        //{
        //    VolatileHeapAllocation* alloc;
        //
        //    for(alloc = chunk->all.head; alloc; alloc = alloc->next)
        //    {
        //        assert(alloc->self == alloc);
        //        assert(alloc->chunk == chunk);
        //    }
        //}
    }
}

static U8* vhGetChunkMemory(VolatileHeapChunk* chunk)
{
    return (U8*)(chunk + 1);
}

static U8* vhGetAllocationMemory(VolatileHeapAllocation* alloc)
{
    return (U8*)(alloc + 1);
}

static VolatileHeapAllocationFooter* vhGetAllocationFooter(VolatileHeapAllocation* alloc)
{
    U8* allocMemory = vhGetAllocationMemory(alloc);

    return (VolatileHeapAllocationFooter*)(allocMemory + alloc->size);
}

static VolatileHeapNodeIndex vhGetNodeIndexFromListIndex(VolatileHeapListIndex i)
{
    switch(i){
        case VHEAP_LIST_FREE:
        case VHEAP_LIST_ALLOCATED:
            return VHEAP_NODE_GROUP;
        case VHEAP_LIST_ALL:
            return VHEAP_NODE_ALL;
    }
    assert(0);
    return 0;
}

static void vhAllocListAdd(VolatileHeapAllocation* alloc, VolatileHeapListIndex i)
{
    VolatileHeapChunk* chunk = alloc->chunk;
    VolatileHeapNodeIndex j = vhGetNodeIndexFromListIndex(i);

    assert(!alloc->listNode[j].prev);
    assert(!alloc->listNode[j].next);
    assert(alloc->allocListIndex[j] == VHEAP_LIST_COUNT);

    alloc->listNode[j].prev = chunk->allocList[i].tail;
    alloc->listNode[j].next = NULL;
    alloc->allocListIndex[j] = i;

    if(chunk->allocList[i].tail)
    {
        chunk->allocList[i].tail->listNode[j].next = alloc;
    }

    chunk->allocList[i].tail = alloc;

    if(!chunk->allocList[i].head)
    {
        chunk->allocList[i].head = alloc;
    }

    chunk->allocList[i].count++;
    chunk->allocList[i].size += alloc->size;
}

static void vhAllocListRemove(VolatileHeapAllocation* alloc, VolatileHeapListIndex i)
{
    VolatileHeapNodeIndex j = vhGetNodeIndexFromListIndex(i);
    VolatileHeapChunk* chunk = alloc->chunk;

    // Check that the list being removed from is the proper list.

    assert(alloc->allocListIndex[j] == i);
    
    alloc->allocListIndex[j] = VHEAP_LIST_COUNT;

    // Check that there is anything in the list.

    assert(chunk->allocList[i].count > 0);

    if(alloc->listNode[j].prev)
    {
        alloc->listNode[j].prev->listNode[j].next = alloc->listNode[j].next;
    }
    else
    {
        assert(chunk->allocList[i].head == alloc);
        chunk->allocList[i].head = alloc->listNode[j].next;
    }

    if(alloc->listNode[j].next)
    {
        alloc->listNode[j].next->listNode[j].prev = alloc->listNode[j].prev;
    }
    else
    {
        assert(chunk->allocList[i].tail == alloc);
        chunk->allocList[i].tail = alloc->listNode[j].prev;
    }

    alloc->listNode[j].prev = alloc->listNode[j].next = NULL;

    chunk->allocList[i].count--;

    assert(chunk->allocList[i].size >= alloc->size);

    chunk->allocList[i].size -= alloc->size;
}

static void vhSetAllocationSize(VolatileHeapAllocation* alloc, U32 size)
{
    VolatileHeapChunk* chunk = alloc->chunk;
    S32 delta = size - alloc->size;
    S32 i;
    
    alloc->size = size;
    
    for(i = 0; i < ARRAY_SIZE(alloc->listNode); i++)
    {
        VolatileHeapListIndex j = alloc->allocListIndex[i];
        
        if(j < VHEAP_LIST_COUNT)
        {
            chunk->allocList[j].size += delta;
        }
    }
}

static void vhSetAllocationFooter(VolatileHeapAllocation* alloc)
{
    VolatileHeapAllocationFooter* allocFooter = vhGetAllocationFooter(alloc);

    allocFooter->size = alloc->size;
}

static void vhUpdateFreeSizeCountHelper(VolatileHeapChunk* chunk, U32 size, U32 loSize, U32 hiSize, U32 loIndex, S32 add)
{
    U32 midSize = (loSize + hiSize) / 2;
    U32 affectIndex;

    if(loIndex >= ARRAY_SIZE(chunk->freeSizeCountTree))
    {
        return;
    }

    if(size >= midSize)
    {
        affectIndex = loIndex + 1;

        vhUpdateFreeSizeCountHelper(chunk, size, midSize, hiSize, (loIndex + 1) * 2 + 1, add);
    }
    else
    {
        affectIndex = loIndex;

        vhUpdateFreeSizeCountHelper(chunk, size, loSize, midSize - 1, (loIndex + 1) * 2, add);
    }

    if(add)
    {
        chunk->freeSizeCountTree[affectIndex]++;
        assert(chunk->freeSizeCountTree[affectIndex] > 0);
    }
    else
    {
        assert(chunk->freeSizeCountTree[affectIndex] > 0);
        chunk->freeSizeCountTree[affectIndex]--;
    }
}

static void vhUpdateFreeSizeCount(VolatileHeapChunk* chunk, U32 size, S32 add)
{
    vhUpdateFreeSizeCountHelper(chunk, size, 0, size, 0, add);
}

static void vhUpdateMaxFreeSize(VolatileHeapChunk* chunk)
{
    VolatileHeapAllocation* alloc;
    U32 maxSize = 0;

    for(alloc = chunk->allocList[VHEAP_LIST_FREE].head; alloc; alloc = alloc->listNode[VHEAP_NODE_GROUP].next)
    {
        if(alloc->size > maxSize)
        {
            maxSize = alloc->size;
        }
    }

    chunk->maxFreeSize = maxSize;
}

static void vhInitFreeAllocation(VolatileHeapAllocation* alloc)
{
    S32 i;
    
    // Set the footer.
    
    vhSetAllocationFooter(alloc);

    // Set the lists.
    
    for(i = 0; i < ARRAY_SIZE(alloc->allocListIndex); i++)
    {
        alloc->allocListIndex[i] = VHEAP_LIST_COUNT;
    }

    // Add to the unused list.

    vhAllocListAdd(alloc, VHEAP_LIST_FREE);

    // Add to the all list.

    vhAllocListAdd(alloc, VHEAP_LIST_ALL);
}

static VolatileHeapChunk* vhGetFreeChunk(U32 size)
{
    VolatileHeapChunk* chunk;
    VolatileHeapAllocation* alloc;
    U32 requiredSize;
    U32 totalSize;

    for(chunk = volatileHeap.chunkHead; chunk; chunk = chunk->next)
    {
        if(chunk->maxFreeSize >= size)
        {
            return chunk;
        }
    }

    // Create a new heap chunk.

    assert(VHEAP_DEFAULT_CHUNK_SIZE > VHEAP_ALLOC_OVERHEAD);

    requiredSize = size + sizeof(*alloc) + sizeof(U32);

    size = max(requiredSize, VHEAP_DEFAULT_CHUNK_SIZE);

    // Allocate the chunk header, plus memory needed.

    totalSize = sizeof(*chunk) + size;

    //printf("Creating chunk, size %s bytes.\n", getCommaSeparatedInt(totalSize));

    chunk = malloc(totalSize);

    ZeroStruct(chunk);

    chunk->self = chunk;
    chunk->totalSize = totalSize;

    // Add to the chunk list.

    chunk->next = volatileHeap.chunkHead;
    volatileHeap.chunkHead = chunk;

    // Create the one empty allocation.

    alloc = (VolatileHeapAllocation*)vhGetChunkMemory(chunk);

    ZeroStruct(alloc);
    alloc->chunk = chunk;
    alloc->self = alloc;
    alloc->size = vhGetChunkMemory(chunk) + size - vhGetAllocationMemory(alloc) - sizeof(U32);
    
    vhInitFreeAllocation(alloc);
    
    // Set the chunk free size.

    vhUpdateMaxFreeSize(chunk);

    return chunk;
}

static VolatileHeapAllocation* vhFindAllocSize(VolatileHeapChunk* chunk, U32 size)
{
    VolatileHeapAllocation* alloc;

    assert(size <= chunk->maxFreeSize);

    for(alloc = chunk->allocList[VHEAP_LIST_FREE].head; alloc; alloc = alloc->listNode[VHEAP_NODE_GROUP].next){
        assert(alloc->self == alloc);
        if(size <= alloc->size){
            assert(alloc->size == *(U32*)((U8*)vhGetAllocationMemory(alloc) + alloc->size));

            return alloc;
        }
    }

    assert(0);

    return NULL;
}

static VolatileHeapAllocation* vhGetNextAdjacentAllocation(VolatileHeapAllocation* alloc)
{
    return (VolatileHeapAllocation*)(vhGetAllocationFooter(alloc) + 1);
}

static VolatileHeapAllocation* vhGetPrevAdjacentAllocation(VolatileHeapAllocation* alloc)
{
    VolatileHeapAllocationFooter* prevAllocFooter = (VolatileHeapAllocationFooter*)alloc - 1;
    
    return (VolatileHeapAllocation*)((U8*)prevAllocFooter - prevAllocFooter->size) - 1;
}

static void vhCreateNewAllocation(VolatileHeapAllocation* alloc, U32 size)
{
    VolatileHeapChunk* chunk = alloc->chunk;
    U8* allocMemory = vhGetAllocationMemory(alloc);
    U32 originalSize = alloc->size;
    U32 remainingSize = originalSize - size;

    // Remove from the unused list.

    vhAllocListRemove(alloc, VHEAP_LIST_FREE);

    // Go to full size if there isn't enough room left for the allocation overhead.

    if(remainingSize >= VHEAP_ALLOC_OVERHEAD)
    {
        VolatileHeapAllocation* newAlloc;

        // Shrink the original allocation.
        
        vhSetAllocationSize(alloc, size);
        vhSetAllocationFooter(alloc);
        
        // Create the new allocation.

        newAlloc = vhGetNextAdjacentAllocation(alloc);

        ZeroStruct(newAlloc);

        newAlloc->size = remainingSize - VHEAP_ALLOC_OVERHEAD;
        newAlloc->self = newAlloc;
        newAlloc->chunk = alloc->chunk;

        vhInitFreeAllocation(newAlloc);
    }

    vhAllocListAdd(alloc, VHEAP_LIST_ALLOCATED);

    if(chunk->maxFreeSize == originalSize)
    {
        vhUpdateMaxFreeSize(chunk);
    }
}


S32 vhAlloc(void** userPointer, U32 size, S32 zeroMemory)
{
    VolatileHeapChunk* chunk;
    VolatileHeapAllocation* alloc;
    U32 userSize = size;

    PERFINFO_AUTO_START("vhAlloc", 1);

        vhValidateHeap();

        // Increment size up to a multiple of 4.

        if(userSize & 3)
        {
            userSize += 4 - (userSize & 3);
        }

        // Get a chunk with that much free space.

        chunk = vhGetFreeChunk(userSize);

        if(!chunk)
        {
            *userPointer = NULL;
            PERFINFO_AUTO_STOP();
            return 0;
        }

        assert(userSize <= chunk->maxFreeSize);

        // Find an unused allocation of large enough size.

        alloc = vhFindAllocSize(chunk, userSize);

        // Carve the necessary sized chunk out of this allocation.

        vhCreateNewAllocation(alloc, userSize);

        // Set the user pointer and return.

        alloc->userPointer = userPointer;

        *userPointer = vhGetAllocationMemory(alloc);

    PERFINFO_AUTO_STOP();

    return 1;
}

S32 vhIsLastAllocation(VolatileHeapAllocation* alloc)
{
    VolatileHeapChunk* chunk = alloc->chunk;
    U8* allocEnd = (U8*)alloc + alloc->size + VHEAP_ALLOC_OVERHEAD;
    U8* chunkEnd = (U8*)chunk + chunk->totalSize;

    assert(allocEnd <= chunkEnd);

    return allocEnd == chunkEnd;
}

void vhFree(void** userPointer)
{
    VolatileHeapAllocation* alloc;
    VolatileHeapAllocation* firstAlloc;
    VolatileHeapChunk*        chunk;
    S32                        addToFreeList;
    
    if(!userPointer || !*userPointer)
    {
        return;
    }

    alloc = (VolatileHeapAllocation*)*userPointer - 1;
    addToFreeList = 1;

    PERFINFO_AUTO_START("vhFree", 1);

        vhValidateHeap();

        assert(alloc->self == alloc);
        assert(alloc->userPointer == userPointer);
        assert(alloc->chunk);

        chunk = alloc->chunk;

        assert(chunk->self == chunk);

        // Remove from the allocated list.

        vhAllocListRemove(alloc, VHEAP_LIST_ALLOCATED);

        // Merge with the previous allocation.

        firstAlloc = (VolatileHeapAllocation*)vhGetChunkMemory(chunk);

        if(alloc != firstAlloc)
        {
            VolatileHeapAllocation* prevAlloc = vhGetPrevAdjacentAllocation(alloc);

            assert(prevAlloc->self == prevAlloc);
            assert(prevAlloc->chunk == chunk);

            // Check if this allocation is free.

            if(!prevAlloc->userPointer)
            {
                // Remove the original allocation completely.

                vhAllocListRemove(alloc, VHEAP_LIST_ALL);

                //+-----------+-----+--------+-------+-----+--------+
                //| prevAlloc | mem | footer | alloc | mem | footer |
                //+-----------+-----+--------+-------+-----+--------+

                vhSetAllocationSize(prevAlloc, prevAlloc->size + alloc->size + VHEAP_ALLOC_OVERHEAD);

                vhSetAllocationFooter(prevAlloc);

                addToFreeList = 0;

                alloc = prevAlloc;
            }
        }

        if(!vhIsLastAllocation(alloc))
        {
            VolatileHeapAllocation* nextAlloc = vhGetNextAdjacentAllocation(alloc);

            assert(nextAlloc->self == nextAlloc);
            assert(nextAlloc->chunk == chunk);

            if(!nextAlloc->userPointer)
            {
                vhAllocListRemove(nextAlloc, VHEAP_LIST_ALL);
                vhAllocListRemove(nextAlloc, VHEAP_LIST_FREE);

                //+-------+-----+--------+-----------+-----+--------+
                //| alloc | mem | footer | nextAlloc | mem | footer |
                //+-------+-----+--------+-----------+-----+--------+

                vhSetAllocationSize(alloc, alloc->size + nextAlloc->size + VHEAP_ALLOC_OVERHEAD);

                vhSetAllocationFooter(alloc);
            }
        }

        // Add to the free list if this is a new allocation (merged with previous).

        if(addToFreeList)
        {
            vhAllocListAdd(alloc, VHEAP_LIST_FREE);
        }

        // Update the maxFreeSize.

        if(alloc->size > chunk->maxFreeSize)
        {
            chunk->maxFreeSize = alloc->size;
        }

        // Set the user pointer.
        
        alloc->userPointer = NULL;
        *userPointer = NULL;

    PERFINFO_AUTO_STOP();
}

void vhRealloc(void** userPointer, U32 newSize, S32 zeroMemory)
{
    
}

void vhCompact()
{
    VolatileHeapChunk* writeChunk;

    PERFINFO_AUTO_START("vhCompact", 1);

    for(writeChunk = volatileHeap.chunkHead; writeChunk; writeChunk = writeChunk->next)
    {
        if(writeChunk->allocList[VHEAP_LIST_FREE].count > 1)
        {
            VolatileHeapAllocation* alloc = (VolatileHeapAllocation*)vhGetChunkMemory(writeChunk);

            while(!vhIsLastAllocation(alloc))
            {
                VolatileHeapAllocation* nextAlloc;
                U32 oldSize;

                assert(alloc->self == alloc);
                assert(alloc->chunk == writeChunk);
                
                nextAlloc = vhGetNextAdjacentAllocation(alloc);

                if(alloc->userPointer)
                {
                    // This is allocated, so go to next allocation.

                    alloc = nextAlloc;
                    continue;
                }

                // Next allocation is free, so merge with me.
                
                assert(nextAlloc->self == nextAlloc);
                assert(nextAlloc->chunk == writeChunk);
                assert(nextAlloc->userPointer);

                vhAllocListRemove(alloc, VHEAP_LIST_FREE);

                vhAllocListRemove(nextAlloc, VHEAP_LIST_ALLOCATED);
                vhAllocListRemove(nextAlloc, VHEAP_LIST_ALL);

                oldSize = alloc->size;
                vhSetAllocationSize(alloc, nextAlloc->size);
                alloc->userPointer = nextAlloc->userPointer;
                *alloc->userPointer = vhGetAllocationMemory(alloc);

                PERFINFO_AUTO_START("memcpy", 1);
                    memcpy(vhGetAllocationMemory(alloc), vhGetAllocationMemory(nextAlloc), alloc->size);
                PERFINFO_AUTO_STOP();

                vhSetAllocationFooter(alloc);

                vhAllocListAdd(alloc, VHEAP_LIST_ALLOCATED);

                // Setup the free allocation.

                alloc = vhGetNextAdjacentAllocation(alloc);

                ZeroStruct(alloc);

                alloc->self = alloc;
                alloc->size = oldSize;
                alloc->chunk = writeChunk;

                if(!vhIsLastAllocation(alloc))
                {
                    // Merge the next chunk if it's free.

                    nextAlloc = vhGetNextAdjacentAllocation(alloc);

                    if(!nextAlloc->userPointer)
                    {
                        vhAllocListRemove(nextAlloc, VHEAP_LIST_FREE);
                        vhAllocListRemove(nextAlloc, VHEAP_LIST_ALL);

                        alloc->size += nextAlloc->size + VHEAP_ALLOC_OVERHEAD;
                    }
                }

                vhInitFreeAllocation(alloc);
            }

            vhUpdateMaxFreeSize(writeChunk);
        }
    }

    PERFINFO_AUTO_STOP();
}
#endif

// List Management ##########################################################
///List management functions for System list and for

static void partSystemReset(ParticleSystem * sys);

//initialize a big reservoir so each is linked to the next...
static void partSystemListInit(ParticleSystem reservoir[MAX_SYSTEMS])
{
    int        i;

    particle_engine.systems     = 0;
    particle_engine.num_systems = 0;

    for(i = 0 ; i < MAX_SYSTEMS ; i++)
    {
        reservoir[i].next  = (i == MAX_SYSTEMS - 1) ? 0 : &reservoir[i+1];
        reservoir[i].prev  = (i == 0              ) ? 0 : &reservoir[i-1];
        reservoir[i].inuse = 0;

        // Just in case the memory we are working with is not already zeroed out.
        // Do this to prevent freeing a bogus pointer in partSystemReset()
        reservoir[i].colorPathCopy.path = NULL;
        reservoir[i].particles = NULL;

        partSystemReset(&reservoir[i]);
    }
    particle_engine.system_head = &reservoir[0];
}

static bool s_complainedAboutNodes = false;

//allocate x nodes from the reservoir to an active list. If no active list, that's fine
static ParticleSystem * partAddSystemNodes(int count, int * alloced)
{
int        i;
ParticleSystem  * added_first;
ParticleSystem  * added_last;

    (*alloced) = 0;

    if(count < 1)
        return 0;
    if(!particle_engine.system_head)
    {
        //too many particles debug
        if (isDevelopmentMode() && !s_complainedAboutNodes)
        {
            s_complainedAboutNodes = true;
            Errorf( "PART: particle_engine.system_head out of system nodes!!! " );
        }
        return 0;
    }

    added_first = particle_engine.system_head;
    added_first->inuse = 1;
    (*alloced)++;

    for(i = 1, added_last = added_first ; i < count ; i++)
    {
        if ( !added_last->next )
            break;
        added_last = added_last->next;
        added_last->inuse = 1;
        (*alloced)++;
    }

    particle_engine.system_head        = added_last->next;
    if(particle_engine.system_head)
        particle_engine.system_head->prev  = 0;

    added_last->next  = particle_engine.systems;
    if(particle_engine.systems)
        particle_engine.systems->prev = added_last;

    particle_engine.systems = added_first;

    particle_engine.num_systems += (*alloced);

    return added_first;
}

//free all nodes from the active on, returning them to reservoir
static void partFreeAllSystemNodes()
{
    ParticleSystem * node;

    if (!particle_engine.systems)
        return;
    for(node = particle_engine.systems ; node->next ; node = node->next)
    {
        node->inuse = 0;
        partSystemReset(node);
    }

    node->next        = particle_engine.system_head;
    if(node->next)
        node->next->prev  = node;

    particle_engine.system_head = particle_engine.systems;
    particle_engine.systems     = 0;
    particle_engine.num_systems = 0;
}

//free just the active node given, returning it to the reservoir
static void partFreeSingleSystemNode(ParticleSystem * active)
{
    if(!active)
        return;

    active->inuse = 0;
    partSystemReset(active);
    particle_engine.num_systems--;

    //If nec. fix systems pointer
    if(active == particle_engine.systems)
    {
        particle_engine.systems = active->next;
        if(particle_engine.systems == 0) assert(particle_engine.num_systems == 0);
    }

    //Sew up gap left by removing active:
    if(active->next)
        active->next->prev = active->prev;
    if(active->prev)
        active->prev->next = active->next;

    //Attach active to the beginning of the reservoir
    if(particle_engine.system_head)
        particle_engine.system_head->prev  = active;
    active->next                       = particle_engine.system_head;
    active->prev                       = 0;
    particle_engine.system_head        = active;
}

static void partSystemReset(ParticleSystem * sys)
{
    void * savedNext;
    void * savedPrev;

    // partSystemReset should only be called on particle systems which are no longer in use
    devassert(sys->inuse == 0);

    savedNext = sys->next;
    savedPrev = sys->prev;

    SAFE_FREE(sys->colorPathCopy.path);

#if USE_VH_HEAP
    vhFree(&sys->particles);
#else
    free( sys->particles ); //New Allocing (someday pool)
#endif

    ZeroStruct(sys);

    sys->next = savedNext;
    sys->prev = savedPrev;
    sys->unique_id = -1;
}

// SystemInfo ############################################################

StaticDefineInt    ParticleFlags[] =
{
    DEFINE_INT
    { "AlwaysDraw",                PART_ALWAYS_DRAW },
    { "FadeImmediatelyOnDeath", PART_FADE_IMMEDIATELY_ON_DEATH },
    { "Ribbon",                    PART_RIBBON },
    { "IgnoreFxTint",            PART_IGNORE_FX_TINT },
    DEFINE_END
};

StaticDefineInt    ParseParticleBlendMode[] =
{
    DEFINE_INT
    { "Normal",                PARTICLE_NORMAL},
    { "Additive",            PARTICLE_ADDITIVE},
    { "Subtractive",        PARTICLE_SUBTRACTIVE},
    { "PremultipliedAlpha",    PARTICLE_PREMULTIPLIED_ALPHA},
    { "Multiply",            PARTICLE_MULTIPLY},
    { "SubtractiveInverse",    PARTICLE_SUBTRACTIVE_INVERSE},
    DEFINE_END
};


TokenizerParseInfo SystemParseInfo[] = 
{
    { "System",                    TOK_IGNORE,    0    }, // hack, so we can use the old list system for a bit..
    { "Name",                    TOK_CURRENTFILE(ParticleSystemInfo,name)            },

    { "FrontOrLocalFacing",        TOK_INT(ParticleSystemInfo,frontorlocalfacing,0)        },
    { "WorldOrLocalPosition",    TOK_INT(ParticleSystemInfo,worldorlocalposition,0)    },

    { "TimeToFull",                TOK_F32(ParticleSystemInfo,timetofull,            PART_DEFAULT_TIMETOFULL)    },
    { "KickStart",                TOK_INT(ParticleSystemInfo,kickstart,0)                },

    { "NewPerFrame",            TOK_F32ARRAY(ParticleSystemInfo,new_per_frame)        },
    { "Burst",                    TOK_INTARRAY(ParticleSystemInfo,burst)                },
    { "BurbleAmplitude",        TOK_F32ARRAY(ParticleSystemInfo,burbleamplitude)    },
    { "BurbleType",                TOK_INT(ParticleSystemInfo,burbletype,0)                },
    { "BurbleFrequency",        TOK_F32(ParticleSystemInfo,burblefrequency,0)        },
    { "BurbleThreshold",        TOK_F32(ParticleSystemInfo,burblethreshold,0)        },
    { "MoveScale",                TOK_F32(ParticleSystemInfo,movescale,0)                },

    { "EmissionType",            TOK_INT(ParticleSystemInfo,emission_type,0)            },
    { "EmissionStartJitter",    TOK_F32ARRAY(ParticleSystemInfo,emission_start_jitter)    },
    { "EmissionRadius",            TOK_F32(ParticleSystemInfo,emission_radius,0)        },
    { "EmissionHeight",            TOK_F32(ParticleSystemInfo,emission_height,0)        },
    { "EmissionLifeSpan",        TOK_F32(ParticleSystemInfo,emission_life_span,0)        },
    { "EmissionLifeSpanJitter",    TOK_F32(ParticleSystemInfo,emission_life_span_jitter,0)        },

    { "Spin",                    TOK_INT(ParticleSystemInfo,spin,0)                    },
    { "SpinJitter",                TOK_INT(ParticleSystemInfo,spin_jitter,0)            },
    { "OrientationJitter",        TOK_INT(ParticleSystemInfo,orientation_jitter,0)        },

    { "Magnetism",                TOK_F32(ParticleSystemInfo,magnetism,0)                },
    { "Gravity",                TOK_F32(ParticleSystemInfo,gravity,0)                },
    { "KillOnZero",                TOK_INT(ParticleSystemInfo,kill_on_zero,0)            },
    { "Terrain",                TOK_INT(ParticleSystemInfo,terrain,0)                },

    { "InitialVelocity",        TOK_F32ARRAY(ParticleSystemInfo,initial_velocity)        },
    { "InitialVelocityJitter",    TOK_F32ARRAY(ParticleSystemInfo,initial_velocity_jitter)},
    { "VelocityJitter",            TOK_VEC3(ParticleSystemInfo,velocity_jitter)        },
    { "TightenUp",                TOK_F32(ParticleSystemInfo,tighten_up,0)                },
    { "SortBias",                TOK_F32(ParticleSystemInfo,sortBias,0)                },
    { "Drag",                    TOK_F32(ParticleSystemInfo,drag,0)                    },
    { "Stickiness",                TOK_F32(ParticleSystemInfo,stickiness,0)                },

    { "ColorOffset",            TOK_VEC3(ParticleSystemInfo,colorOffset)            },
    { "ColorOffsetJitter",        TOK_VEC3(ParticleSystemInfo,colorOffsetJitter)        },

    { "Alpha",                    TOK_INTARRAY(ParticleSystemInfo,alpha)                },
    { "ColorChangeType",        TOK_INT(ParticleSystemInfo,colorchangetype,0)        },

    { "StartColor",                TOK_RGB(ParticleSystemInfo,colornavpoint[0].rgb)    },
    { "BeColor1",                TOK_RGB(ParticleSystemInfo,colornavpoint[1].rgb)    },
    { "BeColor2",                TOK_RGB(ParticleSystemInfo,colornavpoint[2].rgb)    },
    { "BeColor3",                TOK_RGB(ParticleSystemInfo,colornavpoint[3].rgb)    },
    { "BeColor4",                TOK_RGB(ParticleSystemInfo,colornavpoint[4].rgb)    },

    { "ByTime1",                TOK_INT(ParticleSystemInfo,colornavpoint[1].time,0)    },
    { "ByTime2",                TOK_INT(ParticleSystemInfo,colornavpoint[2].time,0)    },
    { "ByTime3",                TOK_INT(ParticleSystemInfo,colornavpoint[3].time,0)    },
    { "ByTime4",                TOK_INT(ParticleSystemInfo,colornavpoint[4].time,0)    },

    { "PrimaryTint",            TOK_F32(ParticleSystemInfo,colornavpoint[0].primaryTint,0)    },
    { "PrimaryTint1",            TOK_F32(ParticleSystemInfo,colornavpoint[1].primaryTint,0)    },
    { "PrimaryTint2",            TOK_F32(ParticleSystemInfo,colornavpoint[2].primaryTint,0)    },
    { "PrimaryTint3",            TOK_F32(ParticleSystemInfo,colornavpoint[3].primaryTint,0)    },
    { "PrimaryTint4",            TOK_F32(ParticleSystemInfo,colornavpoint[4].primaryTint,0)    },

    { "SecondaryTint",            TOK_F32(ParticleSystemInfo,colornavpoint[0].secondaryTint,0)    },
    { "SecondaryTint1",            TOK_F32(ParticleSystemInfo,colornavpoint[1].secondaryTint,0)    },
    { "SecondaryTint2",            TOK_F32(ParticleSystemInfo,colornavpoint[2].secondaryTint,0)    },
    { "SecondaryTint3",            TOK_F32(ParticleSystemInfo,colornavpoint[3].secondaryTint,0)    },
    { "SecondaryTint4",            TOK_F32(ParticleSystemInfo,colornavpoint[4].secondaryTint,0)    },

    { "Rgb0",                    TOK_RGB(ParticleSystemInfo,colornavpoint[0].rgb)    },
    { "Rgb1",                    TOK_RGB(ParticleSystemInfo,colornavpoint[1].rgb)    },
    { "Rgb2",                    TOK_RGB(ParticleSystemInfo,colornavpoint[2].rgb)    },
    { "Rgb3",                    TOK_RGB(ParticleSystemInfo,colornavpoint[3].rgb)    },
    { "Rgb4",                    TOK_RGB(ParticleSystemInfo,colornavpoint[4].rgb)    },
    { "Rgb0Time",                TOK_INT(ParticleSystemInfo,colornavpoint[1].time,0)    },
    { "Rgb1Time",                TOK_INT(ParticleSystemInfo,colornavpoint[2].time,0)    },
    { "Rgb2Time",                TOK_INT(ParticleSystemInfo,colornavpoint[3].time,0)    },
    { "Rgb3Time",                TOK_INT(ParticleSystemInfo,colornavpoint[4].time,0)    },
    { "Rgb4Time",                TOK_INT(ParticleSystemInfo,colornavpoint[0].time,0)    }, //junk

    { "FadeInBy",                TOK_F32(ParticleSystemInfo,fade_in_by,0)                },
    { "FadeOutStart",            TOK_F32(ParticleSystemInfo,fade_out_start,0)            },
    { "FadeOutBy",                TOK_F32(ParticleSystemInfo,fade_out_by,0)            },

    { "DieLikeThis",            TOK_FIXEDSTR(ParticleSystemInfo,dielikethis)        },
    { "DeathAgeToZero",            TOK_U8(ParticleSystemInfo,deathagetozero,0)            },

    { "StartSize",                TOK_F32ARRAY(ParticleSystemInfo,startsize)            },
    { "StartSizeJitter",        TOK_F32(ParticleSystemInfo,startsizejitter,0)        },
    { "Blend_mode",                TOK_INT(ParticleSystemInfo,blend_mode, 0), ParseParticleBlendMode    },

    { "TextureName",            TOK_FIXEDSTR(ParticleSystemInfo,parttex[0].texturename)    },
    { "TextureName2",            TOK_FIXEDSTR(ParticleSystemInfo,parttex[1].texturename)    },
    { "TexScroll1",              TOK_VEC3(ParticleSystemInfo,parttex[0].texscroll)    },
    { "TexScroll2",              TOK_VEC3(ParticleSystemInfo,parttex[1].texscroll)    },

    { "TexScrollJitter1",        TOK_VEC3(ParticleSystemInfo,parttex[0].texscrolljitter)},
    { "TexScrollJitter2",        TOK_VEC3(ParticleSystemInfo,parttex[1].texscrolljitter)},
    { "AnimFrames1",            TOK_F32(ParticleSystemInfo,parttex[0].animframes,0)    },
    { "AnimFrames2",            TOK_F32(ParticleSystemInfo,parttex[1].animframes,0)    },

    { "AnimPace1",                TOK_F32(ParticleSystemInfo,parttex[0].animpace,0)    },
    { "AnimPace2",                TOK_F32(ParticleSystemInfo,parttex[1].animpace,0)    },
    { "AnimType1",                TOK_INT(ParticleSystemInfo,parttex[0].animtype,0)    },
    { "AnimType2",                TOK_INT(ParticleSystemInfo,parttex[1].animtype,0)    },

    { "EndSize",                TOK_F32ARRAY(ParticleSystemInfo,endsize)            },
    { "ExpandRate",                TOK_F32(ParticleSystemInfo,expandrate,0)                },
    { "ExpandType",                TOK_INT(ParticleSystemInfo,expandtype,0)                },

    { "StreakType",                TOK_INT(ParticleSystemInfo,streaktype,0)                },
    { "StreakScale",            TOK_F32(ParticleSystemInfo,streakscale,0)            },
    { "StreakOrient",            TOK_INT(ParticleSystemInfo,streakorient,0)            },
    { "StreakDirection",        TOK_INT(ParticleSystemInfo,streakdirection,0)        },

    { "VisRadius",                TOK_F32(ParticleSystemInfo,visradius,0)                },
    { "VisDist",                TOK_F32(ParticleSystemInfo,visdist,0)                },

    { "Flags",                    TOK_FLAGS(ParticleSystemInfo, flags,    0),    ParticleFlags},

    { "End",                    TOK_END,    0                                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParticleParseInfo[] =
{
    { "System",                    TOK_STRUCT(ParticleInfoList,list,SystemParseInfo) },
    { "", 0, 0 }
};


int fxPartNameCmp(const void* info1Data, const void* info2Data)
{
    const ParticleSystemInfo ** info1 = (const ParticleSystemInfo **)info1Data;
    const ParticleSystemInfo ** info2 = (const ParticleSystemInfo **)info2Data;
    return stricmp( (*info1)->name, (*info2)->name );
}

int fxPartNameCmp2(const void* info1Data, const void* info2Data)
{
    const ParticleSystemInfo * info1 = (const ParticleSystemInfo *)info1Data;
    const ParticleSystemInfo ** info2 = (const ParticleSystemInfo **)info2Data;
    return stricmp( info1->name, (*info2)->name );
}

// load all the particles in the fx directory
void partPreloadParticles()
{
    char *dir=game_state.nofx?NULL:"fx";
    char *filetype=game_state.nofx?"fx/Generic/Generic_pow.part":".part";
    int flags=0;
    char *binFilename = game_state.nofx?0:"particles.bin";
    writeConsole(OUTPUT_DEBUG, "Preloading particles");
    // Absolutely cannot be shared (members get modified/added to)
    ParserLoadFiles(dir, filetype, binFilename, flags, ParticleParseInfo, &particle_info, NULL, NULL, NULL);

    //clean up particle system names and sort them
    {
        char buf2[FX_MAX_PATH];
        int num_structs, i;
        ParticleSystemInfo * sysInfo;

        //Clean Up FxInfo names, and sort them
        num_structs = eaSize(&particle_info.list);
        for (i = 0; i < num_structs; i++)
        {
            sysInfo = particle_info.list[i];
            fxCleanFileName(buf2, sysInfo->name); //(buf2 will always be shorter than info->name)
            strcpy(sysInfo->name, buf2);
            if (sysInfo->velocity_jitter[0] || sysInfo->velocity_jitter[1] || sysInfo->velocity_jitter[2])
                sysInfo->has_velocity_jitter = 1;
        }
        qsort(particle_info.list, num_structs, sizeof(void*), fxPartNameCmp);
        // Check for duplicate part names (i.e. two systems in one file)
        if (game_state.fxdebug) {
            for (i=0; i<num_structs-1; i++) {
                sysInfo = particle_info.list[i];
                if (stricmp(particle_info.list[i]->name, particle_info.list[i+1]->name)==0) {
                    Errorf("Two Systems defined in one .part file: %s", particle_info.list[i+1]->name);
                }
            }
        }
    }
    writeConsole(OUTPUT_INFO, "Preloaded particles");
    //ParserWriteBinaryFile("particles2.bin", ParticleParseInfo, &particle_info);
}

void ExpandF32Params(meafHandle *array, int len)
{
    int fromindex = 0;
    if (!(*array))
    {
        eafCreate(array);
        eafPush(array, 0.0);
    }
    while (eafSize(array) < len)
    {
        eafPush(array, (*array)[fromindex++]);
    }
}

void ExpandIntParams(eaiHandle *array, int len)
{
    int fromindex = 0;
    if (!(*array))
    {
        eaiCreate(array);
        eaiPush(array, 0);
    }
    while (eaiSize(array) < len)
    {
        eaiPush(array, (*array)[fromindex++]);
    }
}

//Makes a best guess on when not to update
void partCalculateDrawingParameters( ParticleSystemInfo * sysInfo )
{


}


void partInitSystemInfo(ParticleSystemInfo* sysInfo)
{
    int i;

    // Set texture animation parameters
    for(i = 0 ; i < 2 ; i++ )
    {
        sysInfo->parttex[i].bind = texLoadBasic(sysInfo->parttex[i].texturename,TEX_LOAD_IN_BACKGROUND, TEX_FOR_FX);

        assert(sysInfo->parttex[i].bind);

        if(sysInfo->parttex[i].animframes <= 1.0)
        {
            sysInfo->parttex[i].framewidth    = 1;
            sysInfo->parttex[i].texscale[0] = 1.0;
            sysInfo->parttex[i].texscale[1] = 1.0;
        }
        else if(sysInfo->parttex[i].animframes <= 2.0)
        {
            sysInfo->parttex[i].framewidth    = 2;
            sysInfo->parttex[i].texscale[0] = 0.5;
            sysInfo->parttex[i].texscale[1] = 1.0;
        }
        else if(sysInfo->parttex[i].animframes <= 4.0)
        {
            sysInfo->parttex[i].framewidth    = 2;
            sysInfo->parttex[i].texscale[0] = 0.5;
            sysInfo->parttex[i].texscale[1] = 0.5;
        }
        else if(sysInfo->parttex[i].animframes <= 8.0)
        {
            sysInfo->parttex[i].framewidth    = 4;
            sysInfo->parttex[i].texscale[0] = 0.25;
            sysInfo->parttex[i].texscale[1] = 0.5;
        }
        else if(sysInfo->parttex[i].animframes <= 16.0)
        {
            sysInfo->parttex[i].framewidth    = 4;
            sysInfo->parttex[i].texscale[0] = 0.25;
            sysInfo->parttex[i].texscale[1] = 0.25;
        }
        else if(sysInfo->parttex[i].animframes <= 32.0)
        {
            sysInfo->parttex[i].framewidth    = 8;
            sysInfo->parttex[i].texscale[0] = 0.125;
            sysInfo->parttex[i].texscale[1] = 0.25;
        }
        else
            assert(0);
    }

    // ############## Init Scaled Params
    // Scalable PARAMS need to be allocated if they don't exist ( because they
    //are read in as dynamically alloced arrays for convenience of the parser, and
    // not alloced if not in the .part file ).  Then the second value needs to be filled
    //in with the first value if it wasn't included...
    ExpandF32Params(&sysInfo->new_per_frame, 2);
    ExpandIntParams(&sysInfo->burst, 2);
    ExpandF32Params(&sysInfo->burbleamplitude, 2);
    ExpandF32Params(&sysInfo->emission_start_jitter, 6);
    ExpandF32Params(&sysInfo->initial_velocity, 6);
    ExpandF32Params(&sysInfo->initial_velocity_jitter, 6);
    ExpandF32Params(&sysInfo->startsize, 2);
    ExpandF32Params(&sysInfo->endsize, 2);
    ExpandIntParams(&sysInfo->alpha, 2);

    if( !sysInfo->alpha[0] )
        sysInfo->alpha[0] = 255;
    if( !sysInfo->alpha[1] )
        sysInfo->alpha[1] = 255;

    if( !sysInfo->visdist) //Init visibility dist to a really big number
        sysInfo->visdist = 10000;

    // ############# End Managing Scaled Params ######################################
    //fade_out_time of more than 256 makes no sense, since you must fade out by one each frame...
    if( sysInfo->fade_out_by - sysInfo->fade_out_start > 256 )
    {
        sysInfo->fade_out_by = sysInfo->fade_out_start + 256;
    }

    sysInfo->burblethreshold = (sysInfo->burblethreshold * 2) - 1; //out smart self? convert 0-1 to -1 to 1
    if(!sysInfo->burblefrequency)
        sysInfo->burblefrequency = 1;

    //TO DO fix this so it does a better job
    if(    !sysInfo->colornavpoint[0].rgb[0] &&
        !sysInfo->colornavpoint[0].rgb[1] &&
        !sysInfo->colornavpoint[0].rgb[2])
    {
        sysInfo->colornavpoint[0].rgb[0] = 255;
        sysInfo->colornavpoint[0].rgb[1] = 255;
        sysInfo->colornavpoint[0].rgb[2] = 255;
    }



    partBuildColorPath(&sysInfo->colorPathDefault, sysInfo->colornavpoint, 0, false);
    sysInfo->colornavpoint[0].time = 0; //mm temp

    partCalculateDrawingParameters( sysInfo );

    // If either of these asserts go off, the optimizing compiler appears to have messed up.
    assert(sysInfo->parttex[0].bind);
    assert(sysInfo->parttex[1].bind);

}

static ParticleSystemInfo * partLoadSystemInfo(char fname[])
{
    ParticleSystemInfo * sysInfo = 0;
    int fileisgood = 0;
    TokenizerHandle tok;
    char buf[1000];

    errorLogFileIsBeingReloaded(fname);

    tok = TokenizerCreate(fname);
    if (tok)
    {
        sysInfo = (ParticleSystemInfo*)listAddNewMember(&particle_engine.systeminfos, sizeof(ParticleSystemInfo));
        assert( sysInfo );
        listScrubMember(sysInfo, sizeof(ParticleSystemInfo));
        ParserInitStruct(sysInfo, 0, SystemParseInfo);
        fileisgood = TokenizerParseList(tok, SystemParseInfo, sysInfo, TokenizerErrorCallback);
        if( !fileisgood )
        {
            listFreeMember( sysInfo, &particle_engine.systeminfos );
            sysInfo = 0;
        }
        else
        {
            fxCleanFileName(buf, sysInfo->name); //(buf will always be shorter than info->name)
            strcpy(sysInfo->name, buf);
            if (sysInfo->velocity_jitter[0] || sysInfo->velocity_jitter[1] || sysInfo->velocity_jitter[2])
                sysInfo->has_velocity_jitter = 1;
            particle_engine.num_systeminfos++;
        }
    }
    TokenizerDestroy(tok);

    return sysInfo;
}


ParticleSystemInfo * partGetSystemInfo( char system_name[] )
{
    ParticleSystemInfo * sysinfo = 0;
    char part_name_cleaned_up[FX_MAX_PATH];
    int numparticles;
    ParticleSystemInfo dummy = {0};
    ParticleSystemInfo * * dptr;

    fxCleanFileName(part_name_cleaned_up, system_name);

    // ## Search Marks binary of loaded fxinfos.
    dummy.name = part_name_cleaned_up;
    numparticles = eaSize(&particle_info.list);
    dptr = bsearch(&dummy, particle_info.list, numparticles,
                  sizeof(ParticleSystemInfo*),fxPartNameCmp2);
    if( dptr )
    {
        sysinfo = *dptr;
        assert(sysinfo);
        //Development only
        if (!global_state.no_file_change_check && isDevelopmentMode())
            sysinfo->fileAge = fileLastChanged( "bin/particles.bin" );
        //End developmetn only
    }

    //Failure in production mode == very bad data.

    // ## Development Only
    if ( !global_state.no_file_change_check && isDevelopmentMode() )
    {
        // Always prefer systems in the list
        {
            ParticleSystemInfo * dev_sysinfo;  //LIFO list means you always get the latest
            for(dev_sysinfo = particle_engine.systeminfos; dev_sysinfo ; dev_sysinfo = dev_sysinfo->next)
            {
                if( !_stricmp(part_name_cleaned_up, dev_sysinfo->name ) )
                {
                    sysinfo = dev_sysinfo;
                    break;
                }
            }
        }

        {
            char buf[500];
            //sprintf( buf, "%s%s", "fx/", part_name_cleaned_up );
            STR_COMBINE_BEGIN(buf);
            STR_COMBINE_CAT("fx/");
            STR_COMBINE_CAT(part_name_cleaned_up);
            STR_COMBINE_END();

            //Reload the sysinfo from the file under these conditions
            if( !sysinfo || //it's brand new sysinfo
                (sysinfo && fileHasBeenUpdated( buf, &sysinfo->fileAge )) )  //the sysinfo file itself has changed
            {
                sysinfo = partLoadSystemInfo(buf);
            }
        }
    }
    //##End development only

    if( sysinfo )
    {
        if ( !sysinfo->initialized )
        {
            partInitSystemInfo(sysinfo);
            //if( !partInitSystemInfo(sysinfo) ) //doesn't return anything, so don't
            //{
            //    Errorf( "Error in file %s", part_name_cleaned_up );
            //    return 0;
            //}
            sysinfo->initialized = 1;
        }
        //Development Only -- initialize the fileAge to current time
        if (!global_state.no_file_change_check && isDevelopmentMode())
        { //development mode initialization
            char buf[500];
            sysinfo->fileAge = 0;
            //sprintf( buf, "%s%s", "fx/", part_name_cleaned_up );
            STR_COMBINE_BEGIN(buf);
            STR_COMBINE_CAT("fx/");
            STR_COMBINE_CAT(part_name_cleaned_up);
            STR_COMBINE_END();
            fileHasBeenUpdated( buf, &sysinfo->fileAge );
        }
        //end development only
    }
    else
    {
        printToScreenLog( 1, "PART: Failed to get system %s", system_name );
    }

    // ## Developnment only: if part fails for any reason, try to remove it from the run time load list
    if( !sysinfo )
    {
        ParticleSystemInfo * checkersys;
        ParticleSystemInfo * checkersys_next;
         for( checkersys = particle_engine.systeminfos ; checkersys ; checkersys = checkersys_next )
        {
            checkersys_next = checkersys->next;
            if( !checkersys->name || !_stricmp( part_name_cleaned_up, checkersys->name ) )
            {
                listFreeMember( checkersys, &particle_engine.systeminfos );
                particle_engine.num_systeminfos--;
                assert( particle_engine.num_systeminfos >= 0 );
            }
        }
    }
    //## end development only

    if( !sysinfo && (game_state.fxdebug & FX_DEBUG_BASIC) )
        Errorf("FXGEO: Particle system %s doesn't exist. Tell Doug", system_name);


    return sysinfo;
}


// #########################################################
//#############################################################
//Tools for Fx to use to monkey with particle systems.

void partToggleDrawing( ParticleSystem * system, int id, int toggle )
{
    if( partConfirmValid(system, id ) )
    {
        system->parentFxDrawState = toggle;
    }
}

void partSetTeleported( ParticleSystem * system, int id, int teleported )
{
    if( partConfirmValid(system, id ) )
    {
        system->teleported = teleported;
    }
}


void partSetInheritedAlpha( ParticleSystem * system, int id, U8 inheritedAlpha )
{
    if( partConfirmValid(system, id ) )
    {
        system->inheritedAlpha = inheritedAlpha;
    }
}

//currently unused, should have ID?
int partGetID( ParticleSystem * system )
{
    return system->unique_id;
}

//currently unused, should have ID?
char * partGetName( ParticleSystem * system )
{
    return system->sysInfo->name;
}

//gets lots of non-debug use.
ParticleSystem * partConfirmValid( ParticleSystem * system, int id )
{
    if( system && system->inuse && system->unique_id == id )
    {
        return system;
    }
    return 0;
}


/*Replace the current system's sysInfo with the new sysInfo. Not used but will be.
*/
ParticleSystemInfo * partGiveNewParameters( ParticleSystem * system, int id, char * sysinfoname )
{
    ParticleSystemInfo * sysInfo = 0;
    F32 power_scale; //power_scale;

    assert( system->power ); //system->power needs to be between 1 and 10

    if( partConfirmValid(system, id) && sysinfoname )
    {
        sysInfo    = partGetSystemInfo(sysinfoname);
        if( sysInfo )
        {
            //TO DO add the new stuff
            system->sysInfo            = sysInfo;
            system->kickstart        = sysInfo->kickstart;
            system->kill_on_zero    = sysInfo->kill_on_zero;

            power_scale = (F32)system->power / PARTICLE_POWER_RANGE;
            if( power_scale < 0.f && power_scale > 1.0f )
            {
                Errorf( "Invalid Power Scale %f in particle system: %s", power_scale, sysinfoname );
                if(power_scale < 0)
                    power_scale = 0.f;
                if(power_scale > 1.f )
                    power_scale = 1.f;
            }

            if(1) //Turn on or off power scaling on particles.  TO DO: ditch if when it's proven stable
            {
                system->new_per_frame    = interpF32( power_scale, sysInfo->new_per_frame[1], sysInfo->new_per_frame[0] );
                system->burbleamplitude = interpF32( power_scale, sysInfo->burbleamplitude[1], sysInfo->burbleamplitude[0] );
                system->burst            = interpInt( power_scale, sysInfo->burst[1], sysInfo->burst[0] );
                system->alpha            = interpInt( power_scale, sysInfo->alpha[1], sysInfo->alpha[0] );
                system->startsize       = interpF32( power_scale, sysInfo->startsize[1], sysInfo->startsize[0] );
                system->endsize         = interpF32( power_scale, sysInfo->endsize[1], sysInfo->endsize[0] );
                interpVec3( power_scale, &sysInfo->initial_velocity[3], &sysInfo->initial_velocity[0], system->initial_velocity );
                interpVec3( power_scale, &sysInfo->initial_velocity_jitter[3], &sysInfo->initial_velocity_jitter[0],system->initial_velocity_jitter );
                interpVec3( power_scale, &sysInfo->emission_start_jitter[3], &sysInfo->emission_start_jitter[0], system->emission_start_jitter );
            }
            else
            {
                system->new_per_frame   = sysInfo->new_per_frame[0]; //PSCALEO correct this to divide and stuff
                system->burbleamplitude    = sysInfo->burbleamplitude[0];
                system->burst            = sysInfo->burst[0];
                system->alpha            = (U8)sysInfo->alpha[0];
                system->startsize       = sysInfo->startsize[0];
                system->endsize         = sysInfo->endsize[0];
                copyVec3( sysInfo->initial_velocity,        system->initial_velocity        );
                copyVec3( sysInfo->initial_velocity_jitter, system->initial_velocity_jitter );
                copyVec3( sysInfo->emission_start_jitter,    system->emission_start_jitter   );
            }
        }
    }
    return sysInfo;
}

/*Set the coordinate (currently only world space) for the particle system magnet
TO DO: figure out how this can be useful for local systems.
*/
void partSetMagnetPoint( ParticleSystem * system, int id, Vec3 point )
{
    if( partConfirmValid(system, id) && point )
    {
        if(system->sysInfo->worldorlocalposition == PARTICLE_WORLDPOS)
        {
            copyVec3( point, system->magnet );
        }
        else //PARTICLE_LOCALPOS
        {
            Mat4 mat;
            invertMat4Copy(system->drawmat,mat);
            mulVecMat4(point,mat,system->magnet);
            //void mulVecMat4Transpose(Vec3 vin, Mat4 m, Vec3 vout);
            //copyVec3( mat[3], system->magnet );
        }
    }
}

/*Set the coordinate (currently only world space) for the particle system other
*/
void partSetOtherPoint( ParticleSystem * system, int id, Vec3 point )
{
    if( partConfirmValid(system, id) && point )
    {
        if(system->sysInfo->worldorlocalposition == PARTICLE_WORLDPOS)
        {
            copyVec3( point, system->other );
        }
        else //PARTICLE_LOCALPOS
        {
            Mat4 mat;
            invertMat4Copy(system->drawmat,mat);
            mulVecMat4(point,mat,system->other);
            //void mulVecMat4Transpose(Vec3 vin, Mat4 m, Vec3 vout);
            //copyVec3( mat[3], system->magnet );
        }
    }
}

/*Set the particle system's matrix.
*/
int partSetOriginMat( ParticleSystem * system, int id, Mat4 mat )
{
    if( partConfirmValid(system, id) )
    {
        copyMat4( mat, system->drawmat );
        return 1;
    }
    return 0;
}

//End tools ######################################################################

static int partCompareSysDist(ParticleSystem * sysOne, ParticleSystem * sysTwo)
{
    if(sysOne->sortVal > sysTwo->sortVal)
        return -1;
    if(sysOne->sortVal < sysTwo->sortVal)
        return 1;

    //If same, just be consistant. DO NOT CHANGE, the artists use the order particle systems are created to set their draw priority.
    if(sysOne->unique_id < sysTwo->unique_id)
        return 1;
    else
        return -1;
}

static int partCompareSysDistAdapter(void* arg0, void* arg1)
{
    return partCompareSysDist((ParticleSystem *)arg0, (ParticleSystem *)arg1);
}

static ParticleSystem * partSortByDistance(ParticleSystem * firstsystem)
{
    ParticleSystem * system;
    ParticleSystem * newFirst;
    Vec3 temp;

    PERFINFO_AUTO_START("partSortByDistance",1);

    //1 Find distances from camera
    for(system = firstsystem ; system ; system = system->next)
    {
        if (system->gfxnode) {
            if (system->gfxnode->viewspace) {
                system->camDistSqr = SQR(system->gfxnode->viewspace[3][2]);
            }
        } else {
            //OLD
            subVec3( cam_info.cammat[3], system->drawmat[3], temp );
            system->camDistSqr = lengthVec3Squared(temp);
            system->camDistSqr -= system->sysInfo->tighten_up;
            //End OLD

            /*/NEW
            if( system->sysInfo->worldorlocalposition == PARTICLE_LOCALPOS )
                mulVecMat4( particleMid, system->drawmat, partMidWorldSpace );
            else //sysInfo->worldorlocalposition == PARTICLE_WORLDPOS
                copyVec3( particleMid, partMidWorldSpace );
            //END NEW*/
        }
        system->sortVal = system->camDistSqr - system->sysInfo->sortBias;
    }

    newFirst = listInsertionSort(firstsystem, partCompareSysDistAdapter);

    PERFINFO_AUTO_STOP();

    return newFirst;
}

// System ##############################################################

#define MAX_PARTICLE_POOL_MEMBERS 256

typedef struct PartPoolMember
{
    ParticleSystem * particles;
    int count;
    F32    timeLastUsed;
} PartPoolMember;

PartPoolMember particlePool[MAX_PARTICLE_POOL_MEMBERS];
int particlePoolCount;



static ParticleSystemInfo dummySysInfo = {0};
ParticleSystem *partCreateDummySystem(void)
{
    ParticleSystem * system;
    int    alloced;

    system = partAddSystemNodes(1, &alloced);
    system->sysInfo = &dummySysInfo;

    return system;
}

static bool s_complainedAboutAlloc = false;

/*Get the sysinfo, load a system, and attach the two, and init
and set up the faclitator arrays for OGL (maybe unnecessary).  Id is filled
with the system ID for later identification
*/
static F32 oneOver255 = 1.0/255.;
ParticleSystem * partCreateSystem(char system_name[], int * id, int kickstart, int power, int fxtype, F32 animScale)
{
    ParticleSystemInfo * sysInfo;
    ParticleSystem * system;
    int    alloced;

    if( game_state.noparticles )
        return 0;

    PERFINFO_AUTO_START("partCreateSystem", 1);

        //Alloc a system to use, and get an id
        system = partAddSystemNodes(1, &alloced);
        if( !system || !alloced)
        {
            //too many particles debug
            if (isDevelopmentMode() && !s_complainedAboutAlloc)
            {
                s_complainedAboutAlloc = true;
                Errorf( "Can't alloc %s. System Count %d Particle Count %d", system_name, particle_engine.num_systems, particle_engine.particleTotal );
            }
            //end debug
            PERFINFO_AUTO_STOP();
            return 0;
        }

        if( particle_engine.system_unique_ids == -1 )
            particle_engine.system_unique_ids++;
        system->unique_id      = particle_engine.system_unique_ids++;
        if(id)
            *id = system->unique_id;
        system->power = power;
        if( !system->power )
            system->power = 10;

        system->inheritedAlpha = 255;
        system->animScale = animScale;

        system->fxtype = fxtype;

        //Get the system an info for it
        sysInfo = partGiveNewParameters( system, system->unique_id, system_name );
        if( !sysInfo )
        {
            printToScreenLog( 1, "PART: Can't find %s", system_name );
            if (isDevelopmentMode() || (playerPtr() && playerPtr()->access_level > 0))
                Errorf( "PART: Can't find %s", system_name );
            partKillSystem(system, system->unique_id);
            PERFINFO_AUTO_STOP();
            return 0;
        }

        //Set just created parameters.
        system->kickstart |= kickstart;
        copyMat4(unitmat, system->drawmat) ;
        system->texarray    = particle_engine.texarray;
        system->tris        = particle_engine.tris;
        system->verts        = particle_engine.verts;
        system->rgbas        = particle_engine.rgbas;

        system->maxParticles = system->burst + ( system->new_per_frame * system->sysInfo->fade_out_by ) + 10;
        //Two hacks
        if(system->sysInfo->movescale)
            system->maxParticles += 200;
        if(system->burbleamplitude )
            system->maxParticles += 200;
        //Replace with smarter system

        system->maxParticles = MIN( system->maxParticles, MAX_PARTSPERSYS );

        system->emission_life_span = MAX( sysInfo->emission_life_span + qfrand() * sysInfo->emission_life_span_jitter, 0.0f );

        #if USE_VH_HEAP
            vhAlloc(&system->particles, system->maxParticles * sizeof(Particle), 0);
        #else
            system->particles = malloc( system->maxParticles * sizeof(Particle) );
        #endif
        //TO DO, most particle systems are under 20 particles as it turns out, I should have a mem pool for them.

        {
            system->myVertArrayId = 0;
        }

        //if( !system->myVertArrayId )
        //    system->maxParticles = MAX_PARTSFORLITTLESYS;

        if ( !vec3IsZero(system->sysInfo->colorOffset) || !vec3IsZero(system->sysInfo->colorOffsetJitter) )
        {
            // Make a copy of the color path, but add the coloroffset.
            int iOurOffset[3];
            int i;

            for (i=0;i<3; ++i)
                iOurOffset[i] = round(system->sysInfo->colorOffset[i] + qfrand()*system->sysInfo->colorOffsetJitter[i]);

            system->colorPathCopy.length = system->sysInfo->colorPathDefault.length;
            system->colorPathCopy.path = malloc(sizeof(U8) * 3 * system->colorPathCopy.length );

            for (i=0; i<system->colorPathCopy.length; ++i )
            {
                system->colorPathCopy.path[i*3 + 0] = (U8)(CLAMP((int)system->sysInfo->colorPathDefault.path[i*3 + 0] + iOurOffset[0], 0, 255 ));
                system->colorPathCopy.path[i*3 + 1] = (U8)(CLAMP((int)system->sysInfo->colorPathDefault.path[i*3 + 1] + iOurOffset[1], 0, 255 ));
                system->colorPathCopy.path[i*3 + 2] = (U8)(CLAMP((int)system->sysInfo->colorPathDefault.path[i*3 + 2] + iOurOffset[2], 0, 255 ));
            }

            system->effectiveColorPath = &system->colorPathCopy;
        }
        else // default path, nothing funky going on
        {
            system->effectiveColorPath = &system->sysInfo->colorPathDefault;
            system->colorPathCopy.length = 0;
            system->colorPathCopy.path = NULL;
        }

    PERFINFO_AUTO_STOP();

    return system;  //return system pointer
}


F32 noise(int x)
{
    x = (x<<13) ^ x;
    return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0) ;
}


F32 cosineInterpolate(F32 a, F32 b, F32 x)
{
    F32 ft, f;
    ft = x * 3.1415927;
    f = (1 - cos(ft)) * .5;
    return  a*(1-f) + b*f;
}

F32 linearInterpolate(F32 a, F32 b, F32 x)
{
    return  a*(1-x) + b*x;
}

F32 smoothedNoise(float x)
{
    return noise(x)/2  +  noise(x-1)/4  +  noise(x+1)/4;
}

F32 interpolatedNoise(float x)
{
    F32 v1, v2, fracx;
    int intx;

    intx = (int)x;
    fracx = (F32)( x - intx );

    //v1 = smoothedNoise(intx);
    //v2 = smoothedNoise(intx + 1);
    v1 = noise(intx);
    v2 = noise(intx + 1);

    return linearInterpolate(v1 , v2 , fracx);
}


/*
  function PerlinNoise_1D(float x)

      total = 0
      p = persistence
      n = Number_Of_Octaves - 1

      loop i from 0 to n

          frequency = 2i
          amplitude = pi

          total = total + InterpolatedNoisei(x * frequency) * amplitude

      end of i loop

      return total

  end function*/


#include "gridcoll/gridcoll.h"
void adjustForTerrain(Vec3 position)
{
    Vec3 start;
    Vec3 end;
    CollInfo coll;
    int flag = 0;
    Entity *player = playerPtr();

    flag |= COLL_DISTFROMSTART;

    copyVec3(position, start);
    start[1] += 10;
    copyVec3(position, end);
    end[1] -= 10;
    collGrid(0, start, end, &coll, 0, flag);

    copyVec3(coll.mat[3], position);

}

static contrapulate( F32 u, Vec3 a, Vec3 b, Vec3 c, Vec3 d, Vec3 r)
{
    Vec3 a1, b1, c1, d1;
    scaleVec3( a, (-0.5*u*u*u + u*u - 0.5*u), a1 );
    scaleVec3( b, ( 1.5*u*u*u + -2.5*u*u + 1.0), b1 );
    scaleVec3( c, (-1.5*u*u*u + 2.0*u*u + 0.5*u), c1 );
    scaleVec3( d, ( 0.5*u*u*u - 0.5*u*u), d1);

    addVec3(a1, b1, r);
    addVec3(r, c1, r);
    addVec3(r, d1, r);
}


/*
int partDebugNewAllocing( ParticleSystem * system )
{
    F32            last_age; //debug
    int            i, partIdx;
    Particle    * particle;

    if(!( !system->particle_count || (system->particles[system->firstParticle].flags & PARTICLE_INUSE )))
        Errorf("Crrap3");
    if( system->particle_count )
    {
        last_age = system->particles[system->firstParticle].age;
        for( i = 0, partIdx = system->firstParticle ; i < system->particle_count ; i++, partIdx++ )
        {
            if( partIdx == system->maxParticles )
                partIdx = 0;
            particle = &system->particles[partIdx];
            if(!( (particle->flags & PARTICLE_INUSE) && i < system->maxParticles ))
                Errorf("Crrap4");
            if( particle->age > last_age )
                Errorf("BUG");
            if( particle->age > last_age )
                Errorf("Dang");

            last_age = particle->age;

            if( !(particle->flags & PARTICLE_INUSE && i < system->maxParticles) )
                Errorf("OOPS");

            if( !(particle->flags & PARTICLE_INUSE && i < system->maxParticles) )
                Errorf("OOPS");
        }
    }
    else
    {
        for( i = 0 ; i < system->particle_count ; i++ )
        {
            if((system->particles[system->firstParticle].flags & PARTICLE_INUSE) )
                Errorf("Crap5");
        }
    }
    return 1;
}*/

//A performance hack, dangerous mimicing of partUpdateSystem logic, maybe merge into one function?
static int partCheckForDeath(ParticleSystem * system, F32 timestep, F32 clampedTimestep)
{
    int still_alive;
    ParticleSystemInfo  * sysInfo;

    sysInfo = system->sysInfo;
    if (system->animScale!=1.0) {
        timestep *= system->animScale;
    }

    system->timeSinceLastUpdate += timestep;

    still_alive = 1;

    //don't worry about the burst cuz the timeToFull will take care of it, but kind of dangerous, maybe consider,
    //TO DO : Should the system age while in suspension?
    if( system->dying || ( !system->new_per_frame && !sysInfo->movescale && !system->burbleamplitude ) )
    {
        if( system->particle_count == 0 )
            still_alive =  0;
        if( system->timeSinceLastUpdate >= sysInfo->fade_out_by )
            still_alive = 0;
    }

    return still_alive;
}

static void advanceAllParticlesToFadeOut( ParticleSystem * system )
{
    Particle * particle;
    int count, partIdx, i;

    count = system->particle_count;  //because system->particle_count can change during loop
    for( i = 0, partIdx = system->firstParticle ; i < count ; i++, partIdx++ )
    {
        if( partIdx == system->maxParticles )
            partIdx = 0;
        particle = &system->particles[partIdx];

        particle->age = MAX( particle->age, system->sysInfo->fade_out_start );

    }
}


static int partUpdateSystem(ParticleSystem * system, F32 timestep, F32 clampedTimestep)
{
    int            i, num_new,frame,maxParticles, count, partIdx;
    F32            gravity,magnetism,expandrate,currpart = 0,new_buffer,size,drag;
    F32            endsize,startsize,oldsize;
    Vec3        move,emission_start,initial_velocity;
    Vec3        initial_velocity_jitter,emission_start_jitter;
    F32            tempf1, tempf2;
    int            tempi1;

    Particle  *    particle;
    ParticleSystemInfo  * sysInfo;
    PartTex   * parttex;

    Vec3 systemPosChange;

    sysInfo = system->sysInfo;

    maxParticles = system->maxParticles;

    system->perf.totalSize = 0;
    system->perf.totalAlpha = 0;

    if (system->animScale!=1.0) {
        timestep *= system->animScale;
        clampedTimestep *= system->animScale;
    }

    ///#############  SET PARAMETERS  ####################################################
    /*facingmat = what you multiply the particle's position by each frame     */

    /*find positions in world space, transform positions by the cammat
    to make frontfacing.  Then give GL:
    MODELVIEW:  unit mat
    CAMERAVIEW: unit mat.
    */
    if( sysInfo->worldorlocalposition == PARTICLE_WORLDPOS && sysInfo->frontorlocalfacing == PARTICLE_FRONTFACING )
    {
        copyMat4(cam_info.viewmat, system->facingmat);
        copyVec3(system->drawmat[3], system->emission_start);
        mulVecMat3(system->initial_velocity, system->drawmat, initial_velocity );
    }

    /*find positions in local space, then transform positions by local mat and
    camera mat to make frontfacing.  Then give GL:
    MODELVIEW:  unit mat
    CAMERAVIEW: unit mat.
    */
    else if( sysInfo->worldorlocalposition == PARTICLE_LOCALPOS && sysInfo->frontorlocalfacing == PARTICLE_FRONTFACING )
    {
        mulMat4(cam_info.viewmat, system->drawmat, system->facingmat);
        zeroVec3(system->emission_start);
        copyVec3(system->initial_velocity, initial_velocity);
    }

    /*find positions in world space, then transform position back to local space to make front
    facing.  Them give GL:
    MODELVIEW: local mat
    CAMERAMAT: camera mat
    */
    else if( sysInfo->worldorlocalposition == PARTICLE_WORLDPOS && sysInfo->frontorlocalfacing == PARTICLE_LOCALFACING )
    {
        Mat4 mat4;

        copyMat4(system->drawmat, mat4);
        pitchMat3(RAD(90), mat4);
        invertMat4Copy(mat4,system->facingmat);

        copyVec3(system->drawmat[3], system->emission_start);
        mulVecMat3(system->initial_velocity, system->drawmat, initial_velocity       );
    }

    /*find positions and frontfacing in local space, then give GL:
    MODELVIEW: local mat
    CAMERAMAT: camera mat
    */
    else if( sysInfo->worldorlocalposition == PARTICLE_LOCALPOS && sysInfo->frontorlocalfacing == PARTICLE_LOCALFACING )
    {
        copyMat4(unitmat, system->facingmat);
        pitchMat3(RAD(-90), system->facingmat);

        copyVec3(zerovec3, system->emission_start);
        copyVec3(system->initial_velocity, initial_velocity);
    }
    /*Do everything the old, dumb way.  This may never come up.
    */
    else
    {
        copyMat4(cam_info.viewmat, system->facingmat);
        copyVec3(system->drawmat[3], system->emission_start);
        copyVec3(system->initial_velocity, initial_velocity);
        copyVec3(system->initial_velocity_jitter, initial_velocity_jitter);
        copyVec3(system->emission_start_jitter, emission_start_jitter);
    }

    if( system->age == 0.0 || system->teleported )
        copyVec3(system->emission_start, system->emission_start_prev);

    subVec3( system->emission_start, system->emission_start_prev, systemPosChange );
    scaleVec3(systemPosChange,sysInfo->stickiness,system->stickinessVec);

    //Note, if you want this right, you've got to per particle do the init vel jitter, *then* xform the init veloity by the drawmat (or copy)
    copyVec3(system->initial_velocity_jitter, initial_velocity_jitter);
    copyVec3(system->emission_start_jitter, emission_start_jitter);

    //Set Scaled Parameters
    magnetism  = sysInfo->magnetism        * timestep;
    gravity    = sysInfo->gravity        * timestep;
    expandrate = sysInfo->expandrate    * timestep; //clampedTimestep?
    drag       = sysInfo->drag            * timestep;

    endsize       = system->endsize;
    startsize  = system->startsize;

    copyVec3(system->emission_start, emission_start);

    //Info for deciding whether to draw this system at all
    copyVec3(emission_start, system->perf.boxMax);
    copyVec3(emission_start, system->perf.boxMin);
    system->perf.biggestParticleSquared = 0;
    system->perf.totalAlpha = 0;

    //Calculate the new age and new number of particles/new buffer
    if(!sysInfo->movescale || !system->age)
    {
        new_buffer = system->new_per_frame * clampedTimestep;
    }
    else if( !system->dying ) //dont do movescale on a dying system
    {
        F32 len;
        len = lengthVec3(systemPosChange);

        if( sysInfo->movescale > 0 )
        {
            new_buffer = system->new_per_frame * (sysInfo->movescale * len);
            //xyprintf( 10, 10, "NewBuffer %f len %f movescale %f", new_buffer, len,  sysInfo->movescale);
        }
        else
        {
            new_buffer = system->new_per_frame * clampedTimestep;
            tempf1 = sysInfo->movescale * len;
            new_buffer += MAX(-new_buffer, tempf1);
        }
    }
    else
        new_buffer = 0;

    if(system->age == 0) //system->age
    {
        new_buffer +=  system->burst;
    }

    if(system->burbleamplitude)
    {
        F32 time_x, result_y;

        time_x = system->age / sysInfo->burblefrequency;

        if( sysInfo->burbletype == PARTICLE_RANDOMBURBLE )
        {
            result_y = interpolatedNoise(time_x/64);
        }
        else //burbletype == PARTICLE_REGULARBURBLE
        {
            result_y = sintable[ ((int)time_x)%256 ];
        }
        if( result_y >= sysInfo->burblethreshold  )  //result_y range == -1.0 to 1.0
            new_buffer += (result_y + 1 ) * system->burbleamplitude * clampedTimestep;
    
    }

    system->new_buffer    += new_buffer;
    num_new                   = (int)system->new_buffer; //get biggest whole number
    system->new_buffer    -= (float)num_new;          //keep remainder for next frame

    //printf( "NumNew %d PosChange %f %f %f\n", num_new, systemPosChange[0], systemPosChange[1], systemPosChange[2] );

    //Advance the Animation/Texture Scrolling //TO DO go over this
    for(i = 0 ; i < 2 ; i++ )
    {
        parttex = &sysInfo->parttex[i];
        if( parttex->animframes > 1.0)// && (system->framecurr[i] < parttex->animframes) )
        {
            system->framecurr[i] += parttex->animpace * timestep;
            
            if( parttex->animtype == PARTICLE_ANIM_LOOP )
            {
                if(system->framecurr[i] >= parttex->animframes)
                    system->framecurr[i] -= parttex->animframes;
            }
            else if (parttex->animtype == PARTICLE_ANIM_ONCE )
            {
                if(system->framecurr[i] >= parttex->animframes)
                    system->framecurr[i] = parttex->animframes - 1;
            }
            else if( parttex->animtype == PARTICLE_ANIM_PINGPONG )
            {
                if(system->framecurr[i] >= parttex->animframes)
                {
                    system->framecurr[i] -= parttex->animframes;
                    system->animpong = system->animpong == 0 ? 1 : 0;
                }
            }    
            
            if( system->animpong )
                frame = (int)( parttex->animframes - system->framecurr[i] );
            else
                frame = (int)system->framecurr[i];
            
            system->textranslate[i][0] = (frame%parttex->framewidth) * parttex->texscale[0];
            system->textranslate[i][1] = (frame/parttex->framewidth) * parttex->texscale[1];
        }

        // Jitter
        {
            Vec3 vScrollThisFrame;
            partVectorJitter(parttex->texscroll, parttex->texscrolljitter, vScrollThisFrame);
            system->textranslate[i][0] += vScrollThisFrame[0] * timestep;
            system->textranslate[i][1] += vScrollThisFrame[1] * timestep;
        }
    }

    ///##### EMIT NEW PARTICLES  ######################################################### 
    
    if( system->particle_count + num_new > maxParticles )//Speed or burble or frozen framerate can make this happen
    {
        tempf1 = maxParticles - system->particle_count;
        num_new = MAX(tempf1, 0);
    }

    if ( system->emission_life_span > 0.0f &&  system->age > system->emission_life_span )
        num_new = 0;


    //Global max
    if( num_new && (num_new + particle_engine.particleTotal) > game_state.maxParticles ) {
        // We want at least 1 particle per system, always, 2 in the case of entities
#define MINIMUM_PARTS_PER_SYSTEM ((system->fxtype == FX_ENTITYFX)?2:1)
// debug/explicit version:
//        int ok_new = MAX(game_state.maxParticles - particle_engine.particleTotal, 0);
//        int required_allowed_new = MAX(MINIMUM_PARTS_PER_SYSTEM - system->particle_count, 0); 
//        int eff_req_new = MIN(required_allowed_new, num_new);
//        num_new = MAX(ok_new, eff_req_new);
// inlined:
        tempf1 = game_state.maxParticles - particle_engine.particleTotal;
        MAX1(tempf1, 0);
        tempf2 = MINIMUM_PARTS_PER_SYSTEM - system->particle_count;
        MAX1(tempf2, 0);
        MIN1(tempf2, num_new);
        num_new = MAX(tempf1, tempf2);
    }

    system->particle_count += num_new;
    particle_engine.particleTotal += num_new;

    for(i = 0 ; i < num_new ; i++)
    {
        particle = &system->particles[ system->firstEmptyParticleSlot++ ]; //NEW ALLOCING
        if( system->firstEmptyParticleSlot == maxParticles ) //wrap around
            system->firstEmptyParticleSlot = 0;

        //Initialize Particle Parameters (note all params need to be given a value as particle is no zeroed on alloc.
        particle->age        = 0.0;
        particle->theta        = 0; 
        particle->thetaAccum = 0;
        particle->pulsedir    = 1;
    
        particle->size        = system->startsize; 
        if(sysInfo->startsizejitter)
            particle->size += qfrand() * sysInfo->startsizejitter;
    
        particle->spin        = sysInfo->spin;
        if(sysInfo->spin_jitter)
        {
            particle->spin += qsirand()%(sysInfo->spin_jitter+1);
        }

        if(sysInfo->orientation_jitter)
            particle->theta += qsirand()%(sysInfo->orientation_jitter+1);
    
         partVectorJitter(initial_velocity, initial_velocity_jitter, particle->velocity);
        //if I want to do vector jitter right, I need to x form by velocity now, not earlier
        
        //Get Initial Position //TO DO figure out how local can use these
        if(sysInfo->emission_type == PARTICLE_POINT)
        {
            copyVec3(emission_start, particle->position); 
        }
        else if(sysInfo->emission_type == PARTICLE_LINE)
        {
            getRandomPointOnLine(emission_start, system->other, particle->position);
        }
        else if(sysInfo->emission_type == PARTICLE_CYLINDER)
        {
            getQuickRandomPointOnCylinder(emission_start, sysInfo->emission_radius, sysInfo->emission_height, particle->position);
        }
        else if(sysInfo->emission_type == PARTICLE_SPHERE)
        {
            getRandomPointOnSphere(emission_start, sysInfo->emission_radius, particle->position);
        }
        else if (sysInfo->emission_type == PARTICLE_TRAIL)
        {
            F32 percent = (1.0 - ((F32)i / (F32)num_new));  
            //TO DO this can be sped up, by finding the diff and then adding it to each particle
            particle->position[0] = emission_start[0] - (systemPosChange[0] * percent); 
            particle->position[1] = emission_start[1] - (systemPosChange[1] * percent);
            particle->position[2] = emission_start[2] - (systemPosChange[2] * percent);

            //printf( "         Percent %f pos %f %f %f %f\n", percent, particle->position[0],particle->position[1],particle->position[2] );
        }
        else if(sysInfo->emission_type == PARTICLE_LINE_EVEN)
        {
            //TO DO nake this work like PARTICLETRAIL. 
             particle->position[0] = emission_start[0] + ((system->other[0] - emission_start[0] )/num_new) * i;
            particle->position[1] = emission_start[1] + ((system->other[1] - emission_start[1] )/num_new) * i;
            particle->position[2] = emission_start[2] + ((system->other[2] - emission_start[2] )/num_new) * i;
    
            /*  Hack in a Noise function
                if(system->age == 0)
                    copyVec3(emission_start, particle->position); 
                else
                {
                Vec3 a, b, c, d,temp1, temp2;
                //Vec3 r;
                Vec3 len;
                subVec3(emission_start, system->emission_start_prev, len);
                 
                temp1[0] =  1.0 , temp1[1] = 0 , temp1[2] = -1.0 * lengthVec3(len); 
                temp2[0] =  -1.0 , temp2[1] = 0 , temp2[2] = -1.0 * lengthVec3(len);

                mulVecMat4(temp1, system->drawmat, a);
                copyVec3(system->drawmat[3], b);
                copyVec3(system->lastdrawmat[3], c);
                //copyVec3(unitmat[3], c);
                mulVecMat4(temp2, system->lastdrawmat, d);
                //mulVecMat4(temp, unitmat, d);

                contrapulate( ((1.0/num_new) * (F32)i), a, b, c, d, particle->position);
                xyprintf(8,8, "%f , %f, %f", particle->position[0],  particle->position[1], particle->position[2]);        
                }
            */
        }
        else 
            assert(0);

        partVectorJitter(particle->position, emission_start_jitter, particle->position);
    }

    if( system->advanceAllParticlesToFadeOut )
    {
        system->advanceAllParticlesToFadeOut = 0;
        advanceAllParticlesToFadeOut( system );
    }

    ///##### UPDATE ALL PARTICLES  #########################################################
    count = system->particle_count;  //because system->particle_count can change during loop
    for( i = 0, partIdx = system->firstParticle ; i < count ; i++, partIdx++ )
    {    
        if( partIdx == system->maxParticles )
            partIdx = 0;
        particle = &system->particles[partIdx]; 

        //Kill old particles then Update live ones and write them to vert array
        if( particle->age >= sysInfo->fade_out_by )  //KILL  
        {
            system->firstParticle++;
            if( system->firstParticle >= system->maxParticles )
                system->firstParticle = 0;
            system->particle_count--;
            particle->age = -1; //debug
        }                                    
        else                                 
        {
            //************** Update fade and kill invisible ones
            {
                F32 alphaPercent;
    
                //TO DO : a particle doesn't really even need an alpha value now,
                //I could move all this to the post visibility updater.  A system can know it's average
                //at any time automatically...

                if(    particle->age >= sysInfo->fade_out_start ) //Im fading out
                {
                    F32 timeFading, timeToFade;

                    timeFading = particle->age - sysInfo->fade_out_start;
                    timeToFade = sysInfo->fade_out_by - sysInfo->fade_out_start;

                    alphaPercent = 1.0 - ( timeFading / timeToFade );
                }
                else if( particle->age < sysInfo->fade_in_by )  //Im fading in
                {
                    alphaPercent = particle->age / sysInfo->fade_in_by;
                }
                else //Im not fading anywhere
                {
                    alphaPercent = 1.0;
                }
                tempi1 = round(((F32)system->alpha) * alphaPercent);
                particle->alpha = MAX( 1, tempi1);  
            }

            //************ Update Absolute position and velocity
            if (sysInfo->has_velocity_jitter)
                partVectorJitter(particle->velocity, sysInfo->velocity_jitter, particle->velocity); 
            particle->velocity[1] -= gravity;
            if(drag)
            {
                particle->velocity[0] -= particle->velocity[0] * drag;
                particle->velocity[1] -= particle->velocity[1] * drag;
                particle->velocity[2] -= particle->velocity[2] * drag;
            }

            scaleVec3(particle->velocity, timestep, move);
            addVec3(particle->position, move, particle->position);    
            if(magnetism) 
                partRunMagnet(particle->velocity, particle->position, system->magnet, magnetism);

            if(sysInfo->terrain == PARTICLE_STICKTOTERRAIN)
                adjustForTerrain(particle->position); 

            if( sysInfo->stickiness && particle->age ) 
                addVec3( system->stickinessVec, particle->position, particle->position );

            //************ Update Size
            if(sysInfo->expandrate)
            {
                if( sysInfo->expandtype == PARTICLE_EXPANDFOREVER )
                {
                    particle->size += expandrate;
                }
                else if( sysInfo->expandtype == PARTICLE_EXPANDANDSTOP )
                {
                    particle->size += expandrate ;
                    if(sysInfo->expandrate < 0)
                        MAX1(particle->size, endsize);
                    if(sysInfo->expandrate > 0)
                        MIN1(particle->size, endsize);
                }
                else if(sysInfo->expandtype == PARTICLE_PINGPONG)
                {
                    // pulsedir is now encoded as a single bit
                    // so we need to treat pulsedir == 0 as == -1
                    int increasing = (particle->pulsedir);
                    int pulseSign = (particle->pulsedir)? 1 : -1;
                    oldsize = particle->size;
                    particle->size += (expandrate * pulseSign);
                    size = particle->size;
                    if (endsize > startsize) {
                        // growing
                        if (increasing && size > endsize  || // still growing but past end
                            !increasing && size < startsize) // shrunk past beginning
                        {
                            particle->size = oldsize;
                            particle->pulsedir = !(particle->pulsedir);
                        }
                    } else {
                        // shrinking
                        if (increasing && size > startsize  || // still growing but past end
                            !increasing && size < endsize) // shrunk past beginning
                        {
                            particle->size = oldsize;
                            particle->pulsedir = !(particle->pulsedir);
                        }
                    }
                }
            }

            if( system->currdrawstate == PART_UPDATE_AND_DRAW_ME )
            {
                //Build bounding box.  
                MIN1(system->perf.boxMin[0], particle->position[0]);
                MIN1(system->perf.boxMin[1], particle->position[1]);
                MIN1(system->perf.boxMin[2], particle->position[2]);

                MAX1(system->perf.boxMax[0], particle->position[0]);
                MAX1(system->perf.boxMax[1], particle->position[1]);
                MAX1(system->perf.boxMax[2], particle->position[2]);

                tempf1 = SQR(particle->size);
                MAX1(system->perf.biggestParticleSquared, tempf1); 
                system->perf.totalSize += tempf1;
                system->perf.totalAlpha += particle->alpha;
            }    

            particle->age += timestep;
        }
    }

    //Add the streak position to the bounding box 
    if( sysInfo->streaktype == PARTICLE_MAGNET || sysInfo->streaktype == PARTICLE_OTHER )
    {
        Vec3 streakpos;
        if(sysInfo->streaktype == PARTICLE_MAGNET)
            copyVec3(system->magnet, streakpos);
        else if(sysInfo->streaktype == PARTICLE_OTHER)
            copyVec3(system->other, streakpos);

        MIN1(system->perf.boxMin[0], streakpos[0]);
        MIN1(system->perf.boxMin[1], streakpos[1]);
        MIN1(system->perf.boxMin[2], streakpos[2]);

        MAX1(system->perf.boxMax[0], streakpos[0]);
        MAX1(system->perf.boxMax[1], streakpos[1]);
        MAX1(system->perf.boxMax[2], streakpos[2]);
    }    

    ///##### DO END UPDATING  #########################################################
    system->age += timestep;

    { //could clean this logic a little
        int keepAlive = 0;
        if( !system->dying && (system->burbleamplitude || sysInfo->movescale) )
            keepAlive = 1; //just to keep things moving
        if(system->particle_count == 0 && system->kill_on_zero && new_buffer <= 0.0 && !keepAlive )
            return 0; //kill system
        else
            return 1;
    }
}

const F32 cfOneOver127 = 0.007874015748031496062992125984252;  // 32 bit precision

void partBuildParticleArray( ParticleSystem * system, F32 total_alpha, F32 * vertarray, U8 * rgbaarray, F32 timestep )
{
    U8          *    rgb;
    int            idx = 0;
    Particle  *    particle;
    ParticleSystemInfo  * sysInfo;
    U8            theta,ur_theta,lr_theta,ll_theta,ul_theta;
    Vec3        part_pos_view,part_pos_view2,streakpos;
    Mat4        facingmat;
    F32            size;
    Vec3        emission_start;
    U8            alpha;
    int            i,  partIdx;
    Vec3        lastul = {0}; //zeroed, because compiler thought I was using initialized var below.
    Vec3        lastur = {0};
    int            isRibbon = 0;

    F32            *f_rgbas = (F32 *)rgbaarray;

    sysInfo = system->sysInfo;
    //rgb = sysInfo->colornavpoint[0].rgb;
    rgb = system->effectiveColorPath->path;
    copyMat4(system->facingmat, facingmat);
    copyVec3(system->emission_start, emission_start);

    isRibbon = system->sysInfo->flags & PART_RIBBON;
    //New Alloc
    //partDebugNewAllocing( system );

    for( i = 0, partIdx = system->firstParticle ; i < system->particle_count ; i++, partIdx++ )
    {    
        if( partIdx == system->maxParticles )
            partIdx = 0;

        particle = &system->particles[partIdx];
    //End

        size = particle->size;

        //************ Set up the Vectors in view space 
        // Get First Center Point:  part_pos_view
        mulVecMat4(particle->position, system->facingmat, part_pos_view);
        
        if (sysInfo->tighten_up != 0.0f)
        {
            if (sysInfo->frontorlocalfacing)
                part_pos_view[2] += sysInfo->tighten_up;
            else
            {
                F32 dx = part_pos_view[0] / part_pos_view[2];
                F32 dy = part_pos_view[1] / part_pos_view[2];
                F32 dz = 1;
                F32 lenSquared = dx*dx + dy*dy + dz*dz;
                if (lenSquared != 0.0f)
                {
                    F32 scale = sysInfo->tighten_up / sqrt(lenSquared);
                    part_pos_view[0] += dx * scale;
                    part_pos_view[1] += dy * scale;
                    part_pos_view[2] += dz * scale;
                }
            }
        }
        //  Get Second Center Point:  part_pos_view2
        if(sysInfo->streaktype == PARTICLE_NOSTREAK)
        {
            copyVec3(part_pos_view, part_pos_view2);
        }
        else 
        {    
            copyVec3(particle->position, streakpos);     
            //subVec3( streakpos, system->stickinessVec, streakpos );

            if(sysInfo->streaktype == PARTICLE_VELOCITY)
            {
                Vec3 streak;
                scaleVec3(particle->velocity, sysInfo->streakscale, streak);
                if(sysInfo->streakdirection == PARTICLE_PULL)
                    subVec3(streakpos, streak, streakpos);
                else // == PARTICLE_PUSH)
                    addVec3(streakpos, streak, streakpos);
            }
            else if (sysInfo->streaktype == PARTICLE_PARENT_VELOCITY)
            {
                FxGeo* parent = hdlGetPtrFromHandle(system->geo_handle);
                Vec3 streak;
                if ( parent )
                    addVec3(particle->velocity, parent->velocity, streak );
                else
                    copyVec3(particle->velocity, streak);
                scaleVec3(streak, sysInfo->streakscale, streak);
                if(sysInfo->streakdirection == PARTICLE_PULL)
                    subVec3(streakpos, streak, streakpos);
                else // == PARTICLE_PUSH)
                    addVec3(streakpos, streak, streakpos);
            }
            else if(sysInfo->streaktype == PARTICLE_GRAVITY)
            {
                streakpos[1] += sysInfo->gravity * sysInfo->streakscale;
            }
            else if(sysInfo->streaktype == PARTICLE_ORIGIN)
            {
                //Vec3 temp;
                //subVec3( streakpos, emission_start, temp );
                //scaleVec3( temp, 0.1, temp );
                //addVec3( streakpos, temp, streakpos );
                copyVec3(emission_start, streakpos);
            }
            else if(sysInfo->streaktype == PARTICLE_MAGNET)
            {
                copyVec3(system->magnet, streakpos);
            }
            else if(sysInfo->streaktype == PARTICLE_OTHER)
            {
                copyVec3(system->other, streakpos);
            }    
            else if(sysInfo->streaktype == PARTICLE_CHAIN)
            {
                //New Allocing makes getting prev particle a little tricky
                if( i < system->particle_count - 1 ) //If this isn't the last particle in the chain
                {
                    int prev = partIdx + 1;
                    if( prev == system->maxParticles )
                        prev = 0;
                    copyVec3(system->particles[prev].position, streakpos); 
                }
                else
                    copyVec3(emission_start, streakpos);
            }
            //else if(sysInfo->streaktype == PARTICLE_STRIP)
            //{
            //    if(particle->prev)
            //        copyVec3(
            //}
            
            mulVecMat4(streakpos, system->facingmat, part_pos_view2); 
            if (sysInfo->tighten_up != 0.0f)
            {
                if (sysInfo->frontorlocalfacing)
                    part_pos_view2[2] += sysInfo->tighten_up;
                else
                {
                    F32 dx = part_pos_view2[0] / part_pos_view2[2];
                    F32 dy = part_pos_view2[1] / part_pos_view2[2];
                    F32 dz = 1;
                    F32 lenSquared = dx*dx + dy*dy + dz*dz;
                    if (lenSquared != 0.0f)
                    {
                        F32 scale = sysInfo->tighten_up / sqrt(lenSquared);
                        part_pos_view2[0] += dx * scale;
                        part_pos_view2[1] += dy * scale;
                        part_pos_view2[2] += dz * scale;
                    }
                }
            }
        }

        //*************** Update Theta (Spin or Streak Orientation) and Set Up
        if(sysInfo->streaktype != PARTICLE_NOSTREAK && sysInfo->streakorient == PARTICLE_ORIENT)
        {
            if(sysInfo->streakdirection == PARTICLE_PUSH) //Switch the two vectors: tricky
            {
                Vec3 temp;
                copyVec3(part_pos_view2, temp); 
                copyVec3(part_pos_view, part_pos_view2);
                copyVec3(temp, part_pos_view);
            }
            theta = findAngle(part_pos_view2, part_pos_view); //TO DO quick atan
            theta-=64;
        }
        else if ( particle->spin != 0)
        {
            // This is kind of wierd. Due to asymmetry in the theta accumulator about zero,
            // it's easier to just always spin in the direction of higher numbers and
            // only flip the result for "theta", the value that actually determines
            // the drawing of the particle
            int iSign = SIGN(particle->spin);
            F32 fSpin = (F32)(abs(particle->spin)) * TIMESTEP * 0.1;
            F32 fThetaAccum = (F32)particle->thetaAccum * cfOneOver127;
            F32 fNewTheta = (F32)particle->theta + fThetaAccum + fSpin;
            F32 fNewThetaTrunc = qtrunc(fNewTheta);

            fThetaAccum = fNewTheta - fNewThetaTrunc;

            // map the theta accum (from 0 to 1) to 7 bits
            particle->thetaAccum = (U8)(CLAMP(fThetaAccum * 127,0,127));

            particle->theta = (U8)(fNewThetaTrunc);

            // for display, round to minimize error and thus jitter
            theta = round(fNewTheta);

            // if the sign is positive, flip the theta, effectively reversing direction
            if ( iSign > 0 )
                theta = 255 - theta;
        }
        else
        {
            theta = particle->theta;
        }

        //*************** Do per frame writing to the vertarray top = pos1, bottom = pos2
        ur_theta = theta + 32; 
        ul_theta = theta + 96;
        ll_theta = theta + 160;
        lr_theta = theta + 224; 

        //DrawArrays Quads goes ul, ur, lr, ll 

        //upper left    
        *vertarray++ = part_pos_view[0] + (costable[ul_theta] * size) ;
        *vertarray++ = part_pos_view[1] + (sintable[ul_theta] * size) ; 
        *vertarray++ = part_pos_view[2]; 

        //upper right
        *vertarray++ = part_pos_view[0] + (costable[ur_theta] * size) ; 
        *vertarray++ = part_pos_view[1] + (sintable[ur_theta] * size) ;
        *vertarray++ = part_pos_view[2]; 

        if( isRibbon && i != 0) //RIBBON PARTICLES  
        {
            //lower left
            *vertarray++ = lastul[0];
            *vertarray++ = lastul[1];
            *vertarray++ = lastul[2];

            //lower right
            *vertarray++ = lastur[0];
            *vertarray++ = lastur[1];
            *vertarray++ = lastur[2];
        }
        else 
        {
            //lower left
            *vertarray++ = part_pos_view2[0] + (costable[ll_theta] * size) ;//lastul[0];//
            *vertarray++ = part_pos_view2[1] + (sintable[ll_theta] * size) ;//lastul[1];//
            *vertarray++ = part_pos_view2[2]; //lastul[2];//

            //lower right
            *vertarray++ = part_pos_view2[0] + (costable[lr_theta] * size) ;//lastur[0];//
            *vertarray++ = part_pos_view2[1] + (sintable[lr_theta] * size) ;//lastur[1];//
            *vertarray++ = part_pos_view2[2];//lastur[2];//
        }

        if( isRibbon )
        {
            lastul[0] = vertarray[-12];
            lastul[1] = vertarray[-11];
            lastul[2] = vertarray[-10];

            lastur[0] = vertarray[-9];
            lastur[1] = vertarray[-8];
            lastur[2] = vertarray[-7];
        }


    }

    //Calc and fill in the system's rgba
    //New Alloc 
    //partDebugNewAllocing( system );

    for( i = 0, partIdx = system->firstParticle ; i < system->particle_count ; i++, partIdx++ )
    {    
        if( partIdx == system->maxParticles )
            partIdx = 0;

        particle = &system->particles[partIdx];
    //End

        //************* Get new rgba if there is a colorpath        
         if( system->effectiveColorPath->length > 1)
        {
            if(sysInfo->colorchangetype == PARTICLE_COLORSTOP)
            {
                int itemp1, itemp2;
                itemp1 = round(particle->age-timestep);
                itemp2 = system->effectiveColorPath->length - 1;
                idx = MIN(itemp1, itemp2);
            }
            else //sysInfo->colorchangetype == PARTICLE_COLORLOOP
                idx = (round32(particle->age-timestep)) % system->effectiveColorPath->length;
            if (idx<0)
                idx = 0;
            rgb = &(system->effectiveColorPath->path[idx * 3]);
        }
        alpha = round( (F32)particle->alpha * total_alpha); //to do, make this smarter?  

        //*************** Do per frame writing to the rgbas array
        *rgbaarray++ = rgb[0];
        *rgbaarray++ = rgb[1];
        *rgbaarray++ = rgb[2];
        *rgbaarray++ = alpha; 

        *rgbaarray++ = rgb[0];
        *rgbaarray++ = rgb[1];
        *rgbaarray++ = rgb[2];
        *rgbaarray++ = alpha;
        
        if( 1 ) //!//RIBBON PARTICLES
        {
            *rgbaarray++ = rgb[0];
            *rgbaarray++ = rgb[1];
            *rgbaarray++ = rgb[2];
            *rgbaarray++ = alpha; 

            *rgbaarray++ = rgb[0];
            *rgbaarray++ = rgb[1];
            *rgbaarray++ = rgb[2];
            *rgbaarray++ = alpha;
        }
    }

    //New Alloc
    //partDebugNewAllocing( system );
    //End
}

/*Immediately remove system
*/
void partKillSystem(ParticleSystem * system, int id)
{
    PERFINFO_AUTO_START("partKillSystem", 1);
        system = partConfirmValid( system, id );
        if(system)
        {
            partFreeSingleSystemNode(system);//dealloc
        }
    PERFINFO_AUTO_STOP();
}

/*Shut down generator and Let the particles die off on their own, possibly changing behavior 
to the death system. Once the system is set to dying don't do this again...
*/
void partSoftKillSystem(ParticleSystem * system, int id)
{    
    ParticleSystemInfo * sysInfo;
    ParticleSystemInfo * newsys;
    int deathagetozero;

    if( !system || !system->inuse || system->unique_id != id ) 
        return;
    if( system->dying )
        return;

    sysInfo = system->sysInfo;
    //New Alloc this is broken until I fix
    if( 0 && sysInfo->dielikethis[0] && strcmp(sysInfo->dielikethis, "0") != 0)
    {
        deathagetozero = sysInfo->deathagetozero;
        newsys = partGiveNewParameters( system, id, sysInfo->dielikethis );
        if(newsys)
        {
            if(deathagetozero)
            {
                int i;
                for(i = 0 ; i < system->maxParticles ; i++)
                    system->particles[i].age = 0;
            }
            printToScreenLog( 0, "PART: System %d converted to %s", system->unique_id, system->sysInfo->name); 
        }
        printToScreenLog( 0, "PART: System %d failed to convert to %s", system->unique_id, sysInfo->dielikethis); 
    }    
    //End 
    system->new_per_frame    = 0; 
    system->burbleamplitude    = 0; 
    system->kill_on_zero    = 1;
    system->dying = 1;

    if( sysInfo->flags & PART_FADE_IMMEDIATELY_ON_DEATH )
    {
        //Could probably advance all particles right here, but I think this is cleaner
        system->advanceAllParticlesToFadeOut = 1;
    }
}



// Engine #############################################################

void printVertexBufferObjectDebugInfo()
{
    static int maxAllocedAtATime;
    static int maxAllocedOnAFrame;
    int i = 20; 
    xyprintf( 20, i++, "totalVertMemAlloced    %d", totalVertMemAlloced );
    xyprintf( 20, i++, "totalVertArraysAlloced %d", totalVertArraysAlloced );
    xyprintf( 20, i++, "totalVertMemFreed      %d", totalVertMemFreed );
    xyprintf( 20, i++, "totalVertArraysFreed   %d", totalVertArraysFreed );
    xyprintf( 20, i++, "totalVertArraysCurr    %d", totalVertMemAlloced - totalVertMemFreed );
    
    if( totalVertMemAlloced - totalVertMemFreed > maxAllocedAtATime )
        maxAllocedAtATime = totalVertMemAlloced - totalVertMemFreed;
    if( maxAllocedOnAFrame < totalVertMemAllocedThisFrame )
        maxAllocedOnAFrame = totalVertMemAllocedThisFrame;

    xyprintf( 20, i++, "maxAllocedAtATime      %d", maxAllocedAtATime );
    xyprintf( 20, i++, "maxAllocedOnAFrame     %d", maxAllocedOnAFrame );

    totalVertMemAllocedThisFrame = 0;
    totalVertArraysAllocedThisFrame = 0;
    totalVertMemFreedThisFrame = 0;
    totalVertArraysFreedThisFrame = 0;
    
}

void partEngineInitialize()
{
    int i,j,vertsize,colorsize,texsize,trisize;
    U8 * big_buffer;

    if( game_state.noparticles )
        return;

    memset(&particle_engine, 0, sizeof(ParticleEngine));

    vertsize  =    ( sizeof(Vec3) * 4 * MAX_PARTSPERSYS );
    colorsize =    ( 4 * 4 * MAX_PARTSPERSYS );

    texsize   =    ( sizeof(F32)* 2 * 4 * MAX_PARTSPERSYS );    
    trisize   =    ( sizeof(int)* 3 * 2 * MAX_PARTSPERSYS );

    big_buffer = calloc( 1, vertsize + colorsize );
    particle_engine.verts      = (void *)&big_buffer[0];
    particle_engine.rgbas      = (void *)&big_buffer[vertsize];

    //RIBBON PARTICLES
    //make custom version of verts, colors, tris and texcoords
    //give em VBOs and switch em around

    //Get an ARRAY buffer for constant tex coords and stuff them into it
    particle_engine.texarray = calloc( 1, texsize );
    for(i = 0 ; i < MAX_PARTSPERSYS * 2 * 4; i+=8)  
    {              //MAX_PARTSPERSYS * the size of a Vec 2 * the four corners
        //DrawArrays Quads goes 00 , 10, 11, 01 
        particle_engine.texarray[i+0]  =  0; //ul    
        particle_engine.texarray[i+1]  =  0;

        particle_engine.texarray[i+2]  =  1; //ur
        particle_engine.texarray[i+3]  =  0; 

        particle_engine.texarray[i+4]  =  0; //ll
        particle_engine.texarray[i+5]  =  1;

        particle_engine.texarray[i+6]  =  1; //lr
        particle_engine.texarray[i+7]  =  1; 
    }


    //Get an ELEMENT_ARRAY buffer for constant tri coords and stuff them into it
    particle_engine.tris = calloc( 1, trisize );
    for(i = 0, j = 0 ; i < MAX_PARTSPERSYS * 6 ; i+=6, j+=4) 
    {
        particle_engine.tris[i+0] = j + 2;
        particle_engine.tris[i+1] = j + 0;
        particle_engine.tris[i+2] = j + 1;
        particle_engine.tris[i+3] = j + 1;
        particle_engine.tris[i+4] = j + 3;
        particle_engine.tris[i+5] = j + 2;
    }
    particle_engine.systems_on = 1;
    partSystemListInit(particle_engine.particle_systems);
    initQuickTrig(); 
    srand(10);       
}

partKickStartSystem( ParticleSystem * system )
{
    int holdcurrdrawstate;
    holdcurrdrawstate = system->currdrawstate;
    while( system->age < system->sysInfo->timetofull )
        partUpdateSystem( system, 1.0, 1.0 );
    system->currdrawstate = holdcurrdrawstate;
    system->kickstart = 0;
}


//Check Visibility
F32 partCalculatePerformanceAlphaFade( ParticleSystem * system, Mat4 systemMatCamSpace )
{
    Vec3 partMidCamSpace;
    Vec3 partMidWorldSpace;
    Vec3 particleMid;
    F32 radius;
    F32 distAlpha;
    F32 alpha;
    F32 camDistEmit;
    F32 camDistMid;
    ParticleSystemInfo * sysInfo;
    
    sysInfo = system->sysInfo; 

    //Calculate Radius, particleMid, partMidWorldSpace, partMidCamSpace, do frustum check
    {
        int visible;
        Vec3 r;

        //Get particleMid( Middle of System ) and Radius, 
        //TO DO we don't curently scale the radius by the scale factor on the local system 
        subVec3(system->perf.boxMax, system->perf.boxMin, r);
        scaleVec3( r, 0.5, r );
        radius = lengthVec3(r);
        radius += sqrtf(system->perf.biggestParticleSquared); //Assume biggest particle is on the edge 
        addVec3( system->perf.boxMin, r, particleMid ); 

        //Do Frustum Check
        if( sysInfo->worldorlocalposition == PARTICLE_LOCALPOS )
            mulVecMat4( particleMid, system->drawmat, partMidWorldSpace );
        else //sysInfo->worldorlocalposition == PARTICLE_WORLDPOS
            copyVec3( particleMid, partMidWorldSpace );

        mulVecMat4( partMidWorldSpace, cam_info.viewmat, partMidCamSpace ); 
        visible = gfxSphereVisible(partMidCamSpace, radius);      
        if (visible && game_state.zocclusion && !((editMode() || game_state.see_everything & 1) && !edit_state.showvis))
        {
            if (!zoTestSphere(partMidCamSpace, radius, visible & CLIP_NEAR))
                visible = 0;
        }
        //End Frustum Check

        if( !visible )
            return 0;
    }

    camDistEmit = sqrt( system->camDistSqr );
    camDistMid = lengthVec3( partMidCamSpace );

    //Calculate dist Alpha.  TO DO : I may want ot do this later, taking more into account
    {
        if(sysInfo->visdist) //TO DO camDistEmit instead?
            distAlpha = MIN(1.0,( (sysInfo->visdist + PARTICLE_DISTFADELEN - camDistMid ) / PARTICLE_DISTFADELEN ));
        else
            distAlpha = 1.0;

        alpha = MIN( distAlpha, 1.0 );
    }

    if(0) //this estimates particle fill ok.  Maybe do a better job
    {
        F32 avgAlpha, avgSize,dp;

        //Calculate DP
        if( sysInfo->frontorlocalfacing == PARTICLE_LOCALFACING )
        {
            Vec3 eyeZ;
            Vec3 sysZ;

            eyeZ[0] = 0; eyeZ[1] = 0; eyeZ[2] = 1;
            mulVecMat3( eyeZ, systemMatCamSpace, sysZ );
            normalVec3( sysZ );

            dp = dotVec3( eyeZ, sysZ ); //TO DO use projection mat for dp?
        }
        else 
            dp = 1; //sysInfo->frontorlocalfacing == PARTICLE_FRONTFACING

        //Average Alpha and Average Size, and Dist from Camera
        avgAlpha = system->perf.totalAlpha / system->particle_count;
        avgSize     = system->perf.totalSize  / system->particle_count; 

        //Calculate a total fill cost:
        //TO DO consider FOV
        {
            F32 fillGuess;
            fillGuess = 50 * system->perf.totalSize * (1/(camDistMid*camDistMid)) * ABS(dp) ;      
 
                //TO DO I think I should if visible this.
                //TO DO alpha mimic by additive and cut colors. Consider?
                //TO DO create fill rate guess and a system to compare with actual fill.

                //Debug info
                if( 0 && game_state.perf )  
                {
                    Mat4 mat;
                    int i = 30;
                    xyprintf( 1, i++, "TOTAL SIZE   %f", system->perf.totalSize );  //accurate
                    xyprintf( 1, i++, "AVG SIZE     %f", avgSize );
                    xyprintf( 1, i++, "BIGGEST      %f", sqrtf(system->perf.biggestParticleSquared) );
                    xyprintf( 1, i++, "TOTAL ALPHA  %f", system->perf.totalAlpha );
                    xyprintf( 1, i++, "AVG ALPHA    %f", avgAlpha );
                    xyprintf( 1, i++, "EMIT CAMDIST %f", camDistEmit );
                    xyprintf( 1, i++, "MID  CAMDIST %f", camDistMid );
                    xyprintf( 1, i++, "MIDPOINT     %f %f %f", particleMid[0], particleMid[1], particleMid[2] );
                    xyprintf( 1, i++, "WORLDSPACEMID%f %f %f", partMidWorldSpace[0], partMidWorldSpace[1], partMidWorldSpace[2] );
                    xyprintf( 1, i++, "CAMSPACEMID  %f %f %f", partMidCamSpace[0], partMidCamSpace[1], partMidCamSpace[2] );
                    xyprintf( 1, i++, "DOT            %f", dp );  //accurate
                    xyprintf( 1, i++, "FILL GUESS   %f", fillGuess );

                    copyMat3( unitmat, mat );
                    mulVecMat4( partMidCamSpace, cam_info.inv_viewmat, mat[3] );
                    drawSphere(mat,radius,11,0xffffffff);
                }
        }
        //End Debug



    }
    return alpha;
}

#define MAX_PART_CLAMPED_TIMESTEP 2.0 //10 fps
void partRunSystem(ParticleSystem * system)
{
    F32 alpha = 0;
    int still_going = 0;

    if(devassert(partialSystemRun_inited))    //if you're asserting here, it's because you didn't partRunStartup.
                                            //remember to partRunShutdown, please.
    {
        if (system->gfxnode) {
            PERFINFO_AUTO_START("system->gfxNode",1);
            rdrCleanUpAfterRenderingParticleSystems();

            if (system->gfxnode->viewspace && system->gfxnode->clothobj) {
                modelDrawGfxNode(system->gfxnode, system->gfxnode->node_blend_mode, -1, TINT_NONE, system->drawmat);
            }

            rdrPrepareToRenderParticleSystems();
            PERFINFO_AUTO_STOP();
            return;
        }

        PERFINFO_AUTO_START("partKickStartSystem and pals",1);

        if( system->kickstart ) //Currently not used for anything
        {
            system->currdrawstate = PART_UPDATE_BUT_DONT_DRAW_ME; 
            partKickStartSystem( system );
        }

        //Figure out what you should do this frame based on your age, your own flags and your parent FX parameters. 
        //Notice particles interpret fx's UPDATE_BUT_DONT_DRAW_ME as don't do anything.  This should help perf
        if( system->parentFxDrawState == FX_UPDATE_AND_DRAW_ME || (system->sysInfo->flags & PART_ALWAYS_DRAW) )
            system->currdrawstate = PART_UPDATE_AND_DRAW_ME;
        else if(system->age < system->sysInfo->timetofull)
            system->currdrawstate = PART_UPDATE_BUT_DONT_DRAW_ME; // parent fx is hidden, but we still run the startup simulation
        else
            system->currdrawstate = PART_DONT_UPDATE_OR_DRAW_ME;

        PERFINFO_AUTO_STOP_START("partUpdateSystem",1);
#if USE_NEW_TIMING_CODE
        PERFINFO_AUTO_START_STATIC(system->sysInfo->name, (PerfInfoStaticData**)system->sysInfo->name, 1);
#endif
        //Debug
        if(!(game_state.fxdebug & FX_DISABLE_PART_CALCULATION))
        {//End Debug
            if( system->currdrawstate == PART_DONT_UPDATE_OR_DRAW_ME )
            {
                PERFINFO_AUTO_START("partCheckForDeath", 1);
                still_going = partCheckForDeath( system, TIMESTEP, TIMESTEP );//add a thing that detect this system's death time, and
                PERFINFO_AUTO_STOP();
                //particleTotalSkipped += system->particle_count;
                //particleTotalSystemsSkipped++;
            }
            else
            {
                PERFINFO_AUTO_START("partUpdateSystem", 1);
                still_going = partUpdateSystem( system, TIMESTEP, TIMESTEP );
                PERFINFO_AUTO_STOP();
            }
        }//Debug
        else
        {
            still_going = 1;
        }
        //End Debug
#if USE_NEW_TIMING_CODE
        PERFINFO_AUTO_STOP();
#endif
        PERFINFO_AUTO_STOP_START("partDrawSystem",1);
        if(fx_engine.fx_auto_timers)
        {
#if USE_NEW_TIMING_CODE
            PERFINFO_AUTO_START_STATIC(system->sysInfo->name, (PerfInfoStaticData**)system->sysInfo->name, 1);
#endif
        }
        if( !still_going )
        {
            PERFINFO_AUTO_START("partKillSystem", 1);
            partKillSystem(system, system->unique_id);
            PERFINFO_AUTO_STOP();
        }
        else
        {
            if(system->particle_count && system->currdrawstate == PART_UPDATE_AND_DRAW_ME )
            {
                Mat4Ptr systemMatCamSpace;
                PERFINFO_AUTO_START("predraw",1);
                if( system->sysInfo->frontorlocalfacing == PARTICLE_LOCALFACING ) //facing hasn't been done yet
                {
                    Mat4 systemMat;
                    static Mat4 systemMatCamSpaceStatic;
                    systemMatCamSpace = systemMatCamSpaceStatic;

                    copyMat4(system->drawmat, systemMat); //rebuilt, so copy is unnecessary
                    pitchMat3(RAD(90), systemMat);
                    mulMat4( cam_info.viewmat, systemMat, systemMatCamSpace );
                }
                else //( sysInfo->facing == PARTICLE_FRONTFACE ) //facing has been done
                {
                    systemMatCamSpace = 0;
                }
                PERFINFO_AUTO_STOP_START("partCalculatePerformanceAlphaFade",1);
                //TO DO what does this do?
                {
                    F32 inheritedAlpha = (F32)system->inheritedAlpha / 255.0;
                    alpha = partCalculatePerformanceAlphaFade( system, systemMatCamSpace ); 
                    alpha = MIN( inheritedAlpha, alpha );
                }

                //xyprintf( 50, y++, "Alpha: %f", system->inheritedAlpha ); 
                PERFINFO_AUTO_STOP_START("modelDrawParticleSys",1);
                if( !(game_state.stopInactiveDisplay && game_state.inactiveDisplay) )
                {
                    if( alpha > ( (F32)PARTICLE_ALPHACUTOFF / 255.0 ) )
                    {
                        modelDrawParticleSys( system, alpha, 0, systemMatCamSpace );                        
                    }
                }
                PERFINFO_AUTO_STOP();
            }

            copyVec3(system->emission_start, system->emission_start_prev); //for PARTICLE_TRAIL //needs to go with particle array write.
            copyMat4(system->drawmat, system->lastdrawmat); //not currently used
            system->teleported = 0;
            //system->inheritedAlpha = 255; //JE: This caused all FX to pop when the entity fades out, bad!
        }
        if(fx_engine.fx_auto_timers)
        {
#if USE_NEW_TIMING_CODE
            PERFINFO_AUTO_STOP();
#endif
        }
        PERFINFO_AUTO_STOP();
        }
}
void partRunStartup()
{
    if(!partialSystemRun_inited)
    {
        rdrPrepareToRenderParticleSystems();
        partialSystemRun_inited = 1;
    }
}
void partRunShutdown()
{
    if(partialSystemRun_inited)
    {
        rdrCleanUpAfterRenderingParticleSystems(); 
        partialSystemRun_inited = 0;
    }    
}
void particleSystemSetDisableMode(int mode)
{
    s_particleDisableDrawMode = mode;
}
int particleSystemGetDisableMode()
{
    return s_particleDisableDrawMode;
}
void partRunEngine()
{
    ParticleSystem * system;
    ParticleSystem * next;
    int still_going;
    int particleTotal, particleTotalUpdated, particleTotalDrawn, particleTotalSystemsUpdated, particleTotalSystemsDrawn; //debug
    int currVertArray = 0;
    F32 alpha;                //performance alpha of the system this frame
    F32 clampedTimestep;
    int y = 0;

    // particle system has not been refactored for independent
    // simulation and rendering
    rdrBeginMarker(__FUNCTION__ " - Particle System Engine");
    
    //Debug
    if( game_state.noparticles || s_particleDisableDrawMode)
        particle_engine.systems_on = 0;
    else
        particle_engine.systems_on = 1;

//    if(0) printVertexBufferObjectDebugInfo();
    
    particleTotalUpdated    = 0;
    particleTotalDrawn        = 0;
    particleTotalSystemsUpdated    = 0;
    particleTotalSystemsDrawn    = 0;

    //End Debug

    particleTotal            = 0;

    if( particle_engine.num_systems && particle_engine.systems_on )
    {
        int iSystem = 0;

PERFINFO_AUTO_START("rdrPrepareToRenderParticleSystems",1);
        rdrPrepareToRenderParticleSystems();

        //TO DO : check if local systems use this correctly
        particle_engine.systems = partSortByDistance(particle_engine.systems);

        currVertArray = 0; //VBO array to alloc for the next system
        clampedTimestep = TIMESTEP > MAX_PART_CLAMPED_TIMESTEP ? MAX_PART_CLAMPED_TIMESTEP : TIMESTEP;
PERFINFO_AUTO_STOP();

        for(system = particle_engine.systems ; system ; system = next)
        {
            next = system->next;

#ifndef FINAL
            // for debugging purposes we can limit only one system to updated and draw (1 based index)
            // but this isn't very robust because the ordering can change from sorting and deaths, etc.
            if ( g_client_debug_state.particles_only_index > 0 && g_client_debug_state.particles_only_index != ++iSystem)    
                continue;
#endif

            if (system->gfxnode) {
                PERFINFO_AUTO_START("system->gfxNode",1);
                rdrCleanUpAfterRenderingParticleSystems();

                if (system->gfxnode->viewspace && system->gfxnode->clothobj) {
                    modelDrawGfxNode(system->gfxnode, system->gfxnode->node_blend_mode, -1, TINT_NONE, system->drawmat);
                }

                rdrPrepareToRenderParticleSystems();
                PERFINFO_AUTO_STOP();
                continue;
            }
            
            PERFINFO_AUTO_START("partKickStartSystem and pals",1);

                if( system->kickstart ) //Currently not used for anything
                {
                    system->currdrawstate = PART_UPDATE_BUT_DONT_DRAW_ME; 
                    partKickStartSystem( system );
                }

                //Figure out what you should do this frame based on your age, your own flags and your parent FX parameters. 
                //Notice particles interpret fx's UPDATE_BUT_DONT_DRAW_ME as don't do anything.  This should help perf
                if( system->parentFxDrawState == FX_UPDATE_AND_DRAW_ME || (system->sysInfo->flags & PART_ALWAYS_DRAW) )
                    system->currdrawstate = PART_UPDATE_AND_DRAW_ME;
                else if(system->age < system->sysInfo->timetofull)
                    system->currdrawstate = PART_UPDATE_BUT_DONT_DRAW_ME; // parent fx is hidden, but we still run the startup simulation
                else
                    system->currdrawstate = PART_DONT_UPDATE_OR_DRAW_ME;

                particleTotal += system->particle_count;
            PERFINFO_AUTO_STOP_START("partUpdateSystem",1);
                #if USE_NEW_TIMING_CODE
                    PERFINFO_AUTO_START_STATIC(system->sysInfo->name, (PerfInfoStaticData**)system->sysInfo->name, 1);
                #endif
                //Debug
                if(!(game_state.fxdebug & FX_DISABLE_PART_CALCULATION))
                {//End Debug
                    if( system->currdrawstate == PART_DONT_UPDATE_OR_DRAW_ME )
                    {
                        PERFINFO_AUTO_START("partCheckForDeath", 1);
                            still_going = partCheckForDeath( system, TIMESTEP, clampedTimestep );//add a thing that detect this system's death time, and
                        PERFINFO_AUTO_STOP();
                        //particleTotalSkipped += system->particle_count;
                        //particleTotalSystemsSkipped++;
                    }
                    else
                    {
                        PERFINFO_AUTO_START("partUpdateSystem", 1);
                            still_going = partUpdateSystem( system, TIMESTEP, clampedTimestep );
                        PERFINFO_AUTO_STOP();
                        particleTotalUpdated += system->particle_count;
                        particleTotalSystemsUpdated++;
                    }
                }//Debug
                else
                {
                    still_going = 1;
                }
                //End Debug
                #if USE_NEW_TIMING_CODE
                    PERFINFO_AUTO_STOP();
                #endif
            PERFINFO_AUTO_STOP_START("partDrawSystem",1);
                if(fx_engine.fx_auto_timers)
                {
                    #if USE_NEW_TIMING_CODE
                        PERFINFO_AUTO_START_STATIC(system->sysInfo->name, (PerfInfoStaticData**)system->sysInfo->name, 1);
                    #endif
                }
                if( !still_going )
                {
                    PERFINFO_AUTO_START("partKillSystem", 1);
                        partKillSystem(system, system->unique_id);
                    PERFINFO_AUTO_STOP();
                }
                else
                {
                    if(system->particle_count && system->currdrawstate == PART_UPDATE_AND_DRAW_ME )
                    {
                        Mat4Ptr systemMatCamSpace;
                        PERFINFO_AUTO_START("predraw",1);
                        if( system->sysInfo->frontorlocalfacing == PARTICLE_LOCALFACING ) //facing hasn't been done yet
                        {
                            Mat4 systemMat;
                            static Mat4 systemMatCamSpaceStatic;
                            systemMatCamSpace = systemMatCamSpaceStatic;

                            copyMat4(system->drawmat, systemMat); //rebuilt, so copy is unnecessary
                            pitchMat3(RAD(90), systemMat);
                            mulMat4( cam_info.viewmat, systemMat, systemMatCamSpace );
                        }
                        else //( sysInfo->facing == PARTICLE_FRONTFACE ) //facing has been done
                        {
                            systemMatCamSpace = 0;
                        }
                        PERFINFO_AUTO_STOP_START("partCalculatePerformanceAlphaFade",1);
                        //TO DO what does this do?
                        {
                            F32 inheritedAlpha = (F32)system->inheritedAlpha / 255.0;
                            alpha = partCalculatePerformanceAlphaFade( system, systemMatCamSpace ); 
                            alpha = MIN( inheritedAlpha, alpha );
                        }

                        //xyprintf( 50, y++, "Alpha: %f", system->inheritedAlpha ); 
                        PERFINFO_AUTO_STOP_START("modelDrawParticleSys",1);
                        if( !(game_state.stopInactiveDisplay && game_state.inactiveDisplay) )
                        {
                            if( alpha > ( (F32)PARTICLE_ALPHACUTOFF / 255.0 ) )
                            {
                                particleTotalDrawn += modelDrawParticleSys( system, alpha, 0, systemMatCamSpace );
                                particleTotalSystemsDrawn++;
                            }
                        }
                        PERFINFO_AUTO_STOP();
                    }

                    copyVec3(system->emission_start, system->emission_start_prev); //for PARTICLE_TRAIL //needs to go with particle array write.
                    copyMat4(system->drawmat, system->lastdrawmat); //not currently used
                    system->teleported = 0;
                    //system->inheritedAlpha = 255; //JE: This caused all FX to pop when the entity fades out, bad!
                }
                if(fx_engine.fx_auto_timers)
                {
                    #if USE_NEW_TIMING_CODE
                        PERFINFO_AUTO_STOP();
                    #endif
                }
            PERFINFO_AUTO_STOP();
        } //end for loop

#ifdef NOVODEX_FLUIDS
        fxDrawFluids();
#endif


PERFINFO_AUTO_START("rdrCleanUpAfterRenderingParticleSystems",1);
        rdrCleanUpAfterRenderingParticleSystems(); 
PERFINFO_AUTO_STOP();
    }

    particle_engine.particleTotal = particleTotal;

#ifndef FINAL
    //Debug 
    {
        static currhigh = 0;
        static high = 0;
        static time = 0;

        if(high < particleTotal)
            high = particleTotal;
        time += TIMESTEP;

        if(game_state.resetparthigh)
        {
            high = 0;
            clearLog();
            game_state.resetparthigh = 0;
        }
        if( game_state.fxdebug & FX_DEBUG_BASIC || game_state.renderinfo)
        {    
            xyprintf(0,480/8-12 + TEXT_JUSTIFY,"Particles Drawn    :%4d", particleTotalDrawn); 
            xyprintf(0,480/8-11 + TEXT_JUSTIFY,"Particles Updated  :%4d", particleTotalUpdated);
            xyprintf(0,480/8-10 + TEXT_JUSTIFY,"Particles Total    :%4d (High%4d)", particleTotal, high);
            xyprintf(0,480/8-9 + TEXT_JUSTIFY,"PartSys   Drawn    :%4d", particleTotalSystemsDrawn);
            xyprintf(0,480/8-8 + TEXT_JUSTIFY,"PartSys   Updated  :%4d", particleTotalSystemsUpdated);
            xyprintf(0,480/8-7 + TEXT_JUSTIFY,"PartSys   Total    :%4d", particle_engine.num_systems);
    
        }

        if( isDevelopmentMode() && particle_engine.num_systems > MAX_SYSTEMS - 50 )
            Errorf("PART: Soon Too many Systems!!!!!!!!!  %d", particle_engine.num_systems);

        if (isDevelopmentMode() && demoIsPlaying())
        {
            static int counter = 0;
            if (counter++ > 30)
            {
                counter = 0;
                //printf("Particle systems: %d\n", particle_engine.num_systems);
            }
        }
    }
#endif // FINAL
    rdrEndMarker();
}



