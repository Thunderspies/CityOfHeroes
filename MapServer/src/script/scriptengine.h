/*\
 *
 *    scriptengine.h/c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides the execution environment for CoH scripts
 *
 */

#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include "script.h"
#include "scripttable.h"
#include "storyarc/storyarcinterface.h"
#include "storyarc/missionobjective.h"
#include <utilitieslib/utils/scriptvars.h>
#include <utilitieslib/utils/textparser.h>
#include "gameData/store.h"

#include <lua.h>

#define FORCE_SCRIPT_TICK (-1.0f)

typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct ScriptTeam ScriptTeam;
typedef struct ScriptLocation ScriptLocation;

// can be embedded in other data files, or independent in a .scriptdef file
typedef struct ScriptDef
{
    const char*                filename;
    const char*                scriptname;
    SCRIPTVARS_STD_FIELD
} ScriptDef;
extern TokenizerParseInfo ParseScriptDef[];

typedef struct ScriptEnvironment
{
    // context
    ScriptFunction*        function;
    ScriptVarsTable     initialvars;
    StashTable            vars;
    StashTable            varsFlags;
    const MissionDef*    mission;
    EncounterGroup*        encounter;
    ScriptLocation*        location;
    Entity*                entity;

    // stuff created during execution
    ScriptTeam**        teams;
    EncounterGroup**    reservedEncounters;
    EncounterGroup**    encountersWithManualspawnDisabled;
    lua_State*            lua_interpreter;

    // time & run state
    U32                    timeDelta;    // diff between script time and dbSecondsSince2000()
    U32                    pauseTime;    // when the script was paused
    ScriptRunState        runState;    
    U32                    id;

    // ScriptUI Related Stuff
    ScriptUIWidget**    widgets;

    char*                pchSourceFile;

    // flags
    unsigned int        deleteMe : 1;
    unsigned int        closeMyEncounter : 1;
} ScriptEnvironment;

extern ScriptEnvironment** g_runningScripts;
extern ScriptEnvironment* g_currentScript;
extern U32 g_currentTime;
extern U32 g_currentKarmaTime;

// script data
void ScriptFreeData(void **sData);
MultiStore *ScriptDataGetStoreBeingUsed(Entity *player, Entity *merchant);

// vars
#define SE_DYNAMIC_LOOKUP (char*)0x00000001 // sentinel: needed if you work directly with vars hash table
void SetEnvironmentVar(ScriptEnvironment* script, const char* key, const char* value);
const char* GetEnvironmentVar(ScriptEnvironment* script, const char* key);
void RemoveEnvironmentVar(ScriptEnvironment* script, char* key);

void SetInstanceVar(const char* key, const char* value);
const char* GetInstanceVar(const char* key);
void RemoveInstanceVar(const char* key);

void ScriptError(char* s, ...);

// execution functions
U32 ScriptBegin(const char* scriptName, ScriptVarsTable* vars, ScriptType type, void* user, const char* errname, const char *dataFilename); // returns scriptid
int ScriptEnd(U32 scriptId, int immediate); // if immediate is set, script won't get STOP signal.  
int ScriptSendSignal(U32 scriptId, char* signal);
int ScriptPause(U32 scriptId, int on);
int ScriptReset(U32 scriptId);

int ScriptRunIdFromName(char* scriptName, ScriptType type);    // gets the last matching script
ScriptEnvironment* ScriptFromRunId(U32 scriptId);

// script closures - script function wrapped with script context
// now scripts can be called outside the main loop
typedef struct ScriptClosure ScriptClosure;
typedef ScriptClosure** ScriptClosureList;
void SetScriptClosure(ScriptClosureList* list, U32 scriptid, void* f, const char* lua_f); // NULL f && NULL lua_f to remove from list
typedef int (*SCLC_Function)(void* func, void* param1, void* param2);
int ScriptClosureListCall(ScriptClosureList* list, SCLC_Function func, void* param1, void* param2); // process until func returns true
void ScriptClosureListDestroy(ScriptClosureList* list);

// script defs
void ScriptDefLoad(void);
const ScriptDef* ScriptDefFromFilename(const char* filename);
U32 ScriptBeginDef(const ScriptDef* def, int type, void* user, const char* errname, const char *dataFilename);

