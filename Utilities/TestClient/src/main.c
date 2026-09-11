#include "testClientInclude.h"
#include "testEmail.h"
#include "testTeam.h"
#include "testFollow.h"
#include "testUtil.h"
#include "testConnections.h"
#include "testClient/testLevelup.h"
#include "testNetworkStats.h"
#include "testContact.h"
#include "testMissions.h"
#include "cmdparse/cmdgame.h"
#include "seq/seq.h"
#include "gameComm/initClient.h"
#include "clientcomm/clientcomm.h"
#include "entity/load_def.h"
#include <utilitieslib/network/sock.h>
#include "UI/uiLogin.h"
#include "UI/uiEditText.h"
#include "gameData/costume_data.h"
#include "gameData/BodyPart.h"
#include "gameData/randomCharCreate.h"
#include "gameComm/Npc.h"
#include "cmdparse/cmdcommon.h"
#include "entity/entrecv.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/error.h>
#include "player/player.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/textparser.h>
#include "PipeClient.h"
#include <conio.h>
#include <time.h>
#include <process.h>
#include "entity/character_base.h"
#include "entity/entclient.h"
#include "entity/entVarUpdate.h"
#include "gameComm/itemselect.h"
#include "player/pmotion.h"
#include "fighter.h"
#include "SlaveLauncher.h"
#include "mover.h"
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/utils/memcheck.h>
#include "clientcomm/dbclient.h"
#include "clientcomm/authclient.h"
#include "chatter.h"
#include "testClient/TestClient.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/network/net_version.h>
#include "packetFlood.h"
#include "entity/character_level.h"
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/SharedHeap.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/version/AppVersion.h>
#include <utilitieslib/version/AppRegCache.h>
#include "entity/entPlayer.h"
#include "gameComm/dooranimcommon.h"
#include <utilitieslib/utils/Stackdump.h>
#include "direct.h"
#include "entity/netfx.h"
#include "entity/entity.h"
#include "ArenaFighter.h"
#include "seq/seqstate.h"
#include "auth/authUserData.h"
#include <utilitieslib/network/crypt.h>
#include "tchar.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StringCache.h>
#include "testClientCmdParse.h"
#include "testAccountServer.h"
#include "testMissionSearch.h"
#include "comm_backend.h"
#include <utilitieslib/utils/log.h>
#include "player/inventory_client.h"
#include "entity/character_eval.h"
#include "entity/character_combat_eval.h"

char *flag_names[] = {
    "",
    "",
    "",
    "FIGHT",
    "MAPMOVE",
    "MOVE",
    "CHAT",
    "TEAMACCEPT",
    "TEAMINVITE",
    "TEAMTHRASH",
    "RANDOMDISCONNECT",
    "TIMED_RECONNECT",
    "FOLLOW",
    "LEVELUP",
    "SUPERGROUPACCEPT",
    "SUPERGROUPINVITE",
    "SUPERGROUPMISC",
    "AUTOREZ",
    "CHATNPC",
    "NOIGNORE",
    "MISSIONS",
    "EMAIL",
    "ARENAFIGHTER",        //bot that will go into arena events
    "KILLBOT",            //nextplayers around, sets health to 1 and attacks opponents
    "EVENTCREATOR",        //creates events rather than joining
    "ARENATHRASHER",    //sends lots of random arena commands to the server (as well as real ones)
    "ARENASUPERGROUP",    //does supergroup related arena stuff
    "ARENASCHEDULED",    //participates in scheduled (single and multiround) events
    "SUPERGROUPLEAVE",
    "AUCTION",
    //NOTE still one slot left at the end
};

char *flag2_names[] = {
    "LEAGUEACCEPT",
    "LEAGUEINVITE",
    "LEAGUETHRASH",
    "TURNSTILE",
    "MISSIONSTRESS",
    "MAPSTRESS",
    "VERBOSE",
};

char *mission_server_test_names[] = {
    "MISSIONSEARCH",
    "MISSIONVOTE",
    "MISSIONPLAY",
    "MISSIONPUBLISH",
};

char *account_server_test_names[] = {
    "ACCOUNTSERVER",
    "ACCOUNTSERVERSUPERPACKS",
    "ACCOUNTSERVERCERTIFICATIONGRANTS",
    "ACCOUNTSERVERCERTIFICATIONCLAIMS",
};

char *display_server_names[] = {
        "Freedom",
        "Justice",
        "Pinnacle",
        "Virtue",
        "Liberty",
        "Guardian",
        "Infinity",
        "Protector",
        "Victory",
        "Champion",
        "Triumph",
        "Defiant",
        "Zukunft",
        "Vigilance",
        "Union",
        "Exalted",
};

void mainloop(void);

extern TokenizerParseInfo ParseCostumeSet[];
extern Entity    *current_target;
extern int do_map_xfer;
extern char glob_map_name[MAX_PATH];
extern int glob_plrstate_valid;

extern void aucscript_Thrash(Entity *e, bool buy, bool sell, int sleep_time);

extern void aucscript_SellAndBuy25(bool start);
extern void aucscript_SellOrBuy50(bool start, bool buy);
extern void aucscript_Seed50(void);
extern void aucscript_GetAll(void);

MissionServerTestMode g_testModeMission = 0;
AccountServerTestMode g_testModeAccount = 0;

#ifdef TEXT_CLIENT
TestMode g_testMode = TEST_LOGIN | TEST_RESUME_CHAR | TEST_STAY_CONNECTED;
TestMode2 g_testMode2 = 0;
#else
TestMode g_testMode = TEST_LOGIN | TEST_RESUME_CHAR | TEST_CREATE_CHAR | TEST_STAY_CONNECTED | TEST_TEAMACCEPT | TEST_FOLLOW | TEST_SUPERGROUPACCEPT | TEST_LEVELUP;
TestMode2 g_testMode2 = TEST2_LEAGUE_ACCEPT;
#endif

// Do we or do we not parse chat starting with @ into commands
int gbParseChatToCommand=0;

