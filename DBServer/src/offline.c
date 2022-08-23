#include "offline.h"
#include "container.h"
#include <utilitieslib/utils/file.h>
#include "comm_backend.h"
#include <utilitieslib/utils/utils.h>
#include "clientcomm.h"
#include "sql_fifo.h"
#include "sql_fifo.h"
#include <zlib/zlib.h>
#include "namecache.h"
#include <utilitieslib/utils/error.h>
#include "dbserver/servercfg.h"
#include <utilitieslib/utils/timing.h>
#include "container_merge.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/HashFunctions.h>
#include <utilitieslib/components/StashTable.h>
#include "dbdispatch.h"
#include <utilitieslib/utils/log.h>

#define MAX_DATA_FILE    (256 * (1<<20))

// Initial size of the sOfflinePlayerData list (# of auth Id's to make space for, initially)
#define kOfflinePlayerDataInitialSize    128

// Maximum number of deleted character index files we will keep loaded
// before dumping the least recently used.
#define kMinDeletedFileIndexesOpen    1
#define kMaxDeletedFileIndexesOpen    4

// Used as an array index, so keep this zero-based!
typedef enum {
    kStorageClass_OfflineCharacters=0,
    kStorageClass_DeletedCharacters,
    //-------------------------
    kStorageClass_COUNT
} tStorageClass;

#define OFFLINE_VALID_STORAGE_CLASS(_aVal_)    \
    (((_aVal_)==kStorageClass_OfflineCharacters) || ((_aVal_)==kStorageClass_DeletedCharacters))

// A key for finding a storage file (not an index).
typedef U32 tDataFileKey;

// A temporary holder for passing around these related values
typedef struct tAuthSpec {
    int                            auth_id;
    char*                        auth_name_p;
} tAuthSpec;

// Spec for finding data stored in disk files
typedef struct {
    tStorageClass                storageClass;
    tDataFileKey                dataFileKey;
} tDataFileSpec;

// A set of offlined or deleted players for an auth_id
typedef struct OfflinePlayerArray {
    int                            arraySize;        // space available
    int                            numItems;        // number actually in use
    OfflinePlayer**                data;            // managed using the dynArrayxxx functions
} OfflinePlayerArray;

// Index is auth_id; stored data is OfflinePlayerArray ptrs.
typedef StashTable tOfflinePlayerArrayCache;

// Index is auth_name; value stored is auth_id
typedef StashTable tAuthNameTable;

// Info about a particular data file
typedef struct {
    tDataFileKey                id;                        // meaning of this number depends on the tStorageClass.
    FILE*                        filePtr;
} tOpenFileInfo;

// An index used to locate the specifics needed to get or put data from/to a data file
typedef struct {
    FILE*                        indexFilePtr;            // notice that an index file can store info about multiple data files
    U32                            lastAccess;                // for release old storage indexes
    tOfflinePlayerArrayCache    playerArrayCache;        // cached index info
    tAuthNameTable                authNameTable;            // contains a name-to-ID mapping if the index data record provides them.
    tOpenFileInfo**                openFileInfo;            // a dynArray of file pointer info
    tOpenFileInfo*                activeFileInfo;         // the file we're currently appendong to
    int                            openFileInfoCount;        // the highest data file we have a pointer to
    int                            openFileInfoArrSz;        // may include some unused elements
    int                            numFilesOpen;            // we may want to limit the number open at one time
    int                            lastTempDbId;            // for internal use we may need to generate temp ID's.
} tStorageIndex;    

// The two storage classes manage data a little differently. 
//        - Offline character data will have 1 tStorageIndex containing multiple tOpenFileInfo objects.
//        - Deleted character data will have >= 0 tStorageIndex objects, each of which contains 1 tOpenFileInfo object.
typedef struct {
    int                            storageIndexCount;        // number of index files we know about
    int                            storageIndexArrSz;        // may include some unused elements
    tStorageIndex**                storageIndexArr;        // a dynArray of storage data structs
    tStorageIndex*                activeStorageIndex;        // the index we're prepared to append to.
} tOfflineDataStore;

// The data
static tOfflineDataStore sDataFileStore[kStorageClass_COUNT] = { 0 };

typedef enum {
    kIndexTextFld_fileNbr = 0,
    kIndexTextFld_filePos,
    kIndexTextFld_auth_id,
    kIndexTextFld_db_id,
    kIndexTextFld_characterName,
    kIndexTextFld_level,
    kIndexTextFld_origin,
    kIndexTextFld_archetype,
    kIndexTextFld_storageStatus,
    kIndexTextFld_dateStored,
    kIndexTextFld_AuthName,
    //------------------------------
    kIndexTextFld_COUNT
} tIndexTextFlds;

// backward compatibility...
#define kVersion1IndexFlds    8
#define kCurrentIndexFlds    kIndexTextFld_COUNT
#define VALID_INDEX_REC_FLD_COUNT(_nFldCount_)    (((_nFldCount_)==kVersion1IndexFlds)||((_nFldCount_)==kCurrentIndexFlds))

static void                  storageIndexLoadFromDisk( tDataFileSpec* spec );
static void                    storageIndexRelease( tOfflineDataStore* pStore, int nIndexOfs );
static char*                offlineDir( void );
static char*                indexFname(tDataFileSpec* fileSpec );
static char*                dataFname(tDataFileSpec* fileSpec );
static bool                   addPlayerToOfflineStore(int id, tOfflineObjStatus status, 
                                    tDataFileSpec* fileSpec,
                                    bool bUpdateIdxCache, bool bUnloadContainer);
static bool                   copyEntityContainerToStorage(int id, tOfflineObjStatus status, 
                                    tDataFileSpec* fileSpec,
                                    EntCon** ppEnt /* OUT (NULL is OK), if *ppEnd != NULL, we use that container, otherwise, we load *ppEnt for id */,
                                    OfflinePlayer* pNewPlayerData /* OUT (NULL is NOT-OK) */ );
static bool                    writePlayerDataToDataFiles(tStorageClass storageClass, OfflinePlayer* pNewPlayerData, const char* szRawData, tOfflineObjStatus status );
static void                  unloadPlayerData( int id );
static int                   restoreFromOfflineStorage( tStorageClass storageClass, OfflinePlayer *offlinePlayer, const tAuthSpec* authSpec );
static void                    prepareOpenFileInfo( tDataFileSpec* fileSpec, tStorageIndex** pIndex, int* pFileInfoOfs );
static tDataFileKey            offlinedPlayers_InitActiveFileInfo( void );
static FILE**                getIndexFilePtrVar( tDataFileSpec* fileSpec );
static FILE*                 prepareIndexFile( tDataFileSpec* fileSpec, long offset /*-1 means "close it*/, int origin /*e.g., SEEK_SET, SEEK_END...*/ );
static FILE*                 prepareDataFile( tDataFileSpec* fileSpec, long offset /*-1 means "close it"*/, int origin /*e.g., SEEK_SET, SEEK_END...*/ );
static bool                  patchIndexRecord( tDataFileSpec* fileSpec, OfflinePlayer* p, tOfflineObjStatus status );
static void                    openDataAppendFile( tStorageClass storageClass, int* p_file_size, FILE** p_file_ptr, tDataFileKey* p_data_fileKey );
static bool                    readIndexStringRec( FILE* dataFile, 
                                char* buffer, size_t buffSz, int* fldOffsets /*OUT*/, int nFldOffsetsSz,
                                size_t* p_string_size /*OUT*/, int* p_num_fields /*OUT*/ );
static U32                    fldValueFromLineList( ContainerTemplate *tplt, LineList* lineList, const char* fldName, U32 defaultVal );
static void                    fldStrFromLineList( ContainerTemplate *tplt, LineList* lineList, const char* fldName, 
                                char* buffer, size_t strSz );
static void                    deletedPlayers_InitDataFileSpec( tDataFileSpec* fileSpec, const char* dataMMYYYY /* if NULL, current month is used */ );
static void                    logTimeToTimeStruct( const char* szLogTime, struct tm* pTime );

static char*                storedData_GetPlayer(tStorageClass storageClass, OfflinePlayer *p);
static void                    storedData_CreateIndexCache( tDataFileSpec* fileSpec );
static void                    storedData_DestroyIndexCache( tStorageIndex* pIndex );
static void                    storedData_FreePlayerArray(OfflinePlayerArray** ppList);
static void                    storedData_AddToPlayerArray( tDataFileSpec* fileSpec, OfflinePlayer* p);

// Various levels of searches within the index structures
static OfflinePlayerArray*    storedData_PlayerArrayFromAuthId( tDataFileSpec* fileSpec, const tAuthSpec* authSpec );
static int                    storedData_PlayerArrayOfsFromDbId(OfflinePlayerArray* arr, int db_id);        // return an array index or -1
static bool                    storedData_PlayerArrayAndOfsFromAuthAndId(tDataFileSpec* fileSpec, const tAuthSpec* authSpec, int db_id, OfflinePlayerArray** ppList /*NULL is OK*/, int* pIndex /*NULL is OK*/);
static int                    storedData_PlayerArrayOfsFromName(OfflinePlayerArray* arr, tOfflineObjStatus objStatus, char* charName, int seqNbr);    // return an array index or -1
static bool                    storedData_PlayerArrayAndOfsFromAuthAndName(tDataFileSpec* fileSpec, tOfflineObjStatus objStatus, const tAuthSpec* authSpec, char* charName, int seqNbr, OfflinePlayerArray** ppList, int* pIndex);
static OfflinePlayer*        storedData_StoredPlayerFromAuthAndName(tDataFileSpec* fileSpec, tOfflineObjStatus objStatus, const tAuthSpec* authSpec, char* charName, int seqNbr);
static OfflinePlayer*        storedData_StoredPlayerFromAuthAndId(tDataFileSpec* fileSpec, const tAuthSpec* authSpec, int db_id);

