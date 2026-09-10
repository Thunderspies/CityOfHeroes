#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaCallback.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"
#include "script/scriptengine.h"
#include "script/Scripthook/ScriptHookInternal.h"
#include <utilitieslib/utils/timing.h>

int lua_callbackEntityString(STRING call, Entity* param1, STRING param2)
{
    return lua_callback(call, EntityNameFromEnt(param1), param2);
}

int lua_callbackEntityEntity(STRING call, Entity* param1, Entity* param2)
{
    return lua_callback(call, EntityNameFromEnt(param1), EntityNameFromEnt(param2));
}

int lua_callbackEntityPlayerTeam(STRING call, Entity* param1, Entity* param2)
{
    return lua_callback(call, EntityNameFromEnt(param1), GetPlayerTeamFromPlayer(EntityNameFromEnt(param2)));
}

int lua_callback(STRING call, STRING param1, STRING param2)
{
    unsigned int saved_fpu_control;
    int args = 0, result = 0;
    /* the Lua interpreter */
    lua_State* L = NULL;
    if(g_currentScript->lua_interpreter)
    {
        L = g_currentScript->lua_interpreter;
    }
    else
    {
        if(call)
            ScriptError("Lua: Callback %s called without a running interpreter\n", call);
        else
            ScriptError("Lua: NULL Callback called\n");
        return 0;
    }

    if(!call || !call[0])
        return 0;

    // make sure we have FPU in full precision or Lua will screw up
    _controlfp_s(&saved_fpu_control, 0, 0);    // retrieve current control word
    _controlfp_s(NULL, _PC_64, _MCW_PC);    // set the fpu to full precision

    lua_getglobal(L, call);
    if(lua_isfunction(L, -1))
    {
        if(param1 && param1[0])
        {
            lua_pushstring(L, param1);
            args++;
        }
        if(param2 && param2[0])
        {
            lua_pushstring(L, param2);
            args++;
        }

        if (lua_pcall(L, args, 1, 0) != 0)
            ScriptError("Lua: Error in script callback: %s\n", lua_tostring(L, -1));
        else
        {
            result = (int) lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
    }
    else
        lua_pop(L, 1);

    // Restore FPU Precision
    _controlfp_s(NULL, saved_fpu_control, _MCW_PC);

    return result;
}

void lua_exec(STRING statement, FRACTION magnitude, FRACTION duration)
{
    unsigned int saved_fpu_control;
    /* the Lua interpreter */
    lua_State* L = NULL;
    if(g_currentScript->lua_interpreter)
    {
        L = g_currentScript->lua_interpreter;
    }
    else
    {
        if(statement)
            ScriptError("Lua: lua_exec %s called without a running interpreter\n", statement);
        else
            ScriptError("Lua: lua_exec called with NULL\n");
        return;
    }

    if(!statement || !statement[0])
        return;

    PERFINFO_AUTO_START("scriptLua Callback", 1);

    // make sure we have FPU in full precision or Lua will screw up
    _controlfp_s(&saved_fpu_control, 0, 0);    // retrieve current control word
    _controlfp_s(NULL, _PC_64, _MCW_PC);    // set the fpu to full precision

    // These values are set from the powers system
    lua_pushnumber(L, magnitude);
    lua_setglobal(L, "Magnitude");

    lua_pushnumber(L, duration);
    lua_setglobal(L, "Duration");

    if(luaL_dostring(L, statement))
        ScriptError("Lua: Error in lua_exec: %s\n", lua_tostring(L, -1));

    PERFINFO_AUTO_STOP();

        // Restore FPU Precision
    _controlfp_s(NULL, saved_fpu_control, _MCW_PC);

    return;
}

static int l_OnPlayerExitingMap (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnPlayerExitingMap(lua_f);
    return 0;
}
static int l_OnPlayerEnteringMap (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    OnPlayerEnteringMapEx(NULL, lua_f);
    return 0;
}
static int l_OnPlayerTeleporting (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnPlayerTeleporting(lua_f);
    return 0;
}
static int l_OnPlayerMissionPort (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnPlayerMissionPort(lua_f);
    return 0;
}
static int l_OnScriptMessage (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnScriptMessage(lua_f);
    return 0;
}
static int l_OnPlayerInteractWithNPCOrGlowie (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    TEAM target = luaL_checkstring(L, 2);
    luaCallback_OnPlayerInteractWithNPCOrGlowie(lua_f, target);
    return 0;
}
static int l_OnPlayerLeavesTeam (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnPlayerLeavesTeam(lua_f);
    return 0;
}
static int l_OnPlayerJoinsTeam (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnPlayerJoinsTeam(lua_f);
    return 0;
}
static int l_OnPlayerLeavesVolume (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnPlayerLeavesVolume(lua_f);
    return 0;
}
static int l_OnPlayerEntersVolume (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnPlayerEntersVolume(lua_f);
    return 0;
}
static int l_OnPlayerDefeatedByPlayer (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnPlayerDefeatedByPlayer(lua_f);
    return 0;
}
static int l_OnEntityFreed (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnEntityFreed(lua_f);
    return 0;
}
static int l_OnEntityCreated (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnEntityCreated(lua_f);
    return 0;
}
static int l_OnEntityDefeated (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnEntityDefeated(lua_f);
    return 0;
}
static int l_OnTeamRewarded (lua_State *L) {
    STRING lua_f = lua_tostring(L, 1);
    luaCallback_OnTeamRewarded(lua_f);
    return 0;
}

const luaL_Reg scriptCallbackLib [] = 
{
    {"OnPlayerExitingMap",        l_OnPlayerExitingMap},
    {"OnPlayerEnteringMap",        l_OnPlayerEnteringMap},
    {"OnPlayerTeleporting",        l_OnPlayerTeleporting},
    {"OnPlayerMissionPort",        l_OnPlayerMissionPort},
    {"OnScriptMessage",            l_OnScriptMessage},
    {"OnPlayerInteractWithNPCOrGlowie",        l_OnPlayerInteractWithNPCOrGlowie},
    {"OnPlayerLeavesTeam",        l_OnPlayerLeavesTeam},
    {"OnPlayerJoinsTeam",        l_OnPlayerJoinsTeam},
    {"OnPlayerLeavesVolume",    l_OnPlayerLeavesVolume},
    {"OnPlayerEntersVolume",    l_OnPlayerEntersVolume},
    {"OnPlayerDefeatedByPlayer",l_OnPlayerDefeatedByPlayer},
    {"OnEntityFreed",            l_OnEntityFreed},
    {"OnEntityCreated",            l_OnEntityCreated},
    {"OnEntityDefeated",        l_OnEntityDefeated},
    {"OnTeamRewarded",            l_OnTeamRewarded},
    {NULL, NULL}  //sentinel - REQUIRED
};
