
#include "beaconClientServerPrivate.h"
#include "beaconPrivate.h"
#include "entity/entity.h"
#include "bases/bases.h"
#include "bases/basedata.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/fileWatch.h>
#include <utilitieslib/utils/log.h>

// Client-To-Server Messages.

const char* BMSG_C2ST_READY_TO_WORK                = "ReadyToWork";
const char* BMSG_C2ST_NEED_MORE_MAP_DATA        = "NeedMoreMapData";
const char* BMSG_C2ST_MAP_DATA_IS_LOADED        = "MapDataIsLoaded";
const char* BMSG_C2ST_NEED_MORE_EXE_DATA        = "NeedMoreExeData";
const char* BMSG_C2ST_GENERATE_FINISHED            = "GenerateFinished";
const char* BMSG_C2ST_BEACON_CONNECTIONS        = "BeaconConnections";
const char* BMSG_C2ST_SENTRY_CLIENT_LIST        = "SentryClientList";
const char* BMSG_C2ST_SERVER_STATUS                = "ServerStatus";
const char* BMSG_C2ST_REQUESTER_MAP_DATA        = "RequesterMapData";
const char* BMSG_C2ST_REQUESTER_CANCEL            = "RequesterCancel";
const char* BMSG_C2ST_USER_INACTIVE                = "UserInactive";
const char* BMSG_C2ST_BEACON_FILE                = "BeaconFile";
const char* BMSG_C2ST_NEED_MORE_BEACON_FILE        = "NeedMoreBeaconFile";
const char* BMSG_C2ST_REQUESTED_MAP_LOAD_FAILED    = "RequestedMapLoadFailed";
const char* BMSG_C2ST_PING                        = "Ping";

// Server-To-Client Messages.

const char* BMSG_S2CT_KILL_PROCESSES            = "KillProcesses";
const char* BMSG_S2CT_MAP_DATA                    = "MapData";
const char* BMSG_S2CT_MAP_DATA_LOADED_REPLY        = "MapDataLoadedReply";
const char* BMSG_S2CT_EXE_DATA                    = "ExeData";
const char* BMSG_S2CT_NEED_MORE_BEACON_FILE        = "NeedMoreBeaconFile";
const char* BMSG_S2CT_PROCESS_LEGAL_AREAS        = "ProcessLegalAreas";
const char* BMSG_S2CT_BEACON_LIST                = "BeaconList";
const char* BMSG_S2CT_CONNECT_BEACONS            = "ConnectBeacons";
const char* BMSG_S2CT_TRANSFER_TO_SERVER        = "TransferToServer";
const char* BMSG_S2CT_CLIENT_CAP                = "ClientCap";
const char* BMSG_S2CT_STATUS_ACK                = "StatusAck";
const char* BMSG_S2CT_REQUEST_CHUNK_RECEIVED    = "RequestChunkReceived";
const char* BMSG_S2CT_REQUEST_ACCEPTED            = "RequestAccepted";
const char* BMSG_S2CT_PROCESS_REQUESTED_MAP        = "ProcessRequestedMap";
const char* BMSG_S2CT_EXECUTE_COMMAND            = "ExecuteCommand";
const char* BMSG_S2CT_BEACON_FILE                = "BeaconFile";
const char* BMSG_S2CT_REGENERATE_MAP_DATA        = "RegenerateMapData";
const char* BMSG_S2CT_PING                        = "Ping";

// The global structure for storing and transferring map data.

typedef struct BeaconMapDataPacket {
    struct {
        S32                        previousSentByteCount;
        S32                        currentByteCount;
    } header;
    
    struct {
        U8*                        data;
        U32                        byteCount;
        U32                        crc;
    } compressed;
    
    U32                            uncompressedBitCount;
    U32                            uncompressedByteCount;
    
    U32                            receivedByteCount;

    S32                            packetDebugInfo;
} BeaconMapDataPacket;

// Other globals.

BeaconClientConnection beacon_client_conn;

static struct {
    S32        clientServerType;
    char*    masterServerName;
    char*    subServerName;
    S32        noNetStart;
    char*    beaconRequestCacheDir;
    S32     onePassOnly;
    S32     forceRebuild;
} beaconizerInit;    
    
struct {
    struct {
        struct {
            S32                        bitCount;
            S32                        hitCount;
        }    defNames,
            defDimensions,
            modelData,
            defs,
            fullVec3,
            indexVec3,
            modelInfo,
            fullDefChild,
            indexDefChild,
            defs2;
    } sent;
    
    U32                                mapDataLoadedFromPacket : 1;
    U32                                productionMode            : 1;
    U32                                allowNovodex            : 1;
} beacon_common;

MP_DEFINE(SentModelInfo);

static SentModelInfo* createSentModelInfo(void){
    MP_CREATE(SentModelInfo, 1000);
    
    return MP_ALLOC(SentModelInfo);
}

static void destroySentModelInfo(SentModelInfo* info){
    MP_FREE(SentModelInfo, info);
}

HWND beaconGetConsoleWindow(){
    return compatibleGetConsoleWindow();
}

//MP_BASIC(BeaconDiskDataChunk, 10);

MP_DEFINE(BeaconLegalAreaCompressed);

BeaconLegalAreaCompressed* createBeaconLegalAreaCompressed(){
    MP_CREATE(BeaconLegalAreaCompressed, 10000);
    
    return MP_ALLOC(BeaconLegalAreaCompressed);
}

void destroyBeaconLegalAreaCompressed(BeaconLegalAreaCompressed* area){
    S32 i = 0;
    
    if(!area){
        return;
    }
    
    #if BEACONGEN_STORE_AREAS
        #if BEACONGEN_CHECK_VERTS
            for(i = 0; i < area->areas.count; i++){
                beaconMemFree(&area->areas.areas[i].defName);
            }
        #endif
        
        SAFE_FREE(area->areas.areas);
        
        ZeroStruct(&area->areas);
    #endif
    
    #if BEACONGEN_CHECK_VERTS
        for(i = 0; i < area->tris.count; i++){
            beaconMemFree(&area->tris.tris[i].defName);
        }
        
        SAFE_FREE(area->tris.tris);
        
        ZeroStruct(&area->tris);
    #endif
    
    MP_FREE(BeaconLegalAreaCompressed, area);
}

BeaconLegalAreaCompressed* beaconAddLegalAreaCompressed(BeaconDiskSwapBlock* block){
    BeaconLegalAreaCompressed* legalArea = createBeaconLegalAreaCompressed();

    legalArea->next = block->legalCompressed.areasHead;
    block->legalCompressed.areasHead = legalArea;
    block->legalCompressed.totalCount++;
    
    return legalArea;
}

void beaconVprintf(S32 color, const char* format, va_list argptr){
    if(color){
        consoleSetColor(color, 0);
    }
    vprintf(format, argptr);
    consoleSetDefaultColor();
}

void beaconPrintfDim(S32 color, const char* format, ...){
    va_list argptr;
    
    va_start(argptr, format);
    beaconVprintf(color, format, argptr);
    va_end(argptr);
}

void beaconPrintf(S32 color, const char* format, ...){
    va_list argptr;
    
    va_start(argptr, format);
    beaconVprintf(COLOR_BRIGHT|color, format, argptr);
    va_end(argptr);
}

void beaconInitCommon(S32 initGrid){
    CONSOLE_CURSOR_INFO info = {0};

    // Make the blinking cursor big, for olde timey fun.
    
    info.dwSize = 100;
    info.bVisible = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    if(initGrid){
        // Set the grid cache.

        gridCacheSetSize(14);
    }
}

void beaconCheckDuplicates(BeaconDiskSwapBlock* block){
    BeaconLegalAreaCompressed* area;
    BeaconLegalAreaCompressed* area2;
    
    return;
    
    for(area = block->legalCompressed.areasHead; area; area = area->next){
        for(area2 = block->legalCompressed.areasHead; area2; area2 = area2->next){
            if(area == area2){
                continue;
            }
            
            if(    area->isIndex == area2->isIndex &&
                area->x == area2->x &&
                area->z == area2->z &&
                (    area->isIndex && area->y_index == area2->y_index ||
                    !area->isIndex && area->y_coord == area2->y_coord))
            {
                assert(0);
            }
        }        
    }
}

void beaconVerifyUncheckedCount(BeaconDiskSwapBlock* block){
    BeaconLegalAreaCompressed* area;
    S32 count = 0;
    
    return;
    
    for(area = block->legalCompressed.areasHead; area; area = area->next){
        if(!area->checked){
            count++;
        }
    }
    
    assert(count == block->legalCompressed.uncheckedCount);
    
    beaconCheckDuplicates(block);
}

void beaconInitGenerating(S32 quiet){
    gridCacheReset();

    beaconClearBeaconData(1, 1, 1);
    
    // Initialize generator.
    
    beaconResetGenerator();

    // Measure everything.

    beaconGenerateMeasureWorld(quiet);
}

void beaconReceiveColumnAreas(Packet* pak, BeaconLegalAreaCompressed* area){
    area->areas.count = pktGetBitsPack(pak, 1);
    
    #if BEACONGEN_STORE_AREAS
    {
        S32 newCount = 0;
        S32 i;
        
        dynArrayAddStructs(&area->areas.areas, &newCount, &area->areas.maxCount, area->areas.count);
            
        for(i = 0; i < newCount; i++){
            area->areas.areas[i].y_min = pktGetF32(pak);
            area->areas.areas[i].y_max = pktGetF32(pak);

            #if BEACONGEN_CHECK_VERTS
            {
                S32 j;
                
                for(j = 0; j < 3; j++){
                    area->areas.areas[i].triVerts[j][0] = pktGetF32(pak);
                    area->areas.areas[i].triVerts[j][1] = pktGetF32(pak);
                    area->areas.areas[i].triVerts[j][2] = pktGetF32(pak);
                }
                
                beaconMemFree(&area->areas.areas[i].defName);
                
                area->areas.areas[i].defName = beaconStrdup(pktGetString(pak));
            }
            #endif
        }

        #if BEACONGEN_CHECK_VERTS
        {
            area->tris.count = 0;
            
            while(pktGetBits(pak, 1) == 1){
                S32 i = area->tris.count;
                S32 j;
            
                dynArrayAddStruct(&area->tris.tris, &area->tris.count, &area->tris.maxCount);
                
                beaconMemFree(&area->tris.tris[i].defName);
                area->tris.tris[i].defName = beaconStrdup(pktGetString(pak));

                area->tris.tris[i].y_min = pktGetF32(pak);
                area->tris.tris[i].y_max = pktGetF32(pak);

                for(j = 0; j < 3; j++){
                    area->tris.tris[i].verts[j][0] = pktGetF32(pak);
                    area->tris.tris[i].verts[j][1] = pktGetF32(pak);
                    area->tris.tris[i].verts[j][2] = pktGetF32(pak);
                }
            }
        }
        #endif
    }
    #endif
}

char* beaconGetLinkIPStr(NetLink* link){
    return makeIpStr(link->addr.sin_addr.S_un.S_addr);
}

