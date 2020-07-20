/*
 * Launcher - starts, tracks, and stops processes (mapservers) for dbserver
 * 
 * Supported commands:
 * 
 *     start process
 *     get processes

connection plan:

    launcher starts up, looks for dbserver
 */

#include <utilitieslib/network/netio.h>
#include "dbserver/servercfg.h"
#include "comm_backend.h"
#include "proclist.h"
#include "monitor.h"
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
#include <utilitieslib/version/AppVersion.h>
#include <utilitieslib/components/SharedHeap.h>
#include "Launcher_enum.h"
#include <utilitieslib/utils/cpu_count.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/version/AppRegCache.h>
#include <utilitieslib/utils/RegistryReader.h>
#include <utilitieslib/utils/winutil.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/log.h>

//#define LAUNCHER_TRACKS_SHARED_HEAP

typedef struct LauncherShard {
    char *name;
    NetLink db_link;
    int retryTimer;
} LauncherShard;

static LauncherShard shards[MAX_DBSERVER];

#define MAX_CLIENTS 50
NetLinkList net_links;

static int local_launcher;
static int version_match=0;
static char dbserver_version[1024];
static int launcher_no_version_check=0;
static bool s_is_launcher_suspended = false;
static int g_process_starts = 0;    // total launch requests handled by launcher    

static int launcherHandleMsg(Packet *pak,int cmd, NetLink *link);

static void printf_timestamp(char *fmt, ...)
{
    char date_buf[100];
    va_list argptr;

    timerMakeDateString(date_buf);
    printf("%s ", date_buf);

    va_start(argptr, fmt);
    vprintf(fmt, argptr);
    va_end(argptr);
}

// Launcher can request that dbserver place it in the 'suspended' state.
// When in the suspended state the dbserver will no longer attempt to launch
// maps on the host machine however the launcher will still report stats
// on the processes already running
static void setSuspendedState(bool suspend)
{
    bool was_suspended = s_is_launcher_suspended;

    // if we are already in that state nothing to do
    if (suspend == was_suspended)
        return;

    // monitor only launchers can't launch anyway
    if (local_launcher && suspend)
    {
        printf("error: monitor only launchers cannot be suspended.\n");
        return;
    }

    s_is_launcher_suspended = suspend;

    if (s_is_launcher_suspended)
    {
        printf_timestamp("** SUSPENDING LAUNCHER ** - press 's' again to end suspension\n");
        setConsoleTitle("Launcher  -** SUSPENDED **--");
    }
    else
    {
        printf_timestamp("launching has been resumed.\n");
        setConsoleTitle("Launcher");
    }
}

static void toggleSuspension(void)
{
    setSuspendedState( !s_is_launcher_suspended );
}

bool is_launcher_suspended()
{
    return s_is_launcher_suspended;
}

void handleGameVersion(Packet *pak)
{
    version_match = pktGetBits(pak, 1);
    strncpyt(dbserver_version, pktGetString(pak), ARRAY_SIZE(dbserver_version));
}

// handle control message from dbserver and update settings accordingly
static void handleControl(Packet* pak)
{
    int suspension_state = pktGetBits(pak, 1);
    setSuspendedState( suspension_state );
}

// Start a connection attempt to a Dbserver.  This is non-blocking.
static void connectDbStart(char *dbServerName, NetLink *netLink)
{
    printf_timestamp("Starting connection to %s\n", dbServerName);
    netConnectAsync(netLink,dbServerName,DEFAULT_DBLAUNCHER_PORT,NLT_TCP,5.0f);
}

