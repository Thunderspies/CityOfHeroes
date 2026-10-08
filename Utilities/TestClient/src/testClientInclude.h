typedef enum {
    TEST_LOGIN            = 0, // assumed
    TEST_RESUME_CHAR    = 1<<0,
    TEST_CREATE_CHAR    = 1<<1,
    TEST_STAY_CONNECTED    = 1<<2,
    TEST_FIGHT            = 1<<3,
    TEST_MAPMOVE        = 1<<4,
    TEST_MOVE            = 1<<5,
    TEST_CHAT            = 1<<6,
    TEST_TEAMACCEPT        = 1<<7,
    TEST_TEAMINVITE        = 1<<8,
    TEST_TEAMTHRASH        = 1<<9,
    TEST_RANDOM_DISCONNECT = 1<<10,
    TEST_TIMED_RECONNECT = 1<<11,
    TEST_FOLLOW            = 1<<12,
    TEST_LEVELUP        = 1<<13,
    TEST_SUPERGROUPACCEPT = 1<<14,
    TEST_SUPERGROUPINVITE = 1<<15,
    TEST_SUPERGROUPMISC = 1<<16,
    TEST_AUTOREZ        = 1<<17,
    TEST_CHATNPC        = 1<<18,
    TEST_NOIGNORE        = 1<<19,
    TEST_MISSIONS        = 1<<20,
    // Bit 21 is reserved for the retired legacy email mode.
    TEST_ARENA_FIGHTER    = 1<<22,
    TEST_ARENA_KILLBOT    = 1<<23,
    TEST_ARENA_CREATOR    = 1<<24,
    TEST_ARENA_THRASHER    = 1<<25,
    TEST_ARENA_SUPERGROUP= 1<<26,
    TEST_ARENA_SCHEDULED = 1<<27,
    TEST_SUPERGROUPLEAVE = 1<<28,
    TEST_AUCTION        = 1<<29,
    
} TestMode;

typedef enum
{
    TEST2_LEAGUE_ACCEPT            = 1<<0,
    TEST2_LEAGUE_INVITE            = 1<<1,
    TEST2_LEAGUE_THRASH            = 1<<2,
    
    TEST2_TURNSTILE_JOIN        = 1<<3,
    TEST2_MISSIONSTRESS            = 1<<4,    // hop from mission map instance to mission map instance for server stress
    TEST2_MAPSTRESS                = 1<<5,    // hop from static maps server to static mapserver, but through entire map list, static map instances included

    TEST2_VERBOSE                = 1<<6, // send info box,npc chat, etc. messages to the console and to the launcher (e.g. mostly battle messages)


}TestMode2;

typedef enum {
    TEST_MISSIONSEARCH            = 1<<0, // assumed
    TEST_MISSIONVOTE    = 1<<1,
    TEST_MISSIONPLAY    = 1<<2,
    TEST_MISSIONPUBLISH    = 1<<3,
} MissionServerTestMode;

typedef enum {
    TEST_ACCOUNTSERVER    = 1<<0,
    TEST_ACCOUNTSERVER_SUPERPACKS = 1<<1,
    TEST_ACCOUNTSERVER_CERTIFICATION_GRANTS = 1<<2,
    TEST_ACCOUNTSERVER_CERTIFICATION_CLAIMS = 1<<3,
} AccountServerTestMode;

extern TestMode g_testMode;
extern TestMode2 g_testMode2;
extern MissionServerTestMode g_testModeMission;
extern AccountServerTestMode g_testModeAccount;

extern int err;
extern int ask_user;
extern char glob_map_name[];

void statusUpdate(const char *fmt, ...);
void sendMessageToLauncher(const char *fmt, ...);
void processCmd(char *buf, int quiet);
int error_exit(int allow_debug);
void testClientRecordEmailStatus(int status);

// This test client's pipe
typedef struct PipeClient_t *PipeClient;
extern PipeClient pc;
