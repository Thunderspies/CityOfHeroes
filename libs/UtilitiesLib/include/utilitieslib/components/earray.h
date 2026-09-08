// earray.h - provides yet another type of expandable array
// these arrays are differentiated in that you generally declare them as
// MyStruct** earray; and access them like a normal array of pointers
// NOTE - EArrays are now threadsafe

#ifndef __EARRAY_H
#define __EARRAY_H

#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

#define PRIVATE_EARRAY_HEAP    0

//////////////////////////////////////////////////// EArray's (64-bit compatible)
// you will usually declare these as MyStruct**, and then use them to
// store pointers to stuff of interest.  

// Const data
typedef void** mEArrayHandle;                                // non-const pointer to a list of pointers
typedef void * const * const ccmEArrayHandle;                // non-const pointer to a const list of const pointers

// Non-const data
typedef const void ** cEArrayHandle;                        // const pointer to a list of pointers
typedef const void * const * const cccEArrayHandle;            // const pointer to a const list of const pointers

typedef void (*EArrayItemDestructor)(void*);
typedef void* (*EArrayItemConstructor)(int size);
typedef void* (*CustomMemoryAllocator)(void* data, size_t size);

/* Adapt addresses of typed array handles to the shared storage API.
 * Handles must be non-NULL; their array may be NULL until created or grown.
 * Const variants preserve element constness while allowing array mutation.
 */
#define mEACast(handle) (cpp_reinterpret_cast(mEArrayHandle*)(handle))
#define cEACast(handle) \
	(cpp_const_cast(mEArrayHandle*)( \
		cpp_reinterpret_cast(cEArrayHandle*)(handle)))
#define ccmEACast(handle) (cpp_reinterpret_cast(ccmEArrayHandle*)(handle))
#define cccEACast(handle) (cpp_reinterpret_cast(cccEArrayHandle*)(handle))

// Mutable array storage whose element values remain read-only.
#define cEAHandleCast(handle) (cpp_const_cast(cEArrayHandle*)(cpp_reinterpret_cast(cccEArrayHandle*)(handle)))

// Use eags() in a debugger
#define eaSize(handle)                            (EAPtrTest(handle), eaSizeUnsafe(handle))
#define eaSizeUnsafe(handle)                    (*(handle) ? EArrayFromHandle(*(handle))->count : 0) // should only be used for void***
int        eaCapacity(cccEArrayHandle* handle);    // get the current number of items that can be held without growing
size_t    eaMemUsage(cccEArrayHandle* handle);    // get the amount of memory actually used (not counting slack allocated)

#define eaPushArray(handle, src) \
	eaPushArrayDbg(mEACast(handle), cccEACast(src), __FILE__, __LINE__)
#define eaPushArrayConst(handle, src)            eaPushArrayDbg(cEACast(handle), cccEACast(src), __FILE__, __LINE__)
#define eaCopy(dest, src) \
	eaCopyDbg(mEACast(dest), cccEACast(src), __FILE__, __LINE__)
#define eaCopyConst(dest, src)                    eaCopyDbg(cEACast(dest), cccEACast(src), __FILE__, __LINE__)
#define eaCompress(dest, src, alloc, data) \
	eaCompressDbg(mEACast(dest), cccEACast(src), alloc, data, __FILE__, \
	__LINE__)
#define eaCompressConst(dest, src, alloc, data) \
	eaCompressDbg(cEACast(dest), cccEACast(src), alloc, data, __FILE__, \
	__LINE__)

#define eaCreate(handle)                        eaCreateWithCapacityDbg(mEACast(handle), 1, __FILE__, __LINE__)
#define eaCreateConst(handle)                    eaCreateWithCapacityDbg(cEACast(handle), 1, __FILE__, __LINE__)
#define eaCreateWithCapacity(handle, cap)        eaCreateWithCapacityDbg(mEACast(handle), cap, __FILE__, __LINE__)
#define eaCreateWithCapacityConst(handle, cap)    eaCreateWithCapacityDbg(cEACast(handle), cap, __FILE__, __LINE__)
#define eaCreateSmallocSafe(handle) \
	eaCreateWithCapacityDbg(mEACast(handle), EARRAY_SMALLOC_SAFE_COUNT, \
	__FILE__, __LINE__)
#define eaSetCapacity(handle, cap)                eaSetCapacityDbg(mEACast(handle), cap, __FILE__, __LINE__)
#define eaSetCapacityConst(handle, cap)            eaSetCapacityDbg(cEACast(handle), cap, __FILE__, __LINE__)
#define eaSetSize(handle, size)                    eaSetSizeDbg(mEACast(handle), size, __FILE__, __LINE__)
#define eaSetSizeConst(handle, size)            eaSetSizeDbg(cEACast(handle), size, __FILE__, __LINE__)
#define eaDestroy(handle)                        eaDestroyDbg(mEACast(handle))
#define eaDestroyConst(handle)                    eaDestroyDbg(cEACast(handle))