// Once a connection attempt has been started, call this each loop to see if the connection has succeeded.
static int connectDbPoll(char *dbServerName, NetLink *netLink)
{
    int ret;
    Packet *pak;

    ret = netConnectPoll(netLink);

    // If anything other than success ....
    if (ret != 1)
    {
        // -1 means failure
        if (ret == -1)
        {
            // Print a message.  The NetLink will be left in a disconnected state, which we detct in the main loop and react as needed.
            printf_timestamp("Connection to %s failed\n", dbServerName);
        }
        // Anything else (i.e. zero) means it's still in progress so do nothing.
        return ret;
    }

    if (local_launcher)
    {
        Packet    *pak;

        pak = pktCreateEx(netLink, LAUNCHERANSWER_LOCALONLY);
        pktSend(&pak,netLink);
    }

    version_match = 0;
    pak = pktCreateEx(netLink, LAUNCHERANSWER_GAMEVERSION);
    pktSendString(pak, getCompatibleGameVersion());
    pktSendBitsPack(pak, 1, getNumRealCpus());
    pktSend(&pak,netLink);

    // This is a blocking call, but unless there's a major problem, it shouldn't take more than a second or two.
    // Timeout is a value in milliseconds - line 810 of net_link.c makes it so:
    //     fTimeout = (float)timeout / 1000.0f;
    ret = netLinkMonitorBlock(netLink, LAUNCHERQUERY_GAMEVERSION, launcherHandleMsg, 10000);
    if (ret)
    {
        if (version_match || launcher_no_version_check)
        {
            printf_timestamp("Connected to %s\n", dbServerName);
            NMAddLink(netLink, launcherHandleMsg);
        }
        else
        {
            printf_timestamp("Version mismatch Local version: %s\n  DbServer @ %s version: %s\n", getCompatibleGameVersion(), dbServerName, dbserver_version);
            clearNetLink(netLink);
            // Make this dbserver's name the empty string, which will cause us to start ignoring it.  Mismatched versions
            // should never happen anyway, and this will cause the dbserver in question to have a hard time starting maps,
            // which in turn will get the attention of the NOC.
            dbServerName[0] = 0;
            ret = -1;
        }
    }
    else
    {
        printf_timestamp("Version check to %s returned 0\n", dbServerName);
        clearNetLink(netLink);
        // Artificially turn this into an error.  That way the five second delay code will cut in and prevent this from spamming over and over.
        // The cause of spam was tracked down and fixed.  However this still wants to be considered an error, therefore we will leave this returning -1
        ret = -1;
    }

    return ret;
}

void sendProcesses(NetLink *link)
{
    Packet    *pak;

    pak = pktCreate();
    pktSendBitsPack(pak,1,LAUNCHERANSWER_PROCESSES);
    // Added the Netlink as a parameter to this, so we can select from the processes we're monitoring based on whether they match this link.
    // This is necessary, so that the data sent to each DbServer only conatins details about mapservers started by that DbServer.
    procSendTrackedInfo(pak, link);
    pktSend(&pak,link);
}

// Added the NetLink as a parameter, so that we can mark which Link (and hence which DbServer) this map belongs to
void handleStartProcess(Packet *pak, NetLink *link)
{
    char        *s;
    int            container_id;
    int            cookie;
    LaunchType    launch_type;
    int            trackByExe;
    int            monitorOnly;
    U32            pid=0;

    container_id = pktGetBitsPack(pak,1);
    s = pktGetString(pak);
    cookie = pktGetBitsPack(pak, 1);
    launch_type = pktGetBitsPack(pak, 1);
    trackByExe = pktGetBits(pak, 1);
    monitorOnly = pktGetBits(pak, 1);
    g_process_starts++;
    if (trackByExe || monitorOnly)
    {
        // Attempt to find already-running application
        // And pass in the link so that it can be saved away with the process info.
        pid = trackProcessByExename(container_id, cookie, launch_type, s, (void *) link);
        if (pid != 0) {
            // Already running!
            printf("launcher already running: %s\n",s);
        }
    }
    if (!pid && !monitorOnly) {
        printf("launcher starting: %s\n",s);
        pid = system_detach(s, 1);
        trackProcessByContainerId(container_id, pid, cookie, launch_type, (void *) link);
    }
}

// currently only called by mapservers when they reach "Server Ready"
void handleProcessRegister(Packet *pak)
{
    int        map_id,cookie;
    U32        process_id;

    map_id = pktGetBitsPack(pak,1);
    process_id = pktGetBitsPack(pak,1);
    cookie = pktGetBitsPack(pak,10);
    procRegisterReady(map_id, process_id, cookie, CONTAINER_MAPS);
}

// defaults to map
static LauncherEnabledServer s_enabledServers = kLauncherEnabledServer_Map;

static int launcherHandleMsg(Packet *pak,int cmd, NetLink *link)
{
    switch(cmd)
    {
        xcase LAUNCHERQUERY_REGISTER:        // mapserver registers when server is ready
            handleProcessRegister(pak);
        xcase LAUNCHERQUERY_STARTPROCESS:
            handleStartProcess(pak, link);
        xcase LAUNCHERQUERY_GAMEVERSION:
            handleGameVersion(pak);
        xcase LAUNCHERQUERY_CONTROL:        // launcher is sending us control information
            handleControl(pak);
        xdefault:
            printf("Unknown command %d\n",cmd);
            return 0;
    }
    return 1;
}

