
#include "beaconClientServerPrivate.h"
#include "beaconServerPrivate.h"
#include "bases/baseserver.h"
#include "comm_backend.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/winutil.h>
#include "storyarc/missionMapCommon.h"
#include <utilitieslib/utils/log.h>
#include "seq/seqstate.h"

#define BEACON_SERVER_PROTOCOL_VERSION    (2)
#define BEACON_MASTER_SERVER_ICON_COLOR    (0xffff00)
#define BEACON_SERVER_ICON_COLOR        (0xff8000)

BeaconServer beacon_server;

static const char* beaconServerExeName = "BeaconServer.exe";

void beaconServerSetDataToolsRootPath(const char* dataToolsRootPath){
    //bool addAppropriateDataDirs(const char *path);
    //
    //addAppropriateDataDirs(dataToolsRootPath);
                    
    estrPrintCharString(&beacon_server.dataToolsRootPath, dataToolsRootPath);

    forwardSlashes(beacon_server.dataToolsRootPath);

    // Remove trailing slashes.

    while(    beacon_server.dataToolsRootPath[0] &&
            strEndsWith(beacon_server.dataToolsRootPath, "/"))
    {
        estrRemove(    &beacon_server.dataToolsRootPath,
                    estrLength(&beacon_server.dataToolsRootPath) - 1,
                    1);
    }
}

const char* beaconServerGetDataToolsRootPath(void){
    return beacon_server.dataToolsRootPath ? beacon_server.dataToolsRootPath : ".";
}

const char* beaconServerGetDataPath(void){
    if(!beacon_server.dataPath){
        estrPrintf(&beacon_server.dataPath, "%s/data", beaconServerGetDataToolsRootPath());
    }

    return beacon_server.dataPath;
}

const char* beaconServerGetToolsPath(void){
    if(!beacon_server.dataPath){
        estrPrintf(&beacon_server.dataPath, "%s/tools", beaconServerGetDataToolsRootPath());
    }

    return beacon_server.dataPath;
}

static void verifyAvailableCount(void){
    BeaconConnectBeaconGroup* cur = beacon_server.beaconConnect.groups.availableHead;
    S32 i;

    for(i = 0; cur; cur = cur->next, i++);

    assert(i == beacon_server.beaconConnect.groups.count - beacon_server.beaconConnect.assignedCount - beacon_server.beaconConnect.finishedCount);
}

void beaconServerSetClientState(BeaconServerClientData* client, BeaconClientState state){
    if(client->state != state){
        client->state = state;
        client->stateBeginTime = beaconGetCurTime();
    }
}

static void beaconServerSetClientType(BeaconServerClientData* client, BeaconClientType clientType){
    if(client->clientType != clientType){
        BeaconServerClientList* clientList;
        S32 i;

        if(client->clientType > BCT_NOT_SET && client->clientType < BCT_COUNT){
            clientList = beacon_server.clientList + client->clientType;

            for(i = 0; i < clientList->count; i++){
                if(clientList->clients[i] == client){
                    clientList->count--;
                    CopyStructsFromOffset(clientList->clients + i, 1, clientList->count - i);
                    break;
                }
            }

            assert(i <= clientList->count);
        }

        client->clientType = clientType;

        if(clientType > BCT_NOT_SET && clientType < BCT_COUNT){
            clientList = beacon_server.clientList + clientType;

            dynArrayAddp(&clientList->clients, &clientList->count, &clientList->maxCount, client);
        }
    }
}

static char* getClientIPStr(BeaconServerClientData* client){
    return beaconGetLinkIPStr(client->link);
}

static void beaconClientVprintf(BeaconServerClientData* client, S32 color, const char* format, va_list argptr){
    consoleSetColor(COLOR_RED, 0);
    printf("%s ", beaconCurTimeString(0));
    switch(client->clientType){
        xcase BCT_SENTRY:
            consoleSetColor(COLOR_BRIGHT|COLOR_YELLOW, 0);
        xcase BCT_REQUESTER:
            consoleSetColor(COLOR_BRIGHT|COLOR_GREEN, 0);
        xdefault:
            consoleSetColor(COLOR_BRIGHT|COLOR_GREEN|COLOR_BLUE, 0);
    }
    printf("%-15s", getClientIPStr(client));
    consoleSetDefaultColor();
    printf(" : ");
    if(color){
        consoleSetColor(color, 0);
    }
    vprintf(format, argptr);
    consoleSetDefaultColor();
}

static void beaconClientPrintf(BeaconServerClientData* client, S32 color, const char* format, ...){
    va_list argptr;

    if(    beaconIsProductionMode() &&
        strnicmp(format, "WARNING:", 8) &&
        strnicmp(format, "ERROR:", 6))
    {
        return;
    }

    va_start(argptr, format);
    beaconClientVprintf(client, COLOR_BRIGHT|color, format, argptr);
    va_end(argptr);
}

static void beaconClientPrintfDim(BeaconServerClientData* client, S32 color, const char* format, ...){
    va_list argptr;

    va_start(argptr, format);
    beaconClientVprintf(client, color, format, argptr);
    va_end(argptr);
}

static HANDLE    hConsoleBack;
static HANDLE    hConsoleMain;
static S32        backConsoleRefCount;

static void beaconEnableBackgroundConsole(void){
    if(!hConsoleMain){
        CONSOLE_CURSOR_INFO info = {0};
        COORD backSize = { 200, 9999 };

        hConsoleMain = GetStdHandle(STD_OUTPUT_HANDLE);

        hConsoleBack = CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

        info.dwSize = 1;
        info.bVisible = 0;
        SetConsoleCursorInfo(hConsoleBack, &info);

        SetConsoleScreenBufferSize(hConsoleBack, backSize);
    }

    if(!backConsoleRefCount++){
        SetConsoleActiveScreenBuffer(hConsoleBack);

        SetStdHandle(STD_OUTPUT_HANDLE, hConsoleBack);
    }
}

static void beaconDisableBackgroundConsole(void){
    assert(backConsoleRefCount > 0);

    if(!--backConsoleRefCount){
        SetConsoleActiveScreenBuffer(hConsoleMain);

        SetStdHandle(STD_OUTPUT_HANDLE, hConsoleMain);
    }
}

static void beaconConsoleVprintf(U32 color, const char* format, va_list argptr){
    char buffer[1000];
    S32 length;
    S32 outLength;

    length = _vsnprintf(buffer, 1000, format, argptr);
    beaconEnableBackgroundConsole();
    if(color){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), buffer, length, &outLength, NULL);
    if(color){
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_WHITE);
    }
    beaconDisableBackgroundConsole();
}

static void beaconConsolePrintf(U32 color, const char* format, ...){
    va_list argptr;

    va_start(argptr, format);
    beaconConsoleVprintf(color | COLOR_BRIGHT, format, argptr);
    va_end(argptr);
}

static void beaconConsolePrintfDim(U32 color, const char* format, ...){
    va_list argptr;

    va_start(argptr, format);
    beaconConsoleVprintf(color & ~COLOR_BRIGHT, format, argptr);
    va_end(argptr);
}

static void beaconServerSetSendStatus(void){
    if(!beacon_server.status.send){
        beacon_server.status.send = 1;
        beacon_server.status.acked = 0;
        beacon_server.status.sendUID++;
    }
}

static void removeClientFromBlock(BeaconServerClientData* client){
    if(client->assigned.block){
        BeaconDiskSwapBlock* block = client->assigned.block;
        S32 i;

        for(i = 0; i < block->clients.count; i++){
            if(block->clients.clients[i] == client){
                block->clients.count--;
                block->clients.clients[i] = block->clients.clients[block->clients.count];
                block->clients.clients[block->clients.count] = NULL;
                client->assigned.block = NULL;
                break;
            }
        }

        assert(i <= block->clients.count);
    }
}

static void removeClientFromGroup(BeaconServerClientData* client){
    if(client->assigned.group){
        BeaconConnectBeaconGroup* group = client->assigned.group;
        S32 i;

        for(i = 0; i < group->clients.count; i++){
            if(group->clients.clients[i] == client){
                group->clients.count--;
                group->clients.clients[i] = group->clients.clients[group->clients.count];
                group->clients.clients[group->clients.count] = NULL;
                client->assigned.group = NULL;

                assert(beacon_server.beaconConnect.assignedCount > 0);

                if(!group->clients.count){
                    group->next = beacon_server.beaconConnect.groups.availableHead;
                    beacon_server.beaconConnect.groups.availableHead = group;
                    if(!beacon_server.beaconConnect.groups.availableTail){
                        assert(!group->next);
                        beacon_server.beaconConnect.groups.availableTail = group;
                    }
                    beacon_server.beaconConnect.assignedCount--;
                }

                verifyAvailableCount();

                break;
            }
        }

        assert(i <= group->clients.count);
    }
}

static S32 sentryHasClient(BeaconServerClientData* sentry, BeaconServerClientData* client, U32 pid, U32 uid, S32* indexOut){
    S32 i;

    if(    !sentry
        ||
        !client &&
        !pid &&
        !uid )
    {
        return 0;
    }

    for(i = 0; i < sentry->sentryClients.count; i++){
        BeaconServerSentryClientData* sentryClient = sentry->sentryClients.clients + i;

        if(    (    !pid ||
                sentryClient->pid == pid)
            &&
            (    !client ||
                sentryClient->client == client)
            &&
            (    !uid ||
                sentryClient->uid == uid))
        {
            if(indexOut){
                *indexOut = i;
            }
            return 1;
        }
    }

    return 0;
}

static void printClients(char* prefix, S32 line, BeaconServerClientData* sentry){
    S32 i;
    return;
    for(i = 0; i < sentry->sentryClients.count; i++){
        BeaconServerSentryClientData* client = sentry->sentryClients.clients + i;

        beaconPrintf(    COLOR_YELLOW,
                        "%s:%d: client %d: 0x%p, pid %d uid %d\n", prefix, line, i, client->client, client->pid, client->uid);

        consoleSetColor(COLOR_BLUE, 0);
        printf("%s\n", client->crashText);
        consoleSetDefaultColor();
    }
}

#define FOR_CLIENTS_LINK(i, clientVar, linkVar)                            \
    {                                                                    \
    S32 i;                                                                \
    for(i = 0; i < beacon_server.clients.links->size; i++){                \
        NetLink* linkVar = beacon_server.clients.links->storage[i];        \
        BeaconServerClientData* clientVar = linkVar->userData;

#define FOR_CLIENTS(i, clientVar)                                        \
    FOR_CLIENTS_LINK(i, clientVar, linkVar__)

#define END_FOR }}

static void checkForSingleSentryEntry(BeaconServerClientData* client){
    S32 sentryFound = 0;

    FOR_CLIENTS(i, sentry)
        S32 j;

        for(j = 0; j < sentry->sentryClients.count; j++){
            BeaconServerSentryClientData* sentryClient = sentry->sentryClients.clients + j;

            if(sentryClient->client == client){
                assert(!sentryFound);
                sentryFound = 1;
            }
        }
    END_FOR
}

static void verifySentryIntegrity(void){
    // MS: This is really slow and it has never found a problem, so I'm commenting it out.

    return;

    FOR_CLIENTS(i, client)
        S32 j;

        switch(client->clientType){
            xcase BCT_SENTRY:{
                assert(!client->sentry);

                for(j = 0; j < client->sentryClients.count; j++){
                    BeaconServerSentryClientData* sentryClient = client->sentryClients.clients + j;

                    assert(!sentryClient->client || sentryClient->client->sentry == client);

                    if(sentryClient->client){
                        checkForSingleSentryEntry(sentryClient->client);
                    }
                }
            }

            xdefault:{
                checkForSingleSentryEntry(client);
            }
        }
    END_FOR
}

static void removeClientFromSentry(    BeaconServerClientData* client,
                                    U32 pid,
                                    U32 uid,
                                    BeaconServerClientData* sentry)
{
    S32 i;

    if(client){
        if(!sentry){
            sentry = client->sentry;
        }else{
            assert(!client->sentry || sentry == client->sentry);
        }
    }

    if(!sentry){
        return;
    }

    //printf("setting client 0x%8.8x to sentry 0x%8.8x\n", client, 0);

    printClients(__FUNCTION__, __LINE__, sentry);

    verifySentryIntegrity();

    if(sentryHasClient(sentry, client, pid, uid, &i)){
        verifySentryIntegrity();

        consoleSetColor(COLOR_BRIGHT|COLOR_RED, 0);
        //printf("Removing from sentry 0x%8.8x:    client %d/%d, 0x%8.8x, pid %d\n%s\n", sentry, i, sentry->sentryClients.count, client, pid, sentry->sentryClients.clients[i].crashText);
        consoleSetDefaultColor();

        SAFE_FREE(sentry->sentryClients.clients[i].crashText);

        //printf("erasing client %4d:0x%8.8x from sentry 0x%8.8x\n", sentry->sentryClients.clients[i].pid, sentry->sentryClients.clients[i].client, sentry);

        if(!client){
            client = sentry->sentryClients.clients[i].client;
        }

        if(client){
            assert(!client || !client->sentry || client->sentry == sentry);
            //printf("set!!!\n");
            client->sentry = NULL;
        }

        if(0){
            S32 j;
            for(j = 0; j < sentry->sentryClients.count; j++){
                printf("  remaining client: 0x%p\n", sentry->sentryClients.clients[j].client);
            }
        }

        // Move the last sentryClient in the list to this position and shrink list by 1
        sentry->sentryClients.clients[i] = sentry->sentryClients.clients[--sentry->sentryClients.count];

        verifySentryIntegrity();

        if(0){
            S32 j;
            for(j = 0; j < sentry->sentryClients.count; j++){
                printf("  remaining client: 0x%p\n", sentry->sentryClients.clients[j].client);
            }
        }
    }

    verifySentryIntegrity();

    printClients(__FUNCTION__, __LINE__, sentry);
}

static BeaconServerSentryClientData* setClientSentry(BeaconServerClientData* client, U32 pid, U32 uid, BeaconServerClientData* sentry){
    BeaconServerSentryClientData* sentryClient;
    S32 i;

    printClients(__FUNCTION__, __LINE__, sentry);
    removeClientFromSentry(client, pid, uid, NULL);
    printClients(__FUNCTION__, __LINE__, sentry);

    assert(!client || !client->sentry);

    if(!sentry){
        return NULL;
    }

    if(client){
        //printf("setting client %4d:0x%8.8x to sentry 0x%8.8x\n", pid, client, sentry);
        assert(client->uid == uid);

        assert(client != sentry && !sentry->sentry);

        client->sentry = sentry;
    }

    verifySentryIntegrity();

    if(!sentryHasClient(sentry, client, pid, uid, &i)){
        printClients(__FUNCTION__, __LINE__, sentry);

        dynArrayAddStruct(    &sentry->sentryClients.clients,
                            &sentry->sentryClients.count,
                            &sentry->sentryClients.maxCount);

        printClients(__FUNCTION__, __LINE__, sentry);

        sentryClient = sentry->sentryClients.clients + sentry->sentryClients.count - 1;

        ZeroStruct(sentryClient);
    }else{
        sentryClient = sentry->sentryClients.clients + i;
    }

    // This is complaining beacause we have been told to add a client who is not currently connected (if client is NULL, we couldn't find the link by UID,
    //   so it means we only have a pid reference to the client on the sentry and no actual net link)
    // but according to our sentry master list, that client is actually net link connected to us (since there is still a net link in our record of the sentry
    // My best guess for why this is happening is that this set client sentry is being called prior to the netdisconnect callback of the client link which would clean out
    //   the net link from the client sentry
    // The fix appears to be to make sure we remove the client from the Master Server's list of clients of the sentry when we do the transfer,
    //   and not wait for the disconnect
    assert(client || !sentryClient->client);

    sentryClient->client = client;
    sentryClient->pid = pid;
    sentryClient->uid = uid;

    verifySentryIntegrity();

    return sentryClient;
}

static const char* getClientTypeName(BeaconServerClientData* client){
    switch(client->clientType){
        xcase BCT_WORKER:
            return "WORKER";
        xcase BCT_SENTRY:
            return "SENTRY";
        xcase BCT_SERVER:
            return "SERVER";
        xcase BCT_REQUESTER:
            return "REQUESTER";
        xdefault:
            return "NO_CLIENT_TYPE";
    }
}

static Packet* createServerToClientPacket(BeaconServerClientData* client, const char* textCmd){
    Packet* pak;

    assert(client->link);
    pak = pktCreateEx(client->link, BMSG_S2C_TEXT_CMD);
    pktSendString(pak, textCmd);

    return pak;
}

static void sendServerToClientPacket(BeaconServerClientData* client, Packet** pak){
    pktSend(pak, client->link);
}

// Macros to create server-to-client packets.

#define BEACON_SERVER_PACKET_CREATE_BASE(pak, client, textCmd){                    \
    Packet* pak;                                                                \
    Packet* serverPacket__ = pak = createServerToClientPacket(client, textCmd);    \
    BeaconServerClientData* serverPacketClient__ = client
#define BEACON_SERVER_PACKET_CREATE(textCmd)                                    \
    BEACON_SERVER_PACKET_CREATE_BASE(pak, client, textCmd)
#define BEACON_SERVER_PACKET_SEND()                                                \
    sendServerToClientPacket(serverPacketClient__, &serverPacket__);}

static void beaconServerSendLoadMapReply(BeaconServerClientData* client, S32 good){
    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_MAP_DATA_LOADED_REPLY);
        pktSendBits(pak, 1, good ? 1 : 0);
    BEACON_SERVER_PACKET_SEND();
}

static void beaconServerSendMapDataToRequestServer(BeaconServerClientData* client){
    BeaconProcessQueueNode* node = client->requestServer.processNode;

    if(    !client->server.isRequestServer ||
        !node ||
        !beaconMapDataPacketIsFullyReceived(node->mapData) ||
        beaconMapDataPacketIsFullySent(    node->mapData,
                                        client->requestServer.sentByteCount))
    {
        return;
    }

    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_MAP_DATA);

        pktSendBitsPack(pak, 1, node->uid);
        pktSendString(pak, node->uniqueStorageName);

        pktSendBits(pak, 1, client->requestServer.sentByteCount ? 0 : 1);

        beaconMapDataPacketSendChunk(    pak,
                                        node->mapData,
                                        &client->requestServer.sentByteCount);

    BEACON_SERVER_PACKET_SEND();
}

void beaconServerAssignProcessNodeToRequestServer(    BeaconProcessQueueNode* processNode,
                                                BeaconServerClientData* requestServer)
{
    BeaconServerClientData* oldRequestServer;

    if(!processNode){
        return;
    }

    oldRequestServer = processNode->requestServer;

    // Clear out the old request server information.

    if(oldRequestServer){
        assert(oldRequestServer->requestServer.processNode == processNode);
        oldRequestServer->requestServer.processNode = NULL;
        processNode->requestServer = NULL;
    }

    processNode->requestServer = requestServer;

    // Set the new request server information.

    if(requestServer){
        beaconServerAssignProcessNodeToRequestServer(requestServer->requestServer.processNode, NULL);

        assert(!requestServer->requestServer.processNode);

        requestServer->requestServer.processNode = processNode;
        requestServer->requestServer.sentByteCount = 0;

        beaconServerSendMapDataToRequestServer(requestServer);
    }
}

static U32 freshCRC(void* dataParam, U32 length){
    cryptAdler32Init();

    return cryptAdler32((U8*)dataParam, length);
}

void beaconServerSetRequestCacheDir(const char* cacheDir){
    estrPrintCharString(&beacon_server.requestCacheDir, cacheDir);

    forwardSlashes(beacon_server.requestCacheDir);

    while(    estrLength(&beacon_server.requestCacheDir) &&
            beacon_server.requestCacheDir[estrLength(&beacon_server.requestCacheDir) - 1] == '/')
    {
        estrSetLength(&beacon_server.requestCacheDir, estrLength(&beacon_server.requestCacheDir) - 1);
    }
}

void beaconServerSetSymStore(void){
    beacon_server.symStore = 1;
}

static S32 beaconServerConnectCallback(NetLink* link){
    BeaconServerClientData*    client = link->userData;

    beaconServerSetSendStatus();

    client->link = link;
    client->connectTime = beaconGetCurTime();
    client->uid = beacon_server.nextClientUID++;

    if(    !beacon_server.localOnly ||
        !stricmp(beaconGetLinkIPStr(link), makeIpStr(getHostLocalIp())))
    {
        beaconServerSetClientState(client, BCS_NOT_CONNECTED);
    }else{
        beaconServerSetClientState(client, BCS_ERROR_NONLOCAL_IP);
    }

    beaconClientPrintf(    client,
                        client->state == BCS_NOT_CONNECTED ? COLOR_GREEN : COLOR_RED,
                        "Link connected!%s\n",
                        client->state == BCS_ERROR_NONLOCAL_IP ? "  Non-local IP!" : "");

    return 1;
}

static S32 beaconServerDisconnectCallback(NetLink* link){
    BeaconServerClientData*    client = link->userData;
    S32 isUpdating = client->state == BCS_NEEDS_MORE_EXE_DATA ||
                     client->state == BCS_RECEIVING_EXE_DATA;

    beaconServerSetSendStatus();

    FOR_CLIENTS_LINK(i, client, curLink)
        if(link == curLink){
            if(i < beacon_server.selectedClient){
                beacon_server.selectedClient--;
            }
            break;
        }
    END_FOR

    beaconClientPrintf(    client,
                        COLOR_RED,
                        "%s%s Disconnected! (%s/%s)\n",
                        isUpdating ? "UPDATING " : "",
                        getClientTypeName(client),
                        client->computerName ? client->computerName : "...",
                        client->userName ? client->userName : "...");

    // Free client processing data.

    removeClientFromBlock(client);

    removeClientFromGroup(client);

    SAFE_FREE(client->userName);
    SAFE_FREE(client->computerName);

    // Free sentry data.

    removeClientFromSentry(client, 0, client->uid, NULL);

    verifySentryIntegrity();

    assert(!client->sentry);

    while(client->sentryClients.count){
        BeaconServerSentryClientData* sentryClient = client->sentryClients.clients;
        BeaconServerClientData* serverClient = sentryClient->client;

        assert(!serverClient || serverClient->sentry == client);

        removeClientFromSentry(sentryClient->client, sentryClient->pid, sentryClient->uid, client);

        assert(!serverClient || !serverClient->sentry);
    }

    SAFE_FREE(client->sentryClients.clients);
    ZeroStruct(&client->sentryClients);

    verifySentryIntegrity();

    // Free the server data.

    estrDestroy(&client->server.mapName);

    // Free the requester data.

    SAFE_FREE(client->requester.dbServerIP);

    beaconServerDetachClientFromLoadRequest(client);

    beaconServerDetachRequesterFromProcessNode(client);

    assert(!client->requester.loadRequest);
    assert(!client->requester.processNode);

    // Free the request server data.

    if(client->requestServer.processNode){
        beaconServerAssignProcessNodeToRequestServer(client->requestServer.processNode, NULL);
    }

    // Reset the client type.

    beaconServerSetClientType(client, BCT_NOT_SET);

    // Make sure no sentries are still referencing me.

    FOR_CLIENTS(i, sentry)
        S32 j;

        for(j = 0; j < sentry->sentryClients.count; j++){
            assert(sentry->sentryClients.clients[j].client != client);
        }
    END_FOR

    return 1;
}

static void beaconServerSendConnectReply(BeaconServerClientData* client, S32 good, S32 sendExeData){
    Packet* pak = pktCreateEx(client->link, BMSG_S2C_CONNECT_REPLY);

    if(good){
        pktSendBits(pak, 1, 1);
        pktSendBits(pak, 32, client->uid);
        pktSendString(pak, beacon_server.serverUID);
    }else{
        beaconServerSetClientState(client, BCS_NEEDS_MORE_EXE_DATA);

        pktSendBits(pak, 1, 0);

        if(sendExeData){
            pktSendBitsPack(pak, 1, beacon_server.exeFile.size);
            pktSendBitsArray(pak, 8 * beacon_server.exeFile.size, beacon_server.exeFile.data);
        }else{
            client->exeData.shouldBeSent = 1;
            client->exeData.sentByteCount = 0;
        }
    }

    pktSend(&pak, client->link);
}

typedef struct TempLegalArea {
    struct TempLegalArea*            nextInAll;
    struct TempLegalArea*            nextInColumn;
    BeaconLegalAreaCompressed*        area;
} TempLegalArea;

TempLegalArea* allTempAreas;

MP_DEFINE(TempLegalArea);

static void insertTempArea(TempLegalArea** cell, BeaconLegalAreaCompressed* area){
    TempLegalArea* newTemp;

    MP_CREATE(TempLegalArea, 256);

    newTemp = MP_ALLOC(TempLegalArea);

    newTemp->nextInAll = allTempAreas;
    allTempAreas = newTemp;

    newTemp->nextInColumn = *cell;
    newTemp->area = area;

    *cell = newTemp;
}

static void integrateNewLegalAreas(    BeaconServerClientData* client,
                                    Packet* pak,
                                    BeaconDiskSwapBlock* block,
                                    S32 checkedAreas,
                                    S32 beaconCount)
{
    S32 client_grid_x = client->assigned.block->x / BEACON_GENERATE_CHUNK_SIZE;
    S32 client_grid_z = client->assigned.block->z / BEACON_GENERATE_CHUNK_SIZE;
    TempLegalArea* areaGrid[BEACON_GENERATE_CHUNK_SIZE][BEACON_GENERATE_CHUNK_SIZE];
    BeaconLegalAreaCompressed* area;
    S32 receiveCount;
    S32 i;
    S32 addedCount = 0;
    S32 preExistCount = 0;

    ZeroArray(areaGrid);

    for(area = block->legalCompressed.areasHead; area; area = area->next){
        insertTempArea(&areaGrid[area->z][area->x], area);

        area->foundInReceiveList = 0;
    }

    receiveCount = pktGetBitsPack(pak, 5);

    beaconVerifyUncheckedCount(block);

    for(i = 0; i < receiveCount; i++){
        U8    x = pktGetBits(pak, 8);
        U8    z = pktGetBits(pak, 8);
        S32 isIndex = pktGetBits(pak, 1);
        S32 y_index = isIndex ? pktGetBitsPack(pak, 5) : 0;
        F32    y_coord = isIndex ? 0 : pktGetF32(pak);
        TempLegalArea* cur;
        BeaconLegalAreaCompressed* area;
        S32 found = 0;

        beaconVerifyUncheckedCount(block);

        for(cur = areaGrid[z][x]; cur; cur = cur->nextInColumn){
            area = cur->area;

            if(area->isIndex == isIndex){
                if(    isIndex && area->y_index == y_index ||
                    !isIndex && area->y_coord == y_coord)
                {
                    beaconReceiveColumnAreas(pak, area);

                    #if BEACONGEN_STORE_AREA_CREATOR
                        area->areas.cx = client_grid_x;
                        area->areas.cz = client_grid_z;
                        area->areas.ip = client->link->addr.sin_addr.S_un.S_addr;
                    #endif

                    beaconVerifyUncheckedCount(block);

                    if(checkedAreas){
                        beaconVerifyUncheckedCount(block);

                        if(!area->checked){
                            area->checked = 1;

                            assert(block->legalCompressed.uncheckedCount > 0);

                            block->legalCompressed.uncheckedCount--;

                            beaconVerifyUncheckedCount(block);
                        }
                    }

                    found = 1;

                    if(!area->foundInReceiveList){
                        area->foundInReceiveList = 1;
                        preExistCount++;
                    }

                    break;
                }
            }
        }

        if(found){
            beaconVerifyUncheckedCount(block);

            continue;
        }

        beaconVerifyUncheckedCount(block);

        area = beaconAddLegalAreaCompressed(block);
        area->x = x;
        area->z = z;
        area->isIndex = isIndex;

        if(isIndex){
            area->y_index = y_index;
        }else{
            area->y_coord = y_coord;
        }

        if(checkedAreas){
            area->checked = 1;
        }else{
            area->checked = 0;

            block->legalCompressed.uncheckedCount++;
        }

        beaconVerifyUncheckedCount(block);

        insertTempArea(&areaGrid[z][x], area);

        addedCount++;

        beaconVerifyUncheckedCount(block);

        beaconReceiveColumnAreas(pak, area);

        #if BEACONGEN_STORE_AREA_CREATOR
            area->areas.cx = client_grid_x;
            area->areas.cz = client_grid_z;
            area->areas.ip = client->link->addr.sin_addr.S_un.S_addr;
        #endif
    }

    if(0){
        beaconClientPrintf(    client, 0,
                            "Added %4d legal areas to (%4d, %4d), %4d unchecked, %4d received, %4d pre-existed",
                            addedCount,
                            block->x / BEACON_GENERATE_CHUNK_SIZE,
                            block->z / BEACON_GENERATE_CHUNK_SIZE,
                            block->legalCompressed.uncheckedCount,
                            receiveCount,
                            preExistCount);

        if(checkedAreas){
            printf(", %4d beacons", beaconCount);
        }

        printf(".\n");
    }

    while(allTempAreas){
        TempLegalArea* next = allTempAreas->nextInAll;
        MP_FREE(TempLegalArea, allTempAreas);
        allTempAreas = next;
    }
}

