#define EARRAY_IMPL
// earray.c - provides yet another type of expandable pArray
// these arrays are differentiated in that you generally declare them as
// MyStruct** earray; and access them like a normal pArray of pointers
// NOTE - EArrays are now threadsafe

#include "utilitieslib/components/earray.h"
#include <string.h>
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/mathutil.h"

#ifndef EXTERNAL_TEST
#include "utilitieslib/utils/MemoryMonitor.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/utils.h"
#endif

#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/components/SharedMemory.h"

#if PRIVATE_EARRAY_HEAP
static const char EARRAY_MEMMONITOR_NAME[] = "EArray";
HANDLE g_earrayheap = NULL;
static int heap_flags = 0; // thread-safe, resizable
#endif

int eaValidateHeap()
{
    int ret = 1;
    #if PRIVATE_EARRAY_HEAP
        if (!g_earrayheap)
            return 1;
        #if DEBUG
            assert(ret = HeapValidate(g_earrayheap,heap_flags,0));
        #endif
    #endif
    return ret;
}

#if EARRAY_CRAZY_DEBUGGING
EArray* EArrayFromHandle(const void *handle)
{
    if(handle)
    {
        EArray *pArray = ((EArray*)(((char*)handle) - EARRAY_HEADER_SIZE));
        #if PRIVATE_EARRAY_HEAP
            if(!(pArray->flags & EARRAY_FLAG_CUSTOMALLOC))
                assert(HeapValidate(g_earrayheap, heap_flags, pArray));
        #endif
        return pArray;
    }
    return NULL;
}

EArray32* EArray32FromHandle(const void *handle)
{
    if(handle)
    {
        EArray32 *pArray = ((EArray32*)(((char*)handle) - EARRAY32_HEADER_SIZE));
        #if PRIVATE_EARRAY_HEAP
            if(!(pArray->flags & EARRAY_FLAG_CUSTOMALLOC))
                assert(HeapValidate(g_earrayheap, heap_flags, pArray));
        #endif
        return pArray;
    }
    return NULL;
}
#endif

/////////////////////////////////////////////////////// EArray (64-bit compatible) functions

// from within this file, you should be calling the *Dbg versions of these
#undef eaCreate
#undef eaSetCapacity
#undef eaSetSize
#undef eaPush
#undef eaPushUnique
#undef eaPushArray
#undef eaSet
#undef eaSetForced
#undef eaGet
#undef eaGetConst
#undef eaLast
#undef eaLastConst
#undef eaInsert
#undef eaCopy
#undef eaCopyEx
#undef eaCompress
#undef eaDestroy
#undef eaDestroyEx
#undef eaFind
#undef eaFindAndRemove
#undef eaFindAndRemoveFast
#undef eaRemove
#undef eaRemoveConst
#undef eaRemoveFast
#undef eaRemoveFastConst
#undef eaPop
#undef eaPopConst
#undef eaClear
#undef eaClearConst

void eaCreateWithCapacityDbg(mEArrayHandle* handle, int capacity, const char *file, int line)
{
    EArray* pArray;
    assert(capacity >= 0);
    if(capacity < 1)
        capacity = 1;
#if PRIVATE_EARRAY_HEAP
    if(!g_earrayheap)
    {
        g_earrayheap = HeapCreate(heap_flags, 0, 0);
        #ifndef _XBOX
            disableRtlHeapChecking(g_earrayheap);
        #endif
    }
    pArray = HeapAlloc(g_earrayheap, 0, EARRAY_HEADER_SIZE + sizeof(pArray->structptrs[0])*(capacity));
    memMonitorTrackUserMemory(EARRAY_MEMMONITOR_NAME, 1, MOT_ALLOC, EARRAY_HEADER_SIZE + sizeof(pArray->structptrs[0])*(capacity));
#else
    pArray = _malloc_dbg(EARRAY_HEADER_SIZE + sizeof(pArray->structptrs[0])*(capacity), _NORMAL_BLOCK, file, line);
#endif
    memset(pArray, 0, EARRAY_HEADER_SIZE); // this makes me feel better than setting the fields directly
//     pArray->count = 0;
//     pArray->flags = 0;
    pArray->size = capacity;
    *handle = HandleFromEArray(pArray);
}

void eaDestroyDbg(mEArrayHandle* handle) // free list
{
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle)
        return;
    devassertmsg(!(pArray->flags & EARRAY_FLAG_CUSTOMALLOC), "trying to perform a heap operation on a compressed EArray");
#if PRIVATE_EARRAY_HEAP
    memMonitorTrackUserMemory(EARRAY_MEMMONITOR_NAME, 1, MOT_FREE, HeapSize(g_earrayheap, heap_flags, pArray));
    HeapFree(g_earrayheap, 0, pArray);
#else
    _free_dbg(pArray, _NORMAL_BLOCK);
#endif
    *handle = NULL;
}

void eaClearExDbg(mEArrayHandle* handle, EArrayItemDestructor destructor)
{
    int i;
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle)
        return;

    for (i = 0; i < pArray->count; i++)
    {
        if (pArray->structptrs[i])
        {
            if(destructor)
                destructor(pArray->structptrs[i]);
            else
                free(pArray->structptrs[i]);    

            pArray->structptrs[i] = NULL;
        }
    }

    pArray->count = 0;
}

