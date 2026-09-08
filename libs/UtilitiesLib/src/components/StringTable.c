/* File StringTable.c
 */


#pragma warning(push)
//#pragma warning(disable:4028) // parameter differs from declaration
//#pragma warning(disable:4047)

#include "utilitieslib/components/StringTable.h"
#include "utilitieslib/components/ArrayOld.h"
#include <string.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/MemoryMonitor.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/serialize.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/SharedHeap.h"

#ifndef _MEMCHECK_H
#include <malloc.h>
#endif

#define DEFAULT_STARTING_MEM_CHUNK_SIZE 128 // available chars to start with
#define MAX_MEMCHUNK_SIZE 131072 // MK: mem chunks can easily be bigger than this when compacted with strTableCopyToAllocatedSpace

//#define STRING_TABLE_TRACK

typedef struct MemChunk
{
    char*                pcData;
    size_t                uiSize;
    size_t                uiCurrentIndex;
    struct MemChunk*    pNextChunk;

} MemChunk;


typedef struct StringTableImp 
{
    Array indexTable;    // array of string pointers that points into the chunkTable

    MemChunk* pFirstMemChunk;
    //int iSharedStringCount;
    SharedHeapHandle* pSharedHeapHandle;
//     StashTable stringStash;
#ifdef STRING_TABLE_TRACK
#define STRTABLEIMP_NAME_LEN 128
    char cName[STRTABLEIMP_NAME_LEN];
    struct StringTableImp* pNext;
    size_t uiMemLeaked;
#endif
    unsigned int indexable        : 1;
    unsigned int wideString        : 1;
    unsigned int readOnly        : 1;
} StringTableImp;




#ifdef STRING_TABLE_TRACK
StringTableImp* stringTableList;
#endif

static MemChunk* pushNewMemChunk( size_t uiSize )
{
    MemChunk* pNewChunk = calloc(1, sizeof(MemChunk));
    pNewChunk->uiSize = uiSize;
    pNewChunk->pcData = calloc( pNewChunk->uiSize, sizeof(char) );
    return pNewChunk;
}

StringTable strTableCreate(StringTableMode mode)
{
    StringTableImp* pTable = createStringTable();
    initStringTable(pTable, 128);
    strTableSetMode(pTable, mode);
    return pTable;
}

// Constructor + Destructor
StringTable createStringTable()
{
    return calloc(1, sizeof(StringTableImp));
}

#ifdef STRING_TABLE_TRACK
static void trackTable(StringTableImp* table, const char *pFile, int iLine)
{
    StringTableImp* pList = stringTableList;
    sprintf_s(SAFESTR(table->cName), "ST: %s - %d", pFile, iLine);

    if ( stringTableList )
    {
        while ( pList->pNext )
            pList = pList->pNext;

        pList->pNext = table;
    }
    else
    {
        stringTableList = table;
    }
}
#endif

void initStringTableEx(StringTableImp* table, unsigned int chunkSize, const char* pFile, int iLine )
{
#ifdef STRING_TABLE_TRACK
    trackTable(table,pFile,iLine);
#endif

    assert(!table->readOnly);

    // We need to initalize the first, static memorychunk to the smallest size
    table->pFirstMemChunk = pushNewMemChunk(DEFAULT_STARTING_MEM_CHUNK_SIZE); // FIXME: why isn't chunkSize used?
}

void destroyStringTable(StringTableImp* table)
{
    MemChunk* pMemChunk = table->pFirstMemChunk;


    if ( table->pSharedHeapHandle )
    {
#ifndef _XBOX
        sharedHeapMemoryManagerLock();
        sharedHeapRelease(table->pSharedHeapHandle);
        sharedHeapMemoryManagerUnlock();
#else
        assert(0);
#endif
        return;
    }
    else
#ifdef STRING_TABLE_TRACK
    {
        // remove from list
        StringTableImp* pList = stringTableList;
        assert( pList );

        if ( pList == table )
        {
            stringTableList = table->pNext;
        }
        else
        {
            StringTableImp* pPrev = stringTableList;
            pList = stringTableList->pNext;
            while ( pList != table )
            {
                pPrev = pList;
                pList = pList->pNext;
            }
            assert( pList == table );
            pPrev->pNext = pList->pNext;
        }
    }
#endif



    assert(!table->readOnly);

    // Free linked list of chunks
    while ( pMemChunk )
    {
        MemChunk* pNextChunk = pMemChunk->pNextChunk;
        free(pMemChunk->pcData);
        free(pMemChunk);
        pMemChunk = pNextChunk;
    }

    // Free the index table
    destroyArrayPartial(&table->indexTable);

//     // Free the NoRedundance StashTable
//     stashTableDestroy(table->stringStash);

    // Free table itself
    free(table);
}

