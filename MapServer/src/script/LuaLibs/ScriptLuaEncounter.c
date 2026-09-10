#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaEncounter.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"
#include "script/Scripthook/ScriptHookInternal.h"

//ENCOUNTERGROUP MyEncounter()
static int l_MyEncounter (lua_State *L) {
    lua_pushstring(L, MyEncounter());
    return 1;
}

//ENTITY ActorFromEncounterActorName(ENCOUNTERGROUP encounter, STRING actorName )
static int l_ActorFromEncounterActorName (lua_State *L) {
    ENCOUNTERGROUP encounter = luaL_checkstring(L, 1);
    STRING actorName = luaL_checkstring(L, 2);
    lua_pushstring(L, ActorFromEncounterActorName(encounter, actorName));
    return 1;
}

//see static void InitSpawn(ScriptSpawn *spawn) in ScriptedZoneEvent.c
static int l_FindAllMatchingEncounterGroups(lua_State *L) {
    ENCOUNTERGROUPITERATOR iter;
    ENCOUNTERGROUP group;
    int i = 0;

    LOCATION loc = lua_tostring(L, 1);
    FRACTION innerRadius = lua_tonumber(L, 2);
    FRACTION outerRadius = lua_tonumber(L, 3);
    AREA area = lua_tostring(L, 4);
    STRING layout = lua_tostring(L, 5);
    STRING groupname = lua_tostring(L, 6);

    EncounterGroupsInitIterator(&iter, loc, innerRadius, outerRadius, area, layout, groupname);
    
    lua_newtable(L);
    for (;;)
    {
        group = EncounterGroupsFindNext(&iter);
        if (StringEqual(group, LOCATION_NONE))
            break;
        else
        {
            lua_pushnumber(L, i+1);
            lua_pushstring(L, group);
            lua_settable(L, -3);
            i++;
        }
    }

    return 1;
}

//LOCATION Spawn(int num, TEAM teamName, STRING spawnfile, ENCOUNTERGROUP encounter, STRING layout, int level, int size)
static int l_Spawn(lua_State *L)
{
    int num = luaL_checkint(L, 1);
    TEAM teamName = lua_tostring(L, 2);
    STRING spawndef = lua_tostring(L, 3) ? lua_tostring(L, 3) : "UseCanSpawns";
    ENCOUNTERGROUP encounter = luaL_checkstring(L, 4);
    STRING layout = lua_tostring(L, 5);
    int level = lua_tointeger(L, 6);
    int size = lua_tointeger(L, 7);
    
    lua_pushstring(L, Spawn(num, teamName, spawndef, encounter, layout, level, size));

    return 1;
}

//void DetachSpawn(TEAM team)
static int l_DetachSpawn (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    DetachSpawn(team);
    return 0;
}

//void StartCutSceneFromString(STRING cutSceneString, ENCOUNTERGROUP encounter)
static int l_StartCutSceneFromString (lua_State *L) {
    STRING cutSceneString = luaL_checkstring(L, 1);
    ENCOUNTERGROUP encounter = lua_tostring(L, 2);
    StartCutSceneFromString(cutSceneString, FindEncounterGroupInternal(encounter, NULL, 0, 0));
    return 0;
}

const luaL_Reg scriptEncounterLib [] = 
{
    {"MyEncounter",                l_MyEncounter},
    {"ActorFromEncounterActorName",        l_ActorFromEncounterActorName},
    {"FindAllMatchingEncounterGroups",    l_FindAllMatchingEncounterGroups},
    {"Spawn",                    l_Spawn},
    {"DetachSpawn",                l_DetachSpawn},
    {"StartCutSceneFromString",    l_StartCutSceneFromString},
    {NULL, NULL}  //sentinel - REQUIRED
};