void offlineInitOnce( void )
{
    // Prepare the initial offline player data structs
    tDataFileSpec offLineSpec = { 0 };
    tDataFileSpec deletedSpec = { 0 };
    
    offLineSpec.storageClass = kStorageClass_OfflineCharacters;
    prepareOpenFileInfo( &offLineSpec, NULL, NULL );
    
    // Maintenance....
    offlineUnusedPlayers();

    deletedPlayers_InitDataFileSpec( &deletedSpec, NULL );
    prepareOpenFileInfo( &deletedSpec, NULL, NULL );

    // Offline data and the current month's deleted character data are pre-loaded.
    storageIndexLoadFromDisk( &offLineSpec );
    storageIndexLoadFromDisk( &deletedSpec );
}

char *offlineDir()
{
    char    *s;
    static char offline_dir[MAX_PATH] = { 0 };
    
    if ( offline_dir[0] == '\0' )    // just do this work once
    {
        strcpy(offline_dir,fileDataDir());
        s = strrchr(offline_dir,'/');
        if (s)
            *s = 0;
        strcat(offline_dir,"/db_offline/");
        mkdirtree(offline_dir);
        s = strrchr(offline_dir,'/');
        if (s)
            *s = 0;
    }
    return offline_dir;
}

char *indexFname( tDataFileSpec* fileSpec )
{
    static tDataFileSpec sDataFileSpec = { ~0, ~0 };
    static char sIndexFilePath[MAX_PATH] = { 0 };
    
    if (( fileSpec->storageClass != sDataFileSpec.storageClass ) ||
        ( fileSpec->dataFileKey  != sDataFileSpec.dataFileKey ))
    {
        if ( fileSpec->storageClass == kStorageClass_OfflineCharacters )
        {
            sprintf_s(sIndexFilePath,ARRAY_SIZE(sIndexFilePath),
                        "%s/players.idx", offlineDir(), 
                        fileSpec->dataFileKey);
        }
        else if ( fileSpec->storageClass == kStorageClass_DeletedCharacters )
        {
            sprintf_s(sIndexFilePath,ARRAY_SIZE(sIndexFilePath),
                        "%s/deletedPlayers_%d.idx", offlineDir(), 
                        fileSpec->dataFileKey);
        }
        sDataFileSpec.storageClass = fileSpec->storageClass;
        sDataFileSpec.dataFileKey  = fileSpec->dataFileKey;
    }
    return sIndexFilePath;
}

char *dataFname(tDataFileSpec* fileSpec)
{
    static tDataFileSpec sDataFileSpec = { ~0, ~0 };
    static char sDataFilePath[MAX_PATH] = { 0 };
    
    if (( fileSpec->storageClass != sDataFileSpec.storageClass ) ||
        ( fileSpec->dataFileKey     != sDataFileSpec.dataFileKey ))
    {
        if ( fileSpec->storageClass == kStorageClass_OfflineCharacters )
        {
            sprintf_s(sDataFilePath,ARRAY_SIZE(sDataFilePath),
                        "%s/players_%d.data", offlineDir(),
                        fileSpec->dataFileKey);
        }
        else if ( fileSpec->storageClass == kStorageClass_DeletedCharacters )
        {
            sprintf_s(sDataFilePath,ARRAY_SIZE(sDataFilePath),
                        "%s/deletedPlayers_%d.data", offlineDir(),
                        fileSpec->dataFileKey);
        }

        sDataFileSpec.storageClass    = fileSpec->storageClass;
        sDataFileSpec.dataFileKey    = fileSpec->dataFileKey;
    }
    return sDataFilePath;
}

FILE** getIndexFilePtrVar( tDataFileSpec* fileSpec )
{
    tStorageIndex* pIndex = NULL;
    prepareOpenFileInfo( fileSpec, &pIndex, NULL );
    return &(pIndex->indexFilePtr);
}

FILE* prepareIndexFile( tDataFileSpec* fileSpec, long offset, int origin /*e.g., SEEK_SET, SEEK_END...*/ )
{
    FILE** ppFile = getIndexFilePtrVar( fileSpec );

    if ( offset == -1 )
    {
        if ( *ppFile == NULL )
        {
            fclose( *ppFile );
            *ppFile = NULL;
        }
        return NULL;
    }

    if ( *ppFile == NULL )
    {
        if ( ! fileExists(indexFname(fileSpec)) )
        {
            *ppFile = fopen(indexFname(fileSpec),"w+b");    // Create the file
        }
        else
        {
            *ppFile = fopen(indexFname(fileSpec),"r+b");    // Opens for both reading and writing. (The file must exist.)
        }
    }
    if ( *ppFile )
    {
        fseek( *ppFile, offset, origin );
    }
    return *ppFile;
}

void deletedPlayers_InitDataFileSpec( tDataFileSpec* fileSpec, 
                                const char* dateMMYYYY /* or NULL for the current month */ )
{
    fileSpec->storageClass = kStorageClass_DeletedCharacters;

    if ( dateMMYYYY != NULL )
    {
        //    format: "MM/YYYY" or "MM-YYYY"
        //
        //    convert to YYYYMM
        devassert(( strlen(dateMMYYYY) == 7 ) &&
                    (( dateMMYYYY[2] == '/' ) || ( dateMMYYYY[2] == '-' )));
        fileSpec->dataFileKey = (
                (( dateMMYYYY[3] - '0' ) * 100000 ) +
                (( dateMMYYYY[4] - '0' ) * 10000 ) +
                (( dateMMYYYY[5] - '0' ) * 1000 ) +
                (( dateMMYYYY[6] - '0' ) * 100 ) +
                (( dateMMYYYY[0] - '0' ) * 10 ) +
                (( dateMMYYYY[1] - '0' ) * 1 )
            );        
    }
    else
    {
        // We append to the current month's file.
        struct tm theTime;
        U32 yy, mm;
        timerMakeTimeStructFromSecondsSince2000(timerSecondsSince2000(),&theTime);
        yy = ( 1900 + ( theTime.tm_year ));
        mm = ( theTime.tm_mon + 1 );
        fileSpec->dataFileKey = (( yy * 100 ) + mm );    // e.g., 200903 for March, 2009
    }
}

void prepareOpenFileInfo( tDataFileSpec* fileSpec, tStorageIndex** ppIndex, int* pFileInfoOfs )
{
    //    Called a lot, so keep this logic brief!
    //
    //    This function gets -- and as a side effect, creates -- the data structures
    //    related to fileSpec.
    tOfflineDataStore* pStore = &sDataFileStore[fileSpec->storageClass];
    tStorageIndex* pIndex = NULL;
    int indexDataOfs = 0;    // to locate the tStorageIndex
    int fileDataOfs = 0;    // to locate a file managed by the tStorageIndex
    
    if ( fileSpec->storageClass == kStorageClass_OfflineCharacters )
    {
        // The "key" is a 1-based file number.
        if ( fileSpec->dataFileKey == 0 )
        {
            fileSpec->dataFileKey = 1;    // if not specified, we use the first valid key value = 1.
        }
        indexDataOfs = 0;    // offline data has one index file
        fileDataOfs = ( fileSpec->dataFileKey - 1 );    // to zero based.
    }
    else if ( fileSpec->storageClass == kStorageClass_DeletedCharacters )
    {
        // The "key" is a YYYYMM number; e.g.,  "200907" for July, 2009.
        // We have to search for it.
        int i;
        indexDataOfs = -1;    // deleted player data has multiple index structures, so we have to search for the right one.
        fileDataOfs = 0;    // each index has one data file, so always zero for any index.
        for ( i=0; i < pStore->storageIndexCount; i++ )
        {
            if ( pStore->storageIndexArr[i]->openFileInfo[0]->id == fileSpec->dataFileKey )
            {
                indexDataOfs = i;
                break;
            }
        }
        if ( indexDataOfs == -1 )
        {
            if ( pStore->storageIndexCount == kMaxDeletedFileIndexesOpen )
            {
                // We can't open any more, so dump the oldest index. As a starting
                // assumption, use the first storage index beyond the minimum.
                int nOldest = kMinDeletedFileIndexesOpen;
                U32 nOldestAccess = pStore->storageIndexArr[nOldest]->lastAccess;
                int i;
                for ( i=kMinDeletedFileIndexesOpen; i < pStore->storageIndexCount; i++ )
                {
                    if ( pStore->storageIndexArr[i]->lastAccess < nOldestAccess )
                    {
                        nOldest            = i;
                        nOldestAccess    = pStore->storageIndexArr[i]->lastAccess;
                    }
                }
                storageIndexRelease( pStore, nOldest ); 
                devassert(pStore->storageIndexCount < kMaxDeletedFileIndexesOpen);
            }
            indexDataOfs = pStore->storageIndexCount;
        }
    }
    
    // Make room to store a new tStorageIndex item, and make the item (if needed)
    dynArrayFit((void **)&pStore->storageIndexArr,sizeof(pStore->storageIndexArr[0]),&pStore->storageIndexArrSz,indexDataOfs);
    pStore->storageIndexCount = ( indexDataOfs >= pStore->storageIndexCount ) ? (indexDataOfs+1) : pStore->storageIndexCount;
    if ( pStore->storageIndexArr[indexDataOfs] == NULL )
    {
        pStore->storageIndexArr[indexDataOfs] = (tStorageIndex*)calloc(1,sizeof(tStorageIndex));
    }
    pIndex = pStore->storageIndexArr[indexDataOfs];
    
    // Create the player array cache stash table
    if ( pIndex->playerArrayCache == NULL )
    {
        pIndex->playerArrayCache = stashTableCreateInt(kOfflinePlayerDataInitialSize);
    }
    if ( pIndex->authNameTable == NULL )
    {
        pIndex->authNameTable =  stashTableCreateWithStringKeys(kOfflinePlayerDataInitialSize, StashDeepCopyKeys);
    }
    
    // Make room to store a new tOpenFileInfo and make the item (if needed)
    dynArrayFit((void **)&pIndex->openFileInfo,sizeof(pIndex->openFileInfo[0]),&pIndex->openFileInfoArrSz,fileDataOfs);
    pIndex->openFileInfoCount = ( fileDataOfs >= pIndex->openFileInfoCount ) ? (fileDataOfs+1) : pIndex->openFileInfoCount;
    if ( pIndex->openFileInfo[fileDataOfs] == NULL )
    {
        pIndex->openFileInfo[fileDataOfs] = (tOpenFileInfo*)calloc(1,sizeof(tOpenFileInfo));
        pIndex->openFileInfo[fileDataOfs]->id = fileSpec->dataFileKey;
    }
    pStore->storageIndexArr[indexDataOfs]->lastAccess = timerSecondsSince2000();
    
    if ( ppIndex )        *ppIndex        = pStore->storageIndexArr[indexDataOfs];
    if ( pFileInfoOfs )    *pFileInfoOfs    = fileDataOfs;
}