size_t strTableMemUsage(StringTableImp* table)
{
    size_t uiTotalSize = sizeof(StringTableImp); // don't count first memchunk struct twice
    MemChunk* pMemChunk = table->pFirstMemChunk;
    while ( pMemChunk )
    {
        uiTotalSize += sizeof(MemChunk);
        uiTotalSize += sizeof(char) * pMemChunk->uiSize;
        pMemChunk = pMemChunk->pNextChunk;
    }

    // Add table->indexTable size
    if ( table->indexable )
        uiTotalSize += table->indexTable.maxSize * sizeof(void*);

//     // Add table->stringStash size
//     if(table->stringStash)
//         uiTotalSize += stashGetMemoryUsage(table->stringStash);

    return uiTotalSize;
}


// Only the memory used for strings
static size_t strTableStringMemUsage(StringTableImp* table)
{
    size_t uiTotalSize = 0;

    MemChunk* pMemChunk = table->pFirstMemChunk;
    while ( pMemChunk )
    {
        uiTotalSize += pMemChunk->uiCurrentIndex;
        pMemChunk = pMemChunk->pNextChunk;
    }

    return uiTotalSize;
}

static size_t strTableWastage(StringTableImp* table)
{
    size_t uiTotalSize = 0;
    MemChunk* pMemChunk = table->pFirstMemChunk;
    while ( pMemChunk )
    {
        uiTotalSize += (pMemChunk->uiSize - pMemChunk->uiCurrentIndex);
        pMemChunk = pMemChunk->pNextChunk;
    }

    // Add table->indexTable unused size
    if ( table->indexable )
    {
        uiTotalSize += (table->indexTable.maxSize - table->indexTable.size) * sizeof(void*);
    }

    // TODO: add wasted stash space, table->stringStash is currently considered 'all used'

    return uiTotalSize;
}

#ifndef _XBOX
StringTableImp*  strTableCheckForSharedCopy(const char* pcStringSharedHashKey, bool* bFirstCaller)
{
    SharedHeapAcquireResult ret = SHAR_Error;
    SharedHeapHandle* pHandle;
    StringTableImp* pSharedTable = NULL;

    // Try to allocate that in shared memory
    ret = sharedHeapAcquire(&pHandle, pcStringSharedHashKey);

    if ( ret == SHAR_FirstCaller )
    {
        *bFirstCaller = true;
    }
    else if ( ret == SHAR_Error )
    {
        *bFirstCaller = false;
    }
    else if (ret == SHAR_DataAcquired)
    {
        *bFirstCaller = false;
        pSharedTable = pHandle->data;
        assert( pSharedTable );
    }
    else
    {
        // no other ret values, right?
        assert(0);
    }

    return pSharedTable;
}
#endif

size_t strTableGetCopyTargetAllocSize(StringTable table)
{
    size_t uiNumStrings = strTableGetStringCount(table);

    size_t uiTotalStringSize = strTableStringMemUsage(table);
    size_t uiIndexArraySize = uiNumStrings * sizeof(char*);
    size_t uiOverheadSize = sizeof(StringTableImp) + sizeof(MemChunk);
    // table->stringStash is not needed and will not be copied

    size_t uiTotalSize = uiOverheadSize + uiIndexArraySize + uiTotalStringSize;
    return uiTotalSize;
}