#define eaPush(handle, structptr) \
	eaPushDbg(mEACast(handle), cpp_reinterpret_cast(void*)(structptr), \
	__FILE__, __LINE__)
#define eaPushConst(handle, structptr) \
	eaPushDbg(cEACast(handle), \
	cpp_const_cast(void*)(cpp_reinterpret_cast(const \
	void*)(structptr)), __FILE__, __LINE__)
#define eaPushUnique(handle, structptr) \
	eaPushUniqueDbg(mEACast(handle), \
	cpp_reinterpret_cast(void*)(structptr), __FILE__, __LINE__)
#define eaPushUniqueConst(handle, structptr) \
	eaPushUniqueDbg(cEACast(handle), \
	cpp_const_cast(void*)(cpp_reinterpret_cast(const \
	void*)(structptr)), __FILE__, __LINE__)
#define eaPushFront(handle, structptr)            eaInsert(handle,structptr,0)
#define eaPop(handle)                            eaPopDbg(mEACast(handle))
#define eaPopConst(handle) \
	eaPopConstDbg(cpp_reinterpret_cast(cEArrayHandle*)(handle))
#define eaClear(handle)                            eaClearDbg(mEACast(handle))
#define eaClearConst(handle)                    eaClearDbg(cEACast(handle))

#define eaSet(handle, structptr, i)                eaSetDbg(mEACast(handle), structptr, i)
#define eaSetConst(handle, structptr, i) \
	eaSetDbg(cEACast(handle), \
	cpp_const_cast(void*)(cpp_reinterpret_cast(const \
	void*)(structptr)), i)
#define eaSetForced(handle, structptr, i)        eaSetForcedDbg(mEACast(handle), structptr, i, __FILE__, __LINE__)
#define eaSetForcedConst(handle, structptr, i) \
	eaSetForcedDbg(cEACast(handle), \
	cpp_const_cast(void*)(cpp_reinterpret_cast(const \
	void*)(structptr)), i, __FILE__, __LINE__)
#define eaGet(handle, i)                        eaGetDbg(ccmEACast(handle), i)
#define eaGetConst(handle, i)                    eaGetConstDbg(cccEACast(handle), i)
#define eaLast(handle)                            eaLastDbg(ccmEACast(handle))
#define eaLastConst(handle)                        eaLastConstDbg(cccEACast(handle))
#define eaInsert(handle, structptr, i)            eaInsertDbg(mEACast(handle), structptr, i, __FILE__, __LINE__)
#define eaInsertConst(handle, structptr, i) \
	eaInsertDbg(cEACast(handle), \
	cpp_const_cast(void*)(cpp_reinterpret_cast(const \
	void*)(structptr)), i, __FILE__, __LINE__)
#define eaRemove(handle, i)                        eaRemoveDbg(mEACast(handle), i)
#define eaRemoveConst(handle, i)                eaRemoveConstDbg(cEAHandleCast(handle), i)
void    eaRemoveAndDestroy(mEArrayHandle* handle, int i, EArrayItemDestructor destructor);                // remove and destroy the i'th element. silent fail.
#define eaRemoveFast(handle, i)                    eaRemoveFastDbg(mEACast(handle), i)
#define eaRemoveFastConst(handle, i)            eaRemoveFastConstDbg(cEAHandleCast(handle), i)
#define eaFind(handle, structptr)                eaFindDbg(cpp_reinterpret_cast(cccEArrayHandle*)(handle), structptr)
#define eaFindAndRemove(handle, structptr)        eaFindAndRemoveDbg(cEAHandleCast(handle), structptr)
#define eaFindAndRemoveFast(handle, structptr)    eaFindAndRemoveFastDbg(cEAHandleCast(handle), structptr)
void    eaSwap(cEArrayHandle* handle, int i, int j);            // exchange the i'th element with the j'th element
void    eaMove(cEArrayHandle* handle, int dest, int src);    // shift left or right to move the src'th element to dest
void    eaReverse(cEArrayHandle* handle);

// deep copy stuff..
#define eaClearEx(handle, destructor)            eaClearExDbg(mEACast(handle), destructor)
#define eaClearExConst(handle, destructor)        eaClearExDbg(cEACast(handle), destructor)
#define eaDestroyEx(handle, destructor)            eaDestroyExDbg(mEACast(handle), destructor)
#define eaDestroyExConst(handle, destructor)    eaDestroyExDbg(cEACast(handle), destructor)
#define eaCopyEx(psrc, pdst, size, ctor) \
	eaCopyExDbg(cccEACast(psrc), mEACast(pdst), size, ctor, __FILE__, \
	__LINE__)
