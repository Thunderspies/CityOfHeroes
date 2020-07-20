
#include <utilitieslib/stdtypes.h>
#include "beaconClientServerPrivate.h"
#include "comm_backend.h"
#include <utilitieslib/utils/winutil.h>
#include <time.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/log.h>

#undef GetEnvironmentStrings

static char* beaconClientExeName = "BeaconClient.exe";

static S32 hideClient = 1;

typedef struct BeaconSentryClientData {
    HANDLE    hPipe;
    U32        processID;
    U32        myServerID;
    char*    serverUID;
    U32        lastSentTime;
    
    char*    crashText;
    U32        terminated        : 1;
    U32        forcedInactive    : 1;
    U32        usedInactive    : 1;
} BeaconSentryClientData;

static struct {
    HANDLE                        hMutexSentry;

    struct {
        BeaconSentryClientData*    clients;
        S32                        count;
        S32                        maxCount;
    } sentryClients;
    
    struct {
        U8*                        data;
        S32                        receivedByteCount;
        S32                        maxTotalByteCount;
    } newExeData;
    
    HANDLE                        hPipeToSentry;
    
    NetLink                        serverLink;
    NetLink                        db_link;

    char*                        masterServerName;
    
    char*                        subServerName;
    S32                            subServerPort;
    
    BeaconMapDataPacket*        mapData;
    
    U32                            executableCRC;
    U32                            mapCRC;
    U32                            myServerID;
    char*                        serverUID;
    
    S32                            cpuCount;
    U32                            timeStartedClient;
    U32                            timeSinceUserActivity;
    U32                            timeOfLastCommand;
    U32                            timeStartedRadiosityClient;
    
    char*                        exeData;
    U32                            exeSize;
    
    S32                            keyboardPressed;
    
    U32                            sentServerID            : 1;
    U32                            sentSentryClients        : 1;
    U32                            workDuringUserActivity    : 1;
    U32                            userInactive            : 1;
    U32                            connectedToSubServer    : 1;
} beacon_client;

static S32 beaconClientIsSentry(void){
    return beacon_client.hMutexSentry ? 1 : 0;
}

static void beaconClientUpdateTitle(const char* format, ...){
    char buffer[1000];
    char clientTitle[100];
    
    if(beaconClientIsSentry()){
        S32 clientCount = max(0, beacon_client.sentryClients.count - 1);
        STR_COMBINE_BEGIN(clientTitle);
        STR_COMBINE_CAT("Sentry-BeaconClient(");
        STR_COMBINE_CAT_D(clientCount);
        STR_COMBINE_CAT(" client");
        if(beacon_client.sentryClients.count != 2){
            STR_COMBINE_CAT("s");
        }
        STR_COMBINE_CAT(")");
        STR_COMBINE_END();
    }else{
        STR_COMBINE_BEGIN(clientTitle);
        STR_COMBINE_CAT("Worker-BeaconClient");
        if(!beacon_client.hPipeToSentry){
            STR_COMBINE_CAT("(No Sentry)");
        }
        STR_COMBINE_END();
    }

    STR_COMBINE_BEGIN(buffer);
    STR_COMBINE_CAT(clientTitle);
    STR_COMBINE_CAT(":");
    STR_COMBINE_CAT_D(_getpid());
    STR_COMBINE_CAT(" ");
    if(beaconClientIsSentry()){
        if(!beacon_client.userInactive){
            STR_COMBINE_CAT("(User Is Active) ");
        }
        else if(beacon_client.workDuringUserActivity){
            STR_COMBINE_CAT("(Work During User Activity) ");
        }
    }
    if(beacon_client.serverLink.connected){
        STR_COMBINE_CAT("Connected!");
    }else{
        STR_COMBINE_CAT("Not Connected!");
    }
    STR_COMBINE_END();
    
    if(format){
        char* pos = buffer + strlen(buffer);
        
        strcpy(pos, " (");
        pos += 2;
        
        VA_START(argptr, format);
        pos += vsprintf_s(pos, ARRAY_SIZE_CHECKED(buffer) - strlen(buffer), format, argptr);
        VA_END();
        
        strcpy(pos, ")");
        pos++;
    }

    setConsoleTitle(buffer);
}

Packet*    beaconClientCreatePacket(NetLink* link, const char* textCmd){
    Packet* pak = pktCreateEx(link, BMSG_C2S_TEXT_CMD);
    pktSendString(pak, textCmd);
    return pak;
}

void beaconClientSendPacket(NetLink* link, Packet** pak){
    pktSend(pak, link);
}

static void sendReadyMessage(void){
    beaconPrintf(COLOR_GREEN, "Sending ready message!\n");
    
    BEACON_CLIENT_PACKET_CREATE(BMSG_C2ST_READY_TO_WORK);
    BEACON_CLIENT_PACKET_SEND();
    
    beacon_client_conn.readyToWork = 1;
    beacon_client.userInactive = 0;
}

static void sendMapDataIsLoaded(void){
    BEACON_CLIENT_PACKET_CREATE(BMSG_C2ST_MAP_DATA_IS_LOADED);
        pktSendBits(pak, 32, beacon_client.mapCRC);
    BEACON_CLIENT_PACKET_SEND();
}

static void beaconClientReceiveLegalAreas(Packet* pak, BeaconDiskSwapBlock* block){
    S32 i;
    S32 count;
    
    if(pktGetBits(pak, 1) == 1){
        U32 surfaceCRC = pktGetBits(pak, 32);
        
        if(block->foundCRC){
            assert(block->surfaceCRC == surfaceCRC);
            block->verifiedCRC = 1;
        }else{
            block->foundCRC = 1;
            block->surfaceCRC = surfaceCRC;
        }
    }    
    
    while(block->legalCompressed.areasHead){
        BeaconLegalAreaCompressed* next = block->legalCompressed.areasHead->next;
        destroyBeaconLegalAreaCompressed(block->legalCompressed.areasHead);
        block->legalCompressed.areasHead = next;
        block->legalCompressed.totalCount--;
    }
    
    assert(!block->legalCompressed.totalCount);
    
    count = pktGetBitsPack(pak, 16);
    
    for(i = 0; i < count; i++){
        BeaconLegalAreaCompressed* area = beaconAddLegalAreaCompressed(block);
        
        area->x = pktGetBits(pak, 8);
        area->z = pktGetBits(pak, 8);
        area->checked = pktGetBits(pak, 1);
        
        if(pktGetBits(pak, 1)){
            area->isIndex = 1;
            area->y_index = pktGetBitsPack(pak, 5);
        }else{
            area->isIndex = 0;
            area->y_coord = pktGetF32(pak);
        }
        
        beaconReceiveColumnAreas(pak, area);
        
        #if BEACONGEN_STORE_AREA_CREATOR
            area->areas.cx = pktGetBitsPack(pak, 1);
            area->areas.cz = pktGetBitsPack(pak, 1);
            area->areas.ip = pktGetBits(pak, 32);
        #endif
    }
    
    assert(block->legalCompressed.totalCount == count);

    beaconCheckDuplicates(block);
}

static void beaconClientPrintColumnAreaError(    BeaconLegalAreaCompressed* legalArea,
                                                BeaconGenerateColumn* column,
                                                BeaconDiskSwapBlock* block)
{
    BeaconGenerateColumnArea* curArea;
    
    printf(    "\n\nMismatched area count (me: %d, received: %d) at block (%d,%d)... point (%d,%d)\n\n",
            column->areaCount,
            legalArea->areas.count,
            block->x / BEACON_GENERATE_CHUNK_SIZE,
            block->z / BEACON_GENERATE_CHUNK_SIZE,
            block->x + legalArea->x,
            block->z + legalArea->z);
    
    #if BEACONGEN_STORE_AREAS
    {
        S32 i;
        
        #if BEACONGEN_STORE_AREA_CREATOR
            printf("Legal area areas (%d,%d from %s):\n", legalArea->areas.cx, legalArea->areas.cz, makeIpStr(legalArea->areas.ip));
        #endif

        for(i = 0; i < legalArea->areas.count; i++){
            printf("  area: %6.8f - %6.8f\n", legalArea->areas.areas[i].y_min, legalArea->areas.areas[i].y_max);

            #if BEACONGEN_CHECK_VERTS
            {
                S32 j;
            
                printf("    def: %s\n", legalArea->areas.areas[i].defName);

                for(j = 0; j < 3; j++){
                    printf("      (%1.8f, %1.8f, %1.8f)\n", vecParamsXYZ(legalArea->areas.areas[i].triVerts[j]));
                }
            }
            #endif
        }
    }
    #endif
        
    printf("\n\nColumn area areas:\n");

    for(curArea = column->areas; curArea; curArea = curArea->nextColumnArea){
        printf("  area: %6.8f - %6.8f\n", curArea->y_min, curArea->y_max);
        
        #if BEACONGEN_CHECK_VERTS
        {
            S32 i;
        
            printf("    def: %s\n", curArea->triVerts.def->name);
            
            for(i = 0; i < 3; i++){
                printf("      (%1.8f, %1.8f, %1.8f)\n", vecParamsXYZ(curArea->triVerts.verts[i]));
            }
        }
        #endif
    }
                
    #if BEACONGEN_CHECK_VERTS
    {
        S32 i;
        
        printf("\n\nLegal area tris:\n");
        
        for(i = 0; i < legalArea->tris.count; i++){
            S32 j;
            
            printf("  tri: %6.8f - %6.8f\n", legalArea->tris.tris[i].y_min, legalArea->tris.tris[i].y_max);
            
            printf("    def: %s\n", legalArea->tris.tris[i].defName);
            
            for(j = 0; j < 3; j++){
                printf("      (%1.8f, %1.8f, %1.8f)\n", vecParamsXYZ(legalArea->tris.tris[i].verts[j]));
            }
        }
        
        printf("\n\nColumn tris:\n");

        for(i = 0; i < column->tris.count; i++){
            S32 j;
            
            printf("  tri: %6.8f - %6.8f\n", column->tris.tris[i].y_min, column->tris.tris[i].y_max);
            
            printf("    def: %s\n", column->tris.tris[i].def->name);
            
            for(j = 0; j < 3; j++){
                printf("      (%1.8f, %1.8f, %1.8f)\n", vecParamsXYZ(column->tris.tris[i].verts[j]));
            }
        }
    }
    #endif

    printf("\n\n");
                
    assert(0);
}

