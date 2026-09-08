
#include "beaconClientServerPrivate.h"
#include "beaconServerPrivate.h"
#include <utilitieslib/utils/utils.h>

#define    BEACON_CACHE_FILE_VERSION        (0)

typedef struct BeaconFileLoadRequest BeaconFileLoadRequest;

typedef struct BeaconFileLoadRequest {
    BeaconServerClientData*            client;

    struct {
        BeaconFileLoadRequest*        next;
        BeaconFileLoadRequest*        prev;
    } node;

    struct {
        char                        fileName[MAX_PATH];
        U32                            crc;
    } input;
    
    struct {
        U32                            fileVersion;
        
        struct {
            U8*                        data;
            U32                        byteCount;
            U32                        uncompressedByteCount;
        } beaconFile;
        
        U32                            success        : 1;
        U32                            complete    : 1;
    } output;
    
    U32                                refCount;
} BeaconFileLoadRequest;

static struct {
    CRITICAL_SECTION        cs;
    CONDITION_VARIABLE        cv;
    BeaconFileLoadRequest*    head;
    BeaconFileLoadRequest*    tail;
    U32                        count;
} load_requests;

MP_DEFINE(BeaconFileLoadRequest);

static void EnterLoadRequestCS(){
    EnterCriticalSection(&load_requests.cs);
}

static void LeaveLoadRequestCS(){
    LeaveCriticalSection(&load_requests.cs);
}

static void WakeLoadRequestConditionCS(){
    WakeConditionVariable(&load_requests.cv);
}

static void WaitLoadRequestConditionCS(){
    SleepConditionVariableCS(&load_requests.cv, &load_requests.cs, INFINITE);
}

BeaconFileLoadRequest* beaconFileLoadRequestCreate(){
    BeaconFileLoadRequest* request;
    
    EnterLoadRequestCS();
    
        MP_CREATE(BeaconFileLoadRequest, 1000);
    
        request = MP_ALLOC(BeaconFileLoadRequest);
    
    LeaveLoadRequestCS();
    
    return request;
}

void beaconFileLoadRequestDestroy(BeaconFileLoadRequest* request){
    if(!request){
        return;
    }
    
    EnterLoadRequestCS();
    
        SAFE_FREE(request->output.beaconFile.data);
        
        MP_FREE(BeaconFileLoadRequest, request);
    
    LeaveLoadRequestCS();
}

static void beaconServerGetRequestFileName(    char* fileName,
                                            const char* subDir,
                                            const char* partialFileName,
                                            const char* extension)
{
    sprintf(fileName,
            "%s"    // Cache dir.
            "/"
            "%s"    // Sub dir.
            "%s"    // Sub dir "/".
            "%s"    // Partial filename.
            "%s"    // Extension.
            ,
            estrLength(&beacon_server.requestCacheDir) ?
                beacon_server.requestCacheDir :
                "c:/beaconizer/requestcache",
            subDir && subDir[0] ? subDir : "",
            subDir && subDir[0] ? "/" : "",
            partialFileName,
            extension ? extension : "");
}

static void beaconServerFileLoadRequestComplete(BeaconFileLoadRequest* request){
    EnterLoadRequestCS();

        assert(request->refCount > 0);
        
        request->output.complete = 1;
        
        if(!--request->refCount){
            assert(!request->client);
            beaconFileLoadRequestDestroy(request);
        }
    
    LeaveLoadRequestCS();
}

void beaconServerDetachClientFromLoadRequest(BeaconServerClientData* client){
    BeaconFileLoadRequest* request;
    
    if(!client->requester.loadRequest){
        return;
    }

    EnterLoadRequestCS();
    
        request = client->requester.loadRequest;
        
        // Check if the thread hasn't begun processing yet.
        
        request->client = NULL;

        if(!--request->refCount){
            S32 decrement = 1;
            
            if(request->node.prev){
                request->node.prev->node.next = request->node.next;
            }
            else if(load_requests.head == request){
                load_requests.head = request->node.next;
            }
            else{
                assert(!request->node.next);
                assert(request != load_requests.tail);
                decrement = 0;
            }
            
            if(decrement){
                if(request->node.next){
                    request->node.next->node.prev = request->node.prev;
                }
                else if(load_requests.tail == request){
                    load_requests.tail = request->node.prev;
                }
                else{
                    assert(0);
                }

                assert(load_requests.count > 0);
                
                load_requests.count--;
            }
            
            ZeroStruct(&request->node);

            beaconFileLoadRequestDestroy(request);
        }
        
        client->requester.loadRequest = NULL;
    
    LeaveLoadRequestCS();
}