static void beaconServerSetReachedFromValid(Beacon* beacon){
    if(!beacon->wasReachedFromValid){
        beacon->wasReachedFromValid = 1;

        dynArrayAddStruct(    &beacon_server.beaconConnect.legalBeacons.indexes,
                            &beacon_server.beaconConnect.legalBeacons.count,
                            &beacon_server.beaconConnect.legalBeacons.maxCount);

        beacon_server.beaconConnect.legalBeacons.indexes[beacon_server.beaconConnect.legalBeacons.count - 1] = beacon->userInt;
    }
}

typedef void (*BeaconForEachClientCallbackFunction)(BeaconServerClientData* client, S32 index, void* userData);

static void beaconServerForEachClient(BeaconForEachClientCallbackFunction callbackFunction, void* userData){
    FOR_CLIENTS_LINK(i, client, link)
        if(link->connected){
            callbackFunction(client, i, userData);
        }
    END_FOR
}

static void readConnectHeader(BeaconServerClientData* client, Packet* pak){
    client->exeCRC = pktGetBits(pak, 32);

    if(!client->exeCRC){
        // This a new client.

        client->protocolVersion = pktGetBitsPack(pak, 1);
        client->exeCRC = pktGetBits(pak, 32);
    }

    client->userName = strdup(pktGetString(pak));
    client->computerName = strdup(pktGetString(pak));
}

static void processClientMsgConnect(BeaconServerClientData* client, Packet* pak){
    switch(client->state){
        xcase BCS_ERROR_NONLOCAL_IP:{
            readConnectHeader(client, pak);
        }

        xcase BCS_NOT_CONNECTED:{
            readConnectHeader(client, pak);

            if(client->exeCRC != beacon_server.exeFile.crc){
                if(beaconIsProductionMode()){
                    beaconClientPrintf(    client,
                                        COLOR_YELLOW,
                                        "WARNING: Executable CRC is different!  (%s/%s)\n",
                                        client->computerName,
                                        client->userName);

                    beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
                }else{
                    beaconClientPrintf(    client,
                                        COLOR_YELLOW,
                                        "WARNING: Executable CRC is different!  Sending update. (%s/%s)\n",
                                        client->computerName,
                                        client->userName);

                    beaconServerSendConnectReply(client, 0, client->protocolVersion < 1);
                }
            }else{
                S32 isSentry = pktGetBits(pak, 1);

                beaconServerSetClientType(client, isSentry ? BCT_SENTRY : BCT_WORKER);
                beaconServerSetClientState(client, isSentry ? BCS_SENTRY : BCS_CONNECTED);

                beaconClientPrintf(client, COLOR_GREEN, "Executable CRC matches. (%s/%s)!\n", client->computerName, client->userName);

                beaconServerSendConnectReply(client, 1, 0);
            }
        }

        xdefault:{
            beaconClientPrintf(client, COLOR_RED, "FATAL!  Sent client connect message while connected.\n");

            beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
        }
    }
}

static void beaconServerSendExeUpdate(BeaconServerClientData* client, const char* reason){
    beaconClientPrintf(    client,
                        COLOR_YELLOW,
                        "WARNING: Sending update to: %s/%s.  Reason: %s\n",
                        client->computerName,
                        client->userName,
                        reason);

    beaconServerSendConnectReply(client, 0, client->server.protocolVersion < 2);
}

static void processClientMsgServerConnect(BeaconServerClientData* client, Packet* pak){
    beaconServerSetClientType(client, BCT_SERVER);

    switch(client->state){
        xcase BCS_ERROR_NONLOCAL_IP:{
            client->server.protocolVersion = pktGetBitsPack(pak, 1);
            pktGetBits(pak, 32);
            client->userName = strdup(pktGetString(pak));
            client->computerName = strdup(pktGetString(pak));
        }

        xcase BCS_NOT_CONNECTED:{
            S32 crcMatches;

            client->server.protocolVersion = pktGetBitsPack(pak, 1);

            if(    client->server.protocolVersion < 0 ||
                client->server.protocolVersion > BEACON_SERVER_PROTOCOL_VERSION)
            {
                beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
                break;
            }

            client->exeCRC = pktGetBits(pak, 32);

            crcMatches = client->exeCRC == beacon_server.exeFile.crc;

            client->userName = strdup(pktGetString(pak));
            client->computerName = strdup(pktGetString(pak));
            client->server.port = pktGetBitsPack(pak, 1);

            if(client->server.protocolVersion >= 1){
                client->server.isRequestServer = pktGetBits(pak, 1);
            }

            beaconServerSetClientState(client, BCS_SERVER);

            beaconClientPrintf(    client,
                                crcMatches ? COLOR_GREEN : COLOR_YELLOW,
                                "Server executable CRC %s: %s/%s, Protocol %d.\n",
                                crcMatches ? "matches" : "DOESN'T MATCH",
                                client->computerName,
                                client->userName,
                                client->server.protocolVersion);

            beaconServerSendConnectReply(client, 1, 0);
        }

        xdefault:{
            beaconClientPrintf(client, COLOR_RED, "FATAL!  Sent server connect message while connected.\n");

            beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
        }
    }
}

static void processClientMsgRequesterConnect(BeaconServerClientData* client, Packet* pak){
    beaconServerSetClientType(client, BCT_REQUESTER);

    switch(client->state){
        xcase BCS_ERROR_NONLOCAL_IP:{
            client->server.protocolVersion = pktGetBitsPack(pak, 1);
            client->userName = strdup(pktGetString(pak));
            client->computerName = strdup(pktGetString(pak));
            client->requester.dbServerIP = strdup(pktGetString(pak));
        }

        xcase BCS_NOT_CONNECTED:{
            client->server.protocolVersion = pktGetBitsPack(pak, 1);
            client->userName = strdup(pktGetString(pak));
            client->computerName = strdup(pktGetString(pak));
            client->requester.dbServerIP = strdup(pktGetString(pak));

            if(    client->server.protocolVersion < 0 ||
                client->server.protocolVersion > BEACON_SERVER_PROTOCOL_VERSION)
            {
                beaconClientPrintf(    client,
                                    COLOR_RED,
                                    "ERROR: Protocol %d is out of range [0,%d].\n",
                                    client->server.protocolVersion,
                                    BEACON_SERVER_PROTOCOL_VERSION);

                beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
            }else{
                beaconServerSetClientState(client, BCS_REQUESTER);
            }
        }

        xdefault:{
            beaconClientPrintf(client, COLOR_RED, "FATAL!  Sent requester connect message while connected.\n");

            beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
        }
    }
}

static void processClientMsgGenerateFinished(BeaconServerClientData* client, Packet* pak){
    if(client->state != BCS_GENERATING){
        beaconClientPrintf(client, COLOR_YELLOW, "WARNING: Sent legal areas when not generating.\n");
    }
    else if(!client->assigned.block){
        beaconClientPrintf(client, COLOR_YELLOW, "Finished generating, but not first.\n");
        beaconServerSetClientState(client, BCS_READY_TO_GENERATE);
    }
    else{
        BeaconDiskSwapBlock* block = client->assigned.block;
        S32 client_grid_x = block->x / BEACON_GENERATE_CHUNK_SIZE;
        S32 client_grid_z = block->z / BEACON_GENERATE_CHUNK_SIZE;
        S32 i;

        beaconClientPrintf(client, 0, "Receiving legal areas (%sb).\n", getCommaSeparatedInt(pak->stream.size));

        while(pktGetBits(pak, 1) == 1){
            S32 grid_x = pktGetBitsPack(pak, 1);
            S32 grid_z = pktGetBitsPack(pak, 1);
            S32 dx = grid_x - client_grid_x;
            S32 dz = grid_z - client_grid_z;

            if(abs(dx) <= 1 && abs(dz) <= 1){
                BeaconDiskSwapBlock* block = beaconGetDiskSwapBlockByGrid(grid_x, grid_z);
                S32 beaconCount = 0;

                if(!block){
                    beaconClientPrintf(    client,
                                        COLOR_RED,
                                        "ERROR: Sent nonexistent block (%d,%d) when assigned block (%d,%d).\n",
                                        grid_x,
                                        grid_z,
                                        client_grid_x,
                                        client_grid_z);

                    beaconServerSetClientState(client, BCS_ERROR_DATA);

                    return;
                }

                if(client_grid_x == grid_x && client_grid_z == grid_z){
                    U32 surfaceCRC = pktGetBits(pak, 32);
                    S32 i;

                    if(block->foundCRC){
                        assert(surfaceCRC == block->surfaceCRC);
                    }else{
                        block->foundCRC = 1;
                        block->surfaceCRC = surfaceCRC;
                    }

                    beaconCount = pktGetBitsPack(pak, 1);

                    for(i = 0; i < beaconCount; i++){
                        S32 j = block->generatedBeacons.count;

                        dynArrayAddStruct(    &block->generatedBeacons.beacons,
                                            &block->generatedBeacons.count,
                                            &block->generatedBeacons.maxCount);

                        pktGetBitsArray(pak, 8 * sizeof(Vec3), block->generatedBeacons.beacons[j].pos);
                        block->generatedBeacons.beacons[j].noGroundConnections = pktGetBits(pak, 1);
                    }
                }

                integrateNewLegalAreas(client, pak, block, !dx && !dz, beaconCount);
            }else{
                beaconClientPrintf(    client,
                                    COLOR_RED,
                                    "ERROR: Was processing (%d,%d), but sent (%d,%d)!\n",
                                    client_grid_x,
                                    client_grid_z,
                                    grid_x,
                                    grid_z);

                beaconServerSetClientState(client, BCS_ERROR_DATA);

                return;
            }
        }

        for(i = 0; i < block->clients.count; i++){
            assert(block->clients.clients[i]->assigned.block == block);
            block->clients.clients[i]->assigned.block = NULL;
            block->clients.clients[i] = NULL;
        }

        block->clients.count = 0;

        beaconServerSetClientState(client, BCS_READY_TO_GENERATE);

        client->completed.blockCount++;
    }
}

static void processClientMsgBeaconConnections(BeaconServerClientData* client, Packet* pak){
    if(client->state != BCS_CONNECTING_BEACONS){
        beaconClientPrintf(client, COLOR_YELLOW, "WARNING: Sent beacon connections when not connecting.\n");
    }
    else if(!client->assigned.group){
        beaconClientPrintf(client, COLOR_YELLOW, "Finished connecting, but not first.\n");
        beaconServerSetClientState(client, BCS_READY_TO_CONNECT_BEACONS);
    }
    else{
        BeaconConnectBeaconGroup* group = client->assigned.group;
        S32 lo;
        S32 hi;
        S32 i;

        assert(group);

        lo = group->lo;
        hi = group->hi;

        beaconClientPrintf(client, COLOR_BLUE, "Receiving %3d beacons [%6d-%6d]\n", hi - lo + 1, lo, hi);

        for(i = lo; i <= hi; i++){
            static Array tempGroundConns;
            static Array tempRaisedConns;

            S32 index = pktGetBitsPack(pak, 1);
            S32 beaconCount;
            Beacon* b;
            S32 j;

            assert(index == beacon_server.beaconConnect.legalBeacons.indexes[i]);

            b = combatBeaconArray.storage[index];

            beaconCount = pktGetBitsPack(pak, 1);

            tempGroundConns.size = 0;
            tempRaisedConns.size = 0;

            for(j = 0; j < beaconCount; j++){
                Beacon* targetBeacon;
                BeaconConnection* conn;
                S32 targetIndex = pktGetBitsPack(pak, 1);
                S32 raisedCount;
                S32 k;

                assert(targetIndex >= 0 && targetIndex < combatBeaconArray.size);

                targetBeacon = combatBeaconArray.storage[targetIndex];

                beaconServerSetReachedFromValid(targetBeacon);

                if(pktGetBits(pak, 1)){
                    conn = createBeaconConnection();
                    conn->destBeacon = targetBeacon;
                    arrayPushBack(&tempGroundConns, conn);
                }

                raisedCount = pktGetBitsPack(pak, 1);

                for(k = 0; k < raisedCount; k++){
                    conn = createBeaconConnection();
                    conn->destBeacon = targetBeacon;
                    conn->minHeight = pktGetF32(pak);
                    conn->maxHeight = pktGetF32(pak);
                    arrayPushBack(&tempRaisedConns, conn);
                }
            }

            assert(!b->gbConns.size && !b->rbConns.size);

            beaconInitCopyArray(&b->gbConns, &tempGroundConns);
            beaconInitCopyArray(&b->rbConns, &tempRaisedConns);
        }

        for(i = 0; i < group->clients.count; i++){
            group->clients.clients[i]->assigned.group = NULL;
        }

        group->clients.count = 0;

        group->finished = 1;
        group->next = beacon_server.beaconConnect.groups.finished;
        beacon_server.beaconConnect.groups.finished = group;
        beacon_server.beaconConnect.assignedCount--;
        beacon_server.beaconConnect.finishedCount++;

        beaconServerSetClientState(client, BCS_READY_TO_CONNECT_BEACONS);

        client->completed.beaconCount += hi - lo + 1;
    }
}

static BeaconServerClientData* findClientByUID(U32 clientUID){
    FOR_CLIENTS(i, client)
        if(clientUID == client->uid){
            return client;
        }
    END_FOR
    return NULL;
}

static void processClientMsgSentryClientList(BeaconServerClientData* sentry, Packet* pak){
    S32 i;

    if(sentry->clientType != BCT_SENTRY){
        beaconClientPrintf(sentry, COLOR_RED, "ERROR: I sent a client list, but I'm not a sentry!\n");
        return;
    }

    for(i = 0; i < sentry->sentryClients.count; i++){
        sentry->sentryClients.clients[i].updated = 0;
    }

    //printf("\n\n\n");

    while(pktGetBits(pak, 1) == 1){
        BeaconServerSentryClientData* sentryClient = NULL;
        BeaconServerClientData* client = NULL;
        U32 clientUID;
        char serverUID[100];
        U32    pid;
        U32 forcedInactive;

        clientUID = pktGetBits(pak, 32);
        Strncpyt(serverUID, pktGetString(pak));
        pid = pktGetBits(pak, 32);
        forcedInactive = pktGetBits(pak, 1);

        //printf("received %4d:0x%8.8x\n", pid, client);

        if(stricmp(serverUID, beacon_server.serverUID)){
            client = NULL;
        }else{
            client = findClientByUID(clientUID);

            if(client){
                if(client->link->addr.sin_addr.S_un.S_addr != sentry->link->addr.sin_addr.S_un.S_addr){
                    //beaconClientPrintf(sentry, COLOR_RED, "Sent client ID with different IP address: %s!\n", getClientIPStr(client));
                    client = NULL;
                }
            }
        }

        sentryClient = setClientSentry(client, pid, clientUID, sentry);

        if(sentryClient){
            sentryClient->updated = 1;
            sentryClient->forcedInactive = forcedInactive;
        }

        if(pktGetBits(pak, 1) == 1){
            char* crashText = pktGetString(pak);
            if(sentryClient){
                if(!sentryClient->crashText || stricmp(crashText, sentryClient->crashText)){
                    sentryClient->crashText = strdup(crashText);

                    beaconClientPrintf(    sentry,
                                        COLOR_RED,
                                        "Client crash (%s/%s):\n%s\n",
                                        SAFE_MEMBER(sentryClient->client, computerName),
                                        SAFE_MEMBER(sentryClient->client, userName),
                                        crashText);
                }
            }
        }else{
            SAFE_FREE(sentryClient->crashText);
        }

        if(sentryClient){
            consoleSetColor(COLOR_BRIGHT|COLOR_GREEN, 0);
            //printf("added sentry 0x%8.8x: client 0x%8.8x, pid %d\n%s\n", sentry, sentryClient->client, sentryClient->pid, sentryClient->crashText);
            consoleSetDefaultColor();
        }
    }

    for(i = 0; i < sentry->sentryClients.count; i++){
        if(!sentry->sentryClients.clients[i].updated){
            removeClientFromSentry(    sentry->sentryClients.clients[i].client,
                                    sentry->sentryClients.clients[i].pid,
                                    sentry->sentryClients.clients[i].uid,
                                    sentry);

            i--;
        }
    }
}

static void beaconServerSendStatusAck(BeaconServerClientData* client){
    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_STATUS_ACK);
        pktSendBitsPack(pak, 1, client->server.sendStatusUID);
    BEACON_SERVER_PACKET_SEND();
}

static void processClientMsgServerStatus(BeaconServerClientData* client, Packet* pak){
    if(client->state != BCS_SERVER){
        beaconClientPrintf(client, COLOR_YELLOW, "Sending server status when not connected!\n");
    }else{
        assert(client->clientType == BCT_SERVER);

        estrPrintCharString(&client->server.mapName, pktGetString(pak));

        client->server.clientCount =        pktGetBitsPack(pak, 1);
        client->server.state =                pktGetBitsPack(pak, 1);
        client->server.sendStatusUID =        pktGetBitsPack(pak, 1);
        client->server.sendClientsToMe =    pktGetBits(pak, 1);

        beaconServerSendStatusAck(client);
    }
}

void beaconServerSendRequestChunkReceived(BeaconProcessQueueNode* node){
    if(!node->requester){
        return;
    }

    node->requester->requester.toldToSendRequestTime = beaconGetCurTime();

    node->state = BPNS_WAITING_FOR_MAP_DATA_FROM_CLIENT;

    BEACON_SERVER_PACKET_CREATE_BASE(pak, node->requester, BMSG_S2CT_REQUEST_CHUNK_RECEIVED);
    BEACON_SERVER_PACKET_SEND();
}

static void beaconServerSendRequestAccepted(BeaconServerClientData* client){
    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_REQUEST_ACCEPTED);

        pktSendBitsPack(pak, 1, client->requester.uid);

    BEACON_SERVER_PACKET_SEND();
}

static void processClientMsgRequesterMapData(BeaconServerClientData* client, Packet* pak){
    if(client->clientType != BCT_REQUESTER){
        beaconClientPrintf(client, COLOR_YELLOW, "Sending map data from non-requester client.");
    }else{
        // Requester is sending the map data.

        U32                     uid = pktGetBitsPack(pak, 1);
        char                    uniqueStorageName[1000];
        BeaconMapDataPacket*    tempMapData;

        client->requester.uid = uid;

        Strncpyt(uniqueStorageName, pktGetString(pak));

        beaconMapDataPacketCreate(&tempMapData);

        beaconMapDataPacketReceiveChunkHeader(pak, tempMapData);

        if(beaconMapDataPacketIsFirstChunk(tempMapData)){
            beaconClientPrintf(    client,
                                COLOR_GREEN,
                                "Adding request: \"%s:%d\", crc 0x%8.8x\n",
                                uniqueStorageName,
                                uid,
                                beaconMapDataPacketGetCRC(tempMapData));

            // Receord the time that this request was first received.

            client->requester.startedRequestTime = beaconGetCurTime();

            // Add me to the process queue.

            beaconServerAddRequesterToProcessQueue(    client,
                                                    uniqueStorageName,
                                                    beaconMapDataPacketGetCRC(tempMapData));

            // Create the load request.

            beaconServerAddToBeaconFileLoadQueue(client, beaconMapDataPacketGetCRC(tempMapData));

            // Tell the requester that the request was accepted.

            beaconServerSendRequestAccepted(client);
        }else{
            //beaconPrintf(COLOR_GREEN, "Received next chunk\n");

            if(    client->requester.processNode &&
                client->requester.processNode->state == BPNS_WAITING_FOR_MAP_DATA_FROM_CLIENT)
            {
                client->requester.processNode->state = BPNS_WAITING_TO_REQUEST_MAP_DATA_FROM_CLIENT;
            }
        }

        switch(client->requester.processNode->state){
            xcase    BPNS_WAITING_FOR_LOAD_REQUEST:
            case    BPNS_WAITING_FOR_MAP_DATA_FROM_CLIENT:
            case    BPNS_WAITING_TO_REQUEST_MAP_DATA_FROM_CLIENT:
            {
                // Copy the data if it hasn't been fully received or gotten a successful load yet.

                assert(!beaconMapDataPacketIsFullyReceived(client->requester.processNode->mapData));

                beaconMapDataPacketCopyHeader(client->requester.processNode->mapData, tempMapData);

                beaconMapDataPacketReceiveChunkData(pak, client->requester.processNode->mapData);
            }
        }

        beaconMapDataPacketDestroy(&tempMapData);
    }
}

static void processClientMsgUserInactive(BeaconServerClientData* client, Packet* pak){
    char* reason;

    client->forcedInactive = pktGetBits(pak, 1);
    reason = strdup(pktGetString(pak));

    if(client->forcedInactive){
        beaconClientPrintf(    client,
                            COLOR_GREEN,
                            "User inactive (%s/%s)!  Reason: %s\n",
                            client->computerName,
                            client->userName,
                            reason);
    }else{
        beaconClientPrintf(    client,
                            COLOR_YELLOW,
                            "User active   (%s/%s)!  Reason: %s\n",
                            client->computerName,
                            client->userName,
                            reason);
    }

    SAFE_FREE(reason);
}

static void processClientMsgReadyToWork(BeaconServerClientData* client, Packet* pak){
    switch(client->state){
        xcase BCS_CONNECTED:{
            beaconClientPrintf(client, COLOR_GREEN, "Ready!  (%s/%s)\n", client->computerName, client->userName);
            beaconServerSetClientState(client, BCS_READY_TO_WORK);
            client->readyForCommands = 1;
        }

        xcase BCS_SENTRY:{
            beaconClientPrintf(client, COLOR_GREEN, "Sentry Ready!\n");
        }

        xdefault:{
            beaconClientPrintf(client, COLOR_RED, "FATAL!  Tried to be ready before connecting!\n");
            beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
        }
    }
}

static void beaconServerSendMapDataToWorker(BeaconServerClientData* client){
    BeaconMapDataPacket* mapData = beacon_server.mapData;

    if(!mapData){
        return;
    }

    if(beaconMapDataPacketIsFullySent(mapData, client->mapData.sentByteCount)){
        beaconClientPrintf(    client,
                            COLOR_RED,
                            "ERROR: Client's sent byte count (%d) is higher than total (%d)!\n",
                            client->mapData.sentByteCount,
                            beaconMapDataPacketGetSize(mapData));

        beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
        return;
    }

    client->mapData.lastCommTime = beaconGetCurTime();

    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_MAP_DATA);

        // Send a bit to indicate whether the server cares about the map CRC.

        pktSendBits(pak, 1, beacon_server.isRequestServer ? 1 : 0);

        beaconMapDataPacketSendChunk(pak, mapData, &client->mapData.sentByteCount);

    BEACON_SERVER_PACKET_SEND();

    beaconServerSetClientState(client, BCS_RECEIVING_MAP_DATA);
}

static void processClientMsgNeedMoreMapData(BeaconServerClientData* client, Packet* pak){
    switch(client->clientType){
        xcase BCT_SERVER:{
            beaconServerSendMapDataToRequestServer(client);
        }

        xcase BCT_WORKER:{
            if(client->state == BCS_RECEIVING_MAP_DATA){
                U32 receivedByteCount;

                if(!beaconMapDataPacketReceiveChunkAck(    pak,
                                                        client->mapData.sentByteCount,
                                                        &receivedByteCount))
                {
                    beaconClientPrintf(    client,
                                        COLOR_RED,
                                        "ERROR: Client says %d bytes received, server says %d!\n",
                                        receivedByteCount,
                                        client->mapData.sentByteCount);

                    beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
                    return;
                }

                beaconServerSetClientState(client, BCS_NEEDS_MORE_MAP_DATA);

                client->mapData.lastCommTime = beaconGetCurTime();
                
                beacon_server.noNetWait = 1;
            }
        }
    }
}

static void beaconServerSendNextExeChunk(BeaconServerClientData* client){
    if(    !client->exeData.shouldBeSent ||
        client->exeData.sentByteCount >= beacon_server.exeFile.size)
    {
        return;
    }

    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_EXE_DATA);

        const U32 maxBytesToSend = 32 * 1024;
        U32 bytesRemaining = beacon_server.exeFile.size - client->exeData.sentByteCount;
        U32 bytesToSend = min(bytesRemaining, maxBytesToSend);

        pktSendBitsPack(pak, 1, client->exeData.sentByteCount);
        pktSendBitsPack(pak, 1, beacon_server.exeFile.size);
        pktSendBitsPack(pak, 1, bytesToSend);
        pktSendBitsArray(pak, 8 * bytesToSend, beacon_server.exeFile.data + client->exeData.sentByteCount);

        client->exeData.sentByteCount += bytesToSend;

        if(0){
            beaconClientPrintf(    client,
                                COLOR_GREEN,
                                "Sent exe: %s/%s bytes.\n",
                                getCommaSeparatedInt(client->exeData.sentByteCount),
                                getCommaSeparatedInt(beacon_server.exeFile.size));
        }

    BEACON_SERVER_PACKET_SEND();

    beaconServerSetClientState(client, BCS_RECEIVING_EXE_DATA);

    client->exeData.lastCommTime = beaconGetCurTime();
}

static void processClientMsgNeedMoreExeData(BeaconServerClientData* client, Packet* pak){
    switch(client->state){
        xcase BCS_RECEIVING_EXE_DATA:{
            S32 receivedByteCount = pktGetBitsPack(pak, 1);

            if(receivedByteCount != client->exeData.sentByteCount){
                beaconClientPrintf(    client,
                                    COLOR_RED,
                                    "ERROR: Client says %d exe bytes received, server says %d!\n",
                                    receivedByteCount,
                                    client->exeData.sentByteCount);

                beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
                return;
            }

            beaconServerSetClientState(client, BCS_NEEDS_MORE_EXE_DATA);

            client->exeData.lastCommTime = beaconGetCurTime();

            beacon_server.noNetWait = 1;
        }
    }
}

