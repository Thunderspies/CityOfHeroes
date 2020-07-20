/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 */
#include "game.h"
#include "NovodeX/NwWrapper.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/SuperAssert.h>
#include "win/win_init.h"
#include <utilitieslib/utils/fpmacros.h>
#include "clienterror.h"
#include <utilitieslib/utils/FolderCache.h>
#include "clientcomm/autoResumeInfo.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/textparser.h>
#include "UI/uiAutomap.h"
#include "seq/seqstate.h"
#include "sound/sound.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h>
#include "gameData/costume_diff.h"
#include <utilitieslib/version/AppRegCache.h>
#include "win/hwlight.h"
#include <utilitieslib/utils/MemoryMonitor.h>    // for memory tracking
#include "LWC.h"
#include <utilitieslib/utils/RegistryReader.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/process_util.h>
#include "win/win_init.h"

#define UPDATE_PROGRESS_STRING(X) loadstart_printf(X); game_setProgressString(X, NULL, PROGRESSDIALOGTYPE_OK);

char g_GameArguments[2048];

void game_beforeRegisterWinClass(int argc, char **argv)
{
    // Make sure the app name is set properly so we access the correct registry entries
    regSetAppName("CoH");
    getProjectKey(argc, argv);

    // CD - don't put this function in game.c or you will break the costume creator, and I will be very unhappy
    parseArgsForCovFlag(argc, argv);
}

// remind bruce to remove this after the issues ncsoft's new "global auth" get ironed out
#include "clientComm/authclient.h"
#include <utilitieslib/network/sock.h>
void quickAuthCheck(BOOL localhost)
{
    char *u;
    char *p;
    newConsoleWindow(); 

    packetStartup(548,1);
    sockStart();
    if(localhost)
    {
        strcpy(game_state.auth_address,"127.0.0.1");
        u = "dummy001"; p = "11111111";
    }
    else
    {
        strcpy(game_state.auth_address,"172.31.22.226");
        u = "qageneric01"; p = "qa1nternal";
    }
    
    devassert(authLogin(u,p));
    
    acSendAboutToPlay(auth_info.servers[0].id);
   if(auth_info.auth2_enabled != Auth2EnabledState_QueueServer)
       devassert(authWaitFor(AC_PLAY_OK));
   else
       devassert(authWaitFor(AC_HANDOFFTOQUEUE));
}

void radiosityErrorfCallback(char* errMsg)
{
    printf("%s\n", errMsg);
}

void radiosityFatalErrorfCallback(char* errMsg)
{
    printf("%s\n", errMsg);
    assertmsg(0, errMsg);
}

// Find the log file for the NCsoft Launcher and make it a part of the crash reports
void FindLauncherLog()
{
    char logPath[1000];
    char *end;
    const char *launcherRegPath = "HKEY_LOCAL_MACHINE\\SOFTWARE\\NCsoft\\Launcher";
    const char *launcherInstallPathKey = "InstallPath";
    const char *launcherLogFileName = "NCApplicationLog.log";
    RegReader rr = createRegReader();

    if (!initRegReader(rr, launcherRegPath))
    {
        destroyRegReader(rr);
        return;
    }

    // This path should look like: "C:\Program Files (x86)\NCSoft\Launcher\NCLauncher.exe"
    if (!rrReadString(rr, launcherInstallPathKey, logPath, 1000))
    {
        destroyRegReader(rr);
        return;
    }

    // Strip off the executable
    end = strrchr(logPath, '\\');
    if (end == NULL)
    {
        destroyRegReader(rr);
        return;
    }

    end[1] = 0;

    // Should have "C:\Program Files (x86)\NCSoft\Launcher\" now.
    // Append filename
    if (strlen(launcherLogFileName) + strlen(logPath) < 1000)
    {
        strcpy(end + 1, launcherLogFileName);

        setAssertLauncherLogs(logPath);
    }

    destroyRegReader(rr);
}

void SaveArguments(int argc, char **argv)
{
    char buffer[2048];
    int i;
    int index = 0;

    for (i = 0; i < argc; i++)
    {
        int bufUsed = sprintf(buffer, "\"%s\" ", argv[i]);
        if (bufUsed < 0 || index + bufUsed > 204)
            break;

        strcpy(g_GameArguments + index, buffer);
        index += bufUsed;
    }
}

