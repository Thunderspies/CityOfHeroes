/*\
 *
 *    script.h - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides all the functions CoH script files should be referencing -
 *    essentially a list of hooks into game systems.
 *
 */

#ifndef SCRIPT_H
#define SCRIPT_H
// ScriptParam.flags

#include <utilitieslib/stdtypes.h>
#include "gameComm/debugCommon.h" // for predefined variable types
#include "scriptconstants.h"
#include <utilitieslib/utils/mathutil.h>
#include "entity/entity_enum.h"
#include "entity/character_base.h"
#include "scripttypes.h"
#include "ZoneEvents/ScriptUI.h"
#include "storyarc/contactCommon.h"
#include "storyarc/contactDialog.h"
#include "taskpvp.h"
#include <utilitieslib/components/StashTable.h>
#include "sound_common.h"

#include "Scripthook/ScriptHookWaypoint.h"
#include "Scripthook/ScriptHookEntityTeam.h"
#include "Scripthook/ScriptHookReward.h"
#include "Scripthook/ScriptHookEncounter.h"
#include "Scripthook/ScriptHookLocation.h"
#include "Scripthook/ScriptHookMission.h"
#include "Scripthook/ScriptHookCallbacks.h"
#include "Scripthook/ScriptHookContact.h"
#include "Scripthook/ScriptHookAI.h"

// script doors
void OpenDoor(  LOCATION doorName );
void CloseDoor(  LOCATION doorName );
void LockDoor(  LOCATION doorName, STRING msg );
void UnlockDoor(  LOCATION doorName );

// sky files
void SkyFileFade(NUMBER sky1, NUMBER sky2, FRACTION fade);
NUMBER SkyFileGetByName(STRING name);

// dynamic geometry
NUMBER DynamicGeometryFind(STRING name, LOCATION location, FRACTION radius);
void DynamicGeometrySetHP(NUMBER id, NUMBER hp, int repair);
NUMBER DynamicGeometryGetHP(NUMBER id);


/////////////////////////////////////////////////////////////////////////////////////////
// glowies
typedef void *GLOWIEDEF;
typedef enum {
    kGlowieEffectInactive,
    kGlowieEffectActive,
    kGlowieEffectCooldown,
    kGlowieEffectCompletion,
    kGlowieEffectFailure,
    kGlowieEffectNum
} ScriptGlowieEffectType;

GLOWIEDEF GlowieCreateDef(STRING name, 
                            STRING model,
                            STRING InteractBeginString,
                            STRING InteractInterruptedString,
                            STRING InteractCompleteString,
                            STRING InteractActionString,
                            NUMBER delay);
ENTITY GlowiePlace(GLOWIEDEF def,
                   LOCATION position, 
                   NUMBER reusable,
                   PlayerInteractWithNPCOrGlowieFunc click,
                   PlayerInteractWithNPCOrGlowieFunc done);
ENTITY GlowiePlaceEx(GLOWIEDEF glowdef,
                     LOCATION position,
                     NUMBER reusable,
                     PlayerInteractWithNPCOrGlowieFunc click, STRING lua_click,
                     PlayerInteractWithNPCOrGlowieFunc done, STRING lua_done);
Entity *ZowiePlace(GLOWIEDEF glowdef, Mat4 position, NUMBER reusable);

void GlowieRemove(ENTITY glowie);
int GlowieGetState(ENTITY glowie);
void GlowieSetState(ENTITY glowie);
void GlowieClearState(ENTITY glowie);
void GlowieAddEffect(GLOWIEDEF glowdef, ScriptGlowieEffectType type, STRING fx);
void GlowieSetDescriptions(GLOWIEDEF glowdef, STRING description, STRING singulardescription);
void GlowieSetActivateRequires(GLOWIEDEF glowdef, STRING requiresParam);
void GlowieSetCharRequires(GLOWIEDEF glowdef, STRING requiresParam, STRING failText);
void GlowieClearAll(void);


