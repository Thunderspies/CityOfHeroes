#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/SharedHeap.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/wininclude.h"

typedef struct StringCache
{
    cStashTable table;     
    CRITICAL_SECTION crit;
    U32 reserved;
} StringCache;
static StringCache alloc_add_string_table;

typedef struct SharedStringCache
{
    cStashTable table;     
    U32 reserved;
} SharedStringCache;
static SHARED_MEMORY SharedStringCache * shared_string_table = NULL;

void stringCachePreAlloc(int size)
{
    alloc_add_string_table.reserved = size;

    if (!alloc_add_string_table.table)
    {
        InitializeCriticalSectionAndSpinCount(&alloc_add_string_table.crit, 4000);
        alloc_add_string_table.table = stashTableCreateWithStringKeys(size, StashDefault|StashDeepCopyKeys);
    }
    else
    {
        stashTableSetSizeConst(alloc_add_string_table.table, size);
    }
}

void StringCache_Init()
{
    if (!alloc_add_string_table.table)
        stringCachePreAlloc(8192);
}

const char *StringCache_AllocAddString(StringCache *sc, const char *s)
{
    cStashElement element;
    
    EnterCriticalSection(&sc->crit);

    if (!stashFindElementConst(sc->table, s, &element)
        && !stashAddIntAndGetElementConst(sc->table, s, 1, false, &element))
        assert(0);

    LeaveCriticalSection(&sc->crit);

    return stashElementGetStringKey(element);
}

const char* allocAddString( const char * s )
{
    StringCache_Init();
    return StringCache_AllocAddString(&alloc_add_string_table, s);
}

const char* allocAddString_checked(const char *s)
{
    if (!s)
        return NULL;

    return allocAddString(s);
}

const char* allocFindString(const char * s)
{
    char const *res = NULL;
    cStashElement element;
    StringCache_Init();

    EnterCriticalSection(&alloc_add_string_table.crit);

    if (stashFindElementConst(alloc_add_string_table.table, s, &element))
        res = stashElementGetStringKey(element);

    LeaveCriticalSection(&alloc_add_string_table.crit);

    return res;
}

// Shared memory string table functions
void sharedStringReserve(int count)
{
    SharedHeapHandle* handle;

    switch (sharedHeapAcquire(&handle, "sharedStringTable"))
    {
        xcase SHAR_FirstCaller:
        {
            SharedStringCache *sscache;

            assert(sharedHeapAlloc(handle, sizeof(SharedStringCache)));
            shared_string_table = sscache = cpp_reinterpret_cast(SharedStringCache*)(handle->data);

            sscache->table = stashTableCreateWithStringKeys(stashOptimalSize(count), StashSharedHeap);
            sscache->reserved = count;

            sharedHeapMemoryManagerUnlock();
        }
        xcase SHAR_DataAcquired:
        {
            shared_string_table = handle->data;
        }
        xcase SHAR_Error:
        {
            sharedHeapTurnOff("disabled");
        }
    }    
}

const char* allocAddSharedString(const char * s)
{
    cStashElement elem;

    assert(s);

    if (!stashFindElementConst(shared_string_table->table, s, &elem))
    {
        size_t size = strlen(s)+1;
        char * add = sharedMalloc(size);
        memcpy(add, s, size);
        assert(stashAddIntAndGetElementConst(shared_string_table->table, add, 1, false, &elem));
        assert(stashGetValidElementCount(shared_string_table->table) < shared_string_table->reserved);
    }

    return stashElementGetStringKey(elem);
}

// Indexed string functions

static constCharPtr *indexed_string_list;
static StashTable indexed_string_table;
static CRITICAL_SECTION indexed_string_table_crit;

const char* allocAddIndexedString( const char * s )
{
    int index;
    StashElement element;
    if (!indexed_string_table)
    {
        indexed_string_table= stashTableCreateWithStringKeys(4096, StashDefault/*|StashDeepCopyKeys*/); // deep copying the indexed strings causes leaks and wasted memory
        InitializeCriticalSectionAndSpinCount(&indexed_string_table_crit, 4000);
    }

    if (!stashFindElement(indexed_string_table, s, &element))
    {
        index = eaSize(&indexed_string_list);
        // Not found, add it!
        EnterCriticalSection(&indexed_string_table_crit);
        if(!stashFindElement(indexed_string_table, s, &element))
        {
            if (!stashAddIntAndGetElement(indexed_string_table, s, index+1, false, &element)) {
                assert(0);
            } else {
                // Add to list as well
                eaPushConst(&(char**)indexed_string_list, stashElementGetStringKey(element));
            }
        }
        LeaveCriticalSection(&indexed_string_table_crit);
    }
    return stashElementGetStringKey(element);
}

int stringToReference(const char* str)
{
    int index;
    if (stashFindInt(indexed_string_table, str, &index))
        return index;
    return 0;
}

const char* stringFromReference(int ref)
{
    if (ref > 0 && ref <= eaSize(&indexed_string_list))
    {
        return indexed_string_list[ref-1];
    }
    return NULL;
}

static int stringCompare(const char **str1, const char **str2)
{
    return stricmp(*str1, *str2);
}

void stringReorder(void)
{
    int i;
    int num_strings = eaSize(&indexed_string_list);

    // Reorder the string table.
    eaQSort((char**)indexed_string_list, stringCompare);

    // Rebuild hash table.
    for(i = 0; i < num_strings; i++)
    {
        EnterCriticalSection(&indexed_string_table_crit);
        if (!stashAddInt(indexed_string_table, indexed_string_list[i], i+1, true))
            assert(0);
        LeaveCriticalSection(&indexed_string_table_crit);
    }
}

int numIndexedStringsInStringTable(void)
{
    return eaSize(&indexed_string_list);
}