static void beaconClientApplyCompressedLegalAreas(BeaconDiskSwapBlock* block){
    BeaconLegalAreaCompressed* legalArea;
    
    for(legalArea = block->legalCompressed.areasHead; legalArea; legalArea = legalArea->next){
        BeaconGenerateColumn* column = block->chunk->columns + BEACON_GEN_COLUMN_INDEX(legalArea->x, legalArea->z);
        BeaconGenerateColumnArea* area;
        
        if(legalArea->areas.count && legalArea->areas.count != column->areaCount){
            beaconClientPrintColumnAreaError(legalArea, column, block);
        }
        
        if(legalArea->isIndex){
            S32 j = legalArea->y_index;
            
            assert(j >= 0 && j < column->areaCount);
            
            area = column->areas;
            
            while(j--){
                area = area->nextColumnArea;
            }
            
            assert(area);
        }else{
            area = beaconGetColumnAreaFromYPos(column, legalArea->y_coord);
        }

        if(area){
            area->inCompressedLegalList = 1;
            
            beaconMakeLegalColumnArea(column, area, legalArea->checked);
        }
    }
}

static void beaconClientCheckWindow(void){
    if(!IsWindowVisible(beaconGetConsoleWindow())){
        #define KEY_IS_DOWN(key) (0x8000 & GetKeyState(key) ? 1 : 0)
        S32 lShift        = KEY_IS_DOWN(VK_LSHIFT);
        S32 rShift        = KEY_IS_DOWN(VK_RSHIFT);
        S32 lControl    = KEY_IS_DOWN(VK_LCONTROL);
        S32 rControl    = KEY_IS_DOWN(VK_RCONTROL);
        S32 lAlt        = KEY_IS_DOWN(VK_LMENU);
        S32 rAlt        = KEY_IS_DOWN(VK_RMENU);
        #undef KEY_IS_DOWN
        
        if(    (lShift || rShift) &&
            (lControl || rControl) &&
            (lAlt || rAlt))
        {
            RECT rectDesktop;
            POINT pt;

            if(    GetWindowRect(GetDesktopWindow(), &rectDesktop) &&
                GetCursorPos(&pt) &&
                SQR(pt.x - rectDesktop.left) + SQR(pt.y - rectDesktop.top) < SQR(10))
            {
                ShowWindow(beaconGetConsoleWindow(), SW_RESTORE);
                
                SetForegroundWindow(beaconGetConsoleWindow());
            }
        }
    }
    else if(!beaconIsProductionMode()){
        WINDOWPLACEMENT wp = {0};
        
        wp.length = sizeof(wp);
        GetWindowPlacement(beaconGetConsoleWindow(), &wp);
        
        switch(wp.showCmd){
            xcase SW_SHOWMINIMIZED:
            case SW_MINIMIZE:{
                if(hideClient){
                    ShowWindow(beaconGetConsoleWindow(), SW_HIDE);
                }
            }
        }
    }
}

static DWORD WINAPI beaconClientWindowThread(void* unused){
    U32 ticksPerSecond = timerCpuSpeed();
    U32 fastCheckTime = 4 * ticksPerSecond;
    U16 oldKeyStates[256];
    U16 keyStates[256];
    S32 i;
    
    for(i = 0; i < 256; i++){
        oldKeyStates[i] = GetKeyState(i);
    }
    
    while(1){
        beaconClientCheckWindow();
        
        if(!beaconClientIsSentry() && beacon_client.timeSinceUserActivity > fastCheckTime){
            //printf("time: %d\n", beacon_client.timeUntilAwake);
            
            for(i = 0; i < 256; i++){
                keyStates[i] = GetKeyState(i);
            }
            
            if(memcmp(keyStates, oldKeyStates, sizeof(keyStates))){
                //printf("Keyboard pressed.\n");
                beacon_client.keyboardPressed = 1;
            }else{
                //printf("No key changes.\n");
            }
            
            memcpy(oldKeyStates, keyStates, sizeof(keyStates));
            
            Sleep(100);
        }else{
            Sleep(1000);
        }
    }
}

static void beaconProcessLegalAreas(BeaconDiskSwapBlock* block){
    S32 grid_x = block->x / BEACON_GENERATE_CHUNK_SIZE;
    S32 grid_z = block->z / BEACON_GENERATE_CHUNK_SIZE;
    
    beaconClearNonAdjacentSwapBlocks(NULL);

    printf("Extract");
    
    beaconExtractSurfaces(grid_x - 1, grid_z - 1, 3, 3, 1);
    
    assert(block->chunk);
    
    //printf(", Legalize(%4d)", block->legalCompressed.totalCount);
    
    //assert(!(grid_x == -18 && grid_z == -4));

    beaconClientApplyCompressedLegalAreas(block);
    
    //assert(block->isLegal);
    
    printf(", Surface");
    
    beaconPropagateLegalColumnAreas(block);
    
    printf(", Generate");
    
    beaconMakeBeaconsInChunk(block->chunk, 1);
    
    printf("(%d)", bp_blocks.generatedBeacons.count);
}

static void beaconClientSendLegalAreas(S32 center_grid_x, S32 center_grid_z){
    BEACON_CLIENT_PACKET_CREATE(BMSG_C2ST_GENERATE_FINISHED);

        BeaconDiskSwapBlock* cur;
        
        for(cur = bp_blocks.list; cur; cur = cur->nextSwapBlock){
            if(cur->addedLegal){
                BeaconLegalAreaCompressed* area;
                S32 grid_x = cur->x / BEACON_GENERATE_CHUNK_SIZE;
                S32 grid_z = cur->z / BEACON_GENERATE_CHUNK_SIZE;
                
                beaconCheckDuplicates(cur);

                pktSendBits(pak, 1, 1);
                
                pktSendBitsPack(pak, 1, grid_x);
                pktSendBitsPack(pak, 1, grid_z);
                
                if(grid_x == center_grid_x && grid_z == center_grid_z){
                    S32 i;
                    
                    // Send stuff related to the block this client was assigned to process.
                    
                    assert(cur->foundCRC);
                    pktSendBits(pak, 32, cur->surfaceCRC);
                    
                    // Send the beacons.
                    
                    pktSendBitsPack(pak, 1, bp_blocks.generatedBeacons.count);
                    
                    for(i = 0; i < bp_blocks.generatedBeacons.count; i++){
                        pktSendBitsArray(pak, 8 * sizeof(Vec3), bp_blocks.generatedBeacons.beacons[i].pos);
                        pktSendBits(pak, 1, bp_blocks.generatedBeacons.beacons[i].noGroundConnections);
                    }
                }
                            
                pktSendBitsPack(pak, 5, cur->legalCompressed.totalCount);
                
                for(area = cur->legalCompressed.areasHead; area; area = cur->legalCompressed.areasHead){
                    S32 columnIndex = BEACON_GEN_COLUMN_INDEX(area->x, area->z);
                    BeaconGenerateColumn* column = cur->chunk->columns + columnIndex;
                    
                    pktSendBits(pak, 8, area->x);
                    pktSendBits(pak, 8, area->z);
                    
                    if(area->isIndex){
                        pktSendBits(pak, 1, 1);
                        pktSendBitsPack(pak, 5, area->y_index);

                        assert(    column->isIndexed &&
                                area->y_index >= 0 &&
                                (S32)area->y_index < column->areaCount);
                    }else{
                        pktSendBits(pak, 1, 0);
                        pktSendF32(pak, area->y_coord);
                    }
                    
                    pktSendBitsPack(pak, 1, column->areaCount);

                    #if BEACONGEN_STORE_AREAS
                    {
                        BeaconGenerateColumnArea* columnArea;
                        
                        for(columnArea = column->areas;
                            columnArea;
                            columnArea = columnArea->nextColumnArea)
                        {
                            pktSendF32(pak, columnArea->y_min);
                            pktSendF32(pak, columnArea->y_max);
                            
                            #if BEACONGEN_CHECK_VERTS
                            {
                                S32 i;
                                
                                for(i = 0; i < 3; i++){
                                    pktSendF32(pak, columnArea->triVerts.verts[i][0]);
                                    pktSendF32(pak, columnArea->triVerts.verts[i][1]);
                                    pktSendF32(pak, columnArea->triVerts.verts[i][2]);
                                }
                                
                                assert(columnArea->triVerts.def);
                                
                                pktSendString(pak, columnArea->triVerts.def->name);
                            }
                            #endif
                        }
                    }
                    #endif
                    
                    #if BEACONGEN_CHECK_VERTS
                    {
                        S32 i;
                        
                        for(i = 0; i < column->tris.count; i++){
                            S32 j;
                            pktSendBits(pak, 1, 1);
                            pktSendString(pak, column->tris.tris[i].def->name);
                            pktSendF32(pak, column->tris.tris[i].y_min);
                            pktSendF32(pak, column->tris.tris[i].y_max);
                            for(j = 0; j < 3; j++){
                                pktSendF32(pak, column->tris.tris[i].verts[j][0]);
                                pktSendF32(pak, column->tris.tris[i].verts[j][1]);
                                pktSendF32(pak, column->tris.tris[i].verts[j][2]);
                            }
                        }

                        pktSendBits(pak, 1, 0);
                    }
                    #endif

                    cur->legalCompressed.areasHead = area->next;
                    destroyBeaconLegalAreaCompressed(area);
                    cur->legalCompressed.totalCount--;
                }
            }
            
            assert(!cur->legalCompressed.totalCount);
        }
        
        // Send terminator bit.

        pktSendBits(pak, 1, 0);
        
    BEACON_CLIENT_PACKET_SEND();
}

