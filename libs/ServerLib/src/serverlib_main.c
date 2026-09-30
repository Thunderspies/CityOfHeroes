
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/log.h"
#include "../include/serverlib/serverlib.h"
#include "utilitieslib/UtilsNew/meta.h"
#include "../../../Common/ServerLib/serverlib_meta.h"

#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/network/crypt.h"
#include "utilitieslib/network/netio.h"
#include "utilitieslib/UtilsNew/Array.h"
#include "utilitieslib/UtilsNew/Str.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/components/StashTable.h"
#include "../../../Common/persist/persist.h"

#include "utilitieslib/utils/ConsoleDebug.h"
#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/utils/MemoryMonitor.h"
#include "utilitieslib/utils/FileSystem.h"

#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/winutil.h"
#include "utilitieslib/utils/sysutil.h"
#include "utilitieslib/utils/utils.h"


#define SERVERLIB_HTTPPORT_DEFAULT    8100        // counting up from here
#define SERVERLIB_HTTPPORT_MAX        8199        // to here
#define SERVERLIB_TICKTIME_MIN        (1.f/30.f)    // sleep if less than this (s)
#define SERVERLIB_SLEEPTIME_MIN        34            // sleep at least this long (ms)

ServerLibState g_serverlibstate;

static void s_memDumpPrompt(void)
{
    printf("Type 'DUMP' to continue.\n");
}

static void s_memDump(char* param)
{
    if(streq(param, "DUMP"))
        memCheckDumpAllocs();
}

static void s_toggleVerboseOutput(void)
{
    g_serverlibstate.verbose = !g_serverlibstate.verbose;
    printf( g_serverlibstate.verbose ? "maximum verbosity" : "terse");
}

static ConsoleDebugMenu s_debugmenu_top[] =
{
    { 0,    "ServerLib Commands:",            NULL,                    NULL        },
    { 'M',    "dump memory table (SLOW!)",    s_memDumpPrompt,        s_memDump    },
    { 'm',    "print memory usage",            memMonitorDisplayStats,    NULL        },
    { 'V',    "toggle verbose output",        s_toggleVerboseOutput,    NULL        },
};
static ConsoleDebugMenu *s_debugmenu;

static BOOL s_CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
        #define QUIT_CASE(event)                                            \
            case event:                                                        \
                printf("Control Event " #event " (%i)", event);    \
                g_serverlibstate.quitting = 1;                                \
                for(;;) /* wait for the main thread to call exit */            \
                    Sleep(10);                                                \
                break;

        QUIT_CASE(CTRL_C_EVENT);
        QUIT_CASE(CTRL_BREAK_EVENT);
        QUIT_CASE(CTRL_CLOSE_EVENT);
        QUIT_CASE(CTRL_LOGOFF_EVENT);
        QUIT_CASE(CTRL_SHUTDOWN_EVENT);

        #undef QUIT_CASE
    }
    return FALSE;
}

META(COMMANDLINE("quit"));

META(COMMANDLINE("plMerge"))
void serverlib_plMerge(const char *typeName)
{
    loadstart_printf("Merging %s...", typeName);
    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
    plMergeByName(typeName);
    g_serverlibstate.quitting = 1; // quit at the next opportunity, but allow multiple merges
    loadend_printf("");
}