static void processClientMsgMapDataIsLoaded(BeaconServerClientData* client, Packet* pak){
    switch(client->clientType){
        xcase BCT_SERVER:{
            if(    client->server.isRequestServer &&
                client->requestServer.processNode)
            {
                // This request server has successfully loaded the map file.

                BeaconProcessQueueNode* node = client->requestServer.processNode;
                U32 nodeUID;
                char uniqueStorageName[1000];

                nodeUID = pktGetBitsPack(pak, 1);

                if(node->uid != nodeUID){
                    return;
                }

                Strncpyt(uniqueStorageName, pktGetString(pak));

                if(stricmp(uniqueStorageName, node->uniqueStorageName)){
                    return;
                }

                if(beaconMapDataPacketIsFullySent(    node->mapData,
                                                    client->requestServer.sentByteCount))
                {
                    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_PROCESS_REQUESTED_MAP);
                    BEACON_SERVER_PACKET_SEND();
                }
            }
        }

        xcase BCT_WORKER:{
            if(client->state != BCS_RECEIVING_MAP_DATA){
                return;
            }
            else if(!beaconMapDataPacketIsFullySent(beacon_server.mapData, client->mapData.sentByteCount)){
                beaconClientPrintf(    client,
                                    COLOR_RED,
                                    "ERROR: Client sent %s when %s/%s data was sent.\n",
                                    BMSG_C2ST_MAP_DATA_IS_LOADED,
                                    getCommaSeparatedInt(client->mapData.sentByteCount),
                                    getCommaSeparatedInt(beaconMapDataPacketGetSize(beacon_server.mapData)));

                beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);

                beaconServerSendLoadMapReply(client, 1);
            }
            else{
                U32 clientMapCRC = pktGetBits(pak, 32);

                if(clientMapCRC == beacon_server.mapCRC){
                    beaconClientPrintf(client, COLOR_GREEN, "Good map CRC! (%s/%s)\n", client->computerName, client->userName);

                    beaconServerSetClientState(client, BCS_READY_TO_GENERATE);

                    beaconServerSendLoadMapReply(client, 1);
                }else{
                    beaconClientPrintf(    client,
                                        COLOR_RED,
                                        "FATAL!  Bad map CRC!  (Good: 0x%8.8x, Sent: 0x%8.8x)\n",
                                        beacon_server.mapCRC,
                                        clientMapCRC);

                    beaconServerSetClientState(client, BCS_ERROR_DATA);

                    beaconServerSendLoadMapReply(client, 0);
                }
            }
        }
    }
}

void beaconServerSendBeaconFileToRequester(BeaconServerClientData* client, S32 start){
    BeaconProcessQueueNode* node = client ? client->requester.processNode : NULL;

    if(    !node ||
        !estrLength(&node->uniqueStorageName))
    {
        return;
    }

    if(start){
        node->beaconFile.sentByteCount = 0;
    }

    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_BEACON_FILE);

        U32 remaining = node->beaconFile.byteCount - node->beaconFile.sentByteCount;
        U32 bytesToSend = min(remaining, 64 * 1024);

        pktSendBitsPack(pak, 1, client->requester.uid);
        pktSendString(pak, node->uniqueStorageName);
        pktSendBitsPack(pak, 1, node->beaconFile.byteCount);
        pktSendBitsPack(pak, 1, node->beaconFile.sentByteCount);
        pktSendBitsPack(pak, 1, bytesToSend);
        pktSendBitsPack(pak, 1, node->beaconFile.uncompressedByteCount);

        pktSendBitsArray(    pak,
                            8 * bytesToSend,
                            node->beaconFile.data + node->beaconFile.sentByteCount);

        node->beaconFile.sentByteCount += bytesToSend;

    BEACON_SERVER_PACKET_SEND();
}

static void processClientMsgBeaconFile(BeaconServerClientData* client, Packet* pak){
    BeaconProcessQueueNode* node = client->requestServer.processNode;
    U32 nodeUID;
    char uniqueStorageName[1000];
    U32 readByteCount;

    // Verify that this is a request server.

    if(!client->server.isRequestServer){
        //beaconClientPrintf(    client,
        //                    COLOR_RED,
        //                    "ERROR: Received beacon file from non-request-server client!\n");

        beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
        return;
    }

    // Verify that there is a process node.

    if(!node){
        //beaconClientPrintf(    client,
        //                    COLOR_YELLOW,
        //                    "Received beacon file when no process node is present!\n");
        return;
    }

    // Read the packet.

    nodeUID = pktGetBitsPack(pak, 1);
    Strncpyt(uniqueStorageName, pktGetString(pak));

    if(    node->uid != nodeUID ||
        !node->uniqueStorageName ||
        stricmp(node->uniqueStorageName, uniqueStorageName))
    {
        //beaconClientPrintf(    client,
        //                    COLOR_YELLOW,
        //                    "Received beacon file for client, but the request has since changed.\n");

        return;
    }

    node->beaconFile.receivedByteCount = pktGetBitsPack(pak, 1);
    readByteCount = pktGetBitsPack(pak, 1);
    node->beaconFile.byteCount = pktGetBitsPack(pak, 1);
    node->beaconFile.uncompressedByteCount = pktGetBitsPack(pak, 1);
    node->beaconFile.crc = pktGetBitsPack(pak, 1);

    if(!node->beaconFile.receivedByteCount){
        SAFE_FREE(node->beaconFile.data);

        node->beaconFile.data = malloc(node->beaconFile.byteCount);
    }

    pktGetBitsArray(pak, 8 * readByteCount, node->beaconFile.data + node->beaconFile.receivedByteCount);

    node->beaconFile.receivedByteCount += readByteCount;

    //beaconClientPrintf(    client,
    //                    COLOR_GREEN,
    //                    "Received %s/%s bytes of beacon file.\n",
    //                    getCommaSeparatedInt(node->beaconFile.receivedByteCount),
    //                    getCommaSeparatedInt(node->beaconFile.byteCount));

    if(node->beaconFile.receivedByteCount == node->beaconFile.byteCount){
        // The whole beacon file has been received, so save it and send to the client.

        beaconServerWriteRequestedBeaconFile(node);

        if(node->requester){
            beaconServerSendBeaconFileToRequester(node->requester, 1);

            beaconServerAssignProcessNodeToRequestServer(node, NULL);

            node->state = BPNS_WAITING_FOR_CLIENT_TO_RECEIVE_BEACON_FILE;
        }else{
            beaconServerCancelProcessNode(node);
        }
    }else{
        BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_NEED_MORE_BEACON_FILE);
        BEACON_SERVER_PACKET_SEND();
    }
}

static void processClientMsgNeedMoreBeaconFile(BeaconServerClientData* client, Packet* pak){
    if(    client->requester.processNode &&
        client->requester.processNode->state == BPNS_WAITING_FOR_CLIENT_TO_RECEIVE_BEACON_FILE)
    {
        beaconServerSendBeaconFileToRequester(client, 0);
    }
}

static void beaconServerSendRegenerateRequest(BeaconServerClientData* client){
    if(!client){
        return;
    }

    // Tell the requester that the request needs to be regenerated.

    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_REGENERATE_MAP_DATA);
    BEACON_SERVER_PACKET_SEND();
}

static void processClientMsgRequestedMapLoadFailed(BeaconServerClientData* client, Packet* pak){
    BeaconProcessQueueNode* node = client->requestServer.processNode;
    U32 nodeUID;
    const char* uniqueStorageName;

    if(    !client->server.isRequestServer ||
        !node)
    {
        return;
    }

    // Check the node's UID.

    nodeUID = pktGetBitsPack(pak, 1);

    if(nodeUID != node->uid){
        return;
    }

    // Check the node's uniqueStorageName.

    uniqueStorageName = pktGetString(pak);

    if(    !node->uniqueStorageName ||
        stricmp(node->uniqueStorageName, uniqueStorageName))
    {
        return;
    }

    // Tell the requester to recreate the map data packet.

    beaconServerSendRegenerateRequest(node->requester);

    // Kill the process node.

    beaconServerCancelProcessNode(node);
}

static void processClientMsgPing(BeaconServerClientData* client, Packet* pak){
    // Don't do anything.
}

static void processClientMsgTextCmd(BeaconServerClientData* client, const char* textCmd, Packet* pak){
    #define BEGIN_HANDLERS()    if(0){
    #define HANDLER(x, y)        }else if(!stricmp(textCmd, x)){y(client, pak)
    #define END_HANDLERS()        }

    BEGIN_HANDLERS();
        HANDLER(BMSG_C2ST_READY_TO_WORK,                processClientMsgReadyToWork                );
        HANDLER(BMSG_C2ST_NEED_MORE_MAP_DATA,            processClientMsgNeedMoreMapData            );
        HANDLER(BMSG_C2ST_MAP_DATA_IS_LOADED,            processClientMsgMapDataIsLoaded            );
        HANDLER(BMSG_C2ST_NEED_MORE_EXE_DATA,            processClientMsgNeedMoreExeData            );
        HANDLER(BMSG_C2ST_GENERATE_FINISHED,            processClientMsgGenerateFinished        );
        HANDLER(BMSG_C2ST_BEACON_CONNECTIONS,            processClientMsgBeaconConnections        );
        HANDLER(BMSG_C2ST_SENTRY_CLIENT_LIST,            processClientMsgSentryClientList        );
        HANDLER(BMSG_C2ST_SERVER_STATUS,                processClientMsgServerStatus            );
        HANDLER(BMSG_C2ST_REQUESTER_MAP_DATA,            processClientMsgRequesterMapData        );
        HANDLER(BMSG_C2ST_USER_INACTIVE,                processClientMsgUserInactive            );
        HANDLER(BMSG_C2ST_BEACON_FILE,                    processClientMsgBeaconFile                );
        HANDLER(BMSG_C2ST_NEED_MORE_BEACON_FILE,        processClientMsgNeedMoreBeaconFile        );
        HANDLER(BMSG_C2ST_REQUESTED_MAP_LOAD_FAILED,    processClientMsgRequestedMapLoadFailed    );
        HANDLER(BMSG_C2ST_PING,                            processClientMsgPing                    );
    END_HANDLERS();

    #undef BEGIN_HANDLERS
    #undef HANDLER
    #undef END_HANDLERS
}

static S32 processClientMsg(Packet* pak, S32 cmd, NetLink* link){
    BeaconServerClientData* client = link->userData;

    client->receivedPingTime = beaconGetCurTime();

    switch(cmd){
        xcase BMSG_C2S_CONNECT:{
            processClientMsgConnect(client, pak);
        }

        xcase BMSG_C2S_SERVER_CONNECT:{
            processClientMsgServerConnect(client, pak);
        }

        xcase BMSG_C2S_REQUESTER_CONNECT:{
            processClientMsgRequesterConnect(client, pak);
        }

        xcase BMSG_C2S_TEXT_CMD:{
            char textCmd[1000];

            Strncpyt(textCmd, pktGetString(pak));

            processClientMsgTextCmd(client, textCmd, pak);
        }

        xdefault:{
            beaconClientPrintf(client, COLOR_RED, "ERROR: Client sent unknown cmd(%d).\n", cmd);
            beaconServerSetClientState(client, BCS_ERROR_PROTOCOL);
        }
    }

    return 0;
}

static void beaconResetClient(BeaconServerClientData* client, S32 index, void* userData){
    removeClientFromBlock(client);
    removeClientFromGroup(client);

    client->completed.beaconCount = 0;
    client->completed.blockCount = 0;

    switch(client->state){
        case BCS_READY_TO_WORK:
        case BCS_RECEIVING_MAP_DATA:
        case BCS_READY_TO_GENERATE:
        case BCS_GENERATING:
        case BCS_READY_TO_CONNECT_BEACONS:
        case BCS_CONNECTING_BEACONS:
            beaconServerSetClientState(client, BCS_CONNECTED);
            beaconServerSendConnectReply(client, 1, 0);
    }
}

static void beaconServerFixBadGroupDefs(void){
    S32 i;

    for(i = 0; i < group_info.file_count; i++){
        GroupFile* file = group_info.files[i];
        S32 j;
        for(j = 0; j < file->count; j++){
            GroupDef* def = file->defs[j];
            S32 k;
            for(k = 0; k < def->count; k++){
                if(def->entries[k].def->file->pre_alloc){
                    beaconPrintf(    COLOR_YELLOW,
                                    "WARNING: Removing GroupEnt reference to: %s\n",
                                    def->entries[k].def->name);

                    def->entries[k--] = def->entries[--def->count];
                }
            }
        }
    }

    for(i = 0; i < group_info.ref_count; i++){
        if(    group_info.refs[i]->def &&
            group_info.refs[i]->def->file &&
            group_info.refs[i]->def->file->pre_alloc)
        {
            beaconPrintf(    COLOR_YELLOW,
                            "WARNING: Removing base reference to: %s\n",
                            group_info.refs[i]->def->name);

            group_info.refs[i--] = group_info.refs[--group_info.ref_count];
        }
    }
}

static void beaconServerLoadMap(const char* mapFileName){
    strcpy(db_state.map_name, mapFileName);

    // Load the map file data.

    groupLoadMap(db_state.map_name, 0, 0);

    beaconServerFixBadGroupDefs();

    beaconCalculateMapCRC(beacon_server.fullCRCInfo);

    beaconCurTimeString(1);
}

static void beaconServerResetMapData(void){
    beaconServerForEachClient(beaconResetClient, NULL);

    beaconResetMapData();

    beaconClearBeaconData(1, 1, 1);

    beaconMapDataPacketDestroy(&beacon_server.mapData);
}

static const char* beaconServerGetPathAtMaps(const char* mapFileName){
    const char* fileNameAtMaps = strstriConst(mapFileName, "maps/");

    if(!fileNameAtMaps){
        fileNameAtMaps = strstriConst(mapFileName, "maps\\");
    }

    if(    fileNameAtMaps &&
        (    fileNameAtMaps == mapFileName ||
            fileNameAtMaps[-1] == '/' ||
            fileNameAtMaps[-1] == '\\'))
    {
        return fileNameAtMaps;
    }else{
        return mapFileName;
    }
}

static void beaconServerGetBeaconLogFile(    char* fileNameOut,
                                            const char* dir,
                                            const char* fileName)
{
    char    buffer[1000];

    if(fileName){
        strcpy(buffer, fileName);
    }else{
        const char*    fileNameAtMaps = beaconServerGetPathAtMaps(beacon_server.curMapName);
        char*        s;

        strcpy(buffer, fileNameAtMaps);

        forwardSlashes(buffer);

        for(s = buffer; *s; s++){
            if(*s == '/' || *s == '\\' || *s == ':'){
                *s = '.';
            }
        }
    }

    sprintf(fileNameOut,
            "c:/beaconizer/info/branch%d/%s/%s",
            9999,
            dir,
            buffer);
}

static StashTable openMapLogFileHandles;

static void beaconServerDeleteMapLogFile(const char* dir){
    char fileName[MAX_PATH];
    
    beaconServerGetBeaconLogFile(fileName, dir, NULL);
    
    DeleteFileA(fileName);
}

static FILE* beaconServerGetMapLogFileHandle(const char* dir){
    StashElement element;
    FILE* f;
    
    if(!openMapLogFileHandles){
        openMapLogFileHandles = stashTableCreateWithStringKeys(100, StashDeepCopyKeys);
    }
    
    if(stashFindElementConst(openMapLogFileHandles, dir, &element)){
        f = stashElementGetPointer(element);
        
        assert(f);
    }else{
        char fileName[MAX_PATH];
        
        beaconServerGetBeaconLogFile(fileName, dir, NULL);
        
        makeDirectoriesForFile(fileName);
        
        f = fopen(fileName, "wt");
        
        if(!f){
            return NULL;
        }
        
        fprintf(f, "loadmap %s\n", beaconServerGetPathAtMaps(beacon_server.curMapName));

        stashAddPointer(openMapLogFileHandles, dir, f, 0);
    }
    
    return f;
}

static void beaconServerCloseMapLogFileHandle(FILE* f){
    fclose(f);
}

static void beaconServerCloseMapLogFileHandles(void){
    if(!openMapLogFileHandles){
        return;
    }
    
    stashTableClearEx(openMapLogFileHandles, NULL, beaconServerCloseMapLogFileHandle);
}

static void beaconServerWriteMapLog(const char* dir, const char* format, ...){
    FILE* f = beaconServerGetMapLogFileHandle(dir);
    
    if(!f){
        return;
    }

    VA_START(argptr, format);
    vfprintf(f, format, argptr);
    VA_END();
}

static void beaconServerVerifyEncounterPositions(void){
    S32 progress = -1;
    S32 set;

    beaconPathInit(1);

    printf("Checking Encounter Positions (%d): ", bp_blocks.encounterPositions.count);

    for(set = 0; set < 2; set++){
        const char* logFileDir = set ? "bad_encounter_positions_40" : "bad_encounter_positions";
        S32 setPositionCount = 0;
        S32 i;

        beaconServerDeleteMapLogFile(logFileDir);

        for(i = 0; i < bp_blocks.encounterPositions.count; i++){
            S32 curProgress = (i + 1) * 50 / bp_blocks.encounterPositions.count;
            S32 validPos = 0;

            while(progress < curProgress){
                progress++;
                printf(".");
            }

            // set: 0 = !40, 1 = 40.

            if(set != (bp_blocks.encounterPositions.pos[i].actorID == 40)){
                continue;
            }

            if(0){
                Vec3 pos = { -419.85, 0, -1338.54 };

                if(distance3XZ(pos, bp_blocks.encounterPositions.pos[i].pos) < 1){
                    printf("");
                }
            }

            if(beaconGetClosestCombatBeaconVerifier(bp_blocks.encounterPositions.pos[i].pos)){
                validPos = 1;
            }else{
                Vec3 snappedPos;

                EncounterDoZDrop(bp_blocks.encounterPositions.pos[i].pos, snappedPos);

                if(beaconGetClosestCombatBeaconVerifier(snappedPos)){
                    validPos = 1;
                }
            }

            if(!validPos){
                printf("x");

                if(set && !setPositionCount){
                    beaconServerWriteMapLog(logFileDir, ";----- Position 40s ------------------------------------\n");
                }

                beaconServerWriteMapLog(logFileDir,
                                        "setpos %1.2f %1.2f %1.2f\n",
                                        vecParamsXYZ(bp_blocks.encounterPositions.pos[i].pos));

                setPositionCount++;
            }
        }
    }

    printf("\n");

}

static S32 beaconServerBeginMapProcess(char* mapFileName){
    S32 canceled = 0;

    beaconServerResetMapData();

    beaconServerLoadMap(mapFileName);

    // Initalize Filenames
    beaconCreateFilenames();

    // See if we need to reprocess
    if(beaconFileIsUpToDate(beacon_server.forceRebuild))
    {
        canceled = 1;
    }
    else if(!beaconCheckoutBeaconFiles(beacon_server.forceRebuild, 0)) // Checkout the beacon files
    {
        canceled = 1;
    }
    else if(beacon_server.checkEncounterPositionsOnly){
        canceled = 1;

        beaconPrintf(COLOR_RED, "CANCELED: Check Encounter Positions Only is ON!\n");
    }

    if(canceled){
        beaconProcessUndoCheckouts();
        beaconServerVerifyEncounterPositions();

        return 0;
    }

    beaconClearBeaconData(1, 1, 1);

    beacon_server.mapCRC = beaconGetMapCRC(1, 0, 0);

    printf("Map CRC: 0x%8.8x\n", beacon_server.mapCRC);

    beaconMapDataPacketFromMapData(&beacon_server.mapData, 0);

    return 1;
}

static S32 fileExistsInList(const char** mapList, const char* findName, S32* index){
    S32 i;
    S32 size = eaSize(&mapList);
    for(i = 0; i < size; i++){
        if(!stricmp(mapList[i], findName)){
            if(index){
                *index = i;
            }
            return 1;
        }
    }
    return 0;
}

static const char*    fileWildcard;
static S32            fileWildcardHasStar;
static S32            validFileFound;
static S32            storeToQueue;

static void storeFileName(char* fileName, S32 quietDupes){
    char*** theList = storeToQueue ? &beacon_server.queueList : &beacon_server.mapList;

    if(!fileExists(fileName)){
        beaconPrintf(COLOR_YELLOW, "    Non-existent ignored: %s\n", fileName);
    }
    else if(fileExistsInList(*theList, fileName, NULL)){
        if(!quietDupes){
            beaconPrintf(COLOR_YELLOW, "    Duplicate ignored: %s\n", fileName);
        }
    }
    else{
        char* newFileName = strdup(fileName);

        eaPush(theList, newFileName);

        //printf("MAP: %s\n", buffer + 5);
    }
}

static FileScanAction storeFileNameCallback(char* dir, struct _finddata32_t* data){
    char* name = data->name;

    if(    !(data->attrib & _A_SUBDIR) &&
        !strchr(name, ' ') &&
        name[0] != '_' &&
        !strstri(name, "_layer_") &&
        (    fileWildcardHasStar && simpleMatch(fileWildcard, name) ||
            !fileWildcardHasStar && !stricmp(fileWildcard, name)))
    {
        char buffer[1000];

        validFileFound = 1;

        sprintf(buffer, "%s/%s", dir, name);

        forwardSlashes(buffer);

        storeToQueue = 0;

        storeFileName(buffer, 0);
    }

    return FSA_NO_EXPLORE_DIRECTORY;
}

static void beaconServerLoadMapListFile(char* fileName, S32 addToQueue){
    S32 previousCount = eaSize(&beacon_server.mapList);
    FILE* f = fileOpen(fileName, "rt");
    char buffer[1000];

    if(!f){
        return;
    }

    printf("  Map list file: ");
    beaconPrintf(COLOR_YELLOW, "%s\n", addToQueue ? fileName : getFileName(fileName));

    while(fgets(buffer, ARRAY_SIZE(buffer) - 1, f)){
        S32 len = (S32)strlen(buffer);
        char* tokens[2];
        S32 token_count = 0;

        if(!len || buffer[0] == ';'){
            continue;
        }

        if(buffer[len-1] == '\n'){
            buffer[len-1] = '\0';
        }

        while(tokens[token_count] = strtok(token_count ? NULL : buffer, " \t")){
            if(++token_count == 2){
                break;
            }
        }

        if(token_count <= 0 || tokens[0][0] == ';'){
            continue;
        }

        if(token_count == 1)
        {
            if(fileExists(tokens[0])){
                storeToQueue = 1;
                storeFileName(tokens[0], 0);
            }else{
                beaconPrintf(COLOR_RED, "WARNING: Non-existent file: %s\n", tokens[0]);
            }
        }
        else if(token_count == 2){
            char mapDir[1000];

            validFileFound = 0;

            sprintf(mapDir, "maps/%s", tokens[0]);
            fileWildcard = tokens[1];
            fileWildcardHasStar = strstr(fileWildcard, "*") ? 1 : 0;
            fileScanAllDataDirs(mapDir, storeFileNameCallback);

            if(!validFileFound){
                beaconPrintf(COLOR_RED, "WARNING: No valid map files found in \"%s/%s\"!!!\n", mapDir, fileWildcard);
            }
        }
        else{
            S32 i;

            beaconPrintf(COLOR_RED, "Bad line: ");

            for(i = 0; i < token_count; i++){
                beaconPrintf(COLOR_YELLOW, "%s ", tokens[i]);
            }

            printf("\n");
        }
    }

    fclose(f);

    beaconPrintf(COLOR_GREEN, "    Done: Added %d maps.\n", eaSize(&beacon_server.mapList) - previousCount);
}

static FileScanAction readMapFile(char* dir, struct _finddata32_t* data){
    char* name = data->name;

    if(!(data->attrib & _A_SUBDIR) && simpleMatch("maps*.txt", name)){
        char buffer[1000];
        sprintf(buffer, "%s/%s", dir, name);
        beaconServerLoadMapListFile(buffer, 0);
    }

    return FSA_NO_EXPLORE_DIRECTORY;
}

static void beaconServerLoadMapFilesFromSpec(void){
    static S32 loadedSpecs;

    S32 previousCount;
    char buffer[1000];
    char** missionMapNames = NULL;
    S32 i;

    if(!loadedSpecs){
        loadedSpecs = 1;

        MapSpecReload();
        MissionSpecReload();
        MissionMapPreload();
        arenaLoadMaps();
    }

    storeToQueue = 0;

    // Arenas.

    printf("  Arenas:\n");
    beaconPrintfDim(COLOR_GREEN, "    Source: server/arena/arenamaps.def\n");
    previousCount = eaSize(&beacon_server.mapList);

    for(i = 0; i < eaSize(&g_arenamaplist.maps); i++){
        if(g_arenamaplist.maps[i]->mapname){
            sprintf(buffer, "%s/%s", beaconServerGetDataPath(), g_arenamaplist.maps[i]->mapname);
            forwardSlashes(buffer);
            storeFileName(buffer, 0);
        }
    }

    beaconPrintf(COLOR_GREEN, "    Done: Added %d maps.\n", eaSize(&beacon_server.mapList) - previousCount);

    // Custom missions.

    printf("  Custom Missions:\n");
    beaconPrintfDim(COLOR_GREEN, "    Source: specs/missions.spec\n");
    previousCount = eaSize(&beacon_server.mapList);

    for(i = 0; i < eaSize(&missionSpecFile.missionSpecs); i++){
        if(missionSpecFile.missionSpecs[i]->mapFile && !missionSpecFile.missionSpecs[i]->varMapSpec){
            sprintf(buffer, "%s/%s", beaconServerGetDataPath(), missionSpecFile.missionSpecs[i]->mapFile);
            forwardSlashes(buffer);
            storeFileName(buffer, 0);
        }
    }

    beaconPrintf(COLOR_GREEN, "    Done: Added %d maps.\n", eaSize(&beacon_server.mapList) - previousCount);

    // Generic missions.

    printf("  Generic Missions:\n");
    beaconPrintfDim(COLOR_GREEN, "    Source: maps/missions/mission.mapspecs\n");
    previousCount = eaSize(&beacon_server.mapList);

    MissionMapGetAllMapNames(&missionMapNames);

    for(i = 0; i < eaSize(&missionMapNames); i++){
        sprintf(buffer, "%s/%s", beaconServerGetDataPath(), missionMapNames[i]);
        forwardSlashes(buffer);
        storeFileName(buffer, 1);
    }

    beaconPrintf(COLOR_GREEN, "    Done: Added %d maps.\n", eaSize(&beacon_server.mapList) - previousCount);

    // Zones.

    printf("  Zones:\n");
    beaconPrintfDim(COLOR_GREEN, "    Source: specs/maps.spec\n");
    previousCount = eaSize(&beacon_server.mapList);

    for(i = 0; i < eaSize(&mapSpecList.mapSpecs); i++){
        char zoneName[1000];
        char* ext;

        strcpy(zoneName, mapSpecList.mapSpecs[i]->mapfile);

        ext = strstr(zoneName, ".txt");

        if(ext){
            ext[0] = 0;
            sprintf(buffer, "%s/maps/City_Zones/%s/%s.txt", beaconServerGetDataPath(), zoneName, zoneName);
            forwardSlashes(buffer);
            storeFileName(buffer, 0);
        }
    }

    beaconPrintf(COLOR_GREEN, "    Done: Added %d maps.\n", eaSize(&beacon_server.mapList) - previousCount);

    eaDestroyEx(&missionMapNames, NULL);
}

static void beaconServerLoadLocalMapList(void){
    char* mapListLocal = "c:/maps_local.txt";

    if(fileExists(mapListLocal)){
        printf("Reading personal map files: %s\n", mapListLocal);
        beaconServerLoadMapListFile(mapListLocal, 0);
    }
}

static void beaconServerLoadMapFileList(S32 justLocal){
    char* mapListFileDir = "server/maps/beaconprocess";

    eaClearEx(&beacon_server.mapList, NULL);

    beaconServerLoadLocalMapList();

    if(!justLocal){
        printf("Reading map file names from spec files:\n");

        beaconServerLoadMapFilesFromSpec();

        printf("Reading maplist files:\n");
        beaconPrintfDim(COLOR_GREEN, "  Source: %s/maps*.txt\n", mapListFileDir);

        fileScanAllDataDirs(mapListFileDir, readMapFile);
    }

    beacon_server.mapCount = eaSize(&beacon_server.mapList);
    beacon_server.curMapIndex = -1;

    //if(!beacon_server.mapCount){
    //    sprintf(buffer, "No existing maps specified in: %s", fileName);
    //    assertmsg(0, buffer);
    //    exit(0);
    //}

    printf("Added %d map files to the list.\n", beacon_server.mapCount);
}

