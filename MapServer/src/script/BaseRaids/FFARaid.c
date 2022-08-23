// ZONE SCRIPT
//
// Runs an FFA SG base raid.

#include "script/scriptutil.h"
#include <utilitieslib/utils/timing.h>
#include "bases/bases.h"
#include "bases/basesystems.h"
#include "bases/basedata.h"
#include <utilitieslib/components/earray.h>
#include "gameComm/VillainDef.h"
#include "gameSys/sgraid.h"
#include <utilitieslib/utils/error.h>
#include "ai/entai.h"
#include "entity/entity.h"
#include "entity/entserver.h"
#include "ai/entaiScript.h"
#include "script/scriptengine.h"
#include "gameComm/dooranimcommon.h"
#include "svr/svr_player.h"
#include "gameSys/raidmapserver.h"
#include "gameSys/sgraid_V2.h"

#define WRAPUP_LENGTH    1.0f    // in minutes
#define ATTACKER_CLOCK    2.0f    // in minutes
#define GAME_CLOCK        15.0f    // in minutes
#define KILLS_TO_WIN    10        // number of kills needed for victory

static void FFARaidStop(void)
{
}

// Called when the player enters the map - set their gang ID to their raid ID
static int FFARaidOnEnterMap(ENTITY player)
{
    int raidID = GetPlayerRaidID(player);

    if (raidID == VarGetNumber("BlueRaidID"))
    {
        SetScriptTeam(player, "BlueTeam");    
    }
    else if (raidID == VarGetNumber("RedRaidID"))
    {
        SetScriptTeam(player, "RedTeam");    
    }
    else
    {
        devassertmsg(0, "Intruder detected on map");
    }

    SetAIGang(player, raidID);
    return 0;
}

// Called when the player leaves the map - removes them from their team
static int FFARaidOnLeaveMap(ENTITY player)
{
    SetAIGang(player, 0);
    return 0;
}

static void FFARaidOnEntityDefeat(ENTITY player, ENTITY victor)
{
    int blueKills;
    int redKills;

    if (IsEntityPlayer(victor) && IsEntityPlayer(player))
    {
        if (IsEntityOnScriptTeam(victor, "BlueTeam") && IsEntityOnScriptTeam(player, "RedTeam"))
        {
            blueKills = VarGetNumber("BlueKills") + 1;
            VarSetNumber("BlueKills", blueKills);
            VarSet("BlueTeamUIKillCount", NumberToString(blueKills));
            if (blueKills >= KILLS_TO_WIN)
            {
                ScriptSendSignal(ScriptRunIdFromName("FFARaid", SCRIPT_ZONE), "BlueTeamWin");
            }
        }
        else if (IsEntityOnScriptTeam(victor, "RedTeam") && IsEntityOnScriptTeam(player, "BlueTeam"))
        {
            redKills = VarGetNumber("RedKills") + 1;
            VarSetNumber("RedKills", redKills);
            VarSet("RedTeamUIKillCount", NumberToString(redKills));
            if (redKills >= KILLS_TO_WIN)
            {
                ScriptSendSignal(ScriptRunIdFromName("FFARaid", SCRIPT_ZONE), "RedTeamWin");
            }
        }
    }
}