int test_client_num=-1;
char launcher_host[256] = ".";
char connserver[256] = "";
char server[256] = "";
int show_hp_changes=0;
int g_nobrawl=0;
int ask_user=0;
int useLauncherVersion = 1;
char character_name[256] = {0};
int err=0;
int reconnect=0;
const char *launcherVersion = NULL;
int login_retry_count = 2;
int report_to_launcher = 1;
int server_num=-1; // Which server in the list the AuthServer returns to connect to 
int sleep_time = 200;
U32 g_mission_hop_time = 30; // seconds on mission then hop
bool g_verbose_client;
char g_achPasswordTC[32];
bool dontpause = false;
int testClientFakeAuth = 0;

void gameStateInit() {
    strcpy(game_state.cs_address, connserver);
    game_state.no_version_check = 1;
    strcpy(game_state.address, server);
    cmdOldSetSends(control_cmds,1);
    //
}

void genLoginInfo() {
    static char name[256] = "";
    static char password[256] = "11111111";
    sprintf(name, "Dummy%05d", (test_client_num+1));
    if (ask_user) {
        printf("Username [%s]: ", name);
        gets_s(name, sizeof(name));
        printf("Password [11111111]: ");
        gets_s(password, sizeof(password));
        if (stricmp(password, "")==0) {
            strcpy(password, "11111111");
        }
    }
    if (!name[0]) {
        sprintf(name, "Dummy%05d", (test_client_num+1));
    }
    strcpy(g_achPasswordTC, password);
    strcpy(g_achAccountName, name);
}

extern PipeClient pc=NULL;

void sendMessageToLauncher(const char *fmt, ...) {
    char str[20000] = {0};
    va_list ap;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    PipeClientSendMessage(pc, str);
}

void statusUpdate(const char *fmt, ...) {
    char str[20000];
    char *base = "Status: ";
    va_list ap;

    strcpy(str, base);

    va_start(ap, fmt);
    vsprintf_s(str + strlen(base), ARRAY_SIZE_CHECKED(str) - strlen(base), fmt, ap);
    va_end(ap);

    PipeClientSendMessage(pc, str);
}

void broadcastStats() 
{
    char buf[4096] = "";
    char *working=buf;
    Entity *e = playerPtr();

    if (!e) 
        return;

    sprintf(buf, "b Login: %s\n", g_achAccountName);
    commAddInput(buf);
    sprintf(buf, "b Level: %d/%d\n", e->pchar->iLevel+1, character_CalcExperienceLevel(e->pchar)+1);
    commAddInput(buf);
    sprintf(buf, "b HP: %1.3f/%1.3f\n", e->pchar->attrCur.fHitPoints, e->pchar->attrMax.fHitPoints);
    commAddInput(buf);
    sprintf(buf, "b XP: %d\n", e->pchar->iExperiencePoints);
    commAddInput(buf);
    sprintf(buf, "b Debt: %d\n", e->pchar->iExperienceDebt);
    commAddInput(buf);
    sprintf(buf, "b Influence: %d\n", e->pchar->iInfluencePoints);
    commAddInput(buf);
}


void checkPlayer(void) {
    static int doneit=0;
    if (!doneit && playerPtr()) {
        char buf[256];
        sprintf(buf, "Running: %s (%s)", playerPtr()->name, g_achAccountName);
        setConsoleTitle(buf);
        sendMessageToLauncher("Player: %s", playerPtr()->name);
        doneit=1;
    }
    if (playerPtr()) {
        static float oldhp = -1;
        static int countdown = -1;
        static int setpos_countdown = -1;
        static int report_countdown = -1;
        char buf[512];
        float hp = playerPtr()->pchar->attrCur.fHitPoints;
        const F32 *pos = ENTPOS(playerPtr());

        report_countdown--;
        countdown--;

        if (hp!=oldhp && report_countdown <=0 && report_to_launcher) {
            sprintf(buf, "HP: %1.1f/%1.1f", hp, playerPtr()->pchar->attrMax.fHitPoints);
            if (show_hp_changes)
                printf("%s\n", buf);
            oldhp = hp;
            report_countdown = 10*5; // About once a 10 seconds
            sendMessageToLauncher("%s", buf);
            if (hp<=0) {
                //statusUpdate("Dead");
                countdown = 45;
                stats.deathcount++;
            } else if (countdown) {
                countdown=-1;
//                statusUpdate("Running");
            }
        }
        if (countdown<=0 && (g_testMode & TEST_AUTOREZ)) {
//            statusUpdate("Running");
            if (hp<=0) {
//                char buff[1024];
                EMPTY_INPUT_PACKET(CLIENTINP_DEAD_NOGURNEY_RESPONSE);
//                sprintf(buff, "entcontrol me sethealth %1.3f\nentcontrol me setendurance %1.3f",
//                    playerPtr()->pchar->attrMax.fHitPoints, playerPtr()->pchar->attrMax.fEndurance);
//                commAddInput(buff);
                countdown = 45;
            }
        }
        if (stricmp(playerPtr()->name, character_name)!=0) {
            printf("Old (expected) name: %s\n", character_name);
            printf("New name: %s\n", playerPtr()->name);
            strcpy(character_name, playerPtr()->name);
        }
#define OUT_OF_RANGE 16000
        if (setpos_countdown<=0 && (pos[0] <-OUT_OF_RANGE || pos[0] >OUT_OF_RANGE ||
            pos[1] <-OUT_OF_RANGE || pos[1] >OUT_OF_RANGE ||
            pos[2] <-OUT_OF_RANGE || pos[2] >OUT_OF_RANGE))
        {
            printf("I seem to be out of range, running STUCK (%1.3f %1.3f %1.3f)\n", pos[0], pos[1], pos[2]);
            commAddInput("stuck");
            setpos_countdown = 10;
        }
        if (setpos_countdown>0)
            setpos_countdown--;
    }
}

void checkReportNetworkStats(void)
{
    static int last_lost_sent=0;
    static int last_lost_recv=0;
    static int countdown = 60*5;
    int diff = ABS(comm_link.lost_packet_sent_count - last_lost_sent) +
        ABS(comm_link.lost_packet_recv_count - last_lost_recv);

    countdown--;
    if (report_to_launcher && countdown<=0 && (diff > 0) || (diff > 20))
    {
        sendMessageToLauncher("LostCount: %d/%d", comm_link.lost_packet_sent_count, comm_link.lost_packet_recv_count);
        last_lost_sent = comm_link.lost_packet_sent_count;
        last_lost_recv = comm_link.lost_packet_recv_count;
        countdown = 60*5;
    }
}

