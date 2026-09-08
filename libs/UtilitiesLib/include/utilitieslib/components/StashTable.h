#ifndef STASHTABLE_H
#define STASHTABLE_H

#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

//----------------------------------
// A StashTable is just a HashTable
//----------------------------------
typedef struct StashTableImp*            StashTable;
typedef struct StashElementImp*            StashElement;
typedef const struct StashTableImp*        cStashTable;
typedef const struct StashElementImp*    cStashElement;
typedef void (*Destructor)(void* value);
typedef unsigned int (*ExternalHashFunction)(void* userData, const void* key, int hashSeed);
typedef int (*ExternalCompareFunction)(void* userData, const void* key1, const void* key2);

typedef struct
{
    StashTable        pTable;
    U32                uiIndex;
} StashTableIterator;

typedef struct
{
    cStashTable        pTable;
    U32                uiIndex;
} cStashTableIterator;

#define STASH_DEEP_COPY_KEY_NAMES_BIT    (1 << 0) // only makes sense on string keys, int keys are always deep copied
#define STASH_TABLE_READ_ONLY            (1 << 1) // lock the table from writing
#define CASE_SENSITIVE_BIT                (1 << 2) // only makes sense on string keys, case sensitive for hashing
#define STASH_IN_SHARED_HEAP            (1 << 3) // this stash table is alloc'd in the shared heap
#define STASH_WIDE_STRINGS                (1 << 4) // this stash table is alloc'd in the shared heap
typedef enum
{
    StashDefault            =            0, // doesn't do anything, a place holder
    StashDeepCopyKeys        =            STASH_DEEP_COPY_KEY_NAMES_BIT,
    StashCaseSensitive        =            CASE_SENSITIVE_BIT,
    StashSharedHeap            =            STASH_IN_SHARED_HEAP,
    StashWideString            =            STASH_WIDE_STRINGS,
} StashTableMode;

typedef enum 
{
    StashKeyTypeStrings,
    StashKeyTypeInts,
    StashKeyTypeFixedSize,
    StashKeyTypeExternalFunctions,
    StashKeyTypeAddress,
} StashKeyType;

#define stashOptimalSize(count) ((count * 3 + 1) / 2) // overallocate by 50%
#define stashShared(shared_memory) ((shared_memory) ? StashSharedHeap : StashDefault)

// -------------------------
// Table management and info
// -------------------------

StashTable            stashTableCreateEx(U32 uiInitialSize, StashTableMode eMode, StashKeyType eKeyType, U32 uiKeyLength, const char* pcFile, U32 uiLine );
StashTable            stashTableCreateWithStringKeysEx(U32 uInitialSize, StashTableMode eMode, const char* pcFile, U32 uiLine );
StashTable            stashTableCreateIntEx(U32 uiInitialSize, const char* pcFile, U32 uiLine );
StashTable            stashTableCreateExternalFunctionsEx(U32 uiInitialSize, StashTableMode eMode, ExternalHashFunction hashFunc, ExternalCompareFunction compFunc, void *userData, const char* pcFile, U32 uiLine );
StashTable            stashTableCreateAddressEx(U32 uiInitialSize, const char* pcFile, U32 uiLine );
StashTable            stashTableCreateFixedSizeEx(U32 uiInitialSize, U32 sizeInBytes, const char* pcFile, U32 uiLine);

#define stashTableCreate(uiInitialSize, eMode, eKeyType, uiKeyLength) stashTableCreateEx(uiInitialSize, eMode, eKeyType, uiKeyLength, __FILE__, __LINE__ )
#define stashTableCreateWithStringKeys(uiInitialSize, eMode) stashTableCreateWithStringKeysEx(uiInitialSize, eMode, __FILE__, __LINE__ )
#define stashTableCreateInt(uiInitialSize) stashTableCreateIntEx(uiInitialSize, __FILE__, __LINE__ )
#define stashTableCreateExternalFunctions(uiInitialSize, eMode, hashFunc, compFunc, userData) stashTableCreateExternalFunctionsEx(uiInitialSize, eMode, hashFunc, compFunc, userData, __FILE__, __LINE__ )
#define stashTableCreateAddress(uiInitialSize) stashTableCreateAddressEx(uiInitialSize, __FILE__, __LINE__ )
#define stashTableCreateFixedSize(uiInitialSize, sizeInBytes) stashTableCreateFixedSizeEx(uiInitialSize, sizeInBytes, __FILE__, __LINE__ )

void                stashTableDestroy(StashTable pTable);
void                stashTableDestroyConst(cStashTable pTable);
void                stashTableDestroyEx(StashTable pTable, Destructor keyDstr, Destructor valDstr );
void                stashTableMerge(StashTable pDestinationTable, cStashTable pSourceTable, bool dataNonPointers);
void                stashTableMergeConst(cStashTable pDestinationTable, cStashTable pSourceTable, bool dataNonPointers);
StashTable            stashTableItersectEx(cStashTable tableOne, cStashTable tableTwo, const char* pcFile, U32 uiLine);
#define                stashTableIntersect(tableOne, tableTwo) stashTableItersectEx(tableOne, tableTwo, __FILE__, __LINE__)
void                stashTableClear(StashTable pTable);
void                stashTableClearConst(cStashTable pTable);
void                stashTableClearEx(StashTable pTable, Destructor keyDstr, Destructor valDstr );
void                stashTableSetSize(StashTable pTable, U32 size);
void                stashTableSetSizeConst(cStashTable pTable, U32 size);


