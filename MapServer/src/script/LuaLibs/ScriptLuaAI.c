#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaAI.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"

//void AddAIBehavior(TEAM team, STRING behaviorstring)
static int l_AddAIBehavior (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    STRING behaviorstring = luaL_checkstring(L, 2);
    AddAIBehavior(team, behaviorstring);
    return 0;
}

//void SetFollowersEx(ENTITY leader, TEAM newFollowerTeam, NUMBER addDefaultFollowBehavior)
static int l_SetFollowers (lua_State *L) {
    ENTITY leader = luaL_checkstring(L, 1);
    TEAM newFollowerTeam = luaL_checkstring(L, 2);
    SetFollowersEx(leader, newFollowerTeam, false);
    return 0;
}

//ENTITY GetTrueOwner( ENTITY petName )
static int l_GetTrueOwner (lua_State *L) {
    ENTITY petName = luaL_checkstring(L, 1);
    lua_pushstring(L, GetTrueOwner(petName));
    return 1;
}

//ENTITY GetOwner( ENTITY petName )
static int l_GetOwner (lua_State *L) {
    ENTITY petName = luaL_checkstring(L, 1);
    lua_pushstring(L, GetOwner(petName));
    return 1;
}

//void SetAIAttackTarget(TEAM attackTeam, TEAM targetTeam, PRIORITY priority)
static int l_SetAttackTarget (lua_State *L) {
    TEAM attackTeam = luaL_checkstring(L, 1);
    TEAM targetTeam = luaL_checkstring(L, 2);
    PRIORITY priority = luaL_checkint(L, 3);
    SetAIAttackTarget(attackTeam, targetTeam, priority);
    return 0;
}

//NUMBER CurAIBehaviorMatchesName(ENTITY entity, STRING behaviorname)
static int l_CurAIBehaviorMatchesName (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    STRING behaviorname = luaL_checkstring(L, 2);
    lua_pushboolean(L, CurAIBehaviorMatchesName(entity, behaviorname));
    return 1;
}

const luaL_Reg scriptAILib [] = 
{
    {"AddBehavior",                l_AddAIBehavior},
    {"SetFollowers",            l_SetFollowers},
    {"GetOwner",                l_GetTrueOwner},
    {"GetCreator",                l_GetOwner},
    {"SetAttackTarget",            l_SetAttackTarget},
    {"CurBehaviorIs",            l_CurAIBehaviorMatchesName},
    {NULL, NULL}  //sentinel - REQUIRED
};
