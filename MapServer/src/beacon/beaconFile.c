
#include <process.h>
#include "beaconFile.h"
#include "beaconPrivate.h"
#include "beaconConnection.h"
#include "beaconPath.h"
#include <utilitieslib/utils/utils.h>
#include "group/group.h"
#include "group/groupnetsend.h"
#include "entity/entserver.h"
#include "entity/entVarUpdate.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/strings_opt.h>
#include "beacon/beaconGenerate.h"
#include <utilitieslib/network/crypt.h>
#include "group/groupscene.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/fileutil.h>
#include "dbcomm/dbcomm.h"
#include "group/groupnetdb.h"
#include "gridcoll/gridcache.h"
#include "cmdparse/cmdserver.h"
#include "render/model.h"
#include "seq/anim.h"
#include <utilitieslib/utils/timing.h>
#include "seq/tricks.h"
#include "gameSys/raidmapserver.h"
#include "ai/entai.h"
#include <utilitieslib/utils/log.h>
// ------------ Beacon file format history ------------------------------------------------------------------
// Version 0: [*** UNSUPPORTED ***]
//   - ???
// Version 1: [*** UNSUPPORTED ***]
//   - ???
// Version 2: [*** UNSUPPORTED ***]
//   - ???
// Version 3: [*** UNSUPPORTED ***]
//   - ???
// Version 4: [*** UNSUPPORTED ***]
//   - ???
// Version 5: [*** UNSUPPORTED ***]
//   - Added connections between basic (NPC) beacons.
//   - Pre-stored beacon indexes for quick lookup when writing the file.
// Version 6:
//   - The pre-stored indexes from v5 were botched for combat beacons.
//   - Bumped lowest-allowable-version to 6, all previous files are fucked.
// Version 7:
//   - Removed beacon radius for combat beacons.
//   - Added beacon ceiling distances (not floor distance though).
// Version 8:
//   - Added beacon floor distances (damn I'm an idiot, see version 7).
//   - Added beacon block galaxies - groups of blocks that can walk to each other.
//   - Removed "blockConn" check data.
// ----------------------------------------------------------------------------------------------------------

// ------------ Beacon date file format history -------------------------------------------------------------
// Version 0:
//   - Only contains latest data date.
// Skipped 1-8 for some reason.
// Version 9:
//   - Contains CRC of stuff.
// ----------------------------------------------------------------------------------------------------------

BeaconProcessState beacon_process;

static const S32 curBeaconFileVersion = 8;
static const S32 oldestAllowableBeaconFileVersion = 6;

static const S32 curBeaconDateFileVersion = 9;

static struct {
    FILE*                    fileHandle;
    S32                        useFileCheck;
    BeaconWriterCallback    callback;
} beaconWriter;

#define FWRITE(x) beaconWriter.callback(&x, sizeof(x))
#define FWRITE_U32(x) {fwriteCompressedU32(x);}
#define FWRITE_U8(x) {U8 i__ = (x);FWRITE(i__);}
#define FWRITE_CHECK(x) if(beaconWriter.useFileCheck) beaconWriter.callback(x,(U32)strlen(x))

static void fwriteCompressedU32(U32 i){
    //fwrite(&i, sizeof(i), 1, f);
    
    if(i < (1 << 6)){
        U8 value = 0x0 | (i << 2);
        FWRITE_U8(value);
    }
    else if(i < (1 << 14)){
        U16 value = 0x1 | (i << 2);
        FWRITE_U8(value & 0xff);
        FWRITE_U8((value >> 8) & 0xff);
    }
    else if(i < (1 << 22)){
        U32 value = 0x2 | (i << 2);
        FWRITE_U8(value & 0xff);
        FWRITE_U8((value >> 8) & 0xff);
        FWRITE_U8((value >> 16) & 0xff);
    }
    else{
        U32 value = 0x3 | (i << 2);
        FWRITE_U8(value & 0xff);
        FWRITE_U8((value >> 8) & 0xff);
        FWRITE_U8((value >> 16) & 0xff);
        FWRITE_U8((value >> 24) & 0xff);
        FWRITE_U8((i >> 30) & 0xff);
    }
}

#if 0
static S32 getGridBlockIndex(BeaconBlock* gridBlock){
    S32 i;
    
    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        if(gridBlock == combatBeaconGridBlockArray.storage[i]){
            return i;
        }
    }
    
    assert(0 && "Uh oh, some majorly messed up stuff happened finding a grid block index.  Please tell Martin.");
    
    return 0;
}

static S32 getSubBlockIndex(BeaconBlock* subBlock){
    BeaconBlock* gridBlock = subBlock->parentBlock;
    S32 i;
    
    for(i = 0; i < gridBlock->subBlockArray.size; i++){
        if(subBlock == gridBlock->subBlockArray.storage[i]){
            return i;
        }
    }
    
    assert(0 && "Uh oh, some majorly messed up stuff happened finding a sub block index.  Please tell Martin.");
    
    return 0;
}

static S32 getGalaxyBlockIndex(BeaconBlock* galaxy){
    S32 i;
    
    for(i = 0; i < combatBeaconGalaxyArray[0].size; i++){
        if(galaxy == combatBeaconGalaxyArray[0].storage[i]){
            return i;
        }
    }
    
    assert(0 && "Uh oh, some majorly messed up stuff happened finding a galaxy block index.  Please tell Martin.");
    
    return 0;
}
#endif

//static S32 getBeaconIndex(Array* array, Beacon* beacon){
//    S32 i;
//    
//    for(i = 0; i < array->size; i++){
//        if(beacon == array->storage[i]){
//            return i;
//        }
//    }
//    
//    assert(0 && "Uh oh, some majorly messed up stuff happened finding a beacon index.  Please tell Martin.");
//    
//    return 0;
//}

static void writeBeacon(Beacon* b){
    S32 i;
    
    FWRITE(b->mat[3]);
    
    FWRITE(b->ceilingDistance);
    FWRITE(b->floorDistance);
    
    // Write ground connections.
    
    FWRITE_U32(b->gbConns.size);
    
    for(i = 0; i < b->gbConns.size; i++){
        BeaconConnection* conn = b->gbConns.storage[i];
        
        // Write the beacon index.
        
        FWRITE_U32(conn->destBeacon->userInt);
    }

    // Write raised connections.
    
    FWRITE_U32(b->rbConns.size);
    
    for(i = 0; i < b->rbConns.size; i++){
        BeaconConnection* conn = b->rbConns.storage[i];
        
        // Write the beacon index.

        FWRITE_U32(conn->destBeacon->userInt);
        
        // Write the min and max height of the connection.
        
        FWRITE(conn->minHeight);
        FWRITE(conn->maxHeight);
    }
}

#if 0
static void writeBlockConnection(BeaconBlockConnection* conn, S32 raised){
    //FWRITE_CHECK("blockconn");

    FWRITE_U32(getGridBlockIndex(conn->destBlock->parentBlock));

    FWRITE_U32(getSubBlockIndex(conn->destBlock));

    if(raised){
        FWRITE(conn->minHeight);
        FWRITE(conn->maxHeight);
    }
}
#endif

static void writeSubBlockStep1(BeaconBlock* subBlock){
    // Write just the position and beacon count so that the beacon array can be created.
    
    //FWRITE_CHECK("subblock1");

    FWRITE(subBlock->pos);
    FWRITE_U32(subBlock->beaconArray.size);
}

static void writeSubBlockStep2(BeaconBlock* subBlock){
    S32 i;

    //FWRITE_CHECK("subblock2");
    
    // Write the connection information.
    
    if(1){
        FWRITE_U32(0);
        FWRITE_U32(0);
    }else{
        #if 0
        FWRITE_U32(subBlock->gbbConns.size);

        for(i = 0; i < subBlock->gbbConns.size; i++){
            writeBlockConnection(subBlock->gbbConns.storage[i], 0);
        }

        FWRITE_U32(subBlock->rbbConns.size);

        for(i = 0; i < subBlock->rbbConns.size; i++){
            writeBlockConnection(subBlock->rbbConns.storage[i], 1);
        }
        #endif
    }
        
    // Write the beacons.
    
    for(i = 0; i < subBlock->beaconArray.size; i++){
        writeBeacon(subBlock->beaconArray.storage[i]);
    }
}

static void writeGridBlockStep1(BeaconBlock* gridBlock){
    // Write just the position, beacon count, and sub-block count so the arrays can be created.
    //FWRITE_CHECK("gridblock1");
    
    FWRITE(gridBlock->pos);
    FWRITE_U32(gridBlock->beaconArray.size);
    FWRITE_U32(gridBlock->subBlockArray.size);
}

static void writeGridBlockStep2(BeaconBlock* gridBlock){
    S32 i;
    
    //FWRITE_CHECK("gridblock2");
    
    for(i = 0; i < gridBlock->subBlockArray.size; i++){
        BeaconBlock* subBlock = gridBlock->subBlockArray.storage[i];
        
        writeSubBlockStep1(subBlock);
    }

    for(i = 0; i < gridBlock->subBlockArray.size; i++){
        BeaconBlock* subBlock = gridBlock->subBlockArray.storage[i];
        
        writeSubBlockStep2(subBlock);
    }
}

static void makeBeaconFileDirectory(char* fileName){
    char absoluteFileName[1000];
    char* atServer;

    forwardSlashes(fileName);
    atServer = strstri(fileName, "server/");
    fileName = atServer ? atServer : fileName;
    sprintf(absoluteFileName, "%s/%s", fileDataDir(), fileName);
    
    if(!makeDirectoriesForFile(absoluteFileName)){
        printf("Can't create directories: %s\n", absoluteFileName);
    }
}

static S32 beaconWriteDateFile(S32 doCheckoutCheckin){
    FILE* f;
    
    assert(estrLength(&beacon_process.beaconDateFileName));
    
    f = fileOpen(beacon_process.beaconDateFileName, "wb");
    
    if(f){
        S32 success = 0;

        //printf("Writing file: %s\n", beacon_process.beaconDateFileName);

        fwrite(&curBeaconDateFileVersion, sizeof(curBeaconDateFileVersion), 1, f);
        
        fwrite(&beacon_process.latestDataFileTime, sizeof(beacon_process.latestDataFileTime), 1, f);
        
        fwrite(&beacon_process.fullMapCRC, sizeof(beacon_process.fullMapCRC), 1, f);
        
        fclose(f);

        return success;
    }
    
    return 0;
}

