#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaWaypoint.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"

//NUMBER CreateWaypoint(LOCATION location, STRING text, STRING icon, STRING iconB, NUMBER compass, NUMBER pulse, FRACTION rotate)
static int l_CreateWaypoint (lua_State *L)
{
    LOCATION location = lua_tostring(L, 1);
    STRING text = lua_tostring(L, 2);
    STRING icon = lua_tostring(L, 3);
    STRING iconB = lua_tostring(L, 4);
    NUMBER compass = lua_toboolean(L, 5) ? 1 : 0;
    NUMBER pulse = lua_toboolean(L, 6) ? 1 : 0;
    FRACTION rotate = luaL_checknumber(L, 7);
    lua_pushnumber(L, CreateWaypoint(location, text, icon, iconB, compass, pulse, rotate));
    return 1;
}

//void DestroyWaypoint(NUMBER id)
static int l_DestroyWaypoint (lua_State *L)
{
    NUMBER id = luaL_checkinteger(L, 1);
    DestroyWaypoint(id);
    return 0;
}

//void SetWaypoint(TEAM team, NUMBER id)
static int l_SetWaypoint (lua_State *L)
{
    TEAM team = luaL_checkstring(L, 1);
    NUMBER id = luaL_checkinteger(L, 2);
    SetWaypoint(team, id);
    return 0;
}

//void ClearWaypoint(TEAM team, NUMBER id)
static int l_ClearWaypoint (lua_State *L)
{
    TEAM team = luaL_checkstring(L, 1);
    NUMBER id = luaL_checkinteger(L, 2);
    ClearWaypoint(team, id);
    return 0;
}

//void ClearAllWaypoints(TEAM team)
static int l_ClearAllWaypoints (lua_State *L)
{
    TEAM team = luaL_checkstring(L, 1);
    ClearAllWaypoints(team);
    return 0;
}

//void UpdateWaypoint(NUMBER id, LOCATION location, STRING text, STRING icon, FRACTION rotate)
static int l_UpdateWaypoint (lua_State *L)
{
    NUMBER id = luaL_checkinteger(L, 1);
    LOCATION location = lua_tostring(L, 2);
    STRING text = lua_tostring(L, 3);
    STRING icon = lua_tostring(L, 4);
    FRACTION rotate = lua_tonumber(L, 5);
    UpdateWaypoint(id, location, text, icon, rotate);
    return 0;
}

const luaL_Reg scriptWaypointLib [] = 
{
    {"Create",                    l_CreateWaypoint},
    {"Destroy",                    l_DestroyWaypoint},
    {"Set",                        l_SetWaypoint},
    {"Clear",                    l_ClearWaypoint},
    {"ClearAll",                l_ClearAllWaypoints},
    {"Update",                    l_UpdateWaypoint},
    {NULL, NULL}  //sentinel - REQUIRED
};