bool isNumeric(const unsigned char *s) {
    const unsigned char *c;
    for (c=s; *c; c++) {
        if (!isdigit(*c))
            return false;
    }
    return true;
}

#define CMDEQ(s) (stricmp(argv[i], s)==0)
void checkArgs(int argc, char **argv) {
    int i;
    test_client_num = 0;
    for (i=1; i<argc; i++) {
        if (CMDEQ("-hideconsole")) {
            hideConsoleWindow();
        } else if (CMDEQ("-authname")) {
            i++;
            strcpy(g_achAccountName,argv[i]);
        } else if (CMDEQ("-password")) {
            i++;
            strcpy(g_achPasswordTC,argv[i]);
        } else if (CMDEQ("-character")) {
            strncpy(character_name,argv[++i], sizeof(character_name));
            g_testMode |= TEST_RESUME_CHAR;
            printf("Will attempt to resume character %s...\n",character_name);
        } else if (CMDEQ("-justlogin")) {
            g_testMode = TEST_LOGIN;
        } else if (CMDEQ("-fakeauth")) {
            testClientFakeAuth = 1;
        } else if (CMDEQ("-selfversion")) {
            useLauncherVersion = 0;
        } else if (CMDEQ("-version")) {
            launcherVersion = argv[i] + ARRAY_SIZE("-version");
            printf("cmd line launcherVersion=%s\n",launcherVersion);
        } else if (CMDEQ("-retry")) {
            login_retry_count = -1;
        } else if (CMDEQ("-askuser")) {
            ask_user = 1;
        } else if (CMDEQ("-silent")) {
            report_to_launcher = 0;
        } else if (CMDEQ("-parsechat")) {
            gbParseChatToCommand = 1;
        } else if (CMDEQ("-nolauncherchat")) {
            g_sendChatToLauncher = 0;
        } else if (CMDEQ("-slavelauncher")) {
            startSlaveLauncher( argv[i+1] );
            exit(-1);
        } else if (CMDEQ("-host")) {
            i++;
            strcpy(launcher_host, argv[i]);
        } else if (CMDEQ("-dontpause")) {
            dontpause = true;
        } else if (CMDEQ("-disconnect")) {
            g_testMode = g_testMode & (~TEST_STAY_CONNECTED);
        } else if (CMDEQ("-randomdisconnect")) {
            g_testMode |= TEST_RANDOM_DISCONNECT;
        } else if (CMDEQ("-disconnectat")) {
            g_testMode |= TEST_RANDOM_DISCONNECT;
            i++;
            drop_stage = atoi(argv[i]);
        } else if (CMDEQ("-timedreconnect")) {
            g_testMode |= TEST_TIMED_RECONNECT;
        } else if (CMDEQ("-evilchat")) {
            g_testMode |= TEST_CHAT;
            setEvilChat(1);
        } else if (CMDEQ("-cs") || CMDEQ("-db")) {
            i++;
            strcpy(connserver, argv[i]);
        } else if (CMDEQ("-auth")) {
            i++;
            strcpy(game_state.auth_address, argv[i]);
        } else if (CMDEQ("-serverchoice")) {
            i++;
            server_num = atoi(argv[i]);
        } else if (CMDEQ("-localmapserver")) {
            game_state.local_map_server = 1;
        } else if (CMDEQ("-server")) {
            char noquote[1024];
            i++;
            if (!strstr(argv[i],"\"")) {
                // Doesn't start with a ", so just copy it
                strcpy(server,argv[i]);
            }
            else {
                strcpy(noquote,&((argv[i])[1]));
                while (strstr(noquote,"\"")==NULL) {
                    strcat(noquote," ");
                    strcat(noquote,argv[++i]);
                }
                strncpy(server,noquote,strlen(noquote)-1);
            }
        } else if (CMDEQ("-notimeout")) {
            control_state.notimeout = 1;
        } else if (CMDEQ("-packetdebug")) {
            pktSetDebugInfo();
            bsAssertOnErrors(true);
        } else if (CMDEQ("-badnetwork")) {
            setBadNetwork();
        } else if (CMDEQ("-nolevel")) {
            g_testMode &= (~TEST_LEVELUP);
        } else if (CMDEQ("-flood")) {
            i++;
            if (i > argc - 4) {
                printf("expected: -flood IPADDR PORT FloodType NumAttacks\n");
            } else {
                packetFlood(argv[i], atoi(argv[i+1]), atoi(argv[i+2]), atoi(argv[i+3]));
            }
            exit(0);
        } else if (CMDEQ("-cov")) {
            game_state.skin = UISKIN_VILLAINS;
        } else if (CMDEQ("-cod")) {
            game_state.cod = 1;
            printf("Using development data (d_).\n");
            FolderCacheRemoveIgnorePrefix("d_");
        } else {
            if (isNumeric(argv[i])) {
                test_client_num = atoi(argv[i]);
            } else {
                char buf[256];
                sprintf(buf, "mode %s", argv[i]);
                processCmd(buf,1);
            }
        }
    }
    if (testClientFakeAuth && (!connserver[0] || game_state.auth_address[0])) {
        printf("-fakeauth requires -db (or -cs) and cannot be combined with -auth.\n");
        exit(1);
    }
    if (ask_user) {
        g_testMode &= ~(TEST_LEVELUP | TEST_EMAIL);
    }
}

// handle shared memory command line flags before the first use
static bool process_cmdline_for_sharedmemory(int argc, char **argv) {
    int i;
    for (i=1; i<argc; i++) {
        if (CMDEQ("-nosharedmemory")) {
            printf(" ** WARN **: Not using shared memory due to -nosharedmemory\n");
            sharedMemorySetMode(SMM_DISABLED);
            sharedHeapTurnOff("disabled");
            return false;
        } else if (CMDEQ("-testsharedmemory")) {
            testSharedMemory();
            error_exit(0);
            return false;
        } else if (CMDEQ("-printsharedmemory")) {
            printSharedMemoryUsage();
            error_exit(0);
            return false;
        }
    }
    return true;    // use shared memory
}