// Util
void ScriptPlayFXOnEntity(ENTITY entity, STRING fxName);
void SendToAllMapsMessage(STRING localizedMessage );
void BroadcastAttentionMessage(STRING localizedMessage, LOCATION location, FRACTION radius );    // sent to everyone on map, or within this radius if there is one
void SendPlayerAttentionMessageWithColor( ENTITY player, STRING message, NUMBER color, int alertType );
void SendPlayerAttentionMessage( ENTITY player, STRING message );
void SendPlayerCaption( ENTITY player, STRING message );
void SendPlayerPopHelp( ENTITY player, STRING popHelpMessage );
void SendPlayerPopHelpReset( ENTITY player );
void SendPlayerSystemMessage( ENTITY player, STRING message );
void SendPlayerFadeSound( ENTITY player, NUMBER channel, FRACTION fadeTime );
void SendPlayerSound( ENTITY player, STRING sound, NUMBER channel, FRACTION volumeLevel );
void ResetPlayerMusic(TEAM team);
void SendPlayerDialog( ENTITY player, STRING message );
void SendPlayerDialogWithIgnore( ENTITY player, STRING message, NUMBER type );
void EndScript(void);
ScriptType GetScriptType(void);
void DebugString(STRING string);
void SetDayNightCycle(DayNightCycleEnum daynight);
FRACTION GetDayNightTime();
void ScriptSkyFileSetFade(NUMBER sky1, NUMBER sky2, FRACTION seconds);
void ScriptSkyFadeTick(F32 timestep);
void EntityScriptBeginScriptDefFromScript(ENTITY entity, STRING scriptDef);
void EntityScriptBeginFunctionFromScript(ENTITY entity, STRING script);

// Timers
#define SCRIPT_TICKS_PER_SECOND 2
int DoEvery(STRING timerName, FRACTION minutes, void(*func)(void));
NUMBER CurrentTime(void);
void TimerSet(STRING timerName, FRACTION minutes);
int TimerElapsed(STRING timerName);
void TimerRemove(STRING timer);
FRACTION TimerRemain(STRING timer);
NUMBER SecondsSince2000(void);
FRACTION CpuTicksInSeconds(void);
FRACTION TimeSinceCpuTicksInSeconds(FRACTION start);

// Script vars
int VarIsEmpty(VARIABLE var);
void VarSetEmpty(VARIABLE var);
STRING VarGet(VARIABLE var);
void VarSet(VARIABLE var, STRING value);
void VarSetHidden(VARIABLE var, STRING value);
int VarEqual(VARIABLE var1, VARIABLE var2);
int VarIsTrue(VARIABLE var);
int VarIsFalse(VARIABLE var);
void VarSetTrue(VARIABLE var);
void VarSetFalse(VARIABLE var);
NUMBER VarGetNumber(VARIABLE var);
void VarSetNumber(VARIABLE var, NUMBER value);
void VarSetNumberHidden(VARIABLE var, NUMBER value);
FRACTION VarGetFraction(VARIABLE var);
void VarSetFraction(VARIABLE var, FRACTION value);
void VarSetFractionHidden(VARIABLE var, FRACTION value);
NUMBER VarGetArrayCount(VARIABLE var);
STRING VarGetArrayElement(VARIABLE var, NUMBER index);
NUMBER VarGetArrayElementNumber(VARIABLE var, NUMBER index);
FRACTION VarGetArrayElementFraction(VARIABLE var, NUMBER index);
void VarSetArrayElement(VARIABLE var, NUMBER index, STRING value);
void VarSetArrayElementNumber(VARIABLE var, NUMBER index, NUMBER value);

mSTRING EntVar(ENTITY ent, STRING var);

// instance vars - these are shared across all scripts on this instance of the mapserver
STRING InstanceVarGet(VARIABLE var);
void InstanceVarSet(VARIABLE var, STRING value);
void InstanceVarRemove(VARIABLE var);
int InstanceVarIsEmpty(VARIABLE var);

// Numbers and strings
FRACTION RandomFraction(FRACTION min, FRACTION max);
NUMBER RandomNumber(NUMBER min, NUMBER max);
int StringEqual(STRING lhs, STRING rhs);
STRING StringAdd(STRING lhs, STRING rhs);
int StringEmpty(STRING str);
STRING StringCopy(STRING str);
mSTRING StringCopySafe(STRING str);
mSTRING NumberToString(NUMBER i);
mSTRING FractionToString(FRACTION f);
mSTRING TimeToString(NUMBER seconds);
mSTRING FractionTimeToString(FRACTION seconds);
NUMBER StringToNumber(STRING str);
FRACTION StringToFraction(STRING str);
NUMBER StringHash(STRING str);
mSTRING StringLocalize(STRING str);
mSTRING StringLocalizeStatic(STRING str);                        // used to look up items in StoryarcStrings.ms
mSTRING StringLocalizeMenuMessages(STRING str);                    // used to look up items in MenuMessages.ms
mSTRING StringLocalizeWithVars(STRING str, ENTITY player);
mSTRING StringLocalizeWithDBID(STRING str, NUMBER playerDBID);
mSTRING StringLocalizeWithReplacements(STRING str, NUMBER nParams, ...);


