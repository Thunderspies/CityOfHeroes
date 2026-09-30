#include <assert.h>
#include <direct.h>

#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/fpmacros.h>
#include <utilitieslib/utils/FileSystem.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/utils/file.h>

#include "dbcomm/dbcomm.h"
#include "cmdparse/cmdserver.h"
#include "svr_base.h"
#include "svr_player.h"
#include "svr_tick.h"
#include "dbcomm/dbquery.h"
#include "entity/entity.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/log.h>

#include "dbcomm/characterRestore.h"
#include "comm_backend.h"

#include "entity/origins.h"
#include "entity/classes.h"


#define loadstart_printf if (!server_state.silent) loadstart_printf
#define loadend_printf if (!server_state.silent) loadend_printf

ServerState server_state;

void setSignal()
{
#ifdef GNUC
    signal(SIGPIPE,SIG_IGN);
#endif
}

static char program_parms[1000];

static void __cdecl at_exit_func(void)
{
    extern int glob_assert_file_err;

    printf_stderr("Quitting: %s\n", program_parms);
    logShutdownAndWait();
    if (glob_assert_file_err)
        assert(0);
}

static void svrInit()
{
    netLinkListAlloc(&net_links,100,sizeof(ClientLink),svrClientCallback);
    net_links.destroyCallback = svrNetDisconnectCallback;
    if (!server_state.noEncryption)
        net_links.encrypted = 1;
    if (server_state.udp_port < 0)
    {
        for(server_state.udp_port = BASE_MAPSERVER_PORT;;server_state.udp_port++)
        {
            if (netInit(&net_links,server_state.udp_port,server_state.tcp_port))
                break;
        }
    }
    else
        netInit(&net_links,server_state.udp_port,server_state.tcp_port);
    NMAddLinkList(&net_links, svrHandleClientMsg);

}

static void startupInfo(int argc,char **argv)
{
    int        i;
    char    buf[1000],buffer[MAX_PATH];

    for(i=0;i<argc;i++)
    {
        strcat(program_parms,argv[i]);
        strcat(program_parms," ");
    }

    getcwd(buffer, MAX_PATH);

    if (!server_state.silent) {
        printf(    "(%d) %s %s\n",_getpid(),getExecutableName(),program_parms);
        printf( "working dir: %s\n", buffer);
    }

    consoleInit(110, 128, 0); // A width of 110 will fit nicely on most resolutions, and be wide enough for us to see stuff

    logSetDir("mapserver");
    printf_stderr("Starting: %s\n",program_parms);

    sprintf(buf, "%d: ", _getpid());

    for(i=0;i<argc;i++)
    {
        strcat(buf,argv[i]);
        strcat(buf," ");
    }

    setConsoleTitle(buf);
}

static void sysInit() // non game-specific initializations
{
    fileSystemExclude("texture_library", 0); // currently accepts only one of these
    fileDataDir();
    loadend_printf("Caching directory tree and misc startup...");

    if(server_state.beaconProcessOnLoad)
        return;

    loadstart_printf("Networking startup.. ");
    timerCpuSpeed();
    atexit( at_exit_func );

#ifdef _DEBUG
    // Bring up the Abort/Retry/Ignore window on assertion instead of outputting to console.
    _set_error_mode(_OUT_TO_MSGBOX);
#endif

    sockStart();
    SET_FP_CONTROL_WORD_DEFAULT;
    packetStartup(0,server_state.noEncryption?0:1);
    setSignal();

    loadend_printf("done");
}

static void parseArgs1(int argc,char **argv)
{
    int        i;

    for(i=1;i<argc;i++)
    {
        int handled = 1;
        int start_i = i;

        if (strcmp(argv[i], "-noencrypt")==0)
        {
            server_state.noEncryption = 1;
        }
        else if (strcmp(argv[i], "-nopigs")==0)
        {
            fileSystemSetMode(FILE_MODE_LOOSE);
        }
        else if (strcmp(argv[i], "-silent")==0)
        {
            // Just silent enough for dbquery to produce clean output, feel free to make it more silent elswhere
            server_state.silent = 1;
        }
        else if (strcmp(argv[i], "-dbquery")==0 || strcmp(argv[i], "-getcharacter")==0 || strcmp(argv[i], "-putcharacter")==0)
        {
            // We want these two options by default for -dbquery to make them fast!
            handled = 0;
            fileSystemSetMode(FILE_MODE_LOOSE);
            server_state.noEncryption = 1;
            server_state.silent = 1;
        }
        else if(strcmp(argv[i], "-tsr2")==0 ||
                strcmp(argv[i], "-tsr3")==0 ||
                strcmp(argv[i], "-charactertransfer")==0 ||
                strcmp(argv[i], "-templates")==0 )
        {
            // Disable encryption/Networking Startup on these commands
            handled = 0;
            server_state.noEncryption = 1;
        }
        else
        {
            handled = 0;
        }

        if(handled)
        {
            // Invalidate handled parameters in such a way that they won't cause errors in parseArgs2.

            while(start_i <= i)
            {
                argv[start_i++][0] = 0;
            }
        }
    }
}