tDataFileKey offlinedPlayers_InitActiveFileInfo( void )
{
    tDataFileKey fileKey;
    tStorageIndex* pIndex;
    tDataFileSpec spec = { 0 };
    spec.storageClass = kStorageClass_OfflineCharacters;
    prepareOpenFileInfo( &spec, &pIndex, NULL );

    if ( pIndex->activeFileInfo == NULL )
    {
        tDataFileKey highKey;
        fileKey = 1;
        for ( highKey=1;;highKey++ )
        {
            spec.dataFileKey = highKey;
            if (!fileExists(dataFname(&spec)))
                break;
            fileKey = highKey;
        }
        
        spec.dataFileKey = fileKey;
        prepareOpenFileInfo( &spec, NULL, NULL );    // make sure data structs are frosty
        pIndex->activeFileInfo = pIndex->openFileInfo[fileKey-1];    // fileKey is a 1-based index
    }
    else
    {
        fileKey = pIndex->activeFileInfo->id;
    }
    return fileKey;
}

FILE* prepareDataFile( tDataFileSpec* fileSpec, long offset, int origin /*e.g., SEEK_SET, SEEK_END...*/ )
{
    tStorageIndex* pIndex = NULL;
    tOpenFileInfo* openFileInfo = NULL;
    int fileInfoOfs = 0;
    
    prepareOpenFileInfo(fileSpec,&pIndex,&fileInfoOfs);
    openFileInfo = pIndex->openFileInfo[fileInfoOfs];
    
    // offset -1 means, "close the file"
    if ( offset == -1 )
    {
        // close the file
        if ( openFileInfo->filePtr != NULL )
        {
            fclose( openFileInfo->filePtr );
            openFileInfo->filePtr = NULL;
            pIndex->numFilesOpen--;
        }
        return NULL;
    }

    // we want to open the file    
    if (!openFileInfo->filePtr)
    {
        openFileInfo->filePtr = fopen(dataFname(fileSpec),"r+b");
        if ( ! openFileInfo->filePtr )
        {
            openFileInfo->filePtr = fopen(dataFname(fileSpec),"w+b");
        }
        pIndex->numFilesOpen++;
    }
    pIndex->lastAccess = timerSecondsSince2000();
    if( openFileInfo->filePtr )
        fseek( openFileInfo->filePtr, offset, origin );
    return openFileInfo->filePtr;
}

bool patchIndexRecord( tDataFileSpec* fileSpec, OfflinePlayer* p, tOfflineObjStatus status )
{
    bool success = true;
    size_t recSize;
    char* fldPtr;
    char indexEntry[512];
    char fldBuffer[16];
    size_t oldFldSz;
    size_t newFldSz;
    FILE *indexFile;
    int patchSpecI;
    int fldCount;
    int fldOffsets[kCurrentIndexFlds+1] = { 0 };
    int currTime = (int)timerSecondsSince2000();
    int dataFileNbr;
    
    typedef struct tPatchSpec {
        int fldNbr;        // 0-based
        int* sourceData;
        const char* szPrintFmtStr;
    } tPatchSpec;

    tPatchSpec specs[] = {
        { kIndexTextFld_fileNbr,        &dataFileNbr,    "%u"    },
        { kIndexTextFld_storageStatus,    (int*)&status,    "%u"    },
        { kIndexTextFld_dateStored,        &currTime,        "%-12u"    },
    };
    
    dataFileNbr = ( fileSpec->storageClass == kStorageClass_OfflineCharacters ) ? fileSpec->dataFileKey : 0;
    
    
    // Get a copy of the index record
    indexFile = prepareIndexFile( fileSpec, p->idxfile_pos, SEEK_SET );
    if (ftell(indexFile) != p->idxfile_pos)
    {
        return false;
    }
    if (( ! readIndexStringRec( indexFile, 
            indexEntry, ARRAY_SIZE(indexEntry), 
            fldOffsets, kCurrentIndexFlds, &recSize, &fldCount )) ||
        ( ! VALID_INDEX_REC_FLD_COUNT(fldCount) ))
    {
        return false;
    }
    
    // The last entry in fldOffsets is the offset of the NULL terminator or newline
    // at the end. Allows us to measure the field length of the last item in the
    // same way as other items: by subtract the current field offset from the next one.
    // In the case of the last item, the "next" is a fake offset.
    fldOffsets[fldCount] = (int)recSize;
    
    for (patchSpecI=0; patchSpecI < ARRAY_SIZE(specs); patchSpecI++ )
    {
        const char* szPrintFmt = "%u";
        tPatchSpec* pSpec = &specs[patchSpecI];
        if ( pSpec->fldNbr >= fldCount )
        {
            continue;
        }
        sprintf_s( fldBuffer, ARRAY_SIZE(fldBuffer), pSpec->szPrintFmtStr, *(pSpec->sourceData) );
        
        // make sure we don't clobber the space separated values...
        fldPtr = ( indexEntry + fldOffsets[pSpec->fldNbr] );
        newFldSz = strlen(fldBuffer);
        oldFldSz = ( fldOffsets[pSpec->fldNbr+1] - fldOffsets[pSpec->fldNbr] );
        if ( newFldSz <= oldFldSz )
        {
            memcpy( fldPtr, fldBuffer, newFldSz );
            while ( newFldSz < oldFldSz )
            {
                // pad with spaces to retain the old field size
                fldPtr[newFldSz++] = ' ';
            }
        }
        else
        {
            // Old record is probably trashed or we are looking at the wrong location?
            success = false;
        }
    }
    
    // Now that we have patch our copy, write it (cautiously) to the index file.
    fseek( indexFile, p->idxfile_pos, SEEK_SET );
    if (ftell(indexFile) != p->idxfile_pos)
    {
        return false;
    }
    fwrite( indexEntry, recSize, 1, indexFile );
    fflush( indexFile );
    
    return success;
}

void releaseAllDataFilePtrs( void )
{
    int storageTypeI;
    for ( storageTypeI=0; storageTypeI < kStorageClass_COUNT; storageTypeI++ )
    {
        tOfflineDataStore* pStore = &sDataFileStore[storageTypeI];
        while ( pStore->storageIndexCount > 0 )
        {
            storageIndexRelease( pStore, ( pStore->storageIndexCount - 1 ) );
        }
    }
}

void openDataAppendFile( tStorageClass storageClass, int* p_file_size, FILE** p_file_ptr, tDataFileKey* p_data_fileKey )
{
    int fileSz = 0;
    FILE* data_append = NULL;
    tOfflineDataStore* pStore = &sDataFileStore[storageClass];
    tStorageIndex* pIndex = NULL;
    int fileDataOfs = 0;
    tDataFileSpec spec = { 0 };
    
    spec.storageClass    = storageClass;
    spec.dataFileKey    = *p_data_fileKey;
    
    if ( storageClass == kStorageClass_OfflineCharacters )
    {
        // We append to the highest numbered file, using our single index
        spec.dataFileKey = offlinedPlayers_InitActiveFileInfo();
        // force storage to be created...
        prepareOpenFileInfo( &spec, &pIndex, &fileDataOfs );
    }
    else if ( storageClass == kStorageClass_DeletedCharacters )
    {
        // We append to the current month's file.
        if ( spec.dataFileKey == 0 )
        {
            deletedPlayers_InitDataFileSpec(&spec, NULL);
        }
        // force storage to be created...
        prepareOpenFileInfo( &spec, &pIndex, &fileDataOfs );
        pIndex->activeFileInfo = pIndex->openFileInfo[fileDataOfs];
    }
    
    data_append = prepareDataFile(&spec,0,SEEK_END);
    if ( storageClass == kStorageClass_DeletedCharacters )
    {
        fileSz = ftell(data_append);
    }
    else if ( storageClass == kStorageClass_OfflineCharacters )
    {
        // make sure this file has enough room to add, based on our max file size restriction
        while ( data_append != NULL )
        {
            fileSz = ftell(data_append);
            if ( fileSz < MAX_DATA_FILE )
            {
                // there is room for growth in this file
                break;
            }
            // that file was too full.
            spec.dataFileKey++;
            data_append = prepareDataFile(&spec, 0, SEEK_END);
        }
    }
    
    if ( p_file_size )        *p_file_size    = fileSz;
    if ( p_file_ptr )        *p_file_ptr        = data_append;
    if ( p_data_fileKey )    *p_data_fileKey    = spec.dataFileKey;
}

bool readIndexStringRec( FILE* dataFile, 
            char* buffer, size_t buffSz, int* fldOffsets, int nFldOffsetsSz, 
            size_t* p_string_size, int* p_num_fields )
{
    size_t str_size = 0;
    int fld_size = 0;
    int fld_count = 0;
    int numSequentialSpaces = 0;
    bool in_quotes = false;
    bool is_blank = false;
    bool was_blank = true;    // needs to be true as an initial state
    
    // like fgets(), but for binary mode files. also counts fields...
    if ( dataFile != NULL )
    {
        while ( str_size < ( buffSz - 1 ))
        {
            char aChar = fgetc( dataFile );
            if (( aChar == EOF ) ||
                ( aChar == '\n' ) ||
                ( aChar == '\r' ) ||
                ( ! isprint( aChar ) ))
            {
                break;
            }
            if ( aChar == '\"' )
            {
                in_quotes = ( ! in_quotes );
            }
            is_blank = (( ! in_quotes ) && ( aChar == ' ' ));
            if ( ! is_blank )
            {
                if ( was_blank )
                {
                    // starting a new field
                    if ( nFldOffsetsSz > 0 )
                    {
                        // Record the starting location as the location
                        // after the first space. (There may be more than one
                        // space separating the fields.)
                        if ( nFldOffsetsSz > fld_count )
                        {
                            fldOffsets[fld_count] = (int)str_size;
                            if ( numSequentialSpaces > 1 )
                            {
                                fldOffsets[fld_count] -= ( numSequentialSpaces - 1 );
                            }
                        }
                    }
                    fld_count++;
                    fld_size = 0;
                }
                fld_size++;
            }
            buffer[str_size++] = aChar;
            was_blank = is_blank;
            numSequentialSpaces = ( was_blank ) ? (numSequentialSpaces+1) : 0;
        }
        buffer[str_size] = '\0';
    }
    
    *p_string_size = str_size;
    *p_num_fields = fld_count;
    
    return ( str_size > 0 );
}