void eaDestroyExDbg(mEArrayHandle *handle,EArrayItemDestructor destructor)
{
    eaClearEx(handle,destructor);
    eaDestroyDbg(handle);
}

void eaSetCapacityDbg(mEArrayHandle* handle, int capacity, const char *file, int line) // grows or shrinks capacity, limits size if required
{
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle)
        return;    // fail silently.  FIXME: i hate this behavior, shouldn't this be an eaCreate instead?
    assert(capacity >= 0);
    devassertmsg(!(pArray->flags & EARRAY_FLAG_CUSTOMALLOC), "trying to perform a heap operation on a compressed EArray");
    pArray->size = capacity>1? capacity: 1;
#if PRIVATE_EARRAY_HEAP
    memMonitorTrackUserMemory(EARRAY_MEMMONITOR_NAME, 1, MOT_REALLOC, EARRAY_HEADER_SIZE + sizeof(pArray->structptrs[0])*(pArray->size) - HeapSize(g_earrayheap, heap_flags, pArray));
    pArray = HeapReAlloc(g_earrayheap, 0, pArray, EARRAY_HEADER_SIZE + sizeof(pArray->structptrs[0])*(pArray->size));
#else
    pArray = _realloc_dbg(pArray, EARRAY_HEADER_SIZE + sizeof(pArray->structptrs[0])*(pArray->size), _NORMAL_BLOCK, file, line);
#endif
    if(pArray->count > capacity)
        pArray->count = capacity;
    *handle = HandleFromEArray(pArray);
}

int eaCapacity(cccEArrayHandle* handle) // just returns current capacity
{
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle)
        return 0;
    return pArray->size;
}

size_t eaMemUsage(cccEArrayHandle* handle) // get the amount of memory actually used (not counting slack allocated)
{
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle)
        return 0;
    return pArray->count*sizeof(pArray->structptrs[0]) + EARRAY_HEADER_SIZE;
}

static int eaIncreaseCapacity(int current_capacity)
{
// TODO: use new logic for all process, not just the client
#ifdef CLIENT
    // New logic
    if (current_capacity < 32)
    {
        // Double in size
        return current_capacity * 2;
    }
    else if (current_capacity < 256)
    {
        // grow by 50%
        return current_capacity + (current_capacity >> 1);
    }

    // grow by 25%
    return current_capacity + (current_capacity >> 2);
#else
    // Old logic
    // Double in size
    return current_capacity * 2;
#endif
}

void eaSetSizeDbg(mEArrayHandle* handle, int size, const char *file, int line) // set the number of items in pArray (fills with zero)
{
    // this is a little confusing because Size = count within the pArray structure, Capacity = size
    EArray* pArray;
    if(!*handle)
        eaCreateWithCapacityDbg(handle, size, file, line);
    pArray = EArrayFromHandle(*handle);
    if (pArray->size < size) 
    {
        eaSetCapacityDbg(handle, size, file, line);
        pArray = EArrayFromHandle(*handle);
    }
    if (pArray->count < size) // increase size
        memset(&pArray->structptrs[pArray->count], 0, sizeof(pArray->structptrs[0])*(size-pArray->count));
    pArray->count = size;
}

int eaPushDbg(mEArrayHandle* handle, void* structptr, const char *file, int line) // add to the end of the list, returns the index it was added at (the new size)
{
    EArray* pArray;
    if(!*handle)
        eaCreateWithCapacityDbg(handle, 1, file, line);
    pArray = EArrayFromHandle(*handle);
    onlyfulldebugdevassert(!(pArray->flags & EARRAY_FLAG_SHAREDMEMORY) || isSharedMemory(structptr));
    assert(pArray->count <= pArray->size);
    if(pArray->count == pArray->size)
        eaSetCapacityDbg(handle, eaIncreaseCapacity(pArray->size), file, line);
    pArray = EArrayFromHandle(*handle);
    pArray->structptrs[pArray->count++] = (void*)structptr;
    return pArray->count-1;
}

int eaPushUniqueDbg(mEArrayHandle* handle, void* structptr, const char *file, int line) // add to the end of the list, returns the index it was added at (the new size)
{
    int idx = eaFindDbg((cccEArrayHandle*)handle, structptr);
    if (idx < 0)
        idx = eaPushDbg(handle, structptr, file, line);
    return idx;
}

int eaPushArrayDbg(mEArrayHandle *handle, cccEArrayHandle *src,
	const char *file, int line)
{
    int count = eaSizeUnsafe(handle);
    int srcCount = eaSizeUnsafe(src);
    eaSetSizeDbg(handle, count + srcCount, file, line);
    if(srcCount)
        CopyStructs(*handle + count, *src, srcCount);
    return count;
}

void* eaPopDbg(mEArrayHandle* handle) // remove the last item from the list
{
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle || !pArray->count)
        return NULL;
    pArray->count--;
    return pArray->structptrs[pArray->count];
}

const void* eaPopConstDbg(cEArrayHandle* handle)
{
    return eaPopDbg(cEACast(handle));
}