void beaconServerAddToBeaconFileLoadQueue(BeaconServerClientData* client, U32 crc){
    BeaconFileLoadRequest* request;
    
    beaconServerDetachClientFromLoadRequest(client);
    
    request = beaconFileLoadRequestCreate();
    
    request->refCount = 1;
    request->client = client;
    client->requester.loadRequest = request;
    
    request->input.crc = crc;
    Strncpyt(request->input.fileName, client->requester.processNode->uniqueFileName);
    
    EnterLoadRequestCS();
    
        if(load_requests.tail){
            load_requests.tail->node.next = request;
            request->node.prev = load_requests.tail;
        }else{
            load_requests.head = request;
        }
        
        load_requests.tail = request;
    
        load_requests.count++;

    LeaveLoadRequestCS();

    WakeLoadRequestConditionCS();
}

static void beaconServerMakeDirectories(const char* fileName){
    char fileDir[MAX_PATH];
    strcpy(fileDir, fileName);

    makeDirectories(getDirectoryName(fileDir));
}

void beaconServerWriteRequestedBeaconFile(BeaconProcessQueueNode* node){
    char fileName[MAX_PATH];
    FILE* f;
    
    beaconServerGetRequestFileName(fileName, NULL, node->uniqueFileName, ".beaconrequest");
    
    makeDirectoriesForFile(fileName);
    
    f = fopen(fileName, "wb");
    
    if(!f){
        return;
    }
    
    #define FWRITE_SIZE(x, size)    fwrite(x, size, 1, f)
    #define FWRITE(x)                FWRITE_SIZE(&x, sizeof(x))
    #define FWRITE_U32(x)            {U32 x_=x;FWRITE(x_);}
    
    // Write version.
    
    FWRITE_U32(BEACON_CACHE_FILE_VERSION);
    
    // Write crc.
    
    FWRITE_U32(beaconMapDataPacketGetCRC(node->mapData));
    
    // Write the compressed size.
    
    FWRITE_U32(node->beaconFile.byteCount);
    
    // Write the uncompressed size.
    
    FWRITE_U32(node->beaconFile.uncompressedByteCount);
    
    // Write the data.
    
    fwrite(node->beaconFile.data, node->beaconFile.byteCount, 1, f);
    
    // Close the file.

    fclose(f);

    // Done!

    #undef FWRITE_SIZE
    #undef FWRITE
    #undef FWRITE_U32
}

static void beaconServerSendLoadedBeaconFileToRequester(BeaconServerClientData* client){
    BeaconProcessQueueNode* node = client->requester.processNode;
    BeaconFileLoadRequest* loadRequest = client->requester.loadRequest;
    
    SAFE_FREE(node->beaconFile.data);
    
    node->beaconFile.data                    = loadRequest->output.beaconFile.data;
    node->beaconFile.byteCount                = loadRequest->output.beaconFile.byteCount;
    node->beaconFile.uncompressedByteCount    = loadRequest->output.beaconFile.uncompressedByteCount;
    node->beaconFile.sentByteCount = 0;
    
    ZeroStruct(&loadRequest->output.beaconFile);
    
    beaconServerSendBeaconFileToRequester(client, 1);
}

U32 beaconServerGetLoadRequestCount(){
    U32 loadRequestCount;
    
    EnterLoadRequestCS();
        loadRequestCount = load_requests.count;
    LeaveLoadRequestCS();
    
    return loadRequestCount;
}

S32 beaconServerIsLoadRequestFinished(BeaconServerClientData* client){
    return    client->requester.loadRequest &&
            client->requester.loadRequest->output.complete;
}

S32 beaconServerSendSuccessfulLoadRequest(BeaconServerClientData* client){
    if(!client->requester.loadRequest->output.success){
        return 0;
    }
    
    // Send the beacon file that was loaded.
    
    beaconServerSendLoadedBeaconFileToRequester(client);
    
    return 1;
}

