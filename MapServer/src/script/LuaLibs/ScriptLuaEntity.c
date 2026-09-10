#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaEntity.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"
#include "script/scriptengine.h"

//FRACTION GetHealth( ENTITY ENTITY )
static int l_GetHealth (lua_State *L)
{
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushnumber(L, GetHealth(entity));
    return 1;
}

//FRACTION GetEndurance( ENTITY entity )
static int l_GetEndurance (lua_State *L)
{
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushnumber(L, GetEndurance(entity));
    return 1;
}

//FRACTION GetAbsorb( ENTITY ENTITY )
static int l_GetAbsorb (lua_State *L)
{
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushnumber(L, GetAbsorb(entity));
    return 1;
}

//FRACTION SetHealth( ENTITY entity, FRACTION percentHealth, bool unAttackedOnly )
static int l_SetHealth (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    FRACTION percentHealth = luaL_checknumber(L, 2);
    bool unAttackedOnly = lua_toboolean(L, 3);
    lua_pushnumber(L, SetHealth(entity, percentHealth, unAttackedOnly));
    return 1;
}

//void Say(ENTITY entity, STRING text, int priorityLevel )
static int l_Say (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    STRING text = luaL_checkstring(L, 2);
    int priorityLevel = luaL_checkint(L, 3);
    Say(entity, text, priorityLevel);
    return 0;
}

//int IsEntityCritter(ENTITY entity)
static int l_IsEntityCritter (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityCritter(entity));
    return 1;
}

//int IsEntityPlayer(ENTITY entity)
static int l_IsEntityPlayer (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityPlayer(entity));
    return 1;
}

//int IsEntityOnScriptTeam(ENTITY entity, TEAM team)
static int l_IsEntityOnScriptTeam (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    TEAM team = luaL_checkstring(L, 2);
    lua_pushboolean(L, IsEntityOnScriptTeam(entity, team));
    return 1;
}

//int IsEntityPhased(ENTITY entity)
static int l_IsEntityPhased (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityPhased(entity));
    return 1;
}

//int IsEntityActive(ENTITY entity)
static int l_IsEntityActive (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityActive(entity));
    return 1;
}

//STRING GetVillainDefName(ENTITY entity)
static int l_GetVillainDefName (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushstring(L, GetVillainDefName(entity));
    return 1;
}

//NUMBER GetVillainGroupID(ENTITY entity)
static int l_GetVillainGroupID (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushnumber(L, GetVillainGroupID(entity));
    return 1;
}

//NUMBER GetVillainGroupIDFromName(STRING name)
static int l_GetVillainGroupIDFromName (lua_State *L) {
    STRING name = luaL_checkstring(L, 1);
    lua_pushnumber(L, GetVillainGroupIDFromName(name));
    return 1;
}

//ENTITY GetVillainByDefName( STRING defName, ENTITY cachedEntity )
static int l_GetVillainByDefName (lua_State *L) {
    STRING defName = luaL_checkstring(L, 1);
    ENTITY cachedEntity = lua_tostring(L, 2);
    lua_pushstring(L, GetVillainByDefName(defName, cachedEntity));
    return 1;
}

//void ScriptPlayFXOnEntity(ENTITY entity, STRING fxName)
static int l_ScriptPlayFXOnEntity (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    STRING fxName = luaL_checkstring(L, 2);
    ScriptPlayFXOnEntity(entity, fxName);
    return 0;
}

//void OverridePrisonGurneyWithTarget( ENTITY player, bool overrideFlag, STRING spawnTarget )
static int l_OverridePrisonGurneyWithTarget (lua_State *L) {
    ENTITY player = luaL_checkstring(L, 1);
    NUMBER overrideFlag = lua_toboolean(L, 2);
    STRING spawnTarget = lua_tostring(L, 3);
    OverridePrisonGurneyWithTarget(player, overrideFlag, spawnTarget);
    return 0;
}

static int l_MyEntity (lua_State *L)
{
    lua_pushstring(L, MyEntity());
    return 1;
}

//STRING GetDisplayName( ENTITY entity )
static int l_GetDisplayName (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushstring(L, GetDisplayName(entity));
    return 1;
}

//NUMBER GetLevel( ENTITY entity )
static int l_GetLevel (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushnumber(L, GetLevel(entity));
    return 1;
}

//NUMBER GetCombatLevel( ENTITY entity )
static int l_GetCombatLevel (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushnumber(L, GetCombatLevel(entity));
    return 1;
}

//NUMBER GetExperienceLevel( ENTITY entity )
static int l_GetExperienceLevel (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushnumber(L, GetExperienceLevel(entity));
    return 1;
}

//bool CanLevel( ENTITY entity )
static int l_CanLevel (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, CanLevel(entity));
    return 1;
}