void eaSetForcedDbg(mEArrayHandle* handle, void* structptr, int i, const char *file, int line)
{
    EArray* pArray;
    int minsize = i+1;
    if(!*handle)
        eaCreateWithCapacityDbg(handle, minsize, file, line);
    pArray = EArrayFromHandle(*handle);
    onlyfulldebugdevassert(!(pArray->flags & EARRAY_FLAG_SHAREDMEMORY) || isSharedMemory(structptr));
    assert(pArray->count <= pArray->size);
    if(pArray->size < minsize)
        eaSetCapacityDbg(handle, MAX(minsize, eaIncreaseCapacity(pArray->size)), file, line);
    pArray = EArrayFromHandle(*handle);
    if (pArray->count < minsize) {
        if (pArray->count+1 < minsize)
            memset(&pArray->structptrs[pArray->count], 0, sizeof(pArray->structptrs[0])*(minsize-pArray->count-1));
        pArray->count = minsize;
    }
    pArray->structptrs[i] = structptr;
}

bool eaSetDbg(mEArrayHandle* handle, void* structptr, int i) // set i'th element (zero-based)
{
    EArray* pArray = EArrayFromHandle(*handle);
    onlyfulldebugdevassert(!(pArray->flags & EARRAY_FLAG_SHAREDMEMORY) || isSharedMemory(structptr));
    if(!*handle || !INRANGE0(i, pArray->count))
        return false;
    pArray->structptrs[i] = structptr;
    return true;
}

void* eaGetDbg(ccmEArrayHandle* handle, int i)    // get i'th element (zero-based), NULL on error
{
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle || !INRANGE0(i, pArray->count))
        return NULL;
    return pArray->structptrs[i];
}

const void* eaGetConstDbg(cccEArrayHandle* handle, int i)
{
	return eaGetDbg((ccmEArrayHandle*)handle, i);
}

void* eaLastDbg(ccmEArrayHandle* handle)
{
    return eaGetDbg(handle, eaSizeUnsafe(handle)-1);
}

const void* eaLastConstDbg(cccEArrayHandle* handle)
{
	return eaLastDbg((ccmEArrayHandle*)handle);
}

void eaInsertDbg(mEArrayHandle* handle, void* structptr, int i, const char *file, int line) // insert before i'th position, will not insert on error (i == -1, etc.)
{
    EArray* pArray;
    if(!*handle)
        eaCreateWithCapacityDbg(handle, 1, file, line);
    pArray = EArrayFromHandle(*handle);
    onlyfulldebugdevassert(!(pArray->flags & EARRAY_FLAG_SHAREDMEMORY) || isSharedMemory(structptr));
    if(i < 0 || i > pArray->count) // it's okay for i to equal count
        return;
    assert(pArray->count <= pArray->size);
    if(pArray->count == pArray->size)
        eaSetCapacityDbg(handle, eaIncreaseCapacity(pArray->size), file, line);
    pArray = EArrayFromHandle(*handle);

    CopyStructsFromOffset(pArray->structptrs + i + 1, -1, pArray->count - i);

    pArray->count++;
    pArray->structptrs[i] = structptr;
}

void* eaRemoveDbg(mEArrayHandle* handle, int i) // remove the i'th element, NULL on error
{
    void* structptr;
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle || !INRANGE0(i, pArray->count))
        return NULL;
    structptr = pArray->structptrs[i];
    CopyStructsFromOffset(pArray->structptrs + i, 1, pArray->count - i - 1);
    pArray->count--;
    return structptr;
}

const void* eaRemoveConstDbg(cEArrayHandle* handle, int i)
{
    return eaRemoveDbg(cEACast(handle), i);
}

void eaRemoveAndDestroy(mEArrayHandle* handle, int i, EArrayItemDestructor destructor)
{
    void *res = eaRemoveDbg(handle,i);
    if(res)
        if (destructor)
            destructor(res);
        else
            free(res);
}


void* eaRemoveFastDbg(mEArrayHandle* handle, int i) // remove the i'th element, and move the last element into this place DOES NOT KEEP ORDER
{    
    void* structptr;
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle || !INRANGE0(i, pArray->count))
        return NULL;
    structptr = pArray->structptrs[i];
    pArray->structptrs[i] = pArray->structptrs[--pArray->count];
    return structptr;
}

const void* eaRemoveFastConstDbg(cEArrayHandle* handle, int i)
{
    return eaRemoveFastDbg(cEACast(handle), i);
}

int    eaFindDbg(cccEArrayHandle* handle, const void* structptr)
{
    int i;
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle)
        return -1;
    for(i = 0; i < pArray->count; i++)
        if(pArray->structptrs[i] == structptr)
            return i;
    return -1;
}

int    eaFindAndRemoveDbg(cEArrayHandle* handle, const void* structptr)
{
    int    idx = eaFindDbg((cccEArrayHandle*)handle,structptr);
    if(idx >= 0)
        eaRemoveConstDbg(handle,idx);
    return idx;
}

int    eaFindAndRemoveFastDbg(cEArrayHandle* handle, const void* structptr)
{
    int    idx = eaFindDbg((cccEArrayHandle*)handle,structptr);
    if(idx >= 0)
        eaRemoveFastConstDbg(handle,idx);
    return idx;
}

void eaReverse(cEArrayHandle* handle)
{
    EArray* pArray = EArrayFromHandle(*handle);
    void **head, **tail;
    if(!*handle)
        return;
    head = pArray->structptrs;
    tail = head + pArray->count - 1;
    
    // Start the index at the two "outer most" elements of the pArray.
    // Each interation through the loop, swap the two elements that are indexed,
    // then bring the indices one step closer to each other.
    for(; head < tail; head++, tail--)
    {
        void* tempBuffer = *head;
        *head = *tail;
        *tail = tempBuffer;
    }
}

