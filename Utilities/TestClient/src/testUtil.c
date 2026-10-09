#include "testUtil.h"
#include "testTeam.h"
#include "testClientInclude.h"
#include "chatter.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/wininclude.h>
#include <conio.h>
#include "cmdparse/cmdcommon.h"
#include "player/player.h"
#include <utilitieslib/utils/error.h>
#include "PipeClient.h"
#include "entity/entity.h"
#include <utilitieslib/assert/assert.h>
#include "ArenaFighter.h"
#include "entity/entVarUpdate.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StringCache.h>
#include "clientcomm/clientcomm.h"

extern int gbParseChatToCommand;

void setBadNetwork() {
    control_state.snc_client.lag = 1200;
    control_state.snc_client.pl = 10;
    control_state.snc_client.oo_packets = 1;
}


void clearBadNetwork() {
    control_state.snc_client.lag = 0;
    control_state.snc_client.pl = 0;
    control_state.snc_client.oo_packets = 0;
}

int testClientSafeError(const char *s) {
    static char *safe[] = {
        "CUSTOM BODY ERROR: bad body name",
            "CUSTOM TEXTURE ERROR:",
            "CUSTOM COLOR ERROR:",
            "CUSTOM SCALE ERROR:",
            "changeGeo called on Garbage Entity",
            "NETFX ERROR",
    };
    int i;
    for (i=0; i<ARRAY_SIZE(safe); i++) 
        if (strnicmp(s, safe[i], strlen(safe[i]))==0)
            return 1;
    return 0;
}




void addFloatingInfo(int svr_idx, char *pch, U32 colorFG, U32 colorBG, U32 colorBorder, float fScale, float fTime, float fDelay, EFloaterStyle style) {
    if (stricmp(pch, "FloatOutOfRange")==0 || stricmp(pch, "FloatRecharging")==0 || stricmp(pch, "FloatNoTarget")==0 || stricmp(pch, "FloatMiss")==0 || stricmp(pch, "FloatNotEnoughEndurance")==0 || stricmp(pch, "FloatTargetUnreachable")==0 ||
        strnicmp(pch, "I just saw", 10)==0 || stricmp(pch, "FloatStunned")==0 || stricmp(pch, "FloatNoEndurance")==0)
    {

    } else {
        printf("Floating info : %s\n", pch);
    }
}

void addFloatingDamage(    Entity    *victim, Entity    *damager, int dmg, char *pch, bool wasAbsorb) {
    //printf("Floating damage : %d\n", dmg);
}

const char *msgType(INFO_BOX_MSGS msg)
{
    switch (msg) {
        xcase INFO_COMBAT: return "[combat] ";
        xcase INFO_DAMAGE: return "[damaged] ";
        xcase INFO_SVR_COM: return "[svr] ";
        xcase INFO_NPC_SAYS: return "[npc_says] ";
        xcase INFO_VILLAIN_SAYS: return "[villain_says] ";
        xcase INFO_PET_COM: return "[pet] ";
        xcase INFO_PRIVATE_COM: return "[private] ";
        xcase INFO_PRIVATE_NOREPLY_COM: return "[private] ";
        xcase INFO_TEAM_COM: return "[team] ";
        xcase INFO_SUPERGROUP_COM: return "[supergroup] ";
        xcase INFO_NEARBY_COM: return "[local] ";
        xcase INFO_SHOUT_COM: return "[broadcast] ";
        xcase INFO_REQUEST_COM: return "[request] ";
        xcase INFO_LOOKING_FOR_GROUP: return "[looking_for_group] ";
        xcase INFO_FRIEND_COM: return "[friend] ";
        xcase INFO_ADMIN_COM: return "[admin] ";
        xcase INFO_USER_ERROR: return "[user_error] ";
        xcase INFO_DEBUG_INFO: return "[debug_info] ";
        xcase INFO_EMOTE: return "[emote] ";
        xcase INFO_AUCTION: return "[auction] ";
        xcase INFO_ARCHITECT: return "[architect] ";
        xcase INFO_COMBAT_SPAM: return "[combat_spam] ";
        xcase INFO_GMTELL: return "[gmtell] ";
        xcase INFO_TAB: return "[tab] ";
        xcase INFO_REWARD: return "[reward] ";
        xcase INFO_COMBAT_ERROR: return "[combat_error] ";
        xcase INFO_HEAL: return "[heal] ";
        xcase INFO_HEAL_OTHER: return "[heal_other] ";
        xcase INFO_CHANNEL: return "[channel] ";
        xcase INFO_ARENA: return "[arena] ";
        xcase INFO_ARENA_ERROR: return "[arena_error] ";
        xcase INFO_ARENA_GLOBAL: return "[arena_global] ";
        xcase INFO_ALLIANCE_OWN_COM: return "[alliance_own] ";
        xcase INFO_ALLIANCE_ALLY_COM: return "[alliance_ally] ";
        xcase INFO_HELP: return "[help] ";
        xcase INFO_CAPTION: return "[caption] ";
        xcase INFO_LEAGUE_COM: return "[league] ";
    }
    return "";
}

U32 lastProxCheck = 0;
StashTable playerNames = NULL;