//NUMBER GetAccessLevel( ENTITY entity )
static int l_GetAccessLevel (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushnumber(L, GetAccessLevel(entity));
    return 1;
}

//int HasClue(ENTITY entity, STRING clueName)
static int l_HasClue (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    STRING clueName = luaL_checkstring(L, 2);
    lua_pushboolean(L, HasClue(entity, clueName));
    return 1;
}

//int HasSouvenirClue(ENTITY entity, STRING clueName)
static int l_HasSouvenirClue (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    STRING clueName = luaL_checkstring(L, 2);
    lua_pushboolean(L, HasSouvenirClue(entity, clueName));
    return 1;
}

//int IsEntityOnTaskforce(ENTITY entity)
static int l_IsEntityOnTaskforce (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityOnTaskforce(entity));
    return 1;
}

//int IsEntityOnFlashback(ENTITY entity)
static int l_IsEntityOnFlashback (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityOnFlashback(entity));
    return 1;
}

//int IsEntityOnArchitect(ENTITY entity)
static int l_IsEntityOnArchitect (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityOnArchitect(entity));
    return 1;
}

//NUMBER GetLevelingPact(ENTITY entity)
static int l_GetLevelingPact (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushnumber(L, GetLevelingPact(entity));
    return 1;
}

//STRING GetOrigin(ENTITY entity)
static int l_GetOrigin (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushstring(L, GetOrigin(entity));
    return 1;
}

//STRING GetClass(ENTITY entity)
static int l_GetClass (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushstring(L, GetClass(entity));
    return 1;
}

//STRING GetGender(ENTITY entity)
static int l_GetGender (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushstring(L, GetGender(entity));
    return 1;
}

//STRING GetPlayerAlignmentString(ENTITY entity);
static int l_GetPlayerAlignmentString (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushstring(L, GetPlayerAlignmentString(entity));
    return 1;
}

//STRING GetPlayerPraetorianProgressString(ENTITY entity);
static int l_GetPlayerPraetorianProgressString (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushstring(L, GetPlayerPraetorianProgressString(entity));
    return 1;
}

//STRING GetAllyString(ENTITY entity);
static int l_GetAllyString (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushstring(L, GetAllyString(entity));
    return 1;
}

//int IsEntityHero(ENTITY entity)
static int l_IsEntityHero (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityHero(entity));
    return 1;
}

//int IsEntityOnBlueSide(ENTITY entity)
static int l_IsEntityOnBlueSide (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityOnBlueSide(entity));
    return 1;
}

//int IsEntityVillain(ENTITY entity)
static int l_IsEntityVillain (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityVillain(entity));
    return 1;
}

//int IsEntityOnRedSide(ENTITY entity)
static int l_IsEntityOnRedSide (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityOnRedSide(entity));
    return 1;
}

//int IsEntityPrimal(ENTITY entity)
static int l_IsEntityPrimal (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityPrimal(entity));
    return 1;
}

//int IsEntityPraetorian(ENTITY entity)
static int l_IsEntityPraetorian (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityPraetorian(entity));
    return 1;
}

//int IsEntityPraetorianTutorial(ENTITY entity)
static int l_IsEntityPraetorianTutorial (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityPraetorianTutorial(entity));
    return 1;
}

//int IsEntityMonster(ENTITY entity)
static int l_IsEntityMonster (lua_State *L) {
    ENTITY entity = luaL_checkstring(L, 1);
    lua_pushboolean(L, IsEntityMonster(entity));
    return 1;
}

//NUMBER GetAllEntities( ENTITY    * entList, 
//                    STRING    villainGroup,    
//                    STRING    villainDef,
//                    STRING    villainHasTag,
//                    STRING    entType,        //Not yet implemented
//                    STRING  scriptTeam,    
//                    FRACTION radius, 
//                    LOCATION center,
//                    AREA    area,
//                    NUMBER  isAlive,
//                    NUMBER  isDead,
//                    NUMBER  max,
//                    NUMBER  onceOnly)
#define LUA_GETALLENTTITIES_MAX 100
static int l_GetAllEntities (lua_State *L) {
    STRING villainGroup = lua_tostring(L, 1);
    STRING villainDef = lua_tostring(L, 2);
    STRING villainHasTag = lua_tostring(L, 3);
    STRING entType = lua_tostring(L, 4);
    STRING scriptTeam = lua_tostring(L, 5);
    FRACTION radius = lua_tonumber(L, 6);
    LOCATION center = lua_tostring(L, 7);
    AREA area = lua_tostring(L, 8);
    NUMBER isAlive = lua_toboolean(L, 9);
    NUMBER isDead = lua_toboolean(L, 10);
    NUMBER max = luaL_checkint(L, 11);
    NUMBER onceOnly = lua_toboolean(L, 12);

    if(max < 0 || max > LUA_GETALLENTTITIES_MAX)
    {
        ScriptError("Invalid size for Entity.GetAllEntities");
        lua_pushnil(L);
        return 1;
    }
    else
    {
        ENTITY entlist[LUA_GETALLENTTITIES_MAX];
        int i = 0;
        int count = GetAllEntities(entlist, villainGroup, villainDef, villainHasTag, entType, scriptTeam, radius, center, area,
            isAlive, isDead, max, onceOnly);
        
        lua_newtable(L);
        for (i = 0; i < count; i++)
        {
            lua_pushnumber(L, i+1);
            lua_pushstring(L, entlist[i]);
            lua_settable(L, -3);
        }
    }
    return 1;
}