StringTable strTableCopyToAllocatedSpace(StringTable table, void* pAllocatedSpace, size_t uiTotalSize )
{
    StringTableImp* pStringTableImp = memset(pAllocatedSpace, 0, uiTotalSize); 

    int uiNumStrings = strTableGetStringCount(table);

    size_t uiTotalStringSize = strTableStringMemUsage(table);
    size_t uiIndexArraySize = uiNumStrings * sizeof(char*); // size of an array
    size_t uiOverheadSize = sizeof(StringTableImp) + sizeof(MemChunk);




    int uiStringIndex;
    MemChunk* pMemChunk = pStringTableImp->pFirstMemChunk = (MemChunk*)((char*)pAllocatedSpace + sizeof(StringTableImp));

    assert( uiTotalSize == uiOverheadSize + uiIndexArraySize + uiTotalStringSize ); // to be safe


	pStringTableImp->indexTable.storage =
		(void **)(pStringTableImp->pFirstMemChunk + 1);
    pStringTableImp->pFirstMemChunk->pcData = (char*)pStringTableImp->indexTable.storage + uiIndexArraySize;
    // Make sure we haven't gone over or under total allocation
    assert( pStringTableImp->pFirstMemChunk->pcData + uiTotalStringSize == (char*)pAllocatedSpace + uiTotalSize);

    // Fill out string table struct
    pStringTableImp->indexable = 1;
    pStringTableImp->wideString = table->wideString;

    // Fill out the memchunk
    pMemChunk->uiSize = uiTotalStringSize;

    // Now, copy every string, in order that they were indexed in the original table
    pStringTableImp->indexTable.maxSize = uiNumStrings;
    pStringTableImp->indexTable.size = 0;
    for (uiStringIndex = 0; uiStringIndex < uiNumStrings; ++uiStringIndex)
    {
        assert(!arrayIsFull(&pStringTableImp->indexTable)); // this would be very bad
        strTableAddString(pStringTableImp, strTableGetString(table, uiStringIndex));
    }

    // Check results
    assert( 
        pStringTableImp->indexTable.maxSize == pStringTableImp->indexTable.size
        && pStringTableImp->indexTable.size == uiNumStrings
        );
    assert( pMemChunk->uiCurrentIndex == pMemChunk->uiSize ); // should be packed
    assert( pMemChunk->pNextChunk == NULL ); // didn't add any memchunks!

    // Return the shared string table!
    return pStringTableImp;
}


#ifndef _XBOX
StringTableImp* strTableCopyIntoSharedHeap(StringTableImp* table, const char* pcStringSharedHashKey, bool bAlreadyAreFirstCaller )
{
    U32 uiNumStrings = strTableGetStringCount(table);

    size_t uiTotalStringSize = strTableStringMemUsage(table);
    size_t uiIndexArraySize = uiNumStrings * sizeof(char*);
    size_t uiOverheadSize = sizeof(StringTableImp) + sizeof(MemChunk);

    size_t uiTotalSize = uiOverheadSize + uiIndexArraySize + uiTotalStringSize;
    SharedHeapAcquireResult ret = SHAR_Error;
    SharedHeapHandle* pHandle = NULL;


    assert(table->indexable);

    // Try to allocate that in shared memory
    if ( bAlreadyAreFirstCaller )
        ret = SHAR_FirstCaller; // i hope you know what you are doing
    else
        ret = sharedHeapAcquire(&pHandle, pcStringSharedHashKey);

    if ( ret == SHAR_FirstCaller )
    {
        bool bSuccess = sharedHeapAlloc(pHandle, uiTotalSize);
        if ( !bSuccess )
        {
            sharedHeapMemoryManagerUnlock();
            return NULL;
        }
        else
        {
            // Ok, now we need to do the real copying, since we're the first ones here...
            StringTableImp* pStringTableImp = strTableCopyToAllocatedSpace(table, pHandle->data, uiTotalSize);

            // Set the string table to be readOnly!
            pStringTableImp->readOnly = 1;

            // Unlock shared heap
            sharedHeapMemoryManagerUnlock();

            // Return the shared string table!
            return pStringTableImp;
        }
    }
    else if ( ret == SHAR_Error )
    {
            return NULL;
    }
    else // ret == SHAR_DataAcquired
    {
        StringTableImp* pSharedTable = pHandle->data;

        assert( pSharedTable );
        // Make sure the shared one is the same
        if ( 
            strTableGetStringCount(table) != strTableGetStringCount(pSharedTable)
            || strTableStringMemUsage(table) != strTableStringMemUsage(pSharedTable)
            )
        {

            sharedHeapRelease(pHandle);
            return NULL; // must have been reloaded or something, forget about it
        }

        return pSharedTable;
    }

    assert(0); // shouldn't get here
    return NULL;
}
#endif

