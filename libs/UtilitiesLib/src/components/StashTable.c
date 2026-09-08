
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/StringTable.h"
#include "utilitieslib/components/HashFunctions.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/components/SharedHeap.h"
#include "utilitieslib/components/SharedMemory.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/memcheck.h"

//#define STASH_TABLE_TRACK
#pragma warning(push)

#if _MSC_VER < 1400
    #pragma warning(disable:4022) // related
#else
    #pragma warning(disable:4133)
#endif

    #pragma warning(disable:4028) // parameter differs from declaration
#pragma warning(disable:4024) // different types for formal and actual parameter
#pragma warning(disable:4047) // return type differs in lvl of indirection warning
#pragma warning(disable:4002) // too manay macro parameters warning

#define STASH_HASH_MASK(n) (n-1)
#define STASH_TABLE_EMPTY_SLOT_KEY 0

#ifdef  _WIN64
#define STASH_TABLE_DELETED_SLOT_VALUE (uintptr_t)0xfffffffffffffffeULL
#else
#define STASH_TABLE_DELETED_SLOT_VALUE (_W64 unsigned int)0xfffffffeU    // _W64 = we promise to expand to 64 bits if _WIN64 is defined
#endif

#define STASH_TABLE_QUADRATIC_PROBE_COEFFICIENT 1

// must be the size of a pointer
typedef union _StashKey
{
    //int iKey;
    //const char* pcKey;
    //const wchar_t* pwcKey;
    //const void* pKey;
    //void* pNonConstKey;
    //U32 uiKey;
    void* pKey;
} StashKey;

// must be the size of a pointer
typedef union _StashValue
{
    //int iValue;
    void* pValue;
    //U32 uiValue;
} StashValue;


typedef struct StashElementImp
{
    StashKey key;
    StashValue value;
} StashElementImp;


typedef struct StashTableImp
{
    U32                            uiSize;                    // Current number of elements, including deleted elements
    U32                            uiValidValues;            // number of valid elements (non-deleted)
    U32                            uiMaxSize;                // Current element storage array size
    StashElementImp*            pStorage;                // Actual element storage
    StringTable                    pStringTable;            // string storage space when deep copy is requested.

    U32                            uiFixedKeyLength;        // If we are not strings, we have a fixed keylength specified here (4 for ints)

    StashKeyType                eKeyType;

    ExternalHashFunction        hashFunc;                // Only used if keytype is StashKeyTypeExternalFunctions
    ExternalCompareFunction        compFunc;                // Only used if keytype is StashKeyTypeExternalFunctions
    void*                        userData;                // Only used if keytype is StashKeyTypeExternalFunctions

    // Bit flags
    U32                            bDeepCopyKeys        : 1; // only makes sense for string keys, copy the keys into the string table above
    U32                            bWideString                : 1; // only makes sense for string keys, use wide strings
    U32                            bCaseSensitive            : 1; // is sensitive to case differences, slight performance increase, but be careful
    U32                            bInSharedHeap            : 1; // means this hash table resides in shared heap
    U32                            bReadOnly                : 1; // do not allow writing or access to nonconst accessors
    U32                            bCantResize                : 1; // if set, resize will assert
    U32                            bCantDestroy            : 1; // if set, destroy will assert
#ifdef STASH_TABLE_TRACK
    char cName[128];
    struct StashTableImp* pNext;
#endif

} StashTableImp;

#ifdef STASH_TABLE_TRACK
StashTableImp* pStashTableList;
#endif

// -------------------------
// Static utility functions
// -------------------------
static void stashSetMode(StashTableImp* pTable, StashTableMode eMode)
{
    assert(!pTable->bReadOnly);

    pTable->bWideString = !!(eMode & STASH_WIDE_STRINGS); // note, this is used later in the function so don't move it
    pTable->bCaseSensitive = !!(eMode & CASE_SENSITIVE_BIT);
    pTable->bInSharedHeap = !!(eMode & STASH_IN_SHARED_HEAP);
    
    
    if(eMode & STASH_DEEP_COPY_KEY_NAMES_BIT)
    {
        pTable->bDeepCopyKeys = 1;
        // Init the string table
        pTable->pStringTable = createStringTable();
#ifdef STASH_TABLE_TRACK
        initStringTableEx(pTable->pStringTable, 4096, pTable->cName, 0);
#else
        initStringTable(pTable->pStringTable, 4096);
#endif
        strTableSetMode(pTable->pStringTable, Indexable);
        if (pTable->bWideString)
            strTableSetMode(pTable->pStringTable, WideString);
    }
    
}
static U32 stashMaskTableIndex( const StashTableImp* pTable, U32 uiValueToMask )
{
    return uiValueToMask & STASH_HASH_MASK(pTable->uiMaxSize);
}

static bool slotEmpty(const StashElementImp* pElem)
{
    return (pElem->key.pKey == STASH_TABLE_EMPTY_SLOT_KEY);
}

static bool slotDeleted(const StashElementImp* pElem)
{
    return (pElem->value.pValue == STASH_TABLE_DELETED_SLOT_VALUE);
}
static bool usesStringKeys(const StashTableImp* pTable)
{
    return (pTable->eKeyType == StashKeyTypeStrings);
}
static U32 getKeyLength(const StashTableImp* pTable, const void* pKey)
{
    devassert(pTable);
    if (!pTable)
        return 0;
    if (usesStringKeys(pTable))
    {
        assert(pKey);
        if ( pTable->bWideString )
            return ( (U32)wcslen(pKey) << 1 );
        else
            return (U32)strlen(pKey);
    }
    return pTable->uiFixedKeyLength;
}

static void setExternalFunctions(StashTableImp* pTable, ExternalHashFunction hashFunc, ExternalCompareFunction compFunc, void *userData)
{
    pTable->hashFunc = hashFunc;
    pTable->compFunc = compFunc;
    pTable->userData = userData;
}

#ifdef STASH_TABLE_TRACK
static void startTrackingStashTable(StashTableImp* pNewTable, const char* pcFile, U32 uiLine)
{
    StashTableImp* pList = pStashTableList;
    sprintf(pNewTable->cName, "HT: %s - %d", pcFile, uiLine);
    if ( pStashTableList )
    {
        while ( pList->pNext )
            pList = pList->pNext;

        pList->pNext = pNewTable;
    }
    else
    {
        pStashTableList = pNewTable;
    }
}

static void stopTrackingStashTable(StashTableImp* pTable)
{
    // remove from list
    StashTableImp* pList = pStashTableList;
    // we don't track (or destroy, for that matter) shared stash tables
    if ( pTable->bInSharedHeap )
        return;
    assert( pList );

    if ( pList == pTable )
    {
        pStashTableList = pTable->pNext;
    }
    else
    {
        StashTableImp* pPrev = pStashTableList;
        pList = pStashTableList->pNext;
        while ( pList != pTable )
        {
            pPrev = pList;
            pList = pList->pNext;
            if ( !pList )
            {
                // we ran out?
                return;
            }
        }
        assert( pList == pTable );
        pPrev->pNext = pList->pNext;
    }
}
#endif

// -------------------------
// Table management and info
// -------------------------
#define STASH_TABLE_MIN_SIZE 8

StashTable stashTableCreateEx(U32 uiInitialSize, StashTableMode eMode, StashKeyType eKeyType, U32 uiKeyLength, const char* pcFile, U32 uiLine )
{
    StashTableImp* pNewTable;
    bool bInSharedHeap = eMode & STASH_IN_SHARED_HEAP;
    
    devassertmsg(!(bInSharedHeap && (eMode & StashDeepCopyKeys)), "shared memory using DeepCopyKeys not allowed");

    if ( bInSharedHeap )
        pNewTable = sharedCalloc(1, sizeof(StashTableImp));
    else
        pNewTable = calloc(1, sizeof(StashTableImp));

    if ( uiInitialSize < STASH_TABLE_MIN_SIZE)
        uiInitialSize = STASH_TABLE_MIN_SIZE;

    // Table maxsize must be a power of 2, so be sure to 
    pNewTable->uiMaxSize = pow2(uiInitialSize);

    // allocate memory for the table
    if (bInSharedHeap)
    {
        pNewTable->pStorage = sharedCalloc(pNewTable->uiMaxSize, sizeof(StashElementImp) );
        assert(pNewTable->pStorage); // this will cause catastrophic failure later on
    }
    else
    {
        pNewTable->pStorage = calloc(pNewTable->uiMaxSize, sizeof(StashElementImp) );
    }

#ifdef STASH_TABLE_TRACK
    if ( !bInSharedHeap && !isSharedMemory(pNewTable) )
    {
        startTrackingStashTable(pNewTable, pcFile, uiLine);
    }
#endif

    // Set flags
    stashSetMode(pNewTable, eMode);

    pNewTable->eKeyType = eKeyType;

    switch (eKeyType)
    {
    case StashKeyTypeStrings:
        pNewTable->uiFixedKeyLength = 0;
        break;
    case StashKeyTypeInts:
        pNewTable->uiFixedKeyLength = sizeof(int);
        assert( uiKeyLength == sizeof(int));// we don't support different sizes if we are using int keys
        break;
    case StashKeyTypeFixedSize:
        pNewTable->uiFixedKeyLength = uiKeyLength;
        break;
    case StashKeyTypeExternalFunctions:
        pNewTable->uiFixedKeyLength = 0;
        break;
    case StashKeyTypeAddress:
        pNewTable->uiFixedKeyLength = sizeof(void*);
        assert(uiKeyLength == sizeof(void*));
        break;
    default:
        assert(0); // need to cover everything
        break;
    }


    return pNewTable;
}