static BeaconFileLoadRequest* beaconServerGetNextLoadRequest(){
    BeaconFileLoadRequest* request = load_requests.head;

    // Take the head request out of the list.
    
    load_requests.head = request->node.next;

    if(load_requests.head){
        assert(load_requests.head->node.prev == request);
        
        load_requests.head->node.prev = NULL;
    }else{
        assert(!request->node.prev);
        
        load_requests.tail = NULL;
    }
    
    // Decrement the load request count.
    
    assert(load_requests.count > 0);
    
    load_requests.count--;
    
    // Make sure the request is still referenced by the client.
    // If it wasn't referenced then it wouldn't have been in the list anymore.

    assert(request->refCount);
    assert(request->client);
    assert(request->client->requester.loadRequest == request);

    request->refCount++;
    ZeroStruct(&request->node);
    
    return request;
}

static unsigned __stdcall beaconServerBeaconFileLoadThread(void* unused){
    EXCEPTION_HANDLER_BEGIN
    
    EnterLoadRequestCS();

    while(1){
        WaitLoadRequestConditionCS();

        if (load_requests.head) {
            char fileName[MAX_PATH];
            FILE* f;

            BeaconFileLoadRequest* request = beaconServerGetNextLoadRequest();

            LeaveLoadRequestCS();

            beaconServerGetRequestFileName(fileName, NULL, request->input.fileName, ".beaconrequest");

            f = fopen(fileName, "rb");

            request->output.success = 0;

            #define FREAD(x) fread(&x, sizeof(x), 1, f)
            
            if(!f){
                beaconServerFileLoadRequestComplete(request);
            }else{
                if(    !FREAD(request->output.fileVersion) ||
                    request->output.fileVersion > BEACON_CACHE_FILE_VERSION)
                {
                    beaconServerFileLoadRequestComplete(request);
                }
                else{
                    U32 crc;
                    
                    if(    FREAD(crc) &&
                        crc == request->input.crc &&
                        FREAD(request->output.beaconFile.byteCount) &&
                        request->output.beaconFile.byteCount <= 100 * SQR(1024) &&
                        FREAD(request->output.beaconFile.uncompressedByteCount))
                    {
                        request->output.beaconFile.data = malloc(request->output.beaconFile.byteCount);
                        
                        if(fread(request->output.beaconFile.data, request->output.beaconFile.byteCount, 1, f)){
                            request->output.success = 1;
                        }else{
                            SAFE_FREE(request->output.beaconFile.data);
                        }
                    }

                    beaconServerFileLoadRequestComplete(request);
                }
                
                fclose(f);
            }

            EnterLoadRequestCS();
        }
    }

    LeaveLoadRequestCS();
    
    EXCEPTION_HANDLER_END
}

void beaconServerInitLoadRequests(){
    static S32 init;
    
    if(!init){
        init = 1;

        beaconPrintf(COLOR_GREEN, "Starting beacon file loader thread: ");
        InitializeCriticalSectionAndSpinCount(&load_requests.cs, 4000);
        InitializeConditionVariable(&load_requests.cv);
        
        _beginthreadex(NULL, 0, beaconServerBeaconFileLoadThread, NULL, 0, NULL);
        beaconPrintf(COLOR_GREEN, "Done!\n");
    }
}

// Process Queue ----------------------------------------------------------------------------------

MP_DEFINE(BeaconProcessQueueNode);

static void EnterQueueCS(){
    EnterCriticalSection(&beacon_server.processQueue.cs);
}

static void LeaveQueueCS(){
    LeaveCriticalSection(&beacon_server.processQueue.cs);
}

static void WakeQueueConditionCS(){
    WakeConditionVariable(&beacon_server.processQueue.cv);
}

static void WaitQueueConditionCS(){
    SleepConditionVariableCS(&beacon_server.processQueue.cv, &beacon_server.processQueue.cs, INFINITE);
}

static BeaconProcessQueueNode* createBeaconProcessQueueNode(){
    static U32 uid;
    BeaconProcessQueueNode* node;
    
    EnterQueueCS();
    
        MP_CREATE(BeaconProcessQueueNode, 100);
        
        node = MP_ALLOC(BeaconProcessQueueNode);
        
        node->uid = ++uid;
        
    LeaveQueueCS();
    
    beaconMapDataPacketCreate(&node->mapData);
    
    return node;
}