void beaconTestCollision(){
    S32 x;
    S32 z;
    
    printf("\n\n");

    for(z = bp_blocks.grid_max_xyz[2]; z >= bp_blocks.grid_min_xyz[2]; z--){
        for(x = bp_blocks.grid_min_xyz[0]; x <= bp_blocks.grid_max_xyz[0]; x++){
            BeaconDiskSwapBlock* block = beaconGetDiskSwapBlockByGrid(x, z);
            
            if(!block){
                printf("%6s", "");
            }else{
                Vec3 pos = {x * BEACON_GENERATE_CHUNK_SIZE + BEACON_GENERATE_CHUNK_SIZE / 2,
                            beacon_process.world_max_xyz[1],
                            z * BEACON_GENERATE_CHUNK_SIZE + BEACON_GENERATE_CHUNK_SIZE / 2};
                Vec3 pos2;
                CollInfo coll;

                copyVec3(pos, pos2);
                
                pos2[1] = beacon_process.world_min_xyz[1];
                
                if(block){
                    if(!block->geoRefs.count){
                        consoleSetColor(COLOR_BRIGHT|COLOR_RED, 0);
                    }
                    else if(block->clients.count){
                        consoleSetColor(COLOR_BRIGHT|COLOR_GREEN|COLOR_BLUE, 0);
                    }
                    else if(block->legalCompressed.uncheckedCount){
                        consoleSetColor(COLOR_BRIGHT|COLOR_GREEN, 0);
                    }
                    else if(block->legalCompressed.totalCount){
                        consoleSetColor(COLOR_BRIGHT|COLOR_GREEN|COLOR_BLUE, 0);
                    }
                    else{
                        consoleSetColor(COLOR_GREEN, 0);
                    }
                }else{
                    consoleSetColor(COLOR_RED, 0);
                }

                if(collGrid(NULL, pos, pos2, &coll, 0, COLL_NOTSELECTABLE | COLL_DISTFROMSTART | COLL_BOTHSIDES)){
                    printf("%6.1f", coll.mat[3][1]);
                }else{
                    printf("%6s", "[----]");
                }
            }
        }
        
        printf("\n");
    }

    printf("\n\n");

    consoleSetDefaultColor();
}

U8* beaconFileAlloc(const char* fileName, U32* fileSize){
    HANDLE h = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
    DWORD size;
    U8* data;
    DWORD outSize;
    
    h = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
    if(h == INVALID_HANDLE_VALUE){
        printf("Can't open file to read: %d\n", GetLastError());
        return NULL;
    }
    
    size = GetFileSize(h, NULL);
    data = beaconMemAlloc("exeFile", size);

    if(fileSize){
        *fileSize = size;
    }
    
    if(!ReadFile(h, data, size, &outSize, NULL)){
        printf("Can't read from file: %d\n", GetLastError());
        return NULL;
    }
    
    CloseHandle(h);
    
    return data;
}

S32 checkForCorrectExePath(    const char* exeName,
                            const char* cmdLineParams,
                            S32 earlyMutexRelease,
                            S32 hideNewWindow)
{
    const char* exeFileName = beaconGetExeFileName();
    const char* baseFolder = "c:/beaconizer/";
    const char* folderPrefix = "beaconcopy.";
    
    // Check if the name is like: "c:/beaconizer/beaconcopy.xxx.x.x/BeaconClient.exe".
    
    if(    !strStartsWith(exeFileName, baseFolder)
        ||
        !strStartsWith(exeFileName + strlen(baseFolder), folderPrefix)
        ||
        !strEndsWith(exeFileName + strlen(baseFolder) + strlen(folderPrefix), exeName)
        ||
        exeFileName[strlen(exeFileName) - strlen(exeName) - 1] != '/')
    {
        U32 size;
        U8* data = beaconFileAlloc(exeFileName, &size);
        
        printf("sizeof(%s) = %d\n", exeFileName, size);
        
        if(!beaconStartNewExe(    exeName, 
                                data,
                                size,
                                cmdLineParams,
                                1,
                                earlyMutexRelease,
                                hideNewWindow))
        {
            printf("Can't start new executable: %s!\n", exeName);
            return 0;
        }
    }
    
    return 1;
}

static U32 freshCRC(void* dataParam, U32 length){
    cryptAdler32Init();
    
    return cryptAdler32((U8*)dataParam, length);
}

U32 beaconGetExeCRC(U8** outFileData, U32* outFileSize){
    S32 fileSize;
    char* fileName = beaconGetExeFileName();
    U8* fileData = beaconFileAlloc(fileName, &fileSize);
    U32 crc;
    
    if(!fileData){
        outFileData = NULL;
        return 0;
    }
    
    crc = fileData ? freshCRC(fileData, fileSize) : 0;
    
    if(outFileData){
        *outFileData = fileData;
        *outFileSize = fileSize;
    }else{
        beaconMemFree((void**)&fileData);
    }
    
    return crc;
}
    
static void beaconClearGroupDef(GroupDef* def){
    S32 i;
    
    if(!def){
        return;
    }
    
    for(i = 0; i < def->count; i++){
        GroupEnt* ent = def->entries + i;
        
        beaconMemFree((void**)&ent->mat);
    }
    
    beaconMemFree(&def->entries);
    
    beaconMemFree((void**)&def->name);
}

static void beaconFreeUnusedMemoryPoolsHelper(MemoryPool pool, void *unused_param){
    if(!mpGetAllocatedCount(pool) && mpGetAllocatedChunkMemory(pool)){
        char fileName[1000];
        
        strcpy(fileName, mpGetFileName(pool) ? mpGetFileName(pool) : "nofile");
        
        if(0){
            beaconPrintf(COLOR_RED|COLOR_GREEN,
                        "Freeing memory pool %s(%s:%d): %s bytes.\n",
                        mpGetName(pool) ? mpGetName(pool) : "???",
                        getFileName(fileName),
                        mpGetFileLine(pool),
                        getCommaSeparatedInt(mpGetAllocatedChunkMemory(pool)));
        }
        
        mpFreeAllocatedMemory(pool);
    }
}

void beaconFreeUnusedMemoryPools(){
    mpForEachMemoryPool(beaconFreeUnusedMemoryPoolsHelper,NULL);
}

void beaconResetMapData(){
    extern Grid obj_grid;
    S32 i;
    
    gridCacheSetSize(0);

    if(beacon_common.mapDataLoadedFromPacket){
        // Only do this stuff for clients.
        
        groupInfoDefault(&group_info);

        for(i = 0; i < group_info.ref_count; i++){
            groupRefDel(group_info.refs[i]);
            
            beaconMemFree(&group_info.refs[i]);
        }
        beaconMemFree((void**)&group_info.refs);
        group_info.ref_count = group_info.ref_max = 0;

        for(i = 0; i < group_info.file_count; i++){
            GroupFile* file = group_info.files[i];
            S32 j;

            for(j = 0; j < file->count; j++){
                beaconClearGroupDef(file->defs[j]);
                
                beaconMemFree(&file->defs[j]);
            }
            
            beaconMemFree((void**)&file->defs);
            
            beaconMemFree(&group_info.files[i]);
        }
        
        beaconMemFree((void**)&group_info.files);
        group_info.file_count = group_info.file_max = 0;

        gridFreeAll(&obj_grid);
        
        beacon_common.mapDataLoadedFromPacket = 0;
    }else{
        groupReset();
    }
    
    beaconClearBeaconData(1, 1, 1);
    
    SAFE_FREE(beacon_process.infoArray);
    
    beaconResetGenerator();

    beaconFreeUnusedMemoryPools();
}

S32 beaconCreateNewExe(const char* path, U8* data, U32 size){
    char buffer[1000];
    DWORD outSize;
    HANDLE h;
    S32 i;
    
    makeDirectoriesForFile(path);

    // Create the new file.
    
    for(i = 0;; i++){
        S32 error;
        
        h = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        
        if(h != INVALID_HANDLE_VALUE){
            break;
        }
        
        error = GetLastError();
        printf("%2d. Failed to open file (error %d): %s\n", i + 1, error, path);
        Sleep(500);

        if(i == 99){
            sprintf(buffer, "Can't open file (error %d): %s\n\nPress IGNORE to continue trying.\n", error, path);
            printf("%s\n", buffer);
            assertmsg(0, buffer);
            i = -1;
        }
    }
            
    if(!WriteFile(h, data, size, &outSize, NULL)){
        sprintf(buffer, "Can't write to file (error %d): %s\n", GetLastError(), path);
        printf("%s\n", buffer);
        assertmsg(0, buffer);
        return 0;
    }
    
    CloseHandle(h);
    
    return 1;
}

S32 beaconDeleteOldExes(const char* exeName, S32* attemptCount){
    intptr_t p;
    struct _finddata_t info;
    S32 deletedFiles = 0;
    char buffer[1000];
    
    if(attemptCount){
        attemptCount[0] = 0;
    }

    sprintf(buffer, "c:/beaconizer/beaconcopy*");
    
    p = _findfirst(buffer, &info);
    
    if(p != -1){
        do{
            const char* fileName;
            strcpy(buffer, exeName);
            fileName = getFileName(buffer);

            if(attemptCount){
                attemptCount[0]++;
            }
            
            // Delete the file.
            
            sprintf(buffer, "c:/beaconizer/%s/%s", info.name, exeName);
            beaconPrintf(COLOR_RED, "Deleting: %s\n", buffer);
            
            if(!DeleteFileA(buffer)){
                beaconPrintf(COLOR_YELLOW, "  FAILED!!!\n");
            }else{
                beaconPrintf(COLOR_GREEN, "  DONE!!!\n");
                deletedFiles++;
            }

            // Delete the folder.
            
            sprintf(buffer, "c:/beaconizer/%s", info.name);
            beaconPrintf(COLOR_RED, "Deleting: %s\n", buffer);

            if(!RemoveDirectoryA(buffer)){
                beaconPrintf(COLOR_YELLOW, "  FAILED!!!\n");
            }else{
                beaconPrintf(COLOR_GREEN, "  DONE!!!\n");
                deletedFiles++;
            }
        }while(!_findnext(p, &info));
        
        _findclose(p);
    }
    
    sprintf(buffer, "c:/beaconizer/beacon*.exe");
    
    p = _findfirst(buffer, &info);
    
    if(p != -1){
        do{
            if(    stricmp(info.name, "BeaconServer.exe") &&
                stricmp(info.name, "BeaconClient.exe"))
            {
                // Delete the file.
                
                sprintf(buffer, "c:/beaconizer/%s", info.name);
                beaconPrintf(COLOR_RED, "Deleting: %s\n", buffer);
                
                if(!DeleteFileA(buffer)){
                    beaconPrintf(COLOR_YELLOW, "  FAILED!!!\n");
                }else{
                    beaconPrintf(COLOR_GREEN, "  DONE!!!\n");
                    deletedFiles++;
                }
            }
        }while(!_findnext(p, &info));
    }
    
    return deletedFiles;
}