StashTable stashTableCreateWithStringKeysEx(U32 uiInitialSize, StashTableMode eMode, const char* pcFile, U32 uiLine)
{
    return stashTableCreateEx(uiInitialSize, eMode, StashKeyTypeStrings, 0, pcFile, uiLine);
}

// Assumes the int key is sizeof(int), not smaller (which is allowed)
StashTable stashTableCreateIntEx(U32 uiInitialSize, const char* pcFile, U32 uiLine)
{
    return stashTableCreateEx(uiInitialSize, StashDefault, StashKeyTypeInts, sizeof(int), pcFile, uiLine);
}

// Assumes the int key is sizeof(int), not smaller (which is allowed)
StashTable stashTableCreateFixedSizeEx(U32 uiInitialSize, U32 sizeInBytes, const char* pcFile, U32 uiLine)
{
    return stashTableCreateEx(uiInitialSize, StashDefault, StashKeyTypeFixedSize, sizeInBytes, pcFile, uiLine);
}

// Assumes the int key is sizeof(int), not smaller (which is allowed)
StashTable stashTableCreateAddressEx(U32 uiInitialSize, const char* pcFile, U32 uiLine)
{
    return stashTableCreateEx(uiInitialSize, StashDefault, StashKeyTypeAddress, sizeof(void*), pcFile, uiLine);
}

StashTable stashTableCreateExternalFunctionsEx(U32 uiInitialSize, StashTableMode eMode, ExternalHashFunction hashFunc, ExternalCompareFunction compFunc, void *userData, const char* pcFile, U32 uiLine)
{
    StashTable pNewTable = stashTableCreateEx(uiInitialSize, eMode, StashKeyTypeExternalFunctions, 0, pcFile, uiLine);
    assert(hashFunc && compFunc);
    setExternalFunctions((StashTableImp*)pNewTable, hashFunc, compFunc, userData);
    return pNewTable;
}

static void destroyStorageValues(StashTableImp* pTable, Destructor keyDstr, Destructor valDstr)
{
    StashTableIterator iter;
    StashElement pElem;


    assert( keyDstr || valDstr ); // don't call this with no destructor
    assert(!pTable->bReadOnly);
    stashGetIterator(pTable, &iter);
    while (stashGetNextElement(&iter, &pElem))
    {
        void* pValue = pElem->value.pValue;
        void* pKey = pElem->key.pKey;

        if ( keyDstr )
            keyDstr(pKey);

        if ( valDstr )
            valDstr(pValue);
    }
}


void stashTableDestroyEx(StashTableImp* pTable, Destructor keyDstr, Destructor valDstr )
{
    assert( keyDstr || valDstr ); // don't call this with no destructor, just call stashTableDestroy()
    devassert(pTable);
    if(!pTable)
        return;
    destroyStorageValues(pTable, keyDstr, valDstr);

    stashTableDestroy(pTable);
}


void stashTableDestroy(StashTableImp* pTable )
{
    devassert(pTable);
    if (!pTable)
        return;
#ifdef STASH_TABLE_TRACK
    stopTrackingStashTable(pTable);
#endif
    assert(!pTable->bReadOnly);

    assert(!pTable->bInSharedHeap); // can't remove a shared hash table...

    assert(!pTable->bCantDestroy); // can't remove an unremovable table... probably a clone with a special allocator


    // free all stored values if neccessary
    //stashTableClearImp(table, func);

    // free memory being used to hold all key/value pairs
    if ( pTable->bInSharedHeap )
        sharedFree( pTable->pStorage );
    else
        free( pTable->pStorage );

    // Do not destroy the string table if the hash table is only being partially destroyed.
    if( pTable->pStringTable )
        destroyStringTable( pTable->pStringTable );

    if ( pTable->bInSharedHeap )
        sharedFree( pTable );
    else
        free( pTable );
}

void stashTableDestroyConst(cStashTable pTable)
{
    stashTableDestroy(cpp_const_cast(StashTable)(pTable));
}

void stashTableMerge(StashTableImp* pDestinationTable, const StashTableImp* pSourceTable, bool dataNonPointers)
{
    StashElementImp* pElement;
    StashTableIterator iter;
    StashKeyType keyType;
    
    if (!pSourceTable)
        return;

    keyType = pSourceTable->eKeyType;
    assert( keyType == pDestinationTable->eKeyType );

    stashGetIterator(cpp_const_cast(StashTable)(pSourceTable), &iter);
    while (stashGetNextElement(&iter, &(StashElement)pElement))
    {
        bool bNoCollision = false;

        switch (pSourceTable->eKeyType)
        {
        case StashKeyTypeStrings:
        case StashKeyTypeExternalFunctions:
        case StashKeyTypeFixedSize:
            {
                if (dataNonPointers)
                    bNoCollision = stashAddInt(pDestinationTable, pElement->key.pKey, PTR_TO_S32(pElement->value.pValue), false);
                else
                    bNoCollision = stashAddPointer(pDestinationTable, pElement->key.pKey, pElement->value.pValue, false);
                break;
            }
        case StashKeyTypeInts:
            {
                if (dataNonPointers)
                    bNoCollision = stashIntAddInt(pDestinationTable, PTR_TO_S32(pElement->key.pKey), PTR_TO_S32(pElement->value.pValue), false);
                else
                    bNoCollision = stashIntAddPointer(pDestinationTable, PTR_TO_S32(pElement->key.pKey), pElement->value.pValue, false);
                break;
            }
        case StashKeyTypeAddress:
            {
                if (dataNonPointers)
                    bNoCollision = stashAddressAddInt(pDestinationTable, pElement->key.pKey, PTR_TO_S32(pElement->value.pValue), false);
                else
                    bNoCollision = stashAddressAddPointer(pDestinationTable, pElement->key.pKey, pElement->value.pValue, false);
                break;
            }
        default:
            assert(0); // need to cover everything
            break;
        }

        assert( bNoCollision );
    }
}

void stashTableMergeConst(cStashTable pDestinationTable, cStashTable pSourceTable, bool dataNonPointers)
{
    stashTableMerge(cpp_const_cast(StashTable)(pDestinationTable), pSourceTable, dataNonPointers);
}

void stashTableClear(StashTableImp* pTable)
{
    devassert(pTable);
    if (!pTable)
        return;
    assert( !pTable->bReadOnly);
    
    // clear the storage
    memset(pTable->pStorage, 0, pTable->uiMaxSize * sizeof(StashElementImp));
    pTable->uiSize = 0;
    pTable->uiValidValues = 0;

    if ( pTable->pStringTable )
        strTableClear(pTable->pStringTable);
}

void stashTableClearConst(cStashTable pTable)
{
    stashTableClear(cpp_const_cast(StashTable)(pTable));
}

void stashTableClearEx(StashTableImp* pTable, Destructor keyDstr, Destructor valDstr )
{
    devassert(pTable);
    if (!pTable)
        return;
    destroyStorageValues(pTable, keyDstr, valDstr);
    stashTableClear(pTable);
}

void stashTableLock(StashTableImp* pTable)
{
    pTable->bReadOnly = 1;
}

void stashTableUnlock(StashTableImp* pTable)
{
    pTable->bReadOnly = 0;
}

// table info 
U32    stashGetValidElementCount(const StashTableImp* pTable)
{
    return pTable->uiValidValues; // external functions want to know how many valid elements we have
}

U32    stashGetOccupiedSlots(const StashTableImp* pTable)
{
    return pTable->uiSize; // in case someone wants to maintain a certain size of hash table
}

U32    stashGetMaxSize(const StashTableImp* pTable)
{
    return pTable->uiMaxSize;
}

size_t stashGetMemoryUsage(const StashTableImp* pTable)
{
    size_t uiMemUsage = sizeof(StashTableImp) + pTable->uiMaxSize * sizeof(StashElementImp);
    if(pTable->pStringTable)
        uiMemUsage += strTableMemUsage(pTable->pStringTable);

    return uiMemUsage;
}

StashTableMode stashTableGetMode(const StashTableImp* pTable)
{
    StashTableMode eMode = StashDefault;

    if ( pTable->bDeepCopyKeys )
        eMode |= StashDeepCopyKeys;

    if ( pTable->bCaseSensitive )
        eMode |= StashCaseSensitive;

    return eMode;
}



// ------------------------
// Shared memory management
// ------------------------
size_t stashGetCopyTargetAllocSize(const StashTableImp* pTable)
{
    size_t uiMemUsage = sizeof(StashTableImp) + pTable->uiMaxSize * sizeof(StashElementImp);
    if ( pTable->pStringTable )
        uiMemUsage += strTableGetCopyTargetAllocSize(pTable->pStringTable);
    return uiMemUsage;
}