int strTableGetStringCount(StringTableImp* table)
{
    if(table->indexable)
    {
        return table->indexTable.size;
    }
    else
        return 0;
}




void* strTableAddString(StringTableImp* table, const void* str)
{
    MemChunk* pMemChunk;
    char* newStringStartLocation = NULL;
    U32 strSize;
    U32 strMemSize;
    
    assert(!table->readOnly);

    // Calculate how much memory the new string is going to need.
    // If the string is a wide string, it will need twice the normal amount of memory to store it.
    if(table->wideString)
    {
        strSize = (int)wcslen(str) + 1;  // strict string size plus null terminating char
        strMemSize = strSize << 1;  // strict string size plus null terminating char
    }
    else
    {
        strMemSize = strSize = (int)strlen(str) + 1;
    }

//     if(table->stringStash && stashFindPointer(table->stringStash,str,&newStringStartLocation))
//     {
//         // NoRedundance is on, and we already have this string
//         return newStringStartLocation;
//     }
//     else
//     {
        newStringStartLocation = NULL;
//     }

    // Find a MemChunk that is capable of holding the given string
    // note that the biggest memchunk is always first,
    // so most of the time we only have to look at one
    pMemChunk = table->pFirstMemChunk;
    
    while ( pMemChunk )
    {
        size_t uiMemAvailable = pMemChunk->uiSize - pMemChunk->uiCurrentIndex;

        if ( uiMemAvailable >= strMemSize )
        {
            newStringStartLocation = &pMemChunk->pcData[pMemChunk->uiCurrentIndex];
            break;
        }
        pMemChunk = pMemChunk->pNextChunk;
    }

    // Could not find a slot, so make a new one, and push it on to the list,
    // so it is first (to speed up future searches).
    if ( !newStringStartLocation )
    {
        MemChunk* pOldFirstChunk = table->pFirstMemChunk;
        size_t uiSize = MIN(pOldFirstChunk->uiSize << 1, MAX_MEMCHUNK_SIZE); // the next chunk is bigger

        assert( !table->pSharedHeapHandle ); // can't add memchunks if we're in shared heap
        table->pFirstMemChunk = pushNewMemChunk(uiSize);
        table->pFirstMemChunk->pNextChunk = pOldFirstChunk;

        // Added a new memory chunk, and now we call ourselves again.
        return strTableAddString(table, str);
    }


    // Ok, we have enough space in some memory chunk, let's copy the string there
    if(table->wideString)
        wcsncpy_s((wchar_t*)newStringStartLocation, (pMemChunk->uiSize - pMemChunk->uiCurrentIndex) >> 1,
                str, strSize);
    else
        strncpy_s(newStringStartLocation, pMemChunk->uiSize - pMemChunk->uiCurrentIndex, str, strSize);
    pMemChunk->uiCurrentIndex += strMemSize;

    // Store the address to the new string in the index table if the table
    // is supposed to be indexable.
    if(table->indexable)
    {
        arrayPushBack(&table->indexTable, newStringStartLocation);
    }

//     // Put the string in the stash table
//     if(table->stringStash)
//     {
//         bool ret = false;
//         if(table->indexable)
//             ret = stashAddInt(table->stringStash, newStringStartLocation, strTableGetStringCount(table)-1, false);
//         else
//             ret = stashAddPointer(table->stringStash, newStringStartLocation, newStringStartLocation, false);
//         assert(ret);
//     }

    return newStringStartLocation;
}

int strTableAddStringGetIndex(StringTable table, const void* str)
{
    assert( table->indexable );

    strTableAddString(table, str);

//     if(table->stringStash)
//     {
//         int ret;
//         assert(stashFindInt(table->stringStash,str,&ret));
//         return ret;
//     }
//     else
        return strTableGetStringCount(table) - 1;
}