S32 beaconStartNewExe(    const char* exeName,
                        U8* data,
                        U32 size,
                        const char* cmdLineParams,
                        S32 exitWhenDone,
                        S32 earlyMutexRelease,
                        S32 hideNewWindow)
{
    static S32 processFileUID = 0;
    
    SYSTEMTIME sys_time;
    HANDLE hMutex = 0;
    char buffer[1000];
    char mutexName[1000];
    S32 attemptCount;
    S32 deletedFiles;
    
    makeDirectoriesForFile(exeName);
    
    _chdir(getDirectoryName(strcpy(buffer, exeName)));
    
    processFileUID++;
    
    GetLocalTime(&sys_time);
    sprintf(buffer,
            "c:/beaconizer/beaconcopy.%d.%d.(%04d-%02d-%02d).(%02d-%02d-%02d)/%s",
            _getpid(),
            processFileUID,
            sys_time.wYear,
            sys_time.wMonth,
            sys_time.wDay,
            sys_time.wHour,
            sys_time.wMinute,
            sys_time.wSecond,
            exeName);
            
    sprintf(mutexName, "Global\\CrypticMutex%s", getFileName((char*)exeName));

    hMutex = CreateMutexA(NULL, FALSE, mutexName);

    printf("Creating new exe (%d bytes): %s\n", size, buffer);
    
    printf("Waiting for mutex \"%s\"...", mutexName);
    WaitForSingleObject(hMutex, INFINITE);
    printf("Done!\n");
    
    if(!beaconCreateNewExe(buffer, data, size)){
        sprintf(buffer, "Couldn't create new exe file: %s\n", buffer);
        printf(buffer);
        assertmsg(0, buffer);
        exit(0);
    }
    
    printf("Starting new exe!\n");
    
    if(exitWhenDone){
        beaconClientReleaseSentryMutex();
    }
        
    if((intptr_t)ShellExecuteA(NULL, "open", buffer, cmdLineParams, "", hideNewWindow ? SW_HIDE : SW_SHOW) <= 32){
        if(hMutex){
            printf("Releasing mutex.\n");
            beaconReleaseAndCloseMutex(&hMutex);
        }
        return 0;
    }

    deletedFiles = beaconDeleteOldExes(exeName, &attemptCount);

    if(exitWhenDone){
        if(attemptCount){
            if(hMutex && earlyMutexRelease){
            
                printf("Releasing mutex.\n");
                beaconReleaseAndCloseMutex(&hMutex);
            }

            if(deletedFiles){
                deletedFiles = 5;
                
                if(IsWindowVisible(beaconGetConsoleWindow())){
                    ShowWindow(beaconGetConsoleWindow(), SW_MINIMIZE);
                }
            
                beaconPrintf(COLOR_GREEN, "Exiting in ");
                
                while(deletedFiles){
                    beaconPrintf(COLOR_GREEN, "%d, ", deletedFiles--);
                    Sleep(1000);
                }

                beaconPrintf(COLOR_GREEN, "BYE!!!\n\n");
                Sleep(500);
            }
        }
    }

    if(hMutex){
        printf("Releasing mutex.\n");
        beaconReleaseAndCloseMutex(&hMutex);
    }

    if(exitWhenDone){
        exit(0);
    }
    
    return 1;
}

void beaconHandleNewExe(Packet* pak,
                        const char* exeName,
                        const char* cmdLineParams,
                        S32 earlyMutexRelease,
                        S32 hideNewWindow)
{
    U32 size;
    U8*    data;

    consoleSetColor(COLOR_BRIGHT|COLOR_GREEN, 0);
    printf("Server sent new exe file.\n");

    consoleSetDefaultColor();
    size = pktGetBitsPack(pak, 1);
    data = beaconMemAlloc("newExe", size);
    pktGetBitsArray(pak, 8 * size, data);

    beaconStartNewExe(exeName, data, size, cmdLineParams, 1, earlyMutexRelease, hideNewWindow);
}

void beaconReleaseAndCloseMutex(HANDLE* mutexPtr){
    if(*mutexPtr){
        ReleaseMutex(*mutexPtr);
        CloseHandle(*mutexPtr);
        *mutexPtr = NULL;
    }
}

static StashTable htBeaconMem;

typedef struct BeaconMemoryModule {
    struct BeaconMemoryModule*    self;
    const char*                    name;
    U32                            count;
    U32                            size;
    U32                            opCount;
} BeaconMemoryModule;

static BeaconMemoryModule* beaconUpdateMemory(const char* module, S32 create, U32 size){
    StashElement el;
    BeaconMemoryModule* bmm;
    
    if(!htBeaconMem){
        htBeaconMem = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);
    }
    
    stashFindElement(htBeaconMem, module, &el);
    
    if(!el){
        bmm = calloc(sizeof(*bmm), 1);
        bmm->self = bmm;
        bmm->name = module;
        stashAddPointer(htBeaconMem, module, bmm, false);
    }else{
        bmm = stashElementGetPointer(el);
        assert(bmm && bmm->self == bmm);
    }
    
    bmm->opCount++;

    if(create){
        //printf("alloc[%20s]: %10d 0x%8.8x\n", module, size);
        
        bmm->count++;
        bmm->size += size;
    }else{
        //printf("free [%20s]: %10d 0x%8.8x\n", module, size);

        assert(bmm->count > 0);
        bmm->count--;
        bmm->size -= size;
    }
    
    return bmm;
}

void beaconPrintMemory(){
    memMonitorDisplayStats();

    if(htBeaconMem){
        StashTableIterator it;
        StashElement el;
        
        stashGetIterator(htBeaconMem, &it);
        
        while(stashGetNextElement(&it, &el)){
            BeaconMemoryModule* bmm = stashElementGetPointer(el);
            
            printf("%20s: %10d %10d bytes\n", bmm->name, bmm->count, bmm->size);
        }
    }
}

void* beaconMemAlloc(const char* module, U32 size){
    U32* mem = calloc(size + 2 * sizeof(*mem), 1);
    
    mem[0] = size;
    mem[1] = (uintptr_t)beaconUpdateMemory(module, 1, size);

    return mem + 2;
}

void beaconMemFree(void** memVoid){
    if(*memVoid){
        uintptr_t* mem = *memVoid;
        BeaconMemoryModule* bmm = (BeaconMemoryModule*)*--mem;
        
        assert(bmm && bmm->self == bmm);
        
        beaconUpdateMemory(bmm->name, 0, *--mem);
        
        free(mem);
        
        *memVoid = NULL;
    }
}

char* beaconStrdup(const char* str){
    char* mem = beaconMemAlloc("strdup", (U32)strlen(str) + 1);
    
    strcpy(mem, str);
    
    return mem;
}

S32 beaconIsGoodStringChar(char c){
    return    isalnum(c) ||
            strchr(" !@#$%^&*()-_=_[{]}\\|;:'\",<.>/?`~", c);
}

S32 beaconEnterString(char* buffer, S32 maxLength){
    S32 length = 0;
    
    buffer[length] = 0;
    
    while(1){
        S32 key = _getch();

        switch(key){
            xcase 0:
            case 224:{
                _getch();
            }
            
            xcase 27:{
                while(length > 0){
                    backSpace(1, 1);
                    buffer[--length] = 0;
                }
                return 0;
            }
            
            xcase 13:{
                return 1;
            }
            
            xcase 8:{
                if(length > 0){
                    backSpace(1, 1);
                    buffer[--length] = 0;
                }
            }
            
            xcase 22:{
                if(OpenClipboard(beaconGetConsoleWindow())){
                    HANDLE handle = GetClipboardData(CF_TEXT);

                    if(handle){
                        char* data = GlobalLock(handle);
                        
                        if(data){
                            while(    *data &&
                                    length < maxLength &&
                                    beaconIsGoodStringChar(*data))
                            {
                                buffer[length++] = *data;
                                buffer[length] = 0;
                                printf("%c", *data);
                                
                                data++;
                            }
                            
                            GlobalUnlock(handle);
                        }
                    }

                    CloseClipboard();
                }
            }
            
            xdefault:{
                if(    length < maxLength &&
                    beaconIsGoodStringChar(key))
                {
                    buffer[length++] = key;
                    buffer[length] = 0;
                    printf("%c", key);
                }
            }
        }
    }
}

U32 beaconGetCurTime(){
    return timerSecondsSince2000();
}

U32 beaconTimeSince(U32 startTime){
    return beaconGetCurTime() - startTime;
}

#define START_BYTES(pak) {S32 serverStartBits = pak->stream.bitLength;
#define STOP_BYTES(pak, var) beacon_common.sent.var.hitCount++;beacon_common.sent.var.bitCount += pak->stream.bitLength - serverStartBits;}

// Def index table.

static StashTable defNameHashTable;
static StashTable defUsedHashTable;

static void beaconAddGroupIndex(GroupDef* def, S32 index){
    if(!defNameHashTable){
        defNameHashTable = stashTableCreateWithStringKeys(2048, StashDeepCopyKeys);
    }
    stashAddInt(defNameHashTable, def->name, index, false);
}

static void beaconAddUsedDefAndChildren(GroupDef* def){
    S32 i;
    
    if(    !def ||
        def->no_coll)
    {
        return;
    }
    
    if(!defUsedHashTable){
        defUsedHashTable = stashTableCreateWithStringKeys(2048, StashDeepCopyKeys);
    }
    
    if(stashFindElement(defUsedHashTable, def->name, NULL)){
        return;
    }
    
    stashAddInt(defUsedHashTable, def->name, 0, false);
    
    for(i = 0; i < def->count; i++){
        beaconAddUsedDefAndChildren(def->entries[i].def);
    }
}

static S32 beaconGetGroupIndex(GroupDef* def){
    S32 ret = -1;
    StashElement element = NULL;
    if(defNameHashTable){
        stashFindElement(defNameHashTable, def->name, &element);
    }
    if(element)
        ret = (S32)(intptr_t)stashElementGetPointer(element);
    return ret;
}

static void sendPackData(Packet* pak, PackData* data){
    pktSendBitsPack(pak, 1, data->packsize);
    pktSendBitsPack(pak, 1, data->unpacksize);
    
    if(data->packsize || data->unpacksize){
        S32 size = data->packsize ? data->packsize : data->unpacksize;

        pktSendBitsArray(pak, 8 * size, data->data);
        
        //printf("send: %d bytes\n", data->packsize);
    }    
}