static int printNames(void* buffer, StashElement element)
{
    estrConcatf((char**)buffer, "(%s,%d)", stashElementGetStringKey(element), stashElementGetInt(element));
    return 1;
}

void checkProximity()
{
    if(ABS_TIME_SINCE(lastProxCheck) > SEC_TO_ABS(10))
    {
        int i;
        static char* buf = NULL;

        lastProxCheck = ABS_TIME;
        estrClear(&buf);

        if(!playerNames)
        {
            playerNames = stashTableCreateWithStringKeys(1024, StashDefault);
        }
        // do a broadcast of all entities you know about
        for(i = 1; i < entities_max; i++)
        {
            if (!(entity_state[i] & ENTITY_IN_USE ) )
                continue;
            if ( ENTTYPE_BY_ID(i) != ENTTYPE_PLAYER )
                continue;

            stashAddInt(playerNames, allocAddString(entities[i]->name), entities[i]->db_id, false);
        }

        estrConcatStaticCharArray(&buf, "b !!names");
        stashForEachElementEx(playerNames, printNames, (void*)&buf);
        commAddInput(buf);
    }
}

void processPlayerList(char* txt)
{
    char* s = strchr(txt, '(');
    char* s2;
    int i;

    while(s && (s2 = strchr(s, ',')))
    {
        *s2 = '\0';
        i = atoi(s2+1);

        stashAddInt(playerNames, allocAddString(s+1), i, false);

        s = strchr(s2+1, '(');
    }
}

void addToChatMsgs(    char *txt, int msg, void * filter) {
    char *s;
    if (strstri(txt, "(randomchat)")!=0)
        return;
    if (strstri(txt, "You are not on a team!")!=0)
        return;
    if (strstri(txt, "You have defeated")) {
        stats.defeatcount++;
    }
#ifndef TEXT_CLIENT
    {
        extern bool g_verbose_client;
        if (!g_verbose_client) {
            if (msg==INFO_NPC_SAYS)
                return;
            // @todo why just this  message?
//            if (strstri(txt, "You have defeated"))
//                return;
        }
    }
#endif
    if( strstri(txt, "!!names")) {
        static bool hideProximityCheckText = true;
        if(!hideProximityCheckText)
            printf("Chat message : %s%s\n", msgType(msg), txt);
        processPlayerList(txt);
    }
    else {
        printf("Chat message : %s%s\n", msgType(msg), txt);
        sendChatToLauncher("Chat:%d:%s",msg,txt);
        if (gbParseChatToCommand && (s=strstri(txt, "@"))) {
            s+=strlen("@");
            processCmd(s,0);
        }
    }
}

void addToChatMsgsEx( char *txt, int msg, int number, int svr_idx) {
    addToChatMsgs(txt, msg, NULL);
}

void dialogStd( int type, char * txt, void(*code1)(), void(*code2)() ) {
    checkTeamUpDialog(txt);
    checkSuperGroupDialog(txt);
    checkLeagueDialog(txt);
    arenaFighterDialog(txt);
    printf("dialog: %s\n", txt);
    if (strstri(txt, "CantFindLocalMapServer")!=0) {
        setConsoleTitle("ERROR");
        statusUpdate("ERROR");
        err=1;
    }
}

void dialogStdWidth( int type, char * txt, char * response1, char * response2, void(*code1)(), void(*code2)(), int flags, float width )
{
    dialogStd(type, txt, code1, code2);
}

void dialogTimed( int type, char * txt, char * response1, char * response2, void(*code1)(), void(*code2)(), int game_only, int time, void *data )
{
    printf("dialogTimed: %s\n", txt);
    if (strstri(txt, "accept") && gbParseChatToCommand) 
    {
        printf("Auto-accepting dialog\n");
        code1();
    }
}


void dialog( int type, float x, float y, float wd, float ht, char * txt, void (*code1)(), void (*code2)(), 
            int text_type, int color, int back_color )
{
    dialogStd(type, txt, code1, code2);
}


static char *filenames[] = {
"TCLOG_FATAL.log",
"TCLOG_SUSPICIOUS.log",
"TCLOG_DEBUG.log",
"TCLOG_STATS.log",
"error",
};

void testClientLog(TestClientLogLevel level, const char *fmt, ...) {
    char str[20000] = {0};
    char *name="No playerPtr";
    va_list ap;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    if (playerPtr()) {
        name = playerPtr()->name;
    }
    if (!strEndsWith(str, "\n"))
        strcat(str, "\n");

    log_printf(filenames[level], "%s\t%s", name, str);
    consoleSetFGColor(COLOR_RED | COLOR_BRIGHT);
    printf("%s", str);
    consoleSetFGColor(COLOR_RED | COLOR_GREEN | COLOR_BLUE);

}

static TestMode mode_stack[16];
static mode_stack_height=0;
void modePush(void)
{
    mode_stack[mode_stack_height++] = g_testMode;
    assert(mode_stack_height < ARRAY_SIZE(mode_stack));
}

void modePop(void)
{
    mode_stack_height--;
    assert(mode_stack_height>=0);
    g_testMode = mode_stack[mode_stack_height];
}