void eaClearDbg(mEArrayHandle* handle)
{
    if(*handle)
    {
        EArray* pArray = EArrayFromHandle(*handle);
        memset(pArray->structptrs, 0, sizeof(pArray->structptrs[0])*pArray->size);
    }
}

void eaCopyDbg(mEArrayHandle* dest, cccEArrayHandle* src, const char *file, int line)
{
    int size = eaSizeUnsafe(src);
    eaSetSizeDbg(dest, size, file, line);
    if(size)
    {
        EArray *d = EArrayFromHandle(*dest);
        EArray *s = EArrayFromHandle(*src);
        memcpy(d->structptrs, s->structptrs, sizeof(d->structptrs[0])*size);
    }
}

void eaCopyExDbg(cccEArrayHandle* src, mEArrayHandle* dst, int size, EArrayItemConstructor constructor, const char *file, int line)
{
    EArray    *src_array, *dst_array;
    int        i;

    if(!*src)
        return; // FIXME: bad semantics, this should probably set dst's size to 0
    src_array = EArrayFromHandle(*src);
    eaSetSizeDbg(dst, src_array->count, file, line);
    dst_array = EArrayFromHandle(*dst);

    for (i = 0; i < src_array->count; i++)
    {
        if (!dst_array->structptrs[i])
        {
            if (!constructor)
                dst_array->structptrs[i] = malloc(size);
            else
                dst_array->structptrs[i] = constructor(size);
        }
        memcpy(dst_array->structptrs[i],src_array->structptrs[i],size);
    }
}

void eaCompressDbg(mEArrayHandle *dst, cccEArrayHandle *src, CustomMemoryAllocator memAllocator, void *customData, const char *file, int line)
{
    const EArray *src_array;
    EArray *dst_array;
    size_t len;

    if (dst != src)
        *dst = NULL; // assume *dst contains garbage

    if(!*src)
        return;

    if (memAllocator)
    {
        len = eaMemUsage(src);
        src_array = EArrayFromHandle(*src);
        dst_array = memAllocator(customData, len);
        memcpy(dst_array, src_array, len);
        dst_array->size = dst_array->count;
        dst_array->flags |= EARRAY_FLAG_CUSTOMALLOC;
#ifdef FULLDEBUG
        if (isSharedMemory(dst_array))
            dst_array->flags |= EARRAY_FLAG_SHAREDMEMORY;
#endif
        *dst = HandleFromEArray(dst_array);
    }
    else
    {
        eaCopyDbg(dst, src, file, line);
    }
}

cccEArrayHandle eaFromPointerUnsafe(const void* ptr)
{
    static const void **ea = NULL;
    eaSetSizeDbg(cEACast(&ea), 1, __FILE__, __LINE__);
    ea[0] = ptr;
    return ea;
}

void eaSwap(cEArrayHandle* handle, int i, int j)
{
    void* temp;
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle || !INRANGE0(i, pArray->count) || !INRANGE0(j, pArray->count))
        return;
    temp = pArray->structptrs[i];
    pArray->structptrs[i] = pArray->structptrs[j];
    pArray->structptrs[j] = temp;
}

void eaMove(cEArrayHandle* handle, int dest, int src)
{
    void* temp;
    EArray* pArray = EArrayFromHandle(*handle);
    if(!*handle || !INRANGE0(src, pArray->count) || !INRANGE0(dest, pArray->count))
        return;
    temp = pArray->structptrs[src];
    if(src < dest)
        CopyStructsFromOffset(pArray->structptrs + src, 1, dest - src);
    else
        CopyStructsFromOffset(pArray->structptrs + dest + 1, -1, src - dest);
    pArray->structptrs[dest] = temp;
}

/////////////////////////////////////////////////////// EArray32 (int, f32) functions

// from within this file, you should be calling the *Dbg versions of these
#undef ea32Create
#undef ea32SetCapacity
#undef ea32SetSize
#undef ea32Push
#undef ea32PushUnique
#undef ea32PushArray
#undef ea32Insert
#undef ea32Copy
#undef ea32Append
#undef ea32Compress

void ea32CreateWithCapacityDbg(mEArray32Handle* handle, int capacity, const char *file, int line)
{
    EArray32* pArray;
    assert(capacity >= 0);
    if(capacity < 1)
        capacity = 1;
#if PRIVATE_EARRAY_HEAP
    if(!g_earrayheap)
    {
        g_earrayheap = HeapCreate(heap_flags, 0, 0);
        disableRtlHeapChecking(g_earrayheap);
    }
    pArray = HeapAlloc(g_earrayheap, 0, EARRAY32_HEADER_SIZE + sizeof(pArray->values[0])*(capacity));
    memMonitorTrackUserMemory(EARRAY_MEMMONITOR_NAME, 1, MOT_ALLOC, EARRAY32_HEADER_SIZE + sizeof(pArray->values[0])*(capacity));
#else
    pArray = _malloc_dbg(EARRAY32_HEADER_SIZE + sizeof(pArray->values[0])*(capacity), _NORMAL_BLOCK, file, line);
#endif
    memset(pArray, 0, EARRAY32_HEADER_SIZE); // this makes me feel better than setting the fields directly
//     pArray->count = 0;
//     pArray->flags = 0;
    pArray->size = capacity;
    *handle = HandleFromEArray32(pArray);
}

