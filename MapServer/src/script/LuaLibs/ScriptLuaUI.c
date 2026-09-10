#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaUI.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"

//int ScriptUICollectionExists(COLLECTION collection)
static int l_ScriptUICollectionExists (lua_State *L)
{
    COLLECTION collection = luaL_checkstring(L, 1);
    lua_pushboolean(L, ScriptUICollectionExists(collection));
    return 1;
}

//void ScriptUITitle(WIDGET widgetName, STRING titleVar, STRING infoText)
static int l_ScriptUITitle (lua_State *L)
{
    WIDGET widgetName = luaL_checkstring(L, 1);
    STRING titleVar = lua_tostring(L, 2);
    STRING infoText = lua_tostring(L, 3);
    ScriptUITitle(widgetName, titleVar, infoText);
    return 0;
}

//void ScriptUIList(WIDGET widgetName, STRING tooltip, STRING item, STRING itemColor, STRING value, STRING valueColor)
static int l_ScriptUIList (lua_State *L)
{
    WIDGET widgetName = luaL_checkstring(L, 1);
    STRING tooltip = lua_tostring(L, 2);
    STRING item = lua_tostring(L, 3);
    STRING itemColor = lua_tostring(L, 4);
    STRING value = lua_tostring(L, 5);
    STRING valueColor = lua_tostring(L, 6);
    ScriptUIList(widgetName, tooltip, item, itemColor, value, valueColor);
    return 0;
}

//void ScriptUIMeterEx(WIDGET widgetName, STRING meterName, STRING shortText, STRING current, STRING target, STRING color, STRING tooltip, ENTITY entID)
static int l_ScriptUIMeterEx (lua_State *L)
{
    WIDGET widgetName = luaL_checkstring(L, 1);
    STRING meterName = lua_tostring(L, 2);
    STRING shortText = lua_tostring(L, 3);
    STRING current = lua_tostring(L, 4);
    STRING target = lua_tostring(L, 5);
    STRING color = lua_tostring(L, 6);
    STRING tooltip = lua_tostring(L, 7);
    ENTITY entID = lua_tostring(L, 8);
    ScriptUIMeterEx(widgetName, meterName, shortText, current, target, color, tooltip, entID);
    return 0;
}

//void ScriptUIText(WIDGET widgetName, STRING text, STRING format, STRING tooltip)
static int l_ScriptUIText (lua_State *L)
{
    WIDGET widgetName = luaL_checkstring(L, 1);
    STRING text = lua_tostring(L, 2);
    STRING format = lua_tostring(L, 3);
    STRING tooltip = lua_tostring(L, 4);
    ScriptUIText(widgetName, text, format, tooltip);
    return 0;
}

//void ScriptUITimer(WIDGET widgetName, STRING timerVar, STRING timerText, STRING tooltip)
static int l_ScriptUITimer (lua_State *L)
{
    WIDGET widgetName = luaL_checkstring(L, 1);
    STRING timerVar = luaL_checkstring(L, 2);
    STRING timerText = lua_tostring(L, 3);
    STRING tooltip = lua_tostring(L, 4);
    ScriptUITimer(widgetName, timerVar, timerText, tooltip);
    return 0;
}

//void ScriptUICreateCollectionEx(COLLECTION collection, int nParams, const char **params);
static int l_ScriptUICreateCollectionEx (lua_State *L)
{
    COLLECTION collection = luaL_checkstring(L, 1);
    int nParams = luaL_checknumber(L, 2);
    STRING *params = GetStringArray(L, nParams);

    ScriptUICreateCollectionEx(collection, nParams, params);
    if(params) eaDestroyConst(&params);
    return 0;
}

//void ScriptUIShow(COLLECTION collection, TEAM player)
static int l_ScriptUIShow (lua_State *L)
{
    COLLECTION collection = luaL_checkstring(L, 1);
    TEAM player = luaL_checkstring(L, 2);
    ScriptUIShow(collection, player);
    return 0;
}

//void ScriptUIHide(COLLECTION collection, TEAM player)
static int l_ScriptUIHide (lua_State *L)
{
    COLLECTION collection = luaL_checkstring(L, 1);
    TEAM player = luaL_checkstring(L, 2);
    ScriptUIHide(collection, player);
    return 0;
}

//void ScriptUISendDetachCommand(ENTITY player, int detach)
static int l_ScriptUISendDetachCommand (lua_State *L)
{
    ENTITY player = luaL_checkstring(L, 1);
    int detach = luaL_checkint(L, 2);
    ScriptUISendDetachCommand(player, detach);
    return 0;
}

//void ScriptUIWidgetHidden(COLLECTION collection, int hiddenStatus)
static int l_ScriptUIWidgetHidden (lua_State *L)
{
    COLLECTION collection = luaL_checkstring(L, 1);
    int hiddenStatus = luaL_checkint(L, 2);
    ScriptUIWidgetHidden(collection, hiddenStatus);
    return 0;
}

//void SetUpEntityTrackingForUI(VARIABLE findVar, ENTITY target)
static int l_SetUpEntityTrackingForUI (lua_State *L)
{
    VARIABLE findVar = luaL_checkstring(L, 1);
    ENTITY target = luaL_checkstring(L, 2);
    SetUpEntityTrackingForUI(findVar, target);
    return 0;
}

const luaL_Reg scriptUILib [] = 
{
    {"CollectionExists",    l_ScriptUICollectionExists},
    {"Title",                l_ScriptUITitle},
    {"List",                l_ScriptUIList},
    {"MeterEx",                l_ScriptUIMeterEx},
    {"Text",                l_ScriptUIText},
    {"Timer",                l_ScriptUITimer},
    {"CreateCollectionEx",    l_ScriptUICreateCollectionEx},
    {"Show",                l_ScriptUIShow},
    {"Hide",                l_ScriptUIHide},
    {"SendDetachCommand",    l_ScriptUISendDetachCommand},
    {"WidgetHidden",        l_ScriptUIWidgetHidden},
    {"SetUpEntityTracking",    l_SetUpEntityTrackingForUI},
    {NULL, NULL}  //sentinel - REQUIRED
};
