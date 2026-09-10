#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaGlowie.h"
#include "script/script.h"

//GLOWIEDEF GlowieCreateDef(STRING name, STRING model, STRING InteractBeginString, STRING InteractInterruptedString,
//                                STRING InteractCompleteString, STRING InteractActionString, NUMBER delay);
static int l_GlowieCreateDef (lua_State *L) {
    STRING name = luaL_checkstring(L, 1);
    STRING model = luaL_checkstring(L, 2);
    STRING InteractBeginString = luaL_checkstring(L, 3);
    STRING InteractInterruptedString = luaL_checkstring(L, 4);
    STRING InteractCompleteString = luaL_checkstring(L, 5);
    STRING InteractActionString = luaL_checkstring(L, 6);
    NUMBER delay = luaL_checkint(L, 7);
    GLOWIEDEF glowie;
    GLOWIEDEF* lua_glowie;
    
    glowie = GlowieCreateDef(name, model, InteractBeginString, InteractInterruptedString, InteractCompleteString, InteractActionString, delay);

    lua_glowie = (GLOWIEDEF*)lua_newuserdata(L, sizeof(GLOWIEDEF));
    *lua_glowie = glowie;

    luaL_getmetatable(L, "GLOWIEDEF");
    lua_setmetatable(L, -2);
    return 1;
}

//ENTITY GlowiePlace(GLOWIEDEF glowdef, LOCATION position, NUMBER reusable, PlayerInteractWithNPCOrGlowieFunc click, PlayerInteractWithNPCOrGlowieFunc done)
static int l_GlowiePlace(lua_State *L) {
    GLOWIEDEF* glowdef;
    LOCATION position = luaL_checkstring(L, 2);
    NUMBER reusable = luaL_checkint(L, 3);
    STRING lua_click = lua_tostring(L, 4);
    STRING lua_done = lua_tostring(L, 5);

    luaL_checktype(L, 1, LUA_TUSERDATA);
    glowdef = (GLOWIEDEF*)luaL_checkudata(L, 1, "GLOWIEDEF");

    if(*glowdef)
        lua_pushstring(L, GlowiePlaceEx(*glowdef, position, reusable, NULL, lua_click, NULL, lua_done));
    else
        lua_pushnil(L);

    return 1;
}

//void GlowieRemove(ENTITY glowie);
static int l_GlowieRemove(lua_State *L) {
    ENTITY glowie = luaL_checkstring(L, 1);
    GlowieRemove(glowie);
    return 0;
}

//void GlowieSetState(ENTITY glowie);
static int l_GlowieSetState(lua_State *L) {
    ENTITY glowie = luaL_checkstring(L, 1);
    GlowieSetState(glowie);
    return 0;
}

//void GlowieClearState(ENTITY glowie);
static int l_GlowieClearState(lua_State *L) {
    ENTITY glowie = luaL_checkstring(L, 1);
    GlowieClearState(glowie);
    return 0;
}

//int GlowieGetState(ENTITY glowie);
//void GlowieAddEffect(GLOWIEDEF glowdef, ScriptGlowieEffectType type, STRING fx);
//void GlowieSetDescriptions(GLOWIEDEF glowdef, STRING description, STRING singulardescription);
//void GlowieClearAll(void);

//void GlowieSetCharRequires(GLOWIEDEF glowdef, STRING requiresParam, STRING failText)
static int l_GlowieSetCharRequires(lua_State *L) {
    GLOWIEDEF* glowdef;
    STRING requiresParam = luaL_checkstring(L, 2);
    STRING failText = lua_tostring(L, 3);

    luaL_checktype(L, 1, LUA_TUSERDATA);
    glowdef = (GLOWIEDEF*)luaL_checkudata(L, 1, "GLOWIEDEF");

    if(*glowdef)
        GlowieSetCharRequires(*glowdef, requiresParam, failText);
    return 0;
}

const luaL_Reg scriptGlowieLib [] = 
{
    {"CreateDef",                l_GlowieCreateDef},
    {"Place",                    l_GlowiePlace},
    {"Remove",                    l_GlowieRemove},
    {"SetState",                l_GlowieSetState},
    {"ClearState",                l_GlowieClearState},
    {"SetCharRequires",            l_GlowieSetCharRequires},
    {NULL, NULL}  //sentinel - REQUIRED
};