void ea32Destroy(mEArray32Handle* handle) // free list
{
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle)
        return;
    devassertmsg(!(pArray->flags & EARRAY_FLAG_CUSTOMALLOC), "trying to perform a heap operation on a compressed EArray");
#if PRIVATE_EARRAY_HEAP
    memMonitorTrackUserMemory(EARRAY_MEMMONITOR_NAME, 1, MOT_FREE, HeapSize(g_earrayheap, heap_flags, pArray));
    HeapFree(g_earrayheap, 0, pArray);
#else
    _free_dbg(pArray, _NORMAL_BLOCK);
#endif
    *handle = NULL;
}

void ea32SetCapacityDbg(mEArray32Handle* handle, int capacity, const char *file, int line) // grows or shrinks capacity, limits size if required
{
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle)
        return;    // fail silently.  i hate this behavior, shouldn't this be an ea32Create instead?
    devassertmsg(!(pArray->flags & EARRAY_FLAG_CUSTOMALLOC), "trying to perform a heap operation on a compressed EArray");
    pArray->size = capacity>1? capacity: 1;
#if PRIVATE_EARRAY_HEAP
    memMonitorTrackUserMemory(EARRAY_MEMMONITOR_NAME, 1, MOT_REALLOC, EARRAY32_HEADER_SIZE + sizeof(pArray->values[0])*(pArray->size) - HeapSize(g_earrayheap, heap_flags, pArray));
    pArray = HeapReAlloc(g_earrayheap, 0, pArray, EARRAY32_HEADER_SIZE + sizeof(pArray->values[0])*(pArray->size));
#else
    pArray = _realloc_dbg(pArray, EARRAY32_HEADER_SIZE + sizeof(pArray->values[0])*(pArray->size), _NORMAL_BLOCK, file, line);
#endif
    if(pArray->count > capacity)
        pArray->count = capacity;
    *handle = HandleFromEArray32(pArray);
}

int ea32Capacity(mEArray32Handle* handle) // just returns current capacity
{
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle)
        return 0;
    return pArray->size;
}

size_t ea32MemUsage(mEArray32Handle* handle) // get the amount of memory actually used (not counting slack allocated)
{
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle)
        return 0;
    return pArray->count*sizeof(pArray->values[0]) + EARRAY32_HEADER_SIZE;
}

void ea32SetSizeDbg(mEArray32Handle* handle, int size, const char *file, int line) // set the number of items in pArray (fills with zero)
{
    // this is a little confusing because Size = count within the pArray structure, Capacity = size
    EArray32* pArray;
    if(!*handle)
        ea32CreateWithCapacityDbg(handle, size, file, line);
    pArray = EArray32FromHandle(*handle);
    if (pArray->size < size) 
    {
        ea32SetCapacityDbg(handle, size, file, line);
        pArray = EArray32FromHandle(*handle);
    }
    if (pArray->count < size) // increase size
        memset(&pArray->values[pArray->count], 0, sizeof(pArray->values[0])*(size-pArray->count));
    pArray->count = size;
}

int ea32PushDbg(mEArray32Handle* handle, U32 value, const char *file, int line) // add to the end of the list, returns the index it was added at (the new size)
{
    EArray32* pArray;
    if(!*handle)
        ea32CreateWithCapacityDbg(handle, 1, file, line);
    pArray = EArray32FromHandle(*handle);
    assert(pArray->count <= pArray->size);
    if(pArray->count == pArray->size)
        ea32SetCapacityDbg(handle, eaIncreaseCapacity(pArray->size), file, line);
    pArray = EArray32FromHandle(*handle);
    pArray->values[pArray->count++] = value;
    return pArray->count-1;
}

int ea32PushUniqueDbg(mEArray32Handle* handle, U32 value, const char *file, int line) // add to the end of the list, returns the index it was added at (the new size)
{
    int idx = ea32Find(handle, value);
    if (idx < 0)
        idx = ea32PushDbg(handle, value, file, line);
    return idx;
}

int ea32PushArrayDbg(mEArray32Handle* handle, mEArray32Handle* src, const char *file, int line) // add to the end of the list, returns the index it was added at
{
    int count;
    if(!*handle)
        ea32CreateWithCapacityDbg(handle, ea32Size(src), file, line);
    count = ea32Size( handle );

    if (src)
    {
        int srcCount = ea32Size(src);
        ea32SetSizeDbg(handle, count + srcCount, file, line);
        CopyStructs(*handle + count, *src, srcCount);
    }

    return count;
}

U32 ea32Pop(mEArray32Handle* handle) // remove the last item from the list
{
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle || !pArray->count)
        return 0;
    pArray->count--;
    return pArray->values[pArray->count];
}

void ea32PopAll(mEArray32Handle* handle)
{
    if(*handle)
        EArray32FromHandle(*handle)->count = 0;
}

void ea32Set(mEArray32Handle* handle, U32 value, int i) // set i'th element (zero-based)
{
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle || !INRANGE0(i, pArray->count))
        return;
    pArray->values[i] = value;
}

U32 ea32Get(mEArray32Handle* handle, int i)    // get i'th element (zero-based), NULL on error
{
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle || !INRANGE0(i, pArray->count))
        return 0;
    return pArray->values[i];
}

U32 ea32Last(mEArray32Handle* handle)
{
    return ea32Get(handle, ea32Size(handle)-1);
}