int main(int argc,char **argv)
{
    int timer;

    memCheckInit();

    EXCEPTION_HANDLER_BEGIN 
    setAssertMode(ASSERTMODE_MINIDUMP | ASSERTMODE_DEBUGBUTTONS);
    FatalErrorfSetCallback(NULL); // assert, log, and exit

    timer = timerAlloc(); // startup time
    g_serverlibstate.started = timerSecondsSince2000();
    timerMakeDateString(g_serverlibstate.started_readable);

    mpCompactPools();
    cryptInit();
    srand((unsigned int)time(NULL));

    assert(g_serverlibconfig.name && g_serverlibconfig.name[0]);

    if (g_serverlibconfig.flags & kServerLibLikePiggs)
    {
        fileSystemChooseMode();
    }
    else
    {
        fileSystemSetMode(FILE_MODE_LOOSE);
    }
    preloadDLLs(0);

    if(fileIsUsingDevData())
    {
        bsAssertOnErrors(true);
        setAssertMode(    ASSERTMODE_DEBUGBUTTONS |
                        (!IsDebuggerPresent() ? ASSERTMODE_MINIDUMP : 0) );
    }
    else
    {
        // In production mode on the servers we want to save all dumps and do full dumps
        setAssertMode(    ASSERTMODE_DEBUGBUTTONS |
                        ASSERTMODE_FULLDUMP |
                        ASSERTMODE_DATEDMINIDUMPS |
                        ASSERTMODE_ZIPPED );
    }

    if(g_serverlibconfig.icon_letter)
        setWindowIconColoredLetter(compatibleGetConsoleWindow(), g_serverlibconfig.icon_letter, g_serverlibconfig.icon_color);
    consoleInit(110, 128, 0);
    {
        char *buf = Str_temp();
        Str_catf(&buf, "%d: %s", _getpid(), g_serverlibconfig.name);
        setConsoleTitle(buf);
        Str_destroy(&buf);
    }

    logSetDir(g_serverlibconfig.name);
    errorLogStart();
    assert(SetConsoleCtrlHandler((PHANDLER_ROUTINE)s_CtrlHandler, TRUE));

    strcpy(g_serverlibstate.exe_name, argv[0]);
    strcpy(g_serverlibstate.cmd_line, GetCommandLineA());
    g_serverlibstate.argc = argc;
    g_serverlibstate.argv = argv;

    printf("%s: ", g_serverlibstate.started_readable);
    printf("Starting: %s", g_serverlibstate.cmd_line);
    if(g_serverlibconfig.init_handler)
        g_serverlibconfig.init_handler();

    serverlib_meta_register();
    meta_handleCommandLine(argc, argv);
    if(g_serverlibstate.quitting)
    {
        exit(0);
    }
    else
    {
        int i;
        for(i = 1; i < argc; i++)
        {
            if(stricmp(argv[i],"-quit")==0)
            {
                FatalErrorf("Error: Unhandled quit command");
                exit(1);
            }
        }
    }

    // ensure only one of this type of server
    {
        char hn[128];
        HANDLE mx;
        sprintf(hn,"Global\\%s_mutex", g_serverlibconfig.name);
        mx = CreateMutexA(0,0,hn);
        loadstart_printf("acquiring mutex %s...",hn);
        if(!mx)
            FatalErrorf("failed to allocate mutex %s",hn);
        else if(WAIT_TIMEOUT == WaitForSingleObject(mx,0))
        {
            FatalErrorf("ERROR: couldn't acquire mutex %s two auction servers of this type running at same time.",hn);
        }
        // CloseHandle(mx); - deliberately leak the handle, not released until exit.
        loadend_printf("done");
    }

    loadstart_printf("Networking startup...");
    packetStartup(0, (g_serverlibconfig.flags & kServerLibNetEncryption) != 0 ? 1 : 0);
    loadend_printf("");
    
    loadstart_printf("Loading data...");
    if(!plLoadAll())
    {
        FatalErrorf("Error while loading data.\nPlease see the console/logs for more information.");
        exit(2);
    }
    if(g_serverlibconfig.load_handler)
        g_serverlibconfig.load_handler();
    loadend_printf("");

    loadstart_printf("Etc...");
    {
        ConsoleDebugMenu *configmenu = g_serverlibconfig.consoledebugmenu;
        int count = 0, max_count = 0;
        dynArrayAddStructs(&s_debugmenu, &count, &max_count, ARRAY_SIZE(s_debugmenu_top));
        memcpy(s_debugmenu, s_debugmenu_top, sizeof(s_debugmenu_top));
        if(configmenu)
        {
            ConsoleDebugMenu *newmenu = dynArrayAddStruct(&s_debugmenu, &count, &max_count);
            Str_catf((char**)&newmenu->helptext, "%s Commands:", g_serverlibconfig.name);
            for (; configmenu->cmd || configmenu->helptext; configmenu++)
            {
                memcpy(dynArrayAddStruct(&s_debugmenu, &count, &max_count), configmenu, sizeof(*configmenu));
            }
        }
        dynArrayAddStruct(&s_debugmenu, &count, &max_count); // terminator
    }
    loadend_printf("");

    printfColor(COLOR_RED|COLOR_GREEN|COLOR_BLUE | COLOR_BRIGHT, "%s ready.", g_serverlibconfig.name);
    printf(" (in %f seconds)\n\n", timerElapsed(timer));

    while(!g_serverlibstate.quitting)
    {
        F32 frame_elapsed;
        mpCompactPools();

        NMMonitor(1);
//         shardNetConnectTick(&g_missionServerState);
        timerStart(timer); // frame time, started after connection handling, since that can be extremely slow
//         shardNetProcessTick(&g_missionServerState);

        DoConsoleDebugMenu(s_debugmenu);

        if(g_serverlibconfig.tick_handler)
        {
//            if(g_serverlibstate.verbose)
//                printf("%s tick\n", g_serverlibconfig.name);
            g_serverlibconfig.tick_handler();
        }

//        if(g_serverlibstate.verbose)
//            printf("persist tick\n");
        plTick();

        frame_elapsed = timerElapsed(timer);
//         if(frame_elapsed < SERVERLIB_TICKTIME_MIN)
        {
            int ms_sleep = MAX(SERVERLIB_SLEEPTIME_MIN, (SERVERLIB_TICKTIME_MIN - frame_elapsed)*1000.f);
            Sleep(ms_sleep);
            if(g_serverlibstate.verbose)
                printf("tick %dms + sleep %dms", (int)(frame_elapsed*1000.f), ms_sleep);
        }
    }

    printf("Stopping: %s", g_serverlibstate.cmd_line);
    if(g_serverlibconfig.stop_handler)
        g_serverlibconfig.stop_handler();
    plShutdown();
    exit(0);
    EXCEPTION_HANDLER_END;
}
