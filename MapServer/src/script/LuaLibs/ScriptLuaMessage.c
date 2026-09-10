#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ScriptLuaMessage.h"
#include "script/script.h"
#include "ScriptLuaCommon.h"
#include "gameComm/svr_chat.h"

//void chatSendDebug( const char * msg )
static int l_DebugPrint (lua_State *L) {
    STRING msg = luaL_checkstring(L, 1);
    chatSendDebug(msg);
    return 0;
}

//void TeamSZERewardLogMsg(TEAM team, STRING eventName, STRING message)
static int l_TeamSZERewardLogMsg (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    STRING eventName = luaL_checkstring(L, 2);
    STRING message = luaL_checkstring(L, 3);
    TeamSZERewardLogMsg(team, eventName, message);
    return 0;
}

StaticDefineInt ParseAlertTypes[] =
{
    DEFINE_INT
    { "Attention",            kFloaterStyle_Attention},
    { "PriorityAlert",        kFloaterStyle_PriorityAlert},
    { "Damage",                kFloaterStyle_Damage},
    { "Chat",                kFloaterStyle_Chat},
    { "PrivateChat",        kFloaterStyle_Chat_Private},
    { "Icon",                kFloaterStyle_Icon},
    { "Emote",                kFloaterStyle_Emote},
    { "Info",                kFloaterStyle_Info},
    { "AFK",                kFloaterStyle_AFK},
    { "DeathRattle",        kFloaterStyle_DeathRattle},
    { "Caption",            kFloaterStyle_Caption},
    DEFINE_END
};

//void SendPlayerAttentionMessageWithColor( TEAM team, STRING message, NUMBER color, int alertType ) - actually TEAM not ENTITY
// alertType enum is EFloaterStyle entity_enum.h
static int l_SendPlayerAttentionMessageWithColor (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    STRING message = luaL_checkstring(L, 2);
    //U32 color = lua_tounsigned(L, 3);            //Lua 5.2        // if unspecified, default is 0xffff00ff (RGBA yellow), maybe use smf_ConvertColorNameToValue in the future
    U32 color = lua_tonumber(L, 3);                //Lua 5.1        // if unspecified, default is 0xffff00ff (RGBA yellow), maybe use smf_ConvertColorNameToValue in the future
    STRING alertTypeString = lua_tostring(L, 4);        // if unspecified, default is kFloaterStyle_Attention
    int alertType;

    if (color == 0)
        color = 0xffff00ff;

    if(alertTypeString && alertTypeString[0])
    {
        alertType = StaticDefineIntLookupInt(ParseAlertTypes, alertTypeString);
        if(alertType < 0)
            alertType = kFloaterStyle_Attention;
    }
    else
    {
        alertType = kFloaterStyle_Attention;
    }

    SendPlayerAttentionMessageWithColor(team, message, color, alertType);
    return 0;
}

//void SendPlayerCaption( TEAM team, STRING message ) - actually TEAM not ENTITY
static int l_SendPlayerCaption (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    STRING message = luaL_checkstring(L, 2);
    SendPlayerCaption(team, message);
    return 0;
}

//void SendPlayerPopHelp( ENTITY player, STRING popHelpMessage );
static int l_SendPlayerPopHelp (lua_State *L)
{
    TEAM team = luaL_checkstring(L, 1);
    STRING message = luaL_checkstring(L, 2);
    SendPlayerPopHelp(team, message);
    return 0;
}

//void SendPlayerPopHelpReset( ENTITY player );
static int l_SendPlayerPopHelpReset (lua_State *L)
{
    TEAM team = luaL_checkstring(L, 1);
    SendPlayerPopHelpReset(team);
    return 0;
}

// ScriptedZoneEvent.c
extern StaticDefineInt ParseSoundChannelEnum[];

//void SendPlayerSound( TEAM team, STRING sound, NUMBER channel, FRACTION volumeLevel ) - actually TEAM not ENTITY
static int l_SendPlayerSound (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    STRING sound = luaL_checkstring(L, 2);
    STRING channelString = lua_tostring(L, 3);        // if unspecified, default is SOUND_MUSIC
    FRACTION volumeLevel = luaL_checknumber(L, 4);
    NUMBER channel;

    if(channelString && channelString[0])
    {
        channel = StaticDefineIntLookupInt(ParseSoundChannelEnum, channelString);
        if(channel < 0)
            channel = SOUND_MUSIC;
    }
    else
    {
        channel = SOUND_MUSIC;
    }

    SendPlayerSound(team, sound, channel, volumeLevel);
    return 0;
}


//void SendPlayerFadeSound( TEAM team, NUMBER channel, FRACTION fadeTime ) - actually TEAM not ENTITY
static int l_SendPlayerFadeSound (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    STRING channelString = lua_tostring(L, 2);        // if unspecified, default is SOUND_MUSIC
    FRACTION fadeTime = luaL_checknumber(L, 3);
    NUMBER channel;

    if(channelString && channelString[0])
    {
        channel = StaticDefineIntLookupInt(ParseSoundChannelEnum, channelString);
        if(channel < 0)
            channel = SOUND_MUSIC;
    }
    else
    {
        channel = SOUND_MUSIC;
    }

    SendPlayerFadeSound(team, channel, fadeTime);
    return 0;
}

//void ResetPlayerMusic(TEAM team)
static int l_ResetPlayerMusic (lua_State *L) {
    TEAM team = luaL_checkstring(L, 1);
    ResetPlayerMusic(team);
    return 0;
}

const luaL_Reg scriptMessageLib [] = 
{
    {"DebugPrint",            l_DebugPrint},
    {"SZELog",                l_TeamSZERewardLogMsg},
    {"Floater",                l_SendPlayerAttentionMessageWithColor},
    {"Caption",                l_SendPlayerCaption},
    {"PopHelp",                l_SendPlayerPopHelp},
    {"PopHelpReset",        l_SendPlayerPopHelpReset},
    {"Sound",                l_SendPlayerSound},
    {"FadeSound",            l_SendPlayerFadeSound},
    {"ResetMusic",            l_ResetPlayerMusic},
    {NULL, NULL}  //sentinel - REQUIRED
};