void ea32InsertDbg(mEArray32Handle* handle, U32 value, int i, const char *file, int line) // insert before i'th position, will not insert on error (i == -1, etc.)
{
    EArray32* pArray;
    if(!*handle)
        ea32CreateWithCapacityDbg(handle, 1, file, line);
    pArray = EArray32FromHandle(*handle);
    if(i < 0 || i > pArray->count) // it's okay for i to equal count
        return;
    assert(pArray->count <= pArray->size);
    if(pArray->count == pArray->size)
        ea32SetCapacityDbg(handle, eaIncreaseCapacity(pArray->size), file, line);
    pArray = EArray32FromHandle(*handle);
    CopyStructsFromOffset(pArray->values + i + 1, -1, pArray->count - i);
    pArray->count++;
    pArray->values[i] = value;
}

U32 ea32Remove(mEArray32Handle* handle, int i) // remove the i'th element, NULL on error
{
    U32 value;
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle || !INRANGE0(i, pArray->count))
        return 0;
    value = pArray->values[i];
    CopyStructsFromOffset(pArray->values + i, 1, pArray->count - i - 1);
    pArray->count--;
    return value;
}

U32 ea32RemoveFast(mEArray32Handle* handle, int i) // remove the i'th element, and move the last element into this place DOES NOT KEEP ORDER
{    
    U32 value;
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle || !INRANGE0(i, pArray->count))
        return 0;
    value = pArray->values[i];
    pArray->values[i] = pArray->values[--pArray->count];
    return value;
}

int    ea32Find(mEArray32Handle* handle, U32 value)    // find the first element that matches structptr, returns -1 on error
{
    int i;
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle)
        return -1;
    for(i = 0; i < pArray->count; i++)
        if(pArray->values[i] == value)
            return i;
    return -1;
}

int    ea32FindAndRemove(mEArray32Handle* handle, U32 value)
{
    int    idx;
    if ((idx = ea32Find(handle,value)) >= 0)
        ea32Remove(handle,idx);
    return idx;
}

int    ea32FindAndRemoveFast(mEArray32Handle* handle, U32 value)
{
    int    idx;
    if ((idx = ea32Find(handle,value)) >= 0)
        ea32RemoveFast(handle,idx);
    return idx;
}

void ea32Reverse(mEArray32Handle* handle)
{
    EArray32* pArray = EArray32FromHandle(*handle);
    U32 *head, *tail;
    if(!*handle)
        return;

    head = pArray->values;
    tail = head + pArray->count - 1;
    
    // Start the index at the two "outer most" elements of the pArray.
    // Each iteration through the loop, swap the two elements that are indexed,
    // then bring the indices one step closer to each other.
    for(; head < tail; head++, tail--)
    {
        U32 temp = *head;
        *head = *tail;
        *tail = temp;
    }
}

void ea32Clear(mEArray32Handle* handle)
{
    if (*handle)
    {
        EArray32* pArray = EArray32FromHandle(*handle);
        memset(pArray->values, 0, sizeof(pArray->values[0])*pArray->size);
    }
}

void ea32CopyDbg(mEArray32Handle* dest, mEArray32Handle* src, const char *file, int line)
{
    int size = ea32Size(src);
    ea32SetSizeDbg(dest, size, file, line);
    if(size)
    {
        EArray32 *d = EArray32FromHandle(*dest);
        EArray32 *s = EArray32FromHandle(*src);
        memcpy(d->values, s->values, sizeof(d->values[0])*size);
    }
}

void ea32AppendDbg(mEArray32Handle* handle, U32 *values, int count, const char *file, int line)
{
    EArray32 *pArray;

    if(!count)
        return;

    if(!*handle)
        ea32CreateWithCapacityDbg(handle, count, file, line);

    pArray = EArray32FromHandle(*handle);
    if(pArray->count + count > pArray->size)
    {
        ea32SetCapacityDbg(handle, pArray->count + count, file, line);
        pArray = EArray32FromHandle(*handle);
    }

    memcpy(pArray->values + pArray->count, values, sizeof(U32)*count);
    pArray->count += count;
}

void ea32CompressDbg(mEArray32Handle *dst, mEArray32Handle *src, CustomMemoryAllocator memAllocator, void *customData, const char *file, int line)
{
    const EArray32 *src_array;
    EArray32 *dst_array;
    size_t len;

    *dst = NULL; // assume *dst contains garbage

    if(!*src)
        return;

    if (memAllocator)
    {
        len = ea32MemUsage(src);
        src_array = EArray32FromHandle(*src);
        dst_array = memAllocator(customData, len);
        memcpy(dst_array, src_array, len);
        dst_array->size = dst_array->count;
        dst_array->flags |= EARRAY_FLAG_CUSTOMALLOC;
        *dst = HandleFromEArray32(dst_array);
    }
    else
    {
        ea32CopyDbg(dst, src, file, line);
    }
}

void ea32Swap(mEArray32Handle* handle, int i, int j)
{
    U32 temp;
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle || !INRANGE0(i, pArray->count) || !INRANGE0(j, pArray->count))
        return;
    temp = pArray->values[i];
    pArray->values[i] = pArray->values[j];
    pArray->values[j] = temp;
}