//ENTITY CreateDoppelganger(TEAM teamName, STRING doppleFlags, NUMBER level, STRING name, LOCATION location, ENTITY original, STRING customCritterDef,
//                          STRING villainRank, STRING displayGroup, bool bossReduction)
static int l_CreateDoppelganger (lua_State *L) {
    TEAM teamName = lua_tostring(L, 1);
    STRING doppleFlags = lua_tostring(L, 2);
    NUMBER level = luaL_checkint(L, 3);
    STRING name = lua_tostring(L, 4);
    LOCATION location = luaL_checkstring(L, 5);
    ENTITY original = luaL_checkstring(L, 6);
    STRING customCritterDef = lua_tostring(L, 7);
    STRING villainRank = lua_tostring(L, 8);
    STRING displayGroup = lua_tostring(L, 9);
    bool bossReduction = lua_toboolean(L, 10);

    lua_pushstring(L, CreateDoppelganger(teamName, doppleFlags, level, name, location, original, customCritterDef, villainRank, displayGroup, bossReduction));
    return 1;
}

const luaL_Reg scriptEntityLib [] = 
{
    {"GetHealth",                l_GetHealth},
    {"GetEndurance",            l_GetEndurance},
    {"GetAbsorb",                l_GetAbsorb},
    {"SetHealth",                l_SetHealth},
    {"Say",                        l_Say},
    {"IsCritter",                l_IsEntityCritter},
    {"IsPlayer",                l_IsEntityPlayer},
    {"IsOnScriptTeam",            l_IsEntityOnScriptTeam},
    {"IsPhased",                l_IsEntityPhased},
    {"IsActive",                l_IsEntityActive},
    {"IsHero",                    l_IsEntityHero},
    {"IsOnBlueSide",            l_IsEntityOnBlueSide},
    {"IsVillain",                l_IsEntityVillain},
    {"IsOnRedSide",                l_IsEntityOnRedSide},
    {"IsPrimal",                l_IsEntityPrimal},
    {"IsPraetorian",            l_IsEntityPraetorian},
    {"IsPraetorianTutorial",    l_IsEntityPraetorianTutorial},
    {"IsMonster",                l_IsEntityMonster},
    {"OnTaskforce",                l_IsEntityOnTaskforce},
    {"OnFlashback",                l_IsEntityOnFlashback},
    {"OnArchitect",                l_IsEntityOnArchitect},
    {"GetVillainDefName",        l_GetVillainDefName},
    {"GetVillainGroupID",        l_GetVillainGroupID},
    {"GetVillainGroupIDFromName",    l_GetVillainGroupIDFromName},
    {"GetVillainByDefName",        l_GetVillainByDefName},
    {"PlayFX",                    l_ScriptPlayFXOnEntity},
    {"OverrideGurney",            l_OverridePrisonGurneyWithTarget},
    {"MyEntity",                l_MyEntity},
    {"GetDisplayName",            l_GetDisplayName},
    {"GetLevel",                l_GetLevel},
    {"GetCombatLevel",            l_GetCombatLevel},
    {"GetExperienceLevel",        l_GetExperienceLevel},
    {"CanLevel",                l_CanLevel},
    {"GetAccessLevel",            l_GetAccessLevel},
    {"HasClue",                    l_HasClue},
    {"HasSouvenirClue",            l_HasSouvenirClue},
    {"GetLevelingPact",            l_GetLevelingPact},
    {"GetOrigin",                l_GetOrigin},
    {"GetClass",                l_GetClass},
    {"GetGender",                l_GetGender},
    {"GetAlignment",            l_GetPlayerAlignmentString},
    {"GetAlly",                    l_GetAllyString},
    {"GetPraetorianProgress",    l_GetPlayerPraetorianProgressString},
    {"GetAllEntities",            l_GetAllEntities},
    {"CreateDoppelganger",        l_CreateDoppelganger},
    {NULL, NULL}  //sentinel - REQUIRED
};