static void beaconClientReceiveMapData(Packet* pak){
    S32 doNotCalculateCRC = pktGetBits(pak, 1);

    if(!beacon_client.mapData){
        beaconMapDataPacketCreate(&beacon_client.mapData);
    }
    
    beaconMapDataPacketReceiveChunk(pak, beacon_client.mapData);

    beaconPrintf(    COLOR_GREEN,
                    "Received map data: %s/%s bytes!\n",
                    getCommaSeparatedInt(beaconMapDataPacketGetReceivedSize(beacon_client.mapData)),
                    getCommaSeparatedInt(beaconMapDataPacketGetSize(beacon_client.mapData)));

    beaconResetReceivedMapData();
    
    if(beaconMapDataPacketIsFullyReceived(beacon_client.mapData)){
        if(beaconMapDataPacketToMapData(beacon_client.mapData)){
            beacon_client.mapCRC = doNotCalculateCRC ? 0 : beaconGetMapCRC(1, 0, 0);
            
            beaconCurTimeString(1);
            
            printf("Map CRC: 0x%8.8x\n", beacon_client.mapCRC);

            sendMapDataIsLoaded();
            
            beaconInitGenerating(0);

            printf("Done: %s\n", beaconCurTimeString(0));
        }
        else if(beacon_client.serverLink.connected){
            netSendDisconnect(&beacon_client.serverLink, 0.0f);
        }
    }else{
        BEACON_CLIENT_PACKET_CREATE(BMSG_C2ST_NEED_MORE_MAP_DATA);
            
            beaconMapDataPacketSendChunkAck(pak, beacon_client.mapData);
            
        BEACON_CLIENT_PACKET_SEND();
    }
}

static void beaconClientConnectBeacon(Packet* pak, S32 index){
    Beacon* b;
    BeaconProcessInfo* info;
    S32 j;
    S32 groundCount = 0;
    S32 raisedCount = 0;
    
    assert(index >= 0 && index < combatBeaconArray.size);
    
    b = combatBeaconArray.storage[index];
    info = beacon_process.infoArray + index;

    beaconProcessCombatBeacon2(b);
    
    pktSendBitsPack(pak, 1, index);
    pktSendBitsPack(pak, 1, info->beaconCount);
    
    for(j = 0; j < info->beaconCount; j++){
        S32 k;
        
        pktSendBitsPack(pak, 1, info->beacons[j].targetIndex);
        pktSendBits(pak, 1, info->beacons[j].reachedByGround);
        pktSendBitsPack(pak, 1, info->beacons[j].raisedCount);
        
        for(k = 0; k < info->beacons[j].raisedCount; k++){
            pktSendF32(pak, info->beacons[j].raisedConns[k].minHeight - posY(b));
            pktSendF32(pak, info->beacons[j].raisedConns[k].maxHeight - posY(b));
        }
    
        groundCount += info->beacons[j].reachedByGround;
        raisedCount += info->beacons[j].raisedCount;
        
        if(info->beacons[j].raisedCount){
            SAFE_FREE(info->beacons[j].raisedConns);
        }
    }
    
    SAFE_FREE(info->beacons);
}

static void beaconClientSetPriorityLevel(S32 level){
    S32 priorityClass = NORMAL_PRIORITY_CLASS;
    S32 threadPriority = THREAD_PRIORITY_NORMAL;

    switch(level){
        xcase 0:{
            priorityClass = IDLE_PRIORITY_CLASS;
            threadPriority = THREAD_PRIORITY_IDLE;
        }
    }

    SetPriorityClass(GetCurrentProcess(), priorityClass);
    SetThreadPriority(GetCurrentThread(), threadPriority);
}

static void beaconClientReceiveAndConnectBeacons(Packet* pakIn){
    BEACON_CLIENT_PACKET_CREATE(BMSG_C2ST_BEACON_CONNECTIONS);

        S32 count = pktGetBitsPack(pakIn, 1);
        S32 i;
        
        beaconClientSetPriorityLevel(0);

        printf("Connecting %3d: ", count);
        
        for(i = 0; i < count; i++){
            S32 index = pktGetBitsPack(pakIn, 1);
            
            beaconClientConnectBeacon(pak, index);
            
            printf(".");
        }
        
        printf("\n");
    
    BEACON_CLIENT_PACKET_SEND();

    beaconClientSetPriorityLevel(1);
}

static void beaconClientClosePipeToSentry(void){
    if(beacon_client.hPipeToSentry){
        CloseHandle(beacon_client.hPipeToSentry);
        beacon_client.hPipeToSentry = NULL;
        beacon_client.sentServerID = 0;
    }
}

static void createNewSentryClient(void){
    DWORD dwMode = PIPE_READMODE_MESSAGE | PIPE_NOWAIT;
    BeaconSentryClientData* client;
    
    client = dynArrayAddStruct(    &beacon_client.sentryClients.clients,
                                &beacon_client.sentryClients.count,
                                &beacon_client.sentryClients.maxCount);
                            
    ZeroStruct(client);
    
    client->hPipe = CreateNamedPipeA("\\\\.\\pipe\\CrypticBeaconClientPipe", 
                                     PIPE_ACCESS_DUPLEX,
                                     PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
                                     PIPE_UNLIMITED_INSTANCES,
                                     10000,
                                     10000,
                                     0,
                                     NULL);
                            
    if(client->hPipe == INVALID_HANDLE_VALUE){
        printf("Failed to create pipe!\n");
        assert(0);
        
        return;
    }
    
    printf("Created pipe %d: %d\n", beacon_client.sentryClients.count - 1, client->hPipe);
                            
    if(!SetNamedPipeHandleState(client->hPipe, &dwMode, NULL, NULL)){
        printf("mode failed: %d\n", GetLastError());
    }
    
    beacon_client.sentSentryClients = 0;
}

static S32 writeDataToPipe(HANDLE hPipe, const char* data, S32 length){
    S32 bytesWritten;
    
    if(WriteFile(hPipe, data, length, &bytesWritten, 0)){
        return 1;
    }else{
        printf("pipe write failed: h=0x%p, error=%d\n", hPipe, GetLastError());
        
        if(hPipe == beacon_client.hPipeToSentry){
            beaconClientClosePipeToSentry();
        }
        
        return 0;
    }
}

static S32 writeStringToPipe(HANDLE hPipe, const char* text){
    return writeDataToPipe(hPipe, text, (S32)strlen(text));
}

static S32 printfPipe(HANDLE hPipe, const char* format, ...){
    char buffer[20000];
    va_list argptr;
    
    if(!hPipe){
        hPipe = beacon_client.hPipeToSentry;
    }
    
    if(!hPipe){
        return 0;
    }    
    
    va_start(argptr, format);
    _vsnprintf(buffer, ARRAY_SIZE(buffer) - 100, format, argptr);
    va_end(argptr);
    
    return writeStringToPipe(hPipe, buffer);
}

static char* findNonSpace(char* text){
    while(*text && isspace((unsigned char)*text)){
        text++;
    }
    
    return text;
}

static void beaconClientParseSentryClientCmd(BeaconSentryClientData* client, char* cmdName, char* value){
    #define IF_CMD(cmdstring) else if(!stricmp(cmdName, cmdstring))
    
    if(!*cmdName){
        return;
    }
    IF_CMD("ProcessID"){
        U32 pid = atoi(value);
        if(pid != client->processID){
            client->processID = pid;
            beaconPrintf(COLOR_GREEN, "Client %d:%d: Connected!\n", client->hPipe, client->processID);
        }
    }
    IF_CMD("MyServerID"){
        client->myServerID = atoi(value);
        beacon_client.sentSentryClients = 0;
        beaconPrintf(COLOR_GREEN, "Client %d:%d: MyServerID = %d\n", client->hPipe, client->processID, client->myServerID);
    }
    IF_CMD("ServerUID"){
        estrPrintCharString(&client->serverUID, value);
        beacon_client.sentSentryClients = 0;
        beaconPrintf(COLOR_GREEN, "Client %d:%d: ServerUID = %s\n", client->hPipe, client->processID, client->serverUID);
    }
    IF_CMD("Time"){
        client->lastSentTime = atoi(value);
    }
    IF_CMD("ForcedInactive"){
        client->forcedInactive = atoi(value) ? 1 : 0;
        beacon_client.sentSentryClients = 0;
        beaconPrintf(COLOR_GREEN, "Client %d:%d: ForcedInactive = %d\n", client->hPipe, client->processID, client->forcedInactive);
    }
    IF_CMD("Crash"){
        //{
        //    char computerName[100];
        //    S32 size = ARRAY_SIZE(computerName);
        //    GetComputerName(computerName, &size);
        //    if(!stricmp(computerName, "beacon2")){
        //        S32* x = NULL;
        //        //*x = 10;
        //        //assert(0);
        //    }
        //}
        
        estrPrintCharString(&client->crashText, value);
        beacon_client.sentSentryClients = 0;

        beaconPrintf(COLOR_RED, "Client %d:%d:%d: \n%s\n", client, client->hPipe, client->processID, client->crashText);
    }
    else{
        beaconPrintf(COLOR_RED|COLOR_GREEN,
                    "Client %d:%d: Unknown cmd: %s = %s\n",
                    client->hPipe,
                    client->processID,
                    cmdName,
                    value);
    }
    
    #undef IF_CMD
}