void ea32Move(mEArray32Handle* handle, int dest, int src)
{
    U32 temp;
    EArray32* pArray = EArray32FromHandle(*handle);
    if(!*handle || !INRANGE0(src, pArray->count) || !INRANGE0(dest, pArray->count))
        return;
    temp = pArray->values[src];
    if(src < dest)
        CopyStructsFromOffset(pArray->values + src, 1, dest - src);
    else
        CopyStructsFromOffset(pArray->values + dest + 1, -1, src - dest);
    pArray->values[dest] = temp;
}

/////////////////////////////////////////////////// eai util

int eaiCompare(int** array1, int** array2)
{
    int len1, len2;
    int i;
    len1 = ea32Size(array1);
    len2 = ea32Size(array2);
    if (len1 != len2)
        return len1 - len2;
    if (len1 <= 0)
        return 0;
    for (i=0; i<len1-1 && (*array1)[i]==(*array2)[i]; i++)
        ;
    return (*array1)[i]-(*array2)[i];
}

static int s_eaiSortedCmp(const void* keyData, const void* elemData)
{
    const int * key = (const int *)keyData;
    const int * elem = (const int *)elemData;
    return *key - *elem;
}

int eaiSortedFind(eaiHandle *array, int elem)
{
    if(*array)
    {
        int i = (int)bfind(&elem, *array, eaiSize(array), sizeof(int), s_eaiSortedCmp);
        if(i < eaiSize(array) && (*array)[i] == elem)
            return i;
    }
    return -1;
}

int eaiSortedFindNext(eaiHandle *array, int elem)
{
    int i = 0;
    if(*array)
    {
        i = (int)bfind(&elem, *array, eaiSize(array), sizeof(int), s_eaiSortedCmp);
        if(i < eaiSize(array) && (*array)[i] == elem)
            i++;
    }
    return i;
}

int eaiSortedFindAndRemove(eaiHandle *array, int elem)
{
    int i = eaiSortedFind(array, elem);
    if(i >= 0)
        eaiRemove(array, i);
    return i;
}

int eaiSortedPushDbg(eaiHandle *array, int elem, const char *file, int line)
{
    int i = *array ? (int)bfind(&elem, *array, eaiSize(array), sizeof(int), s_eaiSortedCmp) : 0;
    eaiInsertDbg(array, elem, i, file, line);
    return i;
}

int eaiSortedPushUniqueDbg(eaiHandle *array, int elem, const char *file, int line)
{
    int i = *array ? (int)bfind(&elem, *array, eaiSize(array), sizeof(int), s_eaiSortedCmp) : 0;
    if(i >= eaiSize(array) || (*array)[i] != elem)
        eaiInsertDbg(array, elem, i, file, line);
    return i;
}

int eaiSortedPushAssertUniqueDbg(eaiHandle *array, int elem, const char *file, int line)
{
    int i = *array ? (int)bfind(&elem, *array, eaiSize(array), sizeof(int), s_eaiSortedCmp) : 0;
    assert(i == eaiSize(array) || (*array)[i] != elem);
    eaiInsertDbg(array, elem, i, file, line);
    return i;
}

void eaiSortedIntersectionDbg(eaiHandle *arrayDst, eaiHandle *array1, eaiHandle *array2, const char *file, int line)
{
    int count = 0;
    int i1, i2;

    if(!eaiSize(array1) || !eaiSize(array2))
    {
        eaiPopAll(arrayDst);
        return;
    }

    for(i1 = 0, i2 = 0;;)
    {
        if((*array1)[i1] < (*array2)[i2])
        {
            if(++i1 >= eaiSize(array1))
                break;
        }
        else if((*array2)[i2] < (*array1)[i1])
        {
            if(++i2 >= eaiSize(array2))
                break;
        }
        else // (*array2)[i1] == (*array1)[i2]
        {
            if(!*arrayDst)
                eaiCreateDbg(arrayDst, file, line);
            if(count >= EArray32FromHandle(*arrayDst)->size)
                eaiSetCapacityDbg(arrayDst, count*2, file, line);

            (*arrayDst)[count++] = (*array1)[i1];

            if(++i1 >= eaiSize(array1))
                break;
            if(++i2 >= eaiSize(array2))
                break;
        }
    }

    if(*arrayDst)
        EArray32FromHandle(*arrayDst)->count = count;
}

void eaiSortedDifferenceDbg(eaiHandle *arrayDst, eaiHandle *array1, eaiHandle *array2, const char *file, int line)
{
    int count = 0;
    int i1, i2;

    assert(arrayDst != array2); // what are you thinking?

    if(!eaiSize(array1))
    {
        eaiPopAll(arrayDst);
        return;
    }

    if(!eaiSize(array2))
    {
        if(arrayDst != array1)
            eaiCopyDbg(arrayDst, array1, file, line);
        return;
    }

    for(i1 = 0, i2 = 0;;)
    {

        if((*array1)[i1] < (*array2)[i2])
        {
            if(!*arrayDst)
                eaiCreateDbg(arrayDst, file, line);
            if(count >= EArray32FromHandle(*arrayDst)->size)
                eaiSetCapacityDbg(arrayDst, count*2, file, line);

            (*arrayDst)[count++] = (*array1)[i1];
            if(++i1 >= eaiSize(array1))
                break;

        }
        else if((*array2)[i2] < (*array1)[i1])
        {
            if(++i2 >= eaiSize(array2))
                break;
        }
        else // (*array2)[i1] == (*array1)[i2]
        {

            if(++i1 >= eaiSize(array1))
                break;
            if(++i2 >= eaiSize(array2))
                break;
        }
    }

    if(i1 < eaiSize(array1))
    {
        // note that eaiAppend isn't safe here, because it uses memcpy
        if(!*arrayDst)
            eaiCreateDbg(arrayDst, file, line);
        if(count + eaiSize(array1) - i1 > EArray32FromHandle(*arrayDst)->size)
            eaiSetCapacityDbg(arrayDst, count + eaiSize(array1) - i1, file, line);
        for(; i1 < eaiSize(array1); i1++)
            (*arrayDst)[count++] = (*array1)[i1];
    }

    if(*arrayDst)
        EArray32FromHandle(*arrayDst)->count = count;
}

