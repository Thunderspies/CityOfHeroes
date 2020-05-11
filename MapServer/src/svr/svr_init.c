#include <conio.h>
#include "storyarc/storyarc.h"
#include <utilitieslib/utils/StringUtil.h>
#include "entity/character_inventory.h"
#include "entity/SgrpBadges.h"
#include <stdio.h>
#include <process.h>
#include <time.h>
#include <utilitieslib/network/sock.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_socket.h>
#include <utilitieslib/utils/timing.h>
#include "cmdparse/cmdserver.h"
#include "seq/gfxtree.h"
#include "entity/entity.h"
#include "svr_base.h"
#include "entity/entsend.h"
#include <utilitieslib/network/netcomp.h>
#include "group/group.h"
#include "group/groupfilesave.h"
#include "group/grouputil.h"
#include "generator/entgen.h"
#include <utilitieslib/utils/fpmacros.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/file.h>
#include "dbcomm/dbcomm.h"
#include <utilitieslib/utils/utils.h>
#include "utils/debugUtils.h"
#include "ai/entscript.h"
#include "cmdparse/entcon.h"
#include <utilitieslib/UtilsNew/profiler.h>
#include "dbcomm/dbquery.h"
#include "gridcoll/gridcoll.h"
#include "comm_backend.h"
#include "dbcomm/dbcontainer.h"
#include "group/groupnetdb.h"
#include <utilitieslib/assert/assert.h>
#include <crtdbg.h>
#include <direct.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/SuperAssert.h>
#include "gameComm/initCommon.h"
#include "container/containerloadsave.h"
#include "container/containercallbacks.h"
#include <utilitieslib/network/sock.h>
#include "signal.h"
#include "svr_base.h"
#include "svr_player.h"
#include "svr_tick.h"
#include "generator/encounter.h"
#include "ai/entai.h"
#include "storyarc/storyarcInterface.h"
#include "language/langServerUtil.h"
#include "entity/load_def.h" // for load_AllDefs
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/sysutil.h>        // for getExecutableName()
#include "group/groupfilelib.h"
#include "group/groupfileload.h"
#include <utilitieslib/utils/strings_opt.h>
#include "gameComm/NpcNames.h"
#include <utilitieslib/version/AppVersion.h>
#include "serverError.h"
#include <utilitieslib/components/SharedMemory.h>
#include "filter/profanity.h"
#include "filter/titles.h"
#include "filter/reserved_names.h"
#include "gameComm/NpcChatter.h"
#include "player/stats_base.h" // for InitStats()
#include "gameData/store.h"
#include "gameData/costume_data.h"
#include "entity/power_customization.h"
#include "ai/entaiprivate.h"
#include "gridcoll/gridcollperftest.h"
#include "dbcomm/staticMapInfo.h"            // for staticMapInfosReload()
#include "gridcoll/gridcache.h"
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/utils/DirMonitor.h>
#include "dbcomm/characterTransfer.h"
#include "dbcomm/characterInfo.h"
#include "player/badges_server.h"
#include "dbcomm/shardcomm.h"
#include "dbcomm/shardcommtest.h"
#include "entity/character_combat_eval.h"
#include "dbcomm/logcomm.h"
#include "script/scriptengine.h"
#include "dbcomm/characterRestore.h"
#include <utilitieslib/language/AppLocale.h>
#include "seq/tricks.h"
#include "win/input.h"
#include "seq/modelReload.h"
#include "seq/seq.h"
#include "player/parseClientInput.h"
#include "seq/anim.h"
#include "storyarc/skillobj.h"
#include "container/baseloadsave.h"
#include "bases/basedata.h"
#include "taskRandom.h"
#include "gameData/arenaschedule.h"
#include "gameData/iopdata.h"
#include "seq/seqstate.h"
#include "NovodeX/Nwwrapper.h"
#include "group/groupnovodex.h"
#include "entity/ragdoll.h"
#include <utilitieslib/components/SharedHeap.h>
#include <utilitieslib/components/StringCache.h>
#include "taskpvp.h"
#include "beacon/beaconClientServerPrivate.h"
#include "entity/Supergroup.h"
#include "bases/DetailRecipe.h"
#include "gameSys/raidmapserver.h"
#include "entity/powers.h"
#include "container/containerDump.h"
#include "seq/AnimBitList.h"
#include <utilitieslib/version/AppRegCache.h>
#include <utilitieslib/language/MessageStore.h>
#include "entity/TaskforceParams.h"
#include "gameSys/turnstile.h"
#include "storyarc/playerCreatedStoryarcValidate.h"
#include "plaque.h"
#include "reward.h"
#include "player/inventory_server.h"
#include <utilitieslib/utils/log.h>
#include "account/AccountCatalog.h"
#include "dbcomm/dbmapxfer.h"
#include <psapi.h> // for EmptyWorkingSet()
#include "entity/character_eval.h"
#include "entity/character_combat_eval.h"

#define loadstart_printf if (!server_state.silent) loadstart_printf
#define loadend_printf if (!server_state.silent) loadend_printf

static char gReservedNameTarget[100];

int write_templates;
static int link_db_server=1,frame_timer,coll_test,grid_cache_bits=-1;
static char    *template_dir,*map_batch_cmds;
static char    *svr_config = 0;

extern int g_ignorebrokentasklist;

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

    printf_stderr("Quitting: %s\n",program_parms);

    logShutdownAndWait();
// JE: Not valid because of threaded code, might be in file operations
//    if (glob_assert_file_err)
//        assert(0);
}

static void svrInit()
{
    extern int g_assert_on_netlink_overflow;
    g_assert_on_netlink_overflow = 1; // We want to be delinked and assert if this happens

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
    net_links.publicAccess = 1;
    NMAddLinkList(&net_links, svrHandleClientMsg);
}

#define MIN_STEPTIME (1.f/30.f)

static void sleepAndCalcTimers()
{
    static F32 leftover_timestep;

    extern F32        makemovie_time;
    F32        min_steptime;
    U32        itime;
    int        i;
    F32        elapsed;
    F32        pre_elapsed;

    pre_elapsed = timerElapsed(frame_timer);
    if (pre_elapsed < MIN_STEPTIME)
    {
        int ms_sleep = ceil((MIN_STEPTIME - pre_elapsed) * 1000);

        assert(pre_elapsed + ms_sleep / 1000.0f >= MIN_STEPTIME);

        Sleep(ms_sleep);
    }
    elapsed = timerElapsedAndStart(frame_timer) * server_visible_state.timestepscale;

    min_steptime = MIN_STEPTIME;

    if (elapsed > min_steptime * 2)
    {
        global_state.frame_time_30_discarded = 30.0 * (elapsed - min_steptime * 2);

        elapsed = min_steptime * 2;
    }
    else
    {
        global_state.frame_time_30_discarded = 0;
    }

    // Update the time_since array with the old TIMESTEP.

    for(i=ARRAY_SIZE(global_state.time_since)-1;i>0;i--)
        global_state.time_since[i] = global_state.time_since[i-1] + TIMESTEP;
    global_state.time_since[0] = TIMESTEP;

    // Update the current timestep.

    TIMESTEP = elapsed * 30;//itime / 100.0;
    TIMESTEP_NOSCALE = TIMESTEP / server_visible_state.timestepscale;
    itime = (TIMESTEP + leftover_timestep) * 100;
    ABS_TIME += itime;//itime;
    leftover_timestep = ((TIMESTEP + leftover_timestep) * 100 - itime) / 100.0;

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
        printf( "SVN Revision: %s\n", build_version);
    }

    if (isProductionMode()) {
        consoleInit(80, 10, 256);
    } else {
        consoleInit(110, 128, 0); // A width of 110 will fit nicely on most resolutions, and be wide enough for us to see stuff
    }

    logSetDir("mapserver");
    errorLogStart();

    if(!beaconizerIsStarting())
    {
        LOG_OLD("mapserver Starting: %s\n",program_parms);

        sprintf(buf, "%d: ", _getpid());

        for(i=0;i<argc;i++)
        {
            strcat(buf,argv[i]);
            strcat(buf," ");
        }

        setConsoleTitle(buf);
    }
}

static void setLoadPriority(void)
{
    if(server_state.create_bins || write_templates)
        SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
    else if(isProductionMode())
        SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
    else
        SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
}

static void setRuntimePriority(void)
{
    if(server_state.create_bins || write_templates)
        SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
    else if(isProductionMode())
        SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
    else
        SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
}