static void beaconServerGetPendingFileName(BeaconProcessQueueNode* node, char* fileName){
    beaconServerGetRequestFileName(fileName, "pending", node->uniqueFileName, ".pending");
}

static void destroyBeaconProcessQueueNode(BeaconProcessQueueNode* node){
    char fileName[MAX_PATH];

    EnterQueueCS();
    
        assert(!node->requester);
        assert(!node->requestServer);
        assert(!node->sibling.next && !node->sibling.prev);

        // Delete the pending request file.
        
        beaconServerGetPendingFileName(node, fileName);
        
        DeleteFileA(fileName);
        
        // Free all my member variables.
        
        beaconMapDataPacketDestroy(&node->mapData);
        
        SAFE_FREE(node->beaconFile.data);
        ZeroStruct(&node->beaconFile);

        estrDestroy(&node->uniqueFileName);
        estrDestroy(&node->uniqueStorageName);
        
        MP_FREE(BeaconProcessQueueNode, node);
        
    LeaveQueueCS();
}

static void beaconServerAddProcessNodeTail(BeaconProcessQueueNode* node){
    EnterQueueCS();
    
        assert(!node->sibling.prev && !node->sibling.next);
        
        if(beacon_server.processQueue.tail){
            beacon_server.processQueue.tail->sibling.next = node;
            node->sibling.prev = beacon_server.processQueue.tail;
        }else{
            beacon_server.processQueue.head = node;
        }
        
        beacon_server.processQueue.tail = node;
        
        beacon_server.processQueue.count++;
        
    LeaveQueueCS();

    WakeQueueConditionCS();
}

static void beaconServerRemoveProcessNodeFromList(BeaconProcessQueueNode* node){
    S32 decrement = 1;
    
    EnterQueueCS();

        if(node->sibling.prev){
            node->sibling.prev->sibling.next = node->sibling.next;
        }
        else if(node == beacon_server.processQueue.head){
            beacon_server.processQueue.head = node->sibling.next;
        }
        else{
            decrement = 0;
            assert(!node->sibling.next);
            assert(node != beacon_server.processQueue.tail);
        }
        
        if(node->sibling.next){
            node->sibling.next->sibling.prev = node->sibling.prev;
        }
        else if(node == beacon_server.processQueue.tail){
            beacon_server.processQueue.tail = node->sibling.prev;
        }
        
        ZeroStruct(&node->sibling);
        
        if(decrement){
            beacon_server.processQueue.count--;
        }

    LeaveQueueCS();
}

void beaconServerDetachRequesterFromProcessNode(BeaconServerClientData* requester){
    BeaconProcessQueueNode* node = requester ? requester->requester.processNode : NULL;
    
    if(!node){
        return;
    }

    EnterQueueCS();
    
        assert(node->requester == requester);
    
        node->requester = NULL;
        requester->requester.processNode = NULL;
    
        switch(node->state){
            xcase    BPNS_WAITING_FOR_LOAD_REQUEST:
            case    BPNS_WAITING_FOR_CLIENT_TO_RECEIVE_BEACON_FILE:
            case    BPNS_WAITING_FOR_MAP_DATA_FROM_CLIENT:
            case    BPNS_WAITING_TO_REQUEST_MAP_DATA_FROM_CLIENT:
            {
                beaconServerCancelProcessNode(node);
            }
        }
        
    LeaveQueueCS();
}

static void beaconServerMakeUniqueFileName(    char** outStr,
                                            const char* uniqueStorageName,
                                            const char* dbServerIP)
{
    char buffer[100];
    int i;
    
    estrPrintCharString(outStr, uniqueStorageName);

    for(i = 0; (*outStr)[i]; i++){
        U8 c = (*outStr)[i];
        
        if(    !(tolower(c) >= 'a' && tolower(c) <= 'z') &&
            !(c >= '0' && c <= '9') &&
            !strchr(".,-_=+", c))
        {
            sprintf(buffer, "%2.2x", c);
            (*outStr)[i] = '%';
            estrInsert(outStr, i + 1, buffer, 2);
            i += 2;
        }
    }
    
    if(!beaconIsProductionMode() && dbServerIP){
        sprintf(buffer, "%s_", dbServerIP);
        estrInsert(outStr, 0, buffer, (int)strlen(buffer));
    }
}