void getScreen() {
    COORD size;
    PCHAR_INFO buffer;
    char buf[100000];
    char *pbuf;
    int i, j, w;

    consoleCapture();
    buffer = consoleGetCapturedData();
    consoleGetCapturedSize(&size);
    while ((size.X+1)*size.Y >= ARRAY_SIZE(buf)) {
        size.Y--;
    }
    pbuf = buf;
    for (i=0; i<size.Y; i++) {
        for (w=size.X-1; w && buffer[i*size.X + w].Char.AsciiChar == ' '; w--);
        for (j=0; j<=w; j++) {
            *pbuf++ = buffer[i*size.X + j].Char.AsciiChar;
        }
        *pbuf++ = '\n';
    }
    *pbuf++ = '\0';

    PipeClientSendMessage(pc, "SCREEN:%s", buf);
}


void handleLauncherMessage(char *msg)
{
    printf("MESSAGE FROM LAUNCHER: %s\n",msg);
    if (stricmp(msg, "SCREEN")==0) {
        getScreen();
        return;
    }
    if (strstri(msg, "CMD ") == msg) {
        char * mycmd = &(msg[4]);
        if (strstr(mycmd, "/") == mycmd)
            mycmd = &(msg[5]);
        if (!testClientCmdParse(mycmd))
        {
            commAddInput(mycmd);
        }
    }
    if (strstri(msg, "MODE ") == msg) {
        processCmd(msg,0);
    }
    if(!strnicmp(msg,"SLEEP ",6))
    {
        int tmp = atoi(msg+6);
        if(tmp)
        {
            sleep_time = tmp;
            printf("Sleep Time set to %ims\n",sleep_time);
        }
    }
}

int checkPipe() {
    if (pc) { // Disconnect if the Pipe is broken
        char *msg;
        while (msg = PipeClientGetMessage(pc)) {
            handleLauncherMessage(msg);
        }
        if (pc->connected==PCS_DISCONNECTED) {
            // We were dropped, exit!
            printf("Broken pipe\n");
            return 1;
        }
    }
    return 0;
}

int error_exit(int allow_debug) {
    if (!dontpause) {
        if (allow_debug)
            printf("\nPress 'd' to debug, any other key to exit.\n");
        while (!kbhit()) {
            if (checkPipe()) {
                break;
            }
            Sleep(500);
        }
        if (allow_debug && kbhit() && getch()=='d')
            return 1;
    }
    exit(-1);
    return 0;
}

void errorCallback(char *errMsg) {
    if (!testClientSafeError(errMsg)) {
        printf("Errorf: %s\n", errMsg);
    }
}

void fatalErrorCallback(char *errMsg) {
    printf("Fatal Error: %s\n", errMsg);
    setConsoleTitle("ERROR");
    statusUpdate("ERROR");
    error_exit(0);
}

char errorBuff[1024];
char stackdump[1024*10];

static int CrtReportingFunction( int reportType, char *userMessage, int *retVal )
{
    if (reportType == _CRT_WARN) {
        return FALSE;
    }
    printf("\nCRT Error: %s\n", userMessage);
    sdDumpStackToBuffer(stackdump, ARRAY_SIZE(stackdump)-1, NULL);
    printf("Call stack:\n%s\n", stackdump);
    setConsoleTitle("CRASH");
    statusUpdate("CRASH");
    if (error_exit(1)) {
        *retVal = 1;
        return 1;
    } else {
        *retVal = 0;
        return 0;
    }
}

extern char* GetExceptionName(DWORD code);
int tcAssertExcept(unsigned int code, PEXCEPTION_POINTERS info)
{
    printf("\nException caught: %s\n", GetExceptionName(code));
    sdDumpStackToBuffer(stackdump, ARRAY_SIZE(stackdump)-1, info->ContextRecord);
    printf("Call stack:\n%s\n", stackdump);
    setConsoleTitle("CRASH");
    statusUpdate("CRASH");
    if (error_exit(1)) {
        return EXCEPTION_CONTINUE_SEARCH;
    } else {
        return EXCEPTION_EXECUTE_HANDLER;
    }
}

extern int dbWaitForStartOrQueue(F32 timeout);
int loginWrapper(void) {
    int ret;
    if (!authLogin(g_achAccountName,g_achPasswordTC))
        return 0;

    testClientRandomDisconnect(TCS_loginWrapper);
    if (game_state.auth_address[0]) {
        if (server_num == -1) {
            if (ask_user) {
                int i;
                printf("Available Servers:\n");
                for (i=0; i<auth_info.server_count; i++) {
                    printf("  %d %s (%s) (%d/%d) %s\n", 1 + i, auth_info.servers[i].name, makeIpStr(auth_info.servers[i].ip), auth_info.servers[i].curr_user_count, auth_info.servers[i].max_user_count, auth_info.servers[i].server_status?"up":"DOWN");
                }
                server_num = promptRanged("Which server: ", auth_info.server_count);
            } else {
                server_num=0;
                if (server[0]) {
                    int i;
                    printf("Available Servers:\n");
                    for (i=0; i<auth_info.server_count; i++) {
                        printf("  (%s) %d %s (%s) (%d/%d) %s\n", display_server_names[i], 1 + i, auth_info.servers[i].name, makeIpStr(auth_info.servers[i].ip), auth_info.servers[i].curr_user_count, auth_info.servers[i].max_user_count, auth_info.servers[i].server_status?"up":"DOWN");
                        if (!stricmp(server,display_server_names[i])) {
                            server_num=i;
                            break;
                        }
                    }
                }
                printf("Using server %d (%s)\n",server_num,auth_info.servers[server_num].name);
            }
        }
        acSendAboutToPlay(auth_info.servers[server_num].id);
        if (!authWaitFor(AC_PLAY_OK))
            return 0;
        authDisconnect();
    } else {
        server_num = 0;
    }
    ret = dbConnect(makeIpStr(auth_info.servers[server_num].ip),auth_info.servers[server_num].port,
        auth_info.uid,auth_info.game_key,g_achAccountName,
        1,NO_TIMEOUT,true);

    if(ret == DBGAMESERVER_QUEUE_POSITION) {
        setConsoleTitle("Queued...");
        statusUpdate("Queued");
        while((ret=dbWaitForStartOrQueue(control_state.notimeout?NO_TIMEOUT:30.0f)) == DBGAMESERVER_QUEUE_POSITION) {
            int pos, count;
            if(dbclient_getQueueStatus(&pos, &count)) {
                printf("Queued (%d of %d)\n", pos, count);
            }
            Sleep(1000);
        }
    }
    return ret;
}