static void launcherHandleInput()
{
    static int debug_enabled=0;
    int ch;

    if (_kbhit()) {
        ch = _getch();
        if (debug_enabled<2)
        {
            if (ch == 'd')
            {
                debug_enabled++;
                if (debug_enabled==2)
                {
                    printf("Debugging hotkeys enabled (Press '?' for a list).\n");
                }
            }
            else
            {
                debug_enabled = 0;
            }
        }
        else
        {
            switch (ch)
            {
                case 'm':
                    memMonitorDisplayStats();
                    break;
                case 'n':
                {
                    int i;
                    printf("db_link sendQueue sizes:\n");
                    for (i=0; i<MAX_DBSERVER; i++)
                    {
                         printf("  %s=%d\n", shards[i].name, qGetSize(shards[i].db_link.sendQueue2));
                    }
                    break;
                }
                case 'p':
                    printf("SERVER PAUSED FOR 2 SECONDS PRESS 'p' AGAIN TO PAUSE INDEFINITELY...\n");
                    Sleep(2000);
                    if (_kbhit() && _getch()=='p') {
                        printf("SERVER PAUSED\n");
                        (void)_getch();
                    }
                    printf("Server resumed\n");
                    break;
                case 'r':
                {
                    int i;
                    for (i=0; i<MAX_DBSERVER; i++)
                    {
                        clearNetLink(&shards[i].db_link);
                    }
                    Sleep(1000);
                    break;
                }
                case 's':
                    toggleSuspension();
                    break;
                case '?':
                    printf("Keyboard shortcuts for launcher control and debugging:\n");
                    printf("  s - notify DbServer to suspend launching on this machine. 's' again will resume launching \n");
                    printf("  m - print memory usage\n");
                    printf("  n - print some network stats\n");
                    printf("  p - pause server\n");
                    printf("  r - reconnect to DbServer in 1s\n");
                    break;
            }
        }
    }
}

void autoRegisterCrypticStuff()
{
    const char *install_directory = regGetInstallationDir();
    RegReader reader;
    int last_ran_timestamp = 0;
    int current_timestamp = fileLastChanged("./src/util/NCAutoSetup.exe");
    reader = createRegReader();
    initRegReader(reader, regGetAppKey());
    if (!rrReadInt(reader, "NCAutoSetup", &last_ran_timestamp))
        last_ran_timestamp = 0;
    if (current_timestamp>0    // file exists
        && current_timestamp != last_ran_timestamp // hasn't been ran yet
        && last_ran_timestamp != -2) // not disabled (-2 in registry -> disabled)
    {
        char cmdline[1024];
        sprintf(cmdline, "./src/util/NCAutoSetup.exe %s", install_directory);
        system_detach(cmdline, 0);
        rrWriteInt(reader, "NCAutoSetup", current_timestamp);
    }
    destroyRegReader(reader);
}

static void launcherTickShardConnect()
{
    int i;

    for (i = 0; i < MAX_DBSERVER; i++)
    {
        int result;

        LauncherShard * shard = &shards[i];
        if (!shard->name || !shard->name[0])
            continue;

        if (!shard->db_link.connected)
        {
            // If we're not connected, the pending flag says whether the link is disconnected and idle, or in the process of trying an async connect
            if (shard->db_link.pending)
            {
                // If it's pending, there's a connect in progress, so poll for completion
                result = connectDbPoll(shard->name, &shard->db_link);
                if (result == -1)
                {
                    // Handle the error
                    shard->retryTimer = timerAlloc();
                    timerStart(shard->retryTimer);
                }
            }
            // Not pending, check the retry timer
            else if (shard->retryTimer == 0)
            {
                // No active retry timer, it must have timed out, so kick start a connection attempt.
                connectDbStart(shard->name, &shard->db_link);
            }
            else
            {
                // Otherwise the retry timer is counting
                if (timerElapsed(shard->retryTimer) > 5.0f)
                {
                    // If it's finished clean up, this will cause the connection attempt next time through.
                    timerFree(shard->retryTimer);
                    shard->retryTimer = 0;
                }
            }
        }
    }
}