static const char* getClientStateName(BeaconClientState state){
    static S32 init;

    if(!init){
        S32 i;

        init = 1;

        for(i = 0; i < BCS_COUNT; i++){
            getClientStateName(i);
        }
    }

    switch(state){
        #define CASE(x) xcase x:return #x + 4
        CASE(BCS_NOT_CONNECTED);
        CASE(BCS_CONNECTED);
        CASE(BCS_READY_TO_WORK);
        CASE(BCS_RECEIVING_MAP_DATA);
        CASE(BCS_NEEDS_MORE_MAP_DATA);
        CASE(BCS_READY_TO_GENERATE);
        CASE(BCS_GENERATING);
        CASE(BCS_READY_TO_CONNECT_BEACONS);
        CASE(BCS_CONNECTING_BEACONS);

        CASE(BCS_SENTRY);
        CASE(BCS_SERVER);
        CASE(BCS_NEEDS_MORE_EXE_DATA);
        CASE(BCS_RECEIVING_EXE_DATA);

        CASE(BCS_REQUESTER);

        CASE(BCS_REQUEST_SERVER_IDLE);
        CASE(BCS_REQUEST_SERVER_PROCESSING);

        CASE(BCS_ERROR_DATA);
        CASE(BCS_ERROR_PROTOCOL);
        CASE(BCS_ERROR_NONLOCAL_IP);
        #undef CASE
        xdefault:{
            assert(0);
            return "UNKNOWN";
        }
    }
}

static U32 getClientStateColor(BeaconClientState state){
    switch(state){
        case BCS_ERROR_DATA:
        case BCS_ERROR_PROTOCOL:
            return COLOR_BRIGHT|COLOR_RED;
        case BCS_NEEDS_MORE_EXE_DATA:
        case BCS_RECEIVING_MAP_DATA:
        case BCS_SENTRY:
            return COLOR_BRIGHT|COLOR_RED|COLOR_GREEN;
        case BCS_ERROR_NONLOCAL_IP:
            return COLOR_RED|COLOR_GREEN;
        case BCS_SERVER:
            return COLOR_BRIGHT|COLOR_GREEN;
        default:
            return COLOR_RED|COLOR_GREEN|COLOR_BLUE;
    }
}

static void beaconServerCheckWindow(void){
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
            POINT    pt;
            RECT    rectDesktop;

            if(    GetWindowRect(GetDesktopWindow(), &rectDesktop) &&
                GetCursorPos(&pt) &&
                SQR(pt.x - rectDesktop.right) + SQR(pt.y - rectDesktop.top) < SQR(10))
            {
                ShowWindow(beaconGetConsoleWindow(), SW_RESTORE);
            }
        }
    }
}

static DWORD WINAPI beaconServerWindowThread(void* unused){
    while(1){
        beaconServerCheckWindow();
        Sleep(500);
    }
}

static void beaconServerSetIcon(U8 letter, U32 colorRGB){
    if(letter){
        beacon_server.icon.letter = letter;
    }
    
    if(colorRGB){
        beacon_server.icon.color = colorRGB;
    }
    
    if(beacon_server.icon.letter){
        setWindowIconColoredLetter(    beaconGetConsoleWindow(),
                                    beacon_server.icon.letter,
                                    beacon_server.icon.color);
    }
}

static void beaconServerGetLatestData(void){
    
    _chdir("c:\\game\\");
    
    beaconPrintf(COLOR_YELLOW, "Skipping Perforce get because it is disabled\n");
}

static void beaconServerInitNetwork(void){
    if(!beacon_server.clients.destroyCallback){
        S32 basePort = beacon_server.isMasterServer ? BEACON_MASTER_SERVER_PORT : BEACON_SERVER_PORT;
        S32 portRange = beacon_server.isMasterServer ? 1 : 101;
        S32 i;

        beaconPrintf(COLOR_GREEN, "Initializing network: ");

        // Initialize networking.

        for(i = 0; i < portRange; i++){
            S32 portToTry = basePort + i;

            if(    !beacon_server.isMasterServer &&
                portToTry == BEACON_MASTER_SERVER_PORT)
            {
                continue;
            }

            if(netInit(&beacon_server.clients, 0, portToTry)){
                break;
            }
        }

        if(i == portRange){
            beaconPrintf(    COLOR_RED,
                            "ERROR: Can't bind any ports in range %d-%d!\n",
                            basePort,
                            basePort + portRange - 1);

            assert(0);
            exit(-1);
        }

        beacon_server.port = basePort + i;

        beacon_server.clients.destroyCallback = beaconServerDisconnectCallback;

        NMAddLinkList(&beacon_server.clients, processClientMsg);

        beaconPrintf(COLOR_GREEN, "  Done (port %d)!\n", beacon_server.port);
    }
}

#define BUFSIZE 1000
static char* beaconMyCmdLineParams(S32 noNetStart){
    static char* buffer;

    if(!buffer){
        buffer = beaconMemAlloc("buffer", BUFSIZE);
    }

    STR_COMBINE_BEGIN_S(buffer, BUFSIZE);
    STR_COMBINE_CAT(" -noencrypt");
    if(beacon_server.isMasterServer){
        STR_COMBINE_CAT(" -beaconmasterserver");

        if(noNetStart){
            STR_COMBINE_CAT(" -beaconnonetstart");
        }

        if(estrLength(&beacon_server.requestCacheDir)){
            STR_COMBINE_CAT(" -beaconrequestcachedir ");
            STR_COMBINE_CAT(beacon_server.requestCacheDir);
        }
    }
    else if(beacon_server.isAutoServer){
        STR_COMBINE_CAT(" -beaconautoserver");

        if(beacon_server.masterServerName){
            STR_COMBINE_CAT(" ");
            STR_COMBINE_CAT(beacon_server.masterServerName);
        }
    }
    else if(beacon_server.isRequestServer){
        STR_COMBINE_CAT(" -beaconrequestserver");

        if(beacon_server.masterServerName){
            STR_COMBINE_CAT(" ");
            STR_COMBINE_CAT(beacon_server.masterServerName);
        }
    }
    else{
        STR_COMBINE_CAT(" -beaconserver");

        if(beacon_server.masterServerName){
            STR_COMBINE_CAT(" ");
            STR_COMBINE_CAT(beacon_server.masterServerName);
        }
    }
    if(beacon_server.dataToolsRootPath){
        STR_COMBINE_CAT(" -beacondatatoolsrootpath \"");
        STR_COMBINE_CAT(beacon_server.dataToolsRootPath);
        STR_COMBINE_CAT("\"");
    }
    STR_COMBINE_END();

    beaconGetCommonCmdLine(buffer + strlen(buffer));

    return buffer;
}
#undef BUFSIZE

static void beaconServerInstall(void){
    HANDLE hMutex;
    DWORD result;

    if(beaconIsProductionMode()){
        return;
    }

    hMutex = CreateMutexA(NULL, 0, "Global\\CrypticBeaconServerInstall");

    assert(hMutex);

    result = WaitForSingleObject(hMutex, 0);

    if(result == WAIT_OBJECT_0 || result == WAIT_ABANDONED){
        char buffer[1000];

        sprintf(buffer, "c:/Beaconizer/%s", beaconServerExeName);

        if(stricmp(buffer, beaconGetExeFileName())){
            //printf("Installing BeaconClient in registry: \n%s\n%s\n", buffer, beaconGetExeFileName());

            beaconCreateNewExe(buffer, beacon_server.exeFile.data, beacon_server.exeFile.size);
        }
    }

    beaconReleaseAndCloseMutex(&hMutex);
}

static void beaconServerAddToSymStore(const char* fileName){
    char buffer[1000];
    char fileNameBackSlashes[1000];
    strcpy_s(SAFESTR(fileNameBackSlashes), fileName);
    backSlashes(fileNameBackSlashes);
    beaconPrintf(COLOR_YELLOW, "Adding \"%s\" to symstore (N:\\nobackup\\symserv\\dataVS8)...\n", fileName);
    sprintf(buffer, "symstoreVS8 add /f \"%s\" /s N:\\nobackup\\symserv\\dataVS8 /t \"Cryptic Programs\"", fileNameBackSlashes);
    beaconPrintfDim(COLOR_YELLOW, "Command: \"%s\"\n", buffer);
    system(buffer);
}

static void beaconServerConnectToDBServer(void){
    if(!beaconIsProductionMode()){
        return;
    }

    if(!db_state.server_name || !db_state.server_name[0]){
        assertmsg(0, "No dbserver defined (\"-db <server>\" cmdline option)!");
    }else{
        beaconPrintf(    COLOR_GREEN,
                        "Connecting to dbserver (%s:%d): ",
                        db_state.server_name,
                        DEFAULT_BEACONSERVER_PORT);

        netConnect(&beacon_server.db_link, db_state.server_name, DEFAULT_BEACONSERVER_PORT, NLT_TCP, 30.0f, NULL);

        if(beacon_server.db_link.connected){
            beaconPrintf(COLOR_GREEN, "Done!\n");
        }else{
            beaconPrintf(COLOR_RED, "FAILED!\n");
            exit(0);
        }
    }
}

static void beaconServerStartupSetServerType(BeaconizerType beaconizerType){
    switch(beaconizerType){
        xcase BEACONIZER_TYPE_MASTER_SERVER:
            beaconServerSetIcon('M', BEACON_MASTER_SERVER_ICON_COLOR);
            beacon_server.isMasterServer = 1;

        xcase BEACONIZER_TYPE_AUTO_SERVER:
            beaconServerSetIcon('A', BEACON_SERVER_ICON_COLOR);
            beacon_server.isAutoServer = 1;

        xcase BEACONIZER_TYPE_SERVER:
            beaconServerSetIcon('S', BEACON_SERVER_ICON_COLOR);

        xcase BEACONIZER_TYPE_REQUEST_SERVER:
            beaconServerSetIcon('R', BEACON_SERVER_ICON_COLOR);
            beacon_server.isRequestServer = 1;

        xdefault:
            assertmsg(0, "Bad beacon server type!");
    }

}

static void beaconServerStartup(BeaconizerType beaconizerType,
                                const char* masterServerName,
                                S32 noNetStart,
                                S32 onePassOnly,
                                S32 forceRebuild)
{
    S32 processPriority = NORMAL_PRIORITY_CLASS;
    S32 threadPriority    = THREAD_PRIORITY_NORMAL;
    
    // Minimize the window.
    
    ShowWindow(beaconGetConsoleWindow(), SW_MINIMIZE);

    // Make sure the state list is complete.

    getClientStateName(0);

    if(beaconIsProductionMode()){
        beaconPrintf(COLOR_GREEN, "BeaconServer is in PRODUCTION MODE.\n");
    }

    beaconServerStartupSetServerType(beaconizerType);
    beacon_server.onePassOnly = onePassOnly;
    beacon_server.forceRebuild = forceRebuild;

    if(!beacon_server.isMasterServer){
        if(masterServerName){
            beacon_server.masterServerName = strdup(masterServerName);

            beaconPrintf(COLOR_GREEN, "Using master server:   %s\n", masterServerName);
        }
    
        beaconPrintf(COLOR_GREEN, "Using data/tools root: %s\n", beaconServerGetDataToolsRootPath());
        beaconPrintf(COLOR_GREEN, "Data path:             %s\n", fileDataDir());
    }
    
    // Check if the mapserver should be installed in the symstore.

    if(    beacon_server.isMasterServer &&
        beacon_server.symStore &&
        !beaconIsProductionMode() &&
        !strnicmp(beaconGetExeFileName(), "c:/src/", 7) &&
        strEndsWith(beaconGetExeFileName(), "/mapserver.exe"))
    {
        S32 result = MessageBoxA(NULL,
                                "Add beaconizer to symstore?",
                                "Look at me!",
                                MB_YESNO | MB_SYSTEMMODAL | MB_ICONWARNING);
                                
        if(result == IDYES){
            char fileName[MAX_PATH];
            char pdbFileName[MAX_PATH];

            // Do BeaconServer.exe.

            strcpy(fileName, beaconGetExeFileName());
            
            getDirectoryName(fileName);
            strcat(fileName, "/BeaconServer.exe");
            if(!CopyFileA(beaconGetExeFileName(), fileName, FALSE)){
                beaconPrintf(COLOR_RED, "ERROR: Can't create file: %s\n", fileName);
            }else{
                beaconServerAddToSymStore(fileName);
            }
            
            // Do BeaconClient.exe.

            getDirectoryName(fileName);
            strcat(fileName, "/BeaconClient.exe");
            if(!CopyFileA(beaconGetExeFileName(), fileName, FALSE)){
                beaconPrintf(COLOR_RED, "ERROR: Can't create file: %s\n", fileName);
            }else{
                beaconServerAddToSymStore(fileName);
            }

            // Do BeaconServer.pdb.
            
            strcpy(fileName, beaconGetExeFileName());
            strcpy(fileName + strlen(fileName) - 4, ".pdb");
            strcpy(pdbFileName, fileName);
            
            getDirectoryName(fileName);
            strcat(fileName, "/BeaconServer.pdb");
            if(!CopyFileA(pdbFileName, fileName, FALSE)){
                beaconPrintf(COLOR_RED, "ERROR: Can't create file: %s\n", fileName);
            }else{
                beaconServerAddToSymStore(fileName);
            }
            
            // Do BeaconClient.exe.

            getDirectoryName(fileName);
            strcat(fileName, "/BeaconClient.pdb");
            if(!CopyFileA(pdbFileName, fileName, FALSE)){
                beaconPrintf(COLOR_RED, "ERROR: Can't create file: %s\n", fileName);
            }else{
                beaconServerAddToSymStore(fileName);
            }
        }
        
        printf("Done!\n");
        getch();
        exit(0);
    }

    if(!beaconIsProductionMode()){
        checkForCorrectExePath(beaconServerExeName, beaconMyCmdLineParams(noNetStart), 0, 0);
        pktSetDebugInfo();
    }

    if(beacon_server.isMasterServer){
        // Set the default request cache directory if it isn't set from the command line.

        if(!estrLength(&beacon_server.requestCacheDir)){
            beaconServerSetRequestCacheDir("c:\\beaconizer\\requestcache\\");
        }

        beaconPrintf(COLOR_GREEN, "Using beacon request cache dir: %s\n", beacon_server.requestCacheDir);

        // Start the background loader thread.

        beaconServerInitLoadRequests();

        // Start the background request writer/reader thread.

        beaconServerInitProcessQueue();

    }else{
        if(!beacon_server.isRequestServer){
            beaconPrintf(COLOR_GREEN, "Perforce enabled: ");

            beaconPrintf(COLOR_RED, "NO!\n");

            if(beacon_server.isAutoServer){
                beacon_server.loadMaps = 1;
                beacon_server.isAutoServer = 1;
            }
        }

        processPriority = BELOW_NORMAL_PRIORITY_CLASS;
    }

    server_state.beaconProcessOnLoad = 1;

    beaconServerConnectToDBServer();

    SetPriorityClass(GetCurrentProcess(), processPriority);
    SetThreadPriority(GetCurrentThread(), threadPriority);

    _beginthreadex(NULL, 0, beaconServerWindowThread, NULL, 0, NULL);

    printf("Server IP: %s\n", makeIpStr(getHostLocalIp()));

    srand(time(NULL) + _getpid());

    estrPrintf(    &beacon_server.serverUID,
                "ip%s_pid%d_time%d_rand%d",
                makeIpStr(getHostLocalIp()),
                _getpid(),
                time(NULL),
                rand() % 1000);

    printf("ServerUID: %s\n", beacon_server.serverUID);

    // CRC the executable.

    beacon_server.exeFile.crc = beaconGetExeCRC(&beacon_server.exeFile.data, &beacon_server.exeFile.size);

    assert(beacon_server.exeFile.data);

    beaconServerInstall();

    printf("CRC of \"%s\" = 0x%8.8x\n", beaconGetExeFileName(), beacon_server.exeFile.crc);

    if(!beacon_server.isMasterServer){
        initBackgroundLoader();

        if(beacon_server.isAutoServer){
            // Get latest data.

            beaconServerGetLatestData();

            // Load map list.

            beaconServerLoadMapFileList(0);
        }
        else if(!beacon_server.isRequestServer){
            // Load local map list.

            beaconServerLoadMapFileList(1);
        }
    }

    // Init common stuff for client & server.

    beaconInitCommon(!beacon_server.isMasterServer);

    // Enable folder cache callbacks so that getting latest version at runtime will work properly.

    FolderCacheEnableCallbacks(1);

    if(    !beacon_server.isMasterServer &&
        !beacon_server.isRequestServer)
    {
        // Init stuff for loading world geometry.

        beaconPrintf(COLOR_GREEN, "Loading statebits: ");
        seqLoadStateBits();
        beaconPrintf(COLOR_GREEN, "Done!\n");

        beaconPrintf(COLOR_GREEN, "Loading tricks: ");
        trickLoad();
        beaconPrintf(COLOR_GREEN, "Done!\n");

        groupLoadLibs();
    }

    // Create the NetLinkList.

    netLinkListAlloc(&beacon_server.clients, 500, sizeof(BeaconServerClientData), beaconServerConnectCallback);

    if(    !beacon_server.isMasterServer &&
        !beaconIsProductionMode())
    {
        beaconProcessSetConsoleCtrlHandler(1);
    }

    // Initialize networking.

    if(!noNetStart){
        beaconServerInitNetwork();
    }

    // Disable beacon loading popup errors.

    beaconReaderDisablePopupErrors(1);

    beacon_server.curMapIndex = -1;

    printf(    "\n\n[ BEACON %sSERVER RUNNING %s]\n",
            beacon_server.isMasterServer ? "MASTER " :
                beacon_server.isRequestServer ? "REQUEST " :
                    "",
            beacon_server.isMasterServer ? "" : "?");

    beaconCurTimeString(1);
}

static U32 beaconServerGetLongestRequesterWait(void){
    U32 longest = 0;

    FOR_CLIENTS(i, client)
        if(    client->clientType == BCT_REQUESTER &&
            client->requester.startedRequestTime)
        {
            U32 cur = beaconTimeSince(client->requester.startedRequestTime);

            longest = MAX(cur, longest);
        }
    END_FOR

    return longest;
}

static void beaconServerUpdateTitle(FORMAT format, ...){
    const char* timeString = beaconCurTimeString(0);
    char        buffer[1000];
    char*        pos = buffer;
    S32         serverCount = 0;
    S32            requestServerCount = 0;
    S32         userInactiveCount = 0;
    S32         userActiveCount = 0;
    S32         sentryCount = 0;
    S32         requesterCount = 0;
    S32         noSentryCount = 0;
    S32         noClientCount = 0;
    S32         crashedCount = 0;
    S32            updatingCount = 0;
    char        countBuffer[1000] = "";
    char*        countPos = countBuffer;
    U32            longestWait;

    while(timeString[4] && (timeString[0] == '0' || timeString[0] == ':')){
        timeString++;
    }

    FOR_CLIENTS(i, client)
        if(    client->state == BCS_NEEDS_MORE_EXE_DATA ||
            client->state == BCS_RECEIVING_EXE_DATA)
        {
            updatingCount++;
        }
        
        switch(client->clientType){
            xcase BCT_SERVER:{
                if(client->server.isRequestServer){
                    requestServerCount++;
                }else{
                    serverCount++;
                }
            }

            xcase BCT_SENTRY:{
                sentryCount++;

                if(!client->sentryClients.count){
                    if(client->forcedInactive){
                        noClientCount++;
                    }
                }else{
                    S32 j;
                    for(j = 0; j < client->sentryClients.count; j++){
                        if(client->sentryClients.clients[j].crashText){
                            crashedCount++;
                        }
                    }
                }
            }

            xcase BCT_WORKER:{
                if(client->sentry && client->sentry->forcedInactive){
                    userInactiveCount++;
                }else{
                    userActiveCount++;
                }

                if(!client->sentry){
                    noSentryCount++;
                }
            }
        }
    END_FOR

    // Paused.

    if(beacon_server.paused){
        pos += sprintf(pos, "[PAUSED] ");
    }

    // Status ACK

    if(beacon_server.master_link.connected && !beacon_server.status.acked){
        pos += sprintf(pos, "[Status Not Acked!]");
    }

    // Server type.

    pos += sprintf(    pos, "%s-BeaconServer:",
                    beacon_server.isMasterServer ? "Master" :
                        beacon_server.isAutoServer ? "Auto" :
                            beacon_server.isRequestServer ? "Request" :
                                "Manual");

    // PID.

    pos += sprintf(pos, "%d", _getpid());

    // Time.

    pos += sprintf(pos, " (Time %s): [", timeString);

    // Master server client counts.

    if(beacon_server.isMasterServer){
        pos += sprintf(    pos,
                        "%dlr,%dss,%ds,%dr,%drs,%dpn,",
                        beaconServerGetLoadRequestCount(),
                        serverCount,
                        sentryCount,
                        beacon_server.clientList[BCT_REQUESTER].count,
                        requestServerCount,
                        beacon_server.processQueue.count);
    }

    // Inactive clients.

    pos += sprintf(pos, "%di,", userInactiveCount);

    // Active clients.

    pos += sprintf(pos, "%da]", userActiveCount);

    // Requester wait time.

    longestWait = beaconServerGetLongestRequesterWait();

    if(longestWait){
        pos += sprintf(pos, " (Longest wait: %d:%2.2d)", longestWait / 60, longestWait % 60);
    }

    // Crashes.

    if(crashedCount){
        countPos += sprintf(countPos,
                            "%s%d CRASHED!!!",
                            countBuffer[0] ? ", " : "",
                            crashedCount);
    }

    // Count of sentries that are active but have no clients.
    
    if(noClientCount){
        countPos += sprintf(countPos,
                            "%s%d uncliented",
                            countBuffer[0] ? ", " : "",
                            noClientCount);
    }
    
    // Count of clients that are updating.
    
    if(updatingCount){
        countPos += sprintf(countPos,
                            "%s%d updating",
                            countBuffer[0] ? ", " : "",
                            updatingCount);
    }

    if(countBuffer[0]){
        pos += sprintf(pos, " (%s)", countBuffer);
    }

    if(format){
        va_list argptr;

        pos += sprintf(pos, " (");
        va_start(argptr, format);
        pos += vsprintf_s(pos, ARRAY_SIZE_CHECKED(buffer) - (pos-buffer), format, argptr);
        va_end(argptr);
        pos += sprintf(pos, ")");
    }

    setConsoleTitle(buffer);
}

//static void beaconServerFindPartialProcess(void){
//    S32 i;
//
//    for(i = 0; i < beacon_server.mapCount; i++){
//        char buffer[1000];
//
//        sprintf(buffer, "%s/%s/beaconprocess.tmp", beaconServerTempDir, beacon_server.mapList[i]);
//
//        if(fileExists(buffer)){
//            beacon_server.curMapIndex = i;
//            estrPrintCharString(&beacon_server.curMapName, beacon_server.mapList[i]);
//            printf("RESUMING PROCESS: %s\n", beacon_server.curMapName);
//
//            return;
//        }
//    }
//}

static BeaconDiskSwapBlock* beaconGetLegalBlockWithLeastClients(void){
    BeaconDiskSwapBlock* block;
    BeaconDiskSwapBlock* leastClients = NULL;

    if(0){
        // temp!!!!

        static blockList[][2] = {
            {-4, -2},
        };
        static S32 curBlock = 0;

        if(curBlock < ARRAY_SIZE(blockList)){
            for(block = bp_blocks.list; block; block = block->nextSwapBlock){
                if(    block->x / BEACON_GENERATE_CHUNK_SIZE == blockList[curBlock][0] &&
                    block->z / BEACON_GENERATE_CHUNK_SIZE == blockList[curBlock][1])
                {
                    printf("returning block %d, %d\n", blockList[curBlock][0], blockList[curBlock][1]);
                    curBlock++;
                    return block;
                }
            }
        }

        return NULL;
    }

    for(block = bp_blocks.list; block; block = block->nextSwapBlock){
        if(block->legalCompressed.uncheckedCount > 0){
            if(!block->clients.count){
                return block;
            }
            else if(timerSeconds(timerCpuTicks() - block->clients.assignedTime) > 5.0){
                if(!leastClients || block->clients.count < leastClients->clients.count){
                    leastClients = block;
                }
            }
        }
    }

    return leastClients;
}

static void beaconServerSendLegalAreas(Packet* pak, BeaconDiskSwapBlock* block){
    BeaconLegalAreaCompressed* area;

    beaconCheckDuplicates(block);

    pktSendBits(pak, 1, block->foundCRC);

    if(block->foundCRC){
        pktSendBits(pak, 32, block->surfaceCRC);
    }

    pktSendBitsPack(pak, 16, block->legalCompressed.totalCount);

    for(area = block->legalCompressed.areasHead; area; area = area->next){
        pktSendBits(pak, 8, area->x);
        pktSendBits(pak, 8, area->z);
        pktSendBits(pak, 1, area->checked);

        if(area->isIndex){
            pktSendBits(pak, 1, 1);
            pktSendBitsPack(pak, 5, area->y_index);
        }else{
            pktSendBits(pak, 1, 0);
            pktSendF32(pak, area->y_coord);
        }

        pktSendBitsPack(pak, 1, area->areas.count);

        #if BEACONGEN_STORE_AREAS
        {
            S32 i;
            for(i = 0; i < area->areas.count; i++){
                pktSendF32(pak, area->areas.areas[i].y_min);
                pktSendF32(pak, area->areas.areas[i].y_max);

                #if BEACONGEN_CHECK_VERTS
                {
                    S32 j;

                    for(j = 0; j < 3; j++){
                        pktSendF32(pak, area->areas.areas[i].triVerts[j][0]);
                        pktSendF32(pak, area->areas.areas[i].triVerts[j][1]);
                        pktSendF32(pak, area->areas.areas[i].triVerts[j][2]);
                    }

                    pktSendString(pak, area->areas.areas[i].defName);
                }
                #endif
            }
        }
        #endif

        #if BEACONGEN_CHECK_VERTS
        {
            S32 i;

            for(i = 0; i < area->tris.count; i++){
                S32 j;
                pktSendBits(pak, 1, 1);
                pktSendString(pak, area->tris.tris[i].defName);
                pktSendF32(pak, area->tris.tris[i].y_min);
                pktSendF32(pak, area->tris.tris[i].y_max);
                for(j = 0; j < 3; j++){
                    pktSendF32(pak, area->tris.tris[i].verts[j][0]);
                    pktSendF32(pak, area->tris.tris[i].verts[j][1]);
                    pktSendF32(pak, area->tris.tris[i].verts[j][2]);
                }
            }

            pktSendBits(pak, 1, 0);
        }
        #endif

        #if BEACONGEN_STORE_AREA_CREATOR
            pktSendBitsPack(pak, 1, area->areas.cx);
            pktSendBitsPack(pak, 1, area->areas.cz);
            pktSendBits(pak, 32, area->areas.ip);
        #endif
    }
}

static void beaconAssignBlockToClient(BeaconServerClientData* client){
    BeaconDiskSwapBlock* block = beaconGetLegalBlockWithLeastClients();

    if(block){
        BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_PROCESS_LEGAL_AREAS);
            S32 grid_x = block->x / BEACON_GENERATE_CHUNK_SIZE;
            S32 grid_z = block->z / BEACON_GENERATE_CHUNK_SIZE;

            beaconClientPrintf(    client, 0,
                                "Assigning to block (%4d,%4d), %d legal areas, %d unchecked.\n",
                                grid_x,
                                grid_z,
                                block->legalCompressed.totalCount,
                                block->legalCompressed.uncheckedCount);

            beaconCheckDuplicates(block);

            pktSendBitsPack(pak, 1, grid_x);
            pktSendBitsPack(pak, 1, grid_z);
            beaconServerSendLegalAreas(pak, block);

        BEACON_SERVER_PACKET_SEND();

        if(!block->clients.count){
            block->clients.assignedTime = timerCpuTicks();
        }

        dynArrayAddp(&block->clients.clients, &block->clients.count, &block->clients.maxCount, client);

        client->assigned.block = block;

        beaconServerSetClientState(client, BCS_GENERATING);
    }
}