/* Function strTableClear
 *    Clears the entire string table without altering the table size in any way.
 *    
 *    Remember to throw aways any old pointers into the string table after clearing
 *    a string table.  This function actually only resets the size of all memory 
 *    chunks to zero.  Therefore, any existing pointers into the string table would 
 *    still be "legal" even though they are invalidated logically.  The contents of 
 *    the string pointed to by the old pointers will change when new strings are inserted.
 *
 */
void strTableClear(StringTableImp* table)
{
    MemChunk* pMemChunk = table->pFirstMemChunk;

    assert(!table->readOnly);

    while ( pMemChunk )
    {
        pMemChunk->uiCurrentIndex = 0;
        pMemChunk = pMemChunk->pNextChunk;
    }

    // Free the index table
    destroyArrayPartial(&table->indexTable);

//     // Clear the stash table
//     stashTableClear(table->stringStash);
}

/* Function strTableGetString
 *    Returns a string given it's index in the string table.  This function
 *    will always return NULL unless the string table is indexable.
 *
 */

static char* getString(StringTable table, int index)
{
    // If the table is not indexable, this operation cannot be completed because
    // no string addresses were stored.
    if(!table->indexable)
    {
        assert( "Can't call strTableGetString on non-indexable string table!" == 0);
        return NULL;
    }


    // If the requested index is invalid, return a dummy value.
    if ( index < 0 )
        return NULL;

    if (index >= table->indexTable.size)
        return NULL;
    return table->indexTable.storage[index];
}

char* strTableGetString(StringTableImp* table, int index)
{
    if(table->readOnly)
    {
        assert( "Please call strTableGetConstString instead, this table is readOnly!" == 0);
        return NULL;
    }

    return getString(table, index);
}

const char* strTableGetConstString(StringTable table, int index)
{
    return getString(table, index);
}

StashTable strTableCreateStringToIndexMap(StringTable table)
{
    if(!table->indexable)
    {
        assert( "Can't call strTableCreateStringToIndexMap on non-indexable string table!" == 0);
        return NULL;
    }

//     if(table->stringStash)
//     {
//         // hey, we already have the map ;)
//         return stashTableClone(table->stringStash,NULL,NULL);
//     }

    {
        U32 uiTotalStrings = strTableGetStringCount(table);
        StashTable stStringToIndexMap = stashTableCreateWithStringKeys(uiTotalStrings, StashDefault);
        U32 uiIndex;

        for (uiIndex=0; uiIndex < uiTotalStrings; ++uiIndex)
        {
            bool bSuccess = stashAddInt(stStringToIndexMap, strTableGetConstString(table, uiIndex), uiIndex, false);
            assert(bSuccess); // we have a duplicate!
        }
        return stStringToIndexMap;
    }

}


StringTableMode strTableGetMode(StringTableImp* table)
{
    StringTableMode mode = 0;

    if(table->indexable)
        mode |= Indexable;

    if(table->wideString)
        mode |= WideString;

//     if(table->stringStash)
//         mode |= NoRedundance;

    return mode;
}

/* Function strTableSetMode
 *    Sets the operation mode of the string table.
 *
 *    Returns:
 *        0 - mode set failed
 *        1 - mode set successfully
 */
int strTableSetMode(StringTableImp* table, StringTableMode mode)
{
    assert(!table->readOnly);

    if ( 
        table->pFirstMemChunk 
        && ( table->pFirstMemChunk->uiCurrentIndex > 0 || table->pFirstMemChunk->pNextChunk )
        )
    {
        assert("Can't call strTableSetMode() on a table that has strings in it" == 0 );
        return 0;
    }

    if( Indexable & mode )
    {
        table->indexable = 1;
    }

    if( WideString & mode )
    {
        table->wideString = 1;
    }

//     if( NoRedundance & mode )
//     {
//         table->stringStash = stashTableCreateWithStringKeys(128, table->wideString ? StashWideString : StashDefault);
//     }

    // FIXME!!!
    // Always saying the mode was set successfully is not correct.
    return 1;
}

void strTableLock(StringTableImp* table)
{
    table->readOnly = 1;
}