StashTable stashCopyToAllocatedSpace(const StashTableImp* pTable, void* pAllocatedSpace, size_t uiTotalSize )
{
    StashTableImp* pNewTable = memset(pAllocatedSpace, 0, uiTotalSize);
    size_t uiStorageMemUsage = pTable->uiMaxSize * sizeof(StashElementImp);
    size_t uiStringTableMemUsage = (pTable->pStringTable) ? strTableGetCopyTargetAllocSize(pTable->pStringTable) : 0;

    // first, memcpy the table imp
    memcpy(pNewTable, pTable, sizeof(StashTableImp));
#ifdef STASH_TABLE_TRACK
    pNewTable->pNext = NULL;
#endif

    // the actual storage
    pNewTable->pStorage = (StashElementImp*)((char*)pNewTable + sizeof(StashTableImp));
    memcpy(pNewTable->pStorage, pTable->pStorage, uiStorageMemUsage);

    // unlock the new table, if the old was locked
    stashTableUnlock(pNewTable);

    // The string table

    // Now, we need to make sure every hash table deep copy into the string table is adjusted for the new table
    // we can't just use pointer arithmetic, because the copied string table is compact now
    // so we use string table indices
    if ( pTable->pStringTable ) 
    {
        StashTable stStringToIndexMap = strTableCreateStringToIndexMap(pTable->pStringTable);
        StashTableIterator iter;
        StashElement elem;
        int iNumStrings = strTableGetStringCount(pNewTable->pStringTable);

        pNewTable->pStringTable = strTableCopyToAllocatedSpace(pTable->pStringTable, (char*)pNewTable->pStorage + uiStorageMemUsage, uiStringTableMemUsage );

        stashGetIterator(pNewTable, &iter);


        // Go through every new element and lookup the new string pointer for it, using the stashtable we made
        while (stashGetNextElement(&iter, &elem))
        {
            StashElementImp* pNewElement = (StashElementImp*)elem;
            int iStringIndex;
            bool bSuccess = stashFindInt(stStringToIndexMap, pNewElement->key.pKey, &iStringIndex);
            assert( bSuccess && iStringIndex >= 0 && iStringIndex < iNumStrings ); // failed to find the index for this string, somehting is very wrong

            // Set the new element's key to point to the new string table copy of the string
            pNewElement->key.pKey = strTableGetString(pNewTable->pStringTable, iStringIndex);
        }

        // cleanup
        stashTableDestroy(stStringToIndexMap);
    }

    return pNewTable;
}

StashTable stashTableClone(const StashTableImp* pTable, CustomMemoryAllocator memAllocator, void *customData)
{
    StashTableImp* pNewTable;

    if (memAllocator)
    {
        size_t uiCopySize = stashGetCopyTargetAllocSize((cStashTable)pTable);
        pNewTable = memAllocator(customData, uiCopySize);
        pNewTable = (StashTableImp*)stashCopyToAllocatedSpace((cStashTable)pTable, pNewTable, uiCopySize);
        pNewTable->bCantResize = 1;
        pNewTable->bCantDestroy = 1;
#ifdef STASH_TABLE_TRACK
        if ( !pNewTable->bInSharedHeap && !isSharedMemory(pNewTable) )
        {
            startTrackingStashTable(pNewTable, pTable->cName, 0);
        }
#endif
    }
    else
    {
        // standard table creation
        pNewTable = (StashTableImp*)stashTableCreate( stashGetValidElementCount((cStashTable)pTable), stashTableGetMode((cStashTable)pTable), pTable->eKeyType, pTable->uiFixedKeyLength );
        if ( pTable->eKeyType == StashKeyTypeExternalFunctions )
        {
            setExternalFunctions(pNewTable, pTable->hashFunc, pTable->compFunc, pTable->userData);
        }

        // now use merge to add all elements from old table to new one
        stashTableMerge(pNewTable, (cStashTable)pTable, false); // TODO we do not know what type of data it is currently
    }

    return pNewTable;
}

StashTable stashTableItersectEx(const StashTableImp *tableOne, const StashTableImp *tableTwo, const char* pcFile, U32 uiLine)
{
    cStashTableIterator iter;
    cStashElement elem;
    StashTable pNewTable;
    int max_count = MIN(stashGetValidElementCount((cStashTable)tableOne), stashGetValidElementCount((cStashTable)tableTwo));

    pNewTable = stashTableCreateEx(max_count, stashTableGetMode((cStashTable)tableOne), tableOne->eKeyType, tableOne->uiFixedKeyLength, pcFile, uiLine );
    if (tableOne->eKeyType == StashKeyTypeExternalFunctions)
        setExternalFunctions(pNewTable, tableOne->hashFunc, tableOne->compFunc, tableOne->userData);

    assert(stashTableGetMode((cStashTable)tableOne) == stashTableGetMode((cStashTable)tableTwo));
    assert(tableOne->eKeyType == tableTwo->eKeyType);

    stashGetIteratorConst(tableOne, &iter);
    while (stashGetNextElementConst(&iter, &elem))
    {
        switch (tableOne->eKeyType)
        {
        case StashKeyTypeStrings:
        case StashKeyTypeExternalFunctions:
        case StashKeyTypeFixedSize:
            {
                if (stashFindPointerConst(tableTwo, elem->key.pKey, NULL))
                    stashAddPointer(pNewTable, elem->key.pKey, elem->value.pValue, false);
                break;
            }
        case StashKeyTypeInts:
            {
                if (stashIntFindPointer(tableTwo, PTR_TO_S32(elem->key.pKey), NULL))
                    stashIntAddPointer(pNewTable, PTR_TO_S32(elem->key.pKey), elem->value.pValue, false);
                break;
            }
        case StashKeyTypeAddress:
            {
                if (stashAddressFindPointer(tableTwo, elem->key.pKey, NULL))
                    stashAddressAddPointer(pNewTable, elem->key.pKey, elem->value.pValue, false);
                break;
            }
        default:
            assert(0); // need to cover everything
            break;
        }
    }

    if (stashGetValidElementCount((cStashTable)pNewTable) > 0)
        return pNewTable;

    stashTableDestroy(pNewTable);
    return NULL;
}




// ---------------------------
// Element management and info
// ---------------------------
// pointer values
void* stashElementGetPointer(StashElementImp* pElement)
{
    return pElement->value.pValue;
}

const void* stashElementGetPointerConst(const StashElementImp* pElement)
{
    return pElement->value.pValue;
}

void** stashElementGetPointerRef(StashElementImp* pElement)
{
    return &pElement->value.pValue;
}

void stashElementSetPointer(StashElementImp* pElement, void* pValue)
{
    pElement->value.pValue = pValue;
}

// int values
int    stashElementGetInt(const StashElementImp* pElement)
{
    return PTR_TO_S32(pElement->value.pValue);
}

void stashElementSetInt(StashElementImp* pElement, int iValue)
{
    pElement->value.pValue = S32_TO_PTR(iValue);
}


// keys
const char* stashElementGetStringKey(const StashElementImp* element)
{
    return (const char*)element->key.pKey;
}

const wchar_t* stashElementGetWideStringKey(const StashElementImp* element)
{
    return (const wchar_t*)element->key.pKey;
}

int    stashElementGetIntKey(const StashElementImp* element)
{
    return PTR_TO_S32(element->key.pKey);
}

U32    stashElementGetU32Key(const StashElementImp* element)
{
    return PTR_TO_U32(element->key.pKey);
}

const void* stashElementGetKey(const StashElementImp* element)
{
    return element->key.pKey;
}

// Iterators
void stashGetIterator(StashTableImp* pTable, StashTableIterator* pIter)
{
    if (pTable)
        assert(!pTable->bReadOnly);
    pIter->pTable = pTable;
    pIter->uiIndex = 0;
}

void stashGetIteratorConst(cStashTable pTable, cStashTableIterator* pIter)
{
    stashGetIterator(cpp_const_cast(StashTable)(pTable), pIter);
}

bool stashGetNextElement(StashTableIterator* pIter, StashElementImp** ppElem)
{
    U32 uiCurrentIndex;
    StashTableImp* pTable = pIter->pTable;
    StashElementImp* pElement;

    devassert(pTable);
    if (!pTable)
        return false;

    assert(!pTable->bReadOnly);

    // Look through the table starting at the index specified by the iterator.
    for(uiCurrentIndex = pIter->uiIndex; uiCurrentIndex < pTable->uiMaxSize; ++uiCurrentIndex)
    {
        pElement = &pTable->pStorage[uiCurrentIndex];

        // If we have found an non-empty, non-deleted element...
        if (!slotEmpty(pElement) && !slotDeleted(pElement))
        {
            pIter->uiIndex = uiCurrentIndex + 1;
            if (ppElem)
                *ppElem = pElement;
            return true;
        }
    }
    return false;
}

bool stashGetNextElementConst(cStashTableIterator* pIter, cStashElement* ppElem)
{
    return stashGetNextElement(pIter, cpp_const_cast(StashElement*)(ppElem));
}

