#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaCommon.h"
#include <utilitieslib/components/earray.h>

void PushStringArray(lua_State *L, STRING *strings, int num)
{
    int i;
    lua_newtable(L);

    for(i = 0; i < num; i++)
    {
        if(strings[i])
        {
            lua_pushnumber(L, i+1);
            lua_pushstring(L, strings[i]);
            lua_settable(L, -3);
        }
    }
}

STRING *GetStringArray(lua_State *L, int num)
{
    int i;
    STRING *strings = NULL;

    for(i = 0; i < num; i++)
    {
        STRING newString;
        lua_pushnumber(L, i+1);
        lua_gettable(L, -2);
        newString = luaL_checkstring(L, -1);
        eaPushConst(&strings, newString);
        lua_pop(L, 1);
    }

    return strings;
}