int offlinePlayerMoveOffline(int id)
{
    tDataFileSpec spec = { 0 };
    tStorageIndex* pIndex = NULL;

    offlinedPlayers_InitActiveFileInfo();

    spec.storageClass = kStorageClass_OfflineCharacters;
    prepareOpenFileInfo( &spec, &pIndex, NULL );
    if (( ! pIndex ) || ( pIndex->activeFileInfo == NULL ))
    {
        devassert( pIndex );
        devassert( pIndex->activeFileInfo );
        return 0;
    }
    spec.storageClass    = kStorageClass_OfflineCharacters;
    spec.dataFileKey    = pIndex->activeFileInfo->id;

    return !! addPlayerToOfflineStore(id, kOfflineObjStatus_OFFLINE, &spec, true,true);
}

bool addPlayerToOfflineStore(int id, tOfflineObjStatus status, 
                                    tDataFileSpec* fileSpec,
                                    bool bUpdateIdxCache, bool bUnloadContainer)
{
    EntCon* ent_con = NULL;
    OfflinePlayer player = { 0 };
    if ( copyEntityContainerToStorage( id, status, fileSpec, &ent_con, &player ) )
    {
        if ( bUpdateIdxCache )
        {
            storedData_AddToPlayerArray( fileSpec, &player );
        }
        if (( ent_con ) && ( bUnloadContainer ))
        {
            unloadPlayerData( id );
        }
        return true;
    }
    return false;
}

bool copyEntityContainerToStorage(int id, tOfflineObjStatus status, 
                                    tDataFileSpec* fileSpec,
                                    EntCon** ppEnt, OfflinePlayer* pNewPlayerData )
{
    EntCon* ent_con;
    U32        origin,archetype;
    extern AttributeList *var_attrs;
    
    if (( ppEnt != NULL ) && ( *ppEnt != NULL ))
    {
        // caller passed in an entity container, so use that
        ent_con = *ppEnt;
    }
    else
    {
        // load the container
        ent_con = containerLoad(dbListPtr(CONTAINER_ENTS),id);
        if ( ppEnt )
        {
            *ppEnt = ent_con;
        }
    }

    if (! ent_con )
    {
        return false;
    }
    if (!stashFindInt(var_attrs->hash_table,ent_con->origin,&origin)
        || !stashFindInt(var_attrs->hash_table,ent_con->archetype,&archetype))
    {
        return false;
    }

    pNewPlayerData->auth_id            = ent_con->auth_id;
    strcpy_s(pNewPlayerData->auth_name,ARRAY_SIZE(pNewPlayerData->auth_name),ent_con->account);
    pNewPlayerData->db_id            = ent_con->id;
    pNewPlayerData->storage_status    = status;
    pNewPlayerData->dateStored        = timerSecondsSince2000();
    strcpy( pNewPlayerData->name, ent_con->ent_name );
    pNewPlayerData->origin            = origin;
    pNewPlayerData->archetype        = archetype;
    pNewPlayerData->fileKey            = fileSpec->dataFileKey;
    pNewPlayerData->level            = ent_con->level;
    
    return writePlayerDataToDataFiles(fileSpec->storageClass,pNewPlayerData, containerGetText(ent_con), status );
}

bool writePlayerDataToDataFiles(tStorageClass storageClass, OfflinePlayer* pNewPlayerData, 
                                    const char* szRawData, tOfflineObjStatus status  )
{
    tDataFileSpec spec;
    FILE* data_append = NULL;
    FILE* indexFile = NULL;
    int fileSize = 0;
    int fileNbr = 0;
    
    spec.storageClass    = storageClass;
    spec.dataFileKey    = pNewPlayerData->fileKey;
    
    devassert( OFFLINE_VALID_STORAGE_CLASS(spec.storageClass) );

    openDataAppendFile( storageClass, &fileSize, &data_append, &spec.dataFileKey );
    indexFile = prepareIndexFile(&spec,0,SEEK_END);
    pNewPlayerData->fileKey        = spec.dataFileKey;
    pNewPlayerData->file_pos    = (U32)fileSize;
    pNewPlayerData->idxfile_pos    = ftell(indexFile);
    
    // Write the data
    {
        int        origsize,zipsize;
        char    *zipped;
        origsize = (int)(strlen(szRawData) + 1);
        zipped = zipData(szRawData,origsize,&zipsize);
        fwrite(&origsize,4,1,data_append);
        fwrite(&zipsize,4,1,data_append);
        fwrite(zipped,zipsize,1,data_append);
        fflush(data_append);
        free(zipped);    
    }
    
    // Add an index file entry. 
    // Maintenance note: See patchIndexRecord() if you change this format (which you can't
    // really do without invalidating existing data, but I thought I'd mention it anyway).
    fprintf(indexFile,"%u %u %u %u \"%s\" %d %d %d %d %12u \"%s\"\n",
            pNewPlayerData->fileKey,
            pNewPlayerData->file_pos,
            pNewPlayerData->auth_id,
            pNewPlayerData->db_id,
            pNewPlayerData->name,
            pNewPlayerData->level,
            pNewPlayerData->origin,
            pNewPlayerData->archetype,
            pNewPlayerData->storage_status,
            pNewPlayerData->dateStored,
            pNewPlayerData->auth_name);
    fflush(indexFile);
    
    LOG( LOG_OFFLINE, LOG_LEVEL_IMPORTANT, 0, "%s %u \"%s\"\n", storageClass == kStorageClass_OfflineCharacters ? "Offline" : "Delete", pNewPlayerData->auth_id,pNewPlayerData->name);
        
    return true;
}

void unloadPlayerData( int id )
{
    containerUnload(dbListPtr(CONTAINER_ENTS),id);
    deletePlayer(id,false,0,0,0);
}

void storedData_CreateIndexCache( tDataFileSpec* fileSpec )
{
    prepareOpenFileInfo( fileSpec, NULL, NULL );
}

void storedData_DestroyIndexCache( tStorageIndex* pIndex )
{
    if (pIndex->playerArrayCache || pIndex->authNameTable)
    {
        StashElement elem;
        StashTableIterator iter;
        stashGetIterator(pIndex->playerArrayCache, &iter);
        while ( stashGetNextElement(&iter, &elem))
        {
            OfflinePlayerArray* pList = (OfflinePlayerArray*)stashElementGetPointer(elem);
            storedData_FreePlayerArray(&pList);
            stashElementSetPointer(elem,NULL);    // just in case
        }
        stashTableDestroy(pIndex->playerArrayCache);
        stashTableDestroy(pIndex->authNameTable);
        pIndex->playerArrayCache = NULL;
        pIndex->authNameTable = NULL;
    }
}

OfflinePlayerArray* storedData_PlayerArrayFromAuthId( tDataFileSpec* fileSpec, const tAuthSpec* authSpec )
{
    // Note: Since offline player info uses a single index, file->dataFileKey is
    // irrelevant. For deleted players, it needs to be set to the proper month.
    tStorageIndex* pIndex = NULL;
    int fileInfoIndex = 0;
    int auth_id = authSpec->auth_id;
    OfflinePlayerArray* pList = NULL;
    
    prepareOpenFileInfo( fileSpec, &pIndex, &fileInfoIndex );
    
    if (( auth_id == 0 ) && ( authSpec->auth_name_p != NULL ) && ( authSpec->auth_name_p[0] != '\0' ))
    {
        StashElement elem;
        if ( stashFindElement( pIndex->authNameTable, authSpec->auth_name_p, &elem ) )
        {
            auth_id = stashElementGetInt(elem);
        }
    }        
    if ( stashIntFindPointer(pIndex->playerArrayCache, auth_id, &pList) )
    {
        return pList;
    }
    return NULL;
}

int storedData_PlayerArrayOfsFromDbId(OfflinePlayerArray* arr, int db_id)
{
    //
    // Note: We're search a listing for a specific auth_id, so a sequential
    // search is most likely not insane.
    //
    int arrI;
    for ( arrI=0; arrI < arr->numItems; arrI++ )
    {
        if ( arr->data[arrI]->db_id == db_id )
        {
            return arrI;
        }
    }
    return -1;
}

int storedData_PlayerArrayOfsFromName(OfflinePlayerArray* arr, tOfflineObjStatus objStatus, char* charName, int seqNbr)
{
    //
    // The 1-based sequence number allows the caller to choose a specific item within
    // the index file. If the sequence number is > 0 we find the nth match by name.
    //
    int itemIndex = -1;
    {
        //
        // Sequence number not provide. We're searching a listing for a specific
        // auth_id, so a sequential search is most likely not insanely inefficient.
        //
        int arrI;
        int objMatchCount = 0;
        for ( arrI=0; arrI < arr->numItems; arrI++ )
        {
            if ( arr->data[arrI]->storage_status == objStatus )
            {
                objMatchCount++;
            }
            if ((( objMatchCount == seqNbr ) ||    ( seqNbr <= 0 )) &&
                ( stricmp( arr->data[arrI]->name, charName ) == 0 ))
            {
                itemIndex = arrI;
                break;
            }
        }
    }
    return itemIndex;
}

bool storedData_PlayerArrayAndOfsFromAuthAndId(tDataFileSpec* fileSpec, const tAuthSpec* authSpec, int db_id, OfflinePlayerArray** ppList, int* pIndex)
{
    int charIndex = -1;
    OfflinePlayerArray* pList = NULL;

    // Strategy: find the auth_id via hash table lookup, then add to the
    // the list of db_id's (characters) for this auth_id
    if (( pList = storedData_PlayerArrayFromAuthId( fileSpec, authSpec )) != NULL )
    {
        charIndex = storedData_PlayerArrayOfsFromDbId(pList, db_id);
    }
    if ( ppList ) *ppList = pList;
    if ( pIndex ) *pIndex = charIndex;
    return (( pList != NULL ) && ( charIndex != -1 ));
}