void processCmd(char *buf, int quiet) {
    if (strnicmp(buf, "mode ", 5)==0) {
        int on;
        int i;
        if (!quiet) printf("Current enabled modes:\n");
        for (i=0; i<ARRAY_SIZE(flag_names); i++) {
            if (flag_names[i] && flag_names[i][0] && (g_testMode & (1<<i))) {
                if (!quiet) printf("\t%s\n", flag_names[i]);
            }
        }
        for (i=0; i<ARRAY_SIZE(flag2_names); i++) {
            if (flag2_names[i] && flag2_names[i][0] && (g_testMode2 & (1<<i))) {
                if (!quiet) printf("\t%s\n", flag2_names[i]);
            }
        }

        for (i=0; i<ARRAY_SIZE(mission_server_test_names); i++) {
            if (mission_server_test_names[i] && mission_server_test_names[i][0] && (g_testModeMission & (1<<i))) {
                if (!quiet) printf("\t%s\n", mission_server_test_names[i]);
            }
        }

        for (i=0; i<ARRAY_SIZE(account_server_test_names); i++) {
            if (account_server_test_names[i] && account_server_test_names[i][0] && (g_testModeAccount & (1<<i))) {
                if (!quiet) printf("\t%s\n", account_server_test_names[i]);
            }
        }

        if (buf[5]=='+')
            on=1;
        else if (buf[5]=='-')
            on=0;
        else if (buf[5]=='?') {
            if (!quiet) printf("Available flag toggles:\n");
            for (i=0; i<ARRAY_SIZE(flag_names); i++) {
                if (flag_names[i] && flag_names[i][0]) {
                    if (!quiet) printf("\t%s\n", flag_names[i]);
                }
            }
            for (i=0; i<ARRAY_SIZE(flag2_names); i++) {
                if (flag2_names[i] && flag2_names[i][0]) {
                    if (!quiet) printf("\t%s\n", flag2_names[i]);
                }
            }
            for (i=0; i<ARRAY_SIZE(mission_server_test_names); i++) {
                if (mission_server_test_names[i] && mission_server_test_names[i][0]) {
                    if (!quiet) printf("\t%s\n", mission_server_test_names[i]);
                }
            }
            for (i=0; i<ARRAY_SIZE(account_server_test_names); i++) {
                if (account_server_test_names[i] && account_server_test_names[i][0]) {
                    if (!quiet) printf("\t%s\n", account_server_test_names[i]);
                }
            }
            return;
        } else {
            printf("Error expected \"mode ?|((+|-)FLAG)\"\n");
            return;
        }
        for (i=0; i<ARRAY_SIZE(flag_names); i++) {
            if (flag_names[i] && stricmp(flag_names[i], &buf[6])==0) {
                if (on) {
                    printf("Enabled %s\n", flag_names[i]);
                    g_testMode |= 1<<i;
                } else {
                    printf("Disabled %s\n", flag_names[i]);
                    g_testMode &= ~(1<<i);
                }
            }
        }
        for (i=0; i<ARRAY_SIZE(flag2_names); i++) {
            if (flag2_names[i] && stricmp(flag2_names[i], &buf[6])==0) {
                if (on) {
                    printf("Enabled %s\n", flag2_names[i]);
                    g_testMode2 |= 1<<i;
                } else {
                    printf("Disabled %s\n", flag2_names[i]);
                    g_testMode2 &= ~(1<<i);
                }
            }
        }

        for (i=0; i<ARRAY_SIZE(mission_server_test_names); i++) {
            if (mission_server_test_names[i] && stricmp(mission_server_test_names[i], &buf[6])==0) {
                if (on) {
                    printf("Enabled %s\n", mission_server_test_names[i]);
                    g_testModeMission |= 1<<i;
                } else {
                    printf("Disabled %s\n", mission_server_test_names[i]);
                    g_testModeMission &= ~(1<<i);
                }
            }
        }

        for (i=0; i<ARRAY_SIZE(account_server_test_names); i++) {
            if (account_server_test_names[i] && stricmp(account_server_test_names[i], &buf[6])==0) {
                if (on) {
                    printf("Enabled %s\n", account_server_test_names[i]);
                    g_testModeAccount |= 1<<i;
                } else {
                    printf("Disabled %s\n", account_server_test_names[i]);
                    g_testModeAccount &= ~(1<<i);
                }
            }
        }
    } else if (strnicmp(buf, "gotopos ", strlen("gotopos "))==0) {
        Vec3 dest;
        char *s = buf + strlen("gotopos ");
        if (*s=='r') {
            setDestRandom();
        } else {
            sscanf(s, "%f %f %f", &dest[0], &dest[1], &dest[2]);
            setDest(dest);
        }
    } else if (stricmp(buf, "badnet")==0) {
        setBadNetwork();
    } else if (stricmp(buf, "goodnet")==0) {
        clearBadNetwork();
    } else if (stricmp(buf, "evilchat")==0) {
        setEvilChat(1);
    } else if (stricmp(buf, "goodchat")==0) {
        setEvilChat(0);
    } else if (stricmp(buf, "nobrawl")==0) {
        g_nobrawl=1;
    } else if (stricmp(buf, "brawl")==0) {
        g_nobrawl=0;
    } else if (stricmp(buf, "printstats")==0) {
        broadcastStats();
    } else if (stricmp(buf, "notimeout")==0) {
        control_state.notimeout = 1;
    } else if (stricmp(buf, "askuser")==0) {
        ask_user = 1;
    } else {
        commAddInput(buf);
    }
}