void writeBeaconFileCallback(BeaconWriterCallback callback, S32 writeCheck){
    S32 curBeacon = 0;
    S32 i;
    
    beaconWriter.callback = callback;

    beaconWriter.useFileCheck = writeCheck;
    
    // Write a version so as not to be an idiot when the file format turns out to suck or something.
    
    FWRITE(curBeaconFileVersion);
    
    FWRITE(beaconWriter.useFileCheck);
    
    // Write the bad connections flag. (This is now removed, so it's always 0).
    
    FWRITE_U32(0);
    
    // Write the regular beacons.

    FWRITE_CHECK("regular");

    FWRITE_U32(basicBeaconArray.size);
    
    for(i = 0; i < basicBeaconArray.size; i++){
        Beacon* beacon = basicBeaconArray.storage[i];
        
        // Use userInt as index holder so no lookup necessary.

        beacon->userInt = i;
        
        FWRITE(beacon->mat[3]);
        FWRITE(beacon->proximityRadius);
    }
    
    for(i = 0; i < basicBeaconArray.size; i++){
        Beacon* beacon = basicBeaconArray.storage[i];
        S32 j;
        
        FWRITE_U32(beacon->gbConns.size);
        
        for(j = 0; j < beacon->gbConns.size; j++){
            BeaconConnection* conn = beacon->gbConns.storage[j];
            
            FWRITE_U32(conn->destBeacon->userInt);
        }        
    }
    
    // Write the traffic beacons.

    FWRITE_CHECK("traffic");

    FWRITE_U32(trafficBeaconArray.size);
    
    for(i = 0; i < trafficBeaconArray.size; i++){
        Beacon* beacon = trafficBeaconArray.storage[i];
        
        beacon->userInt = i;
        
        FWRITE(beacon->mat);
        FWRITE(beacon->proximityRadius);
        FWRITE(beacon->trafficGroupFlags);
        FWRITE(beacon->trafficTypeNumber);
        FWRITE_U8(beacon->killerBeacon);
    }
    
    FWRITE_CHECK("traffic conns");

    for(i = 0; i < trafficBeaconArray.size; i++){
        Beacon* beacon = trafficBeaconArray.storage[i];
        S32 j;

        FWRITE_U32(beacon->gbConns.size);
        
        for(j = 0; j < beacon->gbConns.size; j++){
            BeaconConnection* conn = beacon->gbConns.storage[j];

            FWRITE_U32(conn->destBeacon->userInt);
        }
    }
    
    // Write the beacon block size.
    
    FWRITE_CHECK("block size");

    FWRITE(combatBeaconGridBlockSize);
    
    // Write the combat beacons.
    
    FWRITE_U32(combatBeaconArray.size);
    
    // Should be the count of grid blocks.
    
    FWRITE_U32(combatBeaconGridBlockArray.size);
    
    // Re-order the beacons in the combat beacon array.

    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        BeaconBlock* gridBlock = combatBeaconGridBlockArray.storage[i];
        S32 j;
        
        for(j = 0; j < gridBlock->subBlockArray.size; j++){
            BeaconBlock* subBlock = gridBlock->subBlockArray.storage[j];
            S32 k;
            
            for(k = 0; k < subBlock->beaconArray.size; k++){
                combatBeaconArray.storage[curBeacon++] = subBlock->beaconArray.storage[k];
            }
        }
    }
    
    assert(curBeacon == combatBeaconArray.size);

    // Assign numbers to each beacon for easy reference.  Must be after re-ordering of beacon array.
    
    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* beacon = combatBeaconArray.storage[i];
        
        beacon->userInt = i;
    }        

    // Step 1 is to allow the blocks to create all the sub-blocks with no information inside.
    // Step 2 is to fill in the sub-blocks.
    // Step 1 is necessary so step 2 can refer to sub-blocks of other grid blocks which otherwise
    //   would not have been created yet.
    
    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        writeGridBlockStep1(combatBeaconGridBlockArray.storage[i]);
    }
    
    for(i = 0; i < combatBeaconGridBlockArray.size; i++){
        writeGridBlockStep2(combatBeaconGridBlockArray.storage[i]);
    }

    // Write the galaxy data.
    
    FWRITE_CHECK("galaxies");
    
    if(1){
        // Turn off writing galaxies since they will be dynamically recreated anyway.
        
        FWRITE_U32(0);
    }else{
        #if 0
        FWRITE_U32(combatBeaconGalaxyArray[0].size);
        
        for(i = 0; i < combatBeaconGalaxyArray[0].size; i++){
            BeaconBlock* galaxy = combatBeaconGalaxyArray[0].storage[i];
            S32 j;
            
            // Write ground connections.
        
            FWRITE_U32(galaxy->gbbConns.size);
            
            for(j = 0; j < galaxy->gbbConns.size; j++){
                BeaconBlockConnection* conn = galaxy->gbbConns.storage[j];
                BeaconBlock* destGalaxy = conn->destBlock;
                
                assert(destGalaxy->isGalaxy);

                FWRITE_U32(getGalaxyBlockIndex(destGalaxy));
            }
            
            // Write raised connections.

            FWRITE_U32(galaxy->rbbConns.size);

            for(j = 0; j < galaxy->rbbConns.size; j++){
                BeaconBlockConnection* conn = galaxy->rbbConns.storage[j];
                BeaconBlock* block = conn->destBlock;
                
                assert(block->isGalaxy);
                
                FWRITE_U32(getGalaxyBlockIndex(block));
                FWRITE(conn->minHeight);
                FWRITE(conn->maxHeight);
            }

            // Write IDs for sub-blocks.
            
            FWRITE_U32(galaxy->subBlockArray.size);
            
            for(j = 0; j < galaxy->subBlockArray.size; j++){
                BeaconBlock* subBlock = galaxy->subBlockArray.storage[j];
                
                assert(galaxy == subBlock->galaxy);
                
                FWRITE_U32(getGridBlockIndex(subBlock->parentBlock));
                FWRITE_U32(getSubBlockIndex(subBlock));
            }
        }
        #endif
    }
        
    FWRITE_CHECK("finished");
}

static void writeBeaconFileToFile(const void* data, U32 size){
    fwrite(data, size, 1, beaconWriter.fileHandle);
}

static S32 writeBeaconFile(char* fileName, S32 writeCheck, S32 doCheckoutCheckin){
    S32 i = 0;

    // Check out the beacon file for modification

    if(!doCheckoutCheckin){
        DeleteFileA(fileName);
    }
    
    // Make sure the directories all exist.
    
    makeBeaconFileDirectory(fileName);
    
    // Open the file handle and then use it to store some crap.
    
    beaconWriter.fileHandle = fileOpen(fileName, "wb");
    
    if(!beaconWriter.fileHandle){
        char absoluteFileName[1000];
        
        printf("Can't open beacon file: %s\n", fileName);
        
        sprintf(absoluteFileName, "c:\\temp_beacon_file_%d.v%d.bcn\n", rand() % 1000, curBeaconFileVersion);
        
        beaconWriter.fileHandle = fileOpen(absoluteFileName, "wb");
        
        if(!beaconWriter.fileHandle){
            printf("Also can't open temp beacon file: %s\n", absoluteFileName);
            return 0;
        }
        
        printf(    "********** WARNING, READ BELOW ***********\n"
                "  Writing beacon to '%s'\n"
                "  Please copy it to correct place.\n"
                "********** WARNING, READ ABOVE ***********\n",
                absoluteFileName);
    }
    
    writeBeaconFileCallback(writeBeaconFileToFile, writeCheck);

    fileClose(beaconWriter.fileHandle);

    beaconWriter.fileHandle = NULL;

    // Write the date file.
    
    beaconWriteDateFile(doCheckoutCheckin);
    
    return i;
}

#undef FWRITE
#undef FWRITE_U32
#undef FWRITE_CHECK

#define FREAD_FAIL(x)                    {displayFailure(__FILE__, __LINE__, x);goto failed;}
#define CHECK_RESULT(x)                    if(!(x))FREAD_FAIL(#x)
#define FREAD_ERROR(x,name,size)        if(!beaconReader.callback((char*)(x), size)){FREAD_FAIL(name);}
#define FREAD(x)                        FREAD_ERROR(&x,#x,sizeof(x))
#define FREAD_U32(x)                    {CHECK_RESULT(freadCompressedU32(&x));}
#define FREAD_U8(x)                        {U8 temp__;FREAD_ERROR(&temp__,#x,sizeof(temp__));x = temp__;}
#define FREAD_CHECK(x)                    if(beaconReader.useFileCheck){freadCheck(x,__FILE__,__LINE__);}
#define INIT_CHECK_ARRAY(array,size)    initArray((array),size);CHECK_RESULT((array)->maxSize >= size)

static struct {
    FILE*                    fileHandle;
    BeaconReaderCallback    callback;

    S32                        fileVersion;
    S32                        useFileCheck;

    char*                    fileBuffer;
    const char*                fileBufferPos;
    S32                        fileBufferSize;
    S32                        fileBufferValidRemaining;
    
    S32                        fileTotalSize;
    S32                        fileTotalPos;
    
    S32                        failed;
    S32                        disablePopupErrors;
} beaconReader;

S32 freadBeaconFile(char* data, S32 size){
    S32 sizeRemaining = size;
    
    if(    !beaconReader.fileBuffer ||
        beaconReader.failed || 
        size > beaconReader.fileTotalSize - beaconReader.fileTotalPos)
    {
        beaconReader.failed = 1;
        return 0;
    }

    while(sizeRemaining > 0){
        S32 readSize;
        
        if(!beaconReader.fileBufferValidRemaining){
            S32    fileRemainingSize = beaconReader.fileTotalSize - beaconReader.fileTotalPos;
            S32 readSize = min(beaconReader.fileBufferSize, fileRemainingSize);
            
            if(!fread(beaconReader.fileBuffer, readSize, 1, beaconReader.fileHandle)){
                beaconReader.failed = 1;
                return 0;
            }
            
            assert(beaconReader.fileBufferPos == beaconReader.fileBuffer);
            
            beaconReader.fileBufferValidRemaining = readSize;
        }
        
        readSize = min(sizeRemaining, beaconReader.fileBufferValidRemaining);
        
        memcpy(data, beaconReader.fileBufferPos, readSize);
        beaconReader.fileBufferPos += readSize;
        beaconReader.fileBufferValidRemaining -= readSize;
        beaconReader.fileTotalPos += readSize;
        
        assert(beaconReader.fileTotalPos <= beaconReader.fileTotalSize);
        
        sizeRemaining -= readSize;
        
        if(sizeRemaining > 0 || !beaconReader.fileBufferValidRemaining){
            data += readSize;
            
            beaconReader.fileBufferPos = beaconReader.fileBuffer;
            
            assert(!beaconReader.fileBufferValidRemaining);
        }
    }
    
    return 1;
}