static void sysInit() // non game-specific initializations
{
    FolderCacheExclude(FOLDER_CACHE_EXCLUDE_FOLDER, "texture_library"); // currently accepts only one of these

    if(beaconizerIsStarting() != 1){
        // File system completely disabled for beacon clients.

        fileDataDir();

        loadend_printf("Caching directory tree and misc startup...");
    }

    loadstart_printf("Networking startup%s.. ", server_state.noEncryption ? "(No Encryption)" : "");
    //logNSetLogDir("c:/logs/server");
    timerCpuSpeed();
    setLoadPriority();
    atexit( at_exit_func );

#ifdef _DEBUG
    // Bring up the Abort/Retry/Ignore window on assertion instead of outputting to console.
    _set_error_mode(_OUT_TO_MSGBOX);
#endif

    sockStart();
    SET_FP_CONTROL_WORD_DEFAULT;
    packetStartup(0,server_state.noEncryption?0:1);
    setSignal();

    // A number of places check the registry to figure out which locale is
    // appropriate, so if we're set for a language that players can't use
    // we should reset the registry. That way everything will work from
    // the same locale.
    if(!locIDIsValidForPlayers(locGetIDInRegistry()))
        locSetIDInRegistry(DEFAULT_LOCALE_ID);

    setCurrentLocale(locGetIDInRegistry());

    loadend_printf("done");
}

static void ReservedNameMode()
{
    int ret;
    NetLink link = {0};
    printf("\n\nChatserver Reserved Name Mode -- will send reserved names to chatserver '%s'\n\n", gReservedNameTarget);
    while(1)
    {
        if (!link.connected)
        {
            loadstart_printf("Connecting to Chatserver.. ");
            memset(&link, 0, sizeof(link));
            ret = netConnect(&link,gReservedNameTarget,DEFAULT_CHATSERVER_AUX_PORT,NLT_TCP,NO_TIMEOUT,NULL);
            if(ret)
            {
                Packet * pak = pktCreateEx(&link, SHARDCOMM_AUX_RESERVED_NAMES);
                sendReservedNames(pak);
                sendProfanity(pak);
                pktSend(&pak, &link);
                if (!link.disconnected)
                    lnkBatchSend(&link);
                loadend_printf("done");
            }
            else
                printf("Error connecting to chatserver\n");
        }
        else
        {
            netLinkMonitor(&link, 0, 0);
            netIdle(&link,1,10);
        }
    }
}

static void loadConfigFiles()
{
    bool bNewAttribs = 0;
    loadstart_printf("Caching relevant folders.. ");
    cacheRelevantFolders();
    loadend_printf("done");

    loadstart_printf("loading message stores.. ");
    loadServerMessageStores();
    loadend_printf("done");

    loadstart_printf("loading tricks.. ");
    trickLoad();
    loadend_printf("done");

    loadstart_printf("Loading Loyalty Rewards.. ");
    accountLoyaltyRewardTreeLoad();
    loadend_printf("done");

    groupLoadLibs(); 
    modelInitReload();

    // Load the new classes, origins, and power stuff.
    loadstart_printf("loading defs.. ");
    load_AllDefs(); // Must be done after message loading
    loadend_printf("done loading defs");

    loadstart_printf("loading badges.. ");
    badge_ServerInit(); // Must be done after load_AllDefs.
    sgrpbadge_ServerInit();
    loadend_printf("done");

    // map/villain to skill objects
    loadstart_printf("loading skill objs.. ");
    load_SkillObjectives();
    loadend_printf("");

    init_menus(); // timed internally

    if(!server_state.levelEditor)
    {
        loadstart_printf("loading items.. ");
        load_Stores();
        loadend_printf("done");

        StoryPreload();    // need to load this before attributes
        TaskForceLoadTimeLimits(); // need story arcs loaded first
    }
    else
    {
        // See ContactPostprocessAll()!
        assert(sharedMemoryGetMode()==SMM_DISABLED);
        ((ContactList*)&g_contactlist)->contactHandleFromName = stashTableCreateWithStringKeys(stashOptimalSize(0), stashShared(false));
    }

    loadstart_printf("Initializing stats.. ");
    InitStats(); // must be after staticMapInfosReload and before containerWriteTemplates
    loadend_printf("done");

    if (write_templates)
    {
        playerCreatedStoryArc_GenerateData();
        containerWriteTemplates(template_dir);
        exit(0);
    }

    loadstart_printf("loading filters and names.. ");
    LoadProfanity();
    npcLoadNames();      // Must be after profanity.
    npcLoadChatter();    // Must be after profanity.
    LoadTitleDictionaries();
    LoadReservedNames(); // Must be after PNPC and contact loading.
    loadend_printf("done");

    if(gReservedNameTarget[0]) // special mode to send reserved names to ChatServer & then exit
    {
        ReservedNameMode(); // will never return
        exit(0);
    }

    if (!server_state.levelEditor)
    {
#ifdef SERVER
        if (!server_state.tsr)
#endif
        {
            loadstart_printf("loading AI.. ");
            aiInitSystem();
            entconAddKnownHandlers();
            loadend_printf("done");
        }

        loadstart_printf("loading random def files.. ");
        loadRandomDefFiles();
        loadend_printf("done");

        loadstart_printf("loading pvp task modifiers.. ");
        PvPTaskLoadModifiers();
        loadend_printf("done");

        loadstart_printf("loading broken task list.. ");
        LoadBrokenTaskList();
        loadend_printf("done");
    }
    loadCostumes();
    loadPowerCustomizations();

    // Load turnstile mission database
    turnstileMapserver_init();

    if( !server_state.levelEditor )
    {
        loadstart_printf("loading player created story arc files.. ");
        playerCreatedStoryarc_LoadData(server_state.create_bins);
        loadend_printf("done");
    }


    // test clients, production mode, and mapserver -TSR delete allRewardNames after load
    {
        bool cleanup = false;

        if (isProductionMode())
            cleanup = true;

#if SERVER
        if (server_state.tsr)
            cleanup = true;
#endif

#if TESTCLIENT
        cleanup = true;
#endif

        if (cleanup)
            allRewardNamesCleanup();
    }
}

#ifndef FINAL
extern int g_force_production_mode;
#endif

static void parseArgs0(int argc,char **argv)
{
    int        i;

    beaconHandleCmdLine(argc, argv);
    
    // This function is only allowed to do things that don't do much of anything.

    for(i=1;i<argc;i++)
    {
        int handled = 1;
        int start_i = i;

        if (stricmp(argv[i],"-nowinio")==0)
            fileDisableWinIO(1);
        else if (stricmp(argv[i], "-nogui")==0)
        {
            setGuiDisable(true);
            // allow parseArgs1 to also see this parameter to set the assert mode
            handled = 0;
        }
#ifndef FINAL
        else if (stricmp(argv[i], "-productionmode")==0)
        {
            g_force_production_mode = 1;
        }
#endif
        else if (stricmp(argv[i], "-locale")==0)
        {
            locOverrideIDInRegistryForServersOnly(locGetIDByNameOrID(argv[++i]));
        }
        else if (stricmp(argv[i], "-debugonstartup")==0)
        {
            // This is here to allow someone to hook into a mapserver as it starts
            while (1)
            {
                Sleep(1);
            }
        }
        else
        {
            handled = 0;
        }

        if(handled)
        {
            // Invalidate handled parameters in such a way that they won't cause errors in parseArgs1.

            while(start_i <= i)
            {
                argv[start_i++][0] = 0;
            }
        }
    }
}