void stashForEachElement(StashTableImp* pTable, StashElementProcessor proc)
{
    StashTableIterator iter;
    StashElement elem;

    if(!pTable)
        return;

    stashGetIterator(pTable, &iter);

    while (stashGetNextElement(&iter, &elem))
    {
        if ( !proc(elem) )
            return;
    }
}

void stashForEachElementConst(cStashTable pTable, cStashElementProcessor proc)
{
    stashForEachElement(cpp_const_cast(StashTable)(pTable), proc);
}

void stashForEachElementEx(StashTableImp* pTable, StashElementProcessorEx proc, void* userdata)
{
    StashTableIterator iter;
    StashElement elem;

    stashGetIterator(pTable, &iter);

    while (stashGetNextElement(&iter, &elem))
    {
        if ( !proc(userdata, elem) )
            return;
    }
}

// ----------------------------------
// Internal hash processing functions
// ----------------------------------
// This is the real magic behind the hash table, here we translate a key into a hash table index, 
// and deal with hash table index collisions (in this case through quadratic probing)
// 
// returns true if it was already in the table, false otherwise
// will always find a value
static bool stashFindIndexByKeyInternal(const StashTableImp* pTable, StashKey key, U32 uiKeyLengthInBytes, U32* puiIndex, int* piFirstDeletedIndex )
{
    U32 uiNumProbes = 1; // the first probe
    U32 uiHashValue = 0;
    U32 uiStorageIndex;
    if ( piFirstDeletedIndex )
        *piFirstDeletedIndex = -1; // no deleted found yet

    // don't try to find an empty key
    if ( key.pKey == STASH_TABLE_EMPTY_SLOT_KEY ) // sorry, can't find an empty key
        return false;

    switch (pTable->eKeyType)
    {
    case StashKeyTypeStrings:
        if ( pTable->bCaseSensitive)
               uiHashValue = burtlehash3(key.pKey, uiKeyLengthInBytes, DEFAULT_HASH_SEED);
        else // this version of the hash function sets the lowercase bit, so it does not distinguish between the two possibilities
            uiHashValue = burtlehash3StashDefault(key.pKey, uiKeyLengthInBytes, DEFAULT_HASH_SEED);
        break;
    case StashKeyTypeInts:
        {
            U32 i = PTR_TO_U32(key.pKey);
            uiHashValue = burtlehash2(&i, 1, DEFAULT_HASH_SEED);
        }
        break;
    case StashKeyTypeFixedSize:
        uiHashValue = burtlehash3(key.pKey, pTable->uiFixedKeyLength, DEFAULT_HASH_SEED);
        break;
    case StashKeyTypeExternalFunctions:
        uiHashValue = pTable->hashFunc(pTable->userData, key.pKey, DEFAULT_HASH_SEED);
        break;
    case StashKeyTypeAddress:
        uiHashValue = burtlehash3((ub1*)&key.pKey, pTable->uiFixedKeyLength, DEFAULT_HASH_SEED);
        break;
    default:
        assert(0); // need to cover everything
        break;
    }

    uiStorageIndex = stashMaskTableIndex(pTable, uiHashValue);

    while ( 1 )
    {
        // Check to see if the new index is the correct index or not
        const StashElementImp* pElement = &pTable->pStorage[uiStorageIndex];


        // Is it an empty slot?
        if (slotEmpty(pElement))
        {
            // We found an empty slot, here we go
            if (puiIndex)
                *puiIndex = uiStorageIndex;

            return false; // empty
        }

        // It's not empty, so is it the right key?
        if ( !slotDeleted(pElement) ) // if we were deleted, we can't do a compare on the keys legally
        {
            bool bMatch = false;
            switch (pTable->eKeyType)
            {
            case StashKeyTypeStrings:
                // It's a string, do a comparison
                if ( pTable->bCaseSensitive )
                {
                    if ( pTable->bWideString )
                    {
                        if ( wcscmp( (wchar_t*)key.pKey, (wchar_t*)pElement->key.pKey ) == 0 )
                            bMatch = true;
                    }
                    else
                    {
                        if ( strcmp( (char*)key.pKey, (char*)pElement->key.pKey ) == 0 )
                            bMatch = true;
                    }
                }
                else
                {
                    if ( pTable->bWideString )
                    {
                        if ( _wcsicmp( (wchar_t*)key.pKey, (wchar_t*)pElement->key.pKey ) == 0 )
                            bMatch = true;
                    }
                    else
                    {
                        if ( stricmp( (char*)key.pKey, (char*)pElement->key.pKey ) == 0 )
                            bMatch = true;
                    }
                }
                break;
            case StashKeyTypeInts:
                // It's an int, so just compare the two
                if ( key.pKey == pElement->key.pKey )
                    bMatch = true;
                break;
            case StashKeyTypeFixedSize:
                if ( memcmp(key.pKey, pElement->key.pKey, pTable->uiFixedKeyLength) == 0)
                    bMatch = true;
                break;
            case StashKeyTypeExternalFunctions:
                if ( pTable->compFunc(pTable->userData, key.pKey, pElement->key.pKey) == 0)
                    bMatch = true;
                break;
            case StashKeyTypeAddress:
                // It's an address, so just compare the two
                if ( key.pKey == pElement->key.pKey )
                    bMatch = true;
                break;
            default:
                assert(0); // need to cover everything
                break;
            }

            if ( bMatch )
            {
                if ( puiIndex )
                    *puiIndex = uiStorageIndex;

                return true; // it's already in there
            }
        }
        else
        {
            // If we haven't yet recorded the first deleted, do so now
            if (piFirstDeletedIndex && *piFirstDeletedIndex == -1)
            {
                *piFirstDeletedIndex = uiStorageIndex;
            }
        }

        // Ok, it's not the right key and it's not empty, either way we need to try the next one
        // Use quadratic probing
        {
            U32 uiProbeOffset = STASH_TABLE_QUADRATIC_PROBE_COEFFICIENT * uiNumProbes * uiNumProbes;
            uiNumProbes++;
            uiStorageIndex = stashMaskTableIndex(pTable, (uiStorageIndex + uiProbeOffset) );

        }

        // Try again, now with a new index
    }
}

static INLINEDBG bool stashFindValueInternal(const StashTableImp* pTable, StashKey key, U32 uiKeyLengthInBytes, StashValue* pValue)
{
    U32 uiStorageIndex;

    if (!stashFindIndexByKeyInternal(pTable, key, uiKeyLengthInBytes, &uiStorageIndex, NULL))
    {
        if ( pValue )
            pValue->pValue = 0;
        return false;
    }

    if (pValue)
        pValue->pValue = pTable->pStorage[uiStorageIndex].value.pValue;
    return true;
}

static bool stashFindElementInternal(const StashTableImp* pTable, StashKey key, U32 uiKeyLengthInBytes, StashElementImp** ppElement)
{
    U32 uiStorageIndex;

    if (!stashFindIndexByKeyInternal(pTable, key, uiKeyLengthInBytes, &uiStorageIndex, NULL))
    {
        if ( ppElement ) *ppElement = NULL;
        return false;
    }

    if ( ppElement ) *ppElement = &pTable->pStorage[uiStorageIndex];

    return true;
}

static bool stashFindElementInternalConst(const StashTableImp* pTable, StashKey key, U32 uiKeyLengthInBytes, const StashElementImp** ppElement)
{
    U32 uiStorageIndex;

    if (!stashFindIndexByKeyInternal(pTable, key, uiKeyLengthInBytes, &uiStorageIndex, NULL))
    {
        if (ppElement)
            *ppElement = NULL;
        return false;
    }

    if (ppElement)
        *ppElement = &pTable->pStorage[uiStorageIndex];

    return true;
}

static bool stashFindKeyByIndexInternal(const StashTableImp* pTable, U32 uiIndex, StashKey* pKey)
{
    if ( uiIndex >= pTable->uiMaxSize || slotEmpty(&pTable->pStorage[uiIndex]) || slotDeleted(&pTable->pStorage[uiIndex]) )
    {
        pKey->pKey = 0;
        return false;
    }

    pKey->pKey = pTable->pStorage[uiIndex].key.pKey;
    return true;
}

static bool stashAddValueInternal(StashTableImp* pTable, StashKey key, U32 uiKeyLengthInBytes, StashValue value, bool bOverwriteIfFound);