S32 beaconGetReadFileVersion(void){
    return beaconReader.fileVersion;
}

void beaconReaderDisablePopupErrors(S32 set){
    beaconReader.disablePopupErrors = set ? 1 : 0;
}

S32 beaconReaderArePopupErrorsDisabled(void){
    return beaconReader.disablePopupErrors;
}

static void displayFailure(const char* fileName, S32 fileLine, const char* placeName){
    char messageBuffer[10000];
    
    if(beaconReaderArePopupErrorsDisabled()){
        return;
    }
    
    sprintf(messageBuffer,
            "  %s[%d], byte 0x%x/0x%x: FAILED: \"%s\"\n",
            fileName,
            fileLine,
            beaconReader.fileTotalPos,
            beaconReader.fileTotalSize,
            placeName);
            
    printf("%s", messageBuffer);
            
    if(fileIsUsingDevData()){
        Errorf(    "The beacon file could not load.  This may be a weird case that requires a programmer to look into it, or you can just ignore it.  It's up to you." );
    }
    
    LOG_OLD("beacon_load_error %s,%s", world_name, messageBuffer);
}

S32 freadCheck(const char* name, const char* fileName, S32 fileLine){
    char temp[1000];
    
    if(!beaconReader.callback(temp, (U32)strlen(name))){
        sprintf(temp, "CHECK: %s", name);
        FREAD_FAIL(temp);
    }

    CHECK_RESULT(!strncmp(name,temp,strlen(name)));

    #if 0
        printf("  %s[%d]: Check passed: %s\n", fileName, fileLine, name)
    #endif
        
    return 1;

    failed:
    return 0;
}

static S32 freadCompressedU32(U32 *i){
    U8 v0, v1, v2, v3, v4;
    S32 type;
    S32 value;

    if(beaconReader.fileVersion < 8){
        FREAD(*i);
        return 1;
    }

    FREAD_U8(v0);
    
    type = v0 & 3;
    
    value = v0 >> 2;
    
    if(type >= 1){
        FREAD_U8(v1);
        
        value |= v1 << 6;
    }
    
    if(type >= 2){
        FREAD_U8(v2);
    
        value |= v2 << 14;
    }
    
    if(type >= 3){
        FREAD_U8(v3);
        FREAD_U8(v4);
        
        value |= v3 << 22;
        value |= v4 << 30;
    }
    
    *i = value;
    
    return 1;    
    
    failed:
    return 0;
}

static S32 readBlockConnection(BeaconBlock* subBlock, S32 raised){
    BeaconBlockConnection* conn = createBeaconBlockConnection();
    S32 gridBlockIndex;
    S32 subBlockIndex;
    BeaconBlock* gridBlock;
    Array* connArray = raised ? &subBlock->rbbConns : &subBlock->gbbConns;

    if(beaconReader.fileVersion <= 7){
        FREAD_CHECK("blockconn");
    }

    FREAD_U32(gridBlockIndex);
    FREAD_U32(subBlockIndex);
    
    CHECK_RESULT(gridBlockIndex >= 0 && gridBlockIndex < combatBeaconGridBlockArray.size);

    gridBlock = combatBeaconGridBlockArray.storage[gridBlockIndex];
    
    CHECK_RESULT(subBlockIndex >= 0 && subBlockIndex < gridBlock->subBlockArray.size);
    
    conn->destBlock = gridBlock->subBlockArray.storage[subBlockIndex];
    
    if(raised){
        FREAD(conn->minHeight);
        FREAD(conn->maxHeight);
    }
    
    if(conn->destBlock == subBlock){
        // Remove links to myself.  What an idiot I am.
        
        destroyBeaconBlockConnection(conn);
    }else{
        arrayPushBack(connArray, conn);
    }
    
    return 1;

    failed:
    return 0;
}

static S32 readBeacon(BeaconBlock* subBlock, S32 index, Beacon* beacon){
    S32 i;
    S32 connCount;
    
    beacon->madeGroundConnections = 1;
    beacon->madeRaisedConnections = 1;

    subBlock->beaconArray.storage[index] = beacon;
    
    arrayPushBack(&subBlock->parentBlock->beaconArray, beacon);
    
    beacon->block = subBlock;

    FREAD(beacon->mat[3]);
    
    if(beaconReader.fileVersion == 6){
        FREAD(beacon->proximityRadius);
    }else{
        FREAD(beacon->ceilingDistance);
        
        if(beaconReader.fileVersion >= 8){
            FREAD(beacon->floorDistance);
        }
    }
    
    // Read ground connections.
    
    FREAD_U32(connCount);
    
    beaconInitArray(&beacon->gbConns, connCount);
    
    for(i = 0; i < connCount; i++){
        S32 dest;
        BeaconConnection* conn = createBeaconConnection();
        
        FREAD_U32(dest);
        
        CHECK_RESULT(dest >= 0 && dest < combatBeaconArray.size && dest != beacon->userInt);

        conn->destBeacon = combatBeaconArray.storage[dest];
        
        arrayPushBack(&beacon->gbConns, conn);
    }

    // Read raised connections.
    
    FREAD_U32(connCount);
    
    beaconInitArray(&beacon->rbConns, connCount);
    
    for(i = 0; i < connCount; i++){
        S32 dest;
        BeaconConnection* conn = createBeaconConnection();
        
        FREAD_U32(dest);
        
        CHECK_RESULT(dest >= 0 && dest < combatBeaconArray.size);
        
        conn->destBeacon = combatBeaconArray.storage[dest];

        FREAD(conn->minHeight);
        FREAD(conn->maxHeight);
        
        conn->minHeight = max(conn->minHeight, 0.1);
        conn->maxHeight = max(conn->maxHeight, conn->minHeight);
        
        if(beaconReader.fileVersion <= 8){
            conn->maxHeight -= 5;
            
            if(conn->maxHeight < conn->minHeight){
                conn->maxHeight = conn->minHeight;
            }
        }
        
        arrayPushBack(&beacon->rbConns, conn);
    }

    return 1;

    failed:
    return 0;
}

static void beaconReaderStop(void){
    SAFE_FREE(beaconReader.fileBuffer);
    fileClose(beaconReader.fileHandle);
    beaconReader.fileHandle = NULL;
}