#define eaCopyExConst(psrc, pdst, size, ctor)    eaCopyExDbg(cccEACast(psrc), cEACast(pdst), size, ctor, __FILE__, __LINE__)

cccEArrayHandle eaFromPointerUnsafe(const void* ptr);    // convert temporarily, returns static buffer

#define eaQSort(handle, comparator) \
	(eaSize(&(handle)) ? qsort((cpp_reinterpret_cast(void*)(handle)), \
	eaSize(&(handle)), sizeof((handle)[0]), (comparator)) : (void)0)
#define eaQSortConst(handle, comparator) \
	(eaSize(&(handle)) ? \
	qsort((cpp_const_cast(void*)(cpp_reinterpret_cast(const \
	void*)(handle))), eaSize(&(handle)), sizeof((handle)[0]), \
	(comparator)) : (void)0)
#define eaQSortG(handle, comparator) \
	(eaSize(&(handle)) ? qsortG((cpp_reinterpret_cast(void*)(handle)), \
	eaSize(&(handle)), sizeof((handle)[0]), (comparator)) : (void)0)
#define eaBSearch(handle, comparator, key) \
	(eaSize(&(handle)) ? bsearch(&(key), \
	(cpp_reinterpret_cast(void*)(handle)), eaSize(&(handle)), \
	sizeof((handle)[0]), (comparator)) : NULL)
#define eaBFind(handle, comparator, key) \
	(eaSize(&(handle)) ? (int)bfind(&(key), \
	(cpp_reinterpret_cast(void*)(handle)), eaSize(&(handle)), \
	sizeof((handle)[0]), (comparator)) : 0)
#define eaSortedInsert(handle, comparator, key)    eaInsert(handle, key, eaBFind(*(handle), comparator, key))
int eaValidateHeap(void);

/////////////////////////////////////////////////// StringArray util
//   These are some extra util functions for char** that are EArrays
//   They assume you want case-insensitivity

typedef char** StringArray;
int StringArrayFind(const char * const * const array, const char* elem);    // returns index, or -1
#define StringArrayIntersection(result, lhs, rhs)        StringArrayIntersectionDbg(result, lhs, rhs, __FILE__, __LINE__)        // dumb implementation, just for small arrays
#define StringArrayIntersectionConst(result, lhs, rhs)    StringArrayIntersectionDbgConst(result, lhs, rhs, __FILE__, __LINE__)    // dumb implementation, just for small arrays
int StringArrayIntersectionDbg(char*** result, char * const * lhs, char * const * rhs, const char *file, int line);
int StringArrayIntersectionDbgConst(const char*** result, const char * const * lhs, const char * const * rhs, const char *file, int line);
void StringArrayPrint(char* buf, int buflen, const char * const * array);    // makes "(str, str, str)"


//////////////////////////////////////////////////// EArray32's (int, f32)

typedef U32* mEArray32Handle;
typedef const U32* cEArray32Handle;

typedef U32* const cmEArray32Handle;
typedef const U32* const ccEArray32Handle;

#define mEA32Cast(handle)                                    (cpp_reinterpret_cast(mEArray32Handle*)(handle))
#define cEA32Cast(handle) \
	(cpp_const_cast(mEArray32Handle*)( \
		cpp_reinterpret_cast(cEArray32Handle*)(handle)))
#define cmEA32Cast(handle)                                    (cpp_reinterpret_cast(cmEArray32Handle*)(handle))
#define ccEA32Cast(handle) \
	(cpp_const_cast(cmEArray32Handle*)( \
		cpp_reinterpret_cast(ccEArray32Handle*)(handle)))

#define ea32Size(handle)                                    (EA32PtrTest(handle), (*(handle) ? EArray32FromHandle(*(handle))->count : 0))
int        ea32Capacity(mEArray32Handle* handle);                // get the current number of items that can be held without growing
size_t    ea32MemUsage(mEArray32Handle* handle);                // get the amount of memory actually used (not counting slack allocated)

#define ea32Create(handle) \
	ea32CreateWithCapacityDbg(mEA32Cast(handle), 1, __FILE__, __LINE__)
#define ea32CreateWithCapacity(handle, cap) \
	ea32CreateWithCapacityDbg(mEA32Cast(handle), cap, __FILE__, __LINE__)
#define ea32CreateSmallocSafe(handle) \
	ea32CreateWithCapacityDbg(mEA32Cast(handle), \
	EARRAY32_SMALLOC_SAFE_COUNT, __FILE__, __LINE__)
#define ea32SetCapacity(handle, cap) \
	ea32SetCapacityDbg(mEA32Cast(handle), cap, __FILE__, __LINE__)