int main(int argc,char **argv)
{
    int    i;
    int timer;

    EXCEPTION_HANDLER_BEGIN
    setAssertMode(ASSERTMODE_MINIDUMP | ASSERTMODE_DEBUGBUTTONS);
    memMonitorInit();
    for(i = 0; i < argc; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");

    consoleInit(110, 128, 0);
    setWindowIconColoredLetter(compatibleGetConsoleWindow(), 'L', 0x8080ff);

    FolderCacheChooseMode();
    FolderCacheSetMode(FOLDER_CACHE_MODE_FILESYSTEM_ONLY);

    if (fileIsUsingDevData()) {
        bsAssertOnErrors(true);
    }

    if (isProductionMode())
        autoRegisterCrypticStuff();

    preloadDLLs(0);
    //trickGoogleDesktopDll(0); // we also have shared memory, so trick the google desktop dll into mapping somewhere reasonable

    printf( "SVN Revision: %s\n", build_version);

    setConsoleTitle("Launcher");

    if (0)
    {
        TestTextParser();
    }

    logSetDir("launcher");
    sockStart();
    serverCfgLoad();
    for(i=1;i<argc;i++)
    {
        if (stricmp(argv[i],"-db")==0)
            strcpy(server_cfg.db_server,argv[++i]);
        if (stricmp(argv[i],"-localmapserver")==0)
            local_launcher = 1;
        if (stricmp(argv[i],"-monitor")==0) {
            setConsoleTitle("Launcher - Performance Monitor Only mode");
            local_launcher = 1;
        }
        if (stricmp(argv[i],"-noversioncheck")==0)
            launcher_no_version_check = 1;
        if(0 == stricmp(argv[i],"-servers"))
        {
            s_enabledServers = kLauncherEnabledServer_None;
            
            for(;i<argc;++i)
            {
                static struct IdStrPairs
                {
                    int id;
                    char const *str;
                } idStrPairs[] = {
                    {kLauncherEnabledServer_Map,"mapserver"},
                    {kLauncherEnabledServer_Stat,"statserver"},
                };

                for(i = 0; i < ARRAY_SIZE( idStrPairs ); ++i)
                {
                    if( 0 == stricmp(argv[i], idStrPairs[i].str ))
                    {
                        s_enabledServers |= idStrPairs[i].id;
                        break;
                    }
                }
            }
        }
    }

    loadstart_printf("Starting system performance counters...");
    system_monitor_init(); // Doing this early seems to open up access for querying more processes?  Weird.
    loadend_printf("");

#ifdef LAUNCHER_TRACKS_SHARED_HEAP
    loadstart_printf("initializing shared heap memory manager...");
    {
        initSharedHeapMemoryManager();

        /*
        U32 uiReservedSize = reserveSharedHeapSpace(getSharedHeapReservationSize());
        if ( uiReservedSize )
        {
            loadend_printf(" reserved %d MB", uiReservedSize / (1024 * 1024));
        }
        else
        {
            loadend_printf(" failed: no shared heap");
        }
        */

        loadend_printf("done");
    }
#endif

    loadstart_printf("Networking startup...");
    timer = timerAlloc();
    timerStart(timer);
    packetStartup(0,0);
    netLinkListAlloc(&net_links,MAX_CLIENTS,0,0);
    while (!netInit(&net_links,0,DEFAULT_LAUNCHER_PORT))
        Sleep(1);
    NMAddLinkList(&net_links, launcherHandleMsg);
    loadend_printf("");

    // Initialise the connection retry timers.
    for (i = 0; i < MAX_DBSERVER; i++)
    {
        shards[i].retryTimer = 0;
        
        // hack for strange config file format
        if (!i)
            shards[i].name = server_cfg.db_server;
        else
            shards[i].name = server_cfg.db_server_aux[i - 1];
    }

    for(;;)
    {
        F32 elapsed_seconds;

        NMMonitor(100);

#ifdef LAUNCHER_TRACKS_SHARED_HEAP
        sharedHeapLazySyncHandlesOnly();
#endif

        launcherHandleInput();
        launcherTickShardConnect();

        elapsed_seconds = timerElapsed(timer);
        if (elapsed_seconds > 1.0f)
        {
            int num_connected_dbservers;
            timerStart(timer);
            
            // count currently connected dbservers so we can report that
            // to clients as it helps in monitoring and interpreting launcher status
            for (num_connected_dbservers = 0, i = 0; i < MAX_DBSERVER; i++)
            {
                if (shards[i].db_link.connected)
                    num_connected_dbservers++;
            }

            procGetList();
            procUpdateTrackingInfo(num_connected_dbservers, g_process_starts, elapsed_seconds);
            system_monitor_sample();
            
            // send an update packet of host machine metrics and child process
            // information to every connected dbserver
            for (i = 0; i < MAX_DBSERVER; i++)
            {
                if (shards[i].db_link.connected)
                    sendProcesses(&shards[i].db_link);
            }
        }
    }

    EXCEPTION_HANDLER_END
}