S32 readBeaconFileCallback(BeaconReaderCallback callback){
    S32 blockCount;
    S32 beaconCount;
    S32 curBeacon = 0;
    S32 gridBlockBeaconCount;
    S32 subBlockBeaconCount;
    S32 i;
    
    beaconClearAllBlockData(1);

    beaconClearBeaconData(1, 1, 1);
    
    beaconReader.callback = callback;

    // Read the file version.

    FREAD(beaconReader.fileVersion);
    
    FREAD(beaconReader.useFileCheck);
    
    // Check that the version is within the bounds of legal version numbers.

    CHECK_RESULT(    beaconReader.fileVersion >= oldestAllowableBeaconFileVersion &&
                    beaconReader.fileVersion <= curBeaconFileVersion);
    
    // Read the bad connections flag.

    FREAD_U32(i);
    
    // Read the regular beacons.
    
    FREAD_CHECK("regular");

    FREAD_U32(beaconCount);
    
    verbose_printf("  Regular beacon count: %d\n", beaconCount);

    destroyArrayPartialEx(&basicBeaconArray, destroyNonCombatBeacon);

    INIT_CHECK_ARRAY(&basicBeaconArray, beaconCount);
    
    beaconsByTypeArray[BEACONTYPE_BASIC] = &basicBeaconArray;

    for(i = 0; i < beaconCount; i++){
        Beacon* beacon = createBeacon();
        
        beacon->type = BEACONTYPE_BASIC;
        
        FREAD(beacon->mat[3]);
        FREAD(beacon->proximityRadius);
        
        arrayPushBack(&basicBeaconArray, beacon);
    }
    
    for(i = 0; i < beaconCount; i++){
        Beacon* beacon = basicBeaconArray.storage[i];
        S32 connCount;
        S32 j;
        
        FREAD_U32(connCount);

        INIT_CHECK_ARRAY(&beacon->gbConns, connCount);
        
        beacon->gbConns.size = connCount;
        
        beacon->madeGroundConnections = 1;
        
        for(j = 0; j < connCount; j++){
            S32 index;
            BeaconConnection* conn;
            
            FREAD_U32(index);
            
            if(index < 0 || index >= beaconCount){
                FREAD_FAIL("Invalid destination beacon.");
            }
            
            conn = createBeaconConnection();
            
            conn->destBeacon = basicBeaconArray.storage[index];
            
            beacon->gbConns.storage[j] = conn;
        }
    }
    
    // Read the traffic beacons.
    
    FREAD_CHECK("traffic");

    FREAD_U32(beaconCount);
    
    verbose_printf("  Traffic beacon count: %d\n", beaconCount);

    destroyArrayPartialEx(&trafficBeaconArray, destroyNonCombatBeacon);
    INIT_CHECK_ARRAY(&trafficBeaconArray, beaconCount);
    beaconsByTypeArray[BEACONTYPE_TRAFFIC] = &trafficBeaconArray;

    for(i = 0; i < beaconCount; i++){
        Beacon* beacon = createBeacon();

        beacon->type = BEACONTYPE_TRAFFIC;

        FREAD(beacon->mat);
        getMat3YPR(beacon->mat, beacon->pyr);
        FREAD(beacon->proximityRadius);
        FREAD(beacon->trafficGroupFlags);
        
        // Version 4 added traffic type number and killer traffic beacons.
        
        FREAD(beacon->trafficTypeNumber);
        FREAD_U8(beacon->killerBeacon);
        
        arrayPushBack(&trafficBeaconArray, beacon);
    }
    
    FREAD_CHECK("traffic conns");

    for(i = 0; i < beaconCount; i++){
        Beacon* beacon = trafficBeaconArray.storage[i];
        S32 connCount;
        S32 j;
        
        FREAD_U32(connCount);
        
        beacon->madeGroundConnections = 1;
        
        INIT_CHECK_ARRAY(&beacon->gbConns, connCount);
        
        for(j = 0; j < connCount; j++){
            BeaconConnection* conn = createBeaconConnection();
            S32 dest;

            FREAD_U32(dest);
            
            CHECK_RESULT(dest >= 0 && dest < trafficBeaconArray.size);
            
            conn->destBeacon = trafficBeaconArray.storage[dest];
            
            arrayPushBack(&beacon->gbConns, conn);

            arrayPushBack(&beacon->curveArray, beaconGetCurve(beacon, conn->destBeacon));
        }
    }
        
    //qsort(trafficBeaconArray.storage, trafficBeaconArray.size, sizeof(trafficBeaconArray.storage[0]), tempCompareFunction);
    //for(i = 0; i < trafficBeaconArray.size; i++){
    //    Beacon* beacon2 = trafficBeaconArray.storage[i];
    //    
    //    printf("%5d: %5.2f, %5.2f, %5.2f: %d\n", i, posX(beacon2), posY(beacon2), posZ(beacon2), beacon2->trafficGroupFlags);
    //}

    // Get the block size, beacon count and set the combat beacon array.

    FREAD_CHECK("block size");

    FREAD(combatBeaconGridBlockSize);
    
    FREAD_U32(beaconCount);

    verbose_printf("  Combat beacon count: %d\n", beaconCount);
    verbose_printf("  Block size: %1.f\n", combatBeaconGridBlockSize);
    
    assert(!combatBeaconArray.size && !combatBeaconArray.storage);

    INIT_CHECK_ARRAY(&combatBeaconArray, beaconCount);
    
    beaconsByTypeArray[BEACONTYPE_COMBAT] = &combatBeaconArray;

    for(i = 0; i < beaconCount; i++){
        Beacon* beacon = createBeacon();
        
        beacon->type = BEACONTYPE_COMBAT;
        beacon->userInt = i;
        
        arrayPushBack(&combatBeaconArray, beacon);
    }    
    
    // Get the grid block count and set the sizes of the hash table and array.
    
    FREAD_U32(blockCount);
    
    verbose_printf("  Grid block count: %d\n", blockCount);
    
    if(combatBeaconGridBlockTable){
        stashTableDestroy(combatBeaconGridBlockTable);
    }
    
    combatBeaconGridBlockTable = stashTableCreateInt((blockCount * 4) / 3);
    
    destroyArrayPartialEx(&combatBeaconGridBlockArray, destroyBeaconGridBlock);
    INIT_CHECK_ARRAY(&combatBeaconGridBlockArray, blockCount);
    
    // Read the grid blocks, step 1.
    
    gridBlockBeaconCount = 0;
    
    for(i = 0; i < blockCount; i++ ){
        BeaconBlock* gridBlock = createBeaconBlock();
        S32 beaconCount;
        S32 subBlockCount;
        S32 j;
        
        gridBlock->isGridBlock = 1;
        gridBlock->madeConnections = 1;
        
        if(beaconReader.fileVersion < 8){
            FREAD_CHECK("gridblock1");
        }
            
        FREAD(gridBlock->pos);
        FREAD_U32(beaconCount);
        FREAD_U32(subBlockCount);
        
        gridBlockBeaconCount += beaconCount;
        
        CHECK_RESULT(gridBlockBeaconCount <= combatBeaconArray.size);

        INIT_CHECK_ARRAY(&gridBlock->beaconArray, beaconCount);
        
        beaconBlockInitArray(&gridBlock->subBlockArray, subBlockCount);
        
        CHECK_RESULT(stashIntAddPointer(    combatBeaconGridBlockTable,
                                        beaconMakeGridBlockHashValue(vecParamsXYZ(gridBlock->pos)),
                                        gridBlock, false));

        arrayPushBack(&combatBeaconGridBlockArray, gridBlock);
        
        for(j = 0; j < subBlockCount; j++){
            BeaconBlock* subBlock = createBeaconBlock();

            subBlock->madeConnections = 1;
            subBlock->parentBlock = gridBlock;
            
            // Put the sub-block into the parent block array.
            
            gridBlock->subBlockArray.storage[gridBlock->subBlockArray.size++] = subBlock;
        }
    }
    
    // Read the grid blocks, step 2.
    
    subBlockBeaconCount = 0;

    for(i = 0; i < blockCount; i++){
        BeaconBlock* gridBlock = combatBeaconGridBlockArray.storage[i];
        S32 j;
        
        if(((i+1)*100) / blockCount != (i*100) / blockCount){
            verbose_printf("  Loading blocks: %d%%\r", ((i+1)*100) / blockCount);
        }

        if(beaconReader.fileVersion < 8){
            FREAD_CHECK("gridblock2");
        }
            
        // Read sub-blocks, step 1.
        
        for(j = 0; j < gridBlock->subBlockArray.size; j++){
            BeaconBlock* subBlock = gridBlock->subBlockArray.storage[j];
            S32 beaconCount;
            
            if(beaconReader.fileVersion < 8){
                FREAD_CHECK("subblock1");
            }
            
            FREAD(subBlock->pos);
            FREAD_U32(beaconCount);
            
            subBlockBeaconCount += beaconCount;
            
            beaconBlockInitArray(&subBlock->beaconArray, beaconCount);
            
            subBlock->beaconArray.size = beaconCount;
        }

        // Read sub-blocks, step 2.
        
        for(j = 0; j < gridBlock->subBlockArray.size; j++){
            BeaconBlock* subBlock = gridBlock->subBlockArray.storage[j];
            S32 k;
            S32 connCount;
            
            if(beaconReader.fileVersion < 8){
                FREAD_CHECK("subblock2");
            }
            
            // Read the ground connection information.
            
            FREAD_U32(connCount);
            
            beaconBlockInitArray(&subBlock->gbbConns, connCount);

            for(k = 0; k < connCount; k++){
                CHECK_RESULT(readBlockConnection(subBlock, 0));
            }

            // Read the raised connection information.
            
            FREAD_U32(connCount);
            
            beaconBlockInitArray(&subBlock->rbbConns, connCount);

            for(k = 0; k < connCount; k++){
                CHECK_RESULT(readBlockConnection(subBlock, 1));
            }
            
            // Read the beacons.
            
            for(k = 0; k < subBlock->beaconArray.size; k++){
                CHECK_RESULT(readBeacon(subBlock, k, combatBeaconArray.storage[curBeacon++]));
            }
        }
    }

    if(beaconReader.fileVersion >= 8){
        S32 galaxyCount;
        S32 j;
        
        FREAD_CHECK("galaxies");
        
        FREAD_U32(galaxyCount);
        
        clearArrayEx(&combatBeaconGalaxyArray[0], destroyBeaconGalaxyBlock);
        beaconBlockInitArray(&combatBeaconGalaxyArray[0], galaxyCount);
        
        for(i = 0; i < galaxyCount; i++){
            BeaconBlock* galaxy = createBeaconBlock();
            galaxy->isGalaxy = 1;
            combatBeaconGalaxyArray[0].storage[combatBeaconGalaxyArray[0].size++] = galaxy;
        }
        
        assert(combatBeaconGalaxyArray[0].size == galaxyCount);
        
        for(i = 0; i < galaxyCount; i++){
            BeaconBlock* galaxy = combatBeaconGalaxyArray[0].storage[i];
            S32 count;
            
            // Read ground connections.
            
            FREAD_U32(count);
            
            beaconBlockInitArray(&galaxy->gbbConns, count);
            
            for(j = 0; j < count; j++){
                BeaconBlockConnection* conn = createBeaconBlockConnection();
                S32 index;
                
                FREAD_U32(index);
                
                CHECK_RESULT(index >= 0 && index < combatBeaconGalaxyArray[0].size && index != i);
                
                conn->destBlock = combatBeaconGalaxyArray[0].storage[index];
                
                assert(conn->destBlock != galaxy);
                
                arrayPushBack(&galaxy->gbbConns, conn);
            }

            // Read ground connections.
            
            FREAD_U32(count);
            
            beaconBlockInitArray(&galaxy->rbbConns, count);
            
            for(j = 0; j < count; j++){
                BeaconBlockConnection* conn = createBeaconBlockConnection();
                S32 index;
                
                FREAD_U32(index);
                FREAD(conn->minHeight);
                FREAD(conn->maxHeight);
                
                CHECK_RESULT(index >= 0 && index < combatBeaconGalaxyArray[0].size && index != i);
                
                conn->destBlock = combatBeaconGalaxyArray[0].storage[index];

                assert(conn->destBlock != galaxy);

                arrayPushBack(&galaxy->rbbConns, conn);
            }
            
            // Read the sub-block IDs.
            
            FREAD_U32(count);
            
            beaconBlockInitArray(&galaxy->subBlockArray, count);
            
            CHECK_RESULT(galaxy->subBlockArray.maxSize >= count);
            
            for(j = 0; j < count; j++){
                BeaconBlock* block;
                S32 id;
                
                FREAD_U32(id);
                
                CHECK_RESULT(id >= 0 && id < combatBeaconGridBlockArray.size);
                
                block = combatBeaconGridBlockArray.storage[id];
                
                FREAD_U32(id);
                
                CHECK_RESULT(id >= 0 && id < block->subBlockArray.size);
                
                block = block->subBlockArray.storage[id];
                
                block->galaxy = galaxy;
                
                arrayPushBack(&galaxy->subBlockArray, block);
            }
        }
    }else{
        destroyArrayPartialEx(&combatBeaconGalaxyArray[0], destroyBeaconGalaxyBlock);
    }
    
    FREAD_CHECK("finished");
    
    return 1;
    
    failed:
    
    return 0;
}

S32 readBeaconFileFromFile(void* data, U32 size){
    return freadBeaconFile(data, size);
}

S32 readBeaconFile(char* fileName){
    beaconReader.fileHandle = fileOpen(fileName, "rb");
    
    if(!beaconReader.fileHandle){
        printf("  Beacon file doesn't exist: %s\n", fileName);
        return 0;
    }
    
    verbose_printf("  Reading beacon file: %s\n", fileName);

    beaconReader.failed                        = 0;
    beaconReader.fileTotalSize                = fileSize(fileName);
    beaconReader.fileTotalPos                = 0;
    
    beaconReader.fileBufferSize             = 1024 * 1024;
    beaconReader.fileBuffer                    = calloc(beaconReader.fileBufferSize, 1);
    beaconReader.fileBufferPos                = beaconReader.fileBuffer;
    beaconReader.fileBufferValidRemaining    = 0;
    
    verbose_printf("  File size: %d bytes.\n", beaconReader.fileTotalSize);

    CHECK_RESULT(readBeaconFileCallback(readBeaconFileFromFile));

    beaconReaderStop();
    
    verbose_printf("  Successfully read beacon file.\n");

    return 1;

    failed:
    
    beaconReaderStop();

    printf("  ERROR: Failed to read beacon file: %s\n\n\n", fileName);

    return 0;    
}