static BeaconProcessQueueNode* beaconServerGetProcessNodeByName(const char* name){
    BeaconProcessQueueNode* node;
    
    if(!beacon_server.processQueue.htFileNameToRequestNode){
        return NULL;
    }
    
    EnterQueueCS();
    
        stashFindPointer( beacon_server.processQueue.htFileNameToRequestNode, name, &node );
        
    LeaveQueueCS();
    
    return node;
}

static S32 beaconServerAddProcessNodeToHashTable(BeaconProcessQueueNode* node){
    if(!beacon_server.processQueue.htFileNameToRequestNode){
        beacon_server.processQueue.htFileNameToRequestNode = stashTableCreateWithStringKeys(500, StashDeepCopyKeys);
    }
    
    if(beaconServerGetProcessNodeByName(node->uniqueFileName)){
        return 0;
    }
    
    EnterQueueCS();
    
        stashAddPointer(beacon_server.processQueue.htFileNameToRequestNode, node->uniqueFileName, node, false);
        
        node->isInHashTable = 1;

    LeaveQueueCS();

    return 1;
}

void beaconServerAddRequesterToProcessQueue(BeaconServerClientData* requester,
                                            const char* uniqueStorageName,
                                            U32 crc)
{
    char*                    uniqueFileName = NULL;
    BeaconProcessQueueNode* node;
    
    // Create the unique filename.
    
    beaconServerMakeUniqueFileName(    &uniqueFileName,
                                    uniqueStorageName,
                                    requester->requester.dbServerIP);

    // Detach the old process node.
    
    beaconServerDetachRequesterFromProcessNode(requester);
    
    // Find an existing process node.
    
    node = beaconServerGetProcessNodeByName(uniqueFileName);
    
    if(node && beaconMapDataPacketGetCRC(node->mapData) != crc){
        // Need to destroy the old process node.
        
        beaconServerCancelProcessNode(node);
        
        node = NULL;
    }
    
    if(!node){
        // Create a new process node.
        
        node = createBeaconProcessQueueNode();
        
        node->state = BPNS_WAITING_FOR_LOAD_REQUEST;
        
        node->requester = requester;
        
        estrPrintCharString(&node->uniqueStorageName, uniqueStorageName);
        
        beaconServerMakeUniqueFileName(    &node->uniqueFileName,
                                        uniqueStorageName,
                                        requester->requester.dbServerIP);

        beaconServerAddProcessNodeToHashTable(node);
        
        requester->requester.processNode = node;
    
        beaconServerAddProcessNodeTail(node);
    }else{
        beaconServerDisconnectClient(    node->requester,
                                        "Node had a requester on it already.  This is bad.");
        
        beaconServerDetachRequesterFromProcessNode(node->requester);
        
        EnterQueueCS();
        
            node->requester = requester;
            requester->requester.processNode = node;
        
        LeaveQueueCS();
    }
        
}

void beaconServerCancelProcessNode(BeaconProcessQueueNode* node){
    if(!node->isInHashTable){
        return;
    }
    
    EnterQueueCS();
    
        stashRemovePointer(    beacon_server.processQueue.htFileNameToRequestNode, node->uniqueFileName, NULL);
                            
        node->isInHashTable = 0;
        
        beaconServerDetachRequesterFromProcessNode(node->requester);
        
        beaconServerAssignProcessNodeToRequestServer(node, NULL);
        
    LeaveQueueCS();
}

S32 beaconServerGetProcessNodeForProcessing(BeaconProcessQueueNode** nodeOut){
    BeaconProcessQueueNode* node;
    
    EnterQueueCS();
    
        for(node = beacon_server.processQueue.head; node; node = node->sibling.next){
            if(    node->state == BPNS_WAITING_FOR_REQUEST_SERVER &&
                !node->requestServer &&
                node->isInHashTable)
            {
                break;
            }
        }
    
    LeaveQueueCS();
    
    if(nodeOut){
        *nodeOut = node;
    }
    
    return node ? 1 : 0;
}