static void beaconSendBeaconsToClient(BeaconServerClientData* client){
    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_BEACON_LIST);

        S32 i;

        pktSendBitsPack(pak, 1, combatBeaconArray.size);

        for(i = 0; i < combatBeaconArray.size; i++){
            Beacon* b = combatBeaconArray.storage[i];

            pktSendBitsArray(pak, 8 * sizeof(Vec3), posPoint(b));

            pktSendBits(pak, 1, b->noGroundConnections);
        }

    BEACON_SERVER_PACKET_SEND();

    beaconServerSetClientState(client, BCS_READY_TO_CONNECT_BEACONS);
}

MP_DEFINE(BeaconConnectBeaconGroup);

BeaconConnectBeaconGroup* createBeaconConnectBeaconGroup(void){
    MP_CREATE(BeaconConnectBeaconGroup, 100);
    return MP_ALLOC(BeaconConnectBeaconGroup);
}

void destroyBeaconConnectBeaconGroup(BeaconConnectBeaconGroup* group){
    MP_FREE(BeaconConnectBeaconGroup, group);
}

static void beaconServerAddBeaconGroupTail(BeaconConnectBeaconGroup* group){
    if(!beacon_server.beaconConnect.groups.availableHead){
        beacon_server.beaconConnect.groups.availableHead = group;
    }else{
        beacon_server.beaconConnect.groups.availableTail->next = group;
    }

    group->next = NULL;
    beacon_server.beaconConnect.groups.availableTail = group;
    beacon_server.beaconConnect.groups.count++;

    verifyAvailableCount();
}

static S32 getClientStateCount(BeaconClientState state){
    S32 count = 0;

    FOR_CLIENTS(i, client)
        if(client->state == state){
            count++;
        }
    END_FOR

    return count;
}

static void beaconServerCreateBeaconGroups(S32 forceGroup){
    S32 maxGroupSize = 20;
    S32 groupSize = combatBeaconArray.size / maxGroupSize;
    S32 groupFound = 0;

    if(groupSize < 10){
        groupSize = 10;
    }
    else if(groupSize > maxGroupSize){
        groupSize = maxGroupSize;
    }

    while(beacon_server.beaconConnect.legalBeacons.groupedCount <= beacon_server.beaconConnect.legalBeacons.count - groupSize){
        BeaconConnectBeaconGroup* group = createBeaconConnectBeaconGroup();

        group->lo = beacon_server.beaconConnect.legalBeacons.groupedCount;
        group->hi = group->lo + groupSize - 1;
        beacon_server.beaconConnect.legalBeacons.groupedCount = group->hi + 1;

        beaconServerAddBeaconGroupTail(group);

        groupFound = 1;
    }

    if(    !groupFound &&
        forceGroup &&
        beacon_server.beaconConnect.legalBeacons.groupedCount < beacon_server.beaconConnect.legalBeacons.count)
    {
        S32 waitingCount = getClientStateCount(BCS_READY_TO_CONNECT_BEACONS);
        S32 remaining = beacon_server.beaconConnect.legalBeacons.count - beacon_server.beaconConnect.legalBeacons.groupedCount;

        groupSize = remaining / waitingCount;

        if(groupSize <= 0){
            groupSize = 1;
        }

        while(remaining){
            BeaconConnectBeaconGroup* group = createBeaconConnectBeaconGroup();

            group->lo = beacon_server.beaconConnect.legalBeacons.groupedCount;
            group->hi = group->lo + groupSize - 1;
            if(group->hi >= beacon_server.beaconConnect.legalBeacons.count){
                group->hi = beacon_server.beaconConnect.legalBeacons.count - 1;
            }
            beacon_server.beaconConnect.legalBeacons.groupedCount += group->hi - group->lo + 1;
            remaining -= group->hi - group->lo + 1;

            beaconServerAddBeaconGroupTail(group);
        }
    }
}

static void addClientToGroup(BeaconConnectBeaconGroup* group, BeaconServerClientData* client){
    if(!group->clients.count){
        group->clients.assignedTime = timerCpuTicks();
    }
    dynArrayAddp(&group->clients.clients, &group->clients.count, &group->clients.maxCount, client);
}

static BeaconConnectBeaconGroup* getUnfinishedGroupWithLeastClients(void){
    BeaconConnectBeaconGroup* leastClients = NULL;

    FOR_CLIENTS(i, client)
        if(client->assigned.group && timerSeconds(timerCpuTicks() - client->assigned.group->clients.assignedTime) > 5.0){
            if(!leastClients || client->assigned.group->clients.count < leastClients->clients.count){
                leastClients = client->assigned.group;
            }
        }
    END_FOR

    return leastClients;
}

static void beaconAssignBeaconGroupToClient(BeaconServerClientData* client){
    S32 count;
    S32 i;

    if(!beacon_server.beaconConnect.groups.availableHead){
        beaconServerCreateBeaconGroups(1);
    }

    assert(!client->assigned.group);

    if(beacon_server.beaconConnect.groups.availableHead){
        client->assigned.group = beacon_server.beaconConnect.groups.availableHead;
        beacon_server.beaconConnect.groups.availableHead = client->assigned.group->next;
        if(!beacon_server.beaconConnect.groups.availableHead){
            beacon_server.beaconConnect.groups.availableTail = NULL;
        }

        beacon_server.beaconConnect.assignedCount++;
    }else{
        client->assigned.group = getUnfinishedGroupWithLeastClients();
    }

    if(client->assigned.group){
        client->assigned.group->next = NULL;
        addClientToGroup(client->assigned.group, client);
    }

    verifyAvailableCount();

    if(!client->assigned.group){
        return;
    }

    beaconClientPrintf(    client,
                        COLOR_BLUE,
                        "Assigning %3d beacons [%6d-%6d]\n",
                        client->assigned.group->hi - client->assigned.group->lo + 1,
                        client->assigned.group->lo,
                        client->assigned.group->hi);

    if(0){
        S32 i;
        printf("    indices: ");
        for(i = client->assigned.group->lo; i <= client->assigned.group->hi; i++){
            S32 index = beacon_server.beaconConnect.legalBeacons.indexes[i];
            printf("%d, ", index);
        }
        printf("\n");
    }

    // Send the packet to the client.

    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_CONNECT_BEACONS);

        count = client->assigned.group->hi - client->assigned.group->lo + 1;

        pktSendBitsPack(pak, 1, count);

        for(i = client->assigned.group->lo; i <= client->assigned.group->hi; i++){
            S32 index = beacon_server.beaconConnect.legalBeacons.indexes[i];
            pktSendBitsPack(pak, 1, index);
        }

    BEACON_SERVER_PACKET_SEND();

    beaconServerSetClientState(client, BCS_CONNECTING_BEACONS);
}

void beaconServerDisconnectClient(BeaconServerClientData* client, const char* reason){
    if(    client &&
        client->link &&
        !client->link->disconnected &&
        client->link->connected &&
        !client->disconnected)
    {
        client->disconnected = 1;

        if(reason){
            beaconClientPrintf(client, COLOR_RED, "Disconnecting: %s\n", reason);
        }

        netSendDisconnect(client->link, 0);
    }
}

static void beaconServerProcessClientPaused(BeaconServerClientData* client,
                                            S32 index,
                                            BeaconServerProcessClientsData* pcd)
{
    switch(client->state){
        xcase BCS_ERROR_NONLOCAL_IP:{
            if(!beacon_server.localOnly){
                beaconServerDisconnectClient(client, "Local IP is no longer required.");
            }
        }
    }
}

static S32 beaconServerNeedsSomeClients(BeaconServerClientData* subServer){
    return    estrLength(&subServer->server.mapName) &&
            subServer->server.sendClientsToMe;
}

static S32 beaconServerRequestServerIsAvailable(BeaconServerClientData* requestServer){
    return    requestServer->server.isRequestServer &&
            !requestServer->requestServer.processNode;
}

static BeaconServerClientData* beaconServerGetAvailableRequestServer(void){
    FOR_CLIENTS(i, client)
        if(beaconServerRequestServerIsAvailable(client)){
            return client;
        }
    END_FOR

    return NULL;
}

static void beaconServerPingClient(BeaconServerClientData* client, S32 index, void* userData){
    if(    client->exeCRC == beacon_server.exeFile.crc &&
        beaconTimeSince(client->sentPingTime) > 5)
    {
        client->sentPingTime = beaconGetCurTime();

        BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_PING);
        BEACON_SERVER_PACKET_SEND();
    }
}

static void beaconServerProcessClient(    BeaconServerClientData* client,
                                        S32 index,
                                        BeaconServerProcessClientsData* pcd)
{
    beaconServerProcessClientPaused(client, index, pcd);

    beaconServerPingClient(client, index, NULL);

    switch(client->state){
        xcase BCS_NEEDS_MORE_EXE_DATA:{
            if(pcd->sentExeDataCount < 5){
                pcd->sentExeDataCount++;

                beaconServerSendNextExeChunk(client);
            }
        }

        xcase BCS_RECEIVING_EXE_DATA:{
            if(beaconTimeSince(client->exeData.lastCommTime) < 3){
                pcd->sentExeDataCount++;
            }
        }

        xcase BCS_READY_TO_WORK:{
            pktSetDebugInfo();

            client->mapData.sentByteCount = 0;

            beaconServerSendMapDataToWorker(client);
        }

        xcase BCS_READY_TO_GENERATE:{
            switch(beacon_server.state){
                xcase BSS_GENERATING:{
                    beaconAssignBlockToClient(client);
                }

                xcase BSS_CONNECT_BEACONS:{
                    beaconSendBeaconsToClient(client);
                }
            }
        }

        xcase BCS_READY_TO_CONNECT_BEACONS:{
            if(beacon_server.state == BSS_CONNECT_BEACONS){
                beaconAssignBeaconGroupToClient(client);
            }
        }

        xcase BCS_RECEIVING_MAP_DATA:{
            if(beaconTimeSince(client->mapData.lastCommTime) < 3){
                pcd->sentMapDataCount++;
            }
        }

        xcase BCS_NEEDS_MORE_MAP_DATA:{
            if(pcd->sentMapDataCount < 10){
                pcd->sentMapDataCount++;
                
                beaconServerSendMapDataToWorker(client);
            }
        }
    }
}

static void beaconServerDisconnectClientCallback(BeaconServerClientData* client, S32 index, char* reason){
    beaconServerDisconnectClient(client, reason);
}

static void beaconServerDisconnectErrorNonLocalIP(BeaconServerClientData* client, S32 index, void* userData){
    if(client->state == BCS_ERROR_NONLOCAL_IP){
        beaconServerDisconnectClient(client, "Local IP is no longer required.");
    }
}

static S32 clientStateIsVisible(BeaconServerClientData* client){
    S32 i;

    if(client->clientType != BCT_SENTRY || !client->sentryClients.count){
        return 1;
    }

    for(i = 0; i < client->sentryClients.count; i++){
        BeaconServerSentryClientData* sentryClient = client->sentryClients.clients + i;

        if(sentryClient->crashText || !sentryClient->client){
            return 1;
        }
    }

    return 0;
}

static const char* beaconGetServerStateName(BeaconServerState state){
    switch(state){
        case BSS_NOT_STARTED:
            return "NOT_STARTED";
        case BSS_GENERATING:
            return "GENERATING";
        case BSS_CONNECT_BEACONS:
            return "CONNECT_BEACONS";
        case BSS_WRITE_FILE:
            return "WRITE_FILE";
        case BSS_SEND_BEACON_FILE:
            return "SEND_BEACON_FILE";
        case BSS_DONE:
            return "DONE";
        default:
            return "UNKNOWN";
    }
}

static void printClientState(BeaconServerClientData* client, S32 index, void* userData){
    void* ptrs[2] = {client, NULL};
    char buffer[1000];
    U32 elapsedTime;
    S32 bgcolor = 0;
    char* crashText = NULL;
    S32 isCrashed = 0;
    S32 crashedCount = 0;
    S32 notConnectedCount = 0;
    S32 userInactiveCount = 0;
    S32 i;

    if(!clientStateIsVisible(client)){
        return;
    }

    for(i = 0; i < client->sentryClients.count; i++){
        BeaconServerSentryClientData* sentryClient = client->sentryClients.clients + i;

        if(sentryClient->crashText){
            crashedCount++;
        }

        if(!sentryClient->client){
            notConnectedCount++;
        }

        if(client->forcedInactive){
            userInactiveCount++;
        }
    }

    printf("\n");

    printf("?s", index == beacon_server.selectedClient ? ">" : " ");

    if(sentryHasClient(client->sentry, client, 0, client->uid, &i)){
        BeaconServerSentryClientData* sentryClient = client->sentry->sentryClients.clients + i;

        if(sentryClient->crashText){
            crashText = sentryClient->crashText;
            bgcolor = COLOR_RED;
        }
    }

    if(index == beacon_server.selectedClient){
        bgcolor = COLOR_BLUE;
    }

    switch(client->clientType){
        xcase BCT_SERVER:
            consoleSetColor(COLOR_BRIGHT|COLOR_GREEN, bgcolor);
        xcase BCT_SENTRY:
            consoleSetColor(COLOR_BRIGHT|COLOR_GREEN|COLOR_BLUE, bgcolor);
        xcase BCT_WORKER:
            if(client->sentry){
                consoleSetColor(COLOR_BRIGHT|COLOR_GREEN|COLOR_BLUE, bgcolor);
            }else{
                consoleSetColor(COLOR_YELLOW, bgcolor);
            }
        xcase BCT_REQUESTER:
            consoleSetColor(COLOR_BRIGHT|COLOR_YELLOW, bgcolor);
    }

    printf("%-17s", getClientIPStr(client));

    consoleSetColor(getClientStateColor(client->state), bgcolor);

    printf("%-25s", getClientStateName(client->state));

    consoleSetDefaultColor();

    switch(client->clientType){
        xcase BCT_SERVER:
            printf("Protocol: %d", client->server.protocolVersion);
            beaconPrintf(COLOR_GREEN, " | ");
            printf("Clients: %d", client->server.clientCount);
            beaconPrintf(COLOR_GREEN, " | ");
            printf("State: %s", beaconGetServerStateName(client->server.state));

        xdefault:
            consoleSetColor(COLOR_BRIGHT|COLOR_RED|COLOR_GREEN|COLOR_BLUE, bgcolor);

            buffer[0] = 0;

            if(crashText){
                sprintf(buffer, "CRASHED!!!");
            }
            else if(client->assigned.block){
                sprintf(buffer,
                        "(%d,%d)",
                        client->assigned.block->x / BEACON_GENERATE_CHUNK_SIZE,
                        client->assigned.block->z / BEACON_GENERATE_CHUNK_SIZE);
            }
            else if(client->assigned.group){
                sprintf(buffer,
                        "(%d-%d)",
                        client->assigned.group->lo,
                        client->assigned.group->hi);
            }
            else if(client->sentry && !client->sentry->forcedInactive){
                strcpy(buffer, "User Active");
            }

            printf("%-18s", buffer[0] ? buffer : "");

            consoleSetColor(COLOR_BRIGHT|COLOR_RED|COLOR_GREEN|COLOR_BLUE, 0);

            if(client->completed.blockCount){
                printf("%-12d", client->completed.blockCount);
            }else{
                printf("%12s", "");
            }

            if(client->completed.beaconCount){
                printf("%-12d", client->completed.beaconCount);
            }else{
                printf("%12s", "");
            }
    }

    // Next line.

    consoleSetDefaultColor();

    printf("\n?s", index == beacon_server.selectedClient ? ">" : " ");

    consoleSetColor(COLOR_GREEN|COLOR_BLUE, bgcolor);
    elapsedTime = beaconGetCurTime() - client->connectTime;
    sprintf(buffer, "[%d:%2.2d:%2.2d]", elapsedTime / 3600, (elapsedTime / 60) % 60, elapsedTime % 60);
    sprintf(buffer + strlen(buffer), " %s/%s", client->computerName, client->userName);
    printf("%-42s", buffer);

    consoleSetColor(COLOR_BRIGHT|COLOR_BLUE|COLOR_GREEN, bgcolor);
    elapsedTime = beaconGetCurTime() - client->stateBeginTime;
    sprintf(buffer, "%d:%2.2d:%2.2d", elapsedTime / 3600, (elapsedTime / 60) % 60, elapsedTime % 60);
    printf("%-10s", buffer);

    consoleSetDefaultColor();

    printf("%d pkts.  ", qGetSize(client->link->sendQueue2));

    switch(client->clientType){
        xcase BCT_SERVER:{
            printf("Map: %s", estrLength(&client->server.mapName) ? getFileName(client->server.mapName) : "NONE");
        }

        xdefault:{
            if(crashedCount){
                beaconPrintf(COLOR_RED, "%d CRASHED!  ", crashedCount);
            }

            if(notConnectedCount){
                beaconPrintf(COLOR_YELLOW, "%d Not Connected!  ", notConnectedCount);
            }

            if(userInactiveCount){
                beaconPrintf(COLOR_YELLOW, "%d Inactive!  ", userInactiveCount);
            }
        }
    }

    consoleSetDefaultColor();

    printf("\n");

    switch(client->clientType){
        xcase BCT_SERVER:{
            if(    client->server.isRequestServer &&
                client->requestServer.processNode)
            {
                printf("?s", index == beacon_server.selectedClient ? ">" : " ");

                beaconPrintfDim(COLOR_GREEN,
                                "Assigned map: %s\n",
                                client->requestServer.processNode->uniqueFileName);
            }
        }
    }
}

static void beaconServerMoveSelection(S32 delta){
    NetLink** links = (NetLink**)beacon_server.clients.links->storage;
    S32 size = beacon_server.clients.links->size;
    BeaconServerClientData* client;

    if(!size){
        return;
    }

    delta = delta > 0 ? 1 : delta < 0 ? -1 : 0;

    if(delta){
        while(1){
            beacon_server.selectedClient += delta;

            if(beacon_server.selectedClient < 0){
                beacon_server.selectedClient = 0;
                break;
            }
            else if(beacon_server.selectedClient >= size){
                beacon_server.selectedClient = size - 1;
                break;
            }
            else{
                client = links[beacon_server.selectedClient]->userData;

                if(clientStateIsVisible(client)){
                    break;
                }
            }
        }
    }else{
        delta = -1;
    }

    delta = -delta;

    if(beacon_server.selectedClient < 0){
        beacon_server.selectedClient = 0;
    }
    else if(beacon_server.selectedClient >= size){
        beacon_server.selectedClient = size - 1;
    }

    while(1){
        client = links[beacon_server.selectedClient]->userData;

        if(clientStateIsVisible(client)){
            break;
        }

        beacon_server.selectedClient += delta;

        if(beacon_server.selectedClient < 0){
            beacon_server.selectedClient = 0;
            break;
        }
        else if(beacon_server.selectedClient >= size){
            beacon_server.selectedClient = size - 1;
            break;
        }
    }

    delta = -delta;

    while(1){
        client = links[beacon_server.selectedClient]->userData;

        if(clientStateIsVisible(client)){
            break;
        }

        beacon_server.selectedClient += delta;

        if(beacon_server.selectedClient < 0){
            beacon_server.selectedClient = 0;
            break;
        }
        else if(beacon_server.selectedClient >= size){
            beacon_server.selectedClient = size - 1;
            break;
        }
    }
}

static void beaconServerPrintClientStates(void){
    consoleSetDefaultColor();

    printf(    "\n"
            "       ?n"
            "       ?");

    beaconPrintf(COLOR_RED|COLOR_GREEN|COLOR_BLUE, "Client Info");

    printf(    " ?n"
            "\n"
            "?");

    beaconPrintf(COLOR_GREEN,
                "%-17s%-25s%-18s%-12s%-12s\n",
                "IP",
                "State",
                "StateInfo",
                "Blocks",
                "Beacons");

    if(beacon_server.clients.links->size){
        beaconServerMoveSelection(0);
        beaconServerForEachClient(printClientState, NULL);
    }else{
        printf(    "\n"
                "?No Clients Connected!\n");
    }

    printf(    "\n"
            "\n\n");
}

static S32 beaconServerHasMapLoaded(void){
    if(    estrLength(&beacon_server.curMapName) &&
        (    !beacon_server.isRequestServer ||
            beaconMapDataPacketIsFullyReceived(beacon_server.mapData)))
    {
        return 1;
    }

    return 0;
}

static S32 beaconGetSearchString(char* buffer, S32 maxLength){
    beaconPrintf(    COLOR_GREEN|COLOR_BLUE,
                    "Current (Q: %d/%d, L: %d/%d): %s\n\n",
                    beacon_server.nextQueueIndex,
                    eaSize(&beacon_server.queueList),
                    beacon_server.curMapIndex,
                    beacon_server.mapCount,
                    beaconServerHasMapLoaded() ? beacon_server.curMapName : "NONE!");

    beaconPrintf(COLOR_GREEN, "Enter search: ");

    if(!beaconEnterString(buffer, maxLength)){
        beaconPrintf(COLOR_RED, "CANCELED!\n");
        return 0;
    }

    if(!buffer[0]){
        beaconPrintf(COLOR_YELLOW, "No Search String!");
    }

    printf("\n");

    return 1;
}

static void beaconServerPrintQueueList(void){
    S32 i;
    char search[1000];

    if(!eaSize(&beacon_server.queueList)){
        return;
    }

    if(!beaconGetSearchString(search, ARRAY_SIZE(search) - 1)){
        return;
    }

    printf(    "\n"
            "---------------------------------------------------------------------------------------\n"
            "Queue list:\n");

    for(i = 0; i < eaSize(&beacon_server.queueList); i++){
        char* name = beacon_server.queueList[i];
        char* nameAtMaps = strstri(name, "maps/");

        if(search[0] && !strstri(name, search)){
            continue;
        }

        if(    i == beacon_server.nextQueueIndex - 1 &&
            beaconServerHasMapLoaded() &&
            !stricmp(name, beacon_server.curMapName))
        {
            consoleSetColor(COLOR_BRIGHT|COLOR_GREEN, 0);
        }

        printf("  %4d: %s\n", i, nameAtMaps ? nameAtMaps : name);

        consoleSetDefaultColor();
    }

    printf(    "---------------------------------------------------------------------------------------\n"
            "\n"
            "\n");
}

static void beaconServerPrintMapList(void){
    S32 i;
    char search[1000];

    if(!beacon_server.mapCount){
        return;
    }

    if(!beaconGetSearchString(search, ARRAY_SIZE(search) - 1)){
        return;
    }

    printf(    "\n"
            "---------------------------------------------------------------------------------------\n"
            "Map list:\n");

    for(i = 0; i < beacon_server.mapCount; i++){
        char* name = beacon_server.mapList[i];
        char* nameAtMaps = strstri(name, "maps/");

        if(search[0] && !strstri(name, search)){
            continue;
        }

        if(    beaconServerHasMapLoaded() &&
            !stricmp(name, beacon_server.curMapName))
        {
            consoleSetColor(COLOR_BRIGHT|COLOR_GREEN, 0);
        }

        printf("  %4d: %s\n", i, nameAtMaps ? nameAtMaps : name);

        consoleSetDefaultColor();
    }

    printf(    "---------------------------------------------------------------------------------------\n"
            "\n"
            "\n");
}

static void displayBlockInfo(void){
    S32 x;
    S32 z;

    if(!beacon_server.curMapName){
        return;
    }

    beaconConsolePrintf(0, "\nMap: %s\n\n", beacon_server.curMapName);

    for(z = bp_blocks.grid_max_xyz[2]; z >= bp_blocks.grid_min_xyz[2]; z--){
        beaconConsolePrintf(z % 5 ? COLOR_RED|COLOR_GREEN|COLOR_BLUE : COLOR_BRIGHT|COLOR_GREEN, "%4d: ", z);

        for(x = bp_blocks.grid_min_xyz[0]; x <= bp_blocks.grid_max_xyz[0]; x++){
            BeaconDiskSwapBlock* block = beaconGetDiskSwapBlockByGrid(x, z);

            if(block){
                if(!block->geoRefs.count){
                    beaconConsolePrintf(COLOR_RED, "!");
                }
                else if(block->clients.count){
                    beaconConsolePrintf(COLOR_GREEN|COLOR_BLUE, block->clients.count < 16 ? "%x" : "X", block->clients.count);
                }
                else if(block->legalCompressed.uncheckedCount){
                    beaconConsolePrintf(COLOR_GREEN, "?");
                }
                else if(block->legalCompressed.totalCount){
                    beaconConsolePrintf(COLOR_GREEN|COLOR_BLUE, "?");
                }
                else{
                    beaconConsolePrintfDim(COLOR_GREEN, "?");
                }
            }else{
                beaconConsolePrintfDim(COLOR_RED, "?");
            }
        }

        beaconConsolePrintf(0, "\n");
    }

    beaconConsolePrintf(0, "\n      ");

    for(x = bp_blocks.grid_min_xyz[0]; x < (bp_blocks.grid_min_xyz[0] / 5) * 5; x++){
        beaconConsolePrintf(COLOR_WHITE, ".");
    }

    for(x = (bp_blocks.grid_min_xyz[0] / 5) * 5; x <= bp_blocks.grid_max_xyz[0]; x += 10){
        char buffer[100];
        S32 i;

        sprintf(buffer, "%d", x);

        beaconConsolePrintf(COLOR_GREEN, "%s", buffer);

        for(i = (S32)strlen(buffer); i % 10 && x + i <= bp_blocks.grid_max_xyz[0]; i++){
            beaconConsolePrintfDim(COLOR_RED|COLOR_GREEN|COLOR_BLUE, i % 5 ? "." : "|");
        }
    }

    beaconConsolePrintf(0, "\n\n");
}

static void beaconInsertLegalBeacons(void){
    S32 i;
    S32 legalBlockCount = 0;

    if(!beacon_server.isRequestServer){
        printf("Placing %d starting positions: ", combatBeaconArray.size);
    }

    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* b = combatBeaconArray.storage[i];

        if(b->isValidStartingPoint){
            S32 x = posX(b);
            S32 z = posZ(b);
            BeaconDiskSwapBlock* block = beaconGetDiskSwapBlock(x, z, 0);
            BeaconLegalAreaCompressed* area;

            if(!block){
                printf("WARNING: Found legal beacon in empty block at (%1.1f, %1.1f, %1.1f).\n", posParamsXYZ(b));
                continue;
            }

            if(!block->legalCompressed.totalCount){
                legalBlockCount++;
            }

            for(area = block->legalCompressed.areasHead; area; area = area->next){
                if(    !area->isIndex &&
                    area->x == x - block->x &&
                    area->z == z - block->z &&
                    area->y_coord == posY(b))
                {
                    break;
                }
            }

            if(area){
                continue;
            }

            beaconVerifyUncheckedCount(block);

            area = beaconAddLegalAreaCompressed(block);

            area->isIndex = 0;
            area->x = x - block->x;
            area->z = z - block->z;
            area->y_coord = posY(b);
            area->checked = 0;

            block->legalCompressed.uncheckedCount++;

            beaconVerifyUncheckedCount(block);

            beaconCheckDuplicates(block);
        }
    }

    if(!beacon_server.isRequestServer){
        printf("Done!  Used %d blocks.\n", legalBlockCount);
    }
}