static void parseArgs2(int argc,char **argv)
{
    int        i;
    int        ret;

    db_state.local_server = 1;
    for(i=1;i<argc;i++)
    {
#define ISCMD(CMD,N_ARGS)                            \
    (0==stricmp(strchrRemoveStatic(argv[i],'_'),CMD) \
     && (i+N_ARGS) < argc)

        if (strcmp(argv[i],"-db")==0)
        {
            strcpy(db_state.server_name,argv[++i]);
        }
        else if (strcmp(argv[i],"-getcharacter")==0)
        {
            if (i+1 < argc) {
                if (i+2 < argc) {
                    ret = dbQueryGetCharacter(argv[i+1], argv[i+2]);
                    i+=2;
                } else {
                    ret = dbQueryGetCharacter(argv[++i], NULL);
                }
            } else {
                printf("Expected: -getcharacter DBID [newauthname]\n");
                ret = 2;
            }
            exit(ret);
        }
        else if (strcmp(argv[i],"-putcharacter")==0)
        {
            char *filename=NULL;
            if ( i+1 < argc) {
                filename = argv[i+1];
            }
            ret = dbQueryPutCharacter(filename);
            exit(ret);
        }
        else if (strcmp(argv[i],"-dbquery")==0)
        {
            int ret = dbQueryWrapper(argc-i-1,argv+i+1);
            exit(ret);
        }
        else if (ISCMD("-querysgleaders",1))
        {
            int ret = dbQueryGetSgLeaders(argv[1+i]);
            exit(ret);
        }
        else if (strcmp(argv[i],"-udp")==0)
            server_state.udp_port = atoi(argv[++i]);
        else if (strcmp(argv[i],"-cookie")==0)
            db_state.cookie = atoi(argv[++i]);
        else if (strcmp(argv[i],"-tcp")==0)
            server_state.tcp_port = atoi(argv[++i]);
        else if (strcmp(argv[i],"-verbose")==0)
            errorSetVerboseLevel(1);
        else if (strcmp(argv[i], "-nodebug")==0)
            server_state.nodebug = 1;
        else if (strcmp(argv[i], "-notimeout")==0)
            server_state.notimeout = 1;
        else if (strcmp(argv[i], "-packetdebug")==0)
        {
            pktSetDebugInfo();
        }
        else if (strcmp(argv[i], "-restorecharacter")==0)
        {
            server_state.restorecharacter = 1;
        }
        else if (argv[i][0])
        {
            Errorf("Invalid command line parameter passed to mapserver.exe: %s", argv[i]);
        }
    }
}

static void serverStateInit(int argc,char **argv)
{
    dbCommStartup();
    server_state.udp_port = DEFAULT_MAPSERVER_PORT;
    strcpy(db_state.server_name,"localhost");
    parseArgs2(argc,argv);
}

int __cdecl main(int argc,char **argv)
{
    int        startup_timer = 0;

    memCheckInit();

    EXCEPTION_HANDLER_BEGIN

    startup_timer = timerAlloc();

    // Do not perform any file io until fileSystemChooseMode() has been called.
    //        Otherwise, the game will fail to find all of its data files.
    fileSystemChooseMode();
    fileSystemCallbacksEnabled(0);

    fileSystemSetMode(FILE_MODE_LOOSE);
    server_state.noEncryption = 1;
    server_state.silent = 1;

    parseArgs1(argc,argv); // Put this *before* startupInfo so that -nopigs can disable loading of .pig files
    startupInfo(argc,argv);
    sysInit();
    serverStateInit(argc,argv);

    if (server_state.restorecharacter)
    {
        characterRestore();
        exit(0);
    }

    EXCEPTION_HANDLER_END
}