// script locations
void ScriptLocationLoad(void);
void ScriptLocationTick(void);
void LocationNotifyScriptStopping(ScriptLocation* location, int scriptid);
void ScriptLocationDebugPrint(ClientLink* client);
void ScriptLocationStart(ClientLink* client, char* name);
void ScriptLocationStop(ClientLink* client, char* name);
void ScriptLocationSignal(ClientLink* client, char* name, char* signal);

// shard events
void ScriptHandleMapGroup(void);
void ShardEventBegin(ClientLink* client, char* scriptName);
void ShardEventEnd(ClientLink* client);
void ShardEventSignal(ClientLink* client, char* signal);

// zone scripts
void ZoneScriptsStart(void);

// entity scripts
typedef struct Entity Entity;
void EntityScriptsBegin(Entity *entity, const ScriptDef **scripts, int numScripts, ScriptVarsTable *vars, const char *dataFilename);
void EntityScriptsStop(Entity *entity);

// Global hooks
void ScriptTick(F32 timestep); // USe FORCE_SCRIPT_TICK to force this to run
void ScriptMarkerLoad(void);
void ScriptEndAllRunningScripts(bool immediate);
void ScriptPlayerEnteringMap(Entity* player);
void ScriptPlayerExitingMap(Entity* player);
int ScriptPlayerTeleporting(Entity* player);
int ScriptPlayerMissionPort(Entity* player);
int ScriptSendScriptMessage(const char* message);
int ScriptNpcClick(Entity* player, Entity* npc);
int ScriptScriptedContactClick(Entity* player, int link, ContactResponse* response);
void ScriptPlayerLeavesTeam(Entity* player);
void ScriptPlayerJoinsTeam(Entity* player);
void ScriptPlayerLeavesVolume(Entity* player, char *name);
void ScriptPlayerEntersVolume(Entity* player, char *name);
void ScriptPlayerDefeatedByPlayer(Entity* player, Entity* victor);
void ScriptEntityDefeated(Entity* entity, Entity* victor);
void ScriptTeamRewarded(Entity* entity, Entity* victor);
void ScriptZoneEventReward(Entity* entity, Entity* victor, float damage);
void ScriptEntityFreed(Entity* entity);
void ScriptEntityCreated(Entity* entity);
int ScriptGlowieInteract(Entity* player, MissionObjectiveInfo* info, int success);
void ScriptGlowieComplete(Entity* player, MissionObjectiveInfo* info, int success);
int ScriptGlowiesPlaced();
void ScriptGlowiesTick();
void OpenStoreExternal(Entity* merchant, Entity* client, const char *name);

// from scriptstring.c
char* StringAlloc(int len);
char* StringDupe(const char* str);
void FreeAllTempStrings(void);

// script teams
void ScriptTeamDestroyAll(ScriptEnvironment* script);
void EncounterStopReservingAll(ScriptEnvironment* script);
void EncounterReenableManualSpawnAll(ScriptEnvironment* script);

// Functions that have seeped out into the rest of the game code for convenience
int GetPointFromLocation(LOCATION location, Vec3 vec);
int GetMatFromLocation(LOCATION location, Mat4 mat, const Vec3 referencePoint);
int PointInArea(const Vec3 vec, AREA area, Entity* e);

// Testing script functions
void SetStartZone(NUMBER mapID);
void CmdScriptDefStart(const ScriptDef* pScriptDef, Entity *e);
void CmdScriptLuaStart(const char* luaFunc, const char* luaFile, const char* luaString, Entity *e);

// pvp map functions
int OnPvPMap();
void SetScriptCombatLevel(NUMBER combatLevel);
void SetScriptMinCombatLevel(NUMBER combatLevel);
void SetScriptMaxCombatLevel(NUMBER combatLevel);
int ScriptLevelOverride(Character* p);
bool ScriptUsingLevelOverride();                        // this returns true only if the exact level is set (SetScriptCombatLevel)
int ScriptGetEffectiveLevel(int level);

// global NPC behavior
int AreAllNPCsScared(void);

// lua Debug - scriptlua.c
int LuaMemoryUsed(ScriptEnvironment* script);

// lua help
void printCOHLuaLibraries(const char * libName, char * output);

#endif // SCRIPTENGINE_H
