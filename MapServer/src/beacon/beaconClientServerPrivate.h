
#include <process.h>
#include <io.h>
#include <direct.h>
#include "beaconPrivate.h"
#include "beaconConnection.h"
#include "beaconPath.h"
#include <utilitieslib/utils/utils.h>
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
#include "beacon/beaconFile.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/FolderCache.h>
#include <conio.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/network/netio_core.h>
#include <zlib.h>
#include <utilitieslib/utils/RegistryReader.h>
#include "storyarc/missionspec.h"
#include "dbcomm/staticMapInfo.h"
#include "seq/tricks.h"
#include "group/groupgrid.h"
#include "seq/anim.h"
#include "group/group.h"
#include "gameData/arenastruct.h"
#include "NovodeX/NwWrapper.h"
#include <utilitieslib/utils/sysutil.h>

#define BEACON_SERVER_PORT                    (0xBEAC) // == 48812 :P
#define BEACON_MASTER_SERVER_PORT            (BEACON_SERVER_PORT + 1)
#define BEACON_CLIENT_PROTOCOL_VERSION        (1)
#define BEACON_MASTER_SERVER_DEFAULT_NAME    "prgbcnmasterv01"

typedef enum BeaconizerType {
    BEACONIZER_TYPE_NONE = 0,
    BEACONIZER_TYPE_CLIENT,
    BEACONIZER_TYPE_MASTER_SERVER,
    BEACONIZER_TYPE_AUTO_SERVER,
    BEACONIZER_TYPE_SERVER,
    BEACONIZER_TYPE_REQUEST_SERVER,
} BeaconizerType;

typedef enum BeaconMsgClientToServer {
    BMSG_C2S_FIRST_CMD = COMM_MAX_CMD,
    BMSG_C2S_CONNECT,
    BMSG_C2S_SERVER_CONNECT,
    BMSG_C2S_REQUESTER_CONNECT,
    BMSG_C2S_TEXT_CMD,
} BeaconMsgClientToServer;

extern const char* BMSG_C2ST_READY_TO_WORK;
extern const char* BMSG_C2ST_NEED_MORE_MAP_DATA;
extern const char* BMSG_C2ST_MAP_DATA_IS_LOADED;
extern const char* BMSG_C2ST_NEED_MORE_EXE_DATA;
extern const char* BMSG_C2ST_GENERATE_FINISHED;
extern const char* BMSG_C2ST_BEACON_CONNECTIONS;
extern const char* BMSG_C2ST_SENTRY_CLIENT_LIST;
extern const char* BMSG_C2ST_SERVER_STATUS;
extern const char* BMSG_C2ST_REQUESTER_MAP_DATA;
extern const char* BMSG_C2ST_REQUESTER_CANCEL;
extern const char* BMSG_C2ST_USER_INACTIVE;
extern const char* BMSG_C2ST_BEACON_FILE;
extern const char* BMSG_C2ST_NEED_MORE_BEACON_FILE;
extern const char* BMSG_C2ST_REQUESTED_MAP_LOAD_FAILED;
extern const char* BMSG_C2ST_PING;

typedef enum BeaconMsgServerToClient {
    BMSG_S2C_FIRST_CMD = COMM_MAX_CMD,
    BMSG_S2C_CONNECT_REPLY,
    BMSG_S2C_TEXT_CMD,
} BeaconMsgServerToClient;

extern const char* BMSG_S2CT_KILL_PROCESSES;
extern const char* BMSG_S2CT_MAP_DATA;
extern const char* BMSG_S2CT_MAP_DATA_LOADED_REPLY;
extern const char* BMSG_S2CT_EXE_DATA;
extern const char* BMSG_S2CT_NEED_MORE_BEACON_FILE;
extern const char* BMSG_S2CT_PROCESS_LEGAL_AREAS;
extern const char* BMSG_S2CT_BEACON_LIST;
extern const char* BMSG_S2CT_CONNECT_BEACONS;
extern const char* BMSG_S2CT_TRANSFER_TO_SERVER;
extern const char* BMSG_S2CT_CLIENT_CAP;
extern const char* BMSG_S2CT_STATUS_ACK;
extern const char* BMSG_S2CT_REQUEST_CHUNK_RECEIVED;
extern const char* BMSG_S2CT_REQUEST_ACCEPTED;
extern const char* BMSG_S2CT_PROCESS_REQUESTED_MAP;
extern const char* BMSG_S2CT_EXECUTE_COMMAND;
extern const char* BMSG_S2CT_BEACON_FILE;
extern const char* BMSG_S2CT_REGENERATE_MAP_DATA;
extern const char* BMSG_S2CT_PING;