static void parseArgs1(int argc,char **argv)
{
    int        i;

    for(i=1;i<argc;i++)
    {
        int handled = 1;
        int start_i = i;

        if (strcmp(argv[i],"-db")==0) // Need this one as soon as possible in case we crash and need to report
        {
            strcpy(db_state.server_name,argv[++i]);
            handled = 0; // Need it to get hit in parseArgs2 as well!
        }
        else if (strcmp(argv[i],"-map_id")==0)
        {
            db_state.local_server = 0;
            db_state.map_id = atoi(argv[++i]);
            handled = 0; // Need it to get hit in parseArgs2 as well!
        }
        else if (strcmp(argv[i], "-noencrypt")==0)
        {
            server_state.noEncryption = 1;
        }
        else if (strcmp(argv[i], "-nosharedmemory")==0)
        {
            printf("Not using shared memory due to -nosharedmemory\n");
            sharedMemorySetMode(SMM_DISABLED);
            sharedHeapTurnOff("disabled");
        }
        else if (strcmp(argv[i], "-sharedheapmegs")==0)
        {
            U32 uiSize = atoi(argv[++i]) * 1024 * 1024;
            if ( uiSize == 0 )
            {
                printf("Not using shared memory due to -sharedheapmegs 0\n");
                sharedMemorySetMode(SMM_DISABLED);
            }
            else
            {
                printf("Setting default shared heap reservation to %dMB.\n", uiSize / (1024 * 1024));
                setSharedHeapReservationSize(uiSize);
            }
        }
        else if (strcmp(argv[i], "-nopigs")==0)
        {
            printf("Not using piggs due to -nopiggs\n");
            FolderCacheSetMode(FOLDER_CACHE_MODE_FILESYSTEM_ONLY);
        }
        else if (strcmp(argv[i], "-silent")==0)
        {
            // Just silent enough for dbquery to produce clean output, feel free to make it more silent elswhere
            server_state.silent = 1;
        }
        else if (strcmp(argv[i], "-dbquery")==0 || strcmp(argv[i], "-getcharacter")==0 || strcmp(argv[i], "-putcharacter")==0 
            || strcmp(argv[i], "-restorecharacter")==0 || strcmp(argv[i], "-dumpcon")==0)
        {
            // We want these two options by default for -dbquery to make them fast!
            handled = 0;
            FolderCacheSetMode(FOLDER_CACHE_MODE_FILESYSTEM_ONLY);
            server_state.noEncryption = 1;
            server_state.silent = 1;
        }
        else if (stricmp(argv[i], "-nogui")==0)
        {
            // also handled in parseArgs0
            setAssertMode(ASSERTMODE_STDERR | ASSERTMODE_EXIT);
        }
        else if (stricmp(argv[i], "-tsr")==0)
        {
            server_state.tsr = 1;
            server_state.noEncryption = 1;
        }
        else if(strcmp(argv[i], "-tsr2")==0 ||
                strcmp(argv[i], "-tsr3")==0 )
        {
            // This used to be doing FolderCacheSetMode(FOLDER_CACHE_MODE_FILESYSTEM_ONLY); (-nopigs), 
            //  but that makes it slow, but it was probably for a good reason, but I'm taking
            //  it out for now.
            handled = 0;
            server_state.noEncryption = 1;
        } else if (strcmp(argv[i], "-charactertransfer")==0)
        {
            // Disable encryption/Networking Startup on these commands
            handled = 0;
            FolderCacheSetMode(FOLDER_CACHE_MODE_FILESYSTEM_ONLY);
            server_state.noEncryption = 1;
        }
        else if (strcmp(argv[i], "-svrconfig")==0)
        {
            if (i+1 < argc)
                svr_config = argv[++i];
            handled = 0;
        }
        else if (stricmp(argv[i], "-cod")==0)
        {
            printf("Using Development data (d_).\n");
            FolderCacheRemoveIgnorePrefix("d_");
            server_state.cod = 1;
        }
        else if (stricmp(argv[i], "-hidetrans")==0)
        {
            msSetHideTranslationErrors(1);
        }
        else if (strcmp(argv[i],"-templates")==0)
        {
            write_templates = 1;
            server_state.noEncryption = 1;
            if (i+1 < argc)
                template_dir = argv[++i];
            printf("Updating database templates for %s\n",template_dir ? template_dir : "gamedatadir");
            
            printf("Not using shared memory due to -templates\n");
            sharedMemorySetMode(SMM_DISABLED);
            sharedHeapTurnOff("disabled");
        }
        else if (strcmp(argv[i], "-createbins")==0)
        {
            if(i + 1 < argc && isdigit((unsigned char)argv[i+1][0]))
                server_state.create_bins = atoi(argv[++i]);
            else
                server_state.create_bins = 1;

            printf("Not using shared memory due to -createbins\n");
            sharedMemorySetMode(SMM_DISABLED);
            sharedHeapTurnOff("disabled");
        }
        else if (strcmp(argv[i], "-unpackmap")==0)
        {
            server_state.unpack_map = 1;
            if(i + 1 < argc) {
                strcpy(db_state.map_name,argv[++i]);
            } else {
                printf("Missing argument to -unpackmap\n");
            }
        }
        else if (strcmp(argv[i], "-notwostagemapxfer")==0)
        {
            dbUseTwoStageMapXfer(false); // defaults to true
        }
        else if (stricmp(argv[i], "-emailOnAssert")==0)
        {
            enableEmailOnAssert(1);
            // add all email addresses to email list
            if (argv[i+1][0]!='-')
            {
                argv[i][0] = 0;
                ++i;
                while (i <= argc-1 && argv[i][0] != '-')
                {
                    if ( argv[i][0] == '[' )
                        setMachineName(argv[i]);
                    else
                        addEmailAddressToAssert(argv[i]);
                    argv[i][0] = 0;
                    if (argv[i+1] && argv[i+1][0] != '-')
                        ++i;
                    else
                        break;
                }
            }

            // I'm doing the 'handled correction' myself, so don't do it for me
            handled = 0;
        }
        else if (stricmp(argv[i], "-nopopup")==0)
        {
            serverErrorfSetNeverShowDialog();
        }
        else if (stricmp(argv[i], "-forcepopup")==0)
        {
            serverErrorfSetForceShowDialog();
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

void parseArgs2(int argc,char **argv)
{
    int        i;
    int        ret;

    db_state.local_server = 1;
    server_state.disableSpecialReturn = 0;

    cfg_setIsBetaShard(0);
    cfg_setMARTY_enabled(0);

    for(i=1;i<argc;i++)
    {
#define IF_CMD(CMD,N_ARGS)                                         \
        else if(0==stricmp(strchrRemoveStatic(argv[i],'_'),CMD)    \
                && (i+N_ARGS) < argc)

        if (strcmp(argv[i],"-db")==0)
        {
            strcpy(db_state.server_name,argv[++i]);
            if (!db_state.log_server[0])
                strcpy(db_state.log_server,argv[i]);
            testClientSetDb(db_state.server_name);
        }
        else if (strcmp(argv[i],"-logserver")==0)
        {
            strcpy(db_state.log_server,argv[++i]);
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
        else if (strcmp(argv[i],"-dumpcon")==0)
        {
            if (i+2 < argc) 
            {
                char *fname = argv[i+1];
                int j = i + 2;
                int * ids;                
                eaiCreate(&ids);
                while (j < argc)
                {
                    eaiPush(&ids,atoi(argv[j]));
                    j++;
                }
                ret = dumpContainers(fname,ids);
            }
            else
            {
                printf("Expected: -dumpcon FNAME (-LISTID CONTAINERID*)*\n");
                ret = 2;
            }
            exit(ret);

        }
        else if (strcmp(argv[i],"-processdump")==0)
        {
            if (i+2 < argc) 
            {
                ret = processDump(argv[i+1],argv[i+2]);
            }
            else
            {
                printf("Expected: -processdump infile outfile\n");
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
        IF_CMD("-querysgleaders",1)
        {
            int ret;
            printf("\n");
            ret = dbQueryGetSgLeaders(argv[1+i]);
            exit(ret);
        }
        else if (strcmp(argv[i],"-udp")==0)
            server_state.udp_port = atoi(argv[++i]);
        else if (strcmp(argv[i],"-cookie")==0)
            db_state.cookie = atoi(argv[++i]);
        else if (strcmp(argv[i],"-tcp")==0)
            server_state.tcp_port = atoi(argv[++i]);
        else if (strcmp(argv[i],"-fullworld")==0)
            server_state.fullworld = 1;
        else if (stricmp(argv[i],"-noKillCars")==0)
            server_state.noKillCars = 1;
        else if (strcmp(argv[i],"-verbose")==0) {
            int val=1;
            if (argc>i+1 && argv[i+1][0]>='0' && argv[i+1][0]<='9') {
                i++;
                val = atoi(argv[i]);
            }
            errorSetVerboseLevel(val);
        } else if (strcmp(argv[i],"-map_id")==0)
        {
            db_state.local_server = 0;
            db_state.map_id = atoi(argv[++i]);
        }
        else if (strcmp(argv[i],"-maxLevel")==0)
        {
            int level = atoi(argv[++i]);
            g_aiMaxLevels[0][0] = g_aiMaxLevels[1][0] = MINMAX( level, 5, g_aiMaxLevels[0][0] );
        }
        else if (strcmp(argv[i],"-maxCoHLevel")==0)
        {
            int level = atoi(argv[++i]);
            g_aiMaxLevels[0][0] = MINMAX( level, 5, g_aiMaxLevels[0][0] );
        }
        else if (strcmp(argv[i],"-maxCoVLevel")==0)
        {
            int level = atoi(argv[++i]);
            g_aiMaxLevels[1][0] = MINMAX( level, 5, g_aiMaxLevels[1][0] );
        }
        else if (strcmp(argv[i],"-completebrokentasks")==0)
            g_ignorebrokentasklist = 0;
        else if (strcmp(argv[i],"-launcher")==0)
            db_state.from_launcher = true;            // a launcher spawned us, notify it when we are ready for players (for load balancing)
        else if (strcmp(argv[i], "-noai")==0)
            server_state.noAI = 1;
        else if (strcmp(argv[i], "-staticlink")==0)
            db_state.staticlink = 1;
        else if (strcmp(argv[i], "-diffdebug")==0)
            db_state.diff_debug = 1;
        else if (strcmp(argv[i], "-colltest")==0)
            coll_test = 1;
        else if (strcmp(argv[i], "-nodebug")==0)
            server_state.nodebug = 1;
        else if (strcmp(argv[i], "-notimeout")==0)
            server_state.notimeout = 1;
        else if (strcmp(argv[i], "-enableInvention")==0)
            ;
        else if (strcmp(argv[i],"-xpscale")==0)
            server_state.xpscale = atof(argv[++i]);
        else if (strcmp(argv[i],"-clientlogouttime")==0)
            server_state.client_logout_time = atoi(argv[++i]);
        else if (strcmp(argv[i],"-influencescale")==0)
            server_state.influencescale = atof(argv[++i]);
        else if (strcmp(argv[i],"-prestigescale")==0)
            server_state.prestigescale = atof(argv[++i]);
        else if (strcmp(argv[i],"-aescale")==0)
            server_state.aescale = atof(argv[++i]);
        else if (strcmp(argv[i],"-aeinfluencescale")==0)
            server_state.aeinfluencescale = atof(argv[++i]);
        else if (strcmp(argv[i],"-aeprestigescale")==0)
            server_state.aeprestigescale = atof(argv[++i]);
        else if (strcmp(argv[i],"-ticketscale")==0)
            server_state.ticketscale = atof(argv[++i]);
        else if (strcmp(argv[i], "-aggrocap") == 0)
            server_state.aggrocap = atof(argv[++i]);
        else if (strcmp(argv[i], "-targetcapmode") == 0)
            server_state.targetcapmode = atof(argv[++i]);
        else if (strcmp(argv[i], "-gridcachebits")==0)
        {
            if(i + 1 < argc)
            {
                grid_cache_bits = atoi(argv[++i]);
            }
        }
        else if ( (i+2)<argc && strcmp(argv[i], "-chatservertest")==0)
        {
            setShardCommTestParams(atoi(argv[i+1]), atoi(argv[i+2]));
            i+=2;
        }
        else if ( (i+1)<argc && stricmp(argv[i], "-chatservernames")==0)
        {
            db_state.local_server = 0; // So as not to get pop up errors
            strcpy(gReservedNameTarget, argv[++i]);
        }
        else if (strcmp(argv[i], "-checkjfd")==0) //mm debug
            server_state.check_jfd = 1;
        else if (strcmp(argv[i], "-nolastauthor")==0)
            g_disableLastAuthor = 1;
        else if (strcmp(argv[i], "-pause")==0)
        {
            server_visible_state.pause = 1;
        }
        else if (strcmp(argv[i], "-packetdebug")==0)
        {
            pktSetDebugInfo();
        }
        else if (strcmp(argv[i], "-prune")==0)
        {
            server_state.prune = 1;
        }
        else if (strcmp(argv[i], "-mapstats")==0)
        {
            server_state.map_stats = 1;
        }
        else if (strcmp(argv[i], "-minimaps")==0)
        {
            server_state.map_minimaps = 1;
        }
        else if (strcmp(argv[i], "-onmissionmap")==0)
        {
            MissionForceLoad(1);
        }
        else if (strcmp(argv[i], "-leveleditor")==0)
        {
            server_state.levelEditor = 1;
            entgenPause();
        }
        else if (strcmp(argv[i], "-leveleditor2")==0)
        {
            server_state.levelEditor = 2;
        }
        else if (strcmp(argv[i], "-nosharedmemory")==0)
        {
            sharedMemorySetMode(SMM_DISABLED);
        }
        else if (strcmp(argv[i], "-mapbatch")==0)
        {
            map_batch_cmds = fileAlloc(argv[++i],0);
            if (!map_batch_cmds)
                FatalErrorf("Can't load file: %s\n",argv[i]);
        }
        else if (stricmp(argv[i], "-donotautogroup")==0)
        {
            server_state.doNotAutoGroup = 1;
        }
        else if (stricmp(argv[i], "-imanartist")==0)
        {
            server_state.doNotAutoGroup = 1;
        }
        else if (stricmp(argv[i], "-quickloadanims")==0)
        {
            server_state.quickLoadAnims    = 1;
        }
        else if (stricmp(argv[i], "-nofx")==0)
        {
            server_state.nofx    = 1;
        }
        else if (stricmp(argv[i], "-notrickreload")==0)
        {
            g_disableTrickReload = 1;
        }
        else if (stricmp(argv[i], "-nominidump")==0)
        {
            setAssertMode(getAssertMode() & ~ASSERTMODE_MINIDUMP);
        }
        else if (stricmp(argv[i], "-assertmode")==0)
        {
            if(i + 1 < argc){
                setAssertMode(atoi(argv[++i]));
            }
        }
        else if (stricmp(argv[i], "-tsr2")==0)
        {
            server_state.tsr = 2;
        }
        else if (stricmp(argv[i], "-tsr3")==0)
        {
            server_state.tsr = 3;
        }
        else if (stricmp(argv[i], "-tsrdbconnect")==0)
        {
            server_state.tsr_dbconnect = 1;
        }
        else if (stricmp(argv[i], "-nostats")==0)
        {
            server_state.nostats = 1;
        }
        else if (stricmp(argv[i], "-ctputms")==0)
        {
            characterTransfer.putMapserver = argv[++i];
        }
        else if (stricmp(argv[i], "-ctgetms")==0)
        {
            characterTransfer.getMapserver = argv[++i];
        }
        else if (stricmp(argv[i], "-ctpath")==0)
        {
            characterTransfer.rootPath = argv[++i];
        }
        else if (stricmp(argv[i], "-charactertransfer")==0)
        {
            characterTransferMonitor();
        }
        else if (stricmp(argv[i], "-ci_interval")==0)
        {
            g_CharacterInfoSettings.iIntervalMinutes = atoi(argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_shard")==0)
        {
            strcpy(g_CharacterInfoSettings.achShardName, argv[++i]);
            sprintf(g_CharacterInfoSettings.achLogFileName,
                "page_generator_%s", g_CharacterInfoSettings.achShardName);
        }
        else if (stricmp(argv[i], "-ci_req")==0)
        {
            strcpy(g_CharacterInfoSettings.achRequestPath, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_html")==0)
        {
            strcpy(g_CharacterInfoSettings.achHTMLDestPath, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_picture")==0)
        {
            strcpy(g_CharacterInfoSettings.achCharacterImageDestPath, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_picturereq")==0)
        {
            strcpy(g_CharacterInfoSettings.achCostumeRequestPath, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_imagewebroot")==0)
        {
            strcpy(g_CharacterInfoSettings.achImageWebRoot, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_htmlwebroot")==0)
        {
            strcpy(g_CharacterInfoSettings.achCharacterWebRoot, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_picturewebroot")==0)
        {
            strcpy(g_CharacterInfoSettings.achCharacterImageWebRoot, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_webdb")==0)
        {
            strcpy(g_CharacterInfoSettings.achWebDBPath, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_image")==0)
        {
            strcpy(g_CharacterInfoSettings.achImagePath, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_after")==0)
        {
            strcpy(g_CharacterInfoSettings.achNameStart, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_before")==0)
        {
            strcpy(g_CharacterInfoSettings.achNameEnd, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_delete")==0)
        {
            g_CharacterInfoSettings.bDeleteMissing = true;
        }
        else if (stricmp(argv[i], "-ci_accesslevel")==0)
        {
            g_CharacterInfoSettings.bIncludeAccessLevelChars = true;
        }
        else if (stricmp(argv[i], "-ci_inactivedays")==0)
        {
            g_CharacterInfoSettings.iInactiveDeleteTimeDays = atoi(argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_alphasubdir")==0)
        {
            g_CharacterInfoSettings.bAlphaSubdir = true;
        }
        else if (stricmp(argv[i], "-ci_template")==0)
        {
            strcpy(g_CharacterInfoSettings.achTemplate, argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_timedelta")==0)
        {
            g_CharacterInfoSettings.iSecsDelta = atoi(argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_usecaptions")==0)
        {
            g_CharacterInfoSettings.bUseCaptions = true;
        }
        else if (stricmp(argv[i], "-ci_ignorelastupdate")==0)
        {
            g_CharacterInfoSettings.bIgnoreLastUpdate = true;
        }
        else if (stricmp(argv[i], "-ci_minlevel")==0)
        {
            g_CharacterInfoSettings.iMinLevel = atoi(argv[++i]);

            // Levels are 0-based internally so adjust to let it be
            // specified as 1-based on the command line.
            if (g_CharacterInfoSettings.iMinLevel > 0)
            {
                g_CharacterInfoSettings.iMinLevel--;
            }
        }
        else if (stricmp(argv[i], "-ci_nohtml")==0)
        {
            g_CharacterInfoSettings.bNoHTML = true;
        }
        else if (stricmp(argv[i], "-ci_showbanned")==0)
        {
            g_CharacterInfoSettings.bShowBanned = true;
        }
        else if (stricmp(argv[i], "-ci_hidebanned")==0)
        {
            g_CharacterInfoSettings.bShowBanned = false;
        }
        else if (stricmp(argv[i], "-ci_norefresh")==0)
        {
            g_CharacterInfoSettings.RefreshMode = false;
        }
        else if (stricmp(argv[i], "-ci_mindiskfree")==0)
        {
            g_CharacterInfoSettings.iMinDiskFreeMB = atoi(argv[++i]);
        }
        else if (stricmp(argv[i], "-ci_lowdiskstalltime")==0)
        {
            g_CharacterInfoSettings.iLowDiskStallTime = atoi(argv[++i]);
        }
        else if (stricmp(argv[i], "-charinfo")==0)
        {
            server_state.charinfoserver = 1;
            setAssertMode(ASSERTMODE_EXIT);
        }
        else if (stricmp(argv[i], "-svrconfig")==0)
        {
            ++i;
        }
        else if (stricmp(argv[i], "-emptymap")==0)
        {
            //EncounterAllowProcessing(0);
            entgenPause();
        }
        else if (stricmp(argv[i], "-restorecharacter")==0)
        {
            server_state.restorecharacter = 1;
        }
        else if (stricmp(argv[i], "-dummyraidbase")==0)
        {
            server_state.dummy_raid_base  = 1;
        }
        else if (stricmp(argv[i], "-dimret")==0)
        {
            server_state.enableBoostDiminishing = atoi(argv[++i]);
        }
        else if (stricmp(argv[i], "-betabase")==0)
        {
            server_state.beta_base = atoi(argv[++i]);
        }
        else if (stricmp(argv[i], "-nobaseedit")==0)
        {
            server_state.no_base_edit = 1;
        }
        else if (stricmp(argv[i], "-clicktosource")==0)
        {
            server_state.clickToSource = 1;
        }
#if NOVODEX
        else if (stricmp(argv[i], "-noragdoll")==0)
        {
            printf("No server side physics or ragdolls due to -noragdoll\n");
            nx_state.notAllowed = 1;
        }
#endif
        else if (stricmp(argv[i], "-blockedmapkeys")==0)
        {
            char *start, *newStr;
            int len = 0;

            if(i+1 < argc)
            {
                start = argv[++i];
                
                do
                {
                    while(start[len] != ',' && start[len] != '\0')
                        len++;
                    newStr = (char*)malloc((++len) * sizeof(char));
                    strncpyt(newStr, start, len);
                    eaPush(&server_state.blockedMapKeys, newStr);
                    start += len;
                    len = 0;
                }while(start[len-1] != '\0');
            }
        }
        else if (stricmp(argv[i], "-mempooldebug")==0)
        {
            mpEnableSuperFreeDebugging();
        }
        else if (stricmp(argv[i], "-enableArchitect")==0)
        {
            // delete me later
        }
        else if (stricmp(argv[i], "-testMissionServerDatabase")==0)
        {
            printf("Will attempt to connect to Mission Server and verify its database due to -testMissionServerDatabase\n");
            server_state.mission_server_test = 1;
        }
        else if (stricmp(argv[i], "-fixupMissionServerDatabase")==0)
        {
            printf("Will attempt to connect to Mission Server and fix up its database due to -fixupMissionServerDatabase\n");
            server_state.mission_server_test = 2;
        }
        else if (stricmp(argv[i], "-loadtestlogserver")==0)
        {
            U32 timer = timerAlloc();
            int tests = atoi(argv[++i]);
            int chars = atoi(argv[++i]);
            int msgs = atoi(argv[++i]);
            int bytes = chars * msgs;
            F32 kibibytes = bytes / 1024.0f;
            F32 elapsed;
            char * line;
            int m, c;
            int interval = 100000 / (chars + 100);

            printf("Will attempt to connect to Log Server and write %d messages of %d characters at interval %d\n", msgs, chars, interval);

            line = malloc(chars+1);
            c = 0;
            for(i = 0; i < chars; i++) {
                line[i] = (c++ % (90-65)) + 65;
            }
            line[i] = 0;

            if(tests & 1<<0) {
                printf("Doing test of logPutMsg\n");
                logWaitForQueueToEmpty();
                timerStart(timer);
                for(m = 0; m < msgs; m++) {
                    logPutMsg(line, "logPutMsg");
                }
                printf("Messages sent in %.2f seconds, waiting for queue to empty\n", timerElapsed(timer));
                logWaitForQueueToEmpty();
                printf("Queue empty in %.2f seconds, waiting for files to commit to disk\n", timerElapsed(timer));
                logShutdownAndWait();
                elapsed = timerElapsed(timer);
                printf("Testing with %.2f Kibits took %.2f seconds (@ %.2f Kibit/s)\n", kibibytes, elapsed, kibibytes / elapsed);
            }

            if(tests & 1<<1) {
                printf("Doing test of compressed logPutMsgSub\n");
                logWaitForQueueToEmpty();
                timerStart(timer);
                for(m = 0; m < msgs; m++) {
                    extern void logPutMsgSub(char *msg_str,int len,char *filename,int zip_file,int zipped, int text_file, char *delim);
                    logPutMsgSub(line, chars+1, "logPushMsgSub", 1, 0, 0, NULL);
                }
                printf("Messages sent in %.2f seconds, waiting for queue to empty\n", timerElapsed(timer));
                logWaitForQueueToEmpty();
                printf("Queue empty in %.2f seconds, waiting for files to commit to disk\n", timerElapsed(timer));
                logShutdownAndWait();
                elapsed = timerElapsed(timer);
                printf("Testing with %.2f Kibits took %.2f seconds (@ %.2f Kibit/s)\n", kibibytes, elapsed, kibibytes / elapsed);
            }

            if(tests & 1<<2) {
                logWaitForQueueToEmpty();
                timerStart(timer);
                printf("Messages sent in %.2f seconds, waiting for queue to empty\n", timerElapsed(timer));
                logWaitForQueueToEmpty();
                printf("Queue empty in %.2f seconds, waiting for files to commit to disk\n", timerElapsed(timer));
                logShutdownAndWait();
                elapsed = timerElapsed(timer);
                printf("Testing with %.2f Kibits took %.2f seconds (@ %.2f Kibit/s)\n", kibibytes, elapsed, kibibytes / elapsed);
            }

            if(tests & 1<<3) {
                loadstart_printf("Connecting to logserver..");
                logNetInit();
                loadend_printf(" done");

                printf("Doing throughput test of logserver\n");
                timerStart(timer);
                for(m = 0; m < msgs; m++) {
                    if(!(m%interval)) {
                        logFlush(0);
                    }
                }
                printf("Messages sent in %.2f seconds, waiting for queue to empty\n", timerElapsed(timer));
                logFlush(1);
                elapsed = timerElapsed(timer);
                printf("Testing with %.2f Kibits took %.2f seconds (@ %.2f Kibit/s)\n", kibibytes*2, elapsed, kibibytes*2 / elapsed);
            }

            if(tests & 1<<4) {
                loadstart_printf("Connecting to logserver..");
                logNetInit();
                loadend_printf(" done");

                printf("Doing trickle test of logserver\n");
                timerStart(timer);
                for(m = 0; m < msgs; m++) {
                    if(!(m%interval)) {
                        logFlush(1);
                        Sleep(1);
                    }
                }
                printf("Messages sent in %.2f seconds, waiting for queue to empty\n", timerElapsed(timer));
                logFlush(1);
                elapsed = timerElapsed(timer);
                printf("Testing with %.2f Kibits took %.2f seconds (@ %.2f Kibit/s)\n", kibibytes*2, elapsed, kibibytes*2 / elapsed);
            }

            timerFree(timer);
            exit(0);
        }
        else if (stricmp(argv[i], "-showLoadMemUsage")==0)
        {
            setShowLoadMemUsage(true);
        }
        else if (stricmp(argv[i], "-isBetaShard") == 0)
        {
            cfg_setIsBetaShard(1);
        }
        else if (stricmp(argv[i], "-MARTY_enabled") == 0)
        {
            cfg_setMARTY_enabled(1);
        }
        else if (stricmp(argv[i], "-NoSpecialReturn") == 0)
        {
            server_state.disableSpecialReturn = 1;
        }
        else if (stricmp(argv[i], "-WeeklyTFTokens") == 0)
        {
            reward_SetWeeklyTFList(argv[++i]);
        }
        else if (stricmp(argv[i], "-transient") == 0)
        {
            server_state.transient = 1;
        }
        else if (stricmp(argv[i], "-idleExitTimeout") == 0)
        {
            int timeout_minutes = atoi(argv[++i]);
            server_state.idle_exit_timeout = timeout_minutes > 0 ? (U32)timeout_minutes : 0;
        }
        else if (stricmp(argv[i], "-idleUpkeep") == 0)
        {
            int idle_minutes = atoi(argv[++i]);
            server_state.maintenance_idle = idle_minutes > 0 ? (U32)idle_minutes : 0;
        }
        else if (stricmp(argv[i], "-dailyUpkeep") == 0)
        {
            int hour_begin = atoi(argv[++i]);
            int hour_end = atoi(argv[++i]);
            if (hour_begin >=0 && hour_end > hour_begin && hour_end <= 24)
            {
                server_state.maintenance_daily_start = hour_begin;
                server_state.maintenance_daily_end = hour_end;
            }
            else
                Errorf("Invalid command line parameters for '-maintenanceHours begin end', begin and end need to be hours from 0-24");
        }
        /* else if (argv[i][0])
        {
            Errorf("Invalid command line parameter passed to mapserver.exe: %s", argv[i]);
        } */
    }
}

static void serverStateInit(int argc,char **argv)
{
    server_state.udp_port = DEFAULT_MAPSERVER_PORT;
    strcpy(db_state.server_name,"localhost");
    strcpy(db_state.log_server,"");
    server_state.enableBoostDiminishing = 1;
    server_state.profiling_memory = 64;
    server_state.doNotAutoGroup = isProductionMode();
    server_state.maintenance_idle = 20;        // idle minutes after which to do periodic maintenance (0 disables)
    serverSetSkyFade(0, 1, 0.0);

    if(!beaconizerIsStarting())
    {
        cmdOldCommonInit(); // Need to be before serverStateLoad(), were these after parseArgs for a reason?
        cmdOldServerInit();
        serverStateLoad(svr_config);
    }

    // If the registry says to pause the server when it starts up, go into an infinite sleeping loop.
    // Someone will have to manually bring the server out of the loop through a debugger.
    if(duGetIntDebugSetting("PauseServerOnStartup"))
    {
        printf("Sleeping until someone debugs me...\n");
        while(1)
        {
            Sleep(100);
        }
    }

    if(!beaconizerIsStarting())
    {
        initBackgroundLoader();
        containerSetDbCallbacks();
        dbCommStartup();
    }
    
    parseArgs2(argc,argv);

    if(db_state.local_server)
        server_state.lock_doors = 1;
    else
        server_state.lock_doors = 0;

    gfxTreeInit();

    if(!beaconizerIsStarting())
    {
        cmdOldServerInitShortcuts();
        groupInit();

        frame_timer = timerAlloc();
        timerStart(frame_timer);
        TIMESTEP = MIN_STEPTIME;
        server_visible_state.timestepscale = 1;
    }

    chareval_Init();
    combateval_Init();
}

#define REMOTE_DEBUG_MAPID 0 // 1020 // set to 0 to disable
// Set the mapid to non-zero if you want to be able to have mapserver go into a loop at startup
//    in order to allow connecting a debugger.  This makes it easier to connect to a specific mapserver
//    when there are hundreds running on the same shard.
#if REMOTE_DEBUG_MAPID
    static int sDelayForRemoteDebuggerFlag = 1;
    #pragma optimize("", off)
#endif

int connectToDbserver(float timeout)
{
    U32        ip_list[3] = {0,0,0};
    static bool done_once = false;
    // In -localmapserver mode, this function is called to reconnect as well as for an initial connection

    setHostIpList(ip_list);

    if (!done_once) {
        loadstart_printf("Listening for clients..");
        verbose_printf("IP: %s / %s   Port: %d\n",makeIpStr(ip_list[0]),makeIpStr(ip_list[1]),server_state.udp_port);

        svrInit();
        loadend_printf(" done");
    }

#if REMOTE_DEBUG_MAPID
    if(db_state.map_id == REMOTE_DEBUG_MAPID) {
        // fpe for test. Near end of map load, set breakpoint here.
        printf("About to connec to dbserver for mapID %d\n", db_state.map_id);
        while(sDelayForRemoteDebuggerFlag) {
            printf("Delaying for debugger attach...\n");
        }
    }
#endif

    if (link_db_server)
    {
        loadstart_printf("Connecting to dbserver..");
        fflush(fileGetStdout());
        if (dbTryConnect(timeout, DEFAULT_DB_PORT)) {
            loadend_printf(" done");
            if (!done_once) {
                loadstart_printf("Connecting to logserver..");
                logNetInit();
                loadend_printf(" done");
            }
            loadstart_printf("Registering with dbserver.. ");
            dbRegister(db_state.map_id,ip_list,server_state.udp_port,server_state.tcp_port,db_state.cookie);
            if (db_state.is_static_map) {
                netLinkSetMaxBufferSize(&db_comm_link, BothBuffers, 8*1024*1024); // Set max size to auto-grow to
                netLinkSetBufferSize(&db_comm_link, BothBuffers, 1*1024*1024);
            } else {
                netLinkSetMaxBufferSize(&db_comm_link, BothBuffers, 1*1024*1024); // Set max size to auto-grow to
                netLinkSetBufferSize(&db_comm_link, BothBuffers, 64*1024);
            }
            if (db_state.local_server) {
                server_state.notimeout = 1;
            }
        } else
        {
            LOG_OLD("Exiting because we failed to finish connecting to the DbServer (it must have crashed or delinked us)");
            if (!done_once)
            {
                exit(-2);
            }
            return 0;
        }
    }

    dbLog("MapServer:Connect", 0, "MapID: %d at IP: %s/%s Port: %d",
        db_state.map_id, makeIpStr(ip_list[0]), makeIpStr(ip_list[1]), server_state.udp_port);

    if (done_once) {
        // Reconnect
        dbReadyForPlayers();
    }

    done_once = true;
    return 1;
}
#if REMOTE_DEBUG_MAPID
    #pragma optimize("", on)
#endif

// mostly a shameless copy-and-paste from filewatcher //////////////////////////
#include "resource.h"

static S32 windowVisible = 1;
static S32 taskBarCreateMessage;

static HWND createWndIconHandler();

static void createShellIcon(S32 forceCreate)
{
    static HICON    hIcon;
    static S32        iconExists;
    static S32        prevWindowVisible;

    if(forceCreate || prevWindowVisible != windowVisible || !iconExists)
    {
        int i;
        NOTIFYICONDATAA data = {0};

        for(i = 0; i < 2; i++)
        {
            prevWindowVisible = windowVisible;

            if(!hIcon)
                hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CRYPTICSVR));

            data.cbSize = sizeof(data);
            data.hWnd = createWndIconHandler();
            data.uID = 0;
            data.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
            data.hIcon = hIcon;
            data.uCallbackMessage = WM_USER;

            strcpy_s(    SAFESTR(data.szTip),
                        windowVisible ?
                            "Shared Memory Preloader\nClick or minimize to HIDE the window." :
                            "Shared Memory Preloader\nDouble-click to SHOW the window." );

            iconExists = Shell_NotifyIconA(iconExists ? NIM_MODIFY : NIM_ADD, &data);

            if(iconExists)
                break;
        }
    }
}

static void showWindow(S32 show)
{
    HWND hwnd = compatibleGetConsoleWindow();

    if(show)
    {
        ShowWindow(hwnd, SW_SHOW);
        if(IsIconic(hwnd))
            ShowWindow(hwnd, SW_RESTORE);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        SetForegroundWindow(hwnd);
    }
    else
    {
        ShowWindow(hwnd, SW_HIDE);
    }

    if(!!show != windowVisible)
    {
        windowVisible = !!show;
        createShellIcon(0);
    }
}

static void deleteTrayIcon(void)
{
    NOTIFYICONDATA data = {0};

    data.cbSize = sizeof(data);
    data.hWnd = createWndIconHandler();
    data.uID = 0;

    Shell_NotifyIcon(NIM_DELETE, &data);
}

static CALLBACK iconWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
        xcase WM_USER:
        {
            static S32 eatDoubleClick;

            // The icon is being interacted with.

            if(    lParam == WM_LBUTTONDOWN ||
                lParam == WM_RBUTTONDOWN)
            {
                if(windowVisible)
                {
                    showWindow(0);
                    eatDoubleClick = 1;
                }
                else
                {
                    eatDoubleClick = 0;
                }
            }
            else if(lParam == WM_LBUTTONDBLCLK ||
                    lParam == WM_RBUTTONDBLCLK)
            {
                if(!eatDoubleClick && !windowVisible)
                    showWindow(1);
            }
        }

        xcase WM_CLOSE:
            deleteTrayIcon();
            exit(0);

        xdefault:
            if(Msg == taskBarCreateMessage)
                createShellIcon(1);
    }

    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

static HWND createWndIconHandler(void)
{
    static HWND hwndIconHandler;

    if(!hwndIconHandler)
    {
        WNDCLASSEXA winClass = {0};

        winClass.cbSize            = sizeof(winClass);
        winClass.style            = CS_OWNDC | CS_DBLCLKS;
        winClass.lpfnWndProc    = (WNDPROC)iconWindowProc;
        winClass.cbClsExtra        = 0;
        winClass.cbWndExtra        = 0;
        winClass.hInstance        = GetModuleHandle(NULL);
        winClass.hCursor        = LoadCursor( NULL, IDC_HAND );
        winClass.hbrBackground    = NULL;
        winClass.lpszMenuName    = "";
        winClass.lpszClassName    = "MapServerTSRWindow";

        RegisterClassExA(&winClass);

        hwndIconHandler = CreateWindowA("MapServerTSRWindow",
                                        "",
                                        WS_POPUP,
                                        0,
                                        0,
                                        300,
                                        300,
                                        NULL,
                                        NULL,
                                        GetModuleHandle(0),
                                        NULL);

        if(!hwndIconHandler)
            FatalErrorf("Can't create icon handler window: %d", GetLastError());

        taskBarCreateMessage = RegisterWindowMessageA("TaskbarCreated");

        createShellIcon(1);
        showWindow(0);

        Sleep(100);
    }

    return hwndIconHandler;
}

static BOOL deleteIconAtExit(DWORD fdwCtrlType)
{
    switch(fdwCtrlType)
    {
        xcase CTRL_CLOSE_EVENT:
        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_C_EVENT:
            deleteTrayIcon();
    }
    return FALSE;
}

static DWORD WINAPI iconThread(void* unused)
{
    HWND hwnd = createWndIconHandler();

    SetConsoleCtrlHandler((PHANDLER_ROUTINE)deleteIconAtExit, 1);

    while(1)
    {
        MSG msg;
        while(PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(windowVisible)
        {
            if(    !IsWindowVisible(compatibleGetConsoleWindow()) ||
                IsIconic(compatibleGetConsoleWindow()) )
            {
                showWindow(0);
            }
        }

        Sleep(100);
    }
}

void compactWorkingSet(bool try_logging)
{
    mpCompactPools();
    heapCompactAll();


    if (try_logging)
    {
        if(strstr(GetCommandLineA(),"-heapinfo"))
            heapInfoPrint();
        if(strstr(GetCommandLineA(),"-heaplog"))
            heapInfoLog();
        if(strstr(GetCommandLineA(),"-smmonitor"))
            memMonitorDisplayStats();
    }

    EmptyWorkingSet(GetCurrentProcess());
}

////////////////////////////////////////////////////////////////////////////////

static void checkTsr()
{
    if (server_state.tsr > 1)
    {
        gridCacheSetSize(0);

        if (server_state.tsr_dbconnect)
        {
            if(!db_state.server_name || !db_state.server_name[0]){
                assertmsg(0, "No dbserver defined (\"-db <server>\" cmdline option)!");
            }else{
                loadstart_printf("Connecting to dbserver...");
                netConnect(&db_comm_link, db_state.server_name, DEFAULT_DB_PORT, NLT_TCP, 20.0f, NULL);
                if(db_comm_link.connected){
                    loadend_printf("done.");
                }else{
                    loadend_printf("FAILED.");
                    exit(0);
                }
            }
        }

        printf("Done loading, press Esc to exit\n");
        if(server_state.tsr > 2)
            _beginthreadex(0, 0, iconThread, 0, 0, 0);

        while (true)
        {
            if (server_state.tsr_dbconnect)
            {
                netLinkMonitor(&db_comm_link, 0, NULL);
                if(!db_comm_link.connected){
                    logShutdownAndWait();
                    exit(0);
                }
            }
            if (kbhit())
                if (_getch()==27)
                    break;
            sharedHeapLazySync();
            Sleep(400);
        }
        exit(0);
    }
    else if (server_state.tsr)
    {
        char tsr_name[MAX_PATH];
        strcpy(tsr_name, getExecutableName());
        strcpy(strstri(tsr_name, ".exe"), "_TSR.EXE");
        remove(tsr_name);
        fileCopy(getExecutableName(), tsr_name);
        strcat(tsr_name, " -tsr3");
        system_detach(tsr_name, 1);
        exit(0);
    }
} 

#ifdef ENABLE_PROFILER
static int profiling = 0;
static int profileNumber = 0;
static U32 profilingTimer = 0;
#endif

static void startProfilingCPU()
{
#ifdef ENABLE_PROFILER
    if(!server_state.profile && !server_state.profile_spikes) {
        return;
    }

    if(!profilingTimer) {
        profilingTimer = timerAlloc();
    }

    BeginProfile(server_state.profiling_memory * 1024 * 1024);
    timerStart(profilingTimer);
    profiling = 1;
#endif
}

static void endProfilingCPU()
{
#ifdef ENABLE_PROFILER
    char profname[MAX_PATH];
    float ms;

    if(!profiling) {
        return;
    }

    ms = timerElapsed(profilingTimer) * 1000.0f;

    snprintf(profname, MAX_PATH, "profile_%d_%dms.txt", profileNumber, (long)ms);

    if(server_state.profile) {
        EndProfile(profname);
        profileNumber++;
        server_state.profile--;
    } else if(server_state.profile_spikes) {
        if(ms >= (float)server_state.profile_spikes) {
            EndProfile(profname);
            profileNumber++;
        } else {
            EndProfile(NULL);
        }
    } else {
        // user just turned it off
        EndProfile(NULL);
    }
    profiling = 0;
#endif
}

//! @brief A callback to over-ride the default behavior of FatalErrorf().
static void fatalErrorCallback(char* errMsg)
{
    // We want FatalErrorf() to assert, so we get a call stack.
    assertmsg(0, errMsg);
    LOG( LOG_CRASH, LOG_LEVEL_ALERT, 0, "Shutting down from fatal error \"%s\"", errMsg);
    logShutdownAndWait();
    exit(-1);
}

int __cdecl main(int argc,char **argv)
{
    int        startup_timer;

#if defined(_M_X64)
    #pragma message("Don't even think about running 64 bit compiled MapServer binaries. The only reason they are compile-able in this state is to simplify batch builds")
#endif
    
    memCheckInit();

    regSetAppName("CoH");

    parseArgs0(argc, argv);

    startup_timer = timerAlloc();

    EXCEPTION_HANDLER_BEGIN

    if (argc==2 && stricmp(argv[1], "-tsr")==0) {
        server_state.tsr = 1;
        checkTsr();
    }

    timeBeginPeriod(1);

    mpCompactPools();
    ErrorfSetCallback(serverErrorfCallback);
    FatalErrorfSetCallback(fatalErrorCallback); // default is to wait for keypress.

    if(!beaconizerIsStarting())
    {
        FolderCacheIgnoreStdPrefixes();
    }
    else
    {
        // Disable Errorf dialog box for beaconizing.
        
        serverErrorfSetNeverShowDialog();
    }
    
    // Do not perform any file io until FolderCacheChooseMode() has been called.
    //        Otherwise, the game will fail to find all of its data files.
    FolderCacheChooseMode();
    FolderCacheEnableCallbacks(0);
    FolderCacheSetManualCallbackMode(1);
    if (isDevelopmentMode()) {
        bsAssertOnErrors(true);
        disableRtlHeapChecking(NULL);
    } else {
        // Only tell the DbServer to auto-delink us if we're in production mode
        setAssertCallback(dbDelinkMeWrapper);
        dirMonSetBufferSize(4096); // Don't use much memory for the DirMonitor, not much should ever need be reloaded anyway!
    }
    // Set the assert mode, if we're launched by a launcher we probably get our assert mode overridden by the DbServer
    // In production mode on the servers we want to save all minidumps timestamped, or possibly full dumps
    setAssertMode(ASSERTMODE_DEBUGBUTTONS |
                (!IsDebuggerPresent() ? (ASSERTMODE_MINIDUMP) : 0) |
                (isProductionMode() ? (ASSERTMODE_DATEDMINIDUMPS|ASSERTMODE_ZIPPED|ASSERTMODE_NOIGNOREBUTTONS) : 0));

    newConsoleWindow();
    if(beaconizerIsStarting())
    {
        FolderCacheSetMode(FOLDER_CACHE_MODE_FILESYSTEM_ONLY);
        sharedMemorySetMode(SMM_DISABLED);
        sharedHeapTurnOff("disabled");
    }

    parseArgs1(argc,argv); // Put this *before* startupInfo so that -nopigs can disable loading of .pig files
    // Nothing above this point should have hit the disk or shared memory at all, all file access must be after parseArgs1()
    // Additionally, no console output can be outputted before this, otherwise it breaks
    // -getcharacter and other -dbquery command lines

    trickGoogleDesktopDll(server_state.silent);

    if(server_state.create_bins)
    {
        ParserForceBinCreation(server_state.create_bins);
        objectLibraryLoadNames(server_state.create_bins);
    }

    loadstart_printf(""); // to time the caching of the directory tree, but not have it be the first message
    startupInfo(argc,argv);

    // start queueing errors
    ErrorvBeginQueueing();

    if(!beaconizerIsStarting()){
        setAssertProgramVersion(getExecutablePatchVersion("CoH Server"));
    }

    sysInit();
    loadstart_printf("serverStateInit.. ");
    serverStateInit(argc,argv);
    loadend_printf("done");

    getServerCmdName(0);
    getClientInpCmdName(0);

    // for character webpages
//     {
//         extern void loadExistInfoCache(void);
//         loadExistInfoCache();
//     }    

    // for character webpages: charweb
    if (server_state.charinfoserver)
    {
        staticMapInfosLoad();
        MapSpecReload();
        MissionSpecReload();
        seqLoadStateBits();
        loadConfigFiles();
        svrInit();
        seqPreloadSeqInfos();
        characterInfoMonitor();
        exit(0);
    }

    if (server_state.restorecharacter)
    {
        staticMapInfosLoad();
        MapSpecReload();
        MissionSpecReload();
        seqLoadStateBits();
        loadConfigFiles();
        svrInit();
        seqPreloadSeqInfos();
        characterRestore();
        exit(0);
    }

    loadstart_printf("reserving shared heap space...");
    {
        U32 uiReservedSize = (U32)reserveSharedHeapSpace(getSharedHeapReservationSize());
        if ( uiReservedSize )
        {
            loadend_printf(" reserved %d MB", uiReservedSize / (1024 * 1024));
        }
        else
        {
            loadend_printf(" failed: no shared heap");
        }
    }

    if (sharedHeapEnabled())
        sharedStringReserve(65536);

    // Check if I'm a beacon client or server.
    loadstart_printf("more miscellany..");
    beaconizerStart();
    staticMapInfosLoad();
    seqLoadStateBits();
    MapSpecReload();
    MissionSpecReload();
    MonorailsLoad();
    loadend_printf("done");

    seqPreloadSeqInfos(); // Must be *before* groupLoadMap

    loadConfigFiles(); // templates get written here.

    if (grid_cache_bits < 0)
    {
        if (db_state.is_static_map)
            grid_cache_bits = 14;
        else
            grid_cache_bits = 12;
    }
    gridCacheSetSize(grid_cache_bits);

    if (!server_state.levelEditor)
    {
        EncounterPreload();
        ScriptDefLoad();
    }

    loadstart_printf("loading sequencers...");

#ifdef RAGDOLL // ragdoll requires all seq info on server
    seqPreLoadPlayerCharacterAnimations(SEQ_LOAD_FULL_SHARED); 
    //seqPreLoadAllAnimations(SEQ_LOAD_FULL);
    //seqPreLoadAllAnimations(SEQ_LOAD_FULL_SHARED);
#else
    seqPreLoadPlayerCharacterAnimations(SEQ_LOAD_HEADER_ONLY); //after PreloadSeqInfos
#endif

    // moved this to ai init to let animlists be verified in behavior aliases
//     parseAnimLists();

    loadend_printf("done");

    // csv dump mode
    if(strstr(GetCommandLineA(),"-dumpspawndefs"))
        DumpSpawndefs();

    // empty the process working set before we connect to the dbserver
    compactWorkingSet(true);

    // check if this is a TSR instance
    checkTsr();

    if(!server_state.create_bins && !server_state.unpack_map)
    {
        cmdCfgLoad();
        connectToDbserver(NO_TIMEOUT);

        if (map_batch_cmds)
            groupdbRunBatch(map_batch_cmds);

        groupLoadMap(db_state.map_name,0,!server_state.beaconProcessOnLoad);
        dbSetupMap();
        dbReadyForPlayers();
    }

    //memCheckDumpAllocs();
    setRuntimePriority();
    if (isDevelopmentMode())
    {
        loadstart_printf("Checking memory.. ");
        assert(heapValidateAll());
        loadend_printf("done");
        loadstart_printf("Verifying SG permission tables.. ");
        sgroup_verifyPermissionTable();
        loadend_printf("done");
    }

    // This needs to stay above the groupLoadMakeAllBin call, since the folder cache needs
    // to get updated when files change that might get loaded in the next group load
    FolderCacheEnableCallbacks(1);

    // if we're creating bin files exit on completion
    if( server_state.map_stats || server_state.map_minimaps)
    {
        server_state.create_bins = 1; // to quash dialog box warnings
        groupLoadMakeAllMapStats();
        return 0;
    }

    if (server_state.create_bins)
    {
        PERFINFO_AUTO_START("server_state.create_bins",1);
        serverParseClient("scmdms", NULL);
        LoadScheduledEvents();
        LoadItemOfPowerInfos();
        plaque_Load();
        groupLoadMakeAllBin(server_state.create_bins); // make sure to do these last because they are slow, and don't tend to cause bugs
        PERFINFO_AUTO_STOP();
        return 0;
    }

    if (server_state.unpack_map) {
        if (db_state.map_name[0]) {
            loadstart_printf("loading map %s...", db_state.map_name);
            groupLoadMap(db_state.map_name, 0, 0);
            loadend_printf("done");
            loadstart_printf("saving map %s...", db_state.map_name);
            groupSave(db_state.map_name);
            loadend_printf("done");
        } else {
            FatalErrorf("-unpack_map requires -map_name");
        }
        return 0;
    }

    rule30Seed(timerCpuTicks64());
    srand(timerCpuTicks64());

    //groupExtractSubtrees( &group_info, test_cb );

    memset(&timing_state, 0, sizeof(timing_state));

    server_visible_state.timestepscale = 1;
//    ai_state.noNPCGroundSnap = isDevelopmentMode();
    server_state.safe_player_count = 310;

    requestItemOfPowerGameStateUpdate();

    // starting any zone scripts for this map
//    ZoneScriptsStart(); // now done in initmap

    // stop queueing errors
    ErrorvEndQueueing();

    printfColor(COLOR_RED|COLOR_GREEN|COLOR_BLUE | COLOR_BRIGHT, "Server ready.");
    printf(" (took %f seconds)\n",timerElapsed(startup_timer));

    // If we are spawned from a launcher notify it that we are ready for players.
    // This lets the launcher update it's metrics on starting maps for load balancing
    if (db_state.from_launcher)
    {
        contactLauncher(); // *After* parsing the -cookie and -map_id args!
    }

    timerFree(startup_timer);
    if (server_state.prune)
        return 0;

    if (coll_test)
        gridCollPerfTest();

    for(;;)
    {
        timerTickBegin();

        startProfilingCPU();

        PERFINFO_AUTO_START("svrTick", 1);
            svrTick();
        PERFINFO_AUTO_STOP_CHECKED("svrTick");

        endProfilingCPU();

        timerTickEnd();

        sleepAndCalcTimers();
    }
    EXCEPTION_HANDLER_END
}