#undef CHECK_RESULT
#undef FREAD
#undef FREAD_CHECK

    
void beaconGetBeaconFileName(char* outName, S32 version){
    char preMaps[1000];
    char* maps;
    char old_char;

    //char* absoluteWorld;

    // JE: this needs to return a relative path, otherwise if the Map is in a different
    //   folder than the .bcn (i.e. one is in a Pig or a second game data dir), it will not
    //   correctly find the .bcn file
    //absoluteWorld = fileLocateRead(world_name, NULL);
    //if(!absoluteWorld){
    //    printf("Can't find absolute path for world file: %s\n", world_name);
    //    return NULL;
    //}

    maps = strstri(world_name, "maps");

    if(!maps){
        maps = world_name;
    }
    
    old_char = *maps;
    *maps = 0;
        
    sprintf(preMaps, "%s", world_name);
    
    *maps = old_char;

    sprintf(outName, "%sserver/%s.v%d.bcn", preMaps, maps, version);
}

static S32 loadOnlyTrafficBeacons = 0;

F32 beaconSnapPosToGround(Vec3 posInOut){
    F32 floorDistance = beaconGetPointFloorDistance(posInOut);
    
    vecY(posInOut) += 2.0f - floorDistance;
    
    return floorDistance;
}

Beacon* addCombatBeacon(const Vec3 beaconPos,
                        S32 silent,
                        S32 snapToGround,
                        S32 destroyIfTooHigh)
{
    BeaconBlock* gridBlock;
    Beacon* beacon;
    Vec3 pos;
    S32 i;
    
    beacon = createBeacon();
    
    beacon->type = BEACONTYPE_COMBAT;
    
    copyVec3(beaconPos, posPoint(beacon));

    if(snapToGround){
        F32 floorDistance = beaconSnapPosToGround(posPoint(beacon));

        if(floorDistance > 4000.0f){
            if(destroyIfTooHigh){
                destroyCombatBeacon(beacon);
                return NULL;
            }
        }
    }

    beacon->proximityRadius = 0;
    beacon->floorDistance = 0;
    beacon->ceilingDistance = 0;

    copyVec3(posPoint(beacon), pos);
    
    beaconMakeGridBlockCoords(pos);
    
    gridBlock = beaconGetGridBlockByCoords(vecParamsXYZ(pos), 1);
    
    for(i = 0; i < gridBlock->beaconArray.size; i++){
        Beacon* otherBeacon = gridBlock->beaconArray.storage[i];
        
        if(    distance3SquaredXZ(posPoint(otherBeacon), posPoint(beacon)) < SQR(0.1f) &&
            distanceY(posPoint(otherBeacon), posPoint(beacon)) < 5.0f)
        {
            destroyCombatBeacon(beacon);
            return NULL;
        }
    }

    arrayPushBack(&gridBlock->beaconArray, beacon);
    
    beacon->block = gridBlock;

    arrayPushBack(&combatBeaconArray, beacon);
    
    return beacon;
}

struct {
    S32 npc;
    S32 traffic;
} loadBeaconTypes;

static S32 beaconLoaderCallback(GroupDef* def, Mat4 parent_mat){
    Beacon *beacon;
    
    // Don't bother activating things that are inside scenarios.
    if(groupDefIsScenario(def))
        return 0;

    //if(!def->has_beacon) {
    //    if (!def->child_beacon)
    //        return 0;
    //    else
    //        return 1;
    //}

    if(loadBeaconTypes.npc && def->beacon_name && !stricmp(def->beacon_name, "BasicBeacon")){
        beacon = createBeacon();
        beacon->type = BEACONTYPE_BASIC;
        copyVec3(parent_mat[3], beacon->mat[3]);
        beacon->proximityRadius = ceil(def->beacon_radius);
        if(groupDefFindPropertyValue(def, "NPCNoAutoConnect"))
            beacon->NPCNoAutoConnect = 1;
        arrayPushBack(&basicBeaconArray, beacon);

        //addCombatBeacon(parent_mat[3], 0, 1);
        
        return 0;
    }
    //else if(!loadOnlyTrafficBeacons && def->beacon_name && !stricmp(def->beacon_name, "CombatBeacon")){
    //    //addCombatBeacon(parent_mat[3], 0, 1);
    //    return 0;
    //}
    else if(loadBeaconTypes.traffic && def->beacon_name && !stricmp(def->beacon_name, "TrafficBeacon")){
        beacon = createBeacon();
        beacon->type = BEACONTYPE_TRAFFIC;
        copyMat4(parent_mat, beacon->mat);
        getMat3YPR(beacon->mat, beacon->pyr);
        beacon->proximityRadius = ceil(def->beacon_radius);
        beacon->trafficGroupFlags = 0;

        if(def->properties){
            char* group = groupDefFindPropertyValue(def, "BeaconGroup");
            char* typeNumber = groupDefFindPropertyValue(def, "TypeNumber");
            char* killerBeacon = groupDefFindPropertyValue(def, "KillerBeacon");
                
            if(group){
                for(;*group;group++){
                    char thechar = tolower(*group);
                    
                    if(thechar >= '0' && thechar <= '9'){
                        beacon->trafficGroupFlags |= 1 << (thechar - '0');
                    }
                    else if(thechar >= 'a' && thechar <= 'f'){
                        beacon->trafficGroupFlags |= 1 << (10 + thechar - 'a');
                    }
                }
            }
            
            if(typeNumber){
                beacon->trafficTypeNumber = atoi(typeNumber);
            }
            
            if(killerBeacon){
                beacon->killerBeacon = 1;
            }
        }
                
        arrayPushBack(&trafficBeaconArray, beacon);
        return 0;
    }
    
    return 1;
}

void beaconWriteCurrentFile(S32 doCheckoutCheckin){
    char fileName[1000];
    
    beaconGetBeaconFileName(fileName, curBeaconFileVersion);
    
    writeBeaconFile(fileName, 1, doCheckoutCheckin);
}

void beaconFindBeaconsInWorldData(S32 loadNPC, S32 loadTraffic){
    combatBeaconGridBlockSize = 256;
    
    loadBeaconTypes.npc = loadNPC;
    loadBeaconTypes.traffic = loadTraffic;
    
    // Traverse the group info tree to find all beacons.
    
    groupProcessDef(&group_info, beaconLoaderCallback);

    if(loadNPC && basicBeaconArray.size){
        S32 i;

        for(i = basicBeaconArray.size - 1; i >= 0; i--){
            Beacon* beacon = basicBeaconArray.storage[i];

            if(!beaconCheckEmbedded(beacon)){
                beacon->isEmbedded = 1;
            }
        }

        printf("Loaded %d Basic beacons\n", basicBeaconArray.size);
    }
    
    if(loadTraffic && trafficBeaconArray.size){
        printf("Loaded %d Traffic beacons\n", trafficBeaconArray.size);
    }
}

void beaconReload(void){
    S32 readFile = 0;
    S32 v;
    void aiClearBeaconReferences();
    
    verbose_printf("\nLoading beacons.\n");
    
    // Clear all entity variables that may be pointing at beacons.
    
    aiClearBeaconReferences();

    // Initialize the beacon variables.
    
    beaconClearBeaconData(1, 1, 1);

    // Read the stored beacon file if it exists.                                                      d
    
    for(v = curBeaconFileVersion; v >= oldestAllowableBeaconFileVersion; v--){
        char beaconFileName[1000];
        
        beaconGetBeaconFileName(beaconFileName, v);
        
        if(fileExists(beaconFileName)){
            PERFINFO_AUTO_START("readBeaconFile", 1);
                readFile = readBeaconFile(beaconFileName);
            PERFINFO_AUTO_STOP();
            
            if(!readFile){
                // Re-initialize so there isn't partial garbage in the beacon variables.
                
                beaconClearBeaconData(1, 1, 1);
            }else{
                break;
            }
        }
    }
    
    if(!readFile){
        // Get the dummy base raid beacon file if possible.
        
        if(server_state.dummy_raid_base && OnSGBase()){
            readFile = readBeaconFile("server/maps/testing/baseraid_normal.txt.v8.bcn");
            
            if(!readFile){
                beaconClearBeaconData(1, 1, 1);
            }
        }
        
        if(!readFile){
            if(isProductionMode()){
                Errorf("Beacon file not loaded for map: %s", world_name);
                LOG_OLD("beacon Can't load beacons for: %s", world_name);
            }else{
                printf("Beacon file not loaded for map: %s\n", world_name);
            }
            
            beaconReader.fileVersion = 0;
        }
    }

    // Need version 8 to be able to clusterize.

    PERFINFO_AUTO_START("findLimiterVolumes",1);
        aiFindPathLimitedVolumes();
    PERFINFO_AUTO_STOP();
    
    if(readFile && v >= 8){
        beaconRebuildBlocks(0, 1);
    }
            
    verbose_printf("\n");

    beaconPathInit(1);
}

static S32 isFileTimeNewer(time_t older, const char* checkFileName){
    time_t newer = fileLastChanged(checkFileName);
    time_t t;
    
    if (older <= 0 || newer <= 0)
        return 1;

       t = newer - older;
       
       if(!beacon_process.latestDataFileName || !beacon_process.latestDataFileName[0] || newer - beacon_process.latestDataFileTime > 0){
           beacon_process.latestDataFileTime = newer;
          estrPrintCharString(&beacon_process.latestDataFileName, checkFileName);
       }
       
    return t > 0;
}

static S32 printedFirstLine;

static void printFirstLine(void){
    if(!printedFirstLine){
        printedFirstLine = 1;
        consoleSetColor(COLOR_GREEN, 0);
        printf("  +---------------------------------------------------------------------------\n");
    }
}

static const char* getDateString(time_t t){
    static char** tempBuffers;
    static S32 curBufferIndex;
    
    struct tm* theTime;
    char* timeString;
    char** curBuffer;

    if(!tempBuffers){
        tempBuffers = calloc(sizeof(*tempBuffers), 10);
    }
    
    curBuffer = tempBuffers + curBufferIndex++;
    curBufferIndex %= 10;

    theTime = localtime(&t);
    timeString = asctime(theTime);
    
    estrPrintCharString(curBuffer, timeString);
    
    (*curBuffer)[strlen(*curBuffer) - 1] = '\0';
    
    return *curBuffer;
}

