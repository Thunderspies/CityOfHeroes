// ZONE SCRIPT
//

// Runs the main IOP SG base raid

#include "scriptutil.h"
#include "timing.h"

#define WRAPUP_LENGTH	1.0		// in minutes
#define ATTACKER_CLOCK	3.0		// in minutes
#define CAPTURES_TO_WIN 3		// number of flag caps needed for victory

#define	NO_TEAM			0
#define	BLUE_TEAM		1
#define	RED_TEAM		2

static GLOWIEDEF BlueTeamIOP = NULL;
static GLOWIEDEF RedTeamIOP = NULL;

//void RaidComplete(int attackerswin, char* why, char * nameOfItemOfPowerCaptured, int creationTimeOfItemOfPowerCaptured );
//void RaidStop(void);
//int RaidTimeRemaining(void);
//int RaidPastEntryPhase(void);

static void UpdateUI(void)
{
	int UI = VarGetNumber("UI");
	int blueFlagState = 0;
	int redFlagState = 0;
	int blueCaptures = VarGetNumber("BlueCaptures");
	int redCaptures = VarGetNumber("RedCaptures");
	char reason[128];

	if (blueCaptures >= CAPTURES_TO_WIN)
	{
		strcpy(reason, VarGet("BlueSGName"));
		strcat(reason, StringLocalizeStatic("spacewon"));
		VarSet("Why", reason);
		SET_STATE("BlueTeamWin");
	}
	if (redCaptures >= CAPTURES_TO_WIN)
	{
		strcpy(reason, VarGet("RedSGName"));
		strcat(reason, StringLocalizeStatic("spacewon"));
		VarSet("Why", reason);
		SET_STATE("RedTeamWin");
	}
}

#include "bases.h"
#include "basesystems.h"
#include "basedata.h"
#include "earray.h"
#include "VillainDef.h"
#include "sgraid.h"
#include "error.h"
#include "entai.h"
#include "entity.h"
#include "entserver.h"
#include "entaiScript.h"
#include "scriptengine.h"
#include "dooranimcommon.h"
#include "svr_player.h"
#include "raidmapserver.h"
#include "sgraid_V2.h"

#if 0
void RaidItemInteract(Entity* player, RoomDetail* det)
{
	BaseRoom* room = baseGetRoom(&g_base, det->parentRoom); // needs work - g_raidbases
	Vec3 pos;
	int i, found = 0;

	if (!room)
	{
		Errorf("Couldn't get room in RaidItemInteract");
		return;
	}

	// check distance
	addVec3(room->pos, det->mat[3], pos);
	if (distance3Squared(pos, ENTPOS(player)) > SERVER_INTERACT_DISTANCE*SERVER_INTERACT_DISTANCE)
		return;

	// check force field
	for (i = 0; i < EArrayGetSize(&forcefields); i++)
	{
		Entity* ff = erGetEnt(*forcefields[i]);
		if (ff && nearSameVec3(ENTPOS(ff), pos) && ff->pchar->attrCur.fHitPoints > 0)
			found = 1;
	}

	if (!found)
	{
		ItemCaptured(det->info->pchName, det->creation_time);
	}
}
#endif

// Create the glowie defs for the IOPs.  This will need some major work: pull the models from the base def at
// loadin time (sgraid_V2.c, parseBaseIOP(...);  and use that data to create the up to four glowie defs for
// each side.
static void CreateIOPs(void)
{
#if 0
	if (BlueTeamIOP == NULL)
	{
		BlueTeamIOP = GlowieCreateDef(StringLocalize(VarGet("IOPName")), VarGet("BlueIOPModel"), 
												VarGet("IOPInteract"), VarGet("IOPInterrupt"), 
												VarGet("IOPComplete"), VarGet("IOPInteractBar"), 
												VarGetNumber("IOPInteractTime"));
	}
	if (RedTeamIOP == NULL)
	{
		RedTeamIOP = GlowieCreateDef(StringLocalize(VarGet("IOPName")), VarGet("RedIOPModel"), 
												VarGet("IOPInteract"), VarGet("IOPInterrupt"), 
												VarGet("IOPComplete"), VarGet("IOPInteractBar"), 
												VarGetNumber("IOPInteractTime"));
	}
#endif
}

// Destroy the glowies, and possibly destroy the glowiedefs
static void DestroyIOPs(void)
{
}

// Remove "I'm carrying an IOP" buffs from players
static void RemoveIOPpowers(void)
{
}

static void RaidStop(void)
{
}

// Called when the player enters the map - set their gang ID to their raid ID
int IOPRaidOnEnterMap(ENTITY player)
{
	int raidID = GetPlayerRaidID(player);
	SetAIGang(player, raidID);
	if (raidID == VarGetNumber("BlueSGID"))
	{
		SetScriptTeam(player, "BlueTeam");	
	}
	else if (raidID == VarGetNumber("RedSGID"))
	{
		SetScriptTeam(player, "RedTeam");	
	}
	else
	{
		devassertmsg(0, "Intruder detected on map");
	}
	return 0;
}

// Called when the player leaves the map.  Remove the "I'm carrying an IOP" buff from the player.
// Drop the IOP at their location if they were carrying, reset their gang ID to zero, and remove them from
// whatever team they're in
int IOPRaidOnLeaveMap(ENTITY player)
{
	SetAIGang(player, 0);
	// How do we remove an ENTITY from a TEAM?
	return 0;
}