#define ea32SetSize(handle, size) \
	ea32SetSizeDbg(mEA32Cast(handle), size, __FILE__, __LINE__)
void    ea32Destroy(mEArray32Handle* handle);                // free list

#define ea32Push(handle, value) \
	ea32PushDbg(mEA32Cast(handle), value, __FILE__, __LINE__)
#define ea32PushUnique(handle, value) \
	ea32PushUniqueDbg(mEA32Cast(handle), value, __FILE__, __LINE__)
#define ea32PushArray(handle, src)                            ea32PushArrayDbg(handle, src, __FILE__, __LINE__)
#define ea32Insert(handle, value, i) \
	ea32InsertDbg(mEA32Cast(handle), value, i, __FILE__, __LINE__)
U32        ea32Pop(mEArray32Handle* handle);                    // remove the last item from the list
void    ea32PopAll(mEArray32Handle* handle);                    // empty the list
void    ea32Reverse(mEArray32Handle* handle);
void    ea32Clear(mEArray32Handle* handle);                    // sets all elements to 0
#define ea32Copy(dest, src) \
	ea32CopyDbg(mEA32Cast(dest), mEA32Cast(src), __FILE__, __LINE__)
#define ea32Append(handle, values, count) \
	ea32AppendDbg(mEA32Cast(handle), (U32*)(values), count, __FILE__, \
	__LINE__)
#define ea32Compress(dst, src, alloc, data) \
	ea32CompressDbg(mEA32Cast(dst), mEA32Cast(src), alloc, data, \
	__FILE__, __LINE__)

void    ea32Set(mEArray32Handle* handle, U32 value, int i);    // set i'th element (zero-based)
U32        ea32Get(mEArray32Handle* handle, int i);                // get i'th element (zero-based), 0 on error
U32        ea32Last(mEArray32Handle* handle);                    // get the size-1'th element, 0 on error
U32        ea32Remove(mEArray32Handle* handle, int i);            // remove the i'th element, NULL on error
U32        ea32RemoveFast(mEArray32Handle* handle, int i);        // remove the i'th element, and move the last element into this place DOES NOT KEEP ORDER
int        ea32Find(mEArray32Handle* handle, U32 value);    // find the first element that matches structptr, returns -1 on error
int        ea32FindAndRemove(mEArray32Handle* handle, U32 value); // finds element, deletes it, returns same value as eaFind
int        ea32FindAndRemoveFast(mEArray32Handle* handle, U32 value); // finds element, deletes it, returns same value as eaFind DOES NOT KEEP ORDER
void    ea32Swap(mEArray32Handle* handle, int i, int j);        // exchange the i'th element with the j'th element
void    ea32Move(mEArray32Handle* handle, int dest, int src);// shift left or right to move the src'th element to dest

#define ea32QSort(handle, comparator) qsort((handle), ea32Size(&(handle)), sizeof((handle)[0]), (comparator))


/////////////////////////////////////////////////// eai's
// The same internals as EArray32.  Declare as int*.  
typedef int* meaiHandle;

#define eaiCreate(handle)                    (EAIntPtrTest(handle), ea32Create(mEA32Cast(handle)))
#define eaiCreateSmallocSafe(handle)        (EAIntPtrTest(handle), ea32CreateSmallocSafe(mEA32Cast(handle)))
#define eaiCreateWithCapacity(handle, cap)    (EAIntPtrTest(handle), ea32CreateWithCapacity(mEA32Cast(handle), cap))
#define eaiDestroy(handle)                    (EAIntPtrTest(handle), ea32Destroy(mEA32Cast(handle)))
#define eaiSetSize(handle, size)            (EAIntPtrTest(handle), ea32SetSize(mEA32Cast(handle), size))
#define eaiSize(handle)                        (EAIntPtrTest(handle), ea32Size(ccEA32Cast(handle)))
#define eaiSetCapacity(handle, size)        (EAIntPtrTest(handle), ea32SetCapacity(mEA32Cast(handle), size))
#define eaiCapacity(handle)                    (EAIntPtrTest(handle), ea32Capacity(mEA32Cast(handle)))
#define eaiMemUsage(handle)                    (EAIntPtrTest(handle), ea32MemUsage(mEA32Cast(handle)))
#define eaiCompress(dest, src, alloc, param)(EAIntPtrTest(dest), EAIntPtrTest(src), ea32Compress(mEA32Cast(dest), mEA32Cast(src), alloc, param))