int handleKey() {
    int ch = getch();
    switch (ch) {
    case 27:
        return 1;
    xcase '1':
        commAddInput("setpos 1000 0 0");
    xcase '2':
        commAddInput("setpos 0 0 0");
    xcase 'a':
        doAttacks();
    xcase 'b':
        __debugbreak();
    xcase 'c':
        ticks_until_move=0;
        cyclicStaticMapTransfer();
    xcase 'e':
        checkEmail(1);
    xcase 'j':
        doJump();
    xcase 'l':
        testLevelup_automatedLevelup(0);    /* 0 - sequential power/slot selection, 1 - randomize*/
    xcase 'm':
        memMonitorDisplayStats();
        memCheckDumpAllocs();
    xcase 'n':
        enableNetworkStats(-1);
    xcase 'o':
        testChatNPC();
        printContactInfo();
    xcase 'p':
        testLevelup_displayPowers();
    xcase 'r':
        reconnect=1;
        return 1;
    xcase 's':
        broadcastStats();
    xcase 't':
        if (current_target) {
            printf("Currently targeting: ");
            if ( current_target->name[0] )
                printf("%s\n", current_target->name );
            else
                printf("%s\n", npcDefList.npcDefs[current_target->npcIndex]->displayName );
        } else {
            printf("No current target\n");
        }
    xcase '`':
        {
            static const size = 1024 * 1024;
            static char *buf = NULL;
            if (!buf) {
                buf = malloc(size);
            }
            printf(">");
            gets_s(buf, size);
            processCmd(buf,0);
        }
    xcase '!':
        system("cmd");
    }
    return 0;
}

void calcTimeStep(void);

void fixUpEntities(void)
{
    int i;
    for(i=1;i<entities_max;i++)
    {
        if (entity_state[i] & ENTITY_IN_USE ) {
            Entity *e = entities[i];
            memset( e->netfx_list, 0, sizeof( NetFx ) * e->netfx_count );
            e->netfx_count = 0; 
        }
    }
}

void mainloop() {
    extern int glob_have_camera_pos;
    int framecount=0;
    for(;;) // the official infinite loop
    {
        //heapValidateAll();
        sharedHeapLazySync();
        framecount++;
        Sleep(sleep_time);
        glob_plrstate_valid = 0;
        glob_have_camera_pos = PLAYING_GAME;
        commSendInput();
        commCheck(0);
        entNetUpdate();
        entClientProcess();
        fixUpEntities();

        calcNetworkStats();

        calcTimeStep();

        // Test Client specific:
        if (playerPtr()) {
            stats.xp = playerPtr()->pchar->iExperiencePoints;
            stats.influence = playerPtr()->pchar->iInfluencePoints;
        }
        checkPlayer();
//         checkProximity();
        checkReportNetworkStats();
        checkTarget();
        checkTeamUp();
        checkLeague();
        checkTurnstileJoin();
        checkSuperGroup();
        checkEmail(0);

        g_verbose_client = (g_testMode2 & TEST2_VERBOSE) != 0;

        if (g_testMode & TEST_FIGHT) {
            doAttacks();
        }
        // new 'mapserver blade stress' mode takes precedence over limited mapmove targets
        if (g_testMode2 & TEST2_MAPSTRESS) {
            randomStaticMapStressTransfer();
        }
        else if (g_testMode & TEST_MAPMOVE) {
            randomStaticMapTransfer();
        }

        if ((g_testMode & TEST_MOVE) && framecount>15 && !do_map_xfer && playerPtr()) {
            startMoving();
        }
        if (!(g_testMode & TEST_MOVE) && !do_map_xfer && playerPtr()) {
            stopMoving();
        }
        if (g_testMode & TEST_TIMED_RECONNECT && framecount > 75) {
            break;
        }
        if (g_testMode & TEST_FOLLOW) {
            checkFollow();
        }
        if (g_testMode & TEST_LEVELUP) {
            checkLevelUp();
        }
        if (g_testMode & TEST_CHATNPC) {
            testChatNPC();
        }

        // new 'mission stress' mode takes precedence over older (and broken?) 'missions mode'
        if (g_testMode2 & TEST2_MISSIONSTRESS) {
            checkMissionHop( g_mission_hop_time );
        }
        else if (g_testMode & TEST_MISSIONS) {
                checkMissions();
        }

        doMoverLoop();
        if (g_testMode & TEST_CHAT) {
            chatterLoop();
        }

        if (g_testMode & TEST_ARENA_FIGHTER) {
            arenaFighterLoop();
        }

        if (g_testMode & TEST_AUCTION) {
            aucscript_Thrash(playerPtr(), true, true, sleep_time/10);
        }

        if (g_testModeAccount & TEST_ACCOUNTSERVER) {
            testAccountServer();
        }

        //MISSION SEARCH TESTING STARTS HERE
        if (g_testModeMission & TEST_MISSIONPUBLISH) {
             testMissionSearchTestPublish();
        }

        if(g_testModeMission & TEST_MISSIONSEARCH) {
            testMissionSearch_testSearch();
            testMissionSearch_forceSearch();
        }

        if (g_testModeMission & TEST_MISSIONVOTE) {
            testMissionSearch_testVoteRandom(12);
            testMissionSearch_forceVote();
        }

        if (g_testModeMission & TEST_MISSIONPLAY) {
            testMissionSearch_forcePlay();
            //testMissionSearch_testPlayRandom();
        }
        //END MISSION SEARCH TEST

        if (checkPipe()) break;

        if (kbhit() && handleKey())
            break;

        if (do_map_xfer)
        {
            do_map_xfer = 0;
            if (!doMapXfer()) {
                printf("Error connecting to MapServer on MapMove\n");
                setConsoleTitle("ERROR");
                statusUpdate("ERROR");
                error_exit(0);
                break;
            } else {
                printf("Map: %s\n", glob_map_name);
                sendMessageToLauncher("MapName: %s", glob_map_name);
                checkMissionMapXferCallback();
            }
        }
        {
            static int done=0;
            static int timer=0;
            if (playerPtr() && playerPtr()->pl->door_anim && (playerPtr()->pl->door_anim->state != DOORANIM_WAITDOOROPEN && playerPtr()->pl->door_anim->state != DOORANIM_ACK))
            {
                if (!timer && !done) {
                    timer=20;
                    done=1;
                }
                if (timer) {
                    if (timer==1) {
                        playerPtr()->pl->door_anim->state = DOORANIM_ACK;
                        EMPTY_INPUT_PACKET(CLIENTINP_DOORANIM_DONE);
                        done = 0;
                    }
                }
            }
            if (timer) {
                timer--;
            }
        }

        mpCompactPools();
    }
}