static void stashTableResize(StashTableImp* pTable)
{
    U32 uiOldMaxSize = pTable->uiMaxSize;
    U32 uiOldValidValues = pTable->uiValidValues;
    StashElementImp* pOldStorage = pTable->pStorage;
    U32 uiOldIndex;
    bool bWasDeepCopy = pTable->bDeepCopyKeys;

    PERFINFO_AUTO_START("stash table resize", 1);
    assert(!pTable->bCantResize); // can't resize!
    devassertmsg(!pTable->bInSharedHeap, "shared memory stash table is being resized");

    // The new size should be based off of the number of valid values, not the size
    // since the size includes deleted elems which will not find their way into the
    // the new table
    //
    // New maxsize should be the first power of 2 such that validvalues is less than half
    pTable->uiMaxSize = MAX(STASH_TABLE_MIN_SIZE, pow2(uiOldValidValues<<1));

    pTable->uiSize = 0; // this will increas as we add to it
    pTable->uiValidValues = 0;
    pTable->bDeepCopyKeys = 0; // we need to make shallow copies for this stage, since we already made the deep copies


    if ( pTable->bInSharedHeap )
    {
        StashElementImp *pStorage = sharedCalloc(pTable->uiMaxSize, sizeof(StashElementImp));
        assert(pStorage); // this will cause catastrophic failure later on
        pTable->pStorage = pStorage;
    }
    else
    {
        pTable->pStorage = calloc(pTable->uiMaxSize, sizeof(StashElementImp));
    }

    // Go through every old element
    for (uiOldIndex=0; uiOldIndex < uiOldMaxSize; ++uiOldIndex)
    {
        StashElementImp* pOldElement = &pOldStorage[uiOldIndex];
        U32 uiKeyLengthInBytes = pTable->uiFixedKeyLength;
        bool bSuccess;
        if ( slotEmpty(pOldElement) || slotDeleted(pOldElement) )
            continue; // don't add deleted or unused slots

        // put it in the new table, since it must be valid at this point
        if ( usesStringKeys(pTable) )
        {
            if ( pTable->bWideString )
                uiKeyLengthInBytes = ((U32)wcslen((wchar_t*)pOldElement->key.pKey)) << 1;
            else
                uiKeyLengthInBytes = (U32)strlen((char*)pOldElement->key.pKey);
        }
        bSuccess  = stashAddValueInternal(pTable, pOldElement->key, uiKeyLengthInBytes, pOldElement->value, false);
        assertmsgf(bSuccess, "Key: %p", pOldElement->key.pKey); // otherwise, we're trying to add element twice somehow (e.g. outside forces modifying keys)
    }

    assert(
        pTable->uiValidValues == pTable->uiSize 
        && uiOldValidValues == pTable->uiValidValues
        ); // should always be equal after a resize

    pTable->bDeepCopyKeys = bWasDeepCopy; // be sure to restore the deep copy status

    // Free old storage
    if ( pTable->bInSharedHeap)
        sharedFree(pOldStorage);
    else
        free(pOldStorage);

    PERFINFO_AUTO_STOP();
}

void stashTableSetSize(StashTableImp* pTable, U32 size)
{
    U32 uiOldMaxSize = pTable->uiMaxSize;
    U32 uiOldValidValues = pTable->uiValidValues;
    StashElementImp* pOldStorage = pTable->pStorage;
    U32 uiOldIndex;
    bool bWasDeepCopy = pTable->bDeepCopyKeys;

    PERFINFO_AUTO_START("stash table resize", 1);
    assert(!pTable->bCantResize); // can't resize!
    devassertmsg(!pTable->bInSharedHeap, "shared memory stash table is being resized");

    // The new size should be based off of the number of valid values, not the size
    // since the size includes deleted elems which will not find their way into the
    // the new table
    //
    // New maxsize should be the first power of 2 such that validvalues is less than half
    pTable->uiMaxSize = MAX(STASH_TABLE_MIN_SIZE, MAX((U32)pow2(size), uiOldMaxSize));

    pTable->uiSize = 0; // this will increas as we add to it
    pTable->uiValidValues = 0;
    pTable->bDeepCopyKeys = 0; // we need to make shallow copies for this stage, since we already made the deep copies


    if ( pTable->bInSharedHeap )
    {
        StashElementImp *pStorage = sharedCalloc(pTable->uiMaxSize, sizeof(StashElementImp));
        assert(pStorage); // this will cause catastrophic failure later on
        pTable->pStorage = pStorage;
    }
    else
    {
        pTable->pStorage = calloc(pTable->uiMaxSize, sizeof(StashElementImp));
    }

    // Go through every old element
    for (uiOldIndex=0; uiOldIndex < uiOldMaxSize; ++uiOldIndex)
    {
        StashElementImp* pOldElement = &pOldStorage[uiOldIndex];
        U32 uiKeyLengthInBytes = pTable->uiFixedKeyLength;
        bool bSuccess = false;
        if ( slotEmpty(pOldElement) || slotDeleted(pOldElement) )
            continue; // don't add deleted or unused slots

        // put it in the new table, since it must be valid at this point
        if ( usesStringKeys(pTable) )
        {
            if ( pTable->bWideString )
                uiKeyLengthInBytes = ((U32)wcslen((wchar_t*)pOldElement->key.pKey)) << 1;
            else
                uiKeyLengthInBytes = (U32)strlen((char*)pOldElement->key.pKey);
        }
        bSuccess  = stashAddValueInternal(pTable, pOldElement->key, uiKeyLengthInBytes, pOldElement->value, false);
        assert(bSuccess); // otherwise, we're trying to add element twice somehow
    }

    assert(
        pTable->uiValidValues == pTable->uiSize 
        && uiOldValidValues == pTable->uiValidValues
        ); // should always be equal after a resize

    pTable->bDeepCopyKeys = bWasDeepCopy; // be sure to restore the deep copy status

    // Free old storage
    if ( pTable->bInSharedHeap)
        sharedFree(pOldStorage);
    else
        free(pOldStorage);

    PERFINFO_AUTO_STOP();
}

void stashTableSetSizeConst(cStashTable pTable, U32 size)
{
    stashTableSetSize(cpp_const_cast(StashTable)(pTable), size);
}

static bool stashAddValueInternal(StashTableImp* pTable, StashKey key, U32 uiKeyLengthInBytes, StashValue value, bool bOverwriteIfFound)
{
    U32 uiStorageIndex;
    int iFirstDeletedIndex;
    bool bFound;
    StashElementImp* pElement;

    assert( value.pValue != STASH_TABLE_DELETED_SLOT_VALUE ); // sorry, can't add the one value that means it's a deleted slot!
    assert( key.pKey != STASH_TABLE_EMPTY_SLOT_KEY ); // sorry, can't add the one key that means it's an empty slot!

    bFound = stashFindIndexByKeyInternal(pTable, key, uiKeyLengthInBytes, &uiStorageIndex, &iFirstDeletedIndex);
    if ( bFound && !bOverwriteIfFound )
        return false;
    else if ( bFound && pTable->bDeepCopyKeys )
    {
        //Errorf("You can't overwrite in a deep copy stash table, or it will cause a leak in the string table\n");

        // Instead, just log that you'd like to delete this string
        if ( pTable->pStringTable )
        {
            strTableLogRemovalRequest(pTable->pStringTable, (char*)pTable->pStorage[uiStorageIndex].key.pKey );
        }
    }
    // Otherwise, copy it in and return true;

    if (!bFound && iFirstDeletedIndex > -1)
    {
        // we couldn't find the key, but found this nice deleted slot on the way to the first empty slot
        // use this instead, so we don't waste the deleted slot 
        uiStorageIndex = (U32)iFirstDeletedIndex;
    }

    pElement = &pTable->pStorage[uiStorageIndex];

    if ( slotDeleted(pElement) || slotEmpty(pElement) )
    {
        // it was not a valid slot, now it will be
        pTable->uiValidValues++;
    }

    // copy the value
    pElement->value.pValue = value.pValue;

    if ( usesStringKeys(pTable) && pTable->bDeepCopyKeys )
    {
        // deep copy the string
        assert( pTable->pStringTable );
        pElement->key.pKey = strTableAddString(pTable->pStringTable, key.pKey);

        if ( pTable->bWideString )
        {
            assert( _wcsicmp((wchar_t*)pElement->key.pKey, (wchar_t*)key.pKey) == 0 );
        }
    }
    else
    {
#ifdef FULLDEBUG
        if (pTable->bInSharedHeap && ((pTable->eKeyType == StashKeyTypeStrings) || (pTable->eKeyType == StashKeyTypeAddress))) {
            assert(isSharedMemory(key.pKey));
        }
#endif

        // just copy whatever is in the key field, be it a pointer or int
        pElement->key.pKey = key.pKey;
    }

    // We just added an element to our hash table. Increment the size, and check if we need to resize
    if ( !bFound ) // don't add it if it's found in table, it means we are overwriting and there is no increase in size
    {
        pTable->uiSize++;
    }

    // If we are 75% or more full, resize so that storage index hash collision is minimized
    if ( pTable->uiSize >= ((pTable->uiMaxSize >> 1) + (pTable->uiMaxSize >> 2))  )
        stashTableResize(pTable);

    return true;
}


static bool stashRemoveValueInternal(StashTableImp* pTable, StashKey key, U32 uiKeyLengthInBytes, StashValue* pValue)
{
    U32 uiStorageIndex;
    if ( !stashFindIndexByKeyInternal(pTable, key, uiKeyLengthInBytes, &uiStorageIndex, NULL))
    {
        if ( pValue ) pValue->pValue = 0;
        return false;
    }
    if ( pValue )
        pValue->pValue = pTable->pStorage[uiStorageIndex].value.pValue;
    pTable->pStorage[uiStorageIndex].value.pValue = STASH_TABLE_DELETED_SLOT_VALUE;
    pTable->uiValidValues--;
    if ( pTable->bDeepCopyKeys )
    {
    //    Errorf("You can't remove from a deep copy stash table, or it will cause a leak in the string table\n");

        // Instead, just log that you'd like to delete this string
        if ( pTable->pStringTable )
        {
            strTableLogRemovalRequest(pTable->pStringTable, (char*)pTable->pStorage[uiStorageIndex].key.pKey );
        }
    }
    
    return true;
}