bool storedData_PlayerArrayAndOfsFromAuthAndName(tDataFileSpec* fileSpec, tOfflineObjStatus objStatus, const tAuthSpec* authSpec, char* charName, int seqNbr, OfflinePlayerArray** ppList, int* pIndex)
{
    int charIndex = -1;
    OfflinePlayerArray* pList = NULL;

    // Strategy: find the auth_id via hash table lookup, then add to the
    // the list of db_id's (characters) for this auth_id
    if (( pList = storedData_PlayerArrayFromAuthId( fileSpec, authSpec )) != NULL )
    {
        charIndex = storedData_PlayerArrayOfsFromName(pList, objStatus, charName, seqNbr);
    }
    if ( ppList ) *ppList = pList;
    if ( pIndex ) *pIndex = charIndex;
    return (( pList != NULL ) && ( charIndex != -1 ));
}

OfflinePlayer* storedData_StoredPlayerFromAuthAndId(tDataFileSpec* fileSpec, const tAuthSpec* authSpec, int db_id)
{
    OfflinePlayerArray* pList = NULL;
    int charIndex = -1;
    if ( storedData_PlayerArrayAndOfsFromAuthAndId( fileSpec, authSpec, db_id, &pList, &charIndex ))
    {
        return pList->data[charIndex];
    }
    return NULL;
}

OfflinePlayer* storedData_StoredPlayerFromAuthAndName(tDataFileSpec* fileSpec, tOfflineObjStatus objStatus, const tAuthSpec* authSpec, char* charName, int seqNbr)
{
    OfflinePlayerArray* pList = NULL;
    int charIndex = -1;
    if ( storedData_PlayerArrayAndOfsFromAuthAndName( fileSpec, objStatus, authSpec, charName, seqNbr, &pList, &charIndex ))
    {
        return pList->data[charIndex];
    }
    return NULL;
}

void storedData_FreePlayerArray(OfflinePlayerArray** ppList)
{
    int i;
    for ( i=0; i < (*ppList)->numItems; i++ )
    {
        OfflinePlayer* p = (*ppList)->data[i];
        free( p );
    }
    free( (*ppList)->data );
    free( *ppList );
}

void storedData_AddToPlayerArray( tDataFileSpec* fileSpec, OfflinePlayer* p)
{
    int charIndex = -1;
    OfflinePlayerArray* pList = NULL;
    tOpenFileInfo* fileInfo = NULL;
    tStorageIndex* pIndex = NULL;
    tAuthSpec authSpec = { 0 };
    
    // Strategy: find the auth_id via hash table lookup, then add to the
    // the list of db_id's (characters) for this auth_id
    
    //    First, make sure we have data to search 
    prepareOpenFileInfo( fileSpec, &pIndex, NULL );
    
    // If the player's db_id is zero we need to assign a temporary id.
    // Otherwise, we don't be able to find him by id later on.
    if ( p->db_id == 0 )
    {
        // this is a negative number, to distringuish it from real id's
        p->db_id = --pIndex->lastTempDbId;
    }

    authSpec.auth_id = p->auth_id;
    authSpec.auth_name_p = p->auth_name;
    if (( pList = storedData_PlayerArrayFromAuthId( fileSpec, &authSpec )) == NULL )
    {
        devassert(pIndex->playerArrayCache);
        pList = (OfflinePlayerArray*)calloc( 1, sizeof(OfflinePlayerArray) );
        stashIntAddPointer(pIndex->playerArrayCache,p->auth_id,pList,true);
        if (( p->auth_name != NULL ) && ( p->auth_name[0] != '\0' ))
        {
            stashAddInt(pIndex->authNameTable,p->auth_name,p->auth_id,true);
        }
    }
    charIndex = storedData_PlayerArrayOfsFromDbId(pList,p->db_id);
    if ( charIndex != -1 )
    {
        memcpy(pList->data[charIndex],p,sizeof(OfflinePlayer));
    }
    else
    {
        OfflinePlayer* pNew = (OfflinePlayer*)malloc(sizeof(OfflinePlayer));
        memcpy(pNew,p,sizeof(OfflinePlayer));
        dynArrayAddp(&pList->data, &pList->numItems,&pList->arraySize, pNew);
    }
}

static int dbIdInList(U32 id,U32 *ids,int count)
{
    int        i;

    for(i=0;i<count;i++)
    {
        if (ids[i] == id)
            return 1;
    }
    return 0;
}

int offlinePlayerList(int auth_id,char *auth_name,OfflinePlayer *list,int max_list,U32 *online_ids,int online_count)
{
    int                    fromIdx,toIdx;
    OfflinePlayerArray* pList = NULL;
    OfflinePlayer        *p;
    tDataFileSpec        spec = { 0 };
    tAuthSpec            authSpec = { 0 };
    
    spec.storageClass    = kStorageClass_OfflineCharacters;
    authSpec.auth_id = auth_id;
    authSpec.auth_name_p = auth_name;
    if (( pList = storedData_PlayerArrayFromAuthId( &spec, &authSpec )) == NULL )
    {
        return 0;
    }
    toIdx = 0;
    for (fromIdx=0; fromIdx < pList->numItems && toIdx < max_list; fromIdx++)
    {
        // We may have an entry for a player who is no longer offline, so before sending
        // an offline record, check to make sure p->db_id is not in the list of
        // currently online player ids.
        p = pList->data[fromIdx];
        if (p->name[0] && 
            p->fileKey && 
            p->storage_status == kOfflineObjStatus_OFFLINE && 
            !dbIdInList(p->db_id,online_ids,online_count))
        {
            list[toIdx++] = *p;
        }
    }
    return toIdx;
}

void storageIndexLoadFromDisk( tDataFileSpec* spec )
{
    char            *s,*s_next,*mem,*args[100];
    int                i,count,del_count=0;
    intptr_t        indexFileSz;
    FILE*            indexFile;
    
    // Check to see if it's already loaded
    {
        tStorageIndex* pIndex = NULL;
        int fileOfs = 0;
        prepareOpenFileInfo( spec, &pIndex, &fileOfs );
        if ( pIndex->indexFilePtr != NULL )
        {
            return;
        }
    }
    
    indexFileSz = fileSize(indexFname(spec));
    if (( indexFileSz <= 0 ) ||
        (( indexFile = prepareIndexFile(spec,0,SEEK_SET)) == NULL ))
    {
        return;
    }
    
    if (( mem = (char*)malloc(indexFileSz+1)) == NULL )
    {
        return;
    }
    
    fread(mem,indexFileSz,1,indexFile);
    mem[indexFileSz] = 0;

    //
    // Build our cached index data from scratch    
    //
    storedData_CreateIndexCache(spec);

    s = mem;
    for(i=0;s && (count=tokenize_line(s,args,&s_next));i++,s=s_next)
    {
        int auth_id;
        int db_id;
        OfflinePlayer player;
        if (! VALID_INDEX_REC_FLD_COUNT(count))
        {
            i--;
            continue;
        }
        auth_id = atoi(args[kIndexTextFld_auth_id]);
        db_id    = atoi(args[kIndexTextFld_db_id]);
            
        player.fileKey = atoi(args[kIndexTextFld_fileNbr]);
        player.file_pos = atoi(args[kIndexTextFld_filePos]);
        player.auth_id    = auth_id;
        player.db_id    = db_id;
        strcpy_s(player.name,ARRAY_SIZE(player.name),args[kIndexTextFld_characterName]);
        player.level    = atoi(args[kIndexTextFld_level]);
        player.origin    = atoi(args[kIndexTextFld_origin]);
        player.archetype= atoi(args[kIndexTextFld_archetype]);
        
        // Older records may not have all of these fields
        player.storage_status    = 
                ( count > kIndexTextFld_storageStatus ) ?  
                    atoi(args[kIndexTextFld_storageStatus]) : 
                    kOfflineObjStatus_OFFLINE;
        player.dateStored =
                ( count > kIndexTextFld_dateStored ) ?
                    atoi(args[kIndexTextFld_dateStored]) :
                    0;
        strcpy_s(player.auth_name,ARRAY_SIZE(player.auth_name),
                ( count > kIndexTextFld_AuthName ) ?
                    args[kIndexTextFld_AuthName] :
                    "" );
        
        player.idxfile_pos = s - mem;
        storedData_AddToPlayerArray(spec,&player);
    }
    free(mem);
}

void storageIndexRelease( tOfflineDataStore* pStore, int nIndexOfs )
{
    int i;
    tStorageIndex* pIndex = pStore->storageIndexArr[nIndexOfs];
    if ( pIndex->indexFilePtr != NULL )
    {
        fclose(pIndex->indexFilePtr);
    }

    storedData_DestroyIndexCache(pIndex);

    // Data files.
    {
        for ( i=0; i < pIndex->openFileInfoCount; i++ )
        {
            tOpenFileInfo* pOpenFileInfo = pIndex->openFileInfo[i];
            if ( pOpenFileInfo->filePtr != NULL )
            {
                fclose( pOpenFileInfo->filePtr );
            }
            free(pOpenFileInfo);
        }
        SAFE_FREE(pIndex->openFileInfo);
    }
    
    if (( pStore->storageIndexCount > 1 ) &&
        ( nIndexOfs < (pStore->storageIndexCount-1) ))
    {
        // shift
        for ( i=nIndexOfs; i < ( pStore->storageIndexCount - 1 ); i++ )
        {
            pStore->storageIndexArr[i] = pStore->storageIndexArr[i+1];
        }
    }
    free( pIndex );
    pStore->storageIndexCount--;
    pStore->storageIndexArr[pStore->storageIndexCount] = NULL;

    if ( pStore->activeStorageIndex == pIndex )
    {
        pStore->activeStorageIndex = NULL;    // !!!
    }
}