// We can't remove strings, but we can record how many we would have removed and track how much memory is wasted because of it.
void strTableLogRemovalRequest(StringTable table, const char* pcStringToRemove)
{
#ifdef STRING_TABLE_TRACK
    size_t uiMemLeaked = ( strlen(pcStringToRemove) + 1 ) * sizeof(char); // init it with the size of the string leaked
    if ( table->indexable )
        uiMemLeaked += sizeof(void*); // also, add the index

    table->uiMemLeaked += uiMemLeaked;
#endif
}


/* Function strTableForEachString
*    Invokes the given processor function for each of the string held in the string
*    table.  This function is provided so that it is possible to perform some operation
*    using all the strings stored in the string table even if the table is not indexable.
*
*    Expected StringProcessor return values:
*        0 - stop examining any more strings
*        1 - continue examining strings
*
*/
void strTableForEachString(StringTableImp* table, StringProcessor processor)
{
    U32 uiIndex;
    U32 uiTotalStrings = strTableGetStringCount(table);
    assert( table->indexable && !table->pSharedHeapHandle ); // need an indextable to do this, for now

    for (uiIndex=0; uiIndex < uiTotalStrings; ++uiIndex)
    {
        if (!processor( strTableGetString(table, uiIndex) ))
            return;
    }
}

/* Function WriteStringTable
*/
int WriteStringTable(SimpleBufHandle file, StringTableImp* table)
{
    int written = 0;
    U32 uiIndex, uiStringCount, uiStringBytes;

    assert(table->indexable); // this doesn't make sense if not indexable
    uiStringCount = strTableGetStringCount(table);
    uiStringBytes = strTableStringMemUsage(table);

    // FIXME: wide strings? endian-ness?
    SimpleBufGrow(file, uiStringBytes + 4 + 4); // less resizing
    written += SimpleBufWriteU32(uiStringCount, file);
    written += SimpleBufWriteU32(uiStringBytes, file);
    for(uiIndex=0; uiIndex < uiStringCount; ++uiIndex)
    {
        const char *str = strTableGetConstString(table,uiIndex);
        written += SimpleBufWrite(str, (int)strlen(str) + 1, file);
    }

    return written;
}

/* Function ReadStringTable
*/
StringTableImp* ReadStringTable(SimpleBufHandle file)
{
    StringTableImp* table;
    MemChunk *pMemChunk;
    U32 uiIndex;
    U32 uiStringCount;
    U32 uiStringBytes;

    // find out how much space is needed
    if( SimpleBufReadU32(&uiStringCount, file) != 4 ||
        SimpleBufReadU32(&uiStringBytes, file) != 4 )
    {
        // read error!
        return NULL;
    };

    // allocate a MemChunk and read in all the strings
    pMemChunk = calloc(1, sizeof(MemChunk));
    pMemChunk->uiSize = uiStringBytes;
    pMemChunk->pcData = malloc(uiStringBytes);
    if(SimpleBufRead(pMemChunk->pcData, uiStringBytes, file) != (int)uiStringBytes)
    {
        // read error!
        free(pMemChunk->pcData);
        free(pMemChunk);
        return NULL;
    }

    // create the table and reserve index space
    table = createStringTable();
    strTableSetMode(table,Indexable); // FIXME: wide strings?
#ifdef STRING_TABLE_TRACK
    trackTable(table,__FILE__,__LINE__);
#endif
    table->pFirstMemChunk = pMemChunk;
    initArray(&table->indexTable,uiStringCount);

    // index the strings
    for(uiIndex = 0; uiIndex < uiStringCount; ++uiIndex)
    {
        char *str = pMemChunk->pcData + pMemChunk->uiCurrentIndex;
        U32 uiBytesLeft = (U32)(pMemChunk->uiSize - pMemChunk->uiCurrentIndex);
        size_t uiStrBytes = strnlen(str,uiBytesLeft) + 1;

        if(uiStrBytes > uiBytesLeft)
        {
            // we read off the end of the chunk!
            destroyStringTable(table);
            return NULL;
        }

        arrayPushBack(&table->indexTable,str);
        pMemChunk->uiCurrentIndex += uiStrBytes;
    }
    if(pMemChunk->uiCurrentIndex < pMemChunk->uiSize)
    {
        // we didn't read the entire chunk!
        destroyStringTable(table);
        return NULL;
    }

    return table;
}