#define eaiPush(handle, elem)                (EAIntPtrTest(handle), ea32Push(mEA32Cast(handle), (U32)(elem)))
#define eaiPushUnique(handle, elem)            (EAIntPtrTest(handle), ea32PushUnique(mEA32Cast(handle), (U32)(elem)))
#define eaiPop(handle)                        (EAIntPtrTest(handle), (int)ea32Pop(mEA32Cast(handle)))
#define eaiPopAll(handle)                    (EAIntPtrTest(handle), ea32PopAll(mEA32Cast(handle)))
#define eaiReverse(handle)                    (EAIntPtrTest(handle), ea32Reverse(mEA32Cast(handle)))
#define eaiClear(handle)                    (EAIntPtrTest(handle), ea32Clear(mEA32Cast(handle)))
#define eaiCopy(dest, src)                    (EAIntPtrTest(dest), EAIntPtrTest(src), ea32Copy(mEA32Cast(dest), mEA32Cast(src)))
#define eaiAppend(handle, values, count)    (EAIntPtrTest(handle), ea32Append(mEA32Cast(handle), values, count))

#define eaiSet(handle, elem, i)                (EAIntPtrTest(handle), ea32Set(mEA32Cast(handle), (U32)(elem), i))
#define eaiGet(handle, i)                    (EAIntPtrTest(handle), (int)ea32Get(mEA32Cast(handle), i))
#define eaiLast(handle)                        (EAIntPtrTest(handle), (int)ea32Last(mEA32Cast(handle)))
#define eaiInsert(handle, elem, i)            (EAIntPtrTest(handle), ea32Insert(mEA32Cast(handle), (U32)(elem), i))
#define eaiRemove(handle, i)                (EAIntPtrTest(handle), (int)ea32Remove(mEA32Cast(handle), i))
#define eaiRemoveFast(handle, i)            (EAIntPtrTest(handle), (int)ea32RemoveFast(mEA32Cast(handle), i))
#define eaiFind(handle, elem)                (EAIntPtrTest(handle), ea32Find(mEA32Cast(handle), (U32)(elem)))
#define eaiFindAndRemove(handle, elem)        (EAIntPtrTest(handle), ea32FindAndRemove(mEA32Cast(handle), (U32)(elem)))
#define eaiFindAndRemoveFast(handle, elem)    (EAIntPtrTest(handle), ea32FindAndRemoveFast(mEA32Cast(handle), (U32)(elem)))
#define eaiSwap(handle, i, j)                (EAIntPtrTest(handle), ea32Swap(mEA32Cast(handle), i, j))
#define eaiMove(handle, dest, src)            (EAIntPtrTest(handle), ea32Move(mEA32Cast(handle), dest, src))

// type-specific util
int eaiCompare(meaiHandle *array1, meaiHandle *array2);

// sorted int arrays
#define eaiSortedPush(array, elem)                        eaiSortedPushDbg(array, elem, __FILE__, __LINE__)
#define eaiSortedPushUnique(array, elem)                eaiSortedPushUniqueDbg(array, elem, __FILE__, __LINE__)
#define eaiSortedPushAssertUnique(array, elem)            eaiSortedPushAssertUniqueDbg(array, elem, __FILE__, __LINE__)
int eaiSortedFind(meaiHandle *array, int elem);
int eaiSortedFindNext(meaiHandle *array, int elem); // returns the index of the first element greater than elem or size
int eaiSortedFindAndRemove(meaiHandle *array, int elem);
#define eaiSortedIntersection(arrayDst, array1, array2)    eaiSortedIntersectionDbg(arrayDst, array1, array2, __FILE__, __LINE__)    // arrayDst may be array1 or array2
#define eaiSortedDifference(arrayDst, array1, array2)    eaiSortedDifferenceDbg(arrayDst, array1, array2, __FILE__, __LINE__)    // arrayDst may be array1 but not array2
#define eaiSortedUnion(arrayDst, array1, array2)        eaiSortedUnionDbg(arrayDst, array1, array2, __FILE__, __LINE__)            // arrayDst must not be array1 or array2. maintains uniqueness.


/////////////////////////////////////////////////// eaf's
// The same internals as EArray32.  Declare as F32*.  
typedef F32* meafHandle;

#define eafCreate(handle)                    (EAF32PtrTest(handle), ea32Create(mEA32Cast(handle)))
#define eafCreateSmallocSafe(handle)        (EAF32PtrTest(handle), ea32CreateSmallocSafe(mEA32Cast(handle)))
#define eafDestroy(handle)                    (EAF32PtrTest(handle), ea32Destroy(mEA32Cast(handle)))
#define eafSetSize(handle, size)            (EAF32PtrTest(handle), ea32SetSize(mEA32Cast(handle), size))
#define eafSize(handle)                        (EAF32PtrTest(handle), ea32Size(mEA32Cast(handle)))
#define eafSetCapacity(handle, size)        (EAF32PtrTest(handle), ea32SetCapacity(mEA32Cast(handle), size))
#define eafCapacity(handle)                    (EAF32PtrTest(handle), ea32Capacity(mEA32Cast(handle)))
#define eafMemUsage(handle)                    (EAF32PtrTest(handle), ea32MemUsage(mEA32Cast(handle)))
#define eafCompress(dest, src, alloc, param)(EAF32PtrTest(dest), EAF32PtrTest(src), ea32Compress(mEA32Cast(dest), mEA32Cast(src), alloc, param))