static void beaconSendModelInfo(Packet* pak, Model* model, S32 forceLargeModelData){
    S32 i;
    S32 start;
    S32 curFlags;
    //U8    curSurface;
    S32    tri_count = model->tri_count;
    
    modelCreateCtris(model);
    
    //printf(    "%5d tris, %5db/%5db grid, %5db/%5db tris, %5db/%5db verts: %s\n",
    //        model->tri_count,
    //        model->pack.grid.packsize,
    //        model->pack.grid.unpacksize,
    //        model->pack.tris.packsize,
    //        model->pack.tris.unpacksize,
    //        model->pack.verts.packsize,
    //        model->pack.verts.unpacksize,
    //        model->name); 

    pktSendString(pak, model->name);
    
    pktSendBitsPack(pak, 1, model->tri_count);
    pktSendBitsPack(pak, 1, model->vert_count);

    if(!forceLargeModelData && model->gld->geo_data){
        pktSendBits(pak, 1, 1);
        
        sendPackData(pak, &model->pack.grid);
        sendPackData(pak, &model->pack.tris);
        sendPackData(pak, &model->pack.verts);

        for(start = -1, i = 0; i <= tri_count; i++){
            CTri* tri = model->ctris + i;
            
            if(start < 0 || i == tri_count || curFlags != tri->flags){
                if(start >= 0){
                    pktSendBits(pak, 32, curFlags);
                    pktSendBitsPack(pak, 1, i - start);
                    
                    //printf("send %d/%d\n", i - start, tri_count);
                    
                    if(i == tri_count){
                        break;
                    }
                }
                
                start = i;
                curFlags = tri->flags;
            }
        }
    }else{
        static U8*    dataStaticBuffer;
        static U32    dataStaticBufferSize;
        
        U8            dataStackBuffer[10 * 1000];
        U32            dataSize = model->pack.grid.unpacksize;
        PolyCell*    tempCell;
        
        modelCreateCtris(model);
        
        if(dataSize <= sizeof(dataStackBuffer)){
            tempCell = (PolyCell*)dataStackBuffer;
        }else{
            dynArrayFitStructs(&dataStaticBuffer, &dataStaticBufferSize, dataSize);
            tempCell = (PolyCell*)dataStaticBuffer;
        }

        memset(tempCell, 0, dataSize);
        
        pktSendBits(pak, 1, 0);
        
        pktSendBitsPack(pak, 1, model->pack.grid.unpacksize);
        
        polyCellPack(model, model->grid.cell, NULL, tempCell, NULL);
        //polyCellUnpack(model, tempCell, NULL);
        //polyCellPack(model, tempCell, NULL, NULL, NULL);
        pktSendBitsArray(pak, 8 * model->pack.grid.unpacksize, tempCell);

        pktSendBitsArray(pak, 8 * model->tri_count * sizeof(model->ctris[0]), model->ctris);
    }
    
    pktSendBitsArray(pak, 8 * sizeof(model->grid.pos), &model->grid.pos);
    pktSendF32(pak, model->grid.size);
}

// Mat3 index table.

static StashTable mat3HashTable;

MP_DEFINE(Mat3);

static S32 beaconGetMat3Index(Mat3 mat){
    S32 iResult;

    if (mat3HashTable && stashFindInt(mat3HashTable, mat, &iResult))
        return iResult;
    return -1;
}

static void beaconAddMat3Index(Mat3 mat){
    Vec3* newMat;
    
    MP_CREATE(Mat3, 1000);
    
    if(!mat3HashTable){
        mat3HashTable = stashTableCreate(1024, StashDefault, StashKeyTypeFixedSize, sizeof(Mat3));
    }
    
    newMat = (Vec3*)MP_ALLOC(Mat3);
    copyMat3(mat, newMat);
    
    stashAddInt(mat3HashTable, newMat, stashGetValidElementCount(mat3HashTable), false);
}

static void destroyMat3(Mat3 mat){
    MP_FREE(Mat3, (Mat3*)mat);
}

// Vec3 index table.

static StashTable vec3HashTable;

MP_DEFINE(Vec3);

static S32 beaconGetVec3Index(Vec3 vec){
    S32 iResult;

    if (vec3HashTable && stashFindInt(vec3HashTable, vec, &iResult))
        return iResult;
    return -1;
}

static void beaconAddVec3Index(Vec3 vec){
    F32* newVec;
    
    MP_CREATE(Vec3, 1000);
    
    if(!vec3HashTable){
        vec3HashTable = stashTableCreate(1024, StashDefault, StashKeyTypeFixedSize, sizeof(Vec3));
    }
    
    newVec = (F32*)MP_ALLOC(Vec3);
    copyVec3(vec, newVec);
    
    stashAddInt(vec3HashTable, newVec, stashGetValidElementCount(vec3HashTable), false);
}

static void destroyVec3(Vec3 vec){
    MP_FREE(Vec3, (Vec3*)vec);
}

static void sendVec3Index(Packet* pak, Vec3 vec){
    S32 vecIndex = beaconGetVec3Index(vec);
    
    if(vecIndex < 0){
        START_BYTES(pak);
        pktSendBits(pak, 1, 1);
        beaconAddVec3Index(vec);
        pktSendBitsArray(pak, 8 * sizeof(Vec3), vec);
        STOP_BYTES(pak, fullVec3);
    }else{
        assert(vecIndex < (S32)stashGetValidElementCount(vec3HashTable));
        START_BYTES(pak);
        pktSendBits(pak, 1, 0);
        pktSendBitsPack(pak, 5, vecIndex);
        STOP_BYTES(pak, indexVec3);
    }
}

// Model index table.

static StashTable modelNameHashTable;

static void beaconAddModelIndex(Model* model){
    SentModelInfo* info = createSentModelInfo();
    
    if(!modelNameHashTable){
        modelNameHashTable = stashTableCreateWithStringKeys(2048, StashDeepCopyKeys);
    }
    
    info->index = stashGetValidElementCount(modelNameHashTable);
    info->sentModel = 0;
    
    stashAddPointer(modelNameHashTable, model->name, info, false);
}

static SentModelInfo* beaconGetModelInfo(Model* model){
    StashElement element = NULL;
    if (modelNameHashTable) stashFindElement(modelNameHashTable, model->name, &element);
    
    if(!element){
        return NULL;
    }else{
        return stashElementGetPointer(element);
    }
}

static S32 beaconGetModelIndex(Model* model){
    SentModelInfo* info = beaconGetModelInfo(model);

    if(!info){
        return -1;
    }else{
        assert(info->index >= 0);

        return info->index;
    }
}

static void beaconSendRelevantModels(Packet* pak, GroupDef* def, S32 forceLargeModelData){
    S32 i;
    
    if(    !def ||
        def->no_coll)
    {
        return;
    }
        
    if(def->model && def->model->grid.size){
        SentModelInfo* info = beaconGetModelInfo(def->model);
        
        if(!info->sentModel){
            info->sentModel = 1;
            
            START_BYTES(pak);
            {
                U32 start = pak->stream.bitLength;
                beaconSendModelInfo(pak, def->model, forceLargeModelData);
                //printf("%-50s %d\n", def->model->name, pak->stream.bitLength - start);
            }
            STOP_BYTES(pak, modelData);
        }
    }
    
    for(i = 0; i < def->count; i++){
        GroupEnt* ent = def->entries + i;
        
        beaconSendRelevantModels(pak, ent->def, forceLargeModelData);
    }
}

static S32 maxGroupDefIndex;

static void beaconSendGroupFileStep1(Packet* pak, GroupFile* file){
    S32 i;
    
    pktSendBitsPack(pak, 1, file->count);

    for(i = 0; i < file->count; i++){
        GroupDef* def = file->defs[i];
        
        if(    !defUsedHashTable ||
            !stashFindElement(defUsedHashTable, def->name, NULL))
        {
            pktSendBits(pak, 1, 0);
            continue;
        }
        
        START_BYTES(pak);

        pktSendBits(pak, 1, 1);
        
        START_BYTES(pak);
        pktSendString(pak, def->name);
        STOP_BYTES(pak, defNames);
        
        START_BYTES(pak);
        sendVec3Index(pak, def->min);
        sendVec3Index(pak, def->mid);
        sendVec3Index(pak, def->max);
        pktSendF32(pak, def->radius);
        STOP_BYTES(pak, defDimensions);
        
        beaconAddGroupIndex(def, maxGroupDefIndex++);
        
        modelCreateCtris(def->model);
        
        if(    def->model &&
            def->model->grid.size)
        {
            Model* model = def->model;
            S32 index = beaconGetModelIndex(model);
            
            pktSendBits(pak, 1, 1);
            
            if(index < 0){
                START_BYTES(pak);
                pktSendBits(pak, 1, 1);
                pktSendBits(pak, 1, def->no_beacon_ground_connections);
                pktSendIfSetBits(pak, 32, def->model->ctriflags_setonall);
                pktSendIfSetBits(pak, 32, def->model->ctriflags_setonsome);
                //printf("%s: %d %d %d\n", def->model->name, def->no_beacon_ground_connections, def->model->ctriflags_setonall, def->model->ctriflags_setonsome);
                beaconAddModelIndex(model);
                STOP_BYTES(pak, modelInfo);
            }else{
                pktSendBits(pak, 1, 0);
                pktSendBitsPack(pak, 1, index);
            }
        }else{
            pktSendBits(pak, 1, 0);
        }
        
        STOP_BYTES(pak, defs);
    }
}

static S32 beaconDefIsNoColl(GroupDef* def){
    return def->no_coll;
}

static S32 beaconCountNonNoCollChildren(GroupDef* def){
    S32 count = 0;
    S32 i;
    
    for(i = 0; i < def->count; i++){
        GroupEnt* ent = def->entries + i;
        
        if(!beaconDefIsNoColl(ent->def)){
            count++;
        }
    }
    
    return count;
}

static int beaconSendGroupFileStep2(Packet* pak, GroupFile* file){
    S32 i;
    
    for(i = 0; i < file->count; i++){
        GroupDef* def = file->defs[i];
        S32 j;
        S32 nonNoCollCount;
        S32 sentCount = 0;
        
        if(    !defUsedHashTable ||
            !stashFindElement(defUsedHashTable, def->name, NULL))
        {
            continue;
        }
        
        START_BYTES(pak);
        
        nonNoCollCount = beaconCountNonNoCollChildren(def);
        
        pktSendBitsPack(pak, 3, nonNoCollCount);
        
        for(j = 0; j < def->count; j++){
            GroupEnt* ent = def->entries + j;
            
            if(!beaconDefIsNoColl(ent->def)){
                S32 defIndex = beaconGetGroupIndex(ent->def);
                S32 matIndex = beaconGetMat3Index(ent->mat);

                sentCount++;
                
                if (!devassert(defIndex >= 0 && defIndex < maxGroupDefIndex))
                    return 0;
                
                pktSendBitsPack(pak, 5, defIndex);
                
                if(matIndex < 0){
                    START_BYTES(pak);
                    pktSendBits(pak, 1, 1);
                    beaconAddMat3Index(ent->mat);
                    pktSendBitsArray(pak, 8 * sizeof(Mat3), ent->mat);
                    STOP_BYTES(pak, fullDefChild);
                }else{
                    assert(matIndex < (S32)stashGetValidElementCount(mat3HashTable));
                    START_BYTES(pak);
                    pktSendBits(pak, 1, 0);
                    pktSendBitsPack(pak, 5, matIndex);
                    STOP_BYTES(pak, indexDefChild);
                }

                sendVec3Index(pak, ent->mat[3]);
            }
        }
        
        assert(nonNoCollCount == sentCount);
        
        STOP_BYTES(pak, defs2);
    }

    return 1;
}

void beaconMapDataPacketCreate(BeaconMapDataPacket** mapData){
    *mapData = calloc(1, sizeof(BeaconMapDataPacket));
}

void beaconMapDataPacketDestroy(BeaconMapDataPacket** mapData){
    if(!mapData || !*mapData){
        return;
    }

    SAFE_FREE((*mapData)->compressed.data);
    ZeroStruct(*mapData);
    SAFE_FREE(*mapData);
}

typedef struct InitialBeacon {
    Vec3    pos;
    U32        isValid : 1;
} InitialBeacon;