void IOPRaid(void)
{
	INITIAL_STATE
		ON_ENTRY 
//			TimerSet("BlueClock", ATTACKER_CLOCK);
//			TimerSet("RedClock", ATTACKER_CLOCK);
//			TimerSet("Clock", 5.0f /* (F32)RaidTimeRemaining()/60.0 */ );
			VarSetNumber("BlueSGID", getBaseRaidSGID(0));
			VarSetNumber("RedSGID", getBaseRaidSGID(1));
			VarSet("BlueSGName", "Blue");
			VarSet("RedSGName", "Red");
			VarSetNumber("BlueRaidID", getBaseRaidRaidID(0));
			VarSetNumber("RedRaidID", getBaseRaidRaidID(1));
			VarSet("BlueFlagState", "FlagInBase");
			VarSet("RedFlagState", "FlagInBase");
			VarSetNumber("BlueCaptures", 0);
			VarSetNumber("RedCaptures", 0);

//			VarSetNumber("RaidEndTime", RaidTimeRemaining() + timerSecondsSince2000());
//			VarSet("Title", StringLocalizeStatic("BaseRaidTitle"));
//			ScriptUITitle(ONENTER("TitleUI"), "Title", StringLocalizeStatic("BaseRaidDesc"));
//			ScriptUITimer(ONENTER("TimerUI"), "RaidEndTime", StringLocalizeStatic("BaseRaidTimer"), StringLocalizeStatic("BaseRaidTimerDesc"));
//			ScriptUIMeter(ONENTER("DisruptorUI"), "DisruptorsRemaining", StringLocalizeStatic("BaseRaidDisruptors"), "1", "0", "5", "dd0000ff", "2255ffff",
//				StringLocalizeStatic("BaseRaidDisruptorsDesc"));
//			ScriptUIMeter(ONENTER("StabilizerUI"), "StabilizersRemaining", StringLocalizeStatic("BaseRaidStabilizers"), "1", "1", "6", "2255ffff", "dd0000ff",
//				StringLocalizeStatic("BaseRaidStabilizersDesc"));
//			ScriptUIMeter(ONENTER("TrophyUI"), "ForceFields", StringLocalizeStatic("BaseRaidForceField"), "1", "0", "100", "2255ffff", "dd0000ff",
//				StringLocalizeStatic("BaseRaidForceFieldDesc"));
//			CreateIOPs();

			OnPlayerEnteringMap( IOPRaidOnEnterMap );
			OnPlayerExitingMap( IOPRaidOnLeaveMap );
		END_ENTRY

		UpdateUI();
		if (TimerElapsed("Clock"))
		{
			VarSet("Why", "Time ran out");
			// if redcap > bluecap RedWin etc.
			SET_STATE("Draw");
		}

		if (NumEntitiesInTeam("BlueTeam"))
		{
			TimerSet("BlueClock", ATTACKER_CLOCK);
		}
		else if (TimerElapsed("BlueClock"))
		{
			char reason[256];
			strcpy(reason, VarGet("BlueSGName"));
			strcat(reason, " left" /* StringLocalizeStatic("spacedeparted") */);
			VarSet("Why", reason);
			SET_STATE("RedTeamWin");
		}
		if (NumEntitiesInTeam("RedTeam"))
		{
			TimerSet("RedClock", ATTACKER_CLOCK);
		}
		else if (TimerElapsed("RedClock"))
		{
			char reason[256];
			strcpy(reason, VarGet("RedSGName"));
			strcat(reason, " left" /* StringLocalizeStatic("spacedeparted") */);
			VarSet("Why", reason);
			SET_STATE("BlueTeamWin");
		}

	STATE("BlueTeamWin")
		BroadcastAttentionMessage(StringLocalizeStatic("BaseRaidBlue team Win"), NULL, 0.0);
		DestroyIOPs();
		RemoveIOPpowers();
		//RaidComplete(attackerswin, VarGet("Why"), VarGet("NameOfItemOfPowerCaptured"), VarGetNumber( "CreationTimeOfItemOfPowerCaptured"  ) );
		//VarSet("Title", StringLocalizeStatic(attackerswin?"BaseRaidAttackersWin":"BaseRaidDefendersWin"));
		SET_STATE("WrapupTime");


	STATE("RedTeamWin")
		BroadcastAttentionMessage(StringLocalizeStatic("BaseRaidDefendersWin"), NULL, 0.0);
		DestroyIOPs();
		RemoveIOPpowers();
		//RaidComplete(attackerswin, VarGet("Why"), VarGet("NameOfItemOfPowerCaptured"), VarGetNumber( "CreationTimeOfItemOfPowerCaptured"  ) );
		//VarSet("Title", StringLocalizeStatic(attackerswin?"BaseRaidAttackersWin":"BaseRaidDefendersWin"));
		SET_STATE("WrapupTime");
		

	STATE("Draw")
		BroadcastAttentionMessage(StringLocalizeStatic("NobodyWon"), NULL, 0.0);
		DestroyIOPs();
		RemoveIOPpowers();
		//RaidComplete(attackerswin, VarGet("Why"), VarGet("NameOfItemOfPowerCaptured"), VarGetNumber( "CreationTimeOfItemOfPowerCaptured"  ) );
		//VarSet("Title", StringLocalizeStatic(attackerswin?"BaseRaidAttackersWin":"BaseRaidDefendersWin"));
		SET_STATE("WrapupTime");


	STATE("WrapupTime")
		ON_ENTRY
			TimerSet("Clock", WRAPUP_LENGTH);
		END_ENTRY

		if (TimerElapsed("Clock"))
		{
			RaidStop();
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
		DestroyIOPs();
		RemoveIOPpowers();
		RaidStop();
		EndScript();
	END_SIGNAL
}

void IOPRaidInit()
{
	SetScriptName( "IOPRaid" );
	SetScriptFunc( IOPRaid );
	SetScriptType( SCRIPT_ZONE );		
}