static INLINEDBG int eafPush(meafHandle *handle, F32 val);
static INLINEDBG F32 eafPop(meafHandle *handle);

#define eafReverse(handle)                    (EAF32PtrTest(handle), ea32Reverse(mEA32Cast(handle)))
#define eafClear(handle)                    (EAF32PtrTest(handle), ea32Clear(mEA32Cast(handle)))
#define eafCopy(dest, src)                    (EAF32PtrTest(dest), EAF32PtrTest(src), ea32Copy(mEA32Cast(dest), mEA32Cast(src)))

#define eafSwap(handle, i, j)                (EAF32PtrTest(handle), ea32Swap(mEA32Cast(handle), i, j))
#define eafMove(handle, dest, src)            (EAF32PtrTest(handle), ea32Move(mEA32Cast(handle), dest, src))

// type-specific util
int eafCompare(meafHandle *array1, meafHandle *array2);


////////////////////////////////////////////////////////// private

#define EARRAY_FLAG_CUSTOMALLOC            (1 << 0)
#define EARRAY_FLAG_SHAREDMEMORY        (1 << 1) // currently only set for FULLDEBUG

// #define EARRAY_CRAZY_DEBUGGING 1

// Struct-related inlined functions
typedef struct EArray
{
    int count;
    int size;
    U32 flags;
    void* structptrs[1];
} EArray;
#define EARRAY_HEADER_SIZE            (offsetof(EArray,structptrs))
#define EARRAY_SMALLOC_SAFE_COUNT    ( MEMALLOC_NOT_SMALLOC < sizeof(EArray) ? 1 : (MEMALLOC_NOT_SMALLOC-EARRAY_HEADER_SIZE+sizeof(void*)-1)/sizeof(void*) )

#if EARRAY_CRAZY_DEBUGGING
    EArray* EArrayFromHandle(const void *handle);
#else
#    define EArrayFromHandle(handle) ((EArray*)(((uintptr_t)handle) - EARRAY_HEADER_SIZE))
#endif
#define HandleFromEArray(array) ((mEArrayHandle)(((uintptr_t)array) + EARRAY_HEADER_SIZE))

#ifdef __cplusplus
extern "C++" {
template <class T>
static INLINEDBG void EAPtrTest(T const * const * const * ptr) {}
}
#elif defined(__GNUC__)
/* Validate pointer elements without converting a typed array handle. */
#define EAPtrTest(ptr) ((void)sizeof(char[ \
	__builtin_classify_type(**(ptr)) == 5 ? 1 : -1]))
#else
static INLINEDBG void EAPtrTest(void const * const * const * ptr) {}
#endif


void eaCreateWithCapacityDbg(mEArrayHandle* handle, int capacity, const char *file, int line);
void eaSetCapacityDbg(mEArrayHandle* handle, int capacity, const char *file, int line);            // set the current capacity to size, may reduce size
void eaSetSizeDbg(mEArrayHandle* handle, int size, const char *file, int line);                    // grows or shrinks to i, adds NULL entries if required
int eaPushDbg(mEArrayHandle* handle, void* structptr, const char *file, int line);                // add to the end of the list, returns the index it was added at (the new size)
int eaPushUniqueDbg(mEArrayHandle* handle, void* structptr, const char *file, int line);        // add to the end of the list if not already in the list, returns the index it was added at (the new size)
// Append src pointer values; return their starting index in handle.
int eaPushArrayDbg(mEArrayHandle *handle, cccEArrayHandle *src,
	const char *file, int line);