static void beaconClientOpenPipeToSentry(void){
    if(!beacon_client.hPipeToSentry){
        beacon_client.hPipeToSentry = CreateFileA("\\\\.\\pipe\\CrypticBeaconClientPipe", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);

        if(beacon_client.hPipeToSentry != INVALID_HANDLE_VALUE){
            printf("Connected to sentry with pipe: %d\n", beacon_client.hPipeToSentry);
            printfPipe(0, "ProcessID %d\nForcedInactive %d", _getpid(), beacon_client.userInactive);
        }else{
            //printf("CreateFile failed: %d\n", GetLastError());
            beacon_client.hPipeToSentry = NULL;
        }
    }
}

void beaconClientSetWorkDuringUserActivity(S32 set){
    beacon_client.workDuringUserActivity = set ? 1 : 0;
}

static S32 beaconAcquireMutex(HANDLE hMutex){
    DWORD result = WaitForSingleObject(hMutex, 0);
    
    return    result == WAIT_OBJECT_0 ||
            result == WAIT_ABANDONED;
}

void beaconClientReleaseSentryMutex(void){
    beaconReleaseAndCloseMutex(&beacon_client.hMutexSentry);
}

static S32 beaconClientAcquireSentryMutex(void){
    beacon_client.hMutexSentry = CreateMutexA(NULL, 0, "Global\\CrypticBeaconClientSentry");

    assert(beacon_client.hMutexSentry);
    
    if(beaconAcquireMutex(beacon_client.hMutexSentry)){
        return 1;
    }
    
    beaconClientReleaseSentryMutex();

    return 0;
}

static char* beaconClientGetCmdLine(S32 useMaster,
                                    S32 useSub,
                                    S32 useWorkDuringUserActivity)
{
    static char* buffer = NULL;

    estrPrintCharString(&buffer, " -beaconclient");
    
    if(    useMaster &&
        beacon_client.masterServerName)
    {
        estrConcatf(&buffer, " %s", beacon_client.masterServerName);
    }
    
    if(    useSub &&
        beacon_client.subServerName)
    {
        estrConcatf(&buffer, " -beaconclientsubserver %s", beacon_client.subServerName);
        
        if(    beacon_client.subServerPort &&
            !strchr(beacon_client.subServerName, ':'))
        {
            estrConcatf(&buffer, ":%d", beacon_client.subServerPort);
        }
    }
    
    if(    useWorkDuringUserActivity &&
        beacon_client.workDuringUserActivity)
    {
        estrConcatf(&buffer, " -beaconworkduringuseractivity");
    }
    
    beaconGetCommonCmdLine(buffer + strlen(buffer));
    
    return buffer;
}

static void beaconClientCheckForSentry(void){
    static S32 lastTotalTime = -1;
    static U32 lastTime;
    static U32 totalTime;
    static HANDLE hMutex;

    if(beacon_client.hPipeToSentry){
        lastTime = 0;
        return;
    }
    
    if(!hMutex){
        hMutex = CreateMutexA(NULL, 0, "Global\\CrypticBeaconClientSentryCheck");
    }

    assert(hMutex);

    if(beaconAcquireMutex(hMutex)){
        U32 curTime = time(NULL);
        
        if(!lastTime){
            lastTime = curTime;
        }
        
        if(beaconClientAcquireSentryMutex()){
            totalTime += curTime - lastTime;
            
            beaconClientReleaseSentryMutex();
            
            if(totalTime >= 10){
                printf("Starting a new sentry!\n");
                
                beaconStartNewExe(    beaconClientExeName,
                                    beacon_client.exeData,
                                    beacon_client.exeSize,
                                    beaconClientGetCmdLine(1, 0, 1),
                                    0, 0,
                                    hideClient);
                
                totalTime = 0;
                lastTime = 0;
            }else{
                if(lastTotalTime != totalTime){
                    beaconPrintf(COLOR_RED|COLOR_GREEN, "Starting new sentry in %d seconds.\n", 10 - totalTime);
                    lastTotalTime = totalTime;
                }
            }
        }else{
            totalTime = 0;
            lastTotalTime = -1;
        }

        lastTime = curTime;

        ReleaseMutex(hMutex);
    }
}

static void beaconClientParseSentryClientCmds(BeaconSentryClientData* client, char* cmds){
    char* cmd;
    char delim;
    BeaconSentryClientData* clientBackup = client;
    
    for(cmd = strsep2(&cmds, "\n", &delim); cmd; cmd = strsep2(&cmds, "\n", &delim)){
        char* cmdName;

        for(cmdName = cmd = findNonSpace(cmd); *cmd && !isspace((unsigned char)*cmd); cmd++);
        if(*cmd){
            *cmd++ = 0;
        }
        cmd = findNonSpace(cmd);
        
        if(!stricmp(cmd, "<<") && delim){
            char* start = cmd + 3;
            
            for(cmd = strsep2(&cmds, "\n", &delim); cmd; cmd = strsep2(&cmds, "\n", &delim)){
                if(!stricmp(cmd, ">>")){
                    break;
                }
            }

            if(!cmd){
                cmd = cmds;
            }
            
            {
                char* cur = start;
                
                while(cur != cmd){
                    if(!*cur){
                        *cur = '\n';
                    }
                    cur++;
                }
                
                cur[-1] = 0;
            }
            
            assert(client == clientBackup);
            beaconClientParseSentryClientCmd(client, cmdName, start);
        }else{
            assert(client == clientBackup);
            beaconClientParseSentryClientCmd(client, cmdName, cmd);
        }
    }
}

static void beaconSentryDestroyClientData(BeaconSentryClientData* client){
    DisconnectNamedPipe(client->hPipe);
    CloseHandle(client->hPipe);
    
    estrDestroy(&client->crashText);
    
    estrDestroy(&client->serverUID);
}

static void beaconClientCheckSentry(void){
    if(beaconClientIsSentry()){
        S32 desiredClientCount = (beacon_client.cpuCount + 1);
        S32 needNewSentryClient = 0;
        S32 bytesRead;
        S32 i;
        
        if(    !beacon_client.timeStartedClient ||
            beacon_client.sentryClients.count == desiredClientCount)
        {
            // Reset the timer whenever there's enough clients.
            
            beacon_client.timeStartedClient = beaconGetCurTime();
        }
        else if(beacon_client.sentryClients.count < desiredClientCount){
            // Not enough clients.
            
            if(    !beaconIsProductionMode() &&
                beacon_client.userInactive &&
                beaconTimeSince(beacon_client.timeStartedClient) > 10)
            {
                beacon_client.timeStartedClient = beaconGetCurTime();
                
                // Start a new client.

                beaconStartNewExe(    beaconClientExeName,
                                    beacon_client.exeData,
                                    beacon_client.exeSize,
                                    beaconClientGetCmdLine(1, 0, 1),
                                    0, 0,
                                    hideClient);
            }
        }
        else if(!beaconIsProductionMode()) {
            // Too many clients!!!
            
            S32 toKillCount = beacon_client.sentryClients.count - desiredClientCount;
            
            beacon_client.timeStartedClient = beaconGetCurTime();

            // Discount clients that were already terminated.

            for(i = 0; toKillCount && i < beacon_client.sentryClients.count; i++){
                BeaconSentryClientData* client = beacon_client.sentryClients.clients + i;
                
                if(client->terminated){
                    toKillCount--;
                }
            }

            // Kill extra processes.
            
            for(i = 0; toKillCount && i < beacon_client.sentryClients.count; i++){
                BeaconSentryClientData* client = beacon_client.sentryClients.clients + i;
                
                if(client->processID && !client->crashText && !client->terminated){
                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, client->processID);
                    
                    if(hProcess){
                        toKillCount--;
                        beaconPrintf(COLOR_RED, "Terminating extra process: %d\n", client->processID);
                        client->terminated = 1;
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                    }
                }
            }
        }

        if(!beacon_client.sentryClients.count){
            createNewSentryClient();
        }
        
        for(i = 0; i < beacon_client.sentryClients.count;){
            BeaconSentryClientData* client = beacon_client.sentryClients.clients + i;
            BeaconSentryClientData* clientBackup = client;
            S32 gotoNext = 1;
            S32 readCount = 0;
            char inBuffer[20000];
            
            while(readCount < 10 && ReadFile(client->hPipe, inBuffer, ARRAY_SIZE(inBuffer) - 1, &bytesRead, NULL)){
                readCount++;
                
                assert(client == clientBackup);
                
                assert(bytesRead <= ARRAY_SIZE(inBuffer));
                
                if(bytesRead){
                    S32 wasCrashed = client->crashText ? 1 : 0;
                    
                    inBuffer[bytesRead] = 0;
                    
                    beaconClientParseSentryClientCmds(client, inBuffer);
                    
                    if(!wasCrashed && client->crashText){
                        beaconPrintf(COLOR_GREEN,
                                    "client %d:%d:%d\nCRASH!!!!!!!!!!!!!!!!!!!!!!\n%s\n",
                                    client,
                                    client->hPipe,
                                    client->processID,
                                    client->crashText);
                    }

                    if(    readCount == 1 &&
                        i == beacon_client.sentryClients.count - 1)
                    {
                        needNewSentryClient = 1;
                    }
                }
            }

            if(readCount < 10){
                S32 error = GetLastError();
                
                switch(error){
                    xcase ERROR_BROKEN_PIPE:
                    //case ERROR_PIPE_BUSY:
                    case ERROR_BAD_PIPE:{
                        beaconPrintf(COLOR_RED,
                                    "Client disconnected: pid %d, pipe %d, error %d\n",
                                    client->processID, client->hPipe, error);
                        
                        beaconSentryDestroyClientData(client);
                        
                        CopyStructs(beacon_client.sentryClients.clients + i,
                                    beacon_client.sentryClients.clients + i + 1,
                                    --beacon_client.sentryClients.count - i);
                                
                        beacon_client.sentSentryClients = 0;
                        
                        gotoNext = 0;
                    }
                    
                    xcase ERROR_PIPE_LISTENING:
                    case ERROR_NO_DATA:{
                        // These are normal messages for an open pipe.
                    }
                    
                    xdefault:{
                        printf("Unhandled error on pipe %d, handle %d: %d\n", i, client->hPipe, error);
                    }
                }
            }
                    
            //printf("failed: %d\n", GetLastError());
            
            if(gotoNext){
                i++;
            }
        }
        
        if(needNewSentryClient){
            createNewSentryClient();
        }
        
        if(    !beacon_client.sentSentryClients &&
            beacon_client.serverLink.connected &&
            beacon_client_conn.readyToWork)
        {
            BEACON_CLIENT_PACKET_CREATE(BMSG_C2ST_SENTRY_CLIENT_LIST);

                S32 i;
                
                beacon_client.sentSentryClients = 1;
                
                for(i = 0; i < beacon_client.sentryClients.count - 1; i++){
                    BeaconSentryClientData* client = beacon_client.sentryClients.clients + i;
                    pktSendBits(pak, 1, 1);
                    pktSendBits(pak, 32, client->myServerID);
                    pktSendString(pak, client->serverUID ? client->serverUID : "none");
                    pktSendBits(pak, 32, client->processID);
                    pktSendBits(pak, 1, client->forcedInactive);
                    
                    printf(    "sent client %d:%d:%d: myServerID=%d, serverUID=%s\n%s\n",
                            client,
                            client->hPipe,
                            client->processID,
                            client->myServerID,
                            client->serverUID,
                            client->crashText);
                    
                    if(client->crashText){
                        pktSendBits(pak, 1, 1);
                        pktSendString(pak, client->crashText);
                    }else{
                        pktSendBits(pak, 1, 0);
                    }
                }
                
                pktSendBits(pak, 1, 0);
            
            BEACON_CLIENT_PACKET_SEND();
        }
    }else{
        static U32 checkSentryTime;
        
        if(timerSeconds(timerCpuTicks() - checkSentryTime) > 0.5){
            S32 sendProcessID = !beacon_client.hPipeToSentry;

            beaconClientOpenPipeToSentry();

            if(beacon_client.hPipeToSentry){
                if(!beacon_client.sentServerID){
                    beacon_client.sentServerID = 1;
                    
                    printfPipe(0, "MyServerID %d\nServerUID %s", beacon_client.myServerID, beacon_client.serverUID);
                }else{
                    printfPipe(0, "Time %d", time(NULL));
                }
            }
            
            if(!beaconIsProductionMode()){
                beaconClientCheckForSentry();
            }
        }
    }
}

