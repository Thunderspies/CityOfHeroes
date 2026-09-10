#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaMission.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"

// Mission Library Defintions

//void SetMissionObjectiveComplete( int status, const char * ObjectiveName )
static int l_SetMissionObjectiveComplete (lua_State *L) {
    int status = luaL_checkint(L, 1);
    const char *ObjectiveName = luaL_checkstring(L, 2);
    SetMissionObjectiveComplete(status, ObjectiveName);
    return 0;
}

//int ObjectiveIsComplete(STRING objective)
static int l_ObjectiveIsComplete (lua_State *L) {
    STRING objective = luaL_checkstring(L, 1);
    lua_pushboolean(L, ObjectiveIsComplete(objective));
    return 1;
}

//ENTITY GetOwner(void)
static int l_GetOwner (lua_State *L)
{
    lua_pushstring(L, MissionOwner());
    return 1;
}

// Door Library Definitions

//void OpenDoor(  LOCATION doorName )
static int l_OpenDoor (lua_State *L)
{
    LOCATION doorName = luaL_checkstring(L, 1);
    OpenDoor(doorName);
    return 0;
}

//void CloseDoor(  LOCATION doorName )
static int l_CloseDoor (lua_State *L)
{
    LOCATION doorName = luaL_checkstring(L, 1);
    CloseDoor(doorName);
    return 0;
}

//void LockDoor(  LOCATION doorName, STRING msg )
static int l_LockDoor (lua_State *L)
{
    LOCATION doorName = luaL_checkstring(L, 1);
    STRING msg = lua_tostring(L, 2);
    LockDoor(doorName, msg);
    return 0;
}

//void UnlockDoor(  LOCATION doorName )
static int l_UnlockDoor (lua_State *L)
{
    LOCATION doorName = luaL_checkstring(L, 1);
    UnlockDoor(doorName);
    return 0;
}

const luaL_Reg scriptMissionLib [] = 
{
    {"SetObjectiveComplete",                l_SetMissionObjectiveComplete},
    {"ObjectiveIsSucceeded",                l_ObjectiveIsComplete},
    {"GetOwner",                            l_GetOwner},
    {NULL, NULL}  //sentinel - REQUIRED
};

const luaL_Reg scriptDoorLib [] = 
{
    {"Lock",                    l_LockDoor},
    {"Unlock",                    l_UnlockDoor},
    {"Open",                    l_OpenDoor},
    {"Close",                    l_CloseDoor},
    {NULL, NULL}  //sentinel - REQUIRED
};