struct {
    InitialBeacon*    beacons;
    S32                count;
    S32                maxCount;
} initial_beacons;

void beaconMapDataPacketClearInitialBeacons(){
    SAFE_FREE(initial_beacons.beacons);
    ZeroStruct(&initial_beacons);
}

void beaconMapDataPacketAddInitialBeacon(Vec3 pos, S32 isValidStartingPoint){
    InitialBeacon* beacon = dynArrayAddStruct(    &initial_beacons.beacons,
                                                &initial_beacons.count,
                                                &initial_beacons.maxCount);
                                        
    copyVec3(pos, beacon->pos);
    beacon->isValid = isValidStartingPoint ? 1 : 0;
}

void beaconMapDataPacketInitialBeaconsToRealBeacons(){
    S32 i;
    
    for(i = 0; i < initial_beacons.count; i++){
        InitialBeacon* initialBeacon = initial_beacons.beacons + i;
        Beacon* beacon = addCombatBeacon(initialBeacon->pos, 1, 1, 1);
        
        if(beacon){
            beacon->isValidStartingPoint = initialBeacon->isValid ? 1 : 0;
        }
    }
}

static void beaconSendDefInstance(Packet* pak, GroupDef* def, const Mat4 mat, S32 forceLargeModelData){
    if(    def &&
        !def->no_coll)
    {
        S32 defIndex = beaconGetGroupIndex(def);
        
        pktSendBits(pak, 1, 1);
        
        assert(defIndex >= 0 && defIndex < maxGroupDefIndex);
    
        pktSendBitsPack(pak, 1, defIndex);
        
        pktSendBitsArray(pak, 8 * sizeof(Mat4), mat);
        
        //printf("\n\n");
        
        beaconSendRelevantModels(pak, def, forceLargeModelData);
    }else{
        pktSendBits(pak, 1, 0);
    }
}

typedef void (*DetailEntityFunction)(Packet* pak, GroupDef* def, const Mat4 mat, void* userData);

static void forEachDetailEntity(DetailEntityFunction func, Packet* pak, void* userData){
    S32 i;
    
    for(i = 0; i < entities_max; i++){
        Entity* e = validEntFromId(i);
        RoomDetail* detail;
        GroupDef* def;
        
        if(!e || !e->idDetail){
            continue;
        }
        
        detail = baseGetDetail(&g_base, e->idDetail, NULL);
        
        if(    !detail ||
            !detail->info ||
            !detail->info->pchGroupName)
        {
            continue;
        }
        
        def = groupDefFindWithLoad(detail->info->pchGroupName);
        
        if(def){
            func(pak, def, ENTMAT(e), userData);
        }
    }
}

static void countDetailEntities(Packet* pak, GroupDef* def, const Mat4 mat, S32* entityCollisionCount){
    beaconAddUsedDefAndChildren(def);
    
    (*entityCollisionCount)++;
}

static void sendDetailEntities(Packet* pak, GroupDef* def, const Mat4 mat, void* forceLargeModelData){
    beaconSendDefInstance(pak, def, mat, forceLargeModelData ? 1 : 0);
}

static void destroySentModelInfoAdapter(void* arg0)
{
    destroySentModelInfo((SentModelInfo*)arg0);
}

static void destroyMat3Adapter(void* arg0)
{
    destroyMat3((Mat3Ptr)arg0);
}

static void destroyVec3Adapter(void* arg0)
{
    destroyVec3((F32*)arg0);
}

void beaconMapDataPacketFromMapData(BeaconMapDataPacket** mapDataIn, S32 forceLargeModelData){
    #define WRITE_CHECK(s) pktSendString(pak, s);
    
    BeaconMapDataPacket* mapData;
    Packet* pak = pktCreate();
    S32 i;
    int failed = 0;

    S32 entityCollisionCount = 0;
    
    pak->hasDebugInfo = 0;
    
    beaconMapDataPacketDestroy(mapDataIn);
    beaconMapDataPacketCreate(mapDataIn);
    mapData = *mapDataIn;
    
    ZeroStruct(&beacon_common.sent);
    
    //printf("Sending: %s\n", beaconCurTimeString(0));
    
    if (defNameHashTable)
        stashTableClear(defNameHashTable);
    if (defUsedHashTable)
        stashTableClear(defUsedHashTable);
    if (modelNameHashTable)
        stashTableClearEx(modelNameHashTable, NULL, destroySentModelInfoAdapter);
    if (mat3HashTable)
        stashTableClearEx(mat3HashTable, destroyMat3Adapter, NULL);
    if (vec3HashTable)
        stashTableClearEx(vec3HashTable, destroyVec3Adapter, NULL);
    
    // Mark the defs that are actually used.

    for(i = 0; i < group_info.ref_count; i++){
        DefTracker* ref = group_info.refs[i];
        GroupDef* def = ref->def;
        
        beaconAddUsedDefAndChildren(def);
    }    
    
    // Send the MapDataPacket version.
    
    pktSendBitsPack(pak, 1, 0);
    
    // Send scene info.
    
    WRITE_CHECK("scene");
    
    pktSendF32(pak, scene_info.maxHeight);
    pktSendF32(pak, scene_info.minHeight);
    
    // Send the entities that have collisions.
    
    forEachDetailEntity(countDetailEntities, pak, &entityCollisionCount);
    
    // Send all the defs and models.
    
    WRITE_CHECK("groupfile1");

    pktSendBitsPack(pak, 1, group_info.file_count);
    
    maxGroupDefIndex = 0;
    
    for(i = 0; i < group_info.file_count; i++){
        beaconSendGroupFileStep1(pak, group_info.files[i]);
    }
    
    //beaconClientPrintf(    client, 0,
    //                "Sent map data (%s defs, %s refs, %s bytes).\n",
    //                getCommaSeparatedInt(group_info.def_count),
    //                getCommaSeparatedInt(group_info.ref_count),
    //                getCommaSeparatedInt(pak->stream.size));
    
    WRITE_CHECK("groupfile2");

    for(i = 0; i < group_info.file_count; i++){
        if(!beaconSendGroupFileStep2(pak, group_info.files[i])){
            failed = 1;
            break;
        }
    }

    // Something went wrong with building the data so we can't use it and
    // the request can't be sent.
    if(failed){
        pktDestroy(pak);
        *mapDataIn = NULL;

        return;
    }
    //beaconClientPrintf(    client, 0,
    //                "Sent map data (%s defs, %s refs, %s bytes).\n",
    //                getCommaSeparatedInt(group_info.def_count),
    //                getCommaSeparatedInt(group_info.ref_count),
    //                getCommaSeparatedInt(pak->stream.size));
    
    // Send the refs (should be small).
    
    WRITE_CHECK("refs");

    pktSendBitsPack(pak, 1, group_info.ref_count + entityCollisionCount);
    
    for(i = 0; i < group_info.ref_count; i++){
        DefTracker* ref = group_info.refs[i];
        GroupDef* def = ref->def;
        
        beaconSendDefInstance(pak, def, ref->mat, forceLargeModelData);
    }
    
    if(entityCollisionCount){
        forEachDetailEntity(sendDetailEntities, pak, (void*)(intptr_t)forceLargeModelData);
    }

    // Tack on the initial beacon list.
    
    WRITE_CHECK("beacons");

    pktSendBitsPack(pak, 1, initial_beacons.count);
    
    for(i = 0; i < initial_beacons.count; i++){
        InitialBeacon* beacon = initial_beacons.beacons + i;
        
        pktSendF32(pak, beacon->pos[0]);
        pktSendF32(pak, beacon->pos[1]);
        pktSendF32(pak, beacon->pos[2]);
        
        pktSendBits(pak, 1, beacon->isValid);
    }        
    
    WRITE_CHECK("end");

    {
        U32 packetByteCount = (pak->stream.bitLength + 7) / 8;
        U32 ret;
        
        mapData->compressed.byteCount = packetByteCount * 1.0125 + 12;

        SAFE_FREE(mapData->compressed.data);
        
        mapData->compressed.data = malloc(mapData->compressed.byteCount);
                    
        ret = compress2(mapData->compressed.data,
                        &mapData->compressed.byteCount,
                        pak->stream.data,
                        packetByteCount,
                        5);
                        
        if(0){
            // Malform the packet to test beaconserver robustness.
        
            int i;
            for(i = 0; i < 100; i++){
                mapData->compressed.data[rand() % mapData->compressed.byteCount] = rand() % 256;
            }
        }
                        
        assert(ret == Z_OK);
    }
    
    mapData->uncompressedBitCount = pak->stream.bitLength;
    mapData->packetDebugInfo = pak->hasDebugInfo;
    
    mapData->compressed.crc = freshCRC(mapData->compressed.data, mapData->compressed.byteCount);

    if(0){
        beaconPrintf(    COLOR_GREEN,
                        "Created map data packet: %s defs, %s refs, %s bytes\n",
                        getCommaSeparatedInt(maxGroupDefIndex),
                        getCommaSeparatedInt(group_info.ref_count),
                        getCommaSeparatedInt((pak->stream.bitLength + 7) / 8));

        #define PRINT(x)                                                                \
            beaconPrintf(    COLOR_GREEN,                                                \
                            "  %-20s%s.%db/%d\n", #x":",                                \
                            getCommaSeparatedInt(beacon_common.sent.x.bitCount / 8),    \
                            beacon_common.sent.x.bitCount % 8,                            \
                            beacon_common.sent.x.hitCount)

        PRINT(defNames);
        PRINT(defDimensions);
        PRINT(modelData);
        PRINT(defs);
        PRINT(fullVec3);
        PRINT(indexVec3);
        PRINT(modelInfo);
        PRINT(fullDefChild);
        PRINT(indexDefChild);
        PRINT(defs2);
        
        #undef PRINT
    
        beaconPrintf(    COLOR_GREEN,
                        "  initial beacons:    %d\n"
                        "  crc:                0x%8.8x).\n",
                        initial_beacons.count,
                        mapData->compressed.crc);
    }
                
    pktDestroy(pak);
}

U32 beaconMapDataPacketGetCRC(BeaconMapDataPacket* mapData){
    if(!mapData){
        return 0;
    }
    
    return mapData->compressed.crc;
}

S32 beaconMapDataPacketIsSame(BeaconMapDataPacket* mapData1, BeaconMapDataPacket* mapData2){
    if((mapData1 ? 1 : 0) != (mapData2 ? 1 : 0)){
        return 0;
    }
    
    if(!mapData1){
        // And by implication, !mapData2.
        return 0;
    }
    
    if(    mapData1->compressed.byteCount != mapData2->compressed.byteCount ||
        mapData1->uncompressedBitCount != mapData2->uncompressedBitCount ||
        memcmp(mapData1->compressed.data, mapData2->compressed.data, mapData1->compressed.byteCount))
    {
        return 0;
    }
    
    return 1;
}

static void receivePackData(Packet* pak, PackData* data){
    ZeroStruct(data);
    
    data->packsize = pktGetBitsPack(pak, 1);
    data->unpacksize = pktGetBitsPack(pak, 1);
    
    if(data->packsize || data->unpacksize){
        S32 size = data->packsize ? data->packsize : data->unpacksize;
        data->data = beaconMemAlloc("packData", size);
        pktGetBitsArray(pak, 8 * size, data->data);
        
        //printf("send: %d bytes\n", data->packsize);
    }    
}