static S32 beaconIsFileNewerThanDef(GroupDef* def, time_t fileTime, StashTable table){
    char defName[1000], buf[MAX_PATH];
    char* name;
    char* absoluteDef;
    S32 newer = 0;

    strcpy(defName, def->file->basename);
    
    if ( stashFindElement(table, defName, NULL))
        return 1;
        
    stashAddInt(table, defName, 0, false);
    
    name = strrchr(defName, '/');
    
    if(!name)
        name = defName;
    else
        name++;
        
    strcat(defName, "/");
    name[strlen(name) * 2 - 1] = 0;
    memcpy(defName + strlen(defName), name, strlen(name) - 1);
    strcat(defName, ".txt");
    
    absoluteDef = fileLocateWrite(defName, buf);
    
    if(absoluteDef && fileExists(absoluteDef) && isFileTimeNewer(fileTime, absoluteDef)){
        printFirstLine();

        printf(    "  | NEWER: \"%s\"\n"
                "  |  DATE: %s\n",
                absoluteDef,
                getDateString(fileLastChanged(absoluteDef)));

        newer = 1;
    }
    
    strcpy(defName + strlen(defName) - 4, ".geo");
    
    absoluteDef = fileLocateWrite(defName, buf);

    if(absoluteDef && fileExists(absoluteDef) && isFileTimeNewer(fileTime, absoluteDef)){
        printFirstLine();

        printf(    "  | NEWER: \"%s\"\n"
                "  |  DATE: %s\n",
                absoluteDef,
                getDateString(fileLastChanged(absoluteDef)));
                
        newer = 1;
    }

    return newer;
}

static S32 beaconIsFileNewerThanAllUsedFiles(time_t fileTime){
    S32 i,k;
    char* absoluteWorld;
    StashTable table;
    S32 newer = 0;
    
    table = stashTableCreateWithStringKeys(4,  StashDeepCopyKeys | StashCaseSensitive );
    
    absoluteWorld = fileLocateWrite_s(world_name, NULL, 0);
    
    if(!absoluteWorld){
        printf("WARNING: \"%s\" doesn't exist in any data directories.\n", world_name);
    }

    printedFirstLine = 0;
    
    if(!fileExists(absoluteWorld)){
        printf("DOESN'T EXIST: %s\n", absoluteWorld);
        fileLocateFree(absoluteWorld);
        return 1;
    }

    if(isFileTimeNewer(fileTime, absoluteWorld)){
        printFirstLine();

        printf(    "  | NEWER: \"%s\"\n"
                "  |  DATE: %s\n",
                absoluteWorld,
                getDateString(fileLastChanged(absoluteWorld)));

        newer = 1;
    }

    fileLocateFree(absoluteWorld);

    for(k=0;k<group_info.file_count;k++)
    {
        GroupFile    *file = group_info.files[k];
        for(i = 0; i < file->count; i++){
            S32 groupIsLibSub(GroupDef *def);
            GroupDef* def = file->defs[i];
            
            if(!def->in_use){
                continue;
            }
            
            //if(!def->saved){
            //    printf("A GroupDef is not saved.\n");
            //    newer = 0;
            //    break;
            //}
            
            if(!groupIsLibSub(def) && groupInLib(def) && !beaconIsFileNewerThanDef(def, fileTime, table)){
                newer = 1;
            }
        }
    }
        
    if(printedFirstLine){
        printf("  +---------------------------------------------------------------------------\n");
        consoleSetDefaultColor();
    }

    stashTableDestroy(table);

    return !newer;
}

void beaconProcessNPCBeacons(void){
    S32 i;

    loadstart_printf("Reprocessing NPC beacons..");
    
    aiClearBeaconReferences();
    beaconClearBeaconData(1, 0, 0);
    beaconFindBeaconsInWorldData(1, 0);

    if(basicBeaconArray.size){
        for(i = 0; i < basicBeaconArray.size; i++){
            Beacon* b = basicBeaconArray.storage[i];
        
            printf("    Connecting NPC beacons: %d%%\r", (i+1) * 100 / basicBeaconArray.size);

            if(b->madeGroundConnections){
                clearArrayEx(&b->gbConns, destroyBeaconConnection);
            
                b->madeGroundConnections = 0;
            }

            beaconGetGroundConnections(b, &basicBeaconArray);
        }

        printf("\n");
    }
    loadend_printf("done");
}

void beaconProcessTrafficBeacons()
{
    S32 i;

    aiClearBeaconReferences();
    beaconClearBeaconData(0, 1, 0);
    beaconFindBeaconsInWorldData(0, 1);

    if(trafficBeaconArray.size){
        for(i = 0; i < trafficBeaconArray.size; i++){
            Beacon* b = trafficBeaconArray.storage[i];

            printf("    Connecting traffic beacons: %d%%\r", (i+1) * 100 / trafficBeaconArray.size);

            // Clear the current connections.
            
            if(b->madeGroundConnections){
                clearArrayEx(&b->gbConns, destroyBeaconConnection);
            
                b->madeGroundConnections = 0;
            }

            beaconGetGroundConnections(b, &trafficBeaconArray);
        }

        printf("\n");
    }
}

static void undoCheckout(char* fileName){
    if(!fileName || !fileName[0]){
        return;
    }
        
    printf("Undoing checkout: %s...", fileName);
    
    printf(" DONE!\n");
}

void beaconProcessUndoCheckouts(void){
    undoCheckout(beacon_process.beaconFileName);
    estrDestroy(&beacon_process.beaconFileName);

    undoCheckout(beacon_process.beaconDateFileName);
    estrDestroy(&beacon_process.beaconDateFileName);
}

static BOOL consoleCtrlHandler(DWORD fdwCtrlType){ 
    switch (fdwCtrlType){ 
        case CTRL_CLOSE_EVENT: 
        case CTRL_LOGOFF_EVENT: 
        case CTRL_SHUTDOWN_EVENT: 
        case CTRL_BREAK_EVENT: 
        case CTRL_C_EVENT: 
            consoleSetColor(COLOR_BRIGHT|COLOR_RED, 0);
            printf("\n\n\nBEACON PROCESSING CANCELED!!!\n");
            consoleSetDefaultColor();

            beaconProcessUndoCheckouts();
            
            return FALSE; 

        // Pass other signals to the next handler.

        default: 
            return FALSE; 
    } 
} 

void beaconProcessSetConsoleCtrlHandler(S32 on){
    static S32 curState = 0;
    
    on = on ? 1 : 0;

    if(curState != on){
        SetConsoleCtrlHandler((PHANDLER_ROUTINE)consoleCtrlHandler, on);

        curState = on;
    }
}

static S32 beaconGetFileLock(char* fileName, S32* isNew){
    return 1;
}

void beaconProcessSetTitle(F32 progress, const char* sectionName){
    static char last_section[100] = "None";
    
    char buffer[2000];
    
    if(!beacon_process.titleMapName){
        return;
    }
    
    if(sectionName){
        STR_COMBINE_BEGIN(last_section);
        STR_COMBINE_CAT(sectionName);
        STR_COMBINE_CAT(" ");
        STR_COMBINE_END();
    }
    
    STR_COMBINE_BEGIN(buffer);
    STR_COMBINE_CAT(last_section);
    STR_COMBINE_CAT_D((S32)progress);
    STR_COMBINE_CAT(".");
    STR_COMBINE_CAT_D(((S32)(progress * 10) % 10));
    STR_COMBINE_CAT("% - ");
    STR_COMBINE_CAT_D(_getpid());
    STR_COMBINE_CAT(" : ");
    STR_COMBINE_CAT(beacon_process.titleMapName);
    STR_COMBINE_END();
    
    setConsoleTitle(buffer);
}

typedef struct CRCTriangle {
    Vec3            normal;
    Vec3            vert[3];
    S32                noGroundConnections;
} CRCTriangle;

typedef struct CRCBasicBeacon {
    Vec3            pos;
    S32                validBeaconLevel;
} CRCBasicBeacon;

typedef struct CRCTrafficBeacon {
    Mat4            mat;
    S32                killer;
    S32                group;
    S32                typeNumber;
} CRCTrafficBeacon;

typedef struct CRCModelInfo {
    Model*            model;
    S32                count;
} CRCModelInfo;

static struct {
    StashTable                htModels;
    struct {
        CRCModelInfo*        models;
        S32                    count;
        S32                    maxCount;
    } models;
    
    U32                        totalCRC;
    U8*                        buffer;
    U32                        cur;
    U32                        size;
    U32*                    tri_count;
    
    //struct {
    //    CRCTriangle*        buffer;
    //    U32                    count;
    //    U32                    maxCount;
    //} tris;
    
    struct {
        CRCBasicBeacon*        buffer;
        U32                    count;
        U32                    maxCount;
    } basicBeacons;
    
    struct {
        CRCBasicBeacon*        buffer;
        U32                    count;
        U32                    maxCount;
        S32                    maxValidLevel;
    } legalBeacons;

    struct {
        CRCTrafficBeacon*    buffer;
        U32                    count;
        U32                    maxCount;
    } trafficBeacons;
    
    S32                        no_coll_count;
} crc_info;

#define GET_STRUCTS(name, structCount)                            \
    dynArrayAddStructs(    &crc_info.name.buffer,                    \
                        &crc_info.name.count,                    \
                        &crc_info.name.maxCount,                \
                        structCount),                            \
    ZeroStruct(crc_info.name.buffer + crc_info.name.count)

//static CRCTriangle* getTriMemory(U32 count){
//    return GET_STRUCTS(tris, count);
//}

static CRCBasicBeacon* getBasicBeacon(void){
    return GET_STRUCTS(basicBeacons, 1);
}

static CRCBasicBeacon* getLegalBeacon(void){
    return GET_STRUCTS(legalBeacons, 1);
}

static CRCTrafficBeacon* getTrafficBeacon(void){
    return GET_STRUCTS(trafficBeacons, 1);
}

static F32 approxF32(F32 value){
    return value - fmod(value, 1.0 / 1024);
}

#undef GET_STRUCTS

static U32 freshCRC(void* dataParam, U32 length){
    cryptAdler32Init();
    
    return cryptAdler32((U8*)dataParam, length);
}

static void flushCRC(void){
    U32 crc = freshCRC(crc_info.buffer, crc_info.cur);
    
    crc_info.totalCRC += crc;
    
    //printf("\nadding %5d bytes: 0x%8.8x ==> 0x%8.8x\n", crc_info.cur, crc, crc_info.totalCRC);

    crc_info.cur = 0;
}

static void crcData(void* dataParam, U32 length){
    U8* data = dataParam;
    
    while(length){
        U32 addLen = min(length, crc_info.size - crc_info.cur);
        
        memcpy(crc_info.buffer + crc_info.cur, data, addLen);
        
        length -= addLen;
        crc_info.cur += addLen;
        
        data += addLen;
        
        if(crc_info.cur == crc_info.size){
            flushCRC();
        }
    }
}

