#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaVar.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"
#include <utilitieslib/utils/SuperAssert.h>

//STRING VarGet(VARIABLE var)
static int l_VarGet (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    lua_pushstring(L, VarGet(var));
    return 1;
}

//void VarSet(VARIABLE var, STRING value)
static int l_VarSet (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    STRING value = lua_tostring(L, 2);
    VarSet(var, value);
    return 0;
}

//void VarSetHidden(VARIABLE var, STRING value)
static int l_VarSetHidden (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    STRING value = lua_tostring(L, 2);
    VarSetHidden(var, value);
    return 0;
}

//NUMBER VarGetNumber(VARIABLE var)
//FRACTION VarGetFraction(VARIABLE var)
static int l_VarGetNumber (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    lua_pushnumber(L, VarGetFraction(var));
    return 1;
}

//void VarSetNumber(VARIABLE var, NUMBER value)
static int l_VarSetNumber (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    NUMBER value = luaL_checkint(L, 2);
    VarSetNumber(var, value);
    return 0;
}

//void VarSetNumberHidden(VARIABLE var, NUMBER value)
static int l_VarSetNumberHidden (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    NUMBER value = luaL_checkint(L, 2);
    VarSetNumberHidden(var, value);
    return 0;
}

//void VarSetFraction(VARIABLE var, FRACTION value)
static int l_VarSetFraction (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    FRACTION value = luaL_checknumber(L, 2);
    VarSetFraction(var, value);
    return 0;
}

//void VarSetFractionHidden(VARIABLE var, FRACTION value)
static int l_VarSetFractionHidden (lua_State *L) {
    VARIABLE var= luaL_checkstring(L, 1);
    FRACTION value = luaL_checknumber(L, 2);
    VarSetFractionHidden(var, value);
    return 0;
}

//int VarIsTrue(VARIABLE var)
static int l_VarIsTrue (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    lua_pushboolean(L, VarIsTrue(var));
    return 1;
}

//NUMBER VarGetArrayCount(VARIABLE var)
static int l_VarGetArrayCount (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    lua_pushnumber(L, VarGetArrayCount(var));
    return 1;
}

//STRING VarGetArrayElement(VARIABLE var, NUMBER index)
static int l_VarGetArrayElement (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    NUMBER index = luaL_checkint(L, 2) - 1; //Lua convention starts with 1
    devassertmsg(index >= 0, "Lua indices begin with 1.");
    lua_pushstring(L, VarGetArrayElement(var, index));
    return 1;
}

//FRACTION VarGetArrayElementFraction(VARIABLE var, NUMBER index)
static int l_VarGetArrayElementFraction (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    NUMBER index = luaL_checkint(L, 2) - 1; //Lua convention starts with 1
    devassertmsg(index >= 0, "Lua indices begin with 1.");
    lua_pushnumber(L, VarGetArrayElementFraction(var, index));
    return 1;
}

//mSTRING EntVar(ENTITY entName, STRING var)
static int l_EntVar (lua_State *L) {
    ENTITY entName = luaL_checkstring(L, 1);
    STRING var = luaL_checkstring(L, 2);
    lua_pushstring(L, EntVar(entName, var));
    return 1;
}

//STRING InstanceVarGet(VARIABLE var)
static int l_InstanceVarGet (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    lua_pushstring(L, InstanceVarGet(var));
    return 1;
}

//void InstanceVarSet(VARIABLE var, STRING value)
static int l_InstanceVarSet (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    STRING value = lua_tostring(L, 2);
    InstanceVarSet(var, value);
    return 0;
}

//void InstanceVarRemove(VARIABLE var)
static int l_InstanceVarRemove (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    InstanceVarRemove(var);
    return 0;
}

//int InstanceVarIsEmpty(VARIABLE var)
static int l_InstanceVarIsEmpty (lua_State *L) {
    VARIABLE var = luaL_checkstring(L, 1);
    lua_pushboolean(L, InstanceVarIsEmpty(var));
    return 1;
}

//int MapGetDataToken(STRING token)
static int l_MapGetDataToken (lua_State *L) {
    STRING token = luaL_checkstring(L, 1);
    lua_pushnumber(L, MapGetDataToken(token));
    return 1;
}

//void MapSetDataToken(STRING token, NUMBER value)
static int l_MapSetDataToken (lua_State *L) {
    STRING token = luaL_checkstring(L, 1);
    NUMBER value = luaL_checkinteger(L, 2);
    MapSetDataToken(token, value);
    return 0;
}

//void MapRemoveDataToken(STRING token)
static int l_MapRemoveDataToken (lua_State *L) {
    STRING token = luaL_checkstring(L, 1);
    MapRemoveDataToken(token);
    return 0;
}

//MapTokenIsEmpty is inverse of
//int MapHasDataToken(STRING token)
//for standardization's sake
static int l_MapTokenIsEmpty (lua_State *L) {
    STRING token = luaL_checkstring(L, 1);
    lua_pushboolean(L, !MapHasDataToken(token));
    return 1;
}

const luaL_Reg scriptVarLib [] = 
{
    {"Get",                    l_VarGet},
    {"Set",                    l_VarSet},
    {"SetHidden",            l_VarSetHidden},
    {"GetNumber",            l_VarGetNumber},
    {"SetNumber",            l_VarSetNumber},
    {"SetNumberHidden",        l_VarSetNumberHidden},
    {"GetFraction",            l_VarGetNumber},
    {"SetFraction",            l_VarSetFraction},
    {"SetFractionHidden",    l_VarSetFractionHidden},
    {"IsTrue",                l_VarIsTrue},
    {"ArrayCount",            l_VarGetArrayCount},
    {"ArrayGet",            l_VarGetArrayElement},
    {"ArrayGetNumber",        l_VarGetArrayElementFraction},
    {"ArrayGetFraction",    l_VarGetArrayElementFraction},
    {"EntVar",                l_EntVar},
    {"InstanceGet",            l_InstanceVarGet},
    {"InstanceSet",            l_InstanceVarSet},
    {"InstanceRemove",        l_InstanceVarRemove},
    {"InstanceIsEmpty",        l_InstanceVarIsEmpty},
    {"MapTokenGet",            l_MapGetDataToken},
    {"MapTokenSet",            l_MapSetDataToken},
    {"MapTokenRemove",        l_MapRemoveDataToken},
    {"MapTokenIsEmpty",        l_MapTokenIsEmpty},
    {NULL, NULL}  //sentinel - REQUIRED
};