#ifdef STRING_TABLE_TRACK
typedef struct STTrackerInfo
{
    char cName[128];
    int iNumStringTables;
    size_t uiTotalSize;
    size_t uiWastage;
    size_t uiLeakage;
} STTrackerInfo;

FILE* stringDumpFile;

static int stringTrackerProcessor(StashElement a)
{
    STTrackerInfo* pInfo = stashElementGetPointer(a);
    printf("%.1f KB\t%.1f KB\t%.1f KB\t%d tables\t%s\n", ((float)pInfo->uiTotalSize) / 1024.0f, ((float)pInfo->uiWastage) / 1024.0f, ((float)pInfo->uiLeakage) / 1024.0f, pInfo->iNumStringTables, pInfo->cName );
    fprintf(stringDumpFile, "%Id,%Id,%Id,%d,%s\n", pInfo->uiTotalSize, pInfo->uiWastage, pInfo->uiLeakage, pInfo->iNumStringTables, pInfo->cName );
    return 1;
}

void printStringTableMemUsage()
{
    StringTableImp* pList = stringTableList;
    StashTable pStringTableHashTable = stashTableCreateWithStringKeys( 64, StashDeepCopyKeys | StashCaseSensitive ); // whoo
    size_t iTotalOver64k = 0;
    size_t iTotalUnder64k = 0;

    stringDumpFile = NULL;

    while (!stringDumpFile)
    {
        stringDumpFile = fopen( "c:/stringdump.csv", "wt" );
        if (!stringDumpFile)
        {
            printf("Let go of c:/stringdump.csv, please!\n");
            Sleep(1000);
        }
    }
    fprintf(stringDumpFile, "Size,Wastage,Leaked,NumInstances,Name\n");

    printf("String Table Mem Usage\n");
    printf("----------------------\n");

    while (pList)
    {
        STTrackerInfo* pInfo = (STTrackerInfo*)stashFindPointerReturnPointer(pStringTableHashTable, pList->cName);
        size_t iMemUsage = strTableMemUsage(pList);

        if ( !pInfo )
        {
            pInfo = calloc(1, sizeof(STTrackerInfo));
            strcpy(pInfo->cName, pList->cName);
            stashAddPointer(pStringTableHashTable, pList->cName, pInfo, false);
        }

        assert(pInfo);
        pInfo->iNumStringTables++;
        pInfo->uiTotalSize += iMemUsage;
        pInfo->uiWastage += strTableWastage(pList);
        pInfo->uiLeakage += pList->uiMemLeaked;

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
    stashForEachElement(pStringTableHashTable, stringTrackerProcessor);

    printf("----------------------\n");
    printf("Under64k = %Id        Over64k = %Id        Total = %Id\n", iTotalUnder64k, iTotalOver64k, iTotalOver64k + iTotalUnder64k);
    printf("----------------------\n");

    stashTableDestroy(pStringTableHashTable);
    fclose(stringDumpFile);
}
#else
void printStringTableMemUsage()
{
    printf("ERROR: Please #define STRING_TABLE_TRACK in stringtable.c for string table tracking information!\n");
}
#endif


#ifdef STRING_TABLE_TRACK
int printStringToFile( char* string )
{
    assert( stringDumpFile);
    fprintf(stringDumpFile, "%s\n", string);

    return 1;
}

void printStringTable(StringTableImp* table)
{
    stringDumpFile = NULL;

    while (!stringDumpFile)
    {
        stringDumpFile = fopen( "c:/stringtable.txt", "wt" );
        if (!stringDumpFile)
        {
            printf("Let go of c:/stringtable.txt, please!\n");
            Sleep(1000);
        }
    }
    //fprintf(stringDumpFile, "Size,Wastage,NumInstances,Name\n");

    strTableForEachString(table, printStringToFile);

    fclose(stringDumpFile);

}
#endif

/*
 * Begin StringTable Test related functions
 ********************************************************************/
#pragma warning(pop)