static void beaconCreateCommonModelData(Model* model){
    model->tags = beaconMemAlloc("tags", model->tri_count * sizeof(model->tags[0]));
    model->ctris = beaconMemAlloc("ctris", model->tri_count * sizeof(model->ctris[0]));
}

S32 beaconCreateCtris(Model* model)
{
    PolyCell *polyCellUnpack(Model* model,PolyCell *cell,void *base_offset);

    Vec3    vert_buffer[1000];
    S32        tri_buffer[1000 * 3];
    
    S32        i,*tri,base=0;
    Vec3    *verts;
    S32        *tris;

    if (!model->tri_count || model->tags || !model->pack.grid.unpacksize)
        return 0;

    model->grid.cell = beaconMemAlloc("gridCell", model->pack.grid.unpacksize);
    uncompress((U8*)model->grid.cell,&model->pack.grid.unpacksize,model->pack.grid.data,model->pack.grid.packsize);
    model->grid.cell = polyCellUnpack(model, model->grid.cell, (void*)model->grid.cell);

    if(model->vert_count <= ARRAY_SIZE(vert_buffer))
        verts = vert_buffer;
    else
        verts = beaconMemAlloc("verts", model->vert_count * sizeof(verts[0]));
        
    if(model->tri_count * 3 <= ARRAY_SIZE(tri_buffer))
        tris = tri_buffer;
    else
        tris = beaconMemAlloc("tris", model->tri_count * 3 * sizeof(tris[0]));

    geoUnpackDeltas(&model->pack.tris,tris,3,model->tri_count,PACK_U32,model->name,model->filename);
    geoUnpackDeltas(&model->pack.verts,verts,3,model->vert_count,PACK_F32,model->name,model->filename);

    beaconCreateCommonModelData(model);

    for(i=0;i<model->tri_count;i++){
        tri = &tris[i * 3];
        model->ctris[i].flags = 0;

        if (createCTri(&model->ctris[i],verts[tri[0]],verts[tri[1]],verts[tri[2]])
            && model->ctris[i].scale)
        {
            model->ctris[i].flags |= COLL_NORMALTRI;
            {
                Vec3    tverts[3];
                S32        j;
                F32        d;

                expandCtriVerts(&model->ctris[i],tverts);

                for(j=0;j<3;j++)
                {
                    d = distance3(tverts[j],verts[tri[j]]);
                }
            }
        }
    }

    if(verts != vert_buffer)
        beaconMemFree((void**)&verts);
    if(tris != tri_buffer)
        beaconMemFree((void**)&tris);
        
    return 1;
}

static void beaconReceiveModelInfo(Packet* pak, Model* model){
    S32 start;
    S32    tri_count;
    
    model->name = beaconStrdup(pktGetString(pak));
    
    model->tri_count = tri_count = pktGetBitsPack(pak, 1);

    model->vert_count = pktGetBitsPack(pak, 1);

    if(pktGetBits(pak, 1)){
        receivePackData(pak, &model->pack.grid);
        receivePackData(pak, &model->pack.tris);
        receivePackData(pak, &model->pack.verts);
        
        #if 0
            printf(    "%5d tris, %5db/%5db grid, %5db/%5db tris, %5db/%5db verts: %s\n",
                    model->tri_count,
                    model->pack.grid.packsize,
                    model->pack.grid.unpacksize,
                    model->pack.tris.packsize,
                    model->pack.tris.unpacksize,
                    model->pack.verts.packsize,
                    model->pack.verts.unpacksize,
                    model->name); 
        #endif
        
        beaconCreateCtris(model);
        
        for(start = 0; start < tri_count;){
            S32 flags = pktGetBits(pak, 32);
            S32 count = pktGetBitsPack(pak, 1);

            while(count--){
                model->ctris[start++].flags = flags;
            }
        }
    }else{
        beaconCreateCommonModelData(model);

        model->pack.grid.unpacksize = pktGetBitsPack(pak, 1);
        
        model->grid.cell = beaconMemAlloc("gridCell", model->pack.grid.unpacksize);

        pktGetBitsArray(pak, 8 * model->pack.grid.unpacksize, model->grid.cell);
        polyCellUnpack(model, model->grid.cell, model->grid.cell);

        pktGetBitsArray(pak, 8 * model->tri_count * sizeof(model->ctris[0]), model->ctris);
    }

    pktGetBitsArray(pak, 8 * sizeof(model->grid.pos), &model->grid.pos);
    model->grid.size = pktGetF32(pak);
}

static struct {
    SentModelInfo**    models;
    S32                count;
    S32                maxCount;
} models;

static struct {
    Mat3*        mat3s;
    S32            count;
    S32            maxCount;
} mat3s;

static struct {
    Vec3*        vec3s;
    S32            count;
    S32            maxCount;
} vec3s;

StashTable modelInfoHashTable;

static void receiveVec3Index(Packet* pak, Vec3 vec){
    if(pktGetBits(pak, 1)){
        // New Vec3.
        
        F32* newVec = dynArrayAddStruct(&vec3s.vec3s, &vec3s.count, &vec3s.maxCount);
        
        pktGetBitsArray(pak, 8 * sizeof(Vec3), newVec);

        copyVec3(newVec, vec);
    }else{
        S32 index = pktGetBitsPack(pak, 5);
        
        assert(index >= 0 && index < vec3s.count);
        
        copyVec3(vec3s.vec3s[index], vec);
    }
}

static void beaconReceiveRelevantModels(Packet* pak, GroupDef* def){
    S32 i;
    
    if(!def)
        return;
    
    if(def->model){
        SentModelInfo* info;
        bool bFound;
        bFound = stashAddressFindPointer(modelInfoHashTable, def->model, &info);
        
        assert(bFound && info);
        
        if(!info->sentModel){
            info->sentModel = 1;

            beaconReceiveModelInfo(pak, def->model);
        }
    }
    
    for(i = 0; i < def->count; i++){
        GroupEnt* ent = def->entries + i;
        
        beaconReceiveRelevantModels(pak, ent->def);
    }
}

static void beaconDestroyModel(Model* model){
    beaconMemFree(&model->grid.cell);
    beaconMemFree(&model->tags);
    beaconMemFree(&model->ctris);
    beaconMemFree(&model->extra);

    beaconMemFree(&model->pack.grid.data);
    beaconMemFree(&model->pack.tris.data);
    beaconMemFree(&model->pack.verts.data);
    
    beaconMemFree(&model->name);
    
    beaconMemFree(&model);
}

static void destroySentModelInfoHelper(SentModelInfo* info){
    beaconDestroyModel(info->model);

    destroySentModelInfo(info);
}

static struct {
    GroupDef**    defs;
    S32            count;
    S32            maxCount;
} groupDefs;

static void beaconReceiveGroupFileStep1(Packet* pak, GroupFile* file){
    S32 i;
    
    file->count = pktGetBitsPack(pak, 1);
    
    file->defs = beaconMemAlloc("defs", file->count * sizeof(*file->defs));
    
    for(i = 0; i < file->count; i++){
        GroupDef* def;
        
        if(!pktGetBits(pak, 1)){
            continue;
        }
    
        def = file->defs[i] = beaconMemAlloc("def", sizeof(*def));
        
        dynArrayAddp(&groupDefs.defs, &groupDefs.count, &groupDefs.maxCount, def);
        
        //if(i % 100 == 0){
        //    printf("%d : ------------------------------------------------------------------------\n", i);
        //}

        def->name = beaconStrdup(pktGetString(pak));
        
        receiveVec3Index(pak, def->min);
        receiveVec3Index(pak, def->mid);
        receiveVec3Index(pak, def->max);
        def->radius = pktGetF32(pak);
        
        if(pktGetBits(pak, 1)){
            if(pktGetBits(pak, 1)){
                // New model.
                
                SentModelInfo* info = createSentModelInfo();
                
                def->no_beacon_ground_connections = pktGetBits(pak, 1);

                def->model = info->model = beaconMemAlloc("model", sizeof(*info->model));
                
                def->model->ctriflags_setonall = pktGetIfSetBits(pak, 32);
                def->model->ctriflags_setonsome = pktGetIfSetBits(pak, 32);
                
                if(!modelInfoHashTable){
                    modelInfoHashTable = stashTableCreateAddress(1024);
                }
                
                stashAddressAddPointer(modelInfoHashTable, info->model, info, false);
                
                //printf("%5d: ", i);

                dynArrayAddp(&models.models, &models.count, &models.maxCount, info);
            }else{
                // Old model.
                
                S32 index = pktGetBitsPack(pak, 1);
                
                assert(index >= 0 && index < models.count);
                
                def->model = models.models[index]->model;
            }
        }else{
            def->model = NULL;
        }
    }
}

static void beaconReceiveGroupFileStep2(Packet* pak, GroupFile* file){
    S32 i;
    
    for(i = 0; i < file->count; i++){
        GroupDef* def = file->defs[i];
        
        if(!def){
            continue;
        }
        
        def->count = pktGetBitsPack(pak, 3);
        
        if(def->count){
            S32 j;
            
            def->entries = beaconMemAlloc("entries", sizeof(*def->entries) * def->count);
            
            for(j = 0; j < def->count; j++){
                GroupEnt* ent = def->entries + j;
                S32 index;
                
                // Receive the def.
                
                index = pktGetBitsPack(pak, 5);

                assert(index >= 0 && index < groupDefs.count);
                
                ent->def = groupDefs.defs[index];
                
                ent->mat = beaconMemAlloc("mat4", sizeof(Mat4));
                
                // Get the Mat3.
                
                if(pktGetBits(pak, 1)){
                    // New Mat3.
                    
                    Vec3* mat = dynArrayAddStruct(&mat3s.mat3s, &mat3s.count, &mat3s.maxCount);
                    
                    pktGetBitsArray(pak, 8 * sizeof(Mat3), mat);
                    
                    copyMat3(mat, ent->mat);
                }else{
                    index = pktGetBitsPack(pak, 5);
                    
                    assert(index >= 0 && index < mat3s.count);
                    
                    copyMat3(mat3s.mat3s[index], ent->mat);
                }
                
                receiveVec3Index(pak, ent->mat[3]);
            }
        }
    }
}

static void destroySentModelInfoHelperAdapter(void* arg0)
{
    destroySentModelInfoHelper((SentModelInfo*)arg0);
}

void beaconResetReceivedMapData()
{
    if (modelInfoHashTable)
        stashTableClearEx(modelInfoHashTable, NULL, destroySentModelInfoHelperAdapter);

    SAFE_FREE(vec3s.vec3s);
    ZeroStruct(&vec3s);
    
    SAFE_FREE(mat3s.mat3s);
    ZeroStruct(&mat3s);
    
    SAFE_FREE(models.models);
    ZeroStruct(&models);
    
    SAFE_FREE(groupDefs.defs);
    ZeroStruct(&groupDefs);

    beaconResetMapData();
}

