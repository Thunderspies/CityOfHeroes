// CmdRelay.c - recieves commands from a Server Monitor and executes them on a local machine

#include <process.h>
#include <utilitieslib/network/netio.h>
#include "dbserver/servercfg.h"
#include "comm_backend.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/network/sock.h>
#include <stdio.h>
#include <conio.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/network/netio_core.h>
#include "relay_actions.h"
#include "relay_util.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/version/AppRegCache.h>
#include <utilitieslib/utils/log.h>


NetLink        g_serverMonitorLink;
static char g_serverMonitor[256];
CmdRelayCmdStatus g_status;
CRITICAL_SECTION g_criticalSection;
BOOL g_cancelThread = FALSE;
BOOL g_workerThreadRunning = FALSE;
BOOL g_bDebugZeroProtocol = FALSE;
BOOL g_bDebugOutput = FALSE;
char gStopAllScript[FILENAME_MAX];
char gStartAllScript[FILENAME_MAX];
char g_patch_projects[2048];
int g_relayType = RELAY_TYPE_MAPSERVER;
char g_launcherFlags[1024] = {0};
char g_workingDir[MAX_PATH] = {0};
int g_are_executables_in_bin=0;

void SendResultToServer(int res)
{
    Packet    *pak;
    pak = pktCreate();
    pktSendBitsPack(pak,1,res);
    pktSend(&pak,&g_serverMonitorLink);
    printf("Sent result (%d) to server monitor\n", res);
}


typedef struct {
    int cmd;
    char * data;
}workerThreadParams;

static unsigned __stdcall workerThreadMain(void *data) {
    
    workerThreadParams * params = (workerThreadParams*) data;
    BOOL res = FALSE;

    printf("Starting worker thread\n");


    switch(params->cmd)
    {
        xcase CMDRELAY_REQUEST_APPLY_PATCH:
            res = onApplyPatch(params->data);
        xcase CMDRELAY_REQUEST_CUSTOM_CMD:
            res = onExecuteCustomCommand(params->data);
        xcase CMDRELAY_REQUEST_RUN_BATCH_FILE:
            res = onExecuteBatchFile(params->data);

        break;
        default:
            printf("Unknown command %d passed to workerThreadMain()\n",params->cmd);
    }

    if(res)
    {
        printf("Success!\n");
        SetStatus(CMDRELAY_CMDSTATUS_READY, 0);
        SendResultToServer(CMDRELAY_ANSWER_ACTION_SUCCESS);
    }
    else
    {
        printf("Command failed!\n");
        SendResultToServer(CMDRELAY_ANSWER_ACTION_FAILURE);
    }
    
    free(params->data);
    free(params);    // was allocated before thread was created

    // not sure if this next line needs to be in crit. section
    EnterCriticalSection(&g_criticalSection);
    g_cancelThread = FALSE;
    g_workerThreadRunning = FALSE;
    LeaveCriticalSection(&g_criticalSection);

    printf("Exiting worker thread...\n");
    return 0;
}


// assumes that 'data' was allocated on heap
void relayWorkerThreadBegin(int cmd, char * data) {
    int ret;
    workerThreadParams * params;

    if (g_workerThreadRunning) {
        printf("Thread already running\n");
        return;
    }

    g_workerThreadRunning = TRUE;

    // will be freed by thread
    params = malloc(sizeof(workerThreadParams));
    params->cmd            = cmd;
    params->data        = data;

    ret = _beginthreadex(NULL,0,workerThreadMain, params,0,NULL);
}