static void beaconServerAddAllMadeBeacons(void){
    BeaconDiskSwapBlock* block;
    S32 count = 0;
    S32 i;

    //beaconPrintDebugInfo();

    if(!beacon_server.isRequestServer){
        printf("Adding beacons: ");
    }

    for(block = bp_blocks.list; block; block = block->nextSwapBlock){
        if(block->generatedBeacons.count){
            if(!beacon_server.isRequestServer){
                printf("%d, ", block->generatedBeacons.count);
            }

            count += block->generatedBeacons.count;

            for(i = 0; i < block->generatedBeacons.count; i++){
                BeaconGeneratedBeacon* madeBeacon = block->generatedBeacons.beacons + i;
                Beacon* beacon = addCombatBeacon(madeBeacon->pos, 1, 1, 1);

                if(beacon){
                    beacon->noGroundConnections = madeBeacon->noGroundConnections;
                }
            }
        }
    }

    beacon_server.beaconConnect.legalBeacons.count = 0;

    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* b = combatBeaconArray.storage[i];

        b->userInt = i;

        if(b->isValidStartingPoint){
            b->wasReachedFromValid = 1;

            dynArrayAddStruct(    &beacon_server.beaconConnect.legalBeacons.indexes,
                                &beacon_server.beaconConnect.legalBeacons.count,
                                &beacon_server.beaconConnect.legalBeacons.maxCount);

            beacon_server.beaconConnect.legalBeacons.indexes[beacon_server.beaconConnect.legalBeacons.count - 1] = i;
        }
    }

    beacon_server.beaconConnect.legalBeacons.groupedCount = 0;

    if(!beacon_server.isRequestServer){
        printf("Done!\nAdded %d beacons.\n", count);
    }

    beacon_server.beaconConnect.assignedCount = 0;
    beacon_server.beaconConnect.finishedCount = 0;
    beacon_server.beaconConnect.groups.count = 0;

    while(beacon_server.beaconConnect.groups.availableHead){
        BeaconConnectBeaconGroup* next = beacon_server.beaconConnect.groups.availableHead->next;

        SAFE_FREE(beacon_server.beaconConnect.groups.availableHead->clients.clients);
        destroyBeaconConnectBeaconGroup(beacon_server.beaconConnect.groups.availableHead);
        beacon_server.beaconConnect.groups.availableHead = next;
    }

    beacon_server.beaconConnect.groups.availableTail = NULL;

    while(beacon_server.beaconConnect.groups.finished){
        BeaconConnectBeaconGroup* next = beacon_server.beaconConnect.groups.finished->next;

        SAFE_FREE(beacon_server.beaconConnect.groups.finished->clients.clients);
        destroyBeaconConnectBeaconGroup(beacon_server.beaconConnect.groups.finished);
        beacon_server.beaconConnect.groups.finished = next;
    }

    beaconServerCreateBeaconGroups(0);

    //beaconPrintDebugInfo();
}

static void sendKillToSentry(BeaconServerClientData* sentry, S32 justCrashed){
    if(sentry->clientType == BCT_SENTRY){
        BEACON_SERVER_PACKET_CREATE_BASE(pak, sentry, BMSG_S2CT_KILL_PROCESSES);

            pktSendBits(pak, 1, justCrashed ? 1 : 0);

        BEACON_SERVER_PACKET_SEND();
    }
}

UINT_PTR CALLBACK getFileNameDialogHook(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam){
    switch(uiMsg){
        xcase WM_SHOWWINDOW:{
            BringWindowToTop(beaconGetConsoleWindow());
            BringWindowToTop(hdlg);
        }
    }

    return FALSE;
}

S32 getFileNameDialog(char* fileMask, char* fileNameParam, char*** multiSelectArray){
    char            tempDir[2000];
    char            fileName[20000];
    OPENFILENAMEA    theFileInfo;
    S32                ret;

    _getcwd(tempDir, 2000);

    strcpy(fileName, fileNameParam);
    
    backSlashes(fileName);

    ZeroStruct(&theFileInfo);
    theFileInfo.lStructSize = sizeof(OPENFILENAME);
    theFileInfo.hwndOwner = beaconGetConsoleWindow();
    theFileInfo.lpstrFilter = fileMask;
    theFileInfo.lpstrFile = fileName;
    theFileInfo.nMaxFile = ARRAY_SIZE(fileName) - 1;
    theFileInfo.lpstrTitle = "Choose map files to beaconize:";
    theFileInfo.Flags = OFN_EXPLORER |
                        OFN_LONGNAMES |
                        OFN_OVERWRITEPROMPT |
                        OFN_ENABLEHOOK |
                        OFN_ENABLESIZING |
                        OFN_HIDEREADONLY |
                        (multiSelectArray ? OFN_ALLOWMULTISELECT : 0)
                        // | OFN_PATHMUSTEXIST
                        ;
    theFileInfo.lpstrDefExt = NULL;
    theFileInfo.lpfnHook = getFileNameDialogHook;

    ret = GetOpenFileNameA(&theFileInfo);

    if(!ret && (GetLastError() || CommDlgExtendedError())){
        printf("GetOpenFileName Error: %d, %d (", GetLastError(), CommDlgExtendedError());

        switch(CommDlgExtendedError()){
            #define CASE(x) xcase x: printf(#x)
            CASE(CDERR_DIALOGFAILURE);
            CASE(CDERR_FINDRESFAILURE);
            CASE(CDERR_NOHINSTANCE);
            CASE(CDERR_INITIALIZATION);
            CASE(CDERR_NOHOOK);
            CASE(CDERR_LOCKRESFAILURE);
            CASE(CDERR_NOTEMPLATE);
            CASE(CDERR_LOADRESFAILURE);
            CASE(CDERR_STRUCTSIZE);
            CASE(CDERR_LOADSTRFAILURE);
            CASE(FNERR_BUFFERTOOSMALL);
            CASE(CDERR_MEMALLOCFAILURE);
            CASE(FNERR_INVALIDFILENAME);
            CASE(CDERR_MEMLOCKFAILURE);
            CASE(FNERR_SUBCLASSFAILURE);
            #undef CASE
            xdefault:printf("Unknown");
        }

        printf(")\n");
    }

    if(ret){
        if(multiSelectArray){
            char* path = fileName;
            char* curFile = fileName + theFileInfo.nFileOffset;

            forwardSlashes(path);

            eaClearEx(multiSelectArray, NULL);

            if(theFileInfo.nFileOffset > strlen(path)){
                while(*curFile){
                    char buffer[1000];
                    sprintf(buffer, "%s/%s", path, curFile);
                    //printf("adding: %s\n", buffer);
                    eaPush(multiSelectArray, strdup(buffer));

                    curFile += strlen(curFile) + 1;
                }
            }else{
                //printf("adding: %s\n", path);
                eaPush(multiSelectArray, strdup(path));
            }
        }else{
            strcpy(fileNameParam, fileName);
        }
    }

    _chdir(tempDir);

    return ret ? 1 : 0;
}

static void beaconServerQueueMapListFile(void){
    char buffer[1000];
    
    STR_COMBINE_SS(buffer, beaconServerGetDataPath(), "\\maps\\enter a map list filename");

    if(getFileNameDialog("*.txt", buffer, NULL)){
        beaconServerLoadMapListFile(buffer, 1);
    }
}

THREADSAFE_STATIC S32        strdupaf_storedCount;
THREADSAFE_STATIC char*        strdupaf_buffer;

void strdupaf_setCount(FORMAT format, ...){
    va_list argptr;

    va_start(argptr, format);
    strdupaf_storedCount = 1 + vsnprintf_s(NULL, 0, 0, format, argptr);
    va_end(argptr);
}

S32 strdupaf_getCount()
{
    return strdupaf_storedCount;
}

strdupaf_setBuffer(char* buffer)
{
    strdupaf_buffer = buffer;
    THREADSAFE_STATIC_MARK(strdupaf_buffer);
}

char* strdupaf_copy(FORMAT format, ...)
{
    va_list argptr;
    int count;

    va_start(argptr, format);
    count = 1 + vsprintf_s(strdupaf_buffer, strdupaf_storedCount, format, argptr);
    assert(count == strdupaf_storedCount);
    va_end(argptr);

    return strdupaf_buffer;
}

#define strdupaf(p) (strdupaf_setCount p,strdupaf_setBuffer(_alloca(strdupaf_getCount())),strdupaf_copy p);

static S32 beaconServerQueueMapFile(char* newMapFile){
    char validPrefix[1000];

    sprintf(validPrefix, "%s/maps/", beaconServerGetDataPath());

    if(strnicmp(newMapFile, validPrefix, strlen(validPrefix))){
        beaconPrintf(COLOR_RED, "Invalid map directory: %s\n", newMapFile);
    }
    else if(!fileExists(newMapFile)){
        beaconPrintf(COLOR_YELLOW, "Map file doesn't exist: %s\n", newMapFile);
    }
    else if(fileExistsInList(beacon_server.queueList, newMapFile, NULL)){
        beaconPrintf(COLOR_YELLOW, "Map already in queue: %s\n", newMapFile);
    }
    else{
        S32 index;

        if(fileExistsInList(beacon_server.mapList, newMapFile, &index)){
            beaconPrintf(    COLOR_GREEN,
                            "Queueing map %d/%d in list: %s\n",
                            index + 1,
                            beacon_server.mapCount,
                            strstr(newMapFile, "maps/"));
        }else{
            beaconPrintf(    COLOR_YELLOW,
                            "Queueing map not in list: %s\n",
                            strstr(newMapFile, "maps/"));
        }

        eaPush(&beacon_server.queueList, newMapFile);

        return 1;
    }

    return 0;
}

static void beaconServerQueueMapFiles(void){
    char** newMaps = NULL;
    char buffer[1000];
    
    STR_COMBINE_SS(buffer, beaconServerGetDataPath(), "\\maps\\enter a map name");

    if(estrLength(&beacon_server.curMapName)){
        strcpy(buffer, beacon_server.curMapName);
        backSlashes(buffer);
        //printf("opening %s\n", buffer);
    }

    if(getFileNameDialog("*", buffer, &newMaps)){
        S32 i;

        for(i = 0; i < eaSize(&newMaps); i++){
            if(beaconServerQueueMapFile(newMaps[i])){
                newMaps[i] = NULL;
            }
        }
    }

    eaDestroyEx(&newMaps, NULL);
}

static void beaconServerQueueMainListMaps(void){
    char search[1000];
    char confirm[4];
    S32 i;

    if(!beacon_server.mapCount){
        beaconPrintf(COLOR_YELLOW, "No maps in main list.\n\n");
        return;
    }

    beaconPrintf(COLOR_GREEN, "Enter search string: ");

    if(!beaconEnterString(search, ARRAY_SIZE(search) - 1) || !search[0]){
        beaconPrintf(COLOR_RED, "Canceled!\n");
        return;
    }

    printf("\n");

    for(i = 0; i < beacon_server.mapCount; i++){
        if(strstri(beacon_server.mapList[i], search)){
            printf("%6d: %s\n", i, beacon_server.mapList[i]);
        }
    }

    beaconPrintf(COLOR_GREEN, "\nQueue these maps? (type 'yes'): ");

    if(!beaconEnterString(confirm, 3) || stricmp(confirm, "yes")){
        beaconPrintf(COLOR_RED, "Canceled!\n");
        return;
    }

    printf("\n");

    for(i = 0; i < beacon_server.mapCount; i++){
        if(strstri(beacon_server.mapList[i], search)){
            beaconServerQueueMapFile(beacon_server.mapList[i]);
        }
    }

    printf("Done!\n\n");
}

static void beaconServerMapListMenu(void){
    S32 printMenu = 1;
    char confirm[4];

    // Load the file list.

    beaconServerUpdateTitle("Map List Menu");

    while(1){
        S32 done = 0;

        if(printMenu){
            beaconPrintf(COLOR_GREEN,
                        "-------------------------------------------------------------------------------------------------\n"
                        "Map list menu:\n"
                        "\n"
                        "\tESC : Exit this menu\n"
                        "\t1   : Reload map list from spec files and \"%s/server/maps/beaconprocess/maps*.txt\"\n"
                        "\t2   : Open map list directory\n"
                        "\t3   : Clear the map list\n"
                        "\t4   : View the map list (%d maps)\n"
                        "\t5   : View the queue list (%d maps)\n"
                        "\t6   : Add maps to the queue\n"
                        "\t7   : Add a map list file to the queue\n"
                        "\t8   : Restart the queue\n"
                        "\t9   : Add some maps from the main list to the queue\n"
                        "\t0   : Clear the queue\n"
                        "\n"
                        "Enter selection: ",
                        beaconServerGetDataPath(),
                        beacon_server.mapCount,
                        eaSize(&beacon_server.queueList));
        }

        printMenu = 1;

        switch(tolower(_getch())){
            xcase 0:
            case 224:{
                _getch();
                done = 0;
                printMenu = 0;
            }

            xcase 27:{
                printf("Exiting map list menu\n\n");
                done = 1;
            }

            xcase '1':{
                printf("Loading map list\n\n");
                beaconServerLoadMapFileList(0);
            }

            xcase '2':{
                char buffer[1000];
                STR_COMBINE_SS(buffer, beaconServerGetDataPath(), "\\server\\maps\\beaconprocess\\");
                printf("Opening map list directory\n\n");
                ShellExecuteA(NULL, "explore", buffer, "", "", SW_SHOW);
            }

            xcase '3':{
                printf("Clearing map list\n\n");
                printf("Type 'yes': ");
                if(beaconEnterString(confirm, 3) && !strcmp(confirm, "yes")){
                    printf("\nClearing the map list!");
                    eaClearEx(&beacon_server.mapList, NULL);
                    beacon_server.mapCount = 0;
                    beacon_server.curMapIndex = -1;
                }
                printf("\n");
            }

            xcase '4':{
                printf("Printing map list\n\n");
                beaconServerPrintMapList();
            }

            xcase '5':{
                printf("Printing queue list\n\n");
                beaconServerPrintQueueList();
            }

            xcase '6':{
                printf("Queueing map files\n\n");
                beaconServerQueueMapFiles();
            }

            xcase '7':{
                printf("Queueing map list file\n\n");
                beaconServerQueueMapListFile();
            }

            xcase '8':{
                printf("Starting at beginning of queue\n\n");
                beacon_server.nextQueueIndex = 0;
            }

            xcase '9':{
                printf("Queueing main-list maps\n\n");
                beaconServerQueueMainListMaps();
            }

            xcase '0':{
                printf("Clearing map queue!\n");
                if(beaconEnterString(confirm, 3) && !strcmp(confirm, "yes")){
                    printf("\nClearing the map queue!");
                    eaClearEx(&beacon_server.queueList, NULL);
                    beacon_server.nextQueueIndex = 0;
                }
                printf("\n");
            }

            xdefault:{
                printMenu = 0;
            }
        }

        if(done){
            break;
        }
    }
}

static void beaconServerSentryMenu(void){
    S32 foundCrashed = 0;
    NetLink* link;
    BeaconServerClientData* sentry;
    S32 i;

    if(!beacon_server.clients.links->size){
        return;
    }

    beaconServerMoveSelection(0);

    link = beacon_server.clients.links->storage[beacon_server.selectedClient];
    sentry = link->userData;

    if(sentry->sentry){
        sentry = sentry->sentry;
    }

    if(sentry->clientType != BCT_SENTRY){
        return;
    }

    beaconClientPrintf(sentry, COLOR_GREEN, "Client status for %s/%s @ %s:\n", sentry->computerName, sentry->userName, getClientIPStr(sentry));

    for(i = 0; i < sentry->sentryClients.count; i++){
        BeaconServerSentryClientData* sentryClient = sentry->sentryClients.clients + i;

        printf("%d. pid %d\n", i, sentryClient->pid);

        if(sentryClient->crashText){
            foundCrashed = 1;

            if(sentryClient->client){
                assert(sentryClient->client->sentry == sentry);

                printf(    "      %s/%s @ %s\n",
                        sentryClient->client->computerName,
                        sentryClient->client->userName,
                        getClientIPStr(sentryClient->client));
            }

            beaconPrintf(COLOR_GREEN|COLOR_BLUE, "%s\n", sentryClient->crashText);
        }else{
            beaconPrintf(COLOR_GREEN, "  No current problems.\n");
        }
    }

    printf("\n\n");

    beaconPrintf(COLOR_GREEN,
                "-----------------------------------------------------------------------------------------\n"
                "Sentry Menu:\n"
                "\n"
                "\tESC : Exit this menu\n"
                "\t1   : Kill all clients\n"
                "\t2   : Kill crashed clients\n"
                "\n"
                "Enter selection: ");

    beaconServerUpdateTitle("Sentry Menu");

    while(1){
        U8 theChar = _getch();
        S32 done = 1;

        if(theChar == 0 || theChar == 224){
            _getch();
            continue;
        }

        switch(theChar){
            xcase 27:{
            }

            xcase '1':{
                printf("Sending kill all clients.\n\n");
                sendKillToSentry(sentry, 0);
            }

            xcase '2':{
                printf("Sending kill crashed clients.\n\n");
                sendKillToSentry(sentry, 1);
            }

            xdefault:{
                done = 0;
            }
        }

        if(done){
            break;
        }
    }

    beaconPrintf(COLOR_RED, "Exited Sentry Menu\n\n");
}

static void sendKillAllClientsToSentry(BeaconServerClientData* client, S32 index, void* userData){
    if(client->clientType == BCT_SENTRY){
        sendKillToSentry(client, 0);
    }
}

static void sendKillCrashedClientsToSentry(BeaconServerClientData* client, S32 index, void* userData){
    if(client->clientType == BCT_SENTRY){
        sendKillToSentry(client, 1);
    }
}

static void beaconServerKillClientsMenu(void){
    S32 killAll;

    beaconPrintf(COLOR_RED, "Kill ALL or CRASHED clients (A/C)? ");

    switch(tolower(_getch())){
        xcase 'a':
            beaconPrintf(COLOR_GREEN, "ALL!\n\n");
            killAll = 1;
        xcase 'c':
            beaconPrintf(COLOR_GREEN, "CRASHED!\n\n");
            killAll = 0;
        xdefault:
            beaconPrintf(COLOR_RED, "CANCELED!\n\n");
            return;
    }

    beaconPrintf(COLOR_RED, "Are you sure you want to kill all clients (yes/no)?  ");

    if(tolower(_getch()) == 'y'){
        beaconPrintf(COLOR_GREEN, "Y");

        if(tolower(_getch()) == 'e'){
            beaconPrintf(COLOR_GREEN, "E");

            if(tolower(_getch()) == 's'){
                beaconPrintf(COLOR_GREEN, "S!!!\n\n");

                beaconServerForEachClient(killAll ? sendKillAllClientsToSentry : sendKillCrashedClientsToSentry, NULL);

                return;
            }
        }
    }

    while(_kbhit()){
        _getch();
    }

    beaconPrintf(COLOR_RED, " -- Canceled!!!\n\n");
}

static void beaconServerSetState(BeaconServerState state){
    if(beacon_server.state != state){
        beacon_server.state = state;

        beacon_server.setStateTime = beaconGetCurTime();

        beaconServerSetSendStatus();

        //printf("changing state: %d\n", state);
    }
}

static void beaconServerSetCurMap(const char* newMapFile){
    if(!newMapFile){
        newMapFile = "";
    }

    if(beacon_server.curMapName && !stricmp(beacon_server.curMapName, newMapFile)){
        return;
    }
    
    beaconServerCloseMapLogFileHandles();

    beaconProcessUndoCheckouts();

    beacon_server.sendClientsToMe = 1;

    beaconServerSetSendStatus();

    estrPrintCharString(&beacon_server.curMapName, newMapFile);

    beaconServerSetState(BSS_NOT_STARTED);

    if(estrLength(&beacon_server.curMapName)){
        forwardSlashes(beacon_server.curMapName);

        beaconServerUpdateTitle("Starting map: %s", beacon_server.curMapName);

        beaconPrintf(    COLOR_GREEN|COLOR_BLUE,
                        "---(Q: %d/%d, L: %d/%d)----------------------------------------------------------------------------------------------\n",
                        beacon_server.nextQueueIndex,
                        eaSize(&beacon_server.queueList),
                        beacon_server.curMapIndex,
                        beacon_server.mapCount);

        beaconPrintf(    COLOR_GREEN,
                        "STARTING PROCESS: ");

        beaconPrintf(    COLOR_GREEN|COLOR_BLUE,
                        "%s\n",
                        beacon_server.curMapName);
    }
}

typedef struct ExecuteCommandParams {
    const char* commandText;
    S32            useActive;
    S32            showWindowType;
} ExecuteCommandParams;

static void beaconServerSendCommandToExecute(BeaconServerClientData* client, S32 index, ExecuteCommandParams* params){
    if(client->state != BCS_SENTRY){
        return;
    }

    if(    !params->useActive &&
        !client->forcedInactive)
    {
        return;
    }

    beaconClientPrintf(    client,
                        COLOR_YELLOW,
                        "(%s/%s) Running command: \"%s\"\n",
                        client->computerName,
                        client->userName,
                        params->commandText);

    BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_EXECUTE_COMMAND);

    pktSendString(pak, params->commandText);
    pktSendBitsPack(pak, 1, params->showWindowType);

    BEACON_SERVER_PACKET_SEND();
}

static void beaconServerRunCommandOnSentry(void){
    ExecuteCommandParams    params = {0};

    char                    buffer[1000];
    S32                        ret;

    beaconPrintf(COLOR_BRIGHT|COLOR_GREEN, "Enter command: ");

    ret = beaconEnterString(buffer, ARRAY_SIZE(buffer) - 1);

    if(!ret){
        beaconPrintf(COLOR_RED, "Canceled!");
        return;
    }

    printf("\n");

    printf("Apply to active clients (instead of just inactive ones)? (y/n) ");

    switch(tolower(_getch())){
        xcase 'y':
            beaconPrintf(COLOR_GREEN, "yes!\n");
            params.useActive = 1;
        xcase 'n':
            beaconPrintf(COLOR_RED, "no!\n");
        xdefault:
            beaconPrintf(COLOR_RED, "Canceled!\n");
            return;
    }

    printf("Show window: (N)ormal, (M)inimized, (H)idden? ");

    switch(tolower(_getch())){
        xcase 'n':
            beaconPrintf(COLOR_GREEN, "normal\n");
            params.showWindowType = SW_NORMAL;
        xcase 'm':
            beaconPrintf(COLOR_GREEN, "minimized\n");
            params.showWindowType = SW_MINIMIZE;
        xcase 'h':
            beaconPrintf(COLOR_GREEN, "hidden\n");
            params.showWindowType = SW_HIDE;
        xdefault:
            beaconPrintf(COLOR_RED, "Canceled!\n");
            return;
    }

    printf("Are you sure? (y/n) ");

    if(tolower(_getch()) == 'y'){
        beaconPrintf(COLOR_GREEN, "yes!\n");
    }else{
        beaconPrintf(COLOR_RED, "no!\n");
        return;
    }

    printf("\n");

    params.commandText = buffer;

    beaconServerForEachClient(beaconServerSendCommandToExecute, &params);
}

static void beaconServerMainMenu(void){
    S32 startTime = time(NULL);
    S32 printMenu = 1;

    // Load the file list.

    while(1){
        S32 done = 0;
        S32 keyIsPressed = _kbhit();

        beaconServerUpdateTitle("Main Menu (Exit in %ds)", startTime + 10 - time(NULL));

        if(!printMenu){
            if(time(NULL) - startTime >= 10){
                done = 1;
            }
        }else{
            startTime = time(NULL);

            printf(    "\n\n"
                    "---- Main Menu ---------------------------------------\n"
                    " 1. Memory Dump\n"
                    " 2. Toggle Local IPs Only:................ %3s\n"
                    " 3. Network Initialized:.................. %3s\n"
                    " 4. Toggle Loading Maps:.................. %3s\n"
                    " 5. Toggle Forced Rebuild:................ %3s\n"
                    " 6. Toggle Full CRC Info:................. %3s\n"
                    " 7. Toggle Generate Only:................. %3s\n"
                    " 8. Toggle Check Encounter Positions Only: %3s\n"
                    " 9. Toggle Testing Request Client\n"
                    " 0. Kill Clients On All Sentries\n"
                    "-----------------------------------------------------\n"
                    " C. Print client states.\n"
                    " W. Write current map data.\n"
                    " L. Map list submenu.\n"
                    " N. Cancel current map.\n"
                    " P. Toggle pause.\n"
                    " R. Run a command on inactive sentries.\n"
                    " U. Toggle use new positions (%s).\n"
                    "-----------------------------------------------------\n"
                    " ESC.    Exit this menu.\n"
                    "-----------------------------------------------------\n"
                    "\n\n"
                    "Enter selection: ",
                    beacon_server.localOnly                        ? "ON" : "OFF",
                    beacon_server.clients.destroyCallback        ? "YES" : "NO",
                    beacon_server.loadMaps                        ? "ON" : "OFF",
                    beacon_server.forceRebuild                    ? "ON" : "OFF",
                    beacon_server.fullCRCInfo                    ? "ON" : "OFF",
                    beacon_server.generateOnly                    ? "ON" : "OFF",
                    beacon_server.checkEncounterPositionsOnly    ? "ON" : "OFF",
                    beaconGenerateUseNewPositions(-1)            ? "ON" : "OFF");
        }

        printMenu = 1;

        switch(keyIsPressed ? tolower(_getch()) : -1){
            xcase 0:
            case 224:{
                if(_kbhit()){
                    _getch();
                    done = 0;
                }
                printMenu = 0;
            }

            xcase 27:{
                done = 1;
            }

            xcase '1':{
                beaconPrintf(COLOR_YELLOW, "Printing memory usage...\n\n");
                beaconPrintMemory();
            }

            xcase '2':{
                S32 on = beacon_server.localOnly = !beacon_server.localOnly;
                beaconPrintf(COLOR_YELLOW, "Local IPs Only: %s\n\n", on ? "ON" : "OFF");
            }

            xcase '3':{
                beaconPrintf(COLOR_YELLOW, "Init network...\n\n");
                beaconServerInitNetwork();
            }

            xcase '4':{
                S32 on = beacon_server.loadMaps = !beacon_server.loadMaps;

                beaconPrintf(COLOR_YELLOW, "Load Maps: %s\n\n", on ? "ON" : "OFF");
            }

            xcase '5':{
                S32 on = beacon_server.forceRebuild = !beacon_server.forceRebuild;

                beaconPrintf(COLOR_YELLOW, "Forced Rebuild: %s\n\n", on ? "ON" : "OFF");
            }

            xcase '6':{
                S32 on = beacon_server.fullCRCInfo = !beacon_server.fullCRCInfo;

                beaconPrintf(COLOR_YELLOW, "Full CRC Info: %s\n\n", on ? "ON" : "OFF");
            }

            xcase '7':{
                S32 on = beacon_server.generateOnly = !beacon_server.generateOnly;

                beaconPrintf(COLOR_YELLOW, "Generate Only: %s\n\n", beacon_server.generateOnly ? "ON" : "OFF");
            }

            xcase '8':{
                S32 on = beacon_server.checkEncounterPositionsOnly = !beacon_server.checkEncounterPositionsOnly;

                beaconPrintf(COLOR_YELLOW, "Check Encounter Positions Only: %s\n\n", on ? "ON" : "OFF");
            }

            xcase '9':{
                beaconPrintf(COLOR_YELLOW, "Sending beaconizing request...\n\n");
                beacon_server.testRequestClient = 1;
                beacon_server.sendClientsToMe = 0;
            }

            xcase '0':{
                beaconPrintf(COLOR_YELLOW, "Kill clients...\n\n");
                beaconServerKillClientsMenu();
            }

            xcase 'c':{
                beaconPrintf(COLOR_YELLOW, "Client list...\n\n");
                beaconServerPrintClientStates();
            }

            xcase 'w':{
                S32 on = beacon_server.writeCurMapData = !beacon_server.writeCurMapData;

                beaconPrintf(COLOR_YELLOW, "Write current map data: %s\n\n", on ? "ON" : "OFF");
            }

            xcase 'l':{
                if(!beacon_server.isMasterServer){
                    beaconPrintf(COLOR_YELLOW, "Map list menu...\n\n");
                    beaconServerMapListMenu();
                }else{
                    startTime = time(NULL);
                    printMenu = 0;
                }
            }

            xcase 'n':{
                beaconServerSetCurMap(NULL);
            }

            xcase 'p':{
                beacon_server.paused = !beacon_server.paused;
            }

            xcase 'r':{
                beaconPrintf(COLOR_YELLOW, "Running command on all inactive sentries...\n\n");

                beaconServerRunCommandOnSentry();
            }
            
            xcase 'u':{
                S32 on = beaconGenerateUseNewPositions(!beaconGenerateUseNewPositions(-1));
                
                beaconPrintf(COLOR_YELLOW, "Use new positions: %s\n\n", on ? "ON" : "OFF");
            }

            //xcase 't':{
            //    beaconTestCollision();
            //}

            xcase 13:{
                beaconPrintf(COLOR_YELLOW, "Showing sentry menu...\n\n");
                beaconServerSentryMenu();
            }

            xdefault:{
                if(keyIsPressed){
                    startTime = time(NULL);
                }

                printMenu = 0;
            }
        }

        if(done){
            break;
        }

        Sleep(10);
    }

    printf("Exiting main menu\n\n");
}