void beaconMapDataPacketSendChunk(Packet* pak, BeaconMapDataPacket* mapData, U32* sentByteCount){
    S32 byteCountRemaining = mapData->compressed.byteCount - *sentByteCount;
    S32 byteCountToSend = min(byteCountRemaining, 64 * 1024);
    
    assert(byteCountToSend >= 0);
    
    if(*sentByteCount == 0){
        // If this is the first packet, then send a single byte to
        //   start the send but let the server throttle the receive.
    
        byteCountToSend = min(1, byteCountToSend);
    }
    
    pktSendBitsPack(pak, 1, *sentByteCount);
    pktSendBitsPack(pak, 1, byteCountToSend);
    pktSendBitsPack(pak, 1, mapData->compressed.byteCount);
    pktSendBitsPack(pak, 1, mapData->uncompressedBitCount);
    pktSendBits(pak, 1, mapData->packetDebugInfo ? 1 : 0);
    pktSendBits(pak, 32, mapData->compressed.crc);
    
    pktSendBitsArray(    pak,
                        8 * byteCountToSend,
                        mapData->compressed.data + *sentByteCount);
                        
    *sentByteCount += byteCountToSend;
}

S32 beaconMapDataPacketToMapData(BeaconMapDataPacket* mapData){
    #define READ_CHECK(s) {                                                                        \
        const char* rs = pktGetString(pak);                                                        \
        if(strcmp(rs, s)){                                                                        \
            beaconPrintf(COLOR_RED, "ERROR: READ_CHECK failed: \"%s\" != \"%s\".\n", rs, s);    \
            SAFE_FREE(pak->stream.data);                                                        \
            return 0;                                                                            \
        }                                                                                        \
    }
    
    Packet fakePacket = {0};
    Packet* pak = &fakePacket;
    S32 i;
    S32 ret;
    S32 uncompressedByteCount;
    S32 mapDataPacketVersion;
    U32 crc;
    
    // Check if the packet has been fully received.
    
    if(!beaconMapDataPacketIsFullyReceived(mapData)){
        beaconPrintf(    COLOR_RED,
                        "ERROR: Map data packet isn't fully received (%s/%s bytes).\n",
                        getCommaSeparatedInt(mapData ? mapData->receivedByteCount : 0),
                        getCommaSeparatedInt(mapData ? mapData->compressed.byteCount : 0));
                        
        return 0;
    }
    
    crc = freshCRC(mapData->compressed.data, mapData->compressed.byteCount);
    
    if(crc != mapData->compressed.crc){
        beaconPrintf(    COLOR_RED,
                        "ERROR: Map data packet CRC doesn't match: 0x%8.8x != 0x%8.8x.\n",
                        crc,
                        mapData->compressed.crc);

        return 0;
    }
    
    beacon_common.mapDataLoadedFromPacket = 1;
    
    beaconPrintf(    COLOR_GREEN,
                    "Decompressing map data (%s to %s bytes): %s\n",
                    getCommaSeparatedInt(mapData->compressed.byteCount),
                    getCommaSeparatedInt(mapData->uncompressedByteCount),
                    beaconCurTimeString(0));
                    
    pak->stream.data = malloc(mapData->uncompressedByteCount);
    pak->stream.mode = Read;
    pak->stream.size = uncompressedByteCount = mapData->uncompressedByteCount;
    pak->stream.bitLength = mapData->uncompressedBitCount;
    pak->hasDebugInfo = mapData->packetDebugInfo ? 1 : 0;

    ret = uncompress(    pak->stream.data,
                        &uncompressedByteCount,
                        mapData->compressed.data,
                        mapData->compressed.byteCount);
                        
    if(ret != Z_OK){
        beaconPrintf(COLOR_RED, "ERROR: Map data failed to decompress!\n");
        
        SAFE_FREE(pak->stream.data);
        
        return 0;
    }
    
    if(uncompressedByteCount != mapData->uncompressedByteCount){
        beaconPrintf(COLOR_RED, "ERROR: Map data decompressed to the wrong size!\n");
        
        SAFE_FREE(pak->stream.data);
        
        return 0;
    }

    if (modelInfoHashTable)
        stashTableClearEx(modelInfoHashTable, NULL, destroySentModelInfoHelperAdapter);
    
    // Get the MapDataPacket version.
    
    mapDataPacketVersion = pktGetBitsPack(pak, 1);
    
    // Get scene info.
    
    READ_CHECK("scene");
    
    scene_info.maxHeight = pktGetF32(pak);
    scene_info.minHeight = pktGetF32(pak);
    
    // Get all the defs and models.
    
    READ_CHECK("groupfile1");
    
    group_info.file_count = group_info.file_max = pktGetBitsPack(pak, 1);
    group_info.files = beaconMemAlloc("files", sizeof(group_info.files[0]) * group_info.file_max);
    
    models.count = 0;
    mat3s.count = 0;
    vec3s.count = 0;

    groupDefs.count= 0;
    
    for(i = 0; i < group_info.file_max; i++){
        group_info.files[i] = beaconMemAlloc("file", sizeof(*group_info.files[i]));
        beaconReceiveGroupFileStep1(pak, group_info.files[i]);
    }
    
    READ_CHECK("groupfile2");
    
    for(i = 0; i < group_info.file_max; i++){
        beaconReceiveGroupFileStep2(pak, group_info.files[i]);
    }

    // Get the refs (should be small).
    
    READ_CHECK("refs");
    
    group_info.ref_count = group_info.ref_max = pktGetBitsPack(pak, 1);
    group_info.refs = beaconMemAlloc("refs", sizeof(*group_info.refs) * group_info.ref_max);
    
    for(i = 0; i < group_info.ref_max; i++){
        DefTracker* ref = group_info.refs[i] = beaconMemAlloc("ref", sizeof(*ref));
        
        if(pktGetBits(pak, 1) == 1){
            S32 defIndex = pktGetBitsPack(pak, 1);
            
            assert(defIndex >= 0 && defIndex < groupDefs.count);

            ref->def = groupDefs.defs[defIndex];
            
            pktGetBitsArray(pak, 8 * sizeof(Mat4), ref->mat);
            
            beaconReceiveRelevantModels(pak, ref->def);
            
            groupRefActivate(ref);
        }
    }
    
    // Get the initial beacons.
    
    READ_CHECK("beacons");
    
    beaconMapDataPacketClearInitialBeacons();
    
    initial_beacons.count = pktGetBitsPack(pak, 1);
    
    dynArrayFitStructs(    &initial_beacons.beacons,
                        &initial_beacons.maxCount,
                        initial_beacons.count);
                
    for(i = 0; i < initial_beacons.count; i++){
        InitialBeacon* beacon = initial_beacons.beacons + i;
        
        beacon->pos[0] = pktGetF32(pak);
        beacon->pos[1] = pktGetF32(pak);
        beacon->pos[2] = pktGetF32(pak);
        beacon->isValid = pktGetBits(pak, 1);
    }
    
    READ_CHECK("end");
    
    //printf(    "Got map data (%s defs, %s refs).\n",
    //        getCommaSeparatedInt(groupDefs.count),
    //        getCommaSeparatedInt(group_info.ref_count));
            
    SAFE_FREE(pak->stream.data);

    return 1;
    
    #undef READ_CHECK
}

void beaconMapDataPacketReceiveChunkHeader(Packet* pak, BeaconMapDataPacket* mapData){
    mapData->header.previousSentByteCount = pktGetBitsPack(pak, 1);
    mapData->header.currentByteCount = pktGetBitsPack(pak, 1);

    mapData->compressed.byteCount = pktGetBitsPack(pak, 1);

    mapData->uncompressedBitCount = pktGetBitsPack(pak, 1);
    mapData->uncompressedByteCount = (mapData->uncompressedBitCount + 7) / 8;

    mapData->packetDebugInfo = pktGetBits(pak, 1);
    
    mapData->compressed.crc = pktGetBits(pak, 32);
}

S32 beaconMapDataPacketIsFirstChunk(BeaconMapDataPacket* mapData){
    return mapData ? !mapData->header.previousSentByteCount : 0;
}

void beaconMapDataPacketCopyHeader(BeaconMapDataPacket* to, const BeaconMapDataPacket* from){
    #define COPY_FIELD(x) to->x = from->x
    
    COPY_FIELD(header.previousSentByteCount);
    COPY_FIELD(header.currentByteCount);
    COPY_FIELD(compressed.byteCount);
    COPY_FIELD(uncompressedBitCount);
    COPY_FIELD(uncompressedByteCount);
    COPY_FIELD(packetDebugInfo);
    COPY_FIELD(compressed.crc);
    
    #undef COPY_FIELD
}

void beaconMapDataPacketReceiveChunkData(Packet* pak, BeaconMapDataPacket* mapData){
    if(!mapData->header.previousSentByteCount){
        SAFE_FREE(mapData->compressed.data);
        
        mapData->compressed.data = malloc(mapData->compressed.byteCount);
    }

    assert(    !mapData->header.previousSentByteCount ||
            mapData->receivedByteCount == mapData->header.previousSentByteCount);

    pktGetBitsArray(pak,
                    8 * mapData->header.currentByteCount,
                    (U8*)mapData->compressed.data + mapData->header.previousSentByteCount);
                    
    mapData->receivedByteCount = mapData->header.previousSentByteCount + mapData->header.currentByteCount;
}

void beaconMapDataPacketReceiveChunk(Packet* pak, BeaconMapDataPacket* mapData){
    beaconMapDataPacketReceiveChunkHeader(pak, mapData);
    beaconMapDataPacketReceiveChunkData(pak, mapData);
}

void beaconMapDataPacketSendChunkAck(Packet* pak, BeaconMapDataPacket* mapData){
    pktSendBitsPack(pak, 1, mapData->receivedByteCount);
}

S32 beaconMapDataPacketReceiveChunkAck(Packet* pak, U32 sentByteCount, U32* receivedByteCount){
    *receivedByteCount = pktGetBitsPack(pak, 1);
    
    return *receivedByteCount == sentByteCount;
}

S32 beaconMapDataPacketIsFullyReceived(BeaconMapDataPacket* mapData){
    return mapData ? mapData->compressed.byteCount && mapData->compressed.byteCount == mapData->receivedByteCount : 0;
}

S32 beaconMapDataPacketIsFullySent(BeaconMapDataPacket* mapData, U32 sentByteCount){
    return mapData->compressed.byteCount == sentByteCount;
}

U32 beaconMapDataPacketGetSize(BeaconMapDataPacket* mapData){
    return mapData ? mapData->compressed.byteCount : 0;
}

U8* beaconMapDataPacketGetData(BeaconMapDataPacket* mapData){
    return mapData ? mapData->compressed.data : NULL;
}

U32 beaconMapDataPacketGetReceivedSize(BeaconMapDataPacket* mapData){
    return mapData ? mapData->receivedByteCount : 0;
}

void beaconMapDataPacketDiscardData(BeaconMapDataPacket* mapData){
    if(!mapData){
        return;
    }
    
    SAFE_FREE(mapData->compressed.data);
    
    mapData->receivedByteCount = 0;
}