static void beaconClientPrintSentryClients(void){
    S32 i;

    if(!beaconClientIsSentry()){
        return;
    }

    beaconPrintf(    COLOR_GREEN,
                    "\n\n"
                    "%-12s%-12s%-12s%-1s\n",
                    "PID",
                    "TimeDelta",
                    "MyServerID",
                    "ServerUID");
    
    for(i = 0; i < beacon_client.sentryClients.count - 1; i++){
        BeaconSentryClientData* client = beacon_client.sentryClients.clients + i;
        S32 state;
        S32 curInstances;
        S32 maxCollectionCount;
        S32 collectDataTimeout;
        char userName[100];
        char timeString[100];
        
        sprintf(timeString, "%ds", time(NULL) - client->lastSentTime);
        
        printf("%-12d%-12s%-12d%-1s\n", client->processID, timeString, client->myServerID, client->serverUID);
        
        GetNamedPipeHandleStateA(beacon_client.sentryClients.clients[i].hPipe,
                                 &state,
                                 &curInstances,
                                 &maxCollectionCount,
                                 &collectDataTimeout,
                                 userName,
                                 ARRAY_SIZE(userName));
                                
        if(0)printf(    "%d, %d, %d, %d, %d, %s\n",
                beacon_client.sentryClients.clients[i].hPipe,
                state,
                curInstances,
                maxCollectionCount,
                collectDataTimeout,
                userName);
    }

    printf("\n\n");
}

static void beaconClientSetForcedInactive(S32 on, char* reason){
    on = on ? 1 : 0;
    
    if(beacon_client.userInactive != on){
        beaconPrintf(    COLOR_GREEN,
                        "Switched to %s: %s\n",
                        on ? "INACTIVE" : "ACTIVE",
                        reason);

        BEACON_CLIENT_PACKET_CREATE(BMSG_C2ST_USER_INACTIVE);
        
            pktSendBits(pak, 1, on);
            pktSendString(pak, reason);

        BEACON_CLIENT_PACKET_SEND();
            
        beacon_client.userInactive = on;

        printfPipe(0, "ForcedInactive %d", on);
    }
}

static void beaconClientRunRadiosityClient(void){
    time_t curTime = time(NULL);
    const char* commandText = "C:\\RadiosityClient\\RadiosityClient.exe";
    char* timeString = asctime(localtime(&curTime));
    
    timeString[strlen(timeString) - 1] = 0;
    
    printf("%s: Running radiosity client: %s\n", timeString, commandText);
    
    ShellExecuteA(NULL, "open", commandText, "", "", SW_NORMAL);
    
    printf("Done running radiosity client.\n");
}

static void setEnvironmentVar(StashTable st, const char* name, const char* valueParam, int concatPath){
    StashElement    element;
    char            value2[10000];
    char            value[10000];
    
    strcpy(value, valueParam);
    
    if(stashFindElement(st, name, &element)){
        char* oldValue = stashElementGetPointer(element);
        
        if(    !stricmp(name, "path") &&
            concatPath)
        {
            S32 oldLen = (S32)strlen(oldValue);
            
            memmove(value + oldLen + 1, value, strlen(value) + 1);
            
            strcpy(value, oldValue);
            
            value[oldLen] = ';';
        }
                        
        free(oldValue);
    }

    ExpandEnvironmentStringsA(value, value2, sizeof(value2));

    stashAddPointer(st, name, strdup(value2), true);

    //printfColor(COLOR_BRIGHT|COLOR_GREEN, "%s=%s\n", name, value2);
}

static void getDefaultEnvironmentHelper(StashTable st, const char* keyName){
    RegReader    rr = createRegReader();
    S32            i;

    initRegReader(rr, keyName);
    
    for(i = 0;; i++){
        char    name[1000];
        S32        nameLen = sizeof(name);
        char    value[10000];
        S32        valueLen = sizeof(value);
        S32        retVal;
        
        retVal = rrEnumStrings(rr, i, name, &nameLen, value, &valueLen);
        
        if(retVal < 0){
            break;
        }
        else if(retVal > 0){
            setEnvironmentVar(st, name, value, 1);
        }
    }
    
    destroyRegReader(rr);
}

static void freeValue(void* value){
    free(value);
}

static void* getDefaultEnvironment(void){
    StashTable            st = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);
    StashTableIterator    it;
    StashElement        element;
    char*                buffer = NULL;
    S32                    bufferLen = 0;
    S32                    bufferMaxLen = 0;
    char*                newLine;
    char*                curEnv = GetEnvironmentStrings();
    char*                curEnvEnd = curEnv;
    char                line[10000];
    
    // Start with all the current environment strings.
    
    while(curEnvEnd[0]){
        char* start = curEnvEnd;
        char* equal;
        
        curEnvEnd += strlen(curEnvEnd) + 1;
        
        strcpy(line, start);
        
        equal = strstr(line, "=");
        
        if(equal && line[0]){
            *equal = 0;
            
            setEnvironmentVar(st, line, equal + 1, 0);
        }
    }
    
    FreeEnvironmentStringsA(curEnv);
    
    // Now copy over with the default environment.
    
    getDefaultEnvironmentHelper(st, "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment");
    getDefaultEnvironmentHelper(st, "HKEY_CURRENT_USER\\Environment");
    
    for(stashGetIterator(st, &it); stashGetNextElement(&it, &element); ){
        sprintf(line, "%s=%s", stashElementGetStringKey(element), (char*)stashElementGetPointer(element));
        
        //printf("%s\n", line);
        
        newLine = dynArrayAddStructs(&buffer, &bufferLen, &bufferMaxLen, (int)strlen(line) + 1);
        
        memmove(newLine, line, strlen(line) + 1);
    }
    
    newLine = dynArrayAddStructs(&buffer, &bufferLen, &bufferMaxLen, 1);
    
    newLine[0] = 0;
    
    stashTableDestroyEx(st, NULL, freeValue);
    
    return buffer;
}

static void beaconClientKillSentryProcesses(S32 killActive, S32 killCrashed){
    S32 i;
    
    if(!beaconClientIsSentry()){
        return;
    }
    
    for(i = 0; i < beacon_client.sentryClients.count; i++){
        BeaconSentryClientData* client = beacon_client.sentryClients.clients + i;
        
        if(    !client->terminated
            &&
            (    killCrashed &&
                client->crashText
                ||
                killActive &&
                !client->crashText))
        {
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, client->processID);
            
            if(hProcess){
                beaconPrintf(COLOR_RED, "Terminating process: %d\n", client->processID);
                client->terminated = 1;
                TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);
            }
        }
    }
}