void beaconServerUpdateProcessNodes(){
    BeaconProcessQueueNode* node;
    U32 totalReceivers = 0;
    
    EnterQueueCS();
    
    for(node = beacon_server.processQueue.head; node; node = node->sibling.next){
        if(!node->isInHashTable){
            continue;
        }
        
        switch(node->state){
            xcase BPNS_WAITING_FOR_LOAD_REQUEST:{
                // The client is waiting to see if the cached beacon file is valid.
                if(!node->requester){
                    beaconServerCancelProcessNode(node);
                }
                else if(beaconServerIsLoadRequestFinished(node->requester)){
                    if(beaconServerSendSuccessfulLoadRequest(node->requester)){
                        // Set this state, and then just wait for the process node to die.
                        
                        node->state = BPNS_WAITING_FOR_CLIENT_TO_RECEIVE_BEACON_FILE;
                    }else{
                        // The beacon file doesn't match the CRC, so rebuild it.
                        
                        node->state = BPNS_WAITING_TO_REQUEST_MAP_DATA_FROM_CLIENT;
                    }
                }
            }
            
            xcase BPNS_WAITING_TO_REQUEST_MAP_DATA_FROM_CLIENT:{
                // The client is waiting to send data, so see if it's allowed to.
                
                if(!node->requester){
                    beaconServerCancelProcessNode(node);
                }
                else if(beaconMapDataPacketIsFullyReceived(node->mapData)){
                    // Done receiving the packet.
                    
                    node->state = BPNS_WAITING_FOR_WRITE_TO_DISK;
                }
                else if(totalReceivers < 5){
                    // Still allowed to send.
                    
                    beaconServerSendRequestChunkReceived(node);
                    
                    totalReceivers++;
                }
            }
                
            xcase BPNS_WAITING_FOR_MAP_DATA_FROM_CLIENT:{
                // See if we're still waiting for this client to send the map data.
                
                if(!node->requester){
                    beaconServerCancelProcessNode(node);
                }
                else if(beaconTimeSince(node->requester->requester.toldToSendRequestTime) < 3){
                    totalReceivers++;
                }
            }
        }            
    }
        
    LeaveQueueCS();
}

static void beaconServerProcessNodeWriteFile(BeaconProcessQueueNode* node){
    char fileName[MAX_PATH];
    
    beaconServerGetPendingFileName(node, fileName);
    
    beaconServerMakeDirectories(fileName);

    beaconMapDataPacketWriteFile(node->mapData, fileName, node->uniqueStorageName, node->timeStamp);
}

static void beaconServerReadProcessNodeFile(BeaconProcessQueueNode* node){
    char fileName[MAX_PATH];
    
    beaconServerGetPendingFileName(node, fileName);
    
    beaconMapDataPacketReadFile(node->mapData, fileName, &node->uniqueStorageName, &node->timeStamp, 0);
}

static unsigned __stdcall beaconServerProcessQueueThread(void* unused){
    EXCEPTION_HANDLER_BEGIN
    
    EnterQueueCS();
    while(1){
        BeaconProcessQueueNode* node;
        BeaconProcessQueueNode* nextNode;
        U32 totalByteCount = 0;
        U32 writtenCount = 0;
        
        WaitQueueConditionCS();

        for(node = beacon_server.processQueue.head; node; node = nextNode){
            S32 byteCountExceeded = totalByteCount >= 50 * SQR(1024);
            
            nextNode = node->sibling.next;
            
            if(!node->isInHashTable){
                beaconServerRemoveProcessNodeFromList(node);
                
                destroyBeaconProcessQueueNode(node);
                
                continue;
            }
            
            if(    node->state == BPNS_WAITING_FOR_WRITE_TO_DISK &&
                writtenCount < 3)
            {
                assert(beaconMapDataPacketIsFullyReceived(node->mapData));
                
                LeaveQueueCS();
                
                beaconServerProcessNodeWriteFile(node);
                
                EnterQueueCS();
                
                writtenCount++;
                
                if(byteCountExceeded){
                    node->state = BPNS_WAITING_FOR_MAP_DATA_FROM_DISK;
                
                    beaconMapDataPacketDiscardData(node->mapData);
                }else{
                    node->state = BPNS_WAITING_FOR_REQUEST_SERVER;
                }
            }
            
            if( node->state == BPNS_WAITING_FOR_MAP_DATA_FROM_DISK &&
                !byteCountExceeded)
            {
                assert(!beaconMapDataPacketIsFullyReceived(node->mapData));
                
                LeaveQueueCS();
                
                beaconServerReadProcessNodeFile(node);
                
                EnterQueueCS();
                
                node->state = BPNS_WAITING_FOR_REQUEST_SERVER;
            }                
            
            totalByteCount += beaconMapDataPacketGetSize(node->mapData);
        }
    }

    LeaveQueueCS();
    
    EXCEPTION_HANDLER_END
}
                                                
