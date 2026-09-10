#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaKarma.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"
#include "script/ZoneEvents/ScriptedZoneEventKarma.h"

//void ScriptSetKarmaContainer(ENTITY player);
static int l_ScriptSetKarmaContainer (lua_State *L)
{
    ENTITY player = luaL_checkstring(L, 1);
    ScriptSetKarmaContainer(player);
    return 0;
}

//void ScriptClearKarmaContainer(ENTITY player);
static int l_ScriptClearKarmaContainer (lua_State *L)
{
    ENTITY player = luaL_checkstring(L, 1);
    ScriptClearKarmaContainer(player);
    return 0;
}

//void ScriptGlowieBubbleActivate(ENTITY activator, ENTITY player);
static int l_ScriptGlowieBubbleActivate (lua_State *L)
{
    ENTITY activator = luaL_checkstring(L, 1);
    ENTITY player = luaL_checkstring(L, 2);
    ScriptGlowieBubbleActivate(activator, player);
    return 0;
}

//void ScriptEnableKarma();
static int l_ScriptEnableKarma (lua_State *L)
{
    ScriptEnableKarma();
    return 0;
}

//void ScriptDisableKarma();
static int l_ScriptDisableKarma (lua_State *L)
{
    ScriptDisableKarma();
    return 0;
}

//bool IsKarmaEnabled();
static int l_IsKarmaEnabled (lua_State *L)
{
    lua_pushboolean(L, IsKarmaEnabled());
    return 1;
}

//void ScriptKarmaAdvanceStage();
static int l_ScriptKarmaAdvanceStage (lua_State *L)
{
    ScriptKarmaAdvanceStage();
    return 0;
}

//void ScriptUpdateKarma(bool pause);
static int l_ScriptUpdateKarma (lua_State *L)
{
    bool pause = lua_toboolean(L, 1);
    ScriptUpdateKarma(pause);
    return 0;
}

//void ScriptAddObjectiveKarma(ENTITY player, NUMBER teamKarmaValue, NUMBER teamKarmaLife, NUMBER allKarmaValue, NUMBER allKarmaLife);
static int l_ScriptAddObjectiveKarma (lua_State *L)
{
    ENTITY player = luaL_checkstring(L, 1);
    NUMBER teamKarmaValue = (int) luaL_checknumber(L, 2);
    NUMBER teamKarmaLife = (int) luaL_checknumber(L, 3);
    NUMBER allKarmaValue = (int) luaL_checknumber(L, 4);
    NUMBER allKarmaLife = (int) luaL_checknumber(L, 5);
    ScriptAddObjectiveKarma(player, teamKarmaValue, teamKarmaLife, allKarmaValue, allKarmaLife);
    return 0;
}

static int l_HandleKarmaRewards (lua_State *L)
{
    int i;
    STRING scriptName = lua_tostring(L, 1);
    bool minorStageReward = lua_toboolean(L, 2);
    int numStageThresholds = luaL_checknumber(L, 3);
    int numRewards = luaL_checknumber(L, 5);
    int *stageThreshs = NULL;
    ScriptReward **rewards = NULL;

    for(i = 0; i < numStageThresholds; i++)
    {
        int stageThreshold;
        lua_pushnumber(L, i+1);
        lua_gettable(L, -4);
        stageThreshold = (int) lua_tonumber(L, -1);
        eaiPush(&stageThreshs, stageThreshold);
        lua_pop(L, 1);
    }

    for(i = 0; i < numRewards; i++)
    {
        ScriptReward *newReward = (ScriptReward*)_alloca(sizeof(ScriptReward));
        memset(newReward, 0, sizeof(ScriptReward));

        lua_pushnumber(L, i+1);
        lua_gettable(L, -2);

        lua_pushstring(L, "pointsNeeded");
        lua_gettable(L, -2);
        newReward->pointsNeeded = (int) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "percentileNeeded");
        lua_gettable(L, -2);
        newReward->percentileNeeded = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "numStagesRequired");
        lua_gettable(L, -2);
        newReward->numStagesRequired = (int) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "rewardTable");
        lua_gettable(L, -2);
        newReward->rewardTable = lua_tostring(L, -1);
        lua_pop(L, 1);

        eaPush(&rewards, newReward);
        lua_pop(L, 1);
    }

    HandleRewardGroup(rewards, stageThreshs, minorStageReward, scriptName);

    eaiDestroy(&stageThreshs);

    return 0;
}

//int ScriptKarmaGetActivePlayers(int scriptId)
static int l_ScriptKarmaGetActivePlayers (lua_State *L)
{
    NUMBER scriptId = luaL_checkinteger(L, 1);
    lua_pushnumber(L, ScriptKarmaGetActivePlayers(scriptId));
    return 1;
}

//void ScriptResetKarma()
static int l_ScriptResetKarma (lua_State *L)
{
    ScriptResetKarma();
    return 0;
}

const luaL_Reg scriptKarmaLib [] = 
{
    {"SetContainer",            l_ScriptSetKarmaContainer},
    {"ClearContainer",            l_ScriptClearKarmaContainer},
    {"GlowieBubbleActivate",    l_ScriptGlowieBubbleActivate},
    {"Enable",                    l_ScriptEnableKarma},
    {"Disable",                    l_ScriptDisableKarma},
    {"IsEnabled",                l_IsKarmaEnabled},
    {"AdvanceStage",            l_ScriptKarmaAdvanceStage},
    {"Update",                    l_ScriptUpdateKarma},
    {"Reset",                    l_ScriptResetKarma},
    {"AddObjective",            l_ScriptAddObjectiveKarma},
    {"HandleRewards",            l_HandleKarmaRewards},
    {"GetActivePlayers",        l_ScriptKarmaGetActivePlayers},
    {NULL, NULL}  //sentinel - REQUIRED
};