// -----------
// Pointer Keys
// -----------
bool stashFindElement(StashTableImp* pTable, const void* pKey, StashElementImp** pElement)
{
    U32 uiKeyLength;
    if (!pTable)
    {
        if (pElement)
            *pElement = NULL;
        return false;
    }

    uiKeyLength = getKeyLength(pTable, pKey);
    assert(pTable->eKeyType != StashKeyTypeInts);
    assert(!pTable->bReadOnly);
    return stashFindElementInternal(pTable, *((StashKey*)&pKey), uiKeyLength, pElement);
}

bool stashFindElementConst(const StashTableImp* pTable, const void* pKey, cStashElement* pElement)
{
    U32 uiKeyLength;
    if (!pTable)
    {
        if (pElement)
            *pElement = NULL;
        return false;
    }

    uiKeyLength = getKeyLength(pTable, pKey);
    assert(pTable->eKeyType != StashKeyTypeInts);
    return stashFindElementInternalConst(pTable, *((StashKey*)&pKey), uiKeyLength, pElement);
}

bool stashFindIndexByKey(const StashTableImp* pTable, const void* pKey, U32* puiIndex)
{
    U32 uiKeyLength = getKeyLength(pTable, pKey);
    assert(pTable);
    assert(pTable->eKeyType != StashKeyTypeInts);
    return stashFindIndexByKeyInternal(pTable, *((StashKey*)&pKey), uiKeyLength, puiIndex, NULL);
}

bool stashGetKey(const StashTableImp* pTable, const void* pKeyIn, const void** pKeyOut)
{
    StashElementImp* pElement;
    U32 uiKeyLength = getKeyLength(pTable, pKeyIn);
    devassert(pTable);
    if (!pTable)
        return false;
    assert(pTable->eKeyType != StashKeyTypeInts);
    if (!stashFindElementInternal(pTable, *((StashKey*)&pKeyIn), uiKeyLength, &pElement))
        return false;
    if (pKeyOut)
        *pKeyOut = pElement->key.pKey;
    return true;
}

bool stashFindKeyByIndex(const StashTableImp* pTable, U32 uiIndex, const void** pKeyOut)
{
    devassert(pTable);
    if (!pTable)
        return false;
    assert(pTable->eKeyType != StashKeyTypeInts);
    return stashFindKeyByIndexInternal(pTable, uiIndex, (StashKey*)pKeyOut);
}


// pointer values
bool stashAddPointer(StashTableImp* pTable, const void* pKey, void* pValue, bool bOverwriteIfFound)
{
    U32 uiKeyLength = getKeyLength(pTable, pKey);
    assert(pTable && !pTable->bReadOnly);
    assert(pTable->eKeyType != StashKeyTypeInts);
    onlyfulldebugdevassert(!pTable->bInSharedHeap || isSharedMemory(pValue));
    return stashAddValueInternal(pTable, *((StashKey*)&pKey), uiKeyLength, *((StashValue*)&pValue), bOverwriteIfFound);
}

bool stashAddPointerConst(cStashTable pTable, const void* pKey, const void* pValue, bool bOverwriteIfFound)
{
    return stashAddPointer(cpp_const_cast(StashTable)(pTable), pKey, cpp_const_cast(void*)(pValue), bOverwriteIfFound);
}

bool stashAddPointerAndGetElement(StashTableImp* pTable, const void* pKey, void* pValue, bool bOverwriteIfFound, StashElement* ppElement)
{
    bool bSuccess;
    U32 uiKeyLength = getKeyLength(pTable, pKey);
    assert(pTable && !pTable->bReadOnly);
    assert(pTable->eKeyType != StashKeyTypeInts);
    onlyfulldebugdevassert(!pTable->bInSharedHeap || isSharedMemory(pValue));
    bSuccess = stashAddValueInternal(pTable, *((StashKey*)&pKey), uiKeyLength, *((StashValue*)&pValue), bOverwriteIfFound);
    if ( bSuccess && ppElement)
        return stashFindElementInternal(pTable, *((StashKey*)&pKey), uiKeyLength, ppElement);
    return bSuccess;
}

bool stashAddPointerAndGetElementConst(StashTableImp* pTable, const void* pKey, const void* pValue, bool bOverwriteIfFound, cStashElement* ppElement)
{
    return stashAddPointerAndGetElement(pTable, pKey, cpp_const_cast(void*)(pValue), bOverwriteIfFound, (StashElement*)ppElement);
}

bool stashRemovePointer(StashTableImp* pTable, const void* pKey, void** ppValue)
{
    U32 uiKeyLength = getKeyLength(pTable, pKey);
    assert(pTable && !pTable->bReadOnly);
    assert(pTable->eKeyType != StashKeyTypeInts);
    return stashRemoveValueInternal(pTable, *((StashKey*)&pKey), uiKeyLength, (StashValue*)ppValue);
}

bool stashFindPointer(StashTableImp* pTable, const void* pKey, void** ppValue)
{
    U32 uiKeyLength;

    if(!pKey)
        return 0;

    if (!pTable)
    {
        if (ppValue)
            *ppValue = NULL;
        return false;
    }

    uiKeyLength = getKeyLength(pTable, pKey);
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType != StashKeyTypeInts);
    return stashFindValueInternal(pTable, *((StashKey*)&pKey), uiKeyLength, (StashValue*)ppValue);
}

bool stashFindPointerConst(const StashTableImp* pTable, const void* pKey, const void** ppValue)
{
    U32 uiKeyLength;
    
    if (!pTable)
    {
        if (ppValue)
            *ppValue = NULL;
        return false;
    }

    uiKeyLength = getKeyLength(pTable, pKey);
    assert(pTable->eKeyType != StashKeyTypeInts);
    return stashFindValueInternal(pTable, *((StashKey*)&pKey), uiKeyLength, (StashValue*)ppValue);
}

// please don't use, for backwards-compatibility only    
void* stashFindPointerReturnPointer(StashTableImp* table, const void* pKey) 
{
    void* pResult;
    if ( table && stashFindPointerConst((cStashTable)table, pKey, &pResult) )
        return pResult;
    return NULL;
}

const void* stashFindPointerReturnPointerConst(cStashTable table, const void* pKey) 
{
    return stashFindPointerReturnPointer(cpp_const_cast(StashTable)(table), pKey);
}

// int values
bool stashAddInt(StashTableImp* pTable, const void* pKey, int iValue, bool bOverwriteIfFound)
{
    StashValue value;
    U32 uiKeyLength = getKeyLength(pTable, pKey);
    value.pValue = S32_TO_PTR(iValue);
    assert(pTable);
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType != StashKeyTypeInts);
    return stashAddValueInternal(pTable, *((StashKey*)&pKey), uiKeyLength, value, bOverwriteIfFound);
}

bool stashAddIntConst(cStashTable table, const void* pKey, int iValue, bool bOverwriteIfFound)
{
    return stashAddInt(cpp_const_cast(StashTable)(table), pKey, iValue, bOverwriteIfFound);
}

bool stashAddIntAndGetElement(StashTableImp* pTable, const void* pKey, int iValue, bool bOverwriteIfFound, StashElementImp** ppElement)
{
    StashValue value;
    bool bSuccess;
    U32 uiKeyLength = getKeyLength(pTable, pKey);
    value.pValue = S32_TO_PTR(iValue);
    assert(pTable);
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType != StashKeyTypeInts);
    bSuccess = stashAddValueInternal(pTable, *((StashKey*)&pKey), uiKeyLength, value, bOverwriteIfFound);
    if ( bSuccess && ppElement)
        return stashFindElementInternal(pTable, *((StashKey*)&pKey), uiKeyLength, ppElement);
    return false;
}

bool stashAddIntAndGetElementConst(cStashTable table, const void* pKey, int iValue, bool bOverwriteIfFound, cStashElement* pElement)
{
    return stashAddIntAndGetElement(cpp_const_cast(StashTable)(table), pKey, iValue, bOverwriteIfFound, cpp_const_cast(StashElement*)(pElement));
}

bool stashRemoveInt(StashTableImp* pTable, const void* pKey, int* piResult)
{
    StashValue value;
    bool ret;
    U32 uiKeyLength = getKeyLength(pTable, pKey);
    assert(pTable);
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType != StashKeyTypeInts);
    ret = stashRemoveValueInternal(pTable, *((StashKey*)&pKey), uiKeyLength, &value);
    if (piResult)
        *piResult = PTR_TO_S32(value.pValue);
    return ret;
}

bool stashFindInt(const StashTableImp* pTable, const void* pKey, int* piResult)
{
    StashValue value;
    bool ret;
    U32 uiKeyLength = getKeyLength(pTable, pKey);
    devassert(pTable);
    if (!pTable)
    {
        if (piResult)
            *piResult = 0;
        return false;
    }
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType != StashKeyTypeInts);
    ret = stashFindValueInternal(pTable, *((StashKey*)&pKey), uiKeyLength, &value);
    if (piResult)
        *piResult = PTR_TO_S32(value.pValue);
    return ret;
}