struct {
    BeaconProcessQueueNode**    nodes;
    S32                            count;
    S32                            maxCount;
} pending;

static FileScanAction queuePendingProcess(char* dir, struct _finddata32_t* data){
    if(!(data->attrib & _A_SUBDIR) && strEndsWith(data->name, ".pending")){
        BeaconProcessQueueNode* node = createBeaconProcessQueueNode();
        char fileName[MAX_PATH];
        
        strcpy(fileName, getFileName(data->name));
        fileName[strlen(fileName) - strlen(".pending")] = 0;
        
        estrPrintCharString(&node->uniqueFileName, fileName);

        beaconServerGetPendingFileName(node, fileName);
        
        beaconMapDataPacketCreate(&node->mapData);

        if(!beaconMapDataPacketReadFile(node->mapData, fileName, &node->uniqueStorageName, &node->timeStamp, 1)){
            destroyBeaconProcessQueueNode(node);

            return FSA_NO_EXPLORE_DIRECTORY;
        }
        
        node->state = BPNS_WAITING_FOR_MAP_DATA_FROM_DISK;
        
        beaconMapDataPacketDiscardData(node->mapData);
        
        dynArrayAddp(&pending.nodes, &pending.count, &pending.maxCount, node);
    }
    
    return FSA_NO_EXPLORE_DIRECTORY;
}

static S32 __cdecl compareNodeTimestamps(const void* n1Data, const void* n2Data)
{
    const BeaconProcessQueueNode** n1 = (const BeaconProcessQueueNode**)n1Data;
    const BeaconProcessQueueNode** n2 = (const BeaconProcessQueueNode**)n2Data;
    U32 t1 = (*n1)->timeStamp;
    U32 t2 = (*n2)->timeStamp;
    
    if(t1 < t2){
        return -1;
    }
    
    if(t1 == t2){
        return 0;
    }

    return 1;
}

static void beaconServerLoadPendingRequests(){
    char pendingDir[1000];
    S32 i;
    
    beaconPrintf(COLOR_GREEN, "Finding pending requests: \n");
    
    SAFE_FREE(pending.nodes);
    ZeroStruct(&pending);
    
    sprintf(pendingDir, "%s/pending", beacon_server.requestCacheDir);
    
    fileScanAllDataDirs(pendingDir, queuePendingProcess);
    
    qsort(pending.nodes, pending.count, sizeof(pending.nodes[0]), compareNodeTimestamps);
    
    for(i = 0; i < pending.count; i++){
        BeaconProcessQueueNode* node = pending.nodes[i];
        
        printf("   Found pending map: ");
        beaconPrintf(COLOR_GREEN, "%s\n", node->uniqueFileName);
        
        beaconServerAddProcessNodeToHashTable(node);

        beaconServerAddProcessNodeTail(node);
    }
    
    beaconPrintf(COLOR_GREEN, "Done!  %d pending requests found.\n", pending.count);
    
    SAFE_FREE(pending.nodes);
    ZeroStruct(&pending);
}

void beaconServerInitProcessQueue(){
    static S32 init;
    
    if(!init){
        init = 1;
        
        InitializeCriticalSectionAndSpinCount(&beacon_server.processQueue.cs, 4000);
        InitializeConditionVariable(&beacon_server.processQueue.cv);
        
        beaconServerLoadPendingRequests();
        
        beaconPrintf(COLOR_GREEN, "Starting beacon process queue thread: ");
        _beginthreadex(NULL, 0, beaconServerProcessQueueThread, NULL, 0, NULL);
        beaconPrintf(COLOR_GREEN, "Done!\n");
    }
}