char *storedData_GetPlayer(tStorageClass storageClass, OfflinePlayer *p)
{
    int        origsize,zipsize,data_size;
    char    *zipdata,*data=NULL;
    FILE    *file;
    tDataFileSpec spec;
    
    spec.storageClass    = storageClass;
    spec.dataFileKey    = p->fileKey;

    if (( file = prepareDataFile(&spec,p->file_pos,SEEK_SET )) == NULL )
    {
        return 0;
    }
    fread(&origsize,4,1,file);
    fread(&zipsize,4,1,file);
    if ( zipsize )
    {
        //
        //    Do some fixups to the loaded data. We need to remove team container ID
        //  references here, to completely cut the player from those teams.
        //  It's very possible that this group no longer exists now, in which case
        //  onlining the character will fail.
        //
        //    The other adjustment is needed to make sure that the database
        //    information obtained from the data file matches the current auth ID
        //    and name. In 04/2011 we modified the auth information and assigned
        //    new IDs. Rather than change multiple gig 'o bytes of stored offline
        //    and deleted player information, we modified the index files only.
        //    Now, when/if the old data is read we do a fixup.
        //
        const char* kStandardSwapText = 
                "SupergroupsId 0\n"
                "TaskforcesId 0\n"
                "Ents2[0].LevelingPactsId 0\n";
        char templateMods[512];
        const char* newAuthName = p->auth_name;
        
        if (( newAuthName == NULL ) || ( *newAuthName == '\0' ))
        {
            newAuthName = pnameFindById( p->auth_id );
        }
        if (( newAuthName != NULL ) && ( *newAuthName != '\0' ))
        {
            sprintf_s( templateMods, ARRAY_SIZE(templateMods),
                    "AuthId %d\n"
                    "AuthName \"%s\"\n"
                    "%s",
                p->auth_id,
                newAuthName,
                kStandardSwapText
             );        
        }
        else
        {
            sprintf_s( templateMods, ARRAY_SIZE(templateMods),
                    "AuthId %d\n"
                    "%s",
                p->auth_id,
                kStandardSwapText
             );
        }
    
        zipdata = malloc(zipsize);
        fread(zipdata,zipsize,1,file);
        data_size = origsize + 1;
        data = malloc(data_size);
        uncompress(data,&origsize,zipdata,zipsize);
        free(zipdata);
        data[data_size-1] = 0;
        tpltFixOfflineData(&data, &data_size, dbListPtr(CONTAINER_ENTS)->tplt);
        tpltUpdateData(&data, &data_size, templateMods, dbListPtr(CONTAINER_ENTS)->tplt);
        prepareDataFile(&spec,-1,0);
    }
    return data;
}

AnyContainer *offlinePlayerTempLoad(int db_id)
{
    char    *data;
    EntCon    *ent_con;
    OfflinePlayer* p;
    DbList    *list = dbListPtr(CONTAINER_ENTS);
    EntCatalogInfo* pCatalogInfo;
    tDataFileSpec spec = { 0 };
    tAuthSpec authSpec = { 0 };

    spec.storageClass = kStorageClass_OfflineCharacters;
    if (( pCatalogInfo = entCatalogGet( db_id, false )) == NULL )
    {
        return NULL;
    }
    authSpec.auth_id = pCatalogInfo->authId;
    authSpec.auth_name_p = NULL;
    
    if ((( p = storedData_StoredPlayerFromAuthAndId( &spec, &authSpec, db_id )) != NULL ) &&
        ( p->fileKey != 0 ))
    {
        data = storedData_GetPlayer(kStorageClass_OfflineCharacters,p);
        ent_con = containerAlloc(list,db_id);
        ent_con->just_created = 1;
        ent_con->demand_loaded = 1;
#if !UPDATE5
        ent_con->dont_save = 1;
#endif
        containerUpdate(list,ent_con->id,data,0);
        return containerPtr(list,db_id);
    }
    return NULL;
}

int offlinePlayerFindByName(char *name)
{
    //
    //    SLOW!! Sequential search through all auths
    //
    int db_id = 0;
    StashElement elem;
    StashTableIterator iter;
    tDataFileSpec spec = { 0 };
    tStorageIndex* pStore = NULL;
    int fileDataIndex = 0;
    
    spec.storageClass = kStorageClass_OfflineCharacters;
    prepareOpenFileInfo( &spec, &pStore, &fileDataIndex );

    stashGetIterator( pStore->playerArrayCache, &iter);
    while ( stashGetNextElement(&iter, &elem))
    {
        OfflinePlayerArray* pList = (OfflinePlayerArray*)stashElementGetPointer(elem);
        int itemI;
        for ( itemI=0; itemI < pList->numItems; itemI++ )
        {
            if ( stricmp( pList->data[itemI]->name,name) == 0 )
            {
                db_id = pList->data[itemI]->db_id;
                break;
            }
        }
    }
    return db_id;
}

int offlinePlayerDelete(int auth_id,char *auth_name,int db_id)
{
    OfflinePlayer *p = NULL;
    tDataFileSpec spec = { 0 };
    tAuthSpec authSpec = { 0 };
    
    authSpec.auth_id = auth_id;
    authSpec.auth_name_p = auth_name;
    
    // We're looking in the offline storage, even though the
    // operation is for deletion.
    spec.storageClass = kStorageClass_OfflineCharacters;
    
    // A player is deleted by "offlining" him and marking his index record
    // with status kOfflineObjStatus_DELETED. That allows a deleted player
    // to be easily restored.
    if (( p = storedData_StoredPlayerFromAuthAndId( &spec, &authSpec, db_id )) != NULL )
    {
        spec.dataFileKey = 0;    //MS: deleted offlined characters will show up if they don't have a file key of 0.  See subversion note for why this is necessary. =(
        patchIndexRecord(&spec,p,kOfflineObjStatus_DELETED);
        p->storage_status = kOfflineObjStatus_DELETED;
    }

    // Add him as a deleted character. Let the caller deal with his entity container, though.
    deletedPlayers_InitDataFileSpec(&spec,NULL);
    addPlayerToOfflineStore(db_id,kOfflineObjStatus_DELETED,&spec,true,false);

    return 1;
}

static char *lockOfflinedCharacter(char *str)
{
    static char *buf=NULL;
    static int buflen=0;

    char diff[128];
    sprintf(diff, "IsSlotLocked \"1\"\n");
    // Update the string
    dynArrayFit(&buf, 1, &buflen, (int)(strlen(str) + 1));
    strcpy(buf, str);
    tpltUpdateData(&buf,&buflen,diff,dbListPtr(CONTAINER_ENTS)->tplt);//,1);
    str = buf;

    return str;
}

int restoreFromOfflineStorage( tStorageClass storageClass, OfflinePlayer *offlinePlayer, const tAuthSpec* authSpec )
{
    int                data_size;
    char            *data,*new_data,old_name[200]="",new_name[200]="";
    EntCon            *ent_con;
    DbList            *list = dbListPtr(CONTAINER_ENTS);

    if (!offlinePlayer)
    {
        return kOfflineRestore_PLAYER_NOT_FOUND;
    }
    if (!offlinePlayer->fileKey)
    {
        return kOfflineRestore_DATA_FILE_UNKNOWN;
    }

    data = storedData_GetPlayer( storageClass, offlinePlayer );
    if(!data)
    {
        return kOfflineRestore_NO_MATCHING_DATA;
    }
    
    if ( offlinePlayer->db_id == 0 )
    {
        // If zero, set it to a negative number to force it to get a new db_id in containerAlloc()
        offlinePlayer->db_id = -1;
    }

    findFieldText(data,"Name",old_name);
    new_data = putCharacterVerifyUniqueName(data);
    if (!new_data)
    {
        //cleanup
        free(data);
        containerUnload(list,offlinePlayer->db_id);    
        return kOfflineRestore_NO_MATCHING_DATA;
    }
    if (new_data != data)
    {
        free(data);
        data = strdup(new_data);
    }
    new_data = lockOfflinedCharacter(data);
    if (new_data != data)
    {
        free(data);
        data = strdup(new_data);
    }
    findFieldText(data,"Name",new_name);
    data_size = (int)(strlen(data) + 1);
    if (stricmp(old_name,new_name)!=0)
    {
        char    db_flags_text[200] = "";
        U32        db_flags;

        findFieldText(data,"DbFlags",db_flags_text);
        db_flags = atoi(db_flags_text);
        db_flags |= 1 << 12; // DBFLAG_RENAMEABLE
        sprintf(db_flags_text,"DbFlags %u\n",db_flags);
        tpltUpdateData(&data, &data_size, db_flags_text, list->tplt);
    }

    if (authSpec->auth_id<=0 || !authSpec->auth_name_p || !authSpec->auth_name_p[0]) {
        return kOfflineRestore_NO_MATCHING_DATA;
    } else {
        char new_auth_text[512] = "";

        // In 04/2011 we modified the auth information and assigned new IDs.
        // Rather than change multiple gig 'o bytes of stored offline and
        // deleted player information, we modified the index files only.

        snprintf(new_auth_text, ARRAY_SIZE(new_auth_text), "AuthId %d\nAuthName \"%s\"\n", authSpec->auth_id, authSpec->auth_name_p);
        tpltUpdateData(&data, &data_size, new_auth_text, list->tplt);
    }

    ent_con = containerAlloc(list,offlinePlayer->db_id);
    ent_con->just_created = 1;

    containerUpdate(list,ent_con->id,putCharacterVerifyUniqueName(data),1);
    playerNameCreate(ent_con->ent_name,ent_con->id, ent_con->auth_id);
    LOG( LOG_OFFLINE, LOG_LEVEL_IMPORTANT, 0, "Restore %u \"%s\" \"%s\"\n", ent_con->auth_id, ent_con->account, ent_con->ent_name);
    containerUnload(dbListPtr(CONTAINER_ENTS),offlinePlayer->db_id);
    free(data);
    
    {
        tDataFileSpec spec;
        spec.storageClass    = storageClass;
        spec.dataFileKey    = offlinePlayer->fileKey;
        patchIndexRecord(&spec,offlinePlayer,kOfflineObjStatus_RESTORED);
    }
    offlinePlayer->storage_status = kOfflineObjStatus_RESTORED;
    
    return kOfflineRestore_SUCCESS;
}