static void crcS32(S32 value){
    crcData((U8*)&value, sizeof(value));
}

static void crcApproxF32(F32 value){
    value = approxF32(value);
    
    crcData((U8*)&value, sizeof(value));
}

static void crcVec3(const Vec3 vec){
    crcApproxF32(vec[0]);
    crcApproxF32(vec[1]);
    crcApproxF32(vec[2]);
}

static copyApproxVec3(const Vec3 in, Vec3 out){
    out[0] = approxF32(in[0]);
    out[1] = approxF32(in[1]);
    out[2] = approxF32(in[2]);
}

static copyApproxMat4(const Mat4 in, Mat4 out){
    copyApproxVec3(in[0], out[0]);
    copyApproxVec3(in[1], out[1]);
    copyApproxVec3(in[2], out[2]);
    copyApproxVec3(in[3], out[3]);
}

static S32 __cdecl compareVec3(const Vec3 v1, const Vec3 v2){
    S32 i;
    
    for(i = 0; i < 3; i++){
        if(v1[i] < v2[i]){
            return -1;
        }
        else if(v1[i] > v2[i]){
            return 1;
        }
    }
    
    return 0;
}

//static S32 __cdecl compareTriangles(const CRCTriangle* t1, const CRCTriangle* t2){
//    S32 i;
//    
//    for(i = 0; i < 3; i++){
//        S32 compare = compareVec3(t1->vert[i], t2->vert[i]);
//        
//        if(compare){
//            return compare;
//        }
//    }
//    
//    return compareVec3(t1->normal, t2->normal);
//}

static S32 getIntProperty(GroupDef* def, char* propName){
    char* propValue = groupDefFindPropertyValue(def, propName);
    
    return propValue ? atoi(propValue) : 0;
}

static S32 beaconCRCCallback(GroupDefTraverser* traverser){
    GroupDef*        def = traverser->def;
    Vec3*            parent_mat = traverser->mat;
    Model*            model = def->model;
    S32                i;
    S32                noGroundConnections;
    char*            actorID;
    
    // Find encounter positions.
    
    actorID = groupDefFindPropertyValue(def, "EncounterPosition");

    if(actorID){
        S32 i = bp_blocks.encounterPositions.count;
        
        dynArrayAddStruct(    &bp_blocks.encounterPositions.pos,
                            &bp_blocks.encounterPositions.count,
                            &bp_blocks.encounterPositions.maxCount);
                    
        ZeroStruct(bp_blocks.encounterPositions.pos + i);

        copyVec3(parent_mat[3], bp_blocks.encounterPositions.pos[i].pos);
        bp_blocks.encounterPositions.pos[i].actorID = atoi(actorID);
    }

    if(def->beacon_name){
        if(!stricmp(def->beacon_name, "BasicBeacon")){
            CRCBasicBeacon* b = getBasicBeacon();
            
            copyApproxVec3(parent_mat[3], b->pos);
        }
        else if(!stricmp(def->beacon_name, "TrafficBeacon")){
            CRCTrafficBeacon* b = getTrafficBeacon();
            
            copyApproxMat4(parent_mat, b->mat);
            
            b->killer = groupDefFindPropertyValue(def, "KillerBeacon") ? 1 : 0;
            b->group = getIntProperty(def, "BeaconGroup");
            b->typeNumber = getIntProperty(def, "TypeNumber");
        }
    }else{
        S32 level = beaconGetValidStartingPointLevel(def);
        
        if(level){
            CRCBasicBeacon* b = getLegalBeacon();
            
            copyApproxVec3(parent_mat[3], b->pos);
            beaconSnapPosToGround(b->pos);
            
            b->validBeaconLevel = level;

            crc_info.legalBeacons.maxValidLevel = max(crc_info.legalBeacons.maxValidLevel, b->validBeaconLevel);
        }
    }

    //#define USE_CORRECT_CRC 1
    
    #if USE_CORRECT_CRC
        // This is the newer, correct version.
        
        if (!model
            ||
            traverser->no_coll_parents
            ||
            def->no_coll
            ||
            model->trick &&
            (    model->trick->info->lod_near > 0 ||
                model->trick->info->tnode.flags1 & TRICK_NOCOLL))
        {
            return 1;
        }
    #else
        // This is the older, broken version.  Maintaining so all beacon files don't rebuild, yet.
        
        if(    !model
            ||
            model->grid.size == 0.0f
            ||
            def->no_coll
            ||
            traverser->no_coll_parents)
        {
            return 1;
        }
    #endif
    
    noGroundConnections = groupDefFindProperty(def, "NoGroundConnections") ? 1 : 0;
    
    if(!model->ctris){
        modelCreateCtris(model);
        assert(model->ctris);
    }
    
    #if USE_CORRECT_CRC
        assert(model->grid.size > 0);
    #endif
    
    {
        CRCModelInfo*    info;
        S32                i;
        
        if(!crc_info.htModels){
            crc_info.htModels = stashTableCreateWithStringKeys(1000, StashDeepCopyKeys);
        }
        
        if(stashFindInt(crc_info.htModels, model->name, &i)){
            info = crc_info.models.models + i;
        }else{
            info = dynArrayAddStruct(    &crc_info.models.models,
                                        &crc_info.models.count,
                                        &crc_info.models.maxCount);
                                
            ZeroStruct(info);
            info->model = model;
                                
            stashAddInt(crc_info.htModels, model->name, crc_info.models.count - 1, false);
        }
        
        info->count++;
    }
    
    for(i = 0; i < model->tri_count; i++){
        CRCTriangle    crcTri;
        U32            triCRC;
        CTri*        tri;
        Vec3        verts[3];
        Vec3        normal;
        S32            j;
        
        tri = model->ctris + i;
        
        crcTri.noGroundConnections = noGroundConnections;
        
        mulVecMat3(tri->norm, parent_mat, normal);
        
        copyApproxVec3(normal, crcTri.normal);
        
        expandCtriVerts(tri, verts);
        
        for(j = 0; j < 3; j++){
            Vec3 vert;
            
            mulVecMat4(verts[j], parent_mat, vert);
            
            copyApproxVec3(vert, crcTri.vert[j]);
        }
        
        qsort(crcTri.vert, 3, sizeof(crcTri.vert[0]), compareVec3);
        
        triCRC = freshCRC(&crcTri, sizeof(crcTri));
        
        {
            U16 crcU16 = (triCRC & (0xffff)) + ((triCRC >> 16) & 0xffff);
            
            crc_info.tri_count[crcU16]++;
        }
    }
    
    return 1;
}

static S32 __cdecl compareBasicBeacons(const CRCBasicBeacon* b1, const CRCBasicBeacon* b2){
    return compareVec3(b1->pos, b2->pos);
}

static S32 compareInt(S32 i1, S32 i2){
    if(i1 < i2)
        return -1;
    else if(i1 > i2)
        return 1;
    else
        return 0;
}

static S32 __cdecl compareTrafficBeacons(const CRCTrafficBeacon* b1, const CRCTrafficBeacon* b2){
    S32 i;
    S32 compare;
    
    #define COMPARE(x){        \
        compare = x;        \
        if(compare){        \
            return compare;    \
        }                    \
    }
    
    for(i = 0; i < 4; i++){
        COMPARE(compareVec3(b1->mat[i], b2->mat[i]));
    }
    
    COMPARE(compareInt(b1->group, b2->group));
    
    COMPARE(compareInt(b1->killer, b2->killer));
    
    COMPARE(compareInt(b1->group, b2->group));
    
    return 0;
}

static S32 __cdecl compareCRCModelInfo(const CRCModelInfo* i1, const CRCModelInfo* i2){
    return stricmp(i1->model->name, i2->model->name);
}

U32 beaconGetMapCRC(S32 collisionOnly, S32 quiet, S32 printFullInfo){
    GroupDefTraverser traverser = {0};
    U8 buffer[256*256];
    char* suffix;
    
    if(quiet){
        printFullInfo = 0;
    }
    
    suffix = printFullInfo ? "\n" : "";

    ZeroStruct(&crc_info);
    crc_info.buffer = buffer;
    crc_info.size = ARRAY_SIZE(buffer);
    
    if(!quiet){
        char* info = printFullInfo ? " (With Full Info)" : "";
        if(collisionOnly){
            printf("Calculating collision CRC%s: %s", info, suffix);
        }else{
            printf("Calculating full CRC%s: %s", info, suffix);
        }
    }
    
    // CRC the max height.

    if(printFullInfo){
        printf("Max Height: %1.1f\n", scene_info.maxHeight);
    }

    crcApproxF32(scene_info.maxHeight);
    
    // CRC all the collidable world geometry.
    
    if(!quiet){
        printf("CRCing Triangles, %s", suffix);
    }

    if(!crc_info.tri_count){
        crc_info.tri_count = calloc(sizeof(crc_info.tri_count[0]), 1 << 16);
    }
    
    bp_blocks.encounterPositions.count = 0;
    crc_info.legalBeacons.maxValidLevel = 0;
    crc_info.no_coll_count = 0;

    groupProcessDefExBegin(&traverser, &group_info, beaconCRCCallback);
    
    if(1){
        S32 i;
        U32 total_tri_count = 0;
        
        for(i = 0; i < (1<<16); i++){
            if(printFullInfo){
                if(!(i % 10)){
                    printf("\n");
                }
                if(crc_info.tri_count[i]){
                    printf("%5d(%d), ", i, crc_info.tri_count[i]);
                }
            }
            total_tri_count += crc_info.tri_count[i];
        }
        
        if(printFullInfo){
            printf("\n");
        }

        if(printFullInfo){
            printf("Total triangles: %d\n", total_tri_count);
        }
    }
    
    // Sort and CRC the triangles.
    
    crcData(crc_info.tri_count, sizeof(crc_info.tri_count[0]) * (1 << 16));
    
    SAFE_FREE(crc_info.tri_count);
    
    // Sort and CRC the beacons.
    
    if(!quiet){
        printf("Sorting Extras, %s", suffix);
    }

    if(!collisionOnly){
        S32 i;
        
        if(printFullInfo){
            printf("Basic Beacons: %d\n", crc_info.basicBeacons.count);
        }
        
        qsort(crc_info.basicBeacons.buffer, crc_info.basicBeacons.count, sizeof(crc_info.basicBeacons.buffer[0]), compareBasicBeacons);
        crcData(crc_info.basicBeacons.buffer, sizeof(crc_info.basicBeacons.buffer[0]) * crc_info.basicBeacons.count);
        SAFE_FREE(crc_info.basicBeacons.buffer);
        ZeroStruct(&crc_info.basicBeacons);
        
        // Find maxValidLevel and invalidate beacons that are less than it.

        for(i = 0; i < (S32)crc_info.legalBeacons.count; i++){
            if(crc_info.legalBeacons.buffer[i].validBeaconLevel < crc_info.legalBeacons.maxValidLevel){
                crc_info.legalBeacons.buffer[i].validBeaconLevel = 0;
            }
        }

        if(printFullInfo){
            printf("Legal Beacons: %d\n", crc_info.legalBeacons.count);
        }

        qsort(crc_info.legalBeacons.buffer, crc_info.legalBeacons.count, sizeof(crc_info.legalBeacons.buffer[0]), compareBasicBeacons);
        crcData(crc_info.legalBeacons.buffer, sizeof(crc_info.legalBeacons.buffer[0]) * crc_info.legalBeacons.count);
        SAFE_FREE(crc_info.legalBeacons.buffer);
        ZeroStruct(&crc_info.legalBeacons);

        if(printFullInfo){
            printf("Traffic Beacons: %d\n", crc_info.trafficBeacons.count);
        }

        qsort(crc_info.trafficBeacons.buffer, crc_info.trafficBeacons.count, sizeof(crc_info.trafficBeacons.buffer[0]), compareTrafficBeacons);
        crcData(crc_info.trafficBeacons.buffer, sizeof(crc_info.trafficBeacons.buffer[0]) * crc_info.trafficBeacons.count);
        SAFE_FREE(crc_info.trafficBeacons.buffer);
        ZeroStruct(&crc_info.trafficBeacons);
    }
    
    flushCRC();
    
    if(printFullInfo){
        S32 i;
        
        qsort(crc_info.models.models, crc_info.models.count, sizeof(crc_info.models.models[0]), compareCRCModelInfo);
        
        for(i = 0; i < crc_info.models.count; i++){
            CRCModelInfo* info = crc_info.models.models + i;
            
            printf(    "%5d, %5d, %d: %s\n",
                    info->count,
                    info->model->tri_count,
                    info->model->grid.size ? 1 : 0,
                    info->model->name);
        }
    }
    
    if(crc_info.htModels)
        stashTableDestroy(crc_info.htModels);
    SAFE_FREE(crc_info.models.models);
    ZeroStruct(&crc_info.models);

    if(!quiet){
        printf("Done! (CRC=0x%8.8x, EncounterPositions=%d)\n", crc_info.totalCRC, bp_blocks.encounterPositions.count);
    }
    
    return crc_info.totalCRC;
}

