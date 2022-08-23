// ZONE SCRIPT
//
// Runs the main IOP SG base raid

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
#include "bases/baseRaid.h"
#include "entity/character_target.h"
#include "gameSys/sgraid_V2.h"

#define INITIAL_WAIT_DELAY    1.0        /* time in minutes for everyone to connect*/
#define WRAPUP_DELAY        1.0        /* time in minutes at end */

#define    WonTextWonColor            0x00ff00ff
#define    WonTextLostColor        0xff0000ff
#define    DrawTextColor            0xff8000ff

static void MayhemRaidOnEntityDefeat(ENTITY player, ENTITY victor)
{
    int blueDamage;
    int redDamage;
    
    if (IsEntityOnScriptTeam(victor, "BlueTeam"))
    {
        // Need to drill into player's Entity to get max hp for damage meter, and whether it's in "interesting" item
        // Note if it's control or energy, we'll need to check that for "early win"
        blueDamage = VarGetNumber("BlueDamage") + 1;
        VarSetNumber("BlueDamage", blueDamage);
        VarSet("BlueTeamUIDamageCount", NumberToString(blueDamage));
    }
    else if    (IsEntityOnScriptTeam(victor, "RedTeam"))
    {
        redDamage = VarGetNumber("RedDamage") + 1;
        VarSetNumber("RedDamage", redDamage);
        VarSet("RedTeamUIDamageCount", NumberToString(redDamage));
    }
}

// Called when the player enters the map - set their gang ID to their raid ID
int MayhemRaidOnEnterMap(ENTITY player)
{
    int raidID = GetPlayerRaidID(player);

    if (raidID == VarGetNumber("BlueRaidID"))
    {
        SetAIGang(player, sgRaidGetGangIDByIndex(0));
        SetScriptTeam(player, "BlueTeam");    
        ScriptUIShow("Collection:BlueTeamUI", player);
    }
    else if (raidID == VarGetNumber("RedRaidID"))
    {
        SetAIGang(player, sgRaidGetGangIDByIndex(1));
        SetScriptTeam(player, "RedTeam");    
        ScriptUIShow("Collection:RedTeamUI", player);
    }
    if (StringEqual(GET_STATE, "WaitForPlayers"))
    {
        // Freeze the player - totally inactive
    }
    return 0;
}

// Called when the player leaves the map.  Remove the "I'm carrying an IOP" buff from the player.
// Drop the IOP at their location if they were carrying, reset their gang ID to zero, and remove them from
// whatever team they're in
int MayhemRaidOnLeaveMap(ENTITY player)
{
    SetAIGang(player, 0);
    // "defeat" the player so the flag drops
    return 0;
}