static void beaconClientCheckForUserActivity(void){
    static U32 goActiveTime;
    static U32 runRadiosityTime;

    if(!goActiveTime){
        goActiveTime = timerCpuSpeed() * 5 * 60;
        runRadiosityTime = timerCpuSpeed() * 15 * 60;
        beacon_client.timeSinceUserActivity = goActiveTime;
    }

    if(!beacon_client_conn.readyToWork){
        return;
    }
    
    if(beacon_client.workDuringUserActivity){
        beaconClientSetForcedInactive(1, "Work during user activity is enabled.");
        beacon_client.timeSinceUserActivity = max(goActiveTime, beacon_client.timeSinceUserActivity);
    }
    else if(beaconClientIsSentry()){
        // Only sentries will monitor for activity.
        
        static S32        beenHereBefore;
        static POINT    ptLast;
        static U32        lastTime;
        
        POINT ptCur = {0};
        U32 curTime = timerCpuTicks();

        GetCursorPos(&ptCur);
        
        if(!beenHereBefore){
            beenHereBefore = 1;
            ptLast = ptCur;
            lastTime = curTime;
        }

        if(    (    beacon_client.keyboardPressed ||
                abs(ptCur.x - ptLast.x) > 10 ||
                abs(ptCur.y - ptLast.y) > 10))
        {
            // A key was pressed or the mouse was moved.
            
            beaconClientSetForcedInactive(0, beacon_client.keyboardPressed ? "Keyboard pressed." : "Mouse moved.");

            ptLast = ptCur;
            beacon_client.timeSinceUserActivity = 0;
            beacon_client.keyboardPressed = 0;
            
            if(!beaconIsProductionMode()){
                beaconClientKillSentryProcesses(1, 0);
            }
        }else{
            U32 delta = curTime - lastTime;
            
            beacon_client.timeSinceUserActivity += delta;

            if(    0 &&
                !beaconIsProductionMode() &&
                beacon_client.timeSinceUserActivity >= runRadiosityTime &&
                beaconTimeSince(beacon_client.timeStartedRadiosityClient) >= 15 * 60)
            {
                beacon_client.timeStartedRadiosityClient = beaconGetCurTime();
                
                beaconClientRunRadiosityClient();
            }
            
            if(beacon_client.timeSinceUserActivity >= goActiveTime){
                beaconClientSetForcedInactive(1, "No user activity for a while.");
            }
            else{
                beaconClientSetForcedInactive(0, "Timer still going down.");
            }
        }
                
        lastTime = curTime;
    }
}

static void beaconClientConnectIdleCallback(F32 timeLeft){
    if(timeLeft){
        //beaconClientUpdateTitle("Connecting...", timeLeft);
    }
    
    beaconClientCheckForUserActivity();
    
    //printf("waiting %1.2f\n", timeLeft);
    
    if(IsWindowVisible(beaconGetConsoleWindow()) && _kbhit()){
        switch(tolower(_getch())){
            xcase 27:{
                printf(    "\n"
                        "a : Toggle work during user activity.\n"
                        "c : Print client list.\n"
                        "d : Foribly disconnect.\n"
                        "h : Hide window (also hides when minimized).\n"
                        "m : Print memory usage.\n"
                        "r : Run radiosity client.\n"
                        "t : Run collision test.\n"
                        "x : Prompted to cause intentional crash.\n"
                        "\n");
            }
            
            xcase 'a':{
                if(beaconClientIsSentry()){
                    S32 set = beacon_client.workDuringUserActivity = !beacon_client.workDuringUserActivity;
                    
                    beaconPrintf(    COLOR_GREEN,
                                    "Setting work during user activity: %s\n",
                                    set ? "ON" : "OFF");
                }                    
            }
            
            xcase 'c':{
                beaconClientPrintSentryClients();
            }
            
            xcase 'd':{
                if(beacon_client.serverLink.connected){
                    printf("Disconnecting because of keypress: ");
                    netSendDisconnect(&beacon_client.serverLink, 0);
                    printf("Done!\n");
                }
            }
            
            xcase 'h':{
                if(!beaconIsProductionMode()){
                    ShowWindow(beaconGetConsoleWindow(), SW_HIDE);
                }
            }
            
            xcase 'm':{
                beaconPrintMemory();
            }

            xcase 'r':{
                beaconClientRunRadiosityClient();
            }

            xcase 't':{
                beaconTestCollision();
            }
            
            xcase 'x':{
                char buffer[6];
                
                beaconPrintf(COLOR_YELLOW, "Enter \"CRASH\" to crash: ");
                
                if(beaconEnterString(buffer, 5) && !strncmp(buffer, "CRASH", 5)){
                    beaconPrintf(COLOR_RED, "\nCrashing intentionally!\n\n");
                    assertmsg(0, "Intentional beacon client crash!");
                    beaconPrintf(COLOR_RED, "\nDone crashing intentionally, continuing execution.\n\n");
                }else{
                    printf("Canceled!\n\n");
                }
            }
        }
    }

    beaconClientCheckSentry();
}

static void beaconClientSetServerID(U32 clientUID, const char* serverUID){
    if(    beacon_client.myServerID != clientUID ||
        !beacon_client.serverUID ||
        stricmp(beacon_client.serverUID, serverUID))
    {
        beacon_client.myServerID = clientUID;
        estrPrintCharString(&beacon_client.serverUID, serverUID);
        
        printf("New server ID: %s:%d\n", serverUID, beacon_client.myServerID);

        beacon_client.sentServerID = 0;
    }
}

static void beaconClientProcessMsgLegalAreas(Packet* pak){
    S32 grid_x = pktGetBitsPack(pak, 1);
    S32 grid_z = pktGetBitsPack(pak, 1);
    BeaconDiskSwapBlock* block = beaconGetDiskSwapBlockByGrid(grid_x, grid_z);
    BeaconDiskSwapBlock* cur;
    
    assert(block);
    
    beaconClientReceiveLegalAreas(pak, block);
    
    // Clear the addedLegal flag.
    
    for(cur = bp_blocks.list; cur; cur = cur->nextSwapBlock){
        cur->addedLegal = 0;
    }
    
    printf(    "Processing %4d legal areas in (%4d, %4d): ",
            block->legalCompressed.totalCount,
            grid_x,
            grid_z);
    
    beaconClientUpdateTitle("Processing block (%d, %d)", grid_x, grid_z);

    beaconClientSetPriorityLevel(0);
    beaconProcessLegalAreas(block);
    beaconClientSetPriorityLevel(1);
    
    block->addedLegal = 1;
    
    printf(", Done!\n");
    
    beaconClientSendLegalAreas(grid_x, grid_z);
    
    beaconClearNonAdjacentSwapBlocks(NULL);
}

static void beaconClientProcessMsgBeaconList(Packet* pak){
    S32 count = pktGetBitsPack(pak, 1);
    S32 i;
    
    printf("Receiving beacon list (%s bytes).\n", getCommaSeparatedInt(pak->stream.size));
    
    beaconFreeUnusedMemoryPools();

    assert(!combatBeaconArray.size);
    
    for(i = 0; i < count; i++){
        Vec3 pos;
        Beacon* b;
        
        pktGetBitsArray(pak, 8 * sizeof(Vec3), pos);
        
        b = addCombatBeacon(pos, 1, 0, 0);
        
        assert(b);
        
        b->userInt = i;
        b->noGroundConnections = pktGetBits(pak, 1);
    }
    
    assert(combatBeaconArray.size == count);
    
    beacon_process.infoArray = beaconAllocateMemory(combatBeaconArray.size * sizeof(*beacon_process.infoArray));
}

static void beaconClientProcessMsgTransferToServer(Packet* pak){
    char serverName[100];
    
    Strncpyt(serverName, pktGetString(pak));
    
    SAFE_FREE(beacon_client.subServerName);
    
    beacon_client.subServerName = strdup(pktGetString(pak));
    beacon_client.subServerPort = pktGetBitsPack(pak, 1);

    if(beacon_client.serverLink.connected){
        printf("Disconnecting to transfer to %s:%d: ", beacon_client.subServerName, beacon_client.subServerPort);
        netSendDisconnect(&beacon_client.serverLink, 0);
        printf("Done!\n");
        
        beaconPrintf(    COLOR_GREEN,
                        "Transferring to subserver: %s/%s:%d\n",
                        serverName,
                        beacon_client.subServerName,
                        beacon_client.subServerPort);
    }
}

static void beaconClientProcessMsgKillProcesses(Packet* pak){
    S32 justCrashed = pktGetBits(pak, 1);
    
    beaconClientKillSentryProcesses(!justCrashed, 1);
}

static void beaconClientProcessMsgMapData(Packet* pak){
    beaconClientSetPriorityLevel(0);

    beaconClientReceiveMapData(pak);

    beaconClientSetPriorityLevel(1);
}

static void beaconClientProcessMsgExeData(Packet* pak){
    U32 curReceived = pktGetBitsPack(pak, 1);
    U32 totalBytes = pktGetBitsPack(pak, 1);
    U32 curBytes = pktGetBitsPack(pak, 1);

    dynArrayFitStructs(    &beacon_client.newExeData.data,
                        &beacon_client.newExeData.maxTotalByteCount,
                        totalBytes);

    beaconPrintf(    COLOR_GREEN,
                    "Receiving %s/%s bytes of new exe.\n",
                    getCommaSeparatedInt(curReceived + curBytes),
                    getCommaSeparatedInt(totalBytes));
    
    pktGetBitsArray(pak, 8 * curBytes, (U8*)beacon_client.newExeData.data + curReceived);
    
    if(curReceived + curBytes == totalBytes){
        beaconPrintf(COLOR_GREEN, "Starting new exe!\n");
        
        beaconStartNewExe(    beaconClientExeName,
                            beacon_client.newExeData.data,
                            totalBytes,
                            beaconClientGetCmdLine(1, 1, 1), 
                            1,
                            !beaconClientIsSentry(), hideClient);
    }else{
        BEACON_CLIENT_PACKET_CREATE(BMSG_C2ST_NEED_MORE_EXE_DATA);

            pktSendBitsPack(pak, 1, curReceived + curBytes);

        BEACON_CLIENT_PACKET_SEND();
    }
}