void beaconCalculateMapCRC(S32 printFullInfo){
    beacon_process.fullMapCRC = beaconGetMapCRC(0, 0, printFullInfo);
}

static S32 beaconFileMatchesMapCRC(const char* beaconDateFile, S32 getTime){
    // Get the date from the date file first, and failing that the beacon file.
    
    FILE* f = fileOpen(beaconDateFile, "rb");
    S32 gotTime = 0;
    S32 crcMatches = 0;

    if(f){
        S32 version;
        
        fread(&version, sizeof(version), 1, f);
        
        // Read the file time.
        
        if(fread(&beacon_process.beaconFileTime, sizeof(beacon_process.beaconFileTime), 1, f)){
            gotTime = 1;
            
            if(version >= 9){
                U32 fileCRC;
    
                // Read the CRC.
                
                if(fread(&fileCRC, sizeof(fileCRC), 1, f)){
                    // Check if the CRC matches.
                    
                    crcMatches = fileCRC == beacon_process.fullMapCRC;
                    
                    if(!crcMatches){
                        consoleSetColor(COLOR_BRIGHT|COLOR_GREEN, 0);
                        printf("CRC doesn't match!  (new: 0x%8.8x   old: 0x%8.8x)\n", beacon_process.fullMapCRC, fileCRC);
                        consoleSetDefaultColor();
                    }
                }
            }
        }
        
        fileClose(f);
    }
    
    if(getTime && !gotTime){
        beacon_process.beaconFileTime = fileLastChanged(beacon_process.beaconFileName);
        gotTime = 1;
    }
    
    if(gotTime){
        printf("Previous Newest Data Time: %s\n", getDateString(beacon_process.beaconFileTime));
    }
    
    return crcMatches;
}

S32 beaconFileIsUpToDate(S32 noFileCheck){
    if(!noFileCheck){
        S32 crcMatches = beaconFileMatchesMapCRC(beacon_process.beaconDateFileName, 1);
        S32 readFile;

        PERFINFO_AUTO_START("readBeaconFile", 1);
            readFile = readBeaconFile(beacon_process.beaconFileName);
        PERFINFO_AUTO_STOP();
        
        if(!readFile){
            printf("WARNING: Current beacon file won't load.  Forcing beaconizing.\n");
        }
        else if(crcMatches){
            consoleSetColor(COLOR_BRIGHT|COLOR_RED, 0);
            printf("CANCELED: CRC matches.\n");
            consoleSetDefaultColor();
            return 1;
        }
        else if(0 && beaconIsFileNewerThanAllUsedFiles(beacon_process.beaconFileTime)){
            printf("CANCELED: Beacon file is newer than all data files for this level.\n");
            return 1;
        }
    }else{
        printf("WARNING: Skipping file-date check.\n");
    }
    
    return 0;
}    

S32 beaconDoesTheBeaconFileMatchTheMap(S32 printFullInfo){
    // Thie is an function to call from an external source to check if the beacons are up to date.

    char beaconDateFile[1000];
    
    beaconGetBeaconFileName(beaconDateFile, curBeaconFileVersion);
    strcat(beaconDateFile, ".date");

    beaconCalculateMapCRC(printFullInfo);
    
    return beaconFileMatchesMapCRC(beaconDateFile, 0);
}

void beaconCreateFilenames()
{
    char tempFileName[1000];
    
    // Create the filenames.
    beaconGetBeaconFileName(tempFileName, curBeaconFileVersion);
    estrPrintCharString(&beacon_process.beaconFileName, tempFileName);
    strcat(tempFileName, ".date");
    estrPrintCharString(&beacon_process.beaconDateFileName, tempFileName);
}

S32 beaconCheckoutBeaconFiles(S32 noFileCheck, S32 removeOldFiles){
    // Create a temporary lock file if the beacon file doesn't currently exist.
    if(!beaconGetFileLock(beacon_process.beaconFileName, noFileCheck ? NULL : &noFileCheck)){
        beaconProcessUndoCheckouts();
        return 0;
    }
    
    // Get a lock on the beacon date file.
    if(!beaconGetFileLock(beacon_process.beaconDateFileName, NULL)){
        beaconProcessUndoCheckouts();
        return 0;
    }
    
    if(beaconFileIsUpToDate(noFileCheck)){
        beaconProcessUndoCheckouts();

        return 0;
    }

    // Force a run through the def list to get the latest data file change.

    beacon_process.latestDataFileTime = 0;
    estrClear(&beacon_process.latestDataFileName);
    
    beaconIsFileNewerThanAllUsedFiles(beacon_process.beaconFileTime);

    printf("Current Newest Data Time: %s\n", getDateString(beacon_process.latestDataFileTime));

    if(beacon_process.latestDataFileName){
        printf("Current Newest Data File: %s\n", beacon_process.latestDataFileName);
    }
    
    return 1;
}

void beaconProcess(S32 noFileCheck, S32 removeOldFiles, S32 generateOnly){
    //char*    absoluteWorld;
    //char    buf[MAX_PATH];
    
    // Intialize the timer.
    
    beaconCurTimeString(1);
        
    consoleInit(110, 128, 0);
    
    // Clear all entity variables that may be pointing at beacons.
    
    aiClearBeaconReferences();

    printf("\n**************************************** BEACON PROCESSING ****************************************\n\n");
    
    if(!group_info.ref_count){
        consoleSetColor(COLOR_BRIGHT | COLOR_RED, 0);
        printf("CANCELED: There are no objects in this world (%s).\n", world_name);
        consoleSetDefaultColor();
        return;
    }
    
    beaconCalculateMapCRC(0);
    
    if(beaconGetDebugVar("noprocess")){
        return;
    }

    // Set the title-bar map name.

    beacon_process.titleMapName = strstr(world_name, "maps/");
    
    if(!beacon_process.titleMapName){
        beacon_process.titleMapName = strstr(world_name, "maps\\");
    }
    
    if(beacon_process.titleMapName){
        beacon_process.titleMapName += 5;
    }else{
        beacon_process.titleMapName = world_name;
    }
    
    // Find the disk location of the map file.

    //absoluteWorld = fileLocateWrite(world_name, buf);
    //
    //if(!absoluteWorld){
    //    consoleSetColor(COLOR_BRIGHT | COLOR_RED, 0);
    //    printf("CANCELED: Can't find absolute path for world file: %s\n", world_name);
    //    consoleSetDefaultColor();
    //    return;
    //}

    beaconProcessSetConsoleCtrlHandler(1);
    
    if(!generateOnly){
        beaconCreateFilenames();
        if(!beaconCheckoutBeaconFiles(noFileCheck, removeOldFiles)){
            return;
        }
    }
    
    beaconClearBeaconData(1, 1, 1);
    beaconFindBeaconsInWorldData(1, 1);

    // Create basic beacon connections.

    if(!generateOnly){
        printf("Connecting %d NPC beacons.\n", basicBeaconArray.size);

        beaconProcessNPCBeacons();
    }
    
    // Connect traffic beacons.
    
    if(!generateOnly){
        printf("Connecting %d Traffic beacons.\n", trafficBeaconArray.size);

        beaconProcessTrafficBeacons();
    }
    
    // Process combat beacons.

    printf("Processing %d COMBAT beacons.\n", combatBeaconArray.size);

    PERFINFO_AUTO_START("beaconProcessCombatBeacons", 1);
        beaconProcessCombatBeacons(1, !generateOnly);
    PERFINFO_AUTO_STOP();
    
    if(generateOnly){
        return;
    }
    
    printf("Done at %s\n", getDateString(time(NULL)));
    
    printf("\nTotal process time: %s\n", beaconCurTimeString(0));
            
    printf("Writing file: %s\n", beacon_process.beaconFileName);
    
    if(writeBeaconFile(beacon_process.beaconFileName, 1, 1)){
        
    }
    
    printf("\nFile size: %d bytes\n", fileSize(beacon_process.beaconFileName));
    
    beaconProcessSetConsoleCtrlHandler(0);

    assert(heapValidateAll());
}