tOfflineRestoreResult offlinePlayerRestoreOfflined(int auth_id,char *auth_name,int db_id)
{
    OfflinePlayer* offlinePlayer = NULL;
    tDataFileSpec spec = { 0 };
    tAuthSpec authSpec = { 0 };
    spec.storageClass = kStorageClass_OfflineCharacters;
    authSpec.auth_id = auth_id;
    authSpec.auth_name_p = auth_name;
    offlinePlayer = storedData_StoredPlayerFromAuthAndId( &spec, &authSpec, db_id);

    if (( offlinePlayer == NULL ) ||
        ( offlinePlayer->storage_status != kOfflineObjStatus_OFFLINE ))
    {
        return kOfflineRestore_NO_MATCHING_DATA;
    }

    if (!authSpec.auth_id)
        authSpec.auth_id = offlinePlayer->auth_id;
    if (!authSpec.auth_name_p || !authSpec.auth_name_p[0])
        authSpec.auth_name_p = offlinePlayer->auth_name;

    return restoreFromOfflineStorage( kStorageClass_OfflineCharacters, offlinePlayer, &authSpec );
}

tOfflineRestoreResult offlinePlayerRestoreDeleted(int auth_id, char* auth_name, char* char_name, 
                                                        int seqNbr, char* deletionDateMMYYYY /* MM/YYYY */)
{
    tDataFileSpec spec;
    OfflinePlayer* offlinePlayer = NULL;
    tAuthSpec authSpec = { 0 };
    deletedPlayers_InitDataFileSpec( &spec, deletionDateMMYYYY );
    storageIndexLoadFromDisk( &spec );
    authSpec.auth_id = auth_id;
    authSpec.auth_name_p = auth_name;
    
    offlinePlayer = storedData_StoredPlayerFromAuthAndName( &spec, kOfflineObjStatus_DELETED, &authSpec, char_name, seqNbr );
    if (( offlinePlayer == NULL ) ||
        ( offlinePlayer->storage_status != kOfflineObjStatus_DELETED ))
    {
        return kOfflineRestore_NO_MATCHING_DATA;
    }

    if (!authSpec.auth_id)
        authSpec.auth_id = offlinePlayer->auth_id;
    if (!authSpec.auth_name_p || !authSpec.auth_name_p[0])
        authSpec.auth_name_p = offlinePlayer->auth_name;

    return restoreFromOfflineStorage( kStorageClass_DeletedCharacters, offlinePlayer, &authSpec );
}

void offlinePlayerListDeleted(int auth_id,char *auth_name, char* deletionDateMMYYYY /* MM/YYYY */, 
                                OfflinePlayer *list,int max_list, int* p_num_found, int* p_auth_id)
{
    int player_count = 0;
    tDataFileSpec spec;
    OfflinePlayerArray* playerList = NULL;
    tAuthSpec authSpec = { 0 };
    deletedPlayers_InitDataFileSpec( &spec, deletionDateMMYYYY );
    storageIndexLoadFromDisk( &spec );
    authSpec.auth_id = auth_id;
    authSpec.auth_name_p = auth_name;
    playerList = storedData_PlayerArrayFromAuthId(&spec, &authSpec );
    if ( playerList != NULL )
    {
        int i=0;
        for ( i=0; (player_count < max_list) && (i < playerList->numItems); i++ )
        {
            if ( playerList->data[i]->storage_status == kOfflineObjStatus_DELETED )
            {
                memcpy( &list[player_count++], playerList->data[i], sizeof(OfflinePlayer) );
            }
        }
    }
    *p_num_found    = player_count;
    *p_auth_id        = auth_id;
}

void makeFullOfflineList()
{
    ColumnInfo    *field_ptrs[2];
    char        *cols,*estr=0;
    int            count,i,idx=0,auth_id;

    cols = sqlReadColumnsSlow(dbListPtr(CONTAINER_ENTS)->tplt->tables,"distinct","AuthId","",&count,field_ptrs);
    for(i=0;i<count;i++)
    {
        auth_id = *(int *)(&cols[idx]);
        idx += field_ptrs[0]->num_bytes;
        estrConcatf(&estr,"INSERT INTO dbo.Offline (AuthId) VALUES (%d);",auth_id);
        if (estrLength(&estr) > 5000 || i == count-1)
        {
            sqlExecAsync(estr, estrLength(&estr));
            estrClear(&estr);
        }
    }

    sqlFifoFinish();
}

void offlineUnusedPlayers()
{
    int                i,db_id,idx,row_count,offline_idx,j,offline_count,auth_id,max_auth_container=0,protect_level,timer;
    char            *cols,*offline_cols,datestr[200],buf[200];
    ColumnInfo        *field_ptrs[3],*offline_ptrs[3];
    DbList            *list = dbListPtr(CONTAINER_ENTS);
    U32                last_active;
    tDataFileSpec    spec;

    //makeFullOfflineList();
    loadstart_printf("Moving old characters to offline (non-sql) storage...");

    protect_level = server_cfg.offline_protect_level;
    if (!protect_level)
        protect_level = 35;

    last_active = timerSecondsSince2000() - 15 * 24 * 3600;
    timerMakeDateStringFromSecondsSince2000(datestr,last_active);

    timer = timerAlloc();

    // Get a list of all containers and authid's in the offline table
    offline_cols = sqlReadColumnsSlow(dbListPtr(CONTAINER_OFFLINE)->tplt->tables, NULL, "ContainerId, AuthId", "ORDER BY ContainerId", &offline_count, offline_ptrs);
    for(offline_idx=0, j=0; j<offline_count; j++)
    {
        char buf[SHORT_SQL_STRING];
        char buf_len;

        // The first item in raw SQL is the container ID. offline_idx starts at the beginning of the buffer.
        max_auth_container = *(int*)(&offline_cols[offline_idx]);
        offline_idx += offline_ptrs[0]->num_bytes;
        auth_id = *(int*)(&offline_cols[offline_idx]);
        offline_idx += offline_ptrs[1]->num_bytes;

        LOG(LOG_OFFLINE, LOG_LEVEL_IMPORTANT, 0, "offlining %d/%d (%.2f / %.2f)\r", j+1, offline_count, timerElapsed(timer)/60, ((timerElapsed(timer) / j) * offline_count)/60);

        // Get a list of the container id's in the ents (active characters) table for this authid
        // which have been active since the cutoff date.
        buf_len = sprintf(buf, "SELECT COUNT(*) FROM dbo.Ents WHERE AuthId = %d AND LastActive > '%s'", auth_id, datestr);
        row_count = sqlGetSingleValue(buf, buf_len, NULL, SQLCONN_FOREGROUND);
        if (row_count)
        {
            // Yes, we found >0 containers for this authid which are active. So go to the next auth id.
            // LSTL: If this authid has more entities in the offline table, we'll be repeating
            // this exact same query again.
            // LSTL: If all we care about is >0 shouldn't we be adding "LIMIT 1" to the query?
            LOG(LOG_OFFLINE, LOG_LEVEL_IMPORTANT, 0, "rejecting offline of authid %i, lastactive is greater than %s", auth_id, datestr);
            continue;
        }
        
        // Dude has no active ents so move his characters to the offline table, unless the
        // player level is above "protect_level". Keep those guys around.
        buf_len = sprintf(buf, "WHERE AuthId = %d AND (Level < %d OR Level IS NULL)", auth_id, protect_level-1);
        cols = sqlReadColumnsSlow(list->tplt->tables, NULL, "ContainerId", buf, &row_count, field_ptrs);
        spec.storageClass    = kStorageClass_OfflineCharacters;
        spec.dataFileKey    = sDataFileStore[kStorageClass_OfflineCharacters].activeStorageIndex->activeFileInfo->id;
        for(idx=0, i=0; i<row_count; i++)
        {
            db_id = *(int *)(&cols[idx]);
            idx += field_ptrs[0]->num_bytes;
            // Serializes the full entity record to a zip file and updates an index file.
            addPlayerToOfflineStore(db_id,kOfflineObjStatus_OFFLINE,&spec,false,true);
        }
        free(cols);
    }
    free(offline_cols);

    // Now we have moved all of the qualifying items from the ents table to non-SQL
    // storage. We no longer need the items currenty in the "offline" table.
    if ( offline_count )
    {
        int buf_len = sprintf(buf, "DELETE FROM dbo.Offline WHERE ContainerId <= %d;", max_auth_container);
        sqlFifoFinish();
        sqlConnExecDirect(buf, buf_len, SQLCONN_FOREGROUND, false);
        printf("\n");
    }

    releaseAllDataFilePtrs();
    timerFree(timer);

    sqlFifoFinish();
    loadend_printf("");
}

U32 fldValueFromLineList( ContainerTemplate *tplt, LineList* lineList, const char* fldName, U32 defaultVal )
{
    U32 val = defaultVal;
    char fldBuffer[512];

    // findFieldTplt() does a strcpy, so check for memory stomps
    *((U32*)( fldBuffer + ARRAY_SIZE(fldBuffer) - sizeof(U32))) = 0xcacaface;
    {
        char* foundVal;
        if (( foundVal = findFieldTplt(tplt,lineList,(char*)fldName,fldBuffer) ) != NULL )
        {
            val = (U32)atoi(foundVal);
        }
    }
    devassert( *((U32*)( fldBuffer + ARRAY_SIZE(fldBuffer) - sizeof(U32))) == 0xcacaface );

    return val;
}

void fldStrFromLineList( ContainerTemplate *tplt, LineList* lineList, const char* fldName, char* buffer, size_t strSz )
{
    char fldBuffer[512];
    buffer[0] = '\0';

    // findFieldTplt() does a strcpy, so check for memory stomps
    *((U32*)( fldBuffer + ARRAY_SIZE(fldBuffer) - sizeof(U32))) = 0xcacaface;
    {
        char* foundVal;
        if (( foundVal = findFieldTplt(tplt,lineList,(char*)fldName,fldBuffer) ) != NULL )
        {
            // an extra copy....  :(
            strncpy_s( buffer, strSz, fldBuffer, _TRUNCATE );
        }
    }
    devassert( *((U32*)( fldBuffer + ARRAY_SIZE(fldBuffer) - sizeof(U32))) == 0xcacaface );
}