typedef struct SentModelInfo {
    Model*    model;
    S32        index;
    S32        sentModel;
} SentModelInfo;

typedef struct BeaconClientConnection {
    U32            timeHeardFromServer;
    U32            readyToWork                : 1;
} BeaconClientConnection;

extern BeaconClientConnection beacon_client_conn;

typedef struct BeaconMapDataPacket BeaconMapDataPacket;

// beaconClientServer.c ---------------------------------------------------------------------------

#define COLOR_YELLOW    (COLOR_RED|COLOR_GREEN)
#define COLOR_WHITE        (COLOR_RED|COLOR_GREEN|COLOR_BLUE)

HWND        beaconGetConsoleWindow(void);
void         beaconCheckDuplicates(BeaconDiskSwapBlock* block);
void         beaconInitCommon(S32 initGrid);
void         beaconInitGenerating(S32 quiet);
void         beaconTestCollision(void);
char*        beaconGetLinkIPStr(NetLink* link);
void         beaconVerifyUncheckedCount(BeaconDiskSwapBlock* block);
void         beaconReceiveColumnAreas(Packet* pak, BeaconLegalAreaCompressed* area);
void         beaconResetMapData(void);
void        beaconVprintf(S32 color, const char* format, va_list argptr);
void         beaconPrintfDim(S32 color, const char* format, ...);
void         beaconPrintf(S32 color, const char* format, ...);
char*        beaconGetExeFileName(void);
S32         checkForCorrectExePath(const char* exePrefix, const char* cmdLineParams, S32 earlyMutexRelease, S32 hideNewWindow);
U32         beaconGetExeCRC(U8** outFileData, U32* outFileSize);
U8*         beaconFileAlloc(const char* fileName, U32* fileSize);
S32         beaconDeleteOldExes(const char* exePrefix, S32* attemptCount);
S32         beaconStartNewExe(const char* exePrefix, U8* data, U32 size, const char* cmdLineParams, S32 exitWhenDone, S32 earlyMutexRelease, S32 hideNewWindow);
void         beaconHandleNewExe(Packet* pak, const char* exeName, const char* cmdLineParams, S32 earlyMutexRelease, S32 hideNewWindow);
void         beaconFreeUnusedMemoryPools(void);
S32            beaconCreateNewExe(const char* path, U8* data, U32 size);
void         beaconReleaseAndCloseMutex(HANDLE* mutexPtr);
void         beaconPrintMemory(void);
void*        beaconMemAlloc(const char* module, U32 size);
void        beaconMemFree(void** memVoid);
char*        beaconStrdup(const char* str);
S32            beaconEnterString(char* buffer, S32 maxLength);
U32            beaconGetCurTime(void);
U32            beaconTimeSince(U32 startTime);
void        beaconResetReceivedMapData(void);
void        beaconMapDataPacketSendChunk(Packet* pak, BeaconMapDataPacket* mapData, U32* sentByteCount);
void        beaconMapDataPacketReceiveChunkHeader(Packet* pak, BeaconMapDataPacket* mapData);
S32            beaconMapDataPacketIsFirstChunk(BeaconMapDataPacket* mapData);
void        beaconMapDataPacketCopyHeader(BeaconMapDataPacket* to, const BeaconMapDataPacket* from);
void        beaconMapDataPacketReceiveChunkData(Packet* pak, BeaconMapDataPacket* mapData);
void        beaconMapDataPacketReceiveChunk(Packet* pak, BeaconMapDataPacket* mapData);
void        beaconMapDataPacketSendChunkAck(Packet* pak, BeaconMapDataPacket* mapData);
S32            beaconMapDataPacketReceiveChunkAck(Packet* pak, U32 sentByteCount, U32* receivedByteCount);
S32            beaconMapDataPacketIsFullyReceived(BeaconMapDataPacket* mapData);
S32            beaconMapDataPacketIsFullySent(BeaconMapDataPacket* mapData, U32 sentByteCount);
U32            beaconMapDataPacketGetSize(BeaconMapDataPacket* mapData);
U8*            beaconMapDataPacketGetData(BeaconMapDataPacket* mapData);
void        beaconMapDataPacketWriteFile(BeaconMapDataPacket* mapData, const char* fileName, const char* uniqueStorageName, U32 timeStamp);
S32            beaconMapDataPacketReadFile(BeaconMapDataPacket* mapData, const char* fileName, char** uniqueStorageName, U32* timeStamp, S32 headerOnly);
U32            beaconMapDataPacketGetReceivedSize(BeaconMapDataPacket* mapData);
void        beaconMapDataPacketDiscardData(BeaconMapDataPacket* mapData);
void        beaconMapDataPacketCreate(BeaconMapDataPacket** mapData);
void        beaconMapDataPacketDestroy(BeaconMapDataPacket** mapData);
S32            beaconMapDataPacketToMapData(BeaconMapDataPacket* mapData);
void        beaconMapDataPacketFromMapData(BeaconMapDataPacket** mapData, S32 forceLargeModelData);
U32            beaconMapDataPacketGetCRC(BeaconMapDataPacket* mapData);
S32            beaconMapDataPacketIsSame(BeaconMapDataPacket* mapData1, BeaconMapDataPacket* mapData2);
void        beaconMapDataPacketClearInitialBeacons(void);
void        beaconMapDataPacketAddInitialBeacon(Vec3 pos, S32 isValidStartingPoint);
void        beaconMapDataPacketInitialBeaconsToRealBeacons(void);
void        beaconHandleCmdLine(S32 argc, char** argv);
S32            beaconizerIsStarting(void);
void        beaconizerStart(void);
S32            beaconIsProductionMode(void);
void        beaconGetCommonCmdLine(char* buffer);