static void setConsolePosition(S32 x, S32 y){
    COORD pt = {x, y};

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pt);
}

static const char* beaconServerGetTabName(BeaconServerDisplayTab tab){
    switch(tab){
        xcase BSDT_MAP:            return "Map";
        xcase BSDT_SENTRIES:    return "Sentries";
        xcase BSDT_SERVERS:        return "Servers";
        xcase BSDT_WORKERS:        return "Workers";
        xcase BSDT_REQUESTERS:    return "Requesters";
        xdefault:                return "Unknown";
    }
}

static void beaconServerSetCurTab(BeaconServerDisplayTab tab){
    if(tab < 0){
        tab = 0;
    }
    else if(tab >= BSDT_COUNT){
        tab = BSDT_COUNT - 1;
    }

    if(tab != beacon_server.display.curTab){
        beacon_server.display.curTab = tab;
        beacon_server.display.updated = 0;
    }
}

static void beaconServerUpdateDisplay(void){
    static struct {
        S32                            init;

        BeaconServerDisplayTab        tab;

        char*                        mapName;
    } last;

    if(beacon_server.display.updated || !backConsoleRefCount){
        return;
    }

    beacon_server.display.updated = 1;

    if(!last.init){
        last.tab = -1;
    }

    if(    beacon_server.curMapName &&
        (    !last.mapName ||
            stricmp(last.mapName, beacon_server.curMapName)))
    {
        estrPrintCharString(&last.mapName, beacon_server.curMapName);

        setConsolePosition(0, 0);

        beaconConsolePrintf(0, "Map: %s\nStuff: %s\n", beacon_server.curMapName, "lksjdf");
    }

    if(last.tab != beacon_server.display.curTab){
        S32 i;

        last.tab = beacon_server.display.curTab;

        setConsolePosition(0, 4);

        for(i = 0; i < BSDT_COUNT; i++){
            beaconConsolePrintf(i == beacon_server.display.curTab ? COLOR_GREEN : COLOR_WHITE, "%s%s", i ? "   " : "", beaconServerGetTabName(i));
        }
    }

    switch(beacon_server.display.curTab){
        xcase BSDT_MAP:{
            if(0) if(beacon_server.display.mapTab.updated){
                break;
            }

            beacon_server.display.mapTab.updated = 1;

            setConsolePosition(0, 6);

            displayBlockInfo();
        }

        xcase BSDT_SENTRIES:{
        }

        xcase BSDT_SERVERS:{
        }

        xcase BSDT_WORKERS:{
        }

        xcase BSDT_REQUESTERS:{
        }
    }
}

static void beaconServerCheckInput(void){
    U8 theChar;

    if(!IsWindowVisible(beaconGetConsoleWindow()) || !_kbhit()){
        return;
    }

    theChar = tolower(_getch());

    switch(theChar){
        xcase 0:{
            theChar = _getch();
            switch(theChar){
                xdefault:{
                    beaconPrintf(COLOR_YELLOW, "unhandled extended key:   0+%d\n", theChar);
                }
            }
        }

        xcase 224:{
            theChar = _getch();
            switch(theChar){
                xcase 72:
                    // Up.

                    beaconServerMoveSelection(-1);
                    beaconServerPrintClientStates();

                xcase 75:{
                    // Left.

                    beaconServerSetCurTab(beacon_server.display.curTab - 1);
                }

                xcase 77:{
                    // Right.

                    beaconServerSetCurTab(beacon_server.display.curTab + 1);
                }

                xcase 80:{
                    // Down.

                    beaconServerMoveSelection(1);
                    beaconServerPrintClientStates();
                }

                xdefault:{
                    beaconPrintf(COLOR_YELLOW, "unhandled extended key: 224+%d\n", theChar);
                }
            }
        }

        xcase 27:
        case 'm':{
            beaconServerMainMenu();
        }

        xcase 'h':{
            if(!beaconIsProductionMode()){
                ShowWindow(beaconGetConsoleWindow(), SW_HIDE);
            }
        }

        xcase '\t':{
            static S32 on;
            on = !on;
            if(on){
                beaconEnableBackgroundConsole();
            }else{
                beaconDisableBackgroundConsole();
            }
        }

        xdefault:{
            beaconPrintf(COLOR_YELLOW, "unhandled key: %d\n", theChar);
        }
    }
}

static void beaconServerCheckForNewMap(void){
    if(    beacon_server.isRequestServer ||
        !beacon_server.loadMaps ||
        beaconServerHasMapLoaded())
    {
        return;
    }

    // Find a new map to load, and load it.

    //beaconServerFindPartialProcess();

    if(beacon_server.nextQueueIndex >= 0 && beacon_server.nextQueueIndex < eaSize(&beacon_server.queueList)){
        beaconServerSetCurMap(beacon_server.queueList[beacon_server.nextQueueIndex]);
        beacon_server.nextQueueIndex++;
    }

    if(!beaconServerHasMapLoaded()){
        beacon_server.curMapIndex++;

        if(    beacon_server.isAutoServer &&
            beacon_server.curMapIndex >= beacon_server.mapCount)
        {
            S32 i;

            if (beacon_server.onePassOnly) {
                beaconPrintf(COLOR_GREEN, "Finished processing all maps.  Exiting.");
                exit(0);
            }

            beaconPrintf(COLOR_YELLOW, "Done processing maps.  Restarting self and exiting in: ");

            beaconStartNewExe(    beaconServerExeName,
                                beacon_server.exeFile.data,
                                beacon_server.exeFile.size,
                                beaconMyCmdLineParams(0),
                                0, 0, 0);

            beaconDeleteOldExes(beaconServerExeName, NULL);

            beaconServerForEachClient(beaconServerDisconnectClientCallback, "All maps are done processing.");

            for(i = 5; i; i--){
                beaconPrintf(COLOR_GREEN, "%d, ", i);

                Sleep(1000);
            }

            beaconPrintf(COLOR_GREEN, "BYE!\n\n\n\n\n");

            exit(0);
        }

        if(beacon_server.curMapIndex < 0 || beacon_server.curMapIndex >= beacon_server.mapCount){
            beacon_server.curMapIndex = 0;
        }

        if(beacon_server.curMapIndex < beacon_server.mapCount){
            beaconServerSetCurMap(beacon_server.mapList[beacon_server.curMapIndex]);
        }
    }

    if(beaconServerHasMapLoaded()){
        if(!beaconServerBeginMapProcess(beacon_server.curMapName)){
            beaconServerSetCurMap(NULL);
            beaconPrintf(COLOR_RED, "CANCELING PROCESS!\n");
        }
    }
}

static void beaconServerDoStateNotStarted(void){
    beaconServerSetState(BSS_GENERATING);

    beaconServerUpdateTitle("Init generator: %s", beacon_server.curMapName);

    beaconInitGenerating(0);

    // Insert the legal beacons into their blocks.

    beaconInsertLegalBeacons();

    beaconPrintf(COLOR_GREEN, "Ready to generate!\n");
}

static void beaconServerDoStateGenerating(void){
    BeaconDiskSwapBlock* block;
    S32 assignedCount = 0;
    S32 availableCount = 0;
    S32 closedCount = 0;
    S32 unused = 0;

    for(block = bp_blocks.list; block; block = block->nextSwapBlock){
        if(block->clients.count){
            assignedCount++;
        }
        else if(block->legalCompressed.uncheckedCount){
            availableCount++;
        }
        else if(block->legalCompressed.totalCount){
            closedCount++;
        }
        else{
            unused++;
        }
    }

    beaconServerUpdateTitle("Generate: %d assigned, %d available, %d closed, %d unused",
                            assignedCount,
                            availableCount,
                            closedCount,
                            unused);

    if(!availableCount && !assignedCount){
        if(!beacon_server.isRequestServer){
            printf("\nFinished generating: %s\n", beaconCurTimeString(0));
        }

        beaconServerSetState(BSS_CONNECT_BEACONS);

        beaconServerAddAllMadeBeacons();
    }
}

static void beaconServerDoStateConnecting(void){
    S32 done = 1;

    if(!beacon_server.generateOnly){
        S32 availableCount =    beacon_server.beaconConnect.groups.count -
                                beacon_server.beaconConnect.assignedCount -
                                beacon_server.beaconConnect.finishedCount;

        S32 ungroupedCount =    beacon_server.beaconConnect.legalBeacons.count -
                                beacon_server.beaconConnect.legalBeacons.groupedCount;

        S32 unreachedCount =    combatBeaconArray.size -
                                beacon_server.beaconConnect.legalBeacons.count;

        assert(availableCount >= 0);

        beaconServerUpdateTitle("Connect: %d assigned, %d todo, %d done, %d ungrouped, %s / %s unreached",
                                beacon_server.beaconConnect.assignedCount,
                                availableCount,
                                beacon_server.beaconConnect.finishedCount,
                                ungroupedCount,
                                getCommaSeparatedInt(unreachedCount),
                                getCommaSeparatedInt(combatBeaconArray.size));

        done =    !availableCount &&
                !ungroupedCount &&
                !beacon_server.beaconConnect.assignedCount;
    }

    if(done){
        // And we're done connecting beacons.

        if(!beacon_server.isRequestServer){
            printf("\n\nFinished connecting beacons: %s\n\n\n", beaconCurTimeString(0));
        }

        beaconServerSetState(BSS_WRITE_FILE);

        beacon_server.sendClientsToMe = 0;

        beaconServerSetSendStatus();
    }
}

static void beaconServerWriteFileCallback(const void* data, U32 size){
    dynArrayFitStructs(    &beacon_server.beaconFile.uncompressed.data,
                        &beacon_server.beaconFile.uncompressed.maxByteCount,
                        beacon_server.beaconFile.uncompressed.byteCount + size);

    memcpy(beacon_server.beaconFile.uncompressed.data + beacon_server.beaconFile.uncompressed.byteCount, data, size);

    beacon_server.beaconFile.uncompressed.byteCount += size;
}

static void beaconServerCompressBeaconFile(void){
    S32 ret;

    beacon_server.beaconFile.compressed.byteCount = beacon_server.beaconFile.uncompressed.byteCount * 1.0125 + 12;

    dynArrayFitStructs(    &beacon_server.beaconFile.compressed.data,
                        &beacon_server.beaconFile.compressed.maxByteCount,
                        beacon_server.beaconFile.compressed.byteCount);

    ret = compress2(beacon_server.beaconFile.compressed.data,
                    &beacon_server.beaconFile.compressed.byteCount,
                    beacon_server.beaconFile.uncompressed.data,
                    beacon_server.beaconFile.uncompressed.byteCount,
                    5);

    assert(ret == Z_OK);

    beacon_server.beaconFile.crc = freshCRC(beacon_server.beaconFile.compressed.data,
                                            beacon_server.beaconFile.compressed.byteCount);
}

static void beaconServerDoStateWriteFile(void){
    beaconServerForEachClient(beaconServerDisconnectClientCallback, "Server is done processing.");

    // Don't do the writing until all clients have disconnected.

    if(    beacon_server.clients.links->size &&
        beaconTimeSince(beacon_server.setStateTime) < 10)
    {
        beaconServerUpdateTitle("Waiting for %d clients to disconnect: %ds",
                                beacon_server.clients.links->size,
                                10 - beaconTimeSince(beacon_server.setStateTime));
        return;
    }

    if(!beacon_server.status.acked){
        beaconServerUpdateTitle("Waiting for master server to ACK my status");
        return;
    }

    beaconRebuildBlocks(!beacon_server.generateOnly, beacon_server.isRequestServer);
    beaconProcessNPCBeacons();
    beaconProcessTrafficBeacons();

    if(!beacon_server.isRequestServer){
        beaconServerVerifyEncounterPositions();

        beaconServerUpdateTitle("Writing beacon file to disk!");

        printf("Writing file: ");
        beaconWriteCurrentFile(false);
        printf("Done!\n");
    }else{
        beaconServerUpdateTitle("Writing beacon file to memory!");

        beacon_server.beaconFile.uncompressed.byteCount = 0;

        writeBeaconFileCallback(beaconServerWriteFileCallback, 1);

        beaconServerCompressBeaconFile();

        beaconPrintf(    COLOR_GREEN,
                        "Beacon file: %s bytes\n",
                        getCommaSeparatedInt(beacon_server.beaconFile.compressed.byteCount));
    }

    beaconPrintf(    COLOR_GREEN,
                    "Done (%s), Map: %s\n",
                    beaconCurTimeString(0),
                    beacon_server.curMapName);

    beaconServerSetState(BSS_DONE);
}

static void beaconServerSendBeaconFileToMaster(void){
    U32 remaining = beacon_server.beaconFile.compressed.byteCount - beacon_server.beaconFile.sentByteCount;
    U32 bytesToSend = min(remaining, 64 * 1024);

    BEACON_CLIENT_PACKET_CREATE_TO_LINK(&beacon_server.master_link, BMSG_C2ST_BEACON_FILE);

        pktSendBitsPack(pak, 1, beacon_server.request.nodeUID);
        pktSendString(pak, beacon_server.request.uniqueStorageName);
        pktSendBitsPack(pak, 1, beacon_server.beaconFile.sentByteCount);
        pktSendBitsPack(pak, 1, bytesToSend);
        pktSendBitsPack(pak, 1, beacon_server.beaconFile.compressed.byteCount);
        pktSendBitsPack(pak, 1, beacon_server.beaconFile.uncompressed.byteCount);
        pktSendBitsPack(pak, 1, beacon_server.beaconFile.crc);

        pktSendBitsArray(    pak,
                            8 * bytesToSend,
                            beacon_server.beaconFile.compressed.data + beacon_server.beaconFile.sentByteCount);

        beacon_server.beaconFile.sentByteCount += bytesToSend;

    BEACON_CLIENT_PACKET_SEND();

    if(beacon_server.beaconFile.sentByteCount == beacon_server.beaconFile.compressed.byteCount){
        estrDestroy(&beacon_server.request.uniqueStorageName);
        ZeroStruct(&beacon_server.request);
        beaconServerSetState(BSS_DONE);
    }else{
        beaconServerSetState(BSS_SEND_BEACON_FILE);
    }
}

static void beaconServerCountClientPackets(BeaconServerClientData* client, S32 index, S32* curSendQueue){
    *curSendQueue += qGetSize(client->link->sendQueue2);
}

static void printClientSendQueue(BeaconServerClientData* client, S32 index, void* userData){
    printf(    "%20s%20s%20s:%d\n",
            getClientIPStr(client),
            client->computerName,
            client->userName,
            qGetSize(client->link->sendQueue2));
}

static void beaconServerProcessClients(void){
    BeaconServerProcessClientsData pcd = {0};

    beaconServerForEachClient(beacon_server.paused ? beaconServerProcessClientPaused : beaconServerProcessClient, &pcd);
}

static void beaconServerProcessCurrentMap(void){
    if(beaconServerHasMapLoaded()){
        // TEMP!
        if(0){
            static S32 maxSendQueue = 0;
            S32 curSendQueue = 0;
            beaconServerForEachClient(beaconServerCountClientPackets, &curSendQueue);

            if(curSendQueue > maxSendQueue){
                printf("new max send queue: %d (was %d)\n", curSendQueue, maxSendQueue);
                maxSendQueue = curSendQueue;
                beaconServerForEachClient(printClientSendQueue, NULL);
            }
        }

        // Process all the clients.

        beaconServerProcessClients();

        switch(beacon_server.state){
            xcase BSS_NOT_STARTED:{
                beaconServerDoStateNotStarted();
            }

            xcase BSS_GENERATING:{
                beaconServerDoStateGenerating();
            }

            xcase BSS_CONNECT_BEACONS:{
                beaconServerDoStateConnecting();
            }

            xcase BSS_WRITE_FILE:{
                beaconServerDoStateWriteFile();
            }

            xcase BSS_SEND_BEACON_FILE:{
                beaconServerForEachClient(beaconServerDisconnectClientCallback, "Server isn't processing.");

                beaconServerUpdateTitle("Sending beacon file: %s/%s bytes",
                                        getCommaSeparatedInt(beacon_server.beaconFile.sentByteCount),
                                        getCommaSeparatedInt(beacon_server.beaconFile.compressed.byteCount));
            }

            xcase BSS_DONE:{
                if(estrLength(&beacon_server.request.uniqueStorageName)){
                    beacon_server.beaconFile.sentByteCount = 0;

                    beaconServerSendBeaconFileToMaster();
                }else{
                    beaconServerUpdateTitle(NULL);
                    beaconServerSetCurMap(NULL);
                    beaconServerResetMapData();

                    if(!beacon_server.isRequestServer){
                        memMonitorDisplayStats();
                    }
                }
            }

            xdefault:{
                beaconServerUpdateTitle("Unknown Mode: %d!", beacon_server.state);
            }
        }
    }else{
        beaconServerForEachClient(beaconServerPingClient, NULL);

        beaconServerUpdateTitle(beacon_server.isMasterServer ? NULL : "No Map Loaded");

        if(!beacon_server.isMasterServer){
            beaconServerForEachClient(beaconServerDisconnectClientCallback, "There is no map loaded.");
        }else{
            if(!beacon_server.localOnly){
                beaconServerForEachClient(beaconServerDisconnectErrorNonLocalIP, NULL);
            }
        }
    }
}

static struct {
    const char* name;
    S32            port;
} connectMasterServer;

void beaconServerConnectIdleCallback(F32 timeLeft){
    if(timeLeft){
        beaconServerUpdateTitle("Connecting to master server (%s:%d) %1.1f...",
                                connectMasterServer.name,
                                connectMasterServer.port,
                                timeLeft);
    }
}

static void beaconServerSendMasterConnect(void){
    Packet* pak = pktCreateEx(&beacon_server.master_link, BMSG_C2S_SERVER_CONNECT);

    pktSendBitsPack(pak, 1, BEACON_SERVER_PROTOCOL_VERSION);

    pktSendBits(pak, 32, beacon_server.exeFile.crc);

    pktSendString(pak, getUserName());
    pktSendString(pak, getComputerName());

    pktSendBitsPack(pak, 1, beacon_server.port);

    if(BEACON_SERVER_PROTOCOL_VERSION >= 1){
        pktSendBits(pak, 1, beacon_server.isRequestServer ? 1 : 0);
    }

    pktSend(&pak, &beacon_server.master_link);
}

static void beaconServerSendServerStatus(void){
    if(!beacon_server.status.send || !beacon_client_conn.readyToWork){
        return;
    }

    beacon_server.status.send = 0;

    BEACON_CLIENT_PACKET_CREATE_TO_LINK(&beacon_server.master_link, BMSG_C2ST_SERVER_STATUS);

        pktSendString(pak, beacon_server.curMapName ? beacon_server.curMapName : "");
        pktSendBitsPack(pak, 1, beacon_server.clients.links->size);
        pktSendBitsPack(pak, 1, beacon_server.state);
        pktSendBitsPack(pak, 1, beacon_server.status.sendUID);
        pktSendBits(pak, 1, beacon_server.sendClientsToMe);

    BEACON_CLIENT_PACKET_SEND();
}

static void processBeaconMasterServerMsgClientCap(Packet* pak){
    S32 maxAllowed = pktGetBitsPack(pak, 1);
    S32 i;

    {
        char* timeString = pktGetString(pak);
        //printf("%s: got maxAllowed = %d\n", timeString, maxAllowed);
    }

    for(i = beacon_server.clients.links->size - 1; i >= maxAllowed; i--){
        NetLink* link = beacon_server.clients.links->storage[i];
        BeaconServerClientData* client = link->userData;
        char buffer[1000];

        sprintf(buffer, "I have %d clients and the cap is %d.", beacon_server.clients.links->size, maxAllowed);

        beaconServerDisconnectClient(client, buffer);
    }
}

static void processBeaconMasterServerMsgStatusAck(Packet* pak){
    U32 ackedStatusUID = pktGetBitsPack(pak, 1);

    if(ackedStatusUID == beacon_server.status.sendUID){
        beacon_server.status.acked = 1;
    }
}

static void beaconServerWriteCurMapData(void){
    static S32 count;

    char fileName[MAX_PATH];
    Vec3 pyr;
    FILE* f;
    S32 i;
    S32 j;
    S32 k;

    strcpy(fileName, "c:\\beaconizer\\requestmaps");

    makeDirectories(fileName);

    sprintf(fileName, "c:\\beaconizer\\requestmaps\\%s.%d.txt", beacon_server.serverUID, count++);

    f = fopen(fileName, "wt");

    if(!f){
        return;
    }

    for(i = 0; i < group_info.file_count; i++){
        GroupFile* file = group_info.files[i];

        fprintf(f, "Group File: %s\n", file->fullname);

        for(j = 0; j < file->count; j++){
            GroupDef* def = file->defs[j];

            fprintf(f, "Def: %s\n", def ? def->name : "NONE");

            if(!def){
                continue;
            }

            fprintf(f, "  NoBeaconGroundConnections: %d\n", def->no_beacon_ground_connections);

            if(def->model){
                fprintf(f, "  model->ctriflags_setonall:  0x%8.8x\n", def->model->ctriflags_setonall);
                fprintf(f, "  model->ctriflags_setonsome: 0x%8.8x\n", def->model->ctriflags_setonsome);
            }

            for(k = 0; k < def->count; k++){
                GroupEnt* child = def->entries + k;

                getMat3YPR(child->mat, pyr);

                fprintf(f, "    Child: %s\n", child->def->name);
                fprintf(f, "      pos: %f %f %f\n", posParamsXYZ(child));
                fprintf(f, "      pyr: %f %f %f\n", vecParamsXYZ(pyr));
                fprintf(f, "\n");
            }
        }
    }

    for(i = 0; i < group_info.ref_count; i++){
        DefTracker* ref = group_info.refs[i];

        fprintf(f, "Ref: %s", ref->def ? ref->def->name : "NONE");

        if(!ref->def){
            continue;
        }

        getMat3YPR(ref->mat, pyr);

        fprintf(f, "  pos: %f %f %f\n", posParamsXYZ(ref));
        fprintf(f, "  pyr: %f %f %f\n", vecParamsXYZ(pyr));
        fprintf(f, "\n");
    }

    fprintf(f, "Starting beacons:\n");

    for(i = 0; i < combatBeaconArray.size; i++){
        Beacon* b = combatBeaconArray.storage[i];

        fprintf(f, "Beacon: %f %f %f\n", posParamsXYZ(b));
    }

    fprintf(f, "\nScene Info:\n");
    fprintf(f, "MaxHeight: %f\n", scene_info.maxHeight);

    fprintf(f, "\nEnd of map dump\n\n");

    fclose(f);
}

static void processBeaconMasterServerMsgMapData(Packet* pak){
    char uniqueStorageName[1000];

    beacon_server.request.nodeUID = pktGetBitsPack(pak, 1);
    Strncpyt(uniqueStorageName, pktGetString(pak));

    estrPrintCharString(&beacon_server.request.uniqueStorageName, uniqueStorageName);

    if(pktGetBits(pak, 1)){
        beaconServerResetMapData();
        beaconServerSetCurMap(NULL);
        beaconServerSetState(BSS_NOT_STARTED);
        beacon_server.sendClientsToMe = 0;
        beaconServerSetSendStatus();
    }

    if(!beacon_server.mapData){
        beaconMapDataPacketCreate(&beacon_server.mapData);
    }

    beaconMapDataPacketReceiveChunk(pak, beacon_server.mapData);

    //if(newMap){
    //    beaconPrintf(    COLOR_GREEN,
    //                    "Receiving new map (%s:%d): %s bytes.\n",
    //                    beacon_server.request.uniqueStorageName,
    //                    beacon_server.request.uid,
    //                    getCommaSeparatedInt(beaconMapDataPacketGetSize(beacon_server.mapData)));
    //}

    //if(0){
    //    beaconPrintf(    COLOR_GREEN,
    //                    "Received data for map (%s): %s/%s bytes.\n",
    //                    beacon_server.request.uniqueStorageName,
    //                    getCommaSeparatedInt(beaconMapDataPacketGetReceivedSize(beacon_server.mapData)),
    //                    getCommaSeparatedInt(beaconMapDataPacketGetSize(beacon_server.mapData)));
    //}

    if(!beaconMapDataPacketIsFullyReceived(beacon_server.mapData)){
        BEACON_CLIENT_PACKET_CREATE_TO_LINK(&beacon_server.master_link, BMSG_C2ST_NEED_MORE_MAP_DATA);
        BEACON_CLIENT_PACKET_SEND();
    }else{
        beaconPrintf(    COLOR_GREEN,
                        "Received map: %s.\n",
                        beacon_server.request.uniqueStorageName);

        BEACON_CLIENT_PACKET_CREATE_TO_LINK(&beacon_server.master_link, BMSG_C2ST_MAP_DATA_IS_LOADED);

            pktSendBitsPack(pak, 1, beacon_server.request.nodeUID);
            pktSendString(pak, beacon_server.request.uniqueStorageName);

        BEACON_CLIENT_PACKET_SEND();
    }
}

static void processBeaconMasterServerMsgNeedMoreBeaconFile(Packet* pak){
    if(beacon_server.state == BSS_SEND_BEACON_FILE){
        beaconServerSendBeaconFileToMaster();
    }
}

static void processBeaconMasterServerMsgProcessRequestedMap(Packet* pak){
    //beaconPrintf(COLOR_GREEN, "Loading map!\n");

    if(beaconMapDataPacketToMapData(beacon_server.mapData)){
        beaconPrintf(COLOR_GREEN, "Map loaded!\n");

        estrPrintEString(&beacon_server.curMapName, beacon_server.request.uniqueStorageName);

        beaconServerSetState(BSS_GENERATING);

        beaconInitGenerating(1);

        beacon_server.sendClientsToMe = 1;
        beaconServerSetSendStatus();

        beaconCurTimeString(1);

        beaconMapDataPacketInitialBeaconsToRealBeacons();

        beaconInsertLegalBeacons();

        if(beacon_server.writeCurMapData){
            beaconServerWriteCurMapData();
        }
    }
    else if(beacon_server.master_link.connected){
        BEACON_CLIENT_PACKET_CREATE_TO_LINK(&beacon_server.master_link, BMSG_C2ST_REQUESTED_MAP_LOAD_FAILED);

            pktSendBitsPack(pak, 1, beacon_server.request.nodeUID);
            pktSendString(pak, beacon_server.request.uniqueStorageName);

        BEACON_CLIENT_PACKET_SEND();
    }
}