#ifdef OFFLINE_ENABLE_DELETION_LOG_IMPORT_CODE
        
    void logTimeToTimeStruct( const char* szLogTime, struct tm* pTime )
    {
        //                           111111111  
        //                 0123456789012345678
        // Format sample: "03-02-2009 11:36:37"
        memset( pTime, 0, sizeof(struct tm) );
        
        if (( szLogTime[2]  == '-' ) &&
            ( szLogTime[5]  == '-' ) &&
            ( szLogTime[13] == ':' ) &&
            ( szLogTime[16] == ':' ))
        {
            pTime->tm_hour    = (((( szLogTime[11] - '0' ) * 10 ) + ( szLogTime[12] - '0' )) - 1 );
            pTime->tm_min    = (((( szLogTime[14] - '0' ) * 10 ) + ( szLogTime[15] - '0' )) - 1 );
            pTime->tm_sec    =  ((( szLogTime[17] - '0' ) * 10 ) + ( szLogTime[18] - '0' ));
            pTime->tm_mon    = (((( szLogTime[0 ] - '0' ) * 10 ) + ( szLogTime[1 ] - '0' )) - 1 );
            pTime->tm_mday    =  ((( szLogTime[3 ] - '0' ) * 10 ) + ( szLogTime[4 ] - '0' ));
            pTime->tm_year    = ((
                (( szLogTime[6] - '0' ) * 1000 ) + 
                (( szLogTime[7] - '0' ) * 100 ) + 
                (( szLogTime[8] - '0' ) * 10 ) + 
                ( szLogTime[9] - '0' )) - 1900 );
        }
        else
        {
            devassert( 0 );    // bad format
        }
    }
    
    static void offlineProcessOneDeletionLogFile( const char* szLogFile )
    {
        // Read a deletion log and add it to our database
        static const size_t kContainerBufferSz = ( 256 * 1024 );

        int line_count = 0;
        int num_imported = 0;
        char* containerBuffer = (char*)malloc( kContainerBufferSz );
        char deletionDate[21] = { 0 };
        char userIpAdd[16] = { 0 };
        char authName[64] = { 0 };
        char charName[64] = { 0 };
        enum 
        {
            kLineStart = 0,
            // these are in order as they appear in the file line
            kTimeStamp,
            kIPAddress,
            kAuthName,
            kCharName,
            kContainer 
        } fld_id;
        int lineDataSize;
        char* destFld = NULL;
        int fldSize;
        int maxFldSize;
        char aChar;
        char lastChar = 0;

        FILE* fLog = fopen( szLogFile, "r" );
        
        LOG( LOG_OFFLINE, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "Processing deletion log: %s.\n", szLogFile );
        if ( fLog == NULL )
        {
            LOG( LOG_OFFLINE, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "offlineProcessDeletionLogFiles - can't open %s.\n", szLogFile );
            return;
        }

        fld_id    = kLineStart;
        lineDataSize    = 0;
        
        while (( aChar = fgetc(fLog)) != EOF )
        {
            if (( aChar == '\t' ) || ( fld_id == kLineStart ))
            {
                //
                //    New Field
                //
                if ( aChar == '\t' )
                {
                    destFld[fldSize] = '\0';
                }
                fld_id++;
                fldSize = 0;
                
                switch( fld_id )
                {
                    case kLineStart :
                        break;
                    case kTimeStamp :
                        destFld        = deletionDate;
                        maxFldSize    = (ARRAY_SIZE(deletionDate)-1);
                        break;
                    case kIPAddress :
                        destFld        = userIpAdd;
                        maxFldSize    = (ARRAY_SIZE(userIpAdd)-1);
                        break;
                    case kAuthName :
                        destFld        = authName;
                        maxFldSize    = (ARRAY_SIZE(authName)-1);
                        break;
                    case kCharName :
                        destFld        = charName;
                        maxFldSize    = (ARRAY_SIZE(charName)-1);
                        break;
                    case kContainer :
                        destFld        = containerBuffer;
                        maxFldSize    = (int)(kContainerBufferSz-1);
                        break;
                    default :
                        break;
                }
                
                if ( aChar == '\t' )
                {
                    continue;
                }
            }
            if (( aChar == '\r' ) || ( aChar == '\n' ))
            {
                //
                //    End-o-line character
                //
                destFld[fldSize] = '\0';
                if ( lineDataSize > 0 )
                {
                    // End of a real line and not just a '\n' after '\r'
                    OfflinePlayer player = { 0 };
                    DbList* list = dbListPtr(CONTAINER_ENTS);
                    LineList lineList = { 0 };
                    struct tm delDate = { 0 };
                    
                    #if 0
                        LOG( LOG_OFFLINE, LOG_LEVEL_IMPORTANT, LOG_CONSOLE,
                            "Date: %s\n"
                            "IP: %s\n"
                            "Auth: %s\n"
                            "Char: %s\n"
                            "Data:\n%s\n",
                                deletionDate, userIpAdd, authName, charName, containerBuffer );
                    #endif
                    
                    logTimeToTimeStruct( deletionDate, &delDate );
                    
                    textToLineList(list->tplt,containerBuffer,&lineList,0);
                    player.auth_id            = (int)fldValueFromLineList(list->tplt,&lineList,"AuthId",0);
                    strcpy_s(player.auth_name,ARRAY_SIZE(player.auth_name),authName);
                    player.db_id            = 0;
                    player.storage_status    = kOfflineObjStatus_DELETED;
                    player.dateStored        = timerGetSecondsSince2000FromTimeStruct(&delDate);
                    //fldStrFromLineList(list->tplt,&lineList,"Name",player.name,ARRAY_SIZE(player.name));
                    strncpy_s(player.name, ARRAY_SIZE(player.name), charName, _TRUNCATE );
                    player.origin            = fldValueFromLineList(list->tplt,&lineList,"Origin",0);
                    player.archetype        = fldValueFromLineList(list->tplt,&lineList,"PlayerType",0);
                    player.fileKey            = 0;
                    player.level            = fldValueFromLineList(list->tplt,&lineList,"Level",0);                    
                    
                    // Presumed date format is mm-dd-yyyy
                    // The file key is YYYYMM.
                    devassert( (strlen(deletionDate)==19) && ( deletionDate[2] == '-' ) && ( deletionDate[5] == '-' ));
                    player.fileKey = (
                            (( deletionDate[6] - '0' ) * 100000) +
                            (( deletionDate[7] - '0' ) * 10000) +
                            (( deletionDate[8] - '0' ) * 1000) +
                            (( deletionDate[9] - '0' ) * 100) +
                            (( deletionDate[0] - '0' ) * 10) +
                            (( deletionDate[1] - '0' ) * 1)
                        );
                    if ( writePlayerDataToDataFiles(kStorageClass_DeletedCharacters, &player, containerBuffer, kOfflineObjStatus_DELETED ))
                    {
                        num_imported++;
                        if (( num_imported % 100 ) == 0 )
                        {
                            printf( "...%d...\n", num_imported );
                        }
                    }
                    else
                    {
                        LOG( LOG_OFFLINE, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "offlineProcessDeletionLogFiles - Error storing character: %s\n", charName );
                    }

                    freeLineList(&lineList);
                }
                line_count++;
                lineDataSize = 0;
                fld_id = kLineStart;
            }
            else
            {
                //
                //    More data
                //
                if (( fld_id == kContainer ) && ( lastChar == '\\' ))
                {
                    // Fixup for strings representing escape sequences.
                    switch ( aChar )
                    {
                        case 'n' :    // "\n" = newline -> \r\n
                            destFld[fldSize-1] = '\r';
                            aChar = '\n';
                            break;
                        case 'q' :    // "\q" = quotation mark
                            fldSize--;
                            aChar = '\"';
                            break;
                        case 's' : // "\s" = single quote
                            fldSize--;
                            aChar = '\'';
                            break;
                        default :
                            break;
                    }
                }
                if ( fldSize < maxFldSize )
                {
                    destFld[fldSize++] = aChar;
                }
                else
                {
                    destFld[fldSize] = '\0';
                    LOG( LOG_OFFLINE, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "offlineProcessDeletionLogFiles - Format error. Field #%d is too long at line %d. Max size is %d\n", fld_id, line_count, maxFldSize );
                    break;
                }
                lineDataSize++;
            }
            lastChar = aChar;
        }
        
        LOG( LOG_OFFLINE, LOG_LEVEL_IMPORTANT, LOG_CONSOLE, "  --> Records imported from %s: %d\n", szLogFile, num_imported );
        fclose( fLog );
        free(containerBuffer);
    }

    void offlineProcessDeletionLogFiles( const char* szLogFileSpec /* can include wildcards */ )
    {
        struct _finddata_t findInfo = { 0 };
        intptr_t findDataHdl;
        char* lastPathNameSegment;
        char fullPath[2048];
        
        strncpy_s( fullPath, ARRAY_SIZE(fullPath), szLogFileSpec, _TRUNCATE );
        
        lastPathNameSegment = strrchr( fullPath, '\\' );
        if ( lastPathNameSegment == NULL ) lastPathNameSegment = strrchr( fullPath, '/' );
        if ( lastPathNameSegment != NULL ) lastPathNameSegment++;    // pass the divider...
        if ( lastPathNameSegment == NULL ) lastPathNameSegment = fullPath;
        
        if (( strchr( lastPathNameSegment, '*' ) != NULL ) ||
            ( strchr( lastPathNameSegment, '?' ) != NULL ))
        {
            //
            // Handle wildcards
            //
            size_t spaceForFileName = (( ARRAY_SIZE(fullPath) - strlen(fullPath) ) -  1 ); 
            bool bMoreFiles = (( findDataHdl = _findfirst( szLogFileSpec, &findInfo )) != -1 );
            while ( bMoreFiles )
            {
                strncpy_s( lastPathNameSegment, spaceForFileName, findInfo.name, _TRUNCATE );
                offlineProcessOneDeletionLogFile( fullPath );
                bMoreFiles = ( _findnext( findDataHdl, &findInfo ) == 0 );
            }
            _findclose( findDataHdl );
        }
        else
        {
            offlineProcessOneDeletionLogFile( szLogFileSpec );
        }
    }

#endif // OFFLINE_ENABLE_DELETION_LOG_IMPORT_CODE