void beaconMapDataPacketWriteFile(    BeaconMapDataPacket* mapData,
                                    const char* fileName,
                                    const char* uniqueStorageName,
                                    U32 timeStamp)
{
    FILE* f = fopen(fileName, "wb");
    
    if(!f){
        return;
    }
    
    #define FWRITE_SIZE(x, size)    fwrite(x, size, 1, f)
    #define FWRITE(x)                FWRITE_SIZE(&x, sizeof(x))
    #define FWRITE_U32(x)            {U32 x_=x;FWRITE(x_);}

    // Write the file version.
    
    FWRITE_U32(0);
    
    // Write the CRC.
    
    FWRITE_U32(mapData->compressed.crc);
    
    // Write the unique storage name.
    
    FWRITE_U32((U32)strlen(uniqueStorageName));
    FWRITE_SIZE(uniqueStorageName, (U32)strlen(uniqueStorageName));
    
    // Write the timestamp.
    
    FWRITE_U32(timeStamp);
    
    // Write the packet's debuginfo flag.
    
    FWRITE_U32(mapData->packetDebugInfo ? 1 : 0);
    
    // Write the compressed data size.
    
    FWRITE_U32(mapData->compressed.byteCount);
    
    // Write the uncompressed bit count.
    
    FWRITE_U32(mapData->uncompressedBitCount);

    // Write the data.
    
    FWRITE_SIZE(mapData->compressed.data, mapData->compressed.byteCount);
    
    // Close the file.
    
    fclose(f);

    // Done!

    #undef FWRITE_SIZE
    #undef FWRITE
    #undef FWRITE_U32
}

S32 beaconMapDataPacketReadFile(BeaconMapDataPacket* mapData,
                                const char* fileName,
                                char** uniqueStorageName,
                                U32* timeStamp,
                                S32 headerOnly)
{
    FILE* f = fopen(fileName, "rb");
    U32 fileVersion;
    U32 storageNameLength;
    char tempStorageName[1000];
    
    if(!f){
        return 0;
    }

    #define FAIL                {fclose(f);return 0;}
    #define CHECK(x)            if(!(x)){FAIL;}
    #define FREAD_SIZE(x, size)    CHECK(fread(x, size, 1, f))
    #define FREAD(x)            FREAD_SIZE(&x, sizeof(x))
    #define FREAD_U32(x)        {U32 x_;FREAD(x_);x = x_;}

    // Read the file version.
    
    FREAD_U32(fileVersion);

    CHECK(fileVersion <= 0);

    // Read the CRC.
    
    FREAD_U32(mapData->compressed.crc);
    
    // Read the unique storage name.
    
    FREAD_U32(storageNameLength);
    CHECK(storageNameLength < 1000);
    FREAD_SIZE(tempStorageName, storageNameLength);
    tempStorageName[storageNameLength] = 0;
    CHECK(strlen(tempStorageName) == storageNameLength);
    estrPrintCharString(uniqueStorageName, tempStorageName);
    
    // Read the timestamp.
    
    assert(timeStamp);
    FREAD_U32(*timeStamp);
    
    // Read the packet's debuginfo flag.
    
    FREAD_U32(mapData->packetDebugInfo);
        
    // Read the compressed data size.
    
    FREAD_U32(mapData->compressed.byteCount);
        
    // Read the uncompressed bit count.
    
    FREAD_U32(mapData->uncompressedBitCount);

    mapData->uncompressedByteCount = (mapData->uncompressedBitCount + 7) / 8;    
        
    // Verify that there is enough data left in the file.
        
    CHECK(mapData->compressed.byteCount <= (U32)(fileGetSize(f) - ftell(f)));

    // Read the data.
        
    if(!headerOnly){
        SAFE_FREE(mapData->compressed.data);
        
        mapData->compressed.data = malloc(mapData->compressed.byteCount);
        
        FREAD_SIZE(mapData->compressed.data, mapData->compressed.byteCount);

        mapData->receivedByteCount = mapData->compressed.byteCount;
    }else{
        mapData->receivedByteCount = 0;
    }
        
    // Close the file.
    
    fclose(f);

    // Done!

    #undef FREAD
    #undef FREAD_U32
    
    return 1;
}

static S32 cmdOldLineHasParams(S32 argc, char** argv, S32 cur, S32 count){
    S32 i;
    
    if(cur + count > argc){
        return 0;
    }
    
    for(i = cur; i < cur + count; i++){
        if(!argv[i][0] || argv[i][0] == '-'){
            return 0;
        }
    }
    
    return 1;
}

void beaconHandleCmdLine(S32 argc, char** argv){
    S32        i;

    // This function is only allowed to do things that don't do much of anything.

    for(i=1;i<argc;i++)
    {
        S32 handled = 1;
        S32 start_i = i;

        #define HANDLERS_BEGIN    if(0){
        #define HANDLER(x)        }else if(!stricmp(argv[i],x)){
        #define HANDLERS_END    }else{handled = 0;}
        #define HAS_PARAMS(x)    cmdOldLineHasParams(argc, argv, i + 1, x)
        #define HAS_PARAM        HAS_PARAMS(1)
        #define GET_NEXT_PARAM    (argv[++i])

        HANDLERS_BEGIN
            HANDLER("-beaconclient"){
                assert(!beaconizerInit.clientServerType);
                
                beaconizerInit.clientServerType = BEACONIZER_TYPE_CLIENT;
                
                // Get the master server name.
                
                if(HAS_PARAM){
                    beaconizerInit.masterServerName = strdup(GET_NEXT_PARAM);
                }
            }
            HANDLER("-beaconclientsubserver"){
                // Get the subserver name.
                
                if(HAS_PARAM){
                    beaconizerInit.subServerName = strdup(GET_NEXT_PARAM);
                }
            }
            HANDLER("-beaconserver"){
                assert(!beaconizerInit.clientServerType);
                
                beaconizerInit.clientServerType = BEACONIZER_TYPE_SERVER;

                // Get the master server name.
                
                if(HAS_PARAM){
                    beaconizerInit.masterServerName = strdup(GET_NEXT_PARAM);
                }
            }
            HANDLER("-beaconautoserver"){
                assert(!beaconizerInit.clientServerType);
                
                beaconizerInit.clientServerType = BEACONIZER_TYPE_AUTO_SERVER;

                // Get the master server name.
                
                if(HAS_PARAM){
                    beaconizerInit.masterServerName = strdup(GET_NEXT_PARAM);
                }
            }
            HANDLER("-beaconmasterserver"){
                assert(!beaconizerInit.clientServerType);
                
                beaconizerInit.clientServerType = BEACONIZER_TYPE_MASTER_SERVER;
            }
            HANDLER("-beaconrequestserver"){
                assert(!beaconizerInit.clientServerType);
                
                beaconizerInit.clientServerType = BEACONIZER_TYPE_REQUEST_SERVER;

                // Get the master server name.
                
                if(HAS_PARAM){
                    beaconizerInit.masterServerName = strdup(GET_NEXT_PARAM);
                }
            }
            HANDLER("-beaconusemasterserver"){
                if(HAS_PARAM){
                    beaconRequestSetMasterServerAddress(GET_NEXT_PARAM);
                }else{
                    Errorf("No address specified for -beaconusemasterserver!");
                }
            }
            HANDLER("-beaconrequestcachedir"){
                if(HAS_PARAM){
                    beaconServerSetRequestCacheDir(GET_NEXT_PARAM);
                }
            }
            HANDLER("-beaconproductionmode"){
                beacon_common.productionMode = 1;
            }
            HANDLER("-beaconallownovodex"){
                beacon_common.allowNovodex = 1;
            }
            HANDLER("-beaconsymstore"){
                beaconServerSetSymStore();
            }
            HANDLER("-beaconworkduringuseractivity"){
                beaconClientSetWorkDuringUserActivity(1);
            }
            HANDLER("-beaconnonetstart"){
                beaconizerInit.noNetStart = 1;
            }
            HANDLER("-beacononepassonly"){
                beaconizerInit.onePassOnly = 1;
            }
            HANDLER("-beaconforcerebuild"){
                beaconizerInit.forceRebuild = 1;
            }
            HANDLER("-beacondatatoolsrootpath"){
                if(HAS_PARAM){
                    beaconServerSetDataToolsRootPath(GET_NEXT_PARAM);
                }
            }
        HANDLERS_END

        #undef HANDLERS_BEGIN
        #undef HANDLER
        #undef HANDLERS_END
        #undef HAS_PARAMS
        #undef HAS_PARAM
        #undef GET_NEXT_PARAM

        if(handled){
            // Invalidate handled parameters.

            while(start_i <= i){
                argv[start_i++][0] = 0;
            }
        }
    }
}

S32 beaconIsProductionMode(){
    return beacon_common.productionMode;
}

void beaconGetCommonCmdLine(char* buffer){
    buffer += sprintf(buffer, " -noencrypt");
    
    if(beacon_common.productionMode){
        buffer += sprintf(buffer, " -beaconproductionmode");
    }
    
    if(beacon_common.allowNovodex){
        buffer += sprintf(buffer, " -beaconallownovodex");
    }
}

S32 beaconizerIsStarting()
{
    return beaconizerInit.clientServerType ? 1 : 0;
}

void beaconizerInitConsoleWindow(){
    if(!beaconizerInit.clientServerType){
        return;
    }
    
    consoleInit(110, 128, 0);

    switch(beaconizerInit.clientServerType){
        xcase BEACONIZER_TYPE_CLIENT:
            setConsoleTitle("BeaconClient Starting...");
        xcase BEACONIZER_TYPE_SERVER:
            setConsoleTitle("Manual-BeaconServer Starting...");
        xcase BEACONIZER_TYPE_AUTO_SERVER:
            setConsoleTitle("Auto-BeaconServer Starting...");
        xcase BEACONIZER_TYPE_MASTER_SERVER:
            setConsoleTitle("Master-BeaconServer Starting...");
        xcase BEACONIZER_TYPE_REQUEST_SERVER:
            setConsoleTitle("Request-BeaconServer Starting...");
        xdefault:
            assert(0);
    }
}

void beaconizerStart()
{
    if(!beaconizerInit.clientServerType){
        return;
    }
    
    logBackgroundWriterSetSleep(500);
    
    #if NOVODEX
        if(    !beaconIsProductionMode() &&
            !beacon_common.allowNovodex)
        {
            FatalErrorf("Beaconizer must have NovodeX disabled!");
        }
    #endif

    beaconizerInitConsoleWindow();
    
    switch(beaconizerInit.clientServerType){
        xcase    BEACONIZER_TYPE_CLIENT:{
            beaconClient(    beaconizerInit.masterServerName,
                            beaconizerInit.subServerName);
        }

        xcase    BEACONIZER_TYPE_SERVER:
        case    BEACONIZER_TYPE_MASTER_SERVER:
        case    BEACONIZER_TYPE_AUTO_SERVER:
        case    BEACONIZER_TYPE_REQUEST_SERVER:
        {
            beaconServer(    beaconizerInit.clientServerType,
                            beaconizerInit.masterServerName,
                            beaconizerInit.noNetStart,
                            beaconizerInit.onePassOnly,
                            beaconizerInit.forceRebuild);
        }
    }
}

