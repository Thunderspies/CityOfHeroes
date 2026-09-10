#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaScript.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"
#include "entity/character_eval.h"
#include <utilitieslib/utils/file.h>                // needed for isDevelopmentMode()
#include "gameSys/Turnstile.h"
#include "storyarc/mission.h"            // needed for turnstileConfigDef somehow

// Script Systems Library Defintions

//NUMBER SecondsSince2000(void)
static int l_Now (lua_State *L) {
    lua_pushnumber(L, SecondsSince2000());
    return 1;
}

//NUMBER CurrentTime(void)
static int l_CurrentTime (lua_State *L) {
    lua_pushnumber(L, CurrentTime());
    return 1;
}

//see static int ScriptedZoneEventOnMessageReceived(STRING param) for why this exists
static int l_CharEvalResultSet (lua_State *L) {
    NUMBER result = luaL_checkinteger(L, 1);
    g_scriptCharEvalResult = result;
    return 0;
}

static int l_GetScriptID (lua_State *L) {
    lua_pushnumber(L, g_currentScript->id);
    return 1;
}

//int isDevelopmentMode(void)
static int l_isDevelopmentMode (lua_State *L)
{
    lua_pushboolean(L, isDevelopmentMode());
    return 1;
}

//Combination of:
//void SetScriptCombatLevel(NUMBER combatLevel)
//void SetScriptMinCombatLevel(NUMBER combatLevel)
//void SetScriptMaxCombatLevel(NUMBER combatLevel)
static int l_SetCombatLevelRange (lua_State *L) {
    NUMBER minCombatLevel = luaL_checkinteger(L, 1);
    NUMBER maxCombatLevel = luaL_checkinteger(L, 2);
    if(minCombatLevel == maxCombatLevel)
    {
        SetScriptCombatLevel(minCombatLevel);
        SetScriptMinCombatLevel(0); // clear
        SetScriptMaxCombatLevel(0);
    }
    else
    {
        SetScriptCombatLevel(0); // clear
        SetScriptMinCombatLevel(minCombatLevel);
        SetScriptMaxCombatLevel(maxCombatLevel);
    }
    return 0;
}

//void DisableGurneysOnThisMap( bool flag )
static int l_DisableGurneysOnThisMap (lua_State *L) {
    bool flag = lua_toboolean(L, 1);
    DisableGurneysOnThisMap(flag);
    return 0;
}

// Turnstile Library Defintions

//int turnstileMapserver_eventLockedByGroup()
static int l_turnstileMapserver_eventLockedByGroup (lua_State *L)
{
    lua_pushboolean(L, turnstileMapserver_eventLockedByGroup());
    return 1;
}

//see static void scripteval_TurnstileMissionName(EvalContext *pcontext) in ScriptedZoneEvent.c
static int l_turnstileMapserver_eventName (lua_State *L)
{
    int missionID = 0;

    turnstileMapserver_getMapInfo(NULL, &missionID);

    if(EAINRANGE(missionID, turnstileConfigDef.missions))
        lua_pushstring(L, turnstileConfigDef.missions[missionID]->name);
    else
        lua_pushnil(L);
    
    return 1;
}

//see static void SZE_TrialComplete(TrialStatus status) in ScriptedZoneEvent.c
static int l_CompleteEvent (lua_State *L)
{
    STRING statusString = luaL_checkstring(L, 1);
    int status = StaticDefineIntLookupInt(TrialStatusEnum, statusString);

    if (status > trialStatus_None && OnMissionMap())
    {
        SetTrialStatus(status);
        SendTrialStatus(ALL_PLAYERS);
    }
    return 0;
}

//void SkyFileFade(NUMBER sky1, NUMBER sky2, FRACTION fade);
static int l_SkyFileFade (lua_State *L)
{
    NUMBER sky1 = luaL_checkinteger(L, 1);
    NUMBER sky2 = luaL_checkinteger(L, 2);
    FRACTION fade = luaL_checknumber(L, 3);
    SkyFileFade(sky1, sky2, fade);
    return 0;
}

//NUMBER SkyFileGetByName(STRING name);
static int l_SkyFileGetByName (lua_State *L)
{
    STRING name = luaL_checkstring(L, 1);
    lua_pushnumber(L, SkyFileGetByName(name));
    return 1;
}

const luaL_Reg scriptScriptLib [] = 
{
    {"IsDevMode",                l_isDevelopmentMode},
    {"Now",                        l_Now},
    {"CurrentTime",                l_CurrentTime},
    {"GetID",                    l_GetScriptID},
    {"CharEvalResultSet",        l_CharEvalResultSet},
    {"SetCombatLevelRange",        l_SetCombatLevelRange},
    {"DisableGurneys",            l_DisableGurneysOnThisMap},
    {"SkyFileFade",                l_SkyFileFade},
    {"SkyFileGetByName",        l_SkyFileGetByName},
    {NULL, NULL}  //sentinel - REQUIRED
};

const luaL_Reg scriptTurnstileLib [] = 
{
    {"IsEventLocked",            l_turnstileMapserver_eventLockedByGroup},
    {"EventName",                l_turnstileMapserver_eventName},
    {"CompleteEvent",            l_CompleteEvent},
    {NULL, NULL}  //sentinel - REQUIRED
};