static int cmdRelayHandleMsg(Packet *pak,int cmd, NetLink *link)
{
    switch(cmd)
    {
        xcase CMDRELAY_REQUEST_STATUS:
            onSendStatus(link);
            return 1;
        xcase CMDRELAY_REQUEST_APPLY_PATCH:
        case CMDRELAY_REQUEST_CUSTOM_CMD:
        {    
            char * str = strdup(pktGetString(pak));    // this will be deallocated by the thread
            relayWorkerThreadBegin(cmd, str);
        }
        xcase CMDRELAY_REQUEST_RUN_BATCH_FILE:
        {
            FILE * file;
            int count;
            char filename[2000];
            char buf[2000];
            int size = pktGetBitsPack(pak, 1);
            char * data = malloc(sizeof(char) * size);
            char * commandStr;
            pktGetBitsArray(pak, size * 8, data);

            sprintf(filename, "%s\\batch_file.bat", getExecutableDir(buf));
            file = fopen(filename, "wb");
            count = fwrite(data, sizeof(char), size, file);
            fclose(file);
            free(data);

            if(count != size)
            {
                SetError("Error writing file to disk");
                return FALSE;
            }

            if(!fileExists(filename))
            {
                SetError("Unknown error - batch file doesn't exist, but should!");
                return FALSE;
            }

            commandStr = strdup(filename);    // this will be deallocated by thread
            relayWorkerThreadBegin(CMDRELAY_REQUEST_RUN_BATCH_FILE, commandStr);
        }
        xcase CMDRELAY_REQUEST_START_ALL:
            onStartAll();
        xcase CMDRELAY_REQUEST_STOP_ALL:
            onStopAll();
        xcase CMDRELAY_REQUEST_KILL_ALL_LAUNCHER:
            onKillAllLauncher();
        xcase CMDRELAY_REQUEST_KILL_ALL_MAPSERVER:
            onKillAllMapsever();
        xcase CMDRELAY_REQUEST_START_LAUNCHER:
            onStartLauncher();
        xcase CMDRELAY_REQUEST_START_DBSERVER:
            onStartDbServer();
        xcase CMDRELAY_REQUEST_CANCEL_ALL:
            onCancelAll();
        xcase CMDRELAY_REQUEST_UPDATE_SELF:
            onUpdateSelf(pak);
        xcase CMDRELAY_REQUEST_PROTOCOL:
            onSendProtocol(link);
        xcase CMDRELAY_REQUEST_START_ARENASERVER:
            onStartArenaServer();
        xcase CMDRELAY_REQUEST_START_STATSERVER:
            onStartStatServer();
        xcase CMDRELAY_REQUEST_KILL_ALL_ARENASERVER:
            onKillAllArenaServer();
        xcase CMDRELAY_REQUEST_KILL_ALL_STATSERVER:
            onKillAllStatServer();
            break;
        default:
            printf("Unknown command %d\n",cmd);
            return 0;
    }

    if(!g_workerThreadRunning)
    {
        SetStatus(CMDRELAY_CMDSTATUS_READY, 0);
    }

    return 1;
}

int connectSvrMon()
{
    int        ret;
    loadstart_printf("Connecting to %s...", g_serverMonitor);
#undef fflush
    fflush(stdout);
    ret = netConnect(&g_serverMonitorLink, g_serverMonitor,DEFAULT_CMDRELAY_PORT,NLT_TCP,NO_TIMEOUT,0);

    if (ret) {
        loadend_printf("connected");
        NMAddLink(&g_serverMonitorLink, cmdRelayHandleMsg);
    } else {
        loadend_printf("failed");
    }
    return ret;
}