// -----------
// Int Keys
// -----------
bool stashIntFindElement(StashTableImp* pTable, int iKey, StashElementImp** pElement)
{
    StashKey key;
    key.pKey = S32_TO_PTR(iKey);
    devassert(pTable);
    if (!pTable)
    {
        if (pElement)
            *pElement = NULL;
        return false;
    }
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType == StashKeyTypeInts);
    return stashFindElementInternal(pTable, key, pTable->uiFixedKeyLength, pElement);
}

bool stashIntFindElementConst(const StashTableImp* pTable, int iKey, const StashElementImp** pElement)
{
    StashKey key;
    key.pKey = S32_TO_PTR(iKey);
    devassert(pTable);
    if (!pTable)
    {
        if (pElement)
            *pElement = NULL;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeInts);
    return stashFindElementInternalConst(pTable, key, pTable->uiFixedKeyLength, pElement);
}

bool stashIntFindIndexByKey(const StashTableImp* pTable, int iKey, U32* puiIndex)
{
    StashKey key;
    key.pKey = S32_TO_PTR(iKey);
    assert(pTable);
    assert(pTable->eKeyType == StashKeyTypeInts);
    return stashFindIndexByKeyInternal(pTable, key, pTable->uiFixedKeyLength, puiIndex, NULL);
}

bool stashIntGetKey(const StashTableImp* pTable, int iKeyIn, int* piKeyOut)
{
    StashKey key;
    StashElementImp* pElement;
    key.pKey = S32_TO_PTR(iKeyIn);
    devassert(pTable);
    if (!pTable)
    {
        if (piKeyOut)
            *piKeyOut = 0;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeInts);
    if (!stashFindElementInternal(pTable, key, pTable->uiFixedKeyLength, &pElement))
        return false;
    if (piKeyOut)
        *piKeyOut = PTR_TO_S32(pElement->key.pKey);
    return true;
}

bool stashIntFindKeyByIndex(const StashTableImp* pTable, U32 uiIndex, int* piKeyOut)
{
    StashKey key;
    bool ret;
    devassert(pTable);
    if (!pTable)
    {
        if (piKeyOut)
        {
            *piKeyOut = 0;
            return false;
        }
    }
    assert(pTable->eKeyType == StashKeyTypeInts);
    ret = stashFindKeyByIndexInternal(pTable, uiIndex, &key);
    if (piKeyOut)
        *piKeyOut = PTR_TO_S32(key.pKey);
    return ret;
}


// pointer values
bool stashIntAddPointer(StashTableImp* pTable, int iKey, void* pValue, bool bOverwriteIfFound)
{
    StashKey key;
    key.pKey = S32_TO_PTR(iKey);
    assert(pTable);
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType == StashKeyTypeInts);
    onlyfulldebugdevassert(!pTable->bInSharedHeap || isSharedMemory(pValue));
    return stashAddValueInternal(pTable, key, pTable->uiFixedKeyLength, *((StashValue*)&pValue), bOverwriteIfFound);
}

bool stashIntAddPointerConst(StashTableImp* pTable, int iKey, const void* pValue, bool bOverwriteIfFound)
{
    return stashIntAddPointer(pTable, iKey, cpp_const_cast(void*)(pValue), bOverwriteIfFound);
}

bool stashIntAddPointerAndGetElement(StashTableImp* pTable, int iKey, void* pValue, bool bOverwriteIfFound, StashElementImp** ppElement)
{
    StashKey key;
    bool bSuccess;
    key.pKey = S32_TO_PTR(iKey);
    assert(pTable);
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType == StashKeyTypeInts);
    onlyfulldebugdevassert(!pTable->bInSharedHeap || isSharedMemory(pValue));
    bSuccess = stashAddValueInternal(pTable, key, pTable->uiFixedKeyLength, *((StashValue*)&pValue), bOverwriteIfFound);
    if ( bSuccess && ppElement)
        return stashFindElementInternal(pTable, key, pTable->uiFixedKeyLength, ppElement);
    return bSuccess;
}

bool stashIntRemovePointer(StashTableImp* pTable, int iKey, void** ppValue)
{
    StashKey key;
    key.pKey = S32_TO_PTR(iKey);
    devassert(pTable);
    if (!pTable)
    {
        if (ppValue)
            *ppValue = NULL;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeInts);
    assert(!pTable->bReadOnly);
    return stashRemoveValueInternal(pTable, key, pTable->uiFixedKeyLength, (StashValue*)ppValue);
}

bool stashIntFindPointer(const StashTableImp* pTable, int iKey, void** ppValue)
{
    StashKey key;
    key.pKey = S32_TO_PTR(iKey);
    devassert(pTable);
    if (!pTable)
    {
        if (ppValue)
            *ppValue = NULL;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeInts);
    assert(!pTable->bReadOnly);
    return stashFindValueInternal(pTable, key, pTable->uiFixedKeyLength, (StashValue*)ppValue);
}

bool stashIntFindPointerConst(const StashTableImp* pTable, int iKey, const void** ppValue)
{
    StashKey key;
    key.pKey = S32_TO_PTR(iKey);
    devassert(pTable);
    if (!pTable)
    {
        if (ppValue)
            *ppValue = NULL;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeInts);
    return stashFindValueInternal(pTable, key, pTable->uiFixedKeyLength, (StashValue*)ppValue);
}

// int values
bool stashIntAddInt(StashTableImp* pTable, int iKey, int iValue, bool bOverwriteIfFound)
{
    StashValue value;
    StashKey key;
    key.pKey = S32_TO_PTR(iKey);
    value.pValue = S32_TO_PTR(iValue);
    assert(pTable);
    assert(pTable->eKeyType == StashKeyTypeInts);
    assert(!pTable->bReadOnly);
    return stashAddValueInternal(pTable, key, pTable->uiFixedKeyLength, value, bOverwriteIfFound);
}

bool stashIntAddIntConst(cStashTable table, int iKey, int iValue, bool bOverwriteIfFound)
{
    return stashIntAddInt(cpp_const_cast(StashTable)(table), iKey, iValue, bOverwriteIfFound);
}

bool stashIntAddIntAndGetElement(StashTableImp* pTable, int iKey, int iValue, bool bOverwriteIfFound, StashElementImp** ppElement)
{
    StashValue value;
    StashKey key;
    bool bSuccess;
    key.pKey = S32_TO_PTR(iKey);
    value.pValue = S32_TO_PTR(iValue);
    assert(pTable);
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType == StashKeyTypeInts);
    bSuccess = stashAddValueInternal(pTable, key, pTable->uiFixedKeyLength, value, bOverwriteIfFound);
    if ( bSuccess && ppElement)
        return stashFindElementInternal(pTable, key, pTable->uiFixedKeyLength, ppElement);
    return bSuccess;
}

bool stashIntRemoveInt(StashTableImp* pTable, int iKey, int* piResult)
{
    StashValue value;
    StashKey key;
    bool ret;
    key.pKey = S32_TO_PTR(iKey);
    assert(pTable);
    assert(pTable->eKeyType == StashKeyTypeInts);
    assert(!pTable->bReadOnly);
    ret = stashRemoveValueInternal(pTable, key, pTable->uiFixedKeyLength, &value);
    if (piResult)
        *piResult = PTR_TO_S32(value.pValue);
    return ret;
}

bool stashIntFindInt(const StashTableImp* pTable, int iKey, int* piResult)
{
    bool ret;
    StashValue value;
    StashKey key;
    key.pKey = S32_TO_PTR(iKey);
    devassert(pTable);
    if (!pTable)
    {
        if (piResult)
            *piResult = 0;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeInts);
    assert(!pTable->bReadOnly);
    ret = stashFindValueInternal(pTable, key, pTable->uiFixedKeyLength, &value);
    if (piResult)
        *piResult = PTR_TO_S32(value.pValue);
    return ret;
}

// -----------
// Address Keys
// -----------
bool stashAddressFindElement(StashTableImp* pTable, const void* pKey, StashElementImp** pElement)
{
    devassert(pTable);
    if (!pTable)
    {
        if (pElement)
            *pElement = NULL;
        return false;
    }
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType == StashKeyTypeAddress);
    return stashFindElementInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, pElement);
}

bool stashAddressFindElementConst(const StashTableImp* pTable, const void* pKey, const StashElementImp** pElement)
{
    devassert(pTable);
    if (!pTable)
    {
        if (pElement)
            *pElement = NULL;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeAddress);
    return stashFindElementInternalConst(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, pElement);
}

bool stashAddressFindIndexByKey(const StashTableImp* pTable, const void* pKey, U32* puiIndex)
{
    assert(pTable);
    assert(pTable->eKeyType == StashKeyTypeAddress);
    return stashFindIndexByKeyInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, puiIndex, NULL);
}

bool stashAddressGetKey(const StashTableImp* pTable, const void* pKeyIn, void** ppKeyOut)
{
    StashElementImp* pElement;
    devassert(pTable);
    if (!pTable)
    {
        if (ppKeyOut)
            *ppKeyOut = 0;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeAddress);
    if (!stashFindElementInternal(pTable, *((StashKey*)&pKeyIn), pTable->uiFixedKeyLength, &pElement))
        return false;
    *ppKeyOut = pElement->key.pKey;
    return true;
}

