#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaReward.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"

//void EntityGrantReward(ENTITY ent, STRING rewardTable)
static int l_EntityGrantReward (lua_State *L)
{
    ENTITY ent = luaL_checkstring(L, 1);
    STRING rewardTable = luaL_checkstring(L, 2);
    EntityGrantReward(ent, rewardTable);
    return 0;
}

//int EntityHasRewardToken(ENTITY ent, STRING token)
static int l_EntityHasRewardToken (lua_State *L)
{
    ENTITY ent = luaL_checkstring(L, 1);
    STRING token = luaL_checkstring(L, 2);
    lua_pushboolean(L, EntityHasRewardToken(ent, token));
    return 1;
}

//int EntityGetRewardToken(ENTITY ent, STRING token)
// returns -1 if no token of this name
static int l_EntityGetRewardToken (lua_State *L)
{
    ENTITY ent = luaL_checkstring(L, 1);
    STRING token = luaL_checkstring(L, 2);
    lua_pushnumber(L, EntityGetRewardToken(ent, token));
    return 1;
}

//int EntityGetRewardTokenTime(ENTITY ent, STRING token)
// returns -1 if no token of this name, otherwise returns time that the token was last awarded in seconds since 2000
static int l_EntityGetRewardTokenTime (lua_State *L)
{
    ENTITY ent = luaL_checkstring(L, 1);
    STRING token = luaL_checkstring(L, 2);
    lua_pushnumber(L, EntityGetRewardTokenTime(ent, token));
    return 1;
}

//void EntityGrantRewardToken(ENTITY ent, STRING token, NUMBER value)
// modifies the timestamp of the token
static int l_EntityGrantRewardToken (lua_State *L)
{
    ENTITY ent = luaL_checkstring(L, 1);
    STRING token = luaL_checkstring(L, 2);
    NUMBER value = luaL_checkint(L, 3);
    EntityGrantRewardToken(ent, token, value);
    return 0;
}

//void EntityRemoveRewardToken(ENTITY ent, STRING token)
static int l_EntityRemoveRewardToken (lua_State *L)
{
    ENTITY ent = luaL_checkstring(L, 1);
    STRING token = luaL_checkstring(L, 2);
    EntityRemoveRewardToken(ent, token);
    return 0;
}

//int EntityHasBadge(ENTITY ent, STRING badgeName)
static int l_EntityHasBadge (lua_State *L)
{
    ENTITY entity = luaL_checkstring(L, 1);
    STRING badgeName = luaL_checkstring(L, 2);
    lua_pushnumber(L, EntityHasBadge(entity, badgeName));
    return 1;
}

// void MissionGrantReward(TEAM team, STRING reward)
static int l_MissionGrantReward (lua_State *L) {
    STRING reward = luaL_checkstring(L, 1);
    MissionGrantReward(reward);
    return 0;
}

//void EntityGrantPower(ENTITY ent, STRING power)
static int l_EntityGrantPower (lua_State *L)
{
    ENTITY ent = luaL_checkstring(L, 1);
    STRING power = luaL_checkstring(L, 2);
    EntityGrantPower(ent, power);
    return 0;
}

//void EntityRemovePower(ENTITY ent, STRING power)
static int l_EntityRemovePower (lua_State *L)
{
    ENTITY ent = luaL_checkstring(L, 1);
    STRING power = luaL_checkstring(L, 2);
    EntityRemovePower(ent, power);
    return 0;
}

const luaL_Reg scriptRewardLib [] = 
{
    {"GrantTable",            l_EntityGrantReward},
    {"HasToken",            l_EntityHasRewardToken},
    {"GetToken",            l_EntityGetRewardToken},
    {"GetTokenTime",        l_EntityGetRewardTokenTime},
    {"GrantToken",            l_EntityGrantRewardToken},
    {"RemoveToken",            l_EntityRemoveRewardToken},
    {"HasBadge",            l_EntityHasBadge},
    {"GrantToMissionTeam",    l_MissionGrantReward},
    {"GrantPower",            l_EntityGrantPower},
    {"RemovePower",            l_EntityRemovePower},
    {NULL, NULL}  //sentinel - REQUIRED
};