// beaconClient.c ---------------------------------------------------------------------------------

void        beaconClientReleaseSentryMutex(void);
void        beaconClientSetWorkDuringUserActivity(S32 set);
void        beaconClient(const char* masterServerName, const char* subServerName);
Packet*        beaconClientCreatePacket(NetLink* link, const char* textCmd);
void        beaconClientSendPacket(NetLink* link, Packet** pak);

#define BEACON_CLIENT_PACKET_CREATE_BASE(pak, link, textCmd){                            \
            Packet* pak;                                                                \
            Packet* clientPacket__ = pak = beaconClientCreatePacket(link, textCmd);        \
            NetLink* clientNetLink__ = link
#define BEACON_CLIENT_PACKET_CREATE_TO_LINK(link, textCmd)                                \
            BEACON_CLIENT_PACKET_CREATE_BASE(pak, link, textCmd)
#define BEACON_CLIENT_PACKET_CREATE(textCmd)                                            \
            BEACON_CLIENT_PACKET_CREATE_TO_LINK(&beacon_client.serverLink, textCmd)
#define BEACON_CLIENT_PACKET_SEND()                                                        \
            beaconClientSendPacket(clientNetLink__, &clientPacket__);                    \
        }

// beaconServer.c ---------------------------------------------------------------------------------

void        beaconServerSetDataToolsRootPath(const char* dataToolsRootPath);
const char* beaconServerGetDataToolsRootPath(void);
const char* beaconServerGetDataPath(void);
const char* beaconServerGetToolsPath(void);
void        beaconServerSetRequestCacheDir(const char* cacheDir);
void        beaconServerSetSymStore(void);
void        beaconServer(BeaconizerType beaconizerType, const char* masterServerName, S32 noNetStart, S32 onePassOnly, S32 forceRebuild);
void        beaconRequestBeaconizing(const char* uniqueStorageName);
void        beaconRequestUpdate(void);
void        beaconRequestSetMasterServerAddress(const char* address);

// ------------------------------------------------------------------------------------------------