static void beaconClientProcessMsgMapDataLoadedReply(Packet* pak){
    S32 good = pktGetBits(pak, 1);
    
    if(good){
        printf("Server accepted map CRC, the fool!\n");
    }else{
        printf("Server rejected map CRC, the fool!\n");
        assertmsg(0, "Server rejected map CRC.");
    }
}

static void beaconClientProcessMsgExecuteCommand(Packet* pak){
    char commandText[1000];
    U32 showWindowType;
    
    Strncpyt(commandText, pktGetString(pak));
    showWindowType = pktGetBitsPack(pak, 1);
    
    printf("Running command from server: %s\n", commandText);
    
    if(0){
        ShellExecuteA(NULL, "open", commandText, "", "", showWindowType);
    }else{
        STARTUPINFOA si = {0};
        PROCESS_INFORMATION pi;
        BOOL ret;
        
        si.cb = sizeof(si);
        si.dwFlags |= STARTF_USESHOWWINDOW;
        si.wShowWindow = showWindowType;
        
        ret = CreateProcessA(NULL,
                            commandText,
                            NULL,
                            NULL,
                            FALSE,
                            NORMAL_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP | CREATE_NEW_CONSOLE,
                            NULL,
                            NULL,
                            &si,
                            &pi);
                            
        if(!ret){
            beaconPrintf(COLOR_RED, "Failed %d!\n", GetLastError());
        }
    }
        
    printf("Finished running command from server: %s\n", commandText);
}

static void beaconClientProcessMsgPing(Packet* pak){
    // Don't do anything.
}

static void beaconClientProcessMsgTextCmd(const char* textCmd, Packet* pak){
    #define BEGIN_HANDLERS()        if(0){
    #define HANDLER(x, y)            }else if(!stricmp(textCmd, x)){y(pak);beacon_client.timeOfLastCommand = timerCpuTicks()
    #define HANDLER_NO_TIME(x, y)    }else if(!stricmp(textCmd, x)){y(pak)
    #define END_HANDLERS()            }
    
    BEGIN_HANDLERS();
        HANDLER(BMSG_S2CT_KILL_PROCESSES,            beaconClientProcessMsgKillProcesses        );
        HANDLER(BMSG_S2CT_MAP_DATA,                    beaconClientProcessMsgMapData            );
        HANDLER(BMSG_S2CT_MAP_DATA_LOADED_REPLY,    beaconClientProcessMsgMapDataLoadedReply);
        HANDLER(BMSG_S2CT_EXE_DATA,                    beaconClientProcessMsgExeData            );
        HANDLER(BMSG_S2CT_PROCESS_LEGAL_AREAS,        beaconClientProcessMsgLegalAreas        );
        HANDLER(BMSG_S2CT_BEACON_LIST,                beaconClientProcessMsgBeaconList        );
        HANDLER(BMSG_S2CT_CONNECT_BEACONS,            beaconClientReceiveAndConnectBeacons    );
        HANDLER(BMSG_S2CT_TRANSFER_TO_SERVER,        beaconClientProcessMsgTransferToServer    );
        HANDLER(BMSG_S2CT_EXECUTE_COMMAND,            beaconClientProcessMsgExecuteCommand    );
        HANDLER_NO_TIME(BMSG_S2CT_PING,                beaconClientProcessMsgPing                );
    END_HANDLERS();

    #undef BEGIN_HANDLERS
    #undef HANDLER
    #undef END_HANDLERS
}

static S32 beaconClientHandleMsg(Packet* pak, S32 cmd, NetLink* link){
    beacon_client_conn.timeHeardFromServer = timerCpuTicks();

    switch(cmd){
        xcase BMSG_S2C_CONNECT_REPLY:{
            if(pktGetBits(pak, 1) == 1){
                // Connection accepted.
                
                if(!beacon_client_conn.readyToWork){
                    U32 clientUID = pktGetBits(pak, 32);
                    const char* serverUID = pktGetString(pak);
                    
                    beaconClientSetServerID(clientUID, serverUID);
                    
                    printf("Connected!  Server: %s\n", beaconGetLinkIPStr(link));
                }
            
                sendReadyMessage();
            }else{
                if(BEACON_CLIENT_PROTOCOL_VERSION < 1){
                    // Get a new executable.
                    
                    beaconClientSetPriorityLevel(1);
                    beaconHandleNewExe(    pak,
                                        beaconClientExeName,
                                        beaconClientGetCmdLine(1, 1, 1),
                                        !beaconClientIsSentry(),
                                        hideClient);
                }
            }
        }
        
        xcase BMSG_S2C_TEXT_CMD:{
            char textCmd[100];
            
            strncpyt(textCmd, pktGetString(pak), ARRAY_SIZE(textCmd) - 1);
            
            beaconClientProcessMsgTextCmd(textCmd, pak);
        }
        
        xdefault:{
            beaconPrintf(COLOR_RED, "Unknown command from server %d\n", cmd);
        }
    }
    
    beacon_client_conn.timeHeardFromServer = timerCpuTicks();
    
    return 1;
}

char* beaconGetExeFileName(void){
    static char* fileName;
    
    if(!fileName){
        fileName = beaconMemAlloc("fileName", 1000);
        GetModuleFileNameA(NULL, fileName, 999);
        forwardSlashes(fileName);
    }
    return fileName;
}

static void beaconClientSendConnect(void){
    Packet* pak = pktCreateEx(&beacon_client.serverLink, BMSG_C2S_CONNECT);
    
    //beaconMakeNetLinkBeFast(&beacon_client.serverLink);
    
    pktSendBits(pak, 32, 0);
    
    // Send the protocol version.
    
    pktSendBitsPack(pak, 1, BEACON_CLIENT_PROTOCOL_VERSION);
    
    pktSendBits(pak, 32, beacon_client.executableCRC);
    
    pktSendString(pak, getUserName());
    pktSendString(pak, getComputerName());
    
    pktSendBits(pak, 1, beaconClientIsSentry() ? 1 : 0);
    
    pktSend(&pak, &beacon_client.serverLink);
}

static U32 fakeCRC(char* data, S32 length){
    U32 acc = 0;
    while(length){
        acc += ((*data + length) * length) + *data;
        data++;
        length--;
    }
    return acc;
}

static void printFlags(U32 flags){
    #define CASE(x) if(flags & x){printf("%s,", #x + 5);}else{U32 i;for(i=0;0&&i<=strlen(#x+5);i++)printf(" ");}
    CASE(PAGE_EXECUTE);
    CASE(PAGE_EXECUTE_READ);
    CASE(PAGE_EXECUTE_READWRITE);
    CASE(PAGE_EXECUTE_WRITECOPY);
    CASE(PAGE_GUARD);
    CASE(PAGE_NOACCESS);
    CASE(PAGE_NOCACHE);
    CASE(PAGE_READONLY);
    CASE(PAGE_READWRITE);
    CASE(PAGE_WRITECOMBINE);
    CASE(PAGE_WRITECOPY);
    #undef CASE
}

static void beaconClientAssertCallback(char* errMsg){
    S32 sentStuff = 0;
    S32 done = 0;
    S32 printMenu = 1;
    
    beaconPrintf(COLOR_RED, "Crash:\n%s\n", errMsg);

    while(!done){
        Sleep(1000);
        
        beaconClientCheckSentry();
        
        if(!beacon_client.hPipeToSentry){
            sentStuff = 0;
        }else{
            if(!sentStuff){
                sentStuff = 1;
                
                printfPipe(0, "ProcessID %d\nCrash <<\n%s\n>>", _getpid(), errMsg);
            }
        }
        
        if(printMenu){
            printMenu = 0;
            
            beaconPrintf(COLOR_GREEN,
                        "\n\nCrash menu:\n"
                        "\n"
                        "m    : Memory display.\n"
                        "d    : Open crash dialog.\n"
                        "\n"
                        "Enter selection: ");
        }
        
        if(_kbhit()){
            switch(_getch()){
                xcase 0:
                case 224:{
                    _getch();
                }
                
                xcase 'm':{
                    beaconPrintf(COLOR_YELLOW, "Memory stats...\n");
                    beaconPrintMemory();
                    printMenu = 1;
                }
                
                xcase 'd':{
                    beaconPrintf(COLOR_YELLOW, "Showing window and opening assert dialog box...\n");
                    ShowWindow(beaconGetConsoleWindow(), SW_RESTORE);
                    done = 1;
                }
            }
        }
    }
}

static void beaconClientInstall(void){
    HANDLE hMutex;
    
    if(    beaconIsProductionMode() ||
        beacon_client.masterServerName)
    {
        return;
    }
    
    hMutex = CreateMutexA(NULL, 0, "Global\\CrypticBeaconClientInstall");
    
    assert(hMutex);
    
    if(beaconAcquireMutex(hMutex)){
        char buffer[1000];

        sprintf(buffer, "c:/Beaconizer/%s", beaconClientExeName);
        
        // Check if I am not the installed file (c:/beaconizer/BeaconClient.exe).
        
        if(stricmp(buffer, beaconGetExeFileName())){
            RegReader reader;
            char* key = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
            char* value = "BeaconClient";
            
            //printf("Installing BeaconClient in registry: \n%s\n%s\n", buffer, beaconGetExeFileName());

            beaconCreateNewExe(buffer, beacon_client.exeData, beacon_client.exeSize);

            sprintf(buffer, "c:\\Beaconizer\\%s -beaconclient -noencrypt", beaconClientExeName);
            
            consoleSetColor(COLOR_BRIGHT|COLOR_RED|COLOR_GREEN, 0);
            printf(    "Setting registry entry:\n"
                    "  Key:   \"%s\\%s\"\n"
                    "  Value: \"%s\"\n",
                    key,
                    value,
                    buffer);
            consoleSetDefaultColor();

            reader = createRegReader();
            initRegReader(reader, key);
            rrWriteString(reader, value, buffer);
            destroyRegReader(reader);
        }
    }
    
    beaconReleaseAndCloseMutex(&hMutex);
}