void MayhemRaid()
{
    int blueDamage;
    int redDamage;
    int blueRaidID;
    int redRaidID;

    INITIAL_STATE
        ON_ENTRY 
            VarSetNumber("BlueSGID", sgRaidGetBaseRaidSGID(0));
            VarSetNumber("RedSGID", sgRaidGetBaseRaidSGID(1));
            VarSet("BlueSGName", sgRaidGetBaseRaidSGName(0));
            VarSet("RedSGName", sgRaidGetBaseRaidSGName(1));
            blueRaidID = sgRaidGetBaseRaidRaidID(0);
            redRaidID = sgRaidGetBaseRaidRaidID(1);
            VarSetNumber("BlueRaidID", blueRaidID);
            VarSetNumber("RedRaidID", redRaidID);
            VarSetNumber("BlueDestruction", 0);
            VarSetNumber("RedDestruction", 0);

            // Set up the UI
            VarSet("BlueTeamUIDamageTitle", StringAdd(/*VarGet("BlueSGName")*/ "Blue" , StringAdd(" ", StringLocalize(VarGet("DamageText")))));
            VarSet("RedTeamUIDamageTitle", StringAdd(/*VarGet("RedSGName")*/ "Red" , StringAdd(" ", StringLocalize(VarGet("DamageText")))));
            VarSet("BlueTeamUIDamageCount", "0");
            VarSet("RedTeamUIDamageCount", "0");

            ScriptUIList("UIBlueTeamDamage", "", "BlueTeamUIDamageTitle", "ffffffff", "BlueTeamUIDamageCount", "ffffffff");
            ScriptUIList("UIRedTeamDamage", "", "RedTeamUIDamageTitle", "ffffffff", "RedTeamUIDamageCount", "ffffffff");

            ScriptUITimer("UIGameTimer", "TimeLeft", "TimeRemaining", "");

            ScriptUICreateCollection("BlueTeamUI", 4, "UIBlueTeamCollection", "UIGameTimer", "UIBlueTeamDamage", "UIRedTeamDamage");
            ScriptUICreateCollection("RedTeamUI", 4, "UIRedTeamCollection", "UIGameTimer", "UIRedTeamDamage", "UIBlueTeamDamage");

            OnPlayerEnteringMap(MayhemRaidOnEnterMap);
            OnPlayerExitingMap(MayhemRaidOnLeaveMap);
            OnEntityDefeated(MayhemRaidOnEntityDefeat);
            //OnPlayerEntersVolume();

        END_ENTRY

        // Need to change state immediately, so I can test if we're in this state
        SET_STATE("WaitForPlayers");

    STATE("WaitForPlayers")
        ON_ENTRY 
            TimerSet("Clock", INITIAL_WAIT_DELAY); // change this for maximum time we'll wait for everyone
            VarSetNumber("TimeLeft", (int) (INITIAL_WAIT_DELAY * 60.0f) + timerSecondsSince2000());
        END_ENTRY
        if (1 || TimerElapsed("Clock") /* || AllPlayersOnMap() */ )
        {
            // Unfreeze All Players
            SET_STATE("BuffDelay");
        }

    STATE("BuffDelay")
        ON_ENTRY 
            TimerSet("Clock", 0.25f); // This needs to be a parmeter - buff delay
            VarSetNumber("TimeLeft", (int) (0.25f * 60.0f) + timerSecondsSince2000());
        END_ENTRY

        if (TimerElapsed("Clock"))
        {
            SET_STATE("Play");
        }

    STATE("Play")
        ON_ENTRY
            TimerSet("Clock", 10.0f); // This needs to be a parmeter - game duration
            VarSetNumber("TimeLeft", 600 + timerSecondsSince2000());
        END_ENTRY

        if (TimerElapsed("Clock"))
        {
            blueDamage = VarGetNumber("BlueDamage");
            redDamage = VarGetNumber("RedDamage");
            //EntityAddRaidPoints("BlueTeam", blueDamage * DAMAGE_TO_RAID_POINT_CONVERSION_FACTOR, 1);
            //EntityAddRaidPoints("RedTeam", redDamage * DAMAGE_TO_RAID_POINT_CONVERSION_FACTOR, 1);
            if (blueDamage > redDamage)
            {
                SET_STATE("BlueTeamWin");
            }
            else if (redDamage > blueDamage)
            {
                SET_STATE("RedTeamWin");
            }
            else
            {
                SET_STATE("Draw");
            }
        }

    STATE("BlueTeamWin")
        SendPlayerAttentionMessageWithColor("BlueTeam", StringAdd(VarGet("BlueSGName"), StringAdd(" ", StringLocalize(VarGet("WonText")))), WonTextWonColor, kFloaterStyle_Attention);
        SendPlayerAttentionMessageWithColor("RedTeam", StringAdd(VarGet("BlueSGName"), StringAdd(" ", StringLocalize(VarGet("WonText")))), WonTextLostColor, kFloaterStyle_Attention);
        SET_STATE("WrapupTime");

    STATE("RedTeamWin")
        SendPlayerAttentionMessageWithColor("RedTeam", StringAdd(VarGet("RedSGName"), StringAdd(" ", StringLocalize(VarGet("WonText")))), WonTextWonColor, kFloaterStyle_Attention);
        SendPlayerAttentionMessageWithColor("BlueTeam", StringAdd(VarGet("RedSGName"), StringAdd(" ", StringLocalize(VarGet("WonText")))), WonTextLostColor, kFloaterStyle_Attention);
        SET_STATE("WrapupTime");
        

    STATE("Draw")
        SendPlayerAttentionMessageWithColor(ALL_PLAYERS, VarGet("DrawText"), DrawTextColor, kFloaterStyle_Attention);
        SET_STATE("WrapupTime");

    STATE("WrapupTime")
        ON_ENTRY
            TimerSet("Clock", WRAPUP_DELAY);
            VarSetNumber("TimeLeft", (int) (WRAPUP_DELAY * 60.0f) + timerSecondsSince2000());
        END_ENTRY

        if (TimerElapsed("Clock"))
        {
            // BootEveryone
            EndScript();
        }
    END_STATES

    ON_STOPSIGNAL
        // BootEveryone
        EndScript();
    END_SIGNAL
}

void MayhemRaidInit()
{
    SetScriptName("MayhemRaid");
    SetScriptFunc(MayhemRaid);
    SetScriptType(SCRIPT_ZONE);

    SetScriptVar("BlueSGID",                    NULL,        SP_OPTIONAL);
    SetScriptVar("RedSGID",                        NULL,        SP_OPTIONAL);
    SetScriptVar("BlueSGName",                    NULL,        SP_OPTIONAL);
    SetScriptVar("RedSGName",                    NULL,        SP_OPTIONAL);
    SetScriptVar("BlueRaidID",                    NULL,        SP_OPTIONAL);
    SetScriptVar("RedRaidID",                    NULL,        SP_OPTIONAL);
    SetScriptVar("BlueDestruction",                NULL,        SP_OPTIONAL);
    SetScriptVar("RedDestruction",                NULL,        SP_OPTIONAL);

    SetScriptVar("GameDuration",                NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("WonText",                        NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("DrawText",                    NULL,        SP_DONTDISPLAYDEBUG);
    SetScriptVar("DamageText",                    NULL,        SP_DONTDISPLAYDEBUG);
}