int main(int argc, char **argv)
{
    int timer;
    int maximize;

    memCheckInit();

    timer = timerAlloc();
    
//     quickAuthCheck(TRUE); // TRUE for localhost

    setAssertMode(ASSERTMODE_ERRORREPORT);    // we'll get reset better later

    getProjectKey(argc, argv);

    // Do this before arguments get cleared out
    SaveArguments(argc, argv);

    SET_FP_CONTROL_WORD_DEFAULT;

    ErrorfSetCallback(clientErrorfCallback);
    FatalErrorfSetCallback(clientFatalErrorfCallback);
    
    autoTimerInitThreads();

    if(0){
        autoTimerQueueTimingStart();
    }

    autoTimerSetDepth(100);
    
    autoTimerTickBegin();
    
    PERFINFO_AUTO_START("startup", 1);

    PERFINFO_AUTO_START("top", 1);
    
    game_beforeFolderCacheIgnore(timer, argc, argv);

    //writeConsole(OUTPUT_INFO, "CityOfHeroes client count: %d", game_runningCohClientCount());

    // cov and dev file ignore
    FolderCacheIgnoreStdPrefixes();
    //    FolderCacheAddIgnorePrefix("server/"); // Server data files do not exist on the client, hide them in dev mode!

    // Do this before parseArgs0 where -lwc is processed, and before any data is loaded
    LWC_Init();

    parseArgs0(argc, argv);

    if (!isDevelopmentMode())
    {
        // Production Mode -- validate checksums
        bool bForceFullVerify = (game_state.doFullVerify == 1);
        bool bCancelled; // no dialog if user cancels, they are electing to exit the app.
        if(!game_validateChecksums(bForceFullVerify, &bCancelled) ) {
            // Make sure this file is saves with UTF-8 encoding or this string may not show up properly
            if(!bCancelled) {
                // Verification failed.  Notify user they need to repair.
                char* str;
                int locale = locGetIDInRegistry();

                if(locale == LOCALE_ID_GERMAN)
                    str = "Fehlende oder beschädigte Spiel-Dateien wurden erfasst. Bitte benutzen Sie die \"Reparieren\" Funktion in dem Ncsoft Launcher, um diese Fehler aufzulösen. Das Spiel fährt jetzt runter.";
                else if(locale == LOCALE_ID_FRENCH)
                    str = "Des données de jeu corrompues ou manquantes ont été détectées. Veuillez lancer la fonction \"Réparer\" au travers du NCsoft Launcher pour résoudre ce problème. Le jeu va maintenant se fermer.";
                else
                    str = "We have detected missing or corrupt game files, please run the \"Repair\" option in NCsoft Launcher to fix this. The game will now exit.";
                winMsgAlert(str);
                exit(0);
            } else {
                // User cancelled the verify.  We already warned them that this isn't wise, so let them continue into the game.
            }
        }
    }

    // parseArgs0() above will set game_state.usedLauncher if "-launcher" is present on the command line
    if (game_state.usedLauncher)
    {
        FindLauncherLog();
    }

    // localized strings are loaded here
    game_beforeParseArgs(1);

    //Do after getAutoResumeInfo() so game_state.fullscreen will be correct
    parseArgs(argc,argv);

    /*if (isDevelopmentMode())
        printf("%s\n", GetCommandLine());*/

    // queue all loading errors
    ErrorvBeginQueueing();

    // if we are creating bin files, tell parser
    if (game_state.create_bins)
        ParserForceBinCreation(1);

    PERFINFO_AUTO_STOP_START("middle", 1);

    if(game_state.ask_no_audio)
    {
        extern AudioState g_audio_state;
        g_audio_state.noaudio = winMsgYesNo("Do you want to DISABLE SOUND?");
    }

    if(game_state.ask_local_map_server)
        game_state.local_map_server = winMsgYesNo("Connect to LOCAL MAP SERVER?");

    if(game_state.local_map_server)
        printf("connecting to LOCAL MAP SERVER\n");

    if(game_state.ask_quick_login)
        game_state.quick_login = winMsgYesNo("Do you want to use QUICK LOGIN?");

    if (game_state.enableHardwareLights) {
        hwlightInitialize();
        writeConsole(OUTPUT_INFO, "Initialized hardware lights");
    }

    seqLoadStateBits();

    PERFINFO_AUTO_STOP_START("middle2", 1);

    maximize = game_loadSoundsTricksFonts(argc, argv);

    PERFINFO_AUTO_STOP_START("middle3", 1);

    if (game_state.create_bins)
    {
        game_state.fullscreen = 0;
        game_state.screen_x = 800;
        game_state.screen_y = 600;
    }

    game_initWindow(maximize);

    PERFINFO_AUTO_STOP_START("middle4", 1);

    game_networkStart();

    PERFINFO_AUTO_STOP_START("game_loadData", 1);

    //get account name and password from c:resume_info.txt if you are running -cryptic
    //No, instead, always use resume_info if you have it, that way cryptic people can use resume_info when running off the patch
    //if( game_state.cryptic )
    getAutoResumeInfoCryptic();

    game_loadData(0);

    // init sound in development mode and in create-bins mode
    if (game_state.create_bins || FolderCacheGetMode() == FOLDER_CACHE_MODE_DEVELOPMENT_DYNAMIC)
        sndInit();

    // exit before main loop and graphics stuff if we're just creating binaries
    if (game_state.create_bins)
    {
        cmdAccessOverride(10);
        cmdParse("cmdms");
        mapperLoadCityInfo();
        writeConsole(OUTPUT_DEBUG, "Client binning process complete!");
        /*while (kbhit())
            getch();
        printf("Press any key to quit.");
        getch();*/
        return 0;
    }

    if( game_state.costume_diff )
    {
        return costumeReportDiff(game_state.costume_diff);
    }

    PERFINFO_AUTO_STOP_START("bottom", 1);

    game_beforeLoop(0, timer);
    
    PERFINFO_AUTO_STOP();

    // finish queueing errors
    ErrorvEndQueueing();

    PERFINFO_AUTO_STOP(); // "startup"
    
    autoTimerTickEnd();


    return game_mainLoop(timer);
}