// Externs
int last_db_error_code;
char last_db_error[256];
PlayerEnts player_ents[PLAYER_ENTTYPE_COUNT];
//Entity *entities[MAX_ENTITIES_PRIVATE];
U8 entity_state[MAX_ENTITIES_PRIVATE];
int entities_max;
EntityInfo entinfo[MAX_ENTITIES_PRIVATE];

typedef struct ContainerStore ContainerStore;

U32 dealWithContainer(ContainerInfo *ci,int type) { return 0; }
char* teamTypeName(ContainerType type) { return ""; }
char *dbg_FillNameStr(char *pchDest, Entity *e) { return ""; }
StaticMapInfo* staticMapInfoFind(int mapID) { return NULL; }
Entity *entFromDbId(int db_id) { return NULL; }

void cmdOldServerHandleRelay(char *msg) {;}
void dbGatherMapLinkInfo() {;}
void dbNameTableInit() {;}
void forceLogout(int db_id,int reason) {;}
void svrLogPerf(void) {}
void logSendDisconnect(F32 timeout) {}
void handleClearPnameCacheEntry(Packet *pak) {;}
void handleDbNewMapLoading(Packet *pak) {;}
void handleDbNewMapReady(Packet *pak) {;}
void handleDbNewMap(Packet *pak) {;}
void handleDbNewMapFail(Packet *pak) {;}
void handleForceLogout(Packet *pak) {;}
void handleMapRegisterCallback(Packet *pak) {;}
void handleSendEntNames(Packet *pak) {;}
void handleSendGroupNames(Packet *pak) {;}
void handleServerDoorUpdate(Packet *pak) {;}
void MissionRequestShutdown() {;}
void returnAllPlayers(int disconnect,char *msg) {;}
void sendDoorsToDbServer(int is_static) {;}
void serverSynchTime(void) {;}
void staticMapInfoResetPlayerCount() {;}
void teamlogPrintf(const char* func, const char* s, ...) {;}
void containerCacheAdd(int list_id,int container_id,char *data) {;}
typedef struct StoryArc StoryArc;
void MissionSetInfoString(char* info, StoryArc *pArc) {;}
void ArenaSetInfoString(char* info) {;}
void SGBaseSetInfoString(char* info) {;}
void RaidSetInfoString(char *info) {}
void EndGameRaidSetInfoString(char *info) {}
void dealWithLostMembership(int list_id, int container_id, int ent_id) {;}
void dealWithBroadcast(int container,int *members,int count,char *msg,int msg_type,int senderID,int container_type) {;}
void dealWithEntAck(int list_id,int id) {;}
void dealWithShardAccountAck(int list_id,int id) {;}
void containerCacheSendDiff(Packet *pak,int list_id,int container_id,char *data,int diff_debug) 
{
    pktSendBits(pak,1,1);
    pktSendBits(pak,1,0);
    pktSendString(pak,data);
}
void mapGroupInit(char* uniquename) {;}
void handleArenaAddress(Packet *pak) {;}
void getAuthId(Entity *e) {;}
void handleSgChannelInviteHelper(Packet *pak) {;}
SHARED_MEMORY CharacterClasses g_CharacterClasses;
SHARED_MEMORY CharacterOrigins g_CharacterOrigins;
int classes_GetIndexFromName(const CharacterClasses *pclasses, const char *pch) {return 0;}
int origins_GetIndexFromName(const CharacterOrigins *porigins, const char *pch) {return 0;}
void dealWithContainerRelay(Packet* pak, U32 cmd, U32 listid, U32 cid) {;}
void dealWithContainerReceipt(U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data, U32 err, char* str, Packet* pak) {;}
void dealWithContainerReflection(int list_id, int* ids, int deleting, Packet* pak, ContainerReflectInfo** reflectinfo) {;}
void cstoreUnpack(ContainerStore* cstore, void* container, char* mem, U32 cid) {;}
void* cstoreGetAdd(ContainerStore* cstore, U32 cid, int add) {return NULL;}
void beaconRequestSetMasterServerAddress(char* serverAddress){;}
void handleDbBackupSearch(Packet *pak){;}
void handleDbBackupApply(Packet *pak){;}
void handleDbBackupView(Packet *pak){;}
void dbLog(char const *cmdname,Entity *e,char const *fmt, ...){}