static void processBeaconMasterServerMsgTextCmd(const char* textCmd, Packet* pak){
    #define BEGIN_HANDLERS()    if(0){
    #define HANDLER(x, y)        }else if(!stricmp(textCmd, x)){y(pak)
    #define END_HANDLERS()        }

    BEGIN_HANDLERS();
        HANDLER(BMSG_S2CT_CLIENT_CAP,                processBeaconMasterServerMsgClientCap            );
        HANDLER(BMSG_S2CT_STATUS_ACK,                processBeaconMasterServerMsgStatusAck            );
        HANDLER(BMSG_S2CT_MAP_DATA,                    processBeaconMasterServerMsgMapData                );
        HANDLER(BMSG_S2CT_NEED_MORE_BEACON_FILE,    processBeaconMasterServerMsgNeedMoreBeaconFile    );
        HANDLER(BMSG_S2CT_PROCESS_REQUESTED_MAP,    processBeaconMasterServerMsgProcessRequestedMap    );
    END_HANDLERS();

    #undef BEGIN_HANDLERS
    #undef HANDLER
    #undef END_HANDLERS
}

static S32 beaconServerHandleMasterMsg(Packet* pak, S32 cmd, NetLink* link){
    beacon_client_conn.timeHeardFromServer = timerCpuTicks();

    switch(cmd){
        xcase BMSG_S2C_CONNECT_REPLY:{
            if(pktGetBits(pak, 1) == 1){
                beacon_client_conn.readyToWork = 1;
            }else{
                // Get a new executable.

                beaconHandleNewExe(    pak,
                                    beaconServerExeName,
                                    beaconMyCmdLineParams(0),
                                    0,
                                    0);
            }
        }

        xcase BMSG_S2C_TEXT_CMD:{
            char textCmd[100];

            strncpyt(textCmd, pktGetString(pak), ARRAY_SIZE(textCmd) - 1);

            processBeaconMasterServerMsgTextCmd(textCmd, pak);
        }

        xdefault:{
            beaconPrintf(COLOR_RED, "ERROR: Bad cmd received from server: %d!\n", cmd);
        }
    }

    return 1;
}

static void beaconServerMonitorMasterConnection(void){
    if(beacon_server.isMasterServer){
        return;
    }

    if(!beacon_server.master_link.connected){
        beacon_client_conn.readyToWork = 0;
        beacon_server.status.sendUID = 0;
        beacon_server.status.ackedUID = 0;
        beacon_server.status.acked = 0;
        beacon_server.sendClientsToMe = 1;

        connectMasterServer.name = beacon_server.masterServerName ? beacon_server.masterServerName : BEACON_MASTER_SERVER_DEFAULT_NAME;
        connectMasterServer.port = BEACON_MASTER_SERVER_PORT;

        netConnect(    &beacon_server.master_link,
                    connectMasterServer.name,
                    connectMasterServer.port,
                    NLT_TCP,
                    beaconIsProductionMode() ? 5 : 30,
                    beaconServerConnectIdleCallback);

        if(beacon_server.master_link.connected){
            beaconServerSetSendStatus();

            beaconServerSendMasterConnect();
        }
    }

    if(beacon_server.master_link.connected){
        if(beaconTimeSince(beacon_server.sentPingTime) > 5){
            beacon_server.sentPingTime = beaconGetCurTime();

            BEACON_CLIENT_PACKET_CREATE_TO_LINK(&beacon_server.master_link, BMSG_C2ST_PING);
            BEACON_CLIENT_PACKET_SEND();
        }

        netLinkMonitor(&beacon_server.master_link, 0, beaconServerHandleMasterMsg);

        beaconServerSendServerStatus();
    }
}

static void beaconServerTransferClient(BeaconServerClientData* transferClient, BeaconServerClientData* subServer){
    transferClient->transferred = 1;

    beaconClientPrintf(    transferClient,
                        COLOR_RED,
                        "Transferring %s/%s to server %s/%s.\n",
                        transferClient->computerName,
                        transferClient->userName,
                        subServer->computerName,
                        subServer->userName);

    BEACON_SERVER_PACKET_CREATE_BASE(pak, transferClient, BMSG_S2CT_TRANSFER_TO_SERVER);

        pktSendString(pak, subServer->computerName);
        pktSendString(pak, getClientIPStr(subServer));
        pktSendBitsPack(pak, 1, subServer->server.port);

    BEACON_SERVER_PACKET_SEND();

    // Added to fix issues with receiving a new sentry's client list over the wire before we removed this link from 
    //   the sentry's client list due to the netdisconnect callback of the transferred client (which would also remove it from the list)
    removeClientFromSentry(transferClient, 0, transferClient->uid, NULL);
}

struct {
    S32 neededCount;
    S32 clientCount;
    S32 maxAllowed;
    S32 minAllowed;
} clientStats;

static BeaconServerClientData** availableClients;

static void beaconMasterServerProcessClient(BeaconServerClientData* client,
                                            S32 index,
                                            BeaconServerProcessClientsData* pcd)
{
    NetLink* link = client->link;

    switch(client->state){
        xcase BCS_NEEDS_MORE_EXE_DATA:{
            if(pcd->sentExeDataCount < 5){
                pcd->sentExeDataCount++;

                beaconServerSendNextExeChunk(client);
            }else{
                beacon_server.noNetWait = 1;
            }
        }

        xcase BCS_RECEIVING_EXE_DATA:{
            if(beaconTimeSince(client->exeData.lastCommTime) < 3){
                pcd->sentExeDataCount++;
            }
        }
    }

    switch(client->clientType){
        xcase BCT_SERVER:{
            if(    client->server.isRequestServer &&
                beaconTimeSince(client->receivedPingTime) >= 60)
            {
                beaconServerDisconnectClient(client, "Server took more than 60 seconds to respond.");
                break;
            }

            // Send me some clients.

            if(beaconServerNeedsSomeClients(client)){
                clientStats.neededCount++;
            }

            // Find a new requester for this request server.

            if(beaconServerRequestServerIsAvailable(client)){
                BeaconProcessQueueNode* node;

                if(    beaconServerGetProcessNodeForProcessing(&node) &&
                    beaconMapDataPacketIsFullyReceived(node->mapData))
                {
                    //beaconClientPrintf(    client,
                    //                    COLOR_GREEN,
                    //                    "Assigning to request server %s (%s/%s).\n",
                    //                    getClientIPStr(requestServer),
                    //                    requestServer->computerName,
                    //                    requestServer->userName);

                    beaconServerAssignProcessNodeToRequestServer(node, client);
                }
            }
        }

        xcase BCT_SENTRY:{
            S32 i;
            for(i = 0; i < client->sentryClients.count; i++){
                BeaconServerSentryClientData* sentryClient = client->sentryClients.clients + i;

                if(    !sentryClient->crashText &&
                    client->forcedInactive)
                {
                    clientStats.clientCount++;
                }
            }
        }

        xcase BCT_WORKER:{
            if(    client->state == BCS_READY_TO_WORK &&
                client->sentry &&
                client->sentry->forcedInactive &&
                !client->transferred)
            {
                eaPush(&availableClients, client);
            }
        }

        xcase BCT_REQUESTER:{
            if(beaconTimeSince(client->receivedPingTime) >= 60){
                beaconServerDisconnectClient(client, "Requester took more than 60 seconds to respond.");
                break;
            }
        }
    }
}

static void beaconServerAssignClientsToNeedyServer(BeaconServerClientData* client, S32 index, void* userData){
    S32 sentCount = 0;

    if(    client->clientType != BCT_SERVER ||
        !beaconServerNeedsSomeClients(client))
    {
        return;
    }

    if(clientStats.neededCount >= 2){
        BEACON_SERVER_PACKET_CREATE(BMSG_S2CT_CLIENT_CAP);

            char buffer[100];
            pktSendBitsPack(pak, 1, clientStats.maxAllowed);
            sprintf(buffer, "time: %d", time(0));
            pktSendString(pak, buffer);

        BEACON_SERVER_PACKET_SEND();
    }

    while(    eaSize(&availableClients) &&
            (clientStats.neededCount < 2 || client->server.clientCount + sentCount < clientStats.minAllowed))
    {
        BeaconServerClientData* transferClient = availableClients[eaSize(&availableClients) - 1];

        sentCount++;

        beaconServerTransferClient(transferClient, client);

        eaSetSize(&availableClients, eaSize(&availableClients) - 1);
    }
}

static void beaconServerAssignRemainingClientsToNeedyServer(BeaconServerClientData* client, S32 index, void* userData){
    if(    client->clientType == BCT_SERVER &&
        estrLength(&client->server.mapName) &&
        eaSize(&availableClients) &&
        client->server.clientCount < clientStats.maxAllowed)
    {
        BeaconServerClientData* transferClient = availableClients[eaSize(&availableClients) - 1];

        beaconServerTransferClient(transferClient, client);

        eaSetSize(&availableClients, eaSize(&availableClients) - 1);
    }
}

static U32 beaconServerHasTimePassed(U32* previousTime, U32 seconds){
    U32 curTime = beaconGetCurTime();

    if((U32)(curTime - *previousTime) < seconds){
        return 0;
    }

    *previousTime = curTime;

    return 1;
}

static void beaconMasterServerProcessClients(void){
    BeaconServerProcessClientsData pcd = {0};

    beaconServerForEachClient(beaconMasterServerProcessClient, &pcd);
}

static void beaconServerDoMasterServerStuff(void){
    static U32 lastClientCapCheck;

    beaconServerMonitorMasterConnection();

    eaSetSize(&availableClients, 0);

    if(!beacon_server.isMasterServer){
        return;
    }

    if(    !beacon_server.noNetWait &&
        !beaconServerHasTimePassed(&lastClientCapCheck, 1))
    {
        return;
    }

    ZeroStruct(&clientStats);
    
    beaconMasterServerProcessClients();

    beaconServerUpdateProcessNodes();

    if(!clientStats.neededCount){
        return;
    }

    clientStats.minAllowed = clientStats.clientCount / clientStats.neededCount;
    clientStats.maxAllowed = clientStats.minAllowed + ((clientStats.clientCount % clientStats.neededCount) ? 1 : 0);

    //printf("time %d: sending maxAllowed = %d\n", time(0), maxAllowed);

    // Send the client cap and/or client transfers.

    beaconServerForEachClient(beaconServerAssignClientsToNeedyServer, NULL);

    // Transfer remaining available clients to whatever server needs some.

    if(eaSize(&availableClients)){
        beaconServerForEachClient(beaconServerAssignRemainingClientsToNeedyServer, NULL);
    }
}

static void beaconServerMonitorNetworkOrSleep(void){
    // Check on the dbserver link.

    if(beaconIsProductionMode()){
        netLinkMonitor(&beacon_server.db_link, 0, NULL);

        if(!beacon_server.db_link.connected){
            LOG_OLD("BeaconServer Detected dbserver shut down, shutting down.\n");
            logShutdownAndWait();
            exit(0);
        }
        else if(beacon_server.isMasterServer){
            static S32 lastTimeSent;

            if(!lastTimeSent){
                lastTimeSent = beaconGetCurTime();
            }

            if(beaconTimeSince(lastTimeSent) > 0){
                Packet* pak = pktCreateEx(&beacon_server.db_link, BEACON2DB_BEACONSERVER_STATUS);
                U32 longest = beaconServerGetLongestRequesterWait();

                // Packet version.

                pktSendBitsPack(pak, 1, 0);

                // Send the longest wait.

                pktSendBitsPack(pak, 1, longest);

                pktSend(&pak, &beacon_server.db_link);

                lastTimeSent = beaconGetCurTime();
            }
        }
    }

    if(beacon_server.clients.destroyCallback){
        S32 noNetWait = beacon_server.noNetWait;

        beacon_server.noNetWait = 0;

        NMMonitor(noNetWait ? POLL : 10);
    }else{
        Sleep(100);
    }
}

static void beaconServerTestRequestClient(void){
    if(beacon_server.testRequestClient){
        beacon_server.testRequestClient = 0;

        if(beaconServerHasMapLoaded()){
            beaconRequestBeaconizing("flarp");
        }
    }

    beaconRequestUpdate();
}

void beaconServer(BeaconizerType beaconizerType, const char* masterServerName, S32 noNetStart, S32 onePassOnly, S32 forceRebuild){
    beaconServerStartup(beaconizerType, masterServerName, noNetStart, onePassOnly, forceRebuild);

    while(1){
        beaconServerMonitorNetworkOrSleep();
        beaconServerDoMasterServerStuff();
        beaconServerCheckInput();
        beaconServerCheckForNewMap();
        beaconServerProcessCurrentMap();
        beaconServerTestRequestClient();
        beaconServerUpdateDisplay();
    }
}

// Beaconizer Live Request Stuff -------------------------------------------------------------------------------

typedef enum BeaconizerRequestState {
    BRS_INACTIVE,
    BRS_REQUEST_NOT_SENT,
    BRS_REQUEST_SENDING,
    BRS_REQUEST_ACCEPTED,
} BeaconizerRequestState;

struct {
    BeaconizerRequestState                state;
    NetLink                                link;
    char*                                masterServerAddress;
    U32                                    sentPingTime;

    U32                                    wasConnected;

    char*                                uniqueStorageName;

    BeaconMapDataPacket*                mapData;

    U32                                    sentByteCount;
    U32                                    uid;

    char*                                createNewRequest;

    U32                                    lastCreateRequestTime;

    struct {
        struct {
            U8*                            data;
            U32                            byteCount;
        } compressed, uncompressed;

        U32                                receivedByteCount;
        U32                                readCursor;
    } beaconFile;
} beacon_request;

static void beaconRequestSendMasterConnect(void){
    Packet* pak = pktCreateEx(&beacon_request.link, BMSG_C2S_REQUESTER_CONNECT);

    pktSendBitsPack(pak, 1, BEACON_SERVER_PROTOCOL_VERSION);

    pktSendString(pak, getUserName());
    pktSendString(pak, getComputerName());
    pktSendString(pak, beaconGetLinkIPStr(&db_comm_link));

    pktSend(&pak, &beacon_request.link);
}

static S32 beaconRequestConnectToServer(void){
    if(!beacon_request.link.connected){
        static F32 curConnectTryTime = 0.05;
        static int failureCount = 0;

        const char* serverAddress = NULL;

        if(beacon_request.wasConnected){
            beacon_request.wasConnected = 0;

            // Force a re-send of the request if the connection was broken.

            switch(beacon_request.state){
                xcase BRS_INACTIVE:
                case BRS_REQUEST_NOT_SENT:
                    // Ignored.
                xdefault:
                    beacon_request.state = BRS_REQUEST_NOT_SENT;
                    beacon_request.uid++;
            }
        }

        if(estrLength(&beacon_request.masterServerAddress)){
            serverAddress = beacon_request.masterServerAddress;
        }
        else if(isDevelopmentMode()){
            serverAddress = BEACON_MASTER_SERVER_DEFAULT_NAME;
        }

        if(serverAddress && failureCount < 5){
            netConnect(    &beacon_request.link,
                        serverAddress,
                        BEACON_MASTER_SERVER_PORT,
                        NLT_TCP,
                        curConnectTryTime,
                        NULL);

            if(beacon_request.link.connected){
                curConnectTryTime = 0.05;

                beacon_request.wasConnected = 1;

                beaconRequestSendMasterConnect();
            }else{
                if (isDevelopmentMode())
                {
                    failureCount++;
                }

                curConnectTryTime += 0.05;

                if(curConnectTryTime > 0.25){
                    curConnectTryTime = 0.05;
                }
            }
        }
    }

    return beacon_request.link.connected;
}

void beaconRequestBeaconizing(const char* uniqueStorageName){
    estrPrintCharString(&beacon_request.createNewRequest, uniqueStorageName);
    beacon_request.lastCreateRequestTime = beaconGetCurTime();
}

static void beaconRequestSendNextRequestChunk(void){
    if(!beacon_request.link.connected){
        return;
    }

    if(beacon_request.state == BRS_REQUEST_NOT_SENT){
        beacon_request.sentByteCount = 0;
        beacon_request.state = BRS_REQUEST_SENDING;
    }

    if(beaconMapDataPacketIsFullySent(beacon_request.mapData, beacon_request.sentByteCount)){
        beaconPrintf(COLOR_RED, "ERROR: Already sent the whole BeaconMapDataPacket.\n");
        return;
    }

    BEACON_CLIENT_PACKET_CREATE_TO_LINK(&beacon_request.link, BMSG_C2ST_REQUESTER_MAP_DATA);

        pktSendBitsPack(pak, 1, beacon_request.uid);
        pktSendString(pak, beacon_request.uniqueStorageName);

        beaconMapDataPacketSendChunk(    pak,
                                        beacon_request.mapData,
                                        &beacon_request.sentByteCount);

        if(0){
            beaconPrintf(    COLOR_GREEN,
                            "Sent beacon request: %s/%s bytes.\n",
                            getCommaSeparatedInt(beacon_request.sentByteCount),
                            getCommaSeparatedInt(beaconMapDataPacketGetSize(beacon_request.mapData)));
        }

    BEACON_CLIENT_PACKET_SEND();

    if(beaconMapDataPacketIsFullySent(    beacon_request.mapData,
                                        beacon_request.sentByteCount))
    {
        beaconPrintf(    COLOR_GREEN,
                        "Sent beacon request: %s bytes.\n",
                        getCommaSeparatedInt(beacon_request.sentByteCount));
    }
}

static void beaconRequestProcessMsgRequestChunkReceived(Packet* pak){
    beaconRequestSendNextRequestChunk();
}

static void beaconRequestProcessMsgRequestAccepted(Packet* pak){
    U32 uid = pktGetBitsPack(pak, 1);

    if(uid == beacon_request.uid){
        beaconPrintf(    COLOR_GREEN,
                        "Beacon request accepted: %s\n",
                        beacon_request.uniqueStorageName);

        beacon_request.state = BRS_REQUEST_ACCEPTED;
    }
}

static S32 beaconReaderCallbackReadRequestFile(void* data, U32 size){
    if(size > beacon_request.beaconFile.uncompressed.byteCount - beacon_request.beaconFile.readCursor){
        return 0;
    }

    memcpy(    data,
            beacon_request.beaconFile.uncompressed.data + beacon_request.beaconFile.readCursor,
            size);

    beacon_request.beaconFile.readCursor += size;

    return 1;
}

static void beaconRequestUncompressBeaconFile(void){
    void aiClearBeaconReferences();

    S32 ret;
    U32 uncompressedByteCount = beacon_request.beaconFile.uncompressed.byteCount;

    SAFE_FREE(beacon_request.beaconFile.uncompressed.data);
    beacon_request.beaconFile.uncompressed.data = malloc(beacon_request.beaconFile.uncompressed.byteCount);

    ret = uncompress(    beacon_request.beaconFile.uncompressed.data,
                        &uncompressedByteCount,
                        beacon_request.beaconFile.compressed.data,
                        beacon_request.beaconFile.compressed.byteCount);

    assert(ret == Z_OK);

    assert(uncompressedByteCount == beacon_request.beaconFile.uncompressed.byteCount);

    SAFE_FREE(beacon_request.beaconFile.compressed.data);
    ZeroStruct(&beacon_request.beaconFile.compressed);

    beacon_request.beaconFile.readCursor = 0;

    beaconPrintf(    COLOR_GREEN,
                    "Reading beacon file: %s bytes\n",
                    getCommaSeparatedInt(beacon_request.beaconFile.uncompressed.byteCount));

    aiClearBeaconReferences();

    readBeaconFileCallback(beaconReaderCallbackReadRequestFile);

    beaconRebuildBlocks(0, 1);

    SAFE_FREE(beacon_request.beaconFile.uncompressed.data);
    ZeroStruct(&beacon_request.beaconFile.uncompressed);
}

static void beaconRequestProcessMsgBeaconFile(Packet* pak){
    if(beacon_request.state == BRS_REQUEST_ACCEPTED){
        U32 uid;
        char uniqueStorageName[1000];
        U32 bytesToRead;

        if(!estrLength(&beacon_request.uniqueStorageName)){
            beaconPrintf(COLOR_YELLOW, "WARNING: Receiving beacon file when no request is active.\n");
            return;
        }

        uid = pktGetBitsPack(pak, 1);

        if(uid != beacon_request.uid){
            beaconPrintf(    COLOR_YELLOW,
                            "WARNING: Receiving old beacon file (old:%d, new:%d)\n",
                            uid,
                            beacon_request.uid);
            return;
        }

        Strncpyt(uniqueStorageName, pktGetString(pak));

        if(stricmp(uniqueStorageName, beacon_request.uniqueStorageName)){
            beaconPrintf(    COLOR_YELLOW,
                            "WARNING: Receiving beacon file for other request (other:%s, me:%s)\n",
                            uniqueStorageName,
                            beacon_request.uniqueStorageName);
            return;
        }

        beacon_request.beaconFile.compressed.byteCount = pktGetBitsPack(pak, 1);
        beacon_request.beaconFile.receivedByteCount = pktGetBitsPack(pak, 1);
        bytesToRead = pktGetBitsPack(pak, 1);
        beacon_request.beaconFile.uncompressed.byteCount = pktGetBitsPack(pak, 1);

        if(!beacon_request.beaconFile.receivedByteCount){
            SAFE_FREE(beacon_request.beaconFile.compressed.data);

            beacon_request.beaconFile.compressed.data = malloc(beacon_request.beaconFile.compressed.byteCount);
        }

        pktGetBitsArray(pak,
                        8 * bytesToRead,
                        beacon_request.beaconFile.compressed.data + beacon_request.beaconFile.receivedByteCount);

        beacon_request.beaconFile.receivedByteCount += bytesToRead;

        if(beacon_request.beaconFile.receivedByteCount == beacon_request.beaconFile.compressed.byteCount){
            beacon_request.state = BRS_INACTIVE;

            beaconRequestUncompressBeaconFile();

            SAFE_FREE(beacon_request.beaconFile.compressed.data);
            ZeroStruct(&beacon_request.beaconFile.compressed);
        }else{
            BEACON_CLIENT_PACKET_CREATE_TO_LINK(&beacon_request.link, BMSG_C2ST_NEED_MORE_BEACON_FILE);

                pktSendBitsPack(pak, 1, beacon_request.beaconFile.receivedByteCount);

            BEACON_CLIENT_PACKET_SEND();
        }
    }
}

static void beaconRequestProcessMsgRegenerateMapData(Packet* pak){
    if(estrLength(&beacon_request.uniqueStorageName)){
        beaconPrintf(    COLOR_YELLOW,
                        "WARNING: Master-BeaconServer says my map data packet is bad.  Regenerating.\n");

        beaconRequestBeaconizing(beacon_request.uniqueStorageName);
    }
}

static void beaconRequestProcessMsgTextCmd(const char* textCmd, Packet* pak){
    #define BEGIN_HANDLERS    if(0){
    #define HANDLER(x, y)    }else if(!stricmp(textCmd, x)){y(pak)
    #define END_HANDLERS    }else{beaconPrintf(COLOR_RED, "Unknown text cmd: %s!\n", textCmd);}

    BEGIN_HANDLERS
        HANDLER(BMSG_S2CT_REQUEST_CHUNK_RECEIVED,    beaconRequestProcessMsgRequestChunkReceived    );
        HANDLER(BMSG_S2CT_REQUEST_ACCEPTED,            beaconRequestProcessMsgRequestAccepted        );
        HANDLER(BMSG_S2CT_BEACON_FILE,                beaconRequestProcessMsgBeaconFile            );
        HANDLER(BMSG_S2CT_REGENERATE_MAP_DATA,        beaconRequestProcessMsgRegenerateMapData    );
    END_HANDLERS

    #undef BEGIN_HANDLERS
    #undef HANDLER
    #undef END_HANDLERS
}

static S32 beaconRequestHandleMsg(Packet* pak, S32 cmd, NetLink* link){
    switch(cmd){
        xcase BMSG_S2C_TEXT_CMD:{
            char textCmd[100];

            Strncpyt(textCmd, pktGetString(pak));

            beaconRequestProcessMsgTextCmd(textCmd, pak);
        }

        xdefault:{
            beaconPrintf(COLOR_YELLOW, "Unknown cmd from server: %d.\n", cmd);
        }
    }

    return 1;
}

static void beaconRequestCreateRequest(void){
    BeaconMapDataPacket* newMapData = NULL;
    Vec3 safeEntrancePos;

    if(    !beacon_request.createNewRequest ||
        (U32)(beaconGetCurTime() - beacon_request.lastCreateRequestTime) < 5)
    {
        return;
    }

    beacon_request.lastCreateRequestTime = beaconGetCurTime();

    PERFINFO_AUTO_START("createRequest", 1);

    beaconMapDataPacketClearInitialBeacons();

    if(getSafeEntrancePos(NULL, safeEntrancePos, 0, NULL)){
        beaconMapDataPacketAddInitialBeacon(safeEntrancePos, 1);
    }else{
        Vec3 pos = {-5 * 256 + 128, 0, -3 * 256 + 128};
        beaconMapDataPacketAddInitialBeacon(pos, 1);
    }

    beaconMapDataPacketFromMapData(&newMapData, 1);

    // There's something awry with the map data and we couldn't build a
    // description packet. That means we have nothing to send as a request.
    if(!newMapData){
        estrDestroy(&beacon_request.createNewRequest);
        PERFINFO_AUTO_STOP();
        return;
    }

    if(    beacon_request.uniqueStorageName &&
        !stricmp(beacon_request.uniqueStorageName, beacon_request.createNewRequest) &&
        beaconMapDataPacketIsSame(newMapData, beacon_request.mapData))
    {
        // Destroy the new packet.

        beaconMapDataPacketDestroy(&newMapData);

        // The same request is currently in some state of pending.

        estrDestroy(&beacon_request.createNewRequest);

        //beaconPrintf(COLOR_YELLOW, "Duplicate beacon request made while previous request is still pending.\n");

        PERFINFO_AUTO_STOP();

        return;
    }

    if(beacon_request.state != BRS_INACTIVE){
        //printf("Canceling beacon request, unique name: %s\n", beacon_request.uniqueStorageName);
    }

    beaconMapDataPacketDestroy(&beacon_request.mapData);
    beacon_request.mapData = newMapData;
    estrPrintEString(&beacon_request.uniqueStorageName, beacon_request.createNewRequest);
    estrDestroy(&beacon_request.createNewRequest);
    beacon_request.uid++;

    beacon_request.state = BRS_REQUEST_NOT_SENT;

    PERFINFO_AUTO_STOP();
}

void beaconRequestUpdate(void){
    PERFINFO_AUTO_START("beaconRequestUpdate", 1);

    beaconRequestCreateRequest();

    if(beacon_request.state == BRS_INACTIVE){
        if(beacon_request.link.connected){
            netSendDisconnect(&beacon_request.link, 0);
            netLinkMonitor(&beacon_request.link, 0, NULL);
        }
        PERFINFO_AUTO_STOP();
        return;
    }

    if(!beaconRequestConnectToServer()){
        PERFINFO_AUTO_STOP();
        return;
    }

    if(beaconTimeSince(beacon_request.sentPingTime)){
        beacon_request.sentPingTime = beaconGetCurTime();

        BEACON_CLIENT_PACKET_CREATE_TO_LINK(&beacon_request.link, BMSG_C2ST_PING);
        BEACON_CLIENT_PACKET_SEND();
    }

    if(beacon_request.state == BRS_REQUEST_NOT_SENT){
        beaconRequestSendNextRequestChunk();
    }

    netLinkMonitor(&beacon_request.link, 0, beaconRequestHandleMsg);

    PERFINFO_AUTO_STOP();
}

void beaconRequestSetMasterServerAddress(const char* address){
    if(beacon_request.link.connected){
        netSendDisconnect(&beacon_request.link, 0);
    }

    if(!address){
        estrDestroy(&beacon_request.masterServerAddress);
    }else{
        estrPrintCharString(&beacon_request.masterServerAddress, address);
    }
}