bool stashAddressFindKeyByIndex(const StashTableImp* pTable, U32 uiIndex, void** ppKeyOut)
{
    devassert(pTable);
    if (!pTable)
    {
        if (ppKeyOut)
        {
            *ppKeyOut = 0;
            return false;
        }
    }
    assert(pTable->eKeyType == StashKeyTypeAddress);
    return stashFindKeyByIndexInternal(pTable, uiIndex, (StashKey*)ppKeyOut);
}


// pointer values
bool stashAddressAddPointer(StashTableImp* pTable, const void* pKey, void* pValue, bool bOverwriteIfFound)
{
    assert(pTable);
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType == StashKeyTypeAddress);
    onlyfulldebugdevassert(!pTable->bInSharedHeap || isSharedMemory(pValue));
    return stashAddValueInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, *((StashValue*)&pValue), bOverwriteIfFound);
}

bool stashAddressAddPointerAndGetElement(StashTableImp* pTable, const void* pKey, void* pValue, bool bOverwriteIfFound, StashElementImp** ppElement)
{
    bool bSuccess;
    assert(pTable);
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType == StashKeyTypeAddress);
    onlyfulldebugdevassert(!pTable->bInSharedHeap || isSharedMemory(pValue));
    bSuccess = stashAddValueInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, *((StashValue*)&pValue), bOverwriteIfFound);
    if ( bSuccess && ppElement)
        return stashFindElementInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, ppElement);
    return bSuccess;
}

bool stashAddressRemovePointer(StashTableImp* pTable, const void* pKey, void** ppValue)
{
    devassert(pTable);
    if (!pTable)
    {
        if (ppValue)
            *ppValue = NULL;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeAddress);
    assert(!pTable->bReadOnly);
    return stashRemoveValueInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, (StashValue*)ppValue);
}

bool stashAddressFindPointer(const StashTableImp* pTable, const void* pKey, void** ppValue)
{
    devassert(pTable);
    if (!pTable)
    {
        if (ppValue)
            *ppValue = NULL;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeAddress);
    assert(!pTable->bReadOnly);
    return stashFindValueInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, (StashValue*)ppValue);
}

bool stashAddressFindPointerConst(const StashTableImp* pTable, const void* pKey, const void** ppValue)
{
    devassert(pTable);
    if (!pTable)
    {
        if (ppValue)
            *ppValue = NULL;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeAddress);
    return stashFindValueInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, (StashValue*)ppValue);
}

// int values
bool stashAddressAddInt(StashTableImp* pTable, const void* pKey, int iValue, bool bOverwriteIfFound)
{
    StashValue value;
    value.pValue = S32_TO_PTR(iValue);
    assert(pTable);
    assert(pTable->eKeyType == StashKeyTypeAddress);
    assert(!pTable->bReadOnly);
    return stashAddValueInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, value, bOverwriteIfFound);
}

bool stashAddressAddIntAndGetElement(StashTableImp* pTable, const void* pKey, int iValue, bool bOverwriteIfFound, StashElementImp** ppElement)
{
    StashValue value;
    bool bSuccess;
    value.pValue = S32_TO_PTR(iValue);
    assert(pTable);
    assert(!pTable->bReadOnly);
    assert(pTable->eKeyType == StashKeyTypeAddress);
    bSuccess = stashAddValueInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, value, bOverwriteIfFound);
    if ( bSuccess && ppElement)
        return stashFindElementInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, ppElement);
    return false;
}

bool stashAddressAddIntAndGetElementConst(cStashTable table, const void* pKey, int iValue, bool bOverwriteIfFound, cStashElement* pElement)
{
    return stashAddressAddIntAndGetElement(cpp_const_cast(StashTable)(table), pKey, iValue, bOverwriteIfFound, cpp_const_cast(StashElement*)(pElement));
}

bool stashAddressRemoveInt(StashTableImp* pTable, const void* pKey, int* piResult)
{
    StashValue value;
    bool ret;
    assert(pTable);
    assert(pTable->eKeyType == StashKeyTypeAddress);
    assert(!pTable->bReadOnly);
    ret = stashRemoveValueInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, &value);
    if (piResult)
        *piResult = PTR_TO_S32(value.pValue);
    return ret;
}

bool stashAddressFindInt(const StashTableImp* pTable, const void* pKey, int* piResult)
{
    StashValue value;
    bool ret;
    devassert(pTable);
    if (!pTable)
    {
        if (piResult)
            *piResult = 0;
        return false;
    }
    assert(pTable->eKeyType == StashKeyTypeAddress);
    assert(!pTable->bReadOnly);
    ret = stashFindValueInternal(pTable, *((StashKey*)&pKey), pTable->uiFixedKeyLength, &value);
    if (piResult)
        *piResult = PTR_TO_S32(value.pValue);
    return ret;
}


// -----------
// Stash Tracker
// -----------

#ifdef STASH_TABLE_TRACK
typedef struct HTTrackerInfo
{
    char cName[128];
    int iNumStashTables;
    unsigned int uiTotalSize;
    unsigned int uiStringTableSize;
} HTTrackerInfo;

FILE* stashDumpFile;

static int stashTrackerProcessor(StashElement a)
{
    HTTrackerInfo* pInfo = stashElementGetPointer(a);
    printf("%.1f KB\t%.1f KB\t%d tables\t%s\n", ((float)pInfo->uiTotalSize) / 1024.0f, ((float)pInfo->uiStringTableSize) / 1024.0f, pInfo->iNumStashTables, pInfo->cName );
    fprintf(stashDumpFile, "%d,%d,%d,%s\n", pInfo->uiTotalSize, pInfo->uiStringTableSize, pInfo->iNumStashTables, pInfo->cName );
    return 1;
}

void printStashTableMemDump()
{
    StashTableImp* pList = pStashTableList;
    StashTable pStashTableStashTable = stashTableCreateWithStringKeys(64, StashDeepCopyKeys);
    int iTotalOver64k = 0;
    int iTotalUnder64k = 0;

    stashDumpFile = NULL;

    while (!stashDumpFile)
    {
        stashDumpFile = fopen( "c:/hashdump.csv", "wt" );
        if (!stashDumpFile)
        {
            printf("Let go of c:/hashdump.csv, please!\n");
            Sleep(1000);
        }
    }    
    fprintf(stashDumpFile, "Size,StringTableSize,NumInstances,Name\n");

    printf("Hash Table Mem Usage\n");
    printf("----------------------\n");

    while (pList)
    {
        HTTrackerInfo* pInfo;
        int iMemUsage = stashGetMemoryUsage((cStashTable)pList);
        int iStringTableUsage = 0;

        if ( pList->pStringTable )
            iStringTableUsage = strTableMemUsage(pList->pStringTable);


        if ( !stashFindPointer(pStashTableStashTable, pList->cName, &pInfo))
        {
            pInfo = calloc(1, sizeof(HTTrackerInfo));
            strcpy(pInfo->cName, pList->cName);
            stashAddPointer(pStashTableStashTable, pList->cName, pInfo, false);
        }

        assert(pInfo);
        pInfo->iNumStashTables++;
        pInfo->uiTotalSize += iMemUsage;
        pInfo->uiStringTableSize += iStringTableUsage;

        //printf("%5d - %s\n", iMemUsage, pList->cName);
        
        if ( iMemUsage > 1024 * 64)
        {
            iTotalOver64k += iMemUsage;
        }
        else
        {
            iTotalUnder64k += iMemUsage;
        }
        pList = pList->pNext;
    }
    stashForEachElement(pStashTableStashTable, stashTrackerProcessor);

    printf("----------------------\n");
    printf("Under64k = %d        Over64k = %d        Total = %d\n", iTotalUnder64k, iTotalOver64k, iTotalOver64k + iTotalUnder64k);
    printf("----------------------\n");

    stashTableDestroy(pStashTableStashTable);
    fclose(stashDumpFile);
}
#else
void printStashTableMemDump()
{
    printf("ERROR: Please #define STASH_TABLE_TRACK in stashtable.c for stash table tracking information!\n");
}
#endif

int stashStorageIsNotNull(const StashTableImp* table)
{
    return !!table->pStorage;
}

void stashTableMultiLevelAddVA( StashTableImp* pTable, int depth, va_list va )
{
    while( depth > 0 )
    {
        StashElement elt;
        char* param = va_arg(va, char*);

        stashFindElement( pTable, param, &elt);
        if(!elt)
        {
            StashTable stNew = stashTableCreateWithStringKeys( 8, StashDeepCopyKeys );
            stashAddPointerAndGetElement(pTable, param, stNew, false, &elt);
        }

        if(verify( elt ))
        {
            pTable = stashElementGetPointer(elt);
        }        
        depth--;
    }
}

void stashTableMultiLevelAdd( StashTableImp* pTable, int depth, ... )
{
    va_list va;
    va_start( va, depth );
    stashTableMultiLevelAddVA(pTable, depth, va);
    va_end( va );
}


#pragma warning(pop)