static void beaconClientConnectToDBServer(void){
    if(!beaconIsProductionMode()){
        return;
    }
    
    if(!db_state.server_name || !db_state.server_name[0]){
        assertmsg(0, "No dbserver defined (\"-db <server>\" cmdline option)!");
    }else{
        beaconPrintf(COLOR_GREEN, "Connecting to dbserver: ");
        netConnect(&beacon_client.db_link, db_state.server_name, DEFAULT_BEACONCLIENT_PORT, NLT_TCP, 30.0f, NULL);
        if(beacon_client.db_link.connected){
            beaconPrintf(COLOR_GREEN, "Done!\n");
        }else{
            beaconPrintf(COLOR_RED, "FAILED!\n");
            exit(0);
        }
    }
}

static void beaconClientStartup(const char* masterServerName, const char* subServerName){
    pktSetDebugInfo();
    
    // Hide if not in production mode.
    
    if(!beaconIsProductionMode()){
        ShowWindow(beaconGetConsoleWindow(), hideClient ? SW_HIDE : SW_SHOW);
    }

    // Check production mode.
    
    printf("Production mode: ");
    
    if(beaconIsProductionMode()){
        U32 ips[2] = {0};
        
        beacon_client.workDuringUserActivity = 1;
        
        beaconPrintf(COLOR_GREEN, "YES\n");
    }else{
        beaconPrintf(COLOR_RED, "NO\n");
    }
    
    // Check master server.
    
    if(masterServerName){
        beacon_client.masterServerName = strdup(masterServerName);
        
        beaconPrintf(COLOR_GREEN, "Using master server: %s\n", masterServerName);
    }
    
    // Check sub server.
    
    if(subServerName){
        beacon_client.subServerName = strdup(subServerName);

        beaconPrintf(COLOR_GREEN, "Using sub server: %s\n", subServerName);
    }
    
    // Copy the executable data.
    
    if(!beaconIsProductionMode()){
        beacon_client.exeData = beaconFileAlloc(beaconGetExeFileName(), &beacon_client.exeSize);
        
        assert(beacon_client.exeData);
    }
    
    // Install the beacon client on this machine.
    
    beaconClientInstall();

    // Figure out if I'm the sentry.
    
    if(beaconClientAcquireSentryMutex()){
        SYSTEM_INFO si;
        
        SAFE_FREE(beacon_client.subServerName);
        
        // I'm the sentry!
        
        setWindowIconColoredLetter(beaconGetConsoleWindow(), 'S', 0x30ff30);
        
        GetSystemInfo(&si);

        beacon_client.cpuCount = si.dwNumberOfProcessors;
        
        consoleSetColor(COLOR_BRIGHT|COLOR_GREEN, 0);
        printf("I'm the sentry!!!  (%d CPU%s)\n", beacon_client.cpuCount, beacon_client.cpuCount > 1 ? "s" : "");
        consoleSetDefaultColor();
    }else{
        setWindowIconColoredLetter(beaconGetConsoleWindow(), 'W', 0x30ff30);

        consoleSetColor(COLOR_BRIGHT|COLOR_RED, 0);
        printf("I'm not the sentry!!!\n");
        consoleSetDefaultColor();
    }
    
    beaconClientSetPriorityLevel(1);

    _beginthreadex(NULL, 0, beaconClientWindowThread, NULL, 0, NULL);
    
    if(!beaconIsProductionMode()){
        if(!checkForCorrectExePath(    beaconClientExeName,
                                    beaconClientGetCmdLine(1, 1, 1),
                                    !beaconClientIsSentry(),
                                    hideClient))
        {
            assert(0);
        }
    }
    
    // Connect to the dbserver.
    
    beaconClientConnectToDBServer();

    // Disable the assert dialog box for clients.
    
    if(    !beaconIsProductionMode() &&
        !beaconClientIsSentry())
    {
        setAssertMode(getAssertMode() | ASSERTMODE_CALLBACK);
        setAssertCallback(beaconClientAssertCallback);
    }

    server_state.beaconProcessOnLoad = 1;
    
    beaconInitCommon(!beaconClientIsSentry());

    beacon_process.entity = beaconCreatePathCheckEnt();

    beacon_client.executableCRC = beaconGetExeCRC(NULL, NULL);
    
    printf("CRC of \"%s\" = 0x%8.8x\n", beaconGetExeFileName(), beacon_client.executableCRC);

    printf(    "\n\n[컴컴컴컴컴컴 BEACON %s RUNNING 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�]\n",
            beaconClientIsSentry() ? "SENTRY" : "CLIENT");

    beacon_client_conn.timeHeardFromServer = timerCpuTicks();
}

static void beaconClientConnectToServer(void){
    static S32 wasConnected = 0;
    
    S32 isMasterServer = 0;
    char serverName[1000];
    char* portString;
    S32 port;
    
    if(beacon_client.serverLink.connected){
        return;
    }
    
    beaconClientSetPriorityLevel(1);

    beaconClientSetServerID(0, "NoServer");

    beacon_client_conn.readyToWork = 0;
    beacon_client.sentSentryClients = 0;
    
    if(wasConnected){
        beaconResetReceivedMapData();
    }
    
    // Clear out the subserver name if necessary.
    
    if(beacon_client.connectedToSubServer){
        beacon_client.connectedToSubServer = 0;
        
        SAFE_FREE(beacon_client.subServerName);
    }
    
    // Get the server name.
    
    if(beacon_client.subServerName){
        strcpy(serverName, beacon_client.subServerName);
    }
    else if(beacon_client.masterServerName){
        strcpy(serverName, beacon_client.masterServerName);
        isMasterServer = 1;
    }
    else if(!beaconIsProductionMode()){
        strcpy(serverName, BEACON_MASTER_SERVER_DEFAULT_NAME);
    }
    else{
        exit(0);
        return;
    }
                    
    // Get the port.
    
    if(portString = strstr(serverName, ":")){
        *portString++ = 0;
    }
    
    port = portString ? atoi(portString) : beacon_client.subServerName ? beacon_client.subServerPort : BEACON_MASTER_SERVER_PORT;
    
    // Connect!

    beaconClientUpdateTitle("Connecting (%s:%d)...", serverName, port);
    
    netConnect(&beacon_client.serverLink, serverName, port, NLT_TCP, beaconIsProductionMode() ? 3 : 30, beaconClientConnectIdleCallback);
                
    if(beacon_client.serverLink.connected){
        wasConnected = 1;
        
        if(beacon_client.subServerName){
            beacon_client.connectedToSubServer = 1;
        }
        
        beaconClientUpdateTitle("Connected!!!");
        beaconClientSendConnect();
        
        beacon_client_conn.timeHeardFromServer = timerCpuTicks();
        beacon_client.timeOfLastCommand = timerCpuTicks();
        NMAddLink(&beacon_client.serverLink, beaconClientHandleMsg);
    }else{
        wasConnected = 0;
        
        SAFE_FREE(beacon_client.subServerName);
        
        beaconClientUpdateTitle("Connect failed!!!");
    }
}

static void beaconClientMonitorConnection(void){
    if(beaconIsProductionMode()){
        if(!beacon_client.db_link.connected){
            LOG_OLD("BeaconClient Detected dbserver shut down, shutting down.\n");
            logShutdownAndWait();
            exit(0);
        }
    }
    
    if(beacon_client.serverLink.connected){
        F32 timeSince = timerSeconds(timerCpuTicks() - beacon_client.timeOfLastCommand);
        
        if(timeSince <= 10){
            beaconClientUpdateTitle("%1.1fs", timeSince);
        }else{
            beaconClientUpdateTitle("Idling");
        }
    }else{
        beaconClientUpdateTitle(NULL);
    }
    
    if(    beacon_client.serverLink.connected &&
        timerSeconds(timerCpuTicks() - beacon_client_conn.timeHeardFromServer) > 20)
    {
        netSendDisconnect(&beacon_client.serverLink, 0);
    }
}

#if 0
static S32 showOptionsWindow()
{
    SUIMainWindow* mw;
    SUIWindow* w;
    
    if(suiMainWindowCreate(&mw, "Test")){
        if(suiWindowCreate(&w, NULL, NULL)){
            suiAddMainChildWindow(&mw, &w);
            suiSetWindowPos(&w, 10, 10);
            suiSetWindowSize(&w, 500, 300);
        }
        
        if(suiWindowCreate(&w, NULL, NULL)){
            suiAddMainChildWindow(&mw, &w);
            suiSetWindowPos(&w, 300, 250);
            suiSetWindowSize(&w, 400, 600);
        }

        if(suiWindowCreate(&w, NULL, NULL)){
            suiAddMainChildWindow(&mw, &w);
            suiSetWindowPos(&w, 150, 50);
            suiSetWindowSize(&w, 700, 400);
        }
    }

    suiProcessUntilDone(&mw);
    
    return 1;
}
#endif

void beaconClient(const char* masterServerName, const char* subServerName){
    //showOptionsWindow();
    
    beaconClientStartup(masterServerName, subServerName);

    while(1){
        NMMonitor(100);

        beaconClientConnectIdleCallback(0);
        
        beaconClientConnectToServer();
        
        beaconClientMonitorConnection();
    }
}