void FFARaid(void)
{
    INITIAL_STATE
        ON_ENTRY 
            //TimerSet("BlueClock", ATTACKER_CLOCK);
            //TimerSet("RedClock", ATTACKER_CLOCK);
            TimerSet("Clock", GAME_CLOCK);
            VarSetNumber("BlueSGID", sgRaidGetBaseRaidSGID(0));
            VarSetNumber("RedSGID", sgRaidGetBaseRaidSGID(1));
            VarSet("BlueSGName", sgRaidGetBaseRaidSGName(0));
            VarSet("RedSGName", sgRaidGetBaseRaidSGName(1));
            VarSetNumber("BlueRaidID", sgRaidGetBaseRaidRaidID(0));
            VarSetNumber("RedRaidID", sgRaidGetBaseRaidRaidID(1));
            VarSetNumber("BlueKills", 0);
            VarSetNumber("RedKills", 0);
            //VarSetNumber("KillsToWin", 10);
            //VarSetNumber("GameDuration", 10);

            // Set up the UI
            VarSet("BlueTeamUIKillTitle", StringAdd(VarGet("BlueSGName"), StringAdd(" ", StringLocalize(VarGet("KillText")))));
            VarSet("RedTeamUIKillTitle", StringAdd(VarGet("RedSGName"), StringAdd(" ", StringLocalize(VarGet("KillText")))));
            VarSet("BlueTeamUIKillCount", "0");
            VarSet("RedTeamUIKillCount", "0");

            ScriptUIList("UIBlueTeamKills", "", "BlueTeamUIKillTitle", "ffffffff", "BlueTeamUIKillCount", "ffffffff");
            ScriptUIList("UIRedTeamKills", "", "RedTeamUIKillTitle", "ffffffff", "RedTeamUIKillCount", "ffffffff");

            ScriptUICreateCollection("BlueTeamUI", 2, "UIBlueTeamKills", "UIRedTeamKills");
            ScriptUICreateCollection("RedTeamUI", 2, "UIRedTeamKills", "UIBlueTeamKills");

            OnPlayerEnteringMap(FFARaidOnEnterMap);
            OnPlayerExitingMap(FFARaidOnLeaveMap);
            OnEntityDefeated(FFARaidOnEntityDefeat);
            SET_STATE("WaitForPlayers");
        END_ENTRY

    STATE("WaitForPlayers")
        BroadcastAttentionMessage(VarGet("Why"), NULL, 0.0);
        SET_STATE("WrapupTime");

    STATE("BuffDelay")
        BroadcastAttentionMessage(VarGet("Why"), NULL, 0.0);
        SET_STATE("WrapupTime");


        {
        int blueKills = VarGetNumber("BlueKills");
        int redKills = VarGetNumber("RedKills");
        if (TimerElapsed("Clock"))
        {
            VarSet("Why", "Time ran out");
            if (blueKills > redKills)
            {
                char reason[256];
                strcpy(reason, VarGet("BlueSGName"));
                strcat(reason, " won" /* StringLocalizeStatic("spacewon") */);
                VarSet("Why", reason);
                SET_STATE("BlueTeamWin");
            }
            else if (blueKills > redKills)
            {
                char reason[256];
                strcpy(reason, VarGet("RedSGName"));
                strcat(reason, " won" /* StringLocalizeStatic("spacewon") */);
                VarSet("Why", reason);
                SET_STATE("RedTeamWin");
            }
            else
            {
                VarSet("Why", "Drawn match");
                SET_STATE("Draw");
            }
        }

        if (NumEntitiesInTeam("BlueTeam"))
        {
            //TimerSet("BlueClock", ATTACKER_CLOCK);
        }
        else if (0 /* TimerElapsed("BlueClock") */ )
        {
            char reason[256];
            strcpy(reason, VarGet("BlueSGName"));
            strcat(reason, " left" /* StringLocalizeStatic("spacedeparted") */);
            VarSet("Why", reason);
            SET_STATE("RedTeamWin");
        }
        if (NumEntitiesInTeam("RedTeam"))
        {
            //TimerSet("RedClock", ATTACKER_CLOCK);
        }
        else if (0 /* TimerElapsed("RedClock") */)
        {
            char reason[256];
            strcpy(reason, VarGet("RedSGName"));
            strcat(reason, " left" /* StringLocalizeStatic("spacedeparted") */);
            VarSet("Why", reason);
            SET_STATE("BlueTeamWin");
        }
        }

    STATE("BlueTeamWin")
        BroadcastAttentionMessage(VarGet("Why"), NULL, 0.0);
        SET_STATE("WrapupTime");


    STATE("RedTeamWin")
        BroadcastAttentionMessage(VarGet("Why"), NULL, 0.0);
        SET_STATE("WrapupTime");
        

    STATE("Draw")
        BroadcastAttentionMessage(VarGet("Why"), NULL, 0.0);
        SET_STATE("WrapupTime");

    STATE("WrapupTime")
        ON_ENTRY
            TimerSet("Clock", WRAPUP_LENGTH);
        END_ENTRY

        if (TimerElapsed("Clock"))
        {
            FFARaidStop();
            EndScript();
        }
    END_STATES

    ON_SIGNAL("BlueTeamWin")
        SET_STATE("BlueTeamWin")
    END_SIGNAL

    ON_SIGNAL("RedTeamWin")
        SET_STATE("RedTeamWin")
    END_SIGNAL

    ON_STOPSIGNAL
        FFARaidStop();
        EndScript();
    END_SIGNAL
}

void FFARaidInit()
{
    SetScriptName("FFARaid");
    SetScriptFunc(FFARaid);
    SetScriptType(SCRIPT_ZONE);

    SetScriptVar("BlueSGID",            NULL,        SP_OPTIONAL);
    SetScriptVar("RedSGID",                NULL,        SP_OPTIONAL);
    SetScriptVar("BlueSGName",            NULL,        SP_OPTIONAL);
    SetScriptVar("RedSGName",            NULL,        SP_OPTIONAL);
    SetScriptVar("BlueRaidID",            NULL,        SP_OPTIONAL);
    SetScriptVar("RedRaidID",            NULL,        SP_OPTIONAL);
    SetScriptVar("BlueKills",            NULL,        SP_OPTIONAL);
    SetScriptVar("RedKills",            NULL,        SP_OPTIONAL);
    SetScriptVar("KillsText",            NULL,        0);
    SetScriptVar("KillsToWin",            NULL,        0);
    SetScriptVar("GameDuration",        NULL,        0);
    SetScriptVar("WonText",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("DrawText",            NULL,        SP_DONTDISPLAYDEBUG);
}