void eaiSortedUnionDbg(eaiHandle *arrayDst, eaiHandle *array1, eaiHandle *array2, const char *file, int line)
{
    int i1 = 0, i2 = 0;
    int size1,size2;
    int placed = 0;

    assert(arrayDst != array1 && arrayDst != array2); // safe for intersection, not union
    size1 = eaiSize(array1);
    size2 = eaiSize(array2);

    if(!size1 && !size2)
    {
        eaiPopAll(arrayDst);
        return;
    }

    if(!*arrayDst)
        eaiCreateDbg(arrayDst, file, line);
    if(EArray32FromHandle(*arrayDst)->size < size1+size2)
        eaiSetCapacityDbg(arrayDst, size1+size2, file, line);

    if(size1 && size2)
    {
        for(;;)
        {
            if((*array1)[i1] < (*array2)[i2])
            {
                (*arrayDst)[placed++] = (*array1)[i1];
                if(++i1 >= size1)
                    break;
            }
            else if((*array2)[i2] < (*array1)[i1])
            {
                (*arrayDst)[placed++] = (*array2)[i2];
                if(++i2 >= size2)
                    break;
            }
            else // (*array2)[i1] == (*array1)[i2]
            {
                (*arrayDst)[placed++] = (*array1)[i1];
                i1++;
                i2++;
                if(i1 >= size1)
                    break;
                if(i2 >= size2)
                    break;
            }
        }
    }
    EArray32FromHandle(*arrayDst)->count = placed;

    if(i1 < size1)
    {
        eaiAppendDbg(arrayDst, (*array1) + i1, size1 - i1, file, line);
        placed += size1 - i1;
    }
    else if(i2 < size2)
    {
        eaiAppendDbg(arrayDst, (*array2) + i2, size2 - i2, file, line);
        placed += size2 - i2;
    }
}

/////////////////////////////////////////////////// eaf util

int eafCompare(F32** array1, F32** array2)
{
    int len1, len2;
    int i;
    len1 = eafSize(array1);
    len2 = eafSize(array2);
    if (len1 != len2)
        return len1 - len2;
    if (len1 <= 0)
        return 0;
    for (i=0; i<len1-1 && (*array1)[i]==(*array2)[i]; i++)
        ;
    return (*array1)[i]-(*array2)[i];
}

/////////////////////////////////////////////////// StringArray util

// returns index, or -1
int StringArrayFind(const char * const * const pArray, const char* elem)
{
    int result = -1;
    int i, n = eaSize(&pArray);
    for (i = 0; i < n; i++)
    {
        if (!stricmp(pArray[i], elem))
        {
            result = i;
            break;
        }
    }
    return result;
}

// dumb implementation of this, just something for small arrays
int StringArrayIntersectionDbg(char*** result, char * const * lhs, char * const * rhs, const char *file, int line)
{
    int i, n;
	eaSetSizeDbg(mEACast(result), 0, file, line);
    n = eaSize(&lhs);
    for (i = 0; i < n; i++)
    {
		if (StringArrayFind((const char *const *)rhs, lhs[i]) >= 0 &&
			StringArrayFind((const char *const *)*result,
				lhs[i]) < 0)
        {
			eaPushDbg(mEACast(result), lhs[i], file, line);
        }
    }
    return eaSize(result);
}

int StringArrayIntersectionDbgConst(const char*** result, const char * const * lhs, const char * const * rhs, const char *file, int line)
{
    return StringArrayIntersectionDbg(cpp_const_cast(char***)(result), cpp_const_cast(char * const *)(lhs), cpp_const_cast(char * const *)(rhs), file, line);
}

void StringArrayPrint(char* buf, int buflen, const char * const * pArray)
{
#define PUSH(str) \
    { if (buflen <= 1) return; \
    strncpyt(bufptr, str, buflen); \
    len = (int)strlen(bufptr); \
    buflen -= len; \
    bufptr += len; }

    int i, n, len;
    char* bufptr = buf; 
    *bufptr = '\0';
    PUSH("(")
    n = eaSize(&pArray);
    for (i = 0; i < n; i++)
    {
        if (i != 0)
            PUSH(", ")
        PUSH(pArray[i])
    }
    PUSH(")")
#undef PUSH
}

int eags(mEArrayHandle* handle)
{
    return eaSizeUnsafe(handle);
}

int eags32(mEArray32Handle* handle)
{
    return ea32Size(handle);
}

int eagsf(meafHandle* handle)
{
    return ea32Size(mEA32Cast(handle));
}

int eagsi(meaiHandle* handle)
{
    return ea32Size(handle);
}