void                stashTableLock(StashTable table);
void                stashTableUnlock(StashTable table);
// table info 
U32                    stashGetValidElementCount(cStashTable table);
U32                    stashGetOccupiedSlots(cStashTable table);
U32                    stashGetMaxSize(cStashTable table);
size_t                stashGetMemoryUsage(cStashTable table);
StashTableMode        stashTableGetMode(cStashTable table);

// ------------------------
// Shared memory management
// ------------------------
typedef void* (*CustomMemoryAllocator)(void* data, size_t size);
size_t stashGetCopyTargetAllocSize(cStashTable pTable);
StashTable stashCopyToAllocatedSpace(cStashTable pTable, void* pAllocatedSpace, size_t uiTotalSize );
StashTable stashTableClone(cStashTable pTable, CustomMemoryAllocator memAllocator, void *customData);





// -------------------------
// Element management and info
// -------------------------
// pointer values
void*                stashElementGetPointer(StashElement element);
const void*            stashElementGetPointerConst(cStashElement element);
void**              stashElementGetPointerRef(StashElement element);
void                stashElementSetPointer(StashElement element, void* pValue);
// int values
int                    stashElementGetInt(cStashElement element);
void                stashElementSetInt(StashElement element, int iValue);
// keys
const char*            stashElementGetStringKey(cStashElement element);
const wchar_t*        stashElementGetWideStringKey(cStashElement element);
int                    stashElementGetIntKey(cStashElement element);
U32                    stashElementGetU32Key(cStashElement element);
const void*            stashElementGetKey(cStashElement element);


// Iterators
void                stashGetIterator(StashTable pTable, StashTableIterator* pIter);
void                stashGetIteratorConst(cStashTable pTable, cStashTableIterator* pIter);
bool                stashGetNextElement(StashTableIterator* pIter, StashElement* ppElem);
bool                stashGetNextElementConst(cStashTableIterator* pIter, cStashElement* ppElem);
typedef int            (*StashElementProcessor)(StashElement element);
typedef int            (*cStashElementProcessor)(cStashElement element);
typedef int            (*StashElementProcessorEx)(void* userData, StashElement element);
void                stashForEachElement(StashTable pTable, StashElementProcessor proc);
void                stashForEachElementConst(cStashTable pTable, cStashElementProcessor proc);
void                stashForEachElementEx(StashTable pTable, StashElementProcessorEx proc, void* userdata);



// -----------
// Pointer Keys
// -----------
bool                stashFindElement(StashTable table, const void* pKey, StashElement* pElement);
bool                stashFindElementConst(cStashTable table, const void* pKey, cStashElement* pElement);
bool                stashFindKeyByIndex(cStashTable table, U32 uiIndex, const void** pKey);
bool                stashFindIndexByKey(cStashTable table, const void* pKey, U32* piIndex);
bool                stashGetKey(cStashTable table, const void* pKeyIn, const void** pKeyOut);
// pointer values
bool                stashAddPointer(StashTable table, const void* pKey, void* pValue, bool bOverwriteIfFound);
bool                stashAddPointerConst(cStashTable table, const void* pKey, const void* pValue, bool bOverwriteIfFound);
bool                stashAddPointerAndGetElement(StashTable table, const void* pKey, void* pValue, bool bOverwriteIfFound, StashElement* pElement);
bool                stashAddPointerAndGetElementConst(StashTable table, const void* pKey, const void* pValue, bool bOverwriteIfFound, cStashElement* pElement);
bool                stashRemovePointer(StashTable table, const void* pKey, void** ppValue);
bool                stashFindPointer(StashTable table, const void* pKey, void** ppValue);
bool                stashFindPointerConst(cStashTable table, const void* pKey, const void** ppValue);
void*                stashFindPointerReturnPointer(StashTable table, const void* pKey); // please don't use, for backwards-compatibility only    
const void*            stashFindPointerReturnPointerConst(cStashTable table, const void* pKey);
// int values
bool                stashAddInt(StashTable table, const void* pKey, int iValue, bool bOverwriteIfFound);
bool                stashAddIntConst(cStashTable table, const void* pKey, int iValue, bool bOverwriteIfFound);
bool                stashAddIntAndGetElement(StashTable table, const void* pKey, int iValue, bool bOverwriteIfFound, StashElement* pElement);
bool                stashAddIntAndGetElementConst(cStashTable table, const void* pKey, int iValue, bool bOverwriteIfFound, cStashElement* pElement);
bool                stashRemoveInt(StashTable table, const void* pKey, int* piResult);
bool                stashFindInt(cStashTable table, const void* pKey, int* piResult);

