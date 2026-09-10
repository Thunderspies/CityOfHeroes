#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaTeam.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"

//int NumEntitiesInTeam(TEAM team)
static int l_NumEntitiesInTeam (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    lua_pushnumber(L, NumEntitiesInTeam(team));
    return 1;
}

//int NumEntitiesInTeamEvenDead(TEAM team)
static int l_NumEntitiesInTeamEvenDead (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    lua_pushnumber(L, NumEntitiesInTeamEvenDead(team));
    return 1;
}

//ENTITY GetEntityFromTeam(TEAM team, int number)
static int l_GetEntityFromTeam (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    int number = luaL_checkint(L, 2);
    lua_pushstring(L, GetEntityFromTeam(team, number));
    return 1;
}

//void SetScriptTeam( TEAM currentTeam, TEAM additionalTeam )
static int l_SetScriptTeam (lua_State *L) {
    TEAM currentTeam = luaL_checkstring(L, 1);
    TEAM additionalTeam = luaL_checkstring(L, 2);
    SetScriptTeam(currentTeam, additionalTeam );
    return 0;
}

//void SwitchScriptTeam( TEAM team, TEAM fromTeam, TEAM newTeam )
static int l_SwitchScriptTeam (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    TEAM fromTeam = luaL_checkstring(L, 2);
    TEAM newTeam = luaL_checkstring(L, 3);
    SwitchScriptTeam(team, fromTeam, newTeam);
    return 0;
}

//void SayOnKillHero(TEAM team, STRING text)
static int l_SayOnKillHero (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    STRING text = luaL_checkstring(L, 2);
    SayOnKillHero(team, text);
    return 0;
}

//void TeleportToLocation(TEAM team, LOCATION loc)
static int l_TeleportToLocation (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    LOCATION loc = luaL_checkstring(L, 2);
    TeleportToLocation(team, loc);
    return 0;
}

// void TeamGrantReward(TEAM team, STRING reward)
static int l_TeamGrantReward (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    STRING reward = luaL_checkstring(L, 2);
    TeamGrantReward(team, reward);
    return 0;
}

const luaL_Reg scriptTeamLib [] = 
{
    {"NumEntitiesInTeam",        l_NumEntitiesInTeam},
    {"NumEntitiesInTeamEvenDead",    l_NumEntitiesInTeamEvenDead},
    {"GetEntityFromTeam",        l_GetEntityFromTeam},
    {"SetScriptTeam",            l_SetScriptTeam},
    {"SwitchScriptTeam",        l_SwitchScriptTeam},
    {"TeleportToLocation",        l_TeleportToLocation},
    {"SayOnKillHero",            l_SayOnKillHero},
    {"GrantReward",                l_TeamGrantReward},
    {NULL, NULL}  //sentinel - REQUIRED
};