int main(int argc,char **argv)
{
    int        i;
    char buf[1000];
    int count = 0;
    BOOL bSendProtocol = TRUE;
    BOOL bNoTemp = FALSE;
    int exclude_coh_debug = 0, exclude_coh_server = 0;
    g_patch_projects[0] = 0;

    EXCEPTION_HANDLER_BEGIN
        setAssertMode(ASSERTMODE_MINIDUMP | ASSERTMODE_DEBUGBUTTONS);

    memMonitorInit();

    for(i = 0; i < argc; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");


    InitializeCriticalSection(&g_criticalSection);

    logSetDir("cmdrelay");
    sockStart();

    if (fileExists("./bin/Ouroboros.exe"))
    {
        g_are_executables_in_bin = 1;
        strcpy(g_workingDir, ".");
    }
    else if (fileExists("./Ouroboros.exe"))
    {
        g_are_executables_in_bin = 0;
        strcpy(g_workingDir, ".");
    }
    else
    {
        SetWorkingDirectory();
    }

    printf("Protocol version %d\n", CMDRELAY_PROTOCOL_VERSION);


    for(i=1;i<argc;i++)
    {
        if (stricmp(argv[i],"-svrmon")==0)
            strcpy(g_serverMonitor,argv[++i]);
        else if(!stricmp(argv[i], "-dbserver"))
        {
            g_relayType = RELAY_TYPE_DBSERVER;        // default is RELAY_TYPE_MAPSERVER
        }
        else if(!stricmp(argv[i], "-arenaserver"))
        {
            g_relayType = RELAY_TYPE_ARENASERVER;        // default is RELAY_TYPE_MAPSERVER
        }
        else if(!stricmp(argv[i], "-statserver"))
        {
            g_relayType = RELAY_TYPE_STATSERVER;
        }
        else if(!stricmp(argv[i], "-logserver"))
        {
            g_relayType = RELAY_TYPE_LOGSERVER;        // default is RELAY_TYPE_MAPSERVER
        }
        else if(!stricmp(argv[i], "-custom"))
        {
            g_relayType = RELAY_TYPE_CUSTOM;        // default is RELAY_TYPE_MAPSERVER
        }
        else if( ( i+1 < argc) && !stricmp(argv[i], "-startall"))
        {
            strncpyt(gStartAllScript, argv[i+1], sizeof(gStartAllScript));
            ++i;
        }
        else if( ( i+1 < argc) && !stricmp(argv[i], "-stopall"))
        {
            strncpyt(gStopAllScript, argv[i+1], sizeof(gStopAllScript));
            ++i;
        }
        else if(!stricmp(argv[i], "-notemp"))
        {
            bNoTemp = TRUE;                            // won't copy to temp dir (for debugging)
        }
        else if(!stricmp(argv[i], "-zeroprotocol"))
        {
            g_bDebugZeroProtocol = TRUE;            // won't copy to temp dir (for debugging)
        }
        else if((i+1 < argc) && (stricmp(argv[i], "-includeproject")==0))
        {
            sprintf(g_patch_projects, "%s -project \"%s\"", g_patch_projects, argv[++i]);
        }
        else if((i+1 < argc) && (stricmp(argv[i], "-excludeproject")==0))
        {
            ++i;
            if (stricmp(argv[i], "coh debug"))
                exclude_coh_debug = 1;
            else if (stricmp(argv[i], "coh server"))
                exclude_coh_server = 1;
        }
        else if((i+1 < argc) && 0 == stricmp(argv[i], "-launcherServers"))
        {
            i++;
            strcat(g_launcherFlags, "-servers ");
            for(;i<argc && argv[i] && argv[i][0] != '-'; ++i){
                strcat(g_launcherFlags, argv[i]);
                strcat(g_launcherFlags, " ");
            }
        }
        else if((i+1 < argc) && 0 == stricmp(argv[i], "-setworkingdir"))
        {
            i++;
            if( SetCurrentDirectoryA(argv[i]) )
            {
                strcpy(g_workingDir,argv[i]);
                printf("overriding install directory to '%s'\n", argv[i]);
            }
            else
            {
                printf("couldn't set working dir to %s\n", argv[i]);
            }
        }
    }

    if (!exclude_coh_debug)
        sprintf(g_patch_projects, "%s -project \"coh debug\"", g_patch_projects);

    if (!exclude_coh_server)
        sprintf(g_patch_projects, "%s -project \"coh server\"", g_patch_projects);


    if(g_relayType == RELAY_TYPE_CUSTOM)
    {        
        if(fileExists(gStopAllScript))
            printf("Using custom stop-all script %s\n", gStopAllScript);
        else
            gStopAllScript[0] = 0;

        if(fileExists(gStartAllScript))
            printf("Using custom start-all script %s\n", gStartAllScript);
        else
            gStartAllScript[0] = 0;
    }
        
    // copy self to a temporary directory if necessary - this avoids problems with
    // CohUpdater attempting to patch a running CmdRelay.exe 
    if(    ! filePathCompare(getExecutableDir(buf), regGetInstallationDir())
        || ! filePathCompare(getExecutableDir(buf), "C:\\util"))    // for debugging
    {
        if(!bNoTemp)
        {
            if(! copyAndRestartInTempDir())
            {
                SetError("Unable to start new version of CmdRelay?");    
            }
        }
    }


    loadstart_printf("Networking startup...");
    packetStartup(0,0);

    g_status = CMDRELAY_CMDSTATUS_READY;

    for(;;)
    {
        NMMonitor(2000);

        if (!g_serverMonitorLink.connected)
        {
            connectSvrMon();
            bSendProtocol = TRUE;
        }
        if(bSendProtocol)
        {
            onSendProtocol(&g_serverMonitorLink);    // this could probably be moved up to where it actually connects above
            bSendProtocol = FALSE;
        }
        else
        {
            onSendStatus(&g_serverMonitorLink);
        }

        sprintf(buf, "(%d) CmdRelay - %s mode", count, StaticDefineIntRevLookup(cmdRelayTypeDef, g_relayType));
        setConsoleTitle(buf);
        count = (count + 1) % 10;
    }
    EXCEPTION_HANDLER_END
}