void eaInsertDbg(mEArrayHandle* handle, void* structptr, int i, const char *file, int line);    // insert before i'th position, will not insert on error (i == -1, etc.)
bool eaSetDbg(mEArrayHandle* handle, void* structptr, int i);                                    // set i'th element (zero-based)
void eaSetForcedDbg(mEArrayHandle* handle, void* structptr, int i, const char *file, int line);    // set i'th element (zero-based), increase capacity/size if necessary
void* eaGetDbg(ccmEArrayHandle* handle, int i);                                                    // get i'th element (zero-based), NULL on error
const void* eaGetConstDbg(cccEArrayHandle* handle, int i);                                        // get i'th const element (zero-based), NULL on error
void* eaLastDbg(ccmEArrayHandle* handle);                                                        // get the size-1'th element, NULL on error
const void* eaLastConstDbg(cccEArrayHandle* handle);                                            // get the size-1'th const element, NULL on error
void eaCopyDbg(mEArrayHandle* dest, cccEArrayHandle* src, const char *file, int line);            // create dest if needed, copy pointers
void eaCopyExDbg(cccEArrayHandle* src, mEArrayHandle* dst, int size, EArrayItemConstructor constructor, const char *file, int line); // calls constructor or malloc for each element
void eaCompressDbg(mEArrayHandle *dst, cccEArrayHandle *src, CustomMemoryAllocator memAllocator, void *customData, const char *file, int line);
void eaDestroyDbg(mEArrayHandle* handle);                                                        // free list
void eaDestroyExDbg(mEArrayHandle *handle,EArrayItemDestructor destructor);                        // calls destroy contents, then destroy
int eaFindDbg(cccEArrayHandle* handle, const void* structptr);                                    // find the first element that matches structptr, returns -1 on error
int eaFindAndRemoveDbg(cEArrayHandle* handle, const void* structptr);                            // finds element, deletes it, returns same value as eaFind
int eaFindAndRemoveFastDbg(cEArrayHandle* handle, const void* structptr);                        // finds element, deletes it, returns same value as eaFind DOES NOT KEEP ORDER
void* eaRemoveDbg(mEArrayHandle* handle, int i);                                                // remove the i'th element, NULL on error
const void* eaRemoveConstDbg(cEArrayHandle* handle, int i);                                        // remove the i'th const element, NULL on error
void* eaRemoveFastDbg(mEArrayHandle* handle, int i);                                            // remove the i'th element, and move the last element into this place DOES NOT KEEP ORDER
const void* eaRemoveFastConstDbg(cEArrayHandle* handle, int i);                                    // remove the i'th element, and move the last element into this place DOES NOT KEEP ORDER
void* eaPopDbg(mEArrayHandle* handle);                                                            // remove the last item from the list
const void* eaPopConstDbg(cEArrayHandle* handle);                                                // remove the last const item from the list
void eaClearDbg(mEArrayHandle* handle);                                                            // sets all elements to NULL
void eaClearExDbg(mEArrayHandle* handle, EArrayItemDestructor destructor);                        // calls destructor or free on each element in the array


typedef struct EArray32
{
    int count;
    int size;
    U32 flags;
    U32 values[1];
} EArray32;
#define EARRAY32_HEADER_SIZE        (offsetof(EArray32,values))
#define EARRAY32_SMALLOC_SAFE_COUNT    ( MEMALLOC_NOT_SMALLOC < sizeof(EArray32) ? 1 : (MEMALLOC_NOT_SMALLOC-EARRAY32_HEADER_SIZE+sizeof(U32)-1)/sizeof(U32) )

#if EARRAY_CRAZY_DEBUGGING
    EArray32* EArray32FromHandle(const void *handle);
#else
#    define EArray32FromHandle(handle) ((EArray32*)(((char*)handle) - EARRAY32_HEADER_SIZE))
#endif
#define HandleFromEArray32(array) ((U32*)(((char*)array) + EARRAY32_HEADER_SIZE))
#if defined(__GNUC__) && !defined(__cplusplus)
/* Check signed/unsigned 32-bit storage without evaluating the handle. */
#define EA32PtrTest(ptr) ((void)sizeof(char[ \
	(__builtin_types_compatible_p(__typeof__(**(ptr)), int) || \
	__builtin_types_compatible_p(__typeof__(**(ptr)), U32)) ? 1 : -1]))
#else
static INLINEDBG void EA32PtrTest(U32 const * const * ptr) {}
#endif

void    ea32CreateWithCapacityDbg(mEArray32Handle* handle, int capacity, const char *file, int line);
void    ea32SetCapacityDbg(mEArray32Handle* handle, int capacity, const char *file, int line);    // set the current capacity to size, may reduce size
void    ea32SetSizeDbg(mEArray32Handle* handle, int size, const char *file, int line);        // grows or shrinks to i, adds NULL entries if required
int        ea32PushDbg(mEArray32Handle* handle, U32 value, const char *file, int line);        // add to the end of the list, returns the index it was added at (the new size)
int        ea32PushUniqueDbg(mEArray32Handle* handle, U32 value, const char *file, int line);    // add to the end of the list if not already in the list, returns the index it was added at (the new size)
int        ea32PushArrayDbg(mEArray32Handle* handle, mEArray32Handle* src, const char *file, int line); // add an earray to the end of the list, returns the index it was added at
void    ea32InsertDbg(mEArray32Handle* handle, U32 value, int i, const char *file, int line); // insert before i'th position, will not insert on error (i == -1, etc.)
void    ea32CopyDbg(mEArray32Handle* dest, mEArray32Handle* src, const char *file, int line);// create dest if needed, copy pointers
void    ea32AppendDbg(mEArray32Handle* handle, U32 *values, int count, const char *file, int line);    // grow if needed
void    ea32CompressDbg(mEArray32Handle *dst, mEArray32Handle *src, CustomMemoryAllocator memAllocator, void *customData, const char *file, int line);