void GiveRewardToAllInRadius( STRING reward , LOCATION location, FRACTION radius );
void ShowTimer(FRACTION time, LOCATION location, FRACTION radius );
LOCATION EntityPos(ENTITY ent);
void examineScript(void); //a hole where I can look at variables in the debugger
void OverrideEncounterGroupsInMap( STRING layout, STRING spawn, FRACTION chance );
void ClearEncounterGroupOverRides();
void ReserveEncountersInRadius( LOCATION loc, FRACTION radius );

void DisableManualSpawn( STRING groupName );

int ScriptCountRunningInstances(STRING name);
void ScriptControlsCompletion( ENCOUNTERGROUP encounter );
STRING GetGenericMissionSpawn(void);
NUMBER IsGenericMissionSpawn(ENCOUNTERGROUP grp);
NUMBER EntityFledMap( ENTITY entity );
void SetShowOnMap(TEAM team, NUMBER flag);

ENCOUNTERSTATE SetEncounterActive(ENCOUNTERGROUP encounter);
ENCOUNTERSTATE SetEncounterInactive(ENCOUNTERGROUP encounter);

//////////////////////////////////////////////////////////////////////
// PvP
//////////////////////////////////////////////////////////////////////
void SetPvPMap(NUMBER flag);        // used to notify server that this is a PvP map
void SetScriptCombatLevel(NUMBER combatLevel);
void SetScriptMinCombatLevel(NUMBER combatLevel);
void SetScriptMaxCombatLevel(NUMBER combatLevel);
void SetBloodyBayTeam( ENTITY player );
void RunBloodyBayTick(void);

//////////////////////////////////////////////////////////////////////
// NPCs Globals
//////////////////////////////////////////////////////////////////////
void SetAreAllNPCsScared(NUMBER flag);

//////////////////////////////////////////////////////////////////////
// Testing script functions
//////////////////////////////////////////////////////////////////////
void SetStartZone(NUMBER mapID);

//////////////////////////////////////////////////////////////////////
/////Script init
// WARNING - all the script init functions save a pointer to the passed string, 
// you should only use compile-time constants for these strings
//////////////////////////////////////////////////////////////////////

void SetScriptName( STRING name );
void SetScriptFunc( void * func );
void SetScriptFuncToRunOnInitialization( void * func );
void SetScriptType( NUMBER scriptType );    
void SetScriptVar( STRING var, STRING defaultVal, NUMBER flags ); 
void SetScriptSignal( STRING displayStr, STRING signalStr );


//////////////////////////////////////////////////////////////////////
// B A D G E S
//////////////////////////////////////////////////////////////////////
typedef enum ScriptBadgeType {
    kScriptBadgeTypePresents,
    kScriptBadgeTypeRockets,
    kScriptBadgeTypeRVPillbox,
    kScriptBadgeTypeRVHeavyPet,
    kScriptBadgeTypeBBOreConvert,
    kScriptBadgeTypeRWZBombPlace,
    kScriptBadgeTypeNum
} ScriptBadgeType;

void GiveBadgeCredit( ENTITY player, ScriptBadgeType type );
void GiveBadgeStat(TEAM team, STRING badgeStat, NUMBER count);
void CountTowardGhostTrapBadge( ENTITY deadGhost );

void AllowHeroAndVillainPlayerTypesToTeamUpOnThisMap( bool test );
void AllowPrimalAndPraetorianPlayersToTeamUpOnThisMap( bool test );
void DisableGurneysOnThisMap( bool test );
void SendMessageToPlayer( ENTITY player, char *fmt, ... );
void ScriptDBLog(char *basis, ENTITY player, char *fmt, ...);
ENCOUNTERGROUP LaunchGenericWaveAttack( LOCATION target, STRING layout, STRING spawnName, STRING shout );
void TeamSZERewardLogMsg(TEAM team, STRING eventName, STRING message);

// CutScene
void StartCutSceneFromString(STRING cutSceneString, EncounterGroup *group);

#endif // SCRIPT_H