// -----------
// Int Keys
// -----------
bool                stashIntFindElement(StashTable table, int iKey, StashElement* pElement);
bool                stashIntFindKeyByIndex(cStashTable table, U32 uiIndex, int* piKeyOut);
bool                stashIntFindIndexByKey(cStashTable table, int iKey, U32* piIndex);
bool                stashIntGetKey(cStashTable table, int iKeyIn, int* piKeyOut);
// pointer values
bool                stashIntAddPointer(StashTable table, int iKey, void* pValue, bool bOverwriteIfFound);
bool                stashIntAddPointerConst(StashTable table, int iKey, const void* pValue, bool bOverwriteIfFound);
bool                stashIntAddPointerAndGetElement(StashTable table, int iKey, void* pValue, bool bOverwriteIfFound, StashElement* pElement);
bool                stashIntRemovePointer(StashTable table, int iKey, void** ppValue);
bool                stashIntFindPointer(cStashTable table, int iKey, void** ppValue);
bool                stashIntFindPointerConst(cStashTable table, int iKey, const void** ppValue);
// int values
bool                stashIntAddInt(StashTable table, int iKey, int iValue, bool bOverwriteIfFound);
bool                stashIntAddIntConst(cStashTable table, int iKey, int iValue, bool bOverwriteIfFound);
bool                stashIntAddIntAndGetElement(StashTable table, int iKey, int iValue, bool bOverwriteIfFound, StashElement* pElement);
bool                stashIntRemoveInt(StashTable table, int iKey, int* piResult);
bool                stashIntFindInt(cStashTable table, int iKey, int* piResult);
bool                stashIntFindIntConst(cStashTable table, int iKey, const int* piResult);

// -----------
// Address Keys - for pointer keys that aren't dereferenced
// -----------
bool                stashAddressFindElement(StashTable table, const void* pKey, StashElement* pElement);
bool                stashAddressElementConst(cStashTable table, const void* pKey, const StashElement* pElement);
bool                stashAddressFindKeyByIndex(cStashTable table, U32 uiIndex, void** ppKeyOut);
bool                stashAddressFindIndexByKey(cStashTable table, const void* pKey, U32* piIndex);
bool                stashAddressGetKey(cStashTable table, const void* pKeyIn, void** ppKeyOut);
// pointer values
bool                stashAddressAddPointer(StashTable table, const void* pKey, void* pValue, bool bOverwriteIfFound);
bool                stashAddressAddPointerAndGetElement(StashTable table, const void* pKey, void* pValue, bool bOverwriteIfFound, StashElement* pElement);
bool                stashAddressRemovePointer(StashTable table, const void* pKey, void** ppValue);
bool                stashAddressFindPointer(cStashTable table, const void* pKey, void** ppValue);
bool                stashAddressFindPointerConst(cStashTable table, const void* pKey, const void** ppValue);
// int values
bool                stashAddressAddInt(StashTable table, const void* pKey, int iValue, bool bOverwriteIfFound);
bool                stashAddressAddIntAndGetElement(StashTable table, const void* pKey, int iValue, bool bOverwriteIfFound, StashElement* pElement);
bool                stashAddressAddIntAndGetElementConst(cStashTable table, const void* pKey, int iValue, bool bOverwriteIfFound, cStashElement* pElement);
bool                stashAddressRemoveInt(StashTable table, const void* pKey, int* piResult);
bool                stashAddressFindInt(cStashTable table, const void* pKey, int* piResult);
bool                stashAddressFindIntConst(cStashTable table, const void* pKey, const int* piResult);

// ----------
// Tracking
// ----------
void                printStashTableMemDump();
int                    stashStorageIsNotNull(cStashTable table);


void stashTableMultiLevelAdd( StashTable table, int depth, ... );


#ifndef STASH_TABLE_IMPL
/* Pointer outputs may be addresses of typed object pointers, or NULL.
* The caller must request the type stored in the table. These adapters retain
* the raw functions and their success/miss behavior for untyped callers.
*/
#define stashFindPointer(table, key, out) \
	(stashFindPointer)(table, key, (void**)(out))
#define stashRemovePointer(table, key, out) \
	(stashRemovePointer)(table, key, (void**)(out))
#define stashIntFindPointer(table, key, out) \
	(stashIntFindPointer)(table, key, (void**)(out))
#define stashIntRemovePointer(table, key, out) \
	(stashIntRemovePointer)(table, key, (void**)(out))
#define stashAddressFindPointer(table, key, out) \
	(stashAddressFindPointer)(table, key, (void**)(out))
#define stashAddressRemovePointer(table, key, out) \
	(stashAddressRemovePointer)(table, key, (void**)(out))
#define stashFindPointerConst(table, key, out) \
	(stashFindPointerConst)(table, key, (const void**)(out))
#define stashAddressFindPointerConst(table, key, out) \
	(stashAddressFindPointerConst)(table, key, (const void**)(out))
#define stashGetKey(table, key, out) \
	(stashGetKey)(table, key, (const void**)(out))
#define stashFindKeyByIndex(table, key, out) \
	(stashFindKeyByIndex)(table, key, (const void**)(out))
#endif

C_DECLARATIONS_END

#endif