typedef int* eaiHandle;

#if defined(__GNUC__) && !defined(__cplusplus)
/* Both integer spellings represent the same 32-bit array storage. */
#define EAIntPtrTest(ptr) ((void)sizeof(char[ \
	(__builtin_types_compatible_p(__typeof__(**(ptr)), int) || \
	__builtin_types_compatible_p(__typeof__(**(ptr)), U32)) ? 1 : -1]))
#else
static INLINEDBG void EAIntPtrTest(int const * const * ptr) {}
#endif

#define eaiCreateDbg(handle, file, line) \
	(EAIntPtrTest(handle), ea32CreateWithCapacityDbg(mEA32Cast(handle), \
	1, file, line))
#define eaiSetCapacityDbg(handle, capacity, file, line) \
	(EAIntPtrTest(handle), ea32SetCapacityDbg(mEA32Cast(handle), \
	capacity, file, line))
#define eaiInsertDbg(handle, value, i, file, line) \
	(EAIntPtrTest(handle), ea32InsertDbg(mEA32Cast(handle), value, i, \
	file, line))
#define eaiCopyDbg(dest, src, file, line) \
	(EAIntPtrTest(dest), EAIntPtrTest(src), \
	ea32CopyDbg(mEA32Cast(dest), mEA32Cast(src), file, line))
#define eaiAppendDbg(handle, values, count, file, line) \
	(EAIntPtrTest(handle), ea32AppendDbg(mEA32Cast(handle), \
	(U32*)(values), count, file, line))

int eaiSortedPushDbg(meaiHandle *array, int elem, const char *file, int line);
int eaiSortedPushUniqueDbg(meaiHandle *array, int elem, const char *file, int line);
int eaiSortedPushAssertUniqueDbg(meaiHandle *array, int elem, const char *file, int line);
void eaiSortedIntersectionDbg(meaiHandle *arrayDst, meaiHandle *array1, meaiHandle *array2, const char *file, int line);
void eaiSortedDifferenceDbg(meaiHandle *arrayDst, meaiHandle *array1, meaiHandle *array2, const char *file, int line);
void eaiSortedUnionDbg(meaiHandle *arrayDst, meaiHandle *array1, meaiHandle *array2, const char *file, int line);

typedef F32* meafHandle;

#if defined(__GNUC__) && !defined(__cplusplus)
/* Require float storage without evaluating the handle. */
#define EAF32PtrTest(ptr) ((void)sizeof(char[ \
	__builtin_types_compatible_p(__typeof__(**(ptr)), F32) ? 1 : -1]))
#else
static INLINEDBG void EAF32PtrTest(F32 const * const * ptr) {}
#endif

static INLINEDBG int eafPush(meafHandle *handle, F32 val)
{
    int res = ea32Push(mEA32Cast(handle), 0);
    (*handle)[res] = val;
    return res;
}

static INLINEDBG F32 eafPop(meafHandle *handle)
{
    EArray32* pArray = EArray32FromHandle(*(handle));
    if(!(*handle) || !pArray->count)
        return 0.f;
    pArray->count--;
    return (*handle)[pArray->count];
}

static INLINEDBG void eafInsert(meafHandle *handle, F32 val, int idx )
{
    ea32Insert(mEA32Cast(handle), (U32)0, idx);
    (*handle)[idx] = val;
}


#ifndef EARRAY_IMPL
/* Typed handles use the same pointer/32-bit storage as the implementation.
* Keep the owning functions available for function pointers and raw callers.
*/
#define eaCapacity(handle) (eaCapacity)(cccEACast(handle))
#define eaMemUsage(handle) (eaMemUsage)(cccEACast(handle))
#define ea32MemUsage(handle) (ea32MemUsage)(mEA32Cast(handle))
#define ea32Destroy(handle) (ea32Destroy)(mEA32Cast(handle))
#define eaSwap(handle, i, j) (eaSwap)(cEAHandleCast(handle), i, j)
#define eaMove(handle, dest, src) (eaMove)(cEAHandleCast(handle), dest, src)
#define eaReverse(handle) (eaReverse)(cEAHandleCast(handle))
#define StringArrayFind(array, elem) \
	(StringArrayFind)((const char *const *)(array), elem)
#endif

C_DECLARATIONS_END

#endif // __EARRAY_H