int main(int argc, char **argv)
{
    int firstEmptySlot=0;
    int timer;
    int global_count=0;

    memCheckInit();

    timer = timerAlloc();

    //pktSetDebugInfo();
    //bsAssertOnErrors(true);
    cryptInit();

    setAssertMode( (!IsDebuggerPresent()? ASSERTMODE_THROWEXCEPTION : 0) | ASSERTMODE_DEBUGBUTTONS);
    _CrtSetReportHook(CrtReportingFunction);
    __try {
        if (!"stack corruption") {
            int a[4];
            a[-1] = -1;
            return;
        }
        if (!"heap corruption") {
            char *data = malloc(5);
            data[-1] = -1;
            free(data);
            return;
        }
        if (!"assert") {
            assert(0);
            assertmsg(0, "die!");
        }
        if (!"exception") {
            int a = 0;
            int b = 1/a;
        }
        if (!"debug break") {
            __debugbreak();
        }
        

        //print some info
        {
            TCHAR buffer[MAX_PATH];
            int i;

            printf("(%d) %s\n",_getpid(), getExecutableName());

            printf("cmdline args: ");
            for( i=1; i< argc; ++i)
            {
                printf("%s ", argv[i]);
            }
            printf("\n");

            GetCurrentDirectory(ARRAY_SIZE(buffer), buffer);
            _tprintf(_T("Working directory: %s\n"),buffer);

        }
        
        mpCompactPools();
        memset(&game_state, 0, sizeof(game_state));

        printf("Memory startup...\n");
        {
            bool bUseSharedMemory = process_cmdline_for_sharedmemory(argc, argv);
            if (bUseSharedMemory)
            {
                loadstart_printf("reserving shared heap space...");
                {
                    // @todo this should be a different heap name than the server heap
                    // with a smaller reservation size for test clients
                    U32 uiReservedSize = reserveSharedHeapSpace(getSharedHeapReservationSize());
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
            }
        }


        loadstart_printf("Misc startup...\n");

        // cov and dev file ignore
        //FolderCacheAddIgnorePrefix("v_");
        FolderCacheAddIgnorePrefix("d_");

        authUserSetHeroAccess((U32*)db_info.auth_user_data,1); // for development debugging
        authUserSetVillainAccess((U32*)db_info.auth_user_data,1); // for development debugging
        authUserSetRogueAccess((U32*)db_info.auth_user_data,1);    // for development debugging

        checkArgs(argc, argv);
        FolderCacheChooseMode();
        FatalErrorfSetCallback(fatalErrorCallback);
        ErrorfSetCallback(errorCallback);
        disableRtlHeapChecking(NULL);
        loadend_printf("");

        loadstart_printf("Pigs/Foldercache...");
        logSetDir("TestClient");
        loadend_printf("");

        if (strcmp(g_achAccountName,"")==0)
            genLoginInfo();
        printf("Connecting as %s...\n", g_achAccountName);

        loadstart_printf("PipeClientCreate(\"TestClientLauncher\", \"%s\")...", launcher_host);
        pc = PipeClientCreate("TestClientLauncher", launcher_host);
        if (pc==NULL) {
            printf("Warning: no TestClientLauncher running\n");
        }
        sendMessageToLauncher("PID: %d", _getpid());
        sendMessageToLauncher("Host: %s", getComputerName());

        setConsoleTitle("Loading...");
        statusUpdate("Loading");

        SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);

        srand( timerCpuTicks64() );
        gameStateInit();

        // Test client init
        sendMessageToLauncher("AuthName: %s", g_achAccountName);
        if (useLauncherVersion) {
            sendMessageToLauncher("VersionRequest: NULL");
            while (pc && !launcherVersion && pc->connected == PCS_CONNECTED) {
                launcherVersion = PipeClientGetMessage(pc);
            }
            printf("getting version from launcher: %s\n", launcherVersion);
        }
        else {
            if (strcmp(server, "Training Room"))
                launcherVersion = getExecutablePatchVersion("CoH");
            else
                launcherVersion = getExecutablePatchVersion("CohTest");
            printf("getting version from registry: %s\n", launcherVersion);
        }
        if (launcherVersion) {
            setExecutablePatchVersionOverride(launcherVersion);
        }

        // From game.main.c
        cmdOldCommonInit();
        loadend_printf("");
//         if (launcherVersion) {
//             printf("Using version specified by TCLauncher: %s\n", launcherVersion);
//         }

        chareval_Init();
        combateval_Init();

        loadstart_printf("Networking startup...");
        packetStartup(0,1); // <--- All of the time is in here
        commNewInputPak();
        sockStart();
        commInit();
        //pktSetDebugInfo(); // ab: need to fix so this works!
        loadend_printf("");

        if (g_testMode != TEST_LOGIN) {
            loadstart_printf("load_AllDefs()");
            seqLoadStateBits();
            load_AllDefs();
            loadend_printf("");
            //init_menus();
            //loadFonts();
            loadstart_printf("bpReadBodyPartFiles()");
            bpReadBodyPartFiles(); // Need this, at least, for character creation
            loadend_printf("");
            //loadstart_printf("npcReadDefFiles()");
            //npcReadDefFiles();
            //loadend_printf("");

            loadstart_printf("loadCostumes()");
            loadCostumes();
            loadPowerCustomizations();
            loadend_printf("");
        }

        setConsoleTitle("Connecting...");
        statusUpdate("Connecting");

        global_count = 0;
        do {
            global_count++;
            err = 0;
            // Log on
            {
                int count=0;
                bool logged_in=false;
                for (;!logged_in && count!=login_retry_count; count++) {
                    if(loginWrapper()) {
                        logged_in = true;
                    } else if (count != login_retry_count-1) {
                        printf("Error logging on\nAuthServer Error:\n%s\nDBServer Error:\n%s\nTrying again...\n", authGetError(), dbGetError());
                    }
                    if (kbhit() && getch()==27) break;
                }
                if (!logged_in) {
                    printf("Error logging on\nAuthServer Error:\n%s\nDBServer Error:\n%s\n", authGetError(), dbGetError());
                    setConsoleTitle("ERROR");
                    statusUpdate("ERROR");
                    err=1;
                }
            }
            gPlayerNumber = 0;
            if (!err) {
                for (firstEmptySlot=0; firstEmptySlot<db_info.player_count; firstEmptySlot++) {
                    if (stricmp(db_info.players[firstEmptySlot].name, "empty")==0)
                        break;
                }
                if (strcmp(character_name,"") != 0)
                {
                    printf("Searching for %s in character list...\n",character_name);
                    for (gPlayerNumber=0; gPlayerNumber<db_info.player_count; gPlayerNumber++) {
                        printf(" %d - %s\n",gPlayerNumber,db_info.players[gPlayerNumber].name);
                        if (stricmp(db_info.players[gPlayerNumber].name, character_name)==0)
                            break;
                    }
                    if (gPlayerNumber == db_info.player_count) {
                        gPlayerNumber = -1;
                        printf("Unable to locate character %s\n",character_name);
                    }
                    else {
                        printf("Found character %s in slot %d\n",character_name, gPlayerNumber);
                    }
                }
                if (db_info.player_count && firstEmptySlot==db_info.player_count)
                    firstEmptySlot = -1;
            }
            if (!(g_testMode & TEST_CREATE_CHAR) && !(g_testMode & TEST_RESUME_CHAR)) {
                // Neither resuming or creating, just logging into the CS
                if (!(g_testMode & TEST_STAY_CONNECTED) && !err) {
                    statusUpdate("Running");
                    return 0;
                }
            }

            if (ask_user && !err) {
                bool chooseExisting = false;
                if (db_info.player_count>0)    // account has some pre-existing characters, let user choose
                {
                    printf("Do you wish to choose an existing character? [yN]");
                    chooseExisting = consoleYesNo();
                    if (chooseExisting) {
                        int i;
                        for (i=0; i<db_info.player_count; i++) {
                            printf("\t%d : %s (dbid %d)\n", i+1, db_info.players[i].name, db_info.players[i].db_id);
                        }
                        gPlayerNumber=-1;
                        while (gPlayerNumber<0 || gPlayerNumber>=db_info.player_count) {
                            int k;
                            printf("Choose a slot: [1-%d] ", db_info.player_count);
                            k = getch();
                            gPlayerNumber = k - '1';
                            printf("\n");
                        }
                    }
                }
                else
                {
                    printf("No existing characters to choose from.\n");
                }

                if (!chooseExisting)
                {
                    // either nothing to choose or they don't want to, so we have to create one
                    printf("Do you wish to randomly create a character? [yN]");
                    if (consoleYesNo()) {
                        ask_user = 0;
                    }
                }
            }

            if (!err && (firstEmptySlot<=gPlayerNumber) && (firstEmptySlot!=-1) && !(g_testMode & TEST_CREATE_CHAR)) {
                printf("%s does not have a character in slot %d to log into.\n", g_achAccountName, gPlayerNumber);
                setConsoleTitle("ERROR");
                statusUpdate("ERROR");
                err = 1;
            }

            if (!err && (firstEmptySlot>gPlayerNumber || firstEmptySlot==-1) && (g_testMode & TEST_RESUME_CHAR) && (gPlayerNumber>=0)) {
                // Resume a character
                loadstart_printf("Resuming character in slot %d...",gPlayerNumber);
                if(!choosePlayerWrapper( gPlayerNumber, 0 )) {
                    printf("Error calling choosePlayerWrapper/resuming character:\n%s\n", dbGetError());
                    setConsoleTitle("ERROR");
                    statusUpdate("ERROR");
                    dbDisconnect();
                    err = 1;
                }
                if (!err) {
                    strcpy(character_name, db_info.players[gPlayerNumber].name);
                    dbDisconnect();           //Character being resumed, no longer need dbServer.
                    player_being_created = 0;
                    loadend_printf("");
                    tryConnect();
                    if( !commConnected() ) {
                        printf("Error connecting to MapServer\n");
                        setConsoleTitle("ERROR");
                        statusUpdate("ERROR");
                        err = 1;
                    } else {
                        // this is where we receive costume back
                        loadstart_printf("commReqScene()");
                        commReqScene(1);
                        loadend_printf("");
                    }
                }
            } else if (!err && (g_testMode & TEST_CREATE_CHAR)) {
                if (firstEmptySlot==-1) {
                    printf("No free slots for %s\n", g_achAccountName);
                    setConsoleTitle("ERROR");
                    statusUpdate("ERROR");
                    err=1;
                } else {
                    player_slot = gPlayerNumber = firstEmptySlot;
                    // Create a new character in firstEmptySlot
                    printf("simulateCharacterCreate()\n");
                    loadstart_printf("");
                    if(db_info.players)
                    {
                        simulateCharacterCreate(ask_user, 0);
                        if (!playerPtr() || player_being_created || !commConnected()) {
                            printf("Character creation did not reach the MapServer.\n");
                            setConsoleTitle("ERROR");
                            statusUpdate("ERROR");
                            err = 1;
                            loadend_printf("failed");
                        } else {
                            strcpy(character_name, playerPtr()->name);
                            loadend_printf("done");
                        }
                    }
                    else
                    {
                        printf("Did not get players from dbserver.  Error: %s\n", db_info.error_msg);
                        setConsoleTitle("ERROR");
                        statusUpdate("ERROR");
                        err=1;
                    }
                }
            }

        } while(global_count != (login_retry_count+1) && err);

        if (glob_map_name[0]) {
            printf("Map: %s\n", glob_map_name);
            sendMessageToLauncher("MapName: %s", glob_map_name);
        }

        if (!err) {
            checkPlayer();
            statusUpdate("Running");
        }
        loadstart_printf("");

        printf("Total time to launch TestClient : %1.3g\n", timerElapsed(timer));
        timerFree(timer);

        testClientRandomDisconnect(TCS_allDone);
        
        // Switch to normal-er priority
        SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);

        printf("Press Esc to exit...\n");
        if (err || !(g_testMode & TEST_STAY_CONNECTED)) {
            if (err)
                error_exit(0);
            return 0;
        }
        game_state.local_map_server = 0; // So that it gets booted back to login screen when it gets disconnected

        enableNetworkStats(1);
        calcNetworkStats();
        enableNetworkStats(0);

        if (!err)
            mainloop();

        if ((reconnect || (g_testMode & TEST_TIMED_RECONNECT)) && !err) {
            commDisconnect();
            setConsoleTitle("Reconnecting...");
            statusUpdate("Reconnect");
            Sleep(5000);
            error_exit(0); // temp hack so testclients stick around so we can see the log better...
        }

    }
    __except (tcAssertExcept(GetExceptionCode(), GetExceptionInformation()))
    {
        return 0;
    }
    return 0;
}
